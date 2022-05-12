
/*****************************************************************************
 ** FUNCTION:
 **                     This file contains utility functions invoked by the SSA
 ** 			for DNS related functionality
 **
 *****************************************************************************
 **
 ** FILENAME:           icf_ssa_dns_util.c
 **
 ** DESCRIPTION:        This file contains utility functions invoked by the SSA
 **
 ** DATE                NAME            REFERENCE               REASON
 ** ----                ----            ---------               ------
 ** 12-Jul-2006     Aashish Suchdev                       INITIAL CREATION
 ** 23-JAN-2008     Anurag Khare       REl 8.0            Functions added for
 **                                                       Secondary dns server
 **                                                       Support
 ** 03-Apr-2008     Tarun Gupta        SPR 18355          Merged Windows Porting Changes
 ** 08-Apr-2008     Alok Tiwari        SPR 18363      klocwork warning removal
 ** 01-May-2008     Amit Sharma        SPR 18390  Fix for IPTK crash in TLS case
 **                                               when Pri-server is not responding
 ** 11-May-2008 Abhishek Dhammawat   SPR 18535    modified
 **                                              icf_ssa_resend_on_sec_address 
 ** 17-May-2008     Amit Sharma        SPR 18577     Assigning APP ctxt to glb_pdb
 ** 24-Jun-2008     Anurag Khare       SPR 18716     IF the mapping is found in ssa_ctx
 **                                                  in icf_ssa_dns_get_ip_frm_fqdn then 
 **                                                  also it is returning failure
 ** 05-Jul-2008     Anuradha Gupta     SPR 18773     In case of DNS failure, send generic
 **                                                  call clear to internal modules.
 ** 07-Jul-2008     Amit Sharma        SPR 18772     Initialize dns_buffer_timer_id
 ** 04-Nov-2008     Tarun Gupta        SPR 19189     Merged Rel 7.1.2 SPR 18880
 ** 04-Nov-2008 Abhishek Dhammawat     SPR 19189     Merged the SPR 18846
 ** 12-Nov-2008     Tarun Gupta        SPR 19189     Merged Rel 7.1.2 SPR 18872
 ** 17-Nov-2008     Tarun Gupta        SPR 19189     Merged Rel 7.1.2 SPR 19007
 ** 25-Nov-2008     Rajiv Kumar        SPR 19215     Merged CSR-1-6259119
 ** 08-Dec-2008 Abhishek Dhammawat     SPR 19223     Klocworks warning removal 
 ** 08-Dec-2008     Rajiv Kumar        SPR 19218     Merged CSR 1-6431768
 ** 10-Dec-2008     Anurag Khare       SPR 19218     CSR 1-5613546 Merged
 ** 02-Mar-2009     Alok Tiwari      IPTK Rel8.1 IPTK open source replacement:
 **                                              Data related to ares has
 **                                              been removed.    
 ** 04-Mar-2009 Saurabh Gupta         SPR 19349      Klocworks fix
 ** 13-Mar-2009 Rajiv Kumar           IPTK Rel8.1    Merging CSR-1-6452321
 ** 17-Mar-2009 Rajiv Kumar           IPTK Rel8.1    UT Defect fix 
 ** 18-Mar-2009 Rajiv Kumar           IPTK Rel8.1    UT Defect fix 
 ** 18-Mar-2009 Rajiv Kumar           IPTK Rel8.1    UT Defect fix
 ** 1-Apr-2009  Abhishek Dhammawat    Rel 8.2        REFER/NOTIFY Enhancements
 ** 03-Apr-2009 Kamal Ashraf          Rel 8.2        Klocwork Fix
 ** 06-Apr-2009 Tarun Gupta           Rel 8.2        REFER/NOTIFY Enh UT Defect Fix
 ** 22-Apr-2009 Anuradha Gupta        SPR 19477      Modified fn icf_ssa_dns_purge_timer_expire
 ** 22-Apr-2009 Anuradha Gupta        SPR 19468      Modified fn icf_ssa_post_dns_resp_processing
 ** 25-Apr-2009 Abhishek Dhammawat    SPR 19503      Modified fn
 **                                                  icf_ssa_dns_add_fqdn_to_ip_mapping
 ** 01-May-2009 Anuradha Gupta        SPR 19535      Modified fn icf_ssa_process_dns_fail_resp
 ** 25-May-2009 Rajiv Kumar           SPR 19666      Fix For SPR: 19666
 ** 27-May-2009 Anuradha Gupta        SPR 19672      CSR 1-6669377 Merge
 ** 11-Jun-2009 Ashutosh Mohan        SPR 19737      Changes done for GCC 4.1 warnings removal
 ** 27-Jul-2009 Anuradha Gupta        SPR 19776      Modified fn icf_ssa_create_next_transaction
 ** 02-Sep-2009 Rajiv Kumar           SPR 19999      Merged CSR 1-7570808 
 ** 10-Oct-2009 Preksha               SPR 20024      Modified fn icf_ssa_process_dns_fail_resp
 ** 27-Oct-2009 Rajiv Kumar           SPR 20192      Fix For SPR:20192
 ** 28-May-2010 Alok Tiwari           SPR 20412      CSR 1-8564918 merged.
 ** 29-May-2010 Alok Tiwari           SPR 20412      CSR 1-8223807 merged.
 **
 *****************************************************************************
 **                 Copyrights 2009, Aricent .
 *****************************************************************************/

/*Message body header changes Start*/

#define SIP_MIME_PARSING
/*Message body header changes Ends*/


#include "icf_feature_flags.h"

#include "stack_headers.h"
#include "uatk_headers.h"

#include "icf_internal_api_id.h"
#include "icf_common_types.h"
#include "icf_common_defs.h"


ICF_SESSION_CONTROL_START
#include "icf_common_prototypes.h"
ICF_SESSION_CONTROL_END

#include "icf_port_prototypes.h"
#include "icf_macro.h"


#include "icf_ssa_prototypes.h"
#include "icf_ssa_macro.h"
#include "icf_common_prototypes.h"
#include "icf_dbm_prototypes.h"
#include "icf_regm_types.h"
#include "icf_dbm_regm_prototypes.h"

#include "icf_ssa_common_types.h"

#include "icf_ecode.h"
#include "icf_cfg_defs.h"


#ifdef ICF_DNS_LOOKUP_ENABLED
#include "sdf_lookup.h"
#include "sdf_lookupport.h"
#endif
#ifdef ICF_PORT_WINDOWS
#pragma warning(disable:4701)
#pragma warning ( disable : 4702 )
#endif

/* Global defintion of persistent global pdb */
extern icf_glb_pdb_st        *p_persistent_glb_pdb;

#ifdef ICF_DNS_LOOKUP_ENABLED
/*******************************************************************************
 * FUNCTION:        icf_ssa_process_dns_fail_resp
 *
 * DESCRIPTION:     This function handles the DNS failure response from NW
 *                  Argument dns_resp is added to decide whether it is a failure
 *                  in DNS response and retry is required on secondary DNS 
 *                  server address or DNS response was success but failure in 
 *                  some other handling in calling function to send failure
 *                  to handler modules like CC, CFG or SIC
 ******************************************************************************/
icf_return_t	icf_ssa_process_dns_fail_resp(
        icf_ssa_pdb_st         *p_ssa_pdb,
        Sdf_st_dnsEventContext *p_dns_evt_ctxt,
        icf_boolean_t          dns_resp,
        icf_boolean_t		   dns_map_change_ind_sent)
{
	icf_return_t	     ret_val = ICF_SUCCESS;
	/*this flag will be used to judge whether we need to invoke
	FreeCallObject at the end of this routine or not. This will
	be set on a per case basis depending on the app module's
	behavior*/
	icf_boolean_t	     ivk_free_call_obj = ICF_FALSE;
	icf_internal_msg_st  *p_internal_msg = ICF_NULL;
    Sdf_st_error         Error = {0,0,0,"\0"};
    icf_boolean_t        dns_failure = ICF_FALSE;
    Sdf_ty_s8bit         method[ICF_MAX_SIZE_INT_USAGE_STR] = {'\0'}; 
    icf_uint8_t          fqdn_str[ICF_MAX_STR_LEN] = {'\0'};


    if((ICF_NULL ==  p_ssa_pdb) || (ICF_NULL == p_ssa_pdb->p_glb_pdb))
    {
         ICF_PRINT(((icf_uint8_t *)"\n[SSA]: SSA PDB or GLB PDB is NULL "))
         return ICF_FAILURE;
    }

	ICF_FUNCTION_ENTER(p_ssa_pdb->p_glb_pdb)

    p_internal_msg = p_ssa_pdb->p_internal_msg;    

    if((ICF_NULL ==  p_ssa_pdb->p_ssa_ctxt) || (ICF_NULL == p_dns_evt_ctxt))
    {
         ICF_PRINT(((icf_uint8_t *)"\n[SSA]: DNS Event context or SSA context is NULL "))
         ICF_FUNCTION_EXIT(p_ssa_pdb->p_glb_pdb)    
         return ICF_FAILURE;
    }

	/* SPR 19027: Initialize the dest module id in SSA pdb to Invalid module */
	p_ssa_pdb->dest_module_id = ICF_INT_MODULE_INVALID;    

    /*If DNS response is failure then try on secondary DNS server address
     * otherwise this function is called from
     * icf_ssa_post_dns_resp_processing for failure handling in other scenarios
     * after DNS success response*/
    if (ICF_FALSE == dns_resp)
    {
#ifdef ICF_TRACE_ENABLE                  
      if(p_dns_evt_ctxt->dDnsRecordType == Sdf_en_SRVrecord)
      {
          p_ssa_pdb->dns_query_type = ICF_DNS_SRV_QUERY;
      }
      else if(p_dns_evt_ctxt->dDnsRecordType == Sdf_en_IPv4Address)
      {
          p_ssa_pdb->dns_query_type = ICF_DNS_AAA_QUERY;
      }
      ICF_SSA_APP_TRACE(p_ssa_pdb,ICF_TRACE_DNS_QUERY_FAILURE)
#endif

      /* if both DNS Servers have been tried */
      if (ICF_SSA_MAX_DNS_SERVERS_TRIED == 
              p_ssa_pdb->p_ssa_ctxt->dns_servers_tried)
      {
          /* SPR 19189 */
          ICF_CHECK_IF_VAR_IS_NULL(p_ssa_pdb->p_glb_pdb,
                                   p_ssa_pdb->p_glb_cfg_data,
                                   ret_val)
          if(ICF_FAILURE == ret_val)
          {
              ICF_FUNCTION_EXIT(p_ssa_pdb->p_glb_pdb)
              return ret_val;    
          }

          dns_failure = ICF_TRUE;
          /* mark secondary server inactive */
          if (ICF_TRUE == p_ssa_pdb->p_glb_cfg_data->sec_dns_server_active)
          {
              p_ssa_pdb->p_glb_cfg_data->sec_dns_server_active = ICF_FALSE;
              ICF_SSA_APP_TRACE(p_ssa_pdb,
                  ICF_TRACE_SECONDARY_DNS_SERVER_NOT_RESPONDING)
          }
          else
          {
              ICF_SSA_APP_TRACE(p_ssa_pdb,
                  ICF_TRACE_PRIMARY_DNS_SERVER_NOT_RESPONDING)
          }
          if (ICF_NULL != p_ssa_pdb->p_ssa_ctxt)
          {
               /*If DNS server failure response is received for primary
                *Proxy/Registrar then now try for secondary proxy/registrar
                *This is the case when primary and secondary DNS server address
                *are configured and both returned failure for domain name*/
               if (ICF_SUCCESS == icf_ssa_resend_on_sec_address(
                                p_ssa_pdb->p_ssa_ctxt->p_call_obj,
                                p_dns_evt_ctxt->pOverlappedTransaction, 
                                p_ssa_pdb->p_ssa_ctxt, 
                                &Error))
                   {
                       return ICF_SUCCESS;
                   }
          }
          *(p_ssa_pdb->p_ecode) = 
                  ICF_CAUSE_DNS_SERVER_NOT_RESPONDING;
      }/* end if - both DNS Servers have been tried */
      else /* request is to be sent to secondary DNS Server 
                * (if configured) */
      {
          /* SPR 19189 */
          ICF_CHECK_IF_VAR_IS_NULL(p_ssa_pdb->p_glb_pdb,
                                   p_ssa_pdb->p_glb_cfg_data,
                                   ret_val)
          if(ICF_FAILURE == ret_val)
          {
              ICF_FUNCTION_EXIT(p_ssa_pdb->p_glb_pdb)
              return ret_val;    
          }

          if (ICF_TRUE == p_ssa_pdb->p_glb_cfg_data->sec_dns_server_active)
          {
              ICF_SSA_APP_TRACE(p_ssa_pdb,
                 ICF_TRACE_SECONDARY_DNS_SERVER_NOT_RESPONDING)
          }
          else
          {
              ICF_SSA_APP_TRACE(p_ssa_pdb,
                  ICF_TRACE_PRIMARY_DNS_SERVER_NOT_RESPONDING)
          }
          /* if secondary DNS Server is configured */
          if (ICF_NULL != p_ssa_pdb->p_glb_cfg_data->
                  p_sec_dns_server_addr)
          {
              /* increment dns_servers_tried to 2 */
              p_ssa_pdb->p_ssa_ctxt->dns_servers_tried++;
              if (ICF_TRUE == p_ssa_pdb->p_glb_cfg_data->sec_dns_server_active)
              {
                  /*Toggle DNS server active flag*/
                  p_ssa_pdb->p_glb_cfg_data->sec_dns_server_active = 
                      ICF_FALSE;
              }
              else
              {
                   /* Mark secondary DNS Server active */
                  p_ssa_pdb->p_glb_cfg_data->sec_dns_server_active = 
                      ICF_TRUE;
                  /* generate trace-secondary DNS Server is active */
                  ICF_SSA_APP_TRACE(p_ssa_pdb,
                      ICF_TRACE_SECONDARY_DNS_SERVER_ACTIVE)
              }

              if (ICF_NULL != p_dns_evt_ctxt->pOverlappedTransaction)
              {
                  p_dns_evt_ctxt->pOverlappedTransaction->dTxnDnsState = 
                      Sdf_en_dnsQuery;
              }
              else
              {
                  p_dns_evt_ctxt->pTransaction->dTxnDnsState = 
                      Sdf_en_dnsQuery;
              }

              /* send DNS query */
              if(Sdf_co_success == 
                      sdf_fn_uaDnsQuery((icf_int8_t*)p_ssa_pdb->p_ssa_ctxt->fqdn.str,
                          p_dns_evt_ctxt,
                          &Error))
              { 
                  dns_failure = ICF_FALSE;
              }
              /* Reset ssa_pdb.p_ssa_ctxt->dns_retrans_timer 
               * for the DNS request to secondary DNS Server */
              p_ssa_pdb->p_ssa_ctxt->dns_retrans_timer = 0;
          }/* end if - secondary DNS Server configured */
          else /* secondary DNS Server not configured */
          {
              if (ICF_NULL != p_ssa_pdb->p_ssa_ctxt)
              {
                  icf_port_strcpy((icf_uint8_t*)fqdn_str, (icf_uint8_t*)p_ssa_pdb->p_ssa_ctxt->fqdn.str);
                  /*If DNS server failure response is received for primary
                    *Proxy/Registrar then now try for secondary proxy/registrar
                    *This is the case when only primary DNS server address
                    *is configured and it returned failure for domain name*/
                   if (ICF_SUCCESS == icf_ssa_resend_on_sec_address(
                                p_ssa_pdb->p_ssa_ctxt->p_call_obj,
                                p_dns_evt_ctxt->pOverlappedTransaction, 
                                p_ssa_pdb->p_ssa_ctxt, 
                                &Error))
                   {
                       return ICF_SUCCESS;
                   }
               }
              dns_failure = ICF_TRUE;
              /* return ERROR cause */
              *(p_ssa_pdb->p_ecode) = 
                  ICF_CAUSE_DNS_SERVER_NOT_RESPONDING;
          }/* end else - secondary DNS server not configured */
      }/* end else -request to be sent through sec. DNS server */
    }
    else
    {
        /*Even if this is not DNS failure, but this flag is set to ICF_TRUE 
         * to send message to other handler modules*/
        dns_failure = ICF_TRUE;
    }

    if (ICF_TRUE == dns_failure)
    {
        /* SPR 18872: IN case of failure/no response for DNS query, reset the
         * ongoing query bitmask
         */       
        if( p_ssa_pdb->p_ssa_ctxt && (ICF_SSA_DNS_QUERY_ONGOING & p_ssa_pdb->p_ssa_ctxt->bitmask_1))
        {
          p_ssa_pdb->p_ssa_ctxt->bitmask_1 &= ~ICF_SSA_DNS_QUERY_ONGOING;
          if(ICF_NULL != p_ssa_pdb->p_ssa_ctxt->internal_msg_id)
          {
              icf_ssa_free_buffered_msg_for_dns(p_ssa_pdb);
          }
        }
        /* SPR 19027: Send DNS mapping change indication to App */
        if (ICF_FALSE == dns_map_change_ind_sent)
        {
            if (ICF_FAILURE == icf_ssa_send_dns_map_change_ind(
                        p_persistent_glb_pdb,
                        fqdn_str,
                        ICF_NULL,
                        (icf_uint8_t)ICF_DNS_MAP_CHANGE_QUERY_INVOCATION_FAILURE))
            {
                ICF_PRINT(((icf_uint8_t*)"\n[SSA]: icf_ssa_send_dns_map_change_ind failed\n"));
                ret_val = ICF_FAILURE;
            }
        }

        if (ICF_FAILURE == ret_val)
        {
            /* no-op */
            ret_val = ICF_FAILURE;
        }
        else if (ICF_NULL != p_ssa_pdb->p_call_ctxt)
        {
	        p_ssa_pdb->p_glb_pdb->p_call_ctx = 
				p_ssa_pdb->p_call_ctxt;

			/*Fill internal message for other module*/
			p_internal_msg->p_msg_data = ICF_NULL;/*no payload*/
			/*ICF_GENERIC_CALL_CLEAR;*/

			p_internal_msg->msg_hdr.payload_length = 0;
			p_internal_msg->msg_hdr.p_glb_pdb = p_ssa_pdb->p_glb_pdb;

            /* SPR 18773: In case of DNS failure always send generic call clear towards
             * internal modules so that all the interfaces are gracefully
             * cleared , on forceful call clear , no attempt will be made to
             * to clear the diffrent interfaces of IPTK
             * if once generic call clear is sent and still DNS query is
             * attempted then send forceful call clear to CC
             */

            if(ICF_CALL_CLEAR_REASON_DNS_SERVER_NOT_RESPONDING !=
                    p_ssa_pdb->p_call_ctxt->release_cause)
            {
                p_internal_msg->msg_hdr.msg_id = 
					ICF_GENERIC_CALL_CLEAR;
                p_ssa_pdb->p_call_ctxt->release_cause = 
                     ICF_CALL_CLEAR_REASON_DNS_SERVER_NOT_RESPONDING;
            }
            else
            {
                p_internal_msg->msg_hdr.msg_id = ICF_FORCEFUL_CALL_CLEAR_IND;
            }

			p_ssa_pdb->dest_module_id = ICF_INT_MODULE_CM;
#ifdef ICF_ERROR_ENABLE
            icf_port_strcpy((icf_uint8_t*)method, (icf_uint8_t*)"INVITE");
#endif
        }
    	else if (ICF_NULL != p_ssa_pdb->p_rgm_context)
	    {
   	        icf_rgm_nw_resp_st               *p_rgm_msg = ICF_NULL;
			/* Populate the internal message header */
		    p_internal_msg->msg_hdr.msg_id = ICF_NW_INC_REG_RESP;
            p_internal_msg->msg_hdr.payload_length = 
		    	ICF_PORT_SIZEOF(icf_rgm_nw_resp_st);

        	/* Populate the internal message payload */
		    p_rgm_msg = (icf_rgm_nw_resp_st *)p_internal_msg->p_msg_data;

            /* SPR 19189 */
            ICF_CHECK_IF_VAR_IS_NULL(p_ssa_pdb->p_glb_pdb,
                                     p_rgm_msg,
                                     ret_val)
            if(ICF_FAILURE == ret_val)
            {
                ICF_FUNCTION_EXIT(p_ssa_pdb->p_glb_pdb)
                return ret_val;    
            }

        	p_rgm_msg->p_rgm_context = p_ssa_pdb->p_rgm_context;
		    ICF_ASSERT(p_ssa_pdb->p_glb_pdb, (ICF_NULL != p_rgm_msg->p_rgm_context))

	        p_rgm_msg->status = ICF_RGM_SSA_NW_FAILURE;

            if(ICF_NULL != p_ssa_pdb->p_ecode)
            {
                p_rgm_msg->ecode = *(p_ssa_pdb->p_ecode);
            }

            /* SPR 19535: Set the response code as 0 so that it does not result into 
             * UMR while being accessed at REGM module
             */
            p_rgm_msg->response_code = 0;

		    p_internal_msg->msg_hdr.p_glb_pdb = p_ssa_pdb->p_glb_pdb;

        	p_ssa_pdb->dest_module_id = ICF_INT_MODULE_CFG;
#ifdef ICF_ERROR_ENABLE
            icf_port_strcpy((icf_uint8_t*)method, (icf_uint8_t*)"REGISTER");
#endif
	    }
	    else if (ICF_NULL != p_ssa_pdb->p_sic_ctxt)
	    {
		    en_SipMessageType	msg_type = SipMessageAny;

            icf_port_strcpy((icf_uint8_t *)method,(icf_uint8_t *)"");
		    if ( ICF_FAILURE == icf_ssa_get_msg_mthd_type_frm_txn(p_ssa_pdb, 
				        p_dns_evt_ctxt->pTransaction,
        				p_dns_evt_ctxt->pOverlappedTransaction,
		        		&msg_type, method))
		    {
			    ret_val = ICF_FAILURE;
		    }
		    else
		    {
			    if(icf_port_strcmp((icf_uint8_t *)method,(icf_uint8_t *)"REFER" ) == 0)
			    {
			        icf_nw_inc_refer_resp_st * p_refer_resp = ICF_NULL;
			
			       	/* Populate the internal message header */
        			p_internal_msg->msg_hdr.msg_id = ICF_SIC_REFER_STATUS;
		        	p_internal_msg->msg_hdr.payload_length = 
				        ICF_PORT_SIZEOF(icf_nw_inc_refer_resp_st);

        			/* Populate the internal message payload */
		        	p_refer_resp = (icf_nw_inc_refer_resp_st *)p_internal_msg->p_msg_data;
                    
                    /* SPR 19189 */
                    ICF_CHECK_IF_VAR_IS_NULL(p_ssa_pdb->p_glb_pdb,
                                             p_refer_resp,
                                             ret_val)
                    if(ICF_FAILURE == ret_val)
                    {
                        ICF_FUNCTION_EXIT(p_ssa_pdb->p_glb_pdb)
                        return ret_val;    
                    }

				    p_refer_resp->response = 0;
        			p_refer_resp->p_sic_ctx = p_ssa_pdb->p_sic_ctxt;
                    if (ICF_NULL != p_refer_resp->p_sic_ctx)
                    {
                        /* SPR 19189 */
                        ICF_CHECK_IF_ANY_OF_TWO_VAR_IS_NULL(p_ssa_pdb->p_glb_pdb,
                                                            p_ssa_pdb->p_ssa_ctxt,
                                                            p_ssa_pdb->p_ssa_ctxt->p_sic_ctxt,
                                                            ret_val)
                        if(ICF_FAILURE == ret_val)
                        {
                            ICF_FUNCTION_EXIT(p_ssa_pdb->p_glb_pdb)
                            return ret_val;    
                        }
                        if(ICF_NULL != p_ssa_pdb->p_ecode)
                        {
                            p_refer_resp->p_sic_ctx->error_code = *(p_ssa_pdb->p_ecode);
                        }
                        if (ICF_SIC_REQUEST_WITHIN_DIALOG !=
                                (p_ssa_pdb->p_ssa_ctxt->p_sic_ctxt->bitmask &
                                 ICF_SIC_REQUEST_WITHIN_DIALOG))
                        {
                            ivk_free_call_obj = ICF_TRUE;
                        }
                        ICF_ASSERT(p_ssa_pdb->p_glb_pdb, (ICF_NULL != p_refer_resp->p_sic_ctx))
                    }
			    }
			    else if(icf_port_strcmp((icf_uint8_t *)method,(icf_uint8_t *)"SUBSCRIBE") == 0)
			    {
				    icf_nw_inc_subs_resp_st * p_subs_resp = ICF_NULL;

        			if (ICF_NULL == p_ssa_pdb->p_sic_ctxt)
		        	{

				    	/* In this case application has delinked itself
        				 * and hence it should ignore this response 
		        		 * and return SUCCESS */
				       	ICF_PRINT(((icf_uint8_t *)"\n[SSA]: Application delinked: Txn Timeout for SUBSCRIBE ignored "));
				    }
        			else
		        	{
				       	/* Populate the internal message header */
        				p_internal_msg->msg_hdr.msg_id = ICF_NW_INC_SUBSCRIBE_RESP;
        				p_internal_msg->msg_hdr.payload_length = 
		        			ICF_PORT_SIZEOF(icf_nw_inc_subs_resp_st);

				       	/* Populate the internal message payload */
        				p_subs_resp = (icf_nw_inc_subs_resp_st *)p_internal_msg->p_msg_data;

                        /* SPR 19189 */
                        ICF_CHECK_IF_VAR_IS_NULL(p_ssa_pdb->p_glb_pdb,
                                                 p_subs_resp,
                                                 ret_val)
                        if(ICF_FAILURE == ret_val)
                        {
                            ICF_FUNCTION_EXIT(p_ssa_pdb->p_glb_pdb)
                            return ret_val;    
                        }

		        		p_subs_resp->bitmask = 0;
				       	p_subs_resp->response = ICF_NW_INC_SUBS_RESP_NO_RETRY;
        				p_subs_resp->resp_code=0;
		        		p_subs_resp->p_sic_ctx = p_ssa_pdb->p_sic_ctxt;
                        if(ICF_NULL != p_ssa_pdb->p_ecode)
                        {
                            p_subs_resp->p_sic_ctx->error_code = *(p_ssa_pdb->p_ecode);
                        }
				       	ICF_ASSERT(p_ssa_pdb->p_glb_pdb, (ICF_NULL != p_subs_resp->p_sic_ctx))
        			}
		       	}
			    else if( icf_port_strcmp((icf_uint8_t *)method,(icf_uint8_t *)"MESSAGE" ) == 0 )
			    {
        			icf_nw_inc_msg_resp_st * p_msg_resp = ICF_NULL;

		       		/* Populate the internal message header */
			        p_internal_msg->msg_hdr.msg_id = ICF_SIC_MESSAGE_STATUS;
        			p_internal_msg->msg_hdr.payload_length = 
		        			ICF_PORT_SIZEOF(icf_nw_inc_msg_resp_st);

			        /* Populate the internal message payload */
        			p_msg_resp = (icf_nw_inc_msg_resp_st *)p_internal_msg->p_msg_data;
                    
                    /* SPR 19189 */
                    ICF_CHECK_IF_VAR_IS_NULL(p_ssa_pdb->p_glb_pdb,
                                             p_msg_resp,
                                             ret_val)
                    if(ICF_FAILURE == ret_val)
                    {
                        ICF_FUNCTION_EXIT(p_ssa_pdb->p_glb_pdb)
                        return ret_val;    
                    }
		       		p_msg_resp->response = 0;
			        p_msg_resp->p_sic_ctx = p_ssa_pdb->p_sic_ctxt;
                    /* Fix for SPR 20412: CSR 1-8564918 merged*/ 
                   /* Fix for CSR 1-8564918 : SPR-20395*/
                   /* No need to set the boolean flag as this will free the call object
                    * In case of MESSAGE ,call object should be freed through
                    * the function icf_ssa_process_delink_req.
                    */
                    /*ivk_free_call_obj = ICF_TRUE;*/
                    if (ICF_NULL != p_ssa_pdb->p_ecode)
                    {
                        p_msg_resp->p_sic_ctx->error_code = *(p_ssa_pdb->p_ecode);
                    }
                    ICF_ASSERT(p_ssa_pdb->p_glb_pdb, (ICF_NULL != p_msg_resp->p_sic_ctx))
        	    }
		        else if(icf_port_strcmp((icf_uint8_t *)method,(icf_uint8_t *)"OPTIONS" ) == 0)
			    {   
        			icf_nw_inc_opt_resp_st * p_opt_resp = ICF_NULL;
				    /* Populate the internal message header */
				    p_internal_msg->msg_hdr.msg_id = ICF_SIC_OPTION_STATUS;
        			p_internal_msg->msg_hdr.payload_length = 
		        		ICF_PORT_SIZEOF(icf_nw_inc_opt_resp_st);

				    /* Populate the internal message payload */
        			p_opt_resp = (icf_nw_inc_opt_resp_st *)p_internal_msg->p_msg_data;

                    /* SPR 19189 */
                    ICF_CHECK_IF_VAR_IS_NULL(p_ssa_pdb->p_glb_pdb,
                                             p_opt_resp,
                                             ret_val)
                    if(ICF_FAILURE == ret_val)
                    {
                        ICF_FUNCTION_EXIT(p_ssa_pdb->p_glb_pdb)
                        return ret_val;    
                    }

		        	p_opt_resp->response = 0;
				    p_opt_resp->p_sic_ctx = p_ssa_pdb->p_sic_ctxt;
                    if (ICF_NULL != p_ssa_pdb->p_ecode)
                    {
                        p_opt_resp->p_sic_ctx->error_code = *(p_ssa_pdb->p_ecode);
                    }
	        		ICF_ASSERT(p_ssa_pdb->p_glb_pdb, (ICF_NULL != p_opt_resp->p_sic_ctx))
                    /* Fix for SPR 20412: CSR 1-8564918 merged*/ 
                    /* Fix for CSR 1-8564918 : SPR-20395*/
                    /* Set the correct error cause */
                    /* No need to set the boolean flag as this will free the call object
                     * In case of OPTIONS, call object should be freed through
                     * the function icf_ssa_process_delink_req.
                     */
                    /*ivk_free_call_obj = ICF_TRUE;*/
			    }
        		else if( icf_port_strcmp((icf_uint8_t *)method, (icf_uint8_t *)"PUBLISH") == 0)
		        {
				    icf_nw_inc_publish_resp_st * p_publish_resp = ICF_NULL;
				
        		    /* Populate the internal message header */
        			p_internal_msg->msg_hdr.msg_id = ICF_SIC_PUBLISH_STATUS;
        			p_internal_msg->msg_hdr.payload_length = 
		        	ICF_PORT_SIZEOF(icf_nw_inc_publish_resp_st);

				    /* Populate the internal message payload */
        			p_publish_resp = (icf_nw_inc_publish_resp_st *)p_internal_msg->p_msg_data;

                    /* SPR 19189 */
                    ICF_CHECK_IF_VAR_IS_NULL(p_ssa_pdb->p_glb_pdb,
                                             p_publish_resp,
                                             ret_val)
                    if(ICF_FAILURE == ret_val)
                    {
                        ICF_FUNCTION_EXIT(p_ssa_pdb->p_glb_pdb)
                        return ret_val;    
                    }

		        	p_publish_resp->response = 0;
				    p_publish_resp->duration = 0;
        			p_publish_resp->p_sic_ctx = p_ssa_pdb->p_sic_ctxt;
                    /* Fix for SPR 20412: CSR 1-8564918 merged*/ 
                    /* Fix for CSR 1-8564918 : SPR-20395*/
                    /* No need to set the boolean flag as this will free the call object
                     * In case of PUBLISH ,call object should be freed through
                     * the function icf_ssa_process_delink_req.
                     */
                    /*ivk_free_call_obj = ICF_TRUE;*/
                    if (ICF_NULL != p_ssa_pdb->p_ecode)
                    {
                        p_publish_resp->p_sic_ctx->error_code = *(p_ssa_pdb->p_ecode);
                    }
                    ICF_ASSERT(p_glb_pdb, (ICF_NULL != p_publish_resp->p_sic_ctx))
                }    
			    else
                {
		            ret_val = ICF_FAILURE;
				    ICF_PRINT(((icf_uint8_t *)"\n[SSA]:Failed to Identify ctxt for FailedDNSQuery"));
                }
        		p_internal_msg->msg_hdr.p_glb_pdb = p_ssa_pdb->p_glb_pdb;
		    }
        	p_ssa_pdb->dest_module_id = ICF_INT_MODULE_SIC;
	    }
    }

    if (ICF_TRUE == dns_failure)
    {
#ifdef ICF_ERROR_ENABLE
        if ( (p_ssa_pdb->p_ecode) && (ICF_NULL != p_ssa_pdb->p_ssa_ctxt) 
           && (ICF_CAUSE_DNS_SERVER_NOT_RESPONDING == *(p_ssa_pdb->p_ecode)))
        {
            ICF_SSA_SET_ECODE(p_ssa_pdb, method,ICF_ERROR_DNS_QUERY_FAILURE)
            *(p_ssa_pdb->p_ecode) = ICF_CAUSE_DNS_SERVER_NOT_RESPONDING;
        }
#endif
        if ( ICF_TRUE == ivk_free_call_obj)
        {
            sdf_ivk_uaFreeCallObject(&(p_dns_evt_ctxt->pCallObj));
        }
        if ( ICF_SUCCESS == ret_val)
        {
            ret_val = icf_ssa_process_otg_intrnl_msg(p_ssa_pdb);
        }
    }

	ICF_FUNCTION_EXIT(p_ssa_pdb->p_glb_pdb)
	return ret_val;
}

