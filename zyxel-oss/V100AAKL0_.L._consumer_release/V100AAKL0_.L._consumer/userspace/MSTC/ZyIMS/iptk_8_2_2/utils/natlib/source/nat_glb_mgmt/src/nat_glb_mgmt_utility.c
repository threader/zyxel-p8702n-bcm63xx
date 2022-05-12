/******************************************************************************
 * 
 * FILE NAME:   nat_glb_mgmt_utility.c
 * 
 * DESCRIPTION: This file contains the utility functions needed by NAT Client 
 *				Manager
  *              
 * REVISION HISTORY:    
 *              Date            Name            Reference       Comments
 *          22-June-2006	Abhishek Chhibber       LLD         Initial Version
 *	    10-July-2006	Anuradha Gupta		LLD	Coding 
 *      02-Mar-2009     Alok Tiwari         IPTK Rel8.0    IPTK Open source
 *                                                 replacement from IPTK :-
 *                                          invokation of stun_enc_dec APIs      
 *                                          replaced with arStunEncDec APIs.
 *      14-Jul-2009     Rajiv Kumar         IPTK Rel8.2   Vxwork 5.4 Porting
 *                                                        
 *
 ******************************************************************************/       

#include "arStunEncDec_api.h"
#include "nat_common_internal_api_id.h"
#include "nat_api.h"
#include "stun_shsec_prototypes.h"
#include "nat_glb_mgmt_prototypes.h"
#include "nat_error_id.h"
#include "stun_prototypes.h"
#include "nat_common_internal_api.h"
#include "nat_port_pltfm_prototypes.h"
#include "stun_lifetime_prototypes.h"
#include "stun_discovery_prototypes.h"
#include "nat_port_prototypes.h"
#include "nat_glb_mgmt_macro.h"
#include "nat_common_defs.h"
#include "nat_port_pltfm_prototypes.h"

/******************************************************************************
 *
 * FUNCTION:        nat_stun_init_req
 *
 * DESCRIPTION:     This function is invoked whenever stun init request
 *                  is received from application.
 *                   
 * RETURNS:         NAT_RETURN_SUCCESS in case of success and NAT_RETURN_FAILURE in 
 *                  case of any failure.       
 *
 ******************************************************************************/
nat_return_t nat_stun_init_req(
        INOUT nat_glb_pdb_st             *p_glb_nat_pdb)
{
	nat_return_t      	ret_val = NAT_RETURN_SUCCESS;
	
	nat_stun_init_req_st 	*p_stun_init_req = (nat_stun_init_req_st *)
		(p_glb_nat_pdb->p_recv_msg->p_msg->p_msg_data);
    nat_msg_hdr_st  msg_hdr = p_glb_nat_pdb->p_recv_msg->p_msg->msg_hdr;
	
	nat_internal_msg_st     	internal_msg;
	
	stun_shsec_init_req_st		shsec_init_req;
	stun_discovery_init_req_st 	discovery_init_req;
	stun_lifetime_init_req_st	lifetime_init_req;
	
	nat_error_t		ecode = NAT_ERR_UNDEFINED;

	if(NAT_NULL != p_glb_nat_pdb->p_glb_data->p_stun_context_map)
	{
		p_glb_nat_pdb->p_glb_data->ecode = NAT_ERR_STUN_INIT_ALREADY_DONE;
                ret_val = NAT_RETURN_FAILURE;
        }	
	else if(NAT_INVALID_CONTEXT_VALUE == p_stun_init_req->max_contexts)
	{
		p_glb_nat_pdb->p_glb_data->ecode = NAT_ERR_STUN_INVALID_VALUE;
		ret_val = NAT_RETURN_FAILURE;
	}
	else if(NAT_RETURN_SUCCESS != nat_stun_send_stun_init_req(p_glb_nat_pdb))
	{
		ret_val = NAT_RETURN_FAILURE;
	}
	
	/* if no procedure execution request is there then just send the init 
	   response back */
	else if(NAT_STUN_PROC_NOT_PRESENT == p_stun_init_req->start_procedures)
	{
		ret_val = NAT_RETURN_SUCCESS;
	}
	else if(NAT_STUN_PROC_SHARED_SECRET_SUPP == 
		(NAT_STUN_PROC_SHARED_SECRET_SUPP & p_stun_init_req->start_procedures))
	{
		if(NAT_FALSE == p_glb_nat_pdb->p_glb_data->p_stun_config_data->shsec_enabled)
        {
            p_glb_nat_pdb->p_glb_data->ecode = NAT_ERR_STUN_INVALID_PROC;
            ret_val = nat_stun_send_init_resp_err(p_glb_nat_pdb);
            return ret_val;
        }

		/* Firstly initialize the Shared Secret State machine */
		internal_msg.msg_hdr.p_glb_pdb = p_glb_nat_pdb;
		internal_msg.msg_hdr.ecode = ecode;
		internal_msg.msg_hdr.msg_id = NAT_INT_STUN_SHSEC_INIT_REQ;
		internal_msg.p_msg_data = (nat_uint8_t *)(&shsec_init_req);
		
		shsec_init_req.timer_dur = p_glb_nat_pdb->p_glb_data->\
			p_stun_config_data->shsec_refresh_timer;
		
		if(NAT_RETURN_SUCCESS != nat_stun_shsec_process_mesg(&internal_msg))
		{
			p_glb_nat_pdb->p_glb_data->ecode = internal_msg.msg_hdr.ecode;
			ret_val = NAT_RETURN_FAILURE;
		}
        else
        {
            
            /* now send the trigger to start the procedure execution */
            internal_msg.msg_hdr.p_glb_pdb = p_glb_nat_pdb;
            internal_msg.msg_hdr.ecode = ecode;
            internal_msg.msg_hdr.msg_id = NAT_INT_STUN_SHSEC_REQ;
            internal_msg.msg_hdr.payload_length = 0;
            
            if(NAT_RETURN_SUCCESS != nat_stun_shsec_process_mesg(&internal_msg))
            {
                p_glb_nat_pdb->p_glb_data->ecode = internal_msg.msg_hdr.ecode;
                ret_val = NAT_RETURN_FAILURE;
            }
            else
            {
                
                p_glb_nat_pdb->p_glb_data->
                    p_stun_context_map[NAT_STUN_MODULE_SHSEC].app_context_id = \
                    msg_hdr.app_context_id;
                
                p_glb_nat_pdb->p_glb_data->p_stun_context_map[NAT_STUN_MODULE_SHSEC].\
                    module_id = NAT_STUN_MODULE_EXTERNAL;
                
                p_glb_nat_pdb->p_glb_data->shared_secret_proc_status = \
                    NAT_STUN_SHARED_SECRET_ONGOING;
                
                if(NAT_STUN_PROC_BINDING_LIFETIME_SUPP ==
                    (NAT_STUN_PROC_BINDING_LIFETIME_SUPP & p_stun_init_req->start_procedures))
                {
                    p_glb_nat_pdb->p_glb_data->p_stun_context_map[NAT_STUN_MODULE_LIFETIME].
                        app_context_id = msg_hdr.app_context_id;
                    
                    p_glb_nat_pdb->p_glb_data->p_stun_context_map[NAT_STUN_MODULE_LIFETIME].\
                        module_id = NAT_STUN_MODULE_EXTERNAL;
                    
                    p_glb_nat_pdb->p_glb_data->bind_lifetime_disc_status = \
                        NAT_STUN_BINDING_LIFETIME_PENDING;
                }
                
                if(NAT_STUN_PROC_TYPE_DISCOVERY_SUPP ==
                    (NAT_STUN_PROC_TYPE_DISCOVERY_SUPP & p_stun_init_req->start_procedures))
                {
                    p_glb_nat_pdb->p_glb_data->
                        p_stun_context_map[NAT_STUN_MODULE_DISCOVERY].app_context_id = \
						msg_hdr.app_context_id;
                    
                    p_glb_nat_pdb->p_glb_data->p_stun_context_map[NAT_STUN_MODULE_DISCOVERY].\
                        module_id = NAT_STUN_MODULE_EXTERNAL;
                    
                    p_glb_nat_pdb->p_glb_data->nat_type_disc_status = \
                        NAT_STUN_NAT_TYPE_DISCOVERY_PENDING;
                }
            }
        }
	}
	else
    {
        if(NAT_STUN_PROC_BINDING_LIFETIME_SUPP ==
            (NAT_STUN_PROC_BINDING_LIFETIME_SUPP & p_stun_init_req->start_procedures))
        {
            if(NAT_FALSE == p_glb_nat_pdb->p_glb_data->p_stun_config_data->lifetime_disc_enabled)
            {
                p_glb_nat_pdb->p_glb_data->ecode = NAT_ERR_STUN_INVALID_PROC;
                ret_val = nat_stun_send_init_resp_err(p_glb_nat_pdb);
                return ret_val;
            }

            /* Firstly initialize the Shared Secret State machine */
            internal_msg.msg_hdr.p_glb_pdb = p_glb_nat_pdb;
            internal_msg.msg_hdr.ecode = ecode;
            internal_msg.msg_hdr.msg_id = NAT_INT_STUN_LIFETIME_INIT_REQ;
            internal_msg.p_msg_data = (nat_uint8_t *)(&lifetime_init_req);
            
            lifetime_init_req.timer_dur = p_glb_nat_pdb->p_glb_data->\
                p_stun_config_data->lifetime_disc_timer;
            
            lifetime_init_req.delta_interval = p_glb_nat_pdb->p_glb_data->\
                p_stun_config_data->lifetime_delta_timer;
            
            if(NAT_RETURN_SUCCESS != nat_stun_lifetime_process_mesg(&internal_msg))
            {
                p_glb_nat_pdb->p_glb_data->ecode = internal_msg.msg_hdr.ecode;
                ret_val = NAT_RETURN_FAILURE;
            }
            else
            {
                
                /* now send the trigger to start the procedure execution */
                internal_msg.msg_hdr.p_glb_pdb = p_glb_nat_pdb;
                internal_msg.msg_hdr.ecode = ecode;
                internal_msg.msg_hdr.msg_id = NAT_INT_STUN_LIFETIME_DISC_REQ;
                internal_msg.msg_hdr.payload_length = 0;
                
                if(NAT_RETURN_SUCCESS != nat_stun_lifetime_process_mesg(&internal_msg))
                {
                    p_glb_nat_pdb->p_glb_data->ecode = internal_msg.msg_hdr.ecode;
                    ret_val = NAT_RETURN_FAILURE;
                }
                else
                {
                    p_glb_nat_pdb->p_glb_data->p_stun_context_map[NAT_STUN_MODULE_LIFETIME].\
                        app_context_id = msg_hdr.app_context_id;
                    
                    p_glb_nat_pdb->p_glb_data->p_stun_context_map[NAT_STUN_MODULE_LIFETIME].\
                        module_id = NAT_STUN_MODULE_EXTERNAL;
                    
                    p_glb_nat_pdb->p_glb_data->bind_lifetime_disc_status = \
                        NAT_STUN_BINDING_LIFETIME_ONGOING;
                }
            }            
        }
        
        if(NAT_STUN_PROC_TYPE_DISCOVERY_SUPP ==
            (NAT_STUN_PROC_TYPE_DISCOVERY_SUPP & p_stun_init_req->start_procedures))
        {
            /* Firstly initialize the Shared Secret State machine */
            internal_msg.msg_hdr.p_glb_pdb = p_glb_nat_pdb;
            internal_msg.msg_hdr.ecode = ecode;
            internal_msg.msg_hdr.msg_id = NAT_INT_STUN_DISCOVERY_INIT_REQ;
            internal_msg.p_msg_data = (nat_uint8_t *)(&discovery_init_req);
            
            discovery_init_req.timer_dur = p_glb_nat_pdb->p_glb_data->\
                p_stun_config_data->type_disc_timer;
            
            if(NAT_RETURN_SUCCESS != nat_stun_discovery_process_mesg(&internal_msg))
            {
                p_glb_nat_pdb->p_glb_data->ecode = internal_msg.msg_hdr.ecode;
                ret_val = NAT_RETURN_FAILURE;
            }
            else
            {
                
                /* now send the trigger to start the procedure execution */
                internal_msg.msg_hdr.p_glb_pdb = p_glb_nat_pdb;
                internal_msg.msg_hdr.ecode = ecode;
                internal_msg.msg_hdr.msg_id = NAT_INT_STUN_DISCOVERY_TYPE_REQ;
                internal_msg.msg_hdr.payload_length = 0;
                
                if(NAT_RETURN_SUCCESS != nat_stun_discovery_process_mesg(&internal_msg))
                {
                    p_glb_nat_pdb->p_glb_data->ecode = internal_msg.msg_hdr.ecode;
                    ret_val = NAT_RETURN_FAILURE;
                    
                }
                else
                {
                    
                    p_glb_nat_pdb->p_glb_data->
                        p_stun_context_map[NAT_STUN_MODULE_DISCOVERY].app_context_id = \
						msg_hdr.app_context_id;
                    
                    p_glb_nat_pdb->p_glb_data->p_stun_context_map[NAT_STUN_MODULE_DISCOVERY].\
                        module_id = NAT_STUN_MODULE_EXTERNAL;
                    
                    p_glb_nat_pdb->p_glb_data->nat_type_disc_status = \
                        NAT_STUN_NAT_TYPE_DISCOVERY_ONGOING;
                }
            }
        }
        
    }
	/* generate response here */
	if(NAT_RETURN_FAILURE == ret_val)
	{
		ret_val = nat_stun_send_init_resp_err(p_glb_nat_pdb);
	}
	else
	{
		ret_val = nat_stun_send_init_resp(p_glb_nat_pdb);
	}
	
	return ret_val;
}

