/**********************************************************************************
 ** FUNCTION    : This file has all the Lookup dPortlayer related funtions
 **
 *********************************************************************************
 **
 ** FILENAME    : sdf_lookupdPort.c
 **
 ** DESCRIPTION    : This file has all the Lookup dPortlayer related funtions
 **
 ** DATE    NAME   						 REFERENCE    REASON
 ** ----    ----   						 ---------    --------
 ** 8-JUN-05  R.Parthasarathi          				  Creation
 ** 08-Nov-06 Lakshmi,Sunithi					      Modified for Async DNS
 **	                                                  release
 ** 7-Sep-05  Subhash                                 AresData/IPv6 handling 	
 ** 27-Mar-08 Shruti Thakral             SPR 18248,   Klocworks warning removal
 **                                          18302,
 **                                          18307
 ** 22-Apr-08 Alok Tiwari                SPR 18456    Klocworks warning removal
 ** 24-Jun-2008  Anurag Khare           SPR 18716    Ack was not getting send if 200 OK/home/rajiv/mainline/ 
 **                                                  contains   Domain name as a contact
 ** 30-JUn-2008  Anurag Khare           SPR 18684    CSR 1-6085731 Merge
 ** 12-Nov-2008  Tarun Gupta            SPR 19189    Merged Rel 7.1.2 SPR 18872
 ** 25-Nov-2008  Rajiv Kumar            SPR 19188    Merged Rel 7.1.2 SPR
 **                                                  18913 
 ** 08-Dec-2008  Anurag Khare           SPR 19218    CSR 1-6486039 Merge
 ** 10-Dec-2008  Tarun Gupta            SPR 19218    Merged CSR 1-5826918
 ** 29-Jan-200   Alok Tiwari      ICF Rel-8.1       Changes done for 
 **                               flag ICF_UATK_PORT.This flag filters
 **                               the ICF related code/changes from this file.
 ** 07-Feb-2009  Alok Tiwari      ICF Rel-8.1   Code review comments incorporated
 ** 18-Feb-2009  Rajiv Kumar       IPTK Rel 8.1,   Klocworks warning removal
 ** 02-Mar-2009  Rajiv Kumar       IPTK Rel 8.1,   Code Changes done for
 **                                                replacing "ARES" with
 **                                                "arDnsClient"
 ** 13-Mar-2009  Rajiv Kumar       IPTK Rel8.1     Merging CSR-1-6452321 
 ** 18-Mar-2009  Rajiv Kumar       IPTK Rel8.1     UT Defeact Fix 
 ** 18-Mar-2009  Rajiv Kumar       IPTK Rel8.1     UT Defect Fix
 ** 08-Apr-2009  Alok Tiwari       IPTK Rel8.2     SPR-19432 Fixed.    
 ** 01-May-2009  Anuradha Gupta    SPR 19443       Compilation warning resolved.  
 ** 05-Apr-2009  Rajiv Kumar       IPTK Rel8.2     Fix for SPR: 19593
 ** 02-Jun-2009  Kamal Ashraf      SPR 19672       DNS Issue resolved
 ** 08-jun-2009  Anuradha Gupta  SPR 19737    Klocworks fix
 ** 11-Jun-2009 Ashutosh Mohan   SPR 19737    Changes done for GCC 4.1 warnings removal
 ** 20-Aug-2009 Ashutosh Mohan   SPR 19927          GCC warnings removal
 ** 29-Sep-2009  Anuradha Gupta    SPR 20080       Fix for GCC 4.1 warnings in standalone
 **                                                stack compilation
 ** 27-Oct-2009  Rajiv Kumar       SPR 20192       Fix For SPR:20192
 ** 22-Apr-2010  Alok Tiwari       SPR 20385       Fix for CSR 1-8538021
 ** 24-May-2010  Preksha           SPR 20412       CSR 1-7953063 Merged
 ** 25-May-2010  Preksha           SPR 20412       CSR 1-8038197 Merged
 ** 
 *******************************************************************************
 **      COPYRIGHT , Aricent, 2009
 ******************************************************************************/
#ifdef SDF_LOOKUP
#include "ALheader.h"
#include "sdf_struct.h"
#include "sdf.h"
#include "sdf_network.h"
#include "sdf_accessor.h"
#include "sdf_init.h"
#include "sdf_internal.h"
#include "sdf_sdpinternal.h"
#include "sdf_reqresp.h"
#include "sdf_debug.h"
#include "sdf_trace.h"
#include "sdf_hash.h"
#include "sdf_portlayer.h"
#include "sdf_callbacks.h"
#include "sdf_lookupport.h"
#include "sdf_dnsclient.h"
#include "sdf_lookup.h"
#include "sdf_common.h"
#ifdef SDF_WINDOWS
#include "mysysdefines.h"
#else
#if defined(ICF_PORT_VXWORKS) || defined(SDF_VXWORKS)
#include "resolv/nameser.h"
#else
#include <arpa/nameser.h> 
#endif  /*defined(ICF_PORT_VXWORKS) || defined(SDF_VXWORKS)*/

#ifdef SDF_ASYNC_DNS
#include <math.h>
#endif/*SDF_ASYNC_DNS*/
#endif/*SDF_WINDOWS*/

#if defined(ICF_PORT_VXWORKS) || defined(SDF_VXWORKS)
#include "resolv/nameser.h"
#else
#ifndef ICF_PORT_WINDOWS
#include <arpa/nameser.h>
#endif
#endif

/* Change for vxWork Compilation */
 
#ifdef SDF_THREAD_SAFE 
#if defined(SDF_WINDOWS) || defined(SDF_WINCE)
Sdf_st_ThreadKey  dThreadKey;
#else
pthread_key_t  dThreadKey;
#endif
#endif

#ifdef SDF_ASYNC_DNS
extern Sdf_st_fdSetInfo  dGlbAresFdSet;
extern Sdf_st_fdSetInfo	dGlbFdSet;
Sdf_st_hash dGlbAresDataHash;
#endif

#ifndef ICF_UATK_PORT
#include <pthread.h>

/*Global Variable need to be define by application*/
extern Sdf_st_dnsClientInfo dDnsClientInfo;
extern Sdf_st_fdSetInfo	dGlbFdSet;
#endif

/*ICF SSA module inclusion*/

#ifdef ICF_UATK_PORT

#include "icf_feature_flags.h"
#include "icf_port_struct.h"
#include "icf_common_types.h"
#include "icf_common_prototypes.h"
#include "icf_port_prototypes.h"
#include "icf_port_dns_client.h"
#include "icf_ssa_types.h"
#include "icf_macro.h"

extern icf_return_t icf_ssa_dns_get_ip_from_fqdn(
        IN     icf_uint8_t        *p_fqdn,
        OUT    icf_dns_buffer_st  **p_dns_record,
        OUT    icf_error_t        *p_error);

extern icf_void_t  icf_ssa_init_pdb(
        icf_ssa_pdb_st  *p_ssa_pdb,
        icf_glb_pdb_st  *p_glb_pdb);

extern icf_return_t  icf_ssa_get_dns_srv_records (
        icf_ssa_pdb_st*          p_ssa_pdb,
        Sdf_st_dnsEventContext*  pDnsEventContext);

extern icf_return_t icf_ssa_dns_query_request (
        icf_ssa_pdb_st           *p_ssa_pdb,
        Sdf_st_overlapTransInfo  *pOverlapTrans,
        icf_uint8_t              *p_domain_name,
        icf_void_t               *p_ans_list,
        Sdf_ty_DnsRecordType      dns_record_type);

extern icf_void_t icf_ssa_post_dns_resp_processing(
        Sdf_st_dnsEventContext  *p_dns_evt_ctxt, 
        Sdf_ty_bool             dns_resp_flag);

extern icf_uint8_t icf_ssa_get_transport_mode(
        Sdf_st_callObject *pCallObj,
        icf_boolean_t tls_present,
        icf_boolean_t tcp_present,
        icf_boolean_t udp_present);

extern icf_return_t icf_ssa_dns_start_dns_resp_timer(
        INOUT icf_ssa_ctxt_st      *p_ssa_ctxt,
        IN  Sdf_st_dnsEventContext *p_dns_evt_ctxt, 
        IN  icf_duration_t         timer_dur,
        OUT icf_error_t            *p_ecode);

extern icf_void_t icf_ssa_stat_dns_type(
        IN icf_uint32_t dns_query_type);

extern icf_return_t icf_ssa_dns_stop_dns_resp_timer(
        INOUT	icf_ssa_ctxt_st *p_ssa_ctxt,
        OUT     icf_error_t    *p_ecode);

extern icf_void_t icf_ssa_stat_dns_resp_type(
        IN     icf_uint32_t       dns_query_type,
        IN     icf_boolean_t      dns_query_resp);

extern icf_return_t icf_ssa_validate_dnsEventContext(
	icf_void_t   *pDnsEventContext);

extern icf_glb_pdb_st *p_persistent_glb_pdb;

#endif

#ifdef ICF_UATK_PORT

/*****************************************************************************
 ** FUNCTION:   sdf_fn_uaDnsQuery
 **
 ** DESCRIPTION: This function has the routine to query the DNS server 
 **
 **	PARAMETERS:
 ** 	pDomainName(IN)       	    : The given Domain Name
 **     pDnsEventContext(IN)        : Sdf_st_dnsEventContext Structure
 **	 	pErr(OUT)					: Error structure in case the function fail
 *****************************************************************************/
