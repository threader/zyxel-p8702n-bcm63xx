/*******************************************************************************
 *
 * FILE NAME :      icf_cfg_init.c
 *
 * DESCRIPTION:     This file contains the functions to initialize and 
 *                  de-initialize the global database of CFG.
 *
 * REVISION HISTORY:
 *
 * Date         Name                Ref#        Comments
 * --------     ------------        ----        ---------------
 * 21-Jan-2005  Shefali Aggarwal    ICF LLD  Initial
 * 27-Jun-2005  Jalaj Negi          IMS CLIENT  Added code for bandwidth
 *                                              support in IMS CLIENT
 * 12-Jul-2005  Shradha Budhiraja   Rel 2.0 LLD
 * 1-Aug-2005   Shradha Budhiraja   Rel 2.0     Code review comments

 * 08-Dec-2005  Rohit Aggarwal      ICF 4.0 CDD Code changes for IPSec support
 * 02-Feb-2006  Amit Sharma                     CFG merged with ICF 2.0
 * 30-Mar-2006    Anuradha Gupta	ICF 5.0   Call Modify Feature changes
 * 28-Nov-2006  Deepti Goyal        ICFRel 6.2  NAT Feature Support.
 * 11-Dec-2006	Deepti Goyal	    SPR 14029	Changes to Update the Contact Ip address 
 *                      						with Self ip address.
 * 28-Dec-2006	Deepti Goyal	    SPR 14173	Changes to initialize is_initial_reg
 *                                              field in rport_config_data
 * 07-Nov-2008  Anurag Khare        SPR 19168   Changes to initialise the field
 *	                                        port_to_be_sent_in_from_header
 *
 * 07-Jan-2009  Vinay Jindal        SPR 19250  Initialized 'conf_fail_call_clear_not_reqd'
 * 05-Feb-2009   Rohit Aggarwal  SPR 19316   Changes for INVITE with Replaces
 *
 * Copyright (c) 2009, Aricent.
 ******************************************************************************/

#include "icf_map_extif_feature_flags.h"
#include "icf_common_prototypes.h"
#include "icf_cfg_macro.h"
#include "icf_cfg_defs.h"
#include "icf_cfg_prototypes.h"

/*******************************************************************************
 *
 * FUNCTION:        icf_cfg_init
 *
 * DESCRIPTION:     This function is invoked at startup and is used to take the
 *                  default data values needed by CFG.
 *
 ******************************************************************************/