/******************************************************************************
 *
 * FUNCTION:        nat_stun_send_init_resp
 *
 * DESCRIPTION:     This function is invoked to send a successful init response
 *                  to the application.
 *      
 * RETURNS:         NAT_RETURN_SUCCESS in case of success and NAT_RETURN_FAILURE in
 *                  case of any failure.
 *
 ******************************************************************************/
nat_return_t nat_stun_send_init_resp(
        INOUT nat_glb_pdb_st             *p_glb_nat_pdb)
{
	nat_return_t      	ret_val = NAT_RETURN_SUCCESS;
	nat_error_t		ecode = NAT_ERR_NONE;
	nat_msg_st		*p_init_resp_msg = NAT_NULL;
	nat_stun_init_resp_st	*p_stun_init_resp = NAT_NULL;
	nat_msg_hdr_st    	recv_msg_hdr = p_glb_nat_pdb->p_recv_msg->p_msg->msg_hdr;

	p_init_resp_msg = (nat_msg_st *)nat_port_static_memget
		(nat_port_sizeof(nat_msg_hdr_st) + nat_port_sizeof(nat_stun_init_resp_st),
		&ecode);
	
	p_init_resp_msg->msg_hdr.api_id = NAT_STUN_INIT_RESP;
	p_init_resp_msg->msg_hdr.app_context_id = recv_msg_hdr.app_context_id;
	p_init_resp_msg->msg_hdr.source_id = recv_msg_hdr.destination_id;
	p_init_resp_msg->msg_hdr.destination_id = recv_msg_hdr.source_id;
	p_init_resp_msg->msg_hdr.api_length = nat_port_sizeof(nat_msg_hdr_st) + \
					       nat_port_sizeof(nat_stun_init_resp_st);	

	p_stun_init_resp = (nat_stun_init_resp_st *)p_init_resp_msg->p_msg_data;

	p_stun_init_resp->result = NAT_RESULT_SUCCESS;
	p_stun_init_resp->error_cause = NAT_ERR_NONE;

	ret_val = nat_port_send_msg_to_app(p_glb_nat_pdb,p_init_resp_msg);

	nat_port_static_memfree(p_init_resp_msg,&ecode);
	return ret_val;

}

/******************************************************************************
 *
 * FUNCTION:        nat_stun_send_init_resp_err
 *
 * DESCRIPTION:     This function is invoked to send a failure init response
 *                  to the application.
 *      
 * RETURNS:         NAT_RETURN_SUCCESS in case of success and NAT_RETURN_FAILURE in
 *                  case of any failure.
 *
 ******************************************************************************/
nat_return_t nat_stun_send_init_resp_err(
        INOUT nat_glb_pdb_st             *p_glb_nat_pdb)
{
	nat_return_t      	ret_val = NAT_RETURN_SUCCESS;
	nat_msg_st		*p_init_resp_msg = NAT_NULL;
	nat_stun_init_resp_st	*p_stun_init_resp = NAT_NULL;
	nat_msg_hdr_st    	recv_msg_hdr = p_glb_nat_pdb->p_recv_msg->p_msg->msg_hdr;
	nat_error_t		ecode = NAT_ERR_NONE;
	
	p_init_resp_msg = (nat_msg_st *)nat_port_static_memget
		(nat_port_sizeof(nat_msg_hdr_st) + nat_port_sizeof(nat_stun_init_resp_st),
		&ecode);

	p_init_resp_msg->msg_hdr.api_id = NAT_STUN_INIT_RESP;
	p_init_resp_msg->msg_hdr.app_context_id = recv_msg_hdr.app_context_id;
	p_init_resp_msg->msg_hdr.source_id = recv_msg_hdr.destination_id;
	p_init_resp_msg->msg_hdr.destination_id = recv_msg_hdr.source_id;
	p_init_resp_msg->msg_hdr.api_length = nat_port_sizeof(nat_msg_hdr_st) + \
					       nat_port_sizeof(nat_stun_init_resp_st);	

	p_stun_init_resp = (nat_stun_init_resp_st *)p_init_resp_msg->p_msg_data;

	p_stun_init_resp->result = NAT_RESULT_FAILURE;
	p_stun_init_resp->error_cause = p_glb_nat_pdb->p_glb_data->ecode;

	ret_val = nat_port_send_msg_to_app(p_glb_nat_pdb,p_init_resp_msg);

	nat_port_static_memfree(p_init_resp_msg,&ecode);

	return ret_val;
}

/******************************************************************************
 *
 * FUNCTION:        nat_stun_send_stun_init_req
 *
 * DESCRIPTION:     This function is invoked whenever stun init request
 *                  is received from application.
 *      
 * RETURNS:         NAT_RETURN_SUCCESS in case of success and NAT_RETURN_FAILURE in
 *                  case of any failure.
 *
 ******************************************************************************/
nat_return_t nat_stun_send_stun_init_req(
        INOUT nat_glb_pdb_st             *p_glb_nat_pdb)
{
	nat_return_t      	ret_val = NAT_RETURN_SUCCESS;
	nat_stun_init_req_st 	*p_stun_init_req = \
			(nat_stun_init_req_st *)(p_glb_nat_pdb->p_recv_msg->p_msg->p_msg_data);
        nat_internal_msg_st     internal_msg;
        nat_error_t             ecode = NAT_ERR_UNDEFINED;
	stun_init_req_st	stun_init_req;

	internal_msg.msg_hdr.p_glb_pdb = p_glb_nat_pdb;
        internal_msg.msg_hdr.ecode = ecode;
        internal_msg.msg_hdr.msg_id = NAT_INT_STUN_INIT_REQ;
	internal_msg.p_msg_data = (nat_uint8_t *)(&stun_init_req);

	stun_init_req.max_contexts = p_stun_init_req->max_contexts;

	if(NAT_RETURN_SUCCESS != nat_stun_process_mesg(&internal_msg))
        {
	      p_glb_nat_pdb->p_glb_data->ecode = internal_msg.msg_hdr.ecode;
              ret_val = NAT_RETURN_FAILURE;
              
         }

	return ret_val;
}


/******************************************************************************
 *
 * FUNCTION:        nat_stun_send_exec_proc_resp
 *
 * DESCRIPTION:     This function is invoked to send a successful exec proc response
 *                  to the application.
 *      
 * RETURNS:         NAT_RETURN_SUCCESS in case of success and NAT_RETURN_FAILURE in
 *                  case of any failure.
 *
 ******************************************************************************/
nat_return_t nat_stun_send_exec_proc_resp(
        INOUT nat_glb_pdb_st             *p_glb_nat_pdb)
{
	nat_return_t      	ret_val = NAT_RETURN_SUCCESS;
 	nat_error_t		ecode = NAT_ERR_NONE;	
	nat_msg_st		*p_exec_proc_resp_msg = NAT_NULL;
	nat_stun_exec_proc_resp_st	*p_stun_exec_proc_resp = NAT_NULL;
	nat_msg_hdr_st    	recv_msg_hdr = p_glb_nat_pdb->p_recv_msg->p_msg->msg_hdr;

	
	p_exec_proc_resp_msg = (nat_msg_st *)nat_port_static_memget
		(nat_port_sizeof(nat_msg_hdr_st) + 
		nat_port_sizeof(nat_stun_exec_proc_resp_st), &ecode);

	p_exec_proc_resp_msg->msg_hdr.api_id = NAT_STUN_EXEC_PROC_RESP;
	p_exec_proc_resp_msg->msg_hdr.app_context_id = recv_msg_hdr.app_context_id;
	p_exec_proc_resp_msg->msg_hdr.source_id = recv_msg_hdr.destination_id;
	p_exec_proc_resp_msg->msg_hdr.destination_id = recv_msg_hdr.source_id;
	p_exec_proc_resp_msg->msg_hdr.api_length = nat_port_sizeof(nat_msg_hdr_st) + \
					       nat_port_sizeof(nat_stun_exec_proc_resp_st);	

	p_stun_exec_proc_resp = (nat_stun_exec_proc_resp_st *)p_exec_proc_resp_msg->p_msg_data;

	p_stun_exec_proc_resp->result = NAT_RESULT_SUCCESS;
	p_stun_exec_proc_resp->error_cause = NAT_ERR_NONE;

	ret_val = nat_port_send_msg_to_app(p_glb_nat_pdb,p_exec_proc_resp_msg);

	nat_port_static_memfree(p_exec_proc_resp_msg,&ecode);
	return ret_val;
}

/******************************************************************************
 *
 * FUNCTION:        nat_stun_send_exec_proc_resp_err
 *
 * DESCRIPTION:     This function is invoked to send a failure exec proc response
 *                  to the application.
 *      
 * RETURNS:         NAT_RETURN_SUCCESS in case of success and NAT_RETURN_FAILURE in
 *                  case of any failure.
 *
 ******************************************************************************/
