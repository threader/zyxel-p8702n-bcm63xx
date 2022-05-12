/******************************************************************************
*                                    
* FILE NAME    : nat_api.h                
*                                    
* DESCRIPTION  : Contains the definitions for the structures used in NAT CLIENT
*                APIs.    
*                                    
* Revision History :                            
*
*  DATE            NAME            REFERENCE         REASON        
* ------         ------------     ---------        ----------
* 24-Nov-2006	Anuradha Gupta		NAT API Doc	    Initial Version
*
* Copyright 2006, Aricent                      
*                                    
*******************************************************************************/

#ifndef __NAT_API_H__
#define __NAT_API_H__

#include "nat_defs.h"
#include "nat_types.h"

/* Prevent Name Mangling By C++ Compilers */

#ifdef __cplusplus

extern "C"
{
#endif /* __cplusplus */

/*------------------------------------------------------------------------------
 *
 * Type Name   :  nat_stun_init_req_st 
 * Description :  Request to initialize STUN protocol module and also request
 *				  to execute procedures.
 *
 *----------------------------------------------------------------------------*/
typedef struct
{
    nat_uint8_t          start_procedures;
/*
	This is a bitmask to start multiple procedures and has values :
	#define NAT_STUN_PROC_NOT_PRESENT				0x00
	#define NAT_STUN_PROC_BINDING_LIFETIME_SUPP           0x01
	#define NAT_STUN_PROC_TYPE_DISCOVERY_SUPP			0x02
	#define NAT_STUN_PROC_SHARED_SECRET_SUPP              0x04 
*/

	/* Maximum number of contexts to be supported for Binding requests */
    nat_uint8_t          max_contexts;
 
}nat_stun_init_req_st;


/*------------------------------------------------------------------------------
 *
 * Type Name   :  nat_init_resp_st 
 * Description :  Response for the initialization request.
 *
 *----------------------------------------------------------------------------*/
typedef struct
{
	/* nat_result -- This can have foolowing values :
				  NAT_RESULT_FAILURE -  0
				  NAT_RESULT_SUCCESS -  1
	*/
    nat_result_t          result;

	/* If the result is failure the error cause */
    nat_error_t           error_cause;
}nat_init_resp_st;


/*------------------------------------------------------------------------------
 *
 * Type Name   :  nat_stun_init_resp_st 
 * Description :  Response for the STUN initialization request.
 *
 *----------------------------------------------------------------------------*/
typedef nat_init_resp_st nat_stun_init_resp_st;

/*------------------------------------------------------------------------------
 *
 * Type Name   :  nat_stun_exec_proc_req_st 
 * Description :  Request for execution of STUN procedures..
 *
 *----------------------------------------------------------------------------*/
typedef struct
{
    nat_uint8_t         execute_procedures;
	/*	
	This is a bitmask to execute multiple procedures and has values :

	#define NAT_STUN_PROC_BINDING_LIFETIME_SUPP         0x01
	#define NAT_STUN_PROC_TYPE_DISCOVERY_SUPP			0x02
	#define NAT_STUN_PROC_SHARED_SECRET_SUPP            0x04 
*/
}nat_stun_exec_proc_req_st;


/*------------------------------------------------------------------------------
 *
 * Type Name   :  nat_stun_exec_proc_resp_st 
 * Description :  Response for the Procedure execution request.
 *
 *----------------------------------------------------------------------------*/
typedef nat_init_resp_st nat_stun_exec_proc_resp_st;

/*------------------------------------------------------------------------------
 *
 * Type Name   :  nat_stun_proc_status_ind_st 
 * Description :  The indication API for the result of the procedures.
 *
 *----------------------------------------------------------------------------*/
typedef struct
{
    /* This Name of the requested procedure. */
	nat_uint8_t         execute_procedures;

	/* Result of the procedure execution */
	nat_result_t        result;

	/* OPTIONAL : the binding lifetime value for the 
	 * NAT_STUN_PROC_BINDING_LIFETIME_SUPP procedure */
	nat_duration_t      bind_discovery_timer;

	/* OPTIONAL : the NAT type value for the 
	 * NAT_STUN_PROC_TYPE_DISCOVERY_SUPP procedure */
	nat_uint8_t         nat_type;
	/* 
	* #define NAT_PRIVATE_NETWORK_ONLY        0x01
	* #define NAT_OPEN_INTERNET_INTERFACE     0x02
	* #define NAT_FULL_CONE                   0x03
	* #define NAT_RESTRICTED                  0x04
	* #define NAT_PORT_RESTRICTED             0x05
	* #define NAT_SYMMETRIC                   0x06
	* #define NAT_INVALID_TYPE                0xff
	*/
	
	/* If the result is failure the error cause */
	nat_error_t	    error_cause;
	
}nat_stun_proc_status_ind_st;

/*------------------------------------------------------------------------------
 *
 * Type Name   :  nat_deinit_req_st 
 * Description :  Deinit request for the NAT protocol.
 *
 *----------------------------------------------------------------------------*/
typedef struct
{
    nat_uint8_t         nat_protocol;
	/* 
	* #define NAT_DISCOVERY_STUN		0x01
	* #define NAT_DISCOVERY_TURN		0x02
	* #define NAT_DISCOVERY_ICE			0x04
	*/

} nat_deinit_req_st;

/*------------------------------------------------------------------------------
 *
 * Type Name   :  nat_get_public_ip_port_req_st 
 * Description :  The request for resolving the local address to public address.
 *
 *----------------------------------------------------------------------------*/
typedef struct
{
	/* 
	*	This is a bitmask specifying the protcols which should be used for 
	*	finding the public ip/port.The success response is sent by the first  
	*	protocol which is successful in finding this binding.
	*/
    nat_uint8_t			nat_protocol;

	/* The IPV$ address for the local entity */
    nat_ip_port_address_st    	local_address;

	/* Flag to depict if the refresh is required for the NAT pin hole */
    nat_boolean_t		is_refresh_enabled;

} nat_get_public_ip_port_req_st;

/*------------------------------------------------------------------------------
 *
 * Type Name   :  nat_get_public_ip_port_resp_st 
 * Description :  The response for resolving the local address to public address.
 *
 *----------------------------------------------------------------------------*/
typedef struct
{
	/* The protocol used for resolving the request */
	nat_uint8_t		             nat_protocol;

	/* Result of the request , SUCCESS / FAILURE */
	nat_result_t                 result;

	/* Error cause in case of failure */
   	nat_error_t		             error_cause;

	/* the resolved public address in case of success */
   	nat_ip_port_address_st       public_addr;
   
 } nat_get_public_ip_port_resp_st;

/*------------------------------------------------------------------------------
 *
 * Type Name   :  nat_rel_public_ip_port_req_st 
 * Description :  The request for releasing the binding. no message payload
 *
 *----------------------------------------------------------------------------*/


/*------------------------------------------------------------------------------
 *
 * Type Name   :  nat_recv_udp_pkt_ind_st 
 * Description :  The indication for receipt of UDP packet.
 *
 *----------------------------------------------------------------------------*/
typedef struct
{
	/* The local address on which the packet is received */
    nat_ip_port_address_st    local_addr;

	/* the UDP octet data */
    nat_octet_string_st		  octets;
 } nat_recv_udp_pkt_ind_st;

/*------------------------------------------------------------------------------
 *
 * Type Name   :  nat_recv_sec_tcp_pkt_ind_st 
 * Description :  The indication for receipt of secure TCP packet.
 *
 *----------------------------------------------------------------------------*/
typedef struct
{
	/* The local address on which the packet is received */
    nat_ip_port_address_st    local_addr;

	/* the TCP octet data */
    nat_octet_string_st		  octets;
} nat_recv_sec_tcp_pkt_ind_st;


/*------------------------------------------------------------------------------
 *
 * Type Name   :  nat_err_ind_st 
 * Description :  The indication about any error in the NAT library.
 *
 *----------------------------------------------------------------------------*/
typedef struct
{
	/* The error ID */
    nat_error_t        nat_error;
} nat_err_ind_st;

/***********************************************************************
 * Follwoing APIs are used for Asynchronous timer handling
 *                
 ************************************************************************/

/*------------------------------------------------------------------------------
 *
 * Type Name   :  nat_timer_start_req_st 
 * Description :  The request to start the timer.
 *
 *----------------------------------------------------------------------------*/
typedef struct
{
	/* The timer data to be returned on expiry of timer */
    nat_timer_data_st   timer_data;

	/* The duration for which the timer is to be started */
    nat_duration_t      timer_dur;
} nat_timer_start_req_st;

/*------------------------------------------------------------------------------
 *
 * Type Name   :  nat_timer_start_resp_st 
 * Description :  The request to start the timer.
 *
 *----------------------------------------------------------------------------*/
typedef struct
{
	/* the timer data */
    nat_timer_data_st   timer_data;

	/* The ID of the timer */
    nat_timer_id_t      timer_id;

    nat_uint8_t         padding[3];
} nat_timer_start_resp_st;

/*------------------------------------------------------------------------------
 *
 * Type Name   :  nat_timer_stop_req_st 
 * Description :  The request to stop the timer.
 *
 *----------------------------------------------------------------------------*/
typedef struct
{
	/* The timer ID */
    nat_timer_id_t        timer_id;
} nat_timer_stop_req_st;


/*------------------------------------------------------------------------------
 *
 * Type Name   :  nat_timer_expiry_ind_st 
 * Description :  The indication for timer expiry
 *
 *----------------------------------------------------------------------------*/
typedef struct
{
	/* the timer data */
    nat_timer_data_st   timer_data;

	/* The timer ID */
    nat_timer_id_t      timer_id;

    nat_uint8_t         padding[3];
} nat_timer_expiry_ind_st;


#ifdef __cplusplus

}

#endif /* __cplusplus */

#endif /* __NAT_API_H__ */


