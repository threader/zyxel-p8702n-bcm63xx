/******************************************************************************
 * 
 * FILE NAME:   icf_mmi_06_awt_open_chnl_resp.c
 * 
 * DESCRIPTION: This file contains functions to handle events in the AWT_OPEN
 * 				CHANNEL_RESP state in MMI.
 *              
 * REVISION HISTORY:    
 *   Date            Name          Reference       Comments
 * 25-july-2006     Puneet       ICF Rel 6.0    Initial Version- support for
 * 												multiple m-lines
 *
 * 30-Nov-2006      Deepti       ICF Rel 6.2        NAT Feature Support
 * 16-Mar-2007      Deepti       SPR 14577          Changes for one call after
 *                               SPR 14574          another. 
 *                               SPR 14564
 * 7-Apr-2008   Sumant Gupta    ICF 8.0         Rel7.1 Windows porting Merging
 * 26-Jun-2008      Amit Sharma    SPR 18725      Populated warning codes
 * 04-Mar-2009 Saurabh Gupta	   SPR 19349      Klocworks fix
 * 11-Jun-2009 Ashutosh Mohan     SPR 19737     Changes done for GCC 4.1 warnings removal
 * Copyright (c) 2006, Aricent.  
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
 * FUNCTION:        icf_mmi_06_dms_request
 * 
 * DESCRIPTION:     This function is invoked when the MMI is
 *                  awaiting Open Channel resp from MM and it receives DMS req 
 *                  from CC. 
 *                                    
 * RETURNS:         ICF_SUCCESS in case of success and ICF_FAILURE in 
 *                  case of any failure.                  
 *
 *****************************************************************************/

icf_return_t icf_mmi_06_dms_request(
        INOUT icf_mmi_pdb_st             *p_mmi_pdb)
{
    icf_return_t             ret_val = ICF_SUCCESS;

	ICF_FUNCTION_ENTER(p_mmi_pdb->p_glb_pdb)

	ret_val = icf_mmi_01_dms_request(p_mmi_pdb);

	ICF_FUNCTION_EXIT(p_mmi_pdb->p_glb_pdb)
    
    return ret_val;
}    

/******************************************************************************
 *
 * FUNCTION:        icf_mmi_06_open_channel_resp
 * 
 * DESCRIPTION:     This function is invoked when the MMI is
 *                  awaiting Open Channel resp from MM and it receives it.
 *                                    
 * RETURNS:         ICF_SUCCESS in case of success and ICF_FAILURE in 
 *                  case of any failure.                  
 *
 *****************************************************************************/
icf_return_t icf_mmi_06_open_channel_resp(
        INOUT icf_mmi_pdb_st             *p_mmi_pdb)
{
    icf_return_t    ret_val = ICF_SUCCESS;
    icf_open_media_channel_resp_st    *p_recv_msg_payload = ICF_NULL;
	
    ICF_FUNCTION_ENTER(p_mmi_pdb->p_glb_pdb)
 
    p_recv_msg_payload = (icf_open_media_channel_resp_st *)
        (p_mmi_pdb->p_glb_pdb->p_recv_msg->payload);

    ICF_MMI_SET_STATE(p_mmi_pdb,ICF_MMI_00_IDLE)
	
    if(ICF_SUCCESS == p_recv_msg_payload->result)
    {
#ifndef ICF_UNIT_TEST
        ret_val = icf_mmi_check_api_total_streams(p_mmi_pdb, 
                     ICF_OPEN_MEDIA_CHANNEL_RESP);

        if(ICF_FAILURE == ret_val)
        {
            p_mmi_pdb->p_call_ctx->warning_header_code = 
            ICF_WARNING_CODE_INCOMPATIBLE_MEDIA_FMT;

            ICF_ALARM(((icf_uint8_t *)"[MMI]: MEDIA MANAGER failure"));
            icf_mmi_send_err_ind_call_clear(p_mmi_pdb);
            return ICF_SUCCESS;
        }
#endif 
        icf_mmi_open_channel_success_resp(p_mmi_pdb);
    }
    else
    {
        p_mmi_pdb->p_call_ctx->warning_header_code = 
            ICF_WARNING_CODE_INCOMPATIBLE_MEDIA_FMT;

        if((ICF_CAUSE_RES_NOT_AVAILABLE_FOR_T38_FAX ==
           p_recv_msg_payload->error_cause) &&
            (ICF_NULL == p_mmi_pdb->p_call_ctx->release_cause))
        {
            p_mmi_pdb->p_call_ctx->release_cause = 
            ICF_CALL_CLEAR_REASON_RESOURCES_NOT_AVAILABLE_FOR_T38_FAX;
        } 

        icf_mmi_open_channel_failure_resp(p_mmi_pdb);	
#ifdef ICF_TRACE_ENABLE
        p_mmi_pdb->result = ICF_FAILURE;
        p_mmi_pdb->stream_type = 0;
        /*ICF_MMI_APP_TRACE(p_mmi_pdb,
        ICF_TRACE_RTP_ADDR_ALLOCATE)*/
#endif
    }

    ICF_FUNCTION_EXIT(p_mmi_pdb->p_glb_pdb)
    
    return ret_val;
} 

