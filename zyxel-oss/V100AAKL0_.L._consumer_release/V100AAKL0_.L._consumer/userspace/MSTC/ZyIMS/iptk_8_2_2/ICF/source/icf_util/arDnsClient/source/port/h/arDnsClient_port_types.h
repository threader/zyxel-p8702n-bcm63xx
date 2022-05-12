/******************************************************************************
*                                    
* FILE NAME    : arDnsClient_port_types.h                
*                                    
* DESCRIPTION  : Contains the definitions for the data types. 
*                                    
* Revision History :                            
*
*  DATE            NAME                 REFERENCE       REASON        
* ------         ------------           ---------     ----------
* 05-Feb-2009    Anuradha Gupta         None          Initial
* 06-Feb-2009    Anuradha Gupta         Porting Doc   Basic data types
*
* Copyright 2009, Aricent.                       
*                                    
*******************************************************************************/

#ifndef __ARDNSCLIENT_PORT_TYPES_H__
#define __ARDNSCLIENT_PORT_TYPES_H__

#include "arDnsClient_port_defs.h"

/* Prevent Name Mangling By C++ Compilers */

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

/*
---------------------------- BASIC TYPES -------------------------------------
*/

/*-----------------------------------------------------------------------------
 * 
 * typedef Name: dnscl_void_t
 * 
 * Description: Represent the type void.
 *
 *---------------------------------------------------------------------------*/
typedef void dnscl_void_t;

/*-----------------------------------------------------------------------------
 * 
 * typedef Name: dnscl_uint8_t
 *
 * Description: This data type represents a 1 byte unsigned integer.
 *
 *---------------------------------------------------------------------------*/
typedef unsigned char dnscl_uint8_t;

/*-----------------------------------------------------------------------------
 * 
 * typedef Name: dnscl_boolean_t
 * 
 * Description: Represent boolean vales
 *
 *---------------------------------------------------------------------------*/
typedef dnscl_uint8_t dnscl_boolean_t;

/*-----------------------------------------------------------------------------
 * 
 * typedef Name: dnscl_return_t
 * 
 * Description: Represent the  execution status values return by a function.
 *              This type is used in function interface and not in message based
 *              interface.
 *
 *---------------------------------------------------------------------------*/
typedef dnscl_uint8_t dnscl_return_t;

/*-----------------------------------------------------------------------------
 * 
 * typedef Name: dnscl_result_t
 * 
 * Description: Represent the result of the API .
 *
 *---------------------------------------------------------------------------*/
typedef dnscl_uint8_t dnscl_result_t;

/*-----------------------------------------------------------------------------
 * 
 * typedef Name: dnscl_uint16_t
 * 
 * Description: This data type represents a 2 byte unsigned integer. 
 * 
 *---------------------------------------------------------------------------*/

typedef unsigned short dnscl_uint16_t;

/*-----------------------------------------------------------------------------
 * 
 * typedef Name: dnscl_uint8_t
 * 
 * Description: This data type represents a 1 byte unsigned error code. 
 * 
 *---------------------------------------------------------------------------*/
typedef dnscl_uint8_t           dnscl_error_t;

/*-----------------------------------------------------------------------------
 * 
 * typedef Name: dnscl_uint8_t
 * 
 * Description: This data type represents a 1 byte unsigned query type. 
 * 
 *---------------------------------------------------------------------------*/
typedef dnscl_uint8_t           dnscl_query_t;

/*-----------------------------------------------------------------------------
 * 
 * typedef Name: dnscl_uint32_t
 *
 * Description: This data type represents a 4 byte unsigned long integer.
 *  
 *---------------------------------------------------------------------------*/
typedef unsigned int dnscl_uint32_t;

/*-----------------------------------------------------------------------------
 * 
 * typedef Name: dnscl_int8_t
 * 
 * Description: This data type represents a 1 byte signed integer
 *
 *---------------------------------------------------------------------------*/
typedef char dnscl_int8_t;

/*-----------------------------------------------------------------------------
 * 
 * typedef Name: dnscl_int16_t
 *
 * Description: This data type represents a 2 byte-signed integer.
 * 
 *---------------------------------------------------------------------------*/
typedef signed short dnscl_int16_t; 

/*-----------------------------------------------------------------------------
 * 
 * typedef Name: dnscl_int32_t
 *
 * Description: This data type represents a 4 byte signed long integer
 *
 *---------------------------------------------------------------------------*/
typedef signed int dnscl_int32_t;

/*-----------------------------------------------------------------------------
 * 
 * typedef Name: dnscl_timeval_st
 *
 * Description: This is the timeval data structure
 *
 *---------------------------------------------------------------------------*/
typedef struct timeval                dnscl_timeval_st;

/*-----------------------------------------------------------------------------
 * 
 * typedef Name: dnscl_buff_ptr
 *
 * Description: This is buffer pointer for DNS response
 *
 *---------------------------------------------------------------------------*/
#if defined (ARDNSCL_PORT_LINUX) || defined (ARDNSCL_PORT_VXWORKS)
typedef dnscl_uint8_t *                dnscl_buff_ptr;
#elif defined (ARDNSCL_PORT_WINDOWS)
typedef PDNS_RECORD                   dnscl_buff_ptr;
#endif

/*---------------------------------------------------------------------------
 * 
 * typedef Name: dnscl_ip_addr_st
 *
 * Description: This is IPv4 address data structure
 *
 *---------------------------------------------------------------------------*/
#if defined (ARDNSCL_PORT_LINUX) || defined (ARDNSCL_PORT_VXWORKS)
typedef  struct in_addr       dnscl_ip_addr_st;        
#elif defined (ARDNSCL_PORT_WINDOWS)
typedef   IN_ADDR       dnscl_ip_addr_st; 
#endif


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __ARDNSCLIENT_PORT_TYPES_H__ */



