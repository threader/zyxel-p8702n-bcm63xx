/*****************************************************************************
 ** FUNCTION:
 **                     This file contains utility functions invoked by the SSA
 **
 *****************************************************************************
 **
 ** FILENAME:           icf_ssa_transport.c
 **
 ** DESCRIPTION:        This file contains the transport related implementation
 **
 ** DATE                NAME            REFERENCE               REASON
 ** ----                ----            ---------               ------
 ** 20Feb2005           Aashish                             Initial
 ** 05 MAY 2005        Vidyut                                changed the include files
 **                                to port on windows
 ** 07-JUL-2005         Aashish Suchdev                     TCP checkin            
 ** 08-Feb-2006         Amit Sharma               Network PDU Logging API from ICF
 ** 03-Mar-2006         Amit Sharma                     ICF merging with ICF2.0
 ** 23-Mar-2006     Amit Sharma                      Code merged with ICF 4.2
 ** 30-Jun-2006        Umang Singh        Rel 5.2           TLS support
 ** 29-Nov-2006        Umang Singh        SPR 13633
 ** 08-JUN-2007        Alok Tiwari        Rel early 7.0     Added code for stat 
 **                                                         collection of TCP/TLS/UDP connection    
 ** 28-NOV-2007        Jagmohan Chauhan   PQT support       Memory leak fix for spr:17569 
 *****************************************************************************
 **                             Copyrights 2006, Aricent.
 *****************************************************************************/

/*OS inclusions Start*/
#if !defined(ICF_PORT_WINDOWS) && !defined(ICF_PORT_SYMBIAN) 
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>

#endif 

#ifdef ICF_PORT_SYMBIAN
#include <_ansi.h>
#include <sys\types.h>
#include <netinet/in.h>
#endif
/*OS inclusions End*/

#include "icf_feature_flags.h"
#include "stack_headers.h"
#include "uatk_headers.h"

#include "icf_common_interface_types.h"
#include "icf_dbm_prototypes.h"
#include "icf_common_types.h"
#include "icf_common_interface_types.h"
#include "icf_common_defs.h"
#include "icf_internal_api_id.h"
#include "icf_macro.h"
#include "icf_port_defs.h"

#include "icf_ssa_macro.h"
#include "icf_ssa_defs.h"
#include "icf_ssa_types.h"
#include "icf_ssa_prototypes.h"

#ifdef ICF_DNS_LOOKUP_ENABLED
#include "sdf_lookup.h"
#endif

#ifdef ICF_PORT_WINDOWS
#pragma warning(disable:4701)
#pragma warning ( disable : 4702 )
#endif

#ifdef ICF_SIGCOMP_SUPPORT
#include "icf_sigsa_defs.h"

extern icf_return_t icf_sigsa_check_n_compress_sip_message(
                  icf_void_t    *p_sigsa_ctxt,
                  icf_uint8_t   transPortType,
                  icf_uint8_t   *p_UnCompBuffer,
                  icf_uint32_t  buffLen,
                  icf_uint8_t   **pp_CompBuffer,
                  icf_uint32_t  *pCompBuffLen,
                  icf_error_t   *p_ecode);

#endif


/*Defined in Eshell*/
extern icf_glb_pdb_st        *p_persistent_glb_pdb;



#ifdef SDF_TXN

/*****************************************************************************
 ** FUNCTION:     sdf_cbk_uaSendMsgOnNetwork
 **
 ** DESCRIPTION: send the message to peer
 **
 *****************************************************************************/
Sdf_ty_retVal sdf_cbk_uaSendMsgOnNetwork 
#ifdef ANSI_PROTO
    (Sdf_st_callObject    *pCallObj, 
     Sdf_ty_s8bit        *pBuffer, 
     Sdf_ty_u32bit         dBuflen, 
     SipTranspAddr        *pTranspaddr, 
     Sdf_ty_protocol     dTransptype, 
     Sdf_st_error        *pError)
#else
    (pCallObj, pBuffer, dBuflen, pTranspaddr, dTransptype, pError)
     Sdf_st_callObject    *pCallObj;
     Sdf_ty_s8bit        *pBuffer; 
     Sdf_ty_u32bit         dBuflen; 
     SipTranspAddr        *pTranspaddr; 
     Sdf_ty_protocol     dTransptype; 
     Sdf_st_error        *pError;
#endif
{
    icf_error_t          ecode;
    icf_return_t         ret_val = ICF_SUCCESS;
    icf_ssa_tcpm_conn_data_st *p_conn = ICF_NULL;

    icf_ssa_ctxt_st       *p_ssa_ctxt = 
        (icf_ssa_ctxt_st*)(pCallObj->pAppData->pData);

     icf_ssa_pdb_st       ssa_pdb, *p_ssa_pdb = ICF_NULL;


    /*This assignment is a temporary fix*/
    icf_glb_pdb_st       *p_glb_pdb = p_persistent_glb_pdb;

    icf_uint8_t          *p_sip_pdu_start = ICF_NULL;
    /*p_ssa_ctxt->p_glb_pdb;*/

    Sdf_ty_retVal           sdf_ret_val = Sdf_co_success;

    icf_uint8_t          *p_buffer_to_send_on_nw = (icf_uint8_t *)pBuffer;
    icf_uint32_t         buffLenSipMsg = dBuflen;
    icf_config_data_st    *p_glb_cfg_data = ICF_NULL; 
 
#ifdef ICF_SIGCOMP_SUPPORT
    icf_void_t            *p_sigsa_ctxt = ICF_NULL;
#endif
#ifdef ICF_DNS_LOOKUP_ENABLED
    Sdf_ty_s32bit         dTemp = 0,count=0;
    Sdf_st_DnsRecord    *pDnsRecord = ICF_NULL;
    icf_uint8_t         dTemp2 = 0, count2 = 0;
    SipTranspAddr          *pCloneTranspaddr = Sdf_co_null;
    Sdf_ty_pvoid         *pIp = ICF_NULL;
#endif
    (void) pError;
    (void) pCallObj;
   
    if(ICF_NULL == p_ssa_ctxt)
        return Sdf_co_fail;
 
    if(ICF_TRUE == p_ssa_ctxt->no_retrans_req)
        return Sdf_co_success;

    icf_ssa_init_pdb(&ssa_pdb, p_glb_pdb);
    p_ssa_ctxt->p_ssa_pdb = &ssa_pdb;
    ssa_pdb.p_ssa_ctxt = p_ssa_ctxt;
    ssa_pdb.p_ssa_glb_data = p_ssa_ctxt->p_ssa_glb_data;

    if (ICF_FAILURE == icf_dbm_get_module_glb_data(
        p_glb_pdb,
        (icf_int_module_id_t)ICF_INT_MODULE_CFG,
        (icf_void_t **)&p_glb_cfg_data,
        &ecode))
    {
        ICF_PRINT(((icf_uint8_t *)"\n[SSA]:FailureInGettingCfgGlbData in SendMsgOnNetwork Cbk"));
        return Sdf_co_fail;
    }
    if (ICF_NULL != p_ssa_ctxt->p_call_ctxt)
        p_ssa_ctxt->line_id = p_ssa_ctxt->p_call_ctxt->line_id;
    else if (ICF_NULL != p_ssa_ctxt->p_sic_ctxt)
        p_ssa_ctxt->line_id = p_ssa_ctxt->p_sic_ctxt->line_id;
    else if (ICF_NULL != p_ssa_ctxt->p_rgm_ctxt)
    {
        p_ssa_ctxt->line_id = 
        p_ssa_ctxt->p_rgm_ctxt->rgm_list_of_lines[p_ssa_ctxt->p_rgm_ctxt->num_of_lines - 1];
    }
    p_ssa_ctxt->p_ssa_pdb->p_glb_cfg_data = p_glb_cfg_data;

#ifdef ICF_SIGCOMP_SUPPORT
#ifdef ICF_IPSEC_ENABLED
    if (ICF_TRUE == p_ssa_ctxt->p_ssa_pdb->p_glb_cfg_data->sigcomp_required)
    {
        if (ICF_TRUE == p_ssa_ctxt->p_ssa_pdb->p_glb_cfg_data->ipsec_enabled)
        {
            icf_rgm_glb_data_st         *p_rgm_glb_data  = ICF_NULL;

            if (ICF_FAILURE == \
                            icf_dbm_get_module_glb_data(p_ssa_ctxt->p_ssa_pdb->p_glb_pdb,\
                            ICF_INT_MODULE_REGM, 
                            (icf_void_t **)(p_rgm_glb_data),
                            p_ssa_ctxt->p_ssa_pdb->p_ecode))
            {
                /*ICF_SIGSA_TRACE(("\nFailure while getting RGM Global Data in SIGSA\n"));*/
                return ICF_FAILURE;
            }

            p_sigsa_ctxt = p_rgm_glb_data->p_sigsa_ctxt;
        }
        else
        {
            p_sigsa_ctxt = p_ssa_ctxt->p_sigsa_ctxt;
        }
    }
#else
    if (ICF_TRUE == p_ssa_ctxt->p_ssa_pdb->p_glb_cfg_data->sigcomp_required)
    {
        p_sigsa_ctxt = p_ssa_ctxt->p_sigsa_ctxt;
    }
#endif
#endif

    if ( ICF_NULL != p_ssa_ctxt)
    {
            p_ssa_pdb = (icf_ssa_pdb_st*)(p_ssa_ctxt->p_ssa_pdb);
        
        pError = pError;
#ifdef ICF_SIGCOMP_SUPPORT
        if (ICF_TRUE == p_ssa_pdb->p_glb_cfg_data->sigcomp_required)
        {
            /* Check and Compress the Message By Calling the Utility
             * function of SIGSA*/
            if((ICF_NULL != p_ssa_ctxt) &&
               (ICF_NULL != p_sigsa_ctxt) &&
               (ICF_SSA_SIP_MSG_SHOULD_BE_COMPRESSED ==
                   ICF_SSA_GET_COMMON_BITMASK(p_ssa_ctxt->bitmask,\
                     ICF_SSA_SIP_MSG_SHOULD_BE_COMPRESSED)))
            {
                icf_uint8_t transPortType = ICF_SIGSA_TRANSPORT_TYPE_UDP;
                icf_uint32_t CompBuffLen = 0;

                if(dTransptype == Sdf_en_protoTcp)
                {
                    transPortType = ICF_SIGSA_TRANSPORT_TYPE_TCP;
                }
        
                if (ICF_FAILURE == 
                    icf_sigsa_check_n_compress_sip_message(
                            p_sigsa_ctxt,transPortType,
                            p_buffer_to_send_on_nw,buffLenSipMsg,
                            &pCompBuffer,&CompBuffLen,&ecode))
                {
                    ICF_PRINT(("\n[SSA]:Not Able to Compress the Message\n"));
                    ICF_PRINT(("\n[SSA]:Trying to Send UnCompressed Message\n"));
                }
                else
                {
                    if(ICF_NULL != pCompBuffer)
                    {
                        p_buffer_to_send_on_nw = pCompBuffer;
                        buffLenSipMsg = CompBuffLen;
                    }
                    
                }
            }
        }
#endif



        if (dTransptype == Sdf_en_protoUdp)
        {
            ICF_SSA_RESET_COMMON_BITMASK(p_ssa_ctxt->bitmask, \
                    ICF_SSA_INVITE_PENDING)

            ICF_SSA_RESET_COMMON_BITMASK(p_ssa_ctxt->bitmask, \
                    ICF_SSA_REG_PENDING)

            ICF_SSA_RESET_COMMON_BITMASK(p_ssa_ctxt->bitmask, \
                ICF_SSA_SUBS_PENDING)
            
            ICF_SSA_RESET_COMMON_BITMASK(p_ssa_ctxt->bitmask, \
                ICF_SSA_MESSAGE_PENDING)
            
            ICF_SSA_RESET_COMMON_BITMASK(p_ssa_ctxt->bitmask, \
                ICF_SSA_PUBLISH_PENDING)
            
            ICF_SSA_RESET_COMMON_BITMASK(p_ssa_ctxt->bitmask, \
                ICF_SSA_REFER_PENDING)
            
            ICF_SSA_RESET_COMMON_BITMASK(p_ssa_ctxt->bitmask, \
                ICF_SSA_OPTION_PENDING)

			ICF_MEMGET(p_persistent_glb_pdb, buffLenSipMsg, ICF_MEM_COMMON,\
					p_sip_pdu_start,ICF_RET_ON_FAILURE,\
					&ecode, ret_val)
			icf_port_memcpy(p_sip_pdu_start, 
								  p_buffer_to_send_on_nw, buffLenSipMsg);

            #if ICF_IPSEC_ENABLED
            if (ICF_TRUE == p_glb_cfg_data->ipsec_enabled)
            {
                /* check SA is estd. */
                /* if this context has a vlid REGM context set in it */
                if ((p_ssa_ctxt->ssa_sa_context.p_rgm_context) && 

                    (p_ssa_ctxt->ssa_sa_context.p_sa_context))
                {
                    icf_port_set_sa_enabled(
                        p_glb_pdb->p_glb_data->p_glb_port_info, 
                        ICF_TRUE);
                }
                else
                {
                    /* no SA exists */
                    icf_port_set_sa_enabled(
                        p_glb_pdb->p_glb_data->p_glb_port_info, 
                        ICF_FALSE);
                }
            }
            else
            {
                /* IPSEC not configured hence no SA valid */
                icf_port_set_sa_enabled(
                    p_glb_pdb->p_glb_data->p_glb_port_info, 
                    ICF_FALSE);
            }
#endif
        if (ICF_FAILURE == 
                icf_ssa_dispatch_pdu_to_es(p_glb_pdb, p_ssa_ctxt, pTranspaddr, 
                    dTransptype, p_sip_pdu_start, buffLenSipMsg, 
                    p_ssa_ctxt->line_id, &ecode))
        {
            sdf_ret_val = Sdf_co_fail;
        }

#ifdef ICF_DNS_LOOKUP_ENABLED

			if (p_buffer_to_send_on_nw[0] == 'A' && 
				p_buffer_to_send_on_nw[1] == 'C' && 
				p_buffer_to_send_on_nw[2] == 'K' )



			{
				/* Clone the transport address so that the original 
				 * one is not modified */
				ICF_MEMGET(p_glb_pdb,
						ICF_PORT_SIZEOF(SipTranspAddr),
						ICF_MEM_COMMON,
						pCloneTranspaddr,
						ICF_RET_ON_FAILURE,
						p_ssa_pdb->p_ecode,
						ret_val)
					icf_port_strcpy((icf_uint8_t *)pCloneTranspaddr->dIpv4, (icf_uint8_t *)pTranspaddr->dIpv4);
				pCloneTranspaddr->dPort = pTranspaddr->dPort;
				pCloneTranspaddr->pData = pTranspaddr->pData;
				Sdf_mc_strdup(pCloneTranspaddr->pHost, pTranspaddr->pHost);
				pCloneTranspaddr->dSockFd = pTranspaddr->dSockFd;
#ifdef SDF_RPORT
				pCloneTranspaddr->dReceiversPort = pTranspaddr->dReceiversPort;
				Sdf_mc_strdup(pCloneTranspaddr->pReceiversIp, 
						pTranspaddr->pReceiversIp);
#endif
				sdf_ivk_uaGetRecordCountFromUACTransaction(
						pCallObj,&count,pError);

				for (dTemp=0; dTemp < count; dTemp++) 
				{
					sdf_ivk_uaGetRecordFromUACTransaction(
							pCallObj, dTemp, &pDnsRecord, pError);

					/* IP Address */
					if (pDnsRecord->dDnsRecordType == Sdf_en_IPv4Address || 
							pDnsRecord->dDnsRecordType == Sdf_en_IPv6Address || 
							pDnsRecord->dDnsRecordType == Sdf_en_IPAddress)
					{
						/* Get the transport information */
						Sdf_mc_strdup(pCloneTranspaddr->pHost, 
									  pDnsRecord->pValue);
						
						/* Post the message */
						ICF_MEMGET(
							p_persistent_glb_pdb,
							buffLenSipMsg,
							ICF_MEM_COMMON,
							p_sip_pdu_start,
							ICF_RET_ON_FAILURE,
							&ecode,
							ret_val)
						icf_port_memcpy(
								p_sip_pdu_start, 
								p_buffer_to_send_on_nw,
								buffLenSipMsg);
						if (ICF_FAILURE == icf_ssa_dispatch_pdu_to_es(
									p_glb_pdb, p_ssa_ctxt, pCloneTranspaddr, dTransptype, 
									p_sip_pdu_start, buffLenSipMsg, 
									p_ssa_ctxt->line_id, &ecode))
						{
							sdf_ret_val = Sdf_co_fail;
						}

						ICF_MEMFREE(
								p_glb_pdb,pCloneTranspaddr->pHost,
								ICF_MEM_COMMON, p_ssa_pdb->p_ecode, ret_val);
					} /* IP Address */

					/* SRV Record */
					else if (pDnsRecord->dDnsRecordType == Sdf_en_SRVrecord)
					{
						count2 = (icf_uint8_t)((Sdf_st_dnsSrvRecord *)pDnsRecord->pValue)
							->slIpRecords.size;

						for (dTemp2 = 0; dTemp2 < count2; dTemp2++)
						{
							sdf_listGetAt(
									&(((Sdf_st_dnsSrvRecord *)pDnsRecord->pValue)
										->slIpRecords),
									dTemp2,
									(Sdf_ty_pvoid *)&pIp,
									pError);
							Sdf_mc_strdup(pCloneTranspaddr->pHost, (const char *)pIp);
							pCloneTranspaddr->dPort = 
								((Sdf_st_dnsSrvRecord *)pDnsRecord->pValue)->dPort;

							/* Post the message */
							ICF_MEMGET(
									p_persistent_glb_pdb,
									buffLenSipMsg,
									ICF_MEM_COMMON,
									p_sip_pdu_start,
	                            ICF_RET_ON_FAILURE,
	                            &ecode,
	                            ret_val)
	                        icf_port_memcpy(
	                            p_sip_pdu_start,
	                            p_buffer_to_send_on_nw,
	                            buffLenSipMsg);
	                        if (ICF_FAILURE == icf_ssa_dispatch_pdu_to_es(
	                            p_glb_pdb, p_ssa_ctxt, pCloneTranspaddr, dTransptype,
                	            p_sip_pdu_start, buffLenSipMsg, 
				    p_ssa_ctxt->line_id, &ecode))
				{
					sdf_ret_val = Sdf_co_fail;
				}

				ICF_MEMFREE(
						p_glb_pdb,pCloneTranspaddr->pHost,
						ICF_MEM_COMMON, p_ssa_pdb->p_ecode, ret_val);
						} /* for (dTemp2) */
					} /* SRV Record */

					/* NAPTR or any other record type */
					else
					{
						/* do nothing for now */
					}
				} /* for (dTemp) */

				/* Free the cloned transport address structure */
				if (ICF_NULL != pCloneTranspaddr->pHost)
				{
					ICF_MEMFREE(
							p_glb_pdb,pCloneTranspaddr->pHost,
							ICF_MEM_COMMON, p_ssa_pdb->p_ecode, ret_val);
				}
#ifdef SDF_RPORT
				if (ICF_NULL != pCloneTranspaddr->pReceiversIp)
				{
					ICF_MEMFREE(
							p_glb_pdb,pCloneTranspaddr->pReceiversIp,
							ICF_MEM_COMMON, p_ssa_pdb->p_ecode, ret_val);
				}
#endif
				ICF_MEMFREE(p_glb_pdb, pCloneTranspaddr,
						ICF_MEM_COMMON, p_ssa_pdb->p_ecode, ret_val);
			} /* p_buffer_to_send_on_nw = "ack" */
#endif			
		}
		else
		{
			/* Send message on TCP */
            en_SipMessageType dMsgType;
            dMsgType = icf_ssa_get_msg_type(p_ssa_ctxt->p_ssa_pdb,p_ssa_ctxt->p_call_obj);
            if (((dMsgType == SipMessageRequest) &&
                 (ICF_NULL == p_ssa_ctxt->p_conn_table_req_entry)) ||
                ((dMsgType == SipMessageResponse) &&
                 (ICF_NULL == p_ssa_ctxt->p_conn_table_resp_entry))) 
            {
                p_conn = icf_ssa_get_conn_db( pTranspaddr->dSockFd, 
                        p_ssa_pdb->p_ssa_glb_data->p_ssa_tcpm_conn_db);

                /*Hitting this means that there is no connection associated
                 * with this call in the SSA DB. In that case a replacement
                 * conn must have been triggered by UATK through Connect 
                 * cbk.(via theSM). So we search for the conn db node here 
				 * for the given FD and set that index in the SSA CTXT*/
                if ( ICF_NULL == p_conn)
                {
                    ICF_PRINT(((icf_uint8_t *)"\n[SSA]:ConnDbNode corresponding to FD: %d \
                                given by UATK not found in DB",pTranspaddr->dSockFd));
                    return Sdf_co_fail;
                }
                else if ((dMsgType == SipMessageRequest) && 
                         (ICF_CONN_TYPE_REQUEST & p_conn->prot_conn_bitmask))
                { 
                    p_ssa_ctxt->p_conn_table_req_entry = p_conn;
                }
                else if ((dMsgType == SipMessageResponse) && 
                         (ICF_CONN_TYPE_RESPONSE & p_conn->prot_conn_bitmask))
                { 
                    p_ssa_ctxt->p_conn_table_resp_entry = p_conn;
                }
            }
            if (ICF_NULL != p_conn)
            {
                p_ssa_pdb->p_conn_table_entry = p_conn;

                if ((dMsgType == SipMessageRequest) &&
                             (ICF_CONN_TYPE_REQUEST & p_conn->prot_conn_bitmask))
                {
                    p_ssa_pdb->p_conn_table_entry = p_ssa_ctxt->p_conn_table_req_entry;
                    p_ssa_pdb->p_conn_table_entry->prot_conn_bitmask |= ICF_CONN_TYPE_REQUEST;
                }
                if ((dMsgType == SipMessageResponse)  &&
                             (ICF_CONN_TYPE_RESPONSE & p_conn->prot_conn_bitmask))
                {
                    p_ssa_pdb->p_conn_table_entry = p_ssa_ctxt->p_conn_table_resp_entry;
                    p_ssa_pdb->p_conn_table_entry->prot_conn_bitmask |= ICF_CONN_TYPE_RESPONSE;
                }
            }
            else
            {
                if (Sdf_co_fail == sdf_ret_val)
                {
                   return ICF_FAILURE; 
                }
                if (dMsgType == SipMessageRequest)
                {
                    p_ssa_pdb->p_conn_table_entry = p_ssa_ctxt->p_conn_table_req_entry;
                    p_ssa_pdb->p_conn_table_entry->prot_conn_bitmask |= ICF_CONN_TYPE_REQUEST;
                }
                if (dMsgType == SipMessageResponse)
                {
                    p_ssa_pdb->p_conn_table_entry = p_ssa_ctxt->p_conn_table_resp_entry;
                    p_ssa_pdb->p_conn_table_entry->prot_conn_bitmask |= ICF_CONN_TYPE_RESPONSE;
                }
            }
            ICF_SSA_PRINT_CONN_DB(p_ssa_pdb->p_conn_table_entry)

			/*Here I am explicitly checking the state of the connection 
			 * to avoid repetition of code in handlers for state other
			 * than AWT CONN RESP.In all these I just need to invoke 
			 * ES to write the messge to CONN*/

			if ( ICF_SSA_TCPM_AWT_CON_RESP != p_ssa_pdb->
			 p_conn_table_entry->tcpm_sm_state)
			{ 
				ICF_MEMGET(p_glb_pdb, buffLenSipMsg, \
						ICF_MEM_COMMON,\
				p_sip_pdu_start,ICF_RET_ON_FAILURE,\
				&ecode, ret_val)
				icf_port_memcpy(p_sip_pdu_start, 
				  p_buffer_to_send_on_nw, buffLenSipMsg);
				if (ICF_FAILURE == 
						icf_ssa_dispatch_pdu_to_es(p_glb_pdb, \
							p_ssa_ctxt, pTranspaddr, 
							dTransptype,p_sip_pdu_start, buffLenSipMsg, 
							p_ssa_ctxt->line_id, &ecode))
				{
					sdf_ret_val = Sdf_co_fail;
				}
				else
				{
					ICF_SSA_RESET_COMMON_BITMASK(p_ssa_ctxt->bitmask, \
							ICF_SSA_INVITE_PENDING)
					ICF_SSA_RESET_COMMON_BITMASK(p_ssa_ctxt->bitmask, \
							ICF_SSA_REG_PENDING)
				}
			}
			else
			{
				p_ssa_pdb->p_stream_pdu = p_buffer_to_send_on_nw;
				p_ssa_pdb->pdu_len = buffLenSipMsg;
			}
					/*Invoke the TCPM SM to write the message on connection*/
			ICF_SSA_IVK_TCPSM(p_ssa_pdb,ICF_SSA_TCPM_WRITE_TO_CONN, \
					ret_val)
			if ( ICF_FAILURE == ret_val)
			{
				ICF_PRINT(((icf_uint8_t *)"[SSA]:FailureInSM, returning failure for SendMsgOnNetwork\n"));
				sdf_ret_val = Sdf_co_fail;
			}
		}
	}/*if p_ssa_ctxt != NULL*/
	else
	{
		        ICF_MEMGET(p_persistent_glb_pdb, dBuflen, ICF_MEM_COMMON,\
         

                p_sip_pdu_start,ICF_RET_ON_FAILURE,\
                &ecode, ret_val)
        icf_port_memcpy(p_sip_pdu_start, pBuffer, dBuflen);
        if (ICF_FAILURE == 
                icf_ssa_dispatch_pdu_to_es(p_glb_pdb, p_ssa_ctxt, pTranspaddr, 
                    dTransptype, p_sip_pdu_start, dBuflen, 
                    p_ssa_ctxt->line_id, &ecode))
        {
            sdf_ret_val = Sdf_co_fail;
        }



	}


#ifdef ICF_SIGCOMP_SUPPORT
    if (ICF_TRUE == p_ssa_pdb->p_glb_cfg_data->sigcomp_required)
    {
        if(ICF_NULL != pCompBuffer)
        {
            /* Mem Free the Compresedd Buffer, As This is allocated by SIGSA
             * and Its SSA responsible to Free it*/
            ICF_MEMFREE(p_glb_pdb,pCompBuffer,\
                ICF_MEM_COMMON, p_ssa_pdb->p_ecode, ret_val);

        }
    }
#endif
    /*pBuffer[dBuflen] = '\0';*/
   /* icf_print_pdu(p_glb_pdb, pBuffer, pTranspaddr->pHost, pTranspaddr->dPort, dBuflen);*/
    if (ICF_NULL != p_ssa_ctxt)
    {
        p_ssa_ctxt->p_pending_txn = ICF_NULL;       
        p_ssa_ctxt->p_ssa_pdb = ICF_NULL;
    }
    return sdf_ret_val;
}

