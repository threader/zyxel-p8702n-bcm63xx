/******************************************************************************
 * 
 * FILE NAME:   stun_entry_point.c
 * 
 * DESCRIPTION: This file contains the entry point function and the utitlity 
 *              function called by the entry point function to call the API 
 *              handlers.
 *              
 * REVISION HISTORY:    
 *      Date            Name            Reference       Comments
 *  20-June 2006	Abhishek Chhibber    LLD			Initial Version	
 *  18-July-2006    Anuradha Gupta       LLD            Coding
 *	24-Nov-2006	    Anuradha Gupta       LLD			Coding
 *  14-May-2010	    Preksha Gupta		SPR 20375	    Merged CSR 1-7845448
 *
 * Copyright 2010, Aricent                   
 * ******************************************************************************/     

#include "nat_common_internal_api_id.h"
#include "nat_common_internal_api.h"
#include "stun_macros.h"
#include "stun_defs.h"
#include "stun_types.h"
#include "stun_prototypes.h"
#include "nat_port_pltfm_prototypes.h"
#include "nat_error_id.h"
#include "nat_glb_mgmt_macro.h"

/*******************************************************************************
 *
 * FUNCTION:        stun_init
 *
 * DESCRIPTION:     This is the initialization function of STUN State Machine
 *                  
 * RETURNS:         NAT_RETURN_SUCCESS in case of success and NAT_RETURN_FAILURE 
 *                  in case of failure.                 
 *
 ******************************************************************************/
nat_return_t stun_init(
		INOUT   nat_internal_msg_st        *p_internal_msg)
        
