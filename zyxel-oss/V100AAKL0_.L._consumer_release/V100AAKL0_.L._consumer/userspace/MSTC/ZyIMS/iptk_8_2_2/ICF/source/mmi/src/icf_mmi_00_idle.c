/******************************************************************************
 * 
 * FILE NAME:   icf_mmi_00_idle.c
 * 
 * DESCRIPTION: This file contains functions to handle events in the IDLE state
 * 		in MMI.  
 *              
 * REVISION HISTORY:    
 *   Date            Name          Reference       Comments
 * 25-july-2006  	Puneet       ICF Rel 6.0    Initial Version- support for
 * 												multiple m-lines
 * 4-Apr-2008   Sumant Gupta    ICF 8.0         Rel7.1 Windows porting Merging 						
 * 09-Apr-2008  Alok Tiwari     SPR-18362       Klocwork warning removal 
 * 09-Feb-2009  Tarun Gupta     Rel 8.1 LLD     SDP Tunnelling Enhancements
 * 04-Mar-2009 Saurabh Gupta     SPR 19349      Klocworks fix
 * Copyright (c) 2009, Aricent .  
 ******************************************************************************/

/* include files */
#include "icf_internal_api_id.h"
#include "icf_mmi_defs.h"
#include "icf_mmi_types.h"
#include "icf_mmi_macro.h"
#include "icf_mmi_prototypes.h"
#include "icf_dbm_prototypes.h"
#include "icf_ecode.h"
#include "icf_common_types.h"

#ifdef ICF_PORT_WINDOWS
#pragma warning ( disable : 4702 )
#endif


/******************************************************************************
 *
 * FUNCTION:        icf_mmi_00_cms_request
 *
 * DESCRIPTION:     This function is invoked when the MMI is in
 *                  in IDLE state and it receives CMS request from CC
 *                  Current State: ICF_MMI_00_IDLE
 *                  Current Event: ICF_MMI_CMS_REQ
 *                  
 * RETURNS:         ICF_SUCCESS in case of success and ICF_FAILURE in 
 *                  case of any failure.                  
 *
 *****************************************************************************/

icf_return_t icf_mmi_00_cms_request(
        INOUT icf_mmi_pdb_st            *p_mmi_pdb)
{
    icf_return_t                        ret_val = ICF_SUCCESS;
    icf_stream_id_t			stream_count = ICF_NULL;
    icf_internal_list_st		*p_src = ICF_NULL;
    icf_mmi_internal_cms_resp_st	*p_cms_resp = ICF_NULL;
    
    ICF_FUNCTION_ENTER(p_mmi_pdb->p_glb_pdb)

    
    /* CMS request is recvd from CC.MMI will send GCL request to RM with 
     * remote media codec list if available. Incase of non-fast start call, 
     * ICF will send request for a single audio stream to MM in the GCL request
     * MM will return the default parameters for a single audio call.
     */
    
    /* Make cms_resp in MMI ctxt. initilise all active streams as success */
    /* MEMGET for p_cms_resp as type icf_mmi_internal_cms_resp_st */
    ICF_MEMGET (p_mmi_pdb->p_glb_pdb,
       	ICF_PORT_SIZEOF(icf_mmi_internal_cms_resp_st),
		ICF_MEM_COMMON,
		p_cms_resp,
		ICF_RET_ON_FAILURE,
		p_mmi_pdb->p_ecode,
		ret_val)

    if(ICF_FAILURE == ret_val)
    {
        ICF_ALARM(((icf_uint8_t *)"[MMI]: MEMGET failure"));
		return ret_val;
    }
	p_cms_resp->result = ICF_SUCCESS;
	p_src = p_mmi_pdb->p_call_ctx->p_stream_sdp_info;

	while((ICF_NULL != p_src) && (stream_count <ICF_MAX_STREAM_PER_CALL)) 
	{
		if(ICF_MEDIA_STREAM_ACTIVE &
			((icf_stream_sdp_info_st *)p_src->p_payload)->stream_status)
		{
			p_cms_resp->stream_status[stream_count].stream_id = 
				((icf_stream_sdp_info_st *)p_src->p_payload)->stream_id;
			
			p_cms_resp->stream_status[stream_count].stream_type = 
				((icf_stream_sdp_info_st *)p_src->p_payload)->stream_type;

			p_cms_resp->stream_status[stream_count].status_success = ICF_TRUE;
			stream_count++;
		}
		p_src = p_src->p_next;
	}

	p_cms_resp->count = stream_count;

	p_mmi_pdb->p_mmi_ctx->p_mmi_resp = p_cms_resp;
	
	/* set CMS ongoing */
	p_mmi_pdb->p_mmi_ctx->ongoing_event = ICF_MMI_CMS_ONGOING;

    ret_val = icf_mmi_send_gcl_req(p_mmi_pdb);

    if(ICF_SUCCESS == ret_val)
    {
        ICF_MMI_SET_STATE(p_mmi_pdb,ICF_MMI_01_AWT_GCL_RESP)
        
        /* Delete is not complete now so reset the bit. This bit will indicate 
         * that if DMS req is recieved from CC then is there any need to send 
         * DMS req or the delete is already complete
         */

        ICF_MMI_RESET_BIT(p_mmi_pdb,ICF_DELETE_COMPLETE)

        ICF_MMI_TRACE(p_mmi_pdb,ICF_TRACE_API_SEND)            
    }
	else
	{
		icf_mmi_clear_buffered_req_resp_in_mmi_ctx(p_mmi_pdb);
	}
    
    ICF_FUNCTION_EXIT(p_mmi_pdb->p_glb_pdb)
    return ret_val;
}

