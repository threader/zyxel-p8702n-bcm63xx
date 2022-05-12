/******************************************************************************
 * 
 * FILE NAME:   icf_mmi_11_awt_tunnelled_cms_resp.c
 * 
 * DESCRIPTION: This file contains functions to handle events in the AWT 
 *              tunnelled CMS response state in MMI.  
 *              
 * REVISION HISTORY:    
 *   Date            Name          Reference       Comments
 *
 * 09-Feb-2009     Tarun Gupta    Rel 8.1 LLD   Initial Revision created for
 *                                              SDP Tunnelling Enhancements
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
#include "icf_ecode.h"

/******************************************************************************
 *
 * FUNCTION:        icf_mmi_11_tunnelled_cms_resp
 * 
 * DESCRIPTION:     This function is invoked when the MMI is
 *                  awaiting tunnelled CMS response from MM and it receives the same.
 *                  Current State: ICF_MMI_11_AWT_TUNNELLED_CMS_RESP
 *                  Current Event: ICF_MMI_TUNNELLED_CMS_RESP
 *                  
 * RETURNS:         ICF_SUCCESS in case of success and ICF_FAILURE in 
 *                  case of any failure.                  
 *
 ******************************************************************************/
icf_return_t icf_mmi_11_tunnelled_cms_resp(
        INOUT icf_mmi_pdb_st             *p_mmi_pdb)
{
    icf_return_t               ret_val = ICF_SUCCESS;
    
    ICF_FUNCTION_ENTER(p_mmi_pdb->p_glb_pdb) /* function entry trace */

    /* Tunnelled CMS request/response completed. Change state. */
    ICF_MMI_SET_STATE(p_mmi_pdb,ICF_MMI_00_IDLE)

    /* Call the handler function to send the tunnelled CMS response
       received to CC.
    */
    ret_val = icf_mmi_send_tunnelled_cms_resp(p_mmi_pdb);

    ICF_FUNCTION_EXIT(p_mmi_pdb->p_glb_pdb) /* function exit trace */    
    return ret_val;
}

/******************************************************************************
 *
 * FUNCTION:        icf_mmi_send_tunnelled_cms_resp
 * 
 * DESCRIPTION:     This function sends the tunnelled CMS response received
*                   from the application/MM to CC.
 *                  
 * RETURNS:         ICF_SUCCESS in case of success and ICF_FAILURE in 
 *                  case of any failure.                  
 *
 ******************************************************************************/
