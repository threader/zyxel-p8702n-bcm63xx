
/*****************************************************************************
 **
 ** FILENAME:		icf_port.h
 **
 ** DESCRIPTION:	This file containg the prototypes and the #defs required 
 **					for porting functionality
 **
 ** DATE			NAME		REFERENCE		REASON
 ** ----			----		---------		------
 ** 06 MAY 2005		Vidyut Gupta				Modified to port on WINDOWS
 ** 04 AUG 2005		Rohit Aggarwal				Changes for ICF compilation
 ** 15-Sept-2005	Ashutos Rath  	ICF	         Added Memory Pool for 
 **                                             SIGSA DICTIONARY
 ** 13 Jan 2006		Aman Arora		IPSEC		Added function prototype to
 **												close SA servers
 ** 12 Jan 2007     Abhishek Dhammawat ICF      Addition for SIP DSCP parameter
 **                                             per line feature.
 ** 28 Mar 2008     Shruti Thakral  SPR 18316   Klocwork warning removal
 ** 01 May 2008     Shruti Thakral  SPR 18316   Klocwork warning removal
 ** 21-Aug-2009     Tarun Gupta     SPR 19935   Defined icf_port_strncat
 **
 *****************************************************************************
 **			Copyrights 2009, Aricent.
 *****************************************************************************/

#ifndef __ICF_PORT_H__
#define __ICF_PORT_H__


/******************** USER INCLUDE FILES *********************************/

/* Determine if a C++ compiler is being used.
 * If so, ensure that standard
 * C is used to process the header information.
 */
#include "icf_types.h"
#ifdef __cplusplus

/* Yes, C++ compiler is present.
 * Use standard C.
 * */

