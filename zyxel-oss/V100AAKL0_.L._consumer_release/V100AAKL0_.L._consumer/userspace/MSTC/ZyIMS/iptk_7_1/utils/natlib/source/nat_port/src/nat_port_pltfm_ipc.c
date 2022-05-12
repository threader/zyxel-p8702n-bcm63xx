/******************************************************************************
*                                    
* FILE NAME    : nat_port_pltfm_ipc.c                
*                                    
* DESCRIPTION  : This file contains port wrappers for IPC
*                mechanisms ( writing to UDP/TCP sockets, pipes,
*                reading from the same, opening of channels etc.)
*                          
* Revision History :                            
*
*  DATE            NAME                        REFERENCE       REASON        
* ------         ------------                  ---------     ----------
* 20-Nov-2006  	Anuradha Gupta                  HLD         Initial Version
* 01-MAR-2007	Krishna Ram Dhunju							Changes made for vxworks port
*
* Copyright 2006, Aricent                  
*                                    
*******************************************************************************/
#include "nat_types.h"
#include "nat_port_pltfm_prototypes.h"
#include "nat_error_id.h"
#include "nat_port_types.h"
#include "nat_common_defs.h"
#include "nat_common_prototypes.h"



/*************************************************************************
*
*  FUNCTION NAME  :   nat_port_flush_fifo
*
*  DESCRIPTION    :   This function is used to read and dump all the data
*                   pending on the FIFO correspondig to the FD
*
*  RETURNS:         Either NAT_RETURN_SUCCESS or NAT_RETURN_FAILURE
*************************************************************************/
nat_return_t
nat_port_flush(
        nat_uint32_t             fd,
        nat_uint32_t             msg_len,
        nat_error_t              *p_ecode)
{
    nat_return_t     ret_val = NAT_RETURN_SUCCESS;
#if !(defined(NAT_PORT_WINDOWS) || defined(NAT_PORT_WINCE))
    nat_fd_set       readfdset;

#endif
    nat_uint8_t      *p_msg = NAT_NULL;

#if defined(NAT_PORT_WINDOWS)
    msg_len = msg_len;
	p_msg = p_msg;
	fd=fd;
#endif 
#if defined(NAT_PORT_LINUX) || defined(NAT_PORT_SOLARIS) || defined(NAT_PORT_VXWORKS)
	
    struct timeval      timeout;

    timeout.tv_sec  = 0;
    timeout.tv_usec = 0;

    p_msg = (nat_uint8_t*)malloc(msg_len); 
    NAT_PORT_FD_ZERO( &readfdset);
    NAT_PORT_FD_SET(fd, &readfdset);
    while ( NAT_NULL < 
            select(FD_SETSIZE,&readfdset,NAT_NULL,NAT_NULL,&timeout))
    {
        read(fd, p_msg, msg_len);
        NAT_PORT_FD_SET(fd, &readfdset);
    }
    free(p_msg);
#endif

    p_ecode = p_ecode;

    return ret_val;
}

