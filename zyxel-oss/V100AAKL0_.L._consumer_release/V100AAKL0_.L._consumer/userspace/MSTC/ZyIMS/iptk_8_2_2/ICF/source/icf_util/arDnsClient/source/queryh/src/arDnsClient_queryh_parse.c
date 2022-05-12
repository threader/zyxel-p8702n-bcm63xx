/******************************************************************************
 *                                    
 * FILE NAME    : arDnsClient_queryh_parse.c                
 *                                    
 * DESCRIPTION  : Contains the arDnsClient Query handler parsing routines  
 *                                    
 * Revision History :                            
 *
 *  DATE            NAME                 REFERENCE       REASON        
 * ------         ------------           ---------     ----------
 * 05-Feb-2009    Anuradha Gupta         None          Initial
 * 06-Feb-2009    Shweta Jain            LLD           arDnsClient 1.0  
 * 27-Apr-2009    Anuradha Gupta         Bug 7285      Error codes corrected  
 * 17-Jul-2009    Rajiv Kumar            IPTK Rel 8.2  Vxwork 5.3 Porting  
 * 20-Aug-2009    Anuradha Gupta         SPR 19935     Warning removal
 * 20-Aug-2009    Anuradha Gupta         SPR 19927     GCC warning fixing
 *
 * Copyright 2009, Aricent.                       
 *                                    
 *******************************************************************************/
#include "arDnsClient_queryh_defs.h"
#include "arDnsClient_queryh_prototypes.h"
#include "arDnsClient_queryh_types.h"
#include "arDnsClient_defs.h"
#include "arDnsClient_port_defs.h"

#if defined (ARDNSCL_PORT_LINUX) || defined (ARDNSCL_PORT_VXWORKS)
/****************************************************************************
 * API Name: arDnsClient_parse_dns_header
 * 
 * Description: This API is invoked to parse the DNS Response Header.
 * INPUT       :pOrg_buff is a buffer having the data for parsing. 
 * OUTPUT      :pHeader is a DNS message Header.
 *             :pError is the error code.    
 *              
 * Return : It will return DNSCL_SUCCESS if successful else return DNSCL_FAILURE.
 ******************************************************************************/
dnscl_return_t  arDnsClient_parse_dns_header (
        arDnsClient_global_data_st *pDnscl_glb_data,
        dnscl_buff_ptr             pOrg_buff, 
        arDnsClient_hdr_st         *pHeader, 
        dnscl_error_t              *pError)
{
    /* It specifies the result of API */
    dnscl_return_t ret_val = ARDNSCL_SUCCESS;

    dnscl_uint8_t *msg_buff = ARDNSCL_NULL;

    /* This line has been added just to remove compiler warning */
    pDnscl_glb_data = pDnscl_glb_data;

    ARDNSCL_PRINT(pDnscl_glb_data, ("Enterd Function:arDnsClient_parse_dns_header\n")); 
        
        /* Initialize current pointer */
        msg_buff = pOrg_buff;

        /* populate all the fields of header. */
        /* copy header */
        ARDNSCL_MEMCPY(pHeader, msg_buff, ARDNSCL_DNS_RESPONSE_HEADER_SIZE);

        /* As per as RFC 1035 rcode will be checked for following values */
        /* If name server was unable to interpret the query */
        if(1 == pHeader->rcode)
        {
            ARDNSCL_PRINT(pDnscl_glb_data,("Format Error\n"));
            *pError = ARDNSCL_ERROR_QUERY_FORMAT_ERROR;
            ARDNSCL_PRINT(pDnscl_glb_data,("Exit Function: arDnsClient_parse_dns_header\n"))
                ret_val = ARDNSCL_FAILURE;
        }

        /* If name server was unable to process this query */
        else if(2 == pHeader->rcode)
        {
            ARDNSCL_PRINT(pDnscl_glb_data,("Server Error: Server Failure\n"));
            *pError = ARDNSCL_ERROR_NON_RECOVERABLE_SERVER_ERROR;
            ARDNSCL_PRINT(pDnscl_glb_data,("Exit Function: arDnsClient_parse_dns_header\n"))
                ret_val = ARDNSCL_FAILURE;
        }

        /* If domain name referenced in query does not exist */
        else if(3 == pHeader->rcode)
        {
            ARDNSCL_PRINT(pDnscl_glb_data,("Server Error: Name Error\n"));
            *pError = ARDNSCL_ERROR_INVALID_DOMAIN_NAME;
            ARDNSCL_PRINT(pDnscl_glb_data,("Exit Function: arDnsClient_parse_dns_header\n"))
                ret_val = ARDNSCL_FAILURE;
        }

        /* If name server does not support requested kind of query */
        else if(4 == pHeader->rcode)
        {
            ARDNSCL_PRINT(pDnscl_glb_data,("Server Not implemented\n"));
            *pError = ARDNSCL_ERROR_SERVER_NOT_IMPLEMENTED;
            ARDNSCL_PRINT(pDnscl_glb_data,("Exit Function: arDnsClient_parse_dns_header\n"))
                ret_val = ARDNSCL_FAILURE;
        }

        /* If name server refuses to perform specified operation for policy reasons */
        else if(5 == pHeader->rcode)
        {
            ARDNSCL_PRINT(pDnscl_glb_data,("Server refused\n"));
            *pError = ARDNSCL_ERROR_SERVER_REFUSED;
            ARDNSCL_PRINT(pDnscl_glb_data,("Exit Function: arDnsClient_parse_dns_header\n"))
                ret_val = ARDNSCL_FAILURE;
        }

        else
        {
            /* number of question entries */
            pHeader->qdcount = ARDNSCL_NTOHS(pHeader->qdcount);

            /* number of answer entries */
            pHeader->ancount = ARDNSCL_NTOHS(pHeader->ancount);

            /* Validating the number of answer records field */
            if ( 0 == pHeader->ancount )
            {
                ARDNSCL_PRINT(pDnscl_glb_data,("Data Does Not Exist\n"));
                *pError = ARDNSCL_ERROR_DATA_DOES_NOT_EXIST;
                ARDNSCL_PRINT(pDnscl_glb_data,("Exit Function: arDnsClient_parse_dns_header\n"))
                ret_val = ARDNSCL_FAILURE;
            }
        }
    
    ARDNSCL_PRINT(pDnscl_glb_data,("Exit Function:arDnsClient_parse_dns_header\n"));                              
    return ret_val;
}
#endif

/*****************************************************************************
 * FUNCTION :   arDnsClient_parse_A_dns_response 
 *
 * Description: This function is used to parse the DNS Response having 
 *              A type of query.  
 * INPUT       :pOrg_buff is a buffer having the data for parsing. 
 *             :org_buff_len is length of buffer.
 * OUTPUT      :head is a structure having data of all query types.
 *             :nanswer_rec will contain number of answer records.
 *             :pError is the error code.    

 * Return:      It will return DNSCL_SUCCESS if successful else return DNSCL_FAILURE.
 ********************************************************************************/

