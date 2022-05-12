/*****************************************************************************
 ** FUNCTION:
 **                     This file contains the implementation for all the 
 **                     SIP UA toolkit related common callbacks.
 **
 *****************************************************************************
 **
 ** FILENAME:           icf_ssa_commoncallbacks.c
 **
 ** DESCRIPTION:        This file contains the implementation for the
 **                     SIP UA toolkit related callbacks that both the
 **                     UAC and the UAS have to implement.
 **
 ** DATE                NAME            REFERENCE               REASON
 ** ----                ----            ---------               ------
 ** 29-JAN-2005     Aashish Suchdev
 **
 ** 24-MAY-2005     Umang Singh        SPR 7506
 ** 07-JUL-2005     Aashish Suchdev							TCP checkin            
 ** 07-DEC-2005		Jyoti Narula		ICF 4.0 LLD			Ecode setting for 
 **															timeout failure
 **															in case of REGISTER 
 **															request
 ** 01-Dec-2005		Mohan Dhawan		ICF 4.0 LLD			code merged for UT
 **															changes
 ** 02-Mar-2006     Amit Sharma                         ICF merging with ICF2.0
 ** 23-Mar-2006     Amit Sharma                         Code merged with ICF 4.2
 ** 30-Jun-2006     Umang Singh		Rel 5.2		        TLS support
 ** 03-Aug-2007     Tarun Gupta     CSR_1-4966233       Added a callback
 **                                                     sdf_cbk_uaGetIpFromNonSipUrl
 ** 23-Dec-2007     Abhishek Dhammawat SPR17855         Modified sdf_cbk_uaFreeApplicationData
 **                                                     sip_indicateMessageRetransmission 
 ** 24-Dec-2007     Abhishek Dhammawat SPR 17862        Modified 
 **                                                     sip_indicateMessageRetransmission
 **                                                      
 ** 22-Jan-2007     Shruti Thakral                      Enhancements for Rel 8.0
 ** 13-Feb-2008     Tarun Gupta     IPTK Rel 8.0        Changes done for AKA handling.
 ** 28-Mar-2008     Tarun Gupta     SPR 18311           Merged Publish-Message Enhancements
 ** 03-Apr-2008     Tarun Gupta     SPR 18355           Merged Windows Porting Changes
 ** 19-Apr-2008 Abhishek Dhammawat  SPR 18430           modified
 **                                                sdf_cbk_uaFreeApplicationData
 ** 03-Jul-2008 Anurag Khare        SPR 18684           Compilation issue on gcc 4.2.3
 ** 04-Nov-2008 Abhishek Dhammawat  SPR 19189      Merging of SPR 18846 from
 **                                                release 7.1.2
 ** 12-Nov-2008 Tarun Gupta         SPR 19189           Merged Rel 7.1.2 SPR 18872
 ** 12-Nov-2008 Anuradha Gupta      SPR 19188           Merged Rel 7.1.2 SPR 18937
 ** 19-Nov-2008 Abhishek Dhammawat  SPR 19189      Merging of SPR 18882 from
 **                                                release 7.1.2
 ** 26-Nov-2008 Abhishek Dhammawat  SPR 19189      Merged SPR 18882 cycle4
 ** 26-Nov-2008 Rajiv Kumar         SPR 19188      Rel 7.1.2 SPR merging (SPR
 **                                                19135)  
 ** 01-Dec-2008 Rajiv Kumar         SPR 19215      CSR-1-6212448 Merged
 ** 04-Dec-2008 Rajiv Kumar         SPR 19218      CSR-1-6291842 Merged
 ** 08-Dec-2008 Rajiv Kumar         SPR 19218      Merged CSR 1-6431768
 ** 13-Dec-2008 Anurag Khare        SPR 19218      CSR 1-6412398 Merged
 ** 29-Jan-2009 Alok Tiwari         Rel_8.1        implentation of callback
 **                                                sdf_cbk_uaResolvePresUrl
 ** 02-Mar-2009     Alok Tiwari      IPTK Rel8.1 IPTK open source replacement:
 **                                              Data related to ares has
 **                                              been removed.
 ** 18-Mar-2009 Rajiv Kumar         IPTK Rel8.1     UT Defect Fix     
 ** 09-Apr-2009 Rajiv Kumar         SPR 19428       SPR-19335 Merged 
 ** 27-Apr-2009 Tarun Gupta         SPR 19505       Modified sdf_cbk_uaFreeApplicationData
 ** 30-Apr-2009 Rajiv Kumar         SPR 19528       Modified Function 
 **                                                 sdf_cbk_uaIndicateTxnTimeOut 
 ** 01-May-2009 Anuradha Gupta      SPR 19535       Modified Function 
 **                                                 sdf_cbk_uaIndicateTxnTimeOut 
 ** 02-May-2009 Anuradha Gupta      SPR 19469       Modified Function 
 **                                                 sdf_cbk_uaIndicateTxnTimeOut 
 ** 05-Apr-2009 Rajiv Kumar         IPTK Rel8.2     Fix for SPR: 19593
 ** 20-May-2009 Kamal Ashraf        SPR 19672       CSR_1_1_7027627 Merged
 ** 22-May-2009 Kamal Ashraf        SPR 19672       CSR_1_1_7113244 Merged
 ** 25-May-2009 Kamal Ashraf        SPR 19672       SPR 19334
 ** 11-Jun-2009 Ashutosh Mohan      SPR 19737       Changes done for GCC 4.1 warnings removal
 ** 02-Sep-2009 Anuradha Gupta      SPR 19999       CSR 1-7493306 Merged
 ** 08-Sep-2009 Rajiv Kumar         SPR 19999       Merged CSR 1-7586127 
 ** 15-Sep-2009 Preksha             SPR 19999       Merged CSR 1-7694442 :SPR 19981 
 ** 15-Sep-2009 Rajiv Kumar         SPR 19999       Merged CSR 1-7605218
 ** 18-Sep-2009 Tarun Gupta         SPR 20061       Modified sdf_cbk_uaIndicateTxnTimeOut
 ** 05-Oct-2009 Anuradha Gupta      SPR 20082       Modified sdf_cbk_uaIndicateTxnTimeOut
 ** 08-Oct-2009 Abhishek Dhammawat  SPR 20063       Merged CSR 1-7407479
 ** 27-Oct-2009 Rajiv Kumar         SPR 20192       Fix For SPR:20192
 ** 28-Oct-2009 Anuradha Gupta      SPR 20194       Modified sdf_cbk_uaIndicateTxnTimeOut
 **
 *****************************************************************************
 **                 Copyrights 2009, Aricent.
 *****************************************************************************/

/*ICF Common inclusions*/
#include "icf_feature_flags.h"

#include "uatk_headers.h"
#include "stack_headers.h"

/*ICF Common inclusions*/
#include "icf_common_types.h"
#include "icf_macro.h"
#include "icf_internal_api_id.h"



#include "icf_dbm_prototypes.h"

/*ICF SSA inclusions*/
#include "icf_ssa_types.h"
#include "icf_ssa_macro.h"
#include "icf_ssa_prototypes.h"
#include "icf_ssa_defs.h"
#include "sdf_lookupport.h"
#ifdef ICF_DNS_LOOKUP_ENABLED
/* UATK inclusions */
#include "sdf_lookup.h"
#endif
#ifdef ICF_PORT_WINDOWS
#pragma warning(disable:4701)
#pragma warning ( disable : 4702 )
#endif



extern Sdf_ty_retVal sdf_fn_uaDnsQuery (Sdf_ty_s8bit  *pDomainName,
    Sdf_st_dnsEventContext   *pDnsEventContext,
    Sdf_st_error             *pErr);

/*****************************************************************************
 ** FUNCTION:    sdf_cbk_uaFreeApplicationData
 **
 ** DESCRIPTION: This callback is issued by the toolkit when
 **              sdf_ivk_uaFreeAppData is invoked and the reference count of
 **              the Sdf_st_appData structure is 0. It indicates that the
 **              toolkit no longer needs access to the structure. The
 **              application is passed the pData member of the Sdf_st_appData
 **              structure and may free the member at this stage if it
 **              wants to.
 **
 *****************************************************************************/
