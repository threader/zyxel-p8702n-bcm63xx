/*****************************************************************************
* 
* FUNCTION    : Source file for Generic library - Socket manager 
*               implementation.
*            
******************************************************************************
*
* FILENAME    : gl_skm_sys_calls_wrppr_api.c
*
* DESCRIPTION : This file implements socket management functionality of 
*               generic libraries. 
*
*   DATE            NAME        REFERENCE           REASON
*-----------    ------------   -----------      ------------------------------
* April'01      Rajesh Jain     New file        Generic Libraries
*
* 18/08/2001    Prashant Jain   SPR 2276        Compiling with CC on Solaris.
*
* 04/09/2001    Prashant Jain   SPR-2560        Adding OSE Operating System 
*                                               Support.
* 04/10/2001    Ravi Jindal     SPR-2751        Remove warnings with cc,
*                                               gcc,CC on Solaris. 
* 18/10/2001    Ravi Jindal     SPR-2788        Packaging of Genlib-Code. 
* 19/10/2001    Ravi Jindal     SPR-2802        Adding Pre-ANSI-C features.
* 17/11/2001    Ravi Jindal     SPR-3107        Incoprate the defects and change
*                                               cosmmetics of file.
*
* 04/04/2002    Ravi Jindal     SPR-3959        Enhancements for Rel-1.0.
* 12/07/2004    Navneet Gupta   SPR 6946        QNX port
* 12/08/2004    Sudhanshu       SPR 6946        QNX warnings removal
*       
*COPYRIGHT , Aricent, 2006
*****************************************************************************/

#include "gl_skm_interfaces.h"

/*****************************************************************************
* Function Name    : gl_sm_init
* Arguments        : void
* Return value     : gl_return_t
* Functionality    : Initializes the socket manager.   
* Functions called :
*****************************************************************************/
gl_return_t    
gl_sm_init( void )
/****************************************************************************/
{
#ifdef GL_WINNT
    WSADATA                     wsaData; 
    
    if (WSAStartup( 1, &wsaData ) != 0 )
    {
        /* 
         * Tell the user that we could not find a usable WinSock DLL.
         */
        return GL_FAILURE;
    }
#endif
    return GL_SUCCESS;
}



/*****************************************************************************
* Function Name    : gl_sm_create_socket
* Arguments        : gl_sm_addr_family_et   family,
*                    gl_sm_sock_type_et     type,
*                    gl_sm_protocol_et      protocol
* Return value     : gw_S32bit
* Functionality    : Opens a socket and returns a socket descriptor.
* Functions called :
*****************************************************************************/
gw_S32bit 
gl_sm_create_socket

#ifdef _ANSI_C_
    ( gl_sm_addr_family_et      family, 
      gl_sm_sock_type_et        type,
      gl_sm_protocol_et         protocol
    )
#else
    ( family, type, protocol )
    gl_sm_addr_family_et        family;
    gl_sm_sock_type_et          type;
    gl_sm_protocol_et           protocol;
#endif
    
/****************************************************************************/
{
    gw_S32bit                   soc_family;
    gw_S32bit                   soc_type;
    gw_S32bit                   soc_proto;
    gw_S32bit                   sockid;
    
    soc_family = (gw_S32bit)family;
    soc_type   = (gw_S32bit)type;
    soc_proto  = (gw_S32bit)protocol;

#ifdef GL_WINNT
    if(INVALID_SOCKET==(sockid=(gw_S32bit)socket(soc_family,
                                          soc_type, soc_proto)))
#elif GL_VXWORKS
    if(ERROR==(sockid=socket(soc_family, soc_type, soc_proto)))
#else
    if(GL_ERROR==(sockid=socket(soc_family, soc_type, soc_proto)))
#endif
    {
        return GL_ERROR;
    }
    else
    {
        return sockid;
    }
}



/*****************************************************************************
* Function Name    : gl_sm_bind_socket
* Arguments        : gw_S32bit          sockid,
*                    gw_S8bit           *ip_addr,
*                    gw_U16bit          portno
* Return value     : gl_return_t
* Functionality    : Associates a network address with a specified socket.
* Functions called :
*****************************************************************************/
gl_return_t 
gl_sm_bind_socket

