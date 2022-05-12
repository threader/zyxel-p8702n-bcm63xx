/******************************************************************************
*                                    
* FILE NAME    : arDnsClient_queryh_intf.c                
*                                    
* DESCRIPTION  : Contains the arDnsClient Query Handler interface routines  
*                                    
* Revision History :                            
*
*  DATE            NAME                 REFERENCE       REASON        
* ------         ------------           ---------     ----------
* 05-Feb-2009    Anuradha Gupta         None          Initial
*
* Copyright 2009, Aricent.                       
*                                    
*******************************************************************************/

#include "arDnsClient_api.h"
#include "arDnsClient_cmn_prototypes.h"
#include "arDnsClient_queryh_prototypes.h"
#include "arDnsClient_port_prototypes.h"

/*******************************************************************************
 * FUNCTION: arDnsClient_start
 * 
 * Description: This is the query handler function for query thread. It will open
 *		IPC channel with Application thread. It will continuously Service 
 *		requests form Application Thread. Look up Cache databases for
 * 		queries and request for new DNS queries
 *
 * Return : nothing
 * 
 *******************************************************************************/
#ifndef ARDNSCL_PORT_WINDOWS
dnscl_void_t   *arDnsClient_start (
       IN dnscl_void_t  *pArg)
#else

dnscl_int32_t arDnsClient_start (
       IN dnscl_void_t  *pArg)
#endif
{
	/* Structure for storing the Global Data passed to Query Thread */
	arDnsClient_global_data_st *pGlobal_data = (arDnsClient_global_data_st *)pArg;

    dnscl_return_t  ret_val = ARDNSCL_SUCCESS;

    dnscl_error_t  error = ARDNSCL_INIT_VAL;

    /* Structure for query request */
	arDnsClient_request_st *pRequest = ARDNSCL_NULL;

    dnscl_uint8_t            api_id = 0;

	/* Check if Global Data passed is null */
	if(ARDNSCL_NULL == pGlobal_data)
	{
#ifndef ARDNSCL_PORT_WINDOWS
	    return ARDNSCL_NULL;
#else
	    return 0;
#endif
	}

	while(1)
    {
        /* Receive message from IPC channel */
        ret_val = arDnsClient_port_recv_msg_frm_app(
                (dnscl_void_t *)pGlobal_data,
                &pRequest,
                &error);

        if(ARDNSCL_FAILURE == ret_val)
        {
            /* raise alarm about which API has failed */
            arDnsClient_generate_port_alarm(pGlobal_data,pRequest,error);
            ret_val = ARDNSCL_SUCCESS;

            /*release memory of free request*/
            if(ARDNSCL_NULL != pRequest)
            {
                arDnsClient_free_request(pGlobal_data , pRequest, &error);
            }
            break;
        }

        if(ARDNSCL_API_DEINIT_REQ == pRequest->api_id)
        {
            api_id = pRequest->api_id;
        }

        ret_val = arDnsClient_process_request(
                (dnscl_void_t *)pGlobal_data,
                pRequest,
                &error);

         /* Free request */
         arDnsClient_free_request(pGlobal_data , pRequest, &error);
         ARDNSCL_MEMFREE(pRequest);

        if(ARDNSCL_FAILURE == ret_val)
        {
            break;
        }
        else if(ARDNSCL_API_DEINIT_REQ == api_id)
        {
            /*exit query thread */
            break;
        }
    }

    /*Free Global Data */
    ARDNSCL_MEMFREE(pGlobal_data);

#ifndef ARDNSCL_PORT_WINDOWS
    return ARDNSCL_NULL;
#else
    return 1;
#endif
}


/*******************************************************************************
 * FUNCTION: arDnsClient_process_request
 * 
 * Description: This is the entry point function for query handling.
 *		IPC channel with Application thread. It will continuously Service 
 *		requests form Application Thread. Look up Cache databases for
 * 		queries and request for new DNS queries
 *
 * Return : returns ARDNSCL_SUCCESS / ARDNSCL_FAILURE 
 * 
 *******************************************************************************/