dnscl_return_t    arDnsClient_parse_A_dns_response  (
        arDnsClient_global_data_st      *pDnscl_glb_data,
        dnscl_buff_ptr                  pOrg_buff, 
        dnscl_uint32_t                  org_buff_len,
        arDnsClient_dns_rr_list_st      **head,
        dnscl_uint8_t                  *nanswer_rec,
        dnscl_error_t                   *pError)
{
#ifndef ARDNSCL_PORT_WINDOWS
    arDnsClient_hdr_st msg_header;

    /* buffer will contain current message */
    dnscl_uint8_t *pCurr_buff = ARDNSCL_NULL;

    /* Counter for no. of answer records */
    dnscl_uint16_t  ans_cnt = 0; 
	dnscl_uint32_t qn_byte_cnt = 0;

    /* no. of bytes in answer record */
    dnscl_uint32_t an_byte_cnt = ARDNSCL_INIT_VAL;

    dnscl_int32_t   length = ARDNSCL_INIT_VAL; 

    /* Array to store the domain name of answer section */
    dnscl_uint8_t rr_domain_name[ARDNSCL_MAX_STR_LEN];

    /* pointer variables of type arDnsClient_dns_rr_list_st */
    arDnsClient_dns_rr_list_st  *prev = ARDNSCL_NULL, *curr;
#else
    dnscl_buff_ptr pDns_record, pDns_next; 
    dnscl_uint32_t count=0;
    arDnsClient_dns_rr_list_st  *prev = ARDNSCL_NULL, *curr;
    dnscl_ip_addr_st ipaddr;
#endif

    dnscl_return_t ret_val = ARDNSCL_SUCCESS;

    ARDNSCL_PRINT(pDnscl_glb_data, ("Enterd Function:arDnsClient_parse_A_dns_response\n")) 

#ifndef ARDNSCL_PORT_WINDOWS
    /* Checking whether original buffer length is greater than header size or not*/
    if (ARDNSCL_DNS_RESPONSE_HEADER_SIZE > org_buff_len)
    {
        ARDNSCL_PRINT(pDnscl_glb_data,("Bad message: invalid message length\n"))
            *pError = ARDNSCL_ERROR_BAD_DNS_MESSAGE;
        ARDNSCL_PRINT(pDnscl_glb_data,("Exit Function: arDnsClient_parse_A_dns_response\n"))
            return ARDNSCL_FAILURE;
    }

    /* Parse DNS Header first */
    if ( ARDNSCL_FAILURE == arDnsClient_parse_dns_header(pDnscl_glb_data,pOrg_buff, 
                &msg_header, pError))
    {
        ARDNSCL_PRINT(pDnscl_glb_data,("Failed to parse DNS header\n"));
        ARDNSCL_PRINT(pDnscl_glb_data,("Exit Function: arDnsClient_parse_A_dns_response\n"))
            return ARDNSCL_FAILURE;
    } 

    /* take the reference of message buffer */
    pCurr_buff = pOrg_buff + ARDNSCL_DNS_RESPONSE_HEADER_SIZE ; 

    /* parse the query section of header.*/ 
    if ( ARDNSCL_FAILURE == arDnsClient_parse_dns_qsection(pDnscl_glb_data,pOrg_buff, 
                org_buff_len, pCurr_buff,msg_header.qdcount, &qn_byte_cnt, pError))
    {
        ARDNSCL_PRINT(pDnscl_glb_data,("Failed to parse Question section\n"));
        ARDNSCL_PRINT(pDnscl_glb_data,("Exit Function: arDnsClient_parse_A_dns_response\n"))
            return ARDNSCL_FAILURE;
    } 

    /* Moving the pointer above the size of question part */
    pCurr_buff += qn_byte_cnt;

    /* parse the answer section of header. */

    while ( ans_cnt < msg_header.ancount )
    {
        ARDNSCL_MEMGET(curr, ARDNSCL_SIZEOF(arDnsClient_dns_rr_list_st))
            if (ARDNSCL_NULL == curr) 
            {
                ARDNSCL_PRINT(pDnscl_glb_data,("Failure in memory allocation\n"));
                *pError = ARDNSCL_ERROR_MEMORY_ALLOC_FAILURE;
                ARDNSCL_PRINT(pDnscl_glb_data,("Exit Function: arDnsClient_parse_A_dns_response\n"))
                    return ARDNSCL_FAILURE;
            }
        curr->pNext = ARDNSCL_NULL;
        if (ARDNSCL_NULL == *head)
            *head = curr;
        if (ARDNSCL_NULL != prev)
            prev->pNext = curr;

        /* Calculate the length of domain name of answer section */
        length = ARDNSCL_DN_EXPAND(pOrg_buff, pOrg_buff + org_buff_len, pCurr_buff,  
                (dnscl_int8_t *)rr_domain_name,ARDNSCL_SIZEOF(rr_domain_name));

        if ( 0 > length ) 
        {
            ARDNSCL_PRINT(pDnscl_glb_data,("name parameter is not present in answer section\n"));
            *pError =  ARDNSCL_ERROR_BAD_DNS_MESSAGE;
            ARDNSCL_PRINT(pDnscl_glb_data,("Exit Function: arDnsClient_parse_A_dns_response\n"))
                return ARDNSCL_FAILURE;
        }
        pCurr_buff += length;

        /* skip the type and class  of answer section */
        pCurr_buff = pCurr_buff + ARDNSCL_QUERY_TYPE_SIZE + ARDNSCL_QUERY_CLASS_SIZE ;

        /* Decode the TTL field */
        arDnsClient_decode_U32(&pCurr_buff , &(curr->ttl));

        /* Decode the structure of A query */
        if ( ARDNSCL_FAILURE == arDnsClient_parse_A_query(pDnscl_glb_data,pCurr_buff, 
                    &(curr->query_rr_data),&an_byte_cnt, pError))
        {
            ARDNSCL_PRINT(pDnscl_glb_data,("Failed to parse A type query\n"));
            ARDNSCL_PRINT(pDnscl_glb_data,("Exit Function: arDnsClient_parse_A_dns_response\n"))
                return ARDNSCL_FAILURE;
        } 

        /* moving the pointer to next record in buffer */
        pCurr_buff += an_byte_cnt;
        ans_cnt += 1;

        /* Checking for number of RR records */
        if (ARDNSCL_MAX_DNS_RR <= ans_cnt)
            break;

        prev = curr;
    } 
    *nanswer_rec = ans_cnt;
#endif

#ifdef ARDNSCL_PORT_WINDOWS
    pDns_record=(dnscl_buff_ptr) pOrg_buff;

    for (; pDns_record; pDns_record=pDns_next)
    {
		pDns_next=pDns_record->pNext;

		if(DnsSectionAnswer == pDns_record->Flags.S.Section)
		{
        ARDNSCL_MEMGET(curr, ARDNSCL_SIZEOF(arDnsClient_dns_rr_list_st))
            if (ARDNSCL_NULL == curr)
            {
                ARDNSCL_PRINT(pDnscl_glb_data,("Failure in memory allocation\n")); 
                *pError = ARDNSCL_ERROR_MEMORY_ALLOC_FAILURE;
                ARDNSCL_PRINT(pDnscl_glb_data,("Exit Function: arDnsClient_parse_A_dns_response\n"))
                    return ARDNSCL_FAILURE;
            }
        curr->pNext = ARDNSCL_NULL;

        if (ARDNSCL_NULL == *head)
            *head = curr;
        if (ARDNSCL_NULL != prev)
            prev->pNext = curr;

        curr->ttl = pDns_record->dwTtl;

        /* convert the Internet network address into a string 
           Internet standard dotted format.*/

        ipaddr.S_un.S_addr = pDns_record->Data.A.IpAddress;
        ARDNSCL_STRCPY(curr->query_rr_data.A_rr_data.addrIP4.str_val, 
                ARDNSCL_INET_NTOA(ipaddr));

        /* Populate the address field */
        curr->query_rr_data.A_rr_data.addrIP4.str_len = ARDNSCL_MAX_IPV4_STR_LEN;
        count += 1;

		

        /* To check it should not exceed max no of RR */
        if (ARDNSCL_MAX_DNS_RR <= count)
            break;
        prev = curr;
		}
    }
    *nanswer_rec = count;
#endif

    ARDNSCL_PRINT(pDnscl_glb_data,("Exit Function: arDnsClient_parse_A_dns_response\n"))
        return ret_val;
}


/*****************************************************************************
 * FUNCTION :   arDnsClient_parse_AAAA_dns_response 
 *
 * Description: This function is used to parse the DNS Response having 
 *              AAAA type of query.  
 * INPUT       :pOrg_buff is a buffer having the data for parsing. 
 *             :org_buff_len is length of buffer.
 * OUTPUT      :head is a structure having data of all query types.
 *             :nanswer_rec will contain number of answer records.
 *             :pError is the error code.    

 * Return:      It will return DNSCL_SUCCESS if successful else return DNSCL_FAILURE.
 ********************************************************************************/

