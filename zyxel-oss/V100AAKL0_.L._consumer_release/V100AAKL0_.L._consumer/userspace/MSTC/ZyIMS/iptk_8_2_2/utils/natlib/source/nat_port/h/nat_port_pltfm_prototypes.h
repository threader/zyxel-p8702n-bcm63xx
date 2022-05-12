/******************************************************************************
*                                    
* FILE NAME    : nat_port_pltfm_prototypes.h                
*                                    
* DESCRIPTION  : Contains the definitions for the structures used in NAT Client Prototypes
*                APIs.    
*                                    
* Revision History :                            
*
*  DATE            NAME                        REFERENCE       REASON        
* ------         ------------                  ---------     ----------
* 10-Jul-2006	Anuradha Gupta					LLD			Initial Version
* 16-Nov-2006	Anuradha Gupta					LLD			Coding
* 01-Mar-2007 	Krishna Ram Dhunju							Changes made for vxworks port
*
* Copyright 2006, Aricent.                       
*                                    
*******************************************************************************/



#ifndef __NAT_PORT_PLTFM_PROTOTYPES_H_

#define __NAT_PORT_PLTFM_PROTOTYPES_H_

#include "nat_common_types.h"
#include "nat_common_port_types.h"
#include "nat_port_types.h"



#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

#if defined(NAT_PORT_WINDOWS) || defined(NAT_PORT_LINUX) || defined(NAT_PORT_WINCE) || defined(NAT_PORT_VXWORKS)

#define nat_port_sizeof  	sizeof
#define nat_port_sprintf    	sprintf
#define nat_port_htons		htons
#define nat_port_htonl      	htonl
#define nat_port_file_t		FILE
#define nat_port_fopen		fopen
#define nat_port_fclose		fclose
#define nat_port_sscanf		sscanf
#define nat_port_fgets      fgets
#define nat_port_strncpy      strncpy

#define nat_port_assert(condition) \
{\
        if((condition)) {}\
}

#endif

/*************************************************************************
*
*  FUNCTION NAME  :   nat_port_static_memget
*
*  DESCRIPTION    :   This function is called to allocate static buffers
*                     in the system.
*
* 
*  RETURNS        :   Pointer to the allocated buffer if there is available
*                     memory in the system, else NAT_NULL with ecode as
*                     NAT_ERR_MEM_ALLOC_FAILURE.
* 
 *************************************************************************/

extern nat_void_t*
nat_port_static_memget(
        IN  nat_uint32_t          size,
        OUT nat_error_t    *p_ecode );

/*************************************************************************
*
*  FUNCTION NAME  :   nat_port_memset
*
*  DESCRIPTION    :   Utility function for setting the buffer to zero
*
*  RETURNS        :  Void
*
 *************************************************************************/
extern nat_void_t nat_port_memset(
        IN  nat_void_t       *p_buf,
        IN  nat_uint8_t      ch,
        IN  nat_uint32_t     n);



/*************************************************************************
*
*  FUNCTION NAME  :   nat_port_static_memfree
*
*  DESCRIPTION    :   This function is called to free static buffers
*                     in the system.
*
*
*  RETURNS        :   NAT_RETURN_SUCCESS/NAT_RETURN_FAILURE
*
 *************************************************************************/
extern nat_return_t
nat_port_static_memfree(
        IN  nat_void_t     *p_buf,
        OUT nat_error_t    *p_ecode );

/*************************************************************************
*
*  FUNCTION NAME  :   nat_port_memcpy
*
*  DESCRIPTION    :   Utility function for copying memory block.
*
*  RETURNS        :   Address of the destination memory location 
*
 *************************************************************************/     
extern nat_void_t nat_port_memcpy(
               OUT     nat_void_t *dest_buff, 
              IN       nat_void_t *src_buff,
              IN       nat_uint32_t  size);
  
/*************************************************************************
*
*  FUNCTION NAME  :   nat_port_strcmp
*
*  DESCRIPTION    :   Utility function for string comparison.
*
*  RETURNS        :  0 if strings match, non zero if they don't
*
 *************************************************************************/           
