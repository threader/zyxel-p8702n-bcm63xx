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
 ** 08-Nov-06 Lakshmi,Sunithi					Modified for Async DNS
 **	                                                            release
 ** 7-Sep-05  Subhash                                           AresData/IPv6 handling 	
 ** 5-Nov-09  Preksha                                           Replace host address 
                                                                strcpy with strncpy
 ** 29-Apr-10 Alok Tiwari               SPR-20385    modified function 
 **                                                  sdf_fn_uaAppendSrvRecord for
 **                                                  correct priority
 ** 05-May-10 Alok Tiwari               SPR-20395    Memory leak resolved to free
 **                                                  the domain name.
 **********************************************************************************
 **      COPYRIGHT , Aricent, 2006
 **********************************************************************************/
#ifdef SDF_LOOKUP
#include "icf_feature_flags.h"
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
#include "sdf_lookup.h"
#include "sdf_common.h"
#ifdef ICF_PORT_WINDOWS
#include "mysysdefines.h"
#else 
#ifdef SDF_ASYNC_DNS
#include <math.h>
#endif/*SDF_ASYNC_DNS*/
#endif

/* ARES specific header files */
#if defined(ICF_PORT_VXWORKS) || defined(SDF_VXWORKS)
#include "resolv/nameser.h"
#else
#ifndef ICF_PORT_WINDOWS
#include <arpa/nameser.h>
#endif
#endif

/* Change for vxWork Compilation */
 

#if defined(SDF_WINDOWS) || defined(SDF_WINCE)
Sdf_st_ThreadKey  dThreadKey;
/*#else
pthread_key_t  dThreadKey;*/
#endif


#ifdef SDF_ASYNC_DNS
extern Sdf_st_fdSetInfo  dGlbAresFdSet;
extern Sdf_st_fdSetInfo	dGlbFdSet;
Sdf_st_hash dGlbAresDataHash;
#endif

/*Sdf_st_ThreadKey  dThreadKey;*/
/*ICF SSA module inclusion*/
#include "icf_port_struct.h"
#include "icf_common_types.h"
#include "icf_common_prototypes.h"
#include "icf_es_prototypes.h"
#include "icf_port_prototypes.h"
#include "icf_ssa_types.h"

extern icf_return_t    icf_ssa_dns_get_ip_from_fqdn(
       IN     icf_uint8_t        *p_fqdn,
       OUT    icf_dns_buffer_st  **p_dns_record,
       OUT    icf_error_t        *p_error
       );

extern icf_void_t icf_ssa_post_dns_resp_processing(
	Sdf_st_dnsEventContext *p_dns_evt_ctxt, 
	Sdf_ty_bool            dns_resp_flag);

extern icf_uint8_t icf_ssa_get_transport_mode(Sdf_st_callObject *pCallObj);

extern icf_return_t icf_ssa_dns_start_dns_resp_timer(
		                   INOUT icf_ssa_ctxt_st *p_ssa_ctxt,
                                   IN  Sdf_st_dnsEventContext *p_dns_evt_ctxt, 
				   IN  icf_void_t *p_ares_ctxt, 
		                   IN  icf_duration_t timer_dur,
		                   OUT icf_error_t   *p_ecode);

extern icf_void_t icf_ssa_stat_dns_type(IN icf_uint32_t dns_query_type);

extern icf_return_t icf_ssa_dns_stop_dns_resp_timer(
                       INOUT	icf_ssa_ctxt_st *p_ssa_ctxt,
                       OUT     icf_error_t    *p_ecode);


extern icf_glb_pdb_st	*p_persistent_glb_pdb;
#define IPADDRSTRING 35

/*****************************************************************************
 ** FUNCTION:   sdf_fn_uaDnsQuery
 **
 ** DESCRIPTION: This function has the routine to query the DNS server 
 **
 **	PARAMETERS:
 ** 	pDomainName(IN)       	    : The given Domain Name
 **   pDnsEventContext(IN)        : Sdf_st_dnsEventContext Structure
 **	 	pErr(OUT)										: Error structure in case the function fail
 *****************************************************************************/
