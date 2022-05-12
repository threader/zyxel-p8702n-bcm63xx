/******************************************************************************
*                                    
* FILE NAME    : arDnsClient_port_query.c                
*                                    
* DESCRIPTION  : Contains the arDnsClient porting query handling routines  
*                                    
* Revision History :                            
*
*  DATE            NAME                 REFERENCE       REASON        
* ------         ------------           ---------     ----------
* 05-Feb-2009    Anuradha Gupta         None          Initial
* 11-Feb-2009    Abbas Haque            None          Added code for dns query
* 13-Apr-2009    Alok Tiwari            Rel 8.2       Merging of Bug 6709
* 11-Jun-2009    Ashutosh Mohan         SPR 19737     Changes done for GCC 4.1 warnings removal
* 20-Aug-2009    Anuradha Gupta         SPR 19927     GCC warning fixing
* Copyright 2009, Aricent.                       
*                                    
*******************************************************************************/
#include "arDnsClient_api.h"
#include "arDnsClient_port_prototypes.h"

/****************************************************************************
 * API Name: arDnsClient_send_dns_query
 * 
 * Description: This API is invoked to send DNS query.
 * INPUT       :pDnscl_glb_data is a pointer for global data. 
 *             :pDns_Query is a pointer to DNS query data structure.
 * OUTPUT      :pOutBuff is an output buffer.
 *             :pError is the error code.    
 *              
 * Return : It will return DNSCL_SUCCESS if successful else return DNSCL_FAILURE.
 ******************************************************************************/