Sdf_ty_retVal sdf_fn_uaDnsQuery (
        Sdf_ty_s8bit  *pDomainName,
        Sdf_st_dnsEventContext   *pDnsEventContext,
        Sdf_st_error             *pErr)
{
#ifdef SDF_ASYNC_DNS
    Sdf_ty_bool                dIsToHashAdd  = Sdf_co_false;
    Sdf_ty_u32bit              i = 0, dTempFd = 0;
    Sdf_ty_fdSet 		       dReadfdSet;
    Sdf_st_asyncDNSContextInfo *pAsyncDnsCxtInfo = Sdf_co_null;
#ifdef SDF_THREAD_SAFE
    Sdf_ty_threadId 		     dCallingThread = 0;
#endif
#endif /*SDF_ASYNCD_DNS*/

    icf_global_port_info_st	   *p_glb_port_info = ICF_NULL; 
    icf_error_t				   err = ICF_ERROR_NONE;
    icf_dns_buffer_st		   *p_dns_record = ICF_NULL;
    Sdf_st_transportInfo 	   *p_dest_Info = Sdf_co_null;	    
    icf_return_t               ret_val = ICF_FAILURE;
    icf_return_t               dummy_ret_val = ICF_FAILURE;
    Sdf_st_overlapTransInfo    *pOverlapTransInfo = Sdf_co_null;
    Sdf_st_transaction	       *pTransaction	= Sdf_co_null;
    icf_ssa_ctxt_st            *p_ssa_ctxt = ICF_NULL;  
    /*  temporary variable that will get the servere address */
    icf_dns_server_data_st     *p_dns_server_data = ICF_NULL;

    /*Variable for DNS CLIENT*/
    Sdf_ty_pvoid               *pDnscl_glb_data = Sdf_co_null;
    arDnsClient_conf_st        *pConfigData = Sdf_co_null; 
    dnscl_error_t              dns_err = 0; 
    dnscl_return_t             dnscl_ret_val = ARDNSCL_SUCCESS;
    arDnsClient_query_st       *pQueryData = Sdf_co_null;
    dnscl_uint8_t              service_name = ARDNSCL_SERVICE_INVALID;
    dnscl_uint8_t              protocol_name = ARDNSCL_PROTOCOL_INVALID;

#ifdef SDF_ASYNC_DNS
    if((ICF_NULL == pDnsEventContext) ||
            (ICF_NULL == pDnsEventContext->pAppData))
    {
        ICF_PRINT(("\n[UATK-Port]: pDnsEventContext or pDnsEventContext->pAppData is NULL"))
        return  Sdf_co_fail;
    }
    pAsyncDnsCxtInfo = (Sdf_st_asyncDNSContextInfo*)pDnsEventContext->pAppData;
#endif

    if((ICF_NULL == p_persistent_glb_pdb) ||
            (ICF_NULL == p_persistent_glb_pdb->p_glb_data) ||
            (ICF_NULL == p_persistent_glb_pdb->p_glb_data->p_glb_port_info))
    {
        ICF_PRINT(((icf_uint8_t*)"\n[UATK-Port]: p_glb_data or p_glb_port_info is NULL"))
        return  Sdf_co_fail;
    }

    p_glb_port_info = p_persistent_glb_pdb->p_glb_data->p_glb_port_info;

    /*Get the global data for DNS CLIENT form glb_port_info*/
    pDnscl_glb_data = p_glb_port_info->pDnsclient_glb_data;

    if(ICF_NULL == pDnscl_glb_data)
    {
        ICF_PRINT(((icf_uint8_t*)"\n[UATK-Port]: pDnscl_glb_data is NULL"))
        return  Sdf_co_fail;
    }  
    if(ICF_NULL == pDnsEventContext)
    {
        ICF_PRINT(((icf_uint8_t*)"\n[UATK-Port]: pDnsEventContext is NULL"))
        return  Sdf_co_fail;
    }

    /* Code added for DNS buffer- if the entry is found in the DNS buffer
       then dont  send the DNS query */
    if (sdf_ivk_uaGetDestInfoFromTransaction(pDnsEventContext->pTransaction,
                pDnsEventContext->pOverlappedTransaction,
                &p_dest_Info, 
                pErr) == Sdf_co_fail)
    {
        return Sdf_co_fail;
    }

    /*Fetch icf_ssa_ctxt_st from pDnsEventContext*/
    if((ICF_NULL != pDnsEventContext->pCallObj) &&
            (ICF_NULL != pDnsEventContext->pCallObj->pAppData) &&
            (ICF_NULL != pDnsEventContext->pCallObj->pAppData->pData))
    {
        p_ssa_ctxt = (icf_ssa_ctxt_st*)(pDnsEventContext->pCallObj-> \
                pAppData->pData);
    }
    else
    {
        ICF_PRINT(((icf_uint8_t*)"\n[UATK-Port]: Either call object or SSA context is NULL"))
        return Sdf_co_fail;
    }

    /*Free the destination Info from Transaction or OverlapTransaction*/
    if(Sdf_co_null != pDnsEventContext->pTransaction)
    {
        (void)sdf_ivk_uaFreeTransportInfo(pDnsEventContext->pTransaction->pDestInfo);
    }
    else if(Sdf_co_null != pDnsEventContext->pOverlappedTransaction)
    {
        (void)sdf_ivk_uaFreeTransportInfo(pDnsEventContext->pOverlappedTransaction->pDestInfo);
    } 

    /* SPR 18872: if DNS query is ongoing then no need to check the database
     * as this request is for AA query after SRV query and it should happen
     */ 
    if(!(p_ssa_ctxt->bitmask_1 & ICF_SSA_DNS_QUERY_ONGOING))
    {
        ret_val = icf_ssa_dns_get_ip_from_fqdn((icf_uint8_t *)pDomainName,
                &p_dns_record,  
                &err); 
    }

    /*  If any contact ip is present in ssa_ctxt then remove it because a new
        DNS query is going to occur */
    if (ICF_FAILURE == ret_val)
    {
        if (p_ssa_ctxt->bitmask & ICF_SSA_CONTACT_IP_PRESENT) 
        {
            if (ICF_NULL != p_ssa_ctxt->p_dns_ip_addr)
            {
                ICF_MEMFREE(p_persistent_glb_pdb,
                        p_ssa_ctxt->p_dns_ip_addr,
                        ICF_MEM_COMMON,
                        &err, dummy_ret_val)
                        p_ssa_ctxt->bitmask &= ~ICF_SSA_CONTACT_IP_PRESENT;
            }
        }

        /* SPR 19672 : In case DNS Query is issued and the entry is not
         * available in the buffer then reset the following bitmask.
         * This bitmaks is set when the resolved entry is picked from the buffer
         * but when a DNS query is required then it should be reset as well.
         */  
         p_ssa_ctxt->bitmask &= ~ICF_DNS_RECORD_FETCHED; 
    } 
    if(ICF_SUCCESS == ret_val)
    {
        /* found the DNS entry with active IP address assign the DNS address
           and the port*/
        if (p_dest_Info->pIp != Sdf_co_null)
        {
            (Sdf_ty_void)sdf_memfree(Sdf_mc_callHandlerMemId,(Sdf_ty_pvoid*)&\
                                     (p_dest_Info->pIp),pErr);
        }
        /* SPR 19189 */
        if(ICF_NULL == p_dns_record)
        {
            ICF_PRINT(((icf_uint8_t*)"\n[UATK-Port]: p_dns_record is NULL"))
            return  Sdf_co_fail;
        }

        Sdf_mc_strdup(p_dest_Info->pIp, (Sdf_ty_s8bit*)p_dns_record->ip_address);
        p_ssa_ctxt->transport = p_dns_record->transport_mode;
	/*If same domain is obtained but with different port then 
	  old port received from cache should not be be updated in p_dest_info*/    
	if(p_dest_Info->dPort == 0 && p_dns_record->port != 0 )

        {
            p_dest_Info->dPort = p_dns_record->port;
        }
        else if (p_dest_Info->dPort == 0 && p_dns_record->port == 0)
        {
            p_dest_Info->dPort = Sdf_mc_defaultSipPort;
        }
        /*  Copy the transport mode in dest info */
        if (ICF_TRANSPORT_TYPE_UDP == p_dns_record->transport_mode)
        { 
            p_dest_Info->dScheme = Sdf_en_protoUdp;

            /* Reset ICF_SSA_EXPLICIT_CONNECT_REQ in case of UDP*/
            p_ssa_ctxt->bitmask &= ~ICF_SSA_EXPLICIT_CONNECT_REQ;
        }
        else if ((ICF_TRANSPORT_TYPE_TCP == p_dns_record->transport_mode) ||
                (ICF_TRANSPORT_MODE_BOTH == p_dns_record->transport_mode))
        {
            p_dest_Info->dScheme = Sdf_en_protoTcp;
            p_ssa_ctxt->bitmask |= ICF_SSA_TCP_TRANSPORT;
        }
#ifdef SDF_TLS
        else if (ICF_TRANSPORT_TYPE_TLS == p_dns_record->transport_mode)
        {
            p_dest_Info->dScheme = Sdf_en_protoTls;
            p_ssa_ctxt->bitmask |= ICF_SSA_TLS_TRANSPORT;
        }
#endif 
      
        pOverlapTransInfo = pDnsEventContext->pOverlappedTransaction;
        
        pTransaction = pDnsEventContext->pTransaction; 
        
        if(Sdf_co_null == pOverlapTransInfo)
        {
            if (Sdf_co_null == pTransaction)
            {
                return Sdf_co_fail;
            }	
#ifdef SDF_THREAD_SAFE
            sdf_fn_uaLockMutex(0,  &pTransaction->dTxnDnsStateMutex, 0);  	
#endif
            pTransaction->dTxnDnsState = Sdf_en_dnsIpUsed; 
#ifdef SDF_THREAD_SAFE
            sdf_fn_uaUnlockMutex(0,  &pTransaction->dTxnDnsStateMutex);	
#endif
        }	
        else
        {
#ifdef SDF_THREAD_SAFE
            sdf_fn_uaLockMutex(0,  &pOverlapTransInfo->dTxnDnsStateMutex, 0);  	
#endif
            pOverlapTransInfo->dTxnDnsState = Sdf_en_dnsIpUsed ; 
#ifdef SDF_THREAD_SAFE
            sdf_fn_uaUnlockMutex(0,  &pOverlapTransInfo->dTxnDnsStateMutex);	
#endif
        }
        
        /* this is to keep the information that the record is fethced from DNS Buffer */
        /* this will help to remove the dns entry if no response is received */
        if((ICF_NULL != pDnsEventContext->pCallObj) &&
                (ICF_NULL != pDnsEventContext->pCallObj->pAppData) &&
                (ICF_NULL != pDnsEventContext->pCallObj->pAppData->pData))
        {
            icf_ssa_ctxt_st      *p_temp_ssa_ctxt = ICF_NULL; 
            p_temp_ssa_ctxt = (icf_ssa_ctxt_st*)(pDnsEventContext->pCallObj->pAppData->pData);
            p_temp_ssa_ctxt->bitmask |= ICF_DNS_RECORD_FETCHED;
            Sdf_mc_strcpy(p_temp_ssa_ctxt->fqdn.str, pDomainName);
            /* Merging of CSR 1-6486039: Set the DNS FQDN Entry present bitmask */
            p_temp_ssa_ctxt->bitmask |= ICF_SSA_DNS_FQDN_PRESENT;

        }
        return Sdf_co_success;
    }
    else
    {
        /* No DNS entry in the buffer store the FQDN in ssa context
           it will be used when response is received to mark the active IP */
        icf_ssa_ctxt_st      *p_temp_ssa_ctxt = ICF_NULL; 
        if((ICF_NULL != pDnsEventContext->pCallObj) &&
                (ICF_NULL != pDnsEventContext->pCallObj->pAppData) &&
                (ICF_NULL != pDnsEventContext->pCallObj->pAppData->pData))
        {
            p_temp_ssa_ctxt = (icf_ssa_ctxt_st*)(pDnsEventContext->pCallObj->pAppData->pData);
            /* FQDN will be stored in ssa_ctxt only for the domain name got once,
             * for the subsequent responses containing domain names from DNS server, 
             * FQDN will not be overwritten. */
            if (!(p_temp_ssa_ctxt->bitmask & ICF_SSA_DNS_FQDN_PRESENT))
                Sdf_mc_strcpy(p_temp_ssa_ctxt->fqdn.str, pDomainName);	  
            p_temp_ssa_ctxt->bitmask |= ICF_SSA_DNS_FQDN_PRESENT;
        }
    }
    /* end of the code for DNS buffer */

    /*If the value return by function icf_port_isnameserver_pres is
      True then it indicates that application has configures the 
      Primary or secondary DNS server address and we need to used the
      Same as DNS server address in DNS CLIENT.( For Vxwork only :The
      configuration of primary and secondary DNS Server at run time is
      also govern by the retun value of this function*/

      if(ICF_TRUE == icf_port_is_nameserver_pres())
      {
          /*Get the DNS server data for fetching IP/PORT*/
          p_dns_server_data = icf_port_dns_server_data_init(p_glb_port_info,&err);

          /*If p_dns_server_data is NULL return failure*/
          if(ICF_NULL == p_dns_server_data)
          { 
#ifdef SDF_ERROR
              sdf_fn_dumpError((Sdf_ty_s8bit *)"\n ALARM DNS server data is"
                      "not fetched.Would not be supporing DNS query");
#endif
              return Sdf_co_fail;
          }
          else
          {
              /*Populate the DNS server address in config data of DNS 
                CLIENT*/
              pConfigData = (arDnsClient_conf_st *)sdf_memget(Sdf_mc_initMemId,\
                                         sizeof(arDnsClient_conf_st), pErr);
              
              if (Sdf_co_null == pConfigData)
              {
                  ICF_PRINT(((icf_uint8_t*)"\n[UATK-Port]: Memory allocation failure for"
                              "config data of DNS CLIENT"))
                  return Sdf_co_fail;
              }
              
              Sdf_mc_strcpy(pConfigData->dns_server_addr.ipAddress.str_val,\
                                  p_dns_server_data->dns_server_ip);
              
              pConfigData->dns_server_addr.ipAddress.str_len =
                  Sdf_mc_strlen((Sdf_ty_s8bit *)pConfigData->dns_server_addr.ipAddress.str_val);

              pConfigData->dns_server_addr.port = p_dns_server_data->\
                                                   dns_server_port;

              /*Invoke the DNS CLIENT API to configure the DNS server 
                address*/
              dnscl_ret_val = arDnsClient_set_conf(pDnscl_glb_data, 
                      pConfigData, &dns_err);

              /*Free the memory allocated for config data of DNS CLIENT*/
              if (Sdf_co_null != pConfigData)
              {
                  (Sdf_ty_void)sdf_memfree(Sdf_mc_initMemId,
                                           (Sdf_ty_pvoid*)&pConfigData, pErr);   
              }

              if (ARDNSCL_FAILURE == dnscl_ret_val)
              {
#ifdef SDF_ERROR
                  sdf_fn_dumpError((Sdf_ty_s8bit *)"\n DNS server address can not be"
                          "configured in DNS CLIENT.Would not be supporing DNS query");
#endif
                  return Sdf_co_fail;  
              }
          }
      }

      /* Getting the Record Type from Transaction's DNS Event Context  */
      if (pDnsEventContext->dDnsRecordType == Sdf_en_IPAddress)
#ifndef SDF_IPV6
          pDnsEventContext->dDnsRecordType = Sdf_en_IPv4Address;

      if ((pDnsEventContext->pTransaction != Sdf_co_null) && \
              (pDnsEventContext->pTransaction->dDnsRecordType == \
               Sdf_en_IPAddress))

          pDnsEventContext->pTransaction->dDnsRecordType = Sdf_en_IPv4Address;
						
      else if ((pDnsEventContext->pOverlappedTransaction != Sdf_co_null) && \
               (pDnsEventContext->pOverlappedTransaction->dDnsRecordType == \
               Sdf_en_IPAddress))
								
          pDnsEventContext->pOverlappedTransaction->dDnsRecordType = Sdf_en_IPv4Address;

#else
      pDnsEventContext->dDnsRecordType = Sdf_en_IPv6Address;
					
      if ((pDnsEventContext->pTransaction != Sdf_co_null) && \
              (pDnsEventContext->pTransaction->dDnsRecordType == Sdf_en_IPAddress))
							
          pDnsEventContext->pTransaction->dDnsRecordType = Sdf_en_IPv6Address;
								
      else if ((pDnsEventContext->pOverlappedTransaction != Sdf_co_null) && \
              (pDnsEventContext->pOverlappedTransaction->dDnsRecordType == Sdf_en_IPAddress))
						
          pDnsEventContext->pOverlappedTransaction->dDnsRecordType = Sdf_en_IPv6Address;
#endif

#ifdef ICF_STAT_ENABLE
      icf_ssa_stat_dns_type(pDnsEventContext->dDnsRecordType);
#endif

      /*Populate the Query data structure of DNS CLIENT*/
      pQueryData = (arDnsClient_query_st *)sdf_memget(Sdf_mc_initMemId,
              sizeof(arDnsClient_query_st),pErr);	

      if (Sdf_co_null == pQueryData)
      {
#ifdef SDF_ERROR
          sdf_fn_dumpError((Sdf_ty_s8bit *)"\n Memory allocation failure for "
                  "query data of DNS CLIENT.Would not be supporing DNS query");
#endif
          return Sdf_co_fail;  
      }      

      switch(pDnsEventContext->dDnsRecordType)
      {
          case Sdf_en_NAPTRrecord:
          {
              pQueryData->query_type = ARDNSCL_QUERY_TYPE_NAPTR;
              break;
          }
          case Sdf_en_SRVrecord:
          {
              pQueryData->query_type = ARDNSCL_QUERY_TYPE_SRV;

              switch (pDnsEventContext->dProtocol)
              {
                  case Sdf_en_protoAny:
                      protocol_name= ARDNSCL_PROTOCOL_UDP;
                      service_name = ARDNSCL_SERVICE_SIP;
                      break;
#ifdef SDF_TLS
                  case Sdf_en_protoTls:
                      protocol_name= ARDNSCL_PROTOCOL_TCP;
                      service_name = ARDNSCL_SERVICE_SIPS;
                      break;
#endif
                  case Sdf_en_protoTcp: 
                      protocol_name= ARDNSCL_PROTOCOL_TCP;
                      service_name = ARDNSCL_SERVICE_SIP;
                      break;
                  default:
                      protocol_name= ARDNSCL_PROTOCOL_UDP;
                      service_name = ARDNSCL_SERVICE_SIP;
              }
              break;
          }	
#ifndef SDF_IPV6
          case Sdf_en_IPv4Address:
          {   
              pQueryData->query_type = ARDNSCL_QUERY_TYPE_A;
              break;
          }			
#else		
          case Sdf_en_IPv6Address:
          {
              pQueryData->query_type = ARDNSCL_QUERY_TYPE_AAAA;
              break;
          }		 
#endif		
          default:
          {
              (Sdf_ty_void)sdf_memfree(Sdf_mc_initMemId,
                                       (Sdf_ty_pvoid*)&pQueryData,pErr);            
              return Sdf_co_fail;
          }
      }

      /* Increment the reference counts in DnsEvent context */
      HSS_LOCKEDINCREF(pDnsEventContext->dRefCount);

      /*Populate the application data of DNS CLIENT with pDnsEventContext*/

      pQueryData->pApp_query_ctx = (dnscl_void_t *)(pDnsEventContext);
      
      Sdf_mc_strcpy(pQueryData->query_string.str_val, pDomainName);

      pQueryData->query_string.str_len = Sdf_mc_strlen((Sdf_ty_s8bit *)pQueryData->query_string.str_val);

      /*Issue a DNS query from DNS Client*/
      dnscl_ret_val = arDnsClient_send_query(pDnscl_glb_data, 
              pQueryData, service_name, protocol_name,&dns_err);

      if (ARDNSCL_FAILURE == dnscl_ret_val)
      {
#ifdef SDF_ERROR
          sdf_fn_dumpError((Sdf_ty_s8bit *)"\n DNS CLIENT return failure for DNS query");
#endif
          (Sdf_ty_void)sdf_memfree(Sdf_mc_initMemId,
                                   (Sdf_ty_pvoid*)&pQueryData,pErr);          
          return Sdf_co_fail;  
      }
      else
      {
          /*Free the memory allocated to Query data*/
          (Sdf_ty_void)sdf_memfree(Sdf_mc_initMemId,
                                   (Sdf_ty_pvoid*)&pQueryData,pErr);          
      }

      /*Populate the DNS Server data in DNS client*/
      if((ICF_NULL != pDnsEventContext->pCallObj) &&
              (ICF_NULL != pDnsEventContext->pCallObj->pAppData) &&
              (ICF_NULL != pDnsEventContext->pCallObj->pAppData->pData))
      {
          p_ssa_ctxt = (icf_ssa_ctxt_st*)(pDnsEventContext->pCallObj->
                  pAppData->pData);
          p_ssa_ctxt->p_dns_server_data = p_dns_server_data;
          
          /* Fix for SPR:19593*/
          /* Store the dns event context pointer in ssa context. This is used
           * for validating the response for DNS CLIENT.*/
          p_ssa_ctxt->p_dns_event_context = (icf_void_t *)pDnsEventContext;
          
          /*If the DNS response time is invalid start the same*/
          if(ICF_TIMER_ID_INVALID == p_ssa_ctxt->dns_resp_timer_id)
          {
              ret_val =
                  icf_ssa_dns_start_dns_resp_timer(p_ssa_ctxt,pDnsEventContext,
                          p_ssa_ctxt->dns_retrans_timer,
                          &err);
              if (ICF_FAILURE == ret_val)
              {
                  /* Do not decrement the refcount for pDnsEventContext and
                     return failure.
                     This is done because even if there is a failure in
                     starting the dns response timer but still we have issued
                     the DNS query to DNS CLIENT in the above code.In the Dns
                     query structure of DNS CLIENT pDnsEventContext is passed
                     as an application data,which need to be access in
                     function sdf_fn_uaProcessQueryRespFromDnsClient.Now when the
                     DNS CLIENT respond back with the result ie success or failure
                     there will be FMR for the pDnsEventContext in function
                     sdf_fn_uaProcessQueryRespFromDnsClient.*/
                  /*HSS_LOCKEDDECREF(pDnsEventContext->dRefCount);*/
                  ICF_PRINT(((icf_uint8_t*)"\n Failure in starting DNS Resp Timer. \n"));
                  /*return ret_val;*/
              }
          }
      }/* end if NULL != pDnsEventContext->pCallObj->pAppData->pData */
  
      pErr->errCode = Sdf_en_noUaError;
      return Sdf_co_success;
}

#endif

/*****************************************************************************
 ** FUNCTION:    sdf_fn_uaAppendIpRecord 
 ** DESCRIPTION: The function use to update the Sorted SRV records into the 
 **              Lookup module
 **
 **	PARAMETERS:
 ** 	pslDnsRecords(IN)      : List of Dns Records
 **     pAddrString(IN)        : The IP Address.
 **	 	pErr(OUT)			   : Error structure in case the function fails
 *****************************************************************************/
Sdf_ty_retVal sdf_fn_uaAppendIpRecord (Sdf_ty_slist *pslDnsRecords, 
              Sdf_ty_s8bit 		  *pAddrString, 
        	  Sdf_st_error        *pErr)
{
    Sdf_st_DnsRecord	  *pDnsRec = Sdf_co_null;		

	pDnsRec = (Sdf_st_DnsRecord *)sdf_memget(Sdf_mc_initMemId,
            sizeof(Sdf_st_DnsRecord), pErr);

    /* Check if memory allocation successful for pDnsRec, 
     * as cannot proceed without this */
    if (Sdf_co_null == pDnsRec)
    {
        /* No need to set error as already set in pErr from memget */
        return Sdf_co_fail;
    }
#ifndef IPV6
    pDnsRec->dDnsRecordType = Sdf_en_IPv4Address;	
#else
    pDnsRec->dDnsRecordType = Sdf_en_IPv6Address;	
#endif	
    pDnsRec->pValue= (Sdf_ty_pvoid) pAddrString;
    if(sdf_listAppend (pslDnsRecords, (Sdf_ty_pvoid)pDnsRec,pErr) !=Sdf_co_success)
    {
        (Sdf_ty_void)sdf_memfree(Sdf_mc_callHandlerMemId,(Sdf_ty_pvoid*)&pDnsRec->pValue,pErr);
        (Sdf_ty_void)sdf_memfree(Sdf_mc_callHandlerMemId,(Sdf_ty_pvoid*)&pDnsRec,pErr);
#ifdef SDF_ERROR
        sdf_fn_setError(Sdf_en_majorErrors, pErr->stkErrCode, 
                (Sdf_ty_s8bit *)"sdf_fn_uaAppendIpRecord( ):"
                "Failed to Append the Dns Record in list",pErr);
#endif
        return Sdf_co_fail;
    }
    return Sdf_co_success;
}

/*****************************************************************************
 ** FUNCTION:    sdf_fn_uaAppendSrvRecord 
 ** DESCRIPTION: The function use to update the Sorted SRV records into the 
 **              Lookup module
 **	PARAMETERS:
 ** 	pslDnsRecords(IN)      : List of Dns Records
 **     pSrvRec(IN)            : Srv Record which is to append in the list.
 **	 	pErr(OUT)			   : Error structure in case the function fails
 *****************************************************************************/
