/******************************************************************************
*                                    
* FILE NAME    : arDnsClient_port_port_pack.c                
*                                    
* DESCRIPTION  : Contains the arDnsClient CLIB packing routines  
*                                    
* Revision History :                            
*
*  DATE            NAME                 REFERENCE       REASON        
* ------         ------------           ---------     ----------
* 05-Feb-2009    Anuradha Gupta         None          Initial
* 18-Feb-2009    Anuradha Gupta         LLD           Code Start
* 05-May-2009    Anuradha Gupta         Bug 7430      Modfied fn 
*                                                       arDnsClient_port_pack_response
* Copyright 2009, Aricent.                       
*                                    
*******************************************************************************/

#include "arDnsClient_port_prototypes.h"

/****************************************************************************
 * Function Name: arDnsClient_port_pack_U8 
 * 
 * Description: This Function is invoked to convert U8 data type to stream buffer. 
 * 
 ******************************************************************************/
dnscl_return_t
arDnsClient_port_pack_U8(
    dnscl_uint8_t    *pDest,
    dnscl_uint8_t    *p_src,
    dnscl_uint32_t   *p_length)

{
    /* Packing U8 */
    *pDest = *p_src;

    /*Assiging value of length*/
    *p_length    = ARDNSCL_SIZEOF(dnscl_uint8_t);
       
    return ARDNSCL_SUCCESS;
}


/****************************************************************************
 * Function Name: arDnsClient_port_pack_U16 
 * 
 * Description: This Function is invoked to convert U16 data type to stream buffer. 
 * 
 ******************************************************************************/
dnscl_return_t
arDnsClient_port_pack_U16(
    dnscl_uint8_t    *pDest,
    dnscl_uint8_t    *p_src,
    dnscl_uint32_t   *p_length)
{
    /* Packing U16 in Byte order*/
    pDest[0]   = (dnscl_uint8_t)((*(dnscl_uint16_t*)p_src) >> 8);
    pDest[1]   = (dnscl_uint8_t)((*(dnscl_uint16_t*)p_src));

     /*Assiging value of length*/
    *p_length    = ARDNSCL_SIZEOF(dnscl_uint16_t);
     
    return ARDNSCL_SUCCESS;
}


/****************************************************************************
 * Function Name: arDnsClient_port_pack_U32 
 * 
 * Description: This Function is invoked to convert U32 data type to stream buffer. 
 * 
 ******************************************************************************/
dnscl_return_t
arDnsClient_port_pack_U32(
    dnscl_uint8_t    *pDest,
    dnscl_uint8_t    *p_src,
    dnscl_uint32_t   *p_length)
{
    /* Pack dnscl_uint32_t in byte order*/
    pDest[0]  = (dnscl_uint8_t)((*(dnscl_uint32_t*)p_src) >> 24);
    pDest[1] = (dnscl_uint8_t)((*(dnscl_uint32_t*)p_src) >> 16);
    pDest[2] = (dnscl_uint8_t)((*(dnscl_uint32_t*)p_src) >> 8);
    pDest[3] = (dnscl_uint8_t)((*(dnscl_uint32_t*)p_src));

    /*Assiging value of length*/
    *p_length    = ARDNSCL_SIZEOF(dnscl_uint32_t);

    return ARDNSCL_SUCCESS;
}


/****************************************************************************
 * Function Name: arDnsClient_port_pack_string 
 * 
 * Description: This Function is invoked to convert string data type 
 *              to stream buffer. 
 * 
 ******************************************************************************/
dnscl_return_t
arDnsClient_port_pack_string(
        dnscl_uint8_t *pDest,
        arDnsClient_string_st *pStr,
        dnscl_uint32_t *p_length)
{
    /*Variable to store offset length*/    
    dnscl_uint32_t   len_val = ARDNSCL_INIT_VAL;

    /*Variable for string length*/
    dnscl_uint32_t   count = ARDNSCL_INIT_VAL;

    *p_length = 0;

    /*encode the value of string */
    arDnsClient_port_pack_U8(pDest + (*p_length), 
            (dnscl_uint8_t *)&(pStr->str_len), &len_val);

    /*Incrementing the value of length*/
    *p_length += len_val ;

     /*done to avoid klockwork warning*/
    if(ARDNSCL_MAX_STR_LEN < pStr->str_len)
    {
	    pStr->str_len = ARDNSCL_MAX_STR_LEN;
    }
    /*packing each charcter in string*/
    for(count=0; count < pStr->str_len; count ++)
    {
        /*Setting the value of len_val as 0*/                  
        len_val = ARDNSCL_INIT_VAL;

        /*encode the value of string */
        arDnsClient_port_pack_U8(pDest + (*p_length), 
                (dnscl_uint8_t *)&(pStr->str_val[count]), &len_val);

        /*Incrementing the value of length*/
        *p_length += len_val ;
    }

    return ARDNSCL_SUCCESS;
}