#ifdef _ANSI_C_
    ( gw_S32bit                 sockid,
      gw_S8bit                  *ip_addr,
      gw_U16bit                 portno
    )
#else
    ( sockid, ip_addr, portno )
    gw_S32bit                   sockid;
    gw_S8bit                    *ip_addr;
    gw_U16bit                   portno;
#endif
    
/****************************************************************************/
{
    struct sockaddr_in          sock_addr;

    memset(&sock_addr, 0, sizeof(sock_addr));

    sock_addr.sin_family=AF_INET;
    sock_addr.sin_port=htons(portno);
    sock_addr.sin_addr.s_addr=inet_addr(ip_addr);
#ifdef GL_WINNT
    if(SOCKET_ERROR==bind(sockid, (struct sockaddr*)&sock_addr,
                          sizeof(sock_addr)))
#elif GL_VXWORKS
    if(ERROR==bind(sockid, (struct sockaddr*)&sock_addr, sizeof(sock_addr)))
#else
    if(GL_ERROR==bind(sockid, (struct sockaddr*)&sock_addr, 
                      sizeof(sock_addr)))
#endif
    {
        return GL_FAILURE;
    }
    else
    {
        return GL_SUCCESS;
    }
}


/*****************************************************************************
* Function Name    : gl_sm_conect_socket
* Arguments        : gw_S32bit          sockid,
*                    gw_S8bit           *ip_addr,
*                    gw_U16bit          portno
* Return value     : gl_return_t
* Functionality    : Connects a specified socket to a specified remote address.
* Functions called :
*****************************************************************************/
gl_return_t 
gl_sm_connect_socket

#ifdef _ANSI_C_
    ( gw_S32bit                 sockid,
      gw_S8bit                  *ip_addr, 
      gw_U16bit                 portno
    )
#else
    ( sockid, ip_addr, portno )
    gw_S32bit                   sockid;
    gw_S8bit                    *ip_addr;
    gw_U16bit                   portno;
#endif
    
/****************************************************************************/
{
    struct sockaddr_in          sock_addr;

    memset(&sock_addr, 0, sizeof(sock_addr));

    sock_addr.sin_family=AF_INET;
    sock_addr.sin_port=htons(portno);
    sock_addr.sin_addr.s_addr=inet_addr(ip_addr);
#ifdef GL_WINNT
    if(SOCKET_ERROR==connect(sockid, (struct sockaddr*)&sock_addr, 
                             sizeof(sock_addr)))
#elif GL_VXWORKS
    if(ERROR==connect(sockid, (struct sockaddr*)&sock_addr, 
                      sizeof(sock_addr)))
#else
    if(GL_ERROR==connect(sockid, (struct sockaddr*)&sock_addr, 
                         sizeof(sock_addr)))
#endif
    {
        return GL_FAILURE;
    }
    else
    {
        return GL_SUCCESS;
    }
}




/*****************************************************************************
* Function Name    : gl_sm_listen_socket
* Arguments        : gw_S32bit          sockid,
*                    gw_S32bit          backlog
* Return value     : gl_return_t
* Functionality    : Connects a specified socket to a specified remote address.
* Functions called :
*****************************************************************************/
gl_return_t 
gl_sm_listen_socket

#ifdef _ANSI_C_
    ( gw_S32bit                 sockid,
      gw_S32bit                 backlog 
    )
#else
    ( sockid, backlog )
    gw_S32bit                   sockid;
    gw_S32bit                   backlog;
#endif
    
/****************************************************************************/
{
#ifdef GL_WINNT
    if(SOMAXCONN < backlog)
    {
        return GL_FAILURE;
    }
    if(SOCKET_ERROR==listen(sockid, backlog)) 
#elif GL_VXWORKS
    if(ERROR==listen(sockid, backlog)) 
#elif GL_OSE
    if(GL_ERROR==listen(sockid, backlog)) 
#elif defined(GL_SOLARIS) || defined(GW_LINUX) || defined(GW_QNX) 
    if(6 < backlog)
    {
        return GL_FAILURE;
    }
    /*
     * For Solaris Maximum Connection backlog is not known.
     * So 6 is used as a reference value.
     */ 
    if(GL_ERROR==listen(sockid, backlog)) 
#endif
    {
        return GL_FAILURE;
    }
    else
    {
        return GL_SUCCESS;
    }
}




