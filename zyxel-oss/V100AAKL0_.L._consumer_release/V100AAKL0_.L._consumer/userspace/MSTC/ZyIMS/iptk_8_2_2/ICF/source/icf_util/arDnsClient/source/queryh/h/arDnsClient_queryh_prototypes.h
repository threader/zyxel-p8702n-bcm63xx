/******************************************************************************
*                                    
* FILE NAME    : arDnsClient_queryh_prototypes.h                
*                                    
* DESCRIPTION  : Contains the definitions for the interfaces. 
*                                    
* Revision History :                            
*
*  DATE            NAME                 REFERENCE       REASON        
* ------         ------------           ---------     ----------
* 05-Feb-2009    Anuradha Gupta           None          Initial
* 06-Feb-2009    Shweta Jain              LLD           arDnsClient 1.0  
*                                                                
* Copyright 2009, Aricent.                       
*                                    
*******************************************************************************/

#ifndef __ARDNSCLIENT_QUERYH_PROTOTYPES_H__
#define __ARDNSCLIENT_QUERYH_PROTOTYPES_H__

#include "arDnsClient_port_types.h" 
#include "arDnsClient_types.h"
#include "arDnsClient_queryh_types.h"

/* Prevent Name Mangling By C++ Compilers */

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */
		
/*******************************************************************************
 * FUNCTION: arDnsClient_create_cache
 * 
 * Description: This function creates and initializes the Cache Database. 
 * INPUT       :pCacheInit - Initialization parameters(not supported in this 
 *		release 
 * OUTPUT      :pGlobalData - pointer to global data
 *
 * Return : Return ARDNSCLIENT_SUCCESS if database created successfully else
 *          return ARDNSCLIENT_FAILURE    
 *******************************************************************************/
dnscl_return_t  arDnsClient_create_cache(
arDnsClient_global_data_st *pGlobalData,
dnscl_error_t 		   *pError);

/*******************************************************************************
 * FUNCTION: arDnsClient_store_entry
 * 
 * Description: Stores a new entry in the Cache Databse. If Calclutaed TTL for 
 *		new entry is 0 then don't add the new entry to Cache. If there is
 *		space for no new entry then delete entry  with minimum TTL value 
 *		and store new enrty in place of old entry.
 * INPUT       :domain_name - domain name of the query
 *		query_type - query type
 * 		pRecRecords - List of records for this query
 *		pGlobalData - pointer to global data
 *
 * Return : Return ARDNSCLIENT_SUCCESS if entry stored successfully else
 *          return ARDNSCLIENT_FAILURE 
 *******************************************************************************/
dnscl_return_t  arDnsClient_store_entry(
arDnsClient_string_st 	*pDomainName,
dnscl_uint8_t  		query_type, 
arDnsClient_dns_rr_list_st *pRecRecords,
dnscl_uint8_t		   no_of_records,
arDnsClient_global_data_st *pGlobalData,
dnscl_error_t 		   *pError);

/*******************************************************************************
 * FUNCTION: arDnsClient_adjustTTL
 * 
 * Description: Adjust the TTL of all the entries and remove the entries whose
 *              TTL is 0 or less after adjustment
 *
 * INPUT       	:pGlobalData - pointer to global data
 *		 cache_counter - cache counter to adjust TTL values 
 *
 * Return : return ARDNSCLIENT_SUCCESS after all the TTLs are adjusted
 * 
*******************************************************************************/
dnscl_return_t arDnsClient_adjustTTL(
arDnsClient_global_data_st *pGlobalData,
dnscl_uint32_t             cache_counter,
dnscl_error_t 		   *pError);

/*******************************************************************************
 * FUNCTION: arDnsClient_search_entry
 * 
 * Description: Serches for an entry in the Cache database based on domain name
 *		query type. If entry is not found then ARDNSCLIENT_FAILURE is 
 *		returned
 *  
 * INPUT       	:domain_name - domian name of query to be searched
 *		 query_type - query type of query to be searched
 * 		 pGlobalData - pointer to global data
 * OUTPIT	:pNumRecords - Number of records
 *		 pAnswerRecords - List of answer resource records
 *
 * Return : return ARDNSCLIENT_FAILURE if entry is not found else return
 *	    ARDNSCLIENT_SUCCESS  
 * 
*******************************************************************************/
dnscl_return_t arDnsClient_search_entry(
	arDnsClient_global_data_st 	*pGlobalData,
	arDnsClient_string_st 		*pDomainName,
	dnscl_uint8_t 			     query_type,
	dnscl_uint8_t			    *pNumRecords,
	arDnsClient_dns_rr_list_st 	**pAnswerRecords,
	dnscl_error_t 			    *pError);

/*******************************************************************************
 * FUNCTION: arDnsClient_remove_entry
 * 
 * Description: removes an entry from Cache database based on Damin name and 
 *		query type 
 * INPUT       	:domain_name - domian name of query to be removed 
 *		 query_type - query type of query to be removed
 * 		 pGlobalData - pointer to global data
 * Return : return ARDNSCLIENT_SUCCESS when entry is removed 
 * 
*******************************************************************************/
dnscl_return_t arDnsClient_remove_entry (
arDnsClient_string_st	*pDomainName,
dnscl_uint8_t           query_type,
arDnsClient_global_data_st 	*pGlobalData,
dnscl_error_t 		   	*pError);

