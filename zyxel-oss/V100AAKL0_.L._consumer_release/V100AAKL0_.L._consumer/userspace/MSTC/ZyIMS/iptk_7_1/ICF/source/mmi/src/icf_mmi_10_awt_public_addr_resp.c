/******************************************************************************
 * 
 * FILE NAME:   icf_mmi_10_awt_public_addr_resp.c
 * 
 * DESCRIPTION: This file contains functions to handle events in the AWT PUBLIC 
 *              ADDR response state in MMI.
 *              
 * REVISION HISTORY:    
 *   Date            Name          Reference       Comments
 * 25-july-2006     Puneet       ICF Rel 6.0    Initial Version- support for
 * 												multiple m-lines
 * 13-Mar-2007      Deepti Goyal    SPR 14578   If failure for one stream then 
 *                                  SPR 14581   proceed with next stream.
 *
 * 16-Mar-2007      Deepti Goyal    SPR 14574   CHanges for multiple calls
 *                                  SPR 14564
 *                                  SPR 14577   
 * Copyright (c) 2007, Aricent.  
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


#ifdef ICF_NAT_MEDIA_TRAVERSAL
/******************************************************************************
 *
 * FUNCTION:        icf_mmi_10_dms_request
 * 
 * DESCRIPTION:     This function is invoked when the MMI is
 *                  awaiting NAT_PUBLIC_IP_PORT_RESP from MM and it receives DMS req 
 *                  from CC. 
 *                                    
 * RETURNS:         ICF_SUCCESS in case of success and ICF_FAILURE in 
 *                  case of any failure.                  
 *
 *****************************************************************************/

icf_return_t icf_mmi_10_dms_request(
        INOUT icf_mmi_pdb_st             *p_mmi_pdb)
{
    icf_return_t             ret_val = ICF_SUCCESS;

	ret_val = icf_mmi_01_dms_request(p_mmi_pdb);
    
    return ret_val;
} 


/******************************************************************************
 *
 * FUNCTION:        icf_mmi_10_nat_public_ip_port_resp
 * 
 * DESCRIPTION:     This function is invoked when the MMI is
 *                  awaiting NAT_PUBLIC_IP_PORT_RESP from MM and it receives it
 *                                    
 * RETURNS:         ICF_SUCCESS in case of success and ICF_FAILURE in 
 *                  case of any failure.                  
 *
 *****************************************************************************/