/******************************************************************************
 *
 * FUNCTION:        icf_mmi_00_mms_request
 *
 * DESCRIPTION:     This function is invoked when the MMI is in
 *                  in IDLE state and it receives MMS request from CC
 *                  Current State: ICF_MMI_00_IDLE
 *                  
 * RETURNS:        ICF_SUCCESS in case of success and ICF_FAILURE in 
 *                  case of any failure.                   
 *
 ******************************************************************************/
icf_return_t icf_mmi_00_mms_request(
        INOUT icf_mmi_pdb_st            *p_mmi_pdb)
{
    icf_return_t                        ret_val = ICF_FAILURE,
										dummy_ret_val = ICF_SUCCESS;
    icf_mmi_mms_request_st              *p_msg_data_recd = ICF_NULL,
										*p_mms_req = ICF_NULL;
	icf_mmi_internal_mms_resp_st		*p_mms_resp = ICF_NULL;
	icf_stream_id_t						stream_count = ICF_NULL;
    
    ICF_FUNCTION_ENTER(p_mmi_pdb->p_glb_pdb)
        
    p_msg_data_recd = (icf_mmi_mms_request_st *)(p_mmi_pdb->
            p_internal_msg->p_msg_data);

	/* Save request in MMI ctxt */

	/* MEMGET for p_mmi_pdb->p_mmi_ctx->p_mms_req as type 
	 * icf_mmi_mms_request_st */
	ICF_MEMGET (p_mmi_pdb->p_glb_pdb,
				ICF_PORT_SIZEOF(icf_mmi_mms_request_st),
				ICF_MEM_COMMON,
				p_mms_req,
				ICF_RET_ON_FAILURE,
				p_mmi_pdb->p_ecode,
				dummy_ret_val)

	if(ICF_FAILURE == dummy_ret_val)
	{
		ICF_ALARM(((icf_uint8_t *)"[MMI]: MEMGET failure"));
		return dummy_ret_val;
	}

	if (ICF_NULL != p_mms_req)
	{
		icf_port_memcpy((icf_void_t *)p_mms_req, 
					(icf_void_t *)p_msg_data_recd,
					(icf_uint32_t)ICF_PORT_SIZEOF(icf_mmi_mms_request_st));

		if(ICF_NULL != p_msg_data_recd->p_add_media)
		{
			ICF_MEMGET (p_mmi_pdb->p_glb_pdb,
						ICF_PORT_SIZEOF(icf_stream_id_list_st),
						ICF_MEM_COMMON,
						p_mms_req->p_add_media,
						ICF_DONOT_RET_ON_FAILURE,
						p_mmi_pdb->p_ecode,
						dummy_ret_val)

			if(ICF_FAILURE == dummy_ret_val)
			{
				ICF_ALARM(((icf_uint8_t *)"[MMI]: MEMGET failure"));
                                ICF_MEMFREE(p_mmi_pdb->p_glb_pdb,
				            p_mms_req,
				            ICF_MEM_COMMON,
                                            p_mmi_pdb->p_ecode,
				            dummy_ret_val) 

				return ICF_FAILURE;
			}

			icf_port_memcpy((icf_void_t *)p_mms_req->p_add_media,
					(icf_void_t *)p_msg_data_recd->p_add_media,
					(icf_uint32_t)ICF_PORT_SIZEOF(icf_stream_id_list_st));
#ifndef ICF_UNIT_TEST
			/* Free memory */
			ICF_MEMFREE(p_mmi_pdb->p_glb_pdb,p_msg_data_recd->p_add_media,
				ICF_MEM_COMMON, p_mmi_pdb->p_ecode,ret_val)
#endif

		}
		if(ICF_NULL != p_msg_data_recd->p_delete_media)
		{
			ICF_MEMGET (p_mmi_pdb->p_glb_pdb,
						ICF_PORT_SIZEOF(icf_stream_id_list_st),
						ICF_MEM_COMMON,
						p_mms_req->p_delete_media,
						ICF_DONOT_RET_ON_FAILURE,
						p_mmi_pdb->p_ecode,
						dummy_ret_val)

			if(ICF_FAILURE == dummy_ret_val)
			{
				ICF_ALARM(((icf_uint8_t *)"[MMI]: MEMGET failure"));
                /*Klocwork warning removal*/               
                ICF_MEMFREE(p_mmi_pdb->p_glb_pdb,
				            p_mms_req->p_add_media,
				            ICF_MEM_COMMON,
                            p_mmi_pdb->p_ecode,
				            dummy_ret_val)

                ICF_MEMFREE(p_mmi_pdb->p_glb_pdb,
				            p_mms_req,
				            ICF_MEM_COMMON,
                            p_mmi_pdb->p_ecode,
				            dummy_ret_val)
				return ICF_FAILURE;
			}

			icf_port_memcpy(
				(icf_void_t *)p_mms_req->p_delete_media,
				(icf_void_t *)p_msg_data_recd->p_delete_media,
				(icf_uint32_t)ICF_PORT_SIZEOF(icf_stream_id_list_st));
#ifndef ICF_UNIT_TEST
			/* Free memory */
			ICF_MEMFREE(p_mmi_pdb->p_glb_pdb,p_msg_data_recd->p_delete_media,
				ICF_MEM_COMMON,	p_mmi_pdb->p_ecode,ret_val)
#endif
		}
	}

	p_mmi_pdb->p_mmi_ctx->p_mms_req = p_mms_req;

	/* Initialise mms_resp in MMI ctxt with all streams as in request as 
	 * success */

	/* MEMGET for p_mms_resp as type icf_mmi_internal_mms_resp_st */
	ICF_MEMGET (p_mmi_pdb->p_glb_pdb,
				ICF_PORT_SIZEOF(icf_mmi_internal_mms_resp_st),
				ICF_MEM_COMMON,
				p_mms_resp,
				ICF_RET_ON_FAILURE,
				p_mmi_pdb->p_ecode,
				dummy_ret_val)

	if(ICF_FAILURE == dummy_ret_val)
	{
		ICF_ALARM(((icf_uint8_t *)"[MMI]: MEMGET failure"));
    
		ICF_MEMFREE(p_mmi_pdb->p_glb_pdb,
				p_mmi_pdb->p_mmi_ctx->p_mms_req,
				ICF_MEM_COMMON,
                p_mmi_pdb->p_ecode,
				dummy_ret_val)

		return ICF_FAILURE;
	}
	p_mms_resp->result = ICF_SUCCESS;
	p_mms_resp->bit_mask = ICF_NULL;


	/* we do not need to save response for delete action  so 
	 * ICF_MMI_MMS_ACTION_DELETE is not checked */

	if(p_msg_data_recd->bit_mask & ICF_MMI_MMS_ACTION_MODIFY)
	{
		p_mms_resp->mod_stream_count = p_msg_data_recd->modify_stream_count;
		p_mms_resp->bit_mask |= ICF_MMI_MMS_ACTION_MODIFY_RESP;

		for(stream_count = 0; stream_count < p_mms_resp->mod_stream_count &&
						stream_count < ICF_MAX_STREAM_PER_CALL;
			stream_count++)
		{
			p_mms_resp->modified_stream_resp[stream_count].bitmask =
				p_msg_data_recd->modify_media[stream_count].bitmask;

			p_mms_resp->modified_stream_resp[stream_count].stream_id =
				p_msg_data_recd->modify_media[stream_count].stream_id;

			p_mms_resp->modified_stream_resp[stream_count].stream_type =
				p_msg_data_recd->modify_media[stream_count].stream_type;

			p_mms_resp->modified_stream_resp[stream_count].status_success =
				ICF_TRUE;

			p_mms_resp->modified_stream_resp[stream_count].send_mms =
				ICF_TRUE;
		}
	}
	else
	{
		p_mms_resp->mod_stream_count = ICF_NULL;
	}
	if (p_msg_data_recd->bit_mask & ICF_MMI_MMS_ACTION_ADD)
	{
		p_mms_resp->add_stream_count = p_mms_req->p_add_media->count;
		p_mms_resp->bit_mask |= ICF_MMI_MMS_ACTION_ADD_RESP;

		for(stream_count = 0; stream_count < p_mms_resp->add_stream_count &&
						stream_count <ICF_MAX_STREAM_PER_CALL;
			stream_count++)
		{
			p_mms_resp->added_stream_resp[stream_count].stream_id =
				p_mms_req->p_add_media->stream_id[stream_count];

			p_mms_resp->added_stream_resp[stream_count].stream_type =
				icf_mmi_get_stream_type_for_stream_id(p_mmi_pdb,
					p_mms_resp->added_stream_resp[stream_count].stream_id);

			p_mms_resp->added_stream_resp[stream_count].status_success =
				ICF_TRUE;
		}
	}
	else
	{
		p_mms_resp->add_stream_count = ICF_NULL;
	}
   
	p_mmi_pdb->p_mmi_ctx->p_mmi_resp = p_mms_resp;


#ifdef ICF_TRACE_ENABLE
	icf_mmi_trace_internal_sdp(p_mmi_pdb, ICF_SDP_LOCAL);
	icf_mmi_trace_internal_sdp(p_mmi_pdb, ICF_SDP_REMOTE);
	icf_mmi_trace_internal_sdp(p_mmi_pdb, ICF_SDP_APPLIED);
	icf_mmi_trace_internal_sdp(p_mmi_pdb, ICF_SDP_LAST_COMMITED);
#endif

	if(p_msg_data_recd->bit_mask & ICF_MMI_MMS_ACTION_DELETE)
	{
		ret_val = icf_mmi_process_mms_delete_action(p_mmi_pdb);
	}
	else if(p_msg_data_recd->bit_mask & ICF_MMI_MMS_ACTION_MODIFY)
	{
		ret_val = icf_mmi_process_mms_modify_action(p_mmi_pdb);
	}
	/* The case with ICF_MMI_MMS_ACTION_ADD is not applicable in the current
	 * release of ICF6.0 as we shall never add a stream mid call except T.38 
	 * and for adding this stream it has to be coupled with other action of 
	 * DELETE or MODIFY if it was then trigger 
	 * icf_process_mms_add_action(p_mmi_pdb) */
	else if(p_msg_data_recd->bit_mask & ICF_MMI_MMS_ACTION_ADD)
	{
		p_mmi_pdb->p_mmi_ctx->ongoing_event = ICF_MMI_MMS_ADD_ONGOING;

		ret_val = icf_mmi_send_gcl_req(p_mmi_pdb);

		if(ICF_SUCCESS == ret_val)
		{
			ICF_MMI_SET_STATE(p_mmi_pdb,ICF_MMI_01_AWT_GCL_RESP)
		}
	}

	if(ICF_FAILURE == ret_val)
	{
		icf_mmi_clear_buffered_req_resp_in_mmi_ctx(p_mmi_pdb);
	}

    ICF_FUNCTION_EXIT(p_mmi_pdb->p_glb_pdb)
    return ret_val;        
}/* End function */   

