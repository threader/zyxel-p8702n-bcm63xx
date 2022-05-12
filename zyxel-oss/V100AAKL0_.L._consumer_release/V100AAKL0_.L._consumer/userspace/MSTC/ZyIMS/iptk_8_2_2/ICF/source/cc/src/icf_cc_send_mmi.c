/*------------------------------------------------------------------------------
 *
 * File name        : icf_cc_send_mmi.c
 *
 * Purpose          : This file contains the function required to make and send 
 *                    messages to MMI.
 *
 * Revision History :
 *
 *
 * Date         Name            Ref#                Comments
 * --------     ------------    ----                ---------------
 * 13-Dec-2004  Pankaj Negi     Non Service LLD     Initial
 * 20-Dec-2004  Rohit Aggarwal                      Beginning of coding
 * 24-Dec-2004  Rohit Aggarwal                      Compilation fixes
 * 27-Dec-2004  Rohit Aggarwal                      Header file changes
 * 06-May-2005  Sumit Gupta       SPR 7438          Reset call_clear status MM 
 *                                                  cleared after sending cms 
 *                                                  req
 * 26-Jun-2006  Amit Sharma     ICF 6.0 APIs        Structure changes for multiple
 *                                                  m-lines
 * 10-Jul-2007  Abhishek Dutta  ICF 7.0             Cancel call modify changes
 * 09-Feb-2009  Tarun Gupta     Rel 8.1 LLD         SDP Tunelling Enhancements
 * 28-May-2009  Ashutosh Mohan  SPR 19672           CSR-1-6976670 merged
 * 10-Jun-2009  Kamal Ashraf    SPR 19590           Modified icf_cc_send_mms_req  
 * 14-Jun-2009  Abhishek Dhammawat SPR 19590        Modified icf_cc_send_cms_req 
 * 05-Oct-2009  Tarun Gupta     SPR 20091           Modified icf_cc_send_dms_req
 * Copyright (c) 2009, Aricent.
 *----------------------------------------------------------------------------*/


#include "icf_common_prototypes.h"
#include "icf_ecode.h"
#include "icf_internal_api_id.h"
#include "icf_cc_prototypes.h"
#include "icf_cc_macro.h"
#include "icf_macro.h"

/*******************************************************************************
 *
 * FUNCTION:        icf_cc_send_cms_req
 *
 * DESCRIPTION:     This function is used to make a Create Media Session 
 *                  request and send it to MMI.
 *
 ******************************************************************************/
icf_return_t icf_cc_send_cms_req(
        INOUT icf_cc_pdb_st *p_cc_pdb)
{
    icf_return_t                     ret_val = ICF_SUCCESS;
    icf_internal_msg_st              *p_mmi_msg = ICF_NULL;

    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /* function entry trace */
    
    /* SPR 19590 : Start */        
    /* Store the txn_id in mmi ctx once doing the CMS req, this
       shall be useful once we have the cms resp and we populate the
       txn_id from the mmi ctx into call ctx so that the correct node
       in used when indicating to PA */
       ICF_CHECK_IF_VAR_IS_NULL(p_cc_pdb->p_glb_pdb,
                     p_cc_pdb->p_call_ctx,
                     ret_val)
       if(ICF_FAILURE == ret_val)
       {
            ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */
            return ret_val;
       }
       p_cc_pdb->p_call_ctx->mmi_context.txn_id =
              p_cc_pdb->p_call_ctx->txn_id;
    /* SPR 19590 : End */
      
    p_mmi_msg = p_cc_pdb->p_internal_msg;
    
    /* **** CMS request header **** */
    p_mmi_msg->msg_hdr.msg_id = ICF_CREATE_MEDIA_REQ;
    p_mmi_msg->msg_hdr.payload_length = 0;
    p_mmi_msg->msg_hdr.ecode = ICF_ERROR_NONE;

    /* **** CMS request payload **** */ 
    p_mmi_msg->p_msg_data = ICF_NULL;

    /* Send message to MMI */   
    ret_val = icf_mmi_process_mesg(p_mmi_msg);
    
    /* Set media ongoing flag in call context */
    if(ret_val == ICF_SUCCESS)
    {

        ICF_CC_SET_MEDIA_ONGOING(p_cc_pdb)
    	ICF_CC_TRACE(p_cc_pdb, ICF_TRACE_SEND_MMI_REQ)
        /*If cms indication was successfullly sent to MM then 
         * reset the MM_CLEAR flag in call_clear_status*/
         ICF_CC_RESET_CALL_CLR_STATUS(p_cc_pdb,
                 ICF_CALL_CLEAR_STATUS_MM_CLEARED) 
    }
    
    
    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */
    return ret_val;
}