Sdf_ty_retVal sdf_cbk_uaFreeApplicationData(
        Sdf_ty_pvoid *ppData)
{
    icf_return_t         ret_val = ICF_SUCCESS;
    icf_error_t          ecode = ICF_ERROR_NONE;
    icf_ssa_ctxt_st      *p_ssa_ctxt = ICF_NULL;
    icf_glb_pdb_st       *p_glb_pdb = ICF_NULL;
    icf_ssa_pdb_st       ssa_pdb;
    Sdf_st_error         sdf_error = {0,0,0,"\0"};
    icf_uint8_t			 index = ICF_NULL;

    p_ssa_ctxt = (icf_ssa_ctxt_st*)(*ppData);
    if ( ICF_NULL != p_ssa_ctxt)
    {
        p_glb_pdb = p_ssa_ctxt->p_glb_pdb;

        icf_ssa_init_pdb(&ssa_pdb, p_glb_pdb);

        ssa_pdb.p_ssa_glb_data = p_ssa_ctxt->p_ssa_glb_data;
        ssa_pdb.p_glb_pdb = p_glb_pdb;
        ssa_pdb.p_ecode = &ecode;
        ssa_pdb.p_ssa_ctxt = p_ssa_ctxt;

        if (ICF_NULL != p_ssa_ctxt->p_rem_addr_active)
        {
            ICF_MEMFREE(
						ssa_pdb.p_glb_pdb,
						p_ssa_ctxt->p_rem_addr_active,
						ICF_MEM_COMMON,
						ssa_pdb.p_ecode,
						ret_val)
        }
        if (ICF_NULL != p_ssa_ctxt->p_rem_addr_inactive)
        {

            ICF_MEMFREE(
						ssa_pdb.p_glb_pdb,
						p_ssa_ctxt->p_rem_addr_inactive,
						ICF_MEM_COMMON,
						ssa_pdb.p_ecode,
						ret_val)
        }
        /*SPR 19886 reset the flag*/
        p_ssa_ctxt->invite_being_sent = ICF_FALSE;
        if(p_ssa_ctxt->p_call_obj)
        {
            if (Sdf_en_PermanentCallObject == p_ssa_ctxt->p_call_obj->dCallObjectType)
            {
#ifdef ICF_LOAD_DEBUG
                ICF_PRINT(("\nFreeAppData: Permanent CallObj\n"));
#endif
                if(p_ssa_ctxt->p_call_ctxt)
                {
                    if (p_ssa_ctxt->p_call_ctxt->p_sip_ctx == p_ssa_ctxt) 
                    {
#ifdef ICF_LOAD_DEBUG 
                        ICF_PRINT(("\n %d: Sip Ctx Matched\n", 
                               p_ssa_ctxt->call_id));
#endif
                        p_ssa_ctxt->p_call_ctxt->p_sip_ctx=ICF_NULL;
                    }
                    else
                    {
#ifdef ICF_LOAD_DEBUG 
                        ICF_PRINT(("\n %d Sip Ctx did not match\n",
                                p_ssa_ctxt->call_id));
#endif
                    }
                }
		    /*
             * Iterate thru all the open connections in conn_db and close
             * all connections that are for the given SSA Conteext and then
             * free SSA Context.
             */
                /* SPR 17855: Moved flush_alive_connections function above
                   to remove klocworks warning and redundant for callobj
                   type*/
                 icf_ssa_flush_alive_connections(&ssa_pdb);
             }    
        }


        /*
         * In this routine we will release the memory for SSA context 
         * whose pointer is the pData pointer and we will also reset the 
         * entry for this intance of ssa ctxt and the nw call id in the 
         * SSA-maintained map.
         */

		/*ICF_SSA_RESET_CALLID_CTXT_MAP(p_ssa_ctxt)*/

		if ( ICF_NULL != p_ssa_ctxt->p_hash_key)
		{
            icf_ssa_remove_ctxt_list_entry(p_glb_pdb,
                    p_ssa_ctxt);
#if 0
			icf_dbm_hash_remove_ssa_ctxt(p_glb_pdb,
				p_ssa_ctxt->p_hash_key,
				p_tmp_ssa_ctxt,&ecode);
#endif
	    	ICF_MEMFREE(
                p_glb_pdb,
                p_ssa_ctxt->p_hash_key,
                ICF_MEM_COMMON,
                &ecode,
                ret_val)
			p_ssa_ctxt->p_hash_key = ICF_NULL;
		}
        
        /*Fix for SPR: 19999 (Merged CSR 1-7586127 : SPR-19988)*/
        if (ICF_NULL != p_ssa_ctxt->p_via_hash_key)
        {
            icf_ssa_remove_via_mapped_ctxt_list_entry(p_glb_pdb,
                    p_ssa_ctxt);
        }

        if (ICF_NULL != p_ssa_ctxt->p_auth_params_list)
        {
            Sdf_st_error     sdf_error;
            sdf_listDeleteAll(p_ssa_ctxt->p_auth_params_list, &sdf_error);
            ICF_MEMFREE(
                p_glb_pdb,
                p_ssa_ctxt->p_auth_params_list,
                ICF_MEM_COMMON,
                &ecode,
                ret_val)
        }
        p_ssa_ctxt->auth_params_list_size = ICF_NULL; 
        /* Clear the contents of old nonce in ssa context */

        if(ICF_NULL != p_ssa_ctxt->auth_data_recd.p_nonce)
        {
            /* memfree the nonce string */
            ICF_MEMFREE(
                p_glb_pdb,
                (p_ssa_ctxt->auth_data_recd.p_nonce),
                ICF_MEM_COMMON,
                &ecode,
                ret_val)
        }

        if(ICF_NULL != p_ssa_ctxt->auth_data_sent.p_nonce)
        {
            /* memfree the nonce string */
            ICF_MEMFREE(
                p_glb_pdb,
                (p_ssa_ctxt->auth_data_sent.p_nonce),
                ICF_MEM_COMMON,
                &ecode,
                ret_val)
         }

#ifdef ICF_SIGCOMP_SUPPORT
		if(ICF_SUCCESS == icf_dbm_get_module_glb_data(
                    ssa_pdb.p_glb_pdb,
                    (icf_int_module_id_t)ICF_INT_MODULE_CFG,
                    (icf_void_t **)&(ssa_pdb.p_glb_cfg_data),
                    ssa_pdb.p_ecode))
		{
			/*
			 * Free the reference Counter of SIGSA CONTEXT as we are going to
			 * delete the SSA CONTEXT
			 */
			if (ICF_TRUE == ssa_pdb.p_glb_cfg_data->sigcomp_required)
			{
#ifdef ICF_IPSEC_ENABLED

				if (ICF_TRUE == ssa_pdb.p_glb_cfg_data->ipsec_enabled)
				{
					icf_rgm_glb_data_st		 *p_rgm_glb_data  = ICF_NULL;

					if (ICF_FAILURE == \
							icf_dbm_get_module_glb_data(ssa_pdb.p_glb_pdb,\
							ICF_INT_MODULE_REGM, 
							(icf_void_t **)(p_rgm_glb_data),
							ssa_pdb.p_ecode))
					{
						/*ICF_SIGSA_TRACE(("\nFailure while getting RGM Global Data in SIGSA\n"));*/

						return ICF_FAILURE;
					}

					/* Free the reference Counter of SIGSA CONTEXT */
					icf_sigsa_decr_ref_count_to_sigsa_ctxt(
											p_rgm_glb_data->p_sigsa_ctxt);
				}
				else
				{
					/*
					 * Free the reference Counter of SIGSA CONTEXT as we are going to
					 * delete the SSA CONTEXT
					 */
					if(ICF_NULL != p_ssa_ctxt->p_sigsa_ctxt)
					{
						icf_sigsa_decr_ref_count_to_sigsa_ctxt(
										p_ssa_ctxt->p_sigsa_ctxt);
					}
				}
#else
				if(ICF_NULL != p_ssa_ctxt->p_sigsa_ctxt)
				{
					icf_sigsa_decr_ref_count_to_sigsa_ctxt(
									p_ssa_ctxt->p_sigsa_ctxt);
				}
#endif 
			} /* if sigcomp_required */
		}/* if cfg_data */
#endif
        if (ICF_NULL != p_ssa_ctxt->p_sic_ctxt)
        {
            p_ssa_ctxt->p_sic_ctxt->p_ssa_ctx = ICF_NULL;
            /* Fix For SPR : 19135*/
            /* Since the application data ie SSA context will be freed here
             * in this function set the value of sic context in ssa context
             * as NULL*/
            p_ssa_ctxt->p_sic_ctxt = ICF_NULL;           
        }
        if(ICF_NULL != p_ssa_ctxt->p_rgm_ctxt)
        {
            p_ssa_ctxt->p_rgm_ctxt->p_ssa_ctxt = ICF_NULL;
            /* SPR 19334 Merged SPR 19672 Start */
            p_ssa_ctxt->p_rgm_ctxt = ICF_NULL;
            /* SPR 19334 Merged SPR 19672 End */
        } 
#ifdef ICF_DNS_LOOKUP_ENABLED
        if((p_ssa_ctxt->bitmask & ICF_SSA_CONTACT_IP_PRESENT) &&
            p_ssa_ctxt->p_dns_ip_addr != ICF_NULL)
    {
        ICF_MEMFREE(p_glb_pdb,p_ssa_ctxt->p_dns_ip_addr, \
                ICF_MEM_COMMON, &ecode, ret_val)
    }
    ICF_CMN_INIT_STRING(p_ssa_ctxt->fqdn);   
    p_ssa_ctxt->p_transaction = ICF_NULL;
    p_ssa_ctxt->p_overlap_transaction = ICF_NULL;
    if((ICF_TIMER_ID_INVALID == p_ssa_ctxt->dns_resp_timer_id) && 
             (ICF_NULL != p_ssa_ctxt->p_dns_resp_timer_data))
     {
         ICF_STOP_TIMER(p_glb_pdb,p_ssa_ctxt->dns_resp_timer_id,
                &ecode,ret_val)

        icf_dbm_ret_mem_to_pool(p_glb_pdb,
                ICF_MEM_TIMER,p_ssa_ctxt->p_dns_resp_timer_data,  
                &ecode);
     }
     p_ssa_ctxt->p_dns_resp_timer_data = ICF_NULL;
     p_ssa_ctxt->dns_resp_timer_id = ICF_TIMER_ID_INVALID;
     p_ssa_ctxt->ip_fr_req[0] = '\0';
     p_ssa_ctxt->port_fr_req = 0;
     p_ssa_ctxt->dns_retrans_timer = 0;
#endif

/*  CSR 1-5385208 Merge */
#ifdef ICF_TRACE_ENABLE
    if(ICF_NULL != p_ssa_ctxt->p_criteria_data)
     {
         if(ICF_NULL != p_ssa_ctxt->p_criteria_data->p_calling_party_address)
         {
             ICF_MEMFREE(
                 p_glb_pdb,
                 p_ssa_ctxt->p_criteria_data->p_calling_party_address,
                 ICF_MEM_COMMON,
                 &ecode,
                 ret_val)
         }

        if(ICF_NULL != p_ssa_ctxt->p_criteria_data->p_called_party_address)
         {
             ICF_MEMFREE(
                 p_glb_pdb,
                 p_ssa_ctxt->p_criteria_data->p_called_party_address,
                 ICF_MEM_COMMON,
                 &ecode,
                 ret_val)
         }

        ICF_MEMFREE(
            p_glb_pdb,
            p_ssa_ctxt->p_criteria_data,
            ICF_MEM_COMMON,
            &ecode,
            ret_val)
     }
#endif
    p_ssa_ctxt->bitmask = ICF_NULL;
     p_ssa_ctxt->bitmask_1 = ICF_NULL;
     p_ssa_ctxt->failure_bitmask = ICF_NULL;
    
#ifdef ICF_NAT_RPORT_SUPPORT     
     p_ssa_ctxt->rport_bitmask = ICF_NULL;
     p_ssa_ctxt->destination_port = ICF_NULL;
     p_ssa_ctxt->destination_ip[0] = '\0';
#endif

     p_ssa_ctxt->callobj_map_index = ICF_SSA_MAX_OPTIONS_CALL_OBJ;        
     p_ssa_ctxt->options_map_index = ICF_SSA_MAX_OPTIONS_CALL_OBJ;
#ifdef ICF_SESSION_TIMER
    p_ssa_ctxt->session_timer_expired_refresh = ICF_FALSE;
    p_ssa_ctxt->refresher_method_type    = ICF_SSA_REFRESHER_METHOD_NONE;
#endif

#ifdef IMS_CLIENT
     if((ICF_TIMER_ID_INVALID == p_ssa_ctxt->timer_id) && 
             (ICF_NULL != p_ssa_ctxt->p_timer_data))
     {
         ICF_STOP_TIMER(p_glb_pdb,p_ssa_ctxt->timer_id,
                &ecode,ret_val)

        icf_dbm_ret_mem_to_pool(p_glb_pdb,
                ICF_MEM_TIMER,p_ssa_ctxt->p_timer_data,  
                &ecode);
     }
     p_ssa_ctxt->timer_id = ICF_TIMER_ID_INVALID;
     p_ssa_ctxt->p_timer_data = ICF_NULL;
     p_ssa_ctxt->p_uac_auth_info_list = ICF_NULL;
     p_ssa_ctxt->uac_auth_info_list_size = ICF_NULL;
#endif

    if(ICF_NULL !=  p_ssa_ctxt->internal_msg_id)
    {  
        icf_ssa_free_buffered_msg_for_dns(&ssa_pdb);
        /* SPR 19334 Merged SPR 19672 Start */
        p_ssa_ctxt->internal_msg_id = ICF_NULL;
        /* SPR 19334 Merged SPR 19672 End */
    }
    if((ICF_TIMER_ID_INVALID == p_ssa_ctxt->options_timer_id) && 
             (ICF_NULL != p_ssa_ctxt->p_options_timer))
     {
         ICF_STOP_TIMER(p_glb_pdb,p_ssa_ctxt->options_timer_id,
                &ecode,ret_val)

        icf_dbm_ret_mem_to_pool(p_glb_pdb,
                ICF_MEM_TIMER,p_ssa_ctxt->p_options_timer,  
                &ecode);
     }
     p_ssa_ctxt->options_timer_id = ICF_TIMER_ID_INVALID;
     p_ssa_ctxt->p_options_timer = ICF_NULL;
     p_ssa_ctxt->context_identifier = ICF_NULL; 
     p_ssa_ctxt->no_retrans_req = ICF_FALSE;
     p_ssa_ctxt->p_conn_table_resp_entry = ICF_NULL;
     p_ssa_ctxt->p_conn_table_req_entry = ICF_NULL;
     p_ssa_ctxt->sdp_o_version = ICF_START_SDP_O_VERSION;

     for (index = 0; index < ICF_MAX_PENDING_PDU_PER_CONN; index++)
     {
         /* Fix for SPR 19505 :
            Freeing the memory allocated for p_pending_pdu 
         */
         if(ICF_NULL != p_ssa_ctxt->p_pending_pdu[index])
         {
             ICF_MEMFREE(p_glb_pdb,
                         p_ssa_ctxt->p_pending_pdu[index],
                         ICF_MEM_COMMON, 
                         &ecode,
                         ret_val)
         }
         p_ssa_ctxt->p_pending_pdu[index] = ICF_NULL;
     }
     p_ssa_ctxt->no_of_pending_pdu = 0;

	 for(index = 0; index < ICF_MAX_STREAM_PER_CALL; index++)
	 {
         ICF_CMN_INIT_STRING(p_ssa_ctxt->rej_stream[index].protocol_name)
         p_ssa_ctxt->rej_stream[index].stream_id =  ICF_STREAM_ID_INVALID;
     }

     /* stop all the running timers and return the allocated memory to timer pool
      */
     if (ICF_TRUE == icf_ssa_check_if_any_timers_exist(p_ssa_ctxt))
     {
        icf_ssa_timer_list_st      *p_timer_list = ICF_NULL;
        icf_ssa_timer_list_st      *p_temp_timer_list = ICF_NULL;

        p_timer_list = p_ssa_ctxt->p_timer_list;

        while (0 != p_ssa_ctxt->timer_count)
        {

            if (ICF_NULL != p_timer_list)
            {
                if(ICF_NULL != p_timer_list->p_timer_data)
                {
#ifdef ICF_SESSION_TIMER
                    icf_ssa_timer_data_st      *p_ssa_timer_data = 
                        (icf_ssa_timer_data_st *)p_timer_list->p_timer_data->p_timer_buffer;

                    if(Sdf_en_sessionTimer == p_ssa_timer_data->timer_type)  
                    {
                        ICF_CHECK_IF_ANY_OF_TWO_VAR_IS_NULL(p_glb_pdb, p_ssa_ctxt->p_call_obj, \
                                p_ssa_ctxt->p_call_obj->pSessionTimer, ret_val) 

                        if (ICF_FAILURE == ret_val)
                        {
                            /* session timer in call object does not exist, just stop
                             * the timer and free the timer data
                             */ 
                            ICF_STOP_TIMER(p_glb_pdb, p_timer_list->timer_id, 
                                &ecode, ret_val)

                            icf_dbm_ret_mem_to_pool(p_glb_pdb,
                                ICF_MEM_TIMER,p_timer_list->p_timer_data,  
                                &ecode);
                        }
                        else
                        {
                            sdf_fn_uaStopSessionTimer(\
                                (Sdf_ty_pvoid *)&(p_ssa_ctxt->p_call_obj->pSessionTimer->pTimerId), \
                                &sdf_error);
                        }
                    } 
                    else
                    {
#endif
                        ICF_STOP_TIMER(p_glb_pdb, p_timer_list->timer_id, 
                                &ecode, ret_val)
   
                        icf_dbm_ret_mem_to_pool(p_glb_pdb,
                                ICF_MEM_TIMER,p_timer_list->p_timer_data,  
                                &ecode);    
#ifdef ICF_SESSION_TIMER
                    } 
#endif
                } 
                p_temp_timer_list = p_timer_list;
                p_timer_list = p_timer_list->p_next;
                ICF_MEMFREE(
                    p_glb_pdb,
                    p_temp_timer_list,
                    ICF_MEM_COMMON,
                    &ecode,
                    ret_val)

            } /* if (ICF_NULL != p_timer_lis... */   
            else
            {
                /* timer data list is NULL */
                break;
            }         
            p_ssa_ctxt->timer_count--;
        }       
    }

     p_ssa_ctxt->p_timer_list = ICF_NULL;
     p_ssa_ctxt->timer_count = ICF_NULL;

#ifdef ICF_RUNTIME_NAMESERVER_CONFIG
	p_ssa_ctxt->refresh_nameserver = ICF_FALSE;
#endif     
    p_ssa_ctxt->forked_resp_counter = ICF_NULL; 
    p_ssa_ctxt->p_pending_txn = ICF_NULL;
    p_ssa_ctxt->revert_mode = ICF_FALSE;
    p_ssa_ctxt->trans_mode_reverted = ICF_FALSE;
    p_ssa_ctxt->p_ssa_glb_data = ICF_NULL;
    p_ssa_ctxt->p_glb_pdb = ICF_NULL;   
    p_ssa_ctxt->p_ssa_pdb = ICF_NULL;
    p_ssa_ctxt->p_call_obj = ICF_NULL;
    p_ssa_ctxt->transport = ICF_TRANSPORT_TYPE_UDP;
    p_ssa_ctxt->line_id= ICF_INVALID_LINE_ID;
    p_ssa_ctxt->call_type = ICF_NULL;
    p_ssa_ctxt->nc = 0;
    p_ssa_ctxt->p_rem_addr_active = ICF_NULL;
    p_ssa_ctxt->p_rem_addr_inactive = ICF_NULL;
    p_ssa_ctxt->dns_servers_tried = ICF_NULL;
    p_ssa_ctxt->rem_servers_tried = ICF_NULL;
    p_ssa_ctxt->p_current_dns_server = ICF_NULL;
    for(index = 0; index < ICF_SSA_MAX_FQDN_ENTRIES; index++)
    {
        ICF_CMN_INIT_STRING(p_ssa_ctxt->fqdn_list[index])
    }
    icf_cmn_init_reg_auth_data(&(p_ssa_ctxt->auth_data_sent));
    icf_cmn_init_reg_auth_data(&(p_ssa_ctxt->auth_data_recd));
    p_ssa_ctxt->challenge_algo = ICF_NULL;
    p_ssa_ctxt->p_dns_server_data = ICF_NULL;
    
    /*Fix For SPR: 19593*/
    p_ssa_ctxt->p_dns_event_context = ICF_NULL;
    /* SPR 19334 Merged SPR 19672 Start */
    p_ssa_ctxt->num_491_retries_allowed = ICF_SESSION_REFRESH_NUM_491_RETRY_ALWD;
    /* SPR 19334 Merged SPR 19672 End */ 
#ifdef ICF_IPSEC_ENABLED
    p_ssa_ctxt->p_criteria_data = ICF_NULL;
    /* SPR 19334 Merged SPR 19672 Start */
    p_ssa_ctxt->ssa_sa_context.p_sa_context = ICF_NULL;
    p_ssa_ctxt->ssa_sa_context.p_rgm_context = ICF_NULL;
    /* SPR 19334 Merged SPR 19672 End */
#endif
    /* SPR 19334 Merged SPR 19672 Start */
    p_ssa_ctxt->p_dns_query_internal_msg = ICF_NULL;
	if ((ICF_NULL != p_ssa_ctxt->p_call_ctxt) &&
		(p_ssa_ctxt->p_call_ctxt->p_sip_ctx == p_ssa_ctxt))
	{
		p_ssa_ctxt->p_call_ctxt->p_sip_ctx = ICF_NULL;
		p_ssa_ctxt->p_call_ctxt = ICF_NULL;
	}

    /* Fix for SPR: 19999(Merged CSR 1-7605218: SPR-19997 fixed)*/
    icf_cmn_init_address(&(p_ssa_ctxt->req_uri));
    /*SPR 20063 CSR 1-7407479 merge */
    p_persistent_glb_pdb->no_of_call_obj--; 
    /*SPR 20063 CSR 1-7407479 merge */
    /* SPR 19334 Merged SPR 19672 End */
    ICF_MEMFREE(p_glb_pdb,p_ssa_ctxt, \
                ICF_MEM_COMMON, &ecode, ret_val)

        *ppData = Sdf_co_null;
    }

    return Sdf_co_success;
}


/*****************************************************************************
 ** FUNCTION:    sdf_cbk_uaCloneAppData
 **
 ** DESCRIPTION: This is issued by the toolkit to the application when
 **              the CallObject is to be cloned. The application should
 **              clone the pData member of the AppData structure
 **              to copy the source to the destination.
 **
 *****************************************************************************/
Sdf_ty_retVal sdf_cbk_uaCloneAppData(
        Sdf_st_appData *pDestInfo,
        Sdf_st_appData *pSrcInfo,
        Sdf_st_error *pErr)
{
    /* To remove compiler warning */
    (void)pErr;

    /* Simply copy the source ptr into the destination ptr */
    pDestInfo = pSrcInfo;

    return Sdf_co_success;
}