dnscl_return_t   arDnsClient_parse_AAAA_dns_response (
        arDnsClient_global_data_st     *pDnscl_glb_data,
        dnscl_buff_ptr                 pOrg_buff, 
        dnscl_uint32_t                 org_buff_len,
        arDnsClient_dns_rr_list_st     **head,
        dnscl_uint8_t                 *nanswer_rec,
        dnscl_error_t                  *pError)
{

#ifndef ARDNSCL_PORT_WINDOWS 

    arDnsClient_hdr_st msg_header;

    /* buffer will contain current message */
    dnscl_uint8_t *pCurr_buff = ARDNSCL_NULL;

    /* Counter for no. of answer records */
    dnscl_uint16_t  ans_cnt = 0; 
	dnscl_uint32_t qn_byte_cnt = 0;

    /* no. of bytes in answer record */
    dnscl_uint32_t an_byte_cnt = ARDNSCL_INIT_VAL;

    dnscl_int32_t  length = ARDNSCL_INIT_VAL;

    /* Array to store the domain name of answer section */
    dnscl_uint8_t rr_domain_name[ARDNSCL_MAX_STR_LEN];

    /* pointer variables of type arDnsClient_dns_rr_list_st */
    arDnsClient_dns_rr_list_st *prev = ARDNSCL_NULL, *curr;
#else
    dnscl_buff_ptr pDns_record, pDns_next; 
    dnscl_uint32_t count=0, counter;
    arDnsClient_dns_rr_list_st  *prev = ARDNSCL_NULL, *curr;
	dnscl_uint8_t temp[ARDNSCL_MAX_STR_LEN] = {'\0',}, temp_buff[ARDNSCL_MAX_STR_LEN] = {'\0',};
#endif

    dnscl_return_t ret_val = ARDNSCL_SUCCESS;

    ARDNSCL_PRINT(pDnscl_glb_data,("Enterd Function:arDnsClient_parse_AAAA_dns_response\n")) 

#ifndef ARDNSCL_PORT_WINDOWS
	/* Checking whether original buffer length is greater than header size or not*/
    if (ARDNSCL_DNS_RESPONSE_HEADER_SIZE > org_buff_len)
    {
        ARDNSCL_PRINT(pDnscl_glb_data,("Bad message: message length\
                    is smaller than header size\n"));
        *pError = ARDNSCL_ERROR_BAD_DNS_MESSAGE;
        ARDNSCL_PRINT(pDnscl_glb_data,("Exit Function: arDnsClient_parse_AAAA_dns_response\n"))
            return ARDNSCL_FAILURE;
    }

    /* Parse DNS Header first */
    if ( ARDNSCL_FAILURE == arDnsClient_parse_dns_header(pDnscl_glb_data,pOrg_buff, 
                &msg_header, pError))
    {
        ARDNSCL_PRINT(pDnscl_glb_data,("Failed to parse DNS header\n"));
        ARDNSCL_PRINT(pDnscl_glb_data,("Exit Function: arDnsClient_parse_AAAA_dns_response\n"))
            return ARDNSCL_FAILURE;
    } 

    /* take the reference of message buffer */
    pCurr_buff = pOrg_buff + ARDNSCL_DNS_RESPONSE_HEADER_SIZE; 

    /* parse the query section of header. */
    if ( ARDNSCL_FAILURE == arDnsClient_parse_dns_qsection(pDnscl_glb_data,pOrg_buff, 
                org_buff_len, pCurr_buff,msg_header.qdcount, &qn_byte_cnt, pError))
    {
        ARDNSCL_PRINT(pDnscl_glb_data,("Failed to parse question section\n"));
        ARDNSCL_PRINT(pDnscl_glb_data,("Exit Function: arDnsClient_parse_AAAA_dns_response\n"))
            return ARDNSCL_FAILURE;
    } 

    /* Moving the pointer above the size of question part */
    pCurr_buff += qn_byte_cnt;

    /* parse the answer section of header. */
    while ( ans_cnt < msg_header.ancount )
    {
        ARDNSCL_MEMGET(curr, ARDNSCL_SIZEOF(arDnsClient_dns_rr_list_st))
            if (ARDNSCL_NULL == curr) 
            {
                ARDNSCL_PRINT(pDnscl_glb_data,("Failure in memory allocation")) 
                    *pError = ARDNSCL_ERROR_MEMORY_ALLOC_FAILURE;
                ARDNSCL_PRINT(pDnscl_glb_data,
                        ("Exit Function: arDnsClient_parse_AAAA_dns_response\n"))
                    return ARDNSCL_FAILURE;
            }
        curr->pNext = ARDNSCL_NULL;
        if (ARDNSCL_NULL == *head)
            *head = curr;
        else if (ARDNSCL_NULL != prev)
            prev->pNext = curr;

        /* Calculating the length of domain name of answer section */ 
        length = ARDNSCL_DN_EXPAND(pOrg_buff, pOrg_buff + org_buff_len, pCurr_buff, 
                (dnscl_int8_t *)rr_domain_name,ARDNSCL_SIZEOF(rr_domain_name));

        if ( 0 > length ) 
        {
            ARDNSCL_PRINT(pDnscl_glb_data,("name parameter is not present in answer section\n"))
                *pError =  ARDNSCL_ERROR_BAD_DNS_MESSAGE;
            ARDNSCL_PRINT(pDnscl_glb_data,("Exit Function: arDnsClient_parse_AAAA_dns_response\n"))
                return ARDNSCL_FAILURE;
        }

        pCurr_buff += length;

        /* skip the type and class of answer section */
        pCurr_buff = pCurr_buff +  ARDNSCL_QUERY_TYPE_SIZE + ARDNSCL_QUERY_CLASS_SIZE;

        /* Decode the TTL field */
        arDnsClient_decode_U32(&pCurr_buff , &(curr->ttl));

        /* Decode the structure of AAAA query */
        if ( ARDNSCL_FAILURE == arDnsClient_parse_AAAA_query(pDnscl_glb_data,pCurr_buff,  
                    &(curr->query_rr_data), &an_byte_cnt,pError))
        {
            ARDNSCL_PRINT(pDnscl_glb_data,("Failed to parse AAAA type query\n"))
                ARDNSCL_PRINT(pDnscl_glb_data,("Exit Function: arDnsClient_parse_AAAA_dns_response\n"))
                return ARDNSCL_FAILURE;
        } 

        /* moving the pointer to next record in buffer */
        pCurr_buff += an_byte_cnt;
        ans_cnt += 1;

        if (ARDNSCL_MAX_DNS_RR <= ans_cnt)
            break;
        prev = curr;
    }
    *nanswer_rec = ans_cnt;

#endif

#ifdef ARDNSCL_PORT_WINDOWS
    pDns_record=(dnscl_buff_ptr) pOrg_buff;

    for (; pDns_record; pDns_record=pDns_next)
    {
		pDns_next=pDns_record->pNext;

		if(DnsSectionAnswer == pDns_record->Flags.S.Section)
		{
        ARDNSCL_MEMGET(curr, ARDNSCL_SIZEOF(arDnsClient_dns_rr_list_st));
        if (ARDNSCL_NULL == curr)
        {
            ARDNSCL_PRINT(pDnscl_glb_data,("Failure in memory allocation"));
            *pError = ARDNSCL_ERROR_MEMORY_ALLOC_FAILURE;
            ARDNSCL_PRINT(pDnscl_glb_data,("Exit Function: arDnsClient_parse_AAAA_dns_response\n"))
                return ARDNSCL_FAILURE;
        }
        curr->pNext = ARDNSCL_NULL;
        if (ARDNSCL_NULL == *head)
            *head = curr;
        if (ARDNSCL_NULL != prev)
            prev->pNext = curr;

        curr->ttl = pDns_record->dwTtl;

        for (counter = 0; counter < 16; counter++) 
        {
            ARDNSCL_SPRINTF(temp_buff,"%02x",(pDns_record->Data.AAAA.Ip6Address.IP6Byte)[counter]);
            ARDNSCL_STRCAT(temp,temp_buff); 
            if (!((counter + 1) % 2) && (counter != 15)) 
				ARDNSCL_STRCAT(temp,":"); 
        }

        ARDNSCL_STRCPY(curr->query_rr_data.AAAA_rr_data.addrIP6.str_val,temp); 
        curr->query_rr_data.AAAA_rr_data.addrIP6.str_len = 
			(dnscl_uint8_t)ARDNSCL_STRLEN(temp);

        count += 1;

        /* To check it should not exceed max no of RR */
        if (ARDNSCL_MAX_DNS_RR <= count)
            break;
        prev = curr;
		}
    }  
    *nanswer_rec = count;
#endif

    ARDNSCL_PRINT(pDnscl_glb_data,("Exit Function: arDnsClient_parse_AAAA_dns_response\n"))
        return ret_val;
}