icf_return_t icf_cfg_init(
        INOUT   icf_glb_pdb_st           *p_glb_pdb,
        OUT     icf_void_t               **p_p_cfg_data,
        OUT     icf_error_t              *p_ecode)
{
    icf_return_t                         ret_val = ICF_SUCCESS;
    icf_config_data_st                   *p_config_data = ICF_NULL;
#ifdef ICF_IPV6_ENABLED
	icf_uint32_t dType = 0;
#endif
    icf_uint8_t i =0;

    ICF_SET_MODULE(p_glb_pdb, ICF_INT_MODULE_CFG)
    ICF_FUNCTION_ENTER(p_glb_pdb)

    /* Allocate memory for config global data */
    ICF_STATIC_MEMGET(
            p_glb_pdb,
            ICF_PORT_SIZEOF(icf_config_data_st),
            p_config_data,
            ICF_RET_ON_FAILURE,
            p_ecode,
            ret_val)

    /* Set default values for various parameters */
    
    /* num_of_calls_to_log */
    p_config_data->num_of_calls_to_log = ICF_CFG_DEF_NUM_OF_CALLS_TO_LOG;

    /*----------- Transport data ----------------*/

    /* self_ip_address */
    p_config_data->self_ip_address.addr.addr_type =
            ICF_TRANSPORT_ADDRESS_IPV4_ADDR;
   
    p_config_data->self_ip_address.addr.addr.ipv4_addr.octet_1 = 0;
    p_config_data->self_ip_address.addr.addr.ipv4_addr.octet_2 = 0;
    p_config_data->self_ip_address.addr.addr.ipv4_addr.octet_3 = 0;
    p_config_data->self_ip_address.addr.addr.ipv4_addr.octet_4 = 0;
    
    p_config_data->self_ip_address.port_num = ICF_CFG_DEF_SELF_PORT;
    
    /* self_mode */
    p_config_data->self_mode = ICF_TRANSPORT_MODE_BOTH;

	

    /*------------- Call data -------------------*/


    /* default_settings */
    p_config_data->default_settings = ICF_CFG_CC_CALL_THRU_SIP_PROXY |
                                      ICF_CFG_CC_PRIVACY_HEADER_PRESENT;

    /* qos_mark */
    p_config_data->qos_mark = ICF_QOS_MARK_NONE;

    /* setup_timer */
    p_config_data->setup_timer = ICF_CFG_DEF_SETUP_TIMER;

    /* options_timer */
    p_config_data->options_timer = ICF_CFG_DEF_OPTIONS_TIMER;

    /* alerting_timer */
    p_config_data->alerting_timer = ICF_CFG_DEF_ALERTING_TIMER;

    /* release_timer */
    p_config_data->release_timer = ICF_CFG_DEF_RELEASE_TIMER;

    /* modify_media_timer */
    p_config_data->modify_media_timer = ICF_CFG_DEF_MODIFY_MEDIA_TIMER;

    /* registration_resp_timer */
    p_config_data->registration_resp_timer = ICF_CFG_DEF_REG_RESP_TIMER;

    /* registration_retry_timer */
    p_config_data->registration_retry_timer = ICF_CFG_DEF_REG_RETRY_TIMER;

    /* registration_retry_timer */
    p_config_data->info_timer = ICF_CFG_DEF_INFO_TIMER;

#ifndef ICF_FEATURE_ABANDON
    /* Phone info -User agent Header */
    p_config_data->p_phone_info = ICF_NULL;
#endif

	p_config_data->subs_resp_timer = ICF_CFG_DEF_SUBS_RESP_TIMER;
		
    p_config_data->subs_retry_timer = ICF_CFG_DEF_SUBS_RETRY_TIMER;
#ifdef ICF_SESSION_TIMER
	p_config_data->session_timer = ICF_CFG_DEF_SESSION_TIMER_INTERVAL;
	p_config_data->min_se = ICF_CFG_DEF_MIN_SESSION_TIMER_INTERVAL;
    /* By default, session timer will be enabled at run time.*/
        p_config_data->session_timer_enable = ICF_TRUE;

        /* Session refresher is TRUE by default not to disturb existing handling.*/
        p_config_data->session_refresher_reqd = ICF_TRUE;

        /* session_timer_logic is FALSE by default not to disturb
         * existing timer logic.*/
        p_config_data->session_timer_logic = ICF_FALSE;
#endif

    /*------------------- Service data ------------------------*/
#ifdef ICF_SERVICE_ENABLE
    /* Global Subscription Flag */
    p_config_data->global_service_subscription_flag = ICF_NULL;
    
    /* Global Activation Flag */
    p_config_data->global_service_activation_flag = ICF_NULL;

    /* call_hold_option */
    p_config_data->hold_option = ICF_CALL_HOLD_OPT_INV_WTH_MODE_SEND_ONLY;
#endif
    /* max_num_of_call_forwarding_hops */
    p_config_data->max_num_of_call_forwarding_hops = 
        ICF_CFG_DEF_NUM_CALL_FWD_HOPS;

    /* max_num_of_calls_in_call_waiting */
    p_config_data->max_num_of_calls_in_call_waiting = 
        ICF_CFG_DEF_MAX_CALLS_WAITING;

    /* call_hold_resume_timer */
    p_config_data->call_hold_resume_timer = ICF_CFG_DEF_CHR_TIMER;

    /* reinvite_race_timer */
    p_config_data->reinvite_race_timer = ICF_CFG_DEF_REINVITE_RACE_TIMER;

    /* twc_timer */
    p_config_data->twc_timer = ICF_CFG_DEF_TWC_TIMER;

    /* ctu_timer */
    p_config_data->ctu_timer = ICF_CFG_DEF_CTU_TIMER;

    /* cta_timer */
    p_config_data->cta_timer = ICF_CFG_DEF_CTA_TIMER;

    /* rnw_timer */
    p_config_data->rnw_timer = ICF_CFG_DEF_RNW_TIMER;
    
    /* cw_alert_timer */
    p_config_data->cw_alert_timer = ICF_CFG_DEF_CW_ALERT_TIMER;

    /* cnd_alert_timer */
    p_config_data->cnd_alert_timer = ICF_CFG_DEF_CND_ALERT_TIMER;

    /* cw_n_cnd_alert_timer */
    p_config_data->cw_n_cnd_alert_timer = ICF_CFG_DEF_CW_N_CND_ALERT_TIMER;

    /* config_data_init_complete */
    p_config_data->config_data_init_complete = ICF_FALSE;

/*-------------------------VMS Data----------------------------------*/
#ifndef ICF_FEATURE_ABANDON
    /* Group VMS Address */
    p_config_data->p_group_vms_addr = ICF_NULL;

    /* Group VMS Duration */
    p_config_data->vms_subscribe_duration = ICF_CFG_DEF_SUBS_DURATION;
    
    /* VMS Subscription Status */
    p_config_data->vms_subscription_status = ICF_NULL;
#endif
    /* scale_param_recd */
    p_config_data->scale_param_recd = ICF_FALSE;

    /* Global init complete flag to be set to FALSE */
    p_glb_pdb->p_glb_data->global_data_init_complete = ICF_FALSE;

    p_config_data->dns_min_retrans = ICF_CFG_DEF_DNS_MIN_RETRANS;
        
    p_config_data->dns_max_retrans = ICF_CFG_DEF_DNS_MAX_RETRANS;

ICF_INIT_START
	p_config_data->max_applications = 0; 
#ifdef ICF_NW_ACTIVATE
	p_config_data->max_networks = 0; 
#endif
    p_config_data->max_registration_req = 0;
#ifdef ICF_IPSEC_ENABLED
    /* SPR 8517 fix */
    p_config_data->ipsec_enabled = ICF_DEFAULT_IPSEC_ENABLED;
    p_config_data->p_ipsec_config_data = ICF_NULL;
#endif /* ICF_IPSEC_ENABLED */
ICF_INIT_END

	p_config_data->nw_res_reservation_supported = ICF_FALSE;

#ifdef IMS_CLIENT

	/* This code will be removed after configuration manager for IMS CLIENT
	is attached */
	/*********************************************************************/
	/*p_config_data->default_settings |= ICF_CFG_CC_CONFIG_PRACK_REQD;*/

	/*********************************************************************/
    /* Code to be removed ends */
	
	/* This specifies number of codecs supported by IMS CLIENT */
    p_config_data->codec_bw_map_list.num_of_codecs = 3;
    
    /* Filling codec type (IANA) */
    p_config_data->codec_bw_map_list.codec_bw_map[0].codec_id = 
        ICF_IANA_PT_G711_ULAW;
    /* Number of bandwidths supported */
    p_config_data->codec_bw_map_list.codec_bw_map[0].num_of_bw_supported = 1;
    /* Exact bandwidth supported by the codec */
    p_config_data->codec_bw_map_list.codec_bw_map[0].as_bandwidth[0] = 
        ICF_IANA_PT_G711_ULAW_BW;

    /* Filling codec type (IANA) */
    p_config_data->codec_bw_map_list.codec_bw_map[1].codec_id = 
        ICF_IANA_PT_G711_ALAW;
    /* Number of bandwidths supported */
    p_config_data->codec_bw_map_list.codec_bw_map[1].num_of_bw_supported = 1;
    /* Exact bandwidth supported by the codec */
    p_config_data->codec_bw_map_list.codec_bw_map[1].as_bandwidth[0] = 
        ICF_IANA_PT_G711_ALAW_BW;    

    /* Filling codec type (IANA) */
    p_config_data->codec_bw_map_list.codec_bw_map[2].codec_id = 
        ICF_IANA_PT_G729_8;
    /* Number of bandwidths supported */
    p_config_data->codec_bw_map_list.codec_bw_map[2].num_of_bw_supported = 1;
    /* Exact bandwidth supported by the codec */
    p_config_data->codec_bw_map_list.codec_bw_map[2].as_bandwidth[0] = 
        ICF_IANA_PT_G729_8_BW;    
#endif

#ifdef IMS_CLIENT    
    p_config_data->ims_oprn_flag = ICF_TRUE;
#else
    p_config_data->ims_oprn_flag = ICF_FALSE;
#endif

ICF_HANDLE_REINVITE_START
	/* By default we send INVITE for a call modification request */
	p_config_data->call_modify_req_method = ICF_CFG_CALL_MODIFY_METHOD_INVITE;

ICF_HANDLE_REINVITE_END

#ifdef ICF_NW_ACTIVATE
   p_config_data->gprs_suspended = ICF_TRUE;
#endif
#ifdef ICF_NAT_RPORT_SUPPORT
   {

   icf_rport_config_data_st    *p_rport_config_data =
         &p_config_data->rport_config_data;

   /* initialise the rport config data */
   icf_cfg_rport_config_data_init(p_glb_pdb,&p_rport_config_data);

   icf_port_memcpy((icf_transport_address_st *)&(p_config_data->rport_config_data.contact_ip_addr),\
		(icf_transport_address_st*)&(p_config_data->self_ip_address),
        (icf_uint32_t)ICF_PORT_SIZEOF(icf_transport_address_st));
   p_config_data->rport_config_data.contact_ip_addr.port_num = \
	     p_config_data->self_ip_address.port_num;
   }
#endif

  p_config_data->conf_reqd_at_transferee = ICF_TRUE;
  p_config_data->conf_cfm_reqd_at_transferee = ICF_FALSE;
  p_config_data->conf_fail_call_clear_not_reqd = ICF_FALSE;
  /* While init dns_configuration bitnask will be initialised with 0.*/
  p_config_data->dns_configuration = 0;

  /* while cfg init dns_source_port will be 0, because during init time dns_configuration
   * bitmask will be 0 and IPTK will not check for dns_port while making DNS query,
   * it will use random port.
   */ 
  p_config_data->dns_source_port = 0;
  /*  Fix for spr 19168 */
  /*  Make this value as True because IPTK
   *  by default sends port in FROM header */
   p_config_data->port_to_be_sent_in_from_header = ICF_TRUE;

	/* Fix for CSR 1-6727350: SPR 19316 */
	p_config_data->app_connect_reqd_for_replaces = ICF_FALSE;

   /*Initialize the max servers*/
  for (i = 0; i <= ICF_WIN_TLS_SIP_SERVER; i++)
  {
	p_config_data->server_index[i] = ICF_MAX_NUM_OF_SERVERS;
  }  
    /* CFG's global data pointer needs to be returned to the calling module */
    ICF_ASSERT(p_glb_pdb, (ICF_NULL != p_p_cfg_data))
    *p_p_cfg_data = (icf_void_t *)(p_config_data);

    ICF_FUNCTION_EXIT(p_glb_pdb)
    ICF_RESET_MODULE(p_glb_pdb)
    return ret_val;
} /* End of icf_cfg_init */


