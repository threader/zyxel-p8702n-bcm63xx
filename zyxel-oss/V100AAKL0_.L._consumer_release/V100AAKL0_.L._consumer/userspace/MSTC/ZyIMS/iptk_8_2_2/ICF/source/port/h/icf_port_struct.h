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
 *         11/03/08    Divij Agarwal       SPR 18122      Support for async AKA
 *                                                        negotiation
 *         02/03/09    Kamal Ashraf      IPTK REL 8.1    structure icf_dns_client_fds_st
 *                                                       added, two structure variables
 *                                                       added in icf_global_port_info_st,
 *                                                       icf_ares_fds_st has been removed,
 *                                                       some of the fields in icf_dns_server_data_st
 *                                                       has been removed for open source replacement.
 *
 *         27/03/09    Alok Tiwari      IPTK REL 8.2     New members added in structure
 *                                                       icf_aka_auth_input_st for
 *                                                       security association.
 *         02-May-2009 Rajiv Kumar      SPR 19531        Klocwork Fix
 *         13-Jul-2009 Rajiv Kumar      IPTK Rel 8.2     Openssl Removal
 * Copyright 2009, Aricent.
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
    /*Fix For SPR: 19531 (Klocwork Warning Removal)*/
    icf_uint32_t             fd_id;
    icf_boolean_t            is_secure_conn;

}icf_tcp_data_st;

typedef struct
{
    icf_uint32_t             fd_id;
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
    /*Fix For SPR: 19531 (Klocwork Warning Removal)*/
    icf_uint32_t  pending_connect_fd[ICF_MAX_NO_OF_PERSISTENT_CONNECTIONS];/*for reading on TCP conn
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

    /*Fix For SPR: 19531 (Klocwork Warning Removal)*/
    icf_uint32_t			self_fd_ipc;
    icf_uint8_t			    mm_addr[32];

    /*In case of TLS call IPTK open three ports for receiving request from
     * peer one each for UDP,TCP and TLS.*/
    icf_server_type_st		self_fd_nw[ICF_WIN_TLS_SIP_SERVER + 1];
    icf_uint16_t			write_fd_nw[ICF_WIN_TLS_SIP_SERVER + 1];
	
    icf_fd_set			win_read_fd;
    icf_fd_set			win_write_fd;
    icf_uint32_t			act_fd_count ;
    /*Fix For SPR: 19531 (Klocwork Warning Removal)*/
    icf_uint32_t		act_read_fd[FD_SETSIZE];

/* START: For MPH on Windows */
    icf_uint32_t			mph_tcp_conn_fd[ICF_MAX_NO_OF_PERSISTENT_CONNECTIONS];
/* END: For MPH on Windows */


    icf_boolean_t		ipsec_enabled;
#define ICF_WIN_UDP_SA_SERVER	0
#define ICF_WIN_TCP_SA_SERVER	1
    /*Fix For SPR: 19531 (Klocwork Warning Removal)*/
    icf_uint32_t			curr_act_serv_fd[ICF_WIN_TCP_SA_SERVER + 1];
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
 * typedef Name :  icf_dns_client_fds_st
 * 
 *  Description  : This data type is used to store the fd's return by arDnsClient
 *                 while initiating the arDnsClient.
 *   
 -----------------------------------------------------------------------------*/
typedef struct
{
    icf_fd_set      read_fd;
    icf_fd_set      write_fd;
    /* This fd is returned by arDnsClient after opening the socket at 
     * dns client side. Dns Client will listen the requests from
     * dns_clib on this fd.
     */ 
    icf_uint32_t    dns_client_fd;
    /* This fd is returned by arDnsClient after opening the socket at
     * dns clib side. Dns Clib will listen the mesasges from dns client
     * on this fd.
     */
    icf_uint32_t    dns_clib_fd;
}icf_dns_client_fds_st;

/* --------------------------------------------------------------------
 * Type Name   :  icf_dns_server_data_st
 * Description :  Structure to hold the info/data required for a connection
 *                with a DNS server. It will be a list with all DNS servers
 *                IPTK is currently communicating
 * ------------------------------------------------------------------------*/
typedef struct icf_dns_server_data_st
{
    /*DNS server IP address*/
    icf_uint8_t           	dns_server_ip[16];

    /*DNS server Port*/
    icf_uint16_t         	dns_server_port;

    /*next DNS server Data pointer*/
    struct  icf_dns_server_data_st*   	p_next;
} icf_dns_server_data_st;

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


/*  Following will be a linked list for each DNS server address and will be 
    removed when there is no query response is pending for a DNS server address 
    and DNS query time out is received
*/
    icf_dns_server_data_st* p_dns_server_data;
#endif

    icf_msg_list_st          internal_msg_list;

#ifdef ICF_TRACE_ENABLE
    icf_trace_ipc_info_st    icf_trace_ipc_info;
#endif

#ifdef ICF_DNS_LOOKUP_ENABLED
    /*variable added for open source replacement(IPTK REL 8.1)------START---*/
    icf_void_t               *pDnsclient_glb_data;
    icf_dns_client_fds_st     *pDnsclient_fds;
    /*variable added for open source replacement(IPTK REL 8.1)------END----*/
#endif

}icf_global_port_info_st;
 