/*************************************************************************
*
*  FUNCTION NAME  :   nat_port_select
*
*  DESCRIPTION    :   This function invokes the system call select
*
*  RETURNS:         Either NAT_RETURN_SUCCESS or NAT_RETURN_FAILURE
*               timeout_val is in microseconds.
*************************************************************************/
nat_return_t
nat_port_select(nat_fd_set    *p_read_fdset,
        nat_fd_set       *p_write_fdset,
		nat_fd_set		*p_except_fdset,
        nat_uint32_t     timeout_val,
        nat_error_t      *p_ecode)
{
    nat_return_t     ret_val = NAT_RETURN_SUCCESS;
    int                 retn = 0,return1 = 0;
#if defined(NAT_PORT_WINDOWS) || defined(NAT_PORT_WINCE)
    int                 err;
#endif

#if defined(NAT_PORT_LINUX) || defined(NAT_PORT_SOLARIS) || defined(NAT_PORT_WINDOWS) || defined(NAT_PORT_WINCE)|| defined(NAT_PORT_VXWORKS)
    struct timeval      timeout;
#endif

    p_ecode = p_ecode;
    p_write_fdset = p_write_fdset;
    p_read_fdset = p_read_fdset;
    timeout_val = timeout_val;

#if defined(NAT_PORT_LINUX) || defined(NAT_PORT_SOLARIS) || defined (NAT_PORT_WINDOWS) || defined(NAT_PORT_WINCE)|| defined(NAT_PORT_VXWORKS)
    timeout.tv_sec  = 1;
    timeout.tv_usec  = 0;
    if ( (return1 = select(FD_SETSIZE, p_read_fdset,
                p_write_fdset, p_except_fdset, &timeout)) == retn)
    {
        /*p_ecode = NAT_SELECT_FAILURE;*/
        ret_val = NAT_RETURN_SUCCESS;
    }
#endif
#if defined(NAT_PORT_WINDOWS) || defined(NAT_PORT_WINCE)
    err = WSAGetLastError();
#endif
    return ret_val;
}



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
nat_return_t
nat_port_open_socket(
        /*NAT_TRANSPORT_TYPE_UDP/NAT_TRANSPORT_TYPE_TCP*/
#ifdef NAT_IPV6_ENABLED
		nat_uint32_t		proto_family,
#endif
        nat_uint8_t      transport_type,
        nat_uint32_t     *p_fd,
        nat_error_t      *p_ecode)
{
#if defined(NAT_PORT_SOLARIS) || defined(NAT_PORT_LINUX) || defined(NAT_PORT_WINDOWS) || defined(NAT_PORT_WINCE)|| defined(NAT_PORT_VXWORKS)
    int                 on = 1;
    nat_int32_t      fd;
#if defined(NAT_PORT_WINDOWS) || defined(NAT_PORT_WINCE)
	nat_uint32_t		ioctl_arg =1;
#endif

    if ( NAT_TRANSPORT_TYPE_UDP == transport_type)
    {
        fd = socket(
#ifdef NAT_IPV6_ENABLED
			proto_family,
#else
			AF_INET, 
#endif
			SOCK_DGRAM, 0);
    }
    else
    {
        fd = socket(
#ifdef NAT_IPV6_ENABLED
			proto_family,
#else
			AF_INET, 
#endif
			SOCK_STREAM, 0);
    }
    if ( 0 > fd)
    {
        *p_ecode = NAT_ERR_SOCKET_CREATE;
        return (NAT_RETURN_FAILURE);
    }
#if defined(NAT_PORT_WINDOWS) || defined(NAT_PORT_WINCE)
	/* for TCP connection */
	if (0 != ioctlsocket(fd, FIONBIO, (unsigned long*)&ioctl_arg))
    {
        *p_ecode = NAT_ERR_SET_SOCKET;
        return (NAT_RETURN_FAILURE);
    }
#endif

    if ( - 1 == setsockopt(fd,SOL_SOCKET, SO_REUSEADDR, (char*)(&on), 
			sizeof(int)))
    {
        *p_ecode = NAT_ERR_SET_SOCKET;
        return (NAT_RETURN_FAILURE);
    }
    *p_fd = fd;
#endif
    return NAT_RETURN_SUCCESS;
}
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
nat_return_t
nat_port_open_channel(nat_uint32_t    port_num, 

        /*NAT_TRANSPORT_TYPE_UDP/NAT_TRANSPORT_TYPE_TCP*/
        nat_uint8_t      transport_type,

        /*TRUE->Tk is passing a dotted IP address
         * FALSE->port should use IPADDR_ANY
         */
        nat_boolean_t    is_ipaddr_present,
        nat_uint8_t      *p_ipaddr,

        nat_uint32_t     *p_fd,
        nat_error_t      *p_ecode)
{
#if (defined(NAT_PORT_SOLARIS) || defined(NAT_PORT_LINUX))|| defined(NAT_PORT_WINDOWS) || defined(NAT_PORT_WINCE)|| defined(NAT_PORT_VXWORKS)
    nat_int32_t     fd;
    int                 on = 1;
#ifdef NAT_IPV6_ENABLED
	struct sockaddr_storage recv_addr;
	char port[6] = "\0";
	struct addrinfo ai_hints;
	struct addrinfo *p_ai_response = NULL;
#else
    struct sockaddr_in  recv_addr;
#endif
#if defined(NAT_PORT_WINDOWS) || defined(NAT_PORT_WINCE)
	nat_uint32_t		ioctl_arg =1;
#endif
    size_t  remote_addr_len = sizeof(struct sockaddr_in);

#ifndef NAT_IPV6_ENABLED
	if ( NAT_TRUE == is_ipaddr_present)
    {
        recv_addr.sin_addr.s_addr = inet_addr((const nat_int8_t *)p_ipaddr);
    }
    else
    {
#ifndef NAT_PORT_WINDOWS
        recv_addr.sin_addr.s_addr = INADDR_ANY;
#else
		recv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
#endif
	}
	recv_addr.sin_family = AF_INET;
	recv_addr.sin_port = htons((nat_uint16_t)port_num);
    memset(&(recv_addr.sin_zero), '\0', 8);
#else
	memset((void *)&ai_hints,0,sizeof(ai_hints));
	ai_hints.ai_flags = AI_PASSIVE;
	itoa(port_num,port,10);
	if(getaddrinfo(p_ipaddr, port, &ai_hints,&p_ai_response) != 0)         
	{
		return NAT_RETURN_FAILURE;
    }
	if(p_ai_response == NULL)
	{
		return NAT_RETURN_FAILURE;
	}
	if(NAT_FALSE == is_ipaddr_present && p_ai_response->ai_next != NAT_NULL){
		memcpy((void *)&recv_addr,(void *)(p_ai_response->ai_next->ai_addr),
			p_ai_response->ai_next->ai_addrlen);
		remote_addr_len = p_ai_response->ai_next->ai_addrlen;
	}
	else{
		memcpy((void *)&recv_addr,(void *)(p_ai_response->ai_addr),
			p_ai_response->ai_addrlen);
		remote_addr_len = p_ai_response->ai_addrlen;
	}
	freeaddrinfo(p_ai_response);
	p_ai_response = NULL;
#endif

    if ( NAT_TRANSPORT_TYPE_UDP == transport_type)
    {
        fd = socket(
#ifdef NAT_IPV6_ENABLED
			recv_addr.ss_family,
#else
			AF_INET, 
#endif
			SOCK_DGRAM, 0);
    }
    else
    {
        fd = socket(
#ifdef NAT_IPV6_ENABLED
			recv_addr.ss_family,
#else
			AF_INET, 
#endif
			SOCK_STREAM, 0);
#if defined(NAT_PORT_WINDOWS) || defined(NAT_PORT_WINCE)
	/* for TCP connection */
		if (0 != ioctlsocket(fd, FIONBIO, (unsigned long*)&ioctl_arg))
		{
		    *p_ecode = NAT_ERR_SET_SOCKET;
		    return (NAT_RETURN_FAILURE);
		}
#endif
    }
    if ( 0 >= fd)
    {
        *p_ecode = NAT_ERR_SOCKET_CREATE;
        return (NAT_RETURN_FAILURE);
    }
    if ( -1 == bind( fd,(struct sockaddr*)(&recv_addr), 
		remote_addr_len))
    {
        *p_ecode = NAT_ERR_SOCKET_BIND;
        return (NAT_RETURN_FAILURE);
    }
    if ( - 1 == setsockopt(fd,SOL_SOCKET, SO_REUSEADDR, (char*)(&on), sizeof(int)))
    {
        *p_ecode = NAT_ERR_SET_SOCKET;
        return (NAT_RETURN_FAILURE);
    }
    if (( NAT_TRANSPORT_TYPE_TCP == transport_type) || ( NAT_TRANSPORT_TYPE_TLS == transport_type))
    {
        listen ( fd, NAT_MAX_TCP_CONN);
    }

    *p_fd = (nat_uint32_t)fd;
#endif
    return NAT_RETURN_SUCCESS;
}

