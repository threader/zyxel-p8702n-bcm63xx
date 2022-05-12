/******************************************************************************
*                                    
* FILE NAME    : arMD5_defs.h                
*                                    
* DESCRIPTION  : Contains the literal and porting definitions 
*                                    
* Revision History :                            
*
*  DATE            NAME                 REFERENCE       REASON        
* ------         ------------           ---------     ----------
* 19-Dec-2008    Alok Tiwari            None          Initial
* 22-Dec-2008    Alok Tiwari            LLD           Definitions
*                                                                
* Copyright 2008, Aricent.                       
*                                    
*******************************************************************************/

#ifndef __ARMD5_DEFS_H__
#define __ARMD5_DEFS_H__

#include "stdlib.h"

#define ARMD5_NULL             NULL

/* The MD5 hash block size */    
#define ARMD5_BLOCK_SIZE 	64

/* message length */ 
#define ARMD5_MSG_LENGTH        8   

/* Prevent Name Mangling By C++ Compilers */

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __ARMD5_DEFS_H__ */



