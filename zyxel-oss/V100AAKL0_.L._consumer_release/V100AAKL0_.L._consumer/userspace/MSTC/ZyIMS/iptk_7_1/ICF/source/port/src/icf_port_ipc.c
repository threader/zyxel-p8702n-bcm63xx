/*************************************************************************
 *
 *     FILENAME           :  icf_port_ipc.c
 *
 *     DESCRIPTION        :  This file contains port wrappers for IPC
 *                          mechanisms ( writing to UDP/TCP sockets, pipes,
 *                          reading from the same, opening of channels
 *                          etc.)
 *
 *     Revision History   :
 *         DATE        NAME                REFERENCE            REASON
 *         ----------------------------------------------------------
 *         25/11/04    Pankaj Negi        ICF LLD        Initial Version.
 *         07/01/05    Aashish Suchdev                      replaced retval
 *         10/01/05    Aashish Suchdev                      Mayank's comment
 *								Incorporated
 *	   05/06/05	Vidyut Gupta	                   Replaced the pipes
 *							   with sockets to port
 *							   onto Windows, Also 
 *							   changed the include
 *							   files .
 *         10-Jan-2006  Aman Arora         IPSEC           add functions for IPSEC
 *         30-Jun-2006  Umang Singh        Rel 5.2         TLS support
 * 	       22-Aug-2007	Deepti Goyal	   ICF 6.4.1       DNS/TCP fix.
 * 	       04-Aug-2008  Amit Sikka         SPR 18100       VX Warnings removal
 *         25-Jun-2008  Rajiv Kumar        SPR - 18722     IPTK not compilable with gcc
 *                                                         version 4.2.3  	       
 *         12-Dec-2008  Vinay Jindal       SPR 19234       Merge SPR 19199 fix
 *
 *     Copyright 2008, Aricent.
 *
 *************************************************************************/
#include "icf_common_types.h"
#include "icf_common_prototypes.h"
#include "icf_port_macros.h"
#include "icf_port_prototypes.h"
#include "icf_port_intf_prototypes.h"
#include "icf_ecode.h"
#include "icf_port_defs.h"

#if (defined(ICF_PORT_SOLARIS) || defined(ICF_PORT_LINUX))
#include <fcntl.h>
#include <sys/un.h>
#include<sys/types.h>
#include<sys/socket.h>
#include <sys/errno.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <signal.h>

#include <stdlib.h>

#endif

#if defined(ICF_PORT_WINDOWS) 
#include<winsock2.h>
#include<windows.h>
#endif
#if defined(ICF_PORT_WINDOWS) || defined(ICF_PORT_LINUX)|| defined(ICF_PORT_VXWORKS)
#ifdef ICF_SECURE_SUPPORT
#include "openssl/x509.h"
#include "openssl/dh.h"
#include "openssl/pem.h"
#include "openssl/ssl.h"
#include "openssl/rand.h"
#endif
#endif 

#include <time.h>

#ifndef ICF_PORT_WINDOWS
#include <sys/stat.h>
#endif


#include <ctype.h>

extern icf_glb_pdb_st        *p_persistent_glb_pdb;

extern icf_int32_t icf_port_recvfrom(icf_int32_t fd, icf_uint8_t* p_msg);

/*extern int errno;*/

/*************************************************************************
*
*  FUNCTION NAME  :   icf_port_flush_fifo
*
*  DESCRIPTION    :   This function is used to read and dump all the data
*                   pending on the FIFO correspondig to the FD
*
*  RETURNS:         Either ICF_SUCCESS or ICF_FAILURE
*************************************************************************/
icf_return_t
icf_port_flush(
        icf_uint32_t             fd,
        icf_uint32_t             msg_len,
        icf_error_t              *p_ecode)
{
    icf_return_t     ret_val = ICF_SUCCESS;
#ifndef ICF_PORT_WINDOWS
    icf_fd_set       readfdset;
#endif

#if defined(ICF_PORT_LINUX) || defined(ICF_PORT_SOLARIS) || defined(ICF_PORT_VXWORKS)
    icf_uint8_t      *p_msg = ICF_NULL;
#endif 


#if defined(ICF_PORT_WINDOWS)
	fd = fd;
	msg_len = msg_len;
#endif

#if defined(ICF_PORT_LINUX) || defined(ICF_PORT_SOLARIS) || defined(ICF_PORT_VXWORKS)
    struct timeval      timeout;

    timeout.tv_sec  = 0;
    timeout.tv_usec = 0;

    p_msg = (icf_uint8_t*)malloc(msg_len); 
    ICF_PORT_FD_ZERO( &readfdset);
    ICF_PORT_FD_SET(fd, &readfdset);
    while ( ICF_NULL < 
            select(FD_SETSIZE,&readfdset,ICF_NULL,ICF_NULL,&timeout))
    {
        read(fd, p_msg, msg_len);
        ICF_PORT_FD_SET(fd, &readfdset);
    }
    free(p_msg);
#endif

    p_ecode = p_ecode;

    return ret_val;
}
/*************************************************************************
*
*  FUNCTION NAME  :   icf_port_select
*
*  DESCRIPTION    :   This function invokes the system call select
*
*  RETURNS:         Either ICF_SUCCESS or ICF_FAILURE
*               timeout_val is in microseconds.
*************************************************************************/
icf_return_t
icf_port_select(icf_fd_set    *p_read_fdset,
        icf_fd_set       *p_write_fdset,
		icf_fd_set		*p_except_fdset,
        icf_uint32_t     timeout_val,
        icf_error_t      *p_ecode)
{
    icf_return_t     ret_val = ICF_SUCCESS;
    int                 retn = 0;
#ifdef ICF_PORT_WINDOWS
    int                 err;
#endif

#if defined(ICF_PORT_LINUX) || defined(ICF_PORT_SOLARIS) || defined(ICF_PORT_WINDOWS) || defined(ICF_PORT_VXWORKS)
    struct timeval      timeout;
#endif

    p_ecode = p_ecode;
    p_write_fdset = p_write_fdset;
    p_read_fdset = p_read_fdset;
    timeout_val = timeout_val;

#if defined(ICF_PORT_LINUX) || defined(ICF_PORT_SOLARIS) || defined (ICF_PORT_WINDOWS) || defined(ICF_PORT_VXWORKS)
    timeout.tv_sec  = 0;
    timeout.tv_usec  = timeout_val*1000;
    if ( retn == select(FD_SETSIZE, p_read_fdset,
                p_write_fdset, p_except_fdset, &timeout))
    {
        /*p_ecode = ICF_SELECT_FAILURE;*/
        ret_val = ICF_FAILURE;
    }
#endif
#ifdef ICF_PORT_WINDOWS
    err = WSAGetLastError();
#endif
    return ret_val;
}

/*************************************************************************
*
*  FUNCTION NAME  :   icf_port_create_fifo
*
*  DESCRIPTION    :   This function creates the named fifo with the given
*			 name string.
*
*  RETURNS:         Either ICF_SUCCESS or ICF_FAILURE
*
*************************************************************************/
icf_return_t
icf_port_create_fifo(icf_uint8_t		*p_fifo_name,
        icf_uint32_t    permssions,
        icf_error_t    *p_ecode)
{
p_fifo_name = p_fifo_name;
permssions = permssions;
p_ecode = p_ecode;
#if (defined(ICF_PORT_SOLARIS) || defined(ICF_PORT_LINUX))
    /*Fix done for SPR-18722 : CSR-1-6068965*/
    if (0 != access((icf_int8_t *)p_fifo_name,F_OK))
    {
        if (0 != (mkfifo((icf_int8_t *)p_fifo_name,permssions)))
        {
            *p_ecode = ICF_ERROR_FIFO_CREATION;
            return (ICF_FAILURE);
        }
    }
    else
    {
    }
#endif
    return ICF_SUCCESS;
}

/*************************************************************************
*
*  FUNCTION NAME  :   icf_port_open_fifo
*
*  DESCRIPTION    :   This function opens the named fifo with the given set
*                   of permissions
*
*  RETURNS:         Either ICF_SUCCESS or ICF_FAILURE
*
*************************************************************************/
icf_return_t
icf_port_open_fifo(icf_uint8_t		*p_fifo_name,
        icf_uint32_t    *p_fd,
        icf_uint32_t    permissions,
        icf_error_t    *p_ecode)
{
p_fifo_name = p_fifo_name;
p_fd = p_fd;
permissions = permissions;
p_ecode = p_ecode;
#if (defined(ICF_PORT_SOLARIS) || defined(ICF_PORT_LINUX))
    icf_int32_t  fd;
	/*Fix done for SPR-18722 : CSR-1-6068965*/
    if ( -1 == (fd = open((icf_int8_t *)p_fifo_name, permissions)))
    {
        *p_ecode = ICF_ERROR_FIFO_CREATION;
        return (ICF_FAILURE);
    }
    *p_fd = (icf_uint32_t)fd;
#endif
    return ICF_SUCCESS;
}