/*******************************************************************************
 *
 * FUNCTION:        icf_cc_send_mms_req
 *
 * DESCRIPTION:     This function is used to make a Modify Media Session 
 *                  request and send it to MMI.
 *
 ******************************************************************************/
icf_return_t icf_cc_send_mms_req(
        INOUT icf_cc_pdb_st *p_cc_pdb)
{
    icf_return_t                     ret_val = ICF_SUCCESS;
	icf_internal_msg_st              *p_mmi_msg = ICF_NULL;
    icf_mmi_mms_request_st           mms_payload; 
    icf_uint8_t                      ongoing_event = 0;
	icf_internal_list_st	         *p_stream_sdp_info_node = ICF_NULL;
	icf_stream_sdp_info_st		     *p_stream_sdp_info = ICF_NULL;
    icf_internal_list_st             *p_list=ICF_NULL;

    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /* function entry trace */

    /* Extracting the payload of the p_stream_sdp_info in the call context. */
	ICF_CHECK_IF_ANY_OF_TWO_VAR_IS_NULL(p_cc_pdb->p_glb_pdb,
			                            p_cc_pdb->p_call_ctx,
							            p_cc_pdb->p_call_ctx->p_stream_sdp_info,
										ret_val)
	if(ICF_FAILURE == ret_val)
	{
        ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */
        return ret_val;
	}
    /* SPR 19590 : Start */        
    /* Store the txn_id in mmi ctx once doing the MMS req, this
       shall be useful once we have the mms resp and we populate the
       txn_id from the mmi ctx into call ctx so that the correct node
       in used when indicating to PA */
    p_cc_pdb->p_call_ctx->mmi_context.txn_id = p_cc_pdb->p_call_ctx->txn_id;
    /* SPR 19590 : End */
	p_stream_sdp_info_node = p_cc_pdb->p_call_ctx->p_stream_sdp_info;
    p_stream_sdp_info = (icf_stream_sdp_info_st *)
                        (p_stream_sdp_info_node->p_payload);

    icf_port_memset(&mms_payload,0,ICF_PORT_SIZEOF(icf_mmi_mms_request_st));
   	
	mms_payload.bit_mask = 0;
	mms_payload.modify_stream_count = 0;
	mms_payload.p_add_media = ICF_NULL;
	mms_payload.p_delete_media = ICF_NULL;


    p_mmi_msg = p_cc_pdb->p_internal_msg;

    /* **** MMS request header **** */
    p_mmi_msg->msg_hdr.msg_id = ICF_MODIFY_MEDIA_REQ;
    p_mmi_msg->msg_hdr.payload_length = sizeof(icf_mmi_mms_request_st);
    p_mmi_msg->msg_hdr.ecode = ICF_ERROR_NONE;
    
	/* In tunnel mode, setting of the media_modify_type is done by the calling
	   function. Thus the invocation of the following function is not 
	   required.
	   In tunnel mode, if media_modify_type is not NULL, just set the 
	   variable mms_sent in the CC pdb to TRUE.
	*/   
ICF_CHECK_TUNNEL_MODE_START(p_cc_pdb->p_glb_pdb,ICF_FALSE)
    /* **** MMS request payload **** */
    /*Fill MMS type per stream basis*/

    ret_val = icf_cc_set_media_modify_type(p_cc_pdb,&mms_payload);      

    /*mms_payload.modify_media_type = p_cc_pdb->mms_type;
    mms_payload.count = 0;*/
ICF_CHECK_TUNNEL_MODE_END

ICF_CHECK_TUNNEL_MODE_START(p_cc_pdb->p_glb_pdb,ICF_TRUE)
    if((ICF_NULL != p_stream_sdp_info) &&
       (ICF_NULL != p_stream_sdp_info->media_modify_type))
	{
		/* media_modify_type is not NULL */
        p_cc_pdb->mms_sent = ICF_TRUE;
	}
ICF_CHECK_TUNNEL_MODE_END

	if((ICF_TRUE == p_cc_pdb->mms_sent) && (ICF_SUCCESS == ret_val))
    {
        p_mmi_msg->p_msg_data = (icf_uint8_t *)(&mms_payload);
        
        /*ICF_CC_TRACE(p_cc_pdb, ICF_TRACE_MMS_REQ_PAYLOAD)*/

        /* Send message to MMI */
        ret_val = icf_mmi_process_mesg(p_mmi_msg);

        /* CSR-1-6976670: Make mms req as per the codecs received from remote */
        if (ICF_TRUE == p_cc_pdb->p_config_data->check_remote_codec_list)  
	{         
	    p_list = p_cc_pdb->p_call_ctx->p_stream_sdp_info;
            while (ICF_NULL != p_list)
            {
                p_stream_sdp_info = (icf_stream_sdp_info_st *)p_list->p_payload;
                if ((ICF_NULL != p_stream_sdp_info) && 
                    (ICF_NULL != p_stream_sdp_info->p_remote_received_sdp) && 
                    (ICF_NULL != p_stream_sdp_info->p_remote_received_sdp->\
                    sdp_stream.p_codec_info_list))
                {
                    if(ICF_NULL != (p_stream_sdp_info->p_bckup_codec_from_remote))
                    {
                        icf_cmn_delete_codec_attrib_list(p_cc_pdb->p_glb_pdb, 
                            &p_stream_sdp_info->p_bckup_codec_from_remote,p_cc_pdb->p_ecode);
                            p_stream_sdp_info->p_bckup_codec_from_remote = ICF_NULL;
                    }
                    icf_cmn_clone_codec_attrib_list(
              	    p_cc_pdb->p_glb_pdb,
                        &(p_stream_sdp_info->p_bckup_codec_from_remote),
                        p_stream_sdp_info->p_remote_received_sdp->
                        sdp_stream.p_codec_info_list);
                }
                p_list=p_list->p_next;
            }
        }
        
        /* Set media ongoing flag in call context */
        if(ret_val == ICF_SUCCESS)
		{
            p_cc_pdb->p_call_ctx->sdp_changed = ICF_TRUE;
            ICF_CC_SET_MEDIA_ONGOING(p_cc_pdb)
/*call_mod_cancel*/
            /*Don't reset the ongoing event to current event when ongoing event is 
             *ICF_CC_SSA_CALL_MODIFY_CANCEL_RESP*/ 
            ongoing_event =  ICF_CC_GET_ONGOING_EVENT(p_cc_pdb);
            if (ICF_NULL == ongoing_event)
            {
                ICF_CC_SET_ONGOING_EVENT(p_cc_pdb,p_cc_pdb->current_event)
            } 
            else if(ICF_CC_SSA_CALL_MODIFY_CANCEL_RESP != ongoing_event)
            {
               ICF_CC_SET_ONGOING_EVENT(p_cc_pdb,p_cc_pdb->current_event)
            }
        }

        ICF_CC_TRACE(p_cc_pdb, ICF_TRACE_SEND_MMI_REQ)
        /* Tunnel mode specific trace invocation. Trace data will be filled 
           in the function icf_cc_trace.
        */
ICF_CHECK_TUNNEL_MODE_START(p_cc_pdb->p_glb_pdb,ICF_TRUE)
#ifdef ICF_TRACE_ENABLE                
       /* Tunnel mode specific trace */
       ICF_CC_APP_TRACE(p_cc_pdb,ICF_TRACE_TUNNELLED_MMS_REQ_SENT)  
#endif
       ICF_CC_TRACE(p_cc_pdb,ICF_TRACE_MEDIA_MODIFY_SENT)
ICF_CHECK_TUNNEL_MODE_END
    } /* end of if for ret_val of set_mms_type */

	else
    {
        /* In tunnel mode we will not set the bitmask ICF_OFFER_CODECS_IN_GCL */
ICF_CHECK_TUNNEL_MODE_START(p_cc_pdb->p_glb_pdb,ICF_FALSE)

        /* TBD - post 6.3 */
		/* Uncommented corresponding to spr 14476.*/
        ICF_CC_RESET_COMMON_BITMASK(p_cc_pdb, ICF_OFFER_CODECS_IN_GCL)

ICF_CHECK_TUNNEL_MODE_END
    }

    ICF_CC_TRACE(p_cc_pdb,ICF_TRACE_MEDIA_TO_BE_SENT)
    
    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */
    return ret_val;
}


