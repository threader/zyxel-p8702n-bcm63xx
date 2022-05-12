/******************************************************************************
 * 
 * FILE NAME:   icf_mmi_02_awt_cms_resp.c
 * 
 * DESCRIPTION: This file contains functions to handle events in the AWT CMS
 * 				response state in MMI.  
 *              
 * REVISION HISTORY:    
 *   Date            Name          Reference       Comments
 * 25-july-2006     Puneet       ICF Rel 6.0    Initial Version- support for
 * 												multiple m-lines
 * 18-Jan-2008     Kamini        IPTK Rel 8.0   Modified function 
 *                                              icf_mmi_update_sdp_from_cms_resp_for_stream_id
 *                                              to check the media transport mode. 
 * 24-Jan-2008     Kamini        IPTK Rel 8.0   Modified function icf_mmi_cms_failure_resp
 *                                              for MMI traces
 * 						
 * 24-Jan-2008      Abhishek Dhammawat Rel8.0   Modified
 *                                              icf_mmi_cms_failure_resp
 * 4-Apr-2008   Sumant Gupta    ICF 8.0         Rel7.1 Windows porting Merging
 * 22-Apr-2008  Shruti Thakral  SPR 18455       Klocwork warning removal Rel 8.0
 * 03-May-2008  Alok Tiwari     SPR 18500       Modified function 
 *                                           icf_mmi_update_sdp_for_tbcp_stream
 * 26-Jun-2008      Amit Sharma    SPR 18725      Populated warning codes
 * 04-Mar-2009 Saurabh Gupta	   SPR 19349      Klocworks fix
 * 
 * Copyright (c) 2008, Aricent.  
 *
 ******************************************************************************/

/* include files */
#include "icf_internal_api_id.h"
#include "icf_mmi_defs.h"
#include "icf_mmi_types.h"
#include "icf_mmi_macro.h"
#include "icf_mmi_prototypes.h"
#include "icf_dbm_prototypes.h"
#include "icf_ecode.h"


/******************************************************************************
 *
 * FUNCTION:        icf_mmi_02_dms_request
 *
 * DESCRIPTION:     This function is invoked when the MMI is 
 *                  awaiting CMS resp from MM and it receives a DMS request
 *                  Current State: ICF_MMI_02_AWT_CMS_RESP
 *                  
 * RETURNS:         ICF_SUCCESS in case of success and ICF_FAILURE in 
 *                  case of any failure.                 
 *
 *****************************************************************************/
icf_return_t icf_mmi_02_dms_request(
        INOUT icf_mmi_pdb_st             *p_mmi_pdb)
{
    icf_return_t                         ret_val = ICF_SUCCESS;
    icf_mmi_dms_request_st               *p_msg_data_recd = ICF_NULL;
    
    ICF_FUNCTION_ENTER(p_mmi_pdb->p_glb_pdb)

    p_msg_data_recd = (icf_mmi_dms_request_st *)
        (p_mmi_pdb->p_internal_msg->p_msg_data);

    /* Check if this is not forceful delete */
    if(ICF_FALSE == p_msg_data_recd->forceful_delete)
    {
        ICF_MMI_TRACE(p_mmi_pdb,ICF_TRACE_DMS_NOT_FORCEFUL)
        /* Dump the request */
    }
    /* If it is a forceful delete */
    else
    {
        ICF_MMI_TRACE(p_mmi_pdb,ICF_TRACE_FORCEFUL_DMS)
            
        /* Send DMS request so that if success is received for CMS then 
         * session can be deleted. In case of forceful DMs there is no need to 
         * wait for response of either DMS or REL RES REQ. Thus, both req will 
         * be issued simultaneously and MMI will go in IDLE state and the 
         * responses of both of then will be ignored as MMI will not be 
         * waiting for them.If QOS Support is enabled then MMI will also send
		 * secondary PDP Context DeActivation Request and thereafter will change 
		 * it's state to IDLE.
         */

		icf_mmi_send_forceful_dms_req(p_mmi_pdb);

		/* No need to send DMS resp to CC for forceful delete */
    }/* End else */ 
    
    ICF_FUNCTION_EXIT(p_mmi_pdb->p_glb_pdb)
    return ret_val;
}/* End function */