dnscl_return_t   arDnsClient_port_send_dns_query (
		dnscl_void_t              *pDns_glb_data,
		arDnsClient_query_st      *pDns_Query,
		dnscl_buff_ptr             *ppOutBuff,
		dnscl_uint32_t            *pResp_Length, 
		dnscl_error_t             *pError)
{
    	/* Received response length */
    	dnscl_int32_t     resLength       = ARDNSCL_INIT_VAL;

	/*Pointer to global data structure*/
	arDnsClient_global_data_st   *pDnsCl_glb_data = ARDNSCL_NULL;
#if defined(ARDNSCL_PORT_WINDOWS)
	PIP4_ARRAY pSrvList = ARDNSCL_NULL;
	dnscl_buff_ptr   dns_buff = ARDNSCL_NULL;
	WCHAR str_buffer[ARDNSCL_MAX_STR_LEN];
#else
    /* Send Query Buffer */
	dnscl_int8_t sendBuff[ARDNSCL_MAX_BUFFER_LEN];

	/* Recv Query Response buffer */
	dnscl_int8_t recvBuff[ARDNSCL_MAX_BUFFER_LEN];
#endif

	/*Check for  error pointer*/
	if(ARDNSCL_NULL == pError)
	{
		return ARDNSCL_FAILURE;
	}

	/*Check for  global data pointer*/
	if(ARDNSCL_NULL == pDns_glb_data)
	{
		/*global data structure NULL retunn FAILURE */
		*pError =  ARDNSCL_ERROR_INVALID_GLB_DATA;
		return ARDNSCL_FAILURE;
	}
	
	/*Type cast global data*/       
	pDnsCl_glb_data =(arDnsClient_global_data_st*)pDns_glb_data;

	ARDNSCL_PRINT (pDnsCl_glb_data, ("Entered Function arDnsClient_send_dns_query\n"))

	ARDNSCL_PRINT (pDnsCl_glb_data,
		("Query data -> Type : %d , String : %s, App Ctx : %x \n",pDns_Query->query_type,
	             pDns_Query->query_string.str_val, pDns_Query->pApp_query_ctx))

#ifndef ARDNSCL_PORT_WINDOWS	
	if(pDnsCl_glb_data->dns_client_data.pConfig_data == ARDNSCL_NULL)	
	{
#if defined(ARDNSCL_PORT_LINUX)
		/*res_query function query for domain name with query type as NAPTR/A/SRV/AAA*/
		resLength = res_query((const dnscl_int8_t*)pDns_Query->query_string.str_val,C_IN,
			pDns_Query->query_type,*ppOutBuff,ARDNSCL_MAX_BUFFER_LEN);                                    
		/*if error is return from res_query the failure has been 
	  	return from query function call*/
		if(resLength < ARDNSCL_INIT_VAL)
		{
			switch(h_errno) 
			{
				case HOST_NOT_FOUND:
                 	ARDNSCL_PRINT (pDnsCl_glb_data, ("Domain name not found \n"))
                 	*pError = ARDNSCL_ERROR_INVALID_DOMAIN_NAME; 
					break;
           		case TRY_AGAIN:
                	ARDNSCL_PRINT (pDnsCl_glb_data, ("Dns Server Error \n"))
                	*pError = ARDNSCL_ERROR_SERVER_NOT_REACHABLE; 
					break;
				case NO_DATA:
                	ARDNSCL_PRINT (pDnsCl_glb_data, 
                        ("Domain name exists but there is no data of the requested type \n"))
                	*pError = ARDNSCL_ERROR_DATA_DOES_NOT_EXIST;        
					break;
           		case NO_RECOVERY:
                	ARDNSCL_PRINT (pDnsCl_glb_data, ("An unrecoverable error has occurred \n"))
                	*pError = ARDNSCL_ERROR_NON_RECOVERABLE_SERVER_ERROR;        
					break;
				default:
		       		ARDNSCL_PRINT (pDnsCl_glb_data, ("Unknown error has occurred \n"))
               		*pError = ARDNSCL_ERROR_SERVER_ERROR;
		       		break;
			}
			ARDNSCL_PRINT (pDnsCl_glb_data, ("Res Query Failure\n")) 

			/*Return failure corresponding to error number */
			return ARDNSCL_FAILURE;
		}
#endif
#if defined(ARDNSCL_PORT_VXWORKS)
		/*resolvQuery function query for domain name with 
	  	query type as A/AAAA/SRV/NAPTR*/
		resLength = resolvQuery(pDns_Query->query_string.str_val,C_IN,
		pDns_Query->query_type,*ppOutBuff, ARDNSCL_MAX_BUFFER_LEN); 

		/*if resLength < 0 the failure has been return from query function call*/
		if(resLength < ARDNSCL_INIT_VAL)
		{
            	switch (errno)
	      		{
				case S_resolvLib_TRY_AGAIN:
                    			ARDNSCL_PRINT (pDnsCl_glb_data, ("Dns Server Error \n"))
                    			*pError = ARDNSCL_ERROR_SERVER_NOT_REACHABLE; 
		     			break; 
	        		case S_resolvLib_HOST_NOT_FOUND:
                    			ARDNSCL_PRINT (pDnsCl_glb_data, ("Domain name not found \n"))
                    			*pError = ARDNSCL_ERROR_INVALID_DOMAIN_NAME; 
		     			break;
	        		case S_resolvLib_NO_DATA:
                    			ARDNSCL_PRINT (pDnsCl_glb_data, 
                            		("Domain name exists but there is no data of the requested type \n"))
                    			*pError = ARDNSCL_ERROR_DATA_DOES_NOT_EXIST;        
		     			break;
	         		case S_resolvLib_NO_RECOVERY:
                    			ARDNSCL_PRINT (pDnsCl_glb_data, ("An unrecoverable error has occurred \n"))
                    			*pError = ARDNSCL_ERROR_NON_RECOVERABLE_SERVER_ERROR;        
		        		break;
		     		default:
                    			ARDNSCL_PRINT (pDnsCl_glb_data, ("Unknown error has occurred \n"))
               				*pError = ARDNSCL_ERROR_SERVER_ERROR;
		      			break; 
	     		}
           	ARDNSCL_PRINT (pDnsCl_glb_data, ("Resolv Query Failure\n")) 
			return ARDNSCL_FAILURE;
		}
#endif
		/*Assigning response length*/
		*pResp_Length = resLength;	
	}
	else
	{
		/* Socket fd of UDP socket for sending queriries */
		dnscl_int32_t socket_id = pDnsCl_glb_data->dns_client_data.dns_query_fd;
		
		/* Amount of time elapsed before receiving query response */ 
		dnscl_int32_t time_elapsed = ARDNSCL_INIT_VAL;

		/* Number of Query Retries */
		dnscl_int8_t retryCount = ARDNSCL_INIT_VAL;

		/* Variable maintaining Send query Id */ 
		dnscl_int16_t sendQueryId = ARDNSCL_INIT_VAL;

		/* Variable maintaining recv query Id */
		dnscl_int16_t recvQueryId = ARDNSCL_INIT_VAL;
		
		dnscl_int16_t msg_length = ARDNSCL_MAX_BUFFER_LEN;

		/* Variable for timeout in select and for storing timeOut value for each loop */
		struct timeval dnsQueryTimeout, loopTimeVal;

		/* fd_set for recv in select */
		fd_set recvFdSet;

        dnsQueryTimeout.tv_sec = (ARDNSCL_DEFAULT_QUERY_TIME * 1000)/ 1000000;
        dnsQueryTimeout.tv_usec = 0;

#if defined(ARDNSCL_PORT_LINUX)
		/* Make query buffer */
		msg_length = res_mkquery(QUERY,(const dnscl_int8_t*)pDns_Query->query_string.str_val, 
                               C_IN,pDns_Query->query_type,
				               ARDNSCL_NULL,0,ARDNSCL_NULL,
                               (dnscl_uint8_t *)sendBuff,ARDNSCL_MAX_BUFFER_LEN);
    
		/* Check for invalid query buffer */ 
		if(ARDNSCL_INIT_VAL > msg_length)
		{
			ARDNSCL_PRINT (pDnsCl_glb_data, ("Unable to Construct DNS packet\n"))
			*pError = ARDNSCL_ERROR_DNS_INVALID_QUERY_DATA;
			return ARDNSCL_FAILURE;	
		}
	
#elif defined(ARDNSCL_PORT_VXWORKS)
		/* Make query buffer */
		msg_length = resolvMkQuery(QUERY,pDns_Query->query_string.str_val,C_IN, pDns_Query->query_type,
				ARDNSCL_NULL,0,ARDNSCL_NULL,sendBuff,ARDNSCL_MAX_BUFFER_LEN);
		
		/* Check for invalid query buffer */
		if(ERROR == msg_length)
		{
			ARDNSCL_PRINT (pDnsCl_glb_data, ("Unable to Construct DNS packet\n"))
			*pError = ARDNSCL_ERROR_DNS_INVALID_QUERY_DATA;
			return ARDNSCL_FAILURE;	
		}
#endif
		/* seore query Id of query buffer */ 
		ARDNSCL_MEMCPY(&sendQueryId,sendBuff,ARDNSCL_SIZEOF(dnscl_uint16_t));
		sendQueryId = ARDNSCL_NTOHS(sendQueryId);
		
		while(ARDNSCL_MAX_DNS_RETRY > retryCount)
		{
			loopTimeVal = dnsQueryTimeout;
            /* Now it send the failure after ARDNSCL_DNS_QUERY_TIMEOUT seconds */
            /* Calculate the elapsed time */
            time_elapsed = time_elapsed + ((dnsQueryTimeout.tv_sec * 1000000) +   
                                      dnsQueryTimeout.tv_usec);
 
            if((ARDNSCL_DNS_QUERY_TIMEOUT * 1000000) < time_elapsed) 
            {
                 ARDNSCL_PRINT (pDnsCl_glb_data, ("Dns Server Error \n"))
                 ARDNSCL_PRINT (pDnsCl_glb_data, ("Query timeout \n"))
                 *pError = ARDNSCL_ERROR_SERVER_NOT_REACHABLE;
                 return ARDNSCL_FAILURE;
            }


			if(ARDNSCL_FAILURE == arDnsClient_port_send_message(pDnsCl_glb_data,
									socket_id,
									(dnscl_uint8_t*)sendBuff,
									msg_length,
									&(pDnsCl_glb_data->dns_client_data.pConfig_data->dns_server_addr),
									pError)) 
			{
				ARDNSCL_PRINT (pDnsCl_glb_data, ("Unable to Send to DNS Server\n"))
				*pError = ARDNSCL_ERROR_MESSAGE_SEND;
				return ARDNSCL_FAILURE;
			}
			
			/*Add recv Fd set for seclect */
			FD_ZERO(&recvFdSet);
   	 		FD_SET(socket_id, &recvFdSet);
			
			/* Select wait for receving response from DNS Server */
			if(ARDNSCL_INIT_VAL > select((socket_id+1),&recvFdSet,ARDNSCL_NULL,ARDNSCL_NULL,&loopTimeVal))
			{
				ARDNSCL_PRINT (pDnsCl_glb_data, ("Unable to recv Mesg from DNS Server\n"))
				*pError = ARDNSCL_ERROR_SERVER_ERROR;
				return ARDNSCL_FAILURE;
			}

			if(FD_ISSET(socket_id,&recvFdSet))
			{
				/* recev message from DNS server */
				if(ARDNSCL_FAILURE ==  
							arDnsClient_port_recv_message(pDns_glb_data,
							socket_id,
							(dnscl_uint8_t*)recvBuff,
							(dnscl_uint32_t*)&resLength,
							pError))
				
				{
					ARDNSCL_PRINT (pDnsCl_glb_data, ("Unable to Recv Mesg from DNS Server\n"))
					*pError = ARDNSCL_ERROR_MESSAGE_RECV;
					return ARDNSCL_FAILURE;
				}

				/* Clooect RecvQueryId for matching with sendQueryId */        							
				ARDNSCL_MEMCPY(&recvQueryId,recvBuff,ARDNSCL_SIZEOF(dnscl_uint16_t));
				recvQueryId = ARDNSCL_NTOHS(recvQueryId);
				
				/*check that the received packet is response to sent packet */
				if(sendQueryId == recvQueryId)
				{
				    ARDNSCL_PRINT (pDnsCl_glb_data, ("Query matched \n"))
					break;
				}
				else
				{
					ARDNSCL_PRINT (pDnsCl_glb_data, ("RecvId mismatch with sendId\n"))
					ARDNSCL_PRINT (pDnsCl_glb_data, ("Discarding Packet\n"))
				}
			}

			/* Now after doing check it will double the wait time for next loop */ 
			dnsQueryTimeout.tv_sec = (((dnsQueryTimeout.tv_sec * 1000000 + 
										dnsQueryTimeout.tv_usec) * 2) / 1000000);
			dnsQueryTimeout.tv_usec = (((dnsQueryTimeout.tv_sec * 1000000 + 
										dnsQueryTimeout.tv_usec) * 2) % 1000000);
		}
		
		/* Check for maximim number of retries */
		if(ARDNSCL_MAX_DNS_RETRY == retryCount)
		{
			ARDNSCL_PRINT (pDnsCl_glb_data, ("Dns Server Error \n"))
			ARDNSCL_PRINT (pDnsCl_glb_data, ("Max Number of retries reached \n"))
			*pError = ARDNSCL_ERROR_SERVER_NOT_REACHABLE;
			return ARDNSCL_FAILURE;
		}

		ARDNSCL_MEMCPY(*ppOutBuff,recvBuff,resLength);
		/*Assigning response length*/
		*pResp_Length = resLength;
	}
#endif
#if defined(ARDNSCL_PORT_WINDOWS)

	if(ARDNSCL_NULL != pDnsCl_glb_data->dns_client_data.pConfig_data)
	{
		pSrvList = (PIP4_ARRAY) LocalAlloc(LPTR,sizeof(IP4_ARRAY));
		pSrvList->AddrCount = 1;
		pSrvList->AddrArray[0] = inet_addr(pDnsCl_glb_data->dns_client_data.
			pConfig_data->dns_server_addr.ipAddress.str_val);
	}

	pDns_Query->query_string.str_val[pDns_Query->query_string.str_len] = '\0';

	/*res_query function query for domain name with query  
	  type as NAPTR/A/AAAA/SRV
	   */
	

	/* convert the Char string to Wide char string */
	MultiByteToWideChar(
		CP_ACP,
		MB_COMPOSITE,
		pDns_Query->query_string.str_val,
		-1,
		str_buffer,
		ARDNSCL_MAX_STR_LEN);

	resLength = DnsQuery(
		/*pDns_Query->query_string.str_val,*/
		str_buffer,
		pDns_Query->query_type,
		DNS_QUERY_STANDARD,
		pSrvList, 
		&dns_buff,
		ARDNSCL_NULL);

	if(ARDNSCL_NULL != pSrvList)
	{
		LocalFree(pSrvList);
	}
  
	
	/*if resLength < 0 the failure has been return from query function call*/
    if(resLength )
	{
		ARDNSCL_PRINT (pDnsCl_glb_data, ("DnsQuery Failure \n"))

        switch(resLength)
	  {
		case DNS_ERROR_RCODE_FORMAT_ERROR:
             ARDNSCL_PRINT (pDnsCl_glb_data, ("Format error \n"))
             *pError = ARDNSCL_ERROR_INVALID_DOMAIN_NAME; 
		     break;

		case DNS_ERROR_RCODE_SERVER_FAILURE:
             ARDNSCL_PRINT (pDnsCl_glb_data, ("Server failure \n"))
             *pError = ARDNSCL_ERROR_SERVER_ERROR; 
		     break;

		case DNS_ERROR_RCODE_NAME_ERROR:
			 ARDNSCL_PRINT (pDnsCl_glb_data, ("Server failure \n"))
             *pError = ARDNSCL_ERROR_INVALID_DOMAIN_NAME; 
		     break;

		case DNS_ERROR_RCODE_REFUSED:
             ARDNSCL_PRINT (pDnsCl_glb_data, ("Refused \n"))
             *pError = ARDNSCL_ERROR_SERVER_ERROR; 
		     break;

		case DNS_ERROR_RCODE_YXDOMAIN:
             ARDNSCL_PRINT (pDnsCl_glb_data, ("Domain name should not exist \n"))
             *pError = ARDNSCL_ERROR_INVALID_DOMAIN_NAME; 
		     break;
		
        case DNS_ERROR_RCODE_NOT_IMPLEMENTED:
		case DNS_ERROR_RCODE_NOTZONE:
	    case DNS_ERROR_RCODE_NOTAUTH:
		case DNS_ERROR_RCODE_NXRRSET:
 		case DNS_ERROR_RCODE_YXRRSET:     
		case DNS_ERROR_RCODE_BADSIG :
        case DNS_ERROR_RCODE_BADTIME:
		case DNS_ERROR_RCODE_BADKEY:
		     ARDNSCL_PRINT (pDnsCl_glb_data, ("Bad key  \n"))
             *pError = ARDNSCL_ERROR_SERVER_ERROR; 
		     break;
		
        case DNS_ERROR_TRY_AGAIN_LATER:     
 		     ARDNSCL_PRINT (pDnsCl_glb_data, ("server not reachable\n"))
             *pError = ARDNSCL_ERROR_SERVER_NOT_REACHABLE; 
		     break;

		case DNS_ERROR_NO_PACKET:
		case DNS_INFO_NO_RECORDS:
			 ARDNSCL_PRINT (pDnsCl_glb_data, ("server not reachable\n"))
             *pError = ARDNSCL_ERROR_DATA_DOES_NOT_EXIST; 
		     break;

        default:
              ARDNSCL_PRINT (pDnsCl_glb_data, ("Unknown error has occurred \n"))
              *pError = ARDNSCL_ERROR_SERVER_ERROR;
		      break; 
	     }
        ARDNSCL_PRINT (pDnsCl_glb_data, ("Resolv Query Failure\n"))
		return ARDNSCL_FAILURE;
	}

	 *ppOutBuff = dns_buff;

	/*Assigning response length*/
	*pResp_Length = resLength;  
#endif
	ARDNSCL_PRINT (pDnsCl_glb_data, ("Exiting Function arDnsClient_send_dns_query\n"))

	
	/*return Success*/
	return ARDNSCL_SUCCESS;
}


 /*****************************************************************************
 * FUNCTION :   arDnsClient_port_set_dns_server
 *
 * Description: This function is used to configure DNS server dynamically.
 *                            
 * Return:      It will return ARDNSCL_SUCCESS if successful 
 *              else return ARDNSCL_FAILURE.
 ********************************************************************************/
