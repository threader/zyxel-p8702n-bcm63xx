/*
*
*    FUNCTION    : Header file for Generic Library - Socket Manager 
*                  Interface level prototype and data structure declarations.  
******************************************************************************
*
*    FILENAME    : gl_skm_interfaces.h
*
*    DESCRIPTION : This file contains the interface level prototype and 
*                  data structure declarations of socket manager sub-module 
*                  of the generic library implementation.
*                  This file is to be include in the application module, in
*                  order to use the supported APIs.
*
*   DATE            NAME        REFERENCE           REASON
*-----------   -------------  ---------------  ----------------------
*
* 29/11/2001 Ravi Jindal        SPR-2788        Packaging of Genlib-Code.
* 
* 06/12/2001 Prashant Jain      SPR-3363        Compilation with CC.
*
* 04/04/2002 Ravi Jindal        SPR-3959        Enhancements for Rel-1.0.
*
* 11/11/2002 Ravi Jindal        SPR-5123        Provide Socket APIs for Solaris.
*
* 29/11/2002 Sudhir Gupta       SPR-5123        Provide Socket APIs for WinNT,
*                                               Linux.
*
* 23/12/2003 Sudhanshu Garg     SPR-6265        Support for SCTP in socket
*                                               manager
* 12/07/2004 Navneet Gupta      SPR-6946        QNX Porting
*
* COPYRIGHT , Aricent, 2006
*****************************************************************************/

#ifndef _GL_SKM_INTERFACES_H_
#define _GL_SKM_INTERFACES_H_

#include "gl_config.h"
#include "gl_types.h"
#include "gl_error.h"
#include "gl_os_port.h"
#include "gl_gscim_interfaces.h"
#include "gl_ulm_interfaces.h"

#include "gl_htm_interfaces.h"
#include "gl_mm_interfaces.h"

#include "gw_pragma_defs.h"