/*******************************************************************************
 * FUNCTION: arDnsClient_clear_cache
 * 
 * Description: Removes all entries from Cache database 
 *
 * INPUT       	:pGlobalData - pointer to global data
 *
 * Return : return ARDNSCLIENT_SUCCESS when entry is removed 
 * 
*******************************************************************************/
dnscl_return_t arDnsClient_cache_clear (
arDnsClient_global_data_st 	*pGlobalData,
dnscl_error_t 		   	*pError);

/*******************************************************************************
 * FUNCTION: arDnsClient_free_rr_list
 * 
 * Description:This function deletes all the resource records from the rr_list 	
 *
 * Return : Return ARDNSCL_SUCCESS if all the records were deleted 
 *******************************************************************************/
dnscl_return_t arDnsClient_free_rr_list(
arDnsClient_dns_rr_list_st 	*dns_rr_list);

/*******************************************************************************
 * FUNCTION: arDnsClient_free_entry
 * 
 * Description:This function an entry from the database. 
 *
 * Return : Return ARDNSCL_SUCCESS if all the records were deleted 
 *******************************************************************************/
dnscl_return_t arDnsClient_free_entry(
arDnsClient_cache_entry_st  **pp_cache,
arDnsClient_cache_entry_st 	*pPrev, 
arDnsClient_cache_entry_st 	*pDel);

/*******************************************************************************
 * FUNCTION: arDnsClient_handle_send_query
 * 
 * Description: This function handles send query requests 
 *
 * Return : Return ARDNSCL_SUCCESS if all the records were deleted
 *******************************************************************************/
dnscl_return_t arDnsClient_handle_send_query(
arDnsClient_global_data_st 	*pGlobalData,
arDnsClient_request_st 		*pRequest,
dnscl_error_t 		 	*pError);

#if defined (ARDNSCL_PORT_LINUX) || defined (ARDNSCL_PORT_VXWORKS)
/*****************************************************************************
 * FUNCTION :   arDnsClient_parse_dns_header 
 *
 * Description: This function is used to parse DNS header.  
 *                            
 * Return:      It will return DNSCL_SUCCESS if successful else return DNSCL_FAILURE.
 ********************************************************************************/
 
 dnscl_return_t    arDnsClient_parse_dns_header  (
                                 arDnsClient_global_data_st *pDnscl_glb_data,
                                 dnscl_buff_ptr             pOrg_buff, 
                                 arDnsClient_hdr_st         *pHeader, 
                                 dnscl_error_t              *pError);

 /*****************************************************************************
 * FUNCTION : arDnsClient_parse_A_query
 *
 * Description: This function is used to populate the A query structure.
 *                            
 * Return:      It will return DNSCL_SUCCESS if successful else return DNSCL_FAILURE.
 ********************************************************************************/

 dnscl_return_t   arDnsClient_parse_A_query (
                           arDnsClient_global_data_st   *pDnscl_glb_data, 
                           dnscl_buff_ptr               pCurr_buff, 
                           arDnsClient_rr_data_un       *rec,
                           dnscl_uint32_t               *nBytes_qr,
                           dnscl_error_t                *pError);


/*****************************************************************************
 * FUNCTION : arDnsClient_parse_AAAA_query
 *
 * Description: This function is used to populate the AAAA query structure.
 *                            
 * Return:      It will return DNSCL_SUCCESS if successful else return DNSCL_FAILURE.
 ********************************************************************************/

 dnscl_return_t   arDnsClient_parse_AAAA_query (
                           arDnsClient_global_data_st  *pDnscl_glb_data,
                           dnscl_buff_ptr               pCurr_buff, 
                           arDnsClient_rr_data_un       *rec,
                           dnscl_uint32_t               *nBytes_qr,
                           dnscl_error_t                *pError);


/*****************************************************************************
 * FUNCTION : arDnsClient_parse_Srv_query
 *
 * Description: This function is used to populate the SRV query structure.
 *                            
 * Return:      It will return DNSCL_SUCCESS if successful else return DNSCL_FAILURE.
 ********************************************************************************/

 dnscl_return_t    arDnsClient_parse_Srv_query  (
                             arDnsClient_global_data_st   *pDnscl_glb_data,
                             dnscl_buff_ptr               pOrg_buff, 
                             dnscl_uint32_t               org_buff_len,
                             dnscl_buff_ptr               pCurr_buff,
                             arDnsClient_rr_data_un       *rec,
                             dnscl_uint32_t               *nBytes_qr,
                             dnscl_error_t                *pError);


