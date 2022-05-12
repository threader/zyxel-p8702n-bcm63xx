/******************************************************************************
*                                    
* FILE NAME    : nat_glb_mgmt_prototypes.h                
*                                    
* DESCRIPTION  : Contains the definitions for the structures used in NAT Global
*				 Management Prototypes APIs.    
*                                    
* Revision History :                            
*
*  DATE            NAME            REFERENCE       REASON        
* ------         ------------      ---------     ----------
* 24-Nov-2006	Anuradha Gupta     LLD			 Initial Version
*
* Copyright 2006, Aricent                     
*                                    
*******************************************************************************/



#ifndef __NAT_GLB_MGMT_PROTOTYPES_H_

#define __NAT_GLB_MGMT_PROTOTYPES_H_

#ifdef __cplusplus

extern "C"
{
#endif /* __cplusplus */

#include "nat_glb_mgmt_types.h"


extern nat_glb_pdb_st      *p_nat_global_pdb;

/*******************************************************************************
 *
 * FUNCTION:        nat_init
 *
 * DESCRIPTION:     This is the initialization function of Global NAT module
 *				
 *                  
 * RETURNS:         NAT_RETURN_SUCCESS in case of success and NAT_RETURN_FAILURE
 *                  in case of failure.                 
 *
 ******************************************************************************/
nat_return_t nat_init(nat_void_t **p_p_nat_glb_data);

/*******************************************************************************
 *
 * FUNCTION:        nat_print_msg_hdr
 *
 * DESCRIPTION:     Prints the message Header
 *                  
 * RETURNS:         nothing.                 
 *
 ******************************************************************************/
nat_void_t nat_print_msg_hdr(
        IN	 nat_msg_st        *p_msg);

/*******************************************************************************
 *
 * FUNCTION:        nat_process_mesg
 *
 * DESCRIPTION:     This is the entry point function of NAT Traversal Client that is 
 * 					invoked by application to send a external message to the library
  *					The function checks the API Id in the message 
 *                  received and invokes appropriate handler
 *                  
 * RETURNS:         NAT_RETURN_SUCCESS in acse of success and NAT_RETURN_FAILURE 
 *                  in case of failure.                 
 *
 ******************************************************************************/
nat_return_t nat_process_mesg(
        INOUT	 nat_msg_st        *p_msg);

/*******************************************************************************
 *
 * FUNCTION:        nat_process_internal_mesg
 *
 * DESCRIPTION:     This is the function of NAT Traversal Client that is 
 * 					invoked by internal modules to send a external message to the application
  *					The function checks the internal API Id in the message 
 *                  received and creates appropriate external message and sends it.
 *                  
 * RETURNS:         NAT_RETURN_SUCCESS in acse of success and NAT_RETURN_FAILURE 
 *                  in case of failure.                 
 *
 ******************************************************************************/
nat_return_t nat_process_internal_mesg(
        INOUT	 nat_internal_msg_st        *p_msg);


/******************************************************************************
 *
 * FUNCTION:        nat_stun_init_req
 *
 * DESCRIPTION:     This function is invoked whenever stun init request
 *                  is received from application.
 *                   
 * RETURNS:         NAT_SUCCESS in case of success and NAT_FAILURE in 
 *                  case of any failure.       
 *
 ******************************************************************************/
nat_return_t nat_stun_init_req(
        INOUT nat_glb_pdb_st             *p_glb_nat_pdb);

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
        INOUT nat_glb_pdb_st             *p_glb_nat_pdb);


/******************************************************************************
 *
 * FUNCTION:        nat_stun_exec_proc_req
 *
 * DESCRIPTION:     This function is invoked whenever stun init request
 *                  is received from application.
 *                   
 * RETURNS:         NAT_SUCCESS in case of success and NAT_FAILURE in 
 *                  case of any failure.       
 *
 ******************************************************************************/
nat_return_t nat_stun_exec_proc_req(
        INOUT nat_glb_pdb_st             *p_glb_nat_pdb);


/******************************************************************************
 *
 * FUNCTION:        nat_stun_get_public_ip_port_req
 *
 * DESCRIPTION:     This function is invoked whenever stun get public Ip-Port request
 *                  is received from application.
 *             *      
 * RETURNS:         NAT_SUCCESS in case of success and NAT_FAILURE in 
 *                  case of any failure.       
 *
 ******************************************************************************/
nat_return_t nat_get_public_ip_port_req(
        INOUT nat_glb_pdb_st             *p_glb_nat_pdb);

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
        INOUT nat_glb_pdb_st             *p_glb_nat_pdb);


/******************************************************************************
 *
 * FUNCTION:        nat_process_udp_pkt_ind
 *
 * DESCRIPTION:     This function is invoked whenever stun receives UDP Packet 
 *                  Indication from application.
 *                 
 * RETURNS:         NAT_SUCCESS in case of success and NAT_FAILURE in 
 *                  case of any failure.       
 *
 ******************************************************************************/
nat_return_t nat_process_udp_pkt_ind(
        INOUT nat_glb_pdb_st             *p_glb_nat_pdb);

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
        INOUT nat_glb_pdb_st             *p_glb_nat_pdb);

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
        INOUT nat_glb_pdb_st             *p_glb_nat_pdb);


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
        INOUT nat_glb_pdb_st             *p_glb_nat_pdb);


