/*------------------------------------------------------------------------------
 *
 * File name        : icf_cc_send_ssa.c
 *
 * Purpose          : This file contains the function required to make and send
 *                    messages to SSA.
 *
 * Revision History :
 *
 * Date         Name            Ref#            Comments
 * --------     ------------    ----            ---------------
 * 13-Dec-04    Pankaj Negi     Non Service LLD Initial
 * 24-Dec-2004  Rohit Aggarwal                  Compilation fixes
 * 27-Dec-2004  Rohit Aggarwal                  Header file changes
 * 05-Apr-2005  Sumit Gupta     SPR No 7354     Call a function to clear up 
 *                                              diversion info call context
 * 13-Apr-2005  Sumit Gupta     SPR No 7380     Reset signaling ongoing when 
 *                                              offer change response was sent 
 *                                              after call is connected
 * 13-Apr-2005  Sumit Gupta     SPR No 7387     Added functionality to send 
 *                                              ICF_NW_OTG_CALL_ABORT_RESP
 *                                              when cancel is received from NW 
 *  27-Apr-2005 Sumit Gupta    Changes in common p_replace_nw_call_id in 
 *                             form: cf_0001101  icf_common_types.h was 
 *                                               replaced with 
 *                                               p_replaces_header. 
 *                                               Corresponding change done
 * 06-May-2005 Sumit Gupta     SPR 7438          Reset call_clear status NW 
 *                                               cleared after sending setup req
 * 17-May-2005 Sumit Gupta     SPR 7583          Forceful clear would only be 
 *                                               sent to if nw cleared flag is 
 *                                               not set in call_clear_status
 * 25-May-2005 Sumit Gupta     SPR 7440          Updated Stats    
 * 26-May-2005 Sumit Gupta     SPR 7599          Remote hold indication would be
 *                                               send to PA after sending 
 *                                               connect ack to SSA if the media
 *                                               state is remote held  
 * 27-May-2005 Sumit Gupta     SPR 7599          Reverted back the previous 
 *                                               change done for the SPR 
 * 09-Jun-2005 Jalaj Negi      IMS CLIENT        Added function for sending
 *                                               UPDATE message.
 * 08-Mar-2006  Aman Aggarwal					ICF merging with ICF2.0
 * 30-Mar-2006  Anuradha Gupta	ICF 5.0				Call Modify Feature changes
 * 26-Jun-2006  Amit Sharma     ICF 6.0 APIs    Structure changes for multiple
 *                                              m-lines
 * 03-Jan-2007  Amit Mahajan                    INFO confirmation changes. 
 * 20-FEB-2007 Abhishek Dhammawat SPR 14479     Setting the correct bitmask to avoid 
 *                                              sending re-invite instead of BYE after
 *                                              mms failure.
 * 10-Jul-2007 Abhishek Dutta     ICF 7.0       Added the changes for
 *                                              Cancel call modify 
 *                                              Handling
 * 03-Dec-2008 Tarun Gupta        SPR 19218     Merged CSR 1-6188936
 * 13-Dec-2008 Abhishek Dhammawat SPR 19218     Merged CSR 1-6132503
 * 10-Feb-2009 Tarun Gupta        Rel 8.1 LLD   SDP Tunnelling Enhancements
 * 16-Feb-2009 Anurag Khare       Rel 8.1       Call modify enhancements
 *                                              in forking scenario
 * 18-FEB-2009 Abhishek Dhammawat Rel 8.1       Modified
 *                                              icf_cc_ssa_send_update_msg  
 * 19-Feb-2009 Anurag Khare       Rel 8.1       Call modify Enhacements
 * 18-Mar-2009 Anurag Khare       SPR 19395     Chengse done for SDP Tunnelling
 * 30-Mar-2009 Anurag Khare       Rel 8.2       Changes done for PRACK Enh
 * 09-Apr-2009 Rajiv Kumar        SPR 19428     Merged CSR 1-6851629
 * 29-Apr-2009 Abhishek Dhammawat SPR 19491     Modified
 *                                              icf_cc_send_ssa_connect_ack
 * 05-May-2009 Anuradha Gupta     SPR 19576     Modified icf_cc_send_ssa_call_term_req
 * 28-May-2009 Anuradha Gupta     SPR 19672     Modified icf_cc_ssa_send_update_msg
 * 10-Jun-2009 Abhishek Dhammawat SPR 19590     Modified
 *                                              icf_cc_send_ssa_info_req
 * 02-Aug-2009 Rajiv Kumar        SPR 19886     Merged CSR 1-7444867 
 * 08-Oct-2009 Anuradha Gupta     SPR 20111     Modified 
 *                                                icf_cc_send_ssa_force_clear
 * 22-Oct-20099 Anurag Khare      SPR 20078     Fix for SPR 20078
 *
 * Copyright (c) 2009, Aricent.
 *---------------------------------------------------------------------------*/


#include "icf_common_prototypes.h"
#include "icf_internal_api_id.h"
#include "icf_cc_prototypes.h"
#include "icf_cc_macro.h"

ICF_HANDLE_REINVITE_START
#include "icf_dbm_prototypes.h"
ICF_HANDLE_REINVITE_END

/*******************************************************************************
 *
 * FUNCTION:        icf_cc_send_ssa_setup_req
 *
 * DESCRIPTION:     This function is used to send Call Setup Request to SSA.
 *                  SSA will create INVITE using media parameters present in
 *                  call context.
 *                  ICF_SSA_CALL_SETUP (SIP INVITE to be sent)
 *
 ******************************************************************************/
icf_return_t icf_cc_send_ssa_setup_req(
        INOUT icf_cc_pdb_st *p_cc_pdb)
{
    icf_return_t                     ret_val = ICF_SUCCESS;


    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb)
    
    /* Fix for CSR 1-6132503 : SPR 18794 */
	/* Reset the release_cause in call_ctx so that the old value is not 
	 * used in sdf_cbk_uaCallFailed( ) on getting final failure response for 
	 * this INVITE if this INVITE is being sent again (with Auth or Redirect).
	 */
    ICF_CHECK_IF_VAR_IS_NULL(p_cc_pdb->p_glb_pdb,
           p_cc_pdb->p_call_ctx, ret_val)
    if(ICF_FAILURE == ret_val)
    {
        ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb)
        return ret_val;
    }
	p_cc_pdb->p_call_ctx->release_cause = ICF_REL_CAUSE_INVALID;

    p_cc_pdb->p_internal_msg->msg_hdr.msg_id = ICF_NW_OTG_CALL_SETUP;
    p_cc_pdb->p_internal_msg->msg_hdr.payload_length = 0;


    /*Set pointer of message to SSA in msg_data*/
    p_cc_pdb->p_internal_msg->p_msg_data = ICF_NULL;

    /* Issuing the SDP related trace only in case of 
       non tunnel mode.
    */   
ICF_CHECK_TUNNEL_MODE_START(p_cc_pdb->p_glb_pdb,ICF_FALSE)
    ICF_CC_APP_TRACE(p_cc_pdb, ICF_TRACE_LOCAL_SDP_OFFER)
ICF_CHECK_TUNNEL_MODE_END    

    /*Call entry point of SSA*/
    ret_val = icf_ssa_process_mesg(p_cc_pdb->p_internal_msg);

    if(ICF_FAILURE == ret_val)
    {
        ICF_CC_TRACE(p_cc_pdb, ICF_INTERFACE_FAILURE)
        ret_val = ICF_FAILURE;
    }
    else
    {
        /* Reset the sdp changed flag */
        p_cc_pdb->p_call_ctx->sdp_changed = ICF_FALSE;
		/* This is being commented as it is not supported by ICF phase 1 */
#if ICF_SERVICE_ENABLE
        /* Fix for SPR: 19886 (Merged CSR 1-7444867)*/
        /* Fix For SPR:19788
         * Consider the following flow:
         * INVITE (1) ----->
         * 302        <-----
         * INVITE (2) ----->(On Redirected address)
         * 407        <-----
         * INVITE (3) ----->
         * If p_diversion_header_list is present in call context and
         * ICF_CALL_AUTHENTICATION_REQ bitmask is also set it means cc has
         * triggered function "icf_cc_send_ssa_setup_req" to send the
         * new INVITE(3) with auth params for the INVITE(2) request which is send on
         * redirected address of 302. In this case no need to send the
         * redirect indication to application.
         */        
        if((ICF_NULL != p_cc_pdb->p_call_ctx->p_diversion_header_list) &&
                (ICF_NULL == ICF_CC_CHECK_COMMON_BITMASK(p_cc_pdb,
                ICF_CALL_AUTHENTICATION_REQ)))        
        {
            ret_val = icf_cc_send_pa_forwarding_ind(p_cc_pdb);
        }
#endif
        /*If setup request was successfullly sent to SSA then 
         * reset the NW_CLEAR flag in call_clear_status*/
        ICF_CC_RESET_CALL_CLR_STATUS(p_cc_pdb,
                ICF_CALL_CLEAR_STATUS_NW_CLEARED)
    }

    /* Fix For SPR:19788
     * Diversion header list in call context is not freed at this point to
     * send the diversion header in INVITE (3) of above flow. It will be freed
     * while freeing the call context.
     */
    /* icf_cc_clear_div_info(p_cc_pdb);*/

    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb)
    return ret_val;
}


/*******************************************************************************
 *
 * FUNCTION:        icf_cc_send_ssa_setup_resp
 *
 * DESCRIPTION:     This function is invoked by Call Control after INVITE has
 *                  been validated to send Call Setup response to SSA.
 *                  ICF_SSA_CALL_SETUP_ACK (SIP 100 Trying to be sent)
 *
 ******************************************************************************/