/*****************************************************************************
 * FUNCTION :   arDnsClient_parse_SRV_dns_response 
 *
 * Description: This function is used to parse the DNS Response having 
 *              SRV type of query.  
 * INPUT       :pOrg_buff is a buffer having the data for parsing. 
 *             :org_buff_len is length of buffer.
 * OUTPUT      :head is a structure having data of all query types.
 *             :nanswer_rec will contain number of answer records.
 *             :pError is the error code.    

 * Return:      It will return DNSCL_SUCCESS if successful else return DNSCL_FAILURE.
 ********************************************************************************/

dnscl_return_t   arDnsClient_parse_SRV_dns_response (
        arDnsClient_global_data_st      *pDnscl_glb_data,
        dnscl_buff_ptr                  pOrg_buff, 
        dnscl_uint32_t                  org_buff_len,
        arDnsClient_dns_rr_list_st      **head,
        dnscl_uint8_t                  *nanswer_rec,
        dnscl_error_t                   *pError)
{
#ifndef ARDNSCL_PORT_WINDOWS

    arDnsClient_hdr_st msg_header;

    /* buffer will contain current message */
    dnscl_uint8_t *pCurr_buff = ARDNSCL_NULL;

    /* Counter for no. of answer records */
    dnscl_uint16_t  ans_cnt = 0; 
	dnscl_uint32_t qn_byte_cnt = 0;

    /* no. of bytes in answer record */
    dnscl_uint32_t an_byte_cnt = ARDNSCL_INIT_VAL;

    dnscl_int32_t  length = ARDNSCL_INIT_VAL;

    /* Array to store the domain name of answer section */
    dnscl_uint8_t rr_domain_name[ARDNSCL_MAX_STR_LEN];

    /* pointer variables of type arDnsClient_dns_rr_list_st */
    arDnsClient_dns_rr_list_st *prev = ARDNSCL_NULL, *curr;
#else
    dnscl_buff_ptr pDns_record, pDns_next; 
    dnscl_uint32_t count=0;
    arDnsClient_dns_rr_list_st  *prev = ARDNSCL_NULL, *curr;
	int ret = 0;
#endif

    dnscl_return_t ret_val = ARDNSCL_SUCCESS;

    ARDNSCL_PRINT(pDnscl_glb_data,("Enterd Function:arDnsClient_parse_SRV_dns_response")); 

#ifndef ARDNSCL_PORT_WINDOWS 

	/* Checking whether original buffer length is greater than header size or not*/
    if (ARDNSCL_DNS_RESPONSE_HEADER_SIZE > org_buff_len)
    {
        ARDNSCL_PRINT(pDnscl_glb_data,("Bad message: message\
                    length is smaller than header size\n")); 
            *pError = ARDNSCL_ERROR_BAD_DNS_MESSAGE;
        ARDNSCL_PRINT(pDnscl_glb_data,("Exit Function: arDnsClient_parse_SRV_dns_response\n"))
        return ARDNSCL_FAILURE;
    }

    /* Parse DNS Header first */
    if ( ARDNSCL_FAILURE == arDnsClient_parse_dns_header(pDnscl_glb_data,pOrg_buff, 
                &msg_header, pError))
    {
        ARDNSCL_PRINT(pDnscl_glb_data,("Failed to parse DNS header\n"));
        ARDNSCL_PRINT(pDnscl_glb_data,("Exit Function: arDnsClient_parse_SRV_dns_response\n"))
        return ARDNSCL_FAILURE;
    } 

    /* take the reference of message buffer */
    pCurr_buff = pOrg_buff + ARDNSCL_DNS_RESPONSE_HEADER_SIZE; 

    /* parse the query section of header. */
    if ( ARDNSCL_FAILURE == arDnsClient_parse_dns_qsection(pDnscl_glb_data,pOrg_buff, 
                org_buff_len, pCurr_buff,msg_header.qdcount, &qn_byte_cnt, pError))
    {
        ARDNSCL_PRINT(pDnscl_glb_data,("Failed to parse question section\n"));
        ARDNSCL_PRINT(pDnscl_glb_data,("Exit Function: arDnsClient_parse_SRV_dns_response\n"))
        return ARDNSCL_FAILURE;
    } 

    /* Moving the pointer above the size of question part */
    pCurr_buff += qn_byte_cnt;

    /* parse the answer section of header. */
    while ( ans_cnt < msg_header.ancount )
    {
        ARDNSCL_MEMGET(curr, ARDNSCL_SIZEOF(arDnsClient_dns_rr_list_st));
        if (ARDNSCL_NULL == curr) 
        {
            ARDNSCL_PRINT(pDnscl_glb_data,("Failure in memory allocation"));
            *pError = ARDNSCL_ERROR_MEMORY_ALLOC_FAILURE;
            ARDNSCL_PRINT(pDnscl_glb_data,("Exit Function: arDnsClient_parse_SRV_dns_response\n"))
            return ARDNSCL_FAILURE;
        }
        curr->pNext = ARDNSCL_NULL;

        if (ARDNSCL_NULL == *head)
            *head = curr;
        if (ARDNSCL_NULL != prev)
            prev->pNext = curr;

        /* Calculating the length of domain name of answer section */
        length = ARDNSCL_DN_EXPAND(pOrg_buff, pOrg_buff + org_buff_len, pCurr_buff, 
                (dnscl_int8_t *)rr_domain_name,ARDNSCL_SIZEOF(rr_domain_name));

        if ( 0 > length ) 
        {
            ARDNSCL_PRINT(pDnscl_glb_data,("name parameter is not present in answer section\n"));
            *pError =  ARDNSCL_ERROR_BAD_DNS_MESSAGE;
            ARDNSCL_PRINT(pDnscl_glb_data,("Exit Function: arDnsClient_parse_SRV_dns_response\n"))
            return ARDNSCL_FAILURE;
        }

        pCurr_buff += length;

        /* skip the type and class of answer section */
        pCurr_buff = pCurr_buff + ARDNSCL_QUERY_TYPE_SIZE + ARDNSCL_QUERY_CLASS_SIZE;

        /* Decode the TTL field */
        arDnsClient_decode_U32(&pCurr_buff ,&(curr->ttl));

        /* Decode the structure of SRV query */
        if ( ARDNSCL_FAILURE == arDnsClient_parse_Srv_query(pDnscl_glb_data,pOrg_buff, 
                    org_buff_len, pCurr_buff, &(curr->query_rr_data), &an_byte_cnt, pError))
        {
            ARDNSCL_PRINT(pDnscl_glb_data,("Failed to parse SRV type query\n"));
            ARDNSCL_PRINT(pDnscl_glb_data,("Exit Function: arDnsClient_parse_SRV_dns_response\n"))
            return ARDNSCL_FAILURE;
        } 

        /* moving the pointer to next record in buffer */
        pCurr_buff += an_byte_cnt;
        ans_cnt += 1;
        if (ARDNSCL_MAX_DNS_RR <= ans_cnt)
            break;
        prev = curr; 
    }
    *nanswer_rec = ans_cnt;
#endif


#ifdef ARDNSCL_PORT_WINDOWS
    pDns_record=(dnscl_buff_ptr) pOrg_buff;

    for (; pDns_record; pDns_record=pDns_next)
    {
		pDns_next=pDns_record->pNext;

		if(DnsSectionAnswer == pDns_record->Flags.S.Section)
		{
			ARDNSCL_MEMGET(curr , ARDNSCL_SIZEOF(arDnsClient_dns_rr_list_st))
            if (ARDNSCL_NULL == curr)
            {
                ARDNSCL_PRINT(pDnscl_glb_data,("Failure in memory allocation"));
                *pError = ARDNSCL_ERROR_MEMORY_ALLOC_FAILURE;
                ARDNSCL_PRINT(pDnscl_glb_data,
                        ("Exit Function: arDnsClient_parse_SRV_dns_response\n"))
                return ARDNSCL_FAILURE;
             }
        curr->pNext = ARDNSCL_NULL;

        if (ARDNSCL_NULL == *head)
            *head = curr;
        if (ARDNSCL_NULL != prev)
            prev->pNext = curr;

        
        curr->ttl = pDns_record->dwTtl;

        /* Populating priority */
        curr->query_rr_data.srv_rr_data.priority = 
            pDns_record->Data.SRV.wPriority;

        /* Populating weight */
        curr->query_rr_data.srv_rr_data.weight = 
            pDns_record->Data.SRV.wWeight;

        /* Populating port */
        curr->query_rr_data.srv_rr_data.port = 
            pDns_record->Data.SRV.wPort; 

		/* convert the Wide char string to Char string */
		curr->query_rr_data.srv_rr_data.nameTarget.str_val[0] = '\0';
		ret = WideCharToMultiByte(CP_ACP,WC_COMPOSITECHECK,
			pDns_record->Data.SRV.pNameTarget,
			-1, 
			curr->query_rr_data.srv_rr_data.nameTarget.str_val,
			ARDNSCL_MAX_STR_LEN,NULL, 0);

		if(0 == ret)
		{
			ret = GetLastError();
            ARDNSCL_PRINT(pDnscl_glb_data,("Failure in string conversion"));
           *pError = ARDNSCL_ERROR_BAD_DNS_MESSAGE;
            ARDNSCL_PRINT(pDnscl_glb_data,
                        ("Exit Function: arDnsClient_parse_SRV_dns_response\n"))
            return ARDNSCL_FAILURE;
         }

        /* Populating nametarget */
        /* ARDNSCL_STRCPY(curr->query_rr_data.srv_rr_data.nameTarget.str_val, 
                pDns_record->Data.SRV.pNameTarget);*/
        curr->query_rr_data.srv_rr_data.nameTarget.str_len = 
            (dnscl_uint8_t)ARDNSCL_STRLEN((dnscl_int8_t *)
                    curr->query_rr_data.srv_rr_data.nameTarget.str_val);
        count += 1;

        /* To check it should not exceed max no of RR */
        if (ARDNSCL_MAX_DNS_RR <= count)
            break;
        prev = curr;
		}
    }    
    *nanswer_rec = count;
#endif

    ARDNSCL_PRINT(pDnscl_glb_data,("Exit Function: arDnsClient_parse_SRV_dns_response\n"))
    return ret_val;
}


