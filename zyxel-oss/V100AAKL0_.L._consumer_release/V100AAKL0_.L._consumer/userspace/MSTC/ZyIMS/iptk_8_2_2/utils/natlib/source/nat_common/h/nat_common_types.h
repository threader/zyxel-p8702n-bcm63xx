/*------------------------------------------------------------------------------
 *
 * File name        : nat_common_types.h
 *
 * Purpose          : This file contains all the common structures
 *                    or typedefs that are being used across the whole NAT Client Library.
 *
 * Revision History :
 *
 *   Date        Name           Ref#      Comments
 * --------      ------------   ----      ---------------
 * 22-Jun-2006   Abhishek Chhibber	Initial Version	
 * 14-Nov-2006	 Anuradha Gupta		Coding
 * 15-Jul-2009   Tarun Gupta            Modified nat_glb_pdb_st
 *                                        
 * Copyright (c) 2009, Aricent 
 *----------------------------------------------------------------------------*/

#ifndef __NAT_COMMON_TYPES_H__
#define __NAT_COMMON_TYPES_H__

#include "nat_types.h"

#include "nat_common_defs.h"
#include "nat_common_port_types.h"


/* Determine if a C++ compiler is being used.
 * If so, ensure that standard
 * C is used to process the API information.
 */

#ifdef __cplusplus

/* Yes, C++ compiler is present.
 * Use standard C.
 */

