/******************************************************************************
*                                    
* FILE NAME    : aDigest_prototypes.h                
*                                    
* DESCRIPTION  : Contains the definitions for function prototypes. 
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

#ifndef __ADIGEST_PROTOTYPES_H__
#define __ADIGEST_PROTOTYPES_H__

#include "common_types.h"
#include "common_defs.h"

/* Prevent Name Mangling By C++ Compilers */

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */
    
/*****************************************************************************
 * FUNCTION :   aDigest_calc_HA2
 *
 * Description: This function is used to calculate H (A2).
 *              The MD5 hash of the combined method and digest URI is
 *              calculated.
 *              
 * Return:      Nothing
 ********************************************************************************/

 util_void_t  aDigest_calc_HA2 ( 
        IN      util_string_st      req_method,                          
        IN      util_string_st      digest_uri,
        IN      util_string_st      qop_val,
        IN      util_string_st      HEntity,
        OUT     util_string_st      HA2);


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __ADIGEST_PROTOTYPES_H__ */