icf_return_t icf_mmi_10_nat_public_ip_port_resp(
        INOUT icf_mmi_pdb_st             *p_mmi_pdb)
{
    icf_return_t				        ret_val = ICF_SUCCESS;
	icf_stream_id_t			        	stream_id = ICF_NULL;
	icf_call_id_t			        	call_id = ICF_NULL;
    icf_uint8_t                         req_type = ICF_NULL;
    nat_msg_st                          *p_nat_msg = ICF_NULL;
    nat_get_public_ip_port_resp_st      *p_recv_msg = ICF_NULL;
    nat_msg_hdr_st                      *p_nat_hdr = ICF_NULL;
    icf_transport_address_st            public_addr;
    icf_media_type_t                    stream_type = ICF_NULL;
    icf_internal_sdp_stream_st          *p_int_sdp = ICF_NULL;

    p_nat_msg = (nat_msg_st*)(p_mmi_pdb->p_glb_pdb->p_recv_msg->payload);
    p_nat_hdr = (nat_msg_hdr_st *)&(p_nat_msg->msg_hdr);
	p_recv_msg = (nat_get_public_ip_port_resp_st *)\
                 (p_nat_msg->p_msg_data);

    call_id = icf_cmn_fetch_call_id_from_nat_context_id(p_nat_hdr->app_context_id);
	
	if(call_id != p_mmi_pdb->p_call_ctx->call_id)
	{
		return ret_val;
	}

	stream_id = icf_cmn_fetch_stream_id_from_nat_context_id(p_nat_hdr->app_context_id);
    req_type = icf_cmn_fetch_req_type_from_nat_context_id(p_nat_hdr->app_context_id);
    stream_type = icf_mmi_get_stream_type_for_stream_id(p_mmi_pdb,stream_id);


	if(NAT_RESULT_FAILURE == p_recv_msg->result)
	{
		/* Send REL_RES_IND for this stream */
		icf_mmi_send_rel_res_ind(p_mmi_pdb);

		/* Send CLOSE_CHANNEL_IND for this stream */
		icf_mmi_send_close_channel_ind(p_mmi_pdb);

		/* Set NAT COMPLETE flag to true for the failed stream */

        p_mmi_pdb->p_mmi_ctx->nat_streams[p_mmi_pdb->\

						p_mmi_ctx->nat_stream_count].is_nat_complete = ICF_TRUE;

        (p_mmi_pdb->p_mmi_ctx->nat_stream_count)--;

		if(ICF_MMI_CMS_ONGOING == p_mmi_pdb->p_mmi_ctx->ongoing_event)
		{
			icf_mmi_mark_failure_stream_cms_resp(p_mmi_pdb,stream_id);
		}
		else if(ICF_MMI_MMS_ADD_ONGOING == p_mmi_pdb->p_mmi_ctx->ongoing_event)
		{
			icf_mmi_mark_failure_stream_mms_add_resp(p_mmi_pdb,stream_id);
			/* send MM_resp to CC */

			ret_val = icf_mmi_send_modify_media_response(p_mmi_pdb);
			return ret_val;
		}
	}
	else
	{
		/* copy nat_ip_port_address_st to local SDP */
        /*ret_val = icf_mmi_get_sdp_for_stream_id(p_mmi_pdb,stream_id,\
                ICF_SDP_LOCAL,&sdp);*/
        ret_val = icf_cmn_fetch_sdp_in_list_for_stream_id(
                p_mmi_pdb->p_glb_pdb, stream_id, ICF_SDP_LOCAL, &p_int_sdp);
        if(ICF_SUCCESS == ret_val)
        {
            p_mmi_pdb->p_mmi_ctx->context_info[\
                p_mmi_pdb->p_mmi_ctx->no_of_nat_ctxt].stream_id = stream_id;
            p_mmi_pdb->p_mmi_ctx->context_info[\
                p_mmi_pdb->p_mmi_ctx->no_of_nat_ctxt].context_id = \
                p_nat_hdr->app_context_id;
            (p_mmi_pdb->p_mmi_ctx->no_of_nat_ctxt)++;

            icf_mmi_convert_nat_addr_to_trans_addr(&(p_recv_msg->public_addr),\
                    &public_addr);
            if(ICF_MEDIA_REQ_TYPE_STREAM == req_type)
            {
                /* Fill this address in the stream address field
                 */
                icf_port_memcpy(&(p_int_sdp->sdp_stream.stream_address),\
                        &public_addr,(icf_uint32_t)\
                        ICF_PORT_SIZEOF(icf_transport_address_st));
            }
            else
            {
                /* Fill this address in the stream address field
                 */
                icf_port_memcpy(&(p_int_sdp->sdp_stream.rtcp_address),\
                        &public_addr,(icf_uint32_t)\
                        ICF_PORT_SIZEOF(icf_transport_address_st));
                p_int_sdp->sdp_stream.bit_mask |= ICF_SDP_STREAM_RTCP_INFO_PRESENT;
            }
        }

	}

	if(ICF_NULL == p_mmi_pdb->p_mmi_ctx->nat_stream_count)
	{
		/* Doubt: for both ? */
		ret_val = icf_mmi_send_cms_req(p_mmi_pdb);
		
		if(ICF_SUCCESS == ret_val)
		{
			ICF_MMI_SET_STATE(p_mmi_pdb, ICF_MMI_02_AWT_CMS_RESP)
		}
	}
	else
	{
		if(ICF_SUCCESS == ret_val)
		{
			if(ICF_NULL != p_mmi_pdb->p_mmi_ctx->nat_stream_count)
            {
                p_mmi_pdb->p_mmi_ctx->nat_streams[(p_mmi_pdb->p_mmi_ctx->\
                        nat_stream_count)-1].count--;
                while(ICF_NULL != p_mmi_pdb->p_mmi_ctx->nat_stream_count)
                {
                    icf_stream_id_t         temp_stream_id = \
                                  p_mmi_pdb->p_mmi_ctx->nat_streams[\
                      (p_mmi_pdb->p_mmi_ctx->nat_stream_count)-1].stream_id;

                    if(stream_id != temp_stream_id)
                    {
                        /* Get the SDP for the stream id
                         */
                        ret_val = icf_cmn_fetch_sdp_in_list_for_stream_id(\
                                p_mmi_pdb->p_glb_pdb,temp_stream_id,\
                                ICF_SDP_LOCAL,&p_int_sdp);
                        if(ICF_SUCCESS != ret_val)
                        {
                            ICF_PRINT(("\n Failed to get SDP for stream id %d",\
                                        temp_stream_id));
                        }
                        else
                        {
                            /* Get the stream type for the stream id
                             */
                          stream_type = icf_mmi_get_stream_type_for_stream_id(p_mmi_pdb,\
                                    temp_stream_id);
                        }
                    }
                    switch(stream_type)
                    {
                        case ICF_STREAM_TYPE_AUDIO:
                        case ICF_STREAM_TYPE_VIDEO:
                            if(ICF_NULL != p_mmi_pdb->p_mmi_ctx->nat_streams\
                                    [(p_mmi_pdb->p_mmi_ctx->nat_stream_count) - 1].count)
                            {
                                if(ICF_MEDIA_REQ_TYPE_STREAM == req_type &&\
                                      1 == p_mmi_pdb->p_mmi_ctx->nat_streams\
                                   [(p_mmi_pdb->p_mmi_ctx->nat_stream_count) - 1].\
                                   count )
                                {
                                    req_type = ICF_MEDIA_REQ_TYPE_RTCP;
                                }
                                else
                                {
                                    req_type = ICF_MEDIA_REQ_TYPE_STREAM;
                                }
                                break;
                            }
                            else
                            {
                                /* Decrement the stream count for nat streams
                                 */
                                p_mmi_pdb->p_mmi_ctx->nat_streams[p_mmi_pdb->\
                                    p_mmi_ctx->nat_stream_count].\
                                    is_nat_complete = ICF_TRUE;
                                (p_mmi_pdb->p_mmi_ctx->nat_stream_count)--;
                                continue;
                            }
                        case ICF_STREAM_TYPE_TBCP:
                        case ICF_STREAM_TYPE_FAX:
                            if(ICF_NULL != p_mmi_pdb->p_mmi_ctx->nat_streams\
                                    [(p_mmi_pdb->p_mmi_ctx->nat_stream_count)\
                                    - 1].count)
                            {
                                req_type = ICF_MEDIA_REQ_TYPE_STREAM;
                                break;
                            }
                            else
                            {
                                p_mmi_pdb->p_mmi_ctx->nat_streams[p_mmi_pdb->\
                                    p_mmi_ctx->nat_stream_count].\
                                    is_nat_complete = ICF_TRUE;
                                (p_mmi_pdb->p_mmi_ctx->nat_stream_count)--;
                                continue;
                            }
                        default:
                            ICF_PRINT(("\n Invalid Stream Type for stream id %d",\
                                        stream_id));
                    }
                    if(ICF_FALSE == p_mmi_pdb->p_mmi_ctx->nat_streams[p_mmi_pdb->\
                                    p_mmi_ctx->nat_stream_count-1].\
                                    is_nat_complete)
                    {
		                ret_val = icf_mmi_send_get_public_ip_port_req(\
                            p_mmi_pdb,p_mmi_pdb->p_mmi_ctx->\
                            nat_streams[(p_mmi_pdb->p_mmi_ctx->\
                            nat_stream_count) - 1].stream_id,\
                            req_type);
					break;
                    }
                    else

                    {

                        (p_mmi_pdb->p_mmi_ctx->nat_stream_count)--;

                    }
                }/* End of while loop*/

                if(ICF_NULL == p_mmi_pdb->p_mmi_ctx->nat_stream_count)
                {
                    p_mmi_pdb->p_mmi_ctx->nat_stream_count = \
                                            p_mmi_pdb->p_mmi_ctx->restore_count;
                    /* Send CMS req for success streams in \
                     * cms_resp/mms_resp in MMI ctxt
                     */
                    ret_val = icf_mmi_send_cms_req(p_mmi_pdb);

                    if(ICF_SUCCESS == ret_val)
                    {
                        ICF_MMI_SET_STATE(p_mmi_pdb,ICF_MMI_02_AWT_CMS_RESP)
                    }
                }
                else
                {
                    ICF_MMI_SET_STATE(p_mmi_pdb,ICF_MMI_10_AWT_NAT_PUBLIC_IP_PORT_RESP)
                }
            }/* End of stream count check if condition */
        }
    }
    return ret_val;
}


#endif /* #ifdef ICF_NAT_MEDIA_TRAVERSAL */