extern nat_int32_t nat_port_strcmp(
        IN nat_uint8_t* p_str1,
        IN  nat_uint8_t* p_str2);
       
/*************************************************************************
*
*  FUNCTION NAME  :   nat_port_strcpy
*
*  DESCRIPTION    :   Utility function for string copy.
*
*  RETURNS        :  Void
*
 *************************************************************************/
extern nat_void_t nat_port_strcpy(
        INOUT nat_uint8_t   *p_dest,
        IN  nat_uint8_t   *p_src);
     
/*************************************************************************
*
*  FUNCTION NAME  :   nat_port_memget
*
*  DESCRIPTION    :   This function is called to allocate buffers in the system.
*  Arguments      :   size      - Size of memory buffer required
*                     pool_id   - Pool from which memory is required
*                     p_eocde   - pointer to variable to return error code
*
*  RETURNS        :   Pointer to the allocated buffer if there is available 
*                     memory in the system, else NAT_NULL with ecode as 
*                     NAT_OUT_OF_MEM.
*
*************************************************************************/
extern nat_void_t* 
nat_port_memget(
        IN  nat_void_t           *p_port_info,
        IN  nat_uint32_t         size,
        IN  nat_pool_id_t        pool_id,
        OUT nat_error_t          *p_ecode);
    
/*************************************************************************
*
*  FUNCTION NAME  :   nat_port_memfree
*
*  DESCRIPTION    :   This function is called to deallocate buffers.
*  RETURNS        :   void.
*
*************************************************************************/
extern nat_return_t
nat_port_memfree(
        IN  nat_void_t       *p_port_info,
        IN  nat_void_t       *p_buf,
        IN  nat_pool_id_t    pool_id,
        OUT nat_error_t      *p_ecode);


/*************************************************************************
*
*  FUNCTION NAME  :   nat_port_init
*
*  DESCRIPTION    :   This function initializes the porting layer .
*
*  RETURNS:         Either NAT_RETURN_SUCCESS or NAT_RETURN_FAILURE
*
*************************************************************************/
extern nat_return_t
nat_port_init(
        INOUT nat_void_t                **p_p_port_info,
        OUT nat_error_t              *p_ecode );

/*************************************************************************
*
*  FUNCTION NAME  :   nat_port_memory_init
*
*  DESCRIPTION    :   This function initializes common memory pools.
*
*  RETURNS:         Either NAT_RETURN_SUCCESS or NAT_RETURN_FAILURE depending on
*                  whether the pointer was successfull or not.
*
*************************************************************************/
extern nat_return_t
nat_port_memory_init(
        IN nat_void_t              *p_port_info,
        IN nat_memory_pool_table_st *p_cmn_pool_data,
        OUT nat_error_t            *p_ecode );

/*************************************************************************
*
*  FUNCTION NAME  :   nat_port_task_memory_init
*
*  DESCRIPTION    :   This function initializes  memory pools.
*
*  RETURNS:         Either NAT_RETURN_SUCCESS or NAT_RETURN_FAILURE depending on
*                  whether the pointer was successfull or not.
*
*************************************************************************/
extern nat_return_t
nat_port_task_memory_init(
         IN nat_void_t               *p_port_info,
         IN nat_memory_pool_table_st *p_task_pool_data,
         INOUT nat_error_t            *p_ecode);

/*************************************************************************
*
*  FUNCTION NAME  :   nat_mem_deinit
*
*  DESCRIPTION    :   This function destroys memory pools.
*
*  RETURNS:         Either NAT_RETURN_SUCCESS or NAT_RETURN_FAILURE
*
*************************************************************************/
extern nat_return_t
nat_mem_deinit(
         IN nat_void_t               *p_port_info,
         INOUT nat_error_t            *p_ecode);

