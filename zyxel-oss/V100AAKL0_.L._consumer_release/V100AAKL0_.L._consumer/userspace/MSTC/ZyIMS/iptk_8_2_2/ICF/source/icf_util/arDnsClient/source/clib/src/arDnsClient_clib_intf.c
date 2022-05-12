/******************************************************************************
*                                    
* FILE NAME    : arDnsClient_clib_intf.c                
*                                    
* DESCRIPTION  : Contains the arDnsClient CLIB interface routines  
*                                    
* Revision History :                            
*
*  DATE            NAME                 REFERENCE       REASON        
* ------         ------------           ---------     ----------
* 05-Feb-2009    Anuradha Gupta         None          Initial
* 09-Feb-2009    Abbas Haque        	None          Added code for arDnsClient
* 28-May-2010    Alok Tiwari            SPR-20412     Memory issues resolved.
*                                         Modified function arDnsClient_deinit.
*
* Copyright 2009, Aricent.                       
*                                    
*******************************************************************************/
#include "arDnsClient_api.h"
#include "arDnsClient_port_prototypes.h"
#include "arDnsClient_cmn_prototypes.h"
#include "arDnsClient_clib_prototypes.h"
#include "arDnsClient_clib_defs.h"

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
 * return : It will return ADNSCL_SUCCESS if successful else return ARDNSCL_FAILURE.
 ******************************************************************************/
dnscl_return_t  arDnsClient_init (
		IN    arDnsClient_address_st   *pDns_client_addr,
		IN    arDnsClient_address_st   *pClib_addr,
		IN    dnscl_void_t             *pApp_port_ctxt,
		OUT   dnscl_void_t             **ppDnscl_glb_data,
		OUT   dnscl_int32_t            *pClib_fd,
		OUT   dnscl_int32_t            *pDnscl_fd,
		OUT   dnscl_error_t            *pError)
{
    /* Value of return type*/
	dnscl_return_t   ret_val = ARDNSCL_SUCCESS;
      
	/*Pointer to global data structure*/
	arDnsClient_global_data_st   *pGlob_data = ARDNSCL_NULL;

	if(ARDNSCL_NULL == pError)
	{
		return ARDNSCL_FAILURE;
	}

	if(ARDNSCL_NULL == pDns_client_addr)
	{
		*pError = ARDNSCL_ERROR_INVALID_IP_ADDRESS;
		return ARDNSCL_FAILURE;
	}

	if(ARDNSCL_NULL == pClib_addr)
	{
		*pError = ARDNSCL_ERROR_INVALID_IP_ADDRESS;
		return ARDNSCL_FAILURE;
	}

	/*Allocate memory for arDnsClient utilty global data 
	  structure*/ 
	ARDNSCL_MEMGET(pGlob_data,ARDNSCL_SIZEOF(arDnsClient_global_data_st))

	/*Check for global data structure memory allocation 
	  failure*/
	if(ARDNSCL_NULL == pGlob_data )
	{
		*pError = ARDNSCL_ERROR_MEMORY_ALLOC_FAILURE;
		return ARDNSCL_FAILURE; 
	}

	/*Assign the dns client address */ 
	ARDNSCL_MEMCPY(&(pGlob_data->dns_client_addr), 
            pDns_client_addr, 
            ARDNSCL_SIZEOF(arDnsClient_address_st));

	/*Assign the CLIB address */ 
	ARDNSCL_MEMCPY(&(pGlob_data->clib_addr), 
			pClib_addr,
			ARDNSCL_SIZEOF(arDnsClient_address_st));

	pGlob_data->clib_data.enable_trace = ARDNSCL_DEFAULT_TRACE_VAL;
	pGlob_data->clib_data.cache_counter = ARDNSCL_INIT_VAL;

	/*Open a ipc for client library*/
	ret_val = arDnsClient_port_create_ipc(
                 pGlob_data,
				 pGlob_data->clib_addr,
				 &pGlob_data->clib_data.ipc_fd,
				 pError);

	/*Check for IPC FAILURE*/
	if(ARDNSCL_FAILURE == ret_val)
	{
        ARDNSCL_CLIB_PRINT(pGlob_data, ("Failure in create IPC\n"))
		ARDNSCL_MEMFREE(pGlob_data);
		/*return FAILURE*/
		return ARDNSCL_FAILURE;
	}

     /*Open a ipc for DNS Client*/
	ret_val = arDnsClient_port_create_ipc(
                pGlob_data,
				pGlob_data->dns_client_addr,
				&pGlob_data->dns_client_data.ipc_fd,
				pError);
	/*Check for IPC FAILURE*/
	if(ARDNSCL_FAILURE == ret_val)
	{
  
        ARDNSCL_CLIB_PRINT(pGlob_data, ("Failure in create IPC\n"))
        arDnsClient_port_close_ipc(pGlob_data, pGlob_data->clib_data.ipc_fd,pError);
		ARDNSCL_MEMFREE(pGlob_data);
		/*return FAILURE*/
		return ARDNSCL_FAILURE;
	}

    /*Assigning Client library FD to application*/ 
    *pClib_fd = pGlob_data->clib_data.ipc_fd;
    
    /*Assigning DNS client FD*/
   *pDnscl_fd = pGlob_data->dns_client_data.ipc_fd;


	/* store the app port ctxt in the global data */
	pGlob_data->pApp_port_data = (dnscl_void_t *)pApp_port_ctxt;

	/* assign the glb data pointer */
	*ppDnscl_glb_data = pGlob_data;

    ARDNSCL_CLIB_PRINT(pGlob_data,("Exiting funtion arDnsClient_init \n"))

	/*return SUCCESS*/
	return ARDNSCL_SUCCESS;
}
/****************************************************************************
 * API Name: arDnsClient_set_conf
 * 
 * Description: This API is invoked by the application for configuration purpose. 
 * INPUT       :pDnscl_glb_data - The pointer to the global data of arDnsClient.
 *             :pConfig_data - The pointer to configuration data.  
 * OUTPUT      :pError - Error pointer to be populated in case of error. 
 *
 * return : It will return ADNSCL_SUCCESS if successful else return ARDNSCL_FAILURE.
 ******************************************************************************/