/*************************************************************************
*
*  FUNCTION NAME  :   icf_port_open_socket
*
*  DESCRIPTION    :   This function opens a TCP/UDP 
*                       socket. This is a subset of the next routine
*                       in the sense that it does not bind to a port num
*                       currentlt this will be used by SSA for opening
*                       tcp client connection
*
*  RETURNS:         Either ICF_SUCCESS or ICF_FAILURE
*************************************************************************/
icf_return_t
icf_port_open_socket(
        /*ICF_TRANSPORT_TYPE_UDP/ICF_TRANSPORT_TYPE_TCP*/
#ifdef ICF_IPV6_ENABLED
		icf_uint32_t		proto_family,
#endif
        icf_uint8_t      transport_type,
        icf_uint32_t     *p_fd,
        icf_error_t      *p_ecode)
{
#if defined(ICF_PORT_SOLARIS) || defined(ICF_PORT_LINUX) || defined(ICF_PORT_WINDOWS) || defined(ICF_PORT_VXWORKS)
    int                 on = 1;
    icf_int32_t      fd;
#ifdef ICF_PORT_WINDOWS
	/*icf_uint32_t		ioctl_arg =1; */
	unsigned long		ioctl_arg =1;
#endif

    if ( ICF_TRANSPORT_TYPE_UDP == transport_type)
    {
        fd = socket(
#ifdef ICF_IPV6_ENABLED
			proto_family,
#else
			AF_INET, 
#endif
			SOCK_DGRAM, 0);
    }
    else
    {
        fd = socket(
#ifdef ICF_IPV6_ENABLED
			proto_family,
#else
			AF_INET, 
#endif
			SOCK_STREAM, 0);
    }
    if ( 0 > fd)
    {
        *p_ecode = ICF_ERROR_SOCKET_CREATE;
        return (ICF_FAILURE);
    }
#ifdef ICF_PORT_WINDOWS
	/* for TCP connection */
	if (0 != ioctlsocket(fd, FIONBIO, &ioctl_arg))
    {
        *p_ecode = ICF_ERROR_SET_SOCKET;
        return (ICF_FAILURE);
    }
#endif

    if ( - 1 == setsockopt(fd,SOL_SOCKET, SO_REUSEADDR, (char*)(&on), 
			sizeof(int)))
    {
        *p_ecode = ICF_ERROR_SET_SOCKET;
        return (ICF_FAILURE);
    }
    *p_fd = fd;
#endif
    return ICF_SUCCESS;
}

