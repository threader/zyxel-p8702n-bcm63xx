/******************************************************************************
*                                    
* FILE NAME    : arDnsClient_port_port_unpack.c                
*                                    
* DESCRIPTION  : Contains the arDnsClient porting unpacking routines  
*                                    
* Revision History :                            
*
*  DATE            NAME                 REFERENCE       REASON        
* ------         ------------           ---------     ----------
* 18-Feb-2009    Anuradha Gupta         None          Initial
* 11-Jun-2009 Ashutosh Mohan     SPR 19737     Changes done for GCC 4.1 warnings removal
* Copyright 2009, Aricent.                       
*                                    
*******************************************************************************/

#include "arDnsClient_port_prototypes.h"
#include "arDnsClient_cmn_prototypes.h"
#include "arDnsClient_api.h"

/****************************************************************************
 * Function Name: arDnsClient_port_unpack_U8 
 * 
 * Description: This Function is invoked to convert stream buffer to U8 data type. 
 * 
 ******************************************************************************/
dnscl_return_t
arDnsClient_port_unpack_U8
(
    dnscl_uint8_t    *pDest,
    dnscl_uint8_t    *pSrc,
    dnscl_uint32_t   *p_length)
{

    /*Assign sizeof of dnscl_uint8_t*/
    *p_length    = ARDNSCL_SIZEOF(dnscl_uint8_t);

    /*assigning string*/
    *pDest = *pSrc;  

    return ARDNSCL_SUCCESS;

}

/****************************************************************************
 * Function Name: arDnsClient_port_unpack_U16 
 * 
 * Description: This Function is invoked to convert stream buffer to U16 data type. 
 * 
 ******************************************************************************/
dnscl_return_t
arDnsClient_port_unpack_U16
(
    dnscl_uint8_t    *pDest,
    dnscl_uint8_t    *pSrc,
    dnscl_uint32_t   *p_length)
{

    /*Assign sizeof of dnscl_uint8_t*/
    *p_length    = ARDNSCL_SIZEOF(dnscl_uint16_t);

    /* Unpacking U16 */
    *(dnscl_uint16_t*)pDest    = 0;
    *(dnscl_uint16_t*)pDest    |= (dnscl_uint16_t)pSrc[0] << 8;
    *(dnscl_uint16_t*)pDest    |= (dnscl_uint16_t)pSrc[1];

    return ARDNSCL_SUCCESS;
}

/****************************************************************************
 * Function Name: arDnsClient_port_unpack_U32
 * 
 * Description: This Function is invoked to convert stream buffer to U32 data type. 
 * 
 ******************************************************************************/
dnscl_return_t
arDnsClient_port_unpack_U32
(
    dnscl_uint8_t    *pDest,
    dnscl_uint8_t    *pSrc,
    dnscl_uint32_t   *p_length)
{

    /*Assign sizeof of dnscl_uint8_t*/
    *p_length    = ARDNSCL_SIZEOF(dnscl_uint32_t);

    /*Unpack 4 byte integer*/
    *(dnscl_uint32_t*)pDest    |= (dnscl_uint32_t)pSrc[0] << 24;
    *(dnscl_uint32_t*)pDest    |= (dnscl_uint32_t)pSrc[1] << 16;
    *(dnscl_uint32_t*)pDest    |= (dnscl_uint32_t)pSrc[2] << 8;
    *(dnscl_uint32_t*)pDest    |= (dnscl_uint32_t)pSrc[3];

    return ARDNSCL_SUCCESS;
}

/****************************************************************************
 * Function Name: arDnsClient_port_unpack_string 
 * 
 * Description: This Function is invoked to convert stream buffer to string
 *              data type. 
 * 
 ******************************************************************************/
dnscl_return_t
arDnsClient_port_unpack_string(
    arDnsClient_string_st    *pDest,
    dnscl_uint8_t    		*pSrc,
    dnscl_uint32_t 		*p_length)
{
    /*Variable for length*/  
    dnscl_uint32_t        len_val = 0;

    /*Variable for count*/
    dnscl_uint32_t        count = 0;


    /* Initialise length to 0 */
    *p_length    = 0;

    /* unpack string length */
    arDnsClient_port_unpack_U8(
            (dnscl_uint8_t*)&(pDest->str_len),
            pSrc + (*p_length),
            &len_val);

    /*Assign length variable to p_length */
    *p_length    += len_val;

    /*done to avoid klockwork warning*/
    if(ARDNSCL_MAX_STR_LEN <  pDest->str_len)
    {
	    pDest->str_len =ARDNSCL_MAX_STR_LEN;
    }
    /*unpack the string*/
    for(count=0;count< pDest->str_len;count++)
    {
        arDnsClient_port_unpack_U8(
                (dnscl_uint8_t*)&(pDest->str_val[count]),
                pSrc + (*p_length), 
                &len_val);

        /*Assign length variable to p_length */
        *p_length    += len_val;
    }

    /*return success*/ 
    return ARDNSCL_SUCCESS;
}

