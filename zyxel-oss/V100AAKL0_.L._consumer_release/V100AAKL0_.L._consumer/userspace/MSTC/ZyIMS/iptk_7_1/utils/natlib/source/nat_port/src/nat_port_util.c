/******************************************************************************
*                                     
* FILE NAME    : nat_port_util.c                
*                                    
* DESCRIPTION  : Contains the platform specific declarations.
*                
*                                    
* Revision History :                            
*
*  DATE            NAME                        REFERENCE       REASON        
* ------         ------------                  ---------     ----------
* 14-Jul-2006    Anuradha Gupta                LLD           Initial version
* 01-Dec-2006	 Anuradha Gupta					LLD			 Coding
* Copyright 2006, Aricent                      
*                                    
*******************************************************************************/

#include "nat_types.h"
#include "nat_port_pltfm_prototypes.h"
#include "nat_port_prototypes.h"
#include "nat_error_id.h"
#ifdef NAT_PORT_WINDOWS
#include "nat_glb_mgmt_prototypes.h"
#endif

/******************************************************************************
 *
 * FUNCTION:        nat_port_send_msg_to_app
 * 
 * DESCRIPTION:     This function is invoked to send message to application 
 *                                    
 * RETURNS:         NAT_RETURN_SUCCESS in case of success and NAT_RETURN_FAILURE  
 *                  in case of any failure.                  
 *
 *****************************************************************************/
nat_return_t	
nat_port_send_msg_to_app(nat_void_t     *p_nat_glb_pdb,
                         nat_msg_st		*p_nat_msg)
{
    nat_return_t ret_val = NAT_RETURN_SUCCESS;
    nat_print_msg_hdr(p_nat_msg); 
    p_nat_msg->msg_hdr.destination_id = NAT_MODULE_APP;
    ret_val = (((nat_glb_pdb_st *)p_nat_glb_pdb)->es_nat_send_app_data)
                            ((nat_void_t *)p_nat_msg);

    return ret_val;
}

/******************************************************************************
 *
 * FUNCTION:        nat_port_open_udp_socket
 * 
 * DESCRIPTION:     This function is invoked to open UDP socket 
 *                                    
 * RETURNS:         NAT_RETURN_SUCCESS in case of success and NAT_RETURN_FAILURE  
 *                  in case of any failure.                  
 *
 *****************************************************************************/
nat_return_t	
nat_port_open_udp_socket(nat_void_t     *p_nat_glb_pdb,
                         nat_ip_port_address_st *p_local_addr)
{
    nat_return_t ret_val = NAT_RETURN_SUCCESS;
    
    ret_val = (((nat_glb_pdb_st *)p_nat_glb_pdb)->es_nat_open_udp_socket)(p_local_addr);
    
    return ret_val;
}

/******************************************************************************
 *
 * FUNCTION:        nat_port_close_udp_socket
 * 
 * DESCRIPTION:     This function is invoked to close UDP socket 
 *                                    
 * RETURNS:         NAT_RETURN_SUCCESS in case of success and NAT_RETURN_FAILURE  
 *                  in case of any failure.                  
 *
 *****************************************************************************/
nat_return_t	
nat_port_close_udp_socket(nat_void_t     *p_nat_glb_pdb,
                          nat_ip_port_address_st *p_local_addr)
{
    nat_return_t ret_val = NAT_RETURN_SUCCESS;
    
    ret_val = (((nat_glb_pdb_st *)p_nat_glb_pdb)->es_nat_close_udp_socket)(p_local_addr);
    
    return ret_val;
}

/******************************************************************************
 *
 * FUNCTION:        nat_port_open_secure_tcp_socket
 * 
 * DESCRIPTION:     This function is invoked to open secure TCP connection 
 *                                    
 * RETURNS:         NAT_RETURN_SUCCESS in case of success and NAT_RETURN_FAILURE  
 *                  in case of any failure.                  
 *
 *****************************************************************************/
nat_return_t	
nat_port_open_secure_tcp_socket(
                                nat_ip_port_address_st *p_local_addr,
								nat_ip_port_address_st * p_dest_addr)
{
       nat_return_t ret_val = NAT_RETURN_SUCCESS;
#ifdef NAT_PORT_WINDOWS
	p_local_addr = p_local_addr;
	p_dest_addr = p_dest_addr;
#endif
       return ret_val;
}

/******************************************************************************
 *
 * FUNCTION:        nat_port_close_secure_tcp_socket
 * 
 * DESCRIPTION:     This function is invoked to close secure TCP connection 
 *                                    
 * RETURNS:         NAT_RETURN_SUCCESS in case of success and NAT_RETURN_FAILURE  
 *                  in case of any failure.                  
 *
 *****************************************************************************/
nat_return_t	
nat_port_close_secure_tcp_socket(
                                 nat_ip_port_address_st *p_local_addr)
{
       nat_return_t ret_val = NAT_RETURN_SUCCESS;
#ifdef NAT_PORT_WINDOWS
	p_local_addr = p_local_addr;
#endif
       return ret_val;
}

/******************************************************************************
 *
 * FUNCTION:        nat_port_send_udp_data
 * 
 * DESCRIPTION:     This function is invoked to send data on UDP 
 *                                    
 * RETURNS:         NAT_RETURN_SUCCESS in case of success and NAT_RETURN_FAILURE  
 *                  in case of any failure.                  
 *
 *****************************************************************************/