/******************************************************************************
 *
 * FUNCTION:        icf_mmi_00_dms_request
 *
 * DESCRIPTION:     This function is invoked when the MMI is in
 *                  in IDLE state and it receives a DMS request
 *                  Current State: ICF_MMI_00_IDLE
 *                  Current Event: ICF_MMI_DMS_REQ
 *                  
 * RETURNS:        ICF_SUCCESS in case of success and ICF_FAILURE in 
 *                  case of any failure.                    
 *
 ******************************************************************************/

icf_return_t icf_mmi_00_dms_request(
        INOUT icf_mmi_pdb_st             *p_mmi_pdb)
{
    icf_return_t                 ret_val = ICF_SUCCESS;
    icf_mmi_dms_request_st       *p_msg_data_recd = ICF_NULL;
    
    ICF_FUNCTION_ENTER(p_mmi_pdb->p_glb_pdb)

    p_msg_data_recd = (icf_mmi_dms_request_st *)
        (p_mmi_pdb->p_internal_msg->p_msg_data);

    /* Check if ICF_DELETE_COMPLETE is not set. DMS request shall be sent only 
	 * if delete complete is not set.Else there will be no need to send DMS 
	 * request */
    if(!(ICF_DELETE_COMPLETE & p_mmi_pdb->p_mmi_ctx->mmi_bitmask))
    {
        if(ICF_FALSE == p_msg_data_recd->forceful_delete)
        {
            ICF_MMI_TRACE(p_mmi_pdb,ICF_TRACE_DMS_NOT_FORCEFUL)

			icf_mmi_send_awaited_dms_req(p_mmi_pdb);
        }
        /* If this is a forceful delete */
        else
        {
            ICF_MMI_TRACE(p_mmi_pdb,ICF_TRACE_FORCEFUL_DMS)

			icf_mmi_send_forceful_dms_req(p_mmi_pdb);
			/* No need to send DMS response for forceful delete */
        }
    }/* End if to check delete complete */
    else
    {
        /* No media session exists. Set Error code which would be send to cc */
        ICF_MMI_SET_ECODE(p_mmi_pdb,ICF_ECODE_NO_MMI_CONTEXT_FOUND)
        ret_val = ICF_FAILURE;
    }
    
    ICF_FUNCTION_EXIT(p_mmi_pdb->p_glb_pdb)
    return ret_val;
}/* End function */    