Sdf_ty_retVal sdf_fn_uaDnsQuery (Sdf_ty_s8bit  *pDomainName,
    Sdf_st_dnsEventContext   *pDnsEventContext,
    Sdf_st_error             *pErr)
{
  Sdf_ty_s32bit            count, dnsClass      = C_IN;
  Sdf_ty_s32bit            dType         = 0;
  #ifdef SDF_ASYNC_DNS
  Sdf_ty_bool              dIsToHashAdd  = Sdf_co_false;
  Sdf_ty_u32bit            i = 0, dTempFd;
  Sdf_ty_fdSet 			   dReadfdSet;
  Sdf_ty_socketId		   *pTempAresKey;
  Sdf_st_asyncDNSContextInfo *pAsyncDnsCxtInfo = (Sdf_st_asyncDNSContextInfo*) \
                                                 pDnsEventContext->pAppData;
#ifdef SDF_THREAD_SAFE
	Sdf_ty_threadId 		dCallingThread = 0;
#endif
#endif /*SDF_ASYNCD_DNS*/
  
  Sdf_ty_socketId          dnfds          = 0;
  Sdf_st_aresEventContext  *pAresContext = Sdf_co_null;
  Sdf_ty_s8bit             *pLookup      = Sdf_co_null; 
/*  Sdf_st_aresData          *pAresData    = Sdf_co_null;*/
                         
  Sdf_ty_u16bit            timeout_millisec = 0;

#ifndef SDF_VXWORKS
   Sdf_st_timeVal        stTimeVal ={0,0}, maxtv={0,0};
   Sdf_st_timeVal           *pTimeVal          = Sdf_co_null;
#else
   struct timespec        stTimeVal = {0,0}, maxtv = {0,0};
   struct timespec        *pTimeVal          = Sdf_co_null;
#endif 
  /* Sdf_ty_s32bit nDomain; */
  icf_global_port_info_st		   *p_glb_port_info = 
			p_persistent_glb_pdb->p_glb_data->p_glb_port_info;

  icf_error_t				err;

  icf_dns_buffer_st			*p_dns_record = NULL;
  Sdf_st_transportInfo 		*p_dest_Info = Sdf_co_null;	    
  icf_return_t              ret_val = ICF_FAILURE;
  struct                    server_state *p_server;
  ares_channel          	pChannel;
  Sdf_st_overlapTransInfo   *pOverlapTransInfo = Sdf_co_null;
  Sdf_st_transaction	    *pTransaction	= Sdf_co_null;
  icf_ssa_ctxt_st         *p_ssa_ctxt = ICF_NULL;  
   /* code added for DNS buffer- if the entry is found in the DNS buffer then don't
      send the DNS query */
  if (sdf_ivk_uaGetDestInfoFromTransaction(pDnsEventContext->pTransaction,
			  pDnsEventContext->pOverlappedTransaction,
			  &p_dest_Info, 
			  pErr) == Sdf_co_fail)
  {
	  return Sdf_co_fail;
  }
  if(Sdf_co_null != pDnsEventContext->pTransaction)
  {
        (void)sdf_ivk_uaFreeTransportInfo(pDnsEventContext->pTransaction->pDestInfo);
  }
  else if(Sdf_co_null != pDnsEventContext->pOverlappedTransaction)
  {
	(void)sdf_ivk_uaFreeTransportInfo(pDnsEventContext->pOverlappedTransaction->pDestInfo);
  } 

  ret_val = icf_ssa_dns_get_ip_from_fqdn((icf_uint8_t *)pDomainName,
			  &p_dns_record,  
			  &err); 
  
  if(ICF_SUCCESS == ret_val)
  {
	  /* found the DNS entry with active IP address assign the DNS address and the port*/
	  if (p_dest_Info->pIp != Sdf_co_null)
	  {
		  (Sdf_ty_void)sdf_memfree(Sdf_mc_callHandlerMemId,(Sdf_ty_pvoid*)&\
				  (p_dest_Info->pIp),pErr);
	  }

	  Sdf_mc_strdup(p_dest_Info->pIp, (Sdf_ty_s8bit*)p_dns_record->ip_address);

	  /*	Sdf_mc_strcpy(p_dest_Info->pIp,p_dns_record->ip_address[p_dns_record->active_ip_index]);*/
        /*Fix for CSR 8038197 SPR 20226*/
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
	  /************/
      pOverlapTransInfo = pDnsEventContext->pOverlappedTransaction;
      pTransaction = pDnsEventContext->pTransaction; 
      if(pOverlapTransInfo==Sdf_co_null)
      {
          if (pTransaction == Sdf_co_null)
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
      /************/
    /* this is to keep the information that the record is fethced from DNS Buffer */
      /* this will help to remove the dns entry if no response is received */
      if(ICF_NULL != pDnsEventContext->pCallObj->pAppData->pData)
      {
          icf_ssa_ctxt_st      *p_temp_ssa_ctxt = ICF_NULL; 
          p_temp_ssa_ctxt = (icf_ssa_ctxt_st*)(pDnsEventContext->pCallObj->pAppData->pData);
          p_temp_ssa_ctxt->bitmask |= ICF_DNS_RECORD_FETCHED;
          Sdf_mc_strcpy(p_temp_ssa_ctxt->fqdn.str, pDomainName);

      }
      return Sdf_co_success;

  }
  else
  {
      /* No DNS entry in the buffer store the FQDN in ssa context
         it will be used when response is received to mark the active IP */
      icf_ssa_ctxt_st      *p_temp_ssa_ctxt = ICF_NULL; 
      if(ICF_NULL != pDnsEventContext->pCallObj->pAppData->pData)
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


    /*
     * Check if ares is initialized. If not send a failure for DNS query
     */
     if(ICF_NULL == p_glb_port_info->p_ares_port_info)
     {
	 /* 
	  * Check if any name server is present in resolv.conf
	  * if it is then call ares init else the same would be 
	  * called next time when DNS query would be made
	  */ 
          if(ICF_TRUE == icf_port_is_nameserver_pres())
	  {
	       if(ICF_FAILURE== icf_port_ares_init(p_glb_port_info,&err))
	       { 
#ifdef SDF_ERROR
	            sdf_fn_dumpError((Sdf_ty_s8bit *)"\n ALARM ares init Unsuccessfull. \
			       Would not be supporing DNS query");
#endif
		    return Sdf_co_fail;
	       }
	  }
          else
          {
#ifdef SDF_ERROR
   	      sdf_fn_dumpError((Sdf_ty_s8bit *)"\n ALARM DNS query failed as ares is not \
                          initialized");
	       sdf_fn_dumpError((Sdf_ty_s8bit *)"\n nameserver entry not present in resolv.conf");
#endif
	       return Sdf_co_fail;
          } 
     }

/*  ares_callback Ares_cbk_uaCommonQueryResult; */

/* Getting the Record Type from Transaction's DNS Event Context  */
if (pDnsEventContext->dDnsRecordType == Sdf_en_IPAddress)
#ifndef SDF_IPV6
		pDnsEventContext->dDnsRecordType = Sdf_en_IPv4Address;

				if ((pDnsEventContext->pTransaction != Sdf_co_null) && \
					(pDnsEventContext->pTransaction->dDnsRecordType == \
					Sdf_en_IPAddress))
						
								pDnsEventContext->pTransaction->dDnsRecordType = Sdf_en_IPv4Address;
						
				else if ((pDnsEventContext->pOverlappedTransaction != Sdf_co_null) && \
					      (pDnsEventContext->pOverlappedTransaction->dDnsRecordType == Sdf_en_IPAddress))
								
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
	
	switch(pDnsEventContext->dDnsRecordType)
	{
	case Sdf_en_NAPTRrecord:
        
	case Sdf_en_SRVrecord:
	{
			dType = T_SRV;
		/* SRV record formation Logic.  The following operation will not be required 
		* if NAPTR query is done.
		*/
		pLookup = (Sdf_ty_s8bit *)sdf_memget (Sdf_mc_callHandlerMemId, Sdf_mc_strlen(pDomainName) +12, \
                        pErr);

    	switch (pDnsEventContext->dProtocol)
		 {
		  case Sdf_en_protoAny:
    		Sdf_mc_strcpy(pLookup, "_sip._udp.");
			break;
#ifdef SDF_TLS
	    case Sdf_en_protoTls:
	   		Sdf_mc_strcpy(pLookup, "_sips._tcp.");
			break;
#endif
		case Sdf_en_protoTcp: 
	    	Sdf_mc_strcpy(pLookup, "_sip._tcp.");
			break;
		default:
	    	Sdf_mc_strcpy(pLookup, "_sip._udp.");
		}

		/* Ares specific query */
		Sdf_mc_strcat(pLookup,pDomainName);
		break;
	}	

#ifndef SDF_IPV6
	case Sdf_en_IPv4Address:
	{    	dType = T_A;
        Sdf_mc_strdup(pLookup, pDomainName);
	    	break;
	}			
#else		
	case Sdf_en_IPv6Address:
  {
			dType = T_AAAA;
      Sdf_mc_strdup(pLookup, pDomainName);
		  break;
	}		 
#endif		
	default:
		return Sdf_co_fail;
	}
      
#ifdef SDF_ASYNC_DNS
	if (pAsyncDnsCxtInfo->pAresData == Sdf_co_null)
	{
#endif /*SDF_ASYNC_DNS*/
	
	/* Get Ares specific data from the thread/task specific data
	pAresData = (Sdf_st_aresData *)sdf_fn_uaGetSpecificThreadKey(dThreadKey);

	if (pAresData == Sdf_co_null)
	{
	    (Sdf_ty_void)sdf_memfree(Sdf_mc_callHandlerMemId,(Sdf_ty_pvoid*)&pLookup, pErr);
   		return Sdf_co_fail;
	}*/
#ifdef SDF_ASYNC_DNS
		dIsToHashAdd = Sdf_co_true;
		pAsyncDnsCxtInfo->pAresData = pAresData;
	}
	else
		pAresData = pAsyncDnsCxtInfo->pAresData;
#endif /*SDF_ASYNC_DNS*/
  
  /* Ares specific Eventcontext creation */
  pAresContext = (Sdf_st_aresEventContext *) sdf_memget (Sdf_mc_callHandlerMemId,\
                             sizeof(Sdf_st_aresEventContext), pErr);  
  if (pAresContext == Sdf_co_null)
  {
#ifdef SDF_ERROR
    sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_noMemoryError, \
      (Sdf_ty_s8bit *)"sdf_fn_uaDnsQuery( ):"
      "Failed to allocate memory for AresEventContext structure",pErr);
#endif
    pErr->errCode=Sdf_en_noMemoryError;
    (Sdf_ty_void)sdf_memfree(Sdf_mc_callHandlerMemId,(Sdf_ty_pvoid*)&pLookup, pErr);
    return Sdf_co_fail;
  }

  /* Increment the reference counts in DnsEvent context */
  HSS_LOCKEDINCREF(pDnsEventContext->dRefCount);

  pAresContext->pDnsEventContext = pDnsEventContext;
  /* Save the domain name, this may be needed later to make A query if SRV 
   * query fails */ 
  Sdf_mc_strdup(pAresContext->pDomainName, pDomainName);

  ares_search (p_glb_port_info->p_ares_port_info->p_ares_channel, pLookup, dnsClass, dType,      
              Ares_cbk_uaQueryResult, (Sdf_ty_pvoid)pAresContext);  
  (Sdf_ty_void)sdf_memfree(Sdf_mc_callHandlerMemId,(Sdf_ty_pvoid*)&pLookup, pErr);
  FD_ZERO(&p_glb_port_info->p_ares_port_info->read_fd); 
  FD_ZERO(&p_glb_port_info->p_ares_port_info->write_fd);
  dnfds = ares_fds(p_glb_port_info->p_ares_port_info->p_ares_channel, 
	&p_glb_port_info->p_ares_port_info->read_fd,&p_glb_port_info->p_ares_port_info->write_fd);
  if (dnfds == 0)
    return Sdf_co_fail;

  pTimeVal = ares_timeout(p_glb_port_info->p_ares_port_info->p_ares_channel, 
	&maxtv, &stTimeVal);
   
  if(ICF_NULL != pDnsEventContext->pCallObj->pAppData->pData)
  {
        p_ssa_ctxt = (icf_ssa_ctxt_st*)(pDnsEventContext->pCallObj->pAppData->pData);
   
        pChannel = (ares_channel)p_glb_port_info->p_ares_port_info->p_ares_channel;

       if(ICF_TIMER_ID_INVALID == p_ssa_ctxt->dns_resp_timer_id)
       {
        if (ICF_NULL != p_ssa_ctxt)
        {
           /*Set the Query-Id in Timer Data.This is done to 
            *Free the corresponding AresContext incase of DnsTimerExpiry.
            */
            p_ssa_ctxt->ares_qid = pChannel->qid;
            pChannel->qid = -1;
           
            ret_val =
            icf_ssa_dns_start_dns_resp_timer(p_ssa_ctxt,pDnsEventContext,
                                            (icf_void_t *)pAresContext,
                                            p_ssa_ctxt->dns_retrans_timer,
                                            &err);
           if (ICF_FAILURE == ret_val)
           {
             ICF_PRINT(((icf_uint8_t *)"\n Failure in starting DNS Resp Timer. \n"));
             return ret_val;
           }
        }
        else
        {
          ICF_PRINT(((icf_uint8_t *)"\n SSA ctxt is NULL, So Failure in starting DNS Resp Timer. \n"));
          return ICF_FAILURE;
        }
    }
  }

  for(count = 0; count < pChannel->nservers; count++)
  {
        p_server = &(pChannel->servers[count]);

        if(p_server->qhead && p_server->tcp_socket != -1 &&
            FD_ISSET(p_server->tcp_socket, &p_glb_port_info->p_ares_port_info->write_fd))
        {
            if(Sdf_co_fail == icf_es_set_ares_fds(p_glb_port_info, p_server->tcp_socket, 1, timeout_millisec))
            {
                return Sdf_co_fail;
            }
            else
            {
                p_ssa_ctxt->ares_fd = p_server->tcp_socket;
            }
        }

        if (p_server->tcp_socket != -1 && FD_ISSET(p_server->tcp_socket, &p_glb_port_info->p_ares_port_info->read_fd))
        {
            if(Sdf_co_fail == icf_es_set_ares_fds(p_glb_port_info, p_server->tcp_socket, 0, timeout_millisec))
            {
                return Sdf_co_fail;
            }
            else
            {
                p_ssa_ctxt->ares_fd = p_server->tcp_socket;
            }				
        }

        if (p_server->udp_socket != -1 && FD_ISSET(p_server->udp_socket, &p_glb_port_info->p_ares_port_info->read_fd))
        {
            if(Sdf_co_fail == icf_es_set_ares_fds(p_glb_port_info, p_server->udp_socket, 0, timeout_millisec))
            {
                return Sdf_co_fail;
            }
            else
            {
                p_ssa_ctxt->ares_fd = p_server->udp_socket;
            }				
        }	
   }

  
  pErr->errCode = Sdf_en_noUaError;
  return Sdf_co_success;
}
/*****************************************************************************
 ** FUNCTION:   sdf_fn_uaAppendIpRecord 
 ** DESCRIPTION: The function use to update the Sorted SRV records into the 
 **     Lookup module
 **
 **	PARAMETERS:
 ** 	pslDnsRecords(IN)      : List of Dns Records
 **   pAddrString(IN)        : The IP Address.
 **	 	pErr(OUT)						   : Error structure in case the function fails
 *****************************************************************************/
Sdf_ty_retVal sdf_fn_uaAppendIpRecord (Sdf_ty_slist *pslDnsRecords, 
              Sdf_ty_s8bit 		  *pAddrString, 
        	  Sdf_st_error        *pErr)
{
	Sdf_st_DnsRecord	  *pDnsRec = Sdf_co_null;		

	pDnsRec = (Sdf_st_DnsRecord *)sdf_memget(Sdf_mc_initMemId,
			sizeof(Sdf_st_DnsRecord), pErr);	
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
 ** FUNCTION:   sdf_fn_uaAppendSrvRecord 
 ** DESCRIPTION: The function use to update the Sorted SRV records into the 
 **     Lookup module
 **	PARAMETERS:
 ** 	pslDnsRecords(IN)      : List of Dns Records
 **   pSrvRec(IN)            : Srv Record which is to append in the list.
 **	 	pErr(OUT)						   : Error structure in case the function fails
 *****************************************************************************/
Sdf_ty_retVal sdf_fn_uaAppendSrvRecord (Sdf_ty_slist *pslDnsRecords, 
              Sdf_st_dnsSrvRecord *pSrvRec, 
        	  Sdf_st_error        *pErr)
{
	Sdf_ty_bool           bAddFlag = Sdf_co_false;
	Sdf_ty_u32bit	      dSrvCount;
	Sdf_st_listIterator   dListIterator;
	Sdf_st_dnsSrvRecord   *pTempSrvRec = Sdf_co_null;
	Sdf_st_dnsSrvRecord   *pPrevSrvRec = Sdf_co_null;
	Sdf_st_DnsRecord	  *pDnsRec = Sdf_co_null;		
	Sdf_st_DnsRecord	  *pTempDnsRec = Sdf_co_null;		
	Sdf_ty_u32bit 		  dCount = 1;	

	pDnsRec = (Sdf_st_DnsRecord *)sdf_memget(Sdf_mc_initMemId,
			sizeof(Sdf_st_DnsRecord), pErr);	
	pDnsRec->dDnsRecordType = Sdf_en_SRVrecord;
	(Sdf_ty_void)sdf_listSizeOf (pslDnsRecords, &dSrvCount, pErr);
  /* 
   * The Insertion sorting algorithm is choosen for sorting. The insertion 
   * sort is choosen as the list will contain less than 10 elements.   
   */
	(Sdf_ty_void)sdf_listInitIterator(pslDnsRecords, &dListIterator, pErr);
  
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
                /* Fix for CSR 1-8538021: SPR-20385*/
                /* The function sdf_listInsertAt calculates the position
                 * from the index zero.
                 * To insert pDnsRec on correct position in
                 * pslDnsRecords list,give the parameter as (dCount-1)
                 * (which is the position number where the node pDnsRec
                 * will be inserted in list pslDnsRecords) since the
                 * dcount is intialised with 1.
                 */
				 if(sdf_listInsertAt (pslDnsRecords,(dCount - 1) , pDnsRec,pErr) \
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
                        /* Fix for CSR 1-8538021: SPR-20385*/
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
 ** FUNCTION:   Ares_cbk_uaQueryResult 
 ** DESCRIPTION: The callback thrown by the ARES for the SRV query result
 ** 
 **	PARAMETERS:
 ** 	pArescontext(IN)      : List of Dns Records
 **   dstatus(IN)           : Message Status Type 
 **   pabuf(IN)							: Address Buffer
 **	 	pErr(OUT)						  : Error structure in case the function fails
 *****************************************************************************/
void Ares_cbk_uaQueryResult (
   Sdf_ty_pvoid  pAresContext,
   Sdf_ty_s32bit dstatus, 
   Sdf_ty_u8bit *pabuf, 
   Sdf_ty_s32bit dlen)
{
  Sdf_ty_s8bit           *perrmem           = Sdf_co_null;
  Sdf_ty_u8bit           *paptr             = Sdf_co_null;
  Sdf_ty_u32bit          dqdcount           = 0, dancount = 0, dIndex = 0;
  Sdf_st_dnsEventContext *pDnsEventContext = Sdf_co_null;
  Sdf_st_error           dErr;
  Sdf_ty_bool		 bStatus = Sdf_co_true, is_further_query = Sdf_co_false;	
  Sdf_ty_DnsRecordType   dDnsRecordType;	
  Sdf_st_transportInfo *pDestInfo = Sdf_co_null;
  Sdf_st_destDetails   *pDestDetails = Sdf_co_null;
  Sdf_ty_s8bit         dTempAddr[Sdf_co_characterBufferSize]="\0";
  SipAddrSpec          *pAddrSpec=Sdf_co_null;
  SipUrl		*pUrl = Sdf_co_null;
  Sdf_ty_s8bit       	*pTempHost = Sdf_co_null;
  SipMessage            *pSipMsg = Sdf_co_null;
  Sdf_st_aresEventContext  *pAresCtxt = (Sdf_st_aresEventContext *) \
                                            pAresContext;

  icf_global_port_info_st                  *p_glb_port_info =
                        p_persistent_glb_pdb->p_glb_data->p_glb_port_info;
  icf_ssa_ctxt_st        *p_ssa_ctxt = ICF_NULL;
  icf_error_t             ecode;

  icf_es_rmv_ares_fd(p_glb_port_info);

  if (pAresCtxt == Sdf_co_null)
  {
#ifdef SDF_ERROR
    sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidDNSrecord, 
      (Sdf_ty_s8bit *)"Ares_cbk_uaQueryResult( ):"
      "Failed to get the AresContext structure",&dErr);
#endif
    return;
  }

  if (pAresCtxt->pDnsEventContext == Sdf_co_null)
  {
#ifdef SDF_ERROR
    sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_invalidDNSrecord, 
      (Sdf_ty_s8bit *)"Ares_cbk_uaQueryResult( ):"
      "Failed to get pDnsEventContext from AresContext structure",&dErr);
#endif
    /* Free AresContext */
    return;
  }

  pDnsEventContext = pAresCtxt->pDnsEventContext;
 
  p_ssa_ctxt = (icf_ssa_ctxt_st*)(pDnsEventContext->pCallObj->pAppData->pData);
  
  if(ICF_NULL == p_ssa_ctxt) 
   {
     	/* Free DnsEventContext of the Dns Query */
      (Sdf_ty_void)sdf_ivk_uaFreeDnsEventContext (pDnsEventContext);

      if(Sdf_co_null != pAresCtxt->pDomainName)
      {  
               (Sdf_ty_void)sdf_memfree(
                        Sdf_mc_callHandlerMemId,
                        (Sdf_ty_pvoid*)&(pAresCtxt->pDomainName), &dErr);
       }
  (Sdf_ty_void)sdf_memfree(Sdf_mc_callHandlerMemId,(Sdf_ty_pvoid*)&pAresContext, &dErr);
     return;
   }
   else
   {
     /*Fix for SPR 16713
      * IPTK shall stop this timer here as if no response for SRV query,
      * then fn returned without stopping the DNS query.Ideally, it should stop here.
      */  
     icf_ssa_dns_stop_dns_resp_timer(p_ssa_ctxt,&ecode);
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
  response is received. 
A better approach would be to maintain a map of call objects
to the Query ID of the Ares Query*/
  if ( (0 == pDnsEventContext->dRefCount) ||
	(0 == pDnsEventContext->pCallObj->dRefCount))
  { 
#ifdef SDF_ERROR
    sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidDNSrecord, 
      (Sdf_ty_s8bit *)"Ares_cbk_uaQueryResult( ):"
      "Contexts deleted for the DNS query",&dErr);
#endif
    return;
  }

  if (pDnsEventContext->pCallObj == Sdf_co_null)
  { 
#ifdef SDF_ERROR
    sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidDNSrecord, 
      (Sdf_ty_s8bit *)"Ares_cbk_uaQueryResult( ):"
      "Failed to get the callobject from  AresContext structure",&dErr);
#endif
    return;
  }
  if (pDnsEventContext->pTransaction != Sdf_co_null)
	dDnsRecordType =	pDnsEventContext->pTransaction->dDnsRecordType;
  else
	dDnsRecordType =	pDnsEventContext->pOverlappedTransaction->dDnsRecordType;

  /* Display an error message if there was an error, but only stop if  
   * we actually didn't get an answer buffer.
   */  
	if (dstatus != ARES_SUCCESS)  
	{
/*
#ifdef SDF_TLS
    if(pDnsEventContext->dDnsRecordType == Sdf_en_SRVrecord) 
		{
				if(pDnsEventContext->dUrlType == SipAddrSipSUri)
						if(sdf_ivk_uaGetDestInfoFromTransaction)
						
						 pDestInfo->dPort = Sdf_mc_defaultSipSPort;
					else
#endif
			    	pDestInfo->dPort = Sdf_mc_defaultSipPort; 
		}
*/		
		bStatus = Sdf_co_false;
		if (dstatus == ARES_ETIMEOUT)
    	{
#ifdef SDF_ERROR
	      sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_dnsTimeout, 
    	    (Sdf_ty_s8bit *)"Ares_cbk_uaQueryResult( ):"
        	"DNS query Timeout",&dErr);
#endif    
	
    	}
	    else  if (dstatus == ARES_ENODATA)
	    {  
#ifdef SDF_ERROR
    	  sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidDNSrecord, 
        	(Sdf_ty_s8bit *)"Ares_cbk_uaQueryResult( ):"
	        "DNS Record not found",&dErr);
#endif    
	    }
else  if (dstatus == ARES_ENOTFOUND) 
{
   
     if(pDnsEventContext->dDnsRecordType == Sdf_en_SRVrecord && dstatus != ARES_ECONNREFUSED)
    /* second check is added to not attempt the AA query in case we are not
       able to contact to DNS server */
{	/*
	 * If no SRV Record found in DNS then do A record query taking default
	 * ports
	 */

#ifdef SDF_ERROR
	sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidDNSrecord, 
			(Sdf_ty_s8bit *)"Ares_cbk_uaQueryResult( ):"
			"DNS Record not found doing A query taking default port",&dErr);
#endif  
	if (sdf_ivk_uaGetDestInfoFromTransaction(pDnsEventContext->pTransaction,\
				pDnsEventContext->pOverlappedTransaction,&pDestInfo,&dErr) == Sdf_co_fail)
	{
#ifdef SDF_ERROR				
		sdf_fn_setError(Sdf_en_minorErrors, \
				Sdf_en_transpInfoAccessError, \
				(Sdf_ty_s8bit *)"SDF_ERROR - "
				" Ares_cbk_uaQueryResult ( ): Failed to get "
				"Destination Info from the Transaction", &dErr);
#endif				
		/* Free DnsEventContext of the Dns Query */
		(Sdf_ty_void)sdf_ivk_uaFreeDnsEventContext (pDnsEventContext);

        /* Fix for CSR 1-8564918 : SPR-20395*/ 
	    if(Sdf_co_null != pAresCtxt->pDomainName)
        {  
               (Sdf_ty_void)sdf_memfree(
                        Sdf_mc_callHandlerMemId,
                        (Sdf_ty_pvoid*)&(pAresCtxt->pDomainName), &dErr);
        }

		(Sdf_ty_void)sdf_memfree(Sdf_mc_callHandlerMemId,(Sdf_ty_pvoid*)&pAresContext, &dErr);
		return; 
	}

#ifdef SDF_TLS
		/*if(pDnsEventContext->dDnsRecordType == Sdf_en_SRVrecord) */

		if(pDnsEventContext->dUrlType == SipAddrSipSUri)
			pDestInfo->dPort = Sdf_mc_defaultSipSPort;
		else
#endif
			pDestInfo->dPort = Sdf_mc_defaultSipPort; 

	(Sdf_ty_void)sdf_ivk_uaFreeTransportInfo(pDestInfo);
/*	pDnsEventContext->pTransaction->dDnsRecordType = Sdf_en_IPAddress;
	pDnsEventContext->dDnsRecordType = Sdf_en_IPAddress;*/
	if (pDnsEventContext->pTransaction != Sdf_co_null)
		pDnsEventContext->pTransaction->dDnsRecordType = Sdf_en_IPAddress;
	else
		pDnsEventContext->pOverlappedTransaction->dDnsRecordType= Sdf_en_IPAddress;

	pDnsEventContext->dDnsRecordType = Sdf_en_IPAddress;
	/* 
	 * To get the domain name to be passed for A Query
	 */
      /*  SipMessage   *pSipMsg = Sdf_co_null; */

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
				(Sdf_ty_s8bit *)"Ares_cbk_uaQueryResult  (): "
				"Failed to get the Sip Message.", &dErr);
#endif
		/* Free DnsEventContext of the Dns Query */
		(Sdf_ty_void)sdf_ivk_uaFreeDnsEventContext (pDnsEventContext);

        /* Fix for CSR 1-8564918 : SPR-20395*/ 
	    if(Sdf_co_null != pAresCtxt->pDomainName)
        {  
               (Sdf_ty_void)sdf_memfree(
                        Sdf_mc_callHandlerMemId,
                        (Sdf_ty_pvoid*)&(pAresCtxt->pDomainName), &dErr);
        }

		(Sdf_ty_void)sdf_memfree(Sdf_mc_callHandlerMemId,(Sdf_ty_pvoid*)&pAresContext, &dErr);
                 
                 return;
        }
 
	if((pDnsEventContext->pTransaction != Sdf_co_null) || 
                 (pDnsEventContext->pOverlappedTransaction != Sdf_co_null))
	if (sdf_ivk_uaGetDestinationDetails(pDnsEventContext->pCallObj,pDnsEventContext->pOverlappedTransaction,\
			pSipMsg, &pDestDetails,&dErr ) == Sdf_co_fail)
	{

#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, dErr.errCode, \
				(Sdf_ty_s8bit *)"Ares_cbk_uaQueryResult  (): "
				"Failed to get the target address spec.", &dErr);