/*******************************************************************************
 * FUNCTION:        icf_ssa_post_dns_resp_processing
 *
 * DESCRIPTION:     This function is triggered after a DNS resposne has
 * received and transaction has been updated successfully.
 ******************************************************************************/
icf_void_t icf_ssa_post_dns_resp_processing(
	Sdf_st_dnsEventContext *p_dns_evt_ctxt, 
	Sdf_ty_bool            dns_resp_flag)
{
	icf_ssa_pdb_st          ssa_pdb;
	icf_return_t		    ret_val = ICF_SUCCESS;
	icf_internal_msg_st	    internal_msg = {{0,0,0,0,0,0},0};
	icf_ssa_glb_data_st     *p_ssa_glb_data = ICF_NULL;
	icf_uint32_t            count = 0;
	icf_ssa_ctxt_st		    *p_ssa_ctxt = ICF_NULL; 
	Sdf_st_error		    sdf_error = {0,0,0,"\0"};
	Sdf_st_eventContext	    *p_event_context = Sdf_co_null;
	icf_ssa_intrnl_intf_ut  internal_msg_payload;
    icf_error_t             ecode = ICF_ERROR_NONE;
    SipMessage              *p_sip_msg = Sdf_co_null;
    SipMessage              *p_msg = Sdf_co_null;
    Sdf_ty_s8bit            *p_method = Sdf_co_null;

    if(ICF_NULL == p_persistent_glb_pdb)
    {
        ICF_PRINT(((icf_uint8_t *)"\n[SSA]: Persistent Gld Pdb is NULL "))
        return;
    }

    if((ICF_NULL == p_dns_evt_ctxt) || (ICF_NULL == p_dns_evt_ctxt->pCallObj) ||
       (ICF_NULL == p_dns_evt_ctxt->pCallObj->pAppData))
    {
        ICF_PRINT(((icf_uint8_t *)"\n[SSA]: DNS event context or Call Object is NULL "))
        ICF_FUNCTION_EXIT(p_persistent_glb_pdb)
        return;
    }

    p_ssa_ctxt = (icf_ssa_ctxt_st*)(p_dns_evt_ctxt->pCallObj->pAppData->pData);
    if(ICF_NULL == p_ssa_ctxt)
    {
        ICF_PRINT(((icf_uint8_t *)"\n[SSA]: SSA Context is NULL "))
        ICF_FUNCTION_EXIT(p_persistent_glb_pdb)
        return;
    }

	ICF_FUNCTION_ENTER(p_persistent_glb_pdb)

    if(Sdf_co_null != p_dns_evt_ctxt->pOverlappedTransaction)
	{
		/*overlap transaction*/
		p_sip_msg = p_dns_evt_ctxt->pOverlappedTransaction->pSipMsg;
	}
	else if(Sdf_co_null != p_dns_evt_ctxt->pTransaction)/*either a uac or a Uas transaction*/
	{
		p_sip_msg = p_dns_evt_ctxt->pTransaction->pSipMsg;
	}
    else
    {
       ICF_PRINT(((icf_uint8_t *)"\n[SSA]:SIP message not available"));
       return;
    }

	internal_msg.msg_hdr.ecode =  ICF_ERROR_NONE;
	/*
	 * Populate SSA PDB
	 */
	icf_ssa_init_pdb(&ssa_pdb,p_persistent_glb_pdb);
	ssa_pdb.p_internal_msg      = &(internal_msg);
	ssa_pdb.p_ecode             = &(internal_msg.msg_hdr.ecode);
	ssa_pdb.p_conn_table_entry   = ICF_NULL;
	/*
	 * Now we init the void pointer which will be used to store the
	 * list of memory chunks that will be allocated in callbacks and will
	 * be freed on return path (after invocation of destination module-CM,
	 * CFG etc.)
	 */
	ssa_pdb.alloc_index = 0;
	ssa_pdb.p_ssa_ctxt = p_ssa_ctxt;

    /*Applocate APP context in p_glb_pdb*/
    icf_ssa_alloc_app_ctx_in_glb_pdb(&ssa_pdb);

    /*Store p_call_ctxt, p_rgm_ctxt and p_sic_ctxt in p_ssa_pdb
     *from p_ssa_ctxt, as these are required in function
     *icf_ssa_process_dns_fail_resp, to send response to CC/REGM/SIC
     *in case of failure and icf_ssa_ctxt_st is already freed, SSA context
     *can be freed when inavalid DNS response received for primary server address and
     *some failure occurs while trying on sec server address*/
	ssa_pdb.p_call_ctxt = p_ssa_ctxt->p_call_ctxt;
	ssa_pdb.p_rgm_context = p_ssa_ctxt->p_rgm_ctxt;
	ssa_pdb.p_sic_ctxt = p_ssa_ctxt->p_sic_ctxt;
        
    p_ssa_ctxt->p_ssa_pdb = &ssa_pdb;
    ssa_pdb.p_glb_pdb->p_call_ctx = p_ssa_ctxt->p_call_ctxt;
	for ( count = 0; count < ICF_SSA_MAX_MMRY_CHUNK_LST;
			count++)
	{
		ssa_pdb.allocated_memory[count].p_mem_chunk =
			ICF_NULL;
	}
	
	internal_msg.p_msg_data  = (icf_uint8_t*)(&internal_msg_payload);
	
	internal_msg.msg_hdr.payload_length  =
        			ICF_PORT_SIZEOF(icf_ssa_intrnl_intf_ut);

	/* get the ssa global data pointer from DBM */
	icf_dbm_get_module_glb_data(ssa_pdb.p_glb_pdb,ICF_INT_MODULE_SSA,
			(icf_void_t **)&p_ssa_glb_data,ssa_pdb.p_ecode);
	ssa_pdb.p_ssa_glb_data = p_ssa_glb_data;

    p_ssa_ctxt->bitmask_1 &= ~ICF_SSA_DNS_QUERY_ONGOING;
  	/* get the config data pointer from DBM */
	if (ICF_FAILURE == icf_dbm_get_module_glb_data(
				ssa_pdb.p_glb_pdb,
				(icf_int_module_id_t)ICF_INT_MODULE_CFG,
				(icf_void_t *)&(ssa_pdb.p_glb_cfg_data),
				ssa_pdb.p_ecode))
	{
		ICF_PRINT(((icf_uint8_t *)"\n[SSA]:Failure in GLB Cfg data retreival!!!"));
	}
	else if (ICF_FAILURE == icf_dbm_get_module_glb_data(
				ssa_pdb.p_glb_pdb,
				(icf_int_module_id_t)ICF_INT_MODULE_REGM,
				(icf_void_t *)&(ssa_pdb.p_glb_rgm_data),
				ssa_pdb.p_ecode)) 
	{
		ICF_PRINT(((icf_uint8_t *)"\n[SSA]:Failure in GLB Regm data retreival!!!"));
	}
	else
	{
		if ( Sdf_co_false != dns_resp_flag)
		{
                    /*  Merge for CSR 1-5613546 */
                    if ( ICF_SSA_APP_CTXT_TERMINATED ==
                                (ICF_SSA_APP_CTXT_TERMINATED & p_ssa_ctxt->bitmask))
                    {
                        if(Sdf_co_null == p_dns_evt_ctxt->pTransaction) 
                        {
                            sdf_ivk_uaAbortTransaction(p_ssa_ctxt->p_call_obj,
                            p_dns_evt_ctxt->pOverlappedTransaction->dTxnId,
                            (Sdf_ty_pvoid*)p_ssa_ctxt, &sdf_error);
                        }
                        else 
                        {
                            sdf_ivk_uaAbortTransaction(p_ssa_ctxt->p_call_obj,
                            p_dns_evt_ctxt->pTransaction->dTxnId,
                            (Sdf_ty_pvoid*)p_ssa_ctxt, &sdf_error);
                        }

                    }
                    else
                    {
			Sdf_st_transportInfo    *p_dest_info = Sdf_co_null;
			/*Now we retreive the IP address from the destinfo field of
			  the transaction and fill in ssa pdb*/
			if ( Sdf_co_null != p_dns_evt_ctxt->pOverlappedTransaction)
			{
				p_dest_info = 
					p_dns_evt_ctxt->pOverlappedTransaction->pDestInfo;
			}
			else if ( Sdf_co_null != p_dns_evt_ctxt->pTransaction)
			{
				p_dest_info = 
					p_dns_evt_ctxt->pTransaction->pDestInfo;
			}

            if ((Sdf_co_null == p_dest_info) || 
					(Sdf_co_null == p_dest_info->pIp))
			{
                ICF_PRINT(((icf_uint8_t *)"\n[SSA]:EmptyDestinationInfo!!returning"))
                return;
			}

            if (p_ssa_ctxt->p_dns_ip_addr != ICF_NULL)
            {
                SipOptions dOptions = {0};
                SipMessage	*p_sip_msg = ICF_NULL;
                
                /* Fix For SPR:20192*/
                icf_ssa_dns_delete_fqdn_entry(&p_ssa_ctxt->fqdn,
                        &p_ssa_ctxt->naptr_fqdn,&p_ssa_ctxt->srv_fqdn,
                        &p_ssa_ctxt->queryA_fqdn,
                        &ecode,
                        (icf_uint8_t)ICF_DNS_MAP_CHANGE_ENTRY_REFRESH);
                if ((ICF_NULL == icf_port_strcmp(p_ssa_ctxt->p_dns_ip_addr->ip_address,
                                                 (icf_uint8_t *)p_dest_info->pIp)) &&
                    (p_ssa_ctxt->p_dns_ip_addr->port == p_dest_info->dPort))
                {
                    dOptions.dOption = SIP_OPT_CLEN|SIP_OPT_FULLFORM;

                    if (ICF_NULL != p_dns_evt_ctxt->pOverlappedTransaction)
                    {
                        p_sip_msg = p_dns_evt_ctxt->pOverlappedTransaction->pSipMsg;
                    }
                    else if(ICF_NULL != p_dns_evt_ctxt->pTransaction)
                    {
                        p_sip_msg = p_dns_evt_ctxt->pTransaction->pSipMsg;
                    }
                    if(ICF_SUCCESS == icf_ssa_resend_ongoing_txn(p_ssa_ctxt->p_call_obj, 
                            p_sip_msg, 
                            p_dns_evt_ctxt->pOverlappedTransaction,
							dOptions))
	                {
                        ICF_FUNCTION_EXIT(p_persistent_glb_pdb)
		                return;
	                }
                    else if (ICF_SUCCESS == icf_ssa_resend_on_sec_address(p_ssa_ctxt->p_call_obj,
                                p_dns_evt_ctxt->pOverlappedTransaction, 
                                p_ssa_ctxt, 
                                &sdf_error))
                    {
                        ICF_FUNCTION_EXIT(p_persistent_glb_pdb)
                        return;
                    }
                    ICF_PRINT(((icf_uint8_t*)"Getting the same entry again. So returning failure"));
                    ICF_FUNCTION_EXIT(p_persistent_glb_pdb)
                    return ;
                }
                else
			    {
				    icf_port_strcpy((icf_uint8_t *)ssa_pdb.dest_ip, (icf_uint8_t *)p_dest_info->pIp);
				    ssa_pdb.dest_port = (icf_uint16_t)p_dest_info->dPort;
			    }
            }

            if(ICF_NW_OTG_CALL_TERMINATE == p_ssa_ctxt->internal_msg_id)
            {
                /* buffered message is call termination then abort the transaction and 
                 * process BYE. In case the method is ACK then it should be sent before sending
                 * the call termination message.
                 */
                 if ( Sdf_co_null == p_dns_evt_ctxt->pTransaction)
				 {
							/*overlap transaction*/
							if (p_dns_evt_ctxt->pOverlappedTransaction) p_msg = p_dns_evt_ctxt->pOverlappedTransaction->pSipMsg;
				  }
				  else if(Sdf_co_null != p_dns_evt_ctxt->pTransaction)
                      /*either a uac or a Uas transaction*/
				  {
							p_msg = p_dns_evt_ctxt->pTransaction->pSipMsg;
				  }

                  if(Sdf_co_fail != sdf_ivk_uaGetMethodFromSipMessage(
											p_msg,
											&p_method, &sdf_error))
				  {
						if(0 != icf_port_strcmp((icf_uint8_t*)p_method,(icf_uint8_t*)"ACK"))
						{
                            /* SPR 18872: In case the transaction is nor ACK then abort
                             * the request and process BYE
                             */  
                            if(Sdf_co_null == p_dns_evt_ctxt->pTransaction) 
				            {
                                 if (p_dns_evt_ctxt->pOverlappedTransaction) sdf_ivk_uaAbortTransaction(p_ssa_ctxt->p_call_obj,
                                      p_dns_evt_ctxt->pOverlappedTransaction->dTxnId,
                                      (Sdf_ty_pvoid*)p_ssa_ctxt, &sdf_error);
				            }
				            else 
        		            {
            		              sdf_ivk_uaAbortTransaction(p_ssa_ctxt->p_call_obj,
        			                     p_dns_evt_ctxt->pTransaction->dTxnId,
                                        (Sdf_ty_pvoid*)p_ssa_ctxt, &sdf_error);
        		            }

                           if(ICF_NULL != p_ssa_ctxt->p_call_ctxt)
                           {
                                p_persistent_glb_pdb->p_call_ctx = p_ssa_ctxt->p_call_ctxt;
                                /* SPR 19189 */
                                p_ssa_ctxt->bitmask_1 &= ~ICF_SSA_DNS_QUERY_ONGOING;
                                ret_val = icf_ssa_process_buffered_msg_on_dns_resp(&ssa_pdb);
                            }
            
                            p_ssa_ctxt->internal_msg_id = ICF_NULL;
                            ICF_FUNCTION_EXIT(p_persistent_glb_pdb)
	                        return;
                        }
                  }
            }

			if ((Sdf_en_protoTcp == p_dest_info->dScheme)
#ifdef SDF_TLS
               || (Sdf_en_protoTls == p_dest_info->dScheme)
#endif
               )
            {
                ICF_SSA_SET_COMMON_BITMASK(p_ssa_ctxt->bitmask,
                                          ICF_SSA_EXPLICIT_CONNECT_REQ)
            }
            else
            {
                ICF_SSA_RESET_COMMON_BITMASK(p_ssa_ctxt->bitmask,
                                           ICF_SSA_EXPLICIT_CONNECT_REQ)
            }
	
			if ( ICF_SUCCESS == ret_val)
			{	
                icf_boolean_t	ready_to_send = ICF_TRUE;

				/* In case the SRV query failed and we had attempted a failover 
                 * A query, then the port number returned might still be zero.
                 * In this case, attempt to send on default SIP port 5060 */
                if (ICF_NULL == p_dest_info->dPort)
                {
                    ssa_pdb.dest_port = ICF_SSA_DEFAULT_SIP_PORT_NUM;
                    p_dest_info->dPort = ICF_SSA_DEFAULT_SIP_PORT_NUM;
			     	ICF_PRINT(((icf_uint8_t *)"\n[SSA]: SRV query failed, using port 5060"));
                }

				/*this flag is used to identify whether a connection
				has been made fully or not. Tjus is it to be used for
				TCP/TLS cases only*/
				
				if(Sdf_co_fail ==
						sdf_ivk_uaInitEventContext(&(p_event_context),
							&sdf_error))
				{
					ICF_PRINT(((icf_uint8_t*)"\n[SSA]:FailureInInitEventCtxt in ProcessDnsResp"));
		            sdf_ivk_uaFreeEventContext(p_event_context);
					ret_val = ICF_FAILURE;
				}
				else
				{
					p_event_context->pData = p_ssa_ctxt;
#ifdef ICF_DNS_LOOKUP_ENABLED
				    ssa_pdb.p_event_ctxt = p_event_context;
#endif
					if (ICF_NULL != 
					        (ICF_SSA_EXPLICIT_CONNECT_REQ & p_ssa_ctxt->bitmask))
					{
						if (ICF_FAILURE == icf_ssa_attempt_out_of_uatk_connect(
									&ssa_pdb, p_ssa_ctxt->p_call_obj, 
									p_dns_evt_ctxt->pTransaction, 
									p_dns_evt_ctxt->pOverlappedTransaction,
									&ready_to_send))
						{
							ICF_PRINT(((icf_uint8_t *)"\n[SSA]:FailureFrom AttemptOutOfDlgConnect"));
							ret_val = ICF_FAILURE;
						}
					}/*out of UATK connect*/
/* Fix for CSR-1-5333230 */
/* If request_url is domain name,then insert rport parameter in Via header,
   if ICF_NAT_RPORT_SUPPORT is enabled */ 
#ifdef ICF_SECURE_SUPPORT
                    /*Add alias parameter in Via in case of persistent connection*/
                    if ((ICF_SUCCESS == ret_val) &&
                        (ICF_FAILURE == icf_ssa_insert_alias_params_in_via(&ssa_pdb,
                                                   p_sip_msg)))
                    {
                        ret_val = ICF_FAILURE;
                    }
#endif
#ifdef ICF_NAT_RPORT_SUPPORT
                    /*Add rport parameter in Via in case of NAT support*/
                    if ((ICF_SUCCESS == ret_val) &&
                        (ICF_FAILURE == icf_ssa_insert_rport_params_in_via(&ssa_pdb,
                                                   p_sip_msg)))
                    {
                        ret_val = ICF_FAILURE;
                    }
#endif
/*Fix for CSR-1-5333230 ends */
					if ((ICF_SUCCESS == ret_val) &&
                        (ICF_TRUE == ready_to_send) &&
					    (Sdf_co_fail == 
							 icf_ssa_initiate_dispatch_sequence(
						    	 &ssa_pdb, p_ssa_ctxt->p_call_obj,
								 p_dns_evt_ctxt->pTransaction, 
								 p_dns_evt_ctxt->pOverlappedTransaction,
								 p_event_context,
								 ssa_pdb.p_ecode)))
					{
						ICF_PRINT(((icf_uint8_t *)"\n[SSA]:Failure in StartingDispatchSequence"));
						ret_val = ICF_FAILURE;
					}
                    /* SPR 19468: the CSR 1-6145361 was not completely merged. the DNS entry 
                     * should not be buffered in case of ACK but the DNS Transaction state should be 
                     * reset to IDLE in case it is IP Used
                     */
					/*  Now check for that if the method is ACK then buffer that 
						DNS mapping Now */
                    else if ( Sdf_co_null != p_dns_evt_ctxt->pOverlappedTransaction)
					{
							/*overlap transaction*/
							p_msg = p_dns_evt_ctxt->pOverlappedTransaction->pSipMsg;
					}
					else if(Sdf_co_null != p_dns_evt_ctxt->pTransaction)
					{
                           /*either a uac or a Uas transaction*/
							p_msg = p_dns_evt_ctxt->pTransaction->pSipMsg;
					}
                    else
                    {
                        ret_val = ICF_FAILURE;
                    }

					if(ICF_SUCCESS == ret_val && 
                            (Sdf_co_fail !=
									sdf_ivk_uaGetMethodFromSipMessage(
											p_msg,
											&p_method, &sdf_error)))
                    {
                        if(0 == icf_port_strcmp((icf_uint8_t*)p_method,(icf_uint8_t*)"ACK"))
                        {
                             /* CSR 1-6145361: Check if Dns state is IPUSed then reset to 
                             * Idle
                             */  
                            if((Sdf_co_null != p_dns_evt_ctxt->pTransaction) &&
                                    (Sdf_en_dnsIpUsed == p_dns_evt_ctxt->pTransaction->dTxnDnsState))
                            {
                                p_dns_evt_ctxt->pTransaction->dTxnDnsState = Sdf_en_Idle;
                            }

                        }
                    }
					sdf_ivk_uaFreeEventContext(p_event_context);
                    p_event_context = Sdf_co_null;
				}/*Init Event Context successful*/
			}/*ret_val success check*/
                        }
		}/*dns resp success failure check*/
		if ( (Sdf_co_false == dns_resp_flag) || 
			(ICF_FAILURE == ret_val))
		{
			if ( ICF_FAILURE == icf_ssa_process_dns_fail_resp(&ssa_pdb,
						p_dns_evt_ctxt, dns_resp_flag,ICF_FALSE))
			{
				ICF_PRINT(((icf_uint8_t *)"\n[SSA]:Failure in handling DNS FAIL RSP"));
				ret_val = ICF_FAILURE;
			}
		}
        /*SPR 18872: If any message is buffered due to ongoing DNS query then 
         * it will be handled here. Also reset the bitmask for ONGOING DNS Query.
         */ 
        else if(ICF_NULL != p_ssa_ctxt->internal_msg_id)
        {
            if(ICF_NULL != p_ssa_ctxt->p_call_ctxt)
            {
                p_persistent_glb_pdb->p_call_ctx = p_ssa_ctxt->p_call_ctxt;
                /* SPR 19189 */
                p_ssa_ctxt->bitmask_1 &= ~ICF_SSA_DNS_QUERY_ONGOING;
                ret_val = icf_ssa_process_buffered_msg_on_dns_resp(&ssa_pdb);
            }
            p_ssa_ctxt->internal_msg_id = ICF_NULL;
        }
	}

    /*This bit is set in ssa_pdb when primary server is not responding
     *and SSA sent request on secondary server, so call object with primary server
     *will be freed in this case*/
    if (ICF_SSA_DEALLOCATE_CALL_OBJECT & ssa_pdb.common_bitmask)
    {
        Sdf_st_callObject *p_call_obj = ssa_pdb.p_call_obj_to_dealloc;
        sdf_ivk_uaFreeCallObject(&p_call_obj);
    }
	
	ICF_FUNCTION_EXIT(p_persistent_glb_pdb)
	return;
}