Sdf_ty_retVal sdf_fn_uaAppendSrvRecord (Sdf_ty_slist *pslDnsRecords, 
        Sdf_st_dnsSrvRecord *pSrvRec, 
        Sdf_st_error        *pErr)
{
    Sdf_ty_bool           bAddFlag = Sdf_co_false;
    Sdf_ty_u32bit	      dSrvCount;
	Sdf_st_listIterator   dListIterator,dListIterator2;
    Sdf_st_dnsSrvRecord   *pTempSrvRec = Sdf_co_null;
	Sdf_st_dnsSrvRecord   *pPrevSrvRec = Sdf_co_null;
    Sdf_st_DnsRecord	  *pDnsRec = Sdf_co_null;		
    Sdf_st_DnsRecord	  *pTempDnsRec = Sdf_co_null;		
	Sdf_ty_u32bit 		  dCount = 1,dCount2 = 1;	

    pDnsRec = (Sdf_st_DnsRecord *)sdf_memget(Sdf_mc_initMemId,
            sizeof(Sdf_st_DnsRecord), pErr);

    /* Check if memory allocation successful for pDnsRec, 
     * as cannot proceed without this */
    if (Sdf_co_null == pDnsRec)
    {
        /* No need to set error as already set in pErr from memget */
        return Sdf_co_fail;
    }

    pDnsRec->dDnsRecordType = Sdf_en_SRVrecord;
    (Sdf_ty_void)sdf_listSizeOf (pslDnsRecords, &dSrvCount, pErr);
    /* 
     * The Insertion sorting algorithm is choosen for sorting. The insertion 
     * sort is choosen as the list will contain less than 10 elements.   
     */
    (Sdf_ty_void)sdf_listInitIterator(pslDnsRecords, &dListIterator, pErr);

    /* SPR 19218 : CSR 1-5826918 merged */
    /*
     * Only Unique FQDNs will be added in the list.
     */

    dCount2 = dCount;
    (Sdf_ty_void)sdf_listInitIterator(pslDnsRecords, &dListIterator2, pErr);

    while (dListIterator2.pCurrentElement != Sdf_co_null && dCount2 <= dSrvCount)
    {
        pTempDnsRec = (Sdf_st_DnsRecord *) (dListIterator2.pCurrentElement-> \
                pData);

        pTempSrvRec = (Sdf_st_dnsSrvRecord *) pTempDnsRec->pValue;

        if(0 == Sdf_mc_strcmp(pTempSrvRec->pTarget, pSrvRec->pTarget))
        {
            (Sdf_ty_void)sdf_memfree(Sdf_mc_callHandlerMemId,(Sdf_ty_pvoid*)&pDnsRec,pErr);
            pErr->errCode = Sdf_en_noUaError;
            return Sdf_co_success;
	    }
        (Sdf_ty_void)sdf_listNext(&dListIterator2, pErr);
        dCount2++;
    }

    /* End SPR 19218 : CSR 1-5826918 merged */

    while (dListIterator.pCurrentElement != Sdf_co_null && dCount <= dSrvCount)
    {
        pTempDnsRec = (Sdf_st_DnsRecord *) (dListIterator.pCurrentElement-> \
                pData);    

        pTempSrvRec = (Sdf_st_dnsSrvRecord *) pTempDnsRec->pValue;

        if (pSrvRec->dPriority < pTempSrvRec->dPriority)
        {
            pDnsRec->pValue		= (Sdf_ty_pvoid)pSrvRec;
            if (pPrevSrvRec != Sdf_co_null)
            {		
                /* Fix for CSR 1-8538021 : SPR-20385*/
                /* The function sdf_listInsertAt calculates the position
                 * from the index zero.
                 * To insert pDnsRec on correct position in
                 * pslDnsRecords list,give the parameter as (dCount-1)
                 * (which is the position number where the node pDnsRec
                 * will be inserted in list pslDnsRecords) since the
                 * dcount is intialised with 1.
                 */
                if(sdf_listInsertAt (pslDnsRecords,(dCount-1), pDnsRec,pErr) \
                        !=Sdf_co_success) 
                {
                    (Sdf_ty_void)sdf_memfree(Sdf_mc_callHandlerMemId,(Sdf_ty_pvoid*)&pDnsRec,pErr);
#ifdef SDF_ERROR
                    sdf_fn_setError(Sdf_en_majorErrors, pErr->stkErrCode, 
                            (Sdf_ty_s8bit *)"sdf_fn_uaAppendSrvRecord( ):"
                            "Failed to Append the srv Record in list",pErr);
#endif
                    return Sdf_co_fail;

                }
            }
            else
            {
                (Sdf_ty_void)sdf_listPrepend (pslDnsRecords,pDnsRec,pErr);
            }
            bAddFlag = Sdf_co_true;
            break;
        }
        else if (pSrvRec->dPriority == pTempSrvRec->dPriority)
        {
            while (dListIterator.pCurrentElement != Sdf_co_null && dCount <= dSrvCount)
            {
                pTempSrvRec = (Sdf_st_dnsSrvRecord *) (dListIterator.pCurrentElement-> \
                        pData);  
                /* The following weight calculation is deviation from RFC 2782. Larger
                   weights will be given a higher probability of being selected. */
		        if (pSrvRec->dWeight > pTempSrvRec->dWeight) 
        		{
                    pDnsRec->pValue			= (Sdf_ty_pvoid)pSrvRec;
                    if (pPrevSrvRec != Sdf_co_null) 
                    {
                       /* Fix for CSR 1-8538021 : SPR-20385*/
                       /* The function sdf_listInsertAt calculates the position
                        * from the index zero.
                        * To insert pDnsRec on correct position in
                        * pslDnsRecords list,give the parameter as (dCount-1)
                        * (which is the position number where the node pDnsRec
                        * will be inserted in list pslDnsRecords) since the
                        * dcount is intialised with 1.
                        */
                        if(sdf_listInsertAt (pslDnsRecords,(dCount - 1), pDnsRec,pErr)!= \
                                Sdf_co_success) 
                        {
							(Sdf_ty_void)sdf_memfree(Sdf_mc_callHandlerMemId,(Sdf_ty_pvoid*) \
                                                     &pDnsRec,pErr);
#ifdef SDF_ERROR
                            sdf_fn_setError(Sdf_en_majorErrors, pErr->stkErrCode, 
                                    (Sdf_ty_s8bit *)"sdf_fn_uaAppendSrvRecord( ):"
                                    "Failed to Append the srv Record in list",pErr);
#endif
                            return Sdf_co_fail;
						}
        			}
                    else
                    {
                        (Sdf_ty_void)sdf_listPrepend (pslDnsRecords,pDnsRec,pErr);
                    }
                    bAddFlag = Sdf_co_true;
                    break;
                }
                else if (pSrvRec->dWeight == pTempSrvRec->dWeight)
                {
                    pDnsRec->pValue			= (Sdf_ty_pvoid)pSrvRec;
                    if(sdf_listInsertAt (pslDnsRecords,dCount, \
							pDnsRec,pErr)!=Sdf_co_success)
                    {
                        (Sdf_ty_void)sdf_memfree(Sdf_mc_callHandlerMemId,\
                                                 (Sdf_ty_pvoid*)&pDnsRec,pErr);
#ifdef SDF_ERROR
                        sdf_fn_setError(Sdf_en_majorErrors, pErr->stkErrCode, 
                                (Sdf_ty_s8bit *)"sdf_fn_uaAppendSrvRecord( ):"
								"Failed to Append the srv Record in list",pErr);
#endif
						return Sdf_co_fail;

                    }

                    bAddFlag = Sdf_co_true;
                    break;
		        }  
                pPrevSrvRec = pTempSrvRec;
                (Sdf_ty_void)sdf_listNext(&dListIterator, pErr);  
				dCount++;
            }        
            if (bAddFlag == Sdf_co_true)
                break;
        }  
        else 
        {        
            pPrevSrvRec = pTempSrvRec;
            (Sdf_ty_void)sdf_listNext(&dListIterator, pErr);  
            dCount++;
        }
    }  
    if (bAddFlag == Sdf_co_false)
    {	
        pDnsRec->pValue		= (Sdf_ty_pvoid)pSrvRec;
        if(sdf_listAppend (pslDnsRecords, pDnsRec,pErr) !=Sdf_co_success)  
		{
            (Sdf_ty_void)sdf_memfree(Sdf_mc_callHandlerMemId,(Sdf_ty_pvoid*)&pDnsRec,pErr);
#ifdef SDF_ERROR
            sdf_fn_setError(Sdf_en_majorErrors, pErr->stkErrCode, 
                    (Sdf_ty_s8bit *)"sdf_fn_uaAppendSrvRecord( ):"
                    "Failed to Append the Srv Record in list",pErr);
#endif
            return Sdf_co_fail;
        }

    }	
    pErr->errCode = Sdf_en_noUaError;
	return Sdf_co_success;
}

/*****************************************************************************
 ** FUNCTION:   sdf_ivk_uaExpandString
 ** DESCRIPTION:  
 ** 
 *****************************************************************************/
Sdf_ty_u8bit sdf_ivk_uaExpandString(Sdf_ty_s8bit *p_src_str,
                                    Sdf_ty_s8bit **p_p_dest_str,
                                    Sdf_ty_u32bit length)
{
    Sdf_st_error        dErr;           

    *p_p_dest_str = (Sdf_ty_s8bit *)sdf_memget(Sdf_mc_callHandlerMemId,\
         length+1, &dErr);

    if(*p_p_dest_str == Sdf_co_null)
      return -1;
    
    Sdf_mc_memcpy(*p_p_dest_str,p_src_str,length);
    *(*p_p_dest_str+length)='\0';
    
    return Sdf_co_success;
}

#ifdef ICF_UATK_PORT
/*****************************************************************************
 ** FUNCTION:    sdf_fn_uaProcessQueryRespFromDnsClient 
 ** DESCRIPTION: The function is invoked by ES of IPTK for the processing of
 **              query result from DNS Client.This function will replace the
 **              function Ares_cbk_uaQueryResult.
 ** PARAMETERS:
 ** pResponse    Response varible for DNS query in structured format.
 ** 
 *****************************************************************************/
void sdf_fn_uaProcessQueryRespFromDnsClient(
        IN arDnsClient_response_st *pResponse)
{
    Sdf_ty_u32bit            index =0;
    Sdf_st_dnsEventContext   *pDnsEventContext = Sdf_co_null;
    Sdf_st_error             dErr = {0,0,0,"\0"};
    Sdf_ty_bool		         bStatus = Sdf_co_true, is_further_query = Sdf_co_false;	
    Sdf_ty_DnsRecordType     dDnsRecordType = Sdf_en_unKnownAddr;	
    Sdf_st_transportInfo     *pDestInfo = Sdf_co_null;
    Sdf_st_destDetails       *pDestDetails = Sdf_co_null;
    Sdf_ty_s8bit             dTempAddr[Sdf_co_characterBufferSize]="\0";
    SipAddrSpec              *pAddrSpec=Sdf_co_null;
    SipUrl		             *pUrl = Sdf_co_null;
    Sdf_ty_s8bit       	     *pTempHost = Sdf_co_null;
    SipMessage               *pSipMsg = Sdf_co_null;
    icf_global_port_info_st  *p_glb_port_info = ICF_NULL;
    icf_ssa_ctxt_st          *p_ssa_ctxt = ICF_NULL;
    icf_error_t              ecode = ICF_ERROR_NONE;
    icf_ssa_pdb_st           ssa_pdb;
    icf_return_t             ret_val = ICF_SUCCESS;
    /*  SRV Record */
    Sdf_st_dnsSrvRecord     *p_srv_rec = ICF_NULL;
    arDnsClient_dns_rr_list_st *pAnswerRecord = Sdf_co_null;    

    if(ICF_NULL == p_persistent_glb_pdb->p_glb_data)
    {
        ICF_PRINT(((icf_uint8_t*)"\n[UATK-Port]:glb data is NULL"));
        return;
    }
    else
    {
        p_glb_port_info = p_persistent_glb_pdb->p_glb_data->p_glb_port_info;
        if(ICF_NULL == p_glb_port_info)
        {
            ICF_PRINT(((icf_uint8_t*)"\n[UATK-Port]:glb port info is NULL"));
            return;
        }
    }

    icf_ssa_init_pdb(&ssa_pdb, p_persistent_glb_pdb);

    /*Fetch the pDnsEventContext from application data in query structure
      of DNS CLIENT*/

    if ((ICF_NULL == pResponse->pQuery) || \
         (ICF_NULL == pResponse->pQuery->pApp_query_ctx))
    {
            ICF_PRINT(((icf_uint8_t*)"\n[UATK-Port]:Not able to fetch application data in"
                        "query response from DNS CLIENT"));
            return;
    }

    pDnsEventContext = (Sdf_st_dnsEventContext *)(pResponse->pQuery->pApp_query_ctx);
    /* Fix For SPR: 19593*/
    /* Validate whether the pDnsEventContext pointer is present in any ssa
     * context or not. If not this is a delayed response from DNS CLIENT so
     * return from here.*/
    if (ICF_FAILURE == icf_ssa_validate_dnsEventContext((icf_void_t *)pDnsEventContext))
    {
        ICF_PRINT(((icf_uint8_t*)"\n[UATK-Port]:Query response from DNS CLIENT have invalid Sdf_st_dnsEventContext"));
        return;
    }
    if (pDnsEventContext->pCallObj == Sdf_co_null)
    { 
#ifdef SDF_ERROR
        sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidDNSrecord, 
                (Sdf_ty_s8bit *)"sdf_fn_uaProcessQueryRespFromDnsClient( ):"
                "Failed to get the callobject from DnsEventContext  structure",&dErr);
#endif
        return;
    }

    if(ICF_NULL != pDnsEventContext->pCallObj->pAppData)
    {
        p_ssa_ctxt = (icf_ssa_ctxt_st*)(pDnsEventContext->pCallObj->pAppData->pData);
    }

    if(ICF_NULL == p_ssa_ctxt) 
    {
        /* Free DnsEventContext of the Dns Query */
        (Sdf_ty_void)sdf_ivk_uaFreeDnsEventContext (pDnsEventContext);
        return;
    }
    else
    {
        /*Fix for SPR 16713
         * IPTK shall stop this timer here as if no response for SRV query,
         * then fn returned without stopping the DNS query.Ideally, it should stop here.
         */  
        icf_ssa_dns_stop_dns_resp_timer(p_ssa_ctxt,&ecode);

        p_ssa_ctxt->p_ssa_pdb = &ssa_pdb;
        ssa_pdb.p_ssa_ctxt = p_ssa_ctxt;
        ssa_pdb.p_ecode = &ecode;

        /* Fix for SPR:19593*/
        /* As we have received the DNS response from DNS CLIENT and DNS resp
         * timer is also stoped, reset the value of p_dns_event_context to
         * ICF_NULL in p_ssa_ctxt.*/
        p_ssa_ctxt->p_dns_event_context = ICF_NULL;
    }
    /* code for dns buffer */
#ifdef ICF_DNS_LOOKUP_ENABLED
    p_ssa_ctxt->bitmask |= ICF_SSA_DNS_XSACTION_PRESENT;
    p_ssa_ctxt->p_transaction = (icf_void_t*)pDnsEventContext->pTransaction;
    p_ssa_ctxt->p_overlap_transaction = (icf_void_t*)pDnsEventContext->pOverlappedTransaction;
#endif
    /* end of code for DNS buffer */

    /*This check is a stop-gap arrangement to minimize the risk
      of a crash in case the callobject is released before a DNS
      response is received. */
    if ((HSS_CHECKREF(pDnsEventContext->dRefCount)) ||
            (HSS_CHECKREF(pDnsEventContext->pCallObj->dRefCount)))
    { 
#ifdef SDF_ERROR
        sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidDNSrecord, 
                (Sdf_ty_s8bit *)"sdf_fn_uaProcessQueryRespFromDnsClient( ):"
                "Contexts deleted for the DNS query",&dErr);
#endif
        return;
    }

    if (pDnsEventContext->pTransaction != Sdf_co_null)
    {
        dDnsRecordType = pDnsEventContext->pTransaction->dDnsRecordType;
    }
    else if(pDnsEventContext->pOverlappedTransaction != Sdf_co_null)
    {
        dDnsRecordType = pDnsEventContext->pOverlappedTransaction->dDnsRecordType;
    }

#ifdef ICF_STAT_ENABLE
    /* If success response is received from DNS CLIENT update the STAT for the
     * same*/
    if (pResponse->result == ARDNSCL_SUCCESS)
    {
        /*Update DNS response STATS*/
        icf_ssa_stat_dns_resp_type(pDnsEventContext->dDnsRecordType, ICF_TRUE);
    }
    else
    {
        /*Update DNS response STATS*/
        icf_ssa_stat_dns_resp_type(pDnsEventContext->dDnsRecordType, ICF_FALSE);
    }
#endif
    /* Display an error message if there was an error, but only stop if  
     * we actually didn't get an answer buffer.
     */  
    if (pResponse->result != ARDNSCL_SUCCESS)  
    {
        bStatus = Sdf_co_false;

        /* Rel-8.2:SPR-19432:If any of the error code is returned by DNS
         * Client call should not be cleared.If any of these error code
         * is returned by DNS Client, the action taken by IPTK for 
         * different type of query is as below: 
         * 1. If failure response has been received for NAPTR query failure,
         *    SRV query will be attempted and transport mode is taken
         *    as self mode.
         * 2. If failure response has been received for SRV query failure,
         *    the port is taken as 5060.
         */ 
        if (pResponse->ecode != ARDNSCL_ERROR_DNS_INVALID_MSG_BUFFER &&
                pResponse->ecode != ARDNSCL_ERROR_INVALID_DOMAIN_NAME &&
                pResponse->ecode != ARDNSCL_ERROR_DATA_DOES_NOT_EXIST)  
        {
#ifdef SDF_ERROR
            sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_dnsTimeout, 
                    (Sdf_ty_s8bit *)"sdf_fn_uaProcessQueryRespFromDnsClient( ):DNS query Timeout",&dErr);
#endif    
            if(ICF_NULL != p_ssa_ctxt->p_call_ctxt)
            {   
                p_ssa_ctxt->p_call_ctxt->release_cause =
                    ICF_CALL_CLEAR_REASON_DNS_QUERY_FAILURE;
            }   	
        }
        else 
        {
            /*If NAPTR Query fails then ignore the failure and do the SRV Query */
            if(((Sdf_en_SRVrecord == pDnsEventContext->dDnsRecordType) || \
                        (Sdf_en_NAPTRrecord == pDnsEventContext->dDnsRecordType))) 
            {	
                /* If no SRV Record found in DNS then do A record query taking
                 * default ports.
                 */
#ifdef SDF_ERROR
                /*  Set the error that Naptr query fails */
                if(Sdf_en_NAPTRrecord == pDnsEventContext->dDnsRecordType)
                {
                    sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidDNSrecord,
                            (Sdf_ty_s8bit *)"sdf_fn_uaProcessQueryRespFromDnsClient( ):"
                            "DNS Mode not found doing SRV query taking default Mode",&dErr);
                }
                else
                {
                    sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidDNSrecord, 
                            (Sdf_ty_s8bit *)"sdf_fn_uaProcessQueryRespFromDnsClient( ):"
                            "DNS Record not found doing A query taking default port",&dErr);
                }
