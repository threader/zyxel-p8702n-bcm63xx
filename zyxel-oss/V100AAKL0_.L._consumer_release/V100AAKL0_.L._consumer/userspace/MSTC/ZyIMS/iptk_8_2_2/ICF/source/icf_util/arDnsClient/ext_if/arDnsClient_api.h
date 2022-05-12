/******************************************************************************
*                                    
* FILE NAME    : arDnsClient_api.h                
*                                    
* DESCRIPTION  : Contains the definitions for the interfaces. 
*                                    
* Revision History :                            
*
*  DATE            NAME                 REFERENCE       REASON        
* ------         ------------           ---------     ----------
* 05-Feb-2009    Anuradha Gupta           None          Initial
* 06-Feb-2009    Anuradha Gupta           API Doc       API declarations
*                                                                
* Copyright 2009, Aricent.                       
*                                    
*******************************************************************************/

#ifndef __ARDNSCLIENT_API_H__
#define __ARDNSCLIENT_API_H__

#include "arDnsClient_types.h"

/* Prevent Name Mangling By C++ Compilers */

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

/****************************************************************************
 * API Name: arDnsClient_init
 * 
 * Description: This API is invoked by the application to initialize the CLIB and
 *              get the pointer to the global data of arDNSClient and the IPC fd. 
 * INPUT       :pDns_client_addr - listening address of DNS Client.
 *             :pClib_addr - listening address of CLIB.
 *             :pApp_port_ctxt - Application specific porting data pointer.
 *             :ppDnscl_glb_data - The pointer to the global data of arDnsClient.
 * OUTPUT      :ppDnscl_glb_data - The pointer to the global data of arDnsClient. 
 *             :pIpc_fd - the identifier of the IPC channel for CLIB.
 *             :pError - Error pointer to be populated in case of error.
 *
 * Return : It will return ADNSCL_SUCCESS if successful else return ARDNSCL_FAILURE.
 ******************************************************************************/
dnscl_return_t  arDnsClient_init (
     IN    arDnsClient_address_st   *pDns_client_addr,
     IN    arDnsClient_address_st   *pClib_addr,
     IN    dnscl_void_t             *pApp_port_ctxt,
     OUT   dnscl_void_t             **ppDnscl_glb_data,
     OUT   dnscl_int32_t            *pClib_fd,
     OUT   dnscl_int32_t            *pDnscl_fd,
     OUT   dnscl_error_t            *pError);

/****************************************************************************
 * API Name: arDnsClient_deinit
 * 
 * Description: This API is invoked by the application to deinitialize the utility
 * INPUT       :pDnscl_glb_data - The pointer to the global data of arDnsClient.
 * OUTPUT      :pError - Error pointer to be populated in case of error. 
 *
 * Return : It will return ADNSCL_SUCCESS if successful else return ARDNSCL_FAILURE.
 ******************************************************************************/
dnscl_return_t  arDnsClient_deinit (
      IN    dnscl_void_t   *pDnsCl_glb_data,
      OUT   dnscl_error_t  *pError);

/****************************************************************************
 * API Name: arDnsClient_set_conf
 * 
 * Description: This API is invoked by the application for configuration purpose. 
 * INPUT       :pDnscl_glb_data - The pointer to the global data of arDnsClient.
 *             :pConfig_data - The pointer to configuration data.  
 * OUTPUT      :pError - Error pointer to be populated in case of error. 
 *
 * Return : It will return ADNSCL_SUCCESS if successful else return ARDNSCL_FAILURE.
 ******************************************************************************/
dnscl_return_t  arDnsClient_set_conf (
     IN    dnscl_void_t         *pDnsCl_glb_data,
     IN    arDnsClient_conf_st  *pConfig_data,
     OUT   dnscl_error_t        *pError);

/****************************************************************************
 * API Name: arDnsClient_set_trace
 * 
 * Description: This API is invoked by the application for enabling / disabling
 *              trace reporting. 
 * INPUT       :pDnscl_glb_data - The pointer to the global data of arDnsClient.
 *             :enable_trace - The flag for enable or disable.  
 * OUTPUT      :pError - Error pointer to be populated in case of error. 
 *
 * Return : It will return ADNSCL_SUCCESS if successful else return ARDNSCL_FAILURE.
 ******************************************************************************/
dnscl_return_t  arDnsClient_set_trace (
     IN    dnscl_void_t         *pDnsCl_glb_data,
     IN    dnscl_boolean_t      enable_trace,
     OUT   dnscl_error_t        *pError);