#endif
		/* Free DnsEventContext of the Dns Query */
		(Sdf_ty_void)sdf_ivk_uaFreeDnsEventContext (pDnsEventContext);

        /* Fix for CSR 1-8564918 : SPR-20395*/ 
	    if(Sdf_co_null != pAresCtxt->pDomainName)
        {  
               (Sdf_ty_void)sdf_memfree(
                        Sdf_mc_callHandlerMemId,
                        (Sdf_ty_pvoid*)&(pAresCtxt->pDomainName), &dErr);
        }

		(Sdf_ty_void)sdf_memfree(Sdf_mc_callHandlerMemId,(Sdf_ty_pvoid*)&pAresContext, &dErr);

		return ;
	}
	if((pDnsEventContext->pTransaction != Sdf_co_null) || 
                 (pDnsEventContext->pOverlappedTransaction != Sdf_co_null))
	if(pSipMsg->dType==SipMessageRequest)
	{

		pAddrSpec = pDestDetails->u.pAddrSpec;
		if(sdf_ivk_uaGetUrlFromAddrSpec(pAddrSpec, &pUrl,\
					&dErr) == Sdf_co_fail)
		{
			(Sdf_ty_void)sdf_ivk_uaFreeDestDetails(pDestDetails);
#ifdef SDF_ERROR					
			sdf_fn_setError(Sdf_en_minorErrors,\
					Sdf_en_destinationInfoError,\
					(Sdf_ty_s8bit *)"Ares_cbk_uaQueryResult   ( ): "
					"Failed to get Url From Addr Spec",&dErr);
#endif					
			/* Free DnsEventContext of the Dns Query */
			(Sdf_ty_void)sdf_ivk_uaFreeDnsEventContext (pDnsEventContext);

            /* Fix for CSR 1-8564918 : SPR-20395*/ 
	        if(Sdf_co_null != pAresCtxt->pDomainName)
            {  
               (Sdf_ty_void)sdf_memfree(
                        Sdf_mc_callHandlerMemId,
                        (Sdf_ty_pvoid*)&(pAresCtxt->pDomainName), &dErr);
            }

			(Sdf_ty_void)sdf_memfree(Sdf_mc_callHandlerMemId,(Sdf_ty_pvoid*)&pAresContext, &dErr);

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
					(Sdf_ty_s8bit *)"Ares_cbk_uaQueryResult  ( ): "
					"Failed to get Host from Url",&dErr);
#endif					
			/* Free DnsEventContext of the Dns Query */
			(Sdf_ty_void)sdf_ivk_uaFreeDnsEventContext (pDnsEventContext);

            /* Fix for CSR 1-8564918 : SPR-20395*/ 
	        if(Sdf_co_null != pAresCtxt->pDomainName)
            {  
               (Sdf_ty_void)sdf_memfree(
                        Sdf_mc_callHandlerMemId,
                        (Sdf_ty_pvoid*)&(pAresCtxt->pDomainName), &dErr);
            }

			(Sdf_ty_void)sdf_memfree(Sdf_mc_callHandlerMemId,(Sdf_ty_pvoid*)&pAresContext, &dErr);

			return ;
		}
        /* Fix for IPTK CSR 1-7953063 */
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
					(Sdf_ty_s8bit *)" Ares_cbk_uaQueryResult (): "
					"DnsQuery failed", &dErr);