#endif  
                if (sdf_ivk_uaGetDestInfoFromTransaction(pDnsEventContext->pTransaction,\
                            pDnsEventContext->pOverlappedTransaction,\
                            &pDestInfo,&dErr) == Sdf_co_fail)
                {
#ifdef SDF_ERROR				
                    sdf_fn_setError(Sdf_en_minorErrors, \
                            Sdf_en_transpInfoAccessError, \
                            (Sdf_ty_s8bit *)"SDF_ERROR - "
                            "sdf_fn_uaProcessQueryRespFromDnsClient( ): Failed to get "
                            "Destination Info from the Transaction", &dErr);
#endif				
                    /* Free DnsEventContext of the Dns Query */
                    (Sdf_ty_void)sdf_ivk_uaFreeDnsEventContext (pDnsEventContext);
                    return; 
                }
                /*Set protocol in case of DNS-NAPTR failure*/
                if(Sdf_en_NAPTRrecord == pDnsEventContext->dDnsRecordType)
                {
                    pDnsEventContext->dProtocol = Sdf_en_protoAny;
                }
                /*  This should be done only in case of SRV */
                if(Sdf_en_SRVrecord == pDnsEventContext->dDnsRecordType)
                {
#ifdef SDF_TLS
                    /*if(pDnsEventContext->dDnsRecordType == Sdf_en_SRVrecord) */
                    /*  CSR 1-6085731 Merge */
                    if(pDnsEventContext->dUrlType == Sdf_en_SipsUrl)
                        pDestInfo->dPort = Sdf_mc_defaultSipSPort;
                    else
#endif
                        pDestInfo->dPort = Sdf_mc_defaultSipPort; 
                }

                (Sdf_ty_void)sdf_ivk_uaFreeTransportInfo(pDestInfo);
                /*pDnsEventContext->pTransaction->dDnsRecordType = Sdf_en_IPAddress;
                  pDnsEventContext->dDnsRecordType = Sdf_en_IPAddress;*/
                if(Sdf_en_SRVrecord == pDnsEventContext->dDnsRecordType)
                {
                    if (pDnsEventContext->pTransaction != Sdf_co_null)
                    {
                        pDnsEventContext->pTransaction->dDnsRecordType = 
                            Sdf_en_IPAddress;
                    }
                    else
                    {
                        if(Sdf_co_null != pDnsEventContext->pOverlappedTransaction)
                        {
                            pDnsEventContext->pOverlappedTransaction->dDnsRecordType= 
                                Sdf_en_IPAddress;
                        }
                    }

                    pDnsEventContext->dDnsRecordType = Sdf_en_IPAddress;
                }
                else
                {
                    if (pDnsEventContext->pTransaction != Sdf_co_null)
                    {
                        pDnsEventContext->pTransaction->dDnsRecordType = 
                            Sdf_en_SRVrecord;
                    }
                    else
                    {
                        if(Sdf_co_null != pDnsEventContext->pOverlappedTransaction)
                        {                        
                            pDnsEventContext->pOverlappedTransaction->dDnsRecordType= 
                                Sdf_en_SRVrecord;
                        }
                    }
                    pDnsEventContext->dDnsRecordType = Sdf_en_SRVrecord;
                }

                /*To get the domain name to be passed for A Query*/
                if((pDnsEventContext->pTransaction != Sdf_co_null) && 
                        (Sdf_co_null != pDnsEventContext->pTransaction->pSipMsg))
                {
                    pSipMsg = pDnsEventContext->pTransaction->pSipMsg;
                }
                else if((pDnsEventContext->pOverlappedTransaction != Sdf_co_null)
                        && (Sdf_co_null != pDnsEventContext->pOverlappedTransaction->pSipMsg))
                {
                    pSipMsg = pDnsEventContext->pOverlappedTransaction->pSipMsg;
                }
                else
                {
#ifdef SDF_ERROR
                    sdf_fn_setError(Sdf_en_majorErrors, dErr.errCode, \
                            (Sdf_ty_s8bit *)"sdf_fn_uaProcessQueryRespFromDnsClient(): "
                            "Failed to get the Sip Message.", &dErr);
#endif
                    /* Free DnsEventContext of the Dns Query */
                    (Sdf_ty_void)sdf_ivk_uaFreeDnsEventContext (pDnsEventContext);
                    return;
                }

                if((pDnsEventContext->pTransaction != Sdf_co_null) || 
                        (pDnsEventContext->pOverlappedTransaction != Sdf_co_null))
                    if (sdf_ivk_uaGetDestinationDetails(pDnsEventContext->pCallObj,\
                                pDnsEventContext->pOverlappedTransaction,\
                                pSipMsg, &pDestDetails,&dErr ) == Sdf_co_fail)
                    {
#ifdef SDF_ERROR
                        sdf_fn_setError(Sdf_en_majorErrors, dErr.errCode, \
                                (Sdf_ty_s8bit *)"sdf_fn_uaProcessQueryRespFromDnsClient(): "
                                "Failed to get the target address spec.", &dErr);
#endif
                        /* Free DnsEventContext of the Dns Query */
                        (Sdf_ty_void)sdf_ivk_uaFreeDnsEventContext (pDnsEventContext);
                        /*Klocwork warning removal*/
                        (Sdf_ty_void)sdf_ivk_uaFreeDestDetails(pDestDetails);
                        return ;
                    }
                if((pDnsEventContext->pTransaction != Sdf_co_null) || 
                        (pDnsEventContext->pOverlappedTransaction != Sdf_co_null))
                    if(pSipMsg->dType==SipMessageRequest)
                    {
                        if(Sdf_co_null != pDestDetails)
                        {     
                            pAddrSpec = pDestDetails->u.pAddrSpec;
                        }
                        if(sdf_ivk_uaGetUrlFromAddrSpec(pAddrSpec, &pUrl,\
                                    &dErr) == Sdf_co_fail)
                        {
                            (Sdf_ty_void)sdf_ivk_uaFreeDestDetails(pDestDetails);
#ifdef SDF_ERROR					
                            sdf_fn_setError(Sdf_en_minorErrors,\
                                    Sdf_en_destinationInfoError,\
                                    (Sdf_ty_s8bit *)"sdf_fn_uaProcessQueryRespFromDnsClient(): "
                                    "Failed to get Url From Addr Spec",&dErr);
#endif					
                            /* Free DnsEventContext of the Dns Query */
                            (Sdf_ty_void)sdf_ivk_uaFreeDnsEventContext (pDnsEventContext);
                            return ;
                        }
                        if (sip_getHostFromUrl(pUrl, &pTempHost,\
                                    (SipError *)&(dErr.stkErrCode)) == SipFail)
                        {
                            sip_freeSipUrl(pUrl);
                            (Sdf_ty_void)sdf_ivk_uaFreeDestDetails(pDestDetails);
#ifdef SDF_ERROR					
                            sdf_fn_setError(Sdf_en_minorErrors,\
                                    Sdf_en_destinationInfoError,\
                                    (Sdf_ty_s8bit *)"sdf_fn_uaProcessQueryRespFromDnsClient(): "
                                    "Failed to get Host from Url",&dErr);
#endif					
                            /* Free DnsEventContext of the Dns Query */
                            (Sdf_ty_void)sdf_ivk_uaFreeDnsEventContext (pDnsEventContext);
                            return ;
                        }
                        /* Replace strcpy with strncpy. Function strcpy
                         * will cause memory corruption if source string is 
                         * larger than Sdf_co_characterBufferSize.
                         */
                        Sdf_mc_strncpy(dTempAddr, pTempHost, (Sdf_co_characterBufferSize-1));
                        dTempAddr[Sdf_co_characterBufferSize-1]='\0';
                        if(sdf_fn_uaDnsQuery(dTempAddr, pDnsEventContext,&dErr) == Sdf_co_fail)
                        {
                            sip_freeSipUrl(pUrl);
                            (Sdf_ty_void)sdf_ivk_uaFreeDestDetails(pDestDetails);
#ifdef SDF_ERROR							
                            sdf_fn_setError(Sdf_en_minorErrors, Sdf_en_destinationInfoError,\
                                    (Sdf_ty_s8bit *)"sdf_fn_uaProcessQueryRespFromDnsClient(): "
                                    "DnsQuery failed", &dErr);
#endif					
                            /* Free DnsEventContext of the Dns Query */
                            (Sdf_ty_void)sdf_ivk_uaFreeDnsEventContext (pDnsEventContext);
                            return ;
                        }
                        sip_freeSipUrl(pUrl);
                    }
                (Sdf_ty_void)sdf_ivk_uaFreeDestDetails(pDestDetails);
                /* Free DnsEventContext of the Dns Query */
                (Sdf_ty_void)sdf_ivk_uaFreeDnsEventContext (pDnsEventContext);
                return;
            } 
        }  
    }/*pResponse->result != ARDNSCL_SUCCESS*/
    else 
    { 
        if ((ICF_NULL == pResponse->pDnsQueryResp))
        {
            ICF_PRINT(((icf_uint8_t*)"\n[UATK-Port]:Not able to fetch query response"
                        "from DNS CLIENT data"));
            return;
        }      
        
#ifdef ICF_DNS_LOOKUP_ENABLED   
        /* Fix For SPR:20192*/
        /* This will be used while adding the A query response in dns buffer
         * so that same will be populated in dns buffer,Now on dns purge timer
         * expiry same can be removed from DNS Client cache*/        
        if (ARDNSCL_QUERY_TYPE_A ==
                pResponse->pQuery->query_type)
        {
            Sdf_mc_strncpy(p_ssa_ctxt->queryA_fqdn.str,\
                    pResponse->pQuery->query_string.str_val,\
                    ICF_MAX_STR_LEN -1);            
            
            p_ssa_ctxt->queryA_fqdn.str_len =
                pResponse->pQuery->query_string.str_len;
        }
        /* Populate the NAPTR query in ssa context.
         * This will be used while adding the A query response in dns buffer
         * so that same will be populated in dns buffer,Now dns purge timer
         * expiry same can be removed from DNS Client cache*/        
        if (ARDNSCL_QUERY_TYPE_NAPTR ==
                pResponse->pQuery->query_type)
        {
            /* klocoworks: while doing strncpy, the maximum allowed
             * length of destination string should be passed so as to
             * avoid ABR
             */ 
            Sdf_mc_strncpy(p_ssa_ctxt->naptr_fqdn.str,\
                    pResponse->pQuery->query_string.str_val,\
                    ICF_MAX_STR_LEN -1);            
            
            p_ssa_ctxt->naptr_fqdn.str_len =
                pResponse->pQuery->query_string.str_len;
        }
        /* Populate the SRV query in ssa context.
         * This will be used while adding the A query response in dns buffer
         * so that same will be populated in dns buffer,Now dns purge timer
         * expiry same can be removed from DNS Client cache*/        
        if (ARDNSCL_QUERY_TYPE_SRV ==
                pResponse->pQuery->query_type)
        {
            /* klocoworks: while doing strncpy, the maximum allowed
             * length of destination string should be passed so as to
             * avoid ABR
             */ 
            Sdf_mc_strncpy(p_ssa_ctxt->srv_fqdn.str,\
                    pResponse->pQuery->query_string.str_val,\
                    ICF_MAX_STR_LEN -1);            
            
            p_ssa_ctxt->srv_fqdn.str_len =
                pResponse->pQuery->query_string.str_len;
        }
#endif        

        pAnswerRecord = pResponse->pDnsQueryResp->pAnswerRecords; 
        
        /*Start a for loop equal to number of responses received from DNS server*/
        for (index=1; index <= pResponse->pDnsQueryResp->numRecords; index++)
        {
            if (Sdf_co_null != pAnswerRecord)
            {
                /*Populate each response in DNS EVENT CONTEXT*/
                if (Sdf_co_fail ==  sdf_fn_uaFillQueryRespInDnsEvtCtx(pDnsEventContext, 
                            pAnswerRecord,pResponse->pQuery->query_type,
                            &is_further_query,&dErr))
                {
                    bStatus = Sdf_co_false;
#ifdef SDF_ERROR
                    sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidDNSrecord, 
                            (Sdf_ty_s8bit *)"sdf_fn_uaProcessQueryRespFromDnsClient():"
                            "DNS Response can not be populated in DNSEventContext",&dErr);
#endif
                }
                /*If the response type is NAPTR then break from the loop as we
                  have already processed each answer record in function 
                  Sdf_fn_uaFillQueryrespInDnsEvtCtx*/
                if (ARDNSCL_QUERY_TYPE_NAPTR == pResponse->pQuery->query_type)
                {
                    break;
                }	
                pAnswerRecord = pAnswerRecord->pNext;
            }
            else
            {
                bStatus = Sdf_co_false;
                break;
            }
        }

        /*  Now this function will store the two top priority fqdn 
            and do the A Query for the top priority fqdn */
        if ((Sdf_en_SRVrecord == pDnsEventContext->dDnsRecordType) && 
                (ICF_TRUE == is_further_query))
        {
            p_srv_rec = ((Sdf_st_dnsSrvRecord *)(((Sdf_st_DnsRecord *) \
                            pDnsEventContext->pslRecordList->head->pData) \
                        ->pValue));

            /*Store SRV records in SSA Context in order of priority*/
            if (ICF_SUCCESS == 
                    icf_ssa_get_dns_srv_records(&ssa_pdb, pDnsEventContext))
            {
                /*DNS-A query for highest priority SRV record*/
                ret_val = icf_ssa_dns_query_request(
                        &ssa_pdb,
                        pDnsEventContext->pOverlappedTransaction,
                        ssa_pdb.p_ssa_ctxt->fqdn_list[0].str, 
                        &(p_srv_rec->slIpRecords),
                        Sdf_en_IPv4Address);
            }
            /* else - when icf_ssa_get_dns_srv_records returns failure */
            else
            {
                ret_val = ICF_FAILURE;
            }
            /* Here ret_val will false if 
             * -icf_ssa_dns_query_request returned FAILURE
             * OR
             * -icf_ssa_get_dns_srv_records returned FAILURE
             * In either case is_further_query is set to FALSE
             * so that further processing can be done accordingly */
            if (ICF_FAILURE == ret_val)
            {
                is_further_query = ICF_FALSE;
            }
        }

    }/*pResponse->result == ARDNSCL_SUCCESS**/

    /* Update the lookup module with the current list if IP address found */

    if (((Sdf_co_false == is_further_query) ||
                (bStatus == Sdf_co_false)))
    {
        if(sdf_ivk_uaUpdateDnsRecordinTransaction (pDnsEventContext,bStatus,&dErr) \
                == Sdf_co_fail)
        {
            icf_ssa_post_dns_resp_processing(pDnsEventContext,Sdf_co_false);
#ifdef SDF_ERROR
            sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidDNSrecord, 
                    (Sdf_ty_s8bit *)"sdf_fn_uaProcessQueryRespFromDnsClient( ):"
                    "DNS Record not found",&dErr);
#endif 
        }

        /* ICF Addition: DNS records have been updated, no further query on network, 
           this is understood as dns query has been successful, so we
           trigger SendCallToPeer API*/
        else if ( Sdf_co_false == is_further_query)
        {
            icf_ssa_post_dns_resp_processing(pDnsEventContext, bStatus);
        }
    }
    /* Free DnsEventContext of the Dns Query */
    (Sdf_ty_void)sdf_ivk_uaFreeDnsEventContext (pDnsEventContext);
    return;
}


