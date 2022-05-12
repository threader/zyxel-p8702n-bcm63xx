/******************************************************************************
 * 
 * FILE NAME:   icf_mmi_12_awt_tunnelled_mms_resp.c
 * 
 * DESCRIPTION: This file contains functions to handle events in the AWT 
 *              tunnelled MMS response state in MMI.  
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
 * FUNCTION:        icf_mmi_12_tunnelled_mms_resp
 * 
 * DESCRIPTION:     This function is invoked when the MMI is
 *                  awaiting tunnelled MMS response from MM and it receives the same.
 *                  Current State: ICF_MMI_12_AWT_TUNNELLED_MMS_RESP
 *                  Current Event: ICF_MMI_TUNNELLED_MMS_RESP
 *                  
 * RETURNS:         ICF_SUCCESS in case of success and ICF_FAILURE in 
 *                  case of any failure.                  
 *
 ******************************************************************************/
icf_return_t icf_mmi_12_tunnelled_mms_resp(
        INOUT icf_mmi_pdb_st             *p_mmi_pdb)
{
    icf_return_t               ret_val = ICF_SUCCESS;
    
    ICF_FUNCTION_ENTER(p_mmi_pdb->p_glb_pdb) /* function entry trace */

    /* Tunnelled MMS request/response completed. Change state. */
    ICF_MMI_SET_STATE(p_mmi_pdb,ICF_MMI_00_IDLE)

    /* Call the handler function to send the tunnelled MMS response
       received to CC.
    */
    ret_val = icf_mmi_send_tunnelled_mms_resp(p_mmi_pdb);

    ICF_FUNCTION_EXIT(p_mmi_pdb->p_glb_pdb) /* function exit trace */    
    return ret_val;
}

/******************************************************************************
 *
 * FUNCTION:        icf_mmi_send_tunnelled_mms_resp
 * 
 * DESCRIPTION:     This function sends the tunnelled MMS response received
*                   from the application/MM to CC.
 *                  
 * RETURNS:         ICF_SUCCESS in case of success and ICF_FAILURE in 
 *                  case of any failure.                  
 *
 ******************************************************************************/
icf_return_t icf_mmi_send_tunnelled_mms_resp(
        INOUT icf_mmi_pdb_st             *p_mmi_pdb)
{
    icf_return_t			ret_val = ICF_SUCCESS;
    icf_return_t            dummy_ret_val = ICF_SUCCESS;
    icf_internal_msg_st     *p_cc_internal_msg = ICF_NULL;

    /* Initialising the internal MMS response structure to be sent to CC.
       This is required as we are using the same internal interface between
       CC and MMI in tunnel and non tunnel mode. In tunnel mode, fields like
       mod_stream_count,modified_stream_resp etc are not used.
       Hence they should be intialised.
    */  
    icf_mmi_mms_resp_st	    mms_resp = {0,0,0,0,{{0,0}},0,{{0,0}}};

    icf_tunnelled_modify_media_session_resp_st
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

    p_recv_msg_payload = (icf_tunnelled_modify_media_session_resp_st *)
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
    p_cc_internal_msg->msg_hdr.msg_id = ICF_MODIFY_MEDIA_RESP;
    p_cc_internal_msg->msg_hdr.payload_length = ICF_PORT_SIZEOF(icf_mmi_mms_resp_st);
    
    /* Making the internal message */
    mms_resp.result = p_recv_msg_payload->result;
    mms_resp.error_cause = ICF_NULL;
    mms_resp.bit_mask = ICF_NULL;

    /* Success tunnelled MMS response received */
    if(ICF_SUCCESS == mms_resp.result)
    {
        /* Local SDP was sought from the application */
        
        if((ICF_NULL != p_mmi_pdb->p_mmi_ctx) &&
           (ICF_MMI_LOCAL_SDP_REQD == (p_mmi_pdb->p_mmi_ctx->mmi_bitmask &
            ICF_MMI_LOCAL_SDP_REQD)))
        {
            /* SDP was sought and it is present in the tunnelled MMS response. */

            if(ICF_TUNNELLED_MMS_RESP_SDP_INFO_PRESENT == (p_recv_msg_payload->bit_mask
                    & ICF_TUNNELLED_MMS_RESP_SDP_INFO_PRESENT))
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

                    /* Memory acquired successfully.
                       Copy the local SDP received from the application to the 
                       local SDP of the call context.
                    */   
                    icf_port_memcpy(p_stream_sdp_info->p_local_sdp->p_tunnelled_sdp,
                                    &p_recv_msg_payload->tunnelled_sdp_info,
                                    ICF_PORT_SIZEOF(icf_tunnelled_sdp_info_st));
                    /* Set this following bitmask to indicate to CC that
                       MMI has copied local SDP.
                    */
                    mms_resp.bit_mask |= ICF_MMI_MMS_ACTION_LOCAL_SDP_RCVD;
                }
                else
                {   
                    /* p_stream_sdp_info->p_local_sdp is NULL */
                    ICF_ALARM(((icf_uint8_t *)"[MMI]: p_stream_sdp_info->p_local_sdp is NULL"))
                    ret_val = ICF_FAILURE;
                }
            } /* end of local SDP expected */

            /* If the flow did not enter the above if condition, it means that 
               local SDP was sought from the application but the application did
               not provide it.We will not return failure.We will not set the bit
               ICF_MMI_MMS_ACTION_LOCAL_SDP_RCVD in the mms internal message. 
               CC will take appropriate action on the basis of this bit.
               For example, for a new offer received from remote, CC expects application to
               answer (provide local SDP in tunnelled MMS response). If it doesnt, it has to 
               clear the call/clear the transaction/send a negative response to the offer
               received.
            */   
        } /* end of result is success */

        /* If the flow did not enter the above if condition, it means that
           local SDP was not sought from the application.We will not set the bit
           ICF_MMI_MMS_ACTION_LOCAL_SDP_RCVD in the mms internal message and return
           success.
        */
    }
    else /* failure result */
    {
        /* If error cause is present in the tunnelled MMS response,
           set the same in the call context.
        */
        if(ICF_TUNNELLED_MMS_RESP_ERROR_CAUSE_PRESENT == (p_recv_msg_payload->bit_mask &
                    ICF_TUNNELLED_MMS_RESP_ERROR_CAUSE_PRESENT))
        {
            mms_resp.error_cause = p_recv_msg_payload->error_cause;
            switch (p_recv_msg_payload->error_cause) 
            {
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

    /* Reset the bitmask ICF_MMI_LOCAL_SDP_REQD */
    if(ICF_NULL != p_mmi_pdb->p_mmi_ctx)
    {
        p_mmi_pdb->p_mmi_ctx->mmi_bitmask &= ~ICF_MMI_LOCAL_SDP_REQD;
    }

    /* Route the message to CC only if ret_val is ICF_SUCCESS */
    if(ICF_SUCCESS == ret_val)
    {
        /* Fill internal message's payload */
        p_cc_internal_msg->p_msg_data = (icf_void_t *)&(mms_resp);

        /* Tunnelled MMS request/response processing complete. Change state. */
        ICF_MMI_SET_STATE(p_mmi_pdb,ICF_MMI_00_IDLE)
    
        /* Route the tunnelled MMS response message to CC */
        ret_val = icf_cm_process_mesg(p_cc_internal_msg);
    }

    ICF_FUNCTION_EXIT(p_mmi_pdb->p_glb_pdb)
    return ret_val;
}