/*************************************************************************
*
*  FUNCTION NAME  :   nat_port_realloc
*
*  DESCRIPTION    :   reallocates memory
*
*  RETURNS        :   
*
*************************************************************************/
extern nat_void_t* 
nat_port_realloc(
        INOUT  	nat_void_t           	*p_port_info,
        IN  	nat_uint32_t		size
        );


/*************************************************************************
 *
 *  FUNCTION NAME   :    nat_port_start_sync_timer
 *
 *  DESCRIPTION     :    This function starts the NAT timer for an SLP
 *                       It consequently has an SLP specific interface. 
 *
 *  RETURNS         :    NAT_RETURN_SUCCESS if successful else NAT_RETURN_FAILURE 
 *************************************************************************/

extern nat_return_t 
nat_port_start_sync_timer(
 INOUT    nat_void_t           *p_port_info,
 IN    nat_timer_data_st    *p_buff,
 IN	   nat_uint32_t         duration,
 OUT   nat_timer_id_t       *p_timer_id,
 OUT   nat_error_t  	       *p_ecode);


/*************************************************************************
 *
 *  FUNCTION NAME   :    nat_port_stop_sync_timer
 *
 *  DESCRIPTION     :    This function is the port function for stopping 
 *                       a started timer. It has checks for timers not
 *                       started.
 *
 *  RETURNS         :    NAT_RETURN_SUCCESS if successful, else NAT_RETURN_FAILURE
 *************************************************************************/
extern nat_return_t
nat_port_stop_sync_timer(
        INOUT          nat_void_t       *p_port_info,
        INOUT       nat_timer_id_t   timer_id,
        OUT         nat_error_t      *p_ecode);

/*************************************************************************
 *
 *  FUNCTION NAME   :    nat_port_timer_init
 *
 *  DESCRIPTION     :    This function is the port function for initializing 
 *                       the timer. It attaches a call back function to the
 *                       same.
 * DESCRIPTION	:  	This function initializes the genlib timer library.
 *
 *  RETURNS         :    NAT_RETURN_SUCCESS if successful, else NAT_RETURN_FAILURE
***************************************************************************/
extern nat_return_t nat_port_timer_init(
         IN nat_void_t            *p_port_info,
         IN nat_timer_cb_fn_t     p_timer_cb_fn,
         IN nat_uint8_t           no_of_timer_list,
         IN nat_uint16_t          no_of_timers,
         INOUT nat_error_t        *p_ecode);


/*************************************************************************
 *
 *  FUNCTION NAME   :    nat_port_timer_deinit
 *
 *  DESCRIPTION     :    This function is the port function for initializing 
 *                       the timer. It attaches a call back function to the
 *                       same.
 * DESCRIPTION	:  	This function initializes the genlib timer library.
 *
 *  RETURNS         :    NAT_RETURN_SUCCESS if successful, else NAT_RETURN_FAILURE
***************************************************************************/

extern nat_return_t nat_port_timer_deinit(
        IN nat_void_t            *p_port_info,
        OUT   nat_error_t *p_ecode);


/*************************************************************************
 *
 *  FUNCTION NAME   :    nat_port_check_timer_expiry
 *
 *  DESCRIPTION     :    This function loops over all the timer lists
 *                      and checks the timers for expiry
 *  RETURNS         :    Nothing 
 *************************************************************************/
extern nat_void_t 
nat_port_check_timer_expiry(
        INOUT      nat_void_t       *p_port_info,
        OUT     nat_error_t      *p_ecode);

/*************************************************************************
 *
 *  FUNCTION NAME   :    nat_port_get_current_time
 *
 *  DESCRIPTION     :    This function gets the current time in seconds
 *  RETURNS         :    Nothing 
 *************************************************************************/

extern nat_return_t 
nat_port_get_current_time(
				nat_time_t		 *p_curr_time);