/****************************************************************************
 * Function Name: arDnsClient_port_pack_short_string 
 * 
 * Description: This Function is invoked to convert short string data type 
 *              to stream buffer. 
 * 
 ******************************************************************************/
dnscl_return_t
arDnsClient_port_pack_short_string(
        dnscl_uint8_t *pDest,
        arDnsClient_short_string_st *pStr,
        dnscl_uint32_t *p_length)
{
    /*Variable to store offset length*/    
    dnscl_uint32_t   len_val = ARDNSCL_INIT_VAL;

    /*Variable for string length*/
    dnscl_uint32_t   count = ARDNSCL_INIT_VAL;

    *p_length = 0;

    /*encode the value of string */
    arDnsClient_port_pack_U8(pDest + (*p_length), 
            (dnscl_uint8_t *)&(pStr->str_len), &len_val);

    /*Incrementing the value of length*/
    *p_length += len_val ;

    /*done to avoid klockwork warning*/
    if(ARDNSCL_MAX_SHORT_STR_LEN < pStr->str_len)
    {
	    pStr->str_len = ARDNSCL_MAX_SHORT_STR_LEN;
    }

    /*packing each charcter in string*/
    for(count=0; count < pStr->str_len; count ++)
    {
        /*Setting the value of len_val as 0*/                  
        len_val = ARDNSCL_INIT_VAL;

        /*encode the value of string */
        arDnsClient_port_pack_U8(pDest + (*p_length), 
                (dnscl_uint8_t *)&(pStr->str_val[count]), &len_val);

        /*Incrementing the value of length*/
        *p_length += len_val ;
    }

    return ARDNSCL_SUCCESS;
}

/****************************************************************************
 * Function Name: arDnsClient_port_pack_address 
 * 
 * Description: This Function is invoked to convert address data type 
 *              to stream buffer. 
 * 
 ******************************************************************************/
dnscl_return_t
arDnsClient_port_pack_address(
     dnscl_uint8_t    *pDest,
     arDnsClient_address_st    *p_dnscl_address,
 	 dnscl_uint32_t   *p_length)
{
    /*Variable for length value*/
    dnscl_uint32_t             len_val = 0;

    /* Initialise length to 0 */
    *p_length	= 0;

    /* This function pack IP Address */
    arDnsClient_port_pack_short_string(pDest + (*p_length), 
            &(p_dnscl_address->ipAddress), 
            &len_val);

    *p_length    += len_val;

    arDnsClient_port_pack_U16(
            pDest + (*p_length), 				
            (dnscl_uint8_t*)&(p_dnscl_address->port), 
            &len_val);


    /*Assinging value of offset to length*/
    *p_length    += len_val;

    return ARDNSCL_SUCCESS;
}

/****************************************************************************
 * Function Name: arDnsClient_port_pack_conf 
 * 
 * Description: This Function is invoked to convert conf data type 
 *              to stream buffer. 
 * 
 ******************************************************************************/
dnscl_return_t
arDnsClient_port_pack_conf(
        dnscl_uint8_t       *pDest, 
        arDnsClient_conf_st *pConfData,
        dnscl_uint32_t    *p_length)
{
    /*Variable to store length*/       
    dnscl_uint32_t     len_val = ARDNSCL_INIT_VAL;

    /* Initialise length to 0 */
    *p_length	= 0;

    /*Packing the DNS server address*/
    arDnsClient_port_pack_address(pDest + (*p_length), 
            &(pConfData->dns_server_addr), 
            &len_val);

    *p_length += len_val;

    return ARDNSCL_SUCCESS;
}

/****************************************************************************
 * Function Name: arDnsClient_port_pack_query 
 * 
 * Description: This Function is invoked to convert query type 
 *              to stream buffer. 
 * 
 ******************************************************************************/