/*****************************************************************************
 * FUNCTION :   arDnsClient_parse_NAPTR_dns_response 
 *
 * Description: This function is used to parse the DNS Response having 
 *              NAPTR type of query.  
 * INPUT       :pOrg_buff is a buffer having the data for parsing. 
 *             :org_buff_len is length of buffer.
 * OUTPUT      :head is a structure having data of all query types.
 *             :nanswer_rec will contain number of answer records.
 *             :pError is the error code.    

 * Return:      It will return DNSCL_SUCCESS if successful else return DNSCL_FAILURE.
 ********************************************************************************/

dnscl_return_t  arDnsClient_parse_NAPTR_dns_response (
        arDnsClient_global_data_st     *pDnscl_glb_data,
        dnscl_buff_ptr                 pOrg_buff, 
        dnscl_uint32_t                 org_buff_len,
        arDnsClient_dns_rr_list_st     **head,
        dnscl_uint8_t                 *nanswer_rec,
        dnscl_error_t                  *pError)
{
    dnscl_return_t  ret_val = ARDNSCL_SUCCESS;
#ifndef ARDNSCL_PORT_WINDOWS
    arDnsClient_hdr_st msg_header;

    /* buffer will contain current message */
    dnscl_uint8_t *pCurr_buff = ARDNSCL_NULL;

    /* Counter for no. of answer records */
    dnscl_uint16_t  ans_cnt = 0; 
	dnscl_uint32_t qn_byte_cnt = 0;

    /* no. of bytes in answer record */
    dnscl_uint32_t an_byte_cnt = ARDNSCL_INIT_VAL;

    dnscl_int32_t  length = ARDNSCL_INIT_VAL;

    /* Array to store the domain name of answer section */
    dnscl_uint8_t  rr_domain_name[ARDNSCL_MAX_STR_LEN];

    /* pointer variables of type arDnsClient_dns_rr_list_st */
    arDnsClient_dns_rr_list_st *prev = ARDNSCL_NULL, *curr;
#else
    dnscl_buff_ptr pDns_record, pDns_next; 
    dnscl_uint32_t count=0, length = 0, temp_len = 0;
    arDnsClient_dns_rr_list_st  *prev = ARDNSCL_NULL, *curr;
	dnscl_uint8_t *temp_buff = ARDNSCL_NULL, temp[ARDNSCL_MAX_STR_LEN] = {'\0',}, 
		buffer[ARDNSCL_MAX_STR_LEN] = {'\0',};
#endif

#ifndef ARDNSCL_PORT_WINDOWS

    ARDNSCL_PRINT(pDnscl_glb_data,("Enterd Function:arDnsClient_parse_NAPTR_dns_response")); 

    /* perform Null pointer validations */    
    if(ARDNSCL_NULL == pOrg_buff)
    {
        ARDNSCL_PRINT(pDnscl_glb_data,("Message Buffer is Invalid\n")); 
        *pError = ARDNSCL_ERROR_DNS_INVALID_MSG_BUFFER;
        ARDNSCL_PRINT(pDnscl_glb_data,("Exit Function: arDnsClient_parse_NAPTR_dns_response\n"))
        return ARDNSCL_FAILURE;
    }

    /* Checking whether original buffer length is greater than header size or not*/
    if (ARDNSCL_DNS_RESPONSE_HEADER_SIZE > org_buff_len)
    {
        ARDNSCL_PRINT(pDnscl_glb_data,("Bad message: message\
                    length is smaller than header size\n"));
        *pError = ARDNSCL_ERROR_BAD_DNS_MESSAGE;
        ARDNSCL_PRINT(pDnscl_glb_data,("Exit Function: arDnsClient_parse_NAPTR_dns_response\n"))
        return ARDNSCL_FAILURE;
    }

    /* Parse DNS Header first */
    if ( ARDNSCL_FAILURE == arDnsClient_parse_dns_header(pDnscl_glb_data,pOrg_buff, 
                &msg_header, pError))
    {
        ARDNSCL_PRINT(pDnscl_glb_data,("Failed to parse DNS header\n"));
        ARDNSCL_PRINT(pDnscl_glb_data,("Exit Function: arDnsClient_parse_NAPTR_dns_response\n"))
        return ARDNSCL_FAILURE;
    } 

    /* take the reference of message buffer */
    pCurr_buff = pOrg_buff + ARDNSCL_DNS_RESPONSE_HEADER_SIZE; 

    /* parse the query section of header. */
    if ( ARDNSCL_FAILURE == arDnsClient_parse_dns_qsection(pDnscl_glb_data,pOrg_buff, 
                org_buff_len, pCurr_buff,msg_header.qdcount, &qn_byte_cnt, pError))
    {
        ARDNSCL_PRINT(pDnscl_glb_data,("Failed to parse question section\n"));
        ARDNSCL_PRINT(pDnscl_glb_data,("Exit Function: arDnsClient_parse_NAPTR_dns_response\n"))
        return ARDNSCL_FAILURE;
    } 

    /* Moving the pointer above the size of question part */
    pCurr_buff += qn_byte_cnt;

    /* parse the answer section of header. */
    while ( ans_cnt < msg_header.ancount )
    {
        ARDNSCL_MEMGET(curr, ARDNSCL_SIZEOF(arDnsClient_dns_rr_list_st))
            if (ARDNSCL_NULL  == curr) 
            {
                ARDNSCL_PRINT(pDnscl_glb_data,("Failure in memory allocation"));
                *pError = ARDNSCL_ERROR_MEMORY_ALLOC_FAILURE;
               ARDNSCL_PRINT(pDnscl_glb_data,
                       ("Exit Function: arDnsClient_parse_NAPTR_dns_response\n"))
                return ARDNSCL_FAILURE;
            }
        curr->pNext = ARDNSCL_NULL;

        if (ARDNSCL_NULL  == *head)
            *head = curr;
        if (ARDNSCL_NULL != prev)
            prev->pNext = curr;

        /* Calculating the length of domain name of answer section */
        length = ARDNSCL_DN_EXPAND(pOrg_buff, pOrg_buff + org_buff_len, pCurr_buff, 
                (dnscl_int8_t *)rr_domain_name,ARDNSCL_SIZEOF(rr_domain_name));

        if ( 0 > length ) 
        {
            ARDNSCL_PRINT(pDnscl_glb_data,("name parameter is not present in answer section\n"));
            *pError =  ARDNSCL_ERROR_BAD_DNS_MESSAGE;
            ARDNSCL_PRINT(pDnscl_glb_data,
                       ("Exit Function: arDnsClient_parse_NAPTR_dns_response\n"))
            return ARDNSCL_FAILURE;
        }

        pCurr_buff += length;

        /* skip the type and class of answer section */
        pCurr_buff = pCurr_buff + ARDNSCL_QUERY_TYPE_SIZE + ARDNSCL_QUERY_CLASS_SIZE;

        /* Decode the TTL field */
        arDnsClient_decode_U32(&pCurr_buff ,  &(curr->ttl));

        /* Decode the structure of NAPTR query */
        if ( ARDNSCL_FAILURE == arDnsClient_parse_Naptr_query(pDnscl_glb_data,pOrg_buff, 
                    org_buff_len, pCurr_buff, &(curr->query_rr_data),&an_byte_cnt, pError))
        {
            ARDNSCL_PRINT(pDnscl_glb_data,("Failed to parse NAPTR type query\n"))
            ARDNSCL_PRINT(pDnscl_glb_data,
                       ("Exit Function: arDnsClient_parse_NAPTR_dns_response\n"))    
            return ARDNSCL_FAILURE;
        } 

        /* moving the pointer to next record in buffer */
        pCurr_buff +=an_byte_cnt;
        ans_cnt += 1;

        /* Checking no.of answer records should not exceed than MAX_DNS_RR */
        if (ARDNSCL_MAX_DNS_RR <= ans_cnt)
            break;

        prev = curr;
    }
    *nanswer_rec = ans_cnt;
#endif

#ifdef ARDNSCL_PORT_WINDOWS
    pDns_record=(dnscl_buff_ptr) pOrg_buff;

    for (; pDns_record; pDns_record=pDns_next)
    {
		pDns_next=pDns_record->pNext;

		if(DnsSectionAnswer == pDns_record->Flags.S.Section)
		{
        ARDNSCL_MEMGET(curr, ARDNSCL_SIZEOF(arDnsClient_dns_rr_list_st))
            if (ARDNSCL_NULL == curr)
            {
                ARDNSCL_PRINT(pDnscl_glb_data,("Failure in memory alloaction"));
                *pError = ARDNSCL_ERROR_MEMORY_ALLOC_FAILURE;
                ARDNSCL_PRINT(pDnscl_glb_data,
                       ("Exit Function: arDnsClient_parse_NAPTR_dns_response\n"))
                return ARDNSCL_FAILURE;
            }
        curr->pNext = ARDNSCL_NULL;

        if (ARDNSCL_NULL == *head)
            *head = curr;
        if (ARDNSCL_NULL != prev)
            prev->pNext = curr;

        curr->ttl = pDns_record->dwTtl;

        curr->query_rr_data.naptr_rr_data.order = 
            ARDNSCL_HTONS(pDns_record->Data.NAPTR.wOrder);
        curr->query_rr_data.naptr_rr_data.preference = 
            ARDNSCL_HTONS(pDns_record->Data.NAPTR.wPreference);
        temp_buff = (dnscl_uint8_t *) &pDns_record->Data.NAPTR.pFlags;      

        /* populating flags */
        length = *temp_buff++;
        if ( 0 != length)
		{
			if(ARDNSCL_MAX_SHORT_STR_LEN <=  length) 
			{
				temp_len = ARDNSCL_MAX_SHORT_STR_LEN - 1;
			}
			else
			{
				temp_len = length;
			}
            ARDNSCL_SPRINTF(curr->query_rr_data.naptr_rr_data.flags.   
                    str_val, "%.*s", temp_len, temp_buff);
		}

        curr->query_rr_data.naptr_rr_data.flags.str_len = temp_len;
        temp_buff += length;

        /* populating Service */
        length = *temp_buff++;
        if ( 0 != length )
		{
			if(ARDNSCL_MAX_SHORT_STR_LEN <=  length) 
			{
				temp_len = ARDNSCL_MAX_SHORT_STR_LEN - 1;
			}
			else
			{
				temp_len = length;
			}
            ARDNSCL_SPRINTF(curr->query_rr_data.naptr_rr_data.service. 
                    str_val, "%.*s", temp_len, temp_buff);
		}

        curr->query_rr_data.naptr_rr_data.service.str_len = temp_len;
        temp_buff += length;

        /* populating Regexp */
        length = *temp_buff++;
        if ( 0 != length )
		{
			if(ARDNSCL_MAX_SHORT_STR_LEN <=  length) 
			{
				temp_len = ARDNSCL_MAX_SHORT_STR_LEN - 1;
			}
			else
			{
				temp_len = length;
			}

            ARDNSCL_SPRINTF(curr->query_rr_data.naptr_rr_data.regularExpr. 
                    str_val,"%.*s", temp_len, temp_buff);
		}

        curr->query_rr_data.naptr_rr_data.regularExpr.str_len = temp_len;
        temp_buff += length;

        /* Populating Replacement */
        length = *temp_buff++;

		temp_len = 0;
		ARDNSCL_STRCPY(buffer,"\0");
        while(0 < length)
        {     
			if(ARDNSCL_MAX_STR_LEN <=  temp_len) 
			{
				temp_len = ARDNSCL_MAX_STR_LEN - 1;
				break;
			}
            ARDNSCL_SPRINTF(temp,"%.*s", length, temp_buff);
            ARDNSCL_STRCAT(buffer,temp);      
            temp_buff += length;
            temp_len += length;
            length = *temp_buff++;  
            if(length)
            {
                ARDNSCL_SPRINTF(temp,".");
                ARDNSCL_STRCAT(buffer,temp);
                temp_len++;
            }
        }

        curr->query_rr_data.naptr_rr_data.replacement.str_len = temp_len;
        ARDNSCL_MEMCPY(curr->query_rr_data.naptr_rr_data.replacement.str_val, buffer,temp_len);
        count += 1;

        if (ARDNSCL_MAX_DNS_RR <= count)
            break;
        prev = curr;
		}
    }
    *nanswer_rec = count;
#endif

    ARDNSCL_PRINT(pDnscl_glb_data,
                       ("Exit Function: arDnsClient_parse_NAPTR_dns_response\n"))
    return ret_val;
}