icf_return_t icf_cc_send_ssa_setup_resp(
        INOUT icf_cc_pdb_st *p_cc_pdb)
{
    icf_return_t                     ret_val = ICF_SUCCESS;


    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb)

    p_cc_pdb->p_internal_msg->msg_hdr.msg_id = ICF_NW_OTG_CALL_SETUP_ACK;
    p_cc_pdb->p_internal_msg->msg_hdr.payload_length = 0;

    /*Call entry point of SSA*/
    ret_val = icf_ssa_process_mesg(p_cc_pdb->p_internal_msg);

    if(ICF_FAILURE == ret_val)
    {
        ICF_CC_TRACE(p_cc_pdb, ICF_INTERFACE_FAILURE)
        ret_val = ICF_FAILURE;
    }
    p_cc_pdb->p_call_ctx->sdp_changed = ICF_FALSE;

    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb)

    return ret_val;
}


/*******************************************************************************
 *
 * FUNCTION:        icf_cc_send_ssa_alert
 *
 * DESCRIPTION:     This function is invoked by Call Control to send alert
 *                  without media to remote party.
 *                  ICF_SSA_ALERT (SIP 180 without SDP to be sent)
 *
 ******************************************************************************/
icf_return_t icf_cc_send_ssa_alert(
        INOUT icf_cc_pdb_st *p_cc_pdb)
{
    icf_nw_otg_call_alert_st         call_alert;
    icf_return_t                     ret_val = ICF_SUCCESS;


    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb)

    p_cc_pdb->p_internal_msg->msg_hdr.msg_id = ICF_NW_OTG_CALL_ALERT;
    p_cc_pdb->p_internal_msg->msg_hdr.payload_length = sizeof(call_alert);


    call_alert.bitmask = ICF_NULL;

    /* Check if Prack is required */
    if(ICF_TRUE == p_cc_pdb->prack_reqd)
    {
        call_alert.bitmask |= ICF_NW_OTG_ALERT_RPR_REQD;
    }
    /*  Rel 8.2 changes start */
    /*  This is done for the case if first 18x did not contain 
     *  PRACK field then if next 18x we get PRACK field then
     *  to ignore that we forcefully set the call ctx field to the 
     *  value of system level flag
     */
    icf_cc_handle_prack_value(p_cc_pdb);
    /*  Rel 8.2 changes End */
    /*Set pointer of message to SSA in msg_data*/
    p_cc_pdb->p_internal_msg->p_msg_data = (icf_uint8_t *)&call_alert;


    /*Call entry point of SSA*/
    ret_val = icf_ssa_process_mesg(p_cc_pdb->p_internal_msg);

    if(ICF_FAILURE == ret_val)
    {
        ICF_CC_TRACE(p_cc_pdb, ICF_INTERFACE_FAILURE)
        ret_val = ICF_FAILURE;
    }
    else
    {
        /* Reset the sdp changed flag */
        p_cc_pdb->p_call_ctx->sdp_changed = ICF_FALSE;
        /* This is call with inband info as true . set the bit in call ctx*/
        p_cc_pdb->p_call_ctx->inband_info_present = ICF_FALSE;
    }

    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb)
    return ret_val;
}


/*******************************************************************************
 *
 * FUNCTION:        icf_cc_send_ssa_alert_media
 *
 * DESCRIPTION:     This function is invoked by Call Control to send alert
 *                  with media to remote party.
 *                  ICF_SSA_ALERT_WITH_MEDIA (SIP 180 with SDP to be sent)
 *
 ******************************************************************************/
icf_return_t icf_cc_send_ssa_alert_media(
        INOUT icf_cc_pdb_st *p_cc_pdb)
{
    icf_nw_otg_call_alert_st         call_alert;
    icf_return_t                     ret_val = ICF_SUCCESS;


    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb)

    p_cc_pdb->p_internal_msg->msg_hdr.msg_id = ICF_NW_OTG_ALERT_WITH_MEDIA;
    p_cc_pdb->p_internal_msg->msg_hdr.payload_length = sizeof(call_alert);


    call_alert.bitmask = ICF_NULL;

    /* Fill the replaced network call id */
    if(ICF_TRUE == p_cc_pdb->prack_reqd)
    {
        call_alert.bitmask |= ICF_NW_OTG_ALERT_WITH_MEDIA_RPR_REQD;
	if(ICF_TRUE == p_cc_pdb->p_call_ctx->media_committed)
        {
            ICF_CC_SET_COMMON_BITMASK(p_cc_pdb, ICF_CC_FINAL_SDP_SENT)
        }
    }
    /*  Rel 8.2 changes Start */
    /* This is done for the case if first 18x did not contain 
     * PRACK field then if next 18x we get PRACK field then
     * to ignore that we forcefully set the call ctx field to the 
     * value of system level flag
     */
    icf_cc_handle_prack_value(p_cc_pdb);
    /*  Rel 8.2 changes End */

    /*Set pointer of message to SSA in msg_data*/
    p_cc_pdb->p_internal_msg->p_msg_data = (icf_uint8_t *)&call_alert;


    /*Call entry point of SSA*/
    ret_val = icf_ssa_process_mesg(p_cc_pdb->p_internal_msg);

    if(ICF_FAILURE == ret_val)
    {
        ICF_CC_TRACE(p_cc_pdb, ICF_INTERFACE_FAILURE)
        ret_val = ICF_FAILURE;
    }
    else
    {
        /* Reset the sdp changed flag */
        p_cc_pdb->p_call_ctx->sdp_changed = ICF_FALSE;
        /* This is call with inband info as true . set the bit in call ctx*/
        p_cc_pdb->p_call_ctx->inband_info_present = ICF_TRUE;
    }

    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb)

    return ret_val;
}


/*******************************************************************************
 *
 * FUNCTION:        icf_cc_send_ssa_media_param
 *
 * DESCRIPTION:     This function is invoked by Call Control to send Session
 *                  Progress (with media) to remote party.
 *                  ICF_SSA_MEDIA_PARAM (SIP 183 to be sent)
 *
 ******************************************************************************/
icf_return_t icf_cc_send_ssa_media_param(
        INOUT icf_cc_pdb_st *p_cc_pdb)
{
    icf_return_t                     ret_val = ICF_SUCCESS;
    icf_nw_otg_media_param_st        media_param; 



    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb)

    p_cc_pdb->p_internal_msg->msg_hdr.msg_id = ICF_NW_OTG_MEDIA_PARAM;
    p_cc_pdb->p_internal_msg->msg_hdr.payload_length = sizeof(media_param);
    
    media_param.bitmask = ICF_NULL;

    /* Fill the replaced network call id */
    if(ICF_TRUE == p_cc_pdb->prack_reqd)
    {
        media_param.bitmask |=  ICF_NW_OTG_MEDIA_PARAM_RPR_REQD;
	if(ICF_TRUE == p_cc_pdb->p_call_ctx->media_committed)
        {
            ICF_CC_SET_COMMON_BITMASK(p_cc_pdb, ICF_CC_FINAL_SDP_SENT)
        }
    }
    /* Rel 8.2 Changes Start */
    /*  This is done for the case if first 18x did not contain 
     *  PRACK field then if next 18x we get PRACK field then
     *  to ignore that we forcefully set the call ctx field to the 
     *  value of system level flag
     */
    icf_cc_handle_prack_value(p_cc_pdb);
    /*  Rel 8.2 changes End */

    /*Set pointer of message to SSA in msg_data*/
    p_cc_pdb->p_internal_msg->p_msg_data = (icf_uint8_t *)&media_param;




    /*Call entry point of SSA*/
    ret_val = icf_ssa_process_mesg(p_cc_pdb->p_internal_msg);
    /* Reset the sdp changed flag */
    p_cc_pdb->p_call_ctx->sdp_changed = ICF_FALSE;

    if(ICF_FAILURE == ret_val)
    {
        ICF_CC_TRACE(p_cc_pdb, ICF_INTERFACE_FAILURE)
    }

    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb)
    return ret_val;
}

/*******************************************************************************
 *
 * FUNCTION:        icf_cc_send_ssa_media_param_wo_sdp
 *
 * DESCRIPTION:     This function is invoked by Call Control to send Session
 *                  Progress (without media) to remote party.
 *                  ICF_SSA_MEDIA_PARAM_WO_SDP (SIP 183 without SDP to be sent)
 *
 ******************************************************************************/
icf_return_t icf_cc_send_ssa_media_param_wo_sdp(
        INOUT icf_cc_pdb_st *p_cc_pdb)
{
    icf_return_t                     ret_val = ICF_SUCCESS;
    icf_nw_otg_media_param_st        media_param;

    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb)

    p_cc_pdb->p_internal_msg->msg_hdr.msg_id = ICF_NW_OTG_MEDIA_PARAM;
    p_cc_pdb->p_internal_msg->msg_hdr.payload_length = sizeof(media_param);

    media_param.bitmask = ICF_NULL;

	/* Message to be sent without SDP */
	media_param.bitmask |=  ICF_NW_OTG_MEDIA_PARAM_WITHOUT_SDP;

	/* Check if Prack is required */
    if(ICF_TRUE == p_cc_pdb->prack_reqd)
    {
        media_param.bitmask |=  ICF_NW_OTG_MEDIA_PARAM_RPR_REQD;
    }
    
    /*  Rel 8.2 Changes start 
     */
    /*  This is done for the case if first 18x did not contain 
     *  PRACK field then if next 18x we get PRACK field then
     *  to ignore that we forcefully set the call ctx field to the 
     *  value of system level flag
     */
    icf_cc_handle_prack_value(p_cc_pdb);
    /* Rel 8.2 Changes End */

    /* Set pointer of message to SSA in msg_data */
    p_cc_pdb->p_internal_msg->p_msg_data = (icf_uint8_t *)&media_param;

    /* Call entry point of SSA */
    ret_val = icf_ssa_process_mesg(p_cc_pdb->p_internal_msg);

    if(ICF_FAILURE == ret_val)
    {
        ICF_CC_TRACE(p_cc_pdb, ICF_INTERFACE_FAILURE)
    }

    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb)
    return ret_val;
}


