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
 *
 *
 * Copyright (c) 2006, Aricent.
 *----------------------------------------------------------------------------*/


#include "icf_common_prototypes.h"
#include "icf_ecode.h"
#include "icf_internal_api_id.h"
#include "icf_cc_prototypes.h"
#include "icf_cc_macro.h"


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
    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /* function entry trace */

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
    
    /* **** MMS request payload **** */
    /*Fill MMS type per stream basis*/
    
    ret_val = icf_cc_set_media_modify_type(p_cc_pdb,&mms_payload);      

    /*mms_payload.modify_media_type = p_cc_pdb->mms_type;
    mms_payload.count = 0;*/

    if((ICF_TRUE == p_cc_pdb->mms_sent) && (ICF_SUCCESS == ret_val))
    {
        p_mmi_msg->p_msg_data = (icf_uint8_t *)(&mms_payload);
        
        /*ICF_CC_TRACE(p_cc_pdb, ICF_TRACE_MMS_REQ_PAYLOAD)*/

        /* Send message to MMI */
        ret_val = icf_mmi_process_mesg(p_mmi_msg);
        
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
    } /* end of if for ret_val of set_mms_type */
	else
	{
		/* TBD - post 6.3 */
		/* Uncommented corresponding to spr 14476.*/
		ICF_CC_RESET_COMMON_BITMASK(p_cc_pdb, ICF_OFFER_CODECS_IN_GCL)
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

    p_mmi_msg = p_cc_pdb->p_internal_msg;

    /* **** DMS request header **** */
    p_mmi_msg->msg_hdr.msg_id = ICF_DELETE_MEDIA_REQ;
    p_mmi_msg->msg_hdr.payload_length = sizeof(icf_mmi_dms_request_st);
    p_mmi_msg->msg_hdr.ecode = ICF_ERROR_NONE;

    /* **** DMS request payload **** */
    dms_payload.forceful_delete = p_cc_pdb->forceful_delete;
 
    p_mmi_msg->p_msg_data = (icf_uint8_t *)(&dms_payload);

    ICF_CC_TRACE(p_cc_pdb, ICF_TRACE_DMS_REQ_PAYLOAD)

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
    	ICF_CC_TRACE(p_cc_pdb, ICF_TRACE_SEND_MMI_REQ)

    /* Reset the forceful delete flag */
    p_cc_pdb->forceful_delete = ICF_FALSE;
    
        
    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */
    return ret_val;
}
