/*------------------------------------------------------------------------------
 *
 * File name        : nat_common_port_types.h
 *
 * Purpose          : This file contains all the common port structures
 *                    or typedefs that are being used across the 
 *                    whole NAT Client Library.
 *
 * Revision History :
 *
 *   Date        Name           Ref#      Comments
 * --------      ------------   ----      ---------------
 * 14-Nov-2006	 Anuradha Gupta		        Initial Version
 *                                        
 * Copyright (c) 2006, Aricent 
 *----------------------------------------------------------------------------*/

#ifndef __NAT_COMMON_PORT_TYPES_H__
#define __NAT_COMMON_PORT_TYPES_H__

#include "nat_types.h"

#include "nat_common_defs.h"
/* #include "nat_port_defs.h" */

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

#define NAT_BLK_SIZE_8				8
#define NAT_NUM_BLK_SIZE_8			2400
#define NAT_INDX_BLK_SIZE_8			0

#define NAT_BLK_SIZE_16				16
#define NAT_NUM_BLK_SIZE_16			1500
#define NAT_INDX_BLK_SIZE_16		1

#define NAT_BLK_SIZE_32				32
#define NAT_NUM_BLK_SIZE_32			1500
#define NAT_INDX_BLK_SIZE_32		2

#define NAT_BLK_SIZE_64				64
#define NAT_NUM_BLK_SIZE_64			1300
#define NAT_INDX_BLK_SIZE_64		3

#define NAT_BLK_SIZE_128			128
#define NAT_NUM_BLK_SIZE_128		600
#define NAT_INDX_BLK_SIZE_128		4

#define NAT_BLK_SIZE_256			256
#define NAT_NUM_BLK_SIZE_256		400
#define NAT_INDX_BLK_SIZE_256		5

#define NAT_BLK_SIZE_512			512
#define NAT_NUM_BLK_SIZE_512		350
#define NAT_INDX_BLK_SIZE_512		6

#define NAT_BLK_SIZE_1024			1024
#define NAT_NUM_BLK_SIZE_1024		150
#define NAT_INDX_BLK_SIZE_1024		7

#define NAT_BLK_SIZE_2048			2048
#define NAT_NUM_BLK_SIZE_2048		20
#define NAT_INDX_BLK_SIZE_2048		8

#define NAT_BLK_SIZE_4096			4096
#define NAT_NUM_BLK_SIZE_4096		15
#define NAT_INDX_BLK_SIZE_4096		9

#define NAT_BLK_SIZE_8192			8192
#define NAT_NUM_BLK_SIZE_8192		9
#define NAT_INDX_BLK_SIZE_8192		10

#define NAT_BLK_SIZE_10240			10240              
#define NAT_NUM_BLK_SIZE_10240		8
#define NAT_INDX_BLK_SIZE_10240		11

#define NAT_BLK_SIZE_20480			20480
#define NAT_NUM_BLK_SIZE_20480		3
#define NAT_INDX_BLK_SIZE_20480		12

/* Temporary fix: Defining NAT_BLK_SIZE_MAX */
#define NAT_BLK_SIZE_MAX          NAT_BLK_SIZE_4096

#define NAT_MAX_NUM_COMMON_POOL     14

    
/*Following is used in port for reading UDP messages*/
#define NAT_MAX_UDP_MSG_LNGTH       NAT_BLK_SIZE_MAX

/*Following is used in port for reading TCP connections*/
#define NAT_MAX_TCP_MSG_LNGTH       NAT_BLK_SIZE_MAX

/*Following is used in the pdu pointer array for tcpm connblock*/
#define NAT_MAX_PENDING_PDU_PER_CONN     2

/*Following is used for the size of tcpm conn database.
 * This value is equal to the maximum number of call
 * objects allowed in SSA(NAT_SSA_MAX_CALL_OBJ)*/
#define NAT_MAX_TCP_CONN             10

#define NAT_MAX_DNS_SERVER_FDS       10
	
/*Following is used to mark the conn_fd in tcpm conn
 * db node invalid. Mapped to UATK hash define
 * Sdf_co_InvalidSocketFd*/
#define NAT_INVALID_SOCKFD       -1

/* Values for icf_interface_type_t */
#define NAT_INTF_TYPE_FIFO  0
#define NAT_INTF_TYPE_UDP   1
#define NAT_INTF_TYPE_TCP   2
#define NAT_INTF_TYPE_TLS   3

typedef nat_uint8_t   nat_interface_type_t;

/* --------------------------------------------------------------------
 * Type Name   :  nat_interface_info_st
 * Description :  This data is given by ES on receiving message from 
 *                IP Packetphone toolkit and AL pass this data to os_send. 
 *                OS_SEND use this data to write mesg on network.
 * 
 * ------------------------------------------------------------------------*/
/*  */
typedef struct
{
  nat_interface_type_t    interface_type;
  nat_uint32_t  msg_len;
  nat_uint32_t  fd_id;
  nat_uint16_t  dest_port;
  nat_uint8_t   dest_ip[NAT_MAX_IPV4_ADDRESS_LENGTH];
  nat_uint16_t  local_port;
  nat_uint8_t   local_ip[NAT_MAX_IPV4_ADDRESS_LENGTH];
  nat_void_t    *p_port_info;
}nat_interface_info_st;

/* --------------------------------------------------------------------
 * Type Name   :  nat_memory_pool_data_st
 * Description :  Structure to hold the info/data required for maintaining
 *                mapping between NAT Lib's pool IDs and the implementation
 *                in port for one pool. This structure will be used by the
 *                porting interface.
 * ------------------------------------------------------------------------*/
typedef struct
{
    /*
     * All pools will be created with a common task id.
     * The pool id will be used to differentiate between
     * various pools
     */

    /* size of buffer in the pool */
    nat_uint32_t           pool_buffer_size;

    /* number of buffers in the pool */
    nat_uint32_t           num_buffers;

    /* pool identifier */
    nat_uint16_t           pool_id;

} nat_memory_pool_data_st;


/* --------------------------------------------------------------------
 * Type Name   :  nat_buffer_pool_table_st
 * Description :  Structure to hold the instances of pool specific information
 *                chunks(nat_pool_data_t)
 * ------------------------------------------------------------------------*/
typedef struct 
{

    /* number of buffer pools */
    nat_uint16_t             pool_count;

    /* pool specific information */
    nat_memory_pool_data_st  pools[NAT_MAX_NUM_POOL];

}nat_memory_pool_table_st;


 /*------------------------------------------------------------------------------
 *
 * Type Name   :  nat_msg_list_st
 * Description :  List of messages that are received from ES recv
 *
 *----------------------------------------------------------------------------*/
typedef struct nat_msg_list_s
{
    nat_msg_st              *p_msg;

    struct nat_msg_list_s   *p_msg_next;
} nat_msg_list_st;

/*-----------------------------------------------------------------------------
 *
 *  Structure Name : nat_timer_cb_fn_t
 *
 *  Description :    Function ptr type for defining the call back function
 *                  to be invoked by the timer port in the event of a timer
 *                  expiry
 *---------------------------------------------------------------------------*/
typedef nat_void_t (* nat_timer_cb_fn_t)(nat_void_t*);



/*------------------------------------------------------------------------------
 *
 * Type Name   :  nat_nw_msg_data_st
 * Description :  Msg data for sending to n/w
 *
 *----------------------------------------------------------------------------*/

typedef struct
{
    /* internal message data which will be cast as per msg id */
    nat_octet_string_st     *p_buffer_data;

} nat_nw_msg_data_st;

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* __NAT_COMMON_PORT_TYPES_H__ */