/*******************************************************************************
 *
 * FUNCTION:        icf_cc_send_ssa_mediaprov_resp
 *
 * DESCRIPTION:     Invoked by CC toward SSA to send provisional/reliable
 *                  response to remote party for 183/media_session.
 *                  ICF_SSA_PRACK (SIP PRACK to be sent)
 *
 ******************************************************************************/
icf_return_t icf_cc_send_ssa_media_prov_resp(
        INOUT icf_cc_pdb_st *p_cc_pdb)
{
    icf_return_t                     ret_val = ICF_SUCCESS;


    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb)


    p_cc_pdb->p_internal_msg->msg_hdr.msg_id = ICF_NW_OTG_MEDIA_PARAM_PRACK;
    p_cc_pdb->p_internal_msg->msg_hdr.payload_length = 0;


    /*Call entry point of SSA*/
    ret_val = icf_ssa_process_mesg(p_cc_pdb->p_internal_msg);
    /* Reset the sdp changed flag */
    p_cc_pdb->p_call_ctx->sdp_changed = ICF_FALSE;

    if(ICF_FAILURE == ret_val)
    {
        ICF_CC_TRACE(p_cc_pdb, ICF_INTERFACE_FAILURE)
    }

    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb)
    return ret_val;
}


/*******************************************************************************
 *
 * FUNCTION:        icf_cc_send_ssa_alert_prov_resp
 *
 * DESCRIPTION:     Invoked by CC toward SSA to send provisional/reliable
 *                  response to remote party for ALERT
 *                  ICF_SSA_PRACK (SIP PRACK to be sent)
 *
 ******************************************************************************/
icf_return_t icf_cc_send_ssa_alert_prov_resp(
        INOUT icf_cc_pdb_st *p_cc_pdb)
{
    icf_return_t                     ret_val = ICF_SUCCESS;


    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb)


    p_cc_pdb->p_internal_msg->msg_hdr.msg_id = ICF_NW_OTG_ALERT_PRACK;
    p_cc_pdb->p_internal_msg->msg_hdr.payload_length = 0;


    /*Call entry point of SSA*/
    ret_val = icf_ssa_process_mesg(p_cc_pdb->p_internal_msg);
    /* Reset the sdp changed flag */
    p_cc_pdb->p_call_ctx->sdp_changed = ICF_FALSE;
    if(ICF_FAILURE == ret_val)
    {
        ICF_CC_TRACE(p_cc_pdb, ICF_INTERFACE_FAILURE)
    }

    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb)
    return ret_val;
}


/*******************************************************************************
 *
 * FUNCTION:        icf_cc_send_ssa_media_prov_resp_ack
 *
 * DESCRIPTION:     Invoked by CC toward SSA to acknowledge
 *                  provisional/reliable response received from remote party.
 *                  ICF_SSA_PRACK_ACK (SIP ACK to be sent for PRACK)
 *
 ******************************************************************************/
icf_return_t icf_cc_send_ssa_media_prov_resp_ack(
        INOUT icf_cc_pdb_st *p_cc_pdb)
{
    icf_nw_otg_prack_ack_st          prack_ack;
    icf_return_t                     ret_val = ICF_SUCCESS;


    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb)

    p_cc_pdb->p_internal_msg->msg_hdr.msg_id =
         ICF_NW_OTG_MEDIA_PARAM_PRACK_ACK;

    p_cc_pdb->p_internal_msg->msg_hdr.payload_length = sizeof(prack_ack);

    prack_ack.bitmask = ICF_NULL;
    prack_ack.status = ICF_TRUE;

    if(ICF_TRUE == p_cc_pdb->media_to_be_sent)
    {
        /*Set corresponding bitmask */
        prack_ack.bitmask |= ICF_NW_OTG_PRACK_ACK_SDP;
        /* Reset the bit in pdb */
        p_cc_pdb->media_to_be_sent = ICF_FALSE;

    }

    /*Set pointer of message to SSA in msg_data*/
    p_cc_pdb->p_internal_msg->p_msg_data = (icf_uint8_t *)&prack_ack;


    /*Call entry point of SSA*/
    ret_val = icf_ssa_process_mesg(p_cc_pdb->p_internal_msg);

    /*  Rel 8.2 changes Start */
    /*  in case of Tunnel mode On
     *  send ICF_PRACK_RESP_SENT_IND to application
     */
ICF_CHECK_TUNNEL_MODE_START(p_cc_pdb->p_glb_pdb,ICF_TRUE)
    if(ICF_SUCCESS == ret_val)
    {
        if(ICF_TRUE == p_cc_pdb->media_to_be_sent)
        {
            p_cc_pdb->sdp_present = ICF_TRUE;
        }
        else
        {
            p_cc_pdb->sdp_present = ICF_FALSE;
        }
        ret_val = icf_cc_send_pa_prack_resp_sent_ind(p_cc_pdb);
        if(ICF_FAILURE ==ret_val)
        {
            ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb)
            return ret_val;
        }
    }
ICF_CHECK_TUNNEL_MODE_END


    /* Reset the sdp changed flag */
    p_cc_pdb->p_call_ctx->sdp_changed = ICF_FALSE;
    if(ICF_FAILURE == ret_val)
    {
        ICF_CC_TRACE(p_cc_pdb, ICF_INTERFACE_FAILURE)
    }

    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb)
    return ret_val;
}

/*******************************************************************************
 *
 * FUNCTION:        icf_cc_send_ssa_alert_prov_resp_ack
 *
 * DESCRIPTION:     Invoked by CC toward SSA to acknowledge
 *                  provisional/reliable response received from remote party.
 *                  ICF_SSA_PRACK_ACK (SIP ACK to be sent for PRACK)
 *
 ******************************************************************************/
icf_return_t icf_cc_send_ssa_alert_prov_resp_ack(
        INOUT icf_cc_pdb_st *p_cc_pdb)
{
    icf_nw_otg_prack_ack_st          prack_ack;
    icf_return_t                     ret_val = ICF_SUCCESS;


    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb)

    p_cc_pdb->p_internal_msg->msg_hdr.msg_id =
         ICF_NW_OTG_ALERT_PRACK_ACK;
    p_cc_pdb->p_internal_msg->msg_hdr.payload_length = sizeof(prack_ack);

    prack_ack.bitmask = ICF_NULL;
	prack_ack.status = ICF_TRUE;

    if(ICF_TRUE == p_cc_pdb->media_to_be_sent)
    {
        prack_ack.bitmask |= ICF_NW_OTG_PRACK_ACK_SDP;

        /* reset media_to_be_sent flag */
        p_cc_pdb->media_to_be_sent = ICF_FALSE;
    }

    /*Set pointer of message to SSA in msg_data*/
    p_cc_pdb->p_internal_msg->p_msg_data = (icf_uint8_t *)&prack_ack;


    /*Call entry point of SSA*/
    ret_val = icf_ssa_process_mesg(p_cc_pdb->p_internal_msg);
   
    /*  Rel 8.2 changse start*/
    /*  In case of tunnel mode send ICF_PRACK_RESP_SENT_IND
     *  to application
     */
ICF_CHECK_TUNNEL_MODE_START(p_cc_pdb->p_glb_pdb,ICF_TRUE)
    if(ICF_SUCCESS == ret_val)
    {
        p_cc_pdb->sdp_present = p_cc_pdb->media_to_be_sent;
        ret_val = icf_cc_send_pa_prack_resp_sent_ind(p_cc_pdb);
        if(ICF_FAILURE ==ret_val)
        {
            ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb)
            return ret_val;
        }
    }
ICF_CHECK_TUNNEL_MODE_END
    /*  Rel 8.2 changes End */

    /* Reset the sdp changed flag */
    p_cc_pdb->p_call_ctx->sdp_changed = ICF_FALSE;
    
    if(ICF_FAILURE == ret_val)
    {
        ICF_CC_TRACE(p_cc_pdb, ICF_INTERFACE_FAILURE)
    }

    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb)
    return ret_val;
}



/*******************************************************************************
 *
 * FUNCTION:        icf_cc_send_ssa_connect
 *
 * DESCRIPTION:     Invoked by the call control when the call is accepted by
 *                  IP Packet Phone Signaling Toolkit user and results in
 *                  Connect being sent to SSA.The CC shall also indicate
 *                  whether the Media needs to be sent in this or not.
 *                  ICF_SSA_CONNECT (SIP 200 OK to be sent for INVITE)
 *
 ******************************************************************************/