/*************************************************************************
*
*  FUNCTION NAME  :   nat_port_udp_send
*
*  DESCRIPTION    :   This function sends the message on UDP channel.
*
*  RETURNS:         Either NAT_RETURN_SUCCESS or NAT_RETURN_FAILURE
*
*************************************************************************/
nat_return_t
nat_port_udp_send(nat_uint32_t    fd,
                     nat_void_t     *p_msg,
                     nat_uint32_t    msg_len,
                     nat_uint32_t    dest_port_num,
                     nat_uint8_t*    dest_ip_addr,
                     nat_error_t    *p_ecode)
{
    nat_uint32_t     bytes_written;
#if (defined(NAT_PORT_SOLARIS) || defined(NAT_PORT_LINUX) || defined(NAT_PORT_WINDOWS) || defined(NAT_PORT_WINCE)|| defined(NAT_PORT_VXWORKS)) 
#ifdef NAT_IPV6_ENABLED
	struct sockaddr_storage remote_addr;
	char port[6] = "\0";
	struct addrinfo ai_hints;
	struct addrinfo *p_ai_response = NULL;
#else
    struct sockaddr_in  remote_addr;
#endif
	nat_uint32_t  remote_addr_len = sizeof(struct sockaddr_in);
#endif
#if (defined(NAT_PORT_SOLARIS) || defined(NAT_PORT_LINUX))|| defined(NAT_PORT_WINDOWS) || defined(NAT_PORT_WINCE)|| defined(NAT_PORT_VXWORKS)
#ifdef NAT_IPV6_ENABLED
	memset((void *)&ai_hints,0,sizeof(ai_hints));
	ai_hints.ai_flags = AI_PASSIVE;
	itoa(dest_port_num,port,10);
	if(getaddrinfo(dest_ip_addr, port, &ai_hints, &p_ai_response) != 0){
		*p_ecode = NAT_ERR_UDP_SEND;
		return NAT_RETURN_FAILURE;
	}
	if(p_ai_response == NULL){
		*p_ecode = NAT_ERR_UDP_SEND;
		return NAT_RETURN_FAILURE;
	}
	memcpy((void *)&remote_addr, (void *)(p_ai_response->ai_addr),
		p_ai_response->ai_addrlen);
	remote_addr_len = p_ai_response->ai_addrlen;
	freeaddrinfo(p_ai_response);
	p_ai_response = NULL;
#else
    remote_addr.sin_family = AF_INET;
	remote_addr.sin_port = htons((nat_uint16_t)dest_port_num);

    remote_addr.sin_addr.s_addr = inet_addr((const nat_int8_t *)dest_ip_addr);
    memset(&(remote_addr.sin_zero),'\0',8);
#endif
    NAT_PRINT(("\nUDP_SEND:Destination IP\t\t%s\n",dest_ip_addr));
	NAT_PRINT(("Destination Port\t\t%d\n",(nat_uint16_t)dest_port_num));
	NAT_PRINT(("Buffer Length\t\t\t%d\n",msg_len));

    bytes_written = sendto (fd, 
            (void*)p_msg,
            msg_len,
            0,
            (struct sockaddr*)&remote_addr,
            remote_addr_len);
    if ( msg_len != bytes_written)
    {
        *p_ecode = NAT_ERR_UDP_SEND;
        return (NAT_RETURN_FAILURE);
    }
#ifdef NAT_SSA_ES_UT
    else
    {
        printf("\nPRINTING THE PDU SENT ON TCP:\n%s", p_msg);
        printf("\n-----------PDU END-----------------\n");
    }
#endif

#endif
    return (NAT_RETURN_SUCCESS);
}

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
nat_return_t
nat_port_tcp_send(nat_uint32_t    fd,
                     nat_void_t     *p_msg,
                     nat_uint32_t    msg_len,
                     nat_error_t    *p_ecode)
{
    nat_int32_t      num = 0; 
    nat_uint32_t     bytes_written = 0;

#if (defined(NAT_PORT_SOLARIS) || defined(NAT_PORT_LINUX) || defined(NAT_PORT_WINDOWS) || defined(NAT_PORT_WINCE)|| defined(NAT_PORT_VXWORKS))
    while ( bytes_written < msg_len)
    {
        num = send (fd, 
                (char*)p_msg + bytes_written, msg_len - bytes_written,0);
        if ( NAT_NULL >= num)
        {
            *p_ecode = NAT_ERR_TCP_SEND;
            return (NAT_RETURN_FAILURE);
        }
        else
        {
            bytes_written += num;
        }
    }
#endif
    return (NAT_RETURN_SUCCESS);
}

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
nat_return_t
nat_port_udp_recv(nat_interface_info_st  *p_interface_info,
                     nat_void_t     *p_msg,
                     nat_error_t    *p_ecode)
{
#if (defined(NAT_PORT_SOLARIS) || defined(NAT_PORT_LINUX))|| (defined(NAT_PORT_WINDOWS) || defined(NAT_PORT_WINCE)|| defined(NAT_PORT_VXWORKS))
    nat_int32_t      num_of_bytes_read = 0;
#ifndef NAT_IPV6_ENABLED
    struct sockaddr_in  source_addr;
    size_t              source_addr_len = sizeof (struct sockaddr_in);
#else
	struct sockaddr_storage source_addr;
	size_t					source_addr_len = sizeof(struct sockaddr_storage);
#endif
    num_of_bytes_read = recvfrom (p_interface_info->fd_id, p_msg, NAT_MAX_UDP_MSG_LNGTH, 0,\
            (struct sockaddr*)(&source_addr), (int*)&source_addr_len);
    if ( -1 == num_of_bytes_read)
    {
        *p_ecode = NAT_ERR_UDP_RECV;
        return (NAT_RETURN_FAILURE);
    }
#ifdef NAT_IPV6_ENABLED
	nat_port_ntop(&source_addr,p_interface_info->dest_ip);
	if(source_addr.ss_family == AF_INET6){
		p_interface_info->dest_port= 
			ntohs(((struct sockaddr_in6 *)&source_addr)->sin6_port);
	}
	else{
		p_interface_info->dest_port= 
			ntohs(((struct sockaddr_in *)&source_addr)->sin_port);
	}
#else
    nat_port_strcpy((nat_uint8_t *)p_interface_info->dest_ip,
            (nat_uint8_t *)inet_ntoa(source_addr.sin_addr));
	  p_interface_info->dest_port = ntohs(source_addr.sin_port);
#endif
    p_interface_info->msg_len = num_of_bytes_read;
    ((nat_uint8_t*)p_msg)[num_of_bytes_read] = '\0';
#endif
 	NAT_PRINT(("\nUDP_RECV:Destination IP\t%s\n",p_interface_info->dest_ip));
	NAT_PRINT(("Destination Port\t\t%d\n",(nat_uint16_t)p_interface_info->dest_port));
	NAT_PRINT(("Buffer Length\t\t\t%d\n",p_interface_info->msg_len));
    return (NAT_RETURN_SUCCESS);
}


