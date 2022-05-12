/******************************************************************************
*                                    
* FILE NAME    : arDnsClient_queryh_cache.c                
*                                    
* DESCRIPTION  : Contains the arDnsClient Query Handler caching routines  
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

#include "arDnsClient_defs.h"
#include "arDnsClient_queryh_prototypes.h"

/*******************************************************************************
 * FUNCTION: arDnsClient_create_cache
 * 
 * Description: This function creates and initializes the Cache Database. 
 *              
 *
 * Return : Return ARDNSCL_SUCCESS if database created successfully else
 *          return ARDNSCL_FAILURE    
 *******************************************************************************/
dnscl_return_t  arDnsClient_create_cache(
arDnsClient_global_data_st 	*pGlobalData,
dnscl_error_t 			*pError)
{
	/* Cache databse pointer */
	arDnsClient_cache_db_st *pCache_db = ARDNSCL_NULL;
	
	/*Initialize Error variable */
	*pError = ARDNSCL_ERROR_START;

	ARDNSCL_PRINT(pGlobalData,("Enter: arDnsClient_create_cache\n"));	
		
	/* Allocate Memory for Cache Databse */
	ARDNSCL_MEMGET(pGlobalData->dns_client_data.pCache_db , ARDNSCL_SIZEOF(arDnsClient_cache_db_st));
	
	if(!pGlobalData->dns_client_data.pCache_db)
	{
		ARDNSCL_PRINT(pGlobalData,("No memory left for Cache Database\n"));
		*pError = ARDNSCL_ERROR_MEMORY_ALLOC_FAILURE;
		ARDNSCL_PRINT(pGlobalData,("Exit: arDnsClient_create_cache\n"));
		return ARDNSCL_FAILURE;
	}

	pCache_db = pGlobalData->dns_client_data.pCache_db;

	/* Initialize Cache Database elements */
	pCache_db->no_of_entries = ARDNSCL_INIT_VAL;
	pCache_db->p_cache_entries = ARDNSCL_NULL;

	ARDNSCL_PRINT(pGlobalData,("Exit: arDnsClient_create_cache\n"));
		
	return ARDNSCL_SUCCESS;
}
/*******************************************************************************
 * FUNCTION: arDnsClient_store_entry
 * 
 * Description: Stores a new entry in the Cache Databse. If Calclutaed TTL for 
 *		new entry is 0 then don't add the new entry to Cache. If there is
 *		space for no new entry then delete entry  with minimum TTL value 
 *		and store new enrty in place of old entry.	
 *
 * Return : Return ARDNSCL_SUCCESS if entry stored successfully else
 *          return ARDNSCL_FAILURE 
 *******************************************************************************/