icf_return_t icf_cc_send_ssa_connect(
        INOUT icf_cc_pdb_st *p_cc_pdb)
{
    icf_nw_otg_connect_st            call_connect;
    icf_return_t                     ret_val = ICF_SUCCESS;


    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb)

    p_cc_pdb->p_internal_msg->msg_hdr.msg_id = ICF_NW_OTG_CONNECT;
    p_cc_pdb->p_internal_msg->msg_hdr.payload_length = sizeof(call_connect);


    call_connect.bitmask = ICF_NULL;
    
    if((ICF_FALSE == p_cc_pdb->media_to_be_sent)||
       (ICF_CC_FINAL_SDP_SENT & p_cc_pdb->p_call_ctx->common_bitmask))
    {
    	call_connect.bitmask |= ICF_NW_OTG_CONNECT_WITHOUT_SDP;
    }
    else
    {
        /* reset media_to_be_sent flag */
        p_cc_pdb->media_to_be_sent = ICF_FALSE;
    }
    /* Issuing the SDP related trace only in case of 
       non tunnel mode.
    */   
ICF_CHECK_TUNNEL_MODE_START(p_cc_pdb->p_glb_pdb,ICF_FALSE)
    ICF_CC_APP_TRACE(p_cc_pdb,ICF_TRACE_LOCAL_SDP_ANSWER)
ICF_CHECK_TUNNEL_MODE_END

    /*Set pointer of message to SSA in msg_data*/
    p_cc_pdb->p_internal_msg->p_msg_data = (icf_uint8_t *)&call_connect;


    /*Call entry point of SSA*/
    ret_val = icf_ssa_process_mesg(p_cc_pdb->p_internal_msg);

    ICF_CC_RESET_COMMON_BITMASK(p_cc_pdb, ICF_CC_FINAL_SDP_SENT)
    
    /* Reset the sdp changed flag */
    p_cc_pdb->p_call_ctx->sdp_changed = ICF_FALSE;

    if(ICF_FAILURE == ret_val)
    {
        ICF_CC_TRACE(p_cc_pdb, ICF_INTERFACE_FAILURE)
    }

    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb)
    return ret_val;
}


/*******************************************************************************
 *
 * FUNCTION:        icf_cc_send_ssa_connect_ack
 *
 * DESCRIPTION:     Invoked by the Call Control to acknowledge call connect
 *                  received from SSA.
 *                  ICF_SSA_CONNECT_ACK (SIP ACK for 200 OK of INVITE)
 *
 ******************************************************************************/
icf_return_t icf_cc_send_ssa_connect_ack(
        INOUT icf_cc_pdb_st *p_cc_pdb)
{
    icf_nw_otg_connect_ack_st         connect_ack;
    icf_return_t                     ret_val = ICF_SUCCESS;


    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb)

    p_cc_pdb->p_internal_msg->msg_hdr.msg_id = ICF_NW_OTG_CONNECT_ACK;
    p_cc_pdb->p_internal_msg->msg_hdr.payload_length = sizeof(connect_ack);

    connect_ack.bitmask = ICF_NULL;

    if(ICF_TRUE == p_cc_pdb->media_to_be_sent)
    {
        connect_ack.bitmask |= ICF_NW_OTG_CONNECT_ACK_WITH_SDP;
        /* reset media_to_be_sent flag */
        p_cc_pdb->media_to_be_sent = ICF_FALSE;

    }

    /*Set pointer of message to SSA in msg_data*/
    p_cc_pdb->p_internal_msg->p_msg_data = (icf_uint8_t *)&connect_ack;


    /*Call entry point of SSA*/
    ret_val = icf_ssa_process_mesg(p_cc_pdb->p_internal_msg);

    if(ICF_FAILURE == ret_val)
    {
        ICF_CC_TRACE(p_cc_pdb, ICF_INTERFACE_FAILURE)
    }
    else
    {
        /* Reset the sdp changed flag */
        p_cc_pdb->p_call_ctx->sdp_changed = ICF_FALSE;
        
       /*twc_update handling*/
        /*If common_bitmask is ICF_CALL_MOD_REQ_WO_SDP then don't reset the flag*/ 
        if (ICF_NULL == ICF_CC_CHECK_COMMON_BITMASK(p_cc_pdb,ICF_CALL_MOD_REQ_WO_SDP))
        {
            /*SPR 19491 The call_modify_req_ongoing flag shall be set in
             * tunnel mode only and default it is false. So if call modify
             * UPDATE with SDP is send to network and it's response is awaited
             and we have recv 200OK for INV and sending ACK then basically we
             should not be reseting signalling ongoing so that CM can route 
             200OK for UPDATE recv after sending ACK can be processed by CC */
            if(ICF_FALSE == p_cc_pdb->p_call_ctx->call_modify_req_ongoing)
            {    
                /* Return status as signalling done to CM */
                ICF_CC_SET_STATUS(p_cc_pdb, ICF_STATUS_SIGNAL_ONGOING_ENDED)

                /* Reset signalling ongoing flag */
                ICF_CC_RESET_SIG_ONGOING(p_cc_pdb)
            }    
        }

        /* Set the stat for successfull establishment of call */
        ICF_STAT(p_cc_pdb->p_glb_pdb,
                     ICF_STAT_SUCC_CALLS_EST)

        /* Set the stat for successfull establishment of call */
        ICF_STAT(p_cc_pdb->p_glb_pdb,
                     ICF_STAT_SUCC_OTG_CALLS_EST)
         
        /* bitmask set after icf_cc_validate_ith_level_m_line_check
           changes made due to forking and multiple ans */ 
	/*ICF_CC_SET_COMMON_BITMASK(p_cc_pdb,ICF_CALL_CONNECT_DONE)*/
    }

	/* Fix for CSR 1-6851629: SPR 19297 */
	/* Set p_initial_sip_ctx to NULL at this point. This is required so that 
     * this freed pointer is not accessed later if CC mistakenly requests SSA
	 * to send CANCEL instead of BYE.
	 */
	p_cc_pdb->p_call_ctx->p_initial_sip_ctx = ICF_NULL;
    
    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb)
    return ret_val;
}

/*******************************************************************************
 *
 * FUNCTION:        icf_cc_send_ssa_force_clear
 *
 * DESCRIPTION:     Invoked by the Call Control to forceflly clear call
 *                  in SSA. CC will not expect any response now from SSA
 *
 ******************************************************************************/
icf_return_t icf_cc_send_ssa_force_clear(
        INOUT icf_cc_pdb_st *p_cc_pdb)
{
    icf_return_t                     ret_val = ICF_SUCCESS;


    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb)
    /*Send forceclear indication to SSA in case NW cleared flag is not set*/ 
    if (ICF_NULL == ICF_CC_CHECK_CALL_CLR_STATUS(p_cc_pdb,
                ICF_CALL_CLEAR_STATUS_NW_CLEARED))
    {
        p_cc_pdb->p_internal_msg->msg_hdr.msg_id =
            ICF_FORCEFUL_CALL_CLEAR_IND;

        p_cc_pdb->p_internal_msg->msg_hdr.payload_length = 0;

        /*Call entry point of SSA*/
        ret_val = icf_ssa_process_mesg(p_cc_pdb->p_internal_msg);

        if(ICF_FAILURE == ret_val)
        {
            ICF_CC_TRACE(p_cc_pdb, ICF_INTERFACE_FAILURE)
        }
      
        /* SPR 20111 : If invokation of forceful call clear, releases
         * the SSA context then the cross reference will be reset by
         * SSA and in case it is not released then the cross referecnce
         * should not be reset here and it should be released through
         * the function icf_cc_send_ssa_detach_call_ctx.
         */  
        /*p_cc_pdb->p_call_ctx->p_sip_ctx = ICF_NULL;*/
  
        /* We wouldn't wait for any responses from network once forceful clear 
           has been sent to SSA so set the NW cleared flag*/ 
        ICF_CC_SET_CALL_CLR_STATUS(p_cc_pdb,
                ICF_CALL_CLEAR_STATUS_NW_CLEARED) 
    }

    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb)
    return ret_val;
}

/*******************************************************************************
 *
 * FUNCTION:        icf_cc_send_ssa_redirect_ack
 *
 * DESCRIPTION:     Invoked by the Call Control to acknowledge redirect
 *                  received from SSA.
 *                  ICF_SSA_REDIRECT_ACK (ACK for SIP 302 to be sent)
 *
 ******************************************************************************/
icf_return_t icf_cc_send_ssa_redirect_ack(
        INOUT icf_cc_pdb_st *p_cc_pdb)
{
    icf_return_t                     ret_val = ICF_SUCCESS;


    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb)

    p_cc_pdb->p_internal_msg->msg_hdr.msg_id = ICF_NW_OTG_CALL_REDIRECT_ACK;
    p_cc_pdb->p_internal_msg->msg_hdr.payload_length = 0;

    /*Call entry point of SSA*/
    ret_val = icf_ssa_process_mesg(p_cc_pdb->p_internal_msg);

    if(ICF_FAILURE == ret_val)
    {
        ICF_CC_TRACE(p_cc_pdb, ICF_INTERFACE_FAILURE)
    }
    /* Reset the sdp changed flag */
    p_cc_pdb->p_call_ctx->sdp_changed = ICF_FALSE;

    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb)
    return ret_val;
}


/*******************************************************************************
 *
 * FUNCTION:        icf_cc_send_ssa_reject
 *
 * DESCRIPTION:     Invoked by the call control when the incoming call is
 *                  rejected/cleared by IP Packet Phone Signaling Toolkit user
 *                  and results in 3xx/4xx/5xx being sent to Remote Party. The
 *                  SSA shall check the release cause in call context to send
 *                  appropriate error response.
 *                  ICF_SSA_REJECT(SIP 3xx/4xx/5xx be sent)
 *
 ******************************************************************************/