/*****************************************************************************
 ** FUNCTION:    sdf_fn_uaFillQueryRespInDnsEvtCtx
 ** DESCRIPTION: The function is invoked by sdf_fn_uaProcessQueryRespFromDnsClient
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
           Sdf_st_error                *pErr)
{
    Sdf_ty_s32bit           dttl = 0;
    Sdf_st_dnsSrvRecord     *pSrvRec = Sdf_co_null;  
    Sdf_ty_s8bit            *psrvAddress=Sdf_co_null; 
    Sdf_ty_u16bit           dPriority = 0,dWeight = 0,dPort = 0;
    Sdf_st_dnsEventContext  *pTmpDnsEventCtxt = Sdf_co_null;
    Sdf_ty_u32bit           dCurrentTime    = 0;
    Sdf_ty_u8bit            *p_host_string=Sdf_co_null, *p_new_host_string=Sdf_co_null;
    icf_error_t             ecode = ICF_NULL;
    
    /* used for passing it to icf_ssa_get_transport_mode to get the 
       final transport mode. */
    icf_boolean_t tls_present = ICF_FALSE;
    icf_boolean_t tcp_present = ICF_FALSE;
    icf_boolean_t udp_present = ICF_FALSE;
    Sdf_ty_u32bit orig_list_size = 0;

    dttl = pAnswerRecord->ttl;

    switch(query_type) 	
	{
        case ARDNSCL_QUERY_TYPE_NAPTR:
        {
            Sdf_ty_s8bit     tls_index=0,udp_index=0,tcp_index=0,trans_index=0;
            Sdf_ty_s8bit     *phosts[10] = {"\0","\0","\0","\0","\0",
                                            "\0","\0","\0","\0","\0"}; 
            Sdf_ty_u8bit     hostcount=0,record_len=0;
            
            arDnsClient_dns_rr_list_st *pNptrRecords = pAnswerRecord;
            
            /* Fetch the NAPTR responses in a loop */ 
            do
            {
                record_len =
                    pNptrRecords->query_rr_data.naptr_rr_data.replacement.str_len;
                
                psrvAddress = (Sdf_ty_s8bit *) sdf_memget(Sdf_mc_callHandlerMemId,\
                        record_len + 1, pErr);  
                
                if (Sdf_co_null == psrvAddress)
                {
                    return Sdf_co_fail;
                }
                else
                {
                    Sdf_mc_strncpy(psrvAddress,\
                            pNptrRecords->query_rr_data.naptr_rr_data.replacement.str_val,\
                            record_len);

                    phosts[hostcount++]=psrvAddress;
                    
                    if(psrvAddress == Sdf_mc_strstr(psrvAddress,"_sips._tcp"))
                    {
                        tls_index = hostcount;
                        /*  TLS Mode is present for this srv */
                        tls_present = ICF_TRUE;
                    }
                    else if(psrvAddress == Sdf_mc_strstr(psrvAddress,"_sip._tcp"))
                    {
                        tcp_index = hostcount;
                        /*  TCP Mode is present for this srv */
                        tcp_present = ICF_TRUE;	
                    }
                    else if(psrvAddress == Sdf_mc_strstr(psrvAddress,"_sip._udp"))
                    {
                        udp_index = hostcount;
                        /*  UDP Mode is present for this srv */
                        udp_present = ICF_TRUE;	
                    }
                }
                pNptrRecords = pNptrRecords->pNext;
            }while((Sdf_co_null != pNptrRecords) && (hostcount < 10));
            /* klocowrks: Since pHost is declared as array of 10 therefore the
             * do-while loop should continue till the max index of pHost otherwise
             * ABR will be encountered
             */     

            /* Get the transport mode from ssa, choose transport 
               appropriately and set in Transaction */
            switch(icf_ssa_get_transport_mode(pDnsEventContext->pCallObj,
                        tls_present,
                        tcp_present,
                        udp_present))
            {
#ifdef SDF_TLS
                case ICF_TRANSPORT_TYPE_TLS :
                {
                    trans_index = 
                        tls_index?tls_index:(tcp_index?tcp_index:udp_index);
                    break;
                }
#endif
                case ICF_TRANSPORT_TYPE_TCP :
                {
                    trans_index = tcp_index;
                    break;
                }
                case ICF_TRANSPORT_MODE_BOTH :
                {
                    trans_index = tcp_index?tcp_index:udp_index;
                    break;
                }
                case ICF_TRANSPORT_MODE_UDP :
                {
                    trans_index = udp_index;
                    break;
                }
                default : /* Error Condition */
                break;
            }
            if(trans_index)
            {
                Sdf_ty_u8bit         trans_mode[5] = "\0";
                Sdf_ty_s8bit         *pMethod = Sdf_co_null;
                Sdf_ty_messageType   msg_type = Sdf_en_unknownRequest;
                SipMessage           *pSipMsg = Sdf_co_null;
                SipError             sip_error = 0;
                Sdf_ty_protocol      dProto = Sdf_en_protoNone;
#ifdef SDF_TLS
                if(trans_index == tls_index)
                {
                    Sdf_mc_strcpy(trans_mode, "TLS");
                    dProto = Sdf_en_protoTls;
                }
                else 
#endif
                if(trans_index == tcp_index)
                {
                    Sdf_mc_strcpy(trans_mode, "TCP");
                    dProto = Sdf_en_protoTcp;
                }
                else if(trans_index == udp_index)
                {
                    Sdf_mc_strcpy(trans_mode, "UDP");
                    dProto = Sdf_en_protoUdp;
                }
                /* Set the transport mode */
                if (Sdf_co_fail == sdf_ivk_uaSetTransportSchemeInTransaction(
                                     pDnsEventContext->pTransaction,
                                     pDnsEventContext->pOverlappedTransaction,
                                     (icf_int8_t *)trans_mode,
                                     pErr))
                {
                    (Sdf_ty_void)sdf_memfree(Sdf_mc_callHandlerMemId,
                                             (Sdf_ty_pvoid*)&psrvAddress,pErr);
                    return Sdf_co_fail;
                }

                pSipMsg = (Sdf_co_null == pDnsEventContext->pTransaction)?\
                          pDnsEventContext->pOverlappedTransaction->pSipMsg:\
                          pDnsEventContext->pTransaction->pSipMsg;

                if (Sdf_co_fail == sdf_ivk_uaGetMethodFromSipMessage(pSipMsg,\
                            &pMethod,\
                            pErr))
                {
                    (Sdf_ty_void)sdf_memfree(Sdf_mc_callHandlerMemId,
                                             (Sdf_ty_pvoid*)&psrvAddress,pErr);
                    return Sdf_co_fail;
                }

                if (Sdf_co_fail == sdf_fn_uaGetMethodNameAsEnum(pMethod,\
                            &msg_type,\
                            pErr))
                {
                    (Sdf_ty_void)sdf_memfree(Sdf_mc_callHandlerMemId,
                                             (Sdf_ty_pvoid*)&psrvAddress,pErr);
                    return Sdf_co_fail;                    
                }

                if(SipFail == sip_deleteHeaderAtIndex(
                            pSipMsg, SipHdrTypeVia,0, &sip_error))
                {
                    (Sdf_ty_void)sdf_memfree(Sdf_mc_callHandlerMemId,
                                             (Sdf_ty_pvoid*)&psrvAddress,pErr);                    
                    return Sdf_co_fail;
                }

                /* Generate the new via header to reflect the new
                 * transport scheme selected*/
                else if(Sdf_co_fail == sdf_fn_uaFormViaHeader(pDnsEventContext->pCallObj,
                            pDnsEventContext->pOverlappedTransaction,msg_type,pSipMsg,
                            pErr))
                {
                    (Sdf_ty_void)sdf_memfree(Sdf_mc_callHandlerMemId,
                                             (Sdf_ty_pvoid*)&psrvAddress,pErr);                    
                    /*Klocwork warning resolve*/
                    ICF_KLOCWORK_FALSE_POSITIVE(p_persistent_glb_pdb,pSipMsg)
                    return Sdf_co_fail;
                }

                /* Event Context Filling  */
                if(sdf_ivk_uaInitDnsEventContext (&pTmpDnsEventCtxt, pErr)
                        != Sdf_co_success)
                {
                    (Sdf_ty_void)sdf_memfree(Sdf_mc_callHandlerMemId,
                                             (Sdf_ty_pvoid*)&pSrvRec, pErr);

#ifdef SDF_ERROR
                    sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidDNSrecord, 
                            (Sdf_ty_s8bit *)"Sdf_fn_uaFillQueryRespInDnsEvtCtx( ):"
                            "Failed to Initialize DNS Event context memory for SRV record", pErr);
 
#endif
                    /*Klocwork warning removal*/
                    (Sdf_ty_void)sdf_ivk_uaFreeDnsEventContext (pTmpDnsEventCtxt);
                    ICF_KLOCWORK_FALSE_POSITIVE(p_persistent_glb_pdb,pSipMsg)
                        return Sdf_co_fail;
                }
		
 
                HSS_LOCKEDINCREF(pDnsEventContext->pCallObj->dRefCount);
                if (pDnsEventContext->pTransaction != Sdf_co_null)
                    HSS_LOCKEDINCREF(pDnsEventContext->pTransaction->dRefCount);
                if (pDnsEventContext->pOverlappedTransaction != Sdf_co_null)
                    HSS_LOCKEDINCREF(pDnsEventContext->pOverlappedTransaction
                            ->dRefCount);
	
                pTmpDnsEventCtxt->pCallObj       = pDnsEventContext->pCallObj;
                pTmpDnsEventCtxt->pTransaction   = pDnsEventContext->pTransaction;
                pTmpDnsEventCtxt->pOverlappedTransaction = 
                    pDnsEventContext->pOverlappedTransaction;
                pTmpDnsEventCtxt->pslRecordList  = pDnsEventContext->pslRecordList;
                pTmpDnsEventCtxt->dDnsRecordType = Sdf_en_SRVrecord;

      				
                pTmpDnsEventCtxt->dProtocol    = dProto;
                p_host_string = (Sdf_ty_u8bit *)Sdf_mc_strstr(phosts[trans_index-1],".");

                /* klocworks : in case strstr return NULL then incrementing
                 * a NULL pointer to 1 is error therefore NULL checks have been
                 * added so that there is no NULL pointer dereferencing
                 */ 
				if(Sdf_co_null != p_host_string)
				{
					p_host_string += 1;
				}

				if(Sdf_co_null != p_host_string)
				{
                    p_new_host_string = (Sdf_ty_u8bit *)Sdf_mc_strstr((Sdf_ty_s8bit *)p_host_string,".");

					if(Sdf_co_null != p_new_host_string)
					        p_host_string = p_new_host_string + 1;
				}

                if((Sdf_co_null == p_host_string) ||
					   (sdf_fn_uaDnsQuery((Sdf_ty_s8bit *)p_host_string, pTmpDnsEventCtxt, \
                            pErr) !=Sdf_co_success) )
                {
#ifdef SDF_ERROR
                    sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidDNSrecord, 
                            (Sdf_ty_s8bit *)"sdf_fn_uaParseRR(): Failed to " 
                            "Make DnsQuery", pErr);
#endif
                    (Sdf_ty_void)sdf_memfree(Sdf_mc_callHandlerMemId,
                                             (Sdf_ty_pvoid*)&psrvAddress,pErr);                     
                    /*Klocwork Warning removal*/
                    ICF_KLOCWORK_FALSE_POSITIVE(p_persistent_glb_pdb,pSipMsg)
                    return Sdf_co_fail;		
                }
                /* Free DnsEventContext of the Dns Query */
                (Sdf_ty_void)sdf_ivk_uaFreeDnsEventContext (pTmpDnsEventCtxt);

                *p_query_flag = Sdf_co_true;
                /*Klocwork Warning removal*/
                ICF_KLOCWORK_FALSE_POSITIVE(p_persistent_glb_pdb,pSipMsg)
            }
            else
            {
                /* The memory for phosts was not getting freed if we are not going 
                   to do the SRV */
                while(hostcount)
                {
                    if ( ICF_FAILURE ==
                            icf_cmn_static_memfree(p_persistent_glb_pdb, 
                                phosts[--hostcount],         &ecode))
                    {
                        ICF_PRINT(((icf_uint8_t*)"\n[UATK-Port]:Failure in Static	Memfree"));
                    }
                }
                return Sdf_co_fail;
            }
            while(hostcount)
            {
                if ( ICF_FAILURE == 
                        icf_cmn_static_memfree(p_persistent_glb_pdb, phosts[--
                            hostcount], &ecode))
                {
                    ICF_PRINT(((icf_uint8_t*)"\n[UATK-Port]:Failure in Static Memfree"));
                }
            }
            break;
        }
		case ARDNSCL_QUERY_TYPE_SRV:
		{
            Sdf_ty_u8bit   record_len=0;
            /*Populate dPriority, dWeight, and dPort, followed by a domain 
              name from DNS response*/  
            
            dPriority  = pAnswerRecord->query_rr_data.srv_rr_data.priority;
            dWeight    = pAnswerRecord->query_rr_data.srv_rr_data.weight; 
            dPort      = pAnswerRecord->query_rr_data.srv_rr_data.port;
            record_len = pAnswerRecord->query_rr_data.srv_rr_data.nameTarget.str_len;

            psrvAddress = (Sdf_ty_s8bit *) sdf_memget(Sdf_mc_callHandlerMemId,\
                    record_len + 1, pErr);  
            if (Sdf_co_null == psrvAddress)
            {
                return Sdf_co_fail;
            }
            else
            {
                Sdf_mc_strncpy(psrvAddress,\
                        pAnswerRecord->query_rr_data.srv_rr_data.nameTarget.str_val,\
                        record_len);
            }

            /* SRV record  Filling  */
            if (sdf_ivk_uaInitDnsSrvRecord (&pSrvRec, pErr) != 
                    Sdf_co_success)
            {
                /* Free any allocated memory */
#ifdef SDF_ERROR
                sdf_fn_setError(Sdf_en_majorErrors,
                        Sdf_en_invalidDNSrecord, 
                        (Sdf_ty_s8bit *)"Sdf_fn_uaFillQueryRespInDnsEvtCtx():"
                        "Failed to allocate memory for SRV record", pErr);
#endif
                /*Klocwork warning removal*/
                (Sdf_ty_void)sdf_memfree(Sdf_mc_callHandlerMemId,
                                         (Sdf_ty_pvoid*)&pSrvRec, pErr);
                (Sdf_ty_void)sdf_memfree(Sdf_mc_callHandlerMemId,
                                         (Sdf_ty_pvoid*)&psrvAddress,pErr);                 
                return Sdf_co_fail;
            }		

            pSrvRec->dPriority = dPriority;
            pSrvRec->dWeight = dWeight;
            pSrvRec->dPort = dPort; 
            pSrvRec->dttl = dttl + dCurrentTime;

            Sdf_mc_strdup(pSrvRec->pTarget, psrvAddress);

            /*Free the memory allocated for temporary variable psrvAddress*/
            (Sdf_ty_void)sdf_memfree(Sdf_mc_callHandlerMemId,
                                     (Sdf_ty_pvoid*)&psrvAddress,pErr);              

            /* Store the initial size of list */
            orig_list_size = pDnsEventContext->pslRecordList->size; 

            /* Insert the record into the list */
            if(sdf_fn_uaAppendSrvRecord (pDnsEventContext->pslRecordList, 
                        pSrvRec, pErr) != Sdf_co_success)
            {
                (Sdf_ty_void)sdf_memfree(Sdf_mc_callHandlerMemId,
                                         (Sdf_ty_pvoid*)&pSrvRec, pErr);
#ifdef SDF_ERROR
                sdf_fn_setError(Sdf_en_majorErrors,
                        Sdf_en_invalidDNSrecord, 
                        (Sdf_ty_s8bit *)"Sdf_fn_uaFillQueryRespInDnsEvtCtx():" 
                        "Failed to Append Srv Record memory for SRV record", pErr);
#endif
                return Sdf_co_fail;		
            }
            if(orig_list_size != pDnsEventContext->pslRecordList->size)
            { 
                *p_query_flag = Sdf_co_true;
            }
            else if(orig_list_size == pDnsEventContext->pslRecordList->size)
            {
                /* If the SRV record is duplicate and hence not 
                   appended in this list, then free its allocated 
                   memory  */
                (Sdf_ty_void)sdf_memfree(Sdf_mc_callHandlerMemId,
                                         (Sdf_ty_pvoid*)&(pSrvRec->pTarget),pErr);
                (Sdf_ty_void)sdf_memfree(Sdf_mc_callHandlerMemId,
                                         (Sdf_ty_pvoid*)&pSrvRec, pErr);
            }
            break;
        }	
#ifndef SDF_IPV6
        case ARDNSCL_QUERY_TYPE_A:
		{
            /* The RR data is a four-byte Internet address */
            Sdf_ty_s8bit               *pAddrString = Sdf_co_null;
			Sdf_ty_DnsRecordType       dDnsRecordType;
            Sdf_ty_u8bit               record_len=0;
            /* IPTK CSR 1-6452321: IPTK SPR 19216: UATK CSR 1-6589034 */
            Sdf_ty_u32bit dAddrLen = 0;            

            record_len =
                pAnswerRecord->query_rr_data.A_rr_data.addrIP4.str_len;

            /*Fix done while merging CSR-1-6452321*/
            /*Allocate the memory = length of IP string + 4 bytes for storing
             * ttl value + 1*/
           
            pAddrString = (Sdf_ty_s8bit*)sdf_memget(
                    Sdf_mc_callHandlerMemId,
                    IPADDRSTRING + sizeof(int) + 1,pErr);  
            
            if (Sdf_co_null == pAddrString)
            {
                ICF_PRINT(((icf_uint8_t*)"\n[UATK-Port]: Memory allocation failure for"
                            "pAddrString while handling A response"))                
                return Sdf_co_fail;
            }
            else
            {
                Sdf_mc_strncpy(pAddrString,\
                        pAnswerRecord->query_rr_data.A_rr_data.addrIP4.str_val,\
                        record_len);   
                
                /* IPTK CSR 1-6452321: IPTK SPR 19216: UATK CSR 1-6589034 */
                /* get the string len of pAddrString */
                dAddrLen = Sdf_mc_strlen(pAddrString);

                /* store the integer in 4 bytes */
                (pAddrString + dAddrLen + 1)[0] = (Sdf_ty_u8bit) dttl;
                (pAddrString + dAddrLen + 1)[1] = (Sdf_ty_u8bit) (dttl >> 8);
                (pAddrString + dAddrLen + 1)[2] = (Sdf_ty_u8bit) (dttl >> 16);
                (pAddrString + dAddrLen + 1)[3] = (Sdf_ty_u8bit) (dttl >> 24);
                (pAddrString + dAddrLen + 1)[4] = 0;                
            }
          
            if (pDnsEventContext->pTransaction != Sdf_co_null)
            {
                dDnsRecordType =  pDnsEventContext->pTransaction
                    ->dDnsRecordType;	
            }
            else
            {
                dDnsRecordType =  pDnsEventContext->pOverlappedTransaction
                    ->dDnsRecordType;	
            }

            if (dDnsRecordType == Sdf_en_IPv4Address)
                (Sdf_ty_void)sdf_fn_uaAppendIpRecord(pDnsEventContext->pslRecordList,
                                                     pAddrString, pErr);
            else
            {
                /* Insert the record into the list */
                if(sdf_listAppend(pDnsEventContext->pslRecordList
                            , pAddrString, pErr) !=Sdf_co_success)
                {
                    (Sdf_ty_void)sdf_memfree(Sdf_mc_callHandlerMemId,
                                             (Sdf_ty_pvoid*)&pAddrString,pErr);
#ifdef SDF_ERROR
		    	sdf_fn_setError(Sdf_en_majorErrors, 
                        pErr->stkErrCode, 
                        (Sdf_ty_s8bit *)"Sdf_fn_uaFillQueryRespInDnsEvtCtx( ):"
                        "Failed to Append the Record in list",pErr);
#endif
                return Sdf_co_fail;
                }

            }
            break;
        }
