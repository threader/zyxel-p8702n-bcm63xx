/******************************************************************************
 * 
 * FILE NAME:   icf_mmi_05_awt_dms_resp.c
 * 
 * DESCRIPTION: This file contains functions to handle events in the AWT DMS
 * 				response state in MMI.
 *              
 * REVISION HISTORY:    
 *   Date            Name          Reference       Comments
 * 25-july-2006     Puneet       ICF Rel 6.0    Initial Version- support for
 * 												multiple m-lines
 * 09-Feb-2009      Tarun Gupta  Rel 8.1 LLD    SDP Tunnelling Enhancements
 * 						
 * Copyright (c) 2009, Aricent.  
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
 * FUNCTION:        icf_mmi_05_dms_request
 * 
 * DESCRIPTION:     This function is invoked when the MMI is
 *                  awaiting DMS resp from MM and it receives DMS req from CC.   
 *                  Current State: ICF_MMI_05_AWT_DMS_RESP
 *                  
 * RETURNS:         ICF_SUCCESS in case of success and ICF_FAILURE in 
 *                  case of any failure.                  
 *
 *****************************************************************************/

icf_return_t icf_mmi_05_dms_request(
        INOUT icf_mmi_pdb_st             *p_mmi_pdb)
{
    icf_return_t                         ret_val = ICF_SUCCESS;
    icf_mmi_dms_request_st               *p_msg_data_recd = ICF_NULL;
    
    ICF_FUNCTION_ENTER(p_mmi_pdb->p_internal_msg->msg_hdr.p_glb_pdb)
        
    p_msg_data_recd = (icf_mmi_dms_request_st *)(p_mmi_pdb->
            p_internal_msg->p_msg_data);
    
    /* Check if it is a forceful delete */
    if(ICF_TRUE == p_msg_data_recd->forceful_delete)
    {
        ICF_MMI_TRACE(p_mmi_pdb,ICF_TRACE_FORCEFUL_DMS)
        
        /* In this case rel res request will be send and MMI will go to IDLE 
         * state
         */
		
        /* Reset bit */
        ICF_MMI_RESET_BIT(p_mmi_pdb,ICF_MEDIA_ESTABLISHED)

		if(ICF_MMI_DMS_ONGOING == p_mmi_pdb->p_mmi_ctx->ongoing_event)
		{
			/* Send REL_RES_IND for all active streams in call ctxt */
			icf_mmi_send_rel_res_ind(p_mmi_pdb);

			/* Send CLOSE_CHANNEL_IND for all active streams in call ctxt */
			ICF_MMI_PDB_SET_BIT(p_mmi_pdb,ICF_MMI_PDB_ALL_STREAMS)
			icf_mmi_send_close_channel_ind(p_mmi_pdb); /* need to send close channel for 
											    * for all streams including TBCP */
			ICF_MMI_PDB_RESET_BIT(p_mmi_pdb,ICF_MMI_PDB_ALL_STREAMS)

#ifdef ICF_NAT_MEDIA_TRAVERSAL
			/* Send REL_PUBLIC_IP_PORT for all active streams in call ctxt */
			icf_mmi_rel_nat_resources(p_mmi_pdb);
#endif

			ICF_MMI_SET_STATE(p_mmi_pdb,ICF_MMI_00_IDLE)
		}
		else if(ICF_MMI_MMS_DEL_ONGOING == p_mmi_pdb->p_mmi_ctx->ongoing_event)
		{
			icf_mmi_send_forceful_dms_req(p_mmi_pdb);
		}
    }
    else
    {
        ICF_MMI_TRACE(p_mmi_pdb,ICF_TRACE_DMS_NOT_FORCEFUL)
        /* Ignore the request */
    }    
    
    ICF_FUNCTION_EXIT(p_mmi_pdb->p_glb_pdb)
    return ret_val;
}/* End function */ 


/******************************************************************************
 *
 * FUNCTION:        icf_mmi_05_dms_resp
 * 
 * DESCRIPTION:     This function is invoked when the MMI is
 *                  awaiting DMS resp from MM and it receives the same.
 *                  Current State: ICF_MMI_05_AWT_DMS_RESP
 *                  
 * RETURNS:         ICF_SUCCESS in case of success and ICF_FAILURE in 
 *                  case of any failure.                  
 *
 *****************************************************************************/