/******************************************************************************
 *
 * FUNCTION:        icf_mmi_00_merge_request
 * 
 * DESCRIPTION:     This function is invoked when the MMI is in IDLE state and
 *                  it receives merge req from SC.
 *                                    
 * RETURNS:         ICF_SUCCESS in case of success and ICF_FAILURE in 
 *                  case of any failure.                  
 *
 *****************************************************************************/

icf_return_t icf_mmi_00_merge_request(
        INOUT icf_mmi_pdb_st         *p_mmi_pdb)
{
    icf_return_t                     ret_val = ICF_SUCCESS;
    icf_mmi_merge_request_st         *p_msg_data_recd = ICF_NULL;
    icf_call_ctx_st                  *p_dbm_call_ctx = ICF_NULL;
    
    ICF_FUNCTION_ENTER(p_mmi_pdb->p_glb_pdb)
    
    ret_val = icf_mmi_send_merge_req(p_mmi_pdb);

    if(ICF_SUCCESS == ret_val)
    {
        ICF_MMI_SET_STATE(p_mmi_pdb,
                ICF_MMI_07_AWT_MERGE_RESP)

        ICF_MMI_TRACE(p_mmi_pdb,ICF_TRACE_API_SEND)
        
        p_msg_data_recd = (icf_mmi_merge_request_st *)(p_mmi_pdb->
                p_internal_msg->p_msg_data);
        
        /* Store the call id 2 in mmi ctx */
        p_mmi_pdb->p_mmi_ctx->merge_call_id = p_msg_data_recd->merge_call_id_2;
        
        /* Get Call context pointer from DBM for Call id in the request */
        ret_val = icf_dbm_find_call_cntxt_blk(
                p_mmi_pdb->p_glb_pdb,
                p_msg_data_recd->merge_call_id_2,
                &(p_dbm_call_ctx),
                p_mmi_pdb->p_ecode);
        
        if((ICF_SUCCESS == ret_val) && (p_msg_data_recd->merge_call_id_2 == 
                    p_dbm_call_ctx->call_id))
        {
            p_dbm_call_ctx->mmi_context.merge_call_id  = 
                p_mmi_pdb->p_call_ctx->call_id;
            p_dbm_call_ctx->mmi_context.mmi_state = 
                ICF_MMI_07_AWT_MERGE_RESP;
        }
        else
        {
            /* Failure by DBM */
            ICF_MMI_TRACE(p_mmi_pdb,ICF_TRACE_FAILURE_BY_DBM)
        }
    }/* End if */
    
    ICF_FUNCTION_EXIT(p_mmi_pdb->p_glb_pdb)
    return ret_val;       
}/* End function */