nat_return_t nat_stun_send_exec_proc_resp_err(
        INOUT nat_glb_pdb_st             *p_glb_nat_pdb)
{
	nat_return_t      	ret_val = NAT_RETURN_SUCCESS;
	nat_msg_st		*p_exec_proc_resp_msg = NAT_NULL;
	nat_stun_exec_proc_resp_st	*p_stun_exec_proc_resp = NAT_NULL;
	nat_msg_hdr_st    	recv_msg_hdr = p_glb_nat_pdb->p_recv_msg->p_msg->msg_hdr;
	nat_error_t		ecode = NAT_ERR_NONE;

	p_exec_proc_resp_msg = (nat_msg_st *)nat_port_static_memget
		(nat_port_sizeof(nat_msg_hdr_st) + 
		nat_port_sizeof(nat_stun_exec_proc_resp_st), &ecode);

	p_exec_proc_resp_msg->msg_hdr.api_id = NAT_STUN_EXEC_PROC_RESP;
	p_exec_proc_resp_msg->msg_hdr.app_context_id = recv_msg_hdr.app_context_id;
	p_exec_proc_resp_msg->msg_hdr.source_id = recv_msg_hdr.destination_id;
	p_exec_proc_resp_msg->msg_hdr.destination_id = recv_msg_hdr.source_id;
	p_exec_proc_resp_msg->msg_hdr.api_length = nat_port_sizeof(nat_msg_hdr_st) + \
					       nat_port_sizeof(nat_stun_exec_proc_resp_st);	

	p_stun_exec_proc_resp = (nat_stun_exec_proc_resp_st *)p_exec_proc_resp_msg->p_msg_data;

	p_stun_exec_proc_resp->result = NAT_RESULT_FAILURE;
	p_stun_exec_proc_resp->error_cause = p_glb_nat_pdb->p_glb_data->ecode;

	ret_val = nat_port_send_msg_to_app(p_glb_nat_pdb,p_exec_proc_resp_msg);

	nat_port_static_memfree(p_exec_proc_resp_msg,&ecode);
	return ret_val;
}

/******************************************************************************
 *
 * FUNCTION:        nat_stun_send_proc_status_ind
 *
 * DESCRIPTION:     This function is invoked to send a successful exec proc 
 *                  status indication to the application.
 *      
 * RETURNS:         NAT_RETURN_SUCCESS in case of success and NAT_RETURN_FAILURE in
 *                  case of any failure.
 *
 ******************************************************************************/
nat_return_t nat_stun_send_proc_status_ind(
        INOUT nat_glb_pdb_st            *p_glb_nat_pdb,
	IN nat_uint8_t          	procedure_name)
{
	nat_return_t      	ret_val = NAT_RETURN_SUCCESS;
	
	nat_msg_st		*p_proc_status_ind_msg = NAT_NULL;
	nat_error_t		ecode = NAT_ERR_NONE;
	nat_stun_proc_status_ind_st	*p_stun_proc_status_ind = NAT_NULL;
	nat_msg_hdr_st    	recv_msg_hdr = p_glb_nat_pdb->p_recv_msg->p_msg->msg_hdr;

	p_proc_status_ind_msg = (nat_msg_st *)nat_port_static_memget(
			nat_port_sizeof(nat_msg_hdr_st) +
			nat_port_sizeof(nat_stun_proc_status_ind_st),&ecode);

	p_proc_status_ind_msg->msg_hdr.api_id = NAT_STUN_PROC_STATUS_IND;
	p_proc_status_ind_msg->msg_hdr.app_context_id = recv_msg_hdr.app_context_id;
	p_proc_status_ind_msg->msg_hdr.source_id = recv_msg_hdr.destination_id;
	p_proc_status_ind_msg->msg_hdr.destination_id = recv_msg_hdr.source_id;
	p_proc_status_ind_msg->msg_hdr.api_length = nat_port_sizeof(nat_msg_hdr_st) + \
					       nat_port_sizeof(nat_stun_proc_status_ind_st);	

	p_stun_proc_status_ind = (nat_stun_proc_status_ind_st *)p_proc_status_ind_msg->p_msg_data;

	p_stun_proc_status_ind->result = NAT_RESULT_SUCCESS;
	p_stun_proc_status_ind->error_cause = NAT_ERR_NONE;
	p_stun_proc_status_ind->execute_procedures = procedure_name;

	switch(procedure_name)
	{
	case NAT_STUN_PROC_BINDING_LIFETIME_SUPP:
		p_stun_proc_status_ind->bind_discovery_timer = \
			((stun_lifetime_context_st *)p_glb_nat_pdb->p_glb_data->\
				p_stun_lifetime_data)->refresh_lifetime_duration;		
		break;

	case NAT_STUN_PROC_TYPE_DISCOVERY_SUPP:
		p_stun_proc_status_ind->nat_type = \
			((stun_discovery_context_st *)p_glb_nat_pdb->p_glb_data->\
				p_stun_discovery_data)->nat_type;
		break;
	}

	ret_val = nat_port_send_msg_to_app(p_glb_nat_pdb,p_proc_status_ind_msg);

	nat_port_static_memfree(p_proc_status_ind_msg,&ecode);
	return ret_val;
}

/******************************************************************************
 *
 * FUNCTION:        nat_stun_send_proc_status_ind_err
 *
 * DESCRIPTION:     This function is invoked to send a failure exec proc status
 *		    indication to the application.
 *      
 * RETURNS:         NAT_RETURN_SUCCESS in case of success and NAT_RETURN_FAILURE in
 *                  case of any failure.
 *
 ******************************************************************************/
nat_return_t nat_stun_send_proc_status_ind_err(
        INOUT nat_glb_pdb_st            *p_glb_nat_pdb,
	IN nat_uint8_t          	procedure_name)
{
	nat_return_t      	ret_val = NAT_RETURN_SUCCESS;
	
	nat_msg_st		*p_proc_status_ind_msg = NAT_NULL;
	nat_error_t		ecode = NAT_ERR_UNDEFINED;
	nat_stun_proc_status_ind_st	*p_stun_proc_status_ind = NAT_NULL;
	nat_msg_hdr_st    	recv_msg_hdr = p_glb_nat_pdb->p_recv_msg->p_msg->msg_hdr;

	p_proc_status_ind_msg = (nat_msg_st *)nat_port_static_memget(
		nat_port_sizeof(nat_msg_hdr_st) + nat_port_sizeof(nat_stun_proc_status_ind_st)
		,&ecode);

	p_proc_status_ind_msg->msg_hdr.api_id = NAT_STUN_PROC_STATUS_IND;
	p_proc_status_ind_msg->msg_hdr.app_context_id = recv_msg_hdr.app_context_id;
	p_proc_status_ind_msg->msg_hdr.source_id = recv_msg_hdr.destination_id;
	p_proc_status_ind_msg->msg_hdr.destination_id = recv_msg_hdr.source_id;
	p_proc_status_ind_msg->msg_hdr.api_length = nat_port_sizeof(nat_msg_hdr_st) + \
					       nat_port_sizeof(nat_stun_proc_status_ind_st);	

	p_stun_proc_status_ind = (nat_stun_proc_status_ind_st *)p_proc_status_ind_msg->p_msg_data;

	p_stun_proc_status_ind->result = NAT_RESULT_FAILURE;
	p_stun_proc_status_ind->error_cause = p_glb_nat_pdb->p_glb_data->ecode;
	p_stun_proc_status_ind->execute_procedures = procedure_name;


	ret_val = nat_port_send_msg_to_app(p_glb_nat_pdb,p_proc_status_ind_msg);

	nat_port_static_memfree(p_proc_status_ind_msg,&ecode);
	return ret_val;	
}

/******************************************************************************
 *
 * FUNCTION:        nat_stun_exec_proc_req
 *
 * DESCRIPTION:     This function is invoked whenever exec procedure request
 *                  is received from application.
 *                   
 * RETURNS:         NAT_RETURN_SUCCESS in case of success and NAT_RETURN_FAILURE in 
 *                  case of any failure.       
 *
 ******************************************************************************/