#endif

/*If it is micro uatk 2.2, then the following function will be visible irrespective of
 * SDF_NETWORKCALLBACKS flag, if it is not UATK2.2, then the visibility depends on
 * Snetwork call backs flag as per UATK 2.1*/
#if defined ICF_WITH_MUATK_2_2 || defined SDF_NETWORKCALLBACKS
/******************************************************************************
 ** FUNCTION:      sdf_cbk_uaGetHostIp
 **
 ** DESCRIPTION: This function invokes the port wrapper for gethostbyname to
 **              resolve the HostName in pHost to an IP address.  The memory
 **              for IP address is allocated by the port and is released by
 **              UATK when it's processing is complete
 ******************************************************************************/
Sdf_ty_retVal sdf_cbk_uaGetHostIp
#ifdef ANSI_PROTO
    (Sdf_ty_s8bit* pHost,
    Sdf_ty_s8bit** ppDest,
    Sdf_st_error *pError)
#else
    (pHost,ppDest,pError)
    Sdf_ty_s8bit* pHost;
    Sdf_ty_s8bit** ppDest;
    Sdf_st_error *pError;
#endif
{
    Sdf_ty_retVal       sdf_ret_val = Sdf_co_success;

    if(ICF_FAILURE == icf_port_get_host_ip(pHost,ppDest))
    {
        pError->errCode = Sdf_en_systemError;
        sdf_ret_val = Sdf_co_fail;
    }

    return sdf_ret_val;
}
#endif /*defined ICF_WITH_MUATK_2_2 || defined SDF_NETWORKCALLBACKS*/

#ifdef SDF_NETWORKCALLBACKS
/******************************************************************************
 ** FUNCTION:      sdf_cbk_uaGetHostName
 **
 ** DESCRIPTION: This function return the machines local host name, the memory
 **                 is allocated by this routine and is freed by the calling
 **                 context.
 **
 ******************************************************************************/
Sdf_ty_retVal sdf_cbk_uaGetHostName
#ifdef ANSI_PROTO
    (Sdf_ty_s8bit **ppHost,
    Sdf_st_error *pError)
#else
    (pHost,pError)
    Sdf_ty_s8bit **ppHost;
    Sdf_st_error *pError;
#endif
{
    icf_int8_t    host_name_str[100];

    Sdf_ty_retVal   sdf_ret_val = Sdf_co_success;

#if defined ICF_PORT_LINUX || defined ICF_PORT_WINDOWS || defined(ICF_PORT_VXWORKS) 


    if(ICF_NULL != gethostname(host_name_str,100))
    {
#ifdef SDF_ERROR
        sdf_fn_setError(Sdf_en_minorErrors,Sdf_en_systemError,\
            (Sdf_ty_s8bit *)"sdf_cbk_uaGetHostName(): "
                "Failed to get hostname",pError);
#endif
        pError->errCode = Sdf_en_systemError;
        sdf_ret_val = Sdf_co_fail;
    }
    else
    {
        Sdf_mc_strdup((*ppHost),host_name_str);
    }
#endif /*PORT_LINUX*/
    return sdf_ret_val;
}



/*****************************************************************************
** FUNCTION: sdf_cbk_uaIsConnectedTcpSocket
**
** DESCRIPTION: The function should return whether the socket is connected to
**                the other end or not.
**
******************************************************************************/

Sdf_ty_retVal sdf_cbk_uaIsConnectedTcpSocket
#ifdef ANSI_PROTO
    (Sdf_ty_s32bit dSockfd,
    Sdf_st_error *pError)
#else     
    (dSockfd, pError)
    Sdf_ty_s32bit  dSockfd;
    Sdf_st_error *pError;
#endif
{
    icf_ssa_pdb_st           ssa_pdb;
    icf_return_t             ret_val  =ICF_SUCCESS;
    icf_error_t              ecode;
    Sdf_ty_retVal   sdf_ret_val = Sdf_co_success;

    (void)pError;
    if(ICF_NULL == p_persistent_glb_pdb)
    {
    return Sdf_co_fail;
    }
    
    ICF_FUNCTION_ENTER(p_persistent_glb_pdb)

#if defined ICF_PORT_LINUX || defined ICF_PORT_WINDOWS || defined(ICF_PORT_VXWORKS)

    ssa_pdb.p_ecode = &ecode;
    icf_ssa_init_pdb(&ssa_pdb,p_persistent_glb_pdb);


    /* get the ssa global data pointer from DBM */
    if (ICF_FAILURE == icf_dbm_get_module_glb_data(
                p_persistent_glb_pdb, ICF_INT_MODULE_SSA,
               (icf_void_t **)(&ssa_pdb.p_ssa_glb_data),&ecode))
    {
        ICF_PRINT(((icf_uint8_t *)"\n[SSA]:Failure in SSA Data retreival in IsConnected"));
        sdf_ret_val = Sdf_co_fail;
        ret_val = ICF_FAILURE;
    }
    else
    {
        if ( ICF_NULL == 
                icf_ssa_get_conn_db(dSockfd, ssa_pdb.p_ssa_glb_data->p_ssa_tcpm_conn_db ))
        {
            ICF_PRINT(((icf_uint8_t *)"\n[SSA]:ConnDbNode corresponding to FD: %d \
                        given by UATK not found in DB",dSockfd));
            sdf_ret_val = Sdf_co_fail;
        ret_val = ICF_FAILURE;
        }
        else
        {
             /*Check if IP address and port in Transaction->DestInfo is same
               as Conn_table entry or not, if not then return failure else
               return success*/
             
        }
    }
#endif
    ICF_FUNCTION_EXIT(p_persistent_glb_pdb)
    return sdf_ret_val;
}


/*****************************************************************************
 ** FUNCTION:      sdf_cbk_uaCloseSocket
 **
 ** DESCRIPTION: The function should close a network socket.
 **
 *****************************************************************************/
Sdf_ty_retVal sdf_cbk_uaCloseSocket
#ifdef ANSI_PROTO
    (Sdf_st_socket *pSocket)    
#else     
    (pSocket)
    Sdf_st_socket *pSocket;    
#endif
{
    icf_return_t     ret_val = ICF_SUCCESS;
    Sdf_ty_retVal   sdf_ret_val = Sdf_co_success;

    if(ICF_NULL == p_persistent_glb_pdb)
    {
        return Sdf_co_fail;
    }

    ICF_FUNCTION_ENTER(p_persistent_glb_pdb)

#if defined ICF_PORT_LINUX || defined ICF_PORT_WINDOWS || defined(ICF_PORT_VXWORKS)
    if (Sdf_en_protoTcp == pSocket->dProtocol
#ifdef ICF_SECURE_SUPPORT    
     || Sdf_en_protoTls == pSocket->dProtocol)
#else
    )