icf_return_t icf_ssa_resend_ongoing_txn(
                Sdf_st_callObject *pCallObj,
                SipMessage *pSipMsg,
                Sdf_st_overlapTransInfo *pOverlapTxn,
                SipOptions dOptions)
{
	icf_return_t	ret_val = ICF_SUCCESS;

	Sdf_st_eventContext *pEventContext = ICF_NULL;
	Sdf_st_error ecode;
	icf_ssa_ctxt_st		      *p_ssa_ctxt = ICF_NULL; 
    Sdf_st_overlapTransInfo   *pTransInfo = Sdf_co_null;
    Sdf_st_transaction        *pTransaction = Sdf_co_null;
    Sdf_st_transportInfo 	  *p_dest_Info = Sdf_co_null;	
    /* Fix for SPR 20412: CSR 1-8223807 merged*/
    Sdf_ty_s8bit              *p_remove_hdr_list = ICF_NULL;
    icf_return_t               dummy_ret_val = ICF_SUCCESS;
	icf_error_t			       err = 0;


    /*Start SPR 18846 */ 
    if((Sdf_co_null != pCallObj->pAppData) &&
       (Sdf_co_null != pCallObj->pAppData->pData))
    {
        p_ssa_ctxt = (icf_ssa_ctxt_st*)(pCallObj->pAppData->pData);
    }
    else
    {
        ICF_PRINT(((icf_uint8_t *)"\n[SSA]:SSA CTX is null")); 
        return ICF_FAILURE;
    }/*End SPR18846 */ 
        /*Check for CSR 1-4055404.By Aman 7.12.06*/
	if(ICF_NULL == pSipMsg)
        {
		ret_val = ICF_FAILURE;
	}
	if(Sdf_co_fail == 
		sdf_ivk_uaInitEventContext(&pEventContext, &ecode))
	{
		ICF_PRINT(((icf_uint8_t *)"\n[SSA]:FailureInInitEventCtxt in SSAResendOngngTxn")); 
		ret_val = ICF_FAILURE;
	}
	
    if ( ICF_SUCCESS == ret_val)
	{
		icf_config_data_st		*p_config_data = ICF_NULL;
        icf_line_id_t		    line_id = ICF_INVALID_LINE_ID;
        icf_line_data_st	    *p_line_data = ICF_NULL;

		if ( ICF_NULL != p_ssa_ctxt->p_call_ctxt)
		{
			line_id = p_ssa_ctxt->p_call_ctxt->line_id;
		}
		else if ( ICF_NULL != p_ssa_ctxt->p_sic_ctxt)
		{	
			line_id = p_ssa_ctxt->p_sic_ctxt->line_id;
            /* Fix for SPR 20412: CSR 1-8223807 merged*/
            if(ICF_NULL != p_ssa_ctxt->p_sic_ctxt->p_remove_hdrs)
            {
                ICF_MEMGET(p_persistent_glb_pdb,
                      icf_port_strlen((icf_uint8_t *)p_ssa_ctxt->p_sic_ctxt->p_remove_hdrs) + 1,
                      ICF_MEM_COMMON,
                      p_remove_hdr_list,
                      ICF_RET_ON_FAILURE,
                      (icf_error_t *)&err, ret_val)

                icf_port_strcpy(p_remove_hdr_list,
                           p_ssa_ctxt->p_sic_ctxt->p_remove_hdrs);
            }
		}
        else if((ICF_NULL != p_ssa_ctxt->p_rgm_ctxt) && ((p_ssa_ctxt->p_rgm_ctxt->num_of_lines-1)< ICF_MAX_NUM_OF_LINES) && ((p_ssa_ctxt->p_rgm_ctxt->num_of_lines-1) >= 0))
        {
            line_id = p_ssa_ctxt->p_rgm_ctxt->rgm_list_of_lines[p_ssa_ctxt->p_rgm_ctxt->num_of_lines-1];
        }

		
		if (ICF_SUCCESS == 
           icf_dbm_fetch_line_cntxt_blk(
           p_persistent_glb_pdb, line_id,
           (icf_line_data_st **)(&p_line_data),
           &err))
        {
			pEventContext->dTimerOption |= SIP_OPT_TIMER_T1;
			pEventContext->dTimerOption |= SIP_OPT_TIMER_F;
			pEventContext->dTimerOption |= SIP_OPT_TIMER_B;
			pEventContext->dTimerOption |= SIP_OPT_TIMER_T2;
			pEventContext->dTimerOption |= SIP_OPT_TIMER_H;
            pEventContext->dTimerOption |= SIP_OPT_TIMER_I;
            pEventContext->dTimerOption |= SIP_OPT_TIMER_K;

			pEventContext-> dTimeoutValues.dT1 = 
                  p_line_data->sip_timer_t1;

			pEventContext-> dTimeoutValues.dT2 = 
                  p_line_data->sip_timer_t2;
      
			pEventContext-> dTimeoutValues.dTimerB = 
                  p_line_data->sip_timer_invite;

			pEventContext-> dTimeoutValues.dTimerF_T3 = 
                  p_line_data->sip_timer_non_invite;
	
			pEventContext-> dTimeoutValues.dTimerH = 
                  p_line_data->sip_timer_t1 * 64;

            pEventContext-> dTimeoutValues. dTimerI_T4 =
                  p_line_data->sip_timer_t4;

            pEventContext-> dTimeoutValues. dTimerK_T4 =
                  p_line_data->sip_timer_t4;

		}

#ifdef ICF_DNS_LOOKUP_ENABLED

        /* DNS-Buffer code starts here-- this code is for making the retries
		   configurable iff line data is filled with default values */ 

        if(pEventContext-> dTimeoutValues.dT1 == ICF_CFG_DEF_SIP_TIMER_T1 &&
                pEventContext-> dTimeoutValues.dT2 == ICF_CFG_DEF_SIP_TIMER_T2 &&
                pEventContext->dTimeoutValues.dTimerB == ICF_CFG_DEF_SIP_TIMER_INVITE &&
                pEventContext->dTimeoutValues.dTimerF_T3 == ICF_CFG_DEF_SIP_TIMER_NON_INVITE)
        {
            icf_error_t                 err;
            if(ICF_FAILURE == icf_dbm_get_module_glb_data(
                        p_persistent_glb_pdb,
                        (icf_int_module_id_t)ICF_INT_MODULE_CFG,
                        (icf_void_t *)&p_config_data, &err))
            {
                /* didn't get the config data can't make the retry to be configurable
                   raise the error */
            }
            else if (ICF_NULL != p_config_data )
            {
                pEventContext->dTimerOption |= SIP_OPT_TIMER_T1;
                pEventContext->dTimerOption |= SIP_OPT_TIMER_F;
                pEventContext->dTimerOption |= SIP_OPT_TIMER_B;
                pEventContext->dTimerOption |= SIP_OPT_TIMER_T2;
                pEventContext->dTimeoutValues.dT1= ICF_CFG_DEF_SIP_TIMER_T1; /* default value */
                /* for NON-Invite x-saction */
                pEventContext->dTimeoutValues.dTimerF_T3=  
                    (icf_ssa_dns_2_pow(p_config_data->sip_req_retry)) * pEventContext->dTimeoutValues.dT1; 
                /* for INvite X-saction */
                pEventContext->dTimeoutValues.dTimerB= 
                    (icf_ssa_dns_2_pow(p_config_data->sip_req_retry)) * pEventContext->dTimeoutValues.dT1;
                /* configure the T2 */
              /*  if(6==p_config_data->sip_req_retry)
                {
                    pEventContext-> dTimeoutValues.dT2 = 2 * ICF_CFG_DEF_SIP_TIMER_T2;
                }
                else if(7==p_config_data->sip_req_retry)
                {
                    pEventContext-> dTimeoutValues.dT2 = 4 * ICF_CFG_DEF_SIP_TIMER_T2;
                }
                else*/
                {
                    pEventContext-> dTimeoutValues.dT2 = ICF_CFG_DEF_SIP_TIMER_T2;/* default value */
                }


            }
        }
#endif
		pEventContext->pData = pCallObj->pAppData->pData;
		do
		{
			if (Sdf_co_fail == sdf_ivk_uaNextDnsTransaction(pCallObj,
						pOverlapTxn, &ecode))
			{
#if 0
                /* DNS Buffer Code : all the IP addresses returned in the DNS query have been checked
				   with no success delete the DNS buffer entry for this FQDN*/

				if((p_persistent_glb_pdb->p_glb_data->p_dns_info->bitmask & ICF_DNS_BUFFER_FQDN_PRESENT) != ICF_NULL )
				{
					icf_error_t ecode;

					if(ICF_FAILURE == (icf_ssa_dns_delete_fqdn_entry(&(p_persistent_glb_pdb->p_glb_data->p_dns_info->fqdn),&ecode)))
					{
							
						/* Unable to delete the DNS entry log the Error */
					}
				}
				/* end of DNS buffer code */
#endif
				ret_val = ICF_FAILURE;
				break;
			}
			else if (Sdf_co_success == sdf_ivk_uaSendCallToPeer(pCallObj, pSipMsg, \
                                                       dOptions, 0, pEventContext, \
                                                       p_remove_hdr_list, &ecode))
			{
                   /* Fix for SPR 20412: CSR 1-8223807 merged*/
                   if(ICF_NULL != p_remove_hdr_list)
                   {
                      ICF_MEMFREE(
                           p_persistent_glb_pdb,
                           p_remove_hdr_list,
                           ICF_MEM_COMMON,
                           &err,dummy_ret_val);

                      p_remove_hdr_list = ICF_NULL;
                   }
#ifdef ICF_DNS_LOOKUP_ENABLED
                   /*This is to save the Ip and the port at which the request is sent,So that when the response comes 
                    *from this Ip,Iptk willl buffer it.
                    */ 
                    if(sdf_ivk_uaGetTransactionForSipMsg(\
                                        (pCallObj), pSipMsg, &pTransaction, \
                                        &pTransInfo, &ecode) == Sdf_co_success)
                    {
 
                     if (sdf_ivk_uaGetDestInfoFromTransaction(pTransaction,
            			  pTransInfo,
			              &p_dest_Info, 
                		  &ecode) == Sdf_co_fail)
                     {
                    	  ret_val = Sdf_co_fail;
                          break;
                     }
                     icf_port_strcpy(p_ssa_ctxt->ip_fr_req, (icf_uint8_t *)p_dest_Info->pIp);
                     p_ssa_ctxt->port_fr_req = (icf_uint16_t)p_dest_Info->dPort;
                     if(Sdf_co_null != pTransaction)
                     {
                        (void)sdf_ivk_uaFreeTransportInfo(pTransaction->pDestInfo);
                         sdf_ivk_uaFreeTransaction(pTransaction);
                     }
                     else if(Sdf_co_null != pTransInfo)
                     {
                       (void)sdf_ivk_uaFreeTransportInfo(pTransInfo->pDestInfo);
                        sdf_ivk_uaFreeOverlapTransInfo(pTransInfo);
                     } 
                   } 

#endif
				break;
			}
		} while (1);
	}
    
    sdf_ivk_uaFreeEventContext(pEventContext);
#ifdef ICF_DNS_LOOKUP_ENABLED
    p_ssa_ctxt= p_ssa_ctxt;
#endif

    /* Fix for SPR 20412: CSR 1-8223807 merged*/
    if(ICF_NULL != p_remove_hdr_list)
    {
       ICF_MEMFREE(
            p_persistent_glb_pdb,
            p_remove_hdr_list,
            ICF_MEM_COMMON,
            &err,dummy_ret_val);

       p_remove_hdr_list = ICF_NULL;
    }

	return ret_val;
}

/*****************************************************************************
 ** FUNCTION:           icf_ssa_resend_on_sec_address
 **
 ** DESCRIPTION:        This function will be called when request is to be 
 **                     sent on secondary(inactive) server (if configured) 
 **                     or secondary FQDN of primary (active) server (if 
 **                     received in DNS SRV response)
 **
 *****************************************************************************/