/*****************************************************************************
 ** FUNCTION:    sdf_cbk_uaCloneEventContextpData
 **
 ** DESCRIPTION: This is issued by the toolkit to the application when
 **              the CallObject is to be cloned. The application should
 **              clone the pData member of the Event Context structure
 **              to copy the source to the destination.
 **
 *****************************************************************************/
Sdf_ty_retVal sdf_cbk_uaCloneEventContextpData(
        Sdf_st_eventContext *pDestInfo,
        Sdf_st_eventContext *pSrcInfo,
        Sdf_st_error *pErr)
{
    /* To remove compiler warning */
    (void)pErr;

    /* Simply copy the source ptr into the destination ptr */
    pDestInfo->pData = pSrcInfo->pData;

    return Sdf_co_success;
}


/*****************************************************************************
** FUNCTION: sdf_cbk_uaFreeEventContextData
**
**
** DESCRIPTION: This callback is issued by the toolkit when
**              sdf_ivk_uaFreeEventContext is invoked and the
**              reference count of the Sdf_st_eventContext structure
**              is 0. It indicates that the toolkit no longer needs
**              access to the structure. The application is passed
**              the pData member of the Sdf_st_eventContext
**              structure and may free the member at this stage if it wants to.
**
**  PARAMETERS:
**              ppData(IN)      : Pointer to the void* member of the
**                                Sdf_st_eventContext structure
******************************************************************************/
Sdf_ty_retVal sdf_cbk_uaFreeEventContextData(
        Sdf_ty_pvoid *ppData)
{
    /* To remove compiler warning */
    (void)ppData;

    return Sdf_co_success;
}

#ifdef SIP_AUTHENTICATE
/*****************************************************************************
 ** FUNCTION:           sdf_cbk_uaAuthenticateUser
 **
 ** DESCRIPTION:        This callback is issued by the toolkit when it needs to
 **                     verify the credentials of a user. It is issued by the
 **                     sdf_ivk_uaAuthorize API when the request contains an
 **                     Authorization header, and the toolkit needs to verify if
 **                     the credentials in the header are valid. The application
 **                     should return the clear-text password (in case of Basic)
 **                     or H(A1) (in case of Digest) from its database.
 **
 *****************************************************************************/
Sdf_ty_authResult sdf_cbk_uaAuthenticateUser(
        Sdf_st_callObject **ppCallObj,
        SipAddrSpec *pAddrSpec,
        Sdf_st_uasAuthInfo *pUasAuthInfo,
        Sdf_ty_pvoid *ppContext,
        Sdf_st_error *pErr)
{
    /* WILL BE IMPLEMENTED LATER */
    /* To remove compiler warning */
    (void)ppCallObj;
    (void)pAddrSpec;
    (void)pUasAuthInfo;
    (void)ppContext;
    (void)pErr;

    return Sdf_co_success;
}

/*****************************************************************************
** FUNCTION: sdf_cbk_uaChallengeUser
**
** DESCRIPTION: This callback is issued by the toolkit under
**              two scenarios:
**              a) If a request arrives without Authorization header.
**                 The application should look up it's database, and
**                 inform the toolkit if the user needs to be challenged
**                 and if so, with what parameters.
**              b) If a request arrives with a Authorization header
**                 but the credentials do not match with that
**                 present in the application. In this case, the
**                 application is issued the callback to re-challenge
**                 the user with new/same parameters
**                 The application should indicate whether the user is to be
**                 challenged, and if so, return the realm (in case of Basic)
**                 OR the {realm, opaque, qop, algorithm, domain, stale,
**                 nonce} (in case of Digest)
**
*****************************************************************************/
Sdf_ty_authResult sdf_cbk_uaChallengeUser(
        Sdf_st_callObject **ppCallObj,
        SipAddrSpec *pAddrSpec,
        Sdf_st_authenticationParams *pAuthParams,
        Sdf_ty_pvoid *ppContext,
        Sdf_st_error *pErr)
{
    /* WILL BE IMPLEMENTED LATER */
    /* To remove compiler warning */
    (void)ppCallObj;
    (void)pAddrSpec;
    (void)pAuthParams;
    (void)ppContext;
    (void)pErr;

    return Sdf_co_success;

}

#endif /* SIP_AUTHENTICATE */

#ifdef SDF_TXN
/*****************************************************************************
 ** FUNCTION:           sdf_cbk_uaIndicateTxnTimeOut
 **
 ** DESCRIPTION:        This callback is issued by the toolkit when
 **
 *****************************************************************************/