icf_return_t icf_mmi_05_dms_resp(
        INOUT icf_mmi_pdb_st             *p_mmi_pdb)
{
    icf_return_t                         ret_val = ICF_SUCCESS;
    icf_msg_resp_st *p_mmi_msg_payload;
    
    ICF_FUNCTION_ENTER(p_mmi_pdb->p_glb_pdb)
        
    p_mmi_msg_payload = (icf_msg_resp_st *)
        (p_mmi_pdb->p_glb_pdb->p_recv_msg->payload);

	ICF_MMI_SET_STATE(p_mmi_pdb,ICF_MMI_00_IDLE)
    
    if(ICF_FAILURE == p_mmi_msg_payload->result)
    {
        ICF_MMI_TRACE(p_mmi_pdb,ICF_TRACE_FAILURE_RESP)
		icf_mmi_dms_failure_resp(p_mmi_pdb);
	}
	else
	{
		icf_mmi_dms_success_resp(p_mmi_pdb);
	}

    ICF_FUNCTION_EXIT(p_mmi_pdb->p_glb_pdb)
    return ret_val;        
}/* End function */

/******************************************************************************
 *
 * FUNCTION:    icf_mmi_dms_success_resp
 *
 * DESCRIPTION: 
 *****************************************************************************/
icf_return_t	icf_mmi_dms_success_resp(icf_mmi_pdb_st		*p_mmi_pdb)
{
	icf_return_t					ret_val = ICF_SUCCESS;
	icf_mmi_mms_request_st			*p_mms_req = ICF_NULL;
#ifdef ICF_NAT_MEDIA_TRAVERSAL
        icf_uint8_t                      cnt = 0;
#endif

	ICF_FUNCTION_ENTER(p_mmi_pdb->p_glb_pdb)

	/* In tunnel mode, we will just send the success dummy DMS response
	   received to CC.
	*/
ICF_CHECK_TUNNEL_MODE_START(p_mmi_pdb->p_glb_pdb,ICF_TRUE)
    ret_val = icf_mmi_send_delete_media_response(p_mmi_pdb);
ICF_CHECK_TUNNEL_MODE_END

ICF_CHECK_TUNNEL_MODE_START(p_mmi_pdb->p_glb_pdb,ICF_FALSE)

	if(ICF_MMI_DMS_ONGOING == p_mmi_pdb->p_mmi_ctx->ongoing_event)
	{
		/* Reset bit */
		ICF_MMI_RESET_BIT(p_mmi_pdb,ICF_MEDIA_ESTABLISHED)

		/* Send REL_RES_IND for all active streams minus TBCP stream */
		icf_mmi_send_rel_res_ind(p_mmi_pdb);

#ifdef ICF_NAT_MEDIA_TRAVERSAL
		/* Send REL_PUBLIC_IP_PORT for all active streams in call ctxt */
		icf_mmi_rel_nat_resources(p_mmi_pdb);
#endif

		/* Send Success DMS resp to CC */

		ret_val = icf_mmi_send_delete_media_response(p_mmi_pdb);
	}
	else if(ICF_MMI_MMS_DEL_ONGOING == p_mmi_pdb->p_mmi_ctx->ongoing_event)
	{
		/* If CC is waiting for DMS response then send DMS */
		if(ICF_CC_07_AWT_DMS_RESP == p_mmi_pdb->p_call_ctx->call_state)
		{
			ICF_MMI_TRACE(p_mmi_pdb,ICF_TRACE_CC_AWT_DMS)
			ret_val = icf_mmi_send_awaited_dms_req(p_mmi_pdb);
			return ret_val;        
		}/* End if to check CC state */

		p_mms_req = p_mmi_pdb->p_mmi_ctx->p_mms_req;
/*Fix for CSR-1-5140139 */
#ifdef ICF_NAT_MEDIA_TRAVERSAL
       for(cnt=0;cnt< p_mms_req->p_delete_media->count;cnt++)
       {    
       icf_mmi_send_rel_public_ip_port(p_mmi_pdb, p_mms_req->p_delete_media->stream_id[cnt]);
       } 
#endif        
          /* Send REL_RES_IND for all streams minus TBCP in DEL action of MM_req*/
		icf_mmi_send_rel_res_ind(p_mmi_pdb);

		if(p_mms_req->bit_mask & ICF_MMI_MMS_ACTION_MODIFY)
		{
			icf_mmi_process_mms_modify_action(p_mmi_pdb);			
		}
		else if(p_mms_req->bit_mask & ICF_MMI_MMS_ACTION_ADD)
		{
			p_mmi_pdb->p_mmi_ctx->ongoing_event = ICF_MMI_MMS_ADD_ONGOING;

			ret_val = icf_mmi_send_gcl_req(p_mmi_pdb);

			if(ICF_SUCCESS == ret_val)
			{
				ICF_MMI_SET_STATE(p_mmi_pdb,ICF_MMI_01_AWT_GCL_RESP)
			}
		}
		/* Case when only del action was present in MM_req */
		else
		{
			/* Send success MM_resp to CC */

			ret_val = icf_mmi_send_modify_media_response(p_mmi_pdb);
		}
	}
ICF_CHECK_TUNNEL_MODE_END

	ICF_FUNCTION_EXIT(p_mmi_pdb->p_glb_pdb)
	return ret_val;    
}