/****************************************************************************
 * Function Name: arDnsClient_port_unpack_short_string 
 * 
 * Description: This Function is invoked to convert stream buffer to short  string
 *              data type. 
 * 
 ******************************************************************************/
dnscl_return_t
arDnsClient_port_unpack_short_string
(
    arDnsClient_short_string_st    *pDest,
    dnscl_uint8_t    		*pSrc,
    dnscl_uint32_t 		*p_length)
{
    /*Variable for length*/  
    dnscl_uint32_t        len_val = 0;

    /*Variable for count*/
    dnscl_uint32_t        count = 0;


    /* Initialise length to 0 */
    *p_length    = 0;

    /* unpack string length */
    arDnsClient_port_unpack_U8(
            (dnscl_uint8_t*)&(pDest->str_len),
            (dnscl_uint8_t *)pSrc + (*p_length),
            &len_val);

    /*Assign length variable to p_length */
    *p_length    += len_val;

    /*done to avoid klockwork warning*/
    if(ARDNSCL_MAX_SHORT_STR_LEN <  pDest->str_len)
    {
	    pDest->str_len = ARDNSCL_MAX_SHORT_STR_LEN;
    }
    /*unpack the string*/
    for(count=0;count< pDest->str_len;count++)
    {
        arDnsClient_port_unpack_U8(
                (dnscl_uint8_t*)&(pDest->str_val[count]),
                pSrc + (*p_length), 
                &len_val);

        /*Assign length variable to p_length */
        *p_length    += len_val;
    }

    /*return success*/ 
    return ARDNSCL_SUCCESS;
}

/****************************************************************************
 * Function Name: arDnsClient_port_unpack_address
 * 
 * Description: This Function is invoked to convert stream buffer to address 
 *              data type. 
 * 
 ******************************************************************************/
dnscl_return_t
arDnsClient_port_unpack_address(
    arDnsClient_address_st  *pDest,
    dnscl_uint8_t           *pSrc,
    dnscl_uint32_t          *p_length)
{
    /*Variable for length*/
    dnscl_uint32_t        len_val = 0;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* Function unpacks Address */
    arDnsClient_port_unpack_short_string(
            &(pDest ->ipAddress),
            (dnscl_uint8_t *)pSrc + (* p_length), 
            &len_val);

    /*Incrementing the p_length by len_val*/
    *p_length    += len_val;

    /*Intialize the value of length to zero*/
    len_val = 0;

    /* Unpack the port*/
    arDnsClient_port_unpack_U16(
            (dnscl_uint8_t*)&(pDest->port),
            (dnscl_uint8_t *)pSrc + (*p_length), 
            &len_val);

    /*Incrementing the p_length by len_val*/
    *p_length    += len_val;
    
    return ARDNSCL_SUCCESS;
}

/****************************************************************************
 * Function Name: arDnsClient_port_unpack_conf
 * 
 * Description: This Function is invoked to convert stream buffer to conf 
 *              data type. 
 * 
 ******************************************************************************/
dnscl_return_t
arDnsClient_port_unpack_conf(
	arDnsClient_conf_st **ppConfData,
	dnscl_uint8_t        *pSrc,
    dnscl_uint32_t        *p_length,
    dnscl_error_t        *pError)
{
    /*Variable to store length*/       
    dnscl_uint32_t    len_val = ARDNSCL_INIT_VAL;

    /*Pointer to configuration structure*/
    arDnsClient_conf_st *pConfData = ARDNSCL_NULL;

    /* Initialise length to 0 */
    *p_length    = 0;

    /*Allocate memory for conf structure*/
    ARDNSCL_MEMGET(pConfData, ARDNSCL_SIZEOF(arDnsClient_conf_st));

    /*Check for memory allocation failure*/
    if(ARDNSCL_NULL == pConfData)
    {
        *pError = ARDNSCL_ERROR_MEMORY_ALLOC_FAILURE;
        return ARDNSCL_FAILURE; 
    }

    /*Packing the DNS server address*/
    arDnsClient_port_unpack_address(
            &(pConfData->dns_server_addr), 
            pSrc + (*p_length), 
            &len_val);

    /*Increment the vlue of pLength  by len_val*/
    *p_length += len_val ;

    /*Address assignment of conf data */
    *ppConfData= pConfData;

    return ARDNSCL_SUCCESS;
}

/****************************************************************************
 * Function Name: arDnsClient_port_unpack_query
 * 
 * Description: This Function is invoked to convert stream buffer to query 
 *              data type. 
 * 
 ******************************************************************************/