Sdf_ty_retVal sdf_cbk_uaIndicateTxnTimeOut(
        Sdf_st_callObject                      *pCallObj,
        Sdf_st_overlapTransInfo                *pOverlapTxn,
        Sdf_ty_txnTimerType                    dTimerType,
        Sdf_ty_pvoid                           pContext,
        Sdf_st_error                           *pError)
{
    icf_return_t                    ret_val = ICF_SUCCESS;
    icf_return_t                    ret_value = ICF_SUCCESS;
    icf_rgm_nw_resp_st              *p_rgm_msg = ICF_NULL;
	icf_nw_inc_refer_resp_st        *p_refer_resp = ICF_NULL;
	icf_nw_inc_msg_resp_st			*p_msg_resp = ICF_NULL;
	icf_nw_inc_subs_resp_st			*p_subs_resp = ICF_NULL;
	icf_nw_inc_publish_resp_st      *p_publish_resp = ICF_NULL;
	icf_nw_inc_opt_resp_st			*p_opt_resp = ICF_NULL;
    /*  Fix for spr 18882 */
    Sdf_st_error                    sdf_error = {0,0,0,"\0"};
#ifdef ICF_DNS_LOOKUP_ENABLED
    icf_error_t                     err = 0;
    icf_uint8_t				        dns_map_change_cause = ICF_DNS_MAP_CHANGE_INVALID;
#endif
    Sdf_st_transaction              *pTransaction = Sdf_co_null;
    icf_ssa_ctxt_st                 *p_ssa_ctxt = ICF_NULL;
    icf_glb_pdb_st                  *p_glb_pdb = ICF_NULL;
    icf_internal_msg_st             *p_internal_msg = ICF_NULL;
    icf_int_module_id_t             dest_module_id = ICF_INT_MODULE_INVALID;
#ifdef ICF_IPSEC_ENABLED
    icf_ssa_pdb_st                  *p_ssa_pdb = ICF_NULL;
#endif
    /*  Fix for spr 18882 */
    SipOptions                      dOptions = {0} ;
    SipMessage	                    *p_sip_msg = ICF_NULL;	
	Sdf_ty_s8bit                    *p_method = Sdf_co_null;
    Sdf_ty_messageType 	            dMsgType = Sdf_en_unknownRequest;
    en_SipMessageType 	            dReqRespType = SipMessageAny;
    en_SipMessageType 	            dReqRespTypeTemp = SipMessageAny;
    /*  Fix for spr 18882 */
    Sdf_ty_u16bit                   resp_code = 0;

    /*Start SPR 18846*/
    if(Sdf_co_null != pContext)
    {   
       p_ssa_ctxt = (icf_ssa_ctxt_st*)((Sdf_st_appData*)pContext);
    }  
    /*Normally we do not access GLB PDB  through p_ssa_ctxt.
     * This is special handling for timer exprie */
    if((ICF_NULL != p_ssa_ctxt) &&
       (ICF_NULL != p_ssa_ctxt->p_ssa_pdb))
    {   
        p_glb_pdb = ((icf_ssa_pdb_st*)(p_ssa_ctxt->p_ssa_pdb))->p_glb_pdb;
        p_internal_msg = ((icf_ssa_pdb_st*)(p_ssa_ctxt->p_ssa_pdb))->p_internal_msg;
    }
    else
    {
        ICF_PRINT(((icf_uint8_t *)"\nSSA PDB/Context is NULL.\n"))
        return Sdf_co_fail;
    }/*End SPR18846 */
    /*  Fix for spr 18882*/
    if (ICF_NULL == p_glb_pdb)
    {
        ICF_PRINT(((icf_uint8_t *)"\nGLB PDB is NULL.\n"))
        return Sdf_co_fail;
    }
    ICF_FUNCTION_ENTER(p_glb_pdb)   
    /* Fix for spr 18882 */
    ICF_CHECK_IF_VAR_IS_NULL(p_glb_pdb,p_ssa_ctxt->p_ssa_pdb->p_ssa_ctxt,ret_val)
    if (ICF_FAILURE == ret_val)
    {
		ICF_FUNCTION_EXIT(p_glb_pdb)    
        return Sdf_co_fail;
    } 
    ICF_CHECK_IF_ANY_OF_TWO_VAR_IS_NULL(p_glb_pdb,pCallObj,pError,ret_val)
    if (ICF_FAILURE == ret_val)
    {
		ICF_FUNCTION_EXIT(p_glb_pdb)    
        return Sdf_co_fail;
    } 
#ifdef ICF_IPSEC_ENABLED
	p_ssa_pdb = (icf_ssa_pdb_st*)p_ssa_ctxt->p_ssa_pdb;
#endif
    ret_val = ret_val; 

   /* SPR 19469: The protective check of SPR 19355 is relocated.
    * This function does some logic on the basis of certain types
    * of timer expiry, for example when the expired timer is timerH
    * then on final failure reponse,call object is freed. the protective
    * code check has been moved to places where 1) DNS Query is 
    * reattempted, 2) Request is retired on secondary address, 3)
    * when timer type specific handling is done.
    */ 	

    /* check if the record was picked up from the DNS buffer if 
     * yes then delete the DNS buffer entry*/
    if (Sdf_co_null == pOverlapTxn)
    {
        if((dTimerType == Sdf_en_timerB) || (dTimerType == Sdf_en_timerF) ||\
                (dTimerType == Sdf_en_timerNoAnswer))
        {
            /*  Fix for spr 18882 */
            ICF_CHECK_IF_VAR_IS_NULL(p_glb_pdb,pCallObj->pUacTransaction,ret_val)
            if (ICF_SUCCESS == ret_val)
            {
               p_sip_msg = pCallObj->pUacTransaction->pSipMsg;
               pTransaction =  pCallObj->pUacTransaction;
            }   
        }
        else
        {
            /*  Fix for spr 18882 */
            ICF_CHECK_IF_VAR_IS_NULL(p_glb_pdb,pCallObj->pUasTransaction,ret_val)
            if (ICF_SUCCESS == ret_val)
            {
               p_sip_msg = pCallObj->pUasTransaction->pSipMsg;
               pTransaction =  pCallObj->pUasTransaction;
            }   
        }
    }
    else
    {
        p_sip_msg = pOverlapTxn->pSipMsg;
    }
#ifdef ICF_DNS_LOOKUP_ENABLED
       /*  Fix for spr 18882 */
    ICF_CHECK_IF_VAR_IS_NULL(p_glb_pdb,p_sip_msg,ret_val)
    if(ICF_SUCCESS == ret_val)
    {
        /*IPTK should not retry on next available address if its a forceful call clear*/
        /*CSR 1-7694442 SPR 19981- IPTK shall not clear the DNS entries on NoAnswer Timer expiry.*/

        if ((p_ssa_ctxt->bitmask & ICF_DNS_RECORD_FETCHED) && 
            (dTimerType != Sdf_en_timerNoAnswer) &&
            (ICF_FALSE == p_ssa_ctxt->no_retrans_req))
        {
            p_ssa_ctxt->rem_servers_tried = 1;
            /*Reset the TxnDNSState to send DNS query again*/
            if(Sdf_co_null == pOverlapTxn)
            {
                if(Sdf_en_Idle != pTransaction->dTxnDnsState)
                {
                    pTransaction->dTxnDnsState = Sdf_en_Idle;  
                }
            }
            else
            {
                if(Sdf_en_Idle != pOverlapTxn->dTxnDnsState)
                {
                   pOverlapTxn->dTxnDnsState = Sdf_en_Idle;
                }   
            }

            sip_getMessageType(p_sip_msg, &dReqRespTypeTemp, (SipError*)
                        &(pError->stkErrCode));

            if ((SipMessageRequest == dReqRespTypeTemp) &&
                    (p_ssa_ctxt->p_conn_table_req_entry != ICF_NULL))
            {
                icf_nw_interface_info_st nw_interface_info;
                nw_interface_info.bitmap = ICF_INTF_INFO_PROTOCOL_TCP;
                nw_interface_info.receiver_fd = 
                p_ssa_ctxt->p_conn_table_req_entry->conn_fd;
#ifdef ICF_SECURE_SUPPORT
                if (p_ssa_ctxt->p_conn_table_req_entry->prot_conn_bitmask &
                        ICF_TRANSPORT_TLS)
                {
                    nw_interface_info.bitmap = ICF_INTF_INFO_PROTOCOL_TLS;
                }
#endif
                ICF_PRINT(((icf_uint8_t *)"Closing the Request connection\n"))
                /* Fix for spr 18882*/
                ICF_CHECK_IF_VAR_IS_NULL(p_glb_pdb,p_ssa_ctxt->p_ssa_pdb->p_glb_pdb->p_glb_data,ret_val)
                if(ICF_FAILURE == ret_val)
                {
		            ICF_FUNCTION_EXIT(p_glb_pdb)    
                    return Sdf_co_fail;
                }
                icf_es_close_tcp_conn(
                        p_ssa_ctxt->p_ssa_pdb->p_glb_pdb->p_glb_data->p_glb_port_info, 
                        &nw_interface_info, p_ssa_ctxt->p_ssa_pdb->p_ecode);

                icf_ssa_remove_conn_entry(p_ssa_ctxt->p_ssa_pdb,
                        p_ssa_ctxt->p_conn_table_req_entry->conn_fd);
                /*  Fix for spr 18882 */
                p_ssa_ctxt->p_conn_table_req_entry->conn_fd = ICF_INVALID_SOCKFD; 
                /*  Fix for spr 18882 */
                ICF_CHECK_IF_VAR_IS_NULL(p_glb_pdb,pCallObj->pUacTransaction,ret_val)
                if (ICF_SUCCESS == ret_val)
                {
                   pCallObj->pUacTransaction->dTxnDnsState = Sdf_en_Idle;
                }   

            }
            else if ((SipMessageResponse == dReqRespTypeTemp) &&
                    (p_ssa_ctxt->p_conn_table_resp_entry != ICF_NULL))
            {
                icf_nw_interface_info_st nw_interface_info;
                nw_interface_info.bitmap = ICF_INTF_INFO_PROTOCOL_TCP;
                nw_interface_info.receiver_fd = 
                p_ssa_ctxt->p_conn_table_resp_entry->conn_fd;
#ifdef ICF_SECURE_SUPPORT
                if (p_ssa_ctxt->p_conn_table_resp_entry->prot_conn_bitmask &
                        ICF_TRANSPORT_TLS)
                {
                    nw_interface_info.bitmap = ICF_INTF_INFO_PROTOCOL_TLS;
                }
#endif
                ICF_PRINT(((icf_uint8_t *)"Closing the Request connection\n"))
                /* Fix for spr 18882*/
                ICF_CHECK_IF_VAR_IS_NULL(p_glb_pdb,p_ssa_ctxt->p_ssa_pdb->p_glb_pdb->p_glb_data,ret_val)
                if(ICF_FAILURE == ret_val)
                {
		            ICF_FUNCTION_EXIT(p_glb_pdb)    
                    return Sdf_co_fail;
                }
                icf_es_close_tcp_conn(
                        p_ssa_ctxt->p_ssa_pdb->p_glb_pdb->p_glb_data->p_glb_port_info, 
                        &nw_interface_info, p_ssa_ctxt->p_ssa_pdb->p_ecode);
                icf_ssa_remove_conn_entry(p_ssa_ctxt->p_ssa_pdb,
                        p_ssa_ctxt->p_conn_table_resp_entry->conn_fd);
                /*  Fix for spr 18882 */
                p_ssa_ctxt->p_conn_table_resp_entry->conn_fd = ICF_INVALID_SOCKFD;
                /*  Fix for spr 18882 */
                ICF_CHECK_IF_VAR_IS_NULL(p_glb_pdb,pCallObj->pUasTransaction,ret_val)
                if (ICF_SUCCESS == ret_val)
                {
                   pCallObj->pUasTransaction->dTxnDnsState = Sdf_en_Idle;
                }   
            }

            p_ssa_ctxt->bitmask &= ~ICF_DNS_RECORD_FETCHED;
			if (ICF_TRUE == p_glb_pdb->p_glb_data->recvd_restart_reinit_event)
			{
				dns_map_change_cause = ICF_DNS_MAP_CHANGE_SYSTEM_REINIT;
			}
			else
			{
				dns_map_change_cause = ICF_DNS_MAP_CHANGE_TXN_TIMEOUT;
			}
            /* Fix For SPR:20192*/
            icf_ssa_dns_delete_fqdn_entry(&p_ssa_ctxt->fqdn, 
                    &p_ssa_ctxt->naptr_fqdn,&p_ssa_ctxt->srv_fqdn,
                    &p_ssa_ctxt->queryA_fqdn,&err, 
                    dns_map_change_cause);    


            /* Fix for SPR 19335 : Protective code */
            /* If CC, REGM, SIC contexts in SSA context are null, and the timer list in
             * SSA context is also empty, it means that ideally SSA context, CallObj
             * should also have been freed by now but may still have remained due to 
             * some unforeseen error scenario. In that case, free them now to avoid
             * any potential memory issues or crashes.
             */
            if ((ICF_NULL == p_ssa_ctxt->p_call_ctxt) &&
                    (ICF_NULL == p_ssa_ctxt->p_rgm_ctxt) &&
                    (ICF_NULL == p_ssa_ctxt->p_sic_ctxt) &&
                    (ICF_FALSE == icf_ssa_check_if_any_timers_exist(p_ssa_ctxt)))
            {
                icf_error_data_st		err_data;

                /* ------------------------------------------------------ */
                /* Catch the unexpected scenario */
                icf_port_memset(
                        (icf_void_st *)&err_data, 0, ICF_PORT_SIZEOF(icf_error_data_st));
                err_data.num_of_integer_values = 1;
                err_data.number_of_string_values = 0;
                err_data.int_val[0] = p_ssa_ctxt->p_call_obj->dRefCount;
                icf_port_strcpy(
                        err_data.err_string.str,
                        (icf_uint8_t*)("Transaction timeout after CC/REGM/SIC freed"));
                err_data.err_string.str_len = icf_port_strlen(err_data.err_string.str);

                icf_port_check_unexpected_scenario(p_glb_pdb, &err_data);

                /* ------------------------------------------------------ */
                /* Close all open sockets for this call */
                sdf_ivk_uaCloseAllSocketsForCall(p_ssa_ctxt->p_call_obj, pError);

                /* Abort any pending transaction */
                sdf_ivk_uaAbortCall(p_ssa_ctxt->p_call_obj, pContext, pError);


                return Sdf_co_success;
            }

            /*CSR 1-6291842: If Forceful call clear is initiated then don't 
             * send DNS query again.As the call context will be de-allocted in the
             * return path. 
             */
            if((ICF_NULL != p_ssa_ctxt->p_call_ctxt) &&
                    (ICF_TRUE == p_ssa_ctxt->p_call_ctxt->force_clr_rcvd))
            {
                ICF_FUNCTION_EXIT(p_glb_pdb)
                return ret_val;
            }
            else 
            {
                /* Fix for SPR:19528*/
                /* Perform the fresh DNS query only if the SIP message type is
                 * request.This is done as for any SIP response IPTK never perform
                 * the DNS query and further if there is transaction timeout
                 * for any response for which ACK is awaited IPTK must clear
                 * the call gracefully as remote party is not responding.*/
                if (SipMessageRequest == dReqRespTypeTemp)
                {
                    /*Delete existing transaction and create new transaction 
                     * to send request on new remote party address*/
                    if (ICF_FAILURE == icf_ssa_create_next_transaction(pCallObj,
                                pOverlapTxn, &sdf_error))
                    {
                        ret_val = ICF_FAILURE;
                    }
                    /* 7.0 DNS enhancement . 
                     * New DNS query is fired and earlier record is deleted*/
                    else if ( Sdf_co_fail == sdf_ivk_uaHandleLookup(pCallObj,
                                pOverlapTxn, p_sip_msg,
                                p_ssa_ctxt->p_ssa_pdb->p_event_ctxt,&sdf_error) &&
                            ( Sdf_en_dnsInvState != sdf_error.errCode))
                    {
                        ICF_PRINT(((icf_uint8_t *)"\n[SSA]:FailureIn HandleLookUp API"));
                        ret_val = ICF_FAILURE;
                    }
                    else if ((Sdf_co_false == sdf_ivk_uaIsDnsQueryState(pCallObj,
                                    pOverlapTxn, &sdf_error)))
                        /* ||                   (ICF_NULL == (p_ssa_ctxt->bitmask & 
                           ICF_SSA_DNS_FQDN_PRESENT )))*/
                    {
                        ret_val = ICF_FAILURE;
                    }
                    else
                    {
                        ret_val =  ICF_SUCCESS;
                    }
                    /* We return from here as now next processing shall be done when response
                     * will be received from DNS query */
                    return ret_val;
                }
            }/* End of Change for CSR 1-6291842*/
        }
    }
#endif

    if (ICF_NULL != p_sip_msg)
    {
      if( SipFail != sip_getMessageType(p_sip_msg, &dReqRespType, (SipError*)
            &(pError->stkErrCode)))
      {
        /* 
         * If the call context is not null fetch the response code which
         * would be sent to the application
         */
        if(ICF_NULL != p_ssa_ctxt->p_ssa_pdb->p_ssa_ctxt->p_call_ctxt && 
            SipMessageResponse == dReqRespType)
        {
          if (Sdf_co_fail != sdf_ivk_uaGetRespCodeFromSipMessage(
                p_sip_msg, &resp_code, pError))
          {
            p_ssa_ctxt->p_ssa_pdb->p_ssa_ctxt->p_call_ctxt->\
              inc_response_code = resp_code;
          }
          else
          {
            pError->errCode = Sdf_en_noUaError;
            ICF_FUNCTION_EXIT(p_glb_pdb)
              return Sdf_co_fail;
          }
        }

        if(Sdf_co_fail == 
            sdf_ivk_uaGetMethodFromSipMessage(p_sip_msg, &p_method, pError))

        {
          pError->errCode = Sdf_en_noUaError;

          ICF_FUNCTION_EXIT(p_glb_pdb)    
            return Sdf_co_fail;
        }
      }
      else
      {
        pError->errCode = Sdf_en_noUaError;
        ICF_FUNCTION_EXIT(p_glb_pdb)
          return Sdf_co_fail;
      }
/* CSR 1-6212448: SPR 18873: Invoke trace only when sip message is not null.
 */    
#ifdef ICF_TRACE_ENABLE    
    p_ssa_ctxt->p_ssa_pdb->p_method = (icf_uint8_t *)p_method;
    ICF_SSA_APP_TRACE(p_ssa_ctxt->p_ssa_pdb, 
                   ICF_TRACE_SIP_MSG_RETRANSMISSION_TIMEOUT)
#endif 
    }
	if(p_method)
		sdf_fn_uaGetMethodNameAsEnum(p_method, &dMsgType, pError);

	dOptions.dOption = SIP_OPT_CLEN|SIP_OPT_FULLFORM;

    if(dTimerType == Sdf_en_timerH)
    {
        /* ACK wait timer expired */
        /*  Fix for spr18882 */
        ICF_CHECK_IF_VAR_IS_NULL(p_glb_pdb,pCallObj->pCallInfo,ret_val)
        if (ret_val == ICF_SUCCESS)
        {
            if((Sdf_en_finalSuccessSent == 
                        pCallObj->pCallInfo->dState) ||
                    (Sdf_en_reInviteFinalResponseSent == 
                     pCallObj->pCallInfo->dState)) 
            {
                dest_module_id = ICF_INT_MODULE_CM;
            }

            /*CSR_1_7113244 Merged SPR 19672 Start */
            /* Referred from mainline */
            else if ((Sdf_en_finalFailureSent == pCallObj->pCallInfo->dState) &&
                          (ICF_FALSE == p_ssa_ctxt->no_retrans_req))
            {
              /*Free call object only when RefCount is greater then 1
               *In general scenario it will be ==1, when failure is sent
               *to remote not because of decode failure, In some CERT scenarios
               *dRefCount can be greater then 1 where we need to FreeCallObject once*/
               if (1 < pCallObj->dRefCount)
               {
                  sdf_ivk_uaFreeCallObject(&pCallObj);
               }
            }
	    /*CSR_1_7113244 Merged SPR 19672 End */

            /*thie code was merged from cert branch caused crashes in LOAD testing 
              for ICF_6_5_PQT_SUPPORT branch*/
            /*Free call onject only if timers are not expired because of free call 
             * object*/
            else if ((Sdf_en_finalFailureSent == pCallObj->pCallInfo->dState) &&
                     (ICF_FALSE == p_ssa_ctxt->no_retrans_req))
            {
                /*Free call object only when RefCount is greater then 1
                 *In general scenario it will be ==1, when failure is sent
                 *to remote not because of decode failure, In some CERT scenarios
                 *dRefCount can be greater then 1 where we need to FreeCallObject once*/
#ifdef SDF_THREAD_SAFE
                if (1 < pCallObj->dRefCount.ref)
#else
                if (1 < pCallObj->dRefCount)
#endif
                {
                    sdf_ivk_uaFreeCallObject(&pCallObj);
                }
            }
        }
    }
    else if((dTimerType == Sdf_en_timerB) || (dTimerType == Sdf_en_timerF))
    {
        /* Request Timeout.  Sdf_en_timerB for Invite Transactions and
         * Sdf_en_timerF for Regular Transactions */

#ifdef ICF_IPSEC_ENABLED
		
		if ((ICF_TRUE == p_ssa_pdb->p_glb_cfg_data->ipsec_enabled) &&
			(dReqRespType == SipMessageRequest) &&
			(dMsgType != Sdf_en_register))
		{
			/* decrement the active transaction count */
			p_ssa_ctxt->ssa_sa_context.p_sa_context->num_active_trans -= 1;
			ICF_PRINT(("[SSA]: num_active_trans decremented, current value = %d", 
				p_ssa_ctxt->ssa_sa_context.p_sa_context->num_active_trans));
			
			/* check if the active transaction count == 0. if yes and the
			* timeout is for a non Register transaction, so send a message
			* to REGM for deleting the SA if the transaction comp. on the 
			* old SA
			*/

			p_ssa_pdb->p_rgm_context = p_ssa_ctxt->ssa_sa_context.p_rgm_context;
			p_ssa_pdb->p_sa_context = p_ssa_ctxt->ssa_sa_context.p_sa_context;

			if (ICF_NULL == 
				p_ssa_ctxt->ssa_sa_context.p_sa_context->num_active_trans)
			{
				ret_val = icf_ssa_rgm_delete_old_sa_ind(p_ssa_pdb);
			}
		}
#endif

        if ((SipMessageRequest == dReqRespType) &&
                (p_ssa_ctxt->p_conn_table_req_entry != ICF_NULL))
        {
            icf_nw_interface_info_st nw_interface_info;
            nw_interface_info.bitmap = ICF_INTF_INFO_PROTOCOL_TCP;
            nw_interface_info.receiver_fd = 
            p_ssa_ctxt->p_conn_table_req_entry->conn_fd;
#ifdef ICF_SECURE_SUPPORT
            if (p_ssa_ctxt->p_conn_table_req_entry->prot_conn_bitmask &
                    ICF_TRANSPORT_TLS)
            {
                nw_interface_info.bitmap = ICF_INTF_INFO_PROTOCOL_TLS;
            }
#endif
            ICF_PRINT(((icf_uint8_t *)"Closing the Request connection\n"))
            /* Fix for spr 18882*/
            ICF_CHECK_IF_VAR_IS_NULL(p_glb_pdb,p_ssa_ctxt->p_ssa_pdb->p_glb_pdb->p_glb_data,ret_val)
            if(ICF_FAILURE == ret_val)
            {
                ICF_FUNCTION_EXIT(p_glb_pdb)
                return Sdf_co_fail;
            }
            icf_es_close_tcp_conn(
                    p_ssa_ctxt->p_ssa_pdb->p_glb_pdb->p_glb_data->p_glb_port_info, 
                    &nw_interface_info, p_ssa_ctxt->p_ssa_pdb->p_ecode);

            icf_ssa_remove_conn_entry(p_ssa_ctxt->p_ssa_pdb,
                    p_ssa_ctxt->p_conn_table_req_entry->conn_fd);

        }
        else if ((SipMessageResponse == dReqRespType) &&
                (p_ssa_ctxt->p_conn_table_resp_entry != ICF_NULL))
        {
            icf_nw_interface_info_st nw_interface_info;
            nw_interface_info.bitmap = ICF_INTF_INFO_PROTOCOL_TCP;
            nw_interface_info.receiver_fd = 
            p_ssa_ctxt->p_conn_table_resp_entry->conn_fd;
#ifdef ICF_SECURE_SUPPORT
            if (p_ssa_ctxt->p_conn_table_resp_entry->prot_conn_bitmask &
                    ICF_TRANSPORT_TLS)
            {
                nw_interface_info.bitmap = ICF_INTF_INFO_PROTOCOL_TLS;
            }
#endif
            ICF_PRINT(((icf_uint8_t *)"Closing the Request connection\n"))
            /* Fix for spr 18882*/
            ICF_CHECK_IF_VAR_IS_NULL(p_glb_pdb,p_ssa_ctxt->p_ssa_pdb->p_glb_pdb->p_glb_data,ret_val)
            if(ICF_FAILURE == ret_val)
            {
                ICF_FUNCTION_EXIT(p_glb_pdb)
                return Sdf_co_fail;
            }
            icf_es_close_tcp_conn(
                    p_ssa_ctxt->p_ssa_pdb->p_glb_pdb->p_glb_data->p_glb_port_info, 
                    &nw_interface_info, p_ssa_ctxt->p_ssa_pdb->p_ecode);
            icf_ssa_remove_conn_entry(p_ssa_ctxt->p_ssa_pdb,
                    p_ssa_ctxt->p_conn_table_resp_entry->conn_fd);
        }
        

        /* Fix for SPR 19335 : Protective code */
        /* If CC, REGM, SIC contexts in SSA context are null, and the timer list in
         * SSA context is also empty, it means that ideally SSA context, CallObj
         * should also have been freed by now but may still have remained due to 
         * some unforeseen error scenario. In that case, free them now to avoid
         * any potential memory issues or crashes.
         */
        if ((ICF_NULL == p_ssa_ctxt->p_call_ctxt) &&
                (ICF_NULL == p_ssa_ctxt->p_rgm_ctxt) &&
                (ICF_NULL == p_ssa_ctxt->p_sic_ctxt) &&
                (ICF_FALSE == icf_ssa_check_if_any_timers_exist(p_ssa_ctxt)))
        {
            icf_error_data_st		err_data;

            /* ------------------------------------------------------ */
            /* Catch the unexpected scenario */
            icf_port_memset(
                    (icf_void_st *)&err_data, 0, ICF_PORT_SIZEOF(icf_error_data_st));
            err_data.num_of_integer_values = 1;
            err_data.number_of_string_values = 0;
            err_data.int_val[0] = p_ssa_ctxt->p_call_obj->dRefCount;
            icf_port_strcpy(
                    err_data.err_string.str,
                    (icf_uint8_t*)("Transaction timeout after CC/REGM/SIC freed"));
            err_data.err_string.str_len = icf_port_strlen(err_data.err_string.str);

            icf_port_check_unexpected_scenario(p_glb_pdb, &err_data);

            /* ------------------------------------------------------ */
            /* Close all open sockets for this call */
            sdf_ivk_uaCloseAllSocketsForCall(p_ssa_ctxt->p_call_obj, pError);

            /* Abort any pending transaction */
            sdf_ivk_uaAbortCall(p_ssa_ctxt->p_call_obj, pContext, pError);


            return Sdf_co_success;
        }

        if((dReqRespType == SipMessageRequest || 
                    (dReqRespType == SipMessageResponse)) &&
                ((dMsgType == Sdf_en_update) || (dMsgType == Sdf_en_invite) ||
                 (dMsgType == Sdf_en_info) || (dMsgType == Sdf_en_prack) ||
                 (dMsgType == Sdf_en_cancel)))
        {
#ifdef ICF_DNS_LOOKUP_ENABLED
            if (ICF_FALSE != p_ssa_ctxt->no_retrans_req)
            {
                /*In case of forceful call clear do not try to send 
                 *message on other IPs or secondary addresses*/
            }
            /* Resend transaction on the current server. If maximum 
             * restransmissions done already then send on secondary server */
            else if(ICF_SUCCESS == icf_ssa_resend_ongoing_txn(pCallObj, p_sip_msg, pOverlapTxn,\
                                    dOptions))
            {
                return ICF_SUCCESS;
            }
            else if (ICF_SUCCESS == icf_ssa_resend_on_sec_address(pCallObj,
                        pOverlapTxn, pContext, pError))
            {
                return ICF_SUCCESS;
            }
            else
            {
                /* Control won't reach here in SUCCESS - do nothing*/
            }
#endif
            dest_module_id = ICF_INT_MODULE_CM;
        }
        else if (dMsgType == Sdf_en_bye)
        {
#ifdef ICF_DNS_LOOKUP_ENABLED
            if (ICF_FALSE != p_ssa_ctxt->no_retrans_req)
            {
                /*In case of forceful call clear do not try to send 
                 *message on other IPs or secondary addresses*/
            }
            /* Resend transaction on the current server. If maximum 
             * restransmissions done already then send on secondary server */
            else if(ICF_SUCCESS == icf_ssa_resend_ongoing_txn(pCallObj, p_sip_msg, pOverlapTxn,\
	    						dOptions))
    	    {
                 return ICF_SUCCESS;
            }
            else if (ICF_SUCCESS == icf_ssa_resend_on_sec_address(pCallObj,
                        pOverlapTxn, pContext, pError))
            {
                return ICF_SUCCESS;
            }
            else
            {
                /* Control won't reach here in SUCCESS - do nothing*/
            }

#endif
           /* Other than Request Timeouts, and final Response timeouts */
            sdf_ivk_uaCloseAllSocketsForCall(pCallObj, pError);
            sdf_ivk_uaAbortCall(pCallObj, pContext, pError);

        }
        else if ( Sdf_en_register == dMsgType)
        {
#ifdef ICF_DNS_LOOKUP_ENABLED
            if (ICF_FALSE != p_ssa_ctxt->no_retrans_req)
            {
                /*In case of forceful call clear do not try to send 
                 *message on other IPs or secondary addresses*/
            }
            /* Resend transaction on the current server. If maximum 
             * restransmissions done already then send on secondary server */
            else if(ICF_SUCCESS == icf_ssa_resend_ongoing_txn(pCallObj, p_sip_msg, pOverlapTxn,\
                                    dOptions))
            {
                return ICF_SUCCESS;
            }
            else if (ICF_SUCCESS == icf_ssa_resend_on_sec_address(pCallObj,
                        pOverlapTxn, pContext, pError))
            {
                return ICF_SUCCESS;
            }
            else
            {
                /* Control won't reach here in SUCCESS - do nothing*/
            }

#endif
            dest_module_id = ICF_INT_MODULE_CFG;
        }
ICF_SIC_START
	else if(ICF_NULL != p_method)
	{
        /* Fix for SPR 20061 : In case the application sends ICF_GENERIC_MSG_REQ
           to send a REFER or a NOTIFY and Timer F fires, we will trigger call
           clear towards CC module.CC would then send failure ICF_GENERIC_MSG_RESP
           to the application. As a fix for this SPR, a check for NOTIFY has been
           added.
        */   
        if((Sdf_en_refer == dMsgType) ||
           (Sdf_en_notify == dMsgType))
        {
#ifdef ICF_DNS_LOOKUP_ENABLED
            if (ICF_FALSE != p_ssa_ctxt->no_retrans_req)
            {
                /*In case of forceful call clear do not try to send 
                 *message on other IPs or secondary addresses*/
            }
            /* Resend transaction on the current server. If maximum 
             * restransmissions done already then send on secondary server */
            else if(ICF_SUCCESS == 
                    icf_ssa_resend_ongoing_txn(pCallObj, p_sip_msg, pOverlapTxn,\
                        dOptions))
            {
                return ICF_SUCCESS;
            }
            else if (ICF_SUCCESS == icf_ssa_resend_on_sec_address(pCallObj,
                        pOverlapTxn, pContext, pError))
            {
                return ICF_SUCCESS;
            }
            else
            {
                /* Control won't reach here in SUCCESS - do nothing*/
            }
#endif
            if (ICF_NULL != p_ssa_ctxt->p_sic_ctxt)
            {
                dest_module_id = ICF_INT_MODULE_SIC;
            }
            else
            {
                dest_module_id = ICF_INT_MODULE_CM;
            }
        }
		if(
			( Sdf_en_subscribe == dMsgType)||
			( Sdf_en_options == dMsgType)||
			/*( Sdf_en_refer == dMsgType)||*/
			!( icf_port_strcmp( (icf_uint8_t *)p_method,(icf_uint8_t *)"MESSAGE" ) ) ||
			!( icf_port_strcmp((icf_uint8_t *) p_method,(icf_uint8_t *)"OPTIONS" ) ) ||
			!( icf_port_strcmp( (icf_uint8_t *)p_method,(icf_uint8_t *) "PUBLISH") )
		)
		{
#ifdef ICF_DNS_LOOKUP_ENABLED
            if (ICF_FALSE != p_ssa_ctxt->no_retrans_req)
            {
                /*In case of forceful call clear do not try to send 
                 *message on other IPs or secondary addresses*/
            }
            /* Resend transaction on the current server. If maximum 
             * restransmissions done already then send on secondary server */
            else if(ICF_SUCCESS == icf_ssa_resend_ongoing_txn(pCallObj, p_sip_msg, pOverlapTxn,\
                                     dOptions))
            {
                return ICF_SUCCESS;
            }
            else if (ICF_SUCCESS == icf_ssa_resend_on_sec_address(pCallObj,
                        pOverlapTxn, pContext, pError))
            {
                return ICF_SUCCESS;
            }
            else
            {
                /* Control won't reach here in SUCCESS - do nothing*/
            }

#endif
            dest_module_id = ICF_INT_MODULE_SIC;
        }
    }
    /* code added */
    else if (Sdf_en_refer == dMsgType)
    {
#ifdef ICF_DNS_LOOKUP_ENABLED
        if (ICF_FALSE != p_ssa_ctxt->no_retrans_req)
        {
            /*In case of forceful call clear do not try to send 
             *message on other IPs or secondary addresses*/
        }
        /* Resend transaction on the current server. If maximum 
         * restransmissions done already then send on secondary server */
        else if(ICF_SUCCESS == icf_ssa_resend_ongoing_txn(pCallObj, p_sip_msg, pOverlapTxn,\
                            dOptions))
        {
             return ICF_SUCCESS;
        }
        else if (ICF_SUCCESS == icf_ssa_resend_on_sec_address(pCallObj,
                        pOverlapTxn, pContext, pError))
        {
            return ICF_SUCCESS;
        }
        else
        {
                /* Control won't reach here in SUCCESS - do nothing*/
        }
#endif
        if (ICF_NULL != p_ssa_ctxt->p_sic_ctxt)
        {
            dest_module_id = ICF_INT_MODULE_SIC;
        }
        else
        {
            dest_module_id = ICF_INT_MODULE_CM;
        }
    }
ICF_SIC_END    
    }
    /*  for SPR 18472 */
    else if (Sdf_en_timerNoAnswer == dTimerType)
    {
        /* SPR 20194: Reverted the fix for 20082.
           SPR 20082 is resolved due to SPR 20111*/
        if (ICF_NULL != p_ssa_ctxt->p_ssa_pdb->p_ssa_ctxt->p_call_ctxt)
        {
            dest_module_id = ICF_INT_MODULE_CM;
        }
        /* SPR 19999: Fix for CSR 1-7493306: SPR 19864 */
	/* On receiving 1xx response, UATK runs a No Answer timer of 180sec.
	 * When this timer expires, we should not abort the call. Instead,
 	 * we should trigger CC to clear the call and handle the clearance
	 * gracefully.
	 */
    }
    /* Fix for SPR 19335 : Protective code */
	/* If CC, REGM, SIC contexts in SSA context are null, and the timer list in
     * SSA context is also empty, it means that ideally SSA context, CallObj
     * should also have been freed by now but may still have remained due to 
     * some unforeseen error scenario. In that case, free them now to avoid
     * any potential memory issues or crashes.
	 */
    else if ((ICF_NULL == p_ssa_ctxt->p_call_ctxt) &&
		(ICF_NULL == p_ssa_ctxt->p_rgm_ctxt) &&
		(ICF_NULL == p_ssa_ctxt->p_sic_ctxt) &&
		(ICF_FALSE == icf_ssa_check_if_any_timers_exist(p_ssa_ctxt)))
	{
		icf_error_data_st		err_data;

		/* ------------------------------------------------------ */
		/* Catch the unexpected scenario */
		icf_port_memset(
			(icf_void_st *)&err_data, 0, ICF_PORT_SIZEOF(icf_error_data_st));
		err_data.num_of_integer_values = 1;
		err_data.number_of_string_values = 0;
		err_data.int_val[0] = p_ssa_ctxt->p_call_obj->dRefCount;
		icf_port_strcpy(
				err_data.err_string.str,
				(icf_uint8_t*)("Transaction timeout after CC/REGM/SIC freed"));
		err_data.err_string.str_len = icf_port_strlen(err_data.err_string.str);

		icf_port_check_unexpected_scenario(p_glb_pdb, &err_data);

		/* ------------------------------------------------------ */
		/* Close all open sockets for this call */
		sdf_ivk_uaCloseAllSocketsForCall(p_ssa_ctxt->p_call_obj, pError);

		/* Abort any pending transaction */
		sdf_ivk_uaAbortCall(p_ssa_ctxt->p_call_obj, pContext, pError);
		return Sdf_co_success;
	}
    else 
    {
        /* Other than Request Timeouts, and final Response timeouts */
        sdf_ivk_uaCloseAllSocketsForCall(pCallObj, pError);
        sdf_ivk_uaAbortCall(pCallObj, pContext, pError);
    }

    /*SPR 7743 Starts*/
    if (( ICF_SSA_APP_CTXT_TERMINATED ==
    (ICF_SSA_APP_CTXT_TERMINATED & p_ssa_ctxt->bitmask))
    || (ICF_SSA_APP_CTX_DELINKED == 
                (ICF_SSA_APP_CTX_DELINKED & p_ssa_ctxt->bitmask)))
    {
        dest_module_id = ICF_INT_MODULE_INVALID;
    }
    /*SPR 7743 Ends*/
    else
    {
             /*  Fix for spr 18882 */
        ICF_CHECK_IF_VAR_IS_NULL(p_glb_pdb,p_internal_msg,ret_val)
        if (ICF_SUCCESS == ret_val)
        {
            if (ICF_INT_MODULE_CM == dest_module_id)
            {
#ifdef ICF_SESSION_TIMER
                if((pCallObj->pSessionTimer != ICF_NULL) && 
                (pCallObj->pSessionTimer->pTimerId != ICF_NULL))
                {
                   if(sdf_fn_uaStopSessionTimer((Sdf_ty_pvoid *)\
                      &(pCallObj->pSessionTimer->pTimerId) ,pError)==Sdf_co_fail)
                   {
                       ret_val = ICF_FAILURE;
                   }
                }
#endif

                /*Fill internal message for other module*/
                p_internal_msg->p_msg_data = ICF_NULL;/*no payload*/
                p_internal_msg->msg_hdr.msg_id = 
                        ICF_GENERIC_CALL_CLEAR;
                p_internal_msg->msg_hdr.payload_length = 0;
                p_internal_msg->msg_hdr.p_glb_pdb = p_glb_pdb;
                ((icf_ssa_pdb_st*)(p_ssa_ctxt->p_ssa_pdb))->dest_module_id =
                    dest_module_id;

                /*Set call clear reason as proxy not responding when 
                 *p_rem_addr_active is Non-NULL as it indicates that
                 *request was sent thru proxy*/
                if ((ICF_NULL != p_glb_pdb->p_call_ctx) &&
                    (ICF_NULL != p_ssa_ctxt->p_rem_addr_active))
                {
                    p_glb_pdb->p_call_ctx->release_cause = 
                        ICF_CALL_CLEAR_REASON_PROXY_SERVER_NOT_RESPONDING;
                }
            }
            else if (ICF_INT_MODULE_CFG == dest_module_id)
            {
                /* Populate the internal message header */
                 p_internal_msg->msg_hdr.msg_id = ICF_NW_INC_REG_RESP; 
                p_internal_msg->msg_hdr.payload_length = 
                    ICF_PORT_SIZEOF(icf_rgm_nw_resp_st);

                /* Populate the internal message payload */
                p_rgm_msg = (icf_rgm_nw_resp_st *)p_internal_msg->p_msg_data;
                /*  Fix for spr 18882 */
                ICF_CHECK_IF_ANY_OF_TWO_VAR_IS_NULL(p_glb_pdb,p_rgm_msg,
                        p_ssa_ctxt->p_ssa_pdb->p_ecode, ret_value)
                if(ICF_FAILURE == ret_value)
                {
                    ret_val = ICF_FAILURE;
		            ICF_FUNCTION_EXIT(p_glb_pdb)    
		            return Sdf_co_fail;
                }
                p_rgm_msg->p_rgm_context = p_ssa_ctxt->p_rgm_ctxt;
                ICF_ASSERT(p_glb_pdb, (ICF_NULL != p_rgm_msg->p_rgm_context))

                    p_rgm_msg->status = ICF_RGM_SSA_NW_FAILURE;

                p_rgm_msg->ecode = *(p_ssa_ctxt->p_ssa_pdb->p_ecode);

                /* SPR 19535: Set the response code as 0 so that it does not result into 
                 * UMR while being accessed at REGM module
                 */
                p_rgm_msg->response_code = 0;

                p_internal_msg->msg_hdr.p_glb_pdb = p_glb_pdb;
                ICF_ASSERT(p_glb_pdb, 
                        (ICF_NULL != p_internal_msg->msg_hdr.p_glb_pdb))

                    ((icf_ssa_pdb_st*)(p_ssa_ctxt->p_ssa_pdb))->dest_module_id = 
                    dest_module_id;
            }
            else if (ICF_INT_MODULE_SIC == dest_module_id)
            {
                
                
                if( Sdf_en_refer == dMsgType)
                {
                    /* Populate the internal message header */
                    p_internal_msg->msg_hdr.msg_id = ICF_SIC_REFER_STATUS;
                    p_internal_msg->msg_hdr.payload_length = 
                        ICF_PORT_SIZEOF(icf_nw_inc_refer_resp_st);

                    /* Populate the internal message payload */
                    p_refer_resp = (icf_nw_inc_refer_resp_st *)p_internal_msg->p_msg_data;
                    /*  Fix for spr 18882 */
                    ICF_CHECK_IF_ANY_OF_TWO_VAR_IS_NULL(p_glb_pdb,p_refer_resp,
                            p_ssa_ctxt->p_ssa_pdb->p_ecode,ret_value)
                    if(ICF_FAILURE == ret_value)
                    {
                       ret_val = ICF_FAILURE;
		               ICF_FUNCTION_EXIT(p_glb_pdb)    
		               return Sdf_co_fail;
                    }

                    p_refer_resp->response = 0;
                    p_refer_resp->p_sic_ctx = p_ssa_ctxt->p_sic_ctxt;
                    /*KLOCWORKS */
                    if (ICF_NULL != p_refer_resp->p_sic_ctx)
                    {
                        p_refer_resp->p_sic_ctx->error_code = *(p_ssa_ctxt->p_ssa_pdb->p_ecode);
                        ICF_ASSERT(p_glb_pdb, (ICF_NULL != p_refer_resp->p_sic_ctx))
                    }
                
                }
                else if( Sdf_en_subscribe == dMsgType)
                {
                    if (ICF_NULL == p_ssa_ctxt->p_sic_ctxt)
                    {
                        /* In this case application has delinked itself
                         * and hence it should ignore this response 
                         * and return SUCCESS */
                        ICF_PRINT(((icf_uint8_t *)"\n[SSA]: Application delinked: Txn Timeout for SUBSCRIBE ignored "));
                        dest_module_id = ICF_INT_MODULE_INVALID;
                    }
                    else
                    {
                        /* Populate the internal message header */
                        p_internal_msg->msg_hdr.msg_id = ICF_NW_INC_SUBSCRIBE_RESP;
                        p_internal_msg->msg_hdr.payload_length = 
                            ICF_PORT_SIZEOF(icf_nw_inc_subs_resp_st);

                        /* Populate the internal message payload */
                        p_subs_resp = (icf_nw_inc_subs_resp_st *)p_internal_msg->p_msg_data;
                        /*  Fix for spr 18882 */
                        ICF_CHECK_IF_ANY_OF_TWO_VAR_IS_NULL(p_glb_pdb,p_subs_resp,
                            p_ssa_ctxt->p_ssa_pdb->p_ecode ,ret_value) 
                        if(ICF_FAILURE == ret_value)
                        {
                            ret_val = ICF_FAILURE;
		                    ICF_FUNCTION_EXIT(p_glb_pdb)    
		                    return Sdf_co_fail;
                        }

                        p_subs_resp->bitmask = 0;
                        p_subs_resp->response = ICF_NW_INC_SUBS_RESP_NO_RETRY;
                        p_subs_resp->resp_code=0;
                        p_subs_resp->p_sic_ctx = p_ssa_ctxt->p_sic_ctxt;
                        p_subs_resp->p_sic_ctx->error_code = *(p_ssa_ctxt->p_ssa_pdb->p_ecode);
                        ICF_ASSERT(p_glb_pdb, (ICF_NULL != p_subs_resp->p_sic_ctx))
                    }
                }
                else if( Sdf_en_options == dMsgType)
                {
                                /* Populate the internal message header */
                                p_internal_msg->msg_hdr.msg_id = ICF_SIC_OPTION_STATUS;
                                p_internal_msg->msg_hdr.payload_length = 
                                                ICF_PORT_SIZEOF(icf_nw_inc_opt_resp_st);

                                /* Populate the internal message payload */
                                p_opt_resp = (icf_nw_inc_opt_resp_st *)p_internal_msg->p_msg_data;
                                /*  Fix for spr 18882 */
                                /*Klocwork Warning Removal*/
                                ICF_CHECK_IF_ANY_OF_THREE_VAR_IS_NULL(p_glb_pdb,p_opt_resp,
                                    p_ssa_ctxt->p_sic_ctxt,    
                                    p_ssa_ctxt->p_ssa_pdb->p_ecode,ret_value)
                                if(ICF_FAILURE == ret_value)
                                {
                                    ret_val = ICF_FAILURE;
		                            ICF_FUNCTION_EXIT(p_glb_pdb)    
		                            return Sdf_co_fail;
                                }

                                p_opt_resp->response = 0;
                                p_opt_resp->p_sic_ctx = p_ssa_ctxt->p_sic_ctxt;
                                p_opt_resp->p_sic_ctx->error_code = *(p_ssa_ctxt->p_ssa_pdb->p_ecode);
                }
                else if(p_method)
                {
                     if(icf_port_strcmp( (icf_uint8_t *)p_method,(icf_uint8_t *)"MESSAGE" ) == 0 )
                      {
                          /* Populate the internal message header */
                           p_internal_msg->msg_hdr.msg_id = ICF_SIC_MESSAGE_STATUS;
                           p_internal_msg->msg_hdr.payload_length = 
                                   ICF_PORT_SIZEOF(icf_nw_inc_msg_resp_st);

                          /* Populate the internal message payload */
                           p_msg_resp = (icf_nw_inc_msg_resp_st *)p_internal_msg->p_msg_data;
                           /*  Fix for spr 18882 */
                           ICF_CHECK_IF_ANY_OF_TWO_VAR_IS_NULL(p_glb_pdb,p_msg_resp,
                                    p_ssa_ctxt->p_ssa_pdb->p_ecode,ret_value)
                           if(ICF_FAILURE == ret_value)
                           {
                                 ret_val = ICF_FAILURE;
		                         ICF_FUNCTION_EXIT(p_glb_pdb)    
		                         return Sdf_co_fail;
                           }

                           p_msg_resp->response = 0;
                           p_msg_resp->p_sic_ctx = p_ssa_ctxt->p_sic_ctxt;
                           if (ICF_NULL != p_msg_resp->p_sic_ctx)
                            {
                                  p_msg_resp->p_sic_ctx->error_code =
                                        *(p_ssa_ctxt->p_ssa_pdb->p_ecode);
                                  ICF_ASSERT(p_glb_pdb,
                                        (ICF_NULL != p_msg_resp->p_sic_ctx))
                            }
                      }
                      else if( icf_port_strcmp((icf_uint8_t *) p_method, (icf_uint8_t *)"PUBLISH") == 0)
                      {
                           /* Populate the internal message header */
                           p_internal_msg->msg_hdr.msg_id = ICF_SIC_PUBLISH_STATUS;
                           p_internal_msg->msg_hdr.payload_length = 
                                        ICF_PORT_SIZEOF(icf_nw_inc_publish_resp_st);

                           /* Populate the internal message payload */
                           p_publish_resp = (icf_nw_inc_publish_resp_st *)p_internal_msg->p_msg_data;
                           /*  Fix for spr 18882 */
                           ICF_CHECK_IF_ANY_OF_TWO_VAR_IS_NULL(p_glb_pdb,p_publish_resp,
                                    p_ssa_ctxt->p_ssa_pdb->p_ecode,ret_value)
                           if(ICF_FAILURE == ret_value)
                           {
                                 ret_val = ICF_FAILURE;
		                         ICF_FUNCTION_EXIT(p_glb_pdb)    
		                         return Sdf_co_fail;
                           }

                           p_publish_resp->response = 0;
                           p_publish_resp->duration = 0;
                           p_publish_resp->p_sic_ctx = p_ssa_ctxt->p_sic_ctxt;
                           if (ICF_NULL != p_publish_resp->p_sic_ctx)
                           {
                                  p_publish_resp->p_sic_ctx->error_code =
                                            *(p_ssa_ctxt->p_ssa_pdb->p_ecode);
                                  ICF_ASSERT(p_glb_pdb,
                                       (ICF_NULL != p_publish_resp->p_sic_ctx))
                            }
                      } 
                }
                p_internal_msg->msg_hdr.p_glb_pdb = p_glb_pdb;
                ICF_ASSERT(p_glb_pdb, 
                             (ICF_NULL != p_internal_msg->msg_hdr.p_glb_pdb))

                ((icf_ssa_pdb_st*)(p_ssa_ctxt->p_ssa_pdb))->dest_module_id = 
                               dest_module_id;
            }
        }
    }

    if((ICF_NULL == p_ssa_ctxt->p_call_ctxt)||
       (p_ssa_ctxt->p_call_ctxt->p_sip_ctx == p_ssa_ctxt))
    {
        /*The following check is included to prevent the report trace
         * indication for ICF_ERROR_RETRANSMISSION_TIMEOUT in case when we
         * have done the forcefull call clear .For example in case of forcefull
         * call clear we simply send the BYE on network and do not wait for 200 
         * response from network so in that case after timeout report trace
         * indication for ICF_ERROR_RETRANSMISSION_TIMEOUT should not be
         * given. */
        if (ICF_FALSE == p_ssa_ctxt->no_retrans_req)
        {    
            ICF_SSA_SET_ECODE(p_ssa_ctxt->p_ssa_pdb,p_method,ICF_ERROR_RETRANSMISSION_TIMEOUT)
        }        
    }
    else if(ICF_INT_MODULE_CM == dest_module_id)
    {   /*This leg is for the case when forceful call has been called for clearing the 
              forked callobjs and timer expiry is being simulated. In such scenario we should
              ignore the timer expiry*/
        dest_module_id = ICF_INT_MODULE_INVALID;
    }
		pError->errCode = Sdf_en_noUaError;

		ICF_FUNCTION_EXIT(p_glb_pdb)    
		return Sdf_co_success;

}

