/******************************************************************************
*                                    
* FILE NAME    : arDnsClient_cmn_prototypes.h                
*                                    
* DESCRIPTION  : Contains the definitions for the interfaces. 
*                                    
* Revision History :                            
*
*  DATE            NAME                 REFERENCE       REASON        
* ------         ------------           ---------     ----------
* 13-Feb-2009    Anuradha Gupta         LLD           Code Start
*                                                                
* Copyright 2009, Aricent.                       
*                                    
*******************************************************************************/

#ifndef __ARDNSCLIENT_CMN_PROTOTYPES_H__
#define __ARDNSCLIENT_CMN_PROTOTYPES_H__

#include "arDnsClient_types.h"

/* Prevent Name Mangling By C++ Compilers */

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */



/****************************************************************************
 * API Name: arDnsClient_free_request
 * 
 * Description: This API is invoked by the application for freeing the request
 *              structure. 
 *  PUT       :pDnscl_glb_data - The pointer to the global data of arDnsClient.
 *             :pRequest - The pointer to request structure.  
 * PUT      :pError - Error pointer to be populated in case of error. 
 *
 * Return : It will return ADNSCL_SUCCESS if successful else return ARDNSCL_FAILURE.
 ******************************************************************************/
dnscl_return_t  arDnsClient_free_request (
         dnscl_void_t            *pDnsCl_glb_data,
         arDnsClient_request_st  *pRequest,
       dnscl_error_t           *pError);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __ARDNSCLIENT_CMN_PROTOTYPES_H__ */

