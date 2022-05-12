/******************************************************************************
*                                    
* FILE NAME    : arDnsClient_defs.h                
*                                    
* DESCRIPTION  : Contains the definitions for the literal definitions. 
*                                    
* Revision History :                            
*
*  DATE            NAME                 REFERENCE       REASON        
* ------         ------------           ---------     ----------
* 05-Feb-2009    Anuradha Gupta         None          Initial
* 06-Feb-2009    Anuradha Gupta         API Doc       literal definitions
* 28-May-2010    Alok Tiwari            SPR-20412     Modified the value of
*                                                     ARDNSCL_MAX_STR_LEN
*
* Copyright 2009, Aricent.                       
*                                    
*******************************************************************************/

#ifndef __ARDNSCLIENT_DEFS_H__
#define __ARDNSCLIENT_DEFS_H__

/* Prevent Name Mangling By C++ Compilers */

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

/*****************************************************************************/
/* Request API definitions */
#define 	ARDNSCL_API_SET_CONF_REQ         1
#define 	ARDNSCL_API_SET_TRACE_REQ        2
#define 	ARDNSCL_API_CLEAR_CACHE_REQ      3
#define 	ARDNSCL_API_SEND_QUERY_REQ       4
#define 	ARDNSCL_API_DEINIT_REQ           5

/* Response API definitions */
#define 	ARDNSCL_API_QUERY_RESP           6

/*****************************************************************************/

/*****************************************************************************/
/* Error codes definitions */
#define   ARDNSCL_ERROR_START                          0

/* This error code is returned when query type is invalid */    
#define   ARDNSCL_ERROR_INVALID_QUERY_TYPE             ARDNSCL_ERROR_START + 1

/* This error code is returned when invalid cache clear option is set. */    
#define   ARDNSCL_ERROR_INVALID_CACHE_OPTION           ARDNSCL_ERROR_START + 2

 /* This error code is returned when failure is returned while allocating 
  * memory */   
#define   ARDNSCL_ERROR_MEMORY_ALLOC_FAILURE           ARDNSCL_ERROR_START + 3

/* This error code is returned when failure is returned while freeing memory */    
#define   ARDNSCL_ERROR_MEMORY_FREE_FAILURE            ARDNSCL_ERROR_START + 4

/* This error code is returned when Domain/Host name specified is unknown */    
#define   ARDNSCL_ERROR_INVALID_DOMAIN_NAME            ARDNSCL_ERROR_START + 5

/* This error code is returned when domain/host name is valid but there is 
 * no data corresponding to domain name  */    
#define   ARDNSCL_ERROR_DATA_DOES_NOT_EXIST      ARDNSCL_ERROR_START + 6

/* This error code is returned when non-recoverable name server error occurred */    
#define  ARDNSCL_ERROR_NON_RECOVERABLE_SERVER_ERROR    ARDNSCL_ERROR_START + 7  

/* This error code is returned when some temporary error is occurred with 
 * DNS server, domain name can be resolved later */    
#define  ARDNSCL_ERROR_DNS_SERVER_TRY_LATER            ARDNSCL_ERROR_START + 8  

/* This error code is returned when server failure is returned */    
#define  ARDNSCL_ERROR_SERVER_ERROR                    ARDNSCL_ERROR_START + 9  

/* This error code is returned when message can not be sent over IPC */    
#define  ARDNSCL_ERROR_MESSAGE_SEND                    ARDNSCL_ERROR_START + 10  

/* This error code is returned when message can not be received from IPC */    
#define  ARDNSCL_ERROR_MESSAGE_RECV                    ARDNSCL_ERROR_START + 11   

/* This error is returned when corrupt response is received from DNS server */    
#define  ARDNSCL_ERROR_BAD_DNS_MESSAGE                 ARDNSCL_ERROR_START + 12  

/* This error is returned when NULL pointer is passed for Global data 
 * for DNS client */    
#define  ARDNSCL_ERROR_INVALID_GLB_DATA                ARDNSCL_ERROR_START + 13   

/* This error is returned when DNS Server is not reachable */    
#define  ARDNSCL_ERROR_SERVER_NOT_REACHABLE            ARDNSCL_ERROR_START + 14  

/* This error is returned in case of invalid trace data */    
#define  ARDNSCL_ERROR_DNS_INVALID_TRACE_DATA          ARDNSCL_ERROR_START + 15  

/* This error is returned in case of invalid query data */    
#define  ARDNSCL_ERROR_DNS_INVALID_QUERY_DATA          ARDNSCL_ERROR_START + 16  

/* This error is returned by server in case of format error in the query */
#define ARDNSCL_ERROR_QUERY_FORMAT_ERROR               ARDNSCL_ERROR_START + 17
    
/* This error is returned by server in case of the requested query is not
 * supported by server */
#define ARDNSCL_ERROR_SERVER_NOT_IMPLEMENTED           ARDNSCL_ERROR_START + 18
    