#endif					
			/* Free DnsEventContext of the Dns Query */
			(Sdf_ty_void)sdf_ivk_uaFreeDnsEventContext (pDnsEventContext);

            /* Fix for CSR 1-8564918 : SPR-20395*/ 
	        if(Sdf_co_null != pAresCtxt->pDomainName)
            {  
               (Sdf_ty_void)sdf_memfree(
                        Sdf_mc_callHandlerMemId,
                        (Sdf_ty_pvoid*)&(pAresCtxt->pDomainName), &dErr);
            }

			(Sdf_ty_void)sdf_memfree(Sdf_mc_callHandlerMemId,(Sdf_ty_pvoid*)&pAresContext, &dErr);

			return ;
		}
		sip_freeSipUrl(pUrl);
	}
	(Sdf_ty_void)sdf_ivk_uaFreeDestDetails(pDestDetails);
	/* Free DnsEventContext of the Dns Query */
	(Sdf_ty_void)sdf_ivk_uaFreeDnsEventContext (pDnsEventContext);

        /* Fix for CSR 1-8564918 : SPR-20395*/ 
	    if(Sdf_co_null != pAresCtxt->pDomainName)
        {  
               (Sdf_ty_void)sdf_memfree(
                        Sdf_mc_callHandlerMemId,
                        (Sdf_ty_pvoid*)&(pAresCtxt->pDomainName), &dErr);
        }

	(Sdf_ty_void)sdf_memfree(Sdf_mc_callHandlerMemId,(Sdf_ty_pvoid*)&pAresContext, &dErr);

	return;

} 
	 	
	    else 
	    {
#ifdef SDF_ERROR
   		   sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidDNSrecord, 
        	(Sdf_ty_s8bit *) ares_strerror(dstatus, &perrmem),
	        &dErr);