icf_return_t icf_ssa_resend_on_sec_address(
        Sdf_st_callObject                      *pCallObj,
        Sdf_st_overlapTransInfo                *pOverlapTxn,
        Sdf_ty_pvoid                           pContext,
        Sdf_st_error                           *pError)
{
    icf_return_t        ret_val = ICF_SUCCESS;
    Sdf_st_error        sdf_error = {0,0,0,"\0"};
    SipMessage	        *p_sip_msg = ICF_NULL;
    icf_ssa_ctxt_st     *p_ssa_ctxt = ICF_NULL;
    icf_ssa_pdb_st		*p_ssa_pdb = ICF_NULL;
    icf_glb_pdb_st      *p_glb_pdb = ICF_NULL;
    /*SPR18535 added ifdef for TRACE ENABLE*/
#ifdef ICF_TRACE_ENABLE
    icf_boolean_t       sec_server_active = ICF_FALSE;
    icf_boolean_t       trace_for_reg = ICF_FALSE;
#endif    

    pError = pError;
    p_ssa_ctxt = (icf_ssa_ctxt_st*)((Sdf_st_appData*)pContext);
    /* SPR 19189 */
    if(ICF_NULL == p_ssa_ctxt)
    {
        ICF_PRINT(((icf_uint8_t *)"\n[SSA]: p_ssa_ctxt is NULL"))
        return ICF_FAILURE;
    }
    p_ssa_pdb = (icf_ssa_pdb_st*)p_ssa_ctxt->p_ssa_pdb;
    /* SPR 19189 */
    if(ICF_NULL == p_ssa_pdb)
    {
        ICF_PRINT(((icf_uint8_t *)"\n[SSA]: p_ssa_pdb is NULL"))
        return ICF_FAILURE;
    }
    p_glb_pdb = p_ssa_pdb->p_glb_pdb;
    if(ICF_NULL == p_glb_pdb)
    {
        ICF_PRINT(((icf_uint8_t *)"\n[SSA]: p_glb_pdb is NULL"))
        return ICF_FAILURE;
    }
    /*SPR18535 :warning removal when TRACE_ENABLE is off*/
    p_glb_pdb = p_glb_pdb;

    ICF_FUNCTION_ENTER(p_glb_pdb)
    
    /* Fix for SPR 19614 */
    
    if (ICF_NULL != p_ssa_ctxt->p_call_ctxt)
    {
        p_ssa_ctxt->line_id = p_ssa_ctxt->p_call_ctxt->line_id;
    }
    if ( ICF_FAILURE == 
        icf_dbm_fetch_line_cntxt_blk(
           p_ssa_pdb->p_glb_pdb, 
           p_ssa_pdb->p_ssa_ctxt->line_id,
           (icf_void_t *)&(p_ssa_pdb->p_line_data),
           p_ssa_pdb->p_ecode))
    {
        ret_val = ICF_FAILURE;
    }
    /* If primary as well as secondary has been tried */
    else if (ICF_SSA_MAX_REM_SERVERS_TRIED == p_ssa_ctxt->rem_servers_tried)
    {
        /* return FAILURE
         * error - Server not reponding*/
        if (ICF_NULL != p_ssa_ctxt->p_rgm_ctxt)
        {
            /* Registrar not responding */
            *(p_ssa_pdb->p_ecode) = ICF_CAUSE_REGISTRAR_NOT_RESPONDING;
        }
        else if (ICF_NULL != p_ssa_ctxt->p_call_ctxt)
        {
            /* Proxy not responding */
            *(p_ssa_pdb->p_ecode) = 
                ICF_CAUSE_PROXY_SERVER_NOT_RESPONDING;
            p_ssa_ctxt->p_call_ctxt->release_cause =
                ICF_CALL_CLEAR_REASON_PROXY_SERVER_NOT_RESPONDING;
        }
        else if (ICF_NULL != p_ssa_ctxt->p_sic_ctxt)
        {
            /* Proxy not responding */
            *(p_ssa_pdb->p_ecode) = 
                ICF_CAUSE_PROXY_SERVER_NOT_RESPONDING;
        }
#ifdef ICF_TRACE_ENABLE
        /*Generate Trace for server not responding*/
        /* SPR 19189 */
        ICF_CHECK_IF_VAR_IS_NULL(p_glb_pdb,
                                 p_ssa_pdb->p_glb_cfg_data,
                                 ret_val)
        if(ICF_FAILURE == ret_val)
        {
            ICF_FUNCTION_EXIT(p_glb_pdb)
            return ret_val;
        }

        /* SPR 19189 */
        ICF_CHECK_IF_VAR_IS_NULL(p_glb_pdb,
                                 p_ssa_pdb->p_line_data,
                                 ret_val)
        if(ICF_FAILURE == ret_val)
        {
            ICF_FUNCTION_EXIT(p_glb_pdb)
            return ret_val;
        }

        if ((ICF_NULL != p_ssa_ctxt->p_rgm_ctxt) &&
        (ICF_FALSE == p_ssa_pdb->p_glb_cfg_data->ims_oprn_flag))
        {
            /*If its a direct REGISTER request then copy flag from
             * line_data*/
            sec_server_active = 
                p_ssa_pdb->p_line_data->sec_registrar_active;
            /*Also mark that this trace is for REGISTRAR address*/
            trace_for_reg = ICF_TRUE;
        }
        else if (ICF_NULL != p_ssa_pdb->p_glb_cfg_data->p_dhcp_resp)
        {
            /*If its a PROXY routed request when proxy address is fetched
             * from DHCP response, then copy flag from config_data*/
            sec_server_active = 
                p_ssa_pdb->p_glb_cfg_data->sec_proxy_server_active;
        }
        else
        {
            /*If its a PROXY routed request when proxy address is configured
             * by application then copy flag from line_data*/
            sec_server_active =
                p_ssa_pdb->p_line_data->sec_proxy_server_active;
        }
        /*If trace is for register then following condition:
         * 1. Trace for primary reg not resp, when sec is found in DNS cache
         * is unreachable and Secondary resolved thru DNS query is also 
         * unreachable and now primary is also not responding
         * 2. Trace for Sec not responding, when sec found in DNS cache is 
         * unreachable and secondary resolved thru DNS query is also 
         * unreachable
         * 3. Same is true when primary is active*/
        if (ICF_TRUE == trace_for_reg)
        {   
            if (ICF_TRUE == sec_server_active)
            {
                if (ICF_NULL != 
                        (p_ssa_ctxt->bitmask_1 & 
                         ICF_SSA_SEC_SERVER_ADDRESS_IS_USED))
                {
                    ICF_SSA_APP_TRACE(p_ssa_pdb,
                            ICF_TRACE_PRIMARY_REGISTRAR_NOT_RESPONDING)
                }
                else
                {
                    ICF_SSA_APP_TRACE(p_ssa_pdb,
                            ICF_TRACE_SECONDARY_REGISTRAR_NOT_RESPONDING)
                }
            }
            else
            {
                if (ICF_NULL != 
                        (p_ssa_ctxt->bitmask_1 & 
                         ICF_SSA_SEC_SERVER_ADDRESS_IS_USED))
                {
                    ICF_SSA_APP_TRACE(p_ssa_pdb,
                            ICF_TRACE_SECONDARY_REGISTRAR_NOT_RESPONDING)
                }
                else
                {
                    ICF_SSA_APP_TRACE(p_ssa_pdb,
                            ICF_TRACE_PRIMARY_REGISTRAR_NOT_RESPONDING)
                }
            }
        }
        /*If trace is for proxy then following condition:
         * 1. Trace for primary proxy not resp, when sec is found in DNS cache
         * is unreachable and Secondary resolved thru DNS query is also 
         * unreachable and now primary is also not responding
         * 2. Trace for Sec not responding, when sec found in DNS cache is 
         * unreachable and secondary resolved thru DNS query is also 
         * unreachable
         * 3. Same is true when primary is active*/
        else
        {   
            if (ICF_TRUE == sec_server_active)
            {
                if (ICF_NULL != 
                        (p_ssa_ctxt->bitmask_1 & 
                         ICF_SSA_SEC_SERVER_ADDRESS_IS_USED))
                {
                    ICF_SSA_APP_TRACE(p_ssa_pdb,
                            ICF_TRACE_PRIMARY_PROXY_NOT_RESPONDING)
                }
                else
                {
                    ICF_SSA_APP_TRACE(p_ssa_pdb,
                            ICF_TRACE_SECONDARY_PROXY_NOT_RESPONDING)
                }
            }
            else
            {
                if (ICF_NULL != 
                        (p_ssa_ctxt->bitmask_1 & 
                         ICF_SSA_SEC_SERVER_ADDRESS_IS_USED))
                {
                    ICF_SSA_APP_TRACE(p_ssa_pdb,
                            ICF_TRACE_SECONDARY_PROXY_NOT_RESPONDING)
                }
                else
                {
                    ICF_SSA_APP_TRACE(p_ssa_pdb,
                            ICF_TRACE_PRIMARY_PROXY_NOT_RESPONDING)
                }
            }
        }
#endif
        ret_val = ICF_FAILURE;
    }
    /*  else if inactive server is available to be resolved */
    else if (ICF_NULL != p_ssa_ctxt->p_rem_addr_inactive)
    {
        p_ssa_ctxt->bitmask_1 |= ICF_SSA_SEC_SERVER_ADDRESS_IS_USED;
        (p_ssa_ctxt->rem_servers_tried)++;

        if ((ICF_NULL != pCallObj->pUacTransaction) &&
            (ICF_NULL != pCallObj->pUacTransaction->pSipMsg))
        {
            p_sip_msg = pCallObj->pUacTransaction->pSipMsg;
        }
        /*Klocwork warning removal*/
        else if(ICF_NULL != pOverlapTxn)
        {
            p_sip_msg = pOverlapTxn->pSipMsg;
        }

        /*Send call on secondary server address*/
        ret_val = icf_ssa_update_dialog_for_sec_addr(p_ssa_pdb, p_sip_msg);
    }
    /* else if second FQDN of active server is available to be resolved */
    else if (0 != p_ssa_ctxt->fqdn_list[1].str_len)
    {
        Sdf_st_dnsSrvRecord     *p_srv_rec  = Sdf_co_null;
        /* SPR 19189 */
        ICF_CHECK_IF_VAR_IS_NULL(p_glb_pdb,
                                 p_ssa_pdb->p_ssa_ctxt->p_call_obj,
                                 ret_val)
        if(ICF_FAILURE == ret_val)
        {
            ICF_FUNCTION_EXIT(p_glb_pdb)
            return ret_val;
        }
        
        /* Control reaches here when top FQDN of primary server has been tried, 
         * secondary server is not configured, and now second top FQDN of
         * primary server is to be tried */
        if (ICF_NULL != pOverlapTxn)
        {
            sdf_ivk_uaGetDnsSrvRecord(
                    &(pOverlapTxn->slDnsRecords),
                    &p_srv_rec,&sdf_error);
            
            /*If the value of p_srv_rec is NULL retrun failure from here.
              This is done because in the following code we are passing the
              &(p_srv_rec->slIpRecords) as an input parameter for function
              icf_ssa_dns_query_request.This results in a crash in DNS query
              handling.Now in this case the retry on secondary address will be
              failure.*/
            if (ICF_NULL == p_srv_rec)
            {
                ret_val = ICF_FAILURE;
                ICF_FUNCTION_EXIT(p_glb_pdb)
                return ret_val;
            }            
        }
        else
        {
            /* SPR 19189 */
            ICF_CHECK_IF_VAR_IS_NULL(p_glb_pdb,
                                     p_ssa_pdb->p_ssa_ctxt->p_call_obj->pUacTransaction,
                                     ret_val)
            if(ICF_FAILURE == ret_val)
            {
                ICF_FUNCTION_EXIT(p_glb_pdb)
                return ret_val;
            }

            sdf_ivk_uaGetDnsSrvRecord(
                    &(p_ssa_pdb->p_ssa_ctxt->p_call_obj->pUacTransaction->slDnsRecords),
                    &p_srv_rec,&sdf_error);
            
            /*If the value of p_srv_rec is NULL retrun failure from here.
              This is done because in the following code we are passing the
              &(p_srv_rec->slIpRecords) as an input parameter for function
              icf_ssa_dns_query_request.This results in a crash in DNS query
              handling.Now in this case the retry on secondary address will be
              failure.*/
            if (ICF_NULL == p_srv_rec)
            {
                ret_val = ICF_FAILURE;
                ICF_FUNCTION_EXIT(p_glb_pdb)
                return ret_val;
            }
        }

        if (ICF_FAILURE == icf_ssa_create_next_transaction(
                    p_ssa_pdb->p_ssa_ctxt->p_call_obj,
                    pOverlapTxn, &sdf_error))
        {
            ret_val = ICF_FAILURE;
        }
        else
        {
            ret_val = icf_ssa_dns_query_request(p_ssa_pdb,pOverlapTxn, 
                p_ssa_ctxt->fqdn_list[1].str,
                &(p_srv_rec->slIpRecords),
                Sdf_en_IPv4Address);
            (p_ssa_ctxt->rem_servers_tried)++;
        }
    }
    /* else if neither inactive server nor second FQDN of active server 
     * is available */
    else
    {
        /* SPR 19189 */
        ICF_CHECK_IF_VAR_IS_NULL(p_glb_pdb,
                                 p_ssa_pdb->p_glb_cfg_data,
                                 ret_val)
        if(ICF_FAILURE == ret_val)
        {
            ICF_FUNCTION_EXIT(p_glb_pdb)
            return ret_val;
        }
        /* Control reaches here if first FQDN of primary server has been tried 
         * and is not responding, but there is no secondary server configured, 
         * also there is no second FQDN of primary server to be tried now, thus
         * return FAILURE
         * error - Server not reponding*/
        if ((ICF_NULL != p_ssa_ctxt->p_rgm_ctxt) && 
            (ICF_FALSE == p_ssa_pdb->p_glb_cfg_data->ims_oprn_flag))
        {
            /* Registrar not responding */
            ICF_SSA_APP_TRACE(p_ssa_pdb,
                      ICF_TRACE_PRIMARY_REGISTRAR_NOT_RESPONDING)
            *(p_ssa_pdb->p_ecode) = ICF_CAUSE_REGISTRAR_NOT_RESPONDING;
        }
        else
        {
            /* Proxy not responding */
            ICF_SSA_APP_TRACE(p_ssa_pdb,
                      ICF_TRACE_PRIMARY_PROXY_NOT_RESPONDING)
            *(p_ssa_pdb->p_ecode) = 
                ICF_CAUSE_PROXY_SERVER_NOT_RESPONDING;
            /*Fill call clear reason as proxy not responding in 
             *release_cause in call context, only when p_rem_addr_active 
             *is non-NULL which indicates that request was thru proxy*/
            if ((ICF_NULL != p_ssa_ctxt->p_call_ctxt) &&
                (ICF_NULL != p_ssa_ctxt->p_rem_addr_active))
            {                    
                p_ssa_ctxt->p_call_ctxt->release_cause = 
                   ICF_CALL_CLEAR_REASON_PROXY_SERVER_NOT_RESPONDING;
            }
        }
        ret_val = ICF_FAILURE;
    }
    ICF_FUNCTION_EXIT(p_glb_pdb)
    return ret_val;
}

icf_return_t icf_ssa_get_next_dest_from_list(
        icf_ssa_pdb_st	*p_ssa_pdb,
        Sdf_st_callObject *pCallObj,
        Sdf_st_transaction *p_txn,
        Sdf_st_overlapTransInfo *p_overlap_txn)
{
    icf_return_t	ret_val = ICF_FAILURE;
	Sdf_st_transportInfo    *p_dest_info = Sdf_co_null;
	Sdf_st_error            sdf_error;

	do
	{
		if (Sdf_co_fail == sdf_ivk_uaNextDnsTransaction(pCallObj,
					p_overlap_txn, &sdf_error))
		{
			break;
		}
		else
		{
			if ( Sdf_co_null == p_txn)
			{
				p_dest_info = 
					p_overlap_txn->pDestInfo;
			}
			else
			{
				p_dest_info = 
					p_txn->pDestInfo;
			}
			if ((Sdf_co_null == p_dest_info) || 
					(Sdf_co_null == p_dest_info->pIp))
			{
				ICF_PRINT(((icf_uint8_t *)"\n[SSA]:EmptyDestinationInfo!!returning"));
				continue;
			}
			else
			{
				icf_port_strcpy((icf_uint8_t *)p_ssa_pdb->dest_ip, (icf_uint8_t *)p_dest_info->pIp);
				p_ssa_pdb->dest_port = (icf_uint16_t)p_dest_info->dPort;
				ret_val = ICF_SUCCESS;
				break;
			}
		}
	} while (1);

	return ret_val;
}


/***************************************************************************
 ** FUNCTION:           icf_ssa_dns_start_timer
 **
 ** DESCRIPTION:        This function will be used to start
 **                     the dns timer. 
 ***************************************************************************/
icf_return_t icf_ssa_dns_start_timer(
		INOUT icf_glb_pdb_st *p_glb_pdb,
		IN  icf_duration_t   timer_dur,
		OUT icf_error_t      *p_ecode)
{
	icf_timer_data_st           *p_timer_data = ICF_NULL;
	icf_return_t                ret_val = ICF_SUCCESS;
    icf_ssa_timer_data_st       *p_ssa_timer_data = ICF_NULL; 
    icf_return_t                return_val = ICF_SUCCESS;

    /* SPR 18880 : Added Null checks */
    if(ICF_NULL == p_glb_pdb)
    {
        ICF_PRINT(((icf_uint8_t *)("\n[SSA] p_glb_pdb is NULL\n.")))
        return ICF_FAILURE;
    }
 
    ICF_FUNCTION_ENTER(p_glb_pdb) /* function entry trace */

    ICF_CHECK_IF_ANY_OF_TWO_VAR_IS_NULL(p_glb_pdb,
                                        p_glb_pdb->p_glb_data,
                                        p_glb_pdb->p_glb_data->p_dns_info,
                                        return_val)
   
    if(ICF_FAILURE == return_val)
    {
        /* no DNS info return the failue */
        ret_val = ICF_FAILURE;
        return ret_val;
    }
    if(ICF_FAILURE == icf_dbm_get_mem_from_pool(
                p_glb_pdb, ICF_MEM_TIMER,(icf_void_t **) &p_timer_data, 
                p_ecode))
    {
        ret_val = ICF_FAILURE;
    }
    else if ( ICF_FAILURE == icf_dbm_get_mem_from_pool(\
                p_glb_pdb, ICF_MEM_SSA_TIMER, 
                &(p_timer_data->p_timer_buffer),p_ecode))
    {
        /*Klocwork Warning Removal*/
        ICF_KLOCWORK_FALSE_POSITIVE(p_glb_pdb, p_timer_data->p_timer_buffer)
            /*SPR 19223 removed null check for p_timer_data as
              it's accessed just before so no point in having 
              null check here */
        ret_val = icf_dbm_ret_mem_to_pool(p_glb_pdb, ICF_MEM_TIMER,
                          p_timer_data, p_ecode);
        if(ICF_SUCCESS == ret_val)
        {
                p_timer_data = ICF_NULL;
        }
        /* Timer block freeing failure. Raise a alarm and proceed. */
        else
        {
               ICF_PRINT(((icf_uint8_t *)"[SSA]: Failure in freeing timer block.\n"))
        }
        ret_val = ICF_FAILURE;
    }
    else if((ICF_NULL != p_timer_data) &&
            (ICF_NULL != p_timer_data->p_timer_buffer))
    {    
        /* fill the timer data structure */
        p_timer_data->timer_type = ICF_TIMER_TYPE_DNS_BUFFER_TIMER;
        p_timer_data->module_id = ICF_INT_MODULE_SSA;

        p_ssa_timer_data = p_timer_data->p_timer_buffer;

        p_timer_data->p_glb_pdb = p_glb_pdb;
        p_glb_pdb->p_glb_data->p_dns_info->p_dns_timer_data = p_timer_data;

        ICF_START_TIMER(p_glb_pdb, p_timer_data, p_glb_pdb->p_glb_data->p_dns_info->dns_buffer_timer_id,
                timer_dur, p_ecode, ret_val)

        if(ICF_FAILURE == ret_val)
        {
            ICF_PRINT(((icf_uint8_t *)"\n UNABLE to start the DNS TIMER \n"));
            /*Free the allocated timer block here*/
            if(ICF_NULL != p_timer_data)
            {
                ret_val = icf_dbm_ret_mem_to_pool(p_glb_pdb, ICF_MEM_TIMER,
                          p_timer_data, p_ecode);
                if(ICF_SUCCESS == ret_val)
                {
                    p_timer_data = ICF_NULL;
                }
                /* Timer block freeing failure. Raise a alarm and proceed. */
                else
                { 
                    ICF_PRINT(((icf_uint8_t *)"[SSA]: Failure in freeing timer block.\n"))
                }
            }
            /*Free the allocated ssa timer block here*/
            if(ICF_NULL != p_ssa_timer_data)
            {
                ICF_SSA_RESET_SSA_TIMER_BLK(p_ssa_timer_data)
                ret_val = icf_dbm_ret_mem_to_pool(p_glb_pdb, ICF_MEM_SSA_TIMER,
                          p_ssa_timer_data, 
                          p_ecode);
                if(ICF_SUCCESS == ret_val)
                {
                    p_ssa_timer_data = ICF_NULL;
                }
                /* Timer block freeing failure. Raise a alarm and proceed. */
                else
                {
                    ICF_PRINT(((icf_uint8_t *)"[SSA]: Failure in freeing timer block.\n"))
                }
            }
            /* restore the value of ret_val to ICF_FAILURE */
            ret_val = ICF_FAILURE;
        }    
    }
    ICF_PRINT(((icf_uint8_t *)"\n DNS TIMER STARTED \n"));
    ICF_FUNCTION_EXIT(p_glb_pdb)
    return ret_val;

} 


/*******************************************************************************
 *
 * FUNCTION:        icf_ssa_dns_stop_timer
 *
 * DESCRIPTION:     This function will be used to stop dns timer.
 *
 ******************************************************************************/
icf_return_t icf_ssa_dns_stop_timer(
	INOUT	icf_glb_pdb_st  *p_glb_pdb,
        OUT     icf_error_t    *p_ecode)    
	  
{
    icf_return_t                     ret_val = ICF_SUCCESS;
    icf_ssa_timer_data_st       *p_ssa_timer_data = ICF_NULL;
    
    /* SPR 18880 : Added Null checks */
    if(ICF_NULL == p_glb_pdb)
    {
        ICF_PRINT(((icf_uint8_t *)("\n[SSA] p_glb_pdb is NULL\n.")))
        return ICF_FAILURE;
    }

    ICF_FUNCTION_ENTER(p_glb_pdb) /* function entry trace */

	/* check if the timer is running or not */
    if((ICF_NULL != p_glb_pdb->p_glb_data) && 
       (ICF_NULL != p_glb_pdb->p_glb_data->p_dns_info) &&
       (ICF_TIMER_ID_INVALID !=  p_glb_pdb->p_glb_data->p_dns_info->dns_buffer_timer_id))
	{
		/* Non-zero id means that the timer is running, so stop it */
		ICF_STOP_TIMER(p_glb_pdb, p_glb_pdb->p_glb_data->p_dns_info->dns_buffer_timer_id, p_ecode, \
			ret_val)
        ICF_PRINT(((icf_uint8_t *)"\n DNS Purge timer Stoped \n"));   

        /* free timer blocks only if timer was stopped successfully */
        if(ICF_SUCCESS == ret_val)
        {
            /* free the memory allocated to any existing timer data block */
	        if((ICF_NULL != p_glb_pdb->p_glb_data) && 
               (ICF_NULL != p_glb_pdb->p_glb_data->p_dns_info) && 
               (ICF_NULL != p_glb_pdb->p_glb_data->p_dns_info->p_dns_timer_data))
	        {
                p_ssa_timer_data = (icf_ssa_timer_data_st *)((icf_timer_data_st*)p_glb_pdb->p_glb_data->p_dns_info->
                                               p_dns_timer_data)->p_timer_buffer;
                ICF_SSA_RESET_SSA_TIMER_BLK(p_ssa_timer_data)
                ret_val = icf_dbm_ret_mem_to_pool(p_glb_pdb, ICF_MEM_SSA_TIMER,
                          ((icf_timer_data_st*)
                          p_glb_pdb->p_glb_data->p_dns_info->p_dns_timer_data)->p_timer_buffer, 
                          p_ecode);
                if(ICF_SUCCESS == ret_val)
                {
                    ((icf_timer_data_st*)
                        p_glb_pdb->p_glb_data->p_dns_info->p_dns_timer_data)->p_timer_buffer =
                        ICF_NULL;
                }
                /* Timer block freeing failure. Raise a alarm and proceed. */
                else
                {
                    ICF_PRINT(((icf_uint8_t *)"[SSA]: Failure in freeing timer block.\n"))
                }

                /*Free the allocated timer block here*/
                ret_val = icf_dbm_ret_mem_to_pool(p_glb_pdb, ICF_MEM_TIMER,
                          p_glb_pdb->p_glb_data->p_dns_info->p_dns_timer_data, p_ecode);
                if(ICF_SUCCESS == ret_val)
                {
                    p_glb_pdb->p_glb_data->p_dns_info->p_dns_timer_data = ICF_NULL;
                }
                /* Timer block freeing failure. Raise a alarm and proceed. */
                else
                {
                    ICF_PRINT(((icf_uint8_t *)"[SSA]: Failure in freeing timer block.\n"))
                }
	        }
        }
	} /* end of if for timer_id */
	

    ICF_FUNCTION_EXIT(p_glb_pdb);
	return ret_val;
}/* end of icf_ssa_dns_stop_timer() */



/*******************************************************************************
 **
 ** FUNCTION:     icf_ssa_dns_get_ip_from_fqdn
 ** DESCRIPTION:  This function will search for the corresponding IP address for 
 **               the given FQDN. If found p_active_ip_addr will contain the 
 **               current active IP address else failure will be returned.
 **
 ******************************************************************************/