dnscl_return_t
arDnsClient_port_pack_query(
        dnscl_uint8_t 		*pDest,
        arDnsClient_query_st *pDnsQuery,
        dnscl_uint32_t       *p_length)
{


    /*Variable to store length value*/    
    dnscl_uint32_t    len_val = ARDNSCL_INIT_VAL;

    /* Initialise length to 0 */
    *p_length	= 0;

    /*packing the query type of query data structure*/
    arDnsClient_port_pack_U8(pDest + (*p_length),
            (dnscl_uint8_t *)&(pDnsQuery->query_type), &len_val);

    /*Incrementing the value *p_length  by len_val*/
    *p_length +=len_val;

    /* reset offset length to 0 */
    len_val = ARDNSCL_INIT_VAL;

    /*Packing the domain name*/
    arDnsClient_port_pack_string(pDest + (*p_length),
            &(pDnsQuery->query_string), 
            &len_val);

    /*Incrementing the value *p_length  by len_val*/
    *p_length +=len_val;

    /* reset offset length to 0 */
    len_val = ARDNSCL_INIT_VAL;

    /*Packing the query context*/
    arDnsClient_port_pack_U32(pDest + (*p_length), 
            (dnscl_uint8_t *)&(pDnsQuery->pApp_query_ctx), 
            &len_val);

    /*Incrementing the value *p_length  by len_val*/
    *p_length +=len_val;

    /*return success*/
    return ARDNSCL_SUCCESS;
}


/****************************************************************************
 * Function Name: arDnsClient_port_pack_clear_cache 
 * 
 * Description: This Function is invoked to convert clear cache data type 
 *              to stream buffer. 
 * 
 ******************************************************************************/
dnscl_return_t
arDnsClient_port_pack_clear_cache (
	  dnscl_uint8_t        *pDest,
   	  arDnsClient_clear_cache_st *pClearCache,
      dnscl_uint32_t       *p_length)
{
    /*Variable to store length*/       
    dnscl_uint32_t    len_val = ARDNSCL_INIT_VAL;

    /* Initialise length to 0 */
    *p_length	= 0;

    /*packing the query type*/
    arDnsClient_port_pack_U8(pDest + (*p_length),
            (dnscl_uint8_t *)&(pClearCache->clear_options), &len_val);

    /*Incrementing the value *p_length  by len_val*/
    *p_length +=len_val;

    /* Initialise offset length to 0 */
    len_val = ARDNSCL_INIT_VAL;

    if(ARDNSCL_CLEAR_SINGLE_ENTRY == pClearCache->clear_options)
    {
        /*Packing the domain name*/
        arDnsClient_port_pack_string(pDest + (*p_length), 
                &(pClearCache->domain_name), 
                &len_val);

        /*Incrementing the value *p_length  by len_val*/
        *p_length +=len_val;

        /* Initialise offset length to 0 */
        len_val = ARDNSCL_INIT_VAL;


        /*packing the query type*/
        arDnsClient_port_pack_U8(pDest + (*p_length),
                (dnscl_uint8_t *)&(pClearCache->query_type), &len_val);

        /*Incrementing the value *p_length  by len_val*/
        *p_length +=len_val;
    }

    return ARDNSCL_SUCCESS;
}

/****************************************************************************
 * Function Name: arDnsClient_port_pack_rr_list
 * 
 * Description: This Function is invoked to convert Resource Record list 
 *              to stream buffer. 
 * 
 ******************************************************************************/