{
    nat_return_t        ret_val = NAT_RETURN_SUCCESS;
    nat_glb_pdb_st		*p_glb_pdb=p_internal_msg->msg_hdr.p_glb_pdb;
    nat_error_t			*p_ecode=&(p_internal_msg->msg_hdr.ecode);
	stun_data_st		*p_stun_data = NAT_NULL;
	stun_init_req_st	*p_init_req=(stun_init_req_st *) 
                              p_internal_msg->p_msg_data;
	nat_uint8_t		    i=0, j=0;

    /* Allocate memory to table */
   	p_glb_pdb->p_glb_data->p_stun_context_map =  nat_port_static_memget(
        (nat_port_sizeof(nat_context_data_map_st))*(p_init_req->max_contexts+3),p_ecode);
   
	if(NAT_NULL ==  p_glb_pdb->p_glb_data->p_stun_context_map)
	{
		NAT_PRINT(("STUN_INIT : memory allocation failure\n"));
		*p_ecode = NAT_ERR_MEM_ALLOC_FAILURE;
		return NAT_RETURN_FAILURE;
	} 

    nat_port_memset(p_glb_pdb->p_glb_data->p_stun_context_map,0,
        (nat_port_sizeof(nat_context_data_map_st)*(p_init_req->max_contexts)+3));

    for(i=0;i < (p_init_req->max_contexts+3); i++)
    {
        p_glb_pdb->p_glb_data->p_stun_context_map[i].is_free = NAT_TRUE;
    }
    

    p_glb_pdb->p_glb_data->max_contexts=p_init_req->max_contexts;

    /* Allocate memory to table */
    p_stun_data = (stun_data_st *)nat_port_static_memget(
                                     nat_port_sizeof(stun_data_st),p_ecode);
    
    /* Initilize the table */
    /* Assign the function names in the FSM table for the events recieved when 
     * the system is in IDLE state
     */  
  
    for(i=0;i<NAT_STUN_MAX_STATES;i++)
    {
    	for(j=0;j< NAT_STUN_MAX_EVENTS;j++)
    	{
    		p_stun_data->stun_fsm_table[i][j]= stun_unexpected_event;
    	}
    }
	/* Binding req handler will be called */      
    p_stun_data->stun_fsm_table[NAT_STUN_00_IDLE]
       [NAT_STUN_BINDING_REQ_EVENT] = stun_00_binding_request;
    
    	/* Refresh Timer expiry handler will be called */

	p_stun_data->stun_fsm_table[NAT_STUN_00_IDLE]
       [NAT_STUN_TIMER_EXPIRY_IND_EVENT] = stun_00_refresh_timer_expiry;

    p_stun_data->stun_fsm_table[NAT_STUN_00_IDLE]
       [NAT_STUN_RELEASE_REQ_EVENT] = stun_00_release_req;

    p_stun_data->stun_fsm_table[NAT_STUN_01_AWT_BIND_RESP]
       [NAT_STUN_TIMER_EXPIRY_IND_EVENT] = stun_01_retransmit_timer_expiry;

    p_stun_data->stun_fsm_table[NAT_STUN_01_AWT_BIND_RESP]
       [NAT_STUN_BINDING_RESP_EVENT] = stun_01_binding_resp;

    p_stun_data->stun_fsm_table[NAT_STUN_01_AWT_BIND_RESP]
       [NAT_STUN_RELEASE_REQ_EVENT] = stun_01_release_req;

    p_stun_data->stun_fsm_table[NAT_STUN_02_AWT_EXTRA_BIND_RESP]
       [NAT_STUN_TIMER_EXPIRY_IND_EVENT] = stun_02_timer_expiry;

    p_stun_data->stun_fsm_table[NAT_STUN_02_AWT_EXTRA_BIND_RESP]
       [NAT_STUN_BINDING_RESP_EVENT] = stun_02_binding_resp;

    p_stun_data->stun_fsm_table[NAT_STUN_02_AWT_EXTRA_BIND_RESP]
       [NAT_STUN_RELEASE_REQ_EVENT] = stun_02_release_req;

    p_stun_data->stun_fsm_table[NAT_STUN_03_AWT_START_TIMER_RESP]
       [NAT_STUN_TIMER_START_RESP_EVENT] = stun_03_start_timer_resp;

    p_stun_data->stun_fsm_table[NAT_STUN_03_AWT_START_TIMER_RESP]
       [NAT_STUN_BINDING_RESP_EVENT] = stun_03_binding_resp;

    p_stun_data->stun_fsm_table[NAT_STUN_03_AWT_START_TIMER_RESP]
       [NAT_STUN_RELEASE_REQ_EVENT] = stun_03_release_req;

    p_stun_data->stun_fsm_table[NAT_STUN_04_AWT_BIND_RESP_AFTER_ACK]
       [NAT_STUN_BINDING_RESP_EVENT] = stun_04_binding_resp;

    p_stun_data->stun_fsm_table[NAT_STUN_04_AWT_BIND_RESP_AFTER_ACK]
       [NAT_STUN_TIMER_EXPIRY_IND_EVENT] = stun_04_timer_expiry;

    p_stun_data->stun_fsm_table[NAT_STUN_04_AWT_BIND_RESP_AFTER_ACK]
       [NAT_STUN_RELEASE_REQ_EVENT] = stun_04_release_req;

    p_stun_data->p_stun_contexts = (stun_context_st *)nat_port_static_memget(
          nat_port_sizeof(stun_context_st)*(p_init_req->max_contexts + 3),p_ecode);

	for(i=0;i<p_init_req->max_contexts;i++)
		NAT_STUN_INIT_CONTEXT(p_stun_data->p_stun_contexts[i]);
    
    p_glb_pdb->p_glb_data->p_stun_data = p_stun_data;
    
    return ret_val;    

}


/*******************************************************************************
 *
 * FUNCTION:        stun_api_to_event
 *
 * DESCRIPTION:     This function maps the messages recieved to events.
 *                  
 * RETURNS:         NAT_RETURN_SUCCESS in case of success and NAT_RETURN_FAILURE 
 *                  in case of failure.                 
 *
 ******************************************************************************/