nat_return_t nat_stun_exec_proc_req(
        INOUT nat_glb_pdb_st             *p_glb_nat_pdb)
{
	nat_return_t            ret_val = NAT_RETURN_SUCCESS;
	nat_stun_exec_proc_req_st *p_exec_proc_req = (nat_stun_exec_proc_req_st *)\
					(p_glb_nat_pdb->p_recv_msg->p_msg->p_msg_data);

	nat_internal_msg_st     	internal_msg;

	stun_shsec_init_req_st		shsec_init_req;
	stun_discovery_init_req_st 	discovery_init_req;
	stun_lifetime_init_req_st	lifetime_init_req;

	nat_error_t		ecode = NAT_ERR_UNDEFINED;

	if(NAT_STUN_PROC_NOT_PRESENT == p_exec_proc_req->execute_procedures)
	{
		p_glb_nat_pdb->p_glb_data->ecode = NAT_ERR_STUN_INVALID_PROC;
                ret_val = nat_stun_send_exec_proc_resp_err(p_glb_nat_pdb);
	}
	else
	{
		ret_val = nat_stun_send_exec_proc_resp(p_glb_nat_pdb);
	}

	if(NAT_STUN_PROC_SHARED_SECRET_SUPP == 
		(NAT_STUN_PROC_SHARED_SECRET_SUPP & p_exec_proc_req->execute_procedures))
	{
        	if(NAT_FALSE == p_glb_nat_pdb->p_glb_data->p_stun_config_data->shsec_enabled)
        	{
           	 	p_glb_nat_pdb->p_glb_data->ecode = NAT_ERR_STUN_INVALID_PROC;
            		ret_val = nat_stun_send_proc_status_ind_err(p_glb_nat_pdb,
				NAT_STUN_PROC_SHARED_SECRET_SUPP);
        	}
		else if(NAT_NULL == p_glb_nat_pdb->p_glb_data->p_stun_shsec_data)
		{
			/* shared secret procedure has not as yet been started 
		   	  send the init request and then the trigger to start the 
		  	  procedure 
			*/

			internal_msg.msg_hdr.p_glb_pdb = p_glb_nat_pdb;
			internal_msg.msg_hdr.ecode = ecode;
			internal_msg.msg_hdr.msg_id = NAT_INT_STUN_SHSEC_INIT_REQ;
			internal_msg.p_msg_data = (nat_uint8_t *)(&shsec_init_req);

			shsec_init_req.timer_dur = p_glb_nat_pdb->p_glb_data->\
					p_stun_config_data->shsec_refresh_timer;

			if(NAT_RETURN_SUCCESS != nat_stun_shsec_process_mesg(&internal_msg))
			{
				p_glb_nat_pdb->p_glb_data->ecode = internal_msg.msg_hdr.ecode;
				ret_val = nat_stun_send_proc_status_ind_err(
						p_glb_nat_pdb,NAT_STUN_PROC_SHARED_SECRET_SUPP);   
			}

			/* now send the trigger to start the procedure execution */
			internal_msg.msg_hdr.p_glb_pdb = p_glb_nat_pdb;
                	internal_msg.msg_hdr.ecode = ecode;
			internal_msg.msg_hdr.msg_id = NAT_INT_STUN_SHSEC_REQ;
			internal_msg.msg_hdr.payload_length = 0;
		
 			if(NAT_RETURN_SUCCESS != nat_stun_shsec_process_mesg(&internal_msg))
                	{
				p_glb_nat_pdb->p_glb_data->ecode = internal_msg.msg_hdr.ecode;
                       	 	ret_val = nat_stun_send_proc_status_ind_err(
						p_glb_nat_pdb,NAT_STUN_PROC_SHARED_SECRET_SUPP);   
                	}

			p_glb_nat_pdb->p_glb_data->p_stun_context_map[NAT_STUN_MODULE_SHSEC].\
				app_context_id = p_glb_nat_pdb->p_recv_msg->p_msg->msg_hdr.app_context_id;

			p_glb_nat_pdb->p_glb_data->p_stun_context_map[NAT_STUN_MODULE_SHSEC].\
				module_id = NAT_STUN_MODULE_EXTERNAL;

			p_glb_nat_pdb->p_glb_data->shared_secret_proc_status = \
				NAT_STUN_SHARED_SECRET_ONGOING;
		}
		else if(NAT_STUN_SHARED_SECRET_ONGOING == \
			 p_glb_nat_pdb->p_glb_data->shared_secret_proc_status)
		{
			p_glb_nat_pdb->p_glb_data->ecode = NAT_ERR_SHARED_SECRET_ONGOING;
			ret_val = nat_stun_send_proc_status_ind_err(
					p_glb_nat_pdb,NAT_STUN_PROC_SHARED_SECRET_SUPP);   
		}
		else if(NAT_STUN_SHARED_SECRET_COMPLETE == \
			 p_glb_nat_pdb->p_glb_data->shared_secret_proc_status)
		{
			ret_val = nat_stun_send_proc_status_ind(
					p_glb_nat_pdb,NAT_STUN_PROC_SHARED_SECRET_SUPP);   
		}
	}
	
	if(NAT_STUN_PROC_BINDING_LIFETIME_SUPP == \
		(NAT_STUN_PROC_BINDING_LIFETIME_SUPP & p_exec_proc_req->execute_procedures))
	{
        if(NAT_FALSE == p_glb_nat_pdb->p_glb_data->p_stun_config_data->shsec_enabled)
        {
            p_glb_nat_pdb->p_glb_data->ecode = NAT_ERR_STUN_INVALID_PROC;
            ret_val = nat_stun_send_proc_status_ind_err(
					p_glb_nat_pdb,NAT_STUN_PROC_BINDING_LIFETIME_SUPP);
        }
		else if(NAT_STUN_BINDING_LIFETIME_PENDING == \
			p_glb_nat_pdb->p_glb_data->bind_lifetime_disc_status)
		{
			p_glb_nat_pdb->p_glb_data->ecode = NAT_ERR_BINDING_LIFETIME_PENDING;
			ret_val = nat_stun_send_proc_status_ind_err(
					p_glb_nat_pdb,NAT_STUN_PROC_BINDING_LIFETIME_SUPP);   
		}
		else if(NAT_STUN_BINDING_LIFETIME_ONGOING == \
			p_glb_nat_pdb->p_glb_data->bind_lifetime_disc_status)
		{
			p_glb_nat_pdb->p_glb_data->ecode = NAT_ERR_BINDING_LIFETIME_ONGOING;
			ret_val = nat_stun_send_proc_status_ind_err(
					p_glb_nat_pdb,NAT_STUN_PROC_BINDING_LIFETIME_SUPP);   
		}
		else if(NAT_STUN_BINDING_LIFETIME_COMPLETE == \
			p_glb_nat_pdb->p_glb_data->bind_lifetime_disc_status)
		{
			ret_val = nat_stun_send_proc_status_ind(
					p_glb_nat_pdb,NAT_STUN_PROC_BINDING_LIFETIME_SUPP);   
		}
		else if(NAT_NULL == p_glb_nat_pdb->p_glb_data->p_stun_lifetime_data)
		{
			/* Firstly initialize the Shared Secret State machine */
			internal_msg.msg_hdr.p_glb_pdb = p_glb_nat_pdb;
			internal_msg.msg_hdr.ecode = ecode;
			internal_msg.msg_hdr.msg_id = NAT_INT_STUN_LIFETIME_INIT_REQ;
			internal_msg.p_msg_data = (nat_uint8_t *)(&lifetime_init_req);

			lifetime_init_req.timer_dur = p_glb_nat_pdb->p_glb_data->\
					p_stun_config_data->lifetime_disc_timer;

			lifetime_init_req.delta_interval = p_glb_nat_pdb->p_glb_data->\
					p_stun_config_data->lifetime_delta_timer;

			if(NAT_RETURN_SUCCESS != nat_stun_lifetime_process_mesg(&internal_msg))
			{
				p_glb_nat_pdb->p_glb_data->ecode = internal_msg.msg_hdr.ecode;
				ret_val = nat_stun_send_proc_status_ind_err(
						p_glb_nat_pdb,NAT_STUN_PROC_BINDING_LIFETIME_SUPP);   
			}

			p_glb_nat_pdb->p_glb_data->
				p_stun_context_map[NAT_STUN_MODULE_LIFETIME].app_context_id = \
				 p_glb_nat_pdb->p_recv_msg->p_msg->msg_hdr.app_context_id;

			p_glb_nat_pdb->p_glb_data->p_stun_context_map[NAT_STUN_MODULE_LIFETIME].\
					module_id = NAT_STUN_MODULE_EXTERNAL;

			
			if(NAT_STUN_SHARED_SECRET_ONGOING == \
			 	p_glb_nat_pdb->p_glb_data->shared_secret_proc_status)
			{
				p_glb_nat_pdb->p_glb_data->bind_lifetime_disc_status = \
					NAT_STUN_BINDING_LIFETIME_PENDING;
			}
			else
			{
				/* now send the trigger to start the procedure execution */
				internal_msg.msg_hdr.p_glb_pdb = p_glb_nat_pdb;
               			internal_msg.msg_hdr.ecode = ecode;
				internal_msg.msg_hdr.msg_id = NAT_INT_STUN_LIFETIME_DISC_REQ;
				internal_msg.msg_hdr.payload_length = 0;
		
 				if(NAT_RETURN_SUCCESS != nat_stun_lifetime_process_mesg(&internal_msg))
                		{
					p_glb_nat_pdb->p_glb_data->ecode = internal_msg.msg_hdr.ecode;
                        		ret_val = nat_stun_send_proc_status_ind_err(
							p_glb_nat_pdb,NAT_STUN_PROC_BINDING_LIFETIME_SUPP);
                		}
			
				p_glb_nat_pdb->p_glb_data->bind_lifetime_disc_status = \
					NAT_STUN_BINDING_LIFETIME_ONGOING;
			}
			
		}
		

	}
	
	if(NAT_STUN_PROC_TYPE_DISCOVERY_SUPP == \
		(NAT_STUN_PROC_TYPE_DISCOVERY_SUPP & p_exec_proc_req->execute_procedures))
	{
		if(NAT_STUN_NAT_TYPE_DISCOVERY_PENDING == \
			p_glb_nat_pdb->p_glb_data->nat_type_disc_status)
		{
			p_glb_nat_pdb->p_glb_data->ecode = NAT_ERR_TYPE_DISCOVERY_PENDING;
			ret_val = nat_stun_send_proc_status_ind_err(
					p_glb_nat_pdb,NAT_STUN_PROC_TYPE_DISCOVERY_SUPP);   
		}
		else if(NAT_STUN_NAT_TYPE_DISCOVERY_ONGOING == \
			p_glb_nat_pdb->p_glb_data->nat_type_disc_status)
		{
			p_glb_nat_pdb->p_glb_data->ecode = NAT_ERR_TYPE_DISCOVERY_ONGOING;
			ret_val = nat_stun_send_proc_status_ind_err(
					p_glb_nat_pdb,NAT_STUN_PROC_TYPE_DISCOVERY_SUPP);   
		}
		else if(NAT_STUN_NAT_TYPE_DISCOVERY_COMPLETE == \
			p_glb_nat_pdb->p_glb_data->nat_type_disc_status)
		{
			ret_val = nat_stun_send_proc_status_ind(
					p_glb_nat_pdb,NAT_STUN_PROC_TYPE_DISCOVERY_SUPP);   
		}
		else if(NAT_NULL == p_glb_nat_pdb->p_glb_data->p_stun_discovery_data)
		{
			/* Firstly initialize the Shared Secret State machine */
			internal_msg.msg_hdr.p_glb_pdb = p_glb_nat_pdb;
			internal_msg.msg_hdr.ecode = ecode;
			internal_msg.msg_hdr.msg_id = NAT_INT_STUN_DISCOVERY_INIT_REQ;
			internal_msg.p_msg_data = (nat_uint8_t *)(&discovery_init_req);

			discovery_init_req.timer_dur = p_glb_nat_pdb->p_glb_data->\
					p_stun_config_data->type_disc_timer;

			if(NAT_RETURN_SUCCESS != nat_stun_discovery_process_mesg(&internal_msg))
			{
				p_glb_nat_pdb->p_glb_data->ecode = internal_msg.msg_hdr.ecode;
				ret_val = nat_stun_send_proc_status_ind_err(
						p_glb_nat_pdb,NAT_STUN_PROC_TYPE_DISCOVERY_SUPP);
			}

			p_glb_nat_pdb->p_glb_data->
				p_stun_context_map[NAT_STUN_MODULE_DISCOVERY].app_context_id = \
				 p_glb_nat_pdb->p_recv_msg->p_msg->msg_hdr.app_context_id;

			p_glb_nat_pdb->p_glb_data->p_stun_context_map[NAT_STUN_MODULE_DISCOVERY].\
					module_id = NAT_STUN_MODULE_EXTERNAL;

			
			if(NAT_STUN_SHARED_SECRET_ONGOING == \
			 	p_glb_nat_pdb->p_glb_data->shared_secret_proc_status)
			{
				p_glb_nat_pdb->p_glb_data->nat_type_disc_status = \
					NAT_STUN_NAT_TYPE_DISCOVERY_PENDING;
			}
			else
			{
				/* now send the trigger to start the procedure execution */
				internal_msg.msg_hdr.p_glb_pdb = p_glb_nat_pdb;
               			internal_msg.msg_hdr.ecode = ecode;
				internal_msg.msg_hdr.msg_id = NAT_INT_STUN_DISCOVERY_TYPE_REQ;
				internal_msg.msg_hdr.payload_length = 0;
		
 				if(NAT_RETURN_SUCCESS != nat_stun_discovery_process_mesg(&internal_msg))
                		{
					p_glb_nat_pdb->p_glb_data->ecode = internal_msg.msg_hdr.ecode;
                        		ret_val = nat_stun_send_proc_status_ind_err(
							p_glb_nat_pdb,NAT_STUN_PROC_TYPE_DISCOVERY_SUPP);
                		}
			
				p_glb_nat_pdb->p_glb_data->nat_type_disc_status = \
					NAT_STUN_NAT_TYPE_DISCOVERY_ONGOING;
			}
		}
	}

	return ret_val;
}



/******************************************************************************
 *
 * FUNCTION:        nat_stun_send_get_public_ip_port_resp_err
 *
 * DESCRIPTION:     This function is invoked to send Public IP/PORT error response.
 *             *      
 * RETURNS:         NAT_RETURN_SUCCESS in case of success and NAT_RETURN_FAILURE in 
 *                  case of any failure.       
 *
 ******************************************************************************/
