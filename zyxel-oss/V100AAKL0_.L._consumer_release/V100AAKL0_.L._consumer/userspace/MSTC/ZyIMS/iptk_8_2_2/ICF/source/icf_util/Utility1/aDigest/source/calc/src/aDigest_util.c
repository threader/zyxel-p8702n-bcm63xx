/******************************************************************************
*                                    
* FILE NAME    : aDigest_util.c                
*                                    
* DESCRIPTION  : Contains the Digest utility routines  
*                                    
* Revision History :                            
*
*  DATE            NAME                 REFERENCE       REASON        
* ------         ------------           ---------     ----------
* 03-Dec-2008    Anuradha Gupta         None          Initial
* 05-Dec-2008    Alok Tiwari            LLD           aDigest 1.0
* 11-Jun-2009    Ashutosh Mohan     SPR 19737     Changes done for GCC 4.1 warnings removal          
* Copyright 2008, Aricent.                       
*                                    
*******************************************************************************/

#include "common_defs.h"
#include "common_types.h"
#include "common_prototypes.h"
#include "aDigest_port_types.h"
#include "aDigest_port_prototypes.h"
#include "aDigest_defs.h"
#include "aDigest_macro.h"
#include "aDigest_prototypes.h"

/*****************************************************************************
 * FUNCTION :   aDigest_calc_HA2
 *
 * Description: This function is used to calculate H (A2).
 *              The MD5 hash of the combined method and digest URI is
 *              calculated.
 *              
 * Return:      Nothing
 ********************************************************************************/

 util_void_t  aDigest_calc_HA2( 
        IN      util_string_st      req_method,                          
        IN      util_string_st      digest_uri,
        IN      util_string_st      qop_val,
        IN      util_string_st      HEntity,
        OUT     util_string_st      HA2)
 {
      ADIGEST_MD5_CTX           Md5Ctx;
      util_uint8_t              key_A2[ADIGEST_CHECKSUM_LEN] = "\0";

      ADIGEST_PRINT(("Enterd Function: aDigest_calc_HA2\n"));
                              
      /* compute HA2 = MD5(req_method : digest_uri : HEntity)
       * if qop_val.str is "auth-int" else
       * compute HA2 = MD5(req_method : digest_uri)
       */
      
      /* Initialize the MD5 context.*/
      aDigest_port_MD5Init(&Md5Ctx);
      aDigest_port_MD5Update(&Md5Ctx,req_method.str, req_method.str_len);
      aDigest_port_MD5Update(&Md5Ctx,(util_uint8_t*)":",1);
      aDigest_port_MD5Update(&Md5Ctx,digest_uri.str, digest_uri.str_len); 

     if(NULL != qop_val.str){	//ZyXEL porting
      if(0 == util_strcmpi(qop_val.str, (util_uint8_t*)"auth-int"))
      {
        aDigest_port_MD5Update(&Md5Ctx,(util_uint8_t*)":",1);
        aDigest_port_MD5Update(&Md5Ctx,HEntity.str, HEntity.str_len);
      }
     	}	//ZyXEL porting
      
     /* Assign Md5Ctx to key_A2.*/
     aDigest_port_MD5Final(key_A2,&Md5Ctx);

     /* Convert key_A2 into hexadecimal format and save it into HA2.*/
     ADIGEST_CONVERT_TO_HEXSTR(key_A2,HA2.str)

     ADIGEST_PRINT(("Exit Function: aDigest_calc_HA2\n"));
     return;
 }

 