#endif    
	    }
    	ares_free_errmem(perrmem);  
		
        if (!pabuf) {
 #ifdef SDF_ERROR
      		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidDNSrecord, 
		        (Sdf_ty_s8bit *)"Ares_cbk_uaQueryResult( ):"
		        "DNS Record not found",&dErr);
#endif    
		}  /* pabuf will be null when dstatus is ARES_EDESTRUCTION */ 
	} /* End of dstatus != ARES_SUCCESS */  
        }
	else { /* Else for != ARES_SUCCESS */
	if (dlen < HFIXEDSZ)
	{
 #ifdef SDF_ERROR
    sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidDNSrecord, 
      (Sdf_ty_s8bit *)"Ares_cbk_uaQueryResult( ):"
      "DNS Record not found",&dErr);
#endif    
	bStatus = Sdf_co_false;
	
	}
	else { /* Else For dlen < HFIXEDSZ*/
  /* In case of successful lookup, parse all the anwer records */  
  dqdcount = DNS_HEADER_QDCOUNT(pabuf);  
  dancount = DNS_HEADER_ANCOUNT(pabuf);  

    if(pDnsEventContext->dDnsRecordType == Sdf_en_SRVrecord)
    {
        p_ssa_ctxt->dns_srv_count = dancount;
    }
    else if (0 != p_ssa_ctxt->dns_srv_count)
    {
        p_ssa_ctxt->dns_srv_count--;
    }

  /* Advance aptr by header size */  
  paptr = pabuf + HFIXEDSZ;      

  /* skip the qn secion; */  
  for (dIndex = 0; dIndex < dqdcount; dIndex++) 
  {
    Sdf_ty_s8bit *pname     = Sdf_co_null;
    Sdf_ty_s32bit dqlen     = 0;
    Sdf_ty_s32bit dqnStatus = 0;  

    dqnStatus = ares_expand_name (paptr, pabuf, dlen, &pname,&dqlen);

    if (dqnStatus != ARES_SUCCESS)
    {
      if (pname != Sdf_co_null)
      {
	icf_error_t    ecode;
	if ( ICF_FAILURE == 
	icf_cmn_static_memfree(p_persistent_glb_pdb, pname, &ecode))
	{
		ICF_PRINT(((icf_uint8_t *)"\n[UATK-Port]:Failure in Static Memfree"));
	}
/*
 * ToBeDiscussed With Ganesh: this is giving us errors in Purify as the memroy
 * was allocated through malloc in ares and is being freed through sdf_memfree
 * here.
        (Sdf_ty_void)sdf_memfree(Sdf_mc_callHandlerMemId,(Sdf_ty_pvoid*)&pname,&dErr);
*/
        continue;  
      }
    } /* End of if (dqnStatus != ARES_SUCCESS) */

    paptr +=dqlen;    

    if (paptr+QFIXEDSZ <= pabuf +dlen)  
      paptr += QFIXEDSZ;  

    if (pname != Sdf_co_null)     
    {
	    icf_error_t    ecode;
	    if ( ICF_FAILURE == 
			    icf_cmn_static_memfree(p_persistent_glb_pdb, pname, &ecode))
	    {
		    ICF_PRINT(((icf_uint8_t *)"\n[UATK-Port]:Failure in Static Memfree"));
	    }
	    /*
	     * ToBeDiscussed With Ganesh: this is giving us errors in Purify as the memroy
	     * was allocated through malloc in ares and is being freed through sdf_memfree
	     * here.
	     (Sdf_ty_void)sdf_memfree(Sdf_mc_callHandlerMemId,(Sdf_ty_pvoid*)&pname,&dErr);  
	     */
     }
    } /* End of for(dIndex = 0....) */

	/* Parse the result received from the DNS server */
	for (dIndex = 0;dIndex < dancount; dIndex++ ) 
	{
            Sdf_ty_u8bit num_answers = dancount - dIndex;

	    if (paptr != Sdf_co_null)
    	{
		      /*  display_rr(aptr, pabuf, dlen); */
     		 paptr = (Sdf_ty_u8bit *)sdf_fn_uaParseRR (paptr, pabuf, dlen, (Sdf_ty_pvoid)
                                               pDnsEventContext, &is_further_query, &num_answers,&dErr); 
                 /* All records have been parsed */
                 if(0 == num_answers)
                    break;
                  
    	} /* End of if (aptr != Sdf_co_null) */
		else {
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidDNSrecord, 
        		(Sdf_ty_s8bit *)"Ares_cbk_uaQueryResult( ):"
	        	"Parsing the answer of DNS query failed",&dErr);
#endif 
		}

	} /* End of for(dIndex....) */
	} /* Else For dlen < HFIXEDSZ*/
	} /* End of != ARES SUCCESS*/

	/* Update the lookup module with the current list if IP address found */