/******************************************************************************
 *
 * FUNCTION:        icf_mmi_02_cms_resp
 * 
 * DESCRIPTION:     This function is invoked when the MMI is
 *                  awaiting CMS resp from MM and it receives the same.
 *                  Current State: ICF_MMI_02_AWT_CMS_RESP
 *                  Current Event: ICF_MMI_CMS_RESP
 *                  
 * RETURNS:         ICF_SUCCESS in case of success and ICF_FAILURE in 
 *                  case of any failure.                  
 *
 ******************************************************************************/
icf_return_t icf_mmi_02_cms_resp(
        INOUT icf_mmi_pdb_st             *p_mmi_pdb)
{
    icf_return_t                         ret_val = ICF_SUCCESS;

    icf_create_media_session_resp_st *p_recv_msg_payload = ICF_NULL;
    
    ICF_FUNCTION_ENTER(p_mmi_pdb->p_glb_pdb)
        
    p_recv_msg_payload = (icf_create_media_session_resp_st *)
            (p_mmi_pdb->p_glb_pdb->p_recv_msg->payload);
    
    ICF_MMI_SET_STATE(p_mmi_pdb,ICF_MMI_00_IDLE)

    /* If the response is success then function to process success response 
     * will be called
     */

    if(ICF_SUCCESS == p_recv_msg_payload->result)
    {
        ICF_MMI_TRACE(p_mmi_pdb,ICF_TRACE_SUCCESS_RESP)
#ifndef ICF_UNIT_TEST
        ret_val = icf_mmi_check_api_total_streams(p_mmi_pdb, 
                       ICF_CREATE_MEDIA_SESSION_RESP);
		
        if(ICF_FAILURE == ret_val)
        {
            p_mmi_pdb->p_call_ctx->warning_header_code = 
                ICF_WARNING_CODE_INCOMPATIBLE_MEDIA_FMT;

            ICF_ALARM(((icf_uint8_t *)"[MMI]: MEDIA MANAGER failure"));
            icf_mmi_send_err_ind_call_clear(p_mmi_pdb);
            return ICF_SUCCESS;
        }
#endif            
        ret_val = icf_mmi_cms_success_resp(p_mmi_pdb);
        if(ICF_FAILURE == ret_val)
        {
            /* failure to process */
            ICF_MMI_TRACE(p_mmi_pdb,ICF_TRACE_FAILURE_TO_PROCESS_MSG)
        }
    }    
    
    /* If the response is failure then function to process failure response 
     * will be called
     */
    else
    {
        p_mmi_pdb->p_call_ctx->warning_header_code = 
            ICF_WARNING_CODE_INCOMPATIBLE_MEDIA_FMT;

        ICF_MMI_TRACE(p_mmi_pdb,ICF_TRACE_FAILURE_RESP)

        ret_val = icf_mmi_cms_failure_resp(p_mmi_pdb);
        if(ICF_FAILURE == ret_val)
        {
            /* failure to process */
            ICF_MMI_TRACE(p_mmi_pdb,ICF_TRACE_FAILURE_TO_PROCESS_MSG)             
		}        
#ifdef ICF_TRACE_ENABLE
       p_mmi_pdb->stream_type = 0;
       p_mmi_pdb->result = ICF_FAILURE;
       /*ICF_MMI_APP_TRACE(p_mmi_pdb,ICF_TRACE_STREAM_CREATE)*/
#endif
    }
    
    ICF_FUNCTION_EXIT(p_mmi_pdb->p_glb_pdb)
    return ret_val;        
}/* End function */ 

/******************************************************************************
 *
 * FUNCTION:        icf_mmi_cms_success_resp
 *
 * DESCRIPTION:     This function is invoked when ever success response is
 *                  received from MM for CMS request.
 *                   
 * RETURNS:         ICF_SUCCESS in case of success and ICF_FAILURE in 
 *                  case of any failure.       
 *
 ******************************************************************************/