/*****************************************************************************
* Function Name    : gl_sm_accept_socket
* Arguments        : gw_S32bit          sockid,
*                    gw_S8bit           *p_peer_ip_addr,
*                    gw_U16bit          *p_peer_portno
* Return value     : gl_return_t
* Functionality    : Connects a specified socket to a specified remote address.
* Functions called :
*****************************************************************************/
gw_S32bit 
gl_sm_accept_socket

#ifdef _ANSI_C_
    ( gw_S32bit                 sockid,
      gw_S8bit                  *p_peer_ip_addr, 
      gw_U16bit                 *p_peer_portno
    )
#else
    ( sockid, p_peer_ip_addr, p_peer_portno )
    gw_S32bit                   sockid;
    gw_S8bit                    *p_peer_ip_addr;
    gw_U16bit                   *p_peer_portno;
#endif
    
/****************************************************************************/
{
    struct sockaddr_in          sock_addr;
    gw_S32bit                   sock_addr_len;
    
    sock_addr_len = sizeof(sock_addr);
#ifdef GL_WINNT
    if(SOCKET_ERROR == (sockid = accept(sockid, (struct sockaddr*)&sock_addr, 
                             &sock_addr_len)))
#elif GL_VXWORKS
    if(ERROR == (sockid = accept(sockid, (struct sockaddr*)&sock_addr, 
                        &sock_addr_len)))
#elif GL_LINUX
    if(GL_ERROR == (sockid = accept(sockid, (struct sockaddr*)&sock_addr, 
                        (gw_U32bit *)&sock_addr_len)))    
#else
    if(GL_ERROR == (sockid = accept(sockid, (struct sockaddr*)&sock_addr, 
                        &sock_addr_len)))
#endif
    {
        return GL_ERROR;
    }
    else
    {
        gl_sm_inet_ntoa(sock_addr.sin_addr.s_addr, p_peer_ip_addr);
        *p_peer_portno = ntohs(sock_addr.sin_port);
        return sockid;
    }
}





/*****************************************************************************
* Function Name    : gl_sm_ioctl_socket
* Arguments        : gw_S32bit              sockid,
*                    gl_sm_ioctl_request_et ioctl_equest,
*                    gw_S32bit              *p_ioctl_arg
* Return value     : gl_return_t
* Functionality    : It is used to set or retrieve operating parameters 
*                    associated with the socket.
* Functions called :
*****************************************************************************/
gl_return_t 
gl_sm_ioctl_socket

#ifdef _ANSI_C_
    ( gw_S32bit                 sockid,
      gl_sm_ioctl_request_et    ioctl_request,
      gw_S32bit                 *p_ioctl_arg
    )
#else
    ( sockid, ioctl_request, p_ioctl_arg )
    gw_S32bit                   sockid;
    gl_sm_ioctl_request_et      ioctl_request;
    gw_S32bit                   *p_ioctl_arg;
#endif
    
/****************************************************************************/
{
    gw_S32bit                   ioctl_req;

    ioctl_req = (gw_S32bit)ioctl_request;
    if(GL_SM_FIONBIO == ioctl_request)
    {
        *p_ioctl_arg = 1;
    }
#ifdef GL_WINNT
    if(SOCKET_ERROR == ioctlsocket(sockid, ioctl_req, (u_long *)p_ioctl_arg)) 
#elif GL_VXWORKS
    if(ERROR == ioctl(sockid, ioctl_req, (gw_S32bit)p_ioctl_arg)) 
#elif GL_OSE
    if(GL_ERROR == ioctl(sockid, ioctl_req, (gw_S8bit *)p_ioctl_arg))
#else
    if(GL_ERROR == ioctl(sockid, ioctl_req, p_ioctl_arg))
#endif
    {
        return GL_FAILURE;
    }
    else
    {
        return GL_SUCCESS;
    }
}