/*    if (pDnsEventContext->dDnsRecordType == dDnsRecordType || */

	
if (((Sdf_co_false == is_further_query) ||
	    (bStatus == Sdf_co_false)) && (0 == p_ssa_ctxt->dns_srv_count))
      {
	    if(sdf_ivk_uaUpdateDnsRecordinTransaction (pDnsEventContext,bStatus,&dErr) \
			    == Sdf_co_fail)
	    {
	            icf_ssa_post_dns_resp_processing(pDnsEventContext,Sdf_co_false);
#ifdef SDF_ERROR
		    sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidDNSrecord, 
				    (Sdf_ty_s8bit *)"Ares_cbk_uaQueryResult( ):"
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

	if(Sdf_co_null != pAresCtxt->pDomainName)
        {  
               (Sdf_ty_void)sdf_memfree(
                        Sdf_mc_callHandlerMemId,
                        (Sdf_ty_pvoid*)&(pAresCtxt->pDomainName), &dErr);
        }
	
  (Sdf_ty_void)sdf_memfree(Sdf_mc_callHandlerMemId,(Sdf_ty_pvoid*)&pAresContext, &dErr);
	return;
}


/*****************************************************************************
 ** FUNCTION:   sdf_fn_uaParseRR
 ** DESCRIPTION: The function  parse the RR record and store it in the List 
 ** 
 **	PARAMETERS:
 ** 	paptr(IN)      : List of Dns Records
 **   pabuf(IN)      : Address Buffer
 **   dalen(IN)			 : Length of the address
 **   pAppData(IN)   : Sdf_st_appData Structure
 **	 	pErr(OUT)			 : Error structure in case the function fails
 *****************************************************************************/
Sdf_ty_u8bit* sdf_fn_uaParseRR  (Sdf_ty_u8bit *paptr,
                                 Sdf_ty_u8bit *pabuf,
                                 Sdf_ty_s32bit dalen,
                                 Sdf_ty_pvoid pAppData,
				 Sdf_ty_bool *p_query_flag,
                                 Sdf_ty_u8bit *pcount,
                                 Sdf_st_error *pErr)
{
  Sdf_ty_s32bit dtype, dttl, dlen, dstatus, dlength;
  Sdf_st_dnsSrvRecord    *pSrvRec;  
  Sdf_ty_s8bit           *psrvAddress=Sdf_co_null; 
  Sdf_ty_s8bit*          dname=Sdf_co_null; 
  Sdf_ty_u16bit          dPriority = 0,dWeight = 0,dPort = 0;
  Sdf_st_dnsEventContext *pTmpDnsEventCtxt = Sdf_co_null;
  Sdf_ty_u32bit          dCurrentTime    = 0;
  Sdf_ty_u8bit dcount=0;
  Sdf_ty_s8bit *p_host_string=Sdf_co_null;
  Sdf_st_dnsEventContext *pDnsEventCtxt = (Sdf_st_dnsEventContext *)pAppData;
  icf_error_t    ecode;


  dcount = *pcount;

   
  /* parse the RR name */
 dstatus = ares_expand_name (paptr, pabuf, dalen, &dname, &dlength);

  if (dstatus != ARES_SUCCESS) 
    return Sdf_co_null;

  paptr += dlength;
  	/* NOTE: Free any allocated memory */
  {
	  if ( ICF_FAILURE == 
			  icf_cmn_static_memfree(p_persistent_glb_pdb, dname, &ecode))
	  {
		  ICF_PRINT(((icf_uint8_t *)"\n[UATK-Port]:Failure in Static Memfree"));
	  }
	  /*
	   * ToBeDiscussed With Ganesh: this is giving us errors in Purify as the memroy
	   * was allocated through malloc in ares and is being freed through sdf_memfree
	   * here.
	   (Sdf_ty_void)sdf_memfree(Sdf_mc_callHandlerMemId,(Sdf_ty_pvoid*)&dname, pErr);*/
  }
 
  /*
   * Make sure there is enough data after the RR name for the fixed
   * part of the RR
   */

  if (paptr + RRFIXEDSZ > pabuf + dalen)
  {  
    return Sdf_co_null;
  }

   /* Parse the fixed part of the RR, and advance to the RR data field */
  dtype     = DNS_RR_TYPE (paptr);
  dttl     = DNS_RR_TTL(paptr); 
  dlen     = DNS_RR_LEN(paptr);
  paptr     += RRFIXEDSZ;

  if (paptr + dlen > pabuf + dalen) 
  {  
    return Sdf_co_null;    
  } 

  switch(dtype)
  {
    case T_SRV:
    {
       /* The RR data is three two-byte numbers representing  
        * the dPriority, dWeight, and dPort, followed by a domain name
        */  
        dPriority  = DNS__16BIT(paptr);
        dWeight    = DNS__16BIT(paptr + 2);
        dPort      = DNS__16BIT(paptr + 4);  
        dstatus     = ares_expand_name(paptr + 6, pabuf, dalen, &psrvAddress, &dlength);  

        if (dstatus != ARES_SUCCESS)
	{
		icf_error_t    ecode;
		if ( ICF_FAILURE == 
				icf_cmn_static_memfree(p_persistent_glb_pdb, 
					psrvAddress, &ecode))
		{
			ICF_PRINT(((icf_uint8_t *)"\n[UATK-Port]:Failure in Static Memfree"));
		}
		/*
		 * ToBeDiscussed With Ganesh: this is giving us errors in Purify as the memroy
		 * was allocated through malloc in ares and is being freed through sdf_memfree
		 * here.
		 (Sdf_ty_void)sdf_memfree(Sdf_mc_callHandlerMemId,(Sdf_ty_pvoid*)&psrvAddress, pErr);
		 return Sdf_co_null;*/
	}
  
          /* SRV record  Filling  */
        if (sdf_ivk_uaInitDnsSrvRecord (&pSrvRec, pErr) != Sdf_co_success)
        {
          /* Free any allocated memory */
#ifdef SDF_ERROR
          sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidDNSrecord, 
                          (Sdf_ty_s8bit *)"sdf_fn_uaParseRR( ): Failed to allocate"
                          " memory for SRV record", pErr);
#endif
          return Sdf_co_null;
        }
	
		if(sdf_fn_uaGetCurrentTimeInSecs(&dCurrentTime) != Sdf_co_success)
		{
						/* Free the Srv rec here */
#ifdef SDF_ERROR
          sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_unknownMethodError, 
                 (Sdf_ty_s8bit *)"sdf_fn_uaParseRR( ): Failed to"  
                 " get Current Time", pErr);
#endif
          return Sdf_co_null;
	
		}
	
        pSrvRec->dPriority = dPriority;
        pSrvRec->dWeight = dWeight;
        pSrvRec->dPort = dPort; 
        pSrvRec->dttl = dttl + dCurrentTime;

        Sdf_mc_strdup(pSrvRec->pTarget, psrvAddress);
       
	   /* ARES allocates memory using malloc */
       /* (Sdf_ty_void)sdf_memfree(Sdf_mc_callHandlerMemId,(Sdf_ty_pvoid*)&psrvAddress, pErr);*/
        free(psrvAddress);

        /* A or AAAA record query  */

        /*  Event Context Filling  */
        if(sdf_ivk_uaInitDnsEventContext (&pTmpDnsEventCtxt, pErr) != Sdf_co_success)
		{
			(Sdf_ty_void)sdf_memfree(Sdf_mc_callHandlerMemId,(Sdf_ty_pvoid*)&pSrvRec, pErr);

#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidDNSrecord, 
            	(Sdf_ty_s8bit *)"sdf_fn_uaParseRR( ): Failed to " 
				"Initialize DNS Event context memory for SRV record", pErr);
 
#endif
			return Sdf_co_null;
		}
		
 
        HSS_LOCKEDINCREF(pDnsEventCtxt->pCallObj->dRefCount);
        if (pDnsEventCtxt->pTransaction != Sdf_co_null)
        	HSS_LOCKEDINCREF(pDnsEventCtxt->pTransaction->dRefCount);
        if (pDnsEventCtxt->pOverlappedTransaction != Sdf_co_null)
          	HSS_LOCKEDINCREF(pDnsEventCtxt->pOverlappedTransaction->dRefCount);
	
        pTmpDnsEventCtxt->pCallObj               = pDnsEventCtxt->pCallObj;
        pTmpDnsEventCtxt->pTransaction           = pDnsEventCtxt->pTransaction;
        pTmpDnsEventCtxt->pOverlappedTransaction = 
                                         pDnsEventCtxt->pOverlappedTransaction;
        pTmpDnsEventCtxt->pslRecordList          = &(pSrvRec->slIpRecords);
#ifndef SDF_IPV6				
        pTmpDnsEventCtxt->dDnsRecordType         = Sdf_en_IPv4Address;
#else
		pTmpDnsEventCtxt->dDnsRecordType         = Sdf_en_IPv6Address;
#endif
      				
        pTmpDnsEventCtxt->dProtocol		         = Sdf_en_protoNone;

        /* Insert the record into the list */
      if(sdf_fn_uaAppendSrvRecord (pDnsEventCtxt->pslRecordList, 
									pSrvRec, pErr) != Sdf_co_success)
		{
			(Sdf_ty_void)sdf_memfree(Sdf_mc_callHandlerMemId,(Sdf_ty_pvoid*)&pSrvRec, pErr);

#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidDNSrecord, 
            	(Sdf_ty_s8bit *)"sdf_fn_uaParseRR( ): Failed to " 
				"Append Srv Record memory for SRV record", pErr);
#endif
					return Sdf_co_null;		
		}
       	if(sdf_fn_uaDnsQuery(pSrvRec->pTarget, pTmpDnsEventCtxt, \
						pErr) !=Sdf_co_success) 
		{
						(Sdf_ty_void)sdf_memfree(Sdf_mc_callHandlerMemId,(Sdf_ty_pvoid*)&pSrvRec, pErr);
#ifdef SDF_ERROR
						sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidDNSrecord, 
               				(Sdf_ty_s8bit *)"sdf_fn_uaParseRR(): Failed to " 
							 "Make DnsQuery", pErr);