/*************************************************************************
*
*  FUNCTION NAME  :   nat_port_flush_fifo
*
*  DESCRIPTION    :   This function is used to read and dump all the data
*                   pending on the FIFO correspondig to the FD
*
*  RETURNS:         Either NAT_RETURN_SUCCESS or NAT_RETURN_FAILURE
*************************************************************************/
extern nat_return_t
nat_port_flush(
        nat_uint32_t             fd,
        nat_uint32_t             msg_len,
        nat_error_t              *p_ecode);


/*************************************************************************
*
*  FUNCTION NAME  :   nat_port_select
*
*  DESCRIPTION    :   This function invokes the system call select
*
*  RETURNS:         Either NAT_RETURN_SUCCESS or NAT_RETURN_FAILURE
*               timeout_val is in microseconds.
*************************************************************************/
extern nat_return_t
nat_port_select(nat_fd_set    *p_read_fdset,
        nat_fd_set       *p_write_fdset,
		nat_fd_set		*p_except_fdset,
        nat_uint32_t     timeout_val,
        nat_error_t      *p_ecode);

/*************************************************************************
*
*  FUNCTION NAME  :   nat_port_open_socket
*
*  DESCRIPTION    :   This function opens a TCP/UDP 
*                       socket. This is a subset of the next routine
*                       in the sense that it does not bind to a port num
*                       currentlt this will be used by SSA for opening
*                       tcp client connection
*
*  RETURNS:         Either NAT_RETURN_SUCCESS or NAT_RETURN_FAILURE
*************************************************************************/
extern nat_return_t
nat_port_open_socket(
        /*NAT_TRANSPORT_TYPE_UDP/NAT_TRANSPORT_TYPE_TCP*/
#ifdef NAT_IPV6_ENABLED
		nat_uint32_t		proto_family,
#endif
        nat_uint8_t      transport_type,
        nat_uint32_t     *p_fd,
        nat_error_t      *p_ecode);


/*************************************************************************
*
*  FUNCTION NAME  :   nat_port_open_channel
*
*  DESCRIPTION    :   This function opens the TCP/UDP n/w channel
*                       ->It allocates a fd from system
*                       ->It binds the fd to the port specified
*                       For TCP,it will be the server port
*                       Note: This assumes IPv4 only
*
*  RETURNS:         Either NAT_RETURN_SUCCESS or NAT_RETURN_FAILURE
*
*************************************************************************/
extern nat_return_t
nat_port_open_channel(nat_uint32_t    port_num, 

        /*NAT_TRANSPORT_TYPE_UDP/NAT_TRANSPORT_TYPE_TCP*/
        nat_uint8_t      transport_type,

        /*TRUE->Tk is passing a dotted IP address
         * FALSE->port should use IPADDR_ANY
         */
        nat_boolean_t    is_ipaddr_present,
        nat_uint8_t      *p_ipaddr,

        nat_uint32_t     *p_fd,
        nat_error_t      *p_ecode);


/*************************************************************************
*
*  FUNCTION NAME  :   nat_port_udp_send
*
*  DESCRIPTION    :   This function sends the message on UDP channel.
*
*  RETURNS:         Either NAT_RETURN_SUCCESS or NAT_RETURN_FAILURE
*
*************************************************************************/
extern nat_return_t
nat_port_udp_send(nat_uint32_t    fd,
                     nat_void_t     *p_msg,
                     nat_uint32_t    msg_len,
                     nat_uint32_t    dest_port_num,
                     nat_uint8_t*    dest_ip_addr,
                     nat_error_t    *p_ecode);


/*************************************************************************
*
*  FUNCTION NAME  :   nat_port_tcp_send
*
*  DESCRIPTION    :   This function sends the message on the TCP conn
*                       corresponding to the fd passed.
*
*  RETURNS:         Either NAT_RETURN_SUCCESS or NAT_RETURN_FAILURE
*
*************************************************************************/
extern nat_return_t
nat_port_tcp_send(nat_uint32_t    fd,
                     nat_void_t     *p_msg,
                     nat_uint32_t    msg_len,
                     nat_error_t    *p_ecode);

