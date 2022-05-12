
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
 ** 14-Dec-2009     Pavneet Bajaj          SPR 20237      Fix for CSR 8072456
** 09-Apr-2010     Alok Tiwari         SPR 20362         Modified Function:
 **                                               icf_ssa_resend_ongoing_txn
 ** 03-May-2010     Alok Tiwari         SPR 20395         Modified Function:
 **                                            icf_ssa_process_dns_fail_resp
 *****************************************************************************
 **                 Copyrights 2006, Aricent .
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

#include "basic.h"
#include "icf_ssa_common_types.h"

#include "icf_ecode.h"
#include "icf_cfg_defs.h"


#ifdef ICF_DNS_LOOKUP_ENABLED
#include "sdf_lookup.h"
#endif

#ifdef ICF_PORT_WINDOWS
#pragma warning(disable:4701)
#pragma warning ( disable : 4702 )
#endif


/* Global defintion of persistent global pdb */
extern icf_glb_pdb_st        *p_persistent_glb_pdb;

/*icf_return_t icf_ssa_dns_stop_dns_resp_timer(
	INOUT	icf_ssa_ctxt_st *p_ssa_ctxt,
        OUT     icf_error_t    *p_ecode);*/

#ifdef ICF_DNS_LOOKUP_ENABLED
/*******************************************************************************
 * FUNCTION:        icf_ssa_process_dns_fail_resp
 *
 * DESCRIPTION:     This function handles the DNS failure response from NW
 ******************************************************************************/