#if defined (ARDNSCL_PORT_LINUX) || defined (ARDNSCL_PORT_VXWORKS)
/*****************************************************************************
 * FUNCTION :   arDnsClient_parse_A_query 
 *
 * Description: This function is used to populate the A query structure.
 * INPUT       :pCurr_buff is a buffer having the current data for further parsing. 
 * OUTPUT      :rec is a union will contain data of A query.
 *             :nBytes_qr will contain no. of bytes in particular answer record.          
 *             :pError is the error code.    

 * Return:     It will return DNSCL_SUCCESS if successful else return DNSCL_FAILURE.
 ********************************************************************************/

dnscl_return_t   arDnsClient_parse_A_query (
        arDnsClient_global_data_st   *pDnscl_glb_data,
        dnscl_buff_ptr               pCurr_buff, 
        arDnsClient_rr_data_un       *rec,
        dnscl_uint32_t               *nBytes_qr,
        dnscl_error_t                *pError)
{
    dnscl_return_t   ret_val = ARDNSCL_SUCCESS;
    dnscl_ip_addr_st addr;
    dnscl_uint16_t  length = 0;
    dnscl_uint8_t  *p_addr_val = ARDNSCL_NULL;
    dnscl_uint32_t str_length = 0;

	/* To remove warnings */
	pDnscl_glb_data = pDnscl_glb_data;

    *nBytes_qr = 0;

    /* Decoding IP4 address field */
    arDnsClient_decode_U16(&pCurr_buff , &length);

    *nBytes_qr += ARDNSCL_SIZEOF(dnscl_uint16_t);

    if(ARDNSCL_IPV4_ADDR_LEN != length)
    {
        ARDNSCL_PRINT(pDnscl_glb_data,("wrong address of A query\n"));
        *pError =  ARDNSCL_ERROR_BAD_DNS_MESSAGE;
        return ARDNSCL_FAILURE;
    }
    else
    {
        ARDNSCL_MEMCPY((void *)&addr, (void *)pCurr_buff, ARDNSCL_SIZEOF(addr));
        p_addr_val = (dnscl_uint8_t*)ARDNSCL_INET_NTOA((struct in_addr)addr);
        str_length = ARDNSCL_STRLEN((const dnscl_int8_t*)p_addr_val);

        if(ARDNSCL_MAX_IPV4_STR_LEN <= str_length)
        {
            str_length = ARDNSCL_MAX_IPV4_STR_LEN - 1;
        }

        ARDNSCL_MEMCPY(&rec->A_rr_data.addrIP4.str_val,p_addr_val,str_length);

        rec->A_rr_data.addrIP4.str_len = str_length;
    }

    *nBytes_qr += length;
    return ret_val;
}
/*****************************************************************************
 * FUNCTION :   arDnsClient_parse_AAAA_query 
 *
 * Description: This function is used to populate the AAAA query structure.
 * INPUT       :pCurr_buff is a buffer having the current data for further parsing. 
 * OUTPUT      :rec is a union will contain data of AAAA query.
 *             :nBytes_qr will contain no. of bytes in particular answer record.     
 *             :pError is the error code.    

 * Return:     It will return DNSCL_SUCCESS if successful else return DNSCL_FAILURE.
 ********************************************************************************/