#endif
				return Sdf_co_null;		
		}

		/*ICF Addition*/
		*p_query_flag = Sdf_co_true;
        if(pTmpDnsEventCtxt != Sdf_co_null)
		{
		/* Free DnsEventContext of the Dns Query */
		(Sdf_ty_void)sdf_ivk_uaFreeDnsEventContext (pTmpDnsEventCtxt);
        }
        pSrvRec = Sdf_co_null;
        break;
    } /* End of case T_SRV */
    case T_NAPTR:
    {
       /*Domain TTL Class Type Order Preference Flags Service Regexp Replacement*/
        Sdf_ty_u16bit dOrder, dPreference; 
        Sdf_ty_u32bit doffset; 
        Sdf_ty_s8bit  *pservice,*pregexp,*pflags;
	Sdf_ty_s8bit *phosts[10]; 
        Sdf_ty_u8bit  hostcount=0;
        Sdf_ty_s8bit  tls_index,udp_index,tcp_index,trans_index;

        tls_index=udp_index=tcp_index=trans_index=0;
        
	/* Parse the NAPTR responses in a loop */ 
	do
        {
                doffset=0;
		dOrder  = DNS__16BIT(paptr);
		doffset+=2;
		dPreference = DNS__16BIT(paptr + doffset);
		doffset+=2;

		dlength = *(paptr+doffset);
		doffset++;
		sdf_ivk_uaExpandString((Sdf_ty_s8bit *)(paptr + doffset), &pflags,dlength);
                (Sdf_ty_void) sdf_memfree(Sdf_mc_callHandlerMemId, (Sdf_ty_pvoid *) \
			&pflags, Sdf_co_null);
		doffset += dlength;

		dlength = *(paptr+doffset);
		doffset++;
		dstatus    = sdf_ivk_uaExpandString((Sdf_ty_s8bit *)(paptr + doffset), &pservice, dlength);
                (Sdf_ty_void) sdf_memfree(Sdf_mc_callHandlerMemId, (Sdf_ty_pvoid *) \
			&pservice, Sdf_co_null);
		doffset +=dlength;

		dlength = *(paptr+doffset);
		doffset++;
		dstatus    = sdf_ivk_uaExpandString((Sdf_ty_s8bit *)(paptr + doffset), &pregexp, dlength);
                (Sdf_ty_void) sdf_memfree(Sdf_mc_callHandlerMemId, (Sdf_ty_pvoid *) \
			&pregexp, Sdf_co_null);
		doffset +=dlength;


	        /* Store this entry in an array */	
		dstatus     = ares_expand_name(paptr + doffset, pabuf, dalen, &psrvAddress, &dlength);  

		if (dstatus != ARES_SUCCESS)
		{
			icf_error_t    ecode;
			if ( ICF_FAILURE == 
					icf_cmn_static_memfree(p_persistent_glb_pdb, 
						psrvAddress, &ecode))
			{
				ICF_PRINT(((icf_uint8_t *)"\n[UATK-Port]:Failure in Static Memfree"));
			}
			return Sdf_co_null;
		}
                else
		{
			phosts[hostcount++]=psrvAddress;
			if(psrvAddress == Sdf_mc_strstr(psrvAddress,"_sips._tcp"))
			{
				tls_index = hostcount;
			}
			else if(psrvAddress == Sdf_mc_strstr(psrvAddress,"_sip._tcp"))
			{
				tcp_index = hostcount;
			}
			else if(psrvAddress == Sdf_mc_strstr(psrvAddress,"_sip._udp"))
			{
				udp_index = hostcount;
			}
		}
                if(--dcount)
                {
                        paptr+=dlen;

			/* parse the RR name */
			dstatus = ares_expand_name (paptr, pabuf, dalen, &dname, &dlength);

			if (dstatus != ARES_SUCCESS) 
				return Sdf_co_null;

			paptr += dlength;
			/* NOTE: Free any allocated memory */
			{
				icf_error_t    ecode;
				if ( ICF_FAILURE == 
						icf_cmn_static_memfree(p_persistent_glb_pdb, dname, &ecode))
				{
					ICF_PRINT(((icf_uint8_t *)"\n[UATK-Port]:Failure in Static Memfree"));
				}
			}

			/*
			 * Make sure there is enough data after the RR name for the fixed
			 * part of the RR
			 */

			if (paptr + RRFIXEDSZ > pabuf + dalen)
			{  
				return Sdf_co_null;
			}

			/* Parse the fixed part of the RR, and advance to the RR data field */
			dtype     = DNS_RR_TYPE (paptr);
			dttl     = DNS_RR_TTL(paptr); 
			dlen     = DNS_RR_LEN(paptr);
			paptr     += RRFIXEDSZ;

			if (paptr + dlen > pabuf + dalen) 
			{  
				return Sdf_co_null;    
			} 
		}
	}while(dcount);
	/*  get the transport mode from ssa, choose transport appropriately and set in Transaction */
	switch(icf_ssa_get_transport_mode(pDnsEventCtxt->pCallObj))
	{
#ifdef SDF_TLS
		case ICF_TRANSPORT_TYPE_TLS :
			{
				trans_index = tls_index?tls_index:(tcp_index?tcp_index:udp_index);
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
            Sdf_ty_s8bit trans_mode[5];
            Sdf_ty_s8bit *pMethod;
            Sdf_ty_messageType  msg_type;
            SipMessage   *pSipMsg = Sdf_co_null;
            SipError            sip_error;
#ifdef SDF_TLS
            if(trans_index == tls_index)
            {
               Sdf_mc_strcpy(trans_mode, "TLS");
	    }
            else 
#endif
            if(trans_index == tcp_index)
            {
               Sdf_mc_strcpy(trans_mode, "TCP");
	    }
            else if(trans_index == udp_index)
            {
               Sdf_mc_strcpy(trans_mode, "UDP");
	    }
	    /* Set the transport mode */
	    if (Sdf_co_fail == sdf_ivk_uaSetTransportSchemeInTransaction(
				    pDnsEventCtxt->pTransaction,
				    pDnsEventCtxt->pOverlappedTransaction,
				    trans_mode,
				    pErr))
	    {
		    return  Sdf_co_null;
	    }


        pSipMsg = (Sdf_co_null == pDnsEventCtxt->pTransaction)?\
                  pDnsEventCtxt->pOverlappedTransaction->pSipMsg:\
                  pDnsEventCtxt->pTransaction->pSipMsg;
        sdf_ivk_uaGetMethodFromSipMessage(pSipMsg,&pMethod,pErr);
        sdf_fn_uaGetMethodNameAsEnum(pMethod, &msg_type, pErr);
        if(SipFail == sip_deleteHeaderAtIndex(
                    pSipMsg, SipHdrTypeVia,0, &sip_error))
        {
            return Sdf_co_null;
        }

        /*generate the new via header to reflect the new
         * transport scheme selected*/
        else if(Sdf_co_fail == sdf_fn_uaFormViaHeader(pDnsEventCtxt->pCallObj,
                    pDnsEventCtxt->pOverlappedTransaction,msg_type,pSipMsg, pErr))
        {
            return Sdf_co_null;
        }




        /*  Event Context Filling  */
        if(sdf_ivk_uaInitDnsEventContext (&pTmpDnsEventCtxt, pErr) != Sdf_co_success)
		{
			(Sdf_ty_void)sdf_memfree(Sdf_mc_callHandlerMemId,(Sdf_ty_pvoid*)&pSrvRec, pErr);

#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidDNSrecord, 
            	(Sdf_ty_s8bit *)"sdf_fn_uaParseRR( ): Failed to " 
				"Initialize DNS Event context memory for SRV record", pErr);
 
#endif
			return Sdf_co_null;
		}
		
 
        HSS_LOCKEDINCREF(pDnsEventCtxt->pCallObj->dRefCount);
        if (pDnsEventCtxt->pTransaction != Sdf_co_null)
        	HSS_LOCKEDINCREF(pDnsEventCtxt->pTransaction->dRefCount);
        if (pDnsEventCtxt->pOverlappedTransaction != Sdf_co_null)
          	HSS_LOCKEDINCREF(pDnsEventCtxt->pOverlappedTransaction->dRefCount);
	
        pTmpDnsEventCtxt->pCallObj               = pDnsEventCtxt->pCallObj;
        pTmpDnsEventCtxt->pTransaction           = pDnsEventCtxt->pTransaction;
	pTmpDnsEventCtxt->pOverlappedTransaction = 
                                         pDnsEventCtxt->pOverlappedTransaction;
        pTmpDnsEventCtxt->pslRecordList          = pDnsEventCtxt->pslRecordList;
	pTmpDnsEventCtxt->dDnsRecordType         = Sdf_en_SRVrecord;

      				
        pTmpDnsEventCtxt->dProtocol		         = Sdf_en_protoNone;
        p_host_string = Sdf_mc_strstr(phosts[trans_index-1],".")+1;
        p_host_string = Sdf_mc_strstr(p_host_string,".")+1;

       	if(sdf_fn_uaDnsQuery(p_host_string, pTmpDnsEventCtxt, \
						pErr) !=Sdf_co_success) 
		{
#ifdef SDF_ERROR
						sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidDNSrecord, 
               				(Sdf_ty_s8bit *)"sdf_fn_uaParseRR(): Failed to " 
							 "Make DnsQuery", pErr);
#endif
				return Sdf_co_null;		
		}

		/* Free DnsEventContext of the Dns Query */
		(Sdf_ty_void)sdf_ivk_uaFreeDnsEventContext (pTmpDnsEventCtxt);

	    *p_query_flag = Sdf_co_true;
	}
        else
        {
            return Sdf_co_null;
        }
        while(hostcount)
        {
	  if ( ICF_FAILURE == 
			  icf_cmn_static_memfree(p_persistent_glb_pdb, phosts[--hostcount], &ecode))
	  {
		  ICF_PRINT(((icf_uint8_t *)"\n[UATK-Port]:Failure in Static Memfree"));
	  }
        }
    }
    /* Set *pcount to 0 to indicate that all records have been parsed */
    *pcount=0;
    break;

