/******************************************************************************
*                                    
* FILE NAME    : nat_port_types.h                
*                                    
* DESCRIPTION  : Contains the definitions for the structures used in NAT Client 
*                Prototypes APIs.    
*                                    
* Revision History :                            
*
*  DATE            NAME                        REFERENCE       REASON        
* ------         ------------                  ---------     ----------
* 20-Nov-2006    Anuradha Gupta                  HLD         Initial Version
* 
* 01-Mar-2007    Krishna Ram Dhunju                          Changes made for vxworks port 
*
* Copyright 2006, Aricent                
*                                    
*******************************************************************************/



#ifndef __NAT_PORT_TYPES_H_

#define __NAT_PORT_TYPES_H_

#include "nat_types.h"
#include "nat_common_port_types.h"
#include "nat_port_defs.h"
#include "nat_port_types.h"

#ifdef NAT_PORT_WINDOWS
#include <io.h>
#endif
#if defined(NAT_PORT_WINDOWS) || defined(NAT_PORT_WINCE)
#include <stdio.h>
#include <winsock2.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#endif
#ifdef NAT_PORT_LINUX
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/types.h> 
#include <arpa/inet.h>
#include <fcntl.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/nameser.h>
#include <resolv.h>
#include <net/if.h>
#include <time.h>
#endif
#ifdef NAT_PORT_VXWORKS
#include "vxWorks.h"
#include "stdlib.h"
#include "stdio.h"
#include "stdarg.h"
#include "memLib.h"
#include "timers.h"
#include "inetLib.h"
#include "in.h"
#include "arpa/inet.h"
#include "msgQLib.h"
#include "tickLib.h"
#include "time.h"
#include "sys/times.h"
#include "sockLib.h"
#include "ctype.h"
#include "string.h"
#include "ioLib.h"
#include "ioctl.h"
#include "assert.h"
#include "hostLib.h"
#include "taskLib.h"
#include "kernelLib.h"
#include "errnoLib.h"
#include "resolvLib.h"
#include "netdb.h"
#include "types.h"
#include "mbuf.h"
#include "socket.h"
#include "socketvar.h"
#include "semaphore.h"
#include "signal.h"
#include "ramDrv.h"
#include "dosFsLib.h"
#include "ftpLib.h"
#include "ioLib.h"
#ifdef GL_IPV6
#include "netinet6/in6.h"
#include "netinet6/ip6.h"
#endif
#endif



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

#if defined(NAT_PORT_WINDOWS) || defined(NAT_PORT_LINUX) || defined(NAT_PORT_WINCE) || defined(NAT_PORT_VXWORKS)

typedef fd_set nat_fd_set;
#define NAT_PORT_FD_ZERO(x)  FD_ZERO(x)
#define NAT_PORT_FD_ISSET(x,y)  FD_ISSET(x,y)
#define NAT_PORT_FD_SET(x,y)  FD_SET(x,y)

/* --------------------------------------------------------------------
 * Type Name   :  icf_time_t
 * Description :  A typedef for time_t basic type
 *  
 * ------------------------------------------------------------------------*/
typedef time_t  nat_time_t;

#endif


/* --------------------------------------------------------------------
 * Type Name   :  nat_port_timer_data_st
 * Description :  Structure to hold the info/data required for timer porting
 *                Right now we are using Genlib's timer management for the 
 *                same.  The structure is entirely port specific and NAT lib
 *                core is not aware of it's definition
 * ------------------------------------------------------------------------*/
typedef struct
{
    nat_timer_cb_fn_t        nat_genlib_timer_cb_fn;
    nat_uint16_t             current_list_index;
    nat_uint16_t             timer_id_list[NAT_MAX_TIMER_LIST_COUNT];
}nat_port_timer_data_st;



/* --------------------------------------------------------------------
 * Type Name   :  nat_port_socket_data_st
 * Description :  Structure to hold the info/data required for socket creation
 *
 * ------------------------------------------------------------------------*/
typedef struct
{
    nat_ip_port_address_st      local_addr;
    nat_uint16_t                binding_fd;
    nat_boolean_t               is_free;
}nat_port_socket_data_st;


/* --------------------------------------------------------------------
 * Type Name   :  nat_win_ipc_info
 * Description :  Structure to hold the info/data required for send the 
 *				  to PA, MM and CFG. it also holds it's own fd
 * ------------------------------------------------------------------------*/

#if defined(NAT_PORT_WINDOWS) || defined(NAT_PORT_LINUX) || defined(NAT_PORT_WINCE) || defined(NAT_PORT_VXWORKS)

typedef struct
{
    nat_ip_port_address_st  app_addr;
	nat_uint16_t			port_mm;
	nat_uint16_t			self_fd_ipc;
	
	nat_fd_set			    win_read_fd;
	nat_fd_set			    win_write_fd;

#define NAT_WIN_MAX_NW_FDS      250
	nat_uint8_t			        act_fd_count ;
	nat_port_socket_data_st		act_socket_data[NAT_WIN_MAX_NW_FDS];

    nat_port_socket_data_st     active_sock_data;
    nat_ip_port_address_st      active_dest_addr;
    
}nat_win_ipc_info;

#endif


/* --------------------------------------------------------------------
 * Type Name   :  nat_global_port_info_st
 * Description :  Structure to hold the info/data required for porting
 *                The structure is entirely port specific and NAT lib
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
    nat_uint16_t             gl_pool_id_map[NAT_MAX_NUM_POOL][2];
    nat_port_timer_data_st   glb_timer_data;

#if defined(NAT_PORT_WINDOWS) || defined(NAT_PORT_LINUX) || defined(NAT_PORT_WINCE) || defined(NAT_PORT_VXWORKS)
	nat_win_ipc_info			win_ipc_info;
#endif

    nat_msg_list_st          internal_msg_list;
}nat_global_port_info_st;

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /*__NAT_PORT_TYPES_H_ */


