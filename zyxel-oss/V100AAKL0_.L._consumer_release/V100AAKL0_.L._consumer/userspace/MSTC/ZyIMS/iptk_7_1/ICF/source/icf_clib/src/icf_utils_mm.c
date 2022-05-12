/*******************************************************************************
*										
* FILE NAME    :icf_util_mm.c
*										
* DESCRIPTION  :The file contains some utillity functions required by the encoder
*               and decoder functions of the library
*										
* Revision History :				
*				
*   DATE             NAME               REFERENCE              REASON
* -----------    ---------------      -------------      ------------------
* 05-JUL-2005     Raveen Sharma	       ICF REL 6.0		       Initial
* 11-Apr-2007   Kamini Gangwani        CSR1-4379017/SPR 14897
* 									
* 11-Apr-2007   Kamini Gangwani        CSR1-4379017/SPR 14897
*
* Copyright 2006, Aricent.   					
*										
*******************************************************************************/



#include "icf_clib_mm.h"
#include "icf_mm_api.h"
#include "icf_utils_mm.h"


#include "icf_clib.h"
#include "icf_api.h"
#include "icf_utils.h"

icf_return_t icf_free_mm_msg(icf_msg_st* p_msg)
{


    icf_return_t ret_val = ICF_SUCCESS;
	icf_uint32_t i = 0;

	switch (p_msg->hdr.api_id) 
	{

	case ICF_GET_CODEC_LIST_REQ:
        ICF_CLIB_TRACE(("\n[ICF_CLIB]: icf_free_mm_msg - ICF_GET_CODEC_LIST_REQ\n"));
		
		if(ICF_NULL != ((icf_get_codec_list_req_st*)p_msg->payload)->p_stream_capab_list)
		{
			icf_free_icf_stream_capab_list_st(((icf_get_codec_list_req_st*)p_msg->payload)->p_stream_capab_list);
		}
		if( ((icf_get_codec_list_req_st*)p_msg->payload)->bit_mask & ICF_LOCAL_STREAM_CAPAB_INFO_PRESENT)
                {
                        icf_free_icf_stream_capab_list_st(((icf_get_codec_list_req_st*)p_msg->payload)->p_local_stream_capab_list);
                }


		break;

	case ICF_GET_CODEC_LIST_RESP:
        ICF_CLIB_TRACE(("\n[ICF_CLIB]: icf_free_mm_msg - ICF_GET_CODEC_LIST_RESP\n"));

		if(ICF_NULL != ((icf_get_codec_list_resp_st*)p_msg->payload)->p_reserved_stream_list)
		{
			icf_free_icf_stream_capab_list_st(((icf_get_codec_list_resp_st*)p_msg->payload)->p_reserved_stream_list);
		}

		break;

	case ICF_CODEC_COMMIT_REQ:
        ICF_CLIB_TRACE(("\n[ICF_CLIB]: icf_free_mm_msg - ICF_CODEC_COMMIT_REQ\n"));
		if(ICF_NULL != ((icf_codec_commit_req_st*)p_msg->payload)->p_stream_commit_list)
			icf_free_icf_stream_capab_list_st(((icf_codec_commit_req_st*)p_msg->payload)->p_stream_commit_list);

		break;

	case ICF_CODEC_COMMIT_RESP:
        ICF_CLIB_TRACE(("\n[ICF_CLIB]: icf_free_mm_msg - ICF_CODEC_COMMIT_RESP\n"));

		if (((icf_codec_commit_resp_st*)p_msg->payload)->result)
			icf_free_list(((icf_codec_commit_resp_st*)p_msg->payload)->p_commited_stream_list);

		break;

	case ICF_CREATE_MEDIA_SESSION_REQ:
        ICF_CLIB_TRACE(("\n[ICF_CLIB]: icf_free_mm_msg - ICF_CREATE_MEDIA_SESSION_REQ\n"));
		if(ICF_NULL != ((icf_create_media_session_req_st*)p_msg->payload)->p_stream_list_data)
			icf_free_req_stream(((icf_create_media_session_req_st*)p_msg->payload)->p_stream_list_data);

		break;

	case ICF_CREATE_MEDIA_SESSION_RESP:
        ICF_CLIB_TRACE(("\n[ICF_CLIB]: icf_free_mm_msg - ICF_CREATE_MEDIA_SESSION_RESP\n"));

		if (((icf_create_media_session_resp_st*)p_msg->payload)->result)
			icf_free_resp_stream(((icf_create_media_session_resp_st*)p_msg->payload)->p_accepted_stream_list);

		break;

	case ICF_MODIFY_MEDIA_SESSION_REQ:
        ICF_CLIB_TRACE(("\n[ICF_CLIB]: icf_free_mm_msg - ICF_MODIFY_MEDIA_SESSION_REQ\n"));
		if(ICF_NULL != ((icf_modify_media_session_req_st*)p_msg->payload)->p_stream_data_list)
			icf_free_modify_stream(((icf_modify_media_session_req_st*)p_msg->payload)->p_stream_data_list);

		break;
	case ICF_MODIFY_MEDIA_SESSION_RESP:
        ICF_CLIB_TRACE(("\n[ICF_CLIB]: icf_free_mm_msg - ICF_MODIFY_MEDIA_SESSION_RESP\n"));

		if (((icf_modify_media_session_resp_st*)p_msg->payload)->result)
			icf_free_resp_stream(((icf_modify_media_session_resp_st*)p_msg->payload)->p_accepted_stream_list);

		break;

	case ICF_OPEN_MEDIA_CHANNEL_REQ:
        ICF_CLIB_TRACE(("\n[ICF_CLIB]: icf_free_mm_msg - ICF_OPEN_MEDIA_CHANNEL_REQ\n"));

		if (((icf_open_media_channel_req_st*)p_msg->payload)->p_channel_list)
			icf_free_list(((icf_open_media_channel_req_st*)p_msg->payload)->p_channel_list);

		break;

	case ICF_OPEN_MEDIA_CHANNEL_RESP:
        ICF_CLIB_TRACE(("\n[ICF_CLIB]: icf_free_mm_msg - ICF_OPEN_MEDIA_CHANNEL_RESP\n"));

		if (((icf_open_media_channel_resp_st*)p_msg->payload)->p_channel_addr_list)
			icf_free_list(((icf_open_media_channel_resp_st*)p_msg->payload)->p_channel_addr_list);

		break;

	case ICF_CLOSE_MEDIA_CHANNEL_REQ:
        ICF_CLIB_TRACE(("\n[ICF_CLIB]: icf_free_mm_msg - ICF_CLOSE_MEDIA_CHANNEL_REQ\n"));

		if (((icf_close_media_channel_req_st*)p_msg->payload)->p_channel_list)
			icf_free_list(((icf_close_media_channel_req_st*)p_msg->payload)->p_channel_list);
        
		break;

	case ICF_MEDIA_CAPABILITIES_RESP:
        ICF_CLIB_TRACE(("\n[ICF_CLIB]: icf_free_mm_msg - ICF_MEDIA_CAPABILITIES_RESP\n"));

		if (((icf_media_capabilities_resp_st*)p_msg->payload)->result)
			icf_free_resp_stream(((icf_media_capabilities_resp_st*)p_msg->payload)->p_resp_stream_list);

		break;

	case ICF_MEDIA_CAPABILITIES_REQ:
        ICF_CLIB_TRACE(("\n[ICF_CLIB]: icf_free_mm_msg - ICF_MEDIA_CAPABILITIES_REQ\n"));

		if (((icf_media_capabilities_req_st*)p_msg->payload)->bit_mask & ICF_MED_CAP_STREAM_PRSNT)
			icf_free_icf_stream_capab_list_st(((icf_media_capabilities_req_st*)p_msg->payload)->p_req_stream_list);

		break;

	case ICF_CFG_CODEC_INFO_REQ:
		 ICF_CLIB_TRACE(("\n[ICF_CLIB]: icf_free_mm_msg - ICF_CFG_CODEC_INFO_REQ\n"));
         
		 for(i=0;i<((icf_cfg_codec_info_req_st*)p_msg->payload)->count;i++)
         {
         if(((icf_cfg_codec_info_req_st*)p_msg->payload)->codec_supp[i].bitmask & ICF_ENCODED_PARAM_PRESENT)
                    icf_free_list(((icf_cfg_codec_info_req_st*)p_msg->payload)->codec_supp[i].p_encoded_params);
         }
		break;

    case ICF_RECV_UDP_DATA_IND:
        ICF_CLIB_MEMFREE(((icf_recv_udp_data_ind_st *)p_msg->payload)->p_app_data);
        break;
        /*
    case ICF_SEND_UDP_DATA_REQ:
        ICF_CLIB_MEMFREE(((icf_send_udp_data_req_st *)p_msg->payload)->p_app_data);
        break;
       */
	default:
		break;
	}

    if (ICF_MSG_FRM_PLTFM != p_msg->hdr.api_id)
        ICF_CLIB_MEMFREE(p_msg);

    return ret_val;
}