/*************************************************************************
*
*  FUNCTION NAME  :   nat_port_udp_recv
*
*  DESCRIPTION    :   This function reads a message from udp socket and
*                   returns the data read and the number of bytes read
*
*  RETURNS:         Either NAT_RETURN_SUCCESS or NAT_RETURN_FAILURE
*
*************************************************************************/
extern nat_return_t
nat_port_udp_recv(nat_interface_info_st  *p_interface_info,
                     nat_void_t     *p_msg,
                     nat_error_t    *p_ecode);

/*************************************************************************
*
*  FUNCTION NAME  :   nat_port_close_socket
*
*  DESCRIPTION    :   This function is used to close udp/tcp socket
*
*  RETURNS:         Either NAT_RETURN_SUCCESS or NAT_RETURN_FAILURE
*************************************************************************/
extern nat_void_t
nat_port_close_socket(
        nat_uint32_t             fd,
        nat_error_t              *p_ecode);

#if defined(NAT_PORT_WINDOWS) || defined(NAT_PORT_WINCE)
/*************************************************************************
*
*  FUNCTION NAME  :   nat_port_recv_udp_win
*
*  DESCRIPTION    :  This function reads a from UDP socket. 
*                    
*
*  RETURNS:         Either NAT_RETURN_SUCCESS or NAT_RETURN_FAILURE
*
*************************************************************************/
extern nat_return_t 
nat_port_recv_udp_win(nat_global_port_info_st *p_glb_port_data,
						 nat_uint16_t			fd_id,
						 nat_msg_list_st			*p_nat_msg_list,
						 nat_error_t				*p_ecode);

#endif

/*************************************************************************
*  FUNCTION NAME  :   nat_port_close_fd
*  DESCRIPTION    :   Wrapper for close system call
*************************************************************************/
extern nat_void_t
nat_port_close_fd(nat_uint32_t    fd);

/*************************************************************************
*  FUNCTION NAME  :   nat_port_connect
*  DESCRIPTION    :   Wrapper for connect system call
*  This routine is invoked by IPPHONETK to make a non-blocking connect
*  to a TCP server. If the connection is accepted immediately, it 
*  returns a success, if the peer sends a PENDING signal, it returns failure
*  with error code set as CONNECT_RESP_AWAITED and in case there is 
*  no TCP server at  peer it return failure wo this error.
*************************************************************************/
extern nat_return_t
nat_port_nonblk_connect(
        INOUT  nat_uint16_t  fd,
        IN     nat_uint16_t  dest_port_num,
        IN     nat_uint8_t   *p_dest_ip,
        IN     nat_uint16_t  self_prot_port,
        IN     nat_uint8_t   *p_self_ip,
        OUT    nat_error_t   *p_ecode
        );

/*************************************************************************
*  FUNCTION NAME  :   nat_port_blk_connect
*  DESCRIPTION    :   Wrapper for connect system call
*  This routine is invoked by IPPHONETK to make a blocking connect
*  to a TCP server.
*************************************************************************/
extern nat_return_t
nat_port_blk_connect(
        INOUT  nat_uint16_t  fd,
        IN     nat_uint16_t  dest_port_num,
        IN     nat_uint8_t   *p_dest_ip,
        IN     nat_uint16_t  self_prot_port,
        IN     nat_uint8_t   *p_self_ip,
        OUT    nat_error_t   *p_ecode
        );

/*************************************************************************
*  FUNCTION NAME  :   nat_port_bind
*  DESCRIPTION    :   Wrapper for bind system call. This would primarily be
*  					  required incase of IPSec
*************************************************************************/
extern nat_return_t
nat_port_bind(INOUT  nat_uint16_t  fd,
 		IN	   nat_uint8_t	*p_self_ip,
		IN	   nat_uint16_t	self_port_num,
        OUT    nat_error_t   *p_ecode
        );

/*************************************************************************
*  FUNCTION NAME  :   nat_port_isconnected
*  DESCRIPTION    :   Wrapper for getpeername system call
*************************************************************************/
extern nat_boolean_t
nat_port_isconnected(
        IN      nat_uint32_t         fd);

