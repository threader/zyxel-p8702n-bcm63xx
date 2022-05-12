/******************************************************************************
*                                    
* FILE NAME    : arDnsClient_port_utils.c                
*                                    
* DESCRIPTION  : Contains the arDnsClient porting utility routines  
*                                    
* Revision History :                            
*
*  DATE            NAME                 REFERENCE       REASON        
* ------         ------------           ---------     ----------
* 05-Feb-2009    Anuradha Gupta         None          Initial
* 11-Feb-2009    Abbas Haque            None          addde code for DNS utility
* 17-Feb-2009    Anuradha Gupta         Coding        review comments incorporation
*
* Copyright 2009, Aricent.                       
*                                    
*******************************************************************************/

#if defined(ARDNSCL_PORT_LINUX)
#include <sys/time.h>
#endif

#if defined(ARDNSCL_PORT_WINDOWS)||defined(ARDNSCL_PORT_VXWORKS)
#include <time.h>
#endif

#include "arDnsClient_api.h"
#include "arDnsClient_port_prototypes.h"
#include "arDnsClient_cmn_prototypes.h"

/****************************************************************************
 * API Name: arDnsClient_get_timestamp
 * 
 * Description: This API is invoked to get time stamp value 
 *              request. 
 * INPUT       :pTimeStamp - The pointer to time stamp.
 *
 * return : It will return ADNSCL_SUCCESS if successful else return ARDNSCL_FAILURE.
 ******************************************************************************/
dnscl_return_t
arDnsClient_port_get_timestamp (
        dnscl_void_t   *pDns_glb_data,
        dnscl_uint32_t *pTimeStamp)
{

#if defined(ARDNSCL_PORT_WINDOWS)||defined(ARDNSCL_PORT_VXWORKS)
	time_t  current_time;
#endif

   arDnsClient_global_data_st   *pDnsCl_glb_data;
   
   pDnsCl_glb_data = (arDnsClient_global_data_st *)pDns_glb_data;

   ARDNSCL_PRINT(pDnsCl_glb_data, ("Entered function arDnsClient_port_get_timestamp\n"))

#if defined(ARDNSCL_PORT_LINUX)

	/*Variable to get Timestamp*/
	struct timeval current_time;
       gettimeofday(&current_time, ARDNSCL_NULL);

	*pTimeStamp = current_time.tv_sec;
#endif
#if defined(ARDNSCL_PORT_WINDOWS)||defined(ARDNSCL_PORT_VXWORKS)

         time(&current_time);
	*pTimeStamp = (dnscl_uint32_t)current_time;
#endif

   ARDNSCL_PRINT(pDnsCl_glb_data, ("Exiting function arDnsClient_port_get_timestamp\n"))
	return ARDNSCL_SUCCESS;
}

/*****************************************************************************
 * FUNCTION :   arDnsClient_port_send_msg_to_dnscl
 *
 * Description: This function is used to pack and send message from CLIB to DNS Client.
 *                            
 * Return:      It will return ARDNSCL_SUCCESS if successful 
 *              else return ARDNSCL_FAILURE.
 ********************************************************************************/
 dnscl_return_t    arDnsClient_port_send_msg_to_dnscl(
	                   dnscl_void_t                *pDns_glb_data,
                       arDnsClient_request_st      *pRequest,
                       dnscl_error_t                *pError)
 {
    dnscl_return_t ret_val = ARDNSCL_SUCCESS;
    dnscl_uint8_t                *pBuffer = ARDNSCL_NULL;
    dnscl_uint16_t                buff_len = ARDNSCL_INIT_VAL;

    arDnsClient_global_data_st   *pDnsCl_glb_data = 
          (arDnsClient_global_data_st *)pDns_glb_data;

	ARDNSCL_PRINT(pDnsCl_glb_data, ("Entered function arDnsClient_port_send_msg_to_dnscl\n"))

    if(ARDNSCL_FAILURE == arDnsClient_port_pack_request(
                            pDnsCl_glb_data, pRequest,
                            &pBuffer,
					        &buff_len, pError))
    {
	     ARDNSCL_PRINT(pDnsCl_glb_data, ("Failure in Msg Packing \n"))
         ret_val = ARDNSCL_FAILURE;
    }
    else if(ARDNSCL_FAILURE == arDnsClient_port_send_message(
                            pDnsCl_glb_data,
                            pDnsCl_glb_data->clib_data.ipc_fd,
                            pBuffer,
                            buff_len,
                            &(pDnsCl_glb_data->dns_client_addr),
                            pError))
    {
         ret_val = ARDNSCL_FAILURE;
    }
      
    if(ARDNSCL_NULL != pBuffer)
    {
        /* free the message buffer */
        ARDNSCL_MEMFREE(pBuffer);
    }


	return ret_val;
}


