/******************************************************************************
*                                    
* FILE NAME    : aDigest_api.h                
*                                    
* DESCRIPTION  : Contains the definitions for the interfaces. 
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

#ifndef __ADIGEST_API_H__
#define __ADIGEST_API_H__

#include "common_types.h"
#include "common_defs.h"
#include "aDigest_types.h"

/* Prevent Name Mangling By C++ Compilers */

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */
    
/****************************************************************************
 * API Name: aDigest_calc_HA1
 * 
 * Description: This API is invoked to calculate H(A1) which is a session key
 *              for the authentication of subsequent requests and responses 
 *              which is different for each “authentication session”.
 *
 * Return : It will return UTIL_SUCCESS if successful else return UTIL_FAILURE.
 ******************************************************************************/

 util_return_t  aDigest_calc_HA1 ( 
          IN    util_string_st      algorithm,
          IN    util_string_st      user_name,
          IN    util_string_st      realm,
          IN    util_string_st      password,
          IN    util_string_st      nonce_val,
          IN    util_string_st      cNonce_val,
          OUT   util_string_st      HA1,
          OUT   aDigest_ecode_et    *pError);


/****************************************************************************
 * API Name: aDigest_calc_HEntity
 * 
 * Description: This API is invoked to calculate the checksum of entity_body 
 *              in hexadecimal format as HEntity.
 *
 * Return : It will return UTIL_SUCCESS if successful else return UTIL_FAILURE.
 ******************************************************************************/

 util_return_t   aDigest_calc_HEntity (
          IN    util_string_st      entity_body,
          OUT   util_string_st      HEntity,
          OUT   aDigest_ecode_et    *pError);

/****************************************************************************
 * API Name: aDigest_calc_response
 * 
 * Description: This API is invoked to generate the final digest response.
 *              The MD5 hash of the combined HA1 result, server nonce (nonce),
 *              request counter (nc), client nonce (cnonce),
 *              quality of protection code (qop) and HA2 result is calculated.
 *              The result is in the "response" in case of success.In case of 
 *              failure the Error code is populated.
 *
 * Return : It will return UTIL_SUCCESS if successful else return UTIL_FAILURE.
 ******************************************************************************/

 util_return_t  aDigest_calc_response (
          IN    util_string_st    HA1,
          IN    util_string_st    nonce_val,
          IN    util_string_st    nonce_count,
          IN    util_string_st    cNonce_val,
          IN    util_string_st    qop_val,
          IN    util_string_st    req_method,
          IN    util_string_st    digest_uri,
          IN    util_string_st    HEntity, 
          OUT   util_string_st   response_val,
          OUT   aDigest_ecode_et  *pError);


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __ADIGEST_API_H__ */