/******************************************************************************
 *
 * FUNCTION:        icf_mmi_00_tunnelled_cms_request
 *
 * DESCRIPTION:     This function is invoked when the MMI is in
 *                  in IDLE state and it receives a CMS request from CC.
 *                  Current State: ICF_MMI_00_IDLE
 *                  Current Event: ICF_MMI_TUNNELLED_CMS_REQ
 *                  
 * RETURNS:         ICF_SUCCESS in case of success and ICF_FAILURE in 
 *                  case of any failure.                  
 *
 *****************************************************************************/
icf_return_t icf_mmi_00_tunnelled_cms_request(
    icf_mmi_pdb_st			*p_mmi_pdb)
{
    icf_return_t                    ret_val = ICF_SUCCESS;
    icf_return_t                    dummy_ret_val = ICF_SUCCESS;
    icf_msg_st                      *p_mmi_msg = ICF_NULL;
    icf_int16_t                     api_len = ICF_NULL;
    icf_tunnelled_create_media_session_req_st
                                	*p_mmi_msg_payload = ICF_NULL;
    icf_internal_list_st	        *p_stream_sdp_info_node = ICF_NULL;
	icf_stream_sdp_info_st		    *p_stream_sdp_info = ICF_NULL;

	ICF_FUNCTION_ENTER(p_mmi_pdb->p_glb_pdb) /* Function entry trace */

    ICF_CHECK_IF_ANY_OF_TWO_VAR_IS_NULL(p_mmi_pdb->p_glb_pdb,
                                        p_mmi_pdb->p_call_ctx,
                                        p_mmi_pdb->p_call_ctx->p_stream_sdp_info,
                                        ret_val)
    if(ICF_FAILURE == ret_val)
    {
        ICF_FUNCTION_EXIT(p_mmi_pdb->p_glb_pdb) /* Function exit trace */
        return ret_val;
    }

    /* Extracting the stream SDP node in the call context */
    p_stream_sdp_info_node = p_mmi_pdb->p_call_ctx->p_stream_sdp_info;
    p_stream_sdp_info = (icf_stream_sdp_info_st *)(p_stream_sdp_info_node->p_payload);

    api_len = ICF_PORT_SIZEOF(icf_api_header_st) + 
		      ICF_PORT_SIZEOF(icf_tunnelled_create_media_session_req_st);

    ICF_MEMGET(p_mmi_pdb->p_glb_pdb,
			   api_len,
			   ICF_MEM_COMMON,
			   p_mmi_msg,
               ICF_DONOT_RET_ON_FAILURE,
	 		   p_mmi_pdb->p_ecode,
	 		   dummy_ret_val)
    
	if(ICF_FAILURE == dummy_ret_val)
    {
        ret_val = ICF_FAILURE;
        ICF_FUNCTION_EXIT(p_mmi_pdb->p_glb_pdb) /* Function exit trace */
	    ICF_ALARM(((icf_uint8_t *)"[MMI]: MEMGET failure"))
    	return dummy_ret_val;
    }

    /* Fill the header of the message to be send */
    p_mmi_msg->hdr.api_id = ICF_TUNNELLED_CREATE_MEDIA_SESSION_REQ;
    p_mmi_msg->hdr.source_id = ICF_MODULE_ICF;
    p_mmi_msg->hdr.destination_id = ICF_MODULE_MM;
    p_mmi_msg->hdr.api_length = api_len;
    p_mmi_msg->hdr.call_id = p_mmi_pdb->p_call_ctx->call_id;
    p_mmi_msg->hdr.version = ICF_VERSION_1_0;
    p_mmi_msg->hdr.app_id = p_mmi_pdb->p_call_ctx->app_id;
   
    p_mmi_msg_payload = (icf_tunnelled_create_media_session_req_st *)(p_mmi_msg->payload);    

    ICF_CHECK_IF_VAR_IS_NULL(p_mmi_pdb->p_glb_pdb,
                             p_mmi_msg_payload,
                             ret_val)
    if(ICF_FAILURE == ret_val)
    {
        ICF_FUNCTION_EXIT(p_mmi_pdb->p_glb_pdb) /* Function exit trace */
        return ret_val;
    }

    p_mmi_msg_payload->bit_mask = ICF_NULL;
   
    /* Now populating the payload of the tunnelled CMS request API */

    /* Copy the Line ID and set its bitmask */
    p_mmi_msg_payload->line_id = p_mmi_pdb->p_call_ctx->line_id;
    p_mmi_msg_payload->bit_mask |= ICF_TUNNELLED_CMS_REQ_LINE_ID_PRESENT;

	/* If remote received SDP is present in call context
	   (as will be the case for incoming calls), 
	   copy it to the tunnelled CMS request payload.
	*/ 
    if((ICF_NULL != p_stream_sdp_info) &&
       (ICF_NULL != p_stream_sdp_info->p_remote_received_sdp))
    {
        /* Remote SDP present. Copy it to the tunnelled CMS request payload and
           set its bitmask.
        */
        if(ICF_NULL != p_stream_sdp_info->p_remote_received_sdp->p_tunnelled_sdp)
        {
            icf_port_memcpy(&p_mmi_msg_payload->tunnelled_sdp_info,
                            p_stream_sdp_info->p_remote_received_sdp->p_tunnelled_sdp,
                            ICF_PORT_SIZEOF(icf_tunnelled_sdp_info_st));
            p_mmi_msg_payload->bit_mask |= ICF_TUNNELLED_CMS_REQ_SDP_INFO_PRESENT;
        }
    }
	
    /* Send the tunnelled CMS request to the application/MM */
    ret_val = icf_mmi_send(p_mmi_pdb,p_mmi_msg,p_mmi_pdb->p_ecode);

    if(ICF_SUCCESS == ret_val)
    {
        /* Change state as tunnelled CMS request has been successfully sent. */
	    ICF_MMI_SET_STATE(p_mmi_pdb, ICF_MMI_11_AWT_TUNNELLED_CMS_RESP)
    }
    
	ICF_MEMFREE(p_mmi_pdb->p_glb_pdb,
			    p_mmi_msg,
				ICF_MEM_COMMON,
        		p_mmi_pdb->p_ecode,
				dummy_ret_val)

	ICF_FUNCTION_EXIT(p_mmi_pdb->p_glb_pdb) /* Function exit trace */
	return ret_val;
}

