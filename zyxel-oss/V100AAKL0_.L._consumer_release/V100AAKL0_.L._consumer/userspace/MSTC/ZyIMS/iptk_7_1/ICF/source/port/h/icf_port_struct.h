/*************************************************************************
 *
 *     FILENAME           :  icf_port_struct.h
 *
 *     DESCRIPTION        : 
 *
 *     Revision History   :
 *         DATE        NAME                REFERENCE            REASON
 *         ----------------------------------------------------------
 *         26/12/04    Aashish Suchdev     ICF LLD        Initial Version.
 *         17/01/05    Aashish Suchdev                  Added struct headers.
 *		   06/05/05		Vidyut Gupta					add the include files to 
 *														port on WINDOWS
 *         10/01/06    Aman Arora           IPSEC       updated structure for 
 *                                                      IPSEc
 *         27/11/06    Deepti Goyal         ICF Rel6.2  NAT Feature Support
 *
 * Copyright 2006, Aricent.
 *
 *************************************************************************/

#ifndef __ICF_PORT_STRUCT_H__
#define __ICF_PORT_STRUCT_H__

#include "icf_common_types.h"
#include "icf_port_defs.h"

#if defined(ICF_PORT_SOLARIS) || defined(ICF_PORT_LINUX)
#include <sys/time.h>
#endif


#if defined( ICF_PORT_WINDOWS ) || defined( ICF_PORT_WINCE ) 

#include <time.h>
#include <winsock2.h>

#endif


#ifdef __cplusplus

/* Yes, C++ compiler is present.
 * Use standard C.
 */