/*******************************************************************************
 *
 * FUNCTION:        icf_cc_send_dms_req
 *
 * DESCRIPTION:     This function is used to make a Modify Media Session 
 *                  request and send it to MMI.
 *
 ******************************************************************************/
icf_return_t icf_cc_send_dms_req(
        INOUT icf_cc_pdb_st *p_cc_pdb)
{
    icf_return_t                     ret_val = ICF_SUCCESS;
    icf_internal_msg_st              *p_mmi_msg = ICF_NULL;
    icf_mmi_dms_request_st           dms_payload;
    
    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /* function entry trace */
    /* SPR 19590 : Start */        
    /* Store the txn_id in mmi ctx once doing the DMS req, this
       shall be useful once we have the dms resp and we populate the
       txn_id from the mmi ctx into call ctx so that the correct node
       in used when indicating to PA */
    ICF_CHECK_IF_VAR_IS_NULL(p_cc_pdb->p_glb_pdb,
                     p_cc_pdb->p_call_ctx,
                     ret_val)
    if(ICF_FAILURE == ret_val)
    {
         ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */
         return ret_val;
    }
    p_cc_pdb->p_call_ctx->mmi_context.txn_id =
              p_cc_pdb->p_call_ctx->txn_id;
    /* SPR 19590 : End */

    /* SPR 20091 : In case of forceful call clear scenarios like restart or
       reinitialise request from the application or user/proxy deletion,
       we will not issue a DMS request towards MM in tunnel mode.
       
       In tunnel mode,we do not want to send a DMS request as it would
       lead to ES appending a dummy DMS response and posting it back to MMI.
       However due to absence of a valid call context, MMI will reject
       that DMS response with ICF_UNEXPECTED_MSG_IND through function
       icf_mmi_process_mesg.Thus the application would be given this
       indication without it sending any API.
    */   
ICF_CHECK_TUNNEL_MODE_START(p_cc_pdb->p_glb_pdb,ICF_TRUE)
    if(ICF_TRUE == p_cc_pdb->forceful_delete)
    {
        /* Reset the forceful delete flag */
        p_cc_pdb->forceful_delete = ICF_FALSE;
        ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */
        return ret_val;
    }
ICF_CHECK_TUNNEL_MODE_END
    /* End of SPR 20091 fix */
    
    p_mmi_msg = p_cc_pdb->p_internal_msg;

    /* **** DMS request header **** */
    p_mmi_msg->msg_hdr.msg_id = ICF_DELETE_MEDIA_REQ;
    p_mmi_msg->msg_hdr.payload_length = sizeof(icf_mmi_dms_request_st);
    p_mmi_msg->msg_hdr.ecode = ICF_ERROR_NONE;

    /* **** DMS request payload **** */
    dms_payload.forceful_delete = p_cc_pdb->forceful_delete;
 
    p_mmi_msg->p_msg_data = (icf_uint8_t *)(&dms_payload);

    /* In tunnel mode, no need to send this trace as we are not sending
	   the DMS request.
	*/
ICF_CHECK_TUNNEL_MODE_START(p_cc_pdb->p_glb_pdb,ICF_FALSE)
    ICF_CC_TRACE(p_cc_pdb, ICF_TRACE_DMS_REQ_PAYLOAD)
ICF_CHECK_TUNNEL_MODE_END

    /* Send message to MMI */
    ret_val = icf_mmi_process_mesg(p_mmi_msg);
    
    /* Set media ongoing flag in call context */
    if(ret_val == ICF_SUCCESS)
    {
        ICF_CC_SET_MEDIA_ONGOING(p_cc_pdb)
    }
    else
    {
        /*
         * If the error was returned due to no MMI context being available set 
         * dms_fail_no_mmi_ctx in cc_pdb to TRUE and return success to the 
         * calling function
         */
        if(ICF_ECODE_NO_MMI_CONTEXT_FOUND == p_mmi_msg->msg_hdr.ecode)
        {
            p_cc_pdb->dms_fail_no_mmi_ctx = ICF_TRUE;
            ret_val = ICF_SUCCESS;
        }
    }
    /* In tunnel mode, no need to send this trace as we are not sending
	   the DMS request.
	*/
ICF_CHECK_TUNNEL_MODE_START(p_cc_pdb->p_glb_pdb,ICF_FALSE)
    ICF_CC_TRACE(p_cc_pdb, ICF_TRACE_DMS_REQ_PAYLOAD)
ICF_CHECK_TUNNEL_MODE_END

    	ICF_CC_TRACE(p_cc_pdb, ICF_TRACE_SEND_MMI_REQ)

    /* Reset the forceful delete flag */
    p_cc_pdb->forceful_delete = ICF_FALSE;
    
        
    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */
    return ret_val;
}
