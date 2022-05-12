/******************************************************************************
*                                    
* FILE NAME    : arDnsClient_types.h                
*                                    
* DESCRIPTION  : Contains the definitions for the data types. 
*                                    
* Revision History :                            
*
*  DATE            NAME                 REFERENCE       REASON        
* ------         ------------           ---------     ----------
* 05-Feb-2009    Anuradha Gupta         None          Initial
* 06-Feb-2009    Anuradha Gupta         API Doc       Data type definitions
*
* Copyright 2009, Aricent.                       
*                                    
*******************************************************************************/

#ifndef __ARDNSCLIENT_TYPES_H__
#define __ARDNSCLIENT_TYPES_H__

#include "arDnsClient_defs.h"
#include "arDnsClient_port_types.h"

/* Prevent Name Mangling By C++ Compilers */

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

/****************************************************************************
 * Structure Name: arDnsClient_short_string_st
 * 
 * Description: This structure is for short string.
 *
 * Fields     : str_val - Value of the string
 *             :str_len - Length of the string, Max is 64.
 *******************************************************************************/
typedef struct 
{
	dnscl_uint8_t   	str_len;
	dnscl_uint8_t   	str_val[ARDNSCL_MAX_SHORT_STR_LEN];
} arDnsClient_short_string_st;

/****************************************************************************
 * Structure Name: arDnsClient_string_st
 * 
 * Description: This structure is for string.
 *
 * Fields     : str_val - Value of the string
 *             :str_len - Length of the string, Max is 128.
 *******************************************************************************/
typedef struct 
{
	dnscl_uint8_t   	str_len;
	dnscl_uint8_t   	str_val[ARDNSCL_MAX_STR_LEN];
} arDnsClient_string_st;

/****************************************************************************
 * Structure Name: arDnsClient_address_st
 * 
 * Description: This structure is for IP address.
 *
 * Fields     : ipAddress - formatted address string (with dotted notation)
 *            : port - the port number.
 *******************************************************************************/
typedef struct 
{
	arDnsClient_short_string_st   ipAddress;
	dnscl_uint16_t   		      port;
} arDnsClient_address_st;

/****************************************************************************
 * Structure Name: arDnsClient_conf_st
 * 
 * Description: This structure is for configuration data.
 *
 * Fields     : dns_server_addr - DNS Server address
 *
 *******************************************************************************/
typedef struct 
{
	arDnsClient_address_st 	  dns_server_addr;
} arDnsClient_conf_st;

/****************************************************************************
 * Structure Name: arDnsClient_client_data_st
 * 
 * Description: This structure is the global data for Client.
 *
 * Fields     : ipc_fd - the IPC fd
 *	      : dns_query_fd - dns query fd	 
 *            : pConfig_data - configuration data pointer
 *	      : estimated_query_time - estimated DNS query response time
 *            : pCache_db - cache database pointer
 *            : enable_trace - trace reporting flag
 *
 *******************************************************************************/
typedef struct 
{
    dnscl_int32_t            ipc_fd;
    dnscl_int32_t	         dns_query_fd;
    arDnsClient_conf_st      *pConfig_data;
    dnscl_void_t             *pCache_db; 
    dnscl_boolean_t           enable_trace;    
}arDnsClient_client_data_st;

/****************************************************************************
 * Structure Name: arDnsClient_clib_data_st
 * 
 * Description: This structure is the global data for CLIB.
 *
 * Fields     : ipc_fd - the IPC fd 
 *            : cache_counter - the counter that is updated on time ticks
 *            : enable_trace - trace reporting flag
 *
 *******************************************************************************/
typedef struct 
{
    dnscl_int32_t               ipc_fd;
    dnscl_uint32_t              cache_counter;
    dnscl_boolean_t             enable_trace;
}arDnsClient_clib_data_st;

/****************************************************************************
 * Structure Name: arDnsClient_global_data_st
 * 
 * Description: This structure is the global data for arDnsClient.
 *
 * Fields     : clib_addr - the receiving address of CLIB
 *            : dns_client_addr - the receiving address of client 
 *            : clib_data - the global data of CLIB
 *            : dns_client_data - the global data of client
 *            : pApp_port_data - Application specific porting data pointer
 *
 *******************************************************************************/
typedef struct 
{
     arDnsClient_address_st             clib_addr;
     arDnsClient_address_st             dns_client_addr;
     arDnsClient_clib_data_st           clib_data;
     arDnsClient_client_data_st         dns_client_data;
     dnscl_void_t                       *pApp_port_data;
}arDnsClient_global_data_st;


/****************************************************************************
 * Structure Name: arDnsClient_query_st
 * 
 * Description: This structure is the query data.
 *
 * Fields     : query_type - the query type 
 *            : query_string - the domain name to be resolved
 *            : pApp_query_ctx - Application specific query context
 *
 *******************************************************************************/
typedef struct 
{
     dnscl_query_t             query_type;
     arDnsClient_string_st     query_string;
     dnscl_void_t              *pApp_query_ctx;
} arDnsClient_query_st;



/****************************************************************************
 * Structure Name: arDnsClient_clear_cache_st
 * 
 * Description: This structure is the clear cache data.
 *
 * Fields     : clear_options - the option for clear cache 
 *            : domain_name - the domain name entry
 *            : query_type - the query type
 *
 *******************************************************************************/
typedef struct 
{
      dnscl_uint8_t             clear_options;
#define ARDNSCL_CLEAR_ALL_ENTRY     0x01
#define ARDNSCL_CLEAR_SINGLE_ENTRY  0x02
      arDnsClient_string_st      domain_name;
      dnscl_query_t              query_type;
}arDnsClient_clear_cache_st;