/******************************************************************************
 *
 * FUNCTION:    icf_mmi_open_channel_success_resp
 *
 * DESCRIPTION: 
 *****************************************************************************/
icf_return_t	icf_mmi_open_channel_success_resp
(
	icf_mmi_pdb_st	*p_mmi_pdb
)
{
	icf_return_t					ret_val = ICF_SUCCESS;
	icf_open_media_channel_resp_st	*p_recv_msg_payload = ICF_NULL;
	icf_mmi_internal_cms_resp_st	*p_cms_resp = ICF_NULL;
	icf_mmi_internal_mms_resp_st	*p_mms_resp = ICF_NULL;
	icf_boolean_t					send_cms = ICF_FALSE;
#ifdef ICF_NAT_MEDIA_TRAVERSAL
    icf_transport_address_st        *p_local_addr = ICF_NULL;
    icf_return_t                    dummy_retval = ICF_SUCCESS;
    icf_sdp_stream_st		        sdp;
    icf_stream_id_t                 stream_count=0;
    icf_config_data_st              *p_config_data = ICF_NULL;
    icf_line_data_st                *p_line_data = ICF_NULL;
    icf_rport_config_data_st        *p_rport_config_data = ICF_NULL;
#endif

	ICF_FUNCTION_ENTER(p_mmi_pdb->p_glb_pdb)

    /* If CC is waiting for DMS response then send success response to it */
    if(ICF_CC_07_AWT_DMS_RESP == p_mmi_pdb->p_call_ctx->call_state)
    {
        ICF_MMI_TRACE(p_mmi_pdb,ICF_TRACE_CC_AWT_DMS)

		if(ICF_MMI_CMS_ONGOING == p_mmi_pdb->p_mmi_ctx->ongoing_event)
		{
			/* No CMS done yet so no need for DMS */
			/* Send REL_RES_IND for all active streams minus TBCP in call ctxt*/
			icf_mmi_send_rel_res_ind(p_mmi_pdb);

			/* Send CLOSE_CHANNEL for all active streams in call ctxt */
			icf_mmi_send_close_channel_ind(p_mmi_pdb);
			ret_val = icf_mmi_send_delete_media_response(p_mmi_pdb);
		}
        else /* This is open channel response for mid call request so DMS 
			  * required */
		{
			ret_val = icf_mmi_send_awaited_dms_req(p_mmi_pdb);
		}
        /* No other processing is needed if CC is in AWT DMS state */
        return ret_val;        
    }/* End if */

	p_recv_msg_payload = (icf_open_media_channel_resp_st *)
        (p_mmi_pdb->p_glb_pdb->p_recv_msg->payload);

#ifdef ICF_TRACE_ENABLE
    /* Start traces to print received data */
    p_mmi_pdb->p_mmi_msg_payload = (icf_void_t *)p_recv_msg_payload;
	ICF_MMI_TRACE(p_mmi_pdb,ICF_TRACE_OCR_DATA_RECVD)
	icf_mmi_trace_api_sdp(p_mmi_pdb, ICF_OPEN_MEDIA_CHANNEL_RESP);
#endif
	/* Update IP/port in Local SDP and in MMI ctxt for all success streams */
	ret_val = icf_mmi_update_sdp_from_open_channel_resp(p_mmi_pdb,
									p_recv_msg_payload->p_channel_addr_list);

	if(ICF_FAILURE == ret_val)
	{
		ICF_ALARM(((icf_uint8_t *)"[MMI]: INVALID response for OPEN_CHANNEL_REQ"));
		if(ICF_MMI_CMS_ONGOING == p_mmi_pdb->p_mmi_ctx->ongoing_event)
		{
			icf_mmi_mark_all_stream_cms_resp(p_mmi_pdb);
			ret_val = icf_mmi_send_create_media_response(p_mmi_pdb);
		}
		else if(ICF_MMI_MMS_ADD_ONGOING == p_mmi_pdb->p_mmi_ctx->ongoing_event)
		{
			icf_mmi_mark_all_stream_mms_add_resp(p_mmi_pdb);
			ret_val = icf_mmi_send_modify_media_response(p_mmi_pdb);
		}
		return ret_val;
	}

	if(ICF_MMI_CMS_ONGOING == p_mmi_pdb->p_mmi_ctx->ongoing_event)
	{
		ICF_MMI_SET_BIT(p_mmi_pdb,ICF_MMI_CHANNEL_OPENED)

		p_cms_resp = p_mmi_pdb->p_mmi_ctx->p_mmi_resp;

#ifdef ICF_NAT_MEDIA_TRAVERSAL
/* Get pointer to config data from DBM */

        ret_val = icf_dbm_get_module_glb_data(

            p_mmi_pdb->p_glb_pdb,

            (icf_int_module_id_t)ICF_INT_MODULE_CFG,

            (icf_void_t *)&p_config_data, p_mmi_pdb->p_ecode);

        

        ICF_ASSERT(p_mmi_pdb->p_glb_pdb, (ICF_NULL != p_config_data));

        if (ICF_CFG_SELF_ADDR_PER_LINE ==
              p_config_data->self_addr_per_line)
        {
             if(ICF_FAILURE == icf_dbm_fetch_line_cntxt_blk(
                  p_mmi_pdb->p_glb_pdb, 
                  p_mmi_pdb->p_call_ctx->line_id,
                  &p_line_data, p_mmi_pdb->p_ecode))
             {
                    ret_val = ICF_FAILURE;
                    ICF_PRINT(((icf_uint8_t *)"\n No line data block could be found\n"));
                    return ret_val;
             }
             else
             {
                  p_rport_config_data = p_line_data->p_rport_config_data;
             }

        }
        else
        {
             p_rport_config_data = &(p_config_data->rport_config_data);
        }

        if(ICF_TRUE == p_rport_config_data->is_rport_reqd)

        {
			p_mmi_pdb->p_mmi_ctx->nat_stream_count = ICF_NULL;

            p_mmi_pdb->p_mmi_ctx->no_of_nat_ctxt = ICF_NULL;

        for(stream_count = p_mmi_pdb->p_mmi_ctx->nat_stream_count; stream_count < \
                (p_cms_resp->count + p_mmi_pdb->p_mmi_ctx->nat_stream_count);\
                stream_count++)
		{
			dummy_retval = icf_mmi_get_sdp_for_stream_id(p_mmi_pdb,\
                    p_cms_resp->stream_status[stream_count].stream_id,\
                    ICF_SDP_LOCAL,&sdp);
            if(ICF_SUCCESS == dummy_retval)
            {
                p_local_addr =  &(sdp.stream_address);
                /* Update the Stream local address in the nat_stream_info_st.
                 */
                icf_port_memcpy(&(p_mmi_pdb->p_mmi_ctx->nat_streams[stream_count].\
                            local_addr),p_local_addr,(icf_uint32_t)ICF_PORT_SIZEOF(\
                                icf_transport_address_st));
                p_local_addr = ICF_NULL;

            }
            /* populate nat_streams in MMI ctxt for streams for which 
			 * GET_PUBLIC_IP_PORT must be sent */
			p_mmi_pdb->p_mmi_ctx->nat_streams
					[stream_count].stream_id = 
					p_cms_resp->stream_status[stream_count].stream_id;

			p_mmi_pdb->p_mmi_ctx->nat_streams[stream_count].count++;
            p_mmi_pdb->p_mmi_ctx->nat_streams[stream_count].is_nat_complete =\
                                                                  ICF_FALSE;
            if((ICF_STREAM_TYPE_AUDIO == p_cms_resp->stream_status\
                    [stream_count].stream_type)||
                (ICF_STREAM_TYPE_VIDEO == p_cms_resp->stream_status\
                 [stream_count].stream_type))
            {
                /* Increment the count by 1 for RTCP address also.
                 */
                p_mmi_pdb->p_mmi_ctx->nat_streams[stream_count].count++;
            }
        }
        p_mmi_pdb->p_mmi_ctx->nat_stream_count = stream_count;
        p_mmi_pdb->p_mmi_ctx->restore_count = stream_count;
 }

        else

        {

            send_cms = ICF_TRUE;

        }
	
#else
			send_cms = ICF_TRUE;
#endif
	}
	else if(ICF_MMI_MMS_ADD_ONGOING == p_mmi_pdb->p_mmi_ctx->ongoing_event)
	{
		p_mms_resp = p_mmi_pdb->p_mmi_ctx->p_mmi_resp;

#ifdef ICF_NAT_MEDIA_TRAVERSAL
/* Get pointer to config data from DBM */

        ret_val = icf_dbm_get_module_glb_data(

            p_mmi_pdb->p_glb_pdb,

            (icf_int_module_id_t)ICF_INT_MODULE_CFG,

            (icf_void_t *)&p_config_data, p_mmi_pdb->p_ecode);

        

        ICF_ASSERT(p_mmi_pdb->p_glb_pdb, (ICF_NULL != p_config_data));

        if(ICF_TRUE == p_config_data->rport_config_data.is_rport_reqd)
        {
            stream_count = p_mmi_pdb->p_mmi_ctx->nat_stream_count;

		/*for(stream_count = p_mmi_pdb->p_mmi_ctx->nat_stream_count; \
                stream_count < (p_mms_resp->add_stream_count + p_mmi_pdb->\
                    p_mmi_ctx->nat_stream_count);stream_count++)
		{*/
            dummy_retval = icf_mmi_get_sdp_for_stream_id(p_mmi_pdb,\
                    p_mms_resp->added_stream_resp[stream_count- 1].stream_id,\
                    ICF_SDP_LOCAL,&sdp);
            if(ICF_SUCCESS == dummy_retval)
            {
                p_local_addr =  &(sdp.stream_address);
                /* Update the Stream local address in the nat_stream_info_st.
                 */
                icf_port_memcpy(&(p_mmi_pdb->p_mmi_ctx->nat_streams[stream_count].\
                            local_addr),p_local_addr,(icf_uint32_t)ICF_PORT_SIZEOF(\
                                icf_transport_address_st));
                p_local_addr = ICF_NULL;

            }

            /* populate nat_streams in MMI ctxt for streams for which 
			 * GET_PUBLIC_IP_PORT must be sent */
			p_mmi_pdb->p_mmi_ctx->nat_streams[stream_count].stream_id = 
				p_mms_resp->added_stream_resp[stream_count-1].stream_id;

			p_mmi_pdb->p_mmi_ctx->nat_streams[stream_count].count++;
            p_mmi_pdb->p_mmi_ctx->nat_streams[stream_count].is_nat_complete =\
                                                                 ICF_FALSE;
           if((ICF_STREAM_TYPE_AUDIO == p_mms_resp->added_stream_resp\
							[stream_count-1].stream_type)||
              (ICF_STREAM_TYPE_VIDEO == p_mms_resp->added_stream_resp\
							[stream_count-1].stream_type))
            {
                /* Increment the count by 1 for RTCP address also.
                 */
                p_mmi_pdb->p_mmi_ctx->nat_streams[stream_count].count++;
            }
			p_mmi_pdb->p_mmi_ctx->nat_stream_count = stream_count + 1;

            p_mmi_pdb->p_mmi_ctx->restore_count = stream_count + 1;

       /* }*/

        }

        else

        {

            send_cms = ICF_TRUE;

        }

		

#else
        send_cms = ICF_TRUE;
#endif
	}

#ifdef ICF_NAT_MEDIA_TRAVERSAL

	if(ICF_TRUE == send_cms)
	{

		/* Send CMS req for success streams in cms_resp/mms_resp in MMI ctxt */

		ret_val = icf_mmi_send_cms_req(p_mmi_pdb);

		if(ICF_SUCCESS == ret_val)
		{
			ICF_MMI_SET_STATE(p_mmi_pdb,ICF_MMI_02_AWT_CMS_RESP)
		}
	}	
	else if(ICF_NULL != p_mmi_pdb->p_mmi_ctx->nat_stream_count)
	{
		/* Send nat_get_public_ip_port_req for first stream in cms_resp or
		 * mms_resp in MMI ctxt rest would be sent in consequent response to 
		 * this request */
		
		ret_val = icf_mmi_send_get_public_ip_port_req(p_mmi_pdb,
				(p_mmi_pdb->p_mmi_ctx->nat_streams[(p_mmi_pdb->p_mmi_ctx->\
                        nat_stream_count) - 1]).stream_id,\
                ICF_MEDIA_REQ_TYPE_STREAM);

		if(ICF_SUCCESS == ret_val)
		{
			ICF_MMI_SET_STATE(p_mmi_pdb,ICF_MMI_10_AWT_NAT_PUBLIC_IP_PORT_RESP)
		}
	}
#else
	if(ICF_TRUE == send_cms)
	{
		/* Send CMS req for success streams in cms_resp/mms_resp in MMI ctxt */
		ret_val = icf_mmi_send_cms_req(p_mmi_pdb);

		if(ICF_SUCCESS == ret_val)
		{
			ICF_MMI_SET_STATE(p_mmi_pdb,ICF_MMI_02_AWT_CMS_RESP)
		}
	}
#endif

	ICF_FUNCTION_EXIT(p_mmi_pdb->p_glb_pdb)

	return ret_val;
}