/******************************************************************************
 *
 * FUNCTION:    icf_mmi_dms_failure_resp
 *
 * DESCRIPTION: 
 *****************************************************************************/
icf_return_t	icf_mmi_dms_failure_resp(icf_mmi_pdb_st		*p_mmi_pdb)
{
	icf_return_t					ret_val = ICF_SUCCESS;
/*	icf_mmi_mms_request_st			*p_mms_req = ICF_NULL;*/

	ICF_FUNCTION_ENTER(p_mmi_pdb->p_glb_pdb)

	if(ICF_MMI_DMS_ONGOING == p_mmi_pdb->p_mmi_ctx->ongoing_event)
	{
		/* Reset bit */
		ICF_MMI_RESET_BIT(p_mmi_pdb,ICF_MEDIA_ESTABLISHED)
	
		icf_mmi_send_rel_res_ind(p_mmi_pdb);

		/* Send CLOSE_CHANNEL for all active streams in call ctxt 
		 * IMP: We need to differentiate between the case when CLOSE_CHANNEL 
		 * for only TBCP stream or when for all active streams so set the 
		 * bit in MMI pdb */

		ICF_MMI_PDB_SET_BIT(p_mmi_pdb,ICF_MMI_PDB_ALL_STREAMS)
		icf_mmi_send_close_channel_ind(p_mmi_pdb); /* here we need to send close channel for all 
                                                      * streams including TBCP */
		ICF_MMI_PDB_RESET_BIT(p_mmi_pdb,ICF_MMI_PDB_ALL_STREAMS)

#ifdef ICF_NAT_MEDIA_TRAVERSAL
		 /* Send REL_PUBLIC_IP_PORT for all active streams in call ctxt */
		icf_mmi_rel_nat_resources(p_mmi_pdb);
#endif

		if(ICF_SUCCESS == ret_val)
		{
			ICF_MMI_TRACE(p_mmi_pdb,ICF_TRACE_API_SEND)
		}

		/* Need to clarify with CC that whatever the resp for DMS we are 
		 * freeing all resources Send DMS resp to CC */

		ret_val = icf_mmi_send_delete_media_response(p_mmi_pdb);
	}
	else if(ICF_MMI_MMS_DEL_ONGOING == p_mmi_pdb->p_mmi_ctx->ongoing_event)
	{
		/* If CC is waiting for DMS response then send DMS */
		if(ICF_CC_07_AWT_DMS_RESP == p_mmi_pdb->p_call_ctx->call_state)
		{
			ICF_MMI_TRACE(p_mmi_pdb,ICF_TRACE_CC_AWT_DMS)
			ret_val = icf_mmi_send_awaited_dms_req(p_mmi_pdb);
			return ret_val;        
		}/* End if to check CC state */

/* Fix for SPR 14114: We shall not send rel/close_channel in case of DMS failure
   we'll send the MMS request failure to CC: Decision by Mayank/Pnegi */
#if 0
		p_mms_req = p_mmi_pdb->p_mmi_ctx->p_mms_req;

		/* Send REL_RES_IND for all streams minus TBCP in DEL action of MM_req*/
		icf_mmi_send_rel_res_ind(p_mmi_pdb);

		/* Send CLOSE_CHANNEL_IND for all streams in DEL action of MM_req */
		ICF_MMI_PDB_SET_BIT(p_mmi_pdb,ICF_MMI_PDB_ALL_STREAMS)
		icf_mmi_send_close_channel_ind(p_mmi_pdb); /* here we need to send close channel for all 
                                                      * streams including TBCP */
		ICF_MMI_PDB_RESET_BIT(p_mmi_pdb,ICF_MMI_PDB_ALL_STREAMS)

#ifdef ICF_NAT_MEDIA_TRAVERSAL
		/* Send REL_PUBLIC_IP_PORT for all streams in DEL action of MM_req */
		icf_mmi_rel_nat_resources(p_mmi_pdb);
#endif

#endif

		/* In case any action in MM_req fails we shall not process further 
		 * requests Send failure MM_resp to CC */
		((icf_mmi_internal_mms_resp_st *)p_mmi_pdb->p_mmi_ctx->p_mmi_resp)->result =
			ICF_FAILURE;

		ret_val = icf_mmi_send_modify_media_response(p_mmi_pdb);
	}

	ICF_FUNCTION_EXIT(p_mmi_pdb->p_glb_pdb)

	return ret_val;   
}