dnscl_return_t   arDnsClient_process_request(
                    IN    dnscl_void_t 		      *pDnsCl_glb_data,
                    INOUT  arDnsClient_request_st  *pRequest,
                    OUT   dnscl_error_t      *pError)
 {
    dnscl_return_t    ret_val = ARDNSCL_SUCCESS;

    /*Pointer to global data structure*/
    arDnsClient_global_data_st   *pGlobal_data = ARDNSCL_NULL;

    /* Variable storing Error returned from IPC creation */
	dnscl_error_t error = ARDNSCL_INIT_VAL;

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
    pGlobal_data =(arDnsClient_global_data_st*)pDnsCl_glb_data;

    /* this code leg shall be executed at Dns client start up */
    if(ARDNSCL_NULL == pGlobal_data->dns_client_data.pCache_db)
    {
        /* this means , system is starting up so set the trace value as
         * per default.
         */ 
        pGlobal_data->dns_client_data.enable_trace = ARDNSCL_DEFAULT_TRACE_VAL;
        pGlobal_data->dns_client_data.dns_query_fd = -1;

        /* Allocate memory and initialize Cache Database */
        ret_val = arDnsClient_create_cache(pGlobal_data,pError);

        /* If there is failure in cache create */ 
        if(ARDNSCL_FAILURE == ret_val)
        {
            ARDNSCL_PRINT(pGlobal_data,("Failure in create cache\n"));
            return ret_val;
        }
    }

    switch(pRequest->api_id)
    {
        case ARDNSCL_API_SET_CONF_REQ	:
            /* Request for Setting Configuration */
            ARDNSCL_PRINT(pGlobal_data,
                    ("Request for Changing Configuration Information\n"))

            if(ARDNSCL_NULL == pGlobal_data->dns_client_data.pConfig_data)
            {
                ARDNSCL_MEMGET(pGlobal_data->dns_client_data.pConfig_data, 
                        ARDNSCL_SIZEOF(arDnsClient_conf_st))
               if(ARDNSCL_NULL == pGlobal_data->dns_client_data.pConfig_data)
               {
                    ARDNSCL_PRINT(pGlobal_data,("Failure in memory allocation\n"))
                    *pError = ARDNSCL_ERROR_MEMORY_ALLOC_FAILURE;
                    break;
               }
            }

            if(ARDNSCL_NULL != pGlobal_data->dns_client_data.pConfig_data)
            {
                ARDNSCL_MEMCPY(pGlobal_data->dns_client_data.pConfig_data,
                        pRequest->pConfData,
                        ARDNSCL_SIZEOF(arDnsClient_conf_st));

                /* Set DNS server for DNS query requests */
                ret_val = arDnsClient_port_set_dns_server(pGlobal_data,
                        &pGlobal_data->dns_client_data.pConfig_data->dns_server_addr,
                        &error);
            }

            break;

        case ARDNSCL_API_SET_TRACE_REQ	:

            /* Request for Setting/Unsetting trace */
            ARDNSCL_PRINT(pGlobal_data,("Request for Changing Trace Information\n"))

            /*set/Unset Trace as in request */
            pGlobal_data->dns_client_data.enable_trace = pRequest->enableTrace;

            break;									 

        case ARDNSCL_API_CLEAR_CACHE_REQ:
            /* Request for clearing Cache Database for single or all entries */
            /* check weather a single entry needs to be removed */
            ARDNSCL_PRINT(pGlobal_data,("Request for Clearing cache\n"))
            if(pRequest->pClearCache->clear_options == ARDNSCL_CLEAR_SINGLE_ENTRY)
            {
                ret_val = arDnsClient_remove_entry(&pRequest->pClearCache->domain_name,
                        pRequest->pClearCache->query_type, 
                        pGlobal_data,pError);
            }
            else
            {
                /*Clear Cache Database */
                ret_val = arDnsClient_cache_clear(pGlobal_data,pError);
            }
            break;

        case ARDNSCL_API_SEND_QUERY_REQ	:
            ret_val = arDnsClient_handle_send_query(pGlobal_data,
                    pRequest,
                    pError);
            if(ARDNSCL_FAILURE == ret_val)
            {
                ARDNSCL_PRINT(pGlobal_data,("Send DNS Query failed \n"));
            }	
            break;

        case ARDNSCL_API_DEINIT_REQ	:
            {
                /* requesty for Deinitialization and exiting query thread */
                ARDNSCL_PRINT(pGlobal_data,
                        ("Request for Deinitializing Query Thread\n"));

                /*Free Configuration Data if not ARDNSCL_NULL*/
                if(ARDNSCL_NULL != pGlobal_data->dns_client_data.pConfig_data)
                {
                    ARDNSCL_MEMFREE(pGlobal_data->dns_client_data.pConfig_data);
                }

                /*Close IPC socket */
                arDnsClient_port_close_ipc(pGlobal_data,
                        pGlobal_data->dns_client_data.ipc_fd,&error); 

		        /*Close DNS query socket */
                arDnsClient_port_close_ipc(pGlobal_data,
			            pGlobal_data->dns_client_data.dns_query_fd,
			            &error);

                /*Clear Cache Entries */
                arDnsClient_cache_clear(pGlobal_data,&error);

                /*Remove Cache */
                ARDNSCL_MEMFREE(pGlobal_data->dns_client_data.pCache_db);
                return ARDNSCL_SUCCESS;
            }

        default:
            /* discard in case of wrong API Id */ 
            ARDNSCL_PRINT(pGlobal_data,("Invalid API Id\n"));
            ret_val = ARDNSCL_FAILURE;
            break;
    };

    if(ARDNSCL_FAILURE == ret_val)
    {
       /* raise alarm about which API has failed */
       arDnsClient_generate_port_alarm(pGlobal_data,pRequest,error);
       ret_val = ARDNSCL_SUCCESS; 
    }
 
    return ret_val;
}