extern "C"
{
#endif /* __cplusplus */

#ifdef ICF_PORT_LINUX
#include <ctype.h>
#endif

/* --------------------------------------------------------------------------
 * Constants to describe function arguments
 * --------------------------------------------------------------------------*/
#ifndef INOUT
#define INOUT
#endif

#ifndef OUT
#define OUT
#endif

#ifndef IN
#ifndef ICF_PORT_WINDOWS 
#define IN   const
#else
#define IN
#endif
#endif
 
#define ICF_BLK_SIZE_8				8
#define ICF_NUM_BLK_SIZE_8			2400
#define ICF_INDX_BLK_SIZE_8			0
#define ICF_MAX_NON_PRINTABLE_CHARACTER 0x1f
#define ICF_NEXT_LINE_CHARACTER '\n'

#define ICF_BLK_SIZE_16				16
#define ICF_NUM_BLK_SIZE_16			1500
#define ICF_INDX_BLK_SIZE_16		1

#define ICF_BLK_SIZE_32				32
#define ICF_NUM_BLK_SIZE_32			1500
#define ICF_INDX_BLK_SIZE_32		2

#define ICF_BLK_SIZE_64				64
#define ICF_NUM_BLK_SIZE_64			1300
#define ICF_INDX_BLK_SIZE_64		3

#define ICF_BLK_SIZE_128			128
#define ICF_NUM_BLK_SIZE_128		600
#define ICF_INDX_BLK_SIZE_128		4

#define ICF_BLK_SIZE_256			256
#define ICF_NUM_BLK_SIZE_256		400
#define ICF_INDX_BLK_SIZE_256		5

#define ICF_BLK_SIZE_512			512
#define ICF_NUM_BLK_SIZE_512		350
#define ICF_INDX_BLK_SIZE_512		6

#define ICF_BLK_SIZE_1024			1024
#define ICF_NUM_BLK_SIZE_1024		150
#define ICF_INDX_BLK_SIZE_1024		7

#define ICF_BLK_SIZE_2048			2048
#define ICF_NUM_BLK_SIZE_2048		20
#define ICF_INDX_BLK_SIZE_2048		8

#define ICF_BLK_SIZE_4096			4096
#define ICF_NUM_BLK_SIZE_4096		15
#define ICF_INDX_BLK_SIZE_4096		9

#define ICF_BLK_SIZE_8192			8192
#define ICF_NUM_BLK_SIZE_8192		9
#define ICF_INDX_BLK_SIZE_8192		10

#define ICF_BLK_SIZE_10240			10240              
#define ICF_NUM_BLK_SIZE_10240		8
#define ICF_INDX_BLK_SIZE_10240		11

#define ICF_BLK_SIZE_20480			20480
#define ICF_NUM_BLK_SIZE_20480		3
#define ICF_INDX_BLK_SIZE_20480		12

#define ICF_BLK_SIZE_25600			25600
#define ICF_NUM_BLK_SIZE_25600		3
#define ICF_INDX_BLK_SIZE_25600		13


/* Temporary fix: Defining ICF_BLK_SIZE_MAX */
#define ICF_BLK_SIZE_MAX          ICF_BLK_SIZE_25600

#define ICF_MAX_NUM_COMMON_POOL  14

    
/*Following is used in port for reading UDP messages*/
#define ICF_MAX_UDP_MSG_LNGTH ICF_BLK_SIZE_MAX
/*Following is used in port for reading TCP connections*/
#define ICF_MAX_TCP_MSG_LNGTH ICF_BLK_SIZE_MAX
/*Following is used in the pdu pointer array for tcpm connblock*/
#define ICF_MAX_PENDING_PDU_PER_CONN     10
/*Following is used for the size of tcpm conn database.
 * This value is equal to the maximum number of call
 * objects allowed in SSA(ICF_SSA_MAX_CALL_OBJ)*/
#define ICF_MAX_NO_OF_PERSISTENT_CONNECTIONS             (ICF_MAX_NUM_CALL * 4)

#define ICF_MAX_DNS_SERVER_FDS       10
	
/*Following is used to mark the conn_fd in tcpm conn
 * db node invalid. Mapped to UATK hash define
 * Sdf_co_InvalidSocketFd*/
#define ICF_INVALID_SOCKFD       -1

/*Following is used for flushing out all the unused incoming
 * connection
 * THis will be changed so that this valuse is picked from
 * Global Configuration bsaed Call Set Up timer value*/
#if 1 /*ZyXEL,Jason,SIP over TLS*/
 #define ICF_MAX_PURGE_TIMER_DURATION  18000000
#else
#define ICF_MAX_PURGE_TIMER_DURATION  0
#endif
#define ICF_DEFAULT_PURGE_TIMER_DURATION  120000
#define ICF_MAX_CONN_DURATION_LIMIT 10
#define ICF_PURGE_TIMER_DURATION   (ICF_MAX_PURGE_TIMER_DURATION / ICF_MAX_CONN_DURATION_LIMIT)   
/* ----------------------------------------------------------------------
 * Hash Defines : icf_pool_id_t 
 * Description  : This contains the different pool ids for allocation of
 *                memory pools as required by TK.
 * -----------------------------------------------------------------------*/

#define ICF_MEM_COMMON           0
    /*
     * Following are task specific poolids
     */
#define ICF_MEM_SDP              1
#define ICF_MEM_INT_MSG          2
#define ICF_MEM_TIMER            3
#define ICF_MEM_SSA_TIMER        4
#define ICF_MEM_ADDRESS          5
#define ICF_MEM_CALL_CTX_LIST    6
#define ICF_MAX_NUM_POOL         14


        
/*This would identify the multiplying factor for calculating number of SDP 
 * blocks to be reserved in genlib pool from max_configured_call*/
#define ICF_SDP_BLK_MUL_FACTOR    20
#define ICF_ADDRESS_BLK_MUL_FACTOR  10 
#define ICF_TIMER_BLK_MUL_FACTOR    150
/*
 * Max number of timer that could run at a time
 */
#define ICF_MAX_NUM_OF_TIMER      	4000
#define ICF_MAX_NUM_OF_SSA_TIMER_DATA ICF_MAX_NUM_OF_TIMER

/*
 * Number of timer list that genlib should mentian
 */
#define ICF_NUM_OF_TIMER_LIST      1    


/* Values for icf_interface_type_t */
#define ICF_INTF_TYPE_FIFO  0
#define ICF_INTF_TYPE_UDP   1
#define ICF_INTF_TYPE_TCP   2
#define ICF_INTF_TYPE_TLS   3

typedef icf_uint8_t   icf_interface_type_t;


/*------------------------------------------------------------------------------
 *
 * Type Name   :  icf_msg_list_st
 * Description :  List of messages that are received from ES recv
 *
 *----------------------------------------------------------------------------*/
typedef struct icf_msg_list_s
{
    icf_msg_st              *p_msg;

    struct icf_msg_list_s   *p_msg_next;
} icf_msg_list_st;


/* This data is given by ES on receiving message from IP Packetphone toolkit
 * and AL pass this data to os_send. OS_SEND use this data to write mesg on 
 * network */
typedef struct
{
  icf_interface_type_t    interface_type;
  icf_uint32_t  msg_len;
  icf_uint32_t  fd_id;
  icf_uint16_t  dest_port;
#ifdef ICF_IPV6_ENABLED
  icf_uint8_t   dest_ip[ICF_MAX_IPV6_ADDRESS_LENGTH];
#else
  icf_uint8_t   dest_ip[ICF_MAX_IPV4_ADDRESS_LENGTH];
#endif
  icf_void_t    *p_port_info;
  /* Added for SIP DSCP parameter */
  icf_uint8_t   sip_dscp;
}icf_interface_info_st;

/*-----------------------------------------------------------------------
 * NAME : icf_nw_interface_info_st
 * This structure is used by the port/eshell to fill up information about
 * the external interface conditions at the time of reception of message
 * Currently, only Network interface is the one where the information about
 * the interface is variable in time, so the structure is defined to 
 * contain parameters describing that
 *-----------------------------------------------------------------------*/
typedef struct
{

#define     ICF_INTF_INFO_PROTOCOL_UDP   0x01
#define     ICF_INTF_INFO_PROTOCOL_TCP   0x02
#define     ICF_INTF_INFO_PROTOCOL_TLS   0x04
#define     ICF_INTF_INFO_BLK_CONN       0x08
    icf_uint8_t      bitmap;

    icf_uint16_t     pdu_len;
    icf_uint16_t     remote_port_num;

    icf_uint16_t     self_prot_port;
#ifdef ICF_IPV6_ENABLED
	icf_uint8_t      self_ip_addr[ICF_MAX_IPV6_ADDRESS_LENGTH];
#else
    icf_uint8_t      self_ip_addr[ICF_MAX_IPV4_ADDRESS_LENGTH];
#endif

#ifdef ICF_IPV6_ENABLED
	icf_uint8_t      remote_ip_addr[ICF_MAX_IPV6_ADDRESS_LENGTH];
#else
    icf_uint8_t      remote_ip_addr[ICF_MAX_IPV4_ADDRESS_LENGTH];
#endif
    icf_uint32_t     receiver_fd;
    icf_uint8_t      *p_sip_msg;

    icf_uint16_t     tls_server_port_num;
    /* SSL data which is passed to ES */
    icf_void_t       *p_secure_data;

    icf_uint16_t     server_index[ICF_INTF_INFO_PROTOCOL_TLS + 1];

    icf_uint16_t     curr_server_index;

	/* Added for SIP DSCP parameter */
    icf_uint8_t      sip_dscp;
} icf_nw_interface_info_st;


/*-----------------------------------------------------------------------
 * NAME : icf_secure_nw_interface_st
 * This structure is used by the port/eshell to fill up information about
 * an event which occured at the secure nw interface. currently this is 
 * used for message reception, connection closure etc. on a TLS connection
 * only
 *-----------------------------------------------------------------------*/
typedef struct
{
    icf_uint16_t     pdu_len;
	/*fd/handle to the connection on which the message was received*/
    icf_uint32_t     conn_handle;
	/*pointer/handle to the message received from network. It is assumed
	that if it is a pointer the memory for the message will be released
	by port in the return path*/
    icf_uint8_t      *p_http_msg;
} icf_secure_nw_interface_st;

/* --------------------------------------------------------------------
 * Type Name   :  icf_memory_pool_data_st
 * Description :  Structure to hold the info/data required for maintaining
 *                mapping between ICF's pool IDs and the implementation
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
    icf_uint32_t           pool_buffer_size;

    /* number of buffers in the pool */
    icf_uint32_t           num_buffers;

    /* pool identifier */
    icf_uint16_t           pool_id;

} icf_memory_pool_data_st;