nat_return_t nat_stun_send_get_public_ip_port_resp_err(
        INOUT nat_glb_pdb_st             *p_glb_nat_pdb)
{
	nat_return_t 	ret_val = NAT_RETURN_SUCCESS;
	nat_error_t	ecode = NAT_ERR_NONE;	
	nat_msg_st			*p_public_ip_port_resp_msg = NAT_NULL;
	nat_get_public_ip_port_resp_st	*p_stun_public_ip_port_resp = NAT_NULL;
	nat_msg_hdr_st    		recv_msg_hdr = p_glb_nat_pdb->p_recv_msg->p_msg->msg_hdr;

	p_public_ip_port_resp_msg = (nat_msg_st *)nat_port_static_memget(
		 nat_port_sizeof(nat_msg_hdr_st) + nat_port_sizeof(nat_get_public_ip_port_resp_st),
		&ecode);

	p_public_ip_port_resp_msg->msg_hdr.api_id = NAT_GET_PUBLIC_IP_PORT_RESP;
	p_public_ip_port_resp_msg->msg_hdr.app_context_id = recv_msg_hdr.app_context_id;
	p_public_ip_port_resp_msg->msg_hdr.source_id = recv_msg_hdr.destination_id;
	p_public_ip_port_resp_msg->msg_hdr.destination_id = recv_msg_hdr.source_id;
	p_public_ip_port_resp_msg->msg_hdr.api_length = nat_port_sizeof(nat_msg_hdr_st) + \
					       nat_port_sizeof(nat_get_public_ip_port_resp_st);	

	p_stun_public_ip_port_resp = (nat_get_public_ip_port_resp_st *)
			p_public_ip_port_resp_msg->p_msg_data;

	p_stun_public_ip_port_resp->result = NAT_RESULT_FAILURE;
	p_stun_public_ip_port_resp->error_cause = p_glb_nat_pdb->p_glb_data->ecode;
	p_stun_public_ip_port_resp->nat_protocol = ((nat_get_public_ip_port_req_st *)\
				p_glb_nat_pdb->p_recv_msg->p_msg->p_msg_data)->nat_protocol;


	ret_val = nat_port_send_msg_to_app(p_glb_nat_pdb,p_public_ip_port_resp_msg);
	
	nat_port_static_memfree(p_public_ip_port_resp_msg,&ecode);
	return ret_val;
}

/******************************************************************************
 *
 * FUNCTION:        nat_get_public_ip_port_req
 *
 * DESCRIPTION:     This function is invoked whenever stun get public Ip-Port request
 *                  is received from application.
 *             *      
 * RETURNS:         NAT_RETURN_SUCCESS in case of success and NAT_RETURN_FAILURE in 
 *                  case of any failure.       
 *
 ******************************************************************************/
nat_return_t nat_get_public_ip_port_req(
        INOUT nat_glb_pdb_st             *p_glb_nat_pdb)
{
	nat_return_t                         ret_val=NAT_RETURN_SUCCESS;
	nat_get_public_ip_port_req_st *p_public_ip_port_req = (nat_get_public_ip_port_req_st *)\
					(p_glb_nat_pdb->p_recv_msg->p_msg->p_msg_data);
					
	nat_msg_hdr_st    		recv_msg_hdr = p_glb_nat_pdb->p_recv_msg->p_msg->msg_hdr;

	nat_internal_msg_st     	internal_msg;
	nat_uint8_t			context_id = 0;
	nat_ip_port_address_st    	local_address = 
                        p_public_ip_port_req->local_address;
	nat_uint8_t			module_id = NAT_STUN_MODULE_EXTERNAL,
					source_id = recv_msg_hdr.source_id,
					destination_id = recv_msg_hdr.destination_id;
	nat_boolean_t			is_free = NAT_FALSE;
	stun_get_public_ip_port_req_st	stun_public_ip_port_req;
	nat_error_t			ecode = NAT_ERR_UNDEFINED;
        stun_config_data_st             *p_config_data = NAT_NULL;
        nat_msg_st			*p_public_ip_port_resp_msg = NAT_NULL;
        nat_get_public_ip_port_resp_st	*p_stun_public_ip_port_resp =  NAT_NULL;


	if(NAT_PROTOCOL_STUN != p_public_ip_port_req->nat_protocol)
	{
		p_glb_nat_pdb->p_glb_data->ecode = NAT_ERR_PROTOCOL_NOT_SUPPORTED;
		ret_val = nat_stun_send_get_public_ip_port_resp_err(p_glb_nat_pdb);
	}
    	else if(NAT_NULL == p_nat_global_pdb->p_glb_data->p_stun_context_map)
    	{
        	p_glb_nat_pdb->p_glb_data->ecode = NAT_ERR_STUN_INIT_NOT_COMPLETE;
		ret_val = nat_stun_send_get_public_ip_port_resp_err(p_glb_nat_pdb);
    	}
	else
	{
		if(NAT_FALSE == nat_validate_new_request(p_glb_nat_pdb))
		{
			return NAT_RETURN_SUCCESS;
		}

		NAT_GLB_ALLOC_CONTEXT(p_glb_nat_pdb->p_glb_data->p_stun_context_map,\
				context_id,p_glb_nat_pdb->p_glb_data->max_contexts)

		if(context_id == p_glb_nat_pdb->p_glb_data->max_contexts + 3)
		{
			p_glb_nat_pdb->p_glb_data->ecode = NAT_ERR_STUN_NO_CONTEXT_AVAILABLE;
			ret_val = nat_stun_send_get_public_ip_port_resp_err(p_glb_nat_pdb);
		}
                  /*Fix for SPR 16744-CSR-1-5140139 */
                  /* If the STUN SERVER address is 0.0.0.0 in nat.cfg , then and there will be no
                     binding request to STUN SERVER and The API nat_get_public_ip_port_response
                     will return the locat IP and Port from request only. */
                p_config_data = p_nat_global_pdb->p_glb_data->p_stun_config_data;

               if((NAT_NULL == p_config_data->stun_server_addr.ipv4_addr.octet_1) &&
                  (NAT_NULL == p_config_data->stun_server_addr.ipv4_addr.octet_2) &&
       		  (NAT_NULL == p_config_data->stun_server_addr.ipv4_addr.octet_3) &&
    		  (NAT_NULL == p_config_data->stun_server_addr.ipv4_addr.octet_4))
               {

                 p_public_ip_port_resp_msg = (nat_msg_st *)nat_port_static_memget(
		        nat_port_sizeof(nat_msg_hdr_st) + 
                        nat_port_sizeof(nat_get_public_ip_port_resp_st),
		        &ecode);

                 p_public_ip_port_resp_msg->msg_hdr.api_id = NAT_GET_PUBLIC_IP_PORT_RESP;
                 p_public_ip_port_resp_msg->msg_hdr.app_context_id = 
                                                recv_msg_hdr.app_context_id;
                 p_public_ip_port_resp_msg->msg_hdr.source_id = 
                                                recv_msg_hdr.destination_id;
                 p_public_ip_port_resp_msg->msg_hdr.destination_id = 
                                                recv_msg_hdr.source_id;
	         p_public_ip_port_resp_msg->msg_hdr.api_length = 
                                   nat_port_sizeof(nat_msg_hdr_st) + 
                                   nat_port_sizeof(nat_get_public_ip_port_resp_st);	
    
                 p_stun_public_ip_port_resp = (nat_get_public_ip_port_resp_st *)
                                   p_public_ip_port_resp_msg->p_msg_data;
                               
                 p_stun_public_ip_port_resp->nat_protocol = NAT_PROTOCOL_STUN;
    				    
                 p_stun_public_ip_port_resp->result = NAT_RESULT_SUCCESS;
       		 p_stun_public_ip_port_resp->error_cause = NAT_ERR_NONE;
	         nat_port_memcpy(&p_stun_public_ip_port_resp->public_addr,
                      &(((stun_data_st *)p_glb_nat_pdb->p_glb_data->p_stun_data)
                        ->p_stun_contexts[context_id].destination_addr),
                      nat_port_sizeof(nat_ip_port_address_st));
                 ret_val = nat_port_send_msg_to_app(p_glb_nat_pdb,p_public_ip_port_resp_msg);
	         nat_port_static_memfree(p_public_ip_port_resp_msg,&ecode);
                } /*End of Fix for.... */
		else
		{
					
			NAT_GLB_FILL_CONTEXT(
                p_glb_nat_pdb->p_glb_data->p_stun_context_map[context_id],\
				recv_msg_hdr.app_context_id,local_address,module_id,
                is_free,source_id,\
				destination_id)	
				
			/* now send the trigger to start the procedure execution */
			internal_msg.msg_hdr.p_glb_pdb = p_glb_nat_pdb;
               		internal_msg.msg_hdr.ecode = ecode;
			internal_msg.msg_hdr.msg_id = NAT_INT_STUN_GET_PUBLIC_IP_PORT_REQ;
			internal_msg.p_msg_data = (nat_uint8_t *)(&stun_public_ip_port_req);
		
			nat_port_memset(&stun_public_ip_port_req,0,
				nat_port_sizeof(stun_get_public_ip_port_req_st));
	
			stun_public_ip_port_req.context_id = context_id;
			nat_port_memcpy( &stun_public_ip_port_req.local_addr,
					 &p_public_ip_port_req->local_address,
                     nat_port_sizeof(nat_ip_port_address_st));
		
            nat_port_memcpy( &stun_public_ip_port_req.destination_addr,
					   &p_glb_nat_pdb->p_glb_data->p_stun_config_data->\
                       stun_server_addr,nat_port_sizeof(nat_ip_port_address_st));
		
           stun_public_ip_port_req.change_ip = NAT_FALSE;
           stun_public_ip_port_req.change_port = NAT_FALSE;
           stun_public_ip_port_req.is_refresh_enabled = \
                    p_public_ip_port_req->is_refresh_enabled;
                
 			if(NAT_RETURN_SUCCESS != nat_stun_process_mesg(&internal_msg))
   	        {
				p_glb_nat_pdb->p_glb_data->ecode = internal_msg.msg_hdr.ecode;
   	            ret_val = nat_stun_send_get_public_ip_port_resp_err(p_glb_nat_pdb);
            }				
		}
#ifdef NAT_UT
		}
#endif	
	}	

	return ret_val;
}


/******************************************************************************
 *
 * FUNCTION:        nat_stun_rel_public_ip_port_req
 *
 * DESCRIPTION:     This function is invoked whenever stun release public 
 *                  Ip-Port request is received from application.
 *             *      
 * RETURNS:         NAT_SUCCESS in case of success and NAT_FAILURE in 
 *                  case of any failure.       
 *
 ******************************************************************************/
nat_return_t nat_rel_public_ip_port_req(
        INOUT nat_glb_pdb_st             *p_glb_nat_pdb)
{
	nat_return_t                         ret_val=NAT_RETURN_SUCCESS;

    nat_msg_hdr_st      nat_msg_hdr = p_glb_nat_pdb->p_recv_msg->p_msg->msg_hdr;

					
	nat_internal_msg_st     	internal_msg;
	nat_uint8_t			context_id = 0;
        nat_uint32_t		    app_context_id = 	nat_msg_hdr.app_context_id;
	stun_rel_public_ip_port_req_st	stun_rel_ip_port_req;
	nat_error_t			ecode = NAT_ERR_UNDEFINED;

	NAT_GLB_FIND_CONTEXT(p_glb_nat_pdb->p_glb_data->p_stun_context_map,
		app_context_id,context_id,
		p_glb_nat_pdb->p_glb_data->max_contexts)


	if(context_id == p_glb_nat_pdb->p_glb_data->max_contexts + 3 )
	{
		/* ignore the message */
	}
	else
	{
							
		/* now send the trigger to start the procedure execution */
		internal_msg.msg_hdr.p_glb_pdb = p_glb_nat_pdb;
               	internal_msg.msg_hdr.ecode = ecode;
		internal_msg.msg_hdr.msg_id = NAT_INT_STUN_REL_PUBLIC_IP_PORT_REQ;
		internal_msg.p_msg_data = (nat_uint8_t *)(&stun_rel_ip_port_req);
		
		stun_rel_ip_port_req = context_id;
            
 		ret_val = nat_stun_process_mesg(&internal_msg);
   	    				
	}	

	return ret_val;
}