/*****************************************************************************
* Function Name    : gl_sm_sendto
* Arguments        : gw_S32bit          sockid,
*                    gw_S8bit           *buff,
*                    gw_S32bit          nbytes,
*                    gw_S32bit          flags,
*                    gl_pvoid_t         p_destaddr,
*                    gw_S32bit          destaddr_len
* Return value     : gw_S32bit
* Functionality    : Sends message on a specified datagram socket 
*                    to a specified IP address.
* Functions called :                  
*****************************************************************************/
gw_S32bit 
gl_sm_sendto

#ifdef _ANSI_C_
    ( gw_S32bit                 sockid,
      gw_S8bit                  *buff,
      gw_S32bit                 nbytes,
      gw_S32bit                 flags, 
      gl_pvoid_t                p_destaddr, 
      gw_S32bit                 destaddr_len
    )
#else
    ( sockid, buff, nbytes, flags, p_destaddr, destaddr_len )
    gw_S32bit                   sockid;
    gw_S8bit                    *buff;
    gw_S32bit                   nbytes;
    gw_S32bit                   flags; 
    gl_pvoid_t                  p_destaddr;
    gw_S32bit                   destaddr_len;
#endif
    
/****************************************************************************/
{
    gw_S32bit                   num_bytes;
    struct sockaddr             *p_remote_destaddr;
                
    p_remote_destaddr = (struct sockaddr *)p_destaddr;

#ifdef GL_WINNT
    if(SOCKET_ERROR==(num_bytes=sendto(sockid, buff, nbytes, flags, 
                                       p_remote_destaddr, destaddr_len)))
#elif GL_VXWORKS
    if(ERROR==(num_bytes=sendto(sockid, buff, nbytes, flags, 
                                p_remote_destaddr, destaddr_len)))
#else
    if(GL_ERROR==(num_bytes=sendto(sockid, buff, nbytes, flags, 
                                   p_remote_destaddr, destaddr_len)))
#endif
    {
        return GL_ERROR;
    }
    else
    {
        return num_bytes;
    }
}


/*****************************************************************************
* Function Name    : gl_sm_recvfrom
* Arguments        : gw_S32bit           sockid,
*                    gw_S8bit            *buff, 
*                    gw_S32bit           nbytes,
*                    gw_S32bit           flags,
*                    gl_pvoid_t          p_srcaddr,
*                    gw_S32bit           *p_addrlen
* Return value     : gw_S32bit
* Functionality    : Receives a message from a datagram socket regardless of 
*                    whether it is connected.
* Functions called :
*****************************************************************************/
gw_S32bit 
gl_sm_recvfrom

#ifdef _ANSI_C_
    ( gw_S32bit                 sockid,
      gw_S8bit                  *buff, 
      gw_S32bit                 nbytes,
      gw_S32bit                 flags,
      gl_pvoid_t                p_srcaddr,
      gw_S32bit                 *p_addrlen
    )
#else
    ( sockid, buff, nbytes, flags, p_srcaddr, p_addrlen )
    gw_S32bit                   sockid;
    gw_S8bit                    *buff;
    gw_S32bit                   nbytes;
    gw_S32bit                   flags;
    gl_pvoid_t                  p_srcaddr;
    gw_S32bit                   *p_addrlen;
#endif
    
/****************************************************************************/
{
    gw_S32bit                   num_bytes;
    struct sockaddr             *p_local_srcaddr;
                
    p_local_srcaddr = (struct sockaddr *)p_srcaddr;

#ifdef GL_WINNT
    if(SOCKET_ERROR==(num_bytes=recvfrom(sockid, buff, nbytes, flags,
                                         p_local_srcaddr, p_addrlen)))
#elif GL_VXWORKS
    if(ERROR==(num_bytes=recvfrom(sockid, buff, nbytes, flags, 
                                  p_local_srcaddr, p_addrlen)))
#elif GL_LINUX
    if(GL_ERROR==(num_bytes=recvfrom(sockid, buff, nbytes, flags,
                                  p_local_srcaddr, (gw_U32bit *)p_addrlen)))
#else
    if(GL_ERROR==(num_bytes=recvfrom(sockid, buff, nbytes, flags, 
                                     p_local_srcaddr, p_addrlen)))
#endif
    {
        return GL_ERROR;
    }
    else
    {
        return num_bytes;
    }
}