dnscl_return_t
arDnsClient_port_unpack_query(
	arDnsClient_query_st **ppDnsQuery,
	dnscl_uint8_t 	     *pSrc,
    dnscl_uint32_t        *p_length,
    dnscl_error_t        *pError)
{

    /*Variable to store length*/    
    dnscl_uint32_t      len_val = ARDNSCL_INIT_VAL;

    /*Pointer to query structure */
    arDnsClient_query_st *pDnsQuery = ARDNSCL_NULL;

	ARDNSCL_MEMGET(pDnsQuery, ARDNSCL_SIZEOF(arDnsClient_query_st));


    /*Check for memory allocation failure*/
    if(ARDNSCL_NULL == pDnsQuery )
    {
        *pError = ARDNSCL_ERROR_MEMORY_ALLOC_FAILURE; 
        return ARDNSCL_FAILURE;
    }

    /* Initialise length to 0 */
    *p_length    = 0;


    /*Unpack the query type*/
    arDnsClient_port_unpack_U8(
            (dnscl_uint8_t *)&(pDnsQuery->query_type),
            pSrc + (*p_length), 
            &len_val);


    /*Incremt the size of pLength  by len_val*/
    *p_length += len_val;

    /* Initialise length to 0 */
    len_val = 0;

    /*Packing the domain name*/
    arDnsClient_port_unpack_string(
            &(pDnsQuery->query_string),
            pSrc + (*p_length), 
            &len_val); 


    /*Incremt the size of pLength  by len_val*/
    *p_length += len_val;

    /* Initialise length to 0 */
    len_val = 0;

    /*UnPack the query context*/
    arDnsClient_port_unpack_U32(
            (dnscl_uint8_t *)&(pDnsQuery->pApp_query_ctx), 
            pSrc + (*p_length),
            &len_val);

    /*Incremt the size of pLength  by len_val*/
    *p_length += len_val;

    /*Address assignment of query data */
    *ppDnsQuery = pDnsQuery;

    return ARDNSCL_SUCCESS;
}

/****************************************************************************
 * API Name: ardnscl_port_unpack_clear_cache
 * 
 * Description:    This function shall be  used to unpack clear cache data 
 * INPUT       : ppClearCache is the data to be unpack .
 * OUTPUT      :p_length  length of data that has been unpack. 
 *             :p_dest is the buffer to copy unpack data
 *
 * return :  ARDNSCL_SUCCESS if successful else return ARDNSCL_FAILURE.
 ******************************************************************************/
dnscl_return_t
arDnsClient_port_unpack_clear_cache(
	arDnsClient_clear_cache_st **ppClearCache,
	dnscl_uint8_t 	     *pSrc,
	dnscl_uint32_t        *pLength,
	dnscl_error_t        *pError)
{
	/*Variable to store length*/    
	dnscl_uint32_t      len_val = ARDNSCL_INIT_VAL;

	/*Pointer to clear cache data*/
	arDnsClient_clear_cache_st *pClearCache = ARDNSCL_NULL;

	ARDNSCL_MEMGET(pClearCache , ARDNSCL_SIZEOF(arDnsClient_clear_cache_st));

	/*Check for memory allocation failure*/
	if(ARDNSCL_NULL == pClearCache)
	{
		*pError = ARDNSCL_ERROR_MEMORY_ALLOC_FAILURE;
		return ARDNSCL_FAILURE; 
	}

	/*Unpack the query type*/
	arDnsClient_port_unpack_U8(&pClearCache->clear_options,(dnscl_uint8_t *)pSrc, &len_val);

	/*Incrementing the source charcter buffer pointer*/
	pSrc += len_val;

	/*Incrementing the pLength bt len_val*/
	*pLength =len_val;

	/* Initialise length to 0 */
	len_val = 0;

	if(ARDNSCL_CLEAR_SINGLE_ENTRY & pClearCache->clear_options)
	{
		/*UnPack the domain name*/
		arDnsClient_port_unpack_string(&pClearCache->domain_name,pSrc, &len_val);                          

		/*Increment the source buffer*/
		pSrc += len_val;

		/*Incrementing the pLength bt len_val*/
		*pLength =len_val;

		/* Initialise length to 0 */
		len_val = 0;

		/*Unpack the query type*/
		arDnsClient_port_unpack_U8(&pClearCache->query_type,(dnscl_uint8_t *)pSrc, &len_val);

		/*Increment the source buffer*/
		pSrc += len_val;

		/*Incrementing the pLength bt len_val*/
		*pLength =len_val;

		/* Initialise length to 0 */
		len_val = 0;
	}

	/*Assign the cache Address to ppClearCache */
	*ppClearCache= pClearCache;

	/*return SUCCESS*/
	return ARDNSCL_SUCCESS;
}
/****************************************************************************
 * API Name: arDnsClient_port_unpack_query_rr_data
 * 
 * Description:    This function shall be  used to unpack resource record 
 * INPUT       :ppQuery_resp is the data to be unpack .
 * OUTPUT      :p_length  length of data that has been unpack. 
 *             :p_dest is the buffer to copy unpack data
 *
 * return :  ARDNSCL_SUCCESS if successful else return ARDNSCL_FAILURE.
 ******************************************************************************/

