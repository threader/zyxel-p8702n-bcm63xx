/*****************************************************************************
 ** FUNCTION:    
 **            This file contains printing routines
 **                     for use by the ICF application.
 **
 ******************************************************************************
 **
 ** FILENAME:        icf_print_message.c
 **
 ** DESCRIPTION:    This file contains Printing routines for various
 **                 messages generated during the application execution 
 **                     ICF Application.
 ** Revision History :
 **
 ** Date         Name            Ref#                Comments
 ** --------     ------------    ----                ---------------
 ** 20-Jun-2007  Tarun Gupta     SPR 15059           Change in function icf_print_term_call_ind:
 **                                                  Added a trace.
 ** 13-Nov-2007  Jagmohan Chauhan SPR 17489          Change in function icf_print_set_line_param_req 
 ** 23-Dec-2007  Jagmohan Chauhan SPR 17489          To change all %d format specifier to %u in printing  
 **                                                  api's  structures            
 ** 07-Jan-2007  Amit Sikka       SPR 17945          Support for PUBLISH/MESSAGE
 **    
 ** 07-Jan-2009  Vinay Jindal     SPR 19250          Trace for new parameter in 
 **                                                  set_service_param added
 ** 05-Feb-2009   Rohit Aggarwal  SPR 19316   Changes for INVITE with Replaces
 ** 19-Dec-2009  Rohit Aggarwal   SPR 20246  Configuration to strip Route header
 ** 23-Mar-2010  Rohit Aggarwal   SPR 20343  Event name in subscription ind
 ** 29-Mar-2010  Kamal Ashraf                       CSR_1-8389294(Registaration Enh)
 ** 09-Apr-2010  Alok Tiwari      SPR 20362         Modified function
 **                                                 icf_print_options_req
 **    
 *****************************************************************************
 **             Copyrights 2010, Aricent.
 *****************************************************************************/

#include "icf_common_prototypes.h"
#include "icf_port_prototypes.h"
#include "icf_common_types.h"
#include "icf_common_defs.h"
#include "icf_macro.h"
#include "icf_es_types.h"
#include "icf_es_prototypes.h"
#include "icf_es_macro.h"
#include "icf_es_intf_trace.h" 
#include "icf_common_defs.h" 
#include "icf_error_id.h"
#include "icf_api.h"
#include "icf_mm_error_id.h"
#include "icf_port_pltfm_types.h"

#ifdef ICF_NAT_MEDIA
#include "stun_enc_dec_prototypes.h"
nat_uint128_t transaction_id;
#endif


#ifdef ICF_INTERFACE_TRACE_ENABLE


/******************************************************************************
 *
 * FUNCTION:        icf_print_msg()
 *
 * DESCRIPTION:     Try printing icf_msg_st messages
 *
 * Functions called: 
 *                     
 *
 *****************************************************************************/

icf_return_t icf_print_msg(icf_msg_st* p_msg)
{

 
    if (!p_msg) {
        ICF_PRINT_MESSAGE(("\nFATAL: NULL Argument to icf_print_msg()"));
        return ICF_FAILURE;
    }

     icf_print_api_from_api_id(p_msg->hdr.api_id,p_msg);

#ifdef ICF_IPSEC_ENABLED
    if (ICF_MSG_FRM_PLTFM == p_msg->hdr.api_id)
    {

     }

#endif

    return ICF_SUCCESS;
}




/****************************************************************************
 *
 * FUNCTION:        icf_print_api_header
 *
 * DESCRIPTION:     Print the api header inside the icf_msg_st
 *
 * Functions called: 
 *                     
 *
 ****************************************************************************/

icf_return_t icf_print_api_header(icf_msg_st* p_msg)
{
   if(ICF_NULL == p_msg)
   {
       ICF_PRINT_MESSAGE(("p_msg is null"));
       return ICF_SUCCESS;

   }
 
   /* icf_print_api_from_api_id(p_msg->hdr.api_id,p_msg);*/

   /* ICF_PRINT_MESSAGE(("\nversion = %u, app_id = %u",p_msg->hdr.version, p_msg->hdr.app_id))
    ICF_PRINT_MESSAGE(("\nsource_id = %u, ",p_msg->hdr.source_id))

    ICF_PRINT_MESSAGE(("dest_id = %u, ",p_msg->hdr.destination_id))
    ICF_PRINT_MESSAGE(("\ncall_id = %u, ",(icf_call_id_t)p_msg->hdr.call_id))
    
    ICF_PRINT_MESSAGE(("api_length = %u",p_msg->hdr.api_length))*/

    return ICF_SUCCESS;
  
    
    
}




/******************************************************************************
 *
 * FUNCTION:        icf_print_no_payload_msg()
 *
 * DESCRIPTION:     Print messages with no payload 
 *
 * Functions called: 
 *                     
 *
 *****************************************************************************/

icf_return_t icf_print_no_payload_msg(icf_msg_st* p_msg)
{

    icf_print_api_header(p_msg);
    
   /* ICF_PRINT_MESSAGE(("\n{\n}\n"));*/

#ifdef ICF_FOR_LOAD_TESTING

	if ( p_msg->hdr.api_id == ICF_REMOTE_USER_TERMINATE_CALL_RESP)
    {
		t_calls++;
        ICF_PRINT_MESSAGE(("\nTerminated Calls:%u",t_calls));
    }

#endif

#ifdef ICF_AUTO_RESPOND
    if (p_msg->hdr.api_id == ICF_DELETE_MEDIA_SESSION_REQ && 
        p_msg->hdr.destination_id == ICF_MODULE_MM)
        icf_command_handler("SEND dms_resp dms_resp.mg");
#endif

    return ICF_SUCCESS;
}




/*****************************************************************************
 *
 * FUNCTION:        icf_print_resp_msg()
 *
 * DESCRIPTION:     Print response messages 
 *
 * Functions called: 
 *                     
 *
 ****************************************************************************/

icf_return_t icf_print_resp_msg(icf_msg_st* p_msg)
{

    icf_msg_resp_st* p_resp_msg = NULL;

    icf_print_api_header(p_msg);

    p_resp_msg = (icf_msg_resp_st*)p_msg->payload;

    ICF_PRINT_MESSAGE(("\nresult = %u, error_cause = %u",
              p_resp_msg->result, p_resp_msg->error_cause));
    icf_print_error_details(p_resp_msg->error_cause);

    if (p_resp_msg->bit_mask & ICF_MSG_RESP_LINE_ID_PRESENT)
    {
       ICF_PRINT_MESSAGE(("\nLine_id = %u",p_resp_msg->line_id));
    }            

    return ICF_SUCCESS;
}

/*****************************************************************************
 *
 * FUNCTION:        icf_print_api_resp_msg()
 *
 * DESCRIPTION:     Print response messages 
 *
 * Functions called: 
 *                     
 *
 ****************************************************************************/

icf_return_t icf_print_api_resp_msg(icf_msg_st* p_msg)
{

    icf_api_resp_st* p_resp_msg = NULL;
	icf_uint8_t		i = 0;

    icf_print_api_header(p_msg);

    p_resp_msg = (icf_api_resp_st*)p_msg->payload;

    ICF_PRINT_MESSAGE(("\nresult = %u, error_cause = %u",
              p_resp_msg->result, p_resp_msg->error_cause));

    icf_print_error_details(p_resp_msg->error_cause);

    ICF_PRINT_MESSAGE(("\nbit_mask = %u", 
              p_resp_msg->bit_mask));

/* Header LIST */
    if (p_resp_msg->bit_mask & ICF_HDR_LIST_PRESENT) {
        icf_print_header_list(p_resp_msg->header_list.hdr, p_resp_msg->header_list.count);
    }    

/* Body LIST */
    if (p_resp_msg->bit_mask & ICF_BODY_LIST_PRESENT) {
        icf_print_body_list(p_resp_msg->body_list.body, p_resp_msg->body_list.count);
    }
#ifdef ICF_REL_OLD
    if (p_resp_msg->bit_mask & ICF_STREAM_LIST_PRESENT) {
        icf_print_stream_list(p_resp_msg->stream_list.stream, p_resp_msg->stream_list.count);
    }
#endif
	

	
	if(p_resp_msg->bit_mask & ICF_STREAM_LIST_PRESENT)
	{
		ICF_PRINT_MESSAGE(("\n stream count %u:",p_resp_msg->stream_list.count));	

		for(i=0;i<p_resp_msg->stream_list.count;i++)
		{
			ICF_PRINT_MESSAGE(("\n stream type %u:",p_resp_msg->stream_list.stream_id[i]));
		}
	}

   
    return ICF_SUCCESS;
}

icf_return_t icf_print_string_list(icf_string_list_st *p_str_list,icf_uint8_t *p_list_name)
{
    icf_string_list_node_st *p_cur_node;
    icf_uint8_t          i;

    ICF_PRINT_MESSAGE(("\n\n%s.count = %u",p_list_name,p_str_list->count));
    for(i = 0, p_cur_node = p_str_list->p_list;
    i < p_str_list->count && (p_cur_node);
    p_cur_node = p_cur_node->p_next,i++)
    {
        ICF_PRINT_MESSAGE(("\n%s%u = %s",p_list_name,i+1,p_cur_node->str.str));
    }
    return ICF_SUCCESS;
}




/*****************************************************************************
 *
 * FUNCTION:        icf_print_config_params_resp()
 *
 * DESCRIPTION:     Print set_config_params_resp
 *
 * Functions called: 
 *                     
 *
 ****************************************************************************/

icf_return_t icf_print_config_params_resp(icf_msg_st* p_msg)
{

    icf_cc_cfg_set_config_params_resp_st* p_resp_msg = NULL;

    icf_print_api_header(p_msg);

    p_resp_msg = (icf_cc_cfg_set_config_params_resp_st*)p_msg->payload;

    ICF_PRINT_MESSAGE(("\napi_id = %u, line_id = %u, result = %u, error_cause = %u",\
			p_resp_msg->api_id, p_resp_msg->line_id, 
			p_resp_msg->result, p_resp_msg->error_cause));

    icf_print_error_details(p_resp_msg->error_cause);


   return ICF_SUCCESS;
}/* Function icf_print_config_params_resp() ends here */





icf_return_t icf_print_set_scale_params(icf_msg_st* p_msg)
{

    
    icf_cfg_cc_set_scale_params_req_st* p_scale = NULL;
    
    icf_print_api_header(p_msg);
    
    
    p_scale = (icf_cfg_cc_set_scale_params_req_st*)p_msg->payload;

    ICF_PRINT_MESSAGE(("\nbit_mask= %u,",p_scale->bit_mask));

    ICF_PRINT_MESSAGE(("\nmax_lines = %u, max_audio_calls = %u, ", p_scale->max_lines, p_scale->max_audio_calls));
                   
    ICF_PRINT_MESSAGE(("\nmax_video_calls = %u,",p_scale->max_video_calls));
       
    ICF_PRINT_MESSAGE(("\nmax_service_calls = %u, max_registration_calls = %u, ",p_scale->max_service_calls, p_scale->max_registration_calls)); 

    ICF_PRINT_MESSAGE(("\nmax_users_per_line = %u",p_scale->max_users_per_line));
    
    ICF_PRINT_MESSAGE(("\nnum_of_calls_to_log = %u",p_scale->num_of_calls_to_log));

    ICF_PRINT_MESSAGE(("\nmax_num_of_apps = %u",p_scale->max_num_of_apps));

    return ICF_SUCCESS;

}




icf_return_t icf_print_set_trans_params(icf_msg_st* p_msg)
{

    
    icf_uint8_t i = 0,addr_type=0;
    icf_uint32_t presence_flag = 0;

    icf_cfg_cc_set_transport_params_req_st* set_trans = NULL;
    
    icf_print_api_header(p_msg);
    set_trans = (icf_cfg_cc_set_transport_params_req_st*)p_msg->payload;

	presence_flag = set_trans->bit_mask;


    ICF_PRINT_MESSAGE(("\nbit_mask = %u", presence_flag));

    if (presence_flag & ICF_SET_TRP_REQ_PROXY_ADDR) 
    {            
        ICF_PRINT_MESSAGE(("\ntransport_addr[%u] = (line_id = %u, "
            "action = %u, \taddr_type = %u, ", i, 
            set_trans->proxy_addr.line_id, 
            set_trans->proxy_addr.action,
            set_trans->proxy_addr.transport_addr.addr.addr_type));
        
        addr_type = set_trans->proxy_addr.transport_addr.addr.addr_type;
        
        switch (addr_type) {
        case ICF_TRANSPORT_ADDRESS_DOMAIN_NAME:
            ICF_PRINT_MESSAGE(("%s, ", 
                set_trans->proxy_addr.transport_addr.addr.addr.domain.str));
            break;
            
        case ICF_TRANSPORT_ADDRESS_IPV4_ADDR:
            ICF_PRINT_MESSAGE(("%u.%u.%u.%u, ", 
                set_trans->proxy_addr.transport_addr.addr.addr.ipv4_addr.octet_1, 
                set_trans->proxy_addr.transport_addr.addr.addr.ipv4_addr.octet_2, 
                set_trans->proxy_addr.transport_addr.addr.addr.ipv4_addr.octet_3, 
                set_trans->proxy_addr.transport_addr.addr.addr.ipv4_addr.octet_4));
            break;
            
        }
        
        ICF_PRINT_MESSAGE(("port_num = %u, mode = %u)",
            set_trans->proxy_addr.transport_addr.port_num,
            set_trans->proxy_addr.transport_mode));
    }
    
    if (presence_flag & ICF_CFG_CC_SET_TRP_PRM_SLF_TRANSPORT_ADDRESS) 
    {        
        ICF_PRINT_MESSAGE(("\nself_transport_address = (addr_type = %u, ", 
                  set_trans->self_transport_address.addr_type));
        
        addr_type = set_trans->self_transport_address.addr_type;
        
        switch (addr_type) {
        case ICF_TRANSPORT_ADDRESS_DOMAIN_NAME:
            ICF_PRINT_MESSAGE(("%s, ", 
                      set_trans->self_transport_address.addr.domain.str));
            break;
            
        case ICF_TRANSPORT_ADDRESS_IPV4_ADDR:
            ICF_PRINT_MESSAGE(("%u.%u.%u.%u ", 
                      set_trans->self_transport_address.
                      addr.ipv4_addr.octet_1, 
                      set_trans->self_transport_address.
                      addr.ipv4_addr.octet_2, 
                      set_trans->self_transport_address.
                      addr.ipv4_addr.octet_3, 
                      set_trans->self_transport_address.
                      addr.ipv4_addr.octet_4)); 
            
            break;
        }
    }

    if (presence_flag & ICF_CFG_CC_SET_TRP_PRM_SLF_PORT) 
        ICF_PRINT_MESSAGE(("\nself_port = %u", set_trans->self_port));

    if (presence_flag & ICF_CFG_CC_SET_TRP_PRM_SLF_MODE) 
        ICF_PRINT_MESSAGE((", self_mode = %u)",
                  set_trans->self_mode));

   

    return ICF_SUCCESS;
}

icf_return_t icf_print_set_call_params(icf_msg_st* p_msg)
{

    
    icf_uint32_t i = 0;

    icf_cfg_cc_set_call_params_req_st* p_set_call = NULL;
    
    p_set_call = (icf_cfg_cc_set_call_params_req_st*)p_msg->payload;

    icf_print_api_header(p_msg);


   
    ICF_PRINT_MESSAGE(("bit_mask = %u", p_set_call->bit_mask));

    ICF_PRINT_MESSAGE(("\ndefault_settings = %u, qos_mark = %u, ",
              p_set_call->default_settings, p_set_call->qos_mark));
    
    ICF_PRINT_MESSAGE(("\nsetup_timer = %u, alerting_timer = %u, ", 
              p_set_call->setup_timer, p_set_call->alerting_timer));
    
    ICF_PRINT_MESSAGE(("\nrelease_timer = %u, ", 
              p_set_call->release_timer));

    ICF_PRINT_MESSAGE(("\nmodify_media_timer = %u, registration_resp_timer = %u, ",
              p_set_call->modify_media_timer, 
              p_set_call->registration_resp_timer));
    
    ICF_PRINT_MESSAGE(("\nregistration_retry_timer = %u",
              p_set_call->registration_retry_timer));

    if (p_set_call->bit_mask & ICF_SET_CALL_PRM_INFO_TIMER)
    {
    	ICF_PRINT_MESSAGE(("\ninfo_timer = %u",
              p_set_call->info_timer));
    }

    if (p_set_call->bit_mask & ICF_SET_CALL_PRM_OPTIONS_TIMER)
    {
    	ICF_PRINT_MESSAGE(("\noptions_timer = %u",
              p_set_call->options_timer));
    }

    if(p_set_call->bit_mask & ICF_SET_CALL_PRM_PHONE_INFO)
    {
    	ICF_PRINT_MESSAGE(("\nphone_info = "));
              
	for(i=0;i<p_set_call->phone_info.str_len;i++)
	{
		ICF_PRINT_MESSAGE(("%c",p_set_call->phone_info.str[i]));
	}
    }

#ifdef ICF_SESSION_TIMER
    if(p_set_call->bit_mask & ICF_SET_CALL_PRM_SESSION_TIMER_INTERVAL)
    {
    	ICF_PRINT_MESSAGE(("\nsession_timer = %u",
              p_set_call->session_timer));
    }

    if(p_set_call->bit_mask & ICF_SET_CALL_PRM_MIN_SESSION_TIMER_INTERVAL)
    {
    	ICF_PRINT_MESSAGE(("\nmin_se = %u",
              p_set_call->min_se));
    }
#endif
   /* if(ICF_TRUE==p_set_call->multiple_answer_enable)
	ICF_PRINT_MESSAGE(("\nmult_answer = true"));
	else
	ICF_PRINT_MESSAGE(("\nmult_answer = false"));*/

 
	/* Fix for CSR 1-6727350: SPR 19316 */
    if (p_set_call->bit_mask & ICF_SET_CALL_PRM_APP_CONNECT_REQD_FOR_REPLACES)
    {
    	ICF_PRINT_MESSAGE(("\napp_connect_reqd_for_replaces = %d", 
			p_set_call->app_connect_reqd_for_replaces));
    }


    return ICF_SUCCESS;

}

icf_return_t icf_print_set_line_param_req(icf_msg_st* p_msg)
{
    icf_uint32_t i = 0,j = 0;
    icf_list_st *p_codec_info = NULL;
    icf_list_st *p_encoded_att = NULL;

    icf_set_line_param_req_st* p_set_line = NULL;
    
    p_set_line = (icf_set_line_param_req_st*)p_msg->payload;

    icf_print_api_header(p_msg);


    ICF_PRINT_MESSAGE(("bit_mask = %u", p_set_line->bitmask));

    ICF_PRINT_MESSAGE(("\nline id = %u, ",
              p_set_line->line_id));
    
	if (p_set_line->bitmask & ICF_SET_LINE_PRM_SUBS_HS_TIMER)
	{
	    ICF_PRINT_MESSAGE(("subscribe head start timer = %u, ", 
              p_set_line->subscribe_head_start_timer));
	}

	if (p_set_line->bitmask & ICF_SET_LINE_PRM_T1_TIMER)
	{
	    ICF_PRINT_MESSAGE(("SIP TImer T1 = %u, ", 
              p_set_line->sip_timer_t1));
	}

	if (p_set_line->bitmask & ICF_SET_LINE_PRM_T2_TIMER)
	{
	    ICF_PRINT_MESSAGE(("SIP TImer T2 = %u, ", 
              p_set_line->sip_timer_t2));
	}

    
	if (p_set_line->bitmask & ICF_SET_LINE_PRM_INV_TIMER)
	{
	    ICF_PRINT_MESSAGE(("SIP TImer B (invite) = %u, ", 
              p_set_line->sip_timer_invite));
	}
	
	if (p_set_line->bitmask & ICF_SET_LINE_PRM_NON_INV_TIMER)
	{

    	ICF_PRINT_MESSAGE(( 
              "\nSIP TImer F = %u ",
              p_set_line->sip_timer_noninvite 
              ));
    }
    if (p_set_line->bitmask & ICF_SET_LINE_SIP_DSCP)
    {
      ICF_PRINT_MESSAGE((
                "\nSIP DSCP = %u", p_set_line->sip_dscp));
    }
    if (p_set_line->bitmask & ICF_SET_LINE_RTP_DSCP)
    {
      ICF_PRINT_MESSAGE((
                "\nRTP DSCP = %u", p_set_line->rtp_dscp));
    }

#ifdef ICF_SESSION_TIMER
	if (p_set_line->bitmask & ICF_SET_LINE_SESSION_TIMER)
	{

    	ICF_PRINT_MESSAGE(( 
              "\nsession timer = %u ",
              p_set_line->session_timer 
              ));
    	}
	

	if (p_set_line->bitmask & ICF_SET_LINE_MIN_SE)
	{

    	ICF_PRINT_MESSAGE(( 
              "\nmin se = %u ",
              p_set_line->min_se
              ));
    	}
	if (p_set_line->bitmask & ICF_SET_LINE_SESSION_TIMER_REFRESHER)
	{

    	ICF_PRINT_MESSAGE(( 
              "\nsession timer refresher = %u ",
              p_set_line->session_timer_refresher 
              ));
    	}
#endif

    if ( p_set_line->bitmask & ICF_SET_LINE_AUDIO_CODEC)
    {
        p_codec_info = p_set_line->p_codec_attributes_audio;
	    i = 1;
        ICF_PRINT_MESSAGE(("\nAUDIO CODECS"));
		while(p_codec_info != ICF_NULL)
		{
			ICF_PRINT_MESSAGE(("\nCodec %u",i));
			ICF_PRINT_MESSAGE(("\nCodec Num = %u",
				((icf_codec_attrib_st*)p_codec_info->p_data)->codec_num));
			ICF_PRINT_MESSAGE(("\nCodec Name = "));
			for(j=0;j<(((icf_codec_attrib_st*)p_codec_info->p_data)->codec_name).str_len;j++)
			{
				ICF_PRINT_MESSAGE(("%c",
						((icf_codec_attrib_st*)p_codec_info->p_data)->codec_name.str[j]));
			}

		  icf_print_audio_codec_param((icf_uint8_t*)&((icf_codec_attrib_st*)p_codec_info->p_data)->codec_params);			  
		
			ICF_PRINT_MESSAGE(("\nList of encoded attributes for codec"));

			p_encoded_att = ((icf_codec_attrib_st*)p_codec_info->p_data)->p_encoded_attributes;
			while(p_encoded_att != ICF_NULL)
			{
				ICF_PRINT_MESSAGE((" "));
				for(i=0;i<((icf_string_st*)(p_encoded_att->p_data))->str_len;i++)
				{
					ICF_PRINT_MESSAGE(("%c",
							((icf_string_st*)p_encoded_att->p_data)->str[i]));
				}

				ICF_PRINT_MESSAGE(("\n"));
				p_encoded_att = p_encoded_att->p_next;
			}

			p_codec_info = p_codec_info->p_next;
			i++;
		}

     }
     if ( p_set_line->bitmask & ICF_SET_LINE_VIDEO_CODEC)
     {
        p_codec_info = p_set_line->p_codec_attributes_video;
	    i = 1;
        ICF_PRINT_MESSAGE(("\nVIDEO CODECS"));
		while(p_codec_info != ICF_NULL)
		{
			ICF_PRINT_MESSAGE(("\nCodec %u",i));
			ICF_PRINT_MESSAGE(("\nCodec Num = %u",
				((icf_codec_attrib_st*)p_codec_info->p_data)->codec_num));
			ICF_PRINT_MESSAGE(("\nCodec Name = "));
			for(j=0;j<(((icf_codec_attrib_st*)p_codec_info->p_data)->codec_name).str_len;j++)
			{
				ICF_PRINT_MESSAGE(("%c",
						((icf_codec_attrib_st*)p_codec_info->p_data)->codec_name.str[j]));
			}

		  icf_print_audio_codec_param((icf_uint8_t*)&((icf_codec_attrib_st*)p_codec_info->p_data)->codec_params);			  
		
			ICF_PRINT_MESSAGE(("\nList of encoded attributes for codec"));

			p_encoded_att = ((icf_codec_attrib_st*)p_codec_info->p_data)->p_encoded_attributes;
			while(p_encoded_att != ICF_NULL)
			{
				ICF_PRINT_MESSAGE((" "));
				for(i=0;i<((icf_string_st*)(p_encoded_att->p_data))->str_len;i++)
				{
					ICF_PRINT_MESSAGE(("%c",
							((icf_string_st*)p_encoded_att->p_data)->str[i]));
				}

				ICF_PRINT_MESSAGE(("\n"));
				p_encoded_att = p_encoded_att->p_next;
			}

			p_codec_info = p_codec_info->p_next;
			i++;
        }	
      
     }  

	/* Fix for CSR 1-8047728: SPR 20246 */
	if (p_set_line->bitmask & ICF_SET_LINE_REMOVE_ROUTE_HDR)
	{
	    ICF_PRINT_MESSAGE(("\nremove_route_hdr = %d\n",
			p_set_line->remove_route_hdr));
	} 


 

    return ICF_SUCCESS;

}

icf_return_t icf_print_set_self_id_params(icf_msg_st* p_msg)
{


   
    icf_cfg_cc_set_self_id_params_req_st* p_set_self = NULL;
    
    
    icf_print_api_header(p_msg);
    p_set_self = (icf_cfg_cc_set_self_id_params_req_st*)p_msg->payload;
    
    ICF_PRINT_MESSAGE(( 
        "\npresence_flag = %u", 
        p_set_self->bit_mask));

    ICF_PRINT_MESSAGE(( 
        "\nline_id = %u, action = %u,",        
        p_set_self->line_id,
        p_set_self->action));

    ICF_PRINT_MESSAGE(( 
        "\nself_addr_type = %u, self_addr_val = %s,",
        p_set_self->self_addr.addr_type,
        p_set_self->self_addr.addr_val.str));

    
    ICF_PRINT_MESSAGE(( 
        "\nself_name = %s)",                   
        p_set_self->self_name.str));
    
    ICF_PRINT_MESSAGE(( 
        "\ndefault_settings = %u", 
        p_set_self->default_settings));

    ICF_PRINT_MESSAGE(( 
        "\nauthentication_key = %s", 
        p_set_self->authentication_key.str));
    
    ICF_PRINT_MESSAGE(( 
        "\nauthentication_password = %s", 
        p_set_self->authentication_password.str));

	ICF_PRINT_MESSAGE(( 
        "\nconfigured_realm = %s", 
        p_set_self->configured_realm.str));
   

    return ICF_SUCCESS;

}









icf_return_t icf_print_set_reg_params(icf_msg_st* p_msg)
{


       icf_uint8_t addr_type = 0;
 

    icf_cfg_cc_set_registration_params_req_st* p_set_reg = NULL;


    icf_print_api_header(p_msg);
    p_set_reg = (icf_cfg_cc_set_registration_params_req_st*)p_msg->payload;


    ICF_PRINT_MESSAGE(("\n"));
    ICF_PRINT_MESSAGE(( 
        "\n line_id = %u,", p_set_reg->line_id));

    ICF_PRINT_MESSAGE(( 
        "\n action = %u,", p_set_reg->action));

    ICF_PRINT_MESSAGE(( 
        "\n registrar_addr = addr_type = %u, ",
        p_set_reg->registrar_addr.addr.addr_type));
    
    
    addr_type = p_set_reg->registrar_addr.addr.addr_type;
    
    
    switch (addr_type) {
    case ICF_TRANSPORT_ADDRESS_DOMAIN_NAME:
        ICF_PRINT_MESSAGE(("%s, ", 
            p_set_reg->registrar_addr.addr.addr.domain.str));
        break;
        
    case ICF_TRANSPORT_ADDRESS_IPV4_ADDR:
        ICF_PRINT_MESSAGE(("%u.%u.%u.%u, ", 
            p_set_reg->registrar_addr.addr.addr.ipv4_addr.octet_1, 
            p_set_reg->registrar_addr.addr.addr.ipv4_addr.octet_2, 
            p_set_reg->registrar_addr.addr.addr.ipv4_addr.octet_3, 
            p_set_reg->registrar_addr.addr.addr.ipv4_addr.octet_4));
        break;
        
    }
    
    ICF_PRINT_MESSAGE(("port_num = %u), ",
        p_set_reg->registrar_addr.port_num));
    
    ICF_PRINT_MESSAGE(( 
        "\npresence_flag = %u, ",
        p_set_reg->bit_mask));
        

    ICF_PRINT_MESSAGE(("reg_mode = %u)",
        p_set_reg->reg_mode));
    
    
    ICF_PRINT_MESSAGE(( 
        "\nregistration_duration = %u, ",        
        p_set_reg->registration_duration));
	
	if ( p_set_reg->bit_mask & ICF_SET_REG_HS_DURATION)
	{
	    ICF_PRINT_MESSAGE(( 
    	    "\nregistraion head start duration = %u, ",        
        	p_set_reg->register_head_start_timer));
	}
    
      

    return ICF_SUCCESS;

}

icf_return_t icf_print_set_service_params(icf_msg_st* p_msg)
{
    
    icf_cfg_cc_set_service_params_req_st* p_ser_prm_msg = NULL;

    icf_uint32_t presence_flag = 0,presence_flag2 = 0;
    icf_print_api_header(p_msg);

     
    p_ser_prm_msg = (icf_cfg_cc_set_service_params_req_st*)p_msg->payload;
    
    presence_flag = p_ser_prm_msg->bit_mask;
    
    
    if (presence_flag & ICF_CFG_CC_SET_SER_PRM_SUBSCRIPTION_FLAG)
    {
        ICF_PRINT_MESSAGE((
                "\nsubscription_flag =(line_id = %u , status = %u )",
                  p_ser_prm_msg->service_subscription_flag.line_id,
                  p_ser_prm_msg->service_subscription_flag.status));  
    }

    if (presence_flag & ICF_CFG_CC_SET_SER_PRM_ACTIVATION_FLAG)
    {
        ICF_PRINT_MESSAGE((
                  "\nactivattion_flag = (line_id = %u , status = %u )",
                   p_ser_prm_msg->service_activation_flag.line_id, 
                   p_ser_prm_msg->service_activation_flag.status));  
                
    }
    
    if (presence_flag & ICF_CFG_CC_SET_SER_PRM_CALL_HOLD_OPTION)
    {
        /*ICF_PRINT_MESSAGE((
                  "\ncall_hold_option = %u",  
                    p_ser_prm_msg->call_hold_option));*/
        ICF_PRINT_MESSAGE((
                 "\ncall_hold_option =(hold_option= <%u>,line_id= <%u>",
                 p_ser_prm_msg->call_hold_option.hold_option,
                 p_ser_prm_msg->call_hold_option.line_id));
    }
    
    
    if (presence_flag & ICF_CFG_CC_SET_SER_PRM_MAX_NUM_OF_HOPS)
    {
        ICF_PRINT_MESSAGE((
                  "\nmax_num_of_calls_forwarding_hops = %u",  
                    p_ser_prm_msg->max_num_of_call_forwarding_hops));
    }
    
    
    if (presence_flag & ICF_CFG_CC_SET_SER_PRM_MAX_CALL_WAITING)
    {
        ICF_PRINT_MESSAGE((
                  "\nmax_num_of_calls_in_call_waiting = %u",  
                    p_ser_prm_msg->max_num_of_calls_in_call_waiting));
    }

    
    presence_flag2 = 0;
    
    if (presence_flag & ICF_CFG_CC_SET_SER_PRM_CFU_ADDR)
    {
    
    
        ICF_PRINT_MESSAGE((
             "\naddr_for_cfu = ( line_id = %u ",
               p_ser_prm_msg->addr_for_cfu.line_id)); 
        ICF_PRINT_MESSAGE((
                  "\naddr_val = %s",  
                    p_ser_prm_msg->addr_for_cfu.address.addr_val.str));
        
        ICF_PRINT_MESSAGE((
                  "\naddr_type = %u",    
                    p_ser_prm_msg->addr_for_cfu.address.addr_type));
    
    }    

   
    presence_flag2 = 0;
    
    if (presence_flag & ICF_CFG_CC_SET_SER_PRM_CFB_ADDR)
    {
    
    
        ICF_PRINT_MESSAGE((
             "\naddr_for_cfb = ( line_id = %u ",
               p_ser_prm_msg->addr_for_cfb.line_id)); 
    
        ICF_PRINT_MESSAGE((
                  "\naddr_val = %s",  
                    p_ser_prm_msg->addr_for_cfb.address.addr_val.str));
        
        ICF_PRINT_MESSAGE((
                  "\naddr_type = %u",    
                    p_ser_prm_msg->addr_for_cfb.address.addr_type));
    
    }    

    
    
    presence_flag2 = 0;
    
    if (presence_flag & ICF_CFG_CC_SET_SER_PRM_CFD_ADDR)
    {
    
    
        ICF_PRINT_MESSAGE((
             "\naddr_for_cfd = ( line_id = %u ",
               p_ser_prm_msg->addr_for_cfd.line_id)); 
    
        ICF_PRINT_MESSAGE((
                  "\naddr_val = %s",  
                    p_ser_prm_msg->addr_for_cfd.address.addr_val.str));
        
        ICF_PRINT_MESSAGE((
                  "\naddr_type = %u",    
                    p_ser_prm_msg->addr_for_cfd.address.addr_type));
    
    }    

    
    if ( presence_flag & ICF_CFG_CC_SET_SER_PRM_CHR_TIMER)
    {
    
        ICF_PRINT_MESSAGE((
                 "\ncall_hold_reume_timer = %u",
                  p_ser_prm_msg->call_hold_resume_timer));
    }

    if (presence_flag & ICF_CFG_CC_SET_SER_PRM_REINVITE_RACE_TIMER)
    {
    
         ICF_PRINT_MESSAGE((
              "\nreinvite_race_timer = %u",
                p_ser_prm_msg->reinvite_race_timer));
    }
    
    if (presence_flag & ICF_CFG_CC_SET_SER_PRM_TWC_TIMER)
    {

        ICF_PRINT_MESSAGE((
                  "twc_timer = %u",
                  p_ser_prm_msg->twc_timer));
             
     }

    if (presence_flag & ICF_CFG_CC_SET_SER_PRM_CTU_TIMER)    
    {
            ICF_PRINT_MESSAGE((
                      "\nctu_timer = %u",
                        p_ser_prm_msg->ctu_timer));
          
    }

    if (presence_flag & ICF_CFG_CC_SET_SER_PRM_CTA_TIMER)  
    {
    
            ICF_PRINT_MESSAGE((
                      "\nrnw_timer = %u",
                      p_ser_prm_msg->rnw_timer));
    }

                          
    if (presence_flag & ICF_CFG_CC_SET_SER_PRM_CW_ALERT_TIMER)  
    {
    
            ICF_PRINT_MESSAGE((
                      "\ncw_alert_timer = %u",
                      p_ser_prm_msg->cw_alert_timer));
    }

    if (presence_flag & ICF_CFG_CC_SET_SER_PRM_CND_ALERT_TIMER)  
    {
    
            ICF_PRINT_MESSAGE((
                      "\ncnd_alert_timer = %u",
                      p_ser_prm_msg->cnd_alert_timer));
    }

    if (presence_flag & ICF_CFG_CC_SET_SER_PRM_CW_N_CND_ALERT_TIMER)  
    {
    
            ICF_PRINT_MESSAGE((
                      "\ncw_n_cnd_alert_timer = %u",
                      p_ser_prm_msg->cw_n_cnd_alert_timer));
     }

     if (presence_flag & ICF_CFG_CC_SET_SER_PRM_SUBS_RESP_TIMER)  
     {
    
            ICF_PRINT_MESSAGE((
                      "\nsubscribe_resp_timer = %u",
                     p_ser_prm_msg->subs_resp_timer));
      }

     if (presence_flag & ICF_CFG_CC_SET_SER_PRM_SUBS_RETRY_TIMER)  
     {
    
            ICF_PRINT_MESSAGE((
                      "\nsubscribe_retry_timer = %u",
                      p_ser_prm_msg->subs_retry_timer));
      }

	 if (presence_flag & ICF_SET_SER_PRM_CONF_FAIL_CALL_CLEAR_NOT_REQD)  
	 {

		 ICF_PRINT_MESSAGE((
						 "\nconf_fail_call_clear_not_reqd = %u",
						 p_ser_prm_msg->conf_fail_call_clear_not_reqd));
	 }

    return ICF_SUCCESS;
}







icf_return_t icf_print_incoming_call(icf_msg_st* p_msg)
{

    icf_cc_pa_incoming_call_ind_st* p_incoming_call = NULL;

    icf_list_st *p_list=ICF_NULL;    
    
    icf_uint8_t  header_list_count = 0;
    icf_uint8_t  body_list_count = 0,i=0;
#ifdef ICF_FOR_LOAD_TESTING
	char command_string[ICF_MAX_BUFF_LEN];
#endif
    icf_print_api_header(p_msg);

    p_incoming_call = (icf_cc_pa_incoming_call_ind_st*)p_msg->payload;
    
	ICF_PRINT_MESSAGE(("\nbit_mask = %u", 
              p_incoming_call->bit_mask));
	

    ICF_PRINT_MESSAGE(( 
              "line_id = %u, call_type = %u", 
              p_incoming_call->line_id, p_incoming_call->call_type));


    if (p_incoming_call->bit_mask & ICF_CALL_WAITING_IND_PRESENT)     
        ICF_PRINT_MESSAGE(( 
            "\ncall_waiting_ind = %u",
            p_incoming_call->call_waiting_ind));

    if (p_incoming_call->bit_mask & ICF_CALLING_ADD_ASRTD_IND_PRESENT)     
        ICF_PRINT_MESSAGE(( 
            " ,calling_party_addr_asserted = %u,",
            p_incoming_call->calling_party_addr_asserted));


    if (p_incoming_call->bit_mask & ICF_CALLING_PARTY_ADD_PRESENT)     
        ICF_PRINT_MESSAGE(( 
              "calling_party_addr = (addr_type = %u, %s)" ,
              p_incoming_call->calling_party_addr.addr_type,
              p_incoming_call->calling_party_addr.addr_val.str));

    if (p_incoming_call->bit_mask & ICF_CALLING_PARTY_NAME_PRESENT)     
        ICF_PRINT_MESSAGE(( 
              "\ncalling_party_name = %s",
              p_incoming_call->calling_party_name.str));
             
    if (p_incoming_call->bit_mask & ICF_TRANSF_PARTY_ADD_PRESENT) 
        ICF_PRINT_MESSAGE(( 
            "\ntransfering_party_addr = (addr_type = %u, %s)",
            p_incoming_call->transferring_party_addr.addr_type,
            p_incoming_call->transferring_party_addr.addr_val.str));

    if (p_incoming_call->bit_mask & ICF_TRANSF_PARTY_NAME_PRESENT) 
        ICF_PRINT_MESSAGE(( 
            "\ntransfering_party_name = %s", 
            p_incoming_call->transferring_party_name.str));        

    if (p_incoming_call->bit_mask & ICF_CALLED_PARTY_ADD_PRESENT)     
        ICF_PRINT_MESSAGE(( 
            "\ncalled_party_addr = (addr_type = %u, %s)",
            p_incoming_call->called_party_addr.addr_type,
            p_incoming_call->called_party_addr.addr_val.str));
    
    
/* Header LIST  */
    if (p_incoming_call->bit_mask & ICF_HDR_LIST_PRESENT) {
        header_list_count = (icf_uint8_t)p_incoming_call->header_list.count;
        icf_print_header_list(p_incoming_call->header_list.hdr, header_list_count);
    }

/* Body LIST  */
    if (p_incoming_call->bit_mask & ICF_BODY_LIST_PRESENT) {
        body_list_count = (icf_uint8_t)p_incoming_call->body_list.count;
        icf_print_body_list(p_incoming_call->body_list.body, body_list_count);
    }

/* Stream LIST */
#ifdef ICF_REL_OLD
    if (p_incoming_call->bit_mask & ICF_STREAM_LIST_PRESENT) {
        stream_list_count = p_incoming_call->stream_list.count;
        icf_print_stream_list(p_incoming_call->stream_list.stream, stream_list_count);
    }
#endif



	ICF_PRINT_MESSAGE((
        "\n Accepted Stream List:"));

	i = 1;
        
        p_list=p_incoming_call->p_accepted_stream_list;

	while(p_list!=ICF_NULL)
	{
		ICF_PRINT_MESSAGE(("\nNode %u:",i));

		icf_print_stream_capab(p_list->p_data);
		
		p_list = p_list->p_next;
		
		i++;
	}


	if(p_incoming_call->bit_mask & ICF_REJECTED_MEDIA_STREAM_PRESENT)
	{
		ICF_PRINT_MESSAGE(("\n rejected stream count %u:",p_incoming_call->count));	

		for(i=0;i<p_incoming_call->count;i++)
		{
			ICF_PRINT_MESSAGE(("\n stream type %u:",p_incoming_call->deleted_stream_list[i].stream_type));
			ICF_PRINT_MESSAGE(("\n stream id %u:",p_incoming_call->deleted_stream_list[i].stream_id));
		}
	}



#ifdef ICF_FOR_LOAD_TESTING

	app_global_data.call_id = p_msg->hdr.call_id;
    strcpy(command_string,ICF_DEFAULT_USER_ALERTED_MSG);
    icf_command_handler(command_string);
    strcpy(command_string,ICF_DEFAULT_CONNECT_REQ_MSG);
    icf_command_handler(command_string);

#endif



    return ICF_SUCCESS;

}






icf_return_t icf_print_create_call(icf_msg_st* p_msg)
{



    icf_pa_cc_create_call_req_st* p_create_call = NULL;

    icf_uint8_t  header_list_count = 0, tag_list_count = 0,
                    body_list_count = 0,i=0;
	icf_list_st	*p_pref_media_profile;

    
    icf_print_api_header(p_msg);
    p_create_call = (icf_pa_cc_create_call_req_st*)p_msg->payload;
     
    ICF_PRINT_MESSAGE(("\nbit_mask = %u", 
              p_create_call->bit_mask));
     
 
    ICF_PRINT_MESSAGE(( 
              "called_party_addr = (addr_type = %u, %s)"
              "\ncalled_party_name = %s",
              p_create_call->called_party_addr.addr_type,
              p_create_call->called_party_addr.addr_val.str,
              p_create_call->called_party_name.str));


    ICF_PRINT_MESSAGE(( 
              "\ntoggle_presentation_indicator = %u, qos_mark = %u",
              p_create_call->toggle_presentation_indicator, 
              p_create_call->qos_mark));
/* TBD */
#ifdef ICF_REL_OLD 
    ICF_PRINT_MESSAGE((
        "\ncall_type = %u, pref_identity.addr_val.str = %s"
        "\npref_identity.addr_type = %u", p_create_call->call_type, 
        p_create_call->pref_identity.addr_val.str, p_create_call->pref_identity.addr_type));
#endif

/* Header LIST  */    if (p_create_call->bit_mask & ICF_HDR_LIST_PRESENT) {
        header_list_count = (icf_uint8_t)p_create_call->header_list.count;
        icf_print_header_list(p_create_call->header_list.hdr, header_list_count);
    }


    if (p_create_call->bit_mask & ICF_TAG_LIST_PRESENT) {
        tag_list_count = (icf_uint8_t)p_create_call->tag_list.count;
        icf_print_tag_list(p_create_call->tag_list.tag, tag_list_count);
    }
    
    
        if (p_create_call->bit_mask & ICF_BODY_LIST_PRESENT) {
        body_list_count = (icf_uint8_t)p_create_call->body_list.count;
        icf_print_body_list(p_create_call->body_list.body, body_list_count);
    }
    
   #ifdef ICF_REL_OLD
    if (p_create_call->bit_mask & ICF_STREAM_LIST_PRESENT) {
        stream_list_count = p_create_call->stream_list.count;
        icf_print_stream_list(p_create_call->stream_list.stream, stream_list_count);
    }
#endif




	ICF_PRINT_MESSAGE(("\n Media Profile List:"));

	i = 1;

	p_pref_media_profile = p_create_call->p_pref_media_profile;

	while(p_pref_media_profile!=ICF_NULL)
	{
		ICF_PRINT_MESSAGE(("\nNode %u:",i));

		icf_print_stream_capab(p_pref_media_profile->p_data);
		
		p_pref_media_profile = p_pref_media_profile->p_next;
		
		i++;
	}





	

    ICF_PRINT_MESSAGE(( 

              "calling_party_addr = (addr_type = %u, %s)"

              "\ncalling_party_name = %s",

              p_create_call->calling_party_address.addr_type,

              p_create_call->calling_party_address.addr_val.str,

              p_create_call->calling_party_name.str));



    ICF_PRINT_MESSAGE(( 
              "\nprivacy_hdr_all_req = %u",
              p_create_call->privacy_hdr_all_req));

	ICF_PRINT_MESSAGE(( 
              "\nres_reservation_reqd = %u",
              p_create_call->resource_reservation_reqd));

    return ICF_SUCCESS;
}







icf_return_t icf_print_alert_user(icf_msg_st* p_msg)
{
    

    icf_cc_pa_alert_user_ind_st* p_alert_user = NULL;

    icf_list_st *p_list=ICF_NULL;
    
    icf_uint8_t  header_list_count = 0;  
    icf_uint8_t  body_list_count = 0,i=0;
    icf_print_api_header(p_msg);
    p_alert_user = (icf_cc_pa_alert_user_ind_st*)p_msg->payload;


	ICF_PRINT_MESSAGE(("\nbit_mask = %u", 
              p_alert_user->bit_mask));
    
    ICF_PRINT_MESSAGE(( 
              "\ninband_info_status = %u", 
              p_alert_user->inband_info_status));

    
/* Header LIST  */
    if (p_alert_user->bit_mask & ICF_HDR_LIST_PRESENT) {
        header_list_count = (icf_uint8_t)p_alert_user->header_list.count;
        icf_print_header_list(p_alert_user->header_list.hdr, header_list_count);
    }
    
       
    /* Body LIST */
    if (p_alert_user->bit_mask & ICF_BODY_LIST_PRESENT) {
        body_list_count = (icf_uint8_t)p_alert_user->body_list.count;
        icf_print_body_list(p_alert_user->body_list.body, body_list_count);
    }
    
/* Stream LIST*/
#ifdef ICF_REL_OLD
    if (p_alert_user->bit_mask & ICF_STREAM_LIST_PRESENT) {
        stream_list_count = p_alert_user->stream_list.count;
        icf_print_stream_list(p_alert_user->stream_list.stream, stream_list_count);
    }
#endif


	ICF_PRINT_MESSAGE((
        "\n Accepted Stream List:"));

	i = 1;
	p_list=p_alert_user->p_accepted_stream_list;

	while(p_list!=ICF_NULL)
	{
		ICF_PRINT_MESSAGE(("\nNode %u:",i));

		icf_print_stream_capab(p_list->p_data);
		
		p_list = p_list->p_next;
		
		i++;
	}


	if(p_alert_user->bit_mask & ICF_REM_USER_ALRTED_DELETED_STREAM_PRESENT)
	{
		ICF_PRINT_MESSAGE(("\n Deleted stream count %u:",p_alert_user->count));	

		for(i=0;i<p_alert_user->count;i++)
		{
			ICF_PRINT_MESSAGE(("\n stream type %u:",p_alert_user->deleted_media_streams[i].stream_type));
			ICF_PRINT_MESSAGE(("\n stream id %u:",p_alert_user->deleted_media_streams[i].stream_id));
		}
	}



    return ICF_SUCCESS;

}







icf_return_t icf_print_user_alerted(icf_msg_st* p_msg)
{
    
    icf_pa_cc_user_alerted_req_st* p_alert_ind = NULL;
    
    icf_uint8_t  header_list_count = 0, tag_list_count = 0,
                    body_list_count = 0,i=0;	
    icf_list_st *p_stream_list;

    icf_print_api_header(p_msg);
    p_alert_ind = (icf_pa_cc_user_alerted_req_st*)p_msg->payload;
    
    ICF_PRINT_MESSAGE(( 
              "\ninband_info_status = %u",
              p_alert_ind->inband_info_status));
    
    ICF_PRINT_MESSAGE(( 
              "\nearly_media_reqd = %u",
              p_alert_ind->early_media_reqd));
    


    ICF_PRINT_MESSAGE(( 
        "\nprivacy_ind = %u", p_alert_ind->privacy_ind));

	if (p_alert_ind->bit_mask & ICF_USER_ALERTED_ALERT_TIMER_PRESENT) {      
		ICF_PRINT_MESSAGE(( 
        "\nAlert timer duration = %u", p_alert_ind->alert_timer_duration));
    }

/* Header LIST  */    if (p_alert_ind->bit_mask & ICF_HDR_LIST_PRESENT) {
        header_list_count =(icf_uint8_t) p_alert_ind->header_list.count;
        icf_print_header_list(p_alert_ind->header_list.hdr, header_list_count);
    }
    
    
    /* Tag LIST  */    if (p_alert_ind->bit_mask & ICF_TAG_LIST_PRESENT) {
        tag_list_count = (icf_uint8_t)p_alert_ind->tag_list.count;
        icf_print_tag_list(p_alert_ind->tag_list.tag, tag_list_count);
    }
    
    
    /* Body LIST  */
    if (p_alert_ind->bit_mask & ICF_BODY_LIST_PRESENT) {
        body_list_count = (icf_uint8_t)p_alert_ind->body_list.count;
        icf_print_body_list(p_alert_ind->body_list.body, body_list_count);
    }
    
    /* Stream LIST  */
#ifdef ICF_REL_OLD
    if (p_alert_ind->bit_mask & ICF_STREAM_LIST_PRESENT) {
        stream_list_count = p_alert_ind->stream_list.count;
        icf_print_stream_list(p_alert_ind->stream_list.stream, stream_list_count);
    }
#endif



	if(p_alert_ind->bit_mask & ICF_STREAM_LIST_PRESENT)
	{

		ICF_PRINT_MESSAGE((
	        "\nMedia Profile List:"));
		p_stream_list= p_alert_ind->p_stream_list;
		i = 1;
		while(p_stream_list!=ICF_NULL)
		{
			ICF_PRINT_MESSAGE(("\nNode %u:",i));
			icf_print_stream_capab(p_stream_list->p_data);
			p_stream_list = p_stream_list->p_next;
			i++;
		}
	}

	if(p_alert_ind->bit_mask & ICF_USER_ALERTED_DELETED_STREAM_PRESENT)
	{
		ICF_PRINT_MESSAGE(("\n Deleted stream count: %u",p_alert_ind->count));	

		for(i=0;i<p_alert_ind->count;i++)
		{
			ICF_PRINT_MESSAGE(("\n stream type: %u",p_alert_ind->deleted_media_streams[i].stream_type));
			ICF_PRINT_MESSAGE(("\n stream id: %u",p_alert_ind->deleted_media_streams[i].stream_id));
		}
	}






    return ICF_SUCCESS;
}





icf_return_t icf_print_term_call_req(icf_msg_st* p_msg)
{
    

    icf_pa_cc_terminate_call_req_st* p_terminate_call_req = NULL;
    
    icf_print_api_header(p_msg);
    p_terminate_call_req = (icf_pa_cc_terminate_call_req_st*)p_msg->payload;

 
    
    ICF_PRINT_MESSAGE(( 
              "\ncall_clear_reason = %u"
              "\npresence_flag = %u, line_id = %u",
              p_terminate_call_req->call_clear_reason,
              p_terminate_call_req->bit_mask,
              p_terminate_call_req->line_id));

	
    return ICF_SUCCESS;
}






icf_return_t icf_print_term_call_ind(icf_msg_st* p_msg)
{
    

    icf_cc_pa_terminate_call_ind_st* p_terminate_call = NULL;
#ifdef ICF_FOR_LOAD_TESTING
	char command_string[ICF_MAX_BUFF_LEN];
#endif

    icf_print_api_header(p_msg);
    p_terminate_call = (icf_cc_pa_terminate_call_ind_st*)p_msg->payload;
    
    ICF_PRINT_MESSAGE(( 
              "\ncall_clear_reason = %u",
              p_terminate_call->call_clear_reason));

    ICF_PRINT_MESSAGE(( 
              "\nadditional_info.str = %s", p_terminate_call->additional_info.str));

    ICF_PRINT_MESSAGE((
              "\nResponse code = %u", p_terminate_call->response_code));



#ifdef ICF_FOR_LOAD_TESTING

	app_global_data.call_id = p_msg->hdr.call_id;
    strcpy(command_string,ICF_DEFAULT_TERM_CALL_CFM_MSG);
    icf_command_handler(command_string);
    t_calls++;
    ICF_PRINT_MESSAGE(("\nTerminated_Calls:%u",t_calls));

#endif

    return ICF_SUCCESS;
}



icf_return_t icf_print_get_codec_list_resp(icf_msg_st* p_msg)
{


    icf_get_codec_list_resp_st* p_gcl_resp_msg = NULL;

    icf_uint8_t i = 0;    
    icf_list_st *p_reserved_stream_list;
      
    icf_print_api_header(p_msg);
               
    p_gcl_resp_msg = (icf_get_codec_list_resp_st*)p_msg->payload;

#ifdef ICF_REL_OLD
    ICF_PRINT_MESSAGE(("result = %u", 
              p_gcl_resp_msg->result));
/* TBD */

    num_elem = p_gcl_resp_msg->local_audio_codec_list.num_of_codecs;

    ICF_PRINT_MESSAGE(( 
              "\nlocal_audio_codec_list.num_of_codecs = %u", num_elem));

        ICF_PRINT_MESSAGE(("\nlocal_audio_codec_list"));
/* TBD */

        icf_print_codec_list(&(p_gcl_resp_msg->local_audio_codec_list));
    presence_flag = p_gcl_resp_msg->bit_mask;

    if(presence_flag & ICF_CODEC_LIST_RESP_VIDEO_CODEC_LIST) {

        num_elem = p_gcl_resp_msg->local_video_codec_list.num_of_codecs;

        ICF_PRINT_MESSAGE(( 
                  "\nlocal_video_codec_list.num_of_codecs = %u", num_elem));

        ICF_PRINT_MESSAGE(("\nlocal_video_codec_list"));
        icf_print_codec_list(&(p_gcl_resp_msg->local_video_codec_list));
    }
#endif
        
   

	ICF_PRINT_MESSAGE(("\n result = %u", p_gcl_resp_msg->result));

	ICF_PRINT_MESSAGE(("\n bitmask = %u", p_gcl_resp_msg->bit_mask));

	ICF_PRINT_MESSAGE(("\n error = %u", p_gcl_resp_msg->error_cause));
       
        icf_print_error_details(p_gcl_resp_msg->error_cause);

	ICF_PRINT_MESSAGE(("\n Reserved Stream List:"));

	i = 1;

	p_reserved_stream_list = p_gcl_resp_msg->p_reserved_stream_list;

	while(p_reserved_stream_list!=ICF_NULL)
	{
		ICF_PRINT_MESSAGE(("\nNode %u:",i));

		icf_print_stream_capab(p_reserved_stream_list->p_data);
		
		p_reserved_stream_list = p_reserved_stream_list->p_next;
		
		i++;
	}

	if(p_gcl_resp_msg->bit_mask & ICF_CODEC_LIST_REJECTED_STREAM_LIST)
	{
		ICF_PRINT_MESSAGE(("\n count = %u", p_gcl_resp_msg->rejected_stream_list.count));

		for(i=0;i<p_gcl_resp_msg->rejected_stream_list.count;i++)
		{
			ICF_PRINT_MESSAGE(("\n stream_id = %u", p_gcl_resp_msg->rejected_stream_list.stream_id[i]));

		}
	}





    return ICF_SUCCESS;
}




icf_return_t icf_print_get_codec_list_req(icf_msg_st *p_msg)
{

    icf_get_codec_list_req_st* p_gcl_req_msg = NULL;
    
    icf_uint8_t i = 0;
        
    icf_list_st	*p_stream_capab_list;  

#ifdef ICF_FOR_LOAD_TESTING
	char command_string[ICF_MAX_BUFF_LEN];
#endif


    p_gcl_req_msg = (icf_get_codec_list_req_st*)p_msg->payload;

    icf_print_api_header(p_msg);



    ICF_PRINT_MESSAGE(( 
            "line_id = %u", p_gcl_req_msg->line_id));

#ifdef ICF_REL_OLD
    presence_flag = p_gcl_req_msg->bit_mask;

    ICF_PRINT_MESSAGE(( 
            "\npresence_flag = %u", presence_flag));
    
        

  #endif


	ICF_PRINT_MESSAGE((
        "\nMedia Profile List:"));

	i = 1;

	p_stream_capab_list = p_gcl_req_msg->p_stream_capab_list;

	while(p_stream_capab_list!=ICF_NULL)
	{
		ICF_PRINT_MESSAGE(("\nNode %u:",i));

		icf_print_stream_capab(p_stream_capab_list->p_data);
		
		p_stream_capab_list = p_stream_capab_list->p_next;
		
		i++;
	}

	if (ICF_LOCAL_STREAM_CAPAB_INFO_PRESENT ==(ICF_LOCAL_STREAM_CAPAB_INFO_PRESENT & p_gcl_req_msg->bit_mask))
	{
		ICF_PRINT_MESSAGE((
			"\nLocal Media Profile List:\n"));
		
		i = 1;
		
		p_stream_capab_list = p_gcl_req_msg->p_local_stream_capab_list;
		
		while(p_stream_capab_list!=ICF_NULL)
		{
			ICF_PRINT_MESSAGE(("\nNode %u:",i));
			
			icf_print_stream_capab(p_stream_capab_list->p_data);
			
			p_stream_capab_list = p_stream_capab_list->p_next;
			
			i++;
		}
	}


#ifdef ICF_FOR_LOAD_TESTING

	strcpy(command_string,ICF_DEFAULT_GCL_RSP_MSG);
    app_global_data.call_id = p_msg->hdr.call_id;
    icf_command_handler(command_string);

#endif

    return ICF_SUCCESS;


}

icf_return_t icf_print_codec_commit_req(icf_msg_st *p_msg)
{


   
    icf_codec_commit_req_st* p_gcc_req_msg = NULL;
	icf_list_st *p_list;
    
    	icf_uint32_t i=0;
#ifdef ICF_FOR_LOAD_TESTING
	char command_string[ICF_MAX_BUFF_LEN];
#endif


    icf_print_api_header(p_msg);
  
    p_gcc_req_msg = (icf_codec_commit_req_st*)p_msg->payload;
/* TBD */
#ifdef ICF_REL_OLD
    presence_flag = p_gcc_req_msg->bit_mask;
    
    ICF_PRINT_MESSAGE(("\npresence_flag = %u", 
              presence_flag));

    
    ICF_PRINT_MESSAGE(( 
        "\n\tselected_audio_codec.codecnum = %u"
        "\n\tselected_audio_codec.encoding_rate = %u"
        "\n\tselected_audio_codec.channels = %u"
        "\n\tselected_audio_codec.encoding_name = %s",
        p_gcc_req_msg->selected_audio_codec.codecnum, 
        p_gcc_req_msg->selected_audio_codec.encoding_rate,
        p_gcc_req_msg->selected_audio_codec.channels, 
        p_gcc_req_msg->selected_audio_codec.encoding_name));
      
    
    if (presence_flag & ICF_CODEC_COMMIT_VIDEO_CODEC) {
        ICF_PRINT_MESSAGE(( 
            "\n\tselected_video_codec.codecnum = %u"
            "\n\tselected_video_codec.encoding_rate = %u"
            "\n\tselected_video_codec.channels = %u"
            "\n\tselected_video_codec.encoding_name = %s",
            p_gcc_req_msg->selected_video_codec.codecnum, 
            p_gcc_req_msg->selected_video_codec.encoding_rate,
            p_gcc_req_msg->selected_video_codec.channels, 
            p_gcc_req_msg->selected_video_codec.encoding_name));
           }
    
    

#ifdef ICF_AUTO_RESPOND
    icf_command_handler("SEND codec_commit_resp codec_commit_resp.mg"));
#endif
#endif /* #ifdef ICF_REL_OLD */










ICF_PRINT_MESSAGE((
        "\nMedia Profile List:"));
	
	i = 1;
	 
	p_list = p_gcc_req_msg->p_stream_commit_list;

	while(p_list!=ICF_NULL)
	{
		ICF_PRINT_MESSAGE(("\nNode %u:",i));
	
		icf_print_stream_capab(p_list->p_data);
		
		p_list = p_list->p_next;
		
		i++;
	}



#ifdef ICF_FOR_LOAD_TESTING

	app_global_data.call_id = p_msg->hdr.call_id;
    strcpy(command_string,ICF_DEFAULT_CODEC_COMMIT_RSP_MSG);
    icf_command_handler(command_string);

#endif

    return ICF_SUCCESS;

}


/******************************************************************************
 *
 * FUNCTION:        icf_print_open_channel_req
 *
 * DESCRIPTION:     This function is used to print open_channel_req
 *
 *****************************************************************************/

icf_return_t icf_print_open_channel_req(icf_msg_st* p_msg)
{
    icf_uint8_t i = 0;

    icf_open_media_channel_req_st* p_open_channel_req = ICF_NULL;

	icf_list_st* p_channel_list = ICF_NULL;

    p_open_channel_req = (icf_open_media_channel_req_st*)p_msg->payload;

    icf_print_api_header(p_msg);
	
	ICF_PRINT_MESSAGE(("\nChannel List:"));

	i = 1;

	p_channel_list = p_open_channel_req->p_channel_list;

	while(p_channel_list!=ICF_NULL)
	{
		ICF_PRINT_MESSAGE(("\nNode %u:",i));

		ICF_PRINT_MESSAGE(("\nStream Type:%u",
					((icf_channel_st*)(p_channel_list->p_data))->stream_type));

		ICF_PRINT_MESSAGE(("\nStream Id:%u",
					((icf_channel_st*)(p_channel_list->p_data))->stream_id));

		p_channel_list = p_channel_list->p_next;
		
		i++;
	}


	return ICF_SUCCESS;

}

/******************************************************************************
 *
 * FUNCTION:        icf_print_open_media_channel_resp
 *
 * DESCRIPTION:     This function is used to print open_media_channel_resp
 *
 *****************************************************************************/

icf_return_t icf_print_open_media_channel_resp(icf_msg_st* p_msg)
{
    icf_uint8_t i = 0;

    icf_open_media_channel_resp_st* p_omc_resp = ICF_NULL;

	icf_list_st* p_channel_addr_list = ICF_NULL;

    p_omc_resp = (icf_open_media_channel_resp_st*)p_msg->payload;

    icf_print_api_header(p_msg);

	ICF_PRINT_MESSAGE(("\nResult:%u",
					p_omc_resp->result));

	
	if(!p_omc_resp->result)
	{
		ICF_PRINT_MESSAGE(("\nError Cause:%u",
					p_omc_resp->error_cause));
		icf_print_error_details(p_omc_resp->error_cause);
	}
	if(p_omc_resp->result)
	{
		ICF_PRINT_MESSAGE(("\nChannel List:"));

		i = 1;

		p_channel_addr_list = p_omc_resp->p_channel_addr_list;

		while(p_channel_addr_list!=ICF_NULL)
		{
			ICF_PRINT_MESSAGE(("\nNode %u:",i));

			ICF_PRINT_MESSAGE(("\nStream Type:%u",
					((icf_channel_addr_st*)(p_channel_addr_list->p_data))->stream_type));

			ICF_PRINT_MESSAGE(("\nStream Id:%u",
					((icf_channel_addr_st*)(p_channel_addr_list->p_data))->stream_id));

			ICF_PRINT_MESSAGE(("\n Stream Address "));
			icf_print_transport_addr((icf_transport_addr_st*)&(((icf_channel_addr_st*)(p_channel_addr_list->p_data))->stream_address.addr));
    
			ICF_PRINT_MESSAGE((
			"\n stream_info.port = %u", ((icf_channel_addr_st*)(p_channel_addr_list->p_data))->stream_address.port_num));

			p_channel_addr_list = p_channel_addr_list->p_next;
		
			i++;
		}
	}


	return ICF_SUCCESS;

}

/******************************************************************************
 *
 * FUNCTION:        icf_print_close_media_channel_req
 *
 * DESCRIPTION:     This function is used to print 
 *							close_media_channel_req
 *
 *****************************************************************************/

icf_return_t icf_print_close_media_channel_req(icf_msg_st* p_msg)
{
    icf_uint8_t i = 0;

    icf_close_media_channel_req_st* p_close_channel_req = ICF_NULL;

	icf_list_st* p_channel_list = ICF_NULL;

    p_close_channel_req = (icf_close_media_channel_req_st*)p_msg->payload;

    icf_print_api_header(p_msg);

 	
	ICF_PRINT_MESSAGE(("\nChannel List:"));

	i = 1;

	p_channel_list = p_close_channel_req->p_channel_list;

	while(p_channel_list!=ICF_NULL)
	{
		ICF_PRINT_MESSAGE(("\nNode %u:",i));

		ICF_PRINT_MESSAGE(("\nStream Type:%u",
					((icf_channel_st*)(p_channel_list->p_data))->stream_type));

		ICF_PRINT_MESSAGE(("\nStream Id:%u",
					((icf_channel_st*)(p_channel_list->p_data))->stream_id));

		p_channel_list = p_channel_list->p_next;
		
		i++;
	}


	return ICF_SUCCESS;

}

/******************************************************************************
 *
 * FUNCTION:        icf_print_send_udp_data_req
 *
 * DESCRIPTION:     This function is used to print 
 *							send_udp_data_req
 *
 *****************************************************************************/

icf_return_t icf_print_send_udp_data_req(icf_msg_st* p_msg)
{
    icf_send_udp_data_req_st* p_send_udp_data_req = ICF_NULL;

#ifdef ICF_NAT_MEDIA
    stun_msg_hdr              stun_msg_hdr_received;
    nat_octet_string_st       *p_buffer;
    nat_string_st             password;
    nat_error_t               ecode;
#endif

    p_send_udp_data_req = (icf_send_udp_data_req_st*)p_msg->payload;

#ifdef ICF_NAT_MEDIA
    memset(&stun_msg_hdr_received,0,sizeof(stun_msg_hdr));
    p_buffer = malloc(sizeof(nat_octet_string_st));
        p_buffer->octet_len = p_send_udp_data_req->app_data_len;
    memcpy(p_buffer->str,p_send_udp_data_req->p_app_data,p_buffer->octet_len);

    if (NAT_RETURN_FAILURE == stun_decode_message_header(p_buffer,&stun_msg_hdr_received,ecode))
         return ICF_FAILURE;

    /* storing transaction id received in the app_global data.*/ 
    memcpy(&transaction_id,&(stun_msg_hdr_received.transaction_id),sizeof(nat_uint128_t));

    free(p_buffer);
#endif 

    icf_print_api_header(p_msg);

   	
	ICF_PRINT_MESSAGE(("\nSource Port: %u",
										p_send_udp_data_req->source_port));

	ICF_PRINT_MESSAGE(("\nDestination Addr:"));

	ICF_PRINT_MESSAGE(("\nAddr type: %u",
										p_send_udp_data_req->dest_addr.addr.addr_type));

	switch(p_send_udp_data_req->dest_addr.addr.addr_type)
	{
		case  ICF_TRANSPORT_ADDRESS_DOMAIN_NAME:
			  ICF_PRINT_MESSAGE(("%s", 
              p_send_udp_data_req->dest_addr.addr.addr.domain.str));
			  break;
        
		case  ICF_TRANSPORT_ADDRESS_IPV4_ADDR:
			  ICF_PRINT_MESSAGE(("%u.%u.%u.%u", 
              p_send_udp_data_req->dest_addr.addr.addr.ipv4_addr.octet_1, 
              p_send_udp_data_req->dest_addr.addr.addr.ipv4_addr.octet_2, 
              p_send_udp_data_req->dest_addr.addr.addr.ipv4_addr.octet_3, 
              p_send_udp_data_req->dest_addr.addr.addr.ipv4_addr.octet_4));
			  break;

		case  ICF_TRANSPORT_ADDRESS_IPV6_ADDR:
			  ICF_PRINT_MESSAGE(("%s", 
              p_send_udp_data_req->dest_addr.addr.addr.ipv6_addr));
			  break;
	}

	ICF_PRINT_MESSAGE(("\nPort num: %u",
										p_send_udp_data_req->dest_addr.port_num));
	
	ICF_PRINT_MESSAGE(("\nApp data len: %u",
										p_send_udp_data_req->app_data_len));
	
	ICF_PRINT_MESSAGE(("\nApp data: %s",
										p_send_udp_data_req->p_app_data));


	return ICF_SUCCESS;

}

/******************************************************************************
 *
 * FUNCTION:        icf_print_recv_udp_data_ind
 *
 * DESCRIPTION:     This function is used to print 
 *							recv_udp_data_ind
 *
 *****************************************************************************/

icf_return_t icf_print_recv_udp_data_ind(icf_msg_st* p_msg)
{
    icf_recv_udp_data_ind_st* p_recv_udp_data_req = ICF_NULL;

    p_recv_udp_data_req = (icf_recv_udp_data_ind_st*)p_msg->payload;

    icf_print_api_header(p_msg);

	
	ICF_PRINT_MESSAGE(("\nDest Port: %u",
										p_recv_udp_data_req->dest_port));

	ICF_PRINT_MESSAGE(("\nSource Addr:"));

	ICF_PRINT_MESSAGE(("\nAddr type: %u",
										p_recv_udp_data_req->source_addr.addr.addr_type));

	switch(p_recv_udp_data_req->source_addr.addr.addr_type)
	{
		case  ICF_TRANSPORT_ADDRESS_DOMAIN_NAME:
			  ICF_PRINT_MESSAGE(("%s", 
              p_recv_udp_data_req->source_addr.addr.addr.domain.str));
			  break;

		case  ICF_TRANSPORT_ADDRESS_IPV4_ADDR:
			  ICF_PRINT_MESSAGE(("%u.%u.%u.%u", 
              p_recv_udp_data_req->source_addr.addr.addr.ipv4_addr.octet_1, 
              p_recv_udp_data_req->source_addr.addr.addr.ipv4_addr.octet_2, 
              p_recv_udp_data_req->source_addr.addr.addr.ipv4_addr.octet_3, 
              p_recv_udp_data_req->source_addr.addr.addr.ipv4_addr.octet_4));
			  break;

		case  ICF_TRANSPORT_ADDRESS_IPV6_ADDR:
			  ICF_PRINT_MESSAGE(("%s", 
              p_recv_udp_data_req->source_addr.addr.addr.ipv6_addr));
			  break;
	}

	ICF_PRINT_MESSAGE(("\nPort num: %u",
										p_recv_udp_data_req->source_addr.port_num));
	
	ICF_PRINT_MESSAGE(("\nApp data len: %u",
										p_recv_udp_data_req->app_data_len));
	
	ICF_PRINT_MESSAGE(("\nApp data: %s",
										p_recv_udp_data_req->p_app_data));

	return ICF_SUCCESS;

}


icf_return_t icf_print_cms_req(icf_msg_st* p_msg)
{

    
        icf_uint8_t i = 0;
    icf_uint16_t bit_mask = 0;
#ifdef ICF_FOR_LOAD_TESTING
	char command_string[ICF_MAX_BUFF_LEN];
#endif

    
    icf_create_media_session_req_st* p_cms_req = ICF_NULL;
	icf_list_st *p_list;

  p_cms_req = (icf_create_media_session_req_st*)p_msg->payload;

    icf_print_api_header(p_msg);



    ICF_PRINT_MESSAGE((
        "presence_flag = %u, line_id = %u",
        p_cms_req->bit_mask, p_cms_req->line_id));

#ifdef ICF_REL_OLD

    ICF_PRINT_MESSAGE((
        "\nstream_count = %u", p_cms_req->count));
    stream_count = p_cms_req->count;

    for (i = 0; i < stream_count; i++) {

        ICF_PRINT_MESSAGE(("\nstream_data[%u] = ", i));

        ICF_PRINT_MESSAGE(("\nbit_mask = %u", 
			p_cms_req->stream_data[i].bit_mask));

		bit_mask = p_cms_req->stream_data[i].bit_mask;



        ICF_PRINT_MESSAGE((
            "\n\tstream_data[%u].stream_type = %u"
            "\n\tstream_data[%u].stream_id = %u", 
            i, p_cms_req->stream_data[i].stream_type,
            i, p_cms_req->stream_data[i].stream_id));


        if (bit_mask & ICF_CREATE_MEDIA_LOCAL_CODEC_PRESENT) {
            ICF_PRINT_MESSAGE((
                "\n\tstream_data[%u].local_codec_list", i));
			icf_print_codec_list(&(p_cms_req->stream_data[i].local_codec_list)));
        }

     
        if (bit_mask & ICF_CREATE_MEDIA_MODE_PRESENT) 
			ICF_PRINT_MESSAGE((
            "\n\tstream_data[%u].mode = %u", i, p_cms_req->stream_data[i].mode));
		
		if (bit_mask & ICF_CREATE_MEDIA_QOS_MARK_PRESENT)
			ICF_PRINT_MESSAGE((
            "\n\tstream_data[%u].qos_mark = %u", i, p_cms_req->stream_data[i].qos_mark));

            
    }

 #endif



	p_list = p_cms_req->p_stream_list_data;

    for (i = 0; p_list != ICF_NULL; i++) {

        ICF_PRINT_MESSAGE(("\nstream_data[%u] = ", i));

        ICF_PRINT_MESSAGE(("\nbit_mask = %u", 
			((icf_req_stream_st*)(p_list->p_data))->bit_mask));

		bit_mask = ((icf_req_stream_st*)(p_list->p_data))->bit_mask;

        ICF_PRINT_MESSAGE((
            "\n\tstream_data[%u].stream_type = %u"
            "\n\tstream_data[%u].stream_id = %u", 
            i, ((icf_req_stream_st*)(p_list->p_data))->stream_type,
            i, ((icf_req_stream_st*)(p_list->p_data))->stream_id));


      if (bit_mask & ICF_CREATE_MEDIA_REMOTE_SDP_PRESENT) {
            ICF_PRINT_MESSAGE((
                "\n\tstream_data[%u].remote_sdp", i));
            icf_print_stream_sdp(&((icf_req_stream_st*)(p_list->p_data))->remote_sdp,\
									((icf_req_stream_st*)(p_list->p_data))->stream_type);
        }
		
        if (bit_mask & ICF_CREATE_MEDIA_LOCAL_MEDIA_PROFILE_PRESENT) {
            ICF_PRINT_MESSAGE((
                "\n\tstream_data[%u].local_sdp", i));
			icf_print_stream_sdp(&((icf_req_stream_st*)(p_list->p_data))->local_sdp,\
									((icf_req_stream_st*)(p_list->p_data))->stream_type);
        }

        if (bit_mask & ICF_CREATE_MEDIA_STREAM_MODE_PRESENT) 
	{
		ICF_PRINT_MESSAGE((
			            "\n\tstream_data[%u].mode = ", i));
	
		switch(((icf_req_stream_st*)(p_list->p_data))->mode)
		{
			case 0:  ICF_PRINT_MESSAGE((
					            "ICF_MEDIA_MODE_INVALID"));
				  break;
			case 1:  ICF_PRINT_MESSAGE((
					            "ICF_MEDIA_MODE_INACTIVE"));
				  break;
			case 2:  ICF_PRINT_MESSAGE((
					            "ICF_MEDIA_MODE_SEND_ONLY"));
				  break;
			case 3:  ICF_PRINT_MESSAGE((
					            "ICF_MEDIA_MODE_RECV_ONLY"));
				  break;
			case 4:  ICF_PRINT_MESSAGE((
					            "ICF_MEDIA_MODE_SEND_RECV"));
				  break;

		}
	}

		if (bit_mask & ICF_CREATE_MEDIA_QOS_PARAMS_PRESENT)
			ICF_PRINT_MESSAGE((
            "\n\tstream_data[%u].qos_mark = %u", i, ((icf_req_stream_st*)(p_list->p_data))->qos_mark));

        if (bit_mask & ICF_CREATE_MEDIA_RTP_DETECTION_PRESENT)
			ICF_PRINT_MESSAGE((
            "\n\tstream_data[%u].rtp_detection = %u", i, ((icf_req_stream_st*)(p_list->p_data))->rtp_detection_reqd));

		if (bit_mask & ICF_CREATE_MEDIA_ECHO_CAN_PRESENT)
			ICF_PRINT_MESSAGE((
            "\n\tstream_data[%u].echo_cancellation = %u", i, ((icf_req_stream_st*)(p_list->p_data))->echo_cancellation));
		
		
		p_list = p_list->p_next;
		
	     
    }



#ifdef ICF_FOR_LOAD_TESTING

	app_global_data.call_id = p_msg->hdr.call_id;
    strcpy(command_string,ICF_DEFAULT_CMS_RSP_MSG);
    icf_command_handler(command_string);

#endif
    return ICF_SUCCESS;

}



icf_return_t icf_print_dms_ind(icf_msg_st *p_msg)
{
    
    icf_uint8_t	i = 0;

    icf_delete_media_session_ind_st* p_dms_ind = NULL;


    icf_print_api_header(p_msg);
      
    p_dms_ind = (icf_delete_media_session_ind_st*)p_msg->payload;



	ICF_PRINT_MESSAGE(("count = %u ",p_dms_ind->stream_list.count));

	ICF_PRINT_MESSAGE(("Stream IDs "));

	for(i=0;i<p_dms_ind->stream_list.count;i++)
	{
		ICF_PRINT_MESSAGE(("%u. %u",i,p_dms_ind->stream_list.stream_id[i]));
	}


	if (p_dms_ind->bit_mask & ICF_DEL_MEDIA_SESSION_ERR_CAUSE_PRESENT)
	{
		ICF_PRINT_MESSAGE(("error_cause = %u ",p_dms_ind->error_cause));
		icf_print_error_details(p_dms_ind->error_cause);
    }
    
 
        
    return ICF_SUCCESS;
    
}






icf_return_t icf_print_merge_media_req_msg(icf_msg_st *p_msg)
{
    
    icf_uint8_t	 i=0;

    icf_merge_media_session_req_st* p_mms_req = NULL;


    icf_print_api_header(p_msg);
    
    p_mms_req = (icf_merge_media_session_req_st*)p_msg->payload;



	ICF_PRINT_MESSAGE(("merge_call_id = %u ", p_mms_req->merge_call_id));

    ICF_PRINT_MESSAGE(("count = %u \n", p_mms_req->count));

	for(i=0;i<p_mms_req->count;i++)
	{
		ICF_PRINT_MESSAGE(("stream_count = %u ", i));
    	ICF_PRINT_MESSAGE((" stream_id = %u ", p_mms_req->merge_stream_id[i].stream_id));
		ICF_PRINT_MESSAGE((" merge_stream_id = %u ", p_mms_req->merge_stream_id[i].merge_stream_id));
	}


#ifdef ICF_AUTO_RESPOND
    icf_command_handler("SEND mms_resp mms_resp.mg"));
#endif

    
    
    return ICF_SUCCESS;
    
}



icf_return_t icf_print_cms_resp(icf_msg_st* p_msg)
{

    icf_uint8_t i = 0;
    icf_create_media_session_resp_st* p_cms_resp = ICF_NULL;

    icf_list_st *p_resp= ICF_NULL;
    p_cms_resp = (icf_create_media_session_resp_st*)p_msg->payload;

    p_resp=p_cms_resp->p_accepted_stream_list;


    icf_print_api_header(p_msg);




    ICF_PRINT_MESSAGE(("bitmask = %u, result = %u, error_cause = %u",
        p_cms_resp->bit_mask, p_cms_resp->result, p_cms_resp->error_cause));
    icf_print_error_details(p_cms_resp->error_cause);

	while(p_resp!=ICF_NULL)
	{
		ICF_PRINT_MESSAGE(("\nNode %u:",i));

		icf_print_resp_stream((icf_resp_stream_st*)(p_resp->p_data),i);
		
		p_resp=p_resp->p_next;		
		i++;
	}


	if(p_cms_resp->bit_mask & ICF_CMS_REJECT_STREAM_PRESENT)
	{
		ICF_PRINT_MESSAGE(("\n Rejected Stream List"));

		ICF_PRINT_MESSAGE(("\n count = %u", p_cms_resp->rejected_stream_list.count));

		for(i=0;i<p_cms_resp->rejected_stream_list.count;i++)
		{
			ICF_PRINT_MESSAGE(("\n stream_id = %u", p_cms_resp->rejected_stream_list.stream_id[i]));

		}
	}



    return ICF_SUCCESS;

}



icf_return_t icf_print_mms_req(icf_msg_st* p_msg)
{    
 
    icf_modify_media_session_req_st* p_mms_req = NULL;

       icf_uint8_t i = 0, bit_mask = 0;
	icf_list_st *p_stream_data_list;
#ifdef ICF_FOR_LOAD_TESTING
	char command_string[ICF_MAX_BUFF_LEN];
#endif

    p_mms_req = (icf_modify_media_session_req_st*)p_msg->payload;

    icf_print_api_header(p_msg);



/* TBD */
#ifdef ICF_REL_OLD

   ICF_PRINT_MESSAGE((
        "\nstream_count = %u", p_mms_req->count));

    stream_count = p_mms_req->count;



    for (i = 0; i < stream_count; i++) {

        ICF_PRINT_MESSAGE(("\nstream_data[%u] = ", i));


        ICF_PRINT_MESSAGE((
            "\n\tstream_data[%u].stream_type = %u"
            "\n\tstream_data[%u].stream_id = %u"
			"\n\tstream_data[%u].bit_mask = %u", 
            i, p_mms_req->stream_data[i].stream_type,
            i, p_mms_req->stream_data[i].stream_id,
            i, p_mms_req->stream_data[i].bit_mask));


     
        if (p_mms_req->stream_data[i].bit_mask & ICF_MODIFY_MEDIA_MODE_PRESENT) {
            ICF_PRINT_MESSAGE(("\nstream_data[%u].mode", i));
            ICF_PRINT_MESSAGE(( 
            "\n\tmode = %u", p_mms_req->stream_data[i].mode));
        }

            }

#endif




	p_stream_data_list = p_mms_req->p_stream_data_list;

	for (i = 0; p_stream_data_list != ICF_NULL; i++) {

        ICF_PRINT_MESSAGE(("\nstream_data[%u] = ", i));

        ICF_PRINT_MESSAGE(("\nbit_mask = %u", 
			((icf_modify_stream_st*)(p_stream_data_list->p_data))->bit_mask));

		bit_mask = ((icf_modify_stream_st*)(p_stream_data_list->p_data))->bit_mask;

        ICF_PRINT_MESSAGE((
            "\n\tstream_data[%u].stream_type = %u"
            "\n\tstream_data[%u].stream_id = %u", 
            i, ((icf_modify_stream_st*)(p_stream_data_list->p_data))->stream_type,
            i, ((icf_modify_stream_st*)(p_stream_data_list->p_data))->stream_id));


		if ((bit_mask & ICF_MODIFY_REMOTE_SDP_STREAM_PRESENT) || (bit_mask & ICF_MODIFY_LOCAL_SDP_STREAM_PRESENT)) {
            ICF_PRINT_MESSAGE((
                "\n\tstream_data[%u].stream_sdp", i));
            icf_print_stream_sdp(&((icf_modify_stream_st*)(p_stream_data_list->p_data))->sdp_stream,\
									((icf_modify_stream_st*)(p_stream_data_list->p_data))->stream_type);
        }
		
        if (bit_mask & ICF_MODIFY_MEDIA_MODE_PRESENT) 
	{
			ICF_PRINT_MESSAGE((
			            "\n\tstream_data[%u].media_mode = ", i));
	
		switch(((icf_modify_stream_st*)(p_stream_data_list->p_data))->mode)
		{
			case 0:  ICF_PRINT_MESSAGE((
					            "ICF_MEDIA_MODE_INVALID"));
				  break;
			case 1:  ICF_PRINT_MESSAGE((
					            "ICF_MEDIA_MODE_INACTIVE"));
				  break;
			case 2:  ICF_PRINT_MESSAGE((
					            "ICF_MEDIA_MODE_SEND_ONLY"));
				  break;
			case 3:  ICF_PRINT_MESSAGE((
					            "ICF_MEDIA_MODE_RECV_ONLY"));
				  break;
			case 4:  ICF_PRINT_MESSAGE((
					            "ICF_MEDIA_MODE_SEND_RECV"));
				  break;

		}
	}
		if (bit_mask & ICF_MODIFY_MEDIA_ECHO_CAN_PRESENT)
			ICF_PRINT_MESSAGE((
            "\n\tstream_data[%u].echo_cancellation = %u", i, 
				((icf_modify_stream_st*)(p_stream_data_list->p_data))->echo_cancellation));
		
		
		p_stream_data_list = p_stream_data_list->p_next;
		
	    
    }



#ifdef ICF_FOR_LOAD_TESTING

	app_global_data.call_id = p_msg->hdr.call_id;
    strcpy(command_string,ICF_DEFAULT_MMS_RSP_MSG);
    icf_command_handler(command_string);

#endif

    return ICF_SUCCESS;   
    
}


icf_return_t icf_print_mm_unexpected_msg_ind(icf_msg_st* p_msg)
{
        icf_mm_unexpected_msg_ind_st                  *p_mm_unexpected_msg = ICF_NULL;

        icf_print_api_header(p_msg);

        p_mm_unexpected_msg = (icf_mm_unexpected_msg_ind_st*)p_msg->payload;
        
        ICF_PRINT_MESSAGE(("\nError Cause = %u",
              p_mm_unexpected_msg->error_cause));

        ICF_PRINT_MESSAGE(("\nApi_id = %u",
              p_mm_unexpected_msg->api_id));

        return ICF_SUCCESS;
}




icf_return_t icf_print_transport_addr(icf_transport_addr_st* p_trans)
{                                      


    icf_uint8_t addr_type = 0;

    if (!p_trans)
        return ICF_FAILURE;


    ICF_PRINT_MESSAGE(( 
        "\naddr_type = %u, ", p_trans->addr_type));
    
    addr_type = p_trans->addr_type;
    
    switch (p_trans->addr_type) {
    case ICF_TRANSPORT_ADDRESS_DOMAIN_NAME:
        ICF_PRINT_MESSAGE(("%s, ", 
            p_trans->addr.domain.str));
        break;
        
    case ICF_TRANSPORT_ADDRESS_IPV4_ADDR:
        ICF_PRINT_MESSAGE(("%u.%u.%u.%u", 
            p_trans->addr.ipv4_addr.octet_1, 
            p_trans->addr.ipv4_addr.octet_2, 
            p_trans->addr.ipv4_addr.octet_3, 
            p_trans->addr.ipv4_addr.octet_4));
        break;
        
    }

    return ICF_SUCCESS;
}








icf_return_t icf_print_header_list(icf_header_st* p_hdr, 
                                      icf_int32_t hdr_count)
{

    icf_uint8_t i = 0;

    if (!p_hdr || hdr_count < 0)
        return ICF_FAILURE;


/* Header LIST  */    if (hdr_count > 0) 
        ICF_PRINT_MESSAGE(("\nheader_list.count = %u\n", 
            hdr_count));

    for (i = 0; i < hdr_count && p_hdr; i++, p_hdr = p_hdr->next) 
        ICF_PRINT_MESSAGE(( 
            "\nheader_list.hdr.hdr_name.str = %s,\nheader_list.hdr.hdr_value.str = %s", 
            p_hdr->hdr_name.str, p_hdr->hdr_value.str));


    return ICF_SUCCESS;
}



icf_return_t icf_print_tag_list(icf_tag_st* p_tag, 
                                   icf_int32_t tag_count)
{

    icf_uint8_t i = 0;

    if (!p_tag || tag_count < 0)
        return ICF_FAILURE;

    if (tag_count > 0) 
        ICF_PRINT_MESSAGE(("\ntag_list.count = %u", 
            tag_count));

    for (i = 0; i < tag_count && p_tag; i++, p_tag = p_tag->next) 
	{
        ICF_PRINT_MESSAGE(( 
            "\ntag_list.tag.hdr_name.str = %s, \ntag_list.tag.tag_value.str = %s", 
            p_tag->hdr_name.str, p_tag->tag_value.str));
		if ( p_tag->tag_type == ICF_TAG_TYPE_SIP_URI_PARAM)
		{
			ICF_PRINT_MESSAGE(( 
            "\nICF_TAG_TYPE_SIP_URI_PARAM"));
		}
		else if ( p_tag->tag_type == ICF_TAG_TYPE_SIP_URI_HDR_PARAM)
		{
			ICF_PRINT_MESSAGE(( 
            "\nICF_TAG_TYPE_SIP_URI_HDR_PARAM"));
		}
		else if ( p_tag->tag_type == ICF_TAG_TYPE_HDR_PARAM)
		{
			ICF_PRINT_MESSAGE(( 
            "\nICF_TAG_TYPE_HDR_PARAM"));
		}
		else
		{
			ICF_PRINT_MESSAGE(( 
            "\nICF_TAG_TYPE_INVALID"));
		}

	}



    return ICF_SUCCESS;
}





icf_return_t icf_print_body_list(icf_msg_body_st* p_body, 
                                    icf_int32_t body_count)
{

    icf_uint8_t i = 0;
    icf_uint8_t hdr_count = 0;

    if (!p_body || body_count < 0)
        return ICF_FAILURE;

    if (body_count > 0) 
        ICF_PRINT_MESSAGE(("\nbody_list.count = %u", 
            body_count));

    for (i = 0; i < body_count && p_body; i++, p_body = p_body->next) 
    {
	    ICF_PRINT_MESSAGE(( 
            "\nbody_list.body.content_type.str = %s", p_body->content_type.str));

	    /* Display Body header list if present
	     */
	    hdr_count =(icf_uint8_t) p_body->msg_body_hdr_list.count;
		if(0 < p_body->msg_body_hdr_list.count)
	    {
		    icf_print_header_list(p_body->msg_body_hdr_list.hdr,
				   hdr_count);
	    }
	    
	    ICF_PRINT_MESSAGE((
	    "\nbody_list.body.length = %u" "body_list.body.value = %s", 
	    p_body->length, p_body->value));
    }
   return ICF_SUCCESS;
}


/* TBD */
#ifdef ICF_REL_OLD
icf_return_t icf_print_stream_list(icf_stream_st* p_stream,
                                      icf_uint8_t stream_count)
{

    icf_uint8_t i = 0, j = 0, att_count = 0;

    if (stream_count > 0) 
        ICF_PRINT_MESSAGE(("\nstream_list.count = %u", 
            stream_count));


    for (i = 0; i < stream_count; i++) {

        ICF_PRINT_MESSAGE((
            "\nstream_list.stream[%u].type = %u, stream_list.stream[%u].stream_id = %u", 
            i, p_stream[i].type, i, p_stream[i].stream_id));

        ICF_PRINT_MESSAGE((
            "\nstream_list.stream[%u].codec.str = %s", i, p_stream[i].codec.str));

        att_count = p_stream[i].att_count;


        ICF_PRINT_MESSAGE((
            "\nstream_list.stream[%u].att_count = %u", i, att_count));
        
        for (j = 0; j < att_count; j++) 
            ICF_PRINT_MESSAGE((
                "\nstream_list.stream[%u].attributes[%u].str = %s", i, j,
                p_stream[i].attributes[j].str));
        

    }
    return ICF_SUCCESS;
}
#endif


icf_return_t icf_print_header_name_list(icf_header_name_st* p_hdr, 
                                           icf_uint8_t hdr_count)
{

    icf_uint8_t i = 0;

    if (!p_hdr)
        return ICF_FAILURE;


/* Header LIST  */    if (hdr_count > 0) 
        ICF_PRINT_MESSAGE(( 
            "\nheader_name_list.count = %u", hdr_count));

    for (i = 0; i < hdr_count && p_hdr; i++, p_hdr = p_hdr->next) 
        ICF_PRINT_MESSAGE(( 
            "\nheader_name_list.hdr.hdr_name.str = %s", p_hdr->hdr_name.str));

    return ICF_SUCCESS;
}





icf_return_t icf_print_media_connected(icf_msg_st* p_msg)
{

    icf_media_connected_ind_st* p_med_con = NULL;

    icf_list_st *p_list=ICF_NULL;
    
	icf_uint8_t i=0;
    p_med_con = (icf_media_connected_ind_st*)p_msg->payload;

    icf_print_api_header(p_msg);
    ICF_PRINT_MESSAGE(( 
              "bit_mask = %u", p_med_con->bit_mask));


#ifdef ICF_REL_OLD
    stream_list_count = p_med_con->stream_list.count;
    icf_print_stream_list(p_med_con->stream_list.stream, stream_list_count);
#endif



	ICF_PRINT_MESSAGE((
        "\n Accepted Stream List:"));

	i = 1;
        p_list=p_med_con->p_accepted_stream_list;  
	while(p_list!=ICF_NULL)
	{
		ICF_PRINT_MESSAGE(("\nNode %u:",i));

		icf_print_stream_capab(p_list->p_data);
		
		p_list = p_list->p_next;
		
		i++;
	}


	if(p_med_con->bit_mask & ICF_MEDIA_CONECT_DELETED_STREAM_PRESENT)
	{
		ICF_PRINT_MESSAGE(("\n Deleted stream count %u:",p_med_con->count));	

		for(i=0;i<p_med_con->count;i++)
		{
			ICF_PRINT_MESSAGE(("\n stream type %u:",p_med_con->deleted_media_streams[i].stream_type));
			ICF_PRINT_MESSAGE(("\n stream id %u:",p_med_con->deleted_media_streams[i].stream_id));
		}
	}




    return ICF_SUCCESS;

}








icf_return_t icf_print_connect_req(icf_msg_st* p_msg)
{


    icf_connect_req_st* p_con_req = NULL;

    icf_uint8_t  header_list_count = 0, tag_list_count = 0,
                    body_list_count = 0,i=0;   
    icf_list_st *p_stream_list;

    p_con_req = (icf_connect_req_st*)p_msg->payload;

    icf_print_api_header(p_msg);



    ICF_PRINT_MESSAGE(( 
              "\nbit_mask = %u", p_con_req->bit_mask));

    ICF_PRINT_MESSAGE(( 
        "\nprivacy_ind = %u", p_con_req->privacy_ind));
    
    
/* Header LIST  */    if (p_con_req->bit_mask & ICF_HDR_LIST_PRESENT) {
        header_list_count = (icf_uint8_t)p_con_req->header_list.count;
        icf_print_header_list(p_con_req->header_list.hdr, header_list_count);
    }
       if (p_con_req->bit_mask & ICF_TAG_LIST_PRESENT) {
        tag_list_count =(icf_uint8_t) p_con_req->tag_list.count;
        icf_print_tag_list(p_con_req->tag_list.tag, tag_list_count);
    }
    
        if (p_con_req->bit_mask & ICF_BODY_LIST_PRESENT) {
        body_list_count = (icf_uint8_t)p_con_req->body_list.count;
        icf_print_body_list(p_con_req->body_list.body, body_list_count);
    }
    
    #ifdef ICF_REL_OLD
    if (p_con_req->bit_mask & ICF_STREAM_LIST_PRESENT) {
        stream_list_count = p_con_req->stream_list.count;
        icf_print_stream_list(p_con_req->stream_list.stream, stream_list_count);
    }
#endif




	if(p_con_req->bit_mask & ICF_STREAM_LIST_PRESENT)
	{

		ICF_PRINT_MESSAGE((
	        "\nMedia Profile List:"));
		p_stream_list= p_con_req->p_stream_list;
		i = 1;
		while(p_stream_list!=ICF_NULL)
		{
			ICF_PRINT_MESSAGE(("\nNode %u:",i));
			icf_print_stream_capab(p_stream_list->p_data);
			p_stream_list = p_stream_list->p_next;
			i++;
		}
	}

	if(p_con_req->bit_mask & ICF_CONNECT_DELETED_STREAM_PRESENT)
	{
		ICF_PRINT_MESSAGE(("\n rejected stream count: %u",
			p_con_req->count));	

		for(i=0;i<p_con_req->count;i++)
		{
			ICF_PRINT_MESSAGE(("\n Stream %u", i+1));

			ICF_PRINT_MESSAGE(("\n stream id: %u",
				p_con_req->deleted_media_streams[i].stream_id));

			ICF_PRINT_MESSAGE(("\n stream type: %u",
				p_con_req->deleted_media_streams[i].stream_type));
		}
	}


  
    return ICF_SUCCESS;

}




icf_return_t icf_print_connect_resp(icf_msg_st* p_msg)
{

    
    icf_connect_resp_st* p_con_resp = NULL;

    icf_uint8_t  i=1;
   
    icf_list_st *p_list=ICF_NULL;
    p_con_resp = (icf_connect_resp_st*)p_msg->payload;

    icf_print_api_header(p_msg);
    p_list=p_con_resp->p_accepted_stream_list; 
	while(p_list!=ICF_NULL)
	{
		ICF_PRINT_MESSAGE(("\nNode %u:",i));

		icf_print_modify_media_change_req(p_list->p_data);
		
		p_list = p_list->p_next;
		
		i++;
	}

	if(p_con_resp->bit_mask & ICF_CONNECT_RESP_DELETED_STREAM_PRESENT)
	{
		ICF_PRINT_MESSAGE(("\n Deleted stream count: %u",p_con_resp->count));	

		for(i=0;i<p_con_resp->count;i++)
		{
			ICF_PRINT_MESSAGE(("\n stream type: %u",p_con_resp->deleted_media_streams[i].stream_type));
			ICF_PRINT_MESSAGE(("\n stream id: %u",p_con_resp->deleted_media_streams[i].stream_id));
		}
	}

 
#ifdef ICF_FOR_LOAD_TESTING

	s_calls++;
    ICF_PRINT_MESSAGE(("\n No.of Successfully calls:%u",s_calls));
    
#endif

    return ICF_SUCCESS;

}



icf_return_t icf_print_connect_ind(icf_msg_st* p_msg)
{


    icf_connect_ind_st* p_con_ind = NULL;

    icf_uint8_t  header_list_count = 0, body_list_count = 0,i;


    p_con_ind = (icf_connect_ind_st*)p_msg->payload;

    icf_print_api_header(p_msg);



    ICF_PRINT_MESSAGE(( 
              "bit_mask = %u", p_con_ind->bit_mask));
	if (p_con_ind->bit_mask & ICF_ASSERTED_ID_PRESENT)
		ICF_PRINT_MESSAGE(( 
			"\nasserted_identity.addr_val.str = %s"
			"\nasserted_identity.addr_type = %u", 
			p_con_ind->asserted_identity.addr_val.str, 
			p_con_ind->asserted_identity.addr_type));
    

/* Header LIST  */    if (p_con_ind->bit_mask & ICF_HDR_LIST_PRESENT) {
        header_list_count = (icf_uint8_t)p_con_ind->header_list.count;
        icf_print_header_list(p_con_ind->header_list.hdr, header_list_count);
    }
  
    
    /* Body LIST  */
    if (p_con_ind->bit_mask & ICF_BODY_LIST_PRESENT) {
        body_list_count = (icf_uint8_t)p_con_ind->body_list.count;
        icf_print_body_list(p_con_ind->body_list.body, body_list_count);
    }
    /* Stream LIST  */
#ifdef ICF_REL_OLD
    if (p_con_ind->bit_mask & ICF_STREAM_LIST_PRESENT) {
        stream_list_count = p_con_ind->stream_list.count;
        icf_print_stream_list(p_con_ind->stream_list.stream, stream_list_count);
    }
#endif

    i = 1;

	while(p_con_ind->p_accepted_stream_list!=ICF_NULL)
	{
		ICF_PRINT_MESSAGE(("\nNode %u:",i));

		icf_print_modify_media_change_req(p_con_ind->p_accepted_stream_list->p_data);
		
		p_con_ind->p_accepted_stream_list = p_con_ind->p_accepted_stream_list->p_next;
		
		i++;
	}


	if(p_con_ind->bit_mask & ICF_CONNECT_IND_DELETED_STREAM_PRESENT)
	{
		ICF_PRINT_MESSAGE(("\n Deleted stream count: %u",p_con_ind->count));	

		for(i=0;i<p_con_ind->count;i++)
		{
			ICF_PRINT_MESSAGE(("\n stream type: %u",p_con_ind->deleted_media_streams[i].stream_type));
			ICF_PRINT_MESSAGE(("\n stream id: %u",p_con_ind->deleted_media_streams[i].stream_id));
		}
	}


#ifdef ICF_FOR_LOAD_TESTING

	s_calls++;
    ICF_PRINT_MESSAGE(("\nNo.of Successfully Calls:%u",s_calls);
  
#endif

    return ICF_SUCCESS;

}

icf_return_t icf_print_get_line_settings_resp(icf_msg_st* p_msg)
{ 
	icf_get_line_settings_resp_st* p_get_line_stg_resp = ICF_NULL;
	icf_uint32_t	presence_flag = ICF_NULL;
	icf_transport_addr_type_t  addr_type = ICF_NULL;

	p_get_line_stg_resp = (icf_get_line_settings_resp_st*)p_msg->payload;
	presence_flag = p_get_line_stg_resp->bit_mask;

	icf_print_api_header(p_msg);

	ICF_PRINT_MESSAGE(( \
		"\n------ This is Print function for get_line_settings_resp -------\n"));
		
	if(presence_flag & ICF_REQD_PROXY_ADDR)
	{
		addr_type = p_get_line_stg_resp->proxy_address.addr.addr_type;

		ICF_PRINT_MESSAGE(("\nProxy address = proxy_addr_type = %u addr = ",
					addr_type));
        
        switch (addr_type) 
		{
			case ICF_TRANSPORT_ADDRESS_DOMAIN_NAME:
				ICF_PRINT_MESSAGE(("%s, ", 
					p_get_line_stg_resp->proxy_address.addr.addr.domain.str));
				break;
            
			case ICF_TRANSPORT_ADDRESS_IPV4_ADDR:
				ICF_PRINT_MESSAGE(("%u.%u.%u.%u, ", 
					p_get_line_stg_resp->proxy_address.addr.addr.ipv4_addr.octet_1, 
					p_get_line_stg_resp->proxy_address.addr.addr.ipv4_addr.octet_2, 
					p_get_line_stg_resp->proxy_address.addr.addr.ipv4_addr.octet_3, 
					p_get_line_stg_resp->proxy_address.addr.addr.ipv4_addr.octet_4));
				break;
          }

		ICF_PRINT_MESSAGE(( 
			"\n proxy_port = %u, ",
			  p_get_line_stg_resp->proxy_address.port_num));
	}

	if(presence_flag & ICF_REQD_REGISTRAR_ADDR)
	{

		addr_type = p_get_line_stg_resp->registrar_address.addr.addr_type;

		ICF_PRINT_MESSAGE(( 
				"\n registrar_addr = addr_type = %u, ",
				  addr_type));
				
		switch (addr_type) 
		{
			case ICF_TRANSPORT_ADDRESS_DOMAIN_NAME:
				ICF_PRINT_MESSAGE(("%s, ", 
					p_get_line_stg_resp->registrar_address.addr.addr.domain.str));
				break;
			
			case ICF_TRANSPORT_ADDRESS_IPV4_ADDR:
				ICF_PRINT_MESSAGE(("%u.%u.%u.%u, ", 
					p_get_line_stg_resp->registrar_address.addr.addr.ipv4_addr.octet_1, 
					p_get_line_stg_resp->registrar_address.addr.addr.ipv4_addr.octet_2, 
					p_get_line_stg_resp->registrar_address.addr.addr.ipv4_addr.octet_3, 
					p_get_line_stg_resp->registrar_address.addr.addr.ipv4_addr.octet_4));
				break;
		}

		ICF_PRINT_MESSAGE(( 
			"\n registrar_port = %u, ",
			  p_get_line_stg_resp->registrar_address.port_num));
	}


	if(presence_flag & ICF_REQD_PROXY_TRANSPORT_MODE)
	{
		ICF_PRINT_MESSAGE(("\nproxy_transport_mode = %u" ,
			p_get_line_stg_resp->proxy_transport_mode));
	}

	if(presence_flag & ICF_REQD_SELF_ADDR_LIST)
	{
		ICF_PRINT_MESSAGE(("\nself_address_list = addr_type = %u self_address = %s" ,
			p_get_line_stg_resp->self_address_list.addr_type,
			p_get_line_stg_resp->self_address_list.addr_val.str));
	}
	
	if(presence_flag & ICF_REQD_SELF_NAME)
	{
		ICF_PRINT_MESSAGE(("\nself name = %s" ,
			p_get_line_stg_resp->self_name.str));
	}
	
	if(presence_flag & ICF_REQD_PRESENTATION_STATUS)
	{
		ICF_PRINT_MESSAGE(("\nPresentation Status = %u" ,
			p_get_line_stg_resp->presentation_allowed));
	}
	
	if(presence_flag & ICF_REQD_AUTH_KEY)
	{
		ICF_PRINT_MESSAGE(("\nauth_key = %s" ,
			p_get_line_stg_resp->auth_key.str));
	}
	
	if(presence_flag & ICF_REQD_AUTH_PASSWD)
	{
		ICF_PRINT_MESSAGE(("\nauth_password = %s" ,
			p_get_line_stg_resp->auth_password.str));
	}
	if(presence_flag & ICF_REQD_REG_MODE)
	{
		ICF_PRINT_MESSAGE(("\nreg_mode = %u" ,
			p_get_line_stg_resp->reg_mode));
	}
	if(presence_flag & ICF_REQD_REG_DURATION)
	{
		ICF_PRINT_MESSAGE(("\nreg_duration = %u" ,
			p_get_line_stg_resp->reg_duration));
	}
/*  FIX for spr 17944  */
	if(presence_flag & ICF_REQD_REG_ROUTE_ADDR)
	{
		addr_type = p_get_line_stg_resp->reg_route_addr.addr.addr_type;

		ICF_PRINT_MESSAGE(("\nReg Route address = Reg_route_addr_type = %u addr = ",
					addr_type));
        
        switch (addr_type) 
		{
			case ICF_TRANSPORT_ADDRESS_DOMAIN_NAME:
				ICF_PRINT_MESSAGE(("%s, ", 
					p_get_line_stg_resp->reg_route_addr.addr.addr.domain.str));
				break;
            
			case ICF_TRANSPORT_ADDRESS_IPV4_ADDR:
				ICF_PRINT_MESSAGE(("%u.%u.%u.%u, ", 
					p_get_line_stg_resp->reg_route_addr.addr.addr.ipv4_addr.octet_1, 
					p_get_line_stg_resp->reg_route_addr.addr.addr.ipv4_addr.octet_2, 
					p_get_line_stg_resp->reg_route_addr.addr.addr.ipv4_addr.octet_3, 
					p_get_line_stg_resp->reg_route_addr.addr.addr.ipv4_addr.octet_4));
				break;
                }

		ICF_PRINT_MESSAGE(( 
			"\n Reg Route port = %u, ",
			  p_get_line_stg_resp->reg_route_addr.port_num));
	}	
	if(presence_flag & ICF_REQD_SIP_T1_TIMER)
     {
                ICF_PRINT_MESSAGE(("\nSIP T1 TIMER = %u" ,
                        p_get_line_stg_resp->sip_T1_timer));
     }
    if(presence_flag & ICF_REQD_SIP_B_TIMER)
     {
                ICF_PRINT_MESSAGE(("\nSIP B TIMER = %u" ,
                        p_get_line_stg_resp->sip_B_timer));
     }
    if(presence_flag & ICF_REQD_SIP_F_TIMER)
     {
                ICF_PRINT_MESSAGE(("\nSIP F TIMER = %u" ,
                        p_get_line_stg_resp->sip_F_timer));
     }
/* SPR 17944 FIX END */
	return ICF_SUCCESS;
}

icf_return_t icf_print_get_sys_settings_resp(icf_msg_st* p_msg)
{ 
	icf_get_system_settings_resp_st* p_get_sys_stg_resp = ICF_NULL;
	icf_uint32_t	presence_flag = ICF_NULL;
	icf_transport_addr_type_t  addr_type = ICF_NULL;
	icf_uint8_t i=0;
	p_get_sys_stg_resp = (icf_get_system_settings_resp_st*)p_msg->payload;
	presence_flag = p_get_sys_stg_resp->bit_mask;

	icf_print_api_header(p_msg);
	
	ICF_PRINT_MESSAGE(( \
		"\n------ This is Print function for get_sys_settings -------\n"));

	if(presence_flag & ICF_REQD_PRACK_ENABLED_STATUS)
	{
		ICF_PRINT_MESSAGE(("\nprack_enabled = %u" ,
			p_get_sys_stg_resp->prack_enabled));
	}
	
	if(presence_flag & ICF_REQD_AUTH_TYPE_STATUS)
	{
		ICF_PRINT_MESSAGE(("\nauth_type = %u" ,
			p_get_sys_stg_resp->auth_type));
	}
	if(presence_flag & ICF_REQD_PRIVACY_SUPPORTED)
	{
		ICF_PRINT_MESSAGE(("\nprivacy_supported= %u" ,
			p_get_sys_stg_resp->privacy_supported));

	}
	if(presence_flag & ICF_REQD_MAX_LINES)
	{
		ICF_PRINT_MESSAGE(("\nmax_lines = %u" ,
			p_get_sys_stg_resp->max_lines));
	}
	if(presence_flag & ICF_REQD_SETUP_TIMER)
	{
		ICF_PRINT_MESSAGE(("\nsetup_timer = %u" ,
			p_get_sys_stg_resp->setup_timer));
	}
	if(presence_flag & ICF_REQD_ALERTING_TIMER)
	{
		ICF_PRINT_MESSAGE(("\nalerting_timer = %u" ,
			p_get_sys_stg_resp->alerting_timer));
	}
	if(presence_flag & ICF_REQD_RELEASE_TIMER)
	{
		ICF_PRINT_MESSAGE(("\nrelease_timer = %u" ,
			p_get_sys_stg_resp->release_timer));
	}
	if(presence_flag & ICF_REQD_MODIFY_MEDIA_TIMER)
	{
		ICF_PRINT_MESSAGE(("\nmodify_media_timer = %u" ,
			p_get_sys_stg_resp->modify_media_timer));
	}
	if(presence_flag & ICF_REQD_REG_RESP_TIMER)
	{
		ICF_PRINT_MESSAGE(("\nregistration_resp_timer = %u" ,
			p_get_sys_stg_resp->registration_resp_timer));
	}
	if(presence_flag & ICF_REQD_REG_RETRY_TIMER)
	{
		ICF_PRINT_MESSAGE(("\nregistration_retry_timer = %u" ,
			p_get_sys_stg_resp->registration_retry_timer));
	}
	if(presence_flag & ICF_REQD_SUBS_RETRY_TIMER)
	{
		ICF_PRINT_MESSAGE(("\nsubs_retry_timer = %u" ,
			p_get_sys_stg_resp->subs_retry_timer));
	}

	if(presence_flag & ICF_REQD_SUBS_RESP_TIMER)
	{
		ICF_PRINT_MESSAGE(("\nsubs_resp_timer = %u" ,
			p_get_sys_stg_resp->subs_resp_timer));
	}

	if(presence_flag & ICF_REQD_SIG_COMP_STATUS)
	{
		ICF_PRINT_MESSAGE(("\nsig_comp_enabled = %u" ,
			p_get_sys_stg_resp->sig_comp_enabled));
	}

	if(presence_flag & ICF_REQD_SECURITY_STATUS)
	{
		ICF_PRINT_MESSAGE(("\nsecurity_enabled = %u" ,
			p_get_sys_stg_resp->security_enabled));
	}

	if(presence_flag & ICF_REQD_SELF_IP_ADDRESS)
	{

		addr_type = p_get_sys_stg_resp->self_ip_address.addr.addr_type;

		ICF_PRINT_MESSAGE(( 
				"\n self_ip_address = addr_type = %u, ",
				  addr_type));
				
		switch (addr_type) 
		{
			case ICF_TRANSPORT_ADDRESS_DOMAIN_NAME:
				ICF_PRINT_MESSAGE(("%s, ", 
					p_get_sys_stg_resp->self_ip_address.addr.addr.domain.str));
				break;
			
			case ICF_TRANSPORT_ADDRESS_IPV4_ADDR:
				ICF_PRINT_MESSAGE(("%u.%u.%u.%u, ", 
					p_get_sys_stg_resp->self_ip_address.addr.addr.ipv4_addr.octet_1, 
					p_get_sys_stg_resp->self_ip_address.addr.addr.ipv4_addr.octet_2, 
					p_get_sys_stg_resp->self_ip_address.addr.addr.ipv4_addr.octet_3, 
					p_get_sys_stg_resp->self_ip_address.addr.addr.ipv4_addr.octet_4));
				break;
		}

		ICF_PRINT_MESSAGE((
				"\n self_port = %u, ",
				  p_get_sys_stg_resp->self_ip_address.port_num));
	}

	if(presence_flag & ICF_REQD_ASSO_URI_LIST)
	{
        icf_print_pub_uri_list(p_get_sys_stg_resp->p_uri_list);
    }
		
	i = 0;
	if(presence_flag & ICF_REQD_APN_LIST)
	{
		while (i != p_get_sys_stg_resp->apn_list.count)
		{
			ICF_PRINT_MESSAGE(("\nApn[%u] = %s" ,
				i,p_get_sys_stg_resp->apn_list.p_list_of_apn[i].str));

			i++;
		}
	}
	if(presence_flag & ICF_REQD_SELF_TRANSPORT_MODE)
	{
		ICF_PRINT_MESSAGE(("\nself_transport_mode = %u" ,
			p_get_sys_stg_resp->self_transport_mode));
	}
	if(presence_flag & ICF_REQD_SELF_PORT)
	{
		ICF_PRINT_MESSAGE(("\nself_port = %u" ,
			p_get_sys_stg_resp->self_port));
	}
	if(presence_flag & ICF_REQD_CALL_THRU_PROXY_STATUS)
	{
		ICF_PRINT_MESSAGE(("\ncall_thru_proxy = %u" ,
			p_get_sys_stg_resp->call_thru_proxy));
	}
	if(presence_flag & ICF_REQD_SIP_COMPACT_HDR_STATUS)
	{
		ICF_PRINT_MESSAGE(("\nsip_compact_header = %u" ,
			p_get_sys_stg_resp->sip_compact_header));
	}
	if(presence_flag & ICF_REQD_QOS_MARK)
	{
		ICF_PRINT_MESSAGE(("\nqos_mark = %u" ,
			p_get_sys_stg_resp->qos_mark));
	}
	if(presence_flag & ICF_REQD_SESSION_INTERVAL_TIMER)
	{
		ICF_PRINT_MESSAGE(("\nsession_interval_timer = %u" ,
			p_get_sys_stg_resp->session_interval_timer));
	}

	if(presence_flag & ICF_REQD_MIN_SESSION_INTERVAL_TIMER)
	{
		ICF_PRINT_MESSAGE(("\nmin_session_interval_timer = %u" ,
			p_get_sys_stg_resp->min_session_interval_timer));
	}

	return ICF_SUCCESS;
}

icf_return_t icf_print_get_line_settings(icf_msg_st* p_msg)
{


    icf_get_line_settings_req_st*      p_get_line_stg_req = ICF_NULL;

    icf_route_info_st*          rt_info = ICF_NULL;

    p_get_line_stg_req = (icf_get_line_settings_req_st*)p_msg->payload;
    rt_info = (icf_route_info_st*)p_get_line_stg_req->routing_info.ptr;

    icf_print_api_header(p_msg);

        
    ICF_PRINT_MESSAGE((
        "\nline-id = %u", p_get_line_stg_req->line_id));

	if (p_get_line_stg_req->bit_mask & ICF_REQD_PROXY_ADDR) 
		ICF_PRINT_MESSAGE(("\nICF_REQD_PROXY_ADDR present"));
	
	if (p_get_line_stg_req->bit_mask & ICF_REQD_REGISTRAR_ADDR)
	    ICF_PRINT_MESSAGE(("\nICF_REQD_REGISTRAR_ADDR present"));
	
		
	if (p_get_line_stg_req->bit_mask & ICF_REQD_PROXY_TRANSPORT_MODE) 
		ICF_PRINT_MESSAGE(("\nICF_REQD_PROXY_TRANSPORT_MODE present "));
	
	if (p_get_line_stg_req->bit_mask & ICF_REQD_SELF_ADDR_LIST) 
		ICF_PRINT_MESSAGE(("\nICF_REQD_SELF_ADDR_LIST present"));
	
	if (p_get_line_stg_req->bit_mask & ICF_REQD_SELF_NAME) 
		ICF_PRINT_MESSAGE(("\nICF_REQD_SELF_NAME present"));
	
	if (p_get_line_stg_req->bit_mask & ICF_REQD_PRESENTATION_STATUS) 
		ICF_PRINT_MESSAGE(("\nICF_REQD_PRESENTATION_STATUS present"));
	
	if (p_get_line_stg_req->bit_mask & ICF_REQD_AUTH_KEY) 
		ICF_PRINT_MESSAGE(("\nICF_REQD_AUTH_KEY present "));
	
	if (p_get_line_stg_req->bit_mask & ICF_REQD_AUTH_PASSWD) 
		ICF_PRINT_MESSAGE(("\nICF_REQD_AUTH_PASSWD present "));
	
	if (p_get_line_stg_req->bit_mask & ICF_REQD_REG_MODE) 
		ICF_PRINT_MESSAGE(("\nICF_REQD_REG_MODE present "));

	if (p_get_line_stg_req->bit_mask & ICF_REQD_REG_DURATION) 
		ICF_PRINT_MESSAGE(("\nICF_REQD_REG_DURATION present "));
/* FIX fro spr 17944 */
	if(p_get_line_stg_req->bit_mask & ICF_GET_SETTINGS_ROUTE_INFO_PRESENT)
	{	
    	ICF_PRINT_MESSAGE((
           "\nrt_info.len = %u", p_get_line_stg_req->routing_info.len));

        if( ICF_NULL != rt_info)
        {
	         icf_print_transport_addr(((icf_transport_addr_st*)&(rt_info->ipc.udp.ip)));
    
             ICF_PRINT_MESSAGE((
                    "\nrt_info.port = %u\n", rt_info->ipc.udp.port));
    	}
     }

    return ICF_SUCCESS;

}


icf_return_t icf_print_get_sys_settings(icf_msg_st* p_msg)
{

    
    icf_get_system_settings_req_st*      p_get_sys_stg_req = ICF_NULL;

    icf_route_info_st*          rt_info = ICF_NULL;
    p_get_sys_stg_req = (icf_get_system_settings_req_st*)p_msg->payload;
    rt_info = (icf_route_info_st*)p_get_sys_stg_req->routing_info.ptr;

    icf_print_api_header(p_msg);


 

    if (p_get_sys_stg_req->bit_mask & ICF_REQD_PRACK_ENABLED_STATUS) 
	    ICF_PRINT_MESSAGE(("\nICF_REQD_PRACK_ENABLED_STATUS present"));

	if (p_get_sys_stg_req->bit_mask & ICF_REQD_AUTH_TYPE_STATUS) 
	    ICF_PRINT_MESSAGE(("\nICF_REQD_AUTH_TYPE_STATUS present"));

	if (p_get_sys_stg_req->bit_mask & ICF_REQD_PRIVACY_SUPPORTED) 
	    ICF_PRINT_MESSAGE(("\nICF_REQD_PRIVACY_SUPPORTED present"));

	if (p_get_sys_stg_req->bit_mask & ICF_REQD_MAX_LINES) 
	    ICF_PRINT_MESSAGE(("\nICF_REQD_MAX_LINES present"));

	if (p_get_sys_stg_req->bit_mask & ICF_REQD_SETUP_TIMER) 
	    ICF_PRINT_MESSAGE(("\nICF_REQD_SETUP_TIMER present"));

	if (p_get_sys_stg_req->bit_mask & ICF_REQD_ALERTING_TIMER) 
	    ICF_PRINT_MESSAGE(("\nICF_REQD_ALERTING_TIMER present"));

	if (p_get_sys_stg_req->bit_mask & ICF_REQD_RELEASE_TIMER) 
	    ICF_PRINT_MESSAGE(("\nICF_REQD_RELEASE_TIMER present"));

	if (p_get_sys_stg_req->bit_mask & ICF_REQD_MODIFY_MEDIA_TIMER) 
	    ICF_PRINT_MESSAGE(("\nICF_REQD_MODIFY_MEDIA_TIMER present"));

	if (p_get_sys_stg_req->bit_mask & ICF_REQD_REG_RESP_TIMER) 
	    ICF_PRINT_MESSAGE(("\nICF_REQD_REG_RESP_TIMER present"));
	
	if (p_get_sys_stg_req->bit_mask & ICF_REQD_REG_RETRY_TIMER) 
	    ICF_PRINT_MESSAGE(("\nICF_REQD_REG_RETRY_TIMER present"));

	if (p_get_sys_stg_req->bit_mask & ICF_REQD_SUBS_RETRY_TIMER) 
	    ICF_PRINT_MESSAGE(("\nICF_REQD_SUBS_RETRY_TIMER present"));

	if (p_get_sys_stg_req->bit_mask & ICF_REQD_SUBS_RESP_TIMER) 
	    ICF_PRINT_MESSAGE(("\nICF_REQD_SUBS_RESP_TIMER present"));

	if (p_get_sys_stg_req->bit_mask & ICF_REQD_SIG_COMP_STATUS) 
	    ICF_PRINT_MESSAGE(("\n ICF_REQD_SIG_COMP_STATUS present"));

	if (p_get_sys_stg_req->bit_mask & ICF_REQD_SECURITY_STATUS) 
	    ICF_PRINT_MESSAGE(("\n ICF_REQD_SECURITY_STATUS present"));

	if (p_get_sys_stg_req->bit_mask & ICF_REQD_SELF_IP_ADDRESS) 
	    ICF_PRINT_MESSAGE(("\n ICF_REQD_SELF_IP_ADDRESS present"));

	if (p_get_sys_stg_req->bit_mask & ICF_REQD_SUBS_RESP_TIMER) 
	    ICF_PRINT_MESSAGE(("\n ICF_REQD_ASSO_URI_LIST present"));

	if (p_get_sys_stg_req->bit_mask & ICF_REQD_APN_LIST) 
	    ICF_PRINT_MESSAGE(("\n ICF_REQD_APN_LIST present"));

	if (p_get_sys_stg_req->bit_mask & ICF_REQD_SELF_TRANSPORT_MODE) 
	    ICF_PRINT_MESSAGE(("\n ICF_REQD_SELF_TRANSPORT_MODE present"));

	if (p_get_sys_stg_req->bit_mask & ICF_REQD_SELF_PORT) 
	    ICF_PRINT_MESSAGE(("\n ICF_REQD_SELF_PORT present"));

	if (p_get_sys_stg_req->bit_mask & ICF_REQD_CALL_THRU_PROXY_STATUS) 
	    ICF_PRINT_MESSAGE(("\n ICF_REQD_CALL_THRU_PROXY_STATUS present"));

	if (p_get_sys_stg_req->bit_mask & ICF_REQD_SIP_COMPACT_HDR_STATUS) 
	    ICF_PRINT_MESSAGE(("\n ICF_REQD_SIP_COMPACT_HDR_STATUS present"));

	if (p_get_sys_stg_req->bit_mask & ICF_REQD_QOS_MARK) 
	    ICF_PRINT_MESSAGE(("\n ICF_REQD_QOS_MARK present"));

	if (p_get_sys_stg_req->bit_mask & ICF_REQD_SESSION_INTERVAL_TIMER) 
	    ICF_PRINT_MESSAGE(("\n ICF_REQD_SESSION_INTERVAL_TIMER present"));

	if (p_get_sys_stg_req->bit_mask & ICF_REQD_MIN_SESSION_INTERVAL_TIMER ) 
	    ICF_PRINT_MESSAGE(("\n ICF_REQD_MIN_SESSION_INTERVAL_TIMER  present"));
	
	ICF_PRINT_MESSAGE((
        "\nrt_info.len = %u", p_get_sys_stg_req->routing_info.len));

	icf_print_transport_addr(((icf_transport_addr_st*)&(rt_info->ipc.udp.ip)));
    ICF_PRINT_MESSAGE((
	    "\nrt_info.port = %u\n", rt_info->ipc.udp.port));
    

    return ICF_SUCCESS;

}


/****************************************************************************
*
*				Function : icf_print_audio_codec_param
*
*				Description : This function is used to print 
*								audio codec parameters
*
*****************************************************************************/ 
icf_return_t  icf_print_audio_codec_param(icf_uint8_t *p_audio_param)	
{
	icf_audio_codec_param_st    *p_icf_audio_codec_param_st = (icf_audio_codec_param_st *)p_audio_param;

	icf_uint8_t			i = 0;
   
	if(p_icf_audio_codec_param_st->bit_mask & ICF_AUDIO_CODEC_PARAM_CHANNEL)
		ICF_PRINT_MESSAGE(("\nChannels = %u",p_icf_audio_codec_param_st->channels));
	
	if(p_icf_audio_codec_param_st->bit_mask & ICF_AUDIO_CODEC_PARAM_SAMPLING_RATE)
		ICF_PRINT_MESSAGE(("\nSampling Rate = %u",p_icf_audio_codec_param_st->sampling_rate));

	if(p_icf_audio_codec_param_st->bit_mask & ICF_AUDIO_CODEC_PARAM_AMR_PARAM)
	{
		ICF_PRINT_MESSAGE(("\nAmr Initial Mode = %u",
					p_icf_audio_codec_param_st->amr_params.amr_initial_mode));

		ICF_PRINT_MESSAGE(("\nCodec Mode = %u",
					p_icf_audio_codec_param_st->amr_params.codec_mode));
		ICF_PRINT_MESSAGE(("\nCrc Enabled = %u",
					p_icf_audio_codec_param_st->amr_params.crc_enabled));
		if(ICF_AMR_MODE_LIST_PRESENT & p_icf_audio_codec_param_st->amr_params.bit_mask)
		{
			ICF_PRINT_MESSAGE(("\nAMR Mode Count = %u",
					p_icf_audio_codec_param_st->amr_params.amr_mode_count));
			for(i=0;i<p_icf_audio_codec_param_st->amr_params.amr_mode_count;i++)
				ICF_PRINT_MESSAGE(("%u\n",
					p_icf_audio_codec_param_st->amr_params.amr_mode_list[i]));
		}
	}

	return ICF_SUCCESS;
}

/****************************************************************************
*
*				Function : icf_print_video_codec_param
*
*				Description : This function is used to print 
*								video codec parameters
*
*****************************************************************************/ 
icf_return_t  icf_print_video_codec_param(icf_uint8_t *p_video_param)	
{
	icf_video_codec_param_st    *p_icf_video_codec_param_st = (icf_video_codec_param_st *)p_video_param;
   
	if(p_icf_video_codec_param_st->bit_mask & ICF_VIDEO_CODEC_CLOCK_RATE_PRESENT)
		ICF_PRINT_MESSAGE(("\nClock rate = %u",p_icf_video_codec_param_st->clock_rate));

	if(p_icf_video_codec_param_st->bit_mask & ICF_VIDEO_CODEC_FRAMESIZE_PRESENT)
	{
		ICF_PRINT_MESSAGE(("\nFrame Height = %u",p_icf_video_codec_param_st->frame_size.height));
		ICF_PRINT_MESSAGE(("\nFrame Width = %u",p_icf_video_codec_param_st->frame_size.width));
	}
	
	return ICF_SUCCESS;
}

/****************************************************************************
*
*				Function : icf_print_t38_attrib_param
*
*				Description : This function is used to print 
*								fax parameters
*
*****************************************************************************/ 
icf_return_t  icf_print_t38_attrib_param(icf_uint8_t *p_t38_param)	
{
	icf_t38_attrib_param_st    *p_icf_t38_codec_param_st = (icf_t38_attrib_param_st *)p_t38_param;
   
	if(p_icf_t38_codec_param_st->bit_mask & ICF_FAX_T38_VERSION)
		ICF_PRINT_MESSAGE(("\nFax Version = %u",p_icf_t38_codec_param_st->fax_version));

	if(p_icf_t38_codec_param_st->bit_mask & ICF_FAX_T38_BITRATE)
		ICF_PRINT_MESSAGE(("\nBitrate = %u",p_icf_t38_codec_param_st->bitrate));

	if(p_icf_t38_codec_param_st->bit_mask & ICF_FAX_T38_RATE_MANAGEMENT)
		ICF_PRINT_MESSAGE(("\nRate Management = %u",p_icf_t38_codec_param_st->rate_management));

	if(p_icf_t38_codec_param_st->bit_mask & ICF_FAX_T38_UDP_BUFFSIZE)
		ICF_PRINT_MESSAGE(("\nUdp Buffersize = %u",p_icf_t38_codec_param_st->udp_buffersize));

	if(p_icf_t38_codec_param_st->bit_mask & ICF_FAX_T38_UDP_PACKETSIZE)
		ICF_PRINT_MESSAGE(("\nUdp Packetsize = %u",p_icf_t38_codec_param_st->udp_packetsize));

	if(p_icf_t38_codec_param_st->bit_mask & ICF_FAX_T38_ERR_CORRECTION)
		ICF_PRINT_MESSAGE(("\nErr Correction Method = %u",
				p_icf_t38_codec_param_st->err_correction_method));

	return ICF_SUCCESS;
}

/****************************************************************************
*
*				Function : icf_print_audio_profile
*
*				Description : This function is used to print 
*								audio profile.
*
*****************************************************************************/ 
icf_return_t icf_print_audio_profile(icf_uint8_t *p_audio_profile)
{
	icf_audio_profile_st    *p_audio_profile_st = (icf_audio_profile_st *)p_audio_profile;
   
	if(p_audio_profile_st->bit_mask & ICF_AUDIO_PROF_BNDWDTH_PRESENT)
		ICF_PRINT_MESSAGE(("\nBandwidth = %u",p_audio_profile_st->bandwidth));
	
	if(p_audio_profile_st->bit_mask & ICF_AUDIO_PROF_PTIME_PRESENT)
		ICF_PRINT_MESSAGE(("\nPref ptime = %u",p_audio_profile_st->pref_ptime));
	
	if(p_audio_profile_st->bit_mask & ICF_AUDIO_PROF_SILN_SUPP_PRESENT)
		ICF_PRINT_MESSAGE(("\nSilence Supp = %u",p_audio_profile_st->silence_suppression));
	
	return ICF_SUCCESS;
}

/****************************************************************************
*
*				Function : icf_print_video_profile
*
*				Description : This function is used to print 
*								video profile.
*
*****************************************************************************/ 
icf_return_t icf_print_video_profile(icf_uint8_t *p_video_profile)
{
	icf_video_profile_st    *p_video_profile_st = (icf_video_profile_st *)p_video_profile;
   
	if(p_video_profile_st->bit_mask & ICF_VIDEO_PROFILE_ENC_QUALITY_PRESENT)
		ICF_PRINT_MESSAGE(("\nEncoding Quality = %u",p_video_profile_st->encoding_quality));
	
	if(p_video_profile_st->bit_mask & ICF_VIDEO_PROFILE_FRAME_RATE_PRESENT)
		ICF_PRINT_MESSAGE(("\nFrame Rate = %u",p_video_profile_st->frame_rate));
	
	if(p_video_profile_st->bit_mask & ICF_VIDEO_PROFILE_BANDWIDTH_PRESENT)
		ICF_PRINT_MESSAGE(("\nBandwidth = %u",p_video_profile_st->bandwidth));
	
	return ICF_SUCCESS;
}
/****************************************************************************
*
*				Function : icf_print_stream_capab
*
*				Description : This function is used to print 
*								stream_capab_st structure.
*
*****************************************************************************/ 
icf_return_t icf_print_stream_capab(icf_stream_capab_st* p_stream_capab_st)
{
	icf_uint8_t		j=0,i = 1;
	icf_list_st		*p_codec_info;
	icf_list_st		*p_encoded_att;

	ICF_PRINT_MESSAGE(("\nStream Id = %u\n",p_stream_capab_st->stream_id));
	ICF_PRINT_MESSAGE(("\nStream Type = %u\n",p_stream_capab_st->stream_type));

	if(p_stream_capab_st->bit_mask & ICF_STREAM_CAPAB_CODEC_INFO_PRESENT)
	{
		p_codec_info = p_stream_capab_st->p_codec_info_list;
		
		while(p_codec_info != ICF_NULL)
		{
			ICF_PRINT_MESSAGE(("\nCodec %u",i));
			ICF_PRINT_MESSAGE(("\nCodec Num = %u",
				((icf_codec_attrib_st*)p_codec_info->p_data)->codec_num));
			ICF_PRINT_MESSAGE(("\nCodec Name = "));
			for(j=0;j<(((icf_codec_attrib_st*)p_codec_info->p_data)->codec_name).str_len;j++)
			{
				ICF_PRINT_MESSAGE(("%c",
						((icf_codec_attrib_st*)p_codec_info->p_data)->codec_name.str[j]));
			}

			switch(p_stream_capab_st->stream_type)
			{
				case  ICF_STREAM_TYPE_AUDIO:
					  icf_print_audio_codec_param((icf_uint8_t*)&((icf_codec_attrib_st*)p_codec_info->p_data)->codec_params);			  
					  break;

				case  ICF_STREAM_TYPE_VIDEO:
					  icf_print_video_codec_param((icf_uint8_t*)&((icf_codec_attrib_st*)p_codec_info->p_data)->codec_params);			  
					  break;

				case  ICF_STREAM_TYPE_FAX:
					   icf_print_t38_attrib_param((icf_uint8_t*)&((icf_codec_attrib_st*)p_codec_info->p_data)->codec_params);			  
					  break;
			}
		
			ICF_PRINT_MESSAGE(("\nList of encoded attributes for codec\n"));

			p_encoded_att = ((icf_codec_attrib_st*)p_codec_info->p_data)->p_encoded_attributes;
			while(p_encoded_att != ICF_NULL)
			{
				ICF_PRINT_MESSAGE((" "));
				for(i=0;i<((icf_string_st*)(p_encoded_att->p_data))->str_len;i++)
				{
					ICF_PRINT_MESSAGE(("%c",
							((icf_string_st*)p_encoded_att->p_data)->str[i]));
				}
				p_encoded_att = p_encoded_att->p_next;
			}

			p_codec_info = p_codec_info->p_next;
			i++;
		}
	}

	if(p_stream_capab_st->bit_mask & ICF_STREAM_CAPAB_MEDIA_STREAM_PARAM_PRESENT)
	{
		ICF_PRINT_MESSAGE(("\nMedia Stream Parameters"));
		switch(p_stream_capab_st->stream_type)
		{
			case  ICF_STREAM_TYPE_AUDIO:
				  icf_print_audio_profile((icf_uint8_t*)&(p_stream_capab_st->media_stream_param));			  
				  break;

			case  ICF_STREAM_TYPE_VIDEO:
				  icf_print_video_profile((icf_uint8_t*)&(p_stream_capab_st->media_stream_param));	  
				  break;
		}
	}

	if(p_stream_capab_st->bit_mask & ICF_STREAM_CAPAB_ENC_ATTRIB_PRESENT)
	{
		p_encoded_att = p_stream_capab_st->p_encoded_attributes;
		ICF_PRINT_MESSAGE(("\nList of encoded attributes\n"));

		while(p_encoded_att != ICF_NULL)
		{
			ICF_PRINT_MESSAGE((" "));
			for(i=0;i<((icf_string_st*)(p_encoded_att->p_data))->str_len;i++)
			{
				ICF_PRINT_MESSAGE(("%c",
						((icf_string_st*)p_encoded_att->p_data)->str[i]));
			}

			p_encoded_att = p_encoded_att->p_next;
		}
	}

	return ICF_SUCCESS;
}

/****************************************************************************
*
*				Function : icf_print_modify_media_change_req
*
*				Description : This function is used to print 
*								stream_capab_st structure.
*
*****************************************************************************/ 
icf_return_t icf_print_modify_media_change_req(icf_call_modify_media_data_st* p_modify_media_change_req)
{
	icf_uint8_t		i = 1;
	icf_list_st		*p_codec_info;
	icf_list_st		*p_encoded_att;

	ICF_PRINT_MESSAGE(("\nStream Id = %u",p_modify_media_change_req->stream_id));
	ICF_PRINT_MESSAGE(("\nStream Type = %u",p_modify_media_change_req->stream_type));

	if(p_modify_media_change_req->bit_mask & ICF_CALL_MOD_CODEC_INFO_PRESENT) 
	{
		p_codec_info = p_modify_media_change_req->p_codec_info_list;
		
		while(p_codec_info != ICF_NULL)
		{
			ICF_PRINT_MESSAGE(("\nCodec %u",i));
			ICF_PRINT_MESSAGE(("\nCodec Num = %u",
				((icf_codec_attrib_st*)p_codec_info->p_data)->codec_num));
			ICF_PRINT_MESSAGE(("\nCodec Name = "));

			for(i=0;i<(((icf_codec_attrib_st*)p_codec_info->p_data)->codec_name).str_len;i++)
				{
					ICF_PRINT_MESSAGE(("%c",
							((icf_codec_attrib_st*)p_codec_info->p_data)->codec_name.str[i]));
				}

			switch(p_modify_media_change_req->stream_type)
			{
				case  ICF_STREAM_TYPE_AUDIO:
					  icf_print_audio_codec_param((icf_uint8_t*)&((icf_codec_attrib_st*)p_codec_info->p_data)->codec_params);			  
					  break;

				case  ICF_STREAM_TYPE_VIDEO:
					  icf_print_video_codec_param((icf_uint8_t*)&((icf_codec_attrib_st*)p_codec_info->p_data)->codec_params);			  
					  break;

				case  ICF_STREAM_TYPE_FAX:
					   icf_print_t38_attrib_param((icf_uint8_t*)&((icf_codec_attrib_st*)p_codec_info->p_data)->codec_params);			  
					  break;
			}
		
			ICF_PRINT_MESSAGE(("\nList of encoded attributes for codec\n"));

			p_encoded_att = ((icf_codec_attrib_st*)p_codec_info->p_data)->p_encoded_attributes;
			while(p_encoded_att != ICF_NULL)
			{
				ICF_PRINT_MESSAGE((" "));
				for(i=0;i<((icf_string_st*)(p_encoded_att->p_data))->str_len;i++)
				{
					ICF_PRINT_MESSAGE(("%c",
							((icf_string_st*)p_encoded_att->p_data)->str[i]));
				}

				p_encoded_att = p_encoded_att->p_next;
			}

			p_codec_info = p_codec_info->p_next;
			i++;
		}
	}


	if(p_modify_media_change_req->bit_mask & ICF_CALL_MOD_MEDIA_STREAM_PARAM_PRESENT)
	{
		ICF_PRINT_MESSAGE(("\nMedia Stream Parameters\n"));
		switch(p_modify_media_change_req->stream_type)
		{
			case  ICF_STREAM_TYPE_AUDIO:
				  icf_print_audio_profile((icf_uint8_t*)&(p_modify_media_change_req->media_stream_param));			  
				  break;

			case  ICF_STREAM_TYPE_VIDEO:
				  icf_print_video_profile((icf_uint8_t*)&(p_modify_media_change_req->media_stream_param));	  
				  break;
		}
	}

	if(p_modify_media_change_req->bit_mask & ICF_CALL_MOD_ENC_ATTRIB_PRESENT)
	{
		p_encoded_att = p_modify_media_change_req->p_encoded_attributes;
		ICF_PRINT_MESSAGE(("\nList of encoded attributes\n"));

		while(p_encoded_att != ICF_NULL)
		{
			ICF_PRINT_MESSAGE((" "));
			for(i=0;i<((icf_string_st*)(p_encoded_att->p_data))->str_len;i++)
			{
				ICF_PRINT_MESSAGE(("%c",
						((icf_string_st*)p_encoded_att->p_data)->str[i]));
			}

		p_encoded_att = p_encoded_att->p_next;
		}
	}

	if(p_modify_media_change_req->bit_mask & ICF_CALL_MOD_MEDIA_MODE_PRESENT)
	{
		ICF_PRINT_MESSAGE((" media mode = %u",p_modify_media_change_req->media_mode));
	}

	return ICF_SUCCESS;
}
/****************************************************************************
*
*				Function : icf_print_stream_sdp
*
*				Description : This function is used to print 
*								stream_sdp_st structure.
*
*****************************************************************************/ 
icf_return_t icf_print_stream_sdp(icf_sdp_stream_st* p_sdp_stream_st,icf_media_type_t stream_type)
{
	icf_uint8_t		j=0,i = 1;
	icf_list_st		*p_codec_info;
	icf_list_st		*p_encoded_att;

	{

		p_codec_info = p_sdp_stream_st->p_codec_info_list;
		
		while(p_codec_info != ICF_NULL)
		{
			ICF_PRINT_MESSAGE(("\nCodec %u",i));
			ICF_PRINT_MESSAGE(("\nCodec Num = %u",
				((icf_codec_attrib_st*)p_codec_info->p_data)->codec_num));

			for(j=0;j<((icf_codec_attrib_st*)p_codec_info->p_data)->codec_name.str_len;j++)
			{
				ICF_PRINT_MESSAGE(("%c",
						((icf_codec_attrib_st*)p_codec_info->p_data)->codec_name.str[j]));
			}

		
			
			switch(stream_type)
			{
				case  ICF_STREAM_TYPE_AUDIO:
					  icf_print_audio_codec_param((icf_uint8_t*)&((icf_codec_attrib_st*)p_codec_info->p_data)->codec_params);			  
					  break;

				case  ICF_STREAM_TYPE_VIDEO:
					  icf_print_video_codec_param((icf_uint8_t*)&((icf_codec_attrib_st*)p_codec_info->p_data)->codec_params);			  
					  break;

				case  ICF_STREAM_TYPE_FAX:
					   icf_print_t38_attrib_param((icf_uint8_t*)&((icf_codec_attrib_st*)p_codec_info->p_data)->codec_params);			  
					  break;
			}

			p_encoded_att = ((icf_codec_attrib_st*)p_codec_info->p_data)->p_encoded_attributes;
			
			if(p_encoded_att != ICF_NULL)
			{
				ICF_PRINT_MESSAGE(("\nList of encoded attributes for codec\n"));
			}
			while(p_encoded_att != ICF_NULL)
			{
				for(j=0;j<((icf_string_st*)(p_encoded_att->p_data))->str_len;j++)
				{
					ICF_PRINT_MESSAGE(("%c",
							((icf_string_st*)p_encoded_att->p_data)->str[j]));
				}

				p_encoded_att = p_encoded_att->p_next;
			}

			p_codec_info = p_codec_info->p_next;
			i++;
		}
	}

	ICF_PRINT_MESSAGE(("\n Stream Address "));
	icf_print_transport_addr(((icf_transport_addr_st*)&(p_sdp_stream_st->stream_address.addr)));
    
	ICF_PRINT_MESSAGE((
        "\n stream_info.port = %u", p_sdp_stream_st->stream_address.port_num));

	
	ICF_PRINT_MESSAGE(("\n Transport Mode "));
	ICF_PRINT_MESSAGE((
        "\n transport_mode = %u", p_sdp_stream_st->trans_mode));

	if(p_sdp_stream_st->bit_mask & ICF_SDP_STREAM_MEDIA_STREAM_PRESENT)
	{
		ICF_PRINT_MESSAGE(("\nMedia Stream Parameters \n"));
		switch(stream_type)
		{
			case  ICF_STREAM_TYPE_AUDIO:
				  icf_print_audio_profile((icf_uint8_t*)&(p_sdp_stream_st->media_stream_param));			  
				  break;

			case  ICF_STREAM_TYPE_VIDEO:
				  icf_print_video_profile((icf_uint8_t*)&(p_sdp_stream_st->media_stream_param));	  
				  break;
		}
	}

	if(p_sdp_stream_st->bit_mask & ICF_SDP_STREAM_RTCP_INFO_PRESENT)
	{
		ICF_PRINT_MESSAGE(("\n RTCP Address "));
		icf_print_transport_addr(((icf_transport_addr_st*)&(p_sdp_stream_st->rtcp_address.addr)));
    
		ICF_PRINT_MESSAGE((
			"\n rtcp_info.port = %u", p_sdp_stream_st->rtcp_address.port_num));
	}


	if(p_sdp_stream_st->bit_mask & ICF_SDP_STREAM_ENCPARAMS_PRESENT)
	{
		p_encoded_att = p_sdp_stream_st->p_encoded_attributes;
		ICF_PRINT_MESSAGE(("\nList of encoded attributes\n"));

		while(p_encoded_att != ICF_NULL)
		{
			for(j=0;j<((icf_string_st*)(p_encoded_att->p_data))->str_len;j++)
			{
				ICF_PRINT_MESSAGE(("%c",
						((icf_string_st*)p_encoded_att->p_data)->str[j]));
			}

		p_encoded_att = p_encoded_att->p_next;
		}
	}

	return ICF_SUCCESS;
}

/****************************************************************************
*
*				Function : icf_print_resp_stream
*
*				Description : This function is used to print 
*								stream_capab_st structure.
*
*****************************************************************************/ 
icf_return_t icf_print_resp_stream(icf_resp_stream_st* p_resp_stream_st, icf_uint8_t i)
{
    ICF_PRINT_MESSAGE(("\n\tstream_data[%u].stream_type = %u"
        "\n\tstream_data[%u].stream_id = %u\n", i, p_resp_stream_st->stream_type,
        i, p_resp_stream_st->stream_id));

    ICF_PRINT_MESSAGE(("\n\t stream_data[%u].local_sdp",i));
    icf_print_stream_sdp(&(p_resp_stream_st->local_sdp),p_resp_stream_st->stream_type);
	
   
	return ICF_SUCCESS;
}



icf_return_t icf_print_app_add_req(icf_msg_st* p_msg)
{


    icf_uint8_t				header_name_list_count = 0;
    
    icf_app_add_req_st*      p_app_req = ICF_NULL;
    icf_route_info_st*       rt_info = ICF_NULL;
	icf_filter_st			 *p_filter_list=ICF_NULL;
	icf_uint32_t			 i;
    
	icf_list_st				 *p_media_profile_list = ICF_NULL;
    p_app_req = (icf_app_add_req_st*)p_msg->payload;
    rt_info = (icf_route_info_st*)p_app_req->routing_info.ptr;

    icf_print_api_header(p_msg);


 /* TBD */
#ifdef ICF_REL_OLD
    ICF_PRINT_MESSAGE((
        "\ncall_type = %u, app_name.str = %s"
        "\nrt_info.len = %u", 
        p_app_req->call_type, p_app_req->app_name.str, 
		p_app_req->routing_info.len);
#endif
    ICF_PRINT_MESSAGE((
        "\nfilter.count = %u, filter.match_all = %u", 
        p_app_req->filter.count, p_app_req->filter.match_all));

	
	for(i=0,p_filter_list=p_app_req->filter.p_filter_list;i<p_app_req->filter.count;i++)
	{
	    ICF_PRINT_MESSAGE(("\nFilter%u\n  filter_type = %u\n  match_type = %u\n  target_header = %s\n  expr_str = %s", 
		i+1,
        p_filter_list->filter_type,
		p_filter_list->match_type,
		p_filter_list->target_header.str,
		p_filter_list->expr_str.str));

        		p_filter_list=p_filter_list->p_next;
	}

    icf_print_transport_addr(((icf_transport_addr_st*)&(rt_info->ipc.udp.ip)));
    
    ICF_PRINT_MESSAGE((
        "\nrt_info.port = %u", rt_info->ipc.udp.port));
    
	
    header_name_list_count = (icf_uint8_t)p_app_req->header_name_list.count;
    icf_print_header_name_list(p_app_req->header_name_list.hdr, header_name_list_count);

    icf_print_string_list(&(p_app_req->supported_methods),(icf_uint8_t *)"supported_methods");

    icf_print_string_list(&(p_app_req->supported_events),(icf_uint8_t *)"supported_events");

    icf_print_string_list(&(p_app_req->supported_content_types),(icf_uint8_t *)"supported_content_types");
    
    icf_print_string_list(&(p_app_req->supported_encodings),(icf_uint8_t *)"supported_encodings");

    icf_print_string_list(&(p_app_req->supported_languages),(icf_uint8_t *)"supported_languages");
    


	ICF_PRINT_MESSAGE((
        "\nMedia Profile List:"));
	p_media_profile_list= p_app_req->p_media_profile;
	i = 1;
	while(p_media_profile_list!=ICF_NULL)
	{
		ICF_PRINT_MESSAGE(("\nNode %u:",i));
		icf_print_stream_capab(p_media_profile_list->p_data);
		p_media_profile_list = p_media_profile_list->p_next;
		i++;
	}



     ICF_PRINT_MESSAGE((
        "\nrtp_detection_reqd = %u", 
        p_app_req->rtp_detection_reqd));


    return ICF_SUCCESS;
}




icf_return_t icf_print_app_add_cfm(icf_msg_st* p_msg)
{
    icf_app_add_cfm_st*      p_app_cfm = ICF_NULL;
    
    p_app_cfm = (icf_app_add_cfm_st*)p_msg->payload;

    icf_print_api_header(p_msg);
    /* g_default_app_id = p_msg->hdr.app_id; */

   

    ICF_PRINT_MESSAGE((
        "bit_mask = %u, result = %u, error_cause = %u",
        p_app_cfm->bit_mask, p_app_cfm->result, p_app_cfm->error_cause));
    icf_print_error_details(p_app_cfm->error_cause);

#ifdef ICF_NW_ACTIVATE

	if (p_app_cfm->bit_mask & ICF_APP_ADD_CFM_NET_INFO_PRESENT) {

		num_of_elements = p_app_cfm->num_nw;
		
		ICF_PRINT_MESSAGE(("\nnum_nw = %u", num_of_elements);
		

		for (i = 0; i < num_of_elements; i++) {
			ICF_PRINT_MESSAGE((
				"\nnw_info[%u].bit_mask = %u", 
				i, p_app_cfm->nw_info[i].bit_mask);

            if (p_app_cfm->nw_info[i].bit_mask & ICF_NET_INFO_CTXT_ID_PRESENT) {
    			ICF_PRINT_MESSAGE(("\nnw_info[%u].ctxt_id = %u", 
                    i,p_app_cfm->nw_info[i].ctxt_id));
            }
			
            if (p_app_cfm->nw_info[i].bit_mask & ICF_NET_INFO_SELF_IP_PRESENT) {
    			ICF_PRINT_MESSAGE(("\nnw_info[%u].self_ip", i));
				icf_print_transport_addr(&(p_app_cfm->nw_info[i].self_ip));
            }
			
			if (p_app_cfm->nw_info[i].bit_mask & ICF_NET_INFO_ACCESS_TYPE_PRESENT)
                ICF_PRINT_MESSAGE((
                    "\nnw_info[%u].access_type = %u", 
                    i, p_app_cfm->nw_info[i].access_type));

			if (p_app_cfm->nw_info[i].bit_mask & ICF_NET_INFO_ACCESS_INFO_PRESENT)
                ICF_PRINT_MESSAGE((
                    "\nnw_info[%u].access_info.str = %s", 
                    i, p_app_cfm->nw_info[i].access_info.str));
            

			if (p_app_cfm->nw_info[i].bit_mask & ICF_NET_INFO_ASSO_URI_PRESENT) 
            {
                icf_print_pub_uri_list(p_app_cfm->nw_info[i].p_uri_list);
            }
			
			if (p_app_cfm->nw_info[i].bit_mask & ICF_NET_INFO_OUT_PROXY_ADDR_PRESENT) {
				asso_num = p_app_cfm->nw_info[i].out_proxy_addr.num_of_elements;
				
				
				for (j = 0; j < asso_num; j++) {
					ICF_PRINT_MESSAGE((
						"\nnw_info[%u].out_proxy_addr.addr", j);
					icf_print_transport_addr(&(p_app_cfm->nw_info[i].out_proxy_addr.addr[i]));
										
				}
			}

			
		}
	}

#endif

    return ICF_SUCCESS;

}




icf_return_t icf_print_app_remove_req(icf_msg_st* p_msg)
{

   
    icf_app_remove_req_st*      p_app_rem = ICF_NULL;


    p_app_rem = (icf_app_remove_req_st*)p_msg->payload;

    icf_print_api_header(p_msg);

    ICF_PRINT_MESSAGE(("\noption = %u", p_app_rem->option));
  
    return ICF_SUCCESS;

}





icf_return_t icf_print_app_remove_cfm(icf_msg_st* p_msg)
{

    
    icf_app_remove_cfm_st*      p_app_cfm = ICF_NULL;
 
    p_app_cfm = (icf_app_remove_cfm_st*)p_msg->payload;

    icf_print_api_header(p_msg);




    ICF_PRINT_MESSAGE((
        "\nresult = %u, error_cause = %u",
        p_app_cfm->result, p_app_cfm->error_cause));
    icf_print_error_details(p_app_cfm->error_cause);


    return ICF_SUCCESS;

}



#ifdef ICF_NW_ACTIVATE

icf_return_t icf_print_network_activate_req(icf_msg_st* p_msg)
{


    icf_network_activate_req_st*      p_net_req = ICF_NULL;
    icf_route_info_st*          rt_info = ICF_NULL;
    p_net_req = (icf_network_activate_req_st*)p_msg->payload;
    rt_info = (icf_route_info_st*)p_net_req->routing_info.ptr;

    icf_print_api_header(p_msg);



    ICF_PRINT_MESSAGE((
        "\nsource_id = %u, dest_id = %u"
        "\nversion = %u, call_id = %u"
        "\naccess_type = %u, access_info.str = %s"
        "\nrt_info len = %u, rt_info type = %u"
        "\nrt_info addr_type = %u, rt_info octet1 = %u"
        "\nrt_info octet2 = %u, rt_info octet3 = %u"
        "\nrt_info octet4 = %u, rt_info port = %u",
         p_msg->hdr.source_id, p_msg->hdr.destination_id, 
        p_msg->hdr.version, p_msg->hdr.call_id, 
        p_net_req->access_type, p_net_req->access_info.str, 
		p_net_req->routing_info.len, rt_info->type,
        rt_info->ipc.udp.ip.addr_type, rt_info->ipc.udp.ip.addr.ipv4_addr.octet_1,
        rt_info->ipc.udp.ip.addr.ipv4_addr.octet_2, rt_info->ipc.udp.ip.addr.ipv4_addr.octet_3,
        rt_info->ipc.udp.ip.addr.ipv4_addr.octet_4, rt_info->ipc.udp.port);


    return ICF_SUCCESS;

}




icf_return_t icf_print_event_indication(icf_msg_st* p_msg)
{

    icf_event_indication_st*      p_event_ind = ICF_NULL;
    
    p_event_ind = (icf_event_indication_st*)p_msg->payload;

    icf_print_api_header(p_msg);




    ICF_PRINT_MESSAGE((
        "\nevent_code = %u",
        p_event_ind->event_code));


    return ICF_SUCCESS;

}




icf_return_t icf_print_network_activate_cfm(icf_msg_st* p_msg)
{


    
    icf_network_activate_cfm_st*      p_net_cfm = ICF_NULL;
    icf_uint8_t  i = 0, num_of_elements = 0, asso_num = 0, j = 0;

    p_net_cfm = (icf_network_activate_cfm_st*)p_msg->payload;

    icf_print_api_header(p_msg);



    ICF_PRINT_MESSAGE((
        "\nbit_mask = %u, result = %u, error_cause = %u",
        p_net_cfm->bit_mask, p_net_cfm->result, p_net_cfm->error_cause));
    icf_print_error_details(p_net_cfm->error_cause);




	if (p_net_cfm->bit_mask & ICF_NET_ACT_CFM_NW_INFO_PRESENT) {

		num_of_elements = p_net_cfm->num_nw;
		
		ICF_PRINT_MESSAGE(("\nnum_nw = %u", num_of_elements);
		

		for (i = 0; i < num_of_elements; i++) {
			ICF_PRINT_MESSAGE((
				"\nnw_info[%u].bit_mask = %u", 
				i, p_net_cfm->nw_info[i].bit_mask);

            if (p_net_cfm->nw_info[i].bit_mask & ICF_NET_INFO_CTXT_ID_PRESENT) {
    			ICF_PRINT_MESSAGE(("\nnw_info[%u].ctxt_id = %u", 
                    i,p_net_cfm->nw_info[i].ctxt_id));
            }
			
            if (p_net_cfm->nw_info[i].bit_mask & ICF_NET_INFO_SELF_IP_PRESENT) {
    			ICF_PRINT_MESSAGE(("\nnw_info[%u].self_ip", i));
				icf_print_transport_addr(&(p_net_cfm->nw_info[i].self_ip));
            }
			
			if (p_net_cfm->nw_info[i].bit_mask & ICF_NET_INFO_ACCESS_TYPE_PRESENT)
                ICF_PRINT_MESSAGE((
                    "\nnw_info[%u].access_type = %u", 
                    i, p_net_cfm->nw_info[i].access_type));

			if (p_net_cfm->nw_info[i].bit_mask & ICF_NET_INFO_ACCESS_INFO_PRESENT)
                ICF_PRINT_MESSAGE((
                    "\nnw_info[%u].access_info.str = %s", 
                    i, p_net_cfm->nw_info[i].access_info.str));
            

            if (p_net_cfm->nw_info[i].bit_mask & ICF_NET_INFO_ASSO_URI_PRESENT) 
            {
                icf_print_pub_uri_list(p_net_cfm->nw_info[i].p_uri_list);
            }

			
			if (p_net_cfm->nw_info[i].bit_mask & ICF_NET_INFO_OUT_PROXY_ADDR_PRESENT) {
				asso_num = p_net_cfm->nw_info[i].out_proxy_addr.num_of_elements;
				
				
				for (j = 0; j < asso_num; j++) {
					ICF_PRINT_MESSAGE((
						"\nnw_info[%u].out_proxy_addr.addr", j);
					icf_print_transport_addr(&(p_net_cfm->nw_info[i].out_proxy_addr.addr[i]));
					ICF_PRINT_MESSAGE(("\n"));
					
				}
			}

			
		}
	}


 
    return ICF_SUCCESS;

}





icf_return_t icf_print_network_deactivate_req(icf_msg_st* p_msg)
{



    icf_network_deactivate_req_st*      p_net_req = ICF_NULL;
    icf_route_info_st*          rt_info = ICF_NULL;
    p_net_req = (icf_network_deactivate_req_st*)p_msg->payload;
    rt_info = (icf_route_info_st*)p_net_req->routing_info.ptr;

    icf_print_api_header(p_msg);



    ICF_PRINT_MESSAGE((
        "\nsource_id = %u, dest_id = %u"
        "\nversion = %u, call_id = %u"
        "\nctxt id = %u"
        "\nrt_info len = %u, rt_info type = %u"
        "\nrt_info addr_type = %u, rt_info octet1 = %u"
        "\nrt_info octet2 = %u, rt_info octet3 = %u"
        "\nrt_info octet4 = %u, rt_info port = %u",     
        p_msg->hdr.source_id, p_msg->hdr.destination_id, 
        p_msg->hdr.version, p_msg->hdr.call_id, 
        p_net_req->ctxt_id, 
		p_net_req->routing_info.len, rt_info->type,
        rt_info->ipc.udp.ip.addr_type, rt_info->ipc.udp.ip.addr.ipv4_addr.octet_1,
        rt_info->ipc.udp.ip.addr.ipv4_addr.octet_2, rt_info->ipc.udp.ip.addr.ipv4_addr.octet_3,
        rt_info->ipc.udp.ip.addr.ipv4_addr.octet_4, rt_info->ipc.udp.port);



    return ICF_SUCCESS;

}




icf_return_t icf_print_network_deactivate_cfm(icf_msg_st* p_msg)
{

    
    icf_network_deactivate_cfm_st*      p_net_cfm = ICF_NULL;
    icf_uint8_t  i = 0, num_of_elements = 0, asso_num = 0, j = 0;

    p_net_cfm = (icf_network_deactivate_cfm_st*)p_msg->payload;

    icf_print_api_header(p_msg);




    ICF_PRINT_MESSAGE((
        "\nbit_mask = %u, result = %u, error_cause = %u",
        p_net_cfm->bit_mask, p_net_cfm->result, p_net_cfm->error_cause));
    icf_print_error_details(p_net_cfm->error_cause);




	if (p_net_cfm->bit_mask & ICF_NET_DEACT_CFM_NW_INFO_PRESENT) {

		num_of_elements = p_net_cfm->num_nw;
		
		ICF_PRINT_MESSAGE(("\nnum_nw = %u", num_of_elements);
		

		for (i = 0; i < num_of_elements; i++) {
			ICF_PRINT_MESSAGE((
				"\nnw_info[%u].bit_mask = %u", 
				i, p_net_cfm->nw_info[i].bit_mask);

            if (p_net_cfm->nw_info[i].bit_mask & ICF_NET_INFO_CTXT_ID_PRESENT) {
    			ICF_PRINT_MESSAGE(("\nnw_info[%u].ctxt_id = %u", 
                    i,p_net_cfm->nw_info[i].ctxt_id));
            }
			
            if (p_net_cfm->nw_info[i].bit_mask & ICF_NET_INFO_SELF_IP_PRESENT) {
    			ICF_PRINT_MESSAGE(("\nnw_info[%u].self_ip", i));
				icf_print_transport_addr(&(p_net_cfm->nw_info[i].self_ip));
            }
			
			if (p_net_cfm->nw_info[i].bit_mask & ICF_NET_INFO_ACCESS_TYPE_PRESENT)
                ICF_PRINT_MESSAGE((
                    "\nnw_info[%u].access_type = %u", 
                    i, p_net_cfm->nw_info[i].access_type));

			if (p_net_cfm->nw_info[i].bit_mask & ICF_NET_INFO_ACCESS_INFO_PRESENT)
                ICF_PRINT_MESSAGE((
                    "\nnw_info[%u].access_info.str = %s", 
                    i, p_net_cfm->nw_info[i].access_info.str));
            

            if (p_net_cfm->nw_info[i].bit_mask & ICF_NET_INFO_ASSO_URI_PRESENT) 
            {
                icf_print_pub_uri_list(p_net_cfm->nw_info[i].p_uri_list);
            }

			
			if (p_net_cfm->nw_info[i].bit_mask & ICF_NET_INFO_OUT_PROXY_ADDR_PRESENT) {
				asso_num = p_net_cfm->nw_info[i].out_proxy_addr.num_of_elements;
				
				
				for (j = 0; j < asso_num; j++) {
					ICF_PRINT_MESSAGE((
						"\nnw_info[%u].out_proxy_addr.addr", j);
					icf_print_transport_addr(&(p_net_cfm->nw_info[i].out_proxy_addr.addr[i]));
										
				}
			}

			
		}
	}


    return ICF_SUCCESS;

}


#endif



icf_return_t icf_print_register_req(icf_msg_st* p_msg)
{

    icf_register_req_st* p_reg_req = ICF_NULL;

    icf_uint8_t  header_list_count = 0, tag_list_count = 0,
                    body_list_count = 0;
    p_reg_req = (icf_register_req_st*)p_msg->payload;

    icf_print_api_header(p_msg);

     
    ICF_PRINT_MESSAGE(("\nbit_mask = %u, line_id = %u", \
			p_reg_req->bit_mask, p_reg_req->line_id));
     
    if (p_reg_req->bit_mask & ICF_REG_REQ_TO_PRESENT)
	{
    	ICF_PRINT_MESSAGE(("\nto = (addr_type = %u, addr = %s)",
              p_reg_req->to.addr_type,
              p_reg_req->to.addr_val.str));
	}
  
    if (p_reg_req->bit_mask & ICF_REG_REQ_EXPIRY_PRESENT)
	{
    	ICF_PRINT_MESSAGE(("\nexpiry = %u", p_reg_req->expiry));
	}

/* Header LIST  */    if (p_reg_req->bit_mask & ICF_HDR_LIST_PRESENT) {
        header_list_count = (icf_uint8_t)p_reg_req->header_list.count;
        icf_print_header_list(p_reg_req->header_list.hdr, header_list_count);
    }
    
    /* Tag LIST  */    if (p_reg_req->bit_mask & ICF_TAG_LIST_PRESENT) {
        tag_list_count = (icf_uint8_t)p_reg_req->tag_list.count;
        icf_print_tag_list(p_reg_req->tag_list.tag, tag_list_count);
    }
    
    /* Body LIST  */
    if (p_reg_req->bit_mask & ICF_BODY_LIST_PRESENT) {
        body_list_count = (icf_uint8_t)p_reg_req->body_list.count;
        icf_print_body_list(p_reg_req->body_list.body, body_list_count);
    }
    
    /* CSR_1-8389294 changes start */
    if (p_reg_req->bit_mask & ICF_REG_REQ_DEREG_SPECIFIC_CONTACT_PRESENT)
	{
    	ICF_PRINT_MESSAGE(("\nDeregister Specific Contact = %u",
                                        p_reg_req->dereg_specific_contact));
	}    
    /* CSR_1-8389294 changes end */
  
    return ICF_SUCCESS;

}






icf_return_t icf_print_register_cfm(icf_msg_st* p_msg)
{

    icf_register_cfm_st* p_reg_cfm = ICF_NULL;

    icf_uint8_t  header_list_count = 0, body_list_count = 0;
    
    p_reg_cfm = (icf_register_cfm_st*)p_msg->payload;
    icf_print_api_header(p_msg);


   


    ICF_PRINT_MESSAGE((
        "bit_mask = %u, line_id = %u, request_type = %u"
        "\nresult = %u, error_cause = %u",
        p_reg_cfm->bit_mask, p_reg_cfm->line_id, p_reg_cfm->request_type, 
        p_reg_cfm->result, p_reg_cfm->error_cause));
    icf_print_error_details(p_reg_cfm->error_cause);

    icf_print_pub_uri_list(p_reg_cfm->p_uri_list);


    ICF_PRINT_MESSAGE((
        "\nto.addr_type = %u, to.addr_val.str = %s", 
        p_reg_cfm->to.addr_type, p_reg_cfm->to.addr_val.str));


/* Header LIST  */    if (p_reg_cfm->bit_mask & ICF_HDR_LIST_PRESENT) {
        header_list_count = (icf_uint8_t)p_reg_cfm->header_list.count;
        icf_print_header_list(p_reg_cfm->header_list.hdr, header_list_count);
    }
  
    /* Body LIST  */
    if (p_reg_cfm->bit_mask & ICF_BODY_LIST_PRESENT) {
        body_list_count = (icf_uint8_t)p_reg_cfm->body_list.count;
        icf_print_body_list(p_reg_cfm->body_list.body, body_list_count);
    }
    

  
    return ICF_SUCCESS;

}



icf_return_t icf_print_register_status_ind(icf_msg_st* p_msg)
{

    

    icf_register_status_ind_st* p_reg_stat = ICF_NULL;

    icf_uint8_t  header_list_count = 0;      

    p_reg_stat = (icf_register_status_ind_st*)p_msg->payload;




    icf_print_api_header(p_msg);



    ICF_PRINT_MESSAGE((
        "\nbit_mask = %u, line_id = %u, nrequest_type = %u"
        "\nresult = %u, error_cause = %u",
        p_reg_stat->bit_mask, p_reg_stat->line_id, p_reg_stat->request_type, 
        p_reg_stat->result, p_reg_stat->error_cause));
    icf_print_error_details(p_reg_stat->error_cause);

        icf_print_pub_uri_list(p_reg_stat->p_uri_list);


    ICF_PRINT_MESSAGE((
        "\nto.addr_type = %u, to.addr_val.str = %s", 
        p_reg_stat->to.addr_type, p_reg_stat->to.addr_val.str));


/* Header LIST  */    if (p_reg_stat->bit_mask & ICF_HDR_LIST_PRESENT) {
        header_list_count = (icf_uint8_t)p_reg_stat->header_list.count;
        icf_print_header_list(p_reg_stat->header_list.hdr, header_list_count);
    }


    return ICF_SUCCESS;

}




    

    
icf_return_t icf_print_subscribe_req(icf_msg_st* p_msg)
{


    icf_subscribe_req_st* p_sub_req = ICF_NULL;

    icf_uint8_t  header_list_count = 0, body_list_count = 0, tag_list_count = 0;
       
    p_sub_req = (icf_subscribe_req_st*)p_msg->payload;

    icf_print_api_header(p_msg);



    ICF_PRINT_MESSAGE((
        "\nbit_mask = %u", p_sub_req->bit_mask));


	ICF_PRINT_MESSAGE(( 
		"\ndestination_info = (addr_type = %u, %s)",
		p_sub_req->destination_info.addr_type,
		p_sub_req->destination_info.addr_val.str));



    ICF_PRINT_MESSAGE((
        "\nevent_pkg_name.str = %s, duration = %u"
        "\npref_identity.str = %s", 
        p_sub_req->event_pkg_name.str, p_sub_req->duration, 
        p_sub_req->pref_identity.addr_val.str));
	
	


	/* Header LIST  */    if (p_sub_req->bit_mask & ICF_HDR_LIST_PRESENT) {
        header_list_count = (icf_uint8_t)p_sub_req->header_list.count;
        icf_print_header_list(p_sub_req->header_list.hdr, header_list_count);
    }
    
    /* Tag LIST  */    if (p_sub_req->bit_mask & ICF_TAG_LIST_PRESENT) {
		tag_list_count = (icf_uint8_t)p_sub_req->tag_list.count;
		icf_print_tag_list(p_sub_req->tag_list.tag, tag_list_count);
    }
    
    
    /* Body LIST  */
    if (p_sub_req->bit_mask & ICF_BODY_LIST_PRESENT) {
        body_list_count = (icf_uint8_t)p_sub_req->body_list.count;
        icf_print_body_list(p_sub_req->body_list.body, body_list_count);
    }
    


    return ICF_SUCCESS;


}

icf_return_t icf_print_nw_buff_ind(icf_msg_st* p_msg)
{

	icf_trace_nw_buff_ind_st* p_nw_buff_ind = ICF_NULL;
	icf_transport_addr_type_t addr_type = ICF_NULL;
    icf_uint32_t              len = 0;
    icf_uint32_t              i = 0;

	p_nw_buff_ind = (icf_trace_nw_buff_ind_st*)p_msg->payload;

	icf_print_api_header(p_msg);

 
	ICF_PRINT_MESSAGE(( 
        "\n NW Address : addr_type = %u, ",
        p_nw_buff_ind->nw_address.addr.addr_type));
    
    
    addr_type = p_nw_buff_ind->nw_address.addr.addr_type;
    
    
    switch (addr_type) {
    case ICF_TRANSPORT_ADDRESS_DOMAIN_NAME:
        ICF_PRINT_MESSAGE(("\n%s, ", 
            p_nw_buff_ind->nw_address.addr.addr.domain.str));
        break;
        
    case ICF_TRANSPORT_ADDRESS_IPV4_ADDR:
        ICF_PRINT_MESSAGE(("\n%u.%u.%u.%u, ", 
            p_nw_buff_ind->nw_address.addr.addr.ipv4_addr.octet_1, 
            p_nw_buff_ind->nw_address.addr.addr.ipv4_addr.octet_2, 
            p_nw_buff_ind->nw_address.addr.addr.ipv4_addr.octet_3, 
            p_nw_buff_ind->nw_address.addr.addr.ipv4_addr.octet_4));
        break;
        
    }
    
    ICF_PRINT_MESSAGE(("\nport_num = %u",
        p_nw_buff_ind->nw_address.port_num));

	p_nw_buff_ind->nw_buffer.p_buff[p_nw_buff_ind->nw_buffer.len] = 0;

    len = icf_port_strlen(p_nw_buff_ind->nw_buffer.p_buff);
    ICF_PRINT(((icf_uint8_t *)"\n"));
    for(i = 0; i < len; i++)
    {
        if ((p_nw_buff_ind->nw_buffer.p_buff[i] > ICF_MAX_NON_PRINTABLE_CHARACTER)
                ||(p_nw_buff_ind->nw_buffer.p_buff[i] == ICF_NEXT_LINE_CHARACTER))
            ICF_PRINT(((icf_uint8_t *)"%c",p_nw_buff_ind->nw_buffer.p_buff[i]));
    }

	return ICF_SUCCESS;
}


icf_return_t icf_print_subscribe_resp(icf_msg_st* p_msg)
{

    icf_subscription_status_ind_st* p_sub_resp = ICF_NULL;

    icf_uint8_t  header_list_count = 0;

    
    p_sub_resp = (icf_subscription_status_ind_st*)p_msg->payload;

    icf_print_api_header(p_msg);



	ICF_PRINT_MESSAGE((
        "\nbit_mask = %u", p_sub_resp->bit_mask));


	ICF_PRINT_MESSAGE(("\n\nICF_SUBS_ACCEPTED\t\t\t\t0"));
	ICF_PRINT_MESSAGE(("\nICF_SUBS_SUBSCRIBED\t\t\t\t1"));
	ICF_PRINT_MESSAGE(("\nICF_SUBS_FAIL_RETRY\t\t\t\t2"));
	ICF_PRINT_MESSAGE(("\nICF_SUBS_FAIL_NO_RETRY\t\t\t\t3"));
	ICF_PRINT_MESSAGE(("\nICF_SUBS_UNSUBSCRIBED\t\t\t\t4"));
	
    ICF_PRINT_MESSAGE((
        "\nsubscription_status = %u",
        p_sub_resp->status));

	ICF_PRINT_MESSAGE((
        "\nduration = %u ",
        p_sub_resp->duration));

	if (p_sub_resp->bit_mask & ICF_SUBS_ST_IND_RESP_CODE_PRESENT) 
	{
		ICF_PRINT_MESSAGE((
			"\nresponse_code = %u ",
			p_sub_resp->response_code));
	}

    ICF_PRINT_MESSAGE((
        "\nerror_code = %u ",
        p_sub_resp->error_code));
	



	/* Header LIST  */    if (p_sub_resp->bit_mask & ICF_HDR_LIST_PRESENT) {
        header_list_count = (icf_uint8_t)p_sub_resp->header_list.count;
        icf_print_header_list(p_sub_resp->header_list.hdr, header_list_count);
    }
    
	/* Fix for CSR 1-8420460: SPR 20343 */
	/* Event package name for which subscribe request was sent */
	ICF_PRINT_MESSAGE(( 
		"\nevent_pkg_name = (len = %u, str = %s)\n",
		p_sub_resp->event_pkg_name.str_len,
		p_sub_resp->event_pkg_name.str));
   
    return ICF_SUCCESS;

}





icf_return_t icf_print_publish_req(icf_msg_st* p_msg)
{

    icf_publish_req_st* p_pub_req = ICF_NULL;

    icf_uint8_t  header_list_count = 0, body_list_count = 0, tag_list_count = 0;
       
    p_pub_req = (icf_publish_req_st*)p_msg->payload;

    icf_print_api_header(p_msg);
    ICF_PRINT_MESSAGE((
        "\nbit_mask = %u", p_pub_req->bit_mask));


	ICF_PRINT_MESSAGE(( 
		"\ndestination_info = (addr_type = %u, %s)",
		p_pub_req->destination_info.addr_type,
		p_pub_req->destination_info.addr_val.str));

    ICF_PRINT_MESSAGE((
        "\nLine_id = %u", p_pub_req->line_id));



    ICF_PRINT_MESSAGE((
        "\nevent_pkg_name.str = %s, duration = %u",
        p_pub_req->event_pkg_name.str, p_pub_req->duration));

    if(p_pub_req->bit_mask & ICF_PREF_IDENTITY_PRESENT)
    {
        ICF_PRINT_MESSAGE((
        "\npref_identity.str = %s", 
        p_pub_req->pref_identity.addr_val.str));
    }

    if(p_pub_req->bit_mask & ICF_USER_ADDRESS_PRESENT)
    {
        ICF_PRINT_MESSAGE((
        "\nuser_address.str = %s", 
        p_pub_req->user_address.addr_val.str));
    }

	/* Header LIST  */    if (p_pub_req->bit_mask & ICF_HDR_LIST_PRESENT) {
        header_list_count = (icf_uint8_t)p_pub_req->header_list.count;
        icf_print_header_list(p_pub_req->header_list.hdr, header_list_count);
    }
    
    /* Tag LIST  */    if (p_pub_req->bit_mask & ICF_TAG_LIST_PRESENT) {
		tag_list_count = (icf_uint8_t)p_pub_req->tag_list.count;
		icf_print_tag_list(p_pub_req->tag_list.tag, tag_list_count);
    }
    
    
    /* Body LIST  */
    if (p_pub_req->bit_mask & ICF_BODY_LIST_PRESENT) {
        body_list_count = (icf_uint8_t)p_pub_req->body_list.count;
        icf_print_body_list(p_pub_req->body_list.body, body_list_count);
    }

    return ICF_SUCCESS;
}



icf_return_t icf_print_publish_resp(icf_msg_st* p_msg)
{

    icf_publish_resp_st* p_pub_resp = ICF_NULL;

    icf_uint8_t  header_list_count = 0;

    
    p_pub_resp = (icf_publish_resp_st*)p_msg->payload;

    icf_print_api_header(p_msg);



	ICF_PRINT_MESSAGE((
        "\nbit_mask = %u", p_pub_resp->bit_mask));


    ICF_PRINT_MESSAGE((
        "\nresult = %u, response_code = %u",
        p_pub_resp->result, p_pub_resp->response_code));


    ICF_PRINT_MESSAGE((
        "\nerror_code = %u, duration = %u entity_tag = %s",
        p_pub_resp->error_code, p_pub_resp->duration, p_pub_resp->entity_tag.str));
	



	/* Header LIST  */    if (p_pub_resp->bit_mask & ICF_HDR_LIST_PRESENT) {
        header_list_count = (icf_uint8_t)p_pub_resp->header_list.count;
        icf_print_header_list(p_pub_resp->header_list.hdr, header_list_count);
    }
    

    return ICF_SUCCESS;

}



icf_return_t icf_print_message_req(icf_msg_st* p_msg)
{


    icf_message_req_st* p_msg_req = ICF_NULL;

    icf_uint8_t  header_list_count = 0, body_list_count = 0, tag_list_count = 0;
   
    
    p_msg_req = (icf_message_req_st*)p_msg->payload;

    icf_print_api_header(p_msg);



    ICF_PRINT_MESSAGE((
        "\nbit_mask = %u", p_msg_req->bit_mask));


	ICF_PRINT_MESSAGE(( 
		"\ndestination_info = (addr_type = %u, %s)",
		p_msg_req->destination_info.addr_type,
		p_msg_req->destination_info.addr_val.str));

    ICF_PRINT_MESSAGE((
        "\nLine_id = %u", p_msg_req->line_id));


    if(p_msg_req->bit_mask & ICF_PREF_IDENTITY_PRESENT)
    {
        ICF_PRINT_MESSAGE((
        "\nuser_address.str = %s", 
        p_msg_req->user_address.addr_val.str));
    }

    if(p_msg_req->bit_mask & ICF_USER_ADDRESS_PRESENT)
    {
        ICF_PRINT_MESSAGE((
        "\npref_identity.str = %s", 
        p_msg_req->pref_identity.addr_val.str));
    }

	/* Header LIST  */    if (p_msg_req->bit_mask & ICF_HDR_LIST_PRESENT) {
        header_list_count = (icf_uint8_t)p_msg_req->header_list.count;
        icf_print_header_list(p_msg_req->header_list.hdr, header_list_count);
    }
    
    /* Tag LIST  */    if (p_msg_req->bit_mask & ICF_TAG_LIST_PRESENT) {
		tag_list_count = (icf_uint8_t)p_msg_req->tag_list.count;
		icf_print_tag_list(p_msg_req->tag_list.tag, tag_list_count);
    }
    
    
    /* Body LIST  */
    if (p_msg_req->bit_mask & ICF_BODY_LIST_PRESENT) {
        body_list_count = (icf_uint8_t)p_msg_req->body_list.count;
        icf_print_body_list(p_msg_req->body_list.body, body_list_count);
    }
    


    return ICF_SUCCESS;


}



icf_return_t icf_print_message_resp(icf_msg_st* p_msg)
{

    icf_message_resp_st* p_msg_resp = ICF_NULL;

    icf_uint8_t  header_list_count = 0;

    
    p_msg_resp = (icf_message_resp_st*)p_msg->payload;

    icf_print_api_header(p_msg);


	ICF_PRINT_MESSAGE((
        "\nbit_mask = %u", p_msg_resp->bit_mask));


    ICF_PRINT_MESSAGE((
        "\nresult = %u, response_code = %u",
        p_msg_resp->result, p_msg_resp->response_code));


    ICF_PRINT_MESSAGE((
        "\nerror_code = %u",
        p_msg_resp->error_code));
	



	/* Header LIST  */    if (p_msg_resp->bit_mask & ICF_HDR_LIST_PRESENT) {
        header_list_count = (icf_uint8_t)p_msg_resp->header_list.count;
        icf_print_header_list(p_msg_resp->header_list.hdr, header_list_count);
    }
    

    return ICF_SUCCESS;

}




icf_return_t icf_print_message_ind(icf_msg_st* p_msg)
{

    icf_message_ind_st* p_msg_ind = ICF_NULL;

    icf_uint8_t  header_list_count = 0, body_list_count = 0;

    
    p_msg_ind = (icf_message_ind_st*)p_msg->payload;

    icf_print_api_header(p_msg);


 
	ICF_PRINT_MESSAGE((
        "\nbit_mask = %u", p_msg_ind->bit_mask));


	/* Header LIST  */    if (p_msg_ind->bit_mask & ICF_HDR_LIST_PRESENT) {
        header_list_count = (icf_uint8_t)p_msg_ind->header_list.count;
        icf_print_header_list(p_msg_ind->header_list.hdr, header_list_count);
    }
    
    /* Body LIST  */
    if (p_msg_ind->bit_mask & ICF_BODY_LIST_PRESENT) {
        body_list_count = (icf_uint8_t)p_msg_ind->body_list.count;
        icf_print_body_list(p_msg_ind->body_list.body, body_list_count);
    }

    return ICF_SUCCESS;


}

icf_return_t icf_print_codec_info_req(icf_msg_st *p_msg)
{

    icf_cfg_codec_info_req_st* p_ci_req = NULL;
    
    icf_uint32_t k = 0,i = 0,j = 0;
	icf_list_st		*p_enc_parm = NULL;

    p_ci_req = (icf_cfg_codec_info_req_st*)p_msg->payload;

    icf_print_api_header(p_msg);

#ifdef ICF_REL_OLD
	ICF_PRINT_MESSAGE(( 
		"\ncall_type = %u", p_ci_req->call_type));

	ICF_PRINT_MESSAGE(( 
		"\nnum_of_codecs = %u", p_ci_req->num_of_codecs);

	num_of_codecs = p_ci_req->num_of_codecs;

    if (num_of_codecs > ICF_MAX_CODEC_SUPPORTD)
        num_of_codecs = ICF_MAX_CODEC_SUPPORTD;

    for (j = 0; j < num_of_codecs; j++) {
        ICF_PRINT_MESSAGE(( 
            "\n\tcodec_supp[%u].codec_id = %u"
            "\n\tcodec_supp[%u].packetization_period = %u"
            "\n\tcodec_supp[%u].encoding_rate = %u"
            "\n\tcodec_supp[%u].channels = %u"
			"\n\tcodec_supp[%u].encoding_name = %s",
            j, p_ci_req->codec_supp[j].codec_id,
            j, p_ci_req->codec_supp[j].packetization_period,
			j, p_ci_req->codec_supp[j].encoding_rate,
            j, p_ci_req->codec_supp[j].channels,
            j, p_ci_req->codec_supp[j].encoding_name));
        ICF_PRINT_MESSAGE(("\n"));
    }

	ICF_PRINT_MESSAGE(( 
		"\nbit_mask = %u", p_ci_req->bit_mask);

	ICF_PRINT_MESSAGE(( 
		"\namr_initial_mode = %u", p_ci_req->amr_params.amr_initial_mode));
	ICF_PRINT_MESSAGE(( 
		"\ncodec_mode = %u", p_ci_req->amr_params.codec_mode));
	ICF_PRINT_MESSAGE(( 
		"\ncrc_enabled = %u", p_ci_req->amr_params.crc_enabled));

	if(ICF_AMR_MODE_LIST_PRESENT & p_icf_audio_codec_param_st->amr_params.bit_mask)
	{
		ICF_PRINT_MESSAGE(("\nAMR Mode Count = %u\n",
				p_ci_req->amr_params.amr_mode_count);
		for(i=0;i<p_ci_req->amr_params.amr_mode_count;i++)
			ICF_PRINT_MESSAGE(("%u\n",
				p_ci_req->amr_params.amr_mode_list[i]));
	}

    #endif

	ICF_PRINT_MESSAGE(("\n Count = %u", p_ci_req->count));

	for(i=0;i<p_ci_req->count;i++)
	{
		ICF_PRINT_MESSAGE(("\n Codec Type = %u", p_ci_req->codec_supp[i].codec_type));
	
		ICF_PRINT_MESSAGE(("\n Codec Name = "));

		for(k=0;k<p_ci_req->codec_supp[i].codec_name.str_len;k++)

			ICF_PRINT_MESSAGE(("%c", p_ci_req->codec_supp[i].codec_name.str[k]));

		if(p_ci_req->codec_supp[i].bitmask & ICF_ENCODED_PARAM_PRESENT)
		{
			p_enc_parm = p_ci_req->codec_supp[i].p_encoded_params;

			ICF_PRINT_MESSAGE(("\nList of encoded params\n"));

			while(p_enc_parm != ICF_NULL)
			{
				ICF_PRINT_MESSAGE((" "));

				for(j=0;j<((icf_string_st*)(p_enc_parm->p_data))->str_len;j++)
				{
					ICF_PRINT_MESSAGE(("%c",
							((icf_string_st*)p_enc_parm->p_data)->str[j]));
				}

				ICF_PRINT_MESSAGE(("\n"));
				p_enc_parm = p_enc_parm->p_next;
			}
		}
#if 1 /* ZyXEL_PORTING*/
		if(p_ci_req->codec_supp[i].bitmask & ICF_CODEC_NUM_PRESENT)
		{
			ICF_PRINT_MESSAGE(("\n Codec Num = %u", p_ci_req->codec_supp[i].codec_num));
		}
#endif		
		if(p_ci_req->codec_supp[i].bitmask & ICF_CODEC_PARAM_PRESENT)
		{
			if(ICF_CODEC_TYPE_AUDIO == p_ci_req->codec_supp[i].codec_type)
			{
				if(ICF_AUDIO_CODEC_CHANNEL & p_ci_req->codec_supp[i].codec_param.audio_param.bit_mask)
					ICF_PRINT_MESSAGE(("\n No of Channels = %u",p_ci_req->codec_supp[i].codec_param.audio_param.channels));

				if(ICF_AUDIO_CODEC_PTIME & p_ci_req->codec_supp[i].codec_param.audio_param.bit_mask)
				{
					ICF_PRINT_MESSAGE(("\n No of PTime supported = %u",p_ci_req->codec_supp[i].codec_param.audio_param.num_of_supported_ptime));

					for(j=0;j<p_ci_req->codec_supp[i].codec_param.audio_param.num_of_supported_ptime;j++)
					{
						ICF_PRINT_MESSAGE(("\n PTime[%u] = %u", j ,
											p_ci_req->codec_supp[i].codec_param.audio_param.ptime[j]));
					}
				}

				if(ICF_AUDIO_CODEC_BANDWIDTH & p_ci_req->codec_supp[i].codec_param.audio_param.bit_mask)
					ICF_PRINT_MESSAGE(("\n Bandwidth = %u",p_ci_req->codec_supp[i].codec_param.audio_param.bandwidth));


				if(ICF_AUDIO_CODEC_SAMPLING_RATE & p_ci_req->codec_supp[i].codec_param.audio_param.bit_mask)
				{
					ICF_PRINT_MESSAGE(("\n No of Sampling Rate supported = %u",p_ci_req->codec_supp[i].codec_param.audio_param.num_of_supp_sampling_rate));

					for(j=0;j<p_ci_req->codec_supp[i].codec_param.audio_param.num_of_supp_sampling_rate;j++)
					{
						ICF_PRINT_MESSAGE(("\n Sampling Rate[%u] = %u", j ,
											p_ci_req->codec_supp[i].codec_param.audio_param.sampling_rate[j]));
					}
				}

				if(ICF_AUDIO_CODEC_SILENCE_SUPPRESSION & p_ci_req->codec_supp[i].codec_param.audio_param.bit_mask)
					ICF_PRINT_MESSAGE(("\n Silence Suppression = %u",p_ci_req->codec_supp[i].codec_param.audio_param.silence_suppression));


				if(ICF_AUDIO_CODEC_AMR_PARAM & p_ci_req->codec_supp[i].codec_param.audio_param.bit_mask)
				{
					ICF_PRINT_MESSAGE(("\n AMR Initial Mode = %u",p_ci_req->codec_supp[i].codec_param.audio_param.amr_params.amr_initial_mode));

					ICF_PRINT_MESSAGE(("\n Codec Mode = %u",p_ci_req->codec_supp[i].codec_param.audio_param.amr_params.codec_mode));

					ICF_PRINT_MESSAGE(("\n CRC Enabled = %u",p_ci_req->codec_supp[i].codec_param.audio_param.amr_params.crc_enabled));

					if(ICF_AMR_MODE_LIST_PRESENT & p_ci_req->codec_supp[i].codec_param.audio_param.amr_params.bit_mask)
					{
						ICF_PRINT_MESSAGE(("\nAMR Mode Count = %u",
								p_ci_req->codec_supp[i].codec_param.audio_param.amr_params.amr_mode_count));
						for(j=0;j<p_ci_req->codec_supp[i].codec_param.audio_param.amr_params.amr_mode_count;j++)
							ICF_PRINT_MESSAGE(("%u\n",
								p_ci_req->codec_supp[i].codec_param.audio_param.amr_params.amr_mode_list[j]));
					}
								
				}

			}

			else if(ICF_CODEC_TYPE_VIDEO == p_ci_req->codec_supp[i].codec_type)
			{
				if(ICF_VIDEO_CFG_CODEC_FRAME_RATE_PRESENT & p_ci_req->codec_supp[i].codec_param.video_param.bit_mask)
					ICF_PRINT_MESSAGE(("\n Frame Rate = %u",p_ci_req->codec_supp[i].codec_param.video_param.frame_rate));

				if(ICF_VIDEO_CFG_CODEC_ENC_QUALITY_PRESENT & p_ci_req->codec_supp[i].codec_param.video_param.bit_mask)
					ICF_PRINT_MESSAGE(("\n Encoding Quality = %u",p_ci_req->codec_supp[i].codec_param.video_param.encoding_quality));

				if(ICF_VIDEO_CFG_CODEC_BANDWIDTH_PRESENT & p_ci_req->codec_supp[i].codec_param.video_param.bit_mask)
					ICF_PRINT_MESSAGE(("\n Bandwidth = %u",p_ci_req->codec_supp[i].codec_param.video_param.bandwidth));

				if(ICF_VIDEO_CFG_CODEC_CLOCK_RATE_PRESENT & p_ci_req->codec_supp[i].codec_param.video_param.bit_mask)
				{
					ICF_PRINT_MESSAGE(("\n No of Clock Rate supported = %u",p_ci_req->codec_supp[i].codec_param.video_param.num_of_supp_clock_rate));

					for(j=0;j<p_ci_req->codec_supp[i].codec_param.video_param.num_of_supp_clock_rate;j++)
					{
						ICF_PRINT_MESSAGE(("\n Clock Rate[%u] = %u", j ,
											p_ci_req->codec_supp[i].codec_param.video_param.clock_rate[j]));
					}
				}

				if(ICF_VIDEO_CFG_CODEC_ENC_QUALITY_PRESENT & p_ci_req->codec_supp[i].codec_param.video_param.bit_mask)
				{
					ICF_PRINT_MESSAGE(("\n Frame Size - Height = %u",p_ci_req->codec_supp[i].codec_param.video_param.frame_size.height));

					ICF_PRINT_MESSAGE(("\n Frame Size - Width = %u",p_ci_req->codec_supp[i].codec_param.video_param.frame_size.width));
				}

			}

			else if(ICF_CODEC_TYPE_IMAGE == p_ci_req->codec_supp[i].codec_type)
			{
				if(ICF_FAX_T38_VERSION & p_ci_req->codec_supp[i].codec_param.t38_param.bit_mask)
					ICF_PRINT_MESSAGE(("\n Fax Version = %u",p_ci_req->codec_supp[i].codec_param.t38_param.fax_version));

				if(ICF_FAX_T38_BITRATE & p_ci_req->codec_supp[i].codec_param.t38_param.bit_mask)
					ICF_PRINT_MESSAGE(("\n Bit Rate = %u",p_ci_req->codec_supp[i].codec_param.t38_param.bitrate));

				if(ICF_FAX_T38_RATE_MANAGEMENT & p_ci_req->codec_supp[i].codec_param.t38_param.bit_mask)
					ICF_PRINT_MESSAGE(("\n Rate Management = %u",p_ci_req->codec_supp[i].codec_param.t38_param.rate_management));

				if(ICF_FAX_T38_UDP_BUFFSIZE & p_ci_req->codec_supp[i].codec_param.t38_param.bit_mask)
					ICF_PRINT_MESSAGE(("\n UDP Buffer Size = %u",p_ci_req->codec_supp[i].codec_param.t38_param.udp_buffersize));

				if(ICF_FAX_T38_UDP_PACKETSIZE & p_ci_req->codec_supp[i].codec_param.t38_param.bit_mask)
					ICF_PRINT_MESSAGE(("\n UDP PACKETSIZE = %u",p_ci_req->codec_supp[i].codec_param.t38_param.udp_packetsize));

				if(ICF_FAX_T38_ERR_CORRECTION & p_ci_req->codec_supp[i].codec_param.t38_param.bit_mask)
					ICF_PRINT_MESSAGE(("\n ERR CORRECTION = %u",p_ci_req->codec_supp[i].codec_param.t38_param.err_correction_method));
				
			}
		}
	}

    return ICF_SUCCESS;

}

icf_return_t icf_print_ringer_tones_req(icf_msg_st *p_msg)
{
    icf_cfg_ringer_tones_req_st* p_req = (icf_cfg_ringer_tones_req_st*)
		p_msg->payload;
    icf_uint8_t i = 0;
    icf_uint16_t num = 0;
    
    icf_print_api_header(p_msg);

   
    ICF_PRINT_MESSAGE((\
			"\nnum_of_ringer = %u", p_req->num_of_ringer));

    num=p_req->num_of_ringer;

    if(num > ICF_MAX_RINGER_LIST)
		num = ICF_MAX_RINGER_LIST;

    for (i=0; i<num; i++) {
	    ICF_PRINT_MESSAGE((\
			"\n\tringer[%u].ringer_id = %u"
			"\n\tringer[%u].play_file.str = %s"
			"\n\tringer[%u].play_file.str_len = %u"
			"\n",
			i,p_req->ringer[i].ringer_id,
			i,p_req->ringer[i].play_file.str,
			i,p_req->ringer[i].play_file.str_len			
			));
    }

   
    
    return ICF_SUCCESS;

}
icf_return_t icf_print_jbm_mgmt_req(icf_msg_st *p_msg)
{
    icf_cfg_jbm_mgmt_req_st* p_req = (icf_cfg_jbm_mgmt_req_st*)p_msg->payload;
    
    icf_print_api_header(p_msg);

    
    ICF_PRINT_MESSAGE((\
		    "\njbm_type = %u"
		    "\njbm_scaling_factor = %u"
		    "\njbm_initial_size = %u"
		    "\njbm_max_size = %u"
		    "\njbm_min_size = %u",
		    p_req->jbm_type,
		    p_req->jbm_scaling_factor,
		    p_req->jbm_initial_size,
		    p_req->jbm_max_size,
		    p_req->jbm_min_size
	     ));
#ifdef ICF_REL_OLD
    ICF_PRINT_MESSAGE((
		    "\nbit_mask = %u", p_req->bit_mask);
#endif
    
    return ICF_SUCCESS;

}
icf_return_t icf_print_max_earp_vol_req(icf_msg_st *p_msg)
{
    icf_cfg_max_earp_vol_req_st* p_req = (icf_cfg_max_earp_vol_req_st*)p_msg->payload;
    
    icf_print_api_header(p_msg);

    
    ICF_PRINT_MESSAGE((\
			"\nmax_earp_volume = %u", p_req->max_earp_volume));

/*    ICF_PRINT_MESSAGE((
		    "\nbit_mask = %u", p_req->bit_mask);*/

    
    return ICF_SUCCESS;

}
icf_return_t icf_print_max_speaker_vol_req(icf_msg_st *p_msg)
{
    icf_cfg_max_speaker_vol_req_st* p_req = (icf_cfg_max_speaker_vol_req_st*)p_msg->payload;
    
    icf_print_api_header(p_msg);

    
    ICF_PRINT_MESSAGE((\
			"\nmax_speaker_volume = %u", p_req->max_speaker_volume));

   /*ICF_PRINT_MESSAGE((
		    "\nbit_mask = %u", p_req->bit_mask);*/

    
    return ICF_SUCCESS;

}
icf_return_t icf_print_port_range_req(icf_msg_st *p_msg)
{
    icf_cfg_set_port_range_req_st* p_req = (icf_cfg_set_port_range_req_st*)p_msg->payload;
    
    icf_print_api_header(p_msg);

    /* TBD */
#ifdef ICF_REL_OLD
    ICF_PRINT_MESSAGE((\
			"\nstart_rtp_port = %u"
			"\nend_rtp_port = %u", 
			p_req->start_rtp_port,
			p_req->end_rtp_port);
#endif
    ICF_PRINT_MESSAGE((
		    "\nbit_mask = %u", p_req->bit_mask));



	ICF_PRINT_MESSAGE(("\n start_rtp_port = %u", p_req->start_rtp_port));

	ICF_PRINT_MESSAGE(("\n end_rtp_port = %u", p_req->end_rtp_port));

	if(ICF_TRUE == p_req->is_app_rtcp_port_mux)
	{
		ICF_PRINT_MESSAGE(("\n Multiplexing Enabled"));
	}
	else
	{
		ICF_PRINT_MESSAGE(("\n Multiplexing Not Enabled"));
	}

	ICF_PRINT_MESSAGE(("\n start_app_port = %u", p_req->start_app_port));

	ICF_PRINT_MESSAGE(("\n end_app_port = %u", p_req->end_app_port));
	



    
    return ICF_SUCCESS;

}


/******************************************************************************
 *
 * FUNCTION:        icf_print_self_ip_req
 *
 * DESCRIPTION:     This function is used to print cfg_set_self_ip
 *
 *****************************************************************************/
icf_return_t icf_print_self_ip_req(icf_msg_st *p_msg)
{
    icf_cfg_set_self_ip_req_st* p_self_ip_req = (icf_cfg_set_self_ip_req_st*)p_msg->payload;
    
    icf_print_api_header(p_msg);

    
	ICF_PRINT_MESSAGE(("\nSelf IP Address\n"));

    ICF_PRINT_MESSAGE((\
			"\nValue = %s", p_self_ip_req->self_ip_addr.addr_val.str));

    ICF_PRINT_MESSAGE((\
			"\nAddr Type = %u", p_self_ip_req->self_ip_addr.addr_type));

        return ICF_SUCCESS;

}

/******************************************************************************
 *
 * FUNCTION:        icf_print_digit_trans_req
 *
 * DESCRIPTION:     This function is used to print cfg_set_digit_trans
 *
 *****************************************************************************/
icf_return_t icf_print_digit_trans_req(icf_msg_st *p_msg)
{
    icf_cfg_set_digit_trans_req_st* p_digit_trans_req = (icf_cfg_set_digit_trans_req_st*)p_msg->payload;
    
    icf_print_api_header(p_msg);

    	
    if(p_digit_trans_req->dgt_trns_method == 1)
    {
    ICF_PRINT_MESSAGE((\
			"\nDgt Trans Method = %s", "RFC_2833"));

    }
    if(p_digit_trans_req->dgt_trns_method == 2)
    {
         ICF_PRINT_MESSAGE((\
			"\nDgt Trans Method = %s", "INBAND_TONE"));
    }

   
    return ICF_SUCCESS;

}



icf_return_t icf_print_start_ringing_req(icf_msg_st *p_msg)
{
    icf_start_ringing_req_st* p_req = (icf_start_ringing_req_st*)p_msg->payload;
    
    icf_print_api_header(p_msg);

    
    ICF_PRINT_MESSAGE(("\nbit_mask = %u", p_req->bit_mask));

    if (p_req->bit_mask & ICF_RINGING_LINE_ID_PRESENT)
           ICF_PRINT_MESSAGE(("\nLine_id = %u", p_req->line_id));
		
    ICF_PRINT_MESSAGE(("\nringer_id = %u \nduration = %u",p_req->ringer_id,p_req->duration));

    if(p_req->bit_mask & ICF_RINGING_TIMER_VAL)
		ICF_PRINT_MESSAGE(("\ntimer_value = %u", p_req->timer_value));

        return ICF_SUCCESS;

}

#ifdef ICF_REL_OLD
icf_return_t icf_print_media_session_req(icf_msg_st *p_msg)
{
    icf_media_session_req_st* p_req = (icf_media_session_req_st*)p_msg->payload;
    
    icf_print_api_header(p_msg);

    
    ICF_PRINT_MESSAGE((\
			"\nstream_id = %u"
			"\nconnect_speaker_flg = %u",
			p_req->stream_id,
			p_req->connect_speaker_flg
	     );

    ICF_PRINT_MESSAGE((
		    "\nbit_mask = %u", p_req->bit_mask);

   
    return ICF_SUCCESS;

}
#endif
icf_return_t icf_print_get_stats_req(icf_msg_st *p_msg)
{
    icf_get_stats_req_st* p_req = (icf_get_stats_req_st*)p_msg->payload;
    
    icf_print_api_header(p_msg);

    
    ICF_PRINT_MESSAGE((\
			"\nstream_id = %u"
			"\nstats_type = %u"
			"\ninterval_type = %u", 
			p_req->stream_id,
			p_req->stats_type,
			p_req->interval_type
	     ));

   /* ICF_PRINT_MESSAGE((
		    "\nbit_mask = %u", p_req->bit_mask);*/

    
    return ICF_SUCCESS;

}
icf_return_t icf_print_stop_ringing_req(icf_msg_st *p_msg)
{
    icf_print_api_header(p_msg);

   
    return ICF_SUCCESS;

}

#ifdef ICF_REL_OLD
icf_return_t icf_print_mute_req(icf_msg_st *p_msg)
{
    icf_mute_req_st* p_req = (icf_mute_req_st*)p_msg->payload;
    
    icf_print_api_header(p_msg);

    
    ICF_PRINT_MESSAGE((\
			"\nactivate_mute = %u", 
			p_req->activate_mute));

/*    ICF_PRINT_MESSAGE((
		    "\nbit_mask = %u", p_req->bit_mask);*/

   
    return ICF_SUCCESS;

}
icf_return_t icf_print_play_media_spkr_req(icf_msg_st *p_msg)
{
    icf_play_media_spkr_req_st* p_req = (icf_play_media_spkr_req_st*)p_msg->payload;
    
    icf_print_api_header(p_msg);

    
    ICF_PRINT_MESSAGE((\
			"\nstream_id = %u"
			"\nmedia_play_out_device = %u",
			p_req->stream_id,
			p_req->media_play_out_device
	     );

/*    ICF_PRINT_MESSAGE((
		    "\nbit_mask = %u", p_req->bit_mask);*/

    
    return ICF_SUCCESS;

}
#endif
icf_return_t icf_print_set_earp_volume_req(icf_msg_st *p_msg)
{
    icf_set_earp_volume_req_st* p_req = (icf_set_earp_volume_req_st*)p_msg->payload;
    
    icf_print_api_header(p_msg);

   
    ICF_PRINT_MESSAGE(("\n bit_mask = %u:",p_req->bit_mask));

    if (p_req->bit_mask & ICF_LINE_ID_PRESENT) 
        ICF_PRINT_MESSAGE(("\n Line id = %u:",p_req->line_id));

    ICF_PRINT_MESSAGE((\
			"\nlevel = %u", p_req->level));

/*    ICF_PRINT_MESSAGE((
		    "\nbit_mask = %u", p_req->bit_mask));*/

    
    return ICF_SUCCESS;

}
icf_return_t icf_print_set_speaker_volume_req(icf_msg_st *p_msg)
{
    icf_set_speaker_volume_req_st* p_req = (icf_set_speaker_volume_req_st*)p_msg->payload;
    
    icf_print_api_header(p_msg);

    
    ICF_PRINT_MESSAGE(("\n bit_mask = %u:",p_req->bit_mask));

    if (p_req->bit_mask & ICF_LINE_ID_PRESENT)
        ICF_PRINT_MESSAGE(("\n Line id = %u:",p_req->line_id));

    ICF_PRINT_MESSAGE((\
			"\nlevel = %u", p_req->level));

/*    ICF_PRINT_MESSAGE((
		    "\nbit_mask = %u", p_req->bit_mask);*/

   
    return ICF_SUCCESS;
}

icf_return_t icf_print_send_app_data_req(icf_msg_st *p_msg)
{
    icf_send_app_data_req_st* p_req = (icf_send_app_data_req_st*)p_msg->payload;
    
    icf_print_api_header(p_msg);

        ICF_PRINT_MESSAGE((\
			"\nstream_id = %u"
			"\n\tapplication_data.str = %s"
			"\n\tapplication_data.str_len = %u"
			"\n\tsequence_num = %u"
			"\n\tsub-type = %u",
			p_req->stream_id,
			p_req->application_data.str,
			p_req->application_data.str_len,
			p_req->sequence_num,
			p_req->application_data.sub_type
	     ));
    
    ICF_PRINT_MESSAGE((
		    "\nbit_mask = %u", p_req->bit_mask));

   
    return ICF_SUCCESS;

}



icf_return_t icf_print_send_stats_req(icf_msg_st *p_msg)
{
    icf_send_stats_req_st* p_req = (icf_send_stats_req_st*)p_msg->payload;
    
    icf_print_api_header(p_msg);

    
    ICF_PRINT_MESSAGE((
			"\nstream_id = %u\nreport_type = %u", p_req->stream_id, p_req->report_type));
    
/*    ICF_PRINT_MESSAGE(("\nbit_mask = %u", p_req->bit_mask);*/

   
    return ICF_SUCCESS;
}

icf_return_t icf_print_stop_stats_req(icf_msg_st *p_msg)
{
    icf_stop_stats_req_st* p_req = (icf_stop_stats_req_st*)p_msg->payload;
    
    icf_print_api_header(p_msg);

  
    
    
    ICF_PRINT_MESSAGE((
		    "\nbit_mask = %u", p_req->bit_mask));

	if(p_req->bit_mask & ICF_STOP_STATS_REQ_STREAM_ID)
	{
		ICF_PRINT_MESSAGE(("\nstream_id = %u",p_req->stream_id));
	}

    
    return ICF_SUCCESS;

}
icf_return_t icf_print_stop_ringing_ind(icf_msg_st *p_msg)
{
    icf_stop_ringing_ind_st* p_req = (icf_stop_ringing_ind_st*)p_msg->payload;
    
    icf_print_api_header(p_msg);

   
    ICF_PRINT_MESSAGE((\
			"\nringer_id = %u",
			p_req->ringer_id
	     ));
#ifdef ICF_REL_OLD    
    ICF_PRINT_MESSAGE((
		    "\nbit_mask = %u", p_req->bit_mask));
#endif
   
    return ICF_SUCCESS;
}
icf_return_t icf_print_rcv_app_data_ind(icf_msg_st *p_msg)
{
    icf_rcv_app_data_ind_st* p_req = (icf_rcv_app_data_ind_st*)p_msg->payload;
    
    icf_print_api_header(p_msg);

    
    ICF_PRINT_MESSAGE((\
			"\nstream_id = %u"
			"\n\tapplication_data.str = %s"
			"\n\tapplication_data.str_len = %u"
			"\nsequence_num = %u",
			p_req->stream_id,
			p_req->application_data.str,
			p_req->application_data.str_len,
			p_req->sequence_num
	     ));
    
    ICF_PRINT_MESSAGE((
		    "\nbit_mask = %u", p_req->bit_mask));

    
    return ICF_SUCCESS;
}

icf_return_t icf_print_codec_info_resp(icf_msg_st *p_msg)
{
	icf_print_mm_msg_resp(p_msg);
	return ICF_SUCCESS;
}
icf_return_t icf_print_ringer_tones_resp(icf_msg_st *p_msg)
{
	icf_print_mm_msg_resp(p_msg);
	return ICF_SUCCESS;
}
icf_return_t icf_print_jbm_mgmt_resp(icf_msg_st *p_msg)
{
	icf_print_mm_msg_resp(p_msg);
	return ICF_SUCCESS;
}
icf_return_t icf_print_max_earp_vol_resp(icf_msg_st *p_msg)
{
	icf_print_mm_msg_resp(p_msg);
	return ICF_SUCCESS;
}
icf_return_t icf_print_max_speaker_vol_resp(icf_msg_st *p_msg)
{
	icf_print_mm_msg_resp(p_msg);
	return ICF_SUCCESS;
}
icf_return_t icf_print_set_port_range_resp(icf_msg_st *p_msg)
{
	icf_print_mm_msg_resp(p_msg);
	return ICF_SUCCESS;
}
icf_return_t icf_print_ringing_resp(icf_msg_st *p_msg)
{
	icf_print_mm_msg_resp(p_msg);
	return ICF_SUCCESS;
}
icf_return_t icf_print_media_session_resp(icf_msg_st *p_msg)
{
	icf_print_mm_msg_resp(p_msg);
	return ICF_SUCCESS;
}
icf_return_t icf_print_stop_ringing_resp(icf_msg_st *p_msg)
{
	icf_print_mm_msg_resp(p_msg);
	return ICF_SUCCESS;
}

icf_return_t icf_print_stop_vibration_resp(icf_msg_st *p_msg)
{
	icf_print_mm_msg_resp(p_msg);
	return ICF_SUCCESS;
}
icf_return_t icf_print_send_dgts_resp(icf_msg_st *p_msg)
{
	icf_print_mm_msg_resp(p_msg);
	return ICF_SUCCESS;
}


icf_return_t icf_print_play_media_spkr_resp(icf_msg_st *p_msg)
{
	icf_print_mm_msg_resp(p_msg);
	return ICF_SUCCESS;
}
icf_return_t icf_print_set_earp_volume_resp(icf_msg_st *p_msg)
{
	icf_print_mm_msg_resp(p_msg);
	return ICF_SUCCESS;
}
icf_return_t icf_print_set_speaker_volume_resp(icf_msg_st *p_msg)
{
	icf_print_mm_msg_resp(p_msg);
	return ICF_SUCCESS;
}
icf_return_t icf_print_send_app_data_resp(icf_msg_st *p_msg)
{
	icf_print_mm_msg_resp(p_msg);
	return ICF_SUCCESS;
}
icf_return_t icf_print_send_stats_resp(icf_msg_st *p_msg)
{
	icf_print_mm_msg_resp(p_msg);
	return ICF_SUCCESS;
}
icf_return_t icf_print_stop_stats_resp(icf_msg_st *p_msg)
{
	icf_print_mm_msg_resp(p_msg);
	return ICF_SUCCESS;
}

icf_return_t icf_print_get_stats_resp(icf_msg_st *p_msg)
{
    icf_get_stats_resp_st* p_get_stats_resp = ICF_NULL;
    
    p_get_stats_resp = (icf_get_stats_resp_st*)p_msg->payload;

    icf_print_api_header(p_msg);

   

	ICF_PRINT_MESSAGE(( 
		"\nStream ID = %u", p_get_stats_resp->stream_id));

	ICF_PRINT_MESSAGE(( 
		"\ntime_stamp = %u", p_get_stats_resp->rtcp_report.time_stamp));
	
	ICF_PRINT_MESSAGE(( 
		"\nnum_packet_send = %u", p_get_stats_resp->rtcp_report.num_packet_send));
	
	ICF_PRINT_MESSAGE(( 
		"\nnum_octet_send = %u", p_get_stats_resp->rtcp_report.num_octet_send));
	
	ICF_PRINT_MESSAGE(( 
		"\nnum_packet_rcvd = %u", p_get_stats_resp->rtcp_report.num_packet_rcvd));
	
	ICF_PRINT_MESSAGE(( 
		"\nnum_octet_rcvd = %u", p_get_stats_resp->rtcp_report.num_octet_rcvd));

	ICF_PRINT_MESSAGE(( 
		"\ncum_packet_loss = %u", p_get_stats_resp->rtcp_report.cum_packet_loss));

	
	return ICF_SUCCESS;
}

icf_return_t icf_print_mm_msg_resp(icf_msg_st *p_msg)
{
    icf_mm_msg_resp_st* p_mm_msg_resp = ICF_NULL;
    
    p_mm_msg_resp = (icf_mm_msg_resp_st*)p_msg->payload;

    icf_print_api_header(p_msg);

   

	ICF_PRINT_MESSAGE(( 
		"\nResult = %u", p_mm_msg_resp->result));
/*
	if(p_mm_msg_resp->bit_mask & ICF_MM_MSG_RESP_ERROR)
	{
		ICF_PRINT_MESSAGE(( 
			"\nError Cause = %u", p_mm_msg_resp->error_cause));
	}
*/
	ICF_PRINT_MESSAGE(( 
			"\nError Cause = %u", p_mm_msg_resp->error_cause));
	icf_print_error_details(p_mm_msg_resp->error_cause);
		return ICF_SUCCESS;
}





icf_return_t icf_print_refer_req(icf_msg_st *p_msg)
{


    icf_refer_req_st* p_refer_req = ICF_NULL;

    icf_uint8_t  header_list_count = 0, body_list_count = 0, 
					tag_list_count = 0;
        
    p_refer_req = (icf_refer_req_st*)p_msg->payload;

    icf_print_api_header(p_msg);


    

    ICF_PRINT_MESSAGE((
        "\nbit_mask = %u", p_refer_req->bit_mask));

	ICF_PRINT_MESSAGE(( 
		"\ndestination_info = (addr_type = %u, %s)",
		p_refer_req->destination_info.addr_type,
		p_refer_req->destination_info.addr_val.str));


    ICF_PRINT_MESSAGE((
        "\nrefer_to.str = %s", p_refer_req->refer_to.addr_val.str));

    ICF_PRINT_MESSAGE((
        "\nrefer_to.addr_type = %u", p_refer_req->refer_to.addr_type));

    ICF_PRINT_MESSAGE((
        "\npref_identity.str = %s", p_refer_req->pref_identity.addr_val.str));
	
    ICF_PRINT_MESSAGE((
        "\nsubscription_required = %u", p_refer_req->subscription_required));
	


	/* Header LIST  */    if (p_refer_req->bit_mask & ICF_HDR_LIST_PRESENT) {
        header_list_count = (icf_uint8_t)p_refer_req->header_list.count;
        icf_print_header_list(p_refer_req->header_list.hdr, header_list_count);
    }
    
    /* Tag LIST  */    if (p_refer_req->bit_mask & ICF_TAG_LIST_PRESENT) {
		tag_list_count = (icf_uint8_t)p_refer_req->tag_list.count;
		icf_print_tag_list(p_refer_req->tag_list.tag, tag_list_count);
    }
    
    
    /* Body LIST  */
    if (p_refer_req->bit_mask & ICF_BODY_LIST_PRESENT) {
        body_list_count = (icf_uint8_t)p_refer_req->body_list.count;
        icf_print_body_list(p_refer_req->body_list.body, body_list_count);
    }
    

    
    return ICF_SUCCESS;


}



icf_return_t icf_print_refer_resp(icf_msg_st* p_msg)
{

    icf_refer_resp_st* p_refer_resp = ICF_NULL;

    icf_uint8_t  header_list_count = 0;

    
    p_refer_resp = (icf_refer_resp_st*)p_msg->payload;

    icf_print_api_header(p_msg);


    
	ICF_PRINT_MESSAGE((
        "\nbit_mask = %u", p_refer_resp->bit_mask));


    ICF_PRINT_MESSAGE((
        "\nresult = %u, response_code = %u",
        p_refer_resp->result, p_refer_resp->response_code));


	ICF_PRINT_MESSAGE((
        "\nerror_code = %u",
        p_refer_resp->error_code));
	

	/* Header LIST  */    if (p_refer_resp->bit_mask & ICF_HDR_LIST_PRESENT) {
        header_list_count = (icf_uint8_t)p_refer_resp->header_list.count;
        icf_print_header_list(p_refer_resp->header_list.hdr, header_list_count);
    }
    

    
    return ICF_SUCCESS;

}


icf_return_t icf_print_sec_pdp_activate_req(icf_msg_st *p_msg)
{
    icf_gprs_sec_pdp_activate_req_st *p_msg_payload = ICF_NULL;
    
    p_msg_payload = (icf_gprs_sec_pdp_activate_req_st*)p_msg->payload;

    icf_print_api_header(p_msg);

   

	ICF_PRINT_MESSAGE(("\nPDP Context ID = %u", p_msg_payload->context_id));
 
		
	if(p_msg_payload->auth_token.str_len)
        {
	    ICF_PRINT_MESSAGE(("\nP-Media-Authorization Token  = %s", p_msg_payload->auth_token.str));
         }
 	else
        { 
	    ICF_PRINT_MESSAGE(("\nP-Media-Authorization Token is not present..No SBLP implemented\n"));
        }


	return ICF_SUCCESS;
}

icf_return_t icf_print_sec_pdp_activate_resp(icf_msg_st *p_msg)
{
	icf_port_pltfm_pdp_def_resp_st	*p_port_resp=(icf_port_pltfm_pdp_def_resp_st*)(p_msg->payload);
	
    icf_print_api_header(p_msg);

    

	ICF_PRINT_MESSAGE(( 
		"\nPDP Context ID = %u", p_port_resp->context_id));

	ICF_PRINT_MESSAGE(( 
		"\nStatus = %u", p_port_resp->status));

	
	return ICF_SUCCESS;
}

icf_return_t icf_print_pdp_deactivate_req(icf_msg_st *p_msg)
{
    icf_gprs_pdp_deactivate_req_st *p_msg_payload = ICF_NULL;
    
    p_msg_payload = (icf_gprs_pdp_deactivate_req_st*)p_msg->payload;

    icf_print_api_header(p_msg);

   

	ICF_PRINT_MESSAGE(( 
		"\nPDP Context ID = %u", p_msg_payload->context_id));


	return ICF_SUCCESS;
}


icf_return_t icf_print_pdp_modify_ind(icf_msg_st *p_msg)
{
    icf_port_pltfm_pdp_mod_ind_st	*p_port_resp=(icf_port_pltfm_pdp_mod_ind_st*)(p_msg->payload);
	
    icf_print_api_header(p_msg);

   

	ICF_PRINT_MESSAGE(( 
		"\nPDP Context ID = %u", p_port_resp->context_id));

	ICF_PRINT_MESSAGE(( 
		"\nStatus = %u", p_port_resp->status));

	
	return ICF_SUCCESS;
}

icf_return_t icf_print_pdp_deactivate_ind(icf_msg_st *p_msg)
{
	icf_port_pltfm_pdp_deact_ind_st	*p_port_resp=(icf_port_pltfm_pdp_deact_ind_st*)(p_msg->payload);
	
    icf_print_api_header(p_msg);

    
	ICF_PRINT_MESSAGE(( 
		"\nPDP Context ID = %u", p_port_resp->context_id));

	ICF_PRINT_MESSAGE(( 
		"\nStatus = %u", p_port_resp->status));

	
	return ICF_SUCCESS;
}

icf_return_t icf_print_platform_event_ind(icf_msg_st *p_msg)
{
    icf_port_pltfm_event_ind_st	*p_port_resp=(icf_port_pltfm_event_ind_st*)(p_msg->payload);
	
    icf_print_api_header(p_msg);

    

	ICF_PRINT_MESSAGE(( 
		"\nPDP Event = %u", p_port_resp->event));

	
	return ICF_SUCCESS;
}


/* -------------------------------------------------------------------------- */
#ifdef ICF_IPSEC_ENABLED

icf_void_t  icf_ipsec_set_spi(icf_uint32_t spi)
{
    if (ICF_NULL == app_global_data.spi_1)
    {
        app_global_data.spi_1 = spi;
        app_global_data.spi_expiry = spi;
    }
    else if (ICF_NULL == app_global_data.spi_2)
    {
        app_global_data.spi_2 = spi;
    }
    else if (ICF_NULL == app_global_data.spi_3)
    {
        app_global_data.spi_3 = spi;
    }
    else if (ICF_NULL == app_global_data.spi_4)
    {
        app_global_data.spi_4 = spi;
    }
    else if (ICF_NULL == app_global_data.spi_5)
    {
        app_global_data.spi_5 = spi;
    }
    else if (ICF_NULL == app_global_data.spi_6)
    {
        app_global_data.spi_6 = spi;
    }
    else if (ICF_NULL == app_global_data.spi_7)
    {
        app_global_data.spi_7 = spi;
    }
    else if (ICF_NULL == app_global_data.spi_8)
    {
        app_global_data.spi_8 = spi;
    }
}

icf_return_t icf_print_sa_create_req(icf_msg_st *p_msg)
{

	return ICF_SUCCESS;
}

#ifdef ICF_SA_LIFETIME_MOD_SUPPORT
icf_return_t icf_print_sa_modify_req(icf_msg_st *p_msg)
{

	return ICF_SUCCESS;
}
#endif /* ICF_SA_LIFETIME_MOD_SUPPORT */

icf_return_t icf_print_sa_delete_req(icf_msg_st *p_msg)
{

	return ICF_SUCCESS;
}


icf_return_t icf_print_sa_cmd_resp(icf_msg_st *p_msg)
{
	return ICF_SUCCESS;
}

icf_return_t icf_print_sa_expiry_ind(icf_msg_st *p_msg)
{
	return ICF_SUCCESS;
}

#endif /* ICF_IPSEC_ENABLED */


/*ICF_HANDLE_REINVITE_START*/

icf_return_t icf_print_incoming_call_modify_ind(icf_msg_st *p_msg)
{

    icf_incoming_call_modify_ind_st* p_reinvite_ind = NULL;
    
    icf_uint8_t  header_list_count = 0,body_list_count = 0,i=0;

    icf_list_st *p_list=ICF_NULL;
    icf_print_api_header(p_msg);

    p_reinvite_ind = (icf_incoming_call_modify_ind_st*)p_msg->payload;
    
	ICF_PRINT_MESSAGE(("\nbit_mask = %u\n", 
              p_reinvite_ind->bit_mask));

/* Header LIST  */    if (p_reinvite_ind->bit_mask & ICF_HDR_LIST_PRESENT) {
        header_list_count = (icf_uint8_t)p_reinvite_ind->header_list.count;
        icf_print_header_list(p_reinvite_ind->header_list.hdr, header_list_count);
    }

/* Body LIST  */
    if (p_reinvite_ind->bit_mask & ICF_BODY_LIST_PRESENT) {
        body_list_count = (icf_uint8_t)p_reinvite_ind->body_list.count;
        icf_print_body_list(p_reinvite_ind->body_list.body, body_list_count);
    }

/* Stream LIST  */
/* TBD */
#ifdef ICF_REL_OLD
    if (p_reinvite_ind->bit_mask & ICF_STREAM_LIST_PRESENT) {
        stream_list_count = p_reinvite_ind->stream_list.count;
        icf_print_stream_list(p_reinvite_ind->stream_list.stream, stream_list_count);
    }

#endif

     p_list=p_reinvite_ind->p_active_stream_list;
  
	if(p_reinvite_ind->bit_mask & ICF_INC_CALL_ACCEPT_STREAM_LIST_PRESENT)
	{
		ICF_PRINT_MESSAGE((
			"\n Accepted Stream List:\n"));

		i = 1;

		while(p_list!=ICF_NULL)
		{
			ICF_PRINT_MESSAGE(("\nNode %u:",i));

			icf_print_modify_media_change_req(p_list->p_data);
			
			p_list = p_list->p_next;
			
			i++;
		}
	}

	if(p_reinvite_ind->bit_mask & ICF_INC_CALL_MOD_DELETED_STREAM_PRESENT)
	{
		ICF_PRINT_MESSAGE(("\n Rejected stream count %u:",p_reinvite_ind->count));	

		for(i=0;i<p_reinvite_ind->count;i++)
		{
			ICF_PRINT_MESSAGE(("\n stream id %u:",p_reinvite_ind->deleted_stream_list[i].stream_id));

			ICF_PRINT_MESSAGE(("\n stream type %u:",p_reinvite_ind->deleted_stream_list[i].stream_type));
		}
	}


   
    return ICF_SUCCESS;

}


icf_return_t icf_print_call_modify_cfm(icf_msg_st *p_msg)
{

    icf_call_modify_cfm_st* p_reinvite_cfm = NULL;

    icf_uint8_t  header_list_count = 0, tag_list_count = 0,
                    body_list_count = 0;
    
    icf_print_api_header(p_msg);
    p_reinvite_cfm = (icf_call_modify_cfm_st *)p_msg->payload;
     
    ICF_PRINT_MESSAGE(("\nbit_mask = %u", 
              p_reinvite_cfm->bit_mask));
     

	ICF_PRINT_MESSAGE(("\nmedia_change_result = %u", 
              p_reinvite_cfm->result));

	ICF_PRINT_MESSAGE(("\nreject_reason = %u", 
              p_reinvite_cfm->cause));

/* Header LIST  */    if (p_reinvite_cfm->bit_mask & ICF_HDR_LIST_PRESENT) {
        header_list_count = (icf_uint8_t)p_reinvite_cfm->header_list.count;
        icf_print_header_list(p_reinvite_cfm->header_list.hdr, header_list_count);
    }


/* Tag LIST  */    if (p_reinvite_cfm->bit_mask & ICF_TAG_LIST_PRESENT) {
        tag_list_count = (icf_uint8_t)p_reinvite_cfm->tag_list.count;
        icf_print_tag_list(p_reinvite_cfm->tag_list.tag, tag_list_count);
    }
    
    
    /* Body LIST  */
    if (p_reinvite_cfm->bit_mask & ICF_BODY_LIST_PRESENT) {
        body_list_count = (icf_uint8_t)p_reinvite_cfm->body_list.count;
        icf_print_body_list(p_reinvite_cfm->body_list.body, body_list_count);
    }
    
    /* Stream LIST  */
/* TBD */
#ifdef ICF_REL_OLD
    if (p_reinvite_cfm->bit_mask & ICF_STREAM_LIST_PRESENT) {
        stream_list_count = (icf_uint8_t)p_reinvite_cfm->stream_list.count;
        icf_print_stream_list(p_reinvite_cfm->stream_list.stream, stream_list_count);
    }
#endif


   
    return ICF_SUCCESS;
}


icf_return_t icf_print_incoming_call_modify_cancel_ind(icf_msg_st *p_msg)
{
	icf_print_api_header(p_msg);
	return ICF_SUCCESS;
}

icf_return_t icf_print_call_modify_resp(icf_msg_st *p_msg)
{

    icf_call_modify_resp_st* p_reinvite_resp = NULL;

    icf_uint8_t  header_list_count = 0,body_list_count = 0,i=0;
 
    icf_list_st *p_list=ICF_NULL;                    
    icf_print_api_header(p_msg);
    p_reinvite_resp = (icf_call_modify_resp_st *)p_msg->payload;
     
    ICF_PRINT_MESSAGE(("\nbit_mask = %u", 
              p_reinvite_resp->bit_mask));
     

	ICF_PRINT_MESSAGE(("\nmedia_change_result = %u", 
              p_reinvite_resp->result));

	ICF_PRINT_MESSAGE(("\nreject_reason = %u", 
              p_reinvite_resp->error_cause));
        icf_print_error_details(p_reinvite_resp->error_cause);

	if (p_reinvite_resp->bit_mask & ICF_RESPONSE_CODE_PRESENT) {
		ICF_PRINT_MESSAGE(("\nresponse_code = %u", 
              p_reinvite_resp->response_code));
	}

/* Header LIST  */    if (p_reinvite_resp->bit_mask & ICF_HDR_LIST_PRESENT) {
        header_list_count = (icf_uint8_t)p_reinvite_resp->header_list.count;
        icf_print_header_list(p_reinvite_resp->header_list.hdr, header_list_count);
    }

    
    /* Body LIST  */
    if (p_reinvite_resp->bit_mask & ICF_BODY_LIST_PRESENT) {
        body_list_count = (icf_uint8_t)p_reinvite_resp->body_list.count;
        icf_print_body_list(p_reinvite_resp->body_list.body, body_list_count);
    }
    
    /* Stream LIST  */
/* TBD */
#ifdef ICF_REL_OLD
    if (p_reinvite_resp->bit_mask & ICF_STREAM_LIST_PRESENT) {
        stream_list_count = p_reinvite_resp->stream_list.count;
        icf_print_stream_list(p_reinvite_resp->stream_list.stream, stream_list_count);
    }
#endif


        p_list=p_reinvite_resp->p_active_stream_list;
	if(p_reinvite_resp->bit_mask & ICF_CALL_MOD_ACCEPT_STREAM_LIST_PRESENT)
	{
		ICF_PRINT_MESSAGE(("\n Accepted Stream List:\n"));

		i = 1;

		while(p_list!=ICF_NULL)
		{
			ICF_PRINT_MESSAGE(("\nNode %u:",i));

			icf_print_modify_media_change_req(p_list->p_data);
			
			p_list = p_list->p_next;
			
			i++;
		}
	}

	if(p_reinvite_resp->bit_mask & ICF_CALL_MOD_RESP_REJECT_STREAM_PRESENT)
	{
		ICF_PRINT_MESSAGE(("\n Rejected stream count %u:",p_reinvite_resp->count));	

		for(i=0;i<p_reinvite_resp->count;i++)
		{
			ICF_PRINT_MESSAGE(("\n stream id %u:",p_reinvite_resp->deleted_stream_list[i].stream_id));

			ICF_PRINT_MESSAGE(("\n stream type %u:",p_reinvite_resp->deleted_stream_list[i].stream_type));
		}
	}




    
    return ICF_SUCCESS;
}

icf_return_t icf_print_call_modify_req(icf_msg_st *p_msg)
{

    icf_call_modify_req_st* p_reinvite_req = NULL;
    
    icf_uint8_t  header_list_count = 0, tag_list_count = 0,
                    body_list_count = 0,i=0;
	icf_list_st	 *p_media_modify_list;

    icf_print_api_header(p_msg);

    p_reinvite_req = (icf_call_modify_req_st*)p_msg->payload;
    
	ICF_PRINT_MESSAGE(("\nbit_mask = %u", 
              p_reinvite_req->bit_mask));

/* Header LIST  */    if (p_reinvite_req->bit_mask & ICF_HDR_LIST_PRESENT) {
        header_list_count = (icf_uint8_t)p_reinvite_req->header_list.count;
        icf_print_header_list(p_reinvite_req->header_list.hdr, header_list_count);
    }

	/* Tag LIST  */    if (p_reinvite_req->bit_mask & ICF_TAG_LIST_PRESENT) {
        tag_list_count = (icf_uint8_t)p_reinvite_req->tag_list.count;
        icf_print_tag_list(p_reinvite_req->tag_list.tag, tag_list_count);
    }

/* Body LIST  */
    if (p_reinvite_req->bit_mask & ICF_BODY_LIST_PRESENT) {
        body_list_count = (icf_uint8_t)p_reinvite_req->body_list.count;
        icf_print_body_list(p_reinvite_req->body_list.body, body_list_count);
    }

/* Stream LIST  */
/* TBD */
#ifdef ICF_REL_OLD
    if (p_reinvite_req->bit_mask & ICF_STREAM_LIST_PRESENT) {
        stream_list_count = p_reinvite_req->stream_list.count;
        icf_print_stream_list(p_reinvite_req->stream_list.stream, stream_list_count);
    }
#endif



	if(p_reinvite_req->bit_mask & ICF_CALL_MOD_REQ_MEDIA_MOD_LIST_PRESENT)
	{
		ICF_PRINT_MESSAGE(("\n Accepted Stream List:\n"));

		i = 1;

		p_media_modify_list = p_reinvite_req->p_media_modify_list;

		while(p_media_modify_list!=ICF_NULL)
		{
			ICF_PRINT_MESSAGE(("\nNode %u:",i));

			icf_print_modify_media_change_req(p_media_modify_list->p_data);
			
			p_media_modify_list = p_media_modify_list->p_next;
			
			i++;
		}
	}

	if(p_reinvite_req->bit_mask & ICF_CALL_MOD_REQ_DELETED_STREAM_PRESENT)
	{
		ICF_PRINT_MESSAGE(("\n Rejected stream count %u:",p_reinvite_req->count));	

		for(i=0;i<p_reinvite_req->count;i++)
		{
			ICF_PRINT_MESSAGE(("\n stream id %u:",p_reinvite_req->deleted_media_streams[i].stream_id));

			ICF_PRINT_MESSAGE(("\n stream type %u:",p_reinvite_req->deleted_media_streams[i].stream_type));
		}
	}




   
    return ICF_SUCCESS;

}


/* -------------------------------------------------------------------------- */


/*************** Management Plane Handler (MPH) APIs Starts here *************/




icf_return_t icf_print_create_secure_conn_resp(icf_msg_st *p_msg)
{
	icf_create_secure_conn_resp_st      *p_sec_conn_resp = ICF_NULL;
       p_sec_conn_resp = (icf_create_secure_conn_resp_st *)p_msg->payload;

    icf_print_api_header(p_msg);
    /* g_default_app_id = p_msg->hdr.app_id; */

    

	/*	icf_uint16_t	exchange_id;
		icf_result_t	conn_req_status;
		icf_uint32_t	conn_handle;
		icf_uint32_t	error_cause; */

	ICF_PRINT_MESSAGE((
        "exchange_id = %u, conn_req_status=%u, conn_handle = %u, error cause = %u",  
		p_sec_conn_resp->exchange_id, p_sec_conn_resp->conn_req_status ,p_sec_conn_resp->conn_handle,
        p_sec_conn_resp->error_cause));
	icf_print_error_details(p_sec_conn_resp->error_cause);


	
	return ICF_SUCCESS;
}



icf_return_t icf_print_set_close_secure_conn_resp(icf_msg_st *p_msg)
{
	icf_close_secure_conn_resp_st      *p_close_sec_conn_resp = ICF_NULL;
       p_close_sec_conn_resp = (icf_close_secure_conn_resp_st *)p_msg->payload;

    icf_print_api_header(p_msg);
    /* g_default_app_id = p_msg->hdr.app_id; */

    

	/*	icf_uint16_t	exchange_id;
		icf_result_t	close_conn_status;
		icf_uint32_t	conn_handle;
		icf_uint32_t	error_cause; */

	ICF_PRINT_MESSAGE((
        "conn_status = %u, close_conn_status=%u, conn_handle = %u", 
		p_close_sec_conn_resp->exchange_id, p_close_sec_conn_resp->close_conn_status ,p_close_sec_conn_resp->conn_handle));


	
	return ICF_SUCCESS;
}

icf_return_t icf_print_secure_conn_status_ind(icf_msg_st *p_msg)
{
	icf_secure_conn_status_ind_st      *p_conn_status_ind = ICF_NULL;
       p_conn_status_ind = (icf_secure_conn_status_ind_st*)p_msg->payload;

    icf_print_api_header(p_msg);
    /* g_default_app_id = p_msg->hdr.app_id; */

    
	/*	icf_uint8_t		conn_status;
		icf_uint32_t	conn_handle; */

    ICF_PRINT_MESSAGE((
        "conn_status = %u, conn_handle = %u", 
		p_conn_status_ind->conn_status, p_conn_status_ind->conn_handle));

	

	
	return ICF_SUCCESS;
}





icf_return_t icf_print_mgmt_plane_api_proc_failure(icf_msg_st *p_msg)
{
    icf_api_proc_failure_st				*p_payload = ICF_NULL; 

    p_payload = (icf_api_proc_failure_st *)p_msg->payload;

	
	return ICF_SUCCESS;
}

icf_return_t icf_print_set_mgmt_plane_nw_resp(icf_msg_st *p_msg)
{
	icf_mgmt_plane_nw_resp_st      *p_nw_resp = ICF_NULL;
       p_nw_resp = (icf_mgmt_plane_nw_resp_st *)p_msg->payload;

    icf_print_api_header(p_msg);
    /* g_default_app_id = p_msg->hdr.app_id; */

   
	/* 
	icf_uint8_t				bitmask;
	icf_api_id_t			req_api;
	icf_uint16_t			exchange_id;
	icf_uint32_t			conn_handle;
	icf_stretchable_hdr_st  entity_tag;
	icf_string_st			content_type;
	icf_header_list_st		header_list;
	icf_uint32_t			response_code;
	icf_uint32_t			payload_length;
	icf_uint8_t				*payload; */

	ICF_PRINT_MESSAGE((
        "\nexchange_id = %u, conn_handle = %u, payload_length = %u", 
		p_nw_resp->exchange_id, p_nw_resp->conn_handle, p_nw_resp->payload_length));

	if (p_nw_resp->payload_length > 0)
	{
		ICF_PRINT_MESSAGE(("\npayload = %s", p_nw_resp->payload));
	}

	

	

	return ICF_SUCCESS;
}

/*************** Management Plane Handler (MPH) APIs Ends here ***************/

icf_return_t icf_print_call_merged_ind(icf_msg_st* p_msg)
{

    icf_cc_pa_call_merged_ind_st* p_call_merge = NULL;
       
    p_call_merge = (icf_cc_pa_call_merged_ind_st*)p_msg->payload;
       
    icf_print_api_header(p_msg);
       
    ICF_PRINT_MESSAGE(("\nmerged_call_id = %u", 
              p_call_merge->merge_call_id));

    return ICF_SUCCESS;
     
}


icf_return_t icf_print_rem_call_transfer_init(icf_msg_st* p_msg)
{
    
    icf_uint8_t presence_flag = 0;    
    icf_cc_pa_remote_call_transfer_initiated_ind_st* 
    p_remote_call_transfer = NULL;
     
    p_remote_call_transfer = 
    (icf_cc_pa_remote_call_transfer_initiated_ind_st*)p_msg->payload;
    
    presence_flag = (icf_uint8_t)p_remote_call_transfer->bit_mask;
   
    icf_print_api_header(p_msg);
    

    ICF_PRINT_MESSAGE(( 
              "\nnew_call_id  = %u",
              p_remote_call_transfer->new_call_id));
   
    ICF_PRINT_MESSAGE(( 
              "\nremote_call_transfer = (addr_type = %u, %s)",
              p_remote_call_transfer->referred_party_addr.addr_type ,
              p_remote_call_transfer->referred_party_addr.addr_val.str));

    if(presence_flag & ICF_CC_PA_REFER_INITIATED_REFERRED_PARTY_NAME)
    {
        ICF_PRINT_MESSAGE((
                 "\nreferred_party_name = %s",
          p_remote_call_transfer->referred_party_name.str));
    }
        
    ICF_PRINT_MESSAGE((
             "\npresence_flag = %u", 
              p_remote_call_transfer->bit_mask));
    
    return ICF_SUCCESS;
}
     
icf_return_t icf_print_rem_call_transfer_cfm(icf_msg_st* p_msg)
{

    icf_uint8_t presence_flag = 0;    
    icf_cc_pa_remote_call_transfer_initiated_ind_st* 
    p_remote_call_transfer = NULL;

    p_remote_call_transfer = 
    (icf_cc_pa_remote_call_transfer_initiated_ind_st*)p_msg->payload;

    presence_flag = (icf_uint8_t)p_remote_call_transfer->bit_mask;

    icf_print_api_header(p_msg);


    ICF_PRINT_MESSAGE(( 
              "\nnew_call_id  = %u",
              p_remote_call_transfer->new_call_id));

    ICF_PRINT_MESSAGE((
              "\nremote_call_transfer = (addr_type = %u, %s)",
              p_remote_call_transfer->referred_party_addr.addr_type ,
              p_remote_call_transfer->referred_party_addr.addr_val.str));

    if(presence_flag & ICF_CC_PA_REFER_INITIATED_REFERRED_PARTY_NAME)
    {
        ICF_PRINT_MESSAGE((
                 "\nreferred_party_name = %s",
          p_remote_call_transfer->referred_party_name.str));
    }

    ICF_PRINT_MESSAGE((
             "\npresence_flag = %u",
              p_remote_call_transfer->bit_mask));

    return ICF_SUCCESS;
}

icf_return_t icf_print_cta_req(icf_msg_st* p_msg)
{
    icf_pa_cc_call_transfer_attended_req_st* 
    p_call_transfer_attended_req = NULL;
     
    p_call_transfer_attended_req = 
        (icf_pa_cc_call_transfer_attended_req_st*)p_msg->payload;
     
    icf_print_api_header(p_msg);
            
    ICF_PRINT_MESSAGE(( 
              "\ntransferred_party_id = %u",
              p_call_transfer_attended_req->transferred_party_id));
 
    return ICF_SUCCESS;

}


icf_return_t icf_print_ctu_req(icf_msg_st* p_msg)
{
    
/*

api_id=,source_module_id=,dest_module_id=,call_id=
{
transferred_party_address=(addr_type=,%s),presence_flag=, 
toggle_presentation_ind=
}

*/

        icf_uint32_t presence_flag = 0;
     
    icf_pa_cc_call_transfer_unattended_req_st* 
    p_call_transfer_unattended = NULL;
     
    p_call_transfer_unattended = 
         (icf_pa_cc_call_transfer_unattended_req_st*)p_msg->payload;
    
    presence_flag = p_call_transfer_unattended->bit_mask;
         
    icf_print_api_header(p_msg);
        
    ICF_PRINT_MESSAGE(("presence_flag = %u",
             p_call_transfer_unattended->bit_mask));

    ICF_PRINT_MESSAGE(( 
              "\ntransferred_party_address = (addr_type = %u, %s),",
              p_call_transfer_unattended->transferred_party_addr.addr_type,
              p_call_transfer_unattended->transferred_party_addr.addr_val.str));
    

    if (presence_flag & ICF_PA_CC_CALL_TRANSFER_UNATTENDED_REQ_TOGGLE_PRESENTATION_INDICATOR)
    {
        ICF_PRINT_MESSAGE(( 
              "\ntoggle_presentation_indicator = %u ",
              p_call_transfer_unattended->toggle_presentation_indicator));
    }

    return ICF_SUCCESS;
}

icf_return_t icf_print_conference_req(icf_msg_st* p_msg)
{
/*
api_id=,source_module_id=,dest_module_id=,call_id=
{
merge_call_id=
}
*/
    icf_pa_cc_conference_req_st* p_conference_req = NULL;

    icf_print_api_header(p_msg);
        
    p_conference_req = (icf_pa_cc_conference_req_st*)p_msg->payload;
      
    ICF_PRINT_MESSAGE(("\nmerge_call_id = %u", 
              p_conference_req->merge_call_id));
     
    return ICF_SUCCESS;
}

icf_return_t icf_print_call_redirection_ind(icf_msg_st* p_msg)
{
/*
api_id=,source_module_id=,dest_module_id=,call_id=
{
presence_flag=,redirected_party_address=(addr_type=,%s),
redirected_party_name=%s
}
*/
       icf_cc_pa_call_redirection_ind_st* p_call_redir = NULL;
    
    icf_print_api_header(p_msg);
    
    p_call_redir = (icf_cc_pa_call_redirection_ind_st*)p_msg->payload;
    
    if (ICF_CALL_RDR_IND_PTY_ADDR & p_call_redir->bit_mask)
        icf_print_icf_address_st(&(p_call_redir->redirected_party_addr));

    if (ICF_CALL_RDR_IND_PTY_NAME & p_call_redir->bit_mask)
        ICF_PRINT_MESSAGE((
        "\nself_name = %s)",
        p_call_redir->redirected_party_name.str));

    if (ICF_HDR_LIST_PRESENT & p_call_redir->bit_mask)
                icf_print_header_list(p_call_redir->header_list.hdr, p_call_redir->header_list.count);

    return ICF_SUCCESS;
}


icf_void_t icf_print_icf_address_st(icf_address_st *p_uri)
{
    ICF_PRINT_MESSAGE(( 
        "\naddr_type = %u, addr_val = %s", 
        p_uri->addr_type,p_uri->addr_val.str));
}


icf_void_t icf_print_pub_uri_list(icf_pub_uri_list_node_st *p_list)
{
    icf_uint8_t i,j;
    icf_address_list_node_st *p_assoc_uri_list = ICF_NULL;
    
    for(i=1; ICF_NULL != p_list;p_list=p_list->p_next,i++)
    {
        
        ICF_PRINT_MESSAGE(( 
					"\nPublic Uri %u",i));
        icf_print_icf_address_st(&(p_list->pub_uri));
        
        for(j=1,p_assoc_uri_list = p_list->p_assoc_uri_list; 
        ICF_NULL != p_assoc_uri_list;
        p_assoc_uri_list = p_assoc_uri_list->p_next,j++)
        {
            ICF_PRINT_MESSAGE(( 
                "\n\tAssociated Uri %u", j));
            icf_print_icf_address_st(&(p_assoc_uri_list->user_addr));
        }
   }
}


/* MM API's */

icf_return_t icf_print_max_mic_gain_req(icf_msg_st *p_msg)
{
    icf_cfg_max_mic_gain_req_st* p_req = (icf_cfg_max_mic_gain_req_st*)p_msg->payload;
    
    icf_print_api_header(p_msg);

    
    ICF_PRINT_MESSAGE((\
			"\nmax_mic_gain = %u", p_req->max_mic_gain));

/*    ICF_PRINT_MESSAGE((
		    "\nbit_mask = %u", p_req->bit_mask));*/

   
    return ICF_SUCCESS;

}

icf_return_t icf_print_max_mic_gain_resp(icf_msg_st *p_msg)
{
	icf_print_mm_msg_resp(p_msg);
	return ICF_SUCCESS;
}

icf_return_t icf_print_mic_gain_req(icf_msg_st *p_msg)
{
    icf_set_mic_gain_req_st* p_req = (icf_set_mic_gain_req_st*)p_msg->payload;
    
    icf_print_api_header(p_msg);

   
    ICF_PRINT_MESSAGE((\
			"\ngain_level = %u", p_req->gain_level));

/*    ICF_PRINT_MESSAGE((
		    "\nbit_mask = %u", p_req->bit_mask));*/

   
    return ICF_SUCCESS;

}

icf_return_t icf_print_mic_gain_resp(icf_msg_st *p_msg)
{
	icf_print_mm_msg_resp(p_msg);
	return ICF_SUCCESS;
}
#ifdef ICF_STAT_ENABLED
icf_return_t icf_print_get_icf_stats_req(icf_msg_st *p_msg)
{
    icf_get_icf_stats_req_st* p_req = (icf_get_icf_stats_req_st*)p_msg->payload;
    
    icf_print_api_header(p_msg);

    
    ICF_PRINT_MESSAGE((\
			"\nstats_id = %u", p_req->stats_id));

    ICF_PRINT_MESSAGE((
		    "\nreset = %u", p_req->reset));

    return ICF_SUCCESS;
}

icf_return_t icf_print_get_icf_stats_resp(icf_msg_st *p_msg)
{
    icf_uint8_t        i=0;
    icf_stats_resp_st* p_req = (icf_stats_resp_st*)p_msg->payload;

    icf_print_api_header(p_msg);

   
    ICF_PRINT_MESSAGE((\
			"\nresult = %u", p_req->result));

    ICF_PRINT_MESSAGE((
		    "\nstats_id = %u", p_req->stats_id));

    ICF_PRINT_MESSAGE((
		    "\nerror_cause = %u", p_req->error_cause));
    icf_print_error_details(p_req->error_cause);

    ICF_PRINT_MESSAGE((
		    "\nnum_of_stats_elements = %u", p_req->num_of_stats_elements));

    for (i=0; i < p_req->num_of_stats_elements; i++)
        ICF_PRINT_MESSAGE((
		    "\nstats_data[%u] = %u", i, p_req->stats_data[i]));

   
    return ICF_SUCCESS;
}
#endif

icf_return_t icf_print_info_req(icf_msg_st *p_msg)
{

    icf_info_req_st* p_info_req = NULL;
    
    icf_uint8_t  header_list_count = 0, tag_list_count = 0,
                    body_list_count = 0;

    icf_print_api_header(p_msg);

    p_info_req = (icf_info_req_st*)p_msg->payload;
    
	ICF_PRINT_MESSAGE(("\nbit_mask = %u", 
              p_info_req->bit_mask));

/* Header LIST  */    if (p_info_req->bit_mask & ICF_HDR_LIST_PRESENT) {
        header_list_count = (icf_uint8_t)p_info_req->header_list.count;
        icf_print_header_list(p_info_req->header_list.hdr, header_list_count);
    }

    if (p_info_req->bit_mask & ICF_TAG_LIST_PRESENT) {
        tag_list_count = (icf_uint8_t)p_info_req->tag_list.count;
        icf_print_tag_list(p_info_req->tag_list.tag, tag_list_count);
    }

/* Body LIST  */
    if (p_info_req->bit_mask & ICF_BODY_LIST_PRESENT) {
        body_list_count = (icf_uint8_t)p_info_req->body_list.count;
        icf_print_body_list(p_info_req->body_list.body, body_list_count);
    }

    
    return ICF_SUCCESS;

}


icf_return_t icf_print_chd_req(icf_msg_st *p_msg)
{

    icf_call_hold_req_st* p_chd_req = NULL;
    
    icf_uint8_t  header_list_count = 0, tag_list_count = 0,
                    body_list_count = 0,i=0;

    icf_print_api_header(p_msg);

    p_chd_req = (icf_call_hold_req_st*)p_msg->payload;
    
	ICF_PRINT_MESSAGE(("\nbit_mask = %u", 
              p_chd_req->bit_mask));

/* Header LIST  */    if (p_chd_req->bit_mask & ICF_HDR_LIST_PRESENT) {
        header_list_count = (icf_uint8_t)p_chd_req->header_list.count;
        icf_print_header_list(p_chd_req->header_list.hdr, header_list_count);
    }

    if (p_chd_req->bit_mask & ICF_TAG_LIST_PRESENT) {
        tag_list_count = (icf_uint8_t)p_chd_req->tag_list.count;
        icf_print_tag_list(p_chd_req->tag_list.tag, tag_list_count);
    }

/* Body LIST  */
    if (p_chd_req->bit_mask & ICF_BODY_LIST_PRESENT) {
        body_list_count = (icf_uint8_t)p_chd_req->body_list.count;
        icf_print_body_list(p_chd_req->body_list.body, body_list_count);
    }


	if(p_chd_req->bit_mask & ICF_STREAM_LIST_PRESENT)
	{
		ICF_PRINT_MESSAGE(("\n stream_list count = %u",p_chd_req->stream_list.count));
	
		for(i=0;i<p_chd_req->stream_list.count;i++)
		{
			ICF_PRINT_MESSAGE(("\n stream_list id = %u", 
				  p_chd_req->stream_list.stream_id[i]));
		}
	}



   
    return ICF_SUCCESS;

}


icf_return_t icf_print_crm_req(icf_msg_st *p_msg)
{

    icf_call_resume_req_st* p_crm_req = NULL;
    
    icf_uint8_t  header_list_count = 0, tag_list_count = 0,
                    body_list_count = 0,i=0;

    icf_print_api_header(p_msg);

    p_crm_req = (icf_call_resume_req_st*)p_msg->payload;
    
	ICF_PRINT_MESSAGE(("\nbit_mask = %u",p_crm_req->bit_mask)); 
             

/* Header LIST  */    if (p_crm_req->bit_mask & ICF_HDR_LIST_PRESENT) {
        header_list_count = (icf_uint8_t)p_crm_req->header_list.count;
        icf_print_header_list(p_crm_req->header_list.hdr, header_list_count);
    }

    if (p_crm_req->bit_mask & ICF_TAG_LIST_PRESENT) {
        tag_list_count = (icf_uint8_t)p_crm_req->tag_list.count;
        icf_print_tag_list(p_crm_req->tag_list.tag, tag_list_count);
    }

/* Body LIST  */
    if (p_crm_req->bit_mask & ICF_BODY_LIST_PRESENT) {
        body_list_count = (icf_uint8_t)p_crm_req->body_list.count;
        icf_print_body_list(p_crm_req->body_list.body, body_list_count);
    }



	if(p_crm_req->bit_mask & ICF_STREAM_LIST_PRESENT)
	{
		ICF_PRINT_MESSAGE(("\n stream_list count = %u",p_crm_req->stream_list.count));
	
		for(i=0;i<p_crm_req->stream_list.count;i++)
		{
			ICF_PRINT_MESSAGE(("\n stream_list id = %u", 
				  p_crm_req->stream_list.stream_id[i]));
		}
	}



    
    return ICF_SUCCESS;

}

/*****************************************************************************
 *
 * FUNCTION:        icf_print_ctu_resp()
 *
 * DESCRIPTION:     Print response messages 
 *
 * Functions called: 
 *                     
 *
 ****************************************************************************/

icf_return_t icf_print_ctu_resp(icf_msg_st* p_msg)
{

    icf_ctu_resp_st* p_ctu_resp = NULL;

    icf_print_api_header(p_msg);

    p_ctu_resp = (icf_ctu_resp_st*)p_msg->payload;

    ICF_PRINT_MESSAGE(( 
              "\nresult = %u, error_cause = %u",
              p_ctu_resp->result, p_ctu_resp->error_cause));
    icf_print_error_details(p_ctu_resp->error_cause);

    ICF_PRINT_MESSAGE(("\n{\nbit_mask = %u", 
              p_ctu_resp->bit_mask));

/* Header LIST  */    if (p_ctu_resp->bit_mask & ICF_HDR_LIST_PRESENT) {
        icf_print_header_list(p_ctu_resp->header_list.hdr, p_ctu_resp->header_list.count);
    }    

    
    return ICF_SUCCESS;
}


/*****************************************************************************
 *
 * FUNCTION:        icf_print_cta_resp()
 *
 * DESCRIPTION:     Print response messages 
 *
 * Functions called: 
 *                     
 *
 ****************************************************************************/

icf_return_t icf_print_cta_resp(icf_msg_st* p_msg)
{

    icf_cta_resp_st* p_cta_resp = NULL;

    icf_print_api_header(p_msg);

    p_cta_resp = (icf_cta_resp_st*)p_msg->payload;

    ICF_PRINT_MESSAGE(( 
              "\nresult = %u, error_cause = %u",
              p_cta_resp->result, p_cta_resp->error_cause));
    icf_print_error_details(p_cta_resp->error_cause);

    ICF_PRINT_MESSAGE(("\n{\nbit_mask = %u", 
              p_cta_resp->bit_mask));

/* Header LIST  */    if (p_cta_resp->bit_mask & ICF_HDR_LIST_PRESENT) {
        icf_print_header_list(p_cta_resp->header_list.hdr, p_cta_resp->header_list.count);
    }    

    
    return ICF_SUCCESS;
}


/*****************************************************************************
 *
 * FUNCTION:        icf_print_api_ind()
 *
 * DESCRIPTION:     Print response messages 
 *
 * Functions called: 
 *                     
 *
 ****************************************************************************/

icf_return_t icf_print_api_ind(icf_msg_st* p_msg)
{

    icf_api_ind_st* p_resp_msg = NULL;

	icf_uint8_t		i = 0;

    icf_print_api_header(p_msg);

    p_resp_msg = (icf_api_ind_st*)p_msg->payload;

    ICF_PRINT_MESSAGE(("\nbit_mask = %u", 
              p_resp_msg->bit_mask));

/* Header LIST  */    if (p_resp_msg->bit_mask & ICF_HDR_LIST_PRESENT) {
        icf_print_header_list(p_resp_msg->header_list.hdr, p_resp_msg->header_list.count);
    }    

/* Body LIST  */
    if (p_resp_msg->bit_mask & ICF_BODY_LIST_PRESENT) {
        icf_print_body_list(p_resp_msg->body_list.body, p_resp_msg->body_list.count);
    }

/* TBD */
#ifdef ICF_REL_OLD
    if (p_resp_msg->bit_mask & ICF_STREAM_LIST_PRESENT) {
        icf_print_stream_list(p_resp_msg->stream_list.stream, p_resp_msg->stream_list.count);
    }
#endif



	if(p_resp_msg->bit_mask & ICF_STREAM_LIST_PRESENT)/* for deleted stream lists */
	{
		ICF_PRINT_MESSAGE(("\n Deleted stream count %u:",p_resp_msg->stream_list.count));	

		for(i=0;i<p_resp_msg->stream_list.count;i++)
		{
			ICF_PRINT_MESSAGE(("\n stream id %u:",p_resp_msg->stream_list.stream_id[i]));
		}
	}



    
    return ICF_SUCCESS;
}


icf_return_t icf_print_rtp_start_ind(icf_msg_st *p_msg)
{

    icf_rtp_start_ind_st* p_rtp_start_ind = NULL;

    icf_print_api_header(p_msg);

    p_rtp_start_ind = (icf_rtp_start_ind_st*)p_msg->payload;

    ICF_PRINT_MESSAGE(( 
              "\nstream_id = %u, ssrc = %L",
              p_rtp_start_ind->stream_id, p_rtp_start_ind->ssrc));

    return ICF_SUCCESS;
}


icf_return_t icf_print_unexpected_msg_ind(icf_msg_st* p_msg)
{
        icf_unexpected_msg_ind_st                  *p_unexpected_msg = ICF_NULL;

        icf_print_api_header(p_msg);

        p_unexpected_msg = (icf_unexpected_msg_ind_st*)p_msg->payload;
        
        ICF_PRINT_MESSAGE(("\nError Cause = %u",
              p_unexpected_msg->error_cause));

        ICF_PRINT_MESSAGE(("\nApi_id = %u",
              p_unexpected_msg->api_id));

        return ICF_SUCCESS;
}




#ifdef ICF_FAX_SUPPORT

icf_return_t icf_print_start_fax_req(icf_msg_st *p_msg)
{

    icf_start_fax_req_st* p_start_fax_req = NULL;
    
    icf_print_api_header(p_msg);

    p_start_fax_req = (icf_start_fax_req_st*)p_msg->payload;

   
	ICF_PRINT_MESSAGE(("\nFax Type = %u", 
              p_start_fax_req->fax_type));

	ICF_PRINT_MESSAGE(("\nPref Fax Codec = %u", 
              p_start_fax_req->fax_pt_codec));

	
	return ICF_SUCCESS;
}

icf_return_t icf_print_start_fax_resp(icf_msg_st *p_msg)
{

    icf_start_fax_resp_st* p_start_fax_resp = NULL;
    
    icf_print_api_header(p_msg);

    p_start_fax_resp = (icf_start_fax_resp_st*)p_msg->payload;

    
	ICF_PRINT_MESSAGE(("\nResult = %u", 
              p_start_fax_resp->result));

	ICF_PRINT_MESSAGE(("\nError Cause = %u", 
              p_start_fax_resp->error_cause));
        icf_print_error_details(p_start_fax_resp->error_cause);

	ICF_PRINT_MESSAGE(("\nFax Type = %u", 
              p_start_fax_resp->fax_type));



	return ICF_SUCCESS;
}


icf_return_t icf_print_start_fax_ind(icf_msg_st *p_msg)
{

    icf_start_fax_ind_st* p_start_fax_ind = NULL;
    
    icf_print_api_header(p_msg);

    p_start_fax_ind = (icf_start_fax_ind_st*)p_msg->payload;
    
	ICF_PRINT_MESSAGE(("\nFax Type = %u", 
              p_start_fax_ind->fax_type));

       return ICF_SUCCESS;

}

icf_return_t icf_print_stop_fax_resp(icf_msg_st *p_msg)
{

    icf_stop_fax_resp_st* p_stop_fax_resp = NULL;
   
    icf_print_api_header(p_msg);

    p_stop_fax_resp = (icf_stop_fax_resp_st*)p_msg->payload;

   
        ICF_PRINT_MESSAGE(("\nResult = %u",
              p_stop_fax_resp->result));

        ICF_PRINT_MESSAGE(("\nError Cause = %u",
              p_stop_fax_resp->error_cause));
        icf_print_error_details(p_stop_fax_resp->error_cause);

        return ICF_SUCCESS;
}

icf_return_t icf_print_stop_fax_ind(icf_msg_st *p_msg)
{

    icf_stop_fax_ind_st* p_stop_fax_ind = NULL;

    icf_print_api_header(p_msg);

    p_stop_fax_ind = (icf_stop_fax_ind_st*)p_msg->payload;

        ICF_PRINT_MESSAGE(("\nFax Type = %u",
              p_stop_fax_ind->fax_type));

       return ICF_SUCCESS;

}

#endif

icf_return_t icf_print_call_mod_cancel_resp(icf_msg_st *p_msg)
{

    icf_call_modify_cancel_resp_st* p_call_mod_cancel_resp = NULL;

    icf_print_api_header(p_msg);

    p_call_mod_cancel_resp = (icf_call_modify_cancel_resp_st*)p_msg->payload;


        ICF_PRINT_MESSAGE(("\nResult = %u",
              p_call_mod_cancel_resp->result));

        ICF_PRINT_MESSAGE(("\nError Cause = %u",
              p_call_mod_cancel_resp->error_cause));
        icf_print_error_details(p_call_mod_cancel_resp->error_cause);

        return ICF_SUCCESS;
}

icf_return_t icf_print_remote_video_diaplay_req(icf_msg_st *p_msg)
{

    icf_cfg_set_remote_video_display_st* p_remote_video_display_req = NULL;
    
    icf_print_api_header(p_msg);

    p_remote_video_display_req = (icf_cfg_set_remote_video_display_st*)p_msg->payload;
    
	/*ICF_PRINT_MESSAGE(("\nWindow Handle = %u", 
              p_remote_video_display_req->window_handle)));*/

	ICF_PRINT_MESSAGE(("\nTOP = %u", 
              p_remote_video_display_req->top));

	ICF_PRINT_MESSAGE(("\nLEFT = %u", 
              p_remote_video_display_req->left));
	

	ICF_PRINT_MESSAGE(("\nWIDTH = %u", 
              p_remote_video_display_req->width));

	ICF_PRINT_MESSAGE(("\nHEIGHT = %u", 
              p_remote_video_display_req->height));

    
    return ICF_SUCCESS;

}

icf_return_t icf_print_preview_video_diaplay_req(icf_msg_st *p_msg)
{

    icf_cfg_set_preview_video_display_st* p_preview_video_display_req = NULL;
    
    icf_print_api_header(p_msg);

    p_preview_video_display_req = (icf_cfg_set_preview_video_display_st*)p_msg->payload;

	/*ICF_PRINT_MESSAGE(("\nWindow Handle = %u", 
              p_preview_video_display_req->window_handle));*/

    
	ICF_PRINT_MESSAGE(("\nTOP = %u", 
              p_preview_video_display_req->top));

	ICF_PRINT_MESSAGE(("\nLEFT = %u", 
              p_preview_video_display_req->left));

	ICF_PRINT_MESSAGE(("\nWIDTH = %u", 
              p_preview_video_display_req->width));

	ICF_PRINT_MESSAGE(("\nHEIGHT = %u", 
              p_preview_video_display_req->height));

   
    return ICF_SUCCESS;

}


icf_return_t icf_print_connect_media_device_req(icf_msg_st* p_msg)
{
    icf_connect_media_device_req_st* p_con_media_dev_req = NULL;

    p_con_media_dev_req = (icf_connect_media_device_req_st*)p_msg->payload;

    icf_print_api_header(p_msg);

   
    ICF_PRINT_MESSAGE(("\n bit_mask = %u:",p_con_media_dev_req->bit_mask));

    if (p_con_media_dev_req->bit_mask & ICF_LINE_ID_PRESENT)
        ICF_PRINT_MESSAGE(("\n Line id = %u:",p_con_media_dev_req->line_id));

    ICF_PRINT_MESSAGE(("\n Media Device = %u:",p_con_media_dev_req->media_device));
	
    ICF_PRINT_MESSAGE(("\n Connect Status = %u:",p_con_media_dev_req->connect_status));

   
    return ICF_SUCCESS;

}

icf_return_t icf_print_mms_resp(icf_msg_st* p_msg)
{

    icf_uint8_t i = 1;


    icf_modify_media_session_resp_st* p_mms_resp = ICF_NULL;


    icf_list_st *p_list=ICF_NULL;    
 
    p_mms_resp = (icf_modify_media_session_resp_st*)p_msg->payload;

    icf_print_api_header(p_msg);

    
    ICF_PRINT_MESSAGE(("\nbitmask = %u, result = %u, error_cause = %u",
        p_mms_resp->bit_mask, p_mms_resp->result, p_mms_resp->error_cause));
    icf_print_error_details(p_mms_resp->error_cause);

    p_list=p_mms_resp->p_accepted_stream_list;
	while(p_list!=ICF_NULL)
	{
		ICF_PRINT_MESSAGE(("\nNode %u:",i));

		icf_print_resp_stream((icf_resp_stream_st*)(p_list->p_data),i);
		
		p_list = p_list->p_next;
		
		i++;
	}

	if(p_mms_resp->bit_mask & ICF_MODIFY_MEDIA_RESP_REJECT_STREAM_PRESENT)
	{
		ICF_PRINT_MESSAGE((" \nDeleted media count = %u", p_mms_resp->rejected_stream_list.count));

		for(i=0;i<p_mms_resp->rejected_stream_list.count;i++)
		{
			ICF_PRINT_MESSAGE((" \nStream ID = %u", p_mms_resp->rejected_stream_list.stream_id[i]));
		}
	}

    return ICF_SUCCESS;
}


icf_return_t icf_print_codec_commit_resp(icf_msg_st* p_msg)
{

    icf_uint8_t i = 1, j=0,k=0;

    icf_list_st *p_list= ICF_NULL;
    icf_codec_commit_resp_st* p_ccr_resp = ICF_NULL;

    p_ccr_resp = (icf_codec_commit_resp_st*)p_msg->payload;

    icf_print_api_header(p_msg);

   
    ICF_PRINT_MESSAGE(("\nbitmask = %u, result = %u, error_cause = %u",
        p_ccr_resp->bit_mask, p_ccr_resp->result, p_ccr_resp->error_cause));
    icf_print_error_details(p_ccr_resp->error_cause);

    p_list=p_ccr_resp->p_commited_stream_list;

	while(p_list!=ICF_NULL)
	{
		ICF_PRINT_MESSAGE(("\nNode %u:",i));

		ICF_PRINT_MESSAGE((" \nStream ID = %u",
				((icf_stream_commit_st*)(p_list->p_data))->stream_id));

		ICF_PRINT_MESSAGE((" \nCodec Count = %u",
				((icf_stream_commit_st*)(p_list->p_data))->codec_count));

		for(j=0;j<((icf_stream_commit_st*)(p_list->p_data))->codec_count;j++)
		{
			if(((icf_stream_commit_st*)(p_list->p_data))->codec[j].bit_mask &
				ICF_CODEC_ID_CODEC_ID_PRESENT)
			{
				ICF_PRINT_MESSAGE((" \nCodec ID = %u", 
					((icf_stream_commit_st*)(p_list->p_data))->codec[j].codec_id));
			}

			if(((icf_stream_commit_st*)(p_list->p_data))->codec[j].bit_mask &
				ICF_CODEC_ID_CODEC_NAME_PRESENT)
			{

				k = 0;

				ICF_PRINT_MESSAGE((" \nCodec Name = "));

				while(k<((icf_stream_commit_st*)(p_list->p_data))->codec[j].codec_name.str_len)
				{
					ICF_PRINT_MESSAGE(("%c",
						((icf_stream_commit_st*)(p_list->p_data))->codec[j].codec_name.str[k]));

					k++;
				}
			}
			
		}
		
		p_list = p_list->p_next;
		
		i++;
	}

	if(p_ccr_resp->bit_mask & ICF_CODEC_COMMIT_RESP_REJ_STREAM_LIST_PRESENT)
	{
		ICF_PRINT_MESSAGE((" \n Rejected Stream List Count = %u", p_ccr_resp->rejected_stream_list.count));

		for(i=0;i<p_ccr_resp->rejected_stream_list.count;i++)
		{
			ICF_PRINT_MESSAGE((" \nStream ID = %u", p_ccr_resp->rejected_stream_list.stream_id[i]));
		}
	}

    return ICF_SUCCESS;
}

icf_return_t icf_print_mute_resp(icf_msg_st *p_msg)
{
	icf_mute_media_resp_st* p_media_mute_resp = NULL;

    p_media_mute_resp = (icf_mute_media_resp_st*)p_msg->payload;

    icf_print_api_header(p_msg);

   
	ICF_PRINT_MESSAGE(("\n Result = %u:",p_media_mute_resp->result));
	
	ICF_PRINT_MESSAGE(("\n Error Cause = %u:",p_media_mute_resp->error_cause));
	icf_print_error_details(p_media_mute_resp->error_cause);

    if (p_media_mute_resp->bit_mask & ICF_MUTE_MEDIA_RESP_LINE_ID_PRESENT)
    {
       ICF_PRINT_MESSAGE(("\n Line_id = %u:",p_media_mute_resp->line_id)); 
    }
    
    return ICF_SUCCESS;
}

icf_return_t icf_print_cfg_media_req(icf_msg_st *p_msg)
{
	icf_uint8_t		i = 0;

	icf_cfg_media_req_st* p_cfg_media_req = NULL;

    p_cfg_media_req = (icf_cfg_media_req_st*)p_msg->payload;

    icf_print_api_header(p_msg);

  
	ICF_PRINT_MESSAGE(("\n Count = %u:",p_cfg_media_req->count));

	for(i=0;i<p_cfg_media_req->count;i++)
	{
		ICF_PRINT_MESSAGE(("\n Media ID = %u:",p_cfg_media_req->media[i].media_id));

		ICF_PRINT_MESSAGE(("\n Play File = %s:",p_cfg_media_req->media[i].play_file.str));
	}

    return ICF_SUCCESS;
}

icf_return_t icf_print_play_media_req(icf_msg_st *p_msg)
{
	icf_uint8_t		i = 0;
	
	icf_play_media_req_st* p_play_media_req = NULL;

    p_play_media_req = (icf_play_media_req_st*)p_msg->payload;

    icf_print_api_header(p_msg);

    
    if (p_play_media_req->is_present & ICF_PLAY_MEDIA_LINE_ID_PRESENT)
        ICF_PRINT_MESSAGE(("\n Line id = %u:",p_play_media_req->line_id));

	if(p_play_media_req->is_present & ICF_MEDIA_ID_PRESENT)
		ICF_PRINT_MESSAGE(("\n Media ID = %u:",p_play_media_req->media_id));

	if(p_play_media_req->is_present & ICF_FILE_NAME_PRESENT)
	{
		ICF_PRINT_MESSAGE(("\n File Name = ",p_play_media_req->file_name.str[i]));
		
		for(i=0;i<p_play_media_req->file_name.str_len;i++)
		{
			ICF_PRINT_MESSAGE(("%c",p_play_media_req->file_name.str[i]));
		}
	}

	ICF_PRINT_MESSAGE(("\n Play Mode = %u:",p_play_media_req->play_mode));

	ICF_PRINT_MESSAGE(("\n Duration = %u:",p_play_media_req->duration));

	if(p_play_media_req->is_present & ICF_PLAY_TONE_TIMER_VAL)
		ICF_PRINT_MESSAGE(("\n Count = %u:",p_play_media_req->timer_value));

   	
	return ICF_SUCCESS;
}

icf_return_t icf_print_stop_media_play_req(icf_msg_st *p_msg)
{

	
	icf_stop_media_play_req_st* p_stop_media_play_req = NULL;

    p_stop_media_play_req = (icf_stop_media_play_req_st*)p_msg->payload;

    icf_print_api_header(p_msg);

  
    /*if (p_stop_media_play_req->is_present & ICF_STOP_MEDIA_PLAY_REQ_LINE_ID_PRESENT)
        ICF_PRINT_MESSAGE(("\n Line id = %u:",p_stop_media_play_req->line_id));

	if(p_stop_media_play_req->is_present & ICF_MEDIA_ID_PRESENT)
		ICF_PRINT_MESSAGE(("\n Media ID = %u:",p_stop_media_play_req->media_id));

	if(p_stop_media_play_req->is_present & ICF_FILE_NAME_PRESENT)
	{
		ICF_PRINT_MESSAGE(("\n File Name = ",p_stop_media_play_req->file_name.str[i]));
		
		for(i=0;i<p_stop_media_play_req->file_name.str_len;i++)
		{
			ICF_PRINT_MESSAGE(("%c",p_stop_media_play_req->file_name.str[i]));
		}
	}*/

	
	return ICF_SUCCESS;
}

icf_return_t icf_print_stop_media_play_ind(icf_msg_st *p_msg)
{
	icf_uint8_t		i = 0;
	
	icf_stop_media_play_ind_st* p_stop_media_play_ind = NULL;

    p_stop_media_play_ind = (icf_stop_media_play_ind_st*)p_msg->payload;

    icf_print_api_header(p_msg);

    
    if (p_stop_media_play_ind->is_present & ICF_STOP_MEDIA_PLAY_IND_LINE_ID_PRESENT)
        ICF_PRINT_MESSAGE(("\n Line id = %u:",p_stop_media_play_ind->line_id));

	if(p_stop_media_play_ind->is_present & ICF_MEDIA_ID_PRESENT)
		ICF_PRINT_MESSAGE(("\n Media ID = %u:",p_stop_media_play_ind->media_id));

	if(p_stop_media_play_ind->is_present & ICF_FILE_NAME_PRESENT)
	{
		ICF_PRINT_MESSAGE(("\n File Name = ",p_stop_media_play_ind->file_name.str[i]));
		
		for(i=0;i<p_stop_media_play_ind->file_name.str_len;i++)
		{
			ICF_PRINT_MESSAGE(("%c",p_stop_media_play_ind->file_name.str[i]));
		}
	}

	
	return ICF_SUCCESS;
}

/******************************************************************************
 *
 * FUNCTION:        icf_print_media_record_req
 *
 * DESCRIPTION:     This function is used to print media_record_req
 *
 *****************************************************************************/

icf_return_t icf_print_media_record_req(icf_msg_st* p_msg)
{
    icf_uint8_t i = 0;

    icf_media_record_req_st* p_media_record_req = ICF_NULL;

    p_media_record_req = (icf_media_record_req_st*)p_msg->payload;

    icf_print_api_header(p_msg);

  
    ICF_PRINT_MESSAGE(("\n bit_mask = %u:",p_media_record_req->bit_mask));

    if (p_media_record_req->bit_mask & ICF_MEDIA_RECORD_LINE_ID_PRESENT)
        ICF_PRINT_MESSAGE(("\n Line id = %u:",p_media_record_req->line_id));
	
	ICF_PRINT_MESSAGE(("\nMedia Source:%u",
									p_media_record_req->media_source));

	ICF_PRINT_MESSAGE(("\nFile Type:%u",
									p_media_record_req->file_type));

	ICF_PRINT_MESSAGE(("\nFile name: "));

	for(i=0;i<p_media_record_req->file_name.str_len;i++)
		ICF_PRINT_MESSAGE(("%c",p_media_record_req->file_name.str[i]));


	ICF_PRINT_MESSAGE(("\nBit mask:%u",
									p_media_record_req->bit_mask));

	if(p_media_record_req->bit_mask & ICF_STREAM_ID_PRESENT){
		ICF_PRINT_MESSAGE(("\nStream id:%u",
									p_media_record_req->stream_id));
	}


	return ICF_SUCCESS;

}

/******************************************************************************
 *
 * FUNCTION:        icf_print_stop_media_record_req
 *
 * DESCRIPTION:     This function is used to print stop_media_record_req
 *
 *****************************************************************************/

icf_return_t icf_print_stop_media_record_req(icf_msg_st* p_msg)
{
  
    icf_stop_media_record_req_st* p_stop_media_record_req = ICF_NULL;

    p_stop_media_record_req = (icf_stop_media_record_req_st*)p_msg->payload;

    icf_print_api_header(p_msg);

   
    ICF_PRINT_MESSAGE(("\n bit_mask = %u:",p_stop_media_record_req->bit_mask));

    if (p_stop_media_record_req->bit_mask & ICF_STOP_MEDIA_RECORD_LINE_ID_PRESENT)
        ICF_PRINT_MESSAGE(("\n Line id = %u:",p_stop_media_record_req->line_id));
	
	ICF_PRINT_MESSAGE(("\nMedia Source:%u",
									p_stop_media_record_req->media_source));

	ICF_PRINT_MESSAGE(("\nBit mask:%u",
									p_stop_media_record_req->bit_mask));

	if(p_stop_media_record_req->bit_mask & ICF_STREAM_ID_PRESENT){
		ICF_PRINT_MESSAGE(("\nStream id:%u",
									p_stop_media_record_req->stream_id));
	}


	return ICF_SUCCESS;

}

/******************************************************************************
 *
 * FUNCTION:        icf_print_pause_media_req
 *
 * DESCRIPTION:     This function is used to print pause_media_req
 *
 *****************************************************************************/

icf_return_t icf_print_pause_media_req(icf_msg_st* p_msg)
{
    icf_uint8_t i = 0;

    icf_pause_media_req_st* p_pause_media_req = ICF_NULL;

    p_pause_media_req = (icf_pause_media_req_st*)p_msg->payload;

    icf_print_api_header(p_msg);

  
    ICF_PRINT_MESSAGE(("\n bit_mask = %u:",p_pause_media_req->bit_mask));

    if (p_pause_media_req->bit_mask & ICF_LINE_ID_PRESENT)
        ICF_PRINT_MESSAGE(("\n Line id = %u:",p_pause_media_req->line_id));

	ICF_PRINT_MESSAGE(("\nList of Stream ids"));

	for(i=0;i<p_pause_media_req->stream_id.count;i++)
		ICF_PRINT_MESSAGE(("\nStream Id %u: %u",
									i+1,p_pause_media_req->stream_id.stream_id[i]));

	ICF_PRINT_MESSAGE(("\nActivate Pause:%u",
									p_pause_media_req->activate_pause));

	
	return ICF_SUCCESS;

}

/******************************************************************************
 *
 * FUNCTION:        icf_print_set_media_playback_device_req
 *
 * DESCRIPTION:     This function is used to print set_media_playback_device_req
 *
 *****************************************************************************/

icf_return_t icf_print_set_media_playback_device_req(icf_msg_st* p_msg)
{
   
    icf_set_media_playback_device_req_st* p_set_media_req = ICF_NULL;

    p_set_media_req = (icf_set_media_playback_device_req_st*)p_msg->payload;

    icf_print_api_header(p_msg);

  
    ICF_PRINT_MESSAGE(("\n bit_mask = %u:",p_set_media_req->bit_mask)); 

    if (p_set_media_req->bit_mask & ICF_LINE_ID_PRESENT) 
         ICF_PRINT_MESSAGE(("\n Line id = %u:",p_set_media_req->line_id));

	ICF_PRINT_MESSAGE(("\nMedia playout device:%u",
									p_set_media_req->media_play_out_device));


	return ICF_SUCCESS;

}


/******************************************************************************
 *
 * FUNCTION:        icf_print_mute_media_req
 *
 * DESCRIPTION:     This function is used to print mute_media_req
 *
 *****************************************************************************/

icf_return_t icf_print_mute_media_req(icf_msg_st* p_msg)
{
    icf_uint8_t i = 0;

    icf_mute_media_req_st* p_mute_media_req = ICF_NULL;

    p_mute_media_req = (icf_mute_media_req_st*)p_msg->payload;

    icf_print_api_header(p_msg);

   
    ICF_PRINT_MESSAGE(("\n bit_mask = %u:",p_mute_media_req->bit_mask));

    if (p_mute_media_req->bit_mask & ICF_LINE_ID_PRESENT)
         ICF_PRINT_MESSAGE(("\n Line id = %u:",p_mute_media_req->line_id));

	ICF_PRINT_MESSAGE(("\nList of Stream ids"));

	for(i=0;i<p_mute_media_req->stream_list.count;i++)
		ICF_PRINT_MESSAGE(("\nStream Id %u: %u",
									i+1,p_mute_media_req->stream_list.stream_id[i]));

	ICF_PRINT_MESSAGE(("\nActivate Mute:%u",
									p_mute_media_req->activate_mute));

	
	return ICF_SUCCESS;

}

/******************************************************************************
 *
 * FUNCTION:        icf_print_dgts_ind
 *
 * DESCRIPTION:     This function is used to print dgts_ind
 *
 *****************************************************************************/

icf_return_t icf_print_dgts_ind(icf_msg_st* p_msg)
{
    icf_uint8_t i = 0;

    icf_dgts_ind_st* p_icf_dgts = ICF_NULL;

    p_icf_dgts = (icf_dgts_ind_st*)p_msg->payload;

    icf_print_api_header(p_msg);

    
    ICF_PRINT_MESSAGE(("\n bit_mask = %u:",p_icf_dgts->bit_mask));

    if (p_icf_dgts->bit_mask & ICF_LINE_ID_PRESENT)
         ICF_PRINT_MESSAGE(("\n Line id = %u:",p_icf_dgts->line_id));

	ICF_PRINT_MESSAGE(("\nStream Id: %u",
									p_icf_dgts->stream_id));
	
	ICF_PRINT_MESSAGE(("\nDigits:"));

	for(i=0;i<p_icf_dgts->digits.number_of_dgts;i++)
		ICF_PRINT_MESSAGE(("\nDigit %u: %u",
									i+1, p_icf_dgts->digits.dgts[i]));


	return ICF_SUCCESS;

}

icf_return_t icf_print_vibration_req(icf_msg_st* p_msg)
{
    
    icf_vibration_req_st* p_vibration_req = ICF_NULL;

    p_vibration_req = (icf_vibration_req_st*)p_msg->payload;

    icf_print_api_header(p_msg);

   
	ICF_PRINT_MESSAGE(("\nVibration Id: %u",p_vibration_req->vibration_id));
	
	ICF_PRINT_MESSAGE(("\nDuration: %u",p_vibration_req->duration));

	if(p_vibration_req->bit_mask & ICF_VIBRATION_TIMER_VAL)
		ICF_PRINT_MESSAGE(("\nTimer Value: %u",p_vibration_req->timer_value));


	return ICF_SUCCESS;

}

icf_return_t icf_print_echo_canc_req(icf_msg_st* p_msg)
{
    
    icf_echo_canc_req_st* p_echo_canc_req = ICF_NULL;

    p_echo_canc_req = (icf_echo_canc_req_st*)p_msg->payload;

    icf_print_api_header(p_msg);

    
	ICF_PRINT_MESSAGE(("\nEcho Cancel: %u",p_echo_canc_req->echo_cancel));

	if(p_echo_canc_req->bit_mask & ICF_ECHO_CANC_STREAM_ID)
		ICF_PRINT_MESSAGE(("\nStream ID: %u",p_echo_canc_req->stream_id));


	return ICF_SUCCESS;

}

icf_return_t icf_print_vad_ss_cng_req(icf_msg_st* p_msg)
{
    
    icf_vad_ss_cng_req_st* p_vad_ss_cng_req = ICF_NULL;

    p_vad_ss_cng_req = (icf_vad_ss_cng_req_st*)p_msg->payload;

    icf_print_api_header(p_msg);

    
	if(p_vad_ss_cng_req->bit_mask & ICF_VAD_STREAM_ID)
		ICF_PRINT_MESSAGE(("\nStream ID: %u",p_vad_ss_cng_req->stream_id));

	if(p_vad_ss_cng_req->bit_mask & ICF_VAD_SUPP)
		ICF_PRINT_MESSAGE(("\nVad Supp: %u",p_vad_ss_cng_req->vad_supp));


	if(p_vad_ss_cng_req->bit_mask & ICF_VAD_SILENCE_SUPP)
		ICF_PRINT_MESSAGE(("\nSilence Supp: %u",p_vad_ss_cng_req->silence_supp));


	if(p_vad_ss_cng_req->bit_mask & ICF_VAD_COMFORT_NOISE)
		ICF_PRINT_MESSAGE(("\nComfort Noise: %u",p_vad_ss_cng_req->comfort_noise));

	
	return ICF_SUCCESS;

}

icf_return_t icf_print_stop_vibration_req(icf_msg_st *p_msg)
{
    icf_print_api_header(p_msg);

   
    return ICF_SUCCESS;

}
/******************************************************************************
 *
 * FUNCTION:        icf_print_send_dgts_req
 *
 * DESCRIPTION:     This function is used to print send_dgts_req
 *
 *****************************************************************************/
icf_return_t icf_print_send_dgts_req(icf_msg_st *p_msg)
{
    icf_send_dgts_st* p_send_dgts = (icf_send_dgts_st*)p_msg->payload;
    
    icf_print_api_header(p_msg);

    	
    ICF_PRINT_MESSAGE((\
			"\nStream Id = %u", p_send_dgts->stream_id));

	if(p_send_dgts->bit_mask & ICF_SEND_DGT_TRANSPORT_METHOD_PRESENT)
		ICF_PRINT_MESSAGE((\
				"\nTransport Method = %u", p_send_dgts->transport_method));
	ICF_PRINT_MESSAGE((\
			"\nDigits = %u", p_send_dgts->digits));

    
    return ICF_SUCCESS;

}

icf_return_t icf_print_med_cap_resp(icf_msg_st* p_msg)
{

    icf_uint8_t i = 1;

    icf_media_capabilities_resp_st* p_med_cap_resp = ICF_NULL;
    icf_list_st *ptemp = ICF_NULL;

    p_msg=p_msg;

    p_med_cap_resp = (icf_media_capabilities_resp_st*)p_msg->payload;

    icf_print_api_header(p_msg);


    ICF_PRINT_MESSAGE(("\nresult = %u, error_cause = %u",
        p_med_cap_resp->result, p_med_cap_resp->error_cause));
    icf_print_error_details(p_med_cap_resp->error_cause);

    ptemp = p_med_cap_resp->p_resp_stream_list;
        while(ptemp!=ICF_NULL)
        {
                ICF_PRINT_MESSAGE(("\nNode %u:",i));

                icf_print_resp_stream((icf_resp_stream_st*)(ptemp->p_data),i);

	        ptemp = ptemp->p_next;
	
		i++;
	}

        
        return ICF_SUCCESS;
}
/******************************************************************************
 *
 * FUNCTION:        icf_print_call_fwd_ind
 *
 * DESCRIPTION:     This function is used to print call_forwarded_ind
 *
 *****************************************************************************/
icf_return_t icf_print_call_fwd_ind(icf_msg_st *p_msg)
{
    icf_call_forwarded_ind_st* p_call_fwd = (icf_call_forwarded_ind_st*)p_msg->payload;

    icf_uint32_t i = 0;
    
    icf_print_api_header(p_msg);

   	
    ICF_PRINT_MESSAGE((\
			"\nRemote party address ="));

	icf_print_icf_address_st(&p_call_fwd->remote_party_addr);

	if(p_call_fwd->bit_mask & ICF_REMOTE_PARTY_NAME)
	{
		ICF_PRINT_MESSAGE((\
				"\nRemote party name = "));

		for(i=0;i<p_call_fwd->remote_party_name.str_len;i++)
		{
			ICF_PRINT_MESSAGE(( 
              			"%c",p_call_fwd->remote_party_name.str[i]));
		}
	}

	ICF_PRINT_MESSAGE(("\nCall forwarded type = %u", p_call_fwd->call_forwarded_type));

				
  
    return ICF_SUCCESS;

}

/******************************************************************************
 *
 * FUNCTION:        icf_print_call_dms_req
 *
 * DESCRIPTION:     This function is used to print delete_media_session_req *
 *****************************************************************************/
icf_return_t icf_print_dms_req(icf_msg_st *p_msg)
{
    icf_uint8_t i;

	icf_delete_media_session_req_st* p_dms_req = (icf_delete_media_session_req_st*)p_msg->payload;
    
#ifdef ICF_FOR_LOAD_TESTING
	char command_string[ICF_MAX_BUFF_LEN];
#endif

    icf_print_api_header(p_msg);

  	
    ICF_PRINT_MESSAGE((" \nDeleted media count = %u", p_dms_req->stream_list.count));

	for(i=0;i<p_dms_req->stream_list.count;i++)
	{
		ICF_PRINT_MESSAGE((" \nStream ID = %u", p_dms_req->stream_list.stream_id[i]));
	}

   #ifdef ICF_FOR_LOAD_TESTING

	app_global_data.call_id = p_msg->hdr.call_id;
    strcpy(command_string,ICF_DEFAULT_DMS_RSP_MSG);
    icf_command_handler(command_string);

#endif
    return ICF_SUCCESS;

}





icf_return_t icf_print_notify_ind(icf_msg_st* p_msg)
{

    icf_notify_ind_st* p_notfy_ind = ICF_NULL;

    icf_uint8_t  header_list_count = 0, body_list_count = 0;

    
    p_notfy_ind = (icf_notify_ind_st*)p_msg->payload;

    icf_print_api_header(p_msg);


    
	ICF_PRINT_MESSAGE((
        "\nbit_mask = %u", p_notfy_ind->bit_mask));

	ICF_PRINT_MESSAGE((
        "\nline_id = %u", p_notfy_ind->line_id));




    ICF_PRINT_MESSAGE((
        "\nevent_pkg_name = %s", p_notfy_ind->event_pkg_name.str));
	
	ICF_PRINT_MESSAGE((
        "\nsubs_state = %u", p_notfy_ind->subs_state));

	ICF_PRINT_MESSAGE((
        "\nreason_code = %u", p_notfy_ind->reason_code));

	if (p_notfy_ind->bit_mask & ICF_SUBS_DURATION_PRESENT) {
		ICF_PRINT_MESSAGE((
			"\nduration = %u", p_notfy_ind->duration));
	}



	if (p_notfy_ind->bit_mask & ICF_NOTIFY_IND_SENDER_PRESENT) {
		ICF_PRINT_MESSAGE(( 
              "\nsender = (addr_type = %u, %s),",
              p_notfy_ind->sender.addr_type,
              p_notfy_ind->sender.addr_val.str));
	}

	if (p_notfy_ind->bit_mask & ICF_NOTIFY_IND_USER_ADDR_PRESENT) {
		ICF_PRINT_MESSAGE(( 
              "\nuser_address = (addr_type = %u, %s),",
              p_notfy_ind->user_address.addr_type,
              p_notfy_ind->user_address.addr_val.str));
	}

	if (p_notfy_ind->bit_mask & ICF_NOTIFY_IND_OUT_OF_DIALOG){
		ICF_PRINT_MESSAGE((
			"\nOut of Dialog Message = TRUE"));
	}



	/* Header LIST  */    if (p_notfy_ind->bit_mask & ICF_HDR_LIST_PRESENT) {
        header_list_count = (icf_uint8_t)p_notfy_ind->header_list.count;
        icf_print_header_list(p_notfy_ind->header_list.hdr, header_list_count);
    }
    
    /* Body LIST  */
    if (p_notfy_ind->bit_mask & ICF_BODY_LIST_PRESENT) {
        body_list_count = (icf_uint8_t)p_notfy_ind->body_list.count;
        icf_print_body_list(p_notfy_ind->body_list.body, body_list_count);
    }

    
    return ICF_SUCCESS;


}

/*****************************************************************************
 *
 * FUNCTION:        icf_print_notify_cfm()
 *
 * DESCRIPTION:     Print notify cfm messages 
 *
 * Functions called: 
 *                     
 *
 ****************************************************************************/
icf_return_t icf_print_notify_cfm(icf_msg_st* p_msg)
{

    icf_notify_cfm_st* p_notfy_cfm = ICF_NULL;
   
    p_notfy_cfm = (icf_notify_cfm_st*)p_msg->payload;

    icf_print_api_header(p_msg);


   
	ICF_PRINT_MESSAGE((
        "\nbit_mask = %u", p_notfy_cfm->bit_mask));


	ICF_PRINT_MESSAGE((
        "\nresult = %u", p_notfy_cfm->result));


	if (p_notfy_cfm->bit_mask & ICF_NOTIFY_ERROR_CAUSE_PRESENT) {
		ICF_PRINT_MESSAGE((
			"\nerror_code = %u", p_notfy_cfm->error_code));
	}

	if (p_notfy_cfm->bit_mask & ICF_NOTIFY_RESP_CODE_PRESENT) {
		ICF_PRINT_MESSAGE((
			"\nresponse_code = %u", p_notfy_cfm->response_code));
	}

    
    return ICF_SUCCESS;


}


icf_return_t icf_print_info_ind(icf_msg_st *p_msg)
{

    icf_info_ind_st* p_info_ind = NULL;
    
    icf_uint8_t  header_list_count = 0, body_list_count = 0;


    icf_print_api_header(p_msg);

    p_info_ind = (icf_info_ind_st*)p_msg->payload;
    
	ICF_PRINT_MESSAGE(("\nbit_mask = %u", 
              p_info_ind->bit_mask));

/* Header LIST  */    if (p_info_ind->bit_mask & ICF_HDR_LIST_PRESENT) {
        header_list_count = (icf_uint8_t)p_info_ind->header_list.count;
        icf_print_header_list(p_info_ind->header_list.hdr, header_list_count);
    }

/* Body LIST  */
    if (p_info_ind->bit_mask & ICF_BODY_LIST_PRESENT) {
        body_list_count = (icf_uint8_t)p_info_ind->body_list.count;
        icf_print_body_list(p_info_ind->body_list.body, body_list_count);
    }

    
    return ICF_SUCCESS;

}



icf_return_t icf_print_info_resp(icf_msg_st *p_msg)
{

    icf_info_resp_st* p_info_resp = NULL;
    
    icf_uint8_t  header_list_count = 0, body_list_count = 0;
                   

    icf_print_api_header(p_msg);

    p_info_resp = (icf_info_resp_st*)p_msg->payload;
   
	ICF_PRINT_MESSAGE(("\nbit_mask = %u", 
              p_info_resp->bit_mask));

	ICF_PRINT_MESSAGE(("\nresult = %u", 
              p_info_resp->result));

	if(ICF_FAILURE == p_info_resp->result)
	{
		ICF_PRINT_MESSAGE(("\nerror_cause = %u\n", 
				  p_info_resp->error_cause));
		icf_print_error_details(p_info_resp->error_cause);
	
		/*
		ICF_PRINT_MESSAGE(("response_code = %u\n", 
				  p_info_resp->response_code));
		*/
	}
	else
	{

	/* Header LIST  */		if (p_info_resp->bit_mask & ICF_HDR_LIST_PRESENT) {
			header_list_count = (icf_uint8_t)p_info_resp->header_list.count;
			icf_print_header_list(p_info_resp->header_list.hdr, header_list_count);
		}

	/* Body LIST  */
		if (p_info_resp->bit_mask & ICF_BODY_LIST_PRESENT) {
			body_list_count = (icf_uint8_t)p_info_resp->body_list.count;
			icf_print_body_list(p_info_resp->body_list.body, body_list_count);
		}
	}

  
    return ICF_SUCCESS;

}

icf_return_t icf_print_nw_status(icf_msg_st *p_msg)
{

    icf_nw_status_st* p_nw_status = NULL;
    
    icf_print_api_header(p_msg);

    p_nw_status = (icf_nw_status_st*)p_msg->payload;
    
	ICF_PRINT_MESSAGE(("\nnw_id = %u\n", 
				  p_nw_status->nw_id));

	ICF_PRINT_MESSAGE(("nw_status = %u", 
				  p_nw_status->nw_status));

   
    return ICF_SUCCESS;

}

icf_return_t icf_print_media_capabilities_req(icf_msg_st* p_msg)
{

	icf_media_capabilities_req_st *p_media_capab = ICF_NULL;

	icf_list_st	*p_req_stream_list = ICF_NULL;

	icf_uint8_t i =0;

    icf_print_api_header(p_msg);

    p_media_capab = (icf_media_capabilities_req_st*)p_msg->payload;
	
	if ( ICF_MED_CAP_STREAM_PRSNT ==
		 (p_media_capab->bit_mask & ICF_MED_CAP_STREAM_PRSNT))
	{
		p_req_stream_list = p_media_capab->p_req_stream_list;
		
		while(p_req_stream_list!=ICF_NULL)
		{
			ICF_PRINT_MESSAGE(("\nNode %u:",i));
			
			icf_print_stream_capab(p_req_stream_list->p_data);
			
			p_req_stream_list = p_req_stream_list->p_next;
			
			i++;
		}
		
	}

	return ICF_SUCCESS;
}
icf_return_t icf_print_options_cfm(icf_msg_st* p_msg)
{
    icf_options_cfm_st* p_options_cfm = NULL;

    icf_uint8_t  header_list_count = 0, tag_list_count = 0,
                    body_list_count = 0,i=0;
	icf_list_st	*p_app_sdp_list;

    
    icf_print_api_header(p_msg);
    p_options_cfm = (icf_options_cfm_st*)p_msg->payload;
     
    ICF_PRINT_MESSAGE(("\nbit_mask = %u", 
              p_options_cfm->bit_mask));
     
/* Header LIST  */    if (p_options_cfm->bit_mask & ICF_HDR_LIST_PRESENT) {
        header_list_count = (icf_uint8_t)p_options_cfm->header_list.count;
        icf_print_header_list(p_options_cfm->header_list.hdr, header_list_count);
    }


/* Tag LIST  */    if (p_options_cfm->bit_mask & ICF_TAG_LIST_PRESENT) {
        tag_list_count = (icf_uint8_t)p_options_cfm->tag_list.count;
        icf_print_tag_list(p_options_cfm->tag_list.tag, tag_list_count);
    }
    
    
    /* Body LIST  */
    if (p_options_cfm->bit_mask & ICF_BODY_LIST_PRESENT) {
        body_list_count = (icf_uint8_t)p_options_cfm->body_list.count;
        icf_print_body_list(p_options_cfm->body_list.body, body_list_count);
    }
    

	ICF_PRINT_MESSAGE(("\n Media Profile List:"));

	i = 1;

	p_app_sdp_list = p_options_cfm->p_app_sdp_list;

	while(p_app_sdp_list!=ICF_NULL)
	{
		ICF_PRINT_MESSAGE(("\nNode %u:",i));

		icf_print_stream_capab(p_app_sdp_list->p_data);
		
		p_app_sdp_list = p_app_sdp_list->p_next;
		
		i++;
	}

    ICF_PRINT_MESSAGE(("\nresult = %u", 
              p_options_cfm->result));

    ICF_PRINT_MESSAGE(("\ndelete count = %u", 
              p_options_cfm->count));

    ICF_PRINT_MESSAGE(("\nresult = %u", 
              p_options_cfm->result));
	for (i = 0; i < p_options_cfm->count; i++)
	{
		ICF_PRINT_MESSAGE(("\ndelete count = %u", 
			p_options_cfm->deleted_media_streams[i].stream_id));
		
		ICF_PRINT_MESSAGE(("\ndelete count = %u", 
			p_options_cfm->deleted_media_streams[i].stream_type));
		
	}


    return ICF_SUCCESS;
}

icf_return_t icf_print_options_ind(icf_msg_st* p_msg)
{
    icf_inc_options_ind_st* p_options_ind = NULL;

    icf_uint8_t  header_list_count = 0, tag_list_count = 0,
                    body_list_count = 0,i=0;
	icf_list_st	*p_app_sdp_list;

    
    icf_print_api_header(p_msg);
    p_options_ind = (icf_inc_options_ind_st*)p_msg->payload;
     
    ICF_PRINT_MESSAGE(("\nbit_mask = %u", 
              p_options_ind->bit_mask));
     
/* Header LIST  */    if (p_options_ind->bit_mask & ICF_HDR_LIST_PRESENT) {
        header_list_count = (icf_uint8_t)p_options_ind->header_list.count;
        icf_print_header_list(p_options_ind->header_list.hdr, header_list_count);
    }


/* Tag LIST  */    if (p_options_ind->bit_mask & ICF_TAG_LIST_PRESENT) {
        tag_list_count = (icf_uint8_t)p_options_ind->tag_list.count;
        icf_print_tag_list(p_options_ind->tag_list.tag, tag_list_count);
    }
    
    
    /* Body LIST  */
    if (p_options_ind->bit_mask & ICF_BODY_LIST_PRESENT) {
        body_list_count = (icf_uint8_t)p_options_ind->body_list.count;
        icf_print_body_list(p_options_ind->body_list.body, body_list_count);
    }
    

	ICF_PRINT_MESSAGE(("\n Media Profile List:"));

	i = 1;

	p_app_sdp_list = p_options_ind->p_app_sdp_list;

	while(p_app_sdp_list!=ICF_NULL)
	{
		ICF_PRINT_MESSAGE(("\nNode %u:",i));

		icf_print_stream_capab(p_app_sdp_list->p_data);
		
		p_app_sdp_list = p_app_sdp_list->p_next;
		
		i++;
	}

    return ICF_SUCCESS;

}

icf_return_t icf_print_options_req(icf_msg_st* p_msg)
{


    icf_options_req_st* p_options_req = ICF_NULL;

    icf_uint8_t  header_list_count = 0, body_list_count = 0, tag_list_count = 0;
        
    /* Fix for CSR 1-8223807 : SPR-20362*/
    icf_uint8_t  remove_hdr_list_count = 0;

    p_options_req = (icf_options_req_st*)p_msg->payload;

    icf_print_api_header(p_msg);


  

    ICF_PRINT_MESSAGE((
        "\nbit_mask = %u", p_options_req->bit_mask));


	ICF_PRINT_MESSAGE(( 
		"\ndestination_info = (addr_type = %u, %s)",
		p_options_req->destination_info.addr_type,
		p_options_req->destination_info.addr_val.str));


    ICF_PRINT_MESSAGE((
        "\npref_identity.str = %s", 
        
        p_options_req->pref_identity.addr_val.str));
	
	


	/* Header LIST  */    if (p_options_req->bit_mask & ICF_HDR_LIST_PRESENT) {
        header_list_count = (icf_uint8_t)p_options_req->header_list.count;
        icf_print_header_list(p_options_req->header_list.hdr, header_list_count);
    }
    
    /* Tag LIST  */    if (p_options_req->bit_mask & ICF_TAG_LIST_PRESENT) {
		tag_list_count = (icf_uint8_t)p_options_req->tag_list.count;
		icf_print_tag_list(p_options_req->tag_list.tag, tag_list_count);
    }
    
    
    /* Body LIST  */
    if (p_options_req->bit_mask & ICF_BODY_LIST_PRESENT) {
        body_list_count = (icf_uint8_t)p_options_req->body_list.count;
        icf_print_body_list(p_options_req->body_list.body, body_list_count);
    }
    
    /* Fix for CSR 1-8223807 : SPR-20362*/
    /* headers need to be remove from sip message*/
    if (p_options_req->bit_mask & ICF_OPTIONS_REQ_REMOVE_HDR_LIST_PRESENT) {
        remove_hdr_list_count = (icf_uint8_t)p_options_req->remove_hdr_list.count;
        ICF_PRINT_MESSAGE(("\n\nHeaders need to removed from OPTIONS sip message"));
        icf_print_header_name_list(p_options_req->remove_hdr_list.hdr,
                                                 remove_hdr_list_count);
    }
    
    return ICF_SUCCESS;


}



icf_return_t icf_print_options_resp(icf_msg_st* p_msg)
{

    icf_options_resp_st* p_options_resp = ICF_NULL;

    icf_uint8_t  header_list_count = 0;

	icf_uint8_t	 i = 1;

    
    p_options_resp = (icf_options_resp_st*)p_msg->payload;

    icf_print_api_header(p_msg);


    
	ICF_PRINT_MESSAGE(("\nbit_mask = %u", p_options_resp->bit_mask));


       
    ICF_PRINT_MESSAGE((
        "\nresult = %u, response_code = %u",p_options_resp->result, p_options_resp->response_code_for_options));

        

    ICF_PRINT_MESSAGE(("\nerror_code = %u",p_options_resp->error_code_for_options));
        
       


	/* Header LIST  */    if (p_options_resp->bit_mask & ICF_HDR_LIST_PRESENT) {
        header_list_count = (icf_uint8_t)p_options_resp->header_list.count;
        icf_print_header_list(p_options_resp->header_list.hdr, header_list_count);
    }


    /* Body LIST  */
	if (p_options_resp->bit_mask & ICF_BODY_LIST_PRESENT) {

		icf_print_body_list(p_options_resp->body_list.body,
			p_options_resp->body_list.count);
	}

	ICF_PRINT_MESSAGE((
       "\n Remote Stream List:"));


	while(p_options_resp->p_remote_stream_list!=ICF_NULL)
	{
		ICF_PRINT_MESSAGE(("\nNode %u:",i));

		icf_print_stream_capab(p_options_resp->p_remote_stream_list->p_data);

		p_options_resp->p_remote_stream_list = p_options_resp->p_remote_stream_list->p_next;
		
		i++;
	}


    
    return ICF_SUCCESS;

}


icf_return_t icf_print_info_cfm(icf_msg_st* p_msg)
{
    icf_info_cfm_st* p_info_cfm = ICF_NULL;

    p_info_cfm = (icf_info_cfm_st*)p_msg->payload;

    icf_print_api_header(p_msg);

       
    ICF_PRINT_MESSAGE(("\n Result = %u\n",p_info_cfm->result));
    ICF_PRINT_MESSAGE(("\n bit_mask = %u",p_info_cfm->bit_mask));

    if (p_info_cfm->bit_mask & ICF_INFO_CFM_RESPONSE_CODE_PRESENT)
        ICF_PRINT_MESSAGE(("\n Response Code = %u\n",p_info_cfm->response_code));

    if (p_info_cfm->bit_mask & ICF_INFO_CFM_ERROR_CAUSE_PRESENT)
        ICF_PRINT_MESSAGE(("\n Error Cause = %u",p_info_cfm->error_cause));
	icf_print_error_details(p_info_cfm->error_cause);

    if (p_info_cfm->bit_mask & ICF_INFO_CFM_HEADER_LIST_PRESENT)
       icf_print_header_list(p_info_cfm->header_list.hdr, p_info_cfm->header_list.count);

    if (p_info_cfm->bit_mask & ICF_INFO_CFM_TAG_LIST_PRESENT)
       icf_print_tag_list(p_info_cfm->tag_list.tag, p_info_cfm->tag_list.count);

    if (p_info_cfm->bit_mask & ICF_INFO_CFM_BODY_LIST_PRESENT)
       icf_print_body_list(p_info_cfm->body_list.body,p_info_cfm->body_list.count);
			
           return ICF_SUCCESS;
}


/****************************************************************************
 *
 * FUNCTION:        icf_print_error_details
 *
 * DESCRIPTION:     Prints the error detail which has occured
 *
 * Functions called: 
 *                     
 *
 ****************************************************************************/


void icf_print_error_details(int error_no)
{

	switch(error_no)
	{
		case 0:
			break;

		case ICF_ERROR_MEM_ALLOC_FAILURE:
	 ICF_PRINT_MESSAGE(("\n Error String = ICF_ERROR_MEM_ALLOC_FAILURE"));  
			break;
			
		case ICF_ERROR_MEM_FREE_FAILURE:
	 ICF_PRINT_MESSAGE(("\n Error String = ICF_ERROR_MEM_FREE_FAILURE"));  
			break;
			
		case ICF_ERROR_TMR_START_FAILURE:
	 ICF_PRINT_MESSAGE(("\n Error String = ICF_ERROR_TMR_START_FAILURE"));  
			break;
			
		case ICF_ERROR_TMR_STOP_FAILURE: 
	ICF_PRINT_MESSAGE(("\n Error String = ICF_ERROR_TMR_STOP_FAILURE"));  
			break;
			
		case ICF_ERROR_INVALID_API_ID:
	 ICF_PRINT_MESSAGE(("\n Error String = ICF_ERROR_INVALID_API_ID ")); 
			break;
			
		case ICF_ERROR_CALL_FAIL_NO_SDP:
	 ICF_PRINT_MESSAGE(("\n Error String = ICF_ERROR_CALL_FAIL_NO_SDP ")); 
			break;
			
		case ICF_MSG_RECEIVE_ERROR: 
	ICF_PRINT_MESSAGE(("\n Error String = ICF_MSG_RECEIVE_ERROR "));      
			break;
			
		case ICF_INVALID_SRC_MODULE_ID: 
	ICF_PRINT_MESSAGE(("\n Error String = ICF_INVALID_SRC_MODULE_ID"));        
			break;
			
		case ICF_ERROR_MEM_INIT: 
	ICF_PRINT_MESSAGE(("\n Error String = ICF_ERROR_MEM_INIT "));        
			break;
			
		case ICF_ERROR_MEM_DEINIT:
	ICF_PRINT_MESSAGE(("\n Error String = ICF_ERROR_MEM_DEINIT "));      
			 break;
			 
		case ICF_ERROR_TMR_INIT:
	ICF_PRINT_MESSAGE(("\n Error String = ICF_ERROR_TMR_INIT "));      
			 break;			
			 
		case  ICF_ERROR_UDP_SEND:
	ICF_PRINT_MESSAGE(("\n Error String = ICF_ERROR_UDP_SEND "));      
			 break;
			 
		case  ICF_ERROR_TCP_SEND: 
	ICF_PRINT_MESSAGE(("\n Error String = ICF_ERROR_TCP_SEND"));       
			 break;
			 
		case ICF_ERROR_SOCKET_CREATE:
	ICF_PRINT_MESSAGE(("\n Error String = ICF_ERROR_SOCKET_CREATE"));       
			 break;
			 
		case ICF_ERROR_SOCKET_BIND:
	ICF_PRINT_MESSAGE(("\n Error String = ICF_ERROR_SOCKET_BIND"));     
			 break;
			 
		case ICF_ERROR_SET_SOCKET:
	ICF_PRINT_MESSAGE(("\n Error String = ICF_ERROR_SET_SOCKET"));     
			 break;
			 
		case ICF_ERROR_UDP_RECV:
	 ICF_PRINT_MESSAGE(("\n Error String = ICF_ERROR_UDP_RECV "));    
    			 break;
    			 
		case ICF_CALL_CTX_BLK_EXHAUSTED:
	ICF_PRINT_MESSAGE(("\n Error String = ICF_CALL_CTX_BLK_EXHAUSTED "));
		 break;
		 
		case ICF_RGN_CTX_BLK_EXHAUSTED :
	ICF_PRINT_MESSAGE(("\n Error String = ICF_RGN_CTX_BLK_EXHAUSTED "));
			 break;
			 
		case ICF_NO_MODULE_DATA_AVAILABLE:
	ICF_PRINT_MESSAGE(("\n Error String = ICF_NO_MODULE_DATA_AVAILABLE "));
			 break;
			 
		case ICF_NO_CALL_CTX_EXIST:
	ICF_PRINT_MESSAGE(("\n Error String = ICF_NO_CALL_CTX_EXIST "));
			 break;
			 
		case ICF_NO_RGN_CTX_EXIST:
	ICF_PRINT_MESSAGE(("\n Error String = ICF_NO_RGN_CTX_EXIST"));  
			 break;
			 
		case ICF_USER_NOT_ADDED:
	ICF_PRINT_MESSAGE(("\n Error String = ICF_USER_NOT_ADDED "));
			 break;
			 
		case ICF_DUPLICATE_USER:
	ICF_PRINT_MESSAGE(("\n Error String = ICF_DUPLICATE_USER"));   
			 break;
			 
		case ICF_LA_INIT_FAILURE:
	ICF_PRINT_MESSAGE(("\n Error String = ICF_LA_INIT_FAILURE "));    
			 break;
			 
		case ICF_MSG_PROC_FIELD_TYPE_MISMATCH:
	ICF_PRINT_MESSAGE(("\n Error String = ICF_MSG_PROC_FIELD_TYPE_MISMATCH "));      
			 break;
			 
		case ICF_MSG_PROC_OPR_MISMATCH:
	ICF_PRINT_MESSAGE(("\n Error String = ICF_MSG_PROC_OPR_MISMATCH "));   
			 break;
			 
		case ICF_ERROR_INVALID_TRNSPT_ADDR:
	ICF_PRINT_MESSAGE(("\n Error String = ICF_ERROR_INVALID_TRNSPT_ADDR"));    
			 break;
			 
		case ICF_POOL_EXHAUSTED:
	ICF_PRINT_MESSAGE(("\n Error String = ICF_POOL_EXHAUSTED "));  
			 break;
			 
		case ICF_CAUSE_MAX_CALLS_TO_LOG_OUT_OF_RANGE:
	ICF_PRINT_MESSAGE(("\n Error String = ICF_CAUSE_MAX_CALLS_TO_LOG_OUT_OF_RANGE "));    
			 break;
			 
		case ICF_CAUSE_LINE_NOT_CONFIGURED:
	ICF_PRINT_MESSAGE(("\n Error String = ICF_CAUSE_LINE_NOT_CONFIGURED "));      
			 break
			 ;
		case ICF_CAUSE_SELF_ADDR_DOMAIN_NAME_NOT_HANDLED:
	ICF_PRINT_MESSAGE(("\n Error String = ICF_CAUSE_SELF_ADDR_DOMAIN_NAME_NOT_HANDLED"));   
			 break;
		case ICF_CAUSE_INVALID_TIMER_VALUE:
	ICF_PRINT_MESSAGE(("\n Error String = ICF_CAUSE_INVALID_TIMER_VALUE "));    
			 break;
			 
		case ICF_CAUSE_ACTION_NOT_ALLWD_DURING_REG_ONG:
	ICF_PRINT_MESSAGE(("\n Error String = ICF_CAUSE_ACTION_NOT_ALLWD_DURING_REG_ONG"));  
			break;
			
		case ICF_CAUSE_MAX_NUM_APPLICATION_EXCEEDED:
	ICF_PRINT_MESSAGE(("\n Error String = ICF_CAUSE_MAX_NUM_APPLICATION_EXCEEDED "));   
			break;
			
		case ICF_CAUSE_ICF_NOT_INITIALISED:
	ICF_PRINT_MESSAGE(("\n Error String = ICF_CAUSE_ICF_NOT_INITIALISED"));    
			break;
			
		case ICF_CAUSE_INVALID_ACCESS_TYPE:
	ICF_PRINT_MESSAGE(("\n Error String = ICF_CAUSE_INVALID_ACCESS_TYPE"));    
			break;
						
		case ICF_CAUSE_INVALID_ACCESS_INFO:
	ICF_PRINT_MESSAGE(("\n Error String = ICF_CAUSE_INVALID_ACCESS_INFO ")); 
			break;
			
		case ICF_CAUSE_INVALID_VERSION_ID:
	ICF_PRINT_MESSAGE(("\n Error String = ICF_CAUSE_INVALID_VERSION_ID  "));      
			break;
			
		case ICF_CAUSE_INVALID_CALL_ID:
	ICF_PRINT_MESSAGE(("\n Error String = ICF_CAUSE_INVALID_CALL_ID  "));                   
			break;
			
		case ICF_CAUSE_INVALID_SOURCE_ID:
	ICF_PRINT_MESSAGE(("\n Error String = ICF_CAUSE_INVALID_SOURCE_ID  "));                       
			break;
			
		case ICF_CAUSE_INVALID_DESTINATION_ID:
	ICF_PRINT_MESSAGE(("\n Error String = ICF_CAUSE_INVALID_DESTINATION_ID "));                
			break;
			
		case ICF_CAUSE_DUPLICATE_REQUEST:
	ICF_PRINT_MESSAGE(("\n Error String = ICF_CAUSE_DUPLICATE_REQUEST "));    
			break;
		case ICF_CAUSE_MAX_APP_EXCEEDED:
	ICF_PRINT_MESSAGE(("\n Error String = ICF_CAUSE_MAX_APP_EXCEEDED"));                   
			break;
		case ICF_CAUSE_MAX_NETWORK_EXCEEDED:
	ICF_PRINT_MESSAGE(("\n Error String = ICF_CAUSE_MAX_NETWORK_EXCEEDED  "));      
			break;
			
		case ICF_CAUSE_ONGOING_REGISTRATION:
	ICF_PRINT_MESSAGE(("\n Error String = ICF_CAUSE_ONGOING_REGISTRATION "));            
			break;
			
		case  ICF_CAUSE_NO_PUBLIC_URI:
	ICF_PRINT_MESSAGE(("\n Error String = ICF_CAUSE_NO_PUBLIC_URI"));     
			break;
			
		case  ICF_CAUSE_NO_PRIVATE_URI:
	ICF_PRINT_MESSAGE(("\n Error String = ICF_CAUSE_NO_PRIVATE_URI "));                  
			break;
			
		case ICF_CAUSE_NO_AUTH_PARAMS:
	ICF_PRINT_MESSAGE(("\n Error String = ICF_CAUSE_NO_AUTH_PARAMS"));     
			break;
			
		case ICF_CAUSE_NO_HOME_NETWORK:
	ICF_PRINT_MESSAGE(("\n Error String = ICF_CAUSE_NO_HOME_NETWORK"));                 
			break;
			
		case ICF_CAUSE_TRANSPORT_FAILURE:
	ICF_PRINT_MESSAGE(("\n Error String = ICF_CAUSE_TRANSPORT_FAILURE"));     
			break;
			
		case ICF_CAUSE_SECUITY_ASSOCIATION_FAILURE:
	ICF_PRINT_MESSAGE(("\n Error String = ICF_CAUSE_SECUITY_ASSOCIATION_FAILURE"));             
			break;
			
		case  ICF_CAUSE_AUTH_FAILED:
	ICF_PRINT_MESSAGE(("\n Error String = ICF_CAUSE_AUTH_FAILED"));   
			break;
			
		case ICF_CAUSE_REGISTERATION_RETRY_OVER:
	ICF_PRINT_MESSAGE(("\n Error String = ICF_CAUSE_REGISTERATION_RETRY_OVER "));                 
			break;
			
		case ICF_CAUSE_NETWORK_DEREGISTERED:
	ICF_PRINT_MESSAGE(("\n Error String = ICF_CAUSE_NETWORK_DEREGISTERED "));   
			break;
			
		case ICF_CAUSE_PUBLIC_URI_BARRED:
	ICF_PRINT_MESSAGE(("\n Error String = ICF_CAUSE_PUBLIC_URI_BARRED "));   
			break;
			
		case ICF_CAUSE_HOME_NETWORK_NOT_AUTH:
	ICF_PRINT_MESSAGE(("\n Error String = ICF_CAUSE_HOME_NETWORK_NOT_AUTH"));       
			break;
			
		case ICF_CAUSE_INVALID_CONTEXT:
	ICF_PRINT_MESSAGE(("\n Error String = ICF_CAUSE_INVALID_CONTEXT "));      
			break;
			
		case ICF_CAUSE_CONTEXT_BUSY:
	ICF_PRINT_MESSAGE(("\n Error String = ICF_CAUSE_CONTEXT_BUSY"));
			break;
			
		case ICF_CAUSE_INVALID_APP_ID:
	ICF_PRINT_MESSAGE(("\n Error String = ICF_CAUSE_INVALID_APP_ID  "));
		break;
		
		case ICF_CAUSE_INVALID_OPTION:
	ICF_PRINT_MESSAGE(("\n Error String = ICF_CAUSE_INVALID_OPTION "));
			break;
			
		case ICF_CAUSE_NETWORK_ERROR:
	ICF_PRINT_MESSAGE(("\n Error String = ICF_CAUSE_NETWORK_ERROR "));
			break;
			
		case ICF_CAUSE_TIMER_EXPIRY:
	ICF_PRINT_MESSAGE(("\n Error String = ICF_CAUSE_TIMER_EXPIRY "));
			break;
			
		case ICF_CAUSE_CALL_ALREADY_ACTIVE :
	ICF_PRINT_MESSAGE(("\n Error String = ICF_CAUSE_CALL_ALREADY_ACTIVE "));
			break;
			
		case ICF_CAUSE_ANOTHER_CALL_ACTIVE_ON_SAME_LINE:
	ICF_PRINT_MESSAGE(("\n Error String = ICF_CAUSE_ANOTHER_CALL_ACTIVE_ON_SAME_LINE "));
		   	break;
		   	
		case ICF_CAUSE_AUDIO_CODEC_NOT_AVAILABLE:
	ICF_PRINT_MESSAGE(("\n Error String = ICF_CAUSE_AUDIO_CODEC_NOT_AVAILABLE "));
		 	break;
		 	
		case ICF_CAUSE_VIDEO_CODEC_NOT_AVAILABLE:
	ICF_PRINT_MESSAGE(("\n Error String = ICF_CAUSE_VIDEO_CODEC_NOT_AVAILABLE "));
			break;
			
		case ICF_CAUSE_UNABLE_TO_RELEASE_RESOURCES:
	ICF_PRINT_MESSAGE(("\n Error String = ICF_CAUSE_UNABLE_TO_RELEASE_RESOURCES "));
			break;
			
		case ICF_CAUSE_MODE_NOT_COMPATIBLE_TO_SDP:
	ICF_PRINT_MESSAGE(("\n Error String = ICF_CAUSE_MODE_NOT_COMPATIBLE_TO_SDP "));
			break;
			
		case ICF_CAUSE_UNABLE_TO_CREATE_MEDIA:
	ICF_PRINT_MESSAGE(("\n Error String = ICF_CAUSE_UNABLE_TO_CREATE_MEDIA "));
			break;
			
		case ICF_CAUSE_INSUFFICIENT_PARAMETERS:
	ICF_PRINT_MESSAGE(("\n Error String = ICF_CAUSE_INSUFFICIENT_PARAMETERS "));
			break;
			
		case ICF_CAUSE_INSUFFICIENT_RESOURCES:
	ICF_PRINT_MESSAGE(("\n Error String = ICF_CAUSE_INSUFFICIENT_RESOURCES  "));
			 break;
			 
		case ICF_CAUSE_UNABLE_TO_DELETE_MEDIA_SESSION :
	ICF_PRINT_MESSAGE(("\n Error String = ICF_CAUSE_UNABLE_TO_DELETE_MEDIA_SESSION "));
			 break;
			 
		case ICF_CAUSE_SELF_IP_ADDR_BIND_FAILURE:
	ICF_PRINT_MESSAGE(("\n Error String = ICF_CAUSE_SELF_IP_ADDR_BIND_FAILURE "));
			 break;
			 
		case ICF_CAUSE_INVALID_STATS_ID:
	ICF_PRINT_MESSAGE(("\n Error String = ICF_CAUSE_INVALID_STATS_ID "));
 			 break;
 			 
		case ICF_CAUSE_IP_PORT_OUT_OF_RANGE:
	ICF_PRINT_MESSAGE(("\n Error String = ICF_CAUSE_IP_PORT_OUT_OF_RANGE "));
			 break;
			 
		case ICF_CAUSE_MEDIA_FAILURE:
	ICF_PRINT_MESSAGE(("\n Error String = ICF_CAUSE_MEDIA_FAILURE "));
			 break;
			 
		case ICF_CAUSE_MAX_LINES_OUT_OF_RANGE:
	ICF_PRINT_MESSAGE(("\n Error String = ICF_CAUSE_MAX_LINES_OUT_OF_RANGE "));
			 break;
			 
		case ICF_CAUSE_INVALID_LINE_ID :
	ICF_PRINT_MESSAGE(("\n Error String = ICF_CAUSE_INVALID_LINE_ID "));
			 break;
			 
		case ICF_CAUSE_MAX_REGISTRATION_CALLS_EXCEEDED:
	ICF_PRINT_MESSAGE(("\n Error String = ICF_CAUSE_MAX_REGISTRATION_CALLS_EXCEEDED "));
			 break;
			 
		case  ICF_CAUSE_MAX_USERS_PER_LINE_EXCEEDED:
	ICF_PRINT_MESSAGE(("\n Error String = ICF_CAUSE_MAX_USERS_PER_LINE_EXCEEDED "));
			 break;
			 
		case ICF_CAUSE_INVALID_ADDRESS_RECEIVED:
	ICF_PRINT_MESSAGE(("\n Error String = ICF_CAUSE_INVALID_ADDRESS_RECEIVED "));
			 break;
			 
		case ICF_CAUSE_MAX_AUDIO_CALLS_OUT_OF_RANGE:
	ICF_PRINT_MESSAGE(("\n Error String = ICF_CAUSE_MAX_AUDIO_CALLS_OUT_OF_RANGE "));
			 break;
			 
		case ICF_CAUSE_MAX_REGISTRATION_CALLS_OUT_OF_RANGE:
	ICF_PRINT_MESSAGE(("\n Error String = ICF_CAUSE_MAX_REGISTRATION_CALLS_OUT_OF_RANGE "));
			 break;
			 
		case ICF_CAUSE_MAX_USERS_PER_LINE_OUT_OF_RANGE:
	ICF_PRINT_MESSAGE(("\n Error String = ICF_CAUSE_MAX_USERS_PER_LINE_OUT_OF_RANGE "));
			 break;
			 
		case ICF_ERROR_CAUSE_URI_NOT_DEREGISTERED :
	ICF_PRINT_MESSAGE(("\n Error String = ICF_ERROR_CAUSE_URI_NOT_DEREGISTERED "));
			 break;
			 
		case ICF_CAUSE_ACTIVATE_DENIED:
	ICF_PRINT_MESSAGE(("\n Error String = ICF_CAUSE_ACTIVATE_DENIED "));
			 break;
			 
		case ICF_CAUSE_ACTIVATION_FAILURE:
	ICF_PRINT_MESSAGE(("\n Error String = ICF_CAUSE_ACTIVATION_FAILURE "));
			 break;
			 
		case ICF_CAUSE_DEACTIVATION_FAILURE:
	ICF_PRINT_MESSAGE(("\n Error String = ICF_CAUSE_DEACTIVATION_FAILURE"));
			 break;
			 
		case ICF_CAUSE_ILLEGAL_REQUEST:
	ICF_PRINT_MESSAGE(("\n Error String = ICF_CAUSE_ILLEGAL_REQUEST "));
			 break;
			 
		case ICF_CAUSE_INVALID_CALL_TYPE:
	ICF_PRINT_MESSAGE(("\n Error String = ICF_CAUSE_INVALID_CALL_TYPE "));
			 break;
			 
		case ICF_CAUSE_INVALID_ROUTING_INFO:
	ICF_PRINT_MESSAGE(("\n Error String = ICF_CAUSE_INVALID_ROUTING_INFO "));
			 break;
			 
		case ICF_CAUSE_FILTER_PARSE_FAILED:
	ICF_PRINT_MESSAGE(("\n Error String = ICF_CAUSE_FILTER_PARSE_FAILED "));
			 break;
			 
		case ICF_CAUSE_INTERNAL_FAILURE:
	ICF_PRINT_MESSAGE(("\n Error String = ICF_CAUSE_INTERNAL_FAILURE  "));
			 break;
			 
		case ICF_CAUSE_NW_CURRENTLY_NOT_AVAILABLE:
	ICF_PRINT_MESSAGE(("\n Error String = ICF_CAUSE_NW_CURRENTLY_NOT_AVAILABLE  "));
			 break;
			 
		case ICF_CAUSE_INVALID_REG_REQUEST:
	ICF_PRINT_MESSAGE(("\n Error String = ICF_CAUSE_INVALID_REG_REQUEST "));
			 break;	
			 	
		case ICF_CAUSE_SCHEME_NOT_SUPPORTED:
	ICF_PRINT_MESSAGE(("\n Error String = ICF_CAUSE_SCHEME_NOT_SUPPORTED "));
			 break;
		case ICF_CAUSE_ALGO_NOT_SUPPORTED:
	ICF_PRINT_MESSAGE(("\n Error String = ICF_CAUSE_ALGO_NOT_SUPPORTED "));
			 break;
		case ICF_CAUSE_PROPER_NONCE_NOT_RECD:
	ICF_PRINT_MESSAGE(("\n Error String = ICF_CAUSE_PROPER_NONCE_NOT_RECD "));
			 break;
		case ICF_CAUSE_SQN_OUT_OF_RANGE:
	ICF_PRINT_MESSAGE(("\n Error String = ICF_CAUSE_SQN_OUT_OF_RANGE "));
			 break;
			 
		case ICF_CAUSE_LINE_ID_MISMATCH:
	ICF_PRINT_MESSAGE(("\n Error String = ICF_CAUSE_LINE_ID_MISMATCH "));
			 break;
			 
		case ICF_CAUSE_NW_DEACTIVATED:
	ICF_PRINT_MESSAGE(("\n Error String = ICF_CAUSE_NW_DEACTIVATED "));      
			 break;
			 
		case ICF_CAUSE_INCONSISTENT_TRANSPORT_MODE:
	ICF_PRINT_MESSAGE(("\n Error String = ICF_CAUSE_INCONSISTENT_TRANSPORT_MODE"));       
			 break;
			 
		case ICF_CAUSE_NW_PROBATION:
	ICF_PRINT_MESSAGE(("\n Error String = ICF_CAUSE_NW_PROBATION "));
			 break;
			 
		case ICF_CAUSE_NW_EXPIRATION:
	ICF_PRINT_MESSAGE(("\n Error String = ICF_CAUSE_NW_EXPIRATION"));      
			 break;
			 
		case ICF_CAUSE_NW_REJECTED:
	ICF_PRINT_MESSAGE(("\n Error String = ICF_CAUSE_NW_REJECTED"));   
			 break;
			 
		case ICF_CAUSE_NW_UNREGISTERED:
	ICF_PRINT_MESSAGE(("\n Error String = ICF_CAUSE_NW_UNREGISTERED "));
			 break;
			 
		case ICF_CAUSE_EMPTY_SUPP_HDR_LIST:
	ICF_PRINT_MESSAGE(("\n Error String = ICF_CAUSE_EMPTY_SUPP_HDR_LIST "));
			 break;
			 
		case ICF_CAUSE_REJECT_MEDIA_CHANGE_REQ:
	ICF_PRINT_MESSAGE(("\n Error String = ICF_CAUSE_REJECT_MEDIA_CHANGE_REQ "));
			 break;
			 
/*		case ICF_CAUSE_SA_CREATION_FAILURE:
	ICF_PRINT_MESSAGE(("\n Error String = ICF_CAUSE_SA_CREATION_FAILURE "));
			 break;
			 
		case ICF_CAUSE_MULTIPLE_SA_EXIST:
	ICF_PRINT_MESSAGE(("\n Error String = ICF_CAUSE_MULTIPLE_SA_EXIST "));
			 break;
			 
		case ICF_CAUSE_IPSEC_FAILURE:
	ICF_PRINT_MESSAGE(("\n Error String = ICF_CAUSE_IPSEC_FAILURE "));
			 break;
*/			 
		case ICF_CAUSE_ADDR_FOR_SERV_NOT_CONFIGURED:
	ICF_PRINT_MESSAGE(("\n Error String = ICF_CAUSE_ADDR_FOR_SERV_NOT_CONFIGURED "));
			 break;
			 
		case ICF_CAUSE_SERVICE_NOT_SUBSCRIBED:
	ICF_PRINT_MESSAGE(("\n Error String = ICF_CAUSE_SERVICE_NOT_SUBSCRIBED "));
			 break;
			 
		case ICF_CAUSE_SCALE_PARAMS_NOT_CONFIGURED:
	ICF_PRINT_MESSAGE(("\n Error String = ICF_CAUSE_SCALE_PARAMS_NOT_CONFIGURED "));
			 break;
			 
		case ICF_CAUSE_SCALE_PARAMS_ALREADY_CONFIGURED:
	ICF_PRINT_MESSAGE(("\n Error String = ICF_CAUSE_SCALE_PARAMS_ALREADY_CONFIGURED "));
			 break;
			 
		case ICF_CAUSE_CALL_MODIFY_REQ_PENDING:
	ICF_PRINT_MESSAGE(("\n Error String = ICF_CAUSE_CALL_MODIFY_REQ_PENDING" ));
			 break;
			 
		case ICF_CAUSE_INVALID_CALL_MODIFY_REQUEST:
	ICF_PRINT_MESSAGE(("\n Error String = ICF_CAUSE_INVALID_CALL_MODIFY_REQUEST "));
			 break;
			 
		case ICF_CAUSE_REMOTE_RELEASED:
	ICF_PRINT_MESSAGE(("\n Error String = ICF_CAUSE_REMOTE_RELEASED "));
			 break;
			 
		case ICF_CAUSE_IMPLICITLY_REGISTERED:
	ICF_PRINT_MESSAGE(("\n Error String = ICF_CAUSE_IMPLICITLY_REGISTERED "));
			 break;
			 
		case ICF_CAUSE_MANDATORY_HDR_NOT_RCVD:
	ICF_PRINT_MESSAGE(("\n Error String = ICF_CAUSE_MANDATORY_HDR_NOT_RCVD "));
			 break;
			 
		case ICF_CAUSE_STREAM_ADDITION_NOT_ALLOWED:
	ICF_PRINT_MESSAGE(("\n Error String = ICF_CAUSE_STREAM_ADDITION_NOT_ALLOWED "));
		 	 break;
		 
		case ICF_CAUSE_FAX_CALL_NOT_ALLOWED:
	ICF_PRINT_MESSAGE(("\n Error String = ICF_CAUSE_FAX_CALL_NOT_ALLOWED "));
			 break;
			 
		case ICF_CAUSE_INVALID_FAX_REQUEST:
	ICF_PRINT_MESSAGE(("\n Error String = ICF_CAUSE_INVALID_FAX_REQUEST "));
			 break;
			 
		case ICF_CAUSE_MIME_BODY_DECODE_FAILURE:
	ICF_PRINT_MESSAGE(("\n Error String = ICF_CAUSE_MIME_BODY_DECODE_FAILURE "));
  			 break;
  			 
		case ICF_CAUSE_CALL_SETUP_IN_PROGRESS:
	ICF_PRINT_MESSAGE(("\n Error String = ICF_CAUSE_CALL_SETUP_IN_PROGRESS "));
			 break;
			 
		case ICF_CAUSE_FAX_PROFILE_NOT_CONFIGURED:
	ICF_PRINT_MESSAGE(("\n Error String = ICF_CAUSE_FAX_PROFILE_NOT_CONFIGURED "));
 			 break;
 			 
		case ICF_CAUSE_FAX_REQ_PENDING:
	ICF_PRINT_MESSAGE(("\n Error String = ICF_CAUSE_FAX_REQ_PENDING "));
			 break;
			 
		case ICF_CAUSE_FAX_CALL_ONGOING:
	ICF_PRINT_MESSAGE(("\n Error String = ICF_CAUSE_FAX_CALL_ONGOING "));
			 break;
			 
		case ICF_CAUSE_SERVICE_ONGOING:
	ICF_PRINT_MESSAGE(("\n Error String = ICF_CAUSE_SERVICE_ONGOING "));
			 break;
			 
		case ICF_CAUSE_REALM_MISMATCH:
	ICF_PRINT_MESSAGE(("\n Error String = ICF_CAUSE_REALM_MISMATCH "));
			 break;
			 
		case ICF_CAUSE_BASIC_AUTH_SCHEME_NOT_SUPPORTED:
	ICF_PRINT_MESSAGE(("\n Error String = ICF_CAUSE_BASIC_AUTH_SCHEME_NOT_SUPPORTED "));
			 break;
			 
		case ICF_CAUSE_LICENSE_EXPIRED:
	ICF_PRINT_MESSAGE(("\n Error String = ICF_CAUSE_LICENSE_EXPIRED "));
			 break;
			 
		case ICF_CAUSE_USER_NOT_LICENSED:
	ICF_PRINT_MESSAGE(("\n Error String = ICF_CAUSE_USER_NOT_LICENSED "));
			 break;
			 
		case ICF_CAUSE_INFO_CFM_PENDING:
	ICF_PRINT_MESSAGE(("\n Error String = ICF_CAUSE_INFO_CFM_PENDING "));
			 break;
			 
		case ICF_CAUSE_MAX_SERVER_LIMIT_REACHED:
	ICF_PRINT_MESSAGE(("\n Error String = ICF_CAUSE_MAX_SERVER_LIMIT_REACHED "));
			 break;
			 
		case ICF_CAUSE_SELF_ADDR_MODIFY_NOT_SUPP:
	ICF_PRINT_MESSAGE(("\n Error String = ICF_CAUSE_SELF_ADDR_MODIFY_NOT_SUPP"));
			 break;
			 
		case ICF_CAUSE_UNSUPPORTED_METHOD_IN_LIST:
	ICF_PRINT_MESSAGE(("\n Error String = ICF_CAUSE_UNSUPPORTED_METHOD_IN_LIST"));
			 break;
			 
		case ICF_ERROR_END_CFG:
	ICF_PRINT_MESSAGE(("\n Error String = ICF_ERROR_END_CFG"));
			 break;	

		case ICF_CAUSE_CLOSE_CONN_FAILURE:
		ICF_PRINT_MESSAGE(("\n Error String = ICF_CAUSE_CLOSE_CONN_FAILURE"));
			break;
			
		case ICF_CAUSE_NO_RESP_FROM_SERVER:
		ICF_PRINT_MESSAGE(("\n Error String = ICF_CAUSE_NO_RESP_FROM_SERVER"));
			break;

		case ICF_CAUSE_MAX_APPLICATION_EXCEEDED:
		ICF_PRINT_MESSAGE(("\n Error String = ICF_CAUSE_MAX_APPLICATION_EXCEEDED"));
			break;
			
		case ICF_CAUSE_MAX_CONNECTION_EXCEEDED:
		ICF_PRINT_MESSAGE(("\n Error String = ICF_CAUSE_MAX_CONNECTION_EXCEEDED "));
			break;
			
		case ICF_CAUSE_REQ_PENDING_ON_CURRENT_AUTH_DATA:
		ICF_PRINT_MESSAGE(("\n Error String = ICF_CAUSE_REQ_PENDING_ON_CURRENT_AUTH_DATA"));
			break;
			
		case ICF_CAUSE_SERVER_INFO_MISSING:
		ICF_PRINT_MESSAGE(("\n Error String = ICF_CAUSE_SERVER_INFO_MISSING"));
			break;
			
		case ICF_CAUSE_SERVER_INFO_REMOVED:
		ICF_PRINT_MESSAGE(("\n Error String = ICF_CAUSE_SERVER_INFO_REMOVED"));
			break;
			
		case ICF_CAUSE_CONNECTION_FAILURE:
		ICF_PRINT_MESSAGE(("\n Error String = ICF_CAUSE_CONNECTION_FAILURE"));
			break;
			
		case ICF_CAUSE_CONNECTION_ALREADY_EXISTS:
		ICF_PRINT_MESSAGE(("\n Error String = ICF_CAUSE_CONNECTION_ALREADY_EXISTS"));
			break;
			
		case ICF_CAUSE_FROM_PORT:
		ICF_PRINT_MESSAGE(("\n Error String = ICF_CAUSE_FROM_PORT"));
			break;
			
		case ICF_CAUSE_INVALID_CONN_CLOSURE_TYPE:
		ICF_PRINT_MESSAGE(("\n Error String = ICF_CAUSE_INVALID_CONN_CLOSURE_TYPE"));
			break;
			
		case ICF_CAUSE_PORT_SEND_HTTP_REQ:
		ICF_PRINT_MESSAGE(("\n Error String = ICF_CAUSE_PORT_SEND_HTTP_REQ"));
			break;
			
		case ICF_CAUSE_PORT_MAKE_HTTP_REQ:
		ICF_PRINT_MESSAGE(("\n Error String = ICF_CAUSE_PORT_MAKE_HTTP_REQ"));
			break;
			
		case ICF_CAUSE_SYNCML_HEADERS_NOT_PRESENT:ICF_PRINT_MESSAGE(("\n Error String = ICF_CAUSE_SYNCML_HEADERS_NOT_PRESENT"));
			break;
			
		case ICF_CAUSE_INVALID_CONNECTION_ID:
		ICF_PRINT_MESSAGE(("\n Error String = ICF_CAUSE_INVALID_CONNECTION_ID "));
			break;
			
		case ICF_CAUSE_MANDATORY_HEADERS_NOT_PRESENT:
		ICF_PRINT_MESSAGE(("\n Error String = ICF_CAUSE_MANDATORY_HEADERS_NOT_PRESENT"));
			break;
			
		case ICF_CAUSE_CONNECTION_REATTEMPT_FAILURE:
		ICF_PRINT_MESSAGE(("\n Error String = ICF_CAUSE_CONNECTION_REATTEMPT_FAILURE"));
			break;
			
		case ICF_CAUSE_WWW_AUTHENTICATE_NOT_PRESENT_IN_401:
		ICF_PRINT_MESSAGE(("\n Error String = ICF_CAUSE_WWW_AUTHENTICATE_NOT_PRESENT_IN_401"));
			break;
			
		case ICF_CAUSE_INVALID_USERNAME_PASSWD:
		ICF_PRINT_MESSAGE(("\n Error String = ICF_CAUSE_INVALID_USERNAME_PASSWD"));
			break;
			
		case ICF_CAUSE_AUTH_DATA_NOT_PRESENT:
		ICF_PRINT_MESSAGE(("\n Error String = ICF_CAUSE_AUTH_DATA_NOT_PRESENT"));
			break;
			
		case ICF_CAUSE_UNKNOWN_AUTH_SCHEME_IN_401:
		ICF_PRINT_MESSAGE(("\n Error String = ICF_CAUSE_UNKNOWN_AUTH_SCHEME_IN_401"));
			break;
			
		case ICF_CAUSE_REALM_NOT_PRESENT_IN_401:
		ICF_PRINT_MESSAGE(("\n Error String = ICF_CAUSE_REALM_NOT_PRESENT_IN_401"));
			break;
			
		case ICF_CAUSE_NONCE_NOT_PRESENT_IN_401:
		ICF_PRINT_MESSAGE(("\n Error String = ICF_CAUSE_NONCE_NOT_PRESENT_IN_401"));
			break;
			
		case ICF_CAUSE_APPLICATION_REMOVED:
		ICF_PRINT_MESSAGE(("\n Error String = ICF_CAUSE_APPLICATION_REMOVED "));
			break;
			
		case ICF_CAUSE_NETWORK_DEACTIVATED:
		ICF_PRINT_MESSAGE(("\n Error String = ICF_CAUSE_NETWORK_DEACTIVATED"));
			break;
			
		case ICF_CAUSE_APPLICATION_ADDED_AGAIN:
		ICF_PRINT_MESSAGE(("\n Error String = ICF_CAUSE_APPLICATION_ADDED_AGAIN"));
			break;
			
		case ICF_CAUSE_CLOSE_CONN_REQ:
		ICF_PRINT_MESSAGE(("\n Error String = ICF_CAUSE_CLOSE_CONN_REQ"));
			break;
			
		case ICF_CAUSE_MAKE_AUTHORISATION_FAILED:
		ICF_PRINT_MESSAGE(("\n Error String = ICF_CAUSE_MAKE_AUTHORISATION_FAILED"));
			break;
			
		case ICF_CAUSE_INVALID_CONNECTION_USAGE:
		ICF_PRINT_MESSAGE(("\n Error String = ICF_CAUSE_INVALID_CONNECTION_USAGE"));
			break;
			
		case ICF_CAUSE_NW_RESP_FAILED:
		ICF_PRINT_MESSAGE(("\n Error String = ICF_CAUSE_NW_RESP_FAILED"));
			break;
			
		case ICF_CAUSE_RING_TONE_NOT_SUPPORTED:
		ICF_PRINT_MESSAGE(("\n Error String = ICF_CAUSE_RING_TONE_NOT_SUPPORTED"));
			break;
			
		case ICF_CAUSE_INTERNAL_ERROR:
		ICF_PRINT_MESSAGE(("\n Error String = ICF_CAUSE_INTERNAL_ERROR"));
			break;
			
		case ICF_CAUSE_CONNECTION_FOR_CALLID_NOT_FOUND:
		ICF_PRINT_MESSAGE(("\n Error String = ICF_CAUSE_CONNECTION_FOR_CALLID_NOT_FOUND"));
			break;
			
		case ICF_CAUSE_INVALID_VALUE:
		ICF_PRINT_MESSAGE(("\n Error String = ICF_CAUSE_INVALID_VALUE"));
			break;
			
		case ICF_CAUSE_NOT_SUPPORTED:
		ICF_PRINT_MESSAGE(("\n Error String = ICF_CAUSE_NOT_SUPPORTED"));
			break;
			
		case ICF_CAUSE_FILE_PATH_NOT_FOUND:
		ICF_PRINT_MESSAGE(("\n Error String = ICF_CAUSE_FILE_PATH_NOT_FOUND"));
			break;
			
		case ICF_CAUSE_TONE_NOT_SUPPORTED:
		ICF_PRINT_MESSAGE(("\n Error String = ICF_CAUSE_TONE_NOT_SUPPORTED"));
			break;
			
		case ICF_CAUSE_INVALID_ACTION:
		ICF_PRINT_MESSAGE(("\n Error String = ICF_CAUSE_INVALID_ACTION"));
			break;
			
		case ICF_CAUSE_INVALID_PLAY_OUT_VAL:
		ICF_PRINT_MESSAGE(("\n Error String = ICF_CAUSE_INVALID_PLAY_OUT_VAL"));
			break;
			
		case ICF_CAUSE_INVALID_FILE_TYPE:
		ICF_PRINT_MESSAGE(("\n Error String = ICF_CAUSE_INVALID_FILE_TYPE"));
			break;
			
		case ICF_CAUSE_INVALID_DIGIT:
		ICF_PRINT_MESSAGE(("\n Error String = ICF_CAUSE_INVALID_DIGIT"));
			break;
			
		case ICF_CAUSE_OUT_OF_RANGE:
		ICF_PRINT_MESSAGE(("\n Error String = ICF_CAUSE_OUT_OF_RANGE"));
			break;
			
		case ICF_CAUSE_INVALID_JB_TYPE:
		ICF_PRINT_MESSAGE(("\n Error String = ICF_CAUSE_INVALID_JB_TYPE"));
			break;
			
		case ICF_CAUSE_INVALID_JB_SCALING_FACT:
		ICF_PRINT_MESSAGE(("\n Error String = ICF_CAUSE_INVALID_JB_SCALING_FACT"));
			break;
			
		case ICF_CAUSE_INVALID_JB_INIT_SIZE:
		ICF_PRINT_MESSAGE(("\n Error String = ICF_CAUSE_INVALID_JB_INIT_SIZE"));
			break;
			
		case ICF_CAUSE_INVALID_JB_MAX_SIZE:
		ICF_PRINT_MESSAGE(("\n Error String = ICF_CAUSE_INVALID_JB_MAX_SIZE"));
			break;
			
		case ICF_CAUSE_INVALID_JB_MIN_SIZE:
		ICF_PRINT_MESSAGE(("\n Error String = ICF_CAUSE_INVALID_JB_MIN_SIZE"));
			break;
			
		case ICF_ERROR_UATK_FAILURE:
		ICF_PRINT_MESSAGE(("\n Error String = ICF_ERROR_UATK_FAILURE"));
			break;
			
		case ICF_CONNECT_RESP_PENDING:
		ICF_PRINT_MESSAGE(("\n Error String = ICF_CONNECT_RESP_PENDING"));
			break;
			
		case ICF_ERROR_NW_INTERFACE:
		ICF_PRINT_MESSAGE(("\n Error String = ICF_ERROR_NW_INTERFACE"));
			break;
			
		case ICF_ERROR_RGM_INIT_CMPLT:
		ICF_PRINT_MESSAGE(("\n Error String = ICF_ERROR_RGM_INIT_CMPLT"));
			break;
			
		case ICF_ERROR_DBM_PORT_DATA:
		ICF_PRINT_MESSAGE(("\n Error String = ICF_ERROR_DBM_PORT_DATA"));
			break;
			
		case ICF_ERROR_ES_INIT_CMPLT:
		ICF_PRINT_MESSAGE(("\n Error String = ICF_ERROR_ES_INIT_CMPLT"));
			break;
			
		case ICF_ERROR_INIT_CMPLT:
		ICF_PRINT_MESSAGE(("\n Error String = ICF_ERROR_INIT_CMPLT"));
			break;
			
		case ICF_MSG_PROC_INIT_NOT_COMPLETE:
		ICF_PRINT_MESSAGE(("\n Error String = ICF_MSG_PROC_INIT_NOT_COMPLETE"));
			break;
			
		case ICF_INVALID_API_ID_RECD:
		ICF_PRINT_MESSAGE(("\n Error String = ICF_INVALID_API_ID_RECD"));
			break;
			
		case ICF_INVALID_MSG_ID_RECD:
		ICF_PRINT_MESSAGE(("\n Error String = ICF_INVALID_MSG_ID_RECD"));
			break;
			
		case ICF_MODIFY_USER_FAILED:
		ICF_PRINT_MESSAGE(("\n Error String = ICF_MODIFY_USER_FAILED"));
			break;
			
		case ICF_PROCESS_MODIFY_USER_BLK_FAILED:
		ICF_PRINT_MESSAGE(("\n Error String = ICF_PROCESS_MODIFY_USER_BLK_FAILED"));
			break;
			
		case ICF_MODIFY_REGISTRAR_FAILED:
		ICF_PRINT_MESSAGE(("\n Error String = ICF_MODIFY_REGISTRAR_FAILED"));
			break;
			
		case ICF_PROCESS_MODIFY_REG_BLK_FAILED:
		ICF_PRINT_MESSAGE(("\n Error String = ICF_PROCESS_MODIFY_REG_BLK_FAILED"));
			break;
			
		case ICF_MODIFY_DURATION_FAILED:
		ICF_PRINT_MESSAGE(("\n Error String = ICF_MODIFY_DURATION_FAILED"));
			break;
			
		case ICF_PROCESS_MODIFY_DUR_BLK_FAILED:
		ICF_PRINT_MESSAGE(("\n Error String = ICF_PROCESS_MODIFY_DUR_BLK_FAILED"));
			break;
			
		case ICF_AUTHENTICATION_INFO_ABSENT:
		ICF_PRINT_MESSAGE(("\n Error String = ICF_AUTHENTICATION_INFO_ABSENT"));
			break;
			
		case ICF_INVALID_NETWORK_RESPONSE:
		ICF_PRINT_MESSAGE(("\n Error String = ICF_INVALID_NETWORK_RESPONSE"));
			break;
			
		case ICF_UNABLE_TO_SEND_REG_REQ:
		ICF_PRINT_MESSAGE(("\n Error String = ICF_UNABLE_TO_SEND_REG_REQ"));
			break;
			
		case ICF_UNABLE_TO_SEND_DEREG_REQ:
		ICF_PRINT_MESSAGE(("\n Error String = ICF_UNABLE_TO_SEND_DEREG_REQ"));
			break;
			
		case ICF_INIT_COMPLETE_FAILED:
		ICF_PRINT_MESSAGE(("\n Error String = ICF_INIT_COMPLETE_FAILED"));
			break;
			
		case ICF_NO_SRVC_CTX_EXIST:
		ICF_PRINT_MESSAGE(("\n Error String = ICF_NO_SRVC_CTX_EXIST"));
			break;
			
		case ICF_SRVC_CTX_BLK_EXHAUSTED:
		ICF_PRINT_MESSAGE(("\n Error String = ICF_SRVC_CTX_BLK_EXHAUSTED"));
			break;
			
		case ICF_SRVC_CTX_LGC_BLK_EXHAUSTED:
		ICF_PRINT_MESSAGE(("\n Error String = ICF_SRVC_CTX_LGC_BLK_EXHAUSTED"));
			break;
			
		case ICF_ERROR_SERVICE_NOT_SUBSCRIBED:
		ICF_PRINT_MESSAGE(("\n Error String = ICF_ERROR_SERVICE_NOT_SUBSCRIBED"));
			break;
			
		case ICF_ERROR_SERVICE_NOT_ACTIVE:
		ICF_PRINT_MESSAGE(("\n Error String = ICF_ERROR_SERVICE_NOT_ACTIVE"));
			break;
			
		case ICF_ERROR_INVALID_SERVICE_INTERACTION:
		ICF_PRINT_MESSAGE(("\n Error String = ICF_ERROR_INVALID_SERVICE_INTERACTION"));
			break;
			
		case ICF_ERROR_SL_NOT_RUNNING:
		ICF_PRINT_MESSAGE(("\n Error String = ICF_ERROR_SL_NOT_RUNNING"));
			break;
			
		case ICF_ERROR_INV_PAYLOAD_RECVD:
		ICF_PRINT_MESSAGE(("\n Error String = ICF_ERROR_INV_PAYLOAD_RECVD"));
			break;
			
		case ICF_ERROR_INVALID_PARAM:
		ICF_PRINT_MESSAGE(("\n Error String = ICF_ERROR_INVALID_PARAM"));
			break;
			
		case ICF_ERROR_FIFO_CREATION:
		ICF_PRINT_MESSAGE(("\n Error String = ICF_ERROR_FIFO_CREATION"));
			break;
			
		case ICF_ERROR_FIFO_SEND:
		ICF_PRINT_MESSAGE(("\n Error String = ICF_ERROR_FIFO_SEND"));
			break;
			
		case ICF_ERROR_FIFO_RECV:
		ICF_PRINT_MESSAGE(("\n Error String = ICF_ERROR_FIFO_RECV"));
			break;
			
		case ICF_CAUSE_MM_AL_CONTEXT_NOT_FOUND:
		ICF_PRINT_MESSAGE(("\n Error String = ICF_CAUSE_MM_AL_CONTEXT_NOT_FOUND"));
			break;
			
		case ICF_CAUSE_MM_AL_NO_FREE_CONTEXT_FOUND:
		ICF_PRINT_MESSAGE(("\n Error String = ICF_CAUSE_MM_AL_NO_FREE_CONTEXT_FOUND"));
			break;
			
		case ICF_MM_AL_NO_VALID_CODEC_RETURNED:
		ICF_PRINT_MESSAGE(("\n Error String = ICF_MM_AL_NO_VALID_CODEC_RETURNED"));
			break;
			
		case ICF_MM_AL_UNKNOWN_REQUEST_FROM_MMI:
		ICF_PRINT_MESSAGE(("\n Error String = ICF_MM_AL_UNKNOWN_REQUEST_FROM_MMI"));
			break;
			
		case ICF_MM_AL_UNKNOWN_MESSAGE_FROM_RM:
		ICF_PRINT_MESSAGE(("\n Error String = ICF_MM_AL_UNKNOWN_MESSAGE_FROM_RM"));
			break;
			
		case ICF_TERMINAL_FAILURE_CALL_CLEAR:
		ICF_PRINT_MESSAGE(("\n Error String = ICF_TERMINAL_FAILURE_CALL_CLEAR"));
			break;
			
		case ICF_ERROR_NO_API_HANDLER:
		ICF_PRINT_MESSAGE(("\n Error String = ICF_ERROR_NO_API_HANDLER"));
			break;
			
		case ICF_ERROR_CALL_WAITING_FAILURE:
		ICF_PRINT_MESSAGE(("\n Error String = ICF_ERROR_CALL_WAITING_FAILURE"));
			break;
			
		case ICF_ERROR_LIC_INTEGRTY:
		ICF_PRINT_MESSAGE(("\n Error String = ICF_ERROR_LIC_INTEGRTY"));
			break;
			
		case ICF_RESPONSE_SEND_FAILURE:
		ICF_PRINT_MESSAGE(("\n Error String = ICF_RESPONSE_SEND_FAILURE"));
			break;
			
		case ICF_ERROR_CALL_LOGGING_FAILURE:
		ICF_PRINT_MESSAGE(("\n Error String = ICF_ERROR_CALL_LOGGING_FAILURE"));
			break;
			
		case ICF_CAUSE_MM_AL_MEMORY_FAILURE:
		ICF_PRINT_MESSAGE(("\n Error String = ICF_CAUSE_MM_AL_MEMORY_FAILURE"));
			break;
			
		case ICF_CAUSE_INVALID_SERVICE_ACTION:
		ICF_PRINT_MESSAGE(("\n Error String = ICF_CAUSE_INVALID_SERVICE_ACTION"));
			break;
			
		case ICF_CAUSE_PARTY_ALREADY_ON_LOCAL_HOLD:
		ICF_PRINT_MESSAGE(("\n Error String = ICF_CAUSE_PARTY_ALREADY_ON_LOCAL_HOLD "));
			break;
			
		case ICF_CAUSE_REMOTE_REJECT_CALL_TRANFER_REQ:
		ICF_PRINT_MESSAGE(("\n Error String = ICF_CAUSE_REMOTE_REJECT_CALL_TRANFER_REQ"));
			break;
			
		case ICF_CAUSE_CALL_MERGE_FAIL:
		ICF_PRINT_MESSAGE(("\n Error String = ICF_CAUSE_CALL_MERGE_FAIL"));
			break;
			
		case ICF_CAUSE_SERVICE_NOT_ACTIVATED:
		ICF_PRINT_MESSAGE(("\n Error String = ICF_CAUSE_SERVICE_NOT_ACTIVATED "));
			break;
			
		case ICF_CAUSE_PROHIBITED_BY_SERVICE_INTERACTION:
		ICF_PRINT_MESSAGE(("\n Error String = ICF_CAUSE_PROHIBITED_BY_SERVICE_INTERACTION"));
			break;
			
		case ICF_CAUSE_GPRS_SUSPENDED:
		ICF_PRINT_MESSAGE(("\n Error String = ICF_CAUSE_GPRS_SUSPENDED"));
			break;
			
		case ICF_RES_THRESHOLD_REACHED:
		ICF_PRINT_MESSAGE(("\n Error String = ICF_RES_THRESHOLD_REACHED "));
			break;
			
		case ICF_ERROR_NO_PREF_ID_EXISTS:
		ICF_PRINT_MESSAGE(("\n Error String = ICF_ERROR_NO_PREF_ID_EXISTS"));
			break;
			
		case ICF_ERROR_GENERIC:
		ICF_PRINT_MESSAGE(("\n Error String = ICF_ERROR_GENERIC"));
			break;
			
		case ICF_ERROR_NO_MMI_CONTEXT_FOUND:
		ICF_PRINT_MESSAGE(("\n Error String = ICF_ERROR_NO_MMI_CONTEXT_FOUND"));
			break;
			
		#ifdef ICF_SIGCOMP_SUPPORT
		case ICF_ERROR_INCOMPLETE_MSG_RECEIVED:
	                {
	                 ICF_PRINT_MESSAGE(("\n Error String = ICF_ERROR_INCOMPLETE_MSG_RECEIVED "));
	                 break;
	
	               }
		#endif
		case ICF_ERROR_REFER_PENDING:
	                {   
	                ICF_PRINT_MESSAGE(("\n Error String = ICF_ERROR_REFER_PENDING"));
	                break;
	                }
		case ICF_ERROR_MISMATCH_IN_ADDR_PLAN:
		ICF_PRINT_MESSAGE(("\n Error String = ICF_ERROR_MISMATCH_IN_ADDR_PLAN"));
			break;
			
		case ICF_CAUSE_DNS_QUERY_FAILURE:
		ICF_PRINT_MESSAGE(("\n Error String = ICF_CAUSE_DNS_QUERY_FAILURE"));
			break;
			
		case ICF_CAUSE_REQUEST_ALREADY_PENDING:
		ICF_PRINT_MESSAGE(("\n Error String = ICF_CAUSE_REQUEST_ALREADY_PENDING"));
			break;
			
		case ICF_CAUSE_REMOTE_UNREACHABLE:
		ICF_PRINT_MESSAGE(("\n Error String = ICF_CAUSE_REMOTE_UNREACHABLE"));
			break;
			
		case ICF_CAUSE_IN_DIALOG_OTG_OPTIONS_RECEIVED:
		ICF_PRINT_MESSAGE(("\n Error String = ICF_CAUSE_IN_DIALOG_OTG_OPTIONS_RECEIVED"));
			break;
			
		case ICF_CAUSE_REFER_TO_ABSENT:
		ICF_PRINT_MESSAGE(("\n Error String = ICF_CAUSE_REFER_TO_ABSENT"));
			break;
			
		case ICF_CAUSE_OPEN_NW_SERVER_FAILED:
		ICF_PRINT_MESSAGE(("\n Error String = ICF_CAUSE_OPEN_NW_SERVER_FAILED"));
			break;
			
	}
}

/*****************************************************************************
 *
 * FUNCTION:        icf_print_rem_call_transfer_init_cfm
 *
 * DESCRIPTION:     Print cta init cfm messages
 *
 * Functions called:
 *
 *
 ****************************************************************************/
icf_return_t icf_print_rem_call_transfer_init_cfm(icf_msg_st* p_msg)
{

    icf_remote_call_transfer_initiated_cfm_st* p_rem_cta_init_cfm = ICF_NULL;

    p_rem_cta_init_cfm = (icf_remote_call_transfer_initiated_cfm_st*)p_msg->payload;

    icf_print_api_header(p_msg);



        ICF_PRINT_MESSAGE((
        "\nbit_mask = %u", p_rem_cta_init_cfm->bit_mask));


        ICF_PRINT_MESSAGE((
        "\nresult = %u", p_rem_cta_init_cfm->result));


        if (p_rem_cta_init_cfm->bit_mask & ICF_CALL_TRANS_INIT_ERROR_CAUSE_PRESENT) {
                ICF_PRINT_MESSAGE((
                        "\nerror_code = %u", p_rem_cta_init_cfm->error_code));
        }

        if (p_rem_cta_init_cfm->bit_mask & ICF_CALL_TRANS_INIT_CFM_CODE_PRESENT) {
                ICF_PRINT_MESSAGE((
                        "\nresponse_code = %u", p_rem_cta_init_cfm->response_code));
        }


    return ICF_SUCCESS;


}

icf_return_t icf_print_early_call_ind(icf_msg_st* p_msg)
{


    icf_early_incoming_call_ind_st* p_early_call_ind = NULL;

    icf_print_api_header(p_msg);
    p_early_call_ind = (icf_early_incoming_call_ind_st*)p_msg->payload;

    ICF_PRINT_MESSAGE(("\n bit_mask = %u",p_early_call_ind->bit_mask));

    if (p_early_call_ind->bit_mask & ICF_EARLY_CALL_CLG_PTY_ADDR)
    {
        ICF_PRINT_MESSAGE((
              "\ncalling_party_addr = (addr_type = %u, %s)",
              p_early_call_ind->calling_party_addr.addr_type,
              p_early_call_ind->calling_party_addr.addr_val.str));
    }

    if (p_early_call_ind->bit_mask & ICF_EARLY_CALL_CLG_PTY_NAME)
    {
        ICF_PRINT_MESSAGE((
              "\ncalling_party_name = %s",
              p_early_call_ind->calling_party_name.str));
    }

    if (p_early_call_ind->bit_mask & ICF_EARLY_CALL_CALLED_PTY_ADDR)
    {
        ICF_PRINT_MESSAGE((
              "\ncalled_party_addr = (addr_type = %u, %s)",
              p_early_call_ind->called_party_addr.addr_type,
              p_early_call_ind->called_party_addr.addr_val.str));
    }

    return ICF_SUCCESS;
}


icf_return_t icf_print_early_call_cfm(icf_msg_st* p_msg)
{


    icf_early_incoming_call_cfm_st* p_early_call_cfm = NULL;

    icf_print_api_header(p_msg);
    p_early_call_cfm = (icf_early_incoming_call_cfm_st*)p_msg->payload;


    ICF_PRINT_MESSAGE((
              "\nresult = %u, line_id = %u",
              p_early_call_cfm->result,
              p_early_call_cfm->line_id));


    return ICF_SUCCESS;
}

icf_return_t icf_print_progress_ind(icf_msg_st* p_msg)
{
    icf_progress_ind_st				*p_progress_ind = NULL;
    
    icf_print_api_header(p_msg);
    p_progress_ind = (icf_progress_ind_st *)p_msg->payload;

    ICF_PRINT_MESSAGE((
		"\nresponse_code = %u",
		p_progress_ind->response_code));
    
    return ICF_SUCCESS;
}


icf_return_t icf_print_media_connect_req(icf_msg_st* p_msg)
{
    icf_media_connect_req_st		*p_media_connect_req = NULL;
    icf_uint8_t  					header_list_count = 0;  
    icf_uint8_t  					body_list_count = 0, i=0;

    icf_print_api_header(p_msg);

    p_media_connect_req = (icf_media_connect_req_st *)p_msg->payload;

	ICF_PRINT_MESSAGE(("\nbit_mask = %u", p_media_connect_req->bit_mask));
    
	if (p_media_connect_req->bit_mask & ICF_PRIVACY_IND_PRESENT) 
	{
    	ICF_PRINT_MESSAGE(("\nprivacy_ind = %u", 
         	p_media_connect_req->privacy_ind));
	}

	/* Header LIST  */
    if (p_media_connect_req->bit_mask & ICF_HDR_LIST_PRESENT) 
	{
        header_list_count = (icf_uint8_t)p_media_connect_req->header_list.count;
        icf_print_header_list(p_media_connect_req->header_list.hdr, header_list_count);
    }
          
    /* Body LIST */
    if (p_media_connect_req->bit_mask & ICF_BODY_LIST_PRESENT) 
	{
        body_list_count = (icf_uint8_t)p_media_connect_req->body_list.count;
        icf_print_body_list(p_media_connect_req->body_list.body, body_list_count);
    }
    
	/* Stream LIST*/
#ifdef ICF_REL_OLD
    if (p_media_connect_req->bit_mask & ICF_STREAM_LIST_PRESENT) 
	{
        stream_list_count = p_media_connect_req->stream_list.count;
        icf_print_stream_list(p_media_connect_req->stream_list.stream, stream_list_count);
    }
#endif

	if (p_media_connect_req->bit_mask & ICF_MEDIA_CONNECT_DELETED_STREAM_PRESENT)
	{
		ICF_PRINT_MESSAGE(("\nDeleted stream count %u:", 
			p_media_connect_req->count));	

		for(i=0; i < p_media_connect_req->count; i++)
		{ 
			ICF_PRINT_MESSAGE(("\n stream type %u:",
				p_media_connect_req->deleted_media_streams[i].stream_type));
			ICF_PRINT_MESSAGE(("\n stream id %u:",
				p_media_connect_req->deleted_media_streams[i].stream_id));
		}
	}

	if (p_media_connect_req->bit_mask & ICF_MEDIA_CONNECT_MEDIA_MODE_PRESENT)
	{
		ICF_PRINT_MESSAGE(("\nmedia_mode = %u", 
			p_media_connect_req->media_mode));	
	}

	ICF_PRINT_MESSAGE((
		"\nsend_without_sdp = %u", 
		p_media_connect_req->send_without_sdp));	

    return ICF_SUCCESS;

}
icf_return_t icf_print_get_reg_status_req(icf_msg_st* p_msg)
{
    icf_get_register_status_req_st		*p_get_reg_status_req = ICF_NULL;

    icf_print_api_header(p_msg);
    
	p_get_reg_status_req = (icf_get_register_status_req_st *)p_msg->payload;

    ICF_PRINT_MESSAGE(("\nline_id = %u", p_get_reg_status_req->line_id));
 
    return ICF_SUCCESS;
}
 

icf_return_t icf_print_get_reg_status_resp(icf_msg_st* p_msg)
{
    icf_get_register_status_resp_st     *p_get_reg_status_resp = ICF_NULL;
	icf_list_st							*p_user_reg_list = ICF_NULL;
	icf_user_reg_status_st				*p_user_reg_data = ICF_NULL;	
	icf_uint32_t  						i = 1, addr_type = 0;

    icf_print_api_header(p_msg);
    
    p_get_reg_status_resp = 
			(icf_get_register_status_resp_st *)p_msg->payload;

    ICF_PRINT_MESSAGE(("\nResult = %u\n", p_get_reg_status_resp->result));        
  	if (p_get_reg_status_resp->result == ICF_FAILURE)
    {
    	ICF_PRINT_MESSAGE(("error_cause = %u\n", \
			p_get_reg_status_resp->error_cause));        
    }        
   	else
    {
		p_user_reg_list = p_get_reg_status_resp->p_user_reg_status;

    	while (p_user_reg_list != ICF_NULL)
        {
			p_user_reg_data = (icf_user_reg_status_st *) \
									(p_user_reg_list->p_data);

			ICF_PRINT_MESSAGE(("Info of user %u:\n", i));
			ICF_PRINT_MESSAGE(("     addr_type = %u,", \
				p_user_reg_data->user.addr_type));
			ICF_PRINT_MESSAGE((" addr_val = %s,", \
				p_user_reg_data->user.addr_val.str));
			
			switch (p_user_reg_data->reg_status)
			{
				case ICF_RGM_00_IDLE:
					ICF_PRINT_MESSAGE((" reg_status = IDLE\n"));
					break;
				case ICF_RGM_01_REGISTERING:
					ICF_PRINT_MESSAGE((" reg_status = REGISTERING\n"));
					break;
				case ICF_RGM_02_UNREGISTERED:
					ICF_PRINT_MESSAGE((" reg_status = UNREGISTERED\n"));
					break;
				case ICF_RGM_03_REGISTERED:
					ICF_PRINT_MESSAGE((" reg_status = REGISTERED\n"));
					break;
				case ICF_RGM_04_AUTHORIZING:
					ICF_PRINT_MESSAGE((" reg_status = AUTHORIZING\n"));
					break;
				case ICF_RGM_05_DEREGISTERING:
					ICF_PRINT_MESSAGE((" reg_status = DEREGISTERING\n"));
					break;
				default:
					ICF_PRINT_MESSAGE((" reg_status = UNKNOWN\n"));
					break;
			}

       		p_user_reg_list = p_user_reg_list->p_next;
            i++;
        } /* while */

  		addr_type = p_get_reg_status_resp->registrar_address.addr.addr_type;

        switch (addr_type)
		{
        	case ICF_TRANSPORT_ADDRESS_DOMAIN_NAME:
            	ICF_PRINT_MESSAGE(("Registrar address = %s,",\
				p_get_reg_status_resp->registrar_address.addr.addr.domain.str));
            	break;

        	case ICF_TRANSPORT_ADDRESS_IPV4_ADDR:
            	ICF_PRINT_MESSAGE(("\nRegistrar_address = %u.%u.%u.%u,",\
				p_get_reg_status_resp->registrar_address.addr.addr.\
                                                       ipv4_addr.octet_1,\
                p_get_reg_status_resp->registrar_address.addr.addr.\
                                                       ipv4_addr.octet_2,\
                p_get_reg_status_resp->registrar_address.addr.addr.\
                                                       ipv4_addr.octet_3,\
                p_get_reg_status_resp->registrar_address.addr.addr.\
                                                      ipv4_addr.octet_4));
            	break;
        } /* switch */

    	ICF_PRINT_MESSAGE((" port = %u,", \
            p_get_reg_status_resp->registrar_address.port_num));

    	ICF_PRINT_MESSAGE((" expiry = %u\n", \
			p_get_reg_status_resp->expires_duration / 1000));

      } /* else */

    return ICF_SUCCESS;
}


#endif