/* This error is returned by server has refused to handle the query */
#define ARDNSCL_ERROR_SERVER_REFUSED                   ARDNSCL_ERROR_START + 19
   
/* This error is returned in case of invalid message buffer */    
#define  ARDNSCL_ERROR_DNS_INVALID_MSG_BUFFER          ARDNSCL_ERROR_START + 20

/* This error is returned in case of invalid message buffer length */    
#define  ARDNSCL_ERROR_DNS_INVALID_BUFFER_LENGTH       ARDNSCL_ERROR_START + 21

/* This error is returned in case of invalid config data */    
#define  ARDNSCL_ERROR_DNS_INVALID_CONFIG_DATA         ARDNSCL_ERROR_START + 22

/* This error is returned in case of invalid IP Address */    
#define  ARDNSCL_ERROR_INVALID_IP_ADDRESS          	ARDNSCL_ERROR_START + 23

/* This error is returned in case of invalid Request data */
#define   ARDNSCL_ERROR_INVALID_CACHE_DATA         	ARDNSCL_ERROR_START + 24

/* This error is returned in case of socket creation failure */
#define   ARDNSCL_ERROR_SOCKET_CREATE         		ARDNSCL_ERROR_START + 25
  
/* This error is returned in case of set socket option failure */
#define   ARDNSCL_ERROR_SET_SOCKET_OPTION         	ARDNSCL_ERROR_START + 26

/* This error is returned in case of bind failure */
#define   ARDNSCL_ERROR_SOCKET_BIND         		ARDNSCL_ERROR_START + 27

/* This error is returned in case of not able to change server*/
#define   ARDNSCL_ERROR_UNABLE_TO_CHANGE_SERVER     ARDNSCL_ERROR_START + 28

#define   ARDNSCL_ERROR_END                         ARDNSCL_ERROR_START + 29

/*****************************************************************************/

/*****************************************************************************/
/* Supported Query types */
#define   ARDNSCL_QUERY_TYPE_A              1 
#define   ARDNSCL_QUERY_TYPE_AAAA           28
#define   ARDNSCL_QUERY_TYPE_SRV            33
#define   ARDNSCL_QUERY_TYPE_NAPTR          35
/*****************************************************************************/

/*****************************************************************************/
/* Supported services for SRV Query*/
#define   ARDNSCL_SERVICE_INVALID           0
#define   ARDNSCL_SERVICE_SIP               1
#define   ARDNSCL_SERVICE_SIPS              2

/* Supported protocols for SRV Query */    
#define  ARDNSCL_PROTOCOL_INVALID           0
#define  ARDNSCL_PROTOCOL_UDP               1
#define  ARDNSCL_PROTOCOL_TCP               2
/*****************************************************************************/

/*****************************************************************************/
/* Scaling parameters */
/* the maximun DNS RRs that have to be returned in case of multiple RRs */    
#define ARDNSCL_DEFAULT_DNS_RR          10
#define ARDNSCL_MAX_DNS_RR              10

/* the application can change this entry for any value less than max */    
#define ARDNSCL_NUM_DNS_RR               ARDNSCL_DEFAULT_DNS_RR 

/* Maximum number of Retries to DNS server */
#define ARDNSCL_MAX_DNS_RETRY            5

/* Dns Query Timeout Value in seconds */
#define ARDNSCL_DNS_QUERY_TIMEOUT        16

/* Dns Query Bind Port */
#define ARDNSCL_DNS_QUERY_BIND_PORT      62345

/* Default Etimated Query time in milliseconds */
#define ARDNSCL_DEFAULT_QUERY_TIME	 1000

/* the maximun caching entries that have to maintained */
#define ARDNSCL_DEFAULT_CACHE_DB_ENTRY   50
#define ARDNSCL_MAX_CACHE_DB_ENTRY       50

/* the application can change this entry for any value less than max */    
#define ARDNSCL_NUM_CACHE_DB_ENTRY       ARDNSCL_DEFAULT_CACHE_DB_ENTRY
/*****************************************************************************/
    
/*****************************************************************************/
/* Other constant definition */
#define   ARDNSCL_MAX_SHORT_STR_LEN      64
/* Maximum domain name length alowed in 128 + length of _sip._udp.*/ 
#define   ARDNSCL_MAX_STR_LEN            138

/* Buffer size for receing request on IPC Socket */
#define	  ARDNSCL_MAX_MSG_BUFFER	 10000

/* Maximum size of cache Datadbase */
#define	  ARDNSCL_MAX_CACHE_RECORDS	 50

/* Transport Type */
#define	  ARDNSCL_TRANSPORT_TYPE_UDP	 1

#define   ARDNSCL_MAX_BUFFER_LEN         10000        
#define   ARDNSCL_MAX_SEND_BUFF_SIZE     10000         
#define   ARDNSCL_MAX_RECV_BUFF_SIZE     10000
  

/*****************************************************************************/

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __ARDNSCLIENT_DEFS_H__ */



