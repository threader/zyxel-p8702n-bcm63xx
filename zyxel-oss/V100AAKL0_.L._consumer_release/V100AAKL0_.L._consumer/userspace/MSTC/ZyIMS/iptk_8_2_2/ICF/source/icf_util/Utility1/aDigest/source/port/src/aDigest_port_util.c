/******************************************************************************
*                                    
* FILE NAME    : aDigest_port_util.c                
*                                    
* DESCRIPTION  : Contains the Digest porting utility routines  
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

#include "common_defs.h"
#include "common_types.h"
#include "aDigest_port_types.h"
#include "aDigest_port_prototypes.h"
#include "arMD5_api.h"

/*******************************************************************************
 * FUNCTION: aDigest_port_MD5Init
 * 
 * Description: This is the porting function corresponding to MD5 utility
 *              function MD5Init.
 *
 * Return : Nothing.
 *******************************************************************************/

 util_void_t aDigest_port_MD5Init(ADIGEST_MD5_CTX *pContext)
  {
     arMD5_start(pContext);
  }

/*******************************************************************************
 * FUNCTION: aDigest_port_MD5Update
 * 
 * Description: This is the porting function corresponding to MD5 utility
 *              function MD5Update.
 *
 * Return : Nothing.
 *******************************************************************************/

 util_void_t  aDigest_port_MD5Update(
                 ADIGEST_MD5_CTX *pContext,
                 util_uint8_t    *pInput, 
                 util_uint8_t    inputLen)
  {
     arMD5_update(pContext, pInput, inputLen);
  }

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
                 ADIGEST_MD5_CTX    *pContext)
  {
     arMD5_finish(pContext,digest);
  }