dnscl_return_t   arDnsClient_parse_AAAA_query (
        arDnsClient_global_data_st   *pDnscl_glb_data,
        dnscl_buff_ptr               pCurr_buff, 
        arDnsClient_rr_data_un       *rec,
        dnscl_uint32_t               *nBytes_qr,
        dnscl_error_t                *pError)
{
    dnscl_return_t   ret_val = ARDNSCL_SUCCESS;
    dnscl_uint16_t  length = 0;

    /* This line has been added just to remove compiler warning */
    pDnscl_glb_data = pDnscl_glb_data;

    *nBytes_qr = 0;
    /* Decoding IP6 address field */
    arDnsClient_decode_U16(&pCurr_buff, &length);

    *nBytes_qr += ARDNSCL_SIZEOF(dnscl_uint16_t);
   
    if(ARDNSCL_IPV6_ADDR_LEN != length)
    {
        ARDNSCL_PRINT(pDnscl_glb_data,("wrong address of AAAA query\n"));
        *pError =  ARDNSCL_ERROR_BAD_DNS_MESSAGE;
        return ARDNSCL_FAILURE;
    }
    else
    {
        /*Changes done for Vxwork 5.4 Porting*/
#ifndef ARDNSCL_PORT_VXWORKS
        dnscl_uint8_t addr[ARDNSCL_SIZEOF "ffff:ffff:ffff:ffff:ffff:ffff:255.255.255.255"];
        dnscl_uint32_t str_length = 0;
        ARDNSCL_INET_NTOP(AF_INET6, pCurr_buff, (dnscl_int8_t*)addr, ARDNSCL_SIZEOF(addr));

        str_length = ARDNSCL_STRLEN((const dnscl_int8_t*)addr);

        ARDNSCL_MEMCPY(rec->AAAA_rr_data.addrIP6.str_val,addr,str_length);

        rec->AAAA_rr_data.addrIP6.str_len = str_length;
#else
       /* compilation warning removal */
       rec = rec;
#endif
    }
    *nBytes_qr += length;
    return ret_val;
}


/*****************************************************************************
 * FUNCTION :   arDnsClient_parse_Srv_query 
 *
 * Description: This function is used to populate the SRV query structure.
 * INPUT       :pOrg_buff is a buffer having the data for parsing. 
 *             :org_buff_len is length of buffer.
 :pCurr_buff is a buffer pointing to current location.  
 * OUTPUT      :rec is a union will contain data of SRV query.
 *             :nBytes_qr will contain no. of bytes in particular answer record.     

 *             :pError is the error code.    

 * Return:     It will return DNSCL_SUCCESS if successful else return DNSCL_FAILURE.
 ********************************************************************************/

dnscl_return_t    arDnsClient_parse_Srv_query  (
        arDnsClient_global_data_st   *pDnscl_glb_data,
        dnscl_buff_ptr               pOrg_buff, 
        dnscl_uint32_t               org_buff_len,
        dnscl_buff_ptr               pCurr_buff,
        arDnsClient_rr_data_un       *rec,
        dnscl_uint32_t               *nBytes_qr,
        dnscl_error_t                *pError)
{
    dnscl_return_t ret_val = ARDNSCL_SUCCESS;
    dnscl_int32_t length = 0;
    dnscl_uint16_t rrlength = 0;
    dnscl_uint8_t target_name[ARDNSCL_MAX_STR_LEN];

    /* This line has been added just to remove compiler warning */
    pDnscl_glb_data = pDnscl_glb_data;

    *nBytes_qr = 0;

    /* Decoding RR data length */
    arDnsClient_decode_U16(&pCurr_buff , &rrlength);
    *nBytes_qr += ARDNSCL_SIZEOF(dnscl_uint16_t);

    /* Decoding priority field */
    arDnsClient_decode_U16(&pCurr_buff , &(rec->srv_rr_data.priority));
    *nBytes_qr += ARDNSCL_SIZEOF(dnscl_uint16_t);

    /* Decoding weight field */
    arDnsClient_decode_U16(&pCurr_buff , &(rec->srv_rr_data.weight));
    *nBytes_qr += ARDNSCL_SIZEOF(dnscl_uint16_t);

    /* Decoding port field */
    arDnsClient_decode_U16(&pCurr_buff , &(rec->srv_rr_data.port));
    *nBytes_qr += ARDNSCL_SIZEOF(dnscl_uint16_t);

    /* Decoding nameTarget field */
    length = ARDNSCL_DN_EXPAND(pOrg_buff, pOrg_buff + org_buff_len, pCurr_buff,
            (dnscl_int8_t *)target_name,ARDNSCL_SIZEOF(target_name));
    if (length < 0)
    {
        ARDNSCL_PRINT(pDnscl_glb_data,("nameTarget parameter is not present\n"));
        *pError =  ARDNSCL_ERROR_BAD_DNS_MESSAGE;
        return ARDNSCL_FAILURE;
    }
    rec->srv_rr_data.nameTarget.str_len = ARDNSCL_STRLEN((const dnscl_int8_t*)target_name);
    ARDNSCL_STRCPY( (dnscl_int8_t*)rec->srv_rr_data.nameTarget.str_val, (const dnscl_int8_t*)target_name);
    *nBytes_qr += length;

    return ret_val;
}

/*****************************************************************************
 * FUNCTION :   arDnsClient_parse_Naptr_query 
 *
 * Description: This function is used to populate the NAPTR query structure.
 * INPUT       :pOrg_buff is a buffer having the data for parsing. 
 *             :org_buff_len is length of buffer.
 :pCur_buff is a buffer pointing to current location.  
 * OUTPUT      :rec is a union will contain data of NAPTR query.
 *             :nBytes_qr will contain no. of bytes in particular answer record.     
 *             :pError is the error code.    

 * Return:     It will return DNSCL_SUCCESS if successful else return DNSCL_FAILURE.
 ********************************************************************************/