icf_return_t    icf_ssa_dns_get_ip_from_fqdn(
		IN     icf_uint8_t          *p_fqdn,
		OUT    icf_dns_buffer_st    **p_p_dns_record,
		OUT    icf_error_t          *p_error)
{

	icf_list_st   		*p_tmp_dns_buffer_list = ICF_NULL;
	icf_return_t		ret_val = ICF_FAILURE;
	p_error = p_error;

    if (ICF_NULL == p_persistent_glb_pdb)
    {
        ICF_PRINT(((icf_uint8_t *)"\n p_persistent_glb_pdb is NULL \n"))
        return ICF_FAILURE;
    }

    ICF_FUNCTION_ENTER(p_persistent_glb_pdb) /* function entry trace */

    /* SPR 19189 */
	if((ICF_NULL == p_persistent_glb_pdb->p_glb_data) ||
       (ICF_NULL == p_persistent_glb_pdb->p_glb_data->p_dns_info))
	{
        ICF_PRINT(((icf_uint8_t *)"\n No data in the DNS INFO \n"))
        ret_val = ICF_FAILURE;
	}
	
    else
    {
        p_tmp_dns_buffer_list = p_persistent_glb_pdb->p_glb_data->p_dns_info->p_dns_buffer_list;

        if (ICF_NULL == p_tmp_dns_buffer_list)
        {
		    ICF_PRINT(((icf_uint8_t *)"\n No data in the DNS buffer \n"));
            ret_val = ICF_FAILURE;
        }
        else 
        {
            while (p_tmp_dns_buffer_list != ICF_NULL)
            {
                if ((ICF_NULL != p_tmp_dns_buffer_list->p_data) &&
                    (ICF_NULL == icf_port_strcmp(((icf_dns_buffer_st*)p_tmp_dns_buffer_list->p_data)->fqdn.str, \
                            p_fqdn)))
                {
                    *p_p_dns_record = (icf_dns_buffer_st*)p_tmp_dns_buffer_list->p_data;
                    ICF_PRINT(((icf_uint8_t *)"\n DNS record Found \n"));
                    ret_val = ICF_SUCCESS;
                    break; 
                }
                else
                {
                    p_tmp_dns_buffer_list = p_tmp_dns_buffer_list->p_next;
                }  
            }
        }
    }
    /* While searching the particular FQDN , in database search only global
     * buffer and not in SSA context.Following code is commented for the
     * same.This is done as in some scenario like a basic outgoing
     * call IPTK has removed the stored FQDN from global buffer on purge timer
     * expiry but been never deleted from ssa context.So in scenario in which
     * there is purging of DNS_PURGE_TIMER(time for which DNS entry is buffred)
     * IPTK will never do the fresh DNS query for resolving FQDN , as the
     * entry will be found in SSA context.So searching the entry in ssa context
     * will totally defeat the purpose of DNS_PURGE_TIMER*/
#if 0   
    if((ICF_NULL != p_persistent_glb_pdb->p_call_ctx) && 
       (ICF_NULL != p_persistent_glb_pdb->p_call_ctx->p_sip_ctx))
    {
        /* search if the DNS info is stored here */
        if(((icf_ssa_ctxt_st*)p_persistent_glb_pdb->p_call_ctx->p_sip_ctx)->bitmask & ICF_SSA_CONTACT_IP_PRESENT)
        {
            if ((ICF_NULL != ((icf_ssa_ctxt_st*)p_persistent_glb_pdb->p_call_ctx->p_sip_ctx)->p_dns_ip_addr) &&
                (ICF_NULL == icf_port_strcmp(
                                 ((icf_ssa_ctxt_st*)p_persistent_glb_pdb->p_call_ctx->p_sip_ctx)->p_dns_ip_addr->fqdn.str,
                                   p_fqdn)))
            {
                /* found the DNS record for contact address */
                *p_p_dns_record = ((icf_ssa_ctxt_st*)p_persistent_glb_pdb->p_call_ctx->p_sip_ctx)->p_dns_ip_addr;
                ICF_PRINT(((icf_uint8_t *)"\n Found the DNS record for the contact address \n"));
                /*  Once the record is found then the
                    DNS Query should not be done, so 
                    Success should be returned. */
                ret_val = ICF_SUCCESS;
            }
        }
    }
#endif
    ICF_FUNCTION_EXIT(p_persistent_glb_pdb);
	return ret_val;
}


/*******************************************************************************
 **
 ** FUNCTION:     icf_ssa_dns_add_fqdn_to_ip_mapping
 ** DESCRIPTION:  This function will populate the data structures, containing
 **               mapping of each FQDN to multiple IP addresses. SSA will invoke
 **               this function to provide list of IP addresses for the 
 **               corresponding FQDN to DNS buffer.
 **
 ******************************************************************************/