#endif /* SDF_TXN */

#ifdef SDF_TEL
/*****************************************************************************
 ** FUNCTION:           sdf_fn_uaGetIpFromTelUrl
 **
 ** DESCRIPTION:        This function resolves a TelUrl into a valid IP and
 **                     port. It fills up the pDestInfo structure passed to it.
 **
 *****************************************************************************/

Sdf_ty_retVal sdf_cbk_uaGetIpFromTelUrl(
        Sdf_st_callObject *pCallobj,
        TelUrl *pTelUrl,
        Sdf_st_transportInfo *pDestInfo,
        Sdf_st_error *pErr)
{
    icf_return_t          ret_val = ICF_SUCCESS;
    icf_ssa_ctxt_st       *p_ssa_ctxt = 
        (icf_ssa_ctxt_st*)((Sdf_st_appData*)pCallobj->pAppData->pData);
    icf_ssa_pdb_st        *p_ssa_pdb = (icf_ssa_pdb_st *)
        (p_ssa_ctxt->p_ssa_pdb);
    
    icf_line_data_st      *p_line_data = p_ssa_pdb->p_line_data ;

    /* To remove compiler warning */
    (void)pTelUrl;
    (void)pErr;

    /*
     * Allocate memory for pDestInfo->pIp
     * This memory will be freed by UATK
     */
    ICF_MEMGET(p_ssa_pdb->p_glb_pdb,
            ICF_SSA_IP_MAX_LEN,
            ICF_MEM_COMMON, pDestInfo->pIp,
            ICF_RET_ON_FAILURE,p_ssa_pdb->p_ecode, ret_val)

    /* Extract the proxy address and return as pDestInfo */

    if ( (ICF_TRANSPORT_TYPE_TCP == p_line_data->transport_mode) ||
            (ICF_TRANSPORT_MODE_BOTH == p_line_data->transport_mode))
    {
        pDestInfo->dScheme = Sdf_en_protoTcp ;
    }
#ifdef ICF_SECURE_SUPPORT    
    else if (ICF_TRANSPORT_TYPE_TLS == p_line_data->transport_mode)
    {
        pDestInfo->dScheme = Sdf_en_protoTls ;
    }
#endif    
    else
    {
        pDestInfo->dScheme = Sdf_en_protoUdp ;
    }

    if(ICF_TRANSPORT_ADDRESS_DOMAIN_NAME == 
            p_line_data->proxy_addr.addr.addr_type)
    {
        icf_port_strcpy((icf_uint8_t *)pDestInfo->pIp,
                (icf_uint8_t *)p_line_data->proxy_addr.addr.addr.domain.str);
    } /* p_line_data->proxy_addr.addr.a... */
    else
    {
        icf_ssa_convert_to_IP_addr(&(p_line_data->proxy_addr),
               (icf_uint8_t *) pDestInfo->pIp);
    }
    pDestInfo->dPort = p_line_data->proxy_addr.port_num;
/*    ICF_SSA_TRACE(p_ssa_pdb,ICF_TRACE_TEL_URL_TO_IP_ADDR)*/


    return Sdf_co_success;

}
#endif
#ifdef SDF_SESSION_TIMER
/*****************************************************************************
 ** FUNCTION:	 sdf_cbk_uaHandleSTTimeout
 **
 ** DESCRIPTION: This callback is issued to the application on expiration
 **				 of session timer. In this callback application can perform
 **				 his own sets of operation to refresh/terminate the session 
 **				 and gives an indication to the UATK through pRefresh enum. 
 **				 If return value of pRefresh enum is Sdf_en_applicaton then
 **				 UATK will not perform refresh on behalf of application. 
 **				 If the value is Sdf_en_uatoolkit then UATK will 
 **				 UATK will perform refresh/terminate on behalf of
 **				 application 
 **
 ** PARAMETERS:
 **		pCallObj(IN)	: Call Object that has Session Timer times out
 **		pRefresh(OUT)	: The application is expected to set this enum 
 **						  for performing the refresh/terminated by the UATK 
 **		pErr(OUT)		: Error structure in case the function fails
 **
 ************************************************************************/


