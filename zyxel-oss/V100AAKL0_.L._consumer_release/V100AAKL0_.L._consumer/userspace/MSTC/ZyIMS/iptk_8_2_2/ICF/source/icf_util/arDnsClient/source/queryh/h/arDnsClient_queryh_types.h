/******************************************************************************
*                                    
* FILE NAME    : arDnsClient_queryh_types.h                
*                                    
* DESCRIPTION  : Contains the definitions for the data types. 
*                                    
* Revision History :                            
*
*  DATE            NAME                 REFERENCE       REASON        
* ------         ------------           ---------     ----------
* 05-Feb-2009    Anuradha Gupta         None          Initial
* 09-Feb-2009    Shweta Jain            LLD           arDnsClient 1.0  :w!
*
* Copyright 2009, Aricent.                       
*                                    
*******************************************************************************/

#ifndef __ARDNSCLIENT_QUERYH_TYPES_H__
#define __ARDNSCLIENT_QUERYH_TYPES_H__

#include "arDnsClient_types.h"

/* Prevent Name Mangling By C++ Compilers */

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

/****************************************************************************
 * Structure Name: arDnsClient_cache_entry_st
 * 
 * Description: This structure is for storing the cache Entry 
 *
 * Fields     :domain_name - domain name of Cache Entry
 *            :query_type - Type of DNS query
 *	      :ttl - TTL for this entry
 *     	      :no_of_rr - number of resorce records in this entry
 *	      :p_rr_data - Pointer to resource records
 *	      :p_next_entry - Pointer to next entry
 *******************************************************************************/
typedef struct arDnsClient_cache_entry_s
{
		arDnsClient_string_st	domain_name;
		dnscl_uint8_t		query_type;
		dnscl_uint32_t		ttl;
		dnscl_uint8_t		no_of_rr;
		arDnsClient_dns_rr_list_st *p_rr_data;
		struct arDnsClient_cache_entry_s *p_next_entry;
}arDnsClient_cache_entry_st;

/****************************************************************************
 * Structure Name: arDnsClient_cache_db_st
 * 
 * Description: This structure is for storing the cache database 
 *
 * Fields     :no_of_entries - Number of Cache entries
 *            :p_cache_entries - lLink top Cache Entries
 *******************************************************************************/
typedef struct
{
		dnscl_uint16_t			no_of_entries;
		arDnsClient_cache_entry_st 	*p_cache_entries;
}arDnsClient_cache_db_st;

/* Structure Declaration of DNS Header */  
typedef struct {
	dnscl_uint16_t id :16;
	dnscl_uint16_t qr: 1;
	dnscl_uint16_t opcode: 4;
	dnscl_uint16_t aa: 1;
	dnscl_uint16_t tc: 1;
	dnscl_uint16_t rd: 1;
	dnscl_uint16_t ra: 1;
	dnscl_uint16_t unused :1;
	dnscl_uint16_t ad: 1;
	dnscl_uint16_t cd: 1;
	dnscl_uint16_t rcode :4;
	dnscl_uint16_t qdcount :16;
	dnscl_uint16_t ancount :16;
	dnscl_uint16_t nscount :16;
	dnscl_uint16_t arcount :16;
} arDnsClient_hdr_st;  

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __ARDNSCLIENT_QUERYH_TYPES_H__ */



