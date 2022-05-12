/* Ensure Names are not mangled by C++ compilers */
#ifdef __cplusplus
extern "C" {
#endif 

/******************************************************************************
 ** FUNCTION:
 **	Has function declarations for Lookup portlayer structure and functions
 **				
 ******************************************************************************
 **
 ** FILENAME:  		sdf_lookupport.h
 **
 ** DESCRIPTION:	This file contains function declarations for Lookup 
 **					porlayer structure and functions
 **
 **  DATE        NAME                REFERENCE      REASON
 **  ----        ----                ---------      --------
 ** 10-Jun-05	R.Parthasarathi	 	 CREATION
 ** 29-Jan-2009 Alok Tiwari          ICF Rel-8.1    Changes done for 
 **                                                 flag ICF_UATK_PORT.This flag
 **                                                 filters the ICF related code/
 **                                                 changes from this file.
 ** 13-Mar-2009 Rajiv Kumar          IPTK Rel8.1    Merging CSR-1-6452321
 ** 24-Mar-2009 Kamal Ashraf         IPTK Rel8.2    Changes done for hssUA compilation.
 **
 ******************************************************************************
 **    		COPYRIGHT , Aricent, 2009
 ******************************************************************************/

#ifndef __SDF_LOOKUPPORT_H_
#define __SDF_LOOKUPPORT_H_

#ifdef SDF_LOOKUP
#include "arDnsClient_types.h"
#include "arDnsClient_defs.h"
#include "arDnsClient_api.h"


/* Merge: IPTK CSR 1-6452321: IPTK SPR 19216: UATK CSR 1-6589034 */
#define IPADDRSTRING	35

#ifndef ICF_UATK_PORT   

#define     SDF_BLK_SIZE_MAX     25600   
    
typedef struct
{
    /*Contains the IP address of DNS Client*/
    Sdf_ty_u8bit    dDnsClientIP[IPADDRSTRING];
    /*Contains the IP address of DNS CLIB.*/
    Sdf_ty_u8bit    dDnsClibIP[IPADDRSTRING];
    /*Contains the port of DNS Client.*/
    Sdf_ty_u16bit   dDnsClientPort;
    /*Contains the port of DNS CLIB.*/
    Sdf_ty_u16bit   dDnsClibPort;
    /*Contains the fd of DNS CLIB.DNS CLIENT will send the DNS query response 
      at this fd*/
    Sdf_ty_u32bit   dDnsClibFd;
    /*Specifies pointer to global data of arDnsClient. Application shall
      maintain the pointer and transparently pass it to interfaces of
      arDnsClient.*/
    Sdf_ty_pvoid    *pDnsClientGlbData;
}Sdf_st_dnsClientInfo;

#endif

#ifdef SDF_ASYNC_DNS
typedef struct
{
	/*
	 * Update DNS Record In Txn using these values
	 */
	Sdf_ty_u32bit IPRecCount;
	Sdf_ty_u32bit IPRecIndex;

	/*
	 * To be freed after sdf_fn_uaDnsQuery returns
	 */
	Sdf_ty_s8bit		 *pHost;
	SipUrl				 *pUrl;
	Sdf_st_transportInfo *pDestInfo;
	Sdf_st_destDetails   *pDestDetails;

	/*
	 * To be used for closing TCP sockets after sdf_fn_uaDnsQuery returns
	 */
	Sdf_ty_u16bit dOldPort;
	Sdf_ty_s8bit  dOldIp[Sdf_co_characterBufferSize];

	/*
	 * The type of Message - Request or Response
	 */
	en_SipMessageType   dMessageType;
	/*
	 * To know the Url Type after the DNS response is received
	 */
	Sdf_ty_UrlType           dUrlType;

	Sdf_ty_refCount		dRefCount;

} Sdf_st_asyncDNSContextInfo;
#endif /*SDF_ASYNC_DNS*/


/*****************************************************************************
 ** FUNCTION:	 sdf_fn_uaDnsQuery
 **
 ** DESCRIPTION: This function has the routine to query the DNS server 
 **
 **	PARAMETERS:
 ** 	pDomainName(IN)       	    : The given Domain Name
 **     pDnsEventContext(IN)        : Sdf_st_dnsEventContext Structure
 **	 	pErr(OUT)					: Error structure in case the function fails

 *****************************************************************************/
Sdf_ty_retVal sdf_fn_uaDnsQuery _ARGS_ ((Sdf_ty_s8bit *pDomainName,
		Sdf_st_dnsEventContext *pDnsEventContext,
		Sdf_st_error *pErr));

/*****************************************************************************
 ** FUNCTION:	  sdf_fn_uaAppendSrvRecord
 ** DESCRIPTION: The function use to update the Sorted SRV records into the 
 **		 Lookup module
 **	PARAMETERS:
 ** 	pslDnsRecords(IN)      : List of Dns Records
 **   pSrvRec(IN)            : Srv Record which is to append in the list.
 **	 	pErr(OUT)						   : Error structure in case the function fails
 **  
 *****************************************************************************/
Sdf_ty_retVal sdf_fn_uaAppendSrvRecord _ARGS_ ((Sdf_ty_slist *pslDnsRecords, 
		Sdf_st_dnsSrvRecord *pSrvRec, 
		Sdf_st_error *pErr));
/*****************************************************************************
 ** FUNCTION:	  sdf_fn_uaAppendIpRecord
 ** DESCRIPTION: The function use to update the Sorted SRV records into the 
 **		 Lookup module
 **	PARAMETERS:
 ** 	pslDnsRecords(IN)      : List of Dns Records
 **   pAddrString(IN)        : The IP Address.
 **	 	pErr(OUT)						   : Error structure in case the function fails
 **  
 *****************************************************************************/
Sdf_ty_retVal sdf_fn_uaAppendIpRecord _ARGS_((Sdf_ty_slist *pslDnsRecords,  
    Sdf_ty_s8bit *pAddrString, 
		Sdf_st_error *pErr));

/*****************************************************************************
 ** FUNCTION:    sdf_fn_uaFillQueryRespInDnsEvtCtx
 ** DESCRIPTION: The function is invoked by icf_process_queryresp_from_dnsclient
 **              for populating query result in DnsEventContext. 
 ** PARAMETERS:
 **   pDnsEventContext          Sdf_st_dnsEventContext Structure.
 **   pAnswerRecord             Answer record given by DNS CLIENT.
 **   query_type                Variable indicating type of query.
 **   p_query_flag              Variable indicating whether calling function
 **                             need to invoke further DNS QUERY.
 **   pErr                      Error variable.
 ** 
 *****************************************************************************/
Sdf_ty_retVal sdf_fn_uaFillQueryRespInDnsEvtCtx(
           Sdf_st_dnsEventContext      *pDnsEventContext, 
	       arDnsClient_dns_rr_list_st  *pAnswerRecord,
           Sdf_ty_u8bit                query_type,
           Sdf_ty_bool                 *p_query_flag,
           Sdf_st_error                *pErr);


Sdf_ty_u8bit sdf_ivk_uaExpandString(
                Sdf_ty_s8bit *p_src_str,
                Sdf_ty_s8bit **p_p_dest_str,
                Sdf_ty_u32bit dlength);

#ifndef ICF_UATK_PORT

/*****************************************************************************
 ** FUNCTION:	 sdf_fn_uaInitLookup
 ** DESCRIPTION: This function is used to initialize the lookup
 **              component.
 **  
 *****************************************************************************/
Sdf_ty_retVal  sdf_fn_uaInitLookup _ARGS_ ((void));   

/*****************************************************************************
 ** FUNCTION:	 sdf_fn_uaFreeLookup
 ** DESCRIPTION: This function is used to free the lookup
 **              component.
 **  
 *****************************************************************************/
Sdf_ty_retVal  sdf_fn_uaFreeLookup _ARGS_ ((void));

/********************************************************************************
 ** FUNCTION:    sdf_fn_uaGetDnsClientAddr(Sdf_st_dnsClientInfo  *pDnsClientInfo) 
 **
 ** DESCRIPTION: This function is used to get the DNS CLIENT and DNS CLIB IP/PORT.
 **              This is a porting layer function and application has to provide
 **              its own implementation for getting the DNS CLIENT and DNS CLIB 
 **              IP/PORT.Currently we are just harding coding the DNS CLIENT/CLIB
 **              IP/PORT.
 **
 ********************************************************************************/
Sdf_ty_retVal sdf_fn_uaGetDnsClientAddr(Sdf_st_dnsClientInfo
                                        *pDnsClientInfo);

/***********************************************************************************
 ** FUNCTION:    sdf_fn_uaInitDnsClientThread(Sdf_st_dnsClientInfo  *pDnsClientInfo) 
 **
 ** DESCRIPTION: This function is used initialised the DNS CLIENT thread.
 **
 ***********************************************************************************/
Sdf_ty_retVal sdf_fn_uaInitDnsClientThread(Sdf_st_dnsClientInfo
                                                    *pDnsClientInfo);

/********************************************************************************
 ** FUNCTION:    sdf_fn_uaAddDnsClibFdToAppFdSet(Sdf_st_dnsClientInfo 
 **                                                  *pDnsClientInfo) 
 **
 ** DESCRIPTION: This function is used to add the CLIB fd to application fd set, 
 **              so that DNS response can be received once send by DNS CLIENT.
 **              This is a porting layer function and application has to provide
 **              its own implementation for adding the DNS CLIB fd to its read 
 **              set. 
 **
 ********************************************************************************/
Sdf_ty_retVal sdf_fn_uaAddDnsClibFdToAppFdSet(Sdf_st_dnsClientInfo
                                                *pDnsClientInfo);

/***********************************************************************************
 ** FUNCTION:    sdf_fn_uaQueryRespProcs 
 **
 ** DESCRIPTION: This function is used for post DNS response processing.
 **              This is a porting layer function and application can specify
 **              its own implementation.The input parameters are 
 **              Sdf_st_dnsEventContext structure which is created while initiating
 **              the DNS query. bStatus parameter specify whether success response
 **              or failure response is received.In the porting layer function we 
 **              have provided the default implementation when success response for
 **              dns query is recived.For the failure leg application can specify
 **              its own implementation (for example try on any secondary address 
 **              or clear the call)
 **
 ***********************************************************************************/
void sdf_fn_uaQueryRespProcs(Sdf_st_dnsEventContext   *pDnsEventContext,
                             Sdf_ty_bool		      bStatus);

#endif

#ifdef SDF_ASYNC_DNS
/*****************************************************************************
 ** FUNCTION:	 sdf_fn_uaInitAsyncDnsContextInfo
 ** DESCRIPTION: Inits the Sdf_st_asyncDNSContextInfo structure for storing 
 **              context information
 **	PARAMETERS:
 **   pAsyncDnsCxtInfo(IN)   : Sdf_st_asyncDNSContextInfo Structure
 **   pErr(OUT)			  : Error structure in case the function fails 
 *****************************************************************************/
Sdf_ty_retVal  sdf_fn_uaInitAsyncDnsContextInfo _ARGS_ \
				((Sdf_st_asyncDNSContextInfo **pAsyncDnsCxtInfo, \
					  Sdf_st_error *pErr));

/******************************************************************
** FUNCTION:  sdf_ivk_uaFreeAsyncDnsContextInfo
** DESCRIPTION: This function frees Async DNS Event Info structure
** PARAMETERS:
**   pAsyncDnsCxtInfo(IN)   : Sdf_st_asyncDNSContextInfo Structure
******************************************************************/
Sdf_ty_retVal sdf_fn_uaFreeAsyncDnsContextInfo _ARGS_ \
				((Sdf_st_asyncDNSContextInfo *pAsyncDnsCxtInfo));

#endif /*SDF_ASYNC_DNS*/
#endif
#endif 

/* Ensure Names are not mangled by C++ compilers */
#ifdef __cplusplus
}
#endif