icf_return_t icf_cc_send_ssa_reject(
        INOUT icf_cc_pdb_st *p_cc_pdb)
{
    icf_nw_otg_reject_st             call_reject;
    icf_return_t                     ret_val = ICF_SUCCESS;


    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb)
    
    if ((ICF_NULL == ICF_CC_CHECK_CALL_CLR_STATUS(p_cc_pdb, 
                        ICF_CALL_CLEAR_STATUS_NW_CLEARED)))
    {  
        p_cc_pdb->p_internal_msg->msg_hdr.msg_id = ICF_NW_OTG_CALL_REJECT;
        p_cc_pdb->p_internal_msg->msg_hdr.payload_length = sizeof(call_reject);

        call_reject.response_code = ICF_NULL;

        if(ICF_NULL != p_cc_pdb->p_call_ctx->response_code)
        {
            call_reject.response_code = p_cc_pdb->p_call_ctx->response_code ;
        }

        call_reject.reason_code = p_cc_pdb->p_call_ctx->release_cause ;
        call_reject.response_code = p_cc_pdb->p_call_ctx->response_code;

        /*Set pointer of message to SSA in msg_data*/
        p_cc_pdb->p_internal_msg->p_msg_data = (icf_uint8_t *)&call_reject;


        /*Call entry point of SSA*/
        ret_val = icf_ssa_process_mesg(p_cc_pdb->p_internal_msg);

        /* Reset the sdp changed flag */
        p_cc_pdb->p_call_ctx->sdp_changed = ICF_FALSE;

        if(ICF_FAILURE == ret_val)
        {
            ICF_CC_TRACE(p_cc_pdb, ICF_INTERFACE_FAILURE)
                ret_val = icf_cc_send_ssa_force_clear(p_cc_pdb);
            *(p_cc_pdb->p_ecode) = ICF_ERROR_NONE;
            ret_val = ICF_FAILURE;
        }
    }

    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb)
    return ret_val;
}


/*******************************************************************************
 *
 * FUNCTION:        icf_cc_send_ssa_reject_ack
 *
 * DESCRIPTION:     Invoked by the call control to acknowledge final failure
 *                  response (remote reject) received from SSA.
 *                  ICF_SSA_REJECT_ACK(SIP ACK to be sent for 3xx/4xx/5xx)
 *
 ******************************************************************************/
icf_return_t icf_cc_send_ssa_reject_ack(
        INOUT icf_cc_pdb_st *p_cc_pdb)
{
    icf_return_t                     ret_val = ICF_SUCCESS;


    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb)

    p_cc_pdb->p_internal_msg->msg_hdr.msg_id = ICF_NW_OTG_CALL_REJECT_ACK;
    p_cc_pdb->p_internal_msg->msg_hdr.payload_length = 0;


    /*Call entry point of SSA*/
    ret_val = icf_ssa_process_mesg(p_cc_pdb->p_internal_msg);

    /* Reset the sdp changed flag */
    p_cc_pdb->p_call_ctx->sdp_changed = ICF_FALSE;

    if(ICF_FAILURE == ret_val)
    {
        ICF_CC_TRACE(p_cc_pdb, ICF_INTERFACE_FAILURE)
    }

    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb)
    return ret_val;
}


/*******************************************************************************
 *
 * FUNCTION:        icf_cc_send_ssa_cancel_req
 *
 * DESCRIPTION:     Invoked by CC toward SSA to send CANCEL for outgoing call.
 *                  ICF_SSA_ABORT (SIP CANCEL to be sent)
 *
 ******************************************************************************/
icf_return_t icf_cc_send_ssa_cancel_req(
        INOUT icf_cc_pdb_st *p_cc_pdb)
{
    icf_return_t                     ret_val = ICF_SUCCESS;


    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb)

    /*call_mod_cancel*/
    if(ICF_NULL != ICF_CC_CHECK_COMMON_BITMASK_2(p_cc_pdb,ICF_CALL_MOD_CANCEL_REQ))
    {
        /*Case when call modify cancel req is sent just after sending Re-Invite*/
        p_cc_pdb->p_internal_msg->msg_hdr.msg_id = ICF_NW_OTG_MEDIA_UPDATE_ABORT_REQ;
    }
    else
    {
        p_cc_pdb->p_internal_msg->msg_hdr.msg_id = ICF_NW_OTG_CALL_ABORT;
    }
    p_cc_pdb->p_internal_msg->msg_hdr.payload_length = 0;

    /* Following assignment in if statement will be made because 
     * whenever we will send CANCEL req to nw, we will try to fetch p_initial_sip_ctx, if it
     * is found.
     * it will be containing initial ssa ctxt that was saved while creating ssa ctxt before
     * sending INVITE in icf_ssa_process_call_setup_req.
     * p_cc_pdb->p_call_ctx->p_initial_sip_ctx will be assigned NULL value while giving connect
     * indication to PA.
     */
    if(ICF_NULL != p_cc_pdb->p_call_ctx->p_initial_sip_ctx)
    {
       p_cc_pdb->p_call_ctx->p_sip_ctx = 
                   p_cc_pdb->p_call_ctx->p_initial_sip_ctx;
    }

    /*Call entry point of SSA*/
    ret_val = icf_ssa_process_mesg(p_cc_pdb->p_internal_msg);

    if(ICF_FAILURE == ret_val)
    {
        if(ICF_NULL == ICF_CC_CHECK_COMMON_BITMASK_2(p_cc_pdb,ICF_CALL_MOD_CANCEL_REQ))
        {
            ICF_CC_TRACE(p_cc_pdb, ICF_INTERFACE_FAILURE)
            ret_val = icf_cc_send_ssa_force_clear(p_cc_pdb);
		*(p_cc_pdb->p_ecode) = ICF_ERROR_NONE;
			ret_val = ICF_SUCCESS;
        }
    }
    /* Reset the sdp changed flag */
    p_cc_pdb->p_call_ctx->sdp_changed = ICF_FALSE;

    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb)
    return ret_val;
}


/*******************************************************************************
 *
 * FUNCTION:        icf_cc_send_ssa_cancel_resp
 *
 * DESCRIPTION:     Invoked by CC toward SSA to send response for incoming call
 *                  rejection by peer.
 *                  ICF_SSA_ABORT_RESP (SIP 200 OK for CANCEL to be sent)
 *
 ******************************************************************************/
icf_return_t icf_cc_send_ssa_cancel_resp(
        INOUT icf_cc_pdb_st *p_cc_pdb)
{
    icf_return_t                     ret_val = ICF_SUCCESS;


    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb)
    
    /* 200 OK for cancle will be sent directly */
    p_cc_pdb->p_internal_msg->msg_hdr.msg_id = ICF_NW_OTG_CALL_ABORT_RESP;
    p_cc_pdb->p_internal_msg->msg_hdr.payload_length = 0;

    /*Call entry point of SSA*/
    ret_val = icf_ssa_process_mesg(p_cc_pdb->p_internal_msg);

    /* Reset the sdp changed flag */
    p_cc_pdb->p_call_ctx->sdp_changed = ICF_FALSE;

    if(ICF_FAILURE == ret_val)
    {
        ICF_CC_TRACE(p_cc_pdb, ICF_INTERFACE_FAILURE)
        ret_val = ICF_FAILURE;
    }

	/* If we are sending 487 for a Reinvite transaction then reset signaling ongoing */
	if(ICF_CC_14_AWT_CALL_MODIFY_RESP == ICF_CC_GET_CALL_STATE(p_cc_pdb) ||
		ICF_CC_15_AWT_CALL_MODIFY_ALERT_RESP == ICF_CC_GET_CALL_STATE(p_cc_pdb))
	{
		ICF_CC_RESET_SIG_ONGOING(p_cc_pdb)
	}

    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb)
    return ret_val;
}


/*******************************************************************************
 *
 * FUNCTION:        icf_cc_send_ssa_call_term_req
 *
 * DESCRIPTION:     Should be invoked when SIP CC wants to release the call and
 *                  indicate the same to the peer.
 *                  ICF_SSA_CALL_TERMINATE (SIP BYE to be sent)
 *
 ******************************************************************************/
icf_return_t icf_cc_send_ssa_call_term_req(
        INOUT icf_cc_pdb_st *p_cc_pdb)
{
    icf_return_t                     ret_val = ICF_SUCCESS;


    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb)

    p_cc_pdb->p_internal_msg->msg_hdr.msg_id = ICF_NW_OTG_CALL_TERMINATE;
    p_cc_pdb->p_internal_msg->msg_hdr.payload_length = 0;

    /*Call entry point of SSA*/
    ret_val = icf_ssa_process_mesg(p_cc_pdb->p_internal_msg);

    if(ICF_FAILURE == ret_val)
    {
        ICF_CC_TRACE(p_cc_pdb, ICF_INTERFACE_FAILURE)
       	ret_val = icf_cc_send_ssa_force_clear(p_cc_pdb);
		*(p_cc_pdb->p_ecode) = ICF_ERROR_NONE;
		ret_val = ICF_FAILURE;
    }
    else
    {
        /* As we are sending BYE to SSA, signallig ongoing flag should be
         * set to true */
        ICF_CC_SET_SIG_ONGOING(p_cc_pdb)
        
    }

#ifdef ICF_LOAD_STAT
    if(ICF_SUCCESS==ret_val)
    {
        p_cc_pdb->p_glb_pdb->load_data.otg_call_term_req++;
    }