icf_return_t icf_mmi_cms_success_resp(
        INOUT icf_mmi_pdb_st             *p_mmi_pdb)
{
    icf_return_t                        ret_val = ICF_SUCCESS,
										dummy_ret_val = ICF_SUCCESS;
    icf_create_media_session_resp_st	*p_recv_msg_payload = ICF_NULL;
	icf_mmi_internal_cms_resp_st		*p_cms_resp = ICF_NULL;
	icf_mmi_internal_mms_resp_st		*p_mms_resp = ICF_NULL;
	icf_stream_id_t						stream_count = ICF_NULL,
										stream_id = ICF_NULL;
	icf_media_type_t					stream_type = ICF_STREAM_TYPE_INVALID;
	icf_sdp_stream_st					sdp;

    ICF_FUNCTION_ENTER(p_mmi_pdb->p_glb_pdb)        
    
    /* Set the bit. This bit indicates that DMS request is to be send */
    ICF_MMI_SET_BIT(p_mmi_pdb,ICF_MEDIA_ESTABLISHED)

    /* Check the state of CC. If CC is in AWT DMS RESP State then 
     * current session is to be cleared
     */
    
    if(ICF_CC_07_AWT_DMS_RESP == p_mmi_pdb->p_call_ctx->call_state)
    {
		ICF_MMI_TRACE(p_mmi_pdb,ICF_TRACE_CC_AWT_DMS)
		ret_val = icf_mmi_send_awaited_dms_req(p_mmi_pdb);
        return ret_val;
    }/* End if to check state of CC */
   
    p_recv_msg_payload = (icf_create_media_session_resp_st *)
            (p_mmi_pdb->p_glb_pdb->p_recv_msg->payload);

#ifdef ICF_TRACE_ENABLE
    /* Start traces for data received in CMS Response */
    p_mmi_pdb->p_mmi_msg_payload = (icf_void_t *)p_recv_msg_payload;

    ICF_MMI_TRACE(p_mmi_pdb,ICF_TRACE_CMS_DATA_RECVD)
	icf_mmi_trace_api_sdp(p_mmi_pdb, ICF_CREATE_MEDIA_SESSION_RESP);
#endif

	icf_port_memset(&sdp, 0, ICF_PORT_SIZEOF(sdp));

	if(ICF_MMI_CMS_ONGOING == p_mmi_pdb->p_mmi_ctx->ongoing_event)
	{
		p_cms_resp = p_mmi_pdb->p_mmi_ctx->p_mmi_resp;

		/* Mark the failure streams in p_mmi_resp in MMI ctxt */
		if(p_recv_msg_payload->bit_mask & ICF_CMS_REJECT_STREAM_PRESENT)
		{
			for(stream_count = 0; (stream_count < p_cms_resp->count) && (stream_count < ICF_MAX_STREAM_PER_CALL); 
				stream_count++)
			{
				stream_id = p_cms_resp->stream_status[stream_count].stream_id;

				dummy_ret_val = icf_mmi_search_failure_stream(p_mmi_pdb, 
							&p_recv_msg_payload->rejected_stream_list,
							stream_id);
				
				if(ICF_SUCCESS == dummy_ret_val)
				{
					p_cms_resp->stream_status[stream_count].status_success = 
							ICF_FALSE;
					
#ifdef ICF_NAT_MEDIA_TRAVERSAL
					/* Send REL_PUBLIC_IP_PORT for failure stream */
					icf_mmi_send_rel_public_ip_port(p_mmi_pdb, stream_id);
#endif
				}
			}

			/* Send REL_RES_IND for failure streams */
			icf_mmi_send_rel_res_ind(p_mmi_pdb);

			/* Send CLOSE_CHANNEL_IND for failure streams */
			icf_mmi_send_close_channel_ind(p_mmi_pdb); /* this is sent for all streams including 
												        * TBCP */

			/* REL_PUBLIC_IP_PORT done already */
		}

		for(stream_count = 0; (stream_count < p_cms_resp->count) && (stream_count < ICF_MAX_STREAM_PER_CALL); stream_count++)
		{
			if(p_cms_resp->stream_status[stream_count].status_success == 
				ICF_TRUE)
			{
				ICF_MMI_SET_BIT(p_mmi_pdb,ICF_MEDIA_ESTABLISHED)

				stream_id = p_cms_resp->stream_status[stream_count].stream_id;
				stream_type = p_cms_resp->stream_status[stream_count].stream_type;

				if(stream_type == ICF_STREAM_TYPE_TBCP)
				{
					/* Update local SDP */
					ret_val = icf_mmi_update_sdp_for_tbcp_stream(
										p_mmi_pdb,
										stream_id,
										ICF_SDP_LOCAL);
					
					/* If call is incoming and FS */
					if((ICF_CALL_DIR & p_mmi_pdb->p_call_ctx->common_bitmask) && 
						(ICF_NULL != p_mmi_pdb->p_mmi_ctx->p_remote_recvd_sdp))
					{
						ret_val = icf_mmi_update_sdp_for_tbcp_stream(
										p_mmi_pdb,
										stream_id,
										ICF_SDP_APPLIED);
					}
				}
				else
				{
					/* Update local SDP */
					ret_val = icf_mmi_update_sdp_from_cms_resp_for_stream_id(
										p_mmi_pdb,stream_id,
										ICF_SDP_LOCAL,
										p_recv_msg_payload->p_accepted_stream_list);

					/* If call is incoming and FS */
					if((ICF_CALL_DIR & p_mmi_pdb->p_call_ctx->common_bitmask) && 
						(ICF_NULL != p_mmi_pdb->p_mmi_ctx->p_remote_recvd_sdp))
					{
						ret_val = icf_mmi_update_sdp_from_cms_resp_for_stream_id(
										p_mmi_pdb,stream_id,
										ICF_SDP_APPLIED,
										p_recv_msg_payload->p_accepted_stream_list);
					}
				}
			}
		}

		if(ICF_SUCCESS == ret_val)
		{
			/* send CMS_resp to CC */
			ret_val = icf_mmi_send_create_media_response(p_mmi_pdb);
		}
		else
		{
			ret_val = icf_mmi_send_err_ind_call_clear(p_mmi_pdb);
		}
	}
	else if(ICF_MMI_MMS_ADD_ONGOING == p_mmi_pdb->p_mmi_ctx->ongoing_event)
	{
		p_mms_resp = p_mmi_pdb->p_mmi_ctx->p_mmi_resp;

		/* Mark the failure streams in p_mmi_resp in MMI ctxt */
		if(p_recv_msg_payload->bit_mask & ICF_CMS_REJECT_STREAM_PRESENT)
		{
			p_mms_resp->added_stream_resp[ICF_NULL].status_success = ICF_FALSE;

			/* Send REL_RES_IND for this stream */
			icf_mmi_send_rel_res_ind(p_mmi_pdb);

			/* Send CLOSE_CHANNEL_IND for this stream */
			icf_mmi_send_close_channel_ind(p_mmi_pdb); /* send close channel ind. for the T.38 
														* stream */

#ifdef ICF_NAT_MEDIA_TRAVERSAL
			/* Send REL_PUBLIC_IP_PORT for this stream */
			icf_mmi_send_rel_public_ip_port(p_mmi_pdb, 
						p_mms_resp->added_stream_resp[ICF_NULL].stream_id);
#endif
			p_mms_resp->added_stream_resp[ICF_NULL].status_success = 
				ICF_FALSE;
		}
		else
		{
			/* Update local SDP */
			ret_val = icf_mmi_update_sdp_from_cms_resp_for_stream_id(
							p_mmi_pdb,
							p_mms_resp->added_stream_resp[ICF_NULL].stream_id,
							ICF_SDP_LOCAL,
							p_recv_msg_payload->p_accepted_stream_list);
			
			/* Check if the Reinvite for adding stream was Incoming if yes then
			 * Update applied SDP */
			dummy_ret_val = icf_mmi_get_sdp_for_stream_id(
							p_mmi_pdb,
							p_mms_resp->added_stream_resp[ICF_NULL].stream_id,
							ICF_SDP_REMOTE,
							&sdp);

			if(ICF_SUCCESS == dummy_ret_val)
			{
				ret_val = icf_mmi_update_sdp_from_cms_resp_for_stream_id(
							p_mmi_pdb,
							p_mms_resp->added_stream_resp[ICF_NULL].stream_id,
							ICF_SDP_APPLIED,
							p_recv_msg_payload->p_accepted_stream_list);
			}
		}

		if(ICF_SUCCESS == ret_val)
		{
			/* send MM_resp to CC */
			ret_val = icf_mmi_send_modify_media_response(p_mmi_pdb);
		}
		else
		{
			ret_val = icf_mmi_send_err_ind_call_clear(p_mmi_pdb);
		}
	}
	
    ICF_FUNCTION_EXIT(p_mmi_pdb->p_glb_pdb)
    return ret_val;        
}/* End function */    