#endif
    {
        /*    call get_module_data with p_persistent_glb_pdb to get the SSA
              module data.  Then get the instance of 
              icf_ssa_tcpm_conn_data_st corresponding to the FD in 
              pSocket. ( we iterate through the
              array in SSA global data and keep matching the FDs)
              If FD is found we invoke the TCPM SM */

        icf_ssa_pdb_st           ssa_pdb;
        icf_error_t              ecode;
        icf_return_t             ret_val = ICF_SUCCESS;

        ssa_pdb.p_ecode = &ecode;
        icf_ssa_init_pdb(&ssa_pdb,p_persistent_glb_pdb);

        if (pSocket->dSockfd == Sdf_co_InvalidSocketFd)
        {
            ICF_PRINT(((icf_uint8_t *)"\n[SSA]:InvalidFdInCloseSocketCBK, returning"));
        }    

        /* get the ssa global data pointer from DBM */
        else if (ICF_FAILURE == icf_dbm_get_module_glb_data(
                    p_persistent_glb_pdb, ICF_INT_MODULE_SSA,
                    (icf_void_t **)(&ssa_pdb.p_ssa_glb_data),&ecode))
        {
            ICF_PRINT(((icf_uint8_t *)"\n[SSA]:Failure in SSA Data retreival in IsConnected"));
            sdf_ret_val = Sdf_co_fail;
            ret_val = ICF_FAILURE;
        }
        else if (ICF_FAILURE == icf_dbm_get_module_glb_data(
                    p_persistent_glb_pdb, ICF_INT_MODULE_CFG,
                    (icf_void_t **)(&ssa_pdb.p_glb_cfg_data),&ecode))
        {
            ICF_PRINT(((icf_uint8_t *)"\n[SSA]:Failure in SSA Data retreival in IsConnected"));
            sdf_ret_val = Sdf_co_fail;
			ret_val = ICF_FAILURE;
        }
        else
        {
            ssa_pdb.p_conn_table_entry = icf_ssa_get_conn_db(pSocket->dSockfd,
                   ssa_pdb.p_ssa_glb_data->p_ssa_tcpm_conn_db); 
            if ( ICF_NULL == 
                    ssa_pdb.p_conn_table_entry)
            {
                ICF_PRINT(((icf_uint8_t *)"\n[SSA]:ConnDbNode corresponding to FD: %d \
                            given by UATK not found in DB",pSocket->dSockfd));
                sdf_ret_val = Sdf_co_fail;
                ret_val = ICF_FAILURE;
            }
            else
            {
                icf_ssa_pdb_st   *p_ssa_pdb = ICF_NULL;

                p_ssa_pdb = &ssa_pdb;
                ICF_SSA_IVK_TCPSM(p_ssa_pdb, 
                        ICF_SSA_TCPM_CLOSE_CONN_REQ, ret_val)
                if (ICF_FAILURE == ret_val)
                {
                    ICF_PRINT(((icf_uint8_t *)"\n[SSA]:FailureInSMForClose"));
                    /*       sdf_ret_val = Sdf_co_fail;*/
                }
                else
                {
                    pSocket->dSockfd = Sdf_co_InvalidSocketFd;
                    pSocket->dProtocol = Sdf_en_protoNone;
                }
            }            
        }        
    }

#endif
    ICF_FUNCTION_EXIT(p_persistent_glb_pdb)
    return ret_val;
}

/*****************************************************************************
 ** FUNCTION:      sdf_cbk_uaSocketCreate
 **
 ** DESCRIPTION: The function should create a new network socket.
 **              This is invoked from following two SIP UATK routines
 **     sdf_ivk_uaInitSocket->This is an API which is not used in UATK.
 **     and as application we will not use it.
 **     sdf_fn_uaInitiateConnection->This routine is used by UATK for getting
 **     a socket to start a new TCP connection.
 **
 *****************************************************************************/
Sdf_ty_socketId sdf_cbk_uaSocketCreate
#ifdef ANSI_PROTO
    (Sdf_ty_s32bit dDomain,
    Sdf_ty_s32bit dType,
    Sdf_ty_s32bit dProtocol)
#else
    (dDomain,dType,dProtocol)
    Sdf_ty_s32bit dDomain;
    Sdf_ty_s32bit dType;
    Sdf_ty_s32bit dProtocol;
#endif
{
    icf_uint8_t      transport_type;
    icf_error_t      ecode;

    Sdf_ty_socketId sockfd;
#ifdef SDF_ERROR    
    Sdf_st_error dError;
#endif
    (void)dDomain;
    (void)dType;
    if ( Sdf_en_protoUdp == dProtocol)
    {
        transport_type = ICF_TRANSPORT_TYPE_UDP;
    }
    else
    {
        transport_type = ICF_TRANSPORT_TYPE_TCP;
    }
        
    if (ICF_FAILURE == icf_port_open_socket(
#ifdef ICF_IPV6_ENABLED
                dDomain,
#endif
                transport_type,
                (icf_uint32_t*)(&sockfd),
                &ecode))
    {
#ifdef SDF_ERROR
        sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_socketOpenError,\
                (Sdf_ty_s8bit *)"sdf_cbk_uaSocketCreate( ): "
                "Could not open UDP socket for initiating call",&dError) ;
#endif
        return Sdf_co_fail;
    }
    return sockfd;
}

/*****************************************************************************
 ** FUNCTION:      sdf_cbk_uaConnectSocket
 **
 ** DESCRIPTION: The function should open a new network connection.
 **
 *****************************************************************************/
Sdf_ty_retVal sdf_cbk_uaConnectSocket
#ifdef ANSI_PROTO
    (Sdf_st_socket *pSocket,
    Sdf_ty_pvoid  pName,
    Sdf_ty_s32bit dNameLength,
    Sdf_ty_pvoid  pExtraArgs)
#else
    (pSocket, pName, dNameLength, pExtraArgs)
    Sdf_st_socket *pSocket;
    Sdf_ty_pvoid  pName;
    Sdf_ty_s32bit dNameLength;
    Sdf_ty_pvoid  pExtraArgs;
#endif
{
    Sdf_ty_retVal               sdf_ret_val = Sdf_co_success;
    icf_ssa_pdb_st           ssa_pdb;
    icf_error_t              ecode;
    icf_return_t             ret_val = ICF_SUCCESS;


    icf_ssa_pdb_st           *p_ssa_pdb  = ICF_NULL;
    /* Fix for CSR 1-5028975 */
#ifdef ICF_PORT_VXWORKS
    icf_uint8_t              host_addr_fmt[INET_ADDR_LEN]="\0";
#endif
    (void)dNameLength;
    (void)pExtraArgs;
   
    if (ICF_NULL == p_persistent_glb_pdb)
    {
       return ICF_FAILURE;
    }
 
    icf_ssa_init_pdb(&ssa_pdb, p_persistent_glb_pdb);
    ecode = ICF_ERROR_NONE;

    ssa_pdb.p_ecode = &ecode;
    ssa_pdb.p_glb_pdb           = p_persistent_glb_pdb;
    ssa_pdb.p_internal_msg      = ICF_NULL;
    ssa_pdb.p_line_data         = ICF_NULL;
    ssa_pdb.p_glb_cfg_data      = ICF_NULL;
    ssa_pdb.common_bitmask      = ICF_NULL;
    ssa_pdb.p_conn_table_entry   = ICF_NULL;
    ssa_pdb.dest_module_id  = ICF_INT_MODULE_INVALID;

    /* get the ssa global data pointer from DBM */
    if (ICF_FAILURE == icf_dbm_get_module_glb_data(
                p_persistent_glb_pdb, ICF_INT_MODULE_SSA,
               (icf_void_t **)(&ssa_pdb.p_ssa_glb_data),&ecode))
    {
        ICF_PRINT(((icf_uint8_t *)"\n[SSA]:Failure in SSA Data retreival in ConnectSocket"));
        sdf_ret_val = Sdf_co_fail;
    }
    else if (ICF_FAILURE == icf_dbm_get_module_glb_data(
                p_persistent_glb_pdb,
                (icf_int_module_id_t)ICF_INT_MODULE_CFG,
                (icf_void_t **)&(ssa_pdb.p_glb_cfg_data),
                ssa_pdb.p_ecode))
    {
        ICF_PRINT(((icf_uint8_t *)"\n[SSA]:Failure in CFG Data retreival in ConnectSocket"));
        sdf_ret_val = Sdf_co_fail;
    }
    else
    {
        icf_port_strcpy((icf_uint8_t *)ssa_pdb.dest_ip,
                        (icf_uint8_t *)(inet_ntoa(((struct sockaddr_in*)pName)->sin_addr)));
                
        ssa_pdb.dest_port
                  = ntohs(((struct sockaddr_in*)pName)->sin_port);
#ifdef ICF_SECURE_SUPPORT	
        if (Sdf_en_protoTls == pSocket->dProtocol)
        {
            ssa_pdb.conn_type = ICF_CONN_TYPE_BOTH;
            ssa_pdb.protocol = ICF_TRANSPORT_TLS;
        }
        else if (Sdf_en_protoTcp == pSocket->dProtocol)
#endif
        {
            ssa_pdb.conn_type = ICF_CONN_TYPE_REQUEST;
            ssa_pdb.protocol = ICF_TRANSPORT_TCP;
        }
        ssa_pdb.p_conn_table_entry = icf_ssa_get_conn_db_by_ip(&ssa_pdb,
               ssa_pdb.p_ssa_glb_data->p_ssa_tcpm_conn_db); 
 
        if ((ICF_NULL != ssa_pdb.p_conn_table_entry)&&(0 != ICF_PURGE_TIMER_DURATION))
        {
            pSocket->dSockfd = ssa_pdb.p_conn_table_entry->conn_fd;
            ICF_PRINT(((icf_uint8_t *) "\n[SSA]:ConnDbNodeAllocationFailure"));
            sdf_ret_val = Sdf_co_success;
        }
        else
        {
           ssa_pdb.p_conn_table_entry = 
               icf_ssa_add_conn_entry(ssa_pdb.p_ssa_glb_data->p_ssa_tcpm_conn_db);
	   if( ICF_NULL == ssa_pdb.p_conn_table_entry )
           {
                sdf_ret_val = ICF_FAILURE;
           }
           else
           {
	
		ssa_pdb.p_conn_table_entry->conn_fd = (icf_int16_t)pSocket->dSockfd;

            /* Fix for CSR 1-5028975 */
#ifdef ICF_PORT_VXWORKS
            inet_ntoa_b(((struct sockaddr_in *)pName)->sin_addr,host_addr_fmt);
            icf_port_strcpy(
                            ssa_pdb.p_conn_table_entry->peer_ip, 
                            host_addr_fmt);

#else
            /*get the destination IP address and port from
             * destination address*/
            icf_port_strcpy(ssa_pdb.p_conn_table_entry->peer_ip , 
                    (icf_uint8_t *)(inet_ntoa(((struct sockaddr_in*)pName)->sin_addr)));
#endif

		ssa_pdb.p_conn_table_entry->peer_port_num
			= ntohs(((struct sockaddr_in*)pName)->sin_port);

            
        ssa_pdb.p_conn_table_entry->is_persistent = ICF_TRUE;
        ICF_SSA_INC_CONN_REF_COUNT(ssa_pdb.p_conn_table_entry)
		p_ssa_pdb = &ssa_pdb;
		/* Also set the transport type in the SSA PDB */
#ifdef ICF_SECURE_SUPPORT	
		if (Sdf_en_protoTls == pSocket->dProtocol)
		{
			p_ssa_pdb->common_bitmask |= ICF_SSA_SECURE_CONN;
            p_ssa_pdb->p_conn_table_entry->prot_conn_bitmask |= ICF_TRANSPORT_TLS;
		}
        else
#endif	
        {
            p_ssa_pdb->p_conn_table_entry->prot_conn_bitmask |= ICF_TRANSPORT_TCP;
            p_ssa_pdb->p_conn_table_entry->prot_conn_bitmask |= ICF_CONN_TYPE_REQUEST;
        }
		ICF_SSA_IVK_TCPSM(p_ssa_pdb, 
				ICF_SSA_TCPM_OPEN_CONN_REQ, ret_val)
		if (ICF_FAILURE == ret_val)
		{
			ICF_PRINT(((icf_uint8_t *)"\n[SSA]:FailureReturnedBySMForConnect"));
			sdf_ret_val = Sdf_co_fail;
		}
	    }
	}/*end of success leg for get conn db node*/
    }/*End of success leg for get cfg data*/	
    return sdf_ret_val;
}


/*****************************************************************************
 ** FUNCTION:      sdf_cbk_uaBindSocket
 **
 ** DESCRIPTION: The function should bind a socket to a specified port.
 **
 *****************************************************************************/
Sdf_ty_retVal sdf_cbk_uaBindSocket
#ifdef ANSI_PROTO
    (Sdf_ty_socketId dSocket,
    Sdf_ty_pvoid  pName,
    Sdf_ty_s32bit dNameLength)
#else
    (dSocket,pName,dNameLength)
    Sdf_ty_socketId dSocket;
    Sdf_ty_pvoid  pName;
    Sdf_ty_s32bit dNameLength;
#endif
{
#ifdef SDF_ERROR
    Sdf_st_error dErr;
#endif

    if ((bind(dSocket,(struct sockaddr *)pName,dNameLength)) <0)
    {
#ifdef SDF_ERROR
        sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_socketBindError, \
            (Sdf_ty_s8bit*)"SDF_ERROR - sdf_cbk_uaBindSocket(): "
            "Could not bind to address", &dErr);
#endif
        return Sdf_co_fail;
    }
    return Sdf_co_success;
}


/*****************************************************************************
 ** FUNCTION:      sdf_cbk_uaListenSocket
 **
 ** DESCRIPTION: The function should set the socket in listen mode for TCP 
 **                 connections.
 **
 *****************************************************************************/
Sdf_ty_retVal sdf_cbk_uaListenSocket
#ifdef ANSI_PROTO
    (Sdf_ty_socketId dSocket,
    Sdf_ty_s32bit dMaxPendingConnections)
#else
    (dSocket, dMaxPendingConnections)
    Sdf_ty_socketId dSocket;
    Sdf_ty_s32bit dMaxPendingConnections;
#endif
{
    if (listen(dSocket, dMaxPendingConnections) < 0)
        return Sdf_co_fail;
    else
        return Sdf_co_success;
}


/*****************************************************************************
 ** FUNCTION:      sdf_cbk_uaSetSockOpt
 **
 ** DESCRIPTION: The function should set socket options.
 **
 *****************************************************************************/
Sdf_ty_retVal sdf_cbk_uaSetSockOpt
#ifdef ANSI_PROTO
    (Sdf_ty_socketId dSocket,
    Sdf_ty_s32bit  dLevel,
    Sdf_ty_s32bit  dOptName,
    Sdf_ty_pvoid   pOptVal,
    Sdf_ty_s32bit dOptLen)
#else
    (dSocket,dLevel,dOptName,dOptVal,dOptLen)
    Sdf_ty_socketId dSocket;
    Sdf_ty_s32bit  dLevel;
    Sdf_ty_s32bit  dOptName;
    Sdf_ty_pvoid   pOptVal;
    Sdf_ty_s32bit dOptLen;
#endif
{
#ifdef SDF_ERROR
    Sdf_st_error dErr;
#endif

    if (setsockopt(dSocket,dLevel,\
        dOptName, (Sdf_ty_s8bit *)pOptVal,dOptLen) < 0)
        {
#ifdef SDF_ERROR
            sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_socketOpenError, \
                (Sdf_ty_s8bit*)"SDF_ERROR - sdf_cbk_uaSetSockOpt"
                "(): Failed to set socket options", &dErr);
#endif
            return Sdf_co_fail;
        }    
        return Sdf_co_success;
}
#endif /* endif for SDF_NETWORKCALLBACKS */    

/*****************************************************************************
 ** FUNCTION:      icf_ssa_tcpm_00_inc_conn_ind
 **
 ** DESCRIPTION: Move to State Connected.Store the remote IP/Port. 
 *****************************************************************************/
icf_return_t icf_ssa_tcpm_00_inc_conn_ind(
        icf_ssa_pdb_st   *p_ssa_pdb)
{
    icf_return_t     ret_val = ICF_SUCCESS;

    icf_ssa_tcpm_conn_data_st  *p_conn_db_node = 
        p_ssa_pdb->p_conn_table_entry;

    ICF_FUNCTION_ENTER(p_ssa_pdb->p_glb_pdb)
    ICF_ASSERT(p_ssa_pdb->p_glb_pdb, p_conn_db_node)

    p_conn_db_node->tcpm_sm_state = ICF_SSA_TCPM_INUSE;
    p_ssa_pdb->p_conn_table_entry->bitmap |= ICF_TCP_DATA_SERVER_PORT; 
    ICF_FUNCTION_EXIT(p_ssa_pdb->p_glb_pdb)
    return ret_val;
}


/*****************************************************************************
 ** FUNCTION:      icf_ssa_tcpm_04_write_to_conn
 **
 ** DESCRIPTION:  Store the SSA ctxt in the connection DB node.
 **                 Write the message on connection. SM has been invoked from
 **                 callback SendMsgOnNetwork
 *****************************************************************************/