dnscl_return_t  arDnsClient_set_conf (
		IN    dnscl_void_t         *pDnsCl_glb_data,
		IN    arDnsClient_conf_st  *pConfig_data,
		OUT   dnscl_error_t        *pError)
{
	/*Pointer to global data structure*/
	arDnsClient_global_data_st   *pGlob_data = ARDNSCL_NULL;

	/*variable for arDnsClient request data structure*/
	arDnsClient_request_st       request_data ; 
                     /*{0,0,0,ARDNSCL_NULL,ARDNSCL_NULL,0,ARDNSCL_NULL};*/

	/*Variable for return type*/
	dnscl_return_t       ret_val =ARDNSCL_SUCCESS;

    /*Variable for error*/
	dnscl_error_t       error = ARDNSCL_INIT_VAL;

	/*Check for  error pointer*/
	if(ARDNSCL_NULL == pError)
	{
		return ARDNSCL_FAILURE;
	}
	
        /*Check for  global data pointer*/
	if(ARDNSCL_NULL == pDnsCl_glb_data)
	{
		/*retun FAILURE with appropriate error*/
		*pError =  ARDNSCL_ERROR_INVALID_GLB_DATA;
		return ARDNSCL_FAILURE;
	}

	/*Type cast global data*/       
	pGlob_data =(arDnsClient_global_data_st*)pDnsCl_glb_data;

	ARDNSCL_CLIB_PRINT(pGlob_data, ("Entered function arDnsClient_set_conf\n"))

        ARDNSCL_MEMSET(&request_data, 0, ARDNSCL_SIZEOF(arDnsClient_request_st));
	/*check for invalid configuration data*/
	if(ARDNSCL_NULL == pConfig_data)
	{
		ARDNSCL_CLIB_PRINT(pGlob_data, ("Invalid Configuration data\n"))
		/*retun FAILURE with appropriate error*/
		*pError =  ARDNSCL_ERROR_DNS_INVALID_CONFIG_DATA;
		return ARDNSCL_FAILURE;
	}

    ARDNSCL_MEMGET(request_data.pConfData , ARDNSCL_SIZEOF(arDnsClient_conf_st));

	if(ARDNSCL_NULL == request_data.pConfData)
	{
	    ARDNSCL_CLIB_PRINT (pGlob_data, ("Memory Allocation Failure For Config Data of arDns Request Structure\n"))
		/*return FAILURE*/
		return ARDNSCL_FAILURE;
	}

	/*Assign the value of clear cache data pointer*/
	ARDNSCL_MEMCPY(request_data.pConfData,
			pConfig_data,
			ARDNSCL_SIZEOF(arDnsClient_conf_st));


	/*Set the value of API id*/
	request_data.api_id = ARDNSCL_API_SET_CONF_REQ;

	/*Send data to application*/
	ret_val = arDnsClient_port_send_msg_to_dnscl(
                       pGlob_data,
                       &request_data, 
                       pError);
	if(ret_val == ARDNSCL_FAILURE)
	{
		ARDNSCL_CLIB_PRINT (pGlob_data, ("Failure In IPC Send\n"));
		arDnsClient_free_request (pGlob_data,&request_data,&error);
		return ARDNSCL_FAILURE;
	}


	/*Free the memory allocated for request structure*/
	arDnsClient_free_request (pGlob_data,&request_data,&error);

	ARDNSCL_CLIB_PRINT (pGlob_data, ("Exiting Function arDnsClient_set_conf\n"))

	/*return Success*/
	return ARDNSCL_SUCCESS;
}