/*****************************************************************************
* Function Name    : gl_sm_send
* Arguments        : gw_S32bit          sockid,
*                    gw_S8bit           *buff, 
*                    gw_S32bit          nbytes,
*                    gw_S32bit          flags
* Return value     : gw_S32bit
* Functionality    : Sends message on a specified stream socket.
* Functions called :
*****************************************************************************/
gw_S32bit 
gl_sm_send

#ifdef _ANSI_C_
    ( gw_S32bit                 sockid,
      gw_S8bit                  *buff, 
      gw_S32bit                 nbytes,
      gw_S32bit                 flags
    )
#else
    ( sockid, buff, nbytes, flags )
    gw_S32bit                   sockid;
    gw_S8bit                    *buff;
    gw_S32bit                   nbytes;
    gw_S32bit                   flags;
#endif
    
/****************************************************************************/
{
    gw_S32bit                   num_bytes;

#ifdef GL_WINNT
    if(SOCKET_ERROR==(num_bytes=send(sockid, buff, nbytes, flags)))
#elif GL_VXWORKS
    if(ERROR==(num_bytes=send(sockid, buff, nbytes, flags)))
#elif defined(GL_SOLARIS) || defined(GW_LINUX) || defined(GW_QNX)
    if(GL_ERROR==(num_bytes=send(sockid, buff, nbytes, flags)))
#elif GL_OSE
    if(GL_ERROR==(num_bytes=inet_send(sockid, (const void * )buff, 
                                      nbytes, flags)))
#endif
    {
        return GL_ERROR;
    }
    else
    {
        return num_bytes;
    }
}


/*****************************************************************************
* Function Name    : gl_sm_recv
* Arguments        : gw_S32bit          sockid,
*                    gw_S8bit           *buff, 
*                    gw_S32bit          nbytes, 
*                    gw_S32bit          flags
* Return value     : gw_S32bit
* Functionality    : Receives a message from a stream socket.
* Functions called :
*****************************************************************************/
gw_S32bit 
gl_sm_recv

#ifdef _ANSI_C_
    ( gw_S32bit                 sockid,
      gw_S8bit                  *buff, 
      gw_S32bit                 nbytes, 
      gw_S32bit                 flags
    )
#else
    ( sockid, buff, nbytes, flags )
    gw_S32bit                   sockid;
    gw_S8bit                    *buff;
    gw_S32bit                   nbytes;
    gw_S32bit                   flags;
#endif
    
/****************************************************************************/
{
    gw_S32bit                   num_bytes;
                
#ifdef GL_WINNT
    if(SOCKET_ERROR==(num_bytes=recv(sockid, buff, nbytes, flags)))
#elif GL_VXWORKS
    if(ERROR==(num_bytes=recv(sockid, buff, nbytes, flags)))
#else
    if(GL_ERROR==(num_bytes=recv(sockid, buff, nbytes, flags)))
#endif
    {
        return GL_ERROR;
    }
    else
    {
        return num_bytes;
    }
}



/*****************************************************************************
* Function Name    : gl_sm_setsockopt
* Arguments        : gw_S32bit          sockid,
*                    gl_sm_level_et     level,
*                    gl_sm_optname_et   optname, 
*                    gw_S8bit           *p_optval, 
*                    gw_S32bit          optlen
* Return value     : gl_return_t
* Functionality    : Sets the options associated with a socket.
* Functions called :
*****************************************************************************/
gl_return_t 
gl_sm_setsockopt

#ifdef _ANSI_C_
    ( gw_S32bit                 sockid,
      gl_sm_level_et            level,
      gl_sm_optname_et          optname, 
      gw_S8bit                  *p_optval, 
      gw_S32bit                 optlen
    )
#else
    ( sockid, level, optname, p_optval, optlen )
    gw_S32bit                   sockid;
    gl_sm_level_et              level;
    gl_sm_optname_et            optname;
    gw_S8bit                    *p_optval;
    gw_S32bit                   optlen;
#endif
    