dnscl_return_t  arDnsClient_store_entry(
	arDnsClient_string_st 		*pDomainName, 
	dnscl_uint8_t  				query_type, 
	arDnsClient_dns_rr_list_st  *pRecRecords,
	dnscl_uint8_t				no_of_records,
	arDnsClient_global_data_st 	*pGlobalData,
	dnscl_error_t 				*pError)
{
	/* Cache databse pointer */
	arDnsClient_cache_db_st *pCache_db = (arDnsClient_cache_db_st *)
				(pGlobalData->dns_client_data.pCache_db);

	/* Variable for traversing the Cache Database */
	arDnsClient_cache_entry_st *p_temp_entry = ARDNSCL_NULL;
		
	/* Variable for traversing the rr_list */
	arDnsClient_dns_rr_list_st *p_temp_rr_list = ARDNSCL_NULL;
		
	/* Variable for storing the minimum TTL value */
	dnscl_uint32_t    min_ttl = ARDNSCL_INIT_VAL;

	/* Allocate Memory for Cache Entry */
	arDnsClient_cache_entry_st *p_new_entry = ARDNSCL_NULL;

	/*Initialize Error variable */
	*pError = ARDNSCL_ERROR_START;
		
	ARDNSCL_PRINT(pGlobalData,("Enter: arDnsClient_store_entry\n"));	

#if defined(ARDNSCL_PORT_WINDOWS)
	/* No need to store non NAPTR resource records in Windows*/
	if(ARDNSCL_QUERY_TYPE_NAPTR != query_type)
	{
		ARDNSCL_PRINT(pGlobalData,("Exit: arDnsClient_store_entry\n"));

		return ARDNSCL_FAILURE;
	}
#endif

    /* calculate the Minimum TTL value */
    p_temp_rr_list = pRecRecords; 

	min_ttl = p_temp_rr_list->ttl;
	while(ARDNSCL_NULL != p_temp_rr_list)
	{
		if(p_temp_rr_list->ttl < (dnscl_uint32_t)min_ttl)
			min_ttl = p_temp_rr_list->ttl;
		p_temp_rr_list = p_temp_rr_list->pNext;
	}

	if(ARDNSCL_INIT_VAL == min_ttl)
	{
		ARDNSCL_PRINT(pGlobalData,("TTL 0 so the entry need not be stored\n"));
		ARDNSCL_PRINT(pGlobalData,("Exit: arDnsClient_store_entry\n"));
		return ARDNSCL_FAILURE;
	}		

	ARDNSCL_MEMGET(p_new_entry, ARDNSCL_SIZEOF(arDnsClient_cache_entry_st))

	if(!p_new_entry)
	{
		ARDNSCL_PRINT(pGlobalData,("No memory left creating new entry\n"));
		ARDNSCL_PRINT(pGlobalData,("Exit: arDnsClient_store_entry\n"));
		*pError = ARDNSCL_ERROR_MEMORY_ALLOC_FAILURE;
		return ARDNSCL_FAILURE;
	}

	/* Store records in the new cache entry */
	ARDNSCL_STRCPY((dnscl_int8_t*)p_new_entry->domain_name.str_val,(const dnscl_int8_t*)pDomainName->str_val);
	p_new_entry->domain_name.str_len = pDomainName->str_len;	
	p_new_entry->query_type = query_type;
	p_new_entry->p_rr_data = pRecRecords;
	p_new_entry->no_of_rr = ARDNSCL_INIT_VAL;	
	p_new_entry->no_of_rr = no_of_records;
	
	/* Store the minimum TTL as the TTL for this entry */	
	p_new_entry->ttl = min_ttl;
	
	/* Check for number of entries to know weather a previous entry needs to be deleted */
	if(ARDNSCL_MAX_CACHE_RECORDS > pCache_db->no_of_entries)
	{
		/* If there is no entry in the Cache database then store this as first entry */
		if(ARDNSCL_INIT_VAL == pCache_db->no_of_entries)
		{
			pCache_db->p_cache_entries = p_new_entry;
			p_new_entry->p_next_entry = ARDNSCL_NULL;
			pCache_db->no_of_entries++;
			ARDNSCL_PRINT(pGlobalData,("Exit: arDnsClient_store_entry\n"));
			return ARDNSCL_SUCCESS;
		}
			
		p_temp_entry = pCache_db->p_cache_entries;
	
		/* Move to the end of list of entries */
		while(ARDNSCL_NULL != p_temp_entry->p_next_entry )
		{
			p_temp_entry = p_temp_entry->p_next_entry;
		}
			
		/* Store the new entry at the end of all the entries */
		p_temp_entry->p_next_entry = p_new_entry;
		p_new_entry->p_next_entry = ARDNSCL_NULL;
		pCache_db->no_of_entries++;
	
		ARDNSCL_PRINT(pGlobalData,("Exit: arDnsClient_store_entry\n"));
		
		return ARDNSCL_SUCCESS;
	}
	else
	{
		/* Vaiable for storing entry which is to be deleted */
		arDnsClient_cache_entry_st *p_tbd = ARDNSCL_NULL;

		/* Variable for storing entry previous to current entry */
		arDnsClient_cache_entry_st *p_prev = ARDNSCL_NULL;

		/* Variable for storing previous of entry to be deleted */
		arDnsClient_cache_entry_st *p_tbd_prev = ARDNSCL_NULL;
		
		/* Initialize p_temp_entry to traverse the Cache Databse f
         * or minimum TTL value */
		p_temp_entry = pCache_db->p_cache_entries;
		min_ttl = p_temp_entry->ttl;
		p_tbd = p_temp_entry;
		
		while(ARDNSCL_NULL != p_temp_entry)
		{
			/* Check for minimum TTL Value */
			if(p_temp_entry->ttl < min_ttl)
			{
				min_ttl = p_temp_entry->ttl;
				p_tbd = p_temp_entry;
				p_tbd_prev = p_prev;
			}
			p_prev = p_temp_entry;
			p_temp_entry = p_temp_entry->p_next_entry;
		}
		arDnsClient_free_entry(&pCache_db->p_cache_entries,p_tbd_prev,p_tbd);

        if(ARDNSCL_NULL != p_tbd_prev)
		{
			/* Put next of new entry to next of deleted entry */
			p_new_entry->p_next_entry = p_tbd_prev->p_next_entry;

			/* Put new entry in the linked list in place of old entry */
			p_tbd_prev->p_next_entry = p_new_entry;
		}
        else
        {
            /* Put next of new entry to next of deleted entry */
			p_new_entry->p_next_entry = pCache_db->p_cache_entries->p_next_entry;

			/* Put new entry in the linked list in place of old entry */
			pCache_db->p_cache_entries->p_next_entry = p_new_entry;
        }
		ARDNSCL_PRINT(pGlobalData,("Exit: arDnsClient_store_entry\n"));

		return ARDNSCL_SUCCESS;
	}
}