nat_return_t stun_api_to_event(
        INOUT stun_pdb_st             *p_stun_pdb)
{
    nat_return_t		ret_val = NAT_RETURN_SUCCESS;

    switch(p_stun_pdb->p_internal_msg->msg_hdr.msg_id)
    {     
        case NAT_INT_STUN_GET_PUBLIC_IP_PORT_REQ:
            NAT_STUN_SET_EVENT(p_stun_pdb,NAT_STUN_BINDING_REQ_EVENT)
            break;

        case NAT_INT_START_TIMER_RESP:
            NAT_STUN_SET_EVENT(p_stun_pdb,NAT_STUN_TIMER_START_RESP_EVENT)
            break;
                   
        case NAT_INT_TIMER_EXPIRY_IND:    
           NAT_STUN_SET_EVENT(p_stun_pdb,NAT_STUN_TIMER_EXPIRY_IND_EVENT)
           break;

        case NAT_INT_STUN_REL_PUBLIC_IP_PORT_REQ:    
           NAT_STUN_SET_EVENT(p_stun_pdb,NAT_STUN_RELEASE_REQ_EVENT)
           break;

	    case NAT_INT_STUN_BINDING_RESP:    
           NAT_STUN_SET_EVENT(p_stun_pdb,NAT_STUN_BINDING_RESP_EVENT)
           break;

        default:
    		/*NAT_STUN_SET_ECODE(p_stun_shsec_pdb,NAT_INVALID_MSG_ID_RECD)*/
		   ret_val = NAT_RETURN_FAILURE;
           break;

    }
    
   return ret_val;
}


/*******************************************************************************
 *
 * FUNCTION:        nat_stun_process_mesg
 *
 * DESCRIPTION:     This is the entry point function of STUN state machine.  
 *			  The function checks the API Id in the message 
 *                  received and invokes appropriate handler based on current 
 *                  stun sm state and the event (mapped from API Id).
 *                  
 * RETURNS:         NAT_RETURN_SUCCESS in case of success and NAT_RETURN_FAILURE 
 *                  in case of failure.                 
 *
 ******************************************************************************/

nat_return_t nat_stun_process_mesg(
        INOUT nat_internal_msg_st        *p_internal_msg)
{
    nat_return_t         ret_val=NAT_RETURN_SUCCESS;
    stun_pdb_st          stun_pdb;
    stun_shsec_resp_st   *p_shsec_resp = NAT_NULL;
    nat_uint8_t		 context_id=0;
   	stun_context_st     *p_stun_context = NAT_NULL;
   	
	if(NAT_INT_STUN_INIT_REQ == p_internal_msg->msg_hdr.msg_id)
	{
          ret_val = stun_init(p_internal_msg);
    }
    else if(NAT_INT_STUN_DEINIT_REQ == p_internal_msg->msg_hdr.msg_id)
	{
         stun_deinit(p_internal_msg);
    }  
    else
    {
        /*Initialize the stun_pdb structure*/
        stun_pdb.p_glb_pdb = p_internal_msg->msg_hdr.p_glb_pdb;

        stun_pdb.p_internal_msg = p_internal_msg;

        stun_pdb.p_ecode =  &(stun_pdb.p_internal_msg->msg_hdr.ecode);
        
		nat_port_memcpy(&context_id,p_internal_msg->p_msg_data,
               nat_port_sizeof(nat_uint8_t));

		stun_pdb.p_stun_context= &(((stun_data_st *)stun_pdb.p_glb_pdb->
                        p_glb_data->p_stun_data)->p_stun_contexts[context_id]);
  
        p_stun_context = stun_pdb.p_stun_context;
                        
        
		     
        if(NAT_INT_STUN_SHSEC_RESP == p_internal_msg->msg_hdr.msg_id)
        {
             /* store the shared secret username & password in STUN_context */
             p_shsec_resp = (stun_shsec_resp_st *)p_internal_msg->p_msg_data;
             	
             if( NAT_STUN_00_IDLE == p_stun_context->current_state)
             {              
                 nat_port_strcpy(p_stun_context->shared_secret_username.str,
                             p_shsec_resp->shared_secret_username.str);
                 p_stun_context->shared_secret_username.str_len =
                             p_shsec_resp->shared_secret_username.str_len;
                             
                 nat_port_strcpy(p_stun_context->shared_secret_password.str,
                             p_shsec_resp->shared_secret_password.str);
                 p_stun_context->shared_secret_password.str_len =
                             p_shsec_resp->shared_secret_password.str_len;
             }
             else if(NAT_STUN_01_AWT_BIND_RESP == p_stun_context->current_state)
             {
                  /* here first compare the username and password 
                   * if it fails, set the appropriate error code to be 
                   * interpreted at calling place and then store the new
                   * username and password in the stun context 
                   */
                  if(nat_port_strcmp(p_stun_context->shared_secret_username.str,
                         p_shsec_resp->shared_secret_username.str))
                  {
                         stun_pdb.p_glb_pdb->p_glb_data->ecode =
                                           NAT_ERR_SHSEC_MISMATCH;
                         ret_val = NAT_RETURN_FAILURE;  
                  }
                  else if(nat_port_strcmp(p_stun_context->shared_secret_password.str,
                         p_shsec_resp->shared_secret_password.str))
                  {
                         stun_pdb.p_glb_pdb->p_glb_data->ecode =
                                           NAT_ERR_SHSEC_MISMATCH;
                         ret_val = NAT_RETURN_FAILURE;      
                  }
                  
                 nat_port_strcpy(p_stun_context->shared_secret_username.str,
                             p_shsec_resp->shared_secret_username.str);
                 p_stun_context->shared_secret_username.str_len =
                             p_shsec_resp->shared_secret_username.str_len;
                             
                 nat_port_strcpy(p_stun_context->shared_secret_password.str,
                             p_shsec_resp->shared_secret_password.str);
                 p_stun_context->shared_secret_password.str_len =
                             p_shsec_resp->shared_secret_password.str_len;
             }               
        }
        else
        {
    		ret_val = stun_api_to_event(&stun_pdb);
    
    		if(NAT_RETURN_SUCCESS == ret_val)
    		{
                ret_val = ((stun_data_st *)stun_pdb.p_glb_pdb->p_glb_data->
                    p_stun_data)->stun_fsm_table[stun_pdb.p_stun_context->
                    current_state][stun_pdb.p_stun_context->current_event]
                    (&stun_pdb);
            }
        }
    }
	    
    return ret_val;
}/* End function */