/* --------------------------------------------------------------------
 * Type Name   :  icf_buffer_pool_table_st
 * Description :  Structure to hold the instances of pool specific information
 *                chunks(icf_pool_data_t)
 * ------------------------------------------------------------------------*/
typedef struct 
{

    /* number of buffer pools */
    icf_uint16_t             pool_count;

    /* pool specific information */
    icf_memory_pool_data_st  pools[ICF_MAX_NUM_POOL];

}icf_memory_pool_table_st;


/*-----------------------------------------------------------------------------
 Interface structure between ICF and IAS for GPRS
 * simulation by IAS for SI on Windows.
 *---------------------------------------------------------------------------*/
typedef struct
{
	icf_uint32_t    context_id;
} icf_gprs_pri_pdp_activate_req_st;

typedef struct
{
	icf_uint32_t    context_id;
	icf_string_st   auth_token;
} icf_gprs_sec_pdp_activate_req_st;

typedef struct
{
	icf_uint32_t    context_id;
} icf_gprs_pdp_deactivate_req_st;

/*-----------------------------------------------------------------------------
 *
 *  Structure Name : icf_timer_cb_fn_t
 *
 *  Description :    Function ptr type for defining the call back function
 *                  to be invoked by the timer port in the event of a timer
 *                  expiry
 *---------------------------------------------------------------------------*/
