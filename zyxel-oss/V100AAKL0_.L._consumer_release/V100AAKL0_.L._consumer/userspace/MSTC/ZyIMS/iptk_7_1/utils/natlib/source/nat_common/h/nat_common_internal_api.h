/******************************************************************************
*                                    
* FILE NAME    : nat_common_internal_api.h                
*                                    
* DESCRIPTION  : Contains the definitions for the internal apis
*                                   
* Revision History :                            
*
*  DATE            NAME                        REFERENCE       REASON        
* ------         ------------                  ---------     ----------
* 5-JULY-2006    Anuradha Gupta                LLD           Initial version
* 14-Oct-2009    Anurag Khare                  SPR 20145     Fix for CSR
*                                                            1-7873034
*
* Copyright 2009, Aricent                       
*                                    
*******************************************************************************/


#ifndef __NAT_COMMON_INTERNAL_API_H__
#define __NAT_COMMON_INTERNAL_API_H__

/* Prevent Name Mangling By C++ Compilers */
#include "nat_defs.h"
#include "nat_types.h"
#include "nat_common_types.h"

#ifdef __cplusplus

extern "C"
{
#endif /* __cplusplus */

/*------------------------------------------------------------------------------
 *
 * Type Name   :  stun_init_req_st 
 * Description :  Request to initialize STUN protocol module 
 *
 *----------------------------------------------------------------------------*/
typedef struct
{
	/* maximum context allocated for binding request */
	nat_uint8_t         max_contexts;
} stun_init_req_st;

/*------------------------------------------------------------------------------
 *
 * Type Name   :  stun_shsec_init_req_st 
 * Description :  Request to initialize SHSEC state machine 
 *
 *----------------------------------------------------------------------------*/
typedef struct
{
	/* SHSEC refresh timer duration */
	nat_duration_t  timer_dur;
} stun_shsec_init_req_st;

/*------------------------------------------------------------------------------
 *
 * Type Name   :  stun_discovery_init_req_st 
 * Description :  Request to initialize NAT Type discovery state machine 
 *
 *----------------------------------------------------------------------------*/
typedef stun_shsec_init_req_st  stun_discovery_init_req_st;

/*------------------------------------------------------------------------------
 *
 * Type Name   :  stun_lifetime_init_req_st 
 * Description :  Request to initialize NAT binding lifetime state machine 
 *
 *----------------------------------------------------------------------------*/
typedef struct
{
	/* refresh timer duration */
	nat_duration_t    timer_dur;

	/* the delta interval till the procedure is to be repeated */
	nat_duration_t    delta_interval;
}stun_lifetime_init_req_st;

/*------------------------------------------------------------------------------
 *
 * Type Name   :  stun_get_public_ip_port_req_st 
 * Description :  Binding request message 
 *
 *----------------------------------------------------------------------------*/
typedef struct
{
    nat_uint8_t        			context_id;
    nat_ip_port_address_st    	local_addr;
    nat_ip_port_address_st    	destination_addr;
    nat_ip_port_address_st    	response_addr;
    nat_boolean_t				change_ip;
    nat_boolean_t				change_port;
    nat_boolean_t				is_refresh_enabled;
} stun_get_public_ip_port_req_st;

/*------------------------------------------------------------------------------
 *
 * Type Name   :  stun_get_public_ip_port_resp_st 
 * Description :  External Binding response message 
 *
 *----------------------------------------------------------------------------*/
typedef struct
{
    nat_uint8_t					context_id;
    nat_result_t				result;
    nat_error_t					error;
    nat_ip_port_address_st		local_addr;
    nat_ip_port_address_st		mapped_addr;
    nat_ip_port_address_st		changed_addr;
} stun_get_public_ip_port_resp_st;

/*------------------------------------------------------------------------------
 *
 * Type Name   :  stun_rel_public_ip_port_req_st 
 * Description :  Release request. 
 *
 *----------------------------------------------------------------------------*/
typedef nat_uint32_t  stun_rel_public_ip_port_req_st;

/*------------------------------------------------------------------------------
 *
 * Type Name   :  stun_binding_resp_st 
 * Description :  Server Binding response message 
 *
 *----------------------------------------------------------------------------*/
typedef struct
{
    nat_uint8_t        context_id;
	nat_result_t		result;
	nat_error_t			error;
	nat_ip_port_address_st    mapped_addr;
	nat_ip_port_address_st    changed_addr;
} stun_binding_resp_st;

/*------------------------------------------------------------------------------
 *
 * Type Name   :  stun_shsec_req_st 
 * Description :  SHSEC request message
 *
 *----------------------------------------------------------------------------*/
typedef struct
{
    nat_uint8_t        context_id;
} stun_shsec_req_st;

/*------------------------------------------------------------------------------
 *
 * Type Name   :  stun_shsec_resp_st 
 * Description :  SHSEC response message
 *
 *----------------------------------------------------------------------------*/
typedef struct
{
    nat_uint8_t         context_id;
    nat_result_t		result;
    nat_error_t			error;
    nat_string_st		shared_secret_username;
    nat_string_st		shared_secret_password;

} stun_shsec_resp_st;


/*------------------------------------------------------------------------------
 *
 * Type Name   :  stun_shsec_req_st 
 * Description :  SHSEC TLS server response message
 *
 *----------------------------------------------------------------------------*/
typedef stun_shsec_resp_st stun_shsec_tls_resp_st;

/*------------------------------------------------------------------------------
 *
 * Type Name   :  stun_type_discovery_resp_st 
 * Description :  NAT Type discovery response
 *
 *----------------------------------------------------------------------------*/
typedef struct
{
    nat_result_t		result;
    nat_error_t			error;
    nat_uint8_t         nat_type;
/* 
	#define NAT_PRIVATE_NETWORK_ONLY        0x01
	#define NAT_OPEN_INTERNET_INTERFACE     0x02
	#define NAT_FULL_CONE                   0x03
	#define NAT_RESTRICTED                  0x04
	#define NAT_PORT_RESTRICTED             0x05
	#define NAT_SYMMETRIC                   0x06
*/
} stun_type_discovery_resp_st;

/*------------------------------------------------------------------------------
 *
 * Type Name   :  stun_lifetime_discovery_resp_st 
 * Description :  Binding lifetime discovery response
 *
 *----------------------------------------------------------------------------*/
typedef struct
{
    nat_result_t			result;
    nat_error_t				error;
    nat_duration_t  	lifetime_bind_dur;
} stun_lifetime_discovery_resp_st;

/*------------------------------------------------------------------------------
 *
 * Type Name   :  stun_timer_start_resp_st 
 * Description :  Start timer response
 *
 *----------------------------------------------------------------------------*/
typedef struct
{
    nat_uint8_t        context_id;
    nat_timer_id_t      timer_id;
    nat_timer_type_t    timer_type;
} stun_timer_start_resp_st;

/*------------------------------------------------------------------------------
 *
 * Type Name   :  stun_timer_expire_ind_st 
 * Description :  Timer expiry indication
 *
 *----------------------------------------------------------------------------*/
typedef stun_timer_start_resp_st stun_timer_expire_ind_st;

/*------------------------------------------------------------------------------
 *
 * Type Name   :  stun_error_ind_st 
 * Description :  to pass on any error to application
 *
 *----------------------------------------------------------------------------*/
typedef struct
{
    nat_uint8_t        context_id;
    nat_error_t		error_cause;
} stun_error_ind_st;


#ifdef __cplusplus
}
#endif

#endif /* __NAT_COMMON_INTERNAL_API_H__ */

