/******************************************************************************
*                                    
* FILE NAME    : arDnsClient_queryh_utils.c                
*                                    
* DESCRIPTION  : Contains the arDnsClient Query handler utility routines  
*                                    
* Revision History :                            
*
*  DATE            NAME                 REFERENCE       REASON        
* ------         ------------           ---------     ----------
* 05-Feb-2009    Anuradha Gupta         None          Initial
* 14-Feb-2009    Anuradha Gupta         LLD           Corrected implementation
*
* Copyright 2009, Aricent.                       
*                                    
*******************************************************************************/

#include "arDnsClient_api.h"
#include "arDnsClient_queryh_prototypes.h"
#include "arDnsClient_cmn_prototypes.h"
#include "arDnsClient_port_prototypes.h"

/*******************************************************************************
 * FUNCTION: arDnsClient_free_rr_list
 * 
 * Description:This function deletes all the resource records from the rr_list 	
 *
 * Return : Return ARDNSCL_SUCCESS if all the records were deleted 
 *******************************************************************************/
dnscl_return_t arDnsClient_free_rr_list(arDnsClient_dns_rr_list_st *dns_rr_list)
{
	/* Variable for traversing the rr_list */
	arDnsClient_dns_rr_list_st *p_temp_rr_list = dns_rr_list;
	/* Variable for deleting a record from the rr_list */
	arDnsClient_dns_rr_list_st *p_del_rr_list = p_temp_rr_list;
	
	/* Delete all nodes from rr_list*/
	while(ARDNSCL_NULL != p_temp_rr_list)
	{
		p_temp_rr_list = p_temp_rr_list->pNext;
		ARDNSCL_MEMFREE(p_del_rr_list);
		p_del_rr_list = p_temp_rr_list;
	}
	
	/* Set dns_rr_list to NULL */
	dns_rr_list = ARDNSCL_NULL;
	
	return ARDNSCL_SUCCESS;
}

/*******************************************************************************
 * FUNCTION: arDnsClient_handle_send_query
 * 
 * Description: This function handles send query requests 
 *
 * Return : Return ARDNSCL_SUCCESS if all the records were deleted
 *******************************************************************************/
dnscl_return_t arDnsClient_handle_send_query(arDnsClient_global_data_st *pGlobalData,
					     arDnsClient_request_st 	*pRequest,
					     dnscl_error_t 		 *pError)
					     
