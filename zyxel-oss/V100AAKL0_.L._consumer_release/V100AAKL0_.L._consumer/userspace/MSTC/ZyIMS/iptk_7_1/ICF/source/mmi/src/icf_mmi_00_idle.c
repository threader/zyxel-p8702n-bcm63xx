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
 * 						
 * Copyright (c) 2006, Aricent .  
 ******************************************************************************/

/* include files */
#include "icf_internal_api_id.h"
#include "icf_mmi_defs.h"
#include "icf_mmi_types.h"
#include "icf_mmi_macro.h"
#include "icf_mmi_prototypes.h"
#include "icf_dbm_prototypes.h"
#include "icf_ecode.h"
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

	while(ICF_NULL != p_src)
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