icf_return_t icf_ssa_tcpm_04_write_to_conn(
        icf_ssa_pdb_st   *p_ssa_pdb)
{
    icf_return_t     ret_val = ICF_SUCCESS;
    icf_ssa_tcpm_conn_data_st  *p_conn_db_node = 
        p_ssa_pdb->p_conn_table_entry;

    ICF_FUNCTION_ENTER(p_ssa_pdb->p_glb_pdb)
    ICF_ASSERT(p_ssa_pdb->p_glb_pdb, p_conn_db_node)

    p_conn_db_node->tcpm_sm_state = ICF_SSA_TCPM_INUSE;
    p_conn_db_node->timer_count = 0;
    ICF_FUNCTION_EXIT(p_ssa_pdb->p_glb_pdb)
    return ret_val;
}
/*****************************************************************************
 ** FUNCTION:      icf_ssa_tcpm_03_write_to_conn
 **
 ** DESCRIPTION:  Store the SSA ctxt in the connection DB node.
 **                 Write the message on connection. SM has been invoked from
 **                 callback SendMsgOnNetwork
 *****************************************************************************/
icf_return_t icf_ssa_tcpm_03_write_to_conn(
        icf_ssa_pdb_st   *p_ssa_pdb)
{
    icf_return_t     ret_val = ICF_SUCCESS;
    icf_ssa_tcpm_conn_data_st  *p_conn_db_node = 
        p_ssa_pdb->p_conn_table_entry;

    ICF_FUNCTION_ENTER(p_ssa_pdb->p_glb_pdb)
    ICF_ASSERT(p_ssa_pdb->p_glb_pdb, p_conn_db_node)

    p_conn_db_node->tcpm_sm_state = ICF_SSA_TCPM_INUSE;
    p_conn_db_node->timer_count = 0;
    ICF_FUNCTION_EXIT(p_ssa_pdb->p_glb_pdb)
    return ret_val;
}


/*****************************************************************************
 ** FUNCTION:      icf_ssa_tcpm_02_write_to_conn
 **
 ** DESCRIPTION:  Write the message on connection. SM has been invoked from
 **                 callback SendMsgOnNetwork
 *****************************************************************************/
icf_return_t icf_ssa_tcpm_02_write_to_conn(
        icf_ssa_pdb_st   *p_ssa_pdb)
{
    icf_return_t     ret_val = ICF_SUCCESS;
    icf_ssa_tcpm_conn_data_st  *p_conn_db_node = 
        p_ssa_pdb->p_conn_table_entry;

    ICF_FUNCTION_ENTER(p_ssa_pdb->p_glb_pdb)
    ICF_ASSERT(p_ssa_pdb->p_glb_pdb, p_conn_db_node)
    p_conn_db_node->tcpm_sm_state = ICF_SSA_TCPM_INUSE;
    p_conn_db_node->timer_count = 0;
    ICF_FUNCTION_EXIT(p_ssa_pdb->p_glb_pdb)
    return ret_val;
}


/*****************************************************************************
 ** FUNCTION:      icf_ssa_tcpm_01_succ_open_conn_resp
 **
 ** DESCRIPTION:  Check if there are any messages in the list.
 **             If so, dispatch all and release the memory.Change state to 
 **             Connected
 *****************************************************************************/
icf_return_t icf_ssa_tcpm_01_succ_open_conn_resp(
        icf_ssa_pdb_st   *p_ssa_pdb)
{
    icf_return_t     ret_val = ICF_SUCCESS;
    icf_uint8_t      pdu_ctr = 0;
    icf_ssa_ctxt_st  *p_ssa_ctxt = ICF_NULL;
    SipTranspAddr   sip_trans_addr;
    Sdf_ty_protocol	protocol = Sdf_en_protoTcp;
    icf_uint32_t pdu_len = 0;
    icf_ssa_ctxt_list_st    *p_ssa_list = ICF_NULL;
    icf_list_st          *p_root = ICF_NULL;
    icf_list_st          *p_temp = ICF_NULL;
    icf_boolean_t    non_persistent_break=ICF_FALSE;

    icf_ssa_tcpm_conn_data_st  *p_conn_db_node = 
        p_ssa_pdb->p_conn_table_entry;
    sip_trans_addr.pHost=Sdf_co_null;
    sip_trans_addr.pReceiversIp=Sdf_co_null;
    sip_trans_addr.dPort=0;

    ICF_FUNCTION_ENTER(p_ssa_pdb->p_glb_pdb)
    /*GUIDELINE: There can be two kinds of pending messages.
     * One which mark the start of dialog (INVITE, REG)and i
     * others which are part of a dialog ( 180, 183, 200, ACK,
     * UPDATE, BYE, REINVITE etc.)
     * Our guideline is in case of dual transport mode, the mode
     * will be decided at the start of the dialogue only. Thus, 
     * currently we attempt a new connection first in case of 
     * INVITE and REG and do not proceed further. Once connect
     * is through ( either to connected or to PENDING), we 
     * invoke SendCallToPeer from the SM and then EndTransaction.
     * If we get a failure response for connect we check 
     * if the msg was INV or REG, if so we change TXN mode to
     * UDP and invoke SendCallToPeer. 
     * In case of the second category of messages, we fail
     * the call if the connect response is a failure.
     * In future, we may go for support of OPTIONS outside
     * dialog, that will fall in the first category so 
     * should be coded accordingly in apihandlers.c
     */
        /* 7.0 */
    sip_trans_addr.dSockFd = p_conn_db_node->conn_fd;

    p_conn_db_node->tcpm_sm_state = ICF_SSA_TCPM_CONNECTED;
    if(ICF_SSA_SECURE_CONN == (p_ssa_pdb->common_bitmask & ICF_SSA_SECURE_CONN))
    {
        ICF_SSA_APP_TRACE(p_ssa_pdb,ICF_TRACE_TLS_CONNECTION_OPEN)
    }
    else
    { 
       ICF_SSA_APP_TRACE(p_ssa_pdb,ICF_TRACE_TCP_CONNECTION_OPEN)
    }

    p_root =  icf_dbm_fetch_all_ssa_ctxt_from_hash_table(p_ssa_pdb->p_glb_pdb);
    for (p_temp = p_root; ICF_NULL != p_temp; p_temp = p_temp->p_next)
    {
        for (p_ssa_list = (icf_ssa_ctxt_list_st *)p_temp->p_data; ICF_NULL != p_ssa_list ;
                p_ssa_list = p_ssa_list->p_next)
        {
            p_ssa_ctxt = p_ssa_list->p_ssa_ctxt;
            p_ssa_pdb->p_ssa_ctxt = p_ssa_ctxt;
            if ((p_ssa_ctxt->p_conn_table_req_entry == p_conn_db_node) ||
                    (p_ssa_ctxt->p_conn_table_resp_entry == p_conn_db_node))
            {
                if ( ICF_NULL != 
                        ((ICF_SSA_REG_PENDING | ICF_SSA_INVITE_PENDING | 
                          ICF_SSA_SUBS_PENDING | ICF_SSA_REFER_PENDING |
                          ICF_SSA_OPTION_PENDING | ICF_SSA_PUBLISH_PENDING |
                          ICF_SSA_MESSAGE_PENDING )
                         & (p_ssa_ctxt->bitmask)))
                {
                    /*For the first type, there can be only one pending PDU here*/
                    /*A StartTransaction would have been done here for which we should
                     * do an End when we have successfully sent it to peer*/
                    if (ICF_FAILURE == 
                            icf_ssa_send_dialog_initiator_msg(p_ssa_pdb))
                    {
                        /*           ICF_SSA_DEBUG(("[SSA]:FailureInSendingMsgOnTcpConn, \
                                     Trigering CallRelease"));
                         */         ret_val = ICF_FAILURE;
                        /*Some failure, be it failure in getting line data, 
                         * in sending message to peer or transport mode not
                         * supporting a  revert to UDP, we will send failure
                         * indication to converned module*/
                        icf_ssa_send_failure_to_app_module(p_ssa_pdb);
                    }
                }

                for (pdu_ctr = 0; pdu_ctr < p_ssa_ctxt->no_of_pending_pdu; pdu_ctr++)
                {
                    pdu_len = 
                        icf_port_strlen(p_ssa_ctxt->p_pending_pdu[pdu_ctr]);

                    icf_ssa_dispatch_pdu_to_es(p_ssa_pdb->p_glb_pdb,
                            p_ssa_ctxt,
                            &sip_trans_addr,protocol,
                            p_ssa_ctxt->p_pending_pdu[pdu_ctr],
                            pdu_len, p_ssa_ctxt->line_id, p_ssa_pdb->p_ecode);

                    p_ssa_ctxt->p_pending_pdu[pdu_ctr] = ICF_NULL;
                }
                p_ssa_ctxt->no_of_pending_pdu = 0;
                if(0 == ICF_PURGE_TIMER_DURATION)
                {
                    non_persistent_break = ICF_TRUE;
                    break;
                }
            }
        }
        if(ICF_TRUE == non_persistent_break)
        {
            break;
        }
    }


    icf_ssa_free_list(p_ssa_pdb,p_root);
    ICF_SSA_PRINT_CONN_DB(p_conn_db_node)
    /*Stat collection for TCP connection*/ 
    if( ret_val != ICF_FAILURE)
    {
        ICF_STATS(p_ssa_pdb->p_glb_pdb->p_stats_info->
                stat_tcp_connections.stats_active_conn)

        ICF_STATS(p_ssa_pdb->p_glb_pdb->p_stats_info->
                   stat_tcp_connections.total_tcp_conn_since_last_reboot)
    }
    ICF_FUNCTION_EXIT(p_ssa_pdb->p_glb_pdb)
        return ret_val;
}


/*****************************************************************************
 ** FUNCTION:      icf_ssa_tcpm_01_fail_open_conn_resp
 **
 ** DESCRIPTION:  Check if there are any messages in the list.
 **             If so, delete all, Trigger generic failure ti release the call 
 *****************************************************************************/
icf_return_t icf_ssa_tcpm_01_fail_open_conn_resp(
        icf_ssa_pdb_st   *p_ssa_pdb)
{
    icf_return_t     ret_val = ICF_SUCCESS;
    icf_ssa_tcpm_conn_data_st  *p_conn_db_node = 
        p_ssa_pdb->p_conn_table_entry;
    icf_transport_mode_et     mode = ICF_TRANSPORT_TYPE_UDP;
    Sdf_ty_s8bit                method[ICF_MAX_SIZE_INT_USAGE_STR] = {'\0'};
    icf_ssa_ctxt_st  *p_ssa_ctxt = ICF_NULL;
    icf_ssa_ctxt_list_st    *p_ssa_list = ICF_NULL;
    icf_list_st          *p_root = ICF_NULL;
    icf_list_st          *p_temp = ICF_NULL;
    icf_boolean_t    non_persistent_break=ICF_FALSE;

    ICF_FUNCTION_ENTER(p_ssa_pdb->p_glb_pdb)

    /* Taking the first ssa context for DNS handling. It suffices */
    p_root =  icf_dbm_fetch_all_ssa_ctxt_from_hash_table(p_ssa_pdb->p_glb_pdb);

    if (ICF_NULL == p_root)
    {
        return ICF_FAILURE;
    } 

    p_ssa_pdb->p_ssa_ctxt = ((icf_ssa_ctxt_list_st*)(p_root->p_data))->p_ssa_ctxt;
   if (ICF_SSA_SECURE_CONN == (p_ssa_pdb->common_bitmask & ICF_SSA_SECURE_CONN))
   {
        ICF_SSA_APP_TRACE(p_ssa_pdb,ICF_TRACE_TLS_CONNECTION_OPEN)
   }
   else
   {
     ICF_SSA_APP_TRACE(p_ssa_pdb,ICF_TRACE_TCP_CONNECTION_OPEN)
   } 

    /* Collect stats of connection failure of TCP*/
    ICF_STATS(p_ssa_pdb->p_glb_pdb->p_stats_info->
            stat_tcp_connections.stats_conn_failures)


#ifdef ICF_DNS_LOOKUP_ENABLED
/* this cade is added here to make the retries on the different IPs in case of 
 * TCP, we will check if the Transaction related informationis stored here and
 * then will fetch the next IP and retry on the same */
    if(ICF_NULL != p_ssa_pdb->p_ssa_ctxt && 
       p_ssa_pdb->p_ssa_ctxt->bitmask & ICF_SSA_DNS_XSACTION_PRESENT &&
       !(p_ssa_pdb->p_ssa_ctxt->p_transaction == ICF_NULL &&
        p_ssa_pdb->p_ssa_ctxt->p_overlap_transaction == ICF_NULL))
    {
        icf_boolean_t                ready_to_send = ICF_TRUE;
        icf_ssa_ctxt_st             *p_ssa_ctxt = p_ssa_pdb->p_ssa_ctxt;

        ICF_SSA_SET_TXN_DNS_STATE(((Sdf_st_transaction*)p_ssa_ctxt->p_transaction), 
                                  ((Sdf_st_overlapTransInfo*)p_ssa_ctxt->p_overlap_transaction), Sdf_en_dnsIpUsed)

        /*Connect has failed to the current IP, so we attempt to move to the next IP*/
        icf_ssa_remove_conn_entry(p_ssa_pdb ,
                p_conn_db_node->conn_fd);
        p_ssa_pdb->p_conn_table_entry = ICF_NULL;    
		if ( ICF_FAILURE !=  icf_ssa_get_next_dest_from_list(
					        p_ssa_pdb,p_ssa_ctxt->p_call_obj,
                            p_ssa_ctxt->p_transaction, p_ssa_ctxt->p_overlap_transaction))
        {
           
            if (ICF_FAILURE == icf_ssa_attempt_out_of_uatk_connect(
                                    p_ssa_pdb, p_ssa_ctxt->p_call_obj, 
                                    p_ssa_ctxt->p_transaction, 
                                    p_ssa_ctxt->p_overlap_transaction,
                                    &ready_to_send))
                        {
                            p_ssa_pdb->p_ssa_ctxt->bitmask &= ~ICF_SSA_DNS_XSACTION_PRESENT;
                            ICF_PRINT(((icf_uint8_t *)"\n[SSA]:FailureFrom AttemptOutOfDlgConnect"));
                        }
            return ICF_SUCCESS;
        }


    }
#endif
    for (p_temp = p_root; ICF_NULL != p_temp; p_temp = p_temp->p_next)
    {
        for (p_ssa_list = (icf_ssa_ctxt_list_st *)p_temp->p_data; ICF_NULL != p_ssa_list ;)
                /*p_ssa_list = p_ssa_list->p_next)*/
        {
            p_ssa_ctxt = p_ssa_list->p_ssa_ctxt;
            p_ssa_pdb->p_ssa_ctxt = p_ssa_ctxt;
            ret_val = ICF_SUCCESS;
            if (ICF_NULL != p_ssa_ctxt)
            {
                if ((p_ssa_ctxt->p_conn_table_req_entry != p_conn_db_node) &&
                        (p_ssa_ctxt->p_conn_table_resp_entry != p_conn_db_node))
                {
                    p_ssa_list = p_ssa_list->p_next;
                    continue;
                }
                ICF_PRINT(((icf_uint8_t *)"SSA context connection type got matched\n"));

                if (ICF_SSA_INVITE_PENDING == 
                        (ICF_SSA_INVITE_PENDING & p_ssa_ctxt->bitmask))
                {
                    /*Get line data, we need it to get the transport mode*/
                    if ( ICF_FAILURE == 
                            icf_dbm_fetch_line_cntxt_blk(
                                p_ssa_pdb->p_glb_pdb, 
                                p_ssa_ctxt->p_call_ctxt->line_id,
                                &(p_ssa_pdb->p_line_data), p_ssa_pdb->p_ecode))
                    {
                        /*                ICF_SSA_DEBUG(("[SSA]:Failure In FetchLineCtxt\n"));*/
                        ret_val = ICF_FAILURE;
                    }
                    else
                    {
                        mode = p_ssa_pdb->p_glb_cfg_data->self_mode;
                    }
                }
                else if(ICF_SSA_REG_PENDING == 
                        (ICF_SSA_REG_PENDING & p_ssa_ctxt->bitmask))
                {
                    /*Get line data, we need it to get the transport mode*/
                    if ( ICF_FAILURE == 
                            icf_dbm_fetch_line_cntxt_blk(
                                p_ssa_pdb->p_glb_pdb, 
                                p_ssa_ctxt->p_rgm_ctxt->rgm_list_of_lines[0],
                                &(p_ssa_pdb->p_line_data), p_ssa_pdb->p_ecode))
                    {
                        /*                ICF_SSA_DEBUG(("[SSA]:Failure In FetchLineCtxt\n"));*/
                        ret_val = ICF_FAILURE;
                    }
                    else
                    {
                        mode = p_ssa_pdb->p_glb_cfg_data->self_mode;
                    }
                }
                else if ( ICF_NULL != 
                        ((ICF_SSA_SUBS_PENDING | ICF_SSA_REFER_PENDING |
                          ICF_SSA_OPTION_PENDING | ICF_SSA_PUBLISH_PENDING |
                          ICF_SSA_MESSAGE_PENDING )
                         & (p_ssa_ctxt->bitmask)))
                {
                    mode = p_ssa_pdb->p_glb_cfg_data->self_mode;

                }
                else
                {
                    ret_val = ICF_FAILURE;
                }
            } /* if (ICF_NULL != p_conn_db_n... */
            else
            {
                /* Hitting this means that the connect req was sent for 
                 * a non-dialog initiator message. As of now the dialog
                 * can be intitiated by REGISTER and INVITE only.*/
                ret_val = ICF_FAILURE;
            }


            if ((ICF_TRANSPORT_MODE_BOTH == mode) && 
                    (ICF_SUCCESS == ret_val))
            {
                p_ssa_pdb->p_ssa_ctxt->revert_mode = ICF_TRUE;
            }
            else if (ICF_SUCCESS == ret_val)
            {
                p_ssa_pdb->p_ssa_ctxt->revert_mode = ICF_FALSE;
            }


            if ( ICF_FAILURE != ret_val)
            {
                if ( ICF_TRUE == p_ssa_pdb->p_ssa_ctxt->revert_mode)
                {
                    ICF_SSA_RESET_COMMON_BITMASK(\
                            p_ssa_ctxt->bitmask,\
                            ICF_SSA_TCP_TRANSPORT)
                        if (ICF_FAILURE == 
                                icf_ssa_send_dialog_initiator_msg(p_ssa_pdb))
                        {
                            /*                ICF_SSA_DEBUG(("\n[SSA]:FailureInSendingMsgOnTcpConn, \
                                              Trigering Release"));
                             */              ret_val = ICF_FAILURE;
                        }
                }
                else
                {
                    /*            ICF_SSA_DEBUG(("[SSA]:FailureResponseForOpenConn in TcpOnly, triggering Release\n"));*/
                    ret_val = ICF_FAILURE;
                }
            }

#ifdef ICF_ERROR_ENABLE
            if (( ICF_FAILURE != ret_val)&& (ICF_NULL != p_ssa_ctxt))
            {

                if (ICF_SSA_INVITE_PENDING == (ICF_SSA_INVITE_PENDING &
                            p_ssa_ctxt->bitmask))
                {
                    icf_port_strcpy((icf_uint8_t *)method,(icf_uint8_t *)"INVITE");
                }
                else
                {
                    icf_ssa_get_method_name(p_ssa_ctxt->bitmask, (icf_uint8_t *)method);
                } 
                ICF_SSA_SET_ECODE(p_ssa_pdb,method,ICF_ERROR_OPEN_SOCKET)
            }
#endif

            if (ICF_NULL != p_ssa_ctxt)
            {
                if (p_ssa_ctxt->p_conn_table_req_entry == p_ssa_pdb->p_conn_table_entry)
                {
                    p_ssa_ctxt->p_conn_table_req_entry  = ICF_NULL;
                }
                if (p_ssa_ctxt->p_conn_table_resp_entry == p_ssa_pdb->p_conn_table_entry)
                {
                    p_ssa_ctxt->p_conn_table_resp_entry  = ICF_NULL;
                }
            }
            if ( ICF_FAILURE == ret_val)
            {
               p_ssa_list = p_ssa_list->p_next;
                /*Some failure, be it failure in getting line data, 
                 * in sending message to peer or transport mode not
                 * supporting a  revert to UDP, we will send failure
                 * indication to converned module*/
                icf_ssa_send_failure_to_app_module(p_ssa_pdb);
            }
 
            if(0 == ICF_PURGE_TIMER_DURATION)
            {
                non_persistent_break = ICF_TRUE;
                break;
            }
        }
        if(ICF_TRUE == non_persistent_break)
        {
            break;
        }
    }
   icf_ssa_free_list (p_ssa_pdb,p_root); 
   icf_ssa_remove_conn_entry(p_ssa_pdb ,
                p_conn_db_node->conn_fd);
	ICF_FUNCTION_EXIT(p_ssa_pdb->p_glb_pdb)
    return ret_val;
}