/******************************************************************************
 *
 * FUNCTION:        icf_mmi_00_tunnelled_mms_request
 *
 * DESCRIPTION:     This function is invoked when the MMI is in
 *                  in IDLE state and it receives a tunnelled MMS request from CC.
 *                  Current State: ICF_MMI_00_IDLE
 *                  Current Event: ICF_MMI_TUNNELLED_MMS_REQ
 *                  
 * RETURNS:         ICF_SUCCESS in case of success and ICF_FAILURE in 
 *                  case of any failure.                  
 *
 *****************************************************************************/
icf_return_t icf_mmi_00_tunnelled_mms_request(
    icf_mmi_pdb_st			*p_mmi_pdb)
{
    icf_msg_st              *p_mmi_msg = ICF_NULL;
    icf_return_t            dummy_ret_val = ICF_SUCCESS;
    icf_return_t            ret_val = ICF_SUCCESS;              
    icf_int16_t             api_len = ICF_NULL;
    icf_tunnelled_modify_media_session_req_st
                            *p_mmi_msg_payload = ICF_NULL;
    icf_internal_list_st	*p_stream_sdp_info_node = ICF_NULL;
	icf_stream_sdp_info_st	*p_stream_sdp_info = ICF_NULL;

	ICF_FUNCTION_ENTER(p_mmi_pdb->p_glb_pdb) /* Function entry trace */

    ICF_CHECK_IF_ANY_OF_TWO_VAR_IS_NULL(p_mmi_pdb->p_glb_pdb,
                                        p_mmi_pdb->p_call_ctx,
                                        p_mmi_pdb->p_call_ctx->p_stream_sdp_info,
                                        ret_val)
    if(ICF_FAILURE == ret_val)
    {
        ICF_FUNCTION_EXIT(p_mmi_pdb->p_glb_pdb) /* Function exit trace */
        return ret_val;
    }

    /* Extracting the stream SDP node in the call context */
    p_stream_sdp_info_node = p_mmi_pdb->p_call_ctx->p_stream_sdp_info;
    p_stream_sdp_info = (icf_stream_sdp_info_st *)(p_stream_sdp_info_node->p_payload);

    api_len = ICF_PORT_SIZEOF(icf_api_header_st) + 
              ICF_PORT_SIZEOF(icf_tunnelled_modify_media_session_req_st);

    ICF_MEMGET(p_mmi_pdb->p_glb_pdb,
               api_len,
               ICF_MEM_COMMON,
               p_mmi_msg,
               ICF_DONOT_RET_ON_FAILURE,
               p_mmi_pdb->p_ecode,
               dummy_ret_val)

    if(ICF_FAILURE == dummy_ret_val)
    {
        ret_val = ICF_FAILURE;
	    ICF_ALARM(((icf_uint8_t *)"[MMI]: MEMGET failure"))
        ICF_FUNCTION_EXIT(p_mmi_pdb->p_glb_pdb) /* Function exit trace */
	    return dummy_ret_val;
    }    

    /* Fill the header of the message to be send */
    p_mmi_msg->hdr.api_id = ICF_TUNNELLED_MODIFY_MEDIA_SESSION_REQ;
    p_mmi_msg->hdr.source_id = ICF_MODULE_ICF;
    p_mmi_msg->hdr.destination_id = ICF_MODULE_MM;
    p_mmi_msg->hdr.api_length = api_len;
    p_mmi_msg->hdr.call_id = p_mmi_pdb->p_call_ctx->call_id;
    p_mmi_msg->hdr.version = ICF_VERSION_1_0;
    p_mmi_msg->hdr.app_id = p_mmi_pdb->p_call_ctx->app_id;
    
    p_mmi_msg_payload = (icf_tunnelled_modify_media_session_req_st *)(p_mmi_msg->payload);  

    ICF_CHECK_IF_VAR_IS_NULL(p_mmi_pdb->p_glb_pdb,
                             p_mmi_msg_payload,
                             ret_val)
    if(ICF_FAILURE == ret_val)
    {
        ICF_FUNCTION_EXIT(p_mmi_pdb->p_glb_pdb) /* Function exit trace */
        return ret_val;
    }

    p_mmi_msg_payload->bit_mask = ICF_NULL;

    /* Now populating the payload of tunnelled MMS request API */

    /* 
       
       In tunnel mode, whenever MMI has to send a tunnelled MMS request to
       the application/MM or it receives a tunnelled MMS response from the
       application/MM, it checks for the following 2 bits in the 
       media_modify_type of the call context:
       
       1. ICF_LOCAL_SDP_REQD
       2. ICF_TUNNELLED_REMOTE_SDP_PRESENT

       In tunnel mode,CC always sets/resets these 2 bits before triggerring 
       MMI.

       ICF_LOCAL_SDP_REQD is set : It means that local SDP is sought from 
       the application in tunnelled MMS response. This is indicated to the
       application by setting the boolean flag local_sdp_reqd in tunnelled
       MMS request to ICF_TRUE. 

       ICF_LOCAL_SDP_REQD is not set : It means that local SDP is not sought
       from the application in tunnelled MMS response. This is indicated to the
       application by setting the boolean flag local_sdp_reqd in tunnelled
       MMS request to ICF_FALSE.

       ICF_TUNNELLED_REMOTE_SDP_PRESENT is not set : It means that remote SDP is not 
       being given to the application in tunnelled MMS request.

       ICF_TUNNELLED_REMOTE_SDP_PRESENT is set : It means that remote SDP is being given 
       to the application in tunnelled MMS request.

    */   


    /* Case when both tunnelled MMS request and tunnelled MMS response will have SDP */
    
    if((ICF_NULL != p_stream_sdp_info) &&
       (ICF_NULL != p_stream_sdp_info->p_remote_received_sdp) &&
       (ICF_LOCAL_SDP_REQD == (p_stream_sdp_info->media_modify_type & ICF_LOCAL_SDP_REQD)) &&
       (ICF_TUNNELLED_REMOTE_SDP_PRESENT == 
       (p_stream_sdp_info->media_modify_type & ICF_TUNNELLED_REMOTE_SDP_PRESENT)))
    {
        /* Remote SDP present. Copy it to the tunnelled MMS request payload and
           set its bitmask.
        */
        if(ICF_NULL != p_stream_sdp_info->p_remote_received_sdp->p_tunnelled_sdp)
        {
            icf_port_memcpy(&p_mmi_msg_payload->tunnelled_sdp_info,
                            p_stream_sdp_info->p_remote_received_sdp->p_tunnelled_sdp,
                            ICF_PORT_SIZEOF(icf_tunnelled_sdp_info_st));

            p_mmi_msg_payload->bit_mask |= ICF_TUNNELLED_MMS_REQ_SDP_INFO_PRESENT;
  
            /* We are expecting local SDP in tunnelled MMS response. */
            p_mmi_msg_payload->local_sdp_reqd = ICF_TRUE;

            /* Set ICF_MMI_LOCAL_SDP_REQD bitmask in mmi context. This will be 
               used by MMI when tunnelled MMS response is received from the application.
            */
            if(ICF_NULL != p_mmi_pdb->p_mmi_ctx)
            {
                p_mmi_pdb->p_mmi_ctx->mmi_bitmask |= ICF_MMI_LOCAL_SDP_REQD;
            }
        }
    }
    
    /* Case when only tunnelled MMS request will have SDP */

    else if((ICF_NULL != p_stream_sdp_info) &&
            (ICF_NULL != p_stream_sdp_info->p_remote_received_sdp) &&
            (ICF_LOCAL_SDP_REQD != (p_stream_sdp_info->media_modify_type & ICF_LOCAL_SDP_REQD)) &&
            (ICF_TUNNELLED_REMOTE_SDP_PRESENT == 
            (p_stream_sdp_info->media_modify_type & ICF_TUNNELLED_REMOTE_SDP_PRESENT)))
    {
        /* Remote SDP present. Copy it to the tunnelled MMS request payload and
           set its bitmask.
        */
        if(ICF_NULL != p_stream_sdp_info->p_remote_received_sdp->p_tunnelled_sdp)
        {
        
            icf_port_memcpy(&p_mmi_msg_payload->tunnelled_sdp_info,
                            p_stream_sdp_info->p_remote_received_sdp->p_tunnelled_sdp,
                            ICF_PORT_SIZEOF(icf_tunnelled_sdp_info_st));

            p_mmi_msg_payload->bit_mask |= ICF_TUNNELLED_MMS_REQ_SDP_INFO_PRESENT;

            /* We are not expecting local SDP in tunnelled MMS response. */
            p_mmi_msg_payload->local_sdp_reqd = ICF_FALSE;

            /* Reset ICF_MMI_LOCAL_SDP_REQD bitmask in mmi context. This will be 
               used by MMI when tunnelled MMS response is received from the application.
            */
            if(ICF_NULL != p_mmi_pdb->p_mmi_ctx)
            {
                p_mmi_pdb->p_mmi_ctx->mmi_bitmask &= ~ICF_MMI_LOCAL_SDP_REQD;
            }
        }
    }

    /* Case when only tunnelled MMS response will have SDP */

    else if(p_stream_sdp_info && ((ICF_LOCAL_SDP_REQD == (p_stream_sdp_info->media_modify_type & ICF_LOCAL_SDP_REQD)) &&
            (ICF_TUNNELLED_REMOTE_SDP_PRESENT != 
            (p_stream_sdp_info->media_modify_type & ICF_TUNNELLED_REMOTE_SDP_PRESENT))))
    {
        /* Tunnelled MMS request without SDP */

        /* We are expecting local SDP in tunnelled MMS response. */
        p_mmi_msg_payload->local_sdp_reqd = ICF_TRUE;

        /* Set ICF_MMI_LOCAL_SDP_REQD bitmask in mmi context. This will be 
           used by MMI when tunnelled MMS response is received from the application.
        */
        if(ICF_NULL != p_mmi_pdb->p_mmi_ctx)
        {
            p_mmi_pdb->p_mmi_ctx->mmi_bitmask |= ICF_MMI_LOCAL_SDP_REQD;
        }
    }

     /* Send the tunnelled MMS request to the application/MM */
	ret_val = icf_mmi_send(p_mmi_pdb,p_mmi_msg,p_mmi_pdb->p_ecode);

    if(ICF_SUCCESS == ret_val)
    {
        /* Change state as tunnelled MMS request has been successfully sent. */
	    ICF_MMI_SET_STATE(p_mmi_pdb, ICF_MMI_12_AWT_TUNNELLED_MMS_RESP)
    }
    
    ICF_MEMFREE(p_mmi_pdb->p_glb_pdb,
                p_mmi_msg,
                ICF_MEM_COMMON,
        		p_mmi_pdb->p_ecode,
                dummy_ret_val)

    ICF_FUNCTION_EXIT(p_mmi_pdb->p_glb_pdb) /* Function exit trace */
	return ret_val;
}