#ifdef __cplusplus
extern "C"
{
#endif

/*------------------------------------------------------------------------------
 * Key types to be used for bitmask in the structure gl_sm_udp_tpt_info_st
 *----------------------------------------------------------------------------*/
#define GL_SM_UDP_TPT_KEY_IP       0x01
#define GL_SM_UDP_TPT_KEY_DNAME    0x02

/*----------------------------------------------------------------------------
 * Data Structures used for - Socket Manager
 *---------------------------------------------------------------------------*/
#define GL_SM_UDP_SCTP_STR_KEY      90

/*----------------------------------------------------------------------------
 * Data Structures used for - Socket Manager
 *---------------------------------------------------------------------------*/
    
#define GL_SKM_MAX_SOCKET_FD_BIT_ARRAY_INDEX           (FD_SETSIZE/32)+1 

typedef struct gl_fd_info_s
{
    /* 
     * The number of socket bits set corresponding to fd_bits array index of fd_set.
     */ 
    gw_U8bit                    num_of_fd_bits_set;
    /*
     * Next index of fd_bits array of fd_set, index are linked in decsending order
     * based on number of socket bits set for the index.
     */ 
    gw_U16bit                   next_index;
}gl_fd_info_st;
    
typedef  struct gl_fd_set_s
{
    fd_set                      read_fd;
    gl_fd_info_st               a_fd_bit_index[GL_SKM_MAX_SOCKET_FD_BIT_ARRAY_INDEX];
    gw_U16bit                   first_index;
    gw_U16bit                   last_index;
}gl_fd_set_t;




typedef enum gl_sm_tpt_type_e
{
    GL_SM_TLM_TPT_TYPE_UDP = 0,
    GL_SM_TLM_TPT_TYPE_TCP,
    GL_SM_TLM_TPT_TYPE_SCTP_UDP,
    GL_SM_TLM_TPT_TYPE_SCTP_TCP,
    GL_SM_TLM_TPT_TYPE_MTP,
    GL_SM_TLM_TPT_TYPE_ATM
}gl_sm_tpt_type_et;


typedef enum gl_sm_lnk_type_e
{
    GL_SM_TLM_LNK_TYPE_UDP_TCP = 0,
    GL_SM_TLM_LNK_TYPE_SCTP_UDP,
    GL_SM_TLM_LNK_TYPE_SCTP_TCP,
    GL_SM_TLM_LNK_TYPE_MTP,
    GL_SM_TLM_LNK_TYPE_ATM
}gl_sm_lnk_type_et;



typedef struct gl_sm_tpt_lnk_mgmt_s
{
    gl_sm_tpt_type_et           tpt_type;
}gl_sm_tpt_lnk_mgmt_st;


typedef struct gl_sm_tpt_lnk_id_data_s
{
    gl_sm_tpt_lnk_mgmt_st       tpt_lnk_mgmt;
    gl_pvoid_t                  p_user_data;
}gl_sm_tpt_lnk_id_data_st;

typedef struct gl_sm_tcp_tpt_info_s
{
    gw_S32bit                   socket_id;
}gl_sm_tcp_tpt_info_st;

typedef struct gl_sm_udp_tpt_info_s
{
    gw_U8bit                    bitmask;
    gw_S32bit                   socket_id;
    gw_U32bit                   remote_ip_addr;
    gw_U8bit                    remote_dname[64];
    gw_U16bit                   remote_port_number;
}gl_sm_udp_tpt_info_st;


typedef union gl_sm_tlm_tpt_lnk_key_u
{
    gl_sm_tcp_tpt_info_st       tcp_tpt_info;
    gl_sm_udp_tpt_info_st       udp_tpt_info;
}gl_sm_tlm_tpt_lnk_key_ut;


#if defined(GL_SOLARIS) || defined(GL_VXWORKS) || defined(GL_WINNT)
typedef enum
{
    GL_SM_AF_NS         = AF_NS,
    GL_SM_AF_IMPLINK    = AF_IMPLINK,
    GL_SM_AF_UNIX       = AF_UNIX,
    GL_SM_AF_INET       = AF_INET
}gl_sm_addr_family_et;
#endif

#if defined(GL_LINUX) || defined(GL_QNX)
typedef enum
{
    GL_SM_AF_UNIX       = AF_UNIX,
    GL_SM_AF_INET       = AF_INET
}gl_sm_addr_family_et;
#endif

#ifdef GL_OSE
typedef enum
{
    GL_SM_AF_INET       = AF_INET,
    GL_SM_AF_IMPLINK    = AF_LINK
}gl_sm_addr_family_et;
#endif


typedef enum
{
#ifndef GL_OSE
   GL_SM_SOCK_RDM       = SOCK_RDM,
   /*
    * Promiscous mode interface.
    */
   GL_SM_SOCK_SEQPACKET = SOCK_SEQPACKET,
#else
   /*
    * Promiscous mode interface.
    */
   GL_SM_SOCK_SEQPACKET= SOCK_PACKET,
#endif
   /*
    * Stream socket.
    */
   GL_SM_SOCK_STREAM    = SOCK_STREAM,
   /*
    * Datagram socket.
    */
   GL_SM_SOCK_DGRAM     = SOCK_DGRAM,
   /*
    * Raw-protocol interface.
    */
   GL_SM_SOCK_RAW       = SOCK_RAW
      
}gl_sm_sock_type_et;


typedef enum
{
    GL_SM_IPPROTO_IP    = IPPROTO_IP,
    GL_SM_IPPROTO_UDP   = IPPROTO_UDP,
    GL_SM_IPPROTO_TCP   = IPPROTO_TCP,
    GL_SM_IPPROTO_ICMP  = IPPROTO_ICMP,
    GL_SM_IPPROTO_RAW   = IPPROTO_RAW
}gl_sm_protocol_et;

typedef enum
{
    GL_SM_LVL_SOL_SOCKET= SOL_SOCKET
}gl_sm_level_et;


#ifndef GL_OSE

typedef enum
{
#if !defined(GL_LINUX) && !defined(GL_QNX)
    GL_SM_SO_ACCEPTCONN = SO_ACCEPTCONN,
    GL_SM_SO_USELOOPBACK= SO_USELOOPBACK,
#endif    
    GL_SM_SO_DEBUG      = SO_DEBUG,
    GL_SM_SO_REUSEADDR  = SO_REUSEADDR,
    GL_SM_SO_KEEPALIVE  = SO_KEEPALIVE,
    GL_SM_SO_DONTROUTE  = SO_DONTROUTE,
    GL_SM_SO_BROADCAST  = SO_BROADCAST,
    GL_SM_SO_LINGER     = SO_LINGER,
    GL_SM_SO_OOBINLINE  = SO_OOBINLINE,
    GL_SM_SO_SNDBUF     = SO_SNDBUF,
    GL_SM_SO_RCVBUF     = SO_RCVBUF,
    GL_SM_SO_SNDLOWAT   = SO_SNDLOWAT,
    GL_SM_SO_RCVLOWAT   = SO_RCVLOWAT,
    GL_SM_SO_SNDTIMEO   = SO_SNDTIMEO,
    GL_SM_SO_RCVTIMEO   = SO_RCVTIMEO,
    GL_SM_SO_ERROR      = SO_ERROR,
    GL_SM_SO_TYPE       = SO_TYPE
}gl_sm_optname_et;

#else /* GL_OSE */

typedef enum
{
    GL_SM_SO_DEBUG      = SO_DEBUG,
    GL_SM_SO_REUSEADDR  = SO_REUSEADDR,
    GL_SM_SO_KEEPALIVE  = SO_KEEPALIVE,
    GL_SM_SO_DONTROUTE  = SO_DONTROUTE,
    GL_SM_SO_BROADCAST  = SO_BROADCAST,
    GL_SM_SO_SNDBUF     = SO_SNDBUF,
    GL_SM_SO_RCVBUF     = SO_RCVBUF,
    GL_SM_SO_ERROR      = SO_ERROR,
    GL_SM_SO_TYPE       = SO_TYPE
}gl_sm_optname_et;

#endif



typedef enum 
{
#if defined(GL_WINNT) || defined(GL_VXWORKS)
    GL_SM_SIOCATMARK = SIOCATMARK,
#endif
    GL_SM_FIONREAD   = (gw_S32bit)FIONREAD,
    GL_SM_FIONBIO    = (gw_S32bit)FIONBIO
}gl_sm_ioctl_request_et;



/*----------------------------------------------------------------------------
 * EXTERNAL INTERFACE APIs - Socket Manager
 *---------------------------------------------------------------------------*/

extern gl_return_t
gl_sm_init( void );

extern gw_S32bit 
gl_sm_create_socket
            
    _ARGS_( (   gl_sm_addr_family_et    family,
                gl_sm_sock_type_et      type,
                gl_sm_protocol_et       protocol    ) );
            
                  

extern gl_return_t 
gl_sm_bind_socket
             
    _ARGS_( (   gw_S32bit               sockid,
                gw_S8bit                *ip_addr,
                gw_U16bit               portno     ) );
            
                    

extern gl_return_t
gl_sm_connect_socket
            
    _ARGS_( (   gw_S32bit               sockid,
                gw_S8bit                *ip_addr,
                gw_U16bit               portno      ) ); 
            
              
extern gl_return_t 
gl_sm_listen_socket

    _ARGS_( (   gw_S32bit               sockid,
                gw_S32bit               backlog     ) );
    


extern gw_S32bit 
gl_sm_accept_socket

    _ARGS_( (   gw_S32bit               sockid,
                gw_S8bit                *p_peer_ip_addr, 
                gw_U16bit               *p_peer_portno  ) );
    



extern gl_return_t 
gl_sm_ioctl_socket

    _ARGS_( (   gw_S32bit               sockid,
                gl_sm_ioctl_request_et  ioctl_request,
                gw_S32bit               *p_ioctl_arg    ) );
    



extern gw_S32bit 
gl_sm_setup_tcp_server

    _ARGS_( (   gw_S8bit                *p_server_ip_addr, 
                gw_U16bit               server_portno   ) );
    


extern gw_S32bit 
gl_sm_setup_tcp_client

    _ARGS_( (   gw_S8bit                *p_server_ip_addr, 
                gw_U16bit               server_portno   ) );



extern gw_S32bit 
gl_sm_setup_udp_server_or_client

    _ARGS_( ( gw_S8bit                  *p_ip_addr,
              gw_U16bit                 portno          ) );
    
    
    

extern gw_S32bit 
gl_sm_recvfrom
            
    _ARGS_( (   gw_S32bit               sockid,
                gw_S8bit                *buff,
                gw_S32bit               nbytes,
                gw_S32bit               flags, 
                gl_pvoid_t              p_srcaddr, 
                gw_S32bit               *p_addrlen   ) );
            
                   

extern gw_S32bit 
gl_sm_sendto
            
    _ARGS_( (   gw_S32bit               sockid, 
                gw_S8bit                *buff, 
                gw_S32bit               nbytes, 
                gw_S32bit               flags, 
                gl_pvoid_t              p_destaddr,
                gw_S32bit               addrlen     ) );
            
                   

extern gw_S32bit 
gl_sm_recv
            
    _ARGS_( (   gw_S32bit               sockid,
                gw_S8bit                *buff,
                gw_S32bit               nbytes,
                gw_S32bit               flags      ) );
            
                  

extern gw_S32bit 
gl_sm_send 
            
    _ARGS_( (   gw_S32bit               sockid,
                gw_S8bit                *buff, 
                gw_S32bit               nbytes,
                gw_S32bit               flags      ) );
            

extern gl_return_t 
gl_sm_setsockopt
            
    _ARGS_( (   gw_S32bit               sockid,
                gl_sm_level_et          level,
                gl_sm_optname_et        optname,
                gw_S8bit                *p_optval,
                gw_S32bit               optlen     ) );
            

extern gl_return_t 
gl_sm_getsockopt

    _ARGS_( (   gw_S32bit               sockid,
                gl_sm_level_et          level,
                gl_sm_optname_et        optname, 
                gw_S8bit                *p_optval, 
                gw_S32bit               *p_optlen   ) );
    

extern gl_return_t 
gl_sm_getsockname

    _ARGS_( (   gw_S32bit                 sockid,
                gl_pvoid_t                p_localaddr,
                gw_S32bit                 *p_localaddrlen   ) );
    

extern gl_return_t 
gl_sm_getpeername

    _ARGS_( (   gw_S32bit                 sockid,
                gl_pvoid_t                p_peeraddr,
                gw_S32bit                 *p_peeraddrlen    ) );
    

extern gl_return_t 
gl_sm_close_socket
            
    _ARGS_( (   gw_S32bit               sockid   ) );
            
                

extern gw_U32bit 
gl_sm_htonl
            
    _ARGS_( (   gw_U32bit               hostlong  ) );
            
                  

extern gw_U16bit
gl_sm_htons
            
    _ARGS_( (   gw_U16bit               hostshort  ) );
            
                  

extern gw_U32bit
gl_sm_ntohl
            
    _ARGS_( (   gw_U32bit               netlong    ) );
            
                  

extern gw_U16bit
gl_sm_ntohs
            
    _ARGS_( (   gw_U16bit               netshort   ) );
            
                  

extern gw_U32bit 
gl_sm_inet_addr
            
    _ARGS_( (   gw_S8bit                *p_addr   ) );
            
                  

extern void 
gl_sm_inet_ntoa
            
    _ARGS_( (   gw_U32bit               addr, 
                gw_S8bit                *p_str   ) );
            
                  

extern gw_S32bit 
gl_sm_gethostname
            
    _ARGS_( (   gw_S8bit                *name,
                gw_U32bit               name_len   ) );
            
                  

extern gw_S32bit 
gl_sm_hostgetbyname
            
    _ARGS_( (   gw_S8bit                *name    ) );
            



gl_return_t
gl_sm_tlm_init

    _ARGS_(( gw_U32bit                 num_of_tpt_link_ids, 
             gw_U32bit                 num_of_tpt_link_keys, 
             gl_sm_tpt_type_et         transport_type, 
             gl_error_t                *p_ecode            ));


gl_return_t
gl_sm_tlm_set_tpt_lnk_id_data

    _ARGS_(( gl_sm_lnk_type_et         lnk_type, 
             gw_U32bit                 tpt_lnk_id, 
             gl_sm_tpt_lnk_id_data_st  *p_tpt_lnk_id_data, 
             gl_error_t                *p_ecode            ));

gl_return_t
gl_sm_tlm_get_tpt_lnk_id_data

    _ARGS_(( gl_sm_lnk_type_et         lnk_type, 
             gw_U32bit                 tpt_lnk_id, 
             gl_sm_tpt_lnk_id_data_st  **p_p_tpt_lnk_id_data, 
             gl_error_t                *p_ecode            ));


gl_return_t
gl_sm_tlm_delete_tpt_lnk_id_data

    _ARGS_(( gl_sm_lnk_type_et         lnk_type, 
             gw_U32bit                 tpt_lnk_id, 
             gl_sm_tpt_lnk_id_data_st  **p_p_tpt_lnk_id_data, 
             gl_error_t                *p_ecode            ));



gl_return_t
gl_sm_tlm_set_tpt_link_key_map

    _ARGS_(( gl_sm_tpt_type_et         transport_type, 
             gl_sm_tpt_lnk_id_data_st  *p_tpt_lnk_id_data, 
             gl_sm_tlm_tpt_lnk_key_ut  *p_tpt_lnk_key,
             gw_U32bit                 su_lnk_id,
             gl_pvoid_t                p_su_lnk_id_data,
             gl_error_t                *p_ecode            ));


gl_return_t
gl_sm_tlm_get_tpt_link_key_map

    _ARGS_(( gl_sm_tpt_type_et         transport_type, 
             gl_sm_tpt_lnk_id_data_st  *p_tpt_lnk_id_data, 
             gl_sm_tlm_tpt_lnk_key_ut  *p_tpt_lnk_key,
             gw_U32bit                 *p_su_lnk_id,
             gl_pvoid_t                *p_p_su_lnk_id_data,
             gl_error_t                *p_ecode            ));



gl_return_t
gl_sm_tlm_delete_tpt_link_key_map

    _ARGS_(( gl_sm_tpt_type_et         transport_type, 
             gl_sm_tpt_lnk_id_data_st  *p_tpt_lnk_id_data, 
             gl_sm_tlm_tpt_lnk_key_ut  *p_tpt_lnk_key,
             gw_U32bit                 *p_su_lnk_id,
             gl_pvoid_t                *p_p_su_lnk_id_data,
             gl_error_t                *p_ecode            ));


/*
 * Internal to Socket Manager 
 */

typedef struct gl_sm_tcp_udp_lnk_id_info_s
{
    gl_sm_tpt_lnk_id_data_st    *p_tpt_lnk_id_data;
    /*
     * Following field is used in WinNT
     */
    gw_S32bit                   link_id;
}gl_sm_tcp_udp_lnk_id_info_t;

typedef gl_sm_tcp_udp_lnk_id_info_t gl_sm_sctp_lnk_id_info_t;

typedef struct gl_sm_tcp_lnk_map_info_s
{
    gw_U32bit                   su_lnk_id;
    /*
     * Following field is used in WinNT
     */
    gw_S32bit                   link_id;
    gl_pvoid_t                  p_su_lnk_id_data;
}gl_sm_tcp_lnk_map_info_t;

typedef gl_sm_tcp_lnk_map_info_t gl_sm_sctp_lnk_map_info_t;

typedef struct gl_sm_udp_lnk_map_info_s
{
    gl_hash_table_t             hash_table_id;
}gl_sm_udp_lnk_map_info_t;

typedef gl_sm_udp_lnk_map_info_t gl_sm_sctp_listen_lnk_map_info_t;
typedef struct gl_sm_bit_val_s
{
    gw_U8bit                    num_of_bits;
    gw_U8bit                    a_bit_val[8];
}gl_sm_bit_val_t;

extern gl_sm_bit_val_t g_a_bit_values[256];


extern gl_return_t
gl_sm_htm_free_func

    _ARGS_(( gw_U32bit                 task_id, 
             gl_pvoid_t                p_elem_buf_ptr,
             gl_error_t                *p_ecode         ));

extern gl_return_t
gl_sm_convert_udp_tpt_key_to_string_key
    
    _ARGS_(( gw_U8bit                  *p_str_key, 
             gl_sm_tlm_tpt_lnk_key_ut  *p_tpt_lnk_key,
             gl_sm_tpt_type_et         tpt_type,
             gl_error_t                *p_ecode         ));

extern void
gl_sm_slm_fd_clear

    _ARGS_(( gl_fd_set_t               *p_rd_fd,
             gw_S32bit                 a_sock_id[],
             gw_U32bit                 num           ));

extern void
gl_sm_slm_fd_set

    _ARGS_(( gl_fd_set_t               *p_rd_fd,
             gw_S32bit                 a_sock_id[],
             gw_U32bit                 num           ));

extern void
gl_sm_slm_fd_get

    _ARGS_(( gl_fd_set_t               *p_rd_fd,
             gw_S32bit                 a_sock_id[],
             gw_U32bit                 *p_num           ));


/*
 * Socket FD Macros for optimization.
 * User should call select with reference of p_rd_fd->read_fd member,
 * as Select system call expect the address of that variable.
 */ 

#define GL_SM_SLM_FD_ZERO(p_rd_fd)       { \
    gl_mm_memset((gw_U8bit *)(p_rd_fd), 0, sizeof(*(p_rd_fd))); \
}