dnscl_return_t
arDnsClient_port_set_dns_server(
		dnscl_void_t        	*pDns_glb_data,
		arDnsClient_address_st  *pServer_addr,
		dnscl_error_t           *pError)
{
#ifndef ARDNSCL_PORT_WINDOWS
	/* Local bind Address fro DNS query */
	arDnsClient_address_st dnsBindAddress;
#endif

	/* Global Data Structure pointer */ 
	arDnsClient_global_data_st   *pDnsCl_glb_data = 
			(arDnsClient_global_data_st*)pDns_glb_data;

	ARDNSCL_PRINT (pDnsCl_glb_data, ("Entered Function arDnsClient_port_set_dns_server\n"))

	/* Do nothing with server address */
	pServer_addr=pServer_addr;

#ifndef ARDNSCL_PORT_WINDOWS
	ARDNSCL_STRCPY((dnscl_int8_t*)dnsBindAddress.ipAddress.str_val,
            (const dnscl_int8_t*)pDnsCl_glb_data->dns_client_addr.ipAddress.str_val);
	dnsBindAddress.ipAddress.str_len =
        pDnsCl_glb_data->dns_client_addr.ipAddress.str_len;
	dnsBindAddress.port = ARDNSCL_DNS_QUERY_BIND_PORT;

    if(0 > pDnsCl_glb_data->dns_client_data.dns_query_fd)
    {
        /* Check for socket creation failure */
        if(ARDNSCL_FAILURE == arDnsClient_port_create_ipc(pDnsCl_glb_data,
                    dnsBindAddress,
                    &pDnsCl_glb_data->dns_client_data.dns_query_fd,
                    pError))
        {
            ARDNSCL_PRINT (pDnsCl_glb_data, ("Unable to create socket for DNS request\n"))
            *pError = ARDNSCL_ERROR_SOCKET_CREATE;
            return ARDNSCL_FAILURE;	
        }
    }
#endif
	ARDNSCL_PRINT (pDnsCl_glb_data, ("Exiting Function arDnsClient_port_set_dns_server\n"))

	return ARDNSCL_SUCCESS;
}