#endif

    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb)
    return ret_val;
}


/*******************************************************************************
 *
 * FUNCTION:        icf_cc_send_ssa_call_term_rsp
 *
 * DESCRIPTION:     Invoked by the Call Control to indicate that release
 *                  procedure is completed on a given leg of the call.
 *                  ICF_SSA_CALL_TERMINATE_RESP (SIP 200 OK for BYE)
 *
 ******************************************************************************/
icf_return_t icf_cc_send_ssa_call_term_rsp(
        INOUT icf_cc_pdb_st *p_cc_pdb)
{
    icf_return_t                     ret_val = ICF_SUCCESS;


    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb)

    p_cc_pdb->p_internal_msg->msg_hdr.msg_id =
         ICF_NW_OTG_CALL_TERMINATE_RESP;

    p_cc_pdb->p_internal_msg->msg_hdr.payload_length = 0;


    /*Call entry point of SSA*/
    ret_val = icf_ssa_process_mesg(p_cc_pdb->p_internal_msg);

    if(ICF_FAILURE == ret_val)
    {
        ICF_CC_TRACE(p_cc_pdb, ICF_INTERFACE_FAILURE)
        ret_val = ICF_FAILURE;
    }

    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb)
    return ret_val;
}


/*******************************************************************************
 *
 * FUNCTION:        icf_cc_send_ssa_offer_chg_rsp
 *
 * DESCRIPTION:     Invoked by the Call Control to send offer change resp
 *
 ******************************************************************************/
icf_return_t icf_cc_send_ssa_offer_chg_rsp(
        INOUT icf_cc_pdb_st *p_cc_pdb)
{
    icf_nw_otg_media_change_resp_st  media_change_resp;
    icf_return_t                     ret_val = ICF_SUCCESS;

    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /* function entry trace */

    p_cc_pdb->p_internal_msg->msg_hdr.msg_id = ICF_NW_OTG_MEDIA_UPDATE_RESP;
    p_cc_pdb->p_internal_msg->msg_hdr.payload_length =
        sizeof(media_change_resp);


    /*Set pointer of message to SSA in msg_data*/
    p_cc_pdb->p_internal_msg->p_msg_data = (icf_uint8_t *)&media_change_resp;

    media_change_resp.status = ICF_TRUE;
    /*Reset the bitmask*/
    media_change_resp.bitmask = ICF_NULL;

    if((ICF_NULL == ICF_CC_CHECK_COMMON_BITMASK(p_cc_pdb,
                    ICF_CALL_MOD_REQ_WO_SDP)) ||
        (ICF_FALSE == p_cc_pdb->p_call_ctx->update_recv))
    {
        /*Set the local_sdp in media_change_resp */
        media_change_resp.bitmask = ICF_NW_OTG_SDP_PRESENT;
    }

    /*Set pointer of message to SSA in msg_data*/
    p_cc_pdb->p_internal_msg->p_msg_data = (icf_uint8_t *)&media_change_resp;
    /* This flag should be reset here as if update has been done 
     * before connect then we need not send update again */
    p_cc_pdb->p_call_ctx->update_reqd = ICF_FALSE;
    /*Call entry point of SSA*/
    ret_val = icf_ssa_process_mesg(p_cc_pdb->p_internal_msg);
    if(ICF_FAILURE == ret_val)
    {
        ICF_CC_TRACE(p_cc_pdb, ICF_INTERFACE_FAILURE)
        ret_val = ICF_FAILURE;
    }
      
    /* Reset the sdp changed flag */
    p_cc_pdb->p_call_ctx->sdp_changed = ICF_FALSE;
    /*  Reset the bit update_recv */
    p_cc_pdb->p_call_ctx->update_recv = ICF_FALSE;
    p_cc_pdb->p_call_ctx->is_call_modify_ongoing = ICF_FALSE;

    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb)
    return ret_val;
}


/*******************************************************************************
 *
 * FUNCTION:        icf_cc_send_ssa_offer_chg_err
 *
 * DESCRIPTION:     Invoked by the Call Control to send error resp for offer
 *                  change
 *
 ******************************************************************************/
icf_return_t icf_cc_send_ssa_offer_chg_err(
        INOUT icf_cc_pdb_st *p_cc_pdb)
{
    icf_nw_otg_media_change_resp_st  media_change_resp;
    icf_return_t                     ret_val = ICF_SUCCESS;

    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /* function entry trace */

    p_cc_pdb->p_internal_msg->msg_hdr.msg_id = ICF_NW_OTG_MEDIA_UPDATE_RESP;
    p_cc_pdb->p_internal_msg->msg_hdr.payload_length =
        sizeof(media_change_resp);


    media_change_resp.status = ICF_FALSE;
    /*Reset the bitmask*/
    media_change_resp.bitmask = ICF_NULL;

    /*Set the error cause in media_change_resp */
    media_change_resp.bitmask = ICF_NW_OTG_MC_RESP_REASON;
    media_change_resp.reason_type = p_cc_pdb->p_call_ctx->offer_failure_reason;

    if(p_cc_pdb->p_call_ctx->response_code)
    {
        media_change_resp.nw_response_code = p_cc_pdb->p_call_ctx->response_code;
        media_change_resp.bitmask |= ICF_NW_OTG_MC_RESP_NW_RESPONSE;
        p_cc_pdb->p_call_ctx->response_code = ICF_NULL;
    } 

    /*Set pointer of message to SSA in msg_data*/
    p_cc_pdb->p_internal_msg->p_msg_data = (icf_uint8_t *)&media_change_resp;


    /*Call entry point of SSA*/
    ret_val = icf_ssa_process_mesg(p_cc_pdb->p_internal_msg);
    if(ICF_FAILURE == ret_val)
    {
        ICF_CC_TRACE(p_cc_pdb, ICF_INTERFACE_FAILURE)
        ret_val = ICF_FAILURE;
    }
     /* Reset the sdp changed flag */
     p_cc_pdb->p_call_ctx->sdp_changed = ICF_FALSE;

    /* Reset the error reason */
    p_cc_pdb->p_call_ctx->offer_failure_reason = ICF_NULL;
    p_cc_pdb->p_call_ctx->is_call_modify_ongoing = ICF_FALSE;

    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb)
    return ret_val;
}