/*************************************************************************
*
*  FUNCTION NAME  :   nat_port_close_socket
*
*  DESCRIPTION    :   This function is used to close udp/tcp socket
*
*  RETURNS:         Either NAT_RETURN_SUCCESS or NAT_RETURN_FAILURE
*************************************************************************/
nat_void_t
nat_port_close_socket(
        nat_uint32_t             fd,
        nat_error_t              *p_ecode)
{

#if defined(NAT_PORT_LINUX) || defined(NAT_PORT_SOLARIS)|| defined(NAT_PORT_VXWORKS)
    close(fd);
#endif
#if defined(NAT_PORT_WINDOWS) || defined(NAT_PORT_WINCE)
        closesocket(fd);
#endif
    p_ecode = p_ecode;
}


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
nat_return_t nat_port_recv_udp_win(nat_global_port_info_st *p_glb_port_data,
						 nat_uint16_t			fd_id,
						 nat_msg_list_st			*p_nat_msg_list,
						 nat_error_t				*p_ecode)
{
	
    nat_int32_t	 len_of_msg_read, sock_err;
	nat_int8_t	 buffer[NAT_BLK_SIZE_MAX];
    nat_uint16_t		payload_length;
	nat_void_t		*p_pa_msg;
#ifdef NAT_PORT_WINDOWS
	p_glb_port_data = p_glb_port_data;
#endif 

	

	len_of_msg_read = recv (fd_id, buffer, NAT_BLK_SIZE_MAX, 0);
	sock_err = WSAGetLastError();

	if(0 >= len_of_msg_read)
    {
        return (NAT_RETURN_FAILURE);
    }
	
	/* get the payload length */
	payload_length = (nat_uint16_t)(((nat_msg_hdr_st*)buffer)->api_length);
    
	p_pa_msg = nat_port_static_memget(payload_length,p_ecode);
	
	if( NAT_NULL != p_pa_msg)
	{
		nat_port_memcpy(p_pa_msg, buffer, payload_length);

		p_nat_msg_list->p_msg = (nat_msg_st *)p_pa_msg;
		p_nat_msg_list->p_msg_next = NAT_NULL;
	}
	return NAT_RETURN_SUCCESS;
}