/****************************************************************************
 * API Name: arDnsClient_set_trace
 * 
 * Description: This API is invoked by the application for enabling / disabling
 *              trace reporting. 
 * INPUT       :pDnscl_glb_data - The pointer to the global data of arDnsClient.
 *             :enable_trace - The flag for enable or disable.  
 * OUTPUT      :pError - Error pointer to be populated in case of error. 
 *
 * return : It will return ADNSCL_SUCCESS if successful else return ARDNSCL_FAILURE.
 ******************************************************************************/
dnscl_return_t  arDnsClient_set_trace (
		IN    dnscl_void_t         *pDnsCl_glb_data,
		IN    dnscl_boolean_t      enable_trace,
		OUT   dnscl_error_t        *pError)
{

	/*Pointer to global data structure*/
	arDnsClient_global_data_st   *pGlob_data = ARDNSCL_NULL;

   /*variable for arDnsClient request data structure*/
	arDnsClient_request_st       request_data = 
                     {0,0,0,ARDNSCL_NULL,ARDNSCL_NULL,0,ARDNSCL_NULL};
	
	/*Variable for error*/
	dnscl_error_t       error = ARDNSCL_INIT_VAL;

	/*Variable for return type*/
	dnscl_return_t       ret_val = ARDNSCL_SUCCESS;

	/*Check for  error pointer*/
	if(ARDNSCL_NULL == pError)
	{
		return ARDNSCL_FAILURE;
	}

	/*Check for  global data pointer*/
	if(ARDNSCL_NULL == pDnsCl_glb_data)
	{
		/*retun FAILURE with appropriate error*/
		*pError =  ARDNSCL_ERROR_INVALID_GLB_DATA;
		return ARDNSCL_FAILURE;
	}

	/*Type cast global data*/       
	pGlob_data =(arDnsClient_global_data_st *)pDnsCl_glb_data;

	ARDNSCL_CLIB_PRINT (pGlob_data, ("Entered Function arDnsClient_set_trace\n"));

        ARDNSCL_MEMSET(&request_data, 0, ARDNSCL_SIZEOF(arDnsClient_request_st));

	/*check for invalid trace data*/
	if((ARDNSCL_TRUE != enable_trace) && (ARDNSCL_FALSE != enable_trace))
	{
		ARDNSCL_CLIB_PRINT (pGlob_data, ("Invalid Set Trace Value\n"));
		/*retun FAILURE with appropriate error*/
		*pError =  ARDNSCL_ERROR_DNS_INVALID_TRACE_DATA;
		return ARDNSCL_FAILURE;
	}

	/*Setting trace value in global data*/
	pGlob_data->clib_data.enable_trace = enable_trace; 

	/*Assign the value of trace data*/
	request_data.enableTrace = enable_trace;


	/*Set the value of API id*/
	request_data.api_id = ARDNSCL_API_SET_TRACE_REQ;

	/*Send data to application*/
	ret_val = arDnsClient_port_send_msg_to_dnscl(
                       pGlob_data,	
                       &request_data,
                       pError);

	if(ret_val == ARDNSCL_FAILURE)
	{
		ARDNSCL_CLIB_PRINT (pGlob_data, ("Failure In IPC Send\n"))
		arDnsClient_free_request (pGlob_data,&request_data,&error);
		return ARDNSCL_FAILURE;
	}

	/*Free the memory allocated for request structure*/
	arDnsClient_free_request (pGlob_data,&request_data,pError);

	ARDNSCL_CLIB_PRINT (pGlob_data, ("Exiting function arDnsClient_set_trace"))
	/*return Success*/
	return ARDNSCL_SUCCESS;
}