/******************************************************************************
 *
 * FUNCTION:    icf_mmi_open_channel_failure_resp
 *
 * DESCRIPTION: 
 *****************************************************************************/

icf_return_t	icf_mmi_open_channel_failure_resp(icf_mmi_pdb_st	*p_mmi_pdb)
{
	icf_return_t					ret_val = ICF_SUCCESS;
	icf_mmi_internal_mms_resp_st   *p_mms_resp = ICF_NULL;
	icf_stream_id_t					stream_count = ICF_NULL;
    icf_open_media_channel_resp_st  *p_recv_msg_payload = ICF_NULL;

	ICF_FUNCTION_ENTER(p_mmi_pdb->p_glb_pdb)

    /* If CC is waiting for DMS response then send success response to it */
    if(ICF_CC_07_AWT_DMS_RESP == p_mmi_pdb->p_call_ctx->call_state)
    {
        ICF_MMI_TRACE(p_mmi_pdb,ICF_TRACE_CC_AWT_DMS)

		if(ICF_MMI_CMS_ONGOING == p_mmi_pdb->p_mmi_ctx->ongoing_event)
		{
			/* No CMS done yet so no need for DMS */
			/* Send REL_RES_IND for all active streams minus TBCP in call ctxt*/
			ret_val = icf_mmi_send_rel_res_ind(p_mmi_pdb);

			ret_val = icf_mmi_send_delete_media_response(p_mmi_pdb);
		}
        else /* This is open channel response for mid call request so DMS 
			  * required*/
		{
			ret_val = icf_mmi_send_awaited_dms_req(p_mmi_pdb);
		}
        
        /* No other processing is needed if CC is in AWT DMS state */
        return ret_val;        
    }/* End if */

	if(ICF_MMI_CMS_ONGOING == p_mmi_pdb->p_mmi_ctx->ongoing_event)
	{
		/* make failure cms_resp and send to CC */

		((icf_mmi_internal_cms_resp_st *)p_mmi_pdb->p_mmi_ctx->p_mmi_resp)->\
			result = ICF_FAILURE;

         /* Filling the  error cause */ 
        p_recv_msg_payload = (icf_open_media_channel_resp_st *)
                               (p_mmi_pdb->p_glb_pdb->p_recv_msg->payload);
       ((icf_mmi_internal_cms_resp_st *)p_mmi_pdb->p_mmi_ctx->p_mmi_resp)->\
                        error_cause = p_recv_msg_payload->error_cause;

		ret_val = icf_mmi_send_create_media_response(p_mmi_pdb);
	}
	else if(ICF_MMI_MMS_ADD_ONGOING == p_mmi_pdb->p_mmi_ctx->ongoing_event)
	{
		/* Update mms_resp for add as failure and send the MM_resp to CC.*/

		p_mms_resp = p_mmi_pdb->p_mmi_ctx->p_mmi_resp;

		for(stream_count = 0; (stream_count < p_mms_resp->add_stream_count) && (stream_count < ICF_MAX_STREAM_PER_CALL);
			stream_count++)
		{
			p_mms_resp->added_stream_resp[stream_count].status_success = 
				ICF_FALSE;
		}
		icf_mmi_send_rel_res_ind(p_mmi_pdb);
		icf_mmi_send_modify_media_response(p_mmi_pdb);
	}
	
	ICF_FUNCTION_EXIT(p_mmi_pdb->p_glb_pdb)
	return ret_val;
}