/****************************************************************************/
{
    gw_S32bit                   lev, opt;

    lev = (gw_S32bit)level;
    opt = (gw_S32bit) optname;

#ifdef GL_WINNT
    if(SOCKET_ERROR==setsockopt(sockid, lev, opt, p_optval, optlen))
#elif GL_VXWORKS
    if(ERROR==setsockopt(sockid, lev, opt, p_optval, optlen))
#else
    if(GL_ERROR==setsockopt(sockid, lev, opt, p_optval, optlen))
#endif
    {
        return GL_FAILURE;
    }
    else
    {
        return GL_SUCCESS;
    }
}




/*****************************************************************************
* Function Name    : gl_sm_getsockopt
* Arguments        : gw_S32bit          sockid,
*                    gl_sm_level_et     level,
*                    gl_sm_optname_et   optname, 
*                    gw_S8bit           *p_optval, 
*                    gw_S32bit          *p_optlen
* Return value     : gl_return_t
* Functionality    : Gets the options associated with a socket.
* Functions called :
*****************************************************************************/
gl_return_t 
gl_sm_getsockopt

#ifdef _ANSI_C_
    ( gw_S32bit                 sockid,
      gl_sm_level_et            level,
      gl_sm_optname_et          optname, 
      gw_S8bit                  *p_optval, 
      gw_S32bit                 *p_optlen
    )
#else
    ( sockid, level, optname, p_optval, p_optlen )
    gw_S32bit                   sockid;
    gl_sm_level_et              level;
    gl_sm_optname_et            optname;
    gw_S8bit                    *p_optval;
    gw_S32bit                   *p_optlen;
#endif
    
/****************************************************************************/
{
    gw_S32bit                   lev, opt;

    lev = (gw_S32bit)level;
    opt = (gw_S32bit) optname;

#ifdef GL_WINNT
    if(SOCKET_ERROR==getsockopt(sockid, lev, opt, p_optval, p_optlen))
#elif GL_VXWORKS
    if(ERROR==getsockopt(sockid, lev, opt, p_optval, p_optlen))
#elif GL_LINUX
    if(GL_ERROR==getsockopt(sockid, lev, opt, p_optval, (gw_U32bit *)p_optlen))
#else
    if(GL_ERROR==getsockopt(sockid, lev, opt, p_optval, p_optlen))
#endif
    {
        return GL_FAILURE;
    }
    else
    {
        return GL_SUCCESS;
    }
}





/*****************************************************************************
* Function Name    : gl_sm_getsockname
* Arguments        : gw_S32bit           sockid,
*                    gl_pvoid_t          p_srcaddr,
*                    gw_S32bit           *p_addrlen
* Return value     : gl_return_t
* Functionality    : To get the local association of a socket.
* 
* Functions called :
*****************************************************************************/
gl_return_t 
gl_sm_getsockname

#ifdef _ANSI_C_
    ( gw_S32bit                 sockid,
      gl_pvoid_t                p_localaddr,
      gw_S32bit                 *p_localaddrlen
    )
#else
    ( sockid, p_localaddr, p_localaddrlen )
    gw_S32bit                   sockid;
    gl_pvoid_t                  p_localaddr;
    gw_S32bit                   *p_localaddrlen;
#endif
    
/****************************************************************************/
{

#ifdef GL_WINNT
    if(SOCKET_ERROR == getsockname(sockid, (struct sockaddr *)p_localaddr,
                    p_localaddrlen))
#elif GL_VXWORKS
    if(ERROR == getsockname(sockid, (struct sockaddr *)p_localaddr, 
                    p_localaddrlen))
#elif GL_LINUX
    if(GL_ERROR == getsockname(sockid, (struct sockaddr *)p_localaddr, 
                    (gw_U32bit *)p_localaddrlen))  
#else
    if(GL_ERROR == getsockname(sockid, (struct sockaddr *)p_localaddr,
                    p_localaddrlen)) 
#endif
    {
        return GL_FAILURE;
    }
    else
    {
        return GL_SUCCESS;
    }
}







/*****************************************************************************
* Function Name    : gl_sm_getpeername
* Arguments        : gw_S32bit           sockid,
*                    gl_pvoid_t          p_peeraddr,
*                    gw_S32bit           *p_peeraddrlen
* Return value     : gl_return_t
* Functionality    : To get the Peer association of a socket.
* 
* Functions called :
*****************************************************************************/
gl_return_t 
gl_sm_getpeername