/******************************************************************************
 *
 * FUNCTION:        icf_mmi_cms_failure_resp
 *
 * DESCRIPTION:     This function is invoked when ever failure response is
 *                  received from MM for CMS request.
 *                   
 * RETURNS:         ICF_SUCCESS in case of success and ICF_FAILURE in 
 *                  case of any failure.       
 *
 ******************************************************************************/

icf_return_t icf_mmi_cms_failure_resp(
        INOUT icf_mmi_pdb_st             *p_mmi_pdb)
{
    icf_return_t                         ret_val = ICF_SUCCESS;
	icf_mmi_internal_mms_resp_st		*p_mms_resp = ICF_NULL;
	icf_stream_id_t						stream_count = ICF_NULL;
    icf_create_media_session_resp_st    *p_recv_msg_payload = ICF_NULL;
    
    ICF_FUNCTION_ENTER(p_mmi_pdb->p_glb_pdb)

	ICF_MMI_PDB_SET_BIT(p_mmi_pdb,ICF_MMI_PDB_CMS_FAILURE)
        
           
    p_recv_msg_payload = (icf_create_media_session_resp_st *)
    (p_mmi_pdb->p_glb_pdb->p_recv_msg->payload);

    /*Rel 8.0 :Check for error code and then print trace 
      for SRTP Negotiation Failure*/
    if(ICF_CAUSE_SRTP_NEG_FAILURE == 
                   p_recv_msg_payload->error_cause)
    {
        p_mmi_pdb->p_call_ctx->warning_header_code = 
            ICF_WARNING_CODE_INCOMPATIBLE_TRANSPORT_PROTOCOL;
       ICF_MMI_APP_TRACE(p_mmi_pdb,ICF_TRACE_SRTP_NEG_FAILURE) 
    } 
    

    /* Check the state of CC */
    if(ICF_CC_07_AWT_DMS_RESP == p_mmi_pdb->p_call_ctx->call_state)
    {
        ICF_MMI_TRACE(p_mmi_pdb,ICF_TRACE_CC_AWT_DMS)
            
		if(ICF_MMI_CMS_ONGOING == p_mmi_pdb->p_mmi_ctx->ongoing_event)
		{
			/* Send rel res req */
			icf_mmi_send_close_channel_ind(p_mmi_pdb); /* send close channel ind. for all streams 
														* including TBCP and other media streams */
			icf_mmi_send_rel_res_ind(p_mmi_pdb);
			ret_val = icf_mmi_send_delete_media_response(p_mmi_pdb);
		}
        else
		{
			ret_val = icf_mmi_send_awaited_dms_req(p_mmi_pdb);
		}
        return ret_val;
    }/* End if to check state of CC */

	if(ICF_MMI_CMS_ONGOING == p_mmi_pdb->p_mmi_ctx->ongoing_event)
	{
		/* Send REL_RES_IND for all streams marked success in cms_resp except 
		 * TBCP */
		icf_mmi_send_rel_res_ind(p_mmi_pdb);

		/* Send CLOSE_CHANNEL_IND for all streams marked success in cms_resp 
		 * for this use cms_failure bit in MMI pdb */
		icf_mmi_send_close_channel_ind(p_mmi_pdb);
		
#ifdef ICF_NAT_MEDIA_TRAVERSAL
		/* Send REL_PUBLIC_IP_PORT for all streams marked success in cms_resp */
		icf_mmi_rel_nat_resources(p_mmi_pdb);
#endif
		((icf_mmi_internal_cms_resp_st *)p_mmi_pdb->p_mmi_ctx->p_mmi_resp)->\
				result = ICF_FAILURE;

                ((icf_mmi_internal_cms_resp_st *)p_mmi_pdb->p_mmi_ctx->p_mmi_resp)->\
                     error_cause = p_recv_msg_payload->error_cause; 


 
#ifdef ICF_QOS_SUPPORT
ICF_CHECK_QOS_SUPPORT_START(p_mmi_pdb->p_glb_pdb,ICF_TRUE)
		/* Precondition: need to send it only for incoming case */
		if((ICF_CALL_DIR & p_mmi_pdb->p_call_ctx->common_bitmask) && 
			(ICF_NULL != p_mmi_pdb->p_mmi_ctx->p_remote_recvd_sdp))
		{
            /* Rel8.0: This is hook for resource reservation. Previously
               icf_mmi_send_qos_release_req function was invoked here.
               However now we do not require to do PDP activation for IMS
               mode and keeping behaviour same for both the IMS/NON IMS
               mode we invoke non ims function*/
			ret_val = icf_mmi_send_qos_release_res_req(p_mmi_pdb);
		}
ICF_CHECK_QOS_SUPPORT_END
#endif	
		/* send CMS_resp to CC */
		ret_val = icf_mmi_send_create_media_response(p_mmi_pdb);
	}
	else if(ICF_MMI_MMS_ADD_ONGOING == p_mmi_pdb->p_mmi_ctx->ongoing_event)
	{
		/* Send REL_RES_IND for this stream */
		icf_mmi_send_rel_res_ind(p_mmi_pdb);

		/* Send CLOSE_CHANNEL_IND for this stream */
		icf_mmi_send_close_channel_ind(p_mmi_pdb); /* send close channel ind. for T.38 stream */

#ifdef ICF_NAT_MEDIA_TRAVERSAL
		/* Send REL_PUBLIC_IP_PORT for all this stream */
		icf_mmi_rel_nat_resources(p_mmi_pdb);
#endif

		/* send MM_resp to CC with ADD action as failure */
		p_mms_resp = p_mmi_pdb->p_mmi_ctx->p_mmi_resp;

                /* SPR 17399: If the error cause is Resources not available for T38 then 
                 * set the release cause to send 488
                 */
                if((ICF_CAUSE_RES_NOT_AVAILABLE_FOR_T38_FAX == 
                   p_recv_msg_payload->error_cause) &&
                    (ICF_NULL == p_mmi_pdb->p_call_ctx->release_cause))
                {
                    p_mmi_pdb->p_call_ctx->release_cause = 
                    ICF_CALL_CLEAR_REASON_RESOURCES_NOT_AVAILABLE_FOR_T38_FAX;
                    p_mmi_pdb->p_call_ctx->release_type |= 
                    ICF_REL_TYPE_INTERNAL_INITIATED;
                }                


		for(stream_count = 0; (stream_count < p_mms_resp->add_stream_count) && (stream_count <ICF_MAX_STREAM_PER_CALL) ;
			stream_count++)
		{
			p_mms_resp->added_stream_resp[stream_count].status_success = 
				ICF_FALSE;
		}

		ret_val = icf_mmi_send_modify_media_response(p_mmi_pdb);
	}

	/* Reset Bit */
	ICF_MMI_PDB_RESET_BIT(p_mmi_pdb,ICF_MMI_PDB_CMS_FAILURE)
	
    ICF_FUNCTION_EXIT(p_mmi_pdb->p_glb_pdb)
    return ret_val;        
}/* End function */ 