icf_void_t icf_free_list(icf_list_st* p_list)
{
	icf_list_st *p_next_node;

	for(;p_list != ICF_NULL; p_list = p_next_node)
	{
		p_next_node = p_list->p_next;
		ICF_CLIB_MEMFREE(p_list->p_data);
		ICF_CLIB_MEMFREE(p_list);
	}
}

icf_void_t	icf_free_modify_stream(icf_list_st *p_list)
{
	icf_list_st *p_next_node;

	for(;p_list != ICF_NULL; p_list = p_next_node)
	{
		p_next_node = p_list->p_next;
		icf_free_codec_attrib_st(((icf_modify_stream_st*)p_list->p_data)->sdp_stream.p_codec_info_list);
        if( ((icf_modify_stream_st*)p_list->p_data)->sdp_stream.bit_mask & ICF_SDP_STREAM_ENCPARAMS_PRESENT)
        {
		    icf_free_list(((icf_modify_stream_st*)p_list->p_data)->sdp_stream.p_encoded_attributes);
        }

		ICF_CLIB_MEMFREE(p_list->p_data);
		ICF_CLIB_MEMFREE(p_list);
	}
}

icf_void_t	icf_free_req_stream(icf_list_st *p_list)
{
	icf_list_st *p_next_node;

	for(;p_list != ICF_NULL; p_list = p_next_node)
	{
		p_next_node = p_list->p_next;
#if 1 /* ZyXEL , Jason*/
	if( ((icf_req_stream_st*)p_list->p_data)->bit_mask & ICF_CREATE_MEDIA_LOCAL_MEDIA_PROFILE_PRESENT)
#else
       if( ((icf_req_stream_st*)p_list->p_data)->bit_mask & ICF_MODIFY_LOCAL_SDP_STREAM_PRESENT)
#endif
        {
		    icf_free_codec_attrib_st(((icf_req_stream_st*)p_list->p_data)->local_sdp.p_codec_info_list);
#if 1 /* ZyXEL , Jason*/            
            if( ((icf_req_stream_st*)p_list->p_data)->local_sdp.bit_mask & ICF_SDP_STREAM_ENCPARAMS_PRESENT)
#else
            if( ((icf_req_stream_st*)p_list->p_data)->remote_sdp.bit_mask & ICF_SDP_STREAM_ENCPARAMS_PRESENT)
#endif
            {

		        icf_free_list(((icf_req_stream_st*)p_list->p_data)->local_sdp.p_encoded_attributes);
            }
        }
#if 1 /* ZyXEL , Jason*/    
	if( ((icf_req_stream_st*)p_list->p_data)->bit_mask & ICF_CREATE_MEDIA_REMOTE_SDP_PRESENT)
#else
        if( ((icf_req_stream_st*)p_list->p_data)->bit_mask & ICF_MODIFY_REMOTE_SDP_STREAM_PRESENT)
#endif
        {
		    icf_free_codec_attrib_st(((icf_req_stream_st*)p_list->p_data)->remote_sdp.p_codec_info_list);
            if( ((icf_req_stream_st*)p_list->p_data)->remote_sdp.bit_mask & ICF_SDP_STREAM_ENCPARAMS_PRESENT)
            {
		        icf_free_list(((icf_req_stream_st*)p_list->p_data)->remote_sdp.p_encoded_attributes); 
            }
        }

		ICF_CLIB_MEMFREE(p_list->p_data);
		ICF_CLIB_MEMFREE(p_list);
	}
}

icf_void_t icf_free_resp_stream(icf_list_st *p_list)
{
	icf_list_st *p_next_node;

	for(;p_list != ICF_NULL; p_list = p_next_node)
	{
		p_next_node = p_list->p_next;
		icf_free_codec_attrib_st(((icf_resp_stream_st*)p_list->p_data)->local_sdp.p_codec_info_list);
        if( ((icf_resp_stream_st*)p_list->p_data)->local_sdp.bit_mask & ICF_SDP_STREAM_ENCPARAMS_PRESENT)
        {
		    icf_free_list(((icf_resp_stream_st*)p_list->p_data)->local_sdp.p_encoded_attributes);
        }

		ICF_CLIB_MEMFREE(p_list->p_data);
		ICF_CLIB_MEMFREE(p_list);
	}
}