typedef icf_void_t* (* icf_timer_cb_fn_t)(icf_void_t*);


/*************************************************************************
*
*  FUNCTION NAME  :   icf_es_init
*
*  DESCRIPTION    :   This function initializes the eshell and porting layer .
*
*  RETURNS:         Either ICF_SUCCESS or ICF_FAILURE
*
*************************************************************************/
extern icf_return_t icf_es_init(
        INOUT icf_void_t       **p_p_port_data,
        INOUT icf_error_t      *p_ecode);

extern icf_return_t icf_es_init_complete_handler(
                        icf_void_t      *p_port_data,
                        icf_error_t      *p_ecode);

/*******************************************************************************
 *
 * FUNCTION:        icf_es_recv
 *
 * DESCRIPTION:    This function will read message from socket, give the message
 *                 to corresponding AL and return message to MRM
 *                 of port to write msg on NW interface
 *
*******************************************************************************/
extern icf_return_t icf_es_recv(IN icf_void_t *p_port_data,
       icf_uint16_t select_timer_duration,/* in milliseconds */
       icf_msg_list_st *p_icf_msg_list,
        icf_error_t *p_ecode);



/*************************************************************************
*
*  FUNCTION NAME  :   icf_es_deinit
*
*  DESCRIPTION    :   This function deinit the eshell and porting layer .
*
*  RETURNS:         Either ICF_SUCCESS or ICF_FAILURE
*
*************************************************************************/
extern icf_return_t icf_es_deinit(
        INOUT icf_void_t       *p_port_data,
        INOUT icf_error_t      *p_ecode);


/*************************************************************************
*
*  FUNCTION NAME  :   icf_port_memory_init
*
*  DESCRIPTION    :   This function initializes common memory pools.
*
*  RETURNS:         Either ICF_SUCCESS or ICF_FAILURE depending on
*                  whether the pointer was successfull or not.
*
*************************************************************************/
extern icf_return_t icf_port_memory_init(
         IN icf_void_t                *p_port_info,
         IN icf_memory_pool_table_st  *p_cmn_pool_data,
         INOUT icf_error_t              *p_ecode );

/*************************************************************************
*
*  FUNCTION NAME  :   icf_port_task_memory_init
*
*  DESCRIPTION    :   This function initializes  memory pools.
*
*  RETURNS:         Either ICF_SUCCESS or ICF_FAILURE depending on
*                  whether the pointer was successfull or not.
*
*************************************************************************/
extern  icf_return_t
icf_port_task_memory_init(
         IN icf_void_t               *p_port_info,
         IN icf_memory_pool_table_st *p_task_pool_data,
         INOUT icf_error_t            *p_ecode);



/*************************************************************************
 *
 *  FUNCTION NAME   :    icf_port_timer_init
 *
 *  DESCRIPTION     :    This function is the port function for initializing 
 *                       the timer. It attaches a call back function to the
 *                       same.
 * DESCRIPTION	:  	This function initializes the genlib timer library.
 *
 *  RETURNS         :    ICF_SUCCESS if successful, else ICF_FAILURE
***************************************************************************/
extern  icf_return_t icf_port_timer_init(
         IN icf_void_t            *p_port_info,
         IN icf_timer_cb_fn_t     p_timer_cb_fn,
         IN icf_uint8_t           no_of_timer_list,
         IN icf_uint16_t          no_of_timers,
         INOUT icf_error_t        *p_ecode);