{
   /* Structure for DNS query response*/
   arDnsClient_response_st	*pResp = ARDNSCL_NULL;

   /* Structure for storing Answer records of DNS query*/
   arDnsClient_dns_rr_list_st *p_answer_records = ARDNSCL_NULL;

   /* Return variable for storing the return value while calling functions */
   dnscl_return_t ret_val = ARDNSCL_SUCCESS;
	
   /* Variable storing Error returned from IPC creation */
   dnscl_error_t error = ARDNSCL_INIT_VAL;

   /* Variable for storing the number of resource records */
   dnscl_uint8_t	num_records = ARDNSCL_INIT_VAL;

   /* pointer to the buffer for DNS query */
   dnscl_buff_ptr 		pDNS_query_buff =  ARDNSCL_NULL;
   dnscl_uint32_t          msg_len = ARDNSCL_INIT_VAL;

   /* Request for sending DNS query */
   ARDNSCL_PRINT(pGlobalData,("Request for DNS Query\n"));

   /*Adjust TTL for request Cache Counter and remove expired entries */
   arDnsClient_adjustTTL(pGlobalData,pRequest->cache_counter,&error);

   ARDNSCL_MEMGET(pResp, ARDNSCL_SIZEOF(arDnsClient_response_st)) 
       
   /* Check for memory Alloc failure */ 
   if(ARDNSCL_NULL == pResp)
   {
        ARDNSCL_PRINT(pGlobalData,("No memory for DNS query\n"));
        *pError = ARDNSCL_ERROR_MEMORY_ALLOC_FAILURE;
	ARDNSCL_MEMFREE(pRequest->pDnsQuery);
        pRequest->pDnsQuery = ARDNSCL_NULL;
        return ARDNSCL_FAILURE;
   } 

   /* Search for Cache Entry with matching Domain Name and query type of Request */
   if(ARDNSCL_SUCCESS == arDnsClient_search_entry(
					pGlobalData,
                    			&pRequest->pDnsQuery->query_string,
					pRequest->pDnsQuery->query_type,
					&num_records,
					&p_answer_records,
					&error))
    {
        /* Construct query response message */	
        pResp->api_id = ARDNSCL_API_QUERY_RESP;
        pResp->result = ARDNSCL_SUCCESS;
        /* Allocate memory for response */
        ARDNSCL_MEMGET(pResp->pDnsQueryResp, 
                ARDNSCL_SIZEOF(arDnsClient_query_resp_st))

        /* Check for memory Alloc failure */ 
        if(ARDNSCL_NULL == pResp->pDnsQueryResp)
        {
		ARDNSCL_PRINT(pGlobalData,("No memory for DNS query\n"));
		*pError = ARDNSCL_ERROR_MEMORY_ALLOC_FAILURE;
		arDnsClient_free_response(pGlobalData , pResp , &error);
		ARDNSCL_MEMFREE(pRequest->pDnsQuery);
		pRequest->pDnsQuery = ARDNSCL_NULL;
		return ARDNSCL_FAILURE;
        }

        pResp->pQuery = pRequest->pDnsQuery;
        pResp->pDnsQueryResp->numRecords = num_records;
        pResp->pDnsQueryResp->pAnswerRecords = p_answer_records;
        pRequest->pDnsQuery = ARDNSCL_NULL;

        if(ARDNSCL_FAILURE == arDnsClient_port_send_msg_to_app(
                                     pGlobalData,
                                     pResp,
                                     &error))
        {
            ARDNSCL_PRINT(pGlobalData,("Error in sending response\n"));
            *pError = error;
            ret_val = ARDNSCL_FAILURE;
        }

        pResp->pDnsQueryResp->pAnswerRecords = ARDNSCL_NULL;
    }
    else
    {
#if defined(ARDNSCL_PORT_LINUX) || defined(ARDNSCL_PORT_VXWORKS)
         if(ARDNSCL_NULL == pDNS_query_buff)
         {
             	ARDNSCL_MEMGET(pDNS_query_buff, ARDNSCL_MAX_BUFFER_LEN);
		/*If Failure to get memory for DNS query buffer */
		if(ARDNSCL_NULL == pDNS_query_buff)
		{
			*pError = ARDNSCL_ERROR_MEMORY_ALLOC_FAILURE;
			arDnsClient_free_response(pGlobalData , pResp , &error);
			ARDNSCL_MEMFREE(pRequest->pDnsQuery);
			pRequest->pDnsQuery = ARDNSCL_NULL;
			return 	ARDNSCL_FAILURE;
		}
			
         }
#else
	/* Windows logic to be developed */	
#endif 
       /* Initialize Answer Record pointer */
       pResp->pQuery = pRequest->pDnsQuery;	
       pRequest->pDnsQuery = ARDNSCL_NULL;

         /* Send DNS Query to DNS Server and receive response */
         ret_val = arDnsClient_port_send_dns_query(pGlobalData,
                 			pResp->pQuery,
                 			&pDNS_query_buff,
                 			&msg_len,
                 			&error);

         if(ARDNSCL_FAILURE != ret_val)    
         {
             	/* Allocate memory for response */
             	ARDNSCL_MEMGET(pResp->pDnsQueryResp, 
                	ARDNSCL_SIZEOF(arDnsClient_query_resp_st));

             	/* Check for memory Alloc failure */
             	if(ARDNSCL_NULL == pResp->pDnsQueryResp)
             	{
                 	ARDNSCL_PRINT(pGlobalData,("No memory for DNS query\n"));
                 	*pError = ARDNSCL_ERROR_MEMORY_ALLOC_FAILURE;
                        arDnsClient_free_response(pGlobalData , pResp , &error);
                 	return ARDNSCL_FAILURE;
             	}
             
               pResp->pDnsQueryResp->pAnswerRecords = ARDNSCL_NULL;

             	switch(pResp->pQuery->query_type)
             	{
                 	case ARDNSCL_QUERY_TYPE_A	:
                     		/* Parse A type response of DNS query */
                     		ret_val = arDnsClient_parse_A_dns_response(pGlobalData,
                       				pDNS_query_buff,
                       				msg_len,
                       				&pResp->pDnsQueryResp->pAnswerRecords,
                       				&pResp->pDnsQueryResp->numRecords,
                       				&error);
                     		break;
                 	case ARDNSCL_QUERY_TYPE_AAAA	:
                     		/* Parse AAAA type response of DNS query */
                     		ret_val = arDnsClient_parse_AAAA_dns_response(pGlobalData,
                       				pDNS_query_buff,
                       				msg_len,
                       				&pResp->pDnsQueryResp->pAnswerRecords,
                       				&pResp->pDnsQueryResp->numRecords,
                             			&error);
                     		break;
                 	case ARDNSCL_QUERY_TYPE_SRV	:
                     		/* Parse SRV type response of DNS query */
                     		ret_val = arDnsClient_parse_SRV_dns_response(pGlobalData,
                             			pDNS_query_buff,
                             			msg_len,
                             			&pResp->pDnsQueryResp->pAnswerRecords,
                             			&pResp->pDnsQueryResp->numRecords,
                             			&error);
                     		break;
                 	case ARDNSCL_QUERY_TYPE_NAPTR	:
                     		/* Parse NAPTR type response of DNS query */
                     		ret_val = arDnsClient_parse_NAPTR_dns_response(pGlobalData,
                             			pDNS_query_buff,
                             			msg_len,
                             			&pResp->pDnsQueryResp->pAnswerRecords,
                             			&pResp->pDnsQueryResp->numRecords,
                             			&error);
                     		break;
             	};
         }

#if defined(ARDNSCL_PORT_LINUX) || defined(ARDNSCL_PORT_VXWORKS)
         ARDNSCL_MEMFREE(pDNS_query_buff);
#else
	 if(ARDNSCL_NULL != pDNS_query_buff)
	 { 
		ARDNSCL_FREE_RECORD_LIST(pDNS_query_buff , DnsFreeRecordList);
	 }
#endif
         pResp->api_id = ARDNSCL_API_QUERY_RESP;

         /* Send Failure response if failure to Parse Query response */
         if(ret_val == ARDNSCL_FAILURE)
         {
             	pResp->result = ARDNSCL_FAILURE;
             	pResp->ecode = error;
                ret_val = ARDNSCL_SUCCESS;
         }
         else
         {
             	pResp->result = ARDNSCL_SUCCESS;
         }

      	 if(ARDNSCL_FAILURE == arDnsClient_port_send_msg_to_app(
                                     pGlobalData,
                                     pResp,
                                     &error))
        {
             	ARDNSCL_PRINT(pGlobalData,("Error in sending response\n"));
             	*pError = error;
             	ret_val = ARDNSCL_FAILURE;
        }

        if(ARDNSCL_SUCCESS == pResp->result)
        {
               if(ARDNSCL_NULL != pResp->pDnsQueryResp->pAnswerRecords)
               {
            	    /* Store the query response in cachace Database */ 
                 	if(ARDNSCL_FAILURE == arDnsClient_store_entry(
                       			&pResp->pQuery->query_string,
                       			pResp->pQuery->query_type,
                       			pResp->pDnsQueryResp->pAnswerRecords,
                       			pResp->pDnsQueryResp->numRecords,
                       			pGlobalData,&error))
                 	{
                     		ARDNSCL_PRINT(pGlobalData,("Failed to store Entry in Cache\n"));
                 	}
                 	else
                 	{
                     		pResp->pDnsQueryResp->pAnswerRecords = ARDNSCL_NULL; 
                 	}
             	}
         }
    }
       
    /* Deallocate Response structure */
    arDnsClient_free_response(pGlobalData , pResp , &error);

    return ret_val;
}