/*******************************************************************************
 * FUNCTION: arDnsClient_adjustTTL
 * 
 * Description: Adjust the TTL of all the entries and remove the entries whose
 *              TTL is 0 or less after adjustment
 *
 * Return : return ARDNSCL_SUCCESS after all the TTLs are adjusted
 * 
*******************************************************************************/
dnscl_return_t arDnsClient_adjustTTL(
	arDnsClient_global_data_st 	*pGlobalData,
	dnscl_uint32_t          	cache_counter,
	dnscl_error_t 			*pError)
{
	/* Cache databse pointer */
	arDnsClient_cache_db_st *pCache_db = (arDnsClient_cache_db_st *)
				(pGlobalData->dns_client_data.pCache_db);
		
	/* Vaiable for storing entry whose next entry is to be deleted */
	arDnsClient_cache_entry_st *p_previous = ARDNSCL_NULL; 
		
	/* Variable for traversing the Cache Database */
	arDnsClient_cache_entry_st *p_temp_entry = ARDNSCL_NULL;
	
	/*Initialize Error variable */
	*pError = ARDNSCL_ERROR_START;
		
	ARDNSCL_PRINT(pGlobalData,("Entry: arDnsClient_adjustTTL\n"));

	if(ARDNSCL_INIT_VAL == pCache_db->no_of_entries)
	{			
		ARDNSCL_PRINT(pGlobalData,("Cache Empty\n"));
		ARDNSCL_PRINT(pGlobalData,("Exit: arDnsClient_adjustTTL\n"));
		return ARDNSCL_SUCCESS;
	}
	else
	{			
		/* Initialize p_temp_entry to traverse the Cache Database for minimum TTL value */
		p_previous = pCache_db->p_cache_entries;
		p_temp_entry = p_previous;

		/* Subtract cache_counter from TTL and delete Entry if TTL <= 0 */
		while(ARDNSCL_NULL != p_temp_entry)
		{
			/* if TTL is less than 0 then delete the entry */
			if(p_temp_entry->ttl <= cache_counter)
			{
				/* node to be deleted is the first entry in Cache */
				if(pCache_db->p_cache_entries == p_temp_entry)
				{
					arDnsClient_free_rr_list(p_temp_entry->p_rr_data);
					p_previous = p_temp_entry->p_next_entry;
					ARDNSCL_MEMFREE(p_temp_entry);
					pCache_db->no_of_entries--;
					p_temp_entry = p_previous;
					pCache_db->p_cache_entries = p_previous;
				}
				else
				{
					arDnsClient_free_entry(&pCache_db->p_cache_entries,p_previous,p_temp_entry);
					pCache_db->no_of_entries--;
					/* Point p_temp_entry to next entry */
					p_temp_entry = p_previous->p_next_entry;
				}
			}
			else
			{
				p_temp_entry->ttl = p_temp_entry->ttl - cache_counter;
				p_previous=p_temp_entry;
				p_temp_entry=p_temp_entry->p_next_entry;
			}
		}
	}
		
	ARDNSCL_PRINT(pGlobalData,("Exit: arDnsClient_adjustTTL\n"));
	return ARDNSCL_SUCCESS;
}
/*******************************************************************************
 * FUNCTION: arDnsClient_search_entry
 * 
 * Description: Serches for an entry in the Cache database based on domain name
 *		query type. If entry is not found then ARDNSCL_FAILURE is 
 *		returned 
 *
 * Return : return ARDNSCL_FAILURE if entry is not found else return
 *	    ARDNSCL_SUCCESS  
 * 
*******************************************************************************/
dnscl_return_t arDnsClient_search_entry(
	arDnsClient_global_data_st 	*pGlobalData,
	arDnsClient_string_st 		*pDomainName,
	dnscl_uint8_t 			     query_type,
	dnscl_uint8_t			    *pNumRecords,
	arDnsClient_dns_rr_list_st 	**pAnswerRecords,
	dnscl_error_t 			    *pError)
{
	/* Cache databse pointer */
	arDnsClient_cache_db_st *pCache_db = (arDnsClient_cache_db_st *)
				(pGlobalData->dns_client_data.pCache_db);
	
	/* Variable for traversing the Cache Database */
	arDnsClient_cache_entry_st	*p_temp_entry = ARDNSCL_NULL;

	/* Initialize p_temp_entry to traverse the Cache Database */
	p_temp_entry =	pCache_db->p_cache_entries;
	
	/*Initialize Error variable */
	*pError = ARDNSCL_ERROR_START;
	
	ARDNSCL_PRINT(pGlobalData,("Entry: arDnsClient_search_entry\n"));

#if defined(ARDNSCL_PORT_WINDOWS)
	/* Non NAPTR resource records in Windows are not stored */
	if(ARDNSCL_QUERY_TYPE_NAPTR != query_type)
	{
		return ARDNSCL_FAILURE;
	}
#endif

	while(ARDNSCL_NULL != p_temp_entry)
	{
		/* Check weather both the Daomin Name 
		and query type	matches with the entry */
		if((ARDNSCL_INIT_VAL == ARDNSCL_STRCMP((dnscl_int8_t *)
					p_temp_entry->domain_name.str_val,
					(dnscl_int8_t *)pDomainName->str_val))
					&&(p_temp_entry->query_type == query_type))
		{
			*pAnswerRecords = p_temp_entry->p_rr_data;
			*pNumRecords = p_temp_entry->no_of_rr;

			ARDNSCL_PRINT(pGlobalData,("Exit: arDnsClient_search_entry\n"));

			return ARDNSCL_SUCCESS;
		}
		p_temp_entry=p_temp_entry->p_next_entry;
	}

	ARDNSCL_PRINT(pGlobalData,("Entry not found\n"));	
	ARDNSCL_PRINT(pGlobalData,("Exit: arDnsClient_search_entry\n"));
			
	/* return Failure in Finding and Cached record for this query */
	return ARDNSCL_FAILURE; 
}
/*******************************************************************************
 * FUNCTION: arDnsClient_remove_entry
 * 
 * Description: removes an entry from Cache database based on Damin name and 
 *		query type 
 *
 * Return : return ARDNSCL_SUCCESS when entry is removed 
 * 
*******************************************************************************/
dnscl_return_t arDnsClient_remove_entry (
	arDnsClient_string_st		*pDomainName,
	dnscl_uint8_t           	query_type,
	arDnsClient_global_data_st 	*pGlobalData,
	dnscl_error_t 			*pError)
{
	/* Cache databse pointer */
	arDnsClient_cache_db_st *pCache_db = (arDnsClient_cache_db_st *)
				(pGlobalData->dns_client_data.pCache_db);

	/* Variable for traversing the Cache Database */
	arDnsClient_cache_entry_st	*p_temp_entry = ARDNSCL_NULL;
	
	/* Vaiable for storing entry whose next entry is to be deleted */
	arDnsClient_cache_entry_st *p_delete_next = ARDNSCL_NULL;

	/*Initialize Error variable */
	*pError = ARDNSCL_ERROR_START;
	
	p_temp_entry	= pCache_db->p_cache_entries;
	/* Initialize p_temp_entry to traverse the Cache Database */

	ARDNSCL_PRINT(pGlobalData,("Entry: arDnsClient_remove_entry\n"));

	if(ARDNSCL_INIT_VAL == pCache_db->no_of_entries)
	{
		ARDNSCL_PRINT(pGlobalData,("Cache Empty\n"));
		return ARDNSCL_SUCCESS;
	}

	p_delete_next = ARDNSCL_NULL;

	while(ARDNSCL_NULL != p_temp_entry)
	{
		/* Chech weather both the Daomin Name and Quary type matches with the entry */
		if((ARDNSCL_INIT_VAL == ARDNSCL_STRCMP((dnscl_int8_t *)p_temp_entry->domain_name.str_val,
				(dnscl_int8_t *)pDomainName->str_val))
				&&(p_temp_entry->query_type == query_type))
		{
			arDnsClient_free_entry(&pCache_db->p_cache_entries,p_delete_next,p_temp_entry);
			pCache_db->no_of_entries--;
			ARDNSCL_PRINT(pGlobalData,("Entry deleted from cache\n"));
			ARDNSCL_PRINT(pGlobalData,("Exit: arDnsClient_remove_entry\n"));
						
			return ARDNSCL_SUCCESS;
		}
		p_delete_next = p_temp_entry;
		p_temp_entry=p_temp_entry->p_next_entry;
	}
	ARDNSCL_PRINT(pGlobalData,("Entry not found in cache\n"));
	ARDNSCL_PRINT(pGlobalData,("Exit: arDnsClient_remove_entry\n"));
	return ARDNSCL_SUCCESS; 
}
/*******************************************************************************
 * FUNCTION: arDnsClient_clear_cache
 * 
 * Description: Removes all entries from Cache database 
 *
 * Return : return ARDNSCL_SUCCESS when entry is removed 
 * 
*******************************************************************************/
dnscl_return_t arDnsClient_cache_clear (
 arDnsClient_global_data_st 	*pGlobalData,
 dnscl_error_t 			*pError)
{
	/* Cache databse pointer */
	arDnsClient_cache_db_st *pCache_db = (arDnsClient_cache_db_st *)
				(pGlobalData->dns_client_data.pCache_db);

	/* Variable for traversing the Cache Database */
	arDnsClient_cache_entry_st *p_temp_entry =ARDNSCL_NULL ;
	/* Pointer to entry to be deleted */
	arDnsClient_cache_entry_st *p_delete_entry = ARDNSCL_NULL;
	
	/*Initialize Error variable */
	*pError = ARDNSCL_ERROR_START;
		
	p_temp_entry = pCache_db->p_cache_entries;

	ARDNSCL_PRINT(pGlobalData,("Entry: arDnsClient_clear_cache\n"));

	while(ARDNSCL_NULL != p_temp_entry)
	{
		p_delete_entry = p_temp_entry;
		p_temp_entry = p_temp_entry->p_next_entry;
		/* Free rr_list of deleted entry */
		arDnsClient_free_rr_list(p_delete_entry->p_rr_data); 
		ARDNSCL_MEMFREE(p_delete_entry);
	}
	/* Set Cache entries to NULL */
	pCache_db->p_cache_entries = ARDNSCL_NULL; 
	/* Set number of entries to 0 */
	pCache_db->no_of_entries = ARDNSCL_INIT_VAL; 
	
	ARDNSCL_PRINT(pGlobalData,("Cache cleared successfully\n"));
	ARDNSCL_PRINT(pGlobalData,("Exit: arDnsClient_clear_cache\n"));

	return ARDNSCL_SUCCESS;
}

/*******************************************************************************
 * FUNCTION: arDnsClient_free_entry
 * 
 * Description:This function an entry from the database. 
 *
 * Return : Return ARDNSCL_SUCCESS if all the records were deleted 
 *******************************************************************************/
dnscl_return_t arDnsClient_free_entry(arDnsClient_cache_entry_st **pp_cache, 
					arDnsClient_cache_entry_st *pPrev, 
					arDnsClient_cache_entry_st *pDel)
{
	/* Free rr_list */
	arDnsClient_free_rr_list(pDel->p_rr_data);
	/* Check if node to be deleted is the first node */
	if(ARDNSCL_NULL == pPrev)
		*pp_cache = pDel->p_next_entry;
	else
	{
		/* Point next of previous enrty to next of deleted entry */
		pPrev->p_next_entry = pDel->p_next_entry;
	}
	/* Release deleted Entry memory */
	ARDNSCL_MEMFREE(pDel);
	
	return ARDNSCL_SUCCESS;
}