/*******************************************************************************
 *
 * FUNCTION:        icf_cfg_deinit
 *
 * DESCRIPTION:     This function is invoked at system termination and is used 
 *                  to free the memory allocated at init time.
 *
 ******************************************************************************/
icf_void_t icf_cfg_deinit(
        INOUT   icf_glb_pdb_st           *p_glb_pdb,
        OUT     icf_void_t               *p_data,
        OUT     icf_error_t              *p_ecode)
{
    icf_return_t                         ret_val = ICF_SUCCESS;
    icf_config_data_st	                *p_config_data = ICF_NULL;
    
    ICF_SET_MODULE(p_glb_pdb, ICF_INT_MODULE_CFG)
    ICF_FUNCTION_ENTER(p_glb_pdb)

    p_config_data = (icf_config_data_st *)(p_data);

#ifdef ICF_IPSEC_ENABLED

    /* free the IPSec init data stored in CFG global data */
    
    if (ICF_NULL != p_config_data->p_ipsec_config_data)
    {
        ICF_STATIC_MEMFREE(
            p_glb_pdb,
            p_config_data->p_ipsec_config_data,
            p_ecode,
            ret_val)
    }

#endif /* ICF_IPSEC_ENABLED */

    if (ICF_NULL != p_config_data->p_tls_init_data)
    {
        ICF_STATIC_MEMFREE(
            p_glb_pdb,
            p_config_data->p_tls_init_data,
            p_ecode,
            ret_val)
    }

    ICF_STATIC_MEMFREE(
            p_glb_pdb,
            p_data,
            p_ecode,
            ret_val)

    ICF_FUNCTION_EXIT(p_glb_pdb)
    ICF_RESET_MODULE(p_glb_pdb)
    return;
} /* End of icf_cfg_deinit */
