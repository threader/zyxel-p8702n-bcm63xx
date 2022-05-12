/******************************************************************************
*                                    
* FILE NAME    : aDigest_types.h                
*                                    
* DESCRIPTION  : Contains the definitions for the data types. 
*                                    
* Revision History :                            
*
*  DATE            NAME                 REFERENCE       REASON        
* ------         ------------           ---------     ----------
* 03-Dec-2008    Anuradha Gupta         None          Initial
* 04-Dec-2008    Alok Tiwari            LLD           aDigest 1.0      
* Copyright 2008, Aricent.                       
*                                    
*******************************************************************************/

#ifndef __ADIGEST_TYPES_H__
#define __ADIGEST_TYPES_H__

/* Prevent Name Mangling By C++ Compilers */

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */
   
/* This enum contains all the error codes which will be
 * reported to application.
 */ 
typedef enum
{
    ADIGEST_ERROR_START = 0,
    ADIGEST_ERROR_INVALID_ALGORITHM_NAME,
    ADIGEST_ERROR_INVALID_USERNAME,
    ADIGEST_ERROR_INVALID_REALM,
    ADIGEST_ERROR_INVALID_PASSWORD,
    ADIGEST_ERROR_INVALID_NONCE,
    ADIGEST_ERROR_INVALID_CNONCE,
    ADIGEST_ERROR_INVALID_ENTITY_BODY,
    ADIGEST_ERROR_INVALID_NONCE_COUNT,
    ADIGEST_ERROR_INVALID_QOP_VALUE,
    ADIGEST_ERROR_INVALID_METHOD_NAME,
    ADIGEST_ERROR_INVALID_URI,
    ADIGEST_ERROR_INVALID_CHECKSUM,
    ADIGEST_ERROR_END
} aDigest_ecode_et;


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __ADIGEST_TYPES_H__ */