/*************************************************************************
*  FUNCTION NAME  :   nat_port_accept
*  DESCRIPTION    :   Wrapper for accept system call
*************************************************************************/
extern nat_return_t
nat_port_accept(
        IN      nat_uint32_t         server_fd,
        INOUT   nat_uint32_t     *p_fd,
        INOUT   nat_uint8_t      *p_peer_ip,
        INOUT   nat_uint16_t     *p_peer_port);

/*************************************************************************
*  FUNCTION NAME  :   nat_port_tcp_recv
*  DESCRIPTION    :   This function reads the given number of bytes from 
*  given TCP conn fd
*  RETURNS:         Either NAT_RETURN_SUCCESS or NAT_RETURN_FAILURE
*************************************************************************/
extern nat_return_t
nat_port_tcp_recv(nat_uint16_t    fd,
        nat_void_t     *p_msg,
        nat_uint32_t  msg_len);

/*************************************************************************
*  FUNCTION NAME  :   nat_port_tcp_peek
*  DESCRIPTION    :   This function peeks a message from tcp socket
*  RETURNS:         Either NAT_RETURN_SUCCESS or NAT_RETURN_FAILURE
*************************************************************************/
extern nat_return_t
nat_port_tcp_peek(nat_interface_info_st  *p_interface_info,
                     nat_void_t     *p_msg,
                     nat_error_t    *p_ecode);

/*************************************************************************
*  FUNCTION NAME  :   nat_port_gethostbyname
*  DESCRIPTION    :   Wrapper for gethostbyname.p_p_addr is the address
*           of memory allocated by calling routine for storing the IP
*  RETURNS:         Either NAT_RETURN_SUCCESS or NAT_RETURN_FAILURE
*************************************************************************/
extern nat_return_t     nat_port_gethostbyname(
        IN  nat_uint8_t*     p_hostname,
        INOUT   nat_uint8_t*    p_addr);

/*************************************************************************
*
*  FUNCTION NAME  :   nat_port_recvfrom
*
*  DESCRIPTION    :   This function reads a message from udp socket and
*                   returns the data read and the number of bytes read
*
*
*************************************************************************/
extern nat_int32_t nat_port_recvfrom(nat_int32_t fd, nat_uint8_t* p_msg,
                                     nat_ip_port_address_st *p_dest_addr);

/*************************************************************************
*
*  FUNCTION NAME  :   nat_port_strmatch
*
*  DESCRIPTION    :   Utility function for string comparison.
*                     It checks if str1 comes at the start of str2
*                     This is slightly different than strcmp. 
*                     It lets str2 to be longer than str1.
*
*  RETURNS        :   0 if strings match, non zero if they don't
*
 *************************************************************************/
extern nat_int32_t nat_port_strmatch(
        IN nat_uint8_t   *p_str1,
        IN nat_uint8_t   *p_str2);
                
/*************************************************************************
*
*  FUNCTION NAME  :   nat_port_strlen
*
*  DESCRIPTION    :   Utility function for calculating string length.
*
*  RETURNS        :   A U32 holding value of the length of the string  
*
 *************************************************************************/
extern nat_uint32_t
    nat_port_strlen (IN nat_uint8_t    *p_s);

/*************************************************************************
*
*  FUNCTION NAME  :   nat_port_strcat
*
*  DESCRIPTION    :   Utility function for string cat.
*
*  RETURNS        :  Void
*
 *************************************************************************/
extern nat_void_t nat_port_strcat(
        INOUT nat_uint8_t   *p_dest,
        IN  nat_uint8_t   *p_src);

/*************************************************************************
*
*  FUNCTION NAME  :   nat_port_strdup
*
*                     Duplicates the the string 
*Uses:
*
*Exceptions:
*
*******************************************************************************/
extern nat_uint8_t*  nat_port_strdup (
        IN nat_uint8_t* p_string);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /*__NAT_PORT_PRTOTYPES_H_ */