/*****************************************************************************
 * FUNCTION :   arDnsClient_port_send_msg_to_app
 *
 * Description: This function is used to pack and send message from DNS Client to App.
 *                            
 * Return:      It will return ARDNSCL_SUCCESS if successful 
 *              else return ARDNSCL_FAILURE.
 ********************************************************************************/
 dnscl_return_t    arDnsClient_port_send_msg_to_app(
	                   dnscl_void_t                 *pDns_glb_data,
                       arDnsClient_response_st      *pResponse,
                       dnscl_error_t                *pError)
 {
    dnscl_return_t ret_val = ARDNSCL_SUCCESS;
    dnscl_uint8_t                *pBuffer = ARDNSCL_NULL;
    dnscl_uint16_t                buff_len = ARDNSCL_INIT_VAL;

    arDnsClient_global_data_st   *pDnsCl_glb_data = 
          (arDnsClient_global_data_st *)pDns_glb_data;

	ARDNSCL_PRINT(pDnsCl_glb_data, ("Entered function arDnsClient_port_send_msg_to_app\n"))

    if(ARDNSCL_FAILURE == arDnsClient_port_pack_response(
                            pDnsCl_glb_data, pResponse,
                            &pBuffer,
					        &buff_len, pError))
    {
	     ARDNSCL_PRINT(pDnsCl_glb_data, ("Failure in Msg Packing \n"))
         ret_val = ARDNSCL_FAILURE;
    }
    else if(ARDNSCL_FAILURE == arDnsClient_port_send_message(
                            pDnsCl_glb_data,
                            pDnsCl_glb_data->dns_client_data.ipc_fd,
                            pBuffer,
                            buff_len,
                            &(pDnsCl_glb_data->clib_addr),
                            pError))
    {
	     ARDNSCL_PRINT(pDnsCl_glb_data, ("Failure in IPC send \n"))
         ret_val = ARDNSCL_FAILURE;
    }

    if(ARDNSCL_NULL != pBuffer)
    {
        /* free the message buffer */
        ARDNSCL_MEMFREE(pBuffer);
    }

	ARDNSCL_PRINT(pDnsCl_glb_data, ("Exiting function arDnsClient_port_send_msg_to_app\n"))

	/*return SUCCESS*/
	return ret_val;
}

/*****************************************************************************
 * FUNCTION :   arDnsClient_port_recv_msg_frm_app
 *
 * Description: This function is used to receive and unpack message from application.
 *                            
 * Return:      It will return ARDNSCL_SUCCESS if successful
 *             else return ARDNSCL_FAILURE.
 ********************************************************************************/
 dnscl_return_t   arDnsClient_port_recv_msg_frm_app (
		               dnscl_void_t                *pDns_glb_data,
                       arDnsClient_request_st      **ppRequest,
		               dnscl_error_t              *pError)
 {
    dnscl_return_t   ret_val = ARDNSCL_SUCCESS;

    dnscl_uint8_t    buffer[ARDNSCL_MAX_RECV_BUFF_SIZE];
	dnscl_uint32_t   msg_len = ARDNSCL_MAX_RECV_BUFF_SIZE;
	dnscl_uint16_t   buff_len = ARDNSCL_INIT_VAL;

    arDnsClient_global_data_st   *pDnsCl_glb_data = 
          (arDnsClient_global_data_st *)pDns_glb_data;

    if(ARDNSCL_FAILURE == arDnsClient_port_recv_message(
                     pDns_glb_data,
                     pDnsCl_glb_data->dns_client_data.ipc_fd,
                     buffer,
                     &msg_len,
                     pError))
    {
		ret_val = ARDNSCL_FAILURE;
    }
    /* unpack the request */
    else if(ARDNSCL_FAILURE == arDnsClient_port_unpack_request(
                     pDns_glb_data,
                     buffer,
                     ppRequest,
                     &buff_len,
                     pError))
    {
		ret_val = ARDNSCL_FAILURE;
    }

	return ret_val;
}

/*****************************************************************************
 * FUNCTION :   arDnsClient_port_recv_msg_frm_dnscl
 *
 * Description: This function is used to receive and unpack message from Dns Clinet.
 *                            
 * Return:      It will return ARDNSCL_SUCCESS if successful
 *             else return ARDNSCL_FAILURE.
 ********************************************************************************/