/****************************************************************************
 * API Name: arDnsClient_clear_cache
 * 
 * Description: This API is invoked by the application for clearing the cache data
 *              Option for Full clear or single entry clear is provided. 
 * INPUT       :pDnscl_glb_data - The pointer to the global data of arDnsClient.
 *             :pClear_cache - The pointer to clear cache information.  
 * OUTPUT      :pError - Error pointer to be populated in case of error. 
 *
 * Return : It will return ADNSCL_SUCCESS if successful else return ARDNSCL_FAILURE.
 ******************************************************************************/
dnscl_return_t  arDnsClient_clear_cache(
     IN    dnscl_void_t               *pDnsCl_glb_data,            
     IN    arDnsClient_clear_cache_st *pClear_cache,
     OUT   dnscl_error_t              *pError);

/****************************************************************************
 * API Name: arDnsClient_send_query
 * 
 * Description: This API is invoked by the application for sending a DNS query 
 *              request. 
 * INPUT       :pDnscl_glb_data - The pointer to the global data of arDnsClient.
 *             :pQuery_data - The pointer to query related information.  
 *             :service_name - In case of SRV query the name of the service
 *             :protocol_name - In case of SRV query the name of the protocol
 * OUTPUT      :pError - Error pointer to be populated in case of error. 
 *
 * Return : It will return ADNSCL_SUCCESS if successful else return ARDNSCL_FAILURE.
 ******************************************************************************/
dnscl_return_t  arDnsClient_send_query (
     IN    dnscl_void_t          *pDnsCl_glb_data,
     IN    arDnsClient_query_st  *pQuery_data,
     IN    dnscl_uint8_t         service_name,
     IN    dnscl_uint8_t         protocol_name,
     OUT   dnscl_error_t         *pError);





/****************************************************************************
 * API Name: arDnsClient_free_response
 * 
 * Description: This API is invoked by the application for freeing the response
 *              structure. 
 * INPUT       :pDnscl_glb_data - The pointer to the global data of arDnsClient.
 *             :pResponse - The pointer to response structure.  
 * OUTPUT      :pError - Error pointer to be populated in case of error. 
 *
 * Return : It will return ADNSCL_SUCCESS if successful else return ARDNSCL_FAILURE.
 ******************************************************************************/
dnscl_return_t  arDnsClient_free_response (
    IN    dnscl_void_t            *pDnsCl_glb_data,
    IN    arDnsClient_response_st  *pResponse,
    OUT   dnscl_error_t           *pError);

/****************************************************************************
 * API Name: arDnsClient_start
 * 
 * Description: This function is invoked by the application for starting the 
 *              DNS Client. This function shall be associated to thread while
 *              creating the thread
 * INPUT       :pArg - The pointer to the global data of arDnsClient should be
 *                     of type arDnsClient_global_data_st.
 * OUTPUT      :None. 
 *
 * Return :     None.
 ******************************************************************************/
#ifndef ARDNSCL_PORT_WINDOWS
dnscl_void_t   *arDnsClient_start (
       IN    dnscl_void_t  *pArg);
#else
dnscl_int32_t arDnsClient_start (
       IN dnscl_void_t  *pArg);
#endif

/*******************************************************************************
 * FUNCTION: arDnsClient_process_request 
 * 
 * Description: This is the entry point function for query handling.
 * INPUT       :pDnscl_glb_data - The pointer to the global data of arDnsClient.
 *             :pRequest - The pointer to request structure.
 * OUTPUT      :pError - Error pointer to be populated in case of error.
 *
 * Return : ARDNSCL_SUCCESS / ARDNSCL_FAILURE 
 * 
 *******************************************************************************/
 dnscl_return_t   arDnsClient_process_request(
                    IN    dnscl_void_t 		      *pDnsCl_glb_data,
                    INOUT arDnsClient_request_st  *pRequest,
                    OUT   dnscl_error_t      *pError);


/*******************************************************************************
 * FUNCTION: arDnsClient_get_response 
 * 
 * Description: This function receives the message from DNS Client, unpacks the 
 *              stream buffer and provides the Response Structure to application
 *              for futher processing.
 * INPUT       :pDnscl_glb_data - The pointer to the global data of arDnsClient.
 * OUTPUT      :ppResponse - Pointer to the address of Response structure that shall
 *             :             be allocated and populated by CLIB.
 *             :pError - Error pointer to be populated in case of error.
 *
 * Return : ARDNSCL_SUCCESS / ARDNSCL_FAILURE 
 * 
 *******************************************************************************/
 dnscl_return_t   arDnsClient_get_response(
                    IN    dnscl_void_t 		     *pDnsCl_glb_data,
                    OUT arDnsClient_response_st  **ppResponse,
                    OUT   dnscl_error_t          *pError); 

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __ARDNSCLIENT_API_H__ */