Sdf_ty_retVal sdf_cbk_uaHandleSTTimeout(
	Sdf_st_callObject *pCallObj,Sdf_ty_handleEvent *pRefresh,
	Sdf_ty_sessionRefreshRequest *pRefreshReq, Sdf_st_error  *pErr )
{
        icf_return_t  ret_val = ICF_SUCCESS;
     
#ifdef ICF_SESSION_TIMER	
	Sdf_ty_refresher dRefresher = Sdf_en_refresherNone;
	Sdf_st_appData *pAppData = ICF_NULL;
	icf_ssa_ctxt_st *p_ssa_ctxt = ICF_NULL;
	icf_ssa_pdb_st *p_ssa_pdb = ICF_NULL;
	icf_glb_pdb_st       *p_glb_pdb = ICF_NULL;
        icf_nw_otg_session_refresh_req_st  *p_session_refresh_req = ICF_NULL;
	icf_internal_msg_st  *p_internal_msg = ICF_NULL;
	if(sdf_ivk_uaGetAppDataFromCallObject(pCallObj, &pAppData, pErr) == Sdf_co_fail)
    {
		return Sdf_co_fail;
	}
    
    if (ICF_NULL == pAppData)
        {
           ICF_PRINT(((icf_uint8_t *)"\n[SSA]: Session Timer: Unable to fetch App Data.\n"))
           /*  CSR 1-6412398 Merge */
           return Sdf_co_fail;
        }

	p_ssa_ctxt = (icf_ssa_ctxt_st *)pAppData->pData;
        if (ICF_NULL == p_ssa_ctxt)
        {
           ICF_PRINT(((icf_uint8_t *)"\n[SSA]: Session Timer: Unable to fetch SSA Ctxt.\n"))
           /*  CSR 1-6412398 Merge */
           return Sdf_co_fail;
        }

	p_ssa_pdb = (icf_ssa_pdb_st*)(p_ssa_ctxt->p_ssa_pdb);
        if (ICF_NULL == p_ssa_pdb)
        {
           ICF_PRINT(((icf_uint8_t *)"\n[SSA]: Session Timer: Unable to fetch SSA Pdb.\n"))
           /*  CSR 1-6412398 Merge */
           return Sdf_co_fail;
        }

	    p_glb_pdb = p_ssa_pdb->p_glb_pdb;
        if (ICF_NULL == p_glb_pdb)
        {
           ICF_PRINT(((icf_uint8_t *)"\n[SSA]: Session Timer: p_glb_pdb is NULL.\n"))
           /*  CSR 1-6412398 Merge */
           return Sdf_co_fail;
        }
        /* CSR_1_1_7027627 Merged SPR 19672 Start */
        if (ICF_NULL == p_ssa_ctxt->p_call_ctxt)
        {
           ICF_PRINT(((icf_uint8_t *)"\n[SSA]: Session Timer: p_ssa_ctxt->p_call_ctxt is NULL.\n"))
           return Sdf_co_fail;
        } 
        /* CSR_1_1_7027627 Merged SPR 19672 End */

        /* Moved FUNCTION_ENTER macro below to avoid klocworks warning 
           Also it should be call only if p_glb_pdb is non-null */
        ICF_FUNCTION_ENTER(p_glb_pdb)   

        /* Merge for CSR 1-6412398: In case the RE-INVITE from N/W is being processed, 
         * drop this session expiry and return failure to stack.
         * The check should be done for reinvite / reinvite wo SDP / Update received from
         * network. In all these cases the session timer expiry should be ignored.
         */
 
        if(((ICF_NULL != pCallObj->pCallInfo) && (Sdf_en_reInviteprovRespSent ==
                 pCallObj->pCallInfo->dState)) ||
            (ICF_NULL != (p_ssa_ctxt->bitmask & ICF_SSA_CTXT_REINVITE_RECVD)) ||
            (ICF_NULL != (p_ssa_ctxt->bitmask & ICF_SSA_CTXT_REINVITE_RECVD_WO_SDP)) ||
            (ICF_NULL != (p_ssa_ctxt->bitmask & ICF_SSA_CTXT_UPDATE_RECVD)))
        {
            ICF_PRINT(((icf_uint8_t *)"\n[SSA]: STTimeout when ReINVITE from N/W is being handled\n"))
            sdf_ivk_uaFreeAppData(pAppData);
            return Sdf_co_fail;
        }
        
        /* SPR 18937: If Call context is not present in SSA context, means call has been
         * cleared but the SSA context and call object is still active. Therfore no need
         * to send session reinvite as the session has already been terminated.
         */     
        if((ICF_NULL != p_ssa_ctxt->p_call_ctxt) &&
                (ICF_TRUE == p_ssa_ctxt->p_call_ctxt->session_timer_enable))
        {
	  p_internal_msg = ((icf_ssa_pdb_st*)(p_ssa_ctxt->p_ssa_pdb))->p_internal_msg;
   	  p_session_refresh_req = (icf_nw_otg_session_refresh_req_st*)
		(p_internal_msg->p_msg_data);
    

	if(pCallObj->pMediaInfo != ICF_NULL){
		pCallObj->pMediaInfo->dMediaState = Sdf_en_idleMedia;
	}
	/* get the ICF session refresher role */

	if(sdf_ivk_uaGetRefresher(pCallObj, &dRefresher,pErr) == Sdf_co_fail){
		sdf_ivk_uaFreeAppData(pAppData);
		return Sdf_co_fail;
	}
	*pRefresh = Sdf_en_applicaton;
	/*if remote UA is the refresher then send a message to CC to terminate the call*/
	if(dRefresher == Sdf_en_refresherRemote){
		ICF_PRINT(((icf_uint8_t *)"\n [SSA] Sending Bye when Session Timer Expired"));
		
		/* this reason code indiates to CC that session timer had expired when */

		/* session refresher is remote UA.*/

		p_internal_msg->p_msg_data = ICF_NULL;/*no payload*/
			p_internal_msg->msg_hdr.msg_id = 
				    ICF_GENERIC_CALL_CLEAR;
			p_internal_msg->msg_hdr.payload_length = 0;
			p_internal_msg->msg_hdr.p_glb_pdb = p_glb_pdb;    
		p_ssa_pdb->dest_module_id = ICF_INT_MODULE_CM;
	}
	else{
		/* Send an event ICF_NW_OTG_REFRESH_SEND_OR_ACK to CC to set
		 *  signalling ongoing to CC and send Session refresh request
		 *  using UATK */

		if(p_ssa_ctxt->refresher_method_type == Sdf_en_refresherInvite){
			ICF_PRINT(((icf_uint8_t *)"\n [SSA] Sending Re-Invite when Session Timer Expired"));
			p_session_refresh_req->refresher_req_type = ICF_SSA_REFRESHER_METHOD_INVITE;
		}
		else if(p_ssa_ctxt->refresher_method_type == Sdf_en_refresherUpdate){
			ICF_PRINT(((icf_uint8_t *)"\n [SSA] Sending Update when Session Timer Expired"));
			p_session_refresh_req->refresher_req_type = ICF_SSA_REFRESHER_METHOD_UPDATE;
		}
		else
			ret_val = ICF_FAILURE;
		if ( ICF_SUCCESS == ret_val)
		{
			p_ssa_pdb->dest_module_id = ICF_INT_MODULE_INVALID;
			ICF_SSA_SET_COMMON_BITMASK((p_ssa_pdb->common_bitmask), 
				ICF_SSA_PENDING_SESSION_REFRESH)
			p_ssa_ctxt->session_timer_expired_refresh = ICF_TRUE;
		}
/*		p_ssa_pdb->p_internal_msg->p_msg_data = (icf_uint8_t *)&session_refresh_req;
		if(icf_ssa_process_mesg(p_ssa_pdb->p_internal_msg) == Sdf_co_fail){
			sdf_ivk_uaFreeAppData(pAppData);
			return Sdf_co_fail;
		}*/
	}
	sdf_ivk_uaFreeAppData(pAppData);
	pRefreshReq = pRefreshReq;
       }
       else
       {
	  pCallObj = pCallObj;
	  pRefresh = pRefresh;
	  pErr = pErr;
   	  pRefreshReq = pRefreshReq;
      sdf_ivk_uaFreeAppData(pAppData);
       }
    ICF_FUNCTION_EXIT(p_glb_pdb)
#else
	pCallObj = pCallObj;
	pRefresh = pRefresh;
	pErr = pErr;
   	pRefreshReq = pRefreshReq;
#endif


	if ( ICF_SUCCESS == ret_val)
	{
		return Sdf_co_success;
	}
	else
	{
		return Sdf_co_fail;
	}
}
#endif