/******************************************************************************
 *
 * FUNCTION:        nat_process_udp_pkt_ind
 *
 * DESCRIPTION:     This function is invoked whenever stun receives UDP Packet 
 *                  Indication from application.
 *                 
 * RETURNS:         NAT_RETURN_SUCCESS in case of success and NAT_RETURN_FAILURE in 
 *                  case of any failure.       
 *
 ******************************************************************************/
nat_return_t nat_process_udp_pkt_ind(
        INOUT nat_glb_pdb_st             *p_glb_nat_pdb)
{
    nat_return_t          ret_val=NAT_RETURN_SUCCESS;
    arStun_msg_st         stun_msg;
    arStun_ecode_et       ecode = ARSTUN_ERROR_START;
    nat_uint8_t			context_id = 0;
    nat_string_st	password = {0,{"\0"}};
    nat_recv_udp_pkt_ind_st *p_recv_msg = 
		(nat_recv_udp_pkt_ind_st *)p_glb_nat_pdb->p_recv_msg->p_msg->p_msg_data;

    nat_port_memset(&stun_msg,0,nat_port_sizeof(arStun_msg_st));

    if(UTIL_FAILURE == arStun_decode_message_header(
                        (arStun_msg_buffer_st *)&(p_recv_msg->octets),
                                       &(stun_msg.msg_hdr) , &ecode))
    {
        /* ignore the message, could be some stray message */
        return NAT_RETURN_SUCCESS;
    }
    else
    {
        arStun_uint128_t transaction_id = stun_msg.msg_hdr.transaction_id;

        
        NAT_GLB_GET_CONTEXT_ID_FRM_TRANSACTION_ID(\
            p_glb_nat_pdb->p_glb_data->p_stun_context_map,\
            p_glb_nat_pdb->p_glb_data->max_contexts,\
            transaction_id, context_id)

        if(NAT_STUN_SHARED_SECRET_NOT_REQD == 
            p_glb_nat_pdb->p_glb_data->shared_secret_proc_status)
        {
            password.str_len = 0;
	        password.str[0] = '\0';
        }
        else if(NAT_STUN_SHARED_SECRET_COMPLETE == 
            p_glb_nat_pdb->p_glb_data->shared_secret_proc_status)
        {
            /* copy the username / password in the context */
            password.str_len = 
                (nat_uint16_t)nat_port_strlen(p_glb_nat_pdb->p_glb_data->p_stun_config_data->password);
	        nat_port_strcpy(password.str,
                p_glb_nat_pdb->p_glb_data->p_stun_config_data->password);
        }


		if(context_id == p_glb_nat_pdb->p_glb_data->max_contexts + 3)
		{
			/* ignore the message, could be some stray message */
            return NAT_RETURN_SUCCESS;
		}
        else if(UTIL_SUCCESS != arStun_decode_message(
                              (arStun_msg_buffer_st *)&(p_recv_msg->octets),
                              &stun_msg,&ecode))
        {
            /* ignore the message, could be some stray message 
            p_glb_nat_pdb->p_glb_data->p_stun_context_map[context_id]*/
            return NAT_RETURN_SUCCESS;
        }
        else
        {
            nat_internal_msg_st     	internal_msg;
            stun_binding_resp_st        stun_binding_resp;
			
            /* now send the trigger to start the procedure execution */
			internal_msg.msg_hdr.p_glb_pdb = p_glb_nat_pdb;
            internal_msg.msg_hdr.ecode = ecode;
			internal_msg.msg_hdr.msg_id = NAT_INT_STUN_BINDING_RESP;
			internal_msg.p_msg_data = (nat_uint8_t *)(&stun_binding_resp);
			
			stun_binding_resp.context_id = context_id;
			
            if(ARSTUN_BIND_RESP_MSG == stun_msg.msg_hdr.msg_type)
            {
			  if((ARSTUN_MAPPED_ADDRESS_PRESENT & stun_msg.msg_attrib_presence) &&
                 (ARSTUN_SOURCE_ADDRESS_PRESENT & stun_msg.msg_attrib_presence) &&
                 (ARSTUN_CHANGED_ADDRESS_PRESENT & stun_msg.msg_attrib_presence))
                {
                  /* integrity check to be performed at decoding itself */
                  stun_binding_resp.result = NAT_RESULT_SUCCESS;

                  nat_port_memcpy( &(stun_binding_resp.mapped_addr),
				    	  &(stun_msg.mapped_addr.ipaddr), 
                          nat_port_sizeof(arStun_ip_port_address_st));
		
                  nat_port_memcpy( &(stun_binding_resp.changed_addr),
				         &(stun_msg.changed_addr.ipaddr),
                          nat_port_sizeof(arStun_ip_port_address_st));
		 	    }
                else
				{
					return NAT_RETURN_SUCCESS;
				}
            }
            else if(ARSTUN_BIND_ERR_RESP_MSG == stun_msg.msg_hdr.msg_type)
            {
                stun_binding_resp.result = NAT_RESULT_FAILURE;
                stun_binding_resp.error = 
						(nat_uint16_t)((stun_msg.error_code.error_class * 100) + 
						stun_msg.error_code.number);
            }
			
		
                
 			if(NAT_RETURN_SUCCESS != nat_stun_process_mesg(&internal_msg))
   	        {
				p_glb_nat_pdb->p_glb_data->ecode = internal_msg.msg_hdr.ecode;
   	            ret_val = nat_stun_send_get_public_ip_port_resp_err(p_glb_nat_pdb);
            }	
        }
        
    }
	/*
		1. Invoke arStun_decode_message_header()to decode the header of received UDP packet.
		2. If decode message header failed then
			a)Ignore this message.
		3. else 
			a) Use the transaction ID to get the context in the global data map.
			b) If no context is found then ignore this response message.
			c) Decode the message , with function arStun_decode_message() 
			c) check the message type:
				i) if Binding Success Response
					- Get the Password from local context using the context ID.
					- If Password present, and the decoded message has no Message Integrity field,
					   discard the response.
					- else call function stun_check_msg_integrity() to check the message integirty.
					- If check fails, Send a Binding error response to binding state machine with error
					   cause as STUN_DECODE_ERR_MSG_INTEG_FAIL
					- If check passes, send a Binding success response to binding state machine
				ii) if Binding Error Response
					- Send a Binding error response to binding state machine with error
					   cause as received in message payload.
			
     */
	return ret_val;

}


/******************************************************************************
 *
 * FUNCTION:        nat_process_tcp_sec_pkt_ind
 *
 * DESCRIPTION:     This function is invoked whenever receves stun TCP Packet 
 *                  Indication from application.
 *                
 * RETURNS:         NAT_RETURN_SUCCESS in case of success and NAT_RETURN_FAILURE in 
 *                  case of any failure.       
 *
 ******************************************************************************/
nat_return_t nat_process_tcp_sec_pkt_ind(
        INOUT nat_glb_pdb_st             *p_glb_nat_pdb)
{

	nat_return_t                         ret_val=NAT_RETURN_SUCCESS;
	p_glb_nat_pdb = p_glb_nat_pdb;


	/*
		1. Invoke arStun_decode_message_header()to decode the header of received TCP packet.
		2. If decode message header failed then
			a)Ignore this message.
		3. else 
			a) Match the transaction ID with the 0th context in the global data map.
			b) If no match is found then ignore this response message.
			c) Else Decode the message , with function arStun_decode_message() 
			c) Send the shared secret response to shared secret state machine.
     */

	return ret_val;
}

/******************************************************************************
 *
 * FUNCTION:        nat_process_start_timer_resp
 *
 * DESCRIPTION:     This function is invoked whenever timer response
 *                  is received from application.
 *                
 * RETURNS:         NAT_RETURN_SUCCESS in case of success and NAT_RETURN_FAILURE in 
 *                  case of any failure.       
 *
 ******************************************************************************/
nat_return_t nat_process_start_timer_resp(
        INOUT nat_glb_pdb_st             *p_glb_nat_pdb)
{
	nat_return_t                         ret_val=NAT_RETURN_SUCCESS;

	p_glb_nat_pdb = p_glb_nat_pdb;
	/*

	1. if context_id == NAT_STUN_MODULE_SHSEC , send the timer response to Shared Secret
		state machine.
	2. if context_id == NAT_STUN_MODULE_LIFETIME , send the timer response to Lifetime
		state machine.
	3. if context_id == NAT_STUN_MODULE_DISCOVERY , send the timer response to Discovery
		state machine.
	4. else validate the context id, 
	   a) if context id >=3 && context id <= max_context, and allocated as well
		  then send the timer response to binding state machine.
	   b) else ignore the response.
	     
	*/
	return ret_val;

}


/******************************************************************************
 *
 * FUNCTION:        nat_process_timer_exp_ind
 *
 * DESCRIPTION:     This function is invoked whenever timer expiry indication
 *                  is received from application.
 *                
 * RETURNS:         NAT_RETURN_SUCCESS in case of success and NAT_RETURN_FAILURE in 
 *                  case of any failure.       
 *
 ******************************************************************************/
nat_return_t nat_process_timer_exp_ind(
        INOUT nat_glb_pdb_st             *p_glb_nat_pdb)
{

	nat_return_t            ret_val = NAT_RETURN_SUCCESS;
    nat_error_t             ecode = NAT_ERR_NONE;
	nat_timer_expiry_ind_st *p_timer_exp_ind = (nat_timer_expiry_ind_st *)\
					(p_glb_nat_pdb->p_recv_msg->p_msg->p_msg_data);

	nat_internal_msg_st     	internal_msg;

    stun_timer_expire_ind_st	nat_timer_expire_ind;

    internal_msg.msg_hdr.p_glb_pdb = p_glb_nat_pdb;
	internal_msg.msg_hdr.ecode = ecode;
	internal_msg.msg_hdr.msg_id = NAT_INT_TIMER_EXPIRY_IND;
	internal_msg.p_msg_data = (nat_uint8_t *)(&nat_timer_expire_ind);

	
    nat_timer_expire_ind.context_id = p_timer_exp_ind->timer_data.context_id;
    nat_timer_expire_ind.timer_id = p_timer_exp_ind->timer_id;
    nat_timer_expire_ind.timer_type = p_timer_exp_ind->timer_data.timer_type;


	if(NAT_RETURN_SUCCESS != nat_process_internal_mesg(&internal_msg))
	{
		ret_val = NAT_RETURN_FAILURE;   
	}

	
	/*

	1. if context_id == NAT_STUN_MODULE_SHSEC , send the timer expiry ind to Shared Secret
		state machine.
	2. if context_id == NAT_STUN_MODULE_LIFETIME , send the timer expiry ind to Lifetime
		state machine.
	3. if context_id == NAT_STUN_MODULE_DISCOVERY , send the timer expiry ind to Discovery
		state machine.
	4. else validate the context id, 
	   a) if context id >=3 && context id <= max_context, and allocated as well
		  then send the timer expiry ind to binding state machine.
	   b) else ignore the response.
	     
	*/
	return ret_val;
}

/******************************************************************************
 *
 * FUNCTION:        nat_send_get_public_ip_port_resp
 *
 * DESCRIPTION:     This function is invoked to send Public IP/PORT 
 *                  response.
 *      
 * RETURNS:         NAT_RETURN_SUCCESS in case of success and NAT_RETURN_FAILURE in 
 *                  case of any failure.       
 *
 ******************************************************************************/