#if defined (GW_SOLARIS) || defined (GW_LINUX) || defined(GW_QNX) || (defined GW_VXWORKS)
    #define GL_SM_SLM_FD_CLR(p_rd_fd, a_sock_id, num) \
        gl_sm_slm_fd_clear(p_rd_fd, a_sock_id, num); 
#endif
#ifdef GW_WINNT
    #define GL_SM_SLM_FD_CLR(p_rd_fd, a_sock_id, num) \
    { \
        gw_S32bit       loop_index; \
        for( loop_index = 0; loop_index < (num); loop_index++ ) \
        { \
            FD_CLR((gw_U32bit)((a_sock_id)[loop_index]), &((p_rd_fd)->read_fd)); \
        } \
    }
#endif


#if defined (GW_SOLARIS) || defined (GW_LINUX) || defined(GW_QNX) || (defined GW_VXWORKS)
    #define GL_SM_SLM_FD_SET(p_rd_fd, a_sock_id, num) \
        gl_sm_slm_fd_set(p_rd_fd, a_sock_id, num);
#endif
#ifdef GW_WINNT
    #define GL_SM_SLM_FD_SET(p_rd_fd, a_sock_id, num) \
    { \
        gw_S32bit       loop_index; \
        for( loop_index = 0; loop_index < (num); loop_index++ ) \
        { \
            FD_SET((gw_U32bit)((a_sock_id)[loop_index]), &((p_rd_fd)->read_fd)); \
        } \
    }
#endif


#if defined (GW_SOLARIS) || defined (GW_LINUX) || defined(GW_QNX) || (defined GW_VXWORKS)
    #define GL_SM_SLM_FD_GET(p_rd_fd, a_sock_id, p_num) \
        gl_sm_slm_fd_get(p_rd_fd, a_sock_id, p_num);
#endif
#ifdef GW_WINNT
    #define GL_SM_SLM_FD_GET(p_rd_fd, a_sock_id, p_num) \
    { \
        gw_U32bit       loop_index; \
        for( loop_index = 0; loop_index <  (p_rd_fd)->read_fd.fd_count; \
                loop_index++ ) \
        { \
            (a_sock_id)[loop_index] = (p_rd_fd)->read_fd.fd_array[loop_index]; \
        } \
        *(p_num) =  (p_rd_fd)->read_fd.fd_count;\
    }
#endif


#ifdef __cplusplus
}
#endif
                  
#include "gw_pragma_undefs.h"

#endif