#ifndef SDF_IPV6
    case T_A:
    {
      /* The RR data is a four-byte Internet address */
		Sdf_ty_s8bit *pAddrString = Sdf_co_null;
		Sdf_ty_DnsRecordType          dDnsRecordType;


	    if (dlen !=4)
        	return Sdf_co_null;
	
   /*   pAddrString = (Sdf_ty_s8bit *)sdf_memget(Sdf_mc_callHandlerMemId,
         sizeof (Sdf_mc_INET_ADDRSTRLEN)+1, pErr); 
		Temporary memory fix...needs to analyse the exact memory size */

	    pAddrString = (Sdf_ty_s8bit *)sdf_memget(Sdf_mc_callHandlerMemId,
        	IPADDRSTRING, pErr);  
#if defined (SDF_WINDOWS) || defined (SDF_VXWORKS)  || defined(SDF_WINCE)
		(Sdf_ty_void)sdf_fn_uaInetNtoP(AF_INET,paptr, pAddrString, Sdf_mc_INET_ADDRSTRLEN); 
#else

		(Sdf_ty_void)inet_ntop(AF_INET,paptr, pAddrString, Sdf_mc_INET_ADDRSTRLEN); 
#endif
		if (pDnsEventCtxt->pTransaction != Sdf_co_null)
		{
			dDnsRecordType =  pDnsEventCtxt->pTransaction->dDnsRecordType;	
		}
		else
		{
			dDnsRecordType =  pDnsEventCtxt->pOverlappedTransaction->dDnsRecordType;	
		}

		if (dDnsRecordType == Sdf_en_IPv4Address)
			(Sdf_ty_void)sdf_fn_uaAppendIpRecord(pDnsEventCtxt->pslRecordList, pAddrString, pErr);
		else
        {
			/* Insert the record into the list */
		    if(sdf_listAppend(pDnsEventCtxt->pslRecordList, pAddrString, pErr) !=Sdf_co_success)
				{
/*    Not required 
		(Sdf_ty_void)sdf_memfree(Sdf_mc_callHandlerMemId,(Sdf_ty_pvoid)&pDnsEventCtxt,pErr); */
						(Sdf_ty_void)sdf_memfree(Sdf_mc_callHandlerMemId,(Sdf_ty_pvoid*)&pAddrString,pErr);
#ifdef SDF_ERROR
    sdf_fn_setError(Sdf_en_majorErrors, pErr->stkErrCode, 
      (Sdf_ty_s8bit *)"sdf_fn_uaParseRR( ):"
      "Failed to Append the Record in list",pErr);
#endif
		return Sdf_co_null;
				}
				
		}
		break;
    } /* End of case T_A*/
#else    
    case T_AAAA:
    {
	    /* The RR data is a four-byte Internet address */
    	Sdf_ty_s8bit *pAddrString = Sdf_co_null;
		Sdf_ty_DnsRecordType 	dDnsRecordType;
	      if (dlen < 4)
        	return Sdf_co_null;
           
		pAddrString=(Sdf_ty_s8bit*)sdf_memget(Sdf_mc_callHandlerMemId,
				IPADDRSTRING,pErr);		  
		(Sdf_ty_void)sdf_fn_uaInetNtoP(AF_INET6,paptr,pAddrString,Sdf_mc_INET6_ADDRSTRLEN);
         
		if (pDnsEventCtxt->pTransaction != Sdf_co_null)
		{
			dDnsRecordType =  pDnsEventCtxt->pTransaction->dDnsRecordType;	
		}
		else
		{
			dDnsRecordType =  pDnsEventCtxt->pOverlappedTransaction->dDnsRecordType;	
		}
		if (dDnsRecordType == Sdf_en_IPv6Address)
		{
			(Sdf_ty_void)sdf_fn_uaAppendIpRecord(pDnsEventCtxt->pslRecordList, pAddrString, pErr);

    }
		else
		{
	    /* Insert the record into the list */
	 			if(sdf_listAppend(pDnsEventCtxt->pslRecordList, pAddrString, pErr) !=Sdf_co_success)
				{
						/* Not required 
						(Sdf_ty_void)sdf_memfree(Sdf_mc_callHandlerMemId,(Sdf_ty_pvoid)&pDnsEventCtxt,pErr); */
						(Sdf_ty_void)sdf_memfree(Sdf_mc_callHandlerMemId,(Sdf_ty_pvoid*)&pAddrString,pErr);
#ifdef SDF_ERROR
						sdf_fn_setError(Sdf_en_majorErrors, pErr->stkErrCode, 
						(Sdf_ty_s8bit *)"sdf_fn_uaParseRR( ):"
						"Failed to Append the Record in list",pErr);
#endif
						return Sdf_co_null;
				}
		}
    	break;
    } /* End of case T_AAAA*/
#endif	
  
	default:
    	break;
  } /* End of switch(type) */   

  return paptr+dlen;
}

Sdf_ty_u8bit sdf_ivk_uaExpandString(Sdf_ty_s8bit *p_src_str,Sdf_ty_s8bit **p_p_dest_str, Sdf_ty_u32bit length)
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


Sdf_ty_retVal sdf_ivk_uaFreeAresContext(Sdf_ty_pvoid p_ares_ctxt,Sdf_ty_u16bit id)
{
     Sdf_st_error        dErr;
     Sdf_st_aresEventContext *pAresContext =
        (Sdf_st_aresEventContext *)p_ares_ctxt;
     icf_global_port_info_st                  *p_glb_port_info =
                        p_persistent_glb_pdb->p_glb_data->p_glb_port_info;
     icf_ares_fds_st    *p_ares_fds = ICF_NULL;
     p_ares_fds =
                (icf_ares_fds_st*)p_glb_port_info->p_ares_port_info;
     free_query(p_ares_fds->p_ares_channel, id);

     if(Sdf_co_null != pAresContext)
     {
          if(Sdf_co_null != pAresContext->pDomainName)
         {
           (Sdf_ty_void)sdf_memfree(
                   Sdf_mc_callHandlerMemId,
               (Sdf_ty_pvoid*)&(pAresContext->pDomainName), &dErr);
         }

          (Sdf_ty_void)sdf_memfree(Sdf_mc_callHandlerMemId,(Sdf_ty_pvoid*)&pAresContext, &dErr);
     }

     return Sdf_co_success;
}
#if 0 /* commented as not required in ICF implementation*/
/*****************************************************************************
 ** FUNCTION:    sdf_fn_uaInitLookup() 
 **
 ** DESCRIPTION: This function is used to initialized the lookup component.
 **              This function is invoked per thread basis. 
 **
 *****************************************************************************/
Sdf_ty_retVal sdf_fn_uaInitLookup(void)
{
  Sdf_st_aresData     *pAresData;
  Sdf_ty_u32bit       dstatus;
  Sdf_st_error        dErr;           

	if(sdf_fn_uaInitAresData(&pAresData, &dErr) != Sdf_co_success)
	{		
#ifdef SDF_ERROR 
	
		sdf_fn_setError(Sdf_en_majorErrors,dErr.errCode, \
		(Sdf_ty_s8bit *)"sdf_ivk_uaInitLookup( ): " \
	  	"Intialization of ares data failed	",&dErr);
#endif
      return Sdf_co_fail;	
	}
  dstatus = ares_init (&(pAresData->pChannel),AF_INET);  

  if (dstatus != ARES_SUCCESS)  
  {
    if(ARES_ECONNREFUSED == dstatus)       
    {         
#ifdef SDF_ERROR 
	  sdf_fn_setError(Sdf_en_majorErrors, dErr.errCode, \
		(Sdf_ty_s8bit *)"sdf_ivk_uaInitLookup( ): "
	  	"None of the Name servers are "
	" compatible with selected Network Type",&dErr);
#endif      
      return Sdf_co_success;
    }            
    else            
    {          
#ifdef SDF_ERROR 
	  sdf_fn_setError(Sdf_en_majorErrors, dErr.errCode, \
		(Sdf_ty_s8bit *)"sdf_ivk_uaInitLookup( ): "
	  	"Intialization of ares library failed	",&dErr);
#endif
      return Sdf_co_fail; 
     }   
  }

  /* dThreadKey previously created */
 /* if (sdf_fn_uaSetSpecificThreadKey(dThreadKey, (Sdf_ty_pvoid*)&pAresData) != \
                                    Sdf_co_success)
  {
    return Sdf_co_fail;
  }*/
  return Sdf_co_success;
}

/*********************************************************************
 ** FUNCTION:   sdf_fn_uaFreeLookup () 
 **
 ** DESCRIPTION: This function is used to free the lookup
 **  component.
 **
 *********************************************************************/
Sdf_ty_retVal  sdf_fn_uaFreeLookup(Sdf_ty_pvoid pVoidData)
{
	Sdf_st_aresData	*pAresData = (Sdf_st_aresData *) pVoidData;

	if (pVoidData != Sdf_co_null)
	{
		ares_destroy(pAresData->pChannel);
/*		(Sdf_ty_void) sdf_fn_uaFreeAresData (pAresData);*/
	}

  	return Sdf_co_fail;
}
/*****************************************************************
** FUNCTION:sdf_fn_uaInitAresData()
**
**
** DESCRIPTION: It Creates the Thread Key for Gloable to all thread
** 
**	PARAMETERS:
**   pAresData(IN)   : Sdf_st_aresData Structure
**   pErr(OUT)			  : Error structure in case the function fails 

*******************************************************************/
Sdf_ty_retVal sdf_fn_uaInitAresData
(Sdf_st_aresData **ppAresData,
 Sdf_st_error  *pErr)
{
    *ppAresData = (Sdf_st_aresData *)sdf_memget(Sdf_mc_callHandlerMemId,\
         sizeof (Sdf_st_aresData), pErr);

    if(*ppAresData == Sdf_co_null)
      return Sdf_co_fail;

    return Sdf_co_success;
}
/*****************************************************************
** FUNCTION:sdf_fn_uaFreeAresData()
** DESCRIPTION: It Frees the Ares Data
** PARAMETERS:
**   pAresData(IN)   : Sdf_st_aresData Structure
**     
*******************************************************************/
void sdf_fn_uaFreeAresData (Sdf_st_aresData *pAresData)
{
	if (pAresData != Sdf_co_null)
	{	
	    (Sdf_ty_void) sdf_memfree(Sdf_mc_callHandlerMemId, (Sdf_ty_pvoid *) \
			&pAresData, Sdf_co_null);
	}
}
#endif/*if 0 */
#endif