nat_return_t 
nat_send_get_public_ip_port_resp(
                                 INOUT nat_internal_msg_st        *p_msg)
{
	nat_return_t 	ret_val = NAT_RETURN_SUCCESS;
	nat_error_t	ecode = NAT_ERR_NONE;	
	stun_get_public_ip_port_resp_st *p_int_ip_port_resp = 
                 (stun_get_public_ip_port_resp_st *)p_msg->p_msg_data;
    nat_internal_msg_hdr_st int_msg_hdr  = p_msg->msg_hdr;
    
    nat_context_data_map_st context_data_map = int_msg_hdr.p_glb_pdb->p_glb_data\
                            ->p_stun_context_map[p_int_ip_port_resp->context_id];
                                            
	nat_msg_st			*p_public_ip_port_resp_msg = NAT_NULL;
	nat_get_public_ip_port_resp_st	*p_stun_public_ip_port_resp =  NAT_NULL;

    /* check if the request was from external module */
    if(NAT_STUN_MODULE_EXTERNAL == context_data_map.module_id)
    {
	p_public_ip_port_resp_msg = (nat_msg_st *)nat_port_static_memget(
		nat_port_sizeof(nat_msg_hdr_st) + nat_port_sizeof(nat_get_public_ip_port_resp_st),
		&ecode);

    	p_public_ip_port_resp_msg->msg_hdr.api_id = NAT_GET_PUBLIC_IP_PORT_RESP;
    	p_public_ip_port_resp_msg->msg_hdr.app_context_id = 
                                                  context_data_map.app_context_id;
    	p_public_ip_port_resp_msg->msg_hdr.source_id = 
                                                  context_data_map.destination_id;
    	p_public_ip_port_resp_msg->msg_hdr.destination_id = 
                                                  context_data_map.source_id;
    	p_public_ip_port_resp_msg->msg_hdr.api_length = 
                   nat_port_sizeof(nat_msg_hdr_st) + 
                   nat_port_sizeof(nat_get_public_ip_port_resp_st);	
    
    	p_stun_public_ip_port_resp = (nat_get_public_ip_port_resp_st *)
                                   p_public_ip_port_resp_msg->p_msg_data;
                                   
         p_stun_public_ip_port_resp->nat_protocol = NAT_PROTOCOL_STUN;
    				
        if(NAT_RESULT_SUCCESS == p_int_ip_port_resp->result)
        {
              p_stun_public_ip_port_resp->result = NAT_RESULT_SUCCESS;
              p_stun_public_ip_port_resp->error_cause = NAT_ERR_NONE;
              nat_port_memcpy(&p_stun_public_ip_port_resp->public_addr,
                              &p_int_ip_port_resp->mapped_addr,
                               nat_port_sizeof(nat_ip_port_address_st));
        }
        else
        {
    	      p_stun_public_ip_port_resp->result = NAT_RESULT_FAILURE;
              p_stun_public_ip_port_resp->error_cause = p_int_ip_port_resp->error;
        }

    	ret_val = nat_port_send_msg_to_app(int_msg_hdr.p_glb_pdb,p_public_ip_port_resp_msg);
	nat_port_static_memfree(p_public_ip_port_resp_msg,&ecode);
    }
    else
    {
        /* send the response to the calling imternal module */
	if(NAT_STUN_MODULE_DISCOVERY == context_data_map.module_id)
        {
            if(NAT_RETURN_SUCCESS != nat_stun_discovery_process_mesg(p_msg))
            {
                ret_val = NAT_RETURN_FAILURE;
                
            }
        }

    }
	
	return ret_val;
}

/******************************************************************************
 *
 * FUNCTION:        nat_send_shsec_resp
 *
 * DESCRIPTION:     This function is invoked to send shared secret response
 *      
 * RETURNS:         NAT_RETURN_SUCCESS in case of success and NAT_RETURN_FAILURE in 
 *                  case of any failure.       
 *
 ******************************************************************************/
nat_return_t 
nat_send_shsec_resp(
                    INOUT nat_internal_msg_st        *p_msg)
{
     nat_return_t 	ret_val = NAT_RETURN_SUCCESS;
	
	stun_get_public_ip_port_resp_st *p_int_ip_port_resp = 
                 (stun_get_public_ip_port_resp_st *)p_msg->p_msg_data;
    nat_internal_msg_hdr_st int_msg_hdr  = p_msg->msg_hdr;
    
    nat_context_data_map_st context_data_map = int_msg_hdr.p_glb_pdb->p_glb_data\
                            ->p_stun_context_map[NAT_STUN_MODULE_SHSEC];
                                            
	nat_msg_st			*p_shsec_ind_msg = NAT_NULL;
	nat_error_t			ecode = NAT_ERR_UNDEFINED;
	
	nat_stun_proc_status_ind_st	*p_stun_shsec_status_ind = NAT_NULL;

    /* check if the request was from external module */
    if(NAT_STUN_MODULE_EXTERNAL == context_data_map.module_id)
    {
    	p_shsec_ind_msg = (nat_msg_st *)nat_port_static_memget(
                                      nat_port_sizeof(nat_msg_st),&ecode);
    
    	p_shsec_ind_msg->msg_hdr.api_id = NAT_STUN_PROC_STATUS_IND;
    	p_shsec_ind_msg->msg_hdr.app_context_id = 
                                          context_data_map.app_context_id;
    	p_shsec_ind_msg->msg_hdr.source_id = 
                                          context_data_map.destination_id;
    	p_shsec_ind_msg->msg_hdr.destination_id = 
                                          context_data_map.source_id;
    	p_shsec_ind_msg->msg_hdr.api_length = 
                   nat_port_sizeof(nat_msg_hdr_st) + 
                   nat_port_sizeof(nat_stun_proc_status_ind_st);	
    
    	p_stun_shsec_status_ind = (nat_stun_proc_status_ind_st *)
                                   p_shsec_ind_msg->p_msg_data;
                                   
        p_stun_shsec_status_ind->execute_procedures = 
                                            NAT_STUN_PROC_SHARED_SECRET_SUPP;
    				
        if(NAT_RESULT_SUCCESS == p_int_ip_port_resp->result)
        {
              p_stun_shsec_status_ind->result = NAT_RESULT_SUCCESS;
              p_stun_shsec_status_ind->error_cause = NAT_ERR_NONE;
              
        }
        else
        {
    	      p_stun_shsec_status_ind->result = NAT_RESULT_FAILURE;
              p_stun_shsec_status_ind->error_cause = int_msg_hdr.ecode;
        }

    	ret_val = nat_port_send_msg_to_app(int_msg_hdr.p_glb_pdb,p_shsec_ind_msg);

     }
     else
     {
         /* send internal message to the calling internal module */
         switch(context_data_map.module_id)
         {
         case NAT_STUN_MODULE_LIFETIME:
              nat_stun_lifetime_process_mesg(p_msg);
              break;
              
         case NAT_STUN_MODULE_DISCOVERY:
              nat_stun_discovery_process_mesg(p_msg);
              break;
              
         case NAT_STUN_MODULE_BINDING:
              nat_stun_process_mesg(p_msg);
              break;
         }
     }    
	
	return ret_val;                           
}

/******************************************************************************
 *
 * FUNCTION:        nat_send_discovery_type_resp
 *
 * DESCRIPTION:     This function is invoked to send NAT Type discovery 
 *                  response.
 *      
 * RETURNS:         NAT_RETURN_SUCCESS in case of success and NAT_RETURN_FAILURE  
 *                  in case of any failure.       
 *
 ******************************************************************************/
nat_return_t 
nat_send_discovery_type_resp(
                                 INOUT nat_internal_msg_st        *p_msg)
{
	nat_return_t 	ret_val = NAT_RETURN_SUCCESS;
	
	stun_type_discovery_resp_st *p_int_type_disc_resp = 
                 (stun_type_discovery_resp_st *)p_msg->p_msg_data;
    nat_internal_msg_hdr_st int_msg_hdr  = p_msg->msg_hdr;
    
    nat_context_data_map_st context_data_map = int_msg_hdr.p_glb_pdb->p_glb_data\
                            ->p_stun_context_map[NAT_STUN_MODULE_DISCOVERY];
                                            
	nat_msg_st			*p_type_disc_ind_msg = NAT_NULL;
	nat_error_t			ecode = NAT_ERR_NONE;	
	nat_stun_proc_status_ind_st	*p_stun_proc_status_ind = NAT_NULL;

    /* check if the request was from external module */
    if(NAT_STUN_MODULE_EXTERNAL == context_data_map.module_id)
    {
	p_type_disc_ind_msg = (nat_msg_st *)nat_port_static_memget(
		nat_port_sizeof(nat_msg_hdr_st) + 
		nat_port_sizeof(nat_stun_proc_status_ind_st), &ecode);

    	p_type_disc_ind_msg->msg_hdr.api_id = NAT_STUN_PROC_STATUS_IND;
    	p_type_disc_ind_msg->msg_hdr.app_context_id = 
                                                  context_data_map.app_context_id;
    	p_type_disc_ind_msg->msg_hdr.source_id = 
                                                  context_data_map.destination_id;
    	p_type_disc_ind_msg->msg_hdr.destination_id = 
                                                  context_data_map.source_id;
    	p_type_disc_ind_msg->msg_hdr.api_length = 
                   nat_port_sizeof(nat_msg_hdr_st) + 
                   nat_port_sizeof(nat_stun_proc_status_ind_st);	
    
    	p_stun_proc_status_ind = (nat_stun_proc_status_ind_st *)
                                   p_type_disc_ind_msg->p_msg_data;
                                   
         p_stun_proc_status_ind->execute_procedures = 
                                         NAT_STUN_PROC_TYPE_DISCOVERY_SUPP;
    				
        if(NAT_RESULT_SUCCESS == p_int_type_disc_resp->result)
        {
              p_stun_proc_status_ind->result = NAT_RESULT_SUCCESS;
              p_stun_proc_status_ind->error_cause = NAT_ERR_NONE;
              p_stun_proc_status_ind->nat_type = p_int_type_disc_resp->nat_type;
        }
        else
        {
    	      p_stun_proc_status_ind->result = NAT_RESULT_FAILURE;
              p_stun_proc_status_ind->error_cause = p_int_type_disc_resp->error;
        }

    	ret_val = nat_port_send_msg_to_app(int_msg_hdr.p_glb_pdb,p_type_disc_ind_msg);
	nat_port_static_memfree(p_type_disc_ind_msg,&ecode);
    }
    else
    {
        /* invalid place, raise error */
    }
	
	return ret_val;
}

/******************************************************************************
 *
 * FUNCTION:        nat_send_lifetime_disc_resp
 *
 * DESCRIPTION:     This function is invoked to send lifetime discovery 
 *                  response.
 *      
 * RETURNS:         NAT_RETURN_SUCCESS in case of success and NAT_RETURN_FAILURE  
 *                  in case of any failure.       
 *
 ******************************************************************************/