/******************************************************************************
 *
 * FUNCTION:        icf_mmi_update_sdp_from_cms_resp_for_stream_id
 * 
 * DESCRIPTION:            
 *
 ******************************************************************************/
					
icf_return_t	icf_mmi_update_sdp_from_cms_resp_for_stream_id
 (
	icf_mmi_pdb_st		*p_mmi_pdb, 
	icf_stream_id_t		stream_id,
	icf_uint8_t			sdp_type,
	icf_list_st			*p_list
)
{
	icf_return_t				ret_val = ICF_SUCCESS;
	icf_resp_stream_st			*p_data = ICF_NULL;
	icf_sdp_stream_st			sdp;
	icf_boolean_t				corrupt_address = ICF_FALSE;
#ifdef ICF_QOS_SUPPORT
	icf_internal_sdp_stream_st			*p_int_sdp = ICF_NULL;
#endif

	ICF_FUNCTION_ENTER(p_mmi_pdb->p_glb_pdb)

	icf_port_memset(&sdp, 0 , ICF_PORT_SIZEOF(sdp));

	while(ICF_NULL != p_list)
	{
		p_data = p_list->p_data;

		if(stream_id == p_data->stream_id)
		{
		        /* Check for trans mode type, it should be RTP/AVP or RTP/SAVP*/
			switch(p_data->stream_type)
			{
				case ICF_STREAM_TYPE_AUDIO:
				case ICF_STREAM_TYPE_VIDEO:

					if((ICF_TRANSPORT_MODE_RTP_AVP != 
                                              p_data->local_sdp.trans_mode) &&
                                           (ICF_TRANSPORT_MODE_RTP_SAVP != 
			                        p_data->local_sdp.trans_mode))
						corrupt_address = ICF_TRUE;

					break;

				case ICF_STREAM_TYPE_FAX:
#ifdef ICF_FAX_SUPPORT
#ifdef NON_IMS_CLIENT
					if(p_data->local_sdp.trans_mode != 
						ICF_TRANSPORT_MODE_UDPTL)
						corrupt_address = ICF_TRUE;
#endif
#endif
					break;
			}

			if(ICF_FAILURE == icf_mmi_validate_transport_addr(p_mmi_pdb, 
				&(p_data->local_sdp.stream_address)))
			{
				corrupt_address = ICF_TRUE;
			}

			if(ICF_TRUE == corrupt_address)
			{
				ICF_ALARM(((icf_uint8_t *)"[MMI]: Corrupt Address in CMS resp"));

				switch(p_mmi_pdb->p_mmi_ctx->ongoing_event)
				{
				case ICF_MMI_CMS_ONGOING:
					icf_mmi_mark_failure_stream_cms_resp(p_mmi_pdb, 
						p_data->stream_id);
				break;

				case ICF_MMI_MMS_ADD_ONGOING:
					icf_mmi_mark_failure_stream_mms_add_resp(p_mmi_pdb, 
						p_data->stream_id);
				break;
				}
			}

			if(ICF_SDP_APPLIED == sdp_type)
			{
				if(ICF_FALSE == icf_mmi_validate_commited_no_of_codecs(
									p_mmi_pdb,
									stream_id,
									p_data->local_sdp.p_codec_info_list))
				{
					icf_mmi_mark_failure_stream_cms_resp(p_mmi_pdb,stream_id);
					break;
				}
			}

			ret_val = icf_mmi_check_sdp_in_call_ctx(p_mmi_pdb,
											stream_id,
											sdp_type);

			if(ICF_SDP_APPLIED == sdp_type)
			{
				icf_mmi_get_sdp_for_stream_id(
										p_mmi_pdb,
										stream_id,
										ICF_SDP_REMOTE,
										&sdp);

				ret_val = icf_cmn_update_media_params_for_stream_id_in_call_ctxt(
													p_mmi_pdb->p_glb_pdb,
													stream_id,
													sdp_type,
													&sdp);
			}
			else if(ICF_SDP_LOCAL == sdp_type)
			{		
				ret_val = icf_cmn_update_media_params_for_stream_id_in_call_ctxt(
													p_mmi_pdb->p_glb_pdb,
													stream_id,
													sdp_type,
													&(p_data->local_sdp));


#ifdef ICF_QOS_SUPPORT
ICF_CHECK_QOS_SUPPORT_START(p_mmi_pdb->p_glb_pdb,ICF_TRUE)
				switch(p_data->stream_type)
				{
					case ICF_STREAM_TYPE_AUDIO:

						if(!p_data->local_sdp.media_stream_param.audio_profile.bandwidth)
						{
							icf_cmn_fetch_sdp_in_list_for_stream_id
											(p_mmi_pdb->p_glb_pdb,
											 stream_id,
											 ICF_SDP_LOCAL,
											 &p_int_sdp);
							if ( p_int_sdp)
							{
							p_int_sdp->sdp_stream.media_stream_param.\
								 audio_profile.bit_mask |= ICF_AUDIO_PROF_BNDWDTH_PRESENT;

							p_int_sdp->sdp_stream.media_stream_param.\
								 audio_profile.bandwidth = ICF_QOS_AUDIO_CODEC_BANDWIDTH; 
							}
						 }
						 break;

					case ICF_STREAM_TYPE_VIDEO:

						if(!p_data->local_sdp.media_stream_param.video_profile.bandwidth)
						{
							icf_cmn_fetch_sdp_in_list_for_stream_id
											(p_mmi_pdb->p_glb_pdb,
											 stream_id,
											 ICF_SDP_LOCAL,
											 &p_int_sdp);

							if ( p_int_sdp)
							{
							p_int_sdp->sdp_stream.media_stream_param.\
								 video_profile.bit_mask |= ICF_VIDEO_PROFILE_BANDWIDTH_PRESENT;

							p_int_sdp->sdp_stream.media_stream_param.\
								 video_profile.bandwidth = ICF_QOS_VIDEO_CODEC_BANDWIDTH; 
							}
						 }
						 break;	
					}
ICF_CHECK_QOS_SUPPORT_END
#endif

			}

			ret_val = icf_cmn_update_codecs_for_stream_id_in_call_ctxt(
													p_mmi_pdb->p_glb_pdb,
													stream_id,
													sdp_type,
													&(p_data->local_sdp));
			break;
		}
		p_list = p_list->p_next;
	}

	ICF_FUNCTION_EXIT(p_mmi_pdb->p_glb_pdb)
	return ret_val;
}