#else 
	    case ARDNSCL_QUERY_TYPE_AAAA:
	    {
            /* The RR data is a four-byte Internet address */
            Sdf_ty_s8bit            *pAddrString = Sdf_co_null;
            Sdf_ty_DnsRecordType 	dDnsRecordType;
            Sdf_ty_u8bit            record_len=0;

            record_len =
                pAnswerRecord->query_rr_data.AAAA_rr_data.addrIP6.str_len;
            
            pAddrString = (Sdf_ty_s8bit*)sdf_memget(
                    Sdf_mc_callHandlerMemId,
                    record_len + 1,pErr);  
            
            if (Sdf_co_null == pAddrString)
            {
                ICF_PRINT(("\n[UATK-Port]: Memory allocation failure for"
                            "pAddrString while handling AAAA response"))                
                return Sdf_co_fail;
            }      
            else
            {
                Sdf_mc_strncpy(pAddrString,\
                        pAnswerRecord->query_rr_data.AAAA_rr_data.addrIP6.str_val,\
                        record_len);            
            }            

            if (pDnsEventContext->pTransaction != Sdf_co_null)
            {
                dDnsRecordType =  pDnsEventContext->pTransaction->dDnsRecordType;	
            }
            else
            {
                dDnsRecordType =  pDnsEventContext->pOverlappedTransaction->dDnsRecordType;	
            }
            if (dDnsRecordType == Sdf_en_IPv6Address)
            {
                (Sdf_ty_void)sdf_fn_uaAppendIpRecord(pDnsEventContext->pslRecordList,
                                                     pAddrString, pErr);
            }
            else
            {
                /* Insert the record into the list */
                if(sdf_listAppend(pDnsEventContext->pslRecordList, 
                            pAddrString, pErr) !=Sdf_co_success)
                {
                    (Sdf_ty_void)sdf_memfree(Sdf_mc_callHandlerMemId,
                                             (Sdf_ty_pvoid*)&pAddrString,pErr);
#ifdef SDF_ERROR
                    sdf_fn_setError(Sdf_en_majorErrors, pErr->stkErrCode, 
                            (Sdf_ty_s8bit *)"Sdf_fn_uaFillQueryRespInDnsEvtCtx( ):"
					"Failed to Append the Record in list",pErr);
#endif
                    return Sdf_co_fail;
                }
            }
            break;
        }
#endif        
        default:
        break;
    } /* End of switch(type) */     
    return Sdf_co_success;
}
#endif

/*Fuctional implementaion of sdf_fn_uaDnsQuery for SIP leg */
#ifndef ICF_UATK_PORT
/*****************************************************************************
 ** FUNCTION:   sdf_fn_uaDnsQuery
 **
 ** DESCRIPTION: This function has the routine to query the DNS server 
 **
 **	PARAMETERS:
 ** 	pDomainName(IN)       	    : The given Domain Name
 **     pDnsEventContext(IN)        : Sdf_st_dnsEventContext Structure
 **	 	pErr(OUT)					: Error structure in case the function fail
 *****************************************************************************/
Sdf_ty_retVal sdf_fn_uaDnsQuery (
        Sdf_ty_s8bit  *pDomainName,
        Sdf_st_dnsEventContext   *pDnsEventContext,
        Sdf_st_error             *pErr)
{
    /*Variable for DNS CLIENT*/
    Sdf_ty_pvoid               *pDnscl_glb_data = Sdf_co_null;
    dnscl_error_t              dns_err = 0; 
    dnscl_return_t             dnscl_ret_val = ARDNSCL_SUCCESS;
    arDnsClient_query_st       *pQueryData = Sdf_co_null;
    dnscl_uint8_t              service_name = ARDNSCL_SERVICE_INVALID;
    dnscl_uint8_t              protocol_name = ARDNSCL_PROTOCOL_INVALID;
    Sdf_st_dnsClientInfo       *pDnsClientInfo = (Sdf_st_dnsClientInfo *)&dDnsClientInfo;

    /*Get the global data for DNS CLIENT form Sdf_st_dnsClientInfo*/
    pDnscl_glb_data = pDnsClientInfo->pDnsClientGlbData;
#ifdef SDF_THREAD_SAFE
    Sdf_ty_threadId 		dCallingThread = 0;
#endif

    if(Sdf_co_null == pDnscl_glb_data)
    {
#ifdef SDF_ERROR
        sdf_fn_dumpError((Sdf_ty_s8bit *)"\n DNS CLIENT data is NULL");
#endif        
        return  Sdf_co_fail;
    }  
    if(Sdf_co_null == pDnsEventContext)
    {
#ifdef SDF_ERROR
        sdf_fn_dumpError((Sdf_ty_s8bit *)"\n pDnsEventContext is NULL");
#endif          
        return  Sdf_co_fail;
    }

	if(Sdf_co_null == pDomainName)
    {
#ifdef SDF_ERROR
        sdf_fn_dumpError((Sdf_ty_s8bit *)"\n pDomain Name is NULL");
#endif          
        return  Sdf_co_fail;
    }
	
    /* Getting the Record Type from Transaction's DNS Event Context  */
    if (pDnsEventContext->dDnsRecordType == Sdf_en_IPAddress)
#ifndef SDF_IPV6
        pDnsEventContext->dDnsRecordType = Sdf_en_IPv4Address;

    if ((pDnsEventContext->pTransaction != Sdf_co_null) && \
            (pDnsEventContext->pTransaction->dDnsRecordType == \
             Sdf_en_IPAddress))

        pDnsEventContext->pTransaction->dDnsRecordType = Sdf_en_IPv4Address;

    else if ((pDnsEventContext->pOverlappedTransaction != Sdf_co_null) && \
            (pDnsEventContext->pOverlappedTransaction->dDnsRecordType == \
             Sdf_en_IPAddress))

        pDnsEventContext->pOverlappedTransaction->dDnsRecordType = Sdf_en_IPv4Address;

#else
    pDnsEventContext->dDnsRecordType = Sdf_en_IPv6Address;

    if ((pDnsEventContext->pTransaction != Sdf_co_null) && \
            (pDnsEventContext->pTransaction->dDnsRecordType == Sdf_en_IPAddress))

        pDnsEventContext->pTransaction->dDnsRecordType = Sdf_en_IPv6Address;

    else if ((pDnsEventContext->pOverlappedTransaction != Sdf_co_null) && \
            (pDnsEventContext->pOverlappedTransaction->dDnsRecordType == Sdf_en_IPAddress))

        pDnsEventContext->pOverlappedTransaction->dDnsRecordType = Sdf_en_IPv6Address;
#endif

    /*Populate the Query data structure of DNS CLIENT*/
    pQueryData = (arDnsClient_query_st *)sdf_memget(Sdf_mc_initMemId,
            sizeof(arDnsClient_query_st),pErr);	

    if (Sdf_co_null == pQueryData)
    {
#ifdef SDF_ERROR
        sdf_fn_dumpError((Sdf_ty_s8bit *)"\n Memory allocation failure for "
                "query data of DNS CLIENT.Would not be supporing DNS query");
#endif
        return Sdf_co_fail;  
    }   

    switch(pDnsEventContext->dDnsRecordType)
    {
        case Sdf_en_NAPTRrecord:
        {
            pQueryData->query_type = ARDNSCL_QUERY_TYPE_NAPTR;
            break;
        }
        case Sdf_en_SRVrecord:
        {
            pQueryData->query_type = ARDNSCL_QUERY_TYPE_SRV;

            switch (pDnsEventContext->dProtocol)
            {
                case Sdf_en_protoAny:
                    protocol_name= ARDNSCL_PROTOCOL_UDP;
                    service_name = ARDNSCL_SERVICE_SIP;
                    break;
#ifdef SDF_TLS
                case Sdf_en_protoTls:
                    protocol_name= ARDNSCL_PROTOCOL_TCP;
                    service_name = ARDNSCL_SERVICE_SIPS;
                    break;
#endif
                case Sdf_en_protoTcp: 
                    protocol_name= ARDNSCL_PROTOCOL_TCP;
                    service_name = ARDNSCL_SERVICE_SIP;
                    break;
                default:
                    protocol_name= ARDNSCL_PROTOCOL_UDP;
                    service_name = ARDNSCL_SERVICE_SIP;
            }
            break;
        }	
#ifndef SDF_IPV6
        case Sdf_en_IPv4Address:
        {   
            pQueryData->query_type = ARDNSCL_QUERY_TYPE_A;
            break;
        }			
#else		
        case Sdf_en_IPv6Address:
        {
            pQueryData->query_type = ARDNSCL_QUERY_TYPE_AAAA;
            break;
        }		 
#endif		
        default:
        {
            (Sdf_ty_void)sdf_memfree(Sdf_mc_initMemId,
                                       (Sdf_ty_pvoid*)&pQueryData,pErr);            
            return Sdf_co_fail;
        }
    }

    /* Increment the reference counts in DnsEvent context */
    HSS_LOCKEDINCREF(pDnsEventContext->dRefCount);

    /*Populate the application data of DNS CLIENT with pDnsEventContext*/

    pQueryData->pApp_query_ctx = (dnscl_void_t *)(pDnsEventContext);

    Sdf_mc_strcpy(pQueryData->query_string.str_val, pDomainName);

    pQueryData->query_string.str_len = 
        Sdf_mc_strlen((Sdf_ty_s8bit *)pQueryData->query_string.str_val);

    /*Issue a DNS query from DNS Client*/
    dnscl_ret_val = arDnsClient_send_query(pDnscl_glb_data, 
            pQueryData, service_name, protocol_name,&dns_err);

#ifdef SDF_THREAD_SAFE
    dCallingThread = sdf_fn_uaGetCallingThreadId(Sdf_co_null);
    if (sdf_fn_uaTryLock(&(pDnsEventContext->pCallObj->dCallObjectMutex), \
                dCallingThread) == Sdf_co_success)
    {
        (void)sdf_ivk_uaUnlockCallObject(pDnsEventContext->pCallObj, pErr);
    }
#endif

    if (ARDNSCL_FAILURE == dnscl_ret_val)
    {
#ifdef SDF_ERROR
        sdf_fn_dumpError((Sdf_ty_s8bit *)"\n DNS CLIENT return failure for DNS query");
#endif
        (Sdf_ty_void)sdf_memfree(Sdf_mc_initMemId,
                                 (Sdf_ty_pvoid*)&pQueryData,pErr);          
        return Sdf_co_fail;  
    }
    else
    {
        /*Free the memory allocated to Query data*/
        (Sdf_ty_void)sdf_memfree(Sdf_mc_initMemId,
                                 (Sdf_ty_pvoid*)&pQueryData,pErr);          
    }

    pErr->errCode = Sdf_en_noUaError;
    return Sdf_co_success;
}


/*Fuctional implementaion of sdf_fn_uaFillQueryRespInDnsEvtCtx for SIP leg */

/*****************************************************************************
 ** FUNCTION:    sdf_fn_uaFillQueryRespInDnsEvtCtx
 ** DESCRIPTION: The function is invoked by sdf_fn_uaProcessQueryRespFromDnsClient
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
           Sdf_st_error                *pErr)
{
    Sdf_ty_s32bit           dttl = 0;
    Sdf_st_dnsSrvRecord     *pSrvRec = Sdf_co_null;  
    Sdf_ty_s8bit            *psrvAddress=Sdf_co_null; 
    Sdf_ty_u16bit           dPriority = 0,dWeight = 0,dPort = 0;
    Sdf_st_dnsEventContext  *pTmpDnsEventCtxt = Sdf_co_null;
    Sdf_ty_u32bit           dCurrentTime    = 0;
   
    p_query_flag = p_query_flag;
 
    dttl = pAnswerRecord->ttl;

    switch(query_type) 	
	{
		case ARDNSCL_QUERY_TYPE_SRV:
		{
            Sdf_ty_u8bit   record_len=0;
            /*Populate dPriority, dWeight, and dPort, followed by a domain 
              name from DNS response*/  
            
            dPriority  = pAnswerRecord->query_rr_data.srv_rr_data.priority;
            dWeight    = pAnswerRecord->query_rr_data.srv_rr_data.weight; 
            dPort      = pAnswerRecord->query_rr_data.srv_rr_data.port;
            record_len = pAnswerRecord->query_rr_data.srv_rr_data.nameTarget.str_len;

            psrvAddress = (Sdf_ty_s8bit *) sdf_memget(Sdf_mc_callHandlerMemId,\
                    record_len + 1, pErr);  
            if (Sdf_co_null == psrvAddress)
            {
                return Sdf_co_fail;
            }
            else
            {
                Sdf_mc_strncpy(psrvAddress,\
                        pAnswerRecord->query_rr_data.srv_rr_data.nameTarget.str_val,\
                        record_len);
            }

            /* SRV record  Filling  */
            if (sdf_ivk_uaInitDnsSrvRecord (&pSrvRec, pErr) != 
                    Sdf_co_success)
            {
                /* Free any allocated memory */
#ifdef SDF_ERROR
                sdf_fn_setError(Sdf_en_majorErrors,
                        Sdf_en_invalidDNSrecord, 
                        (Sdf_ty_s8bit *)"Sdf_fn_uaFillQueryRespInDnsEvtCtx():"
                        "Failed to allocate memory for SRV record", pErr);
#endif
                /*Klocwork warning removal*/
                (Sdf_ty_void)sdf_memfree(Sdf_mc_callHandlerMemId,
                                         (Sdf_ty_pvoid*)&pSrvRec, pErr);
                (Sdf_ty_void)sdf_memfree(Sdf_mc_callHandlerMemId,
                                         (Sdf_ty_pvoid*)&psrvAddress,pErr);                 
                return Sdf_co_fail;
            }		
            if(sdf_fn_uaGetCurrentTimeInSecs(&dCurrentTime) != Sdf_co_success)
            {
#ifdef SDF_ERROR
                sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_unknownMethodError, 
                        (Sdf_ty_s8bit *)"Sdf_fn_uaFillQueryRespInDnsEvtCtx( ): Failed to"  
                        " get Current Time", pErr);
#endif
                return Sdf_co_fail;
            }            

            pSrvRec->dPriority = dPriority;
            pSrvRec->dWeight = dWeight;
            pSrvRec->dPort = dPort; 
            pSrvRec->dttl = dttl + dCurrentTime;

            Sdf_mc_strdup(pSrvRec->pTarget, psrvAddress);

            /*Free the memory allocated for temporary variable psrvAddress*/
            (Sdf_ty_void)sdf_memfree(Sdf_mc_callHandlerMemId,
                                     (Sdf_ty_pvoid*)&psrvAddress,pErr);   
            /*  Event Context Filling  */
            if(sdf_ivk_uaInitDnsEventContext (&pTmpDnsEventCtxt, pErr) != Sdf_co_success)
            {
                (Sdf_ty_void)sdf_memfree(Sdf_mc_callHandlerMemId,(Sdf_ty_pvoid*)&pSrvRec, pErr);

#ifdef SDF_ERROR
                sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidDNSrecord, 
                        (Sdf_ty_s8bit *)"sdf_fn_uaParseRR( ): Failed to " 
                        "Initialize DNS Event context memory for SRV record", pErr);
#endif
                return Sdf_co_fail;
            }            
            
            HSS_LOCKEDINCREF(pDnsEventContext->pCallObj->dRefCount);
            
            if (pDnsEventContext->pTransaction != Sdf_co_null)
                HSS_LOCKEDINCREF(pDnsEventContext->pTransaction->dRefCount);
            
            if (pDnsEventContext->pOverlappedTransaction != Sdf_co_null)
                HSS_LOCKEDINCREF(pDnsEventContext->pOverlappedTransaction->dRefCount);
            
#ifdef SDF_ASYNC_DNS
            if (pDnsEventContext->pAppData != Sdf_co_null)
                HSS_LOCKEDINCREF(((Sdf_st_asyncDNSContextInfo*)pDnsEventContext->pAppData)->dRefCount);
#endif /* SDF_AYSYNC_DNS */	
            pTmpDnsEventCtxt->pCallObj               = pDnsEventContext->pCallObj;
            
            pTmpDnsEventCtxt->pTransaction           = pDnsEventContext->pTransaction;
            
            pTmpDnsEventCtxt->pOverlappedTransaction = 
                pDnsEventContext->pOverlappedTransaction;
            pTmpDnsEventCtxt->pslRecordList          = &(pSrvRec->slIpRecords);
#ifndef SDF_IPV6				
            pTmpDnsEventCtxt->dDnsRecordType         = Sdf_en_IPv4Address;
#else
            pTmpDnsEventCtxt->dDnsRecordType         = Sdf_en_IPv6Address;
#endif

            pTmpDnsEventCtxt->dProtocol		         = Sdf_en_protoNone;
#ifdef SDF_ASYNC_DNS
            pTmpDnsEventCtxt->pAppData		= pDnsEventContext->pAppData;
#endif /* SDF_AYSYNC_DNS */

            /* Insert the record into the list */
            if(sdf_fn_uaAppendSrvRecord (pDnsEventContext->pslRecordList, 
                        pSrvRec, pErr) != Sdf_co_success)
            {
                (Sdf_ty_void)sdf_memfree(Sdf_mc_callHandlerMemId,
                                         (Sdf_ty_pvoid*)&pSrvRec, pErr);
#ifdef SDF_ERROR
                sdf_fn_setError(Sdf_en_majorErrors,
                        Sdf_en_invalidDNSrecord, 
                        (Sdf_ty_s8bit *)"Sdf_fn_uaFillQueryRespInDnsEvtCtx():" 
                        "Failed to Append Srv Record memory for SRV record", pErr);
#endif
                return Sdf_co_fail;		
            }
            if(sdf_fn_uaDnsQuery(pSrvRec->pTarget, pTmpDnsEventCtxt, \
                        pErr) !=Sdf_co_success) 
            {
                (Sdf_ty_void)sdf_memfree(Sdf_mc_callHandlerMemId,(Sdf_ty_pvoid*)&pSrvRec, pErr);
#ifdef SDF_ERROR
                sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidDNSrecord, 
                        (Sdf_ty_s8bit *)"Sdf_fn_uaFillQueryRespInDnsEvtCtx():Failed to " 
                        "Make DnsQuery", pErr);
#endif
                return Sdf_co_fail;		
            }
            /* Free DnsEventContext of the Dns Query */
            if(pTmpDnsEventCtxt != Sdf_co_null)
            {
                (Sdf_ty_void)sdf_ivk_uaFreeDnsEventContext (pTmpDnsEventCtxt);
            }
            pSrvRec = Sdf_co_fail;            

            break;
        }	