#ifdef SDF_TLS

/***********************************************************************
** FUNCTION:  sdf_cbk_uaTLSConnectionEstdNtfn
**
** DESCRIPTION: This is a notification callback thrown on the application 
**              to notify that a new TLS connection has been established. 
**
** PARAMETERS:
** 		dSockfd(IN)	: The Socket Descriptor. 
** 		pSockInfo(IN)	: The socket related information corresponding to
**                        dSockfd.
**	 	pErr(OUT)		: Error structure in case the function fails
**
************************************************************************/
Sdf_ty_retVal sdf_cbk_uaTLSConnectionEstdNtfn(
    Sdf_ty_socketId	dSockfd,
    Sdf_st_sockInfo	*pSockInfo,
    Sdf_st_error *pErr)
{
	(void)pErr;
    (void)pSockInfo;
    dSockfd = dSockfd;

	return Sdf_co_success;
}


/***********************************************************************
** FUNCTION:  sdf_cbk_uaTLSConnectionClosedNtfn
**
** DESCRIPTION: This is a notification callback thrown on the application 
**              to notify that an existing TLS connection has been closed. 
**
** PARAMETERS:
** 		dSockfd(IN)	: The Socket Descriptor that was closed. 
**	 	pErr(OUT)	: Error structure in case the function fails
**
************************************************************************/
Sdf_ty_retVal sdf_cbk_uaTLSConnectionClosedNtfn
(Sdf_ty_socketId	dSockfd,
 Sdf_st_error *pErr)
{
	(void)pErr;
    dSockfd = dSockfd;
	return Sdf_co_success;
}

#endif /* SDF_TLS */
#ifdef SDF_LOOKUP

/***********************************************************************
** FUNCTION: sdf_cbk_uaSendCallToPeer 
**
** DESCRIPTION:  This callback is thrown when the UATK forms the message
**                       and application needs to send the message in the network
************************************************************************/

Sdf_ty_retVal sdf_cbk_uaSendCallToPeer  (Sdf_st_callObject  \
                *pObject, Sdf_st_overlapTransInfo *pOverlapTransInfo,
                SipMessage *pSipMsg, SipOptions dOptions, \
                Sdf_ty_s8bit transptype, Sdf_st_eventContext *pContext, \
                Sdf_st_error *pErr)
{
        Sdf_ty_s8bit *pMethod = Sdf_co_null;
        Sdf_ty_retVal   sdf_ret_val = Sdf_co_success;

        if (Sdf_co_fail == sdf_ivk_uaHandleLookup(pObject, pOverlapTransInfo, pSipMsg,
                                pContext,pErr))
        {
                if ( Sdf_en_dnsInvState != pErr->errCode)
                {
                        ICF_PRINT(((icf_uint8_t *)"\n[SSA]:DNS lookup API failed \n"));
                        sdf_ret_val = Sdf_co_fail;
                }
        }
        else if (Sdf_co_fail == sdf_ivk_uaGetMethodFromSipMessage(pSipMsg,&pMethod,pErr))
        {
                ICF_PRINT(((icf_uint8_t *)"\n[SSA]:failure In SendcalLToPeer CBK\n"));
                sdf_ret_val = Sdf_co_fail;
        }
        else if (Sdf_co_fail == sdf_ivk_uaEndTransaction(pObject, pOverlapTransInfo, \
                                pMethod, pErr))
        {
                ICF_PRINT(((icf_uint8_t *)"\n[SSA]:failure In Ending txn SendCalLToPeer CBK\n"));
                sdf_ret_val = Sdf_co_fail;
        }

#ifdef ICF_DNS_LOOKUP_ENABLED
        dOptions = dOptions;
        transptype = transptype;
#endif

        return sdf_ret_val;
}

/****************************************************************************
 ** FUNCTION: sdf_cbk_uaGetIpFromNonSipUrl
 **
 ** DESCRIPTION: This Callback resolves a Non Sip URl into a valid IP and port.
 **				 It fills up the pDestInfo structure passed to it.
 **
 ****************************************************************************/
Sdf_ty_retVal sdf_cbk_uaGetIpFromNonSipUrl
(Sdf_st_dnsEventContext   *pDnsEventContext,
 Sdf_ty_void              *pUrl,
 Sdf_ty_UrlType           dUrlType,	
 Sdf_st_error *pErr)
{
	Sdf_ty_DnsRecordType	     dDnsRecordType = Sdf_en_unKnownAddr;
	Sdf_ty_bool		             dUpdateState =	Sdf_co_false;
    Sdf_st_transportInfo         *pDestInfo = Sdf_co_null;

    icf_return_t          ret_val = ICF_SUCCESS,
                          dummy_ret_val = ICF_SUCCESS;
    icf_ssa_ctxt_st       *p_ssa_ctxt = 
        (icf_ssa_ctxt_st*)((Sdf_st_appData*)pDnsEventContext->pCallObj->pAppData->pData);
    icf_ssa_pdb_st        *p_ssa_pdb = (icf_ssa_pdb_st *)
        (p_ssa_ctxt->p_ssa_pdb);
    
    icf_line_data_st      *p_line_data = p_ssa_pdb->p_line_data ;

    ICF_MEMGET(p_ssa_pdb->p_glb_pdb,
        sizeof(Sdf_st_transportInfo),
        ICF_MEM_COMMON, pDestInfo,
        ICF_RET_ON_FAILURE,p_ssa_pdb->p_ecode, ret_val)

    ICF_MEMGET(p_ssa_pdb->p_glb_pdb,
        ICF_MAX_STR_LEN,
        ICF_MEM_COMMON, pDestInfo->pIp,
        ICF_DONOT_RET_ON_FAILURE,p_ssa_pdb->p_ecode, ret_val)

    if (ICF_FAILURE == ret_val || ICF_NULL == pDestInfo->pIp)
    {
       ICF_MEMFREE(
                p_ssa_pdb->p_glb_pdb,
                pDestInfo,
                ICF_MEM_COMMON,
                p_ssa_pdb->p_ecode,
                ret_val)
        return ICF_FAILURE;        
    }

	pDestInfo->dPort = p_line_data->proxy_addr.port_num;
      
   if (pDnsEventContext->pOverlappedTransaction == Sdf_co_null)
        pDnsEventContext->pTransaction->pDestInfo->dPort =pDestInfo->dPort; 
   else
        pDnsEventContext->pOverlappedTransaction->pDestInfo->dPort =pDestInfo->dPort;	
 
	
    /* Extract the proxy address and return as pDestInfo */
    if ( (ICF_TRANSPORT_TYPE_TCP == p_line_data->transport_mode) ||
            (ICF_TRANSPORT_MODE_BOTH == p_line_data->transport_mode))
    {
        pDestInfo->dScheme = Sdf_en_protoTcp ;
    }
#ifdef ICF_SECURE_SUPPORT    
    else if (ICF_TRANSPORT_TYPE_TLS == p_line_data->transport_mode)
    {
        pDestInfo->dScheme = Sdf_en_protoTls ;
    }
#endif    
    else
    {
        pDestInfo->dScheme = Sdf_en_protoUdp ;
    }

    if(ICF_TRANSPORT_ADDRESS_DOMAIN_NAME == 
            p_line_data->proxy_addr.addr.addr_type)
    {
        icf_port_strcpy((icf_uint8_t *)pDestInfo->pIp,
                (icf_uint8_t *)p_line_data->proxy_addr.addr.addr.domain.str);

        if ((pDestInfo->dPort) == 0)
            dDnsRecordType = Sdf_en_SRVrecord;
        else
            dDnsRecordType = Sdf_en_IPAddress;	

        pDnsEventContext->dDnsRecordType  = dDnsRecordType;
      
	    if (pDnsEventContext->pOverlappedTransaction == Sdf_co_null)
	        pDnsEventContext->pTransaction->dDnsRecordType  = \
					dDnsRecordType;
	    else
	        pDnsEventContext->pOverlappedTransaction->dDnsRecordType = \
					dDnsRecordType;	
	
	  /* Query the portlayer code */
      if(sdf_fn_uaDnsQuery(pDestInfo->pIp, pDnsEventContext,pErr) == Sdf_co_fail)
	  {
#ifdef SDF_TEL      
		if (dUrlType == Sdf_en_TelUrl)
		    sip_freeTelUrl((TelUrl*)pUrl);
#endif
#ifdef SDF_IM
        if (dUrlType == Sdf_en_ImUrl)
		    sip_freeImUrl((ImUrl*)pUrl);
#endif
#ifdef SDF_PRES
        if (dUrlType == Sdf_en_PresUrl)
            sip_freePresUrl((PresUrl*)pUrl);
#endif
        (Sdf_ty_void)sdf_ivk_uaFreeDnsEventContext(pDnsEventContext);
	    pErr->errCode=Sdf_en_destinationInfoError;
	    ret_val =  Sdf_co_fail;
	  }
      ICF_MEMFREE(
                p_ssa_pdb->p_glb_pdb,
                pDestInfo->pIp,
                ICF_MEM_COMMON,
                p_ssa_pdb->p_ecode,
                dummy_ret_val)
    }
    else
    {
        icf_ssa_convert_to_IP_addr(&(p_line_data->proxy_addr),
                (icf_uint8_t *)pDestInfo->pIp);

	    dUpdateState = Sdf_co_true;	

        if (pDnsEventContext->pOverlappedTransaction == Sdf_co_null)
        {
            if(pDnsEventContext->pTransaction->pDestInfo->pIp)
            {
                ICF_MEMFREE(
                    p_ssa_pdb->p_glb_pdb,
                    pDnsEventContext->pTransaction->pDestInfo->pIp,
                    ICF_MEM_COMMON,
                    p_ssa_pdb->p_ecode,
                    dummy_ret_val)
            }
            pDnsEventContext->pTransaction->pDestInfo->pIp = pDestInfo->pIp;
              
            if(pDestInfo->dPort == 0)
            {
#ifdef SDF_TLS
                if(ICF_TRANSPORT_TYPE_TLS == p_line_data->transport_mode)
                {
                    pDnsEventContext->pTransaction->pDestInfo->dPort = 
                        ICF_DEFAULT_SIPS_SERVER_PORT;
                }
                else
#endif
                {
                    pDnsEventContext->pTransaction->pDestInfo->dPort = 
                        ICF_DEFAULT_SIP_SERVER_PORT;
                }
            }
        }
        else
        {
            if(pDnsEventContext->pOverlappedTransaction->pDestInfo->pIp)
            {
                ICF_MEMFREE(
                    p_ssa_pdb->p_glb_pdb,
                    pDnsEventContext->pOverlappedTransaction->pDestInfo->pIp,
                    ICF_MEM_COMMON,
                    p_ssa_pdb->p_ecode,
                    dummy_ret_val)
            }

            pDnsEventContext->pOverlappedTransaction->pDestInfo->pIp = pDestInfo->pIp;  

            if(pDestInfo->dPort == 0)
            {      
#ifdef SDF_TLS
            
                if(ICF_TRANSPORT_TYPE_TLS == p_line_data->transport_mode)
                {
                    pDnsEventContext->pOverlappedTransaction->pDestInfo->dPort =
                        ICF_DEFAULT_SIPS_SERVER_PORT;
                }
                else
#endif
                {
		            pDnsEventContext->pOverlappedTransaction->pDestInfo->dPort =
                        ICF_DEFAULT_SIP_SERVER_PORT;
                }
            }
        }
	
          
       if (dUpdateState == Sdf_co_true)
       {
	       if(pDnsEventContext->pOverlappedTransaction==Sdf_co_null)
	       {
		       if(pDnsEventContext->pTransaction != Sdf_co_null)
               {

                   switch (pDnsEventContext->pTransaction->dTxnDnsState)  
                   {
		           case Sdf_en_dnsQuery:
                       pDnsEventContext->pTransaction->dTxnDnsState = 
                           Sdf_en_dnsFirstRecordFilled;	
                   break;

                   case Sdf_en_dnsIpUsed:
                       pDnsEventContext->pTransaction->dTxnDnsState = 
                           Sdf_en_dnsFilled;
                   break;

                   default: ; 
                       /* To avoid warning, default is added */
                   }
               }
               else
               {
                   ret_val = Sdf_co_fail;
               }
		   }
		   else
		   {
			   switch (pDnsEventContext->pOverlappedTransaction->dTxnDnsState)  
               {
                   case Sdf_en_dnsQuery:
                       pDnsEventContext->pOverlappedTransaction->dTxnDnsState = 
                           Sdf_en_dnsFirstRecordFilled;
                   break;
                   
                   case Sdf_en_dnsIpUsed:
                       pDnsEventContext->pOverlappedTransaction->dTxnDnsState = Sdf_en_dnsFilled;
                   break;
                   
                   default: break ;
                       /* To avoid warning, default is added */
               }
		   }
	   }		
    }

	ICF_MEMFREE(
                p_ssa_pdb->p_glb_pdb,
                pDestInfo,
                ICF_MEM_COMMON,
                p_ssa_pdb->p_ecode,
                dummy_ret_val)

    return ret_val;
}