/*****************************************************************************
 ** FUNCTION:      icf_ssa_tcpm_01_write_to_conn
 **
 ** DESCRIPTION:  Add the message to queue. SM has been invoked from
 **                 callback SendMsgOnNetwork
 *****************************************************************************/
icf_return_t icf_ssa_tcpm_01_write_to_conn(
        icf_ssa_pdb_st   *p_ssa_pdb)
{
    icf_return_t     ret_val = ICF_SUCCESS;
    icf_uint8_t      *p_pdu;

    ICF_FUNCTION_ENTER(p_ssa_pdb->p_glb_pdb)

    ICF_MEMGET(p_ssa_pdb->p_glb_pdb, p_ssa_pdb->pdu_len + 1, ICF_MEM_COMMON,\
            p_pdu,ICF_RET_ON_FAILURE,\
            p_ssa_pdb->p_ecode, ret_val)
    icf_port_memcpy(p_pdu, p_ssa_pdb->p_stream_pdu, p_ssa_pdb->pdu_len);
    p_pdu[p_ssa_pdb->pdu_len] = '\0';
    if ( ICF_FAILURE == 
            icf_ssa_add_to_pdu_array(p_ssa_pdb, p_pdu))
    {
        ICF_PRINT(((icf_uint8_t *)"\n[SSA]:FailureInWriteToConn in State 01"));
        ret_val = ICF_FAILURE;
    }
    ICF_FUNCTION_EXIT(p_ssa_pdb->p_glb_pdb)
    return ret_val;
}


/*****************************************************************************
 ** FUNCTION:      icf_ssa_tcpm_01_close_ind
 **
 ** DESCRIPTION: Close connection from peer, we trigger UATK to 
 **  reset the fds in all Socket structures fo the call object
 *****************************************************************************/

icf_return_t icf_ssa_tcpm_01_close_ind(
        icf_ssa_pdb_st   *p_ssa_pdb)
{
    icf_return_t     ret_val = ICF_SUCCESS;
    icf_ssa_tcpm_conn_data_st  *p_conn_db_node = 
       p_ssa_pdb->p_conn_table_entry;
    icf_nw_interface_info_st nw_interface_info;
    icf_ssa_ctxt_st  *p_ssa_ctxt = ICF_NULL;
    icf_ssa_ctxt_list_st    *p_ssa_list = ICF_NULL;
    icf_list_st          *p_root = ICF_NULL;
    icf_list_st          *p_temp = ICF_NULL;
    icf_boolean_t        non_persistent_break=ICF_FALSE;

    ICF_FUNCTION_ENTER(p_ssa_pdb->p_glb_pdb)

    if(ICF_SSA_SECURE_CONN == (p_ssa_pdb->common_bitmask & ICF_SSA_SECURE_CONN))
    {
        ICF_SSA_APP_TRACE(p_ssa_pdb,ICF_TRACE_TLS_CONNECTION_CLOSE)
    }
   
   else
   {
       ICF_SSA_APP_TRACE(p_ssa_pdb, ICF_TRACE_TCP_CONNECTION_CLOSE)
   }
    /*bug fix*/
    nw_interface_info.bitmap = ICF_NULL;

    if (ICF_FAILURE == icf_dbm_get_module_glb_data(
        p_ssa_pdb->p_glb_pdb,ICF_INT_MODULE_SSA,
        (icf_void_t **)&(p_ssa_pdb->p_ssa_glb_data) ,
        p_ssa_pdb->p_ecode))
    {
        return ICF_FAILURE;
    }
    /* get the config data pointer from DBM */
    if (ICF_FAILURE == icf_dbm_get_module_glb_data(
        p_ssa_pdb->p_glb_pdb,
        (icf_int_module_id_t)ICF_INT_MODULE_CFG,
        (icf_void_t **)&(p_ssa_pdb->p_glb_cfg_data),
        p_ssa_pdb->p_ecode))
    {
        return ICF_FAILURE;
    }

    if (ICF_FAILURE != ret_val)
    {

        nw_interface_info.bitmap = ICF_INTF_INFO_PROTOCOL_TCP;
        nw_interface_info.receiver_fd = 
            p_ssa_pdb->p_conn_table_entry->conn_fd;

        /*Should be replaced by port close fd*/
        icf_es_close_tcp_conn(
                p_ssa_pdb->p_glb_pdb->p_glb_data->p_glb_port_info, 
                &nw_interface_info, p_ssa_pdb->p_ecode);
        p_root =  icf_dbm_fetch_all_ssa_ctxt_from_hash_table(p_ssa_pdb->p_glb_pdb);
        for (p_temp = p_root; ICF_NULL != p_temp; p_temp = p_temp->p_next)
        {
            for (p_ssa_list = (icf_ssa_ctxt_list_st *)p_temp->p_data; ICF_NULL != p_ssa_list ;
                    p_ssa_list = p_ssa_list->p_next)
            {
                p_ssa_ctxt = p_ssa_list->p_ssa_ctxt;
                ICF_PRINT(((icf_uint8_t *)"Found SSA CONTEXT\n"))

                if ((p_ssa_ctxt->p_conn_table_req_entry ==
                            p_conn_db_node) ||
                        (p_ssa_ctxt->p_conn_table_resp_entry ==
                         p_conn_db_node)) 
                {
#ifdef ICF_LOAD_DEBUG
                printf("\nCloseInd FD(2): %d,\tCall-ID: %s\n", p_ssa_pdb->p_conn_table_entry->conn_fd, p_ssa_ctxt->p_hash_key);
#endif
                    icf_ssa_reset_fd_in_uaCallObj(p_ssa_ctxt->p_call_obj,
                            p_conn_db_node->conn_fd);

                    if(0 == ICF_PURGE_TIMER_DURATION)
                    {
                        non_persistent_break = ICF_TRUE;
                    }
                }
                if (p_ssa_ctxt->p_conn_table_req_entry ==
                        p_conn_db_node)
                {
                    ICF_PRINT(((icf_uint8_t *)"Making its req connection entry NULL\n"))
                    p_ssa_ctxt->p_conn_table_req_entry
                        = ICF_NULL;
                }
                if (p_ssa_ctxt->p_conn_table_resp_entry ==
                        p_conn_db_node)
                {
                    ICF_PRINT(((icf_uint8_t *)"Making its resp connection entry NULL\n"))
                    p_ssa_ctxt->p_conn_table_resp_entry
                        = ICF_NULL;
                }

                if(ICF_TRUE == non_persistent_break)
                {
                    break;
                }
                /* reset the conn_db_index in the ssa_pdb context */
            }

            if(ICF_TRUE == non_persistent_break)
            {
                break;
            }
        }
        p_ssa_pdb->p_conn_table_entry = ICF_NULL;
        icf_ssa_remove_conn_entry(p_ssa_pdb ,
                p_conn_db_node->conn_fd);
    }

#ifdef ICF_STAT_ENABLE
        /* Stat collection*/
   /* stat collection for TCP and TLS connection*/ 
   if (nw_interface_info.bitmap == ICF_INTF_INFO_PROTOCOL_TCP)
    {
    ICF_STATS(p_ssa_pdb->p_glb_pdb->p_stats_info->
        stat_tcp_connections.stats_remote_closure_for_local_conn)

    (p_ssa_pdb->p_glb_pdb->p_stats_info->stat_tcp_connections.
                            stats_active_conn)--;
     }
   else
     {
    ICF_STATS(p_ssa_pdb->p_glb_pdb->p_stats_info->
                stat_tls_connections.stats_remote_closure_for_local_conn)

    (p_ssa_pdb->p_glb_pdb->p_stats_info->stat_tls_connections.
                                                        stats_active_conn)--;

     }
#endif

   icf_ssa_free_list(p_ssa_pdb,p_root);
    ICF_FUNCTION_EXIT(p_ssa_pdb->p_glb_pdb)
    return ret_val;
}

/*****************************************************************************
 ** FUNCTION:      icf_ssa_tcpm_01_close_req
 **
 ** DESCRIPTION: This is invoked when UATK requests conn closure
 ** Check if any messages are pending in the queue, if so, 
 ** release the memory for them and then invoke ES to close the conn

 7.0: Only TLS connection, if not persistent shall be removed and closed.
 *****************************************************************************/
icf_return_t icf_ssa_tcpm_01_close_req(
        icf_ssa_pdb_st   *p_ssa_pdb)
{
    icf_return_t     ret_val = ICF_SUCCESS;
    icf_ssa_tcpm_conn_data_st  *p_conn_db_node = 
       p_ssa_pdb->p_conn_table_entry;

    icf_nw_interface_info_st nw_interface_info;
    icf_ssa_ctxt_st  *p_ssa_ctxt = ICF_NULL;
    icf_ssa_ctxt_list_st    *p_ssa_list = ICF_NULL;
    icf_list_st          *p_root = ICF_NULL;
    icf_list_st          *p_temp = ICF_NULL;
#ifdef ICF_IPSEC_ENABLED
    icf_boolean_t        delete_connection = ICF_FALSE;
#endif
    icf_interface_info_st  interface_info;
#ifdef ICF_STAT_ENABLE
    icf_boolean_t        update_stats = ICF_FALSE;
#endif
    icf_boolean_t        non_persistent_break=ICF_FALSE;

    ICF_FUNCTION_ENTER(p_ssa_pdb->p_glb_pdb)

    if(ICF_SSA_SECURE_CONN == (p_ssa_pdb->common_bitmask & ICF_SSA_SECURE_CONN))
    {
        ICF_SSA_APP_TRACE(p_ssa_pdb,ICF_TRACE_TLS_CONNECTION_CLOSE)
    }
       
    else
    {
        ICF_SSA_APP_TRACE(p_ssa_pdb, ICF_TRACE_TCP_CONNECTION_CLOSE)
    }

    /*bug fix*/
    nw_interface_info.bitmap = ICF_NULL;
     
    if (ICF_FAILURE == icf_dbm_get_module_glb_data(
        p_ssa_pdb->p_glb_pdb,ICF_INT_MODULE_SSA,
        (icf_void_t **)&(p_ssa_pdb->p_ssa_glb_data) ,
        p_ssa_pdb->p_ecode))
    {
        return ICF_FAILURE;
    }
    /* get the config data pointer from DBM */
    if (ICF_FAILURE == icf_dbm_get_module_glb_data(
        p_ssa_pdb->p_glb_pdb,
        (icf_int_module_id_t)ICF_INT_MODULE_CFG,
        (icf_void_t **)&(p_ssa_pdb->p_glb_cfg_data),
        p_ssa_pdb->p_ecode))
    {
        return ICF_FAILURE;
    }
    if (ICF_NULL != p_conn_db_node)
    {

        nw_interface_info.bitmap = ICF_INTF_INFO_PROTOCOL_TCP;
        nw_interface_info.receiver_fd = p_conn_db_node->conn_fd;

            if ((p_conn_db_node->is_persistent != ICF_TRUE) ||
                    (0 == ICF_PURGE_TIMER_DURATION))
            {
                /* SPR 16019:Check if any message is pending on the TCP socket,
                 * then no need to close the socket
                 */
                interface_info.p_port_info = p_ssa_pdb->p_glb_pdb->p_glb_data->p_glb_port_info;
                interface_info.interface_type = ICF_INTF_TYPE_FIFO;
                interface_info.fd_id = nw_interface_info.receiver_fd;
                interface_info.interface_type = ICF_INTF_TYPE_TCP;

                p_root =  icf_dbm_fetch_all_ssa_ctxt_from_hash_table(p_ssa_pdb->p_glb_pdb);
                for (p_temp = p_root; ICF_NULL != p_temp; p_temp = p_temp->p_next)
                {
                    for (p_ssa_list = (icf_ssa_ctxt_list_st *)p_temp->p_data; ICF_NULL != p_ssa_list ;
                            p_ssa_list = p_ssa_list->p_next)
                    {
                        p_ssa_ctxt = p_ssa_list->p_ssa_ctxt;
                        if (p_ssa_ctxt->p_conn_table_req_entry ==
                                p_conn_db_node)
                        {
                            ICF_PRINT(((icf_uint8_t *)"Resetting request conn entry\n"));
                            p_ssa_ctxt->p_conn_table_req_entry = ICF_NULL;

                            if(0 == ICF_PURGE_TIMER_DURATION)
                            {
                                non_persistent_break = ICF_TRUE;
                            }
                        }
                        if (p_ssa_ctxt->p_conn_table_resp_entry ==
                                p_conn_db_node)
                        {
                            if(0 == ICF_PURGE_TIMER_DURATION)
                            {
                                non_persistent_break = ICF_TRUE;
                            }
                            ICF_PRINT(((icf_uint8_t *)"Resetting response conn entry\n"));
                            p_ssa_ctxt->p_conn_table_resp_entry = ICF_NULL;
                        }

                        if(ICF_TRUE == non_persistent_break)
                        {
                            break;
                        }
                    }

                    if(ICF_TRUE == non_persistent_break)
                    {
                        break;
                    }
                }
                if(ICF_FAILURE == icf_port_tcp_non_blocking_peek(&interface_info,
                                     (icf_void_t*)(((icf_global_port_info_st*)(p_ssa_pdb->
                                     p_glb_pdb->p_glb_data->p_glb_port_info))->
                                    ext_info.p_tcp_recv_buffer),
                                    p_ssa_pdb->p_ecode))
                {
                     if(ICF_NULL == (ICF_TCP_DATA_SERVER_PORT & p_conn_db_node->bitmap))
		     {
			     icf_es_close_tcp_conn(
                            p_ssa_pdb->p_glb_pdb->p_glb_data->p_glb_port_info, 
			    &nw_interface_info, p_ssa_pdb->p_ecode);
			     icf_ssa_remove_conn_entry(p_ssa_pdb ,
                            p_conn_db_node->conn_fd);
                    p_ssa_pdb->p_conn_table_entry = ICF_NULL;
                    /*icf_ssa_free_list(p_ssa_pdb,p_root);*/
#ifdef ICF_STAT_ENABLE
			     update_stats = ICF_TRUE;
#endif
			     ret_val = ICF_SUCCESS;
		     }
		}
		else
                {
                    /*IN case the Peek is succeesful that means we have some data and hence
                      will not close the connection */
                    ICF_PRINT(((icf_uint8_t *)"\n[SSA]:TCP Data pending on Socket.Socket Not Closed"));
		    ret_val = ICF_FAILURE;
		}
                icf_ssa_free_list(p_ssa_pdb,p_root); /* Memory leak fix for 17569 */

            }
    }
#ifdef ICF_STAT_ENABLE
    if(ICF_TRUE == update_stats)
    {
        /* Stat collection for TCP and TLS*/
        if(nw_interface_info.bitmap == ICF_INTF_INFO_PROTOCOL_TCP)
        {

            (p_ssa_pdb->p_glb_pdb->p_stats_info->
             stat_tcp_connections.stats_active_conn)--;
        }
        else
        {

            (p_ssa_pdb->p_glb_pdb->p_stats_info->
             stat_tls_connections.stats_active_conn)--;
        }
    }
#endif
        ICF_FUNCTION_EXIT(p_ssa_pdb->p_glb_pdb)
            return ret_val;
}
/*****************************************************************************
 ** FUNCTION:      icf_ssa_tcpm_00_connect_req
 **
 ** DESCRIPTION:  Will be invoked from INVITE/REGISTER code legs or from 
 **     Connect call back in UATK
 *****************************************************************************/