#ifdef _ANSI_C_
    ( gw_S32bit                 sockid,
      gl_pvoid_t                p_peeraddr,
      gw_S32bit                 *p_peeraddrlen
    )
#else
    ( sockid, p_peeraddr, p_peeraddrlen )
    gw_S32bit                   sockid;
    gl_pvoid_t                  p_peeraddr;
    gw_S32bit                   *p_peeraddrlen;
#endif
    
/****************************************************************************/
{

#ifdef GL_WINNT
    if(SOCKET_ERROR == getpeername(sockid, (struct sockaddr *)p_peeraddr, 
                    p_peeraddrlen))
#elif GL_VXWORKS
    if(ERROR == getpeername(sockid, (struct sockaddr *)p_peeraddr, 
                    p_peeraddrlen))
#elif GL_LINUX
    if(GL_ERROR == getpeername(sockid, (struct sockaddr *)p_peeraddr, 
                    (gw_U32bit*)p_peeraddrlen))
#else
    if(GL_ERROR == getpeername(sockid, (struct sockaddr *)p_peeraddr,
                    p_peeraddrlen)) 
#endif
    {
        return GL_FAILURE;
    }
    else
    {
        return GL_SUCCESS;
    }
}







/*****************************************************************************
* Function Name    : gl_sm_close_socket
* Arguments        : gw_S32bit          sockid 
* Return value     : gl_return_t
* Functionality    : Closes a socket.
* Functions called :
*****************************************************************************/
gl_return_t 
gl_sm_close_socket

#ifdef _ANSI_C_
    ( gw_S32bit                 sockid )
#else
    ( sockid )
    gw_S32bit                   sockid;
#endif
    
/****************************************************************************/
{
#ifdef GL_WINNT
    if(SOCKET_ERROR==closesocket(sockid))
#elif GL_VXWORKS
    if(ERROR==close(sockid))
#else
    if(GL_ERROR==close(sockid))
#endif
    {
        return GL_FAILURE;
    }
    else
    {
        return GL_SUCCESS;    
    }
}



/*****************************************************************************
* Function Name    : gl_sm_htonl
* Arguments        : gw_U32bit          hostlong
* Return value     : gw_U32bit
* Functionality    : Converts a long from host to network byte ordering.
* Functions called :
*****************************************************************************/
gw_U32bit 
gl_sm_htonl

#ifdef _ANSI_C_
    ( gw_U32bit                 hostlong )
#else
    ( hostlong )
    gw_U32bit                   hostlong;
#endif
    
/****************************************************************************/
{
    return htonl(hostlong);
}


/*****************************************************************************
* Function Name    : gl_sm_htons
* Arguments        : gw_U16bit          hostshort
* Return value     : gw_U16bit
* Functionality    : Converts a short from host to network byte ordering.
* Functions called :
*****************************************************************************/
gw_U16bit 
gl_sm_htons

#ifdef _ANSI_C_
    ( gw_U16bit                 hostshort )
#else
    ( hostshort )
    gw_U16bit                   hostshort;
#endif
    
/****************************************************************************/
{
    return htons(hostshort);
}



/*****************************************************************************
* Function Name    : gl_sm_ntohl
* Arguments        : gw_U32bit          netlong
* Return value     : gw_U32bit
* Functionality    : Converts a long from network to host byte ordering.
* Functions called :
*****************************************************************************/
gw_U32bit 
gl_sm_ntohl

#ifdef _ANSI_C_
    ( gw_U32bit                 netlong )
#else
    ( netlong )
    gw_U32bit                   netlong;
#endif
    
/****************************************************************************/
{
    return ntohl(netlong);
}



/*****************************************************************************
* Function Name    : gl_sm_ntohs
* Arguments        : gw_U16bit          netshort
* Return value     : gw_U16bit
* Functionality    : Converts a short from network to host byte ordering.
* Functions called :
*****************************************************************************/
gw_U16bit 
gl_sm_ntohs

#ifdef _ANSI_C_
    ( gw_U16bit                 netshort )