/*****************************************************************************
 * FUNCTION : arDnsClient_parse_Naptr_query
 *
 * Description: This function is used to populate the NAPTR query structure.
 *                            
 * Return:      It will return DNSCL_SUCCESS if successful else return DNSCL_FAILURE.
 ********************************************************************************/

 dnscl_return_t    arDnsClient_parse_Naptr_query  (
                             arDnsClient_global_data_st   *pDnscl_glb_data,
                             dnscl_buff_ptr               pOrg_buff, 
                             dnscl_uint32_t               org_buff_len,
                             dnscl_buff_ptr               pCurr_buff,
                             arDnsClient_rr_data_un       *rec,
                             dnscl_uint32_t               *nBytes_qr,
                             dnscl_error_t                *pError);

/*****************************************************************************
 * FUNCTION : arDnsClient_parse_dns_qsection
 *
 * Description: This function is used to parse the question section of DNS response.
 *                            
 * Return:      It will return DNSCL_SUCCESS if successful else return DNSCL_FAILURE.
 ********************************************************************************/

 dnscl_return_t   arDnsClient_parse_dns_qsection  (
                           arDnsClient_global_data_st   *pDnscl_glb_data,
                           dnscl_buff_ptr               pOrg_buff, 
                           dnscl_uint32_t               org_buff_len,
                           dnscl_buff_ptr               pCurr_buff,
                           dnscl_uint16_t               q_count,
                           dnscl_uint32_t               *nBytes_qn,  
                           dnscl_error_t                *pError);

/*****************************************************************************
 * FUNCTION : arDnsClient_decode_U16
 *
 * Description: This function is used to decode 2 byte data.
 *                            
 * Return:      Nothing.
 ********************************************************************************/

 dnscl_void_t    arDnsClient_decode_U16  (
                           dnscl_uint8_t          **pSrc,
                           dnscl_uint16_t          *pOutput);


/*****************************************************************************
 * FUNCTION : arDnsClient_decode_U32
 *
 * Description: This function is used to decode 4 byte data.
 *                            
 * Return:      Nothing.
 ********************************************************************************/

 dnscl_void_t    arDnsClient_decode_U32  (
                           dnscl_uint8_t          **pSrc,
                           dnscl_uint32_t          *pOutput);
#endif


/*****************************************************************************
 * FUNCTION :   arDnsClient_parse_A_dns_response 
 *
 * Description: This function is used to parse the DNS Response having 
 *              A type of query.  
 *                            
 * Return:      It will return DNSCL_SUCCESS if successful else return DNSCL_FAILURE.
 ********************************************************************************/

dnscl_return_t    arDnsClient_parse_A_dns_response  (
        arDnsClient_global_data_st      *pDnscl_glb_data,
        dnscl_buff_ptr                  pOrg_buff, 
        dnscl_uint32_t                  org_buff_len,
        arDnsClient_dns_rr_list_st      **head,
        dnscl_uint8_t                  *nanswer_rec,
        dnscl_error_t                   *pError);



/*****************************************************************************
 * FUNCTION :   arDnsClient_parse_AAAA_dns_response 
 *
 * Description: This function is used to parse the DNS Response having 
 *              AAAA type of query.  
 *                            
 * Return:      It will return DNSCL_SUCCESS if successful else return DNSCL_FAILURE.
 ********************************************************************************/
 dnscl_return_t   arDnsClient_parse_AAAA_dns_response (
                                  arDnsClient_global_data_st     *pDnscl_glb_data,
                                  dnscl_buff_ptr                 pOrg_buff, 
                                  dnscl_uint32_t                 org_buff_len,
                                  arDnsClient_dns_rr_list_st     **head,
		                  dnscl_uint8_t                 *nanswer_rec,
                                  dnscl_error_t                  *pError);


/*****************************************************************************
 * FUNCTION :   arDnsClient_parse_SRV_dns_response 
 *
 * Description: This function is used to parse the DNS Response having 
 *              SRV type of query.  
 *                            
 * Return:      It will return DNSCL_SUCCESS if successful else return DNSCL_FAILURE.
 ********************************************************************************/

 dnscl_return_t   arDnsClient_parse_SRV_dns_response (
                              arDnsClient_global_data_st      *pDnscl_glb_data,
                              dnscl_buff_ptr                  pOrg_buff, 
                              dnscl_uint32_t                  org_buff_len,
                              arDnsClient_dns_rr_list_st      **head,
                              dnscl_uint8_t                  *nanswer_rec,
                              dnscl_error_t                   *pError);


/*****************************************************************************
 * FUNCTION :   arDnsClient_parse_NAPTR_dns_response 
 *
 * Description: This function is used to parse the DNS Response having 
 *              NAPTR type of query.  
 *                            
 * Return:      It will return DNSCL_SUCCESS if successful else return DNSCL_FAILURE.
 ********************************************************************************/

dnscl_return_t  arDnsClient_parse_NAPTR_dns_response (
                            arDnsClient_global_data_st     *pDnscl_glb_data,
                            dnscl_buff_ptr                 pOrg_buff, 
                            dnscl_uint32_t                 org_buff_len,
                            arDnsClient_dns_rr_list_st     **head,
                            dnscl_uint8_t                 *nanswer_rec,
                            dnscl_error_t                  *pError);




#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __ARDNSCLIENT_QUERYH_PROTOTYPES_H__ */