icf_return_t icf_ssa_tcpm_00_connect_req(
        icf_ssa_pdb_st   *p_ssa_pdb)
{
    icf_return_t     ret_val = ICF_SUCCESS;
    icf_ssa_tcpm_conn_data_st  *p_conn_db_node = 
       p_ssa_pdb->p_conn_table_entry;
    icf_nw_interface_info_st nw_interface_info;
    icf_ssa_ctxt_st             *p_ssa_ctxt = p_ssa_pdb->p_ssa_ctxt;

    icf_transport_mode_t	transport;
#ifdef ICF_IPSEC_ENABLED
    icf_transport_address_st     *p_transport_address = ICF_NULL;
#endif    
    Sdf_ty_s8bit                *p_method = Sdf_co_null;
    SipMessage                   *pMessage = Sdf_co_null;
    Sdf_st_error            sdf_error;
    Sdf_st_overlapTransInfo      *p_overlap_trans_info = Sdf_co_null;


    ICF_FUNCTION_ENTER(p_ssa_pdb->p_glb_pdb)
    ICF_ASSERT(p_ssa_pdb->p_glb_pdb, p_conn_db_node);

    /*bug fix*/
    nw_interface_info.bitmap = ICF_NULL;

    if (ICF_NULL == p_ssa_pdb->p_ssa_ctxt)
    {
        if (ICF_SSA_SECURE_CONN == (p_ssa_pdb->common_bitmask & ICF_SSA_SECURE_CONN))
        {
            transport = ICF_TRANSPORT_TYPE_TLS;
        }
        else
        {
            transport = ICF_TRANSPORT_TYPE_TCP;
        }
    }
    else
    {
        transport = p_ssa_ctxt->transport;
    }

    if ((ICF_TRANSPORT_TYPE_TLS == transport) && 
            (ICF_FALSE== p_ssa_pdb->p_glb_cfg_data->is_blocking_connect))
    {
        ret_val = ICF_FAILURE;
        return ret_val;
    }
    else
    {
        if (((ICF_TRUE == p_ssa_pdb->p_glb_cfg_data->is_blocking_connect) &&
                    (ICF_TRANSPORT_TYPE_TLS == transport)) ||\
                (ICF_TRANSPORT_TYPE_TCP == transport && (ICF_TRUE == \
                         p_ssa_pdb->p_glb_cfg_data->is_tcp_blocking_connect)))
        {
            nw_interface_info.bitmap |= ICF_INTF_INFO_BLK_CONN;
        }
    }
#ifdef ICF_SECURE_SUPPORT     
    if (ICF_TRANSPORT_TYPE_TLS == transport)
    {
        nw_interface_info.bitmap |= ICF_INTF_INFO_PROTOCOL_TLS;
        nw_interface_info.p_secure_data = p_ssa_pdb->p_ssa_glb_data->p_default_profile->pSslData->pData;
    }
    else
    {
#endif        
        nw_interface_info.bitmap |= ICF_INTF_INFO_PROTOCOL_TCP;
#ifdef ICF_SECURE_SUPPORT     
    }
#endif        
    nw_interface_info.remote_port_num = p_conn_db_node->peer_port_num;
    icf_port_strcpy(nw_interface_info.remote_ip_addr, 
            p_conn_db_node->peer_ip);
    nw_interface_info.p_sip_msg = ICF_NULL;
    nw_interface_info.pdu_len = 0;
    nw_interface_info.receiver_fd = p_conn_db_node->conn_fd;
#ifdef ICF_IPSEC_ENABLED
    if (ICF_TRUE == p_ssa_pdb->p_glb_cfg_data->ipsec_enabled)
    {
        /* if SA exists */
        if (ICF_NULL != 
            p_ssa_pdb->p_ssa_ctxt->ssa_sa_context.p_sa_context)
        {
            nw_interface_info.self_prot_port = 
                p_ssa_pdb->p_ssa_ctxt->ssa_sa_context.p_sa_context->
                p_ue_security_params->port_c;

            p_transport_address = &(p_ssa_pdb->p_glb_cfg_data->self_ip_address);
            if (ICF_TRANSPORT_ADDRESS_DOMAIN_NAME == 
                p_transport_address->addr.addr_type)
            {
                icf_port_strcpy(nw_interface_info.self_ip_addr, 
                    p_transport_address->addr.addr.domain.str);
            }
            else if (ICF_TRANSPORT_ADDRESS_IPV4_ADDR == 
                p_transport_address->addr.addr_type)
            {
                icf_ssa_convert_to_IP_addr(p_transport_address,
                    nw_interface_info.self_ip_addr);
            }
        }
        else
        {
            nw_interface_info.self_prot_port = ICF_NULL;
            icf_port_memset((icf_void_t*)nw_interface_info.self_ip_addr,
                0,
#ifdef ICF_IPV6_ENABLED
                ICF_MAX_IPV6_ADDRESS_LENGTH
#else
                ICF_MAX_IPV4_ADDRESS_LENGTH
#endif
                );
            /*nw_interface_info.self_ip_addr = ICF_NULL;*/
        }
    }
    else
    {
        nw_interface_info.self_prot_port = ICF_NULL;
        icf_port_memset((icf_void_t*)nw_interface_info.self_ip_addr,
                0,
        #ifdef ICF_IPV6_ENABLED
                ICF_MAX_IPV6_ADDRESS_LENGTH
#else
                ICF_MAX_IPV4_ADDRESS_LENGTH
#endif
                );
    }
#else
    nw_interface_info.self_prot_port = ICF_NULL;
    icf_port_memset((icf_void_t*)nw_interface_info.self_ip_addr,
        0,
#ifdef ICF_IPV6_ENABLED
        ICF_MAX_IPV6_ADDRESS_LENGTH
#else
        ICF_MAX_IPV4_ADDRESS_LENGTH
#endif
        );
#endif
          
        /* When there are multiple network interfaces, TCP connect should be
         * attempted from the configured IP. For bind(), the source address needed
         * to be explicitly specified as the configured self address
         */
        icf_cmn_convert_transport_addr_to_ip_string(
                 &(p_ssa_pdb->p_glb_cfg_data->self_ip_address.addr),
                 (icf_uint8_t *)&(nw_interface_info.self_ip_addr));

        ICF_PRINT(((icf_uint8_t *)"\n[SSA]: About to attempt TCP connect for: \n"));
        ICF_PRINT(((icf_uint8_t *)"[SSA]: bitmap=%x\n",nw_interface_info.bitmap));
        ICF_PRINT(((icf_uint8_t *)"[SSA]: pdu_len=%d\n",nw_interface_info.pdu_len));
        ICF_PRINT(((icf_uint8_t *)"[SSA]: self ip=%s\n",nw_interface_info.self_ip_addr));
    ICF_PRINT(((icf_uint8_t *)"[SSA]: self port=%d\n",nw_interface_info.self_prot_port));
    ICF_PRINT(((icf_uint8_t *)"[SSA]: remote ip=%s\n",nw_interface_info.remote_ip_addr));
    ICF_PRINT(((icf_uint8_t *)"[SSA]: remote port=%d\n",nw_interface_info.remote_port_num));
          
         
    if( ICF_FAILURE == icf_es_open_tcp_conn(
        p_ssa_pdb->p_glb_pdb->p_glb_data->p_glb_port_info, &nw_interface_info, 
        p_ssa_pdb->p_ecode))
    {
        if ( ICF_CONNECT_RESP_PENDING == *(p_ssa_pdb->p_ecode))
        {
            ICF_PRINT(((icf_uint8_t *)"\n[SSA]:Moving to state Await connect resp"));
#ifdef ICF_IPSEC_ENABLED
            if (ICF_NULL != nw_interface_info.self_prot_port)
            {
                p_conn_db_node->act_connec_count += 1;
            }
#endif
            p_conn_db_node->tcpm_sm_state = ICF_SSA_TCPM_AWT_CON_RESP;
            p_conn_db_node->conn_fd = (icf_int16_t)nw_interface_info.receiver_fd;
        }
        else
        {
#ifdef ICF_ERROR_ENABLE
	if(p_ssa_ctxt != ICF_NULL)
	{
            if (p_ssa_ctxt->p_call_obj->pUacTransaction->pSipMsg != Sdf_co_null)
            {
                pMessage = p_ssa_ctxt->p_call_obj->pUacTransaction->pSipMsg;
            }
            else if (Sdf_co_null != p_ssa_ctxt->p_call_obj->pUasTransaction->pSipMsg)
            {
                pMessage = p_ssa_ctxt->p_call_obj->pUasTransaction->pSipMsg;
            }
            else
            {
	              p_overlap_trans_info = (Sdf_st_overlapTransInfo*)
		                                   (p_ssa_ctxt->p_pending_txn);
	              pMessage = p_overlap_trans_info->pSipMsg;
            }
	 }		
            if(Sdf_co_fail == 
                sdf_ivk_uaGetMethodFromSipMessage(
                                pMessage,
                                &p_method, &sdf_error))
            {
                ret_val = ICF_FAILURE;
            }
            ICF_SSA_SET_ECODE(p_ssa_pdb,p_method,ICF_ERROR_OPEN_SOCKET)
#endif

            ICF_PRINT(((icf_uint8_t *)"\n[SSA]:Failure in ES open conn, \
                        resetting the conn db node"));
#ifdef ICF_IPSEC_ENABLED
            p_conn_db_node->act_connec_count = 0;
#endif
            /*Stat Collection of TCP and TLS*/

         if(ICF_TRANSPORT_TYPE_TLS == transport)
         {
            ICF_STATS(p_ssa_pdb->p_glb_pdb->p_stats_info->
                stat_tls_connections.stats_conn_failures)
        }
        else
        {
            ICF_STATS(p_ssa_pdb->p_glb_pdb->p_stats_info->
                                stat_tcp_connections.stats_conn_failures)
        }
            p_ssa_pdb->p_conn_table_entry = ICF_NULL;        

            ret_val = ICF_FAILURE;
        }
    }

    else
    {
        ICF_PRINT(((icf_uint8_t *)"\n[SSA]:ConnectSuccess, Moving to state connected"));
#ifdef ICF_IPSEC_ENABLED
        /* this count has significance only is SA is established else it 
         * this count should not be tampered with
         */
        if (ICF_NULL != nw_interface_info.self_prot_port)
        {
            p_conn_db_node->act_connec_count += 1;
        }
#endif

        p_conn_db_node->tcpm_sm_state = ICF_SSA_TCPM_CONNECTED;
        p_conn_db_node->conn_fd = (icf_int16_t)nw_interface_info.receiver_fd;
              /*Stat Collection of TCP and TLS*/

        if(ICF_TRANSPORT_TYPE_TLS == transport)
        {
             ICF_STATS(p_ssa_pdb->p_glb_pdb->p_stats_info->
                     stat_tls_connections.total_tls_conn_since_last_reboot)

        ICF_STATS(p_ssa_pdb->p_glb_pdb->p_stats_info->
                     stat_tls_connections.stats_active_conn)
        }
        else
        {
                 ICF_STATS(p_ssa_pdb->p_glb_pdb->p_stats_info->
                     stat_tcp_connections.total_tcp_conn_since_last_reboot)

        ICF_STATS(p_ssa_pdb->p_glb_pdb->p_stats_info->
                     stat_tcp_connections.stats_active_conn)
        }

    }

    ICF_FUNCTION_EXIT(p_ssa_pdb->p_glb_pdb)
    return ret_val;
}

/*****************************************************************************
 ** FUNCTION:      icf_ssa_tcpm_03_purge_timer_xpry
 **
 ** DESCRIPTION:  Move to state 4
 *****************************************************************************/
icf_return_t icf_ssa_tcpm_03_purge_timer_xpry(
        icf_ssa_pdb_st   *p_ssa_pdb)
{
    icf_return_t     ret_val = ICF_SUCCESS;
    icf_ssa_tcpm_conn_data_st  *p_conn_db_node = 
        p_ssa_pdb->p_conn_table_entry;
    ICF_FUNCTION_ENTER(p_ssa_pdb->p_glb_pdb)
        if (p_conn_db_node->ref_count == 0)
            p_conn_db_node->timer_count++;

    if (p_conn_db_node->timer_count > ICF_MAX_CONN_DURATION_LIMIT)
        p_conn_db_node->tcpm_sm_state = ICF_SSA_TCPM_NOT_INUSE;

    ICF_FUNCTION_EXIT(p_ssa_pdb->p_glb_pdb)
        return ret_val;
}

/*****************************************************************************
 ** FUNCTION:      icf_ssa_tcpm_04_data_ind
 **
 ** DESCRIPTION:   DataReceived first time on incoming connection
 **                move to INUSE state from NOT_INUSE
 *****************************************************************************/
icf_return_t icf_ssa_tcpm_04_data_ind(
        icf_ssa_pdb_st   *p_ssa_pdb)
{
    icf_return_t     ret_val = ICF_SUCCESS;
    icf_ssa_tcpm_conn_data_st  *p_conn_db_node = 
        p_ssa_pdb->p_conn_table_entry;

    ICF_FUNCTION_ENTER(p_ssa_pdb->p_glb_pdb);
    p_conn_db_node->tcpm_sm_state = ICF_SSA_TCPM_INUSE;
    p_conn_db_node->timer_count = 0;
    ICF_FUNCTION_EXIT(p_ssa_pdb->p_glb_pdb);
    return ret_val;
}


/*****************************************************************************
 ** FUNCTION:      icf_ssa_tcpm_04_purge_timer_xpry
 **
 ** DESCRIPTION:  Close the connection, issue close conn to ES, 
 **     reset the node and the socket in CallObject, if any is associated
 *****************************************************************************/