#endif
/*************************************************************************
*  FUNCTION NAME  :   nat_port_close_fd
*  DESCRIPTION    :   Wrapper for close system call
*************************************************************************/
nat_void_t
nat_port_close_fd(nat_uint32_t    fd)
{
#if (defined(NAT_PORT_SOLARIS) || defined(NAT_PORT_LINUX)|| defined(NAT_PORT_VXWORKS))
    NAT_PRINT(("\n[Port]:Entered CloseReqForFD:%u\n",fd));
    close(fd);
#endif
#if defined(NAT_PORT_WINDOWS) || defined(NAT_PORT_WINCE)
    NAT_PRINT(("\n[Port]:Entered CloseReqForFD:%u\n",fd));
    closesocket(fd);
#endif

}
#if 0
/*************************************************************************
*  FUNCTION NAME  :   nat_port_connect
*  DESCRIPTION    :   Wrapper for connect system call
*  This routine is invoked by IPPHONETK to make a non-blocking connect
*  to a TCP server. If the connection is accepted immediately, it 
*  returns a success, if the peer sends a PENDING signal, it returns failure
*  with error code set as CONNECT_RESP_AWAITED and in case there is 
*  no TCP server at  peer it return failure wo this error.
*************************************************************************/
nat_return_t
nat_port_nonblk_connect(
        INOUT  nat_uint16_t  fd,
        IN     nat_uint16_t  dest_port_num,
        IN     nat_uint8_t   *p_dest_ip,
        IN     nat_uint16_t  self_prot_port,
        IN     nat_uint8_t   *p_self_ip,
        OUT    nat_error_t   *p_ecode
        )
{
#if defined(NAT_PORT_WINDOWS) || defined(NAT_PORT_WINCE)
	nat_uint32_t		error =0 ;
#endif

#if (defined(NAT_PORT_SOLARIS) || defined(NAT_PORT_LINUX) || defined(NAT_PORT_WINDOWS) || defined(NAT_PORT_WINCE))
#ifdef NAT_IPV6_ENABLED
	struct sockaddr_storage remote_addr;
	char port[6] = "\0";
	struct addrinfo *p_ai_response = NULL;
#else
    struct sockaddr_in  remote_addr;
#endif
    size_t  remote_addr_len = sizeof(struct sockaddr_in);
#endif

#if (defined(NAT_PORT_SOLARIS) || defined(NAT_PORT_LINUX))
	int errno;
#endif

#if (defined(NAT_PORT_SOLARIS) || defined(NAT_PORT_LINUX) || defined(NAT_PORT_WINDOWS) || defined(NAT_PORT_WINCE))
    /* if the self_port is not null, then we need to bind to that port, 
     * this is a case , when the SA has been established and so we need 
     * to bind to this specific port instead of the ephemeral port
     */
    if (NAT_NULL != self_prot_port)
    {
        if(NAT_RETURN_FAILURE == nat_port_bind(fd,
            p_self_ip,
            self_prot_port,
            p_ecode))
        {
            NAT_PRINT(("\n[PORT]: Failed to bind to self protected port"));
            return NAT_RETURN_FAILURE;
        }
    }

#ifdef NAT_IPV6_ENABLED
	itoa(dest_port_num,port,10);
	if(getaddrinfo(p_dest_ip, port, NULL, &p_ai_response) != 0){
		NAT_PRINT(("\nFailure in getting address information"));
		return NAT_RETURN_FAILURE;
	}
	if(p_ai_response == NULL){
		NAT_PRINT(("\nFailure in getting address information"));
		return NAT_RETURN_FAILURE;
	}
	memcpy((void *)&remote_addr, (void *)(p_ai_response->ai_addr), 
		p_ai_response->ai_addrlen);
	remote_addr_len = p_ai_response->ai_addrlen;
	freeaddrinfo(p_ai_response);
	p_ai_response = NULL;
#else
    remote_addr.sin_family = AF_INET;
    remote_addr.sin_port = htons(dest_port_num);
    remote_addr.sin_addr.s_addr = inet_addr(p_dest_ip);
    memset(&(remote_addr.sin_zero),'\0',8);
#endif
#ifndef NAT_PORT_WINDOWS
    if (fcntl(fd, F_SETFL, 
                fcntl(fd, F_GETFL) | O_NONBLOCK)<0)
    {
        NAT_PRINT(("\nFailure in FCNTL for making the socket nonblocking"));
        return NAT_RETURN_FAILURE;
    }
#endif
    if ( 0 > connect(fd, 
                (struct sockaddr *)&remote_addr, 
				remote_addr_len))
    {
#if defined(NAT_PORT_WINDOWS) || defined(NAT_PORT_WINCE)
		error = WSAGetLastError();
#endif
        NAT_PRINT(("\nFailure in nonblocking connect call"));

#if defined(NAT_PORT_SOLARIS) || defined(NAT_PORT_LINUX)
        if ( EINPROGRESS == errno)
#elif defined(NAT_PORT_WINDOWS) || defined(NAT_PORT_WINCE)
		//error_ret = WSAGetLastError();
		if (WSAEWOULDBLOCK == error)
#endif
        {
            NAT_PRINT(("\nNonBlockingConnectResponseAwaited"));
            *p_ecode = NAT_ERR_CONNECT_RESP_PENDING;
            return NAT_RETURN_FAILURE;
        }
        else
        {
		    NAT_PRINT(("\nReturningFailure!!"));
            return NAT_RETURN_FAILURE;
        }
    }
    return NAT_RETURN_SUCCESS;
#endif
}