#ifndef SDF_IPV6
        case ARDNSCL_QUERY_TYPE_A:
		{
            /* The RR data is a four-byte Internet address */
            Sdf_ty_s8bit               *pAddrString = Sdf_co_null;
			Sdf_ty_DnsRecordType       dDnsRecordType;
            Sdf_ty_u8bit               record_len=0;
            /* IPTK CSR 1-6452321: IPTK SPR 19216: UATK CSR 1-6589034 */
            Sdf_ty_u32bit dAddrLen = 0;            

            record_len =
                pAnswerRecord->query_rr_data.A_rr_data.addrIP4.str_len;

            /*Fix done while merging CSR-1-6452321*/
            /*Allocate the memory = length of IP string + 4 bytes for storing
             * ttl value + 1*/
           
            pAddrString = (Sdf_ty_s8bit*)sdf_memget(
                    Sdf_mc_callHandlerMemId,
                    IPADDRSTRING + sizeof(int) + 1,pErr);  
            
            if (Sdf_co_null == pAddrString)
            {
                return Sdf_co_fail;
            }
            else
            {
                Sdf_mc_strncpy(pAddrString,\
                        pAnswerRecord->query_rr_data.A_rr_data.addrIP4.str_val,\
                        record_len);   
                
                /* IPTK CSR 1-6452321: IPTK SPR 19216: UATK CSR 1-6589034 */
                /* get the string len of pAddrString */
                dAddrLen = Sdf_mc_strlen(pAddrString);

                /* store the integer in 4 bytes */
                (pAddrString + dAddrLen + 1)[0] = (Sdf_ty_u8bit) dttl;
                (pAddrString + dAddrLen + 1)[1] = (Sdf_ty_u8bit) (dttl >> 8);
                (pAddrString + dAddrLen + 1)[2] = (Sdf_ty_u8bit) (dttl >> 16);
                (pAddrString + dAddrLen + 1)[3] = (Sdf_ty_u8bit) (dttl >> 24);
                (pAddrString + dAddrLen + 1)[4] = 0;                
            }
          
            if (pDnsEventContext->pTransaction != Sdf_co_null)
            {
                dDnsRecordType =  pDnsEventContext->pTransaction
                    ->dDnsRecordType;	
            }
            else
            {
                dDnsRecordType =  pDnsEventContext->pOverlappedTransaction
                    ->dDnsRecordType;	
            }

            if (dDnsRecordType == Sdf_en_IPv4Address)
                (Sdf_ty_void)sdf_fn_uaAppendIpRecord(pDnsEventContext->pslRecordList,
                                                     pAddrString, pErr);
            else
            {
                /* Insert the record into the list */
                if(sdf_listAppend(pDnsEventContext->pslRecordList
                            , pAddrString, pErr) !=Sdf_co_success)
                {
                    (Sdf_ty_void)sdf_memfree(Sdf_mc_callHandlerMemId,
                                             (Sdf_ty_pvoid*)&pAddrString,pErr);
#ifdef SDF_ERROR
		    	sdf_fn_setError(Sdf_en_majorErrors, 
                        pErr->stkErrCode, 
                        (Sdf_ty_s8bit *)"Sdf_fn_uaFillQueryRespInDnsEvtCtx( ):"
                        "Failed to Append the Record in list",pErr);
#endif
                return Sdf_co_fail;
                }

            }
            break;
        }
#else 
	    case ARDNSCL_QUERY_TYPE_AAAA:
	    {
            /* The RR data is a four-byte Internet address */
            Sdf_ty_s8bit            *pAddrString = Sdf_co_null;
            Sdf_ty_DnsRecordType 	dDnsRecordType;
            Sdf_ty_u8bit            record_len=0;

            record_len =
                pAnswerRecord->query_rr_data.AAAA_rr_data.addrIP6.str_len;
            
            pAddrString = (Sdf_ty_s8bit*)sdf_memget(
                    Sdf_mc_callHandlerMemId,
                    record_len + 1,pErr);  
            
            if (Sdf_co_null == pAddrString)
            {
                return Sdf_co_fail;
            }      
            else
            {
                Sdf_mc_strncpy(pAddrString,\
                        pAnswerRecord->query_rr_data.AAAA_rr_data.addrIP6.str_val,\
                        record_len);            
            }            

            if (pDnsEventContext->pTransaction != Sdf_co_null)
            {
                dDnsRecordType =  pDnsEventContext->pTransaction->dDnsRecordType;	
            }
            else
            {
                dDnsRecordType =  pDnsEventContext->pOverlappedTransaction->dDnsRecordType;	
            }
            if (dDnsRecordType == Sdf_en_IPv6Address)
            {
                (Sdf_ty_void)sdf_fn_uaAppendIpRecord(pDnsEventContext->pslRecordList,
                                                     pAddrString, pErr);
            }
            else
            {
                /* Insert the record into the list */
                if(sdf_listAppend(pDnsEventContext->pslRecordList, 
                            pAddrString, pErr) !=Sdf_co_success)
                {
                    (Sdf_ty_void)sdf_memfree(Sdf_mc_callHandlerMemId,
                                             (Sdf_ty_pvoid*)&pAddrString,pErr);
#ifdef SDF_ERROR
                    sdf_fn_setError(Sdf_en_majorErrors, pErr->stkErrCode, 
                            (Sdf_ty_s8bit *)"Sdf_fn_uaFillQueryRespInDnsEvtCtx( ):"
					"Failed to Append the Record in list",pErr);
#endif
                    return Sdf_co_fail;
                }
            }
            break;
        }
#endif        
        default:
        break;
    } /* End of switch(type) */     
    return Sdf_co_success;
}

/*Fuctional implementaion of sdf_fn_uaProcessQueryRespFromDnsClient for SIP leg */

/*****************************************************************************
 ** FUNCTION:    sdf_fn_uaProcessQueryRespFromDnsClient 
 ** DESCRIPTION: The function is invoked by ES of IPTK for the processing of
 **              query result from DNS Client.This function will replace the
 **              function Ares_cbk_uaQueryResult.
 ** PARAMETERS:
 ** pResponse    Response varible for DNS query in structured format.
 ** 
 *****************************************************************************/
void sdf_fn_uaProcessQueryRespFromDnsClient(
        IN arDnsClient_response_st *pResponse)
{
    Sdf_ty_u32bit            index =0;
    Sdf_st_dnsEventContext   *pDnsEventContext = Sdf_co_null;
    Sdf_st_error             dErr = {0,0,0,"\0"};
    Sdf_ty_bool		         bStatus = Sdf_co_true, is_further_query = Sdf_co_false;	
    Sdf_ty_DnsRecordType     dDnsRecordType = Sdf_en_unKnownAddr;	
    Sdf_st_transportInfo     *pDestInfo = Sdf_co_null;
    Sdf_st_destDetails       *pDestDetails = Sdf_co_null;
    Sdf_ty_s8bit             dTempAddr[Sdf_co_characterBufferSize]="\0";
    SipAddrSpec              *pAddrSpec=Sdf_co_null;
    SipUrl		             *pUrl = Sdf_co_null;
    Sdf_ty_s8bit       	     *pTempHost = Sdf_co_null;
    SipMessage               *pSipMsg = Sdf_co_null;
    /*  SRV Record */
    arDnsClient_dns_rr_list_st *pAnswerRecord = Sdf_co_null;    

    /*Fetch the pDnsEventContext from application data in query structure
      of DNS CLIENT*/

    if ((Sdf_co_null == pResponse->pQuery) || \
         (Sdf_co_null == pResponse->pQuery->pApp_query_ctx))
    {
#ifdef SDF_ERROR
        sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidDNSrecord, 
                (Sdf_ty_s8bit *)"sdf_fn_uaProcessQueryRespFromDnsClient( ):"
                "Not able to fetch application data in query response from DNS CLIENT",&dErr);
#endif        
            return;
    }

    pDnsEventContext = (Sdf_st_dnsEventContext *)(pResponse->pQuery->pApp_query_ctx);

    if (pDnsEventContext->pCallObj == Sdf_co_null)
    { 
#ifdef SDF_ERROR
        sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidDNSrecord, 
                (Sdf_ty_s8bit *)"sdf_fn_uaProcessQueryRespFromDnsClient( ):"
                "Failed to get the callobject from DnsEventContext  structure",&dErr);
#endif
        return;
    }

    /*This check is a stop-gap arrangement to minimize the risk
      of a crash in case the callobject is released before a DNS
      response is received. */
    if ((HSS_CHECKREF(pDnsEventContext->dRefCount)) ||
            (HSS_CHECKREF(pDnsEventContext->pCallObj->dRefCount)))
    { 
#ifdef SDF_ERROR
        sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidDNSrecord, 
                (Sdf_ty_s8bit *)"sdf_fn_uaProcessQueryRespFromDnsClient( ):"
                "Contexts deleted for the DNS query",&dErr);
#endif
        return;
    }

    if (pDnsEventContext->pTransaction != Sdf_co_null)
    {
        dDnsRecordType = pDnsEventContext->pTransaction->dDnsRecordType;
    }
    else if(pDnsEventContext->pOverlappedTransaction != Sdf_co_null)
    {
        dDnsRecordType = pDnsEventContext->pOverlappedTransaction->dDnsRecordType;
    }

    /* Display an error message if there was an error, but only stop if  
     * we actually didn't get an answer buffer.
     */  
    if (pResponse->result != ARDNSCL_SUCCESS)  
    {
        bStatus = Sdf_co_false;

        if (pResponse->ecode != ARDNSCL_ERROR_DNS_INVALID_MSG_BUFFER &&
                pResponse->ecode != ARDNSCL_ERROR_INVALID_DOMAIN_NAME)  
        {
#ifdef SDF_ERROR
            sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_dnsTimeout, 
                    (Sdf_ty_s8bit *)"sdf_fn_uaProcessQueryRespFromDnsClient( ):DNS query Timeout",&dErr);
#endif    
        }
        else 
        {
            /*If NAPTR Query fails then ignore the failure and do the SRV Query */
            if((Sdf_en_SRVrecord == pDnsEventContext->dDnsRecordType)) 
            {	
                /* If no SRV Record found in DNS then do A record query taking
                 * default ports.
                 */
#ifdef SDF_ERROR
                sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidDNSrecord, 
                        (Sdf_ty_s8bit *)"sdf_fn_uaProcessQueryRespFromDnsClient( ):"
                        "DNS Record not found doing A query taking default port",&dErr);
#endif  
                if (sdf_ivk_uaGetDestInfoFromTransaction(pDnsEventContext->pTransaction,\
                            pDnsEventContext->pOverlappedTransaction,\
                            &pDestInfo,&dErr) == Sdf_co_fail)
                {
#ifdef SDF_ERROR				
                    sdf_fn_setError(Sdf_en_minorErrors, \
                            Sdf_en_transpInfoAccessError, \
                            (Sdf_ty_s8bit *)"SDF_ERROR - "
                            "sdf_fn_uaProcessQueryRespFromDnsClient( ): Failed to get "
                            "Destination Info from the Transaction", &dErr);
#endif				
                    /* Free DnsEventContext of the Dns Query */
                    (Sdf_ty_void)sdf_ivk_uaFreeDnsEventContext (pDnsEventContext);
                    return; 
                }

                /*  This should be done only in case of SRV */
                if(Sdf_en_SRVrecord == pDnsEventContext->dDnsRecordType)
                {
#ifdef SDF_TLS
                    /*if(pDnsEventContext->dDnsRecordType == Sdf_en_SRVrecord) */
                    /*  CSR 1-6085731 Merge */
                    if(pDnsEventContext->dUrlType == Sdf_en_SipsUrl)
                        pDestInfo->dPort = Sdf_mc_defaultSipSPort;
                    else
#endif
                        pDestInfo->dPort = Sdf_mc_defaultSipPort; 
                }

                (Sdf_ty_void)sdf_ivk_uaFreeTransportInfo(pDestInfo);
                /*pDnsEventContext->pTransaction->dDnsRecordType = Sdf_en_IPAddress;
                  pDnsEventContext->dDnsRecordType = Sdf_en_IPAddress;*/
                if (pDnsEventContext->pTransaction != Sdf_co_null)
                {
                    pDnsEventContext->pTransaction->dDnsRecordType = 
                        Sdf_en_IPAddress;
                }
                else
                {
                    if(Sdf_co_null != pDnsEventContext->pOverlappedTransaction)
                    {
                        pDnsEventContext->pOverlappedTransaction->dDnsRecordType= 
                            Sdf_en_IPAddress;
                    }
                }

                pDnsEventContext->dDnsRecordType = Sdf_en_IPAddress;


                /*To get the domain name to be passed for A Query*/
                if((pDnsEventContext->pTransaction != Sdf_co_null) && 
                        (Sdf_co_null != pDnsEventContext->pTransaction->pSipMsg))
                {
                    pSipMsg = pDnsEventContext->pTransaction->pSipMsg;
                }
                else if((pDnsEventContext->pOverlappedTransaction != Sdf_co_null)
                        && (Sdf_co_null != pDnsEventContext->pOverlappedTransaction->pSipMsg))
                {
                    pSipMsg = pDnsEventContext->pOverlappedTransaction->pSipMsg;
                }
                else
                {
#ifdef SDF_ERROR
                    sdf_fn_setError(Sdf_en_majorErrors, dErr.errCode, \
                            (Sdf_ty_s8bit *)"sdf_fn_uaProcessQueryRespFromDnsClient(): "
                            "Failed to get the Sip Message.", &dErr);
#endif
                    /* Free DnsEventContext of the Dns Query */
                    (Sdf_ty_void)sdf_ivk_uaFreeDnsEventContext (pDnsEventContext);
                    return;
                }

                if((pDnsEventContext->pTransaction != Sdf_co_null) || 
                        (pDnsEventContext->pOverlappedTransaction != Sdf_co_null))
                    if (sdf_ivk_uaGetDestinationDetails(pDnsEventContext->pCallObj,\
                                pDnsEventContext->pOverlappedTransaction,\
                                pSipMsg, &pDestDetails,&dErr ) == Sdf_co_fail)
                    {
#ifdef SDF_ERROR
                        sdf_fn_setError(Sdf_en_majorErrors, dErr.errCode, \
                                (Sdf_ty_s8bit *)"sdf_fn_uaProcessQueryRespFromDnsClient(): "
                                "Failed to get the target address spec.", &dErr);
#endif
                        /* Free DnsEventContext of the Dns Query */
                        (Sdf_ty_void)sdf_ivk_uaFreeDnsEventContext (pDnsEventContext);
                        /*Klocwork warning removal*/
                        (Sdf_ty_void)sdf_ivk_uaFreeDestDetails(pDestDetails);
                        return ;
                    }
                if((pDnsEventContext->pTransaction != Sdf_co_null) || 
                        (pDnsEventContext->pOverlappedTransaction != Sdf_co_null))
                    if(pSipMsg->dType==SipMessageRequest)
                    {
                        if(Sdf_co_null != pDestDetails)
                        {     
                            pAddrSpec = pDestDetails->u.pAddrSpec;
                        }
                        if(sdf_ivk_uaGetUrlFromAddrSpec(pAddrSpec, &pUrl,\
                                    &dErr) == Sdf_co_fail)
                        {
                            (Sdf_ty_void)sdf_ivk_uaFreeDestDetails(pDestDetails);
#ifdef SDF_ERROR					
                            sdf_fn_setError(Sdf_en_minorErrors,\
                                    Sdf_en_destinationInfoError,\
                                    (Sdf_ty_s8bit *)"sdf_fn_uaProcessQueryRespFromDnsClient(): "
                                    "Failed to get Url From Addr Spec",&dErr);
#endif					
                            /* Free DnsEventContext of the Dns Query */
                            (Sdf_ty_void)sdf_ivk_uaFreeDnsEventContext (pDnsEventContext);
                            return ;
                        }
                        if (sip_getHostFromUrl(pUrl, &pTempHost,\
                                    (SipError *)&(dErr.stkErrCode)) == SipFail)
                        {
                            sip_freeSipUrl(pUrl);
                            (Sdf_ty_void)sdf_ivk_uaFreeDestDetails(pDestDetails);
#ifdef SDF_ERROR					
                            sdf_fn_setError(Sdf_en_minorErrors,\
                                    Sdf_en_destinationInfoError,\
                                    (Sdf_ty_s8bit *)"sdf_fn_uaProcessQueryRespFromDnsClient(): "
                                    "Failed to get Host from Url",&dErr);
#endif					
                            /* Free DnsEventContext of the Dns Query */
                            (Sdf_ty_void)sdf_ivk_uaFreeDnsEventContext (pDnsEventContext);
                            return ;
                        }
                         /* Replace strcpy with strncpy. Function strcpy
                         * will cause memory corruption if source string is 
                         * larger than Sdf_co_characterBufferSize.
                         */
                        Sdf_mc_strncpy(dTempAddr, pTempHost, (Sdf_co_characterBufferSize-1));
                        dTempAddr[Sdf_co_characterBufferSize-1]='\0';
                        if(sdf_fn_uaDnsQuery(dTempAddr, pDnsEventContext,&dErr) == Sdf_co_fail)
                        {
                            sip_freeSipUrl(pUrl);
                            (Sdf_ty_void)sdf_ivk_uaFreeDestDetails(pDestDetails);
#ifdef SDF_ERROR							
                            sdf_fn_setError(Sdf_en_minorErrors, Sdf_en_destinationInfoError,\
                                    (Sdf_ty_s8bit *)"sdf_fn_uaProcessQueryRespFromDnsClient(): "
                                    "DnsQuery failed", &dErr);
#endif					
                            /* Free DnsEventContext of the Dns Query */
                            (Sdf_ty_void)sdf_ivk_uaFreeDnsEventContext (pDnsEventContext);
                            return ;
                        }
                        sip_freeSipUrl(pUrl);
                    }
                (Sdf_ty_void)sdf_ivk_uaFreeDestDetails(pDestDetails);
                /* Free DnsEventContext of the Dns Query */
                (Sdf_ty_void)sdf_ivk_uaFreeDnsEventContext (pDnsEventContext);
                return;
            } 
        }  
    }/*pResponse->result != ARDNSCL_SUCCESS*/
    else 
    { 
        if ((Sdf_co_null == pResponse->pDnsQueryResp))
        {
            return;
        }      
        
        pAnswerRecord = pResponse->pDnsQueryResp->pAnswerRecords; 
        
        /*Start a for loop equal to number of responses received from DNS server*/
        for (index=1; index <= pResponse->pDnsQueryResp->numRecords; index++)
        {
            if (Sdf_co_null != pAnswerRecord)
            {
                /*Populate each response in DNS EVENT CONTEXT*/
                if (Sdf_co_fail ==  sdf_fn_uaFillQueryRespInDnsEvtCtx(pDnsEventContext, 
                            pAnswerRecord,pResponse->pQuery->query_type,
                            &is_further_query,&dErr))
                {
                    bStatus = Sdf_co_false;
#ifdef SDF_ERROR
                    sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidDNSrecord, 
                            (Sdf_ty_s8bit *)"sdf_fn_uaProcessQueryRespFromDnsClient():"
                            "DNS Response can not be populated in DNSEventContext",&dErr);
#endif
                }
                /*If the response type is NAPTR then break from the loop as we
                  have already processed each answer record in function 
                  Sdf_fn_uaFillQueryrespInDnsEvtCtx*/
                if (ARDNSCL_QUERY_TYPE_NAPTR == pResponse->pQuery->query_type)
                {
                    break;
                }	
                pAnswerRecord = pAnswerRecord->pNext;
            }
            else
            {
                bStatus = Sdf_co_false;
                break;
            }
        }

    }/*pResponse->result == ARDNSCL_SUCCESS**/

    /* Update the lookup module with the current list if IP address found */

    if(sdf_ivk_uaUpdateDnsRecordinTransaction (pDnsEventContext,bStatus,&dErr) \
            == Sdf_co_fail)
    {
        sdf_fn_uaQueryRespProcs(pDnsEventContext,Sdf_co_false);
#ifdef SDF_ERROR
        sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidDNSrecord, 
                (Sdf_ty_s8bit *)"sdf_fn_uaProcessQueryRespFromDnsClient( ):"
                "DNS Record not found",&dErr);
