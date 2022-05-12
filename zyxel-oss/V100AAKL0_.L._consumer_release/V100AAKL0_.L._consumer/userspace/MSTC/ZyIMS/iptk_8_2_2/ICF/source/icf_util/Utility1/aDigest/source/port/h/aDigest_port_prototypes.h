/******************************************************************************
*                                    
* FILE NAME    : aDigest_port_prototypes.h                
*                                    
* DESCRIPTION  : Contains the definitions for the porting interfaces. 
*                                    
* Revision History :                            
*
*  DATE            NAME                 REFERENCE       REASON        
* ------         ------------           ---------     ----------
* 03-Dec-2008    Anuradha Gupta         None          Initial
* 04-Dec-2008    Alok Tiwari            LLD           aDigest 1.0          
*
* Copyright 2008, Aricent.                       
*                                    
*******************************************************************************/

#ifndef __ADIGEST_PORT_PROTOTYPES_H__
#define __ADIGEST_PORT_PROTOTYPES_H__

#include "common_types.h"
#include "aDigest_port_types.h"



/* Prevent Name Mangling By C++ Compilers */

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

/*******************************************************************************
 * FUNCTION: aDigest_port_MD5Init
 * 
 * Description: This is the porting function corresponding to MD5 utility
 *              function MD5Init.
 *
 * Return : Nothing.
 *******************************************************************************/

  util_void_t  aDigest_port_MD5Init(ADIGEST_MD5_CTX  *pContext);
    

/*******************************************************************************
 * FUNCTION: aDigest_port_MD5Update
 * 
 * Description: This is the porting function corresponding to MD5 utility
 *              function MD5Update.
 *
 * Return : Nothing.
 *******************************************************************************/

  util_void_t  aDigest_port_MD5Update( 
                                    ADIGEST_MD5_CTX  *pContext,
                                    util_uint8_t    *pInput, 
                                    util_uint8_t    inputLen) ;


/*******************************************************************************
 * FUNCTION: aDigest_port_MD5Final
 * 
 * Description: This is the porting function corresponding to MD5 utility
 *              function MD5Final.
 *
 * Return : Nothing.
 *******************************************************************************/

util_void_t  aDigest_port_MD5Final(
               util_uint8_t       digest[16],
               ADIGEST_MD5_CTX    *pContext);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __ADIGEST_PORT_PROTOTYPES_H__ */