/*************************************************************************
*  FUNCTION NAME  :   nat_port_blk_connect
*  DESCRIPTION    :   Wrapper for connect system call
*  This routine is invoked by IPPHONETK to make a blocking connect
*  to a TCP server.
*************************************************************************/
nat_return_t
nat_port_blk_connect(
        INOUT  nat_uint16_t  fd,
        IN     nat_uint16_t  dest_port_num,
        IN     nat_uint8_t   *p_dest_ip,
        IN     nat_uint16_t  self_prot_port,
        IN     nat_uint8_t   *p_self_ip,
        OUT    nat_error_t   *p_ecode
        )
{
	nat_uint32_t		error =0 ;

#if (defined(NAT_PORT_SOLARIS) || defined(NAT_PORT_LINUX) || defined(NAT_PORT_WINDOWS) || defined(NAT_PORT_WINCE))
#ifdef NAT_IPV6_ENABLED
	struct sockaddr_storage remote_addr;
	char port[6] = "\0";
	struct addrinfo *p_ai_response = NULL;
#else
    struct sockaddr_in  remote_addr;
#endif
    size_t  remote_addr_len = sizeof(struct sockaddr_in);
#endif

#if (defined(NAT_PORT_SOLARIS) || defined(NAT_PORT_LINUX))
	int errno;
#endif

#if (defined(NAT_PORT_SOLARIS) || defined(NAT_PORT_LINUX) || defined(NAT_PORT_WINDOWS) || defined(NAT_PORT_WINCE))
    /* if the self_port is not null, then we need to bind to that port, 
     * this is a case , when the SA has been established and so we need 
     * to bind to this specific port instead of the ephemeral port
     */
    if (NAT_NULL != self_prot_port)
    {
        if(NAT_RETURN_FAILURE == nat_port_bind(fd,
            p_self_ip,
            self_prot_port,
            p_ecode))
        {
            NAT_PRINT(("\n[PORT]: Failed to bind to self protected port"));
            return NAT_RETURN_FAILURE;
        }
    }

#ifdef NAT_IPV6_ENABLED
	itoa(dest_port_num,port,10);
	if(getaddrinfo(p_dest_ip, port, NULL, &p_ai_response) != 0){
		NAT_PRINT(("\nFailure in getting address information"));
		return NAT_RETURN_FAILURE;
	}
	if(p_ai_response == NULL){
		NAT_PRINT(("\nFailure in getting address information"));
		return NAT_RETURN_FAILURE;
	}
	memcpy((void *)&remote_addr, (void *)(p_ai_response->ai_addr), 
		p_ai_response->ai_addrlen);
	remote_addr_len = p_ai_response->ai_addrlen;
	freeaddrinfo(p_ai_response);
	p_ai_response = NULL;
#else
    remote_addr.sin_family = AF_INET;
    remote_addr.sin_port = htons(dest_port_num);
    remote_addr.sin_addr.s_addr = inet_addr(p_dest_ip);
    memset(&(remote_addr.sin_zero),'\0',8);
#endif
    if ( 0 > connect(fd, 
                (struct sockaddr *)&remote_addr, 
				remote_addr_len))
    {
        NAT_PRINT(("\nFailure in blocking connect call"));

	    NAT_PRINT(("\nReturningFailure!!"));
        return NAT_RETURN_FAILURE;
    }
    return NAT_RETURN_SUCCESS;
#endif
}

#endif
/*************************************************************************
*  FUNCTION NAME  :   nat_port_bind
*  DESCRIPTION    :   Wrapper for bind system call. This would primarily be
*  					  required incase of IPSec
*************************************************************************/
nat_return_t
nat_port_bind(INOUT  nat_uint16_t  fd,
 		IN	   nat_uint8_t	*p_self_ip,
		IN	   nat_uint16_t	self_port_num,
        OUT    nat_error_t   *p_ecode
        )
{
	/* local variables */
    nat_return_t     ret_val = NAT_RETURN_SUCCESS;

#ifdef NAT_IPV6_ENABLED
	
	char port[6] = "\0";
	struct addrinfo *p_ai_response = NULL;
	/* need to bind the connection with the self IP and port for IPSEC */
	struct sockaddr_storage self_addr;

#else /* if IPV6 is compiled off */

    /* need to bind the connection with the self IP and port for IPSEC */
	struct sockaddr_in	self_addr;
#endif

	size_t  self_addr_len = sizeof(struct sockaddr_in);


#ifdef NAT_IPV6_ENABLED
	
	itoa(self_port_num,port,10);
	if (p_self_ip) /* this is to ensure that IPSEC is configured in the system 
		            * at init-time (read frm NAT.cfg */
	{
		if(getaddrinfo(p_self_ip, port, NULL, &p_ai_response) != 0)
		{
			NAT_PRINT(("\nFailure in getting address information"));
			return NAT_RETURN_FAILURE;
		}
		if(p_ai_response == NULL)
		{
			NAT_PRINT(("\nFailure in getting address information"));
			return NAT_RETURN_FAILURE;
		}
		memcpy((void *)&self_addr, (void *)(p_ai_response->ai_addr), 
			p_ai_response->ai_addrlen);
		self_addr_len = p_ai_response->ai_addrlen;
		freeaddrinfo(p_ai_response);
		p_ai_response = NULL;
	}

#else /* else if only IPv4 address is supported */
  
	/* need to set the self_addr struct */
	/* need to make sure that IPSEC is configured at init time */
	if (p_self_ip) 
	{
		self_addr.sin_family = AF_INET;
		self_addr.sin_port = htons(self_port_num);
		self_addr.sin_addr.s_addr = inet_addr((const nat_int8_t *)p_self_ip);
		memset(&(self_addr.sin_zero),'\0',8);
	}
#endif


	/* bind to self address if IPSEC is configured at init time */
	/* thus when the connection is  made it is made with this port and
	 * not the ephemeral port 
	 */
	if ( -1 == bind( fd,(struct sockaddr*)(&self_addr), 
		self_addr_len))
    {
		NAT_PRINT(("\nFailure in BIND for self addr if IPsec is enabled"));
        *p_ecode = NAT_ERR_SOCKET_BIND;
        return (NAT_RETURN_FAILURE);
    }


	return ret_val;
}