/*******************************************************************************
*
* FUNCTION NAME    :   icf_cc_ssa_send_update_msg
*
* DESCRIPTION  : This function triggers SSA to send UPDATE request on NW,
*                So it creates an internal message for SSA entry point function
*                for API ICF_NW_OTG_MEDIA_UPDATE.
*
* RETURNS:  
*
*******************************************************************************/
icf_return_t icf_cc_ssa_send_update_msg(
        INOUT  icf_cc_pdb_st *p_cc_pdb)
{
    icf_glb_pdb_st          *p_glb_pdb = ICF_NULL;
    icf_return_t            ret_val = ICF_SUCCESS;
    icf_nw_otg_media_change_req_st   media_change_req = {0,0,0,0}; 
    icf_uint8_t              ongoing_event = 0;

	media_change_req.bitmask = 0;
    media_change_req.call_hold = ICF_FALSE;
    media_change_req.mesg_type = 0;

    p_glb_pdb = p_cc_pdb->p_internal_msg->msg_hdr.p_glb_pdb;

    ICF_FUNCTION_ENTER(p_glb_pdb)

    ongoing_event =  ICF_CC_GET_ONGOING_EVENT(p_cc_pdb);    

    /* no need to fetch config data as present in p_cc_pdb
     */ 	

    /* Fill Message ID */
ICF_HANDLE_REINVITE_START
    /*p_cc_pdb->p_internal_msg->msg_hdr.msg_id = ICF_NW_OTG_MEDIA_UPDATE;*/
	p_cc_pdb->p_internal_msg->msg_hdr.msg_id = ICF_NW_OTG_CALL_MODIFY_REQ;

    media_change_req.bitmask = ICF_NULL;
    media_change_req.mesg_type = ICF_NULL;

	ICF_CC_SET_SIG_ONGOING(p_cc_pdb)
ICF_HANDLE_REINVITE_END

    /* This is SET to False so that SDP is picked from Call Context */
    media_change_req.call_hold = ICF_FALSE;

    /* CSR 1-6188936: SPR 18835: ICF_UPDATE_PRESENT_IN_ALLOW_HDR is 
     * changed to bitmask*/ 
    media_change_req.bitmask |= ICF_MESG_TYPE_PRESENT;

    /* CSR 1-7047249: (modified changes)
     * in case of Fax Request , pick the method from fax req method and
     * in case of reINVITE, pick the method from call modify method
     */ 
    if ((ICF_CC_PA_STOP_FAX_REQ == ongoing_event) || 
            (ICF_CC_PA_FAX_REQ == ongoing_event)) 
    {
        if ((ICF_CFG_CALL_MODIFY_METHOD_INVITE ==  
                    p_cc_pdb->p_config_data->fax_req_method) || 
                (ICF_UPDATE_PRESENT_IN_ALLOW_HDR != (ICF_UPDATE_PRESENT_IN_ALLOW_HDR & 
                                 p_cc_pdb->p_call_ctx->allowed_value)))  
        {
            media_change_req.mesg_type = ICF_NW_OTG_SEND_RE_INVITE;
        }
        else
        {
            media_change_req.mesg_type = ICF_NW_OTG_SEND_UPDATE;
        }
    }
    else
    {
        if ((ICF_CFG_CALL_MODIFY_METHOD_INVITE ==  
                    p_cc_pdb->p_config_data->call_modify_req_method) || 
                (ICF_UPDATE_PRESENT_IN_ALLOW_HDR != (ICF_UPDATE_PRESENT_IN_ALLOW_HDR & 
                                  p_cc_pdb->p_call_ctx->allowed_value)))  
        {
            media_change_req.mesg_type = ICF_NW_OTG_SEND_RE_INVITE;
        }
        else
        {
            media_change_req.mesg_type = ICF_NW_OTG_SEND_UPDATE;
        }
    }

     /* spr 17399 : fix modified,no special handling for Fax is required */
    /*
    if(ICF_CC_LOCAL_FAX_REQ ==
        (ICF_CC_LOCAL_FAX_REQ & p_cc_pdb->common_bitmask))
    {
        media_change_req.mesg_type = ICF_NW_OTG_SEND_RE_INVITE;
        media_change_req.bitmask |= ICF_MESG_TYPE_PRESENT;

        ICF_CC_RESET_PDB_COMMON_BITMASK(p_cc_pdb,ICF_CC_LOCAL_FAX_REQ)
    }
   */    
    /* Rel 8.1
     * if to tag is present in call context
     * then copy it in internal msg and
     * send the to tag to SSA
     * and free the memory of p_to_tag
     * in call ctxt and make it NULL
     */
    if(ICF_NULL != p_cc_pdb->p_call_ctx->p_to_tag)
    {
        media_change_req.bitmask |= ICF_TO_TAG_PRESENT;
        ICF_MEMGET(p_cc_pdb->p_glb_pdb,
               icf_port_strlen(p_cc_pdb->p_call_ctx->p_to_tag)+1,
               ICF_MEM_COMMON,
               media_change_req.p_to_tag,
               ICF_RET_ON_FAILURE,
               p_cc_pdb->p_ecode,
               ret_val)
        icf_port_strcpy(media_change_req.p_to_tag,
		                p_cc_pdb->p_call_ctx->p_to_tag);
        ICF_MEMFREE(p_cc_pdb->p_glb_pdb,
                 p_cc_pdb->p_call_ctx->p_to_tag,
                 ICF_MEM_COMMON,
                 p_cc_pdb->p_ecode, ret_val)
    }

    p_cc_pdb->p_internal_msg->msg_hdr.payload_length = 
        ICF_PORT_SIZEOF(icf_nw_otg_media_change_req_st);

    p_cc_pdb->p_internal_msg->p_msg_data = (icf_uint8_t *)&media_change_req;

    /* Send to SSA */
    ret_val = icf_ssa_process_mesg(p_cc_pdb->p_internal_msg);
    /* Reset the sdp changed flag */
    p_cc_pdb->p_call_ctx->sdp_changed = ICF_FALSE;

/*Change Start Rel8.1: In precondition outgoing call after
  sending the UPDATE for precondition negotiation set the flag
  offer received to TRUE so that offer can be expected from SIP UE
  in tunnel mode */
ICF_CHECK_TUNNEL_MODE_START(p_cc_pdb->p_glb_pdb, ICF_TRUE)
    if(ICF_SUCCESS == ret_val)
    {   
ICF_CHECK_QOS_SUPPORT_START(p_cc_pdb->p_glb_pdb, ICF_TRUE)
       p_cc_pdb->p_call_ctx->offer_received = ICF_TRUE; 
ICF_CHECK_QOS_SUPPORT_END
    }
ICF_CHECK_TUNNEL_MODE_END
/*Change End Rel8.1 */    

    ICF_FUNCTION_EXIT(p_glb_pdb)
    
    return ret_val;    
}


/*******************************************************************************
 *
 * FUNCTION:        icf_cc_send_ssa_alert_prack_err
 *
 * DESCRIPTION:    Invoked by CC toward SSA to send error for the offer 
 *                 received  in media PRACK from remote party.
 *                 4xx/5xx to be sent for PRACK 
 *
 ******************************************************************************/
icf_return_t icf_cc_send_ssa_alert_prack_err(
        INOUT icf_cc_pdb_st *p_cc_pdb)
{
    icf_nw_otg_prack_ack_st          prack_ack;
    icf_return_t                     ret_val = ICF_SUCCESS;


    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb)

    p_cc_pdb->p_internal_msg->msg_hdr.msg_id =
         ICF_NW_OTG_ALERT_PRACK_ACK;

    p_cc_pdb->p_internal_msg->msg_hdr.payload_length = sizeof(prack_ack);

    prack_ack.bitmask = ICF_NULL;
    
    prack_ack.status = ICF_FALSE;

    prack_ack.reason_type = p_cc_pdb->p_call_ctx->offer_failure_reason;
    prack_ack.bitmask = ICF_NW_OTG_PRACK_RESP_REASON;
    
    /*Set pointer of message to SSA in msg_data*/
    p_cc_pdb->p_internal_msg->p_msg_data = (icf_uint8_t *)&prack_ack;


    /*Call entry point of SSA*/
    ret_val = icf_ssa_process_mesg(p_cc_pdb->p_internal_msg);

    /*  Rel 8.2 changes Start */
    /*  In case of Tunnel mode  send
     *  ICF_PRACK_RESP_SENT_IND to application
     */
ICF_CHECK_TUNNEL_MODE_START(p_cc_pdb->p_glb_pdb, ICF_TRUE)
    if(ICF_SUCCESS == ret_val)
    {
        ret_val = icf_cc_send_pa_prack_resp_sent_ind(p_cc_pdb);
        if(ICF_FAILURE ==ret_val)
        {
            ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb)
            return ret_val;
        }
    }
ICF_CHECK_TUNNEL_MODE_END

    /* Reset the sdp changed flag */
    p_cc_pdb->p_call_ctx->sdp_changed = ICF_FALSE;

    /* Reset the error reason */
    p_cc_pdb->p_call_ctx->offer_failure_reason = ICF_NULL;

    if(ICF_FAILURE == ret_val)
    {
        ICF_CC_TRACE(p_cc_pdb, ICF_INTERFACE_FAILURE)
    }

    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb)
    return ret_val;
}

/*******************************************************************************
 *
 * FUNCTION:        icf_cc_send_ssa_info_req
 *
 * DESCRIPTION:     This funtion would send info request to SSA when the same is
 *                  received from PA 
 *
 ******************************************************************************/
icf_return_t icf_cc_send_ssa_info_req(
        INOUT icf_cc_pdb_st *p_cc_pdb)
{
    icf_return_t                     ret_val = ICF_SUCCESS;
    /*  KlocWorks Warning Removal */
    icf_nw_otg_info_req_st           info_req = {0,{0,"\0"},{0,"\0"},{0,"\0"}};
    icf_pa_cc_info_req_st            *p_pa_info_req = ICF_NULL;

    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb)

    p_pa_info_req = 
       (icf_pa_cc_info_req_st *)p_cc_pdb->p_glb_pdb->p_recv_msg->payload;
    
    /*  CSR 1-5576525 Merge */
    icf_port_memset(&info_req,0,sizeof(icf_nw_otg_info_req_st));
    
    p_cc_pdb->p_internal_msg->msg_hdr.msg_id = ICF_NW_OTG_INFO_REQ;
    p_cc_pdb->p_internal_msg->msg_hdr.payload_length = sizeof(info_req);
    p_cc_pdb->p_internal_msg->p_msg_data = (icf_uint8_t *)(&info_req);

    /* CSR 1-5576525 Merge */
    /* CSR 1-5576525: Copy the username in otg Info request if received 
     * from application
     */
    if(ICF_USER_NAME_PRESENT & p_pa_info_req->bit_mask)
    {
        icf_port_strcpy(info_req.username.str,p_pa_info_req->username.str);
        info_req.username.str_len = icf_port_strlen(p_pa_info_req->username.str);
        info_req.presence_flag |= ICF_OTG_INFO_USERNAME_PRESENT;
    }

    /*Call entry point of SSA*/
    ret_val = icf_ssa_process_mesg(p_cc_pdb->p_internal_msg);

    /* Reset the sdp changed flag */
    p_cc_pdb->p_call_ctx->sdp_changed = ICF_FALSE;
  
    if(ICF_FAILURE == ret_val)
    {
        ICF_CC_TRACE(p_cc_pdb, ICF_INTERFACE_FAILURE)
        ret_val = ICF_FAILURE;
    }

    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb)

    return ret_val;
}

/*******************************************************************************
 *
 * FUNCTION:        icf_cc_send_ssa_media_prack_err
 *
 * DESCRIPTION:    Invoked by CC toward SSA to send error for the offer 
 *                 received  in media PRACK from remote party.
 *                 4xx/5xx to be sent for PRACK 
 *
 ******************************************************************************/
icf_return_t icf_cc_send_ssa_media_prack_err(
        INOUT icf_cc_pdb_st *p_cc_pdb)
{
    icf_nw_otg_prack_ack_st          prack_ack;
    icf_return_t                     ret_val = ICF_SUCCESS;


    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb)

    p_cc_pdb->p_internal_msg->msg_hdr.msg_id =
         ICF_NW_OTG_MEDIA_PARAM_PRACK_ACK;

    p_cc_pdb->p_internal_msg->msg_hdr.payload_length = sizeof(prack_ack);

    prack_ack.bitmask = ICF_NULL;
    
    prack_ack.status = ICF_FALSE;

    prack_ack.reason_type = p_cc_pdb->p_call_ctx->offer_failure_reason;
    prack_ack.bitmask = ICF_NW_OTG_PRACK_RESP_REASON;
    
    /*Set pointer of message to SSA in msg_data*/
    p_cc_pdb->p_internal_msg->p_msg_data = (icf_uint8_t *)&prack_ack;


    /*Call entry point of SSA*/
    ret_val = icf_ssa_process_mesg(p_cc_pdb->p_internal_msg);
    
    /*  Rel 8.2 changes Start */
    /*  In case of Tunnel mode  send 
     *  ICF_PRACK_RESP_SENT_IND to application
     */