extern icf_return_t icf_send(IN icf_void_t *p_port_data,
                            INOUT icf_msg_st *p_msg_data,
                            OUT icf_error_t *p_ecode);


/*******************************************************************************
 *
 * FUNCTION:        icf_es_open_nw_port
 *
 * DESCRIPTION:    This function will open up a UDP/TCP socket ( as specified)
 *                  by transport_type, and bind to it and add the FD in the li
 *                  istening FDSET maintained in global port info. It will 
 *                  also return the bound fd to calling context through the 
 *                  pointer of the nw interface structure
 *                  Only the Port and transport type info is passed
*******************************************************************************/
extern icf_return_t icf_es_open_nw_port(
        INOUT icf_void_t                 *p_port_data,
        INOUT icf_nw_interface_info_st   *p_nw_intf_info,
		INOUT icf_int8_t                 *p_self_ip,
        OUT   icf_error_t                *p_ecode);

/*******************************************************************************
 * FUNCTION:        icf_es_close_nw_servers
 * DESCRIPTION:   
*******************************************************************************/
extern icf_return_t icf_es_close_nw_servers(
        INOUT icf_void_t                 *p_port_data,
	IN    icf_uint16_t               server_index);

/*******************************************************************************
 * FUNCTION:        icf_es_close_tcp_conn
 *
 * DESCRIPTION:    This function will close a TCP connection, it searches
 * through both the read conn array and pending conn array
*******************************************************************************/
extern icf_return_t icf_es_close_tcp_conn(
        INOUT icf_void_t                 *p_port_data,
        INOUT icf_nw_interface_info_st   *p_nw_intf_info,
        OUT   icf_error_t                *p_ecode);

/*******************************************************************************
 * FUNCTION:        icf_es_open_tcp_conn
 * DESCRIPTION:    This function will attempt to open UP a TCP stream socket,
 * then attempt to issue connect on it. The socket is opened in non-blocking
 * mode
*******************************************************************************/
extern icf_return_t icf_es_open_tcp_conn(
        INOUT icf_void_t                 *p_port_data,
        INOUT icf_nw_interface_info_st   *p_nw_intf_info,
        OUT   icf_error_t                *p_ecode);

/*******************************************************************************
 *
 * FUNCTION:        icf_es_port_close_sa_servers 
 *
 * DESCRIPTION:     This function is invoked from SSA to close the servers 
 * 					openned on the protected ports
 *
 * RETURNS:         ICF_SUCCESS
 *
 ******************************************************************************/
icf_return_t icf_es_port_close_sa_servers (
    INOUT icf_void_t     *p_port_data);

#ifdef ICF_SECURE_SUPPORT
icf_void_t* icf_es_fetch_secure_data_from_map(
        icf_void_t *p_port_info,
        icf_uint32_t fd_id,
        icf_error_t *p_ecode);

icf_return_t icf_es_add_secure_data_to_map(
        icf_void_t *p_secure_data,
        icf_void_t *p_port_info,
        icf_uint32_t fd_id,
        icf_error_t *p_ecode);

icf_void_t icf_es_rem_secure_data_from_map(
        icf_void_t *p_port_info,
        icf_uint32_t fd_id);
#endif


#if defined (ICF_PORT_WINDOWS) || defined (ICF_PORT_WINCE) || defined (ICF_PORT_LINUX) 
#define icf_port_strtokr self_implemented_strtok_r
#else
#define icf_port_strtokr strtok_r
#endif
#define icf_port_strtok strtok


#ifdef ICF_PORT_WINCE
#define icf_port_strcasecmp _stricmp
#else

#if defined (ICF_PORT_WINDOWS)
#define icf_port_strcasecmp strcmpi

#else
#if defined (ICF_PORT_VXWORKS)
#define icf_port_strcasecmp icf_port_strcmpi
#else
#define icf_port_strcasecmp strcasecmp
#endif/* end of ifdef for VXWORKS */
#endif /* end of ifdef for windows */
#endif /* end of ifdef for WINCE */



#define icf_port_atoi atoi
#define icf_port_sscanf sscanf
#define icf_port_sprintf sprintf
#define icf_port_strncpy strncpy
#define icf_port_isspace isspace

#define icf_port_tolower tolower
#define icf_port_strstr strstr
/* SPR 19935 */
#define icf_port_strncat strncat


#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* End of ifndef __ICF_PORT_H__ */
