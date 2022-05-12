/******************************************************************************
*                                    
* FILE NAME    : arMD5_api.h                
*                                    
* DESCRIPTION  : Contains the definitions for the interfaces. 
*                                    
* Revision History :                            
*
*  DATE            NAME                 REFERENCE       REASON        
* ------         ------------           ---------     ----------
* 19-Dec-2008    Alok Tiwari            None          Initial
* 22-Dec-2008    Alok Tiwari            LLd           arMD5 1.0 
*                                                                
* Copyright 2008, Aricent.                       
*                                    
*******************************************************************************/

#ifndef __ARMD5_API_H__
#define __ARMD5_API_H__

#include "cryartdf.h"
#include "cryardef.h"

/* Prevent Name Mangling By C++ Compilers */

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

/*******************************************************************************
 * Function:    arMD5_start
 *    
 * Description: This function is used to MD5 initialization. It begins an MD5
 *              operation, writing a new context.
 *
 * Return:      Nothing
 ******************************************************************************/

 AR_CRYPTO_VOID arMD5_start (
           OUT  unArCryptoHash  *pHash_ctx);

/*******************************************************************************
 * Function:    arMD5_update
 *    
 * Description: This function is used to update MD5 block.It Continues an
 *              MD5 message-digest operation, processing another message block,
 *              and updating the context.
 *
 * Return:      Nothing
 ******************************************************************************/
 AR_CRYPTO_VOID arMD5_update (
           INOUT      unArCryptoHash   *pHash_ctx,
           IN         UINT1          *pInput,
           IN         UINT4          inputlen);

/*******************************************************************************
 * Function:    arMD5_finish
 *    
 * Description: This function is used for MD5 finalization. It is used to ends
 *              an MD5 message-digest operation and writing the message digest
 *              at output.
 *
 * Return:      Nothing
 ******************************************************************************/

 AR_CRYPTO_VOID arMD5_finish (
           INOUT      unArCryptoHash   *pHash_ctx,
           OUT        UINT1          *pOutput);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __ARMD5_API_H__ */