ICF_CHECK_TUNNEL_MODE_START(p_cc_pdb->p_glb_pdb, ICF_TRUE)
    if(ICF_SUCCESS == ret_val)
    {
        ret_val = icf_cc_send_pa_prack_resp_sent_ind(p_cc_pdb);
        if(ICF_FAILURE ==ret_val)
        {
            ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb)
            return ret_val;
        }
    }
ICF_CHECK_TUNNEL_MODE_END

    /* Reset the sdp changed flag */
    p_cc_pdb->p_call_ctx->sdp_changed = ICF_FALSE;

    /* reset the error reason */
    p_cc_pdb->p_call_ctx->offer_failure_reason = ICF_NULL;

    if(ICF_FAILURE == ret_val)
    {
        ICF_CC_TRACE(p_cc_pdb, ICF_INTERFACE_FAILURE)
    }

    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb)
    return ret_val;
}

/*******************************************************************************
 *
 * FUNCTION:        icf_cc_send_ssa_info_cfm
 *
 * DESCRIPTION:    Invoked by CC toward SSA to send info_cfm.
 *                 This function will pass the response code depending upon
 *                 whether result received is true or not. 
 *                  
 ******************************************************************************/
icf_return_t icf_cc_send_ssa_info_cfm(
                             INOUT icf_cc_pdb_st *p_cc_pdb)
{
    icf_nw_inc_info_cfm_st           info_cfm;
    icf_pa_cc_info_cfm_st            *p_info_cfm = ICF_NULL;
    icf_return_t                     ret_val = ICF_SUCCESS;

    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb)

    icf_port_memset(&info_cfm,0,ICF_PORT_SIZEOF(icf_nw_inc_info_cfm_st));

    p_cc_pdb->p_internal_msg->msg_hdr.msg_id = ICF_NW_INC_INFO_CFM;
    p_cc_pdb->p_internal_msg->msg_hdr.payload_length = sizeof(info_cfm);
    p_cc_pdb->p_internal_msg->p_msg_data = (icf_uint8_t *)(&info_cfm);

    p_info_cfm = (icf_pa_cc_info_cfm_st *)p_cc_pdb->p_glb_pdb->p_recv_msg->payload;

    info_cfm.bit_mask = 0;
    if (p_info_cfm->result)
    {
       /* if result is TRUE, then response code sent on n/w will be 200.*/
       info_cfm.response_code = 200;
    }
    else
    {
       /* if result is not true then response code will be filled, if provided by application
        * otherwise, response code will be 500, if error_cause provided by
        * appn, if error cause is also not there then response code will be 400.
        */
       if (p_info_cfm->bit_mask & ICF_INFO_CFM_RESPONSE_CODE_PRESENT)
       {
          info_cfm.response_code =  p_info_cfm->response_code;
       }
       else if(p_info_cfm->bit_mask & ICF_INFO_CFM_ERROR_CAUSE_PRESENT)
       {
          info_cfm.response_code = 500;
       }
       else
       {
          info_cfm.response_code = 400;
       }     
    }

    if (p_info_cfm->bit_mask & ICF_INFO_CFM_HEADER_LIST_PRESENT)
    {
	ret_val = icf_cmn_clone_header_list(
			p_cc_pdb->p_glb_pdb,
			&(p_info_cfm->header_list),
			&(info_cfm.header_list),
			p_cc_pdb->p_ecode);

	icf_cmn_free_header_list(p_cc_pdb->p_glb_pdb,
			&(p_info_cfm->header_list),
			p_cc_pdb->p_ecode);

	info_cfm.bit_mask |= ICF_NW_INC_INFO_CFM_HEADER_LIST_PRESENT;
    }

    if (p_info_cfm->bit_mask & ICF_INFO_CFM_TAG_LIST_PRESENT)
    {
	ret_val = icf_cmn_clone_tag_list(
			p_cc_pdb->p_glb_pdb,
			&(p_info_cfm->tag_list),
			&(info_cfm.tag_list),
			p_cc_pdb->p_ecode);

	icf_cmn_free_tag_list(p_cc_pdb->p_glb_pdb,
			&(p_info_cfm->tag_list),
			p_cc_pdb->p_ecode);

	info_cfm.bit_mask |= ICF_NW_INC_INFO_CFM_TAG_LIST_PRESENT;
    }

    if (p_info_cfm->bit_mask & ICF_INFO_CFM_BODY_LIST_PRESENT)
    {
	ret_val = icf_cmn_clone_body_list(
			p_cc_pdb->p_glb_pdb,
			&(p_info_cfm->body_list),
			&(info_cfm.body_list),
			p_cc_pdb->p_ecode);

	icf_cmn_free_msg_body_list(p_cc_pdb->p_glb_pdb,
			&(p_info_cfm->body_list),
			p_cc_pdb->p_ecode);

	info_cfm.bit_mask |= ICF_NW_INC_INFO_CFM_BODY_LIST_PRESENT;
    }

    /*Call entry point of SSA*/
    ret_val = icf_ssa_process_mesg(p_cc_pdb->p_internal_msg);

    if (info_cfm.bit_mask & ICF_NW_INC_INFO_CFM_HEADER_LIST_PRESENT) 
    {
	icf_cmn_free_header_list(p_cc_pdb->p_glb_pdb,
			&(info_cfm.header_list),
			p_cc_pdb->p_ecode);
    }
    if (info_cfm.bit_mask & ICF_NW_INC_INFO_CFM_TAG_LIST_PRESENT) 
    {
	icf_cmn_free_tag_list(p_cc_pdb->p_glb_pdb,
			&(info_cfm.tag_list),
			p_cc_pdb->p_ecode);
    }
    if (info_cfm.bit_mask & ICF_NW_INC_INFO_CFM_BODY_LIST_PRESENT) 
    {
	icf_cmn_free_msg_body_list(p_cc_pdb->p_glb_pdb,
			&(info_cfm.body_list),
			p_cc_pdb->p_ecode);
    }

    if(ICF_FAILURE == ret_val)
    {
        ICF_CC_TRACE(p_cc_pdb, ICF_INTERFACE_FAILURE)
    }

    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb)
    return ret_val;
}


/*******************************************************************************
 *
 * FUNCTION:        icf_cc_send_ssa_info_cfm_err
 *
 * DESCRIPTION:    Invoked by CC toward SSA to send info_cfm error reponse.
 *                 This function will pass the response code depending upon
 *                 whether it is due to timer expiry or info_cfm is already pending. 
 *                  
 ******************************************************************************/
icf_return_t icf_cc_send_ssa_info_cfm_err(
                             INOUT icf_cc_pdb_st *p_cc_pdb)
{
    icf_nw_inc_info_cfm_st           info_cfm;
    icf_return_t                     ret_val = ICF_SUCCESS;

    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb)
    icf_port_memset(&info_cfm,0,ICF_PORT_SIZEOF(icf_nw_inc_info_cfm_st));

    if(ICF_NULL != p_cc_pdb->p_call_ctx->p_sip_ctx)
    {
    p_cc_pdb->p_internal_msg->msg_hdr.msg_id = ICF_NW_INC_INFO_CFM;
    p_cc_pdb->p_internal_msg->msg_hdr.payload_length = sizeof(icf_nw_inc_info_cfm_st);
    p_cc_pdb->p_internal_msg->p_msg_data = (icf_uint8_t *)(&info_cfm);

    info_cfm.bit_mask = 0;
    switch(p_cc_pdb->p_call_ctx->info_failure_cause)
    {
       case ICF_CAUSE_TIMER_EXPIRY:
          info_cfm.response_code = 486;
          break;
 
       case ICF_CAUSE_INFO_CFM_PENDING:
          info_cfm.response_code = 400;
          break;
    } 

    /*Call entry point of SSA*/
    ret_val = icf_ssa_process_mesg(p_cc_pdb->p_internal_msg);

    if(ICF_FAILURE == ret_val)
    {
        ICF_CC_TRACE(p_cc_pdb, ICF_INTERFACE_FAILURE)
    }
   }

    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb)
    return ret_val;
}

/*******************************************************************************
 *
 * FUNCTION:        icf_cc_send_ssa_detach_call_ctx
 *
 * DESCRIPTION:     This function is used to send an detach call context to SSA
 *
 *
 ******************************************************************************/
icf_return_t icf_cc_send_ssa_detach_call_ctx(
                             INOUT icf_cc_pdb_st *p_cc_pdb)
{
    icf_return_t                     ret_val = ICF_SUCCESS;

    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb)

    p_cc_pdb->p_internal_msg->msg_hdr.msg_id = ICF_DETACH_CALL_CTX_WITH_SSA_CTX;
    p_cc_pdb->p_internal_msg->p_msg_data = ICF_NULL;

    /*Call entry point of SSA*/
    ret_val = icf_ssa_process_mesg(p_cc_pdb->p_internal_msg);

    if(ICF_FAILURE == ret_val)
    {
        ICF_CC_TRACE(p_cc_pdb, ICF_INTERFACE_FAILURE)
    }

    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb)
    return ret_val;
}



