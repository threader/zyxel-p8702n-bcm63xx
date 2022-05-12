/******************************************************************************
*                                    
* FILE NAME    : arDnsClient_queryh_defs.h                
*                                    
* DESCRIPTION  : Contains the definitions for the literal definitions. 
*                                    
* Revision History :                            
*
*  DATE            NAME                 REFERENCE       REASON        
* ------         ------------           ---------     ----------
* 05-Feb-2009    Anuradha Gupta         None          Initial
* 09-Feb-2009    Shweta Jain            LLD           arDnsClient 1.0  
*
* Copyright 2009, Aricent.                       
*                                    
*******************************************************************************/

#ifndef __ARDNSCLIENT_QUERYH_DEFS_H__
#define __ARDNSCLIENT_QUERYH_DEFS_H__

/* Prevent Name Mangling By C++ Compilers */

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

/* macro supported */
#define         ARDNSCL_DNS_RESPONSE_HEADER_SIZE         12 
#define         ARDNSCL_QUERY_TYPE_SIZE                  2
#define         ARDNSCL_QUERY_CLASS_SIZE                 2
#define         ARDNSCL_IPV4_ADDR_LEN                    4
#define         ARDNSCL_IPV6_ADDR_LEN                    16
#define         ARDNSCL_MAX_IPV6_STR_LEN                 46
#define         ARDNSCL_MAX_IPV4_STR_LEN                 16
 
#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __ARDNSCLIENT_QUERYH_DEFS_H__ */