/******************************************************************************
 *
 * FUNCTION:    icf_mmi_update_sdp_from_open_channel_resp
 *
 * DESCRIPTION: 
 *****************************************************************************/

icf_return_t	icf_mmi_update_sdp_from_open_channel_resp
(
	icf_mmi_pdb_st	*p_mmi_pdb,
	icf_list_st		*p_src
)
{
	icf_return_t				ret_val = ICF_FAILURE;
	icf_internal_list_st		*p_dest = ICF_NULL;
	icf_stream_sdp_info_st		*p_data = ICF_NULL;

	ICF_FUNCTION_ENTER(p_mmi_pdb->p_glb_pdb)

	while(ICF_NULL != p_src)
	{
		p_dest = p_mmi_pdb->p_call_ctx->p_stream_sdp_info;

		while(ICF_NULL != p_dest)
		{
			p_data = (icf_stream_sdp_info_st*)p_dest->p_payload;

			if(p_data->stream_id == 
						((icf_channel_addr_st *)p_src->p_data)->stream_id)
			{
				if(ICF_FAILURE == icf_mmi_validate_transport_addr(p_mmi_pdb, 
					&((icf_channel_addr_st*)p_src->p_data)->stream_address))
				{
#ifdef ICF_TRACE_ENABLE
                    p_mmi_pdb->result = ICF_FAILURE;
                    p_mmi_pdb->stream_type = 
                        ((icf_channel_addr_st*)p_src->p_data)->stream_type;
#endif
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
					p_dest = p_dest->p_next;

					continue;
				}

				ret_val = icf_mmi_check_sdp_in_call_ctx(p_mmi_pdb,
											p_data->stream_id,
											ICF_SDP_LOCAL);

				icf_port_memcpy(
					&p_data->p_local_sdp->sdp_stream.stream_address,
					&((icf_channel_addr_st *)p_src->p_data)->\
					stream_address,
					(icf_uint32_t)ICF_PORT_SIZEOF(icf_transport_address_st));

				ret_val = ICF_SUCCESS;
				break;
			}
			p_dest = p_dest->p_next;
		}
		p_src= p_src->p_next;
	}

	ICF_FUNCTION_EXIT(p_mmi_pdb->p_glb_pdb)
	return ret_val;
}