#endif
        /* Free DnsEventContext of the Dns Query */
        (Sdf_ty_void)sdf_ivk_uaFreeDnsEventContext (pDnsEventContext);
        return;
    }

    /* DNS records have been updated, no further query on network, 
       this is understood as dns query has been successful, so we
       trigger SendCallToPeer API*/

    sdf_fn_uaQueryRespProcs(pDnsEventContext, bStatus);

    /* Free DnsEventContext of the Dns Query */
    (Sdf_ty_void)sdf_ivk_uaFreeDnsEventContext (pDnsEventContext);
    return;
}

#endif

/******************************************************************************
*
*  FUNCTION NAME  :   	sdf_fn_uaRecvDnsQueryResp
*
*  DESCRIPTION    :   	This function is used to process the DNS query
*                       response from DNS client.In this function we will read
*                       the buffer from fd and call the unpack routine of DNS
*                       client for unpacking.After unpacking is successful,a
*                       function defined in porting layer of UATK is call to
*                       further process the response.
*
*  RETURNS        :   	Success : If desired options are performed.
*                       Failure : If desired options can not be performed.
*
 ******************************************************************************/
Sdf_ty_dnsRetVal sdf_fn_uaRecvDnsQueryResp()
{
    dnscl_error_t      err = 0;
    /*Number of bytes recived */
    dnscl_int32_t     bytes_recv = 0;
    arDnsClient_response_st      *pResponse = ARDNSCL_NULL;    
    arDnsClient_global_data_st   *pDnsCl_glb_data = ARDNSCL_NULL;
    /*Socket address structure*/ 
    struct sockaddr_in  source_addr;
    /*Socket address length*/
    dnscl_uint32_t  source_addr_len = 0;
    Sdf_ty_dnsRetVal    ret_val = Sdf_co_dns_success;
    
#ifdef ICF_UATK_PORT    
    icf_global_port_info_st	 *p_glb_port_data = ICF_NULL;
    icf_uint8_t        pBuffer[ICF_BLK_SIZE_MAX] = "\0";
    icf_uint32_t       msg_len = ICF_BLK_SIZE_MAX,fd=0;
    icf_uint16_t       buff_len = 0; 
    dnscl_uint32_t     buffer_len = ICF_BLK_SIZE_MAX;

    if((ICF_NULL == p_persistent_glb_pdb) ||
            (ICF_NULL == p_persistent_glb_pdb->p_glb_data) ||
            (ICF_NULL == p_persistent_glb_pdb->p_glb_data->p_glb_port_info))
    {
        return  Sdf_co_dns_fail;
    }

    p_glb_port_data = p_persistent_glb_pdb->p_glb_data->p_glb_port_info;    
    
    pDnsCl_glb_data  =  p_glb_port_data->pDnsclient_glb_data;
    
    fd = p_glb_port_data->pDnsclient_fds->dns_clib_fd;


#else
    Sdf_ty_u8bit        pBuffer[SDF_BLK_SIZE_MAX] = "\0";
    Sdf_ty_u32bit       msg_len = SDF_BLK_SIZE_MAX,fd=0;
    Sdf_ty_u16bit       buff_len = 0; 
    dnscl_uint32_t      buffer_len = SDF_BLK_SIZE_MAX;

    Sdf_st_dnsClientInfo  *pDnsClientInfo = (Sdf_st_dnsClientInfo *)&dDnsClientInfo;
    pDnsCl_glb_data   = (arDnsClient_global_data_st *)pDnsClientInfo->pDnsClientGlbData;
    fd = pDnsClientInfo->dDnsClibFd;
#endif
    /* SPR 19591 : populate the length of the buffer of source address length
     */ 
    source_addr_len = sizeof (struct sockaddr_in);

    /*recv message on UDP*/
    bytes_recv = recvfrom (fd, pBuffer, buffer_len,0,
                 (struct sockaddr *)(&source_addr), &source_addr_len);
    if ( -1 == bytes_recv)
    {
        /*No data present on recv buffer,so send failure */
        return Sdf_co_dns_fail;
    }

    msg_len = bytes_recv;    

    /* unpack the response */
    if(ARDNSCL_FAILURE == arDnsClient_port_unpack_response(
                  pDnsCl_glb_data,
                  pBuffer,
                  &pResponse,
                  &buff_len,
                  &err))
    {
        return  Sdf_co_dns_fail;
    }     
    
    sdf_fn_uaProcessQueryRespFromDnsClient(pResponse);
    
    /*Free the response structure after processing*/
    arDnsClient_free_response(
            pDnsCl_glb_data,
            pResponse,&err);

    return ret_val;
}

#ifndef ICF_UATK_PORT

/*****************************************************************************
 ** FUNCTION:    sdf_fn_uaInitLookup() 
 **
 ** DESCRIPTION: This function is used to initialized the lookup component.
 **              This function is invoked per thread basis. 
 **
 *****************************************************************************/
Sdf_ty_retVal sdf_fn_uaInitLookup(void)
{
    Sdf_st_error        dErr = {0,0,0,"\0"};           

    /*Invoke the porting layer function for getting the IP/PORT for
      DNS CLIENT and DNS CLIB */
    if(Sdf_co_success != sdf_fn_uaGetDnsClientAddr(&dDnsClientInfo))
    {		
#ifdef SDF_ERROR 

        sdf_fn_setError(Sdf_en_majorErrors,dErr.errCode, \
                (Sdf_ty_s8bit *)"sdf_ivk_uaInitLookup( ): " \
                "Failure in getting DNS CLIENT address",&dErr);
#endif
        return Sdf_co_fail;	
    }
    /*Initialised the DNS CLIENT thread*/ 
    if(Sdf_co_success != sdf_fn_uaInitDnsClientThread(&dDnsClientInfo))
    {		
#ifdef SDF_ERROR 

        sdf_fn_setError(Sdf_en_majorErrors,dErr.errCode, \
                (Sdf_ty_s8bit *)"sdf_ivk_uaInitLookup( ): " \
                "Failure while starting the DNS CLIENT thread",&dErr);
#endif
        return Sdf_co_fail;	
    }
    /*Invoke the porting layer function for adding clib fd in read set.
      This is done for reading the DNS query response from DNS CLIENT.*/
    if(Sdf_co_success != sdf_fn_uaAddDnsClibFdToAppFdSet(&dDnsClientInfo))
	{		
#ifdef SDF_ERROR 

        sdf_fn_setError(Sdf_en_majorErrors,dErr.errCode, \
		(Sdf_ty_s8bit *)"sdf_ivk_uaInitLookup( ): " \
        "Failure while adding the DNS CLIB Fd to application Fd Set",&dErr);
#endif
      return Sdf_co_fail;	
	}

    return Sdf_co_success;
}
/******************************************************************************
 ** FUNCTION:    sdf_fn_uaFreeLookup () 
 **
 ** DESCRIPTION: This function is used to free the lookup
 **              component.
 **
 ******************************************************************************/
Sdf_ty_retVal  sdf_fn_uaFreeLookup()
{
    Sdf_st_dnsClientInfo  *pDnsClientInfo = (Sdf_st_dnsClientInfo *)&dDnsClientInfo;
    Sdf_st_error          dErr = {0,0,0,"\0"};    
    /* Error returned by the function arDnsClient_deinit */ 
    dnscl_error_t         err   = 0;

    if ((Sdf_co_null != pDnsClientInfo) && 
            (Sdf_co_null != pDnsClientInfo->pDnsClientGlbData))
    {
        if (ARDNSCL_FAILURE == arDnsClient_deinit(\
                    pDnsClientInfo->pDnsClientGlbData,&err))
        {
#ifdef SDF_ERROR 
            sdf_fn_setError(Sdf_en_majorErrors,dErr.errCode, \
                    (Sdf_ty_s8bit *)"sdf_fn_uaFreeLookup( ): " \
                    "Failure in deinitialising arDnsClient thread",&dErr);
#endif            
            return Sdf_co_fail;
        }        
    }
    return Sdf_co_success;
}

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
Sdf_ty_retVal sdf_fn_uaGetDnsClientAddr(Sdf_st_dnsClientInfo  *pDnsClientInfo)
{
    Sdf_mc_strcpy(pDnsClientInfo->dDnsClientIP, "172.16.105.152");
    Sdf_mc_strcpy(pDnsClientInfo->dDnsClibIP, "172.16.105.152");
    pDnsClientInfo->dDnsClientPort = 23452;
    pDnsClientInfo->dDnsClibPort = 23456;
    return Sdf_co_success;  
}

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
Sdf_ty_retVal sdf_fn_uaAddDnsClibFdToAppFdSet(Sdf_st_dnsClientInfo  *pDnsClientInfo)
{
    sdf_fn_uaSetSocketFd(&dGlbFdSet, pDnsClientInfo->dDnsClibFd);
    return Sdf_co_success;  
}


/***********************************************************************************
 ** FUNCTION:    sdf_fn_uaInitDnsClientThread(Sdf_st_dnsClientInfo  *pDnsClientInfo) 
 **
 ** DESCRIPTION: This function is used initialised the DNS CLIENT thread.
 **
 ***********************************************************************************/
Sdf_ty_retVal sdf_fn_uaInitDnsClientThread(Sdf_st_dnsClientInfo
                                                    *pDnsClientInfo)
{
    /* This variable is passed in init function of arDnsClient
       (arDnsClient_init) containing ip address and its length
       of Dns client */
    arDnsClient_address_st      dnsclient = {{0,""},0};
    /* This variable is passed in init function of arDnsClient
       (arDnsClient_init) containing ip address and its length 
       of Dns clib */
    arDnsClient_address_st      dnsclib = {{0,""},0};
    /* Passed as an output parameter in arDnsClient.It will 
       contain fd of ipc channel opned by dns client */ 
    Sdf_ty_s32bit               dns_client_fd = -1;
    /* Passed as an output parameter in arDnsClient.It will 
       contain fd of ipc channel opned by dns clib */ 
    Sdf_ty_s32bit               dns_clib_fd = -1;
    /* Pointer to global data of arDnsClient */
    Sdf_ty_void                 *pDnscl_glb_data = Sdf_co_null;
    /* error returned by the function arDnsClient_init */ 
    dnscl_error_t               err   = 0;
    pthread_t                   thread_id;

    if (Sdf_co_null == pDnsClientInfo)
    {
        return Sdf_co_fail;
    }

    /*Copy the DNS CLIENT address in arDnsClient structure*/
    Sdf_mc_strcpy(dnsclient.ipAddress.str_val, pDnsClientInfo->dDnsClientIP);
    dnsclient.ipAddress.str_len = Sdf_mc_strlen((Sdf_ty_s8bit *)pDnsClientInfo->dDnsClientIP);
    /* populating the port of dnsclient structure */
    dnsclient.port = pDnsClientInfo->dDnsClientPort;

    /*Copy the DNS CLIB address in arDnsClient structure*/
    Sdf_mc_strcpy(dnsclib.ipAddress.str_val, pDnsClientInfo->dDnsClibIP);
    dnsclib.ipAddress.str_len = Sdf_mc_strlen((Sdf_ty_s8bit *)pDnsClientInfo->dDnsClibIP);
    /* populating the port of dnsclib structure */
    dnsclib.port = pDnsClientInfo->dDnsClibPort; 

    /* Calling the init function of Dns client to initialize
       Dns client global data and getting fds */
    if(ARDNSCL_FAILURE == arDnsClient_init(&dnsclient, &dnsclib,
                Sdf_co_null, &pDnscl_glb_data, &dns_clib_fd,
                &dns_client_fd, &err))
    {
        return Sdf_co_fail;
    }
    else
    {
        if(0 != pthread_create(&thread_id,Sdf_co_null,
                    (dnscl_void_t*)&arDnsClient_start,pDnscl_glb_data))
        {
            return Sdf_co_fail;
        } 
        else
        {
            /*Add the clib fd and global data of arDnsClient in 
              Sdf_st_dnsClientInfo. The fd will be used for reading the response
              from DNS CLIENT and global data of arDnsClient will be used while 
              sending any request to arDnsClient.*/
            pDnsClientInfo->dDnsClibFd = dns_clib_fd;
            pDnsClientInfo->pDnsClientGlbData = pDnscl_glb_data;
        }
    }
    return Sdf_co_success;  
}

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
                             Sdf_ty_bool		      bStatus)
{
    Sdf_st_eventContext      *p_event_context = Sdf_co_null;
    Sdf_st_error             sdf_error = {0,0,0,"\0"};
    SipMessage               *pMessage = Sdf_co_null;
    Sdf_st_callObject        *pCallObj = Sdf_co_null;
    SipOptions               options = {0};
    Sdf_st_transaction 		 *pTransaction = Sdf_co_null;
    Sdf_st_overlapTransInfo  *pOverlapTransaction = Sdf_co_null;
    Sdf_st_msgInfo			 dMessageInfo;
    Sdf_ty_messageType 	   	 dMsgType = Sdf_en_unknownRequest;
    

    options.dOption = SIP_OPT_CLEN|SIP_OPT_FULLFORM;

    if (Sdf_co_fail == bStatus)
    {
        /*Provide the default behaviour in failure scenario*/
        return;
    }
    /*Check the callobject in pDnsEventContext*/
    if ((Sdf_co_null == pDnsEventContext) || 
            (Sdf_co_null == pDnsEventContext->pCallObj))
    {
        return;
    }
    else
    {
        pCallObj = pDnsEventContext->pCallObj;
    }

    /*Fetch the sip meaages which need to be send over network*/
    if((Sdf_co_null != pCallObj->pUasTransaction) && 
            (Sdf_co_null != pCallObj->pUasTransaction->pSipMsg))
    {
        pMessage = pCallObj->pUasTransaction->pSipMsg;
    }
    else if((Sdf_co_null != pCallObj->pUacTransaction) &&
            (Sdf_co_null != pCallObj->pUacTransaction->pSipMsg))
    {
        pMessage = pCallObj->pUacTransaction->pSipMsg;
    }
    else if(sdf_ivk_uaGetTransactionForSipMsg(\
                (pCallObj), pMessage, &pTransaction, \
                &pOverlapTransaction, &sdf_error) == Sdf_co_success)
    {
        if((Sdf_co_null == pOverlapTransaction) &&
                (Sdf_co_null != pTransaction))
        {
            pMessage = pTransaction->pSipMsg;
            sdf_ivk_uaFreeTransaction(pTransaction);
        }
        else if(Sdf_co_null != pOverlapTransaction)
        {
            pMessage = pOverlapTransaction->pSipMsg;
            sdf_ivk_uaFreeOverlapTransInfo(pOverlapTransaction);
        }
    }
    else
    {
        return;
    }

    /*Initilised the Event contect which is used in API sdf_ivk_uaHandleLookup
      and sdf_ivk_uaSendCallToPeer*/
    if (Sdf_co_fail ==
            sdf_ivk_uaInitEventContext(&p_event_context, &sdf_error))
    {
        return;
    }
	else
    {
        p_event_context->pData = pCallObj->pAppData->pData;
    }
    /* Get the Message Type of the SIP message*/   
    if(Sdf_co_success != sdf_fn_uaGetMessageInfo(pMessage, &dMessageInfo,
                &sdf_error))
    {
#ifdef SDF_ERROR		
        sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_headerManipulationError, \
                (Sdf_ty_s8bit *)"sdf_fn_uaQueryRespProcs(): "
                "Failed to get message details from  SIP Message",&sdf_error);
#endif	
        sdf_error.errCode = Sdf_en_headerManipulationError;
        return ;
    }

    dMsgType  = dMessageInfo.dMsgType;	
    
    if (dMsgType != Sdf_en_ack && dMsgType != Sdf_en_bye)    
    {
        /*The the DNS state using API sdf_ivk_uaHandleLookup*/
        if ( Sdf_co_fail == sdf_ivk_uaHandleLookup(pCallObj,
                    Sdf_co_null, pMessage,
                    p_event_context,&sdf_error) &&
                    (Sdf_en_dnsInvState != sdf_error.errCode))
        {
            return ;
        }
    }
    /*Send the Sip message on resoved IP/Port*/ 
    if (( Sdf_co_fail == sdf_ivk_uaSendCallToPeer(pCallObj, pMessage,
                    options, 0, p_event_context, &sdf_error)))
    {
        return;
    }
    sdf_ivk_uaFreeEventContext(p_event_context);
    return;
}
#endif 

#endif /* SDF_LOOKUP */