extern "C"
{
#endif /* __cplusplus */

typedef struct 
{
    nat_string_st       self_ip;
    nat_uint16_t        app_recv_port;

    nat_string_st       app_ip;
    nat_uint16_t        app_send_port;

	nat_duration_t		shsec_refresh_timer;
	nat_duration_t        	type_disc_timer;
	nat_duration_t		lifetime_disc_timer;
	nat_duration_t		lifetime_delta_timer;
    nat_duration_t      	binding_refresh_timer;

	nat_ip_port_address_st	stun_server_addr;

	nat_boolean_t		shsec_enabled;
	nat_boolean_t		lifetime_disc_enabled;

	nat_uint8_t		username[100];
	nat_uint8_t		password[100];

	nat_uint32_t		scale_timer_value;
	nat_uint16_t		type_disc_port;
}stun_config_data_st;




/*-----------------------------------------------------------------------------
 *
 * Name : stun_timer_info_st
 *
 * Description  : This structure defines the timer information of STUN.
 *
 *----------------------------------------------------------------------------*/

typedef struct
{
        nat_timer_id_t          timer_id;
        nat_timer_status_t      timer_status;
        /*
           * #define NAT_TIMER_STATUS_INVALID        	0
           * #define NAT_TIMER_STATUS_VALID  		1
           * #define NAT_TIMER_STATUS_PENDING        	2
        */
  	nat_void_t	*p_timer_data;        
}nat_timer_info_st;

/*-----------------------------------------------------------------------------
 *
 *  Structure Name : nat_context_data_map_st
 *
 *  Description :    Would contain context data map
 *
 *---------------------------------------------------------------------------*/
typedef struct 
{
	/* The requested context ID in the external message */
	nat_uint32_t				app_context_id;

	/* The local address if the request is for binding*/
	nat_ip_port_address_st  	local_address;

	/* The transaction ID generated for the binding request*/
	nat_uint128_t				transaction_id;

	/* The module ID from which the binding request is generated */
	nat_uint8_t 	  			module_id;

	/* The flag whether the context is free or not */
	nat_boolean_t	  			is_free;

	/* destination ID for which the request has come */
	nat_uint8_t					destination_id;

	/* source ID from which the request is received */
	nat_uint8_t					source_id;

	/* message integrity value */
	nat_uint8_t		message_integrity[NAT_STUN_HMAC_DIGEST_LENGTH + 1];
}nat_context_data_map_st;

/*------------------------------------------------------------------------------
 *
 * Type Name   :  nat_glb_data_st
 * Description :  Structure containing all global data which is of importance to *               
 *			      all modules.
 *
 *----------------------------------------------------------------------------*/

typedef struct
{
	

	nat_uint8_t         max_contexts;

	nat_boolean_t		message_integrity;

	nat_uint8_t        bind_lifetime_disc_status;

#define NAT_STUN_BINDING_LIFETIME_NOT_REQD	0x00
#define NAT_STUN_BINDING_LIFETIME_PENDING		0x01
#define NAT_STUN_BINDING_LIFETIME_ONGOING		0x02
#define NAT_STUN_BINDING_LIFETIME_COMPLETE	0x03



	nat_uint8_t        nat_type_disc_status;

#define NAT_STUN_NAT_TYPE_DISCOVERY_NOT_REQD    0x00
#define NAT_STUN_NAT_TYPE_DISCOVERY_PENDING	0x01
#define NAT_STUN_NAT_TYPE_DISCOVERY_ONGOING	0x02
#define NAT_STUN_NAT_TYPE_DISCOVERY_COMPLETE	0x03

	nat_uint8_t        shared_secret_proc_status;

#define NAT_STUN_SHARED_SECRET_NOT_REQD		0x00
#define NAT_STUN_SHARED_SECRET_ONGOING		0x01
#define NAT_STUN_SHARED_SECRET_COMPLETE		0x02


	nat_context_data_map_st		*p_stun_context_map;

	nat_void_t			*p_stun_data;
	nat_void_t			*p_stun_shsec_data;
	nat_void_t			*p_stun_discovery_data;
	nat_void_t			*p_stun_lifetime_data;

	stun_config_data_st		*p_stun_config_data;
	
	/*
     	* The memory for this would be allocated by the port_init in port
     	*/
    	nat_void_t  			*p_glb_port_info;

	/* return error code */
    	nat_error_t             ecode;
 
} nat_glb_data_st;


/*------------------------------------------------------------------------------
 *
 * Type Name   :  nat_glb_pdb_st
 * Description :  global process data block
 *
 *----------------------------------------------------------------------------*/

typedef struct
{
/* pointer to message received from external interface */
    nat_msg_list_st           *p_recv_msg;

	/* pointer to global data */
    nat_glb_data_st         *p_glb_data;

    func_open_udp_socket    es_nat_open_udp_socket;
    func_close_udp_socket   es_nat_close_udp_socket;
    func_send_udp_data      es_nat_send_udp_data;
    func_send_app_data      es_nat_send_app_data;
    func_send_mm_data      es_nat_send_mm_data;
    func_start_timer        es_nat_start_timer;
    func_stop_timer         es_nat_stop_timer;
    func_rel_port_data	    es_nat_rel_port_data;	

    nat_timer_info_st       timer_info[NAT_MAX_TIMER_INFO_BLOCKS];
}nat_glb_pdb_st;

/*------------------------------------------------------------------------------
 *
 * Type Name   :  nat_internal_msg_hdr_st
 * Description :  Header for internal message
 *
 *----------------------------------------------------------------------------*/

typedef struct
{

    /* internal message id */
    nat_uint16_t            msg_id;

    /* length of p_msg_data in internal message. This may be deleted if felt
       lateron that it is not being used */
    nat_uint16_t            payload_length;

    /* global pdb pointer */
    nat_glb_pdb_st          *p_glb_pdb;

    /* return error code */
    nat_error_t             ecode;

    /* return status */
    nat_uint32_t            status;

} nat_internal_msg_hdr_st;


/*------------------------------------------------------------------------------
 *
 * Type Name   :  nat_internal_msg_st
 * Description :  Msg from any internal module to another internal module
 *
 *----------------------------------------------------------------------------*/

typedef struct
{

    /* header of internal message */
    nat_internal_msg_hdr_st    msg_hdr;

    /* internal message data which will be cast as per msg id */
    nat_uint8_t               *p_msg_data;

} nat_internal_msg_st;

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* __NAT_COMMON_TYPES_H__ */