/*************************************************************************
*
*  FUNCTION NAME  :   icf_port_open_channel
*
*  DESCRIPTION    :   This function opens the TCP/UDP n/w channel
*                       ->It allocates a fd from system
*                       ->It binds the fd to the port specified
*                       For TCP,it will be the server port
*                       Note: This assumes IPv4 only
*
*  RETURNS:         Either ICF_SUCCESS or ICF_FAILURE
*
*************************************************************************/
icf_return_t
icf_port_open_channel(icf_uint32_t    port_num, 

        /*ICF_TRANSPORT_TYPE_UDP/ICF_TRANSPORT_TYPE_TCP*/
        icf_uint8_t      transport_type,

        /*TRUE->Tk is passing a dotted IP address
         * FALSE->port should use IPADDR_ANY
         */
        icf_boolean_t    is_ipaddr_present,
        icf_uint8_t      *p_ipaddr,

        icf_uint32_t     *p_fd,
        icf_error_t      *p_ecode)
{
       icf_uint32_t     send_buf_size = ICF_MAX_SEND_BUFF_SIZE;
       icf_uint32_t     recv_buf_size = ICF_MAX_RECV_BUFF_SIZE;

#if (defined(ICF_PORT_SOLARIS) || defined(ICF_PORT_LINUX))|| defined(ICF_PORT_WINDOWS) || defined(ICF_PORT_VXWORKS)
    icf_int32_t     fd;
    int                 on = 1;
#ifdef ICF_IPV6_ENABLED
	struct sockaddr_storage recv_addr;
	char port[6] = "\0";
	struct addrinfo ai_hints;
	struct addrinfo *p_ai_response = NULL;
#else
    struct sockaddr_in  recv_addr;
#endif
#if (defined(ICF_PORT_WINDOWS) || defined(ICF_PORT_VXWORKS))
	icf_uint32_t		ioctl_arg = 1;
	/*unsigned long		ioctl_arg = 1;*/
#endif
    size_t  remote_addr_len = sizeof(struct sockaddr_in);

#ifndef ICF_IPV6_ENABLED
	if ( ICF_TRUE == is_ipaddr_present)
    {
	    /*Fix done for SPR-18722 : CSR-1-6068965*/
        recv_addr.sin_addr.s_addr = inet_addr((icf_int8_t *)p_ipaddr);
    }
    else
    {
#ifndef ICF_PORT_WINDOWS
        recv_addr.sin_addr.s_addr = INADDR_ANY;
#else
		recv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
#endif
	}
	recv_addr.sin_family = AF_INET;
	recv_addr.sin_port = htons((icf_uint16_t)port_num);
    memset(&(recv_addr.sin_zero), '\0', 8);
#else
	memset((void *)&ai_hints,0,sizeof(ai_hints));
	ai_hints.ai_flags = AI_PASSIVE;
	itoa(port_num,port,10);
	if(getaddrinfo(p_ipaddr, port, &ai_hints,&p_ai_response) != 0)         
	{
		return ICF_FAILURE;
    }
	if(p_ai_response == NULL)
	{
		return ICF_FAILURE;
	}
	if(ICF_FALSE == is_ipaddr_present && p_ai_response->ai_next != ICF_NULL){
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

    if ( ICF_TRANSPORT_TYPE_UDP == transport_type)
    {
        fd = socket(
#ifdef ICF_IPV6_ENABLED
			recv_addr.ss_family,
#else
			AF_INET, 
#endif
			SOCK_DGRAM, 0);
    }
    else
    {
        fd = socket(
#ifdef ICF_IPV6_ENABLED
			recv_addr.ss_family,
#else
			AF_INET, 
#endif
			SOCK_STREAM, 0);
#ifdef ICF_PORT_WINDOWS
	/* for TCP connection */
		if (0 != ioctlsocket(fd, FIONBIO, &ioctl_arg))
		{
		    *p_ecode = ICF_ERROR_SET_SOCKET;
		    return (ICF_FAILURE);
		}
#endif

#ifdef ICF_PORT_VXWORKS /* VxWorks porting to make the socket non-blocking */
    ICF_PRINT(("\n[PORT]: VxWorks::Setting fd flags through ioctl for " \
							"sockfd = %d\n", fd));
    ioctl_arg = TRUE;
    if (0 != ioctl(fd, FIONBIO, (int)&ioctl_arg))
    {
        ICF_PRINT(("\n[PORT]: ioctl FAILED in setting socket nonblock flag\n"));
        return ICF_FAILURE;
    }
    else
    {
        ICF_PRINT(("\n[PORT]: ioctl success in making the socket nonblocking"));
    }
#endif /* ifdef ICF_PORT_VXWORKS */

    }
    if ( 0 > fd)
    {
        *p_ecode = ICF_ERROR_SOCKET_CREATE;
        return (ICF_FAILURE);
    }
    if ( - 1 == setsockopt(fd,SOL_SOCKET, SO_REUSEADDR, (char*)(&on), sizeof(int)))
    {
        *p_ecode = ICF_ERROR_SET_SOCKET;
        return (ICF_FAILURE);
    }
    if ( -1 == bind( fd,(struct sockaddr*)(&recv_addr), 
                remote_addr_len))
    {
        *p_ecode = ICF_ERROR_SOCKET_BIND;
        return (ICF_FAILURE);
    }
    if ( - 1 == setsockopt (fd, SOL_SOCKET, SO_SNDBUF, (char *)(&send_buf_size), (socklen_t)sizeof(send_buf_size)))
    {
        *p_ecode = ICF_ERROR_SET_SOCKET;
        return (ICF_FAILURE);
    }
    if ( - 1 == setsockopt (fd, SOL_SOCKET, SO_RCVBUF, (char *)(&recv_buf_size), (socklen_t)sizeof(recv_buf_size)))
    {
        *p_ecode = ICF_ERROR_SET_SOCKET;
        return (ICF_FAILURE);
    }

    if (( ICF_TRANSPORT_TYPE_TCP == transport_type) || ( ICF_TRANSPORT_TYPE_TLS == transport_type))
    {
        listen ( fd, ICF_MAX_NO_OF_PERSISTENT_CONNECTIONS);
    }

    *p_fd = (icf_uint32_t)fd;
     ICF_PRINT(((icf_uint8_t *)"\n[PORT]: icf_port_open_channel created fd = %d for mode = %d\n",
				fd, transport_type));
#endif
    return ICF_SUCCESS;
}


/*************************************************************************
*
*  FUNCTION NAME  :   icf_port_open_ipc_channel
*
*  DESCRIPTION    :   This function opens the TCP/UDP n/w channel
*                       ->It allocates a fd from system
*                       ->It binds the fd to the port specified
*                       For TCP,it will be the server port
*                       Note: This assumes IPv4 only
*
*  RETURNS:         Either ICF_SUCCESS or ICF_FAILURE
*
*************************************************************************/
icf_return_t icf_port_open_ipc_channel(icf_uint32_t *p_fd, icf_error_t *p_ecode, icf_string_st channel)
{
#if (defined(ICF_PORT_SOLARIS) || defined(ICF_PORT_LINUX))|| defined(ICF_PORT_WINDOWS) || defined(ICF_PORT_VXWORKS)
	icf_int32_t fd;
	struct sockaddr_un addr;
#endif

	fd = socket(PF_LOCAL, SOCK_DGRAM, 0);

	if ( 0 > fd) {
		*p_ecode = ICF_ERROR_SOCKET_CREATE;
		return (ICF_FAILURE);
	}

	/*if ( - 1 == setsockopt(fd,SOL_SOCKET, SO_REUSEADDR, (char*)(&on), sizeof(int))) {
		*p_ecode = ICF_ERROR_SET_SOCKET;
		return (ICF_FAILURE);
	}*/

	printf("icf_port_open_ipc_channel path = %s\r\n", channel.str);
	unlink(channel.str);

	bzero(&addr, sizeof(addr));
	addr.sun_family = PF_LOCAL;
	strncpy(addr.sun_path, (char *)&(channel.str[0]), channel.str_len);
	if ( -1 == bind( fd,(struct sockaddr*)(&addr), sizeof(addr))) {
		*p_ecode = ICF_ERROR_SOCKET_BIND;
		return (ICF_FAILURE);
	}

	/*if ( - 1 == setsockopt (fd, SOL_SOCKET, SO_SNDBUF, (char *)(&send_buf_size), (socklen_t)sizeof(send_buf_size))) {
		*p_ecode = ICF_ERROR_SET_SOCKET;
		return (ICF_FAILURE);
	}

	if ( - 1 == setsockopt (fd, SOL_SOCKET, SO_RCVBUF, (char *)(&recv_buf_size), (socklen_t)sizeof(recv_buf_size))) {
		*p_ecode = ICF_ERROR_SET_SOCKET;
		return (ICF_FAILURE);
	}*/

	*p_fd = (icf_uint32_t)fd;
	ICF_PRINT(((icf_uint8_t *)"\n[PORT]: icf_port_open_ipc_channel created fd = %d for channel = %s\n", fd, channel.str));

	return ICF_SUCCESS;
}

/*************************************************************************
*
*  FUNCTION NAME  :   icf_port_udp_send
*
*  DESCRIPTION    :   This function sends the message on UDP channel.
*
*  RETURNS:         Either ICF_SUCCESS or ICF_FAILURE
*
*************************************************************************/
icf_return_t
icf_port_udp_send(icf_uint32_t    fd,
                     icf_void_t     *p_msg,
                     icf_uint32_t    msg_len,
                     icf_uint32_t    dest_port_num,
                     icf_uint8_t*    dest_ip_addr,
                     icf_error_t    *p_ecode)
{
    icf_uint32_t     bytes_written;
#if (defined(ICF_PORT_SOLARIS) || defined(ICF_PORT_LINUX) || defined(ICF_PORT_WINDOWS)) || defined(ICF_PORT_VXWORKS)
#ifdef ICF_IPV6_ENABLED
	struct sockaddr_storage remote_addr;
	char port[6] = "\0";
	struct addrinfo ai_hints;
	struct addrinfo *p_ai_response = NULL;
#else
    struct sockaddr_in  remote_addr;
#endif
	icf_uint32_t  remote_addr_len = sizeof(struct sockaddr_in);
#endif
#if (defined(ICF_PORT_SOLARIS) || defined(ICF_PORT_LINUX))|| defined(ICF_PORT_WINDOWS) || defined(ICF_PORT_VXWORKS)
#ifdef ICF_IPV6_ENABLED
	memset((void *)&ai_hints,0,sizeof(ai_hints));
	ai_hints.ai_flags = AI_PASSIVE;
	itoa(dest_port_num,port,10);
	if(getaddrinfo(dest_ip_addr, port, &ai_hints, &p_ai_response) != 0){
		*p_ecode = ICF_ERROR_UDP_SEND;
		return ICF_FAILURE;
	}
	if(p_ai_response == NULL){
		*p_ecode = ICF_ERROR_UDP_SEND;
		return ICF_FAILURE;
	}
	memcpy((void *)&remote_addr, (void *)(p_ai_response->ai_addr),
		p_ai_response->ai_addrlen);
	remote_addr_len = p_ai_response->ai_addrlen;
	freeaddrinfo(p_ai_response);
	p_ai_response = NULL;
#else
    remote_addr.sin_family = AF_INET;
	remote_addr.sin_port = htons((icf_uint16_t)dest_port_num);

    remote_addr.sin_addr.s_addr = inet_addr((icf_int8_t *)dest_ip_addr);
    memset(&(remote_addr.sin_zero),'\0',8);
#endif

#if 0
    ICF_PRINT(("\nUDP_SEND:Destination IP\t\t%s\n",dest_ip_addr));
	ICF_PRINT(("Destination Port\t\t%d\n",(icf_uint16_t)dest_port_num));
	ICF_PRINT(("Buffer Length\t\t\t%d\n",msg_len));
#endif
    if ((0 == fd) || ((icf_uint32_t)ICF_INVALID_SOCKFD == fd))
    {
#if 0
		ICF_PRINT(("\n[PORT]: Not attempting UDP send on fd 0 or 65535\n"));
        ICF_PRINT(("\n-----------DATA SENT ON UDP FAILED-----------\n"));
#endif
        *p_ecode = ICF_ERROR_UDP_SEND;
		return (ICF_FAILURE);
	}
    bytes_written = sendto (fd, 
            (void*)p_msg,
            msg_len,
            0,
            (struct sockaddr*)&remote_addr,
            remote_addr_len);
    if ( msg_len != bytes_written)
    {
        *p_ecode = ICF_ERROR_UDP_SEND;
        return (ICF_FAILURE);
    }
#ifdef ICF_SSA_ES_UT
    else
    {
        printf("\nPRINTING THE PDU SENT ON UDP:\n%s", p_msg);
        printf("\n-----------PDU END-----------------\n");
    }
#endif

#endif
    return (ICF_SUCCESS);
}

#if 1 /* ZyXEL porting */
icf_return_t icf_port_udp_SetDSCP( icf_uint32_t fd, icf_uint8_t dscp )
{

    return setsockopt( fd, SOL_IP, IP_TOS, &(icf_uint8_t){ dscp }, sizeof (icf_uint8_t));
}
#endif

/* ZyXEL porting */
icf_return_t icf_port_ipc_send(
	icf_uint32_t fd,
	icf_void_t *p_msg,
	icf_uint32_t msg_len,
	icf_error_t *p_ecode,
	icf_string_st channel )
{
	icf_uint32_t bytes_written = 0;
	struct sockaddr_un addr;

	if ((0 == fd) || ((icf_uint32_t)ICF_INVALID_SOCKFD == fd)) {
		*p_ecode = ICF_ERROR_UDP_SEND;
		return (ICF_FAILURE);
	}

	memset(&addr, 0, sizeof(addr));
	addr.sun_family = PF_LOCAL;
	strncpy(addr.sun_path, (char *)&(channel.str[0]), channel.str_len);
	bytes_written = sendto(fd,(icf_void_t*)p_msg,msg_len,0,(struct sockaddr*)&addr,sizeof(struct sockaddr_un));

	if ( msg_len != bytes_written) {
		 *p_ecode = ICF_ERROR_UDP_SEND;
		return (ICF_FAILURE);
	}
	
	return (ICF_SUCCESS);
}

/*************************************************************************
*
*  FUNCTION NAME  :   icf_port_fifo_send
*
*  DESCRIPTION    :   This function sends the message on named pipe/FIFO.
*
*  RETURNS:         Either ICF_SUCCESS or ICF_FAILURE
*
*************************************************************************/
icf_return_t
icf_port_fifo_send(icf_uint32_t    fd,
                     icf_void_t     *p_msg,
                     icf_uint32_t    msg_len,
                     icf_error_t    *p_ecode)
{
#if (defined(ICF_PORT_SOLARIS) || defined(ICF_PORT_LINUX))
	icf_uint32_t     bytes_written;

    bytes_written = write (fd,
            (char*)p_msg,
            msg_len);
    if ( msg_len != bytes_written)
    {
        *p_ecode = ICF_ERROR_FIFO_SEND;
        return (ICF_FAILURE);
    }
#else	
fd = fd;
p_msg = ICF_NULL;
msg_len = msg_len;
*p_ecode = *p_ecode;
#endif
    return (ICF_SUCCESS);
}

/*************************************************************************
*
*  FUNCTION NAME  :   icf_port_tcp_send
*
*  DESCRIPTION    :   This function sends the message on the TCP conn
*                       corresponding to the fd passed.
*
*  RETURNS:         Either ICF_SUCCESS or ICF_FAILURE
*
*************************************************************************/
icf_return_t
icf_port_tcp_send(icf_uint32_t    fd,
                     icf_void_t     *p_msg,
                     icf_uint32_t    msg_len,
                     icf_error_t    *p_ecode)
{
    icf_int32_t      num = 0; 
    icf_uint32_t     bytes_written = 0;
	
	if ((0 == fd) || ((icf_uint32_t)ICF_INVALID_SOCKFD == fd))
	{
#if 0
		ICF_PRINT(("\n[PORT]: Not attempting TCP send on fd 0 or 65535\n"));
        ICF_PRINT(("\n-----------DATA SENT ON TCP FAILED-----------\n"));
#endif
		*p_ecode = ICF_ERROR_TCP_SEND;
		return (ICF_FAILURE);
	}
#if (defined(ICF_PORT_SOLARIS) || defined(ICF_PORT_LINUX) || defined(ICF_PORT_WINDOWS) || defined(ICF_PORT_WINCE)) || defined(ICF_PORT_VXWORKS)
    while ( bytes_written < msg_len)
    {
#if defined(ICF_PORT_VXWORKS) || defined(ICF_PORT_WINDOWS)
        num = send (fd, 
                (char*)p_msg + bytes_written, msg_len - bytes_written,0);
#else
	num = send (fd,
                (char*)p_msg + bytes_written, msg_len - bytes_written,MSG_NOSIGNAL);
#endif

        if ( ICF_NULL >= num)
        {
            *p_ecode = ICF_ERROR_TCP_SEND;

            printf("\n-----------DATA SENT ON TCP FAILED-----    %d\n",fd);

            return (ICF_FAILURE);
        }
        else
        {
            bytes_written += num;
        }
    }
#endif

#ifndef ICF_LOAD_STAT
    printf("\n-----------DATA SENT ON TCP SUCCESS-----------\n");
#endif
    return (ICF_SUCCESS);
}

/*************************************************************************
*
*  FUNCTION NAME  :   icf_port_udp_recv
*
*  DESCRIPTION    :   This function reads a message from udp socket and
*                   returns the data read and the number of bytes read
*
*  RETURNS:         Either ICF_SUCCESS or ICF_FAILURE
*
*************************************************************************/
icf_return_t
icf_port_udp_recv(icf_interface_info_st  *p_interface_info,
                     icf_void_t     *p_msg,
                     icf_error_t    *p_ecode)
{
#if (defined(ICF_PORT_SOLARIS) || defined(ICF_PORT_LINUX))|| defined(ICF_PORT_WINDOWS) || defined(ICF_PORT_VXWORKS)
    icf_int32_t      num_of_bytes_read = 0;
#ifndef ICF_IPV6_ENABLED
    struct sockaddr_in  source_addr;
#ifdef ICF_PORT_VXWORKS
    icf_uint8_t			host_addr_fmt[INET_ADDR_LEN] = "\0"; 
    /* Fix for CSR 1-5028975 */

    icf_int32_t         source_addr_len = sizeof (struct sockaddr_in);
#else
    /*size_t              source_addr_len = sizeof (struct sockaddr_in);*/
	icf_uint32_t			source_addr_len = sizeof (struct sockaddr_in);
#endif
#else
	struct sockaddr_storage source_addr;
	size_t					source_addr_len = sizeof(struct sockaddr_storage);
#endif
	if ((0 == p_interface_info->fd_id) || 
            ((icf_uint32_t)ICF_INVALID_SOCKFD == p_interface_info->fd_id))
    {
#if 0
        ICF_PRINT(("\n[PORT]: Not attempting UDP recv on fd 0 or 65535\n"));
#endif
		return (ICF_FAILURE);
	}

    num_of_bytes_read = recvfrom (p_interface_info->fd_id, p_msg, ICF_MAX_UDP_MSG_LNGTH, 0,\
            (struct sockaddr*)(&source_addr), &source_addr_len);
    if ( -1 == num_of_bytes_read)
    {
        *p_ecode = ICF_ERROR_UDP_RECV;
        return (ICF_FAILURE);
    }
#ifdef ICF_IPV6_ENABLED
	icf_port_ntop(&source_addr,p_interface_info->dest_ip);
	if(source_addr.ss_family == AF_INET6){
		p_interface_info->dest_port= 
			ntohs(((struct sockaddr_in6 *)&source_addr)->sin6_port);
	}
	else{
		p_interface_info->dest_port= 
			ntohs(((struct sockaddr_in *)&source_addr)->sin_port);
	}
#else
    /* Fix for CSR 1-5028975 */
#ifdef ICF_PORT_VXWORKS
	inet_ntoa_b(source_addr.sin_addr, host_addr_fmt);
	icf_port_strcpy(p_interface_info->dest_ip, host_addr_fmt);
#elif defined(ICF_PORT_LINUX)
    icf_port_ntop(&source_addr,p_interface_info->dest_ip);
#else
    icf_port_strcpy((icf_uint8_t *)p_interface_info->dest_ip,
            (icf_uint8_t *)(inet_ntoa(source_addr.sin_addr)));
#endif
	  p_interface_info->dest_port = ntohs(source_addr.sin_port);
#endif
    p_interface_info->msg_len = num_of_bytes_read;
    ((icf_uint8_t*)p_msg)[num_of_bytes_read] = '\0';
#endif
        
 	ICF_PRINT(((icf_uint8_t *)"\nUDP_RECV:Destination IP\t%s\n",p_interface_info->dest_ip));
	ICF_PRINT(((icf_uint8_t *)"Destination Port\t\t%d\n",(icf_uint16_t)p_interface_info->dest_port));
	ICF_PRINT(((icf_uint8_t *)"Buffer Length\t\t\t%d\n",p_interface_info->msg_len));
    return (ICF_SUCCESS);
}

/*************************************************************************
*
*  FUNCTION NAME  :   icf_port_fifo_recv
*
*  DESCRIPTION    :   This function reads a message from  named pipe 
*                    It accepts the calling context to give it the number
*                    of bytes to be read
*
*  RETURNS:         Either ICF_SUCCESS or ICF_FAILURE
*
*************************************************************************/
icf_return_t
icf_port_fifo_recv(icf_uint32_t    fd,
        icf_void_t     *p_msg,
        icf_uint32_t   msg_len,
        icf_error_t    *p_ecode)
{
#if (defined(ICF_PORT_SOLARIS) || defined(ICF_PORT_LINUX))
    icf_uint32_t     num_of_bytes_read = 0;
    num_of_bytes_read = read(fd, p_msg, msg_len);
    if ( msg_len != num_of_bytes_read)
    {
        *p_ecode = ICF_ERROR_FIFO_RECV;
        return (ICF_FAILURE);
    }
#else	
fd = fd;
p_msg = ICF_NULL;
msg_len = msg_len;
*p_ecode = *p_ecode;
#endif
    return ICF_SUCCESS;
}


/*************************************************************************
*
*  FUNCTION NAME  :   icf_port_close_socket
*
*  DESCRIPTION    :   This function is used to close udp/tcp socket
*
*  RETURNS:         Either ICF_SUCCESS or ICF_FAILURE
*************************************************************************/
icf_void_t
icf_port_close_socket(
        icf_uint32_t             fd,
        icf_error_t              *p_ecode)
{

#if defined(ICF_PORT_LINUX) || defined(ICF_PORT_SOLARIS) || defined(ICF_PORT_VXWORKS)
   icf_int32_t sock_err;
   sock_err =  close(fd);

#endif
#ifdef ICF_PORT_WINDOWS
        closesocket(fd);
#endif
    p_ecode = p_ecode;
}


#ifdef ICF_PORT_WINDOWS
/*************************************************************************
*
*  FUNCTION NAME  :   icf_port_recv_udp_win
*
*  DESCRIPTION    :  This function reads a from UDP socket. 
*                    
*
*  RETURNS:         Either ICF_SUCCESS or ICF_FAILURE
*
*************************************************************************/
icf_return_t icf_port_recv_udp_win(icf_global_port_info_st *p_glb_port_data,
						 icf_uint16_t			fd_id,
						 icf_msg_list_st			*p_icf_msg_list,
						 icf_error_t				*p_ecode)
{
	
    icf_int32_t	 len_of_msg_read, sock_err;
	icf_int8_t	 buffer[ICF_BLK_SIZE_MAX];
    icf_uint16_t		payload_length;
	icf_void_t		*p_pa_msg;
	

	len_of_msg_read = recv (fd_id, buffer, ICF_BLK_SIZE_MAX, 0);
	sock_err = WSAGetLastError();

	if(0 >= len_of_msg_read)
    {
        return (ICF_FAILURE);
    }
	
	/* get the payload length */
	payload_length = ((icf_api_header_st*)buffer)->api_length;
    
	p_pa_msg = icf_port_memget(p_glb_port_data, payload_length,
								  ICF_MEM_COMMON,p_ecode);
	
	if( ICF_NULL != p_pa_msg)
	{
		icf_port_memcpy(p_pa_msg, buffer, payload_length);

		p_icf_msg_list->p_msg = (icf_msg_st *)p_pa_msg;
		p_icf_msg_list->p_msg_next = ICF_NULL;
	}
	return ICF_SUCCESS;
}

#endif
/*************************************************************************
*  FUNCTION NAME  :   icf_port_close_fd
*  DESCRIPTION    :   Wrapper for close system call
*************************************************************************/
icf_void_t
icf_port_close_fd(icf_uint32_t    fd)
{

#if (defined(ICF_PORT_SOLARIS) || defined(ICF_PORT_LINUX)) || defined(ICF_PORT_VXWORKS)
    icf_int32_t sock_err;
    ICF_PRINT(((icf_uint8_t *)"\n[Port]:Entered CloseReqForFD:%u\n",fd));
    sock_err = close(fd);
#endif
#ifdef ICF_PORT_WINDOWS
    ICF_PRINT(((icf_uint8_t *)"\n[Port]:Entered CloseReqForFD:%u\n",fd));
    closesocket(fd);
#endif

}

/*************************************************************************
*  FUNCTION NAME  :   icf_port_connect
*  DESCRIPTION    :   Wrapper for connect system call
*  This routine is invoked by IPPHONETK to make a non-blocking connect
*  to a TCP server. If the connection is accepted immediately, it 
*  returns a success, if the peer sends a PENDING signal, it returns failure
*  with error code set as CONNECT_RESP_AWAITED and in case there is 
*  no TCP server at  peer it return failure wo this error.
*************************************************************************/
icf_return_t
icf_port_nonblk_connect(
        INOUT  icf_uint16_t  fd,
        IN     icf_uint16_t  dest_port_num,
        IN     icf_uint8_t   *p_dest_ip,
        IN     icf_uint16_t  self_prot_port,
        IN     icf_uint8_t   *p_self_ip,
        OUT    icf_error_t   *p_ecode
        )
{
#ifdef ICF_PORT_WINDOWS
	icf_uint32_t		error =0 ;
#endif
/*#if (defined(ICF_PORT_SOLARIS) || defined(ICF_PORT_LINUX)) || defined(ICF_PORT_VXORKS) */
#if defined(ICF_PORT_SOLARIS) ||  defined(ICF_PORT_VXORKS)
	int errno;
#endif

#if (defined(ICF_PORT_SOLARIS) || defined(ICF_PORT_LINUX) || defined(ICF_PORT_WINDOWS)) || defined(ICF_PORT_VXWORKS)
#ifdef ICF_IPV6_ENABLED
	struct sockaddr_storage remote_addr;
	char port[6] = "\0";
	struct addrinfo *p_ai_response = NULL;
#else
    struct sockaddr_in  remote_addr;
#endif
    size_t  remote_addr_len = sizeof(struct sockaddr);
#endif

#ifdef ICF_PORT_VXWORKS
	int					ioctl_arg = 0;
#endif

#if (defined(ICF_PORT_SOLARIS) || defined(ICF_PORT_LINUX) || defined(ICF_PORT_WINDOWS)) || defined(ICF_PORT_VXWORKS)
    /* if the self_port is not null, then we need to bind to that port, 
     * this is a case , when the SA has been established and so we need 
     * to bind to this specific port instead of the ephemeral port
     */
    if ((ICF_NULL != self_prot_port) ||
         (ICF_NULL != p_self_ip))
    {
		ICF_PRINT(((icf_uint8_t *)"\n[PORT]: Attempting bind ...\n"));
        if(ICF_FAILURE == icf_port_bind(fd,
           p_self_ip,
           self_prot_port,
           p_ecode))
        {
            ICF_PRINT(((icf_uint8_t *)"\n[PORT]: Failed to bind to self protected port"));
            return ICF_FAILURE;
        }
        else
        {
            ICF_PRINT(((icf_uint8_t *)"\n[PORT]: socket bind successful"));
        }

    }

#ifdef ICF_IPV6_ENABLED
	itoa(dest_port_num,port,10);
	if(getaddrinfo(p_dest_ip, port, NULL, &p_ai_response) != 0){
		ICF_PRINT(("\nFailure in getting address information"));
		return ICF_FAILURE;
	}
	if(p_ai_response == NULL){
		ICF_PRINT(("\nFailure in getting address information"));
		return ICF_FAILURE;
	}
	memcpy((void *)&remote_addr, (void *)(p_ai_response->ai_addr), 
		p_ai_response->ai_addrlen);
	remote_addr_len = p_ai_response->ai_addrlen;
	freeaddrinfo(p_ai_response);
	p_ai_response = NULL;
#else
    remote_addr.sin_family = AF_INET;
    remote_addr.sin_port = htons(dest_port_num);
    remote_addr.sin_addr.s_addr = inet_addr((icf_int8_t *)p_dest_ip);
    memset(&(remote_addr.sin_zero),'\0',8);
#endif

#ifndef ICF_PORT_VXWORKS
#ifndef ICF_PORT_WINDOWS
	ICF_PRINT(((icf_uint8_t *)"\n[PORT]: Non-VxWorks::Setting fd flags through fcntl ...\n"));
    if (fcntl(fd, F_SETFL, 
                fcntl(fd, F_GETFL) | O_NONBLOCK)<0)
    {
        ICF_PRINT(((icf_uint8_t *)"\nFailure in FCNTL for making the socket nonblocking"));
        return ICF_FAILURE;
    }
	else
	{
        ICF_PRINT(((icf_uint8_t *)"\n[PORT]: fcntl success in making the socket nonblocking"));
	}
#endif
#else /* VxWorks porting to make the socket non-blocking */
    ICF_PRINT(("\n[PORT]: VxWorks::Setting fd flags through ioctl for " \
                                                        "sockfd = %d\n", fd));
        ioctl_arg = TRUE;
    if (0 != ioctl(fd, FIONBIO, (int)&ioctl_arg))
    {
        ICF_PRINT(("\n[PORT]: ioctl FAILED in setting socket nonblock flag\n"));
        return ICF_FAILURE;
    }
        else
        {
        ICF_PRINT(("\n[PORT]: ioctl success in making the socket nonblocking"));
        }
#endif /* ifdef ICF_PORT_VXWORKS */

        ICF_PRINT(((icf_uint8_t *)"\n[PORT]: Attempting connect ...\n"));
    
if ( 0 > connect(fd, 
                (struct sockaddr *)&remote_addr, 
				remote_addr_len))
    {
#ifdef ICF_PORT_WINDOWS
		error = WSAGetLastError();
#endif
        ICF_PRINT(((icf_uint8_t *)"\nFailure in nonblocking connect call\n"));

#if defined(ICF_PORT_SOLARIS) || defined(ICF_PORT_LINUX) || defined(ICF_PORT_VXWORKS)
        ICF_PRINT(((icf_uint8_t *)"\n[PORT]: connect returned with " \
                                 "errno=%d, %s \n", errno, strerror(errno)));

        if ( EINPROGRESS == errno)
#elif ICF_PORT_WINDOWS
		/*error_ret = WSAGetLastError();*/
		if (WSAEWOULDBLOCK == error)
#endif
        {
            ICF_PRINT(((icf_uint8_t *)"\nNonBlockingConnectResponseAwaited"));
            *p_ecode = ICF_CONNECT_RESP_PENDING;
            return ICF_FAILURE;
        }
        else
        {
		    ICF_PRINT(((icf_uint8_t *)"\nReturningFailure!!"));
            return ICF_FAILURE;
        }
    }
    return ICF_SUCCESS;
#endif
}

/*************************************************************************
*  FUNCTION NAME  :   icf_port_blk_connect
*  DESCRIPTION    :   Wrapper for connect system call
*  This routine is invoked by IPPHONETK to make a blocking connect
*  to a TCP server.
*************************************************************************/
icf_return_t
icf_port_blk_connect(
        INOUT  icf_uint16_t  fd,
        IN     icf_uint16_t  dest_port_num,
        IN     icf_uint8_t   *p_dest_ip,
        IN     icf_uint16_t  self_prot_port,
        IN     icf_uint8_t   *p_self_ip,
        OUT    icf_error_t   *p_ecode
        )
{
#if defined(ICF_PORT_SOLARIS)
	int errno;
#endif
#if (defined(ICF_PORT_SOLARIS) || defined(ICF_PORT_LINUX) || defined(ICF_PORT_WINDOWS)) || defined(ICF_PORT_VXWORKS)
#ifdef ICF_IPV6_ENABLED
	struct sockaddr_storage remote_addr;
	char port[6] = "\0";
	struct addrinfo *p_ai_response = NULL;
#else
    struct sockaddr_in  remote_addr;
#endif
    size_t  remote_addr_len = sizeof(struct sockaddr_in);
#endif


#if (defined(ICF_PORT_SOLARIS) || defined(ICF_PORT_LINUX) || defined(ICF_PORT_WINDOWS)) || defined(ICF_PORT_VXWORKS)
    /* if the self_port is not null, then we need to bind to that port, 
     * this is a case , when the SA has been established and so we need 
     * to bind to this specific port instead of the ephemeral port
     */
    
   if ((ICF_NULL != self_prot_port) ||
         (ICF_NULL != p_self_ip))
    {
        if(ICF_FAILURE == icf_port_bind(fd,
            p_self_ip,
            self_prot_port,
            p_ecode))
        {
            ICF_PRINT(((icf_uint8_t *)"\n[PORT]: Failed to bind to self protected port"));
            return ICF_FAILURE;
        }
        else
        {
            ICF_PRINT(((icf_uint8_t *)"\n[PORT]: socket bind successful"));
         }

    }

#ifdef ICF_IPV6_ENABLED
	itoa(dest_port_num,port,10);
	if(getaddrinfo(p_dest_ip, port, NULL, &p_ai_response) != 0){
		ICF_PRINT(("\nFailure in getting address information"));
		return ICF_FAILURE;
	}
	if(p_ai_response == NULL){
		ICF_PRINT(("\nFailure in getting address information"));
		return ICF_FAILURE;
	}
	memcpy((void *)&remote_addr, (void *)(p_ai_response->ai_addr), 
		p_ai_response->ai_addrlen);
	remote_addr_len = p_ai_response->ai_addrlen;
	freeaddrinfo(p_ai_response);
	p_ai_response = NULL;
#else
    remote_addr.sin_family = AF_INET;
    remote_addr.sin_port = htons(dest_port_num);
    remote_addr.sin_addr.s_addr = inet_addr((icf_int8_t *)p_dest_ip);
    memset(&(remote_addr.sin_zero),'\0',8);
#endif
    if ( 0 > connect(fd, 
                (struct sockaddr *)&remote_addr, 
				remote_addr_len))
    {
        ICF_PRINT(((icf_uint8_t *)"\nFailure in blocking connect call"));

	    ICF_PRINT(((icf_uint8_t *)"\nReturningFailure!!"));
        return ICF_FAILURE;
    }
    return ICF_SUCCESS;
#endif
}

/*************************************************************************
*  FUNCTION NAME  :   icf_port_bind
*  DESCRIPTION    :   Wrapper for bind system call. This would primarily be
*  					  required incase of IPSec
*************************************************************************/
icf_return_t
icf_port_bind(INOUT  icf_uint16_t  fd,
 		IN	   icf_uint8_t	*p_self_ip,
		IN	   icf_uint16_t	self_port_num,
        OUT    icf_error_t   *p_ecode
        )
{
	/* local variables */
    icf_return_t     ret_val = ICF_SUCCESS;

#ifdef ICF_IPV6_ENABLED
	
	char port[6] = "\0";
	struct addrinfo *p_ai_response = NULL;
	/* need to bind the connection with the self IP and port for IPSEC */
	struct sockaddr_storage self_addr;

#else /* if IPV6 is compiled off */

    /* need to bind the connection with the self IP and port for IPSEC */
	struct sockaddr_in	self_addr;
#endif

	size_t  self_addr_len = sizeof(self_addr);


#ifdef ICF_IPV6_ENABLED
	
	itoa(self_port_num,port,10);
	if (p_self_ip) /* this is to ensure that IPSEC is configured in the system 
		            * at init-time (read frm ICF.cfg */
	{
		if(getaddrinfo(p_self_ip, port, NULL, &p_ai_response) != 0)
		{
			ICF_PRINT(("\nFailure in getting address information"));
			return ICF_FAILURE;
		}
		if(p_ai_response == NULL)
		{
			ICF_PRINT(("\nFailure in getting address information"));
			return ICF_FAILURE;
		}
		memcpy((void *)&self_addr, (void *)(p_ai_response->ai_addr), 
			p_ai_response->ai_addrlen);
		self_addr_len = p_ai_response->ai_addrlen;
		freeaddrinfo(p_ai_response);
		p_ai_response = NULL;
	}

#else /* else if only IPv4 address is supported */
     ICF_PRINT(((icf_uint8_t *)"\n[PORT]: Binding Socket to %s:%d\n", \
                    p_self_ip, self_port_num));
 
	/* need to set the self_addr struct */
	/* need to make sure that IPSEC is configured at init time */
	if (p_self_ip) 
	{
		self_addr.sin_family = AF_INET;
		self_addr.sin_port = htons(self_port_num);
		self_addr.sin_addr.s_addr = inet_addr((icf_int8_t *)p_self_ip);
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
		ICF_PRINT(((icf_uint8_t *)"\nFailure in BIND for self addr if IPsec is enabled"));
        *p_ecode = ICF_ERROR_SOCKET_BIND;
        return (ICF_FAILURE);
    }


	return ret_val;
}


/*************************************************************************
*  FUNCTION NAME  :   icf_port_isconnected
*  DESCRIPTION    :   Wrapper for getpeername system call
*************************************************************************/
icf_boolean_t
icf_port_isconnected(
        IN      icf_uint32_t         fd)
{
    icf_boolean_t    result = ICF_TRUE;
#if (defined(ICF_PORT_SOLARIS) || defined(ICF_PORT_LINUX) || defined(ICF_PORT_WINDOWS)) || defined(ICF_PORT_VXWORKS)
#ifndef ICF_IPV6_ENABLED
    struct sockaddr_in  address;
#ifdef ICF_PORT_VXWORKS
    icf_int32_t addr_len = sizeof (struct sockaddr_in);
#else
    /*size_t addr_len = sizeof (struct sockaddr_in); */
	icf_uint32_t addr_len = sizeof (struct sockaddr_in);
#endif	
#else
	struct sockaddr_storage address;
	size_t addr_len = sizeof(struct sockaddr_storage);
#endif
    if (0 != getpeername(fd, (struct sockaddr*)&address, &addr_len))
    {
        result = ICF_FAILURE;
    }
#endif
    return result;
}
/*************************************************************************
*  FUNCTION NAME  :   icf_port_accept
*  DESCRIPTION    :   Wrapper for accept system call
*************************************************************************/
icf_return_t
icf_port_accept(
        IN      icf_uint32_t         server_fd,
        INOUT   icf_uint32_t     *p_fd,
        INOUT   icf_uint8_t      *p_peer_ip,
        INOUT   icf_uint16_t     *p_peer_port)
{
    icf_return_t     ret_val = ICF_SUCCESS;
    icf_int16_t      fd;
#if (defined(ICF_PORT_SOLARIS) || defined(ICF_PORT_LINUX) || defined(ICF_PORT_WINDOWS)) || defined(ICF_PORT_VXWORKS)
#ifdef ICF_IPV6_ENABLED
	struct sockaddr_storage source_addr;
    size_t source_addr_len = sizeof (struct sockaddr_storage);
#else
    struct sockaddr_in  source_addr;
#ifdef ICF_PORT_VXWORKS
    icf_uint8_t		host_addr_fmt[INET_ADDR_LEN] = "\0"; 
    /* Fix for CSR 1-5028975 */

    icf_int32_t source_addr_len = sizeof (struct sockaddr_in);
#else
	/* For Warning Removal */
    /*size_t     source_addr_len = sizeof (struct sockaddr_in); */
	icf_uint32_t source_addr_len = sizeof (struct sockaddr_in);
#endif	
#endif

    fd = (icf_int16_t)(accept ( server_fd, (struct sockaddr*)&source_addr,
            &source_addr_len));
    if ( -1 == fd)
    {
        ret_val = ICF_FAILURE;
    }
    else
    {
#ifdef ICF_IPV6_ENABLED
		icf_port_ntop(&source_addr,p_peer_ip);
		if(source_addr.ss_family == AF_INET6){
			*p_peer_port = ntohs(((struct sockaddr_in6 *)&source_addr)->sin6_port);
		}
		else{
			*p_peer_port = ntohs(((struct sockaddr_in *)&source_addr)->sin_port);
		}
#else
        /* Fix for CSR 1-5028975 */
#ifdef ICF_PORT_VXWORKS 
    	inet_ntoa_b(source_addr.sin_addr, host_addr_fmt);
    	icf_port_strcpy(p_peer_ip, host_addr_fmt);
#else
        icf_port_strcpy((icf_uint8_t *)p_peer_ip, (icf_uint8_t *)(inet_ntoa(source_addr.sin_addr)));
#endif
        *p_peer_port = ntohs(source_addr.sin_port);
#endif
        *p_fd = (icf_uint32_t)fd;
        ICF_PRINT(((icf_uint8_t *)"\n[PORT]:IncomingConnAccepted FD:%u",fd));
    }
#endif
    return ret_val;
}

/*************************************************************************
*  FUNCTION NAME  :   icf_port_tcp_recv
*  DESCRIPTION    :   This function reads the given number of bytes from 
*  given TCP conn fd
*  RETURNS:         Either ICF_SUCCESS or ICF_FAILURE
*************************************************************************/
icf_return_t
icf_port_tcp_recv(icf_uint16_t    fd,
        icf_void_t     *p_msg,
        icf_uint32_t  msg_len)
{
#if (defined(ICF_PORT_SOLARIS) || defined(ICF_PORT_LINUX) || defined(ICF_PORT_WINDOWS)) || defined(ICF_PORT_VXWORKS)
    icf_int32_t      num_of_bytes_read = 0;
	if ((0 == fd) || ((icf_uint16_t)ICF_INVALID_SOCKFD == fd))
    {
        ICF_PRINT(((icf_uint8_t *)"\n[PORT]: Not attempting TCP recv on fd 0 or 65535\n"));
		return (ICF_FAILURE);
	}
    num_of_bytes_read = recv (fd, p_msg, msg_len, 0);

    ICF_PRINT(((icf_uint8_t *)"\n[PORT]: icf_port_tcp_recv %d bytes read %d bytes from fd %d\n", \
                                        msg_len,num_of_bytes_read,fd));

    if ( -1 == num_of_bytes_read)
    {
        return (ICF_FAILURE);
    }
    if ( (icf_uint32_t)num_of_bytes_read != msg_len)
    {
        return (ICF_FAILURE);
    }
    ICF_PRINT(((icf_uint8_t *)"\n[PORT]:ReadFromTCPConn FD:%u\n",fd));
#endif
    return (ICF_SUCCESS);
}

/*************************************************************************
*  FUNCTION NAME  :   icf_port_tcp_peek
*  DESCRIPTION    :   This function peeks a message from tcp socket
*  RETURNS:         Either ICF_SUCCESS or ICF_FAILURE
*************************************************************************/
icf_return_t
icf_port_tcp_peek(icf_interface_info_st  *p_interface_info,
                     icf_void_t     *p_msg,
                     icf_error_t    *p_ecode)
{
#if (defined(ICF_PORT_SOLARIS) || defined(ICF_PORT_LINUX) || defined(ICF_PORT_WINDOWS)) || defined(ICF_PORT_VXWORKS)
    icf_int32_t      num_of_bytes_read = 0;

    p_ecode = p_ecode;

    if (ICF_NULL == p_msg)
    {
         return ICF_FAILURE;
    }
    num_of_bytes_read = recv (p_interface_info->fd_id, p_msg, 
            ICF_MAX_TCP_MSG_LNGTH, MSG_PEEK);


    if ( -1 == num_of_bytes_read)
    {
        return (ICF_FAILURE);
    }

    p_interface_info->msg_len = num_of_bytes_read;
    ((icf_uint8_t*)p_msg)[num_of_bytes_read] = '\0';

    ICF_PRINT(((icf_uint8_t *)"\n---TCP MSG RECVD OF LNGTH: %d-FD:%u---\n",num_of_bytes_read,p_interface_info->fd_id));
    ICF_PRINT(((icf_uint8_t *)"-----------------------------------------------------\n"));
    ICF_PRINT(((icf_uint8_t *)"%s",p_msg));
    ICF_PRINT(((icf_uint8_t *)"\n----------------------------------------------------\n"));
    
#endif
    return (ICF_SUCCESS);
}

/*************************************************************************
*  FUNCTION NAME  :   icf_port_gethostbyname
*  DESCRIPTION    :   Wrapper for gethostbyname.p_p_addr is the address
*           of memory allocated by calling routine for storing the IP
*  RETURNS:         Either ICF_SUCCESS or ICF_FAILURE
*************************************************************************/
icf_return_t     icf_port_gethostbyname(
        IN  icf_uint8_t*     p_hostname,
        INOUT   icf_uint8_t*    p_addr)
{
    icf_return_t     ret_val = ICF_SUCCESS;
#if (defined(ICF_PORT_SOLARIS) || defined(ICF_PORT_LINUX) || defined(ICF_PORT_WINDOWS)) || defined(ICF_PORT_VXWORKS)
    struct hostent  *p_host_ip = ICF_NULL;
    struct in_addr  *inaddrptr = ICF_NULL;
    p_host_ip = gethostbyname((const icf_int8_t *)p_hostname);
    if ( NULL == p_host_ip)
    {
        ICF_PRINT(((icf_uint8_t *)"\nPORT:Failure in gethostbyname call"));
        ret_val = ICF_FAILURE;
    }
    else
    {
        inaddrptr = (struct in_addr *)*p_host_ip->h_addr_list;
        icf_port_strcpy(p_addr,(icf_uint8_t*) inet_ntoa(*inaddrptr));
    }
#endif
    return ret_val;
}

/*************************************************************************
*
*  FUNCTION NAME  :   icf_port_recvfrom
*
*  DESCRIPTION    :   This function reads a message from udp socket and
*                   returns the data read and the number of bytes read
*
*
*************************************************************************/
icf_int32_t icf_port_recvfrom(icf_int32_t fd, icf_uint8_t* p_msg)
{
    icf_int32_t      num_of_bytes_read = 0;
#ifndef ICF_IPV6_ENABLED
    struct sockaddr_in  source_addr;
    icf_uint32_t      source_addr_len = sizeof (struct sockaddr);
#else
	struct sockaddr_storage  source_addr;
    icf_uint32_t      source_addr_len = sizeof (struct sockaddr_storage);
#endif
    num_of_bytes_read = recvfrom (fd, (icf_int8_t *)p_msg, ICF_MAX_UDP_MSG_LNGTH, 0,
            (struct sockaddr*)(&source_addr), &source_addr_len);
#ifdef ICF_IPV6_ENABLED
#else
    /*ICF_PRINT(("\nPORT_RECV:Destination IP\t%s\n",inet_ntoa(source_addr.sin_addr)));
	ICF_PRINT(("Destination Port\t\t%d\n",ntohs(source_addr.sin_port)));
	ICF_PRINT(("Buffer Length\t\t\t%d\n",num_of_bytes_read));*/
#endif
    return num_of_bytes_read;

}

/***************************************************************************
 ** FUNCTION:     icf_port_set_sa_fd
 ** DESCRIPTION:  The function is invoked from SSA to set the SA sserver 
 **               sock FD in the win_ipc_info structures
 ***************************************************************************/
icf_void_t  icf_port_set_sa_fd (
            icf_void_t               *p_glb_port_info,
            icf_transport_mode_t     trans_mode,
            icf_uint16_t             fd_id)
{
    /* local variables */
    icf_global_port_info_st *p_glb_port_data = 
        (icf_global_port_info_st*)p_glb_port_info;
    
    FD_SET(fd_id,
        &(p_glb_port_data->win_ipc_info.win_read_fd));
    
    if (ICF_TRANSPORT_TYPE_TCP == trans_mode)
    {
        p_glb_port_data->win_ipc_info.
            curr_act_serv_fd[ICF_WIN_TCP_SA_SERVER]= fd_id;
    }
    else
    if (ICF_TRANSPORT_TYPE_UDP == trans_mode)
    {
        p_glb_port_data->win_ipc_info.
            curr_act_serv_fd[ICF_WIN_UDP_SA_SERVER]= fd_id;
    }

    p_glb_port_data->win_ipc_info.act_read_fd[p_glb_port_data->
        win_ipc_info.act_fd_count] = fd_id;
    
    p_glb_port_data->win_ipc_info.act_fd_count += 1;
}

/***************************************************************************
 ** FUNCTION:     icf_port_set_sa_enabled
 ** DESCRIPTION:  The function is invoked from SSA to identify if the SA 
 **               server is setup or not. Incase the SA server is setup, we 
 **               will use that socket to send messages at the protected port
 **
 ***************************************************************************/
icf_void_t  icf_port_set_sa_enabled (
            icf_void_t        *p_glb_port_info,
            icf_boolean_t     mode)
{
    /* local variables */
    icf_global_port_info_st *p_glb_port_data = 
        (icf_global_port_info_st*)p_glb_port_info;
    
    p_glb_port_data->win_ipc_info.ipsec_enabled = mode;
}

/***************************************************************************
 ** FUNCTION:     icf_port_create_secure_conn
 ** DESCRIPTION:  following is used for creating a TLS conn
 ***************************************************************************/
icf_return_t icf_port_create_secure_conn (
 				icf_void_t   *p_port_inf,               
				icf_uint32_t	*p_conn_handle,
				icf_uint8_t  *dest_ip_addr,
				icf_uint32_t	dest_port_num,
				icf_error_t  *p_ecode)
{
	    icf_return_t                     ret_val = ICF_SUCCESS;
    icf_config_data_st               *p_config_data = ICF_NULL;
    icf_string_st		             self_ip;
	icf_uint8_t      		         *p_self_ip = ICF_NULL;
    icf_uint32_t	                 fd;
    icf_uint32_t	                     index = 0;
    icf_global_port_info_st          *p_port_info;

#ifdef ICF_IPV6_ENABLED
	icf_uint32_t	dType = 0;
#endif

    p_self_ip = self_ip.str;
	*p_self_ip = '\0';

    p_port_info = (icf_global_port_info_st *)p_port_inf;
    
        
	for ( index = 0; index < ICF_MAX_NO_OF_PERSISTENT_CONNECTIONS; index++)
	{
		if ( ICF_INV_FD == 
				p_port_info->win_ipc_info.mph_tcp_conn_fd[index])
		{
			break;
		}
	}
	if ( ICF_MAX_NO_OF_PERSISTENT_CONNECTIONS == index)
	{
		ICF_PRINT(((icf_uint8_t *)"[PORT]: MAX CONN Reached!!!\n"));
		ICF_PRINT(((icf_uint8_t *)"[PORT]: Exitting function <icf_port_open_TcpSocket> with FAILURE\n"));
		return (ICF_FAILURE);
	}

    if(ICF_FAILURE == icf_dbm_get_module_glb_data(
                                    p_persistent_glb_pdb,
                                    (icf_int_module_id_t)ICF_INT_MODULE_CFG,
                                    (icf_void_t *)&p_config_data, p_ecode))
    {
        ICF_PRINT(((icf_uint8_t *)"\n[icf_port_create_secure_conn]: Failed in getting cfg data"));
        return ICF_FAILURE;
    }

    icf_cmn_convert_transport_addr_to_ip_string(
                        &(p_config_data->self_ip_address.addr),
                        p_self_ip);

    
    if ( ICF_FAILURE == icf_port_open_socket(
#ifdef ICF_IPV6_ENABLED
				dType,
#endif
				ICF_TRANSPORT_TYPE_TCP,
                &fd,
                p_ecode))
                
	{
	    ret_val = ICF_FAILURE;
	    ICF_PRINT(((icf_uint8_t *)"\n Failure in opening TCP conn socket"));
	}
    else if(ICF_FAILURE == icf_port_nonblk_connect(
                (icf_uint16_t)(fd),
                (icf_uint16_t)dest_port_num,
                dest_ip_addr,
                p_config_data->self_ip_address.port_num,  
                p_self_ip,   
                p_ecode))
    {
        if (ICF_CONNECT_RESP_PENDING == (*p_ecode))
        {
            /*Assuming non blocking connect always in case of symbian porting for now*/
	        *p_ecode = ICF_CONNECT_RESP_PENDING;
            *p_conn_handle = fd;
            p_port_info->win_ipc_info.mph_tcp_conn_fd[index] = fd;
            FD_SET(fd, &(p_port_info->win_ipc_info.win_read_fd));
        }
        else
        {
            ret_val = ICF_FAILURE;
            ICF_PRINT(((icf_uint8_t *)"\n Failure in connecting to the server"));
        }
    }
    else
    {
        *p_conn_handle = fd;
        p_port_info->win_ipc_info.mph_tcp_conn_fd[index] = fd;

        FD_SET(fd, &(p_port_info->win_ipc_info.win_read_fd));

        ICF_PRINT(((icf_uint8_t *)"[PORT]: Exiting function <icf_port_create_secure_conn> with SUCCESS\n"));
    }

	return ret_val;

}

/***************************************************************************
 ** FUNCTION:     icf_port_close_secure_conn
 ** DESCRIPTION:  following is used for closing a TLS conn
 ***************************************************************************/
icf_return_t icf_port_close_secure_conn (
				icf_void_t   *p_port_inf,               
				icf_uint32_t	*p_conn_handle,
				icf_error_t  *p_ecode)
{
	icf_uint32_t	                     index = 0;
    icf_global_port_info_st          *p_port_info;

	p_ecode = p_ecode;

    p_port_info = (icf_global_port_info_st *)p_port_inf;

    icf_port_close_fd(*p_conn_handle);

    for ( index = 0; index < ICF_MAX_NO_OF_PERSISTENT_CONNECTIONS; index++)
	{
		if ( *p_conn_handle == 
				p_port_info->win_ipc_info.mph_tcp_conn_fd[index])
		{
			p_port_info->win_ipc_info.mph_tcp_conn_fd[index] = ICF_INV_FD;
            break;
		}
	}

	return ICF_SUCCESS;
}


/*****************************************************************************
 *
 * FUNCTION:        icf_port_send_http_req
 *
 * DESCRIPTION:     This function is used for sending an HTTP request over the 
 *   			    network over TCP.
 *                  
 * RETURNS:         ICF_SUCCESS in case of success and ICF_FAILURE in
 *                  case of failure.
 *
*****************************************************************************/
icf_return_t icf_port_send_http_req (
					icf_void_t      *p_port_info, 
					icf_uint32_t    connHandle, 			
					icf_uint8_t     *p_http_msg,
					icf_uint32_t    msg_len,
					icf_error_t     *p_ecode)
{
  
	if ( ICF_NULL == p_port_info)
	{
	}	 
    if (ICF_FAILURE == icf_port_tcp_send(connHandle,
				                         p_http_msg,
				                         msg_len,
				                         p_ecode))
    {
        ICF_PRINT(((icf_uint8_t *)"\n-----------HTTP SEND FAILED-----------------\n"));
        return ICF_FAILURE;
    }
    else
    {
        ICF_PRINT(((icf_uint8_t *)"\n-----------HTTP SEND SUCCESS-----------------\n"));
        return ICF_SUCCESS;
    }
}


#ifdef ICF_SECURE_SUPPORT

/***************************************************************************
 ** FUNCTION:     icf_port_secure_connect
 ** DESCRIPTION:  following is used for issuing an SSL connect
 ***************************************************************************/
icf_void_t* icf_port_secure_connect(
            icf_void_t               *p_secure_ctx,
            icf_uint32_t             fd_id,
            icf_error_t              *p_ecode)
{
	SSL              *pSsl = ICF_NULL;
		SSL_CTX          *pSslCtx = ICF_NULL;
	p_ecode = p_ecode;

		pSslCtx = (SSL_CTX *)(p_secure_ctx);
		
		if ((pSsl = SSL_new(pSslCtx)) != ICF_NULL)
		{
            SSL_set_fd(pSsl,fd_id);
	        SSL_set_connect_state(pSsl);
    		if(SSL_connect(pSsl) <= 0)
	    	{
		    	SSL_free(pSsl);
			    pSsl = ICF_NULL;
		    }
	    }
        return (icf_void_t*)pSsl;
}

icf_void_t icf_port_free_secure_data(
            icf_void_t               *p_secure_data)
{
    SSL              *pSsl = (SSL *)p_secure_data;
    SSL_free(pSsl);
}

/***************************************************************************
 ** FUNCTION:     icf_port_secure_accept
 ** DESCRIPTION:  following is used for issuing an SSL accept
 ***************************************************************************/
icf_void_t* icf_port_secure_accept(
            icf_void_t               *p_secure_ctx,
            icf_uint32_t             fd_id,
            icf_error_t              *p_ecode)
{
    	SSL_CTX            *pSslCtx = (SSL_CTX *)p_secure_ctx;
		SSL                *pSsl = ICF_NULL;
		icf_int32_t 	   dRetVal = 0;
		
		p_ecode = p_ecode;
		/*Create a new SSL Object*/
		if ((pSsl = SSL_new(pSslCtx)) != ICF_NULL)
		{
		    /*Associate this Socket with the SSL Object*/
		    SSL_set_fd(pSsl, fd_id);

		    SSL_set_accept_state(pSsl);
					    
		    /* Accept the New TLS Connection*/
		    dRetVal = SSL_accept(pSsl);

		    if (dRetVal <= 0)
		    {
			    SSL_shutdown(pSsl);
			    SSL_free(pSsl);
                pSsl = ICF_NULL;
		    }
        }
        return (icf_void_t*)pSsl;
}

/*************************************************************************
*  FUNCTION NAME  :   icf_port_secure_read
*  DESCRIPTION    :   This function reads the given number of bytes from 
*  given TLS conn fd
*  RETURNS:         Either ICF_SUCCESS or ICF_FAILURE
*************************************************************************/
icf_return_t
icf_port_secure_read(icf_void_t *p_secure_data,
                  icf_uint32_t  fd_id,         
                  icf_void_t    *p_msg,
                  icf_uint32_t  msg_len)
{

#if (defined(ICF_PORT_SOLARIS) || defined(ICF_PORT_LINUX) || defined(ICF_PORT_WINDOWS)) || defined(ICF_PORT_VXWORKS)
    icf_int32_t      num_of_bytes_read = 0;
    SSL              *pSsl = (SSL *)p_secure_data;

    num_of_bytes_read = SSL_read(pSsl,p_msg,msg_len);
    if ( -1 == num_of_bytes_read)
    {
        return (ICF_FAILURE);
    }

    if ( -1 == num_of_bytes_read)
    {
        return (ICF_FAILURE);
    }
    if ( (icf_uint32_t)num_of_bytes_read != msg_len)
    {
        return (ICF_FAILURE);
    }
    ICF_PRINT(("\n[PORT]:ReadFromTLSConn FD:%u\n",fd_id));
#endif
    return (ICF_SUCCESS);
}
/*************************************************************************
*  FUNCTION NAME  :   icf_port_secure_peek
*  DESCRIPTION    :   This function peeks a message from tls socket
*  RETURNS:         Either ICF_SUCCESS or ICF_FAILURE
*************************************************************************/
icf_return_t
icf_port_secure_peek(icf_void_t *p_secure_data,       
                  icf_interface_info_st  *p_interface_info,
                  icf_void_t     *p_msg,
                  icf_error_t    *p_ecode)
{
#if (defined(ICF_PORT_SOLARIS) || defined(ICF_PORT_LINUX) || defined(ICF_PORT_WINDOWS)) || defined(ICF_PORT_VXWORKS)
    icf_int32_t      num_of_bytes_read = 0;
    SSL              *pSsl = (SSL *)p_secure_data;

    p_ecode = p_ecode;
    num_of_bytes_read = SSL_peek(pSsl,p_msg,ICF_MAX_TCP_MSG_LNGTH);
    if ( -1 == num_of_bytes_read)
    {
        return (ICF_FAILURE);
    }

    p_interface_info->msg_len = num_of_bytes_read;
    ((icf_uint8_t*)p_msg)[num_of_bytes_read] = '\0';

    ICF_PRINT(("\n---TLS MSG RECVD OF LNGTH: %d-FD:%u---\n",num_of_bytes_read,p_interface_info->fd_id));
    ICF_PRINT(("-----------------------------------------------------\n"));
    ICF_PRINT(("%s",p_msg));
    ICF_PRINT(("\n----------------------------------------------------\n"));
    
#endif
    return (ICF_SUCCESS);
}

/*************************************************************************
*
*  FUNCTION NAME  :   icf_port_secure_send
*
*  DESCRIPTION    :   This function sends the message on the SSL conn
*                       corresponding to the fd passed.
*
*  RETURNS:         Either ICF_SUCCESS or ICF_FAILURE
*
*************************************************************************/
icf_return_t
icf_port_secure_send(icf_void_t *p_secure_data,
                     icf_void_t     *p_msg,
                     icf_uint32_t    msg_len,
                     icf_error_t    *p_ecode)
{
    icf_return_t        ret_val = ICF_SUCCESS;
    SSL                 *pSsl = (SSL *)p_secure_data;
    p_ecode = p_ecode;

    if(msg_len != (icf_uint32_t)SSL_write(pSsl,p_msg,msg_len))
	{
		ret_val = ICF_FAILURE;
	}
    return ret_val;
}
#endif


/*************************************************************************
*  FUNCTION NAME  :   icf_port_tcp_non_blocking_peek
*  DESCRIPTION    :   This function peeks a message from tcp socket, in non-blocking mode
*  RETURNS:         Either ICF_SUCCESS or ICF_FAILURE
*************************************************************************/
icf_return_t
icf_port_tcp_non_blocking_peek(icf_interface_info_st  *p_interface_info,
                     icf_void_t     *p_msg,
                     icf_error_t    *p_ecode)
{
#if (defined(ICF_PORT_SOLARIS) || defined(ICF_PORT_LINUX) || defined(ICF_PORT_WINDOWS)) || defined(ICF_PORT_VXWORKS)
    icf_int32_t      num_of_bytes_read = 0;

 #ifdef ICF_PORT_WINDOWS
    icf_uint32_t  iMode = 1;
	icf_uint32_t flags=0;

	/* Set the socket I/O mode; iMode = 0 for blocking; iMode != 0 for non-blocking */
      
  
      
    ioctlsocket( p_interface_info->fd_id, FIONBIO,(unsigned long *) &iMode);
  
    flags = MSG_PEEK;

    num_of_bytes_read = recv (p_interface_info->fd_id, p_msg, 
            ICF_MAX_TCP_MSG_LNGTH, MSG_PEEK);

#else 
    num_of_bytes_read = recv (p_interface_info->fd_id, p_msg, 
            ICF_MAX_TCP_MSG_LNGTH, MSG_PEEK|MSG_DONTWAIT);
#endif

    if ( -1 == num_of_bytes_read)
    {
        return (ICF_FAILURE);
    }

    p_interface_info->msg_len = num_of_bytes_read;
    ((icf_uint8_t*)p_msg)[num_of_bytes_read] = '\0';

    ICF_PRINT(((icf_uint8_t *)"\n---TCP MSG RECVD OF LNGTH: %d-FD:%u---\n",num_of_bytes_read,p_interface_info->fd_id));
    ICF_PRINT(((icf_uint8_t *)"-----------------------------------------------------\n"));
    ICF_PRINT(((icf_uint8_t *)"%s",p_msg));
    ICF_PRINT(((icf_uint8_t *)"\n----------------------------------------------------\n"));
    
#endif
	p_ecode = p_ecode;
    return (ICF_SUCCESS);
}