dnscl_uint32_t
arDnsClient_port_unpack_query_rr_data(
		arDnsClient_rr_data_un  *pQuery_rr_data,
		dnscl_uint8_t    	query_type,
		dnscl_uint8_t    	*pSrc,
		dnscl_uint32_t          *pLength)

{
	/*Variable for length*/
	dnscl_uint32_t        len_val = ARDNSCL_INIT_VAL;       

	/*UnPack if query type is  NAPTR */
	if(ARDNSCL_QUERY_TYPE_NAPTR == query_type)
	{

		/*UnPack order for NAPTR query*/
		arDnsClient_port_unpack_U16((dnscl_uint8_t *)&(pQuery_rr_data->naptr_rr_data.order),
				(dnscl_uint8_t *)pSrc, &len_val);

		/*Increment the character buffer by value of length*/
		pSrc += len_val;

		/*Increment the p_length by value of length*/
		*pLength += len_val;

		/*Intialize value of length to zero*/
		len_val = ARDNSCL_INIT_VAL;

		/*UnPack preference for NAPTR query*/
		arDnsClient_port_unpack_U16((dnscl_uint8_t *)&(pQuery_rr_data->naptr_rr_data.
				preference),(dnscl_uint8_t *)pSrc,&len_val);

		/*Increment the character buffer by value of length*/
		pSrc += len_val;

		/*Increment the p_length by value of length*/
		*pLength += len_val;

		/*Intialize value of length to zero*/
		len_val = ARDNSCL_INIT_VAL;

		/*UnPack flag string for NAPTR query*/
		arDnsClient_port_unpack_short_string(&pQuery_rr_data->naptr_rr_data.
				flags,(dnscl_uint8_t *)pSrc, &len_val);

		/*Increment the character buffer by value of length*/
		pSrc += len_val;

		/*Increment the p_length by value of length*/
		*pLength += len_val;

		/*Intialize value of length to zero*/
		len_val = ARDNSCL_INIT_VAL;

		/*UnPack flag string for NAPTR query*/
		arDnsClient_port_unpack_short_string(&(pQuery_rr_data->naptr_rr_data.
				service),(dnscl_uint8_t *)pSrc, &len_val);

		/*Increment the character buffer by value of length*/
		pSrc += len_val;

		/*Increment the p_length by value of length*/
		*pLength += len_val;

		/*Intialize value of length to zero*/
		len_val = ARDNSCL_INIT_VAL;

		/*UnPack flag string for NAPTR query*/
		arDnsClient_port_unpack_short_string(&pQuery_rr_data->naptr_rr_data.
				regularExpr,(dnscl_uint8_t *)pSrc, &len_val);

		/*Increment the character buffer by value of length*/
		pSrc += len_val;

		/*Increment the p_length by value of length*/
		*pLength += len_val;

		/*Intialize value of length to zero*/
		len_val = ARDNSCL_INIT_VAL;

		/*UnPack flag string for NAPTR query*/
		arDnsClient_port_unpack_string(&pQuery_rr_data->naptr_rr_data.replacement,(dnscl_uint8_t *)pSrc, &len_val);

		/*Increment the character buffer by value of length*/
		pSrc += len_val;

		/*Increment the p_length by value of length*/
		*pLength += len_val;

		/*Intialize value of length to zero*/
		len_val = ARDNSCL_INIT_VAL;


	}

	/*UnPack if Query type is SRV*/
	if(query_type == ARDNSCL_QUERY_TYPE_SRV)
	{
		/*UnPack name target  string for SRV query*/
		arDnsClient_port_unpack_string(&pQuery_rr_data->srv_rr_data.
				nameTarget, pSrc, &len_val);

		/*Increment the character buffer by value of length*/
		pSrc += len_val;

		/*Increment the p_length by value of length*/
		*pLength += len_val;

		/*Intialize value of length to zero*/
		len_val = ARDNSCL_INIT_VAL;

		/*UnPack priority for SRV query*/
		arDnsClient_port_unpack_U16((dnscl_uint8_t*)&(pQuery_rr_data->srv_rr_data.
				priority), (dnscl_uint8_t *)pSrc,&len_val);

		/*Increment the character buffer by value of length*/
		pSrc += len_val;

		/*Increment the p_length by value of length*/
		*pLength += len_val;

		/*Intialize value of length to zero*/
		len_val = ARDNSCL_INIT_VAL;

		/*UnPack weight for SRV query*/
		arDnsClient_port_unpack_U16((dnscl_uint8_t*)&(pQuery_rr_data->srv_rr_data.weight),
					(dnscl_uint8_t *)pSrc,&len_val);

		/*Increment the character buffer by value of length*/
		pSrc += len_val;

		/*Increment the p_length by value of length*/
		*pLength += len_val;

		/*Intialize value of length to zero*/
		len_val = ARDNSCL_INIT_VAL;

		/*UnPack port for SRV query*/
		arDnsClient_port_unpack_U16((dnscl_uint8_t*)&(pQuery_rr_data->srv_rr_data.port),
					(dnscl_uint8_t *)pSrc, &len_val);

		/*Increment the character buffer by value of length*/
		pSrc += len_val;

		/*Increment the p_length by value of length*/
		*pLength += len_val;

		/*Intialize value of length to zero*/
		len_val = ARDNSCL_INIT_VAL;

	} 

	/*UnPack if Query type is SRV*/
	if( ARDNSCL_QUERY_TYPE_A == query_type)
	{
		/*Packing IPV4 address for A query*/
		arDnsClient_port_unpack_short_string(&(pQuery_rr_data->A_rr_data.addrIP4),
				(dnscl_uint8_t *)pSrc,&len_val);

		/*Increment the character buffer by value of length*/
		pSrc += len_val;

		/*Increment the p_length by value of length*/
		*pLength += len_val;

		/*Intialize value of length to zero*/
		len_val = ARDNSCL_INIT_VAL;
	}

	/*UnPack if Query type is AAAA*/
	if(ARDNSCL_QUERY_TYPE_AAAA == query_type)
	{
		/*UnPack IPV6 address for A query*/
		arDnsClient_port_unpack_short_string(&(pQuery_rr_data->AAAA_rr_data.addrIP6),
				(dnscl_uint8_t *)pSrc,&len_val);

		/*Increment the character buffer by value of length*/
		pSrc += len_val;

		/*Increment the p_length by value of length*/
		*pLength += len_val;

		/*Intialize value of length to zero*/
		len_val = ARDNSCL_INIT_VAL;
	}

	/*return SUCCESS*/
	return ARDNSCL_SUCCESS;
}


