/******************************************************************************
*                                    
* FILE NAME    : arStunEncDec_defs.h                
*                                    
* DESCRIPTION  : Contains the definitions for the literal definitions. 
*                                    
* Revision History :                            
*
*  DATE            NAME                 REFERENCE       REASON        
* ------         ------------           ---------     ----------
* 05-Jan-2009    Anuradha Gupta         None          Initial
* 07-Jan-2009    Abbas Haque            None          Added for arStunEncDec
*
* Copyright 2009, Aricent.                       
*                                    
*******************************************************************************/

#ifndef __ARSTUNENCDEC_DEFS_H__
#define __ARSTUNENCDEC_DEFS_H__

#include "common_defs.h"

/* Prevent Name Mangling By C++ Compilers */

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

/* types of Stun Messages */
#define ARSTUN_BIND_REQ_MSG            0x0001
#define ARSTUN_BIND_RESP_MSG           0x0101
#define ARSTUN_BIND_ERR_RESP_MSG       0x0111


/*macro supported*/ 
#define 	ARSTUN_MAX_UNKNOWN_ATTRIBUTES    	8
#define       	ARSTUN_MAX_STR_LEN   			128
#define       	ARSTUN_MAX_MSG_LEN   			512
#define 		ARSTUN_MSG_INTEGRITY_LEN		20  
#define         ARSTUN_IPV4_FAMILY              0x01

#define   		ARSTUN_UNUSED_PARAM(x)        ((void)x)
#define           ARSTUN_ATTRIB_ADDR_LEN        	 8 
#define 		ARSTUN_ATTRIB_CHANGE_REQ_LEN		 4  
#define 		ARSTUN_ATTRIB_HDR_SIZE		 4  


#define ARSTUN_CHANGE_IPFLAG_VALUE   0x04
#define ARSTUN_CHANGE_PORTFLAG_VALUE 0x02


/* various attributes types supported */
#define ARSTUN_ATTRIB_MAPPED_ADDRESS      0x0001
#define ARSTUN_ATTRIB_RESPONSE_ADDRESS    0x0002
#define ARSTUN_ATTRIB_CHANGE_REQUEST      0x0003
#define ARSTUN_ATTRIB_SOURCE_ADDRESS      0x0004
#define ARSTUN_ATTRIB_CHANGED_ADDRESS     0x0005
#define ARSTUN_ATTRIB_USERNAME            0x0006
#define ARSTUN_ATTRIB_ERROR_CODE          0x0009
#define ARSTUN_ATTRIB_UNKNOWN_ATTRIBUTE   0x000A
#define ARSTUN_ATTRIB_REFLECTED_FROM      0x000B

/* debug trace reporting */
#define ARSTUN_PRINT     UTIL_PRINT 

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __ARSTUNENCDEC_DEFS_H__ */