dnscl_return_t
arDnsClient_port_pack_rr_list(
			dnscl_uint8_t          *pDest,
			dnscl_uint8_t          query_type,
			arDnsClient_dns_rr_list_st *pList,
            		dnscl_uint32_t          *p_length)
{
    /*Variable for length*/
    dnscl_uint32_t        len_val = ARDNSCL_INIT_VAL;
    arDnsClient_dns_rr_list_st *p_temp_list = ARDNSCL_NULL;
    arDnsClient_rr_data_un   query_rr_data;

    p_temp_list = pList;

    /* Initialise length to 0 */
    *p_length	= 0;

    while (ARDNSCL_NULL != p_temp_list)
    {
        query_rr_data = p_temp_list -> query_rr_data;

        /*Packing the TTL*/
        arDnsClient_port_pack_U32(pDest + (*p_length),
                (dnscl_uint8_t *)&(p_temp_list->ttl),
                &len_val);

        /*Increment the value of p_length  by length*/
        *p_length+= len_val;

        /*Pack if query type is  NAPTR */
        if(ARDNSCL_QUERY_TYPE_NAPTR == query_type)
        {

            /*Packing order for NAPTR query*/        
            arDnsClient_port_pack_U16(pDest + (*p_length),
                    (dnscl_uint8_t *)&(query_rr_data.naptr_rr_data.order),
                    &len_val);

            /*Increment the value of  p_length  by length*/
            *p_length += len_val;

            /*Intialize value of length to zero*/
            len_val = ARDNSCL_INIT_VAL;

            /*Packing preference for NAPTR query*/
            arDnsClient_port_pack_U16(pDest + (*p_length),
                    (dnscl_uint8_t *)&(query_rr_data.naptr_rr_data.preference), 
                    &len_val);

            /*Increment the value of  p_length  by length*/
            *p_length += len_val;


            /*Intialize value of length to zero*/
            len_val = ARDNSCL_INIT_VAL;

            /*Packing flag string for NAPTR query*/
            arDnsClient_port_pack_short_string(pDest+ (*p_length),
                    &(query_rr_data.naptr_rr_data.flags), &len_val);


            /*Increment the value of  p_length  by length*/
            *p_length += len_val;


            /*Intialize value of length to zero*/
            len_val = ARDNSCL_INIT_VAL;

            /*Packing service string for NAPTR query*/
            arDnsClient_port_pack_short_string(pDest+ (*p_length),
                    &(query_rr_data.naptr_rr_data.service), 
                    &len_val);

            /*Increment the value of  p_length  by length*/
            *p_length += len_val;


            /*Intialize value of length to zero*/
            len_val = ARDNSCL_INIT_VAL;

            /*Packing regular expression  string for NAPTR query*/
            arDnsClient_port_pack_short_string(pDest+ (*p_length),
                    &(query_rr_data.naptr_rr_data.regularExpr),
                    &len_val);


            /*Increment the value of  p_length  by length*/
            *p_length += len_val;


            /*Intialize value of length to zero*/
            len_val = ARDNSCL_INIT_VAL;

            /*Packing replacement  string for NAPTR query*/
            arDnsClient_port_pack_string(pDest+ (*p_length),
                    &(query_rr_data.naptr_rr_data.replacement),
                    &len_val);


            /*Increment the value of  p_length  by length*/
            *p_length += len_val;


            /*Intialize value of length to zero*/
            len_val = ARDNSCL_INIT_VAL;
        }

        /*Packing if Query type is SRV*/
        if(ARDNSCL_QUERY_TYPE_SRV == query_type)
        {
            /*Packing name target  string for SRV query*/

            arDnsClient_port_pack_string(pDest+ (*p_length),
                    &(query_rr_data.srv_rr_data.nameTarget),
                    &len_val);


            /*Increment the value of  p_length  by length*/
            *p_length += len_val;


            /*Intialize value of length to zero*/
            len_val = ARDNSCL_INIT_VAL;

            /*Packing priority for SRV query*/
            arDnsClient_port_pack_U16(pDest+ (*p_length),
                    (dnscl_uint8_t *)&(query_rr_data.srv_rr_data.priority),
                    &len_val);

            /*Increment the value of  p_length  by length*/
            *p_length += len_val;


            /*Intialize value of length to zero*/
            len_val = ARDNSCL_INIT_VAL;

            /*Packing weight for SRV query*/
            arDnsClient_port_pack_U16(pDest+ (*p_length),
                    (dnscl_uint8_t *)&(query_rr_data.srv_rr_data.weight),
                    &len_val);

            /*Increment the value of  p_length  by length*/
            *p_length += len_val;

            /*Intialize value of length to zero*/
            len_val = ARDNSCL_INIT_VAL;

            /*Packing port for SRV query*/
            arDnsClient_port_pack_U16(pDest+ (*p_length),
                    (dnscl_uint8_t *)&(query_rr_data.srv_rr_data.port),
                    &len_val);

            /*Increment the value of  p_length  by length*/
            *p_length += len_val;


            /*Intialize value of length to zero*/
            len_val = ARDNSCL_INIT_VAL;
        }

        /*Packing if Query type is SRV*/
        if(ARDNSCL_QUERY_TYPE_A == query_type)
        {
            /*Packing IPV4 address for A query*/
            arDnsClient_port_pack_short_string(pDest+ (*p_length),
                    &(query_rr_data.A_rr_data.addrIP4),
                    &len_val);

            /*Increment the value of  p_length  by length*/
            *p_length += len_val;

            /*Intialize value of length to zero*/
            len_val = ARDNSCL_INIT_VAL;
        }

        /*Packing if Query type is AAAA*/
        if(ARDNSCL_QUERY_TYPE_AAAA == query_type)
        {
            /*Packing IPV4 address for A query*/
            arDnsClient_port_pack_short_string(pDest+ (*p_length),
                    &(query_rr_data.AAAA_rr_data.addrIP6),
                    &len_val);

            
            /*Increment the value of  p_length  by length*/
            *p_length += len_val;

            /*Intialize value of length to zero*/
            len_val = ARDNSCL_INIT_VAL;
        }

        p_temp_list = p_temp_list ->pNext;
    }

    return ARDNSCL_SUCCESS;
}