/****************************************************************************
 * API Name: arDnsClient_port_unpack_query_resp
 * 
 * Description:    This function shall be  used to unpack query response 
 * INPUT       :ppQuery_resp is the data to be unpack .
 * OUTPUT      :p_length  length of data that has been unpack. 
 *             :p_dest is the buffer to copy unpack data
 *
 * return :  ARDNSCL_SUCCESS if successful else return ARDNSCL_FAILURE.
 ******************************************************************************/
dnscl_uint32_t
arDnsClient_port_unpack_query_resp(
		arDnsClient_query_resp_st   **ppQuery_resp,
        dnscl_query_t           query_type, 
		dnscl_uint8_t    	    *pSrc,
		dnscl_uint32_t          *pLength,
        dnscl_error_t           *pError)
{
	/*Variable for length*/
	dnscl_uint32_t        len_val = ARDNSCL_INIT_VAL;
	
	/*Variable for count number of RR*/
	dnscl_uint32_t        count = ARDNSCL_INIT_VAL;

    arDnsClient_dns_rr_list_st   *p_temp_record = ARDNSCL_NULL;
    arDnsClient_dns_rr_list_st     *p_temp1_record = ARDNSCL_NULL ;
     
	/*Pointer to query response structure*/
	arDnsClient_query_resp_st  *pQuery_resp = ARDNSCL_NULL;

	/*Allocate memory for pQuery_resp */
	ARDNSCL_MEMGET(pQuery_resp, ARDNSCL_SIZEOF(arDnsClient_query_resp_st));

	/*Check for memory allocation failure*/
	if(ARDNSCL_NULL == pQuery_resp )
	{
		*pError = ARDNSCL_ERROR_MEMORY_ALLOC_FAILURE;
		return ARDNSCL_FAILURE; 
	}

    /*Intialize value of length to zero*/
	len_val = ARDNSCL_INIT_VAL;

	/*Unpack number of records*/
	arDnsClient_port_unpack_U8(&pQuery_resp->numRecords,(dnscl_uint8_t *)pSrc,&len_val) ;

	/*Increment the source buffer*/
	pSrc += len_val;

	/*Assign the value of length to *p_length */    
	*pLength += len_val;

	/*Intialize value of length to zero*/
	len_val = ARDNSCL_INIT_VAL;
    pQuery_resp->pAnswerRecords = ARDNSCL_NULL;

		/*Unpack the answer records*/
		for(count = 0 ; count< pQuery_resp->numRecords; count++)
		{
			/*Allocate memory for answer records */
			if(ARDNSCL_NULL == pQuery_resp->pAnswerRecords)
			{
				ARDNSCL_MEMGET(pQuery_resp->pAnswerRecords , 
                        ARDNSCL_SIZEOF(arDnsClient_dns_rr_list_st))
			    if(ARDNSCL_NULL == pQuery_resp->pAnswerRecords)
			    {
				    ARDNSCL_MEMFREE(pQuery_resp);
				    *pError = ARDNSCL_ERROR_MEMORY_ALLOC_FAILURE;
				    return ARDNSCL_FAILURE;
			    }
				p_temp1_record = p_temp_record = pQuery_resp->pAnswerRecords;
				pQuery_resp->pAnswerRecords->pNext = ARDNSCL_NULL;
			}
			else
			{ 
				ARDNSCL_MEMGET(p_temp1_record ,
                        ARDNSCL_SIZEOF(arDnsClient_dns_rr_list_st));
				if(ARDNSCL_NULL == p_temp1_record)
				{
                    p_temp_record = pQuery_resp->pAnswerRecords;
                    while(ARDNSCL_NULL != p_temp_record)
                    {
                         p_temp1_record = p_temp_record->pNext;
                         ARDNSCL_MEMFREE(p_temp_record);
                         p_temp_record = p_temp1_record; 
                    }
					ARDNSCL_MEMFREE(pQuery_resp);
					*pError = ARDNSCL_ERROR_MEMORY_ALLOC_FAILURE;
					return ARDNSCL_FAILURE;
				}
				p_temp1_record->pNext = ARDNSCL_NULL;
				p_temp_record->pNext = p_temp1_record;
				p_temp_record = p_temp_record->pNext;
			}
			/*unpack ttl */
			arDnsClient_port_unpack_U32((dnscl_uint8_t *)&(p_temp1_record->ttl), 
                    (dnscl_uint8_t *)pSrc,
					&len_val);
			/*Increment the source buffer*/
			pSrc += len_val;

			/*Assign the value of length to *p_length */    
			*pLength += len_val;

			/*Intialize value of length to zero*/
			len_val = ARDNSCL_INIT_VAL;

			/*Unpack the Record resouce data*/
			arDnsClient_port_unpack_query_rr_data(&p_temp1_record->query_rr_data,
					query_type,
					pSrc, &len_val);

			/*Increment the source buffer*/
			pSrc += len_val;

			/*Assign the value of length to *p_length */    
			*pLength += len_val;

			/*Intialize value of length to zero*/
			len_val = ARDNSCL_INIT_VAL;
		}
       /*Assigning Query response address  to ppQuery_resp */ 
       *ppQuery_resp = pQuery_resp;
	
	/*return SUCCESS*/
	return ARDNSCL_SUCCESS;
}