/* ----------------------------------------------------------------------------
 * Type Name   : icf_aka_auth_input_st
 * Decription  : This structure is used for providing input parameters required
 *               for AKA challenge processing and response generation.
 *
 * --------------------------------------------------------------------------*/
typedef struct
{
    /* call ID. This field has been put for future use.As of now it will
     * be initialized to INVALID CALL ID*/
    icf_call_id_t                 call_id;

    /* application ID */
    icf_app_id_t                  app_id;

    /* line id */
    icf_line_id_t                 line_id;

    /* username as received from set_self_id(auth_key) */
    icf_string_st                 username;

    /* password as received from set_self_id.This is used in AKA as a shared
     * key to calculate the response parameter.*/
    icf_string_st                 password;

    /* Contains the URI parameter */
    icf_string_st                 uri;
    
    /* contains the nonce received */
    icf_short_string_st           nonce;         

    /* contains the realm */
    icf_string_st                 realm;

    /* contains the algo */
    icf_short_string_st           algo;

    /* contains Ik and Ck as received in the challenge.Curently we 
     * are just filling these as NULL and passing to the porting function.
     */
    icf_short_string_st           Ik;
    icf_short_string_st           Ck;

    /* last sequence number stored.This is used by the porting procedure
     * to determine synchronization failure*/
    icf_uint32_t                  sequence_num;

    /* This field denotes the user address for which AKA challenge is received. */
    icf_address_st                user_address;

    /* This field denotes the SIP method name for which AKA challenge is received. */
    icf_uint8_t                   sip_method_name;

    /* This field contains the Security-client parameters provided by
     * application in API ICF_REGISTER_REQ.
     */
    icf_large_string_st           security_client;

    /* This field contains the security-server header value received in 401 
     * of REGISTER request.
     */
    icf_large_string_st          security_server;

}icf_aka_auth_input_st;

/* ----------------------------------------------------------------------------
 * Type Name   : icf_aka_auth_output_st
 * Decription  : This structure is used for storing AKA output parameters
 *               generated by the porting procedure.
 *----------------------------------------------------------------------------*/
typedef struct
{
    /* This is the response sent in the subsequent request
     * with credentials. */
    icf_short_string_st		      res;

    /* This is sent in case of Syncronization failure at the servers's
     * end. */
    icf_short_string_st		      auts;

    /* Cipher key generated by running AKA procedures */
    icf_short_string_st		      Ck;

    /* Integrity key generated by running AKA procedures. */
    icf_short_string_st		      Ik;

    /* Sequence number as derived from nonce.This is stored and 
     * subsequently passed to the porting procedure as an input to enable 
     * the procedure to determine synchronization failure */
    icf_uint32_t                  sequence_num;

    /* This ecode is set by the function icf_port_run_aka_procedure and
     * is used by the calling function.
     */
    icf_error_t			          ecode;
}icf_aka_auth_output_st;


#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* __ICF_PORT_STRUCT_H__ */