icf_return_t    icf_ssa_dns_add_fqdn_to_ip_mapping(
		IN     icf_dns_buffer_st   *p_query_dns_resp,
        INOUT  icf_ssa_ctxt_st     *p_ssa_ctxt,
        IN     Sdf_st_callObject   *p_call_obj,
		OUT    icf_error_t         *p_error,
        IN	   icf_uint8_t		   dns_map_change_cause)
{
	icf_return_t  			ret_val = ICF_FAILURE;
	icf_return_t  			dummy_ret_val = ICF_FAILURE;
    icf_dns_buffer_st       *p_dns_buffer_to_store = ICF_NULL;
    icf_line_data_st		*p_line_data = ICF_NULL;
    Sdf_ty_s8bit            *p_sip_method = ICF_NULL;
    SipMessage		        *pMessage = Sdf_co_null;
    Sdf_st_error            sdf_error = {0,0,0,"\0"};
    icf_config_data_st      *p_cfg_data = ICF_NULL;
    icf_return_t            return_val = ICF_SUCCESS;

	icf_uint32_t			ttl = ICF_NULL;
	icf_uint16_t			timer_count = ICF_NULL;
    Sdf_st_transaction     	*p_txn = Sdf_co_null;
	Sdf_st_overlapTransInfo	*p_ovtxn = Sdf_co_null;    
    
    ICF_FUNCTION_ENTER(p_persistent_glb_pdb);

    /* SPR 19189 */
    ICF_CHECK_IF_VAR_IS_NULL(p_persistent_glb_pdb,
                             p_query_dns_resp,
                             return_val)
    if(ICF_FAILURE == return_val)
    {
        ret_val = ICF_FAILURE;
        ICF_FUNCTION_EXIT(p_persistent_glb_pdb)
        return ret_val;    
    }

    /* check if there is already an entry corresponding to the FQDN */
    if(ICF_SUCCESS == icf_ssa_dns_get_ip_from_fqdn(
                p_query_dns_resp->fqdn.str,&p_dns_buffer_to_store, p_error))
    {
        /*As a part of merging for CSR-1-6452321 the logic of deleting and
          adding same entry is removed from here*/
        ret_val = ICF_SUCCESS;
        ICF_FUNCTION_EXIT(p_persistent_glb_pdb);
        return ret_val;
    }
    
    if(ICF_FAILURE == icf_dbm_fetch_line_cntxt_blk(
                p_persistent_glb_pdb, p_ssa_ctxt->line_id,
                &p_line_data, p_error))	
    {
        ICF_PRINT(((icf_uint8_t *)"\n[SSA]: p_line_data is NULL "))
        ret_val = ICF_FAILURE;
        ICF_FUNCTION_EXIT(p_persistent_glb_pdb);
        return ret_val;
    }

    ICF_MEMGET(p_persistent_glb_pdb,
            ICF_PORT_SIZEOF(icf_dns_buffer_st),ICF_MEM_COMMON,\
            p_dns_buffer_to_store,\
            ICF_RET_ON_FAILURE,\
            p_error, ret_val);
    /*Klocwork warning removal*/
    ICF_KLOCWORK_FALSE_POSITIVE(p_persistent_glb_pdb,p_dns_buffer_to_store)

    icf_port_memcpy(p_dns_buffer_to_store, p_query_dns_resp,sizeof(icf_dns_buffer_st));
   /* find the method here */
    if((ICF_NULL != p_call_obj->pUasTransaction) &&
       (ICF_NULL != p_call_obj->pUasTransaction->pSipMsg))
    {
        pMessage = p_call_obj->pUasTransaction->pSipMsg;
    }
    else if ((ICF_NULL != p_call_obj->pUacTransaction) &&
             (ICF_NULL != p_call_obj->pUacTransaction->pSipMsg))
    {
        pMessage = p_call_obj->pUacTransaction->pSipMsg;
    }
    
    if(Sdf_co_fail == 
            sdf_ivk_uaGetMethodFromSipMessage(
                pMessage,
                &p_sip_method, &sdf_error))
    {
        ret_val = ICF_FAILURE;
    }

    /***********************/
    /*  Fetch the Config data to check the DHCP Resp */
    if (ICF_FAILURE == icf_dbm_get_module_glb_data(
                p_persistent_glb_pdb,
                (icf_int_module_id_t)ICF_INT_MODULE_CFG,
                (icf_void_t *)&(p_cfg_data),
                p_error))
    {
        ICF_MEMFREE(p_persistent_glb_pdb, 
                    p_dns_buffer_to_store, 
                    ICF_MEM_COMMON, 
                    p_error, ret_val)

        ret_val = ICF_FAILURE;
        ICF_FUNCTION_EXIT(p_persistent_glb_pdb);
        return ret_val;
    }		

    /* The following if condition which make sure that only DNS entry for proxy
       & registrar are buffered has been removed.This is done for the
       following scenario:
       INVITE     -------------->
       IXX        <-------------
       2XX(having contact as domain name other than proxy)   <-------------
       ACK        -------------->
       Call connected
       DNS purge timer expire
       Application send call modify request
       REINVITE   -------------->
       401        <-------------
       At this point due to following check the dns response will not be
       buffered.As a result for sending ACK IPTK will initiate a fresh DNS
       query. Now this is not desireable because for Re-Invite flow IPTK need
       to send the ACK and new INVITE with auth params in the same leg.If this
       not happens IPTK clears the call.
       */
#if 0    
    if( p_sip_method && (((p_line_data->line_data_received & ICF_CFG_LINE_DATA_PROXY_RECD) &&
        (p_line_data->proxy_addr.addr.addr_type == ICF_TRANSPORT_ADDRESS_DOMAIN_NAME) &&
        (ICF_NULL == icf_port_strcmp(p_line_data->proxy_addr.addr.addr.domain.str,
                                     p_query_dns_resp->fqdn.str))) || 

       ((p_line_data->line_data_received & ICF_CFG_LINE_DATA_SEC_PROXY_RECD) &&
       (ICF_NULL != p_line_data->p_sec_proxy_addr) &&
       (p_line_data->p_sec_proxy_addr->addr.addr_type == ICF_TRANSPORT_ADDRESS_DOMAIN_NAME) &&
       (ICF_NULL == icf_port_strcmp(p_line_data->p_sec_proxy_addr->addr.addr.domain.str,
                                    p_query_dns_resp->fqdn.str))) ||
       
       /*  Check for DHCP REsponse */
       ((ICF_NULL != p_cfg_data->p_dhcp_resp) && 
       ((ICF_NULL == icf_port_strcmp(p_cfg_data->p_dhcp_resp->proxy_list[0].str,p_query_dns_resp->fqdn.str)) ||
       ((ICF_NULL != p_cfg_data->p_dhcp_resp->proxy_list[1].str_len) && 
       (icf_port_strcmp(p_cfg_data->p_dhcp_resp->proxy_list[0].str,p_query_dns_resp->fqdn.str))))) ||

       icf_port_strcmp(p_sip_method,"REGISTER") == ICF_NULL))
    {
#endif
    /* SPR 19189 */
    ICF_CHECK_IF_VAR_IS_NULL(p_persistent_glb_pdb,
            p_persistent_glb_pdb->p_glb_data,
            return_val)
    if(ICF_FAILURE == return_val)
    {
        ret_val = ICF_FAILURE;
        ICF_FUNCTION_EXIT(p_persistent_glb_pdb)
        return ret_val;    
    } 

    /* Get the transaction */
    /*SPR 19503 We should use UATK API instead of calling the function
      icf_ssa_get_txn_from_call_obj for fetching the Txn. The problem
      was that icf_ssa_get_txn_from_call_obj does not fetch overlap
      txn when PRACK is sent by IPTK and 200OK for PRACK is received.
      Previously the reliable 180 has the domain name so IPTK does
      query for the PRACK. Now when we go inside icf_ssa_get_ttl_for_dns_record_from_txn
      we try to fetch TTL value from UAC txn and as initially proxy was not 
     configured to there was no TTL value in pDestInfo->pIp and thus we get
     ABR*/
    if(Sdf_co_fail == sdf_ivk_uaGetTransactionForSipMsg(\
                  (p_ssa_ctxt->p_call_obj), pMessage, &p_txn, \
                  &p_ovtxn, &sdf_error))
    {
        ICF_PRINT(((icf_uint8_t*)"\n[SSA]: Failed to get txn\n"));
        ret_val = ICF_FAILURE;
    }

    /* ----------------------------------------------------------------- */
    /* Fetch TTL value that was received in DNS response for this IP */
    /* ----------------------------------------------------------------- */
    else if (ICF_FAILURE == icf_ssa_get_ttl_for_dns_record_from_txn(
                p_query_dns_resp, p_txn, p_ovtxn, &sdf_error, &ttl))
    {
        ICF_PRINT(((icf_uint8_t*)"\n[SSA]: Failed to get TTL from txn\n"));
        ret_val = ICF_FAILURE;
    }
    else 
    {
        /* ----------------------------------------------------------------- */
        /* Calculate the DNS buffer purge count for this entry */
        /* Formula for the counter that needs to be stored for each entry is:
         *	(Smaller of TTL and DNS_PURGE_TIMER)/PRECISION_VALUE
         *
         * Units: dns_purge_timer is stored in minutes
         * Units: ICF_DNS_PURGE_TIMER_PRECISION_VALUE is stored in minutes
         * Units: ttl is stored in seconds
         * Units: actual_duration is stored in seconds
         */
        /* ----------------------------------------------------------------- */

        /* Fix for SPR: 19999 (Merged CSR-1-7570808)*/
		/* Fix for CSR 7570808:SPR 19892 - Resetting the value of ttl if it is very small 
			i.e if it is less than ICF_DNS_PURGE_TIMER_PRECISION_VALUE we reset 
			it so that it should be at least equal to ICF_DNS_PURGE_TIMER_PRECISION
			_VALUE.The timer count will be 1 when ttl or dns_purge_timer is less than or
			equql to ICF_DNS_PURGE_TIMER_PRECISION_VALUE */
			
		if(ttl < (60 * ICF_DNS_PURGE_TIMER_PRECISION_VALUE))  
        {
			ttl = 60 * ICF_DNS_PURGE_TIMER_PRECISION_VALUE;
            timer_count = 1;
        }
		else if (p_cfg_data->dns_purge_timer < ICF_DNS_PURGE_TIMER_PRECISION_VALUE)
		{
			timer_count = 1;
		}
        else if (ttl < (60 * p_cfg_data->dns_purge_timer))
        {
            timer_count = (icf_uint16_t)\
                          (ttl / (60 * ICF_DNS_PURGE_TIMER_PRECISION_VALUE));
        }
        else
        {
            timer_count = (icf_uint16_t)\
                          (p_cfg_data->dns_purge_timer/ICF_DNS_PURGE_TIMER_PRECISION_VALUE);
        }

        /* ----------------------------------------------------------------- */
        /* Do not store this mapping if:
         * 	(a) timer_count is 0, or
         * 	(b) TTL in DNS response is less than the precision timer duration
         */
        /* ----------------------------------------------------------------- */

        if ((ICF_NULL == timer_count) ||
                (ttl < 60 * ICF_DNS_PURGE_TIMER_PRECISION_VALUE))
        {
            ICF_PRINT(((icf_uint8_t*)"\n[SSA]: Purge duration too short, entry not stored in"\
                        " DNS buffer for:\n\tfqdn = %s \n\tip = %s"\
                        "\n\tport = %d \n\tttl = %d sec\n\n",
                        p_query_dns_resp->fqdn.str,
                        p_query_dns_resp->ip_address,
                        p_query_dns_resp->port,
                        ttl));
            ret_val = ICF_SUCCESS;
            ICF_MEMFREE(p_persistent_glb_pdb, 
                    p_dns_buffer_to_store, 
                    ICF_MEM_COMMON, 
                    p_error, ret_val)
            ICF_FUNCTION_EXIT(p_persistent_glb_pdb)
            return ret_val;
        }

        /* ----------------------------------------------------------------- */
        /* If the timer_count for this mapping is not 0:
         *		(2.a) Start purge timer if not already running and this will
         *			  happen if DNS buffer is currently empty.
         *		(2.b) Store this entry in the DNS buffer and this may
         *			  be the 1st entry in the DNS buffer.
         */
        /* ----------------------------------------------------------------- */

        /* Start the DNS Purge timer here */
        if ((ICF_NULL == p_persistent_glb_pdb->p_glb_data->p_dns_info) || 
                (ICF_NULL == p_persistent_glb_pdb->p_glb_data->p_dns_info->\
                 p_dns_buffer_list))
        {
            /* CSR 1-6452321: SPR 19216 */
            /* DNS purge timer duration = DNS_PURGE_TIMER_PRECISION_VALUE */
            /* Below 60000 is put to convert the timer from minute to ms */
            if (ICF_FAILURE == icf_ssa_dns_start_timer(
                        p_persistent_glb_pdb,
                        (60000 * ICF_DNS_PURGE_TIMER_PRECISION_VALUE), 
                        p_error))
            {
                ret_val = ICF_FAILURE;
                ICF_MEMFREE(p_persistent_glb_pdb, 
                        p_dns_buffer_to_store, 
                        ICF_MEM_COMMON, 
                        p_error, ret_val)                    
                ICF_FUNCTION_EXIT(p_persistent_glb_pdb)
                return ret_val;
            }
        }
    }

    if (ICF_FAILURE == ret_val)
    {
        ICF_MEMFREE(p_persistent_glb_pdb, 
                p_dns_buffer_to_store, 
                ICF_MEM_COMMON, 
                p_error, ret_val)
        return ICF_FAILURE;
    }

    /* SPR 19189 */
    ICF_CHECK_IF_VAR_IS_NULL(p_persistent_glb_pdb,
            p_persistent_glb_pdb->p_glb_data->p_dns_info,
            return_val)

    if(ICF_FAILURE == return_val)
    {
        ret_val = ICF_FAILURE;
        ICF_FUNCTION_EXIT(p_persistent_glb_pdb)
        return ret_val;    
    }

    p_dns_buffer_to_store->timer_count = timer_count;
    /* CSR 1-6669377: SPR 19245 */
    p_dns_buffer_to_store->ttl = ttl;

    ICF_PRINT(((icf_uint8_t*)"\n[SSA]: Storing the following entry in DNS buffer:\n"));
    ICF_PRINT(((icf_uint8_t*)"\tfqdn = %s\n", p_dns_buffer_to_store->fqdn.str));
    ICF_PRINT(((icf_uint8_t*)"\tip = %s\n", p_dns_buffer_to_store->ip_address));
    ICF_PRINT(((icf_uint8_t*)"\tport = %d\n", p_dns_buffer_to_store->port));
    ICF_PRINT(((icf_uint8_t*)"\tttl = %d sec\n", ttl));
    ICF_PRINT(((icf_uint8_t*)"\tconfig dns purge timer = %d mins\n", p_cfg_data->dns_purge_timer));
    ICF_PRINT(((icf_uint8_t*)"\ttimer_count = %d\n", p_dns_buffer_to_store->timer_count));
    if (0 != p_dns_buffer_to_store->naptr_fqdn.str_len)
    {
        ICF_PRINT(((icf_uint8_t*)"\tfqdn for NAPTR query = %s\n", p_dns_buffer_to_store->\
                    naptr_fqdn.str));
    }
    if (0 != p_dns_buffer_to_store->srv_fqdn.str_len)
    {
        ICF_PRINT(((icf_uint8_t*)"\tfqdn for SRV query = %s\n", p_dns_buffer_to_store->\
                    srv_fqdn.str));
    }
    /* Fix For SPR:20192*/
    if(0 != p_dns_buffer_to_store->queryA_fqdn.str_len)
    {
        ICF_PRINT(((icf_uint8_t*)"\tfqdn for A query = %s\n", p_dns_buffer_to_store->\
                    queryA_fqdn.str));
    }    

    if (ICF_FAILURE != icf_cmn_add_node_to_list(p_persistent_glb_pdb,\
                    &(p_persistent_glb_pdb->p_glb_data->p_dns_info->p_dns_buffer_list),\
                    p_dns_buffer_to_store))
    {
        /* SPR 19027: Send DNS mapping change indication to App */
        ret_val = icf_ssa_send_dns_map_change_ind(
                p_persistent_glb_pdb,
                p_dns_buffer_to_store->fqdn.str,
                p_dns_buffer_to_store->ip_address,
                dns_map_change_cause);            
    }
    else
    {
        ICF_MEMFREE(p_persistent_glb_pdb, 
                p_dns_buffer_to_store, 
                ICF_MEM_COMMON, 
                p_error, ret_val)
        ret_val = ICF_FAILURE;
        ICF_PRINT(((icf_uint8_t*)"\n[SSA]: Failure while adding entry in DNS buffer:\n"));
        ICF_FUNCTION_EXIT(p_persistent_glb_pdb)
        return ret_val;          
    }

    if (ICF_NULL != p_ssa_ctxt->p_dns_ip_addr)
    {
        ICF_MEMFREE(p_persistent_glb_pdb, 
                p_ssa_ctxt->p_dns_ip_addr, 
                ICF_MEM_COMMON, 
                p_error,dummy_ret_val)
    }

    ICF_MEMGET(p_persistent_glb_pdb,
            ICF_PORT_SIZEOF(icf_dns_buffer_st),ICF_MEM_COMMON,\
            p_ssa_ctxt->p_dns_ip_addr,\
            ICF_DONOT_RET_ON_FAILURE,\
            p_error,dummy_ret_val);

    if(p_dns_buffer_to_store && (ICF_SUCCESS == dummy_ret_val))
    {
        icf_port_memcpy(p_ssa_ctxt->p_dns_ip_addr,
                p_dns_buffer_to_store, sizeof(icf_dns_buffer_st));
        p_ssa_ctxt->bitmask |= ICF_SSA_CONTACT_IP_PRESENT;
    }
    /*SPR 19503 As the sdf_ivk_uaGetTransactionFromSipMsg is invoked
      so to decrement the ref count use the free transaction API */
    if(Sdf_co_null != p_txn)
    {
         sdf_ivk_uaFreeTransaction(p_txn);
    }
    if(Sdf_co_null != p_ovtxn)
    {
         sdf_ivk_uaFreeOverlapTransInfo(p_ovtxn);
    }
    ICF_FUNCTION_EXIT(p_persistent_glb_pdb);
	return ret_val;
}

/*******************************************************************************
 **
 ** FUNCTION:     	icf_ssa_get_ttl_for_dns_record_from_txn
 ** DESCRIPTION:  	This function will return the TTL value stored along with
 **					the IP address in destination info of transaction.
 **
 ******************************************************************************/
icf_return_t	icf_ssa_get_ttl_for_dns_record_from_txn(
		IN     	icf_dns_buffer_st  		*p_query_dns_resp,
		IN		Sdf_st_transaction		*p_txn,
		IN		Sdf_st_overlapTransInfo	*p_ovtxn,
		INOUT	Sdf_st_error			*p_sdf_error,
		OUT		icf_uint32_t			*p_ttl_val)
{
	icf_return_t			ret_val = ICF_SUCCESS;
	icf_error_t				err = ICF_ERROR_NONE;
    icf_config_data_st      *p_cfg_data = ICF_NULL;
    icf_uint16_t            ip_len = 0;
	icf_uint32_t			ttl = ICF_NULL;
    Sdf_ty_u8bit            *p_ttl = Sdf_co_null;
    Sdf_ty_pvoid            p_ip = Sdf_co_null;

	ICF_FUNCTION_ENTER(p_persistent_glb_pdb)

	/* warning removal */
	p_sdf_error = p_sdf_error;

	/* Validate the input arguments */
	if ((ICF_NULL == p_query_dns_resp) || (ICF_NULL == p_ttl_val))
	{
		ICF_PRINT(((icf_uint8_t*)"\n[SSA]: Incorrect ttl or dns buffer link\n"));
		ret_val = ICF_FAILURE;
	}
	else if ((ICF_NULL == p_txn) && (ICF_NULL == p_ovtxn))
	{
		ICF_PRINT(((icf_uint8_t*)"\n[SSA]: Incorrect txn or overlap txn link\n"));
		ret_val = ICF_FAILURE;
	}
	/* Get the config data */   
    else if (ICF_FAILURE == icf_dbm_get_module_glb_data(
                        p_persistent_glb_pdb,
                        (icf_int_module_id_t)ICF_INT_MODULE_CFG,
                        (icf_void_t *)&(p_cfg_data),
                        &err))
    {
		ICF_PRINT(((icf_uint8_t*)"\n[SSA]: Failed to get config data\n"));
		ret_val = ICF_FAILURE;
    }
	else if ((Sdf_co_null != p_ovtxn) && (Sdf_co_null != p_ovtxn->pDestInfo) &&
			 (Sdf_co_null != p_ovtxn->pDestInfo->pIp))
	{
		ICF_PRINT(((icf_uint8_t*)"\n[SSA]: Fetching ttl for overlap txn\n"));
		p_ip = p_ovtxn->pDestInfo->pIp;
	}
	else if ((Sdf_co_null != p_txn) && (Sdf_co_null != p_txn->pDestInfo) &&
			 (Sdf_co_null != p_txn->pDestInfo->pIp))
	{
		ICF_PRINT(((icf_uint8_t*)"\n[SSA]: Fetching ttl for uac txn\n"));
		p_ip = p_txn->pDestInfo->pIp;
	}
	else
	{
		ICF_PRINT(((icf_uint8_t*)"\n[SSA]: Failed to get ttl for IP\n"));
		ret_val = ICF_FAILURE;
	}

    if (ICF_FAILURE == ret_val)
	{
		ret_val = ICF_FAILURE;
	}
	else if (ICF_NULL == icf_port_strcmp(
							p_query_dns_resp->ip_address,
                            (Sdf_ty_u8bit *)p_ip))
	{
    	ip_len = icf_port_strlen((Sdf_ty_u8bit *)p_ip);
    	p_ttl = (((Sdf_ty_u8bit *)p_ip) + ip_len + 1);
    	(ttl) = (icf_uint32_t)((p_ttl)[0]);
    	(ttl) |= (icf_uint32_t)((p_ttl)[1] << 8);
    	(ttl) |= (icf_uint32_t)((p_ttl)[2] << 16);
    	(ttl) |= (icf_uint32_t)((p_ttl)[3] << 24);
		*p_ttl_val = ttl;
	}
	else
	{
		ICF_PRINT(((icf_uint8_t*)"\n[SSA]: Failed to match resolved IP with that in txn\n"));
		ret_val = ICF_FAILURE;
	}

	ICF_FUNCTION_EXIT(p_persistent_glb_pdb)
	return ret_val;
} /* End function icf_ssa_get_ttl_for_dns_record_from_txn() */


/*******************************************************************************
 **
 ** FUNCTION:     icf_ssa_dns_delete_fqdn_entry
 ** DESCRIPTION:  This function will receive the pointer of the node record to be deleted. 
 **
 ******************************************************************************/
icf_return_t    icf_ssa_dns_delete_fqdn_entry(
		IN     icf_string_st     	 *p_fqdn,
		IN     icf_string_st     	 *p_naptr_fqdn,
		IN     icf_string_st     	 *p_srv_fqdn,
        IN     icf_string_st     	 *p_queryA_fqdn,        
		OUT    icf_error_t           *p_error,
        IN	   icf_uint8_t			 dns_map_change_cause)
{
	icf_return_t 		ret_val = ICF_FAILURE;
	icf_list_st   		*p_tmp_list = ICF_NULL;

    if (ICF_NULL == p_persistent_glb_pdb)
    {
        return ICF_FAILURE;
    }
    ICF_FUNCTION_ENTER(p_persistent_glb_pdb);
	
	if ((ICF_NULL == p_persistent_glb_pdb->p_glb_data) ||
        (ICF_NULL == p_persistent_glb_pdb->p_glb_data->p_dns_info))
    {
        ICF_PRINT(((icf_uint8_t *)"\n No data in the DNS INFO \n"))
        return ICF_FAILURE;
    }
    if(ICF_NULL == p_persistent_glb_pdb->p_glb_data->p_dns_info->p_dns_buffer_list)
    { 
        ICF_PRINT(((icf_uint8_t *)"\n No data in p_dns_buffer_list\n"))  
        return ICF_SUCCESS;
    }
    p_tmp_list = p_persistent_glb_pdb->p_glb_data->p_dns_info->p_dns_buffer_list ;
	while( p_tmp_list != ICF_NULL)
	{  
		if(0 == icf_port_strcmp(((icf_dns_buffer_st*)(p_tmp_list->p_data))->fqdn.str, p_fqdn->str))
		{
			break;
		}
		else
		{
			p_tmp_list = p_tmp_list->p_next;
		}
	}	

    if(ICF_NULL != p_tmp_list)
    {
        icf_dns_buffer_st       *p_temp_data = p_tmp_list->p_data;
        /* Rel 8.2 Klocworks fix */
        icf_string_st           temp_str_to_print = {0,"\0"};
        /* Rel 8.2 Klocworks fix end */
        /* Pointer to global port info*/
        icf_global_port_info_st     *p_glb_port_info = ICF_NULL;
        temp_str_to_print = *p_fqdn;
                
        if(ICF_FAILURE != icf_cmn_delete_node_frm_list(p_persistent_glb_pdb, \
                    &(p_persistent_glb_pdb->p_glb_data->p_dns_info->p_dns_buffer_list), 
		           &(p_tmp_list->p_data)))
        {
            if (ICF_NULL != p_persistent_glb_pdb->p_glb_data->p_glb_port_info)
            {
                p_glb_port_info =(icf_global_port_info_st *)(p_persistent_glb_pdb->\
                        p_glb_data->p_glb_port_info);
                
                p_glb_port_info = p_persistent_glb_pdb->p_glb_data->p_glb_port_info;

                /* After successfully deleting the DNS buffer from IPTK remove
                 * the same entry from DNS CLIENT*/
                /* Additionally if any SRV for NAPTR query is performed clear
                 * the same from DNS CLIENT Cache.*/
                /* Fix For SPR:20192*/
                /* For clearing the A query domain name new parameter
                 * p_queryA_fqdn is passed which is the one given in response
                 * by DNS server. DNS CLIENT buffer the request with this
                 * domain name.*/
                if (ICF_FAILURE ==
                        icf_port_clear_dns_client_cache(p_glb_port_info,\
                            (icf_string_st *)p_queryA_fqdn,(icf_string_st *)p_naptr_fqdn,\
                            (icf_string_st *)p_srv_fqdn))
                {
                    ICF_PRINT(((icf_uint8_t *)"\n DNS Entry cannot be deleted: %s \n",\
                                temp_str_to_print.str));

                }
                else
                {
                    ICF_PRINT(((icf_uint8_t *)"\n DNS Entry Deleted Successfully : %s \n",\
                                temp_str_to_print.str));
                }
            }
            if(ICF_NULL != p_temp_data)
            {
                ICF_MEMFREE(p_persistent_glb_pdb, p_temp_data, ICF_MEM_COMMON, p_error, ret_val)
            }
            /* SPR 19027: Send DNS mapping change indication to App */
            ret_val = icf_ssa_send_dns_map_change_ind(
						p_persistent_glb_pdb,
                       	temp_str_to_print.str,
						ICF_NULL,
						dns_map_change_cause);            
         }
    }

	if(ICF_NULL == p_persistent_glb_pdb->p_glb_data->p_dns_info->p_dns_buffer_list)
	{
		icf_ssa_dns_stop_timer(p_persistent_glb_pdb, p_error);

	}
    ICF_FUNCTION_EXIT(p_persistent_glb_pdb);
	return ret_val;
}


     
/*******************************************************************************
 **
 ** FUNCTION:     icf_ssa_dns_purge_timer_expire
 ** DESCRIPTION:  This is the call back function and will be registered by the DNS 
 ** buffer to the genlib timer manager as soon as there comes an entry in the DNS 
 ** buffer. This function will decrement the timer_count in each DNS record.
 **
 ******************************************************************************/
icf_return_t    icf_ssa_dns_purge_timer_expire(
		INOUT	icf_glb_pdb_st   *p_glb_pdb,
		OUT	icf_error_t	 *p_error)	
{

	icf_return_t 			ret_val = ICF_SUCCESS;
	icf_list_st   			*p_tmp_list = ICF_NULL;
	icf_dns_buffer_st  		*p_tmp_node = ICF_NULL;
    icf_config_data_st      *p_cfg_data = ICF_NULL;
    icf_return_t            return_val = ICF_SUCCESS;
    icf_ssa_timer_data_st   *p_ssa_timer_data = ICF_NULL;
    /* CSR 1-6669377: SPR 19245 */
	icf_uint8_t				dns_map_change_cause = ICF_DNS_MAP_CHANGE_INVALID;

    /* SPR 18880 : Added Null checks */
    if(ICF_NULL == p_glb_pdb)
    {
        ICF_PRINT(((icf_uint8_t *)("\n[SSA] p_glb_pdb is NULL\n.")))
        return ICF_FAILURE;
    }

    ICF_FUNCTION_ENTER(p_glb_pdb);

    ICF_CHECK_IF_ANY_OF_TWO_VAR_IS_NULL(p_glb_pdb,
                                        p_glb_pdb->p_glb_data,
                                        p_glb_pdb->p_glb_data->p_dns_info,
                                        return_val)
    if(ICF_FAILURE == return_val)
    {
        ret_val = ICF_FAILURE;
        ICF_FUNCTION_EXIT(p_glb_pdb)
        return ICF_FAILURE;
    }

    if (ICF_FAILURE == icf_dbm_get_module_glb_data(
                p_glb_pdb,
                (icf_int_module_id_t)ICF_INT_MODULE_CFG,
                (icf_void_t *)&(p_cfg_data),
                p_error))
    {
        ICF_PRINT(((icf_uint8_t *)("\n[SSA] p_cfg_data is NULL\n.")))
            ICF_FUNCTION_EXIT(p_persistent_glb_pdb);
        return ICF_FAILURE;
    }

	p_tmp_list = p_glb_pdb->p_glb_data->p_dns_info->p_dns_buffer_list;

    /*Mark timer-ID as invalid, as timer is already expired. This is done so that
     *SSA does not try to stop this try in later processing*/
    p_glb_pdb->p_glb_data->p_dns_info->dns_buffer_timer_id = ICF_TIMER_ID_INVALID;

	/* CSR 1-6452321: SPR 19216 */
	ICF_PRINT(((icf_uint8_t*)"\n[SSA]: Scanning DNS buffer on precision timer expiry....\n"));    

	while (p_tmp_list != ICF_NULL)
	{
		p_tmp_node = p_tmp_list->p_data;

        /* SPR 18880 : Added Null check */
        ICF_CHECK_IF_VAR_IS_NULL(p_glb_pdb,
                                 p_tmp_list->p_data,
                                 return_val)
        if(ICF_FAILURE == return_val)
        {
            ret_val = ICF_FAILURE;
            ICF_FUNCTION_EXIT(p_glb_pdb)
            return ICF_FAILURE;
        }

		((icf_dns_buffer_st*)(p_tmp_list->p_data))->timer_count--;

		ICF_PRINT(((icf_uint8_t*)"\n\tfqdn = %s\n", 
			((icf_dns_buffer_st*)(p_tmp_list->p_data))->fqdn.str));
		ICF_PRINT(((icf_uint8_t*)"\t\tip = %s\n", 
			((icf_dns_buffer_st*)(p_tmp_list->p_data))->ip_address));
		ICF_PRINT(((icf_uint8_t*)"\t\tport = %d\n", 
			((icf_dns_buffer_st*)(p_tmp_list->p_data))->port));
		ICF_PRINT(((icf_uint8_t*)"\t\ttimer_count = %d\n", 
			((icf_dns_buffer_st*)(p_tmp_list->p_data))->timer_count));

		if(((icf_dns_buffer_st*)(p_tmp_list->p_data))->timer_count <= 0)
		{
			p_tmp_list = p_tmp_list->p_next;

            /* CSR 1-6669377: SPR 19245 */
			/* Check whether this purge is on TTL expiry or on configured timer
			 * expiry
			 */
			if (p_tmp_node->ttl < (60 * p_cfg_data->dns_purge_timer))
        	{
				dns_map_change_cause = ICF_DNS_MAP_CHANGE_TTL_EXPIRY;
			}
			else
			{
				dns_map_change_cause = ICF_DNS_MAP_CHANGE_PURGE_EXPIRY;
			}
            /* Fix For SPR:20192*/
            if(ICF_FAILURE ==  icf_ssa_dns_delete_fqdn_entry(&p_tmp_node->fqdn,\
						   &p_tmp_node->naptr_fqdn,\
                           &p_tmp_node->srv_fqdn,&p_tmp_node->queryA_fqdn,\
                           p_error,(icf_uint8_t)dns_map_change_cause))
			{
				ret_val = ICF_FAILURE;
			}
		}
        else
        {
            p_tmp_list = p_tmp_list->p_next;
        }
	}

    /*SPR 19223 removed null check for  p_glb_pdb->p_glb_data->p_dns_info
      as top level null check takes care of it and also it is used in above
      code leg */

	/* free the timer memory here for the expired timer */
    /* check for DNS Timer List has been moved as this code block should
     * free the expired timer data. In case there are more entries in the
     * DNS timer list then new timer is started and new timer block shall
     * be allocated.
     */ 
    if(ICF_NULL != p_glb_pdb->p_glb_data->p_dns_info->p_dns_timer_data)
    {
		p_ssa_timer_data = (icf_ssa_timer_data_st *)((icf_timer_data_st*)p_glb_pdb->p_glb_data->p_dns_info->
							p_dns_timer_data)->p_timer_buffer;  
		ICF_SSA_RESET_SSA_TIMER_BLK(p_ssa_timer_data)
        /*Free the allocated ssa timer block here*/
        ret_val = icf_dbm_ret_mem_to_pool(p_glb_pdb, ICF_MEM_SSA_TIMER,
                ((icf_timer_data_st*)
                 p_glb_pdb->p_glb_data->p_dns_info->p_dns_timer_data)->p_timer_buffer, 
                p_error);

        if(ICF_SUCCESS == ret_val)
        {
            ((icf_timer_data_st*)
             p_glb_pdb->p_glb_data->p_dns_info->p_dns_timer_data)->p_timer_buffer = ICF_NULL;
        }
        /* Timer block freeing failure. Raise a alarm and proceed. */
        else
        {
            ICF_PRINT(((icf_uint8_t *)"[SSA]: Failure in freeing timer block.\n"))
        }

        /*Free the allocated timer block here*/
        ret_val = icf_dbm_ret_mem_to_pool(p_glb_pdb, ICF_MEM_TIMER,
                p_glb_pdb->p_glb_data->p_dns_info->p_dns_timer_data, p_error);
        if(ICF_SUCCESS == ret_val)
        {
            p_glb_pdb->p_glb_data->p_dns_info->p_dns_timer_data = ICF_NULL;
        }
        /* Timer block freeing failure. Raise a alarm and proceed. */
        else
        {
            ICF_PRINT(((icf_uint8_t *)"[SSA]: Failure in freeing timer block.\n"))
        }
    }

    /* in case there are more entries in the DNS buffer list then
     * start the DNS Purge timer here */
    if((ICF_SUCCESS == ret_val) && 
            (ICF_NULL !=  p_glb_pdb->p_glb_data->p_dns_info->p_dns_buffer_list))
	{

				
		/* CSR 1-6452321: SPR 19216 */
		/* DNS purge timer duration = DNS_PURGE_TIMER_PRECISION_VALUE */
		/* 60000 is put here to convert the timer from minute to ms */
        if(p_cfg_data == ICF_NULL || (ICF_FAILURE == (icf_ssa_dns_start_timer(
						p_glb_pdb,
						(60000 * ICF_DNS_PURGE_TIMER_PRECISION_VALUE),
						p_error))))
		{
            ICF_FUNCTION_EXIT(p_persistent_glb_pdb);
			return ICF_FAILURE;
		}

	}
    ICF_FUNCTION_EXIT(p_persistent_glb_pdb);
	return ret_val;


} 

/*******************************************************************************
 **
 ** FUNCTION:     icf_ssa_dns_2_pow
 ** DESCRIPTION:  Utility function which returns the timer value corresponding to 
 **               retries 
 ******************************************************************************/

icf_uint16_t
icf_ssa_dns_2_pow(icf_uint8_t  pow_of_2_for_timer)
{
    icf_uint16_t    ret_val = 1;

    ICF_FUNCTION_ENTER(p_persistent_glb_pdb);

    switch(pow_of_2_for_timer)
    {
        case 1:
            ret_val =  1;
            break;
        case 2:
            ret_val  = 2;
            break;
        case 3:
            ret_val = 4;
            break;
        case 4 :
            ret_val = 8;
            break;
        case 5:
            ret_val = 16;
            break;
        case 6:
            ret_val = 32;
            break;
        case 7:
            ret_val = 64;
            break;
        default :
            ret_val = 64;
    }
    ICF_FUNCTION_EXIT(p_persistent_glb_pdb);
    return ret_val;
}

/*******************************************************************************
 **
 ** FUNCTION:     icf_ssa_dns_deinit_dns_buffer
 ** DESCRIPTION:  This function will be registered by the DNS delete all the
 **               Entreis in the DNS buffer and the DNS purge timer will be stopped
 **               automatically
 **
 ******************************************************************************/
icf_return_t    icf_ssa_dns_deinit_dns_buffer(
		        INOUT	icf_glb_pdb_st   *p_glb_pdb)
{
    icf_list_st   		*p_tmp_dns_buffer_list = ICF_NULL,*p_next_node = ICF_NULL;
	icf_return_t		ret_val = ICF_FAILURE;
    icf_error_t         error = ICF_ERROR_NONE;
    icf_return_t        return_val = ICF_SUCCESS;

    /* SPR 19189 : Added Null check */
    ICF_CHECK_IF_ANY_OF_TWO_VAR_IS_NULL(p_glb_pdb,
                                        p_glb_pdb->p_glb_data,
                                        p_glb_pdb->p_glb_data->p_dns_info,
                                        return_val)
    if(ICF_FAILURE == return_val)
    {
        ret_val = ICF_FAILURE;
        ICF_FUNCTION_EXIT(p_glb_pdb)
        return ret_val;
    }

    /*get the pointer to the DNS Buffer list */
    p_tmp_dns_buffer_list = p_glb_pdb->p_glb_data->p_dns_info->p_dns_buffer_list;
    while(ICF_NULL != p_tmp_dns_buffer_list)
    {
        /* call the delete entry function to delete all the entries one by one */
        p_next_node = p_tmp_dns_buffer_list->p_next;
        /* Fix For SPR:20192*/
        ret_val = icf_ssa_dns_delete_fqdn_entry(&(((icf_dns_buffer_st*)(p_tmp_dns_buffer_list->p_data))->fqdn),
                    &(((icf_dns_buffer_st*)(p_tmp_dns_buffer_list->p_data))->naptr_fqdn),
                    &(((icf_dns_buffer_st*)(p_tmp_dns_buffer_list->p_data))->srv_fqdn),
                    &(((icf_dns_buffer_st*)(p_tmp_dns_buffer_list->p_data))->queryA_fqdn), 
                    &error, (icf_uint8_t)ICF_DNS_MAP_CHANGE_SYSTEM_REINIT);
        p_tmp_dns_buffer_list = p_next_node;

    }

    return ret_val;

}
/***************************************************************************
 ** FUNCTION:           icf_ssa_dns_start_dns_resp_timer
 **
 ** DESCRIPTION:        This function will be used to start
 **                     the dns resp timer i.e. this timer will be started 
 **                     for the duration upto which we will wait for DNS resp
 **                     from DNS server. 
 ***************************************************************************/
icf_return_t icf_ssa_dns_start_dns_resp_timer(
		INOUT icf_ssa_ctxt_st *p_ssa_ctxt,
                IN  Sdf_st_dnsEventContext *p_dns_evt_ctxt,
		INOUT  icf_duration_t   timer_dur,
		OUT icf_error_t      *p_ecode)
{
	icf_timer_data_st           *p_timer_data = ICF_NULL;
  icf_timer_id_t              timer_id = ICF_TIMER_ID_INVALID;
  icf_ssa_timer_data_st       *p_ssa_timer_data = ICF_NULL;
	icf_return_t                ret_val = ICF_SUCCESS;

   icf_config_data_st          *p_cfg_data = ICF_NULL;
	ICF_FUNCTION_ENTER(p_ssa_ctxt->p_glb_pdb) /* function entry trace */

  if (ICF_NULL == p_ssa_ctxt->dns_resp_timer_id)
  { 
   if(ICF_FAILURE == icf_dbm_get_module_glb_data(
                            p_ssa_ctxt->p_glb_pdb,
                            ICF_INT_MODULE_CFG,
                            (icf_void_t *)(&p_cfg_data),
                            p_ecode))
    {
         ICF_PRINT(((icf_uint8_t *)"\n[SSA]:Failure in GLB Cfg data retreival!!!"));
         return ICF_FAILURE;
    }
   else if(ICF_FAILURE == icf_dbm_get_mem_from_pool(
                p_ssa_ctxt->p_glb_pdb, ICF_MEM_TIMER,
                (icf_void_t **)&p_timer_data, 
                p_ecode))
    {
        ret_val = ICF_FAILURE;
    }
    else if ( ICF_FAILURE == icf_dbm_get_mem_from_pool(\
                p_ssa_ctxt->p_glb_pdb, ICF_MEM_SSA_TIMER,\
                (icf_void_t **)&p_ssa_timer_data,p_ecode))
    {
        ret_val = ICF_FAILURE;
        icf_dbm_ret_mem_to_pool(p_ssa_ctxt->p_glb_pdb, ICF_MEM_TIMER,
                p_timer_data, p_ecode);
    }
    else
    {    
        /* fill the timer data structure */
        p_timer_data->timer_type = ICF_TIMER_TYPE_DNS_RESP_TIMER;
        p_timer_data->module_id = ICF_INT_MODULE_SSA;
        p_timer_data->p_timer_buffer = p_ssa_timer_data;
        p_ssa_timer_data->p_ssa_ctxt = p_ssa_ctxt;
        p_ssa_timer_data->p_uatk_ctxt_info =(icf_void_t*) p_dns_evt_ctxt;

        /*Klocwork warning removal*/
        ICF_KLOCWORK_FALSE_POSITIVE(p_ssa_ctxt->p_glb_pdb,p_timer_data)
        ICF_KLOCWORK_FALSE_POSITIVE(p_ssa_ctxt->p_glb_pdb,p_ssa_timer_data)

        /*Calculate the DNS timer value*/
        if(ICF_NULL == p_ssa_ctxt->dns_retrans_timer)
        {
            p_ssa_ctxt->dns_retrans_timer = p_cfg_data->dns_min_retrans;
        }
        
        timer_dur = p_ssa_ctxt->dns_retrans_timer;
        p_ssa_ctxt->dns_retrans_timer = 2 * p_ssa_ctxt->dns_retrans_timer;
 
        p_timer_data->p_glb_pdb = p_ssa_ctxt->p_glb_pdb;
        p_ssa_ctxt->p_dns_resp_timer_data = p_timer_data;

        ICF_START_TIMER(p_ssa_ctxt->p_glb_pdb, p_timer_data, timer_id,
                        timer_dur, p_ecode, ret_val)

            if ( ICF_FAILURE == ret_val)
            {
                ICF_PRINT(((icf_uint8_t *)"\n UNABLE to start the DNS RESP TIMER \n"));
                ICF_SSA_RESET_SSA_TIMER_BLK(p_ssa_timer_data)

                /*Free the allocated ssa timer block here*/
                icf_dbm_ret_mem_to_pool(p_ssa_ctxt->p_glb_pdb, ICF_MEM_SSA_TIMER,
                        p_ssa_timer_data, 
                        p_ecode);
                
                /*Free the allocated timer block here*/
                icf_dbm_ret_mem_to_pool(p_ssa_ctxt->p_glb_pdb, ICF_MEM_TIMER,
                        p_timer_data, p_ecode);

                p_ssa_ctxt->p_dns_resp_timer_data = ICF_NULL;
                p_ssa_ctxt->dns_resp_timer_id = ICF_NULL;
				/* CSR 1-6259119 */
				/* Reset the DNS retry timer duration */
                p_ssa_ctxt->dns_retrans_timer = ICF_NULL;                
            }
            else
            {
                p_ssa_ctxt->dns_resp_timer_id = timer_id;
                p_ssa_ctxt->p_dns_resp_timer_data = p_timer_data;
            }
    }
    ICF_PRINT(((icf_uint8_t *)"\n DNS RESP TIMER STARTED \n"))
  }
  ICF_FUNCTION_EXIT(p_ssa_ctxt->p_glb_pdb)
  return ret_val;

} 


/*******************************************************************************
 *
 * FUNCTION:        icf_ssa_dns_stop_dns_resp_timer
 *
 * DESCRIPTION:     This function will be used to stop dns response timer.
 *
 ******************************************************************************/
icf_return_t icf_ssa_dns_stop_dns_resp_timer(
	      INOUT	  icf_ssa_ctxt_st *p_ssa_ctxt,
        OUT     icf_error_t    *p_ecode)	  
{
  icf_return_t                     ret_val = ICF_SUCCESS;
  icf_ssa_timer_data_st       *p_ssa_timer_data = ICF_NULL;

  ICF_FUNCTION_ENTER(p_ssa_ctxt->p_glb_pdb) /* function entry trace */

	/* check if the timer is running or not */
  if(ICF_TIMER_ID_INVALID !=  p_ssa_ctxt->dns_resp_timer_id)
	{
		/* Non-zero id means that the timer is running, so stop it */
		ICF_STOP_TIMER(p_ssa_ctxt->p_glb_pdb, p_ssa_ctxt->dns_resp_timer_id, p_ecode, \
			ret_val)
    ICF_PRINT(((icf_uint8_t *)"\n DNS RESP timer Stoped \n"));    

    /* free the memory allocated to any existing timer data block */
    if(ICF_NULL != p_ssa_ctxt->p_dns_resp_timer_data)
	  {
         /*Free the allocated ssa timer block here*/
		 p_ssa_timer_data = (icf_ssa_timer_data_st *)(p_ssa_ctxt->p_dns_resp_timer_data->p_timer_buffer);                

		 ICF_SSA_RESET_SSA_TIMER_BLK(p_ssa_timer_data)
         
         icf_dbm_ret_mem_to_pool(p_ssa_ctxt->p_glb_pdb, ICF_MEM_SSA_TIMER,
                                 p_ssa_ctxt->p_dns_resp_timer_data->p_timer_buffer,p_ecode);
         
         /*Free the allocated timer block here*/
         icf_dbm_ret_mem_to_pool(p_ssa_ctxt->p_glb_pdb, ICF_MEM_TIMER,
                                 p_ssa_ctxt->p_dns_resp_timer_data, p_ecode);
         
	  }
    p_ssa_ctxt->p_dns_resp_timer_data = ICF_NULL;
    p_ssa_ctxt->dns_resp_timer_id = ICF_TIMER_ID_INVALID;
	/* CSR 1-6259119 */
	/* Reset the DNS retry timer duration */
    p_ssa_ctxt->dns_retrans_timer = ICF_NULL;    
	} /* end of if for timer_id */
	
  ICF_FUNCTION_EXIT(p_ssa_ctxt->p_glb_pdb);
	return ret_val;
}/* end of icf_ssa_dns_stop_timer() */

/***************************************************************************
 ** FUNCTION:          icf_ssa_get_dns_srv_records
 **
 ** DESCRIPTION:       This function stores two top priority DNS-SRV records 
 **                    from DNS response into icf_ssa_ctxt_st. These SRV 
 **                    responses will be stored in fqdn_list parameter.
 **                    
***************************************************************************/
icf_return_t 	icf_ssa_get_dns_srv_records (
                           icf_ssa_pdb_st*           p_ssa_pdb,
                           Sdf_st_dnsEventContext*   pDnsEventContext)
{
    Sdf_st_dnsSrvRecord* p_srv_rec = ICF_NULL;
    icf_return_t ret_val = ICF_SUCCESS;

    /* SPR 19189 */
    if((ICF_NULL == p_ssa_pdb) || (ICF_NULL == p_ssa_pdb->p_glb_pdb))
    {
        ICF_PRINT(((icf_uint8_t *)("\n[SSA] p_ssa_pdb or p_ssa_pdb->p_glb_pdb is NULL\n.")))
        return ICF_FAILURE;
    }

    ICF_FUNCTION_ENTER(p_ssa_pdb->p_glb_pdb)

    /* SPR 19189 : Added Null check */
    ICF_CHECK_IF_VAR_IS_NULL(p_ssa_pdb->p_glb_pdb,
                             pDnsEventContext,
                             ret_val)
    if(ICF_FAILURE == ret_val)
    {
        ICF_FUNCTION_EXIT(p_ssa_pdb->p_glb_pdb)
        return ret_val;
    }

    /*  Check if no SRV REcord found */
    if( ICF_NULL == pDnsEventContext->pslRecordList )
    {
        ret_val = ICF_FAILURE;
    }
    /*  Check if no SRV REcord found */
    else if( 0 == pDnsEventContext->pslRecordList->size )
    {
        ret_val = ICF_FAILURE;
    }
    /*  if any of the SRV Record found */
    else
    { 
        /* SPR 19189 : Added Null check */
        ICF_CHECK_IF_VAR_IS_NULL(p_ssa_pdb->p_glb_pdb,
                                 p_ssa_pdb->p_ssa_ctxt,
                                 ret_val)
        if(ICF_FAILURE == ret_val)
        {
            ICF_FUNCTION_EXIT(p_ssa_pdb->p_glb_pdb)
            return ret_val;
        }

        /*fetch the highest priority  SRV Record */
        p_srv_rec = ((Sdf_st_dnsSrvRecord *)(((Sdf_st_DnsRecord *) \
                 pDnsEventContext->pslRecordList->head->pData)->pValue));
        
        /*  Copy it into the ssa ctxt */
        icf_port_strcpy(p_ssa_pdb->p_ssa_ctxt->fqdn_list[0].str, \
                (icf_uint8_t*)(p_srv_rec->pTarget));

        /*  Copy the string length */
        p_ssa_pdb->p_ssa_ctxt->fqdn_list[0].str_len =  
            icf_port_strlen(p_ssa_pdb->p_ssa_ctxt->fqdn_list[0].str);

        /* atleast two SRV Record found, Second FQDN is required to be 
         * copied in SSA context only when it is proxy routed request or 
         * REGISTER request, in all other cases only highest priority FQDN
         * will be used. p_rem_addr_active will be NULL if its a direct 
         * routed request*/
        if ((1 < pDnsEventContext->pslRecordList->size ) &&
            (ICF_NULL != p_ssa_pdb->p_ssa_ctxt->p_rem_addr_active))
        {
            /*  Fetch the second highest priority SRV Record */
            p_srv_rec = ((Sdf_st_dnsSrvRecord *)(((Sdf_st_DnsRecord *) \
                  pDnsEventContext->pslRecordList->head->next->pData)->pValue));
            /*  Copy it into second fqdn in ssa ctxt */
            icf_port_strcpy(p_ssa_pdb->p_ssa_ctxt->fqdn_list[1].str, \
                                     (icf_uint8_t*)(p_srv_rec->pTarget));
            p_ssa_pdb->p_ssa_ctxt->fqdn_list[1].str_len =
                icf_port_strlen(p_ssa_pdb->p_ssa_ctxt->fqdn_list[1].str);
        }
        ret_val = ICF_SUCCESS;
    }
    ICF_FUNCTION_EXIT(p_ssa_pdb->p_glb_pdb)
    return ret_val;   
}



/***************************************************************************
 ** FUNCTION:          icf_ssa_dns_query_request
 **
 ** DESCRIPTION:       This is a new function in icf_ssa_dns_util.c, to do 
 **                    a DNS query. Function will allocate Sdf_st_dnsEventContext
 **                    and populate it with values from function  arguments. 
 **                    Internally this function will invoke sdf_fn_uaDnsQuery 
 **                    for DNS query.
 **
***************************************************************************/
icf_return_t 	icf_ssa_dns_query_request (
                      icf_ssa_pdb_st           *p_ssa_pdb,
                      Sdf_st_overlapTransInfo  *pOverlapTrans,
                      icf_uint8_t              *p_domain_name, 
                      icf_void_t               *p_ans_list, 
                      Sdf_ty_DnsRecordType     dns_record_type)
{
    Sdf_st_dnsEventContext    *p_dns_evt_ctxt = Sdf_co_null;
    icf_return_t               ret_val = ICF_SUCCESS;
    Sdf_st_error               err = {0,0,0,""};

    /* SPR 19189 */
    if((ICF_NULL == p_ssa_pdb) || (ICF_NULL == p_ssa_pdb->p_glb_pdb))
    {
        ICF_PRINT(((icf_uint8_t *)("\n[SSA] p_ssa_pdb or p_ssa_pdb->p_glb_pdb is NULL\n.")))
        return ICF_FAILURE;
    }

    ICF_FUNCTION_ENTER(p_ssa_pdb->p_glb_pdb)

    /*  Initialise the DNS Event Context for doing AAA Query */ 
    if( Sdf_co_success != sdf_ivk_uaInitDnsEventContext (&p_dns_evt_ctxt, &err) )
    {
        (Sdf_ty_void)sdf_ivk_uaFreeDnsEventContext (p_dns_evt_ctxt);
        p_dns_evt_ctxt = Sdf_co_null;
        ret_val = ICF_FAILURE;
    }
    else
    {
        /* SPR 19189 : Added Null check */
        ICF_CHECK_IF_ANY_OF_TWO_VAR_IS_NULL(p_ssa_pdb->p_glb_pdb,
                                            p_ssa_pdb->p_ssa_ctxt,
                                            p_ssa_pdb->p_ssa_ctxt->p_call_obj,
                                            ret_val)
        if(ICF_FAILURE == ret_val)
        { 
            /*SPR 19223 removing the klocwork warning
             free DnsEventCtx before returning*/
            /*Free DnsEventCtxt, to decrement dRefCount*/
           (Sdf_ty_void)sdf_ivk_uaFreeDnsEventContext (p_dns_evt_ctxt);
            p_dns_evt_ctxt = Sdf_co_null;
            ICF_FUNCTION_EXIT(p_ssa_pdb->p_glb_pdb)
            return ret_val;
        }

        /*Increment Reference count of pCallObj, pTransaction and pOverlapTransaction
         *as new reference is taken in p_dns_evt_ctxt, so that these are not freed 
         *with p_dns_evt_ctxt*/
        HSS_LOCKEDINCREF(p_ssa_pdb->p_ssa_ctxt->p_call_obj->dRefCount);

        /*Populate p_dns_evt_ctxt with values passed as arguments*/
        p_dns_evt_ctxt->pCallObj = p_ssa_pdb->p_ssa_ctxt->p_call_obj;
        
        /*Fetch Transaction from p_call_obj, either UAC or Overlap Transaction*/
        if (ICF_NULL == pOverlapTrans)
        {
            /*Check if DNS query was for outgoing request, then copy pUacTransaction*/
            if ((ICF_NULL != p_ssa_pdb->p_ssa_ctxt->p_call_obj->pUacTransaction) &&
                (ICF_NULL != p_ssa_pdb->p_ssa_ctxt->p_call_obj->pUacTransaction->pSipMsg))
            {
                p_dns_evt_ctxt->pTransaction = 
                    p_ssa_pdb->p_ssa_ctxt->p_call_obj->pUacTransaction;
                HSS_LOCKEDINCREF(p_dns_evt_ctxt->pTransaction->dRefCount);
            }
        }
        else
        {
            /*Copy overlay transaction and inncrement dRefCount, 
             *so that it is not freed with DnsEventCtxt */
            p_dns_evt_ctxt->pOverlappedTransaction = pOverlapTrans; 
            HSS_LOCKEDINCREF(p_dns_evt_ctxt->pOverlappedTransaction->dRefCount);
            p_dns_evt_ctxt->pTransaction = ICF_NULL;
        }
        
        /*Populate DNsEventCtxt with DNS record list, so that DNS response can be stored in this*/
        p_dns_evt_ctxt->pslRecordList = p_ans_list;
        
        p_dns_evt_ctxt->dDnsRecordType = dns_record_type;
        
        p_dns_evt_ctxt->dProtocol = Sdf_en_protoNone;
 
        /*DNS query to UATK*/
        if ( Sdf_co_success != \
                 sdf_fn_uaDnsQuery((icf_int8_t*)p_domain_name, p_dns_evt_ctxt, &err) ) 
        {
            ret_val = ICF_FAILURE;		
        }
        /*Free DnsEventCtxt, to decrement dRefCount*/
        (Sdf_ty_void)sdf_ivk_uaFreeDnsEventContext (p_dns_evt_ctxt);
        p_dns_evt_ctxt = Sdf_co_null;
    }   
    ICF_FUNCTION_EXIT(p_ssa_pdb->p_glb_pdb)
    return ret_val; 
}

#if 0
/***************************************************************************
 ** FUNCTION:           icf_ssa_dns_start_dns_retrans_timer
 **
 ** DESCRIPTION:       The function is used to start DNS retransmission  
 **                    timer.The timer is started when IPTK makes a DNS query.
***************************************************************************/
icf_return_t icf_ssa_dns_start_dns_retrans_timer(
		INOUT icf_ssa_ctxt_st *p_ssa_ctxt,
		IN  icf_duration_t   timer_dur,
		OUT icf_error_t      *p_ecode)
{
   icf_timer_data_st           *p_timer_data = ICF_NULL;
   icf_timer_id_t              timer_id = ICF_TIMER_ID_INVALID;
   icf_ssa_timer_data_st       *p_ssa_timer_data = ICF_NULL;
   icf_return_t                ret_val = ICF_SUCCESS;
   icf_config_data_st          *p_cfg_data = ICF_NULL;

   ICF_FUNCTION_ENTER(p_ssa_ctxt->p_glb_pdb) /* function entry trace */

   if(ICF_FAILURE == icf_dbm_get_mem_from_pool(
                p_ssa_ctxt->p_glb_pdb, ICF_MEM_TIMER, (icf_void_t **)&p_timer_data, 
                p_ecode))
    {
        ret_val = ICF_FAILURE;
    }
    else if ( ICF_FAILURE == icf_dbm_get_mem_from_pool(\
                p_ssa_ctxt->p_glb_pdb, ICF_MEM_SSA_TIMER, 
                (icf_void_t **)&p_ssa_timer_data,p_ecode))
    {
        ret_val = ICF_FAILURE;
        icf_dbm_ret_mem_to_pool(p_ssa_ctxt->p_glb_pdb, ICF_MEM_TIMER,
                p_timer_data, p_ecode);
    }
    else if(ICF_FAILURE == icf_dbm_get_module_glb_data(
                            p_glb_pdb,
                            ICF_INT_MODULE_CFG,
                            (icf_void_t **)&(ssa_pdb.p_glb_cfg_data),
                            ssa_pdb.p_ecode))
    {
         ICF_PRINT(("\n[SSA]:Failure in GLB Cfg data retreival!!!"));
         return ICF_FAILURE;
    }
    else
    {    
        /* fill the timer data structure */
        p_timer_data->timer_type = ICF_TIMER_TYPE_DNS_RETRANS_TIMER;
        p_timer_data->module_id = ICF_INT_MODULE_SSA;
        p_timer_data->p_timer_buffer = p_ssa_timer_data;
        p_ssa_timer_data->p_ssa_ctxt = p_ssa_ctxt;
        p_ssa_timer_data->p_uatk_ctxt_info = p_dns_evt_ctxt;

        p_timer_data->p_glb_pdb = p_ssa_ctxt->p_glb_pdb;

        ICF_START_TIMER(p_ssa_ctxt->p_glb_pdb, p_timer_data, timer_id,
                        timer_dur, p_ecode, ret_val)

         if ( ICF_FAILURE == ret_val)
         {
               ICF_PRINT(("\n UNABLE to start the DNS RETRANS TIMER \n"));

                /*Free the allocated ssa timer block here*/
                icf_dbm_ret_mem_to_pool(p_ssa_ctxt->p_glb_pdb, ICF_MEM_SSA_TIMER,
                        p_ssa_timer_data, 
                        p_ecode);
                
                /*Free the allocated timer block here*/
                icf_dbm_ret_mem_to_pool(p_ssa_ctxt->p_glb_pdb, ICF_MEM_TIMER,
                        p_timer_data, p_ecode);

                p_ssa_ctxt->dns_retrans_timer_id = ICF_NULL;
         }
         else
         {
                p_ssa_ctxt->dns_retrans_timer_id = timer_id;
         }
    }

    ICF_FUNCTION_EXIT(p_persistent_glb_pdb);
    return ret_val;
} 


/*******************************************************************************
 *
 * FUNCTION:        icf_ssa_dns_stop_dns_retrans_timer
 *
 * DESCRIPTION:     This function will be used to stop dns retransmission timer.
 *
 ******************************************************************************/
icf_return_t icf_ssa_dns_stop_dns_retrans_timer(
	      INOUT	  icf_ssa_ctxt_st *p_ssa_ctxt,
        OUT     icf_error_t    *p_ecode)	  
{
  icf_return_t                     ret_val = ICF_SUCCESS;

  ICF_FUNCTION_ENTER(p_ssa_ctxt->p_glb_pdb) /* function entry trace */

  
	/* check if the timer is running or not */
  if(ICF_TIMER_ID_INVALID !=  p_ssa_ctxt->dns_resp_timer_id)
	{
		/* Non-zero id means that the timer is running, so stop it */
		ICF_STOP_TIMER(p_ssa_ctxt->p_glb_pdb, p_ssa_ctxt->dns_resp_timer_id, p_ecode, \
			ret_val)
    ICF_PRINT(("\n DNS RESP timer Stoped \n"));    

    /* free the memory allocated to any existing timer data block */
    if(ICF_NULL != p_ssa_ctxt->p_dns_resp_timer_data)
	  {
         /*Free the allocated ssa timer block here*/
         icf_dbm_ret_mem_to_pool(p_ssa_ctxt->p_glb_pdb, ICF_MEM_SSA_TIMER,
                                 p_ssa_ctxt->p_dns_resp_timer_data->p_timer_buffer,p_ecode);
         
         /*Free the allocated timer block here*/
         icf_dbm_ret_mem_to_pool(p_ssa_ctxt->p_glb_pdb, ICF_MEM_TIMER,
                                 p_ssa_ctxt->p_dns_resp_timer_data, p_ecode);
         
         printf("DNS Resp Timer Stopped Successfully.");

	  }
    p_ssa_ctxt->p_dns_resp_timer_data = ICF_NULL;
    p_ssa_ctxt->dns_resp_timer_id = ICF_TIMER_ID_INVALID;
	} /* end of if for timer_id */
	
  ICF_FUNCTION_EXIT(p_persistent_glb_pdb);
	return ret_val;
}/* end of icf_ssa_dns_stop_timer() */
#endif

/*******************************************************************************
 *
 * FUNCTION:       icf_ssa_create_next_transaction
 *
 * DESCRIPTION:    This function will be used to clear the calls and 
 *                 create new transaction.
 *
 ******************************************************************************/


Sdf_ty_retVal icf_ssa_create_next_transaction(
        Sdf_st_callObject       *pCallObj,
        Sdf_st_overlapTransInfo *pOverlapTransaction,
        Sdf_st_error            *pErr)
{

    Sdf_ty_pvoid                pContext        = Sdf_co_null;
    Sdf_st_transportInfo        *pDestInfo      = Sdf_co_null;
    Sdf_ty_s8bit                *pLocalBranch   = Sdf_co_null;
    Sdf_ty_s8bit                *pViaBranch     = Sdf_co_null;
    Sdf_ty_u32bit               dTagCount       = 0;

    /* PARAMETER VALIDATION ENDS */
    /*   Fix for removing the Tag value in the  TO header starts */

    if (pCallObj->pCallInfo->dState == Sdf_en_idle) 
    {
        if((pCallObj)->pUacTransaction != Sdf_co_null)
        {
            /* SPR 19776: The DNS State should not be changed when the
             * message is reattempted on next FQDN or sent to secondary
             * server. Sdf_en_dnsIpUsed state will not allow a DNS query
             * in case the address is domain name.
             */ 
            (void)sip_getTagCountFromToHdr(pCallObj->pCommonInfo->pTo,
                    &dTagCount, \
                    (SipError *)&(pErr->stkErrCode));
        }
    }
    /*	 End of Fix for removing the Tag value in the  TO header */

    /* Abort the transaction */
    if (pOverlapTransaction != Sdf_co_null)
    {
        /* SPR 19776: The DNS State should not be changed when the
         * message is reattempted on next FQDN or sent to secondary
         * server. Sdf_en_dnsIpUsed state will not allow a DNS query
         * in case the address is domain name.
         */
        /* Overlapped Transaction will be removed inside the Abort
           Transaction. To avoid the removal, the overlap Transaction	
           is again added in the list */
        HSS_LOCKEDINCREF(pOverlapTransaction->dRefCount);
        (Sdf_ty_void)sdf_ivk_uaAbortTransaction(pCallObj, \
                pOverlapTransaction->dTxnId, pContext, pErr);
        if (sdf_listAppend(&(pCallObj->slOverlapTransInfo), \
                    pOverlapTransaction,pErr) == Sdf_co_fail)
        {
#ifdef SDF_ERROR
            sdf_fn_setError(Sdf_en_majorErrors, \
                    Sdf_en_headerManipulationError,\
                    (Sdf_ty_s8bit *)"sdf_ivk_uaNextDnsTransaction(): "
                    "Failed to insert OverlapTransInfo header into the"
                    "Call Object list structure", pErr);
#endif
            pErr->errCode=Sdf_en_headerManipulationError;
            return Sdf_co_fail;
        }
    }
    else
    {
        if (pCallObj->pUacTransaction != Sdf_co_null)
        {
            (Sdf_ty_void)sdf_ivk_uaAbortTransaction(pCallObj,
                    pCallObj->pUacTransaction->dTxnId, pContext,pErr) ;
        }
    }

    /* Create the new branch for the transaction */
    if(sdf_fn_uaGenerateViaBranch(pDestInfo, &pLocalBranch) != Sdf_co_success)
    {
#ifdef SDF_ERROR
        sdf_fn_setError(Sdf_en_majorErrors, pErr->errCode, \
                (Sdf_ty_s8bit *)"sdf_ivk_uaNextDnsTransaction( ): "
                "Failed to Generate Via Branch",pErr);
#endif
        return Sdf_co_fail;
    }


    /* Add Mazic Cookie to the Via branch */
    pViaBranch = (Sdf_ty_s8bit *)sdf_memget(Sdf_mc_callHandlerMemId, \
            Sdf_mc_strlen(pLocalBranch) + 1 + \
            Sdf_mc_strlen(Sdf_co_viaMagicCookie), \
            (Sdf_st_error *)Sdf_co_null);
    if (ICF_NULL != pViaBranch)
    {

        (Sdf_ty_void)sdf_fn_uaSprintf(pViaBranch, "%s%s", Sdf_co_viaMagicCookie,\
                pLocalBranch);

        (Sdf_ty_void)sdf_memfree(Sdf_mc_callHandlerMemId,  \
                (Sdf_ty_pvoid *)&(pLocalBranch), Sdf_co_null);

        if(pCallObj->pUacTransaction != Sdf_co_null)
        {
            if( pCallObj->pCommonInfo->pKey->pLocalViaBranch != Sdf_co_null)
                (Sdf_ty_void)sdf_memfree(Sdf_mc_callHandlerMemId, (Sdf_ty_pvoid *) \
                        &(pCallObj->pCommonInfo->pKey->pLocalViaBranch), \
                        Sdf_co_null);

            pCallObj->pCommonInfo->pKey->pLocalViaBranch = \
                                                           pViaBranch;
        }
        else if (pOverlapTransaction != Sdf_co_null )
        {
            if( pOverlapTransaction->pOverlapViaBranch != Sdf_co_null )
                (Sdf_ty_void)sdf_memfree(Sdf_mc_callHandlerMemId, (Sdf_ty_pvoid *) \
                        &(pOverlapTransaction->pOverlapViaBranch), Sdf_co_null);

            pOverlapTransaction->pOverlapViaBranch = \
                                                     pViaBranch;
        }

        sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - "
                "Exiting sdf_ivk_uaNextDnsTransaction");
        pErr->errCode = Sdf_en_noUaError;
        return Sdf_co_success;
    }
    else
    {
        (Sdf_ty_void)sdf_memfree(Sdf_mc_callHandlerMemId,  \
                (Sdf_ty_pvoid *)&(pLocalBranch), Sdf_co_null);
        return Sdf_co_fail;
    }

}
#endif