/****************************************************************************
 * API Name: arDnsClient_port_unpack_request
 * 
 * Description: This API is invoked by the application for unpacking the request 
 *              ie converting the stream buffer to request structure. 
 * INPUT       :pDnscl_glb_data - The pointer to the global data of arDnsClient.
 *             :pSrc - The pointer to the stream buffer
 * OUTPUT      :ppRequest - The pointer to request structure
 *             :pLength - Pointer to the lenght of the stream buffer.
 *             :pError - Error pointer to be populated in case of error. 
 *
 * return : It will return ADNSCL_SUCCESS if successful else return ARDNSCL_FAILURE.
 ******************************************************************************/
dnscl_return_t  arDnsClient_port_unpack_request (
		   dnscl_void_t             *pDnsCl_glb_data,
		   dnscl_uint8_t            *pSrc,
		   arDnsClient_request_st   **ppRequest,
		   dnscl_uint16_t           *pLength,
		   dnscl_uint8_t            *pError)
{
	/*Variable to store length*/    
	dnscl_uint32_t        len_val = ARDNSCL_INIT_VAL;

         /*Return value*/
         dnscl_return_t ret_val = ARDNSCL_SUCCESS;

         /*Variable to store length*/    
	dnscl_uint8_t        error = ARDNSCL_INIT_VAL;


	/*Pointer to local request structure */
	arDnsClient_request_st      *pReq =  ARDNSCL_NULL;

	/*Pointer to global variable*/
	arDnsClient_global_data_st  *pGlob_data =  ARDNSCL_NULL;

	/*Check for  global data pointer*/
	if(ARDNSCL_NULL == pDnsCl_glb_data)
	{
		/*retun FAILURE with appropriate error*/
		*pError =  ARDNSCL_ERROR_INVALID_GLB_DATA;
		return ARDNSCL_FAILURE;
	}

	/*Type cast global data*/       
	pGlob_data =(arDnsClient_global_data_st*)pDnsCl_glb_data;

	/*Check for  pointer to the stream buffer*/
	if(ARDNSCL_NULL == pSrc)
	{
		/*retun FAILURE with appropriate error*/
		*pError =  ARDNSCL_ERROR_DNS_INVALID_MSG_BUFFER;
		return ARDNSCL_FAILURE;
	}


	/*Check for  length pointer*/
	if(ARDNSCL_NULL == pLength)
	{
		/*retun FAILURE with appropriate error*/
		*pError =  ARDNSCL_ERROR_DNS_INVALID_BUFFER_LENGTH;
		return ARDNSCL_FAILURE;
	}

	/*allocate memmory for request structure*/
	ARDNSCL_MEMGET(pReq, ARDNSCL_SIZEOF(arDnsClient_request_st))

	/*Check for memory allocation failure*/
	if(ARDNSCL_NULL == pReq )
	{
			*pError = ARDNSCL_ERROR_MEMORY_ALLOC_FAILURE; 
		return ARDNSCL_FAILURE;
	}

	/*Setting the value of length to zero*/
	len_val = ARDNSCL_INIT_VAL;

	/*unpacking API ID to request structure*/
	arDnsClient_port_unpack_U8((dnscl_uint8_t*)&(pReq->api_id),
            (dnscl_uint8_t *)pSrc, &len_val);

	/*Increment the pSrc by size of length*/
	pSrc += len_val;

	/*Setting the value of length to zero*/
	len_val = ARDNSCL_INIT_VAL;

	/*unpacking payload size to request structure*/
	arDnsClient_port_unpack_U32((dnscl_uint8_t *)&(pReq->payload_size), 
            (dnscl_uint8_t *)pSrc, &len_val);

	/*Increment the pSrc by size of length*/
	pSrc += len_val;

	/*Setting the value of length to zero*/
	len_val = ARDNSCL_INIT_VAL;

	/*Checking for Set configuration API*/
	if(ARDNSCL_API_SET_CONF_REQ == pReq->api_id)
	{
		ret_val = arDnsClient_port_unpack_conf(&(pReq->pConfData), 
                (dnscl_uint8_t *)pSrc,&len_val, &error);
                /*check for return type*/
		if(ARDNSCL_FAILURE == ret_val)
		{
			*pError = error;
			arDnsClient_free_request (pGlob_data, pReq,&error) ;
                        ARDNSCL_MEMFREE(pReq);
			return ARDNSCL_FAILURE;
		}

		/*Increment the pSrc by size of length*/
		pSrc += len_val;

		/*Setting the value of length to zero*/
		len_val = ARDNSCL_INIT_VAL;
	}
	else if (ARDNSCL_API_SET_TRACE_REQ == pReq->api_id)
	{
		/*unpacking trace value  to request structure*/
		arDnsClient_port_unpack_U8((dnscl_uint8_t *)&(pReq->enableTrace), 
                (dnscl_uint8_t *)pSrc, &len_val);

		/*Increment the pSrc by size of length*/
		pSrc += len_val;

		/*Setting the value of length to zero*/
		len_val = ARDNSCL_INIT_VAL;
	}
	else if (ARDNSCL_API_CLEAR_CACHE_REQ == pReq->api_id)
	{
		/*unpack clear cache*/
		ret_val = arDnsClient_port_unpack_clear_cache(&(pReq->pClearCache),
                (dnscl_uint8_t *)pSrc, &len_val, &error);
	
		/*check for return type*/
		if(ARDNSCL_FAILURE == ret_val)
		 {
			*pError = error;
			arDnsClient_free_request (pGlob_data, pReq,&error);
                        ARDNSCL_MEMFREE(pReq);
			return ARDNSCL_FAILURE;
		}

		
		/*Increment the pSrc by size of length*/
		pSrc += len_val;

		/*Setting the value of length to zero*/
		len_val = ARDNSCL_INIT_VAL;


	}
	else if (ARDNSCL_API_SEND_QUERY_REQ == pReq->api_id)
	{
		/*Unpack send query*/
		ret_val = arDnsClient_port_unpack_query(&(pReq->pDnsQuery),
                (dnscl_uint8_t *)pSrc, &len_val, &error);
		
		/*check for return type*/
		if(ARDNSCL_FAILURE == ret_val)
		{
                       *pError = error;
			arDnsClient_free_request (pGlob_data, pReq,&error);
                         ARDNSCL_MEMFREE(pReq);
			return ARDNSCL_FAILURE;
		}

		/*Increment the pSrc by size of length*/
			pSrc += len_val;

		/*Setting the value of length to zero*/
		len_val = ARDNSCL_INIT_VAL;

        /*unpacking payload size to request structure*/
	     arDnsClient_port_unpack_U32((dnscl_uint8_t *)&(pReq->cache_counter), 
            (dnscl_uint8_t *)pSrc, &len_val);
	}
    else if (ARDNSCL_API_DEINIT_REQ == pReq->api_id)
    {
        /* do nothing */
    }

	/*Assigning value of payload size to pLength */     
	*pLength=pReq->payload_size;

	/*Assign address of local request structure 
	  to unpack request structure*/
	*ppRequest = pReq;

	/*return Success*/
	return ARDNSCL_SUCCESS;
}