/****************************************************************************
 * Function Name: arDnsClient_port_pack_query_resp
 * 
 * Description: This Function is invoked to convert query resp data type 
 *              to stream buffer. 
 * 
 ******************************************************************************/
dnscl_return_t
arDnsClient_port_pack_query_resp(
	dnscl_uint8_t          *pDest,
    dnscl_query_t           query_type,
	arDnsClient_query_resp_st *pQuery_resp,
    dnscl_uint32_t          *p_length)
{
    /*Variable for length*/
    dnscl_uint32_t        len_val = ARDNSCL_INIT_VAL;

    /* initialise the length to 0 */
    *p_length = 0;

    /*Packing number of records */
    arDnsClient_port_pack_U8(pDest+ (*p_length),
            (dnscl_uint8_t *)&(pQuery_resp->numRecords),
            &len_val);

    /*Increment the value of  p_length  by length*/
    *p_length += len_val;

    /*Intialize value of length to zero*/
    len_val = ARDNSCL_INIT_VAL;

    /*pack the resource record list*/           
    arDnsClient_port_pack_rr_list( 
            pDest+ (*p_length),
            query_type,
            pQuery_resp->pAnswerRecords,
            &len_val);
    /*Increment the value of pLength  by length*/
    *p_length+= len_val;

    return ARDNSCL_SUCCESS;
}

/****************************************************************************
 * API Name: arDnsClient_port_pack_response
 * 
 * Description: This API is invoked by the application for packing the response 
 *              ie converting the response structure to stream buffer. 
 * INPUT       :pDnscl_glb_data - The pointer to the global data of arDnsClient.
 *             :pResponse - The pointer to response structure.  
 * OUTPUT      :ppDest - The pointer to the stream buffer
 *             :pLength - Pointer to the lenght of the stream buffer.
 *             :pError - Error pointer to be populated in case of error. 
 *
 * return : It will return ADNSCL_SUCCESS if successful else return ARDNSCL_FAILURE.
 ******************************************************************************/
dnscl_return_t  arDnsClient_port_pack_response (
        dnscl_void_t             *pDnsCl_glb_data,
        arDnsClient_response_st  *pResponse,
        dnscl_uint8_t            **ppDest,
        dnscl_uint16_t           *pLength,
        dnscl_error_t            *pError)