nat_return_t 
nat_send_lifetime_disc_resp(
                                 INOUT nat_internal_msg_st        *p_msg)
{
	nat_return_t 	ret_val = NAT_RETURN_SUCCESS;
	
	stun_lifetime_discovery_resp_st *p_int_lifetime_disc_resp = 
                 (stun_lifetime_discovery_resp_st *)p_msg->p_msg_data;
    nat_internal_msg_hdr_st int_msg_hdr  = p_msg->msg_hdr;
    
    nat_context_data_map_st context_data_map = int_msg_hdr.p_glb_pdb->p_glb_data\
                            ->p_stun_context_map[NAT_STUN_MODULE_DISCOVERY];
                                            
	nat_msg_st			*p_lifetime_disc_ind_msg = NAT_NULL;
	nat_error_t			ecode = NAT_ERR_UNDEFINED;
	
	nat_stun_proc_status_ind_st	*p_stun_proc_status_ind = NAT_NULL;

    /* check if the request was from external module */
    if(NAT_STUN_MODULE_EXTERNAL == context_data_map.module_id)
    {
    	p_lifetime_disc_ind_msg = (nat_msg_st *)nat_port_static_memget(
                                      nat_port_sizeof(nat_msg_st),&ecode);
    
    	p_lifetime_disc_ind_msg->msg_hdr.api_id = NAT_GET_PUBLIC_IP_PORT_RESP;
    	p_lifetime_disc_ind_msg->msg_hdr.app_context_id = 
                                                  context_data_map.app_context_id;
    	p_lifetime_disc_ind_msg->msg_hdr.source_id = 
                                                  context_data_map.destination_id;
    	p_lifetime_disc_ind_msg->msg_hdr.destination_id = 
                                                  context_data_map.source_id;
    	p_lifetime_disc_ind_msg->msg_hdr.api_length = 
                   nat_port_sizeof(nat_msg_hdr_st) + 
                   nat_port_sizeof(nat_stun_proc_status_ind_st);	
    
    	p_stun_proc_status_ind = (nat_stun_proc_status_ind_st *)
                                   p_lifetime_disc_ind_msg->p_msg_data;
                                   
         p_stun_proc_status_ind->execute_procedures = 
                                         NAT_STUN_PROC_BINDING_LIFETIME_SUPP;
    				
        if(NAT_RESULT_SUCCESS == p_int_lifetime_disc_resp->result)
        {
              p_stun_proc_status_ind->result = NAT_RESULT_SUCCESS;
              p_stun_proc_status_ind->error_cause = NAT_ERR_NONE;
              p_stun_proc_status_ind->bind_discovery_timer = 
                                   p_int_lifetime_disc_resp->lifetime_bind_dur;
        }
        else
        {
    	      p_stun_proc_status_ind->result = NAT_RESULT_FAILURE;
              p_stun_proc_status_ind->error_cause = int_msg_hdr.ecode;
        }

    	ret_val = nat_port_send_msg_to_app(int_msg_hdr.p_glb_pdb,p_lifetime_disc_ind_msg);
    }
    else
    {
        /* invalid place, raise error */
    }
	
	return ret_val;
}


/******************************************************************************
 *
 * FUNCTION:        nat_send_timer_expiry_ind
 *
 * DESCRIPTION:     This function is invoked to process the timer expiry ind
 *      
 * RETURNS:         NAT_RETURN_SUCCESS in case of success and NAT_RETURN_FAILURE in 
 *                  case of any failure.       
 *
 ******************************************************************************/
nat_return_t 
nat_send_timer_expiry_ind(
                    INOUT nat_internal_msg_st        *p_msg)
{
    nat_return_t 	ret_val = NAT_RETURN_SUCCESS;
	
	stun_timer_expire_ind_st *p_timer_exp_ind = 
                 (stun_timer_expire_ind_st *)p_msg->p_msg_data;
    
    if(NAT_STUN_MODULE_SHSEC == p_timer_exp_ind->context_id)
    {
        /* send it to shsec state machine */
    }
    else if(NAT_STUN_MODULE_LIFETIME == p_timer_exp_ind->context_id)
    {
        /* send it to lifetime state machine */
    }
    else if(NAT_STUN_MODULE_DISCOVERY == p_timer_exp_ind->context_id)
    {
        /* send it to type discovery state machine */
    }
    else
    {
        /* validate the context id whether valid or not */

        /* send it to binding state machine */
        ret_val = nat_stun_process_mesg(p_msg);
    }

    
	return ret_val;                           
}

/******************************************************************************
 *
 * FUNCTION:        nat_send_error_ind
 *
 * DESCRIPTION:     This function is invoked to send error indication to APP.
 *      
 * RETURNS:         NAT_RETURN_SUCCESS in case of success and NAT_RETURN_FAILURE in 
 *                  case of any failure.       
 *
 ******************************************************************************/
nat_return_t 
nat_send_error_ind(
                    INOUT nat_internal_msg_st        *p_msg)
{
	nat_return_t 	ret_val = NAT_RETURN_SUCCESS;
 	nat_error_t	ecode= NAT_ERR_NONE;	
	stun_error_ind_st *p_int_error_ind = 
                 (stun_error_ind_st *)p_msg->p_msg_data;

    	nat_internal_msg_hdr_st int_msg_hdr  = p_msg->msg_hdr;
    
    	nat_context_data_map_st context_data_map = int_msg_hdr.p_glb_pdb->p_glb_data\
                            ->p_stun_context_map[p_int_error_ind->context_id];
                                            
	nat_msg_st		*p_error_ind_msg = NAT_NULL;
	
	nat_err_ind_st		*p_stun_error_ind = NAT_NULL;

	p_error_ind_msg = (nat_msg_st *)nat_port_static_memget(nat_port_sizeof(nat_msg_hdr_st) +
				nat_port_sizeof(nat_err_ind_st), &ecode);

    	p_error_ind_msg->msg_hdr.api_id = NAT_ERROR_IND;
    	p_error_ind_msg->msg_hdr.app_context_id = 
                                                  context_data_map.app_context_id;
    	p_error_ind_msg->msg_hdr.source_id = 
                                                  context_data_map.destination_id;
    	p_error_ind_msg->msg_hdr.destination_id = 
                                                  context_data_map.source_id;
    	p_error_ind_msg->msg_hdr.api_length = 
                   nat_port_sizeof(nat_msg_hdr_st) + 
                   nat_port_sizeof(nat_err_ind_st);	
    
    	p_stun_error_ind = (nat_err_ind_st *)
                                   p_error_ind_msg->p_msg_data;
                                   
    				
        p_stun_error_ind->nat_error = p_int_error_ind->error_cause;

    	ret_val = nat_port_send_msg_to_app(int_msg_hdr.p_glb_pdb,p_error_ind_msg);

	nat_port_static_memfree(p_error_ind_msg,&ecode);
	
	return ret_val;
}


/******************************************************************************
 *
 * FUNCTION:        nat_deinit_req
 *
 * DESCRIPTION:     This function is invoked whenever deinit request
 *                  is received from application.
 *                   
 * RETURNS:         NAT_SUCCESS in case of success and NAT_FAILURE in 
 *                  case of any failure.       
 *
 ******************************************************************************/
nat_return_t nat_deinit_req(
        INOUT nat_glb_pdb_st             *p_glb_nat_pdb)
{
	nat_return_t ret_val = NAT_RETURN_SUCCESS;
	nat_internal_msg_st     	internal_msg;

	nat_deinit_req_st *p_deinit_req = (nat_deinit_req_st *)p_glb_nat_pdb->p_recv_msg->p_msg;

	if(NAT_PROTOCOL_STUN == p_deinit_req->nat_protocol)
	{
		internal_msg.msg_hdr.p_glb_pdb = p_glb_nat_pdb;
        	internal_msg.msg_hdr.msg_id = NAT_INT_STUN_DEINIT_REQ;
		internal_msg.p_msg_data = NAT_NULL;

		if(NAT_RETURN_SUCCESS != nat_stun_process_mesg(&internal_msg))
        	{
	      	p_glb_nat_pdb->p_glb_data->ecode = internal_msg.msg_hdr.ecode;
              	ret_val = NAT_RETURN_FAILURE;
              
         	}
	}		
	return ret_val;
}


/******************************************************************************
 *
 * FUNCTION:        nat_validate_new_request
 *
 * DESCRIPTION:     This function is invoked to validate the new Public IP/PORT 
 *                  request, if query already done then sends the response as well.
 *      
 * RETURNS:         NAT_TRUE in case of new request and NAT_FALSE in 
 *                  case old request.       
 *
 ******************************************************************************/
nat_boolean_t 
nat_validate_new_request(
                                 INOUT nat_glb_pdb_st             *p_glb_nat_pdb)
{

    nat_get_public_ip_port_req_st *p_public_ip_port_req = 
                    (nat_get_public_ip_port_req_st *)\
					(p_glb_nat_pdb->p_recv_msg->p_msg->p_msg_data);
					
	nat_msg_hdr_st    		recv_msg_hdr = 
                p_glb_nat_pdb->p_recv_msg->p_msg->msg_hdr;

	nat_return_t 	ret_val = NAT_RETURN_SUCCESS;
	nat_error_t	ecode = NAT_ERR_NONE;	
  
	nat_msg_st			*p_public_ip_port_resp_msg = NAT_NULL;
	nat_get_public_ip_port_resp_st	*p_stun_public_ip_port_resp =  NAT_NULL;
    nat_uint8_t context_id = 0;
        
    NAT_GLB_FIND_APP_CONTEXT_ON_ADDR(
            p_glb_nat_pdb->p_glb_data->p_stun_context_map,
            p_public_ip_port_req->local_address,
            context_id,
            p_glb_nat_pdb->p_glb_data->max_contexts)

    if(context_id == p_glb_nat_pdb->p_glb_data->max_contexts + 3 )
	{
        /* it is a new request */
		return NAT_TRUE;
	}
	else
	{

        /* check if the request was from external module */
	    p_public_ip_port_resp_msg = (nat_msg_st *)nat_port_static_memget(
		        nat_port_sizeof(nat_msg_hdr_st) + 
                nat_port_sizeof(nat_get_public_ip_port_resp_st),
		        &ecode);

        p_public_ip_port_resp_msg->msg_hdr.api_id = NAT_GET_PUBLIC_IP_PORT_RESP;
        p_public_ip_port_resp_msg->msg_hdr.app_context_id = 
                                                  recv_msg_hdr.app_context_id;
        p_public_ip_port_resp_msg->msg_hdr.source_id = 
                                                  recv_msg_hdr.destination_id;
        p_public_ip_port_resp_msg->msg_hdr.destination_id = 
                                                  recv_msg_hdr.source_id;
        p_public_ip_port_resp_msg->msg_hdr.api_length = 
                   nat_port_sizeof(nat_msg_hdr_st) + 
                   nat_port_sizeof(nat_get_public_ip_port_resp_st);	
    
        p_stun_public_ip_port_resp = (nat_get_public_ip_port_resp_st *)
                                   p_public_ip_port_resp_msg->p_msg_data;
                               
         p_stun_public_ip_port_resp->nat_protocol = NAT_PROTOCOL_STUN;
    				    
         p_stun_public_ip_port_resp->result = NAT_RESULT_SUCCESS;
         p_stun_public_ip_port_resp->error_cause = NAT_ERR_NONE;
         nat_port_memcpy(&p_stun_public_ip_port_resp->public_addr,
                      &(((stun_data_st *)p_glb_nat_pdb->p_glb_data->p_stun_data)
                        ->p_stun_contexts[context_id].mapped_addr),
                      nat_port_sizeof(nat_ip_port_address_st));
    
        ret_val = nat_port_send_msg_to_app(p_glb_nat_pdb,p_public_ip_port_resp_msg);
	    nat_port_static_memfree(p_public_ip_port_resp_msg,&ecode);
    
	
	    return NAT_FALSE;
    }
}