/*************************************************************************
*  FUNCTION NAME  :   nat_port_isconnected
*  DESCRIPTION    :   Wrapper for getpeername system call
*************************************************************************/
nat_boolean_t
nat_port_isconnected(
        IN      nat_uint32_t         fd)
{
    nat_boolean_t    result = NAT_TRUE;
#if (defined(NAT_PORT_SOLARIS) || defined(NAT_PORT_LINUX) || defined(NAT_PORT_WINDOWS) || defined(NAT_PORT_WINCE)|| defined(NAT_PORT_VXWORKS))
#ifndef NAT_IPV6_ENABLED
    struct sockaddr_in  address;
    size_t addr_len = sizeof (struct sockaddr_in);
#else
	struct sockaddr_storage address;
	size_t addr_len = sizeof(struct sockaddr_storage);
#endif
    if (0 != getpeername(fd, (struct sockaddr*)&address,(int*) &addr_len))
    {
        result = NAT_RETURN_FAILURE;
    }
#endif
    return result;
}
/*************************************************************************
*  FUNCTION NAME  :   nat_port_accept
*  DESCRIPTION    :   Wrapper for accept system call
*************************************************************************/
nat_return_t
nat_port_accept(
        IN      nat_uint32_t         server_fd,
        INOUT   nat_uint32_t     *p_fd,
        INOUT   nat_uint8_t      *p_peer_ip,
        INOUT   nat_uint16_t     *p_peer_port)
{
    nat_return_t     ret_val = NAT_RETURN_SUCCESS;
    nat_int16_t      fd;
#if (defined(NAT_PORT_SOLARIS) || defined(NAT_PORT_LINUX) || defined(NAT_PORT_WINDOWS) || defined(NAT_PORT_WINCE)|| defined(NAT_PORT_VXWORKS))
#ifdef NAT_IPV6_ENABLED
	struct sockaddr_storage source_addr;
    size_t source_addr_len = sizeof (struct sockaddr_storage);
#else
    struct sockaddr_in  source_addr;
    size_t              source_addr_len = sizeof (struct sockaddr_in);
#endif

    fd = (nat_int16_t)accept ( server_fd, (struct sockaddr*)&source_addr,
            (int*)&source_addr_len);
    if ( -1 == fd)
    {
        ret_val = NAT_RETURN_FAILURE;
    }
    else
    {
#ifdef NAT_IPV6_ENABLED
		nat_port_ntop(&source_addr,p_peer_ip);
		if(source_addr.ss_family == AF_INET6){
			*p_peer_port = ntohs(((struct sockaddr_in6 *)&source_addr)->sin6_port);
		}
		else{
			*p_peer_port = ntohs(((struct sockaddr_in *)&source_addr)->sin_port);
		}
#else
        nat_port_strcpy((nat_uint8_t *)p_peer_ip, (nat_uint8_t *)inet_ntoa(source_addr.sin_addr));
        *p_peer_port = ntohs(source_addr.sin_port);
#endif
        *p_fd = (nat_uint32_t)fd;
        NAT_PRINT(("\n[PORT]:IncomingConnAccepted FD:%u",fd));
    }
#endif
    return ret_val;
}

/*************************************************************************
*  FUNCTION NAME  :   nat_port_tcp_recv
*  DESCRIPTION    :   This function reads the given number of bytes from 
*  given TCP conn fd
*  RETURNS:         Either NAT_RETURN_SUCCESS or NAT_RETURN_FAILURE
*************************************************************************/
nat_return_t
nat_port_tcp_recv(nat_uint16_t    fd,
        nat_void_t     *p_msg,
        nat_uint32_t  msg_len)
{
#if (defined(NAT_PORT_SOLARIS) || defined(NAT_PORT_LINUX) || defined(NAT_PORT_WINDOWS) || defined(NAT_PORT_WINCE)|| defined(NAT_PORT_VXWORKS))
    nat_int32_t      num_of_bytes_read = 0;

    num_of_bytes_read = recv (fd, p_msg, msg_len, 0);
    if ( -1 == num_of_bytes_read)
    {
        return (NAT_RETURN_FAILURE);
    }
    if ( (nat_uint32_t)num_of_bytes_read != msg_len)
    {
        return (NAT_RETURN_FAILURE);
    }
    NAT_PRINT(("\n[PORT]:ReadFromTCPConn FD:%u\n",fd));
#endif
    return (NAT_RETURN_SUCCESS);
}