{
	/*Variable to store buffer location*/      
	dnscl_uint8_t   *p_temp_buffer = ARDNSCL_NULL;

   /*Variable to store buffer location*/      
	dnscl_uint8_t   *p_store_buffer = ARDNSCL_NULL ;

	/*Variable to store length*/    
	dnscl_uint32_t     len_val = ARDNSCL_INIT_VAL;

    /* changed the data type as uint32 as it is being packed as uint32
     */ 
    dnscl_uint32_t    payload_length = ARDNSCL_INIT_VAL;

    /*Pointer to global data structure*/
    arDnsClient_global_data_st         *pGlob_data = ARDNSCL_NULL;

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

	ARDNSCL_CLIB_PRINT (pGlob_data, ("Entered Function arDnsClient_port_pack_response\n"))

	/*Check for  length pointer*/
	if(ARDNSCL_NULL == pLength)
	{
		ARDNSCL_CLIB_PRINT (pGlob_data,("Invalid Lenght of the Stream Buffer"))
		/*retun FAILURE with appropriate error*/
		*pError =  ARDNSCL_ERROR_DNS_INVALID_BUFFER_LENGTH;
		return ARDNSCL_FAILURE;
	}

	*pLength = 0;

	/*Allocating memory for p_temp_buffer character buffer */
	ARDNSCL_MEMGET(p_temp_buffer,ARDNSCL_MAX_BUFFER_LEN)
               
	/*Check for memory allocation failure*/
	if(ARDNSCL_NULL == p_temp_buffer)
	{
		ARDNSCL_CLIB_PRINT (pGlob_data, ("Memory Allocation Failure For tempory Buffer"))
		*pError = ARDNSCL_ERROR_MEMORY_ALLOC_FAILURE; 
		return ARDNSCL_FAILURE;
	}

	/*Assigning address of character buffer to temp buffer*/
	*ppDest =  p_temp_buffer;

	/*packing API ID to character buffer*/
	arDnsClient_port_pack_U8(p_temp_buffer + (*pLength),
            (dnscl_uint8_t *)&(pResponse->api_id), &len_val);

	/*Increment the length by size of length*/
	*pLength += len_val;

	/*Increment the pDest by size of length*/
	len_val = ARDNSCL_INIT_VAL;

    /*check for buffer overflow*/
    if(ARDNSCL_MAX_BUFFER_LEN <= *pLength)
    {
	    ARDNSCL_CLIB_PRINT (pGlob_data, ("Tempory Buffer Overflow\n"));
	    *pError =  ARDNSCL_ERROR_DNS_INVALID_MSG_BUFFER;
	    return ARDNSCL_FAILURE;
    }

    p_store_buffer = p_temp_buffer + (*pLength);
    pResponse->payload_size = 0;

	/*packing payload size ID to character buffer*/
	arDnsClient_port_pack_U32(p_temp_buffer + (*pLength),
            (dnscl_uint8_t *)&(pResponse->payload_size), &len_val);

	/*Increment the length by size of length*/
	*pLength += len_val;

	/*Increment the p_temp_buffer by size of length*/
	len_val = ARDNSCL_INIT_VAL;

	/*check for buffer overflow*/
	if(ARDNSCL_MAX_BUFFER_LEN <= *pLength)
	{
		ARDNSCL_CLIB_PRINT (pGlob_data, ("Tempory Buffer Overflow\n"));
		*pError =  ARDNSCL_ERROR_DNS_INVALID_MSG_BUFFER;
		return ARDNSCL_FAILURE;
	}
	/*Packing the result */
	arDnsClient_port_pack_U8(p_temp_buffer + (*pLength),
            (dnscl_uint8_t *)&(pResponse->result), &len_val);

	/*Increment the length by size of length*/
	*pLength += len_val;

	/*Increment the p_temp_buffer by size of length*/
	len_val = ARDNSCL_INIT_VAL;
	
	/*check for buffer overflow*/
	if(ARDNSCL_MAX_BUFFER_LEN <= *pLength)
	{
		ARDNSCL_CLIB_PRINT (pGlob_data, ("Tempory Buffer Overflow\n"));
		*pError =  ARDNSCL_ERROR_DNS_INVALID_MSG_BUFFER;
		return ARDNSCL_FAILURE;
	}


    /*Pack query*/
    arDnsClient_port_pack_query(p_temp_buffer + (*pLength),
            pResponse->pQuery,
            &len_val);


    /*Increment the value of  p_length  by length*/
    *pLength += len_val;

    /*Intialize value of length to zero*/
    len_val = ARDNSCL_INIT_VAL;

	/*Packing  response structure if Query response ispresent*/
	if(ARDNSCL_SUCCESS == pResponse->result)
	{
		/*check for buffer overflow*/
		if(ARDNSCL_MAX_BUFFER_LEN <= *pLength)
		{
			ARDNSCL_CLIB_PRINT (pGlob_data, ("Tempory Buffer Overflow\n"));
			*pError =  ARDNSCL_ERROR_DNS_INVALID_MSG_BUFFER;
			return ARDNSCL_FAILURE;
		}

		arDnsClient_port_pack_query_resp(p_temp_buffer + (*pLength),
             pResponse->pQuery->query_type,
			 pResponse->pDnsQueryResp,&len_val);

		/*Increment the length by size of length*/
	    *pLength += len_val;

		/*Increment the p_temp_buffer by size of length*/
		len_val = ARDNSCL_INIT_VAL;
	}
    else
    {
        /*check for buffer overflow*/
		if(ARDNSCL_MAX_BUFFER_LEN <= *pLength)
		{
			ARDNSCL_CLIB_PRINT (pGlob_data, ("Tempory Buffer Overflow\n"));
			*pError =  ARDNSCL_ERROR_DNS_INVALID_MSG_BUFFER;
			return ARDNSCL_FAILURE;
		}

	   /*Packing  response structure if error is  present*/
	   arDnsClient_port_pack_U8(p_temp_buffer + (*pLength),
            (dnscl_uint8_t *)&(pResponse->ecode), &len_val);

	   /*Increment the length by size of length*/
	   *pLength += len_val;
    }


	/*Setting the value of length to zero*/
	len_val = ARDNSCL_INIT_VAL;

    payload_length = *pLength;

	/*packing payload length to character buffer*/
	arDnsClient_port_pack_U32(p_store_buffer ,(dnscl_uint8_t *)&payload_length, &len_val);

    /*check for buffer overflow*/
	if(ARDNSCL_MAX_BUFFER_LEN <= *pLength)
	{
		ARDNSCL_CLIB_PRINT (pGlob_data, ("Tempory Buffer Overflow\n"));
		*pError =  ARDNSCL_ERROR_DNS_INVALID_MSG_BUFFER;
		return ARDNSCL_FAILURE;
	}

    (*ppDest)[payload_length] = '\0';

	ARDNSCL_CLIB_PRINT (pGlob_data, ("Exiting Function arDnsClient_port_pack_response\n"));
	/*return Success*/
	return ARDNSCL_SUCCESS;
}