/****************************************************************************
 * Structure Name: arDnsClient_A_rr_st
 * 
 * Description: This structure is the resource data for A query.
 *
 * Fields     : addrIP4 - the IPv4 address string 
 *
 *******************************************************************************/
typedef struct 
{
	   arDnsClient_short_string_st      addrIP4;
} arDnsClient_A_rr_st;

/****************************************************************************
 * Structure Name: arDnsClient_AAAA_rr_st
 * 
 * Description: This structure is the resource data for AAAA query.
 *
 * Fields     : addrIP6 - the IPv6 address string 
 *
 *******************************************************************************/
typedef struct 
{
	   arDnsClient_short_string_st      addrIP6;
} arDnsClient_AAAA_rr_st;

/****************************************************************************
 * Structure Name: arDnsClient_srv_rr_st
 * 
 * Description: This structure is resource data for SRV query.
 *
 * Fields     : nameTarget - the FQDN of target host 
 *            : priority - the priority of target host
 *            : weight - the weight in case of same priority 
 *            : port - the port number
 *
 *******************************************************************************/
typedef struct 
{
	   arDnsClient_string_st  nameTarget;
	   dnscl_uint16_t         priority;
	   dnscl_uint16_t         weight;
       dnscl_uint16_t         port;
} arDnsClient_srv_rr_st;

/****************************************************************************
 * Structure Name: arDnsClient_naptr_rr_st
 * 
 * Description: This structure is resource data for NAPTR query.
 *
 * Fields     : order - the order of records 
 *            : preference - the preference in case of same order
 *            : flags - the flags for interpreting the other data in RR 
 *            : regularExpr - the string for regular expression
 *            : replacement - the domain name to be used for next query
 *
 *******************************************************************************/
typedef struct 
{
	dnscl_uint16_t   	            order;
	dnscl_uint16_t   	            preference;
	arDnsClient_short_string_st    	flags;
	arDnsClient_short_string_st    	service;
	arDnsClient_short_string_st    	regularExpr;
	arDnsClient_string_st    	    replacement;
} arDnsClient_naptr_rr_st;

/****************************************************************************
 * Structure Name: arDnsClient_rr_data_un
 * 
 * Description: This union for RR data.
 *
 * Fields     : naptr_rr_data - the NAPTR RR Data 
 *            : srv_rr_data - the SRV RR Data
 *            : A_rr_data - the A RR Data
 *            : AAAA_rr_data - the AAAA RR Data
 *
 *******************************************************************************/
typedef union 
{
        arDnsClient_naptr_rr_st  naptr_rr_data;
        arDnsClient_srv_rr_st    srv_rr_data;
        arDnsClient_A_rr_st      A_rr_data;
        arDnsClient_AAAA_rr_st   AAAA_rr_data;
} arDnsClient_rr_data_un;


/****************************************************************************
 * Structure Name: arDnsClient_dns_rr_list_st
 * 
 * Description: This list structure for RRs.
 *
 * Fields     : ttl - the time to live for RR 
 *            : query_rr_data - the data part of RRs
 *            : pNext - next pointer of list
 *
 *******************************************************************************/
typedef struct arDnsClient_dns_rr_list_s
{
  dnscl_uint32_t                      ttl;
  arDnsClient_rr_data_un              query_rr_data;
  struct arDnsClient_dns_rr_list_s   *pNext;
} arDnsClient_dns_rr_list_st;


/****************************************************************************
 * Structure Name: arDnsClient_query_resp_st
 * 
 * Description: This structure is the data for query response.
 *
 * Fields     : pQuery - the query
 *            : pAnswerRecords - the answer records
 *
 *******************************************************************************/
typedef struct 
{
      dnscl_uint8_t                     numRecords;
      arDnsClient_dns_rr_list_st       *pAnswerRecords;
}  arDnsClient_query_resp_st;

/****************************************************************************
 * Structure Name: arDnsClient_request_st
 * 
 * Description: This structure for Request APIs
 *
 * Fields     : api_id - the API ID 
 *            : payload_size - the size of the payload
 *            : enable_trace - trace reporting flag for Set Trace API
 *            : pConfData - the config info for Set Conf API
 *            : cache_counter - the cache counter for Send Query API
 *            : pClearCache - the clear cache info for Clear Cache API
 *
 *******************************************************************************/
typedef struct 
{
      dnscl_uint8_t                     api_id;
      dnscl_uint32_t                    payload_size;
      dnscl_boolean_t                   enableTrace;
      arDnsClient_conf_st               *pConfData;
      arDnsClient_query_st              *pDnsQuery;
      dnscl_uint32_t                    cache_counter;
      arDnsClient_clear_cache_st        *pClearCache;
} arDnsClient_request_st;

/****************************************************************************
 * Structure Name: arDnsClient_response_st
 * 
 * Description: This structure for Response APIs.
 *
 * Fields     : api_id - the API ID 
 *            : payload_size - the size of the payload
 *            : result - the result of the query
 *            : pDnsQueryResp - query response structure in case of result success
 *            : ecode - the error code in case of result failure
 *
 *******************************************************************************/
typedef struct 
{
 	dnscl_uint8_t                   api_id;
	dnscl_uint32_t                  payload_size;
	dnscl_result_t         		result;
    	arDnsClient_query_st            *pQuery;
	arDnsClient_query_resp_st       *pDnsQueryResp;
	dnscl_uint8_t                   ecode;
} arDnsClient_response_st;


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __ARDNSCLIENT_TYPES_H__ */