/****************************************************************************
* API Name: arDnsClient_clear_cache
* 
* Description: This API is invoked by the application for clearing the cache data
*              Option for Full clear or single entry clear is provided. 
* INPUT       :pDnscl_glb_data - The pointer to the global data of arDnsClient.
*             :pClear_cache - The pointer to clear cache information.  
* OUTPUT      :pError - Error pointer to be populated in case of error. 
*
* return : It will return ADNSCL_SUCCESS if successful else return ARDNSCL_FAILURE.
******************************************************************************/
dnscl_return_t  arDnsClient_clear_cache(
		IN    dnscl_void_t               *pDnsCl_glb_data,            
		IN    arDnsClient_clear_cache_st *pClear_cache,
		OUT   dnscl_error_t              *pError)
{
	/*Pointer to global data structure*/
	arDnsClient_global_data_st   *pGlob_data = ARDNSCL_NULL;

   /*variable for arDnsClient request data structure*/
	arDnsClient_request_st       request_data = 
                     {0,0,0,ARDNSCL_NULL,ARDNSCL_NULL,0,ARDNSCL_NULL};

	/*Variable for return type*/
	dnscl_return_t       ret_val =ARDNSCL_SUCCESS;

    /*Variable for error*/
	dnscl_error_t       error = ARDNSCL_INIT_VAL;

	/*Check for  error pointer*/
	if(ARDNSCL_NULL == pError)
	{
		return ARDNSCL_FAILURE;
	}

	/*Check for  global structure data pointer*/
	if(ARDNSCL_NULL == pDnsCl_glb_data)
	{
		/*retun FAILURE */
		*pError =  ARDNSCL_ERROR_INVALID_GLB_DATA;
		return ARDNSCL_FAILURE;
	}


	/*Type cast global data*/       
	pGlob_data =(arDnsClient_global_data_st*)pDnsCl_glb_data;

	ARDNSCL_CLIB_PRINT (pGlob_data, ("Entered Function arDnsClient_clear_cache\n"))

        ARDNSCL_MEMSET(&request_data, 0, ARDNSCL_SIZEOF(arDnsClient_request_st));

	/*Check for clear cache pointer*/
	if(ARDNSCL_NULL == pClear_cache)
	{
		ARDNSCL_CLIB_PRINT (pGlob_data, ("Invalid Cache data\n"))
		/*retun FAILURE with appropriate error*/
		*pError =  ARDNSCL_ERROR_INVALID_CACHE_DATA;
		return ARDNSCL_FAILURE;
	}
	
	/* Check the type of Clear Cache request */
	if((pClear_cache->clear_options != ARDNSCL_CLEAR_SINGLE_ENTRY)
		&&(pClear_cache->clear_options != ARDNSCL_CLEAR_ALL_ENTRY))
	{
		ARDNSCL_CLIB_PRINT (pGlob_data, ("Invalid Cache Clear Option\n"))
		/*retun FAILURE with appropriate error*/
		*pError =  ARDNSCL_ERROR_INVALID_CACHE_OPTION;
		return ARDNSCL_FAILURE;
	}

    ARDNSCL_MEMGET(request_data.pClearCache , ARDNSCL_SIZEOF(arDnsClient_clear_cache_st));

	if(ARDNSCL_NULL == request_data.pClearCache)
	{
	   ARDNSCL_CLIB_PRINT (pGlob_data, ("Memory Allocation Failure For Clear Cache of arDns Request Structure\n"))
		/*return FAILURE*/
		return ARDNSCL_FAILURE;
	}

	/*Assign the value of clear cache data pointer*/
	ARDNSCL_MEMCPY(request_data.pClearCache,
			pClear_cache,
			ARDNSCL_SIZEOF(arDnsClient_clear_cache_st));


	/*Set the value of API id*/
	request_data.api_id = ARDNSCL_API_CLEAR_CACHE_REQ;

	/*Send data to application*/
	ret_val = arDnsClient_port_send_msg_to_dnscl(
                         pGlob_data,
					     &request_data,
					     pError);

	/*Check for IPC send*/
	if(ret_val == ARDNSCL_FAILURE)
	{
		ARDNSCL_CLIB_PRINT (pGlob_data, ("Failure In IPC Send\n"))
		arDnsClient_free_request (pGlob_data,&request_data,&error);
		return ARDNSCL_FAILURE;
	}


	/*Free the memory allocated for request structure*/
	arDnsClient_free_request (pGlob_data,&request_data,&error);

	ARDNSCL_CLIB_PRINT (pGlob_data, ("Exiting Function arDnsClient_clear_cache\n"))
	/*return Success*/
	return ARDNSCL_SUCCESS;
}

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
 * return : It will return ADNSCL_SUCCESS if successful else return ARDNSCL_FAILURE.
 ******************************************************************************/