icf_return_t icf_ssa_tcpm_04_purge_timer_xpry(
        icf_ssa_pdb_st   *p_ssa_pdb)
{
    icf_return_t     ret_val = ICF_SUCCESS;
    icf_ssa_tcpm_conn_data_st  *p_conn_db_node = 
        p_ssa_pdb->p_conn_table_entry;
    icf_nw_interface_info_st nw_interface_info;
    icf_ssa_ctxt_st  *p_ssa_ctxt = ICF_NULL;
    icf_ssa_ctxt_list_st    *p_ssa_list = ICF_NULL;
    icf_list_st          *p_root = ICF_NULL;
    icf_list_st          *p_temp = ICF_NULL;


    ICF_FUNCTION_ENTER(p_ssa_pdb->p_glb_pdb)

    nw_interface_info.bitmap = ICF_INTF_INFO_PROTOCOL_TCP;
    nw_interface_info.receiver_fd = p_conn_db_node->conn_fd;
    p_conn_db_node->bitmap &= ~(ICF_TCP_DATA_SERVER_PORT);
    icf_es_close_tcp_conn(
    p_ssa_pdb->p_glb_pdb->p_glb_data->p_glb_port_info, 
    &nw_interface_info, p_ssa_pdb->p_ecode);
    p_root =  icf_dbm_fetch_all_ssa_ctxt_from_hash_table(p_ssa_pdb->p_glb_pdb);
    ICF_PRINT(((icf_uint8_t *)"Closing connection fd %d", p_ssa_pdb->p_conn_table_entry->conn_fd))
    for (p_temp = p_root; ICF_NULL != p_temp; p_temp = p_temp->p_next)
    {
        for (p_ssa_list = (icf_ssa_ctxt_list_st *)p_temp->p_data; ICF_NULL != p_ssa_list ;
                p_ssa_list = p_ssa_list->p_next)
        {
            p_ssa_ctxt = p_ssa_list->p_ssa_ctxt;
            if ((p_ssa_ctxt->p_conn_table_req_entry
                        == p_conn_db_node) ||
                    (p_ssa_ctxt->p_conn_table_resp_entry
                     == p_conn_db_node))
            { 
                ICF_PRINT(((icf_uint8_t *)"Setting connection table entry to NULL for ssa context\n"))
                ICF_ASSERT(p_ssa_pdb->p_glb_pdb, p_ssa_ctxt->p_call_obj)
                    /*        ICF_SSA_DEBUG(("\n[SSA]:InvokingResetSocketAPI on the callobject"));*/
                    icf_ssa_reset_fd_in_uaCallObj(
                            p_ssa_ctxt->p_call_obj,
                            p_conn_db_node->conn_fd);	

                p_ssa_ctxt->p_conn_table_req_entry = ICF_NULL;
            }
            /* In TLS, we shall have both request and response connections same */
            if (p_ssa_ctxt->p_conn_table_resp_entry
                    == p_conn_db_node)
            {
                p_ssa_ctxt->p_conn_table_resp_entry = ICF_NULL;
            }
            if (p_ssa_ctxt->p_conn_table_req_entry
                    == p_conn_db_node)
            {
                p_ssa_ctxt->p_conn_table_req_entry = ICF_NULL;
            }
        }
    }
        
    icf_ssa_remove_conn_entry(p_ssa_pdb ,
            p_conn_db_node->conn_fd);
	/* reset the conn_db_index in the ssa_pdb context */
	p_ssa_pdb->p_conn_table_entry = ICF_NULL;

    icf_ssa_free_list(p_ssa_pdb,p_root);
    ICF_FUNCTION_EXIT(p_ssa_pdb->p_glb_pdb)
    return ret_val;
}

/*****************************************************************************
 ** FUNCTION:      icf_ssa_tcpm_01_replace_conn_fd
 **
 ** DESCRIPTION:  Replace the connection in ssa_ctx with connection in
 **               ssa_pdb
 **     
 *****************************************************************************/
icf_return_t icf_ssa_tcpm_01_replace_conn_fd(
        icf_ssa_pdb_st   *p_ssa_pdb)
{
    icf_return_t               ret_val = ICF_SUCCESS;
    
    icf_ssa_tcpm_conn_data_st  *p_ssa_ctxt_db_node = ICF_NULL;
                                  

    icf_ssa_ctxt_st            *p_ssa_ctxt = p_ssa_pdb->p_ssa_ctxt;
    
    /*If valid PDU copied then send it to network*/
    icf_uint32_t               count = 0;
    icf_nw_interface_info_st   nw_interface_info;
    en_SipMessageType          dMsgType;
    icf_ssa_ctxt_st  *p_ssa_ctxt_temp = ICF_NULL;
    icf_ssa_ctxt_list_st    *p_ssa_list = ICF_NULL;
    icf_list_st          *p_root = ICF_NULL;
    icf_list_st          *p_temp = ICF_NULL;
   
    
    ICF_FUNCTION_ENTER(p_ssa_pdb->p_glb_pdb)

    dMsgType = p_ssa_pdb->dMsgType;

    if (p_ssa_ctxt == ICF_NULL)
    {
        return ICF_FAILURE;
    }
    if (dMsgType == SipMessageRequest)
        p_ssa_ctxt_db_node  = p_ssa_ctxt->p_conn_table_resp_entry;
    else
        p_ssa_ctxt_db_node  = p_ssa_ctxt->p_conn_table_req_entry;

    /*bug fix*/
    nw_interface_info.bitmap = ICF_NULL;


    /*Fetch conn_db_node for index in ssa_pdb*/
#if 0
    p_ssa_pdb_db_node = 
        p_ssa_pdb-p_conn_table_entry;
    /* Check if any pending PDU is there in node of ssa_ctx*/
    while (pdu_count < ICF_MAX_PENDING_PDU_PER_CONN)
    {
        /*If PDU is pending then copy pointer in ssa_pdb node*/
        if (ICF_NULL != 
                p_ssa_ctxt_db_node->p_pending_pdu[pdu_count])
        {
            p_ssa_pdb_db_node->p_pending_pdu[pdu_count] = 
                p_ssa_ctxt_db_node->p_pending_pdu[pdu_count];
            pdu_count++;
        } /* p_ssa_ctxt_db_node->p_pending_p... */
        else
        {
            break;
        } /* else */
    } /* while (pdu_count < ICF_MAX_... */
#endif
    /*Close TCP connection of ssa_ctx*/

    nw_interface_info.bitmap = ICF_INTF_INFO_PROTOCOL_TCP;
    nw_interface_info.receiver_fd = p_ssa_ctxt_db_node->conn_fd;

    p_root =  icf_dbm_fetch_all_ssa_ctxt_from_hash_table(p_ssa_pdb->p_glb_pdb);
    for (p_temp = p_root,count = 0; ICF_NULL != p_temp; p_temp = p_temp->p_next)
    {
        for (p_ssa_list = (icf_ssa_ctxt_list_st *)p_temp->p_data; ICF_NULL != p_ssa_list ;
                p_ssa_list = p_ssa_list->p_next)
        {
            p_ssa_ctxt_temp = p_ssa_list->p_ssa_ctxt;
            if (p_ssa_pdb->p_ssa_ctxt != p_ssa_ctxt_temp)
            {
                if ((p_ssa_ctxt_temp->p_conn_table_req_entry == p_ssa_pdb->p_conn_table_entry)
                        || (p_ssa_ctxt_temp->p_conn_table_resp_entry == p_ssa_pdb->p_conn_table_entry))
                {
                    count++;
                }
            }
        }
    }

    if (p_ssa_pdb->p_ssa_ctxt != ICF_NULL)
    {
        icf_ssa_reset_fd_in_uaCallObj(p_ssa_pdb->p_ssa_ctxt->p_call_obj,
                p_ssa_ctxt_db_node->conn_fd);
    }
    if  (count == 0)
    {
        icf_es_close_tcp_conn(
            p_ssa_pdb->p_glb_pdb->p_glb_data->p_glb_port_info, 
            &nw_interface_info, p_ssa_pdb->p_ecode);
        icf_ssa_remove_conn_entry(p_ssa_pdb ,
            p_ssa_ctxt_db_node->conn_fd);
    }
    else
    {
        p_ssa_ctxt_db_node->ref_count--;
    }
    /* here we will not reset the ssa_pdb->conn_sb_index coz it holds the 
     * new connection pointer */
    /*Copy new conn_fd*/
    /* No need to have this segment as now pendin pdus are part of ssa context
     * */
#if 0
    sip_trans_addr.dSockFd = p_ssa_pdb_db_node->conn_fd;
#ifdef ICF_SECURE_SUPPORT     
    if (ICF_TRANSPORT_TYPE_TLS == p_ssa_pdb->p_ssa_ctxt->transport)
    {
        protocol = Sdf_en_protoTls;
    }
    else
    {
#endif        
        protocol = Sdf_en_protoTcp;
#ifdef ICF_SECURE_SUPPORT     
    }
#endif        
    while ( counter < pdu_count)
    {
        if (ICF_NULL != p_ssa_pdb_db_node->p_pending_pdu[counter])
        {
            pdu_len =
                icf_port_strlen(
                        p_ssa_pdb_db_node->p_pending_pdu[counter]);

            icf_ssa_dispatch_pdu_to_es(p_glb_pdb, p_ssa_pdb->p_ssa_ctxt,
                    &sip_trans_addr,
                    protocol,
                    p_ssa_pdb_db_node->p_pending_pdu[counter],
                    pdu_len, p_ssa_ctxt->line_id, p_ssa_pdb->p_ecode);

            p_ssa_pdb_db_node->p_pending_pdu[counter] = ICF_NULL;

            counter++;
        }
        else
        {
            break;
        }
    }
#endif
    /*Copy index in ssa_ctx from ssa_pdb*/
    icf_ssa_free_list(p_ssa_pdb,p_root);
    ICF_FUNCTION_EXIT(p_ssa_pdb->p_glb_pdb)

    return ret_val;    
} /* icf_ssa_tcpm_01_replace_conn_fd */


/*****************************************************************************
 ** FUNCTION:      icf_ssa_tcpm_xx_replace_conn_fd
 **
 ** DESCRIPTION:  Replace the connection in ssa_ctx with connection in
 **               ssa_pdb
 **     
 *****************************************************************************/
icf_return_t icf_ssa_tcpm_xx_replace_conn_fd(
        icf_ssa_pdb_st   *p_ssa_pdb)
{
#if 0
    icf_return_t   ret_val = ICF_SUCCESS;
    icf_ssa_ctxt_st  *p_ssa_ctxt = p_ssa_pdb->p_ssa_ctxt;
    icf_nw_interface_info_st   nw_interface_info;
    icf_ssa_tcpm_conn_data_st  *p_ssa_ctxt_db_node = 
                &(p_ssa_pdb->p_ssa_glb_data->ssa_tcpm_conn_db
                            [p_ssa_ctxt->ssa_conn_db_index]);


    ICF_FUNCTION_ENTER(p_ssa_pdb->p_glb_pdb)

    /* Close TCP connection of ssa_ctx */
    nw_interface_info.bitmap = ICF_INTF_INFO_PROTOCOL_TCP;
    nw_interface_info.receiver_fd = p_ssa_ctxt_db_node->conn_fd;

    icf_es_close_tcp_conn(
        p_ssa_pdb->p_glb_pdb->p_glb_data->p_glb_port_info, 
        &nw_interface_info, p_ssa_pdb->p_ecode);

    if ( ICF_NULL != p_ssa_ctxt_db_node->p_ssa_ctxt)
    {
        /* ICF_SSA_DEBUG(("\n[SSA]:InvokingResetSocketAPI on the callobject"));*/
        icf_ssa_reset_fd_in_uaCallObj(p_ssa_ctxt_db_node->p_ssa_ctxt->p_call_obj,
                p_ssa_ctxt_db_node->conn_fd);
    }

    icf_ssa_reset_conn_db_node(p_ssa_pdb, p_ssa_ctxt->ssa_conn_db_index);
    /* here we will not reset the ssa_pdb->conn_sb_index coz it holds the 
     * new connection pointer */
    /*Copy index in ssa_ctx from ssa_pdb*/
    p_ssa_ctxt->ssa_conn_db_index = p_ssa_pdb->ssa_conn_db_index;

    ICF_FUNCTION_EXIT(p_ssa_pdb->p_glb_pdb)
    return ret_val;
#endif
    return icf_ssa_tcpm_01_replace_conn_fd(p_ssa_pdb);
} /* icf_ssa_pdb_st   *p_ssa_pdb... */
   

icf_return_t icf_ssa_tcpm_ret_success(
        icf_ssa_pdb_st   *p_ssa_pdb)
{
    icf_return_t     ret_val = ICF_SUCCESS;
    (void)p_ssa_pdb;
    ICF_FUNCTION_ENTER(p_ssa_pdb->p_glb_pdb)
    ICF_FUNCTION_EXIT(p_ssa_pdb->p_glb_pdb)
    return ret_val;
}

icf_return_t icf_ssa_tcpm_ret_fail(
        icf_ssa_pdb_st   *p_ssa_pdb)
{
    icf_return_t     ret_val = ICF_FAILURE;
    (void)p_ssa_pdb;
    ICF_FUNCTION_ENTER(p_ssa_pdb->p_glb_pdb)
    ICF_FUNCTION_EXIT(p_ssa_pdb->p_glb_pdb)
    return ret_val;
}
icf_void_t      icf_ssa_init_conn_db (icf_ssa_tcpm_conn_data_st *p_conn)
{
    if (p_conn == ICF_NULL)
        return;

    p_conn->conn_fd = ICF_INVALID_SOCKFD;
    p_conn->peer_ip[0] = '\0';
    p_conn->timer_count = 0;
    p_conn->ref_count = 0;
    p_conn->is_persistent = ICF_FALSE;
    p_conn->tcpm_sm_state = ICF_SSA_TCPM_IDLE;
    p_conn->peer_port_num = 0;
    p_conn->prot_conn_bitmask = 0;
#ifdef ICF_SECURE_SUPPORT
    p_conn->p_peer_name = ICF_NULL;
#endif
}
/**************************************************************************************
 * FUNCTION    icf_ssa_print_conn_db
 *
 * ************************************************************************************/
icf_void_t      icf_ssa_print_conn_db (const icf_ssa_tcpm_conn_data_st *p_conn)
{
    if (p_conn == ICF_NULL)
        return;
    ICF_PRINT(((icf_uint8_t *)"\n#############################################\n"))
    ICF_PRINT(((icf_uint8_t *)"Connection Table Entry \n"))

    ICF_PRINT(((icf_uint8_t *)"Connection Fd = %d\n",p_conn->conn_fd))
    ICF_PRINT(((icf_uint8_t *)"Destination IP= %s\n",p_conn->peer_ip))
    ICF_PRINT(((icf_uint8_t *)"Destination port= %d\n",p_conn->peer_port_num))
    ICF_PRINT(((icf_uint8_t *)"Timer Count = %lu\n",p_conn->timer_count))
    ICF_PRINT(((icf_uint8_t *)"Ref Count = %lu\n",p_conn->ref_count))
    ICF_PRINT(((icf_uint8_t *)"Persistent state = %d\n",p_conn->is_persistent))
    if (p_conn->prot_conn_bitmask & ICF_CONN_TYPE_REQUEST)
    {
        ICF_PRINT(((icf_uint8_t *)"Connection type = Request\n"))
    }
    else if (p_conn->prot_conn_bitmask & ICF_CONN_TYPE_RESPONSE)
    {
        ICF_PRINT(((icf_uint8_t *)"Connection type = Response\n"))
    }
    else if (p_conn->prot_conn_bitmask & ICF_CONN_TYPE_BOTH)
    {
        ICF_PRINT(((icf_uint8_t *)"Connection type = Both\n"))
    }
    else
    {
        ICF_PRINT(((icf_uint8_t *)"Connection type = INVALID\n"))
    }

    if (p_conn->prot_conn_bitmask & ICF_TRANSPORT_TCP)
    {
        ICF_PRINT(((icf_uint8_t *)"Connection type = TCP\n"))
    }
#ifdef ICF_SECURE_SUPPORT
    else if (p_conn->prot_conn_bitmask & ICF_TRANSPORT_TLS)
    {
        ICF_PRINT(("Connection type = TLS\n"))
    }
#endif
#ifdef ICF_SECURE_SUPPORT
    if (ICF_NULL != p_conn->p_peer_name)
    {
        ICF_PRINT(((icf_uint8_t *)"Peer Name = %s",p_conn->p_peer_name))
    }
#endif
    ICF_PRINT(((icf_uint8_t *)"#############################################\n\n\n"))
}
icf_void_t  icf_ssa_print_all_connection(icf_ssa_pdb_st *p_ssa_pdb)
{
    icf_uint16_t i, count;
    for (i = 0, count = 0; i < ICF_MAX_NO_OF_PERSISTENT_CONNECTIONS; i++)
    {
        if (p_ssa_pdb->p_ssa_glb_data->p_ssa_tcpm_conn_db[i].conn_fd !=
                ICF_INVALID_SOCKFD)
        {
            icf_ssa_print_conn_db(&p_ssa_pdb->p_ssa_glb_data->p_ssa_tcpm_conn_db[i]);
            count++;
        }
    }
    ICF_PRINT(((icf_uint8_t *)"Total %d connections are present in the system\n", count))
}
/*****************************************************************************
 ** FUNCTION:      icf_ssa_init_tcp_sm
 **
 ** DESCRIPTION: This routine does the pointer assignments for TCP SM
 **              Called at init time in SSA INIT
 *****************************************************************************/
