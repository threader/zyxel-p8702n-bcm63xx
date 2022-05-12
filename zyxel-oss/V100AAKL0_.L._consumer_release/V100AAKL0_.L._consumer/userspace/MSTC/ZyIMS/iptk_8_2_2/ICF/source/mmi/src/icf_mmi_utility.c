/**************************************************************************
 * 
 * FILE NAME:   icf_mmi_utility.c
 * 
 * DESCRIPTION: This file contains the utility functions of MMI. This also 
 *              includes the functions needed for traces ans errors. 
 *              
 * REVISION HISTORY:    
 *  Date            Name            Reference       Comments
 * 22-Dec-2004    Shefali Aggarwal       LLD         Initial Version
 *          
 * 28-Mar-2005    Shefali Aggarwal                   Updations
 *          
 * 09-May-2005    Shefali Aggarwal                   Fix SPR 7405
 *          
 * 19-May-2005    Shefali Aggarwal                   Fix SPR 7553
 *          
 * 19-May-2005    Shefali Aggarwal                   Fix SPR 7586
 * 01-Jul-2005    Jalaj Negi	        IMS CLIENT    Fixed bugs in 
 *                                                   send interfaces
 *                                                    towards MM.
 *
 * 11-Aug-2005    Kirti Dhingra        IMS CLIENT    Updations
 * 19-Aug-2005	   Rohit Aggarwal		ICF			  Add routing code
 * 01-Dec-2005    Kirti Dhingra		ICF			  QoS Support for ICF
 * 08-Feb-2006	   Puneet Sharma		ICF			  Updations for ICF 4.3
 * 09-Mar-2006    Amit Sharma                        ICF merging with ICF2.0
 * 04-Jul-2006	   Mohan Dhawan			ICF			  QoS Support for ICF 6.0
 * 05-Jul-2006	   Puneet Sharma		ICF			  Multiple m-line support
 *													  for ICF 6.0
 * 25-Aug-2006      Amit Sharma    SPR 12554
 *                  Umang Singh
 *
 * 30-Nov-2006      Deepti Goyal    ICF 6.2         NAT Feature Support
 *
 * 08-Jan-2007    Mohit Sharma      ICF Rel 6.3     Forking and multianswer 
 *                                                  support
 * 30-Jan-2007    Mohit Sharma      SPR 14399       Memory Leak fixed
 * 11-Apr-2007    Deepti Goyal      SPR 14905       Codec change in media connect
 *                                                  request.
 * 16-Mar-2007      Deepti Goyal    SPR 14564       Correct no. of rel_public
 *                                  SPR 14574       ip_port_req.
 *                                  SPR 14577       
 * 18-Jan-2008    Kamini Gangwani   Rel 8.0         Set the transport mode in cms 
 *                                                  request structure.
 * 24-Jan-2008    Kamini Gangwani   Rel 8.0         Modified function icf_mmi_app_trace
 *                                                  for MMI traces 
 *                                                  request structure. 
 * 24-Jan-2008     Abhishek Dhammawat Rel8.0        Modified 
 *                                                  icf_mmi_send_rel_res_ind
 * 31-Jan-2008    Abhishek Dhammawat  Rel8.0        Renamed 
 *                                                  icf_mmi_send_qos_release_for_non_ims_req
 *                                                  as
 *                                                  icf_mmi_send_qos_release_res_req
 * 25-Mar-2008  Tarun Gupta     IPTK Rel 8.0        Merged User Param and 
 *                                                  Dynamic Payload 
 *                                                  Enhancements
 * 28-Mar-2008  Shruti Thakral      SPR 18316       Klocwork warning removal
 * 29-Mar-2008  Alok Tiwari         SPR 18235       Klocwork warning removal
 * 7-Apr-2008   Sumant Gupta        ICF 8.0         Rel7.1 Windows porting 
 *                                                  Merging
 * 11-Apr-2008  Shruti Thakral      SPR 18360,18316 Klocwork warning removal
 * 17-Apr-2008  Shruti Thakral      SPR 18358       Klocwork warning removal
 * 18-Apr-2008  Alok Tiwari                         KW fixing reverted from
 *                            function icf_mmi_check_sdp_for_commited_codecs
 * 28-Apr-2008  Shruti Thakral      SPR 18455       Klocwork Warning Removal
 * 29-Apr-2008  Shruti Thakral      SPR 18455       Klocwork Warning Removal
 * 29-Apr-2008  Alok Tiwari         SPR 18456       Klocwork Warning Removal
 * 30-Apr-2008  Shruti Thakral      SPR 18455       Klocwork Warning Removal
 * 30-Apr-2008  Alok Tiwari         SPR 18456       Klocwork Warning Removal
 * 01-May-2008  Shruti Thakral      SPR 18316       Klocwork Warning Removal
 * 02-May-2008  Shruti Thakral      SPR 18316       Klocwork Warning Removal
 * 03-Jul-2008  Anurag Khare        SPR 18684       Compilation issue on gcc 4.2.3
 * 01-Dec-2008  Rajiv Kumar         SPR 19215       CSR-1-6212448 Merged 
 * 05-Dec-2008  Tarun Gupta         SPR 19218       Merged CSR 1-6367281
 * 15-Dec-2008  Anurag Khare        SPR 19218       SPR 18901 Merged
 * 11-Feb-2009  Tarun Gupta         Rel 8.1 LLD     SDP Tunnelling Enhancements
 * 11-Feb-2009  Anurag Khare        ICF Rel 8.1     Changes for options enhancements
 * 17-Feb-2009  Tarun Gupta         Rel 8.1         Modified icf_mmi_trace_api_sdp :
 *                                                  Klokworks warning resolution
 * 04-Mar-2009 Saurabh Gupta         SPR 19349      Klocworks fix
 * 17-Mar-2009 Saurabh Gupta         SPR 19349      Klocworks fix
 * 01-Apr-2009 Kamal Ashraf         Rel 8.2         Klocworks Fix
 * 24-Apr-2009 Anuradha Gupta       SPR 19499       modified fn icf_mmi_trace_api_sdp
 * 11-Jun-2009 Ashutosh Mohan       SPR 19737       Changes done for GCC 4.1 warnings removal
 * 17-Aug-2009 Abhishek Dhammawat   SPR 19930       Modified 
 *                                                  icf_mmi_copy_remote_recvd_sdp_in_mmi_ctxt
 * 7-Oct-2009  Rajiv Kumar          SPR 20094       Modified 
 *                                                  icf_mmi_copy_remote_recvd_sdp_in_mmi_ctxt
 * Copyright (c) 2009, Aricent.  
 * 
 *****************************************************************************/          

#include "icf_internal_api_id.h"
#include "icf_mmi_defs.h"
#include "icf_mmi_types.h"
#include "icf_mmi_macro.h"
#include "icf_mmi_prototypes.h"
#include "icf_dbm_prototypes.h"
#include "icf_port_prototypes.h"
#include "icf_ecode.h"
ICF_MM_ROUTING_START
#include "icf_defs.h"
#include "icf_error_id.h"
ICF_MM_ROUTING_END
#include "icf_mm_api.h"
#ifdef ICF_QOS_SUPPORT
#include "icf_nm_types.h"
#endif
#include "icf_common_prototypes.h"

#ifdef ICF_PORT_WINDOWS
#pragma warning ( disable : 4702 )
#endif


/******************************************************************************
 *
 * FUNCTION:        icf_mmi_send_gcl_req
 *
 * DESCRIPTION:     This function is invoked when ever any function needs to 
 *                  send the GCL request to the RM.
 *                   
 * RETURNS:         ICF_SUCCESS in case of success and ICF_FAILURE in 
 *                  case of any failure.       
 *
 ******************************************************************************/

icf_return_t icf_mmi_send_gcl_req(
        INOUT icf_mmi_pdb_st             *p_mmi_pdb)
{
    icf_return_t                        ret_val = ICF_FAILURE,
										dummy_ret_val = ICF_FAILURE;
    icf_msg_st                          *p_mmi_msg = ICF_NULL;
    icf_uint16_t                        api_len;
    icf_get_codec_list_req_st			*p_mmi_msg_payload = ICF_NULL;
	icf_mmi_internal_mms_resp_st		*p_mms_resp = ICF_NULL;
	icf_mmi_internal_cms_resp_st		*p_cms_resp = ICF_NULL;
	icf_stream_id_t						stream_count = ICF_NULL,
										stream_id = ICF_NULL,
										api_stream_count = ICF_NULL;
	icf_sdp_stream_st					sdp;
	icf_stream_capab_st					*p_node = ICF_NULL;
	icf_stream_capab_st					*p_stream_capab_node = ICF_NULL;
	icf_media_type_t					stream_type = ICF_STREAM_TYPE_INVALID;
	icf_uint32_t						bitmask = ICF_NULL;
	icf_boolean_t						memget_fail = ICF_FALSE;
    icf_stream_sdp_info_st              *p_sdp = ICF_NULL;                
    icf_internal_list_st                *p_list = ICF_NULL;
    icf_boolean_t						flag_proceed = ICF_TRUE;
/* ZyXEL porting */
#if 1
    icf_line_data_st                    *p_line_data;
#else
/*    icf_line_data_st                    *p_line_data;*/
#endif
/* ZyXEL porting end*/
    
    ICF_FUNCTION_ENTER(p_mmi_pdb->p_glb_pdb)

    /*initialize variable sdp of type  icf_sdp_stream_st */
    icf_mmi_init_sdp(&sdp); 
        
    api_len = ICF_PORT_SIZEOF(icf_api_header_st) + 
        ICF_PORT_SIZEOF(icf_get_codec_list_req_st);
    
    ICF_MEMGET(p_mmi_pdb->p_glb_pdb,
			api_len,
			ICF_MEM_COMMON,
			p_mmi_msg,
            ICF_DONOT_RET_ON_FAILURE,
			p_mmi_pdb->p_ecode,
			dummy_ret_val)

	if(ICF_FAILURE == dummy_ret_val)
	{
		ICF_ALARM(((icf_uint8_t *)"[MMI]: MEMGET failure"));
		return dummy_ret_val;
	}
        
    /* Fill the header of the message to be send */
    p_mmi_msg->hdr.api_id = ICF_GET_CODEC_LIST_REQ;
    p_mmi_msg->hdr.source_id = ICF_MODULE_ICF;
    p_mmi_msg->hdr.destination_id = ICF_MODULE_RM;
    p_mmi_msg->hdr.api_length = api_len;
    p_mmi_msg->hdr.call_id = p_mmi_pdb->p_call_ctx->call_id;
	p_mmi_msg->hdr.version = ICF_VERSION_1_0;
	p_mmi_msg->hdr.app_id = p_mmi_pdb->p_glb_pdb->p_call_ctx->app_id;
    
    /* Fill the payload */
    p_mmi_msg_payload = (icf_get_codec_list_req_st *)
        (p_mmi_msg->payload);

    p_mmi_msg_payload->line_id = p_mmi_pdb->p_call_ctx->line_id;
	p_mmi_msg_payload->p_stream_capab_list = ICF_NULL;

	icf_mmi_negotiate_amr_params(p_mmi_pdb);

	icf_port_memset(&sdp, 0, ICF_PORT_SIZEOF(sdp));

    /* If the ICF_OFFER_CODECS_IN_GCL in set CC call context , then 
      local codecs as in offer should be sent in gcl request  */

    if (ICF_OFFER_CODECS_IN_GCL == 
            (p_mmi_pdb->p_call_ctx->common_bitmask & 
             ICF_OFFER_CODECS_IN_GCL))
    {
        /* Reset the ICF_OFFER_CODECS_IN_GCL bit in bitmask */
       p_mmi_pdb->p_call_ctx->common_bitmask ^=  ICF_OFFER_CODECS_IN_GCL;
      
       p_mmi_msg_payload->bit_mask |= ICF_LOCAL_STREAM_CAPAB_INFO_PRESENT;

       p_list = p_mmi_pdb->p_call_ctx->p_stream_sdp_info;
       while(p_list)
       {
            p_sdp =  (icf_stream_sdp_info_st*)(p_list->p_payload);

            if (ICF_MEDIA_STREAM_ACTIVE != 
                        (p_sdp->stream_status & ICF_MEDIA_STREAM_ACTIVE))      
            {
                p_list = p_list->p_next;
                continue;
            }

			stream_id = p_sdp->stream_id;
			stream_type = p_sdp->stream_type;
            
            /* MEMGET for p_stream_capab_node as type icf_stream_capab_st */
			ICF_MEMGET (p_mmi_pdb->p_glb_pdb,
							ICF_PORT_SIZEOF(icf_stream_capab_st),
							ICF_MEM_COMMON,
							p_stream_capab_node,
							ICF_DONOT_RET_ON_FAILURE,
							p_mmi_pdb->p_ecode,
							dummy_ret_val)

             if(ICF_SUCCESS == dummy_ret_val)
             {
                 p_stream_capab_node->stream_id = stream_id;
                 p_stream_capab_node->stream_type = stream_type;
                 
                 /* Fill p_stream_capab_node with local SDP */
                 dummy_ret_val = icf_mmi_make_gcl_payload(p_mmi_pdb, 
									&p_sdp->p_local_sdp->sdp_stream, 
									stream_type,
									p_stream_capab_node);
                 if(ICF_SUCCESS == dummy_ret_val)
                 {
                    dummy_ret_val = icf_cmn_add_node_to_list(
									p_mmi_pdb->p_glb_pdb,
                                    &((icf_get_codec_list_req_st *)
                                        (p_mmi_msg->payload))->
                                        p_local_stream_capab_list,
                                        p_stream_capab_node);
                    if(ICF_FAILURE == dummy_ret_val)
                    {
                        /* MEMFREE stream capability list, p_stream_capab_node 
                         * and its member pointers */
                        icf_cmn_free_icf_stream_capab_list(p_mmi_pdb->p_glb_pdb,
                                &(((icf_get_codec_list_req_st *)p_mmi_msg->
                                        payload)->p_local_stream_capab_list));
                        icf_cmn_free_icf_stream_capab_node(p_mmi_pdb->p_glb_pdb,
                                &p_stream_capab_node);
                        flag_proceed = ICF_FALSE;
                        break;
                    }
                    ICF_KLOCWORK_FALSE_POSITIVE(p_mmi_pdb->p_glb_pdb,
                            p_stream_capab_node)
                 }
                 /* icf_mmi_make_gcl_payload shall always return SUCCESS,
                  * thus no else required */
             }
             else
             {
                 memget_fail = ICF_TRUE;
                 break;
             }
			
            p_list = p_list->p_next;
            
       }
    }

    if (ICF_TRUE == flag_proceed)
    {
        if(ICF_MMI_CMS_ONGOING == p_mmi_pdb->p_mmi_ctx->ongoing_event)
        {
            p_cms_resp = 
                (icf_mmi_internal_cms_resp_st*)p_mmi_pdb->p_mmi_ctx->p_mmi_resp;

            for(stream_count = 0;(stream_count<p_cms_resp->count) && (stream_count < ICF_MAX_STREAM_PER_CALL) ; stream_count++)
            {
                stream_id = p_cms_resp->stream_status[stream_count].stream_id;
                stream_type=p_cms_resp->stream_status[stream_count].stream_type;

                if((ICF_STREAM_TYPE_TBCP  != stream_type)&&
                        (ICF_TRUE == 
                        p_cms_resp->stream_status[stream_count].status_success))
                {
                    /* MEMGET for p_node as type icf_stream_capab_st */
                    ICF_MEMGET (p_mmi_pdb->p_glb_pdb,
                            ICF_PORT_SIZEOF(icf_stream_capab_st),
                            ICF_MEM_COMMON,
                            p_node,
                            ICF_DONOT_RET_ON_FAILURE,
                            p_mmi_pdb->p_ecode,
                            dummy_ret_val)

                        if(ICF_SUCCESS == dummy_ret_val)
                        {
                            /* Added for RTP DSCP parameter */
/* ZyXEL porting */
#if 1 
                 if(ICF_FAILURE == icf_dbm_fetch_line_cntxt_blk( p_mmi_pdb->p_glb_pdb,
                                                                 p_mmi_pdb->p_call_ctx->line_id,
                                                                 (icf_line_data_st **)(&p_line_data),
                                                                 p_mmi_pdb->p_ecode ) )
                  {
                    dummy_ret_val = ICF_FAILURE;
                  } 
					p_node->stream_id = stream_id;
					p_node->stream_type = stream_type;
                    p_node->qos_mark = p_line_data->rtp_dscp;
					
					printf("\r\n TBD !! icf_mmi_util.c set qos_mark is %d\n", p_node->qos_mark);

#else
                            /*if(ICF_FAILURE == icf_dbm_fetch_line_cntxt_blk( p_mmi_pdb->p_glb_pdb,
                              p_mmi_pdb->p_call_ctx->line_id,
                              (icf_line_data_st **)(&p_line_data),
                              p_mmi_pdb->p_ecode ) )
                              {
                              dummy_ret_val = ICF_FAILURE;
                              }  */
                            p_node->stream_id = stream_id;
                            p_node->stream_type = stream_type;
                            /*p_node->qos_mark = p_line_data->rtp_dscp;*/
#endif
/* ZyXEL porting end*/
                        }
                        else
                        {
                            memget_fail = ICF_TRUE;
                            break;
                        }

                    /* Fill codecs from remote SDP in the request payload for this 
                     * stream - fast start Incoming call */
                    if(p_mmi_pdb->p_mmi_ctx->p_remote_recvd_sdp)
                    {
                        /* Get the remote_recvd_sdp for this stream from mmi ctxt */
                        dummy_ret_val = icf_mmi_get_sdp_for_stream_id(
                                p_mmi_pdb, 
                                stream_id, 
                                ICF_SDP_REMOTE, 
                                &sdp);
                        p_mmi_pdb->stream_id = stream_id;

                        icf_mmi_handle_local_dtmf_capability(
                                p_mmi_pdb,
                                &sdp);

                    }
                    else
                    {
                        /* Fill codecs from local SDP in the request payload for 
                         * this stream */
                        dummy_ret_val = icf_mmi_get_sdp_for_stream_id(
                                p_mmi_pdb,
                                stream_id,
                                ICF_SDP_LOCAL, 
                                &sdp);
                    }
                    if(ICF_SUCCESS == dummy_ret_val)
                    {
                        dummy_ret_val = icf_mmi_make_gcl_payload(p_mmi_pdb, 
                                &sdp, 
                                stream_type,
                                p_node);
                    }

                    /*				if((ICF_SUCCESS == dummy_ret_val)||*/
                    /*					(!(ICF_CALL_DIR & p_mmi_pdb->p_call_ctx->common_bitmask)))*/
                    {

                        dummy_ret_val = icf_cmn_add_node_to_list(
                                p_mmi_pdb->p_glb_pdb,
                                &((icf_get_codec_list_req_st *)
                                    (p_mmi_msg->payload))->
                                p_stream_capab_list,
                                p_node);

						if ( api_stream_count < ICF_MAX_STREAM_PER_CALL )
						{
                        p_mmi_pdb->p_mmi_ctx->api_stream[api_stream_count] = 
                            stream_id;
						}
                        api_stream_count++;
                    }
                    if(ICF_FAILURE == dummy_ret_val)
                    {
                        /* MEMFREE stream capability list, p_stream_capab_node 
                         * and its member pointers */
                        icf_cmn_free_icf_stream_capab_list(p_mmi_pdb->p_glb_pdb,
                                &(((icf_get_codec_list_req_st *)p_mmi_msg->
                                        payload)->p_local_stream_capab_list));
                        icf_cmn_free_icf_stream_capab_node(p_mmi_pdb->p_glb_pdb,
                                &p_node);
                        break;
                    }
                    else
                    {
                        ICF_KLOCWORK_FALSE_POSITIVE(p_mmi_pdb->p_glb_pdb,p_node)
                            ret_val = ICF_SUCCESS;
                    }
                }
            }	

        }
        else if(ICF_MMI_MMS_MOD_ONGOING == p_mmi_pdb->p_mmi_ctx->ongoing_event)
        {
            p_mms_resp = p_mmi_pdb->p_mmi_ctx->p_mmi_resp;

            for(stream_count = 0; (stream_count < p_mms_resp->mod_stream_count) && (stream_count < ICF_MAX_STREAM_PER_CALL); 
                    stream_count++)
            {
                stream_id = 
                    p_mms_resp->modified_stream_resp[stream_count].stream_id;
                stream_type = 
                    p_mms_resp->modified_stream_resp[stream_count].stream_type;
                bitmask = 
                    p_mms_resp->modified_stream_resp[stream_count].bitmask;

                if(ICF_STREAM_TYPE_TBCP  != stream_type)
                {
                    /* MEMGET for p_node as type icf_stream_capab_st */
                    ICF_MEMGET (p_mmi_pdb->p_glb_pdb,
                            ICF_PORT_SIZEOF(icf_stream_capab_st),
                            ICF_MEM_COMMON,
                            p_node,
                            ICF_DONOT_RET_ON_FAILURE,
                            p_mmi_pdb->p_ecode,
                            dummy_ret_val)

                        if(ICF_SUCCESS == dummy_ret_val)
                        {
                            p_node->stream_id = stream_id;
                            p_node->stream_type = stream_type;
                        }
                        else
                        {
                            memget_fail = ICF_TRUE;
                            break;
                        }

                    dummy_ret_val = ICF_FAILURE;

                    if(p_mmi_pdb->p_mmi_ctx->mmi_bitmask & 
                            ICF_RESTORATION_IN_PROGRESS)
                    {
                        if((ICF_FALSE == 
                                 p_mms_resp->modified_stream_resp[stream_count].
                                    status_success)&&
                                ((bitmask & ICF_REMOTE_SDP_CODEC_CHANGE) ||\
                                bitmask & ICF_LOCAL_MEDIA_CONNECT_CODEC_CHANGE))
                        {
                            /* Fill codecs from last_commited SDP in the request 
                             * payload for this stream */
                            dummy_ret_val = icf_mmi_get_sdp_for_stream_id(
                                    p_mmi_pdb,
                                    stream_id,
                                    ICF_SDP_LAST_COMMITED, 
                                    &sdp);

                            if(ICF_SUCCESS == dummy_ret_val)
                            {
                                dummy_ret_val = icf_mmi_make_gcl_payload(
                                        p_mmi_pdb, 
                                        &sdp, 
                                        stream_type,
                                        p_node);
                                ICF_KLOCWORK_FALSE_POSITIVE(p_mmi_pdb->
                                        p_glb_pdb,p_node)
                            }
                        }
                        else
                        {	
                            /* MEMFREE p_node */
                            ICF_MEMFREE(p_mmi_pdb->p_glb_pdb, p_node, 
                                    ICF_MEM_COMMON, p_mmi_pdb->p_ecode, 
                                    dummy_ret_val)
                            continue;
                        }
                    }
                    else
                        if(ICF_FALSE ==
                                p_mms_resp->modified_stream_resp[stream_count].
                                status_success)
                        {	
                            /* MEMFREE p_node */
                            ICF_MEMFREE(p_mmi_pdb->p_glb_pdb, p_node, 
                                    ICF_MEM_COMMON, p_mmi_pdb->p_ecode, 
                                    dummy_ret_val)
                            continue;
                        }
                        else
                            if((bitmask & ICF_REMOTE_SDP_CODEC_CHANGE)||
                               (bitmask & ICF_LOCAL_MEDIA_CONNECT_CODEC_CHANGE))
                            {
                                /* Fill codecs from remote SDP in the request 
                                 * payload for this stream */

                                dummy_ret_val = icf_mmi_get_sdp_for_stream_id(
                                        p_mmi_pdb,
                                        stream_id,
                                        ICF_SDP_REMOTE, 
                                        &sdp);

                                p_mmi_pdb->stream_id = stream_id;

                                icf_mmi_handle_local_dtmf_capability(
                                        p_mmi_pdb,
                                        &sdp);		

                                if(ICF_SUCCESS == dummy_ret_val)
                                {
                                    dummy_ret_val = icf_mmi_make_gcl_payload(
                                            p_mmi_pdb, 
                                            &sdp, 
                                            stream_type,
                                            p_node);
                                }
                            }
                            else 
                                if(bitmask & ICF_LOCAL_SDP_CODEC_CHANGE)
                                {
                                    /* Fill codecs from local SDP in the 
                                     * request payload for this stream */
                                    dummy_ret_val = 
                                        icf_mmi_get_sdp_for_stream_id(
                                            p_mmi_pdb,
                                            stream_id,
                                            ICF_SDP_LOCAL,
                                            &sdp);
                                    if(ICF_SUCCESS == dummy_ret_val)
                                    {
                                        dummy_ret_val= icf_mmi_make_gcl_payload(
                                                p_mmi_pdb,
                                                &sdp, 
                                                stream_type,
                                                p_node);
                                    }
                                }
                                else 
                                    if(bitmask & 
                                        ICF_LOCAL_ANSWER_REVERT_CODEC_CHANGE)
                                    {
                                        /* Fill codecs from last_commited SDP 
                                         * in request payload for this stream */
                                        dummy_ret_val = 
                                            icf_mmi_get_sdp_for_stream_id(
                                                p_mmi_pdb,
                                                stream_id,
                                                ICF_SDP_LAST_COMMITED, 
                                                &sdp);

                                        if(ICF_SUCCESS == dummy_ret_val)
                                        {
                                            dummy_ret_val = 
                                                icf_mmi_make_gcl_payload(
                                                    p_mmi_pdb, 
                                                    &sdp, 
                                                    stream_type,
                                                    p_node);
                                        }
                                    }

                    if(ICF_SUCCESS == dummy_ret_val)
                    {
                        dummy_ret_val = icf_cmn_add_node_to_list(
                                p_mmi_pdb->p_glb_pdb,
                                &((icf_get_codec_list_req_st *)
                                    (p_mmi_msg->payload))->p_stream_capab_list,
                                p_node);

						if ( api_stream_count < ICF_MAX_STREAM_PER_CALL )
						{
                        	p_mmi_pdb->p_mmi_ctx->api_stream[api_stream_count] = 
                            stream_id;
						}
                        api_stream_count++;
                    }
                    if(ICF_FAILURE == dummy_ret_val)
                    {
                        /* MEMFREE p_node */
                        ICF_MEMFREE(p_mmi_pdb->p_glb_pdb, p_node,ICF_MEM_COMMON,
                                p_mmi_pdb->p_ecode, dummy_ret_val)
                            break;
                    }
                    else
                    {
                        ICF_KLOCWORK_FALSE_POSITIVE(p_mmi_pdb->p_glb_pdb,p_node)
                            ret_val = ICF_SUCCESS;
                    }
                }
            }
        }
        else if(ICF_MMI_MMS_ADD_ONGOING == p_mmi_pdb->p_mmi_ctx->ongoing_event)
        {
            /* Fill remote or local codecs which ever are available in the 
             * request payload. */

            p_mms_resp = p_mmi_pdb->p_mmi_ctx->p_mmi_resp;

            for(stream_count = 0; (stream_count < p_mms_resp->add_stream_count) && (stream_count < ICF_MAX_STREAM_PER_CALL); 
                    stream_count++)
            {
                stream_id = 
                    p_mms_resp->added_stream_resp[stream_count].stream_id;
                stream_type = 
                    p_mms_resp->added_stream_resp[stream_count].stream_type;

                if((ICF_STREAM_TYPE_TBCP  != stream_type)&&
                        (ICF_TRUE == 
                         p_mms_resp->added_stream_resp[stream_count].
                         status_success))
                {
                    /* MEMGET for p_node as type icf_stream_capab_st */
                    ICF_MEMGET (p_mmi_pdb->p_glb_pdb,
                            ICF_PORT_SIZEOF(icf_stream_capab_st),
                            ICF_MEM_COMMON,
                            p_node,
                            ICF_DONOT_RET_ON_FAILURE,
                            p_mmi_pdb->p_ecode,
                            dummy_ret_val)

                        if(ICF_SUCCESS == dummy_ret_val)
                        {
                            p_node->stream_id = stream_id;
                            p_node->stream_type = stream_type;
                        }
                        else
                        {
                            memget_fail = ICF_TRUE;
                            break;
                        }
                    /* Fill codecs from remote SDP in the request payload for
                     * this stream */

                    if(p_mmi_pdb->p_mmi_ctx->p_remote_recvd_sdp)
                    {
                        /* Get remote_recvd_sdp for this stream from mmi ctxt */
                        dummy_ret_val = icf_mmi_get_sdp_for_stream_id(p_mmi_pdb,
                                stream_id, 
                                ICF_SDP_REMOTE, 
                                &sdp);
                    }
                    if(ICF_FAILURE == dummy_ret_val)
                    {
                        /* Fill codecs from local SDP in the request payload for
                         * this stream */
                        dummy_ret_val = icf_mmi_get_sdp_for_stream_id(p_mmi_pdb,
                                stream_id,
                                ICF_SDP_LOCAL, 
                                &sdp);
                    }

                    if(ICF_SUCCESS == dummy_ret_val)
                    {
                        dummy_ret_val = icf_mmi_make_gcl_payload(p_mmi_pdb, 
                                &sdp, 
                                stream_type,
                                p_node);
                    }
                    else
                    {
                        /* Case for Outgoing Re-INVITE for adding a T.38 stream
                         * when no profile recvd in app_add or Re-INVITE */
                        dummy_ret_val = ICF_SUCCESS;
                    }

                    if(ICF_SUCCESS == dummy_ret_val)
                    {
                        dummy_ret_val = icf_cmn_add_node_to_list(
                                p_mmi_pdb->p_glb_pdb,
                                &((icf_get_codec_list_req_st *)
                                    (p_mmi_msg->payload))->
                                p_stream_capab_list,
                                p_node);

						if ( api_stream_count < ICF_MAX_STREAM_PER_CALL )
						{
                        	p_mmi_pdb->p_mmi_ctx->api_stream[api_stream_count] = 
                            stream_id;
						}
                        api_stream_count++;
                    }
                    if(ICF_FAILURE == dummy_ret_val)
                    {
                        /* MEMFREE p_node */
                        ICF_MEMFREE(p_mmi_pdb->p_glb_pdb, p_node,ICF_MEM_COMMON,
                                p_mmi_pdb->p_ecode, dummy_ret_val)
                            break;
                    }
                    else
                    {
                        ICF_KLOCWORK_FALSE_POSITIVE(p_mmi_pdb->p_glb_pdb,p_node)
                            ret_val = ICF_SUCCESS;
                    }
                }
            }	
        }
    }/* end if ICF_TRUE == flag_proceed */

    if(ICF_SUCCESS == ret_val)
	{
		icf_cmn_get_list_count(
					p_mmi_msg_payload->p_stream_capab_list,
					&p_mmi_pdb->p_mmi_ctx->api_stream_count);

#ifdef ICF_TRACE_ENABLE
		p_mmi_pdb->p_mmi_msg_payload = (icf_void_t *)p_mmi_msg_payload;
		p_mmi_pdb->hdr = p_mmi_msg->hdr;

		ICF_MMI_TRACE(p_mmi_pdb,ICF_TRACE_HDR_SEND)
		ICF_MMI_TRACE(p_mmi_pdb,ICF_TRACE_GCL_DATA_SEND)

		icf_mmi_trace_api_sdp(p_mmi_pdb, ICF_GET_CODEC_LIST_REQ);
#endif
	    ret_val = icf_mmi_send(p_mmi_pdb,p_mmi_msg,p_mmi_pdb->p_ecode);
	}
	else if(ICF_TRUE == memget_fail)
	{
		/* This is the case when failure is returned while MEMGET */
		ICF_ALARM(((icf_uint8_t *)"[MMI]: MEMGET failure"));
	}
	else 
	{
		/* This is the case when failure is returned by some function */
		ICF_ALARM(((icf_uint8_t *)"[MMI]: GENERIC failure"));
	}

    /* Free memory for msg */
    ICF_MEMFREE(p_mmi_pdb->p_glb_pdb,
				p_mmi_msg,
				ICF_MEM_COMMON,
                p_mmi_pdb->p_ecode,
				dummy_ret_val)

    ICF_FUNCTION_EXIT(p_mmi_pdb->p_glb_pdb)
    return ret_val;
}/* End function */ 


/******************************************************************************
 *
 * FUNCTION:        icf_mmi_send_open_channel_req
 *
 * DESCRIPTION:     This function is invoked when ever any function needs to 
 *                  send the OPEN_CHANNEL request to the MM.
 *                   
 * RETURNS:         ICF_SUCCESS in case of success and ICF_FAILURE in 
 *                  case of any failure.       
 *
 ******************************************************************************/

icf_return_t icf_mmi_send_open_channel_req(
        INOUT icf_mmi_pdb_st             *p_mmi_pdb)
{
    icf_return_t                        ret_val = ICF_FAILURE,
										dummy_ret_val = ICF_FAILURE;
    icf_msg_st                          *p_mmi_msg = ICF_NULL;
    icf_int16_t                         api_len = 0;
    icf_open_media_channel_req_st		*p_mmi_msg_payload = ICF_NULL;
	icf_mmi_internal_cms_resp_st		*p_cms_resp = ICF_NULL;
	icf_mmi_internal_mms_resp_st		*p_mms_resp = ICF_NULL;
	icf_stream_id_t						stream_id = ICF_NULL,
										stream_count = ICF_NULL,
										api_stream_count = ICF_NULL;
	icf_channel_st						*p_node = ICF_NULL;
	icf_media_type_t					stream_type = ICF_STREAM_TYPE_INVALID;
	icf_boolean_t						memget_fail = ICF_FALSE;


    ICF_FUNCTION_ENTER(p_mmi_pdb->p_glb_pdb)

	api_len = ICF_PORT_SIZEOF(icf_api_header_st) + 
			ICF_PORT_SIZEOF(icf_open_media_channel_req_st);
        
    ICF_MEMGET(p_mmi_pdb->p_glb_pdb,api_len,ICF_MEM_COMMON,p_mmi_msg,
            ICF_RET_ON_FAILURE,p_mmi_pdb->p_ecode, dummy_ret_val)

	if(ICF_FAILURE == dummy_ret_val)
	{
		ICF_ALARM(((icf_uint8_t *)"[MMI]: MEMGET failure"));
		return dummy_ret_val;
	}
    
    /* Fill the header of the message to be send */
    p_mmi_msg->hdr.api_id = ICF_OPEN_MEDIA_CHANNEL_REQ;
    p_mmi_msg->hdr.source_id = ICF_MODULE_ICF;
    p_mmi_msg->hdr.destination_id = ICF_MODULE_MM;
    p_mmi_msg->hdr.api_length = api_len;
    p_mmi_msg->hdr.call_id = p_mmi_pdb->p_call_ctx->call_id;
	p_mmi_msg->hdr.version = ICF_VERSION_1_0;
	p_mmi_msg->hdr.app_id = p_mmi_pdb->p_glb_pdb->p_call_ctx->app_id;

    p_mmi_msg_payload = (icf_open_media_channel_req_st *)
        (p_mmi_msg->payload);

	p_mmi_msg_payload->p_channel_list = ICF_NULL;

	if(ICF_MMI_CMS_ONGOING == p_mmi_pdb->p_mmi_ctx->ongoing_event)
	{
		p_cms_resp = p_mmi_pdb->p_mmi_ctx->p_mmi_resp;

		for(stream_count = 0; (stream_count < p_cms_resp->count) && (stream_count < ICF_MAX_STREAM_PER_CALL); stream_count++)
		{
			stream_id = p_cms_resp->stream_status[stream_count].stream_id;
			stream_type = p_cms_resp->stream_status[stream_count].stream_type;

			if(ICF_TRUE == 
				p_cms_resp->stream_status[stream_count].status_success)
			{
				/* Make payload from cms resp */
				/* MEMGET for p_node as type icf_channel_st */
				ICF_MEMGET (p_mmi_pdb->p_glb_pdb,
							ICF_PORT_SIZEOF(icf_channel_st),
							ICF_MEM_COMMON,
							p_node,
							ICF_DONOT_RET_ON_FAILURE,
							p_mmi_pdb->p_ecode,
							dummy_ret_val)

				if(ICF_SUCCESS == dummy_ret_val && ICF_NULL != p_node)
				{
					p_node->stream_id = stream_id;
					p_node->stream_type = stream_type;

					dummy_ret_val = icf_cmn_add_node_to_list(
										p_mmi_pdb->p_glb_pdb,
                                        &((icf_open_media_channel_req_st *)
                                            (p_mmi_msg->payload))->
                                            p_channel_list,
										p_node);

					if ( api_stream_count < ICF_MAX_STREAM_PER_CALL )
					{
						p_mmi_pdb->p_mmi_ctx->api_stream[api_stream_count] = stream_id;
					}
					api_stream_count++;
				}
				else
				{
					memget_fail = ICF_TRUE;
					break;
				}
				if(ICF_FAILURE == dummy_ret_val)
				{
					/* MEMFREE p_node */
					ICF_MEMFREE(p_mmi_pdb->p_glb_pdb, p_node, ICF_MEM_COMMON, 
								p_mmi_pdb->p_ecode, dummy_ret_val)
					break;
				}
				else
				{
                    ICF_KLOCWORK_FALSE_POSITIVE(p_mmi_pdb->p_glb_pdb,p_node)
					ret_val = ICF_SUCCESS;
				}
			}
		}
	}
	else if(ICF_MMI_MMS_ADD_ONGOING == p_mmi_pdb->p_mmi_ctx->ongoing_event)
	{
		p_mms_resp = p_mmi_pdb->p_mmi_ctx->p_mmi_resp;

		for(stream_count = 0; (stream_count < p_mms_resp->add_stream_count) && (stream_count < ICF_MAX_STREAM_PER_CALL);
			stream_count++)
		{
			stream_id = p_mms_resp->added_stream_resp[stream_count].stream_id;
			stream_type = 
					p_mms_resp->added_stream_resp[stream_count].stream_type;

			if(ICF_TRUE == 
					p_mms_resp->added_stream_resp[stream_count].status_success)
			{
				/* Make payload from mms resp */
				/* MEMGET for p_node as type icf_channel_st */
				ICF_MEMGET (p_mmi_pdb->p_glb_pdb,
							ICF_PORT_SIZEOF(icf_channel_st),
							ICF_MEM_COMMON,
							p_node,
							ICF_DONOT_RET_ON_FAILURE,
							p_mmi_pdb->p_ecode,
							dummy_ret_val)

				if(ICF_SUCCESS == dummy_ret_val && ICF_NULL != p_node)
				{
					p_node->stream_id = stream_id;
					p_node->stream_type = stream_type;

					dummy_ret_val = icf_cmn_add_node_to_list(
										p_mmi_pdb->p_glb_pdb,
                                        &((icf_open_media_channel_req_st *)
                                            (p_mmi_msg->payload))->
                                            p_channel_list,
										p_node);

					if ( api_stream_count < ICF_MAX_STREAM_PER_CALL )
					{
						p_mmi_pdb->p_mmi_ctx->api_stream[api_stream_count] = stream_id;
					}
					api_stream_count++;
				}
				else
				{
					memget_fail = ICF_TRUE;
					break;
				}
				if(ICF_FAILURE == dummy_ret_val)
				{
					/* MEMFREE p_node */
					ICF_MEMFREE(p_mmi_pdb->p_glb_pdb, p_node, ICF_MEM_COMMON, 
								p_mmi_pdb->p_ecode, dummy_ret_val)
					break;
				}
				else
				{
                    ICF_KLOCWORK_FALSE_POSITIVE(p_mmi_pdb->p_glb_pdb,p_node)
					ret_val = ICF_SUCCESS;
				}
			}
		}
	}

	if(ICF_SUCCESS == ret_val)
	{
		icf_cmn_get_list_count(p_mmi_msg_payload->p_channel_list,
							&p_mmi_pdb->p_mmi_ctx->api_stream_count);

#ifdef ICF_TRACE_ENABLE
		p_mmi_pdb->p_mmi_msg_payload = (icf_void_t *)p_mmi_msg_payload;
		p_mmi_pdb->hdr = p_mmi_msg->hdr;

		ICF_MMI_TRACE(p_mmi_pdb,ICF_TRACE_HDR_SEND)
		ICF_MMI_TRACE(p_mmi_pdb,ICF_TRACE_OPEN_CHANNEL_DATA_SEND)
		icf_mmi_trace_api_sdp(p_mmi_pdb, ICF_OPEN_MEDIA_CHANNEL_REQ);
#endif
		ret_val = icf_mmi_send(p_mmi_pdb,p_mmi_msg,p_mmi_pdb->p_ecode);
	}
	else if(ICF_TRUE == memget_fail)
	{
		/* This is the case when failure is returned while MEMGET */
		ICF_ALARM(((icf_uint8_t *)"[MMI]: MEMGET failure"));
	}
	else 
	{
		/* This is the case when failure is returned by some function */
		ICF_ALARM(((icf_uint8_t *)"[MMI]: GENERIC failure"));
	}

	ICF_MEMFREE(p_mmi_pdb->p_glb_pdb,p_mmi_msg,ICF_MEM_COMMON,
        p_mmi_pdb->p_ecode,dummy_ret_val)
        
    ICF_FUNCTION_EXIT(p_mmi_pdb->p_glb_pdb)
    return ret_val;     
}/* End function */


/******************************************************************************
 *
 * FUNCTION:        icf_mmi_send_cms_req
 *
 * DESCRIPTION:     This function is invoked when ever any function needs to 
 *                  send the CMS request to the MM.
 *                   
 * RETURNS:         ICF_SUCCESS in case of success and ICF_FAILURE in 
 *                  case of any failure.       
 *
 ******************************************************************************/

icf_return_t icf_mmi_send_cms_req(
        INOUT icf_mmi_pdb_st             *p_mmi_pdb)
{
    icf_return_t                        ret_val = ICF_FAILURE,
										dummy_ret_val = ICF_FAILURE;
    icf_msg_st                          *p_mmi_msg = ICF_NULL;
    icf_int16_t                         api_len = 0;
    icf_create_media_session_req_st		*p_mmi_msg_payload = ICF_NULL;
	icf_mmi_internal_cms_resp_st		*p_cms_resp = ICF_NULL;
	icf_mmi_internal_mms_resp_st		*p_mms_resp = ICF_NULL;
	icf_stream_id_t						stream_count = ICF_NULL,
										stream_id = ICF_NULL,
										api_stream_count = ICF_NULL;
	icf_media_type_t					stream_type = ICF_STREAM_TYPE_INVALID;
	icf_req_stream_st					*p_node = ICF_NULL;
	icf_boolean_t						memget_fail = ICF_FALSE;
    icf_line_data_st                    *p_line_data = ICF_NULL;

    ICF_FUNCTION_ENTER(p_mmi_pdb->p_glb_pdb)

	api_len = ICF_PORT_SIZEOF(icf_api_header_st)+ 
				ICF_PORT_SIZEOF(icf_create_media_session_req_st);

    ICF_MEMGET(p_mmi_pdb->p_glb_pdb,api_len,ICF_MEM_COMMON,p_mmi_msg,
            ICF_DONOT_RET_ON_FAILURE,p_mmi_pdb->p_ecode, dummy_ret_val)

	if(ICF_FAILURE == dummy_ret_val)
	{
		ICF_ALARM(((icf_uint8_t *)"[MMI]: MEMGET failure"));
		return dummy_ret_val;
	}    
    /* Fill the header of the message to be send */
    p_mmi_msg->hdr.api_id = ICF_CREATE_MEDIA_SESSION_REQ;
    p_mmi_msg->hdr.source_id = ICF_MODULE_ICF;
    p_mmi_msg->hdr.destination_id = ICF_MODULE_MM;
    p_mmi_msg->hdr.api_length = api_len;
    p_mmi_msg->hdr.call_id = p_mmi_pdb->p_call_ctx->call_id;
	p_mmi_msg->hdr.version = ICF_VERSION_1_0;
	p_mmi_msg->hdr.app_id = p_mmi_pdb->p_glb_pdb->p_call_ctx->app_id;

    p_mmi_msg_payload = (icf_create_media_session_req_st *)
        (p_mmi_msg->payload);    
    p_mmi_msg_payload->bit_mask = ICF_NULL;

	p_mmi_msg_payload->bit_mask |= ICF_LINE_ID_PRESENT;
    p_mmi_msg_payload->line_id = p_mmi_pdb->p_call_ctx->line_id;

	p_mmi_msg_payload->p_stream_list_data = ICF_NULL;

	if(ICF_MMI_CMS_ONGOING == p_mmi_pdb->p_mmi_ctx->ongoing_event)
	{
		p_cms_resp = p_mmi_pdb->p_mmi_ctx->p_mmi_resp;

		for(stream_count = 0; (stream_count < p_cms_resp->count) && (stream_count < ICF_MAX_STREAM_PER_CALL) ; stream_count++)
		{
			stream_id = p_cms_resp->stream_status[stream_count].stream_id;
			stream_type = p_cms_resp->stream_status[stream_count].stream_type;

			/* For successful non-TBCP streams only */
			if(ICF_TRUE == 
					p_cms_resp->stream_status[stream_count].status_success)
			{
				/* MEMGET for p_node as type icf_req_stream_st */
				ICF_MEMGET (p_mmi_pdb->p_glb_pdb,
							ICF_PORT_SIZEOF(icf_req_stream_st),
							ICF_MEM_COMMON,
							p_node,
							ICF_DONOT_RET_ON_FAILURE,
							p_mmi_pdb->p_ecode,
							dummy_ret_val)


				if(ICF_SUCCESS == dummy_ret_val)
				{
                    ICF_KLOCWORK_FALSE_POSITIVE(p_mmi_pdb->p_glb_pdb,p_node)
                    /* Addition for RTP DSCP parameter */
                    if(ICF_FAILURE == icf_dbm_fetch_line_cntxt_blk(p_mmi_pdb->p_glb_pdb,
                                                                   p_mmi_pdb->p_call_ctx->line_id,
                                                                   (icf_line_data_st **)(&p_line_data),
                                                                   p_mmi_pdb->p_ecode) )
                     {
                       dummy_ret_val = ICF_FAILURE;
                     }  
                                                                   
					p_node->stream_id = stream_id;
					p_node->stream_type = stream_type;

					dummy_ret_val = icf_mmi_make_cms_payload(
										p_mmi_pdb, stream_id, 
										stream_type,p_node);
                    /* Set the transport mode in CMS request structure */
                       p_node->local_sdp.trans_mode =  
                                     p_mmi_pdb->p_call_ctx->media_transport_type;

                       ICF_PRINT(((icf_uint8_t*)"\n[MMI]:ICF Transport Type in CMS Request is %d\n",
                                                  p_node->local_sdp.trans_mode));

                    if(p_mmi_pdb->p_call_ctx->qos_mark != ICF_NULL)
                    { 
                      p_node->qos_mark = p_mmi_pdb->p_call_ctx->qos_mark;
                    }
                    else
                    {
                      p_node->qos_mark = p_line_data->rtp_dscp;
                    }
                    ICF_PRINT(((icf_uint8_t*)"\n[MMI]:ICF RTP_DSCP in CMS Request: %d\n",
                    p_node->qos_mark));
                    p_node->bit_mask |=ICF_CREATE_MEDIA_QOS_PARAMS_PRESENT;
					p_node->mode = 
						icf_mmi_get_mode_for_stream_id(p_mmi_pdb,stream_id);

					ICF_PRINT(((icf_uint8_t*)"\n[MMI]:ICF mode in CMS Request: %d\n", 
								p_node->mode));
					p_node->bit_mask |= ICF_CREATE_MEDIA_STREAM_MODE_PRESENT;

				}
				else
				{
					memget_fail = ICF_TRUE;
					break;
				}

				if(ICF_SUCCESS == dummy_ret_val)
				{
					dummy_ret_val = icf_cmn_add_node_to_list(
										p_mmi_pdb->p_glb_pdb,
                                        &((icf_create_media_session_req_st *)
                                            (p_mmi_msg->payload))->
                                            p_stream_list_data,
										p_node);

					if ( api_stream_count < ICF_MAX_STREAM_PER_CALL )
					{
						p_mmi_pdb->p_mmi_ctx->api_stream[api_stream_count] = stream_id;
					}
					api_stream_count++;
				}
				if(ICF_FAILURE == dummy_ret_val)
				{
                    ICF_KLOCWORK_FALSE_POSITIVE(p_mmi_pdb->p_glb_pdb,p_node)
					/* MEMFREE p_node and its member pointers*/
                    icf_mmi_free_icf_req_stream_node(p_mmi_pdb, 
                            &p_node,p_mmi_pdb->p_ecode);
					break;
				}
				else
				{
					ret_val = ICF_SUCCESS;
				}

			}
		}
	}
	else if(ICF_MMI_MMS_ADD_ONGOING == p_mmi_pdb->p_mmi_ctx->ongoing_event)
	{
		p_mms_resp = p_mmi_pdb->p_mmi_ctx->p_mmi_resp;

		for(stream_count = 0; (stream_count < p_mms_resp->add_stream_count) && (stream_count < ICF_MAX_STREAM_PER_CALL); 
			stream_count++)
		{
			stream_id = p_mms_resp->added_stream_resp[stream_count].stream_id;
			stream_type = 
					p_mms_resp->added_stream_resp[stream_count].stream_type;

			/* For successful stream only */
			if(ICF_TRUE == 
				p_mms_resp->added_stream_resp[stream_count].status_success)
			{
				/* MEMGET for p_node as type icf_req_stream_st */
				ICF_MEMGET (p_mmi_pdb->p_glb_pdb,
							ICF_PORT_SIZEOF(icf_req_stream_st),
							ICF_MEM_COMMON,
							p_node,
							ICF_DONOT_RET_ON_FAILURE,
							p_mmi_pdb->p_ecode,
							dummy_ret_val)

				if(ICF_SUCCESS == dummy_ret_val)
				{
                    ICF_KLOCWORK_FALSE_POSITIVE(p_mmi_pdb->p_glb_pdb,p_node)
                    /* Addition for RTP DSCP parameter */
                    if(ICF_FAILURE == icf_dbm_fetch_line_cntxt_blk(p_mmi_pdb->p_glb_pdb,
                                                                   p_mmi_pdb->p_call_ctx->line_id,
                                                                   (icf_line_data_st **)(&p_line_data),
                                                                   p_mmi_pdb->p_ecode) )
                     {
                       dummy_ret_val = ICF_FAILURE;
                     }
                     
					p_node->stream_id = stream_id;
					p_node->stream_type = stream_type;


					dummy_ret_val = icf_mmi_make_cms_payload(
											p_mmi_pdb, stream_id, 
											stream_type,p_node);

                    /* Set the transport mode in CMS request structure */
                      p_node->local_sdp.trans_mode =  
                              p_mmi_pdb->p_call_ctx->media_transport_type;

                     ICF_PRINT(((icf_uint8_t*)"\n[MMI]:ICF Transport Type in CMS Request is %d\n"
                                     ,p_node->local_sdp.trans_mode));

                    if(ICF_NULL != p_mmi_pdb->p_call_ctx->qos_mark)
                    {
                      p_node->qos_mark =  p_mmi_pdb->p_call_ctx->qos_mark;
                      ICF_PRINT(((icf_uint8_t*)"\n Call context QOS MARK\n")); 
                    }
                    else
                    {
                      p_node->qos_mark = p_line_data->rtp_dscp;
                    }  
                    ICF_PRINT(((icf_uint8_t*)"\n[MMI]:ICF RTP_DSCP in CMS Request is %d\n",
                    p_node->qos_mark));
                    p_node->bit_mask |= ICF_CREATE_MEDIA_QOS_PARAMS_PRESENT;

					p_node->mode = 
						icf_mmi_get_mode_for_stream_id(p_mmi_pdb,stream_id);

					ICF_PRINT(((icf_uint8_t*)"\n[MMI]:ICF mode in CMS Request: %d\n", 
							p_node->mode));
					p_node->bit_mask |= ICF_CREATE_MEDIA_STREAM_MODE_PRESENT;

				}
				else
				{
					memget_fail = ICF_TRUE;
					break;
				}
				if(ICF_SUCCESS == dummy_ret_val)
				{
					dummy_ret_val = icf_cmn_add_node_to_list(
										p_mmi_pdb->p_glb_pdb,
                                        &((icf_create_media_session_req_st *)
                                            (p_mmi_msg->payload))->
                                            p_stream_list_data,
										p_node);
					
					if ( api_stream_count < ICF_MAX_STREAM_PER_CALL )
					{
						p_mmi_pdb->p_mmi_ctx->api_stream[api_stream_count] = stream_id;
					}
					api_stream_count++;
				}
				if(ICF_FAILURE == dummy_ret_val)
				{
                    ICF_KLOCWORK_FALSE_POSITIVE(p_mmi_pdb->p_glb_pdb,p_node)
					/* MEMFREE p_node and its member pointers*/
                    icf_mmi_free_icf_req_stream_node(p_mmi_pdb, 
                            &p_node,p_mmi_pdb->p_ecode);
					break;
				}
				else
				{
					ret_val = ICF_SUCCESS;
				}
			}
		}
	}


	if(ICF_SUCCESS == ret_val)
	{
		icf_cmn_get_list_count(p_mmi_msg_payload->p_stream_list_data,
							&p_mmi_pdb->p_mmi_ctx->api_stream_count);
#ifdef ICF_TRACE_ENABLE
		p_mmi_pdb->p_mmi_msg_payload = (icf_void_t *)p_mmi_msg_payload;
		p_mmi_pdb->hdr = p_mmi_msg->hdr;

		ICF_MMI_TRACE(p_mmi_pdb,ICF_TRACE_HDR_SEND)
		ICF_MMI_TRACE(p_mmi_pdb,ICF_TRACE_CMS_DATA_SEND)
		
		icf_mmi_trace_api_sdp(p_mmi_pdb, ICF_CREATE_MEDIA_SESSION_REQ);
#endif
		ret_val = icf_mmi_send(p_mmi_pdb,p_mmi_msg,p_mmi_pdb->p_ecode);
	}
	else if(ICF_TRUE == memget_fail)
	{
		/* This is the case when failure is returned while MEMGET */
		ICF_ALARM(((icf_uint8_t *)"[MMI]: MEMGET failure"));
	}
	else 
	{
		/* This is the case when failure is returned by some function */
		ICF_ALARM(((icf_uint8_t *)"[MMI]: GENERIC failure"));
	}

	ICF_MEMFREE(p_mmi_pdb->p_glb_pdb,p_mmi_msg,ICF_MEM_COMMON,
		p_mmi_pdb->p_ecode,dummy_ret_val)
        
    ICF_FUNCTION_EXIT(p_mmi_pdb->p_glb_pdb)
    return ret_val;     
}/* End function */        


/******************************************************************************
 *
 * FUNCTION:        icf_mmi_send_mms_req
 *
 * DESCRIPTION:     This function is invoked when ever any function needs to 
 *                  send the MMS request to the MM.
 *                   
 * RETURNS:         ICF_SUCCESS in case of success and ICF_FAILURE in 
 *                  case of any failure.       
 *
 ******************************************************************************/

icf_return_t icf_mmi_send_mms_req(
        INOUT icf_mmi_pdb_st			*p_mmi_pdb)
{
    icf_return_t                        ret_val = ICF_FAILURE,
										dummy_ret_val = ICF_FAILURE;
    icf_msg_st                          *p_mmi_msg = ICF_NULL;
    icf_int16_t                         api_len;
    icf_modify_media_session_req_st		*p_mmi_msg_payload = ICF_NULL;
	icf_mmi_internal_mms_resp_st		*p_mms_resp = ICF_NULL;
	icf_modify_stream_st				*p_node = ICF_NULL;
	icf_stream_id_t						stream_count = ICF_NULL,
										stream_id = ICF_NULL,
										api_stream_count = ICF_NULL;
	icf_media_type_t					stream_type = ICF_STREAM_TYPE_INVALID;
	icf_sdp_stream_st					sdp;
	icf_boolean_t						memget_fail = ICF_FALSE;
	icf_uint32_t						bit_mask = ICF_NULL;
    /*icf_line_data_st                    *p_line_data = ICF_NULL;*/

    ICF_FUNCTION_ENTER(p_mmi_pdb->p_glb_pdb)
    icf_mmi_init_sdp(&sdp);
 
    api_len = ICF_PORT_SIZEOF(icf_api_header_st) + 
        ICF_PORT_SIZEOF(icf_modify_media_session_req_st);
    
    ICF_MEMGET(p_mmi_pdb->p_glb_pdb,api_len,ICF_MEM_COMMON,p_mmi_msg,
            ICF_RET_ON_FAILURE,p_mmi_pdb->p_ecode, dummy_ret_val)

	if(ICF_FAILURE == dummy_ret_val)
	{
		ICF_ALARM(((icf_uint8_t *)"[MMI]: MEMGET failure"));
		return dummy_ret_val;
	}			
    /* Fill the header of the message to be send */
    p_mmi_msg->hdr.api_id = ICF_MODIFY_MEDIA_SESSION_REQ;
    p_mmi_msg->hdr.source_id = ICF_MODULE_ICF;
    p_mmi_msg->hdr.destination_id = ICF_MODULE_MM;
    p_mmi_msg->hdr.api_length = api_len;
    p_mmi_msg->hdr.call_id = p_mmi_pdb->p_call_ctx->call_id;
	p_mmi_msg->hdr.version = ICF_VERSION_1_0;
	p_mmi_msg->hdr.app_id = p_mmi_pdb->p_glb_pdb->p_call_ctx->app_id;

   /* Fill the payload */
    p_mmi_msg_payload = (icf_modify_media_session_req_st *)
        (p_mmi_msg->payload);

	p_mmi_msg_payload->p_stream_data_list = ICF_NULL;

	p_mms_resp = p_mmi_pdb->p_mmi_ctx->p_mmi_resp;

	icf_port_memset(&sdp, 0, (icf_uint32_t)ICF_PORT_SIZEOF(sdp));

	for(stream_count = 0; (stream_count < p_mms_resp->mod_stream_count) && (stream_count < ICF_MAX_STREAM_PER_CALL);
		stream_count++)
	{
		if(ICF_TRUE == 
			p_mms_resp->modified_stream_resp[stream_count].send_mms)
		{
			stream_id = 
				p_mms_resp->modified_stream_resp[stream_count].stream_id;
			stream_type = 
				p_mms_resp->modified_stream_resp[stream_count].stream_type;
			bit_mask = p_mms_resp->modified_stream_resp[stream_count].bitmask;

			/* MEMGET for p_node as type icf_modify_stream_st */
			ICF_MEMGET (p_mmi_pdb->p_glb_pdb,
						ICF_PORT_SIZEOF(icf_modify_stream_st),
						ICF_MEM_COMMON,
						p_node,
						ICF_DONOT_RET_ON_FAILURE,
						p_mmi_pdb->p_ecode,
						dummy_ret_val)

			if(ICF_SUCCESS == dummy_ret_val)
			{
				/* Make default payload for this stream */
				p_node->stream_id = stream_id;
				p_node->stream_type = stream_type;
                /* Start :Additions for per line features */
                /*if(ICF_FAILURE == icf_dbm_fetch_line_cntxt_blk(p_mmi_pdb->p_glb_pdb,
                                                               p_mmi_pdb->p_call_ctx->line_id,
                                                               &p_line_data,
                                                               p_mmi_pdb->p_ecode))
                 {
                   dummy_ret_val = ICF_FAILURE;
                 }*/  
                 /* p_mmi_pdb->p_call_ctx->qos_mark = p_line_data->rtp_dscp;
                    ICF_PRINT(((icf_uint8_t*)"\n[MMI]:ICF RTP_DSCP in CMS Request: %d\n",
                    p_mmi_pdb->p_call_ctx->qos_mark));
                    p_node->bit_mask |=ICF_CREATE_MEDIA_QOS_PARAMS_PRESENT;*/
                /* End :Additions for per line features */
			}
			else
			{
				memget_fail = ICF_TRUE;
				break;
			}

			/* Fill up the mode */
			if((bit_mask & ICF_MODE_CHANGE) || 
          (ICF_LOCAL_MEDIA_CONNECT_MODE_CHANGE & bit_mask))
			{
				/* Make the mode in MMS payload from media_mode_to_be_applied 
				 * in stream_sdp_info for this stream */
				p_node->mode = icf_mmi_get_mode_for_stream_id(p_mmi_pdb,stream_id);
				p_node->bit_mask |= ICF_MODIFY_MEDIA_MODE_PRESENT;
			}
#ifdef ICF_REL_OLD
			else
			{
				dummy_ret_val = icf_cmn_fetch_sdp_in_list_for_stream_id(
											p_mmi_pdb->p_glb_pdb,
											stream_id,
											ICF_SDP_APPLIED,
											&p_int_sdp);

				if(ICF_FAILURE == dummy_ret_val)
				{
					dummy_ret_val = icf_cmn_fetch_sdp_in_list_for_stream_id(
											p_mmi_pdb->p_glb_pdb,
											stream_id,
											ICF_SDP_REMOTE,
											&p_int_sdp);
				}
				if(ICF_FAILURE == dummy_ret_val)
				{
					dummy_ret_val = icf_cmn_fetch_sdp_in_list_for_stream_id(
											p_mmi_pdb->p_glb_pdb,
											stream_id,
											ICF_SDP_LOCAL,
											&p_int_sdp);
				}
				/* In all cases except the case for mode change the mode
				 * shall be picked from the internal_sdp_stream */

				if(ICF_SUCCESS == dummy_ret_val)
				{
					p_node->mode = p_int_sdp->media_mode;
					p_node->bit_mask |= ICF_MODIFY_MEDIA_MODE_PRESENT;
				}
			}
#endif
			if(ICF_SUCCESS == dummy_ret_val)
			{
				/* Populate the payload depending on stream specific bitmask */			
                
                if(p_mmi_pdb->p_mmi_ctx->mmi_bitmask & 
                      ICF_RESTORATION_IN_PROGRESS)
                {
                    /* Make the MMS payload from last_commited SDP */
                    dummy_ret_val = icf_mmi_get_sdp_for_stream_id(
                                                p_mmi_pdb,stream_id,
                                                ICF_SDP_LAST_COMMITED, &sdp);

                    p_node->bit_mask |= ICF_MODIFY_REMOTE_SDP_STREAM_PRESENT;
                }
                else
				if((bit_mask & ICF_REMOTE_SDP_MEDIA_CHANGE)||
					(bit_mask & ICF_REMOTE_SDP_CODEC_CHANGE)||
					(bit_mask & ICF_REMOTE_ANSWER_CODEC_CHANGE))
				{
					/* Make the MMS payload from remote_recvd SDP */
					dummy_ret_val = icf_mmi_get_sdp_for_stream_id(
												p_mmi_pdb,stream_id,
												ICF_SDP_REMOTE, &sdp);

					p_node->bit_mask |= ICF_MODIFY_REMOTE_SDP_STREAM_PRESENT;
					
					p_mmi_pdb->stream_id = stream_id;

                    /*icf_mmi_handle_local_dtmf_capability(p_mmi_pdb, &sdp);*/
				}
				else 
				if((bit_mask & ICF_LOCAL_SDP_MEDIA_CHANGE)||
					(bit_mask & ICF_LOCAL_SDP_CODEC_CHANGE)||
                    (bit_mask & ICF_LOCAL_MEDIA_CONNECT_CODEC_CHANGE))
				{
					/* Make the MMS payload from local SDP */
					dummy_ret_val = icf_mmi_get_sdp_for_stream_id(
												p_mmi_pdb,stream_id,
												ICF_SDP_LOCAL, &sdp);
#ifdef ICF_NAT_MEDIA_TRAVERSAL
                    /* Update the SDP local address with the local Address 
                     * stored in MMI context. 
                     */
                    icf_port_memcpy(&(sdp.stream_address),
                            &(p_mmi_pdb->p_mmi_ctx->nat_streams[stream_id].\
                                local_addr),(icf_uint32_t)ICF_PORT_SIZEOF(\
                                    icf_transport_address_st));


#endif

					p_node->bit_mask |= ICF_MODIFY_LOCAL_SDP_STREAM_PRESENT;

				}
				else 
				if((bit_mask & ICF_REMOTE_ANSWER_REVERT_SDP_CHANGE)||
					(bit_mask & ICF_LOCAL_ANSWER_REVERT_SDP_CHANGE)||
					(bit_mask & ICF_REMOTE_ANSWER_REVERT_CODEC_CHANGE)||
					(bit_mask & ICF_LOCAL_ANSWER_REVERT_CODEC_CHANGE))
				{
					/* Make the MMS payload from last_commited SDP */
					dummy_ret_val = icf_mmi_get_sdp_for_stream_id(
												p_mmi_pdb,stream_id,
												ICF_SDP_LAST_COMMITED, &sdp);
				}

				 if((ICF_STREAM_TYPE_AUDIO == stream_type) &
                                  (ICF_FAX_PT_CALL == p_mmi_pdb->p_call_ctx->fax_type))
                                {
                                   if(ICF_ECHO_CANC_PRESENT & p_mmi_pdb->p_call_ctx->common_bitmask)
                                   {
                                        p_node->echo_cancellation = p_mmi_pdb->p_call_ctx->echo_canc;

                                        p_node->bit_mask |= ICF_MODIFY_MEDIA_ECHO_CAN_PRESENT;
                                  }
                                }

				dummy_ret_val = icf_mmi_make_mms_payload(
											p_mmi_pdb, &sdp, 
											stream_type,p_node);

				if((bit_mask & ICF_REMOTE_ANSWER_REVERT_CODEC_CHANGE)||
					(bit_mask & ICF_LOCAL_ANSWER_REVERT_CODEC_CHANGE)||
					(bit_mask & ICF_REMOTE_SDP_CODEC_CHANGE)||
					(bit_mask & ICF_REMOTE_ANSWER_CODEC_CHANGE)||
                    (bit_mask & ICF_LOCAL_MEDIA_CONNECT_CODEC_CHANGE))
				{
					p_mmi_pdb->stream_id = stream_id;

					icf_mmi_check_sdp_for_commited_codecs(p_mmi_pdb, 
						&p_node->sdp_stream.p_codec_info_list);
				}
			}

			if(ICF_SUCCESS == dummy_ret_val)
			{
				dummy_ret_val = icf_cmn_add_node_to_list(p_mmi_pdb->p_glb_pdb,
                        &((icf_modify_media_session_req_st *)
                            (p_mmi_msg->payload))->p_stream_data_list, p_node);

				if ( api_stream_count < ICF_MAX_STREAM_PER_CALL )
				{
					p_mmi_pdb->p_mmi_ctx->api_stream[api_stream_count] = stream_id;
				}
				api_stream_count++;
			}

			if(ICF_FAILURE == dummy_ret_val)
			{
				/* MEMFREE p_node */
				ICF_MEMFREE(p_mmi_pdb->p_glb_pdb, p_node, ICF_MEM_COMMON, 
							p_mmi_pdb->p_ecode, dummy_ret_val)
				break;
			}
			else
			{
                ICF_KLOCWORK_FALSE_POSITIVE(p_mmi_pdb->p_glb_pdb,p_node)
				ret_val = ICF_SUCCESS;
			}
		}
	}

	if(ICF_SUCCESS == ret_val)
	{
		icf_cmn_get_list_count(p_mmi_msg_payload->p_stream_data_list,
							&p_mmi_pdb->p_mmi_ctx->api_stream_count);
#ifdef ICF_TRACE_ENABLE
		p_mmi_pdb->p_mmi_msg_payload = (icf_void_t *)p_mmi_msg_payload;
		p_mmi_pdb->hdr = p_mmi_msg->hdr;

		ICF_MMI_TRACE(p_mmi_pdb,ICF_TRACE_HDR_SEND)
		ICF_MMI_TRACE(p_mmi_pdb,ICF_TRACE_MMS_DATA_SEND)
		
		icf_mmi_trace_api_sdp(p_mmi_pdb, ICF_MODIFY_MEDIA_SESSION_REQ);
#endif
		ret_val = icf_mmi_send(p_mmi_pdb,p_mmi_msg,p_mmi_pdb->p_ecode);
	}
	else if(ICF_TRUE == memget_fail)
	{
		/* This is the case when failure is returned while MEMGET */
		ICF_ALARM(((icf_uint8_t *)"[MMI]: MEMGET failure"));
	}
	else 
	{
		/* This is the case when failure is returned by some function */
		ICF_ALARM(((icf_uint8_t *)"[MMI]: GENERIC failure"));
	}

    ICF_MEMFREE(p_mmi_pdb->p_glb_pdb,p_mmi_msg,ICF_MEM_COMMON,
        p_mmi_pdb->p_ecode,dummy_ret_val)

	ICF_FUNCTION_EXIT(p_mmi_pdb->p_glb_pdb)
    return ret_val;        
}/* End function */       


/******************************************************************************
 *
 * FUNCTION:        icf_mmi_send_commit_req
 *
 * DESCRIPTION:     This function is invoked when ever any function needs to 
 *                  send codec commit req the RM.
 *                   
 * RETURNS:         ICF_SUCCESS in case of success and ICF_FAILURE in 
 *                  case of any failure.       
 *
 ******************************************************************************/

icf_return_t icf_mmi_send_commit_req(
    INOUT icf_mmi_pdb_st             *p_mmi_pdb)
{
    icf_return_t						ret_val = ICF_FAILURE,
										dummy_ret_val = ICF_FAILURE;
	icf_msg_st							*p_mmi_msg = ICF_NULL;
    icf_uint16_t						api_len = 0;
    icf_codec_commit_req_st				*p_mmi_msg_payload;
	icf_mmi_internal_cms_resp_st		*p_cms_resp = ICF_NULL;
	icf_mmi_internal_mms_resp_st		*p_mms_resp = ICF_NULL;
	icf_stream_id_t						stream_count = ICF_NULL,
										stream_id = ICF_NULL,
										api_stream_count = ICF_NULL;
	icf_media_type_t					stream_type = ICF_STREAM_TYPE_INVALID;
	icf_sdp_stream_st					sdp;
	icf_stream_capab_st					*p_node = ICF_NULL;
	icf_boolean_t						memget_fail = ICF_FALSE;
	icf_uint32_t						bitmask = ICF_NULL;

    ICF_FUNCTION_ENTER(p_mmi_pdb->p_glb_pdb)
    icf_mmi_init_sdp(&sdp);
    
    api_len = ICF_PORT_SIZEOF(icf_api_header_st) + 
        ICF_PORT_SIZEOF(icf_codec_commit_req_st);
    
    ICF_MEMGET(p_mmi_pdb->p_glb_pdb,api_len,ICF_MEM_COMMON,p_mmi_msg,
            ICF_FALSE,p_mmi_pdb->p_ecode,dummy_ret_val)

	if(ICF_FAILURE == dummy_ret_val)
	{
		ICF_ALARM(((icf_uint8_t *)"[MMI]: MEMGET failure"));
		return dummy_ret_val;
	}
        
    /* Fill the header of the message to be send */
    p_mmi_msg->hdr.api_id = ICF_CODEC_COMMIT_REQ;
    p_mmi_msg->hdr.source_id = ICF_MODULE_ICF;
    p_mmi_msg->hdr.destination_id = ICF_MODULE_RM;
    p_mmi_msg->hdr.api_length = api_len;
    p_mmi_msg->hdr.call_id = p_mmi_pdb->p_call_ctx->call_id;
	p_mmi_msg->hdr.version = ICF_VERSION_1_0;
	p_mmi_msg->hdr.app_id = p_mmi_pdb->p_glb_pdb->p_call_ctx->app_id;
    
    /* Fill the payload */
    p_mmi_msg_payload = (icf_codec_commit_req_st *)
        (p_mmi_msg->payload);

	p_mmi_msg_payload->p_stream_commit_list = ICF_NULL;

	icf_mmi_negotiate_amr_params(p_mmi_pdb);

	icf_port_memset(&sdp, 0, ICF_PORT_SIZEOF(sdp));

	if(ICF_MMI_CMS_ONGOING == p_mmi_pdb->p_mmi_ctx->ongoing_event)
	{
		p_cms_resp = p_mmi_pdb->p_mmi_ctx->p_mmi_resp;

		for(stream_count = 0; (stream_count < p_cms_resp->count) && (stream_count < ICF_MAX_STREAM_PER_CALL); stream_count++)
		{
			stream_id = p_cms_resp->stream_status[stream_count].stream_id;
			stream_type = p_cms_resp->stream_status[stream_count].stream_type;

			if((ICF_TRUE == 
				p_cms_resp->stream_status[stream_count].status_success)&&
				(ICF_STREAM_TYPE_TBCP != stream_type))
			{
				dummy_ret_val = icf_mmi_negotiate_codecs_per_stream(
												p_mmi_pdb, stream_id);

				if(ICF_SUCCESS == dummy_ret_val)
				{
					/* Fill COMMIT payload from remote SDP because commit while 
					 * CMS_ONGOING is possible only for INC FS call
					 * Get the remote_recvd_sdp for this stream from mmi ctxt */
					dummy_ret_val = icf_mmi_get_sdp_for_stream_id(
												p_mmi_pdb, stream_id, 
												ICF_SDP_LOCAL, &sdp);

					if(ICF_SUCCESS == dummy_ret_val)
					{
						/* MEMGET for p_node as type icf_stream_capab_st */
						ICF_MEMGET (p_mmi_pdb->p_glb_pdb,
									ICF_PORT_SIZEOF(icf_stream_capab_st),
									ICF_MEM_COMMON,
									p_node,
									ICF_DONOT_RET_ON_FAILURE,
									p_mmi_pdb->p_ecode,
									dummy_ret_val)

						if(ICF_SUCCESS == dummy_ret_val && ICF_NULL != p_node)
						{
							dummy_ret_val = icf_mmi_make_commit_payload(
												p_mmi_pdb,&sdp,
												stream_type,p_node);
							p_node->stream_id = stream_id;
							p_node->stream_type = stream_type;
						}
						else
						{
							memget_fail = ICF_TRUE;
							break;
						}

						if(ICF_SUCCESS == dummy_ret_val)
						{
							dummy_ret_val = 
								icf_cmn_add_node_to_list(
									p_mmi_pdb->p_glb_pdb,
                                    &((icf_codec_commit_req_st *)
                                        (p_mmi_msg->payload))->
                                        p_stream_commit_list,
									p_node);

							if ( api_stream_count < ICF_MAX_STREAM_PER_CALL )
							{ 
								p_mmi_pdb->p_mmi_ctx->api_stream[api_stream_count]= 
								stream_id;
							}
							api_stream_count++;
						}
						if(ICF_FAILURE == dummy_ret_val)
						{
							/* MEMFREE p_node */
							ICF_MEMFREE(p_mmi_pdb->p_glb_pdb, 
										p_node, 
										ICF_MEM_COMMON,
										p_mmi_pdb->p_ecode, 
										dummy_ret_val)
							break;
						}
						else
						{
                            ICF_KLOCWORK_FALSE_POSITIVE(p_mmi_pdb->p_glb_pdb,
                                    p_node)
							ret_val = ICF_SUCCESS;
						}
					}
				}
			}
		}
	}
	else if(ICF_MMI_MMS_MOD_ONGOING == p_mmi_pdb->p_mmi_ctx->ongoing_event)
	{
		p_mms_resp = p_mmi_pdb->p_mmi_ctx->p_mmi_resp;

		for(stream_count = 0; (stream_count < p_mms_resp->mod_stream_count) && (stream_count < ICF_MAX_STREAM_PER_CALL);
			stream_count++)
		{
			dummy_ret_val = ICF_FAILURE;

			stream_id= 
				p_mms_resp->modified_stream_resp[stream_count].stream_id;
			stream_type= 
				p_mms_resp->modified_stream_resp[stream_count].stream_type;
			bitmask = p_mms_resp->modified_stream_resp[stream_count].bitmask;

			if((ICF_TRUE == 
				p_mms_resp->modified_stream_resp[stream_count].status_success)&&
				(ICF_STREAM_TYPE_TBCP != stream_type)&&
				!(p_mmi_pdb->p_mmi_ctx->mmi_bitmask & 
						ICF_RESTORATION_IN_PROGRESS))
			{
				if(bitmask & ICF_REMOTE_ANSWER_CODEC_CHANGE)
				{
					dummy_ret_val = 
						icf_mmi_negotiate_codecs_per_stream(
												p_mmi_pdb, stream_id);

					if(ICF_SUCCESS == dummy_ret_val)
					{
						/* Fill COMMIT payload from remote SDP for this stream*/
						dummy_ret_val = icf_mmi_get_sdp_for_stream_id(
												p_mmi_pdb, stream_id, 
												ICF_SDP_REMOTE, &sdp);
						 p_mmi_pdb->stream_id = stream_id;

                        icf_mmi_handle_local_dtmf_capability(p_mmi_pdb, &sdp);

					}
				}
				else
				if((bitmask & ICF_REMOTE_SDP_CODEC_CHANGE)||\
                    (bitmask & ICF_LOCAL_MEDIA_CONNECT_CODEC_CHANGE))
				{
					dummy_ret_val = 
						icf_mmi_negotiate_codecs_per_stream(
												p_mmi_pdb, stream_id);

					if(ICF_SUCCESS == dummy_ret_val)
					{
						/* Fill COMMIT payload from remote SDP for this stream*/
						dummy_ret_val = icf_mmi_get_sdp_for_stream_id(
												p_mmi_pdb, stream_id, 
												ICF_SDP_LOCAL, &sdp);
					}				
				}
				else if((bitmask & ICF_REMOTE_ANSWER_REVERT_CODEC_CHANGE)||
						(bitmask & ICF_LOCAL_ANSWER_REVERT_CODEC_CHANGE))
				{
					/* Fill COMMIT payload from last_commited SDP for this 
					 * stream */

					dummy_ret_val = icf_mmi_get_sdp_for_stream_id(
												p_mmi_pdb, stream_id, 
												ICF_SDP_LAST_COMMITED, &sdp);
				}
			}
			/* The case when stream is marked failure and RESTORATION in 
			 * progress
			 * This is possible in case 
			 * 1) MMS failed while applying change for INC Re-INVITE, 
			 *	  COMMIT triggered after GCL
			 * 2) MMS failed while applying change for OTG Re-INVITE, 
			 *	  COMMIT triggered straight away
			 * 3) COMMIT failed while applying change for Re-INVITE
			 * fill the codecs from last commited SDP so that this stream is 
			 * reverted */
			else 
			if((ICF_FALSE == 
				p_mms_resp->modified_stream_resp[stream_count].status_success)&&
				(ICF_STREAM_TYPE_TBCP != stream_type)&&
				(p_mmi_pdb->p_mmi_ctx->mmi_bitmask & 
				 ICF_RESTORATION_IN_PROGRESS))
			{
				/* Fill COMMIT payload from last_commited SDP */
				dummy_ret_val = icf_mmi_get_sdp_for_stream_id(
												p_mmi_pdb, stream_id, 
												ICF_SDP_LAST_COMMITED, &sdp);
			}
			if((dummy_ret_val == ICF_SUCCESS)&&
				(ICF_STREAM_TYPE_TBCP != stream_type))
			{
				/* MEMGET for p_node as type icf_stream_capab_st */
				ICF_MEMGET (p_mmi_pdb->p_glb_pdb,
							ICF_PORT_SIZEOF(icf_stream_capab_st),
							ICF_MEM_COMMON,
							p_node,
							ICF_DONOT_RET_ON_FAILURE,
							p_mmi_pdb->p_ecode,
							dummy_ret_val)

				if(ICF_SUCCESS == dummy_ret_val)
				{
					dummy_ret_val = icf_mmi_make_commit_payload(
												p_mmi_pdb,&sdp,
												stream_type,p_node);
					
					p_node->stream_id = stream_id;
					p_node->stream_type = stream_type;
				}
				else
				{
					memget_fail = ICF_TRUE;
					break;
				}
				if(ICF_SUCCESS == dummy_ret_val)
				{
					dummy_ret_val = 
						icf_cmn_add_node_to_list(
								p_mmi_pdb->p_glb_pdb,
								&((icf_codec_commit_req_st *)
                                    (p_mmi_msg->payload))->p_stream_commit_list,
								p_node);

					if ( api_stream_count < ICF_MAX_STREAM_PER_CALL )
					{
						p_mmi_pdb->p_mmi_ctx->api_stream[api_stream_count] = stream_id;
					}
					api_stream_count++;
				}
				if(ICF_FAILURE == dummy_ret_val)
				{
					/* MEMFREE p_node */
					ICF_MEMFREE(p_mmi_pdb->p_glb_pdb, p_node, ICF_MEM_COMMON, 
						p_mmi_pdb->p_ecode, dummy_ret_val)
					break;							
				}
				else
				{
                    ICF_KLOCWORK_FALSE_POSITIVE(p_mmi_pdb->p_glb_pdb,p_node)
					ret_val = ICF_SUCCESS;
				}
			}
		}
	}
	else if(ICF_MMI_MMS_ADD_ONGOING == p_mmi_pdb->p_mmi_ctx->ongoing_event)
	{
		p_mms_resp = p_mmi_pdb->p_mmi_ctx->p_mmi_resp;

		for(stream_count = 0; (stream_count < p_mms_resp->add_stream_count) && (stream_count < ICF_MAX_STREAM_PER_CALL); 
			stream_count++)
		{
			stream_id = 
					p_mms_resp->added_stream_resp[stream_count].stream_id;
			stream_type = 
					p_mms_resp->added_stream_resp[stream_count].stream_type;

			if((ICF_TRUE == 
				p_mms_resp->added_stream_resp[stream_count].status_success)&&
				(ICF_STREAM_TYPE_TBCP != stream_type))
			{
				dummy_ret_val = 
					icf_mmi_negotiate_codecs_per_stream(p_mmi_pdb, stream_id);

				if(ICF_SUCCESS == dummy_ret_val)
				{
					/* Fill COMMIT payload from remote SDP because commit while 
					 * CMS_ONGOING is possible only for INC FS call
					 * Get the remote_recvd_sdp for this stream from mmi ctxt */
					dummy_ret_val = icf_mmi_get_sdp_for_stream_id(
														p_mmi_pdb, stream_id, 
														ICF_SDP_LOCAL, &sdp);

					if(ICF_SUCCESS == dummy_ret_val)
					{
						/* MEMGET for p_node as type icf_stream_capab_st */
						ICF_MEMGET (p_mmi_pdb->p_glb_pdb,
									ICF_PORT_SIZEOF(icf_stream_capab_st),
									ICF_MEM_COMMON,
									p_node,
									ICF_DONOT_RET_ON_FAILURE,
									p_mmi_pdb->p_ecode,
									dummy_ret_val)

						if(ICF_SUCCESS == dummy_ret_val && ICF_NULL != p_node)
						{
							dummy_ret_val = icf_mmi_make_commit_payload(
														p_mmi_pdb,&sdp,
														stream_type, p_node);
							p_node->stream_id = stream_id;
							p_node->stream_type = stream_type;
						}
						else
						{
							memget_fail = ICF_TRUE;
							break;
						}

						if(ICF_SUCCESS == dummy_ret_val)
						{
							dummy_ret_val = 
								icf_cmn_add_node_to_list(
									p_mmi_pdb->p_glb_pdb,
									&((icf_codec_commit_req_st *)
                                        (p_mmi_msg->payload))->
                                        p_stream_commit_list,
									p_node);

							if ( api_stream_count < ICF_MAX_STREAM_PER_CALL )
							{
								p_mmi_pdb->p_mmi_ctx->api_stream[api_stream_count] = stream_id;
							}
							api_stream_count++;
						}
						if(ICF_FAILURE == dummy_ret_val)
						{
							/* MEMFREE p_node */
							ICF_MEMFREE(p_mmi_pdb->p_glb_pdb, p_node, 
											ICF_MEM_COMMON, 
								p_mmi_pdb->p_ecode, dummy_ret_val)
							break;
						}
						else
						{
                            ICF_KLOCWORK_FALSE_POSITIVE(p_mmi_pdb->p_glb_pdb,
                                    p_node)
							ret_val = ICF_SUCCESS;
						}
					}
				}
			}
		}	
	}

	if(ICF_SUCCESS == ret_val)
	{
		icf_cmn_get_list_count(p_mmi_msg_payload->p_stream_commit_list,
							&p_mmi_pdb->p_mmi_ctx->api_stream_count);
#ifdef ICF_TRACE_ENABLE
		p_mmi_pdb->p_mmi_msg_payload = (icf_void_t *)p_mmi_msg_payload;
		p_mmi_pdb->hdr = p_mmi_msg->hdr;

		ICF_MMI_TRACE(p_mmi_pdb,ICF_TRACE_HDR_SEND)
		ICF_MMI_TRACE(p_mmi_pdb,ICF_TRACE_COMMIT_DATA_SEND)

		icf_mmi_trace_api_sdp(p_mmi_pdb, ICF_CODEC_COMMIT_REQ);
#endif
		ret_val = icf_mmi_send(p_mmi_pdb,p_mmi_msg,p_mmi_pdb->p_ecode);
	}
	else if(ICF_TRUE == memget_fail)
	{
		/* This is the case when failure is returned while MEMGET */
		ICF_ALARM(((icf_uint8_t *)"[MMI]: MEMGET failure"));
	}
	else 
	{
		/* This is the case when failure is returned by some function */
		ICF_ALARM(((icf_uint8_t *)"[MMI]: GENERIC failure"));
	}

    /* Free memory */
    ICF_MEMFREE(p_mmi_pdb->p_glb_pdb,p_mmi_msg,ICF_MEM_COMMON,
            p_mmi_pdb->p_ecode,dummy_ret_val)
        
    ICF_FUNCTION_EXIT(p_mmi_pdb->p_glb_pdb)
    return ret_val;
}


/******************************************************************************
 *
 * FUNCTION:        icf_mmi_send_dms_req
 *
 * DESCRIPTION:     This function is invoked when ever DMS request is to be
 *                  send to the MM.                   
 *                  
 * RETURNS:         ICF_SUCCESS in case of success and ICF_FAILURE in 
 *                  case of any failure.       
 *
 ******************************************************************************/

icf_return_t icf_mmi_send_dms_req(
        INOUT icf_mmi_pdb_st             *p_mmi_pdb)
{
    icf_return_t                        ret_val = ICF_FAILURE,
										dummy_ret_val = ICF_SUCCESS;
    icf_msg_st                          *p_mmi_msg = ICF_NULL;
	icf_uint8_t							api_len = ICF_NULL;
	icf_delete_media_session_req_st		*p_mmi_msg_payload = ICF_NULL;
	icf_mmi_mms_request_st				*p_mms_req = ICF_NULL;
	icf_stream_id_t						stream_count = ICF_NULL;
    
    ICF_FUNCTION_ENTER(p_mmi_pdb->p_glb_pdb)

	api_len = ICF_PORT_SIZEOF(icf_delete_media_session_req_st)+
			ICF_PORT_SIZEOF(icf_api_header_st);
    
	ICF_MEMGET(p_mmi_pdb->p_glb_pdb,api_len,
            ICF_MEM_COMMON,
			p_mmi_msg,
			ICF_RET_ON_FAILURE,
            p_mmi_pdb->p_ecode,
			dummy_ret_val)
                
	if(ICF_FAILURE == dummy_ret_val)
	{
		ICF_ALARM(((icf_uint8_t *)"[MMI]: MEMGET failure"));
		return dummy_ret_val;
	}
    /* Fill the header of the message to be send */
    p_mmi_msg->hdr.api_id = 
            ICF_DELETE_MEDIA_SESSION_REQ;
    p_mmi_msg->hdr.source_id = ICF_MODULE_ICF;
    p_mmi_msg->hdr.destination_id = ICF_MODULE_MM;
    p_mmi_msg->hdr.api_length = api_len;
    p_mmi_msg->hdr.call_id = p_mmi_pdb->p_call_ctx->call_id;
	p_mmi_msg->hdr.version = ICF_VERSION_1_0;
	p_mmi_msg->hdr.app_id = p_mmi_pdb->p_glb_pdb->p_call_ctx->app_id;

	/* Fill the payload */
	p_mmi_msg_payload = (icf_delete_media_session_req_st *)(p_mmi_msg->payload);
	
	if(ICF_MMI_MMS_DEL_ONGOING == p_mmi_pdb->p_mmi_ctx->ongoing_event)
	{
		p_mms_req = p_mmi_pdb->p_mmi_ctx->p_mms_req;
		
		p_mmi_msg_payload->stream_list.count = p_mms_req->p_delete_media->count;

		for(stream_count = 0; (stream_count< p_mms_req->p_delete_media->count) && (stream_count < ICF_MAX_STREAM_PER_CALL);
			stream_count++)
		{
			p_mmi_msg_payload->stream_list.stream_id[stream_count] = 
					p_mms_req->p_delete_media->stream_id[stream_count];

#ifdef ICF_TRACE_ENABLE
			p_mmi_pdb->stream_type = icf_mmi_get_stream_type_for_stream_id(
					p_mmi_pdb, 
                    p_mms_req->p_delete_media->stream_id[stream_count]);
            ICF_MMI_APP_TRACE(p_mmi_pdb,ICF_TRACE_STREAM_DELETE)
#endif

			ret_val = ICF_SUCCESS;
		}
	}
	else /* DMS_ONGOING case- when we need to delete all streams in the call */
	{
#ifdef ICF_TRACE_ENABLE
			p_mmi_pdb->stream_type = 0; 
            ICF_MMI_APP_TRACE(p_mmi_pdb,ICF_TRACE_STREAM_DELETE)
#endif

		ICF_MMI_RESET_BIT(p_mmi_pdb,ICF_MEDIA_ESTABLISHED)
		p_mmi_msg_payload->stream_list.count = ICF_NULL;
		ret_val = ICF_SUCCESS;
	}

	if(ICF_SUCCESS == ret_val)
	{
#ifdef ICF_TRACE_ENABLE
		p_mmi_pdb->hdr = p_mmi_msg->hdr;
		p_mmi_pdb->p_mmi_msg_payload = (icf_void_t *)p_mmi_msg_payload;
		ICF_MMI_TRACE(p_mmi_pdb,ICF_TRACE_HDR_SEND)
		ICF_MMI_TRACE(p_mmi_pdb,ICF_TRACE_DMS_DATA_SEND)  	
#endif
		ret_val = icf_mmi_send(p_mmi_pdb,p_mmi_msg,p_mmi_pdb->p_ecode);
	}

    ICF_MEMFREE(p_mmi_pdb->p_glb_pdb,
				p_mmi_msg,
				ICF_MEM_COMMON,
            	p_mmi_pdb->p_ecode,
				dummy_ret_val)

	ICF_FUNCTION_EXIT(p_mmi_pdb->p_glb_pdb)
    return ret_val;    
}


/******************************************************************************
 *
 * FUNCTION:        icf_mmi_send_rel_res_ind
 *
 * DESCRIPTION:     This function is invoked when ever any function needs to 
 *                  send the Relesase Resourece Request to the RM.
 *                   
 * RETURNS:         ICF_SUCCESS in case of success and ICF_FAILURE in 
 *                  case of any failure.       
 *
 ******************************************************************************/

icf_return_t icf_mmi_send_rel_res_ind(
        INOUT icf_mmi_pdb_st             *p_mmi_pdb)
{
    icf_return_t                         ret_val = ICF_SUCCESS,
										dummy_ret_val = ICF_SUCCESS;
    icf_msg_st                           *p_mmi_msg = ICF_NULL;
	icf_uint8_t							api_len = ICF_NULL;
	icf_rel_res_req_st					*p_mmi_msg_payload = ICF_NULL;
	icf_mmi_mms_request_st				*p_mms_req = ICF_NULL;
	icf_mmi_internal_mms_resp_st		*p_mms_resp = ICF_NULL;
	icf_mmi_internal_cms_resp_st		*p_cms_resp = ICF_NULL;
	icf_stream_id_t						stream_count = ICF_NULL;
	icf_stream_id_t						stream_id = ICF_NULL;
	icf_media_type_t					stream_type;
    
    ICF_FUNCTION_ENTER(p_mmi_pdb->p_glb_pdb)
        
	api_len = ICF_PORT_SIZEOF(icf_rel_res_req_st)+
				ICF_PORT_SIZEOF(icf_api_header_st);
    
	ICF_MEMGET(p_mmi_pdb->p_glb_pdb,
			api_len,
            ICF_MEM_COMMON,
			p_mmi_msg,
			ICF_RET_ON_FAILURE,
            p_mmi_pdb->p_ecode,
			dummy_ret_val)
			
	if(ICF_FAILURE == dummy_ret_val)
	{
		ICF_ALARM(((icf_uint8_t *)"[MMI]: MEMGET failure"));
		return dummy_ret_val;
	}
    /* Fill the header of the message to be send */
    p_mmi_msg->hdr.api_id = ICF_REL_RES_REQ;
    p_mmi_msg->hdr.source_id = ICF_MODULE_ICF;
    p_mmi_msg->hdr.destination_id = ICF_MODULE_RM;

    p_mmi_msg->hdr.api_length = api_len;
    p_mmi_msg->hdr.call_id = p_mmi_pdb->p_call_ctx->call_id;    
	p_mmi_msg->hdr.version = ICF_VERSION_1_0;

    if (ICF_NULL != p_mmi_pdb->p_glb_pdb->p_call_ctx)
	p_mmi_msg->hdr.app_id = p_mmi_pdb->p_glb_pdb->p_call_ctx->app_id;
    else
    {
       ICF_MEMFREE(p_mmi_pdb->p_glb_pdb,
			p_mmi_msg,
			ICF_MEM_COMMON,
                        p_mmi_pdb->p_ecode,
			dummy_ret_val)
        return ICF_FAILURE; 
    }

    p_mmi_pdb->hdr = p_mmi_msg->hdr;

	/* Fill the payload */
    p_mmi_msg_payload = (icf_rel_res_req_st *)(p_mmi_msg->payload);	

	/* Initialise count to NULL */
	p_mmi_msg_payload->stream_list.count = ICF_NULL;

	switch(p_mmi_pdb->p_mmi_ctx->ongoing_event)
	{
		case ICF_MMI_DMS_ONGOING:
			ICF_MMI_RESET_BIT(p_mmi_pdb,ICF_MMI_RES_RESERVED)
#ifdef ICF_TRACE_ENABLE
            p_mmi_pdb->stream_type = 0;
            ICF_MMI_APP_TRACE(p_mmi_pdb,ICF_TRACE_CODEC_RELEASE)
#endif

#ifdef ICF_QOS_SUPPORT
ICF_CHECK_QOS_SUPPORT_START(p_mmi_pdb->p_glb_pdb,ICF_TRUE)
        /* Rel8.0: This is hook for resource release. Previously
        icf_mmi_send_qos_release_req function was invoked here.
        However now we do not require to do PDP handling
        for IMS mode and keeping behaviour same for both 
        the IMS/NON IMS mode we invoke non ims function*/
		ret_val = icf_mmi_send_qos_release_res_req(p_mmi_pdb);
					
ICF_CHECK_QOS_SUPPORT_END
#endif

		break;

		case ICF_MMI_MMS_DEL_ONGOING:
			p_mms_req = p_mmi_pdb->p_mmi_ctx->p_mms_req;

		/* make REL_RES ind for all stream minus TBCP stream in DEL 
		 * action of MM_req */
			for(stream_count = 0; (stream_count < 
						p_mms_req->p_delete_media->count) && (stream_count < ICF_MAX_STREAM_PER_CALL);
						stream_count++)
			{
				stream_id = 
					p_mms_req->p_delete_media->stream_id[stream_count];

				stream_type = icf_mmi_get_stream_type_for_stream_id(
										p_mmi_pdb, stream_id);
			
#ifdef ICF_TRACE_ENABLE
                p_mmi_pdb->stream_type = stream_type;
                ICF_MMI_APP_TRACE(p_mmi_pdb,ICF_TRACE_CODEC_RELEASE)
#endif
			
				if(ICF_STREAM_TYPE_TBCP != stream_type)
				{	
					p_mmi_msg_payload->stream_list.stream_id
					[p_mmi_msg_payload->stream_list.count] = stream_id;

					p_mmi_msg_payload->stream_list.count++;
				}
			}
		break;

		case ICF_MMI_CMS_ONGOING:
			ICF_MMI_RESET_BIT(p_mmi_pdb,ICF_MMI_RES_RESERVED)
			p_cms_resp = p_mmi_pdb->p_mmi_ctx->p_mmi_resp;

			if(p_mmi_pdb->bitmask & ICF_MMI_PDB_CMS_FAILURE)
			{
			/* Make REL_RES ind for all Success streams in CMS_resp in MMI 
			 * ctxt minus TBCP stream */
				for(stream_count = 0; (stream_count < p_cms_resp->count) && (stream_count < ICF_MAX_STREAM_PER_CALL); 
					stream_count++)
				{
					if(ICF_TRUE == 
					p_cms_resp->stream_status[stream_count].status_success)
					{
						stream_id = 
						p_cms_resp->stream_status[stream_count].stream_id;
						stream_type = icf_mmi_get_stream_type_for_stream_id(
												p_mmi_pdb, stream_id);

#ifdef ICF_TRACE_ENABLE
                        p_mmi_pdb->stream_type = stream_type;
                        ICF_MMI_APP_TRACE(p_mmi_pdb,ICF_TRACE_CODEC_RELEASE)
#endif

						if(ICF_STREAM_TYPE_TBCP != stream_type)
						{
							p_mmi_msg_payload->stream_list.stream_id
								[p_mmi_msg_payload->stream_list.count] = 
								stream_id;

							p_mmi_msg_payload->stream_list.count++;
						}
					}
				}
			}
			else
			{
			/* Make REL_RES ind for all failure streams in CMS_resp in MMI 
			 * ctxt minus TBCP stream */
				for(stream_count = 0; (stream_count < p_cms_resp->count) && (stream_count < ICF_MAX_STREAM_PER_CALL); 
							stream_count++)
				{
					if(ICF_FALSE == 
					p_cms_resp->stream_status[stream_count].status_success)
					{
						stream_id = 
						p_cms_resp->stream_status[stream_count].stream_id;
						stream_type = icf_mmi_get_stream_type_for_stream_id
											(p_mmi_pdb, stream_id);

#ifdef ICF_TRACE_ENABLE
                        p_mmi_pdb->stream_type = stream_type;
                        ICF_MMI_APP_TRACE(p_mmi_pdb,ICF_TRACE_CODEC_RELEASE)
#endif

						if(ICF_STREAM_TYPE_TBCP != stream_type)
						{
							p_mmi_msg_payload->stream_list.stream_id
							[p_mmi_msg_payload->stream_list.count] = 
								stream_id;

							p_mmi_msg_payload->stream_list.count++;
						}
					}
				}
			}
		break;

		case ICF_MMI_MMS_ADD_ONGOING:
			p_mms_resp = p_mmi_pdb->p_mmi_ctx->p_mmi_resp;

		/* Make REL_RES ind for all failure streams in ADD action of 
		 * MM_req */
			for(stream_count = 0; 
							(stream_count < p_mms_resp->add_stream_count) && (stream_count < ICF_MAX_STREAM_PER_CALL); 
						stream_count++)
			{
				if(ICF_FALSE == 
				p_mms_resp->added_stream_resp[stream_count].status_success)
				{
					stream_id = 
						p_mms_resp->added_stream_resp[stream_count].stream_id;
					stream_type = icf_mmi_get_stream_type_for_stream_id
										(p_mmi_pdb, stream_id);

#ifdef ICF_TRACE_ENABLE
                   p_mmi_pdb->stream_type = stream_type;
                   ICF_MMI_APP_TRACE(p_mmi_pdb,ICF_TRACE_CODEC_RELEASE)
#endif

					if(ICF_STREAM_TYPE_TBCP != stream_type)
					{
						p_mmi_msg_payload->stream_list.stream_id
							[p_mmi_msg_payload->stream_list.count] = 
						stream_id;

						p_mmi_msg_payload->stream_list.count++;
					}
				}
			}
		break;
	}

#ifdef ICF_TRACE_ENABLE
	if(ICF_SUCCESS == ret_val)
	{
		p_mmi_pdb->hdr = p_mmi_msg->hdr;
		p_mmi_pdb->p_mmi_msg_payload = (icf_void_t *)p_mmi_msg_payload;
		ICF_MMI_TRACE(p_mmi_pdb,ICF_TRACE_HDR_SEND)
		ICF_MMI_TRACE(p_mmi_pdb,ICF_TRACE_REL_RES_DATA_SEND)  	
	}
#endif
	
    ret_val = icf_mmi_send(p_mmi_pdb,p_mmi_msg,p_mmi_pdb->p_ecode);

    ICF_MEMFREE(p_mmi_pdb->p_glb_pdb,
			p_mmi_msg,
			ICF_MEM_COMMON,
            p_mmi_pdb->p_ecode,
			dummy_ret_val)
    
	ICF_FUNCTION_EXIT(p_mmi_pdb->p_glb_pdb)
    return ret_val; 
}    


/******************************************************************************
 *
 * FUNCTION:    icf_mmi_send_close_channel_ind
 *
 * DESCRIPTION: 
 *****************************************************************************/

icf_return_t	icf_mmi_send_close_channel_ind
(
	icf_mmi_pdb_st		*p_mmi_pdb
)
{
	icf_return_t						ret_val = ICF_FAILURE,
										dummy_ret_val = ICF_SUCCESS;
    icf_msg_st                          *p_mmi_msg = ICF_NULL;
	icf_uint8_t							api_len = ICF_NULL;
	icf_mmi_mms_request_st				*p_mms_req = ICF_NULL;
	icf_mmi_internal_mms_resp_st		*p_mms_resp = ICF_NULL;
	icf_mmi_internal_cms_resp_st		*p_cms_resp = ICF_NULL;
	icf_internal_list_st				*p_src = ICF_NULL;
	icf_stream_id_t						stream_count = ICF_NULL,
										stream_id = ICF_NULL;
	icf_media_type_t					stream_type = ICF_STREAM_TYPE_INVALID;
	icf_channel_st						*p_node = ICF_NULL;
	icf_boolean_t						memget_fail = ICF_FALSE,
										send_close_channel = ICF_FALSE;

    ICF_FUNCTION_ENTER(p_mmi_pdb->p_glb_pdb)
        
	api_len = ICF_PORT_SIZEOF(icf_close_media_channel_req_st)+
			ICF_PORT_SIZEOF(icf_api_header_st);
    
	ICF_MEMGET(p_mmi_pdb->p_glb_pdb,api_len,
            ICF_MEM_COMMON,p_mmi_msg,ICF_RET_ON_FAILURE,
            p_mmi_pdb->p_ecode,dummy_ret_val)
        
	if(ICF_FAILURE == dummy_ret_val)
	{
		ICF_ALARM(((icf_uint8_t *)"[MMI]: MEMGET failure"));
		return dummy_ret_val;
	}
    /* Fill the header of the message to be send */
    p_mmi_msg->hdr.api_id = ICF_CLOSE_MEDIA_CHANNEL_REQ;
    p_mmi_msg->hdr.source_id = ICF_MODULE_ICF;
    p_mmi_msg->hdr.destination_id = ICF_MODULE_RM;
    p_mmi_msg->hdr.api_length = api_len;
    p_mmi_msg->hdr.call_id = p_mmi_pdb->p_call_ctx->call_id;    
	p_mmi_msg->hdr.version = ICF_VERSION_1_0;
	p_mmi_msg->hdr.app_id = p_mmi_pdb->p_glb_pdb->p_call_ctx->app_id;

    p_mmi_pdb->hdr = p_mmi_msg->hdr;

    ICF_MMI_TRACE(p_mmi_pdb,ICF_TRACE_HDR_SEND)

	/* Set the channel list in the payload to NULL */
	((icf_close_media_channel_req_st *)(p_mmi_msg->payload))->p_channel_list = 
        ICF_NULL;
	
	/* Case when DMS is not successful. So we need to close channel for all 
	 * streams */
	if(p_mmi_pdb->bitmask & ICF_MMI_PDB_ALL_STREAMS)
	{
		ICF_MMI_RESET_BIT(p_mmi_pdb,ICF_MMI_CHANNEL_OPENED)
		if(ICF_MMI_DMS_ONGOING == p_mmi_pdb->p_mmi_ctx->ongoing_event)
		{
			p_src = p_mmi_pdb->p_call_ctx->p_stream_sdp_info;

			/* Make close channel ind for all active stream in call ctxt */
			while(ICF_NULL != p_src)
			{
				if(ICF_MEDIA_STREAM_ACTIVE &
					((icf_stream_sdp_info_st *)p_src->p_payload)->stream_status)
				{
					/* MEMGET for p_node as type icf_channel_st */
					ICF_MEMGET (p_mmi_pdb->p_glb_pdb,
								ICF_PORT_SIZEOF(icf_channel_st),
								ICF_MEM_COMMON,
								p_node,
								ICF_DONOT_RET_ON_FAILURE,
								p_mmi_pdb->p_ecode,
								dummy_ret_val)

					if(ICF_SUCCESS == dummy_ret_val && ICF_NULL != p_node)
					{
						p_node->stream_id = 
							((icf_stream_sdp_info_st *)p_src->p_payload)->\
							stream_id;
						p_node->stream_type = 
							((icf_stream_sdp_info_st *)p_src->p_payload)->\
							stream_type;

						dummy_ret_val = 
							icf_cmn_add_node_to_list(p_mmi_pdb->p_glb_pdb,
                                    &((icf_close_media_channel_req_st *)
                                        (p_mmi_msg->payload))->p_channel_list,
                                    p_node);
					}
					else
					{
						memget_fail = ICF_TRUE;
						break;
					}

					if(ICF_FAILURE == dummy_ret_val)
					{
						/* MEMFREE p_node */
						ICF_MEMFREE(p_mmi_pdb->p_glb_pdb, p_node, ICF_MEM_COMMON, 
							p_mmi_pdb->p_ecode, dummy_ret_val)
						break;						
					}
					else
					{
                        ICF_KLOCWORK_FALSE_POSITIVE(p_mmi_pdb->p_glb_pdb,
                                p_node)
						ret_val = ICF_SUCCESS;
					}
				}
				p_src = p_src->p_next;
			}
		}
		else if(ICF_MMI_MMS_DEL_ONGOING == p_mmi_pdb->p_mmi_ctx->ongoing_event)
		{
			p_mms_req = p_mmi_pdb->p_mmi_ctx->p_mms_req;
			
			/* Make close channel ind for all stream in DEL action of MM_req */
			for(stream_count = 0; 
					(stream_count < p_mms_req->p_delete_media->count) && (stream_count < ICF_MAX_STREAM_PER_CALL);
					stream_count++)
			{
				/* MEMGET for p_node as type icf_channel_st */
				ICF_MEMGET (p_mmi_pdb->p_glb_pdb,
							ICF_PORT_SIZEOF(icf_channel_st),
							ICF_MEM_COMMON,
							p_node,
							ICF_DONOT_RET_ON_FAILURE,
							p_mmi_pdb->p_ecode,
							dummy_ret_val)

				if(ICF_SUCCESS == dummy_ret_val && ICF_NULL != p_node)
				{
					p_node->stream_id =
						p_mms_req->p_delete_media->stream_id[stream_count];
					p_node->stream_type = icf_mmi_get_stream_type_for_stream_id(
											p_mmi_pdb, 
											p_node->stream_id);

					dummy_ret_val = 
						icf_cmn_add_node_to_list(p_mmi_pdb->p_glb_pdb,
										&((icf_close_media_channel_req_st *)
                                        (p_mmi_msg->payload))->p_channel_list,
										p_node);
				}
				else
				{
					memget_fail = ICF_TRUE;
					break;
				}

				if(ICF_FAILURE == dummy_ret_val)
				{
					/* MEMFREE p_node */
					ICF_MEMFREE(p_mmi_pdb->p_glb_pdb, p_node, ICF_MEM_COMMON, 
						p_mmi_pdb->p_ecode, dummy_ret_val)
					break;						
				}
				else
				{
                    ICF_KLOCWORK_FALSE_POSITIVE(p_mmi_pdb->p_glb_pdb,p_node)
					ret_val = ICF_SUCCESS;
				}
			}
		}
	}
	else if(ICF_MMI_CMS_ONGOING == p_mmi_pdb->p_mmi_ctx->ongoing_event)
	{
		p_cms_resp = p_mmi_pdb->p_mmi_ctx->p_mmi_resp;

		for(stream_count = 0; (stream_count < p_cms_resp->count) && (stream_count < ICF_MAX_STREAM_PER_CALL) ;	stream_count++)
		{
			send_close_channel = ICF_FALSE;

			/* Case when we are closing channel because CMS resp is failure*/
			/* Make close Channel ind for all success streams in cms_resp in 
			 * MMI ctxt */
			if((p_mmi_pdb->bitmask & ICF_MMI_PDB_CMS_FAILURE)&&
				(ICF_TRUE == 
					p_cms_resp->stream_status[stream_count].status_success))
			{
				ICF_MMI_RESET_BIT(p_mmi_pdb,ICF_MMI_CHANNEL_OPENED)
				send_close_channel = ICF_TRUE;
			}
			else if(ICF_FALSE == 
					p_cms_resp->stream_status[stream_count].status_success)
			{
				send_close_channel = ICF_TRUE;
			}

			if(ICF_TRUE == send_close_channel)
			{
				/* MEMGET for p_node as type icf_channel_st */
				ICF_MEMGET (p_mmi_pdb->p_glb_pdb,
							ICF_PORT_SIZEOF(icf_channel_st),
							ICF_MEM_COMMON,
							p_node,
							ICF_DONOT_RET_ON_FAILURE,
							p_mmi_pdb->p_ecode,
							dummy_ret_val)

				if(ICF_SUCCESS == dummy_ret_val && ICF_NULL != p_node)
				{
					p_node->stream_id = 
						p_cms_resp->stream_status[stream_count].stream_id;
					p_node->stream_type = 
						p_cms_resp->stream_status[stream_count].stream_type;

					dummy_ret_val = 
						icf_cmn_add_node_to_list(p_mmi_pdb->p_glb_pdb,
										&((icf_close_media_channel_req_st *)
                                        (p_mmi_msg->payload))->p_channel_list,
										p_node);
				}
				else
				{
					memget_fail = ICF_TRUE;
					break;
				}

				if(ICF_FAILURE == dummy_ret_val)
				{
					/* MEMFREE p_node */
					ICF_MEMFREE(p_mmi_pdb->p_glb_pdb, p_node, ICF_MEM_COMMON, 
						p_mmi_pdb->p_ecode, dummy_ret_val)
					break;						
				}
				else
				{
                    ICF_KLOCWORK_FALSE_POSITIVE(p_mmi_pdb->p_glb_pdb,p_node)
					ret_val = ICF_SUCCESS;
				}
			}
		}
	}
	else
	if(ICF_MMI_MMS_ADD_ONGOING == p_mmi_pdb->p_mmi_ctx->ongoing_event)
	{
		p_mms_resp = p_mmi_pdb->p_mmi_ctx->p_mmi_resp;

		for(stream_count = 0; (stream_count < p_mms_resp->add_stream_count) && (stream_count < ICF_MAX_STREAM_PER_CALL);
			stream_count++)
		{
			stream_id = 
				p_mms_resp->added_stream_resp[stream_count].stream_id;
			stream_type = 
				p_mms_resp->added_stream_resp[stream_count].stream_type;

			if(((p_mmi_pdb->bitmask & ICF_MMI_PDB_CMS_FAILURE)&&
			(ICF_TRUE == p_mms_resp->added_stream_resp[stream_count].status_success))||
			(ICF_FALSE == p_mms_resp->added_stream_resp[stream_count].status_success))
			{
				/* MEMGET for p_node as type icf_channel_st */
				ICF_MEMGET (p_mmi_pdb->p_glb_pdb,
							ICF_PORT_SIZEOF(icf_channel_st),
							ICF_MEM_COMMON,
							p_node,
							ICF_DONOT_RET_ON_FAILURE,
							p_mmi_pdb->p_ecode,
							dummy_ret_val)

				if(ICF_SUCCESS == dummy_ret_val && ICF_NULL != p_node)
				{
					p_node->stream_id = stream_id;
					p_node->stream_type = stream_type;

					dummy_ret_val = 
						icf_cmn_add_node_to_list(p_mmi_pdb->p_glb_pdb,
										&((icf_close_media_channel_req_st *)
                                        (p_mmi_msg->payload))->p_channel_list,
										p_node);
				}
				else
				{
					memget_fail = ICF_TRUE;
					break;
				}

				if(ICF_FAILURE == dummy_ret_val)
				{
					/* MEMFREE p_node */
					ICF_MEMFREE(p_mmi_pdb->p_glb_pdb, p_node, ICF_MEM_COMMON, 
						p_mmi_pdb->p_ecode, dummy_ret_val)
					break;						
				}
				else
				{
                    ICF_KLOCWORK_FALSE_POSITIVE(p_mmi_pdb->p_glb_pdb,p_node)
					ret_val = ICF_SUCCESS;
				}				
			}
		}
	}

	if(ICF_SUCCESS == ret_val)
	{
#ifdef ICF_TRACE_ENABLE
		if(ICF_SUCCESS == ret_val)
		{
			p_mmi_pdb->hdr = p_mmi_msg->hdr;
			p_mmi_pdb->p_mmi_msg_payload = (icf_void_t *)(p_mmi_msg->payload);
			ICF_MMI_TRACE(p_mmi_pdb,ICF_TRACE_HDR_SEND)
			ICF_MMI_TRACE(p_mmi_pdb,ICF_TRACE_CLOSE_CHANNEL_DATA_SEND)  	
			icf_mmi_trace_api_sdp(p_mmi_pdb, ICF_CLOSE_MEDIA_CHANNEL_REQ);
		}
#endif
		ret_val = icf_mmi_send(p_mmi_pdb,p_mmi_msg,p_mmi_pdb->p_ecode);
	}
	else if(ICF_TRUE == memget_fail)
	{
		/* This is the case when failure is returned while MEMGET */
		ICF_ALARM(((icf_uint8_t *)"[MMI]: MEMGET failure"));
	}
	else 
	{
		/* This is the case when failure is returned by some function */
		ICF_ALARM(((icf_uint8_t *)"[MMI]: GENERIC failure"));
	}

	ICF_MEMFREE(p_mmi_pdb->p_glb_pdb,
				p_mmi_msg,
				ICF_MEM_COMMON,
        		p_mmi_pdb->p_ecode,
				dummy_ret_val)
        
    ICF_FUNCTION_EXIT(p_mmi_pdb->p_glb_pdb)

	return ret_val;
}


 /******************************************************************************
 *
 * FUNCTION:        icf_mmi_send_merge_req
 *
 * DESCRIPTION:     This function is invoked when ever merge req is to be
 *                  send to MM.
 *                   
 * RETURNS:         ICF_SUCCESS in case of success and ICF_FAILURE in 
 *                  case of any failure.       
 *
 ******************************************************************************/

icf_return_t icf_mmi_send_merge_req(
        INOUT icf_mmi_pdb_st             *p_mmi_pdb)
{
    icf_return_t                        ret_val = ICF_SUCCESS,
										dummy_ret_val = ICF_SUCCESS;
    icf_msg_st                          *p_mmi_msg = ICF_NULL;
    icf_int16_t                         api_len = 0;
    icf_merge_media_session_req_st		*p_mmi_msg_payload = ICF_NULL;
    icf_mmi_merge_request_st            *p_msg_data_recd = ICF_NULL;
	icf_stream_id_t						stream_count = ICF_NULL;

    ICF_FUNCTION_ENTER(p_mmi_pdb->p_glb_pdb)
        
    api_len = ICF_PORT_SIZEOF(icf_api_header_st) + 
        ICF_PORT_SIZEOF(icf_merge_media_session_req_st);
    
    ICF_MEMGET(p_mmi_pdb->p_glb_pdb,api_len,ICF_MEM_COMMON,p_mmi_msg,
            ICF_RET_ON_FAILURE,p_mmi_pdb->p_ecode,
            ret_val)

	if(ICF_FAILURE == ret_val)
	{
		ICF_ALARM(((icf_uint8_t *)"[MMI]: MEMGET failure"));
		return ret_val;
	}        
    /* Fill the header of the message to be send */
    p_mmi_msg->hdr.api_id = 
            ICF_MERGE_MEDIA_SESSION_REQ;
    p_mmi_msg->hdr.source_id = ICF_MODULE_ICF;
    p_mmi_msg->hdr.destination_id = ICF_MODULE_MM;
    p_mmi_msg->hdr.api_length = api_len; 
    p_mmi_msg->hdr.call_id = p_mmi_pdb->p_call_ctx->call_id;
	p_mmi_msg->hdr.version = ICF_VERSION_1_0;
	p_mmi_msg->hdr.app_id = p_mmi_pdb->p_glb_pdb->p_call_ctx->app_id;
    
    /* Fill the payload */
    p_mmi_msg_payload = (icf_merge_media_session_req_st *)
        (p_mmi_msg->payload);
    
    p_msg_data_recd = (icf_mmi_merge_request_st *)(p_mmi_pdb->
            p_internal_msg->p_msg_data);

    p_mmi_msg_payload->merge_call_id = p_msg_data_recd->merge_call_id_2;
    
	p_mmi_msg_payload->count = p_msg_data_recd->count;

	for(stream_count = 0; (stream_count < p_mmi_msg_payload->count) && (stream_count < ICF_MAX_STREAM_PER_CALL); 
		stream_count++)
	{
		p_mmi_msg_payload->merge_stream_id[stream_count].merge_stream_id = 
			p_msg_data_recd->merge_stream[stream_count].merge_stream_id;

		p_mmi_msg_payload->merge_stream_id[stream_count].stream_id= 
			p_msg_data_recd->merge_stream[stream_count].stream_id;
	}


#ifdef ICF_TRACE_ENABLE
	if(ICF_SUCCESS == ret_val)
	{
		p_mmi_pdb->hdr = p_mmi_msg->hdr;
		p_mmi_pdb->p_mmi_msg_payload = (icf_void_t *)p_mmi_msg_payload;
		ICF_MMI_TRACE(p_mmi_pdb,ICF_TRACE_HDR_SEND)
		ICF_MMI_TRACE(p_mmi_pdb,ICF_TRACE_MERGE_DATA_SEND)  	
	}
#endif

    ret_val = icf_mmi_send(p_mmi_pdb,p_mmi_msg,p_mmi_pdb->p_ecode);

    ICF_MEMFREE(p_mmi_pdb->p_glb_pdb,
				p_mmi_msg,
				ICF_MEM_COMMON,
				p_mmi_pdb->p_ecode,
				dummy_ret_val)
        
    ICF_FUNCTION_EXIT(p_mmi_pdb->p_glb_pdb)
    return ret_val;
}/* End function */  


#ifdef ICF_TRACE_ENABLE

/******************************************************************************
 *
 * FUNCTION:    icf_mmi_trace_internal_sdp
 *
 * DESCRIPTION: 
 *****************************************************************************/

icf_return_t	icf_mmi_trace_internal_sdp(
							  icf_mmi_pdb_st	*p_mmi_pdb,
							  icf_api_id_t		sdp_type)
{
	icf_return_t				ret_val = ICF_SUCCESS;
	icf_internal_sdp_stream_st	*p_dest = ICF_NULL;
	icf_internal_list_st		*p_sdp = ICF_NULL;

	ICF_FUNCTION_ENTER(p_mmi_pdb->p_glb_pdb)

	p_sdp = p_mmi_pdb->p_call_ctx->p_stream_sdp_info;

	while(ICF_NULL != p_sdp)
	{
		p_mmi_pdb->stream_id = ((icf_stream_sdp_info_st *)p_sdp->p_payload)->\
							   	stream_id;
		ICF_MMI_TRACE(p_mmi_pdb, ICF_TRACE_STREAM_ID)

		p_mmi_pdb->stream_type = ((icf_stream_sdp_info_st *)p_sdp->p_payload)->\
								stream_type;
		ICF_MMI_TRACE(p_mmi_pdb, ICF_TRACE_STREAM_TYPE)

		switch(sdp_type)
		{
			case ICF_SDP_LOCAL:
				p_dest = ((icf_stream_sdp_info_st *)p_sdp->p_payload)->\
						 p_local_sdp;
				ICF_PRINT(((icf_uint8_t*)"\n[MMI]:Tracing Local SDP"));
				break;

			case ICF_SDP_REMOTE:
				p_dest = ((icf_stream_sdp_info_st *)p_sdp->p_payload)->\
						 p_remote_received_sdp;
				ICF_PRINT(((icf_uint8_t*)"\n[MMI]:Tracing Remote SDP"));
				break;

			case ICF_SDP_APPLIED:
				p_dest = ((icf_stream_sdp_info_st *)p_sdp->p_payload)->\
						 p_applied_sdp;
				ICF_PRINT(((icf_uint8_t*)"\n[MMI]:Tracing Applied SDP"));
				break;

			case ICF_SDP_LAST_COMMITED:
				p_dest = ((icf_stream_sdp_info_st *)p_sdp->p_payload)->\
						 p_last_commited_sdp;
				ICF_PRINT(((icf_uint8_t*)"\n[MMI]:Tracing Last Commited SDP"));
				break;
			
			default:
				p_dest = ICF_NULL;
				break;
		}

		if(ICF_NULL != p_dest)
		{
			icf_port_memcpy(&p_mmi_pdb->stream_addr,
				&(p_dest->sdp_stream.stream_address),
				(icf_uint32_t)ICF_PORT_SIZEOF(icf_transport_address_st));

			ICF_MMI_TRACE(p_mmi_pdb,ICF_TRACE_ADDR)

			if(ICF_STREAM_TYPE_TBCP != p_mmi_pdb->stream_type)
			{
				icf_mmi_trace_internal_codecs(p_mmi_pdb, 
								p_dest->sdp_stream.p_codec_info_list);
			}
		}
		p_sdp = p_sdp->p_next;
	}

	ICF_FUNCTION_EXIT(p_mmi_pdb->p_glb_pdb)

	return ret_val;
}


/******************************************************************************
 *
 * FUNCTION:    icf_mmi_trace_internal_codecs
 *
 * DESCRIPTION: 
 *****************************************************************************/

icf_return_t	icf_mmi_trace_internal_codecs(
							  icf_mmi_pdb_st			*p_mmi_pdb,
							  icf_list_st				*p_codec)
{
	icf_return_t		ret_val = ICF_SUCCESS;

	ICF_FUNCTION_ENTER(p_mmi_pdb->p_glb_pdb)

	icf_cmn_get_list_count(p_codec,&p_mmi_pdb->num_of_codecs);

	ICF_MMI_TRACE(p_mmi_pdb,ICF_TRACE_NUM_OF_CODECS)

	while(ICF_NULL != p_codec)
	{
		icf_port_memcpy(
			(icf_void_t *)&(p_mmi_pdb->codec),
			(icf_void_t *)(p_codec->p_data),
			(icf_uint32_t)ICF_PORT_SIZEOF(icf_codec_attrib_st));

		ICF_MMI_TRACE(p_mmi_pdb,ICF_TRACE_CODEC)

		p_codec = p_codec->p_next;
	}

	ICF_FUNCTION_EXIT(p_mmi_pdb->p_glb_pdb)

	return ret_val;
}


/******************************************************************************
 *
 * FUNCTION:    icf_mmi_trace_api_sdp
 *
 * DESCRIPTION: 
 *****************************************************************************/

icf_return_t	icf_mmi_trace_api_sdp(
							  icf_mmi_pdb_st	*p_mmi_pdb,
							  icf_api_id_t		api_id)
{
	icf_return_t				ret_val = ICF_SUCCESS;
	icf_list_st					*p_dest = ICF_NULL;
	icf_list_st					*p_sdp = ICF_NULL;
	icf_boolean_t				codecs_traced = ICF_FALSE,
								trace_addr = ICF_FALSE,
								mode_present = ICF_FALSE;
	icf_uint8_t					codec_count = ICF_NULL;
    
/* CSR 1-6212448: Fill the remote rtp address in the trace criteria data */
#ifdef ICF_TRACE_ENABLE
    icf_stream_data_st *p_stream_data = ICF_NULL;
    icf_list_st *p_stream_data_list = ICF_NULL;
    icf_error_t ecode;
#endif    

	ICF_FUNCTION_ENTER(p_mmi_pdb->p_glb_pdb)

    /* SPR 19499: The Preventive NULL checks added to avoid crash */    
    ICF_CHECK_IF_ANY_OF_TWO_VAR_IS_NULL(p_mmi_pdb->p_glb_pdb,
            p_mmi_pdb->p_glb_pdb->p_call_ctx,
            p_mmi_pdb->p_glb_pdb->p_call_ctx->p_trace_criteria_data,
            ret_val)
    if(ICF_FAILURE == ret_val)
    {
          ICF_FUNCTION_EXIT(p_mmi_pdb->p_glb_pdb)
          return ret_val;
    }

    p_stream_data_list = 
        p_mmi_pdb->p_glb_pdb->p_call_ctx->p_trace_criteria_data->p_stream_data;

	/* Fetch the list of streams */
	switch(api_id)
	{
		case ICF_GET_CODEC_LIST_REQ:
			p_sdp = 
			((icf_get_codec_list_req_st *)p_mmi_pdb->p_mmi_msg_payload)->
													p_stream_capab_list;
			break;
		case ICF_GET_CODEC_LIST_RESP:
			p_sdp = 
			((icf_get_codec_list_resp_st *)p_mmi_pdb->p_glb_pdb->
								p_recv_msg->payload)->p_reserved_stream_list;
			break;

		case ICF_OPEN_MEDIA_CHANNEL_REQ:
			p_sdp = 
			((icf_open_media_channel_req_st *)p_mmi_pdb->p_mmi_msg_payload)->
													p_channel_list;
			break;

		case ICF_CLOSE_MEDIA_CHANNEL_REQ:
			p_sdp = 
			((icf_close_media_channel_req_st *)p_mmi_pdb->p_mmi_msg_payload)->
													p_channel_list;
			break;

		case ICF_OPEN_MEDIA_CHANNEL_RESP:
			p_sdp = 
			((icf_open_media_channel_resp_st *)p_mmi_pdb->p_glb_pdb->
								p_recv_msg->payload)->p_channel_addr_list;
			break;

		case ICF_CREATE_MEDIA_SESSION_REQ:
			p_sdp = 
			((icf_create_media_session_req_st *)p_mmi_pdb->p_mmi_msg_payload)->
														p_stream_list_data;
			break;
		case ICF_CREATE_MEDIA_SESSION_RESP:
			p_sdp = 
			((icf_create_media_session_resp_st *)p_mmi_pdb->p_glb_pdb->
								p_recv_msg->payload)->p_accepted_stream_list;
			break;


		case ICF_CODEC_COMMIT_REQ:
			p_sdp = 
			((icf_codec_commit_req_st *)p_mmi_pdb->p_mmi_msg_payload)->
													p_stream_commit_list;
			break;

		case ICF_CODEC_COMMIT_RESP:
			p_sdp = ((icf_codec_commit_resp_st *)p_mmi_pdb->p_glb_pdb->
								p_recv_msg->payload)->p_commited_stream_list;
			break;


		case ICF_MODIFY_MEDIA_SESSION_REQ:
			p_sdp = 
			((icf_modify_media_session_req_st *)p_mmi_pdb->p_mmi_msg_payload)->
													p_stream_data_list;
			break;

		case ICF_MODIFY_MEDIA_SESSION_RESP:
			p_sdp = 
			((icf_modify_media_session_resp_st *)p_mmi_pdb->p_glb_pdb->
								p_recv_msg->payload)->p_accepted_stream_list;
			break;
                /* This code is not in use. To send this trace call context is required
                   which is not created for this API */
#if 0
		case ICF_MEDIA_CAPABILITIES_RESP:
			p_sdp = ((icf_media_capabilities_resp_st *)p_mmi_pdb->p_glb_pdb->
								p_recv_msg->payload)->p_resp_stream_list;
			break;
#endif
	}

	/* Fetch the list of codecs from the stream*/
	while(ICF_NULL != p_sdp)
	{
		switch(api_id)
		{
			case ICF_GET_CODEC_LIST_REQ:
				p_dest = 
				((icf_stream_capab_st *)p_sdp->p_data)->p_codec_info_list;

				p_mmi_pdb->stream_id = 
						((icf_stream_capab_st *)p_sdp->p_data)->stream_id;
				p_mmi_pdb->stream_type = 
						((icf_stream_capab_st *)p_sdp->p_data)->stream_type;


				break;

			case ICF_GET_CODEC_LIST_RESP:
				p_dest = 
				((icf_stream_capab_st *)p_sdp->p_data)->p_codec_info_list;

				p_mmi_pdb->stream_id = 
						((icf_stream_capab_st *)p_sdp->p_data)->stream_id;
				p_mmi_pdb->stream_type = 
						((icf_stream_capab_st *)p_sdp->p_data)->stream_type;

                p_mmi_pdb->p_codec_list = p_dest;
                p_mmi_pdb->result = ICF_SUCCESS;
                ICF_MMI_APP_TRACE(p_mmi_pdb, ICF_TRACE_CODEC_RESERVE)

				break;

			case ICF_OPEN_MEDIA_CHANNEL_REQ:
				p_dest = ICF_NULL;

				p_mmi_pdb->stream_id = 
						((icf_channel_st *)p_sdp->p_data)->stream_id;
				p_mmi_pdb->stream_type = 
						((icf_channel_st *)p_sdp->p_data)->stream_type;

				break;

			case ICF_CLOSE_MEDIA_CHANNEL_REQ:
				p_dest = ICF_NULL;

				p_mmi_pdb->stream_id = 
						((icf_channel_st *)p_sdp->p_data)->stream_id;
				p_mmi_pdb->stream_type = 
						((icf_channel_st *)p_sdp->p_data)->stream_type;

                ICF_MMI_APP_TRACE(p_mmi_pdb, ICF_TRACE_RTP_ADDR_DEALLOCATE)

				break;

			case ICF_OPEN_MEDIA_CHANNEL_RESP:
				p_dest = ICF_NULL;

				p_mmi_pdb->stream_id = 
						((icf_channel_addr_st *)p_sdp->p_data)->stream_id;

				p_mmi_pdb->stream_type = 
						((icf_channel_addr_st *)p_sdp->p_data)->stream_type;

				icf_port_memcpy(&p_mmi_pdb->stream_addr,
						&((icf_channel_addr_st*)p_sdp->p_data)->stream_address,
						(icf_uint32_t)ICF_PORT_SIZEOF(icf_transport_address_st));

				trace_addr = ICF_TRUE;

                p_mmi_pdb->result = ICF_TRUE;
                ICF_MMI_APP_TRACE(p_mmi_pdb,ICF_TRACE_RTP_ADDR_ALLOCATE)

				break;

			case ICF_CREATE_MEDIA_SESSION_REQ:
				p_mmi_pdb->stream_id = 
						((icf_req_stream_st *)p_sdp->p_data)->stream_id;
				p_mmi_pdb->stream_type = 
						((icf_req_stream_st *)p_sdp->p_data)->stream_type;

				if(((icf_req_stream_st *)p_sdp->p_data)->\
								local_sdp.p_codec_info_list)
				{
					p_dest = 
					((icf_req_stream_st *)p_sdp->p_data)->local_sdp.p_codec_info_list;

					icf_port_memcpy(&p_mmi_pdb->stream_addr,
						&((icf_req_stream_st*)p_sdp->p_data)->local_sdp.stream_address,
						(icf_uint32_t)ICF_PORT_SIZEOF(icf_transport_address_st));
				}
				else
				{
					p_dest = 
					((icf_req_stream_st *)p_sdp->p_data)->remote_sdp.p_codec_info_list;

					icf_port_memcpy(&p_mmi_pdb->stream_addr,
						&((icf_req_stream_st*)p_sdp->p_data)->remote_sdp.stream_address,
						(icf_uint32_t)ICF_PORT_SIZEOF(icf_transport_address_st));
				}
				/* CSR 1-6212448: Update the remote rtp address in trace criteria data in an 
				 * incoming call.
				*/
				if(ICF_CALL_DIR & p_mmi_pdb->p_call_ctx->common_bitmask)
				{
#ifdef ICF_TRACE_ENABLE

                    p_stream_data = icf_cmn_get_node_from_stream_data_list(
                            p_stream_data_list,
                            p_mmi_pdb->stream_type);

                    if(ICF_NULL == p_stream_data)
                    {
                        ICF_MEMGET (p_mmi_pdb->p_glb_pdb,
                                ICF_PORT_SIZEOF(icf_stream_data_st),
                                ICF_MEM_COMMON,
                                p_stream_data,
                                ICF_DONOT_RET_ON_FAILURE,
                                &ecode,
                                ret_val)
                        if (ICF_FAILURE == ret_val)
                        {
                            return ICF_FAILURE;
                        }

                        p_stream_data->stream_id = p_mmi_pdb->stream_id;
                        p_stream_data->stream_type = p_mmi_pdb->stream_type;

                        p_stream_data->remote_rtp_address = 
                            ((icf_req_stream_st*)p_sdp->p_data)->remote_sdp.stream_address;

                        icf_cmn_add_node_to_list(p_mmi_pdb->p_glb_pdb,
                                &(p_mmi_pdb->p_glb_pdb->p_call_ctx->p_trace_criteria_data
                                    ->p_stream_data), p_stream_data);

                        p_mmi_pdb->p_glb_pdb->p_call_ctx->p_trace_criteria_data
                            ->bitmask |= ICF_TRACE_REMOTE_RTP_IP_PRESENT;
                        p_mmi_pdb->p_glb_pdb->p_call_ctx->p_trace_criteria_data
                            ->bitmask |= ICF_TRACE_REMOTE_RTP_PORT_PRESENT;
                    }
                    else
                    {
                        p_stream_data->remote_rtp_address = 
                            ((icf_req_stream_st*)p_sdp->p_data)->remote_sdp.stream_address;

                        p_mmi_pdb->p_glb_pdb->p_call_ctx->p_trace_criteria_data
                            ->bitmask |= ICF_TRACE_REMOTE_RTP_IP_PRESENT;
                        p_mmi_pdb->p_glb_pdb->p_call_ctx->p_trace_criteria_data
                            ->bitmask |= ICF_TRACE_REMOTE_RTP_PORT_PRESENT;
                    }
                    /* Rel 8.1 : Klokworks warning resolution */
                    ICF_KLOCWORK_FALSE_POSITIVE(p_mmi_pdb->p_glb_pdb,p_stream_data)
#endif	
                }
                trace_addr = ICF_TRUE;
				if ( ICF_CREATE_MEDIA_STREAM_MODE_PRESENT &
					((icf_req_stream_st *)p_sdp->p_data)->bit_mask)
				{
					mode_present = ICF_TRUE;
					p_mmi_pdb->mode = ((icf_req_stream_st *)p_sdp->p_data)->mode;
				}
				break;

			case ICF_CREATE_MEDIA_SESSION_RESP:
				p_dest = 
					((icf_resp_stream_st *)p_sdp->p_data)->\
					local_sdp.p_codec_info_list;

				p_mmi_pdb->stream_id = 
						((icf_resp_stream_st *)p_sdp->p_data)->stream_id;
				p_mmi_pdb->stream_type = 
						((icf_resp_stream_st *)p_sdp->p_data)->stream_type;


				icf_port_memcpy(&p_mmi_pdb->stream_addr,
						&((icf_resp_stream_st*)p_sdp->p_data)->local_sdp.stream_address,
						(icf_uint32_t)ICF_PORT_SIZEOF(icf_transport_address_st));

				trace_addr = ICF_TRUE;
				break;


			case ICF_CODEC_COMMIT_REQ:
				p_dest = 
				((icf_stream_capab_st *)p_sdp->p_data)->p_codec_info_list;

				p_mmi_pdb->stream_id = 
						((icf_stream_capab_st *)p_sdp->p_data)->stream_id;
				p_mmi_pdb->stream_type = 
						((icf_stream_capab_st *)p_sdp->p_data)->stream_type;

				break;

			case ICF_CODEC_COMMIT_RESP:
            {
				p_mmi_pdb->stream_id = 
						((icf_stream_commit_st *)p_sdp->p_data)->stream_id;

				ICF_MMI_TRACE(p_mmi_pdb,ICF_TRACE_STREAM_ID)

				p_mmi_pdb->num_of_codecs = 
						((icf_stream_commit_st *)p_sdp->p_data)->codec_count;

				ICF_MMI_TRACE(p_mmi_pdb,ICF_TRACE_NUM_OF_CODECS)

				for(codec_count = 0; (codec_count < p_mmi_pdb->num_of_codecs) && (codec_count < ICF_MAX_CODECS);
										codec_count++)
				{   

					p_mmi_pdb->codec.codec_name.str_len = 
					((icf_stream_commit_st *)p_sdp->p_data)->\
					codec[codec_count].codec_name.str_len;

					icf_port_strcpy(p_mmi_pdb->codec.codec_name.str,
					((icf_stream_commit_st *)p_sdp->p_data)->\
					codec[codec_count].codec_name.str);

					if(((icf_stream_commit_st *)p_sdp->p_data)->\
					codec[codec_count].bit_mask & ICF_CODEC_ID_CODEC_ID_PRESENT)
					{
						p_mmi_pdb->codec.codec_num = 
							((icf_stream_commit_st *)p_sdp->p_data)->\
							codec[codec_count].codec_id;
						p_mmi_pdb->codec.bit_mask |= ICF_CODEC_ATTRIB_CODEC_NUM_PRESENT;

                        p_mmi_pdb->result = ICF_SUCCESS;
			            p_mmi_pdb->stream_type = 
                          icf_mmi_get_stream_type_for_stream_id(
					          p_mmi_pdb, 
                              p_mmi_pdb->stream_id);
						ICF_MMI_APP_TRACE(p_mmi_pdb,ICF_TRACE_CODEC_COMMIT)

					}

					ICF_MMI_TRACE(p_mmi_pdb,ICF_TRACE_CODEC)
				}
				codecs_traced = ICF_TRUE;
                }
				break;

			case ICF_MODIFY_MEDIA_SESSION_REQ:
				p_dest = 
				((icf_modify_stream_st *)p_sdp->p_data)->\
				sdp_stream.p_codec_info_list;

				p_mmi_pdb->stream_id = 
						((icf_modify_stream_st *)p_sdp->p_data)->stream_id;
				p_mmi_pdb->stream_type = 
						((icf_modify_stream_st *)p_sdp->p_data)->stream_type;


				icf_port_memcpy(&p_mmi_pdb->stream_addr,
						&((icf_modify_stream_st*)p_sdp->p_data)->sdp_stream.stream_address,
						(icf_uint32_t)ICF_PORT_SIZEOF(icf_transport_address_st));

				/* CSR 1-6212448: Update the remote rtp address in trace criteria data otg call */
				if(!(ICF_CALL_DIR & p_mmi_pdb->p_call_ctx->common_bitmask))
				{                
#ifdef ICF_TRACE_ENABLE
                    p_stream_data = icf_cmn_get_node_from_stream_data_list(
                            p_stream_data_list,
                            p_mmi_pdb->stream_type);

                    if(ICF_NULL == p_stream_data)
                    {
                        ICF_MEMGET (p_mmi_pdb->p_glb_pdb,
                                ICF_PORT_SIZEOF(icf_stream_data_st),
                                ICF_MEM_COMMON,
                                p_stream_data,
                                ICF_DONOT_RET_ON_FAILURE,
                                &ecode,
                                ret_val)
                            if (ICF_FAILURE == ret_val)
                            {
                                return ICF_FAILURE;
                            }

                        p_stream_data->stream_id = p_mmi_pdb->stream_id;
                        p_stream_data->stream_type = p_mmi_pdb->stream_type;

                        p_stream_data->remote_rtp_address = 
                            p_mmi_pdb->stream_addr;

                        icf_cmn_add_node_to_list(p_mmi_pdb->p_glb_pdb,
                                &(p_mmi_pdb->p_glb_pdb->p_call_ctx->p_trace_criteria_data
                                    ->p_stream_data), p_stream_data);

                        p_mmi_pdb->p_glb_pdb->p_call_ctx->p_trace_criteria_data
                            ->bitmask |= ICF_TRACE_REMOTE_RTP_IP_PRESENT;
                        p_mmi_pdb->p_glb_pdb->p_call_ctx->p_trace_criteria_data
                            ->bitmask |= ICF_TRACE_REMOTE_RTP_PORT_PRESENT;
                    }
                    else
                    {
                        p_stream_data->remote_rtp_address = 
                            p_mmi_pdb->stream_addr;

                        p_mmi_pdb->p_glb_pdb->p_call_ctx->p_trace_criteria_data
                            ->bitmask |= ICF_TRACE_REMOTE_RTP_IP_PRESENT;
                        p_mmi_pdb->p_glb_pdb->p_call_ctx->p_trace_criteria_data
                            ->bitmask |= ICF_TRACE_REMOTE_RTP_PORT_PRESENT;
                    }
                    /* Rel 8.1 : Klokworks warning resolution */
                    ICF_KLOCWORK_FALSE_POSITIVE(p_mmi_pdb->p_glb_pdb,p_stream_data)
#endif	
                }
                trace_addr = ICF_TRUE;
				if ( ICF_MODIFY_MEDIA_MODE_PRESENT &
					((icf_modify_stream_st *)p_sdp->p_data)->bit_mask)
				{
					mode_present = ICF_TRUE;
					p_mmi_pdb->mode = ((icf_modify_stream_st *)p_sdp->p_data)->mode;
				}
				break;

			case ICF_MODIFY_MEDIA_SESSION_RESP:
				p_dest = 
				((icf_resp_stream_st *)p_sdp->p_data)->\
				local_sdp.p_codec_info_list;

				p_mmi_pdb->stream_id = 
						((icf_resp_stream_st *)p_sdp->p_data)->stream_id;
				p_mmi_pdb->stream_type = 
						((icf_resp_stream_st *)p_sdp->p_data)->stream_type;

				icf_port_memcpy(&p_mmi_pdb->stream_addr,
						&((icf_resp_stream_st*)p_sdp->p_data)->local_sdp.stream_address,
						(icf_uint32_t)ICF_PORT_SIZEOF(icf_transport_address_st));

				trace_addr = ICF_TRUE;
				break;


                       /* This code is not in use. To send this trace call context
                        * is required which is not created for this API */
#if 0
			case ICF_MEDIA_CAPABILITIES_RESP:
				p_dest = 
				((icf_stream_capab_st *)p_sdp->p_data)->p_codec_info_list;

				p_mmi_pdb->stream_id = 
						((icf_stream_capab_st *)p_sdp->p_data)->stream_id;
				p_mmi_pdb->stream_type = 
						((icf_stream_capab_st *)p_sdp->p_data)->stream_type;

                p_mmi_pdb->p_codec_list = p_dest;
                p_mmi_pdb->result = ICF_SUCCESS;
                ICF_MMI_APP_TRACE(p_mmi_pdb, ICF_TRACE_MEDIA_CAPABILITY)

				break;
#endif
		}

		if(ICF_FALSE == codecs_traced)
		{
			ICF_MMI_TRACE(p_mmi_pdb, ICF_TRACE_STREAM_ID)

			ICF_MMI_TRACE(p_mmi_pdb, ICF_TRACE_STREAM_TYPE)

			if((ICF_NULL != p_dest)&&
				(ICF_STREAM_TYPE_TBCP != p_mmi_pdb->stream_type))
			{
				icf_mmi_trace_api_codecs(p_mmi_pdb, p_dest);
			}
		}

		if(ICF_TRUE == trace_addr)
		{
			ICF_MMI_TRACE(p_mmi_pdb,ICF_TRACE_ADDR)		
		}
		if (ICF_TRUE == mode_present)
		{
			ICF_PRINT(((icf_uint8_t*)"\n[MMI]: Mode set in MMI request to MM: %d",
					p_mmi_pdb->mode));
		}
		p_sdp = p_sdp->p_next;
	}

	ICF_FUNCTION_EXIT(p_mmi_pdb->p_glb_pdb)

	return ret_val;
}


/******************************************************************************
 *
 * FUNCTION:    icf_mmi_trace_api_codecs
 *
 * DESCRIPTION: 
 *****************************************************************************/

 icf_return_t	icf_mmi_trace_api_codecs(
							  icf_mmi_pdb_st			*p_mmi_pdb,
							  icf_list_st				*p_codecs)
{
	icf_return_t		ret_val = ICF_SUCCESS;

	ICF_FUNCTION_ENTER(p_mmi_pdb->p_glb_pdb)

	icf_cmn_get_list_count(p_codecs, &p_mmi_pdb->num_of_codecs);
	ICF_MMI_TRACE(p_mmi_pdb,ICF_TRACE_NUM_OF_CODECS)

	while(ICF_NULL != p_codecs)
	{
		icf_port_memcpy(
			(icf_void_t *)&(p_mmi_pdb->codec),
			(icf_void_t *)(p_codecs->p_data),
			(icf_uint32_t)ICF_PORT_SIZEOF(icf_codec_attrib_st));

		ICF_MMI_TRACE(p_mmi_pdb,ICF_TRACE_CODEC)

		p_codecs = p_codecs->p_next;
	}

	ICF_FUNCTION_EXIT(p_mmi_pdb->p_glb_pdb)

	return ret_val;
}


/******************************************************************************
 *
 * FUNCTION:        icf_mmi_trace
 * 
 * DESCRIPTION:     This function is invoked by the macro ICF_MMI_TRACE. 
 *                  It fils in the trace data on the basis of trace id and 
 *                  calls the macro ICF_TRACE to print the trace.
 *                   
 * RETURNS:         Nothing.       
 *
 ******************************************************************************/

icf_void_t icf_mmi_trace(
        IN icf_mmi_pdb_st        *p_mmi_pdb,
        IN icf_trace_id_t        trace_id)
{
    icf_return_t                 ret_val = ICF_SUCCESS;
    icf_trace_data_st            trace_data;
    /* Klocwork Fix Rel 8.2 */ 
    icf_cmn_init_trace_data_st(&trace_data);
    /* Klocwork Fix Rel 8.2 end */
    /*Fix for UMR */
    
    ICF_FUNCTION_ENTER(p_mmi_pdb->p_glb_pdb)
        
    switch(trace_id)
    {
        case ICF_EVENT_RECVD:
            trace_data.num_of_integer_values = ICF_ONE_INT_PRESENT;                      
			trace_data.int_val[0] = 
                p_mmi_pdb->p_mmi_ctx->mmi_event;
            trace_data.number_of_string_values = ICF_NO_STRING_PRESENT;
            break;
    
        case ICF_MMI_TRACE_CURRENT_STATE:
            trace_data.num_of_integer_values = ICF_ONE_INT_PRESENT;                      
			trace_data.int_val[0] = 
                p_mmi_pdb->p_mmi_ctx->mmi_state;
            trace_data.number_of_string_values = ICF_NO_STRING_PRESENT;
            break;
            
        case ICF_TRACE_API_SEND:
            trace_data.num_of_integer_values = ICF_ONE_INT_PRESENT;                      
			trace_data.number_of_string_values = ICF_NO_STRING_PRESENT;
            if(ICF_MMI_01_AWT_GCL_RESP == 
                    p_mmi_pdb->p_mmi_ctx->mmi_state)
            {
                trace_data.int_val[0] = ICF_GET_CODEC_LIST_REQ;
            }
            if(ICF_MMI_02_AWT_CMS_RESP == 
                    p_mmi_pdb->p_mmi_ctx->mmi_state)
            {
                trace_data.int_val[0] = ICF_CREATE_MEDIA_SESSION_REQ;
            }
            if(ICF_MMI_03_AWT_COMMIT_RESP == 
                   p_mmi_pdb->p_mmi_ctx->mmi_state)
            {
                trace_data.int_val[0] = ICF_CODEC_COMMIT_REQ;
            } 
            if(ICF_MMI_04_AWT_MMS_RESP == 
                   p_mmi_pdb->p_mmi_ctx->mmi_state)
            {
                trace_data.int_val[0] = ICF_MODIFY_MEDIA_SESSION_REQ;
            } 
            if(ICF_MMI_05_AWT_DMS_RESP == 
                    p_mmi_pdb->p_mmi_ctx->mmi_state)
            {
                trace_data.int_val[0] = ICF_DELETE_MEDIA_SESSION_REQ;
            }
            if(ICF_MMI_06_AWT_OPEN_CHANNEL_RESP ==
                    p_mmi_pdb->p_mmi_ctx->mmi_state)
            {
                trace_data.int_val[0] = ICF_OPEN_MEDIA_CHANNEL_REQ;
            }
            if(ICF_MMI_07_AWT_MERGE_RESP == 
                    p_mmi_pdb->p_mmi_ctx->mmi_state)
            {
                trace_data.int_val[0] = ICF_MERGE_MEDIA_SESSION_REQ;
            }
#ifdef ICF_NAT_MEDIA_TRAVERSAL
            if(ICF_MMI_10_AWT_NAT_PUBLIC_IP_PORT_RESP == 
                    p_mmi_pdb->p_mmi_ctx->mmi_state)
            {
                trace_data.int_val[0] = ICF_NAT_MEDIA_GET_PUBLIC_IP_PORT_REQ;
            }              
#endif
			break;


        case ICF_TRACE_HDR_SEND:
            trace_data.num_of_integer_values = ICF_FOUR_INT_PRESENT;  
            trace_data.number_of_string_values = ICF_NO_STRING_PRESENT;
            
            trace_data.int_val[0] = p_mmi_pdb->hdr.api_id;
            trace_data.int_val[1] = p_mmi_pdb->hdr.source_id;
            trace_data.int_val[2] = p_mmi_pdb->hdr.destination_id;
            trace_data.int_val[3] = p_mmi_pdb->hdr.call_id;
            break;



        case ICF_TRACE_GCL_DATA_SEND:
            trace_data.num_of_integer_values = ICF_ONE_INT_PRESENT;                      
			trace_data.number_of_string_values = ICF_NO_STRING_PRESENT;
            
            trace_data.int_val[0] = ((icf_get_codec_list_req_st *)
                (p_mmi_pdb->p_mmi_msg_payload))->line_id;

            break;

        case ICF_TRACE_OPEN_CHANNEL_DATA_SEND:
            trace_data.num_of_integer_values = ICF_NO_INT_PRESENT;
			trace_data.number_of_string_values = ICF_NO_STRING_PRESENT;

            break;

        case ICF_TRACE_CLOSE_CHANNEL_DATA_SEND:
            trace_data.num_of_integer_values = ICF_NO_INT_PRESENT;
			trace_data.number_of_string_values = ICF_NO_STRING_PRESENT;

            break;

        case ICF_TRACE_REL_RES_DATA_SEND:
            trace_data.num_of_integer_values = ICF_NO_INT_PRESENT;
			trace_data.number_of_string_values = ICF_NO_STRING_PRESENT;
            
            break;

        case ICF_TRACE_DMS_DATA_SEND:
            trace_data.num_of_integer_values = ICF_NO_INT_PRESENT;
			trace_data.number_of_string_values = ICF_NO_STRING_PRESENT;

            break;
        case ICF_TRACE_MMS_DATA_SEND:
            trace_data.num_of_integer_values = ICF_NO_INT_PRESENT;                      
			trace_data.number_of_string_values = ICF_NO_STRING_PRESENT;

            break;
         
        case ICF_TRACE_MMS_SDP_SEND:
            trace_data.num_of_integer_values = ICF_NO_INT_PRESENT;                      
			trace_data.number_of_string_values = ICF_NO_STRING_PRESENT;
            
            break;
            
        case ICF_TRACE_MMS_ADDR_SEND:
            trace_data.num_of_integer_values = ICF_NO_INT_PRESENT;                       
			trace_data.number_of_string_values = ICF_NO_STRING_PRESENT;
            
 			break;			
        case ICF_TRACE_COMMIT_DATA_SEND:
            trace_data.num_of_integer_values = ICF_NO_INT_PRESENT;                      
			trace_data.number_of_string_values = ICF_NO_STRING_PRESENT;
            
            break;
            
        case ICF_TRACE_CMS_DATA_SEND:
            trace_data.num_of_integer_values = ICF_ONE_INT_PRESENT;                     
			trace_data.number_of_string_values = ICF_NO_STRING_PRESENT;

			trace_data.int_val[0] = ((icf_create_media_session_req_st *)
                (p_mmi_pdb->p_mmi_msg_payload))->line_id;
            
            break;
            
        case ICF_TRACE_CMS_SDP_SEND:
            trace_data.num_of_integer_values = ICF_NO_INT_PRESENT;                    
			trace_data.number_of_string_values = ICF_NO_STRING_PRESENT;
            
             break;
            
        case ICF_TRACE_CMS_ADDR_SEND:
            trace_data.num_of_integer_values = ICF_NO_INT_PRESENT;                       
			trace_data.number_of_string_values = ICF_NO_STRING_PRESENT;
            
            break;

        case ICF_TRACE_MERGE_DATA_SEND:
            trace_data.num_of_integer_values = ICF_ONE_INT_PRESENT;                      
			trace_data.number_of_string_values = ICF_NO_STRING_PRESENT;
            
            trace_data.int_val[0] = ((icf_merge_media_session_req_st *)
                (p_mmi_pdb->p_mmi_msg_payload))->merge_call_id;
            break;




		case ICF_TRACE_STREAM_ID:
            trace_data.num_of_integer_values = ICF_ONE_INT_PRESENT;                      
			trace_data.number_of_string_values = ICF_NO_STRING_PRESENT;
        
            trace_data.int_val[0] = p_mmi_pdb->stream_id;

            break;

		case ICF_TRACE_STREAM_TYPE:
            trace_data.num_of_integer_values = ICF_NO_INT_PRESENT;                      
			trace_data.number_of_string_values = ICF_ONE_STRING_PRESENT;
        
			switch(p_mmi_pdb->stream_type)
			{
				case ICF_STREAM_TYPE_AUDIO:
                
					icf_port_strcpy(trace_data.string_val[0].str,
									(icf_uint8_t *)"AUDIO");
					break;
				
				case ICF_STREAM_TYPE_VIDEO:
					icf_port_strcpy(trace_data.string_val[0].str,
									(icf_uint8_t *)"VIDEO");
					break;

				case ICF_STREAM_TYPE_TBCP:
					icf_port_strcpy(trace_data.string_val[0].str,
									(icf_uint8_t *)"TBCP");
					break;

				case ICF_STREAM_TYPE_FAX:
					icf_port_strcpy(trace_data.string_val[0].str,
									(icf_uint8_t *)"FAX");
					break;

				case ICF_STREAM_TYPE_INVALID:
					icf_port_strcpy(trace_data.string_val[0].str,
									(icf_uint8_t *)"INVALID");
					break;
			}
            break;
            
        case ICF_TRACE_NUM_OF_CODECS:
            trace_data.num_of_integer_values = ICF_ONE_INT_PRESENT;                      
			trace_data.number_of_string_values = ICF_NO_STRING_PRESENT;
             
            trace_data.int_val[0] = p_mmi_pdb->num_of_codecs;
            break;
            
        case ICF_TRACE_CODEC:
			if(ICF_NULL == p_mmi_pdb->codec.codec_name.str_len)
			{
				trace_data.number_of_string_values = ICF_NO_STRING_PRESENT;
			}
			else
			{
				trace_data.number_of_string_values = ICF_ONE_STRING_PRESENT;
				trace_data.string_val[0].str_len = 
										p_mmi_pdb->codec.codec_name.str_len;
				icf_port_strcpy(trace_data.string_val[0].str,
										p_mmi_pdb->codec.codec_name.str);
			}

			if(p_mmi_pdb->codec.bit_mask & ICF_CODEC_ATTRIB_CODEC_NUM_PRESENT)
			{
				trace_data.num_of_integer_values = ICF_ONE_INT_PRESENT;			
				trace_data.int_val[0] = p_mmi_pdb->codec.codec_num;
			}
			else
			{
				trace_data.num_of_integer_values = ICF_NO_INT_PRESENT;
			}
            break;

        case ICF_TRACE_ADDR:

			switch(p_mmi_pdb->stream_addr.addr.addr_type)
			{
				case ICF_TRANSPORT_ADDRESS_IPV4_ADDR:
					trace_data.num_of_integer_values = ICF_FOUR_INT_PRESENT;
					trace_data.int_val[0] = 
						p_mmi_pdb->stream_addr.addr.addr.ipv4_addr.octet_1;
					trace_data.int_val[1] = 
						p_mmi_pdb->stream_addr.addr.addr.ipv4_addr.octet_2;
					trace_data.int_val[2] = 
						p_mmi_pdb->stream_addr.addr.addr.ipv4_addr.octet_3;
					trace_data.int_val[3] = 
						p_mmi_pdb->stream_addr.addr.addr.ipv4_addr.octet_4;
				   /*
					trace_data.int_val[4] = 
						p_mmi_pdb->stream_addr.port_num;
				    */
					break;

				default:
					trace_data.num_of_integer_values = ICF_NO_INT_PRESENT;
					break;
			}
			trace_data.number_of_string_values = ICF_NO_STRING_PRESENT;
            
            break;       



        case ICF_TRACE_GCL_DATA_RECVD:
            trace_data.num_of_integer_values = ICF_TWO_INT_PRESENT;                      
			trace_data.number_of_string_values = ICF_NO_STRING_PRESENT;
            
            trace_data.int_val[0] = ((icf_get_codec_list_resp_st *)
                (p_mmi_pdb->p_mmi_msg_payload))->result;
            
            trace_data.int_val[1] = ((icf_get_codec_list_resp_st *)
                (p_mmi_pdb->p_mmi_msg_payload))->bit_mask;
            
            break;
            
        case ICF_TRACE_OCR_DATA_RECVD:
            trace_data.num_of_integer_values = ICF_TWO_INT_PRESENT;                      
			trace_data.number_of_string_values = ICF_NO_STRING_PRESENT;
            
            trace_data.int_val[0] = ((icf_open_media_channel_resp_st *)
                (p_mmi_pdb->p_mmi_msg_payload))->result;
            trace_data.int_val[1] = ((icf_open_media_channel_resp_st *)
                (p_mmi_pdb->p_mmi_msg_payload))->error_cause;
           
            break;

        case ICF_TRACE_CMS_DATA_RECVD:
            trace_data.num_of_integer_values = ICF_TWO_INT_PRESENT;                      
			trace_data.number_of_string_values = ICF_NO_STRING_PRESENT;
            
            trace_data.int_val[0] = ((icf_create_media_session_resp_st *)
                (p_mmi_pdb->p_mmi_msg_payload))->result;
            
            trace_data.int_val[1] = ((icf_create_media_session_resp_st *)
                (p_mmi_pdb->p_mmi_msg_payload))->bit_mask;
            
            break;

        case ICF_TRACE_MMS_DATA_RECVD:
            trace_data.num_of_integer_values = ICF_TWO_INT_PRESENT;                      
			trace_data.number_of_string_values = ICF_NO_STRING_PRESENT;
            
            trace_data.int_val[0] = ((icf_modify_media_session_resp_st *)
                (p_mmi_pdb->p_mmi_msg_payload))->result;
            
            trace_data.int_val[1] = ((icf_modify_media_session_resp_st *)
                (p_mmi_pdb->p_mmi_msg_payload))->bit_mask;
            
            break;

        case ICF_TRACE_CMS_SDP_RECVD:
            trace_data.num_of_integer_values = ICF_NO_INT_PRESENT;                    
			trace_data.number_of_string_values = ICF_NO_STRING_PRESENT;
            
            break;
            




        case ICF_TRACE_NUM_OF_LOCAL_CODECS:
            trace_data.num_of_integer_values = ICF_NO_INT_PRESENT;                      
			trace_data.number_of_string_values = ICF_NO_STRING_PRESENT;
            
            break;


        case ICF_TRACE_MED_CAP_REQ:
            trace_data.num_of_integer_values = ICF_NO_INT_PRESENT;  
            trace_data.number_of_string_values = ICF_NO_STRING_PRESENT;
            break;
            
        case ICF_TRACE_MED_CAP_RESP:
            trace_data.num_of_integer_values = ICF_NO_INT_PRESENT;  
            trace_data.number_of_string_values = ICF_NO_STRING_PRESENT;
            break;
            
        default:
            trace_data.num_of_integer_values = ICF_NO_INT_PRESENT;                       
			trace_data.number_of_string_values = ICF_NO_STRING_PRESENT;
            
    }/* End switch */
    
    ICF_TRACE(p_mmi_pdb->p_glb_pdb,trace_id,&trace_data)
        
    ICF_FUNCTION_EXIT(p_mmi_pdb->p_glb_pdb)        

}/* End function */ 

/******************************************************************************
 *
 * FUNCTION:        icf_mmi_app_trace
 *
 * DESCRIPTION:     This function is invoked by the macro ICF_MMI_TRACE.
 *                  It fils in the trace data on the basis of trace id and
 *                  calls the macro ICF_TRACE to print the trace.
 *
 * RETURNS:         Nothing.
 *
 ******************************************************************************/

icf_return_t icf_mmi_app_trace(
        IN icf_mmi_pdb_st        *p_mmi_pdb,
        IN icf_trace_id_t        trace_id)
{
    icf_return_t                 ret_val = ICF_SUCCESS;
    icf_trace_data_st            *p_trace_data = ICF_NULL;
    /*Fix for CSR-1-5249829 */
    ret_val = icf_cmn_check_app_trace(p_mmi_pdb->p_glb_pdb,
                                        trace_id,
					p_mmi_pdb->p_glb_pdb->p_call_ctx->p_trace_criteria_data);
    if( ICF_FAILURE == ret_val)
    {
        return ret_val;
    }
    else
    {

    ICF_MEMGET (p_mmi_pdb->p_glb_pdb,
            ICF_PORT_SIZEOF(icf_trace_data_st),
            ICF_MEM_COMMON,
            p_trace_data,
            ICF_RET_ON_FAILURE,
             p_mmi_pdb->p_ecode,
             ret_val)
    
    p_mmi_pdb->p_glb_pdb->p_glb_data->app_trace_info.app_id =
                p_mmi_pdb->p_call_ctx->app_id;
    p_mmi_pdb->p_glb_pdb->p_glb_data->app_trace_info.call_id =
                p_mmi_pdb->p_call_ctx->call_id;

    ICF_FUNCTION_ENTER(p_mmi_pdb->p_glb_pdb)	

	switch(trace_id)
    {
        /* This code is not in use. To send this trace call context
         * is required which is not created in case of this API */
#if 0
	case ICF_TRACE_MEDIA_CAPABILITY:
        {
            p_trace_data->num_of_integer_values = ICF_TWO_INT_PRESENT;
            p_trace_data->number_of_string_values = ICF_NO_STRING_PRESENT;

            p_trace_data->int_val[0] = p_mmi_pdb->stream_type;
            p_trace_data->int_val[1] = p_mmi_pdb->result;

            if( ICF_SUCCESS == p_mmi_pdb->result)
            {
                p_trace_data->p_var_data = (icf_list_st *)p_mmi_pdb->p_codec_list;
            }
            break;
        }
#endif

	case ICF_TRACE_CODEC_RESERVE:
		{
            icf_stream_data_st *p_stream_data = ICF_NULL;
            icf_list_st *p_stream_data_list = 
                 p_mmi_pdb->p_glb_pdb->p_call_ctx->p_trace_criteria_data
                         ->p_stream_data;
            icf_error_t ecode; 

			p_trace_data->num_of_integer_values = ICF_TWO_INT_PRESENT;
        	p_trace_data->number_of_string_values = ICF_NO_STRING_PRESENT;

            p_trace_data->int_val[0] = p_mmi_pdb->stream_type;
            p_trace_data->int_val[1] = p_mmi_pdb->result;

			if( ICF_SUCCESS == p_mmi_pdb->result)
			{
                p_trace_data->p_var_data = (icf_list_st *)p_mmi_pdb->p_codec_list;

                p_stream_data = 
                   icf_cmn_get_node_from_stream_data_list(p_stream_data_list,
                                     p_mmi_pdb->stream_type);

            if(ICF_NULL == p_stream_data)
            {
                 ICF_MEMGET (p_mmi_pdb->p_glb_pdb,
                    ICF_PORT_SIZEOF(icf_stream_data_st),
                    ICF_MEM_COMMON,
                    p_stream_data,
                    ICF_DONOT_RET_ON_FAILURE,
                    &ecode,
                    ret_val)

                if (ICF_FAILURE == ret_val)
                {
	           ICF_MEMFREE(p_mmi_pdb->p_glb_pdb,
		        p_trace_data,ICF_MEM_COMMON,
		        p_mmi_pdb->p_ecode,ret_val)
                   return ICF_FAILURE;
                }
                p_stream_data->stream_id = p_mmi_pdb->stream_id;
                p_stream_data->stream_type = p_mmi_pdb->stream_type;

                icf_cmn_clone_codec_attrib_list(p_mmi_pdb->p_glb_pdb,
                        &(p_stream_data->p_offered_codec),
                        p_mmi_pdb->p_codec_list);
               
                icf_cmn_add_node_to_list(p_mmi_pdb->p_glb_pdb,
                    &(p_mmi_pdb->p_glb_pdb->p_call_ctx->p_trace_criteria_data
                       ->p_stream_data), p_stream_data);

                p_mmi_pdb->p_glb_pdb->p_call_ctx->p_trace_criteria_data
                   ->bitmask |= ICF_TRACE_OFFERED_CODEC_PRESENT;
            }
            else
            {
                icf_cmn_clone_codec_attrib_list(p_mmi_pdb->p_glb_pdb,
                        &(p_stream_data->p_offered_codec),
                        p_mmi_pdb->p_codec_list);

                p_mmi_pdb->p_glb_pdb->p_call_ctx->p_trace_criteria_data
                   ->bitmask |= ICF_TRACE_OFFERED_CODEC_PRESENT;
            }
			}
			break;
		}

	case ICF_TRACE_RTP_ADDR_ALLOCATE:
        {
            icf_stream_data_st *p_stream_data = ICF_NULL;
            icf_list_st *p_stream_data_list = 
                 p_mmi_pdb->p_glb_pdb->p_call_ctx->p_trace_criteria_data
                         ->p_stream_data;
            icf_error_t ecode;
 
            p_trace_data->num_of_integer_values = ICF_TWO_INT_PRESENT;
            p_trace_data->number_of_string_values = ICF_ONE_STRING_PRESENT;

            p_trace_data->int_val[0] = p_mmi_pdb->stream_type;
            p_trace_data->int_val[1] = p_mmi_pdb->result;

            if( ICF_SUCCESS == p_mmi_pdb->result)
            {
                icf_uint8_t  port[10];

                icf_cmn_convert_transport_addr_to_ip_string
                   (&(p_mmi_pdb->stream_addr.addr),
                    p_trace_data->string_val[0].str);

                icf_port_snprintf(port,ICF_PORT_SIZEOF(port),(const icf_uint8_t*)":%d",
                        p_mmi_pdb->stream_addr.port_num);

                icf_port_strcat(p_trace_data->string_val[0].str,port);
                p_trace_data->string_val[0].str_len =
                     (icf_uint16_t)icf_port_strlen(p_trace_data->
                                                   string_val[0].str);
            

            p_stream_data = icf_cmn_get_node_from_stream_data_list(
                                     p_stream_data_list,
                                     p_mmi_pdb->stream_type);

            if(ICF_NULL == p_stream_data)
            {
                 ICF_MEMGET (p_mmi_pdb->p_glb_pdb,
                    ICF_PORT_SIZEOF(icf_stream_data_st),
                    ICF_MEM_COMMON,
                    p_stream_data,
                    ICF_DONOT_RET_ON_FAILURE,
                    &ecode,
                    ret_val)
                if (ICF_FAILURE == ret_val)
                {
	           ICF_MEMFREE(p_mmi_pdb->p_glb_pdb,
		        p_trace_data,ICF_MEM_COMMON,
		        p_mmi_pdb->p_ecode,ret_val)
                   return ICF_FAILURE;
                }

                p_stream_data->stream_id = p_mmi_pdb->stream_id;
                p_stream_data->stream_type = p_mmi_pdb->stream_type;
                
                p_stream_data->local_rtp_address = 
                        p_mmi_pdb->stream_addr;

                icf_cmn_add_node_to_list(p_mmi_pdb->p_glb_pdb,
                    &(p_mmi_pdb->p_glb_pdb->p_call_ctx->p_trace_criteria_data
                       ->p_stream_data), p_stream_data);

                p_mmi_pdb->p_glb_pdb->p_call_ctx->p_trace_criteria_data
                   ->bitmask |= ICF_TRACE_LOCAL_RTP_IP_PRESENT;
                p_mmi_pdb->p_glb_pdb->p_call_ctx->p_trace_criteria_data
                   ->bitmask |= ICF_TRACE_LOCAL_RTP_PORT_PRESENT;
            }
            else
            {
                p_stream_data->local_rtp_address = 
                        p_mmi_pdb->stream_addr;

                p_mmi_pdb->p_glb_pdb->p_call_ctx->p_trace_criteria_data
                   ->bitmask |= ICF_TRACE_LOCAL_RTP_IP_PRESENT;
                p_mmi_pdb->p_glb_pdb->p_call_ctx->p_trace_criteria_data
                   ->bitmask |= ICF_TRACE_LOCAL_RTP_PORT_PRESENT;
            }
            }

            break;
        }

	case ICF_TRACE_CODEC_COMMIT:
		{
            icf_stream_data_st *p_stream_data = ICF_NULL;
            icf_list_st *p_stream_data_list = 
                 p_mmi_pdb->p_glb_pdb->p_call_ctx->p_trace_criteria_data
                         ->p_stream_data;
            icf_error_t ecode; 
            icf_codec_attrib_st *p_codec_commit = ICF_NULL;

             /* allocate memory for the code_attrib_st- payload */
            ICF_MEMGET (p_mmi_pdb->p_glb_pdb,
                    ICF_PORT_SIZEOF(icf_codec_attrib_st),
                    ICF_MEM_COMMON,
                    p_codec_commit,
                    ICF_DONOT_RET_ON_FAILURE,
                    &ecode,
                    ret_val)
                if (ICF_FAILURE == ret_val || ICF_NULL == p_codec_commit)
                {
	           ICF_MEMFREE(p_mmi_pdb->p_glb_pdb,
		        p_trace_data,ICF_MEM_COMMON,
		        p_mmi_pdb->p_ecode,ret_val)
                   return ICF_FAILURE;
                }

            p_codec_commit->bit_mask = p_mmi_pdb->codec.bit_mask;
            p_codec_commit->codec_num = p_mmi_pdb->codec.codec_num;

            icf_port_strcpy(p_codec_commit->codec_name.str,
                  p_mmi_pdb->codec.codec_name.str);

            p_codec_commit->codec_name.str_len =
                    p_mmi_pdb->codec.codec_name.str_len;


			p_trace_data->num_of_integer_values = ICF_TWO_INT_PRESENT;
        	p_trace_data->number_of_string_values = ICF_NO_STRING_PRESENT;

            p_trace_data->int_val[0] = p_mmi_pdb->stream_type;
            p_trace_data->int_val[1] = p_mmi_pdb->result;

			if( ICF_SUCCESS == p_mmi_pdb->result)
			{
                p_trace_data->p_var_data = (icf_void_t *)&(p_mmi_pdb->codec);


            p_stream_data = 
                  icf_cmn_get_node_from_stream_data_list(p_stream_data_list,
                                     p_mmi_pdb->stream_type);

            if(ICF_NULL == p_stream_data)
            {
                 ICF_MEMGET (p_mmi_pdb->p_glb_pdb,
                    ICF_PORT_SIZEOF(icf_stream_data_st),
                    ICF_MEM_COMMON,
                    p_stream_data,
                    ICF_DONOT_RET_ON_FAILURE,
                    &ecode,
                    ret_val)
                if (ICF_FAILURE == ret_val)
                {
	           ICF_MEMFREE(p_mmi_pdb->p_glb_pdb,
		        p_codec_commit,ICF_MEM_COMMON,
		        p_mmi_pdb->p_ecode,ret_val)

	           ICF_MEMFREE(p_mmi_pdb->p_glb_pdb,
		        p_trace_data,ICF_MEM_COMMON,
		        p_mmi_pdb->p_ecode,ret_val)
                   return ICF_FAILURE;
                }

                p_stream_data->stream_id = p_mmi_pdb->stream_id;
                p_stream_data->stream_type = p_mmi_pdb->stream_type;

                p_stream_data->p_negotiated_codec = p_codec_commit;

                icf_cmn_add_node_to_list(p_mmi_pdb->p_glb_pdb,
                    &(p_mmi_pdb->p_glb_pdb->p_call_ctx->p_trace_criteria_data
                       ->p_stream_data), p_stream_data);

                p_mmi_pdb->p_glb_pdb->p_call_ctx->p_trace_criteria_data
                   ->bitmask |= ICF_TRACE_NEGOTIATED_CODEC_PRESENT;
            }
            else
            {
                if(ICF_NULL != p_stream_data->p_negotiated_codec)
                {
				    ICF_MEMFREE(p_mmi_pdb->p_glb_pdb,
				       p_stream_data->p_negotiated_codec,
                       ICF_MEM_COMMON,
				       p_mmi_pdb->p_ecode,ret_val)
                }
    
               p_stream_data->p_negotiated_codec = p_codec_commit;

                p_mmi_pdb->p_glb_pdb->p_call_ctx->p_trace_criteria_data
                   ->bitmask |= ICF_TRACE_NEGOTIATED_CODEC_PRESENT;
            }
			}
            ICF_KLOCWORK_FALSE_POSITIVE(p_mmi_pdb->p_glb_pdb,p_codec_commit)
			break;
		}
	
    case ICF_TRACE_STREAM_DELETE:
	case ICF_TRACE_RTP_ADDR_DEALLOCATE:
	case ICF_TRACE_CODEC_RELEASE:
	        p_trace_data->num_of_integer_values = ICF_TWO_INT_PRESENT;
            p_trace_data->int_val[0] = p_mmi_pdb->stream_type;
            p_trace_data->int_val[1] = ICF_TRUE; 
       	    p_trace_data->number_of_string_values = ICF_NO_STRING_PRESENT;
		break;
	
	case ICF_TRACE_STREAM_MERGE:
                if(ICF_SUCCESS == p_mmi_pdb->result)
                {
                    p_trace_data->num_of_integer_values = ICF_TWO_INT_PRESENT;
                    p_trace_data->int_val[0] = p_mmi_pdb->result;
                    /* If result is success then
                        only print merge call id */
                    if(ICF_NULL != p_mmi_pdb->p_mmi_ctx) 
                    {
                        p_trace_data->int_val[1] = 
                               p_mmi_pdb->p_mmi_ctx->merge_call_id; 
                    }
                }
                else
                {
                    p_trace_data->num_of_integer_values = ICF_ONE_INT_PRESENT;
                    p_trace_data->int_val[0] = p_mmi_pdb->result;
                }
       	        p_trace_data->number_of_string_values = ICF_NO_STRING_PRESENT;
		break;

	case ICF_TRACE_DMS_IND:
			p_trace_data->num_of_integer_values = ICF_NO_INT_PRESENT;
        	p_trace_data->number_of_string_values = ICF_NO_STRING_PRESENT;
		break;

        case ICF_TRACE_SRTP_NEG_FAILURE:
                    p_trace_data->num_of_integer_values = ICF_NO_INT_PRESENT;
        	p_trace_data->number_of_string_values = ICF_NO_STRING_PRESENT;
		break;

	default:
		break;
	}

 	ICF_APP_TRACE(p_mmi_pdb->p_glb_pdb,trace_id,p_trace_data,
         p_mmi_pdb->p_glb_pdb->p_call_ctx->p_trace_criteria_data)

	ICF_MEMFREE(p_mmi_pdb->p_glb_pdb,
		p_trace_data,ICF_MEM_COMMON,
		p_mmi_pdb->p_ecode,ret_val)

    ICF_FUNCTION_EXIT(p_mmi_pdb->p_glb_pdb)
    return ret_val;
}
}/* End function */

#endif


#ifdef ICF_ERROR_ENABLE
/******************************************************************************
 *
 * FUNCTION:        icf_mmi_ecode
 * 
 * DESCRIPTION:     This function is invoked by the macro ICF_MMI_SET_ECODE. 
 *                  It fills in the error data on the basis of ecode id and 
 *                  calls the macro ICF_SET_ECODE to report the error.
 *                   
 * RETURNS:         Nothing.       
 *
 ******************************************************************************/

icf_void_t icf_mmi_ecode(
        IN icf_mmi_pdb_st            *p_mmi_pdb,
        IN icf_error_id_t            ecode)
{
    icf_error_data_st                error_data;
    icf_error_t                      error=ICF_NULL;
    icf_return_t                     ret_val = ICF_SUCCESS;
    icf_error_id_t                   error_code;

    error=(icf_uint16_t) ecode;
    ICF_FUNCTION_ENTER(p_mmi_pdb->p_glb_pdb)

    error_code = ecode;
    error_data.num_of_integer_values = ICF_NO_INT_PRESENT;
    error_data.number_of_string_values = ICF_NO_STRING_PRESENT;

    ICF_SET_ECODE(p_mmi_pdb->p_glb_pdb,&error_data,error_code,
            p_mmi_pdb->p_ecode)

	ret_val = ICF_SUCCESS;   
	
    ICF_FUNCTION_EXIT(p_mmi_pdb->p_glb_pdb)        
}   
#endif


#ifndef ICF_MM_REQUEST_BUFFERING
/******************************************************************************
 *
 * FUNCTION:        icf_mmi_validate_media_session_req
 * 
 * DESCRIPTION:     This function verifies the mmi state machine for media
 *					session request
 *
 *****************************************************************************/
icf_return_t	icf_mmi_validate_media_session_req(
		INOUT	icf_mmi_pdb_st		*p_mmi_pdb)
{
	icf_return_t				ret_val = ICF_FAILURE;
        /* To Remove warning */
        p_mmi_pdb=p_mmi_pdb;
#ifdef ICF_REL_OLD
	icf_media_session_req_st	*p_media_session_req = ICF_NULL;
	
    ICF_FUNCTION_ENTER(p_mmi_pdb->p_glb_pdb)

	call_id = (p_mmi_pdb->p_glb_pdb->p_recv_msg->hdr.call_id);

	icf_dbm_find_call_cntxt_blk(
            p_mmi_pdb->p_glb_pdb,
            p_mmi_pdb->p_glb_pdb->p_recv_msg->hdr.call_id,
            &(p_call_ctx),
            p_mmi_pdb->p_ecode);
    if(ICF_INVALID_CALL_ID != call_id)
	{
		if( (ICF_NULL != p_call_ctx) 
			&& (ICF_NULL != p_call_ctx)
			&& (ICF_MMI_04_AWT_MMS_RESP == 
				p_call_ctx->mmi_context.mmi_state))
        {
		    msg_len = ICF_PORT_SIZEOF(icf_api_header_st) + 
					ICF_PORT_SIZEOF(icf_media_session_req_st);

			if ( ICF_NULL != p_call_ctx->mmi_context.p_media_session_req)
			{
				ICF_MEMFREE(p_mmi_pdb->p_glb_pdb,
				p_call_ctx->mmi_context.p_media_session_req,ICF_MEM_COMMON,
				p_mmi_pdb->p_ecode,ret_val)
				p_call_ctx->mmi_context.p_media_session_req = ICF_NULL;
			}
			ICF_MEMGET(p_mmi_pdb->p_glb_pdb,
				msg_len,
				ICF_MEM_COMMON,
				p_call_ctx->mmi_context.p_media_session_req,
            ICF_RET_ON_FAILURE,p_mmi_pdb->p_ecode,ret_val)
			
			p_msg = p_call_ctx->mmi_context.p_media_session_req;
			p_recv_msg = p_mmi_pdb->p_glb_pdb->p_recv_msg;
			p_media_session_req = (icf_media_session_req_st*)(p_msg->payload);

		    p_msg->hdr.source_id       = p_recv_msg->hdr.source_id;
			p_msg->hdr.destination_id  = p_recv_msg->hdr.destination_id;
		    p_msg->hdr.api_id          = ICF_MEDIA_SESSION_REQ;
		    p_msg->hdr.api_length	   = p_recv_msg->hdr.api_length;
			p_msg->hdr.call_id         = p_recv_msg->hdr.call_id;
		    p_msg->hdr.version         = p_recv_msg->hdr.version;
			p_msg->hdr.app_id          = p_recv_msg->hdr.app_id;
			p_media_session_req->connect_speaker_flg = 
				((icf_media_session_req_st*)(p_recv_msg->payload))->connect_speaker_flg;
			p_media_session_req->bit_mask =
				((icf_media_session_req_st*)(p_recv_msg->payload))->bit_mask;
			p_media_session_req->stream_id = 
				((icf_media_session_req_st*)(p_recv_msg->payload))->stream_id;
			ret_val = ICF_FAILURE;
        }
		else
		{
			if ( ICF_NULL != p_call_ctx)
				p_call_ctx->mmi_context.p_media_session_req = ICF_NULL;
			ret_val = ICF_SUCCESS;
		}
	}
	else
	{
		ret_val = ICF_SUCCESS;
	}
	ICF_FUNCTION_EXIT(p_mmi_pdb->p_glb_pdb)
#endif
	return ret_val;
}
#endif


ICF_MM_ROUTING_START
/******************************************************************************
 *
 * FUNCTION:        icf_mmi_proc_icf_imsc_msg
 * 
 * DESCRIPTION:     This function is invoked when an application wants to send
 *					a request to Media Manager and ICF routes it by doing 
 *					api_id conversion.
 *
 *****************************************************************************/
icf_return_t	icf_mmi_proc_icf_imsc_msg(
		INOUT	icf_mmi_pdb_st		*p_mmi_pdb)
{
	icf_return_t						ret_val = ICF_SUCCESS;
	icf_api_id_t						*p_api_id = ICF_NULL;

    ICF_FUNCTION_ENTER(p_mmi_pdb->p_glb_pdb)

	ICF_ASSERT(p_mmi_pdb->p_glb_pdb, p_mmi_pdb->p_glb_pdb->p_recv_msg)

	p_api_id = &(p_mmi_pdb->p_glb_pdb->p_recv_msg->hdr.api_id);

#ifndef ICF_MM_REQUEST_BUFFERING

    /* Buffer media_session_req if response from MM is pending. This is needed
     * to handle a race condition in incoming call when App sends connect_spkr
     * request before MM sends mm_resp. In that case, ICF will buffer 
	 * connect_spkr
     * request and send it to MM after getting mms_resp */
    /* This is not needed if ICF_MM_REQUEST_BUFFERING as all requests are 
	 * buffered in that case */
    if (ICF_MEDIA_SESSION_REQ == *p_api_id)
    {
        if (ICF_FAILURE == icf_mmi_validate_media_session_req(p_mmi_pdb))
        {
            ICF_PRINT(((icf_uint8_t*)"\nApp to MM request buffered in ICF\n"));
            /* If request is bufferred, this func will return failure */
            ret_val = ICF_SUCCESS;
            ICF_FUNCTION_EXIT(p_mmi_pdb->p_glb_pdb);
            return ret_val;
        }
    }
#endif

	/* now simply forward the message to Media Manager */
	if (ICF_SUCCESS == ret_val)
	{
        ret_val = icf_mmi_send(p_mmi_pdb,p_mmi_pdb->p_glb_pdb->p_recv_msg,
						p_mmi_pdb->p_ecode);

        if(ICF_SEND_UDP_DATA_REQ == *p_api_id)
        {
            icf_send_udp_data_req_st    *p_send_udp_data_req =
                (icf_send_udp_data_req_st *)
                    p_mmi_pdb->p_glb_pdb->p_recv_msg->payload;

            icf_port_static_memfree(p_send_udp_data_req->p_app_data,
                    p_mmi_pdb->p_ecode);
        }
		/* since MMI is not making any new message, it is upto MRM/ES to free
		 * the received external message */
	}
	ICF_FUNCTION_EXIT(p_mmi_pdb->p_glb_pdb)
	return ret_val;
} /* End icf_mmi_proc_icf_imsc_msg() */
ICF_MM_ROUTING_END


ICF_MM_ROUTING_START
/******************************************************************************
 *
 * FUNCTION:        icf_mmi_proc_imsc_icf_msg
 * 
 * DESCRIPTION:     This function is invoked when Media Manager sends response
 *					or indication to an application and ICF routes it by doing 
 *					api_id and error_cause conversion.
 *
 *****************************************************************************/
icf_return_t	icf_mmi_proc_imsc_icf_msg(
		INOUT	icf_mmi_pdb_st		*p_mmi_pdb)
{
	icf_return_t						ret_val = ICF_SUCCESS;
	icf_api_id_t						*p_api_id = ICF_NULL;
	icf_call_id_t						call_id = ICF_INVALID_CALL_ID;
	icf_call_ctx_st					*p_call_ctx=ICF_NULL;

    ICF_FUNCTION_ENTER(p_mmi_pdb->p_glb_pdb)

	ICF_ASSERT(p_mmi_pdb->p_glb_pdb, p_mmi_pdb->p_glb_pdb->p_recv_msg)
	
	p_api_id = &(p_mmi_pdb->p_glb_pdb->p_recv_msg->hdr.api_id);

	call_id = (p_mmi_pdb->p_glb_pdb->p_recv_msg->hdr.call_id);

    if(ICF_INVALID_CALL_ID != call_id)
    {
        if(ICF_FAILURE == icf_dbm_find_call_cntxt_blk(
            p_mmi_pdb->p_glb_pdb,
            p_mmi_pdb->p_glb_pdb->p_recv_msg->hdr.call_id,
            &(p_call_ctx),
            p_mmi_pdb->p_ecode))
        {
            ICF_ALARM(((icf_uint8_t *)"CallID %d received for which no call context exists",call_id));
            ret_val = ICF_FAILURE;
        }
        else
        {
            if(p_call_ctx != ICF_NULL)
            {
                p_mmi_pdb->p_glb_pdb->p_recv_msg->hdr.app_id 
                = p_call_ctx->app_id;
            
            }
        }
    }

    /* this part is not required anymore as ICF and MM interfaces
	have been alligned */
	/* convert ICF api_id into ICF api_id in the message header */
	switch(*p_api_id)
	{

		case ICF_CFG_CODEC_INFO_RESP:
#ifdef FSS_EMUZED_MEDIA_MANAGER
            ret_val = icf_send_port_cfg_req_to_mm(p_mmi_pdb);				
#else
            ret_val = ICF_SUCCESS;
#endif
            break;

        case ICF_CFG_JBM_MGMT_RESP:
        case ICF_CFG_MAX_EARP_VOL_RESP:
        case ICF_CFG_MAX_SPEAKER_VOL_RESP:
        case ICF_SET_MIC_GAIN_RESP:
        case ICF_CFG_MAX_MIC_GAIN_RESP:
        case ICF_CFG_SET_SELF_IP_RESP:
		case ICF_CFG_SET_REMOTE_VIDEO_DISPLAY_RESP:
		case ICF_CFG_SET_PREVIEW_VIDEO_DISPLAY_RESP:
        case ICF_START_RINGING_RESP:
        case ICF_MEDIA_SESSION_RESP:
		case ICF_VIBRATION_RESP:
        case ICF_ECHO_CANC_RESP:
        case ICF_VAD_SS_CNG_RESP:
        case ICF_PLAY_MEDIA_RESP:
		case ICF_STOP_MEDIA_PLAY_RESP:
		case ICF_STOP_MEDIA_PLAY_IND:
		case ICF_SET_MEDIA_PLAYBACK_DEVICE_RESP:
		case ICF_STOP_RINGING_RESP:
		case ICF_STOP_VIBRATION_RESP:
        case ICF_STOP_TONE_IND:
        case ICF_STOP_RINGING_IND:
		case ICF_CONNECT_MEDIA_DEVICE_RESP:
		case ICF_MEDIA_RECORD_RESP:
		case ICF_SEND_DGTS_RESP:
		case ICF_SET_EARP_VOLUME_RESP:
		case ICF_SET_SPEAKER_VOLUME_RESP:
		case ICF_STOP_MEDIA_RECORD_RESP:
        case ICF_SEND_APP_DATA_RESP:
        case ICF_RCV_APP_DATA_IND:
        case ICF_RTP_START_IND:
        case ICF_SEND_STATS_RESP:
        case ICF_STOP_STATS_RESP:
		case ICF_PAUSE_MEDIA_RESP:
        case ICF_GET_STATS_RESP:
        case ICF_RECV_UDP_DATA_IND:
			{
            	ret_val = ICF_SUCCESS;
			}
		    break;
			
		case ICF_CFG_SET_PORT_RANGE_RESP:
#ifdef FSS_EMUZED_MEDIA_MANAGER					
			ret_val = icf_send_ringer_tones_req_to_mm(p_mmi_pdb);
#else
			ret_val = ICF_SUCCESS;
#endif
			break;

		case ICF_MM_INIT_COMPLETE:
#ifdef FSS_EMUZED_MEDIA_MANAGER					
			ret_val = icf_mmi_handle_mm_init_comp_ind(p_mmi_pdb);
#else
			ret_val = ICF_SUCCESS;
#endif
			break;

		case ICF_CFG_RINGER_TONES_RESP:
#ifdef FSS_EMUZED_MEDIA_MANAGER					
			ret_val = icf_mmi_set_init_complete(p_mmi_pdb);
#else
			ret_val = ICF_SUCCESS;
#endif			
			break;	
			
		default:
			ICF_MMI_SET_ECODE(p_mmi_pdb, ICF_MM_CAUSE_INVALID_ACTION)
			ret_val = ICF_FAILURE;
			break;
	} /* End switch(*p_api_id) */

	/* now construct the internal message for sending to App via ADM */
	if (ICF_SUCCESS == ret_val)
	{
#ifdef ICF_NAT_MEDIA_TRAVERSAL   
        if(ICF_RECV_UDP_DATA_IND == *p_api_id)
        {
             ret_val = icf_mmi_send_recv_udp_packet_ind(p_mmi_pdb);

        }
        else
        {
#endif
             /* Fill header in internal message structure */
            p_mmi_pdb->p_internal_msg->msg_hdr.msg_id = *p_api_id;

            p_mmi_pdb->p_internal_msg->msg_hdr.payload_length = 
			    p_mmi_pdb->p_glb_pdb->p_recv_msg->hdr.api_length;

            /* Assign ext msg ptr to p_msg_data of icf_internal_msg_st */
            p_mmi_pdb->p_internal_msg->p_msg_data = 
			    (icf_uint8_t *)p_mmi_pdb->p_glb_pdb->p_recv_msg;

		    ret_val = icf_adm_process_mesg(p_mmi_pdb->p_internal_msg);
        }

#ifdef ICF_NAT_MEDIA_TRAVERSAL        
		/* since MMI is not making any new message, it is upto MRM/ES to free
		 * the received external message */
	}
#endif

	ICF_FUNCTION_EXIT(p_mmi_pdb->p_glb_pdb)
	return ret_val;
} /* End icf_mmi_proc_imsc_icf_msg() */
ICF_MM_ROUTING_END


#ifndef ICF_UNIT_TEST
/******************************************************************************
 * FUNCTION:        icf_mmi_deinit
 *
 * DESCRIPTION:     
 *
 ******************************************************************************/

icf_void_t icf_mmi_deinit(
        INOUT icf_glb_pdb_st *p_glb_pdb,
        INOUT icf_void_t     *p_data,
        OUT   icf_error_t    *p_ecode)
{
    icf_return_t                     ret_val = ICF_SUCCESS;

    ICF_FUNCTION_ENTER(p_glb_pdb)
    
    /* Dellocate memory to table */
    ICF_STATIC_MEMFREE(p_glb_pdb,p_data, p_ecode, ret_val)

    ICF_FUNCTION_EXIT(p_glb_pdb)
}
#endif

/******************************************************************************
 *
 * FUNCTION:        icf_mmi_send_med_cap_req
 *
 * DESCRIPTION:     This function is invoked when ever any function needs to 
 *                  send media capabilities request to MM. 
 *                   
 * RETURNS:         ICF_SUCCESS in case of success and ICF_FAILURE in 
 *                  case of any failure.       
 *
 ******************************************************************************/

icf_return_t icf_mmi_send_med_cap_req(
        INOUT icf_mmi_pdb_st             *p_mmi_pdb)
{
    icf_return_t                         ret_val = ICF_SUCCESS;
    icf_msg_st                           *p_mmi_msg = ICF_NULL;
    icf_uint8_t                          api_len;
    icf_get_media_cap_req_st             *p_med_cap_req = ICF_NULL;
    icf_return_t                         dummy_ret_val = ICF_SUCCESS;
    /* ICF_6_1_OPTIONS_SUPPORT start */
    icf_media_capabilities_req_st	 *p_icf_media_capab_req = ICF_NULL;
    /* ICF_6_1_OPTIONS_SUPPORT end */

    ICF_FUNCTION_ENTER(p_mmi_pdb->p_glb_pdb)
       
    /* 
     * The media capability request does not have any payload so the size would
     * be  equal to the size of API header plus 1 for single element in 
     * stretchible array
     */ 
    api_len = ICF_PORT_SIZEOF(icf_msg_st) + 
	      ICF_PORT_SIZEOF(icf_media_capabilities_req_st); 
    
    ICF_MEMGET(p_mmi_pdb->p_glb_pdb,api_len,ICF_MEM_COMMON,p_mmi_msg,
            ICF_RET_ON_FAILURE,p_mmi_pdb->p_ecode,ret_val)

	if(ICF_FAILURE == ret_val)
	{
		ICF_ALARM(((icf_uint8_t *)"[MMI]: MEMGET failure"));
		return ret_val;
	}
    p_med_cap_req = (icf_get_media_cap_req_st *)
                             p_mmi_pdb->p_internal_msg->p_msg_data;

  /* ICF_6_1_OPTIONS_SUPPORT start */
    p_icf_media_capab_req = (icf_media_capabilities_req_st *)
		  	      (p_mmi_msg->payload);
    p_icf_media_capab_req->bit_mask = 0;

    /*  SPR 18064 Merge */
    p_icf_media_capab_req->line_id = p_med_cap_req->line_id;

    p_icf_media_capab_req->p_req_stream_list = ICF_NULL;

    if ( ICF_NULL != p_med_cap_req->p_app_sdp_list)
    {
	icf_cmn_clone_stream_capab_list(
		p_mmi_pdb->p_glb_pdb,
		&(p_icf_media_capab_req->p_req_stream_list),
		p_med_cap_req->p_app_sdp_list);

	p_icf_media_capab_req->bit_mask |= ICF_MED_CAP_STREAM_PRSNT;
    }
    /* ICF_6_1_OPTIONS_SUPPORT end */


    /* Fill the header of the message to be send */
    p_mmi_msg->hdr.api_id = ICF_MEDIA_CAPABILITIES_REQ;
    p_mmi_msg->hdr.source_id = ICF_MODULE_ICF;
    p_mmi_msg->hdr.destination_id = ICF_MODULE_MM;
    p_mmi_msg->hdr.api_length = api_len ;
    p_mmi_msg->hdr.call_id = p_med_cap_req->context_identifier;
    
    ret_val = icf_mmi_send(p_mmi_pdb,p_mmi_msg,p_mmi_pdb->p_ecode);

#ifdef ICF_TRACE_ENABLE
	if(ICF_SUCCESS == ret_val)
	{
		p_mmi_pdb->hdr = p_mmi_msg->hdr;
		ICF_MMI_TRACE(p_mmi_pdb,ICF_TRACE_HDR_SEND)
	}
#endif
    icf_cmn_free_icf_stream_capab_list(p_mmi_pdb->p_glb_pdb,
		&(p_icf_media_capab_req->p_req_stream_list));	
    /* Free memory */
    ICF_MEMFREE(p_mmi_pdb->p_glb_pdb,p_mmi_msg,ICF_MEM_COMMON,
            p_mmi_pdb->p_ecode,dummy_ret_val)
    /* To Remove Warning */
    dummy_ret_val = dummy_ret_val;
        
    ICF_FUNCTION_EXIT(p_mmi_pdb->p_glb_pdb)
    return ret_val;
}/* End function */


/****************************************************************************** *
 *
 * FUNCTION:        icf_mmi_send_sic_med_cap_resp
 *
 * DESCRIPTION:     This function is invoked whenever media capability response
 *                  is to be sent to SIC. 
 *
 ******************************************************************************/

icf_return_t icf_mmi_send_sic_med_cap_resp(
        INOUT icf_mmi_pdb_st *p_mmi_pdb)
{
    icf_return_t                    ret_val = ICF_FAILURE,
                                    dummy_ret_val = ICF_FAILURE;
    icf_get_media_cap_resp_st       get_med_cap_resp;
    icf_media_capabilities_resp_st  *p_med_cap_resp = ICF_NULL;
    icf_list_st                 *p_src_list = ICF_NULL;
    icf_inc_media_sdp_info_st   *p_dest_data = ICF_NULL;
    icf_resp_stream_st          *p_src_data = ICF_NULL;
    icf_boolean_t               memget_fail = ICF_FALSE;


    ICF_FUNCTION_ENTER(p_mmi_pdb->p_glb_pdb)

    p_mmi_pdb->p_internal_msg->msg_hdr.msg_id = ICF_GET_MEDIA_CAP_RESP;
    p_mmi_pdb->p_internal_msg->msg_hdr.payload_length = 
                              ICF_PORT_SIZEOF(icf_get_media_cap_resp_st);
    p_med_cap_resp = (icf_media_capabilities_resp_st *)
                               p_mmi_pdb->p_glb_pdb->p_recv_msg->payload;

    get_med_cap_resp.result = p_med_cap_resp->result;
    
    get_med_cap_resp.context_identifier = 
                  p_mmi_pdb->p_glb_pdb->p_recv_msg->hdr.call_id;

    get_med_cap_resp.p_sdp_list = ICF_NULL;

    if(ICF_SUCCESS == p_med_cap_resp->result)
    {
        p_src_list = p_med_cap_resp->p_resp_stream_list;
		
        while(ICF_NULL != p_src_list)
        {
            ICF_MEMGET(
                p_mmi_pdb->p_glb_pdb,
                ICF_PORT_SIZEOF(icf_inc_media_sdp_info_st),
                ICF_MEM_COMMON,
                p_dest_data,
                ICF_RET_ON_FAILURE,
                p_mmi_pdb->p_ecode,
                dummy_ret_val)	

                if(ICF_SUCCESS == dummy_ret_val)
                {
                    p_src_data = p_src_list->p_data;

                    if(p_src_data)
                    {
                        ICF_MEMGET(
                            p_mmi_pdb->p_glb_pdb,
                            ICF_PORT_SIZEOF(icf_internal_sdp_stream_st),
                            ICF_MEM_SDP,
                            p_dest_data->p_int_sdp_stream,
                            ICF_DONOT_RET_ON_FAILURE,
                            p_mmi_pdb->p_ecode,
                            dummy_ret_val)

                        if(ICF_SUCCESS == dummy_ret_val)
                        {
                            p_dest_data->stream_id = p_src_data->stream_id;
                            p_dest_data->stream_type = p_src_data->stream_type;
                            	
                            icf_port_memcpy(
                               &(p_dest_data->p_int_sdp_stream->sdp_stream),
                               &(p_src_data->local_sdp),
                               (icf_uint32_t const)(ICF_PORT_SIZEOF(icf_sdp_stream_st)));
                            
                            icf_mmi_clone_sdp_stream_lists(
                               p_mmi_pdb,
                               &p_src_data->local_sdp,
                               &p_dest_data->p_int_sdp_stream->sdp_stream);
                            
                            dummy_ret_val = 
                               icf_cmn_add_node_to_list(p_mmi_pdb->p_glb_pdb,
                                   &get_med_cap_resp.p_sdp_list,
                                   p_dest_data);
                        }
                        else
                        {
                            memget_fail = ICF_TRUE;
                            
                            /* MEMFREE p_node */
                            ICF_MEMFREE(
                              p_mmi_pdb->p_glb_pdb, 
                              p_dest_data, 
                              ICF_MEM_COMMON, 
                              p_mmi_pdb->p_ecode, 
                              dummy_ret_val)
                            
                            break;					
                        }
                            
                        if(ICF_FAILURE == dummy_ret_val)
                        {
                            ICF_MEMFREE(
                               p_mmi_pdb->p_glb_pdb,
                               p_dest_data->p_int_sdp_stream,
                               ICF_MEM_COMMON,
                               p_mmi_pdb->p_ecode, 
                               dummy_ret_val)
                            
                           /* MEMFREE p_node */
                           ICF_MEMFREE(
                               p_mmi_pdb->p_glb_pdb, 
                               p_dest_data, 
                               ICF_MEM_COMMON, 
                               p_mmi_pdb->p_ecode, 
                               dummy_ret_val)
                            
                           break;
                       }
                       else
                       {
                            ICF_KLOCWORK_FALSE_POSITIVE(p_mmi_pdb->p_glb_pdb,
                                       &get_med_cap_resp.p_sdp_list)
                            ICF_KLOCWORK_FALSE_POSITIVE(p_mmi_pdb->p_glb_pdb,
                                       p_dest_data)
                            ret_val = ICF_SUCCESS;
					   }
                   }
                   else
                   {
                       /* MEMFREE p_node */
                       ICF_MEMFREE(
                           p_mmi_pdb->p_glb_pdb, 
                           p_dest_data, 
                           ICF_MEM_COMMON, 
                           p_mmi_pdb->p_ecode, 
                           dummy_ret_val)				
                   }
               }
               else
               {
                   memget_fail = ICF_TRUE;
                   break;
               }
               p_src_list = p_src_list->p_next;
           }
       }
       else
       {
           get_med_cap_resp.reason = p_med_cap_resp->error_cause;
           ret_val = ICF_SUCCESS;
       }

       if(ICF_SUCCESS == ret_val)
       {
           p_mmi_pdb->p_internal_msg->p_msg_data = 
                 (icf_uint8_t *)&get_med_cap_resp;

          /*Call entry point of SSA*/
          ret_val = icf_sic_process_mesg(p_mmi_pdb->p_internal_msg);

          if(ICF_FAILURE == ret_val)
          {
              ICF_MMI_TRACE(p_mmi_pdb, ICF_INTERFACE_FAILURE)
              ret_val = ICF_FAILURE;
          }
      }
      else if(ICF_TRUE == memget_fail)
      {
          /* This is the case when failure is returned while MEMGET */
          ICF_ALARM(((icf_uint8_t *)"[MMI]: MEMGET failure"));
      }
      else 
      {
          /* This is the case when failure is returned by some function */
          ICF_ALARM(((icf_uint8_t *)"[MMI]: GENERIC failure"));
      }

      /* Free memory of SDP list */
      p_src_list = get_med_cap_resp.p_sdp_list;

      while(p_src_list)
      {
          p_dest_data = p_src_list->p_data;

          icf_cmn_delete_list(p_mmi_pdb->p_glb_pdb,
           &p_dest_data->p_int_sdp_stream->sdp_stream.p_codec_info_list,
           ICF_MEM_COMMON);

          icf_cmn_delete_list(p_mmi_pdb->p_glb_pdb,
            &p_dest_data->p_int_sdp_stream->sdp_stream.p_encoded_attributes,
            ICF_MEM_COMMON);

          ICF_MEMFREE(p_mmi_pdb->p_glb_pdb, p_dest_data->p_int_sdp_stream, 
                ICF_MEM_COMMON, p_mmi_pdb->p_ecode, dummy_ret_val)

          p_src_list = p_src_list->p_next;
      }

      icf_cmn_delete_list(p_mmi_pdb->p_glb_pdb,
            	&get_med_cap_resp.p_sdp_list,ICF_MEM_COMMON);

  ICF_FUNCTION_EXIT(p_mmi_pdb->p_glb_pdb)

  return ret_val;
}

#ifdef FSS_EMUZED_MEDIA_MANAGER
/******************************************************************************
 *
 * FUNCTION:        icf_send_port_cfg_req_to_mm
 * 
 * DESCRIPTION:     This function is invoked on receiving codec_cfg response from 
 *					Media Manager during ICF init phase. On receiving the response
 *					ICF shall invoke the API to cfg port range with MM
 *
 *****************************************************************************/

icf_return_t	icf_send_port_cfg_req_to_mm(
	INOUT	icf_mmi_pdb_st	*p_mmi_pdb
)
{
	/* send API to configure app port. app port may be muxd with RTCP port.
	 * In which case, RTCP/APP Port will be rtp port + 1.
	 */
	/* local variables */
	icf_return_t				ret_val = ICF_SUCCESS;
	icf_msg_st					*p_msg=NULL;
	icf_api_header_st   		*p_api_hdr = ICF_NULL;
	icf_mmi_data_st			*p_mmi_data = ICF_NULL;
	icf_cfg_set_port_range_req_st	*p_cfg_port_rng_req = ICF_NULL;
	icf_stream_id_t					stream_count = ICF_NULL;

	ICF_PRINT(((icf_uint8_t*)"[MMI]:Entered icf_mmi_handle_mm_init_comp\n"));
	
	/* fetch the MMI glb data */
	ret_val = icf_dbm_get_module_glb_data(p_mmi_pdb->p_glb_pdb,
        (icf_module_id_t)(ICF_INT_MODULE_MMI),
        (icf_void_t *)&p_mmi_data,
        p_mmi_pdb->p_ecode);
    
    if(ICF_FAILURE == ret_val)
    {
        ICF_MMI_TRACE(p_mmi_pdb,ICF_TRACE_FAILURE_BY_DBM)
        ICF_FUNCTION_EXIT(p_mmi_pdb->p_glb_pdb)
        ICF_RESET_MODULE(p_mmi_pdb->p_glb_pdb)
        return ret_val;
    }        

	if (ICF_TRUE == p_mmi_data->init_complete)
	{
		/* this configuration API needs to be fired only incase of 
		 * initialisation. In subsequent cases this API need not be 
		 * fired towards MM
		 */
		return ICF_SUCCESS;
	}

	ICF_MEMGET(p_mmi_pdb->p_glb_pdb,
		ICF_PORT_SIZEOF(icf_api_header_st) + 
		ICF_PORT_SIZEOF(icf_cfg_set_port_range_req_st),
		ICF_MEM_COMMON,
		p_msg,
        ICF_RET_ON_FAILURE,
		p_mmi_pdb->p_ecode,
		ret_val)

	if (ICF_NULL != p_msg)
	{
		p_api_hdr = &(p_msg->hdr);
		p_api_hdr->api_id = ICF_CFG_SET_PORT_RANGE_REQ;
		p_api_hdr->api_length =
			ICF_PORT_SIZEOF(icf_api_header_st) + 
			ICF_PORT_SIZEOF(icf_cfg_set_port_range_req_st);
		p_api_hdr->app_id = ICF_DUMMY_APP_ID;
		p_api_hdr->call_id = ICF_INVALID_CALL_ID;
		p_api_hdr->destination_id = ICF_MODULE_MM;
		p_api_hdr->source_id = ICF_MODULE_ICF;
		p_api_hdr->version = ICF_VERSION_1_0;

		p_cfg_port_rng_req  = (icf_cfg_set_port_range_req_st*)
								(p_msg->payload);

	    p_cfg_port_rng_req->start_rtp_port = p_mmi_data->mm_init_params.rtp_start_port;
        p_cfg_port_rng_req->end_rtp_port = p_mmi_data->mm_init_params.rtp_end_port;;

		p_cfg_port_rng_req->is_app_rtcp_port_mux = 
			p_mmi_data->mm_init_params.app_rtcp_port_mux;
		
		p_cfg_port_rng_req->bit_mask =0; 

		if (ICF_FALSE == p_cfg_port_rng_req->is_app_rtcp_port_mux)
		{
			p_cfg_port_rng_req->bit_mask |= 
				ICF_APP_PORT_RANGE_PRESENT;	
	    	p_cfg_port_rng_req->start_rtp_port = p_mmi_data->mm_init_params.app_data_start_port;
            p_cfg_port_rng_req->end_rtp_port = p_mmi_data->mm_init_params.app_data_end_port;
		}
		
		ret_val = icf_mmi_send(p_mmi_pdb,p_msg,p_mmi_pdb->p_ecode);

		if(ICF_SUCCESS == ret_val)
	    {
			ICF_PRINT
			(("[MMI]:Port Range Configure Request sent to MM successfully\n"));
			ret_val = ICF_SUCCESS;
		}
		else
		{
			ICF_PRINT
				(("[MMI]:Port Range Configure Request to MM failed\n"));
			ret_val = ICF_FAILURE;
		}
			
        /* Free memory */
        ICF_MEMFREE(p_mmi_pdb->p_glb_pdb,
			p_msg,
			ICF_MEM_COMMON,
            p_mmi_pdb->p_ecode,
			ret_val)
	}
	else
	{
		ICF_PRINT(((icf_uint8_t*)"[MMI]:MemAlloc failure\n"));
		ret_val = ICF_FAILURE;
	}

	ICF_PRINT(((icf_uint8_t*)"[MMI]: icf_handle_codec_cfg_resp\n"));
	return ret_val;
}


/******************************************************************************
 *
 * FUNCTION:        icf_send_ringer_tones_req_to_mm
 * 
 * DESCRIPTION:     This function is invoked on receiving 
 *					ICF_MM_CFG_SET_PORT_RANGE_RESP response from 
 *					Media Manager during ICF init phase. On receiving the response
 *					ICF shall invoke the API to ringer tones with MM
 *
 *****************************************************************************/

icf_return_t	icf_send_ringer_tones_req_to_mm(
	INOUT	icf_mmi_pdb_st	*p_mmi_pdb
)
{
	/* local variables */
	icf_return_t				ret_val = ICF_SUCCESS;
	icf_msg_st					*p_msg=NULL;
	icf_api_header_st   		*p_api_hdr = ICF_NULL;
	icf_mmi_data_st			*p_mmi_data = ICF_NULL;
	icf_cfg_ringer_tones_req_st	*p_cfg_ringer_tones_req = ICF_NULL;
	icf_uint32_t				index = 0;
	icf_uint8_t		play_file_list[ICF_INIT_MAX_NO_OF_RINGER_TONES][ICF_MAX_STR_LEN] 
								= {
									"C:\\ringing.wav",
									"C:\\dialtone.wav",
									"C:\\busytone.wav",
									"C:\\ringback.wav"};

	ICF_PRINT(((icf_uint8_t*)"[MMI]:Entered icf_send_ringer_tones_req_to_mm\n"));
	
	/* fetch the MMI glb data */
	ret_val = icf_dbm_get_module_glb_data(p_mmi_pdb->p_glb_pdb,
        (icf_module_id_t)(ICF_INT_MODULE_MMI),
        (icf_void_t *)&p_mmi_data,
        p_mmi_pdb->p_ecode);
    
    if(ICF_FAILURE == ret_val)
    {
        ICF_MMI_TRACE(p_mmi_pdb,ICF_TRACE_FAILURE_BY_DBM)
        ICF_FUNCTION_EXIT(p_mmi_pdb->p_glb_pdb)
        ICF_RESET_MODULE(p_mmi_pdb->p_glb_pdb)
        return ret_val;
    }        

	if (ICF_TRUE == p_mmi_data->init_complete)
	{
		/* this configuration API needs to be fired only incase of 
		 * initialisation. In subsequent cases this API need not be 
		 * fired towards MM
		 */
		return ICF_SUCCESS;
	}

	ICF_MEMGET(p_mmi_pdb->p_glb_pdb,
		ICF_PORT_SIZEOF(icf_api_header_st) + 
		ICF_PORT_SIZEOF(icf_cfg_ringer_tones_req_st),
		ICF_MEM_COMMON,
		p_msg,
        ICF_RET_ON_FAILURE,
		p_mmi_pdb->p_ecode,
		ret_val)

	if (ICF_NULL != p_msg)
	{
		p_api_hdr = &(p_msg->hdr);
		p_api_hdr->api_id = ICF_CFG_RINGER_TONES_REQ;
		p_api_hdr->api_length =
			ICF_PORT_SIZEOF(icf_api_header_st) + 
			ICF_PORT_SIZEOF(icf_cfg_ringer_tones_req_st);
		p_api_hdr->app_id = ICF_DUMMY_APP_ID;
		p_api_hdr->call_id = ICF_INVALID_CALL_ID;
		p_api_hdr->destination_id = ICF_MODULE_MM;
		p_api_hdr->source_id = ICF_MODULE_ICF;
		p_api_hdr->version = ICF_VERSION_1_0;

		p_cfg_ringer_tones_req  = (icf_cfg_ringer_tones_req_st*)(p_msg->payload);
		
		p_cfg_ringer_tones_req->num_of_ringer = ICF_INIT_MAX_NO_OF_RINGER_TONES;
		
		for ( index = 0; index < p_cfg_ringer_tones_req->num_of_ringer; index++)
		{

			p_cfg_ringer_tones_req->ringer[index].ringer_id = index;
			icf_port_strcpy(p_cfg_ringer_tones_req->ringer[index].play_file.str,
				play_file_list[index]);
			p_cfg_ringer_tones_req->ringer[index].play_file.str_len = 
				icf_port_strlen(play_file_list[index]) + 1;
		}
		
		ret_val = icf_mmi_send(p_mmi_pdb,p_msg,p_mmi_pdb->p_ecode);

		if(ICF_SUCCESS == ret_val)
	    {
			ICF_PRINT
			(("[MMI]:Ringer Tone Configure Request sent to MM successfully\n"));
			ret_val = ICF_SUCCESS;
		}
		else
		{
			ICF_PRINT
				(("[MMI]:Ringer Tone Configure Request to MM failed\n"));
			ret_val = ICF_FAILURE;
		}
			
        /* Free memory */
        ICF_MEMFREE(p_mmi_pdb->p_glb_pdb,
			p_msg,
			ICF_MEM_COMMON,
            p_mmi_pdb->p_ecode,
			ret_val)
	}
	else
	{
		ICF_PRINT(((icf_uint8_t*)"[MMI]:MemAlloc failure\n"));
		ret_val = ICF_FAILURE;
	}

	ICF_PRINT(((icf_uint8_t*)"[MMI]: icf_send_ringer_tones_req_to_mm\n"));
	return ret_val;
}

/******************************************************************************
 *
 * FUNCTION:        icf_mmi_init_mm_cfg_params
 * 
 * DESCRIPTION:     This function is invoked at MMI init. Here we read from 
 *					mm.cfg and save the init params in the mm_init_params str.
 *					in the mmi_data_st 
 *
 *****************************************************************************/

icf_return_t icf_mmi_init_mm_cfg_params(
	INOUT	icf_mmi_data_st	*p_mmi_data,
	INOUT   icf_glb_pdb_st   *p_glb_pdb
)
{
	/* local variables */
	char 						filename[ICF_CONF_MAX_LINE_SIZE] = "mm.cfg";
	char 						temp_str[ICF_CONF_MAX_LINE_SIZE];
	FILE						*fp;
	icf_conf_file_st			conf_file;
    icf_mm_init_params_st	mm_init_params = {0};
	icf_int32_t      		temp_val; /* temp variables */ 
	icf_error_t				ecode;
	icf_return_t				ret_val = ICF_SUCCESS;
	icf_uint8_t				count =0;

	/* initialise the mm_init_params for there default values */
	icf_port_memset(&mm_init_params, 0, ICF_PORT_SIZEOF(icf_mm_init_params_st));
	p_mmi_data->mm_init_params.amr_mode = ICF_DEFAULT_AMR_MODE;
	p_mmi_data->mm_init_params.codec_mode = ICF_DEFAULT_CODEC_MODE;
	p_mmi_data->mm_init_params.crc_enabled = ICF_DEFAULT_CRC_ENABLED;
	p_mmi_data->mm_init_params.app_rtcp_port_mux = ICF_DEFAULT_APP_RTCP_PORT_MUX;
	p_mmi_data->mm_init_params.pkt_period = ICF_DEFAULT_PKT_PERIOD;
	p_mmi_data->mm_init_params.rtp_start_port = ICF_DEFAULT_RTP_START_PORT;
	p_mmi_data->mm_init_params.rtp_end_port = ICF_DEFAULT_RTP_END_PORT;
    p_mmi_data->mm_init_params.app_data_start_port = ICF_DEFAULT_APP_DATA_START_PORT;
	p_mmi_data->mm_init_params.app_data_end_port = ICF_DEFAULT_APP_DATA_END_PORT;

	/* read from mm.cfg file and update the mm_cfg_params structures */
	ICF_MEMGET(p_glb_pdb,
		ICF_PORT_SIZEOF(icf_conf_line_st) * ICF_CONF_MAX_LINES,
		ICF_MEM_COMMON,
		conf_file.line,
        ICF_RET_ON_FAILURE,
		&ecode,
		ret_val)

	conf_file.count = 0; /* initilaising conf_file */
	if ((fp = icf_port_fopen ((const char *)filename, "r")) == NULL)
    {
		/* add a trace here */
        ICF_PRINT(((icf_uint8_t*)"[MMI]: ERROR: Could not open configuration file %s \n",
			filename));
    }
	else
	{
		ret_val = icf_mmi_read_conf_file(fp, &conf_file);
		fclose(fp);
	    ICF_PRINT(((icf_uint8_t*)"\nParameters read from mm.cfg >>"));
	}	
	
	/* update the mm_init_params with info read from mm.cfg */
	for (count =0; count< conf_file.count; count++)
	{
		if (icf_port_strmatch(
			(unsigned char *)"AMR_MODE", 
			(unsigned char *)conf_file.line[count].data) == 0)
		{
			icf_port_sscanf(conf_file.line[count].data, "%s %d\n", 
				temp_str, 
				&temp_val);
			p_mmi_data->mm_init_params.amr_mode = temp_val;
			continue;
		}
		else 
		if (icf_port_strmatch(
			(icf_uint8_t*)"CODEC_MODE", 
			(icf_uint8_t*)conf_file.line[count].data) == 0)
		{
			/* CODEC_MODE parmeter */
			icf_port_sscanf(conf_file.line[count].data, "%s %d\n", 
				temp_str, 
				&temp_val);
			if ((1 == temp_val)||
			    (2 == temp_val))
			{
				p_mmi_data->mm_init_params.codec_mode = temp_val;
			}
            else
            {
                ICF_PRINT(((icf_uint8_t*)"NOTE: CODEC MODE - Supported Input Values [1],[2] \n"));
                ICF_PRINT(
					("NOTE: CODEC MODE - Default Value [%d] applied \n",
					ICF_DEFAULT_CODEC_MODE));
            }
			continue;
		}
		else
		if (icf_port_strmatch( (icf_uint8_t*)"CRC_ENABLED", 
			(icf_uint8_t*)conf_file.line[count].data ) == 0)
		{
			/* CRC_ENABLED parameter */
			icf_port_sscanf(conf_file.line[count].data, "%s %d\n", 
				temp_str, 
				&temp_val);
			if ((1 == temp_val)||
			    (0 == temp_val))
			{
				p_mmi_data->mm_init_params.crc_enabled = temp_val;
			}
            else
            {
                ICF_PRINT(((icf_uint8_t*)"NOTE: CRC ENABLED - Supported Input Values [1/Y],[0/N] \n"));
                ICF_PRINT(
					("NOTE: CRC ENABLEDE - Default Value [%d] applied \n",
					ICF_DEFAULT_CRC_ENABLED));
                mm_init_params.crc_enabled = ICF_DEFAULT_CRC_ENABLED;
            }
			continue;
		}
		else
		if (icf_port_strmatch( (icf_uint8_t*)"APP_RTCP_PORT_MUX", 
			(icf_uint8_t*)conf_file.line[count].data ) == 0)
		{
			/* CRC_ENABLED parameter */
			icf_port_sscanf(conf_file.line[count].data, "%s %d\n", 
				temp_str, 
				&temp_val);
			if ((1 == temp_val)||
			    (0 == temp_val))
			{
				p_mmi_data->mm_init_params.app_rtcp_port_mux = temp_val;
			}
            else
            {
                ICF_PRINT(
				("NOTE: APP_RTCP_PORT_MUX - Supported Input Values [1/Y],[0/N] \n"));
                ICF_PRINT(
					("NOTE: APP_RTCP_PORT_MUX - Default Value [%d] applied \n",
					ICF_DEFAULT_APP_RTCP_PORT_MUX));
            }
			continue;
		}
	    else
		if (icf_port_strmatch((icf_uint8_t*)"PACKETIZATION_PERIOD", 
                (icf_uint8_t*)conf_file.line[count].data) == 0)
        {
            /* PACKETIZATION_PERIOD parameter */
            icf_port_sscanf(conf_file.line[count].data, "%s %d\n", 
				temp_str, 
				&temp_val);
            p_mmi_data->mm_init_params.pkt_period = temp_val;
            continue; 
        }
		else if ( icf_port_strmatch((icf_uint8_t*)"RTP_START_PORT", 
                (icf_uint8_t*)conf_file.line[count].data) == 0)
		{
            icf_port_sscanf(conf_file.line[count].data, "%s %d\n", 
				temp_str, 
				&temp_val);
            p_mmi_data->mm_init_params.rtp_start_port = temp_val;
            continue; 
		}
		else if ( icf_port_strmatch((icf_uint8_t*)"RTP_END_PORT", 
                (icf_uint8_t*)conf_file.line[count].data) == 0)
		{
            icf_port_sscanf(conf_file.line[count].data, "%s %d\n", 
				temp_str, 
				&temp_val);
            p_mmi_data->mm_init_params.rtp_end_port = temp_val;
            continue; 
		}
        else if ( icf_port_strmatch((icf_uint8_t*)"APP_DATA_START_PORT", 
                (icf_uint8_t*)conf_file.line[count].data) == 0)
		{
            icf_port_sscanf(conf_file.line[count].data, "%s %d\n", 
				temp_str, 
				&temp_val);
            p_mmi_data->mm_init_params.app_data_start_port = temp_val;
            continue; 
		}
		else if ( icf_port_strmatch((icf_uint8_t*)"APP_DATA_END_PORT", 
                (icf_uint8_t*)conf_file.line[count].data) == 0)
		{
            icf_port_sscanf(conf_file.line[count].data, "%s %d\n", 
				temp_str, 
				&temp_val);
            p_mmi_data->mm_init_params.app_data_end_port = temp_val;
            continue; 
		}
	}	

	ICF_PRINT(((icf_uint8_t*)"\nINFO: AMR MODE: %d \n", 
		p_mmi_data->mm_init_params.amr_mode)); 
    ICF_PRINT(((icf_uint8_t*)"INFO: CODEC_MODE: %d \n", 
		p_mmi_data->mm_init_params.codec_mode)); 
    ICF_PRINT(((icf_uint8_t*)"INFO: CRC_ENABLED: %d \n", 
		p_mmi_data->mm_init_params.crc_enabled)); 
    ICF_PRINT(((icf_uint8_t*)"INFO: APP_RTCP_PORT_MUX: %d \n", 
		p_mmi_data->mm_init_params.app_rtcp_port_mux)); 
	ICF_PRINT(((icf_uint8_t*)"INFO: PACKETIZATION_PERIOD: %d \n", 
		p_mmi_data->mm_init_params.pkt_period ));
	ICF_PRINT(((icf_uint8_t*)"INFO: RTP_START_PORT: %d \n", 
		p_mmi_data->mm_init_params.rtp_start_port ));
	ICF_PRINT(((icf_uint8_t*)"INFO: RTP_END_PORT: %d \n", 
		p_mmi_data->mm_init_params.rtp_end_port ));

	/* free conf_file */
	ICF_MEMFREE(
            p_glb_pdb,
            conf_file.line,
            ICF_MEM_COMMON,
            &ecode,
            ret_val);

	return ICF_SUCCESS;

}

/******************************************************************************
 *
 * FUNCTION:        icf_mmi_handle_mm_init_comp_ind
 * 
 * DESCRIPTION:     This function is invoked when MM init complete is received 
 *					from MM. ICF shall invk the api to configure the codec attrib
 *					at MMI.
 *
 *****************************************************************************/

icf_return_t	icf_mmi_handle_mm_init_comp_ind(
	INOUT	icf_mmi_pdb_st	*p_mmi_pdb
)
{
	/* local variables */	
	icf_return_t				ret_val = ICF_SUCCESS;
	icf_msg_st					*p_msg=NULL;
	icf_api_header_st   		*p_api_hdr = ICF_NULL;
	icf_cfg_codec_info_req_st	*p_cfg_codec_info_req = ICF_NULL;
	icf_mmi_data_st			*p_mmi_data = ICF_NULL;

	ICF_PRINT(((icf_uint8_t*)"[MMI]: Entered icf_mmi_handle_mm_init_comp\n"));
	
	/* fetch the MMI glb data */
	ret_val = icf_dbm_get_module_glb_data(p_mmi_pdb->p_glb_pdb,
        (icf_module_id_t)(ICF_INT_MODULE_MMI),
        (icf_void_t *)&p_mmi_data,
        p_mmi_pdb->p_ecode);
    
    if(ICF_FAILURE == ret_val)
    {
        ICF_MMI_TRACE(p_mmi_pdb,ICF_TRACE_FAILURE_BY_DBM)
        ICF_FUNCTION_EXIT(p_mmi_pdb->p_glb_pdb)
        ICF_RESET_MODULE(p_mmi_pdb->p_glb_pdb)
        return ret_val;
    }        

	ICF_MEMGET(p_mmi_pdb->p_glb_pdb,
		ICF_PORT_SIZEOF(icf_api_header_st) + 
		ICF_PORT_SIZEOF(icf_cfg_codec_info_req_st),
		ICF_MEM_COMMON,
		p_msg,
        ICF_RET_ON_FAILURE,
		p_mmi_pdb->p_ecode,
		ret_val)

	if (ICF_NULL != p_msg)
	{
		p_api_hdr = &(p_msg->hdr);
		p_api_hdr->api_id = ICF_CFG_CODEC_INFO_REQ;
		p_api_hdr->api_length =
			ICF_PORT_SIZEOF(icf_api_header_st) + 
			ICF_PORT_SIZEOF(icf_cfg_codec_info_req_st);
		p_api_hdr->app_id = ICF_DUMMY_APP_ID;
		p_api_hdr->call_id = ICF_INVALID_CALL_ID;
		p_api_hdr->destination_id = ICF_MODULE_MM;
		p_api_hdr->source_id = ICF_MODULE_ICF;
		p_api_hdr->version = ICF_VERSION_1_0;

		p_cfg_codec_info_req  = (icf_cfg_codec_info_req_st*)(p_msg->payload);
		p_cfg_codec_info_req->count = 2;
		
		p_cfg_codec_info_req->codec_supp[0].codec_type = ICF_CODEC_TYPE_AUDIO;

        icf_port_strcpy(p_cfg_codec_info_req->codec_supp[0].codec_name.str,
			(icf_uint8_t*)"AMRNB");

        p_cfg_codec_info_req->codec_supp[0].codec_name.str_len = 5;

        p_cfg_codec_info_req->codec_supp[0].bitmask &= ICF_CODEC_PARAM_PRESENT;

        p_cfg_codec_info_req->codec_supp[0].codec_param.audio_param.bit_mask &= 
            ICF_AUDIO_CODEC_PTIME;

        p_cfg_codec_info_req->codec_supp[0].codec_param.audio_param.
            num_of_supported_ptime = 1;
		p_cfg_codec_info_req->codec_supp[0].codec_param.audio_param.ptime[0] = 
			p_mmi_data->mm_init_params.pkt_period;

        p_cfg_codec_info_req->codec_supp[0].codec_param.audio_param.bit_mask |=
            ICF_AUDIO_CODEC_SAMPLING_RATE;

        p_cfg_codec_info_req->codec_supp[0].codec_param.audio_param.
            num_of_supp_sampling_rate = 1;
		p_cfg_codec_info_req->codec_supp[0].codec_param.audio_param.sampling_rate[0] = 8000;

        p_cfg_codec_info_req->codec_supp[0].codec_param.audio_param.bit_mask |=
            ICF_AUDIO_CODEC_CHANNEL;
		p_cfg_codec_info_req->codec_supp[0].codec_param.audio_param.channels = 1;
		

        p_cfg_codec_info_req->codec_supp[0].codec_param.audio_param.bit_mask |=
            ICF_AUDIO_CODEC_AMR_PARAM;

        p_cfg_codec_info_req->codec_supp[0].codec_param.audio_param.amr_params.bit_mask &=
            ICF_AMR_MODE_LIST_PRESENT;
        p_cfg_codec_info_req->codec_supp[0].codec_param.audio_param.amr_params.amr_initial_mode=
            p_mmi_data->mm_init_params.amr_mode;
        p_cfg_codec_info_req->codec_supp[0].codec_param.audio_param.amr_params.amr_mode_count = 3;
        p_cfg_codec_info_req->codec_supp[0].codec_param.audio_param.amr_params.amr_mode_list[0] = 0;
        p_cfg_codec_info_req->codec_supp[0].codec_param.audio_param.amr_params.amr_mode_list[0] = 1;
        p_cfg_codec_info_req->codec_supp[0].codec_param.audio_param.amr_params.amr_mode_list[0] = 4;
        p_cfg_codec_info_req->codec_supp[0].codec_param.audio_param.amr_params.crc_enabled = 
            p_mmi_data->mm_init_params.crc_enabled;
        p_cfg_codec_info_req->codec_supp[0].codec_param.audio_param.amr_params.codec_mode = 
                p_mmi_data->mm_init_params.codec_mode;

       p_cfg_codec_info_req->codec_supp[1].bitmask &= ICF_CODEC_PARAM_PRESENT;

		p_cfg_codec_info_req->codec_supp[1].codec_type = ICF_CODEC_TYPE_AUDIO;

        icf_port_strcpy(p_cfg_codec_info_req->codec_supp[1].codec_name.str,
			(icf_uint8_t*)"AMRWB");

        p_cfg_codec_info_req->codec_supp[1].codec_name.str_len = 5;

        p_cfg_codec_info_req->codec_supp[1].codec_param.audio_param.bit_mask &= 
            ICF_AUDIO_CODEC_PTIME;

		p_cfg_codec_info_req->codec_supp[1].codec_param.audio_param.
            num_of_supported_ptime = 1;
		p_cfg_codec_info_req->codec_supp[1].codec_param.audio_param.ptime[0] = 
			p_mmi_data->mm_init_params.pkt_period;

        p_cfg_codec_info_req->codec_supp[1].codec_param.audio_param.bit_mask |=
            ICF_AUDIO_CODEC_SAMPLING_RATE;

        p_cfg_codec_info_req->codec_supp[1].codec_param.audio_param.
            num_of_supp_sampling_rate = 1;

		p_cfg_codec_info_req->codec_supp[1].codec_param.audio_param.sampling_rate[0] = 16000;

        p_cfg_codec_info_req->codec_supp[1].codec_param.audio_param.bit_mask |=
            ICF_AUDIO_CODEC_CHANNEL;
		p_cfg_codec_info_req->codec_supp[1].codec_param.audio_param.channels = 1;
		

        p_cfg_codec_info_req->codec_supp[1].codec_param.audio_param.bit_mask |=
            ICF_AUDIO_CODEC_AMR_PARAM;

        p_cfg_codec_info_req->codec_supp[1].codec_param.audio_param.amr_params.bit_mask &=
            ICF_AMR_MODE_LIST_PRESENT;
        p_cfg_codec_info_req->codec_supp[1].codec_param.audio_param.amr_params.amr_initial_mode=
            p_mmi_data->mm_init_params.amr_mode;
        p_cfg_codec_info_req->codec_supp[1].codec_param.audio_param.amr_params.amr_mode_count = 3;
        p_cfg_codec_info_req->codec_supp[1].codec_param.audio_param.amr_params.amr_mode_list[0] = 0;
        p_cfg_codec_info_req->codec_supp[1].codec_param.audio_param.amr_params.amr_mode_list[0] = 1;
        p_cfg_codec_info_req->codec_supp[1].codec_param.audio_param.amr_params.amr_mode_list[0] = 4;
        p_cfg_codec_info_req->codec_supp[1].codec_param.audio_param.amr_params.crc_enabled = 
            p_mmi_data->mm_init_params.crc_enabled;
        p_cfg_codec_info_req->codec_supp[1].codec_param.audio_param.amr_params.codec_mode = 
                p_mmi_data->mm_init_params.codec_mode;

		ret_val = icf_mmi_send(p_mmi_pdb,p_msg,p_mmi_pdb->p_ecode);

		if(ICF_SUCCESS == ret_val)
	    {
			ICF_PRINT(((icf_uint8_t*)"[MMI]: Codec Configure Request sent to MM successfully\n"));
			ret_val = ICF_SUCCESS;
		}
		else
		{
			ICF_PRINT(((icf_uint8_t*)"[MMI]: Codec Configure Request to MM failed\n"));
			ret_val = ICF_FAILURE;
		}
			
        /* Free memory */
        ICF_MEMFREE(p_mmi_pdb->p_glb_pdb,
			p_msg,
			ICF_MEM_COMMON,
            p_mmi_pdb->p_ecode,
			ret_val)
	}
	else
	{
		ICF_PRINT(((icf_uint8_t*)"[MMI]:MemAlloc failure\n"));
		ret_val = ICF_FAILURE;
	}

	ICF_PRINT(((icf_uint8_t*)"[MMI]: icf_mmi_handle_mm_init_comp\n"));
	return ret_val;
}

/******************************************************************************
 *
 * FUNCTION:        icf_mmi_set_init_complete
 * 
 * DESCRIPTION:     This function is invoked when ringer tones resp is received 
 *					from MM. ICF shall set MM init complete.
 *
 *****************************************************************************/

icf_return_t	icf_mmi_set_init_complete(
	INOUT	icf_mmi_pdb_st	*p_mmi_pdb)
{
	icf_mmi_data_st			*p_mmi_data = ICF_NULL;
	icf_return_t				ret_val = ICF_SUCCESS;
	icf_internal_msg_st		internal_msg;

    ICF_FUNCTION_ENTER(p_mmi_pdb->p_glb_pdb)
	
	ret_val = icf_dbm_get_module_glb_data(p_mmi_pdb->p_glb_pdb,
        (icf_module_id_t)(ICF_INT_MODULE_MMI),
        (icf_void_t *)&p_mmi_data,
        p_mmi_pdb->p_ecode);
	if (ICF_TRUE == p_mmi_data->init_complete)
	{
		/* this configuration API needs to be fired only incase of 
		 * initialisation. In subsequent cases this API need not be 
		 * fired towards MM
		 */
		return ICF_SUCCESS;
	}
	
	p_mmi_data->init_complete = ICF_TRUE;
	internal_msg.msg_hdr.msg_id = ICF_CFG_MMI_MM_INIT_COMPLETE;
	internal_msg.msg_hdr.p_glb_pdb = p_mmi_pdb->p_glb_pdb;
	icf_cfg_process_mesg(&internal_msg);

	ICF_FUNCTION_EXIT(p_mmi_pdb->p_glb_pdb)
	return ret_val;
}

/****************************************************************************
** Function: icf_mmi_read_conf_file
**
****************************************************************************
** DESCRIPTION: This function reads the configuration file and stores
**              in a structure p_file.
**              Comments in the configuration files are not stored.
** 
** 
**
***************************************************************************/

icf_return_t icf_mmi_read_conf_file(FILE* fp, icf_conf_file_st* p_file)
{
    p_file->count = 0;
	
    /* 
	** reading conf file line by line up to end of file 
	*/
	
    for ( 	p_file->count = 0 ; 
			p_file->count < ICF_CONF_MAX_LINES; 
			p_file->count++)
    {
        if (
			  icf_port_fgets((icf_uint8_t*)p_file->line[p_file->count].data,
				ICF_CONF_MAX_LINE_SIZE, fp) == ICF_NULL )
        {
            if ( feof(fp))
            {
               return ICF_SUCCESS;
            }
			
        }
        /* 
		** string start and termination 
		*/
        icf_mmi_str_trim((char*)p_file->line[p_file->count].data);
        /* 
		** Check whether Line starts from # or \n or NULL 
		*/
        if (p_file->line[p_file->count].data[0] == ICF_CONF_COMMENT_CHAR || 
			p_file->line[p_file->count].data[0] == '\n' || 
			p_file->line[p_file->count].data[0] == 0)
        {
            p_file->count--; 
			/* if line starts #,\n or NULL , decrement the count  */
        }
        else
        {
			/* do nothing */
        }
    }
    return ICF_SUCCESS; 
}


/****************************************************************************
** Function: icf_mmi_str_trim 
**
****************************************************************************
** DESCRIPTION: This function removes extra TAB/spaces from start and 
**              end of the string. Replaces all tabs by space in the string.
** 
** 
**
***************************************************************************/

icf_void_t icf_mmi_str_trim(INOUT icf_uint8_t * str)
{
    int starts_from, ends_at, count=0;

    ends_at = icf_port_strlen(str);
    /* 
	** removing all tab bar from the line 
	*/
    for (count = 0; str[count] !=  '\n' && count < ends_at; count++)
    {
        if (str[count] == '\t')
            str[count]=' ';
    }
	
    count = 0;
    while(str[count]==' ') count++;
    starts_from = count;
	
	
    while(str[ends_at]=='\n' || str[ends_at]==0) 
	{
		ends_at--;
		if ( ends_at < 0 )
		{
			break;
		}
	}
    str[ends_at+1] = ICF_NULL;
    icf_port_strcpy(str, str+starts_from);
}
#endif

#ifdef ICF_QOS_SUPPORT
/******************************************************************************
 *
 * FUNCTION:        icf_mmi_qos_reserve_success_resp
 *
 * DESCRIPTION:     This function is invoked when ever DMS request is to be
 *                  send to the MM.                   
 *                  
 * RETURNS:         ICF_SUCCESS in case of success and ICF_FAILURE in 
 *                  case of any failure.       
 *
 ******************************************************************************/

icf_return_t icf_mmi_qos_reserve_success_resp(
		INOUT icf_mmi_pdb_st             *p_mmi_pdb)
{
	icf_return_t                         ret_val = ICF_SUCCESS;
    
    ICF_FUNCTION_ENTER(p_mmi_pdb->p_glb_pdb)

    /* If CC is waiting for DMS response then send success response to it */
    if(ICF_CC_07_AWT_DMS_RESP == p_mmi_pdb->p_call_ctx->call_state)
    {
        ICF_MMI_TRACE(p_mmi_pdb,ICF_TRACE_CC_AWT_DMS)

		if(ICF_MMI_CMS_ONGOING == p_mmi_pdb->p_mmi_ctx->ongoing_event)
		{
			icf_mmi_send_rel_res_ind(p_mmi_pdb);            
			/* Make and send DMS resp to CC */
			ret_val = icf_mmi_send_delete_media_response(p_mmi_pdb);
		}
		else
		{
			ret_val = icf_mmi_send_awaited_dms_req(p_mmi_pdb);
		}

        return ret_val;        
    }/* End if to check CC state */      

    /* Secondary PDP Context Activation should occur only in 2 cases
	 * 1. After Successful GCL Response for Incoming Call
	 * 2. After Successful Codec Commit Response in case of MMS
	      for Remote Media Application.
    */
    
	/* Check if Success Response is in case of MMS */
    
	if(ICF_MMI_MMS_MOD_ONGOING == p_mmi_pdb->p_mmi_ctx->ongoing_event)
    {
		/* send MMS */
        ret_val = icf_mmi_send_mms_req(p_mmi_pdb);                        
        if(ICF_SUCCESS == ret_val)
        {
            /* Change state */
            ICF_MMI_SET_STATE(p_mmi_pdb,
                    ICF_MMI_04_AWT_MMS_RESP)

            ICF_MMI_TRACE(p_mmi_pdb,ICF_TRACE_API_SEND)
        }
	}/* End if to check modify for remote media app */
    else
    {
		ret_val = icf_mmi_send_open_channel_req(p_mmi_pdb);
			
		if(ICF_SUCCESS == ret_val)
		{
			ICF_MMI_SET_STATE(p_mmi_pdb, ICF_MMI_06_AWT_OPEN_CHANNEL_RESP)
		}
    }/* End else */
                
    ICF_FUNCTION_EXIT(p_mmi_pdb->p_glb_pdb)
    return ret_val;
}

/******************************************************************************
 *
 * FUNCTION:        icf_mmi_qos_reserve_failure_resp
 *
 * DESCRIPTION:     This function is invoked when ever DMS request is to be
 *                  send to the MM.                   
 *                  
 * RETURNS:         ICF_SUCCESS in case of success and ICF_FAILURE in 
 *                  case of any failure.       
 *
 ******************************************************************************/

icf_return_t icf_mmi_qos_reserve_failure_resp(
		INOUT icf_mmi_pdb_st             *p_mmi_pdb)
{

	icf_return_t                         ret_val;
    
    ICF_FUNCTION_ENTER(p_mmi_pdb->p_glb_pdb)

    /* If CC is waiting for DMS response then send success response to it */
    if(ICF_CC_07_AWT_DMS_RESP == p_mmi_pdb->p_call_ctx->call_state)
    {
        ICF_MMI_TRACE(p_mmi_pdb,ICF_TRACE_CC_AWT_DMS)

		if(ICF_MMI_CMS_ONGOING == p_mmi_pdb->p_mmi_ctx->ongoing_event)
		{
			icf_mmi_send_rel_res_ind(p_mmi_pdb);            
			/* Make and send DMS resp to CC */
			ret_val = icf_mmi_send_delete_media_response(p_mmi_pdb);
		}
		else
		{
			ret_val = icf_mmi_send_awaited_dms_req(p_mmi_pdb);
		}

        return ret_val;        
    }/* End if to check CC state */  

#ifdef ICF_REL_OLD 
/* to be handled when re-invite with pre-condition is enabled */

    /* Check if modify for codec change is set */
    if(ICF_MODIFY_FOR_CODEC_CHANGE & p_mmi_pdb->p_mmi_ctx->mmi_bitmask)
    {
        ICF_MMI_TRACE(p_mmi_pdb,ICF_TRACE_MODIFY_FOR_CODEC_CHANGE_SET)
        
        /* There is no need to send DMS in this case because CMS has not 
		 * taken place Send rel res req */
            
        ret_val = icf_mmi_send_rel_res_req(p_mmi_pdb);
        if(ICF_SUCCESS == ret_val)
        {
            ICF_MMI_SET_STATE(p_mmi_pdb,
                    ICF_MMI_06_AWT_RM_REL_RES_RESP)

            ICF_MMI_TRACE(p_mmi_pdb,ICF_TRACE_API_SEND)
        }
            
        /* Check if restoration is in progress */
        if(ICF_RESTORATION_IN_PROGRESS & p_mmi_pdb->p_mmi_ctx->mmi_bitmask)
        {
            ICF_MMI_TRACE(p_mmi_pdb,ICF_TRACE_RESTORATION_IN_PROGRESS_SET)
            
            /* Reset bit */
            ICF_MMI_RESET_BIT(p_mmi_pdb,
                    ICF_RESTORATION_IN_PROGRESS)
            ICF_MMI_RESET_BIT(p_mmi_pdb,
                        ICF_MODIFY_FOR_CODEC_CHANGE)    
            ICF_MMI_SET_BIT(p_mmi_pdb,ICF_DELETE_COMPLETE)

            
			/* Failure Response may come even if one of many Sec PDP 
			contexts have failed, therefore any remaining contexts have to be 
			deactivated */
			ret_val = icf_mmi_send_sec_pdp_deactiv_req(p_mmi_pdb);
            
			/* Send error ind to CC */
            p_cc_internal_msg = p_mmi_pdb->p_internal_msg;
            p_cc_internal_msg->msg_hdr.msg_id = ICF_MEDIA_ERROR_IND;
            p_cc_internal_msg->msg_hdr.payload_length = 0;
            
            err_ind.reason = ICF_ECODE_MMS_RESP_FAILURE_MEDIA_ERROR;
            (icf_mm_err_ind_st *)(p_cc_internal_msg->p_msg_data) = &err_ind;

			
		  /* we need not wait for any response in this case */
            ICF_MMI_SET_STATE(p_mmi_pdb,ICF_MMI_00_IDLE)

            ret_val = icf_cm_process_mesg(p_cc_internal_msg);
            if(ICF_FAILURE == ret_val)
            {
                /* Failure by CC */
                ICF_MMI_TRACE(p_mmi_pdb,ICF_TRACE_FAILURE_BY_CM)
            }
        }/* End if to check restoration */
        
        else
        {
            /* Set restoration bit */
            ICF_MMI_SET_BIT(p_mmi_pdb,
                    ICF_RESTORATION_IN_PROGRESS)
        }
    }/* End if to check modify for codec change */ 
    
    else
#endif
    {
        /* Failure Response may come even if one of many Sec PDP 
		contexts have failed, therefore any remaining contexts have to be 
		deactivated . This will be done when CC sends DMS */

		/* Incase Failure Response comes during Outgoing Call DMS should be 
		send from CC else only Release Resource will be sent
		*/
		if(ICF_MMI_MMS_MOD_ONGOING == p_mmi_pdb->p_mmi_ctx->ongoing_event)
		{
			/* Precondition: send failure err ind to CC to CC clear buffered req/resp */
			ret_val = icf_mmi_send_err_ind_call_clear(p_mmi_pdb);
		}
		else
		{
			/* Precondition: send failure CMS reposne to CC clear buffered req/resp */

			p_mmi_pdb->p_call_ctx->release_cause = ICF_CALL_CLEAR_REASON_PRECONDITIONS_FAILURE;
			ret_val = icf_mmi_send_err_ind_call_clear(p_mmi_pdb);

/*			((icf_mmi_internal_cms_resp_st *)p_mmi_pdb->p_mmi_ctx->p_mmi_resp)->\
				result = ICF_FAILURE;

			ret_val = icf_mmi_send_create_media_response(p_mmi_pdb);
*/
		}
    
		if(ICF_FAILURE == ret_val)
        {
            /* Failure by CC */
            ICF_MMI_TRACE(p_mmi_pdb,ICF_TRACE_FAILURE_BY_CM)
        }
    }
    
    ICF_FUNCTION_EXIT(p_mmi_pdb->p_glb_pdb)
    return ret_val;     
}

/******************************************************************************
 *
 * FUNCTION:        icf_mmi_send_qos_release_req
 *
 * DESCRIPTION:                        
 *                  
 * RETURNS:         ICF_SUCCESS in case of success and ICF_FAILURE in 
 *                  case of any failure.       
 *
 ******************************************************************************/

icf_return_t icf_mmi_send_qos_release_req(
		INOUT icf_mmi_pdb_st             *p_mmi_pdb)
{
icf_return_t		ret_val = ICF_SUCCESS;

	if(ICF_QOS_RESERVATION_ATTEMPTED & p_mmi_pdb->p_mmi_ctx->mmi_bitmask)
	{
#ifdef IMS_CLIENT
#ifdef ICF_NW_ACTIVATE
ICF_CHECK_IMS_START(p_mmi_pdb->p_glb_pdb,ICF_TRUE)

		ret_val = icf_mmi_send_sec_pdp_deactiv_req(p_mmi_pdb);

ICF_CHECK_IMS_END
#endif
#endif

#ifdef NON_IMS_CLIENT
ICF_CHECK_IMS_START(p_mmi_pdb->p_glb_pdb,ICF_FALSE)

		ret_val = icf_mmi_send_qos_release_res_req(p_mmi_pdb);

ICF_CHECK_IMS_END
#endif

		ICF_MMI_RESET_BIT(p_mmi_pdb,ICF_QOS_RESERVATION_ATTEMPTED)
	}
	return ret_val;

}

/******************************************************************************
 *
 * FUNCTION:        icf_mmi_send_qos_release_res_req
 *
 * DESCRIPTION:                        
 *                  
 * RETURNS:         ICF_SUCCESS in case of success and ICF_FAILURE in 
 *                  case of any failure.       
 *
 ******************************************************************************/

icf_return_t icf_mmi_send_qos_release_res_req(
		INOUT icf_mmi_pdb_st             *p_mmi_pdb)
{
  
   icf_return_t                         ret_val = ICF_SUCCESS;
   icf_internal_msg_st                  *p_nm_internal_msg = ICF_NULL;
   icf_nm_non_ims_qos_release_st		msg_payload;
    
   ICF_FUNCTION_ENTER(p_mmi_pdb->p_glb_pdb)
	   
	/* Fill the header of the message to be send */
	p_nm_internal_msg = p_mmi_pdb->p_internal_msg;
   p_nm_internal_msg->msg_hdr.msg_id = ICF_NM_QOS_RELEASE_FOR_NON_IMS ;
   p_nm_internal_msg->msg_hdr.payload_length = ICF_PORT_SIZEOF(icf_nm_non_ims_qos_release_st);
   p_nm_internal_msg->msg_hdr.p_glb_pdb = p_mmi_pdb->p_glb_pdb;
   /* Filling the payload */
   p_nm_internal_msg->p_msg_data = (icf_uint8_t *)&msg_payload;
   msg_payload.call_id = p_mmi_pdb->p_call_ctx->call_id;
   
   ICF_MMI_TRACE(p_mmi_pdb,ICF_TRACE_HDR_SEND)
	   
	   ret_val = icf_nm_process_mesg(p_nm_internal_msg);
    
   ICF_FUNCTION_EXIT(p_mmi_pdb->p_glb_pdb)
    return ret_val; 
} 

/******************************************************************************
 *
 * FUNCTION:        icf_mmi_send_qos_reserve_res_req
 *
 * DESCRIPTION:                       
 *                  
 * RETURNS:         ICF_SUCCESS in case of success and ICF_FAILURE in 
 *                  case of any failure.       
 *
 ******************************************************************************/

icf_return_t icf_mmi_send_qos_reserve_res_req(
		INOUT icf_mmi_pdb_st             *p_mmi_pdb)
{
	icf_return_t					ret_val = ICF_SUCCESS;
	icf_internal_msg_st				*p_nm_internal_msg = ICF_NULL;
	icf_nm_non_ims_qos_reserve_st	msg_payload;
	
   ICF_FUNCTION_ENTER(p_mmi_pdb->p_glb_pdb)

	/* Fill the header of the message to be send */
	p_nm_internal_msg = p_mmi_pdb->p_internal_msg;
	p_nm_internal_msg->msg_hdr.msg_id = ICF_NM_QOS_RESERVE_FOR_NON_IMS ;
	p_nm_internal_msg->msg_hdr.payload_length = ICF_PORT_SIZEOF(icf_nm_non_ims_qos_reserve_st);
	p_nm_internal_msg->msg_hdr.p_glb_pdb = p_mmi_pdb->p_glb_pdb;
	/* Filling the payload */
	p_nm_internal_msg->p_msg_data = (icf_uint8_t *)&msg_payload;
	msg_payload.call_id = p_mmi_pdb->p_call_ctx->call_id;
	
	ICF_MMI_TRACE(p_mmi_pdb,ICF_TRACE_HDR_SEND)
		
	ret_val = icf_nm_process_mesg(p_nm_internal_msg);
	
   ICF_FUNCTION_EXIT(p_mmi_pdb->p_glb_pdb)
   return ret_val; 
}

#ifdef IMS_CLIENT
#ifdef ICF_NW_ACTIVATE
/******************************************************************************
 *
 * FUNCTION:        icf_mmi_send_sec_pdp_activ_req
 *
 * DESCRIPTION:                       
 *                  
 * RETURNS:         ICF_SUCCESS in case of success and ICF_FAILURE in 
 *                  case of any failure.       
 *
 ******************************************************************************/

icf_return_t icf_mmi_send_sec_pdp_activ_req(
		INOUT icf_mmi_pdb_st             *p_mmi_pdb)
{
	icf_return_t           ret_val = ICF_SUCCESS;
    icf_nm_pdp_context_st   *p_pdp_ctxts=ICF_NULL;
	icf_internal_msg_st     *p_nm_internal_msg = ICF_NULL;
    icf_nm_multiple_sec_pdp_activate_st msg_payload;
	icf_stream_id_t	   	num_of_m_lines = ICF_NULL;

	if(ICF_NULL == p_mmi_pdb->p_call_ctx->p_auth_token)
	{
		icf_mmi_send_err_ind_call_clear(p_mmi_pdb);
		return ICF_FAILURE;
	}

	if(ICF_QOS_RESERVATION_ATTEMPTED & p_mmi_pdb->p_mmi_ctx->mmi_bitmask)
	{
		icf_mmi_send_sec_pdp_deactiv_req(p_mmi_pdb);
	}

	num_of_m_lines = p_mmi_pdb->p_mmi_ctx->pdp_count;

	ICF_MEMGET(p_mmi_pdb->p_glb_pdb, num_of_m_lines * ICF_PORT_SIZEOF(icf_nm_pdp_context_st),
    			  ICF_MEM_COMMON, p_pdp_ctxts,
				  ICF_RET_ON_FAILURE, p_mmi_pdb->p_ecode, ret_val)

	if(ICF_SUCCESS==icf_mmi_qos_map_sdp_params_to_sec_pdps(
         				p_mmi_pdb,
         				p_pdp_ctxts))
	{

		if(ICF_SUCCESS == icf_dbm_get_module_glb_data(
                             p_mmi_pdb->p_glb_pdb,       
                             (icf_module_id_t)ICF_INT_MODULE_NM,
                             (icf_void_t*)(&msg_payload.p_nm_ptr),
                             &(p_mmi_pdb->p_internal_msg->msg_hdr.ecode)))
		{        
			/* Fill the header of the message to be send */
			p_nm_internal_msg = p_mmi_pdb->p_internal_msg;
			p_nm_internal_msg->msg_hdr.msg_id = ICF_NM_MULTIPLE_SEC_ACT ;
			p_nm_internal_msg->msg_hdr.payload_length = 
					ICF_PORT_SIZEOF(icf_nm_multiple_sec_pdp_activate_st);
			p_nm_internal_msg->msg_hdr.p_glb_pdb = p_mmi_pdb->p_glb_pdb ;
			/* Filling the Call Id in the payload */
			(icf_nm_multiple_sec_pdp_activate_st *)p_nm_internal_msg->\
					p_msg_data = &msg_payload;
			msg_payload.call_id = p_mmi_pdb->p_call_ctx->call_id;
			msg_payload.no_of_cntxts=num_of_m_lines;
			msg_payload.p_pdp_contexts=p_pdp_ctxts;
	
			ICF_MMI_TRACE(p_mmi_pdb,ICF_TRACE_HDR_SEND)
    
			ret_val = icf_nm_process_mesg(p_nm_internal_msg);

		}
	}
	p_mmi_pdb = p_mmi_pdb;


	if(ICF_NULL!=p_pdp_ctxts)
	{
		if(ICF_SUCCESS == ret_val)
        {					
		    ICF_MEMFREE(p_mmi_pdb->p_glb_pdb,p_pdp_ctxts,ICF_MEM_COMMON,
            p_mmi_pdb->p_ecode,ret_val)
        }
		else
		
		if(ICF_FAILURE == ret_val)
		{
			ICF_MEMFREE(p_mmi_pdb->p_glb_pdb,p_pdp_ctxts,ICF_MEM_COMMON,
            p_mmi_pdb->p_ecode,ret_val)
			ret_val = ICF_FAILURE;
        }
	}

    ICF_FUNCTION_EXIT(p_mmi_pdb->p_glb_pdb)
    return ret_val; 
}

/******************************************************************************
 *
 * FUNCTION:        icf_mmi_send_sec_pdp_deactiv_req
 *
 * DESCRIPTION:                        
 *                  
 * RETURNS:         ICF_SUCCESS in case of success and ICF_FAILURE in 
 *                  case of any failure.       
 *
 ******************************************************************************/

icf_return_t icf_mmi_send_sec_pdp_deactiv_req(
		INOUT icf_mmi_pdb_st             *p_mmi_pdb)
{
  
   icf_return_t                         ret_val = ICF_SUCCESS;
   icf_internal_msg_st                  *p_nm_internal_msg = ICF_NULL;
   icf_nm_multiple_sec_pdp_deactivate_st msg_payload;
   
    
   ICF_FUNCTION_ENTER(p_mmi_pdb->p_glb_pdb)

   if(ICF_SUCCESS == icf_dbm_get_module_glb_data(
                              p_mmi_pdb->p_glb_pdb,       
                              (icf_module_id_t)ICF_INT_MODULE_NM,
                              (icf_void_t*)(&msg_payload.p_nm_ptr),
                              &(p_mmi_pdb->p_internal_msg->msg_hdr.ecode)))
    {        
		/* Fill the header of the message to be send */
		p_nm_internal_msg = p_mmi_pdb->p_internal_msg;
	    p_nm_internal_msg->msg_hdr.msg_id = ICF_NM_MULTIPLE_SEC_DEACT ;
		p_nm_internal_msg->msg_hdr.payload_length = 
				ICF_PORT_SIZEOF(icf_nm_multiple_sec_pdp_deactivate_st);
		p_nm_internal_msg->msg_hdr.p_glb_pdb = p_mmi_pdb->p_glb_pdb ;
        /* Filling the Call Id in the payload */
		(icf_nm_multiple_sec_pdp_deactivate_st *)p_nm_internal_msg->\
				p_msg_data = &msg_payload;
		msg_payload.call_id = p_mmi_pdb->p_call_ctx->call_id ;
	}
	      
    ICF_MMI_TRACE(p_mmi_pdb,ICF_TRACE_HDR_SEND)
    
     ret_val = icf_nm_process_mesg(p_nm_internal_msg);

            
    ICF_FUNCTION_EXIT(p_mmi_pdb->p_glb_pdb)
    return ret_val; 
} 

/******************************************************************************
 *
 * FUNCTION:        icf_mmi_convert_sdp_m_lines_to_flow_ids
 * 
 * DESCRIPTION:     This function converts SDP M lines to flow ids
 *					It will extract the media parameters from each
 *					node of the media stream list and assign flow
 *					parameters depending on whether media stream is
 *					Sendrecv / Send only / Recv only.
 *					The rule for generating an ip_flow id is :
 *					i) If 2 downlink streams are present, assign 1
 *					to the flow id with lower port number. The 
 *					other is assigned ip_flow_id as 2 .
 *					ii) If two UPLINKS are present, the above rule
 *					hold valid.
 *					iii) If one uplink and one downlink are present,
 *					the downlink stream is given preference and its 
 *					ip_flow_id is assigned 1.
 *                    
 * RETURNS:         ICF_SUCCESS in case of success and ICF_FAILURE in 
 *                  case of any failure.       
 *
 ******************************************************************************/

icf_return_t icf_mmi_convert_sdp_m_lines_to_flow_ids(
        INOUT icf_mmi_pdb_st         *p_mmi_pdb,
        IN    icf_internal_list_st   *p_sdp,
        INOUT icf_call_flow_map_st	 *p_call_flow_map,
		INOUT icf_stream_flow_map_st *p_stream_flow_map)
{
	icf_uint8_t						flow_idx = ICF_NULL;
	icf_uint8_t						flow_dir_idx =ICF_NULL;
	icf_stream_id_t						stream_idx = ICF_NULL;
	icf_internal_sdp_stream_st		*p_stream_local_sdp;
    icf_sdp_stream_st				rem_sdp;
	icf_stream_sdp_info_st			*p_stream_sdp_node = ICF_NULL;
	icf_return_t					ret_val = ICF_SUCCESS; 

	ICF_FUNCTION_ENTER(p_mmi_pdb->p_glb_pdb)

	while(p_sdp)
	{
		icf_port_memset(&rem_sdp,0,ICF_PORT_SIZEOF(rem_sdp));
		
		p_stream_sdp_node = p_sdp->p_payload;

		if(ICF_FAILURE == icf_mmi_check_if_qos_stream(p_mmi_pdb,
			p_stream_sdp_node->stream_id))
		{
			p_sdp = p_sdp->p_next;
			continue;
		}
		
		p_stream_local_sdp = p_stream_sdp_node->p_local_sdp;
		
		/*getting remote SDP */
		icf_mmi_get_sdp_for_stream_id(p_mmi_pdb,p_stream_sdp_node->stream_id,
										ICF_SDP_REMOTE,&rem_sdp);

		p_stream_flow_map[stream_idx].stream_id = p_stream_sdp_node->stream_id;
		p_stream_flow_map[stream_idx].from_flow_idx = flow_idx;

		if((ICF_STREAM_TYPE_AUDIO == p_stream_sdp_node->stream_type) || 
			(ICF_STREAM_TYPE_VIDEO == p_stream_sdp_node->stream_type))
		{
			p_call_flow_map->flow_info[flow_idx].flow_id.media_component_id = 
				p_stream_sdp_node->stream_id;
			
			if(ICF_STREAM_TYPE_AUDIO == p_stream_sdp_node->stream_type)
				p_call_flow_map->flow_info[flow_idx].flow_type = ICF_FLOW_TYPE_AUDIO;
			else
				p_call_flow_map->flow_info[flow_idx].flow_type = ICF_FLOW_TYPE_VIDEO;
			
			p_call_flow_map->flow_info[flow_idx].flow_protocol = ICF_FLOW_PROTOCOL_RTP;
			
			/* ICF_INVALID_FLOW_IDX value denotes that linked_flow_index is not applicable for 
			RTP flow */
			p_call_flow_map->flow_info[flow_idx].linked_flow_index = ICF_INVALID_FLOW_IDX;
			
			if (p_stream_local_sdp->media_mode == ICF_MEDIA_MODE_RECV_ONLY)
			{
				p_call_flow_map->flow_info[flow_idx].num_flow_directions = 1;
				p_call_flow_map->flow_info[flow_idx].flow_direction_info[flow_dir_idx].destination_address =
					p_stream_local_sdp->sdp_stream.stream_address;
				p_call_flow_map->flow_info[flow_idx].flow_direction_info[flow_dir_idx].direction = 
					ICF_FLOW_DIR_DOWNLINK;
			}
			else if (p_stream_local_sdp->media_mode == ICF_MEDIA_MODE_SEND_ONLY)
			{
				p_call_flow_map->flow_info[flow_idx].num_flow_directions = 1;
				p_call_flow_map->flow_info[flow_idx].flow_direction_info[flow_dir_idx].destination_address =
					rem_sdp.stream_address;/*rtp_address*/
				p_call_flow_map->flow_info[flow_idx].flow_direction_info[flow_dir_idx].direction = 
					ICF_FLOW_DIR_UPLINK;
			}
			else if (p_stream_local_sdp->media_mode == ICF_MEDIA_MODE_SEND_RECV)
			{
				p_call_flow_map->flow_info[flow_idx].num_flow_directions = 2;
				p_call_flow_map->flow_info[flow_idx].flow_direction_info[flow_dir_idx].destination_address =
					p_stream_local_sdp->sdp_stream.stream_address;
				p_call_flow_map->flow_info[flow_idx].flow_direction_info[flow_dir_idx].direction = 
					ICF_FLOW_DIR_DOWNLINK;
				flow_dir_idx++;
				p_call_flow_map->flow_info[flow_idx].flow_direction_info[flow_dir_idx].destination_address =
					rem_sdp.stream_address;
				p_call_flow_map->flow_info[flow_idx].flow_direction_info[flow_dir_idx].direction = 
					ICF_FLOW_DIR_UPLINK;
			}
			else /* in case mode is inactive - qos will not be applied to the media line */
			{
				p_call_flow_map->flow_info[flow_idx].num_flow_directions = 0;
			}
			
			/* RTCP flow */
			flow_dir_idx = 0;
			flow_idx++;
			
			p_call_flow_map->flow_info[flow_idx].flow_id.media_component_id = 
				p_stream_sdp_node->stream_id;
			
			p_call_flow_map->flow_info[flow_idx].flow_type = ICF_FLOW_TYPE_NONE;
			
			p_call_flow_map->flow_info[flow_idx].flow_protocol = ICF_FLOW_PROTOCOL_RTCP;
			
			p_call_flow_map->flow_info[flow_idx].linked_flow_index = flow_idx - 1;
			
			p_call_flow_map->flow_info[flow_idx].num_flow_directions = 2;
			
			p_call_flow_map->flow_info[flow_idx].flow_direction_info[flow_dir_idx].destination_address =
				p_stream_local_sdp->sdp_stream.stream_address;
			
			if (p_stream_local_sdp->sdp_stream.rtcp_address.port_num)
			{
				p_call_flow_map->flow_info[flow_idx].flow_direction_info[flow_dir_idx].destination_address.port_num =
					p_stream_local_sdp->sdp_stream.rtcp_address.port_num;
			}
			else
			{
				p_call_flow_map->flow_info[flow_idx].flow_direction_info[flow_dir_idx].destination_address.port_num =
					p_stream_local_sdp->sdp_stream.stream_address.port_num + 1;
			}
			p_call_flow_map->flow_info[flow_idx].flow_direction_info[flow_dir_idx].direction = 
				ICF_FLOW_DIR_DOWNLINK;
			
			flow_dir_idx++;
			
			p_call_flow_map->flow_info[flow_idx].flow_direction_info[flow_dir_idx].destination_address =
				rem_sdp.stream_address;
			
			if (rem_sdp.rtcp_address.port_num) 
			{
				p_call_flow_map->flow_info[flow_idx].flow_direction_info[flow_dir_idx].destination_address.port_num =
					rem_sdp.rtcp_address.port_num;
			}
			else
			{
				p_call_flow_map->flow_info[flow_idx].flow_direction_info[flow_dir_idx].destination_address.port_num =
					rem_sdp.stream_address.port_num + 1;
			}
			p_call_flow_map->flow_info[flow_idx].flow_direction_info[flow_dir_idx].direction = 
				ICF_FLOW_DIR_UPLINK;
			
			/* Compare ports and assign ip_flow_id */ 
			if (p_call_flow_map->flow_info[flow_idx-1].num_flow_directions == 1)
			{
				if (p_call_flow_map->flow_info[flow_idx-1].flow_direction_info[0].direction ==
					ICF_FLOW_DIR_DOWNLINK)
				{
					if(p_call_flow_map->flow_info[flow_idx-1].flow_direction_info[0].destination_address.port_num <
						p_call_flow_map->flow_info[flow_idx].flow_direction_info[0].destination_address.port_num)
					{
						p_call_flow_map->flow_info[flow_idx-1].flow_id.ip_flow_id = 1;
						p_call_flow_map->flow_info[flow_idx].flow_id.ip_flow_id = 2;
					}
					else
					{
						p_call_flow_map->flow_info[flow_idx].flow_id.ip_flow_id = 1;
						p_call_flow_map->flow_info[flow_idx-1].flow_id.ip_flow_id = 2;
					}
					
				}
				else /* uplink */
				{
					if(p_call_flow_map->flow_info[flow_idx-1].flow_direction_info[0].destination_address.port_num <
						p_call_flow_map->flow_info[flow_idx].flow_direction_info[1].destination_address.port_num)
					{
						p_call_flow_map->flow_info[flow_idx-1].flow_id.ip_flow_id = 1;
						p_call_flow_map->flow_info[flow_idx].flow_id.ip_flow_id = 2;
					}
					else
					{
						p_call_flow_map->flow_info[flow_idx].flow_id.ip_flow_id = 1;
						p_call_flow_map->flow_info[flow_idx-1].flow_id.ip_flow_id = 2;
					}
					
				}
			}
			else if (p_call_flow_map->flow_info[flow_idx-1].num_flow_directions == 2)
			{
				if(p_call_flow_map->flow_info[flow_idx-1].flow_direction_info[0].destination_address.port_num <
					p_call_flow_map->flow_info[flow_idx].flow_direction_info[0].destination_address.port_num)
				{
					p_call_flow_map->flow_info[flow_idx-1].flow_id.ip_flow_id = 1;
					p_call_flow_map->flow_info[flow_idx].flow_id.ip_flow_id = 2;
				}
				else
				{
					p_call_flow_map->flow_info[flow_idx].flow_id.ip_flow_id = 1;
					p_call_flow_map->flow_info[flow_idx-1].flow_id.ip_flow_id = 2;
				}
			}

			p_stream_flow_map[stream_idx].to_flow_idx = flow_idx;

			flow_dir_idx = 0;
			flow_idx++;
		}
		else
		{
			ret_val = ICF_FAILURE;
		}

		stream_idx++;

		p_sdp = p_sdp->p_next;
	}
		
	p_call_flow_map->num_flow_ids = flow_idx;

	ICF_FUNCTION_EXIT(p_mmi_pdb->p_glb_pdb)

	return ret_val;

}


/******************************************************************************
 *
 * FUNCTION:        icf_mmi_calculate_max_authorised_traffic_class_per_flow_id
 * 
 * DESCRIPTION:     This function calculates the traffic class for flow ids
 *					already generated. The traffic class depends upon the no
 *					of UPLINKS and DOWNLINKS streams.
 * 					If flow directions (for all media streams) is strictly 
 *					uplink or downlink, it is a STREAMING_TRAFFIC_CLASS.
 * 					If it is both uplink and downlink, then it is a 
 *					CONVERSATIONAL_TRAFFIC_CLASS.
 *
 *                  Note: Interactive class and Background are mainly meant to
 *					be used by traditional Internet applications like WWW, Email,
 *					Telnet, FTP. Telephony applications involving Audio and Video
 *					are delay sensitive and belong to Streaming and Conversational
 *					classes. We will not have Interactive and Background traffic 
 *					class because ICF supports only Real time streams like AUDIO 
 *					and VIDEO.
 *					
 * RETURNS:         ICF_SUCCESS in case of success and ICF_FAILURE in 
 *                  case of any failure.       
 *
 ******************************************************************************/

 icf_return_t icf_mmi_calculate_max_authorised_traffic_class_per_flow_id(
        INOUT icf_mmi_pdb_st			*p_mmi_pdb,
		INOUT icf_call_flow_map_st		*p_call_flow_map)
{
	icf_uint8_t		temp_ul = 0,temp_dl = 0;
	icf_uint8_t		flow_idx = 0,max_traffic_class = 0;
	icf_return_t	ret_val = ICF_SUCCESS;

	ICF_FUNCTION_ENTER(p_mmi_pdb->p_glb_pdb)

	if(ICF_NULL == p_call_flow_map)
	{	
		ret_val = ICF_FAILURE;
	}
	else
	{
		for (flow_idx=0; flow_idx<p_call_flow_map->num_flow_ids; flow_idx++)
		{
			if((ICF_FLOW_TYPE_AUDIO == p_call_flow_map->flow_info[flow_idx].flow_type) ||
				(ICF_FLOW_TYPE_VIDEO == p_call_flow_map->flow_info[flow_idx].flow_type))
			{
				if(1 == p_call_flow_map->flow_info[flow_idx].num_flow_directions) 
				{
					if(ICF_FLOW_DIR_UPLINK == p_call_flow_map->flow_info[flow_idx].flow_direction_info[0].direction)
					{
						temp_ul++;
					}
					else
					{
						temp_dl++;
					}
				}
				else
				{
					max_traffic_class = ICF_QOS_CONVERSATIONAL_TRAFFIC_CLASS;
				}
			}
		}

		if (max_traffic_class == 0)
		{	
			if ((0 == temp_ul) || (0 == temp_dl))
			{
				max_traffic_class = ICF_QOS_STREAMING_TRAFFIC_CLASS;
			}
			else
			{
				max_traffic_class = ICF_QOS_CONVERSATIONAL_TRAFFIC_CLASS;
			}
		}

		/* Among media IP flows only audio and video flows are supported in ICF 6.0 */

		for (flow_idx=0; flow_idx<p_call_flow_map->num_flow_ids; flow_idx++)
		{
			switch (p_call_flow_map->flow_info[flow_idx].flow_type)
			{
				case ICF_FLOW_TYPE_AUDIO: 
					p_call_flow_map->flow_info[flow_idx].max_traffic_class = max_traffic_class;
					break;

				case ICF_FLOW_TYPE_VIDEO: 
					p_call_flow_map->flow_info[flow_idx].max_traffic_class = max_traffic_class;
					break;

/*TBD*/				/* Maximum Authorised QoS Class of RTCP IP flow is same as for corresponding media IP flow */
				case ICF_FLOW_TYPE_NONE: 
					p_call_flow_map->flow_info[flow_idx].max_traffic_class = 
						p_call_flow_map->flow_info[flow_idx-1].max_traffic_class;
					break;
			}
		}
	}

	ICF_FUNCTION_EXIT(p_mmi_pdb->p_glb_pdb)

	return ret_val;
	
}


/******************************************************************************
 *
 * FUNCTION:        icf_mmi_calculate_max_authorised_traffic_class_per_pdp
 * 
 * DESCRIPTION:     This function calculates the maximum traffic class for
 *					the entire PDP. Max Traffic for PDP is the max traffic
 *					class among all the flow ids.
 *
 *					Priority of different traffic classes is as follows :
 *
 *					Conversational > Streaming > Interactive with 
 *					priority 1 > Interactive with priority 2 > Interactive
 *					with priority 3 > Background
 *                    
 * RETURNS:         ICF_SUCCESS in case of success and ICF_FAILURE in 
 *                  case of any failure.       
 *
 ******************************************************************************/

icf_return_t icf_mmi_calculate_max_authorised_traffic_class_per_pdp(
        IN	  icf_stream_id_t			 num_of_m_lines,
		INOUT icf_mmi_pdb_st         *p_mmi_pdb,
		INOUT icf_call_flow_map_st   *p_call_flow_map,
		INOUT icf_stream_flow_map_st *p_stream_flow_map,
		INOUT icf_nm_pdp_context_st  *p_pdp_ctxts)
{
	icf_uint8_t		max_traffic_class = 0;
	icf_stream_id_t		stream_idx = 0;
	icf_uint8_t		flow_idx = 0;
	icf_return_t	ret_val = ICF_SUCCESS;

	ICF_FUNCTION_ENTER(p_mmi_pdb->p_glb_pdb)

	if((ICF_NULL == p_call_flow_map) ||
	   (ICF_NULL == p_stream_flow_map))
	{	
		ret_val = ICF_FAILURE;
	}
	else
	{
		for(stream_idx=0; stream_idx < num_of_m_lines; stream_idx++)
		{
			flow_idx = p_stream_flow_map[stream_idx].from_flow_idx;

			max_traffic_class = p_call_flow_map->flow_info[flow_idx].max_traffic_class;
			
			for(; flow_idx<=p_stream_flow_map[stream_idx].to_flow_idx; flow_idx++)
			{
				if (p_call_flow_map->flow_info[flow_idx].max_traffic_class > max_traffic_class)
				{
					max_traffic_class = p_call_flow_map->flow_info[flow_idx].max_traffic_class;
				}
			}

			p_pdp_ctxts[stream_idx].qos.traffic_class = max_traffic_class;
			/* traffic_handling_priority is applicable only in case traffic class is interactive */
			/* traffic_class is interactive in case of media types - data/control */
			/* ICF handles only media types = audio/video */
			p_pdp_ctxts[stream_idx].qos.traffic_handling_priority = 0;
		}
	}

	ICF_FUNCTION_EXIT(p_mmi_pdb->p_glb_pdb)

	return ret_val;
}
 
/******************************************************************************
 *
 * FUNCTION:        icf_mmi_calculate_max_authorised_bit_rate_per_flow_id
 * 
 * DESCRIPTION:     This function calculates the bandwidth per flow id. For
 *					uplink streams, the downlink bandwidth will be 0 while 
 *					for downlink streams, the uplink bandwidth will be 0.
 *					For streams with media mode as sendrecv, the bandwidth
 *					in up/down directions will be equal.
 *					For RTCP flow, the uplink and downlink bandwidth will 
 *					be 5% of the bandwidth available for RTP flows.
 *
 * RETURNS:         ICF_SUCCESS in case of success and ICF_FAILURE in 
 *                  case of any failure.       
 *
 ******************************************************************************/

icf_return_t icf_mmi_calculate_max_authorised_bit_rate_per_flow_id(
        INOUT icf_mmi_pdb_st			*p_mmi_pdb,
		INOUT	  icf_stream_flow_map_st	*p_stream_flow_map,
		INOUT icf_call_flow_map_st		*p_call_flow_map)
{

	icf_uint8_t							flow_idx = 0;/*,count=0;*/
	icf_uint16_t						bandwidth = ICF_NULL;
	icf_return_t						ret_val = ICF_SUCCESS;

	ICF_FUNCTION_ENTER(p_mmi_pdb->p_glb_pdb)

	if(ICF_NULL == p_call_flow_map)
	{	
		ret_val = ICF_FAILURE;
	}
	else
	{
		/* Granularity of 1 kbps assumed for bit rates */
		for (flow_idx=0; flow_idx<p_call_flow_map->num_flow_ids; flow_idx++)
		{

			if(ICF_FAILURE == icf_mmi_get_bandwidth_for_flow_id(p_mmi_pdb,p_stream_flow_map, 
					flow_idx, &bandwidth))
			{
				if(ICF_FLOW_TYPE_AUDIO == p_call_flow_map->flow_info[flow_idx].flow_type)
				{
					bandwidth = ICF_QOS_AUDIO_CODEC_BANDWIDTH;
				}
				else if(ICF_FLOW_TYPE_VIDEO == p_call_flow_map->flow_info[flow_idx].flow_type)
				{
					bandwidth = ICF_QOS_VIDEO_CODEC_BANDWIDTH;
				}
			}
				
			/* media IP flow */
			if(p_call_flow_map->flow_info[flow_idx].flow_protocol == ICF_FLOW_PROTOCOL_RTP) 
			{
				/* b=AS: present */
				if(bandwidth != ICF_NULL) 
				{
					if((1 == p_call_flow_map->flow_info[flow_idx].num_flow_directions) &&
						(ICF_FLOW_DIR_UPLINK == p_call_flow_map->flow_info[flow_idx].flow_direction_info[0].direction))
					{
						p_call_flow_map->flow_info[flow_idx].max_bit_rate_ul = bandwidth;
						p_call_flow_map->flow_info[flow_idx].max_bit_rate_dl = 0;
					}
					else if((1 == p_call_flow_map->flow_info[flow_idx].num_flow_directions) &&
						(ICF_FLOW_DIR_DOWNLINK == p_call_flow_map->flow_info[flow_idx].flow_direction_info[0].direction))
					{
						p_call_flow_map->flow_info[flow_idx].max_bit_rate_ul = 0;
						p_call_flow_map->flow_info[flow_idx].max_bit_rate_dl = bandwidth;
					}
					else if(2 == p_call_flow_map->flow_info[flow_idx].num_flow_directions)
					{
						p_call_flow_map->flow_info[flow_idx].max_bit_rate_ul = bandwidth;
						p_call_flow_map->flow_info[flow_idx].max_bit_rate_dl = bandwidth;
					}
				}
			}
			/* RTCP IP flow */
			else 
			{
				/* Default values of b=RS: and b=RR:
				 * The total RTCP bandwidth is 5% of the maximum data bandwidth(b=AS:)
				 * If one of these RTCP bandwidth specifiers is omitted, its value is 5%
				 * minus the value of the other one, but not less than zero. 
				 * If both are omitted, the sender and receiver RTCP bandwidths are 1.25%
				 * and 3.75% of the session bandwidth, respectively.
				 *
				 * For bandwidth modifiers RS: and RR:, <bandwidth-value> is in bits/sec
				 * For all other bandwidth modifiers, <bandwidth-value> is in kilobits/sec
				 * 
				 * b=RS: = (1.25/100) * b=AS: * 1000
				 * b=RR: = (3.75/100) * b=AS: * 1000
				 * 
				 * Maximum bandwidth for RTCP flow 
				 * Max_BW_UL:= (b=RS: + b=RR:) / 1000;
				 * Max_BW_DL:= (b=RS: + b=RR:) / 1000;
				 */

				p_call_flow_map->flow_info[flow_idx].max_bit_rate_ul = 
					(icf_uint16_t)(0.05 * bandwidth * 1000);
				p_call_flow_map->flow_info[flow_idx].max_bit_rate_dl = 
					(icf_uint16_t)(0.05 * bandwidth * 1000);
			}
		}
	}

	ICF_FUNCTION_EXIT(p_mmi_pdb->p_glb_pdb)

	return ret_val;
}


/******************************************************************************
 *
 * FUNCTION:        icf_mmi_calculate_max_authorised_bit_rate_per_pdp
 * 
 * DESCRIPTION:     This function calculates the maximum bit rate per PDP.
 *					Maximum bit rate for uplink / downlink is to be used 
 *					in case the Traffic Class is Interactive or Background.
 *					Guaranteed bit rate for uplink, Guaranteed bit rate for
 * 					downlink is to be used in case the Traffic Class is 
 *					Conversational or Streaming.
 *                    
 * RETURNS:         ICF_SUCCESS in case of success and ICF_FAILURE in 
 *                  case of any failure.       
 *
 ******************************************************************************/

icf_return_t icf_mmi_calculate_max_authorised_bit_rate_per_pdp(
		IN	  icf_stream_id_t 			 num_of_m_lines,
        INOUT icf_mmi_pdb_st         *p_mmi_pdb,
		INOUT icf_call_flow_map_st   *p_call_flow_map,
		INOUT icf_stream_flow_map_st *p_stream_flow_map,
		INOUT icf_nm_pdp_context_st  *p_pdp_ctxts)
{
	icf_uint16_t					max_bit_rate_ul = 0;
	icf_uint16_t					max_bit_rate_dl = 0;
	icf_stream_id_t						stream_idx = 0;
	icf_uint8_t						flow_idx = 0;
	icf_return_t					ret_val = ICF_SUCCESS;

	ICF_FUNCTION_ENTER(p_mmi_pdb->p_glb_pdb)

	if((ICF_NULL == p_call_flow_map) ||
	   (ICF_NULL == p_stream_flow_map))
	{	
		ret_val = ICF_FAILURE;
	}
	else
	{
		for(stream_idx=0; stream_idx < num_of_m_lines; stream_idx++)
		{
			flow_idx = p_stream_flow_map[stream_idx].from_flow_idx;

			max_bit_rate_ul = 0;
			max_bit_rate_dl = 0;
			
			for(; flow_idx<=p_stream_flow_map[stream_idx].to_flow_idx; flow_idx++)
			{
				max_bit_rate_ul +=	p_call_flow_map->flow_info[flow_idx].max_bit_rate_ul;
				max_bit_rate_dl +=	p_call_flow_map->flow_info[flow_idx].max_bit_rate_dl;
			}

	/* Maximum bit rate for uplink, Maximum bit rate for downlink is to be used in case 
	 * the Traffic Class is Interactive or Background
	 * Guaranteed bit rate for uplink, Guaranteed bit rate for downlink is to be used in case 
	 * the Traffic Class is Conversational or Streaming */
		
			if ((ICF_QOS_INTERACTIVE_TRAFFIC_CLASS == p_pdp_ctxts[stream_idx].qos.traffic_class) ||
				(ICF_QOS_BACKGROUND_TRAFFIC_CLASS == p_pdp_ctxts[stream_idx].qos.traffic_class))
			{
				icf_mmi_encode_bit_rate(max_bit_rate_ul, &p_pdp_ctxts[stream_idx].qos.maximum_bit_rate_ul);
				icf_mmi_encode_bit_rate(max_bit_rate_dl, &p_pdp_ctxts[stream_idx].qos.maximum_bit_rate_dl);
				icf_mmi_encode_bit_rate(0, &p_pdp_ctxts[stream_idx].qos.guaranteed_bit_rate_ul);
				icf_mmi_encode_bit_rate(0, &p_pdp_ctxts[stream_idx].qos.guaranteed_bit_rate_dl);
			}
			
			if ((ICF_QOS_CONVERSATIONAL_TRAFFIC_CLASS == p_pdp_ctxts[stream_idx].qos.traffic_class) ||
				(ICF_QOS_STREAMING_TRAFFIC_CLASS == p_pdp_ctxts[stream_idx].qos.traffic_class))
			{
				icf_mmi_encode_bit_rate(0, &p_pdp_ctxts[stream_idx].qos.maximum_bit_rate_ul);
				icf_mmi_encode_bit_rate(0, &p_pdp_ctxts[stream_idx].qos.maximum_bit_rate_dl);
				icf_mmi_encode_bit_rate(max_bit_rate_ul, &p_pdp_ctxts[stream_idx].qos.guaranteed_bit_rate_ul);
				icf_mmi_encode_bit_rate(max_bit_rate_dl, &p_pdp_ctxts[stream_idx].qos.guaranteed_bit_rate_dl);
			}		

		}
	}

	ICF_FUNCTION_EXIT(p_mmi_pdb->p_glb_pdb)

	return ret_val;
}


/******************************************************************************
 *
 * FUNCTION:        icf_mmi_encode_bit_rate
 * 
 * DESCRIPTION:     This function is used to calculate the encoded bit rate
 *					from the actual bit rate.
 *					
 *					The rule for calculating the bit rate is :
 *					
 *					1) The maximum bit rate is binary coded in 8 bits, using a 
 *					granularity of 1 kbps giving a range of values from 1 kbps
 *					to 63 kbps in 1 kbps increments.
 *					
 *					2) The maximum bit rate is 64 kbps + ((the binary coded value 
 *					in 8 bits -01000000) * 8 kbps) giving a range of values from
 *					64 kbps to 568 kbps in 8 kbps increments.
 *
 *					3) The maximum bit rate is 576 kbps + ((the binary coded value
 *					in 8 bits -10000000) * 64 kbps) giving a range of values from
 *					576 kbps to 8640 kbps in 64 kbps increments.
 *
 *					The above rule can be referenced in document no. 3GPP TS 24.008
 *					under section 10.5.6.5 and table no 10.5.156.
 *                    
 * RETURNS:         Encoded bit rate as calculated from the rule.
 *
 ******************************************************************************/

icf_void_t icf_mmi_encode_bit_rate(icf_uint16_t bit_rate, icf_uint16_t *encoded_val)
{
	icf_uint16_t	temp = 0;

	temp = bit_rate;

	if (bit_rate > 575)
	{
		temp = (bit_rate - 576)/64;
		temp = temp + 128;
	}
	else if (bit_rate > 63)
	{
		temp = (bit_rate - 64)/8;
		temp = temp + 64;
	}
	else if (bit_rate == 0)
	{
		temp = 255;
	}

	*encoded_val = temp;

}

/******************************************************************************
 *
 * FUNCTION:        icf_mmi_populate_qos_per_pdp
 * 
 * DESCRIPTION:     This function popoulates the QoS information element
 *					for each media stream. It uses pre-defined values for
 *					error ratio and transfer delay for both AUDIO and VIDEO
 *					streams as stated in document no. 3GPP TS 26.236.
 *                    
 * RETURNS:         ICF_SUCCESS in case of success and ICF_FAILURE in 
 *                  case of any failure.       
 *
 ******************************************************************************/

icf_return_t icf_mmi_populate_qos_per_pdp(
		IN	  icf_stream_id_t		 num_of_m_lines,
        INOUT icf_mmi_pdb_st         *p_mmi_pdb,
		INOUT icf_call_flow_map_st   *p_call_flow_map,
		INOUT icf_stream_flow_map_st *p_stream_flow_map,
		INOUT icf_nm_pdp_context_st  *p_pdp_ctxts)
{
	icf_stream_id_t					stream_idx = 0;
	icf_uint8_t						flow_idx = 0;
	icf_return_t					ret_val = ICF_SUCCESS;

	ICF_FUNCTION_ENTER(p_mmi_pdb->p_glb_pdb)

	if((ICF_NULL == p_call_flow_map) ||
	   (ICF_NULL == p_stream_flow_map))
	{	
		ret_val = ICF_FAILURE;
	}
	else
	{
		for(stream_idx=0; stream_idx < num_of_m_lines; stream_idx++)
		{
			flow_idx = p_stream_flow_map[stream_idx].from_flow_idx;
			
			/* Maximum SDU size : 130 bytes audio, 500 bytes video */
			/* SDU error ratio : 7*10pow-3 - audio, 10 pow-3 - video */
			/* Transfer delay : 100 ms - audio/bidirectional video, 250 ms - unidirectional video */
			if(p_call_flow_map->flow_info[flow_idx].flow_type == ICF_FLOW_TYPE_AUDIO)
			{
				p_pdp_ctxts[stream_idx].qos.max_sdu_size = 130;
				p_pdp_ctxts[stream_idx].qos.sdu_error_ratio = 0x02; /* 7*10pow-3 is encoded as binary 0010 */
				p_pdp_ctxts[stream_idx].qos.transfer_delay=10; /* 100ms is encoded as 10 with granularity is 10ms */
			}
			else if(p_call_flow_map->flow_info[flow_idx].flow_type == ICF_FLOW_TYPE_VIDEO)
			{
				p_pdp_ctxts[stream_idx].qos.max_sdu_size = 500;
				p_pdp_ctxts[stream_idx].qos.sdu_error_ratio = 0x03; /* 10pow-3 is coded as binary 0011 */
				if (p_call_flow_map->flow_info[flow_idx].num_flow_directions == 2)
				{
					p_pdp_ctxts[stream_idx].qos.transfer_delay=10; /* 100ms is encoded as 10 with granularity is 10ms */
				}
				else if (p_call_flow_map->flow_info[flow_idx].num_flow_directions == 1)
				{
					/* 250 is encoded as 010001 ~ (200 + (010001 - 010000) * 50 = 250 ms) */ 
					p_pdp_ctxts[stream_idx].qos.transfer_delay=17;
				}
			}
			
			p_pdp_ctxts[stream_idx].qos.delivery_order=2;	/* binary 10 signifies "Without Delivery Order" */
			p_pdp_ctxts[stream_idx].qos.delivery_of_erroneous_sdu=1; /* binary 001 signifies "No Detect" */
			p_pdp_ctxts[stream_idx].qos.residual_ber = 0x07; /* 10pow-5 is coded as binary 0111 */ 
			p_pdp_ctxts[stream_idx].qos.signaling_indication = 0; 	/* Carries Media so signaling flag is off */ 
			p_pdp_ctxts[stream_idx].qos.source_statistics_descriptor = 0; /* Unknown to be used */ 

		}
	}

	ICF_FUNCTION_EXIT(p_mmi_pdb->p_glb_pdb)
		
	return ret_val;
}



/******************************************************************************
 *
 * FUNCTION:        icf_mmi_populate_tft_per_pdp
 * 
 * DESCRIPTION:     This function populates the TFT Information element
 *					per PDP for each media stream.
 *                    
 * RETURNS:         ICF_SUCCESS in case of success and ICF_FAILURE in 
 *                  case of any failure.       
 *
 ******************************************************************************/

icf_return_t icf_mmi_populate_tft_per_pdp(
		IN    icf_stream_id_t		 num_of_m_lines,
        INOUT icf_mmi_pdb_st         *p_mmi_pdb,
        IN 	  icf_nm_auth_token_st   *p_media_auth_token,
		INOUT icf_call_flow_map_st   *p_call_flow_map,
		INOUT icf_stream_flow_map_st *p_stream_flow_map,
		INOUT icf_nm_pdp_context_st  *p_pdp_ctxts)
{
	icf_stream_id_t					stream_idx = 0;
/*	icf_uint8_t						flow_idx = 0;*/
	icf_return_t					ret_val = ICF_SUCCESS;

	ICF_FUNCTION_ENTER(p_mmi_pdb->p_glb_pdb)

	if((ICF_NULL == p_call_flow_map) ||
	   (ICF_NULL == p_stream_flow_map))
	{	
		ret_val= ICF_FAILURE;
	}
	else
	{
		for(stream_idx=0; stream_idx < num_of_m_lines; stream_idx++)
		{
			p_pdp_ctxts[stream_idx].tft.tft_operation_code = ICF_PDP_CREATE_NEW_TFT;
			p_pdp_ctxts[stream_idx].tft.e_bit=1;
		}
		
		ret_val = icf_mmi_populate_tft_pkt_filter_list_per_pdp(
				   num_of_m_lines,	   
				   p_mmi_pdb,
				   p_call_flow_map,
				   p_stream_flow_map,
				   p_pdp_ctxts);

		ret_val = icf_mmi_populate_tft_parameter_list_per_pdp(
				   num_of_m_lines,
				   p_mmi_pdb,
				   p_media_auth_token,
				   p_call_flow_map,
				   p_stream_flow_map,
				   p_pdp_ctxts);
	}

	ICF_FUNCTION_EXIT(p_mmi_pdb->p_glb_pdb)

	return ret_val;
}


/******************************************************************************
 *
 * FUNCTION:        icf_mmi_populate_tft_pkt_filter_list_per_pdp
 * 
 * DESCRIPTION:     This function populates the packet flter list per PDP.
 *					The packet filter list contains a variable number of 
 *					packet filters. Each packet filter is of variable length
 *					and consists of 
 *					-	a packet filter identifier (1 octet);
 *					-	a packet filter evaluation precedence (1 octet);
 *					- 	the length of the packet filter contents (1 octet); and
 *					-	the packet filter contents itself (v octets).
 *					The packet filter identifier field is used to identify each
 *					packet filter in a TFT. The packet filter evaluation 
 *					precedence field is used to specify the precedence for the 
 *					packet filter among all packet filters in all TFTs associated
 *					with this PDP address. Higher the value of the packet filter 
 *					evaluation precedence field, lower is the precedence of that 
 *					packet filter.
 *                    
 * RETURNS:         ICF_SUCCESS in case of success and ICF_FAILURE in 
 *                  case of any failure.       
 *
 ******************************************************************************/

icf_return_t icf_mmi_populate_tft_pkt_filter_list_per_pdp(
		IN    icf_stream_id_t		 num_of_m_lines,
        INOUT icf_mmi_pdb_st         *p_mmi_pdb,
		INOUT icf_call_flow_map_st   *p_call_flow_map,
		INOUT icf_stream_flow_map_st *p_stream_flow_map,
		INOUT icf_nm_pdp_context_st  *p_pdp_ctxts)
{
	icf_stream_id_t					stream_idx = 0;
	icf_uint8_t						flow_idx = 0;
	icf_uint8_t						filter_idx = 0;
	icf_return_t					ret_val = ICF_SUCCESS;

	ICF_FUNCTION_ENTER(p_mmi_pdb->p_glb_pdb)

	for(stream_idx=0; stream_idx < num_of_m_lines; stream_idx++)
	{
		flow_idx = p_stream_flow_map[stream_idx].from_flow_idx;
		filter_idx = 0;

		for(; flow_idx<=p_stream_flow_map[stream_idx].to_flow_idx; flow_idx++)
		{
			p_pdp_ctxts[stream_idx].tft.pkt_filter_list[filter_idx].pkt_filter_id = filter_idx + 1;

			/* Maximum value <=> NULL precedence */ 
			p_pdp_ctxts[stream_idx].tft.pkt_filter_list[filter_idx].pkt_filter_eval_prec = 255; 

			p_pdp_ctxts[stream_idx].tft.pkt_filter_list[filter_idx].pkt_filter_contents[flow_idx].pkt_filter_component_type = 
				ICF_TFT_COMPONENT_SINGLE_DESTINATION_PORT_TYPE;

			if (p_call_flow_map->flow_info[flow_idx].num_flow_directions == 1)
			{
				p_pdp_ctxts[stream_idx].tft.pkt_filter_list[filter_idx].pkt_filter_contents[flow_idx].pkt_filter_component_value = 
				p_call_flow_map->flow_info[flow_idx].flow_direction_info[0].destination_address.port_num;
			}

			if (p_call_flow_map->flow_info[flow_idx].num_flow_directions == 2)
			{
				p_pdp_ctxts[stream_idx].tft.pkt_filter_list[filter_idx].pkt_filter_contents[flow_idx].pkt_filter_component_value = 
				p_call_flow_map->flow_info[flow_idx].flow_direction_info[0].destination_address.port_num;

				filter_idx++;

				p_pdp_ctxts[stream_idx].tft.pkt_filter_list[filter_idx].pkt_filter_id = filter_idx + 1;

				/* Maximum value <=> NULL precedence */ 
				p_pdp_ctxts[stream_idx].tft.pkt_filter_list[filter_idx].pkt_filter_eval_prec = 255; 

				p_pdp_ctxts[stream_idx].tft.pkt_filter_list[filter_idx].pkt_filter_contents[flow_idx].pkt_filter_component_type = 
					ICF_TFT_COMPONENT_SINGLE_DESTINATION_PORT_TYPE;

				p_pdp_ctxts[stream_idx].tft.pkt_filter_list[filter_idx].pkt_filter_contents[flow_idx].pkt_filter_component_value = 
				p_call_flow_map->flow_info[flow_idx].flow_direction_info[1].destination_address.port_num;
			}

			filter_idx++;
		}
		
		p_pdp_ctxts[stream_idx].tft.tft_operation_code = ICF_PDP_CREATE_NEW_TFT;
		p_pdp_ctxts[stream_idx].tft.e_bit=1;
		p_pdp_ctxts[stream_idx].tft.num_pkt_filters = filter_idx;
	}
	ICF_FUNCTION_EXIT(p_mmi_pdb->p_glb_pdb)
	return ret_val;
}

/******************************************************************************
 *
 * FUNCTION:        icf_mmi_populate_tft_parameter_list_per_pdp
 * 
 * DESCRIPTION:     This function is used to fill the parameter list for
					each PDP. Each parameter included in the parameters 
					list is of variable length and consists of:
					-	a parameter identifier (1 octet);
					-	the length of the parameter contents (1 octet); and
					-	the parameter contents itself (v octets).
					The parameter identifier field is used to identify each 
					parameter included in the parameters list and it contains
					the hexadecimal coding of the parameter identifier. 
					
 * RETURNS:         ICF_SUCCESS in case of success and ICF_FAILURE in 
 *                  case of any failure.       
 *
 ******************************************************************************/

icf_return_t icf_mmi_populate_tft_parameter_list_per_pdp(
		IN    icf_stream_id_t		 num_of_m_lines,
        INOUT icf_mmi_pdb_st         *p_mmi_pdb,
        IN 	  icf_nm_auth_token_st   *p_media_auth_token,
		INOUT icf_call_flow_map_st   *p_call_flow_map,
		INOUT icf_stream_flow_map_st *p_stream_flow_map,
		INOUT icf_nm_pdp_context_st  *p_pdp_ctxts)
{
	icf_stream_id_t					stream_idx = 0;
	icf_uint8_t						flow_idx = 0;
	icf_uint8_t						param_idx = 0;
	icf_return_t					ret_val = ICF_SUCCESS;

	ICF_FUNCTION_ENTER(p_mmi_pdb->p_glb_pdb)

	for(stream_idx=0; stream_idx < num_of_m_lines; stream_idx++)
	{
		param_idx = 0;
		flow_idx = p_stream_flow_map[stream_idx].from_flow_idx;

		if(ICF_NULL!=p_media_auth_token)
		{
			p_pdp_ctxts[stream_idx].tft.params_list[param_idx].parameter_identifier = ICF_PDP_PARAM_IDENT_AUTH_TOKEN;
			p_pdp_ctxts[stream_idx].tft.params_list[param_idx].parameter.auth_token = *p_media_auth_token;
			param_idx++;
		}
		
		for(; flow_idx<=p_stream_flow_map[stream_idx].to_flow_idx; flow_idx++)
		{
			p_pdp_ctxts[stream_idx].tft.params_list[param_idx].parameter_identifier = ICF_PDP_PARAM_IDENT_FLOW_ID;
			p_pdp_ctxts[stream_idx].tft.params_list[param_idx].parameter.flow_id.media_component_id =
				p_call_flow_map->flow_info[flow_idx].flow_id.media_component_id;
			p_pdp_ctxts[stream_idx].tft.params_list[param_idx].parameter.flow_id.ip_flow_id =
				p_call_flow_map->flow_info[flow_idx].flow_id.ip_flow_id;
			param_idx++;
		}

		p_pdp_ctxts[stream_idx].tft.num_params = param_idx;
	}

	ICF_FUNCTION_EXIT(p_mmi_pdb->p_glb_pdb)

	return ret_val;
}

/******************************************************************************
 *
 * FUNCTION:    icf_mmi_qos_map_sdp_params_to_sec_pdps
 *
 * DESCRIPTION: This function is invoked when the MMI wants 
 *              to convert sdp params to UMTS QOS params. One secondary PDP is 
 *				activated per m-line.
 * PARAMETERS : 
 *				p_media_auth_token - Pointer Authorization Token received in 
 *				P-Media-Authorization-Token
 *              p_icf_local_sdp  - Pointer to Local Sdp structure 
 *              p_icf_remote_sdp  - Pointer to Remote Sdp structure 
 *			    p_icf_remote_sdp -  Pointer to Number of PDP Context 
 *				variable passed from calling module
 *			    icf_nm_pdp_context_st - Pointer to Block of PDP Contexts. 
 *				Allocation done within this function.Must be NULL initially
 *
 *  RETURNS:    ICF_SUCCESS in case of success and ICF_FAILURE in 
 *				case of any failure.In case of Success this function returns
 *				the number of PDP Contexts and the pointer to the starting
 *				block of pdp context array.The memory Allocation is done inside
 *				this function and the calling entity must free this memory once
 *				it is through with the pdp context structure.
 *
 *****************************************************************************/

icf_return_t  icf_mmi_qos_map_sdp_params_to_sec_pdps(
        INOUT icf_mmi_pdb_st         *p_mmi_pdb,
        INOUT icf_nm_pdp_context_st  *p_pdp_ctxts)
{
 	icf_return_t					ret_val = ICF_SUCCESS;
	icf_internal_list_st			*p_stream_sdp = p_mmi_pdb->p_call_ctx->p_stream_sdp_info;
	icf_call_flow_map_st			*p_call_flow_map = ICF_NULL;
	icf_stream_flow_map_st			*p_stream_flow_map = ICF_NULL;
	icf_stream_id_t	 			num_of_m_lines = ICF_NULL;

	ICF_FUNCTION_ENTER(p_mmi_pdb->p_glb_pdb)
		
	if((ICF_NULL == p_mmi_pdb)	 || 
	   (ICF_NULL == p_stream_sdp))
	{	
		ret_val = ICF_FAILURE;
	}

	num_of_m_lines = p_mmi_pdb->p_mmi_ctx->pdp_count;

	ICF_MEMGET(p_mmi_pdb->p_glb_pdb, ICF_PORT_SIZEOF(icf_call_flow_map_st),
    			  ICF_MEM_COMMON, p_call_flow_map,
				  ICF_RET_ON_FAILURE, p_mmi_pdb->p_ecode, ret_val)

	ICF_MEMGET(p_mmi_pdb->p_glb_pdb, num_of_m_lines * ICF_PORT_SIZEOF(icf_stream_flow_map_st),
    			  ICF_MEM_COMMON, p_stream_flow_map,
				  ICF_RET_ON_FAILURE, p_mmi_pdb->p_ecode, ret_val)

	/* Memory Allocation Success */
	if((ICF_SUCCESS == ret_val) && (num_of_m_lines != ICF_NULL))
	{
		if(ICF_FAILURE == icf_mmi_convert_sdp_m_lines_to_flow_ids(
							p_mmi_pdb,
							p_stream_sdp,
							p_call_flow_map,
							p_stream_flow_map))
		{
			ret_val = ICF_FAILURE;
		}
		else if(ICF_FAILURE == icf_mmi_calculate_max_authorised_traffic_class_per_flow_id(
					p_mmi_pdb,
					p_call_flow_map))
		{
			ret_val = ICF_FAILURE;
		}
		else if(ICF_FAILURE == icf_mmi_calculate_max_authorised_traffic_class_per_pdp(
					num_of_m_lines,
					p_mmi_pdb,
					p_call_flow_map,
					p_stream_flow_map,
					p_pdp_ctxts))
		{
			ret_val = ICF_FAILURE;
		}
		else if(ICF_FAILURE == icf_mmi_calculate_max_authorised_bit_rate_per_flow_id(
					p_mmi_pdb,
					p_stream_flow_map,
					p_call_flow_map))
		{
			ret_val = ICF_FAILURE;
		}
		else if(ICF_FAILURE == icf_mmi_calculate_max_authorised_bit_rate_per_pdp(
					num_of_m_lines,
					p_mmi_pdb,
					p_call_flow_map,
					p_stream_flow_map,
					p_pdp_ctxts))
		{
			ret_val = ICF_FAILURE;
		}
		else if(ICF_FAILURE ==  icf_mmi_populate_qos_per_pdp(
					num_of_m_lines,
					p_mmi_pdb,
					p_call_flow_map,
					p_stream_flow_map,
					p_pdp_ctxts))
		{
			ret_val = ICF_FAILURE;
		}
		else if(ICF_FAILURE == icf_mmi_populate_tft_per_pdp(
					num_of_m_lines,
					p_mmi_pdb,
					p_mmi_pdb->p_call_ctx->p_auth_token,
					p_call_flow_map,
					p_stream_flow_map,
					p_pdp_ctxts))
		{
			ret_val = ICF_FAILURE;
		}
	}   

	ICF_MEMFREE(p_mmi_pdb->p_glb_pdb,p_call_flow_map,ICF_MEM_COMMON,
                p_mmi_pdb->p_ecode,ret_val)

	ICF_MEMFREE(p_mmi_pdb->p_glb_pdb,p_stream_flow_map,ICF_MEM_COMMON,
                p_mmi_pdb->p_ecode,ret_val)
		
	ICF_FUNCTION_EXIT(p_mmi_pdb->p_glb_pdb)

 	return ret_val;   
}
#endif
#endif
#endif

/* ICF REL 6.0: New Functions start */
/****************************************************************************
 * Function: icf_mmi_negotiate_codecs_per_stream
 * DESCRIPTION: This function returns success in case any codec matches in
 *				remote SDP and local SDP for a particular stream
 *
 ***************************************************************************/

icf_return_t	icf_mmi_negotiate_codecs_per_stream
(
	INOUT	icf_mmi_pdb_st			*p_mmi_pdb,
	IN		icf_stream_id_t			stream_id
)
{
    icf_return_t		ret_val = ICF_FAILURE;
    p_mmi_pdb=p_mmi_pdb;
    
    if(stream_id==stream_id)
    {
       ret_val = ICF_SUCCESS;
    }
	return ret_val;
}


/******************************************************************************
 *
 * FUNCTION:    icf_mmi_compare_codecs_value
 *
 * DESCRIPTION: This function returns success in case two codecs match
 *****************************************************************************/

icf_return_t	icf_mmi_compare_codecs_value
(
	icf_mmi_pdb_st			*p_mmi_pdb,
	icf_codec_attrib_st		*p_codec1,
	icf_codec_attrib_st		*p_codec2
)
{
	icf_return_t		ret_val = ICF_FAILURE;

	if(ICF_NULL == p_mmi_pdb)
    	{
       	   return ICF_FAILURE;
    	}	
	ICF_FUNCTION_ENTER(p_mmi_pdb->p_glb_pdb)

	if(96 <= p_codec1->codec_num)
	{
		if(0 == icf_port_strcasecmp((const icf_int8_t *)p_codec1->codec_name.str, 
									(const icf_int8_t *)p_codec2->codec_name.str))
		{
			ret_val = ICF_SUCCESS;
		}
		/* Since "AMR" and "AMRNB" are same codecs */
		else
		{
			if
			(
				(
					(0 == icf_port_strcasecmp((const icf_int8_t *)p_codec1->codec_name.str,"AMR"))||
					(0 == icf_port_strcasecmp((const icf_int8_t *)p_codec1->codec_name.str,"AMRNB"))
				)&&
				(
					(0 == icf_port_strcasecmp((const icf_int8_t *)p_codec2->codec_name.str,"AMR"))||
					(0 == icf_port_strcasecmp((const icf_int8_t *)p_codec2->codec_name.str,"AMRNB"))
				)
			)
			{
				ret_val = ICF_SUCCESS;
			}
		}
	}
	else 
	if(p_codec1->codec_num == p_codec2->codec_num)
	{
		ret_val = ICF_SUCCESS;
	}

	ICF_FUNCTION_EXIT(p_mmi_pdb->p_glb_pdb)

	return ret_val;
}


/******************************************************************************
 *
 * FUNCTION:    icf_mmi_process_mms_delete_action
 *
 * DESCRIPTION: The function is invoked to send DMS request for the streams
 * 				that need to be deleted as per the request from the peer (in its
 * 				remote offer/answer) or as requested by the application
 * 				
 *****************************************************************************/

icf_return_t	icf_mmi_process_mms_delete_action
(
	icf_mmi_pdb_st		*p_mmi_pdb
)
{
	icf_return_t				ret_val = ICF_SUCCESS;

	ICF_FUNCTION_ENTER(p_mmi_pdb->p_glb_pdb)

	p_mmi_pdb->p_mmi_ctx->ongoing_event = ICF_MMI_MMS_DEL_ONGOING;

	ret_val = icf_mmi_send_dms_req(p_mmi_pdb);

	if(ICF_SUCCESS == ret_val)
	{
		ICF_MMI_SET_STATE(p_mmi_pdb,ICF_MMI_05_AWT_DMS_RESP)
		ICF_MMI_TRACE(p_mmi_pdb,ICF_TRACE_API_SEND)
	}
	else
	{
		icf_mmi_send_rel_res_ind(p_mmi_pdb);

		ICF_MMI_PDB_SET_BIT(p_mmi_pdb,ICF_MMI_PDB_ALL_STREAMS)
		icf_mmi_send_close_channel_ind(p_mmi_pdb); /* send close channel for all streams
													* including TBCP if the stream is set
													* in the DEL action of the MMS requets */
		ICF_MMI_PDB_RESET_BIT(p_mmi_pdb,ICF_MMI_PDB_ALL_STREAMS)


#ifdef ICF_NAT_MEDIA_TRAVERSAL
		icf_mmi_rel_nat_resources(p_mmi_pdb);
#endif

	}

	ICF_FUNCTION_EXIT(p_mmi_pdb->p_glb_pdb)

	return ret_val;
}

/******************************************************************************
 *
 * FUNCTION:    icf_mmi_process_mms_modify_action
 *
 * DESCRIPTION: 
 *****************************************************************************/

icf_return_t	icf_mmi_process_mms_modify_action
(
	icf_mmi_pdb_st *p_mmi_pdb
)
{
	icf_return_t					ret_val = ICF_SUCCESS;
	icf_mmi_internal_mms_resp_st	*p_mms_resp = ICF_NULL;
	icf_boolean_t					send_gcl = ICF_FALSE,
									send_commit = ICF_FALSE;
	icf_stream_id_t					stream_count = ICF_NULL,
									stream_id = ICF_NULL;
	icf_media_type_t				stream_type = ICF_STREAM_TYPE_INVALID;
	icf_uint32_t					bitmask = ICF_NULL;

	ICF_FUNCTION_ENTER(p_mmi_pdb->p_glb_pdb)

	p_mmi_pdb->p_mmi_ctx->ongoing_event = ICF_MMI_MMS_MOD_ONGOING;

	p_mms_resp = p_mmi_pdb->p_mmi_ctx->p_mmi_resp;

	for(stream_count = 0; (stream_count < p_mms_resp->mod_stream_count) && (stream_count < ICF_MAX_STREAM_PER_CALL); 
		stream_count++)
	{
		stream_id = 
			p_mms_resp->modified_stream_resp[stream_count].stream_id;
		stream_type = 
			p_mms_resp->modified_stream_resp[stream_count].stream_type;
		bitmask = p_mms_resp->modified_stream_resp[stream_count].bitmask;

		if(stream_type != ICF_STREAM_TYPE_TBCP)
		{
			if((bitmask & ICF_REMOTE_SDP_CODEC_CHANGE)||\
                (bitmask & ICF_LOCAL_MEDIA_CONNECT_CODEC_CHANGE))
			{
				/* Make GCL payload fill remote codecs from remote_recvd SDP */
				send_gcl = ICF_TRUE;
			}
			else if(bitmask & ICF_LOCAL_SDP_CODEC_CHANGE)
			{
				/* Make GCL payload fill local codecs from local SDP. */
				send_gcl = ICF_TRUE;
			}
			else if( bitmask & ICF_REMOTE_ANSWER_CODEC_CHANGE)
			{
				/* This function returns success if at least one codec match 
				 * from remote_recvd SDP and local SDP for this stream
				 */
				ret_val = 
					icf_mmi_negotiate_codecs_per_stream(p_mmi_pdb,stream_id);
				
				if(ICF_SUCCESS == ret_val)
				{
					/* Make COMMIT payload for this stream pick codecs from 
					 * remote_recvd SDP */
					send_commit = ICF_TRUE;
					break;
				}
				/* case when there is no match for codecs for this stream in 
				 * remote_recvd SDP and local SDP
				 */
				else
				{
					/* Don't send commit and mark the stream as failure */
					icf_mmi_mark_failure_stream_mms_mod_resp(p_mmi_pdb,stream_id);
				}
			}
			else if(bitmask & ICF_REMOTE_ANSWER_REVERT_CODEC_CHANGE)
			{
				/* Make COMMIT payload for this stream pick codecs from 
				 * last_commited SDP */
				send_commit = ICF_TRUE;
				break;
			}
			else if(bitmask & ICF_LOCAL_ANSWER_REVERT_CODEC_CHANGE)
			{
				/* Make GCL payload fill local codecs from last_commited SDP */
				send_gcl = ICF_TRUE;
			}
		}
	}

	if(ICF_TRUE == send_commit)
	{
		ret_val = icf_mmi_send_commit_req(p_mmi_pdb);

		if(ICF_SUCCESS == ret_val)
		{
			ICF_MMI_SET_STATE(p_mmi_pdb,ICF_MMI_03_AWT_COMMIT_RESP)
		}
	}
	else if(ICF_TRUE == send_gcl)
	{
		ret_val = icf_mmi_send_gcl_req(p_mmi_pdb);

		if(ICF_SUCCESS == ret_val)
		{
			ICF_MMI_SET_STATE(p_mmi_pdb,ICF_MMI_01_AWT_GCL_RESP)
		}	
	}
	/* Case when only MMS needed to be sent to MM i.e. no codec change */
	else
	{
		/* The streams shall be dealt in icf_mmi_send_mms_req(p_mmi_pdb)
		 * depending on the bitmask
		 *
		 * if(bitmask & ICF_REMOTE_SDP_MEDIA_CHANGE)
		 * {
		 *   Make the MMS payload from remote_recvd SDP
		 * }
		 * else if(bitmask & ICF_LOCAL_SDP_MEDIA_CHANGE)
		 * {
		 *   Make the MMS payload from local SDP
		 * }
		 * else if((bitmask & ICF_REMOTE_ANSWER_REVERT_SDP_CHANGE)||
		 * (bitmask & ICF_LOCAL_ANSWER_REVERT_SDP_CHANGE))
		 * {
		 *   Make the MMS payload from last_commited SDP
		 * }
		 * if(bitmask & ICF_MODE_CHANGE)
		 * {
		 *   Make the mode in MMS payload from media_mode_to_be_applied
		 *   in stream_sdp_info for this stream
		 * }
		 */

		ret_val = icf_mmi_send_mms_req(p_mmi_pdb);
		
		if(ICF_SUCCESS == ret_val)
		{
			ICF_MMI_SET_STATE(p_mmi_pdb,ICF_MMI_04_AWT_MMS_RESP)
		}
	}

	ICF_FUNCTION_EXIT(p_mmi_pdb->p_glb_pdb)

	return ret_val;
}


/******************************************************************************
 *
 * FUNCTION:    icf_mmi_mark_failure_stream_mms_mod_resp
 *
 * DESCRIPTION: This function marks the status of a particular stream as failure
 *				in MOD action of mms_resp in MMI ctxt
 *****************************************************************************/

icf_return_t	icf_mmi_mark_failure_stream_mms_mod_resp
(
	icf_mmi_pdb_st	*p_mmi_pdb,
	icf_stream_id_t	stream_id
)
{
	icf_return_t	ret_val = ICF_SUCCESS;
	icf_mmi_internal_mms_resp_st	*p_mms_resp = ICF_NULL;
	icf_stream_id_t					stream_count = ICF_NULL;

	ICF_FUNCTION_ENTER(p_mmi_pdb->p_glb_pdb)

	p_mms_resp = p_mmi_pdb->p_mmi_ctx->p_mmi_resp;

	for(stream_count = 0; (stream_count < p_mms_resp->mod_stream_count) && (stream_count < ICF_MAX_STREAM_PER_CALL);
		stream_count++)
	{
		if(stream_id == p_mms_resp->modified_stream_resp[stream_count].stream_id)
		{
			p_mms_resp->modified_stream_resp[stream_count].status_success = 
				ICF_FALSE;
			break;
		}
	}

	ICF_FUNCTION_EXIT(p_mmi_pdb->p_glb_pdb)

	return ret_val;
}


/******************************************************************************
 *
 * FUNCTION:    icf_mmi_mark_failure_stream_mms_add_resp
 *
 * DESCRIPTION: This function marks the status of a particular stream as failure
 *				in ADD action of mms_resp in MMI ctxt
 *****************************************************************************/

icf_return_t	icf_mmi_mark_failure_stream_mms_add_resp
(
	icf_mmi_pdb_st	*p_mmi_pdb, 
	icf_stream_id_t	stream_id
)
{
	icf_return_t					ret_val = ICF_SUCCESS;
	icf_mmi_internal_mms_resp_st	*p_mms_resp = ICF_NULL;
	icf_stream_id_t					stream_count = ICF_NULL;

	ICF_FUNCTION_ENTER(p_mmi_pdb->p_glb_pdb)

	p_mms_resp = p_mmi_pdb->p_mmi_ctx->p_mmi_resp;

	for(stream_count = 0; (stream_count < p_mms_resp->add_stream_count) && (stream_count < ICF_MAX_STREAM_PER_CALL);
		stream_count++)
	{
		if(stream_id == p_mms_resp->added_stream_resp[stream_count].stream_id)
		{
			p_mms_resp->added_stream_resp[stream_count].status_success = 
				ICF_FALSE;
			break;
		}
	}

	ICF_FUNCTION_EXIT(p_mmi_pdb->p_glb_pdb)

	return ret_val;
}


/******************************************************************************
 *
 * FUNCTION:    icf_mmi_mark_failure_stream_cms_resp
 *
 * DESCRIPTION: This function marks the status of a particular stream as failure
 *				in cms_resp in MMI ctxt
 *****************************************************************************/

icf_return_t	icf_mmi_mark_failure_stream_cms_resp
(
	icf_mmi_pdb_st	*p_mmi_pdb, 
	icf_stream_id_t	stream_id
)
{
	icf_return_t	ret_val = ICF_SUCCESS;
	icf_mmi_internal_cms_resp_st	*p_cms_resp = ICF_NULL;
	icf_stream_id_t					stream_count = ICF_NULL;

	ICF_FUNCTION_ENTER(p_mmi_pdb->p_glb_pdb)

	p_cms_resp = p_mmi_pdb->p_mmi_ctx->p_mmi_resp;

	for(stream_count = 0; (stream_count < p_cms_resp->count) && (stream_count < ICF_MAX_STREAM_PER_CALL) ; stream_count++)
	{
		if(stream_id == p_cms_resp->stream_status[stream_count].stream_id)
		{
			p_cms_resp->stream_status[stream_count].status_success = ICF_FALSE;
			break;
		}
	}

	ICF_FUNCTION_EXIT(p_mmi_pdb->p_glb_pdb)

	return ret_val;
}


/******************************************************************************
 *
 * FUNCTION:    icf_mmi_mark_all_stream_mms_add_resp
 *
 * DESCRIPTION: This function marks the status of all streams as failure
 *				in ADD action of mms_resp in MMI ctxt
 *****************************************************************************/

icf_return_t	icf_mmi_mark_all_stream_mms_add_resp
(
	icf_mmi_pdb_st	*p_mmi_pdb
)
{
	icf_return_t					ret_val = ICF_SUCCESS;
	icf_mmi_internal_mms_resp_st	*p_mms_resp = ICF_NULL;
	icf_stream_id_t					stream_count = ICF_NULL;

	ICF_FUNCTION_ENTER(p_mmi_pdb->p_glb_pdb)

	p_mms_resp = p_mmi_pdb->p_mmi_ctx->p_mmi_resp;

	for(stream_count = 0; (stream_count < p_mms_resp->add_stream_count) && (stream_count < ICF_MAX_STREAM_PER_CALL);
		stream_count++)
	{
		p_mms_resp->added_stream_resp[stream_count].status_success = 
			ICF_FALSE;
	}

	ICF_FUNCTION_EXIT(p_mmi_pdb->p_glb_pdb)

	return ret_val;
}


/******************************************************************************
 *
 * FUNCTION:    icf_mmi_mark_all_stream_cms_resp
 *
 * DESCRIPTION: This function marks the status of all streams as failure
 *				in cms_resp in MMI ctxt
 *****************************************************************************/

icf_return_t	icf_mmi_mark_all_stream_cms_resp
(
	icf_mmi_pdb_st	*p_mmi_pdb
)
{
	icf_return_t	ret_val = ICF_SUCCESS;
	icf_mmi_internal_cms_resp_st	*p_cms_resp = ICF_NULL;
	icf_stream_id_t					stream_count = ICF_NULL;

	ICF_FUNCTION_ENTER(p_mmi_pdb->p_glb_pdb)

	p_cms_resp = p_mmi_pdb->p_mmi_ctx->p_mmi_resp;

	for(stream_count = 0; (stream_count < p_cms_resp->count) && (stream_count < ICF_MAX_STREAM_PER_CALL); stream_count++)
	{
		p_cms_resp->stream_status[stream_count].status_success = ICF_FALSE;
	}

	ICF_FUNCTION_EXIT(p_mmi_pdb->p_glb_pdb)

	return ret_val;
}


/******************************************************************************
 *
 * FUNCTION:    icf_mmi_search_failure_stream
 *
 * DESCRIPTION: This function returns success if the particular stream_id is 
 *				present amongst the stream_id list that is sent as argument
 *****************************************************************************/

icf_return_t	icf_mmi_search_failure_stream
(
	icf_mmi_pdb_st			*p_mmi_pdb, 
	icf_stream_id_list_st	*p_src,
	icf_stream_id_t			stream_id
)
{
	icf_return_t		ret_val = ICF_FAILURE;
	icf_stream_id_t						stream_count = ICF_NULL;

	    if(ICF_NULL == p_mmi_pdb)
        {
           return ICF_FAILURE;
        }	
        ICF_FUNCTION_ENTER(p_mmi_pdb->p_glb_pdb)

	p_mmi_pdb = p_mmi_pdb;

	for(stream_count = 0; (stream_count < p_src->count) && (stream_count < ICF_MAX_STREAM_PER_CALL) ; stream_count++)
	{
		if(p_src->stream_id[stream_count] == stream_id)
		{
			ret_val = ICF_SUCCESS;
			break;
		}
	}

	ICF_FUNCTION_EXIT(p_mmi_pdb->p_glb_pdb)

	return ret_val;
}


/******************************************************************************
 *
 * FUNCTION:    icf_mmi_get_sdp_for_stream_id
 *
 * DESCRIPTION: 
 *****************************************************************************/

icf_return_t	icf_mmi_get_sdp_for_stream_id
(
	icf_mmi_pdb_st		*p_mmi_pdb, 
	icf_stream_id_t		stream_id,
	icf_uint8_t			sdp_type,
	icf_sdp_stream_st	*p_sdp
)
{
	icf_return_t					ret_val = ICF_FAILURE;
	icf_inc_media_sdp_info_st		*p_rem_sdp = ICF_NULL;
	icf_internal_sdp_stream_st		*p_int_sdp = ICF_NULL;
	icf_internal_list_st			*p_int_list = ICF_NULL;

	icf_uint32_t	loc_stream = 0;

	ICF_FUNCTION_ENTER(p_mmi_pdb->p_glb_pdb)

	loc_stream = stream_id;

	if((ICF_SDP_REMOTE == sdp_type)&&
		(ICF_NULL != p_mmi_pdb->p_mmi_ctx->p_remote_recvd_sdp))
	{
		p_int_list = icf_cmn_get_node_in_internal_list(
							p_mmi_pdb->p_mmi_ctx->p_remote_recvd_sdp,
							&loc_stream);

		if(ICF_NULL != p_int_list)
		{
			p_rem_sdp = p_int_list->p_payload;
		}

		if(ICF_NULL != p_rem_sdp)
		{
			icf_port_memcpy(
				p_sdp,
				&(p_rem_sdp->p_int_sdp_stream->sdp_stream),
				(icf_uint32_t)ICF_PORT_SIZEOF(icf_sdp_stream_st));

			ret_val = ICF_SUCCESS;
		}
	}
	else
	{
		ret_val = icf_cmn_fetch_sdp_in_list_for_stream_id(
										p_mmi_pdb->p_glb_pdb,
										stream_id, 
										sdp_type, 
										&p_int_sdp);

		if(ICF_SUCCESS == ret_val)
		{
			icf_port_memcpy(
					p_sdp,
					&(p_int_sdp->sdp_stream),
					(icf_uint32_t)ICF_PORT_SIZEOF(icf_sdp_stream_st));
		}
	}

	ICF_FUNCTION_EXIT(p_mmi_pdb->p_glb_pdb)

	return ret_val;
}


/******************************************************************************
 *
 * FUNCTION:    icf_mmi_make_gcl_payload
 *
 * DESCRIPTION: 
 *****************************************************************************/

icf_return_t	icf_mmi_make_gcl_payload
(
	icf_mmi_pdb_st		*p_mmi_pdb, 
	icf_sdp_stream_st	*p_sdp,
	icf_media_type_t	stream_type,
	icf_stream_capab_st	*p_node
)
{
	/* local variables */	
	icf_return_t					ret_val = ICF_SUCCESS;
	
	ICF_FUNCTION_ENTER(p_mmi_pdb->p_glb_pdb)

	p_node->bit_mask = ICF_NULL;
	p_node->p_codec_info_list = ICF_NULL;
	p_node->p_encoded_attributes = ICF_NULL;

	if(p_sdp->p_codec_info_list)
	{
		icf_mmi_clone_codec_info_list(
			p_mmi_pdb,
			p_sdp->p_codec_info_list,
			&p_node->p_codec_info_list);

		p_node->bit_mask |= ICF_STREAM_CAPAB_CODEC_INFO_PRESENT;
	}
	else
	{
		p_node->p_codec_info_list = ICF_NULL;
	}
	
	if(p_sdp->bit_mask & ICF_SDP_STREAM_ENCPARAMS_PRESENT)
	{
		icf_mmi_clone_enc_attrib_list(
			p_mmi_pdb,
			p_sdp->p_encoded_attributes,
			&p_node->p_encoded_attributes);

		p_node->bit_mask |= ICF_STREAM_CAPAB_ENC_ATTRIB_PRESENT;
	}
	else
	{
		p_node->p_encoded_attributes = ICF_NULL;
	}

	if(p_sdp->bit_mask & ICF_SDP_STREAM_MEDIA_STREAM_PRESENT)
	{
		if(ICF_STREAM_TYPE_AUDIO == stream_type)
		{
			p_node->media_stream_param.audio_profile.bandwidth = 
						p_sdp->media_stream_param.audio_profile.bandwidth;

			p_node->media_stream_param.audio_profile.bit_mask = 
						p_sdp->media_stream_param.audio_profile.bit_mask;

			p_node->media_stream_param.audio_profile.pref_ptime = 
						p_sdp->media_stream_param.audio_profile.pref_ptime;

			p_node->media_stream_param.audio_profile.silence_suppression= 
					p_sdp->media_stream_param.audio_profile.silence_suppression;

                        p_node->media_stream_param.audio_profile.echo_canc = 
					p_sdp->media_stream_param.audio_profile.echo_canc;
		}
		else
		{
			p_node->media_stream_param.video_profile.bandwidth = 
						p_sdp->media_stream_param.video_profile.bandwidth;

			p_node->media_stream_param.video_profile.bit_mask = 
						p_sdp->media_stream_param.video_profile.bit_mask;

			p_node->media_stream_param.video_profile.encoding_quality = 
					p_sdp->media_stream_param.video_profile.encoding_quality;

			p_node->media_stream_param.video_profile.frame_rate = 
						p_sdp->media_stream_param.video_profile.frame_rate;
		}
		p_node->bit_mask |= ICF_STREAM_CAPAB_MEDIA_STREAM_PARAM_PRESENT;
	}

	ICF_FUNCTION_EXIT(p_mmi_pdb->p_glb_pdb)

	return ret_val;
}


/******************************************************************************
 *
 * FUNCTION:    icf_mmi_make_cms_payload
 *
 * DESCRIPTION: 
 *****************************************************************************/

icf_return_t	icf_mmi_make_cms_payload
(
	icf_mmi_pdb_st		*p_mmi_pdb, 
	icf_stream_id_t		stream_id,
	icf_media_type_t	stream_type,
	icf_req_stream_st	*p_node
)
{
	icf_return_t			ret_val = ICF_SUCCESS;
	icf_sdp_stream_st		sdp;
#ifdef ICF_NAT_MEDIA_TRAVERSAL
    icf_stream_id_t         index = ICF_NULL;
#endif

	ICF_FUNCTION_ENTER(p_mmi_pdb->p_glb_pdb)

    icf_mmi_init_sdp(&sdp);
	/*icf_port_memset(&sdp, 0, ICF_PORT_SIZEOF(sdp));*/

	p_mmi_pdb->stream_id = stream_id;
	p_mmi_pdb->stream_type = stream_type;

	p_node->bit_mask = ICF_NULL;

	/* Initialise the sdp lists to NULL */
	p_node->local_sdp.bit_mask = ICF_NULL;
	p_node->local_sdp.p_codec_info_list = ICF_NULL;
	p_node->local_sdp.p_encoded_attributes = ICF_NULL;

	p_node->remote_sdp.bit_mask = ICF_NULL;
	p_node->remote_sdp.p_codec_info_list = ICF_NULL;
	p_node->remote_sdp.p_encoded_attributes = ICF_NULL;
        

	/* get local SDP for this stream from call context */
	ret_val = icf_mmi_get_sdp_for_stream_id(
							p_mmi_pdb,
							stream_id,
							ICF_SDP_LOCAL, 
							&sdp);

	if(ICF_SUCCESS == ret_val)
	{
		icf_port_memcpy(
			&p_node->local_sdp,
			&sdp,
			(icf_uint32_t)ICF_PORT_SIZEOF(icf_sdp_stream_st));

		icf_mmi_clone_sdp_stream_lists(
			p_mmi_pdb,
			&sdp,
			&p_node->local_sdp);
        /* Update the SDP stream address from the local address
         * stored in the mmi_ctxt
         */
#ifdef ICF_NAT_MEDIA_TRAVERSAL
        for(index = 0;index < p_mmi_pdb->p_mmi_ctx->nat_stream_count;index++)
        {
            if(stream_id == p_mmi_pdb->p_mmi_ctx->nat_streams[index].stream_id)
            {
                icf_port_memcpy(&(p_node->local_sdp.stream_address),
                        &(p_mmi_pdb->p_mmi_ctx->nat_streams[index].\
                    local_addr),
				(icf_uint32_t)ICF_PORT_SIZEOF(icf_transport_address_st));
                if(ICF_STREAM_TYPE_AUDIO == stream_type || \
                        ICF_STREAM_TYPE_VIDEO == stream_type)
                {
                    /* Update the RTCP address in the cms payload.
                     */
                    icf_port_memcpy(&(p_node->local_sdp.rtcp_address),
                            &(p_mmi_pdb->p_mmi_ctx->nat_streams[index].\
                                local_addr),(icf_uint32_t)\
                            ICF_PORT_SIZEOF(icf_transport_address_st));
                    p_node->local_sdp.rtcp_address.port_num += 1;

                }
                break;
            }
        }
#endif

		p_node->bit_mask |= ICF_CREATE_MEDIA_LOCAL_MEDIA_PROFILE_PRESENT;
#if 0
		if(ICF_STREAM_TYPE_AUDIO == stream_type)
		{
			p_node->echo_cancellation = 
				icf_mmi_get_echo_cancellation(p_mmi_pdb, &sdp);

			p_node->bit_mask |= ICF_CREATE_MEDIA_ECHO_CAN_PRESENT;
		}
#endif		
	}
	if(ICF_SUCCESS == icf_mmi_get_sdp_for_stream_id(p_mmi_pdb, stream_id, 
										ICF_SDP_REMOTE, &sdp))
	{
		/* Get the remote_recvd_sdp for this stream from mmi ctxt */

		icf_port_memcpy(
			&p_node->remote_sdp,
			&sdp,
			(icf_uint32_t)ICF_PORT_SIZEOF(icf_sdp_stream_st));

		icf_mmi_clone_sdp_stream_lists(
			p_mmi_pdb,
			&sdp,
			&p_node->remote_sdp);

		p_node->bit_mask |= ICF_CREATE_MEDIA_REMOTE_SDP_PRESENT;

		if(ICF_STREAM_TYPE_AUDIO == stream_type)
		{
			p_node->echo_cancellation = 
				icf_mmi_get_echo_cancellation(p_mmi_pdb, &sdp);

			p_node->bit_mask |= ICF_CREATE_MEDIA_ECHO_CAN_PRESENT;
		}

		icf_mmi_check_sdp_for_commited_codecs(p_mmi_pdb, 
								&p_node->local_sdp.p_codec_info_list);

		icf_mmi_check_sdp_for_commited_codecs(p_mmi_pdb, 
								&p_node->remote_sdp.p_codec_info_list);
	}

	ICF_FUNCTION_EXIT(p_mmi_pdb->p_glb_pdb)

	return ret_val;
}


/******************************************************************************
 *
 * FUNCTION:    icf_mmi_check_sdp_for_commited_codecs
 *
 * DESCRIPTION: 
 *****************************************************************************/

icf_return_t	icf_mmi_check_sdp_for_commited_codecs
(
	icf_mmi_pdb_st		*p_mmi_pdb, 
	icf_list_st			**p_p_list
)
{
	icf_return_t		ret_val = ICF_FAILURE,
						dummy_ret_val = ICF_FAILURE;
	icf_list_st			*p_list = ICF_NULL,
                        *p_new_list = ICF_NULL;    
	icf_codec_attrib_st	*p_codec = ICF_NULL;

	ICF_FUNCTION_ENTER(p_mmi_pdb->p_glb_pdb)

	p_list = *p_p_list;

	while(p_list)
	{
		if(ICF_SUCCESS == icf_mmi_is_codec_commited(p_mmi_pdb,
								p_list->p_data))
		{
			ICF_MEMGET (
				p_mmi_pdb->p_glb_pdb,
				ICF_PORT_SIZEOF(icf_codec_attrib_st),
				ICF_MEM_COMMON,
				p_codec,
				ICF_RET_ON_FAILURE,
				p_mmi_pdb->p_ecode,
				dummy_ret_val)

			if(ICF_SUCCESS == dummy_ret_val)
			{
				icf_port_memcpy(
					p_codec,
					p_list->p_data,
					(icf_uint32_t)ICF_PORT_SIZEOF(icf_codec_attrib_st));

				dummy_ret_val = icf_cmn_add_node_to_list(
									p_mmi_pdb->p_glb_pdb,
									&p_new_list,
									p_codec);
			}
			else
			{
				ICF_ALARM(((icf_uint8_t *)"[MMI]: MEMGET failure"));
				break;
			}

			if(ICF_SUCCESS == dummy_ret_val)
			{
                ICF_KLOCWORK_FALSE_POSITIVE(p_mmi_pdb->p_glb_pdb,p_codec)
                ICF_KLOCWORK_FALSE_POSITIVE(p_mmi_pdb->p_glb_pdb,p_new_list)
				ret_val = ICF_SUCCESS;
			}
			else
			{
				ICF_MEMFREE(p_mmi_pdb->p_glb_pdb, p_codec, ICF_MEM_COMMON, 
					p_mmi_pdb->p_ecode, ret_val)					
			}
		}

		p_list = p_list->p_next;
	}

	icf_cmn_delete_list(p_mmi_pdb->p_glb_pdb,p_p_list,
		ICF_MEM_COMMON);
    *p_p_list = p_new_list;

	if((ICF_SUCCESS == ret_val)&&(ICF_FAILURE == dummy_ret_val))
	{
		icf_cmn_delete_list(p_mmi_pdb->p_glb_pdb,
							&p_new_list,
							ICF_MEM_COMMON);
	}

	ICF_FUNCTION_EXIT(p_mmi_pdb->p_glb_pdb)

	return ret_val;
}
/******************************************************************************
 *
 * FUNCTION:    icf_mmi_is_codec_commited
 *
 * DESCRIPTION: 
 *****************************************************************************/

icf_return_t	icf_mmi_is_codec_commited
(
	icf_mmi_pdb_st		*p_mmi_pdb, 
	icf_codec_attrib_st	*p_codec
)
{
	icf_return_t		ret_val = ICF_FAILURE;
	icf_mmi_context_st	*p_ctx = p_mmi_pdb->p_mmi_ctx;
	icf_stream_id_t		stream_count = ICF_NULL;
	icf_uint8_t			codec_count = ICF_NULL;

	ICF_FUNCTION_ENTER(p_mmi_pdb->p_glb_pdb)

	for(stream_count = 0; (stream_count < p_ctx->stream_count) && (stream_count < ICF_MAX_STREAM_PER_CALL) ; stream_count++)
	{
		if(p_mmi_pdb->stream_id == 
						p_ctx->codec_for_commit[stream_count].stream_id)
		{
			for(codec_count = 0; codec_count< 
				(p_ctx->codec_for_commit[stream_count].codec_count) &&
				(codec_count < ICF_MAX_CODECS); 
				codec_count++)
			{
				ret_val = icf_mmi_compare_codecs_value(p_mmi_pdb,p_codec,
				&(p_ctx->codec_for_commit[stream_count].codec_info[codec_count]));

				if(ICF_SUCCESS == ret_val)
					break;
			}
			break;
		}
	}

	ICF_FUNCTION_EXIT(p_mmi_pdb->p_glb_pdb)

	return ret_val;
}


/******************************************************************************
 *
 * FUNCTION:    icf_mmi_make_mms_payload
 *
 * DESCRIPTION: 
 *****************************************************************************/

icf_return_t	icf_mmi_make_mms_payload
(
	icf_mmi_pdb_st		*p_mmi_pdb, 
	icf_sdp_stream_st	*p_sdp,
	icf_media_type_t	stream_type,
	icf_modify_stream_st*p_node
)
{
	icf_return_t					ret_val = ICF_SUCCESS;

        /* To remove warning */
        stream_type=stream_type;

	ICF_FUNCTION_ENTER(p_mmi_pdb->p_glb_pdb)
	icf_port_memcpy(
		&p_node->sdp_stream,
		p_sdp,
		(icf_uint32_t)ICF_PORT_SIZEOF(icf_sdp_stream_st));

	icf_mmi_clone_sdp_stream_lists(
		p_mmi_pdb,
		p_sdp,
		&p_node->sdp_stream);

	ICF_FUNCTION_EXIT(p_mmi_pdb->p_glb_pdb)

	return ret_val;
}


/******************************************************************************
 *
 * FUNCTION:    icf_mmi_make_commit_payload
 *
 * DESCRIPTION: 
 *****************************************************************************/

icf_return_t	icf_mmi_make_commit_payload
(
	icf_mmi_pdb_st		*p_mmi_pdb,
	icf_sdp_stream_st	*p_sdp,
	icf_media_type_t	stream_type,
	icf_stream_capab_st *p_node
)
{
	icf_return_t					ret_val = ICF_SUCCESS;

	ICF_FUNCTION_ENTER(p_mmi_pdb->p_glb_pdb)

	ret_val = icf_mmi_make_gcl_payload(
						p_mmi_pdb,
						p_sdp,
						stream_type,
						p_node);

	ICF_FUNCTION_EXIT(p_mmi_pdb->p_glb_pdb)

	return ret_val;
}


/******************************************************************************
 *
 * FUNCTION:    icf_mmi_clone_sdp_stream_lists
 *
 * DESCRIPTION: 
 *****************************************************************************/

icf_return_t	icf_mmi_clone_sdp_stream_lists
(
	INOUT	icf_mmi_pdb_st		*p_mmi_pdb,
	IN	icf_sdp_stream_st	*p_src_sdp,
	OUT	icf_sdp_stream_st	*p_dest_sdp
)
{
	icf_return_t			ret_val = ICF_SUCCESS;

	if(p_src_sdp->p_codec_info_list)
	{
		icf_mmi_clone_codec_info_list(
			p_mmi_pdb,
			p_src_sdp->p_codec_info_list,
			&(p_dest_sdp->p_codec_info_list));
	}

	if(p_src_sdp->bit_mask & ICF_SDP_STREAM_ENCPARAMS_PRESENT)
	{
		icf_mmi_clone_enc_attrib_list(
			p_mmi_pdb,
			p_src_sdp->p_encoded_attributes,
			&p_dest_sdp->p_encoded_attributes);

			p_dest_sdp->bit_mask |= ICF_SDP_STREAM_ENCPARAMS_PRESENT;
	}
#ifdef ICF_RTCP_ATTR_SUPPORT
    if(p_src_sdp->bit_mask & ICF_SDP_STREAM_RTCP_INFO_PRESENT)
    {
        icf_port_memcpy((icf_void_t*)&(p_src_sdp->rtcp_address),&(p_dest_sdp->rtcp_address),\
                ICF_PORT_SIZEOF(icf_transport_address_st));

    }
#endif
	return ret_val;
}


/******************************************************************************
 *
 * FUNCTION:    icf_mmi_delete_sdp_stream_cloned_lists
 *
 * DESCRIPTION: 
 *****************************************************************************/

icf_return_t	icf_mmi_delete_sdp_stream_cloned_lists
(
	INOUT	icf_mmi_pdb_st		*p_mmi_pdb,
	INOUT	icf_sdp_stream_st	*p_sdp
)
{
	if(p_sdp->p_codec_info_list)
	{
		icf_cmn_delete_list(p_mmi_pdb->p_glb_pdb,&p_sdp->p_codec_info_list,
			ICF_MEM_COMMON);
	}

	if(p_sdp->p_encoded_attributes)
	{
		icf_cmn_delete_list(p_mmi_pdb->p_glb_pdb,&p_sdp->p_encoded_attributes,
			ICF_MEM_COMMON);
	}

	return ICF_SUCCESS;
}



/******************************************************************************
 *
 * FUNCTION:    icf_mmi_clone_codec_info_list
 *
 * DESCRIPTION: 
 *****************************************************************************/

icf_return_t	icf_mmi_clone_codec_info_list
(
	INOUT icf_mmi_pdb_st		*p_mmi_pdb,
	IN	  icf_list_st			*p_src_list,
	OUT	  icf_list_st			**p_p_dest_list
)
{
	icf_return_t			ret_val = ICF_FAILURE,
							dummy_ret_val = ICF_FAILURE;
	icf_codec_attrib_st		*p_node = ICF_NULL;
	icf_boolean_t			memget_fail = ICF_FALSE;

	ICF_FUNCTION_ENTER(p_mmi_pdb->p_glb_pdb)

	*p_p_dest_list = ICF_NULL;

	while(p_src_list)
	{
		/* MEMGET for p_node as type icf_codec_attrib_st */
		ICF_MEMGET (p_mmi_pdb->p_glb_pdb,
					ICF_PORT_SIZEOF(icf_codec_attrib_st),
					ICF_MEM_COMMON,
					p_node,
					ICF_DONOT_RET_ON_FAILURE,
					p_mmi_pdb->p_ecode,
					dummy_ret_val)

		if(ICF_SUCCESS == dummy_ret_val)
		{
			icf_port_memcpy(
				p_node,
				p_src_list->p_data,
				(icf_uint32_t)ICF_PORT_SIZEOF(icf_codec_attrib_st));

			if(p_node->bit_mask & ICF_CODEC_ENC_ATTRIB_PRESENT)
			{
				/* clone the codec attributes */
				/* in the current release this is not going to be used, so this
			 	 * block is empty- no concept of encoded attribute lines within
			     * the codec */
			}

            dummy_ret_val = icf_cmn_add_node_to_list(
                                p_mmi_pdb->p_glb_pdb,
                                p_p_dest_list,
                                p_node);
        }
		else
		{
			memget_fail = ICF_TRUE;
			break;
		}

		if(ICF_SUCCESS == dummy_ret_val)
		{
            ICF_KLOCWORK_FALSE_POSITIVE(p_mmi_pdb->p_glb_pdb,p_node)
			ret_val = ICF_SUCCESS;
		}
        else /* if icf_cmn_add_node_to_list fails */
        {
            /* MEMFREE codec attribute list, p_node 
             * and its member pointers */
            icf_cmn_delete_codec_attrib_list(p_mmi_pdb->p_glb_pdb,
                    p_p_dest_list, p_mmi_pdb->p_ecode);
            icf_mmi_free_icf_codec_attrib_node(p_mmi_pdb->p_glb_pdb,
                    &p_node,p_mmi_pdb->p_ecode);
            break;
        }

		p_src_list = p_src_list->p_next;
	}

	if(ICF_TRUE == memget_fail)
	{
		ICF_ALARM(((icf_uint8_t *)"[MMI]: MEMGET failure"));

		/* case where first node is copied successfully but any further node
		 * fails */
		if(ICF_SUCCESS == ret_val)
		{
			icf_cmn_delete_list(p_mmi_pdb->p_glb_pdb,p_p_dest_list,ICF_MEM_COMMON);
			ret_val = ICF_FAILURE;
		}
	}

	ICF_FUNCTION_EXIT(p_mmi_pdb->p_glb_pdb)

	return	ret_val;
}


/******************************************************************************
 *
 * FUNCTION:    icf_mmi_clone_enc_attrib_list
 *
 * DESCRIPTION: 
 *****************************************************************************/

icf_return_t	icf_mmi_clone_enc_attrib_list
(
	IN	icf_mmi_pdb_st		*p_mmi_pdb,
	IN	icf_list_st			*p_src_list,
	OUT	icf_list_st			**p_p_dest_list
)
{
	icf_return_t			ret_val = ICF_FAILURE,
							dummy_ret_val = ICF_FAILURE;
	icf_string_st			*p_node = ICF_NULL;
	icf_boolean_t			memget_fail = ICF_FALSE;

	ICF_FUNCTION_ENTER(p_mmi_pdb->p_glb_pdb)

	*p_p_dest_list = ICF_NULL;
	while(p_src_list)
	{
		/* MEMGET for p_node as type icf_codec_attrib_st */
		ICF_MEMGET (p_mmi_pdb->p_glb_pdb,
					ICF_PORT_SIZEOF(icf_string_st),
					ICF_MEM_COMMON,
					p_node,
					ICF_DONOT_RET_ON_FAILURE,
					p_mmi_pdb->p_ecode,
					dummy_ret_val)

		if(ICF_SUCCESS == dummy_ret_val)
		{
			icf_port_memcpy(
				p_node,
				p_src_list->p_data,
				(icf_uint32_t)ICF_PORT_SIZEOF(icf_string_st));
		
			dummy_ret_val = icf_cmn_add_node_to_list(
										p_mmi_pdb->p_glb_pdb,
										p_p_dest_list,
										p_node);
		}
		else
		{
			memget_fail = ICF_TRUE;
			break;
		}

		if(ICF_SUCCESS == dummy_ret_val)
		{
            ICF_KLOCWORK_FALSE_POSITIVE(p_mmi_pdb->p_glb_pdb,p_node)
			ret_val = ICF_SUCCESS;
		}
        else
        {
            /* Free p_node */
            ICF_MEMFREE(
                    p_mmi_pdb->p_glb_pdb, 
                    p_node,
                    ICF_MEM_COMMON,
                    p_mmi_pdb->p_ecode, 
                    dummy_ret_val)
        }

		p_src_list = p_src_list->p_next;
	}

	/* case where first node is copied successfully but any further node
	 * fails */
	if((ICF_SUCCESS == ret_val)&&
		(ICF_TRUE == memget_fail))
	{
		icf_cmn_delete_list(p_mmi_pdb->p_glb_pdb,p_p_dest_list,ICF_MEM_COMMON);

		ret_val = ICF_FAILURE;
	}

	ICF_FUNCTION_EXIT(p_mmi_pdb->p_glb_pdb)

	return	ret_val;
}


/******************************************************************************
 *
 * FUNCTION:    icf_mmi_get_mode_for_stream_id
 *
 * DESCRIPTION: 
 *****************************************************************************/

icf_media_mode_t	icf_mmi_get_mode_for_stream_id
(
	icf_mmi_pdb_st		*p_mmi_pdb, 
	icf_stream_id_t		stream_id
)
{
	icf_media_mode_t			media_mode = ICF_MEDIA_MODE_INVALID;
	icf_stream_sdp_info_st		*p_node = ICF_NULL;
	icf_internal_list_st		*p_int_list = ICF_NULL;
        icf_uint32_t loc_stream = 0;

	loc_stream = stream_id;
	p_int_list = icf_cmn_get_node_in_internal_list(
					p_mmi_pdb->p_call_ctx->p_stream_sdp_info,
					&loc_stream);

	if(ICF_NULL != p_int_list)
	{
		p_node = p_int_list->p_payload;
	}

	if(p_node)
	{
		media_mode = p_node->mm_applied_mode;
	}

	return media_mode;
}


/******************************************************************************
 *
 * FUNCTION:    icf_mmi_get_echo_cancellation
 *
 * DESCRIPTION: 
 *****************************************************************************/

icf_boolean_t	icf_mmi_get_echo_cancellation
(
	icf_mmi_pdb_st		*p_mmi_pdb, 
	icf_sdp_stream_st	*p_sdp
)
{
	icf_boolean_t				echo_cancellation = ICF_FALSE;

        /* To remove warnings */
         p_mmi_pdb=p_mmi_pdb;

	if(ICF_TRUE == p_sdp->media_stream_param.audio_profile.silence_suppression)
	{
		echo_cancellation = ICF_FALSE;
	}
	else
	{
		echo_cancellation = ICF_TRUE;
	}

	return echo_cancellation;
}


/******************************************************************************
 *
 * FUNCTION:    icf_mmi_get_stream_type_for_stream_id
 *
 * DESCRIPTION: 
 *****************************************************************************/

icf_media_type_t	icf_mmi_get_stream_type_for_stream_id
(
	icf_mmi_pdb_st			*p_mmi_pdb,
	icf_stream_id_t			stream_id
)
{
	icf_media_type_t		stream_type = ICF_STREAM_TYPE_INVALID;
	icf_internal_list_st	*p_src = ICF_NULL;

	p_src = p_mmi_pdb->p_call_ctx->p_stream_sdp_info;

	while(ICF_NULL != p_src)
	{
		if(stream_id == ((icf_stream_sdp_info_st *)p_src->p_payload)->stream_id)
		{
			stream_type = 
				((icf_stream_sdp_info_st *)p_src->p_payload)->stream_type;
			break;
		}
		p_src = p_src->p_next;
	}

	return stream_type;
}

/******************************************************************************
 *
 * FUNCTION:    icf_mmi_copy_remote_recvd_sdp_in_mmi_ctxt
 *
 * DESCRIPTION: 
 *****************************************************************************/

icf_return_t	icf_mmi_copy_remote_recvd_sdp_in_mmi_ctxt
(
	icf_mmi_pdb_st			*p_mmi_pdb
)
{
	icf_return_t				ret_val = ICF_SUCCESS,
                                dummy_ret_val = ICF_SUCCESS;
	icf_internal_list_st		*p_src = ICF_NULL;
	icf_inc_media_sdp_info_st	*p_node = ICF_NULL;
	icf_stream_sdp_info_st		*p_data = ICF_NULL;
	icf_void_t					*p_func_ptr = ICF_NULL;
 	icf_uint32_t			    loc_stream = 0;
    icf_return_t                return_val = ICF_SUCCESS;

	ICF_FUNCTION_ENTER(p_mmi_pdb->p_glb_pdb)

    if(ICF_NULL == p_mmi_pdb->p_call_ctx)    
    {
        ret_val = ICF_FAILURE;
        ICF_PRINT(((icf_uint8_t *)"[MMI] : p_mmi_pdb->p_call_ctx is NULL.\n"))
        ICF_FUNCTION_EXIT(p_mmi_pdb->p_glb_pdb)
        return ret_val;
    }
	if(ICF_NULL != p_mmi_pdb->p_mmi_ctx->p_remote_recvd_sdp)
	{
		ret_val = icf_cmn_free_media_sdp_info_internal_list(p_mmi_pdb->p_glb_pdb,
			&(p_mmi_pdb->p_mmi_ctx->p_remote_recvd_sdp),p_mmi_pdb->p_ecode);
        /* SPR 19930: The function icf_cmn_free_media_sdp_info_internal_list
         * does free the tunnelled sdp memory in remote sdp however the
         * pointer for tunnel sdp in remote sdp present of call ctx does
         * not gets reset and this when accessed later was giving FUM when
         * accessed from icf_cc_clear_all_stream_sdp.*/

        /* Fix for SPR: 20094*/
        /* NULL checks placed for p_remote_received_sdp before accessing
         * p_tunnelled_sdp variable.*/

        ICF_CHECK_IF_ANY_OF_THREE_VAR_IS_NULL(p_mmi_pdb->p_glb_pdb,
                p_mmi_pdb->p_call_ctx->p_stream_sdp_info,
                p_mmi_pdb->p_call_ctx->p_stream_sdp_info->p_payload,
                ((icf_stream_sdp_info_st*)(p_mmi_pdb->p_call_ctx->\
                         p_stream_sdp_info->p_payload))->p_remote_received_sdp,
                return_val)
        /* Value of return_val as ICF_SUCCESS indicates that none of above 3
         * variables are NULL; so update the value of p_tunnelled_sdp */    
        if(ICF_SUCCESS == return_val)    
        {
            ((icf_stream_sdp_info_st*)(p_mmi_pdb->p_call_ctx->p_stream_sdp_info->p_payload))
                ->p_remote_received_sdp->p_tunnelled_sdp = ICF_NULL;
        }
	}
	
	p_src = p_mmi_pdb->p_call_ctx->p_stream_sdp_info;

	while(ICF_NULL != p_src)
	{
		p_data = p_src->p_payload;

		if(p_data->p_remote_received_sdp)
		{
			/* MEMGET for p_node as type icf_inc_media_sdp_info_st */
			ICF_MEMGET (p_mmi_pdb->p_glb_pdb,
						ICF_PORT_SIZEOF(icf_inc_media_sdp_info_st),
						ICF_MEM_COMMON,
						p_node,
						ICF_DONOT_RET_ON_FAILURE,
						p_mmi_pdb->p_ecode,
						ret_val)

			if(ICF_SUCCESS == ret_val)
			{
				p_node->stream_id = p_data->stream_id;
				loc_stream = p_node->stream_id;	
				p_node->stream_type = p_data->stream_type;

				p_node->stream_active = 
						(icf_uint8_t)(ICF_MEDIA_STREAM_ACTIVE & p_data->stream_status);

				/* MEMGET for p_node as type icf_inc_media_sdp_info_st */
				ICF_MEMGET (p_mmi_pdb->p_glb_pdb,
							ICF_PORT_SIZEOF(icf_internal_sdp_stream_st),
							ICF_MEM_SDP,
							p_node->p_int_sdp_stream,
							ICF_DONOT_RET_ON_FAILURE,
							p_mmi_pdb->p_ecode,
							ret_val)

				if(ICF_SUCCESS == ret_val)
				{
					icf_port_memcpy(
						(p_node->p_int_sdp_stream),
						(p_data->p_remote_received_sdp),
						ICF_PORT_SIZEOF(icf_internal_sdp_stream_st));

					icf_mmi_clone_sdp_stream_lists(
						p_mmi_pdb,
						&p_data->p_remote_received_sdp->sdp_stream,
						&p_node->p_int_sdp_stream->sdp_stream);

					ret_val = icf_cmn_add_node_to_internal_list(
						p_mmi_pdb->p_glb_pdb, 
						&p_mmi_pdb->p_mmi_ctx->p_remote_recvd_sdp,
						p_node,&loc_stream,
						p_func_ptr);
                    
                    ICF_KLOCWORK_FALSE_POSITIVE(p_mmi_pdb->p_glb_pdb,
                            p_mmi_pdb->p_mmi_ctx->p_remote_recvd_sdp)
				}
				else
				{
					ICF_ALARM(((icf_uint8_t *)"[MMI]: MEMGET failure"));
					break;
				}
			}
			else
			{
				ICF_ALARM(((icf_uint8_t *)"[MMI]: MEMGET failure"));
				break;
			}
		}

		p_src = p_src->p_next;
	}
    if(ICF_FAILURE == ret_val)
    {
        /* MEMFREE p_node */
        ICF_MEMFREE(p_mmi_pdb->p_glb_pdb, p_node, ICF_MEM_COMMON, 
                p_mmi_pdb->p_ecode, dummy_ret_val)
		icf_cmn_delete_internal_list(p_mmi_pdb->p_glb_pdb,
									&p_mmi_pdb->p_mmi_ctx->p_remote_recvd_sdp,
									ICF_MEM_COMMON);
	}

	ICF_FUNCTION_EXIT(p_mmi_pdb->p_glb_pdb)

	return ret_val;
}

/******************************************************************************
 *
 * FUNCTION:    icf_mmi_send_err_ind_call_clear
 *
 * DESCRIPTION: 
 *****************************************************************************/

icf_return_t	icf_mmi_send_err_ind_call_clear
(
	icf_mmi_pdb_st			*p_mmi_pdb
)
{
	icf_return_t			ret_val = ICF_SUCCESS;
	icf_mm_err_ind_st       err_ind;
	icf_internal_msg_st     *p_cc_internal_msg = ICF_NULL;

	ICF_FUNCTION_ENTER(p_mmi_pdb->p_glb_pdb)

    p_cc_internal_msg = p_mmi_pdb->p_internal_msg;
    p_cc_internal_msg->msg_hdr.msg_id = ICF_MEDIA_ERROR_IND;
    p_cc_internal_msg->msg_hdr.payload_length = 0;
    
    err_ind.reason = ICF_ECODE_MMI_INITIATED_CALL_CLEAR;

    p_cc_internal_msg->p_msg_data = (icf_void_t *)&(err_ind);

	/* In tunnel mode, no need to operate on the following bits or
	   clearing of MMI context's buffered requests and responses.
	*/   
ICF_CHECK_TUNNEL_MODE_START(p_mmi_pdb->p_glb_pdb,ICF_FALSE)

	ICF_MMI_RESET_BIT(p_mmi_pdb, ICF_RESTORATION_IN_PROGRESS)

	icf_mmi_clear_buffered_req_resp_in_mmi_ctx(p_mmi_pdb);
ICF_CHECK_TUNNEL_MODE_END

	ICF_MMI_SET_STATE(p_mmi_pdb,ICF_MMI_00_IDLE)

	ret_val = icf_cm_process_mesg(p_cc_internal_msg);

    if(ICF_FAILURE == ret_val)
    {
        /* Failure by CC */
        ICF_MMI_TRACE(p_mmi_pdb,ICF_TRACE_FAILURE_BY_CM)
    }

	ICF_FUNCTION_EXIT(p_mmi_pdb->p_glb_pdb)

	return ret_val;
}


/******************************************************************************
 *
 * FUNCTION:    icf_mmi_send_err_ind_dms_ind
 *
 * DESCRIPTION: 
 *****************************************************************************/

icf_return_t	icf_mmi_send_err_ind_dms_ind
(
	icf_mmi_pdb_st			*p_mmi_pdb
)
{
	icf_return_t			ret_val = ICF_SUCCESS;
	icf_mm_err_ind_st       err_ind;
	icf_internal_msg_st     *p_cc_internal_msg = ICF_NULL;

	ICF_FUNCTION_ENTER(p_mmi_pdb->p_glb_pdb)

    p_cc_internal_msg = p_mmi_pdb->p_internal_msg;
    p_cc_internal_msg->msg_hdr.msg_id = ICF_MEDIA_ERROR_IND;
    p_cc_internal_msg->msg_hdr.payload_length = 0;
    
    err_ind.reason = ICF_ECODE_DMS_IND_RECVD;

    p_cc_internal_msg->p_msg_data = (icf_void_t *)&(err_ind);

	ICF_MMI_RESET_BIT(p_mmi_pdb,ICF_RESTORATION_IN_PROGRESS)


	icf_mmi_clear_buffered_req_resp_in_mmi_ctx(p_mmi_pdb);

	/* since this function is invoked from any state, MMI will move to the
	 * IDLE state after the event is handled */

	ICF_MMI_SET_STATE(p_mmi_pdb,ICF_MMI_00_IDLE)

    ret_val = icf_cm_process_mesg(p_cc_internal_msg);
    if(ICF_FAILURE == ret_val)
    {
        /* Failure by CC */
        ICF_MMI_TRACE(p_mmi_pdb,ICF_TRACE_FAILURE_BY_CM)
    }

	ICF_FUNCTION_EXIT(p_mmi_pdb->p_glb_pdb)

	return ret_val;
}


/******************************************************************************
 *
 * FUNCTION:    icf_mmi_send_delete_media_response
 *
 * DESCRIPTION: 
 *****************************************************************************/

icf_return_t	icf_mmi_send_delete_media_response
(
	icf_mmi_pdb_st			*p_mmi_pdb
)
{
	icf_return_t			ret_val = ICF_SUCCESS;
	icf_internal_msg_st     *p_cc_internal_msg = ICF_NULL;

	ICF_FUNCTION_ENTER(p_mmi_pdb->p_glb_pdb)

	p_cc_internal_msg = p_mmi_pdb->p_internal_msg;
    p_cc_internal_msg->msg_hdr.msg_id = ICF_DELETE_MEDIA_RESP;
    p_cc_internal_msg->msg_hdr.payload_length = 0;
   
	/* In tunnel mode, no need to operate on the following bits or
	   clearing of MMI context's buffered requests and responses.
	*/   
ICF_CHECK_TUNNEL_MODE_START(p_mmi_pdb->p_glb_pdb,ICF_FALSE)
	ICF_MMI_RESET_BIT(p_mmi_pdb,ICF_RESTORATION_IN_PROGRESS)

    ICF_MMI_SET_BIT(p_mmi_pdb,ICF_DELETE_COMPLETE)

	icf_mmi_clear_buffered_req_resp_in_mmi_ctx(p_mmi_pdb);
ICF_CHECK_TUNNEL_MODE_END

	ICF_MMI_SET_STATE(p_mmi_pdb,ICF_MMI_00_IDLE)

	/* Trigger CC to send the success DMS response */
    ret_val = icf_cm_process_mesg(p_cc_internal_msg);

    if(ICF_FAILURE == ret_val)
    {
        /* Failure to process msg by CC */
        ICF_MMI_TRACE(p_mmi_pdb,ICF_TRACE_FAILURE_BY_CM)
    }

	ICF_FUNCTION_EXIT(p_mmi_pdb->p_glb_pdb)

	return ret_val;
}


/******************************************************************************
 *
 * FUNCTION:    icf_mmi_send_modify_media_response
 *
 * DESCRIPTION: 
 *****************************************************************************/

icf_return_t	icf_mmi_send_modify_media_response
(
	icf_mmi_pdb_st			*p_mmi_pdb
)
{
	icf_return_t						ret_val = ICF_SUCCESS;
	icf_internal_msg_st					*p_cc_internal_msg = ICF_NULL;
	icf_mmi_internal_mms_resp_st		*p_int_resp = ICF_NULL;
	icf_mmi_mms_resp_st					mms_resp;
	icf_stream_id_t						stream_count = ICF_NULL;

	ICF_FUNCTION_ENTER(p_mmi_pdb->p_glb_pdb)

	p_cc_internal_msg = p_mmi_pdb->p_internal_msg;
	p_cc_internal_msg->msg_hdr.msg_id = ICF_MODIFY_MEDIA_RESP;
	p_cc_internal_msg->msg_hdr.payload_length = 
										ICF_PORT_SIZEOF(icf_mmi_mms_resp_st);
	p_int_resp = p_mmi_pdb->p_mmi_ctx->p_mmi_resp;

	mms_resp.result = p_int_resp->result;
	mms_resp.bit_mask = p_int_resp->bit_mask;

	if(ICF_SUCCESS == mms_resp.result)
	{
		mms_resp.add_stream_count = p_int_resp->add_stream_count;

		for(stream_count = 0; (stream_count  < mms_resp.add_stream_count) && (stream_count < ICF_MAX_STREAM_PER_CALL); 
			stream_count++)
		{
			mms_resp.added_stream_resp[stream_count].stream_id = 
						p_int_resp->added_stream_resp[stream_count].stream_id;

			mms_resp.added_stream_resp[stream_count].status_success = 
					p_int_resp->added_stream_resp[stream_count].status_success;
		}

		mms_resp.mod_stream_count = p_int_resp->mod_stream_count;

		for(stream_count = 0; (stream_count  < mms_resp.mod_stream_count) && (stream_count < ICF_MAX_STREAM_PER_CALL); 
			stream_count++)
		{
			mms_resp.modified_stream_resp[stream_count].stream_id = 
				p_int_resp->modified_stream_resp[stream_count].stream_id;

			mms_resp.modified_stream_resp[stream_count].status_success = 
				p_int_resp->modified_stream_resp[stream_count].status_success;
		}
	}	
	p_cc_internal_msg->p_msg_data = (icf_void_t *)&(mms_resp);

	ICF_MMI_RESET_BIT(p_mmi_pdb,ICF_RESTORATION_IN_PROGRESS)

	icf_mmi_clear_buffered_req_resp_in_mmi_ctx(p_mmi_pdb);

	ICF_MMI_SET_STATE(p_mmi_pdb,ICF_MMI_00_IDLE)

	ret_val = icf_cm_process_mesg(p_cc_internal_msg);

	if(ICF_FAILURE == ret_val)
	{
		/* Failure by CC */
		ICF_MMI_TRACE(p_mmi_pdb,ICF_TRACE_FAILURE_BY_CM)
	}

	ICF_FUNCTION_EXIT(p_mmi_pdb->p_glb_pdb)

	return ret_val;
}


/******************************************************************************
 *
 * FUNCTION:    icf_mmi_send_create_media_response
 *
 * DESCRIPTION: 
 *****************************************************************************/

icf_return_t	icf_mmi_send_create_media_response
(
	icf_mmi_pdb_st			*p_mmi_pdb
)
{
	icf_return_t			ret_val = ICF_SUCCESS;
	icf_internal_msg_st     *p_cc_internal_msg = ICF_NULL;
	icf_mmi_internal_cms_resp_st		*p_int_resp = ICF_NULL;
	icf_mmi_cms_resp_st					cms_resp;
	icf_stream_id_t						stream_count = ICF_NULL;

	ICF_FUNCTION_ENTER(p_mmi_pdb->p_glb_pdb)

	p_cc_internal_msg = p_mmi_pdb->p_internal_msg;
	p_cc_internal_msg->msg_hdr.msg_id = ICF_CREATE_MEDIA_RESP;
	p_cc_internal_msg->msg_hdr.payload_length = 
						ICF_PORT_SIZEOF(icf_mmi_cms_resp_st);

	p_int_resp = p_mmi_pdb->p_mmi_ctx->p_mmi_resp;

	cms_resp.result = p_int_resp->result;
        cms_resp.error_cause = ICF_NULL;

	if(ICF_SUCCESS == cms_resp.result)
	{
		cms_resp.count = p_int_resp->count;
		for(stream_count = 0; (stream_count < cms_resp.count) && (stream_count < ICF_MAX_STREAM_PER_CALL) ; stream_count++)
		{
			cms_resp.stream_status[stream_count].stream_id = 
				p_int_resp->stream_status[stream_count].stream_id;
			
			cms_resp.stream_status[stream_count].status_success = 
				p_int_resp->stream_status[stream_count].status_success;
		}
	}
    else
    {
     if(ICF_NULL != p_int_resp->error_cause)
       {
        cms_resp.error_cause = p_int_resp->error_cause;
        switch (p_int_resp->error_cause) 
        {
          case ICF_CAUSE_UNABLE_TO_CREATE_MEDIA: 
            {
             p_mmi_pdb->p_call_ctx->release_cause = ICF_CALL_CLEAR_REASON_UNABLE_TO_CREATE_MEDIA;
             break; 
            }
        
          case ICF_CAUSE_TRANSPORT_LAYER_FAILURE: 
            {
             p_mmi_pdb->p_call_ctx->release_cause = ICF_CALL_CLEAR_REASON_MEDIA_TRANSPORT_FAILURE;
             break; 
            }
          default:
             break;
        }
       }
    }
	p_cc_internal_msg->p_msg_data = (icf_void_t *)&(cms_resp);

	ICF_MMI_RESET_BIT(p_mmi_pdb,ICF_RESTORATION_IN_PROGRESS)

	icf_mmi_clear_buffered_req_resp_in_mmi_ctx(p_mmi_pdb);

	ICF_MMI_SET_STATE(p_mmi_pdb,ICF_MMI_00_IDLE)

	ret_val = icf_cm_process_mesg(p_cc_internal_msg);

	if(ICF_FAILURE == ret_val)
	{
		/* Failure by CC */
		ICF_MMI_TRACE(p_mmi_pdb,ICF_TRACE_FAILURE_BY_CM)
	}

	ICF_FUNCTION_EXIT(p_mmi_pdb->p_glb_pdb)

	return ret_val;
}

/******************************************************************************
 *
 * FUNCTION:    icf_mmi_send_awaited_dms_req
 *
 * DESCRIPTION: 
 *****************************************************************************/

icf_return_t	icf_mmi_send_awaited_dms_req
(
	icf_mmi_pdb_st			*p_mmi_pdb
)
{
	icf_return_t			ret_val = ICF_SUCCESS;

	ICF_FUNCTION_ENTER(p_mmi_pdb->p_glb_pdb)

	p_mmi_pdb->p_mmi_ctx->ongoing_event = ICF_MMI_DMS_ONGOING;

	/* This means that CMS was successful so we need to send DMS */
	if(ICF_MEDIA_ESTABLISHED & p_mmi_pdb->p_mmi_ctx->mmi_bitmask)
	{
		ICF_MMI_RESET_BIT(p_mmi_pdb,ICF_MEDIA_ESTABLISHED)

		/* Send DMS message to MM and wait to send Release Resource */
		ret_val = icf_mmi_send_dms_req(p_mmi_pdb);

		if(ICF_SUCCESS == ret_val)
		{
			/* Change the state */
			ICF_MMI_SET_STATE(p_mmi_pdb,ICF_MMI_05_AWT_DMS_RESP)
			ICF_MMI_TRACE(p_mmi_pdb,ICF_TRACE_API_SEND)    
		}
		else
		{
			icf_mmi_send_rel_res_ind(p_mmi_pdb);

			ICF_MMI_PDB_SET_BIT(p_mmi_pdb,ICF_MMI_PDB_ALL_STREAMS)
			
			/* send close channel for all streams */
			icf_mmi_send_close_channel_ind(p_mmi_pdb); 

			ICF_MMI_PDB_RESET_BIT(p_mmi_pdb,ICF_MMI_PDB_ALL_STREAMS)
#ifdef ICF_NAT_MEDIA_TRAVERSAL
			icf_mmi_rel_nat_resources(p_mmi_pdb);
#endif
			ret_val = icf_mmi_send_delete_media_response(p_mmi_pdb);
		}
	}
	else
	if(ICF_MMI_CHANNEL_OPENED & p_mmi_pdb->p_mmi_ctx->mmi_bitmask)
	{
		/* Send REL_RES_IND */
		p_mmi_pdb->p_mmi_ctx->ongoing_event = ICF_MMI_DMS_ONGOING;
        icf_mmi_send_rel_res_ind(p_mmi_pdb);

		/* Need to send close_channel because we are doing no DMS */
		ICF_MMI_PDB_SET_BIT(p_mmi_pdb,ICF_MMI_PDB_ALL_STREAMS)
		icf_mmi_send_close_channel_ind(p_mmi_pdb); 
		ICF_MMI_PDB_RESET_BIT(p_mmi_pdb,ICF_MMI_PDB_ALL_STREAMS)

		ICF_MMI_RESET_BIT(p_mmi_pdb,ICF_MMI_CHANNEL_OPENED)

		/* send response for DMS_REQ to CC */
		ret_val = icf_mmi_send_delete_media_response(p_mmi_pdb);
	}
	else
	if(ICF_MMI_RES_RESERVED & p_mmi_pdb->p_mmi_ctx->mmi_bitmask)
	{
		/* Send REL_RES_IND */
		p_mmi_pdb->p_mmi_ctx->ongoing_event = ICF_MMI_DMS_ONGOING;
        icf_mmi_send_rel_res_ind(p_mmi_pdb);

		ICF_MMI_RESET_BIT(p_mmi_pdb,ICF_MMI_RES_RESERVED)

		/* send response for DMS_REQ to CC */
		ret_val = icf_mmi_send_delete_media_response(p_mmi_pdb);
	}
	else
	{
		ret_val = icf_mmi_send_delete_media_response(p_mmi_pdb);
	}

	ICF_FUNCTION_EXIT(p_mmi_pdb->p_glb_pdb)

	return ret_val;
}


/******************************************************************************
 *
 * FUNCTION:    icf_mmi_send_forceful_dms_req
 *
 * DESCRIPTION: 
 *****************************************************************************/

icf_return_t	icf_mmi_send_forceful_dms_req
(
	icf_mmi_pdb_st			*p_mmi_pdb
)
{
	icf_return_t			ret_val = ICF_SUCCESS;

	ICF_FUNCTION_ENTER(p_mmi_pdb->p_glb_pdb)

	p_mmi_pdb->p_mmi_ctx->ongoing_event = ICF_MMI_DMS_ONGOING;

	/* This means that CMS was successful so we need to send DMS */
	if(ICF_MEDIA_ESTABLISHED & p_mmi_pdb->p_mmi_ctx->mmi_bitmask)
	{
		/* Send DMS message to MM and wait to send Release Resource */
		ret_val = icf_mmi_send_dms_req(p_mmi_pdb);

		if(ICF_SUCCESS == ret_val)
		{
			/* Change the state */
			ICF_MMI_SET_STATE(p_mmi_pdb,ICF_MMI_05_AWT_DMS_RESP)
			ICF_MMI_TRACE(p_mmi_pdb,ICF_TRACE_API_SEND)    
		}
		ICF_MMI_SET_STATE(p_mmi_pdb,ICF_MMI_00_IDLE)

		icf_mmi_send_rel_res_ind(p_mmi_pdb);

		ICF_MMI_PDB_SET_BIT(p_mmi_pdb,ICF_MMI_PDB_ALL_STREAMS)
			
		/* send close channel for all streams */
		icf_mmi_send_close_channel_ind(p_mmi_pdb); 

		ICF_MMI_PDB_RESET_BIT(p_mmi_pdb,ICF_MMI_PDB_ALL_STREAMS)
#ifdef ICF_NAT_MEDIA_TRAVERSAL
		icf_mmi_rel_nat_resources(p_mmi_pdb);
#endif
		ICF_MMI_RESET_BIT(p_mmi_pdb,ICF_MEDIA_ESTABLISHED)
	}
	else
	if(ICF_MMI_CHANNEL_OPENED & p_mmi_pdb->p_mmi_ctx->mmi_bitmask)
	{
		/* Send REL_RES_IND */
		p_mmi_pdb->p_mmi_ctx->ongoing_event = ICF_MMI_DMS_ONGOING;
        icf_mmi_send_rel_res_ind(p_mmi_pdb);

		/* Need to send close_channel because we are doing no DMS */
		ICF_MMI_PDB_SET_BIT(p_mmi_pdb,ICF_MMI_PDB_ALL_STREAMS)
		icf_mmi_send_close_channel_ind(p_mmi_pdb); 
		ICF_MMI_PDB_RESET_BIT(p_mmi_pdb,ICF_MMI_PDB_ALL_STREAMS)

		ICF_MMI_RESET_BIT(p_mmi_pdb,ICF_MMI_CHANNEL_OPENED)
	}
	else
	if(ICF_MMI_RES_RESERVED & p_mmi_pdb->p_mmi_ctx->mmi_bitmask)
	{
		p_mmi_pdb->p_mmi_ctx->ongoing_event = ICF_MMI_DMS_ONGOING;
        icf_mmi_send_rel_res_ind(p_mmi_pdb);

		ICF_MMI_RESET_BIT(p_mmi_pdb,ICF_MMI_RES_RESERVED)
	}

	ret_val = icf_mmi_send_delete_media_response(p_mmi_pdb);

	ICF_FUNCTION_EXIT(p_mmi_pdb->p_glb_pdb)

	return ret_val;
}


/******************************************************************************
 *
 * FUNCTION:    icf_mmi_clear_buffered_req_resp_in_mmi_ctx
 *
 * DESCRIPTION: 
 *****************************************************************************/

icf_return_t	icf_mmi_clear_buffered_req_resp_in_mmi_ctx
(
	icf_mmi_pdb_st		*p_mmi_pdb
)
{
	/* local variables */	
	icf_return_t		ret_val = ICF_SUCCESS;

	ICF_FUNCTION_ENTER(p_mmi_pdb->p_glb_pdb)

	if (ICF_NULL != p_mmi_pdb->p_mmi_ctx->p_mms_req)
	{
		icf_mmi_mms_request_st	*p_mms_req = ICF_NULL;
		
		p_mms_req = (icf_mmi_mms_request_st *)
							(p_mmi_pdb->p_mmi_ctx->p_mms_req);
		
		if (ICF_MMI_MMS_ACTION_ADD & p_mms_req->bit_mask)
		{
			ICF_MEMFREE(p_mmi_pdb->p_glb_pdb, 
				p_mms_req->p_add_media,
				ICF_MEM_COMMON, 
				p_mmi_pdb->p_ecode, 
				ret_val)
		}

		if (ICF_MMI_MMS_ACTION_DELETE & p_mms_req->bit_mask)
		{
			ICF_MEMFREE(p_mmi_pdb->p_glb_pdb, 
				p_mms_req->p_delete_media,
				ICF_MEM_COMMON, 
				p_mmi_pdb->p_ecode, 
				ret_val)
		}
		
		ICF_MEMFREE(p_mmi_pdb->p_glb_pdb, 
				p_mmi_pdb->p_mmi_ctx->p_mms_req,
				ICF_MEM_COMMON, 
				p_mmi_pdb->p_ecode, 
				ret_val)
	}

	if (ICF_NULL != p_mmi_pdb->p_mmi_ctx->p_mmi_resp)
	{
		/* MEMFREE p_mmi_pdb->p_mmi_ctx->p_mmi_resp */
		ICF_MEMFREE(p_mmi_pdb->p_glb_pdb, 
				p_mmi_pdb->p_mmi_ctx->p_mmi_resp,
				ICF_MEM_COMMON, 
				p_mmi_pdb->p_ecode, 
				ret_val)
	}

	ICF_FUNCTION_EXIT(p_mmi_pdb->p_glb_pdb)

	return ret_val;
}


/******************************************************************************
 *
 * FUNCTION:    icf_mmi_check_sdp_in_call_ctx
 *
 * DESCRIPTION: 
 *
 *****************************************************************************/

icf_return_t icf_mmi_check_sdp_in_call_ctx
(
	IN		icf_mmi_pdb_st		*p_mmi_pdb,
	IN		icf_stream_id_t		stream_id,
	IN		icf_uint8_t			sdp_type
)
{
	icf_return_t					ret_val = ICF_FAILURE;
	icf_internal_sdp_stream_st		*p_int_sdp = ICF_NULL;
	icf_internal_list_st			*p_int_list = ICF_NULL;

	ICF_FUNCTION_ENTER(p_mmi_pdb->p_glb_pdb)

	ret_val = icf_cmn_fetch_sdp_in_list_for_stream_id(
								p_mmi_pdb->p_glb_pdb,
								stream_id,
								sdp_type,
								&p_int_sdp);

	/* This means that sdp is not present in stream_sdp_info_list
	 * so MMI shall MEMGET for local sdp for this stream */
	if(ICF_FAILURE == ret_val)
	{
		icf_uint32_t	loc_stream = 0;
		loc_stream = stream_id;
		p_int_list = icf_cmn_get_node_in_internal_list(
						p_mmi_pdb->p_call_ctx->p_stream_sdp_info,
						&loc_stream);

		if(ICF_NULL != p_int_list)
		{
			switch(sdp_type)
			{
				case ICF_SDP_LOCAL:

					ICF_MEMGET (
						p_mmi_pdb->p_glb_pdb,
						ICF_PORT_SIZEOF(icf_internal_sdp_stream_st),
						ICF_MEM_SDP,
						((icf_stream_sdp_info_st *)p_int_list->p_payload)->\
							p_local_sdp,
						ICF_RET_ON_FAILURE,
						p_mmi_pdb->p_ecode,
						ret_val)

						if(ICF_FAILURE == ret_val)
						{
							ICF_ALARM(((icf_uint8_t *)"[MMI]: MEMGET failure"));
						}
						else
						{
							icf_port_memset(((icf_stream_sdp_info_st *)p_int_list->\
								p_payload)->p_local_sdp,0, 
								ICF_PORT_SIZEOF(icf_internal_sdp_stream_st));
						}
				break;

				case ICF_SDP_APPLIED:

					ICF_MEMGET (
						p_mmi_pdb->p_glb_pdb,
						ICF_PORT_SIZEOF(icf_internal_sdp_stream_st),
						ICF_MEM_SDP,
						((icf_stream_sdp_info_st *)p_int_list->p_payload)->\
							p_applied_sdp,
						ICF_RET_ON_FAILURE,
						p_mmi_pdb->p_ecode,
						ret_val)

						if(ICF_FAILURE == ret_val)
						{
							ICF_ALARM(((icf_uint8_t *)"[MMI]: MEMGET failure"));
						}
						else
						{
							icf_port_memset(((icf_stream_sdp_info_st *)p_int_list->\
								p_payload)->p_applied_sdp,0, 
								ICF_PORT_SIZEOF(icf_internal_sdp_stream_st));
						}
			}
		}
	}

	ICF_FUNCTION_EXIT(p_mmi_pdb->p_glb_pdb)

	return ret_val;
}

#ifndef ICF_UNIT_TEST
/******************************************************************************
 *
 * FUNCTION:    icf_mmi_check_api_total_streams
 *
 * DESCRIPTION: 
 *****************************************************************************/

icf_return_t	icf_mmi_check_api_total_streams
(
	IN icf_mmi_pdb_st	*p_mmi_pdb,
	IN icf_api_id_t		api_id
)
{
	icf_return_t				ret_val = ICF_FAILURE;
	icf_stream_id_list_st		*p_reject_list = ICF_NULL,
						         accpt_list;
	icf_list_st					*p_accpt_list = ICF_NULL;
	icf_stream_id_t				total_count = ICF_NULL,
								count = ICF_NULL,
								loc_count = ICF_NULL;
						
    icf_port_memset(&accpt_list,0,sizeof(icf_stream_id_list_st));
	
	ICF_FUNCTION_ENTER(p_mmi_pdb->p_glb_pdb)

	accpt_list.count = ICF_NULL;

	/* Fetch the list of streams */
	switch(api_id)
	{
		case ICF_GET_CODEC_LIST_RESP:
			p_accpt_list = 
			((icf_get_codec_list_resp_st *)p_mmi_pdb->p_glb_pdb->p_recv_msg->\
			payload)->p_reserved_stream_list;

			while((p_accpt_list) && (accpt_list.count < ICF_MAX_STREAM_PER_CALL))
			{
				accpt_list.stream_id[accpt_list.count] = 
					((icf_stream_capab_st*)p_accpt_list->p_data)->stream_id;

				accpt_list.count++;
				p_accpt_list = p_accpt_list->p_next;
			}

			if(((icf_get_codec_list_resp_st *)p_mmi_pdb->p_glb_pdb->\
				p_recv_msg->payload)->bit_mask &
				ICF_CODEC_LIST_REJECTED_STREAM_LIST)
			{
				p_reject_list = 
				&(((icf_get_codec_list_resp_st *)p_mmi_pdb->p_glb_pdb->p_recv_msg->\
				payload)->rejected_stream_list);
			}
			break;

		case ICF_OPEN_MEDIA_CHANNEL_RESP:
			p_accpt_list = 
			((icf_open_media_channel_resp_st *)p_mmi_pdb->p_glb_pdb->p_recv_msg->\
			payload)->p_channel_addr_list;

			while((p_accpt_list) && (accpt_list.count < ICF_MAX_STREAM_PER_CALL))
			{
				accpt_list.stream_id[accpt_list.count] = 
					((icf_channel_addr_st*)p_accpt_list->p_data)->stream_id;

				accpt_list.count++;
				p_accpt_list = p_accpt_list->p_next;
			}

			break;

		case ICF_CREATE_MEDIA_SESSION_RESP:
			p_accpt_list = 
			((icf_create_media_session_resp_st *)p_mmi_pdb->p_glb_pdb->p_recv_msg->\
			payload)->p_accepted_stream_list;

			while((p_accpt_list) && (accpt_list.count < ICF_MAX_STREAM_PER_CALL))
			{
				accpt_list.stream_id[accpt_list.count] = 
					((icf_resp_stream_st*)p_accpt_list->p_data)->stream_id;

				accpt_list.count++;
				p_accpt_list = p_accpt_list->p_next;
			}

			if(((icf_create_media_session_resp_st *)p_mmi_pdb->p_glb_pdb->\
				p_recv_msg->payload)->bit_mask &
				ICF_CMS_REJECT_STREAM_PRESENT)
			{
				p_reject_list = 
				&(((icf_create_media_session_resp_st *)p_mmi_pdb->p_glb_pdb->\
				p_recv_msg->payload)->rejected_stream_list);
			}

			break;

		case ICF_CODEC_COMMIT_RESP:
			p_accpt_list = 
			((icf_codec_commit_resp_st *)p_mmi_pdb->p_glb_pdb->p_recv_msg->\
			payload)->p_commited_stream_list;

			while((p_accpt_list) && (accpt_list.count < ICF_MAX_STREAM_PER_CALL))
			{
				accpt_list.stream_id[accpt_list.count] = 
					((icf_stream_commit_st*)p_accpt_list->p_data)->stream_id;

				accpt_list.count++;
				p_accpt_list = p_accpt_list->p_next;
			}

			if(((icf_codec_commit_resp_st *)p_mmi_pdb->p_glb_pdb->p_recv_msg->\
				payload)->bit_mask &
				ICF_CODEC_COMMIT_RESP_REJ_STREAM_LIST_PRESENT)
			{
				p_reject_list = 
				&(((icf_codec_commit_resp_st *)p_mmi_pdb->p_glb_pdb->p_recv_msg->\
				payload)->rejected_stream_list);
			}

			break;

		case ICF_MODIFY_MEDIA_SESSION_RESP:
			p_accpt_list = 
			((icf_modify_media_session_resp_st *)p_mmi_pdb->p_glb_pdb->\
			p_recv_msg->payload)->p_accepted_stream_list;

			while((p_accpt_list) && (accpt_list.count < ICF_MAX_STREAM_PER_CALL))
			{
				accpt_list.stream_id[accpt_list.count] = 
					((icf_resp_stream_st*)p_accpt_list->p_data)->stream_id;

				accpt_list.count++;
				p_accpt_list = p_accpt_list->p_next;
			}
			
			if(((icf_modify_media_session_resp_st *)p_mmi_pdb->p_glb_pdb->\
				p_recv_msg->payload)->bit_mask &
				ICF_MODIFY_MEDIA_RESP_REJECT_STREAM_PRESENT)
			{			
				p_reject_list = 
				&(((icf_modify_media_session_resp_st *)p_mmi_pdb->p_glb_pdb->\
				p_recv_msg->payload)->rejected_stream_list);
			}
			break;
	}

	if(p_reject_list)
	{
		total_count = (icf_uint8_t)(accpt_list.count + p_reject_list->count);
	}
	else
	{
		total_count = accpt_list.count;
	}

	if(p_mmi_pdb->p_mmi_ctx->api_stream_count == total_count)
	{
		ret_val = ICF_SUCCESS;
	}

	/* Added as a fix for SPR: 12577 */
	if(ICF_SUCCESS == ret_val)
	{
		/* search for each stream that was sent in the request */
		for(count =0; (count < total_count) && (count < ICF_MAX_STREAM_PER_CALL); count++)
		{
			ret_val = ICF_FAILURE;

			if(p_reject_list)
			{
				for(loc_count = 0; (loc_count < p_reject_list->count) && (loc_count < ICF_MAX_STREAM_PER_CALL); loc_count++)
				{
					if(p_mmi_pdb->p_mmi_ctx->api_stream[count] == 
						p_reject_list->stream_id[loc_count])
					{
						ret_val = ICF_SUCCESS;
						break;
					}
				}
			}

			if(ICF_FAILURE == ret_val)
			{
				for(loc_count = 0; loc_count < accpt_list.count; loc_count++)
				{
					if(p_mmi_pdb->p_mmi_ctx->api_stream[count] == 
						accpt_list.stream_id[loc_count])
					{
						ret_val = ICF_SUCCESS;
						break;
					}					
				}
			}

			/* failure means that this stream is not present in the response */
			if(ICF_FAILURE == ret_val)
			{
				break;
			}
		}
	}

	ICF_FUNCTION_EXIT(p_mmi_pdb->p_glb_pdb)

	return ret_val;
}
#endif


/******************************************************************************
 *
 * FUNCTION:    icf_mmi_free_mm_req
 *
 * DESCRIPTION: 
 *****************************************************************************/

icf_return_t	icf_mmi_free_mm_req
(
	IN    icf_mmi_pdb_st	*p_mmi_pdb,
	IN    icf_msg_st		*p_msg_data
)
{
	icf_return_t				ret_val = ICF_SUCCESS;
	icf_list_st					*p_sdp = ICF_NULL,
								*p_backup_sdp = ICF_NULL;
	icf_list_st					*p_codec_list = ICF_NULL;
	icf_list_st					*p_enc_attrib_list = ICF_NULL;
	icf_void_t					*p_mmi_msg_payload = ICF_NULL;
    icf_list_st					*p_local_sdp = ICF_NULL;
 	icf_list_st					*p_backup_local_sdp = ICF_NULL;
 	icf_list_st					*p_local_codec_list = ICF_NULL;
 	icf_list_st					*p_local_enc_attrib_list = ICF_NULL;

	ICF_FUNCTION_ENTER(p_mmi_pdb->p_glb_pdb)

	p_mmi_msg_payload = (icf_void_t*)(p_msg_data->payload);

	switch(p_msg_data->hdr.api_id)
	{
        case ICF_RECV_UDP_DATA_IND:
            return ICF_SUCCESS;
            break;

		case ICF_GET_CODEC_LIST_REQ:
			p_sdp = 
			((icf_get_codec_list_req_st *)p_mmi_msg_payload)->
													p_stream_capab_list;
           	if(ICF_LOCAL_STREAM_CAPAB_INFO_PRESENT & ((icf_get_codec_list_req_st *)p_mmi_msg_payload)->bit_mask)
			{
				p_local_sdp = 
					((icf_get_codec_list_req_st *)p_mmi_msg_payload)->p_local_stream_capab_list;
			}
			break;

		case ICF_OPEN_MEDIA_CHANNEL_REQ:
			p_sdp = 
			((icf_open_media_channel_req_st *)p_mmi_msg_payload)->p_channel_list;
			break;

		case ICF_CLOSE_MEDIA_CHANNEL_REQ:
			p_sdp = 
			((icf_close_media_channel_req_st *)p_mmi_msg_payload)->\
			p_channel_list;
			break;

		case ICF_CREATE_MEDIA_SESSION_REQ:
			p_sdp = 
			((icf_create_media_session_req_st *)p_mmi_msg_payload)->
														p_stream_list_data;
			break;

		case ICF_CODEC_COMMIT_REQ:
			p_sdp = 
			((icf_codec_commit_req_st *)p_mmi_msg_payload)->
													p_stream_commit_list;
			break;

		case ICF_MODIFY_MEDIA_SESSION_REQ:
			p_sdp = 
			((icf_modify_media_session_req_st *)p_mmi_msg_payload)->
													p_stream_data_list;
			break;
	}

	p_backup_sdp = p_sdp;
    p_backup_local_sdp = p_local_sdp;

	while(ICF_NULL != p_sdp)
	{
		switch(p_msg_data->hdr.api_id)
		{
			case ICF_GET_CODEC_LIST_REQ:
				p_codec_list = 
					((icf_stream_capab_st *)p_sdp->p_data)->p_codec_info_list;
				p_enc_attrib_list = 
					((icf_stream_capab_st *)p_sdp->p_data)->p_encoded_attributes;

				break;

			case ICF_OPEN_MEDIA_CHANNEL_REQ:
				p_codec_list = ICF_NULL;
				p_enc_attrib_list = ICF_NULL;
				break;

			case ICF_CLOSE_MEDIA_CHANNEL_REQ:
				p_codec_list = ICF_NULL;
				p_enc_attrib_list = ICF_NULL;
				break;

			case ICF_CREATE_MEDIA_SESSION_REQ:
				p_codec_list = 
					((icf_req_stream_st *)p_sdp->p_data)->\
					remote_sdp.p_codec_info_list;
				p_enc_attrib_list = 
					((icf_req_stream_st *)p_sdp->p_data)->\
					remote_sdp.p_encoded_attributes;

				if(p_codec_list)
				{
					icf_cmn_delete_list(p_mmi_pdb->p_glb_pdb,&p_codec_list,
									ICF_MEM_COMMON);
				}

				if(p_enc_attrib_list)
				{
					icf_cmn_delete_list(p_mmi_pdb->p_glb_pdb,&p_enc_attrib_list,
									ICF_MEM_COMMON);		
				}

				p_codec_list = 
					((icf_req_stream_st *)p_sdp->p_data)->\
					local_sdp.p_codec_info_list;
				p_enc_attrib_list = 
					((icf_req_stream_st *)p_sdp->p_data)->\
					local_sdp.p_encoded_attributes;

				break;

			case ICF_CODEC_COMMIT_REQ:
				p_codec_list = 
				((icf_stream_capab_st *)p_sdp->p_data)->p_codec_info_list;
				p_enc_attrib_list = 
				((icf_stream_capab_st *)p_sdp->p_data)->p_encoded_attributes;
				break;

			case ICF_MODIFY_MEDIA_SESSION_REQ:
				p_codec_list = 
					((icf_modify_stream_st *)p_sdp->p_data)->\
					sdp_stream.p_codec_info_list;
				p_enc_attrib_list = 
					((icf_modify_stream_st *)p_sdp->p_data)->\
					sdp_stream.p_encoded_attributes;
				break;
		}

		if(p_codec_list)
		{
			icf_cmn_delete_list(p_mmi_pdb->p_glb_pdb,&p_codec_list,
							ICF_MEM_COMMON);
		}

		if(p_enc_attrib_list)
		{
			icf_cmn_delete_list(p_mmi_pdb->p_glb_pdb,&p_enc_attrib_list,
							ICF_MEM_COMMON);		
		}

		p_sdp = p_sdp->p_next;
	}
    /* Delete the local codec list after sending GCL */
    while(ICF_NULL != p_local_sdp)
	{
		switch(p_msg_data->hdr.api_id)
		{
			case ICF_GET_CODEC_LIST_REQ:
				p_local_codec_list = 
					((icf_stream_capab_st *)p_local_sdp->p_data)->p_codec_info_list;
				p_local_enc_attrib_list = 
					((icf_stream_capab_st *)p_local_sdp->p_data)->p_encoded_attributes;

				break;
		}
		if(p_local_codec_list)
		{
			icf_cmn_delete_list(p_mmi_pdb->p_glb_pdb,&p_local_codec_list,
							ICF_MEM_COMMON);
		}

		if(p_local_enc_attrib_list)
		{
			icf_cmn_delete_list(p_mmi_pdb->p_glb_pdb,&p_local_enc_attrib_list,
							ICF_MEM_COMMON);		
		}

		p_local_sdp = p_local_sdp->p_next;
	}

	if(p_backup_sdp)
	{
		icf_cmn_delete_list(p_mmi_pdb->p_glb_pdb,&p_backup_sdp,ICF_MEM_COMMON);
	}
	
	if(p_backup_local_sdp)
	{
		icf_cmn_delete_list(p_mmi_pdb->p_glb_pdb,&p_backup_local_sdp,ICF_MEM_COMMON);
    }

	ICF_FUNCTION_EXIT(p_mmi_pdb->p_glb_pdb)

	return ret_val;
}


/******************************************************************************
 *
 * FUNCTION:    icf_mmi_negotiate_amr_params
 *
 * DESCRIPTION: 
 *
 *****************************************************************************/

icf_return_t icf_mmi_negotiate_amr_params
(
	INOUT      icf_mmi_pdb_st    *p_mmi_pdb
)
{
	icf_return_t					ret_val = ICF_SUCCESS,
									dummy_ret_val = ICF_SUCCESS;
	icf_mmi_internal_mms_resp_st	*p_mms_resp = ICF_NULL;
	icf_mmi_internal_cms_resp_st	*p_cms_resp = ICF_NULL;
	icf_stream_id_t					stream_count = ICF_NULL,
									max_stream = ICF_NULL,
									stream_id = ICF_NULL;
	icf_media_type_t				stream_type = ICF_STREAM_TYPE_INVALID;
	icf_sdp_stream_st				sdp;

	ICF_FUNCTION_ENTER(p_mmi_pdb->p_glb_pdb)

    return ret_val;

	icf_port_memset(&sdp, 0, ICF_PORT_SIZEOF(sdp));

	if(ICF_MMI_CMS_ONGOING == p_mmi_pdb->p_mmi_ctx->ongoing_event)
	{
		p_cms_resp = p_mmi_pdb->p_mmi_ctx->p_mmi_resp;
		max_stream = p_cms_resp->count;
	}
	else
	if(ICF_MMI_MMS_MOD_ONGOING == p_mmi_pdb->p_mmi_ctx->ongoing_event)
	{
		p_mms_resp = p_mmi_pdb->p_mmi_ctx->p_mmi_resp;
		max_stream = p_mms_resp->mod_stream_count;
	}
	else
	if(ICF_MMI_MMS_ADD_ONGOING == p_mmi_pdb->p_mmi_ctx->ongoing_event)
	{
		p_mms_resp = p_mmi_pdb->p_mmi_ctx->p_mmi_resp;
		max_stream = p_mms_resp->add_stream_count;	
	}

	for(stream_count = 0; stream_count < max_stream; stream_count++)
	{
		if(ICF_MMI_CMS_ONGOING == p_mmi_pdb->p_mmi_ctx->ongoing_event)
		{
			if(ICF_NULL != p_cms_resp)
			{
			 	stream_id = p_cms_resp->stream_status[stream_count].stream_id;
				stream_type = p_cms_resp->stream_status[stream_count].stream_type;
				dummy_ret_val = ICF_SUCCESS;
			}
		}
		else
		if(ICF_MMI_MMS_MOD_ONGOING == p_mmi_pdb->p_mmi_ctx->ongoing_event)
		{
			if(ICF_NULL != p_mms_resp)
			{
			stream_id = 
				p_mms_resp->modified_stream_resp[stream_count].stream_id;
			stream_type = 
				p_mms_resp->modified_stream_resp[stream_count].stream_type;
			dummy_ret_val = 
				p_mms_resp->modified_stream_resp[stream_count].status_success;
			}
		}
		else
		if(ICF_MMI_MMS_ADD_ONGOING == p_mmi_pdb->p_mmi_ctx->ongoing_event)
		{
			if(ICF_NULL != p_mms_resp)
			{
			stream_id = 
				p_mms_resp->added_stream_resp[stream_count].stream_id;
			stream_type = 
				p_mms_resp->added_stream_resp[stream_count].stream_type;
			dummy_ret_val = 
				p_mms_resp->added_stream_resp[stream_count].status_success;
			}
		}

		if((ICF_STREAM_TYPE_AUDIO == stream_type)&&
			(ICF_SUCCESS == dummy_ret_val))
		{
			sdp.p_codec_info_list = ICF_NULL;

			dummy_ret_val = icf_mmi_get_sdp_for_stream_id(
											p_mmi_pdb,
											stream_id,
											ICF_SDP_REMOTE,
											&sdp);
			if(ICF_SUCCESS == dummy_ret_val)
			{
				ret_val = icf_mmi_fetch_common_mode_set_for_amr_params(
											p_mmi_pdb,
											stream_id,
											sdp.p_codec_info_list);

				if(ICF_FAILURE == ret_val)
				{
					if(ICF_MMI_CMS_ONGOING == 
									p_mmi_pdb->p_mmi_ctx->ongoing_event)
					{
						icf_mmi_mark_failure_stream_cms_resp(p_mmi_pdb,stream_id);
					}
					else
					if(ICF_MMI_MMS_MOD_ONGOING == 
									p_mmi_pdb->p_mmi_ctx->ongoing_event)
					{
						icf_mmi_mark_failure_stream_mms_mod_resp(p_mmi_pdb,stream_id);
					}
					else
					if(ICF_MMI_MMS_ADD_ONGOING == 
									p_mmi_pdb->p_mmi_ctx->ongoing_event)
					{
						icf_mmi_mark_failure_stream_mms_add_resp(p_mmi_pdb,stream_id);
					}
				}
			}
		}
	}

	ICF_FUNCTION_EXIT(p_mmi_pdb->p_glb_pdb)

	return ret_val;
}


/******************************************************************************
 *
 * FUNCTION:    icf_mmi_fetch_common_mode_set_for_amr_params
 *
 * DESCRIPTION: This function will find the common mode-set value from the
 *              received mode-set and the app-conf mode set for amr params.
 *
 *****************************************************************************/

icf_return_t icf_mmi_fetch_common_mode_set_for_amr_params
(
	INOUT   icf_mmi_pdb_st		*p_mmi_pdb,
	IN		icf_stream_id_t		stream_id,
	IN		icf_list_st			*p_codec_list
)
{
    icf_return_t                    ret_val = ICF_SUCCESS,
									dummy_ret_val = ICF_SUCCESS; 
    icf_app_conf_st                 *p_app_conf = ICF_NULL;
    icf_internal_list_st            *p_app_profile_node=ICF_NULL;
	icf_list_st						*p_app_codec_list = ICF_NULL;

    ICF_FUNCTION_ENTER(p_mmi_pdb->p_glb_pdb)
    
    dummy_ret_val = icf_cfg_fetch_app_conf(p_mmi_pdb->p_glb_pdb,
									p_mmi_pdb->p_call_ctx->app_id,
									&p_app_conf,
									p_mmi_pdb->p_ecode);
    
    if(ICF_SUCCESS == dummy_ret_val)
    {
        if(ICF_NULL != p_app_conf->p_app_media_profile)
        {
			/* Control will be here if app_add profile exists,
			 * Now search for Audio profile in it.
             */
			icf_uint32_t	loc_stream = 0;
			loc_stream= stream_id;
			p_app_profile_node = icf_cmn_get_node_in_internal_list(
									p_app_conf->p_app_media_profile,
									&loc_stream);
        }
        
        if(ICF_NULL != p_app_profile_node)
        {
			while(p_codec_list)
			{
				p_app_codec_list = 
					((icf_stream_capab_st*)p_app_profile_node->p_payload)->\
						p_codec_info_list;

				while(p_app_codec_list)
				{
					if(ICF_SUCCESS == icf_mmi_compare_codecs_value(p_mmi_pdb,
													p_codec_list->p_data,
													p_app_codec_list->p_data))
					{
                        /* negotiation to be done only for dynamic codecs */
                        if(96 <= ((icf_codec_attrib_st*)p_codec_list->p_data)->codec_num)
                        {
						    ret_val = ICF_FAILURE;
						
						    dummy_ret_val = icf_mmi_compare_amr_params_per_codec(
													p_mmi_pdb,
													p_codec_list->p_data,
													p_app_codec_list->p_data);

						if(ICF_SUCCESS == dummy_ret_val)
						{
							ret_val = ICF_SUCCESS;
							break;
						}
						}
					}
					p_app_codec_list = p_app_codec_list->p_next;
				}
				p_codec_list = p_codec_list->p_next;
			}
        }
    }    
    else
    {
        ICF_PRINT(((icf_uint8_t*)"\n Failed to get the application configuration for %d app_id",\
            p_mmi_pdb->p_call_ctx->app_id));
    }

    ICF_FUNCTION_EXIT(p_mmi_pdb->p_glb_pdb)
    return ret_val;
}


/******************************************************************************
 *
 * FUNCTION:    icf_mmi_compare_amr_params_per_codec
 *
 * DESCRIPTION: 
 *****************************************************************************/

icf_return_t	icf_mmi_compare_amr_params_per_codec
(
	icf_mmi_pdb_st		*p_mmi_pdb, 
	icf_codec_attrib_st	*p_codec1,
	icf_codec_attrib_st	*p_codec2
)
{
    icf_return_t		    ret_val = ICF_SUCCESS;
    icf_uint8_t                     app_mode_index = 0;
    icf_uint8_t                     recv_mode_index = 0; 
    icf_amr_params_st               *p_app_amr_params=ICF_NULL,
									*p_amr_params = ICF_NULL;
    icf_boolean_t                   match_found = ICF_FALSE;

	if(ICF_NULL == p_mmi_pdb)
    {
       return ICF_FAILURE;
    }	
    ICF_FUNCTION_ENTER(p_mmi_pdb->p_glb_pdb)

	p_mmi_pdb = p_mmi_pdb;

	if(p_codec1->bit_mask & ICF_CODEC_ATTRIB_CODEC_PARAMS_PRESENT)
	{
		p_amr_params = 
			&(p_codec1->codec_params.audio_param.amr_params);

		if(p_codec2->bit_mask & ICF_CODEC_ATTRIB_CODEC_PARAMS_PRESENT)
		{
			p_app_amr_params = 
				&(p_codec2->codec_params.audio_param.amr_params);

			if(0 != p_app_amr_params->amr_mode_count)
			{
				if(0 != p_amr_params->amr_mode_count)
				{
					ret_val = ICF_FAILURE;

					/* Fetch the common mode-set value for the AMR mode-set
					 */
					for(app_mode_index = 0; (app_mode_index < p_app_amr_params->\
						amr_mode_count) && (app_mode_index < ICF_MAX_AMR_MODE); app_mode_index++)
					{
						for(recv_mode_index = 0; (recv_mode_index < p_amr_params->\
							amr_mode_count) && (recv_mode_index < ICF_MAX_AMR_MODE); recv_mode_index++)
						{
							if(p_amr_params->amr_mode_list[recv_mode_index] == \
								p_app_amr_params->amr_mode_list[app_mode_index])
							{
							   /* Intersection mode-set value found
								*/
								p_amr_params->amr_initial_mode = p_amr_params->\
									amr_mode_list[recv_mode_index];

								match_found = ICF_TRUE;
								break;
							}
						}
						if(ICF_TRUE == match_found)
						{
							ICF_PRINT(((icf_uint8_t*)"\n Common mode-set value found : %d",\
								p_amr_params->amr_initial_mode));
							
							ret_val = ICF_SUCCESS;
							break;
						}
					}
				}/*End of if for amr_mode_count.*/
				else
				{
					/* If no amr mode is recieved in remote SDP then set initial mode
					 * as recieved in config */
					p_amr_params->amr_initial_mode = 
						p_app_amr_params->amr_initial_mode;
				}
			}
			/* process only if success returned from mode check */
			if(ICF_SUCCESS == ret_val)
			{
				ret_val = ICF_FAILURE;
				
				if(p_amr_params->codec_mode)
				{
					/* if codec_mode is present in remote sdp then it must be same
					 * as that set in the app_conf */
					if(p_amr_params->codec_mode == p_app_amr_params->codec_mode)
					{
						ret_val = ICF_SUCCESS;
					}
				}
				/* if codec_mode is not present in remote sdp then it assumes a 
				 * default value of ICF_BANDWIDTH_EFF_MODE so ret_val = success 
				 * if ICF_BANDWIDTH_EFF_MODE was set in the app_conf */
				else
				if((ICF_BANDWIDTH_EFF_MODE == p_app_amr_params->codec_mode)||
					!(p_app_amr_params->codec_mode))
				{
					ret_val = ICF_SUCCESS;
					p_amr_params->codec_mode = ICF_BANDWIDTH_EFF_MODE;
				}
			}

			/* process only if success returned from both above checks */
			if(ICF_SUCCESS == ret_val)
			{
				if(p_amr_params->crc_enabled != p_app_amr_params->crc_enabled)
				{
					ret_val = ICF_FAILURE;
				}
			}
		}
	}

	ICF_FUNCTION_EXIT(p_mmi_pdb->p_glb_pdb)

	return ret_val;
}

/******************************************************************************
 *
 * FUNCTION:    icf_mmi_validate_transport_addr
 *
 * DESCRIPTION: 
 *****************************************************************************/

icf_return_t	icf_mmi_validate_transport_addr
(
	icf_mmi_pdb_st				*p_mmi_pdb, 
	icf_transport_address_st	*p_addr
)
{
	icf_return_t				ret_val = ICF_FAILURE;

    if(ICF_NULL == p_mmi_pdb)
    {
       return ICF_FAILURE;
    }	
    ICF_FUNCTION_ENTER(p_mmi_pdb->p_glb_pdb)

	p_mmi_pdb = p_mmi_pdb;

	switch(p_addr->addr.addr_type)
	{
		case ICF_TRANSPORT_ADDRESS_IPV4_ADDR:

			ret_val = ICF_SUCCESS;
			break;
#ifdef ICF_IPV6_ENABLED
		case ICF_TRANSPORT_ADDRESS_IPV6_ADDR:

			ret_val = ICF_SUCCESS;
			break;
#endif
	}

	if(ICF_NULL == p_addr->port_num)
	{
		ret_val = ICF_FAILURE;
	}

	ICF_FUNCTION_EXIT(p_mmi_pdb->p_glb_pdb)

	return ret_val;
}


#ifdef ICF_NAT_MEDIA_TRAVERSAL

/******************************************************************************
 *
 * FUNCTION:    icf_mmi_send_get_public_ip_port_req
 *
 * DESCRIPTION: This function will send the get public ip port request
 *              message to NAT Client Library.
 *
 * RETURN_TYPES:ICF_SUCCESS/ICF_FAILURE
 *
 *****************************************************************************/

icf_return_t	icf_mmi_send_get_public_ip_port_req
(
	icf_mmi_pdb_st		*p_mmi_pdb, 
	icf_stream_id_t		stream_id,
    icf_uint8_t         req_type
)
{
	icf_return_t						ret_val = ICF_SUCCESS;
	nat_get_public_ip_port_req_st		*p_mmi_msg_payload = ICF_NULL;
    icf_msg_st                          *p_mmi_msg = ICF_NULL;
    icf_uint8_t                         api_len=ICF_NULL;

    ICF_FUNCTION_ENTER(p_mmi_pdb->p_glb_pdb)
        
    api_len = ICF_PORT_SIZEOF(icf_api_header_st) + 
        ICF_PORT_SIZEOF(nat_get_public_ip_port_req_st);
    
    ICF_MEMGET(p_mmi_pdb->p_glb_pdb,api_len,ICF_MEM_COMMON,p_mmi_msg,
            ICF_RET_ON_FAILURE,p_mmi_pdb->p_ecode,ret_val)
        
    /* Fill the header of the message to be send */
    p_mmi_msg->hdr.api_id = NAT_GET_PUBLIC_IP_PORT_REQ;
    p_mmi_msg->hdr.source_id = ICF_MODULE_ICF;
    p_mmi_msg->hdr.destination_id = ICF_MODULE_NAT;
    p_mmi_msg->hdr.api_length = api_len;
    p_mmi_msg->hdr.call_id = p_mmi_pdb->p_call_ctx->call_id;
	p_mmi_msg->hdr.version = ICF_VERSION_1_0;

	/* Doubt: is this correct ? */
	p_mmi_msg->hdr.app_id = 
		icf_mmi_make_nat_context_id(p_mmi_pdb->p_call_ctx->call_id, stream_id,\
                req_type);
    
    /* Fill the payload */
    p_mmi_msg_payload = (nat_get_public_ip_port_req_st *)
        (p_mmi_msg->payload);

	/* fill the payload */
	p_mmi_msg_payload->nat_protocol = NAT_PROTOCOL_STUN;

	if(ICF_MEDIA_REQ_TYPE_STREAM == req_type)
    {
        /* Put the RTP address in the local address
         */
        icf_mmi_convert_to_nat_addr(&((p_mmi_pdb->p_mmi_ctx->nat_streams\
                [p_mmi_pdb->p_mmi_ctx->nat_stream_count - 1]).local_addr),\
                &(p_mmi_msg_payload->local_address));
    }
    else if(ICF_MEDIA_REQ_TYPE_RTCP == req_type)
    {
        /* Put the RTP address in the local address
         */
        icf_mmi_convert_to_nat_addr(&((p_mmi_pdb->p_mmi_ctx->nat_streams\
                        [p_mmi_pdb->p_mmi_ctx->nat_stream_count - 1]).local_addr),\
                &(p_mmi_msg_payload->local_address));

        p_mmi_msg_payload->local_address.port = (p_mmi_msg_payload->\
                local_address.port)+1;
    }

	p_mmi_msg_payload->is_refresh_enabled = ICF_FALSE;

    ret_val = icf_mmi_send(p_mmi_pdb,p_mmi_msg,p_mmi_pdb->p_ecode);

    if(ICF_SUCCESS == ret_val)
    {
        /* Free memory */
        ICF_MEMFREE(p_mmi_pdb->p_glb_pdb,p_mmi_msg,ICF_MEM_COMMON,
                p_mmi_pdb->p_ecode,ret_val)
    }          
    else
    {
        ICF_MEMFREE(p_mmi_pdb->p_glb_pdb,p_mmi_msg,ICF_MEM_COMMON,
            p_mmi_pdb->p_ecode,ret_val)
        ret_val = ICF_FAILURE;
    }    

	ICF_FUNCTION_EXIT(p_mmi_pdb->p_glb_pdb)

	return ret_val;
}


/******************************************************************************
 *
 * FUNCTION:    icf_mmi_rel_nat_resources
 *
 * DESCRIPTION: 
 *****************************************************************************/

icf_return_t	icf_mmi_rel_nat_resources
(
	icf_mmi_pdb_st		*p_mmi_pdb
)
{
	icf_return_t					ret_val = ICF_SUCCESS;
	icf_mmi_internal_cms_resp_st	*p_cms_resp = ICF_NULL;
	icf_mmi_internal_mms_resp_st	*p_mms_resp = ICF_NULL;
    icf_stream_id_t                 stream_count = ICF_NULL;
    icf_stream_id_t                 stream_id = ICF_NULL;

	/* If CMS fails then the PUBLIC_IP_PORT needs to be closed for the streams
	 * that were success till now */
	if(p_mmi_pdb->bitmask & ICF_MMI_PDB_CMS_FAILURE)
	{
		if(ICF_MMI_CMS_ONGOING == p_mmi_pdb->p_mmi_ctx->ongoing_event)
		{
			p_cms_resp = p_mmi_pdb->p_mmi_ctx->p_mmi_resp;

			while(stream_count < p_cms_resp->count)
			{
				if(ICF_FALSE == 
					p_cms_resp->stream_status[stream_count].status_success)
				{
					stream_id = 
						p_cms_resp->stream_status[stream_count].stream_id;
					icf_mmi_send_rel_public_ip_port(p_mmi_pdb, stream_id);
				}
				stream_count++;
			}
		}
		else if (ICF_MMI_MMS_ADD_ONGOING == p_mmi_pdb->p_mmi_ctx->ongoing_event)
		{
			p_mms_resp = p_mmi_pdb->p_mmi_ctx->p_mmi_resp;

			stream_id = p_mms_resp->added_stream_resp[ICF_NULL].stream_id;

			icf_mmi_send_rel_public_ip_port(p_mmi_pdb, stream_id);
		}
	}
	else
	{
        stream_id = ICF_NULL;
        icf_mmi_send_rel_public_ip_port(p_mmi_pdb, stream_id);
    }
	return ret_val;
}


/******************************************************************************
 *
 * FUNCTION:    icf_mmi_send_rel_public_ip_port
 *
 * DESCRIPTION:  This function sends the release public ip - port request
 *               to STUN client.
 *
 *****************************************************************************/
icf_return_t	icf_mmi_send_rel_public_ip_port
(
	icf_mmi_pdb_st			*p_mmi_pdb,
	icf_stream_id_t			stream_id
)
{
	icf_return_t		            ret_val = ICF_SUCCESS;
    icf_stream_id_t                 count = ICF_NULL;
    icf_uint32_t                    context_id = ICF_NULL;
    icf_msg_st                      *p_msg_data = ICF_NULL;
    icf_uint8_t                     api_len = ICF_NULL;

    ICF_FUNCTION_ENTER(p_mmi_pdb->p_glb_pdb)
    if (stream_id ==  ICF_NULL)
    {
        for(;count < p_mmi_pdb->p_mmi_ctx->no_of_nat_ctxt;count++)
        {
            /* Form and send the request to NAT client
             */
         
            api_len = ICF_PORT_SIZEOF(icf_api_header_st);
    
            ICF_MEMGET(p_mmi_pdb->p_glb_pdb,api_len,ICF_MEM_COMMON,p_msg_data,
                    ICF_RET_ON_FAILURE,p_mmi_pdb->p_ecode,ret_val)

            /* Fill the header of the message to be send */
            p_msg_data->hdr.api_id = NAT_REL_PUBLIC_IP_PORT_REQ;
            p_msg_data->hdr.source_id = ICF_MODULE_ICF;
            p_msg_data->hdr.destination_id = ICF_MODULE_NAT;
            p_msg_data->hdr.api_length = api_len;
            p_msg_data->hdr.call_id = p_mmi_pdb->p_call_ctx->call_id;
            p_msg_data->hdr.version = ICF_VERSION_1_0;

            context_id = p_mmi_pdb->p_mmi_ctx->context_info[count].context_id;
            p_msg_data->hdr.app_id = context_id; 
            ret_val = icf_mmi_send(p_mmi_pdb,p_msg_data,p_mmi_pdb->p_ecode);

            if(ICF_SUCCESS == ret_val)
            {
                /* Free memory */
                ICF_MEMFREE(p_mmi_pdb->p_glb_pdb,p_msg_data,ICF_MEM_COMMON,
                        p_mmi_pdb->p_ecode,ret_val)
            }          
            else
            {
                ICF_MEMFREE(p_mmi_pdb->p_glb_pdb,p_msg_data,ICF_MEM_COMMON,
                        p_mmi_pdb->p_ecode,ret_val)
                ret_val = ICF_FAILURE;
                break;
            }    
        }
		p_mmi_pdb->p_mmi_ctx->no_of_nat_ctxt = p_mmi_pdb->p_mmi_ctx->\
	                                             no_of_nat_ctxt - count;
        return ret_val;
        /* send request for all streams */
	}
    else
    {
        /* Send the release ip-port request for the stream id specified.
         */
        for(count = 0;count < p_mmi_pdb->p_mmi_ctx->no_of_nat_ctxt;count++)
        {
            if(stream_id == p_mmi_pdb->p_mmi_ctx->context_info[count].stream_id)
            {
                api_len = ICF_PORT_SIZEOF(icf_api_header_st);

                ICF_MEMGET(p_mmi_pdb->p_glb_pdb,api_len,ICF_MEM_COMMON,p_msg_data,
                        ICF_RET_ON_FAILURE,p_mmi_pdb->p_ecode,ret_val)

                /* Fill the header of the message to be send */
                p_msg_data->hdr.api_id = NAT_REL_PUBLIC_IP_PORT_REQ;
                p_msg_data->hdr.source_id = ICF_MODULE_ICF;
                p_msg_data->hdr.destination_id = ICF_MODULE_NAT;
                p_msg_data->hdr.api_length = api_len;
                p_msg_data->hdr.call_id = p_mmi_pdb->p_call_ctx->call_id;
                p_msg_data->hdr.version = ICF_VERSION_1_0;

                context_id = p_mmi_pdb->p_mmi_ctx->context_info[count].context_id;
                p_msg_data->hdr.app_id = context_id; 
                /* Fill the payload */
                ret_val = icf_mmi_send(p_mmi_pdb,p_msg_data,p_mmi_pdb->p_ecode);

                if(ICF_SUCCESS == ret_val)
                {
                    /* Free memory */
                    ICF_MEMFREE(p_mmi_pdb->p_glb_pdb,p_msg_data,ICF_MEM_COMMON,
                            p_mmi_pdb->p_ecode,ret_val)
                }          
                else
                {
                    ICF_MEMFREE(p_mmi_pdb->p_glb_pdb,p_msg_data,ICF_MEM_COMMON,
                            p_mmi_pdb->p_ecode,ret_val)
                        ret_val = ICF_FAILURE;
                    break;
                }
            }
        }
        return ret_val;
    }
}

/******************************************************************************
 *
 * FUNCTION:    icf_mmi_send_recv_udp_packet_ind
 *
 * DESCRIPTION: This function will send the recv udp packet indication
 *              message to NAT Client Library.
 *
 * RETURN_TYPES:ICF_SUCCESS/ICF_FAILURE
 *
 *****************************************************************************/

icf_return_t	icf_mmi_send_recv_udp_packet_ind
(
	icf_mmi_pdb_st		*p_mmi_pdb
)
{
	icf_return_t				ret_val = ICF_SUCCESS;
	nat_recv_udp_pkt_ind_st		*p_mmi_msg_payload = ICF_NULL;
    icf_msg_st                  *p_mmi_msg = ICF_NULL;
    icf_uint16_t                  api_len=ICF_NULL;
    icf_recv_udp_data_ind_st    *p_mm_recv_msg = 
        (icf_recv_udp_data_ind_st *)p_mmi_pdb->p_glb_pdb->p_recv_msg->payload;
    icf_config_data_st      *p_config_data = ICF_NULL;

    api_len = ICF_PORT_SIZEOF(icf_api_header_st) + 
        ICF_PORT_SIZEOF(nat_recv_udp_pkt_ind_st);
    
    ICF_MEMGET(p_mmi_pdb->p_glb_pdb,api_len,ICF_MEM_COMMON,p_mmi_msg,
            ICF_RET_ON_FAILURE,p_mmi_pdb->p_ecode,ret_val)
        
    /* Fill the header of the message to be send */
    p_mmi_msg->hdr.api_id = NAT_RECV_UDP_PKT_IND;
    p_mmi_msg->hdr.source_id = ICF_MODULE_ICF;
    p_mmi_msg->hdr.destination_id = ICF_MODULE_NAT;
    p_mmi_msg->hdr.api_length = api_len;
    p_mmi_msg->hdr.call_id = p_mmi_pdb->p_glb_pdb->p_recv_msg->hdr.call_id;
	p_mmi_msg->hdr.version = ICF_VERSION_1_0;

	p_mmi_msg->hdr.app_id = ICF_INVALID_APP_ID;
    
    /* Fill the payload */
    p_mmi_msg_payload = (nat_recv_udp_pkt_ind_st *)(p_mmi_msg->payload);

    p_mmi_msg_payload->local_addr.port = p_mm_recv_msg->dest_port;

    ret_val = icf_dbm_get_module_glb_data(
            p_mmi_pdb->p_glb_pdb,
            (icf_int_module_id_t)ICF_INT_MODULE_CFG,
            (icf_void_t *)&p_config_data, p_mmi_pdb->p_ecode);
        
    ICF_ASSERT(p_cc_pdb->p_glb_pdb, (ICF_NULL != p_config_data));

    icf_port_memcpy(&p_mmi_msg_payload->local_addr.ipv4_addr,
                    &p_config_data->self_ip_address.addr.addr.ipv4_addr,
                    ICF_PORT_SIZEOF(nat_ipv4_address_st));

    p_mmi_msg_payload->octets.octet_len = p_mm_recv_msg->app_data_len;
    
    icf_port_memcpy(p_mmi_msg_payload->octets.str,
                p_mm_recv_msg->p_app_data,p_mm_recv_msg->app_data_len);


    ret_val = icf_mmi_send(p_mmi_pdb,p_mmi_msg,p_mmi_pdb->p_ecode);

    if(ICF_SUCCESS == ret_val)
    {
        /* Free memory */
        
        ICF_MEMFREE(p_mmi_pdb->p_glb_pdb,p_mmi_msg,ICF_MEM_COMMON,
                p_mmi_pdb->p_ecode,ret_val)
    }          
    else
    {
        ICF_MEMFREE(p_mmi_pdb->p_glb_pdb,p_mmi_msg,ICF_MEM_COMMON,
            p_mmi_pdb->p_ecode,ret_val)
        ret_val = ICF_FAILURE;
    }    

	ICF_FUNCTION_EXIT(p_mmi_pdb->p_glb_pdb)

	return ret_val;
}


/******************************************************************************
 *
 * FUNCTION:    icf_mmi_make_nat_context_id
 *
 * DESCRIPTION: This function will make the context id from strream id ,\
 *              stream type and call id.
 *              First 2 bytes will represent call_id,
 *              Third Byte will represent the stream id.
 *              Last byte represent the stream_type.
 *
 *****************************************************************************/

icf_uint32_t	icf_mmi_make_nat_context_id
( 
	icf_call_id_t		call_id,
	icf_stream_id_t		stream_id,
    icf_media_type_t    stream_type
)
{
    icf_uint32_t		context_id = ICF_NULL;    
    icf_uint16_t            temp_stream_info = ICF_NULL;
    
    context_id = call_id << 16;
    temp_stream_info = stream_id << 8;
    temp_stream_info += stream_type;
    context_id |= temp_stream_info;
	return context_id;
}


/******************************************************************************
 *
 * FUNCTION:    icf_mmi_convert_to_nat_addr
 *
 * DESCRIPTION: Function to convert the ICF transport address to 
 *              NAT Address
 **************************************************************************/

icf_return_t icf_mmi_convert_to_nat_addr
(
 icf_transport_address_st       *p_local_addr,
 nat_ip_port_address_st         *p_nat_addr
)
{
    icf_return_t        ret_val = ICF_SUCCESS;
    
   p_nat_addr->ipv4_addr.octet_1 = p_local_addr->addr.addr.\
                                   ipv4_addr.octet_1;
   p_nat_addr->ipv4_addr.octet_2 = p_local_addr->addr.addr.\
                                   ipv4_addr.octet_2;
   p_nat_addr->ipv4_addr.octet_3 = p_local_addr->addr.addr.\
                                   ipv4_addr.octet_3;
   p_nat_addr->ipv4_addr.octet_4 = p_local_addr->addr.addr.\
                                   ipv4_addr.octet_4;
   p_nat_addr->port = p_local_addr->port_num; 
   return ret_val;

}


/******************************************************************************
 *
 * FUNCTION:     icf_mmi_convert_nat_addr_to_trans_addr
 * 
 * DESCRIPTION: Function to convert the NAT address to 
 *              ICF Transport Address
 **************************************************************************/

icf_return_t icf_mmi_convert_nat_addr_to_trans_addr
(
 nat_ip_port_address_st         *p_nat_addr,
icf_transport_address_st        *p_local_addr
)
{
    icf_return_t        ret_val = ICF_SUCCESS;
    
   
   p_local_addr->addr.addr.ipv4_addr.octet_1 = p_nat_addr->\
                                          ipv4_addr.octet_1;
   p_local_addr->addr.addr.ipv4_addr.octet_2 = p_nat_addr->\
                                          ipv4_addr.octet_2;
   p_local_addr->addr.addr.ipv4_addr.octet_3 = p_nat_addr->\
                                          ipv4_addr.octet_3;
   p_local_addr->addr.addr.ipv4_addr.octet_4 = p_nat_addr->\
                                          ipv4_addr.octet_4;
   p_local_addr->addr.addr_type = ICF_TRANSPORT_ADDRESS_IPV4_ADDR;

   p_local_addr->port_num = p_nat_addr->port; 

   return ret_val;

}

#endif /* #ifdef ICF_NAT_MEDIA_TRAVERSAL */
/* ICF REL 6.0: New Functions end */




/************************************************************************
 * NAME:          icf_mmi_send 
 * DESCRIPTION:   Function is used to send message to PA/MM
 * 
 ***********************************************************************/

icf_return_t icf_mmi_send(
        IN   icf_mmi_pdb_st *p_mmi_pdb,
        INOUT    icf_msg_st *p_msg_data,
        OUT icf_error_t *p_ecode)
{
    icf_return_t  ret_val = ICF_SUCCESS;
#ifdef ICF_MM_REQUEST_BUFFERING
    if(ICF_MODULE_MM == p_msg_data->hdr.destination_id)
    {
        icf_mmi_data_st *p_mmi_data=ICF_NULL;
        
        ret_val = icf_dbm_get_module_glb_data(p_mmi_pdb->p_glb_pdb,
            (icf_module_id_t)(ICF_INT_MODULE_MMI),
            (icf_void_t *)&p_mmi_data,
            p_ecode);

        if(ICF_SUCCESS == ret_val)
        {
            if(ICF_TRUE == p_mmi_data->mm_resp_awaited)
            {
                ret_val = icf_mmi_buffer_msg(p_mmi_pdb,
                    p_msg_data,
                    p_ecode);
            }
            else 
            {
                ICF_SEND(p_mmi_pdb->p_glb_pdb,p_msg_data,ret_val,p_ecode);
				icf_mmi_free_mm_req(p_mmi_pdb, p_msg_data);

                if(ICF_TRUE == icf_mmi_check_mm_req_msg(p_msg_data))
                {
                    p_mmi_data->mm_resp_awaited = ICF_TRUE;
                }
            }
        }
    }
    else
#endif
	{
		ICF_SEND(p_mmi_pdb->p_glb_pdb,p_msg_data,ret_val,p_ecode)
		icf_mmi_free_mm_req(p_mmi_pdb, p_msg_data);
	}

    return ret_val;
}

#ifdef ICF_MM_REQUEST_BUFFERING
/************************************************************************
 * NAME:          icf_mmi_buffer_msg
 * DESCRIPTION:   Function is used to buffer outgoing MM message
 * 
 ***********************************************************************/

icf_return_t icf_mmi_buffer_msg(IN   icf_mmi_pdb_st  *p_mmi_pdb,
                                IN icf_msg_st *p_msg_data,
                                OUT icf_error_t *p_ecode)
{
    icf_mmi_data_st *p_mmi_data=ICF_NULL;
    icf_mm_msg_node_st **p_p_cur_node;
    icf_return_t  ret_val;
    
    ret_val = icf_dbm_get_module_glb_data(p_mmi_pdb->p_glb_pdb,
        (icf_module_id_t)(ICF_INT_MODULE_MMI),
        (icf_void_t *)&p_mmi_data,
        p_ecode);
    
    if(ICF_SUCCESS == ret_val)
    {
        /* Move to the last node */
        for(p_p_cur_node = &(p_mmi_data->p_mm_msg_buf);
        *p_p_cur_node!=NULL;
        p_p_cur_node = &((*p_p_cur_node)->p_next));

        /* Allocate memory for buffer node */
        ICF_MEMGET(
			p_mmi_pdb->p_glb_pdb,
			ICF_PORT_SIZEOF(icf_mm_msg_node_st),
			ICF_MEM_COMMON,
			(*p_p_cur_node),
            ICF_RET_ON_FAILURE,
			p_ecode,
			ret_val);

        (*p_p_cur_node)->p_next = ICF_NULL;
        (*p_p_cur_node)->p_msg  = ICF_NULL;

        /* Allocate memory for message */
        ICF_MEMGET(p_mmi_pdb->p_glb_pdb,
			p_msg_data->hdr.api_length,
			ICF_MEM_COMMON,
			(*p_p_cur_node)->p_msg,
            ICF_DONOT_RET_ON_FAILURE,
			p_ecode,
			ret_val);

        if(ICF_FAILURE == ret_val)
        {
            ICF_MEMFREE(
				p_mmi_pdb->p_glb_pdb,
				(*p_p_cur_node),
				ICF_MEM_COMMON,
                p_ecode,
				ret_val);

            (*p_p_cur_node) = ICF_NULL;
            return ICF_FAILURE;
        }
        /* Buffer Mesg */
        icf_port_memcpy(
            (icf_void_t *)(*p_p_cur_node)->p_msg,
            (icf_void_t *)p_msg_data,
            p_msg_data->hdr.api_length);
    }
    return ret_val;
}

/************************************************************************
 * NAME:          icf_mmi_pop_buffered_msg
 * DESCRIPTION:   Function is used remove a message from the buffered messages  
 * 
 ***********************************************************************/

icf_void_t icf_mmi_pop_buffered_msg(
                                IN  icf_glb_pdb_st   *p_glb_pdb,
                                INOUT   icf_mmi_data_st  *p_mmi_data,
                                OUT icf_msg_st **p_p_msg_data,
                                OUT icf_error_t *p_ecode)
{
    icf_return_t ret_val = ICF_SUCCESS;

    if(ICF_NULL == p_mmi_data->p_mm_msg_buf)
    {
        *(p_p_msg_data) = ICF_NULL;
    }
    else
    {
        icf_error_t ecode;
        icf_mm_msg_node_st *p_node = p_mmi_data->p_mm_msg_buf;
        *(p_p_msg_data) = p_node->p_msg;
        p_mmi_data->p_mm_msg_buf = p_node->p_next;

        ICF_MEMFREE(p_glb_pdb,p_node,ICF_MEM_COMMON,
            &ecode,ret_val);
    }
}

/************************************************************************
 * NAME:          icf_mmi_check_mm_req_msg
 * DESCRIPTION:   Function checks is the given message is a request message
 *                for MM.
 * 
 ***********************************************************************/
icf_boolean_t icf_mmi_check_mm_req_msg(icf_msg_st *p_msg_data)
{
    if(ICF_MODULE_MM == p_msg_data->hdr.destination_id)
    {
        switch(p_msg_data->hdr.api_id)
        {
        case ICF_CFG_CODEC_INFO_REQ:
        case ICF_CFG_RINGER_TONES_REQ:
        case ICF_CFG_MEDIA_REQ:
        case ICF_CFG_JBM_MGMT_REQ:
        case ICF_CFG_MAX_EARP_VOL_REQ:
        case ICF_CFG_MAX_SPEAKER_VOL_REQ:
        case ICF_CFG_SET_PORT_RANGE_REQ:
        case ICF_CFG_SET_SELF_IP_REQ:
        case ICF_CFG_SET_DIGIT_TRANS_REQ:
		case ICF_CFG_SET_REMOTE_VIDEO_DISPLAY_REQ:
		case ICF_CFG_SET_PREVIEW_VIDEO_DISPLAY_REQ:
        case ICF_START_RINGING_REQ:
        case ICF_VIBRATION_REQ:
        case ICF_MEDIA_SESSION_REQ:
        case ICF_ECHO_CANC_REQ:
        case ICF_VAD_SS_CNG_REQ:
        case ICF_GET_STATS_REQ:
        case ICF_PLAY_MEDIA_REQ:
		case ICF_STOP_MEDIA_PLAY_REQ:
        case ICF_MUTE_MEDIA_REQ:
        case ICF_SET_MEDIA_PLAYBACK_DEVICE_REQ:
        case ICF_STOP_RINGING_REQ:
        case ICF_STOP_VIBRATION_REQ:
        case ICF_CONNECT_MEDIA_DEVICE_REQ:
		case ICF_MEDIA_RECORD_REQ:
        case ICF_SEND_DGTS_REQ:
        case ICF_SET_EARP_VOLUME_REQ:
        case ICF_SET_SPEAKER_VOLUME_REQ:
        case ICF_STOP_MEDIA_RECORD_REQ:
        case ICF_SEND_APP_DATA_REQ:
        case ICF_SEND_STATS_REQ:
        case ICF_STOP_STATS_REQ:
        case ICF_CFG_MAX_MIC_GAIN_REQ:
        case ICF_SET_MIC_GAIN_REQ:
		case ICF_PAUSE_MEDIA_REQ:
        case ICF_CREATE_MEDIA_SESSION_REQ:
        case ICF_MODIFY_MEDIA_SESSION_REQ:
        case ICF_DELETE_MEDIA_SESSION_REQ:
        case ICF_MERGE_MEDIA_SESSION_REQ:
        case ICF_GET_CODEC_LIST_REQ:
        case ICF_CODEC_COMMIT_REQ:
        case ICF_MEDIA_CAPABILITIES_REQ: 
		case ICF_OPEN_MEDIA_CHANNEL_REQ:
		case ICF_CLOSE_MEDIA_CHANNEL_REQ:
		case ICF_SEND_UDP_DATA_REQ:
		case ICF_CFG_SET_SELF_IP_REQ:
				return ICF_TRUE;
        
        default : 
				return ICF_FALSE;
        }
    }
    return ICF_FALSE;
}

/************************************************************************
 * NAME:          icf_mmi_check_mm_resp_msg
 * DESCRIPTION:   Function checks is the given message is a response message
 *                from MM.
 * 
 ***********************************************************************/
icf_boolean_t icf_mmi_check_mm_resp_msg(icf_msg_st *p_msg_data)
{
    if(ICF_MODULE_MM == p_msg_data->hdr.source_id)
    {
        switch(p_msg_data->hdr.api_id)
        {
        case ICF_CFG_CODEC_INFO_RESP:
        case ICF_CFG_RINGER_TONES_RESP:
        case ICF_CFG_MEDIA_RESP:	
        case ICF_CFG_JBM_MGMT_RESP:
        case ICF_CFG_MAX_EARP_VOL_RESP:
        case ICF_CFG_MAX_SPEAKER_VOL_RESP:
        case ICF_CFG_SET_PORT_RANGE_RESP:	
        case ICF_CFG_SET_SELF_IP_RESP:
        case ICF_CFG_SET_DIGIT_TRANS_RESP:
		case ICF_CFG_SET_REMOTE_VIDEO_DISPLAY_RESP:
		case ICF_CFG_SET_PREVIEW_VIDEO_DISPLAY_RESP:
        case ICF_START_RINGING_RESP:
        case ICF_VIBRATION_RESP:	
        case ICF_MEDIA_SESSION_RESP:
        case ICF_ECHO_CANC_RESP:	
        case ICF_VAD_SS_CNG_RESP:
        case ICF_GET_STATS_RESP:	
        case ICF_PLAY_MEDIA_RESP:	
        case ICF_STOP_MEDIA_PLAY_RESP:	
        case ICF_MUTE_MEDIA_RESP:	
        case ICF_SET_MEDIA_PLAYBACK_DEVICE_RESP:
		case ICF_STOP_RINGING_RESP:
        case ICF_STOP_VIBRATION_RESP:
		case ICF_STOP_TONE_IND:
		case ICF_STOP_RINGING_IND:
		case ICF_DGTS_IND:
		case ICF_CONNECT_MEDIA_DEVICE_RESP:
		case ICF_MEDIA_RECORD_RESP:
        case ICF_SEND_DGTS_RESP:	
        case ICF_SET_EARP_VOLUME_RESP:
        case ICF_SET_SPEAKER_VOLUME_RESP:
        case ICF_STOP_MEDIA_RECORD_RESP:
        case ICF_SEND_APP_DATA_RESP:
		case ICF_RCV_APP_DATA_IND:
        case ICF_SEND_STATS_RESP:	
        case ICF_STOP_STATS_RESP:
        case ICF_CFG_MAX_MIC_GAIN_RESP:
        case ICF_SET_MIC_GAIN_RESP:
		case ICF_RTP_START_IND:
		case ICF_PAUSE_MEDIA_RESP:
		case ICF_MM_INIT_COMPLETE:
        case ICF_CREATE_MEDIA_SESSION_RESP:
        case ICF_MODIFY_MEDIA_SESSION_RESP:	
        case ICF_DELETE_MEDIA_SESSION_RESP:	
        case ICF_MERGE_MEDIA_SESSION_RESP:	
        case ICF_GET_CODEC_LIST_RESP:
        case ICF_CODEC_COMMIT_RESP:	
        case ICF_MEDIA_CAPABILITIES_RESP: 
		case ICF_OPEN_MEDIA_CHANNEL_RESP:
		case ICF_RECV_UDP_DATA_IND:
		case ICF_DELETE_MEDIA_SESSION_IND:

				return ICF_TRUE;
        
        default: 
				return ICF_FALSE;
        }
    }
    return ICF_FALSE;
}

#endif /* #ifdef BUFFERING */

/************************************************************************
 * NAME:          icf_mmi_populate_codec_in_applied_sdp
 * DESCRIPTION:   This function copies codecs from codec_for_commit
 * 
 ***********************************************************************/
icf_return_t  icf_mmi_populate_codec_in_applied_sdp(
            icf_mmi_pdb_st      *p_mmi_pdb,
            icf_stream_id_t     stream_id,
            icf_list_st         **p_p_codec_info_list)
{
    icf_return_t            ret_val = ICF_SUCCESS,
                            dummy_ret_val = ICF_SUCCESS;
    icf_codec_attrib_st     *p_codec_attrib = ICF_NULL;
    icf_stream_id_t         cnt = 0;
    icf_uint8_t             codec_cnt = 0;

    ICF_FUNCTION_ENTER(p_mmi_pdb->p_glb_pdb)

	if(*p_p_codec_info_list)
	{
		icf_cmn_delete_list(p_mmi_pdb->p_glb_pdb,p_p_codec_info_list,
			ICF_MEM_COMMON);
	}

    for (cnt = 0; (cnt < p_mmi_pdb->p_mmi_ctx->stream_count) && (cnt < ICF_MAX_STREAM_PER_CALL) ; cnt++)
    {
        if (stream_id == p_mmi_pdb->p_mmi_ctx->codec_for_commit[cnt].stream_id)
        {
            for (codec_cnt = 0; 
                    (codec_cnt < p_mmi_pdb->p_mmi_ctx->codec_for_commit[cnt].codec_count) && (codec_cnt < ICF_MAX_CODECS);
                    codec_cnt++)
            {
                ICF_MEMGET (p_mmi_pdb->p_glb_pdb,
					ICF_PORT_SIZEOF(icf_codec_attrib_st),
					ICF_MEM_COMMON,
					p_codec_attrib,
					ICF_RET_ON_FAILURE,
					p_mmi_pdb->p_ecode,
					ret_val)

                icf_port_memcpy(p_codec_attrib, 
                    &(p_mmi_pdb->p_mmi_ctx->codec_for_commit[cnt].codec_info[codec_cnt]),
                    sizeof(icf_codec_attrib_st));

                if (ICF_CODEC_ENC_ATTRIB_PRESENT & 
                    p_mmi_pdb->p_mmi_ctx->codec_for_commit[cnt].
                    codec_info[codec_cnt].bit_mask)
			    {
				    /* clone the codec attributes */
				    /* in the current release this is not going to be used, so this
			 	    * block is empty- no concept of encoded attribute lines within
			        * the codec */
			    }

                ret_val = icf_cmn_add_node_to_list(p_mmi_pdb->p_glb_pdb, 
                                                    p_p_codec_info_list,
                                                    p_codec_attrib);
                if (ICF_FAILURE == ret_val)
                {
                    ICF_MEMFREE(p_mmi_pdb->p_glb_pdb,
                            p_codec_attrib,
                            ICF_MEM_COMMON,
                            p_mmi_pdb->p_ecode,
                            dummy_ret_val)
                    return ret_val;
                }
                ICF_KLOCWORK_FALSE_POSITIVE(p_mmi_pdb->p_glb_pdb,p_codec_attrib)
            }
        }
    }

    ICF_FUNCTION_EXIT(p_mmi_pdb->p_glb_pdb)
    return ret_val;
}

/************************************************************************
 * NAME:          icf_mmi_validate_commited_no_of_codecs
 * DESCRIPTION:   This function validates that the no of codecs present in
 *				  the list sent in is same as that of commited codecs stored 
 *				  in mmi_ctx
 * 
 ***********************************************************************/
icf_boolean_t  icf_mmi_validate_commited_no_of_codecs
(
	icf_mmi_pdb_st      *p_mmi_pdb,
	icf_stream_id_t     stream_id,
    icf_list_st         *p_codec_info_list
)
{
    icf_boolean_t       codecs_match = ICF_TRUE;
	icf_stream_id_t		stream_count = ICF_NULL;
	icf_uint8_t			codec_count = ICF_NULL;

	icf_cmn_get_list_count(p_codec_info_list,&codec_count);

	for(stream_count = 0; (stream_count < p_mmi_pdb->p_mmi_ctx->stream_count) && (stream_count < ICF_MAX_STREAM_PER_CALL);
		stream_count++)
	{
		if(stream_id == 
			p_mmi_pdb->p_mmi_ctx->codec_for_commit[stream_count].stream_id)
		{
            /* SPR 19187 : Special handling for fec/red */
            /* This function is triggered to ignore codec mismatch if the 
			 * difference is there due to presence of DTMF/FEC/RED/CN payloads.
			 */
            icf_mmi_check_special_codec(
				p_mmi_pdb,
				p_codec_info_list,
				&p_mmi_pdb->p_mmi_ctx->codec_for_commit[stream_count],
				&codec_count);
			
			if (codec_count != 
			    p_mmi_pdb->p_mmi_ctx->codec_for_commit[stream_count].codec_count)
			{
    		    codecs_match = ICF_FALSE;
			}
			break;
		}
	}
    return codecs_match;
}

/************************************************************************
 * NAME:          icf_mmi_unmark_send_mms_in_mms_resp
 * DESCRIPTION:   
 * 
 ***********************************************************************/
icf_return_t  icf_mmi_unmark_send_mms_in_mms_resp
(
	icf_mmi_pdb_st      *p_mmi_pdb,
	icf_stream_id_t     stream_id
)
{
	icf_return_t					ret_val = ICF_SUCCESS;
	icf_mmi_internal_mms_resp_st	*p_mms_resp = ICF_NULL;
	icf_stream_id_t					stream_count = ICF_NULL;

	p_mms_resp = p_mmi_pdb->p_mmi_ctx->p_mmi_resp;

	for(stream_count = 0; (stream_count < p_mms_resp->mod_stream_count) && (stream_count < ICF_MAX_STREAM_PER_CALL);
		stream_count++)
	{
		if(stream_id == 
			p_mms_resp->modified_stream_resp[stream_count].stream_id)
		{
			p_mms_resp->modified_stream_resp[stream_count].send_mms
				= ICF_FALSE;
			break;
		}
	}
    return ret_val;
}


#ifdef ICF_QOS_SUPPORT
/************************************************************************
 * NAME:          icf_mmi_check_if_qos_stream
 * DESCRIPTION:
 * 
 ***********************************************************************/
icf_return_t  icf_mmi_check_if_qos_stream
(
            icf_mmi_pdb_st      *p_mmi_pdb,
            icf_stream_id_t     stream_id
)
{
    icf_return_t            ret_val = ICF_FAILURE;
	icf_stream_id_t			stream_count = ICF_NULL;

    ICF_FUNCTION_ENTER(p_mmi_pdb->p_glb_pdb)

	for(stream_count = 0; (stream_count < p_mmi_pdb->p_mmi_ctx->pdp_count) && (stream_count < ICF_MAX_STREAM_PER_CALL);
		stream_count++)
	{
		if(stream_id == p_mmi_pdb->p_mmi_ctx->pdp_stream_id[stream_count])
		{
			ret_val = ICF_SUCCESS;
			break;
		}
	}

    ICF_FUNCTION_EXIT(p_mmi_pdb->p_glb_pdb)
    return ret_val;
}

/************************************************************************
 * NAME:          icf_mmi_get_bandwidth_for_flow_id
 * DESCRIPTION:
 * 
 ***********************************************************************/
icf_return_t  icf_mmi_get_bandwidth_for_flow_id
(
            icf_mmi_pdb_st			*p_mmi_pdb,
            icf_stream_flow_map_st	*p_stream_flow_map,
			icf_uint8_t				flow_idx,
			icf_uint16_t			*p_bandwidth
)
{
    icf_return_t            ret_val = ICF_FAILURE;
	icf_stream_id_t			stream_count = ICF_NULL,
							mapped_stream_id = ICF_NULL;
	icf_uint32_t			loc_stream = ICF_NULL;
	icf_internal_list_st	*p_stream_node = ICF_NULL;
	icf_stream_sdp_info_st	*p_data = ICF_NULL;

    ICF_FUNCTION_ENTER(p_mmi_pdb->p_glb_pdb)

	for(stream_count = 0; stream_count < p_mmi_pdb->p_mmi_ctx->pdp_count;
		stream_count++)
	{
		if((flow_idx >= p_stream_flow_map[stream_count].from_flow_idx)&&
			(flow_idx <= p_stream_flow_map[stream_count].to_flow_idx))
		{
			mapped_stream_id = p_stream_flow_map[stream_count].stream_id;
			break;
		}
	}

	loc_stream = mapped_stream_id;

	p_stream_node = icf_cmn_get_node_in_internal_list(p_mmi_pdb->p_call_ctx->\
						p_stream_sdp_info, &loc_stream);

	if(p_stream_node)
	{
		p_data = p_stream_node->p_payload;
		if(p_data)
		{
			if(ICF_STREAM_TYPE_AUDIO == 
				icf_mmi_get_stream_type_for_stream_id(p_mmi_pdb, mapped_stream_id))
			{
				if(p_data->p_local_sdp->sdp_stream.media_stream_param.\
					audio_profile.bandwidth)
				{
					*p_bandwidth = p_data->p_local_sdp->sdp_stream.media_stream_param.\
									audio_profile.bandwidth;

					ret_val = ICF_SUCCESS;
				}
			}
			else
			{
				if(p_data->p_local_sdp->sdp_stream.media_stream_param.\
					video_profile.bandwidth)
				{
					*p_bandwidth = p_data->p_local_sdp->sdp_stream.media_stream_param.\
									video_profile.bandwidth;

					ret_val = ICF_SUCCESS;
				}			
			}
		}
	
	}

    ICF_FUNCTION_EXIT(p_mmi_pdb->p_glb_pdb)
    return ret_val;
}

/************************************************************************
 * NAME:          icf_mmi_qos_check_av_stream
 * DESCRIPTION:
 * 
 ***********************************************************************/
icf_return_t  icf_mmi_qos_check_av_stream
(
            icf_mmi_pdb_st			*p_mmi_pdb
)
{
    icf_return_t					ret_val = ICF_FAILURE;
	icf_stream_id_t					stream_count = ICF_NULL;
	icf_mmi_internal_cms_resp_st	*p_cms_resp = ICF_NULL;
	icf_mmi_internal_mms_resp_st	*p_mms_resp = ICF_NULL;

    ICF_FUNCTION_ENTER(p_mmi_pdb->p_glb_pdb)

	p_mmi_pdb->p_mmi_ctx->pdp_count = ICF_NULL;

	if(ICF_MMI_CMS_ONGOING == p_mmi_pdb->p_mmi_ctx->ongoing_event)
	{
		p_cms_resp = p_mmi_pdb->p_mmi_ctx->p_mmi_resp;

		for(stream_count = 0; (stream_count < p_cms_resp->count) && (stream_count < ICF_MAX_STREAM_PER_CALL); stream_count++)
		{
			if((ICF_TRUE == p_cms_resp->stream_status[stream_count].status_success)&&
				((ICF_STREAM_TYPE_AUDIO == p_cms_resp->stream_status[stream_count].stream_type)||
				(ICF_STREAM_TYPE_VIDEO == p_cms_resp->stream_status[stream_count].stream_type)))
			{
				p_mmi_pdb->p_mmi_ctx->pdp_stream_id[p_mmi_pdb->p_mmi_ctx->pdp_count]=
					p_cms_resp->stream_status[stream_count].stream_id;

				p_mmi_pdb->p_mmi_ctx->pdp_count++;
			}
		}
	}
	else if(ICF_MMI_MMS_MOD_ONGOING == p_mmi_pdb->p_mmi_ctx->ongoing_event)
	{
		p_mms_resp = p_mmi_pdb->p_mmi_ctx->p_mmi_resp;

		for(stream_count = 0; (stream_count < p_mms_resp->mod_stream_count) && (stream_count < ICF_MAX_STREAM_PER_CALL); stream_count++)
		{
			if((ICF_TRUE == p_mms_resp->modified_stream_resp[stream_count].status_success)&&
				((ICF_STREAM_TYPE_AUDIO == p_mms_resp->modified_stream_resp[stream_count].stream_type)||
				(ICF_STREAM_TYPE_VIDEO == p_mms_resp->modified_stream_resp[stream_count].stream_type)))
			{
				p_mmi_pdb->p_mmi_ctx->pdp_stream_id[p_mmi_pdb->p_mmi_ctx->pdp_count]=
					p_mms_resp->modified_stream_resp[stream_count].stream_id;

				p_mmi_pdb->p_mmi_ctx->pdp_count++;
			}
		}		
	}

	if(p_mmi_pdb->p_mmi_ctx->pdp_count)
	{
		ret_val = ICF_SUCCESS;
	}
    ICF_FUNCTION_EXIT(p_mmi_pdb->p_glb_pdb)
    return ret_val;
}

#endif

/******************************************************************************
 *
 * FUNCTION:        icf_send_self_ip_req_to_mm
 * 
 *
 *****************************************************************************/
icf_return_t	icf_send_self_ip_req_to_mm(
	INOUT	icf_mmi_pdb_st	*p_mmi_pdb
)
{
	/* send API to configure app port. app port may be muxd with RTCP port.
	 * In which case, RTCP/APP Port will be rtp port + 1.
	 */
	/* local variables */
	icf_return_t				ret_val = ICF_SUCCESS;
	icf_msg_st					*p_msg=NULL;
	icf_api_header_st   		*p_api_hdr = ICF_NULL;
	icf_config_data_st				*p_config_data= ICF_NULL;
	icf_cfg_set_self_ip_req_st	*p_cfg_set_self_ip_req = ICF_NULL;

	ICF_PRINT(((icf_uint8_t*)"[MMI]:Entered icf_send_self_ip_req_to_mm\n"));
	
	/* fetch the MMI glb data */
	ret_val = icf_dbm_get_module_glb_data(p_mmi_pdb->p_glb_pdb,
        (icf_module_id_t)(ICF_INT_MODULE_CFG),
        (icf_void_t *)&p_config_data,
        p_mmi_pdb->p_ecode);
    

	ICF_MEMGET(p_mmi_pdb->p_glb_pdb,
		ICF_PORT_SIZEOF(icf_api_header_st) + 
		ICF_PORT_SIZEOF(icf_cfg_set_self_ip_req_st),
		ICF_MEM_COMMON,
		p_msg,
        ICF_RET_ON_FAILURE,
		p_mmi_pdb->p_ecode,
		ret_val)

	if (ICF_NULL != p_msg)
	{
		p_api_hdr = &(p_msg->hdr);
		p_api_hdr->api_id = ICF_CFG_SET_SELF_IP_REQ;
		p_api_hdr->api_length =
			ICF_PORT_SIZEOF(icf_api_header_st) + 
			ICF_PORT_SIZEOF(icf_cfg_set_self_ip_req_st);
		p_api_hdr->app_id = ICF_DUMMY_APP_ID;
		p_api_hdr->call_id = ICF_INVALID_CALL_ID;
		p_api_hdr->destination_id = ICF_MODULE_MM;
		p_api_hdr->source_id = ICF_MODULE_ICF;
		p_api_hdr->version = ICF_VERSION_1_0;

		p_cfg_set_self_ip_req  = (icf_cfg_set_self_ip_req_st*)
								(p_msg->payload);
		p_cfg_set_self_ip_req->self_ip_addr.addr_type = 
			p_config_data->self_ip_address.addr.addr_type;
		icf_port_strcpy(p_cfg_set_self_ip_req->self_ip_addr.addr_val.str,(icf_uint8_t *)"0.0.0.0");
		if(ICF_TRANSPORT_ADDRESS_IPV4_ADDR == p_config_data->self_ip_address.addr.addr_type)
		{
			icf_port_sprintf((icf_int8_t *)p_cfg_set_self_ip_req->self_ip_addr.addr_val.str,"%d.%d.%d.%d",
				p_config_data->self_ip_address.addr.addr.ipv4_addr.octet_1,
				p_config_data->self_ip_address.addr.addr.ipv4_addr.octet_2,
				p_config_data->self_ip_address.addr.addr.ipv4_addr.octet_3,
				p_config_data->self_ip_address.addr.addr.ipv4_addr.octet_4);
		}
		else if(ICF_TRANSPORT_ADDRESS_IPV6_ADDR == p_config_data->self_ip_address.addr.addr_type)
		{
			icf_port_strcpy(p_cfg_set_self_ip_req->self_ip_addr.addr_val.str,
				p_config_data->self_ip_address.addr.addr.ipv6_addr.str);
		}
		else if (ICF_TRANSPORT_ADDRESS_DOMAIN_NAME == p_config_data->self_ip_address.addr.addr_type)
		{
			icf_port_strcpy(p_cfg_set_self_ip_req->self_ip_addr.addr_val.str,
				p_config_data->self_ip_address.addr.addr.domain.str);
		}
		p_cfg_set_self_ip_req->self_ip_addr.addr_val.str_len = 
			(icf_uint16_t)icf_port_strlen(p_cfg_set_self_ip_req->self_ip_addr.addr_val.str);
		
		ret_val = icf_mmi_send(p_mmi_pdb,p_msg,p_mmi_pdb->p_ecode);

        /* Free memory */
        ICF_MEMFREE(p_mmi_pdb->p_glb_pdb,
			p_msg,
			ICF_MEM_COMMON,
            p_mmi_pdb->p_ecode,
			ret_val)
	}
	else
	{
		ICF_PRINT(((icf_uint8_t*)"[MMI]:MemAlloc failure\n"));
		ret_val = ICF_FAILURE;
	}

	ICF_PRINT(((icf_uint8_t*)"[MMI]: icf_send_self_ip_req_to_mm\n"));
	return ret_val;
}

/******************************************************************************
 * FUNCTION:       icf_mmi_handle_local_dtmf_capability
 *
 * DESCRIPTION:    This function can be triggered while making GCL/COMMIT/MMS
 *                 payload. This function will be triggered only when the payload
 *                 being made uses the remote SDP. In such a scenario the request
 *                 payload being sent should contain the local DTMF information.
 *                 If remote DTMF information is already present in SDP then we 
 *                 replace it by local DTMF info. If remote DTMF info is not 
 *                 present then we add the local information onto the payload.
 *
 *****************************************************************************/
icf_return_t    icf_mmi_handle_local_dtmf_capability
(
    INOUT   icf_mmi_pdb_st      *p_mmi_pdb,
    INOUT   icf_sdp_stream_st   *p_rem_sdp
)
{
    icf_return_t         ret_val = ICF_SUCCESS;
#if 0
    icf_sdp_stream_st    loc_sdp = {0};
    icf_list_st          *p_loc_codec_list = ICF_NULL,
                         *p_rem_codec_list = ICF_NULL;
    icf_codec_attrib_st  *p_node = ICF_NULL;

    if(ICF_SUCCESS == icf_mmi_get_sdp_for_stream_id(p_mmi_pdb,p_mmi_pdb->stream_id,
                          ICF_SDP_LOCAL,&loc_sdp))
    {
        p_loc_codec_list = loc_sdp.p_codec_info_list;

        while(p_loc_codec_list)
        {
            if(ICF_NULL == icf_port_strcmp(
              ((icf_codec_attrib_st*)p_loc_codec_list->p_data)->codec_name.str,
              "telephone-event"))
            {
                p_rem_codec_list = p_rem_sdp->p_codec_info_list;

                while(p_rem_codec_list)
                {
                    if(ICF_NULL == icf_port_strcmp(
                      ((icf_codec_attrib_st*)p_rem_codec_list->p_data)->codec_name.str,
                      "telephone-event"))
                    {
                        if(((icf_codec_attrib_st*)p_rem_codec_list->p_data)->p_encoded_attributes)
                        {
                            ICF_MEMFREE(p_mmi_pdb->p_glb_pdb,
                                    ((icf_codec_attrib_st*)p_rem_codec_list->p_data)->p_encoded_attributes,
                                    ICF_MEM_COMMON,
                                    p_mmi_pdb->p_ecode,ret_val)
                        }

                        icf_cmn_delete_node_frm_list(p_mmi_pdb->p_glb_pdb,
                            &p_rem_sdp->p_codec_info_list, p_rem_codec_list->p_data);

                        break;
                    }

                    p_rem_codec_list = p_rem_codec_list->p_next;
                }

                /* MEMGET for p_node as type icf_stream_capab_st */
                ICF_MEMGET (p_mmi_pdb->p_glb_pdb,
                            ICF_PORT_SIZEOF(icf_codec_attrib_st),
                            ICF_MEM_COMMON,
                            p_node,
                            ICF_RET_ON_FAILURE,
                            p_mmi_pdb->p_ecode,
                            ret_val)
                
                icf_port_memcpy(p_node,
                    p_loc_codec_list->p_data,
                    (icf_uint32_t)ICF_PORT_SIZEOF(icf_codec_attrib_st));

                icf_cmn_add_node_to_list(p_mmi_pdb->p_glb_pdb,
                    &p_rem_sdp->p_codec_info_list, p_node);

                break;
            }

            p_loc_codec_list = p_loc_codec_list->p_next;
        }
    }
#endif 
    p_mmi_pdb = p_mmi_pdb;
    p_rem_sdp = p_rem_sdp;
    return ret_val;
}

/******************************************************************************
 *
 * FUNCTION:        icf_mmi_send_unexpected_msg_ind
 *
 * DESCRIPTION:     This function is invoked when ever any function needs to
 *                  send the UNEXPECTED EVENT IND to the MM.
 *
 * RETURNS:         ICF_SUCCESS in case of success and ICF_FAILURE in
 *                  case of any failure.
 *
 ******************************************************************************/

icf_return_t icf_mmi_send_unexpected_msg_ind(
        INOUT icf_mmi_pdb_st             *p_mmi_pdb,
        IN    icf_error_t                *p_ecode)
{
    icf_return_t                        ret_val = ICF_FAILURE,
                                        dummy_ret_val = ICF_FAILURE;
    icf_msg_st                          *p_mmi_msg = ICF_NULL;
    icf_int16_t                         api_len = 0;
    icf_mm_unexpected_msg_ind_st        *p_mm_unexpected_msg_ind = ICF_NULL;


    ICF_FUNCTION_ENTER(p_mmi_pdb->p_glb_pdb)

        api_len = ICF_PORT_SIZEOF(icf_api_header_st) +
                        ICF_PORT_SIZEOF(icf_mm_unexpected_msg_ind_st);

    ICF_MEMGET(p_mmi_pdb->p_glb_pdb,api_len,ICF_MEM_COMMON,p_mmi_msg,
            ICF_RET_ON_FAILURE,p_mmi_pdb->p_ecode, dummy_ret_val)

        if(ICF_FAILURE == dummy_ret_val)
        {
                ICF_ALARM(((icf_uint8_t *)"[MMI]: MEMGET failure"));
                return dummy_ret_val;
        }
     /* Fill the header of the message to be send */
    p_mmi_msg->hdr.api_id = ICF_MM_UNEXPECTED_MSG_IND;
    p_mmi_msg->hdr.source_id = ICF_MODULE_ICF;
    p_mmi_msg->hdr.destination_id = ICF_MODULE_MM;
    p_mmi_msg->hdr.api_length = api_len;
    p_mmi_msg->hdr.call_id = p_mmi_pdb->p_glb_pdb->p_recv_msg->hdr.call_id;
    p_mmi_msg->hdr.version = ICF_VERSION_1_0;
    p_mmi_msg->hdr.app_id = p_mmi_pdb->p_glb_pdb->p_recv_msg->hdr.app_id;

    p_mm_unexpected_msg_ind = (icf_mm_unexpected_msg_ind_st *)
        (p_mmi_msg->payload); 
    if(ICF_ERROR_NONE != *p_ecode)
      {
        p_mm_unexpected_msg_ind->error_cause = *p_ecode;
      }
    else
      { 
       p_mm_unexpected_msg_ind->error_cause = ICF_MM_CAUSE_INVALID_ACTION;
      }
    p_mm_unexpected_msg_ind->api_id = p_mmi_pdb->p_glb_pdb->p_recv_msg->hdr.api_id;

    ret_val = icf_mmi_send(p_mmi_pdb,p_mmi_msg,p_mmi_pdb->p_ecode);

    ICF_MEMFREE(p_mmi_pdb->p_glb_pdb,p_mmi_msg,ICF_MEM_COMMON,
        p_mmi_pdb->p_ecode,dummy_ret_val)

    ICF_FUNCTION_EXIT(p_mmi_pdb->p_glb_pdb)
    return ret_val;
}/* End function */

/******************************************************************************
 *
 * FUNCTION:        icf_mmi_init_sdp
 *
 * DESCRIPTION:     This function is used to initialize the variable of type 
 *                  icf_sdp_stream_st.
 *
 ******************************************************************************/
 icf_void_t icf_mmi_init_sdp(INOUT icf_sdp_stream_st *p_sdp)
   {

       p_sdp->bit_mask = 0;
       p_sdp->p_codec_info_list = ICF_NULL;
       p_sdp->stream_address.addr.addr_type = 0;
       p_sdp->stream_address.port_num = 0;
       p_sdp->rtcp_address.addr.addr_type = 0;
       p_sdp->rtcp_address.port_num = 0;
       p_sdp->p_encoded_attributes = ICF_NULL;

    }

/******************************************************************************
 * FUNCTION:       icf_mmi_check_dtmf_codec
 *
 * DESCRIPTION:    This function is triggered when validating commited number
 *                 of codecs upon recving MMS response. This will return success
 *                 if either commited codecs or codecs in MMS response contains
 *                 DTMF codec.
 *****************************************************************************/
icf_void_t    icf_mmi_check_dtmf_codec
(
    IN      icf_mmi_pdb_st          *p_mmi_pdb,
    IN      icf_list_st             *p_codec_list,
    IN      icf_codec_for_commit_st *p_codec_for_commit,
    INOUT   icf_uint8_t             *p_codec_count
)
{
    icf_uint8_t     codec_count = ICF_NULL;

    p_mmi_pdb = p_mmi_pdb;

    while(p_codec_list)
    {
        if(ICF_NULL == icf_port_strcmp((icf_uint8_t *) "telephone-event",
                       ((icf_codec_attrib_st*)p_codec_list->p_data)->codec_name.str))
        {
            (*p_codec_count)--;
        }
        p_codec_list = p_codec_list->p_next;
    }

    for(codec_count = ICF_NULL; (codec_count < p_codec_for_commit->codec_count) && (codec_count < ICF_MAX_CODECS);
            codec_count++)
    {
        if(ICF_NULL == icf_port_strcmp((icf_uint8_t *) "telephone-event",
                    p_codec_for_commit->codec_info[codec_count].codec_name.str))
        {
            (*p_codec_count)++;
        }
    }
}

/*****************************************************************************
 ** FUNCTION:     icf_mmi_free_icf_req_stream_node
 **
 ** DESCRIPTION:  This function frees the parameters of icf_req_stream_st
 **
 *****************************************************************************/
icf_return_t	icf_mmi_free_icf_req_stream_node
(
	INOUT	icf_mmi_pdb_st      *p_mmi_pdb,
	INOUT	icf_req_stream_st	**p_p_req_stream_node,
    INOUT	icf_error_t    	    *p_ecode
)
{
    icf_return_t        ret_val = ICF_SUCCESS;
	icf_req_stream_st	*p_temp_req_stream = ICF_NULL;
    icf_sdp_stream_st   *p_temp_req_stream_local_sdp = ICF_NULL;
    icf_sdp_stream_st   *p_temp_req_stream_remote_sdp = ICF_NULL;

    ICF_FUNCTION_ENTER(p_mmi_pdb->p_glb_pdb)
    
    p_temp_req_stream = *p_p_req_stream_node;
    p_temp_req_stream_local_sdp = &(p_temp_req_stream->local_sdp);
    p_temp_req_stream_remote_sdp = &(p_temp_req_stream->remote_sdp);

    if (ICF_NULL != p_temp_req_stream_local_sdp)
    {
        if ( ICF_NULL != p_temp_req_stream_local_sdp->p_codec_info_list)
        {
            icf_cmn_delete_codec_attrib_list(p_mmi_pdb->p_glb_pdb,
                    &(p_temp_req_stream_local_sdp->p_codec_info_list),
                                               p_mmi_pdb->p_ecode);

        }

        if ( ICF_NULL != p_temp_req_stream_local_sdp->p_encoded_attributes)
        {
            icf_cmn_delete_list(p_mmi_pdb->p_glb_pdb,
                    &(p_temp_req_stream_local_sdp->p_encoded_attributes),
                    ICF_MEM_COMMON);
        }
    }
    if (ICF_NULL != p_temp_req_stream_remote_sdp)
    {
        if ( ICF_NULL != p_temp_req_stream_remote_sdp->p_codec_info_list)
        {
            icf_cmn_delete_codec_attrib_list(p_mmi_pdb->p_glb_pdb,
                    &(p_temp_req_stream_remote_sdp->p_codec_info_list),
                    p_mmi_pdb->p_ecode);


        }

        if ( ICF_NULL != p_temp_req_stream_remote_sdp->p_encoded_attributes)
        {
            icf_cmn_delete_list(p_mmi_pdb->p_glb_pdb,
                    &(p_temp_req_stream_remote_sdp->p_encoded_attributes),
                    ICF_MEM_COMMON);
        }
    }
    ICF_MEMFREE(p_mmi_pdb->p_glb_pdb, *p_p_req_stream_node, ICF_MEM_COMMON, 
            p_ecode, ret_val)

    ICF_FUNCTION_EXIT(p_mmi_pdb->p_glb_pdb)
    return ret_val;

}

/*****************************************************************************
 ** FUNCTION:     icf_mmi_free_icf_codec_attrib_node
 **
 ** DESCRIPTION:  This function frees the parameters of icf_codec_attrib_st
 **
 *****************************************************************************/
icf_return_t icf_mmi_free_icf_codec_attrib_node
(
	INOUT	icf_glb_pdb_st	    *p_glb_pdb,
	INOUT	icf_codec_attrib_st	**p_p_codec_attrib,
    INOUT	icf_error_t    	    *p_ecode
)
{
	icf_codec_attrib_st		*p_codec_attrib= *p_p_codec_attrib;
    icf_return_t 	        ret_val=ICF_SUCCESS;

    if (p_codec_attrib != ICF_NULL)
    {
        /*This function is releasing the memory assigned to 
          p_encoded_attributes list inside the node of codec attrib.*/
        if (ICF_CODEC_ENC_ATTRIB_PRESENT & 
                p_codec_attrib->bit_mask)
        {
            if (ICF_NULL != p_codec_attrib->p_encoded_attributes)
            {
                ret_val = icf_cmn_delete_list(p_glb_pdb,
                        (&(p_codec_attrib->p_encoded_attributes)),
                        ICF_MEM_COMMON);
            }
        }
        /*Releasing the memory assigned to the node.*/
        ICF_MEMFREE	(
                p_glb_pdb, 
                (p_codec_attrib), 
                ICF_MEM_COMMON, 
                p_ecode, 
                ret_val);
    }/*End of if (p_codec_attrib != NULL) */

    *p_p_codec_attrib=ICF_NULL;
    return ret_val;
}

/******************************************************************************
 * FUNCTION:       icf_mmi_check_special_codec
 *
 * DESCRIPTION:    This function is triggered when validating commited number
 *                 of codecs upon recving MMS response. This will return success
 *                 if either commited codecs or codecs in MMS response contains
 *                 DTMF/FEC/RED/CN payload types.
 *****************************************************************************/
icf_void_t    icf_mmi_check_special_codec
(
    IN      icf_mmi_pdb_st          *p_mmi_pdb,
    IN      icf_list_st             *p_codec_list,
    IN      icf_codec_for_commit_st *p_codec_for_commit,
    INOUT   icf_uint8_t             *p_codec_count
)
{
    icf_uint8_t     codec_count = ICF_NULL;

    p_mmi_pdb = p_mmi_pdb;
    /* CSR 1-7732501: SPR 20073: Special handling for CN */
    while (p_codec_list)
    {
        /* SPR 19218 : Added NULL check for p_codec_list->p_data */
        if(ICF_NULL != p_codec_list->p_data)
        {
            if ((ICF_NULL == icf_port_strcmp(
				    (icf_uint8_t *) "telephone-event",
				    ((icf_codec_attrib_st*)p_codec_list->p_data)->codec_name.str))||
			    (ICF_NULL == icf_port_strcmp(
                    (icf_uint8_t *) "parityfec",
                    ((icf_codec_attrib_st*)p_codec_list->p_data)->codec_name.str))||
			    (ICF_NULL == icf_port_strcmp(
                    (icf_uint8_t *) "red",
                    ((icf_codec_attrib_st*)p_codec_list->p_data)->codec_name.str))||
    			(ICF_NULL == icf_port_strcmp(
                   (icf_uint8_t *) "CN",
                    ((icf_codec_attrib_st*)p_codec_list->p_data)->codec_name.str)))
            {
                (*p_codec_count)--;
            }
        }
        p_codec_list = p_codec_list->p_next;
    }

    for(codec_count = ICF_NULL; (codec_count < p_codec_for_commit->codec_count) && (codec_count < ICF_MAX_CODECS);
            codec_count++)
    {
        if ((ICF_NULL == icf_port_strcmp(
				(icf_uint8_t *) "telephone-event",
				p_codec_for_commit->codec_info[codec_count].codec_name.str)) ||
			(ICF_NULL == icf_port_strcmp(
                (icf_uint8_t *) "parityfec",
				p_codec_for_commit->codec_info[codec_count].codec_name.str)) ||
			(ICF_NULL == icf_port_strcmp(
                (icf_uint8_t *) "red",
				p_codec_for_commit->codec_info[codec_count].codec_name.str)) ||
			(ICF_NULL == icf_port_strcmp(
                (icf_uint8_t *) "CN",
                p_codec_for_commit->codec_info[codec_count].codec_name.str)))
        {
            (*p_codec_count)++;
        }
    }

	return;
} /* End function icf_mmi_check_special_codec() */


/****************************************************************************** *
 *
 * FUNCTION:        icf_mmi_send_sic_tunnelled_med_cap_resp
 *
 * DESCRIPTION:     This function is invoked whenever tunnelled media capability 
 *                  response is to be sent to SIC.
 *                  THis function copies tunnelled_sdp in 
 *                  internal message and send it to SIC when Tunnel mode is ON
 *
 ******************************************************************************/

icf_return_t icf_mmi_send_sic_tunnelled_med_cap_resp(
        INOUT icf_mmi_pdb_st *p_mmi_pdb)
{

    icf_return_t                    ret_val = ICF_SUCCESS,
                                    dummy_ret_val = ICF_SUCCESS;
    icf_get_media_cap_resp_st       get_med_cap_resp = {0,0,0,0};
    /*  Rel 8.1 changes */
    icf_tunnelled_media_capabilities_resp_st  *p_tunnel_med_cap_resp = ICF_NULL;
    icf_list_st                 *p_src_list = ICF_NULL;
    icf_inc_media_sdp_info_st   *p_dest_data = ICF_NULL;
    icf_boolean_t               memget_fail = ICF_FALSE;


    ICF_FUNCTION_ENTER(p_mmi_pdb->p_glb_pdb)

    p_mmi_pdb->p_internal_msg->msg_hdr.msg_id = ICF_GET_MEDIA_CAP_RESP;
    p_mmi_pdb->p_internal_msg->msg_hdr.payload_length = 
                              ICF_PORT_SIZEOF(icf_get_media_cap_resp_st);

    /* Rel 8.1 
     * If tunnel mode is ON then recev mesg must be 
     * icf_tunnelled_sdp_info_st 
     */
    p_tunnel_med_cap_resp = (icf_tunnelled_media_capabilities_resp_st *)
                               p_mmi_pdb->p_glb_pdb->p_recv_msg->payload;
    if(ICF_NULL == p_tunnel_med_cap_resp)
    {
        ret_val = ICF_FAILURE;
        ICF_FUNCTION_EXIT(p_mmi_pdb->p_glb_pdb)
        return ret_val;
    }
    get_med_cap_resp.result = p_tunnel_med_cap_resp->result;
    
    get_med_cap_resp.context_identifier = 
                  p_mmi_pdb->p_glb_pdb->p_recv_msg->hdr.call_id;

    get_med_cap_resp.p_sdp_list = ICF_NULL;

    if(ICF_SUCCESS == p_tunnel_med_cap_resp->result)
    {
        /* Rel 8.1
         * if SDP is present in response then make a node of 
         * icf_inc_media_sdp_info_st fill the received tunnel 
         * SDP in icf_internal_sdp_stream_st that is present in 
         * icf_inc_media_sdp_info_st and send that internal 
         * message to SIC
         */
        if(ICF_TUNNELLED_MED_CAP_RESP_SDP_INFO_PRESENT ==
          (ICF_TUNNELLED_MED_CAP_RESP_SDP_INFO_PRESENT & p_tunnel_med_cap_resp->bit_mask))
        {
            ICF_MEMGET(
                p_mmi_pdb->p_glb_pdb,
                ICF_PORT_SIZEOF(icf_inc_media_sdp_info_st),
                ICF_MEM_COMMON,
                p_dest_data,
                ICF_DONOT_RET_ON_FAILURE,
                p_mmi_pdb->p_ecode,
                dummy_ret_val)	

            if(ICF_SUCCESS == dummy_ret_val)
            {
                ICF_MEMGET(
                        p_mmi_pdb->p_glb_pdb,
                        ICF_PORT_SIZEOF(icf_internal_sdp_stream_st),
                        ICF_MEM_COMMON,
                        p_dest_data->p_int_sdp_stream,
                        ICF_DONOT_RET_ON_FAILURE,
                        p_mmi_pdb->p_ecode,
                        dummy_ret_val)

                if(ICF_SUCCESS == dummy_ret_val)
                {
                    /*  Rel 8.1
                     *  In tunnel mode stream id should always be 1
                     *  stream type should always be INVALID
                     *  as in tunnel mode IPTK do not understand
                     *  the stream type
                     *  stream status should always be ACTIVE
                     */
                    p_dest_data->stream_id = 1;
                    p_dest_data->stream_type = ICF_STREAM_TYPE_INVALID;
                    p_dest_data->stream_active = ICF_TRUE;

                    icf_port_memset(((icf_void_t *)(p_dest_data->p_int_sdp_stream)), 0,
                            ((icf_uint32_t)ICF_PORT_SIZEOF(icf_internal_sdp_stream_st)));    	

                    ICF_MEMGET(
                        p_mmi_pdb->p_glb_pdb,
                        ICF_PORT_SIZEOF(icf_tunnelled_sdp_info_st),
                        ICF_MEM_COMMON,
                        p_dest_data->p_int_sdp_stream->p_tunnelled_sdp,
                        ICF_DONOT_RET_ON_FAILURE,
                        p_mmi_pdb->p_ecode,
                        dummy_ret_val)
                    if(ICF_SUCCESS ==  dummy_ret_val)
                    {
                        icf_port_memcpy(
                               (p_dest_data->p_int_sdp_stream->p_tunnelled_sdp),
                               &(p_tunnel_med_cap_resp->tunnelled_sdp_info),
                               (icf_uint32_t const)(ICF_PORT_SIZEOF(icf_tunnelled_sdp_info_st)));
                        /*  Add node to the SDP list of internal message
                         *  to communicate it to SIC
                         */
                        dummy_ret_val = 
                               icf_cmn_add_node_to_list(p_mmi_pdb->p_glb_pdb,
                                   &get_med_cap_resp.p_sdp_list,
                                   p_dest_data);
                    }
                    else
                    {
                        memget_fail = ICF_TRUE;
                        ICF_MEMFREE(
                          p_mmi_pdb->p_glb_pdb,
                          p_dest_data->p_int_sdp_stream,
                          ICF_MEM_COMMON,
                          p_mmi_pdb->p_ecode, 
                          dummy_ret_val)
                            
                        /* MEMFREE node */
                        ICF_MEMFREE(
                           p_mmi_pdb->p_glb_pdb, 
                           p_dest_data, 
                           ICF_MEM_COMMON, 
                           p_mmi_pdb->p_ecode, 
                           dummy_ret_val)
                        ret_val = ICF_FAILURE;
                    }
                }
                else
                {
                    memget_fail = ICF_TRUE;
                            
                    /* MEMFREE node */
                    ICF_MEMFREE(
                          p_mmi_pdb->p_glb_pdb, 
                          p_dest_data, 
                          ICF_MEM_COMMON, 
                          p_mmi_pdb->p_ecode, 
                          dummy_ret_val)
                    ret_val = ICF_FAILURE;
                         
                }
                if(ICF_FAILURE == dummy_ret_val)
                {
					if ( ICF_NULL != p_dest_data)
					{
					if ( ICF_NULL !=  p_dest_data->p_int_sdp_stream )
					{
                    ICF_MEMFREE(
                          p_mmi_pdb->p_glb_pdb,
                          p_dest_data->p_int_sdp_stream->p_tunnelled_sdp,
                          ICF_MEM_COMMON,
                          p_mmi_pdb->p_ecode,
                          dummy_ret_val)

                    ICF_MEMFREE(
                          p_mmi_pdb->p_glb_pdb,
                          p_dest_data->p_int_sdp_stream,
                          ICF_MEM_COMMON,
                          p_mmi_pdb->p_ecode, 
                          dummy_ret_val)
                	}            
                    /* MEMFREE node */
                    ICF_MEMFREE(
                           p_mmi_pdb->p_glb_pdb, 
                           p_dest_data, 
                           ICF_MEM_COMMON, 
                           p_mmi_pdb->p_ecode, 
                           dummy_ret_val)
					}
                    ret_val = ICF_FAILURE;
                            
                }
                else
                {
                    ICF_KLOCWORK_FALSE_POSITIVE(p_mmi_pdb->p_glb_pdb,
                                &get_med_cap_resp.p_sdp_list)
                    ICF_KLOCWORK_FALSE_POSITIVE(p_mmi_pdb->p_glb_pdb,
                                p_dest_data)
                }
           }
           else
           {
               memget_fail = ICF_TRUE;
               ret_val = ICF_FAILURE;
           }
       } 
   }
   else
   {
       get_med_cap_resp.reason = p_tunnel_med_cap_resp->error_cause;
       ret_val = ICF_SUCCESS;
   }

   if(ICF_SUCCESS == ret_val)
   {
       p_mmi_pdb->p_internal_msg->p_msg_data = 
                 (icf_uint8_t *)&get_med_cap_resp;

       /*Call entry point of SIC*/
       ret_val = icf_sic_process_mesg(p_mmi_pdb->p_internal_msg);

       if(ICF_FAILURE == ret_val)
       {
           ICF_MMI_TRACE(p_mmi_pdb, ICF_INTERFACE_FAILURE)
           ret_val = ICF_FAILURE;
       }
   }
   else if(ICF_TRUE == memget_fail)
   {
       /* This is the case when failure is returned while MEMGET */
       ICF_ALARM(((icf_uint8_t *)"[MMI]: MEMGET failure"));
   }
   else 
   {
       /* This is the case when failure is returned by some function */
       ICF_ALARM(((icf_uint8_t *)"[MMI]: GENERIC failure"));
   }
   if(ICF_NULL != get_med_cap_resp.p_sdp_list)
   {
       /* Free memory of SDP list */
       p_src_list = get_med_cap_resp.p_sdp_list;
       ICF_KLOCWORK_FALSE_POSITIVE(p_mmi_pdb->p_glb_pdb,
                                p_dest_data)       
       p_dest_data = p_src_list->p_data;
       ICF_MEMFREE(p_mmi_pdb->p_glb_pdb, p_dest_data->p_int_sdp_stream->p_tunnelled_sdp, 
                 ICF_MEM_COMMON, p_mmi_pdb->p_ecode, dummy_ret_val)
     
       ICF_MEMFREE(p_mmi_pdb->p_glb_pdb, p_dest_data->p_int_sdp_stream, 
                 ICF_MEM_COMMON, p_mmi_pdb->p_ecode, dummy_ret_val)
    
       icf_cmn_delete_list(p_mmi_pdb->p_glb_pdb,
            	&get_med_cap_resp.p_sdp_list,ICF_MEM_COMMON);
   }

  ICF_FUNCTION_EXIT(p_mmi_pdb->p_glb_pdb)
  return ret_val;

}