dnscl_return_t   arDnsClient_port_recv_msg_frm_dnscl (
		               dnscl_void_t               *pDns_glb_data,
                       arDnsClient_response_st    **ppResponse,
		               dnscl_error_t              *pError)
 {
    dnscl_return_t   ret_val = ARDNSCL_SUCCESS;

    dnscl_uint8_t    buffer[ARDNSCL_MAX_RECV_BUFF_SIZE];
	dnscl_uint32_t   msg_len = ARDNSCL_MAX_RECV_BUFF_SIZE;
	dnscl_uint16_t   buff_len = ARDNSCL_INIT_VAL;

    arDnsClient_global_data_st   *pDnsCl_glb_data = 
          (arDnsClient_global_data_st *)pDns_glb_data;

    /* receive the message from IPC */
    if(ARDNSCL_FAILURE == arDnsClient_port_recv_message(
                     pDnsCl_glb_data,
                     pDnsCl_glb_data->clib_data.ipc_fd,
                     buffer,
                     &msg_len,
                     pError))
    {
		ret_val = ARDNSCL_FAILURE;
    }
    /* unpack the response */
    else if(ARDNSCL_FAILURE == arDnsClient_port_unpack_response(
                     pDnsCl_glb_data,
                     buffer,
                     ppResponse,
                     &buff_len,
                     pError))
    {
		ret_val = ARDNSCL_FAILURE;
    }

	return ret_val;
}

/****************************************************************************
 * API Name: arDnsClient_port_generate_alarm
 * 
 * Description: This function is used to generate alarm 
 * INPUT       :pDnsCl_glb_data - The pointer to global data structure. 
 *             :pReq_data  -      Request data structure
 *             :error  -          error code
 * return : It will return ADNSCL_SUCCESS if successful else return ARDNSCL_FAILURE.
 ******************************************************************************/
dnscl_void_t
arDnsClient_generate_port_alarm (
		dnscl_void_t            *pDns_glb_data,
		arDnsClient_request_st  *pReq_data,
		dnscl_error_t            error)
{
    /*Pointer to global data structure*/
    arDnsClient_global_data_st   *pDnsCl_glb_data = ARDNSCL_NULL;

    pDnsCl_glb_data =(arDnsClient_global_data_st*)pDns_glb_data;

	/* To remove warnings */
	error = error;

    if(ARDNSCL_NULL == pReq_data)
    {
        ARDNSCL_PRINT(pDnsCl_glb_data,
                ("Unkown API not handled because of error code :  %d\n", error))
        return;    
    }

    switch(pReq_data->api_id)
    {
        case ARDNSCL_API_SET_CONF_REQ:
            {
                ARDNSCL_PRINT(pDnsCl_glb_data,("ARDNSCL_API_SET_CONF_REQ API not handled because of error code :  %d\n  which has parameter server address : %s\n",error, pReq_data->pConfData->dns_server_addr.ipAddress.str_val))
                    break;
            }
        case ARDNSCL_API_SET_TRACE_REQ:
            {
                ARDNSCL_PRINT(pDnsCl_glb_data,("ARDNSCL_API_SET_TRACE_REQ API not handled because of error code : %d\n which has parameter trace data : %d\n",error,pReq_data->enableTrace))
                    break;
            }
        case ARDNSCL_API_CLEAR_CACHE_REQ:
            {
                if(ARDNSCL_CLEAR_SINGLE_ENTRY & pReq_data->pClearCache->clear_options)
                {
                    ARDNSCL_PRINT(pDnsCl_glb_data,("ARDNSCL_API_CLEAR_CACHE_REQ API not handled because of error code : %d\n which has parameter domain name : %s\n query type : %d",error,pReq_data->pClearCache->domain_name.str_val,pReq_data->pClearCache->query_type))
                }
                else
                {
                    ARDNSCL_PRINT(pDnsCl_glb_data,("ARDNSCL_API_CLEAR_CACHE_REQ API not handled because of error code : %d\n ",error))
                }
                break;
            }
        case ARDNSCL_API_SEND_QUERY_REQ:
            {
                if(ARDNSCL_NULL != pReq_data->pDnsQuery)
                {
                ARDNSCL_PRINT(pDnsCl_glb_data,("ARDNSCL_API_SEND_QUERY_REQ API not handled because of error code : %d\n which has parameters query type : %d\n domain name : %s\n application context : %d\n",error,pReq_data->pDnsQuery->query_type,pReq_data->pDnsQuery->query_string.str_val,pReq_data->pDnsQuery->pApp_query_ctx))
                }
                    break;
            }
        case ARDNSCL_API_DEINIT_REQ:
            {
                ARDNSCL_PRINT(pDnsCl_glb_data, ("ARDNSCL_API_DEINIT_REQ API not handled because of error code : %d\n ",error))
                    break;
            }
        default :
            {
                ARDNSCL_PRINT(pDnsCl_glb_data, ("Invalid API ID not handled"))
            }
    }
    return;
}