dnscl_return_t  arDnsClient_send_query (
		IN    dnscl_void_t          *pDnsCl_glb_data,
		IN    arDnsClient_query_st  *pQuery_data,
		IN    dnscl_uint8_t         service_name,
		IN    dnscl_uint8_t         protocol_name,
		OUT   dnscl_error_t         *pError)
{
    
    /*str buffer for service */
    dnscl_int8_t       str_buffer[ARDNSCL_MAX_QUERY_NAME_SIZE]={'\0',};
	
	/*Pointer to global data structure*/
	arDnsClient_global_data_st   *pGlob_data = ARDNSCL_NULL;

    /*variable for arDnsClient request data structure*/
	arDnsClient_request_st       request_data = 
                     {0,0,0,ARDNSCL_NULL,ARDNSCL_NULL,0,ARDNSCL_NULL};

	/*Variable for error*/
	dnscl_uint8_t       error = ARDNSCL_INIT_VAL;

	/*Variable for return type*/
	dnscl_return_t       ret_val =ARDNSCL_SUCCESS;

    /* Value of timestamp */
    dnscl_uint32_t         timeStamp = ARDNSCL_INIT_VAL;

	/*Check for  error pointer*/
	if(ARDNSCL_NULL == pError)
	{
		return ARDNSCL_FAILURE;
	}

	/*Check for  global data pointer*/
	if(ARDNSCL_NULL == pDnsCl_glb_data)
	{
		/*retun FAILURE with appropriate error*/
		*pError =  ARDNSCL_ERROR_INVALID_GLB_DATA;
		return ARDNSCL_FAILURE;
	}

	/*Type cast global data*/       
	pGlob_data =(arDnsClient_global_data_st*)pDnsCl_glb_data;

	ARDNSCL_CLIB_PRINT (pGlob_data, ("Entered Function arDnsClient_send_query\n")) 

        ARDNSCL_MEMSET(&request_data, 0, ARDNSCL_SIZEOF(arDnsClient_request_st));
	/*Check for arDns Query pointer  */
	if(ARDNSCL_NULL == pQuery_data)
	{
		ARDNSCL_CLIB_PRINT (pGlob_data, ("Invalid Query Data\n"))
		/*retun FAILURE with appropriate error*/
		*pError =  ARDNSCL_ERROR_DNS_INVALID_QUERY_DATA;
		return ARDNSCL_FAILURE;
	}	

    if(ARDNSCL_NULL == pQuery_data->pApp_query_ctx)
    {
		ARDNSCL_CLIB_PRINT (pGlob_data, ("Invalid Query App context\n"))
		/*retun FAILURE with appropriate error*/
		*pError =  ARDNSCL_ERROR_DNS_INVALID_QUERY_DATA;
		return ARDNSCL_FAILURE;
	}

    if((ARDNSCL_QUERY_TYPE_A != pQuery_data->query_type) &&
        (ARDNSCL_QUERY_TYPE_AAAA != pQuery_data->query_type) &&
        (ARDNSCL_QUERY_TYPE_SRV != pQuery_data->query_type) &&
        (ARDNSCL_QUERY_TYPE_NAPTR != pQuery_data->query_type))
    {
        ARDNSCL_CLIB_PRINT (pGlob_data, ("Invalid Query type\n"))
		/*retun FAILURE with appropriate error*/
		*pError =  ARDNSCL_ERROR_INVALID_QUERY_TYPE;
		return ARDNSCL_FAILURE;
    }

	ARDNSCL_MEMGET(request_data.pDnsQuery , ARDNSCL_SIZEOF(arDnsClient_query_st))

	if(ARDNSCL_NULL == request_data.pDnsQuery)
	{
		ARDNSCL_CLIB_PRINT (pGlob_data, ("Memory Allocation Failure For arDns Query Structure\n")) 

		*pError = ARDNSCL_ERROR_MEMORY_ALLOC_FAILURE; 
		return ARDNSCL_FAILURE;
	}
        
        /*Check for queryb type as srv*/
	if(ARDNSCL_QUERY_TYPE_SRV == pQuery_data->query_type )
    {
        /*check for service type */ 
        if(ARDNSCL_SERVICE_SIP == service_name)
        {
            ARDNSCL_STRCPY(str_buffer,"_sip.");
        }
        else if (ARDNSCL_SERVICE_SIPS == service_name)
        {
            ARDNSCL_STRCPY(str_buffer,"_sips.");
        }
        else
        {
            ARDNSCL_CLIB_PRINT (pGlob_data, ("Invalid Service Value Hence Ignoring\n"))
        }

        /*check for protocol type */ 
        if(ARDNSCL_PROTOCOL_UDP == protocol_name )
        {
            ARDNSCL_STRCAT(str_buffer,"_udp.");
        }
        else if( ARDNSCL_PROTOCOL_TCP == protocol_name)
        {
            ARDNSCL_STRCAT(str_buffer,"_tcp.");
        }
        else
        {
            ARDNSCL_CLIB_PRINT (pGlob_data, ("Invalid Protocol Name Hence Ignoring\n"))
        }
        request_data.pDnsQuery->query_type =  pQuery_data->query_type;
        ARDNSCL_STRNCAT(str_buffer,
                (const dnscl_int8_t *)(pQuery_data->query_string.str_val),
                ARDNSCL_MAX_STR_LEN);

        ARDNSCL_STRNCPY((dnscl_int8_t *)(request_data.pDnsQuery->query_string.str_val),
                str_buffer,ARDNSCL_MAX_STR_LEN);

        request_data.pDnsQuery->query_string.str_len = (dnscl_uint8_t)ARDNSCL_STRLEN(str_buffer);
        request_data.pDnsQuery->pApp_query_ctx = pQuery_data->pApp_query_ctx ;
    }
	else
	{
		/*Assign the value of query data pointer*/
		ARDNSCL_MEMCPY(request_data.pDnsQuery,
				pQuery_data,
				ARDNSCL_SIZEOF(arDnsClient_query_st));
	}
	
	/*Set the value of API id*/
	request_data.api_id = ARDNSCL_API_SEND_QUERY_REQ;

    /*Cache counter updation*/
    arDnsClient_port_get_timestamp (pGlob_data, &timeStamp);

	if(ARDNSCL_INIT_VAL ==  pGlob_data->clib_data.cache_counter)
	{
         request_data.cache_counter = ARDNSCL_INIT_VAL;
	}
	else
	{
         request_data.cache_counter =  
                  timeStamp -  pGlob_data->clib_data.cache_counter;

	}
    pGlob_data->clib_data.cache_counter = timeStamp;


	/*Send data to application*/
	ret_val = arDnsClient_port_send_msg_to_dnscl(
                       pGlob_data, 
		               &request_data,
                       pError);

	/*Check for IPC send*/
	if(ret_val == ARDNSCL_FAILURE)
	{
		ARDNSCL_CLIB_PRINT (pGlob_data, ("Failure In IPC Send\n"))
		arDnsClient_free_request (pGlob_data,&request_data,&error);
		return ARDNSCL_FAILURE;
	}

	/*Free memory allocated for character buffer*/

	/*Free the memory allocated for request structure*/
	arDnsClient_free_request (pGlob_data,&request_data,&error);

	ARDNSCL_CLIB_PRINT (pGlob_data, ("Exiting Function arDnsClient_send_query\n")) 
	/*return Success*/
	return ARDNSCL_SUCCESS;
}