/*****************************************************************************
 * FUNCTION:        icf_ssa_process_buffered_msg_on_dns_resp
 *
 * DESCRIPTION:     This function is used to process the request that was buffered
 *                  due to ongoing DNS query. Buffered message will be processed
 *                  further only if a success response is received.
 *****************************************************************************/
icf_return_t icf_ssa_process_buffered_msg_on_dns_resp(
        icf_ssa_pdb_st          *p_ssa_pdb)
{
    icf_return_t             ret_val = ICF_SUCCESS;
    icf_internal_msg_st	internal_msg;
    icf_nw_otg_session_refresh_req_st session_refresh_req = {0};
    icf_nw_otg_media_change_req_st media_chng_req = {0,0,0,0}; 
    icf_nw_otg_opt_req_st	opt_req = {0,0};
    icf_nw_otg_refer_req_st refer_req = {0,0,0,0,0};
    icf_nw_otg_info_req_st  info_req = {0,{0,"\0"},{0,"\0"},{0,"\0"}};
    icf_uint32_t   msg_id = ICF_NULL;
    icf_nw_otg_generic_msg_req_st generic_req = {{0,"\0"}};

    ICF_FUNCTION_ENTER(p_ssa_pdb->p_glb_pdb)

    icf_port_memset(
        (icf_void_t *)&internal_msg,
        ICF_NULL,
        ICF_PORT_SIZEOF(icf_internal_msg_st));

    if(ICF_NULL == p_ssa_pdb->p_ssa_ctxt)
    {
        ICF_PRINT(((icf_uint8_t *)"\n[SSA]:SSA Context is NULL"))
        return ICF_FAILURE;
    }

    msg_id = internal_msg.msg_hdr.msg_id = 
         p_ssa_pdb->p_ssa_ctxt->internal_msg_id;

    internal_msg.msg_hdr.p_glb_pdb = p_ssa_pdb->p_glb_pdb;

    switch(p_ssa_pdb->p_ssa_ctxt->internal_msg_id)
    {
        case ICF_NW_OTG_SESSION_REFRESH_REQ:
            {
                  internal_msg.msg_hdr.payload_length = 
				      ICF_PORT_SIZEOF(icf_nw_otg_session_refresh_req_st);

                 icf_port_memcpy(&session_refresh_req, 
                       p_ssa_pdb->p_ssa_ctxt->p_dns_query_internal_msg,
                       ICF_PORT_SIZEOF(icf_nw_otg_session_refresh_req_st));

                 internal_msg.p_msg_data = 
                      (icf_uint8_t *)&session_refresh_req;
            }
            break;

        case ICF_NW_OTG_MEDIA_UPDATE:
        case ICF_NW_OTG_CALL_MODIFY_REQ:
            {
                 internal_msg.msg_hdr.payload_length = 
				      ICF_PORT_SIZEOF(icf_nw_otg_media_change_req_st);
				 internal_msg.p_msg_data = 
                      (icf_uint8_t *)&media_chng_req;

                 icf_port_memcpy(&media_chng_req, 
                       p_ssa_pdb->p_ssa_ctxt->p_dns_query_internal_msg,
                       ICF_PORT_SIZEOF(icf_nw_otg_media_change_req_st));
            }
            break;

        case ICF_NW_OTG_OPTIONS_REQ:
            {
                internal_msg.msg_hdr.payload_length = 
				      ICF_PORT_SIZEOF(icf_nw_otg_opt_req_st);
				internal_msg.p_msg_data = (icf_uint8_t *)&opt_req;

                icf_port_memcpy(&opt_req, 
                       p_ssa_pdb->p_ssa_ctxt->p_dns_query_internal_msg,
                       ICF_PORT_SIZEOF(icf_nw_otg_opt_req_st));

            }
            break;

        case ICF_NW_OTG_INFO_REQ:
            {
                 internal_msg.msg_hdr.payload_length = 
				      ICF_PORT_SIZEOF(icf_nw_otg_info_req_st);
				 internal_msg.p_msg_data = (icf_uint8_t *)&info_req;

                 icf_port_memcpy(&info_req, 
                       p_ssa_pdb->p_ssa_ctxt->p_dns_query_internal_msg,
                       ICF_PORT_SIZEOF(icf_nw_otg_info_req_st));

            }
            break;

        case ICF_NW_OTG_REFER:
            {
                internal_msg.msg_hdr.payload_length = 
				      ICF_PORT_SIZEOF(icf_nw_otg_refer_req_st);
				internal_msg.p_msg_data = (icf_uint8_t *)&refer_req;

                icf_port_memcpy(&refer_req, 
                       p_ssa_pdb->p_ssa_ctxt->p_dns_query_internal_msg,
                       ICF_PORT_SIZEOF(icf_nw_otg_refer_req_st));
            }
            break;

        case ICF_NW_OTG_CALL_TERMINATE:
            {
                internal_msg.msg_hdr.payload_length = ICF_NULL;
            }
            break;

        case ICF_NW_OTG_GENERIC_MSG_REQ:
 	         {
                 internal_msg.msg_hdr.payload_length = 
				     ICF_PORT_SIZEOF(icf_nw_otg_generic_msg_req_st);
			     internal_msg.p_msg_data = (icf_uint8_t *)&generic_req;

                 icf_port_memcpy(&generic_req, 
                       p_ssa_pdb->p_ssa_ctxt->p_dns_query_internal_msg,
                       ICF_PORT_SIZEOF(icf_nw_otg_generic_msg_req_st));
             }
             break;

        default:
            return ICF_FAILURE;

    } 

    ret_val = icf_ssa_process_mesg(&internal_msg);

    p_ssa_pdb->p_ssa_ctxt->internal_msg_id = msg_id;
    icf_ssa_free_buffered_msg_for_dns(p_ssa_pdb);

    ICF_FUNCTION_EXIT(p_ssa_pdb->p_glb_pdb)
    return ret_val;
}

