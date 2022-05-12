/******************************************************************************
*                                    
* FILE NAME    : arDnsClient_clib_defs.h                
*                                    
* DESCRIPTION  : Contains the definitions for the literal definitions. 
*                                    
* Revision History :                            
*
*  DATE            NAME                 REFERENCE       REASON        
* ------         ------------           ---------     ----------
* 05-Feb-2009    Anuradha Gupta         None          Initial
*
* Copyright 2009, Aricent.                       
*                                    
*******************************************************************************/

#ifndef __ARDNSCLIENT_CLIB_DEFS_H__
#define __ARDNSCLIENT_CLIB_DEFS_H__

/* Prevent Name Mangling By C++ Compilers */
/* this max size is taken to cater prepernding service and protocol before
 * domain name in case of SRV query 
 */ 
#define ARDNSCL_MAX_QUERY_NAME_SIZE   150

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __ARDNSCLIENT_CLIB_DEFS_H__ */