/****************************************************************************
 * API Name: arDnsClient_port_unpack_response
 * 
 * Description: This API is invoked by the application for unpacking the response 
 *              ie converting the stream buffer to responset structure. 
 * INPUT       :pDnscl_glb_data - The pointer to the global data of arDnsClient.
 *             :pSrc - The pointer to the stream buffer
 * OUTPUT      :ppResponse - The pointer to response structure
 *             :pLength - Pointer to the lenght of the stream buffer.
 *             :pError - Error pointer to be populated in case of error. 
 *
 * return : ADNSCL_SUCCESS if successful else return ARDNSCL_FAILURE.
 ******************************************************************************/
dnscl_return_t  arDnsClient_port_unpack_response (
		   dnscl_void_t             *pDnsCl_glb_data,
		   dnscl_uint8_t            *pSrc,
		   arDnsClient_response_st  **ppResponse,
		   dnscl_uint16_t           *pLength,
		   dnscl_uint8_t            *pError)
{
        /*Variable for Return type value*/
         dnscl_return_t   ret_val = ARDNSCL_SUCCESS;
 
	 /*Variable for error*/
	 dnscl_uint8_t           error = ARDNSCL_INIT_VAL;

	/*Variable to store length*/    
	dnscl_uint32_t        len_val = ARDNSCL_INIT_VAL;

	/*Pointer to local response structure*/
	arDnsClient_response_st    *pResp = NULL;

	/*Pointer to global variable*/
	arDnsClient_global_data_st  *pGlob_data =  ARDNSCL_NULL;

	/*Check for  global data pointer*/
	if(ARDNSCL_NULL == pDnsCl_glb_data)
	{
		/*retun FAILURE */
		*pError =  ARDNSCL_ERROR_INVALID_GLB_DATA;
		return ARDNSCL_FAILURE;
	}

	/*Type cast global data*/       
	pGlob_data =(arDnsClient_global_data_st*)pDnsCl_glb_data;


	/*Check for  pointer to the stream buffer*/
	if(ARDNSCL_NULL == pSrc)
	{
		/*retun FAILURE with appropriate error*/
		*pError =  ARDNSCL_ERROR_DNS_INVALID_MSG_BUFFER;
		return ARDNSCL_FAILURE;
	}

	/*Check for  length pointer*/
	if(ARDNSCL_NULL == pLength)
	{
		/*retun FAILURE with appropriate error*/
		*pError =  ARDNSCL_ERROR_DNS_INVALID_BUFFER_LENGTH;
		return ARDNSCL_FAILURE;
	}

	/*allocate memmory for request structure*/
    ARDNSCL_MEMGET(pResp , ARDNSCL_SIZEOF(arDnsClient_response_st));

	/*Check for memory allocation failure*/
	if(ARDNSCL_NULL == pResp )
	{
			*pError = ARDNSCL_ERROR_MEMORY_ALLOC_FAILURE; 
		return ARDNSCL_FAILURE;
	}

	/*Setting the value of length to zero*/
	len_val = ARDNSCL_INIT_VAL;

	/*unpack API ID to response structure*/
	arDnsClient_port_unpack_U8((dnscl_uint8_t*)&(pResp->api_id),
            (dnscl_uint8_t *)pSrc, &len_val);

	/*Increment the pSrc by size of length*/
	pSrc += len_val;

	/*Setting the value of length to zero*/
	len_val = ARDNSCL_INIT_VAL;

	/*unpacking payload size to response structure*/
	arDnsClient_port_unpack_U32((dnscl_uint8_t *)&(pResp->payload_size), 
            (dnscl_uint8_t *)pSrc, &len_val);

	/*Increment the pSrc by size of length*/
	pSrc += len_val;

	/*Setting the value of length to zero*/
	len_val = ARDNSCL_INIT_VAL;

	/*UnPacking the result */
	arDnsClient_port_unpack_U8((dnscl_uint8_t *)&(pResp->result), 
            (dnscl_uint8_t *)pSrc, &len_val);

	/*Increment the pDest by size of length*/
	pSrc += len_val;

	/*Setting the value of length to zero*/
	len_val = ARDNSCL_INIT_VAL;

    /*Unpack query data structure*/
	ret_val = arDnsClient_port_unpack_query(&pResp->pQuery, 
            (dnscl_uint8_t *)pSrc, &len_val, pError);

    /*check for return type of unpack query*/	
    if(ARDNSCL_FAILURE == ret_val)
	{
        arDnsClient_free_response(pGlob_data, pResp,&error);
		return ARDNSCL_FAILURE;
	}

	/*Increment the source buffer*/
	pSrc += len_val;

	/*Setting the value of length to zero*/
	len_val = ARDNSCL_INIT_VAL;

	if(ARDNSCL_SUCCESS == pResp->result)
    {
        /*unpack query response*/
        ret_val = arDnsClient_port_unpack_query_resp(&(pResp->pDnsQueryResp),
                pResp->pQuery->query_type,
                (dnscl_uint8_t *)pSrc,&len_val ,&error);

        /* check for return type of query response*/
        if(ARDNSCL_FAILURE == ret_val)
        {
                *pError = error;
            arDnsClient_free_response(pGlob_data, pResp,&error);
            return ARDNSCL_FAILURE;
        }
    }
	else
	{

		/*UnPacking the errror */
		arDnsClient_port_unpack_U8((dnscl_uint8_t *)&(pResp->ecode),
				(dnscl_uint8_t *)pSrc, &len_val);

	}

	/*Assigning value of payload size to */ 
	*pLength=pResp->payload_size;

	/*Assign address of local request structure 
		  to unpack request structure*/
	*ppResponse = pResp;

	/*return Success*/
	return ARDNSCL_SUCCESS;
}