/****************************************************************************
 * API Name: arDnsClient_free_response
 * 
 * Description: This API is invoked by the application for freeing the response
 *              structure. 
 * INPUT       :pDnscl_glb_data - The pointer to the global data of arDnsClient.
 *             :pResponse - The pointer to response structure.  
 * OUTPUT      :pError - Error pointer to be populated in case of error. 
 *
 * return : It will return ADNSCL_SUCCESS if successful else return ARDNSCL_FAILURE.
 ******************************************************************************/
dnscl_return_t  arDnsClient_free_response (
		IN    dnscl_void_t            *pDnsCl_glb_data,
		IN    arDnsClient_response_st  *pResponse,
		OUT   dnscl_uint8_t           *pError)
{
	/*Pointer to global variable*/
	arDnsClient_global_data_st  *pGlob_data =  ARDNSCL_NULL;

	/*Tempory variable for resouce records*/
	arDnsClient_dns_rr_list_st  *p_temp_answer_records = ARDNSCL_NULL;       
	arDnsClient_dns_rr_list_st  *p_temp1_answer_records = ARDNSCL_NULL;       


	/*Type cast global data*/
	if(ARDNSCL_NULL != pDnsCl_glb_data)
		pGlob_data =(arDnsClient_global_data_st*)pDnsCl_glb_data;
	if(ARDNSCL_NULL != pGlob_data)
	{
		ARDNSCL_CLIB_PRINT (pGlob_data, ("Entered Function arDnsClient_free_response\n"))
	}
    /* this field is for future use , statement to remove compilation warning*/
    if(ARDNSCL_NULL != pError)
    {
        *pError = *pError;
    }

	/*Check for request data structure pointer*/
	if(ARDNSCL_NULL != pResponse)
	{
        if(ARDNSCL_NULL != pResponse->pQuery)
	    {	
			/*Freeing memory for query context  data structure*/
			ARDNSCL_MEMFREE(pResponse->pQuery);
        }

		/*Check for query context in response structure*/
		if(ARDNSCL_NULL != pResponse->pDnsQueryResp)
		{
			/*Check for answer records in response structure*/
			if(ARDNSCL_NULL!= pResponse->pDnsQueryResp->pAnswerRecords)
			{
				p_temp_answer_records  =
					pResponse->pDnsQueryResp->pAnswerRecords;

				/*Loop to free all memory from answer record linked list*/
				while(ARDNSCL_NULL != p_temp_answer_records)
				{
                    p_temp1_answer_records = 
						p_temp_answer_records->pNext;
				    
                    /*Freeing memory for linked list*/
				    ARDNSCL_MEMFREE(p_temp_answer_records);
                         p_temp_answer_records =  p_temp1_answer_records;
				}
				/*Freeing memory for answer records*/
				pResponse->pDnsQueryResp->pAnswerRecords = ARDNSCL_NULL;
			}
            ARDNSCL_MEMFREE(pResponse->pDnsQueryResp);
		}

        ARDNSCL_MEMFREE((dnscl_void_t *)pResponse);
	}

	if(ARDNSCL_NULL != pGlob_data)
	{
		ARDNSCL_CLIB_PRINT (pGlob_data, ("Exiting Function arDnsClient_free_response\n"))
	}
	/*return Success*/
	return ARDNSCL_SUCCESS;
}
/****************************************************************************
 * API Name: arDnsClient_deinit
 * 
 * Description: This API is invoked by the application to deinitialize the utility
 * INPUT       :pDnscl_glb_data - The pointer to the global data of arDnsClient.
 * OUTPUT      :pError - Error pointer to be populated in case of error. 
 *
 * return : It will return ADNSCL_SUCCESS if successful else return ARDNSCL_FAILURE.
 ******************************************************************************/