icf_return_t	icf_ssa_process_dns_fail_resp(icf_ssa_pdb_st *p_ssa_pdb,
		Sdf_st_dnsEventContext *p_dns_evt_ctxt)
{
	icf_return_t	ret_val = ICF_SUCCESS;
	/*this flag will be used to judge whether we need to invoke
	FreeCallObject at the end of this routine or not. This will
	be set on a per case basis depending on the app module's
	behavior*/
	icf_boolean_t	ivk_free_call_obj = ICF_FALSE;
         /* icf_glb_pdb_st  *p_glb_pdb = p_ssa_pdb->p_glb_pdb; */

	icf_internal_msg_st  *p_internal_msg = p_ssa_pdb->p_internal_msg;

	ICF_FUNCTION_ENTER(p_ssa_pdb->p_glb_pdb)
#ifdef ICF_STAT_ENABLE
	if(p_dns_evt_ctxt->dDnsRecordType == Sdf_en_SRVrecord)
	{
		ICF_STATS(p_ssa_pdb->p_glb_pdb->p_stats_info->
					misc_stats.stats_SRV_DNS_FAILURE_RESP)
	}
	else if(p_dns_evt_ctxt->dDnsRecordType == Sdf_en_IPv4Address)
	{
		ICF_STATS(p_ssa_pdb->p_glb_pdb->p_stats_info->
                                        misc_stats.stats_AAA_DNS_QUERY_FAILURE_RESP)
	}
#endif

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


	if (ICF_NULL != p_ssa_pdb->p_ssa_ctxt->p_call_ctxt)
	{
		SipOptions      options;
		SipMessage      *p_sip_msg = SIP_NULL;

		options.dOption = SIP_OPT_CLEN|SIP_OPT_FULLFORM;

		if ( Sdf_co_null == p_dns_evt_ctxt->pTransaction)
		{
			/*overlap transaction*/
			p_sip_msg = p_dns_evt_ctxt->pOverlappedTransaction->pSipMsg;
		}
		else/*either a uac or a Uas transaction*/
		{
			p_sip_msg = p_dns_evt_ctxt->pTransaction->pSipMsg;
		}
		if(ICF_SUCCESS == icf_ssa_resend_ongoing_txn(p_ssa_pdb->p_ssa_ctxt->\
					p_call_obj,
					p_sip_msg, 
					p_dns_evt_ctxt->pOverlappedTransaction,\
					options))
		{
			ICF_FUNCTION_EXIT(p_ssa_pdb->p_glb_pdb)
				return ICF_SUCCESS;
		}

		else
		{ 
			p_ssa_pdb->p_glb_pdb->p_call_ctx = 
				p_ssa_pdb->p_ssa_ctxt->p_call_ctxt;

			/*Fill internal message for other module*/
			p_internal_msg->p_msg_data = ICF_NULL;/*no payload*/
			/*ICF_GENERIC_CALL_CLEAR;*/

			p_internal_msg->msg_hdr.payload_length = 0;
			p_internal_msg->msg_hdr.p_glb_pdb = p_ssa_pdb->p_glb_pdb;

			if (ICF_CALL_DIR == (ICF_CALL_DIR & 
						p_ssa_pdb->p_ssa_ctxt->p_call_ctxt->common_bitmask))
			{
                /* Fix for CSR 1-8564918 : SPR-20395*/
                /* No need to set the boolean flag as this will free the call object
                 * When an incoming call is processed,call object will be freed through
                 * the function icf_ssa_process_forceful_call_clear.
                 */
			/*to be set in case of incoming call only*/
				/*ivk_free_call_obj = ICF_TRUE;*/
				p_internal_msg->msg_hdr.msg_id = 
					ICF_FORCEFUL_CALL_CLEAR_IND;
			}
			else
			{				
				p_internal_msg->msg_hdr.msg_id = 
					ICF_GENERIC_CALL_CLEAR;
			}
			p_ssa_pdb->dest_module_id = ICF_INT_MODULE_CM;
#ifdef ICF_ERROR_ENABLE
            ICF_SSA_SET_ECODE(p_ssa_pdb,"INVITE",ICF_ERROR_DNS_QUERY_FAILURE)
#endif
		}
	}
	else if (ICF_NULL != p_ssa_pdb->p_ssa_ctxt->p_rgm_ctxt)
	{

    		icf_rgm_nw_resp_st               *p_rgm_msg = ICF_NULL;
		
		/* Populate the internal message header */
		p_internal_msg->msg_hdr.msg_id = ICF_NW_INC_REG_RESP;
		p_internal_msg->msg_hdr.payload_length = 
			ICF_PORT_SIZEOF(icf_rgm_nw_resp_st);

		/* Populate the internal message payload */
		p_rgm_msg = (icf_rgm_nw_resp_st *)p_internal_msg->p_msg_data;

		p_rgm_msg->p_rgm_context = p_ssa_pdb->p_ssa_ctxt->p_rgm_ctxt;
		ICF_ASSERT(p_ssa_pdb->p_glb_pdb, (ICF_NULL != p_rgm_msg->p_rgm_context))

		p_rgm_msg->status = ICF_RGM_SSA_NW_FAILURE;

		p_rgm_msg->ecode = ICF_REGM_SSA_DNS_FAILURE;

		p_internal_msg->msg_hdr.p_glb_pdb = p_ssa_pdb->p_glb_pdb;

		p_ssa_pdb->dest_module_id = ICF_INT_MODULE_CFG;
#ifdef ICF_ERROR_ENABLE
        ICF_SSA_SET_ECODE(p_ssa_pdb,"REGISTER",ICF_ERROR_DNS_QUERY_FAILURE)
#endif
	}
	else if (ICF_NULL != p_ssa_pdb->p_ssa_ctxt->p_sic_ctxt)
	{
		en_SipMessageType	msg_type;
		Sdf_ty_s8bit		method[ICF_MAX_SIZE_INT_USAGE_STR];

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

				p_refer_resp->response = 0;
				p_refer_resp->p_sic_ctx = p_ssa_pdb->p_ssa_ctxt->p_sic_ctxt;
                if (ICF_NULL != p_refer_resp->p_sic_ctx)
                {
                    p_refer_resp->p_sic_ctx->error_code = ICF_CAUSE_DNS_QUERY_FAILURE;
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

				if (ICF_NULL == p_ssa_pdb->p_ssa_ctxt->p_sic_ctxt)
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

					p_subs_resp->bitmask = 0;
					p_subs_resp->response = ICF_NW_INC_SUBS_RESP_NO_RETRY;
					p_subs_resp->resp_code=0;
					p_subs_resp->p_sic_ctx = p_ssa_pdb->p_ssa_ctxt->p_sic_ctxt;
					ICF_ASSERT(p_ssa_pdb->p_glb_pdb, (ICF_NULL != p_subs_resp->p_sic_ctx))
#ifdef ICF_ERROR_ENABLE
                    ICF_SSA_SET_ECODE(p_ssa_pdb,"SUBSCRIBE",ICF_ERROR_DNS_QUERY_FAILURE)
#endif
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

				p_msg_resp->response = 0;
				p_msg_resp->p_sic_ctx = p_ssa_pdb->p_ssa_ctxt->p_sic_ctxt;
                /* Fix for CSR 1-8564918 : SPR-20395*/
                /* No need to set the boolean flag as this will free the call object
                 * In case of MESSAGE ,call object should be freed through
                 * the function icf_ssa_process_delink_req.
                 */
                /*ivk_free_call_obj = ICF_TRUE;*/
                if (ICF_NULL != p_msg_resp->p_sic_ctx)
                {
                    p_msg_resp->p_sic_ctx->error_code = ICF_CAUSE_DNS_QUERY_FAILURE;
                    ICF_ASSERT(p_ssa_pdb->p_glb_pdb, (ICF_NULL != p_msg_resp->p_sic_ctx))
                }
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

				p_opt_resp->response = 0;
				p_opt_resp->p_sic_ctx = p_ssa_pdb->p_ssa_ctxt->p_sic_ctxt;
				ICF_ASSERT(p_ssa_pdb->p_glb_pdb, (ICF_NULL != p_opt_resp->p_sic_ctx))

                /* Fix for CSR 1-8564918 : SPR-20395*/
                /* Set the correct error cause */
                if(ICF_NULL != p_opt_resp->p_sic_ctx)
                {
                   p_opt_resp->p_sic_ctx->error_code = ICF_CAUSE_DNS_QUERY_FAILURE;
                } 
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

				p_publish_resp->response = 0;
				p_publish_resp->duration = 0;
				p_publish_resp->p_sic_ctx = p_ssa_pdb->p_ssa_ctxt->p_sic_ctxt;
                /* Fix for CSR 1-8564918 : SPR-20395*/
                /* No need to set the boolean flag as this will free the call object
                 * In case of PUBLISH ,call object should be freed through
                 * the function icf_ssa_process_delink_req.
                 */
                /*ivk_free_call_obj = ICF_TRUE;*/
                if (ICF_NULL != p_publish_resp->p_sic_ctx)
                {
                    p_publish_resp->p_sic_ctx->error_code = ICF_CAUSE_DNS_QUERY_FAILURE;
                    ICF_ASSERT(p_glb_pdb, (ICF_NULL != p_publish_resp->p_sic_ctx))
                }
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
	if ( ICF_TRUE == ivk_free_call_obj)
	{
		sdf_ivk_uaFreeCallObject(&(p_dns_evt_ctxt->pCallObj));
	}
	if ( ICF_SUCCESS == ret_val)
	{
		ret_val = icf_ssa_process_otg_intrnl_msg(p_ssa_pdb);
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
	icf_ssa_pdb_st           ssa_pdb;
	icf_return_t		ret_val = ICF_SUCCESS;
	icf_internal_msg_st	internal_msg;
	icf_ssa_glb_data_st      *p_ssa_glb_data = ICF_NULL;
	icf_uint32_t                count = 0;
	icf_ssa_ctxt_st		*p_ssa_ctxt = 
		(icf_ssa_ctxt_st*)(p_dns_evt_ctxt->pCallObj->pAppData->pData);	
	Sdf_st_error		sdf_error;
	Sdf_st_eventContext	*p_event_context = Sdf_co_null;
	icf_ssa_intrnl_intf_ut  internal_msg_payload;
        icf_error_t             ecode;
#ifdef ICF_NAT_RPORT_SUPPORT
        SipMessage              *p_sip_msg = Sdf_co_null;
        SipError                SipErr;
        en_SipMessageType       dReqRespType;
        SipParam                *p_param = ICF_NULL,*p_param_1 = ICF_NULL;
#endif


	if(ICF_NULL == p_persistent_glb_pdb)
	{
		return;
        }

	ICF_FUNCTION_ENTER(p_persistent_glb_pdb)

    /* Fix for CSR 1-8564918 : SPR-20395*/
    /* UMR fixed*/
    icf_port_memset((icf_void_t *)&internal_msg, 0,
                          ICF_PORT_SIZEOF(icf_internal_msg_st));
#ifdef ICF_NAT_RPORT_SUPPORT
        if ( Sdf_co_null == p_dns_evt_ctxt->pTransaction)
	{
		/*overlap transaction*/
		p_sip_msg = p_dns_evt_ctxt->pOverlappedTransaction->pSipMsg;
	}
	else/*either a uac or a Uas transaction*/
	{
		p_sip_msg = p_dns_evt_ctxt->pTransaction->pSipMsg;
	}
#endif

        /*IPTK shall stop the timer in Ares_cbk_uaQueryResult
         *Fix for SPR 16713
         */  
     /* icf_ssa_dns_stop_dns_resp_timer(p_ssa_ctxt,&ecode);*/

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
			Sdf_st_transportInfo    *p_dest_info = Sdf_co_null;
			/*Now we retreive the IP address from the destinfo field of
			  the transaction and fill in ssa pdb*/
			if ( Sdf_co_null == p_dns_evt_ctxt->pTransaction)
			{
				p_dest_info = 
					p_dns_evt_ctxt->pOverlappedTransaction->pDestInfo;
			}
			else
			{
				p_dest_info = 
					p_dns_evt_ctxt->pTransaction->pDestInfo;
			}
            if (p_ssa_ctxt-> p_dns_ip_addr != ICF_NULL)
            {
                icf_ssa_dns_delete_fqdn_entry(&p_ssa_ctxt->fqdn, &ecode);
                if ((icf_port_strcmp(p_ssa_ctxt->p_dns_ip_addr->ip_address,  (icf_uint8_t *)p_dest_info->pIp)
                            == ICF_NULL) &&
                        ( p_ssa_ctxt->p_dns_ip_addr->port == p_dest_info->dPort))
                {
                    ICF_PRINT(((icf_uint8_t *)"Getting the same entry again. So returning failure"));
                    return ;
                }
            }

			if ((Sdf_co_null == p_dest_info) || 
					(Sdf_co_null == p_dest_info->pIp))
			{
				ICF_PRINT(((icf_uint8_t *)"\n[SSA]:EmpryDestinationInfo!!returning"));
				ret_val = ICF_FAILURE;
			}
			else
			{
				icf_port_strcpy((icf_uint8_t *)ssa_pdb.dest_ip, (icf_uint8_t *)p_dest_info->pIp);
				ssa_pdb.dest_port = (icf_uint16_t)p_dest_info->dPort;
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
					ICF_PRINT(((icf_uint8_t *)"\n[SSA]:FailureInInitEventCtxt in ProcessDnsResp"));
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
#ifdef ICF_NAT_RPORT_SUPPORT
             /* Check whether rport needs to be send in the outgoing 
              * request or not. 
              */
             /* Get the message type being send on the network.
              */
             if(SipFail == sip_getMessageType(p_sip_msg,&dReqRespType,\
                            (SipError*)&(SipErr)))
             {
                   ret_val = ICF_FAILURE;
             }

              else
              {
                if(ICF_SUCCESS == ret_val)
                {
                    switch(dReqRespType)
                    {
                         case SipMessageRequest:
                       /* send rport in the Via Header of request
                        * message
                        */
                        if((ICF_TRUE == ssa_pdb.p_glb_cfg_data->\
                                 rport_config_data.is_rport_reqd)&&\
                                 (ICF_TRANSPORT_TYPE_UDP == 
                                   p_ssa_ctxt->transport))
                         {

                           if(SipFail != sip_initSipParam(&p_param,\
                                             (SipError*)&(SipErr)))
                            {
                               Sdf_mc_strdup(p_param->pName,"rport");
                                     
                               if(SipFail == icf_ssa_set_param_in_Via\
                                               (p_sip_msg,&ssa_pdb,p_param,\
                                                SipErr))
                                {
                                  /* Free any local references and
                                   * return failure.
                                   */
                                    sdf_memfree(Sdf_mc_callHandlerMemId,\
                                                (Sdf_ty_pvoid*)&p_param,\
                                                   &sdf_error);
                       	        }
                             }
                                    sip_freeSipParam(p_param);

                          }
                                    break;
                            case SipMessageResponse:
                           /* Check whether rport was received in the
                            * request, if yes then send the received
                            * and rport parameters in the Via Header of 
                            * the response.
                            */
                                     
                            if(ICF_RPORT_RECEIVED & p_ssa_ctxt->\
                                       rport_bitmask)
                            {
                                 icf_uint8_t     temp_rport[10];

                                /* Delete the "rport" and "received" 
                                 * parameters from the Via Header if 
                                 * already present.
                                 */
                                if(ICF_FAILURE == icf_ssa_del_params_in_Via(\
                                                 p_sip_msg,&ssa_pdb,&SipErr))
                                 {
                                    ICF_PRINT(("\n Unable to delete Params"\
                                                    " from Via Header.\n"));
                                     ret_val = ICF_FAILURE;

                                 }
                                 else if(SipFail != sip_initSipParam(&p_param,\
                                                  (SipError*)&(SipErr)))

                                 {
                                     Sdf_mc_strdup(p_param->pName,"received");
                                          sip_listInit(&(p_param->slValue),\
                                                   __sip_freeSipParam, \
                                                    (SipError*)&(SipErr));
                                            
                                     if(SipFail == sip_listInsertAt(\
                                               &(p_param->slValue),0,\
                                             (icf_void_t *)(p_ssa_ctxt->\
                                                 destination_ip),&SipErr))
                                     {
                                         ret_val = ICF_FAILURE;

                                     }
                                            
                                     else if(SipFail == icf_ssa_set_param_in_Via(\
                                                      p_sip_msg,&ssa_pdb,p_param,\
                                                     SipErr))
                                     {
                                      /* Free any local references and
                                       * return failure.
                                       */

                                          sdf_memfree(Sdf_mc_callHandlerMemId,\
                                                   (Sdf_ty_pvoid*)&p_param,&sdf_error);
                                          ret_val = ICF_FAILURE;
                                      }
                                        
                                      /* Set the rport parameter in the Via Header
                                       */
                                     else if(SipFail != sip_initSipParam(&p_param_1,(SipError*)&(SipErr)))
                                     {
                                          Sdf_mc_strdup(p_param_1->pName,"rport");

                                          icf_port_sprintf(temp_rport,"%d",\
                                                     p_ssa_ctxt->destination_port);
                                          sip_listInit(&(p_param_1->slValue),\
                                                      __sip_freeSipParam, \
                                                       (SipError*)&(SipErr));

                                     if(SipFail == sip_listInsertAt(\
                                                     &(p_param_1->slValue),0,\
                                                      (icf_void_t *)(temp_rport),&SipErr))
                                     {
                                          ret_val = ICF_FAILURE;

                                     }

                                     else if(SipFail == icf_ssa_set_param_in_Via(\
                                                        p_sip_msg, &ssa_pdb,p_param_1,\
                                                        SipErr))
                                     {
                                       /* Free any local references and 
                                        * return failure.
                                        */
                                        sdf_memfree(Sdf_mc_callHandlerMemId,\
                                                      (Sdf_ty_pvoid*)&p_param,&sdf_error);
                                                    sdf_memfree(Sdf_mc_callHandlerMemId,\
                                                     (Sdf_ty_pvoid*)&p_param_1,&sdf_error);
                                          ret_val = ICF_FAILURE;
                                      }
                                       else
                                       {
                                              sip_freeSipParam(p_param);
                                              sip_freeSipParam(p_param_1);

                                        }
                                      }
                                   }
                               }
                                    break;
                                default:
                                    ICF_PRINT(("\n[SSA]: Neither a Sip"\
                                                "Request/Response Message"));

                                    break;
                                }
                        }
                     
                    }
#endif
/*Fix for CSR-1-5333230 ends */
					if ((ICF_TRUE == ready_to_send) &&
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
					sdf_ivk_uaFreeEventContext(p_event_context);
                                       /* Here the stats of successful responses of DNS query 
                       will be collected on the basis of their type*/
#ifdef ICF_STAT_ENABLE                  
                   if(p_dns_evt_ctxt->dDnsRecordType == Sdf_en_SRVrecord)
                    {
                      ICF_STATS(p_persistent_glb_pdb->p_stats_info->
                               misc_stats.stats_SRV_DNS_SUCCESS_RESP)
                    }
                   else if(p_dns_evt_ctxt->dDnsRecordType == Sdf_en_IPv4Address)
                    {
                      ICF_STATS(p_persistent_glb_pdb->p_stats_info->
                           misc_stats.stats_AAA_DNS_QUERY_SUCCESS_RESP)
                    }
#endif
				}/*Init Event Context successful*/
			}/*ret_val success check*/
		}/*dns resp success failure check*/
		if ( (Sdf_co_false == dns_resp_flag) || 
			(ICF_FAILURE == ret_val))
		{
			if ( ICF_FAILURE == icf_ssa_process_dns_fail_resp(&ssa_pdb,
						p_dns_evt_ctxt))
			{
				ICF_PRINT(((icf_uint8_t *)"\n[SSA]:Failure in handling DNS FAIL RSP"));
				ret_val = ICF_FAILURE;
			}
		}
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
	icf_ssa_ctxt_st		*p_ssa_ctxt = (icf_ssa_ctxt_st*)
			(pCallObj->pAppData->pData);
    Sdf_st_overlapTransInfo   *pTransInfo = Sdf_co_null;
    Sdf_st_transaction        *pTransaction = Sdf_co_null;
    Sdf_st_transportInfo 	  *p_dest_Info = Sdf_co_null;	
    /*Fix for CSR 1-8223807 : SPR-20362*/
    Sdf_ty_s8bit              *p_remove_hdr_list = ICF_NULL;
    icf_return_t               dummy_ret_val = ICF_SUCCESS;
	icf_error_t			       err = 0;



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

            /*Fix for CSR 1-8223807 : SPR-20362*/
            if(ICF_NULL != p_ssa_ctxt->p_sic_ctxt->p_remove_hdrs)
            {
                ICF_MEMGET(p_persistent_glb_pdb,
                           ICF_MAX_LARGE_STR_LEN,
                           ICF_MEM_COMMON,
                           p_remove_hdr_list,
                           ICF_RET_ON_FAILURE,
                           (icf_error_t *)&err, ret_val)

                icf_port_strcpy(p_remove_hdr_list,
                           p_ssa_ctxt->p_sic_ctxt->p_remove_hdrs);
            }
		}
        else if(ICF_NULL != p_ssa_ctxt->p_rgm_ctxt)
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
            /*Fix for CSR 1-8223807 : SPR-20362*/
			else if (Sdf_co_success == sdf_ivk_uaSendCallToPeer(
                                                  pCallObj, pSipMsg,
                                                  dOptions, 0, pEventContext,
                                                  p_remove_hdr_list,  &ecode))
			{
                   /*Fix for CSR 1-8223807 : SPR-20362*/
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
                    	  return Sdf_co_fail;
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
		} while(1);
		sdf_ivk_uaFreeEventContext(pEventContext);
	}
    
#ifdef ICF_DNS_LOOKUP_ENABLED
    p_ssa_ctxt= p_ssa_ctxt;
#endif
    /*Fix for CSR 1-8223807 : SPR-20362*/
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
				ICF_PRINT(((icf_uint8_t *)"\n[SSA]:EmpryDestinationInfo!!returning"));
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
	icf_return_t                 ret_val = ICF_SUCCESS;
    icf_ssa_timer_data_st        *p_ssa_timer_data = ICF_NULL; 

	ICF_FUNCTION_ENTER(p_glb_pdb) /* function entry trace */

    if(ICF_FAILURE ==p_glb_pdb->p_glb_data->p_dns_info)
    {
        /* no DNS info return the failue */
        ret_val = ICF_FAILURE;
    }
    /*else if ( ICF_FAILURE == icf_dbm_create_timer_block(
				p_glb_pdb,(icf_void_t **)&p_timer_data,
				p_ecode))
	{
		ret_val = ICF_FAILURE;
	}*/
   if(ICF_FAILURE == icf_dbm_get_mem_from_pool(
                p_glb_pdb, ICF_MEM_TIMER, (icf_void_t **)&p_timer_data, 
                p_ecode))
    {
        ret_val = ICF_FAILURE;
    }
    else if ( ICF_FAILURE == icf_dbm_get_mem_from_pool(\
                p_glb_pdb, ICF_MEM_SSA_TIMER, 
                (icf_void_t **)&p_ssa_timer_data,p_ecode))
    {
        ret_val = ICF_FAILURE;
        icf_dbm_ret_mem_to_pool(p_glb_pdb, ICF_MEM_TIMER,
                p_timer_data, p_ecode);
    }
    else
    {    
        /* fill the timer data structure */
        p_timer_data->timer_type = ICF_TIMER_TYPE_DNS_BUFFER_TIMER;
        p_timer_data->module_id = ICF_INT_MODULE_SSA;

        p_timer_data->p_timer_buffer = p_ssa_timer_data;

        p_timer_data->p_glb_pdb = p_glb_pdb;
        p_glb_pdb->p_glb_data->p_dns_info->p_dns_timer_data = p_timer_data;

        ICF_START_TIMER(p_glb_pdb, p_timer_data, p_glb_pdb->p_glb_data->p_dns_info->dns_buffer_timer_id,
                timer_dur, p_ecode, ret_val)

            if ( ICF_FAILURE == ret_val)
            {
                ICF_PRINT(((icf_uint8_t *)"\n UNABLE to start the DNS TIMER \n"));
                /*Free the allocated timer block here*/
                icf_dbm_ret_mem_to_pool(p_glb_pdb, ICF_MEM_TIMER,
                        p_timer_data, p_ecode);
                /*Free the allocated ssa timer block here*/
                icf_dbm_ret_mem_to_pool(p_glb_pdb, ICF_MEM_SSA_TIMER,
                        p_ssa_timer_data, 
                        p_ecode);
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

    ICF_FUNCTION_ENTER(p_glb_pdb) /* function entry trace */

	/* check if the timer is running or not */
	if (ICF_NULL != p_glb_pdb->p_glb_data->p_dns_info &&
        ICF_TIMER_ID_INVALID !=  p_glb_pdb->p_glb_data->p_dns_info->dns_buffer_timer_id)
	{
		/* Non-zero id means that the timer is running, so stop it */
		ICF_STOP_TIMER(p_glb_pdb, p_glb_pdb->p_glb_data->p_dns_info->dns_buffer_timer_id, p_ecode, \
			ret_val)
        ICF_PRINT(((icf_uint8_t *)"\n DNS Purge timer Stoped \n"));    

    /* free the memory allocated to any existing timer data block */
	if (ICF_NULL != p_glb_pdb->p_glb_data->p_dns_info && 
            ICF_NULL != p_glb_pdb->p_glb_data->p_dns_info->p_dns_timer_data)
	{
		/*icf_dbm_free_timer_block(p_glb_pdb,
			p_glb_pdb->p_glb_data->p_dns_info->p_dns_timer_data, \
			p_ecode);*/
         /*Free the allocated ssa timer block here*/
         icf_dbm_ret_mem_to_pool(p_glb_pdb, ICF_MEM_SSA_TIMER,
                        ((icf_timer_data_st*)p_glb_pdb->p_glb_data->p_dns_info->p_dns_timer_data)->p_timer_buffer, 
                       p_ecode);
          /*Free the allocated timer block here*/
         icf_dbm_ret_mem_to_pool(p_glb_pdb, ICF_MEM_TIMER,
                      p_glb_pdb->p_glb_data->p_dns_info->p_dns_timer_data, p_ecode);

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
        return ICF_FAILURE;
    }

    ICF_FUNCTION_ENTER(p_persistent_glb_pdb) /* function entry trace */

	if(p_persistent_glb_pdb->p_glb_data->p_dns_info == ICF_NULL)
	{
        ICF_PRINT(((icf_uint8_t *)"\n No data in the DNS INFO \n"));
        ret_val = ICF_FAILURE;
	}
	
    else
    {
        p_tmp_dns_buffer_list = p_persistent_glb_pdb->p_glb_data->p_dns_info->p_dns_buffer_list;

        if (p_tmp_dns_buffer_list == ICF_NULL)
        {
		    ICF_PRINT(((icf_uint8_t *)"\n No data in the DNS buffer \n"));
            ret_val = ICF_FAILURE;
        }
        else 
        {
            while (p_tmp_dns_buffer_list != ICF_NULL)
            {
                if (ICF_NULL == icf_port_strcmp(((icf_dns_buffer_st*)p_tmp_dns_buffer_list->p_data)->fqdn.str, \
                            p_fqdn))
                {
                    /*active_ip_index = ((icf_dns_buffer_st*)p_tmp_dns_buffer_list->p_data)->active_ip_index;
                      if(active_ip_index != ICF_DNS_INVALID_IP_INDEX)
                      {
                      icf_port_strcpy(p_active_ip,((icf_dns_buffer_st*)p_tmp_dns_buffer_list->p_data)->ip_address[active_ip_index]);
                      ret_val = ICF_SUCCESS;
                      }*/
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
   
    if(p_persistent_glb_pdb->p_call_ctx != ICF_NULL && 
            ICF_NULL != p_persistent_glb_pdb->p_call_ctx->p_sip_ctx)
    {
        /* search if the DNS info is stored here */
        if(((icf_ssa_ctxt_st*)p_persistent_glb_pdb->p_call_ctx->p_sip_ctx)->bitmask & ICF_SSA_CONTACT_IP_PRESENT)
        {
            if (ICF_NULL == icf_port_strcmp(((icf_ssa_ctxt_st*)p_persistent_glb_pdb->p_call_ctx->p_sip_ctx)->p_dns_ip_addr->fqdn.str,
                            p_fqdn))
                /* found the DNS record for contact address */
                *p_p_dns_record = ((icf_ssa_ctxt_st*)p_persistent_glb_pdb->p_call_ctx->p_sip_ctx)->p_dns_ip_addr;
                ICF_PRINT(((icf_uint8_t *)"\n Found the DNS record for the contact address \n"));
        }
    }

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
		IN     icf_dns_buffer_st  *p_query_dns_resp,
        INOUT  icf_ssa_ctxt_st     *p_ssa_ctxt,
        IN     Sdf_st_callObject       *p_call_obj,
		OUT    icf_error_t         *p_error)
{
	icf_return_t  			ret_val = ICF_FAILURE;
    icf_dns_buffer_st       *p_dns_buffer_to_store = ICF_NULL;
    icf_line_data_st		*p_line_data = ICF_NULL;
    Sdf_ty_s8bit             *p_sip_method = ICF_NULL;
    SipMessage		        *pMessage = Sdf_co_null;
    Sdf_st_error            sdf_error;
    
    ICF_FUNCTION_ENTER(p_persistent_glb_pdb);
	
    /* check if there is already an entry corresponding to the FQDN */
    if(ICF_SUCCESS == icf_ssa_dns_get_ip_from_fqdn(
                p_query_dns_resp->fqdn.str,&p_dns_buffer_to_store, p_error))
    {
        /* delete the existing entry and create the new one */
        if(ICF_FAILURE == (icf_ssa_dns_delete_fqdn_entry(&p_query_dns_resp->fqdn, p_error)))
           {
               /* not able to overwrite the entry */
               return ICF_SUCCESS;
           }
    }
    
    if(ICF_FAILURE == icf_dbm_fetch_line_cntxt_blk(
                p_persistent_glb_pdb, p_ssa_ctxt->line_id,
                &p_line_data, p_error))	
    {
        return ICF_FAILURE;
    }

    ICF_MEMGET(p_persistent_glb_pdb,
            ICF_PORT_SIZEOF(icf_dns_buffer_st),ICF_MEM_COMMON,\
            p_dns_buffer_to_store,\
            ICF_RET_ON_FAILURE,\
            p_error, ret_val);

    if(ICF_FAILURE == ret_val)
    {
        return ICF_FAILURE;
    }

    icf_port_memcpy(p_dns_buffer_to_store, p_query_dns_resp,sizeof(icf_dns_buffer_st));
   /* find the method here */
    if(ICF_NULL != p_call_obj->pUasTransaction->pSipMsg)
        pMessage = p_call_obj->pUasTransaction->pSipMsg;
    else
        pMessage = p_call_obj->pUacTransaction->pSipMsg;
    
    if(Sdf_co_fail == 
            sdf_ivk_uaGetMethodFromSipMessage(
                pMessage,
                &p_sip_method, &sdf_error))
    {
        ret_val = ICF_FAILURE;
    }

    /***********************/

    /* we will store the DNS entry only for proxy & registrar */
    if(((p_line_data->line_data_received & ICF_CFG_LINE_DATA_PROXY_RECD) &&
        p_line_data->proxy_addr.addr.addr_type == ICF_TRANSPORT_ADDRESS_DOMAIN_NAME &&
       ICF_NULL == icf_port_strcmp(p_line_data->proxy_addr.addr.addr.domain.str,
                   p_query_dns_resp->fqdn.str)) || 
      /*((p_line_data->line_data_received & ICF_CFG_LINE_DATA_REGISTRAR_RECD) &&
       p_line_data->registrar_addr.addr.addr_type == ICF_TRANSPORT_ADDRESS_DOMAIN_NAME &&
      ICF_NULL == icf_port_strcmp(p_line_data->registrar_addr.addr.addr.domain.str,
                   p_query_dns_resp->fqdn.str)))*/
        icf_port_strcmp((icf_uint8_t *)p_sip_method,(icf_uint8_t *)"REGISTER") == ICF_NULL)
    {

        if(ICF_NULL == p_persistent_glb_pdb->p_glb_data->p_dns_info ||
                ICF_NULL == p_persistent_glb_pdb->p_glb_data->p_dns_info->p_dns_buffer_list)
        {
            /* start the DNS Purge timer here */
            icf_config_data_st       *p_cfg_data = ICF_NULL;

            if(ICF_FAILURE == ret_val)
            {
                return ICF_FAILURE;
            }

            if (ICF_FAILURE == icf_dbm_get_module_glb_data(
                        p_persistent_glb_pdb,
                        (icf_int_module_id_t)ICF_INT_MODULE_CFG,
                        (icf_void_t *)&(p_cfg_data),
                        p_error))
            {
                ret_val = ICF_FAILURE;
                return ret_val;
            }		

            /* Below 60000 is put to convert the timer from minute to ms */
            if(p_cfg_data == ICF_NULL || (ICF_FAILURE == (icf_ssa_dns_start_timer(
                                p_persistent_glb_pdb,
                                ((60000*p_cfg_data->dns_purge_timer)/ICF_DNS_TIMER_DIVISOR), 
                                p_error))))
            {
                return ICF_FAILURE;
            }

        }
        p_dns_buffer_to_store->timer_count = ICF_DNS_TIMER_DIVISOR;
        if (ICF_FAILURE != icf_cmn_add_node_to_list( p_persistent_glb_pdb, \
                    &(p_persistent_glb_pdb->p_glb_data->p_dns_info->p_dns_buffer_list), p_dns_buffer_to_store))
        {
            ret_val = ICF_SUCCESS;
        }
    }
    else /*put the resolved ip address in the ssa context */
    {
        p_ssa_ctxt->bitmask |= ICF_SSA_CONTACT_IP_PRESENT;
        p_ssa_ctxt->p_dns_ip_addr = p_dns_buffer_to_store;

    }

    ICF_FUNCTION_EXIT(p_persistent_glb_pdb);
	return ret_val;

}

/*******************************************************************************
 **
 ** FUNCTION:     icf_ssa_dns_delete_fqdn_entry
 ** DESCRIPTION:  This function will receive the pointer of the node record to be deleted. 
 **
 ******************************************************************************/
icf_return_t    icf_ssa_dns_delete_fqdn_entry(
		IN     icf_string_st     	 *p_fqdn,
		OUT    icf_error_t           *p_error)
{
	icf_return_t 		ret_val = ICF_FAILURE;
	icf_list_st   		*p_tmp_list = ICF_NULL;

    if (ICF_NULL == p_persistent_glb_pdb)
    {
        return ICF_FAILURE;
    }
    ICF_FUNCTION_ENTER(p_persistent_glb_pdb);
	
	if (ICF_NULL == p_persistent_glb_pdb->p_glb_data->p_dns_info)
    {
        return ICF_FAILURE;
    }
    if(ICF_NULL == p_persistent_glb_pdb->p_glb_data->p_dns_info->p_dns_buffer_list)
        return ICF_SUCCESS;
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
        icf_string_st           temp_str_to_print;
        temp_str_to_print = *p_fqdn;
        if(ICF_FAILURE != icf_cmn_delete_node_frm_list(p_persistent_glb_pdb, \
                    &(p_persistent_glb_pdb->p_glb_data->p_dns_info->p_dns_buffer_list), 
		           &(p_tmp_list->p_data)))
        {
            ICF_PRINT(((icf_uint8_t *)"\n DNS Entry Deleted Successfully : %s \n",temp_str_to_print.str));
            if(ICF_NULL != p_temp_data)
            {
                ICF_MEMFREE(p_persistent_glb_pdb, p_temp_data, ICF_MEM_COMMON, p_error, ret_val)
            }
            ret_val = ICF_SUCCESS;
    
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
	icf_config_data_st       *p_cfg_data = ICF_NULL;
    ICF_FUNCTION_ENTER(p_persistent_glb_pdb);

	if (ICF_NULL == p_glb_pdb->p_glb_data->p_dns_info)
    {
        return ICF_FAILURE;
    }
	p_tmp_list = p_glb_pdb->p_glb_data->p_dns_info->p_dns_buffer_list;

	while (p_tmp_list != ICF_NULL)
	{
		p_tmp_node = p_tmp_list->p_data;
		((icf_dns_buffer_st*)(p_tmp_list->p_data))->timer_count--;
		if(((icf_dns_buffer_st*)(p_tmp_list->p_data))->timer_count <= 0)
		{
			p_tmp_list = p_tmp_list->p_next;

            if(ICF_FAILURE ==  icf_ssa_dns_delete_fqdn_entry(&p_tmp_node->fqdn,\
						       p_error))
			{
				ret_val = ICF_FAILURE;
			}
		}
        else
        {
            p_tmp_list = p_tmp_list->p_next;
        }
	}

    
    if((ICF_NULL != p_glb_pdb->p_glb_data->p_dns_info) &&
        (ICF_NULL !=  p_glb_pdb->p_glb_data->p_dns_info->p_dns_buffer_list))
	{
			/* free the timer memory here for the expired timer */
        if (ICF_NULL != p_glb_pdb->p_glb_data->p_dns_info && 
                ICF_NULL != p_glb_pdb->p_glb_data->p_dns_info->p_dns_timer_data)
        {
            /*icf_dbm_free_timer_block(p_glb_pdb,
              p_glb_pdb->p_glb_data->p_dns_info->p_dns_timer_data, \
              p_ecode);*/
            /*Free the allocated ssa timer block here*/
            icf_dbm_ret_mem_to_pool(p_glb_pdb, ICF_MEM_SSA_TIMER,
                    ((icf_timer_data_st*) p_glb_pdb->p_glb_data->p_dns_info->p_dns_timer_data)->p_timer_buffer, 
                    p_error);
            /*Free the allocated timer block here*/
            icf_dbm_ret_mem_to_pool(p_glb_pdb, ICF_MEM_TIMER,
                    p_glb_pdb->p_glb_data->p_dns_info->p_dns_timer_data, p_error);

        }
        /* start the DNS Purge timer here */


		if (ICF_FAILURE == icf_dbm_get_module_glb_data(
					p_glb_pdb,
					(icf_int_module_id_t)ICF_INT_MODULE_CFG,
					(icf_void_t *)&(p_cfg_data),
					 p_error))
		{
			ret_val = ICF_FAILURE;
			return ret_val;
		}		

		/* 60000 is put here to convert the timer from minute to ms */
        if(p_cfg_data == ICF_NULL || (ICF_FAILURE == (icf_ssa_dns_start_timer(
						p_glb_pdb,
						((60000 * p_cfg_data->dns_purge_timer)/ICF_DNS_TIMER_DIVISOR),
						p_error))))
		{
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
    icf_error_t         error;

    /*get the pointer to the DNS Buffer list */
    p_tmp_dns_buffer_list = p_glb_pdb->p_glb_data->p_dns_info->p_dns_buffer_list;
    while(ICF_NULL != p_tmp_dns_buffer_list)
    {
        /* call the delete entry function to delete all the entries one by one */
        p_next_node = p_tmp_dns_buffer_list->p_next;
        ret_val = icf_ssa_dns_delete_fqdn_entry(&(((icf_dns_buffer_st*)(p_tmp_dns_buffer_list->p_data))->fqdn), &error);
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
                IN  icf_void_t *p_ares_ctxt, 
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
    else
    {    
        /* fill the timer data structure */
        p_timer_data->timer_type = ICF_TIMER_TYPE_DNS_RESP_TIMER;
        p_timer_data->module_id = ICF_INT_MODULE_SSA;
        p_timer_data->p_timer_buffer = p_ssa_timer_data;
        p_ssa_timer_data->p_ssa_ctxt = p_ssa_ctxt;
        p_ssa_timer_data->p_uatk_ctxt_info =(icf_void_t*) p_dns_evt_ctxt;
        p_ssa_timer_data->p_ares_ctxt = (icf_void_t *)p_ares_ctxt; 
        p_ssa_timer_data->ares_qid = p_ssa_ctxt->ares_qid;

        /*Calculate the DNS timer value*/
        if(ICF_NULL == p_ssa_ctxt->dns_retrans_timer)
        {
            p_ssa_ctxt->dns_retrans_timer = p_cfg_data->dns_min_retrans;
        }
        /*CSR 8072456 SPR 20237*/
        /*Reset the bitmask while doing DNS query so that
        it should be set again depending on the response of DNS query*/
        p_ssa_ctxt->bitmask_1 &= ~ICF_SSA_DNS_RESP_RECEIVED;
        
        timer_dur = p_ssa_ctxt->dns_retrans_timer;
        p_ssa_ctxt->dns_retrans_timer = 2 * p_ssa_ctxt->dns_retrans_timer;
 
        p_timer_data->p_glb_pdb = p_ssa_ctxt->p_glb_pdb;
        p_ssa_ctxt->p_dns_resp_timer_data = p_timer_data;

        ICF_START_TIMER(p_ssa_ctxt->p_glb_pdb, p_timer_data, timer_id,
                        timer_dur, p_ecode, ret_val)

            if ( ICF_FAILURE == ret_val)
            {
                ICF_PRINT(((icf_uint8_t *)"\n UNABLE to start the DNS RESP TIMER \n"));

                /*Free the allocated ssa timer block here*/
                icf_dbm_ret_mem_to_pool(p_ssa_ctxt->p_glb_pdb, ICF_MEM_SSA_TIMER,
                        p_ssa_timer_data, 
                        p_ecode);
                
                /*Free the allocated timer block here*/
                icf_dbm_ret_mem_to_pool(p_ssa_ctxt->p_glb_pdb, ICF_MEM_TIMER,
                        p_timer_data, p_ecode);

                p_ssa_ctxt->p_dns_resp_timer_data = ICF_NULL;
                p_ssa_ctxt->dns_resp_timer_id = ICF_NULL;
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

  ICF_FUNCTION_ENTER(p_ssa_ctxt->p_glb_pdb) /* function entry trace */

	/* check if the timer is running or not */
  if(ICF_TIMER_ID_INVALID !=  p_ssa_ctxt->dns_resp_timer_id)
	{
             p_ssa_ctxt->bitmask_1 |= ICF_SSA_DNS_RESP_RECEIVED;
		/* Non-zero id means that the timer is running, so stop it */
		ICF_STOP_TIMER(p_ssa_ctxt->p_glb_pdb, p_ssa_ctxt->dns_resp_timer_id, p_ecode, \
			ret_val)
    ICF_PRINT(((icf_uint8_t *)"\n DNS RESP timer Stoped \n"));    

    /* free the memory allocated to any existing timer data block */
    if(ICF_NULL != p_ssa_ctxt->p_dns_resp_timer_data)
	  {
         /*Free the allocated ssa timer block here*/
         icf_dbm_ret_mem_to_pool(p_ssa_ctxt->p_glb_pdb, ICF_MEM_SSA_TIMER,
                                 p_ssa_ctxt->p_dns_resp_timer_data->p_timer_buffer,p_ecode);
         
         /*Free the allocated timer block here*/
         icf_dbm_ret_mem_to_pool(p_ssa_ctxt->p_glb_pdb, ICF_MEM_TIMER,
                                 p_ssa_ctxt->p_dns_resp_timer_data, p_ecode);
         
	  }
    p_ssa_ctxt->p_dns_resp_timer_data = ICF_NULL;
    p_ssa_ctxt->dns_resp_timer_id = ICF_TIMER_ID_INVALID;
	} /* end of if for timer_id */
	
  ICF_FUNCTION_EXIT(p_ssa_ctxt->p_glb_pdb);
	return ret_val;
}/* end of icf_ssa_dns_stop_timer() */

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
#endif
