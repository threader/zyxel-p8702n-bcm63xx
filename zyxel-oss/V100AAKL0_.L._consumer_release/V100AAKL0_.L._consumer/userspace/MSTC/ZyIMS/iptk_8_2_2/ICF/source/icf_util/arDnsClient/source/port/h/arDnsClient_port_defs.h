/******************************************************************************
*                                    
* FILE NAME    : arDnsClient_port_defs.h                
*                                    
* DESCRIPTION  : Contains the definitions for the literal definitions. 
*                                    
* Revision History :                            
*
*  DATE            NAME                 REFERENCE       REASON        
* ------         ------------           ---------     ----------
* 05-Feb-2009    Anuradha Gupta         None          Initial
* 06-Feb-2009    Anuradha Gupta         Porting Doc   Porting definitions
*
* Copyright 2009, Aricent.                       
*                                    
*******************************************************************************/

#ifndef __ARDNSCLIENT_PORT_DEFS_H__
#define __ARDNSCLIENT_PORT_DEFS_H__

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#if defined (ARDNSCL_PORT_LINUX)
#include "resolv.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#elif defined (ARDNSCL_PORT_VXWORKS)
#include "netinet/in.h"
#include "arpa/inet.h"
#include "resolvLib.h"
#include "resolv/nameser.h"
#include "resolv/resolv.h"
#include "inetLib.h"
#include "sys/types.h"
#include "sys/socket.h"
#include "sockLib.h"
#include "selectLib.h"
#include "unistd.h"
#elif defined (ARDNSCL_PORT_WINDOWS)
#include <WinSock2.h>
#include <WinDNS.h>
#include <winerror.h>
#include <Windows.h>
#include <winerror.h>
#endif
/* Prevent Name Mangling By C++ Compilers */

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

#define ARDNSCL_SUCCESS   1
#define ARDNSCL_FAILURE   0

#define ARDNSCL_TRUE      1
#define ARDNSCL_FALSE     0

#define ARDNSCL_NULL      NULL
#define ARDNSCL_INIT_VAL  0


#ifdef ARDNSCL_TRACE_ENABLE
#define ARDNSCL_PRINT(pGlb_data,str) \
    {\
        if(ARDNSCL_TRUE == pGlb_data->dns_client_data.enable_trace)\
        {\
            printf str;\
        }\
    }
#else
#define ARDNSCL_PRINT(pGlb_data, str)   
#endif

#ifdef ARDNSCL_TRACE_ENABLE
#define ARDNSCL_CLIB_PRINT(pGlb_data,str) \
    {\
        if(ARDNSCL_TRUE == pGlb_data->clib_data.enable_trace)\
        {\
            printf str;\
        }\
    }
#else
#define ARDNSCL_CLIB_PRINT(pGlb_data, str)   
#endif


#define ARDNSCL_SIZEOF    sizeof
#define ARDNSCL_MEMCPY	   memcpy
/*#define ARDNSCL_MEMGET	   malloc*/
    
#define ARDNSCL_MEMGET(ptr,ptr_size)\
    {\
        ptr =  malloc(ptr_size);\
        if(ARDNSCL_NULL != ptr) \
        {\
            memset(ptr,0,ptr_size);\
        }\
    }
    
#define ARDNSCL_MEMFREE	   free
#define ARDNSCL_MEMSET	   memset
#define ARDNSCL_STRCPY     strcpy
#define ARDNSCL_STRNCPY    strncpy
#define ARDNSCL_STRCAT     strcat
#define ARDNSCL_STRNCAT    strncat
#define ARDNSCL_STRCMP     strcmp

#define ARDNSCL_HTONS	htons
#define ARDNSCL_HTONL	htonl
#define ARDNSCL_NTOHS	ntohs
#define ARDNSCL_NTOHL	ntohl

#ifdef IN
#undef IN
#endif

#define IN  const
#define OUT
#define INOUT

#define  ARDNSCL_STRLEN     strlen
#define ARDNSCL_SPRINTF     sprintf 
#define ARDNSCL_INET_NTOA   inet_ntoa

/* added to remove klockworks warning*/ 
#define ARDNSCL_SNPRINTF    snprintf  

#if defined (ARDNSCL_PORT_LINUX) || defined (ARDNSCL_PORT_VXWORKS)
#define ARDNSCL_INET_NTOP   inet_ntop
#endif

#if defined (ARDNSCL_PORT_LINUX) 
#define ARDNSCL_DN_EXPAND   dn_expand
#elif defined (ARDNSCL_PORT_VXWORKS)
#define ARDNSCL_DN_EXPAND   resolvDNExpand 
#endif

#ifdef ARDNSCL_PORT_WINDOWS
#define ARDNSCL_FREE_RECORD_LIST   DnsRecordListFree
#endif

#define   ARDNSCL_TRANSPORT_TYPE_UDP     1  
#define   ARDNSCL_TRANSPORT_TYPE_TCP     2 

/* default trace value at the time of system startup */
#define ARDNSCL_DEFAULT_TRACE_VAL      ARDNSCL_TRUE

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __ARDNSCLIENT_PORT_DEFS_H__ */