dnscl_return_t    arDnsClient_parse_Naptr_query  (
        arDnsClient_global_data_st   *pDnscl_glb_data,
        dnscl_buff_ptr               pOrg_buff, 
        dnscl_uint32_t               org_buff_len,
        dnscl_buff_ptr               pCurr_buff,
        arDnsClient_rr_data_un       *rec,
        dnscl_uint32_t               *nBytes_qr,
        dnscl_error_t                *pError)
{
    dnscl_return_t ret_val = ARDNSCL_SUCCESS;
    dnscl_uint32_t field_len = 0;
    dnscl_int32_t length = 0;
    dnscl_uint16_t rrlength = 0;
    dnscl_uint8_t replace_name[ARDNSCL_MAX_STR_LEN];
    dnscl_uint32_t str_length = ARDNSCL_MAX_SHORT_STR_LEN - 1;

    /* This line has been added just to remove compiler warning */
    pDnscl_glb_data = pDnscl_glb_data;

    *nBytes_qr = 0;

    /* Decoding RR data length */
    arDnsClient_decode_U16(&pCurr_buff , &rrlength);
    *nBytes_qr += ARDNSCL_SIZEOF(dnscl_uint16_t);

    /* Decoding order field */
    arDnsClient_decode_U16(&pCurr_buff,  &(rec->naptr_rr_data.order));
    *nBytes_qr += ARDNSCL_SIZEOF(dnscl_uint16_t);

    /* Decoding preference field */
    arDnsClient_decode_U16(&pCurr_buff,  &(rec->naptr_rr_data.preference));
    *nBytes_qr += ARDNSCL_SIZEOF(dnscl_uint16_t);

    field_len = *pCurr_buff++;

    /*to remove klockworks warning*/
    if(ARDNSCL_MAX_SHORT_STR_LEN >=  field_len) 
    {
        str_length = field_len;
    }

    rec->naptr_rr_data.flags.str_len = field_len;
    ARDNSCL_MEMCPY (rec->naptr_rr_data.flags.str_val,pCurr_buff,str_length);

    str_length = ARDNSCL_MAX_SHORT_STR_LEN - 1;

    pCurr_buff += field_len;
    *nBytes_qr += field_len + 1;


    field_len = *pCurr_buff++;

    /*to remove klockworks warning*/
    if(ARDNSCL_MAX_SHORT_STR_LEN >= field_len) 
    {
        str_length = field_len;
    }
    rec->naptr_rr_data.service.str_len = field_len;
    ARDNSCL_MEMCPY (rec->naptr_rr_data.service.str_val,pCurr_buff,str_length);

    str_length = ARDNSCL_MAX_SHORT_STR_LEN - 1;

    pCurr_buff += field_len;
    *nBytes_qr += field_len + 1;


    field_len = *pCurr_buff++;

    /*to remove klockworks warning*/
    if(ARDNSCL_MAX_SHORT_STR_LEN >= field_len) 
    {
        str_length = field_len;
    }
    rec->naptr_rr_data.regularExpr.str_len = field_len;
    ARDNSCL_MEMCPY (rec->naptr_rr_data.regularExpr.str_val,pCurr_buff,str_length);

    str_length = ARDNSCL_MAX_SHORT_STR_LEN - 1;

    pCurr_buff += field_len;
    *nBytes_qr += field_len + 1;


    /* Decoding replacement field */
    length = ARDNSCL_DN_EXPAND(pOrg_buff, pOrg_buff + org_buff_len, pCurr_buff, 
            (dnscl_int8_t *)replace_name,ARDNSCL_SIZEOF(replace_name));

    if ( 0 > length ) 
    {
        ARDNSCL_PRINT(pDnscl_glb_data,("replacement parameter is not present\n"));
        *pError =  ARDNSCL_ERROR_BAD_DNS_MESSAGE;
        return ARDNSCL_FAILURE;
    }
    rec->naptr_rr_data.replacement.str_len = ARDNSCL_STRLEN((const dnscl_int8_t*)replace_name);
    ARDNSCL_STRCPY((dnscl_int8_t*)rec->naptr_rr_data.replacement.str_val, (const dnscl_int8_t*)replace_name);
    *nBytes_qr += length;
    return ret_val;
}


/*****************************************************************************
 * FUNCTION :   arDnsClient_parse_dns_qsection
 *
 * Description: This function is used to parse the question section of DNS response.
 * INPUT       :pOrg_buff is a buffer having the data for parsing. 
 *             :org_buff_len is length of buffer.
 *             :pCurr_buff is a buffer pointing to current location.  
 *             :q_count is a count of number of questions.
 * OUTPUT      :nBytes_qn is no. of bytes in question section.
 *             :pError is the error code.  

 * Return:     It will return DNSCL_SUCCESS if successful else return DNSCL_FAILURE.
 ********************************************************************************/

dnscl_return_t   arDnsClient_parse_dns_qsection  (
        arDnsClient_global_data_st   *pDnscl_glb_data,
        dnscl_buff_ptr               pOrg_buff, 
        dnscl_uint32_t               org_buff_len,
        dnscl_buff_ptr               pCurr_buff,
        dnscl_uint16_t               q_count,
        dnscl_uint32_t               *nBytes_qn,  
        dnscl_error_t                *pError)
{
    dnscl_uint16_t qn_cnt;
    dnscl_int32_t length = 0;
    dnscl_uint8_t query_name[ARDNSCL_MAX_STR_LEN];
    dnscl_return_t ret_val = ARDNSCL_SUCCESS;

    /* This line has been added just to remove compiler warning */
    pDnscl_glb_data = pDnscl_glb_data;

    /* parse the query section of header. */
    for ( qn_cnt = 1; qn_cnt <= q_count; qn_cnt ++)
    {
        /* Calculate number of bytes of query name */      
        length = ARDNSCL_DN_EXPAND(pOrg_buff, pOrg_buff + org_buff_len, pCurr_buff, 
                (dnscl_int8_t *)query_name,ARDNSCL_SIZEOF(query_name));

        if ( 0 > length ) 
        {
            ARDNSCL_PRINT(pDnscl_glb_data,("Query name parameter is not present\n"))
            *pError =  ARDNSCL_ERROR_BAD_DNS_MESSAGE;
            return ARDNSCL_FAILURE;
        }
        *nBytes_qn += length;

        /* skip the Query type and class */
        *nBytes_qn = *nBytes_qn + ARDNSCL_QUERY_TYPE_SIZE + ARDNSCL_QUERY_CLASS_SIZE;
    }
    return ret_val;
}


/*****************************************************************************
 * FUNCTION : arDnsClient_decode_U16
 *
 * Description: This function is used to decode 2 byte data.
 *                            
 * Return:      Nothing.
 ********************************************************************************/
dnscl_void_t    arDnsClient_decode_U16  (
        dnscl_uint8_t          **pSrc,
        dnscl_uint16_t          *pOutput)

{
    dnscl_uint8_t *p_temp = *pSrc;
    dnscl_uint16_t   ntohs_data = 0;

    ARDNSCL_MEMCPY(&ntohs_data, p_temp, ARDNSCL_SIZEOF(dnscl_uint16_t));    
    p_temp += ARDNSCL_SIZEOF(dnscl_uint16_t);
    *pOutput = ARDNSCL_NTOHS(ntohs_data);
    *pSrc = p_temp;

    return;
}

/*****************************************************************************
 * FUNCTION : arDnsClient_decode_U32
 *
 * Description: This function is used to decode 4 byte data.
 *                            
 * Return:      Nothing.
 ********************************************************************************/
dnscl_void_t    arDnsClient_decode_U32  (
        dnscl_uint8_t          **pSrc,
        dnscl_uint32_t          *pOutput)
{
    dnscl_uint8_t *p_temp = *pSrc;
    dnscl_uint32_t   ntohl_data = 0;

    ARDNSCL_MEMCPY(&ntohl_data, p_temp, ARDNSCL_SIZEOF(dnscl_uint32_t));    
    p_temp += ARDNSCL_SIZEOF(dnscl_uint32_t);
    *pOutput = ARDNSCL_NTOHL(ntohl_data);
    *pSrc = p_temp;

    return;
}
#endif