/*************************************************************************
*  FUNCTION NAME  :   nat_port_tcp_peek
*  DESCRIPTION    :   This function peeks a message from tcp socket
*  RETURNS:         Either NAT_RETURN_SUCCESS or NAT_RETURN_FAILURE
*************************************************************************/
nat_return_t
nat_port_tcp_peek(nat_interface_info_st  *p_interface_info,
                     nat_void_t     *p_msg,
                     nat_error_t    *p_ecode)
{
#if (defined(NAT_PORT_SOLARIS) || defined(NAT_PORT_LINUX) || defined(NAT_PORT_WINDOWS) || defined(NAT_PORT_WINCE)|| defined(NAT_PORT_VXWORKS))
    nat_int32_t      num_of_bytes_read = 0;

    p_ecode = p_ecode;
    num_of_bytes_read = recv (p_interface_info->fd_id, p_msg, 
            NAT_MAX_TCP_MSG_LNGTH, MSG_PEEK);
    if ( -1 == num_of_bytes_read)
    {
        return (NAT_RETURN_FAILURE);
    }

    p_interface_info->msg_len = num_of_bytes_read;
    ((nat_uint8_t*)p_msg)[num_of_bytes_read] = '\0';

    NAT_PRINT(("\n---TCP MSG RECVD OF LNGTH: %d-FD:%u---\n",num_of_bytes_read,p_interface_info->fd_id));
#ifndef NAT_SIGCOMP_SUPPORT
    NAT_PRINT(("-----------------------------------------------------\n"));
    NAT_PRINT(("%s",(nat_uint8_t *)p_msg));
    NAT_PRINT(("\n----------------------------------------------------\n"));
#endif
    
#endif
    return (NAT_RETURN_SUCCESS);
}

/*************************************************************************
*  FUNCTION NAME  :   nat_port_gethostbyname
*  DESCRIPTION    :   Wrapper for gethostbyname.p_p_addr is the address
*           of memory allocated by calling routine for storing the IP
*  RETURNS:         Either NAT_RETURN_SUCCESS or NAT_RETURN_FAILURE
*************************************************************************/
nat_return_t     nat_port_gethostbyname(
        IN  nat_uint8_t*     p_hostname,
        INOUT   nat_uint8_t*    p_addr)
{
    nat_return_t     ret_val = NAT_RETURN_SUCCESS;
#if (defined(NAT_PORT_SOLARIS) || defined(NAT_PORT_LINUX) || defined(NAT_PORT_WINDOWS) || defined(NAT_PORT_WINCE)|| defined(NAT_PORT_VXWORKS))
    struct hostent  *p_host_ip = NAT_NULL;
    struct in_addr  *inaddrptr = NAT_NULL;
    p_host_ip = gethostbyname((const nat_int8_t *)p_hostname);
    if ( NULL == p_host_ip)
    {
        NAT_PRINT(("\nPORT:Failure in gethostbyname call"));
        ret_val = NAT_RETURN_FAILURE;
    }
    else
    {
        inaddrptr = (struct in_addr *)*p_host_ip->h_addr_list;
        nat_port_strcpy(p_addr,(nat_uint8_t*) inet_ntoa(*inaddrptr));
    }
#endif
    return ret_val;
}

/*************************************************************************
*
*  FUNCTION NAME  :   nat_port_recvfrom
*
*  DESCRIPTION    :   This function reads a message from udp socket and
*                   returns the data read and the number of bytes read
*
*
*************************************************************************/
nat_int32_t nat_port_recvfrom(nat_int32_t fd, nat_uint8_t* p_msg,
                              nat_ip_port_address_st *p_dest_addr)
{
    nat_int32_t      num_of_bytes_read = 0;
#ifndef NAT_IPV6_ENABLED
    struct sockaddr_in  source_addr;
    nat_int32_t      source_addr_len = sizeof (struct sockaddr);
#else
	struct sockaddr_storage  source_addr;
    nat_int32_t      source_addr_len = sizeof (struct sockaddr_storage);
#endif
    num_of_bytes_read = recvfrom (fd, (nat_int8_t *)p_msg, NAT_MAX_UDP_MSG_LNGTH, 0,
            (struct sockaddr*)(&source_addr), &source_addr_len);
#ifdef NAT_IPV6_ENABLED
#else
    NAT_PRINT(("\nPORT_RECV:Destination IP\t%s\n",inet_ntoa(source_addr.sin_addr)));
	NAT_PRINT(("Destination Port\t\t%d\n",ntohs(source_addr.sin_port)));
	NAT_PRINT(("Buffer Length\t\t\t%d\n",num_of_bytes_read));

    p_dest_addr->port = ntohs(source_addr.sin_port);
    nat_cmn_convert_ip_string_to_ip4_addr(
        (nat_uint8_t *)inet_ntoa(source_addr.sin_addr),
        &(p_dest_addr->ipv4_addr));
    ((nat_uint8_t*)p_msg)[num_of_bytes_read] = '\0';
#endif
    return num_of_bytes_read;

}

