/*******************************************************************************
*                                        
* FILE NAME    :icf_pack_api.c
*                                        
* DESCRIPTION  :The file contains the routines for the buffer packing code.
*               Fraction of this code is automatically generated.
*                                        
* Revision History :                
*                
*    DATE         NAME           REFERENCE     REASON
* --------      ------------     ---------     --------
* 27-JUL-2005    Ram Narayan       None        Initial
* 02-Feb-2006  Amit Sharma                     CFG merged with ICF 2.0
* 08-Feb-2006  Amit Sharma               Network PDU Logging API from ICF
* 07-Mar-2006	Anuradha Gupta	   ICF			Added code changes for Re-invite handling
* 30-Mar-2006	Anuradha Gupta	   ICF 5.0		Call Modify Feature changes
* 31-Mar-2006   Deepti Goyal       ICF_5_0      Changes for encoding ICF message
*                                               Body headers.
*  
* 02-Apr-2006	Puneet Sharma	ICF5.0 LLD	 changes done in the comments 
*											 Multiple Public Users ICF5.0
* 27-June-2006  Raveen Sharma	   ICF 6.0	 Added code changes for list handling
* 29-Nov-2006   Deepti Goyal        ICF 6.2     Set Call Params API change.
* 03-Jan-2007   Amit Mahajan                    INFO confirmation from application changes.
* 15-Jan-2007   Abhishek Dhammawat  ICF6.3   	Per Line Feature Additions.   
*
* 23-MAr-2007   Deepti Goyal        ICF 6.4     Added function for media_connect
*                                               request API
* 08-JUN_2007   Alok Tiwari      ICF Early 7.0  Added functions for STAT and
*
* 10-July-2007  Abhishek Dutta   ICF 7.0        Added function for cancel call
*                                               modify request API
* 27-Aug-2007   Deepti Goyal       SPR 15997    Line Id in configure_cfm API.
* 14-Jan-2008	Shruti Thakral					Changes for Secondary Server 
*												Support in Rel 8.0
* 16-Jan-2008	Neha Gupta	   SPR 18033    Added support new API for func
*						failure simulation for UT.
* 16-Jan-2008   Rajiv Kumar        ICF 8.0      Registration Procedure
*                                               Enhancements
* 17-Jan-2008   Abhishek Dhammawat ICF 8.0      Additions for 
*                                               precondition_required field
* 18-Jan-2008   Kamini Gangwani    Rel 8.0      Addition for packing of 
*                                               media_transport_type
*                                               in icf_set_call_param_req_st
* 22-JAN-2008   Anurag Khare     IPTK 8.0       Changes for DHCP Server support
* 15-Feb-2008   Tarun Gupta      IPTK Rel 8.0   Changes for AKA handling
* 22-Feb-2008   Abhishek Dhammawat IPTKRel8.0   QOS STATS updation 
* 14-Mar-2008   Rajiv Kumar      IPTK Rel 8.0   Enhancements for Rel8.0
*                                               Packing of new field retry-after
*                                               in register status indicataion
* 07-MAR-2008   Vipul Varshney  SPR 18122       AKA Enhancement
* 24-Mar-2008   Tarun Gupta     IPTK Rel 8.0    Merged User Param and
*                                               Dynamic Payload Enhancements
* 19-Mar-2008   Sumant Gupta     IPTK 8.0       Merging Publish & Message
* 4-Apr-2008   Sumant Gupta    ICF 8.0         Rel7.1 Windows porting Merging
* 30-May-2008  Tarun Gupta     SPR 18585       Changed name of API from icf_remote_user_term_call_resp
                                               to icf_remote_user_term_call_resp_st
* 04-Jun-2008  Tarun Gupta     SPR 18585       Modified packing for icf_set_trace_req_st  
* 07-Jun-2008  Amit Sharma       IPTK 8.0      SRTP LLD      SRTP Enhacf_sncements
* 25-Jun-2008  Abhishek Dhammawat IPTK Rel8.0  Modified 
*                                              icf_pack_icf_set_line_param_req_st
* 07-July-2008 Anurag Khare     SPR 18684      CSR 1-6073561 Merge 
* 5-Jul-2008   Amit Sharma     SPR:18714       packing icf_display_ind_st
* 08-Dec-2008  Rajiv Kumar     SPR 19218       Merged CSR 1-6431768
* 11-Dec-2008  Anurag Khare    SPR 19218       CSR 1-6456839 Merged
* 15-Dec-2008  Tarun Gupta     SPR 19218       Merged CSR 1-5478785
* 30-Jan-2009  Rajiv Kumar     IPTK Rel8.1     Changes done for testing UATK
*                                              API under ICF_UT_TEST flag
* 11-Feb-2009  Alok Tiwari     IPTK Rel8.1     Change to fix the crash when
*                                            empty message body is received.
* 11-Feb-2009  Ashutosh Mohan  IPTK Rel8.1     Added code for following functions:
*                                              icf_pack_icf_tunnelled_options_resp_st
*                                              icf_pack_icf_early_session_cfm_st
*                                              icf_pack_icf_early_session_ind st
*                                              icf_pack_icf_update_req_send_st
*                                              icf_pack_icf_tunnelled_sdp_info_st
*                                              Changes done in following functions:
*                                              icf_pack_icf_create_call_req_st
*                                              icf_pack_icf_early_incoming_call_cfm_st
*                                              icf_pack_icf_set_call_params_req_st
*                                              icf_pack_icf_options_req_st
*                                              icf_pack_icf_incoming_call_modify_ind_st
* 16-Feb-2009 Tarun Gupta    Rel 8.1           SDP Tunnelling Enhancements
* 16-Feb-2009 Anurag Khare      Rel 8.1        changes done in 
*                                              icf_pack_icf_call_modify_req_st
* 17-Feb-2009 Ashutosh Mohan   Rel 8.1         Changes done in icf_connect_ind_st
* 17-Feb-2009 Tarun Gupta      Rel 8.1         SDP Tunnelling Enhancements
* 27-Feb-2009 Preksha          Rel 8.1         Modified icf_pack_icf_func_fail_st 
* 04-Mar-2009 Saurabh Gupta	   SPR 19349       Klocworks fix
* 15-Mar-2009 Anuradha Gupta   Rel 8.1         Added code for TImer Enh
* 18-Mar-2009 Alok Tiwari      Rel 8.1         Added code for REGISTRATION
*                                              Enhancement(Multiple contact feature) 
* 22-Mar-2009 Abhishek Dhammawat Rel 8.1     Modified report_error_ind packing  
* 22-Mar-2009 Abhishek Dhammawat Rel 8.1     Modified report_error_ind packing   
* 27-Mar-2009 Ashutosh Mohan     Rel 8.2     Modified icf_pack_icf_register_req_st,
*                                                     icf_pack_icf_register_status_ind_st
*                                                     icf_pack_icf_register_cfm_st
*                                                     icf_pack_icf_create_call_req_st 
* 30-Mar-2009 Abhishek Dhammawat Rel 8.2     Added code for REFER/NOTIFY
*                                            Enhancements
* 31-Mar-2009 Preksha            Rel 8.2     Added code for PRACK
*                                            Enhancements
* 31-Mar-2009 Anuradha Gupta     Rel 8.2     Modified register_req / create_call packing   
* 27-Mar-2009 Alok Tiwari        Rel 8.2     Modified function:
*                                            icf_pack_icf_register_req_st
*                                            icf_pack_icf_aka_auth_resp_st 
* 03-Apr-2009 Tarun Gupta        Rel 8.2     REFER/NOTIFY Enhancements
* 15-Apr-2009 Abhishek Dhammawat SPR 19441   Modified media_connect_ind
*                                            packing for rel_prov_resp
* 18-May-2009 Anuradha Gupta     SPR 19672   Modified fn 
*                                            icf_pack_icf_set_call_params_req_st    
* 21-May-2009 Anuradha Gupta     SPR 19672   Modified fn 
*                                            icf_pack_icf_set_call_params_req_st  
* 26-May-2009 Anuradha Gupta     SPR 19672   Merged CSR 1-6667668 
* 26-May-2009 Anuradha Gupta     SPR 19672   Merged CSR 1-6727350
* 28-May-2009 Tarun Gupta        Rel 8.2     Async NOTIFY Support
* 28-May-2009 Ashutosh Mohan     SPR 19672   CSR-1-6976670 Merged
* 29-May-2009 Kamal Ashraf       SPR 19672   CSR_1_7113139 Merged
* 01-Jun-2009 Preksha           Rel 8.2      Asynchronus Message enhancements
* 18-Jun-2009 Anuradha Gupta    SPR 19752    Modified icf_pack_icf_set_call_params_req_st
* 03-Jul-2009 Anuradha Gupta     SPR 19754   Modified 
*                                            icf_pack_icf_incoming_call_modify_ind_st
* 03-Aug-2009 Tarun Gupta       SPR 19886    Merged CSR 1-7193955
* 03-Aug-2009 Tarun Gupta       SPR 19886    Merged CSR 1-7334184
* 13-Aug-2009 Anuradha Gupta    SPR 19748    Modified fn icf_pack_icf_report_trace_ind_st
* 08-Sep-2009 Anuradha Gupta    SPR 19999    Merged CSR 1-7533733
* 11-Sep-2009 Rajiv Kumar       SPR 19999    Merged CSR 1-7547597
* 12-Sep-2009 Rajiv Kumar       SPR 19999    Merged CSR 1-7467002
* 23-Sep-2009 Tarun Gupta       SPR 20063    Merged CSR 1-7616406
* 25-May-2010  Preksha          SPR 20412   Merged CSR 1-8389294
* 27-May-2010  Preksha           SPR 20412   Merged CSR 1-8420460
* 29-May-2010 Alok Tiwari       SPR 20412    Merged CSR 1-8223807
* 9-Jul-2010  Udit Gattani      SPR 20517   Added parameter is_sdp_received in
*                                           icf_pack_icf_remote_user_alerted_ind_st 
*
* Copyright 2010, Aricent.                       
*******************************************************************************/



#include "icf_clib.h"
#include "icf_utils.h"
#include "icf_api.h"
#include "icf_trace_error_types.h"
#include "icf_trace_id.h"
#include "icf_stats_types.h"


icf_uint32_t
icf_pack_icf_set_call_params_req_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t    *p_length
)
{
    icf_uint32_t        offset = 0;

    icf_set_call_params_req_st    *p_icf_set_call_params_req_st = ICF_NULL;
    
    p_icf_set_call_params_req_st    = (icf_set_call_params_req_st *)p_src;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function packs icf_set_call_params_req_st */
    ICF_CLIB_TRACE(("Packing icf_set_call_params_req_st\n"));
    
    icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_set_call_params_req_st->bit_mask), &offset);
    
    *p_length    += offset;
 
    /*  CSR 1-6073561 Merge */ 
    icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_set_call_params_req_st->bit_mask_1), &offset);
    
    *p_length    += offset;

    if(p_icf_set_call_params_req_st->bit_mask & ICF_SET_CALL_PRM_DEFAULT_SETTINGS)
    {
        icf_pack_U16(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_set_call_params_req_st->default_settings), &offset);
        
        *p_length    += offset;
    }

    if(p_icf_set_call_params_req_st->bit_mask & ICF_SET_CALL_PRM_QOS_MARK)
    {
        icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_set_call_params_req_st->qos_mark), &offset);
        
        *p_length    += offset;
    }

    if(p_icf_set_call_params_req_st->bit_mask & ICF_SET_CALL_PRM_SETUP_TIMER)
    {
        icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_set_call_params_req_st->setup_timer), &offset);
        
        *p_length    += offset;
    }

    if(p_icf_set_call_params_req_st->bit_mask & ICF_SET_CALL_PRM_ALERTING_TIMER)
    {
        icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_set_call_params_req_st->alerting_timer), &offset);
        
        *p_length    += offset;
    }

    if(p_icf_set_call_params_req_st->bit_mask & ICF_SET_CALL_PRM_RELEASE_TIMER)
    {
        icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_set_call_params_req_st->release_timer), &offset);
        
        *p_length    += offset;
    }

    if(p_icf_set_call_params_req_st->bit_mask & ICF_SET_CALL_PRM_MODIFY_MEDIA_TIMER)
    {
        icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_set_call_params_req_st->modify_media_timer), &offset);
        
        *p_length    += offset;
    }

    if(p_icf_set_call_params_req_st->bit_mask & ICF_SET_CALL_PRM_REGISTRATION_RESP_DURATION)
    {
        icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_set_call_params_req_st->registration_resp_timer), &offset);
        
        *p_length    += offset;
    }

    if(p_icf_set_call_params_req_st->bit_mask & ICF_SET_CALL_PRM_REGISTRATION_RETRY_DURATION)
    {
        icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_set_call_params_req_st->registration_retry_timer), &offset);
        
        *p_length    += offset;
    }

#ifdef ICF_SESSION_TIMER
    if(p_icf_set_call_params_req_st->bit_mask & ICF_SET_CALL_PRM_SESSION_TIMER_INTERVAL)
    {
        icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_set_call_params_req_st->session_timer), &offset);
        
        *p_length    += offset;
    }

    if(p_icf_set_call_params_req_st->bit_mask & ICF_SET_CALL_PRM_MIN_SESSION_TIMER_INTERVAL)
    {
        icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_set_call_params_req_st->min_se), &offset);
        
        *p_length    += offset;
    }
    if(p_icf_set_call_params_req_st->bit_mask & ICF_SET_CALL_PRM_SESSION_TIMER_ENABLE)
    {
         icf_pack_U8(p_dest + (*p_length),(icf_uint8_t *)\
            &(p_icf_set_call_params_req_st->session_timer_enable),&offset);
        
        *p_length    += offset;
    }
    if(p_icf_set_call_params_req_st->bit_mask & ICF_SET_CALL_PRM_SESSION_REFRESHER)
    {
         icf_pack_U8(p_dest + (*p_length),(icf_uint8_t *)\
            &(p_icf_set_call_params_req_st->session_refresher),&offset);
        
        *p_length    += offset;
    }
    if(p_icf_set_call_params_req_st->bit_mask & ICF_SET_CALL_PRM_SESSION_TIMER_LOGIC)
    {
         icf_pack_U8(p_dest + (*p_length),(icf_uint8_t *)\
            &(p_icf_set_call_params_req_st->session_timer_logic),&offset);
        
        *p_length    += offset;
    }
#endif

	if(p_icf_set_call_params_req_st->bit_mask & ICF_SET_CALL_PRM_INFO_TIMER)
    {
        icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_set_call_params_req_st->info_timer), &offset);
        
        *p_length    += offset;
    }
	
	if(p_icf_set_call_params_req_st->bit_mask & ICF_SET_CALL_PRM_OPTIONS_TIMER)
    {
        icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_set_call_params_req_st->options_timer), &offset);
        
        *p_length    += offset;
    }

	if(p_icf_set_call_params_req_st->bit_mask & ICF_SET_CALL_PRM_PHONE_INFO)
    {
        icf_pack_icf_string_st(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_set_call_params_req_st->phone_info), &offset);
        
        *p_length    += offset;
    }
#ifdef ICF_NAT_RPORT_SUPPORT
    if(p_icf_set_call_params_req_st->bit_mask & ICF_SET_BINDING_REFRESH_TIMER)
    {
        icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&\
                (p_icf_set_call_params_req_st->binding_refresh_timer), \
                &offset);

        *p_length    += offset;
    }
    icf_pack_U8(p_dest + (*p_length),(icf_uint8_t *)\
            &(p_icf_set_call_params_req_st->binding_query_enable),&offset);
    *p_length += offset;
#endif

    if(p_icf_set_call_params_req_st->bit_mask & ICF_SET_CALL_PRM_MULTIPLE_ANSWER_ENABLE)
    {
     icf_pack_U8(p_dest + (*p_length),(icf_uint8_t *)\
            &(p_icf_set_call_params_req_st->multiple_answer_enable),&offset);
    *p_length += offset;
    }
    if(p_icf_set_call_params_req_st->bit_mask & ICF_SET_CALL_PRM_UPDATE_REQD)
    {
    icf_pack_U8(p_dest + (*p_length),(icf_uint8_t *)\
            &(p_icf_set_call_params_req_st->update_reqd),&offset);
    *p_length += offset;
    }

    if(p_icf_set_call_params_req_st->bit_mask & ICF_SET_CALL_PRM_EARLY_INC_IND_FOR_ALL_USERS)
    {
    icf_pack_U8(p_dest + (*p_length),(icf_uint8_t *)\
            &(p_icf_set_call_params_req_st->early_inc_ind_for_all_users),&offset);
    *p_length += offset;
    }

    if(p_icf_set_call_params_req_st->bit_mask & ICF_SET_CALL_PRM_DNS_MIN_TIMER)
    {
        icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_set_call_params_req_st->dns_min_retrans), &offset);
        *p_length    += offset;
    }

    if(p_icf_set_call_params_req_st->bit_mask & ICF_SET_CALL_PRM_DNS_MAX_TIMER)
    {
        icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_set_call_params_req_st->dns_max_retrans), &offset);
        *p_length    += offset;
    }
    /*Fix Merged for CSR_1_5400409*/
    if(p_icf_set_call_params_req_st->bit_mask & 
           ICF_SET_CALL_PRM_MAX_ACTIVE_CALLS_PER_LINE)
    {
     icf_pack_U8(p_dest + (*p_length),(icf_uint8_t *)\
         &(p_icf_set_call_params_req_st->max_active_calls_per_line),&offset);
 
     *p_length += offset;
    }
    /* Added for p-access-nw-info header handling */
     if(p_icf_set_call_params_req_st->bit_mask & ICF_SET_CALL_PRM_ACCESS_NW_INFO)
    {
        icf_pack_icf_string_st(p_dest + (*p_length),(icf_uint8_t*)\
            &(p_icf_set_call_params_req_st->access_network_info), &offset);
        
        *p_length    += offset;
    }
     /*Rel 8.0:Added for SRTP Support*/
     if(p_icf_set_call_params_req_st->bit_mask & 
                         ICF_SET_CALL_PRM_MEDIA_TYPE_REQD)
     {
        icf_pack_U32(p_dest + (*p_length),(icf_uint8_t*)\
        &(p_icf_set_call_params_req_st->media_transport_type),\
        &offset);
        *p_length    += offset;
     }
    /*Start: Rel8.0 Added for precondition support */ 
    if(p_icf_set_call_params_req_st->bit_mask &
            ICF_SET_CALL_PRM_PRECONDITION_REQD)
    {    
      icf_pack_U8(p_dest + (*p_length), \
      (icf_uint8_t*)&(p_icf_set_call_params_req_st->precondition_required), \
      &offset);
      *p_length += offset;
    }  
    /*End: Rel8.0 Added for precondition support */ 

    /* SPR 19752 : packing of DHCP response timer should be before Reg Event 
     */      
    /*  Packing of new field dhcp_response_timer */
    if(ICF_SET_CALL_PRM_DHCP_RESPONSE_TIMER & p_icf_set_call_params_req_st->bit_mask)
    {
        icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)& \
                        (p_icf_set_call_params_req_st->dhcp_response_timer), &offset);

        *p_length    += offset;

    }

    /***************ICF 8.0*************************/
    /*Packing of reg_event_subs_reqd field which indicate whether application
      wants to configure support of reg event package subscription*/
    if(p_icf_set_call_params_req_st->bit_mask & 
           ICF_SET_CALL_PRM_REG_EVENT_SUBS_REQD)
    {
     icf_pack_U8(p_dest + (*p_length),(icf_uint8_t *)\
         &(p_icf_set_call_params_req_st->reg_event_subs_reqd),&offset);
 
     *p_length += offset;
    }

    /*  Packing of new field srtp_fallback_reqd */
    if(ICF_SET_CALL_PRM_SRTP_FALLBACK_FLAG & p_icf_set_call_params_req_st->bit_mask)
    {
        icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)& \
                        (p_icf_set_call_params_req_st->srtp_fallback_reqd), &offset);

        *p_length    += offset;

    }

    /*  Packing of new field p_srtp_fallback_resp_codes */
    if(ICF_SET_CALL_PRM_SRTP_FALLBACK_RESP_CODES & p_icf_set_call_params_req_st->bit_mask)
    {
        icf_pack_icf_uint16_list(p_dest + (*p_length), 
                                (p_icf_set_call_params_req_st->p_srtp_fallback_resp_codes), 
                                &offset);

        *p_length    += offset;

    }

    if (p_icf_set_call_params_req_st->bit_mask_1 & 
				ICF_SET_CALL_PRM_APP_PORT_CONFIG)
    {
        icf_pack_U16(p_dest + (*p_length), (icf_uint8_t*)\
				&(p_icf_set_call_params_req_st->app_port_config), &offset);
        
        *p_length    += offset;
    }
    /*   Fix Merged for CSR 1-6456839 */
    if(p_icf_set_call_params_req_st->bit_mask_1 & ICF_SET_PORT_IN_FROM_HDR)
    {
        icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_set_call_params_req_st->port_to_be_sent_in_from_hdr), &offset);
        *p_length    += offset;
    }



    if(ICF_SET_CALL_PRM_EARLY_INC_SESSION_IND_REQD & p_icf_set_call_params_req_st->bit_mask_1)
    {
        icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)& \
                        (p_icf_set_call_params_req_st->early_incoming_session_ind_reqd), &offset);

        *p_length    += offset;

    }

    
 
    if(p_icf_set_call_params_req_st->bit_mask_1 & 
               ICF_SET_CALL_PRM_RETRY_CODEC_COMMIT_REQ)
    {
        icf_pack_U8(p_dest + (*p_length),(icf_uint8_t *)\
                &(p_icf_set_call_params_req_st->retry_codec_commit_req),&offset);

        *p_length    += offset;
    }

	if(p_icf_set_call_params_req_st->bit_mask_1 & 
				ICF_SET_CALL_PRM_UPDATE_FOR_OFFERANSWER_AFTER_CONNECT)
    {
         icf_pack_U8(p_dest + (*p_length),(icf_uint8_t *)\
            &(p_icf_set_call_params_req_st->update_for_offeranswer_after_connect),&offset);
        
        *p_length    += offset;
    }

    /* Fix for CSR 1-6727350: SPR 19316 */
    if (p_icf_set_call_params_req_st->bit_mask_1 & 
			ICF_SET_CALL_PRM_APP_CONNECT_REQD_FOR_REPLACES)
    {
        icf_pack_U8(p_dest + (*p_length), (icf_uint8_t *)\
                &(p_icf_set_call_params_req_st->app_connect_reqd_for_replaces), &offset);
        *p_length    += offset;
    }

    /* Asynchronous NOTIFY support */
    if(ICF_SET_CALL_PRM_ASYNC_NOTIFY_SUPPORT_REQD & 
            p_icf_set_call_params_req_st->bit_mask_1)
    {
        icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)& \
            (p_icf_set_call_params_req_st->async_notify_support_reqd),&offset);
        *p_length    += offset;
    }

    /* Fix for CSR-1-6976670: Packing check_remote_codec_list */

    if(p_icf_set_call_params_req_st->bit_mask_1 &
         ICF_SET_CALL_PRM_CHECK_REMOTE_CODEC_LIST)
    {
         icf_pack_U8(p_dest + (*p_length),(icf_uint8_t *)\
             &(p_icf_set_call_params_req_st->check_remote_codec_list),&offset);
        
        *p_length    += offset;
    }

    /* CSR_1_7113139 Merged SPR 19672 Start */
    /*fix for SPR-19462*/
    if(p_icf_set_call_params_req_st->bit_mask_1 &
                 ICF_SET_CALL_PRM_INC_SDP_VERSION_WHEN_NO_CHANGE)
    {
        icf_pack_U8(p_dest + (*p_length),(icf_uint8_t *)\
            &(p_icf_set_call_params_req_st->inc_sdp_version_when_no_change),&offset);
        
        *p_length    += offset;
    }
    /* CSR_1_7113139 Merged SPR 19672 End */

    /* Asynchronous MESSAGE support */
    if(ICF_SET_CALL_PRM_ASYNC_MESSAGE_SUPPORT_REQD & 
            p_icf_set_call_params_req_st->bit_mask_1)
    {
        icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)& \
            (p_icf_set_call_params_req_st->async_message_support_reqd),&offset);
        *p_length    += offset;
    }

    return ICF_SUCCESS;
}


icf_uint32_t
icf_pack_icf_set_scale_params_req_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t    *p_length
)
{
    icf_uint32_t        offset = 0;

    icf_set_scale_params_req_st    *p_icf_set_scale_params_req_st = ICF_NULL;
    
    p_icf_set_scale_params_req_st    = (icf_set_scale_params_req_st *)p_src;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function packs icf_set_call_params_req_st */
    ICF_CLIB_TRACE(("Packing icf_set_scale_params_req_st\n"));
    
    icf_pack_U16(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_set_scale_params_req_st->bit_mask), &offset);
    *p_length    += offset;

    if(p_icf_set_scale_params_req_st->bit_mask & ICF_SET_SCALE_PRM_MAX_LINES)
    {
        icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_set_scale_params_req_st->max_lines), &offset);
        *p_length    += offset;
    }

    if(p_icf_set_scale_params_req_st->bit_mask & ICF_SET_SCALE_PRM_MAX_AUDIO_CALLS)
    {
        icf_pack_U16(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_set_scale_params_req_st->max_audio_calls), &offset);
        *p_length    += offset;
    }

    if(p_icf_set_scale_params_req_st->bit_mask & ICF_SET_SCALE_PRM_MAX_VIDEO_CALLS)
    {
        icf_pack_U16(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_set_scale_params_req_st->max_video_calls), &offset);
        *p_length    += offset;
    }

    if(p_icf_set_scale_params_req_st->bit_mask & ICF_SET_SCALE_PRM_MAX_SERVICE_CALLS)
    {
        icf_pack_U16(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_set_scale_params_req_st->max_service_calls), &offset);
        *p_length    += offset;
    }

    if(p_icf_set_scale_params_req_st->bit_mask & ICF_SET_SCALE_PRM_MAX_REGISTRATION_CALLS)
    {
        icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_set_scale_params_req_st->max_registration_calls), &offset);
        *p_length    += offset;
    }

    if(p_icf_set_scale_params_req_st->bit_mask & ICF_SET_SCALE_PRM_MAX_USERS_PER_LINE)
    {
        icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_set_scale_params_req_st->max_users_per_line), &offset);
        *p_length    += offset;
    }

    if(p_icf_set_scale_params_req_st->bit_mask & ICF_SET_SCALE_PRM_NUM_OF_CALLS_TO_LOG)
    {
        icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_set_scale_params_req_st->num_of_calls_to_log), &offset);
        *p_length    += offset;
    }

    if(p_icf_set_scale_params_req_st->bit_mask & ICF_SET_SCALE_PRM_MAX_NUM_OF_APPS)
    {
        icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_set_scale_params_req_st->max_num_of_apps), &offset);
        *p_length    += offset;
    }
    
    return ICF_SUCCESS;
}


icf_uint32_t
icf_pack_icf_req_uri_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t    *p_length
)
{

    icf_uint32_t        offset = 0;
    icf_uint32_t        count = 0;

    icf_req_uri_st    *p_icf_req_uri_st = ICF_NULL;
    
    p_icf_req_uri_st    = (icf_req_uri_st *)p_src;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function packs icf_string_st */
    ICF_CLIB_TRACE(("Packing icf_req_uri_st\n"));

    icf_pack_U16(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_req_uri_st->str_len), &offset);
    
    *p_length    += offset;

    if (0 >= p_icf_req_uri_st->str_len || p_icf_req_uri_st->str_len > ICF_CLIB_MAX_STR_LEN) {
        ICF_CLIB_TRACE(("\n[ICF_CLIB]: icf_pack_icf_req_uri_st - Invalid String Length (%d) Bytes\n", p_icf_req_uri_st->str_len));
        return ICF_FAILURE;
    } else {
        ICF_CLIB_TRACE(("\n[ICF_CLIB]: icf_pack_icf_req_uri_st - Packing String of Length (%d) Bytes\n", p_icf_req_uri_st->str_len));
    }

    /* This piece of code packs a variable length octet string */
    
    count = 0;
    /* RN Pack the NULL Character also*/
    while((icf_uint16_t)count < p_icf_req_uri_st->str_len + 1)
    {
        icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_req_uri_st->str[count]), &offset);
        
        *p_length    += offset;
        
        count++;
    }
    
    return ICF_SUCCESS;
}


icf_uint32_t
icf_pack_icf_string_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t    *p_length
)
{

    icf_uint32_t        offset = 0;
    icf_uint32_t        count = 0;

    icf_string_st    *p_icf_string_st = ICF_NULL;
    
    p_icf_string_st    = (icf_string_st *)p_src;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function packs icf_string_st */
    ICF_CLIB_TRACE(("Packing icf_string_st\n"));

    icf_pack_U16(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_string_st->str_len), &offset);
    
    *p_length    += offset;

    if (0 >= p_icf_string_st->str_len || p_icf_string_st->str_len > ICF_MAX_STR_LEN) {
        ICF_CLIB_TRACE(("\n[ICF_CLIB]: icf_pack_icf_string_st - Invalid String Length (%d) Bytes\n", p_icf_string_st->str_len));
        return ICF_FAILURE;
    } else {
        ICF_CLIB_TRACE(("\n[ICF_CLIB]: icf_pack_icf_string_st - Packing String of Length (%d) Bytes\n", p_icf_string_st->str_len));
   }

    /* This piece of code packs a variable length octet string */
    
    count = 0;
    /* RN Pack the NULL Character also*/
    while((icf_uint16_t)count < p_icf_string_st->str_len + 1)
    {
        icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_string_st->str[count]), &offset);
        
        *p_length    += offset;
        
        count++;
    }
    
    return ICF_SUCCESS;
}

icf_uint32_t
icf_pack_icf_sdf_string_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t    *p_length
)
{

    icf_uint32_t        offset = 0;
    icf_uint32_t        count = 0;

    icf_sdf_string_st    *p_icf_sdf_string_st = ICF_NULL;
    
    p_icf_sdf_string_st    = (icf_sdf_string_st *)p_src;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function packs icf_sdf_string_st */
    ICF_CLIB_TRACE(("Packing icf_sdf_string_st\n"));

    icf_pack_U16(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_sdf_string_st->dStrLen), &offset);
    
    *p_length    += offset;

    if ((0 >= p_icf_sdf_string_st->dStrLen) || (p_icf_sdf_string_st->dStrLen >SDF_MAXIMUM_STR_LEN )) {
        ICF_CLIB_TRACE(("\n[ICF_CLIB]: icf_pack_icf_sdf_string_st - Invalid String Length (%d) Bytes\n", p_icf_sdf_string_st->dStrLen));
        return ICF_FAILURE;
    } else {
        ICF_CLIB_TRACE(("\n[ICF_CLIB]: icf_pack_icf_sdf_string_st - Packing String of Length (%d) Bytes\n", p_icf_sdf_string_st->dStrLen));
   }

    /* This piece of code packs a variable length octet string */
    
    count = 0;
    /* RN Pack the NULL Character also*/
    while((icf_uint16_t)count < p_icf_sdf_string_st->dStrLen + 1)
    {
        icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_sdf_string_st->dStr[count]), &offset);
        
        *p_length    += offset;
        
        count++;
    }
    
    return ICF_SUCCESS;
}




icf_uint32_t
icf_pack_icf_large_string_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t    *p_length
)
{

    icf_uint32_t        offset = 0;
    icf_uint32_t        count = 0;

    icf_large_string_st    *p_icf_large_string_st = ICF_NULL;
    
    p_icf_large_string_st    = (icf_large_string_st *)p_src;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function packs icf_large_string_st */
    ICF_CLIB_TRACE(("Packing icf_large_string_st\n"));

    icf_pack_U16(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_large_string_st->str_len), &offset);
    
    *p_length    += offset;

    if (0 >= p_icf_large_string_st->str_len || p_icf_large_string_st->str_len > ICF_MAX_LARGE_STR_LEN) {
        ICF_CLIB_TRACE(("\n[ICF_CLIB]: icf_pack_icf_large_string_st - Invalid String Length (%d) Bytes\n", p_icf_large_string_st->str_len));
        return ICF_FAILURE;
    } else { 
        ICF_CLIB_TRACE(("\n[ICF_CLIB]: icf_pack_icf_large_string_st - Packing String of Length (%d) Bytes\n", p_icf_large_string_st->str_len));

    }
    /* This piece of code packs a variable length octet string */
    
    count = 0;
    /* Pack the NULL Character also */
    while((icf_uint16_t)count < p_icf_large_string_st->str_len + 1)
    {
        icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_large_string_st->str[count]), &offset);
        
        *p_length    += offset;
        
        count++;
    }
    
    return ICF_SUCCESS;
}


/* Added in ICF REL 6.0 to pack short string structure*/

icf_uint32_t
icf_pack_icf_short_string_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t    *p_length
)
{

    icf_uint32_t        offset = 0;
    icf_uint32_t        count = 0;

    icf_short_string_st    *p_icf_short_string_st = ICF_NULL;
    
    p_icf_short_string_st    = (icf_short_string_st *)p_src;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function packs icf_short_string_st */
    ICF_CLIB_TRACE(("Packing icf_short_string_st\n"));

    icf_pack_U16(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_short_string_st->str_len), &offset);
    
    *p_length    += offset;

    if (0 >= p_icf_short_string_st->str_len || p_icf_short_string_st->str_len > ICF_MAX_SHORT_STR_LEN) {
        ICF_CLIB_TRACE(("\n[ICF_CLIB]: icf_pack_icf_short_string_st - Invalid String Length (%d) Bytes\n", p_icf_short_string_st->str_len));
        return ICF_FAILURE;
    } else {
        ICF_CLIB_TRACE(("\n[ICF_CLIB]: icf_pack_icf_short_string_st - Packing String of Length (%d) Bytes\n", p_icf_short_string_st->str_len));
   }

    /* This piece of code packs a variable length octet string */
    
    count = 0;
    /* RN Pack the NULL Character also*/
    while((icf_uint16_t)count < p_icf_short_string_st->str_len + 1)
    {
        icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_short_string_st->str[count]), &offset);
        
        *p_length    += offset;
        
        count++;
    }
    
    return ICF_SUCCESS;
}

/* Added in ICF REL 6.0 to pack long string structure*/

icf_uint32_t
icf_pack_icf_long_string_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t    *p_length
)
{

    icf_uint32_t        offset = 0;
    icf_uint32_t        count = 0;

    icf_long_string_st    *p_icf_long_string_st = ICF_NULL;
    
    p_icf_long_string_st    = (icf_long_string_st *)p_src;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function packs icf_long_string_st */
    ICF_CLIB_TRACE(("Packing icf_long_string_st\n"));

    icf_pack_U16(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_long_string_st->str_len), &offset);
    
    *p_length    += offset;

    if (0 >= p_icf_long_string_st->str_len || p_icf_long_string_st->str_len > ICF_MAX_LONG_STR_LEN) {
        ICF_CLIB_TRACE(("\n[ICF_CLIB]: icf_pack_icf_long_string_st - Invalid String Length (%d) Bytes\n", p_icf_long_string_st->str_len));
        return ICF_FAILURE;
    } else { 
        ICF_CLIB_TRACE(("\n[ICF_CLIB]: icf_pack_icf_long_string_st - Packing String of Length (%d) Bytes\n", p_icf_long_string_st->str_len));
    }

    /* This piece of code packs a variable length octet string */
    
    count = 0;
    /* Pack the NULL Character also*/
    while((icf_uint16_t)count < p_icf_long_string_st->str_len + 1)
    {
        icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_long_string_st->str[count]), &offset);
        
        *p_length    += offset;
        
        count++;
    }
    
    return ICF_SUCCESS;
}

icf_uint32_t
icf_pack_icf_address_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t    *p_length
)
{
    icf_uint32_t        offset = 0;

    icf_address_st    *p_icf_address_st = ICF_NULL;
    
    p_icf_address_st    = (icf_address_st *)p_src;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function packs icf_address_st */
    ICF_CLIB_TRACE(("Packing icf_address_st\n"));
    
    icf_pack_icf_string_st(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_address_st->addr_val), &offset);
    
    *p_length    += offset;

    icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_address_st->addr_type), &offset);
    
    *p_length    += offset;


    return ICF_SUCCESS;
}



icf_uint32_t
icf_pack_icf_sdf_address_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t    *p_length
)
{
    icf_uint32_t        offset = 0;

    icf_sdf_address_st    *p_icf_sdf_address_st = ICF_NULL;
    
    p_icf_sdf_address_st    = (icf_sdf_address_st*)p_src;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function packs icf_sdf_address_st */
    ICF_CLIB_TRACE(("Packing icf_sdf_address_st\n"));

    icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_sdf_address_st->dAddrType),
            &offset);
    
    *p_length    += offset;

    
    icf_pack_icf_sdf_string_st(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_sdf_address_st->dAddrVal), &offset);
    
    *p_length    += offset;

    

    return ICF_SUCCESS;
}





icf_uint32_t
icf_pack_icf_config_self_addr_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t    *p_length
)
{
    icf_uint32_t        offset = 0;

    icf_config_self_addr_st    *p_icf_config_self_addr_st = ICF_NULL;
    
    p_icf_config_self_addr_st    = (icf_config_self_addr_st *)p_src;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function packs icf_config_self_addr_st */
    ICF_CLIB_TRACE(("Packing icf_config_self_addr_st\n"));
    
    icf_pack_icf_line_id_t(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_config_self_addr_st->line_id), &offset);
    
    *p_length    += offset;

    icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_config_self_addr_st->action), &offset);
    
    *p_length    += offset;

    icf_pack_icf_address_st(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_config_self_addr_st->addr), &offset);
    
    *p_length    += offset;

    return(ICF_SUCCESS);
}

icf_uint32_t
icf_pack_icf_config_self_addr_list_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t    *p_length
)
{
    icf_uint32_t        offset = 0;
    icf_uint32_t        count = 0;

    icf_config_self_addr_list_st    *p_icf_config_self_addr_list_st = ICF_NULL;
    
    p_icf_config_self_addr_list_st    = (icf_config_self_addr_list_st *)p_src;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function packs icf_config_self_addr_list_st */
    ICF_CLIB_TRACE(("Packing icf_config_self_addr_list_st\n"));
    
    icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_config_self_addr_list_st->all_lines), &offset);
    
    *p_length    += offset;

    icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_config_self_addr_list_st->num_of_elements), &offset);
    
    *p_length    += offset;

    /* This piece of code packs a variable length octet string */
    
    count = 0;
    while(count < p_icf_config_self_addr_list_st->num_of_elements)
    {
        icf_pack_icf_config_self_addr_st(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_config_self_addr_list_st->self_addr[count]), &offset);
        
        *p_length    += offset;
        
        count++;
    }
    return(ICF_SUCCESS);
}

icf_uint32_t
icf_pack_icf_config_self_name_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t    *p_length
)
{
    icf_uint32_t        offset = 0;

    icf_config_self_name_st    *p_icf_config_self_name_st = ICF_NULL;
    
    p_icf_config_self_name_st    = (icf_config_self_name_st *)p_src;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function packs icf_config_self_name_st */
    ICF_CLIB_TRACE(("Packing icf_config_self_name_st\n"));
    
    icf_pack_icf_line_id_t(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_config_self_name_st->line_id), &offset);
    
    *p_length    += offset;

    icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_config_self_name_st->action), &offset);
    
    *p_length    += offset;

    icf_pack_icf_string_st(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_config_self_name_st->name), &offset);
    
    *p_length    += offset;

    return(ICF_SUCCESS);
}

icf_uint32_t
icf_pack_icf_config_self_name_list_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t    *p_length
)
{
    icf_uint32_t        offset = 0;
    icf_uint32_t        count = 0;

    icf_config_self_name_list_st    *p_icf_config_self_name_list_st = ICF_NULL;
    
    p_icf_config_self_name_list_st    = (icf_config_self_name_list_st *)p_src;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function packs icf_config_self_name_list_st */
    ICF_CLIB_TRACE(("Packing icf_config_self_name_list_st\n"));
    
    icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_config_self_name_list_st->all_lines), &offset);
    
    *p_length    += offset;

    icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_config_self_name_list_st->num_of_elements), &offset);
    
    *p_length    += offset;

    /* This piece of code packs a variable length octet string */
    
    count = 0;
    while(count < p_icf_config_self_name_list_st->num_of_elements)
    {
        icf_pack_icf_config_self_name_st(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_config_self_name_list_st->self_name[count]), &offset);
        
        *p_length    += offset;
        
        count++;
    }
    return(ICF_SUCCESS);
}

icf_uint32_t
icf_pack_icf_config_default_settings_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t    *p_length
)
{
    icf_uint32_t        offset = 0;

    icf_config_default_settings_st    *p_icf_config_default_settings_st = ICF_NULL;
    
    p_icf_config_default_settings_st    = (icf_config_default_settings_st *)p_src;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function packs icf_config_default_settings_st */
    ICF_CLIB_TRACE(("Packing icf_config_default_settings_st\n"));
    
    icf_pack_icf_line_id_t(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_config_default_settings_st->line_id), &offset);
    
    *p_length    += offset;

    icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_config_default_settings_st->default_settings), &offset);
    
    *p_length    += offset;

    return(ICF_SUCCESS);
}

icf_uint32_t
icf_pack_icf_config_default_settings_list_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t    *p_length
)
{
    icf_uint32_t        offset = 0;
    icf_uint32_t        count = 0;

    icf_config_default_settings_list_st    *p_icf_config_default_settings_list_st = ICF_NULL;
    
    p_icf_config_default_settings_list_st    = (icf_config_default_settings_list_st *)p_src;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function packs icf_config_default_settings_list_st */
    ICF_CLIB_TRACE(("Packing icf_config_default_settings_list_st\n"));
    
    icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_config_default_settings_list_st->all_lines), &offset);
    
    *p_length    += offset;

    icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_config_default_settings_list_st->num_of_elements), &offset);
    
    *p_length    += offset;

    /* This piece of code packs a variable length octet string */
    
    count = 0;
    while(count < p_icf_config_default_settings_list_st->num_of_elements)
    {
        icf_pack_icf_config_default_settings_st(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_config_default_settings_list_st->default_settings[count]), &offset);
        
        *p_length    += offset;
        
        count++;
    }
    return(ICF_SUCCESS);
}


icf_uint32_t
icf_pack_icf_config_status_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t    *p_length
)
{
    icf_uint32_t        offset = 0;

    icf_config_status_st    *p_icf_config_status_st = ICF_NULL;
    
    p_icf_config_status_st    = (icf_config_status_st *)p_src;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function packs icf_config_status_st */
    ICF_CLIB_TRACE(("Packing icf_config_status_st\n"));
    
    icf_pack_icf_line_id_t(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_config_status_st->line_id), &offset);
    
    *p_length    += offset;

    icf_pack_U16(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_config_status_st->status), &offset);
    
    *p_length    += offset;

    return(ICF_SUCCESS);
}

icf_uint32_t
icf_pack_icf_config_status_list_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t    *p_length
)
{
    icf_uint32_t        offset = 0;
    icf_uint32_t        count = 0;

    icf_config_status_list_st    *p_icf_config_status_list_st = ICF_NULL;
    
    p_icf_config_status_list_st    = (icf_config_status_list_st *)p_src;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function packs icf_config_status_list_st */
    ICF_CLIB_TRACE(("Packing icf_config_status_list_st\n"));
    
    icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_config_status_list_st->all_lines), &offset);
    
    *p_length    += offset;

    icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_config_status_list_st->num_of_elements), &offset);
    
    *p_length    += offset;

    /* This piece of code packs a variable length octet string */
    
    count = 0;
    while(count < p_icf_config_status_list_st->num_of_elements)
    {
        icf_pack_icf_config_status_st(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_config_status_list_st->status[count]), &offset);
        
        *p_length    += offset;
        
        count++;
    }
    return(ICF_SUCCESS);
}

icf_uint32_t
icf_pack_icf_config_address_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t    *p_length
)
{
    icf_uint32_t        offset = 0;

    icf_config_address_st    *p_icf_config_address_st = ICF_NULL;
    
    p_icf_config_address_st    = (icf_config_address_st *)p_src;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function packs icf_config_address_st */
    ICF_CLIB_TRACE(("Packing icf_config_address_st\n"));
    
    icf_pack_icf_line_id_t(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_config_address_st->line_id), &offset);
    
    *p_length    += offset;

    icf_pack_icf_address_st(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_config_address_st->address), &offset);
    
    *p_length    += offset;

    return(ICF_SUCCESS);
}

icf_uint32_t
icf_pack_icf_config_address_list_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t    *p_length
)
{
    icf_uint32_t        offset = 0;
    icf_uint32_t        count = 0;

    icf_config_address_list_st    *p_icf_config_address_list_st = ICF_NULL;
    
    p_icf_config_address_list_st    = (icf_config_address_list_st *)p_src;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function packs icf_config_address_list_st */
    ICF_CLIB_TRACE(("Packing icf_config_address_list_st\n"));
    
    icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_config_address_list_st->all_lines), &offset);
    
    *p_length    += offset;

    icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_config_address_list_st->num_of_elements), &offset);
    
    *p_length    += offset;

    /* This piece of code packs a variable length octet string */
    
    count = 0;
    while(count < p_icf_config_address_list_st->num_of_elements)
    {
        icf_pack_icf_config_address_st(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_config_address_list_st->address[count]), &offset);
        
        *p_length    += offset;
        
        count++;
    }
    return(ICF_SUCCESS);
}



icf_uint32_t
icf_pack_icf_ipv4_int_address_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t    *p_length
)
{
    icf_uint32_t        offset = 0;

    icf_ipv4_int_address_st    *p_icf_ipv4_int_address_st = ICF_NULL;
    
    p_icf_ipv4_int_address_st    = (icf_ipv4_int_address_st *)p_src;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function packs icf_ipv4_int_address_st */
    ICF_CLIB_TRACE(("Packing icf_ipv4_int_address_st\n"));
    
    icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_ipv4_int_address_st->octet_1), &offset);
    
    *p_length    += offset;

    icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_ipv4_int_address_st->octet_2), &offset);
    
    *p_length    += offset;

    icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_ipv4_int_address_st->octet_3), &offset);
    
    *p_length    += offset;

    icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_ipv4_int_address_st->octet_4), &offset);
    
    *p_length    += offset;

    return ICF_SUCCESS;
}

icf_uint32_t
icf_pack_icf_transport_addr_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t    *p_length
)
{
    icf_uint32_t        offset = 0;

    icf_transport_addr_st    *p_icf_transport_addr_st = ICF_NULL;
    
    p_icf_transport_addr_st    = (icf_transport_addr_st *)p_src;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function packs icf_transport_addr_st */
    ICF_CLIB_TRACE(("Packing icf_transport_addr_st\n"));
    
    icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_transport_addr_st->addr_type), &offset);
    
    *p_length    += offset;

    switch(p_icf_transport_addr_st->addr_type)
    {
        case ICF_TRANSPORT_ADDRESS_DOMAIN_NAME:

            icf_pack_icf_string_st(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_transport_addr_st->addr.domain), &offset);

            *p_length    += offset;
        
            break;
        
        case ICF_TRANSPORT_ADDRESS_IPV4_ADDR:

            icf_pack_icf_ipv4_int_address_st(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_transport_addr_st->addr.ipv4_addr), &offset);

            *p_length    += offset;
        
            break;
#ifdef ICF_IPV6_ENABLED
        case ICF_TRANSPORT_ADDRESS_IPV6_ADDR:
            icf_pack_icf_string_st(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_transport_addr_st->addr.ipv6_addr), &offset);
            *p_length += offset;
            break;
#endif

        
    }

    return ICF_SUCCESS;
}

icf_uint32_t
icf_pack_icf_transport_address_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t    *p_length
)
{
    icf_uint32_t        offset = 0;

    icf_transport_address_st    *p_icf_transport_address_st = ICF_NULL;
    
    p_icf_transport_address_st    = (icf_transport_address_st *)p_src;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function packs icf_transport_address_st */
    ICF_CLIB_TRACE(("Packing icf_transport_address_st\n"));
    
    icf_pack_icf_transport_addr_st(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_transport_address_st->addr), &offset);
    
    *p_length    += offset;

    icf_pack_U16(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_transport_address_st->port_num), &offset);
    
    *p_length    += offset;

    return(ICF_SUCCESS);
}


#ifdef ICF_NW_ACTIVATE
icf_uint32_t
icf_pack_icf_network_activate_req_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t    *p_length
)
{
    icf_uint32_t        offset = 0;

    icf_network_activate_req_st    *p_icf_network_activate_req_st = ICF_NULL;
    
    p_icf_network_activate_req_st    = (icf_network_activate_req_st *)p_src;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function packs icf_network_activate_req_st */
    ICF_CLIB_TRACE(("Packing icf_network_activate_req_st\n"));

    icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_network_activate_req_st->access_type), &offset);
    
    *p_length    += offset;

    icf_pack_icf_string_st(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_network_activate_req_st->access_info), &offset);
    
    *p_length    += offset;

    icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_network_activate_req_st->bit_mask), &offset);
    
    *p_length    += offset;

    if (ICF_NET_ACT_REQ_ROUTE_INFO_PRESENT & p_icf_network_activate_req_st->bit_mask)
    {  
        icf_pack_icf_void_st(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_network_activate_req_st->routing_info), &offset);
        
        *p_length    += offset;
    }

    return ICF_SUCCESS;
}
#endif

icf_uint32_t
icf_pack_icf_void_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t    *p_length
)
{
    icf_uint32_t        offset = 0;
    icf_uint32_t        count = 0;

    icf_void_st    *p_icf_void_st = ICF_NULL;

    p_icf_void_st    = (icf_void_st *)p_src;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function packs icf_void_st */
    ICF_CLIB_TRACE(("Packing icf_void_st\n"));
    
    icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_void_st->len), &offset);
    
    *p_length    += offset;

    /* This piece of code packs a variable length octet string */

    if (0 >= p_icf_void_st->len  || p_icf_void_st->len > ICF_CLIB_MAX_API_LENGTH) {
        ICF_CLIB_TRACE(("\n[ICF_CLIB]: icf_pack_icf_void_st - Invalid Structure Length (%d) Bytes\n", p_icf_void_st->len));
        return ICF_FAILURE;
    }
    
    ICF_CLIB_TRACE(("\n[ICF_CLIB]: icf_pack_icf_void_st - Packing Structure of Length (%d) Bytes\n", p_icf_void_st->len));

    count = 0;
    while(count < p_icf_void_st->len)
    {
        icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_void_st->ptr[count]), &offset);
        
        *p_length    += offset;
        
        count++;
    }
            
    return ICF_SUCCESS;
}


#ifdef ICF_NW_ACTIVATE
icf_uint32_t
icf_pack_icf_event_indication_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t    *p_length
)
{
    icf_uint32_t        offset = 0;

    icf_event_indication_st    *p_icf_event_indication_st = ICF_NULL;
    
    p_icf_event_indication_st    = (icf_event_indication_st *)p_src;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function packs icf_event_indication_st */
    ICF_CLIB_TRACE(("Packing icf_event_indication_st\n"));
    
    icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_event_indication_st->event_code), &offset);
    
    *p_length    += offset;

    icf_pack_icf_void_st(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_event_indication_st->event_data), &offset);
    
    *p_length    += offset;

    return ICF_SUCCESS;
}



icf_uint32_t
icf_pack_icf_network_activate_cfm_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t    *p_length
)
{
    icf_uint32_t        offset = 0;
    icf_uint32_t        count = 0;

    icf_network_activate_cfm_st    *p_icf_network_activate_cfm_st = ICF_NULL;
    
    p_icf_network_activate_cfm_st    = (icf_network_activate_cfm_st *)p_src;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function packs icf_network_activate_cfm_st */
    ICF_CLIB_TRACE(("Packing icf_network_activate_cfm_st\n"));
    
    icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_network_activate_cfm_st->bit_mask), &offset);
    
    *p_length    += offset;

    icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_network_activate_cfm_st->result), &offset);
    
    *p_length    += offset;

    if(p_icf_network_activate_cfm_st->bit_mask & ICF_NET_ACT_CFM_ERR_CAUSE_PRESENT)
    {
        icf_pack_U16(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_network_activate_cfm_st->error_cause), &offset);
        
        *p_length    += offset;
    }

    if(p_icf_network_activate_cfm_st->bit_mask & ICF_NET_ACT_CFM_NUM_NW_PRESENT)
    {
        icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_network_activate_cfm_st->num_nw), &offset);
        
        *p_length    += offset;
    }

    if(p_icf_network_activate_cfm_st->bit_mask & ICF_NET_ACT_CFM_NW_INFO_PRESENT)
    {
    /* This piece of code packs a variable length octet string */
    
    count = 0;
    while(count < p_icf_network_activate_cfm_st->num_nw)
    {
        icf_pack_icf_network_info_st(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_network_activate_cfm_st->nw_info[count]), &offset);
        
        *p_length    += offset;
        
        count++;
    }
    }

    return ICF_SUCCESS;
}
#endif/*#ifdef ICF_NW_ACTIVATE*/


icf_uint32_t
icf_pack_icf_apn_list_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t    *p_length
)
{
    icf_uint32_t        offset = 0;
    icf_uint32_t        count = 0;

    icf_apn_list_st    *p_icf_apn_list_st = ICF_NULL;
    
    p_icf_apn_list_st    = (icf_apn_list_st *)p_src;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function packs icf_apn_list_st */
    ICF_CLIB_TRACE(("Packing icf_apn_list_st\n"));

    
    icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_apn_list_st->count), &offset);
    
    *p_length    += offset;
    
    /* This piece of code packs a variable length octet string */
    count = 0;
    while(count < p_icf_apn_list_st->count)
    {
        offset = 0;
        if( ICF_NULL != p_icf_apn_list_st->p_list_of_apn )
        {
            icf_pack_icf_string_st(p_dest + (*p_length), 
                (icf_uint8_t*)&(p_icf_apn_list_st->p_list_of_apn[count]), &offset);
        }
        
        *p_length    += offset;
        
        count++;
    }

    return ICF_SUCCESS;
}


icf_uint32_t
icf_pack_icf_pub_uri_list_st
(
    icf_uint8_t    *p_dest,
    icf_pub_uri_list_node_st    *p_icf_uri_list_st,
    icf_uint32_t    *p_length
)
{
    icf_uint32_t        offset = 0;
    /*ZyXEL porting*/
#if 0
    icf_uint32_t        count = 0;
#else
    icf_uint8_t        count = 0;
#endif
    /*ZyXEL porting end*/

    icf_pub_uri_list_node_st    *p_curr_node = ICF_NULL;
    
    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function packs icf_uri_list_st */
    ICF_CLIB_TRACE(("Packing icf_pub_uri_list_node_st \n"));
	
    
    icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&count, &offset);
    
    *p_length    += offset;

    /* This piece of code packs a variable length octet string */
    
    for(p_curr_node = p_icf_uri_list_st;
    p_curr_node!=ICF_NULL;
    p_curr_node = (icf_pub_uri_list_node_st *)p_curr_node->p_next)
    {
        icf_pack_icf_address_st(p_dest + (*p_length), (icf_uint8_t*)&(p_curr_node->pub_uri), &offset);
        *p_length    += offset;
        
        icf_pack_icf_address_list_node_st(p_dest + (*p_length), p_curr_node->p_assoc_uri_list, &offset);
        *p_length    += offset;
        
        count++;
    }
    /* Pack the count */
    icf_pack_U8(p_dest, (icf_uint8_t*)&count, &offset);
    return ICF_SUCCESS;
}




icf_uint32_t
icf_pack_icf_address_list_node_st
(
    icf_uint8_t    *p_dest,
    icf_address_list_node_st    *p_icf_address_list_node_st,
    icf_uint32_t    *p_length
)
{
    icf_uint32_t        offset = 0;
    /*ZyXEL porting*/
#if 0
    icf_uint32_t        count = 0;
#else
    icf_uint8_t        count = 0;
#endif
    /*ZyXEL porting end*/

    icf_address_list_node_st    *p_curr_node = ICF_NULL;
    
    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function packs icf_uri_list_st */
    ICF_CLIB_TRACE(("Packing icf_address_list_node_st \n"));
    
	icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&count, &offset);
    
    *p_length    += offset;

    /* This piece of code packs a variable length octet string */
    
    for(p_curr_node = p_icf_address_list_node_st;
    p_curr_node!=ICF_NULL;
    p_curr_node = (icf_address_list_node_st *)p_curr_node->p_next)
    {
        icf_pack_icf_address_st(p_dest + (*p_length), (icf_uint8_t*)&(p_curr_node->user_addr), &offset);
        *p_length    += offset;

        count++;
    }
	if(count)
	{
		
		/* Pack the count */
		icf_pack_U8(p_dest, (icf_uint8_t*)&count, &offset);
	}
    return ICF_SUCCESS;
}

icf_uint32_t
icf_pack_icf_transport_list_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t    *p_length
)
{
    icf_uint32_t        offset = 0;
    icf_uint32_t        count = 0;

    icf_transport_list_st    *p_icf_transport_list_st = ICF_NULL;
    
    p_icf_transport_list_st    = (icf_transport_list_st *)p_src;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function packs icf_transport_list_st */
    ICF_CLIB_TRACE(("Packing icf_transport_list_st\n"));
    
    icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_transport_list_st->num_of_elements), &offset);
    
    *p_length    += offset;

    /* This piece of code packs a variable length octet string */
    
    count = 0;
    while(count < p_icf_transport_list_st->num_of_elements)
    {
        icf_pack_icf_transport_addr_st(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_transport_list_st->addr[count]), &offset);
        
        *p_length    += offset;
        
        count++;
    }
    return ICF_SUCCESS;

}

#ifdef ICF_NW_ACTIVATE
icf_uint32_t
icf_pack_icf_network_info_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t    *p_length
)
{


    icf_uint32_t        offset = 0;

    icf_network_info_st    *p_icf_network_info_st = ICF_NULL;
    
    p_icf_network_info_st    = (icf_network_info_st *)p_src;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function packs icf_network_info_st */
    ICF_CLIB_TRACE(("Packing icf_network_info_st\n"));
    
    icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_network_info_st->bit_mask), &offset);
    
    *p_length    += offset;

    if(p_icf_network_info_st->bit_mask & ICF_NET_INFO_CTXT_ID_PRESENT)
    {
        icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_network_info_st->ctxt_id), &offset);
        
        *p_length    += offset;
    }

    if(p_icf_network_info_st->bit_mask & ICF_NET_INFO_SELF_IP_PRESENT)
    {
        icf_pack_icf_transport_addr_st(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_network_info_st->self_ip), &offset);
        
        *p_length    += offset;
    }

    if(p_icf_network_info_st->bit_mask & ICF_NET_INFO_ACCESS_TYPE_PRESENT)
    {
        icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_network_info_st->access_type), &offset);
        
        *p_length    += offset;
    }

    if(p_icf_network_info_st->bit_mask & ICF_NET_INFO_ACCESS_INFO_PRESENT)
    {
        icf_pack_icf_string_st(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_network_info_st->access_info), &offset);
        
        *p_length    += offset;
    }

    if(p_icf_network_info_st->bit_mask & ICF_NET_INFO_ASSO_URI_PRESENT)
    {
         icf_pack_icf_pub_uri_list_st(p_dest + (*p_length), p_icf_network_info_st->p_uri_list, &offset);
 
		 *p_length    += offset;
    }

    if(p_icf_network_info_st->bit_mask & ICF_NET_INFO_OUT_PROXY_ADDR_PRESENT)
    {
        icf_pack_icf_transport_list_st(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_network_info_st->out_proxy_addr), &offset);
        
        *p_length    += offset;
    }

    return ICF_SUCCESS;
}




icf_uint32_t
icf_pack_icf_network_deactivate_req_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t    *p_length
)
{
    icf_uint32_t        offset = 0;

    icf_network_deactivate_req_st    *p_icf_network_deactivate_req = ICF_NULL;
    
    p_icf_network_deactivate_req    = (icf_network_deactivate_req_st *)p_src;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function packs icf_network_deactivate_req */
    ICF_CLIB_TRACE(("Packing icf_network_deactivate_req\n"));

    icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_network_deactivate_req->ctxt_id), &offset);
    
    *p_length    += offset;

    icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_network_deactivate_req->bit_mask), &offset);
    
    *p_length    += offset;
    
    if (ICF_NET_ACT_REQ_ROUTE_INFO_PRESENT & p_icf_network_deactivate_req->bit_mask)
    {    
        icf_pack_icf_void_st(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_network_deactivate_req->routing_info), &offset);
        
        *p_length    += offset;
    }

    return ICF_SUCCESS;
}

icf_uint32_t
icf_pack_icf_network_deactivate_cfm_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t    *p_length
)
{
    icf_uint32_t        offset = 0;
    icf_uint32_t        count = 0;

    icf_network_deactivate_cfm_st    *p_icf_network_deactivate_cfm_st = ICF_NULL;
    
    p_icf_network_deactivate_cfm_st    = (icf_network_deactivate_cfm_st *)p_src;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function packs icf_network_deactivate_cfm_st */
    ICF_CLIB_TRACE(("Packing icf_network_deactivate_cfm_st\n"));
    
    icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_network_deactivate_cfm_st->bit_mask), &offset);
    
    *p_length    += offset;

    icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_network_deactivate_cfm_st->result), &offset);
    
    *p_length    += offset;


    if(p_icf_network_deactivate_cfm_st->bit_mask & ICF_NET_DEACT_CFM_ERR_CAUSE_PRESENT)
    {
        icf_pack_U16(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_network_deactivate_cfm_st->error_cause), &offset);
        
        *p_length    += offset;
    }

    if(p_icf_network_deactivate_cfm_st->bit_mask & ICF_NET_DEACT_CFM_NUM_NW_PRESENT)
    {
        icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_network_deactivate_cfm_st->num_nw), &offset);
        
        *p_length    += offset;
    }

    if(p_icf_network_deactivate_cfm_st->bit_mask & ICF_NET_DEACT_CFM_NW_INFO_PRESENT)
    {
    /* This piece of code packs a variable length octet string */
    
    count = 0;
    while(count < p_icf_network_deactivate_cfm_st->num_nw)
    {
        icf_pack_icf_network_info_st(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_network_deactivate_cfm_st->nw_info[count]), &offset);
        
        *p_length    += offset;
        
        count++;
    }
    }

    return ICF_SUCCESS;
}
#endif/*#ifdef ICF_NW_ACTIVATE*/

icf_uint32_t icf_pack_icf_filter_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t    *p_length
)
{
    icf_uint32_t        offset = 0;

    icf_filter_st       *p_icf_filter_st=(icf_filter_st *)p_src;

       *p_length    = 0;

    /* This function packs icf_header_name_list_st */
    ICF_CLIB_TRACE(("Packing icf_filter_st\n"));
    /* filter_type */
    icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_filter_st->filter_type), &offset);
    *p_length    += offset;
    /* match_type */
    icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_filter_st->match_type), &offset);
    *p_length    += offset;
    /*target_header*/
    icf_pack_icf_string_st(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_filter_st->target_header), &offset);
    *p_length    += offset;
    /*expr_str*/
    icf_pack_icf_string_st(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_filter_st->expr_str), &offset);
    *p_length    += offset;

    return ICF_SUCCESS;
}

icf_uint32_t
icf_pack_icf_filter_list_st
(
    icf_uint8_t     *p_dest,
    icf_uint8_t     *p_src,
    icf_uint32_t *p_length
)
{
    icf_uint32_t        offset = 0;
    icf_uint32_t        filter_list_count_offset=0; 

    icf_filter_st       *p_list_ptr;
    icf_filter_list_st  *p_icf_filter_list_st = (icf_filter_list_st *)p_src;
    icf_uint32_t        i;
    

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function packs icf_header_name_list_st */
    ICF_CLIB_TRACE(("Packing icf_filter_list_st\n"));
    
    filter_list_count_offset = *p_length;
    icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_filter_list_st->count), &offset);
    
    *p_length    += offset;

    icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_filter_list_st->match_all), &offset);

    *p_length    += offset;
    
    for(i=0,p_list_ptr = p_icf_filter_list_st->p_filter_list;
        ((i < p_icf_filter_list_st->count) && (ICF_NULL != p_list_ptr));
        i++,p_list_ptr = p_list_ptr->p_next)
    {
        
        icf_pack_icf_filter_st(p_dest + (*p_length), (icf_uint8_t*)p_list_ptr, &offset);
        *p_length    += offset;
    }
    /* Update the count based on the actual nodes in the linked list */
    icf_pack_U32(p_dest + filter_list_count_offset, (icf_uint8_t*)&i, &offset);

    return ICF_SUCCESS;
}

icf_uint32_t
icf_pack_icf_get_line_settings_req_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t    *p_length
)
{
    icf_uint32_t        offset = 0;

    icf_get_line_settings_req_st    *p_icf_get_line_settings_req_st = ICF_NULL;
    
    p_icf_get_line_settings_req_st    = (icf_get_line_settings_req_st *)p_src;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function packs icf_get_line_settings_req_st */
    ICF_CLIB_TRACE(("Packing icf_get_line_settings_req_st\n"));

    icf_pack_U32(p_dest + (*p_length), (icf_uint8_t *)&(p_icf_get_line_settings_req_st->bit_mask), &offset);
    
    *p_length    += offset;
    
    icf_pack_icf_line_id_t(p_dest + (*p_length), (icf_uint8_t *)&(p_icf_get_line_settings_req_st->line_id), &offset);
    
    *p_length    += offset;

    /* FIX for spr 17944 */
    if(ICF_GET_SETTINGS_ROUTE_INFO_PRESENT & p_icf_get_line_settings_req_st->bit_mask)
    {
	    icf_pack_icf_void_st(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_get_line_settings_req_st->routing_info), &offset);

	    *p_length    += offset;
    }

    return ICF_SUCCESS;
}

icf_uint32_t
icf_pack_icf_get_line_settings_resp_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t    *p_length
)
{
    icf_uint32_t        offset = 0;
    
    icf_get_line_settings_resp_st    *p_icf_get_line_settings_resp_st = ICF_NULL;
    
    p_icf_get_line_settings_resp_st    = (icf_get_line_settings_resp_st *)p_src;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function packs icf_get_line_settings_resp_st */
    ICF_CLIB_TRACE(("Packing icf_get_line_settings_resp_st\n"));

    icf_pack_U32(p_dest + (*p_length), (icf_uint8_t *)&(p_icf_get_line_settings_resp_st->bit_mask), &offset);
    
    *p_length    += offset;

    icf_pack_icf_transport_address_st(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_get_line_settings_resp_st->proxy_address), &offset);
        
    *p_length    += offset;

    icf_pack_icf_transport_address_st(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_get_line_settings_resp_st->registrar_address), &offset);
        
    *p_length    += offset;

    icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_get_line_settings_resp_st->proxy_transport_mode), &offset);
    
    *p_length    += offset;
    
    icf_pack_icf_address_st(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_get_line_settings_resp_st->self_address_list), &offset);

    *p_length    += offset;

    icf_pack_icf_string_st(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_get_line_settings_resp_st->self_name), &offset);
        
    *p_length    += offset;

    icf_pack_U8(p_dest + (*p_length),(icf_uint8_t *)&(p_icf_get_line_settings_resp_st->presentation_allowed), &offset);

    *p_length    += offset;

    icf_pack_icf_string_st(p_dest + (*p_length),(icf_uint8_t *)&(p_icf_get_line_settings_resp_st->auth_key), &offset);

    *p_length    += offset;

    icf_pack_icf_string_st(p_dest + (*p_length),(icf_uint8_t *)&(p_icf_get_line_settings_resp_st->auth_password), &offset);

    *p_length    += offset;

    icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_get_line_settings_resp_st->reg_mode), &offset);
    
    *p_length    += offset;

    icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_get_line_settings_resp_st->reg_duration), &offset);
    
    *p_length    += offset;

	icf_pack_icf_transport_address_st(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_get_line_settings_resp_st->reg_route_addr), &offset);
        
    *p_length    += offset;

       icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&
                      (p_icf_get_line_settings_resp_st->sip_T1_timer),
                                                            &offset);

       *p_length    += offset;

       icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&
                      (p_icf_get_line_settings_resp_st->sip_B_timer), 
                                                            &offset);

       *p_length    += offset;


       icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&
                      (p_icf_get_line_settings_resp_st->sip_F_timer), 
                                                            &offset);

       *p_length    += offset;

       icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&
                      (p_icf_get_line_settings_resp_st->sip_T2_timer),
                                                            &offset);

       *p_length    += offset;

       icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&
                      (p_icf_get_line_settings_resp_st->sip_T4_timer),
                                                            &offset);

       *p_length    += offset;


    return ICF_SUCCESS;
}

icf_uint32_t
icf_pack_icf_get_sys_settings_req_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t    *p_length
)
{
    icf_uint32_t        offset = 0;

    icf_get_system_settings_req_st    *p_icf_get_sys_settings_req_st = ICF_NULL;
    
    p_icf_get_sys_settings_req_st    = (icf_get_system_settings_req_st *)p_src;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function packs icf_get_system_settings_req_st */
    ICF_CLIB_TRACE(("Packing icf_get_system_settings_req_st\n"));


    icf_pack_U32(p_dest + (*p_length), (icf_uint8_t *)&(p_icf_get_sys_settings_req_st->bit_mask), &offset);
    
    *p_length    += offset;

    icf_pack_icf_void_st(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_get_sys_settings_req_st->routing_info), &offset);
    
    *p_length    += offset;

    return ICF_SUCCESS;
}

icf_uint32_t
icf_pack_icf_get_sys_settings_resp_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t    *p_length
)
{
    icf_uint32_t        offset = 0;
    
    icf_get_system_settings_resp_st    *p_icf_get_sys_settings_resp_st = ICF_NULL;
    
    p_icf_get_sys_settings_resp_st    = (icf_get_system_settings_resp_st *)p_src;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function packs icf_get_system_settings_resp_st */
    ICF_CLIB_TRACE(("Packing icf_get_system_settings_resp_st\n"));

    icf_pack_U32(p_dest + (*p_length), (icf_uint8_t *)&(p_icf_get_sys_settings_resp_st->bit_mask), &offset);
    
    *p_length    += offset;

    icf_pack_U8(p_dest + (*p_length), (icf_uint8_t *)&(p_icf_get_sys_settings_resp_st->prack_enabled), &offset);
    
    *p_length    += offset;

    icf_pack_U8(p_dest + (*p_length), (icf_uint8_t *)&(p_icf_get_sys_settings_resp_st->privacy_supported), &offset);
    
    *p_length    += offset;

    icf_pack_U16(p_dest + (*p_length), (icf_uint8_t *)&(p_icf_get_sys_settings_resp_st->max_lines), &offset);
    
    *p_length    += offset;

    icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_get_sys_settings_resp_st->setup_timer), &offset);
        
    *p_length    += offset;

    icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_get_sys_settings_resp_st->alerting_timer), &offset);
        
    *p_length    += offset;

    icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_get_sys_settings_resp_st->release_timer), &offset);
        
    *p_length    += offset;

    icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_get_sys_settings_resp_st->modify_media_timer), &offset);
        
    *p_length    += offset;

    icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_get_sys_settings_resp_st->registration_resp_timer), &offset);
        
    *p_length    += offset;

    icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_get_sys_settings_resp_st->registration_retry_timer), &offset);
        
    *p_length    += offset;

    icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_get_sys_settings_resp_st->subs_retry_timer), &offset);
        
    *p_length    += offset;

    icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_get_sys_settings_resp_st->subs_resp_timer), &offset);
        
    *p_length    += offset;

    icf_pack_U8(p_dest + (*p_length), (icf_uint8_t *)&(p_icf_get_sys_settings_resp_st->sig_comp_enabled), &offset);
    
    *p_length    += offset;

    icf_pack_U8(p_dest + (*p_length), (icf_uint8_t *)&(p_icf_get_sys_settings_resp_st->security_enabled), &offset);
    
    *p_length    += offset;
    
    icf_pack_icf_transport_address_st(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_get_sys_settings_resp_st->self_ip_address), &offset);
        
    *p_length    += offset;

    icf_pack_icf_pub_uri_list_st(p_dest + (*p_length), p_icf_get_sys_settings_resp_st->p_uri_list, &offset);
        
    *p_length    += offset;

    icf_pack_icf_apn_list_st(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_get_sys_settings_resp_st->apn_list), &offset);

    *p_length    += offset;

    icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_get_sys_settings_resp_st->self_transport_mode), &offset);
    
    *p_length    += offset;

    icf_pack_U16(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_get_sys_settings_resp_st->self_port), &offset);
    
    *p_length    += offset;

    icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_get_sys_settings_resp_st->call_thru_proxy), &offset);
    
    *p_length    += offset;

    icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_get_sys_settings_resp_st->sip_compact_header), &offset);
    
    *p_length    += offset;

    icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_get_sys_settings_resp_st->session_interval_timer), &offset);
    
    *p_length    += offset;

    icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_get_sys_settings_resp_st->min_session_interval_timer), &offset);
    
    *p_length    += offset;

#ifdef ICF_NAT_RPORT_SUPPORT

   icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_get_sys_settings_resp_st->binding_query_enable), &offset);

    *p_length    += offset;


   icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_get_sys_settings_resp_st->binding_refresh_timer), &offset);

    *p_length    += offset;

#endif   

    return ICF_SUCCESS;
}

icf_uint32_t
icf_pack_icf_string_list_st
(
	icf_uint8_t	*p_dest,
	icf_uint8_t	*p_src,
	icf_uint32_t	*p_length
)
{
	icf_uint32_t		offset = 0;
	icf_uint32_t		count_offset = 0;
    icf_uint32_t		i;


	icf_string_list_st	*p_icf_string_list_st = ICF_NULL;
    icf_string_list_node_st  *p_cur_node = ICF_NULL;
	
	p_icf_string_list_st	= (icf_string_list_st *)p_src;

	/* Initialise length to 0 */
	*p_length	= 0;

	/* This function packs icf_header_name_list_st */
	ICF_CLIB_TRACE(("Packing icf_string_list_st\n"));
	
	icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_string_list_st->count), &offset);

    count_offset = *p_length;
	*p_length	+= offset;

    for(i=0,p_cur_node = p_icf_string_list_st->p_list;
    (i<p_icf_string_list_st->count) && (ICF_NULL != p_cur_node);
    i++,p_cur_node = p_cur_node->p_next)
    {
        icf_pack_icf_string_st(p_dest + (*p_length), (icf_uint8_t*)&(p_cur_node->str), &offset);
        *p_length	+= offset;
    }
    /* Update correct length */
    icf_pack_U32(p_dest + count_offset, (icf_uint8_t*)&i, &offset);

	return ICF_SUCCESS;
}

icf_uint32_t
icf_pack_icf_app_add_req_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t    *p_length
)
{
    icf_uint32_t        offset = 0;

    icf_app_add_req_st    *p_icf_app_add_req_st = ICF_NULL;
    
    p_icf_app_add_req_st    = (icf_app_add_req_st *)p_src;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function packs icf_app_add_req_st */
    ICF_CLIB_TRACE(("Packing icf_app_add_req_st\n"));

	icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_app_add_req_st->bit_mask), &offset);

	*p_length += offset;

    icf_pack_icf_string_st(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_app_add_req_st->app_name), &offset);
    
    *p_length    += offset;

	icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_app_add_req_st->routing_info.len), &offset);

	*p_length += offset;

	if ( ICF_NULL == p_icf_app_add_req_st->routing_info.ptr)
		return ICF_FAILURE;

    icf_pack_icf_route_info_st(p_dest + (*p_length), (icf_uint8_t*)(p_icf_app_add_req_st->routing_info.ptr), &offset);
    
    *p_length    += offset;


#ifdef ICF_REL_OLD
    icf_pack_U8(p_dest + (*p_length), (icf_uint8_t *)&(p_icf_app_add_req_st->call_type), &offset);
    
    *p_length    += offset;
#endif

    icf_pack_icf_filter_list_st(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_app_add_req_st->filter), &offset);
    
    *p_length    += offset;

    icf_pack_icf_header_name_list_st(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_app_add_req_st->header_name_list), &offset);
    
    *p_length    += offset;

	icf_pack_icf_string_list_st(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_app_add_req_st->supported_methods), &offset);

    *p_length	+= offset;

    icf_pack_icf_string_list_st(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_app_add_req_st->supported_events), &offset);
    
	*p_length	+= offset;

    icf_pack_icf_string_list_st(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_app_add_req_st->supported_content_types), &offset);
    
	*p_length	+= offset;

    icf_pack_icf_string_list_st(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_app_add_req_st->supported_encodings), &offset);
    
	*p_length	+= offset;

    icf_pack_icf_string_list_st(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_app_add_req_st->supported_languages), &offset);
    
	*p_length	+= offset;


/*ICF REL 6.0 : multiple m lines*/
	if(p_icf_app_add_req_st->bit_mask & ICF_APP_ADD_MEDIA_PROFILE_PRESENT)
	{
		icf_pack_icf_stream_capab_list_st(p_dest + (*p_length), (p_icf_app_add_req_st->p_media_profile),  &offset);
	
		*p_length	+= offset;
	}

	icf_pack_U8(p_dest + (*p_length),(icf_uint8_t *)&(p_icf_app_add_req_st->rtp_detection_reqd),  &offset);
	
	*p_length	+= offset;
    
    icf_pack_U8(p_dest + (*p_length),
        (icf_uint8_t *)&(p_icf_app_add_req_st->merge_req_wo_rem_sdp),  &offset);
	
	*p_length	+= offset;

    return ICF_SUCCESS;
}

icf_uint32_t
icf_pack_icf_header_name_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t    *p_length
)
{
    icf_uint32_t        offset = 0;

    icf_header_name_st    *p_icf_header_name_st = ICF_NULL;

/* This function packs icf_header_name_st */
	ICF_CLIB_TRACE(("Packing icf_header_name_st\n"));
	
	/* Initialise length to 0 */
	*p_length	= 0;

    if (!p_src)
        return ICF_SUCCESS;
    
    p_icf_header_name_st    = (icf_header_name_st *)p_src;

    icf_pack_icf_string_st(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_header_name_st->hdr_name), &offset);
    
    *p_length    += offset;

    /* RN -- Manually added code*/
    if (p_icf_header_name_st->next) {
        icf_pack_icf_header_name_st(p_dest + (*p_length), (icf_uint8_t*)p_icf_header_name_st->next, &offset);
        *p_length += offset;
    }


    return ICF_SUCCESS;
}

icf_uint32_t
icf_pack_icf_header_name_list_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t    *p_length
)
{
    icf_uint32_t        offset = 0;

    icf_header_name_list_st    *p_icf_header_name_list_st = ICF_NULL;
    
    p_icf_header_name_list_st    = (icf_header_name_list_st *)p_src;

    /* This function packs icf_header_name_list_st */
    ICF_CLIB_TRACE(("Packing icf_header_name_list_st\n"));
    
    /* Initialise length to 0 */
    *p_length    = 0;

    icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_header_name_list_st->count), &offset);
    
    *p_length    += offset;

    if (ICF_NULL != p_icf_header_name_list_st->count)
    {
	    /* RN -- Manually added code*/
	    icf_pack_icf_header_name_st
            (p_dest + (*p_length), (icf_uint8_t*)(p_icf_header_name_list_st->hdr), &offset);
	
        *p_length	+= offset;
    }


    return ICF_SUCCESS;
}

icf_uint32_t
icf_pack_icf_app_add_cfm_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t    *p_length
)
{
    icf_uint32_t        offset = 0;

#ifdef ICF_NW_ACTIVATE
    icf_uint32_t        count = 0;
#endif

    icf_app_add_cfm_st    *p_icf_app_add_cfm_st = ICF_NULL;
    
    p_icf_app_add_cfm_st    = (icf_app_add_cfm_st *)p_src;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function packs icf_app_add_cfm_st */
    ICF_CLIB_TRACE(("Packing icf_app_add_cfm_st\n"));

    icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_app_add_cfm_st->bit_mask), &offset);
    
    *p_length    += offset;
    
    icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_app_add_cfm_st->result), &offset);
    
    *p_length    += offset;

    if(p_icf_app_add_cfm_st->bit_mask & ICF_APP_ADD_CFM_ERROR_CAUSE_PRESENT)
    {
        icf_pack_U16(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_app_add_cfm_st->error_cause), &offset);
        
        *p_length    += offset;
    }
#ifdef ICF_NW_ACTIVATE
    if(p_icf_app_add_cfm_st->bit_mask & ICF_APP_ADD_CFM_NUM_NW_PRESENT)
    {
        icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_app_add_cfm_st->num_nw), &offset);
        
        *p_length    += offset;
    }

    if(p_icf_app_add_cfm_st->bit_mask & ICF_APP_ADD_CFM_NET_INFO_PRESENT)
    {
    /* This piece of code packs a variable length octet string */
    
    count = 0;
    while(count < p_icf_app_add_cfm_st->num_nw)
    {
        icf_pack_icf_network_info_st(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_app_add_cfm_st->nw_info[count]), &offset);
        
        *p_length    += offset;
        
        count++;
    }
    }
#endif
/*    icf_pack_icf_pub_uri_list_st(p_dest + (*p_length), p_icf_app_add_cfm_st->p_uri_list, &offset);
    *p_length    += offset; */
    return ICF_SUCCESS;
}

icf_uint32_t
icf_pack_icf_app_remove_req_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t    *p_length
)
{
    icf_uint32_t        offset = 0;

    icf_app_remove_req_st    *p_icf_app_remove_req_st = ICF_NULL;
    
    p_icf_app_remove_req_st    = (icf_app_remove_req_st *)p_src;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function packs icf_app_remove_req_st */
    ICF_CLIB_TRACE(("Packing icf_app_remove_req_st\n"));
    
    icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_app_remove_req_st->option), &offset);
    
    *p_length    += offset;

    return ICF_SUCCESS;
}

icf_uint32_t
icf_pack_icf_app_remove_cfm_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t    *p_length
)
{
    icf_uint32_t        offset = 0;

    icf_app_remove_cfm_st    *p_icf_app_remove_cfm_st = ICF_NULL;
    
    p_icf_app_remove_cfm_st    = (icf_app_remove_cfm_st *)p_src;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function packs icf_app_remove_cfm_st */
    ICF_CLIB_TRACE(("Packing icf_app_remove_cfm_st\n"));
    
    icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_app_remove_cfm_st->result), &offset);
    
    *p_length    += offset;

    icf_pack_U16(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_app_remove_cfm_st->error_cause), &offset);
    
    *p_length    += offset;

    return ICF_SUCCESS;
}

icf_uint32_t
icf_pack_icf_set_self_id_req_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t    *p_length
)
{
    icf_uint32_t        offset = 0;

    icf_set_self_id_req_st    *p_icf_set_self_id_req_st = ICF_NULL;
    
    p_icf_set_self_id_req_st    = (icf_set_self_id_req_st *)p_src;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function packs icf_set_self_id_req_st */
    ICF_CLIB_TRACE(("Packing icf_set_self_id_req_st\n"));
    
    icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_set_self_id_req_st->bit_mask), &offset);
    *p_length    += offset;

    icf_pack_icf_line_id_t(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_set_self_id_req_st->line_id), &offset);    
    *p_length    += offset;

    if(p_icf_set_self_id_req_st->bit_mask & ICF_SET_SLF_ID_PRM_ADDR_ACTION)
    {
        icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_set_self_id_req_st->action), &offset);
        *p_length    += offset;
    }

    if(p_icf_set_self_id_req_st->bit_mask & ICF_SET_SLF_ID_PRM_ADDR)
    {
        icf_pack_icf_address_st(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_set_self_id_req_st->self_addr), &offset);
    *p_length    += offset;
    }

    if(p_icf_set_self_id_req_st->bit_mask & ICF_SET_SLF_ID_PRM_PRIORITY)
    {
        icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_set_self_id_req_st->priority), &offset);
    *p_length    += offset;
    }

    if(p_icf_set_self_id_req_st->bit_mask & ICF_SET_SLF_ID_PRM_NAME)
    {
        icf_pack_icf_string_st(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_set_self_id_req_st->self_name), &offset);
    *p_length    += offset;
    
    }

    if(p_icf_set_self_id_req_st->bit_mask & ICF_SET_SLF_ID_PRM_DEFAULT_SETTINGS)
    {
       icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_set_self_id_req_st->default_settings), &offset);
    *p_length    += offset;

    }

    if(p_icf_set_self_id_req_st->bit_mask & ICF_SET_SLF_ID_PRM_AUTH_KEY)
    {
        icf_pack_icf_string_st(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_set_self_id_req_st->authentication_key), &offset);
    *p_length    += offset;
    }

    if(p_icf_set_self_id_req_st->bit_mask & ICF_SET_SLF_ID_PRM_AUTH_PASSWORD)
    {
        
    icf_pack_icf_string_st(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_set_self_id_req_st->authentication_password), &offset);
    *p_length    += offset;
    }

    if(p_icf_set_self_id_req_st->bit_mask & ICF_SET_SLF_ID_PRM_REALM)
    {    
	    icf_pack_icf_string_st(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_set_self_id_req_st->configured_realm), &offset);
	    *p_length    += offset;
    }    
    
    return(ICF_SUCCESS);
}

icf_uint32_t
icf_pack_icf_set_transport_req_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t    *p_length
)
{
    icf_uint32_t        offset = 0;

    icf_set_transport_req_st    *p_icf_set_transport_req_st = ICF_NULL;
    
    p_icf_set_transport_req_st    = (icf_set_transport_req_st *)p_src;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function packs icf_set_transport_req_st */
    ICF_CLIB_TRACE(("Packing icf_set_transport_req_st\n"));
    
    icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&
		(p_icf_set_transport_req_st->bit_mask), &offset);
    *p_length    += offset;

    if(p_icf_set_transport_req_st->bit_mask & ICF_SET_TRP_REQ_PROXY_ADDR)
    {
        icf_pack_icf_config_proxy_transport_address_st(p_dest + (*p_length), 
		(icf_uint8_t*)&(p_icf_set_transport_req_st->proxy_addr), &offset);
        *p_length    += offset;
    }

    if(p_icf_set_transport_req_st->bit_mask & ICF_SET_TRP_REQ_SEC_PROXY_ADDR)
    {
        icf_pack_icf_config_sec_transport_address_st(p_dest + (*p_length), 
		(icf_uint8_t*)&(p_icf_set_transport_req_st->sec_proxy_addr), &offset);
        *p_length    += offset;
    }

	if(p_icf_set_transport_req_st->bit_mask & ICF_SET_TRP_REQ_DNS_SERVER_ADDR)
    {
        icf_pack_icf_transport_address_st(p_dest + (*p_length), (icf_uint8_t*)&
			(p_icf_set_transport_req_st->dns_server_addr), &offset);
        *p_length    += offset;
    }

	if(p_icf_set_transport_req_st->bit_mask & 
		ICF_SET_TRP_REQ_SEC_DNS_SERVER_ADDR)
    {
        icf_pack_icf_transport_address_st(p_dest + (*p_length), (icf_uint8_t*)&
			(p_icf_set_transport_req_st->sec_dns_server_addr), &offset);
        *p_length    += offset;
    }

    if(p_icf_set_transport_req_st->bit_mask & ICF_SET_TRP_REQ_SLF_TRANSPORT_ADDRESS)
    {
        icf_pack_icf_transport_addr_st(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_set_transport_req_st->self_transport_address), &offset);
        *p_length    += offset;
    }

    if(p_icf_set_transport_req_st->bit_mask & ICF_SET_TRP_REQ_SLF_MODE)
    {
        icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_set_transport_req_st->self_mode), &offset);
        *p_length    += offset;
    }

    if(p_icf_set_transport_req_st->bit_mask & ICF_SET_TRP_REQ_SLF_PORT)
    {
        icf_pack_U16(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_set_transport_req_st->self_port), &offset);
        *p_length    += offset;
    }

    if(p_icf_set_transport_req_st->bit_mask & ICF_SET_TRP_REQ_SLF_ADDR_PER_LINE)
    {
        icf_pack_icf_config_transport_address_st(p_dest + (*p_length), 
            (icf_uint8_t*)&(p_icf_set_transport_req_st->self_addr_per_line), &offset);
        *p_length    += offset;
    }
    if(p_icf_set_transport_req_st->bit_mask & ICF_SET_TRP_REQ_DNS_SRC_PORT)
    {
         icf_pack_U16(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_set_transport_req_st->dns_source_port), &offset);

        *p_length    += offset;
    }
    /* Packing of the new fields dhcp_server_addr introduced in 8.0 */
    if(ICF_SET_TRP_REQ_DHCP_SERVER_ADDR & p_icf_set_transport_req_st->bit_mask)
    {
        icf_pack_icf_transport_address_st(p_dest + (*p_length), (icf_uint8_t*)& \
					(p_icf_set_transport_req_st->dhcp_server_addr), &offset);
        *p_length    += offset;
    }     
    /*  Packing of the new Field dhcp_client_port introduced in 8.0 */
    if(ICF_SET_TRP_REQ_DHCP_CLIENT_PORT & p_icf_set_transport_req_st->bit_mask)
    {
	icf_pack_U16(p_dest + (*p_length), (icf_uint8_t*)& \
				(p_icf_set_transport_req_st->dhcp_client_port), &offset);
        *p_length    += offset;	
    }

    return(ICF_SUCCESS);
}

icf_uint32_t
icf_pack_icf_config_proxy_transport_address_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t    *p_length
)
{
    icf_uint32_t        offset = 0;

    icf_config_proxy_transport_address_st    *p_icf_config_transport_address_st = ICF_NULL;
    
    p_icf_config_transport_address_st    = (icf_config_proxy_transport_address_st *)p_src;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function packs icf_config_transport_address_st */
    ICF_CLIB_TRACE(("Packing icf_config_transport_address_st\n"));
    
    icf_pack_icf_line_id_t(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_config_transport_address_st->line_id), &offset);
    
    *p_length    += offset;

    icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_config_transport_address_st->action), &offset);
    
    *p_length    += offset;

    icf_pack_icf_transport_address_st(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_config_transport_address_st->transport_addr), &offset);
    
    *p_length    += offset;

    icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_config_transport_address_st->transport_mode), &offset);
    
    *p_length    += offset;

    icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_config_transport_address_st->sigcomp_options), &offset);
    
    *p_length    += offset;

    return(ICF_SUCCESS);
}

/******************************************************************************
Function name:  icf_pack_icf_config_sec_transport_address_st
Descrtiption:	This function packs the structure 
				icf_config_sec_transport_address_st
*******************************************************************************/
icf_uint32_t
icf_pack_icf_config_sec_transport_address_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t    *p_length
)
{
    icf_uint32_t        offset = 0;

    icf_config_sec_transport_address_st    *p_icf_config_sec_transport_address_st = ICF_NULL;
    
    p_icf_config_sec_transport_address_st    = (icf_config_sec_transport_address_st *)p_src;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function packs icf_config_sec_transport_address_st */
    ICF_CLIB_TRACE(("Packing icf_config_sec_transport_address_st\n"));
    
    icf_pack_icf_line_id_t(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_config_sec_transport_address_st->line_id), &offset);
    
    *p_length    += offset;

    icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_config_sec_transport_address_st->action), &offset);
    
    *p_length    += offset;

    icf_pack_icf_transport_address_st(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_config_sec_transport_address_st->transport_addr), &offset);
    
    *p_length    += offset;

    return(ICF_SUCCESS);
}

icf_uint32_t
icf_pack_icf_config_transport_address_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t    *p_length
)
{
    icf_uint32_t        offset = 0;

    icf_config_transport_address_st    
                                      *p_icf_config_transport_address_st = ICF_NULL;

    p_icf_config_transport_address_st  = (icf_config_transport_address_st *)p_src;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function packs icf_config_transport_address_st */
    ICF_CLIB_TRACE(("Packing icf_config_transport_address_st\n"));

    icf_pack_icf_line_id_t(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_config_transport_address_st->line_id), &offset);

    *p_length    += offset;

    icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_config_transport_address_st->action), &offset);

    *p_length    += offset;

    icf_pack_icf_transport_address_st(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_config_transport_address_st->transport_addr), &offset);

    *p_length    += offset;

    icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_config_transport_address_st->transport_mode), &offset);

    *p_length    += offset;

    return(ICF_SUCCESS);
}

icf_uint32_t
icf_pack_icf_config_transport_address_list_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t    *p_length
)
{
    icf_uint32_t        offset = 0;
    icf_uint32_t        count = 0;

    icf_config_transport_address_list_st    *p_icf_config_transport_address_list_st = ICF_NULL;
    
    p_icf_config_transport_address_list_st    = (icf_config_transport_address_list_st *)p_src;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function packs icf_config_transport_address_list_st */
    ICF_CLIB_TRACE(("Packing icf_config_transport_address_list_st\n"));
    
    icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_config_transport_address_list_st->all_lines), &offset);
    
    *p_length    += offset;

    icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_config_transport_address_list_st->num_of_elements), &offset);
    
    *p_length    += offset;

    /* This piece of code packs a variable length octet string */
    
    count = 0;
    while(count < p_icf_config_transport_address_list_st->num_of_elements)
    {
        icf_pack_icf_config_transport_address_st(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_config_transport_address_list_st->transport_addr[count]), &offset);
        
        *p_length    += offset;
        
        count++;
    }
    return(ICF_SUCCESS);
}

icf_uint32_t
icf_pack_icf_set_registration_req_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t    *p_length
)

   

{
    icf_uint32_t        offset = 0;

    icf_set_registration_req_st    *p_icf_set_registration_req_st = ICF_NULL;
    
    p_icf_set_registration_req_st    = (icf_set_registration_req_st *)p_src;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function packs icf_set_registration_req_st */
    ICF_CLIB_TRACE(("Packing icf_set_registration_req_st\n"));
    
    icf_pack_icf_line_id_t(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_set_registration_req_st->line_id), &offset);
    *p_length    += offset;

    icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_set_registration_req_st->action), &offset);
    *p_length    += offset;

    icf_pack_icf_transport_address_st(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_set_registration_req_st->registrar_addr), &offset);
    *p_length    += offset;

    icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_set_registration_req_st->bit_mask), &offset);
    *p_length    += offset;
	 if(p_icf_set_registration_req_st->bit_mask 
        & ICF_SET_REG_HS_DURATION)
    {
		 icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_set_registration_req_st->register_head_start_timer), &offset);
        *p_length    += offset;

	}

    if(p_icf_set_registration_req_st->bit_mask & ICF_SET_REG_SEC_REGISTRAR)
    {
        icf_pack_icf_config_sec_transport_address_st(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_set_registration_req_st->sec_registrar_addr), &offset);
        *p_length    += offset;
    }

    if(p_icf_set_registration_req_st->bit_mask & ICF_SET_REG_REQ_MODE)
    {
        icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_set_registration_req_st->reg_mode), &offset);
        *p_length    += offset;
    }

    if(p_icf_set_registration_req_st->bit_mask & ICF_SET_REG_REQ_DURATION)
    {
        icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_set_registration_req_st->registration_duration), &offset);
        *p_length    += offset;
    }

    if(p_icf_set_registration_req_st->bit_mask & ICF_SET_REG_REQ_ROUTE_ADDR)
    {
        icf_pack_icf_transport_address_st(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_set_registration_req_st->reg_route_addr), &offset);
        *p_length    += offset;
    }
       
    return(ICF_SUCCESS);
}




icf_uint32_t
icf_pack_icf_config_registration_params_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t    *p_length
)
{
    icf_uint32_t        offset = 0;

    icf_config_registration_params_st    *p_icf_config_registration_params_st = ICF_NULL;
    
    p_icf_config_registration_params_st    = (icf_config_registration_params_st *)p_src;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function packs icf_config_registration_params_st */
    ICF_CLIB_TRACE(("Packing icf_config_registration_params_st\n"));
    
    icf_pack_icf_line_id_t(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_config_registration_params_st->line_id), &offset);
    
    *p_length    += offset;

    icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_config_registration_params_st->action), &offset);
    
    *p_length    += offset;

    icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_config_registration_params_st->bit_mask), &offset);
    
    *p_length    += offset;

    if(p_icf_config_registration_params_st->bit_mask & ICF_CONFIG_RG_PRM_RG_ADDR)
    {
        icf_pack_icf_transport_address_st(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_config_registration_params_st->registrar_addr), &offset);
        
        *p_length    += offset;
    }

    if(p_icf_config_registration_params_st->bit_mask & ICF_CONFIG_RG_PRM_RG_MODE)
    {
        icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_config_registration_params_st->reg_mode), &offset);
        
        *p_length    += offset;
    }

    if(p_icf_config_registration_params_st->bit_mask & ICF_CONFIG_RG_PRM_RG_DURATION)
    {
        icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_config_registration_params_st->registration_duration), &offset);
        
        *p_length    += offset;
    }

    if(p_icf_config_registration_params_st->bit_mask & ICF_CONFIG_RG_PRM_AUTH_KEY)
    {
        icf_pack_icf_string_st(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_config_registration_params_st->authentication_key), &offset);
        
        *p_length    += offset;
    }

    if(p_icf_config_registration_params_st->bit_mask & ICF_CONFIG_RG_PRM_AUTH_PASSWORD)
    {
        icf_pack_icf_string_st(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_config_registration_params_st->authentication_password), &offset);
        
        *p_length    += offset;
    }

    return(ICF_SUCCESS);
}

icf_uint32_t
icf_pack_icf_configure_cfm_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t    *p_length
)
{
    icf_uint32_t        offset = 0;

    icf_configure_cfm_st    *p_icf_configure_cfm_st = ICF_NULL;
    
    p_icf_configure_cfm_st    = (icf_configure_cfm_st *)p_src;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function packs icf_configure_cfm_st */
    ICF_CLIB_TRACE(("Packing icf_configure_cfm_st\n"));
    
    icf_pack_U16(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_configure_cfm_st->api_id), &offset);
    
    *p_length    += offset;
    /* SPR 15997: Line id in configure confirm API */
    icf_pack_icf_line_id_t(p_dest + (*p_length), 
            (icf_uint8_t*)&(p_icf_configure_cfm_st->line_id), &offset);

    *p_length    += offset;
    icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_configure_cfm_st->result), &offset);
    
    *p_length    += offset;

    icf_pack_U16(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_configure_cfm_st->error_cause), &offset);
    
    *p_length    += offset;

    return(ICF_SUCCESS);
}

icf_uint32_t
icf_pack_icf_register_req_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t    *p_length
)
{
    icf_uint32_t        offset = 0;

    icf_register_req_st    *p_icf_register_req_st = ICF_NULL;
    
    p_icf_register_req_st    = (icf_register_req_st *)p_src;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function packs icf_register_req_st */
    ICF_CLIB_TRACE(("Packing icf_register_req_st\n"));
    
    icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_register_req_st->bit_mask), &offset);
    
    *p_length    += offset;

    icf_pack_icf_line_id_t(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_register_req_st->line_id), &offset);
    
    *p_length    += offset;

    if(p_icf_register_req_st->bit_mask & ICF_REG_REQ_TO_PRESENT)
    {
        icf_pack_icf_address_st(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_register_req_st->to), &offset);
        
        *p_length    += offset;
    }

    if(p_icf_register_req_st->bit_mask & ICF_REG_REQ_EXPIRY_PRESENT)
    {
        icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_register_req_st->expiry), &offset);
        
        *p_length    += offset;
    }

    if(p_icf_register_req_st->bit_mask & ICF_HDR_LIST_PRESENT)
    {
        icf_pack_icf_header_list_st(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_register_req_st->header_list), &offset);
        
        *p_length    += offset;
    }

    if(p_icf_register_req_st->bit_mask & ICF_TAG_LIST_PRESENT)
    {
        icf_pack_icf_tag_list_st(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_register_req_st->tag_list), &offset);
        
        *p_length    += offset;
    }

    if(p_icf_register_req_st->bit_mask & ICF_BODY_LIST_PRESENT)
    {
        icf_pack_icf_msg_body_list_st(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_register_req_st->body_list), &offset);
        
        *p_length    += offset;
    }

    if(p_icf_register_req_st->bit_mask & ICF_REG_REQ_CONTACT_LIST_REQD)
    {
       icf_pack_U8(p_dest + (*p_length),\
         (icf_uint8_t*)&(p_icf_register_req_st->contact_list_required), &offset);
        
       *p_length    += offset; 
    }

    /*Packing registration_refresh_required*/
    if(ICF_REG_REQ_REFRESH_REG_FLAG_PRESENT & p_icf_register_req_st->bit_mask)
    {
        icf_pack_U8(p_dest + (*p_length),\
            (icf_uint8_t*)&(p_icf_register_req_st->registration_refresh_required), &offset);

       *p_length    += offset;
    }

    if(ICF_REG_REQ_REG_EVENT_PCKG_SUBS_FLAG_PRESENT & p_icf_register_req_st->bit_mask)
    {
       icf_pack_U8(p_dest + (*p_length),\
         (icf_uint8_t*)&(p_icf_register_req_st->reg_event_pckg_subs_required), &offset);
        
       *p_length    += offset; 
    }

    

    /*pack security_client*/
    if(p_icf_register_req_st->bit_mask & ICF_REG_REQ_SECURITY_CLIENT_PRESENT)
    {
       icf_pack_icf_large_string_st(p_dest + (*p_length), \
           (icf_uint8_t*)&(p_icf_register_req_st->security_client), &offset);

       *p_length    += offset;
    }

    /*  Fix for CSR 1-7193955: SPR 19668 */
    if(ICF_REG_REQ_REFRESH_MULTIPLIER_PRESENT & p_icf_register_req_st->bit_mask)
    {
        icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_register_req_st->registration_refresh_multiplier), &offset);
        
        *p_length    += offset;
    }

    /* SPR 19999 -  Fix for CSR 1-7533733 SPR 19870 */
    if(ICF_REG_REQ_REFRESH_INDICATIONS_REQD & p_icf_register_req_st->bit_mask)
    {
        icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_register_req_st->refresh_ind_reqd), &offset);
        
        *p_length    += offset;
    }
    if(ICF_REG_REQ_DEREG_SPECIFIC_CONTACT_PRESENT & p_icf_register_req_st->bit_mask)
    {
        icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_register_req_st->dereg_specific_contact), &offset);
        
        *p_length    += offset;
    }


    return ICF_SUCCESS;
}

icf_uint32_t
icf_pack_icf_header_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t    *p_length
)
{
    icf_uint32_t        offset = 0;

    icf_header_st    *p_icf_header_st = ICF_NULL;

    /* Initialise length to 0 */
    *p_length    = 0;

    if (!p_src)
        return ICF_SUCCESS;

    p_icf_header_st    = (icf_header_st *)p_src;

    /* This function packs icf_header_st */
    ICF_CLIB_TRACE(("Packing icf_header_st\n"));
    
    icf_pack_icf_string_st(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_header_st->hdr_name), &offset);
    
    *p_length    += offset;

    icf_pack_icf_large_string_st(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_header_st->hdr_value), &offset);
    
    *p_length    += offset;


    /* RN -- Manually added code*/
    if (p_icf_header_st->next) {
        icf_pack_icf_header_st(p_dest + (*p_length), (icf_uint8_t*)p_icf_header_st->next, &offset);
        *p_length += offset;
    }


    return ICF_SUCCESS;
}

icf_uint32_t
icf_pack_icf_header_list_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t    *p_length
)
{
    icf_uint32_t        offset = 0;

    icf_header_list_st    *p_icf_header_list_st = ICF_NULL;
	icf_uint8_t			temp_count = 0;
	icf_header_st 		*p_temp_hdr = ICF_NULL;
    
    p_icf_header_list_st    = (icf_header_list_st *)p_src;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function packs icf_header_list_st */
    ICF_CLIB_TRACE(("Packing icf_header_list_st\n"));

	if(p_icf_header_list_st->hdr)
	{
		p_temp_hdr = (icf_header_st *)p_icf_header_list_st->hdr;
		while(p_temp_hdr)
		{
			temp_count++;
			p_temp_hdr = p_temp_hdr->next;
		}
	}

    p_icf_header_list_st->count = temp_count;
    
    icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_header_list_st->count), &offset);
    
    *p_length    += offset;


    /* RN -- Manually added code*/
    icf_pack_icf_header_st(p_dest + (*p_length), (icf_uint8_t*)(p_icf_header_list_st->hdr), &offset);

    *p_length    += offset;

    return ICF_SUCCESS;
}

icf_uint32_t
icf_pack_icf_tag_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t    *p_length
)
{
    icf_uint32_t        offset = 0;

    icf_tag_st    *p_icf_tag_st = ICF_NULL;

    if (!p_src)
        return ICF_SUCCESS;
    
    p_icf_tag_st    = (icf_tag_st *)p_src;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function packs icf_tag_st */
    ICF_CLIB_TRACE(("Packing icf_tag_st\n"));

    icf_pack_U8(p_dest + (*p_length),(icf_uint8_t*)&(p_icf_tag_st->tag_type),&offset);

    *p_length += offset;
    
    icf_pack_icf_string_st(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_tag_st->hdr_name), &offset);
    
    *p_length    += offset;

    icf_pack_icf_string_st(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_tag_st->tag_value), &offset);
    
    *p_length    += offset;


    /* RN -- Manually added code*/
    if (p_icf_tag_st->next) {
        icf_pack_icf_tag_st(p_dest + (*p_length), (icf_uint8_t*)p_icf_tag_st->next, &offset);
        *p_length += offset;
    }


    return ICF_SUCCESS;
}

icf_uint32_t
icf_pack_icf_tag_list_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t    *p_length
)
{
    icf_uint32_t        offset = 0;

	icf_uint8_t         temp_count = 0;
    icf_tag_st	    	*p_temp_hdr = ICF_NULL;

    icf_tag_list_st    *p_icf_tag_list_st = ICF_NULL;
    
    p_icf_tag_list_st    = (icf_tag_list_st *)p_src;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function packs icf_tag_list_st */
    ICF_CLIB_TRACE(("Packing icf_tag_list_st\n"));

	if(p_icf_tag_list_st->tag)
    {
        p_temp_hdr = (icf_tag_st *)p_icf_tag_list_st->tag;
        while(p_temp_hdr)
        {
            temp_count++;
            p_temp_hdr = p_temp_hdr->next;
        }
    }
    
    p_icf_tag_list_st->count = temp_count;
    
    icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_tag_list_st->count), &offset);
    
    *p_length    += offset;

    
    /* RN -- Manually added code*/
    icf_pack_icf_tag_st(p_dest + (*p_length), (icf_uint8_t*)(p_icf_tag_list_st->tag), &offset);
    
    *p_length    += offset;

    return ICF_SUCCESS;
}

icf_uint32_t
icf_pack_icf_msg_body_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t    *p_length
)
{
    icf_uint32_t        offset = 0;
    icf_uint32_t        count = 0;

    icf_msg_body_st    *p_icf_msg_body_st = ICF_NULL;

    if (!p_src)
        return ICF_SUCCESS;
    
    p_icf_msg_body_st    = (icf_msg_body_st *)p_src;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function packs icf_msg_body_st */
    ICF_CLIB_TRACE(("Packing icf_msg_body_st\n"));
    
    icf_pack_icf_string_st(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_msg_body_st->content_type), &offset);
    
    *p_length    += offset;
    
    /*Message body header changes Start*/
   
    if(0 < p_icf_msg_body_st->msg_body_hdr_list.count)
    {
	    icf_pack_icf_header_list_st(p_dest+ (*p_length),(icf_uint8_t *)
		    &(p_icf_msg_body_st->msg_body_hdr_list), &offset);
    }
	else if(0 == p_icf_msg_body_st->msg_body_hdr_list.count)
	{
		icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_msg_body_st->\
			msg_body_hdr_list.count), &offset);
    }
    
    /*Message body header changes Ends*/
    *p_length += offset;
    icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_msg_body_st->\
			     length), &offset);
    *p_length    += offset;
    /* This piece of code packs a variable length octet string */
    
    count = 0;
    /* Pack the NULL Character*/
    while((count < p_icf_msg_body_st->length + 1) &&
          (ICF_NULL != p_icf_msg_body_st->value))
    {
        icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_msg_body_st->value[count]), &offset);
        
        *p_length    += offset;
        
        count++;
    }

    
    /* RN -- Manually added code    */
    if (p_icf_msg_body_st->next) {
        icf_pack_icf_msg_body_st(p_dest + (*p_length), (icf_uint8_t*)p_icf_msg_body_st->next, &offset);
        *p_length += offset;
    }

    return ICF_SUCCESS;
}

icf_uint32_t
icf_pack_icf_msg_body_list_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t    *p_length
)
{
    icf_uint32_t        offset = 0;

	icf_uint8_t         temp_count = 0;
    icf_msg_body_st		*p_temp_hdr = ICF_NULL;

    icf_msg_body_list_st    *p_icf_msg_body_list_st = ICF_NULL;
    
    p_icf_msg_body_list_st    = (icf_msg_body_list_st *)p_src;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function packs icf_msg_body_list_st */
    ICF_CLIB_TRACE(("Packing icf_msg_body_list_st\n"));
    if(p_icf_msg_body_list_st->body)
	{
        p_temp_hdr = (icf_msg_body_st *)p_icf_msg_body_list_st->body;
        while(p_temp_hdr)
        {
            temp_count++;
            p_temp_hdr = p_temp_hdr->next;
        }
    }
    p_icf_msg_body_list_st->count = temp_count;
    icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_msg_body_list_st->count), &offset);
    
    *p_length    += offset;
    /*Message body header changes Start*/
    /* Pack the Content-Type header present in the message body list for SIP 
     * message.
     */

    icf_pack_icf_string_st(p_dest + (*p_length), (icf_uint8_t*)
		    &(p_icf_msg_body_list_st->content_type), &offset);
    *p_length += offset;
    /*Message body header changes Ends*/
    /* RN -- Manually added code*/
    icf_pack_icf_msg_body_st(p_dest + (*p_length), (icf_uint8_t*)(p_icf_msg_body_list_st->body), &offset);
    
    *p_length    += offset;

    return ICF_SUCCESS;
}

icf_uint32_t
icf_pack_icf_create_call_req_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t    *p_length
)
{
    icf_uint32_t        offset = 0;

    icf_create_call_req_st    *p_icf_create_call_req_st = ICF_NULL;
    
    p_icf_create_call_req_st    = (icf_create_call_req_st *)p_src;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function packs icf_create_call_req_st */
    ICF_CLIB_TRACE(("Packing icf_create_call_req_st\n"));

    icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_create_call_req_st->bit_mask), &offset);
    
    *p_length    += offset;

    icf_pack_icf_line_id_t(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_create_call_req_st->line_id), &offset);
    
    *p_length    += offset;

#ifdef ICF_REL_OLD
    icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_create_call_req_st->call_type), &offset);
   
    *p_length    += offset;
#endif

    icf_pack_icf_address_st(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_create_call_req_st->called_party_addr), &offset);
    
    *p_length    += offset;

    if(p_icf_create_call_req_st->bit_mask & ICF_CALLED_PARTY_NAME_PRESENT)
    {
        icf_pack_icf_string_st(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_create_call_req_st->called_party_name), &offset);
        
        *p_length    += offset;
    }

    if(p_icf_create_call_req_st->bit_mask & ICF_TOGGLE_PRES_IND_PRESENT)
    {
        icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_create_call_req_st->toggle_presentation_indicator), &offset);
        
        *p_length    += offset;
    }

    if(p_icf_create_call_req_st->bit_mask & ICF_QOS_MARK_PRESENT)
    {
        icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_create_call_req_st->qos_mark), &offset);
        
        *p_length    += offset;
    }
   
    if(p_icf_create_call_req_st->bit_mask & ICF_PREF_IDENTITY_PRESENT)
    {
        icf_pack_icf_address_st(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_create_call_req_st->pref_identity), &offset);
        
        *p_length    += offset;
    }

	if (p_icf_create_call_req_st->bit_mask & ICF_CALLING_PARTY_ADDR)
    {
        icf_pack_icf_address_st(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_create_call_req_st->calling_party_address), &offset);
        
        *p_length    += offset;
    }

    if(p_icf_create_call_req_st->bit_mask & ICF_CALLING_PARTY_NAME)
    {
        icf_pack_icf_string_st(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_create_call_req_st->calling_party_name), &offset);
        
        *p_length    += offset;
    }

    if(p_icf_create_call_req_st->bit_mask & ICF_HDR_LIST_PRESENT)
    {
        icf_pack_icf_header_list_st(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_create_call_req_st->header_list), &offset);
        
        *p_length    += offset;
    }

    if(p_icf_create_call_req_st->bit_mask & ICF_TAG_LIST_PRESENT)
    {
        icf_pack_icf_tag_list_st(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_create_call_req_st->tag_list), &offset);
        
        *p_length    += offset;
    }

    if(p_icf_create_call_req_st->bit_mask & ICF_BODY_LIST_PRESENT)
    {
        icf_pack_icf_msg_body_list_st(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_create_call_req_st->body_list), &offset);
        
        *p_length    += offset;
    }

#ifdef ICF_REL_OLD
    if(p_icf_create_call_req_st->bit_mask & ICF_STREAM_LIST_PRESENT)
    {
        icf_pack_icf_stream_list_st(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_create_call_req_st->stream_list), &offset);
        
        *p_length    += offset;
    }
#endif

    if(p_icf_create_call_req_st->bit_mask & ICF_PREF_MEDIA_PROFILE_PRESENT)
    {
        icf_pack_icf_stream_capab_list_st(p_dest + (*p_length), (p_icf_create_call_req_st->p_pref_media_profile), &offset);
        
        *p_length    += offset;
    }
    
    if(p_icf_create_call_req_st->bit_mask & ICF_PRIVACY_HDR_FLAG_PRESENT)
    {
        icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_create_call_req_st->privacy_hdr_all_req), &offset);
        
        *p_length    += offset;
    }

	if(p_icf_create_call_req_st->bit_mask & ICF_RESOURCE_RESERV_FLAG_PRESENT)
	{
		icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_create_call_req_st->resource_reservation_reqd), &offset);

		*p_length    += offset;
	}
    /*Start: Rel8.0 Added for precondition support */
    if(p_icf_create_call_req_st->bit_mask & ICF_PRECONDITION_REQD_FLAG_PRESENT)
    {
        icf_pack_U8(p_dest + (*p_length), \
         (icf_uint8_t*)&(p_icf_create_call_req_st->precondition_required), \
         &offset);
        *p_length    += offset;
    }   
    /*End: Rel8.0 Added for precondition support */

    if(p_icf_create_call_req_st->bit_mask & 
                                     ICF_MEDIA_TYPE_REQD_FLAG_PRESENT)
       {
               icf_pack_U32(p_dest + (*p_length),(icf_uint8_t*)&(
                    p_icf_create_call_req_st->media_transport_type), &offset);
  
               *p_length    += offset;
       }

    /* For User Param Support, if ICF_REQUEST_URI_PARAM_PRESENT bitmask 
     * is present, then pack request_uri_param using the 
     * icf_pack_icf_string_st function */
    if(p_icf_create_call_req_st->bit_mask & ICF_REQUEST_URI_PARAM_PRESENT)
    {
       icf_pack_icf_string_st(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_create_call_req_st->request_uri_param), &offset);
       *p_length    += offset;
    }

    /*PACK srtp_fallback_reqd, if ICF_SRTP_FALLBACK_FLAG_PRESENT bit is set as a boolean*/
    if(ICF_SRTP_FALLBACK_FLAG_PRESENT & p_icf_create_call_req_st->bit_mask)
    {
       icf_pack_U8(p_dest + (*p_length), 
                   (icf_uint8_t*)&(p_icf_create_call_req_st->srtp_fallback_reqd), 
                   &offset);
       *p_length    += offset;
    }

    if(ICF_CREATE_CALL_TUNNEL_MODE_OPERATION_PRESENT & p_icf_create_call_req_st->bit_mask)
    {
       icf_pack_U8(p_dest + (*p_length), 
                   (icf_uint8_t*)&(p_icf_create_call_req_st->tunnel_mode), 
                   &offset);
       *p_length    += offset;
    }

#ifdef ICF_SESSION_TIMER    
    if(ICF_CREATE_CALL_SESSION_TIMER_ENABLE_PRESENT & p_icf_create_call_req_st->bit_mask)
    {
       icf_pack_U8(p_dest + (*p_length), 
                   (icf_uint8_t*)&(p_icf_create_call_req_st->session_timer_enable), 
                   &offset);
       *p_length    += offset;
    }
#endif

    


    if(ICF_CREATE_CALL_PRACK_VALUE_PRESENT & p_icf_create_call_req_st->bit_mask)
    {
        icf_pack_U32(p_dest + (*p_length), 
                (icf_uint8_t*)&(p_icf_create_call_req_st->prack_value), 
                &offset);
        *p_length    += offset;
    }

    /*Packing redirect_support_reqd*/
    if(ICF_CREATE_CALL_REDIRECT_SUPPORT_REQD & p_icf_create_call_req_st->bit_mask)
    {
        icf_pack_U8(p_dest + (*p_length),
            (icf_uint8_t*)&(p_icf_create_call_req_st->redirect_support_reqd),
            &offset);

       *p_length    += offset;
    }

    if(ICF_CREATE_CALL_SETTINGS_PRESENT & p_icf_create_call_req_st->bit_mask)
    {
       icf_pack_U32(p_dest + (*p_length), 
                   (icf_uint8_t*)&(p_icf_create_call_req_st->call_settings), 
                   &offset);
       *p_length    += offset;
    }
    /* Fix for CSR 1-7616406: SPR 20053 */
	if (ICF_CREATE_CALL_SETUP_TIMER_PRESENT & 
            p_icf_create_call_req_st->bit_mask)
	{
        icf_pack_U32(p_dest + (*p_length),
                     (icf_uint8_t*)&(p_icf_create_call_req_st->setup_timer),
                     &offset);
		*p_length    += offset;
	}

    return ICF_SUCCESS;
}

#ifdef ICF_REL_OLD

icf_uint32_t
icf_pack_icf_stream_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t    *p_length
)
{
    icf_uint32_t        offset = 0;
    icf_uint32_t        count = 0;

    icf_stream_st    *p_icf_stream_st = ICF_NULL;
    
    p_icf_stream_st    = (icf_stream_st *)p_src;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function packs icf_stream_st */
    ICF_CLIB_TRACE(("Packing icf_stream_st\n"));
    
    icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_stream_st->type), &offset);
    
    *p_length    += offset;

    icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_stream_st->stream_id), &offset);
    
    *p_length    += offset;

    icf_pack_icf_string_st(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_stream_st->codec), &offset);
    
    *p_length    += offset;

    icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_stream_st->att_count), &offset);
    
    *p_length    += offset;

    /* This piece of code packs a variable length octet string */
    
    count = 0;
    while(count < p_icf_stream_st->att_count)
    {
        icf_pack_icf_string_st(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_stream_st->attributes[count]), &offset);
        
        *p_length    += offset;
        
        count++;
    }
    return(ICF_SUCCESS);
}

#endif



icf_uint32_t
icf_pack_icf_stream_id_list_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t    *p_length
)
{
    icf_uint32_t        offset = 0;
    icf_uint8_t         count = 0;

    icf_stream_id_list_st    *p_icf_stream_id_list_st = ICF_NULL;
    
    p_icf_stream_id_list_st    = (icf_stream_id_list_st *)p_src;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function packs icf_stream_id_list_st */
    ICF_CLIB_TRACE(("Packing icf_stream_id_list_st\n"));
    
    icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_stream_id_list_st->count), &offset);
    
    *p_length    += offset;

    /* This piece of code packs a variable length octet string */
  
    while((count < p_icf_stream_id_list_st->count) && (count < ICF_MAX_STREAM_PER_CALL))
    {
        icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_stream_id_list_st->stream_id[count]), &offset);
        
        *p_length    += offset;
        
        count++;
    }
    return(ICF_SUCCESS);
}

icf_uint32_t
icf_pack_icf_stream_list_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t    *p_length
)
{
	icf_uint32_t        offset = 0;
	
	icf_stream_list_st  *p_icf_stream_list_st = (icf_stream_list_st *)p_src;

	/* This function packs icf_stream_list_st */
    ICF_CLIB_TRACE(("Packing icf_stream_list_st\n"));

	/* Initialise length to 0 */
    *p_length    = 0;

	icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_stream_list_st->stream_id), &offset);

	*p_length += offset;

	
	icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_stream_list_st->stream_type), &offset);
		
	*p_length += offset;

	return ICF_SUCCESS;
}


icf_uint32_t
icf_pack_icf_local_user_alerted_req_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t    *p_length
)
{
    icf_uint32_t        offset = 0;
    icf_uint8_t         count = 0;

    icf_local_user_alerted_req_st    *p_icf_local_user_alerted_req_st = ICF_NULL;
    
    p_icf_local_user_alerted_req_st    = (icf_local_user_alerted_req_st *)p_src;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function packs icf_local_user_alerted_req_st */
    ICF_CLIB_TRACE(("Packing icf_local_user_alerted_req_st\n"));
    
    icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_local_user_alerted_req_st->bit_mask), &offset);
    
    *p_length    += offset;

    icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_local_user_alerted_req_st->early_media_reqd), &offset);
    
    *p_length    += offset;
    
	icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_local_user_alerted_req_st->inband_info_status), &offset);
    
    *p_length    += offset;


    if(p_icf_local_user_alerted_req_st->bit_mask & ICF_PRIVACY_IND_PRESENT)
    {
        icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_local_user_alerted_req_st->privacy_ind), &offset);
        
        *p_length    += offset;
    }

	if(p_icf_local_user_alerted_req_st->bit_mask & ICF_USER_ALERTED_ALERT_TIMER_PRESENT)
    {
        icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_local_user_alerted_req_st->alert_timer_duration), &offset);
        
        *p_length    += offset;
    }

    if(p_icf_local_user_alerted_req_st->bit_mask & ICF_HDR_LIST_PRESENT)
    {
        icf_pack_icf_header_list_st(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_local_user_alerted_req_st->header_list), &offset);
        
        *p_length    += offset;
    }

    if(p_icf_local_user_alerted_req_st->bit_mask & ICF_TAG_LIST_PRESENT)
    {
        icf_pack_icf_tag_list_st(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_local_user_alerted_req_st->tag_list), &offset);
        
        *p_length    += offset;
    }

    if(p_icf_local_user_alerted_req_st->bit_mask & ICF_BODY_LIST_PRESENT)
    {
        icf_pack_icf_msg_body_list_st(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_local_user_alerted_req_st->body_list), &offset);
        
        *p_length    += offset;
    }

#ifdef ICF_REL_OLD
    if(p_icf_local_user_alerted_req_st->bit_mask & ICF_STREAM_LIST_PRESENT)
    {
        icf_pack_icf_stream_list_st(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_local_user_alerted_req_st->stream_list), &offset);
        
        *p_length    += offset;
    }
#endif

/*ICF REL 6.0 : multiple m lines*/
	if(p_icf_local_user_alerted_req_st->bit_mask & ICF_STREAM_LIST_PRESENT)
	{
		icf_pack_icf_stream_capab_list_st(p_dest + (*p_length), (p_icf_local_user_alerted_req_st->p_stream_list),  &offset);
	
		*p_length	+= offset;
	}

	if(p_icf_local_user_alerted_req_st->bit_mask & ICF_USER_ALERTED_DELETED_STREAM_PRESENT)
    {
		icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_local_user_alerted_req_st->count), &offset);
        
        *p_length    += offset;

		while((count<p_icf_local_user_alerted_req_st->count) && (count < ICF_MAX_STREAM_PER_CALL))
		{
			icf_pack_icf_stream_list_st(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_local_user_alerted_req_st->deleted_media_streams[count]), &offset);
        
	        *p_length    += offset;

			count++;
		}
    }
 
    if(ICF_USER_ALERTED_EARLY_MEDIA_SUPPORTED & p_icf_local_user_alerted_req_st->bit_mask)
    {
       icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_local_user_alerted_req_st->early_mode),
                                                                   &offset);
       *p_length    += offset;
    } 
   
   if(p_icf_local_user_alerted_req_st->bit_mask & ICF_USER_ALERTED_MEDIA_MODE_PRESENT)
   {
	icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&(\
                   p_icf_local_user_alerted_req_st->media_mode), &offset);
    *p_length    += offset;
   }
   if(ICF_LOCAL_USER_ALERTED_PRACK_VALUE_PRESENT & p_icf_local_user_alerted_req_st->bit_mask)
   {
       icf_pack_U32(p_dest + (*p_length), 
               (icf_uint8_t*)&(p_icf_local_user_alerted_req_st->prack_value), 
               &offset);
       *p_length    += offset;
   }


    return ICF_SUCCESS;
}

icf_uint32_t
icf_pack_icf_call_forwarded_ind_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t    *p_length
)
{
    icf_uint32_t        offset = 0;

    icf_call_forwarded_ind_st    *p_icf_call_forwarded_ind_st = ICF_NULL;
    
    p_icf_call_forwarded_ind_st    = (icf_call_forwarded_ind_st *)p_src;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function packs icf_call_forwarded_ind_st */
    ICF_CLIB_TRACE(("Packing icf_call_forwarded_ind_st\n"));
    
    icf_pack_icf_address_st(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_call_forwarded_ind_st->remote_party_addr), &offset);
    
    *p_length    += offset;

    icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_call_forwarded_ind_st->bit_mask), &offset);
    
    *p_length    += offset;

    if(p_icf_call_forwarded_ind_st->bit_mask & ICF_REMOTE_PARTY_NAME)
    {
        icf_pack_icf_string_st(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_call_forwarded_ind_st->remote_party_name), &offset);
        
        *p_length    += offset;
    }

	icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_call_forwarded_ind_st->call_forwarded_type), &offset);
    
    *p_length    += offset;

    return ICF_SUCCESS;
}


icf_uint32_t
icf_pack_icf_terminate_call_req_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t    *p_length
)
{
    icf_uint32_t        offset = 0;

    icf_terminate_call_req_st    *p_icf_terminate_call_req_st = ICF_NULL;
    
    p_icf_terminate_call_req_st    = (icf_terminate_call_req_st *)p_src;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function packs icf_terminate_call_req_st */
    ICF_CLIB_TRACE(("Packing icf_terminate_call_req_st\n"));
    
    icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_terminate_call_req_st->bit_mask), &offset);
    
    *p_length    += offset;

    icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_terminate_call_req_st->call_clear_reason), &offset);
    
    *p_length    += offset;

    if(p_icf_terminate_call_req_st->bit_mask & ICF_LINE_ID_PRESENT)
    {
        icf_pack_icf_line_id_t(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_terminate_call_req_st->line_id), &offset);
        
        *p_length    += offset;
    } 

    if(p_icf_terminate_call_req_st->bit_mask & ICF_CALL_CLEAR_RESPONSE_CODE_PRESENT)
    {
        icf_pack_U16(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_terminate_call_req_st->response_code), &offset);
   
        *p_length    += offset;

    }

    if(p_icf_terminate_call_req_st->bit_mask & ICF_HDR_LIST_PRESENT)
    {
        icf_pack_icf_header_list_st(p_dest + (*p_length), 
                (icf_uint8_t*)&(p_icf_terminate_call_req_st->header_list), &offset);
        
        *p_length    += offset;
    }

    if(p_icf_terminate_call_req_st->bit_mask & ICF_TAG_LIST_PRESENT)
    {
        icf_pack_icf_tag_list_st(p_dest + (*p_length), 
                (icf_uint8_t*)&(p_icf_terminate_call_req_st->tag_list), &offset);
        
        *p_length    += offset;
    }

    if(p_icf_terminate_call_req_st->bit_mask & ICF_BODY_LIST_PRESENT)
    {
        icf_pack_icf_msg_body_list_st(p_dest + (*p_length), 
                (icf_uint8_t*)&(p_icf_terminate_call_req_st->body_list), &offset);
        
        *p_length    += offset;
    }
    return ICF_SUCCESS;
}

icf_uint32_t
icf_pack_icf_connect_req_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t    *p_length
)
{
    icf_uint32_t        offset = 0;
    icf_uint8_t         count = 0;

    icf_connect_req_st    *p_icf_connect_req_st = ICF_NULL;
    
    p_icf_connect_req_st    = (icf_connect_req_st *)p_src;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function packs icf_connect_req_st */
    ICF_CLIB_TRACE(("Packing icf_connect_req_st\n"));
    
    icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_connect_req_st->bit_mask), &offset);
    
    *p_length    += offset;

    if(p_icf_connect_req_st->bit_mask & ICF_PRIVACY_IND_PRESENT)
    {
        icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_connect_req_st->privacy_ind), &offset);
        
        *p_length    += offset;
    }

    if(p_icf_connect_req_st->bit_mask & ICF_HDR_LIST_PRESENT)
    {
        icf_pack_icf_header_list_st(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_connect_req_st->header_list), &offset);
        
        *p_length    += offset;
    }

    if(p_icf_connect_req_st->bit_mask & ICF_TAG_LIST_PRESENT)
    {
        icf_pack_icf_tag_list_st(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_connect_req_st->tag_list), &offset);
        
        *p_length    += offset;
    }

    if(p_icf_connect_req_st->bit_mask & ICF_BODY_LIST_PRESENT)
    {
        icf_pack_icf_msg_body_list_st(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_connect_req_st->body_list), &offset);
        
        *p_length    += offset;
    }

#ifdef ICF_REL_OLD
    if(p_icf_connect_req_st->bit_mask & ICF_STREAM_LIST_PRESENT)
    {
        icf_pack_icf_stream_list_st(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_connect_req_st->stream_list), &offset);
        
        *p_length    += offset;
    }
#endif

	/*ICF REL 6.0 : multiple m lines*/
	if(p_icf_connect_req_st->bit_mask & ICF_STREAM_LIST_PRESENT)
	{
		icf_pack_icf_stream_capab_list_st(p_dest + (*p_length), (p_icf_connect_req_st->p_stream_list),  &offset);
	
		*p_length	+= offset;
	}

	if(p_icf_connect_req_st->bit_mask & ICF_CONNECT_DELETED_STREAM_PRESENT)
    {
        icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_connect_req_st->count), &offset);
        
        *p_length    += offset;

		while((count<p_icf_connect_req_st->count) && (count < ICF_MAX_STREAM_PER_CALL))
		{
			icf_pack_icf_stream_list_st(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_connect_req_st->deleted_media_streams[count]), &offset);
        
			*p_length    += offset;

			count++;
		}
    }

    return ICF_SUCCESS;
}

icf_uint32_t
icf_pack_icf_subscribe_req_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t    *p_length
)
{
    icf_uint32_t        offset = 0;

    icf_subscribe_req_st    *p_icf_subscribe_req_st = ICF_NULL;
    
    p_icf_subscribe_req_st    = (icf_subscribe_req_st *)p_src;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function packs icf_subscribe_req_st */
    ICF_CLIB_TRACE(("Packing icf_subscribe_req_st\n"));
    
    icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_subscribe_req_st->bit_mask), &offset);
    
    *p_length    += offset;

	icf_pack_icf_line_id_t(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_subscribe_req_st->line_id), &offset);
    
    *p_length    += offset;

    if(p_icf_subscribe_req_st->bit_mask & ICF_SUB_REQ_DEST_INFO_PRESENT)
    {
        icf_pack_icf_address_st(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_subscribe_req_st->destination_info), &offset);
    
        *p_length    += offset;
    }

    if(p_icf_subscribe_req_st->bit_mask & ICF_SUB_REQ_EVENT_PRESENT)
    {
        icf_pack_icf_string_st(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_subscribe_req_st->event_pkg_name), &offset);
    
        *p_length    += offset;
    }

    icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_subscribe_req_st->duration), &offset);
    
    *p_length    += offset;

    if(p_icf_subscribe_req_st->bit_mask & ICF_PREF_IDENTITY_PRESENT)
    {
        icf_pack_icf_address_st(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_subscribe_req_st->pref_identity), &offset);
        
        *p_length    += offset;
    }

    if(p_icf_subscribe_req_st->bit_mask & ICF_HDR_LIST_PRESENT)
    {
        icf_pack_icf_header_list_st(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_subscribe_req_st->header_list), &offset);
        
        *p_length    += offset;
    }

    if(p_icf_subscribe_req_st->bit_mask & ICF_TAG_LIST_PRESENT)
    {
        icf_pack_icf_tag_list_st(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_subscribe_req_st->tag_list), &offset);
        
        *p_length    += offset;
    }

    if(p_icf_subscribe_req_st->bit_mask & ICF_BODY_LIST_PRESENT)
    {
        icf_pack_icf_msg_body_list_st(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_subscribe_req_st->body_list), &offset);
        
        *p_length    += offset;
    }

    /*Fix for SPR: 19999 (Merged CSR 1-7467002 : SPR 19863)*/
    if(p_icf_subscribe_req_st->bit_mask & ICF_SUB_REQ_SUBS_HEAD_START_DURATION_PRESENT)
    {
        icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_subscribe_req_st->subs_head_start_duration), &offset);
    
        *p_length    += offset;
    }

    return ICF_SUCCESS;
}

icf_uint32_t
icf_pack_icf_publish_req_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t    *p_length
)
{
    icf_uint32_t        offset = 0;

    icf_publish_req_st    *p_icf_publish_req_st = ICF_NULL;
    
    p_icf_publish_req_st    = (icf_publish_req_st *)p_src;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function packs icf_publish_req_st */
    ICF_CLIB_TRACE(("Packing icf_publish_req_st\n"));

    icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_publish_req_st->bit_mask), &offset);
    
    *p_length    += offset;

	icf_pack_icf_line_id_t(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_publish_req_st->line_id), &offset);
    
    *p_length    += offset;


    icf_pack_icf_address_st(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_publish_req_st->destination_info), &offset);
    
    *p_length    += offset;

    icf_pack_icf_string_st(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_publish_req_st->event_pkg_name), &offset);
    
    *p_length    += offset;

    icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_publish_req_st->duration), &offset);
    
    *p_length    += offset;
    
    if(p_icf_publish_req_st->bit_mask & ICF_USER_ADDRESS_PRESENT)
    {
        icf_pack_icf_address_st(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_publish_req_st->user_address), &offset);
        
        *p_length    += offset;
    }

    if(p_icf_publish_req_st->bit_mask & ICF_PREF_IDENTITY_PRESENT)
    {
        icf_pack_icf_address_st(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_publish_req_st->pref_identity), &offset);
        
        *p_length    += offset;
    }

    if(p_icf_publish_req_st->bit_mask & ICF_HDR_LIST_PRESENT)
    {
        icf_pack_icf_header_list_st(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_publish_req_st->header_list), &offset);
        
        *p_length    += offset;
    }

    if(p_icf_publish_req_st->bit_mask & ICF_TAG_LIST_PRESENT)
    {
        icf_pack_icf_tag_list_st(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_publish_req_st->tag_list), &offset);
        
        *p_length    += offset;
    }

    if(p_icf_publish_req_st->bit_mask & ICF_BODY_LIST_PRESENT)
    {
        icf_pack_icf_msg_body_list_st(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_publish_req_st->body_list), &offset);
        
        *p_length    += offset;
    }

    return ICF_SUCCESS;
}

icf_uint32_t
icf_pack_icf_message_req_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t    *p_length
)
{
    icf_uint32_t        offset = 0;

    icf_message_req_st    *p_icf_message_req_st = ICF_NULL;
    
    p_icf_message_req_st    = (icf_message_req_st *)p_src;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function packs icf_message_req_st */
    ICF_CLIB_TRACE(("Packing icf_message_req_st\n"));
    
    icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_message_req_st->bit_mask), &offset);
    
    *p_length    += offset;

	icf_pack_icf_line_id_t(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_message_req_st->line_id), &offset);
    
    *p_length    += offset;

    icf_pack_icf_address_st(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_message_req_st->destination_info), &offset);
    
    *p_length    += offset;

     if(p_icf_message_req_st->bit_mask & ICF_USER_ADDRESS_PRESENT)
    {
        icf_pack_icf_address_st(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_message_req_st->user_address), &offset);
        
        *p_length    += offset;
    }

    if(p_icf_message_req_st->bit_mask & ICF_PREF_IDENTITY_PRESENT)
    {
        icf_pack_icf_address_st(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_message_req_st->pref_identity), &offset);
        
        *p_length    += offset;
    }

    if(p_icf_message_req_st->bit_mask & ICF_HDR_LIST_PRESENT)
    {
        icf_pack_icf_header_list_st(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_message_req_st->header_list), &offset);
        
        *p_length    += offset;
    }

    if(p_icf_message_req_st->bit_mask & ICF_TAG_LIST_PRESENT)
    {
        icf_pack_icf_tag_list_st(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_message_req_st->tag_list), &offset);
        
        *p_length    += offset;
    }

    if(p_icf_message_req_st->bit_mask & ICF_BODY_LIST_PRESENT)
    {
        icf_pack_icf_msg_body_list_st(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_message_req_st->body_list), &offset);
        
        *p_length    += offset;
    }

    return ICF_SUCCESS;
}

icf_uint32_t
icf_pack_icf_refer_req_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t    *p_length
)
{
    icf_uint32_t        offset = 0;

    icf_refer_req_st    *p_icf_refer_req_st = ICF_NULL;
    
    p_icf_refer_req_st    = (icf_refer_req_st *)p_src;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function packs icf_refer_req_st */
    ICF_CLIB_TRACE(("Packing icf_refer_req_st\n"));

    icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_refer_req_st->bit_mask), &offset);
    
    *p_length    += offset;

	icf_pack_icf_line_id_t(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_refer_req_st->line_id), &offset);

	*p_length    += offset;

    if(p_icf_refer_req_st->bit_mask & ICF_REF_REQ_DEST_INFO_PRESENT)
    {
        icf_pack_icf_address_st(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_refer_req_st->destination_info), &offset);
    
        *p_length    += offset;
    }

    icf_pack_icf_address_st(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_refer_req_st->refer_to), &offset);
    
    *p_length    += offset;

    if(p_icf_refer_req_st->bit_mask & ICF_REF_REQ_SUBS_REQD_PRESENT)
    {
        icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_refer_req_st->subscription_required), &offset);
        
        *p_length    += offset;
    }

    if(p_icf_refer_req_st->bit_mask & ICF_PREF_IDENTITY_PRESENT)
    {
        icf_pack_icf_address_st(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_refer_req_st->pref_identity), &offset);
        
        *p_length    += offset;
    }

    if(p_icf_refer_req_st->bit_mask & ICF_HDR_LIST_PRESENT)
    {
        icf_pack_icf_header_list_st(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_refer_req_st->header_list), &offset);
        
        *p_length    += offset;
    }

    if(p_icf_refer_req_st->bit_mask & ICF_TAG_LIST_PRESENT)
    {
        icf_pack_icf_tag_list_st(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_refer_req_st->tag_list), &offset);
        
        *p_length    += offset;
    }

    if(p_icf_refer_req_st->bit_mask & ICF_BODY_LIST_PRESENT)
    {
        icf_pack_icf_msg_body_list_st(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_refer_req_st->body_list), &offset);
        
        *p_length    += offset;
    }

    return ICF_SUCCESS;
}

icf_uint32_t
icf_pack_icf_register_cfm_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t    *p_length
)
{
    icf_uint32_t        offset = 0;

    icf_register_cfm_st    *p_icf_register_cfm_st = ICF_NULL;
    
    p_icf_register_cfm_st    = (icf_register_cfm_st *)p_src;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function packs icf_register_cfm_st */
    ICF_CLIB_TRACE(("Packing icf_register_cfm_st\n"));
    
    icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_register_cfm_st->bit_mask), &offset);
    
    *p_length    += offset;

    icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_register_cfm_st->request_type), &offset);
    
    *p_length    += offset;

    icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_register_cfm_st->result), &offset);
    
    *p_length    += offset;

    if(p_icf_register_cfm_st->bit_mask & ICF_REG_CFM_ERROR_CAUSE_PRESENT)
    {
        icf_pack_U16(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_register_cfm_st->error_cause), &offset);
        
        *p_length    += offset;
    }

    if(p_icf_register_cfm_st->bit_mask & ICF_REG_CFM_ASSO_URI_PRESENT)
    {
        icf_pack_icf_pub_uri_list_st(p_dest + (*p_length), p_icf_register_cfm_st->p_uri_list, &offset);
        
        *p_length    += offset;
    }

    icf_pack_icf_line_id_t(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_register_cfm_st->line_id), &offset);
    
    *p_length    += offset;

    if(p_icf_register_cfm_st->bit_mask & ICF_REG_CFM_TO_PRESENT)
    {
        icf_pack_icf_address_st(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_register_cfm_st->to), &offset);
        
        *p_length    += offset;
    }

    if(p_icf_register_cfm_st->bit_mask & ICF_HDR_LIST_PRESENT)
    {
        icf_pack_icf_header_list_st(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_register_cfm_st->header_list), &offset);
        
        *p_length    += offset;
    }

    if(p_icf_register_cfm_st->bit_mask & ICF_BODY_LIST_PRESENT)
    {
        icf_pack_icf_msg_body_list_st(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_register_cfm_st->body_list), &offset);
        
        *p_length    += offset;
    }

    if(p_icf_register_cfm_st->bit_mask & ICF_REG_CFM_CONTACT_LIST_PRESENT)
    {
        icf_pack_icf_header_list_st(p_dest + (*p_length), \
          (icf_uint8_t*)&(p_icf_register_cfm_st->contact_list), &offset);
        
        *p_length    += offset;
    }

    /*Packing response_code*/
    if(p_icf_register_cfm_st->bit_mask & ICF_REG_CFM_RESP_CODE_PRESENT)
    {
        icf_pack_U16(p_dest + (*p_length),\
            (icf_uint8_t*)&(p_icf_register_cfm_st->response_code), &offset);

        *p_length    += offset;
    }


    return ICF_SUCCESS;
}

/* RN*/
icf_uint32_t
icf_pack_icf_register_status_ind_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t    *p_length
)
{
    icf_uint32_t        offset = 0;

    icf_register_status_ind_st    *p_icf_register_status_ind_st = ICF_NULL;
    
    p_icf_register_status_ind_st    = (icf_register_status_ind_st *)p_src;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function packs icf_register_status_ind_st */
    ICF_CLIB_TRACE(("Packing icf_register_status_ind_st\n"));
    
    icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_register_status_ind_st->bit_mask), &offset);
    
    *p_length    += offset;

    icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_register_status_ind_st->request_type), &offset);
    
    *p_length    += offset;

    icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_register_status_ind_st->result), &offset);
    
    *p_length    += offset;

    if(p_icf_register_status_ind_st->bit_mask & ICF_REG_CFM_ERROR_CAUSE_PRESENT)
    {
        icf_pack_U16(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_register_status_ind_st->error_cause), &offset);
        
        *p_length    += offset;
    }

    if(p_icf_register_status_ind_st->bit_mask & ICF_REG_CFM_ASSO_URI_PRESENT)
    {
        icf_pack_icf_pub_uri_list_st(p_dest + (*p_length), p_icf_register_status_ind_st->p_uri_list, &offset);
        
        *p_length    += offset;
    }

    icf_pack_icf_line_id_t(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_register_status_ind_st->line_id), &offset);
    
    *p_length    += offset;

    if(p_icf_register_status_ind_st->bit_mask & ICF_REG_CFM_TO_PRESENT)
    {
        icf_pack_icf_address_st(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_register_status_ind_st->to), &offset);
        
        *p_length    += offset;
    }

    if(p_icf_register_status_ind_st->bit_mask & ICF_HDR_LIST_PRESENT)
    {
        icf_pack_icf_header_list_st(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_register_status_ind_st->header_list), &offset);
        
        *p_length    += offset;
    }

    /********************ICF 8.0***********************/
    
    /* Packing of new field retry-after in register status indication . */
    
    /* This is the duration received in retry-after attribute when the contact of registered
     * public user is terminated with event attribute as probation and
     * retry-after attribute with retry_after_duration.*/
   
    if(p_icf_register_status_ind_st->bit_mask & ICF_REG_IND_RETRY_AFTER_DURATION_PRESENT)
    {
        icf_pack_U32(p_dest + (*p_length),
                   (icf_uint8_t*)&(p_icf_register_status_ind_st->retry_after_duration),&offset);
        *p_length       += offset;
    }   

    if(p_icf_register_status_ind_st->bit_mask & ICF_REG_IND_CONTACT_LIST_PRESENT)
    {
        icf_pack_icf_header_list_st(p_dest + (*p_length),\
         (icf_uint8_t*)&(p_icf_register_status_ind_st->contact_list), &offset);
        
        *p_length    += offset;
    } 


    /*Packing Response code*/
    if(p_icf_register_status_ind_st->bit_mask & ICF_REG_IND_RESP_CODE_PRESENT)
    {
        icf_pack_U16(p_dest + (*p_length),\
            (icf_uint8_t*)&(p_icf_register_status_ind_st->response_code), &offset);

        *p_length    += offset;
    }


    return ICF_SUCCESS;
}

icf_uint32_t
icf_pack_icf_incoming_call_ind_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t    *p_length
)
{
    icf_uint32_t        offset = 0;
    icf_uint8_t         count = 0;

    icf_incoming_call_ind_st    *p_icf_incoming_call_ind_st = ICF_NULL;
    
    p_icf_incoming_call_ind_st    = (icf_incoming_call_ind_st *)p_src;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function packs icf_incoming_call_ind_st */
    ICF_CLIB_TRACE(("Packing icf_incoming_call_ind_st\n"));

    icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_incoming_call_ind_st->bit_mask), &offset);
    
    *p_length    += offset;

    if(p_icf_incoming_call_ind_st->bit_mask & ICF_LINE_ID_PRESENT)
    {
        icf_pack_icf_line_id_t(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_incoming_call_ind_st->line_id), &offset);
        
        *p_length    += offset;
    }

	if(p_icf_incoming_call_ind_st->bit_mask & ICF_TYPE_OF_CALL_IND_PRESENT)
    {
        icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_incoming_call_ind_st->call_type), &offset);
        
        *p_length    += offset;
    }

    if(p_icf_incoming_call_ind_st->bit_mask & ICF_CALL_WAITING_IND_PRESENT)
    {
        icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_incoming_call_ind_st->call_waiting_ind), &offset);
        
        *p_length    += offset;
    }

    if(p_icf_incoming_call_ind_st->bit_mask & ICF_CALLING_ADD_ASRTD_IND_PRESENT)
    {
        icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_incoming_call_ind_st->calling_party_addr_asserted), &offset);
        
        *p_length    += offset;
    }

    if(p_icf_incoming_call_ind_st->bit_mask & ICF_CALLING_PARTY_ADD_PRESENT)
    {
        icf_pack_icf_address_st(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_incoming_call_ind_st->calling_party_addr), &offset);
        
        *p_length    += offset;
    }

    if(p_icf_incoming_call_ind_st->bit_mask & ICF_CALLING_PARTY_NAME_PRESENT)
    {
        icf_pack_icf_string_st(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_incoming_call_ind_st->calling_party_name), &offset);
        
        *p_length    += offset;
    }

    if(p_icf_incoming_call_ind_st->bit_mask & ICF_CALLED_PARTY_ADD_PRESENT)
    {
        icf_pack_icf_address_st(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_incoming_call_ind_st->called_party_addr), &offset);
        
        *p_length    += offset;
    }

    if(p_icf_incoming_call_ind_st->bit_mask & ICF_TRANSF_PARTY_ADD_PRESENT)
    {
        icf_pack_icf_address_st(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_incoming_call_ind_st->transferring_party_addr), &offset);
        
        *p_length    += offset;
    }

    if(p_icf_incoming_call_ind_st->bit_mask & ICF_TRANSF_PARTY_NAME_PRESENT)
    {
        icf_pack_icf_string_st(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_incoming_call_ind_st->transferring_party_name), &offset);
        
        *p_length    += offset;
    }

    if(p_icf_incoming_call_ind_st->bit_mask & ICF_HDR_LIST_PRESENT)
    {
        icf_pack_icf_header_list_st(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_incoming_call_ind_st->header_list), &offset);
        
        *p_length    += offset;
    }

    if(p_icf_incoming_call_ind_st->bit_mask & ICF_BODY_LIST_PRESENT)
    {
        icf_pack_icf_msg_body_list_st(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_incoming_call_ind_st->body_list), &offset);
        
        *p_length    += offset;
    }

#ifdef ICF_REL_OLD
    if(p_icf_incoming_call_ind_st->bit_mask & ICF_STREAM_LIST_PRESENT)
    {
        icf_pack_icf_stream_list_st(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_incoming_call_ind_st->stream_list), &offset);
        
        *p_length    += offset;
    }
#endif

     icf_pack_icf_stream_capab_list_st(p_dest + (*p_length), (p_icf_incoming_call_ind_st->p_accepted_stream_list), &offset);
        
     *p_length    += offset;
    

	if(p_icf_incoming_call_ind_st->bit_mask & ICF_REJECTED_MEDIA_STREAM_PRESENT)
    {
        icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_incoming_call_ind_st->count), &offset);
        
        *p_length    += offset;
		
		while((count<p_icf_incoming_call_ind_st->count) && (count < ICF_MAX_STREAM_PER_CALL))
		{

			icf_pack_icf_stream_list_st(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_incoming_call_ind_st->deleted_stream_list[count]), &offset);
        
			*p_length    += offset;

			count++;
		}
    }

    icf_pack_U16(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_incoming_call_ind_st->call_id_to_be_joined), &offset);
    *p_length       += offset;

    /* For User Param Support, if ICF_REQ_URI_PARAM_PRESENT bitmask is 
     * set, then pack the request_uri_param using the 
     * icf_pack_icf_string_st function */     
    if(p_icf_incoming_call_ind_st->bit_mask & ICF_REQ_URI_PARAM_PRESENT)
    {
       icf_pack_icf_string_st(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_incoming_call_ind_st->request_uri_param), &offset);
       *p_length    += offset;
    }

    icf_pack_U32(p_dest + (*p_length), 
            (icf_uint8_t*)&(p_icf_incoming_call_ind_st->prack_value), 
            &offset);
    *p_length    += offset;

    icf_pack_U8(p_dest + (*p_length), 
            (icf_uint8_t*)&(p_icf_incoming_call_ind_st->precondition_call), 
            &offset);
    *p_length    += offset;

    return ICF_SUCCESS;
}

icf_uint32_t
icf_pack_icf_remote_user_alerted_ind_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t    *p_length
)
{
    icf_uint32_t        offset = 0;
    icf_uint8_t         count = 0;

    icf_remote_user_alerted_ind_st    *p_icf_remote_user_alerted_ind_st = ICF_NULL;
    
    p_icf_remote_user_alerted_ind_st    = (icf_remote_user_alerted_ind_st *)p_src;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function packs icf_remote_user_alerted_ind_st */
    ICF_CLIB_TRACE(("Packing icf_remote_user_alerted_ind_st\n"));
    
    icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_remote_user_alerted_ind_st->bit_mask), &offset);
    
    *p_length    += offset;

    icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_remote_user_alerted_ind_st->inband_info_status), &offset);
    
    *p_length    += offset;


    if(p_icf_remote_user_alerted_ind_st->bit_mask & ICF_HDR_LIST_PRESENT)
    {
        icf_pack_icf_header_list_st(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_remote_user_alerted_ind_st->header_list), &offset);
        
        *p_length    += offset;
    }

    if(p_icf_remote_user_alerted_ind_st->bit_mask & ICF_BODY_LIST_PRESENT)
    {
        icf_pack_icf_msg_body_list_st(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_remote_user_alerted_ind_st->body_list), &offset);
        
        *p_length    += offset;
    }

#ifdef ICF_REL_OLD
    if(p_icf_remote_user_alerted_ind_st->bit_mask & ICF_STREAM_LIST_PRESENT)
    {
        icf_pack_icf_stream_list_st(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_remote_user_alerted_ind_st->stream_list), &offset);
        
        *p_length    += offset;
    }
#endif
	
   icf_pack_icf_stream_capab_list_st(p_dest + (*p_length), (p_icf_remote_user_alerted_ind_st->p_accepted_stream_list), &offset);
        
   *p_length    += offset;
	
	if(p_icf_remote_user_alerted_ind_st->bit_mask & ICF_REM_USER_ALRTED_DELETED_STREAM_PRESENT)
    {
        icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_remote_user_alerted_ind_st->count), &offset);
        
        *p_length    += offset;
		
		while((count<p_icf_remote_user_alerted_ind_st->count) && (count < ICF_MAX_STREAM_PER_CALL))
		{
			icf_pack_icf_stream_list_st(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_remote_user_alerted_ind_st->deleted_media_streams[count]), &offset);
        
			*p_length    += offset;

			count++;

		}
    }
    /* This Boolean will be set to application whenever in outgoing call
     * Prack is enabled/disabled on per call basis
     */
    if(ICF_REM_USER_ALRTED_REL_PROV_RESP_PRESENT & p_icf_remote_user_alerted_ind_st->bit_mask)
    {
        icf_pack_U8(p_dest + (*p_length), 
                (icf_uint8_t*)&(p_icf_remote_user_alerted_ind_st->rel_prov_resp), 
                &offset);
        *p_length    += offset;
    }

    /* CSR 1-8780514 SPR 20517 */
    /* A new boolean is_sdp_received is added to indicate whether
       180 received is with SDP or without SDP */
        icf_pack_U8(p_dest + (*p_length), 
                (icf_uint8_t*)&(p_icf_remote_user_alerted_ind_st->is_sdp_received), 
                &offset);
        *p_length    += offset;

    return ICF_SUCCESS;
}

icf_uint32_t
icf_pack_icf_terminate_call_ind_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t    *p_length
)
{
    icf_uint32_t        offset = 0;

    icf_terminate_call_ind_st    *p_icf_terminate_call_ind_st = ICF_NULL;
    
    p_icf_terminate_call_ind_st    = (icf_terminate_call_ind_st *)p_src;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function packs icf_terminate_call_ind_st */
    ICF_CLIB_TRACE(("Packing icf_terminate_call_ind_st\n"));
    
    icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_terminate_call_ind_st->bit_mask), &offset);
    
    *p_length    += offset;

    icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_terminate_call_ind_st->call_clear_reason), &offset);
    
    *p_length    += offset;

    if(p_icf_terminate_call_ind_st->bit_mask & ICF_ADDNL_INFO_PRESENT)
    {
        icf_pack_icf_string_st(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_terminate_call_ind_st->additional_info), &offset);
        
        *p_length    += offset;
    }

    if(p_icf_terminate_call_ind_st->bit_mask & ICF_REMOTE_CALL_CLEAR_RESPONSE_CODE_PRESENT)
    {
         icf_pack_U16(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_terminate_call_ind_st->response_code), &offset);
   
         *p_length    += offset;
    }

    if(p_icf_terminate_call_ind_st->bit_mask & ICF_RETRY_AFTER_DURATION_PRESENT)
    {
         icf_pack_U32(p_dest + (*p_length), 
            (icf_uint8_t*)&(p_icf_terminate_call_ind_st->retry_after_duration),
             &offset);

        *p_length       += offset; 
    }

   if(p_icf_terminate_call_ind_st->bit_mask & ICF_HDR_LIST_PRESENT)
    {
        icf_pack_icf_header_list_st(p_dest + (*p_length), 
                (icf_uint8_t*)&(p_icf_terminate_call_ind_st->header_list), &offset);
        
        *p_length    += offset;
    }
	
    if(p_icf_terminate_call_ind_st->bit_mask & ICF_BODY_LIST_PRESENT)
    {
        icf_pack_icf_msg_body_list_st(p_dest + (*p_length), 
                (icf_uint8_t*)&(p_icf_terminate_call_ind_st->body_list), &offset);
        
        *p_length    += offset;
    }
    return ICF_SUCCESS;
}

icf_uint32_t
icf_pack_icf_connect_ind_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t    *p_length
)
{
    icf_uint32_t        offset = 0;
    icf_uint8_t         count = 0 ;

    icf_connect_ind_st    *p_icf_connect_ind_st = ICF_NULL;
    
    p_icf_connect_ind_st    = (icf_connect_ind_st *)p_src;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function packs icf_connect_ind_st */
    ICF_CLIB_TRACE(("Packing icf_connect_ind_st\n"));
    
    icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_connect_ind_st->bit_mask), &offset);
    
    *p_length    += offset;

    if(p_icf_connect_ind_st->bit_mask & ICF_ASSERTED_ID_PRESENT)
    {
        icf_pack_icf_address_st(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_connect_ind_st->\
            asserted_identity), &offset);
        
        *p_length    += offset;
    }

    if(p_icf_connect_ind_st->bit_mask & ICF_HDR_LIST_PRESENT)
    {
        icf_pack_icf_header_list_st(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_connect_ind_st->\
            header_list), &offset);
        
        *p_length    += offset;
    }

    if(p_icf_connect_ind_st->bit_mask & ICF_BODY_LIST_PRESENT)
    {
        icf_pack_icf_msg_body_list_st(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_connect_ind_st->\
            body_list), &offset);
        
        *p_length    += offset;
    }

#ifdef ICF_REL_OLD
    if(p_icf_connect_ind_st->bit_mask & ICF_STREAM_LIST_PRESENT)
    {
        icf_pack_icf_stream_list_st(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_connect_ind_st->\
            stream_list), &offset);
        
        *p_length    += offset;
    }
#endif

    icf_pack_icf_call_modify_media_data_list_st(p_dest + (*p_length), (p_icf_connect_ind_st->\
        p_accepted_stream_list), &offset);
        
    *p_length    += offset;
 	
	if(p_icf_connect_ind_st->bit_mask & ICF_CONNECT_IND_DELETED_STREAM_PRESENT)
    {
        icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_connect_ind_st->count), &offset);
        
        *p_length    += offset;

		while((count<p_icf_connect_ind_st->count) && (count < ICF_MAX_STREAM_PER_CALL))
		{
			icf_pack_icf_stream_list_st(p_dest + (*p_length), 
                        (icf_uint8_t*)&(p_icf_connect_ind_st->deleted_media_streams[count]), &offset);
        
			*p_length    += offset;

			count++;
		}
    }

	icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_connect_ind_st->\
            inband_dtmf_supported), &offset);

	*p_length    += offset;

    if(p_icf_connect_ind_st->bit_mask & ICF_CONNECT_IND_OFFER_RECEIVED_PRESENT)
    {

	icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_connect_ind_st->offer_received), &offset);

	*p_length    += offset;

    }


    if(p_icf_connect_ind_st->bit_mask & ICF_CONNECT_IND_TUNNELLED_SDP_PRESENT)
    {
      
        icf_pack_icf_tunnelled_sdp_info_st(p_dest + (*p_length), (icf_uint8_t*)&\
            (p_icf_connect_ind_st->tunnelled_sdp_info), &offset);

        *p_length    += offset;
    }
return ICF_SUCCESS;
}


icf_uint32_t
icf_pack_icf_subscribe_resp_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t    *p_length
)
{
    icf_uint32_t        offset = 0;

    icf_subscription_status_ind_st    *p_icf_subscribe_resp_st = ICF_NULL;
    
    p_icf_subscribe_resp_st    = (icf_subscription_status_ind_st *)p_src;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function packs icf_subscribe_resp_st */
    ICF_CLIB_TRACE(("Packing icf_subscribe_resp_st\n"));
    
    icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_subscribe_resp_st->bit_mask), &offset);
    
    *p_length    += offset;

    icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_subscribe_resp_st->error_code), &offset);
    
    *p_length    += offset;

    icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_subscribe_resp_st->status), &offset);
    
    *p_length    += offset;

    icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_subscribe_resp_st->duration), &offset);
    
    *p_length    += offset;
    

    if(p_icf_subscribe_resp_st->bit_mask & ICF_SUBS_ST_IND_RESP_CODE_PRESENT)
    {
        icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_subscribe_resp_st->response_code), &offset);
        
        *p_length    += offset;
    }

    if(p_icf_subscribe_resp_st->bit_mask & ICF_HDR_LIST_PRESENT)
    {
        icf_pack_icf_header_list_st(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_subscribe_resp_st->header_list), &offset);
        
        *p_length    += offset;
    }
    /* Event package name for which subscribe request was sent */
    icf_pack_icf_string_st(
            p_dest + (*p_length),
            (icf_uint8_t *)&(p_icf_subscribe_resp_st->event_pkg_name),
            &offset);

    *p_length    += offset;

    return(ICF_SUCCESS);
}

icf_uint32_t
icf_pack_icf_notify_ind_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t    *p_length
)
{
    icf_uint32_t        offset = 0;

    icf_notify_ind_st    *p_icf_notify_ind_st = ICF_NULL;
    
    p_icf_notify_ind_st    = (icf_notify_ind_st *)p_src;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function packs icf_notify_ind_st */
    ICF_CLIB_TRACE(("Packing icf_notify_ind_st\n"));
    
    icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_notify_ind_st->bit_mask), &offset);
    
    *p_length    += offset;

	icf_pack_icf_line_id_t(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_notify_ind_st->line_id), &offset);
	
	*p_length	+= offset;

    icf_pack_icf_string_st(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_notify_ind_st->event_pkg_name), &offset);
    
    *p_length    += offset;

/*NOTIFY_OUT_OF_DIALOG_START*/
	icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_notify_ind_st->subs_state), &offset);
    
    *p_length    += offset;

	icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_notify_ind_st->reason_code), &offset);
    
    *p_length    += offset;

	if(p_icf_notify_ind_st->bit_mask & ICF_SUBS_DURATION_PRESENT)
    {
		icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_notify_ind_st->duration), &offset);
    
		*p_length    += offset;
	}

	if(p_icf_notify_ind_st->bit_mask & ICF_NOTIFY_IND_SENDER_PRESENT)
    {
		icf_pack_icf_address_st(p_dest + (*p_length),(icf_uint8_t*)&(p_icf_notify_ind_st->sender), &offset);
    
		*p_length    += offset;
	}

	if(p_icf_notify_ind_st->bit_mask & ICF_NOTIFY_IND_USER_ADDR_PRESENT)
    {
		icf_pack_icf_address_st(p_dest + (*p_length),(icf_uint8_t*)&(p_icf_notify_ind_st->user_address), &offset);
    
		*p_length    += offset;
	}
/*NOTIFY_OUT_OF_DIALOG_END*/

/*    icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_notify_ind_st->error_code), &offset);*/
    
/*    *p_length    += offset;*/

/*    icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_notify_ind_st->duration), &offset);*/
    
/*    *p_length    += offset;*/


    if(p_icf_notify_ind_st->bit_mask & ICF_HDR_LIST_PRESENT)
    {
        icf_pack_icf_header_list_st(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_notify_ind_st->header_list), &offset);
        
        *p_length    += offset;
    }

    if(p_icf_notify_ind_st->bit_mask & ICF_BODY_LIST_PRESENT)
    {
        icf_pack_icf_msg_body_list_st(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_notify_ind_st->body_list), &offset);
        
        *p_length    += offset;
    }

    return(ICF_SUCCESS);
}

icf_uint32_t
icf_pack_icf_publish_resp_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t    *p_length
)
{
    icf_uint32_t        offset = 0;

    icf_publish_resp_st    *p_icf_publish_resp_st = ICF_NULL;
    
    p_icf_publish_resp_st    = (icf_publish_resp_st *)p_src;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function packs icf_publish_resp_st */
    ICF_CLIB_TRACE(("Packing icf_publish_resp_st\n"));
    
    icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_publish_resp_st->bit_mask), &offset);
    
    *p_length    += offset;

    icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_publish_resp_st->result), &offset);
    
    *p_length    += offset;


    if(p_icf_publish_resp_st->bit_mask & ICF_PUB_RESP_ERROR_CODE_PRESENT)
    {
        icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_publish_resp_st->error_code), &offset);
    
		*p_length    += offset;
	}

    icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_publish_resp_st->duration), &offset);
    
    *p_length    += offset;


    if(p_icf_publish_resp_st->bit_mask & ICF_PUB_RESP_RESP_CODE_PRESENT)
    {
        icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_publish_resp_st->response_code), &offset);
        
        *p_length    += offset;
    }

    icf_pack_icf_string_st(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_publish_resp_st->entity_tag), &offset);

    *p_length    += offset;
        
    if(p_icf_publish_resp_st->bit_mask & ICF_HDR_LIST_PRESENT)
    {
        icf_pack_icf_header_list_st(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_publish_resp_st->header_list), &offset);
        
        *p_length    += offset;
    }

    return(ICF_SUCCESS);
}

icf_uint32_t
icf_pack_icf_message_resp_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t    *p_length
)
{
    icf_uint32_t        offset = 0;

    icf_message_resp_st    *p_icf_message_resp_st = ICF_NULL;
    
    p_icf_message_resp_st    = (icf_message_resp_st *)p_src;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function packs icf_message_resp_st */
    ICF_CLIB_TRACE(("Packing icf_message_resp_st\n"));
    
    icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_message_resp_st->bit_mask), &offset);
    
    *p_length    += offset;

    icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_message_resp_st->result), &offset);
    
    *p_length    += offset;

    if(p_icf_message_resp_st->bit_mask & ICF_MSG_RESP_ERROR_CODE_PRESENT)
    {

		icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_message_resp_st->error_code), &offset);

		*p_length    += offset;
	}

    if(p_icf_message_resp_st->bit_mask & ICF_MSG_RESP_RESP_CODE_PRESENT)
    {
        icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_message_resp_st->response_code), &offset);
        
        *p_length    += offset;
    }

    if(p_icf_message_resp_st->bit_mask & ICF_HDR_LIST_PRESENT)
    {
        icf_pack_icf_header_list_st(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_message_resp_st->header_list), &offset);
        
        *p_length    += offset;
    }

    return(ICF_SUCCESS);
}

icf_uint32_t
icf_pack_icf_message_ind_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t    *p_length
)
{
    icf_uint32_t        offset = 0;

    icf_message_ind_st    *p_icf_message_ind_st = ICF_NULL;
    
    p_icf_message_ind_st    = (icf_message_ind_st *)p_src;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function packs icf_message_ind_st */
    ICF_CLIB_TRACE(("Packing icf_message_ind_st\n"));
    
    icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_message_ind_st->bit_mask), &offset);
    
    *p_length    += offset;

    icf_pack_icf_address_st(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_message_ind_st->sender), &offset);
    
    *p_length    += offset;

    if(p_icf_message_ind_st->bit_mask & ICF_HDR_LIST_PRESENT)
    {
        icf_pack_icf_header_list_st(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_message_ind_st->header_list), &offset);
        
        *p_length    += offset;
    }

    if(p_icf_message_ind_st->bit_mask & ICF_BODY_LIST_PRESENT)
    {
        icf_pack_icf_msg_body_list_st(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_message_ind_st->body_list), &offset);
        
        *p_length    += offset;
    }

    return(ICF_SUCCESS);
}

icf_uint32_t
icf_pack_icf_refer_resp_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t    *p_length
)
{
    icf_uint32_t        offset = 0;

    icf_refer_resp_st    *p_icf_refer_resp_st = ICF_NULL;
    
    p_icf_refer_resp_st    = (icf_refer_resp_st *)p_src;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function packs icf_refer_resp_st */
    ICF_CLIB_TRACE(("Packing icf_refer_resp_st\n"));
    
    icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_refer_resp_st->bit_mask), &offset);
    
    *p_length    += offset;

    icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_refer_resp_st->result), &offset);
    
    *p_length    += offset;

    if(p_icf_refer_resp_st->bit_mask & ICF_REF_RESP_RESP_CODE_PRESENT)
    {
        icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_refer_resp_st->response_code), &offset);
        
        *p_length    += offset;
    }

    if(p_icf_refer_resp_st->bit_mask & ICF_REF_RESP_ERROR_CODE_PRESENT)
    {
        icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_refer_resp_st->error_code), &offset);
        
        *p_length    += offset;
    }

    if(p_icf_refer_resp_st->bit_mask & ICF_HDR_LIST_PRESENT)
    {
        icf_pack_icf_header_list_st(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_refer_resp_st->header_list), &offset);
        
        *p_length    += offset;
    }

    return(ICF_SUCCESS);
}


icf_uint32_t
icf_pack_icf_nw_buff_ind_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t    *p_length
)
{
    icf_uint32_t        offset = 0;
    icf_uint32_t        count = 0;
    icf_uint8_t         *p_str = ICF_NULL;

    icf_trace_nw_buff_ind_st    *p_icf_nw_buff_ind_st = ICF_NULL;
    
    p_icf_nw_buff_ind_st    = (icf_trace_nw_buff_ind_st *)p_src;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function packs icf_refer_resp_st */
    ICF_CLIB_TRACE(("Packing icf_nw_buff_ind_st\n"));

    icf_pack_icf_transport_address_st(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_nw_buff_ind_st->nw_address), &offset);
    
    *p_length    += offset;

    icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_nw_buff_ind_st->nw_buffer.len), &offset);
    
    *p_length    += offset;

    count = 0;

    p_str = p_icf_nw_buff_ind_st->nw_buffer.p_buff;
    
    while(count < p_icf_nw_buff_ind_st->nw_buffer.len + 1)
    {
        icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)(p_str++), &offset);
        
        *p_length    += offset;
        
        count++;
    }

    return(ICF_SUCCESS);
}


icf_uint32_t
icf_pack_icf_api_header_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t    *p_length,
	icf_uint32_t	*p_api_length_index
)
{

    icf_uint32_t        offset = 0;

    icf_api_header_st    *p_icf_api_header_st = ICF_NULL;
    
    p_icf_api_header_st    = (icf_api_header_st *)p_src;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function packs icf_api_header_st */
    ICF_CLIB_TRACE(("Packing icf_api_header_st\n"));

    
    icf_pack_U16(p_dest + (*p_length), (icf_uint8_t *)&(p_icf_api_header_st->api_id), &offset);
    
    *p_length    += offset;

    icf_pack_U8(p_dest + (*p_length), (icf_uint8_t *)&(p_icf_api_header_st->version), &offset);
    
    *p_length    += offset;

    icf_pack_U8(p_dest + (*p_length), (icf_uint8_t *)&(p_icf_api_header_st->source_id), &offset);
    
    *p_length    += offset;

    icf_pack_U8(p_dest + (*p_length), (icf_uint8_t *)&(p_icf_api_header_st->destination_id), &offset);
    
    *p_length    += offset;

    icf_pack_U16(p_dest + (*p_length), (icf_uint8_t *)&(p_icf_api_header_st->call_id), &offset);
    
    *p_length    += offset;

    icf_pack_U32(p_dest + (*p_length), (icf_uint8_t *)&(p_icf_api_header_st->app_id), &offset);
    
    *p_length    += offset;

	*p_api_length_index = *p_length;

    icf_pack_U16(p_dest + (*p_length), (icf_uint8_t *)&(p_icf_api_header_st->api_length), &offset);
    
    *p_length    += offset;

    
    
    return ICF_SUCCESS;
}






icf_uint32_t
icf_pack_unknown_st
(
    icf_uint8_t        *p_dest,
    icf_uint8_t        *p_src,
    icf_uint32_t    *p_length
)
{

    icf_uint32_t        offset = 0;
    icf_uint32_t        count = 0;

    
    while(count < *p_length)
    {
        icf_pack_U8((icf_uint8_t*)&(p_dest[count]), (icf_uint8_t*)&(p_src[count]), &offset);

        count++;
    }

    return ICF_SUCCESS;
}




/*-----------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------*/


icf_uint32_t
icf_pack_U8
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t    *p_length
)
{

#if ICF_CLIB_TRACE_LEVEL == 2
    ICF_CLIB_TRACE(("Value of icf_uint8_t = %d\n", *((icf_uint8_t *)p_src)));
#endif
    
    *p_dest = *p_src;
    
    *p_length    = sizeof(icf_uint8_t);
    
    return ICF_SUCCESS;
}


icf_uint32_t
icf_pack_U16
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t    *p_length
)
{
    /* Packing U16 */


    p_dest[0]    = (icf_uint8_t)((*(icf_uint16_t*)p_src) >> 8);
    p_dest[1]    = (icf_uint8_t)((*(icf_uint16_t*)p_src));
    
    *p_length    = sizeof(icf_uint16_t);

#if ICF_CLIB_TRACE_LEVEL == 2
    ICF_CLIB_TRACE(("Value of icf_uint16_t = %d\n", *((icf_uint16_t*)p_src)));
#endif
    
    return ICF_SUCCESS;
}


icf_uint32_t
icf_pack_U32
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t    *p_length
)
{
    /* Packing icf_uint32_t */
    
    p_dest[0]    = (icf_uint8_t)((*(icf_uint32_t*)p_src) >> 24);
    p_dest[1]    = (icf_uint8_t)((*(icf_uint32_t*)p_src) >> 16);
    p_dest[2]    = (icf_uint8_t)((*(icf_uint32_t*)p_src) >> 8);
    p_dest[3]    = (icf_uint8_t)((*(icf_uint32_t*)p_src));
    
    *p_length    = sizeof(icf_uint32_t);

#if ICF_CLIB_TRACE_LEVEL == 2
    ICF_CLIB_TRACE(("Value of icf_uint32_t = %d\n", *((icf_uint32_t*)p_src)));
#endif
    
    return ICF_SUCCESS;
}

/*****************************************************************************/
/*************** Management Plane Handler (MPH) APIs Starts here *************/
/*****************************************************************************/


icf_uint32_t
icf_pack_icf_stretchable_hdr_st
(
	icf_uint8_t		*p_dest,
	icf_uint8_t		*p_src,
	icf_uint32_t	*p_length
)
{
	icf_uint32_t		offset = 0;
	icf_uint32_t		count = 0;

	icf_stretchable_hdr_st	*p_icf_stretchable_hdr_st = ICF_NULL;
	
	p_icf_stretchable_hdr_st	= (icf_stretchable_hdr_st *)p_src;

	/* Initialise length to 0 */
	*p_length	= 0;

	/* This function packs icf_transport_address_st */
	ICF_CLIB_TRACE(("Packing icf_stretchable_hdr_st\n"));

	/*  icf_uint8_t   hdr_len;
		icf_uint8_t  *p_hdr; */

	icf_pack_U8(p_dest + (*p_length), (icf_uint8_t *)&(p_icf_stretchable_hdr_st->hdr_len), &offset);
	*p_length	+= offset;

	/* This piece of code packs a variable length octet string */
   
        /* dead code - comparison is always false due to limited range of data type */
 /*
	if (p_icf_stretchable_hdr_st->hdr_len > ICF_CLIB_MAX_API_LENGTH) {
        ICF_CLIB_TRACE(("\n[ICF_CLIB]: icf_stretchable_hdr_st - Invalid Structure Length (%d) Bytes\n", p_icf_stretchable_hdr_st->hdr_len));
        return ICF_FAILURE;
    }
*/
    ICF_CLIB_TRACE(("\n[ICF_CLIB]: icf_stretchable_hdr_st - Packing Structure of Length (%d) Bytes\n", p_icf_stretchable_hdr_st->hdr_len));

	count = 0;
	while(count < p_icf_stretchable_hdr_st->hdr_len)
	{
		icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_stretchable_hdr_st->p_hdr[count]), &offset);
		
		*p_length	+= offset;
		
		count++;
	}

	return ICF_SUCCESS;
}


icf_uint32_t
icf_pack_icf_set_mgmt_plane_auth_param_req_st
(
	icf_uint8_t	*p_dest,
	icf_uint8_t	*p_src,
	icf_uint32_t	*p_length
)
{
	icf_uint32_t		offset = 0;
	
	icf_set_mgmt_plane_auth_param_req_st	*p_icf_set_mgmt_plane_auth_param_req_st = ICF_NULL;
	p_icf_set_mgmt_plane_auth_param_req_st	= (icf_set_mgmt_plane_auth_param_req_st *)p_src;

	/* Initialise length to 0 */
	*p_length	= 0;
	/* This function packs icf_api_header_st */
	ICF_CLIB_TRACE(("Packing icf_pack_icf_set_mgmt_plane_auth_param_req_st\n"));

	/* #define ICF_MGMT_PLANE_AUTH_DATA_PRESENT 0x01
	icf_uint16_t				bitmask;
	icf_boolean_t				remove_flag;
	icf_uint8_t					server_type;
	icf_transport_address_st	server_addr;
	icf_uint8_t					auth_method;
	icf_string_st				auth_username;
	icf_string_st				auth_password; */

	icf_pack_U16(p_dest + (*p_length), (icf_uint8_t *)&(p_icf_set_mgmt_plane_auth_param_req_st->bitmask), &offset);
	*p_length	+= offset;

	icf_pack_U8(p_dest + (*p_length), (icf_uint8_t *)&(p_icf_set_mgmt_plane_auth_param_req_st->remove_flag), &offset);
	*p_length	+= offset;

	icf_pack_U8(p_dest + (*p_length), (icf_uint8_t *)&(p_icf_set_mgmt_plane_auth_param_req_st->server_type), &offset);
	*p_length	+= offset;

	icf_pack_icf_transport_address_st(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_set_mgmt_plane_auth_param_req_st->server_addr), &offset);
	*p_length	+= offset;

	icf_pack_U8(p_dest + (*p_length), (icf_uint8_t *)&(p_icf_set_mgmt_plane_auth_param_req_st->auth_method), &offset);
	*p_length	+= offset;

	icf_pack_icf_string_st(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_set_mgmt_plane_auth_param_req_st->auth_username), &offset);
	*p_length	+= offset;

	icf_pack_icf_string_st(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_set_mgmt_plane_auth_param_req_st->auth_password), &offset);
	*p_length	+= offset;

	return ICF_SUCCESS;
}


icf_uint32_t
icf_pack_icf_create_secure_conn_req_st
(
	icf_uint8_t	*p_dest,
	icf_uint8_t	*p_src,
	icf_uint32_t	*p_length
)
{
	icf_uint32_t		offset = 0;
	
	icf_create_secure_conn_req_st	*p_icf_create_secure_conn_req_st = ICF_NULL;
	p_icf_create_secure_conn_req_st	= (icf_create_secure_conn_req_st *)p_src;

	/* Initialise length to 0 */
	*p_length	= 0;
	/* This function packs icf_api_header_st */
	ICF_CLIB_TRACE(("Packing icf_create_secure_conn_req_st\n"));

	/*icf_uint16_t				 exchange_id;
#define ICF_XCAP_USAGE	0x01
#define ICF_SYNCML_USAGE 0x02
	icf_uint8_t					 server_type;
	icf_transport_address_st	 server_addr; */

	icf_pack_U16(p_dest + (*p_length), (icf_uint8_t *)&(p_icf_create_secure_conn_req_st->exchange_id), &offset);
	*p_length	+= offset;

	icf_pack_U8(p_dest + (*p_length), (icf_uint8_t *)&(p_icf_create_secure_conn_req_st->server_type), &offset);
	*p_length	+= offset;

	icf_pack_icf_transport_address_st(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_create_secure_conn_req_st->server_addr), &offset);
	*p_length	+= offset;
	
	return ICF_SUCCESS;
}


icf_uint32_t
icf_pack_icf_create_secure_conn_resp_st
(
	icf_uint8_t	*p_dest,
	icf_uint8_t	*p_src,
	icf_uint32_t	*p_length
)
{
	icf_uint32_t		offset = 0;
	
	icf_create_secure_conn_resp_st	*p_icf_create_secure_conn_resp_st = ICF_NULL;
	p_icf_create_secure_conn_resp_st	= (icf_create_secure_conn_resp_st *)p_src;

	/* Initialise length to 0 */
	*p_length	= 0;
	/* This function packs icf_api_header_st */
	ICF_CLIB_TRACE(("Packing icf_create_secure_conn_resp_st\n"));

	/*	icf_uint16_t	exchange_id;
		icf_result_t	conn_req_status;
		icf_uint32_t	conn_handle;
		icf_uint32_t	error_cause; */

	icf_pack_U16(p_dest + (*p_length), (icf_uint8_t *)&(p_icf_create_secure_conn_resp_st->exchange_id), &offset);
	*p_length	+= offset;

	icf_pack_U8(p_dest + (*p_length), (icf_uint8_t *)&(p_icf_create_secure_conn_resp_st->conn_req_status), &offset);
	*p_length	+= offset;

	icf_pack_U32(p_dest + (*p_length), (icf_uint8_t *)&(p_icf_create_secure_conn_resp_st->conn_handle), &offset);
	*p_length	+= offset;

	icf_pack_U32(p_dest + (*p_length), (icf_uint8_t *)&(p_icf_create_secure_conn_resp_st->error_cause), &offset);
	*p_length	+= offset;

	return ICF_SUCCESS;
}


icf_uint32_t
icf_pack_icf_close_secure_conn_req_st
(
	icf_uint8_t		*p_dest,
	icf_uint8_t		*p_src,
	icf_uint32_t	*p_length
)
{
	icf_uint32_t		offset = 0;
	
	icf_close_secure_conn_req_st	*p_icf_close_secure_conn_req_st = ICF_NULL;
	p_icf_close_secure_conn_req_st	= (icf_close_secure_conn_req_st *)p_src;

	/* Initialise length to 0 */
	*p_length	= 0;
	/* This function packs icf_api_header_st */
	ICF_CLIB_TRACE(("Packing icf_close_secure_conn_req_st\n"));

	/*	icf_uint16_t	exchange_id;
		icf_uint32_t	conn_handle; */

	icf_pack_U16(p_dest + (*p_length), (icf_uint8_t *)&(p_icf_close_secure_conn_req_st->exchange_id), &offset);
	*p_length	+= offset;

	icf_pack_U32(p_dest + (*p_length), (icf_uint8_t *)&(p_icf_close_secure_conn_req_st->conn_handle), &offset);
	*p_length	+= offset;
	
	return ICF_SUCCESS;
}


icf_uint32_t
icf_pack_icf_close_secure_conn_resp_st
(
	icf_uint8_t		*p_dest,
	icf_uint8_t		*p_src,
	icf_uint32_t	*p_length
)
{
	icf_uint32_t		offset = 0;
	
	icf_close_secure_conn_resp_st	*p_icf_close_secure_conn_resp_st = ICF_NULL;
	p_icf_close_secure_conn_resp_st	= (icf_close_secure_conn_resp_st *)p_src;

	/* Initialise length to 0 */
	*p_length	= 0;
	/* This function packs icf_api_header_st */
	ICF_CLIB_TRACE(("Packing icf_close_secure_conn_resp_st\n"));

	/*	icf_uint16_t	exchange_id;
		icf_result_t	close_conn_status;
		icf_uint32_t	conn_handle;
		icf_uint32_t	error_cause; */
	
	icf_pack_U16(p_dest + (*p_length), (icf_uint8_t *)&(p_icf_close_secure_conn_resp_st->exchange_id), &offset);
	*p_length	+= offset;

	icf_pack_U8(p_dest + (*p_length), (icf_uint8_t *)&(p_icf_close_secure_conn_resp_st->close_conn_status), &offset);
	*p_length	+= offset;
	
	icf_pack_U32(p_dest + (*p_length), (icf_uint8_t *)&(p_icf_close_secure_conn_resp_st->conn_handle), &offset);
	*p_length	+= offset;
	
	icf_pack_U32(p_dest + (*p_length), (icf_uint8_t *)&(p_icf_close_secure_conn_resp_st->error_cause), &offset);
	*p_length	+= offset;
	  	
	return ICF_SUCCESS;
}


icf_uint32_t
icf_pack_icf_secure_conn_status_ind_st
(
	icf_uint8_t		*p_dest,
	icf_uint8_t		*p_src,
	icf_uint32_t	*p_length
)
{
	icf_uint32_t		offset = 0;
	
	icf_secure_conn_status_ind_st	*p_icf_secure_conn_status_ind_st = ICF_NULL;
	p_icf_secure_conn_status_ind_st	= (icf_secure_conn_status_ind_st *)p_src;

	/* Initialise length to 0 */
	*p_length	= 0;
	/* This function packs icf_api_header_st */
	ICF_CLIB_TRACE(("Packing icf_secure_conn_status_ind_st\n"));

	/*	icf_uint8_t		conn_status;
		icf_uint32_t	conn_handle; 
        icf_uint32_t	old_conn_handle */

	icf_pack_U8(p_dest + (*p_length), (icf_uint8_t *)&(p_icf_secure_conn_status_ind_st->conn_status), &offset);
	*p_length	+= offset;
	
	icf_pack_U32(p_dest + (*p_length), (icf_uint8_t *)&(p_icf_secure_conn_status_ind_st->conn_handle), &offset);
	*p_length	+= offset;
	
    icf_pack_U32(p_dest + (*p_length), (icf_uint8_t *)&(p_icf_secure_conn_status_ind_st->old_conn_handle), &offset);
	*p_length	+= offset;
	
	return ICF_SUCCESS;
}


icf_uint32_t
icf_pack_icf_create_replace_res_req_st
(
	icf_uint8_t		*p_dest,
	icf_uint8_t		*p_src,
	icf_uint32_t	*p_length
)
{
	icf_uint32_t		offset = 0;
	icf_uint32_t		count = 0;
	
	icf_create_replace_res_req_st	*p_icf_create_replace_res_req_st = ICF_NULL;
	p_icf_create_replace_res_req_st	= (icf_create_replace_res_req_st *)p_src;

	/* Initialise length to 0 */
	*p_length	= 0;
	/* This function packs icf_api_header_st */
	ICF_CLIB_TRACE(("Packing icf_create_replace_res_req_st\n"));

	/* 
#define ICF_NO_RESP_DURATION_PRESENT	0x01
#define ICF_ACCEPT_PRESENT				0x02
#define ICF_IF_MATCH_PRESENT			0x04
#define ICF_IF_NONE_MATCH_PRESENT		0x08
#define ICF_ACCEPT_ENCODING_PRESENT		0x10
#define ICF_CONTENT_ENCODING_PRESENT	0x20
#define ICF_HIGH_PRIORITY_REQ			0x40
	icf_uint8_t					bitmask;
	icf_uint16_t				exchange_id;
	icf_uint32_t				conn_handle;
	icf_req_uri_st				request_uri;
	icf_transport_address_st	host_name;
	icf_xcap_res_et				res_type;
	icf_string_st				content_type;
	icf_duration_t				timer_duration;
	icf_stretchable_hdr_st		accept;
	icf_stretchable_hdr_st		if_match;
	icf_stretchable_hdr_st		if_none_match;
	icf_stretchable_hdr_st		accept_encoding;
	icf_stretchable_hdr_st		content_encoding;
	icf_uint32_t				payload_length;
	icf_uint8_t 				*payload; */
	
	icf_pack_U8(p_dest + (*p_length), (icf_uint8_t *)&(p_icf_create_replace_res_req_st->bitmask), &offset);
	*p_length	+= offset;
	
	icf_pack_U16(p_dest + (*p_length), (icf_uint8_t *)&(p_icf_create_replace_res_req_st->exchange_id), &offset);
	*p_length	+= offset;
	
	icf_pack_U32(p_dest + (*p_length), (icf_uint8_t *)&(p_icf_create_replace_res_req_st->conn_handle), &offset);
	*p_length	+= offset;
	
	icf_pack_icf_req_uri_st(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_create_replace_res_req_st->request_uri), &offset);
	*p_length	+= offset;

	icf_pack_icf_transport_address_st(p_dest + (*p_length),	(icf_uint8_t*)&(p_icf_create_replace_res_req_st->host_name), &offset);
	*p_length	+= offset;

	icf_pack_U32(p_dest + (*p_length), (icf_uint8_t *)&(p_icf_create_replace_res_req_st->res_type), &offset);
	*p_length	+= offset;
	
	icf_pack_icf_string_st(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_create_replace_res_req_st->content_type), &offset);
	*p_length	+= offset;

	icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_create_replace_res_req_st->timer_duration), &offset);  
	*p_length	+= offset;

	icf_pack_icf_stretchable_hdr_st(p_dest + (*p_length), (icf_uint8_t *)&(p_icf_create_replace_res_req_st->accept), &offset);
	*p_length	+= offset;

	icf_pack_icf_stretchable_hdr_st(p_dest + (*p_length), (icf_uint8_t *)&(p_icf_create_replace_res_req_st->if_match), &offset);
	*p_length	+= offset;

	icf_pack_icf_stretchable_hdr_st(p_dest + (*p_length), (icf_uint8_t *)&(p_icf_create_replace_res_req_st->if_none_match), &offset);
	*p_length	+= offset;
	
	icf_pack_icf_stretchable_hdr_st(p_dest + (*p_length), (icf_uint8_t *)&(p_icf_create_replace_res_req_st->accept_encoding), &offset);
	*p_length	+= offset;

	icf_pack_icf_stretchable_hdr_st(p_dest + (*p_length), (icf_uint8_t *)&(p_icf_create_replace_res_req_st->content_encoding), &offset);
	*p_length	+= offset;
  	
	icf_pack_U32(p_dest + (*p_length), (icf_uint8_t *)&(p_icf_create_replace_res_req_st->payload_length), &offset);
	*p_length	+= offset;
	
	/* This piece of code packs a variable length octet string */

    if (p_icf_create_replace_res_req_st->payload_length > ICF_CLIB_MAX_API_LENGTH) {
        ICF_CLIB_TRACE(("\n[ICF_CLIB]: icf_pack_icf_create_replace_res_req_st - Invalid Structure Length (%d) Bytes\n", p_icf_create_replace_res_req_st->payload_length));
        return ICF_FAILURE;
    }
    
    ICF_CLIB_TRACE(("\n[ICF_CLIB]: icf_pack_icf_create_replace_res_req_st - Packing Structure of Length (%d) Bytes\n", p_icf_create_replace_res_req_st->payload_length));

	count = 0;
	while(count < p_icf_create_replace_res_req_st->payload_length)
	{
		icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_create_replace_res_req_st->payload[count]), &offset);
		
		*p_length	+= offset;
		
		count++;
	}
	
	return ICF_SUCCESS;
}


icf_uint32_t
icf_pack_icf_delete_res_req_st
(
	icf_uint8_t		*p_dest,
	icf_uint8_t		*p_src,
	icf_uint32_t	*p_length
)
{
	icf_uint32_t		offset = 0;
	
	icf_delete_res_req_st	*p_icf_delete_res_req_st = ICF_NULL;
	p_icf_delete_res_req_st	= (icf_delete_res_req_st *)p_src;

	/* Initialise length to 0 */
	*p_length	= 0;
	/* This function packs icf_api_header_st */
	ICF_CLIB_TRACE(("Packing icf_delete_res_req_st\n"));

	/*  icf_uint8_t					bitmask;
		icf_uint16_t				exchange_id;
		icf_uint32_t				conn_handle;
		icf_req_uri_st				request_uri;
		icf_transport_address_st	host_name;
		icf_xcap_res_et				res_type;
		icf_duration_t				timer_duration;
		icf_stretchable_hdr_st		accept;
		icf_stretchable_hdr_st		if_match;
		icf_stretchable_hdr_st		if_none_match; */
	
	icf_pack_U8(p_dest + (*p_length), (icf_uint8_t *)&(p_icf_delete_res_req_st->bitmask), &offset);
	*p_length	+= offset;
	
	icf_pack_U16(p_dest + (*p_length), (icf_uint8_t *)&(p_icf_delete_res_req_st->exchange_id), &offset);
	*p_length	+= offset;
	
	icf_pack_U32(p_dest + (*p_length), (icf_uint8_t *)&(p_icf_delete_res_req_st->conn_handle), &offset);
	*p_length	+= offset;
	
	icf_pack_icf_req_uri_st(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_delete_res_req_st->request_uri), &offset);
	*p_length	+= offset;

	icf_pack_icf_transport_address_st(p_dest + (*p_length),	(icf_uint8_t*)&(p_icf_delete_res_req_st->host_name), &offset);
	*p_length	+= offset;

	icf_pack_U32(p_dest + (*p_length), (icf_uint8_t *)&(p_icf_delete_res_req_st->res_type), &offset);
	*p_length	+= offset;

	icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_delete_res_req_st->timer_duration), &offset);  
	*p_length	+= offset;

	icf_pack_icf_stretchable_hdr_st(p_dest + (*p_length), (icf_uint8_t *)&(p_icf_delete_res_req_st->accept), &offset);
	*p_length	+= offset;

	icf_pack_icf_stretchable_hdr_st(p_dest + (*p_length), (icf_uint8_t *)&(p_icf_delete_res_req_st->if_match), &offset);
	*p_length	+= offset;

	icf_pack_icf_stretchable_hdr_st(p_dest + (*p_length), (icf_uint8_t *)&(p_icf_delete_res_req_st->if_none_match), &offset);
	*p_length	+= offset;
	  	
	return ICF_SUCCESS;
}


icf_uint32_t
icf_pack_icf_fetch_res_req_st
(
	icf_uint8_t		*p_dest,
	icf_uint8_t		*p_src,
	icf_uint32_t	*p_length
)
{
	icf_uint32_t		offset = 0;
	
	icf_fetch_res_req_st	*p_icf_fetch_res_req_st = ICF_NULL;
	p_icf_fetch_res_req_st	= (icf_fetch_res_req_st *)p_src;

	/* Initialise length to 0 */
	*p_length	= 0;
	/* This function packs icf_api_header_st */
	ICF_CLIB_TRACE(("Packing icf_fetch_res_req_st\n"));

	/*  
ICF_IF_MATCH_PRESENT 
ICF_IF_NONE_MATCH_PRESENT
ICF_NO_RESP_DURATION_PRESENT 
	icf_uint8_t					bitmask;
	icf_uint16_t				exchange_id;
	icf_uint32_t				conn_handle;
	icf_req_uri_st				request_uri;
	icf_transport_address_st	host_name;
	icf_xcap_res_et				res_type;
	icf_duration_t				timer_duration;
	icf_stretchable_hdr_st		if_match;
	icf_stretchable_hdr_st		if_none_match; */
	
	icf_pack_U8(p_dest + (*p_length), (icf_uint8_t *)&(p_icf_fetch_res_req_st->bitmask), &offset);
	*p_length	+= offset;
	
	icf_pack_U16(p_dest + (*p_length), (icf_uint8_t *)&(p_icf_fetch_res_req_st->exchange_id), &offset);
	*p_length	+= offset;
	
	icf_pack_U32(p_dest + (*p_length), (icf_uint8_t *)&(p_icf_fetch_res_req_st->conn_handle), &offset);
	*p_length	+= offset;
	
	icf_pack_icf_req_uri_st(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_fetch_res_req_st->request_uri), &offset);
	*p_length	+= offset;

	icf_pack_icf_transport_address_st(p_dest + (*p_length),	(icf_uint8_t*)&(p_icf_fetch_res_req_st->host_name), &offset);
	*p_length	+= offset;

	icf_pack_U32(p_dest + (*p_length), (icf_uint8_t *)&(p_icf_fetch_res_req_st->res_type), &offset);
	*p_length	+= offset;

	icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_fetch_res_req_st->timer_duration), &offset);  
	*p_length	+= offset;

	icf_pack_icf_stretchable_hdr_st(p_dest + (*p_length), (icf_uint8_t *)&(p_icf_fetch_res_req_st->if_match), &offset);
	*p_length	+= offset;

	icf_pack_icf_stretchable_hdr_st(p_dest + (*p_length), (icf_uint8_t *)&(p_icf_fetch_res_req_st->if_none_match), &offset);
	*p_length	+= offset;
	 	
	return ICF_SUCCESS;
}


icf_uint32_t
icf_pack_icf_syncml_send_req_st
(
	icf_uint8_t		*p_dest,
	icf_uint8_t		*p_src,
	icf_uint32_t	*p_length
)
{
	icf_uint32_t		offset = 0;
	icf_uint32_t		count= 0;
	
	icf_syncml_send_req_st	*p_icf_syncml_send_req_st = ICF_NULL;
	p_icf_syncml_send_req_st	= (icf_syncml_send_req_st *)p_src;

	/* Initialise length to 0 */
	*p_length	= 0;
	/* This function packs icf_api_header_st */
	ICF_CLIB_TRACE(("Packing icf_syncml_send_req_st\n"));

	/*  
#define ICF_HEADER_LIST_PRESENT 0x80
	icf_uint8_t		bitmask;
	icf_uint16_t	exchange_id;
	icf_uint32_t	conn_handle;
	icf_string_st	content_type;
	icf_string_st	cache_control;
	icf_string_st	transfer_encoding;
	icf_string_st	user_agent;
	icf_string_st	accept;
	icf_string_st	accept_char;
	icf_duration_t	timer_duration;
	icf_uint32_t		payload_length;
    icf_header_list_st  header_list;
	icf_uint8_t 	*payload; */
	
	icf_pack_U8(p_dest + (*p_length), (icf_uint8_t *)&(p_icf_syncml_send_req_st->bitmask), &offset);
	*p_length	+= offset;
	
	icf_pack_U16(p_dest + (*p_length), (icf_uint8_t *)&(p_icf_syncml_send_req_st->exchange_id), &offset);
	*p_length	+= offset;
	
	icf_pack_U32(p_dest + (*p_length), (icf_uint8_t *)&(p_icf_syncml_send_req_st->conn_handle), &offset);
	*p_length	+= offset;
	
	icf_pack_icf_string_st(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_syncml_send_req_st->content_type), &offset);
	*p_length	+= offset;

	icf_pack_icf_string_st(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_syncml_send_req_st->cache_control), &offset);
	*p_length	+= offset;

	icf_pack_icf_string_st(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_syncml_send_req_st->transfer_encoding), &offset);
	*p_length	+= offset;

	icf_pack_icf_string_st(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_syncml_send_req_st->user_agent), &offset);
	*p_length	+= offset;

	icf_pack_icf_string_st(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_syncml_send_req_st->accept), &offset);
	*p_length	+= offset;

	icf_pack_icf_string_st(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_syncml_send_req_st->accept_char), &offset);
	*p_length	+= offset;

	icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_syncml_send_req_st->timer_duration), &offset);  
	*p_length	+= offset;

	icf_pack_icf_header_list_st(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_syncml_send_req_st->header_list), &offset);
	*p_length	+= offset;

	icf_pack_U32(p_dest + (*p_length), (icf_uint8_t *)&(p_icf_syncml_send_req_st->payload_length), &offset);
	*p_length	+= offset;
	
	/* This piece of code packs a variable length octet string */

    if (p_icf_syncml_send_req_st->payload_length > ICF_CLIB_MAX_API_LENGTH) {
        ICF_CLIB_TRACE(("\n[ICF_CLIB]: p_icf_syncml_send_req_st - Invalid Structure Length (%d) Bytes\n", p_icf_syncml_send_req_st->payload_length));
        return ICF_FAILURE;
    }
    
    ICF_CLIB_TRACE(("\n[ICF_CLIB]: p_icf_syncml_send_req_st - Packing Structure of Length (%d) Bytes\n", p_icf_syncml_send_req_st->payload_length));

	count = 0;
	while(count < p_icf_syncml_send_req_st->payload_length)
	{
		icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_syncml_send_req_st->payload[count]), &offset);
		
		*p_length	+= offset;
		
		count++;
	}

	return ICF_SUCCESS;
}


icf_uint32_t
icf_pack_icf_api_proc_failure_st
(
	icf_uint8_t		*p_dest,
	icf_uint8_t		*p_src,
	icf_uint32_t	*p_length
)
{
	icf_uint32_t		offset = 0;
	
	icf_api_proc_failure_st	*p_icf_api_proc_failure_st = ICF_NULL;
	p_icf_api_proc_failure_st	= (icf_api_proc_failure_st *)p_src;

	/* Initialise length to 0 */
	*p_length	= 0;
	/* This function packs icf_api_header_st */
	ICF_CLIB_TRACE(("Packing icf_api_proc_failure_st\n"));

	/*  icf_uint16_t	exchange_id;
		icf_uint32_t	conn_handle;
		icf_api_id_t	api_id;
		icf_error_t		error_cause;*/
	
	icf_pack_U16(p_dest + (*p_length), (icf_uint8_t *)&(p_icf_api_proc_failure_st->exchange_id), &offset);
	*p_length	+= offset;

	icf_pack_U32(p_dest + (*p_length), (icf_uint8_t *)&(p_icf_api_proc_failure_st->conn_handle), &offset);
	*p_length	+= offset;

	icf_pack_U16(p_dest + (*p_length), (icf_uint8_t *)&(p_icf_api_proc_failure_st->api_id), &offset);
	*p_length	+= offset;

	icf_pack_U16(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_api_proc_failure_st->error_cause), &offset);
    *p_length	+= offset;
	  	
	return ICF_SUCCESS;
}


icf_uint32_t
icf_pack_icf_mgmt_plane_nw_resp_st
(
	icf_uint8_t		*p_dest,
	icf_uint8_t		*p_src,
	icf_uint32_t	*p_length
)
{
	icf_uint32_t		offset = 0;
	icf_uint32_t		count = 0;
	
	icf_mgmt_plane_nw_resp_st	*p_icf_mgmt_plane_nw_resp_st = ICF_NULL;
	p_icf_mgmt_plane_nw_resp_st	= (icf_mgmt_plane_nw_resp_st *)p_src;

	/* Initialise length to 0 */
	*p_length	= 0;
	/* This function packs icf_api_header_st */
	ICF_CLIB_TRACE(("Packing icf_mgmt_plane_nw_resp_st\n"));

	/* 
#define ICF_ENTITY_TAG_PRESENT		0x01
#define ICF_CONTENT_TYPE_PRESENT	0x02
#define ICF_CONNECTION_HEADER_VAL_PRESENT       0x04
	icf_uint8_t				bitmask;
	icf_api_id_t			req_api;
	icf_uint16_t			exchange_id;
	icf_uint32_t			conn_handle;
	icf_stretchable_hdr_st  entity_tag;
	icf_string_st			content_type;
#define ICF_CONN_HDR_VAL_INVALID                    0x00
#define ICF_CONN_HDR_VAL_CLOSE_WITH_REATTEMPT       0x01
#define ICF_CONN_HDR_VAL_CLOSE_WITH_NO_REATTEMPT    0x02
    icf_uint8_t             connection_header_val;
	icf_header_list_st		header_list;
	icf_uint32_t			response_code;
	icf_uint32_t			payload_length;
	icf_uint8_t				*payload; */
	
	icf_pack_U8(p_dest + (*p_length), (icf_uint8_t *)&(p_icf_mgmt_plane_nw_resp_st->bitmask), &offset);
	*p_length	+= offset;
	
	icf_pack_U16(p_dest + (*p_length), (icf_uint8_t *)&(p_icf_mgmt_plane_nw_resp_st->req_api), &offset);
	*p_length	+= offset;
	
	icf_pack_U16(p_dest + (*p_length), (icf_uint8_t *)&(p_icf_mgmt_plane_nw_resp_st->exchange_id), &offset);
	*p_length	+= offset;
	
	icf_pack_U32(p_dest + (*p_length), (icf_uint8_t *)&(p_icf_mgmt_plane_nw_resp_st->conn_handle), &offset);
	*p_length	+= offset;
	
	icf_pack_icf_stretchable_hdr_st(p_dest + (*p_length), (icf_uint8_t *)&(p_icf_mgmt_plane_nw_resp_st->entity_tag), &offset);
	*p_length	+= offset;

	icf_pack_icf_string_st(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_mgmt_plane_nw_resp_st->content_type), &offset);
	*p_length	+= offset;

    icf_pack_U8(p_dest + (*p_length), (icf_uint8_t *)&(p_icf_mgmt_plane_nw_resp_st->connection_status), &offset);
	*p_length	+= offset;

	icf_pack_icf_header_list_st(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_mgmt_plane_nw_resp_st->header_list), &offset);
	*p_length	+= offset;

	icf_pack_U32(p_dest + (*p_length), (icf_uint8_t *)&(p_icf_mgmt_plane_nw_resp_st->response_code), &offset);
	*p_length	+= offset;

	icf_pack_U32(p_dest + (*p_length), (icf_uint8_t *)&(p_icf_mgmt_plane_nw_resp_st->payload_length), &offset);
	*p_length	+= offset;
	
	/* This piece of code packs a variable length octet string */

    if (p_icf_mgmt_plane_nw_resp_st->payload_length > ICF_CLIB_MAX_API_LENGTH) {
        ICF_CLIB_TRACE(("\n[ICF_CLIB]: p_icf_mgmt_plane_nw_resp_st - Invalid Structure Length (%d) Bytes\n", p_icf_mgmt_plane_nw_resp_st->payload_length));
        return ICF_FAILURE;
    }
    
    ICF_CLIB_TRACE(("\n[ICF_CLIB]: p_icf_mgmt_plane_nw_resp_st - Packing Structure of Length (%d) Bytes\n", p_icf_mgmt_plane_nw_resp_st->payload_length));

	count = 0;
	while(count < p_icf_mgmt_plane_nw_resp_st->payload_length)
	{
		icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_mgmt_plane_nw_resp_st->payload[count]), &offset);
		
		*p_length	+= offset;
		
		count++;
	}

	return ICF_SUCCESS;
}

/*****************************************************************************/
/*************** Management Plane Handler (MPH) APIs Ends here ***************/
/*****************************************************************************/

/*ICF_HANDLE_REINVITE_START*/



icf_uint32_t
icf_pack_icf_incoming_call_modify_ind_st
(
	icf_uint8_t		*p_dest,
	icf_uint8_t		*p_src,
	icf_uint32_t	*p_length
)
{
	icf_uint32_t		offset = 0;
	icf_uint8_t	    	count = 0;

	icf_incoming_call_modify_ind_st	*p_icf_reinvite_ind_st = ICF_NULL;
	
	p_icf_reinvite_ind_st	= (icf_incoming_call_modify_ind_st *)p_src;

	/* Initialise length to 0 */
	*p_length	= 0;

	/* This function packs icf_incoming_call_ind_st */
	ICF_CLIB_TRACE(("Packing icf_reinvite_ind_st\n"));

	icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_reinvite_ind_st->bit_mask), &offset);
	
	*p_length	+= offset;

    icf_pack_U8(p_dest + (*p_length), (icf_uint8_t *)&(p_icf_reinvite_ind_st->confirm_required),  &offset);
	
	*p_length	+= offset;


	if(p_icf_reinvite_ind_st->bit_mask & ICF_HDR_LIST_PRESENT)
	{
		icf_pack_icf_header_list_st(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_reinvite_ind_st->header_list), &offset);
		
		*p_length	+= offset;
	}

	if(p_icf_reinvite_ind_st->bit_mask & ICF_BODY_LIST_PRESENT)
	{
		icf_pack_icf_msg_body_list_st(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_reinvite_ind_st->body_list), &offset);
		
		*p_length	+= offset;
	}

#ifdef ICF_REL_OLD
	if(p_icf_reinvite_ind_st->bit_mask & ICF_STREAM_LIST_PRESENT)
	{
		icf_pack_icf_stream_list_st(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_reinvite_ind_st->stream_list), &offset);
		
		*p_length	+= offset;
	}
#endif

	if(p_icf_reinvite_ind_st->bit_mask & ICF_INC_CALL_ACCEPT_STREAM_LIST_PRESENT)
    {
        icf_pack_icf_call_modify_media_data_list_st(p_dest + (*p_length), (p_icf_reinvite_ind_st->p_active_stream_list), &offset);
        
        *p_length    += offset;
    }
	
	if(p_icf_reinvite_ind_st->bit_mask & ICF_INC_CALL_MOD_DELETED_STREAM_PRESENT)
    {
        icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_reinvite_ind_st->count), &offset);
        
        *p_length    += offset;
		
		while((count<p_icf_reinvite_ind_st->count) && (count < ICF_MAX_STREAM_PER_CALL))
		{
			icf_pack_icf_stream_list_st(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_reinvite_ind_st->deleted_stream_list[count]), &offset);
        
			*p_length    += offset;
		
			count++;
		}
    }


    if(p_icf_reinvite_ind_st->bit_mask & ICF_INC_CALL_MOD_OFFER_REQUIRED_PRESENT)
    {
        
        icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_reinvite_ind_st->offer_required), &offset);
        
        *p_length    += offset;
    }


    if(ICF_INC_CALL_MOD_SETTINGS_PRESENT & p_icf_reinvite_ind_st->bit_mask)
    {
        
        icf_pack_U32(p_dest + (*p_length), 
                (icf_uint8_t*)&(p_icf_reinvite_ind_st->call_modify_settings), &offset);
        
        *p_length    += offset;
    }

	return ICF_SUCCESS;
}



icf_uint32_t
icf_pack_icf_call_modify_cfm_st
(
	icf_uint8_t		*p_dest,
	icf_uint8_t		*p_src,
	icf_uint32_t	*p_length
)
{
	icf_uint32_t		offset = 0;
        icf_uint8_t         count = 0;

	icf_call_modify_cfm_st	*p_icf_reinvite_cfm_st = ICF_NULL;
	
	p_icf_reinvite_cfm_st	= (icf_call_modify_cfm_st *)p_src;

	/* Initialise length to 0 */
	*p_length	= 0;

	/* This function packs icf_call_modify_cfm_st */
	ICF_CLIB_TRACE(("Packing icf_call_modify_cfm_st\n"));



	icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_reinvite_cfm_st->bit_mask), &offset);
	
	*p_length	+= offset;


	icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_reinvite_cfm_st->result), &offset);
	
	*p_length	+= offset;

	icf_pack_U16(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_reinvite_cfm_st->cause), &offset);
	
	*p_length	+= offset;

	if(p_icf_reinvite_cfm_st->bit_mask & ICF_HDR_LIST_PRESENT)
	{
		icf_pack_icf_header_list_st(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_reinvite_cfm_st->header_list), &offset);
		
		*p_length	+= offset;
	}

	if(p_icf_reinvite_cfm_st->bit_mask & ICF_TAG_LIST_PRESENT)
	{
		icf_pack_icf_tag_list_st(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_reinvite_cfm_st->tag_list), &offset);
		
		*p_length	+= offset;
	}

	if(p_icf_reinvite_cfm_st->bit_mask & ICF_BODY_LIST_PRESENT)
	{
		icf_pack_icf_msg_body_list_st(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_reinvite_cfm_st->body_list), &offset);
		
		*p_length	+= offset;
	}

#ifdef ICF_REL_OLD
	if(p_icf_reinvite_cfm_st->bit_mask & ICF_STREAM_LIST_PRESENT)
	{
		icf_pack_icf_stream_list_st(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_reinvite_cfm_st->stream_list), &offset);
		
		*p_length	+= offset;
	}
#endif
	
/*	if(p_icf_reinvite_cfm_st->bit_mask & ICF_CALL_MOD_CFM_DELETED_STREAM_PRESENT)
    {
        icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_reinvite_cfm_st->count), &offset);
	
		*p_length	+= offset;

		while(count<p_icf_reinvite_cfm_st->count)
		{
			icf_pack_icf_stream_list_st(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_reinvite_cfm_st->deleted_media_streams[count]), &offset);
        
			*p_length    += offset;

			count++;
		}
    }*/


    if(p_icf_reinvite_cfm_st->bit_mask & ICF_CALL_MODIFY_RESPONSE_CODE_PRESENT)
    {
        icf_pack_U16(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_reinvite_cfm_st->nw_response_code), &offset);
        *p_length    += offset;
    }

	if(p_icf_reinvite_cfm_st->bit_mask & ICF_CALL_MODIFY_CFM_REJECTED_STREAM_PRESENT)
    {
		icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_reinvite_cfm_st->count), &offset);
        
        *p_length    += offset;

        while((count<p_icf_reinvite_cfm_st->count) && (count < ICF_MAX_STREAM_PER_CALL))
		{
			icf_pack_icf_stream_list_st(p_dest + (*p_length), 
                 (icf_uint8_t*)&(p_icf_reinvite_cfm_st->deleted_stream_list[count]), &offset);
        
			*p_length    += offset;

			count++;
		}
    }

    if(p_icf_reinvite_cfm_st->bit_mask & ICF_CALL_MODIFY_RESPONSE_SIGNALING_MODE_PRESENT)
    {
        icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_reinvite_cfm_st->signaling_mode), &offset);
        *p_length    += offset;
    }

	return ICF_SUCCESS;
}

icf_uint32_t
icf_pack_icf_call_modify_resp_st
(
	icf_uint8_t		*p_dest,
	icf_uint8_t		*p_src,
	icf_uint32_t	*p_length
)
{
	icf_uint32_t		offset = 0;
	icf_uint8_t			count = 0;

	icf_call_modify_resp_st	*p_icf_reinvite_resp_st = ICF_NULL;
	
	p_icf_reinvite_resp_st	= (icf_call_modify_resp_st *)p_src;

	/* Initialise length to 0 */
	*p_length	= 0;

	/* This function packs icf_call_modify_cfm_st */
	ICF_CLIB_TRACE(("Packing icf_call_modify_cfm_st\n"));



	icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_reinvite_resp_st->bit_mask), &offset);
	
	*p_length	+= offset;


	icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_reinvite_resp_st->result), &offset);
	
	*p_length	+= offset;

	icf_pack_U16(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_reinvite_resp_st->error_cause), &offset);
	
	*p_length	+= offset;

	if(p_icf_reinvite_resp_st->bit_mask & ICF_RESPONSE_CODE_PRESENT)
	{
		icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_reinvite_resp_st->response_code), &offset);
		
		*p_length	+= offset;
	}

	if(p_icf_reinvite_resp_st->bit_mask & ICF_HDR_LIST_PRESENT)
	{
		icf_pack_icf_header_list_st(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_reinvite_resp_st->header_list), &offset);
		
		*p_length	+= offset;
	}

	if(p_icf_reinvite_resp_st->bit_mask & ICF_BODY_LIST_PRESENT)
	{
		icf_pack_icf_msg_body_list_st(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_reinvite_resp_st->body_list), &offset);
		
		*p_length	+= offset;
	}

#ifdef ICF_REL_OLD
	if(p_icf_reinvite_resp_st->bit_mask & ICF_STREAM_LIST_PRESENT)
	{
		icf_pack_icf_stream_list_st(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_reinvite_resp_st->stream_list), &offset);
		
		*p_length	+= offset;
	}
#endif

	if(p_icf_reinvite_resp_st->bit_mask & ICF_CALL_MOD_ACCEPT_STREAM_LIST_PRESENT)
    {
        icf_pack_icf_call_modify_media_data_list_st(p_dest + (*p_length), (p_icf_reinvite_resp_st->p_active_stream_list), &offset);
        
        *p_length    += offset;
    }
	
	if(p_icf_reinvite_resp_st->bit_mask & ICF_CALL_MOD_RESP_REJECT_STREAM_PRESENT)
    {
		icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_reinvite_resp_st->count), &offset);
        
        *p_length    += offset;

        while((count<p_icf_reinvite_resp_st->count) && (count < ICF_MAX_STREAM_PER_CALL))
		{
			icf_pack_icf_stream_list_st(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_reinvite_resp_st->deleted_stream_list[count]), &offset);
        
			*p_length    += offset;

			count++;
		}
    }


    if(p_icf_reinvite_resp_st->bit_mask & ICF_RETRY_AFTER_DURATION_PRESENT)
    {
        icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&
           (p_icf_reinvite_resp_st->retry_after_duration), &offset);

	 *p_length    += offset;
         
    }

    return ICF_SUCCESS;
}

icf_uint32_t
icf_pack_icf_call_modify_req_st
(
	icf_uint8_t		*p_dest,
	icf_uint8_t		*p_src,
	icf_uint32_t	*p_length
)
{
	icf_uint32_t		offset = 0;
	icf_uint32_t		count = 0;

	icf_call_modify_req_st	*p_icf_reinvite_req_st = ICF_NULL;
	
	p_icf_reinvite_req_st	= (icf_call_modify_req_st *)p_src;

	/* Initialise length to 0 */
	*p_length	= 0;

	/* This function packs icf_call_modify_req_st */
	ICF_CLIB_TRACE(("Packing icf_call_modify_req_st\n"));



	icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_reinvite_req_st->bit_mask), &offset);
	
	*p_length	+= offset;

#ifdef ICF_REL_OLD
	if(p_icf_reinvite_req_st->bit_mask & ICF_CALL_MODIFY_MEDIA_MODE_PRESENT)
	{
		icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_reinvite_req_st->media_mode), &offset);
		*p_length	+= offset;
	}
#endif

	if(p_icf_reinvite_req_st->bit_mask & ICF_HDR_LIST_PRESENT)
	{
		icf_pack_icf_header_list_st(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_reinvite_req_st->header_list), &offset);
		
		*p_length	+= offset;
	}

	if(p_icf_reinvite_req_st->bit_mask & ICF_TAG_LIST_PRESENT)
	{
		icf_pack_icf_tag_list_st(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_reinvite_req_st->tag_list), &offset);
		
		*p_length	+= offset;
	}

	if(p_icf_reinvite_req_st->bit_mask & ICF_BODY_LIST_PRESENT)
	{
		icf_pack_icf_msg_body_list_st(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_reinvite_req_st->body_list), &offset);
		
		*p_length	+= offset;
	}

#ifdef ICF_REL_OLD
	if(p_icf_reinvite_req_st->bit_mask & ICF_STREAM_LIST_PRESENT)
	{
		icf_pack_icf_stream_list_st(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_reinvite_req_st->stream_list), &offset);
		
		*p_length	+= offset;
	}
#endif

	if(p_icf_reinvite_req_st->bit_mask & ICF_CALL_MOD_REQ_MEDIA_MOD_LIST_PRESENT)
    {
        icf_pack_icf_call_modify_media_data_list_st(p_dest + (*p_length), (p_icf_reinvite_req_st->p_media_modify_list), &offset);
        
        *p_length    += offset;
    }
	
	if(p_icf_reinvite_req_st->bit_mask & ICF_CALL_MOD_REQ_DELETED_STREAM_PRESENT)
    {
        icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_reinvite_req_st->count), &offset);
        
        *p_length    += offset;

		while((count<p_icf_reinvite_req_st->count) && (count < ICF_MAX_STREAM_PER_CALL))
		{
			icf_pack_icf_stream_list_st(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_reinvite_req_st->deleted_media_streams[count]), &offset);
        
			*p_length    += offset;

			count++;
		}
    }
    /* Rel 8.1 
     * Packing of new field to_tag
     */
    if(ICF_CALL_MOD_REQ_TO_TAG_PRESENT & p_icf_reinvite_req_st->bit_mask)
    {
        icf_pack_icf_string_st(p_dest + (*p_length),
        (icf_uint8_t*)&(p_icf_reinvite_req_st->to_tag), &offset);        
        *p_length    += offset;
    }
    if(ICF_CALL_MOD_REQ_TUNNELLED_SDP_PRESENT & p_icf_reinvite_req_st->bit_mask)
    {
        icf_pack_U8(p_dest + (*p_length),
                   (icf_uint8_t*)&(p_icf_reinvite_req_st->tunnelled_sdp_present),
                   &offset);
       *p_length    += offset;
    }


	return ICF_SUCCESS;
}

icf_uint32_t
icf_pack_icf_incoming_call_modify_cancel_ind_st
(
	icf_uint8_t		*p_dest,
	icf_uint8_t		*p_src,
	icf_uint32_t	*p_length
)
{
   /* To remove warnings */
    p_dest=p_dest;
    p_src=p_src;
    p_length=p_length;
	return ICF_SUCCESS;
}

/*ICF_HANDLE_REINVITE_END*/
#ifdef ICF_SERVICE_ENABLE


icf_uint32_t

icf_pack_icf_set_service_params_req_st

(

    icf_uint8_t    *p_dest,

    icf_uint8_t    *p_src,

    icf_uint32_t    *p_length

)

{

    icf_uint32_t        offset = 0;



    icf_set_service_params_req_st    *p_icf_set_service_params_req_st = ICF_NULL;

    

    p_icf_set_service_params_req_st    = (icf_set_service_params_req_st *)p_src;



    /* Initialise length to 0 */

    *p_length    = 0;



    /* This function packs icf_refer_req_st */

    ICF_CLIB_TRACE(("Packing p_icf_set_service_params_req_st\n"));



    icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_set_service_params_req_st->bit_mask), &offset);

    *p_length    += offset;



    if(p_icf_set_service_params_req_st->bit_mask & ICF_SET_SER_PRM_SUBSCRIPTION_FLAG)

	{

		icf_pack_icf_config_status_st(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_set_service_params_req_st->service_subscription_flag), &offset);

		*p_length	+= offset;

	}



    if(p_icf_set_service_params_req_st->bit_mask & ICF_SET_SER_PRM_ACTIVATION_FLAG)

	{

		icf_pack_icf_config_status_st(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_set_service_params_req_st->service_activation_flag), &offset);

		*p_length	+= offset;

	}



	if(p_icf_set_service_params_req_st->bit_mask & ICF_SET_SER_PRM_CALL_HOLD_OPTION)

	{

		/*icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_set_service_params_req_st->call_hold_option), &offset);*/
        icf_pack_icf_hold_option_st(p_dest + (*p_length), 
                                    (icf_uint8_t*)&(p_icf_set_service_params_req_st->call_hold_option), 
                                    &offset);

		*p_length += offset;

	}



    if(p_icf_set_service_params_req_st->bit_mask & ICF_SET_SER_PRM_MAX_NUM_OF_HOPS)

	{

		icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_set_service_params_req_st->max_num_of_call_forwarding_hops), &offset);

		*p_length += offset;

	}



    if(p_icf_set_service_params_req_st->bit_mask & ICF_SET_SER_PRM_MAX_CALL_WAITING)

	{

		icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_set_service_params_req_st->max_num_of_calls_in_call_waiting), &offset);

		*p_length += offset;

	}



    if(p_icf_set_service_params_req_st->bit_mask & ICF_SET_SER_PRM_CFU_ADDR)

	{

		icf_pack_icf_config_address_st(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_set_service_params_req_st->addr_for_cfu), &offset);

		*p_length	+= offset;

	}



    if(p_icf_set_service_params_req_st->bit_mask & ICF_SET_SER_PRM_CFB_ADDR)

	{

		icf_pack_icf_config_address_st(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_set_service_params_req_st->addr_for_cfb), &offset);

		*p_length	+= offset;

	}



    if(p_icf_set_service_params_req_st->bit_mask & ICF_SET_SER_PRM_CFD_ADDR)

	{

		icf_pack_icf_config_address_st(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_set_service_params_req_st->addr_for_cfd), &offset);

		*p_length	+= offset;

	}



    if(p_icf_set_service_params_req_st->bit_mask & ICF_SET_SER_PRM_CHR_TIMER)

    {

        icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_set_service_params_req_st->call_hold_resume_timer), &offset);

        *p_length    += offset;

    }



    if(p_icf_set_service_params_req_st->bit_mask & ICF_SET_SER_PRM_REINVITE_RACE_TIMER)

    {

        icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_set_service_params_req_st->reinvite_race_timer), &offset);

        *p_length    += offset;

    }



    if(p_icf_set_service_params_req_st->bit_mask & ICF_SET_SER_PRM_TWC_TIMER)

    {

        icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_set_service_params_req_st->twc_timer), &offset);

        *p_length    += offset;

    }

    

	if(p_icf_set_service_params_req_st->bit_mask & ICF_SET_SER_PRM_CTU_TIMER)

    {

        icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_set_service_params_req_st->ctu_timer), &offset);

        *p_length    += offset;

    }

    

	if(p_icf_set_service_params_req_st->bit_mask & ICF_SET_SER_PRM_CTA_TIMER)

    {

        icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_set_service_params_req_st->cta_timer), &offset);

        *p_length    += offset;

    }

    

	if(p_icf_set_service_params_req_st->bit_mask & ICF_SET_SER_PRM_RNW_TIMER)

    {

        icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_set_service_params_req_st->rnw_timer), &offset);

        *p_length    += offset;

    }



	if(p_icf_set_service_params_req_st->bit_mask & ICF_SET_SER_PRM_CW_ALERT_TIMER)

    {

        icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_set_service_params_req_st->cw_alert_timer), &offset);

        *p_length    += offset;

    }



	if(p_icf_set_service_params_req_st->bit_mask & ICF_SET_SER_PRM_CND_ALERT_TIMER)

    {

        icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_set_service_params_req_st->cnd_alert_timer), &offset);

        *p_length    += offset;

    }

	

	if(p_icf_set_service_params_req_st->bit_mask & ICF_SET_SER_PRM_CW_N_CND_ALERT_TIMER)

    {

        icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_set_service_params_req_st->cw_n_cnd_alert_timer), &offset);

        *p_length    += offset;

    }

	

	if(p_icf_set_service_params_req_st->bit_mask & ICF_SET_SER_PRM_SUBS_RESP_TIMER)

    {

        icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_set_service_params_req_st->subs_resp_timer), &offset);

        *p_length    += offset;

    }



	if(p_icf_set_service_params_req_st->bit_mask & ICF_SET_SER_PRM_SUBS_RETRY_TIMER)

    {

        icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_set_service_params_req_st->subs_retry_timer), &offset);

        *p_length    += offset;

    }

     if(p_icf_set_service_params_req_st->bit_mask & ICF_SET_SER_PRM_CONF_REQD_AT_TRANFEREE)
     {
         icf_pack_U8(p_dest + (*p_length),
                 (icf_uint8_t*)&(p_icf_set_service_params_req_st->conf_reqd_at_transferee), &offset);
         *p_length += offset;
     }

     if(p_icf_set_service_params_req_st->bit_mask & ICF_SET_SER_PRM_CONF_CFM_REQD_AT_TRANSFEREE)
     {
         icf_pack_U8(p_dest + (*p_length),
                 (icf_uint8_t*)&(p_icf_set_service_params_req_st->conf_cfm_reqd_at_transferee),
                 &offset);
         *p_length += offset;
     }
    if(p_icf_set_service_params_req_st->bit_mask & 
          ICF_SET_SER_PRM_CONFERENCE_URI_PRESENT)
	{
		icf_pack_icf_config_address_st(p_dest + (*p_length), 
         (icf_uint8_t*)&(p_icf_set_service_params_req_st->conference_uri),
         &offset);
		*p_length	+= offset;
	}
	if(p_icf_set_service_params_req_st->bit_mask & 
        ICF_SET_SER_PRM_SUBSCRIPTION_REQD_IN_EXT_CONF)
    {
        icf_pack_U32(p_dest + (*p_length), 
        (icf_uint8_t*)&(p_icf_set_service_params_req_st->xconf_subs_duration),
          &offset);
        *p_length    += offset;
    }   

    if(p_icf_set_service_params_req_st->bit_mask & 
                           ICF_SET_SER_PRM_CONF_FAIL_CALL_CLEAR_NOT_REQD)
	{
			icf_pack_U8(p_dest + (*p_length),
				(icf_uint8_t*)&(p_icf_set_service_params_req_st->conf_fail_call_clear_not_reqd),
							&offset);
			*p_length += offset;
   }

    return ICF_SUCCESS;

}


icf_uint32_t
icf_pack_icf_cta_req_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t    *p_length
)
{
    icf_uint32_t        offset = 0;

    icf_call_transfer_attended_req_st    *p_icf_cta_req_st = ICF_NULL;
    
    p_icf_cta_req_st    = (icf_call_transfer_attended_req_st *)p_src;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function packs icf_refer_req_st */
    ICF_CLIB_TRACE(("Packing p_icf_cta_req_st\n"));

    icf_pack_U16(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_cta_req_st->transferred_party_id), &offset);
        
    *p_length    += offset;

    icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_cta_req_st->bit_mask), &offset);
    
    *p_length    += offset;

    if(p_icf_cta_req_st->bit_mask & ICF_HDR_LIST_PRESENT)
	{
		icf_pack_icf_header_list_st(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_cta_req_st->header_list), &offset);
		
		*p_length	+= offset;
	}

	if(p_icf_cta_req_st->bit_mask & ICF_TAG_LIST_PRESENT)
	{
		icf_pack_icf_tag_list_st(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_cta_req_st->tag_list), &offset);
		
		*p_length	+= offset;
	}

	if(p_icf_cta_req_st->bit_mask & ICF_BODY_LIST_PRESENT)
	{
		icf_pack_icf_msg_body_list_st(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_cta_req_st->body_list), &offset);
		
		*p_length	+= offset;
	}

    return ICF_SUCCESS;
}

icf_uint32_t
icf_pack_icf_ctu_req_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t    *p_length
)
{
    icf_uint32_t        offset = 0;

    icf_call_transfer_unattended_req_st    *p_icf_ctu_req_st = ICF_NULL;
    
    p_icf_ctu_req_st    = (icf_call_transfer_unattended_req_st *)p_src;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function packs icf_refer_req_st */
    ICF_CLIB_TRACE(("Packing p_icf_ctu_req_st\n"));

    icf_pack_icf_address_st(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_ctu_req_st->transferred_party_addr), &offset);
    
    *p_length    += offset;

    icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_ctu_req_st->bit_mask), &offset);
    
    *p_length    += offset;

    if(p_icf_ctu_req_st->bit_mask & ICF_CALL_TRANSFER_UNATTENDED_REQ_TOGGLE_PRESENTATION_INDICATOR)
    {
        icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_ctu_req_st->toggle_presentation_indicator), &offset);
    
        *p_length    += offset;
    }        

	if(p_icf_ctu_req_st->bit_mask & ICF_HDR_LIST_PRESENT)
	{
		icf_pack_icf_header_list_st(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_ctu_req_st->header_list), &offset);
		
		*p_length	+= offset;
	}

	if(p_icf_ctu_req_st->bit_mask & ICF_TAG_LIST_PRESENT)
	{
		icf_pack_icf_tag_list_st(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_ctu_req_st->tag_list), &offset);
		
		*p_length	+= offset;
	}

	if(p_icf_ctu_req_st->bit_mask & ICF_BODY_LIST_PRESENT)
	{
		icf_pack_icf_msg_body_list_st(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_ctu_req_st->body_list), &offset);
		
		*p_length	+= offset;
	}

    if(p_icf_ctu_req_st->bit_mask & ICF_ADDRESS_ADDNL_URI_PARAMTR_PRES)
	{
        icf_pack_icf_large_string_st(p_dest + (*p_length), (icf_uint8_t*)(p_icf_ctu_req_st->p_uri_parameter), &offset);
    
        *p_length    += offset;
	}

    return ICF_SUCCESS;
}

icf_uint32_t
icf_pack_icf_call_hold_req_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t    *p_length
)
{
    icf_uint32_t        offset = 0;

    icf_call_hold_req_st    *p_icf_chd_req_st = ICF_NULL;
    
    p_icf_chd_req_st    = (icf_call_hold_req_st *)p_src;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function packs icf_refer_req_st */
    ICF_CLIB_TRACE(("Packing icf_call_hold_req_st\n"));

    icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_chd_req_st->bit_mask), &offset);
    
    *p_length    += offset;

   	if(p_icf_chd_req_st->bit_mask & ICF_HDR_LIST_PRESENT)
	{
		icf_pack_icf_header_list_st(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_chd_req_st->header_list), &offset);
		
		*p_length	+= offset;
	}

	if(p_icf_chd_req_st->bit_mask & ICF_TAG_LIST_PRESENT)
	{
		icf_pack_icf_tag_list_st(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_chd_req_st->tag_list), &offset);
		
		*p_length	+= offset;
	}

	if(p_icf_chd_req_st->bit_mask & ICF_BODY_LIST_PRESENT)
	{
		icf_pack_icf_msg_body_list_st(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_chd_req_st->body_list), &offset);
		
		*p_length	+= offset;
	}

    if(p_icf_chd_req_st->bit_mask & ICF_STREAM_LIST_PRESENT)
	{
		icf_pack_icf_stream_id_list_st(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_chd_req_st->stream_list), &offset);
		
		*p_length	+= offset;
	}

    return ICF_SUCCESS;
}

icf_uint32_t
icf_pack_icf_call_resume_req_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t    *p_length
)
{
    icf_uint32_t        offset = 0;

    icf_call_resume_req_st    *p_icf_crm_req_st = ICF_NULL;
    
    p_icf_crm_req_st    = (icf_call_resume_req_st *)p_src;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function packs icf_refer_req_st */
    ICF_CLIB_TRACE(("Packing icf_call_resume_req_st\n"));

    icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_crm_req_st->bit_mask), &offset);
    
    *p_length    += offset;

   	if(p_icf_crm_req_st->bit_mask & ICF_HDR_LIST_PRESENT)
	{
		icf_pack_icf_header_list_st(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_crm_req_st->header_list), &offset);
		
		*p_length	+= offset;
	}

	if(p_icf_crm_req_st->bit_mask & ICF_TAG_LIST_PRESENT)
	{
		icf_pack_icf_tag_list_st(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_crm_req_st->tag_list), &offset);
		
		*p_length	+= offset;
	}

	if(p_icf_crm_req_st->bit_mask & ICF_BODY_LIST_PRESENT)
	{
		icf_pack_icf_msg_body_list_st(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_crm_req_st->body_list), &offset);
		
		*p_length	+= offset;
	}

    if(p_icf_crm_req_st->bit_mask & ICF_STREAM_LIST_PRESENT)
	{
		icf_pack_icf_stream_id_list_st(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_crm_req_st->stream_list), &offset);
		
		*p_length	+= offset;
	}

    return ICF_SUCCESS;
}
#endif

icf_uint32_t
icf_pack_icf_res_unavail_ind_st
(
	icf_uint8_t		*p_dest,
	icf_uint8_t		*p_src,
	icf_uint32_t	*p_length
)
{
	icf_uint32_t		offset = 0;

	icf_res_unavail_ind_st	*p_res_unavail_ind = ICF_NULL;
	
	p_res_unavail_ind	= (icf_res_unavail_ind_st *)p_src;

	*p_length = 0;

	icf_pack_U16(p_dest + (*p_length), (icf_uint8_t*)&(p_res_unavail_ind->req_api_id), &offset);

	*p_length += offset;

	icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&(p_res_unavail_ind->result), &offset);

	*p_length += offset;

	icf_pack_U16(p_dest + (*p_length), (icf_uint8_t*)&(p_res_unavail_ind->cause), &offset);

	*p_length += offset;

	icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&(p_res_unavail_ind->retry_after), &offset);
	
	*p_length +=offset;

	return ICF_SUCCESS;
}


icf_uint32_t
icf_pack_icf_info_req_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t    *p_length
)
{
    icf_uint32_t        offset = 0;

    icf_info_req_st    *p_icf_info_req_st = ICF_NULL;
    
    p_icf_info_req_st    = (icf_info_req_st *)p_src;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function packs icf_refer_req_st */
    ICF_CLIB_TRACE(("Packing icf_info_req_st\n"));

    icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_info_req_st->bit_mask), &offset);
    
    *p_length    += offset;

    if(p_icf_info_req_st->bit_mask & ICF_HDR_LIST_PRESENT)
    {
        icf_pack_icf_header_list_st(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_info_req_st->header_list), &offset);
		
        *p_length += offset;
    }

    if(p_icf_info_req_st->bit_mask & ICF_TAG_LIST_PRESENT)
    {
        icf_pack_icf_tag_list_st(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_info_req_st->tag_list), &offset);
		
        *p_length += offset;
    }

    if(p_icf_info_req_st->bit_mask & ICF_BODY_LIST_PRESENT)
    {
        icf_pack_icf_msg_body_list_st(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_info_req_st->body_list), &offset);
		
        *p_length += offset;
    }    
  
    /* CSR 1-5576525 Merge */
    /* CSR 1-5576525: Pack the user name received
    */
    if(p_icf_info_req_st->bit_mask & ICF_USER_NAME_PRESENT)
    {
        icf_pack_icf_string_st(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_info_req_st->username), &offset);        
        *p_length    += offset;
    }
    return ICF_SUCCESS;
}

icf_uint32_t
icf_pack_icf_api_mesg_resp_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t    *p_length
)
{
    icf_uint32_t        offset = 0;

    icf_api_resp_st    *p_icf_api_resp = ICF_NULL;
    
    p_icf_api_resp    = (icf_api_resp_st *)p_src;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function packs icf_refer_req_st */
    ICF_CLIB_TRACE(("Packing icf_api_resp_st\n"));

    icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_api_resp->bit_mask), &offset);
    
    *p_length    += offset;
    
    icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_api_resp->result), &offset);
    
    *p_length    += offset;

    icf_pack_U16(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_api_resp->error_cause), &offset);
    
    *p_length    += offset;

   	if(p_icf_api_resp->bit_mask & ICF_HDR_LIST_PRESENT)
	{
		icf_pack_icf_header_list_st(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_api_resp->header_list), &offset);
		
		*p_length	+= offset;
	}
	
	if(p_icf_api_resp->bit_mask & ICF_BODY_LIST_PRESENT)
	{
		icf_pack_icf_msg_body_list_st(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_api_resp->body_list), &offset);
		
		*p_length	+= offset;
	}

    if(p_icf_api_resp->bit_mask & ICF_STREAM_LIST_PRESENT)
	{
		icf_pack_icf_stream_id_list_st(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_api_resp->stream_list), &offset);
		
		*p_length	+= offset;
	}
     if(p_icf_api_resp->bit_mask & ICF_RETRY_AFTER_DURATION_PRESENT)
       {
           icf_pack_U32(p_dest + (*p_length),
                (icf_uint8_t*)&(p_icf_api_resp->retry_after_duration),
                 &offset);

           *p_length       += offset; 
        }

    return ICF_SUCCESS;
}

icf_uint32_t
icf_pack_icf_ctu_resp_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t    *p_length
)
{
    icf_uint32_t        offset = 0;

    icf_ctu_resp_st    *p_icf_ctu_resp = ICF_NULL;
    
    p_icf_ctu_resp    = (icf_ctu_resp_st *)p_src;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function packs icf_refer_req_st */
    ICF_CLIB_TRACE(("Packing icf_ctu_resp_st\n"));

    icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_ctu_resp->bit_mask), &offset);
    
    *p_length    += offset;
    
    icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_ctu_resp->result), &offset);
    
    *p_length    += offset;

    icf_pack_U16(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_ctu_resp->error_cause), &offset);
    
    *p_length    += offset;

   	if(p_icf_ctu_resp->bit_mask & ICF_HDR_LIST_PRESENT)
	{
		icf_pack_icf_header_list_st(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_ctu_resp->header_list), &offset);
		
		*p_length	+= offset;
	}
    if(p_icf_ctu_resp->bit_mask & ICF_RETRY_AFTER_DURATION_PRESENT)
    {
         icf_pack_U32(p_dest + (*p_length),
                (icf_uint8_t*)&(p_icf_ctu_resp->retry_after_duration),
                 &offset);

        *p_length       += offset;
    }	
    return ICF_SUCCESS;
}


icf_uint32_t
icf_pack_icf_cta_resp_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t    *p_length
)
{
    icf_uint32_t        offset = 0;

    icf_cta_resp_st    *p_icf_cta_resp = ICF_NULL;
    
    p_icf_cta_resp    = (icf_cta_resp_st *)p_src;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function packs icf_refer_req_st */
    ICF_CLIB_TRACE(("Packing icf_cta_resp_st\n"));

    icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_cta_resp->bit_mask), &offset);
    
    *p_length    += offset;
    
    icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_cta_resp->result), &offset);
    
    *p_length    += offset;

    icf_pack_U16(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_cta_resp->error_cause), &offset);
    
    *p_length    += offset;

   	if(p_icf_cta_resp->bit_mask & ICF_HDR_LIST_PRESENT)
	{
		icf_pack_icf_header_list_st(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_cta_resp->header_list), &offset);
		
		*p_length	+= offset;
	}	
	
    return ICF_SUCCESS;
}

icf_uint32_t
icf_pack_icf_remote_call_transfer_initiated_ind_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t *p_length
)
{
    icf_uint32_t        offset = 0;

    icf_remote_call_transfer_initiated_ind_st    *p_icf_remote_call_transfer_initiated_ind_st = ICF_NULL;
    
    p_icf_remote_call_transfer_initiated_ind_st    = (icf_remote_call_transfer_initiated_ind_st *)p_src;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function is for unpacking icf_remote_call_transfer_initiated_ind_st */
    ICF_CLIB_TRACE(("Packing icf_remote_call_transfer_initiated_ind_st\n"));

    icf_pack_U16(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_remote_call_transfer_initiated_ind_st->new_call_id), &offset);	
	*p_length    += offset;
  
    icf_pack_icf_address_st(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_remote_call_transfer_initiated_ind_st->referred_party_addr), &offset);	
	*p_length    += offset;

    icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_remote_call_transfer_initiated_ind_st->bit_mask), &offset);	
	*p_length    += offset;

    if(p_icf_remote_call_transfer_initiated_ind_st->bit_mask & ICF_REFER_INITIATED_REFERRED_PARTY_NAME)
    {
        icf_pack_icf_string_st(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_remote_call_transfer_initiated_ind_st->referred_party_name), &offset);        
        *p_length    += offset;
    }

    if(p_icf_remote_call_transfer_initiated_ind_st->bit_mask & ICF_HDR_LIST_PRESENT)
	{		
		icf_pack_icf_header_list_st(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_remote_call_transfer_initiated_ind_st->header_list), &offset);
		*p_length	+= offset;
	}

	if(p_icf_remote_call_transfer_initiated_ind_st->bit_mask & ICF_BODY_LIST_PRESENT)
	{
        icf_pack_icf_msg_body_list_st(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_remote_call_transfer_initiated_ind_st->body_list), &offset);		
        *p_length	+= offset;
	}
   	
	return(ICF_SUCCESS);
}

icf_uint32_t
icf_pack_icf_api_ind_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t    *p_length
)
{
    icf_uint32_t        offset = 0;

    icf_api_ind_st    *p_icf_api_resp = ICF_NULL;
    
    p_icf_api_resp    = (icf_api_ind_st *)p_src;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function packs icf_refer_req_st */
    ICF_CLIB_TRACE(("Packing icf_api_ind_st\n"));

    icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_api_resp->bit_mask), &offset);
    
    *p_length    += offset;

   	if(p_icf_api_resp->bit_mask & ICF_HDR_LIST_PRESENT)
	{
		icf_pack_icf_header_list_st(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_api_resp->header_list), &offset);
		
		*p_length	+= offset;
	}
	
	if(p_icf_api_resp->bit_mask & ICF_BODY_LIST_PRESENT)
	{
		icf_pack_icf_msg_body_list_st(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_api_resp->body_list), &offset);
		
		*p_length	+= offset;
	}

    if(p_icf_api_resp->bit_mask & ICF_STREAM_LIST_PRESENT)
	{
		icf_pack_icf_stream_id_list_st(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_api_resp->stream_list), &offset);
		
		*p_length	+= offset;
	}

    return ICF_SUCCESS;
}

icf_uint32_t
icf_pack_icf_redirection_ind_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t    *p_length
)
{
    icf_uint32_t        offset = 0;

    icf_call_redirection_ind_st    *p_icf_red_ind = ICF_NULL;
    
    p_icf_red_ind    = (icf_call_redirection_ind_st *)p_src;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function packs icf_refer_req_st */
    ICF_CLIB_TRACE(("Packing icf_call_redirection_ind_st\n"));

    icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_red_ind->bit_mask), &offset);
    
    *p_length    += offset;

   	if(p_icf_red_ind->bit_mask & ICF_CALL_RDR_IND_PTY_ADDR)
	{
		icf_pack_icf_address_st(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_red_ind->redirected_party_addr), &offset);        
		
		*p_length	+= offset;
	}
	
	if(p_icf_red_ind->bit_mask & ICF_CALL_RDR_IND_PTY_NAME)
	{
        icf_pack_icf_string_st(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_red_ind->redirected_party_name), &offset);		
		
		*p_length	+= offset;
	}

    if(p_icf_red_ind->bit_mask & ICF_HDR_LIST_PRESENT)
	{
		icf_pack_icf_header_list_st(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_red_ind->header_list), &offset);
		
		*p_length	+= offset;
	}

    return ICF_SUCCESS;
}


icf_uint32_t
icf_pack_icf_conference_req_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t    *p_length
)
{
    icf_uint32_t        offset = 0;

    icf_conference_req_st    *p_icf_conference_req_st = ICF_NULL;
    
    p_icf_conference_req_st    = (icf_conference_req_st*)p_src;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function is for packing icf_conference_req_st */
    ICF_CLIB_TRACE(("Packing icf_conference_req_st\n"));

    icf_pack_U32(p_dest + (*p_length), 
       (icf_uint8_t*)&(p_icf_conference_req_st->bit_mask), &offset);

    *p_length    += offset;

    icf_pack_U16(p_dest + (*p_length), 
       (icf_uint8_t*)&(p_icf_conference_req_st->merge_call_id), &offset);
    *p_length    += offset;
    
    if(p_icf_conference_req_st->bit_mask & ICF_HDR_LIST_PRESENT)
    {
       icf_pack_icf_header_list_st(p_dest + (*p_length), 
            (icf_uint8_t*)&(p_icf_conference_req_st->header_list), &offset);

        *p_length       += offset;
    }

    if(p_icf_conference_req_st->bit_mask & ICF_TAG_LIST_PRESENT)
    {
       icf_pack_icf_tag_list_st(p_dest + (*p_length), 
          (icf_uint8_t*)&(p_icf_conference_req_st->tag_list), &offset);

        *p_length       += offset;
    }

    return(ICF_SUCCESS);

}


icf_uint32_t
icf_pack_icf_party_replaced_ind
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t    *p_length
)
{
    icf_uint32_t        offset = 0;

    icf_party_replaced_ind_st    *p_icf_api_resp = ICF_NULL;
    
    p_icf_api_resp    = (icf_party_replaced_ind_st *)p_src;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function packs icf_refer_req_st */
    ICF_CLIB_TRACE(("Packing icf_party_replaced_ind_st\n"));

    icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_api_resp->bit_mask), &offset);
    *p_length    += offset;

    icf_pack_U16(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_api_resp->replacing_call_id), &offset);
	*p_length	+= offset;

    if(p_icf_api_resp->bit_mask & ICF_REPLACING_PARTY_ADDR_PRESENT)
	{
		icf_pack_icf_address_st(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_api_resp->replacing_party_addr), &offset);
		
		*p_length	+= offset;
	}
	
	if(p_icf_api_resp->bit_mask & ICF_PARTY_REPLACED_NAME)
	{
		icf_pack_icf_string_st(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_api_resp->replacing_party_name), &offset);
		
		*p_length	+= offset;
	}

   	if(p_icf_api_resp->bit_mask & ICF_HDR_LIST_PRESENT)
	{
		icf_pack_icf_header_list_st(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_api_resp->header_list), &offset);
		
		*p_length	+= offset;
	}
	
	if(p_icf_api_resp->bit_mask & ICF_BODY_LIST_PRESENT)
	{
		icf_pack_icf_msg_body_list_st(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_api_resp->body_list), &offset);
		
		*p_length	+= offset;
	}

    if(p_icf_api_resp->bit_mask & ICF_STREAM_LIST_PRESENT)
	{
		icf_pack_icf_stream_id_list_st(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_api_resp->stream_list), &offset);
		
		*p_length	+= offset;
	}

    return ICF_SUCCESS;
}

icf_uint32_t
icf_pack_icf_route_info_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t    *p_length
)
{
    icf_uint32_t        offset = 0;

	icf_route_info_st *p_route_info = ICF_NULL;

	p_route_info = (icf_route_info_st*)p_src;

	icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&(p_route_info->type), &offset);

	*p_length += offset;

	if ( ICF_IPC_UDP == p_route_info->type)
	{
		icf_pack_icf_transport_addr_st(p_dest + (*p_length), (icf_uint8_t*)&(p_route_info->ipc.udp.ip),&offset);

		*p_length += offset;

		icf_pack_U16(p_dest + (*p_length), (icf_uint8_t*)&(p_route_info->ipc.udp.port),&offset);

		*p_length += offset;
	}
	else if ( ICF_IPC_TCP == p_route_info->type)
	{
		icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&(p_route_info->ipc.tcp.type), &offset);

		*p_length += offset;

		icf_pack_icf_transport_addr_st(p_dest + (*p_length), (icf_uint8_t*)&(p_route_info->ipc.tcp.ip),&offset);

		*p_length += offset;

		icf_pack_U16(p_dest + (*p_length), (icf_uint8_t*)&(p_route_info->ipc.tcp.port),&offset);

		*p_length += offset;

	}
	else if ( ICF_IPC_MSGQ == p_route_info->type)
	{
		icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&(p_route_info->ipc.msgq.mtype), &offset);

		*p_length += offset;

		icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&(p_route_info->ipc.msgq.mkey), &offset);

		*p_length += offset;
	}
	else
	{
		return ICF_FAILURE;
	}

	return ICF_SUCCESS;
}

/**************************************

			ICF REL 6.0

****************************************/

icf_uint32_t
icf_pack_icf_call_modify_media_data_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t    *p_length
)
{
	icf_uint32_t        offset = 0;

	icf_call_modify_media_data_st  *p_icf_call_modify_media_data_st = (icf_call_modify_media_data_st *)p_src;

	/* This function packs icf_call_modify_media_data_st */
    ICF_CLIB_TRACE(("Packing icf_call_modify_media_data_st\n"));

	/* Initialise length to 0 */
    *p_length    = 0;

	icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_call_modify_media_data_st->bit_mask), &offset);

	*p_length += offset;

	icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_call_modify_media_data_st->stream_id), &offset);

	*p_length += offset;

	icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_call_modify_media_data_st->stream_type), &offset);

	*p_length += offset;

	if(p_icf_call_modify_media_data_st->bit_mask & ICF_CALL_MOD_CODEC_INFO_PRESENT)
	{
		icf_pack_icf_codec_attrib_list_st(p_dest + (*p_length), (p_icf_call_modify_media_data_st->p_codec_info_list), &offset, p_icf_call_modify_media_data_st->stream_type );

		*p_length += offset;
	}

	if(p_icf_call_modify_media_data_st->bit_mask & ICF_CALL_MOD_MEDIA_STREAM_PARAM_PRESENT)
	{
		switch(p_icf_call_modify_media_data_st->stream_type)
		{
		case  ICF_STREAM_TYPE_AUDIO:
			 
			  icf_pack_icf_audio_profile_st(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_call_modify_media_data_st->media_stream_param), &offset);

			  *p_length += offset;

			  break;

		case  ICF_STREAM_TYPE_VIDEO:

			  icf_pack_icf_video_profile_st(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_call_modify_media_data_st->media_stream_param), &offset);

			  *p_length += offset;

			  break;
		}
	}

	if(p_icf_call_modify_media_data_st->bit_mask & ICF_CALL_MOD_ENC_ATTRIB_PRESENT)
	{
		icf_pack_icf_encoded_attributes_list_st(p_dest + (*p_length), p_icf_call_modify_media_data_st->p_encoded_attributes, &offset);

		*p_length += offset;
	}

	if(p_icf_call_modify_media_data_st->bit_mask & ICF_CALL_MOD_MEDIA_MODE_PRESENT)
	{
		icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_call_modify_media_data_st->media_mode), &offset);

		*p_length += offset;
	}

	return ICF_SUCCESS;
}


icf_uint32_t
icf_pack_icf_call_modify_media_data_list_st
(
    icf_uint8_t    *p_dest,
    icf_list_st    *p_icf_call_modify_media_data_list_st,
    icf_uint32_t    *p_length
)
{
	icf_uint32_t        offset = 0;
	icf_uint8_t			count = 0;
	
	icf_list_st       *p_list_ptr = ICF_NULL;
   
	/* This function packs list of icf_call_modify_media_data_st */
    ICF_CLIB_TRACE(("Packing list of icf_call_modify_media_data_st\n"));

	/* Initialise length to 0 */
    *p_length    = 0;

	/*Pack count = 0 initially*/
	icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&count, &offset);
    
    *p_length    += offset;

	for(p_list_ptr = p_icf_call_modify_media_data_list_st;ICF_NULL != p_list_ptr;p_list_ptr = p_list_ptr->p_next)
    {

		icf_pack_icf_call_modify_media_data_st(p_dest + (*p_length), (p_list_ptr->p_data), &offset);

		*p_length += offset;

		count++;

	}

	if(count)
	{
		/* Updating the count */
		icf_pack_U8(p_dest, (icf_uint8_t*)&count, &offset);
	}

	return ICF_SUCCESS;
}

icf_uint32_t
icf_pack_icf_media_connected_ind_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t    *p_length
)
{

	icf_uint32_t        offset = 0;
	icf_uint8_t         count = 0;
	
    icf_media_connected_ind_st  *p_icf_media_connected_ind_st = (icf_media_connected_ind_st *)p_src;

	/* This function packs icf_media_connected_ind_st */
    ICF_CLIB_TRACE(("Packing icf_media_connected_ind_st\n"));

	/* Initialise length to 0 */
    *p_length    = 0;
	
	icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_media_connected_ind_st->bit_mask), &offset);
        
    *p_length    += offset;
    
    icf_pack_icf_stream_capab_list_st(p_dest + (*p_length), (p_icf_media_connected_ind_st->p_accepted_stream_list), &offset);
       
    *p_length    += offset;
	
	if(p_icf_media_connected_ind_st->bit_mask & ICF_MEDIA_CONECT_DELETED_STREAM_PRESENT)
    {
		icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_media_connected_ind_st->count), &offset);
        
        *p_length    += offset;

        while((count<p_icf_media_connected_ind_st->count) && (count < ICF_MAX_STREAM_PER_CALL))
		{
			icf_pack_icf_stream_list_st(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_media_connected_ind_st->deleted_media_streams[count]), &offset);
        
			*p_length    += offset;

			count++;
		}
    }
    /* This field indicates to application that it is a precondition call  or not */
    if(ICF_MEDIA_CONNECT_IND_REL_PROV_RESP_PRESENT & p_icf_media_connected_ind_st->bit_mask)
    {
        icf_pack_U8(p_dest + (*p_length), 
                (icf_uint8_t*)&(p_icf_media_connected_ind_st->rel_prov_resp), 
                &offset);
        *p_length    += offset;
    }


    return ICF_SUCCESS;
}

icf_uint32_t
icf_pack_icf_connect_resp_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t    *p_length
)
{

	icf_uint32_t        offset = 0;
	icf_uint8_t        count = 0;
	
    icf_connect_resp_st  *p_icf_connect_resp_st = (icf_connect_resp_st *)p_src;

	/* This function packs icf_connect_resp_st */
    ICF_CLIB_TRACE(("Packing icf_connect_resp_st\n"));

	/* Initialise length to 0 */
    *p_length    = 0;
	
	icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_connect_resp_st->bit_mask), &offset);
        
    *p_length    += offset;
    
    icf_pack_icf_call_modify_media_data_list_st(p_dest + (*p_length), (p_icf_connect_resp_st->p_accepted_stream_list), &offset);
       
    *p_length    += offset;

	if(p_icf_connect_resp_st->bit_mask & ICF_CONNECT_RESP_DELETED_STREAM_PRESENT)
	{
		icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_connect_resp_st->count), &offset);
        
		*p_length    += offset;
	
		while((count<p_icf_connect_resp_st->count) && (count < ICF_MAX_STREAM_PER_CALL))
		{
			icf_pack_icf_stream_list_st(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_connect_resp_st->deleted_media_streams[count]), &offset);
        
			*p_length    += offset;

			count++;
		}
	}
	if(p_icf_connect_resp_st->bit_mask & ICF_HDR_LIST_PRESENT)
    {
        icf_pack_icf_header_list_st(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_connect_resp_st->header_list), &offset);

        *p_length   += offset;
    }

    return ICF_SUCCESS;
}


/*Functions for fax support*/


#ifdef ICF_FAX_SUPPORT

icf_uint32_t
icf_pack_icf_start_fax_req_st
(
	icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t   *p_length
)
{
    icf_uint32_t        offset = 0;
   
    icf_start_fax_req_st    *p_icf_start_fax_req_st = ICF_NULL;
    
    p_icf_start_fax_req_st    = (icf_start_fax_req_st *)p_src;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function packs icf_start_fax_req_st */
    ICF_CLIB_TRACE(("Packing icf_start_fax_req_st\n"));
    
   icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_start_fax_req_st->bitmask), &offset);
    
    *p_length    += offset;

    icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_start_fax_req_st->fax_type), &offset);
    
    *p_length    += offset;

    icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_start_fax_req_st->fax_pt_codec), &offset);
    
    *p_length    += offset;

     icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_start_fax_req_st->fax_method), &offset);

     *p_length    += offset;

     icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_start_fax_req_st->pt_param), &offset);

    *p_length    += offset;
    
	return ICF_SUCCESS;
}

icf_uint32_t
icf_pack_icf_start_fax_resp_st
(
	icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t   *p_length
)
{
    icf_uint32_t        offset = 0;
   
    icf_start_fax_resp_st    *p_icf_start_fax_resp_st = ICF_NULL;
    
    p_icf_start_fax_resp_st    = (icf_start_fax_resp_st *)p_src;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function packs icf_start_fax_resp_st */
    ICF_CLIB_TRACE(("Packing icf_start_fax_resp_st\n"));

    icf_pack_U32(p_dest + (*p_length), 
        (icf_uint8_t*)&(p_icf_start_fax_resp_st->bit_mask), &offset);
        
    *p_length    += offset;
    
    icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_start_fax_resp_st->result), &offset);
    
    *p_length    += offset;

	if(!p_icf_start_fax_resp_st->result)
	{
		icf_pack_U16(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_start_fax_resp_st->error_cause), &offset);
    
		*p_length    += offset;
	}

	if(p_icf_start_fax_resp_st->result)
	{
		icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_start_fax_resp_st->fax_type), &offset);
    
		*p_length    += offset;
	}

       if(p_icf_start_fax_resp_st->bit_mask & ICF_RETRY_AFTER_DURATION_PRESENT)
       {
           icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&
              (p_icf_start_fax_resp_st->retry_after_duration), &offset);

	 *p_length    += offset;
         
       }

       return ICF_SUCCESS;
}

icf_uint32_t
icf_pack_icf_start_fax_ind_st
(
	icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t   *p_length
)
{
    icf_uint32_t        offset = 0;
   
    icf_start_fax_ind_st    *p_icf_start_fax_ind_st = ICF_NULL;
    
    p_icf_start_fax_ind_st    = (icf_start_fax_ind_st *)p_src;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function packs icf_start_fax_ind_st */
    ICF_CLIB_TRACE(("Packing icf_start_fax_ind_st\n"));
    
    icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_start_fax_ind_st->fax_type), &offset);
    
    *p_length    += offset;

	return ICF_SUCCESS;
}

/**************************************

			ICF REL 7.0

****************************************/


icf_uint32_t
icf_pack_icf_create_xconf_req_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t    *p_length
)
{
    icf_uint32_t        offset = 0;

    icf_create_ext_xconf_req_st    *p_icf_create_ext_xconf_req_st = ICF_NULL;

    p_icf_create_ext_xconf_req_st    = (icf_create_ext_xconf_req_st *)p_src;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function packs icf_create_ext_xconf_req_st */
    ICF_CLIB_TRACE(("Packing icf_create_ext_xconf_req_st\n"));

    icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_create_ext_xconf_req_st->bitmask), &offset);

    *p_length    += offset;

    icf_pack_icf_line_id_t(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_create_ext_xconf_req_st->line_id), &offset);

    *p_length    += offset;
  
    if(p_icf_create_ext_xconf_req_st->bitmask & 
            ICF_CREATE_XCONF_CALLING_PARTY_ADDR_PRESENT)
    {
       icf_pack_icf_address_st(p_dest + (*p_length),
          (icf_uint8_t*)&(p_icf_create_ext_xconf_req_st-> \
           calling_party_address), &offset);
       *p_length    += offset;
    }   

    if(p_icf_create_ext_xconf_req_st->bitmask & 
            ICF_CREATE_XCONF_CALLING_PARTY_NAME_PRESENT)
    {
       icf_pack_icf_string_st(p_dest + (*p_length),
          (icf_uint8_t*)&(p_icf_create_ext_xconf_req_st->calling_party_name), 
           &offset);
       *p_length    += offset;
    }  
    
    if(p_icf_create_ext_xconf_req_st->bitmask & 
            ICF_CREATE_XCONF_TOGGLE_PRES_IND_PRESENT)
    {
        icf_pack_U8(p_dest + (*p_length), \
              (icf_uint8_t*)&(p_icf_create_ext_xconf_req_st-> \
              toggle_presentation_indicator), &offset);
        *p_length    += offset;
    }

     if(p_icf_create_ext_xconf_req_st->bitmask & ICF_CREATE_XCONF_CONF_URI_PRESENT)
     { 
    	icf_pack_icf_address_st(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_create_ext_xconf_req_st->conference_uri), &offset);

        *p_length    += offset;
     }

    if(p_icf_create_ext_xconf_req_st->bitmask & ICF_CREATE_XCONF_EXIST_CALL_ID_PRESENT)
    {
        icf_pack_U16(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_create_ext_xconf_req_st->existing_call_id), &offset);

        *p_length    += offset;
    }

    if(p_icf_create_ext_xconf_req_st->bitmask & ICF_HDR_LIST_PRESENT)
    {
	icf_pack_icf_header_list_st(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_create_ext_xconf_req_st-> header_list), &offset);

        *p_length	+= offset;
    }
    if(p_icf_create_ext_xconf_req_st->bitmask & ICF_TAG_LIST_PRESENT)
    { 
	icf_pack_icf_tag_list_st(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_create_ext_xconf_req_st->tag_list), &offset); 
    
        *p_length	+= offset;
    }

    if(p_icf_create_ext_xconf_req_st->bitmask & ICF_CREATE_XCONF_PREF_MEDIA_PROFILE_PRESENT)
    {
	icf_pack_icf_stream_capab_list_st(p_dest + (*p_length), (p_icf_create_ext_xconf_req_st->p_pref_media_profile), &offset);
        
        *p_length    += offset;
    }
    /*Start: Rel8.0 Added for precondition support */
    if(p_icf_create_ext_xconf_req_st->bitmask & 
            ICF_CREATE_XCONF_PRECONDITION_REQD_PRESENT)
    {
       icf_pack_U8(p_dest + (*p_length), \
       (icf_uint8_t*)&(p_icf_create_ext_xconf_req_st->precondition_required), \
       &offset);
       *p_length    += offset;
    }    
    /*End: Rel8.0 Added for precondition support */

    /*Added media transport type in create xconf request to support
     *media profile on per call basis*/
    if (ICF_CREATE_XCONF_MEDIA_TYPE_PRESENT & p_icf_create_ext_xconf_req_st->bitmask)
    {
         icf_pack_U32(p_dest + (*p_length),
              (icf_uint8_t*)&(p_icf_create_ext_xconf_req_st->media_transport_type), 
              &offset);
         *p_length    += offset;
    }

    /*Added for SRTP Fallback in xconf req*/
    if (ICF_CREATE_XCONF_SRTP_FALLBACK_REQD_PRESENT & 
            p_icf_create_ext_xconf_req_st->bitmask)
    {
        icf_pack_U8(p_dest + (*p_length),
             (icf_uint8_t*)&(p_icf_create_ext_xconf_req_st->srtp_fallback_reqd),
             &offset);
        *p_length    += offset;
    }

    return ICF_SUCCESS;
}

icf_uint32_t
icf_pack_icf_add_xconf_party_req_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t    *p_length
)
{
    icf_uint32_t        offset = 0;

    icf_add_xconf_party_req_st    *p_icf_add_xconf_party_req_st = ICF_NULL;

    p_icf_add_xconf_party_req_st    = (icf_add_xconf_party_req_st *)p_src;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function packs icf_add_xconf_party_req_st */
    ICF_CLIB_TRACE(("Packing icf_add_xconf_party_req_st\n"));

    icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_add_xconf_party_req_st->bitmask), &offset);
    *p_length    += offset;
     
    if(p_icf_add_xconf_party_req_st->bitmask & ICF_HDR_LIST_PRESENT)
    {
        icf_pack_icf_header_list_st(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_add_xconf_party_req_st-> header_list), &offset);

        *p_length       += offset;
    }
    if(p_icf_add_xconf_party_req_st->bitmask & ICF_TAG_LIST_PRESENT)
    {
        icf_pack_icf_tag_list_st(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_add_xconf_party_req_st-> tag_list), &offset);

        *p_length       += offset;
    }
    
    icf_pack_U16(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_add_xconf_party_req_st-> add_party_id), &offset);
    *p_length       += offset;
    return ICF_SUCCESS;
}

icf_uint32_t
icf_pack_icf_delete_xconf_party_req_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t    *p_length
)
{
    icf_uint32_t        offset = 0;

    icf_delete_xconf_party_req_st    *p_icf_delete_xconf_party_req_st = ICF_NULL;

    p_icf_delete_xconf_party_req_st    = (icf_delete_xconf_party_req_st *)p_src;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function packs icf_delete_xconf_party_req_st */
    ICF_CLIB_TRACE(("Packing icf_delete_xconf_party_req_st\n"));

    icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_delete_xconf_party_req_st->bitmask), &offset);
    *p_length    += offset;
  
    if(p_icf_delete_xconf_party_req_st->bitmask & ICF_HDR_LIST_PRESENT)
    {
        icf_pack_icf_header_list_st(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_delete_xconf_party_req_st-> header_list), &offset);

        *p_length       += offset;
    }
    if(p_icf_delete_xconf_party_req_st->bitmask & ICF_TAG_LIST_PRESENT)
    {
        icf_pack_icf_tag_list_st(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_delete_xconf_party_req_st-> tag_list), &offset);

        *p_length       += offset;
    }
     icf_pack_icf_address_st(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_delete_xconf_party_req_st->del_party_uri),  &offset);

    *p_length       += offset;
    return ICF_SUCCESS;
}

icf_uint32_t
icf_pack_icf_release_xconf_req_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t    *p_length
)
{
    icf_uint32_t        offset = 0;

    icf_release_xconf_req_st    *p_icf_release_xconf_req_st = ICF_NULL;

    p_icf_release_xconf_req_st    = (icf_release_xconf_req_st *)p_src;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function packs icf_release_xconf_req_st */
    ICF_CLIB_TRACE(("Packing icf_release_xconf_req_st\n"));

    icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_release_xconf_req_st->bitmask), &offset);
    *p_length    += offset;

    if(p_icf_release_xconf_req_st->bitmask & ICF_HDR_LIST_PRESENT)
    {
        icf_pack_icf_header_list_st(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_release_xconf_req_st-> header_list), &offset);

        *p_length       += offset;
    }
    
    if(p_icf_release_xconf_req_st->bitmask & ICF_TAG_LIST_PRESENT)
    {
        icf_pack_icf_tag_list_st(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_release_xconf_req_st-> tag_list), &offset);

        *p_length       += offset;
    }

    return ICF_SUCCESS;
}

icf_uint32_t
icf_pack_icf_create_xconf_resp_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t    *p_length
)
{
    icf_uint32_t        offset = 0;

    icf_create_ext_xconf_resp_st    *p_icf_create_ext_xconf_resp_st = ICF_NULL;

    p_icf_create_ext_xconf_resp_st    = (icf_create_ext_xconf_resp_st *)p_src;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function packs icf_create_ext_xconf_resp_st */
    ICF_CLIB_TRACE(("Packing icf_create_ext_xconf_resp_st\n"));

    icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_create_ext_xconf_resp_st->bitmask), &offset);
    *p_length    += offset;

    icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_create_ext_xconf_resp_st->result), &offset);
	
    *p_length	+= offset;

    icf_pack_U16(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_create_ext_xconf_resp_st->error_cause), &offset);
	
      *p_length    += offset;

     return ICF_SUCCESS;
}

icf_uint32_t
icf_pack_icf_add_xconf_party_resp_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t    *p_length
)
{
    icf_uint32_t        offset = 0;

    icf_add_xconf_party_resp_st    *p_icf_add_xconf_party_resp_st = ICF_NULL;

    p_icf_add_xconf_party_resp_st    = (icf_add_xconf_party_resp_st *)p_src;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function packs icf_add_xconf_party_resp_st */
    ICF_CLIB_TRACE(("Packing icf_add_xconf_party_resp_st\n"));

    icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_add_xconf_party_resp_st->result), &offset);
	
    *p_length	+= offset;

    icf_pack_U16(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_add_xconf_party_resp_st->error_cause), &offset);
	
      *p_length    += offset;

    return ICF_SUCCESS;
}

icf_uint32_t
icf_pack_icf_delete_xconf_party_resp_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t    *p_length
)
{
    icf_uint32_t        offset = 0;

    icf_delete_xconf_party_resp_st    *p_icf_delete_xconf_party_resp_st = ICF_NULL;

    p_icf_delete_xconf_party_resp_st    = (icf_delete_xconf_party_resp_st *)p_src;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function packs icf_delete_xconf_party_resp_st */
    ICF_CLIB_TRACE(("Packing icf_delete_xconf_party_resp_st\n"));

    icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_delete_xconf_party_resp_st->result), &offset);
	
    *p_length	+= offset;

    icf_pack_U16(p_dest + (*p_length),
            (icf_uint8_t*)&(p_icf_delete_xconf_party_resp_st->error_code), &offset);
	
    *p_length	+= offset;

     icf_pack_icf_address_st(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_delete_xconf_party_resp_st->deleted_party_uri), &offset);

       *p_length    += offset;

     return ICF_SUCCESS;
}


icf_uint32_t
icf_pack_icf_release_xconf_ind_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t    *p_length
)
{
    icf_uint32_t        offset = 0;

    icf_xconf_release_ind_st    *p_icf_xconf_release_ind_st = ICF_NULL;

    p_icf_xconf_release_ind_st    = (icf_xconf_release_ind_st *)p_src;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function packs icf_xconf_release_ind_st */
    ICF_CLIB_TRACE(("Packing icf_xconf_release_ind_st\n"));

    icf_pack_U8(p_dest + (*p_length),
       (icf_uint8_t*)&(p_icf_xconf_release_ind_st->call_clear_reason), &offset);
    
    *p_length    += offset;
    
     return ICF_SUCCESS;
}


icf_uint32_t
icf_pack_icf_release_xconf_resp_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t    *p_length
)
{
    icf_uint32_t        offset = 0;

    icf_release_xconf_resp_st    *p_icf_release_xconf_resp_st = ICF_NULL;

    p_icf_release_xconf_resp_st    = (icf_release_xconf_resp_st *)p_src;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function packs icf_xconf_release_ind_st */
    ICF_CLIB_TRACE(("Packing icf_release_xconf_resp_st\n"));

    icf_pack_U8(p_dest + (*p_length),
            (icf_uint8_t*)&(p_icf_release_xconf_resp_st->result), &offset);
    *p_length    += offset;

     icf_pack_U16(p_dest + (*p_length),
            (icf_uint8_t*)&(p_icf_release_xconf_resp_st->error_code), &offset);
    *p_length    += offset;
    

     return ICF_SUCCESS;
}


#endif

icf_uint32_t
icf_pack_icf_info_resp_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t    *p_length
)
{
    icf_uint32_t        offset = 0;

    icf_info_resp_st    *p_icf_info_resp_st = ICF_NULL;
    
    p_icf_info_resp_st    = (icf_info_resp_st *)p_src;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function packs icf_refer_req_st */
    ICF_CLIB_TRACE(("Packing icf_info_resp_st\n"));

    icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_info_resp_st->bit_mask), &offset);
    
    *p_length    += offset;

	icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_info_resp_st->result), &offset);
	
	*p_length	+= offset;

	icf_pack_U16(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_info_resp_st->error_cause), &offset);
	
	*p_length	+= offset;

	icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_info_resp_st->response_code), &offset);
	
	*p_length	+= offset;

   	if(p_icf_info_resp_st->bit_mask & ICF_HDR_LIST_PRESENT)
	{
		icf_pack_icf_header_list_st(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_info_resp_st->header_list), &offset);
		
		*p_length	+= offset;
	}

	if(p_icf_info_resp_st->bit_mask & ICF_BODY_LIST_PRESENT)
	{
		icf_pack_icf_msg_body_list_st(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_info_resp_st->body_list), &offset);
		
		*p_length	+= offset;
	}  
	
    return ICF_SUCCESS;
}



/*NOTIFY_OUT_OF_DIALOG_START*/

icf_uint32_t
icf_pack_icf_notify_cfm_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t    *p_length
)
{
    icf_uint32_t        offset = 0;

    icf_notify_cfm_st    *p_icf_notify_cfm_st = ICF_NULL;
    
    p_icf_notify_cfm_st    = (icf_notify_cfm_st *)p_src;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function packs icf_notify_cfm_st */
    ICF_CLIB_TRACE(("Packing icf_notify_cfm_st\n"));
    
    icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_notify_cfm_st->bit_mask), &offset);
    
    *p_length    += offset;


	icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_notify_cfm_st->result), &offset);
    
    *p_length    += offset;

	if(p_icf_notify_cfm_st->bit_mask & ICF_NOTIFY_ERROR_CAUSE_PRESENT)
	{
		icf_pack_U16(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_notify_cfm_st->error_code), &offset);
    
		*p_length    += offset;
	}

	if(p_icf_notify_cfm_st->bit_mask & ICF_NOTIFY_RESP_CODE_PRESENT)
	{
		icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_notify_cfm_st->response_code), &offset);
    
		*p_length    += offset;
	}

    /* Asynchronous NOTIFY Support */
    if(p_icf_notify_cfm_st->bit_mask & ICF_HDR_LIST_PRESENT)
	{
		icf_pack_icf_header_list_st(p_dest + (*p_length), 
                (icf_uint8_t*)&(p_icf_notify_cfm_st->header_list), &offset);
		
		*p_length	+= offset;
	}

    if(p_icf_notify_cfm_st->bit_mask & ICF_TAG_LIST_PRESENT)
	{
		icf_pack_icf_tag_list_st(p_dest + (*p_length),
                (icf_uint8_t*)&(p_icf_notify_cfm_st->tag_list), &offset);
		
		*p_length	+= offset;
	}

	if(p_icf_notify_cfm_st->bit_mask & ICF_BODY_LIST_PRESENT)
	{
		icf_pack_icf_msg_body_list_st(p_dest + (*p_length),
                (icf_uint8_t*)&(p_icf_notify_cfm_st->body_list), &offset);
		
		*p_length	+= offset;
	}

    return(ICF_SUCCESS);
}

/*NOTIFY_OUT_OF_DIALOG_END*/

/*HANDLE_INFO_IND_START*/

icf_uint32_t
icf_pack_icf_info_ind_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t    *p_length
)
{
    icf_uint32_t        offset = 0;

    icf_info_ind_st    *p_icf_info_ind_st = ICF_NULL;
    
    p_icf_info_ind_st    = (icf_info_ind_st *)p_src;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function packs icf_info_ind_st */
    ICF_CLIB_TRACE(("Packing icf_info_ind_st\n"));

    icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_info_ind_st->bit_mask), &offset);
    
    *p_length    += offset;

   	if(p_icf_info_ind_st->bit_mask & ICF_HDR_LIST_PRESENT)
	{
		icf_pack_icf_header_list_st(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_info_ind_st->header_list), &offset);
		
		*p_length	+= offset;
	}

	if(p_icf_info_ind_st->bit_mask & ICF_BODY_LIST_PRESENT)
	{
		icf_pack_icf_msg_body_list_st(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_info_ind_st->body_list), &offset);
		
		*p_length	+= offset;
	}    
    return ICF_SUCCESS;
}

/*HANDLE_INFO_IND_END*/

/* OPTIONS handling - start*/

icf_uint32_t
icf_pack_icf_options_req_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t    *p_length
)
{
    icf_uint32_t        offset = 0;

    icf_options_req_st    *p_icf_options_req_st = ICF_NULL;
    
    p_icf_options_req_st    = (icf_options_req_st *)p_src;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function packs icf_options_req_st */
    ICF_CLIB_TRACE(("Packing icf_options_req_st\n"));
    
    icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_options_req_st->bit_mask), &offset);
    
    *p_length    += offset;

	icf_pack_icf_line_id_t(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_options_req_st->line_id), &offset);
    
    *p_length    += offset;

    icf_pack_icf_address_st(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_options_req_st->destination_info), &offset);
    
    *p_length    += offset;

    if(p_icf_options_req_st->bit_mask & ICF_PREF_IDENTITY_PRESENT)
    {
        icf_pack_icf_address_st(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_options_req_st->pref_identity), &offset);
        
        *p_length    += offset;
    }

    if(p_icf_options_req_st->bit_mask & ICF_HDR_LIST_PRESENT)
    {
        icf_pack_icf_header_list_st(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_options_req_st->header_list), &offset);
        
        *p_length    += offset;
    }

    if(p_icf_options_req_st->bit_mask & ICF_TAG_LIST_PRESENT)
    {
        icf_pack_icf_tag_list_st(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_options_req_st->tag_list), &offset);
        
        *p_length    += offset;
    }

    if(p_icf_options_req_st->bit_mask & ICF_BODY_LIST_PRESENT)
    {
        icf_pack_icf_msg_body_list_st(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_options_req_st->body_list), &offset);
        
        *p_length    += offset;
    }


    if(ICF_OPTIONS_TUNNEL_MODE_OPERATION_PRESENT & p_icf_options_req_st->bit_mask)
    {
       icf_pack_U8(p_dest + (*p_length), 
                   (icf_uint8_t*)&( p_icf_options_req_st->tunnel_mode), 
                   &offset);
       *p_length    += offset;
    }

    /* Fix for SPR 20412: CSR 1-8223807 merged*/
    if(p_icf_options_req_st->bit_mask & ICF_OPTIONS_REQ_REMOVE_HDR_LIST_PRESENT)
    {
        icf_pack_icf_header_name_list_st(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_options_req_st->remove_hdr_list), &offset);
    
        *p_length    += offset;
    }



    return ICF_SUCCESS;
}


icf_uint32_t
icf_pack_icf_options_resp_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t    *p_length
)
{
    icf_uint32_t        offset = 0;

    icf_options_resp_st    *p_icf_options_resp_st = ICF_NULL;
    
    p_icf_options_resp_st    = (icf_options_resp_st *)p_src;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function packs icf_options_resp_st */
    ICF_CLIB_TRACE(("Packing icf_options_resp_st\n"));
    
    icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_options_resp_st->bit_mask), &offset);
    
    *p_length    += offset;

    icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_options_resp_st->result), &offset);
    
    *p_length    += offset;

    if(p_icf_options_resp_st->bit_mask & ICF_OPTIONS_RESP_CODE_PRESENT)
    {
        
		icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_options_resp_st->response_code_for_options), &offset);
			
		*p_length    += offset;
	}

    if(p_icf_options_resp_st->bit_mask & ICF_OPTIONS_ERROR_CODE_PRESENT)
    {
        icf_pack_U16(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_options_resp_st->error_code_for_options), &offset);
        
        *p_length    += offset;
    }

    if(p_icf_options_resp_st->bit_mask & ICF_HDR_LIST_PRESENT)
    {
        icf_pack_icf_header_list_st(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_options_resp_st->header_list), &offset);
        
        *p_length    += offset;
    }

    if(p_icf_options_resp_st->bit_mask & ICF_BODY_LIST_PRESENT)
    {
        icf_pack_icf_msg_body_list_st(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_options_resp_st->body_list), &offset);
        
        *p_length    += offset;
    }

    if(p_icf_options_resp_st->bit_mask & ICF_OPTIONS_REMOTE_STREAM_LIST_PRESENT)
    {
        icf_pack_icf_stream_capab_list_st(p_dest + (*p_length),(p_icf_options_resp_st->p_remote_stream_list), &offset);
        
        *p_length    += offset;
    }

    return(ICF_SUCCESS);
}

icf_uint32_t
icf_pack_icf_inc_option_ind_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t    *p_length
)
{
    icf_uint32_t        offset = 0;

    icf_inc_options_ind_st    *p_icf_inc_options_st = ICF_NULL;
    
    p_icf_inc_options_st    = (icf_inc_options_ind_st *)p_src;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function packs icf_options_resp_st */
    ICF_CLIB_TRACE(("Packing icf_options_resp_st\n"));
    
    icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_inc_options_st->bit_mask), &offset);
    
    *p_length    += offset;


    if(p_icf_inc_options_st->bit_mask & ICF_HDR_LIST_PRESENT)
    {
        icf_pack_icf_header_list_st(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_inc_options_st->header_list), &offset);
        
        *p_length    += offset;
    }
	
	if(p_icf_inc_options_st->bit_mask & ICF_TAG_LIST_PRESENT)
    {
        icf_pack_icf_tag_list_st(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_inc_options_st->tag_list), &offset);
        
        *p_length    += offset;
    }

    if(p_icf_inc_options_st->bit_mask & ICF_BODY_LIST_PRESENT)
    {
        icf_pack_icf_msg_body_list_st(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_inc_options_st->body_list), &offset);
        
        *p_length    += offset;
    }

    if(p_icf_inc_options_st->bit_mask & ICF_INC_OPTIONS_IND_MEDIA_PRESENT)
    {
        icf_pack_icf_stream_capab_list_st(p_dest + (*p_length),(p_icf_inc_options_st->p_app_sdp_list), &offset);
        
        *p_length    += offset;
    }
    icf_pack_icf_line_id_t(p_dest + (*p_length),(icf_uint8_t*)&(p_icf_inc_options_st->line_id), &offset);

        *p_length    += offset;
    return(ICF_SUCCESS);
}


icf_uint32_t
icf_pack_icf_set_line_param_req_st
(
 icf_uint8_t    *p_dest,
 icf_uint8_t    *p_src,
 icf_uint32_t    *p_length
)
{
    	icf_uint32_t        offset = 0;
 	icf_set_line_param_req_st   
 	*p_icf_set_line_param_req_st = ICF_NULL;

    /* Initialise length to 0 */
    *p_length    = 0;	
	p_icf_set_line_param_req_st    = 
	(icf_set_line_param_req_st *)p_src;

    /* This function packs icf_set_transport_req_st */
    	ICF_CLIB_TRACE(("Packing icf_set_line_param_req_st \n")); 
    
    icf_pack_icf_line_id_t(p_dest + (*p_length), (icf_uint8_t*)
	    &(p_icf_set_line_param_req_st ->line_id), &offset);
    	*p_length    += offset;
    
    icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)
	    &(p_icf_set_line_param_req_st ->bitmask), &offset);
    	*p_length    += offset;

	if (p_icf_set_line_param_req_st ->bitmask &
	    ICF_SET_LINE_PRM_SUBS_HS_TIMER)
    {
    	icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)
        	&(p_icf_set_line_param_req_st-> subscribe_head_start_timer), &offset);


    	*p_length    += offset;
    }
   	 if (p_icf_set_line_param_req_st->bitmask & ICF_SET_LINE_PRM_T1_TIMER)

    {
    	icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)
	&(p_icf_set_line_param_req_st ->sip_timer_t1), &offset);
    	*p_length    += offset;
    }
    			    if (p_icf_set_line_param_req_st ->bitmask &
	ICF_SET_LINE_PRM_T2_TIMER)
    {
    	icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)
	&(p_icf_set_line_param_req_st ->sip_timer_t2), &offset);
    	*p_length    += offset;
    }
    	if (p_icf_set_line_param_req_st ->bitmask &
	ICF_SET_LINE_PRM_INV_TIMER)
    {
    	icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)
	&(p_icf_set_line_param_req_st ->sip_timer_invite), &offset);
    	*p_length    += offset;
    }    
    	if (p_icf_set_line_param_req_st ->bitmask &
	ICF_SET_LINE_PRM_NON_INV_TIMER)
    {
      	icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)
	&(p_icf_set_line_param_req_st->sip_timer_noninvite), &offset);
    	*p_length    += offset;
    }
    /*Start: Per Line Feature */
    if (p_icf_set_line_param_req_st->bitmask &
        ICF_SET_LINE_SIP_DSCP)
    {
      icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)
      &(p_icf_set_line_param_req_st->sip_dscp), &offset);
      *p_length    += offset;
    }
    if (p_icf_set_line_param_req_st->bitmask &
        ICF_SET_LINE_RTP_DSCP)
    {
     icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)
     &(p_icf_set_line_param_req_st->rtp_dscp), &offset);
     *p_length    += offset;
    }

    #ifdef ICF_SESSION_TIMER
    if (p_icf_set_line_param_req_st->bitmask &
        ICF_SET_LINE_MIN_SE)
    {
     icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)
     &(p_icf_set_line_param_req_st->min_se), &offset);
     *p_length    += offset;
    }  
    if (p_icf_set_line_param_req_st->bitmask &
        ICF_SET_LINE_SESSION_TIMER)
    {
     icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)
     &(p_icf_set_line_param_req_st->session_timer), &offset);
     *p_length    += offset;
    } 
    if (p_icf_set_line_param_req_st->bitmask &
        ICF_SET_LINE_SESSION_TIMER_REFRESHER)
    {
     icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)
     &(p_icf_set_line_param_req_st->session_timer_refresher), &offset);
     *p_length    += offset;
    }
    #endif
    if (p_icf_set_line_param_req_st->bitmask &
        ICF_SET_LINE_AUDIO_CODEC)
     {
       icf_pack_icf_codec_attrib_list_st(p_dest + (*p_length), (p_icf_set_line_param_req_st->p_codec_attributes_audio),
       &offset,  ICF_STREAM_TYPE_AUDIO);
       *p_length    += offset;
     }
    if (p_icf_set_line_param_req_st->bitmask &
        ICF_SET_LINE_VIDEO_CODEC)
     {
       icf_pack_icf_codec_attrib_list_st(p_dest + (*p_length), (p_icf_set_line_param_req_st->p_codec_attributes_video),
       &offset,  ICF_STREAM_TYPE_VIDEO);
       *p_length    += offset;
     }  
	if(p_icf_set_line_param_req_st->bitmask & ICF_SET_LINE_CONTACT_USER_INFO)
    {
        icf_pack_icf_string_st(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_set_line_param_req_st->contact_user_info), &offset);
        *p_length    += offset;
    }
    
    if(p_icf_set_line_param_req_st->bitmask & ICF_STALE_PARAM_HANDLING_PRESENT)
    {
        icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*) &(p_icf_set_line_param_req_st->ignore_stale_param), &offset);

        *p_length    += offset;
    }

    if(p_icf_set_line_param_req_st->bitmask & ICF_LINE_CONFIG_SESSION_REFRESH_METHOD_PRESENT)
    {
        icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*) &(p_icf_set_line_param_req_st->session_refresh_method), &offset);

        *p_length    += offset;
    }

    /* End: Per Line Feature */

   /* Added for p-access-nw-info header handling */
   if(ICF_SET_LINE_PRM_ACCESS_NW_INFO & p_icf_set_line_param_req_st->bitmask)
    {
        icf_pack_icf_string_st(p_dest + (*p_length),(icf_uint8_t*)\
            &(p_icf_set_line_param_req_st->access_network_info), &offset);
        
        *p_length    += offset;
    }

   /* Pack the SIP T4 timer value in case configured by application */
   if (ICF_SET_LINE_PRM_T4_TIMER & p_icf_set_line_param_req_st->bitmask) 
   {
    	icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)
	        &(p_icf_set_line_param_req_st ->sip_timer_t4), &offset);
    	*p_length    += offset;
   }
   /*Fix for CSR 1-7334184 : SPR 19740 */
    if(ICF_SET_LINE_REMOVE_ROUTE_HDR & \
            p_icf_set_line_param_req_st->bitmask)
    {
        icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)\
                &(p_icf_set_line_param_req_st->remove_route_hdr), &offset);

        *p_length    += offset;
    }

    /*Fix for SPR: 19999 (Merged CSR 1-7547597 : SPR 19898)*/
    if(p_icf_set_line_param_req_st->bitmask & ICF_SET_LINE_SESSION_REFRESH_METHOD_STRICT_UPDATE)
    {
        icf_pack_U8(p_dest + (*p_length), 
                (icf_uint8_t*) &(p_icf_set_line_param_req_st->session_refresh_method_strict_update),
                &offset);

        *p_length    += offset;
    }

    /* Fix for CSR 1-9012191 : SPR 20697 */ 
    if(p_icf_set_line_param_req_st->bitmask & ICF_SET_LINE_STRIP_REQ_URI_PARAM) 
    {
        icf_pack_U8(p_dest + (*p_length), 
                (icf_uint8_t*) &(p_icf_set_line_param_req_st->strip_req_uri_param),
                &offset);

        *p_length    += offset;
    }
        
    return ICF_SUCCESS;
}


icf_uint32_t
icf_pack_icf_icf_options_cfm_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t    *p_length
)
{
    icf_uint32_t        offset = 0;
    icf_uint32_t        count = 0;

    icf_options_cfm_st    *p_icf_options_cfm_st = ICF_NULL;
    
    p_icf_options_cfm_st    = (icf_options_cfm_st *)p_src;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function packs icf_options_cfm_st */
    ICF_CLIB_TRACE(("Packing icf_options_cfm_st\n"));
    
    icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_options_cfm_st->bit_mask), &offset);
    
    *p_length    += offset;

    icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_options_cfm_st->result), &offset);
    
    *p_length    += offset;

    if(p_icf_options_cfm_st->bit_mask & ICF_HDR_LIST_PRESENT)
    {
        icf_pack_icf_header_list_st(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_options_cfm_st->header_list), &offset);
        
        *p_length    += offset;
    }
	
	if(p_icf_options_cfm_st->bit_mask & ICF_TAG_LIST_PRESENT)
    {
        icf_pack_icf_tag_list_st(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_options_cfm_st->tag_list), &offset);
        
        *p_length    += offset;
    }

    if(p_icf_options_cfm_st->bit_mask & ICF_BODY_LIST_PRESENT)
    {
        icf_pack_icf_msg_body_list_st(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_options_cfm_st->body_list), &offset);
        
        *p_length    += offset;
    }

    if(p_icf_options_cfm_st->bit_mask & ICF_OPTIONS_CFM_MEDIA_PRESENT)
    {
        icf_pack_icf_stream_capab_list_st(p_dest + (*p_length),(p_icf_options_cfm_st->p_app_sdp_list), &offset);
        
        *p_length    += offset;
    }
	
	if(p_icf_options_cfm_st->bit_mask & ICF_OPTIONS_CFM_DEL_MEDIA_PRESENT)
    {
        icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_options_cfm_st->count), &offset);
        
        *p_length    += offset;

		while((count < p_icf_options_cfm_st->count) && (count <ICF_MAX_STREAM_PER_CALL)) 
		{
			icf_pack_icf_stream_list_st(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_options_cfm_st->deleted_media_streams[count]), &offset);
        
	        *p_length    += offset;

			count++;
		}
    }
    if(p_icf_options_cfm_st->bit_mask & ICF_OPTIONS_CFM_FAILURE_RESP_PRESENT)
    {
        icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_options_cfm_st->response_code), &offset);
        
        *p_length    += offset;
    }

    return(ICF_SUCCESS);
}

icf_uint32_t
icf_pack_icf_info_cfm_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t    *p_length
)
{
    icf_uint32_t        offset = 0;

    icf_info_cfm_st    *p_info_cfm_st = ICF_NULL;
    
    p_info_cfm_st    = (icf_info_cfm_st *)p_src;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function packs icf_info_cfm_st */
    ICF_CLIB_TRACE(("Packing icf_info_cfm_st\n"));
    
    icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&(p_info_cfm_st->result), &offset);
    *p_length    += offset;

    icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&(p_info_cfm_st->bit_mask), &offset);
    *p_length    += offset;    

    if(p_info_cfm_st->bit_mask & ICF_INFO_CFM_ERROR_CAUSE_PRESENT)
    {
        icf_pack_U16(p_dest + (*p_length), (icf_uint8_t*)&(p_info_cfm_st->error_cause), &offset);
        *p_length    += offset;
    }

    if(p_info_cfm_st->bit_mask & ICF_INFO_CFM_RESPONSE_CODE_PRESENT)
    {
        icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&(p_info_cfm_st->response_code), &offset);
        *p_length    += offset;
    }

    if(p_info_cfm_st->bit_mask & ICF_INFO_CFM_HEADER_LIST_PRESENT)
    {
        icf_pack_icf_header_list_st(p_dest + (*p_length), (icf_uint8_t*)&(p_info_cfm_st->header_list), &offset);
        *p_length    += offset;
    }

    if(p_info_cfm_st->bit_mask & ICF_INFO_CFM_TAG_LIST_PRESENT)
    {
        icf_pack_icf_tag_list_st(p_dest + (*p_length), (icf_uint8_t*)&(p_info_cfm_st->tag_list), &offset);
        *p_length    += offset;
    }

    if(p_info_cfm_st->bit_mask & ICF_INFO_CFM_BODY_LIST_PRESENT)
    {
        icf_pack_icf_msg_body_list_st(p_dest + (*p_length), (icf_uint8_t*)&(p_info_cfm_st->body_list), &offset);
        *p_length    += offset;
    }

    return ICF_SUCCESS;
}

icf_uint32_t icf_pack_icf_hold_option_st( icf_uint8_t *p_dest,
                                          icf_uint8_t *p_src,
                                          icf_uint32_t *p_length )
{
 icf_uint32_t        offset = 0;
 icf_hold_option_st  *p_icf_hold_option_st = ICF_NULL;
 p_icf_hold_option_st = (icf_hold_option_st *)p_src;
 /* Initialise length to 0 */
 *p_length    = 0;
 /* This function packs icf_hold_option_st */
 ICF_CLIB_TRACE(("Packing icf_ hold_option _st\n"));
 icf_pack_icf_line_id_t(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_hold_option_st->line_id), &offset);
 *p_length += offset;
 icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_hold_option_st->hold_option), &offset);
 *p_length += offset;
 return(ICF_SUCCESS);
} 


icf_uint32_t 
icf_pack_icf_line_id_t
( 
    icf_uint8_t *p_dest,
    icf_uint8_t *p_src,
    icf_uint32_t *p_length
)
{
	icf_uint32_t offset = 0;
    *p_length    = 0;
    icf_pack_U16(p_dest + (*p_length), (icf_uint8_t*)(p_src), &offset);
    *p_length += offset;
    return(ICF_SUCCESS);
}



icf_uint32_t
icf_pack_icf_early_incoming_call_ind_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t   *p_length
)
{
    icf_uint32_t        offset = 0;

    icf_early_incoming_call_ind_st    *p_icf_early_call_ind = ICF_NULL;
    
    p_icf_early_call_ind    = (icf_early_incoming_call_ind_st *)p_src;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function packs icf_incoming_call_ind_st */
    ICF_CLIB_TRACE(("Packing icf_incoming_call_ind_st\n"));

    icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_early_call_ind->bit_mask), &offset);
    
    *p_length    += offset;

    if(p_icf_early_call_ind->bit_mask & ICF_EARLY_CALL_CLG_PTY_ADDR)
    {
        icf_pack_icf_address_st(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_early_call_ind->calling_party_addr), &offset);
        *p_length    += offset;
    }

    if(p_icf_early_call_ind->bit_mask & ICF_EARLY_CALL_CLG_PTY_NAME)
    {
        icf_pack_icf_string_st(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_early_call_ind->calling_party_name), &offset);
        
        *p_length    += offset;
    }

    if(p_icf_early_call_ind->bit_mask & ICF_EARLY_CALL_CALLED_PTY_ADDR)
    {
        icf_pack_icf_address_st(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_early_call_ind->called_party_addr), &offset);
        
        *p_length    += offset;
    }


    if(p_icf_early_call_ind->bit_mask & ICF_HDR_LIST_PRESENT)
    {
        icf_pack_icf_header_list_st(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_early_call_ind->header_list), &offset);
        
        *p_length    += offset;
    }

 

     return ICF_SUCCESS;
}



icf_uint32_t
icf_pack_icf_early_incoming_call_cfm_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t   *p_length
)
{
    icf_uint32_t				offset = 0;

    icf_early_incoming_call_cfm_st		*p_early_incoming_call_cfm = ICF_NULL;
    
    p_early_incoming_call_cfm    = (icf_early_incoming_call_cfm_st*)p_src;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function packs icf_info_cfm_st */
    ICF_CLIB_TRACE(("Packing icf_early_incoming_call_cfm_st\n"));

    icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&(p_early_incoming_call_cfm->bit_mask), &offset);
    
    *p_length    += offset;
    
    icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&(p_early_incoming_call_cfm->result), &offset);
    *p_length    += offset;

	icf_pack_icf_line_id_t(p_dest + (*p_length), (icf_uint8_t*)&(p_early_incoming_call_cfm->line_id), &offset);
	*p_length    += offset;

    if(p_early_incoming_call_cfm->bit_mask & ICF_EARLY_CALL_CFM_TUNNEL_MODE_OPERATION_PRESENT)
    {
       icf_pack_U8(p_dest + (*p_length), 
                   (icf_uint8_t*)&( p_early_incoming_call_cfm->tunnel_mode), 
                   &offset);
       *p_length    += offset;
    }


    if(p_early_incoming_call_cfm->bit_mask & ICF_EARLY_CALL_CFM_RESPONSE_CODE_PRESENT)
    {
        icf_pack_U16(p_dest + (*p_length), (icf_uint8_t*)&( p_early_incoming_call_cfm->response_code), &offset);
   
        *p_length    += offset;

    }

#ifdef ICF_SESSION_TIMER    
    if(ICF_EARLY_CALL_CFM_SESSION_TIMER_ENABLE_PRESENT & p_early_incoming_call_cfm->bit_mask)
    {
       icf_pack_U8(p_dest + (*p_length), 
                   (icf_uint8_t*)&(p_early_incoming_call_cfm->session_timer_enable), 
                   &offset);
       *p_length    += offset;
    }
#endif
    
    return ICF_SUCCESS;
}



#ifdef ICF_TRACE_ENABLE
icf_uint32_t icf_pack_icf_trace_level_req_st( icf_uint8_t *p_dest,
                                          icf_uint8_t *p_src,
                                          icf_uint32_t *p_length )
{
 icf_uint32_t        offset = 0;
 icf_set_trace_level_req_st *p_trace_level = ICF_NULL;
 p_trace_level = (icf_set_trace_level_req_st *)p_src;
 /* Initialise length to 0 */
 *p_length    = 0;
 /* This function packs icf_set_trace_level_req_st */
 ICF_CLIB_TRACE(("Packing icf_set_trace_level_req_st\n"));
 
 icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&(p_trace_level->status), &offset);
 *p_length += offset;
 
 icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&(p_trace_level->trace_level), &offset);
 *p_length += offset;
 
 icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&(p_trace_level->trace_type), &offset);
 *p_length += offset;
 
 return(ICF_SUCCESS);
}
#endif

#ifdef ICF_ERROR_ENABLE
icf_uint32_t icf_pack_icf_error_report_level_req_st( icf_uint8_t *p_dest,
                                          icf_uint8_t *p_src,
                                          icf_uint32_t *p_length )
{
 icf_uint32_t        offset = 0;
 icf_set_error_report_level_req_st *p_error_report_level = ICF_NULL;
 p_error_report_level = (icf_set_error_report_level_req_st *)p_src;
 /* Initialise length to 0 */
 *p_length    = 0;
 /* This function packs icf_set_trace_level_req_st */
 ICF_CLIB_TRACE(("Packing icf_set_error_report_level_req_st\n"));
 
 icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&(p_error_report_level->status), &offset);
 *p_length += offset;
 
 icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&(p_error_report_level->error_level), &offset);
 *p_length += offset;
 
 icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&(p_error_report_level->error_type), &offset);
 *p_length += offset;
 
 return(ICF_SUCCESS);
}

icf_uint32_t icf_pack_icf_report_error_ind_st( icf_uint8_t *p_dest,
                                          icf_uint8_t *p_src,
                                          icf_uint32_t *p_length )
{
 icf_uint32_t        offset = 0;
 icf_uint8_t        count = 0;
 icf_report_error_ind_st *p_error_report = ICF_NULL;
 p_error_report = (icf_report_error_ind_st*)p_src;
 /* Initialise length to 0 */
 *p_length    = 0;
 /* This function packs icf_report_error_ind_st */
 ICF_CLIB_TRACE(("Packing icf_report_error_ind_st\n"));
 
 icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&(p_error_report->error_id), &offset);
 *p_length += offset;
 
 icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&(p_error_report->error_data.num_of_integer_values), &offset);
 *p_length += offset;
 /* Pack the integer values
  */
 /*Corrected the const ICF_MAX_INT_TRACE_DATA to ICF_MAX_INT_ERROR_DATA for
   resolving compilation issue when TRACE flag is OFF */
 for(count = 0; (count < p_error_report->error_data.num_of_integer_values) && (count < ICF_MAX_INT_ERROR_DATA);\
         count++)
 {
     icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&(p_error_report->error_data.int_val[count]), &offset);
     *p_length += offset;
 }
 
icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&(p_error_report->error_data.number_of_string_values), &offset);
*p_length += offset;
/* Pack the munber of string values present in the array
 */
for(count = 0; (count < p_error_report->error_data.number_of_string_values) && (count < ICF_MAX_STR_ERROR_DATA);\
         count++)
 {
     icf_pack_icf_string_st(p_dest + (*p_length), (icf_uint8_t*)&(p_error_report->error_data.string_val[count]), &offset);
     *p_length += offset;
 }
 

icf_pack_icf_string_st(p_dest + (*p_length), (icf_uint8_t*)&(p_error_report->error_data.err_string), &offset);
 *p_length += offset;
 
 return(ICF_SUCCESS);
}

#endif  



icf_uint32_t
icf_pack_icf_media_connect_req_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t   *p_length
)
{
    icf_uint32_t        offset = 0;
    icf_uint8_t         count = 0;

    icf_media_connect_req_st    *p_icf_media_connect_req_st = ICF_NULL;
    
    p_icf_media_connect_req_st   = (icf_media_connect_req_st *)p_src;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function packs icf_media_connect_req_st*/
    ICF_CLIB_TRACE(("Packing icf_media_connect_req_st\n"));
    
    icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&(\
                p_icf_media_connect_req_st->bit_mask), &offset);
    
    *p_length    += offset;

    if(p_icf_media_connect_req_st->bit_mask & ICF_PRIVACY_IND_PRESENT)
    {
        icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&(\
                    p_icf_media_connect_req_st->privacy_ind), &offset);
        
        *p_length    += offset;
    }

    if(p_icf_media_connect_req_st->bit_mask & ICF_HDR_LIST_PRESENT)
    {
        icf_pack_icf_header_list_st(p_dest + (*p_length), (icf_uint8_t*)&(\
                    p_icf_media_connect_req_st->header_list), &offset);
        
        *p_length    += offset;
    }

    if(p_icf_media_connect_req_st->bit_mask & ICF_TAG_LIST_PRESENT)
    {
        icf_pack_icf_tag_list_st(p_dest + (*p_length), (icf_uint8_t*)&(\
                    p_icf_media_connect_req_st->tag_list), &offset);
        
        *p_length    += offset;
    }

    if(p_icf_media_connect_req_st->bit_mask & ICF_BODY_LIST_PRESENT)
    {
        icf_pack_icf_msg_body_list_st(p_dest + (*p_length), (icf_uint8_t*)&(\
                    p_icf_media_connect_req_st->body_list), &offset);
        
        *p_length    += offset;
    }

/*ICF REL 6.0 : multiple m lines*/
	if(p_icf_media_connect_req_st->bit_mask & ICF_STREAM_LIST_PRESENT)
	{
		icf_pack_icf_stream_capab_list_st(p_dest + (*p_length), (\
                    p_icf_media_connect_req_st->p_stream_list),  &offset);
	
		*p_length	+= offset;
	}

    if(ICF_MEDIA_CONNECT_MEDIA_MODE_PRESENT & p_icf_media_connect_req_st->bit_mask)
    {
      icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&(\
          p_icf_media_connect_req_st->media_mode), &offset);
      *p_length    += offset;
    }

	if(p_icf_media_connect_req_st->bit_mask & ICF_MEDIA_CONNECT_DELETED_STREAM_PRESENT)
    {
		icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&(\
                    p_icf_media_connect_req_st->count), &offset);
        
        *p_length    += offset;

		while((count<p_icf_media_connect_req_st->count) && (count < ICF_MAX_STREAM_PER_CALL))
		{
			icf_pack_icf_stream_list_st(p_dest + (*p_length), (icf_uint8_t*)&(\
               p_icf_media_connect_req_st->deleted_media_streams[count]), &offset);
        
	        *p_length    += offset;

			count++;
		}
    }
    icf_pack_U8(
            p_dest + (*p_length), 
            (icf_uint8_t *)&(p_icf_media_connect_req_st->send_without_sdp), 
            &offset);

	*p_length    += offset;

    if(ICF_MEDIA_CONNECT_REQ_PRACK_VALUE_PRESENT & p_icf_media_connect_req_st->bit_mask)
    {
        icf_pack_U32(p_dest + (*p_length), 
                (icf_uint8_t*)&(p_icf_media_connect_req_st->prack_value), 
                &offset);
        *p_length    += offset;
    }


    return ICF_SUCCESS;
}

icf_uint32_t
icf_pack_icf_unexpected_msg_ind_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t    *p_length
)
{
    icf_uint32_t        offset = 0;

    icf_unexpected_msg_ind_st    *p_icf_unexpected_msg_ind_st = ICF_NULL;

    p_icf_unexpected_msg_ind_st = (icf_unexpected_msg_ind_st *)p_src;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function packs icf_unexpected_msg_ind_st */
    ICF_CLIB_TRACE(("Packing icf_unexpected_msg_ind_st\n"));

    icf_pack_U16(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_unexpected_msg_ind_st->error_cause), &offset);

    *p_length    += offset;

    icf_pack_U16(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_unexpected_msg_ind_st->api_id), &offset);

    *p_length    += offset;

    return ICF_SUCCESS;
}

icf_uint32_t icf_pack_icf_stop_fax_resp_st( icf_uint8_t *p_dest,
                                            icf_uint8_t *p_src,
                                            icf_uint32_t *p_length )
{
   icf_uint32_t           offset = 0;
   icf_stop_fax_resp_st  *p_stop_fax_resp = ICF_NULL;

   p_stop_fax_resp = (icf_stop_fax_resp_st *)p_src;
   /* Initialise length to 0 */
   *p_length    = 0;
   /* This function packs icf_stop_fax_resp_st */

   ICF_CLIB_TRACE(("Packing icf_stop_fax_resp_st \n"));
    
   icf_pack_U32(p_dest + (*p_length), 
        (icf_uint8_t*)&(p_stop_fax_resp->bit_mask), &offset);
        
    *p_length    += offset;

   icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&(p_stop_fax_resp->result), &offset);
   *p_length    += offset;

   icf_pack_U16(p_dest + (*p_length), (icf_uint8_t*)&(p_stop_fax_resp->error_cause), &offset);
   *p_length    += offset;
       
   if(p_stop_fax_resp->bit_mask & ICF_RETRY_AFTER_DURATION_PRESENT)
   {
        icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&
              (p_stop_fax_resp->retry_after_duration), &offset);

	 *p_length    += offset;
         
   }

   return(ICF_SUCCESS);
}

icf_uint32_t icf_pack_icf_stop_fax_ind_st( icf_uint8_t *p_dest,
                                            icf_uint8_t *p_src,
                                            icf_uint32_t *p_length )
{
   icf_uint32_t           offset = 0;
   icf_stop_fax_ind_st  *p_stop_fax_ind = ICF_NULL;

   p_stop_fax_ind = (icf_stop_fax_ind_st *)p_src;

   /* Initialise length to 0 */
   *p_length    = 0;
   /* This function packs icf_stop_fax_ind_st */
   ICF_CLIB_TRACE(("Packing icf_stop_fax_ind_st \n"));

   icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&(p_stop_fax_ind->fax_type), &offset);
   *p_length    += offset;

   return(ICF_SUCCESS);
}

/*call_mod_cancel*/
icf_uint32_t icf_pack_icf_call_mod_cancel_resp_st( icf_uint8_t *p_dest,
                                            icf_uint8_t *p_src,
                                            icf_uint32_t *p_length )
{
   icf_uint32_t           offset = 0;
   icf_call_modify_cancel_resp_st  *p_call_modify_cancel_resp = ICF_NULL;

   p_call_modify_cancel_resp = (icf_call_modify_cancel_resp_st *)p_src;
   /* Initialise length to 0 */
   *p_length    = 0;
   /* This function packs icf_call_modify_cancel_resp_st */

   ICF_CLIB_TRACE(("Packing icf_call_modify_cancel_resp_st \n"));

   icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&(p_call_modify_cancel_resp->result), &offset);
   *p_length    += offset;

   icf_pack_U16(p_dest + (*p_length), (icf_uint8_t*)&(p_call_modify_cancel_resp->error_cause), &offset);
   *p_length    += offset;

   return(ICF_SUCCESS);
}

icf_uint32_t icf_pack_icf_remote_call_transfer_initiated_cfm_st( icf_uint8_t *p_dest,
                                            icf_uint8_t *p_src,
                                            icf_uint32_t *p_length )
{
   icf_uint32_t           offset = 0;
   icf_remote_call_transfer_initiated_cfm_st  *p_cti_cfm = ICF_NULL;

   p_cti_cfm = (icf_remote_call_transfer_initiated_cfm_st *)p_src;

   /* Initialise length to 0 */
   *p_length    = 0;
   /* This function packs icf_stop_fax_ind_st */
   ICF_CLIB_TRACE(("Packing icf_pack_icf_remote_call_transfer_initiated_cfm_st \n"));

   icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&(p_cti_cfm->bit_mask), &offset);
   *p_length    += offset;

   icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&(p_cti_cfm->result), &offset);
   *p_length    += offset;

   if (ICF_CALL_TRANS_INIT_ERROR_CAUSE_PRESENT & p_cti_cfm->bit_mask)
   {
       icf_pack_U16(p_dest + (*p_length), (icf_uint8_t*)&(p_cti_cfm->error_code), &offset);
       *p_length    += offset;
   }

   if (ICF_CALL_TRANS_INIT_CFM_CODE_PRESENT & p_cti_cfm->bit_mask)
   {
       icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&(p_cti_cfm->response_code), &offset);
       *p_length    += offset;
   }

   /* SPR 19218 : Merged CSR 1-5478785 */
   if(p_cti_cfm->bit_mask & ICF_HDR_LIST_PRESENT)
   {
        icf_pack_icf_header_list_st(p_dest + (*p_length), (icf_uint8_t*)&(\
                    p_cti_cfm->header_list), &offset);
        
        *p_length    += offset;
   }

   return(ICF_SUCCESS);
}



icf_uint32_t
icf_pack_icf_user_reg_status_st
(
  icf_uint8_t    *p_dest,
  icf_list_st    *p_src,
  icf_uint32_t    *p_length
)
{
   icf_uint32_t        offset = 0;

   icf_user_reg_status_st  *p_icf_user_reg_status_st = ICF_NULL;

   p_icf_user_reg_status_st = (icf_user_reg_status_st *)p_src;

   /* Initialise length to 0 */
   *p_length    = 0;

   /* This function packs icf_user_reg_status_st */
   ICF_CLIB_TRACE(("Packing icf_user_reg_status_st\n"));

   icf_pack_icf_address_st(p_dest + (*p_length),
             (icf_uint8_t*)&(p_icf_user_reg_status_st->user),&offset);

          *p_length += offset;

   icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&
            (p_icf_user_reg_status_st->reg_status), &offset);

          *p_length += offset;

   return ICF_SUCCESS;

 }



icf_uint32_t
icf_pack_icf_user_reg_status_list_st
(
    icf_uint8_t    *p_dest,
    icf_list_st    *p_icf_user_reg_status_list_st,
    icf_uint32_t    *p_length
)
{
    icf_uint32_t        offset = 0;
    icf_uint8_t        count = 0;

    icf_list_st    *p_curr_node = ICF_NULL;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function packs icf_uri_list_st */
    ICF_CLIB_TRACE(("Packing icf_reg_status_list_st \n"));


    icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&count, &offset);

    *p_length    += offset;

    /* This piece of code packs the fields present in 
     * icf_user_reg_status_st structure */


    for(p_curr_node = p_icf_user_reg_status_list_st;
    p_curr_node!=ICF_NULL;
    p_curr_node = p_curr_node->p_next)
    {
        icf_pack_icf_user_reg_status_st(p_dest + (*p_length),
                         p_curr_node->p_data,&offset);

        *p_length    += offset;


        count++;
    }
        if(count)
        {

                /* Pack the count */
                icf_pack_U8(p_dest, (icf_uint8_t*)&count, &offset);
        }


    return ICF_SUCCESS;
}

#ifdef ICF_STAT_ENABLE
/*****************************************************************************
*
* FUNCTION: icf_pack_icf_stats_req_st
*
* DESCRIPTION: This function is used to pack the payload of API
*              ICF_STATS_REQ.
*
*****************************************************************************/


icf_uint32_t
icf_pack_icf_stats_req_st(icf_uint8_t    *p_dest,
                             icf_uint8_t    *p_src,
                             icf_uint32_t    *p_length)
{

        icf_uint32_t        offset = 0;

        icf_stats_req_st  *p_icf_stats_req_st = (icf_stats_req_st *)p_src;

          /* This function packs icf_stats_req_st */
        ICF_CLIB_TRACE(("Packing icf_stats_req_st\n"));

         /* Initialise length to 0 */
        *p_length    = 0;

        icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&
                         (p_icf_stats_req_st->reset_stats), &offset);

        *p_length    += offset;

        return ICF_SUCCESS;
}

/*****************************************************************************
*
* FUNCTION: icf_pack_icf_stats_resp_st
*
* DESCRIPTION: This function is used to pack the payload of API
*              ICF_STATS_RESP.
*
*****************************************************************************/


icf_uint32_t
icf_pack_icf_stats_resp_st(icf_uint8_t    *p_dest,
                             icf_uint8_t    *p_src,
                             icf_uint32_t    *p_length)
{

        icf_uint32_t        offset = 0;

        icf_stats_resp_st  *p_icf_stats_resp_st = (icf_stats_resp_st *)p_src;

          /* This function packs icf_stats_req_st */
        ICF_CLIB_TRACE(("Packing icf_stats_resp_st\n"));

         /* Initialise length to 0 */
        *p_length    = 0;

/* PACKING OF TCP CONNECTIONS STAT*/

        icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&
           (p_icf_stats_resp_st->stats_data.stat_tcp_connections.
                                    stats_active_conn), &offset);

        *p_length    += offset;

        icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&
           (p_icf_stats_resp_st->stats_data.stat_tcp_connections.
                     total_tcp_conn_since_last_reboot), &offset);

        *p_length    += offset;

       icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&
          (p_icf_stats_resp_st->stats_data.stat_tcp_connections.
                                  stats_send_failures), &offset);

        *p_length    += offset;

       icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&
         (p_icf_stats_resp_st->stats_data.stat_tcp_connections.
                                 stats_conn_failures), &offset);

        *p_length    += offset;

      icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&
         (p_icf_stats_resp_st->stats_data.stat_tcp_connections.
                 stats_remote_closure_for_local_conn), &offset);

        *p_length    += offset;

/* PACKING OF UDP CONNECTION STAT*/


     icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&
         (p_icf_stats_resp_st->stats_data.stat_udp_connections.
                               stats_send_failures), &offset);

        *p_length    += offset;

/* PACKING OF TLS CONNECTION STAT*/

    icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&
         (p_icf_stats_resp_st->stats_data.stat_tls_connections.
                                 stats_active_conn), &offset);

        *p_length    += offset;

    icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&
        (p_icf_stats_resp_st->stats_data.stat_tls_connections.
                  total_tls_conn_since_last_reboot), &offset);

        *p_length    += offset;

    icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&
        (p_icf_stats_resp_st->stats_data.stat_tls_connections.
                               stats_send_failures), &offset);

        *p_length    += offset;

    icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&
        (p_icf_stats_resp_st->stats_data.stat_tls_connections.
                               stats_conn_failures), &offset);

        *p_length    += offset;

    icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&
        (p_icf_stats_resp_st->stats_data.stat_tls_connections.
               stats_remote_closure_for_local_conn), &offset);

        *p_length    += offset;

    icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&
        (p_icf_stats_resp_st->stats_data.stat_tls_connections.
                            stats_handshake_errors), &offset);

        *p_length    += offset;

/* PACKING OF REGISTER STAT*/

    icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&
        (p_icf_stats_resp_st->stats_data.stat_register.
                                  stats_active_reg), &offset);

        *p_length    += offset;

    icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&
        (p_icf_stats_resp_st->stats_data.stat_register.
                                  stats_failed_reg), &offset);

        *p_length    += offset;

    icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&
       (p_icf_stats_resp_st->stats_data.stat_register.
                                   stats_dereg_done), &offset);

        *p_length    += offset;

    icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&
       (p_icf_stats_resp_st->stats_data.stat_register.
                               stats_user_registered), &offset);

        *p_length    += offset;

    icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&
       (p_icf_stats_resp_st->stats_data.stat_register.
                                stats_registrar_addr), &offset);

        *p_length    += offset;

    icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&
       (p_icf_stats_resp_st->stats_data.stat_register.
                                    stats_rereg_done), &offset);

        *p_length    += offset;

    /***********************ICF 8.0********************/
    /*Packing of register stat for ,number of reg event subscription done
      corresponding to successful registration and for number of network 
      initiated deregistration done */    
    icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&
       (p_icf_stats_resp_st->stats_data.stat_register.
                                   stats_reg_event_package_subs), &offset);

        *p_length    += offset;

    icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&
       (p_icf_stats_resp_st->stats_data.stat_register.
                                    stats_nw_dereg_done), &offset);

        *p_length    += offset;        

    /**************************************************/
   /**************AKA ENH STATS **********************/
    icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&
       (p_icf_stats_resp_st->stats_data.stat_register.
                                  stats_reg_guard_timer_expire), &offset);
    *p_length    += offset;

	icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&
       (p_icf_stats_resp_st->stats_data.stat_register.
                                  stats_reg_auth_received), &offset);
    *p_length    += offset;

	icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&
       (p_icf_stats_resp_st->stats_data.stat_register.
                                   stats_reg_auth_success), &offset);
    *p_length    += offset;

	icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&
       (p_icf_stats_resp_st->stats_data.stat_register.
                                   stats_reg_auth_failure), &offset);
    *p_length    += offset;
    /***********************************************************/


/* PACKING OF REQUEST MESSAGES STAT*/


    icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&
       (p_icf_stats_resp_st->stats_data.stat_requests.stat_outbound_req.
                                          stats_INVITE), &offset);

        *p_length    += offset;

    icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&
       (p_icf_stats_resp_st->stats_data.stat_requests.stat_outbound_req.
                                            stats_PRACK), &offset);

        *p_length    += offset;

    icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&
      (p_icf_stats_resp_st->stats_data.stat_requests.stat_outbound_req.
                                           stats_UPDATE), &offset);

        *p_length    += offset;

    icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&
      (p_icf_stats_resp_st->stats_data.stat_requests.stat_outbound_req.
                                         stats_ReINVITE), &offset);

        *p_length    += offset;

    icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&
      (p_icf_stats_resp_st->stats_data.stat_requests.stat_outbound_req.
                                             stats_INFO), &offset);

        *p_length    += offset;

    icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&
      (p_icf_stats_resp_st->stats_data.stat_requests.stat_outbound_req.
                                     stats_outdlg_OPTION), &offset);

        *p_length    += offset;

    icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&
      (p_icf_stats_resp_st->stats_data.stat_requests.stat_outbound_req.
                                      stats_indlg_OPTION), &offset);

        *p_length    += offset;

    icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&
      (p_icf_stats_resp_st->stats_data.stat_requests.stat_outbound_req.
                                            stats_CANCEL), &offset);

        *p_length    += offset;

   icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&
      (p_icf_stats_resp_st->stats_data.stat_requests.stat_outbound_req.
                                               stats_ACK), &offset);

        *p_length    += offset;

   icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&
     (p_icf_stats_resp_st->stats_data.stat_requests.stat_outbound_req.
                                               stats_BYE), &offset);

        *p_length    += offset;

   icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&
     (p_icf_stats_resp_st->stats_data.stat_requests.stat_outbound_req.
                                          stats_REGISTER), &offset);

        *p_length    += offset;

   icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&
     (p_icf_stats_resp_st->stats_data.stat_requests.stat_outbound_req.
                                          stats_SUBSCRIBE), &offset);

        *p_length    += offset;

   icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&
     (p_icf_stats_resp_st->stats_data.stat_requests.stat_outbound_req.
                                          stats_PUBLISH), &offset);
        *p_length    += offset;

   icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&
     (p_icf_stats_resp_st->stats_data.stat_requests.stat_outbound_req.
                                          stats_MESSAGE), &offset);
       *p_length    += offset;

   icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&
     (p_icf_stats_resp_st->stats_data.stat_requests.stat_outbound_req.
                                              stats_REFER), &offset);

	*p_length    += offset;

   icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&
     (p_icf_stats_resp_st->stats_data.stat_requests.stat_outbound_req.
                                              stats_NOTIFY), &offset);

	*p_length    += offset;
    
    icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&
       (p_icf_stats_resp_st->stats_data.stat_requests.stat_outbound_req.
                                          stats_qos_INVITE), &offset);

        *p_length    += offset;
   
   icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&
       (p_icf_stats_resp_st->stats_data.stat_requests.stat_outbound_retrans_req.
                                          stats_INVITE), &offset);

        *p_length    += offset;

    icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&
       (p_icf_stats_resp_st->stats_data.stat_requests.stat_outbound_retrans_req.
                                            stats_PRACK), &offset);

        *p_length    += offset;

    icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&
      (p_icf_stats_resp_st->stats_data.stat_requests.stat_outbound_retrans_req.
                                           stats_UPDATE), &offset);

        *p_length    += offset;

    icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&
      (p_icf_stats_resp_st->stats_data.stat_requests.stat_outbound_retrans_req.
                                             stats_INFO), &offset);

        *p_length    += offset;

    icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&
      (p_icf_stats_resp_st->stats_data.stat_requests.stat_outbound_retrans_req.
                                     stats_OPTION), &offset);

        *p_length    += offset;

    icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&
      (p_icf_stats_resp_st->stats_data.stat_requests.stat_outbound_retrans_req.
                                            stats_CANCEL), &offset);

        *p_length    += offset;


   icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&
     (p_icf_stats_resp_st->stats_data.stat_requests.stat_outbound_retrans_req.
                                               stats_BYE), &offset);

        *p_length    += offset;

   icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&
     (p_icf_stats_resp_st->stats_data.stat_requests.stat_outbound_retrans_req.
                                          stats_REGISTER), &offset);

        *p_length    += offset;

   icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&
     (p_icf_stats_resp_st->stats_data.stat_requests.stat_outbound_retrans_req.
                                          stats_SUBSCRIBE), &offset);

        *p_length    += offset;


   icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&
     (p_icf_stats_resp_st->stats_data.stat_requests.stat_outbound_retrans_req.
                                          stats_PUBLISH), &offset);
       *p_length    += offset;

   icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&
     (p_icf_stats_resp_st->stats_data.stat_requests.stat_outbound_retrans_req.
                                          stats_MESSAGE), &offset);

        *p_length    += offset;


   icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&
     (p_icf_stats_resp_st->stats_data.stat_requests.stat_outbound_retrans_req.
                                              stats_REFER), &offset);

	*p_length    += offset;

   icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&
     (p_icf_stats_resp_st->stats_data.stat_requests.stat_outbound_retrans_req.
                                              stats_NOTIFY), &offset);

	*p_length    += offset;

   icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&
    (p_icf_stats_resp_st->stats_data.stat_requests.stat_inbound_req.
                                              stats_INVITE), &offset);

        *p_length    += offset;

   icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&
    (p_icf_stats_resp_st->stats_data.stat_requests.stat_inbound_req.
                                               stats_PRACK), &offset);

        *p_length    += offset;

   icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&
    (p_icf_stats_resp_st->stats_data.stat_requests.stat_inbound_req.
                                             stats_UPDATE), &offset);

        *p_length    += offset;

   icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&
    (p_icf_stats_resp_st->stats_data.stat_requests.stat_inbound_req.
                                           stats_ReINVITE), &offset);

        *p_length    += offset;

   icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&
    (p_icf_stats_resp_st->stats_data.stat_requests.stat_inbound_req.
                                               stats_INFO), &offset);

        *p_length    += offset;
   
   icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&
    (p_icf_stats_resp_st->stats_data.stat_requests.stat_inbound_req.
                                               stats_MESSAGE), &offset);
        *p_length    += offset;
 
   icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&
    (p_icf_stats_resp_st->stats_data.stat_requests.stat_inbound_req.
                                      stats_outdlg_OPTION), &offset);

       *p_length    += offset;

   icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&
    (p_icf_stats_resp_st->stats_data.stat_requests.stat_inbound_req.
                                       stats_indlg_OPTION), &offset);

        *p_length    += offset;

   icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&
    (p_icf_stats_resp_st->stats_data.stat_requests.stat_inbound_req.
                                             stats_CANCEL), &offset);

        *p_length    += offset;

   icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&
    (p_icf_stats_resp_st->stats_data.stat_requests.stat_inbound_req.
                                               stats_ACK), &offset);

        *p_length    += offset;

   icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&
    (p_icf_stats_resp_st->stats_data.stat_requests.stat_inbound_req.
                                              stats_BYE), &offset);

        *p_length    += offset;

   icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&
    (p_icf_stats_resp_st->stats_data.stat_requests.stat_inbound_req.
                                           stats_NOTIFY), &offset);

        *p_length    += offset;
    
   icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&
    (p_icf_stats_resp_st->stats_data.stat_requests.stat_inbound_req.
                                           stats_REFER), &offset);

        *p_length    += offset;
   icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&
       (p_icf_stats_resp_st->stats_data.stat_requests.stat_inbound_req.
                                          stats_qos_INVITE), &offset);

        *p_length    += offset;


/* PACKING OF STATS OF RESPONSES */


   icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&
     (p_icf_stats_resp_st->stats_data.stat_responses.
                   stat_outbound_invite_resp.stats_100), &offset);

        *p_length    += offset;

   icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&
     (p_icf_stats_resp_st->stats_data.stat_responses.stat_outbound_invite_resp.
                                             stats_2xx), &offset);

        *p_length    += offset;

   icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&
      (p_icf_stats_resp_st->stats_data.stat_responses.
                    stat_outbound_invite_resp.stats_180), &offset);

        *p_length    += offset;

   icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&
      (p_icf_stats_resp_st->stats_data.stat_responses.
                    stat_outbound_invite_resp.stats_181), &offset);

        *p_length    += offset;
   icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&
      (p_icf_stats_resp_st->stats_data.stat_responses.
                    stat_outbound_invite_resp.stats_182), &offset);

        *p_length    += offset;

   icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&
      (p_icf_stats_resp_st->stats_data.stat_responses.
                    stat_outbound_invite_resp.stats_183), &offset);

        *p_length    += offset;

   /*Fix merged for CSR_1_5417045*/
   icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&
      (p_icf_stats_resp_st->stats_data.stat_responses.
                    stat_outbound_invite_resp.stats_300), &offset);

        *p_length    += offset;

   icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&
      (p_icf_stats_resp_st->stats_data.stat_responses.
                    stat_outbound_invite_resp.stats_301), &offset);

        *p_length    += offset;

   icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&
      (p_icf_stats_resp_st->stats_data.stat_responses.
                    stat_outbound_invite_resp.stats_302), &offset);

        *p_length    += offset;

   icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&
      (p_icf_stats_resp_st->stats_data.stat_responses.
                    stat_outbound_invite_resp.stats_305), &offset);

        *p_length    += offset;

   icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&
      (p_icf_stats_resp_st->stats_data.stat_responses.
                    stat_outbound_invite_resp.stats_380), &offset);

        *p_length    += offset;
        

   icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&
      (p_icf_stats_resp_st->stats_data.stat_responses.
                    stat_outbound_invite_resp.stats_3xx), &offset);

        *p_length    += offset;

   icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&
      (p_icf_stats_resp_st->stats_data.stat_responses.
                    stat_outbound_invite_resp.stats_486), &offset);

        *p_length    += offset;

   icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&
      (p_icf_stats_resp_st->stats_data.stat_responses.
                    stat_outbound_invite_resp.stats_404), &offset);

        *p_length    += offset;

   icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&
      (p_icf_stats_resp_st->stats_data.stat_responses.
                    stat_outbound_invite_resp.stats_415), &offset);

        *p_length    += offset;

   icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&
      (p_icf_stats_resp_st->stats_data.stat_responses.
                     stat_outbound_invite_resp.stats_407), &offset);

        *p_length    += offset;

   icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&
      (p_icf_stats_resp_st->stats_data.stat_responses.
                      stat_outbound_invite_resp.stats_401), &offset);

        *p_length    += offset;

   icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&
      (p_icf_stats_resp_st->stats_data.stat_responses.
                      stat_outbound_invite_resp.stats_491), &offset);

        *p_length    += offset;

   icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&
      (p_icf_stats_resp_st->stats_data.stat_responses.
                      stat_outbound_invite_resp.stats_481), &offset);

        *p_length    += offset;

   icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&
      (p_icf_stats_resp_st->stats_data.stat_responses.
                      stat_outbound_invite_resp.stats_402), &offset);

        *p_length    += offset;

icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&
      (p_icf_stats_resp_st->stats_data.stat_responses.
                      stat_outbound_invite_resp.stats_406), &offset);

        *p_length    += offset;
icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&
      (p_icf_stats_resp_st->stats_data.stat_responses.
                      stat_outbound_invite_resp.stats_408), &offset);

        *p_length    += offset;
icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&
      (p_icf_stats_resp_st->stats_data.stat_responses.
                      stat_outbound_invite_resp.stats_409), &offset);

        *p_length    += offset;
icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&
      (p_icf_stats_resp_st->stats_data.stat_responses.
                      stat_outbound_invite_resp.stats_410), &offset);

        *p_length    += offset;
icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&
      (p_icf_stats_resp_st->stats_data.stat_responses.
                      stat_outbound_invite_resp.stats_413), &offset);

        *p_length    += offset;
icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&
      (p_icf_stats_resp_st->stats_data.stat_responses.
                      stat_outbound_invite_resp.stats_482), &offset);

        *p_length    += offset;
icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&
      (p_icf_stats_resp_st->stats_data.stat_responses.
                      stat_outbound_invite_resp.stats_483), &offset);

        *p_length    += offset;
icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&
      (p_icf_stats_resp_st->stats_data.stat_responses.
                      stat_outbound_invite_resp.stats_489), &offset);

        *p_length    += offset;

   icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&
      (p_icf_stats_resp_st->stats_data.stat_responses.
                      stat_outbound_invite_resp.stats_503), &offset);

       *p_length    += offset;

   icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&
      (p_icf_stats_resp_st->stats_data.stat_responses.
                      stat_outbound_invite_resp.stats_488), &offset);

        *p_length    += offset;

   icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&
      (p_icf_stats_resp_st->stats_data.stat_responses.
                       stat_outbound_invite_resp.stats_480), &offset);

        *p_length    += offset;

   icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&
      (p_icf_stats_resp_st->stats_data.stat_responses.
                        stat_outbound_invite_resp.stats_500), &offset);

        *p_length    += offset;

   icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&
      (p_icf_stats_resp_st->stats_data.stat_responses.
                         stat_outbound_invite_resp.stats_403), &offset);

        *p_length    += offset;

   icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&
      (p_icf_stats_resp_st->stats_data.stat_responses.
                          stat_outbound_invite_resp.stats_487), &offset);

        *p_length    += offset;

   icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&
      (p_icf_stats_resp_st->stats_data.stat_responses.
                          stat_outbound_invite_resp.stats_400), &offset);

        *p_length    += offset;

   icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&
      (p_icf_stats_resp_st->stats_data.stat_responses.
                          stat_outbound_invite_resp.stats_420), &offset);

        *p_length    += offset;


   icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&
      (p_icf_stats_resp_st->stats_data.stat_responses.
                           stat_outbound_invite_resp.stats_423), &offset);

        *p_length    += offset;

   icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&
      (p_icf_stats_resp_st->stats_data.stat_responses.
                            stat_outbound_invite_resp.stats_484), &offset);

        *p_length    += offset;

   icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&
      (p_icf_stats_resp_st->stats_data.stat_responses.
                            stat_outbound_invite_resp.stats_485), &offset);

        *p_length    += offset;

   icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&
      (p_icf_stats_resp_st->stats_data.stat_responses.
                             stat_outbound_invite_resp.stats_501), &offset);

        *p_length    += offset;

   icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&
      (p_icf_stats_resp_st->stats_data.stat_responses.
                              stat_outbound_invite_resp.stats_502), &offset);

        *p_length    += offset;

   icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&
      (p_icf_stats_resp_st->stats_data.stat_responses.
                              stat_outbound_invite_resp.stats_504), &offset);

        *p_length    += offset;

   icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&
      (p_icf_stats_resp_st->stats_data.stat_responses.
                               stat_outbound_invite_resp.stats_505), &offset);

        *p_length    += offset;

   icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&
      (p_icf_stats_resp_st->stats_data.stat_responses.
                             stat_outbound_invite_resp.stats_580), &offset);

        *p_length    += offset;

   icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&
      (p_icf_stats_resp_st->stats_data.stat_responses.
                             stat_outbound_invite_resp.stats_600), &offset);

        *p_length    += offset;

   icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&
      (p_icf_stats_resp_st->stats_data.stat_responses.
                             stat_outbound_invite_resp.stats_603), &offset);

        *p_length    += offset;
  icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&
      (p_icf_stats_resp_st->stats_data.stat_responses.
                             stat_outbound_invite_resp.stats_604), &offset);

        *p_length    += offset;
  icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&
      (p_icf_stats_resp_st->stats_data.stat_responses.
                             stat_outbound_invite_resp.stats_606), &offset);

        *p_length    += offset;


   icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&
      (p_icf_stats_resp_st->stats_data.stat_responses.
                         stat_outbound_invite_resp.stats_rest_4xx), &offset);

        *p_length    += offset;

   icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&
     (p_icf_stats_resp_st->stats_data.stat_responses.
                          stat_outbound_invite_resp.stats_rest_5xx), &offset);

        *p_length    += offset;

   icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&
      (p_icf_stats_resp_st->stats_data.stat_responses.
                stat_outbound_invite_resp.stats_rest_6xx), &offset);

        *p_length    += offset;



   icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&
     (p_icf_stats_resp_st->stats_data.stat_responses.
                             stat_outbound_cancel_resp.stats_200), &offset);

        *p_length    += offset;

   icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&
     (p_icf_stats_resp_st->stats_data.stat_responses.
                  stat_outbound_cancel_resp.stats_other_resp), &offset);

        *p_length    += offset;




   icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&
     (p_icf_stats_resp_st->stats_data.stat_responses.
                          stat_outbound_prack_resp.stats_200), &offset);

        *p_length    += offset;

   icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&
     (p_icf_stats_resp_st->stats_data.stat_responses.
                   stat_outbound_prack_resp.stats_other_resp), &offset);

        *p_length    += offset;




   icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&
      (p_icf_stats_resp_st->stats_data.stat_responses.
                          stat_outbound_refer_resp.stats_202), &offset);

        *p_length    += offset;

   icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&
      (p_icf_stats_resp_st->stats_data.stat_responses.
                          stat_outbound_refer_resp.stats_400), &offset);

        *p_length    += offset;

   icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&
      (p_icf_stats_resp_st->stats_data.stat_responses.
                         stat_outbound_refer_resp.stats_481), &offset);

        *p_length    += offset;

   icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&
      (p_icf_stats_resp_st->stats_data.stat_responses.
                  stat_outbound_refer_resp.stats_other_resp), &offset);

        *p_length    += offset;




   icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&
      (p_icf_stats_resp_st->stats_data.stat_responses.
                     stat_outbound_subscribe_resp.stats_200), &offset);

        *p_length    += offset;

   icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&
      (p_icf_stats_resp_st->stats_data.stat_responses.
                     stat_outbound_subscribe_resp.stats_401), &offset);

        *p_length    += offset;

   icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&
      (p_icf_stats_resp_st->stats_data.stat_responses.
                      stat_outbound_subscribe_resp.stats_407), &offset);

        *p_length    += offset;

   icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&
      (p_icf_stats_resp_st->stats_data.stat_responses.
                      stat_outbound_subscribe_resp.stats_489), &offset);

        *p_length    += offset;

   icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&
      (p_icf_stats_resp_st->stats_data.stat_responses.
                stat_outbound_subscribe_resp.stats_other_resp), &offset);

        *p_length    += offset;




   icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&
      (p_icf_stats_resp_st->stats_data.stat_responses.
                            stat_outbound_info_resp.stats_200), &offset);

        *p_length    += offset;

   icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&
      (p_icf_stats_resp_st->stats_data.stat_responses.
                      stat_outbound_info_resp.stats_other_resp), &offset);

        *p_length    += offset;




   icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&
       (p_icf_stats_resp_st->stats_data.stat_responses.
                             stat_outbound_bye_resp.stats_200), &offset);

        *p_length    += offset;

   icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&
       (p_icf_stats_resp_st->stats_data.stat_responses.
                      stat_outbound_bye_resp.stats_other_resp), &offset);

        *p_length    += offset;



   icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&
       (p_icf_stats_resp_st->stats_data.stat_responses.
                           stat_outbound_notify_resp.stats_200), &offset);

        *p_length    += offset;

   icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&
       (p_icf_stats_resp_st->stats_data.stat_responses.
                    stat_outbound_notify_resp.stats_other_resp), &offset);

        *p_length    += offset;



   icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&
       (p_icf_stats_resp_st->stats_data.stat_responses.
                           stat_outbound_update_resp.stats_200), &offset);

        *p_length    += offset;

   icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&
       (p_icf_stats_resp_st->stats_data.stat_responses.
                     stat_outbound_update_resp.stats_other_resp), &offset);

        *p_length    += offset;



   icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&
        (p_icf_stats_resp_st->stats_data.stat_responses.
                         stat_outbound_register_resp.stats_200), &offset);

        *p_length    += offset;

   icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&
        (p_icf_stats_resp_st->stats_data.stat_responses.
                          stat_outbound_register_resp.stats_401), &offset);

        *p_length    += offset;

   icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&
        (p_icf_stats_resp_st->stats_data.stat_responses.
                           stat_outbound_register_resp.stats_407), &offset);

        *p_length    += offset;

   icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&
        (p_icf_stats_resp_st->stats_data.stat_responses.
                     stat_outbound_register_resp.stats_other_resp), &offset);

        *p_length    += offset;



   icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&
       (p_icf_stats_resp_st->stats_data.stat_responses.
                            stat_outbound_options_resp.stats_200), &offset);

        *p_length    += offset;

   icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&
       (p_icf_stats_resp_st->stats_data.stat_responses.
                   stat_outbound_options_resp.stats_other_resp), &offset);

        *p_length    += offset;

   icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&
       (p_icf_stats_resp_st->stats_data.stat_responses.
                            stat_outbound_publish_resp.stats_200), &offset);

        *p_length    += offset;

   icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&
       (p_icf_stats_resp_st->stats_data.stat_responses.
                   stat_outbound_publish_resp.stats_other_resp), &offset);

        *p_length    += offset;

    icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&
       (p_icf_stats_resp_st->stats_data.stat_responses.
                            stat_outbound_message_resp.stats_200), &offset);

        *p_length    += offset;

   icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&
       (p_icf_stats_resp_st->stats_data.stat_responses.
                   stat_outbound_message_resp.stats_other_resp), &offset);

        *p_length    += offset;



   icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&
       (p_icf_stats_resp_st->stats_data.stat_responses.
                            stat_inbound_invite_resp.stats_100), &offset);

        *p_length    += offset;

   icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&
       (p_icf_stats_resp_st->stats_data.stat_responses.
                             stat_inbound_invite_resp.stats_2xx), &offset);

        *p_length    += offset;

   icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&
       (p_icf_stats_resp_st->stats_data.stat_responses.
                             stat_inbound_invite_resp.stats_180), &offset);

        *p_length    += offset;

   icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&
       (p_icf_stats_resp_st->stats_data.stat_responses.
                             stat_inbound_invite_resp.stats_181), &offset);

        *p_length    += offset;

icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&
       (p_icf_stats_resp_st->stats_data.stat_responses.
                             stat_inbound_invite_resp.stats_182), &offset);

        *p_length    += offset;


   icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&
      (p_icf_stats_resp_st->stats_data.stat_responses.
                            stat_inbound_invite_resp.stats_183), &offset);

        *p_length    += offset;

   icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&
      (p_icf_stats_resp_st->stats_data.stat_responses.
                             stat_inbound_invite_resp.stats_3xx), &offset);

        *p_length    += offset;

   icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&
      (p_icf_stats_resp_st->stats_data.stat_responses.
                            stat_inbound_invite_resp.stats_400), &offset);

        *p_length    += offset;

   icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&
      (p_icf_stats_resp_st->stats_data.stat_responses.
                            stat_inbound_invite_resp.stats_486), &offset);

        *p_length    += offset;

   icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&
      (p_icf_stats_resp_st->stats_data.stat_responses.
                            stat_inbound_invite_resp.stats_404), &offset);

        *p_length    += offset;

   icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&
      (p_icf_stats_resp_st->stats_data.stat_responses.
                          stat_inbound_invite_resp.stats_415), &offset);

        *p_length    += offset;

   icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&
      (p_icf_stats_resp_st->stats_data.stat_responses.
                          stat_inbound_invite_resp.stats_407), &offset);

        *p_length    += offset;

   icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&
      (p_icf_stats_resp_st->stats_data.stat_responses.
                          stat_inbound_invite_resp.stats_401), &offset);

        *p_length    += offset;

   icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&
      (p_icf_stats_resp_st->stats_data.stat_responses.
                          stat_inbound_invite_resp.stats_491), &offset);

        *p_length    += offset;

   icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&
      (p_icf_stats_resp_st->stats_data.stat_responses.
                          stat_inbound_invite_resp.stats_481), &offset);

        *p_length    += offset;
   
    icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&
      (p_icf_stats_resp_st->stats_data.stat_responses.
                          stat_inbound_invite_resp.stats_402), &offset);

        *p_length    += offset;

 icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&
      (p_icf_stats_resp_st->stats_data.stat_responses.
                          stat_inbound_invite_resp.stats_406), &offset);

        *p_length    += offset;
 icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&
      (p_icf_stats_resp_st->stats_data.stat_responses.
                          stat_inbound_invite_resp.stats_408), &offset);

        *p_length    += offset;
 icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&
      (p_icf_stats_resp_st->stats_data.stat_responses.
                          stat_inbound_invite_resp.stats_409), &offset);

        *p_length    += offset;
 icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&
      (p_icf_stats_resp_st->stats_data.stat_responses.
                          stat_inbound_invite_resp.stats_410), &offset);

        *p_length    += offset;
 icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&
      (p_icf_stats_resp_st->stats_data.stat_responses.
                          stat_inbound_invite_resp.stats_413), &offset);

        *p_length    += offset;
 icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&
      (p_icf_stats_resp_st->stats_data.stat_responses.
                          stat_inbound_invite_resp.stats_482), &offset);

        *p_length    += offset;
 icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&
      (p_icf_stats_resp_st->stats_data.stat_responses.
                          stat_inbound_invite_resp.stats_483), &offset);

        *p_length    += offset;
 icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&
      (p_icf_stats_resp_st->stats_data.stat_responses.
                          stat_inbound_invite_resp.stats_489), &offset);

        *p_length    += offset;

   icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&
      (p_icf_stats_resp_st->stats_data.stat_responses.
                          stat_inbound_invite_resp.stats_503), &offset);

        *p_length    += offset;

   icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&
      (p_icf_stats_resp_st->stats_data.stat_responses.
                          stat_inbound_invite_resp.stats_488), &offset);

        *p_length    += offset;

   icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&
      (p_icf_stats_resp_st->stats_data.stat_responses.
                         stat_inbound_invite_resp.stats_480), &offset);

        *p_length    += offset;

   icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&
      (p_icf_stats_resp_st->stats_data.stat_responses.
                         stat_inbound_invite_resp.stats_500), &offset);

       *p_length    += offset;

   icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&
      (p_icf_stats_resp_st->stats_data.stat_responses.
                        stat_inbound_invite_resp.stats_403), &offset);

        *p_length    += offset;

   icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&
      (p_icf_stats_resp_st->stats_data.stat_responses.
                        stat_inbound_invite_resp.stats_487), &offset);

        *p_length    += offset;

   icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&
      (p_icf_stats_resp_st->stats_data.stat_responses.
                        stat_inbound_invite_resp.stats_420), &offset);

        *p_length    += offset;

   icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&
      (p_icf_stats_resp_st->stats_data.stat_responses.
                        stat_inbound_invite_resp.stats_423), &offset);

        *p_length    += offset;

   icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&
      (p_icf_stats_resp_st->stats_data.stat_responses.
                        stat_inbound_invite_resp.stats_484), &offset);

        *p_length    += offset;

   icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&
      (p_icf_stats_resp_st->stats_data.stat_responses.
                        stat_inbound_invite_resp.stats_485), &offset);

        *p_length    += offset;

   icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&
      (p_icf_stats_resp_st->stats_data.stat_responses.
                         stat_inbound_invite_resp.stats_501), &offset);

        *p_length    += offset;

   icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&
      (p_icf_stats_resp_st->stats_data.stat_responses.
                         stat_inbound_invite_resp.stats_502), &offset);

        *p_length    += offset;

   icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&
      (p_icf_stats_resp_st->stats_data.stat_responses.
                          stat_inbound_invite_resp.stats_504), &offset);

        *p_length    += offset;

   icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&
        (p_icf_stats_resp_st->stats_data.stat_responses.
                         stat_inbound_invite_resp.stats_505), &offset);

        *p_length    += offset;

   icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&
        (p_icf_stats_resp_st->stats_data.stat_responses.
                          stat_inbound_invite_resp.stats_580), &offset);

        *p_length    += offset;

   icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&
       (p_icf_stats_resp_st->stats_data.stat_responses.
                          stat_inbound_invite_resp.stats_600), &offset);

        *p_length    += offset;

   icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&
       (p_icf_stats_resp_st->stats_data.stat_responses.
                          stat_inbound_invite_resp.stats_603), &offset);

        *p_length    += offset;
icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&
       (p_icf_stats_resp_st->stats_data.stat_responses.
                          stat_inbound_invite_resp.stats_604), &offset);

        *p_length    += offset;
icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&
       (p_icf_stats_resp_st->stats_data.stat_responses.
                          stat_inbound_invite_resp.stats_606), &offset);

        *p_length    += offset;

   icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&
       (p_icf_stats_resp_st->stats_data.stat_responses.
                    stat_inbound_invite_resp.stats_rest_4xx), &offset);

        *p_length    += offset;

   icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&
        (p_icf_stats_resp_st->stats_data.stat_responses.
                    stat_inbound_invite_resp.stats_rest_5xx), &offset);

        *p_length    += offset;

   icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&
         (p_icf_stats_resp_st->stats_data.stat_responses.
                   stat_inbound_invite_resp.stats_rest_6xx), &offset);

        *p_length    += offset;




    icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&
         (p_icf_stats_resp_st->stats_data.stat_responses.
                         stat_inbound_cancel_resp.stats_200), &offset);

        *p_length    += offset;

    icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&
          (p_icf_stats_resp_st->stats_data.stat_responses.
                        stat_inbound_cancel_resp.stats_481), &offset);

        *p_length    += offset;

    icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&
          (p_icf_stats_resp_st->stats_data.stat_responses.
                        stat_inbound_cancel_resp.stats_400), &offset);

        *p_length    += offset;




    icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&
           (p_icf_stats_resp_st->stats_data.stat_responses.
                          stat_inbound_prack_resp.stats_200), &offset);

        *p_length    += offset;

    icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&
          (p_icf_stats_resp_st->stats_data.stat_responses.
                           stat_inbound_prack_resp.stats_500), &offset);

        *p_length    += offset;

    icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&
          (p_icf_stats_resp_st->stats_data.stat_responses.
                            stat_inbound_prack_resp.stats_481), &offset);

        *p_length    += offset;

    icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&
          (p_icf_stats_resp_st->stats_data.stat_responses.
                             stat_inbound_prack_resp.stats_400), &offset);

        *p_length    += offset;



    icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&
         (p_icf_stats_resp_st->stats_data.stat_responses.
                            stat_inbound_refer_resp.stats_202), &offset);

        *p_length    += offset;

    icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&
         (p_icf_stats_resp_st->stats_data.stat_responses.
                            stat_inbound_refer_resp.stats_481), &offset);

        *p_length    += offset;

    icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&
         (p_icf_stats_resp_st->stats_data.stat_responses.
                            stat_inbound_refer_resp.stats_400), &offset);

        *p_length    += offset;

    icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&
         (p_icf_stats_resp_st->stats_data.stat_responses.
                             stat_inbound_refer_resp.stats_403), &offset);

        *p_length    += offset;

    icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&
         (p_icf_stats_resp_st->stats_data.stat_responses.
                    stat_inbound_refer_resp.stats_failure_resp), &offset);

        *p_length    += offset;



    icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&
         (p_icf_stats_resp_st->stats_data.stat_responses.
                          stat_inbound_notify_resp.stats_200), &offset);

        *p_length    += offset;

    /* Added for asynchronous NOTIFY support */
    icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&
        (p_icf_stats_resp_st->stats_data.stat_responses.
                           stat_inbound_notify_resp.stats_500), &offset);

    *p_length    += offset;
    

    icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&
        (p_icf_stats_resp_st->stats_data.stat_responses.
                           stat_inbound_notify_resp.stats_481), &offset);

        *p_length    += offset;

    icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&
        (p_icf_stats_resp_st->stats_data.stat_responses. 
                           stat_inbound_notify_resp.stats_400), &offset);

        *p_length    += offset;




    icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&
        (p_icf_stats_resp_st->stats_data.stat_responses.
                             stat_inbound_info_resp.stats_200), &offset);

        *p_length    += offset;

    icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&
        (p_icf_stats_resp_st->stats_data.stat_responses.
                             stat_inbound_info_resp.stats_481), &offset);

        *p_length    += offset;

    icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&
        (p_icf_stats_resp_st->stats_data.stat_responses.
                              stat_inbound_info_resp.stats_400), &offset);

        *p_length    += offset;




    icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&
         (p_icf_stats_resp_st->stats_data.stat_responses.
                           stat_inbound_bye_resp.stats_200), &offset);

        *p_length    += offset;

    icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&
         (p_icf_stats_resp_st->stats_data.stat_responses.
                          stat_inbound_bye_resp.stats_481), &offset);

        *p_length    += offset;

    icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&
         (p_icf_stats_resp_st->stats_data.stat_responses.
                          stat_inbound_bye_resp.stats_400), &offset);

        *p_length    += offset;



    icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&
         (p_icf_stats_resp_st->stats_data.stat_responses.
                       stat_inbound_update_resp.stats_200), &offset);

        *p_length    += offset;

    icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&
         (p_icf_stats_resp_st->stats_data.stat_responses. 
                        stat_inbound_update_resp.stats_500), &offset);

        *p_length    += offset;

    icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&
         (p_icf_stats_resp_st->stats_data.stat_responses.
                         stat_inbound_update_resp.stats_481), &offset);

        *p_length    += offset;

    icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&
        (p_icf_stats_resp_st->stats_data.stat_responses.
                        stat_inbound_update_resp.stats_400), &offset);

        *p_length    += offset;

        icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&
         (p_icf_stats_resp_st->stats_data.stat_responses.
                       stat_inbound_message_resp.stats_200), &offset);

        *p_length    += offset;

    icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&
         (p_icf_stats_resp_st->stats_data.stat_responses. 
                        stat_inbound_message_resp.stats_500), &offset);

        *p_length    += offset;

    icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&
         (p_icf_stats_resp_st->stats_data.stat_responses.
                         stat_inbound_message_resp.stats_481), &offset);

        *p_length    += offset;

    icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&
        (p_icf_stats_resp_st->stats_data.stat_responses.
                        stat_inbound_message_resp.stats_400), &offset);

        *p_length    += offset;

    icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&
       (p_icf_stats_resp_st->stats_data.stat_responses.
                       stat_inbound_options_resp.stats_200), &offset);

        *p_length    += offset;

    icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&
       (p_icf_stats_resp_st->stats_data.stat_responses.
                        stat_inbound_options_resp.stats_481), &offset);

        *p_length    += offset;

    icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&
      (p_icf_stats_resp_st->stats_data.stat_responses.
                 stat_inbound_options_resp.stats_other_resp), &offset);

        *p_length    += offset;




    icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&
       (p_icf_stats_resp_st->stats_data.misc_stats.
                                       stats_AAA_DNS_QUERY), &offset);

        *p_length    += offset;

    icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&
       (p_icf_stats_resp_st->stats_data.misc_stats.
                                       stats_SRV_DNS_QUERY), &offset);

        *p_length    += offset;


    icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&
         (p_icf_stats_resp_st->stats_data.misc_stats.
                         stats_AAA_DNS_QUERY_SUCCESS_RESP), &offset);

        *p_length    += offset;

    icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&
          (p_icf_stats_resp_st->stats_data.misc_stats.
                               stats_SRV_DNS_SUCCESS_RESP), &offset);

        *p_length    += offset;

    icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&
         (p_icf_stats_resp_st->stats_data.misc_stats.
                         stats_AAA_DNS_QUERY_FAILURE_RESP), &offset);

        *p_length    += offset;

    icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&
         (p_icf_stats_resp_st->stats_data.misc_stats.
                               stats_SRV_DNS_FAILURE_RESP), &offset);

        *p_length    += offset;
    
    icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&
         (p_icf_stats_resp_st->stats_data.misc_stats.
                               stats_DHCP_QUERY), &offset);

        *p_length    += offset;

    icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&
         (p_icf_stats_resp_st->stats_data.misc_stats.
                               stats_DHCP_SUCCESS_QUERY), &offset);

        *p_length    += offset;

    icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&
         (p_icf_stats_resp_st->stats_data.misc_stats.
                               stats_DHCP_FAILURE_QUERY), &offset);

        *p_length    += offset;
  
    /*  Code for NAPTR Stats  */
    
    icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&
         (p_icf_stats_resp_st->stats_data.misc_stats.
                               stats_NAPTR_DNS_QUERY), &offset);

        *p_length    += offset;

    icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&
         (p_icf_stats_resp_st->stats_data.misc_stats.
                               stats_NAPTR_DNS_SUCCESS_QUERY), &offset);

        *p_length    += offset;

    icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&
         (p_icf_stats_resp_st->stats_data.misc_stats.
                               stats_NAPTR_DNS_FAILURE_QUERY), &offset);

        *p_length    += offset;




    icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&
         (p_icf_stats_resp_st->stats_data.misc_stats.
                                     stats_active_timers), &offset);

        *p_length    += offset;

    icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&
         (p_icf_stats_resp_st->stats_data.misc_stats.
                                     inbound_req_405), &offset);

        *p_length    += offset;


    icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&
         (p_icf_stats_resp_st->stats_data.misc_stats.
                                     outbound_req_405), &offset);
        *p_length    += offset;
   
    icf_pack_icf_time_stamp_st(p_dest + (*p_length), (icf_uint8_t*)&
         (p_icf_stats_resp_st->last_reset_time), &offset);

        *p_length    += offset;

    /*PACK srtp_fallback_done*/
    icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&
         (p_icf_stats_resp_st->stats_data.misc_stats.stats_srtp_fallback_done), 
         &offset);

    *p_length    += offset;

    return ICF_SUCCESS;
}

#endif

/*****************************************************************************
*
* FUNCTION: icf_pack_icf_get_register_status_req_st
*
* DESCRIPTION: This function is used to pack the payload of API
*              ICF_GET_REGISTER_STATUS_REQ.
*
*****************************************************************************/

icf_uint32_t
icf_pack_icf_get_register_status_req_st(icf_uint8_t    *p_dest,
                                        icf_uint8_t    *p_src,
                                        icf_uint32_t    *p_length)
{

        icf_uint32_t       offset = 0;

        icf_get_register_status_req_st  *p_icf_get_register_status_req_st = 
                               (icf_get_register_status_req_st *)p_src;

        /* This function packs icf_get_register_status_req_st */
        ICF_CLIB_TRACE(("Packing icf_get_register_status_req_st\n"));

        /* Initialise length to 0 */
        *p_length    = 0;

        icf_pack_icf_line_id_t(p_dest + (*p_length), 
         (icf_uint8_t*)&(p_icf_get_register_status_req_st->line_id), &offset);

        *p_length    += offset;

        return ICF_SUCCESS;
}

/*****************************************************************************
*
* FUNCTION: icf_pack_icf_get_register_status_resp_st
*
* DESCRIPTION: This function is used to pack the payload of API
*              ICF_GET_REGISTER_STATUS_RESP.
*
*****************************************************************************/


icf_uint32_t
icf_pack_icf_get_register_status_resp_st(icf_uint8_t    *p_dest,
                                         icf_uint8_t    *p_src,
                                         icf_uint32_t    *p_length)
{

        icf_uint32_t       offset = 0;

        icf_get_register_status_resp_st  *p_icf_get_register_status_resp_st =
                                     (icf_get_register_status_resp_st *)p_src;

        /* This function packs icf_get_register_status_resp_st */
        ICF_CLIB_TRACE(("Packing icf_get_register_status_resp_st\n"));

        /* Initialise length to 0 */
        *p_length    = 0;

        icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&
               (p_icf_get_register_status_resp_st->result), &offset);

         *p_length    += offset;


        if(p_icf_get_register_status_resp_st->result == ICF_FAILURE)
        {
           icf_pack_U16(p_dest + (*p_length), (icf_uint8_t*)&
               (p_icf_get_register_status_resp_st->error_cause), &offset);

           *p_length    += offset;
        }
     
        else
        {

        icf_pack_icf_user_reg_status_list_st(p_dest + (*p_length), 
        (p_icf_get_register_status_resp_st->p_user_reg_status), &offset);

        *p_length    += offset;

        icf_pack_icf_transport_address_st(p_dest + (*p_length), 
          (icf_uint8_t*)&(p_icf_get_register_status_resp_st->
                                 registrar_address), &offset);

        *p_length    += offset;

        icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&
           (p_icf_get_register_status_resp_st->expires_duration), &offset);

	 *p_length    += offset;

       }

        return ICF_SUCCESS;
}




icf_uint32_t
icf_pack_icf_time_stamp_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t    *p_length
)
{
    icf_uint32_t        offset = 0;

    icf_time_stamp_st    *p_time_stamp = ICF_NULL;

    p_time_stamp    = (icf_time_stamp_st *)p_src;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function packs icf_timestamp_st */
    ICF_CLIB_TRACE(("Packing icf_time_stamp_st\n"));

    icf_pack_U32(p_dest + (*p_length), 
	(icf_uint8_t*)&(p_time_stamp->time_in_secs), &offset);
    *p_length    += offset;

    icf_pack_U32(p_dest + (*p_length), 
	(icf_uint8_t*)&(p_time_stamp->time_in_millisecs), &offset);
    *p_length    += offset;

     return ICF_SUCCESS;
}




#ifdef ICF_TRACE_ENABLE
icf_uint32_t
icf_pack_icf_trace_data_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t    *p_length
)
{
    icf_uint32_t        offset = 0;
    icf_uint32_t        count = 0;

    icf_trace_data_st    *p_icf_trace_data = ICF_NULL;

    p_icf_trace_data    = (icf_trace_data_st *)p_src;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function packs icf_trace_data_st */
    ICF_CLIB_TRACE(("Packing icf_trace_data_st\n"));

    icf_pack_icf_time_stamp_st(p_dest + (*p_length), 
	(icf_uint8_t*)&(p_icf_trace_data->trace_timestamp), &offset);
    *p_length    += offset;


    icf_pack_U8(p_dest + (*p_length), 
	(icf_uint8_t*)&(p_icf_trace_data->num_of_integer_values), &offset);
    *p_length    += offset;

    for(count = 0;(count <p_icf_trace_data->num_of_integer_values) && (count < ICF_MAX_INT_TRACE_DATA);
                count++)
    {
         icf_pack_U32(p_dest + (*p_length), 
		(icf_uint8_t*)&(p_icf_trace_data->int_val[count]), &offset);

         *p_length    += offset;

    }

    icf_pack_U8(p_dest + (*p_length), 
	(icf_uint8_t*)&(p_icf_trace_data->number_of_string_values), &offset);
    *p_length    += offset;

    for(count = 0;(count <p_icf_trace_data->number_of_string_values) && (count < ICF_MAX_STR_TRACE_DATA);
                count++)
    {
         icf_pack_icf_string_st(p_dest + (*p_length), 
		(icf_uint8_t*)&(p_icf_trace_data->string_val[count]), &offset);

         *p_length    += offset;

    }

    icf_pack_icf_string_st(p_dest + (*p_length), 
	(icf_uint8_t*)&(p_icf_trace_data->trc_string), &offset);

    *p_length    += offset;

     return ICF_SUCCESS;
}





icf_uint32_t 
icf_pack_icf_report_trace_ind_st
( 
	icf_uint8_t *p_dest,
    icf_uint8_t *p_src,
    icf_uint32_t *p_length )
{
    icf_uint32_t           offset = 0, count = 0;
    icf_report_trace_ind_st  *p_report_trace_ind = ICF_NULL;

    p_report_trace_ind = (icf_report_trace_ind_st *)p_src;

    /* Initialise length to 0 */
    *p_length    = 0;
    /* This function packs icf_report_trace_ind_st */
    ICF_CLIB_TRACE(("Packing icf_report_trace_ind_st \n"));

    icf_pack_U32(p_dest + (*p_length), 
		(icf_uint8_t*)&(p_report_trace_ind->trace_id), &offset);
    *p_length    += offset;

    icf_pack_U32(p_dest + (*p_length), 
		(icf_uint8_t*)&(p_report_trace_ind->trace_type), &offset);
    *p_length    += offset;

    icf_pack_icf_trace_data_st(p_dest + (*p_length), 
		(icf_uint8_t*)&(p_report_trace_ind->trace_data), &offset);
    *p_length    += offset;

    if(ICF_NULL != p_report_trace_ind->trace_data.p_var_data)
    {
        switch(p_report_trace_ind->trace_id)
        {
        case ICF_TRACE_MEDIA_CAPABILITY:
        case ICF_TRACE_CODEC_RESERVE:
        case ICF_TRACE_STREAM_CREATE:
        case ICF_TRACE_STREAM_MODIFY:
        case ICF_TRACE_CODEC_RELEASE:
            {
		icf_pack_icf_codec_attrib_list_st(p_dest + (*p_length), 
                  (icf_list_st *)(p_report_trace_ind->trace_data.p_var_data), 
                  &offset, p_report_trace_ind->trace_data.int_val[0]);
                break;
            }

        
       case ICF_TRACE_CODEC_COMMIT:
            {
              icf_pack_icf_codec_attrib_st(p_dest + (*p_length), 
                    (icf_uint8_t*)(p_report_trace_ind->trace_data.p_var_data), 
                    &offset, (icf_uint8_t)p_report_trace_ind->trace_data.int_val[0]);
                 
            }
            break;

       case ICF_TRACE_LOCAL_SDP_OFFER:
       case ICF_TRACE_REMOTE_SDP_OFFER:
       case ICF_TRACE_LOCAL_SDP_ANSWER:
       case ICF_TRACE_REMOTE_SDP_ANSWER:
       {
                icf_list_st *p_sdp_list =
                   (icf_list_st *)p_report_trace_ind->trace_data.p_var_data;

                icf_pack_icf_call_modify_media_data_list_st(
                      p_dest + (*p_length), 
                      p_sdp_list, 
                      &offset);
                break;
       }
       case ICF_TRACE_SIP_PDU:      
       case ICF_TRACE_APP_MESG:
       case ICF_TRACE_MEDIA_MESG:
       {
           icf_generic_string_st *p_msg =
               (icf_generic_string_st *)p_report_trace_ind->trace_data.p_var_data;

           icf_pack_U32(p_dest + (*p_length), 
                   (icf_uint8_t*)&(p_msg->len), &offset);

           *p_length += offset;

           /* SPR 19748 : In case the PDU length is very long then no need to 
            * report the message PDU as maximum alllowed payload length for CLIB 
            * is ICF_CLIB_MAX_TEMP_ARBI_BUFF_LENGTH. Else it will result into 
            * memory corruption.
            */ 
           if(ICF_CLIB_MAX_TEMP_ARBI_BUFF_LENGTH <= (*p_length + p_msg->len))
           {
               return ICF_FAILURE;
           }

           for(count = 0;count < p_msg->len; count++)
           {
               icf_pack_U8(p_dest + (*p_length),
                       (icf_uint8_t *)&(p_msg->p_buff[count]),
                       &offset);
               *p_length       += offset;

           }   
           break;
       }
        case ICF_TRACE_CALL_CONNECTED:
                icf_pack_icf_call_connect_data_st(
                      p_dest + (*p_length), 
                      (icf_uint8_t *)(p_report_trace_ind->trace_data.p_var_data), 
                      &offset);
              break;
        }
    }
    *p_length    += offset;
    return ICF_SUCCESS;
}


icf_uint32_t
icf_pack_icf_stream_data_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t    *p_length
)
{
	icf_uint32_t        offset = 0;

	icf_stream_data_st  *p_icf_stream_data_st = (icf_stream_data_st *)p_src;

	/* This function packs icf_stream_data_st */
        ICF_CLIB_TRACE(("Packing icf_stream_data_st\n"));

	/* Initialise length to 0 */
        *p_length    = 0;

	icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_stream_data_st->stream_id), &offset);

	*p_length += offset;

	icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_stream_data_st->stream_type), &offset);

	*p_length += offset;

        if(ICF_STREAM_TYPE_TBCP != p_icf_stream_data_st->stream_type)
        {
          
          if(ICF_NULL != p_icf_stream_data_st->p_offered_codec)
          {
	         icf_pack_icf_codec_attrib_list_st(p_dest + (*p_length), 
                    (p_icf_stream_data_st->p_offered_codec), 
                    &offset, p_icf_stream_data_st->stream_type);

	         *p_length += offset;
           }

           if(ICF_NULL !=  p_icf_stream_data_st->p_negotiated_codec)
           {   
              icf_pack_icf_codec_attrib_st(p_dest + (*p_length), 
                    (icf_uint8_t*)(p_icf_stream_data_st->p_negotiated_codec), 
                    &offset, p_icf_stream_data_st->stream_type);
  
	         *p_length += offset;
           }
        }

        icf_pack_icf_transport_address_st(p_dest + (*p_length), 
                     (icf_uint8_t*)&(p_icf_stream_data_st->local_rtp_address), &offset);
        
        *p_length    += offset;

         icf_pack_icf_transport_address_st(p_dest + (*p_length), 
                     (icf_uint8_t*)&(p_icf_stream_data_st->remote_rtp_address), &offset);

        *p_length    += offset;

	return ICF_SUCCESS;
}

icf_uint32_t
icf_pack_icf_stream_data_list_st
(
        icf_uint8_t    *p_dest,
        icf_list_st    *p_icf_stream_data_list_st,
        icf_uint32_t   *p_length
)
{
    icf_uint32_t        offset = 0;
        icf_uint8_t        count = 0;

    icf_list_st       *p_list_ptr = ICF_NULL;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function packs list of icf_codec_attrib_st*/
    ICF_CLIB_TRACE(("Packing list of icf_codec_attrib_st\n"));
   
    /*Pack count = 0 initially*/
    icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&count, &offset);

    *p_length    += offset;

    for(p_list_ptr = p_icf_stream_data_list_st;ICF_NULL != p_list_ptr;p_list_ptr = p_list_ptr->p_next)
    {

          icf_pack_icf_stream_data_st(p_dest + (*p_length), (p_list_ptr->p_data), &offset);

                *p_length += offset;

                count++;

        }

        if(count)
        {
                /* Updating the count */
                icf_pack_U8(p_dest, (icf_uint8_t*)&count, &offset);
        }

        return ICF_SUCCESS;
}

icf_uint32_t
icf_pack_icf_call_connect_data_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t    *p_length
)
{
        icf_uint32_t        offset = 0;

        icf_call_connect_data_st  *p_call_connect_data_st = (icf_call_connect_data_st *)p_src;

        /* This function packs icf_call_connect_data_st */
        ICF_CLIB_TRACE(("Packing icf_call_connect_data_st\n"));

        /* Initialise length to 0 */
        *p_length    = 0;
    
        icf_pack_icf_address_st(p_dest + (*p_length), 
               (icf_uint8_t*)&(p_call_connect_data_st->calling_party_address), &offset);
    
        *p_length    += offset;

        icf_pack_icf_address_st(p_dest + (*p_length), 
               (icf_uint8_t*)&(p_call_connect_data_st->called_party_address), &offset);
    
        *p_length    += offset;
    
        icf_pack_icf_line_id_t(p_dest + (*p_length), 
             (icf_uint8_t*)&(p_call_connect_data_st->line_id), &offset);
    
        *p_length    += offset;

        icf_pack_U8(p_dest + (*p_length), 
                 (icf_uint8_t*)&(p_call_connect_data_st->call_direction), &offset);

        *p_length += offset;
 
        icf_pack_icf_stream_data_list_st(p_dest + (*p_length),
               (p_call_connect_data_st->p_stream_data), &offset);
        
        *p_length += offset;
         
        icf_pack_icf_transport_address_st(p_dest + (*p_length),
                     (icf_uint8_t*)&(p_call_connect_data_st->local_sip_ip_address), &offset);

        *p_length    += offset;
        
         icf_pack_U8(p_dest + (*p_length), 
               (icf_uint8_t*)&(p_call_connect_data_st->call_transport_mode), &offset);

        *p_length += offset;
         
        icf_pack_U8(p_dest + (*p_length), 
               (icf_uint8_t*)&(p_call_connect_data_st->qos_call), &offset);

        *p_length += offset;


        return ICF_SUCCESS;
}


icf_uint32_t 
icf_pack_icf_set_trace_req_st
( 
	icf_uint8_t *p_dest,
    icf_uint8_t *p_src,
    icf_uint32_t *p_length )
{
   icf_uint32_t           offset = 0;
   icf_set_trace_req_st  *p_set_trace_req = ICF_NULL;

   p_set_trace_req = (icf_set_trace_req_st *)p_src;

   /* Initialise length to 0 */
   *p_length    = 0;
   /* This function packs icf_set_trace_req_st */
   ICF_CLIB_TRACE(("Packing icf_set_trace_req_st \n"));
       
	icf_pack_U8(p_dest + (*p_length), 
		(icf_uint8_t*)&(p_set_trace_req->bitmask),  &offset);
    *p_length       += offset;

	icf_pack_U8(p_dest + (*p_length), 
		(icf_uint8_t*)&(p_set_trace_req->status),  &offset);
    *p_length       += offset;

   if(p_set_trace_req->bitmask & ICF_TRACE_TYPE_PRESENT)
   {
        icf_pack_U32(p_dest + (*p_length), 
		(icf_uint8_t*)&(p_set_trace_req->trace_type),  &offset);

        *p_length       += offset;
   }

   if(p_set_trace_req->bitmask & ICF_TRACE_CRITERIA_PRESENT)
   {
        icf_pack_U32(p_dest + (*p_length),
                (icf_uint8_t*)&(p_set_trace_req->trace_criteria),  &offset);

        *p_length       += offset;
   }

   if(p_set_trace_req->bitmask & ICF_TRACE_RULE_PRESENT)
   {
        icf_pack_U8(p_dest + (*p_length), 
              (icf_uint8_t*)&(p_set_trace_req->trace_rule),  &offset);

       *p_length       += offset;
   }

   if(p_set_trace_req->bitmask & ICF_TRACE_DEBUG_TRACE_STATUS_PRESENT)
   {
        icf_pack_U8(p_dest + (*p_length), 
              (icf_uint8_t*)&(p_set_trace_req->debug_trace_status),  &offset);

       *p_length       += offset;
   }

   return ICF_SUCCESS;
}

#endif

icf_uint32_t
icf_pack_icf_progress_ind_st
(
    icf_uint8_t    	*p_dest,
    icf_uint8_t    	*p_src,
    icf_uint32_t    *p_length
)
{
    icf_uint32_t        			offset = 0;

    icf_progress_ind_st    			*p_icf_progress_ind_st = ICF_NULL;

    p_icf_progress_ind_st = (icf_progress_ind_st *)p_src;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function packs icf_progress_ind_st */
    ICF_CLIB_TRACE(("Packing icf_progress_ind_st\n"));

    icf_pack_U16(
		p_dest + (*p_length), 
		(icf_uint8_t *)&(p_icf_progress_ind_st->response_code),
		&offset);

    *p_length    += offset;

    return ICF_SUCCESS;
}

/*****************************************************************************
*
* FUNCTION: icf_pack_icf_aka_auth_resp_st
*
* DESCRIPTION: This function is used to pack the payload of API
*              ICF_AKA_AUTH_RESP.
*
*****************************************************************************/

icf_uint32_t
icf_pack_icf_aka_auth_resp_st( icf_uint8_t    *p_dest,
    						icf_uint8_t    *p_src,
    						icf_uint32_t    *p_length)

{
    icf_uint32_t       offset = 0;

    icf_aka_auth_resp_st  *p_icf_aka_auth_resp_st = (icf_aka_auth_resp_st *)p_src;

    /* This function is for packing icf_aka_auth_resp_st */
    ICF_CLIB_TRACE (("Packing icf_aka_auth_resp_st \n"));

    /* Initialise length to 0 */
    *p_length = 0;

    icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&( p_icf_aka_auth_resp_st->bit_mask), &offset);
    
    *p_length    += offset;

    /*pack sip_method_name*/
    icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&( p_icf_aka_auth_resp_st-> sip_method_name), &offset);
    *p_length    += offset;

    /*pack line_id*/
    icf_pack_icf_line_id_t(p_dest + (*p_length), (icf_uint8_t*)& (p_icf_aka_auth_resp_st ->line_id), &offset);
    *p_length    += offset;

    /*pack user_address*/
    icf_pack_icf_address_st(p_dest + (*p_length), (icf_uint8_t*) & (p_icf_aka_auth_resp_st ->user_address), &offset);
    *p_length    += offset;

    /*pack result*/
    icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&( p_icf_aka_auth_resp_st -> result), &offset);
    *p_length    += offset;

    /*pack error-cause if present*/
    if(p_icf_aka_auth_resp_st ->bit_mask & ICF_AKA_AUTH_RESP_ERROR_PRESENT)
    {
        icf_pack_U16(p_dest + (*p_length), (icf_uint8_t*)&( p_icf_aka_auth_resp_st ->error_cause), &offset);
        *p_length    += offset;
    }

    /*pack auth response if present*/
    if(p_icf_aka_auth_resp_st ->bit_mask & ICF_AKA_AUTH_RESPONSE_PRESENT)
    {
        icf_pack_icf_string_st (p_dest + (*p_length), (icf_uint8_t*)&( p_icf_aka_auth_resp_st ->auth_response), &offset);
        *p_length    += offset;
    }

    /*pack auts_token if present*/
    if(p_icf_aka_auth_resp_st ->bit_mask & ICF_AKA_AUTH_AUTS_PRESENT)
    {
        icf_pack_icf_string_st (p_dest + (*p_length), (icf_uint8_t*)&( p_icf_aka_auth_resp_st ->auts_token), &offset);
        *p_length    += offset;
    }

    /*pack Ik*/
    if(p_icf_aka_auth_resp_st ->bit_mask & ICF_AKA_AUTH_IK_PRESENT)
    {
        icf_pack_icf_string_st (p_dest + (*p_length), (icf_uint8_t*)&( p_icf_aka_auth_resp_st ->Ik), &offset);
        *p_length    += offset;
    }

    /*pack Ck*/
    if(p_icf_aka_auth_resp_st ->bit_mask & ICF_AKA_AUTH_CK_PRESENT)
    {
        icf_pack_icf_string_st (p_dest + (*p_length), (icf_uint8_t*)&( p_icf_aka_auth_resp_st ->Ck), &offset);
        *p_length    += offset;
    }

    /*pack sequence_num*/
    if(p_icf_aka_auth_resp_st ->bit_mask & ICF_AKA_AUTH_SEQ_NUM_PRESENT)
    {
        icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&( p_icf_aka_auth_resp_st -> sequence_num), &offset);
        *p_length    += offset;
    }

    /*pack security_association_enable*/
    if(p_icf_aka_auth_resp_st ->bit_mask &
                     ICF_AKA_AUTH_SECURITY_ASSOCIATION_REQD_PRESENT)
    {
        icf_pack_U8(p_dest + (*p_length), 
          (icf_uint8_t*)&(p_icf_aka_auth_resp_st->security_association_reqd),
                                      &offset);

        *p_length    += offset;
    }

    return ICF_SUCCESS;
}


/*  SPR 18064 Merge */
icf_uint32_t 
icf_pack_icf_remote_user_terminate_call_resp_st	
(    
   icf_uint8_t    *p_dest,   	
   icf_uint8_t    *p_src,   	
   icf_uint32_t    *p_length
)
{	
    icf_remote_user_term_call_resp_st     *p_call_term_rsp = ICF_NULL;
    icf_uint32_t                                   offset = 0;        
    p_call_term_rsp    = (icf_remote_user_term_call_resp_st*)p_src;  

    /* Initialise length to 0 */	
    *p_length    = 0;    
    /* This function packs icf_remote_user_term_call_resp_st */   	 
    ICF_CLIB_TRACE(("Packing icf_remote_user_term_call_resp_st\n"));  

    icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&(p_call_term_rsp->bitmask), &offset);  
    *p_length    += offset;       

    if(ICF_ERROR_CAUSE_PRESENT & p_call_term_rsp->bitmask)  
    {      	  
	icf_pack_U16(p_dest + (*p_length),(icf_uint8_t*)&(p_call_term_rsp->error_cause), &offset);      
	*p_length    += offset;      
    } 

    return ICF_SUCCESS;
}

#ifdef ICF_UT_TEST

icf_uint32_t
icf_pack_icf_ut_simulate_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t    *p_length
)
{
    icf_uint32_t        offset = 0;
    icf_uint8_t		func_num = 0;

    icf_ut_simulate_st    *p_icf_ut_simulate_st = ICF_NULL;

    p_icf_ut_simulate_st    = (icf_ut_simulate_st *)p_src;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function packs icf_ut_simulate_st */
    ICF_CLIB_TRACE(("Packing icf_ut_simulate_st\n"));

    icf_pack_U16(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_ut_simulate_st->func_num), &offset);

    *p_length    += offset;

    while(func_num<p_icf_ut_simulate_st->func_num)
    {
         icf_pack_icf_func_fail_st(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_ut_simulate_st->func_fail[func_num]), &offset);

         *p_length    += offset;

         func_num++;
    }

    return ICF_SUCCESS;
}



icf_uint32_t
icf_pack_icf_func_fail_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t    *p_length
)
{
    icf_uint32_t        offset = 0;

    icf_func_fail_st    *p_icf_func_fail_st = ICF_NULL;

    p_icf_func_fail_st    = (icf_func_fail_st *)p_src;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function packs icf_func_fail_st */
    ICF_CLIB_TRACE(("Packing icf_func_fail_st\n"));

    icf_pack_icf_string_st(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_func_fail_st->func_name), &offset);

    *p_length    += offset;
   

    icf_pack_U16(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_func_fail_st->count), &offset);

    *p_length    += offset;

    icf_pack_U16(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_func_fail_st->ecode), &offset);

    *p_length    += offset;
    
    icf_pack_icf_string_st(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_func_fail_st->func_level_name), &offset);

    *p_length    += offset;

    return ICF_SUCCESS;
}

icf_uint32_t icf_pack_icf_uatk_api_test_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t    *p_length
)
{
    icf_uint32_t        offset = 0;

    icf_uatk_api_test_st    *p_uatk_api_test = ICF_NULL;

    p_uatk_api_test    = (icf_uatk_api_test_st *)p_src;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function packs icf_uatk_api_test_st */
    ICF_CLIB_TRACE(("\nicf_uatk_api_test_st\n"));

    icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&(p_uatk_api_test->dVar1), &offset);

    *p_length    += offset;

    icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&(p_uatk_api_test->dVar2), &offset);

    *p_length    += offset;
    
    icf_pack_icf_string_st(p_dest + (*p_length), (icf_uint8_t*)&(p_uatk_api_test->string1),
            &offset);

    *p_length    += offset;

    return ICF_SUCCESS;
}


#endif

/******************************************************************************
 * Function name:  icf_pack_icf_display_ind_st
 * Descrtiption:   This function packs the structure 
 *                 icf_display_ind_st
*******************************************************************************/
icf_uint32_t
icf_pack_icf_display_ind_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t    *p_length
)
{
    icf_uint32_t        offset = 0;
    icf_uint32_t        count = 0;

    icf_display_ind_st    *p_icf_display_ind = ICF_NULL;

    p_icf_display_ind    = (icf_display_ind_st *)p_src;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function packs icf_trace_data_st */
    ICF_CLIB_TRACE(("Packing icf_display_ind_st\n"));

    icf_pack_U16(p_dest + (*p_length), 
		(icf_uint8_t*)&(p_icf_display_ind->display_id), &offset);
    *p_length    += offset;

    icf_pack_U8(p_dest + (*p_length), 
		(icf_uint8_t*)&(p_icf_display_ind->bit_mask), &offset);
    *p_length    += offset;

    if (ICF_DISPLAY_IND_DISPLAY_DATA & p_icf_display_ind->bit_mask)
    {
        icf_pack_U8(p_dest + (*p_length), 
            (icf_uint8_t*)&(p_icf_display_ind->display_data.num_of_integer_values), &offset);
        *p_length    += offset;

        for(count = 0; (count < p_icf_display_ind->display_data.num_of_integer_values) && (count < ICF_MAX_INT_DISPLAY_DATA);
                count++)
        {
             icf_pack_U32(p_dest + (*p_length), 
                (icf_uint8_t*)&(p_icf_display_ind->display_data.int_val[count]), &offset);

             *p_length    += offset;
        }

        icf_pack_U8(p_dest + (*p_length), 
            (icf_uint8_t*)&(p_icf_display_ind->display_data.number_of_string_values), &offset);
        *p_length    += offset;

        for(count = 0;(count < p_icf_display_ind->display_data.number_of_string_values) && (count < ICF_MAX_STR_DISPLAY_DATA);
                count++)
        {
             icf_pack_icf_string_st(p_dest + (*p_length), 
                 (icf_uint8_t*)&(p_icf_display_ind->display_data.string_val[count]), &offset);

             *p_length    += offset;
        }
    }

    if (ICF_DISPLAY_IND_TONE & p_icf_display_ind->bit_mask)
    {
        icf_pack_U16(p_dest + (*p_length), 
            (icf_uint8_t*)&(p_icf_display_ind->tone_id), &offset);

        *p_length    += offset;
    }

    return ICF_SUCCESS;
}

/*****************************************************************************
*
* FUNCTION: icf_pack_icf_uint16_list
*
* DESCRIPTION: This function is used to pack icf_list_st of element type
*              icf_uint16_t
*
*****************************************************************************/
icf_uint32_t
icf_pack_icf_uint16_list
(
    icf_uint8_t    *p_dest,
    icf_list_st    *p_icf_uint16_list,
    icf_uint32_t    *p_length
)
{
    icf_uint32_t        offset = 0;
    icf_uint8_t            count = 0;
    
    icf_list_st       *p_list_ptr = ICF_NULL;
   
    /* This function packs list of icf_uint16_t */
    ICF_CLIB_TRACE(("Packing list of icf_uint16_t\n"));

    /* Initialise length to 0 */
    *p_length    = 0;

    /*Pack count = 0 initially*/
    icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&count, &offset);
    
    *p_length    += offset;

    /*Pack element of type icf_uint16_t*/
    for(p_list_ptr = p_icf_uint16_list;ICF_NULL != p_list_ptr;
        p_list_ptr = p_list_ptr->p_next)
    {

        icf_pack_U16(p_dest + (*p_length), (p_list_ptr->p_data), &offset);

        *p_length += offset;

        count++;

    }

    if(count)
    {
        /* Updating the count */
        icf_pack_U8(p_dest, (icf_uint8_t*)&count, &offset);
    }

    return ICF_SUCCESS;
}

#ifdef ICF_DNS_LOOKUP_ENABLED
icf_uint32_t
icf_pack_icf_dns_mapping_change_ind_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t   *p_length
)
{
    icf_uint32_t                	offset = 0;

    icf_dns_mapping_change_ind_st  	*p_dns_map_ind = ICF_NULL;

    p_dns_map_ind = (icf_dns_mapping_change_ind_st *)p_src;

    /* Initialise length to 0 */
    *p_length    = 0;

    ICF_CLIB_TRACE(("Packing icf_dns_mapping_change_ind_st\n"));

    icf_pack_U8(p_dest + (*p_length), 
				(icf_uint8_t*)&(p_dns_map_ind->record_type), 
				&offset);
   
    *p_length    += offset;

    icf_pack_icf_string_st(p_dest + (*p_length), 
				(icf_uint8_t*)&(p_dns_map_ind->fqdn), 
				&offset);

    *p_length   += offset;

    icf_pack_icf_string_st(p_dest + (*p_length), 
				(icf_uint8_t*)&(p_dns_map_ind->ip), 
				&offset);

    *p_length    += offset;

    icf_pack_U8(p_dest + (*p_length), 
				(icf_uint8_t*)&(p_dns_map_ind->cause), 
				&offset);

    *p_length    += offset;

    return ICF_SUCCESS;
}

#endif /* ICF_DNS_LOOKUP_ENABLED */


/*****************************************************************************
*
* FUNCTION: icf_pack_icf_early_session_ind st
*
* DESCRIPTION: This function is used to pack icf_early_session_ind st
*              
*
*****************************************************************************/




icf_uint32_t
icf_pack_icf_early_session_ind_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t   *p_length
)

{
icf_uint32_t        offset = 0;

    icf_early_session_ind_st    *p_icf_early_session_ind_st = ICF_NULL;
    
    p_icf_early_session_ind_st    = (icf_early_session_ind_st *)p_src;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function packs icf_early_session_ind_st */
    ICF_CLIB_TRACE(("Packing icf_early_session_ind_st \n"));
    
    icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&( p_icf_early_session_ind_st->bit_mask), &offset);
    
    *p_length    += offset;


    if(p_icf_early_session_ind_st->bit_mask & ICF_HDR_LIST_PRESENT)
    {
        icf_pack_icf_header_list_st(p_dest + (*p_length), (icf_uint8_t*)&( p_icf_early_session_ind_st->header_list), &offset);

        *p_length    += offset;
    }


    icf_pack_icf_line_id_t(p_dest + (*p_length), (icf_uint8_t*)&( p_icf_early_session_ind_st\
        ->line_id), &offset);

    *p_length    += offset;


    if(p_icf_early_session_ind_st->bit_mask & ICF_EARLY_SESSION_CLG_PTY_ADDR)
    {
        icf_pack_icf_address_st(p_dest + (*p_length), (icf_uint8_t*)&( p_icf_early_session_ind_st->\
            calling_party_addr), &offset);

        *p_length    += offset;
    }



   if(p_icf_early_session_ind_st->bit_mask & ICF_EARLY_SESSION_CALLED_PTY_ADDR)
    {
        icf_pack_icf_address_st(p_dest + (*p_length), (icf_uint8_t*)&( p_icf_early_session_ind_st->called_party_addr), &offset);
   
        *p_length    += offset;
    }

return ICF_SUCCESS;

}



/*****************************************************************************
*
* FUNCTION: icf_pack_icf_early_session_cfm_st
*
* DESCRIPTION: This function is used to pack icf_early_session_cfm_st
*              
*
*****************************************************************************/


icf_uint32_t
icf_pack_icf_early_session_cfm_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t   *p_length
)

{
    icf_uint32_t        offset = 0;

    icf_early_session_cfm_st    *p_icf_early_session_cfm_st = ICF_NULL;
    
    p_icf_early_session_cfm_st    = (icf_early_session_cfm_st *)p_src;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function packs icf_inc_options_ind_st */
    ICF_CLIB_TRACE(("Packing icf_early_session_cfm_st \n"));
    
    icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&( p_icf_early_session_cfm_st->bit_mask),
       &offset);
    
    *p_length    += offset;


    icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&( p_icf_early_session_cfm_st->result), &offset);

    *p_length    += offset;


    if(ICF_EARLY_SESSION_CFM_LINE_ID_PRESENT & p_icf_early_session_cfm_st->bit_mask)
    {
        icf_pack_icf_line_id_t(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_early_session_cfm_st->line_id), &offset);
        *p_length    += offset;
        
    }

    if(ICF_EARLY_SESSION_CFM_TUNNEL_MODE_PRESENT & p_icf_early_session_cfm_st->bit_mask)
    {
       icf_pack_U8(p_dest + (*p_length), 
                   (icf_uint8_t*)&( p_icf_early_session_cfm_st->tunnel_mode), 
                   &offset);
       *p_length    += offset;
    }


    if(p_icf_early_session_cfm_st->bit_mask & ICF_EARLY_SESSION_CFM_RESPONSE_CODE_PRESENT)
    {
        icf_pack_U16(p_dest + (*p_length), (icf_uint8_t*)&( p_icf_early_session_cfm_st->\
            response_code), &offset);
   
        *p_length    += offset;

    }
    return ICF_SUCCESS;
}



/*****************************************************************************
*
* FUNCTION: icf_pack_icf_tunnelled_options_resp_st
*
* DESCRIPTION: This function is used to pack icf_tunnelled_options_resp_st
*              
*
*****************************************************************************/

icf_uint32_t
icf_pack_icf_tunnelled_options_resp_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t   *p_length
)
{
    icf_uint32_t        offset = 0;
   
    icf_tunnelled_options_resp_st    *p_icf_tunnelled_options_resp_st = ICF_NULL;
    
    p_icf_tunnelled_options_resp_st    = (icf_tunnelled_options_resp_st *)p_src;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function packs icf_tunnelled_options_resp_st */
    ICF_CLIB_TRACE(("Packing icf_tunnelled_options_resp_st\n"));
    
    icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_tunnelled_options_resp_st->\
        bit_mask), 
        &offset);
    
    *p_length    += offset;

    icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_tunnelled_options_resp_st->result), 
        &offset);
    
    *p_length    += offset;

    if(p_icf_tunnelled_options_resp_st->bit_mask & ICF_TUNNELLED_OPTIONS_ERROR_CODE_PRESENT)
    {

        icf_pack_U16(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_tunnelled_options_resp_st->\
            error_code_for_options), &offset);
    
        *p_length    += offset;
    }

    if(p_icf_tunnelled_options_resp_st->bit_mask & ICF_TUNNELLED_OPTIONS_RESP_CODE_PRESENT)
    {

        icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_tunnelled_options_resp_st->\
            response_code_for_options), &offset);
    
        *p_length    += offset;
    }

    if(p_icf_tunnelled_options_resp_st->bit_mask & ICF_HDR_LIST_PRESENT)
    {
        icf_pack_icf_header_list_st(p_dest + (*p_length), 
            (icf_uint8_t*)&(p_icf_tunnelled_options_resp_st->header_list), &offset);

        *p_length    += offset;
    }

    if(p_icf_tunnelled_options_resp_st->bit_mask & ICF_BODY_LIST_PRESENT)
    {
        icf_pack_icf_msg_body_list_st(p_dest + (*p_length), 
            (icf_uint8_t*)&(p_icf_tunnelled_options_resp_st->body_list), &offset);
        
        *p_length    += offset;
    }

    /* Code to pack the icf_tunnelled_sdp_info structure */

    if(p_icf_tunnelled_options_resp_st->bit_mask & ICF_TUNNELLED_OPTIONS_REMOTE_SDP_PRESENT)
    {
        
        icf_pack_icf_tunnelled_sdp_info_st(p_dest + (*p_length), (icf_uint8_t*)&\
            (p_icf_tunnelled_options_resp_st->tunnelled_remote_sdp), &offset);

        *p_length    += offset;
    }
return ICF_SUCCESS;
}

/*****************************************************************************
*
* FUNCTION: icf_pack_icf_tunnelled_sdp_info_st
*
* DESCRIPTION: This function is used to pack the 
*              icf_pack_icf_tunnelled_sdp_info_st
*
*****************************************************************************/

icf_uint32_t
icf_pack_icf_tunnelled_sdp_info_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t   *p_length
)
{
    icf_uint32_t        offset = 0;
    icf_uint8_t		i;
   
    icf_tunnelled_sdp_info_st *p_icf_tunnelled_sdp_info_st = ICF_NULL;
    
    p_icf_tunnelled_sdp_info_st = (icf_tunnelled_sdp_info_st *)p_src;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function packs icf_tunnelled_sdp_info_st */
    ICF_CLIB_TRACE(("Packing icf_tunnelled_sdp_info_st\n"));
    icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&\
                 (p_icf_tunnelled_sdp_info_st->\
                  bitmask), &offset);
     *p_length    += offset;

        if(p_icf_tunnelled_sdp_info_st->bitmask & 
            ICF_TUNNELLED_SDP_SESSION_CLINE_PRESENT)
        {

            icf_pack_icf_sdf_string_st(p_dest + (*p_length), (icf_uint8_t*)&\
                (p_icf_tunnelled_sdp_info_st->\
                     session_level_cline.dNetType), &offset);
        
            *p_length    += offset;
            icf_pack_icf_sdf_address_st(p_dest + (*p_length), (icf_uint8_t*)&\
               (p_icf_tunnelled_sdp_info_st->\
                   session_level_cline.dNetworkAddress), &offset);

            *p_length    += offset;

            
        }

        icf_pack_U8(p_dest + (*p_length),(icf_uint8_t*)&\
            ( p_icf_tunnelled_sdp_info_st->num_of_m_line),
            &offset);

            *p_length    += offset;


        for (i = 0; (i < p_icf_tunnelled_sdp_info_st->\
            num_of_m_line) && (i< ICF_MAX_NO_OF_M_LINES); i++)
        {
            icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&\
                 (p_icf_tunnelled_sdp_info_st->\
                  med_level_desc[i].bitmask), &offset);
             *p_length    += offset;

            if(p_icf_tunnelled_sdp_info_st->\
                med_level_desc[i].bitmask & ICF_TUNNELLED_SDP_MEDIA_CLINE_PRESENT)
            {
                icf_pack_icf_sdf_string_st(p_dest + (*p_length), (icf_uint8_t*)&\
                    (p_icf_tunnelled_sdp_info_st->\
                    med_level_desc[i].med_level_cline.dNetType), &offset);
        
                *p_length    += offset;

                icf_pack_icf_sdf_address_st(p_dest + (*p_length), (icf_uint8_t*)&\
                    (p_icf_tunnelled_sdp_info_st->\
                    med_level_desc[i].med_level_cline.dNetworkAddress), &offset);

                *p_length    += offset;

                
            }

            icf_pack_icf_sdf_string_st(p_dest + (*p_length), (icf_uint8_t*)&\
                (p_icf_tunnelled_sdp_info_st->\
                med_level_desc[i].m_line.dMediaType), &offset);

            *p_length    += offset;


            icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&\
                (p_icf_tunnelled_sdp_info_st->\
                med_level_desc[i].m_line.dPort), &offset);


            *p_length    += offset;


            icf_pack_U16(p_dest + (*p_length), (icf_uint8_t*)&\
                (p_icf_tunnelled_sdp_info_st->\
                med_level_desc[i].m_line.dNumOfPorts), &offset);



            *p_length    += offset;


            icf_pack_icf_sdf_string_st(p_dest + (*p_length), (icf_uint8_t*)&\
                (p_icf_tunnelled_sdp_info_st->\
                med_level_desc[i].m_line.dProtocol), &offset);

            *p_length    += offset;


            icf_pack_icf_sdf_string_st(p_dest + (*p_length), (icf_uint8_t*)&\
                (p_icf_tunnelled_sdp_info_st->\
                med_level_desc[i].m_line.dFormat), &offset);

            *p_length    += offset;

        }


        icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&( p_icf_tunnelled_sdp_info_st->num_of_encoded_string),
            &offset);

                   
       *p_length    += offset;

        for (i = 0; (i < p_icf_tunnelled_sdp_info_st->\
            num_of_encoded_string) && (i< ICF_MAX_NO_OF_ENC_STRING); i++)
        {
            icf_pack_icf_sdf_string_st(p_dest + (*p_length), (icf_uint8_t*)&\
                (p_icf_tunnelled_sdp_info_st->\
                encoded_string[i].attr_str), &offset);

            *p_length    += offset;


            icf_pack_U8(p_dest + (*p_length),(icf_uint8_t*)&\
                ( p_icf_tunnelled_sdp_info_st->\
                encoded_string[i].level),&offset);

            *p_length    += offset;


            icf_pack_U8(p_dest + (*p_length),(icf_uint8_t*)&\
            ( p_icf_tunnelled_sdp_info_st->\
            encoded_string[i].pos_of_media),&offset);

            *p_length    += offset;

        }
return ICF_SUCCESS;
}

#ifdef ICF_SESSION_TIMER
icf_uint32_t
icf_pack_icf_session_timer_toggle_req_st
(
 icf_uint8_t    *p_dest,
 icf_uint8_t    *p_src,
 icf_uint32_t    *p_length
)
{
   	icf_uint32_t        offset = 0;
 	icf_session_timer_toggle_req_st *p_icf_session_timer_toggle_req_st =
           ICF_NULL;   

    /* Initialise length to 0 */
    *p_length    = 0;	

	p_icf_session_timer_toggle_req_st    = 
	  (icf_session_timer_toggle_req_st *)p_src;

    /* This function packs icf_session_timer_toggle_req_st */
    ICF_CLIB_TRACE(("Packing icf_session_timer_toggle_req_st \n")); 
    
    icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)
	    &(p_icf_session_timer_toggle_req_st ->bit_mask), &offset);
    *p_length    += offset;

    if (p_icf_session_timer_toggle_req_st->bit_mask &
             ICF_SESS_TOGG_REQ_MINSE_PRESENT)
    {
        icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)
                &(p_icf_session_timer_toggle_req_st->min_se), &offset);
        *p_length    += offset;
    }  
    if (p_icf_session_timer_toggle_req_st->bit_mask &
            ICF_SESS_TOGG_REQ_TIMER_PRESENT)
    {
        icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)
                &(p_icf_session_timer_toggle_req_st->session_timer), &offset);
        *p_length    += offset;
    } 
    if (p_icf_session_timer_toggle_req_st->bit_mask &
            ICF_SESS_TOGG_REQ_REFRESHER_PRESENT)
    {
        icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)
                &(p_icf_session_timer_toggle_req_st->session_refresher), &offset);
        *p_length    += offset;
    }
    return ICF_SUCCESS;
}


icf_uint32_t
icf_pack_icf_session_timer_toggle_resp_st
(
 icf_uint8_t    *p_dest,
 icf_uint8_t    *p_src,
 icf_uint32_t    *p_length
)
{
   	icf_uint32_t        offset = 0;
 	icf_session_timer_toggle_resp_st *p_icf_session_timer_toggle_resp_st =
           ICF_NULL;   

    /* Initialise length to 0 */
    *p_length    = 0;	

	p_icf_session_timer_toggle_resp_st    = 
	  (icf_session_timer_toggle_resp_st *)p_src;

    /* This function packs icf_session_timer_toggle_resp_st */
    ICF_CLIB_TRACE(("Packing icf_session_timer_toggle_resp_st \n")); 
    
    icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)
	    &(p_icf_session_timer_toggle_resp_st ->bit_mask), &offset);
    *p_length    += offset;

    icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)
	    &(p_icf_session_timer_toggle_resp_st ->result), &offset);
    *p_length    += offset;

    if (p_icf_session_timer_toggle_resp_st->bit_mask &
             ICF_SESSION_TOGG_RESP_ERROR_PRESENT)
    {
        icf_pack_U16(p_dest + (*p_length), (icf_uint8_t*)
                &(p_icf_session_timer_toggle_resp_st->error_cause), &offset);
        *p_length    += offset;
    }  
    
    return ICF_SUCCESS;
}
#endif
/*Change Start for Rel 8.2 */
/******************************************************************************
 * Function name:  icf_pack_icf_generic_msg_req_st
 * Description:   This function packs the structure 
 *                 icf_generic_msg_req_st
 * Return Value - ICF_SUCCESS
*******************************************************************************/
icf_uint32_t
icf_pack_icf_generic_msg_req_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t    *p_length
)
{
    icf_uint32_t        offset = 0;
    
    icf_generic_msg_req_st   *p_icf_generic_msg_req_st = ICF_NULL;

    p_icf_generic_msg_req_st   = (icf_generic_msg_req_st *)p_src;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function packs icf_generic_msg_req_st*/
    ICF_CLIB_TRACE(("Packing icf_generic_msg_req_st\n"));

    icf_pack_U32(p_dest + (*p_length), \
        (icf_uint8_t*)&(p_icf_generic_msg_req_st->bit_mask), &offset);

    *p_length    += offset;
        
    icf_pack_icf_string_st(p_dest + (*p_length), \
            (icf_uint8_t*)&(p_icf_generic_msg_req_st->method_name), &offset);
    
    *p_length    += offset;
    
    if(p_icf_generic_msg_req_st->bit_mask & ICF_HDR_LIST_PRESENT)
    {
        icf_pack_icf_header_list_st(p_dest + (*p_length), \
                (icf_uint8_t*)&(p_icf_generic_msg_req_st->header_list), &offset);
        
        *p_length    += offset;
    }

    if(p_icf_generic_msg_req_st->bit_mask & ICF_BODY_LIST_PRESENT)
    {
        icf_pack_icf_msg_body_list_st(p_dest + (*p_length), \
                (icf_uint8_t*)&(p_icf_generic_msg_req_st->body_list), &offset);
        
        *p_length    += offset;
    }
    
    if(p_icf_generic_msg_req_st->bit_mask & ICF_TAG_LIST_PRESENT)
    {
        icf_pack_icf_tag_list_st(p_dest + (*p_length), \
                (icf_uint8_t*)&(p_icf_generic_msg_req_st->tag_list), &offset);
        
        *p_length    += offset;
    }
    return ICF_SUCCESS;
}

/******************************************************************************
 * Function name:  icf_pack_icf_generic_msg_resp_st
 * Description:   This function packs the structure 
 *                 icf_generic_msg_resp_st
 * Return Value - ICF_SUCCESS
*******************************************************************************/
icf_uint32_t
icf_pack_icf_generic_msg_resp_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t    *p_length
)
{
    icf_uint32_t        offset = 0;

    icf_generic_msg_resp_st    *p_icf_generic_msg_resp_st = ICF_NULL;

    p_icf_generic_msg_resp_st    = (icf_generic_msg_resp_st *)p_src;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function packs  icf_generic_msg_resp_st*/
    ICF_CLIB_TRACE(("Packing icf_generic_msg_resp_st\n"));

    icf_pack_U32(p_dest + (*p_length), \
            (icf_uint8_t*)&(p_icf_generic_msg_resp_st->bit_mask), &offset);
    *p_length    += offset;
    
    icf_pack_icf_string_st(p_dest + (*p_length), \
            (icf_uint8_t*)&(p_icf_generic_msg_resp_st->method_name), &offset);
    *p_length    += offset;

    if(p_icf_generic_msg_resp_st->bit_mask & ICF_GENERIC_MSG_RESP_RESPONSE_CODE_PRESENT)
    {
        icf_pack_U16(p_dest + (*p_length), \
                (icf_uint8_t*)&(p_icf_generic_msg_resp_st->response_code), &offset);
        *p_length	+= offset;
    }

    icf_pack_U8(p_dest + (*p_length), \
            (icf_uint8_t*)&(p_icf_generic_msg_resp_st->result), &offset);
    *p_length	+= offset;

    if(p_icf_generic_msg_resp_st->bit_mask &  ICF_GENERIC_MSG_RESP_ERROR_CAUSE_PRESENT)
    {
       icf_pack_U16(p_dest + (*p_length), \
               (icf_uint8_t*)&(p_icf_generic_msg_resp_st->error_cause), &offset);
       *p_length    += offset;
    }   
    if(p_icf_generic_msg_resp_st->bit_mask & ICF_HDR_LIST_PRESENT)
    {
        icf_pack_icf_header_list_st(p_dest + (*p_length), \
                (icf_uint8_t*)&(p_icf_generic_msg_resp_st->header_list), &offset);
        
        *p_length    += offset;
    }
    if(p_icf_generic_msg_resp_st->bit_mask & ICF_BODY_LIST_PRESENT)
    {
        icf_pack_icf_msg_body_list_st(p_dest + (*p_length), \
                (icf_uint8_t*)&(p_icf_generic_msg_resp_st->body_list), &offset);
        
        *p_length    += offset;
    }
    return ICF_SUCCESS;
}

/******************************************************************************
 * Function name:  icf_pack_icf_generic_msg_ind_st
 * Description:   This function packs the structure 
 *                 icf_generic_msg_ind_st
 * Return Value - ICF_SUCCESS
*******************************************************************************/
icf_uint32_t
icf_pack_icf_generic_msg_ind_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t    *p_length
)
{
    icf_uint32_t        offset = 0;

    icf_generic_msg_ind_st    *p_icf_generic_msg_ind_st = ICF_NULL;

    p_icf_generic_msg_ind_st    = (icf_generic_msg_ind_st *)p_src;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function packs icf_generic_msg_ind_st*/
    ICF_CLIB_TRACE(("Packing icf_generic_msg_ind_st\n"));

    icf_pack_U32(p_dest + (*p_length), \
            (icf_uint8_t*)&(p_icf_generic_msg_ind_st->bit_mask), &offset);
    *p_length    += offset;
    
    icf_pack_icf_string_st(p_dest + (*p_length), \
            (icf_uint8_t*)&(p_icf_generic_msg_ind_st->method_name), &offset);
    *p_length    += offset;

    if(p_icf_generic_msg_ind_st->bit_mask & ICF_HDR_LIST_PRESENT)
    {
        icf_pack_icf_header_list_st(p_dest + (*p_length), \
                (icf_uint8_t*)&(p_icf_generic_msg_ind_st->header_list), &offset);
        
        *p_length    += offset;
    }
    if(p_icf_generic_msg_ind_st->bit_mask & ICF_BODY_LIST_PRESENT)
    {
        icf_pack_icf_msg_body_list_st(p_dest + (*p_length), \
                (icf_uint8_t*)&(p_icf_generic_msg_ind_st->body_list), &offset);
        
        *p_length    += offset;
    }
    return ICF_SUCCESS;
}

/******************************************************************************
 * Function name:  icf_pack_icf_generic_msg_cfm_st
 * Description:   This function packs the structure 
 *                 icf_generic_msg_cfm_st
 * Return Value - ICF_SUCCESS
*******************************************************************************/
icf_uint32_t
icf_pack_icf_generic_msg_cfm_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t    *p_length
)
{
    icf_uint32_t        offset = 0;

    icf_generic_msg_cfm_st    *p_icf_generic_msg_cfm_st = ICF_NULL;

    p_icf_generic_msg_cfm_st    = (icf_generic_msg_cfm_st *)p_src;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function packs icf_generic_msg_cfm_st*/
    ICF_CLIB_TRACE(("Packing icf_generic_msg_cfm_st\n"));

    icf_pack_U32(p_dest + (*p_length), \
            (icf_uint8_t*)&(p_icf_generic_msg_cfm_st->bit_mask), &offset);
    *p_length    += offset;
    
    icf_pack_icf_string_st(p_dest + (*p_length), \
            (icf_uint8_t*)&(p_icf_generic_msg_cfm_st->method_name), &offset);
    *p_length    += offset;
	
    icf_pack_U16(p_dest + (*p_length), \
            (icf_uint8_t*)&(p_icf_generic_msg_cfm_st->response_code), &offset);
    *p_length	+= offset;

    if(p_icf_generic_msg_cfm_st->bit_mask & ICF_HDR_LIST_PRESENT)
    {
        icf_pack_icf_header_list_st(p_dest + (*p_length), \
                (icf_uint8_t*)&(p_icf_generic_msg_cfm_st->header_list), &offset);
        *p_length    += offset;
    }

    if(p_icf_generic_msg_cfm_st->bit_mask & ICF_BODY_LIST_PRESENT)
    {
        icf_pack_icf_msg_body_list_st(p_dest + (*p_length), \
                (icf_uint8_t*)&(p_icf_generic_msg_cfm_st->body_list), &offset);
        *p_length    += offset;
    }

    if(p_icf_generic_msg_cfm_st->bit_mask & ICF_TAG_LIST_PRESENT)
    {
        icf_pack_icf_tag_list_st(p_dest + (*p_length), \
                (icf_uint8_t*)&(p_icf_generic_msg_cfm_st->tag_list), &offset);
        *p_length    += offset;
    }
    return ICF_SUCCESS;
}    

/******************************************************************************
 * Function name:  icf_pack_icf_prack_req_recv_ind_st
 * Description:   This function packs the structure 
 *                 icf_prack_req_recv_ind
 * Return Value - ICF_SUCCESS
*******************************************************************************/
icf_uint32_t
icf_pack_icf_prack_req_recv_ind_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t    *p_length
)
{
    icf_uint32_t        offset = 0;

    icf_prack_req_recv_ind_st   *p_icf_prack_req_recv_ind_st = ICF_NULL;

    p_icf_prack_req_recv_ind_st   = (icf_prack_req_recv_ind_st *)p_src;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function packs icf_prack_req_recv_ind_st*/
    ICF_CLIB_TRACE(("Packing icf_prack_req_recv_ind_st\n"));

    icf_pack_U32(p_dest + (*p_length), \
            (icf_uint8_t*)&(p_icf_prack_req_recv_ind_st->bit_mask), &offset);

    *p_length    += offset;

    icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_prack_req_recv_ind_st->settings), &offset);

    *p_length    += offset;


    if(p_icf_prack_req_recv_ind_st->bit_mask & ICF_HDR_LIST_PRESENT)
    {
        icf_pack_icf_header_list_st(p_dest + (*p_length), \
                (icf_uint8_t*)&(p_icf_prack_req_recv_ind_st->header_list), &offset);

        *p_length    += offset;
    }


    if(p_icf_prack_req_recv_ind_st->bit_mask & ICF_BODY_LIST_PRESENT)
    {
        icf_pack_icf_msg_body_list_st(p_dest + (*p_length), \
                (icf_uint8_t*)&(p_icf_prack_req_recv_ind_st->body_list), &offset);

        *p_length    += offset;
    }

    return ICF_SUCCESS;
}



/******************************************************************************
 * Function name:  icf_pack_icf_prack_resp_sent_ind_st
 * Description:   This function packs the structure 
 *                 icf_prack_resp_sent_ind
 * Return Value - ICF_SUCCESS
*******************************************************************************/
icf_uint32_t
icf_pack_icf_prack_resp_sent_ind_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t    *p_length
)
{
    icf_uint32_t        offset = 0;

    icf_prack_resp_sent_ind_st   *p_icf_prack_resp_sent_ind_st = ICF_NULL;

    p_icf_prack_resp_sent_ind_st   = (icf_prack_resp_sent_ind_st *)p_src;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function packs icf_prack_resp_sent_ind_st*/
    ICF_CLIB_TRACE(("Packing icf_prack_resp_sent_ind_st\n"));

    icf_pack_U32(p_dest + (*p_length), \
            (icf_uint8_t*)&(p_icf_prack_resp_sent_ind_st->bit_mask), &offset);

    *p_length    += offset;

    icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_prack_resp_sent_ind_st->settings), &offset);

    *p_length    += offset;

    icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_prack_resp_sent_ind_st->response_code), &offset);
    *p_length    += offset;


    if(p_icf_prack_resp_sent_ind_st->bit_mask & ICF_HDR_LIST_PRESENT)
    {
        icf_pack_icf_header_list_st(p_dest + (*p_length), \
                (icf_uint8_t*)&(p_icf_prack_resp_sent_ind_st->header_list), &offset);

        *p_length    += offset;
    }     
    return ICF_SUCCESS;
}

/******************************************************************************
 * Function name:  icf_pack_icf_prack_req_sent_ind_st
 * Description:   This function packs the structure 
 *                 icf_prack_req_sent_ind
 * Return Value - ICF_SUCCESS
*******************************************************************************/
icf_uint32_t
icf_pack_icf_prack_req_sent_ind_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t    *p_length
)
{
    icf_uint32_t        offset = 0;

    icf_prack_req_sent_ind_st   *p_icf_prack_req_sent_ind_st = ICF_NULL;

    p_icf_prack_req_sent_ind_st   = (icf_prack_req_sent_ind_st *)p_src;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function packs icf_prack_req_sent_ind_st*/
    ICF_CLIB_TRACE(("Packing icf_prack_req_sent_ind_st\n"));

    icf_pack_U32(p_dest + (*p_length), \
            (icf_uint8_t*)&(p_icf_prack_req_sent_ind_st->bit_mask), &offset);

    *p_length    += offset;


    if(p_icf_prack_req_sent_ind_st->bit_mask & ICF_HDR_LIST_PRESENT)
    {
        icf_pack_icf_header_list_st(p_dest + (*p_length), \
                (icf_uint8_t*)&(p_icf_prack_req_sent_ind_st->header_list), &offset);

        *p_length    += offset;
    }        
    return ICF_SUCCESS;
}

/******************************************************************************
 * Function name:  icf_pack_icf_prack_resp_recv_ind_st
 * Description:   This function packs the structure 
 *                 icf_prack_resp_recv_ind
 * Return Value - ICF_SUCCESS
*******************************************************************************/
icf_uint32_t
icf_pack_icf_prack_resp_recv_ind_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t    *p_length
)
{
    icf_uint32_t        offset = 0;

    icf_prack_resp_recv_ind_st   *p_icf_prack_resp_recv_ind_st = ICF_NULL;

    p_icf_prack_resp_recv_ind_st   = (icf_prack_resp_recv_ind_st *)p_src;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function packs icf_prack_resp_recv_ind_st*/
    ICF_CLIB_TRACE(("Packing icf_prack_resp_recv_ind_st\n"));

    icf_pack_U32(p_dest + (*p_length), \
            (icf_uint8_t*)&(p_icf_prack_resp_recv_ind_st->bit_mask), &offset);

    *p_length    += offset;

    icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_prack_resp_recv_ind_st->settings), &offset);

    *p_length    += offset;

    icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_prack_resp_recv_ind_st->response_code), &offset);
    *p_length    += offset;


    if(p_icf_prack_resp_recv_ind_st->bit_mask & ICF_HDR_LIST_PRESENT)
    {
        icf_pack_icf_header_list_st(p_dest + (*p_length), \
                (icf_uint8_t*)&(p_icf_prack_resp_recv_ind_st->header_list), &offset);

        *p_length    += offset;
    }


    if(p_icf_prack_resp_recv_ind_st->bit_mask & ICF_BODY_LIST_PRESENT)
    {
        icf_pack_icf_msg_body_list_st(p_dest + (*p_length), \
                (icf_uint8_t*)&(p_icf_prack_resp_recv_ind_st->body_list), &offset);

        *p_length    += offset;
    }

    return ICF_SUCCESS;
}



/******************************************************************************
 * Function name:  icf_pack_icf_app_confirmation_st
 * Description:   This function packs the structure 
 *                 icf_message_cfm_st
 * Return Value - ICF_SUCCESS
*******************************************************************************/
icf_uint32_t
icf_pack_icf_app_confirmation_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t    *p_length
)
{
    icf_uint32_t        offset = 0;

    icf_app_confirmation_st    *p_icf_app_confirmation_st = ICF_NULL;

    p_icf_app_confirmation_st    = (icf_app_confirmation_st *)p_src;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function packs icf_app_confirmation_st*/
    ICF_CLIB_TRACE(("Packing icf_app_confirmation_st\n"));

    icf_pack_U32(p_dest + (*p_length), \
            (icf_uint8_t*)&(p_icf_app_confirmation_st->bit_mask), &offset);
    *p_length    += offset;
    
    if(p_icf_app_confirmation_st->bit_mask & ICF_APP_CONFIRMATION_RESP_CODE_PRESENT)
    {
    icf_pack_U16(p_dest + (*p_length), \
            (icf_uint8_t*)&(p_icf_app_confirmation_st->response_code), &offset);
    *p_length	+= offset;
    }

    icf_pack_U8(p_dest + (*p_length), \
            (icf_uint8_t*)&(p_icf_app_confirmation_st->result), &offset);

    *p_length    += offset;


    if(p_icf_app_confirmation_st->bit_mask & ICF_HDR_LIST_PRESENT)
    {
        icf_pack_icf_header_list_st(p_dest + (*p_length), \
                (icf_uint8_t*)&(p_icf_app_confirmation_st->header_list), &offset);
        *p_length    += offset;
    }

    if(p_icf_app_confirmation_st->bit_mask & ICF_TAG_LIST_PRESENT)
    {
        icf_pack_icf_tag_list_st(p_dest + (*p_length), \
                (icf_uint8_t*)&(p_icf_app_confirmation_st->tag_list), &offset);
        *p_length    += offset;
    }
    return ICF_SUCCESS;
}



/*Change End for Rel 8.2 */