/*****************************************************************************
 * FUNCTION:       icf_ssa_free_buffered_msg_for_dns 
 *
 * DESCRIPTION:     This function is used to free the request that was buffered
 *                  due to ongoing DNS query. 
 *****************************************************************************/
icf_return_t icf_ssa_free_buffered_msg_for_dns(
        icf_ssa_pdb_st          *p_ssa_pdb)
{
    icf_return_t             ret_val = ICF_SUCCESS;

    if((ICF_NULL == p_ssa_pdb) || (ICF_NULL == p_ssa_pdb->p_glb_pdb))
    {
        ICF_PRINT(((icf_uint8_t *)"\n[SSA]:SSA Pdb or Glb Pdb is NULL"))
        return ICF_FAILURE;
    }

    ICF_FUNCTION_ENTER(p_ssa_pdb->p_glb_pdb)

    ICF_CHECK_IF_VAR_IS_NULL(p_ssa_pdb->p_glb_pdb,p_ssa_pdb->p_ssa_ctxt,ret_val)
    if(ICF_FAILURE == ret_val)
    {
        return ICF_FAILURE;
    }

    switch(p_ssa_pdb->p_ssa_ctxt->internal_msg_id)
    {
            case ICF_NW_OTG_REFER:
            {
                icf_nw_otg_refer_req_st *p_refer_req = 
                    (icf_nw_otg_refer_req_st *)p_ssa_pdb->p_ssa_ctxt->p_dns_query_internal_msg;

                p_refer_req->p_sic_ctx = ICF_NULL;

                if(ICF_NULL != p_refer_req->p_uri_parameter)
                {
                    ICF_MEMFREE(p_ssa_pdb->p_glb_pdb,
                        p_refer_req->p_uri_parameter, 
                        ICF_MEM_COMMON, p_ssa_pdb->p_ecode, ret_val)
                }

                ICF_MEMFREE(p_ssa_pdb->p_glb_pdb,
                      p_refer_req->p_referred_to, 
                      ICF_MEM_COMMON, p_ssa_pdb->p_ecode, ret_val)
            }
            break;

        default:
            break;
    } 

    ICF_MEMFREE(
           p_ssa_pdb->p_glb_pdb,
           p_ssa_pdb->p_ssa_ctxt->p_dns_query_internal_msg,
           ICF_MEM_COMMON,
           p_ssa_pdb->p_ecode,
           ret_val)

    p_ssa_pdb->p_ssa_ctxt->internal_msg_id = ICF_NULL;

    ICF_FUNCTION_EXIT(p_ssa_pdb->p_glb_pdb)
    return ret_val;
}

/*****************************************************************************
 * FUNCTION:       icf_ssa_buffer_internal_msg_on_dns_query 
 *
 * DESCRIPTION:     This function is used to buffer the internal message
 *                  due to ongoing DNS query. 
 *****************************************************************************/
icf_return_t icf_ssa_buffer_internal_msg_on_dns_query(
        icf_ssa_pdb_st          *p_ssa_pdb)
{
    icf_return_t             ret_val = ICF_SUCCESS;

    if((ICF_NULL == p_ssa_pdb) || (ICF_NULL == p_ssa_pdb->p_glb_pdb))
    {
        ICF_PRINT(((icf_uint8_t *)"\n[SSA]:SSA Pdb or Glb Pdb is NULL"))
        return ICF_FAILURE;
    }

    ICF_FUNCTION_ENTER(p_ssa_pdb->p_glb_pdb)

    ICF_CHECK_IF_VAR_IS_NULL(p_ssa_pdb->p_glb_pdb,p_ssa_pdb->p_ssa_ctxt,ret_val)
    if(ICF_FAILURE == ret_val)
    {
        return ICF_FAILURE;
    }

    if(ICF_NULL != p_ssa_pdb->p_ssa_ctxt->p_dns_query_internal_msg)
    {
        icf_ssa_free_buffered_msg_for_dns(p_ssa_pdb);
    }

   p_ssa_pdb->p_ssa_ctxt->internal_msg_id = p_ssa_pdb->p_internal_msg->msg_hdr.msg_id;

    switch(p_ssa_pdb->p_ssa_ctxt->internal_msg_id)
    {
        case ICF_NW_OTG_SESSION_REFRESH_REQ:
            {
                icf_nw_otg_session_refresh_req_st *p_session_refresh_req = ICF_NULL;
                icf_nw_otg_session_refresh_req_st *p_inc_session_refresh_req = 
                     (icf_nw_otg_session_refresh_req_st *)(p_ssa_pdb->p_internal_msg->p_msg_data);

                ICF_MEMGET(
                        p_ssa_pdb->p_glb_pdb,
                        ICF_PORT_SIZEOF(icf_nw_otg_session_refresh_req_st),
                        ICF_MEM_COMMON,
                        p_ssa_pdb->p_ssa_ctxt->p_dns_query_internal_msg,
                        ICF_RET_ON_FAILURE,
                        p_ssa_pdb->p_ecode,
                        ret_val)

                p_session_refresh_req = (icf_nw_otg_session_refresh_req_st *)
                                 p_ssa_pdb->p_ssa_ctxt->p_dns_query_internal_msg;
            
                icf_port_memcpy(p_session_refresh_req,p_inc_session_refresh_req, 
                       ICF_PORT_SIZEOF(icf_nw_otg_session_refresh_req_st));
            }
            break;

        case ICF_NW_OTG_MEDIA_UPDATE:
        case ICF_NW_OTG_CALL_MODIFY_REQ:
            {
                icf_nw_otg_media_change_req_st *p_media_change_req = ICF_NULL;
                icf_nw_otg_media_change_req_st *p_inc_media_change_req = 
                     (icf_nw_otg_media_change_req_st *)(p_ssa_pdb->p_internal_msg->p_msg_data);

                ICF_MEMGET(
                        p_ssa_pdb->p_glb_pdb,
                        ICF_PORT_SIZEOF(icf_nw_otg_media_change_req_st),
                        ICF_MEM_COMMON,
                        p_ssa_pdb->p_ssa_ctxt->p_dns_query_internal_msg,
                        ICF_RET_ON_FAILURE,
                        p_ssa_pdb->p_ecode,
                        ret_val)

                p_media_change_req = (icf_nw_otg_media_change_req_st *)
                                 p_ssa_pdb->p_ssa_ctxt->p_dns_query_internal_msg;
            
                icf_port_memcpy(p_media_change_req,p_inc_media_change_req, 
                       ICF_PORT_SIZEOF(icf_nw_otg_media_change_req_st));


            }
            break;

        case ICF_NW_OTG_OPTIONS_REQ:
            {
                icf_nw_otg_opt_req_st *p_opt_req = ICF_NULL;
                icf_nw_otg_opt_req_st *p_inc_opt_req = 
                     (icf_nw_otg_opt_req_st *)(p_ssa_pdb->p_internal_msg->p_msg_data);

                ICF_MEMGET(
                        p_ssa_pdb->p_glb_pdb,
                        ICF_PORT_SIZEOF(icf_nw_otg_opt_req_st),
                        ICF_MEM_COMMON,
                        p_ssa_pdb->p_ssa_ctxt->p_dns_query_internal_msg,
                        ICF_RET_ON_FAILURE,
                        p_ssa_pdb->p_ecode,
                        ret_val)

                p_opt_req = (icf_nw_otg_opt_req_st *)
                                 p_ssa_pdb->p_ssa_ctxt->p_dns_query_internal_msg;

                icf_port_memcpy(p_opt_req,p_inc_opt_req, 
                       ICF_PORT_SIZEOF(icf_nw_otg_opt_req_st));
             
                p_opt_req->p_sic_ctx = p_inc_opt_req->p_sic_ctx;

            }
            break;

        case ICF_NW_OTG_INFO_REQ:
            {
                icf_nw_otg_info_req_st *p_info_req = ICF_NULL;
                icf_nw_otg_info_req_st *p_inc_info_req = 
                     (icf_nw_otg_info_req_st *)(p_ssa_pdb->p_internal_msg->p_msg_data);

                ICF_MEMGET(
                        p_ssa_pdb->p_glb_pdb,
                        ICF_PORT_SIZEOF(icf_nw_otg_info_req_st),
                        ICF_MEM_COMMON,
                        p_ssa_pdb->p_ssa_ctxt->p_dns_query_internal_msg,
                        ICF_RET_ON_FAILURE,
                        p_ssa_pdb->p_ecode,
                        ret_val)

                p_info_req = (icf_nw_otg_info_req_st *)
                                 p_ssa_pdb->p_ssa_ctxt->p_dns_query_internal_msg;
            
                icf_port_memcpy(p_info_req,p_inc_info_req, 
                       ICF_PORT_SIZEOF(icf_nw_otg_info_req_st));
 
            }
            break;
            
        case ICF_NW_OTG_REFER:
            {
                icf_nw_otg_refer_req_st *p_refer_req = ICF_NULL;
                icf_nw_otg_refer_req_st *p_inc_refer_req = 
                     (icf_nw_otg_refer_req_st *)(p_ssa_pdb->p_internal_msg->p_msg_data);

                ICF_MEMGET(
                        p_ssa_pdb->p_glb_pdb,
                        ICF_PORT_SIZEOF(icf_nw_otg_refer_req_st),
                        ICF_MEM_COMMON,
                        p_ssa_pdb->p_ssa_ctxt->p_dns_query_internal_msg,
                        ICF_RET_ON_FAILURE,
                        p_ssa_pdb->p_ecode,
                        ret_val)

                p_refer_req = (icf_nw_otg_refer_req_st *)
                                 p_ssa_pdb->p_ssa_ctxt->p_dns_query_internal_msg;
             
                p_refer_req->p_sic_ctx = p_inc_refer_req->p_sic_ctx;
                p_refer_req->bitmask = p_inc_refer_req->bitmask;
                p_refer_req->replace_call_id = p_inc_refer_req->replace_call_id;

                /* copy the refer to address and set the incoming refer-to as NULL */

                if(ICF_NULL != p_inc_refer_req->p_referred_to)
                {
                    ICF_MEMGET(p_ssa_pdb->p_glb_pdb,
                        ICF_PORT_SIZEOF(icf_address_st),
                        ICF_MEM_COMMON,
                        p_refer_req->p_referred_to,
                        ICF_DONOT_RET_ON_FAILURE,
                        p_ssa_pdb->p_ecode,
                        ret_val)

                    if(ICF_FAILURE == ret_val)
                    {
                        ICF_MEMFREE(
                           p_ssa_pdb->p_glb_pdb,
                           p_ssa_pdb->p_ssa_ctxt->p_dns_query_internal_msg,
                           ICF_MEM_COMMON,
                           p_ssa_pdb->p_ecode,
                           ret_val)
                        return ICF_FAILURE;    
                    }
                    else
                    {    
                       icf_port_memcpy(p_refer_req->p_referred_to,
                               p_inc_refer_req->p_referred_to,
                               ICF_PORT_SIZEOF(icf_address_st));
                    } 
                }
                else
                {
                    p_refer_req->p_referred_to = ICF_NULL;
                }

                /* copy the uri parameters and set the incoming uri parameters as NULL */
                if(ICF_REFER_TO_ADDNL_URI_PARAMTR_PRES & p_inc_refer_req->bitmask)
                {
                    p_refer_req->p_uri_parameter = p_inc_refer_req->p_uri_parameter;
                    p_inc_refer_req->p_uri_parameter = ICF_NULL; 
                }
                else
                {
                    p_refer_req->p_uri_parameter = ICF_NULL;
                }
            }
            break;

        case ICF_NW_OTG_GENERIC_MSG_REQ:
 	    {
                icf_nw_otg_generic_msg_req_st *p_generic_msg_req = ICF_NULL;
                icf_nw_otg_generic_msg_req_st *p_inc_generic_msg_req = 
                     (icf_nw_otg_generic_msg_req_st *)(p_ssa_pdb->p_internal_msg->p_msg_data);

                ICF_MEMGET(
                        p_ssa_pdb->p_glb_pdb,
                        ICF_PORT_SIZEOF(icf_nw_otg_generic_msg_req_st),
                        ICF_MEM_COMMON,
                        p_ssa_pdb->p_ssa_ctxt->p_dns_query_internal_msg,
                        ICF_RET_ON_FAILURE,
                        p_ssa_pdb->p_ecode,
                        ret_val)

                p_generic_msg_req = (icf_nw_otg_generic_msg_req_st *)
                                 p_ssa_pdb->p_ssa_ctxt->p_dns_query_internal_msg;
            
                icf_port_memcpy(p_generic_msg_req,p_inc_generic_msg_req, 
                       ICF_PORT_SIZEOF(icf_nw_otg_generic_msg_req_st));
 
            }
            break;


        default:
            break;

    } 

    ICF_FUNCTION_EXIT(p_ssa_pdb->p_glb_pdb)
    return ret_val;
}

/* SPR 19007: New function has been added for freeing the DNS specific data. */
/*******************************************************************************
 *
 * FUNCTION:        icf_ssa_dns_free_dns_context_data
 *
 * DESCRIPTION:     This function will be used to free the DNS event context data.
 *
 ******************************************************************************/
icf_return_t icf_ssa_dns_free_dns_context_data(
	      INOUT	  icf_ssa_pdb_st *p_ssa_pdb)
{
   icf_return_t      ret_val = ICF_SUCCESS;
   icf_ssa_ctxt_st   *p_ssa_ctxt = p_ssa_pdb->p_ssa_ctxt;
   icf_timer_data_st        *p_timer_data = ICF_NULL;
   icf_ssa_timer_data_st    *p_ssa_timer_data = ICF_NULL;
   icf_global_port_info_st  *p_glb_port_data = ICF_NULL;
   Sdf_st_dnsEventContext   *p_dns_evt_ctxt = Sdf_co_null;

   ICF_FUNCTION_ENTER(p_ssa_pdb->p_glb_pdb) /* function entry trace */

   ICF_CHECK_IF_VAR_IS_NULL(p_glb_pdb, \
                       p_ssa_pdb->p_ssa_ctxt, ret_val)
   if(ICF_FAILURE == ret_val) 
   {
         ICF_FUNCTION_EXIT(p_ssa_pdb->p_glb_pdb)
         return ret_val;
   }

   ICF_CHECK_IF_ANY_OF_TWO_VAR_IS_NULL(p_ssa_pdb->p_glb_pdb,
                  p_ssa_pdb->p_glb_pdb->p_glb_data,
                  p_ssa_pdb->p_glb_pdb->p_glb_data->p_glb_port_info,
                  ret_val)
    if(ICF_FAILURE == ret_val)
    {
        ICF_FUNCTION_EXIT(p_ssa_pdb->p_glb_pdb)
        return ret_val;
    }

    p_glb_port_data = p_ssa_pdb->p_glb_pdb->p_glb_data->p_glb_port_info;
      

   if(ICF_NULL != p_ssa_ctxt->p_dns_resp_timer_data)
   {
      /* Retreiving dns event ctxt info from ssa timer buffer. */
      p_timer_data = p_ssa_ctxt->p_dns_resp_timer_data;
      p_ssa_timer_data = (icf_ssa_timer_data_st *)p_timer_data->p_timer_buffer;

      if(ICF_NULL != p_ssa_timer_data)
      {
         p_dns_evt_ctxt = (Sdf_st_dnsEventContext *)p_ssa_timer_data->p_uatk_ctxt_info;

         /* Free DnsEventContext of the Dns Query */
         (Sdf_ty_void)sdf_ivk_uaFreeDnsEventContext (p_dns_evt_ctxt);
      }

      /* Stop the dns response timer */
      icf_ssa_dns_stop_dns_resp_timer(p_ssa_ctxt, p_ssa_pdb->p_ecode);
   }
		
   ICF_FUNCTION_EXIT(p_ssa_pdb->p_glb_pdb);
   return ret_val;
}/* end of icf_ssa_dns_free_dns_context_data() */