/******************************************************************************
 *
 * FUNCTION:        icf_mmi_update_sdp_for_tbcp_stream
 * 
 * DESCRIPTION:            
 *
 ******************************************************************************/
					
icf_return_t	icf_mmi_update_sdp_for_tbcp_stream
(
	icf_mmi_pdb_st		*p_mmi_pdb, 
	icf_stream_id_t		stream_id,
	icf_uint8_t			sdp_type
)
{
	icf_return_t				ret_val = ICF_SUCCESS;
	icf_app_conf_st				*p_app_conf = ICF_NULL;
	icf_internal_list_st		*p_app_profile_node = ICF_NULL;
	icf_uint32_t				loc_stream = stream_id;
	icf_list_st					*p_enc_attrib = ICF_NULL;
	icf_internal_sdp_stream_st	*p_int_sdp = ICF_NULL;
	icf_sdp_stream_st			sdp;

	ICF_FUNCTION_ENTER(p_mmi_pdb->p_glb_pdb)

	/*icf_port_memset(&sdp, 0, ICF_PORT_SIZEOF(sdp));*/
        icf_mmi_init_sdp(&sdp);

	ret_val = icf_mmi_check_sdp_in_call_ctx(p_mmi_pdb,
									stream_id,
									sdp_type);

	icf_cmn_fetch_sdp_in_list_for_stream_id(
										p_mmi_pdb->p_glb_pdb,
										stream_id, 
										sdp_type, 
										&p_int_sdp);

     /*SPR-18500 resolved (138 bytes of memory leak)*/
    /*Delete memory allocated to encoded attribute list before 
      create new list through function icf_mmi_clone_enc_attrib_list */
	  if ( p_int_sdp)
	  {
    if(ICF_NULL != p_int_sdp->sdp_stream.p_encoded_attributes)
      {
          icf_cmn_delete_list(p_mmi_pdb->p_glb_pdb,
                              &p_int_sdp->sdp_stream.p_encoded_attributes,      
                              ICF_MEM_COMMON);

       }
	}

	if(ICF_SDP_LOCAL == sdp_type)
	{
		icf_cfg_fetch_app_conf(p_mmi_pdb->p_glb_pdb,
			p_mmi_pdb->p_call_ctx->app_id,&p_app_conf,p_mmi_pdb->p_ecode);

		if(ICF_NULL != p_app_conf)
		{
			p_app_profile_node = icf_cmn_get_node_in_internal_list(
									p_app_conf->p_app_media_profile,
									&loc_stream);

			if(ICF_NULL != p_app_profile_node)
			{
				p_enc_attrib = 
					((icf_stream_capab_st*)p_app_profile_node->p_payload)->\
					p_encoded_attributes;
			}

			if( p_int_sdp && (ICF_NULL != p_enc_attrib))
			{
				icf_mmi_clone_enc_attrib_list(
							p_mmi_pdb,
							p_enc_attrib,
							&p_int_sdp->sdp_stream.p_encoded_attributes);
                ICF_KLOCWORK_FALSE_POSITIVE(p_mmi_pdb->p_glb_pdb,
                            p_int_sdp->sdp_stream.p_encoded_attributes)
			}
		}
	}
	else
	if(ICF_SDP_APPLIED == sdp_type)
	{
		icf_mmi_get_sdp_for_stream_id(
							p_mmi_pdb,
							stream_id,
							ICF_SDP_REMOTE,
							&sdp);

		if ( p_int_sdp ) 
		{
			if(sdp.p_encoded_attributes )
			icf_mmi_clone_enc_attrib_list(
							p_mmi_pdb,
							sdp.p_encoded_attributes,
							&p_int_sdp->sdp_stream.p_encoded_attributes);

            ICF_KLOCWORK_FALSE_POSITIVE(p_mmi_pdb->p_glb_pdb,
                            p_int_sdp->sdp_stream.p_encoded_attributes)
			p_int_sdp->sdp_stream.stream_address = sdp.stream_address;

			p_int_sdp->sdp_stream.rtcp_address = sdp.rtcp_address;
		}

	}

	ICF_FUNCTION_EXIT(p_mmi_pdb->p_glb_pdb)
	return ret_val;
}
