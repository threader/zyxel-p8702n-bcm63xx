/******************************************************************************
*                                    
* FILE NAME    : nat_types.h                 
*                                    
* DESCRIPTION  : Contains the definitions for the data types used in NAT CLIENT
*                APIs.    
*                                    
* Revision History :                            
*
*  DATE            NAME                  REFERENCE       REASON        
* ------         ------------            ---------     ----------
* 30-Nov-2006   Anuradha Gupta              HLD       Initial Version
* 14-May-2010	Preksha Gupta		SPR 20375	    Merged CSR 1-7845448
*
* Copyright 2010, Aricent                      
*                                    
*******************************************************************************/


#ifndef __NAT_TYPES_H__
#define __NAT_TYPES_H__

/* Prevent Name Mangling By C++ Compilers */
#include "nat_defs.h"

#ifdef __cplusplus

extern "C"
{
#endif /* __cplusplus */

/***********************************************************************
 * Description  : Basic data types definitions
 *                
 ************************************************************************/

typedef void            nat_void_t;

typedef unsigned char   nat_uint8_t;

typedef unsigned short  nat_uint16_t;

typedef unsigned int    nat_uint32_t;

typedef char            nat_int8_t;

typedef signed short    nat_int16_t; 

typedef signed int      nat_int32_t;

typedef nat_uint8_t     nat_return_t;

typedef nat_uint8_t     nat_boolean_t;

typedef nat_uint16_t    nat_error_t;

typedef nat_uint16_t    nat_duration_t;

typedef nat_boolean_t   nat_result_t;

typedef nat_uint16_t	nat_ecode_t;

typedef nat_uint8_t     nat_api_id_t;

typedef nat_uint8_t     nat_pool_id_t;


/* -----------------------------------------------------------------------
 * Type Name    : nat_uint128_t
 * Description  : Identifier for the binding transaction
 *                
 * ----------------------------------------------------------------------*/
typedef struct { unsigned char octet[16]; }  nat_uint128_t;

/* -----------------------------------------------------------------------
 * Type Name    : nat_timer_id_t
 * Description  : To define the identifier of the timers
 *                
 * ----------------------------------------------------------------------*/
typedef nat_uint32_t    nat_timer_id_t;

/* -----------------------------------------------------------------------
 * Type Name    : nat_timer_status_t
 * Description  : To define the status for timers. The valid values are :
 *			
 *          * #define NAT_TIMER_STATUS_INVALID        	0
 *          * #define NAT_TIMER_STATUS_VALID  		1
 *          * #define NAT_TIMER_STATUS_PENDING        	2
 *       
 *                
 * ----------------------------------------------------------------------*/
typedef nat_uint8_t     nat_timer_status_t;

/* -----------------------------------------------------------------------
 * Type Name    : nat_int_module_id_t
 * Description  : To define the type for internal module ids that form
 *                
 * ----------------------------------------------------------------------*/
typedef nat_uint8_t   nat_int_module_id_t;

/*-----------------------------------------------------------------------------
 *
 *  Structure Name : nat_timer_type_t
 *
 *  Description :    Would contain different timer types e.g. call hold timer
 *
 *---------------------------------------------------------------------------*/
typedef nat_uint8_t                     nat_timer_type_t;

/*-----------------------------------------------------------------------------
 *
 *  Structure Name : nat_string_st
 *
 *  Description :    Basic string type stucture definition
 *
 *---------------------------------------------------------------------------*/
typedef struct        
{
   /* specifies the length of the string including '\0' character */ 
    nat_uint16_t str_len;
    
    /* array of characters */ 
    nat_uint8_t str[NAT_MAX_STR_LEN];
} nat_string_st;

/*-----------------------------------------------------------------------------
 *
 *  Structure Name : nat_octet_string_st
 *
 *  Description :    Octet string type stucture definition to be used for 
 *					creating UDP Packet.
 *
 *---------------------------------------------------------------------------*/
typedef struct
{
	 /* specifies the length of the string including '\0' character */ 
    nat_uint16_t octet_len;
    
    /* array of characters */ 
    nat_uint8_t str[NAT_MAX_OCTET_LEN];
}nat_octet_string_st;

/*-----------------------------------------------------------------------------
 *
 *  Structure Name : nat_ipv4_address_st
 *
 *  Description :    Octet wise representation of IPV4 
 *
 *---------------------------------------------------------------------------*/
typedef struct
{
	nat_uint8_t     octet_1;
    nat_uint8_t     octet_2;
    nat_uint8_t     octet_3;
    nat_uint8_t     octet_4;
} nat_ipv4_address_st;

/*-----------------------------------------------------------------------------
 *
 *  Structure Name : nat_ip_port_address_st
 *
 *  Description :    IPV4 address structure 
 *
 *---------------------------------------------------------------------------*/
typedef struct
{
    nat_ipv4_address_st	ipv4_addr;
    nat_uint16_t		port;
    nat_uint16_t		padding;
}nat_ip_port_address_st;


/*-----------------------------------------------------------------------------
 *
 *  Structure Name : nat_timer_data_st
 *
 *  Description :    Would contain the information that would be passed to other
 *                   module in case of timer expiry
 *
 *---------------------------------------------------------------------------*/
typedef struct
{
    nat_uint8_t context_id;
    nat_uint8_t	 transaction_id[16];
    
    /* This would contain the information regarding the type of timer */
    nat_timer_type_t   timer_type;
    
}nat_timer_data_st;

/*------------------------------------------------------------------------------
 *
 * Type Name   :  nat_msg_hdr_st
 * Description :  Message header structure 
 *
 *----------------------------------------------------------------------------*/
typedef struct
{
	/* The API identifier */
	nat_uint8_t		api_id;

	/* The unique context identifier for the request */
	nat_uint32_t	app_context_id;

	/* The source ID from where the request is generated */
	nat_uint8_t		source_id;

	/* The destination ID for which the request is destined */
	nat_uint8_t		destination_id;

	/* The length of the API inclusive of Message header */
	nat_uint32_t    api_length;
}nat_msg_hdr_st;

/*------------------------------------------------------------------------------
 *
 * Type Name   :  nat_msg_st
 * Description :  Msg 
 *
 *----------------------------------------------------------------------------*/

typedef struct
{

    /* header of nat message */
    nat_msg_hdr_st    		msg_hdr;

    /* message data which will be cast as per app context id */
    nat_uint8_t             p_msg_data[1];
} nat_msg_st;

/*------------------------------------------------------------------------------
 *
 * Type Name   :  func_open_udp_socket 
 * Description :  function pointer to open a UDP socket
 *
 *----------------------------------------------------------------------------*/
typedef nat_return_t (*func_open_udp_socket)(
                                        nat_ip_port_address_st *p_local_addr);

/*------------------------------------------------------------------------------
 *
 * Type Name   :  func_close_udp_socket 
 * Description :  function pointer to close a UDP socket
 *
 *----------------------------------------------------------------------------*/
typedef nat_return_t (*func_close_udp_socket)(
                                         nat_ip_port_address_st *p_local_addr);

/*------------------------------------------------------------------------------
 *
 * Type Name   :  func_send_udp_data 
 * Description :  function pointer to send data on UDP socket
 *
 *----------------------------------------------------------------------------*/
typedef nat_return_t (*func_send_udp_data)( 
                                      nat_ip_port_address_st *p_local_addr,
                                      nat_ip_port_address_st *p_dest_addr,
                                      nat_void_t *p_msg_data);

/*------------------------------------------------------------------------------
 *
 * Type Name   :  func_send_app_data 
 * Description :  function pointer to send data to APP
 *
 *----------------------------------------------------------------------------*/
typedef nat_return_t (*func_send_app_data)( 
                                      nat_void_t *p_msg_data);

/*------------------------------------------------------------------------------
 *
 * Type Name   :  func_send_mm_data 
 * Description :  function pointer to send data through MM
 *
 *----------------------------------------------------------------------------*/
typedef nat_return_t (*func_send_mm_data)(
                                      nat_ip_port_address_st *p_local_addr,
									  nat_ip_port_address_st *p_dest_addr,
                                      nat_void_t *p_msg_data);


/*------------------------------------------------------------------------------
 *
 * Type Name   :  func_start_timer 
 * Description :  function pointer to start a timer
 *
 *----------------------------------------------------------------------------*/
typedef nat_return_t (*func_start_timer)(nat_void_t *p_timer_data, 
                                         nat_duration_t  timer_dur,
                                         nat_timer_id_t *p_timer_id);

/*------------------------------------------------------------------------------
 *
 * Type Name   :  func_stop_timer 
 * Description :  function pointer to stop a timer
 *
 *----------------------------------------------------------------------------*/
typedef nat_return_t (*func_stop_timer)(nat_timer_id_t timer_id);

/*------------------------------------------------------------------------------
 *
 * Type Name   :  func_rel_port_data 
 * Description :  function pointer to release port data
 *
 *----------------------------------------------------------------------------*/
typedef nat_void_t (*func_rel_port_data)(nat_void_t);


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __NAT_TYPES_H__ */