/******************************************************************************
 *
 * FUNCTION:        nat_send_get_public_ip_port_resp
 *
 * DESCRIPTION:     This function is invoked whenever IP-PORT response
 *                  is received from internal modules.
 *                
 * RETURNS:         NAT_RETURN_SUCCESS in case of success and NAT_RETURN_FAILURE in 
 *                  case of any failure.       
 *
 ******************************************************************************/
nat_return_t nat_send_get_public_ip_port_resp(
        INOUT nat_internal_msg_st        *p_msg);

/******************************************************************************
 *
 * FUNCTION:        nat_send_shsec_resp
 *
 * DESCRIPTION:     This function is invoked whenever Share Security response
 *                  is received from internal modules.
 *                
 * RETURNS:         NAT_RETURN_SUCCESS in case of success and NAT_RETURN_FAILURE in 
 *                  case of any failure.       
 *
 ******************************************************************************/
nat_return_t nat_send_shsec_resp(
        INOUT nat_internal_msg_st        *p_msg);

/******************************************************************************
 *
 * FUNCTION:        nat_send_discovery_type_resp
 *
 * DESCRIPTION:     This function is invoked whenever Type Discovery response
 *                  is received from internal modules.
 *                
 * RETURNS:         NAT_RETURN_SUCCESS in case of success and NAT_RETURN_FAILURE in 
 *                  case of any failure.       
 *
 ******************************************************************************/
nat_return_t nat_send_discovery_type_resp(
        INOUT nat_internal_msg_st        *p_msg);

/******************************************************************************
 *
 * FUNCTION:        nat_send_lifetime_disc_resp
 *
 * DESCRIPTION:     This function is invoked whenever Binding Lifetime response
 *                  is received from internal modules.
 *                
 * RETURNS:         NAT_RETURN_SUCCESS in case of success and NAT_RETURN_FAILURE in 
 *                  case of any failure.       
 *
 ******************************************************************************/
nat_return_t nat_send_lifetime_disc_resp(
        INOUT nat_internal_msg_st        *p_msg);



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
        INOUT nat_glb_pdb_st             *p_glb_nat_pdb);

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
        INOUT nat_glb_pdb_st             *p_glb_nat_pdb);

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
        INOUT nat_glb_pdb_st             *p_glb_nat_pdb);

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
        INOUT nat_glb_pdb_st             *p_glb_nat_pdb);

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
        INOUT nat_glb_pdb_st             *p_glb_nat_pdb);

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
	IN nat_uint8_t          	procedure_name);

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
	IN nat_uint8_t          	procedure_name);


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
        INOUT nat_glb_pdb_st             *p_glb_nat_pdb);
       
       
/******************************************************************************
 *
 * FUNCTION:        nat_send_get_public_ip_port_resp
 *
 * DESCRIPTION:     This function is invoked to send Public IP/PORT success .
 *                  response.
 *      
 * RETURNS:         NAT_RETURN_SUCCESS in case of success and NAT_RETURN_FAILURE in 
 *                  case of any failure.       
 *
 ******************************************************************************/
nat_return_t 
nat_send_get_public_ip_port_resp(
                                 INOUT nat_internal_msg_st        *p_msg);
                                 
                          
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
                    INOUT nat_internal_msg_st        *p_msg);
                    
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
                                 INOUT nat_internal_msg_st        *p_msg);
                                 
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
                                 INOUT nat_internal_msg_st        *p_msg);
                          
                                  
/****************************************************************************
** Function: 	nat_read_conf_file
**
** DESCRIPTION: This function reads the configuration file and stores
**              in a structure p_file.
**              Comments in the configuration files are not stored.
** 
***************************************************************************/

nat_return_t nat_read_conf_file(
			nat_void_t* fp, nat_conf_file_st* p_file);


/****************************************************************************
** Function: 	nat_update_conf_info
**
** DESCRIPTION: This function updates the configuration info.
** 
***************************************************************************/
nat_return_t nat_update_conf_info(nat_void_t);

/****************************************************************************

** Function: nat_str_trim 
**
** DESCRIPTION: This function removes extra TAB/spaces from start and 
**              end of the string. Replaces all tabs by space in the string.
** 
** 
**
***************************************************************************/
nat_void_t nat_str_trim(INOUT nat_uint8_t * str);

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
                    INOUT nat_internal_msg_st        *p_msg);

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
                    INOUT nat_internal_msg_st        *p_msg);

/*******************************************************************************
 *
 * FUNCTION:        nat_deinit
 *
 * DESCRIPTION:     This is the deinitialiation function of NAT Client Library
 *                  
 * RETURNS:         nothing                
 *
 ******************************************************************************/
nat_void_t nat_deinit(nat_void_t);

/****************************************************************************
** Function: 	nat_init_config_data
**
** DESCRIPTION: This function initializes the config data.
** 
***************************************************************************/
nat_void_t nat_init_config_data(stun_config_data_st **p_p_config_data);

/****************************************************************************
** Function: 	nat_validate_config_data
**
** DESCRIPTION: This function validates the config data.
** 
***************************************************************************/
nat_return_t nat_validate_config_data(stun_config_data_st **p_p_config_data);

/****************************************************************************
** Function:    nat_print_external_msg_id
**
** DESCRIPTION: This function prints the external message ID.
**
***************************************************************************/
nat_void_t nat_print_external_msg_id(nat_uint8_t api_id);

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
                                 INOUT nat_glb_pdb_st             *p_glb_nat_pdb);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /*__NAT_GLB_MGMT_PRTOTYPES_H_ */