icf_return_t     icf_ssa_init_tcp_sm
(icf_ssa_pdb_st  *p_ssa_pdb)
{
    icf_return_t     ret_val = ICF_SUCCESS;
    icf_uint32_t      counter = 0;
    icf_error_t     ecode;

    ICF_FUNCTION_ENTER(p_ssa_pdb->p_glb_pdb);

        /*Initialization of the TCPM database*/
    ICF_MEMGET(p_persistent_glb_pdb,(ICF_MAX_NO_OF_PERSISTENT_CONNECTIONS * \
            sizeof(icf_ssa_tcpm_conn_data_st)) , ICF_MEM_COMMON,\
            p_ssa_pdb->p_ssa_glb_data->p_ssa_tcpm_conn_db,\
            ICF_RET_ON_FAILURE,\
            &ecode, ret_val);
    for (counter = 0; counter < ICF_MAX_NO_OF_PERSISTENT_CONNECTIONS;
            counter++)
    {
#if 0
        p_ssa_pdb->p_ssa_glb_data->
            p_ssa_tcpm_conn_db[counter].p_ssa_ctxt = ICF_NULL;
#endif
       icf_ssa_init_conn_db(&( p_ssa_pdb->p_ssa_glb_data->
            p_ssa_tcpm_conn_db[counter]));

    }

    /*Pointer assignments for functions*/

    /*State ICF_SSA_TCPM_IDLE*/
    p_ssa_pdb->p_ssa_glb_data->ssa_tcpm_fsm_table
        [ICF_SSA_TCPM_IDLE][ICF_SSA_TCPM_OPEN_CONN_REQ]
        = icf_ssa_tcpm_00_connect_req;
    
    p_ssa_pdb->p_ssa_glb_data->ssa_tcpm_fsm_table
        [ICF_SSA_TCPM_IDLE][ICF_SSA_TCPM_CLOSE_CONN_REQ]
        = icf_ssa_tcpm_ret_success;

    p_ssa_pdb->p_ssa_glb_data->ssa_tcpm_fsm_table
        [ICF_SSA_TCPM_IDLE][ICF_SSA_TCPM_WRITE_TO_CONN]
        = icf_ssa_tcpm_ret_fail;

    p_ssa_pdb->p_ssa_glb_data->ssa_tcpm_fsm_table
        [ICF_SSA_TCPM_IDLE][ICF_SSA_TCPM_SUCC_OPEN_CONN_RESP]
        = icf_ssa_tcpm_ret_fail;

    p_ssa_pdb->p_ssa_glb_data->ssa_tcpm_fsm_table
        [ICF_SSA_TCPM_IDLE][ICF_SSA_TCPM_FAIL_OPEN_CONN_RESP]
        = icf_ssa_tcpm_ret_fail;

    p_ssa_pdb->p_ssa_glb_data->ssa_tcpm_fsm_table
        [ICF_SSA_TCPM_IDLE][ICF_SSA_TCPM_PURGE_TIMER_XPRY]
    /*returned success as in this state there is no effect of this
     * timer*/
        = icf_ssa_tcpm_ret_success;

    p_ssa_pdb->p_ssa_glb_data->ssa_tcpm_fsm_table
        [ICF_SSA_TCPM_IDLE][ICF_SSA_TCPM_CLOSE_CONN_IND]
        = icf_ssa_tcpm_ret_fail;

    p_ssa_pdb->p_ssa_glb_data->ssa_tcpm_fsm_table
        [ICF_SSA_TCPM_IDLE][ICF_SSA_TCPM_OPEN_CONN_IND]
        = icf_ssa_tcpm_00_inc_conn_ind;

    p_ssa_pdb->p_ssa_glb_data->ssa_tcpm_fsm_table
        [ICF_SSA_TCPM_IDLE][ICF_SSA_TCPM_DATA_IND]
        = icf_ssa_tcpm_ret_fail;

    p_ssa_pdb->p_ssa_glb_data->ssa_tcpm_fsm_table
        [ICF_SSA_TCPM_IDLE][ICF_SSA_TCPM_REPLACE_CONN_FD]
        = icf_ssa_tcpm_xx_replace_conn_fd;

    /*State ICF_SSA_TCPM_AWT_CON_RESP*/
    p_ssa_pdb->p_ssa_glb_data->ssa_tcpm_fsm_table
        [ICF_SSA_TCPM_AWT_CON_RESP][ICF_SSA_TCPM_OPEN_CONN_REQ]
        = icf_ssa_tcpm_ret_fail;

    p_ssa_pdb->p_ssa_glb_data->ssa_tcpm_fsm_table
        [ICF_SSA_TCPM_AWT_CON_RESP][ICF_SSA_TCPM_CLOSE_CONN_REQ]
        = icf_ssa_tcpm_01_close_req;

    p_ssa_pdb->p_ssa_glb_data->ssa_tcpm_fsm_table
        [ICF_SSA_TCPM_AWT_CON_RESP][ICF_SSA_TCPM_WRITE_TO_CONN]
        = icf_ssa_tcpm_01_write_to_conn;

    p_ssa_pdb->p_ssa_glb_data->ssa_tcpm_fsm_table
        [ICF_SSA_TCPM_AWT_CON_RESP][ICF_SSA_TCPM_SUCC_OPEN_CONN_RESP]
        = icf_ssa_tcpm_01_succ_open_conn_resp;

    p_ssa_pdb->p_ssa_glb_data->ssa_tcpm_fsm_table
        [ICF_SSA_TCPM_AWT_CON_RESP][ICF_SSA_TCPM_FAIL_OPEN_CONN_RESP]
        = icf_ssa_tcpm_01_fail_open_conn_resp;

    p_ssa_pdb->p_ssa_glb_data->ssa_tcpm_fsm_table
        [ICF_SSA_TCPM_AWT_CON_RESP][ICF_SSA_TCPM_REPLACE_CONN_FD]
        = icf_ssa_tcpm_01_replace_conn_fd;

    p_ssa_pdb->p_ssa_glb_data->ssa_tcpm_fsm_table
        [ICF_SSA_TCPM_AWT_CON_RESP][ICF_SSA_TCPM_PURGE_TIMER_XPRY]
        = icf_ssa_tcpm_ret_success;

    p_ssa_pdb->p_ssa_glb_data->ssa_tcpm_fsm_table
        [ICF_SSA_TCPM_AWT_CON_RESP][ICF_SSA_TCPM_CLOSE_CONN_IND]
        /*Since connection is not established yet, there cannot be a 
         * close connection from peer*/
        = icf_ssa_tcpm_ret_fail;

    p_ssa_pdb->p_ssa_glb_data->ssa_tcpm_fsm_table
        [ICF_SSA_TCPM_AWT_CON_RESP][ICF_SSA_TCPM_OPEN_CONN_IND]
        = icf_ssa_tcpm_ret_fail;

    p_ssa_pdb->p_ssa_glb_data->ssa_tcpm_fsm_table
        [ICF_SSA_TCPM_AWT_CON_RESP][ICF_SSA_TCPM_DATA_IND]
        /*Since connection is not established yet, there cannot be a 
         * data reception from peer*/
        = icf_ssa_tcpm_ret_fail;

    /*State ICF_SSA_TCPM_CONNECTED*/
    p_ssa_pdb->p_ssa_glb_data->ssa_tcpm_fsm_table
        [ICF_SSA_TCPM_CONNECTED][ICF_SSA_TCPM_OPEN_CONN_REQ]
        = icf_ssa_tcpm_ret_fail;

    p_ssa_pdb->p_ssa_glb_data->ssa_tcpm_fsm_table
        [ICF_SSA_TCPM_CONNECTED][ICF_SSA_TCPM_CLOSE_CONN_REQ]
        = icf_ssa_tcpm_01_close_req;

    p_ssa_pdb->p_ssa_glb_data->ssa_tcpm_fsm_table
        [ICF_SSA_TCPM_CONNECTED][ICF_SSA_TCPM_WRITE_TO_CONN]
        = icf_ssa_tcpm_02_write_to_conn;

    p_ssa_pdb->p_ssa_glb_data->ssa_tcpm_fsm_table
        [ICF_SSA_TCPM_CONNECTED][ICF_SSA_TCPM_REPLACE_CONN_FD]
        = icf_ssa_tcpm_xx_replace_conn_fd;

    p_ssa_pdb->p_ssa_glb_data->ssa_tcpm_fsm_table
        [ICF_SSA_TCPM_CONNECTED][ICF_SSA_TCPM_SUCC_OPEN_CONN_RESP]
        /*Hitting this means some corruption in ES-SSA interface 
         * so return failure*/
        = icf_ssa_tcpm_ret_fail;

    p_ssa_pdb->p_ssa_glb_data->ssa_tcpm_fsm_table
        [ICF_SSA_TCPM_CONNECTED][ICF_SSA_TCPM_FAIL_OPEN_CONN_RESP]
        /*Hitting this means some corruption in ES-SSA interface 
         * so return failure*/
        = icf_ssa_tcpm_ret_fail;

    p_ssa_pdb->p_ssa_glb_data->ssa_tcpm_fsm_table
        [ICF_SSA_TCPM_CONNECTED][ICF_SSA_TCPM_PURGE_TIMER_XPRY]
        = icf_ssa_tcpm_03_purge_timer_xpry;

    p_ssa_pdb->p_ssa_glb_data->ssa_tcpm_fsm_table
        [ICF_SSA_TCPM_CONNECTED][ICF_SSA_TCPM_CLOSE_CONN_IND]
        = icf_ssa_tcpm_01_close_ind;

    p_ssa_pdb->p_ssa_glb_data->ssa_tcpm_fsm_table
        [ICF_SSA_TCPM_CONNECTED][ICF_SSA_TCPM_OPEN_CONN_IND]
        = icf_ssa_tcpm_ret_fail;

    p_ssa_pdb->p_ssa_glb_data->ssa_tcpm_fsm_table
        [ICF_SSA_TCPM_CONNECTED][ICF_SSA_TCPM_DATA_IND]
        = icf_ssa_tcpm_ret_success;

    /*State ICF_SSA_TCPM_INUSE*/
    p_ssa_pdb->p_ssa_glb_data->ssa_tcpm_fsm_table
        [ICF_SSA_TCPM_INUSE][ICF_SSA_TCPM_OPEN_CONN_REQ]
        = icf_ssa_tcpm_ret_fail;

    p_ssa_pdb->p_ssa_glb_data->ssa_tcpm_fsm_table
        [ICF_SSA_TCPM_INUSE][ICF_SSA_TCPM_CLOSE_CONN_REQ]
        = icf_ssa_tcpm_01_close_req;

    p_ssa_pdb->p_ssa_glb_data->ssa_tcpm_fsm_table
        [ICF_SSA_TCPM_INUSE][ICF_SSA_TCPM_WRITE_TO_CONN]
        = icf_ssa_tcpm_03_write_to_conn;

    p_ssa_pdb->p_ssa_glb_data->ssa_tcpm_fsm_table
        [ICF_SSA_TCPM_INUSE][ICF_SSA_TCPM_REPLACE_CONN_FD]
        = icf_ssa_tcpm_xx_replace_conn_fd;

    p_ssa_pdb->p_ssa_glb_data->ssa_tcpm_fsm_table
        [ICF_SSA_TCPM_INUSE][ICF_SSA_TCPM_SUCC_OPEN_CONN_RESP]
        = icf_ssa_tcpm_ret_fail;

    p_ssa_pdb->p_ssa_glb_data->ssa_tcpm_fsm_table
        [ICF_SSA_TCPM_INUSE][ICF_SSA_TCPM_FAIL_OPEN_CONN_RESP]
        = icf_ssa_tcpm_ret_fail;

    p_ssa_pdb->p_ssa_glb_data->ssa_tcpm_fsm_table
        [ICF_SSA_TCPM_INUSE][ICF_SSA_TCPM_PURGE_TIMER_XPRY]
        = icf_ssa_tcpm_03_purge_timer_xpry;

    p_ssa_pdb->p_ssa_glb_data->ssa_tcpm_fsm_table
        [ICF_SSA_TCPM_INUSE][ICF_SSA_TCPM_CLOSE_CONN_IND]
        = icf_ssa_tcpm_01_close_ind;

    p_ssa_pdb->p_ssa_glb_data->ssa_tcpm_fsm_table
        [ICF_SSA_TCPM_INUSE][ICF_SSA_TCPM_OPEN_CONN_IND]
        = icf_ssa_tcpm_ret_fail;

    p_ssa_pdb->p_ssa_glb_data->ssa_tcpm_fsm_table
        [ICF_SSA_TCPM_INUSE][ICF_SSA_TCPM_DATA_IND]
        = icf_ssa_tcpm_ret_success;

    /*State ICF_SSA_TCPM_NOT_INUSE*/
    p_ssa_pdb->p_ssa_glb_data->ssa_tcpm_fsm_table
        [ICF_SSA_TCPM_NOT_INUSE][ICF_SSA_TCPM_OPEN_CONN_REQ]
        = icf_ssa_tcpm_ret_fail;

    p_ssa_pdb->p_ssa_glb_data->ssa_tcpm_fsm_table
        [ICF_SSA_TCPM_NOT_INUSE][ICF_SSA_TCPM_CLOSE_CONN_REQ]
        = icf_ssa_tcpm_01_close_req;

    p_ssa_pdb->p_ssa_glb_data->ssa_tcpm_fsm_table
        [ICF_SSA_TCPM_NOT_INUSE][ICF_SSA_TCPM_WRITE_TO_CONN]
        = icf_ssa_tcpm_04_write_to_conn;

    p_ssa_pdb->p_ssa_glb_data->ssa_tcpm_fsm_table
        [ICF_SSA_TCPM_NOT_INUSE][ICF_SSA_TCPM_REPLACE_CONN_FD]
        = icf_ssa_tcpm_xx_replace_conn_fd;

    p_ssa_pdb->p_ssa_glb_data->ssa_tcpm_fsm_table
        [ICF_SSA_TCPM_NOT_INUSE][ICF_SSA_TCPM_SUCC_OPEN_CONN_RESP]
        = icf_ssa_tcpm_ret_fail;

    p_ssa_pdb->p_ssa_glb_data->ssa_tcpm_fsm_table
        [ICF_SSA_TCPM_NOT_INUSE][ICF_SSA_TCPM_FAIL_OPEN_CONN_RESP]
        = icf_ssa_tcpm_ret_fail;

    p_ssa_pdb->p_ssa_glb_data->ssa_tcpm_fsm_table
        [ICF_SSA_TCPM_NOT_INUSE][ICF_SSA_TCPM_PURGE_TIMER_XPRY]
        = icf_ssa_tcpm_04_purge_timer_xpry;

    p_ssa_pdb->p_ssa_glb_data->ssa_tcpm_fsm_table
        [ICF_SSA_TCPM_NOT_INUSE][ICF_SSA_TCPM_CLOSE_CONN_IND]
        = icf_ssa_tcpm_01_close_ind;

    p_ssa_pdb->p_ssa_glb_data->ssa_tcpm_fsm_table
        [ICF_SSA_TCPM_NOT_INUSE][ICF_SSA_TCPM_OPEN_CONN_IND]
        = icf_ssa_tcpm_ret_fail;

    p_ssa_pdb->p_ssa_glb_data->ssa_tcpm_fsm_table
        [ICF_SSA_TCPM_NOT_INUSE][ICF_SSA_TCPM_DATA_IND]
        = icf_ssa_tcpm_04_data_ind;

    ICF_FUNCTION_EXIT(p_ssa_pdb->p_glb_pdb)
    return ret_val;
}
#ifdef ICF_IPSEC_ENABLED
/*****************************************************************************
 ** FUNCTION:      icf_ssa_sa_open_udp_channel
 **
 ** DESCRIPTION: This function is invoked to open a new UDP server for the 
 **                 protected port once the SA has been established
 *****************************************************************************/
icf_return_t icf_ssa_sa_open_udp_channel(
        INOUT        icf_uint16_t            port_num,
        INOUT        icf_transport_mode_t    trans_mode,
        INOUT      icf_uint8_t          *p_transp_addr_str,
        INOUT   icf_uint32_t         *pFd_id,
        INOUT    icf_error_t            *p_ecode)
{
    /* local variables */
    icf_return_t        ret_val = ICF_SUCCESS;
    ICF_PRINT(("\n:SSA: entered function: icf_ssa_sa_open_udp_channel"));

/* To remove warnings */
    port_num=port_num;
    trans_mode=trans_mode;
    p_transp_addr_str=p_transp_addr_str;
    pFd_id=pFd_id;
    p_ecode=p_ecode;
#ifdef ICF_PORT_SYMBIAN
    icf_port_open_channel(port_num, 
                    trans_mode,
                    ICF_TRUE,
                    p_transp_addr_str,
                    pFd_id,
                    p_ecode);
#endif

#ifdef ICF_PORT_WINDOWS

#ifdef ICF_IPV6_ENABLED
    if (icf_port_get_family_type(
        p_transp_addr_str,
        0,
        &dType) == ICF_FAILURE)
    {
        ret_val = ICF_FAILURE;
        ICF_PRINT(("\n Failure in getting family Type"));
    }
    else
    {
#endif
        if (ICF_FAILURE == icf_port_open_socket (
#ifdef ICF_IPV6_ENABLED
            dType,
#endif
            (icf_uint8_t)ICF_TRANSPORT_TYPE_UDP,
            pFd_id,
            p_ecode))
        {
            ret_val = ICF_FAILURE;
            ICF_PRINT(("\n Failure in opening UDP conn socket"));
        }
        else /* now bind this socket to the prot_us */
        {
            ICF_PRINT(("\nSSA: Socket opened with FD: %d",pFd_id));
            if (ICF_FAILURE == icf_port_bind(
                (icf_uint16_t)(*pFd_id),
                p_transp_addr_str,
                port_num,
                p_ecode))
            {
                ret_val = ICF_FAILURE;
                ICF_PRINT(("\n Failure in binding UDP socket\
                    to prot_us"));
            }
            ICF_PRINT(("\nSSA: Bind Successful Port: %d", port_num));
       }
#ifdef ICF_IPV6_ENABLED
    }
#endif
#endif
    return ret_val;
}
#endif