icf_return_t icf_mmi_send_tunnelled_cms_resp(
        INOUT icf_mmi_pdb_st             *p_mmi_pdb)
{
    icf_return_t			ret_val = ICF_SUCCESS;
    icf_return_t            dummy_ret_val = ICF_SUCCESS;
    icf_internal_msg_st     *p_cc_internal_msg = ICF_NULL;
    
    /* Initialising the internal CMS response structure to be sent to CC.
       This is required as we are using the same internal interface between
       CC and MMI in tunnel and non tunnel mode. In tunnel mode, fields like
       count and stream_status are not used. Hence they should be intialised.
    */  
    icf_mmi_cms_resp_st	    cms_resp = {0,0,0,{{0,0}}};
    
    icf_tunnelled_create_media_session_resp_st
                            *p_recv_msg_payload = ICF_NULL;
    icf_internal_list_st	*p_stream_sdp_info_node = ICF_NULL;
	icf_stream_sdp_info_st	*p_stream_sdp_info = ICF_NULL;

    ICF_FUNCTION_ENTER(p_mmi_pdb->p_glb_pdb) /* function entry trace */

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

    p_recv_msg_payload = (icf_tunnelled_create_media_session_resp_st *)
                         (p_mmi_pdb->p_glb_pdb->p_recv_msg->payload);

    ICF_CHECK_IF_VAR_IS_NULL(p_mmi_pdb->p_glb_pdb,
                             p_recv_msg_payload,
                             ret_val)
    if(ICF_FAILURE == ret_val)
    {
        ICF_FUNCTION_EXIT(p_mmi_pdb->p_glb_pdb) /* Function exit trace */
        return ret_val;
    }

    p_cc_internal_msg = p_mmi_pdb->p_internal_msg;
    p_cc_internal_msg->msg_hdr.msg_id = ICF_CREATE_MEDIA_RESP;
    p_cc_internal_msg->msg_hdr.payload_length = ICF_PORT_SIZEOF(icf_mmi_cms_resp_st);
    
    /* Making the internal message */
    cms_resp.result = p_recv_msg_payload->result;
    cms_resp.error_cause = ICF_NULL;

    /* Success tunnelled CMS response received */
    if(ICF_SUCCESS == cms_resp.result)
    {
        /* If local SDP is present in the tunnelled CMS response,copy
           the same in the local SDP of the call context.
        */
        if(ICF_TUNNELLED_CMS_RESP_SDP_INFO_PRESENT == (p_recv_msg_payload->bit_mask &
                    ICF_TUNNELLED_CMS_RESP_SDP_INFO_PRESENT))
        {
            if(ICF_NULL != p_stream_sdp_info->p_local_sdp)
            {
                /* If the tunnelled SDP structure is not NULL, we will first free 
                   its memory and then allocate new memory for it.
                */
                if(ICF_NULL != p_stream_sdp_info->p_local_sdp->p_tunnelled_sdp)
                {
                    /* Free the acquired memory */
                    ICF_MEMFREE(p_mmi_pdb->p_glb_pdb,
                                p_stream_sdp_info->p_local_sdp->p_tunnelled_sdp,
                                ICF_MEM_COMMON,
                                p_mmi_pdb->p_ecode,
                                dummy_ret_val)
                }
                    
                /*  Allocate new memory */    
                ICF_MEMGET(p_mmi_pdb->p_glb_pdb,
                    	   ICF_PORT_SIZEOF(icf_tunnelled_sdp_info_st),
                		   ICF_MEM_COMMON,
                           p_stream_sdp_info->p_local_sdp->p_tunnelled_sdp,
                    	   ICF_DONOT_RET_ON_FAILURE,
        			       p_mmi_pdb->p_ecode,
     					   ret_val)

				if(ICF_FAILURE == ret_val)
				{
					ICF_ALARM(((icf_uint8_t *)"[MMI]: MEMGET failure"))
                    ICF_FUNCTION_EXIT(p_mmi_pdb->p_glb_pdb) /* function exit trace */    
                    return ret_val;
                }

                /* Memory successfully allocated, now memcpy the SDP received into
                   local SDP of the call context.
                */   
                icf_port_memcpy(p_stream_sdp_info->p_local_sdp->p_tunnelled_sdp,
                                &p_recv_msg_payload->tunnelled_sdp_info,
                                ICF_PORT_SIZEOF(icf_tunnelled_sdp_info_st));
            }
            else
            {
                /* p_stream_sdp_info->p_local_sdp is NULL */
                ICF_ALARM(((icf_uint8_t *)"[MMI]: p_stream_sdp_info->p_local_sdp is NULL"))
                ret_val = ICF_FAILURE;
            }
        }
        else
        {
            /* No SDP received, we are always expecting a SDP in tunnelled
               CMS response. Send ICF_MEDIA_ERROR_IND to CC.
            */
            ret_val = icf_mmi_send_err_ind_call_clear(p_mmi_pdb);
            /* The above function creates an internal message and sends the same
               to CC. No need to continue processing in this function.
            */
            ICF_FUNCTION_EXIT(p_mmi_pdb->p_glb_pdb)
            return ret_val;
        }
    }
    else /* failure result */
    {
        /* If error cause is present in the tunnelled CMS response,
           set the same in the call context.
        */
        if(ICF_TUNNELLED_CMS_RESP_ERROR_CAUSE_PRESENT == (p_recv_msg_payload->bit_mask &
                    ICF_TUNNELLED_CMS_RESP_ERROR_CAUSE_PRESENT))
        {
            cms_resp.error_cause = p_recv_msg_payload->error_cause;
            switch (p_recv_msg_payload->error_cause) 
            {
                case ICF_CAUSE_UNABLE_TO_CREATE_MEDIA: 
                case ICF_CAUSE_TRANSPORT_LAYER_FAILURE:    
                {
                    /* This will map to a 488 response */
                    p_mmi_pdb->p_call_ctx->release_cause =
                        ICF_CALL_CLEAR_MEDIA_MISMATCH;
                    break; 
                }

                default:
        	        break;
            }
        }
    } /* end failure result */

    /* Route the message to CC only if ret_val is ICF_SUCCESS */
    if(ICF_SUCCESS == ret_val)
    {
        /* Fill internal message's payload */
        p_cc_internal_msg->p_msg_data = (icf_void_t *)&(cms_resp);

        /* Tunnelled CMS request/response processing complete. Change state. */
        ICF_MMI_SET_STATE(p_mmi_pdb,ICF_MMI_00_IDLE)
    
        /* Route the tunnelled CMS response message to CC */
        ret_val = icf_cm_process_mesg(p_cc_internal_msg);
    }

    ICF_FUNCTION_EXIT(p_mmi_pdb->p_glb_pdb)
    return ret_val;
}