nat_return_t	
nat_port_send_udp_data(nat_void_t     *p_nat_glb_pdb,
                       nat_ip_port_address_st *p_local_addr,
                       nat_ip_port_address_st * p_dest_addr,
                       nat_octet_string_st *p_enc_buff)
{
    nat_return_t ret_val = NAT_RETURN_SUCCESS;
    nat_msg_st msg_data;
    nat_nw_msg_data_st  *p_nw_data = NAT_NULL;


    /* create the message */
    
    msg_data.msg_hdr.destination_id = NAT_MODULE_NW_UDP;
    msg_data.msg_hdr.source_id = NAT_MODULE_NAT;
    msg_data.msg_hdr.app_context_id = 0;
    msg_data.msg_hdr.api_id = 0;
    msg_data.msg_hdr.api_length = nat_port_sizeof(nat_msg_hdr_st) + \
        p_enc_buff->octet_len;
        
    p_nw_data = (nat_nw_msg_data_st  *)msg_data.p_msg_data;
    p_nw_data->p_buffer_data = p_enc_buff;

    ret_val = (((nat_glb_pdb_st *)p_nat_glb_pdb)->es_nat_send_udp_data)(
                    p_local_addr,p_dest_addr,&msg_data);
    
    return ret_val;
}

/******************************************************************************
 *
 * FUNCTION:        nat_port_send_mm_data
 * 
 * DESCRIPTION:     This function is invoked to send data on UDP through MM
 *                                    
 * RETURNS:         NAT_RETURN_SUCCESS in case of success and NAT_RETURN_FAILURE  
 *                  in case of any failure.                  
 *
 *****************************************************************************/
nat_return_t	
nat_port_send_mm_data(nat_void_t     *p_nat_glb_pdb,
                       nat_ip_port_address_st *p_local_addr,
                       nat_ip_port_address_st * p_dest_addr,
                       nat_octet_string_st *p_enc_buff)
{
   nat_return_t ret_val = NAT_RETURN_SUCCESS;
    nat_msg_st msg_data;
    nat_nw_msg_data_st  *p_nw_data = NAT_NULL;


    /* create the message */
    
    msg_data.msg_hdr.destination_id = NAT_MODULE_NW_UDP;
    msg_data.msg_hdr.source_id = NAT_MODULE_NAT;
    msg_data.msg_hdr.app_context_id = 0;
    msg_data.msg_hdr.api_id = 0;

    msg_data.msg_hdr.api_length = nat_port_sizeof(nat_msg_hdr_st) + \
        p_enc_buff->octet_len;
        
    p_nw_data = (nat_nw_msg_data_st  *)msg_data.p_msg_data;
    p_nw_data->p_buffer_data = p_enc_buff;

    ret_val = (((nat_glb_pdb_st *)p_nat_glb_pdb)->es_nat_send_mm_data)(
                    p_local_addr,p_dest_addr,&msg_data);
 
    return ret_val;
}

/******************************************************************************
 *
 * FUNCTION:        nat_port_send_secure_tcp_data
 * 
 * DESCRIPTION:     This function is invoked to send data on secure TCP line 
 *                                    
 * RETURNS:         NAT_RETURN_SUCCESS in case of success and NAT_RETURN_FAILURE  
 *                  in case of any failure.                  
 *
 *****************************************************************************/
nat_return_t	
nat_port_send_secure_tcp_data(
                             nat_ip_port_address_st *p_local_addr,
                             nat_octet_string_st *p_enc_buff)
{
       nat_return_t ret_val = NAT_RETURN_SUCCESS;
#ifdef NAT_PORT_WINDOWS
	p_local_addr = p_local_addr;
	p_enc_buff = p_enc_buff;
#endif
       return ret_val;
}

/******************************************************************************
 *
 * FUNCTION:        nat_port_start_timer
 * 
 * DESCRIPTION:     Common function to be invoked to start Timer synchrounously
 *                  or asynchronously 
 *                                    
 * RETURNS:         NAT_RETURN_SUCCESS in case of success and NAT_RETURN_FAILURE  
 *                  in case of any failure and NAT_RETURN_PENDING for asynchronous
 *                  Timer.                
 *
 *****************************************************************************/
nat_return_t	
nat_port_start_timer(nat_void_t     *p_nat_glb_pdb,
                     nat_timer_data_st   *p_timer_data,
                     nat_duration_t      timer_dur,
                     nat_timer_id_t		 *p_timer_id)
{
    nat_return_t ret_val = NAT_RETURN_SUCCESS;
    
    ret_val = (((nat_glb_pdb_st *)p_nat_glb_pdb)->es_nat_start_timer)(
        p_timer_data,timer_dur,p_timer_id);
    
    return ret_val;
}

/******************************************************************************
 *
 * FUNCTION:        nat_port_stop_timer
 * 
 * DESCRIPTION:     Common function to be invoked to stop Timer synchrounously
 *                  or asynchronously 
 *                                    
 * RETURNS:         NAT_RETURN_SUCCESS in case of success and NAT_RETURN_FAILURE  
 *                  in case of any failure and NAT_RETURN_PENDING for asynchronous
 *                  Timer.                
 *
 *****************************************************************************/
nat_return_t	
nat_port_stop_timer(nat_void_t     *p_nat_glb_pdb,
                    nat_timer_id_t timer_id)
{
    nat_return_t ret_val = NAT_RETURN_SUCCESS;
    
    ret_val = (((nat_glb_pdb_st *)p_nat_glb_pdb)->es_nat_stop_timer)(timer_id);
    
    return ret_val;
}