#endif

/***********************************************************************
** FUNCTION: sip_indicateMessageRetransmission
**
** DESCRIPTION:  This callback is thrown when the Stack retramsmits the message
**               on the network.
************************************************************************/

icf_void_t sip_indicateMessageRetransmission(SipEventContext *pEventContext, 
                                       SipTxnKey *pTxnKey,
                                       SIP_S8bit *pBuffer,
                                       SIP_U32bit dBufferLength, 
                                       SipTranspAddr *pTranspAddr, 
                                       SIP_U8bit dRetransCount, 
                                       SIP_U32bit dDuration)
{
  Sdf_ty_s8bit              *p_method = Sdf_co_null;
  Sdf_st_error              Error;
  SipMessage                *p_sip_msg = Sdf_co_null;
  icf_ssa_ctxt_st           *p_ssa_ctxt = ICF_NULL;
  icf_glb_pdb_st            *p_glb_pdb = ICF_NULL;
  icf_ssa_pdb_st            ssa_pdb;
  Sdf_ty_messageType        dMsgType = Sdf_en_unknownRequest;
  Sdf_st_txnContextData	  *pContextData	= \
                                 (Sdf_st_txnContextData *)pEventContext->pData;
  Sdf_st_callObject        *pCallObj = pContextData->pRefCallObject;
  en_SipMessageType         dReqRespType = SipMessageAny;
  Sdf_st_overlapTransInfo   *pTransInfo = Sdf_co_null;
  Sdf_st_transaction        *pTransaction = Sdf_co_null;
  Sdf_st_overlapTransInfo   *pOverlapTxn = Sdf_co_null;
  Sdf_ty_u16bit             resp_code;
  icf_error_t          ecode = ICF_ERROR_NONE;

  pTxnKey = pTxnKey;
  pBuffer = pBuffer;
  dBufferLength = dBufferLength;
  pTranspAddr = pTranspAddr;
  dDuration = dDuration;

  /*p_ssa_ctxt = (icf_ssa_ctxt_st *)((Sdf_st_appData*)(pContextData->pApplicationData));*/
  p_ssa_ctxt = (icf_ssa_ctxt_st *)(pCallObj->pAppData->pData);

  if ( ICF_NULL != p_ssa_ctxt)
  {
    p_glb_pdb = p_ssa_ctxt->p_glb_pdb;

    icf_ssa_init_pdb(&ssa_pdb, p_glb_pdb);
    ssa_pdb.p_ecode = &ecode;
    ssa_pdb.p_ssa_glb_data = p_ssa_ctxt->p_ssa_glb_data;
    ssa_pdb.p_glb_pdb = p_glb_pdb;
    p_ssa_ctxt->p_ssa_pdb =
      (struct icf_ssa_pdb_st*)(&ssa_pdb);
    ssa_pdb.p_ssa_ctxt = p_ssa_ctxt;
    ssa_pdb.retrans_count = dRetransCount;

    icf_dbm_get_module_glb_data(
        p_glb_pdb,
        ICF_INT_MODULE_CFG,
        (icf_void_t **)&(ssa_pdb.p_glb_cfg_data),
        ssa_pdb.p_ecode);

    icf_dbm_get_module_glb_data(
        p_glb_pdb,
        (icf_int_module_id_t)ICF_INT_MODULE_REGM,
        (icf_void_t *)&(ssa_pdb.p_glb_rgm_data),
        ssa_pdb.p_ecode);

    if ((pContextData->dType == Sdf_en_overlapTransaction) ||
        (pContextData->dType == Sdf_en_RegularUasTransaction) ||
        (pContextData->dType == Sdf_en_RegularUacTransaction) )
    {
      pOverlapTxn = (Sdf_st_overlapTransInfo *)pContextData->pTransaction;
    }

    if (Sdf_co_null == pOverlapTxn)
    {
      if(ICF_NULL != pCallObj->pUacTransaction->pSipMsg)
         p_sip_msg = pCallObj->pUacTransaction->pSipMsg;
       else
         p_sip_msg = pCallObj->pUasTransaction->pSipMsg;
    }
    else
    {
      p_sip_msg = pOverlapTxn->pSipMsg;
    }

    if(sdf_ivk_uaGetTransactionForSipMsg(\
          (pCallObj), p_sip_msg, &pTransaction, \
          &pTransInfo, &Error) == Sdf_co_success)
    {
      if(pTransInfo == Sdf_co_null)
      {
        p_sip_msg = pTransaction->pSipMsg;
        sdf_ivk_uaFreeTransaction(pTransaction);
      }
      else
      {
        p_sip_msg = pTransInfo->pSipMsg;
        sdf_ivk_uaFreeOverlapTransInfo( \
            pTransInfo);
      }
    }


    if (ICF_NULL != p_sip_msg)
    {
      if( SipFail != sip_getMessageType(p_sip_msg, &dReqRespType, (SipError*)
            &(Error.stkErrCode))) 
      {
        if(SipMessageResponse == dReqRespType)
        {
          /* move the call context NULL check to here as the call context
          will be NULL in non INVITE related transactions and we have to 
          fill the ERROR code for these messages too */
          if (ICF_NULL != ssa_pdb.p_ssa_ctxt->p_call_ctxt &&
            Sdf_co_fail != sdf_ivk_uaGetRespCodeFromSipMessage(
                p_sip_msg, &resp_code, &Error))
          {
            ssa_pdb.p_ssa_ctxt->p_call_ctxt->\
              inc_response_code = resp_code;
          }
          else
          {
            Error.errCode = Sdf_en_noUaError;
          }  
        }

        if(Sdf_co_fail ==
            sdf_ivk_uaGetMethodFromSipMessage(p_sip_msg, &p_method, &Error))

        {
          Error.errCode = Sdf_en_noUaError;
        }
      }
      else
      {
        Error.errCode = Sdf_en_noUaError;
      }
    }

    if(p_method)
      sdf_fn_uaGetMethodNameAsEnum(p_method, &dMsgType, &Error);
    /*  If it is forceful call clear then the message 
     *  will not retransmit so the error should
     *  be set */
    if(ICF_FALSE == ssa_pdb.p_ssa_ctxt->no_retrans_req)
    {
        ICF_SSA_SET_ECODE(p_ssa_ctxt->p_ssa_pdb,p_method,ICF_ERROR_INTERMIDIATE_RETRANSMISSION_TIMEOUT)
          Error.errCode = Sdf_en_noUaError;
    }
    if(ICF_NULL != ssa_pdb.p_ssa_ctxt->p_call_ctxt)
    {
      ssa_pdb.p_ssa_ctxt->p_call_ctxt->\
        inc_response_code = ICF_NULL;
    }
  /* SPR17855: Added null check for klocworks warning removal*/   
  if(Sdf_co_null != p_method)
   {    
     if(icf_port_strcmp((icf_uint8_t *)p_method,(icf_uint8_t *)"INVITE")== 0)
     {
         ICF_STATS(ssa_pdb.p_glb_pdb->p_stats_info->
             stat_requests.stat_outbound_retrans_req.stats_INVITE)
     }
     else if(icf_port_strcmp((icf_uint8_t *)p_method,(icf_uint8_t *)"PRACK")== 0)
     {
       ICF_STATS(ssa_pdb.p_glb_pdb->p_stats_info->
           stat_requests.stat_outbound_retrans_req.stats_PRACK)
     }
    else if(icf_port_strcmp((icf_uint8_t *)p_method,(icf_uint8_t *)"UPDATE")== 0)
     {
       ICF_STATS(ssa_pdb.p_glb_pdb->p_stats_info->
           stat_requests.stat_outbound_retrans_req.stats_UPDATE)
     }
    else if(icf_port_strcmp((icf_uint8_t *)p_method,(icf_uint8_t *)"INFO")== 0)
     {
       ICF_STATS(ssa_pdb.p_glb_pdb->p_stats_info->
           stat_requests.stat_outbound_retrans_req.stats_INFO)
     }
    else if(icf_port_strcmp((icf_uint8_t *)p_method,(icf_uint8_t *)"OPTIONS")== 0)
     {
       /* SPR 17862: Modified the method name as OPTIONS instead of 
          OPTION in strcmp */
       ICF_STATS(ssa_pdb.p_glb_pdb->p_stats_info->
           stat_requests.stat_outbound_retrans_req.stats_OPTION)
     }
    else if(icf_port_strcmp((icf_uint8_t *)p_method,(icf_uint8_t *)"CANCEL")== 0)
     {
       ICF_STATS(ssa_pdb.p_glb_pdb->p_stats_info->
           stat_requests.stat_outbound_retrans_req.stats_CANCEL)
     }
    else if(icf_port_strcmp((icf_uint8_t *)p_method,(icf_uint8_t *)"BYE")== 0)
     {
        ICF_STATS(ssa_pdb.p_glb_pdb->p_stats_info->
           stat_requests.stat_outbound_retrans_req.stats_BYE)
     }
    else if(icf_port_strcmp((icf_uint8_t *)p_method,(icf_uint8_t *)"REGISTER")== 0)
     {
        ICF_STATS(ssa_pdb.p_glb_pdb->p_stats_info->
           stat_requests.stat_outbound_retrans_req.stats_REGISTER)
     }

    else if(icf_port_strcmp((icf_uint8_t *)p_method,(icf_uint8_t *)"SUBSCRIBE")== 0)
     {
        ICF_STATS(ssa_pdb.p_glb_pdb->p_stats_info->
           stat_requests.stat_outbound_retrans_req.stats_SUBSCRIBE)
     }
    else if(icf_port_strcmp((icf_uint8_t *)p_method,(icf_uint8_t *)"REFER")== 0)
     {
        ICF_STATS(ssa_pdb.p_glb_pdb->p_stats_info->
           stat_requests.stat_outbound_retrans_req.stats_REFER)
     }
    else if(icf_port_strcmp((icf_uint8_t *)p_method,(icf_uint8_t *)"NOTIFY")== 0)
     {
        ICF_STATS(ssa_pdb.p_glb_pdb->p_stats_info->
           stat_requests.stat_outbound_retrans_req.stats_NOTIFY)
     }
    else if(icf_port_strcmp((icf_uint8_t *)p_method,(icf_uint8_t*)"PUBLISH")== 0)
     {
        ICF_STATS(ssa_pdb.p_glb_pdb->p_stats_info->
           stat_requests.stat_outbound_retrans_req.stats_PUBLISH)
     }
    else if(icf_port_strcmp((icf_uint8_t*)p_method,(icf_uint8_t*)"MESSAGE")== 0)
     {
        ICF_STATS(ssa_pdb.p_glb_pdb->p_stats_info->
           stat_requests.stat_outbound_retrans_req.stats_MESSAGE)
     }
    
   } 
 
  }/* if ( ICF_NULL != p_ssa_ctxt ... */

}
#ifdef SDF_PRES
/***********************************************************************
** FUNCTION:sdf_cbk_uaResolvePresUrl 
**
** DESCRIPTION: This call back is thrown by UATK to resolves a PRES URL
**              into the other schemes.Here only the dummy definition of
**              this function is provided only to make compile and link
**              IPTK successfully when SDF_PRES flag in enable. 
**               
************************************************************************/
Sdf_ty_retVal sdf_cbk_uaResolvePresUrl
( Sdf_ty_s8bit	*pPresUrl, Sdf_ty_s8bit **ppReslUrl, Sdf_st_error *pError)
{
    (void)pPresUrl;
    (void)ppReslUrl;
    (void)pError;
    return Sdf_co_success;
}
#endif
