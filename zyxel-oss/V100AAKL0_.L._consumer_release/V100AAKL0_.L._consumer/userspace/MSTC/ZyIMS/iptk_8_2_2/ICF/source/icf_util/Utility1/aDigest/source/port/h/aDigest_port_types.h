/******************************************************************************
*                                    
* FILE NAME    : aDigest_port_types.h                
*                                    
* DESCRIPTION  : Contains the porting definitions for the data types. 
*                                    
* Revision History :                            
*
*  DATE            NAME                 REFERENCE       REASON        
* ------         ------------           ---------     ----------
* 03-Dec-2008    Anuradha Gupta         None          Initial
* 04-Dec-2008    Alok Tiwari            LLD           aDigest 1.0
* 27-Feb-2009    Alok Tiwari            ICF Rel-8.1   Changes made to port the
*                                                     MD5 utility.
*
* Copyright 2008, Aricent.                       
*                                    
*******************************************************************************/

#ifndef __ADIGEST_PORT_TYPES_H__
#define __ADIGEST_PORT_TYPES_H__

#include "cryartdf.h"

/* Prevent Name Mangling By C++ Compilers */

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */
  
typedef unArCryptoHash  ADIGEST_MD5_CTX;

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __ADIGEST_PORT_TYPES_H__ */