/****************************************************************************
 * API Name: arDnsClient_port_pack_request
 * 
 * Description: This API is invoked by the application for packing the request 
 *              ie converting the request structure to stream buffer. 
 * INPUT       :pDnscl_glb_data - The pointer to the global data of arDnsClient.
 *             :pRequest - The pointer to request structure.  
 * OUTPUT      :ppDest - The pointer to the stream buffer
 *             :pLength - Pointer to the lenght of the stream buffer.
 *             :pError - Error pointer to be populated in case of error. 
 *
 * return : It will return ADNSCL_SUCCESS if successful else return ARDNSCL_FAILURE.
 ******************************************************************************/
dnscl_return_t  arDnsClient_port_pack_request (
		   dnscl_void_t             *pDnsCl_glb_data,
		   arDnsClient_request_st   *pRequest,
		   dnscl_uint8_t            **ppDest,
		   dnscl_uint16_t           *pLength,
		   dnscl_error_t            *pError)

{

	/*Variable to store buffer location*/      
	dnscl_uint8_t   *p_temp_buffer = ARDNSCL_NULL;

	/*Variable to store buffer location*/      
	dnscl_uint8_t   *p_store_buffer = ARDNSCL_NULL ;

	/*Variable to store length*/    
	dnscl_uint32_t     len_val = ARDNSCL_INIT_VAL;

        /*Variable to store length*/    
	arDnsClient_global_data_st     *pGlob_data = ARDNSCL_NULL;

	/*Variable to store payload length*/    
	dnscl_uint32_t     payload_length = ARDNSCL_INIT_VAL;


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

	ARDNSCL_CLIB_PRINT (pGlob_data, ("Entered Function arDnsClient_port_pack_request\n"))

	/*Check for  length pointer*/
	if(ARDNSCL_NULL == pLength)
	{
		ARDNSCL_CLIB_PRINT (pGlob_data, ("Invalid Lenght of the Stream Buffer"))
		/*retun FAILURE with appropriate error*/
		*pError =  ARDNSCL_ERROR_DNS_INVALID_BUFFER_LENGTH;
		return ARDNSCL_FAILURE;
	}
        
      /*Intialize pLength to 0*/
	*pLength = ARDNSCL_INIT_VAL;

	/*Allocating memory for pDest character buffer */
	ARDNSCL_MEMGET(p_temp_buffer, 
            ARDNSCL_SIZEOF(dnscl_uint8_t) * ARDNSCL_MAX_BUFFER_LEN)

	/*Check for memory allocation failure*/
	if(ARDNSCL_NULL == p_temp_buffer)
	{
		ARDNSCL_CLIB_PRINT (pGlob_data, ("Memory Allocation Failure For tempory Buffer"))
		*pError = ARDNSCL_ERROR_MEMORY_ALLOC_FAILURE; 
		return ARDNSCL_FAILURE;
	}
       

	/*Assigning address of character buffer to temp buffer*/
	*ppDest =  p_temp_buffer;

	/*packing API ID to character buffer*/
	arDnsClient_port_pack_U8(p_temp_buffer + (*pLength),
            (dnscl_uint8_t *)&(pRequest->api_id), &len_val);


	/*Increment the length by size of length*/
	*pLength += len_val;

	/*Increment the pDest by size of length*/
	len_val = ARDNSCL_INIT_VAL;


	
   
    /*check for buffer overflow*/
    if(ARDNSCL_MAX_BUFFER_LEN <= *pLength )
    {
	    ARDNSCL_CLIB_PRINT (pGlob_data, ("Tempory Buffer Overflow\n"));
	    *pError =  ARDNSCL_ERROR_DNS_INVALID_MSG_BUFFER;
	    return ARDNSCL_FAILURE;
    }

    /*Assigning address of character buffer to another temp 
	  buffer*/
	p_store_buffer = p_temp_buffer + (*pLength);

    pRequest->payload_size = 0;

	/*Assigning payload to character buffer*/
	arDnsClient_port_pack_U32(p_temp_buffer + (*pLength),
            (dnscl_uint8_t *)&(pRequest->payload_size), 
			&len_val);


	/*Increment the length by size of length*/
	*pLength += len_val;

	/*Setting the value of length to zero*/
	len_val = ARDNSCL_INIT_VAL;
        

	/*Checking the API iD for packing*/
	if(ARDNSCL_API_SET_TRACE_REQ == pRequest->api_id)
	{
		/*check for buffer overflow*/
		if(ARDNSCL_MAX_BUFFER_LEN <= *pLength)
		{
			ARDNSCL_CLIB_PRINT (pGlob_data, ("Tempory Buffer Overflow\n"));
			*pError =  ARDNSCL_ERROR_DNS_INVALID_MSG_BUFFER;
			return ARDNSCL_FAILURE;
		}
		/*Pack trace value  to character buffer*/
		arDnsClient_port_pack_U8(p_temp_buffer + (*pLength),
				(dnscl_uint8_t *)&(pRequest->enableTrace),&len_val);

		/*Increment the length by size of length*/
	    *pLength += len_val;

		/*Setting the value of length to zero*/
		len_val = ARDNSCL_INIT_VAL;
	}
	/*Checking the bitmask for configuration type of API*/
	else if(ARDNSCL_API_SET_CONF_REQ == pRequest->api_id)

	{
		/*check for buffer overflow*/
		if(ARDNSCL_MAX_BUFFER_LEN <= *pLength)
		{
			ARDNSCL_CLIB_PRINT (pGlob_data, ("Tempory Buffer Overflow\n"));
			*pError =  ARDNSCL_ERROR_DNS_INVALID_MSG_BUFFER;
			return ARDNSCL_FAILURE;
		}
		/*packing configuration data  to character buffer*/
		arDnsClient_port_pack_conf(p_temp_buffer + (*pLength),
				pRequest->pConfData, &len_val);

		/*Increment the length by size of length*/
		*pLength += len_val;

		/*Setting the value of length to zero*/
		len_val = ARDNSCL_INIT_VAL;
	}
	/*Checking the bitmask for Query type of API*/
	else if(ARDNSCL_API_SEND_QUERY_REQ == pRequest->api_id)

	{
		/*check for buffer overflow*/
		if(ARDNSCL_MAX_BUFFER_LEN <= *pLength)
		{
			ARDNSCL_CLIB_PRINT (pGlob_data, ("Tempory Buffer Overflow\n"));
			*pError =  ARDNSCL_ERROR_DNS_INVALID_MSG_BUFFER;
			return ARDNSCL_FAILURE;
		}

		/*packing query data to character buffer*/
		arDnsClient_port_pack_query(p_temp_buffer + (*pLength),
				pRequest->pDnsQuery, &len_val);

		/*Increment the length by size of length*/
		*pLength += len_val;

		/*Setting the value of length to zero*/
		len_val = ARDNSCL_INIT_VAL;
               
                /*check for buffer overflow*/
		if(ARDNSCL_MAX_BUFFER_LEN <= *pLength)
		{
			ARDNSCL_CLIB_PRINT (pGlob_data, ("Tempory Buffer Overflow\n"));
                        *pError =  ARDNSCL_ERROR_DNS_INVALID_MSG_BUFFER;
			return ARDNSCL_FAILURE;
		}
	
		/*Packing cache counter value to character buffer*/
		arDnsClient_port_pack_U32(p_temp_buffer + (*pLength),
				(dnscl_uint8_t *)&(pRequest->cache_counter), &len_val);

		/*Increment the length by size of length*/
	    *pLength += len_val;

		/*Setting the value of length to zero*/
		len_val = ARDNSCL_INIT_VAL;
                

	}
	/*Checking the bit mask for clear cache type of API*/
	else if(ARDNSCL_API_CLEAR_CACHE_REQ == pRequest->api_id)

	{
                /*check for buffer overflow*/
		if(ARDNSCL_MAX_BUFFER_LEN <= *pLength)
		{
			ARDNSCL_CLIB_PRINT (pGlob_data, ("Tempory Buffer Overflow\n"));
                        *pError =  ARDNSCL_ERROR_DNS_INVALID_MSG_BUFFER;
			return ARDNSCL_FAILURE;
		}
		/*Assigning  clear cache data to character buffer*/
		arDnsClient_port_pack_clear_cache(p_temp_buffer + (*pLength),
					    pRequest->pClearCache,&len_val);
 
		/*Increment the length by size of length*/
		*pLength += len_val;

		/*Setting the value of length to zero*/
		len_val = ARDNSCL_INIT_VAL;
     
	}

    payload_length = *pLength;

	/*packing payload length to character buffer*/
	arDnsClient_port_pack_U32(p_store_buffer ,(dnscl_uint8_t *)&payload_length, &len_val);
     
        (*ppDest)[payload_length] = '\0';

	/*Setting the value of length to zero*/
	len_val = ARDNSCL_INIT_VAL;
	ARDNSCL_CLIB_PRINT (pGlob_data, ("Exiting Function arDnsClient_port_pack_request\n"));
	/*return Success*/
	return ARDNSCL_SUCCESS;
}