/*******************************************************************************
 *
 * FUNCTION:        stun_deinit
 *
 * DESCRIPTION:     This is the deinitialization function of STUN State Machine
 *                  
 * RETURNS:         NAT_RETURN_SUCCESS in case of success and NAT_RETURN_FAILURE 
 *                  in case of failure.                 
 *
 ******************************************************************************/
nat_void_t stun_deinit(
      INOUT nat_internal_msg_st        *p_internal_msg)     
{
    stun_pdb_st          stun_pdb;
    nat_uint8_t          i = 0;
    nat_glb_pdb_st		*p_glb_pdb=p_internal_msg->msg_hdr.p_glb_pdb;
    nat_error_t          ecode;
    stun_rel_public_ip_port_req_st    rel_req;
 
    stun_pdb.p_glb_pdb = p_glb_pdb;
    stun_pdb.p_internal_msg = p_internal_msg;
 
    for(i = 3; i < p_glb_pdb->p_glb_data->max_contexts +3; i++)
    {
          if(NAT_FALSE == p_glb_pdb->p_glb_data->p_stun_context_map[i].is_free)
          {
              p_internal_msg->p_msg_data = (nat_uint8_t *)(&rel_req);
              rel_req = i;
              stun_pdb.p_stun_context= &(((stun_data_st *)p_glb_pdb->
                    p_glb_data->p_stun_data)->p_stun_contexts[i]);
              
              ((stun_data_st *)p_glb_pdb->p_glb_data->
                    p_stun_data)->stun_fsm_table[stun_pdb.p_stun_context->
                    current_state][NAT_STUN_RELEASE_REQ_EVENT](&stun_pdb);
          }
          
          NAT_GLB_DEALLOC_CONTEXT(p_glb_pdb->p_glb_data->p_stun_context_map[i])
    }
    
    nat_port_static_memfree(((stun_data_st *)p_glb_pdb->
                    p_glb_data->p_stun_data)->p_stun_contexts, &ecode);
                    
    nat_port_static_memfree(p_glb_pdb->p_glb_data->p_stun_data, &ecode);
    p_glb_pdb->p_glb_data->p_stun_data = NAT_NULL;                
    return;
}