extern "C"
{
#endif /* __cplusplus */


typedef fd_set icf_fd_set ;

#ifdef ICF_TRACE_ENABLE
typedef struct{
    icf_address_st              calling_party_address[ICF_TRACE_MAX_DB_VAL];
    icf_address_st              called_party_address[ICF_TRACE_MAX_DB_VAL];

    icf_line_id_t               line_id[ICF_TRACE_MAX_DB_VAL];

    icf_codec_attrib_st        offered_codec[ICF_TRACE_MAX_DB_VAL];
    icf_codec_attrib_st        negotiated_codec[ICF_TRACE_MAX_DB_VAL];

    icf_transport_addr_st    local_rtp_ip_address[ICF_TRACE_MAX_DB_VAL];
    icf_transport_addr_st    remote_rtp_ip_address[ICF_TRACE_MAX_DB_VAL];
    icf_transport_addr_st    local_sip_ip_address[ICF_TRACE_MAX_DB_VAL];

    icf_uint16_t              local_rtp_port[ICF_TRACE_MAX_DB_VAL];
    icf_uint16_t              remote_rtp_port[ICF_TRACE_MAX_DB_VAL];
    icf_uint16_t              local_sip_port[ICF_TRACE_MAX_DB_VAL];

}icf_trace_database_st;

#endif


/* START: For MPH on Windows */
#define ICF_INV_FD 65535
/* END: For MPH on Windows */



/* --------------------------------------------------------------------
 * Type Name   :  icf_port_timer_data_st
 * Description :  Structure to hold the info/data required for timer porting
 *                Right now we are using Genlib's timer management for the 
 *                same.  The structure is entirely port specific and ICF
 *                core is not aware of it's definition
 * ------------------------------------------------------------------------*/
typedef struct
{
    icf_timer_cb_fn_t        icf_genlib_timer_cb_fn;
    icf_uint16_t             current_list_index;
    icf_uint16_t             timer_id_list[ICF_MAX_TIMER_LIST_COUNT];
}icf_port_timer_data_st;


typedef struct
{

    icf_uint16_t             fd_id;

    icf_boolean_t            is_secure_conn;

}icf_tcp_data_st;

typedef struct
{
    icf_uint16_t             fd_id;
    icf_uint8_t              server_type;
}icf_server_type_st;
typedef struct
{
    icf_uint16_t	     fd_id;
    icf_void_t              *p_secure_data;    
}icf_secure_map_st;

typedef struct
{
    /*Following is allocated at the time of opening of TCP server.
     * It is de-allocated at the time of TCP server closure. Also, take care
     * that the pointer is assigned NULL value after it has been freed.
     * In normal functioning, the buffer will never be freed for the life
     * of IPPacketpone supporting TCP*/
    icf_uint8_t  *p_tcp_recv_buffer;

#define ICF_UDP_SIP_SERVER   ICF_MAX_MODULE + 1
#define ICF_TCP_SIP_SERVER   ICF_MAX_MODULE + 2
#define ICF_MAX_TLS_CONN     (ICF_MAX_NUM_OF_LINES * 2)
    /*FIFOs for each module + UDP+ TCP server*/
    icf_uint16_t  read_module_fd[ICF_MAX_MODULE +3 ];

    icf_uint16_t  write_module_fd[ICF_MAX_MODULE + 3];/*FIFO + UDP*/
    icf_uint32_t  msg_read_size[ICF_MAX_MODULE + 3];
    icf_uint32_t  msg_write_size[ICF_MAX_MODULE + 3];
    icf_fd_set  read_fdset;/*User in copying fds to the actual select call*/
    icf_fd_set  write_fdset;/*for implementing asynchronous connect*/

    /*for reading on TCP conn*/
    icf_uint32_t  num_of_conn;
    icf_tcp_data_st  read_tcp_conn[ICF_MAX_NO_OF_PERSISTENT_CONNECTIONS];

    icf_secure_map_st      secure_map[ICF_MAX_TLS_CONN];

    /* This list has fields of pending connect. Once connect is received on this FD
     * SSA will check if this fd is still waited on and issue API to eshell to move
     * it to read fd set or delete connection*/
    icf_uint32_t  num_of_pending_conn;
    icf_uint16_t  pending_connect_fd[ICF_MAX_NO_OF_PERSISTENT_CONNECTIONS];/*for reading on TCP conn
                                                               ections*/
	icf_int32_t msgqid;

    icf_uint8_t  dscp_bits;
}icf_ext_info_st;



#if defined(ICF_PORT_WINDOWS) || defined(ICF_PORT_LINUX) || defined(ICF_PORT_VXWORKS)

#ifdef ICF_NAT_MEDIA_TRAVERSAL
/* ---------------------------------------------------------------------------
 * Type Name   :  icf_nat_ipc_info_st
 * Description :  Structure to hold the info/data required for IPC
 *
 * ------------------------------------------------------------------------*/
typedef struct
{
    icf_transport_address_st    local_addr;
    icf_uint16_t                binding_fd;
    icf_boolean_t               is_free;
}icf_nat_ipc_info_st;

#endif

/* --------------------------------------------------------------------
 * Type Name   :  icf_win_ipc_info
 * Description :  Structure to hold the info/data required for send the 
 *				  to PA, MM and CFG. it also holds it's own fd
 * ------------------------------------------------------------------------*/
typedef struct
{
	icf_uint16_t			port_pa;
	icf_uint16_t			port_cfg;
	icf_uint16_t			port_mm;

#ifdef ICF_NAT_MEDIA_TRAVERSAL
#define ICF_MAX_NAT_CONTEXT     250
    icf_uint16_t			port_nat;
    icf_uint8_t			    nat_addr[32];
    icf_uint8_t             nat_ipc_count;
    icf_nat_ipc_info_st     nat_ipc_info[ICF_MAX_NAT_CONTEXT];
    icf_uint16_t			nat_fd_ipc;
#endif

    icf_uint16_t			self_fd_ipc;
    icf_uint8_t			    mm_addr[32];

    icf_server_type_st		self_fd_nw[ICF_WIN_TCP_SIP_SERVER + 1];
    icf_uint16_t			write_fd_nw[ICF_WIN_TCP_SIP_SERVER + 1];
	
    icf_fd_set			win_read_fd;
    icf_fd_set			win_write_fd;
    icf_uint32_t			act_fd_count ;
    icf_uint16_t		act_read_fd[FD_SETSIZE];

/* START: For MPH on Windows */
    icf_uint32_t			mph_tcp_conn_fd[ICF_MAX_NO_OF_PERSISTENT_CONNECTIONS];
/* END: For MPH on Windows */


    icf_boolean_t		ipsec_enabled;
#define ICF_WIN_UDP_SA_SERVER	0
#define ICF_WIN_TCP_SA_SERVER	1
	icf_uint16_t			curr_act_serv_fd[ICF_WIN_TCP_SA_SERVER + 1];
	icf_uint16_t                    num_of_open_servers;
}icf_win_ipc_info;

/* Structure used to get the information of ports
from the file */

typedef struct 
{
icf_uint16_t			port_pa;
icf_uint16_t			port_cfg;
icf_uint16_t			port_mm;
#ifdef ICF_NAT_MEDIA_TRAVERSAL
icf_uint16_t            port_nat;
#endif
icf_uint16_t			self_port_ipc;
icf_uint8_t			    mm_addr[32];
#ifdef ICF_NAT_MEDIA_TRAVERSAL
icf_uint8_t			    nat_addr[32];
#endif

#ifdef ICF_TRACE_ENABLE
        icf_trace_database_st *p_trace_db_val;
#endif

}icf_port_info_st;

#endif

#ifdef ICF_DNS_LOOKUP_ENABLED

/*-----------------------------------------------------------------------------
 *
 * typedef Name :  icf_ares_fds_set
 * 
 *  Description  : This data type is used to store the fd's return by ARES while 
 *                 making DNS query.
 *   
 -----------------------------------------------------------------------------*/
typedef struct
{
    icf_fd_set      read_fd;
    icf_fd_set      write_fd;
    icf_uint16_t    timer_duration;

	/*This field keeps track of that fd for which DNS resp is received*/
		icf_uint32_t	curr_fd;

#define ICF_ARES_FD_COUNT	2

	/*First coloumn keeps track of the fd returned by the ARES
	 *Second coloumn tells the number of time that fd has to be read 
	 */
	icf_uint16_t    ares_fds[ICF_MAX_DNS_SERVER_FDS][ICF_ARES_FD_COUNT];
    /*This variable keeps track of the number of the DNS responses to be awaited*/

    icf_uint8_t     count;

	icf_void_t*     p_ares_channel;

}icf_ares_fds_st;

#endif

typedef struct
{

    icf_uint8_t         local_ip[ICF_MAX_IPV6_ADDRESS_LENGTH];
    icf_uint16_t        local_port;
    icf_uint8_t         remote_ip[ICF_MAX_IPV6_ADDRESS_LENGTH];
    icf_uint16_t        remote_port;
    icf_transport_mode_t        self_mode;
}icf_trace_ipc_info_st;

/* --------------------------------------------------------------------
 * Type Name   :  icf_global_port_info_st
 * Description :  Structure to hold the info/data required for porting
 *                The structure is entirely port specific and ICF
 *                core is not aware of it's definition
 * ------------------------------------------------------------------------*/
typedef struct
{
    /*
     * following is the array used for mapping the pool identifiers
     * defined by tk to those returned by GL at the time of return
     * of pool creation interfaces
     * This will not have the common pool ids, only task specific 
     * pool ids from GL
     */
    icf_uint16_t             gl_pool_id_map[ICF_MAX_NUM_POOL][2];
    icf_port_timer_data_st   glb_timer_data;
    icf_ext_info_st          ext_info;
#if defined(ICF_PORT_WINDOWS) || defined(ICF_PORT_LINUX) || defined(ICF_PORT_VXWORKS)
	icf_win_ipc_info			win_ipc_info;
#endif

#ifdef ICF_DNS_LOOKUP_ENABLED
	/*The following st stores the fds return by ARES while making DNS query*/

	icf_ares_fds_st		*p_ares_port_info;
#endif

    icf_msg_list_st          internal_msg_list;

#ifdef ICF_TRACE_ENABLE
    icf_trace_ipc_info_st    icf_trace_ipc_info;
#endif

}icf_global_port_info_st;
 

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* __ICF_PORT_STRUCT_H__ */
