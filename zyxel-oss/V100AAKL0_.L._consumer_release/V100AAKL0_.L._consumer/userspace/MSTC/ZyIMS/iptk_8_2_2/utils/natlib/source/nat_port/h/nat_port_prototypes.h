/******************************************************************************
*                                    
* FILE NAME    : nat_port_prototypes.h                
*                                    
* DESCRIPTION  : Contains the definitions for the structures used in NAT Client 
*                Prototypes APIs.    
*                                    
* Revision History :                            
*
*  DATE            NAME                        REFERENCE       REASON        
* ------         ------------                  ---------     ----------
* 20-Nov-2006    Anuradha Gupta                  HLD         Initial Version
*
* Copyright 2006, Aricent                    
*                                    
*******************************************************************************/



#ifndef __NAT_PORT_PROTOTYPES_H_

#define __NAT_PORT_PROTOTYPES_H_

#include "nat_types.h"


#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

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
nat_port_send_msg_to_app(nat_void_t *p_glb_nat_pdb,
                         nat_msg_st		*p_nat_msg);

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
                         nat_ip_port_address_st *p_local_addr);

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
                          nat_ip_port_address_st *p_local_addr);

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
								nat_ip_port_address_st * p_dest_addr);

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
                                 nat_ip_port_address_st *p_local_addr);

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
                       nat_octet_string_st *p_enc_buff);

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
                             nat_octet_string_st *p_enc_buff);


/* These interfacea can be asynchronous as well as synchronous */

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
                     nat_timer_id_t		 *p_timer_id);

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
                    nat_timer_id_t timer_id);

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
                       nat_octet_string_st *p_enc_buff);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /*__NAT_PORT_PRTOTYPES_H_*/