dnscl_return_t  arDnsClient_deinit (
		IN    dnscl_void_t   *pDnsCl_glb_data,
		OUT   dnscl_error_t  *pError)
{
	/*Pointer to global data structure*/
	arDnsClient_global_data_st   *pGlob_data = ARDNSCL_NULL;
	arDnsClient_global_data_st   glb_data;

    /* Fix for SPR-20412: Initialized glb_data.
     * uninitialization of glb_data was resulted into FMRs and memory leak.
     */
    ARDNSCL_MEMSET(&glb_data,0,ARDNSCL_SIZEOF(arDnsClient_global_data_st));

    /*variable for arDnsClient request data structure*/
	arDnsClient_request_st       request_data = 
                     {0,0,0,ARDNSCL_NULL,ARDNSCL_NULL,0,ARDNSCL_NULL};

    /* Value of error*/
	dnscl_uint8_t 		error = ARDNSCL_INIT_VAL;

	/*Value of return type*/
	dnscl_return_t   	ret_val   = ARDNSCL_SUCCESS;

	/*Check for  error pointer*/
	if(ARDNSCL_NULL == pError)
	{
		return ARDNSCL_FAILURE;
	}
	/*Check for  global data pointer*/
	if(ARDNSCL_NULL == pDnsCl_glb_data)
	{
		/*retun FAILURE*/
		*pError =  ARDNSCL_ERROR_INVALID_GLB_DATA;
		return ARDNSCL_FAILURE;
	}

	/*Type cast global data*/       
	pGlob_data =(arDnsClient_global_data_st*)pDnsCl_glb_data;

	ARDNSCL_CLIB_PRINT (pGlob_data, ("Entered Function arDnsClient_deinit\n"))

        ARDNSCL_MEMSET(&request_data, 0, ARDNSCL_SIZEOF(arDnsClient_request_st));
	/*fill api id in request structure*/
	request_data.api_id = ARDNSCL_API_DEINIT_REQ;

	/*Resetting cache counter value*/
	pGlob_data->clib_data.cache_counter= 0;


    /* make a copy of global data */
    ARDNSCL_MEMCPY(&glb_data, pGlob_data, 
            ARDNSCL_SIZEOF(arDnsClient_global_data_st));

	/*Send data to application*/
	ret_val = arDnsClient_port_send_msg_to_dnscl(
                        pGlob_data,
			            &request_data, 
		                pError);
	/*Check for IPC send*/
	if(ret_val == ARDNSCL_FAILURE)
	{
		ARDNSCL_CLIB_PRINT (pGlob_data, ("Failure In IPC Send\n"));
		arDnsClient_free_request (&glb_data,&request_data,&error);
		return ARDNSCL_FAILURE;
	}

	/*closeing ipc socket*/
	arDnsClient_port_close_ipc(
            &glb_data, glb_data.clib_data.ipc_fd, pError);

	/*free the request*/
	arDnsClient_free_request (&glb_data,&request_data,&error);

	/*return succsss*/
	return ARDNSCL_SUCCESS;
}

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
                    OUT   arDnsClient_response_st  **ppResponse,
                    OUT   dnscl_error_t          *pError)
{
    /*Value of return type*/
	dnscl_return_t   	ret_val   = ARDNSCL_SUCCESS;

    /*Pointer to global data structure*/
	arDnsClient_global_data_st   *pGlob_data = ARDNSCL_NULL;

	/*Check for  error pointer*/
	if(ARDNSCL_NULL == pError)
	{
		return ARDNSCL_FAILURE;
	}

	/*Check for  global data pointer*/
	if(ARDNSCL_NULL == pDnsCl_glb_data)
	{
		/*retun FAILURE*/
		*pError =  ARDNSCL_ERROR_INVALID_GLB_DATA;
		return ARDNSCL_FAILURE;
	}

    /*Type cast global data*/       
	pGlob_data =(arDnsClient_global_data_st*)pDnsCl_glb_data;

    /* call the porting function to return reponse 
     * structure.
     */ 
    ret_val = arDnsClient_port_recv_msg_frm_dnscl(
                                   pGlob_data,
                                   ppResponse,
                                   pError);

    return ret_val;
}

