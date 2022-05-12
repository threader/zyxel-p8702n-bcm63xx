/******************************************************************************
*                                    
* FILE NAME    : common_defs.h                
*                                    
* DESCRIPTION  : Contains the definitions for the common macros and 
*                definitions. 
*                                    
* Revision History :                            
*
*  DATE            NAME                 REFERENCE       REASON        
* ------         ------------           ---------     ----------
* 03-Dec-2008    Anuradha Gupta         None          Initial
* 04-Dec-2008    Anuradha Gupta         LLD           Start of Code
*
* Copyright 2008, Aricent.                       
*                                    
*******************************************************************************/

#ifndef __COMMON_DEFS_H__
#define __COMMON_DEFS_H__

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#if defined(UTIL_PORT_LINUX) || defined(UTIL_PORT_VXWORKS)
#include <netinet/in.h>
#endif

/* Prevent Name Mangling By C++ Compilers */

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */
    
#ifdef UTIL_TRACE_ENABLE
#if defined(UTIL_PORT_LINUX) || defined(UTIL_PORT_VXWORKS)
#define UTIL_PRINT(str) printf str
#else
#define UTIL_PRINT(str)   
#endif
#else
#define UTIL_PRINT(str) 
#endif

#define UTIL_SUCCESS   1
#define UTIL_FAILURE   0

#define UTIL_TRUE      1
#define UTIL_FALSE     0

#define UTIL_NULL      NULL
#define UTIL_INIT_VAL  0

#define UTIL_SIZEOF    sizeof
#define UTIL_MEMCPY	   memcpy
#define UTIL_MEMSET	   memset

#if defined(UTIL_PORT_LINUX) || defined(UTIL_PORT_VXWORKS)
#define UTIL_HTONS	htons
#define UTIL_HTONL	htonl
#define UTIL_NTOHS	ntohs
#define UTIL_NTOHL	ntohl
#endif   

#define IN  const
#define OUT
#define INOUT


#define UTIL_MAX_NON_PRINTABLE_CHARACTER  0x1f 

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __COMMON_DEFS_H__ */