#else
    ( netshort )
    gw_U16bit                   netshort;
#endif
    
/****************************************************************************/
{
    return ntohs(netshort);
}



/*****************************************************************************
* Function Name    : gl_sm_inet_addr
* Arguments        : gw_S8bit           *p_addr
* Return value     : gw_U32bit
* Functionality    : Converts a dot notation Internet address to a long integer.
* Functions called :
*****************************************************************************/
gw_U32bit 
gl_sm_inet_addr

#ifdef _ANSI_C_
    ( gw_S8bit                  *p_addr )
#else
    ( p_addr )
    gw_S8bit                    *p_addr;
#endif
    
/****************************************************************************/
{
    gw_U32bit addr;
#if defined(GL_SOLARIS) || defined(GW_LINUX) || defined (GW_QNX)
    if((gw_U32bit)GL_ERROR == (addr=inet_addr(p_addr)))
#else
    if(INADDR_NONE==(addr=inet_addr(p_addr)))
#endif
    {
        return ((gw_U32bit)GL_ERROR);
    }
    else
    {
        return addr;
    }
}



/*****************************************************************************
* Function Name    : gl_sm_inet_ntoa
* Arguments        : gw_U32bit          addr,
*                    gw_S8bit           *p_str
* Return value     : void
* Functionality    : Converts a dot notation Internet address to a 
*                    long integer.
* Functions called :
*****************************************************************************/
void 
gl_sm_inet_ntoa

#ifdef _ANSI_C_
    ( gw_U32bit                 addr, 
      gw_S8bit                  *p_str
    )
#else
    ( addr, p_str )
    gw_U32bit                   addr;
    gw_S8bit                    *p_str;
#endif
    
/****************************************************************************/
{
    struct in_addr              iaddr;

    iaddr.s_addr = addr;
#ifdef GL_VXWORKS
    inet_ntoa_b(iaddr, p_str);
#else
    strcpy(p_str, inet_ntoa(iaddr));
#endif
}



/*****************************************************************************
* Function Name    : gl_sm_gethostname
* Arguments        : gw_S8bit           *name,
*                    gw_U32bit          name_len
* Return value     : gw_S32bit
* Functionality    : Gets the string name for the host machine.
* Functions called :
*****************************************************************************/
gw_S32bit 
gl_sm_gethostname

#ifdef _ANSI_C_
    ( gw_S8bit                  *name,
      gw_U32bit                 name_len
    )
#else
    ( name, name_len )
    gw_S8bit                    *name;
    gw_U32bit                   name_len;
#endif
    
/****************************************************************************/
{
#ifdef GL_VXWORKS
    if(ERROR == gethostname(name, name_len))
#else
    if((gethostname(name, name_len)) != 0)
#endif
    {
        return GL_ERROR;
    }
    return 1;
}



/*****************************************************************************
* Function Name    : gl_sm_hostgetbyname
* Arguments        : gw_S8bit           *name
* Return value     : gw_S32bit
* Functionality    : Gets the integer IP address from the host name
*                    for the host machine.
* Functions called :
*****************************************************************************/
gw_S32bit 
gl_sm_hostgetbyname

#ifdef _ANSI_C_
    ( gw_S8bit                  *name )
#else
    ( name )
    gw_S8bit                    *name;
#endif
    
/****************************************************************************/
{
    gw_S32bit                   addr = 0;
    
#ifdef GL_VXWORKS
    addr = hostGetByName(name);
    if(addr == ERROR)
    {
        return GL_ERROR;
    }
#else
    struct hostent              *p_hostent;
    struct in_addr              *p_in_addr;

    p_hostent = (struct hostent *)gethostbyname(name);
    if(NULL == p_hostent)
    {
        return 0;
    }

    while((p_in_addr = (struct in_addr *)*(p_hostent->h_addr_list)) != NULL)
    {
#if !(defined GL_OSE) && !(defined GL_LINUX) && !(defined GL_QNX)
        addr = p_in_addr->S_un.S_addr;
#else
        addr = p_in_addr->s_addr;
#endif
        (p_hostent->h_addr_list)++;
    }
#endif
    return addr;
}

