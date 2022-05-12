/******************************************************************************
*                                    
* FILE NAME    : aDigest_calc.c                
*                                    
* DESCRIPTION  : Contains the Digest Calculation routines  
*                                    
* Revision History :                            
*
*  DATE            NAME                 REFERENCE       REASON        
* ------         ------------           ---------     ----------
* 03-Dec-2008    Anuradha Gupta         None          Initial
* 04-Dec-2008    Alok Tiwari            LLD           aDigest 1.0
* 11-Jun-2009    Ashutosh Mohan     SPR 19737     Changes done for GCC 4.1 warnings removal           
* 21-Apr-2010	 Aman Aggarwal       SPR 20383        Check for strlen for qop_val
*                                                      
* Copyright 2010, Aricent.                       
*                                    
*******************************************************************************/

#include "common_defs.h"
#include "common_types.h"
#include "common_prototypes.h"
#include "aDigest_port_types.h"
#include "aDigest_port_prototypes.h"
#include "aDigest_types.h"
#include "aDigest_defs.h"
#include "aDigest_macro.h"
#include "aDigest_prototypes.h"
#include "aDigest_api.h"


/****************************************************************************
 * API Name: aDigest_calc_HA1
 * 
 * Description: This API is invoked to calculate H(A1) which is a session key
 *              for the authentication of subsequent requests and responses 
 *              which is different for each ?œauthentication session??
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
         OUT   aDigest_ecode_et    *pError)
 {
    ADIGEST_MD5_CTX       Md5Ctx;
    util_uint8_t          key_A1[ADIGEST_CHECKSUM_LEN] = "\0" ;
    util_uint8_t          hex_A1[ADIGEST_HEX_CHECKSUM_LEN + 1] = "\0" ;
       
    ADIGEST_PRINT(("Enterd Function: aDigest_calc_HA1\n")); 

    /*Validate the parameter algorithm for md5 and md5-sess*/
   
    if(algorithm.str_len > ADIGEST_MAX_STR_LEN)
    {
      *pError = ADIGEST_ERROR_INVALID_ALGORITHM_NAME;
       ADIGEST_PRINT(("algorithm validation is failure\n"));
       ADIGEST_PRINT(("Exit Function: aDigest_calc_HA1 :\
                                 No further processing required.\n"));
       return UTIL_FAILURE;
    }
 
   /*Validate the parameter user_name*/
    else if((UTIL_NULL == user_name.str)||
            (0 == user_name.str_len) ||
            (ADIGEST_MAX_STR_LEN < user_name.str_len))
    {
      *pError = ADIGEST_ERROR_INVALID_USERNAME;
       ADIGEST_PRINT(("user_name validation is failure\n"));
       ADIGEST_PRINT(("Exit Function: aDigest_calc_HA1 :\
                                 No further processing required.\n"));
       return UTIL_FAILURE; 
    }

   /*Validate the parameter realm.*/
    else if((UTIL_NULL == realm.str)||
            (0 == realm.str_len) ||
            (ADIGEST_MAX_STR_LEN < realm.str_len)) 
    {
      *pError = ADIGEST_ERROR_INVALID_REALM;
       ADIGEST_PRINT(("realm validation is failure\n"));
       ADIGEST_PRINT(("Exit Function: aDigest_calc_HA1 :\
                                 No further processing required.\n"));
       return UTIL_FAILURE;
    }

   /*validate the parameter password.*/
    else if((UTIL_NULL == password.str)||
            (0 == password.str_len) ||
            (ADIGEST_MAX_STR_LEN < password.str_len)) 
    {
      *pError = ADIGEST_ERROR_INVALID_PASSWORD;
       ADIGEST_PRINT(("password validation is failure\n"));
       ADIGEST_PRINT(("Exit Function: aDigest_calc_HA1 :\
                                 No further processing required.\n"));
       return UTIL_FAILURE;
    }
    else if((UTIL_NULL == HA1.str)||
            (ADIGEST_HEX_CHECKSUM_LEN != HA1.str_len))
    {
      *pError = ADIGEST_ERROR_INVALID_CHECKSUM;
       ADIGEST_PRINT(("HA1 validation is failure\n"));
       ADIGEST_PRINT(("Exit Function: aDigest_calc_HA1 :\
                                 No further processing required.\n"));
       return UTIL_FAILURE;
    } 
    else if(0 == util_strcmpi(algorithm.str,(util_uint8_t*)"md5-sess") ||
            0 == util_strcmpi(algorithm.str,(util_uint8_t*)"md5-sert")) 
    {
      /*Validate the parameter nonce_val.*/ 
      if((UTIL_NULL == nonce_val.str)||
         (0 == nonce_val.str_len) ||
         (ADIGEST_MAX_STR_LEN < nonce_val.str_len))
      {
        *pError = ADIGEST_ERROR_INVALID_NONCE;
         ADIGEST_PRINT(("nonce validation is failure\n"));
         ADIGEST_PRINT(("Exit Function: aDigest_calc_HA1 :\
                                 No further processing required.\n"));
         return UTIL_FAILURE;
      }

      /*Validate the parameter cNonce_val.*/ 
      else if((UTIL_NULL == cNonce_val.str)||
              (0 == cNonce_val.str_len) ||
              (ADIGEST_MAX_STR_LEN < cNonce_val.str_len))
      {
        *pError = ADIGEST_ERROR_INVALID_CNONCE;
         ADIGEST_PRINT(("cNonce validation is failure\n"));
         ADIGEST_PRINT(("Exit Function: aDigest_calc_HA1 :\
                                 No further processing required.\n"));
         return UTIL_FAILURE; 
      }
    }

   /*Calculate HA1*/

  /* Initialize the MD5 context.*/
  aDigest_port_MD5Init(&Md5Ctx);

  /* Calculate MD5(user_name : realm :password) */  
  aDigest_port_MD5Update(&Md5Ctx,user_name.str, user_name.str_len);
  aDigest_port_MD5Update(&Md5Ctx, (util_uint8_t*)":", 1);
  aDigest_port_MD5Update(&Md5Ctx,realm.str, realm.str_len);
  aDigest_port_MD5Update(&Md5Ctx, (util_uint8_t*)":", 1);
  aDigest_port_MD5Update(&Md5Ctx,password.str, password.str_len);

  /* Assign it to key_A1.*/
  aDigest_port_MD5Final(key_A1,&Md5Ctx);
  
  if(0 == util_strcmpi(algorithm.str,(util_uint8_t*)"md5-sess"))
  {
    /* Initialize the MD5 context.*/
    aDigest_port_MD5Init(&Md5Ctx);
    
    /* Calculate MD5(key_A1 : nonce_val :cNonce_val) */
    aDigest_port_MD5Update(&Md5Ctx,key_A1, ADIGEST_CHECKSUM_LEN);
    aDigest_port_MD5Update(&Md5Ctx, (util_uint8_t*)":", 1);
    aDigest_port_MD5Update(&Md5Ctx,nonce_val.str, nonce_val.str_len);
    aDigest_port_MD5Update(&Md5Ctx, (util_uint8_t*)":", 1);
    aDigest_port_MD5Update(&Md5Ctx,cNonce_val.str, cNonce_val.str_len);
    
    /* Assign it to key_A1.*/
    aDigest_port_MD5Final(key_A1,&Md5Ctx);
  }
  /* To calculate HA1 according to errata behaviour, appliaction will provide
   * algorithm name as "MD5-sert" if the challenge is received with algorithm 
   * name as "MD5-sess".
   */ 
  else if(0 == util_strcmpi(algorithm.str,(util_uint8_t*)"md5-sert"))
  {
   /*Convert key_A1 into hexadecimal format and save it into hex_A1*/
    ADIGEST_CONVERT_TO_HEXSTR(key_A1,hex_A1)

   /* Initialize the MD5 context.*/
    aDigest_port_MD5Init(&Md5Ctx);
    
    /* Calculate MD5(hex_A1 : nonce_val :cNonce_val) */
    aDigest_port_MD5Update(&Md5Ctx,hex_A1, ADIGEST_HEX_CHECKSUM_LEN);
    aDigest_port_MD5Update(&Md5Ctx, (util_uint8_t*)":", 1);
    aDigest_port_MD5Update(&Md5Ctx,nonce_val.str, nonce_val.str_len);
    aDigest_port_MD5Update(&Md5Ctx, (util_uint8_t*)":", 1);
    aDigest_port_MD5Update(&Md5Ctx,cNonce_val.str, cNonce_val.str_len);
    
    /* Assign it to key_A1.*/
    aDigest_port_MD5Final(key_A1,&Md5Ctx);
  }
  

  /*Convert key_A1 into hexadecimal format and save it into HA1*/
  ADIGEST_CONVERT_TO_HEXSTR(key_A1,HA1.str) 
  
  ADIGEST_PRINT(("Exit Function: aDigest_calc_HA1 ret_val = 1\n"));
  return UTIL_SUCCESS; 
 }   
 

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
         OUT   aDigest_ecode_et    *pError)
 {
      ADIGEST_MD5_CTX            Md5Ctx;
      util_uint8_t               key_entity[ADIGEST_CHECKSUM_LEN]="\0";
     
      ADIGEST_PRINT(("Enterd Function: aDigest_calc_HEntity\n"));
  
      /* Validate the parameter entity_body. */ 
      if((UTIL_NULL == entity_body.str) ||
         (0 == entity_body.str_len) ||
         (ADIGEST_MAX_STR_LEN < entity_body.str_len))
      {
         *pError =ADIGEST_ERROR_INVALID_ENTITY_BODY ;
         ADIGEST_PRINT(("entity_body validation is failure\n"));
         ADIGEST_PRINT(("Exit Function: aDigest_calc_HEntity :\
                                 No further processing required.\n"));
         return UTIL_FAILURE;
      }
    
      /* Validate the parameter HEntity. */
      else if((UTIL_NULL == HEntity.str) ||
             (ADIGEST_HEX_CHECKSUM_LEN != HEntity.str_len))
      {
         *pError =ADIGEST_ERROR_INVALID_CHECKSUM ;
         ADIGEST_PRINT(("HEntity validation is failure\n"));
         ADIGEST_PRINT(("Exit Function: aDigest_calc_HEntity :\
                                 No further processing required.\n"));
         return UTIL_FAILURE; 
      } 

     /*Calculate key_entity.*/

     /* Initialize the MD5 context.*/
     aDigest_port_MD5Init(&Md5Ctx);

     /* Calculate MD5(entity_body) */
     aDigest_port_MD5Update(&Md5Ctx,entity_body.str, entity_body.str_len);

     /* Assign it to key_entity.*/
     aDigest_port_MD5Final(key_entity,&Md5Ctx); 

     /*Convert key_entity into hexadecimal format and save it into HEntity.*/
     ADIGEST_CONVERT_TO_HEXSTR(key_entity,HEntity.str) 

     ADIGEST_PRINT(("Exit Function: aDigest_calc_HEntity ret_val = 1\n"));
     return UTIL_SUCCESS;
 }


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
         IN    util_string_st      HA1,
         IN    util_string_st      nonce_val,
         IN    util_string_st      nonce_count,
         IN    util_string_st      cNonce_val,
         IN    util_string_st      qop_val,
         IN    util_string_st      req_method,
         IN    util_string_st      digest_uri,
         IN    util_string_st      HEntity, 
         OUT   util_string_st      response_val,
         OUT   aDigest_ecode_et    *pError)
 {
      ADIGEST_MD5_CTX           Md5Ctx;
      util_uint8_t              resp_hash [ADIGEST_CHECKSUM_LEN]= "\0";
      util_uint8_t              temp[ADIGEST_HEX_CHECKSUM_LEN + 1] = "\0";
      util_string_st            HA2 = {0,0};

      ADIGEST_PRINT(("Enterd Function: aDigest_calc_response\n"));
 
      HA2.str = temp;
      HA2.str_len = ADIGEST_HEX_CHECKSUM_LEN;

     /* Validate the parameter HA1*/ 
      if((UTIL_NULL == HA1.str) ||
         (ADIGEST_HEX_CHECKSUM_LEN != HA1.str_len))
      {
        *pError = ADIGEST_ERROR_INVALID_CHECKSUM;
         ADIGEST_PRINT(("HA1 validation is failure\n"));
         ADIGEST_PRINT(("Exit Function: aDigest_calc_response :\
                              No further processing required.\n"));
         return UTIL_FAILURE;
      }
 
     /* Validate the parameter nonce_val*/
      else if((UTIL_NULL == nonce_val.str) ||
              (0 == nonce_val.str_len) ||
              (ADIGEST_MAX_STR_LEN < nonce_val.str_len))
      {
        *pError = ADIGEST_ERROR_INVALID_NONCE;
         ADIGEST_PRINT(("nonce validation is failure\n"));
         ADIGEST_PRINT(("Exit Function: aDigest_calc_response :\
                              No further processing required.\n"));
         return UTIL_FAILURE;
      }

      /* Validate the parameter req_method*/
      else if((UTIL_NULL == req_method.str) ||
              (0 == req_method.str_len) ||
              (ADIGEST_MAX_STR_LEN < req_method.str_len))
      {
        *pError = ADIGEST_ERROR_INVALID_METHOD_NAME;
         ADIGEST_PRINT(("req_method validation is failure\n"));
         ADIGEST_PRINT(("Exit Function: aDigest_calc_response :\
                              No further processing required.\n"));
         return UTIL_FAILURE;
      }

      /* Validate the parameter digest_uri*/
      else if((UTIL_NULL == digest_uri.str) ||
              (0 == digest_uri.str_len) ||
              (ADIGEST_MAX_STR_LEN < digest_uri.str_len))
      {
        *pError = ADIGEST_ERROR_INVALID_URI;
         ADIGEST_PRINT(("digest_uri validation is failure\n"));
         ADIGEST_PRINT(("Exit Function: aDigest_calc_response :\
                              No further processing required.\n"));
         return UTIL_FAILURE;
      }
      /*Fix for SPR 20383. CSR 1-8534351. Check for strlen also*/
      else if ((UTIL_NULL != qop_val.str)
               && (0 != qop_val.str_len))
      {
        /* Validate the parameter req_method*/
       if(ADIGEST_MAX_STR_LEN < qop_val.str_len)
        {
          *pError = ADIGEST_ERROR_INVALID_QOP_VALUE;
           ADIGEST_PRINT(("qop validation is failure\n"));
           ADIGEST_PRINT(("Exit Function: aDigest_calc_response :\
                                No further processing required.\n"));
           return UTIL_FAILURE;
        }
       
        /* Validate the parameter nonce_count*/
       else if((UTIL_NULL == nonce_count.str) ||
                (0 == nonce_count.str_len) ||
                (ADIGEST_MAX_STR_LEN < nonce_count.str_len))
        {
          *pError = ADIGEST_ERROR_INVALID_NONCE_COUNT;
           ADIGEST_PRINT(("nonce_count validation is failure\n"));
           ADIGEST_PRINT(("Exit Function: aDigest_calc_response :\
                                  No further processing required.\n"));
           return UTIL_FAILURE;
        }

       /* Validate the parameter cNonce*/
       else if((UTIL_NULL == cNonce_val.str) ||
              (0 == cNonce_val.str_len) ||
              (ADIGEST_MAX_STR_LEN < cNonce_val.str_len))
        {
          *pError = ADIGEST_ERROR_INVALID_CNONCE;
           ADIGEST_PRINT(("cNonce validation is failure\n"));
           ADIGEST_PRINT(("Exit Function: aDigest_calc_response :\
                                 No further processing required.\n"));
           return UTIL_FAILURE;
        }
 
      }
      
      /* Calculate H(A2) */
      aDigest_calc_HA2(req_method, digest_uri, qop_val, HEntity, HA2); 

      /* Calculate Response */

      /* Initialize the MD5 context.*/
      aDigest_port_MD5Init(&Md5Ctx);
    
      /* Calculate MD5(HA1: nonce_val : nonce_count :  cNonce_val : qop_val :HA2)
       * if qop_val.str is not NULL else calculate MD5(HA1: nonce_val :HA2).
       */
      aDigest_port_MD5Update(&Md5Ctx,HA1.str, HA1.str_len);
      aDigest_port_MD5Update(&Md5Ctx, (util_uint8_t*)":", 1);
      aDigest_port_MD5Update(&Md5Ctx,nonce_val.str, nonce_val.str_len);
      aDigest_port_MD5Update(&Md5Ctx, (util_uint8_t*)":", 1);
      
      /*Fix for SPR 20383. CSR 1-8534351. Check for strlen also*/
      if ((UTIL_NULL != qop_val.str)
             && (0 != qop_val.str_len))
      {
        aDigest_port_MD5Update(&Md5Ctx,nonce_count.str, nonce_count.str_len);
        aDigest_port_MD5Update(&Md5Ctx, (util_uint8_t*)":", 1);
        aDigest_port_MD5Update(&Md5Ctx,cNonce_val.str, cNonce_val.str_len);
        aDigest_port_MD5Update(&Md5Ctx, (util_uint8_t*)":", 1);
        aDigest_port_MD5Update(&Md5Ctx,qop_val.str, qop_val.str_len);
        aDigest_port_MD5Update(&Md5Ctx, (util_uint8_t*)":", 1);
      }

      aDigest_port_MD5Update(&Md5Ctx,HA2.str,HA2.str_len);

      /* Assign Md5Ctx to resp_hash.*/
     aDigest_port_MD5Final(resp_hash,&Md5Ctx); 

     /*Convert resp_hash into hexadecimal format and save it into response_val.*/
     ADIGEST_CONVERT_TO_HEXSTR (resp_hash,response_val.str)

     ADIGEST_PRINT(("Exit Function: aDigest_calc_response ret_val = 1\n"));
     return UTIL_SUCCESS;
 
 }
         

