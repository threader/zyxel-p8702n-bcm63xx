/*------------------------------------------------------------------------------
 *
 * File name        : icf_cc_fsm_03.c

 *
 * Purpose          : This file contains the event handlers in
 *                    ICF_CC_03_AWT_ALERT_RESP  state.
 *
 * Revision History :
 *
 * Date         Name            Ref#            Comments
 * --------     ------------    ----            ---------------
 * 10-Dec-2004  Umang Singh     Non Service LLD Initial 
 * 21-Dec-2004  Rohit Aggarwal                  Addition of traces, stats
 * 23-Dec-2004  Rohit Aggarwal                  Addition of traces, stats
 * 24-Dec-2004  Rohit Aggarwal                  Compilation fixes
 * 31-May-2005  Sumit Gupta     SPR 7629        Pa initiated release type flag 
 *                                              would bet set even if call 
 *                                              forwarding indication is sent 
 *                                              back to CM when call terminate 
 *                                              is received from PA
 * 08-Jun-2005  Jalaj Negi      IMS CLIENT      Added code for IMS CLIENT 
 * 10-Jun-2005  Sumit Gupta     SPR 7649        Corrected flags of pending 
 *                                              events
 * 10-Aug-2005	Rohit Aggarwal	ICF				Code changes specific to ICF
 * 22-Feb-2005	Amit Sharma                     Added code for auth support
 * 08-Mar-2006  Aman Aggarwal					ICF merging with ICF2.0
 * 26-Jun-2006  Amit Sharma     ICF 6.0 APIs    Structure changes for multiple
 *                                              m-lines
 * 2-Jan-2007   Priyanka Dua    ICF 6.3         Code changes for Forking and 
 *						Multiple answer
 * 26-Mar-2007  Deepti Goyal    ICF 6.4         Changes for media connect 
 *                                              request API from PA.
 * 04-Apr-2007  Deepti Goyal    ICF 6.3         Changes for media connect 
 *                                              request API from PA.
 * 11-Apr-2007  Deepti Goyal    ICF 6.3         Changes for codec change in 
 *                                              media connect request API.
 * 17-Apr-2007  Abhishek Dutta  ICF 6.3         Added the changes for 
 *                                              TWC-UPDATE Handling
 * 09-Jul-2007  Neha Choraria   ICF 7.0         Changes for Forking &
 *                                              Multiple Answer
 * 24-Jul-2007  Vidyut Gupta    ICF 6.4         Merge the changes for SPR 14905
 *                                              from Alcatel branch
 * 3-Apr-2008   Sumant Gupta    ICF 8.0         Rel7.1 Windows porting Merging
 * 03-Nov-2008  Rajiv Kumar     SPR 19188       Rel 7.1.2 SPR merging (SPR
 *                                              18963)
 * 03-Dec-2008  Tarun Gupta     SPR 19218       Merged CSR 1-6188936
 * 11-Dec-2008  Tarun Gupta     SPR 19218       Merged SPR 18097
 * 11-Feb-2009  Anurag Khare    ICF Rel 8.1     Changes for Call_modify
 *                                              Enhancements
 * 12-Feb-2009  Tarun Gupta     Rel 8.1 LLD     SDP Tunnelling Enhancements
 * 16-Feb-2009  Anurag Khare    Rel 8.1         Call modify enhancements
 *                                              in forking scenario
 * 18-Feb-2009  Abhishek Dhammawat Rel 8.1 LLD  SDP Tunnelling QOS feature
 * 19-Feb-2009  Anurag Khare    Rel 8.1         Call mod enhancements changes
 * 19-Feb-2009  Abhishek Dhammawat Rel 8.1 LLD  SDP Tunnelling QOS feature
 *                                              modified pa alert, connect,
 *                                              media connect handling
 * 04-Mar-2009  Tarun Gupta     Rel 8.1         Modified icf_cc_03_alrt_with_media,
 *                                              icf_cc_03_ssa_call_connect
 * 12-Mar-2009  Tarun Gupta     Rel 8.1         Modified icf_cc_03_pa_alert
 * 13-Mar-2009  Abhishek Dhammawat  Rel 8.1      Modified
 *                                              icf_cc_03_pa_media_connect_req
 * 18-Mar-2009 Anurag Khare     SPR 19395       changes done for SDP Tunnelling
 * 30-Mar-2009 Anurag Khare     Rel 8.2         changes done for Prack Enh
 * 7-Apr-2009 Abhishek Dhammawat SPR 19430      Modified icf_cc_03_pa_alert
 * 22-Apr-2009 Abhishek Dhammawat SPR 19473     Modified
 *                                              icf_cc_03_pa_call_modify_req
 * 30-Apr-2009 Abhishek Dhammawat SPR 19491     Modified icf_cc_03_pa_alert
 * 6-May-2009  Abhishek Dhammawat SPR 19574     Modified icf_cc_03_pa_alert
 * 25-May-2009 Anuradha Gupta     SPR 19672     CSR 1-6779001 Merging
 * 08-Jun-2009 Kamal Ashraf       SPR 19590     Modified icf_cc_03_pa_alert, 
 *                                              icf_cc_03_pa_media_connect_req and
 *                                              icf_cc_03_pa_call_modify_req 
 * 20-Jun-2009 Anuradha Gupta     SPR 19760     Modified icf_cc_03_media_update_resp  
 * 05-Jul-2009 Anuradha Gupta     SPR 19754     Modified icf_cc_03_pa_alert,
 *                                              icf_cc_03_pa_media_connect_req  
 * 08-Oct-2009 Abhishek Dhammawat SPR 20063     Merged CSR 1-7507275
 * 10-Oct-2009 Rajiv Kumar        SPR 20101     Fix for SPR: 20101
 * 22-Oct-2009 Anurag Khare       SPR 20078     Fix for SPR: 20078
 * 27-Oct-2009 Abhishek Dhammawat SPR 20186     Modified
 *                                              icf_cc_03_forceful_call_clear
 *
 * Copyright (c) 2009, Aricent.
 *---------------------------------------------------------------------------*/


#include "icf_ecode.h"
#include "icf_cc_prototypes.h"
#include "icf_cc_macro.h"
#include "icf_ssa_interface.h"
#include "icf_dbm_prototypes.h"
#include "icf_common_prototypes.h"


/******************************************************************************
 *
 * FUNCTION:        icf_cc_03_ssa_media_param
 * 
 * DESCRIPTION:     This function is invoked when the CC is awaiting  
 *                  ALERT(SIP 180) from the SSA and it receives media param
 *                  (SIP 183 ) from SSA.
 *                  Current State: ICF_CC_03_AWT_ALERT_RESP
 *                  Current Event: ICF_CC_SSA_MEDIA_PARAM
 *
 *****************************************************************************/
icf_return_t icf_cc_03_ssa_media_param(
        INOUT icf_cc_pdb_st *p_cc_pdb)
{
    icf_return_t     ret_val = ICF_SUCCESS;
    icf_nw_inc_media_param_st   *p_nw_inc_media_param = ICF_NULL;

    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /* function entry trace */

    p_nw_inc_media_param = (icf_nw_inc_media_param_st *)
                                (p_cc_pdb->p_internal_msg->p_msg_data);

    /*
     * As the handling for this event is the same as in the state
     * ICF_CC_02_AWT_SSA_SETUP_RESP,call their
     * state handler
     */

    if (ICF_FORKED_RESP == (p_cc_pdb->p_call_ctx->common_bitmask & ICF_FORKED_RESP))
    {
ICF_CHECK_MULT_ANS_DISBL_START(p_cc_pdb->p_glb_pdb)

           ICF_PRINT(((icf_uint8_t*)"\n[CC]: Multiple Answer OFF in Forking - Clearing Call "));
           icf_cc_delete_inc_media_sdp_info_list(p_cc_pdb,&p_nw_inc_media_param->p_sdp_list);
           p_cc_pdb->p_call_ctx->release_cause = ICF_CAUSE_NETWORK_ERROR;
           ret_val = icf_cc_03_generic_err(p_cc_pdb);
           return ret_val;

ICF_CHECK_MULT_ANS_DISBL_END
     }

     ret_val = icf_cc_02_ssa_media_param(p_cc_pdb);

    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */
    return ret_val;
}


/******************************************************************************
 *
 * FUNCTION:        icf_cc_03_ssa_alert
 * 
 * DESCRIPTION:     This function is invoked when the CC is awaiting  
 *                  ALERT(SIP 180) from the SSA and it receives alert without 
 *                  media (SIP 180 without SDP) from SSA.
 *                  Current State: ICF_CC_03_AWT_ALERT_RESP
 *                  Current Event: ICF_CC_SSA_ALERT
 *
 *****************************************************************************/
icf_return_t icf_cc_03_ssa_alert(
        INOUT icf_cc_pdb_st *p_cc_pdb)
{
    icf_return_t     ret_val = ICF_SUCCESS;

ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /* function entry trace */
     
    /********* Changes for Forking & Multiple Answer Start (Rel7.0) ***********/

    if (ICF_FORKED_RESP == (p_cc_pdb->p_call_ctx->common_bitmask & ICF_FORKED_RESP))
    {
ICF_CHECK_MULT_ANS_DISBL_START(p_cc_pdb->p_glb_pdb)
 
           ICF_PRINT(((icf_uint8_t*)"\n[CC]: Multiple Answer OFF in Forking - Clearing Call "));
           p_cc_pdb->p_call_ctx->release_cause = ICF_CAUSE_NETWORK_ERROR;
           ret_val = icf_cc_03_generic_err(p_cc_pdb);
           return ret_val;

ICF_CHECK_MULT_ANS_DISBL_END

           p_cc_pdb->p_call_ctx->no_remote_sdp = ICF_TRUE;
	   /* Reset forked answer indication */
	   ICF_CC_RESET_COMMON_BITMASK(p_cc_pdb,ICF_FORKED_RESP)

     }

    /*********** Changes for Forking & Multiple Answer End (Rel7.0) ************/

       /*
        * As the handling for this event is the same in the state
        * ICF_CC_02_AWT_SSA_SETUP_RESP,call their
        * state handler
        */
        if(ICF_SUCCESS == ret_val)
	{
		ret_val = icf_cc_02_ssa_alert(p_cc_pdb);
	}

    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */
    return ret_val;
}

/******************************************************************************
 *
 * FUNCTION:        icf_cc_03_ssa_alrt_with_media
 * 
 * DESCRIPTION:     This function is invoked when the CC is awaiting  
 *                  ALERT(SIP 180) from the SSA and it receives alert with 
 *                  media (SIP 180 with SDP) from SSA.
 *                  Current State: ICF_CC_03_AWT_ALERT_RESP
 *                  Current Event: ICF_CC_SSA_ALERT_WITH_MEDIA
 *
 *****************************************************************************/
icf_return_t icf_cc_03_ssa_alrt_with_media(
        INOUT icf_cc_pdb_st *p_cc_pdb)
{
    icf_return_t     ret_val = ICF_SUCCESS;
	icf_boolean_t	 handle_sdp = ICF_FALSE;

	/* This variable will be holding pointer to the structure received 
	 * from SSA.
	 */
	icf_nw_inc_alert_media_st	*p_nw_inc_alert_media = ICF_NULL;

	p_nw_inc_alert_media = (icf_nw_inc_alert_media_st *)
							(p_cc_pdb->p_internal_msg->p_msg_data);

    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /* function entry trace */

ICF_CHECK_MULT_ANS_ENBL_START(p_cc_pdb->p_glb_pdb)
	/* 180 with different SDP from same UAS(Multiple answer) */
	handle_sdp = ICF_TRUE;            
ICF_CHECK_MULT_ANS_ENBL_END

        /********* Changes for Forking & Multiple Answer Start (Rel7.0) ***********/

         if (ICF_FORKED_RESP == (p_cc_pdb->p_call_ctx->common_bitmask & ICF_FORKED_RESP))
         {
ICF_CHECK_MULT_ANS_DISBL_START(p_cc_pdb->p_glb_pdb)
               
                  ICF_PRINT(((icf_uint8_t*)"\n[CC]: Multiple Answer OFF in Forking  - Clearing Call "));
                  icf_cc_delete_inc_media_sdp_info_list(p_cc_pdb,&p_nw_inc_alert_media->p_sdp_list);
                  p_cc_pdb->p_call_ctx->release_cause = ICF_CAUSE_NETWORK_ERROR;
                  ret_val = icf_cc_03_generic_err(p_cc_pdb);
                  return ret_val;

ICF_CHECK_MULT_ANS_DISBL_END

                  /* 180 with SDP from another UAS(Forking) */
		  handle_sdp = ICF_TRUE;            
		  /* Reset forked answer indication */
		  ICF_CC_RESET_COMMON_BITMASK(p_cc_pdb,ICF_FORKED_RESP)

          }

          /* If it is the first 180/183 then its forked_resp_id should be
             assigned 0 */

          if((ICF_NULL == p_nw_inc_alert_media->forked_resp_id) &&
                   (ICF_NULL == p_cc_pdb->p_call_ctx->forked_resp_counter))
                p_nw_inc_alert_media->forked_resp_id = ICF_NULL;

         /*********** Changes for Forking & Multiple Answer End (Rel7.0) ************/

	/* This field is set to false both in case of forked/non-forked response */ 
	p_cc_pdb->p_call_ctx->no_remote_sdp = ICF_FALSE;

    /*
     * Stop the call setup timer and start the alert timer
     */
    if (ICF_FAILURE == icf_cc_start_alert_timer(p_cc_pdb, ICF_FALSE, ICF_NULL))
    {
        ret_val = ICF_FAILURE;
    } /* if (ICF_FAILURE == icf_cc... */

    if((ICF_FALSE == p_cc_pdb->p_call_ctx->media_committed) || (ICF_TRUE == handle_sdp))
    {
                ICF_CC_TRACE(p_cc_pdb, ICF_TRACE_MEDIA_COMMITTED)

		/* This p_list_1 is being set because this should be initialised 
		 * with icf_inc_media_sdp_info_st before calling this function.
		 */
		p_cc_pdb->p_list_1 = p_nw_inc_alert_media->p_sdp_list; 

                /********* Changes for Forking & Multiple Answer Start (Rel7.0) ***********/

                p_cc_pdb->forked_resp_id =  p_nw_inc_alert_media->forked_resp_id;

                /*********** Changes for Forking & Multiple Answer End (Rel7.0) ***********/

		ret_val = icf_cc_validate_ith_level_m_line_check(p_cc_pdb);
		if (ICF_FAILURE == ret_val)
		{
			if (ICF_CC_STREAM_POS_VALIDATION_FAIL == 
					(p_cc_pdb->common_bitmask & ICF_CC_STREAM_POS_VALIDATION_FAIL))
			{
				ret_val = icf_cc_03_generic_err(p_cc_pdb);
			}
		}
		else
		{
                        /**** Changes for Forking & Multiple Answer Start (Rel7.0) ****/

                        p_cc_pdb->forked_resp_id =  ICF_NULL;

                        /***** Changes for Forking & Multiple Answer End (Rel7.0) *****/
            /* In tunnel mode, dont issue this trace to the application */
                        
ICF_CHECK_TUNNEL_MODE_START(p_cc_pdb->p_glb_pdb,ICF_FALSE) 
#ifdef ICF_TRACE_ENABLE    
            if(ICF_NULL != p_nw_inc_alert_media->p_sdp_list) 
			{
                            ICF_CC_APP_TRACE(p_cc_pdb,ICF_TRACE_REMOTE_SDP_ANSWER)
			}
#endif
			 /* This Function is deriving appropriate mms type.*/
			if (ICF_FALSE == p_cc_pdb->p_call_ctx->media_committed)
				ret_val = icf_cc_make_mms_type_for_rem_succ_ans(p_cc_pdb);
			else
			{
				icf_cc_reset_media_modify_type_for_all_streams(p_cc_pdb);
				ret_val = icf_cc_make_mms_type_for_remote_offer_change(p_cc_pdb);
			}	
ICF_CHECK_TUNNEL_MODE_END
ICF_CHECK_TUNNEL_MODE_START(p_cc_pdb->p_glb_pdb,ICF_TRUE)     
        /* In tunnel mode, even if media committed is true,
           then also this 180 with SDP is an answer only.
           So no local SDP is required from the application.
           We will set the media_modify_type field 
           which will determine whether the tunnelled MMS request
           and response will contain the tunnelled SDP or not.
        */
        ret_val = icf_cc_make_mms_type_for_answer_in_tunnel_mode(p_cc_pdb);
ICF_CHECK_TUNNEL_MODE_END
			
			if (ICF_CC_ALL_STREAMS_DELETED == 
				(p_cc_pdb->common_bitmask & ICF_CC_ALL_STREAMS_DELETED))
			{
				icf_cc_03_generic_err(p_cc_pdb);
			}
			else if (ICF_FAILURE != ret_val)
			{
				/* Send Modify Media Session to the MMI */
                /* IPTK will send the local codecs on the GCL iff 
				  p_prefered media profile is not null that means
				  ICF_NO_LOCAL_CODEC_IN_GCL is not set */
                
                /* In tunnel mode, no GCL is done, so not operating on the following bitmask */
ICF_CHECK_TUNNEL_MODE_START(p_cc_pdb->p_glb_pdb,ICF_FALSE)

				if(ICF_TRUE == p_cc_pdb->p_call_ctx->media_committed &&
                 ICF_NULL == (p_cc_pdb->p_call_ctx->common_bitmask & ICF_NO_LOCAL_CODEC_IN_GCL))
				{
					/* TBD - post 6.3 */
					/* Uncommented corresponding to spr 14476 fix.*/
			   	   p_cc_pdb->p_call_ctx->common_bitmask |= ICF_OFFER_CODECS_IN_GCL;
				}

                if(p_cc_pdb->p_call_ctx->common_bitmask & ICF_NO_LOCAL_CODEC_IN_GCL)
                {
                    p_cc_pdb->p_call_ctx->common_bitmask &= ~ICF_NO_LOCAL_CODEC_IN_GCL;
                }
ICF_CHECK_TUNNEL_MODE_END                

				if (ICF_FAILURE == icf_cc_send_mms_req(p_cc_pdb))
				{
					ret_val = ICF_FAILURE;
				} /* if (ICF_FAILURE == icf_c... */
                /* SPR 19218 : Merged SPR 18097 */
				else 
                {
			        /*
				     * SPR 18109:
				     * Update the states in the call context: If mms is sent.
				     * then set call state as ICF_CC_04_AWT_MMS_RESP
				     * and next state as ICF_CC_05_AWT_CONNECT
				     */
                    if(ICF_TRUE == p_cc_pdb->mms_sent)
                    {                     
				        ICF_CC_SET_CALL_STATE(p_cc_pdb,ICF_CC_04_AWT_MMS_RESP)
				        ICF_CC_SET_NEXT_STATE(p_cc_pdb,ICF_CC_05_AWT_CONNECT)
                    }
                    else
                    {
                        /*
						 * Send ALERT to PA
						 */
						if (ICF_FAILURE == icf_cc_send_pa_alert_ind(p_cc_pdb))
						{
							ret_val = ICF_FAILURE;
						} /* else if (ICF_FAILURE == icf_cc_... */
                        else
                        {
					        /* SPR 18109: Set the call state as 
        					 * ICF_CC_05_AWT_CONNECT as mms not sent
		        			 */
				        	ICF_CC_SET_CALL_STATE(p_cc_pdb,ICF_CC_05_AWT_CONNECT)
				        }
                    }
                }
			}
		}
    }
    else
    {
		icf_cc_delete_inc_media_sdp_info_list(
        	            p_cc_pdb,&(p_nw_inc_alert_media->p_sdp_list));

		/*
         * Send ALERT to PA
         */
        if (ICF_FAILURE == icf_cc_send_pa_alert_ind(p_cc_pdb))
        {
            ret_val = ICF_FAILURE;
        } /* else if (ICF_FAILURE == ipp... */
        else
        {
            /*
             * Update the states in the call context
             */
            ICF_CC_SET_CALL_STATE(p_cc_pdb,ICF_CC_05_AWT_CONNECT)
        } /* else */
    } /* else */
    
    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */
    return ret_val;
}

/******************************************************************************
 *
 * FUNCTION:        icf_cc_03_ssa_call_connect
 * 
 * DESCRIPTION:     This function is invoked when the CC is awaiting  
 *                  ALERT(SIP 180) from the SSA and it receives connect 
 *                  (SIP 200 OK ) from SSA.
 *                  Current State: ICF_CC_03_AWT_ALERT_RESP
 *                  Current Event: ICF_CC_SSA_CALL_CONNECT
 *
 *****************************************************************************/
icf_return_t icf_cc_03_ssa_call_connect(
        INOUT icf_cc_pdb_st *p_cc_pdb)
{
        icf_return_t     ret_val = ICF_SUCCESS;
	icf_boolean_t	 handle_sdp = ICF_FALSE;
        icf_boolean_t    mms_required = ICF_FALSE;  
	/* This variable will be holding pointer to the structure received 
	 * from SSA.
	 */
	icf_nw_inc_connect_st	*p_nw_inc_connect = ICF_NULL;

	p_nw_inc_connect = (icf_nw_inc_connect_st *)
							(p_cc_pdb->p_internal_msg->p_msg_data);

    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /* function entry trace */

        /********* Changes for Forking & Multiple Answer Start (Rel7.0) ***********/

        if (ICF_FORKED_RESP == (p_cc_pdb->p_call_ctx->common_bitmask & ICF_FORKED_RESP))
        {
ICF_CHECK_MULT_ANS_DISBL_START(p_cc_pdb->p_glb_pdb)
                    
               ICF_PRINT(((icf_uint8_t*)"\n[CC]:  Multiple Answer OFF in Forking-Clearing Call "));
               icf_cc_delete_inc_media_sdp_info_list(p_cc_pdb,&p_nw_inc_connect->p_sdp_list);
               p_cc_pdb->p_call_ctx->release_cause = ICF_CAUSE_NETWORK_ERROR;
               
               /*Fix for SPR : 19188 (Rel 7.1.2 SPR merging for SPR 18963)*/
               /* For the scenario in which forked 200 OK response is received
                * and Multiple answer feature is OFF IPTK should 
                * acknowledge the 200 OK response with ACK and subsequently
                * send BYE as IPTK doesnt support Multiple answer in forked
                * response if application have disabled the same.*/
               /*Previously IPTK is initiating CANCLE request instead of
                * BYE which is incorrect from both protocol point of view
                * and also from STACK perspective.*/
               /*Upon receiving the 200 OK response STACK has already
                * issued the sdf_cbk_uaCallAccepted callback towards IPTK
                * and if IPTK initiate CANCLE request from here ,CANCLE
                * flow (CANCLE --->
                         <------200
                         <------487) will not be completed correctly ie
               STACK will not issue the sdf_cbk_uaCallFailed callback on
               receiving 487 response (In this scenario stack is simply
               ignoring the 487 response)*/
               if (ICF_CC_SSA_CALL_CONNECT == p_cc_pdb->current_event)
               {
                   icf_cc_send_ssa_connect_ack(p_cc_pdb);
                   ICF_CC_SET_COMMON_BITMASK(p_cc_pdb,ICF_CALL_CONNECT_DONE)
               }                
               ret_val = icf_cc_03_generic_err(p_cc_pdb);
               /* Set network clear bit */
                ICF_CC_SET_CALL_CLR_STATUS(p_cc_pdb,
                   ICF_CALL_CLEAR_STATUS_NW_CLEARED)

               return ret_val;
                   
ICF_CHECK_MULT_ANS_DISBL_END
         }

         /* If it is the first resp then its forked_resp_id should be
            assigned 0 */

         if((ICF_NULL == p_nw_inc_connect->forked_resp_id) &&
              (ICF_NULL == p_cc_pdb->p_call_ctx->forked_resp_counter))
         p_nw_inc_connect->forked_resp_id = ICF_NULL;

        /*********** Changes for Forking & Multiple Answer End (Rel7.0) ************/
	if (ICF_NW_INC_CONNECT_WITH_SDP == 
		(p_nw_inc_connect->bitmask & ICF_NW_INC_CONNECT_WITH_SDP))
	{
		p_cc_pdb->sdp_present = ICF_TRUE;
                ICF_CHECK_MULT_ANS_ENBL_START(p_cc_pdb->p_glb_pdb)
		/* 200 with different SDP from same UAS(Multiple answer) */
		handle_sdp = ICF_TRUE;            
                ICF_CHECK_MULT_ANS_ENBL_END

		if (ICF_FORKED_RESP == (p_cc_pdb->p_call_ctx->common_bitmask & ICF_FORKED_RESP))
		{
			/* 200 with SDP from another UAS(Forking) */
			handle_sdp = ICF_TRUE;            
			/* Reset forked answer indication */
			ICF_CC_RESET_COMMON_BITMASK(p_cc_pdb,ICF_FORKED_RESP)
		}

		/* This field is set to false both in case of forked/non-forked response */ 
		p_cc_pdb->p_call_ctx->no_remote_sdp = ICF_FALSE;
	}
	else
	{
ICF_CHECK_MULT_ANS_ENBL_START(p_cc_pdb->p_glb_pdb)

                /****** Changes for Forking & Multiple Answer Start (Rel7.0) ******/

                /* If 200 OK received (w/o SDP) is a forked resp */
		if (ICF_FORKED_RESP == (p_cc_pdb->p_call_ctx->common_bitmask & ICF_FORKED_RESP))
		{
                        ICF_PRINT(((icf_uint8_t*)"\n[CC]: 200 OK received w/o SDP is a forked resp\
                                   - Clearing Call "));
                         p_cc_pdb->common_bitmask |= ICF_CC_CALL_CLEAR;
                         /* Reset forked answer indication */
                         ICF_CC_RESET_COMMON_BITMASK(p_cc_pdb,ICF_FORKED_RESP)
		}
                else /* If 200 OK received (w/o SDP) is not a forked resp */
                {
                         p_cc_pdb->forked_resp_id = p_nw_inc_connect->forked_resp_id;

                        /* checks if mms is required based on cmp between remote_sdp
                           & forked_sdp_info */

                        mms_required = icf_cc_check_mms_req_for_forked_resp(p_cc_pdb);
                        p_cc_pdb->sdp_present = ICF_TRUE;
                        p_cc_pdb->p_call_ctx->no_remote_sdp = ICF_FALSE;
                        handle_sdp = ICF_FALSE;
                        /* Reset forked answer indication */
                        ICF_CC_RESET_COMMON_BITMASK(p_cc_pdb,ICF_FORKED_RESP)
                }

                /****** Changes for Forking & Multiple Answer End (Rel7.0) ******/
  
ICF_CHECK_MULT_ANS_ENBL_END     
ICF_CHECK_MULT_ANS_DISBL_START(p_cc_pdb->p_glb_pdb)
		p_cc_pdb->sdp_present = ICF_FALSE;
ICF_CHECK_MULT_ANS_DISBL_END
	}

    /*
     * Send ACK for CALL CONNECT to SSA
     */
    if (ICF_FAILURE == icf_cc_send_ssa_connect_ack(p_cc_pdb))
    {
        ret_val = ICF_FAILURE;
    } /* if (ICF_FAILURE == icf_c... */
    else
    {
        /*  Once ACK has been successfully sent. so there is no need for 
            initial sip context. There would be one ssa_ctxt that would be in
            sip_ctxt. (fix for spr 18466) */
        p_cc_pdb->p_call_ctx->p_initial_sip_ctx = ICF_NULL;
        /*
         * Check if media has been committed
         */
        ICF_CC_TRACE(p_cc_pdb, ICF_TRACE_MEDIA_COMMITTED)
        if((ICF_FALSE == p_cc_pdb->p_call_ctx->media_committed) || (ICF_TRUE == handle_sdp)
		|| (ICF_TRUE == p_cc_pdb->p_call_ctx->no_remote_sdp)|| 
                (ICF_CC_CALL_CLEAR == (p_cc_pdb->common_bitmask & ICF_CC_CALL_CLEAR)))
        {
            /*
             * If SDP is not recvd in any backward message
             * including CALL CONNECT fail the call.
             * (1)Send call termination request to peer
             * (2)Send DMS request to MMI
             */
            /* Fill the value of p_cc_pdb->sdp_present */
            /* Check whether the 200 OK contains SDP or not */
            ICF_CC_TRACE(p_cc_pdb, ICF_TRACE_SDP_IN_PEER_MSG)
            if((ICF_FALSE == p_cc_pdb->sdp_present) || 
              (ICF_CC_CALL_CLEAR == (p_cc_pdb->common_bitmask & ICF_CC_CALL_CLEAR)))
            {
                ret_val = icf_cc_clear_call_no_remote_sdp(p_cc_pdb);
                return ret_val;
            } /* if(ICF_FALSE == p_cc_pdb->s... */
			else
			{

              	/* Set call connect done */
			    /*ICF_CC_SET_COMMON_BITMASK(p_cc_pdb,ICF_CALL_CONNECT_DONE)*/

				/* Sdp is present.*/

				/* This p_list_1 is being set because this should be initialised 
				 * with icf_inc_media_sdp_info_st before calling this function.
				 */
				p_cc_pdb->p_list_1 = p_nw_inc_connect->p_sdp_list;
                                p_cc_pdb->forked_resp_id = p_nw_inc_connect->forked_resp_id;
				ret_val = icf_cc_validate_ith_level_m_line_check(p_cc_pdb);
				if (ICF_FAILURE == ret_val)
				{
					if (ICF_CC_STREAM_POS_VALIDATION_FAIL == 
					(p_cc_pdb->common_bitmask & ICF_CC_STREAM_POS_VALIDATION_FAIL))
					{
						ret_val = icf_cc_03_generic_err(p_cc_pdb);
            /*  Once generic error is done then there is no need to change 
                the call state again as the code written down could again
                change the call state so return from here. (SPR 18466) */
            return ret_val;
					}
				}
				else
				{
                                        /* Set call connect done */
			                ICF_CC_SET_COMMON_BITMASK(p_cc_pdb,ICF_CALL_CONNECT_DONE)
                    /* In tunnel mode, dont issue this trace to the application */
ICF_CHECK_TUNNEL_MODE_START(p_cc_pdb->p_glb_pdb,ICF_FALSE)  
#ifdef ICF_TRACE_ENABLE
					if(ICF_NULL != p_nw_inc_connect->p_sdp_list)
					{
                                            ICF_CC_APP_TRACE(p_cc_pdb,ICF_TRACE_REMOTE_SDP_ANSWER)
					}
#endif                            
					/* This Function is deriving appropriate mms type.*/
			        if (ICF_FALSE == p_cc_pdb->p_call_ctx->media_committed)
						ret_val = icf_cc_make_mms_type_for_rem_succ_ans(p_cc_pdb);
					else
					{
						icf_cc_reset_media_modify_type_for_all_streams(p_cc_pdb);
						ret_val = icf_cc_make_mms_type_for_remote_offer_change(p_cc_pdb);
					}
ICF_CHECK_TUNNEL_MODE_END
ICF_CHECK_TUNNEL_MODE_START(p_cc_pdb->p_glb_pdb,ICF_TRUE)     
                        /* In tunnel mode, even if media committed is true,
                           then also this 200 with SDP is an answer only.
                           So no local SDP is required from the application.
                           We will set the media_modify_type field 
                           which will determine whether the tunnelled MMS request
                          and response will contain the tunnelled SDP or not.
                        */
                        ret_val = icf_cc_make_mms_type_for_answer_in_tunnel_mode(p_cc_pdb);
ICF_CHECK_TUNNEL_MODE_END    

					if (ICF_CC_ALL_STREAMS_DELETED == 
						 (p_cc_pdb->common_bitmask & ICF_CC_ALL_STREAMS_DELETED))
					{	
						ret_val = icf_cc_03_generic_err(p_cc_pdb);
					}
					else if (ICF_FAILURE != ret_val)
					{
						/* Send Modify Media Session to the MMI */
                        	/* IPTK will send the local codecs on the GCL iff 
						   p_prefered media profile is not null that means
						   ICF_NO_LOCAL_CODEC_IN_GCL is not set */
                
                        /* In tunnel mode, no GCL is done, so not operating
                           on the following bitmask.
                        */
ICF_CHECK_TUNNEL_MODE_START(p_cc_pdb->p_glb_pdb,ICF_FALSE)
						if(ICF_TRUE == p_cc_pdb->p_call_ctx->media_committed &&
                        ICF_NULL == (p_cc_pdb->p_call_ctx->common_bitmask & ICF_NO_LOCAL_CODEC_IN_GCL))
						{
							/* TBD - post 6.3 */
							/* Uncommented corresponding to spr 14476 fix.*/
				  		    p_cc_pdb->p_call_ctx->common_bitmask |= ICF_OFFER_CODECS_IN_GCL;
						}

                        if(p_cc_pdb->p_call_ctx->common_bitmask & ICF_NO_LOCAL_CODEC_IN_GCL)
						{
							p_cc_pdb->p_call_ctx->common_bitmask &= ~ICF_NO_LOCAL_CODEC_IN_GCL;
						}
ICF_CHECK_TUNNEL_MODE_END                        

						if (ICF_FAILURE == icf_cc_send_mms_req(p_cc_pdb))
						{
							ret_val = ICF_FAILURE;
						} /* if (ICF_FAILURE == icf_c... */
						else if(ICF_TRUE == p_cc_pdb->mms_sent)
						{
							ICF_CC_SET_ONGOING_EVENT(p_cc_pdb,ICF_CC_SSA_CALL_CONNECT)

							ret_val = icf_cc_start_media_timer(p_cc_pdb);
							/*
							 * Update the states in the call context
							 */
							ICF_CC_SET_CALL_STATE(p_cc_pdb,ICF_CC_04_AWT_MMS_RESP)
							ICF_CC_SET_NEXT_STATE(p_cc_pdb,ICF_CC_06_CONNECTED)
						}
                        /*
                         * Stop the call setup timer 
                         */
#if 0
                        else{

                            if (ICF_FAILURE == icf_cc_stop_current_timer(p_cc_pdb))
                            {
                                ret_val = ICF_FAILURE;
                            } /* if (ICF_FAILURE == icf_c... */
                            /*
                             * Send CONNECT to PA
                             */
                            else if (ICF_FAILURE == icf_cc_send_pa_connect_ind(p_cc_pdb))
                            {
                                ret_val = ICF_FAILURE;
                            } /* else if (ICF_FAILURE == ipp... */
                            else
                            {
                                /*
                                 * Update the states in the call context
                                 */
                                ICF_CC_SET_CALL_STATE(p_cc_pdb,ICF_CC_06_CONNECTED)
                            }
                        }
#endif
                    }
                }

			}/* End of if(ICF_FALSE == p_cc_pdb->sdp_pres..*/
        }
        
    /********** Changes for Forking & Multiple Answer Start (Rel7.0) **********/

    else if((ICF_TRUE == mms_required) && (ICF_FAILURE != ret_val))
    {
ICF_CHECK_MULT_ANS_ENBL_START(p_cc_pdb->p_glb_pdb)
ICF_CHECK_TUNNEL_MODE_START(p_cc_pdb->p_glb_pdb,ICF_FALSE)    
        if (ICF_FALSE == p_cc_pdb->p_call_ctx->media_committed)
		ret_val = icf_cc_make_mms_type_for_rem_succ_ans(p_cc_pdb);
	else
	{
		icf_cc_reset_media_modify_type_for_all_streams(p_cc_pdb);
		ret_val = icf_cc_make_mms_type_for_remote_offer_change(p_cc_pdb);
	}
ICF_CHECK_TUNNEL_MODE_END

ICF_CHECK_TUNNEL_MODE_START(p_cc_pdb->p_glb_pdb,ICF_TRUE)     
        /* In tunnel mode, even if media committed is true,
           then also this 200 with SDP is an answer only.
           So no local SDP is required from the application.
           We will set the media_modify_type field 
           which will determine whether the tunnelled MMS request
           and response will contain the tunnelled SDP or not.
        */
        ret_val = icf_cc_make_mms_type_for_answer_in_tunnel_mode(p_cc_pdb);
ICF_CHECK_TUNNEL_MODE_END 

	if (ICF_CC_ALL_STREAMS_DELETED == 
		(p_cc_pdb->common_bitmask & ICF_CC_ALL_STREAMS_DELETED))
	{	
		ret_val = icf_cc_03_generic_err(p_cc_pdb);
                return ret_val;
	}
        else if (ICF_FAILURE != ret_val)
	{
		/* Send Modify Media Session to the MMI */

                if (ICF_FAILURE == icf_cc_send_mms_req(p_cc_pdb))
                {
                    /* Mark ongoing event as ICF_CC_SSA_CALL_CONNECT */
                   
                    ICF_CC_SET_ONGOING_EVENT(p_cc_pdb,ICF_CC_SSA_CALL_CONNECT)

                    ret_val = ICF_FAILURE;
                 } 
                 else if(ICF_TRUE == p_cc_pdb->mms_sent)
                 {		
		     /* Start the media timer  */
		     ret_val = icf_cc_start_media_timer(p_cc_pdb);

                     /* Mark ongoing event as ICF_CC_SSA_CALL_CONNECT */
                  
                     ICF_CC_SET_ONGOING_EVENT(p_cc_pdb,ICF_CC_SSA_CALL_CONNECT)
                 
                     /* Update the states in the call context */
                  
                     ICF_CC_SET_CALL_STATE(p_cc_pdb,ICF_CC_04_AWT_MMS_RESP)
                     ICF_CC_SET_NEXT_STATE(p_cc_pdb,ICF_CC_06_CONNECTED)
                  }
           }
ICF_CHECK_MULT_ANS_ENBL_END
    }

    /********** Changes for Forking & Multiple Answer End (Rel7.0) **********/

    else if((ICF_TRUE == p_cc_pdb->p_call_ctx->media_committed) 
			&& (ICF_FALSE == p_cc_pdb->p_call_ctx->no_remote_sdp))
        {
			/* Media is Already commited.*/
            if (ICF_TRUE == p_cc_pdb->sdp_present)
            {
                icf_cc_delete_inc_media_sdp_info_list(
                        p_cc_pdb,&(p_nw_inc_connect->p_sdp_list));
            }
        }

            /*
             * Stop the call setup timer 
             */
   if (ICF_FAILURE != ret_val)
   {

       if((ICF_FALSE == p_cc_pdb->mms_sent) && (ICF_TRUE == p_cc_pdb->p_call_ctx->media_committed))
       {
           /* In tunnel mode, we do not send an auto UPDATE, thus the check for
              update_reqd field in not required.
           */   
ICF_CHECK_TUNNEL_MODE_START(p_cc_pdb->p_glb_pdb,ICF_FALSE)           
          if(ICF_FALSE == p_cc_pdb->p_call_ctx->update_reqd)
           {
               
             if (ICF_FAILURE == icf_cc_stop_current_timer(p_cc_pdb))
              {
                ret_val = ICF_FAILURE;
              } /* if (ICF_FAILURE == icf_c... */

           }
ICF_CHECK_TUNNEL_MODE_END
          /* In tunnel mode,stop the timer without the check for update_reqd */
ICF_CHECK_TUNNEL_MODE_START(p_cc_pdb->p_glb_pdb,ICF_TRUE)
    
          if (ICF_FAILURE == icf_cc_stop_current_timer(p_cc_pdb))
          {
              ret_val = ICF_FAILURE;
          }
ICF_CHECK_TUNNEL_MODE_END
            /*
             * Send CONNECT to PA
             */
            if (ICF_FAILURE == icf_cc_send_pa_connect_ind(p_cc_pdb))
            {
                ret_val = ICF_FAILURE;
            } /* else if (ICF_FAILURE == ipp... */
            else
            {
                /* In tunnel mode, we do not send an auto UPDATE, thus the check for
                   update_reqd field in not required.
                */   
ICF_CHECK_TUNNEL_MODE_START(p_cc_pdb->p_glb_pdb,ICF_FALSE)
				if(ICF_TRUE == p_cc_pdb->p_call_ctx->update_reqd ) 
	            {
		           /*Send UPDATE request*/
		           if (ICF_FAILURE == icf_cc_ssa_send_update_msg(p_cc_pdb))
		           {
			           ret_val = ICF_FAILURE;
		           }
                   /* Fix for CSR 1-6779001: SPR 19258 */
					/* Move to state 15 for ReINVITE and state 14 for UPDATE */
					else if ((ICF_CFG_CALL_MODIFY_METHOD_INVITE ==
									p_cc_pdb->p_config_data->call_modify_req_method) ||
							 (ICF_NULL == (ICF_UPDATE_PRESENT_IN_ALLOW_HDR &
										   	p_cc_pdb->p_call_ctx->allowed_value)))
					{
						ICF_CC_SET_CALL_STATE(p_cc_pdb, 
							ICF_CC_15_AWT_CALL_MODIFY_ALERT_RESP)
					}
		           else
		           {
			           ICF_CC_SET_CALL_STATE(p_cc_pdb,
					     ICF_CC_14_AWT_CALL_MODIFY_RESP)
		           }
	            } /* else */
	            else
	            {
		              ICF_CC_SET_CALL_STATE(p_cc_pdb,ICF_CC_06_CONNECTED)

                }
ICF_CHECK_TUNNEL_MODE_END
            
                /* In tunnel mode, just move to state 06 */
ICF_CHECK_TUNNEL_MODE_START(p_cc_pdb->p_glb_pdb,ICF_TRUE)
                    ICF_CC_SET_CALL_STATE(p_cc_pdb,ICF_CC_06_CONNECTED)
ICF_CHECK_TUNNEL_MODE_END
            }
        }
    }

    } /* else */
 
    if (ICF_FAILURE != ret_val)
    {

    /******* Changes for Forking & Multiple Answer Start (Rel7.0) *******/

    /* p_forked_resp_list is deleted after connect ind is sent
       to PA */
    ICF_PRINT(((icf_uint8_t*)"\n[CC]: Freeing Forked resp list 03 connect call "));
    ret_val = icf_cc_free_forked_resp_list(p_cc_pdb,p_cc_pdb->p_ecode);

    /******** Changes for Forking & Multiple Answer End (Rel7.0) ********/

    }
    else
    {
            icf_cc_delete_inc_media_sdp_info_list(p_cc_pdb,
                     &p_nw_inc_connect->p_sdp_list);
    }
        
	ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */
    return ret_val;
}



/******************************************************************************
 *
 * FUNCTION:        icf_cc_03_ssa_remote_reject
 * 
 * DESCRIPTION:     This function is invoked when the CC is awaiting 
 *                  ALERT(SIP 180) from the SSA and it receives Remote Call 
 *                  Reject from SSA (SIP 3xx/4xx/5xx received from peer).
 *                  Current State: ICF_CC_03_AWT_ALERT_RESP
 *                  Current Event: ICF_CC_SSA_REMOTE_REJECT
 *
 *****************************************************************************/
icf_return_t icf_cc_03_ssa_remote_reject(
        INOUT icf_cc_pdb_st *p_cc_pdb)
{
    icf_return_t     ret_val = ICF_SUCCESS;

    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /* function entry trace */

    /* If we are in this state due to 183  clear the call otherwise
     * handling should be same as when awaiting setup */
    if(ICF_FAILURE != icf_cc_check_remote_sdp_in_all_streams(p_cc_pdb))
    {
        /*
         * Stop the call setup timer and start the release timer
         */
        if (ICF_FAILURE == icf_cc_start_release_timer(p_cc_pdb))
        {
            ret_val = ICF_FAILURE;
        }
        /*
         * 1.Send ACK for call rejection
         * 2.Set release type as network initiated
         * 3.Call network clear handler
         * 4.Send a DMS request to MMI
         */
        else if (ICF_FAILURE == icf_cc_send_ssa_reject_ack(p_cc_pdb))
        {
            ret_val = ICF_FAILURE;
        }
        else
        {
            ICF_CC_TRACE(p_cc_pdb, ICF_TRACE_REL_TYPE_NW_INIT)
            ICF_CC_SET_RELEASE_TYPE(p_cc_pdb,ICF_REL_TYPE_NW_INITIATED) 

            if(ICF_FAILURE == icf_cc_nw_clear_handler(p_cc_pdb))
            {
                ret_val = ICF_FAILURE;
            }
            /*
             * Send a DMS request to MMI
             */
            else if(ICF_FAILURE == icf_cc_send_dms_req(p_cc_pdb))
            {
                ret_val = ICF_FAILURE;
            }
            else
            {
                /*
                 * Update the states in the call context
                 */
                ICF_CC_SET_CALL_STATE(p_cc_pdb,ICF_CC_07_AWT_DMS_RESP)
            }
        } /* else */
    }
    else
    {
        ret_val = icf_cc_02_ssa_remote_reject(p_cc_pdb);
    }


    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */
    return ret_val;
}


/******************************************************************************
 *
 * FUNCTION:        icf_cc_03_ssa_redirect
 * 
 * DESCRIPTION:     This function is invoked when the CC is awaiting 
 *                  ALERT(SIP 180) from the SSA and it receives reroute call
 *                  Req from SSA (SIP 302 received from peer).
 *                  Current State: ICF_CC_03_AWT_ALERT_RESP
 *                  Current Event: ICF_CC_SSA_REDIRECT
 *
 *****************************************************************************/
icf_return_t icf_cc_03_ssa_redirect(
        INOUT icf_cc_pdb_st *p_cc_pdb)
{
    icf_return_t     ret_val = ICF_SUCCESS;

    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /* function entry trace */

    /*
     * Send ACK for redirect request
    */
    if (ICF_FAILURE == icf_cc_send_ssa_redirect_ack(p_cc_pdb))
    {
        ret_val = ICF_FAILURE;
    } /* if (ICF_FAILURE == icf_c... */
    else if(ICF_FAILURE == icf_cc_validate_redirect_req(p_cc_pdb))
    {
        /* Validation failed clear the call 
         * Send DMS Request to MMI */
        p_cc_pdb->forceful_delete = ICF_FALSE;

        p_cc_pdb->p_call_ctx->release_cause = ICF_CALL_CLEAR_REASON_NW_ERROR;

        ICF_CC_SET_RELEASE_TYPE(p_cc_pdb, ICF_REL_TYPE_INTERNAL_INITIATED)
        ICF_CC_TRACE(p_cc_pdb, ICF_TRACE_REL_TYPE_INT_ERR)    
        /*
         * Stop the call setup timer and start the release timer
         */
        if (ICF_FAILURE ==icf_cc_start_release_timer(p_cc_pdb))
        {
            ret_val = ICF_FAILURE;
        }
        /*
         * Send a DMS request to MMI
         */
        else if (ICF_FAILURE == icf_cc_send_dms_req(p_cc_pdb))
        {
            ret_val = ICF_FAILURE;
        }
        /*
         * mark network clear
         */
        else if (ICF_FAILURE == icf_cc_nw_clear_handler(p_cc_pdb))
        {
            ret_val = ICF_FAILURE;
        }
        /*
         * Update the states in the call context
         */
        else 
        {
            ICF_CC_SET_CALL_STATE(p_cc_pdb,ICF_CC_07_AWT_DMS_RESP)
        } /* else */
    } /* if(ICF_FAILURE == icf_cc... */
    else
    {
            /*
             * Check if media has been committed.
             */
            ICF_CC_TRACE(p_cc_pdb, ICF_TRACE_MEDIA_COMMITTED)
            if(ICF_TRUE == p_cc_pdb->p_call_ctx->media_committed)
            {
                /*
                 * Send a DMS request to MMI
                 */
                /* Send DMS Request to MMI */
                p_cc_pdb->forceful_delete = ICF_FALSE;
                if (ICF_FAILURE == icf_cc_send_dms_req(p_cc_pdb))
                {
                    ret_val = ICF_FAILURE;
                } /* if (ICF_FAILURE == icf_c... */
                else
                {
                    /*
                     * Update next state as ICF_CC_01_AWT_CMS_RESP
                     */
                    ICF_CC_SET_NEXT_STATE(p_cc_pdb,ICF_CC_01_AWT_CMS_RESP)
                     /*
                      * Update ongoing event as ICF_CC_SSA_REDIRECT
                      */
                    ICF_CC_SET_ONGOING_EVENT(p_cc_pdb,ICF_CC_SSA_REDIRECT)    
                     /*
                      * Update the states in the call context
                      */
                    ICF_CC_SET_CALL_STATE(p_cc_pdb,ICF_CC_07_AWT_DMS_RESP)

     
                } /* else */
            } /* if(ICF_FALSE == p_cc_pdb->p... */
            else
            {
                /*
                 * Send call setup to SSA  which will send the INVITE on the
                 * network. 
                 */
                if(ICF_FAILURE == icf_cc_send_ssa_setup_req(p_cc_pdb))
                {
					/* Set call ctate as idle as we wouldn't be sending any 
                     * messages to ssa*/
                    ICF_CC_SET_CALL_STATE(p_cc_pdb,ICF_CC_00_IDLE)

                    ret_val = ICF_FAILURE;
                } /* if (ICF_FAILURE == icf_c... */
                else
                {
                    /*
                     * Update the states in the call context
                     */
                    ICF_CC_SET_CALL_STATE(p_cc_pdb,ICF_CC_02_AWT_SSA_SETUP_RESP)
                } /* else */
            } /* else */
    } /* else */

    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */
    return ret_val;
}


/******************************************************************************
 *
 * FUNCTION:        icf_cc_03_pa_call_term_req
 * 
 * DESCRIPTION:     This function is invoked when the CC is awaiting 
 *                  ALERT(SIP 180) from the SSA or ALERT RESP from
 *                  the PA(incoming call)and it receives Call 
 *                  Terminate Request from PA.
 *                  Current State: ICF_CC_03_AWT_ALERT_RESP
 *                  Current Event: ICF_CC_PA_CALL_TERMINATE_REQ
 *
 *****************************************************************************/
icf_return_t icf_cc_03_pa_call_term_req(
        INOUT icf_cc_pdb_st *p_cc_pdb)
{
    icf_return_t     ret_val = ICF_SUCCESS;
    icf_line_id_t    alternate_line = ICF_INVALID_LINE_ID;
    icf_uint8_t      trigger_cause = ICF_CAUSE_LINE_FREE;
    icf_uint32_t     status = ICF_STATUS_CONTINUE;
    
    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /* function entry trace */


    /* If PA has requested to present the call on an alternate line then
     * request DBM to find an alternate line to present the call on
     */ 
    /* PA can request for an alternate line only if PA is doing the line 
     * management. */
    /*Fix merged for CSR_1_5400409 , check for
     * icf_simul_call_line_arbitration is removed here */
    if ((ICF_CALL_CLEAR_REASON_ATTEMPT_ON_ALTERNATE_LINE == 
            p_cc_pdb->p_call_ctx->release_cause))
    {
        if(ICF_FAILURE == 
                icf_dbm_get_line_for_inc_call(p_cc_pdb->p_glb_pdb,
                    (icf_boolean_t)(ICF_TRUE),
                    &(p_cc_pdb->p_call_ctx->local_address),
                    &alternate_line,
                    &status,
                    p_cc_pdb->p_ecode))
        {
            ret_val = ICF_FAILURE;
        }
        /*
         * If DBM has returned a valid line ID  and status as line free 
         * means an alternate line was found on which the call could be 
         * presented
         * Copy the line ID in call context and send PA incomming call 
         * indication on the new line
         */
        else if(ICF_INVALID_LINE_ID != alternate_line && 
                ICF_LINE_STATUS_BUSY != status)
        {

             /* Detach the call context from line */
            icf_dbm_detach_cc_from_line(p_cc_pdb->p_glb_pdb,
                    p_cc_pdb->p_call_ctx->line_id,
                    p_cc_pdb->p_call_ctx,
                    p_cc_pdb->p_ecode);
            
            /* Set line ID in call context to alternate line returned by DBM */
            p_cc_pdb->p_call_ctx->line_id = alternate_line;

            if(ICF_FAILURE == icf_dbm_attach_cc_to_line(
                p_cc_pdb->p_glb_pdb, p_cc_pdb->p_call_ctx->line_id,
                p_cc_pdb->p_call_ctx,p_cc_pdb->p_ecode))
            {
                ret_val = ICF_FAILURE;
            }                    
            /*
             * Call DBM's function to check service activated on line
             */ 
            else if(ICF_FAILURE == icf_dbm_chk_line_service(
                        p_cc_pdb->p_glb_pdb,
                        trigger_cause,
                        p_cc_pdb->p_call_ctx->line_id,
                        &status,
                        p_cc_pdb->p_ecode))
            {
                ret_val = ICF_FAILURE;
            }
            /* 
             * Check if status returned by DBM function is 
             * ICF_STATUS_CALL_FWD_UNCOND mean call forwarding unconditional
             *  is active on line then return status to CM
             */ 
            else if(ICF_STATUS_CALL_FWD_UNCOND == status)
            {
                p_cc_pdb->p_internal_msg->msg_hdr.status = status;
            }
            /* 
             * Send incomming call indication to PA on alternate line
             */ 
            else
            {
                /*
                 * Send an incoming call indication to PA
                 */
                if (ICF_FAILURE  == 
                        icf_cc_send_pa_incoming_call_ind(p_cc_pdb))
                {
                    ret_val = ICF_FAILURE;
                }
            }/* End Else */
        }/* End else if(ICF_INVALID_LINE_ID != .. */

        /* RESET the release cause sent by PA */
        ICF_CC_RESET_RELEASE_CAUSE(p_cc_pdb) 
            
        /* 
         * If the was a failure in handling or user had requested to present
         * the call on an alternate line and no other line could be send release
         * cause as 
         * ICF_CALL_CLEAR_REASON_USER_BUSY
         */
        if(ICF_FAILURE == ret_val || 
                ICF_INVALID_LINE_ID == alternate_line)
        {
             ICF_CC_SET_RELEASE_CAUSE(p_cc_pdb,
                     ICF_CALL_CLEAR_REASON_USER_BUSY) 
        }/* End if(ICF_FAILURE == r... */

        if(ICF_INVALID_LINE_ID != alternate_line &&
                ICF_LINE_STATUS_BUSY != status)
        {    
            ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */
            return ret_val;
        }/* End if(ICF_INVALID_LINE_ID != alt... */

    }/* End  if(ICF_CALL_CLEAR_REASON_ATTEMPT_ON_ALTERNATE_LINE == .. */
    else 
    {
        ICF_CC_SET_RELEASE_TYPE(p_cc_pdb,ICF_REL_TYPE_PA_INITIATED)
        ret_val = icf_cc_chk_user_busy_n_srvc(p_cc_pdb);
    }
    
    /*If status was not returned as ICF_STATUS_CALL_FWD_BUSY_LINE continue 
     *with normal call clearing*/
    if(ICF_SUCCESS == ret_val && 
                ICF_STATUS_CALL_FWD_BUSY_LINE != 
                p_cc_pdb->p_internal_msg->msg_hdr.status)
    {
        /*
         * Set release type in call context as PA_INITIAITED
         */
        ICF_CC_TRACE(p_cc_pdb, ICF_TRACE_REL_TYPE_PA_INIT)
        ICF_CC_SET_RELEASE_TYPE(p_cc_pdb,ICF_REL_TYPE_PA_INITIATED)

        if (ICF_NULL != ICF_CC_CHECK_COMMON_BITMASK(p_cc_pdb,
                                                   ICF_CALL_DIR))
          {
            /* validate the response code(if present) */
             icf_cc_validate_response_code_in_term_req(p_cc_pdb);
          }
        
        /*
         * Stop the call setup timer and start the release timer
         */
        if (ICF_FAILURE == icf_cc_start_release_timer(p_cc_pdb))
        {
            ret_val = ICF_FAILURE;
        } /* if (ICF_FAILURE == icf_s... */
        else
        {
      
            /*
             * Set release cause in call context in the case of 
             * incoming call
             */
             ICF_CC_TRACE(p_cc_pdb, ICF_TRACE_CALL_DIR)
            /*
             * Send a DMS request to MMI
             */
            if (ICF_FAILURE == icf_cc_send_dms_req(p_cc_pdb))
            {
                ret_val = ICF_FAILURE;
            } /* if (ICF_FAILURE == icf_c... */
            /*
             * Send call clear request to the SSA
             */
            else if (ICF_FAILURE == 
                    icf_cc_send_clear_req_to_ssa(p_cc_pdb))
            {
                ret_val = ICF_FAILURE;
            } /* else if (ICF_FAILURE == ipp... */
            else
            {
                /*
                 * Update the states in the call context
                 */
                ICF_CC_SET_CALL_STATE(p_cc_pdb,ICF_CC_07_AWT_DMS_RESP)
                /*
                 * On getting the DMS response,CC should be checking the NW 
                 * clear bit and if not set should remain in the same state
                 */
            }
        } /* else */
    }/*End if(ICF_SUCCESS == ret_val ... */
    else if(ICF_SUCCESS == ret_val)
    {
        /* 
         * This code leg would be hit when either call forwarding needs to be 
         * executed of incomming call indication is sent on an alternate line
         * Reset the release cause for this case
         */
        ICF_CC_RESET_RELEASE_CAUSE(p_cc_pdb) 
    }

    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */
    return ret_val;
}


/*******************************************************************************
 *
 * FUNCTION:        icf_cc_03_ssa_offer_change
 *
 * DESCRIPTION:     This function is invoked when the CC is awaiting 
 *                  ALERT and it receives a session param change from SSA
 *                  (SIP UPDATE/RE-INVITE received from peer).
 *                  Current State: ICF_CC_03_AWT_ALERT_RESP
 *                  Current Event: ICF_CC_SSA_OFFER_CHANGE
 *
 ******************************************************************************/
icf_return_t icf_cc_03_ssa_offer_change(
        INOUT icf_cc_pdb_st *p_cc_pdb)
{
    icf_return_t     ret_val = ICF_SUCCESS;

	icf_nw_inc_media_change_req_st	*p_nw_inc_media_change_req = ICF_NULL;

    icf_stream_sdp_info_st      *p_stream_sdp_info = ICF_NULL;
    icf_config_data_st          *p_config_data = ICF_NULL;
	
    p_nw_inc_media_change_req  = (icf_nw_inc_media_change_req_st *)
								p_cc_pdb->p_internal_msg->p_msg_data;
    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /* function entry trace */

    /*
     * Check if media has been committed.
     * If media has not been committed,return FAILURE to remote
     */
    ICF_CC_TRACE(p_cc_pdb, ICF_TRACE_MEDIA_COMMITTED)
ICF_CHECK_TUNNEL_MODE_START(p_cc_pdb->p_glb_pdb, ICF_FALSE)
    if(ICF_FALSE == p_cc_pdb->p_call_ctx->media_committed)
    {
       ret_val =
            icf_cc_delete_inc_media_sdp_info_list(p_cc_pdb,&p_nw_inc_media_change_req->p_sdp_list);
       /*Would direct SSA to send 500 on NW*/
        p_cc_pdb->p_call_ctx->offer_failure_reason = ICF_SA_SERVER_INTERNAL_ERROR;

       if (ICF_FAILURE == icf_cc_send_ssa_offer_chg_err(p_cc_pdb))
       {
           ret_val = ICF_FAILURE;
       } /* if (ICF_FAILURE == icf_c... */
    }
    else
    {
        /* This p_list_1 is being set because this should be initialised 
		 * with icf_inc_media_sdp_info_st before calling this function.
		 */
		p_cc_pdb->p_list_1 = p_nw_inc_media_change_req->p_sdp_list;
        	
		if (
				(ICF_NW_INC_MEDIA_CHG_REJECTED_STREAM == 
					(p_nw_inc_media_change_req->bitmask & 
					ICF_NW_INC_MEDIA_CHG_REJECTED_STREAM))
				||
				(ICF_NW_INC_MEDIA_ADD_STREAM == 
					(p_nw_inc_media_change_req->bitmask & 
					ICF_NW_INC_MEDIA_ADD_STREAM))
			   )
		{
            ret_val = 
            icf_cc_delete_inc_media_sdp_info_list(p_cc_pdb,&p_cc_pdb->p_list_1);

            /* Either media stream is added or deleted.*/
            icf_cc_send_ssa_offer_chg_err(p_cc_pdb);
		}
		else
		{		
               
            ICF_CC_RESET_MODIFY_MEDIA_TYPE(p_cc_pdb)

			/* This function is checking if there is any mismatch in the 
			 * oreder of streams.
			 */
			ret_val = icf_cc_validate_ith_level_m_line_check(p_cc_pdb);
			
			if (ICF_FAILURE == ret_val)
			{
				if (ICF_CC_STREAM_POS_VALIDATION_FAIL == 
					(p_cc_pdb->common_bitmask & ICF_CC_STREAM_POS_VALIDATION_FAIL))
				{
                    p_cc_pdb->p_call_ctx->offer_failure_reason = 
                        ICF_CALL_MODIFY_REASON_REJECT_MEDIA_CHANGE;

                    if (ICF_FAILURE == icf_cc_send_ssa_offer_chg_err(p_cc_pdb))
                    {
                        ret_val = ICF_FAILURE;
                    }
                    else
                    { 
					    ret_val = icf_cc_03_generic_err(p_cc_pdb);
                    }
				}
			}
			else
			{
				if (ICF_FAILURE == 
					icf_cc_make_mms_type_for_remote_offer_change(p_cc_pdb))
				{
					ret_val = ICF_FAILURE;
				}                
                else if(ICF_INT_MODULE_INVALID == ICF_CC_GET_MEDIA_ONGOING(p_cc_pdb))
                {	
			        /*
				    * Send Modify Media Session to the MMI
					*/
					if (ICF_FAILURE == icf_cc_send_mms_req(p_cc_pdb))
					{
						ret_val = ICF_FAILURE;
					}
					else
					{
						/* if mms was not sent on network then
						* remain in this state and send ack to remote party */
						if(ICF_FALSE == p_cc_pdb->mms_sent)
						{
							if (ICF_CC_ALL_STREAMS_DELETED == 
								(p_cc_pdb->common_bitmask & 
										ICF_CC_ALL_STREAMS_DELETED))
							{
								/*Would direct SSA to send 500 on NW*/
						        p_cc_pdb->p_call_ctx->offer_failure_reason = 
									ICF_SA_SERVER_INTERNAL_ERROR;

							    ret_val = icf_cc_send_ssa_offer_chg_err(p_cc_pdb);
								ret_val = icf_cc_03_generic_err(p_cc_pdb);
								return ret_val;
							}

							/* Offer change pdu was saved in call ctx
							* MMS for same will be send later
							* Send ACK for offer change to PA */
							ret_val = icf_cc_send_ssa_offer_chg_rsp(p_cc_pdb);
						}
						else
						{    
          
							/*
							 * MMS was send to remote party.
							 * Update the states in the call context
							 * We are updating the next state as the current state
							 * so that we can call the same function for
							 * handling this event in other states too
							 */
							ICF_CC_SET_NEXT_STATE(p_cc_pdb,
									ICF_CC_GET_CALL_STATE(p_cc_pdb))
                    
							ICF_CC_SET_CALL_STATE(p_cc_pdb,
									ICF_CC_04_AWT_MMS_RESP)
							/*
							 * Update ongoing event
							 */
							 ICF_CC_SET_ONGOING_EVENT(p_cc_pdb,
									ICF_CC_SSA_OFFER_CHANGE)
						}/* MMS was sent to MMI */
					}/* MMS was processed succesfully */
				} /*End of if (ICF_FAILURE == ret_val*/
                else
                {
                    ICF_CC_TRACE(p_cc_pdb, ICF_TRACE_MEDIA_ONGOING)
                    /*
                     * Mark ICF_CC_SSA_OFFER_CHANGE as pending event
                     */
                    ICF_CC_TRACE(p_cc_pdb, ICF_TRACE_EVENT_OFFER_CHANGE)
                    ICF_CC_SET_PENDING_EVENT(p_cc_pdb,ICF_PENDING_EVENT_OFFER_CHANGE_REQ)
                    /*
                     * Update the states in the call context
                     * We are updating the next state as the current state
                     * so that we can call the same function for
                     * handling this event in other states too
                     */
                    ICF_CC_SET_NEXT_STATE(p_cc_pdb,ICF_CC_GET_CALL_STATE(p_cc_pdb))
                    ICF_CC_SET_CALL_STATE(p_cc_pdb,ICF_CC_13_WAIT_START_PROC_MEDIA)
                } /* else */
    		}/* if ((ICF_NW_INC_MEDIA_CHG_REJECTED_STREAM */
		}/* End of if(ICF_INT_MODULE_INVALID == ICF_CC_GET_MEDIA_ONGOI..*/        
    } /* else */
ICF_CHECK_TUNNEL_MODE_END
ICF_CHECK_TUNNEL_MODE_START(p_cc_pdb->p_glb_pdb, ICF_TRUE)
    ret_val = icf_dbm_get_module_glb_data(
                p_cc_pdb->p_glb_pdb,
               (icf_int_module_id_t)ICF_INT_MODULE_CFG,
               (icf_void_t *)&p_config_data, p_cc_pdb->p_ecode);

    if(ICF_NULL == ICF_CC_CHECK_COMMON_BITMASK(p_cc_pdb,
                              ICF_CALL_MOD_REQ_WO_SDP))

    {
        /*  If its incoming call and either media committed is false
         *  or prack is not supported then send 500
         */
        if(ICF_NULL != ICF_CC_CHECK_COMMON_BITMASK(p_cc_pdb,ICF_CALL_DIR) &&
          ((ICF_FALSE == p_cc_pdb->p_call_ctx->media_committed) ||
           (ICF_CALL_CONFIG_PRACK_REQD !=
                   (p_config_data->default_settings & ICF_CALL_CONFIG_PRACK_REQD))))
        {
            ret_val =
                   icf_cc_delete_inc_media_sdp_info_list(p_cc_pdb,
                                    &p_nw_inc_media_change_req->p_sdp_list);
            /*Would direct SSA to send 500 on NW*/
            p_cc_pdb->p_call_ctx->offer_failure_reason = ICF_SA_SERVER_INTERNAL_ERROR;

            if (ICF_FAILURE == icf_cc_send_ssa_offer_chg_err(p_cc_pdb))
            {
                ret_val = ICF_FAILURE;
            } /* if (ICF_FAILURE == icf_c... */
        }
         /*  If its an outgoing call and either media committed is false
          *  or prack is not asked by the peer then send 500
          */

        else if((ICF_NULL == ICF_CC_CHECK_COMMON_BITMASK(p_cc_pdb,ICF_CALL_DIR)) &&
                ((ICF_FALSE == p_cc_pdb->p_call_ctx->media_committed) ||
                 (ICF_REL_PROV_RESP_SUPP_BY_PEER  !=
                 (ICF_REL_PROV_RESP_SUPP_BY_PEER & p_cc_pdb->p_call_ctx->common_bitmask))))
        {
            ret_val =
                  icf_cc_delete_inc_media_sdp_info_list(p_cc_pdb,
                          &p_nw_inc_media_change_req->p_sdp_list);
            /*Would direct SSA to send 500 on NW*/
            p_cc_pdb->p_call_ctx->offer_failure_reason = ICF_SA_SERVER_INTERNAL_ERROR;

            if (ICF_FAILURE == icf_cc_send_ssa_offer_chg_err(p_cc_pdb))
            {
                ret_val = ICF_FAILURE;
            } /* if (ICF_FAILURE == icf_c... */
        }

        /*  if offer in update is allowed */
        else
        {
            if(p_nw_inc_media_change_req->p_sdp_list != ICF_NULL)
            {
                /* take the backup of the local sdp
                 * copy local sdp to last_committed_sdp;           
                 * copy sdp list received in internal msg to remote_recv_sdp.
                 * It would be done by  
                 * icf_cc_validate_ith_level_m_line_check 
                 * It means CC is indicating to mmi that in 
                 * ICF_TUNNELLED_MMS_REQ send the remote recv sdp and
                 * Local sdp is expected from application in 
                 * ICF_TUNNELLED_MMS_RESP. 
                 * set the MMS type to ICF_TUNNELLED_REMOTE_SDP_PRESENT and 
                 * ICF_LOCAL_SDP_REQUIRED*/
                p_cc_pdb->p_list_1 = p_nw_inc_media_change_req->p_sdp_list;
                ICF_CC_RESET_MODIFY_MEDIA_TYPE(p_cc_pdb)
                if(ICF_FAILURE == icf_cc_copy_committed_sdp(p_cc_pdb))
                {
                    ret_val = 
                    icf_cc_delete_inc_media_sdp_info_list(p_cc_pdb,&p_cc_pdb->p_list_1);
                    ret_val = ICF_FAILURE;
                }
                else
                {
                    /* copy sdp list received in internal msg to remote_recv_sdp.
                     */
                    ret_val = icf_cc_validate_ith_level_m_line_check(p_cc_pdb);
                    if (ICF_FAILURE == ret_val)
                    {
                        if (ICF_CC_STREAM_POS_VALIDATION_FAIL == 
                            (p_cc_pdb->common_bitmask & ICF_CC_STREAM_POS_VALIDATION_FAIL))
                        {
                            p_cc_pdb->p_call_ctx->offer_failure_reason = 
                                ICF_CALL_MODIFY_REASON_REJECT_MEDIA_CHANGE;
    
                            if (ICF_FAILURE == icf_cc_send_ssa_offer_chg_err(p_cc_pdb))
                            {
                                ret_val = ICF_FAILURE;
                            }
                        }
                        ret_val =
                           icf_cc_delete_inc_media_sdp_info_list(p_cc_pdb,&p_cc_pdb->p_list_1);

                    }
                    else
                    {
                        /* It means CC is indicating to mmi that in 
                         * ICF_TUNNELLED_MMS_REQ send the remote recv sdp and
                         * Local sdp is expected from application in 
                         * ICF_TUNNELLED_MMS_RESP. 
                         */
                        p_cc_pdb->p_call_ctx->common_bitmask |= ICF_CALL_MODIFY_DIR;
                        p_stream_sdp_info =
                                (icf_stream_sdp_info_st *)
                                (p_cc_pdb->p_call_ctx->p_stream_sdp_info->p_payload);
                        p_stream_sdp_info->media_modify_type = ICF_NULL;
                        p_stream_sdp_info->media_modify_type |= 
                                  ICF_LOCAL_SDP_REQD;
                        p_stream_sdp_info->media_modify_type |= 
                                  ICF_TUNNELLED_REMOTE_SDP_PRESENT;
                        /*  Send incoming call modyfy ind to
                         *  application follwed by MMS 
                         */
                        if(ICF_FAILURE == icf_cc_send_pa_call_modify_ind(p_cc_pdb))
                        {
                            ret_val = ICF_FAILURE;
                        }
                        else
                        {
                            if(ICF_FAILURE == icf_cc_send_mms_req(p_cc_pdb))
                            {
                                ret_val = ICF_FAILURE;
                            }
                            else
                            {
                                /* Fix for SPR: 20101*/
                                /* Set the value of is_call_modify_ongoing and 
                                 * call_modify_req_ongoing as true indicating
                                 * that we are handling the incoimg UPDATE
                                 * with SDP for this call.*/
                                p_cc_pdb->p_call_ctx->is_call_modify_ongoing =
                                    ICF_TRUE;
                                p_cc_pdb->p_call_ctx->call_modify_req_ongoing =
                                    ICF_TRUE;
                                ICF_CC_SET_ONGOING_EVENT(p_cc_pdb,
                                         p_cc_pdb->current_event)
                                /* make call state to next state 
                                 * and move to next state so that when
                                 * call_mod_cfm received then again 
                                 * reach to the same state back
                                 */
                                ICF_CC_SET_CALL_STATE(p_cc_pdb,
                                         ICF_CC_04_AWT_MMS_RESP)
                                ICF_CC_SET_NEXT_STATE(p_cc_pdb, 
                                         ICF_CC_03_AWT_ALERT_RESP)
                            }
                        }
                    }
                }
            }
        }
    }
    /*  If UPDATE receive without SDP */
    else
    {
        if(ICF_FAILURE == icf_cc_send_pa_call_modify_ind(p_cc_pdb))
        {
            ret_val = ICF_FAILURE;
        }
        else
        {
            /* Fix for SPR: 20101*/
            /* Set the value of is_call_modify_ongoing and 
             * call_modify_req_ongoing as true indicating that we are handling
             * the incoimg UPDATE without SDP for this call.*/            
            p_cc_pdb->p_call_ctx->is_call_modify_ongoing = ICF_TRUE;
            p_cc_pdb->p_call_ctx->call_modify_req_ongoing = ICF_TRUE;
            /* make call state to next state 
             * and move to next state so that when
             * call_mod_cfm received then again 
             * reach to the same state back
             */
            p_cc_pdb->p_call_ctx->common_bitmask |= ICF_CALL_MODIFY_DIR;
            ICF_CC_SET_NEXT_STATE(p_cc_pdb, ICF_CC_GET_CALL_STATE(p_cc_pdb))
            ICF_CC_SET_CALL_STATE(p_cc_pdb, ICF_CC_14_AWT_CALL_MODIFY_RESP)
            ICF_CC_SET_ONGOING_EVENT(p_cc_pdb,
                          ICF_CC_SSA_OFFER_CHANGE)
             /*SPR 19395 the below change  is done to resolve the
              problem whenever update is received without SDP 
              then IPTK is sending 200 OK with SDP to peer 
              This was happening because the following boolean 
              governs the decision to send 200 OK with/without SDP
              Thus if UPDATE received without SDP then the boolean 
              update_recv must be set and once 200 OK is  
              sent then it must get reset */
            p_cc_pdb->p_call_ctx->update_recv = ICF_TRUE;
        }
    }
ICF_CHECK_TUNNEL_MODE_END

    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */
    return ret_val;
}



/******************************************************************************
 *
 * FUNCTION:        icf_cc_03_mm_error_ind
 *
 * DESCRIPTION:     This function is invoked when the CC is awaiting 
 *                  ALERT(SIP 180) from the SSA or ALERT RESP from
 *                  the PA(incoming call)and MM error indication
 *                  is received from MMI
 *                  Current State: ICF_CC_03_AWT_ALERT_RESP
 *                  Current Event: ICF_CC_MM_ERR_IND
 *
 *****************************************************************************/
icf_return_t icf_cc_03_mm_error_ind(
        INOUT icf_cc_pdb_st *p_cc_pdb)
{
    icf_return_t     ret_val = ICF_SUCCESS;
	icf_mm_err_ind_st *p_mm_err_ind;
    icf_mmi_reason_t  media_error;

    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /* function entry trace */
	
	p_mm_err_ind = (icf_mm_err_ind_st*)
        (p_cc_pdb->p_internal_msg->p_msg_data);


    media_error = p_mm_err_ind->reason;

    /*
     * Stop the call setup timer and start the release timer
     */
    if (ICF_FAILURE == icf_cc_start_release_timer(p_cc_pdb))
    {
        ret_val = ICF_FAILURE;
    } /* if (ICF_FAILURE == icf_s... */
    else
    {
        /*
         * Set release type in call context as MM_INITIAITED
         */
        ICF_CC_TRACE(p_cc_pdb, ICF_TRACE_REL_TYPE_MM_INIT)
        ICF_CC_SET_RELEASE_TYPE(p_cc_pdb,ICF_REL_TYPE_MM_INITIATED) 
        /*
         * Set release cause in call context as ICF_REL_CAUSE_SERV_INT_ERR
         * so that SIP 500 is sent to peer in the case of incoming call
         */
        ICF_CC_TRACE(p_cc_pdb, ICF_TRACE_CALL_DIR)
         if (ICF_NULL != ICF_CC_CHECK_COMMON_BITMASK(p_cc_pdb,
                     ICF_CALL_DIR))
         {
             ICF_CC_SET_RELEASE_CAUSE(p_cc_pdb,ICF_CALL_CLEAR_MEDIA_MISMATCH)    
         }
        /*
         * Send call clear request to the SSA
         */
        if (ICF_FAILURE == icf_cc_send_clear_req_to_ssa(p_cc_pdb))
        {
            ret_val = ICF_FAILURE;
        } /* if (ICF_FAILURE == icf_c... */
         /*
         * If mmi has send mm_err_ind with reason as ICF_ECODE_DMS_IND_RECVD
         */
        if(ICF_ECODE_DMS_IND_RECVD != media_error)
        {
            /*
             * Send a DMS request to MMI
             */
            if (ICF_FAILURE == icf_cc_send_dms_req(p_cc_pdb))
            {
                ret_val = ICF_FAILURE;
            } /* else if (ICF_FAILURE == ipp... */
            else
            {
                /*
                 * Update the states in the call context
                 */
                ICF_CC_SET_CALL_STATE(p_cc_pdb,ICF_CC_07_AWT_DMS_RESP)
            } /* else */
        }
        else
        {
            /* Send Call Terminate Request to PA */
            ret_val = icf_cc_send_pa_call_term_ind(p_cc_pdb);

            ICF_CC_SET_CALL_CLR_STATUS(p_cc_pdb,
                ICF_CALL_CLEAR_STATUS_PA_CLEARED)

            /* Set MM cleared bit in call_clear_status */
            ICF_CC_SET_CALL_CLR_STATUS(p_cc_pdb,
                ICF_CALL_CLEAR_STATUS_MM_CLEARED)

            ICF_CC_SET_CALL_STATE(p_cc_pdb,
                    ICF_CC_08_AWT_CALL_TERMINATE_RESP)

        } /* else */
    } /* else */

    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */
    return ret_val;
}


/******************************************************************************
 *
 * FUNCTION:        icf_cc_03_timer_expiry
 *
 * DESCRIPTION:     This function is invoked when the CC is awaiting 
 *                  ALERT(SIP 180) from the SSA or ALERT RESP from
 *                  the PA(incoming call)aand Setup Timer expires.
 *                  Current State: ICF_CC_03_AWT_ALERT_RESP
 *                  Current Event: ICF_CC_TIMER_EXPIRY
 *
 *****************************************************************************/
icf_return_t icf_cc_03_timer_expiry(
        INOUT icf_cc_pdb_st *p_cc_pdb)
{
    icf_return_t     ret_val = ICF_SUCCESS;

    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /* function entry trace */

    /* Fix For SPR:20101*/    
    /* While handling the timer expiry in ICF_CC_03_AWT_ALERT_RESP if the
     * value of is_call_modify_ongoing is set to TRUE it indicates that we
     * have received incoming UPDATE with/without sdp and this UPDATE need to
     * be responded with 504 response.We are handling this scenario in
     * icf_cc_14_timer_expiry where UPDATE is responded with failure response
     * and call will be cleared.*/    
    if (ICF_TRUE == p_cc_pdb->p_call_ctx->is_call_modify_ongoing)
    {
        ret_val = icf_cc_14_timer_expiry(p_cc_pdb);
        p_cc_pdb->p_call_ctx->is_call_modify_ongoing = ICF_FALSE;
    }
    else
    {
        /*
         * Stop the call setup timer and start the release timer
         */
        if (ICF_FAILURE == icf_cc_start_release_timer(p_cc_pdb))
        {
            ret_val = ICF_FAILURE;
        } /* if (ICF_FAILURE == icf_s... */
        else
        {
            /*
             * Set release type in call context as TIMER_EXPIRY
             */
            ICF_CC_TRACE(p_cc_pdb, ICF_TRACE_REL_TYPE_TIMER_INIT)
                ICF_CC_SET_RELEASE_TYPE(p_cc_pdb,ICF_REL_TYPE_TIMER_EXPIRY) 

                ICF_CC_TRACE(p_cc_pdb, ICF_TRACE_CALL_DIR)
                if (ICF_NULL != ICF_CC_CHECK_COMMON_BITMASK(p_cc_pdb,
                            ICF_CALL_DIR))
                {
                    ICF_CC_SET_RELEASE_CAUSE(p_cc_pdb,
                            ICF_CALL_CLEAR_REASON_NOANSWER)    
                } /* ICF_CALL_DIR)) */
            /*
             * Send call clear request to the SSA
             */
            if (ICF_FAILURE == icf_cc_send_clear_req_to_ssa(p_cc_pdb))
            {
                ret_val = ICF_FAILURE;
            } /* if (ICF_FAILURE == icf_c... */
            /*
             * Send a DMS request to MMI
             */
            else if (ICF_FAILURE == icf_cc_send_dms_req(p_cc_pdb))
            {
                ret_val = ICF_FAILURE;
            } /* else if (ICF_FAILURE == ipp... */
            else
            {
                /*
                 * Update the states in the call context
                 */
                ICF_CC_SET_CALL_STATE(p_cc_pdb,ICF_CC_07_AWT_DMS_RESP)
            }
        } /* else */
    }

    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */
    return ret_val;
}


/******************************************************************************
 *
 * FUNCTION:        icf_cc_03_pa_alert
 *
 * DESCRIPTION:     This function is invoked when the CC is awaiting 
 *                  ALERT RESP from the PA(incoming call) and
 *                  ALERT RESP from the PA is received.
 *                  Current State: ICF_CC_03_AWT_ALERT_RESP
 *                  Current Event: ICF_CC_PA_ALERT
 *
 *****************************************************************************/
icf_return_t icf_cc_03_pa_alert(
        INOUT icf_cc_pdb_st *p_cc_pdb)
{
    icf_return_t     ret_val = ICF_SUCCESS;
    icf_pa_cc_user_alerted_req_st	*p_user_alerted = ICF_NULL;
    icf_boolean_t		is_early_media_required = ICF_TRUE;
    icf_config_data_st                *p_config_data = ICF_NULL;
    icf_internal_list_st	*p_stream_sdp_info_node=p_cc_pdb->
									p_call_ctx->p_stream_sdp_info;

	icf_stream_sdp_info_st	*p_stream_sdp_info_data = ICF_NULL;

    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /* function entry trace */
    
    ret_val = icf_dbm_get_module_glb_data(
                p_cc_pdb->p_glb_pdb,
               (icf_int_module_id_t)ICF_INT_MODULE_CFG,
               (icf_void_t *)&p_config_data, p_cc_pdb->p_ecode);
    if(ICF_FAILURE == ret_val)
    {
        ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb)
        return ret_val;
    }
ICF_CHECK_TUNNEL_MODE_START(p_cc_pdb->p_glb_pdb,ICF_TRUE)
    if(ICF_TRUE == p_cc_pdb->p_call_ctx->call_modify_req_ongoing)
    {   
         /*SPR 19491 This is to avoid sending the 180 when UPDATE is recv
           fromm peer and MMS is done for UPDATE and instead of call modify
           cfm PA sends alert_req , we will reject this, it is expected that
           PA sends first call_modify_cfm and then alert_req*/
           ret_val = icf_cc_send_pa_unexpected_msg_ind(p_cc_pdb);
           ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */
           return ret_val;
     }
ICF_CHECK_TUNNEL_MODE_END
  
    is_early_media_required = ((icf_pa_cc_user_alerted_req_st *)
                p_cc_pdb->p_glb_pdb->p_recv_msg->payload)->early_media_reqd;

    p_user_alerted = ((icf_pa_cc_user_alerted_req_st *)
                p_cc_pdb->p_glb_pdb->p_recv_msg->payload);
    /* Rel 8.2 changes Start */
    if(ICF_LOCAL_USER_ALERTED_PRACK_VALUE_PRESENT & 
         p_user_alerted->bit_mask)
    {
        /*  If INVITE received with 100rel in Require header
         *  and in first 18x API PRACK is OFF then clear the
         */
        if(ICF_PER_CALL_PRACK_FEATURE_ENABLED  != 
           (ICF_CC_CHECK_COMMON_BITMASK_2(p_cc_pdb,
            ICF_PER_CALL_PRACK_FEATURE_ENABLED)))
        {
            /* It is the case when ICF_NO_PRACK is sent by application
             * and Peer send 100rel in require header of INVITE
             * then clear the call.
             */
            if((ICF_NO_PRACK == p_user_alerted ->prack_value) &&
               (ICF_REL_PROV_RESP_REQUIRED_BY_PEER == 
               (ICF_CC_CHECK_COMMON_BITMASK_2(p_cc_pdb,
                ICF_REL_PROV_RESP_REQUIRED_BY_PEER))))
            {
                p_cc_pdb->p_call_ctx->release_cause =
                       ICF_CALL_CLEAR_REASON_PRACK_CALL_NOT_ALLOWED;
                ret_val = icf_cc_03_generic_err(p_cc_pdb);
                p_cc_pdb->p_call_ctx->release_cause = 
                       ICF_CALL_CLEAR_REASON_NON_PRACK_CALL_NOT_ALLOWED;
                ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb)
                return ret_val;
            }
            /* It is the case when ICF_PRACK_IN_REQUIRE is sent by application
             * and peer neither send 100rel in require nor in supported 
             * header of INVITE
             * then clear the call.
             */
            else if((ICF_REL_PROV_RESP_SUPP_BY_PEER != 
               (ICF_CC_CHECK_COMMON_BITMASK(p_cc_pdb,
                ICF_REL_PROV_RESP_SUPP_BY_PEER))) &&
               ICF_PRACK_IN_REQUIRE  == p_user_alerted ->prack_value)
            {
                p_cc_pdb->p_call_ctx->release_cause = 
                       ICF_CALL_CLEAR_REASON_NON_PRACK_CALL_NOT_ALLOWED;
                ret_val = icf_cc_03_generic_err(p_cc_pdb);
                ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb)
                return ret_val;
            }

    /*  If PRACK Field came in the API then check in call ctx that is is
     *  already populated or not if already populated then ignore
     *  it because then it will not be the first 18x request.
     *  else copy it in call ctx and set the common bitmask and 
     *  update the prack_reqd field in cc pdb, because all the decisions
     *  were taken for the prack on this field of cc pdb
     */
            p_cc_pdb->p_call_ctx-> prack_value =  
                                  p_user_alerted ->prack_value;
            ICF_CC_SET_COMMON_BITMASK_2(p_cc_pdb, 
                ICF_PER_CALL_PRACK_FEATURE_ENABLED)
            if(ICF_PRACK_IN_REQUIRE == p_user_alerted ->prack_value)
            {
                p_cc_pdb->prack_reqd = ICF_TRUE;
            }
            else
            {
                p_cc_pdb->prack_reqd = ICF_FALSE;
            }
        }
    }
    /* If INVITE received with 100rel in Require header
     * and in first 18x API PRACK field is not received 
     * and the system level flag for PRACK is also OFF 
     * then clear the
     */

    else if(ICF_PER_CALL_PRACK_FEATURE_ENABLED != 
           (ICF_CC_CHECK_COMMON_BITMASK_2(p_cc_pdb,
            ICF_PER_CALL_PRACK_FEATURE_ENABLED)))
    {
        /* SPR 19754 : In case system level flag is OFF and 
         * invite is received with 100rel in require then
         * establish a PRACK call irrespective of user
         * configuration
         */ 
        p_cc_pdb->p_call_ctx->system_level_configuration_for_prack = ICF_TRUE;
    }

    /*  Rel 8.2 changes End */
    if(ICF_USER_ALERTED_ALERT_TIMER_PRESENT & 
        p_user_alerted->bit_mask)
    {
        /*Stop the call setup timer and start the alert timer*/
        if (ICF_FAILURE == icf_cc_start_alert_timer(p_cc_pdb, ICF_TRUE, 
            p_user_alerted->alert_timer_duration))
        {
            return ICF_FAILURE;
        } /* if (ICF_FAILURE == icf_c... */
    }
    else
    {
        /*Stop the call setup timer and start the alert timer*/
        if (ICF_FAILURE == icf_cc_start_alert_timer(p_cc_pdb, ICF_FALSE, ICF_NULL))
        {
            return ICF_FAILURE;
        } /* if (ICF_FAILURE == icf_c... */
    }

    /* In tunnel mode, request with SDP will be identified on the basis of
       the field early_media_reqd.
    */   
ICF_CHECK_TUNNEL_MODE_START(p_cc_pdb->p_glb_pdb,ICF_FALSE)
    
    /*
     * Check whether inband info status flag is set in the alert 
     * indication from the PA
     */
    if (ICF_TRUE == p_user_alerted->inband_info_status)
    {
        p_cc_pdb->p_call_ctx->inband_info_present = ICF_TRUE;
    } /* p_cc_pdb->p_glb_pdb->p_recv_ms... */
    else
    {
        p_cc_pdb->p_call_ctx->inband_info_present = ICF_FALSE;
    } /* else */
ICF_CHECK_TUNNEL_MODE_END

    /* Set inband_info_status on the basis of early_media_reqd */
ICF_CHECK_TUNNEL_MODE_START(p_cc_pdb->p_glb_pdb,ICF_TRUE)
    if (ICF_TRUE == is_early_media_required)
    {
        p_cc_pdb->p_call_ctx->inband_info_present = ICF_TRUE;
    }
    else
    {
       p_cc_pdb->p_call_ctx->inband_info_present = ICF_FALSE;
    } 
ICF_CHECK_TUNNEL_MODE_END

    ICF_CC_TRACE(p_cc_pdb, ICF_TRACE_INBAND_INFO)

    if (ICF_FAILURE == ret_val)
    {
        
    }
	else  
	{
	    /* 
         * If it is a NFS call or a FS call with inband info 
         * present,send MEDIA_PARAM to peer.
         * Following this, if PRACK is not required
         * then send ALERT without media to peer.
         */
        ICF_CC_TRACE(p_cc_pdb, ICF_TRACE_MEDIA_SETUP_TYPE)
        ICF_CC_TRACE(p_cc_pdb, ICF_TRACE_INBAND_INFO)



	    /* check if privacy header needs to be sent */
	    if (ICF_PRIVACY_IND_PRESENT & p_user_alerted->bit_mask)
	    {
		    p_cc_pdb->p_call_ctx->send_privacy_hdr = p_user_alerted->privacy_ind;
	    }

        /* SPR 19590 : code removed for cloning header/tag/body list from 
         * here as this shall be taken care in icf_cc_convert_api_to_event
         * function.
         */

        /* In tunnel mode, we do not validate or process streams, media modes etc.
           Thus the following code snippet has to be bypassed.
        */
ICF_CHECK_TUNNEL_MODE_START(p_cc_pdb->p_glb_pdb,ICF_FALSE)        
		if (ICF_STREAM_LIST_PRESENT == (p_user_alerted->bit_mask & ICF_STREAM_LIST_PRESENT))
        {
			if (ICF_NULL != p_user_alerted->p_stream_list)
			{
               p_cc_pdb->p_list_1 = p_user_alerted->p_stream_list; 	
		       if (ICF_FAILURE == icf_cc_process_app_stream_capab_req(p_cc_pdb))
		       {
			     if (ICF_STREAM_ID_VALIDATION_FAIL == 
				 (p_cc_pdb->common_bitmask & ICF_STREAM_ID_VALIDATION_FAIL))
			     {
				    ret_val = icf_cc_03_generic_err(p_cc_pdb);
				    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */
				    return ret_val;
			     }
			     return ret_val;
		      }
           }
       }

		if (ICF_USER_ALERTED_DELETED_STREAM_PRESENT == 
		(p_user_alerted->bit_mask & ICF_USER_ALERTED_DELETED_STREAM_PRESENT))
		{
			p_cc_pdb->count = p_user_alerted->count;
			p_cc_pdb->p_generic_ptr_1 = p_user_alerted->deleted_media_streams;

			/* Following function will check if there is any stream 
		     * deleted by user.
			 */
			ret_val = icf_cc_check_for_streams_deleted_by_user(p_cc_pdb);
	

			/* ith level check was NOT performed inside the above function,
			 * This bit is just being checked if there was any exception occured inside
			 * the above function (Like stream id deleted(if there was any) but it was not
			 * present inside the list of sdps received from SSA.
			 */
			if (ICF_CC_ALL_STREAMS_DELETED == 
				(p_cc_pdb->common_bitmask & ICF_CC_ALL_STREAMS_DELETED))
			{
				ret_val = icf_cc_03_generic_err(p_cc_pdb);
				ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */
				return ret_val;
			}

            /* code for media mode changes starts here */
            /* check if the media mode is present and the value is not
             * send only then do the mms to with the mode given mode 
            * also ICF_MEDIA_MODE_APPLIED_FROM_APP is to check whether the mode
            * already set in Media Connect then don't change the mode here again
            * check for early media is there because we want set the mode as
            * send recv iff SDP is going in the 180/183*/
            if (ICF_USER_ALERTED_MEDIA_MODE_PRESENT == 
                 (p_user_alerted->bit_mask & ICF_USER_ALERTED_MEDIA_MODE_PRESENT) &&
                 (p_user_alerted->media_mode != ICF_MEDIA_MODE_SEND_ONLY ) &&
               (ICF_MEDIA_MODE_APPLIED_FROM_APP != (p_cc_pdb->p_call_ctx->common_bitmask & ICF_MEDIA_MODE_APPLIED_FROM_APP)) &&
               ICF_TRUE == is_early_media_required)
            {
                /* change the mm applied mode here
                 * no Need to check the return value here as 
                 * mms is already use in the existing code*/
               icf_cc_apply_pa_media_mode(
                   p_cc_pdb->p_call_ctx->p_stream_sdp_info,
                   p_user_alerted->media_mode);
            }   
            /* End of media mode change */

			if (icf_cc_send_mms_req(p_cc_pdb) == ICF_FAILURE)
			{
				ret_val=ICF_FAILURE;
			}
			else
			{
				ICF_CC_SET_CALL_STATE(p_cc_pdb, ICF_CC_04_AWT_MMS_RESP)
				ICF_CC_SET_ONGOING_EVENT(p_cc_pdb,ICF_CC_PA_ALERT)
                ICF_CC_SET_NEXT_STATE(p_cc_pdb, ICF_CC_05_AWT_CONNECT)
			}

		}

      /* code for media mode changes starts here */
            /* check if the media mode is present and the value is not
             * send only then do the mms to with the mode given mode 
            * also ICF_MEDIA_MODE_APPLIED_FROM_APP is to check whether the mode
            * already set in Media Connect then don't change the mode here again
            * check for early media is there because we want set the mode as
            * send recv iff SDP is going in the 180/183*/
     else if (ICF_USER_ALERTED_MEDIA_MODE_PRESENT == (p_user_alerted->bit_mask & ICF_USER_ALERTED_MEDIA_MODE_PRESENT) &&
            (p_user_alerted->media_mode != ICF_MEDIA_MODE_SEND_ONLY)&&
            (ICF_MEDIA_MODE_APPLIED_FROM_APP != (p_cc_pdb->p_call_ctx->common_bitmask & ICF_MEDIA_MODE_APPLIED_FROM_APP))&&
            (ICF_TRUE == is_early_media_required))
    {
        /* change the mm applied mode here */
        if(ICF_FAILURE != icf_cc_apply_pa_media_mode(p_cc_pdb->p_call_ctx->p_stream_sdp_info,
                                                        p_user_alerted->media_mode))
        {

            /* p_cc_pdb->p_call_ctx->common_bitmask |= ICF_MEDIA_MODE_APPLIED_FROM_APP ;*/

            if (icf_cc_send_mms_req(p_cc_pdb) == ICF_FAILURE)
            {
                ret_val=ICF_FAILURE;
            }
            else
            {
                ICF_CC_SET_CALL_STATE(p_cc_pdb, ICF_CC_04_AWT_MMS_RESP)
                ICF_CC_SET_ONGOING_EVENT(p_cc_pdb,ICF_CC_PA_ALERT)
                ICF_CC_SET_NEXT_STATE(p_cc_pdb, ICF_CC_05_AWT_CONNECT)
            }
        }
    }


      if((ICF_USER_ALERTED_EARLY_MEDIA_SUPPORTED & p_user_alerted->bit_mask) &&
         (ICF_EARLY_MEDIA_HDR_SUPPORTED & p_cc_pdb->p_call_ctx->common_bitmask_2))          
        {
            while(p_stream_sdp_info_node != NULL)
	        {
              	p_stream_sdp_info_data = (icf_stream_sdp_info_st *)
           			   					(p_stream_sdp_info_node->p_payload);
                if(p_user_alerted->early_mode != p_stream_sdp_info_data->mm_applied_mode)
                {
                  	if (ICF_MEDIA_STREAM_ACTIVE ==
                		(p_stream_sdp_info_data->stream_status &
                                     			ICF_MEDIA_STREAM_ACTIVE))
                     { 
                            p_stream_sdp_info_data->mm_applied_mode = 
                                                  p_user_alerted->early_mode;
                            p_stream_sdp_info_data->media_modify_type |=
					        					ICF_MODE_CHANGE;
                            p_stream_sdp_info_data->p_local_sdp->media_mode = 
                                                  p_user_alerted->early_mode; 
                     }
                 }
                 p_stream_sdp_info_node = (icf_internal_list_st *)
 									(p_stream_sdp_info_node->p_next);

            }
           	if (icf_cc_send_mms_req(p_cc_pdb) == ICF_FAILURE)
			{
				ret_val=ICF_FAILURE;
			}
			else
			{
				ICF_CC_SET_CALL_STATE(p_cc_pdb, ICF_CC_04_AWT_MMS_RESP)
				ICF_CC_SET_ONGOING_EVENT(p_cc_pdb,ICF_CC_PA_ALERT)
                ICF_CC_SET_NEXT_STATE(p_cc_pdb, ICF_CC_05_AWT_CONNECT)
			}
      }
      else
      {
        /*CSR1-5435830 It will check the presence of P-Early-Media  
          header in the outgoing 18x response.
          If it is set it will reset the flag for the 
          outgoing 18x response */  
             if(ICF_EARLY_MEDIA_HDR_SUPPORTED &
                     p_cc_pdb->p_call_ctx->common_bitmask_2)
             {
                  p_cc_pdb->p_call_ctx->common_bitmask_2  &= 
                                         ~(ICF_EARLY_MEDIA_HDR_SUPPORTED);
             }
       }
ICF_CHECK_TUNNEL_MODE_END      

#ifdef ICF_QOS_SUPPORT
ICF_CHECK_QOS_SUPPORT_START(p_cc_pdb->p_glb_pdb,ICF_TRUE)
/*In non tunnel mode we should not check for resource reservation
  completed flag and send 180 directly */
ICF_CHECK_TUNNEL_MODE_START(p_cc_pdb->p_glb_pdb,ICF_FALSE)
 		   if((ICF_FAILURE != ret_val) && (ICF_FALSE == p_cc_pdb->mms_sent))
		   {
       
		     if(ICF_FAILURE == icf_cc_send_ssa_alert(p_cc_pdb))
		     {
			    ret_val = ICF_FAILURE;
		     } /* if (ICF_FAILURE == icf_c... */
		     else if (ICF_TRUE == p_cc_pdb->prack_reqd)
		     {
			    /*
			     * Update the states in the call context
			     */
			    ICF_CC_SET_CALL_STATE(p_cc_pdb,ICF_CC_10_AWT_ALERT_PRACK)
		     } /* if (ICF_TRUE == p_cc_pdb->p... */
		     else
		     {
			    /*
			     * Update the states in the call context
			     */
			    ICF_CC_SET_CALL_STATE(p_cc_pdb,ICF_CC_05_AWT_CONNECT)
		     }
		   }
ICF_CHECK_TUNNEL_MODE_END      
ICF_CHECK_TUNNEL_MODE_START(p_cc_pdb->p_glb_pdb,ICF_TRUE)
        /*If resource reservation is done then after UPDATE-200OK
         we shall be in this state and if the alert request comes now then
         we shall be sending the 180 as done in non-tunnel mode*/
       /*SPR19574 Added the media_connect_sent check for the sceanrio when
         during an incoming QOS call in tunnel mode media_connect_req is
         followed by alert then we should not send the 183 twice*/
        if((ICF_TRUE == p_cc_pdb->p_call_ctx->resource_reservation_completed)
            || (ICF_TRUE == p_cc_pdb->p_call_ctx->media_connect_sent)) 
        {    
 		   if((ICF_FAILURE != ret_val) && (ICF_FALSE == p_cc_pdb->mms_sent))
		   {
       
		     if(ICF_FAILURE == icf_cc_send_ssa_alert(p_cc_pdb))
		     {
			    ret_val = ICF_FAILURE;
		     } /* if (ICF_FAILURE == icf_c... */
		     else if (ICF_TRUE == p_cc_pdb->prack_reqd)
		     {
			    /*
			     * Update the states in the call context
			     */
                ICF_CC_RESET_PENDING_EVENT(p_cc_pdb,
                    ICF_PENDING_EVENT_PA_ALERT_WO_MEDIA_REQ)
			    ICF_CC_SET_CALL_STATE(p_cc_pdb,ICF_CC_10_AWT_ALERT_PRACK)
		     } /* if (ICF_TRUE == p_cc_pdb->p... */
		     else
		     {
			    /*
			     * Update the states in the call context
			     */
                ICF_CC_RESET_PENDING_EVENT(p_cc_pdb,
                    ICF_PENDING_EVENT_PA_ALERT_WO_MEDIA_REQ)
			    ICF_CC_SET_CALL_STATE(p_cc_pdb,ICF_CC_05_AWT_CONNECT)
		     }
		   }
        }
        /*If resource reservation is not yet completed that means UPDATE-200OK
          is not done yet for QOS therefore if alert is received then we shall
          be marking it as pending and after sending 200OK for the UPDATE in
          state 14 we shall process this pending event this was done when
          call modify cfm is recv*/
        else 
        {   
          /* To process 180 wo SDP after sending 200 OK for precondition
             UPDATE */
             ICF_CC_SET_PENDING_EVENT(p_cc_pdb,
                ICF_PENDING_EVENT_PA_ALERT_WO_MEDIA_REQ)
             /*Send the 183 SDP as now it is not triggered in 01 state
              after cms resp, also on recv 180/183/200 from PA we need to send
              183 to NW in case of tunnel QOS call.*/
          	 if ((ICF_FAILURE != ret_val) && (ICF_FALSE ==
                         p_cc_pdb->mms_sent))
		     {
                  ret_val = icf_cc_prep_media_param_for_qos(p_cc_pdb);
             } 
        }
ICF_CHECK_TUNNEL_MODE_END      
		
ICF_CHECK_QOS_SUPPORT_END
ICF_CHECK_QOS_SUPPORT_START(p_cc_pdb->p_glb_pdb,ICF_FALSE)

        /* The following code snippet is not applicable in tunnel mode as in
           tunnel mode, we will never send a 183 on receiving alert request
           from the application.
        */   
ICF_CHECK_TUNNEL_MODE_START(p_cc_pdb->p_glb_pdb,ICF_FALSE)
		if ((ICF_FAILURE != ret_val) && (ICF_FALSE == p_cc_pdb->mms_sent))
		{
            if ((ICF_NULL == ICF_CC_CHECK_COMMON_BITMASK(p_cc_pdb,
                            ICF_CALL_MEDIA_SETUP_TYPE))||
                    ((ICF_NULL != ICF_CC_CHECK_COMMON_BITMASK(p_cc_pdb,
                                   ICF_CALL_MEDIA_SETUP_TYPE)) && 
                     (ICF_FALSE == p_cc_pdb->p_call_ctx->inband_info_present)))
            {
                /*
                 * Send media param to the SSA
                 */
                if (ICF_TRUE == is_early_media_required) 
                {   /*Fix merged for CSR_1-5417099*/
                	/* This is a case of Non Fast Start Call,where we check if
                     * application sends the user alerted api with inband info
                     * as false and early media flag as true.
                     * in such scenario
                     * IPTK will send 183 with sdp followed by 180 without sdp on the 
                     * network.But if inband info and early media are both true
                     * in that case IPTK will send 180 with sdp on the network.*/
                     
                    if(ICF_FALSE == p_cc_pdb->p_call_ctx->inband_info_present)
                    {
                        if(ICF_FAILURE == icf_cc_send_ssa_media_param(p_cc_pdb))
                        {
                     		ret_val = ICF_FAILURE;
                    		/* if (ICF_FAILURE == icf_c... */
                 		}
                 	    else
                 		{
                    	    ICF_CC_TRACE(p_cc_pdb, ICF_TRACE_PRACK_REQD)
           
          		            if (ICF_TRUE == p_cc_pdb->prack_reqd)
                		    {
                        		/*
                         		* Update the states in the call context
                         		*/
                        		ICF_CC_SET_CALL_STATE(p_cc_pdb,ICF_CC_09_AWT_MEDIA_PARAM_PRACK)
                    	    } /* if (ICF_TRUE == p_cc_pdb->p... */
                   	        else
                   	        {
                     			if (ICF_FAILURE == icf_cc_send_ssa_alert(p_cc_pdb))
                        		{
                            		ret_val = ICF_FAILURE;
                        		}	 	/* if (ICF_FAILURE == icf_c... */
                        	}
                 		}
                    }
                    else
                    {
                        if(ICF_FAILURE ==  icf_cc_send_ssa_alert_media(p_cc_pdb))
                        {
                            ret_val = ICF_FAILURE;
                        }
                        else
                        {
                            if (ICF_TRUE == p_cc_pdb->prack_reqd)
                            {
                                /*
                                 * Update the states in the call context
                                 */
                                ICF_CC_SET_CALL_STATE(p_cc_pdb,ICF_CC_10_AWT_ALERT_PRACK)
                             } /* if (ICF_TRUE == p_cc_pdb->p... */
                        }
                    }                     
                 }    
                 else if(ICF_FAILURE == icf_cc_send_ssa_alert(p_cc_pdb))
                 {
                     ret_val = ICF_FAILURE;
                 }
                 else if(ICF_TRUE == p_cc_pdb->prack_reqd)
                  {
                      /*
                       * Update the states in the call context
                       */
                      ICF_CC_SET_CALL_STATE(p_cc_pdb,ICF_CC_10_AWT_ALERT_PRACK)
                   } /* else */
                   else
                   {
                      /*
                       * Update the states in the call context
                       */
                      ICF_CC_SET_CALL_STATE(p_cc_pdb,ICF_CC_05_AWT_CONNECT)
                   }
          }/* (ICF_FALSE == p_cc_pdb->p_c... */

            else if((ICF_NULL != ICF_CC_CHECK_COMMON_BITMASK(p_cc_pdb,
                            ICF_CALL_MEDIA_SETUP_TYPE)) && 
                    (ICF_TRUE == p_cc_pdb->p_call_ctx->inband_info_present))
            {

                ICF_CC_TRACE(p_cc_pdb, ICF_TRACE_MEDIA_SETUP_TYPE)
                ICF_CC_TRACE(p_cc_pdb, ICF_TRACE_INBAND_INFO)
                ICF_CC_TRACE(p_cc_pdb, ICF_TRACE_PRACK_REQD)
    
                if (ICF_TRUE == is_early_media_required)
				{
					ret_val = icf_cc_send_ssa_alert_media(p_cc_pdb);
				}
				else
				{
					ret_val = icf_cc_send_ssa_alert(p_cc_pdb);
				}
				if ( ICF_FAILURE == ret_val)
				{
					/*Nothing to do*/
				}
                else if (ICF_TRUE == p_cc_pdb->prack_reqd)
                {
                    /*
                     * Update the states in the call context
                     */
                    ICF_CC_SET_CALL_STATE(p_cc_pdb,ICF_CC_10_AWT_ALERT_PRACK)
                } /* if (ICF_TRUE == p_cc_pdb->p... */
                else
                {
                    /*
                     * Update the states in the call context
                     */
                    ICF_CC_SET_CALL_STATE(p_cc_pdb,ICF_CC_05_AWT_CONNECT)
                } /* else */
            } /* (ICF_TRUE == p_cc_pdb->p_ca... */
        }/*End of if (ICF_FAILURE != ret_val)*/
ICF_CHECK_TUNNEL_MODE_END
ICF_CHECK_QOS_SUPPORT_END
#else
        /* In tunnel mode, we will not do a tunnelled MMS request on receiving alert 
           request from the application. The field early_media_reqd will determine whether
           the 180 has to be sent with SDP or without SDP. 
           In tunnel mode, if the application has set early_media_reqd as TRUE,
           we will send 180 with SDP, else we will send 180 without SDP.
        */   
ICF_CHECK_TUNNEL_MODE_START(p_cc_pdb->p_glb_pdb,ICF_FALSE)

		if ((ICF_FAILURE != ret_val) && (ICF_FALSE == p_cc_pdb->mms_sent))
		{
            if ((ICF_NULL == ICF_CC_CHECK_COMMON_BITMASK(p_cc_pdb,
                            ICF_CALL_MEDIA_SETUP_TYPE))||
                    ((ICF_NULL != ICF_CC_CHECK_COMMON_BITMASK(p_cc_pdb,
                                   ICF_CALL_MEDIA_SETUP_TYPE)) && 
                     (ICF_FALSE == p_cc_pdb->p_call_ctx->inband_info_present)))
            {
                /*
                 * Send media param to the SSA
                 */
                if (ICF_TRUE == is_early_media_required && 
					ICF_FAILURE == icf_cc_send_ssa_media_param(p_cc_pdb))
                {
                    ret_val = ICF_FAILURE;
                } /* if (ICF_FAILURE == icf_c... */
                else
                {
                    ICF_CC_TRACE(p_cc_pdb, ICF_TRACE_PRACK_REQD)
            
                    if (ICF_TRUE == p_cc_pdb->prack_reqd)
                    {
                        /*
                         * Update the states in the call context
                         */
                        ICF_CC_SET_CALL_STATE(p_cc_pdb,ICF_CC_09_AWT_MEDIA_PARAM_PRACK)
                    } /* if (ICF_TRUE == p_cc_pdb->p... */
                    else
                    {
                        if (ICF_FAILURE == icf_cc_send_ssa_alert(p_cc_pdb))
                        {
                            ret_val = ICF_FAILURE;
                        } /* if (ICF_FAILURE == icf_c... */
                        else
                        {
                            /*
                             * Update the states in the call context
                             */
                            ICF_CC_SET_CALL_STATE(p_cc_pdb,ICF_CC_05_AWT_CONNECT)
                        } /* else */
                    } /* else */
                } /* else */
            } /* (ICF_FALSE == p_cc_pdb->p_c... */

            else if((ICF_NULL != ICF_CC_CHECK_COMMON_BITMASK(p_cc_pdb,
                            ICF_CALL_MEDIA_SETUP_TYPE)) && 
                    (ICF_TRUE == p_cc_pdb->p_call_ctx->inband_info_present))
            {

                ICF_CC_TRACE(p_cc_pdb, ICF_TRACE_MEDIA_SETUP_TYPE)
                ICF_CC_TRACE(p_cc_pdb, ICF_TRACE_INBAND_INFO)
                ICF_CC_TRACE(p_cc_pdb, ICF_TRACE_PRACK_REQD)
    
                if (ICF_TRUE == is_early_media_required)
				{
					ret_val = icf_cc_send_ssa_alert_media(p_cc_pdb);
				}
				else
				{
					ret_val = icf_cc_send_ssa_alert(p_cc_pdb);
				}
				if ( ICF_FAILURE == ret_val)
				{
					/*Nothing to do*/
				}
                else if (ICF_TRUE == p_cc_pdb->prack_reqd)
                {
                    /*
                     * Update the states in the call context
                     */
                    ICF_CC_SET_CALL_STATE(p_cc_pdb,ICF_CC_10_AWT_ALERT_PRACK)
                } /* if (ICF_TRUE == p_cc_pdb->p... */
                else
                {
                    /*
                     * Update the states in the call context
                     */
                    ICF_CC_SET_CALL_STATE(p_cc_pdb,ICF_CC_05_AWT_CONNECT)
                } /* else */
            } /* (ICF_TRUE == p_cc_pdb->p_ca... */
        }/*End of if (ICF_FAILURE != ret_val)*/
ICF_CHECK_TUNNEL_MODE_END
#endif

        /* The following code snippet should not be executed for a 
           QoS call.
        */ 
#ifdef ICF_QOS_SUPPORT    
ICF_CHECK_QOS_SUPPORT_START(p_cc_pdb->p_glb_pdb,ICF_FALSE)    
ICF_CHECK_TUNNEL_MODE_START(p_cc_pdb->p_glb_pdb,ICF_TRUE)
        /* In tunnel mode, if the application has set early_media_reqd as TRUE,
           we will send 180 with SDP, else we will send 180 without SDP and
           got call state 05 or 10 (depending on PRACK support). 
        */
     	if((ICF_FAILURE != ret_val) && (ICF_FALSE == p_cc_pdb->mms_sent))
		{
            if(ICF_FALSE == p_cc_pdb->p_call_ctx->inband_info_present)
            {
                /* 180 without SDP */
                if(ICF_FAILURE == icf_cc_send_ssa_alert(p_cc_pdb))
	            {
        	        ret_val = ICF_FAILURE;
                }
                else if(ICF_TRUE == p_cc_pdb->prack_reqd)
	            {	 
        	        ICF_CC_SET_CALL_STATE(p_cc_pdb,ICF_CC_10_AWT_ALERT_PRACK)
    	        }
         	    else
         	    {
                    ICF_CC_SET_CALL_STATE(p_cc_pdb,ICF_CC_05_AWT_CONNECT)
                } 
           }
           else if(ICF_TRUE == p_cc_pdb->p_call_ctx->inband_info_present)
           {
               /* 180 with SDP */
               if(ICF_FAILURE == icf_cc_send_ssa_alert_media(p_cc_pdb))
	           {
	               ret_val = ICF_FAILURE;
               }
               else if(ICF_TRUE == p_cc_pdb->prack_reqd)
	           {	 
	               ICF_CC_SET_CALL_STATE(p_cc_pdb,ICF_CC_10_AWT_ALERT_PRACK)
	           }
	           else
	           {
                   ICF_CC_SET_CALL_STATE(p_cc_pdb,ICF_CC_05_AWT_CONNECT)
	           }
              /*  Rel 8.1 
               *  SDP has been sent in 180 so
               *  set the media committed true
               */ 
              if(ICF_SUCCESS == ret_val)
              {
                  /*  If it FS call then set teh media_committed to true */
                  if((ICF_NULL != ICF_CC_CHECK_COMMON_BITMASK(p_cc_pdb,
                                               ICF_CALL_MEDIA_SETUP_TYPE)))
                  {
                      p_cc_pdb->p_call_ctx->media_committed = ICF_TRUE;
                  }
              }
           }
        }
ICF_CHECK_TUNNEL_MODE_END
ICF_CHECK_QOS_SUPPORT_END 
#endif
	} /* end else */
    
    /* In tunnel mode, the following mode related bit is not to be processed. */
ICF_CHECK_TUNNEL_MODE_START(p_cc_pdb->p_glb_pdb,ICF_FALSE)
    /* this bit mask is set so that we will not send the media 
     * mode of the APPlICATION in the further APIs */
    p_cc_pdb->p_call_ctx->common_bitmask |= ICF_MEDIA_MODE_APPLIED_FROM_APP;
ICF_CHECK_TUNNEL_MODE_END
    

    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */
    return ret_val;
}


/******************************************************************************
 *
 * FUNCTION:        icf_cc_03_pa_call_connect
 *
 * DESCRIPTION:     This function is invoked when the CC is awaiting 
 *                  ALERT RESP from the PA(incoming call) and
 *                  call CONNECT is received from the PA.
 *                  Current State: ICF_CC_03_AWT_ALERT_RESP
 *                  Current Event: ICF_CC_PA_CALL_CONNECT
 *
 *****************************************************************************/
icf_return_t icf_cc_03_pa_call_connect(INOUT icf_cc_pdb_st *p_cc_pdb)
{
    icf_return_t     ret_val = ICF_SUCCESS;
ICF_SESSION_CONTROL_START
	icf_pa_cc_connect_req_st			*p_connect = ICF_NULL;
ICF_SESSION_CONTROL_END

    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /* function entry trace */

ICF_SESSION_CONTROL_START

	p_connect = (icf_pa_cc_connect_req_st*)
						p_cc_pdb->p_glb_pdb->p_recv_msg->payload;
ICF_CHECK_TUNNEL_MODE_START(p_cc_pdb->p_glb_pdb,ICF_TRUE)
    if(ICF_TRUE == p_cc_pdb->p_call_ctx->call_modify_req_ongoing)
    {   
         /*SPR 19491 This is to avoid sending the 200 when UPDATE is recv
           fromm peer and MMS is done for UPDATE and instead of call modify
           cfm PA sends connect_req , we will reject this, it is expected that
           PA sends first call_modify_cfm and then connect_req*/
           ret_val = icf_cc_send_pa_unexpected_msg_ind(p_cc_pdb);
           ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */
           return ret_val;
     }
ICF_CHECK_TUNNEL_MODE_END

	/* copy the additional parameters from payload into call ctx */
	if (ICF_FAILURE == icf_cc_proc_pa_connect_payload(p_cc_pdb))
	{
		ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb)
		return ret_val;
	}

    /* In tunnel mode, we will not process stream list and the relevant 
       code will be bypassed. Also on receiving connect request from the
       application, we will not do a tunnelled MMS request.
    */   
ICF_CHECK_TUNNEL_MODE_START(p_cc_pdb->p_glb_pdb,ICF_FALSE)
	if (ICF_STREAM_LIST_PRESENT == (p_connect->bit_mask & ICF_STREAM_LIST_PRESENT))
    {
		if (ICF_NULL != p_connect->p_stream_list)
		{
           p_cc_pdb->p_list_1 = p_connect->p_stream_list; 	
	       if (ICF_FAILURE == icf_cc_process_app_stream_capab_req(p_cc_pdb))
	       {
		     if (ICF_STREAM_ID_VALIDATION_FAIL == 
			 (p_cc_pdb->common_bitmask & ICF_STREAM_ID_VALIDATION_FAIL))
		     {
			    ret_val = icf_cc_03_generic_err(p_cc_pdb);
			    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */
			    return ret_val;
		     }
		     return ret_val;
	      }
        }
    }


	/* Check the call type and stream list to take further action */
    if(ICF_TRUE == p_cc_pdb->p_call_ctx->media_committed)
    {	
		if (ICF_CONNECT_DELETED_STREAM_PRESENT == 
		(p_connect->bit_mask & ICF_CONNECT_DELETED_STREAM_PRESENT))
		{
			p_cc_pdb->count = p_connect->count;
			p_cc_pdb->p_generic_ptr_1 = p_connect->deleted_media_streams;

			/* Following function will check if there is any stream 
			 * deleted by user.
			 */
			ret_val = icf_cc_check_for_streams_deleted_by_user(p_cc_pdb);

			/* ith level check was NOT performed inside the above function,
			 * This bit is just being checked if there was any exception occured inside
			 * the above function (Like stream id deleted(if there was any) but it was not
			 * present inside the list of sdps received from SSA.
			 */
			if (ICF_CC_ALL_STREAMS_DELETED == 
			(p_cc_pdb->common_bitmask & ICF_CC_ALL_STREAMS_DELETED))
			{
				ret_val = icf_cc_03_generic_err(p_cc_pdb);
				ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */
				return ret_val;
			}
			else if (icf_cc_pa_connect_send_mms_req(p_cc_pdb) == ICF_FAILURE)
			{
				ret_val=ICF_FAILURE;
			}
			else
			{
				ICF_CC_SET_CALL_STATE(p_cc_pdb, ICF_CC_04_AWT_MMS_RESP)
				ICF_CC_SET_ONGOING_EVENT(p_cc_pdb,ICF_CC_PA_CALL_CONNECT)
		
				ICF_CC_SET_NEXT_STATE(p_cc_pdb, ICF_CC_11_AWT_SSA_CONNECT_ACK)
			}
		}
		else
		{

			/* Check if media ongoing is true */
			ICF_CC_TRACE(p_cc_pdb, ICF_TRACE_MEDIA_ONGOING)
			if(ICF_INT_MODULE_INVALID == ICF_CC_GET_MEDIA_ONGOING(p_cc_pdb))
			{
				/*
				 *  Set the media_state bitmask in the call context
				 */
				ICF_CC_TRACE(p_cc_pdb, ICF_CC_PA_CALL_CONNECT)
                
				/*
				 * Send Modify Media Session to the MMI
				 */
				 ret_val = icf_cc_snd_mms_pa_connect(p_cc_pdb);
				 
			}
			else
			{
				ICF_CC_SET_PENDING_EVENT(p_cc_pdb,
						ICF_PENDING_EVENT_PA_CALL_CONNECT)
				ICF_CC_SET_NEXT_STATE(p_cc_pdb,ICF_CC_11_AWT_SSA_CONNECT_ACK)  
			}
		}
    }
    else
    /*If call is not commited or mms was not sent for PA connect 
    if(ICF_FALSE == p_cc_pdb->p_call_ctx->media_committed ||
                     ICF_FALSE == p_cc_pdb->mms_sent)*/
    {
        if (ICF_CONNECT_DELETED_STREAM_PRESENT == 
		(p_connect->bit_mask & ICF_CONNECT_DELETED_STREAM_PRESENT))
		{
			p_cc_pdb->count = p_connect->count;
			p_cc_pdb->p_generic_ptr_1 = p_connect->deleted_media_streams;

			/* Following function will check if there is any stream 
		     * deleted by user.
			 */
			ret_val = icf_cc_check_for_streams_deleted_by_user(p_cc_pdb);
	

			/* ith level check was NOT performed inside the above function,
			 * This bit is just being checked if there was any exception occured inside
			 * the above function (Like stream id deleted(if there was any) but it was not
			 * present inside the list of sdps received from SSA.
			 */
			if (ICF_CC_ALL_STREAMS_DELETED == 
				(p_cc_pdb->common_bitmask & ICF_CC_ALL_STREAMS_DELETED))
			{
				ret_val = icf_cc_05_generic_err(p_cc_pdb);
				ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */
				return ret_val;
			}
			if (icf_cc_send_mms_req(p_cc_pdb) == ICF_FAILURE)
			{
				ret_val=ICF_FAILURE;
			}
			else
			{
                if(ICF_TRUE == p_cc_pdb->mms_sent)
                {
				    ICF_CC_SET_CALL_STATE(p_cc_pdb, ICF_CC_04_AWT_MMS_RESP)
				    ICF_CC_SET_ONGOING_EVENT(p_cc_pdb,ICF_CC_PA_CALL_CONNECT)
                    ICF_CC_SET_NEXT_STATE(p_cc_pdb, ICF_CC_11_AWT_SSA_CONNECT_ACK)
                }
                else
                {
                    /*Send connect to SSA and set state as await connect ack*/
                    icf_cc_mms_rsp_pa_connect(p_cc_pdb);
                }
			}

		}
        else
        {
            /*Send connect to SSA and set state as await connect ack*/
            icf_cc_mms_rsp_pa_connect(p_cc_pdb);

            ICF_CC_SET_CALL_STATE(p_cc_pdb,ICF_CC_11_AWT_SSA_CONNECT_ACK)
        }

    }
ICF_CHECK_TUNNEL_MODE_END  

    /* Tunnel mode handling of connect request from application.*/
ICF_CHECK_TUNNEL_MODE_START(p_cc_pdb->p_glb_pdb,ICF_TRUE)
ICF_CHECK_QOS_SUPPORT_START(p_cc_pdb->p_glb_pdb,ICF_TRUE)
        /*If resource reservation is not yet completed that means UPDATE-200OK
          is not done yet for QOS therefore if alert is received then we shall
          be marking it as pending and after sending 200OK for the UPDATE in
          state 14 we shall process this pending event this was done when
          call modify cfm is recv*/
       /*SPR19574 Added the media_connect_sent check for the sceanrio when
         during an incoming QOS call in tunnel mode media_connect_req is
         followed by alert then we should not send the 183 twice. 
        in function icf_cc_prep_media_param_for_qos media_connect_sent is set
        to true after sending 183*/
        if((ICF_FALSE == p_cc_pdb->p_call_ctx->resource_reservation_completed)
            && (ICF_FALSE == p_cc_pdb->p_call_ctx->media_connect_sent))
        {   
          /* To process 200 wo SDP after sending 200 OK for precondition
             UPDATE */
             ICF_CC_SET_PENDING_EVENT(p_cc_pdb,
                ICF_PENDING_EVENT_PA_CALL_CONNECT)
             /*Send the 183 SDP as now it is not triggered in 01 state
              after cms resp, also on recv 180/183/200 from PA we need to send
              183 to NW in case of tunnel QOS call.*/
          	 if ((ICF_FAILURE != ret_val) && (ICF_FALSE ==
                         p_cc_pdb->mms_sent))
		     {
                  ret_val = icf_cc_prep_media_param_for_qos(p_cc_pdb);
             }
        }
        else
        {    
            /*  set the media committed true */
            p_cc_pdb->p_call_ctx->media_committed = ICF_TRUE;
            /* This function will send 200 to the peer. */
            ret_val = icf_cc_snd_mms_pa_connect(p_cc_pdb);
            if(ICF_SUCCESS == ret_val)
            {
                ICF_CC_SET_CALL_STATE(p_cc_pdb,ICF_CC_11_AWT_SSA_CONNECT_ACK)
            }
        }    
ICF_CHECK_QOS_SUPPORT_END
ICF_CHECK_QOS_SUPPORT_START(p_cc_pdb->p_glb_pdb,ICF_FALSE)
       /*  set the media committed true in case of FS call*/
       if((ICF_NULL != ICF_CC_CHECK_COMMON_BITMASK(p_cc_pdb,
                              ICF_CALL_MEDIA_SETUP_TYPE)))
       {
             p_cc_pdb->p_call_ctx->media_committed = ICF_TRUE;
       }
       /* This function will send 200 to the peer. */
       ret_val = icf_cc_snd_mms_pa_connect(p_cc_pdb);
       if(ICF_SUCCESS == ret_val)
       {
           ICF_CC_SET_CALL_STATE(p_cc_pdb,ICF_CC_11_AWT_SSA_CONNECT_ACK)
       }
ICF_CHECK_QOS_SUPPORT_END
ICF_CHECK_TUNNEL_MODE_END

    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */
    return ret_val;
}



/******************************************************************************
 *
 * FUNCTION:        icf_cc_03_ssa_call_cancel_req
 *
 * DESCRIPTION:     This function is invoked when the CC is awaiting
 *                  ALERT RESP from the PA(incoming call) and
 *                  it receives a Call Cancel Request from SSA
 *                  (SIP CANCEL received from peer).
 *                  Current State: ICF_CC_03_AWT_ALERT_RESP
 *                  Current Event: ICF_CC_SSA_CALL_CANCEL_REQ
 *
 *****************************************************************************/

icf_return_t icf_cc_03_ssa_call_cancel_req(
        INOUT icf_cc_pdb_st *p_cc_pdb)
{
    icf_return_t     ret_val = ICF_SUCCESS;

    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /* function entry trace */


    /*
     * As the handling for this event is the same in the state
     * ICF_CC_01_AWT_CMS_RESP,call their
     * state handler
     */
    ret_val = icf_cc_01_ssa_call_cancel_req(p_cc_pdb);

	/*
	 * Hadling of cancel request in state 1 set PA cleared bit for incomming call. 
	 * Since we are call the same handler in state 3 so we should reset this bit 
	 * as the PA interface would also need to be cleared 
	 * So Reset PA cleared bit 
	 */
    ICF_CC_RESET_CALL_CLR_STATUS(p_cc_pdb,
            ICF_CALL_CLEAR_STATUS_PA_CLEARED)

    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */
    return ret_val;
}


/******************************************************************************
 *
 * FUNCTION:        icf_cc_03_generic_err
 *
 * DESCRIPTION:     This function is invoked when the CC is awaiting
 *                  create media session resp from MMI and it receives
 *                  an error indication from MMI..
 *                  Current State: ICF_CC_03_AWT_ALERT_RESP
 *                  Current Event: ICF_CC_GENERIC_ERROR
 *
 *****************************************************************************/

icf_return_t icf_cc_03_generic_err (
        INOUT icf_cc_pdb_st *p_cc_pdb)
{
    icf_return_t     ret_val = ICF_SUCCESS;

    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /* function entry trace */

    /* Fix For SPR:20078*/    
    /* While handling the generic error in ICF_CC_03_AWT_ALERT_RESP if the
     * value of is_call_modify_ongoing is set to TRUE it indicates that we
     * have received ICF_CALL_MODIFY_REQ with/without sdp and this  API need to
     * be responded with failure ICF_CALL_MODIFY_RESP .We are handling this scenario in
     * icf_cc_14_generic_err.
     */
    if (ICF_TRUE == p_cc_pdb->p_call_ctx->is_call_modify_ongoing)
    {
        ret_val = icf_cc_14_generic_err(p_cc_pdb);
        p_cc_pdb->p_call_ctx->is_call_modify_ongoing = ICF_FALSE;
    }
    else
    {

        /* Set release type as internal error */
        ICF_CC_SET_RELEASE_TYPE(p_cc_pdb, ICF_REL_TYPE_INTERNAL_INITIATED)
        ICF_CC_TRACE(p_cc_pdb, ICF_TRACE_REL_TYPE_INT_ERR)

        /* Stop Alert timer and start Release timer */
        if(ICF_FAILURE == icf_cc_start_release_timer(p_cc_pdb))
        {
            ret_val = ICF_FAILURE;
        }
        /*
         * Send a DMS request to MMI
         */
        else if (ICF_FAILURE == icf_cc_send_dms_req(p_cc_pdb))
        {
            ret_val = ICF_FAILURE;
        }
        /*
         * Send call clear request to the SSA
         */
        else if (ICF_FAILURE == icf_cc_send_clear_req_to_ssa(p_cc_pdb))
        {
            ret_val = ICF_FAILURE;
        } /* if (ICF_FAILURE == icf_c... */    
        else 
        {
            /*
             * Update the states in the call context
             */
            ICF_CC_SET_CALL_STATE(p_cc_pdb,ICF_CC_07_AWT_DMS_RESP)
        }
    }

    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */
    return ret_val;
}

#ifdef ICF_QOS_SUPPORT
/******************************************************************************
 *
 * FUNCTION:        icf_cc_03_media_prack_resp
 *
 * DESCRIPTION:     This function is invoked when the CC is awaiting ALERT
 *                  Response from SSA and remote sends response for PRACK
 *                  of 183.
 *                  Current State: ICF_CC_03_AWT_ALERT_RESP
 *                  Current Event: ICF_CC_MEDIA_PARAM_PRACK_RESP
 *
 *****************************************************************************/
icf_return_t icf_cc_03_media_prack_resp(icf_cc_pdb_st *p_cc_pdb)
{
    icf_return_t     ret_val = ICF_SUCCESS;
    icf_nw_inc_prack_resp_st *p_prack_resp = ICF_NULL;


    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /* function entry trace */

    ICF_ASSERT(p_cc_pdb->p_glb_pdb, p_cc_pdb->p_internal_msg->p_msg_data)

    p_prack_resp = (icf_nw_inc_prack_resp_st*)
            p_cc_pdb->p_internal_msg->p_msg_data;

    if (ICF_TRUE == p_prack_resp->status)
    {
        /* Set a flag in call context which indicates that successful 
         * response for PRACK is received and UPDATE can be sent for Qos.
         */
        p_cc_pdb->p_call_ctx->received_successful_prack_resp = ICF_TRUE;

ICF_CHECK_QOS_SUPPORT_START(p_cc_pdb->p_glb_pdb,ICF_TRUE)
                if(ICF_SUCCESS == 
                        icf_cc_match_curr_des_status_on_local_nw(p_cc_pdb))
                {
                    /* We have to Send UPDATE request to Peer */
                    ret_val = icf_cc_ssa_send_update_msg(p_cc_pdb);
                    /*SPR 18282 Now as we need to send CANCEL for QOS INV
                      in case 180/183/200OK for INV comes without QOS lines
                      before qos negotiation happens. However if resource
                      reservation is done then if in SDP we do not have QOS
                     attributes which are no more required we will not fail
                     the call. To identify that resource reservation is done
                     we will set the flag resource reservation completed.
                     Later when 200OK for UPDATE comes we will set the bitmask
                     for the same. This is to avoid situation when after
                     UPDATE is sent then 180 comes without QOS lines then we
                     should send CANCEL.*/
                    p_cc_pdb->p_call_ctx->resource_reservation_completed = 
                        ICF_TRUE;
                }
ICF_CHECK_QOS_SUPPORT_END
    }
    else
    {
        /* Set call clear reason in call context */
		p_cc_pdb->p_call_ctx->release_cause = (icf_uint8_t)p_prack_resp->error_cause;

		/* Clear the call */
        /*  Fix for CSR 1-7507275 : SPR 19881 */
        ret_val = icf_cc_03_generic_err(p_cc_pdb);
        /*ret_val = icf_cc_clear_call_context(p_cc_pdb);*/
    }
    return ret_val;
}
#endif

/******************************************************************************
 *
 * FUNCTION:        icf_cc_03_media_update_resp
 *
 * DESCRIPTION:     This function is invoked when the CC is awaiting ALERT
 *                  Response from SSA and remote sends response for UPDATE
 *                  for Qos processing.
 *                  Current State: ICF_CC_03_AWT_ALERT_RESP
 *                  Current Event: ICF_CC_MEDIA_UPDATE_RESP
 *
 *****************************************************************************/
icf_return_t icf_cc_03_media_update_resp(icf_cc_pdb_st *p_cc_pdb)
{
    icf_return_t     ret_val = ICF_SUCCESS;
    icf_nw_inc_media_change_resp_st *p_update_resp = ICF_NULL;

    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /* function entry trace */

    ICF_ASSERT(p_cc_pdb->p_glb_pdb, p_cc_pdb->p_internal_msg->p_msg_data)

    p_update_resp = (icf_nw_inc_media_change_resp_st*)
            p_cc_pdb->p_internal_msg->p_msg_data;

    /* SPR 19760 : The previous check for Call Modify without SDP is being 
     * replaced by check of Application triggered call modify request
     * as otherwise the for UPDATE sent with SDP , no response will be
     * given to application.
     */ 
    if (ICF_NULL != ICF_CC_CHECK_COMMON_BITMASK_2(p_cc_pdb,
                ICF_APP_CALL_MODIFY_REQ_ONGOING))
    {
          ret_val = icf_cc_stop_current_timer(p_cc_pdb);

          /*Send response without SDP*/
          /*Send call modify_resp to application*/
          if (ICF_FALSE == p_update_resp->status)
          {
                 /* Failure resp is received then send failure to PA */
                  p_cc_pdb->p_call_ctx->app_failure_cause = 
                                ICF_CAUSE_NETWORK_ERROR;
                     ICF_CC_SET_COMMON_BITMASK(p_cc_pdb, ICF_CALL_MODIFY_RESP_FAIL)
           }
      
          if(ICF_FAILURE == icf_cc_send_pa_call_modify_resp(p_cc_pdb))
          {
                ret_val = ICF_FAILURE;
          }
          else
          {
                 ret_val = ICF_SUCCESS;
          }
   }

ICF_CHECK_QOS_SUPPORT_START(p_cc_pdb->p_glb_pdb,ICF_TRUE)
    if (ICF_FALSE == p_update_resp->status)
    {

		/* We will clear call generically */
		ICF_CC_SET_STATUS(p_cc_pdb,ICF_STATUS_CALL_R_SRV_TERMINATE)
		ret_val = ICF_SUCCESS;

    }
	else if(ICF_NULL != p_update_resp->p_sdp_list)
	{
		icf_cc_delete_inc_media_sdp_info_list(p_cc_pdb,&p_update_resp->p_sdp_list);
	}
ICF_CHECK_QOS_SUPPORT_END
    return ret_val;
}

/******************************************************************************
 *
 * FUNCTION:        icf_cc_03_forceful_call_clear
 *
 * DESCRIPTION:     This function is invoked when Forceful call clear is 
 *                  received in state ICF_CC_03_AWT_ALERT_RESP 
 *                  Current State: ICF_CC_03_AWT_ALERT_RESP
 *                  Current Event: ICF_CC_FORCEFUL_CLEAR
 *
 *****************************************************************************/
icf_return_t icf_cc_03_forceful_call_clear(
        INOUT icf_cc_pdb_st *p_cc_pdb)
{
    icf_return_t ret_val = ICF_SUCCESS;
    
    /*SPR 20186 invoke clear pending event function for sending 
      the response for the UPDATE.
      On forceful call clear previously the failure response
      for the UPDATE was not being sent and thus MIU for transaction
      was observed */
    icf_cc_clear_pending_events(p_cc_pdb);

    ICF_CC_SET_RELEASE_CAUSE(p_cc_pdb,ICF_REL_CAUSE_SERV_INT_ERR)    
    /*
     * Send call clear request to the SSA
     */
    icf_cc_send_clear_req_to_ssa(p_cc_pdb);
 
    /*Handling would be same as that of forceful call clear in atate IDLE*/
    icf_cc_00_clear_call (p_cc_pdb);

    return ret_val;    
}


/******************************************************************************
 *
 * FUNCTION:        icf_cc_03_pa_media_connect_req
 *
 * DESCRIPTION:     This function is invoked when the CC is awaiting 
 *                  ALERT RESP from the PA(incoming call) and
 *                  media_connect request from PA to send 183 with SDP is
 *                  received.
 *                  Current State: ICF_CC_03_AWT_ALERT_RESP
 *                  Current Event: ICF_CC_PA_MEDIA_CONNECT_REQ 
 *
 *****************************************************************************/
icf_return_t icf_cc_03_pa_media_connect_req(
        INOUT icf_cc_pdb_st *p_cc_pdb)
{
    icf_return_t                      ret_val = ICF_SUCCESS;
    icf_pa_cc_media_connect_req_st    *p_media_connect = ICF_NULL;
    icf_config_data_st                *p_config_data = ICF_NULL;

    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /* function entry trace */
    
    ret_val = icf_dbm_get_module_glb_data(
                p_cc_pdb->p_glb_pdb,
               (icf_int_module_id_t)ICF_INT_MODULE_CFG,
               (icf_void_t *)&p_config_data, p_cc_pdb->p_ecode);
    if(ICF_FAILURE == ret_val)
    {
        ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb)
        return ret_val;
    }
    p_media_connect = ((icf_pa_cc_media_connect_req_st *)
                p_cc_pdb->p_glb_pdb->p_recv_msg->payload);
    ICF_CC_TRACE(p_cc_pdb, ICF_TRACE_MEDIA_SETUP_TYPE)
    ICF_CC_TRACE(p_cc_pdb, ICF_TRACE_INBAND_INFO)
    ICF_CC_TRACE(p_cc_pdb, ICF_TRACE_PRACK_REQD)
ICF_CHECK_TUNNEL_MODE_START(p_cc_pdb->p_glb_pdb,ICF_TRUE)
    if(ICF_TRUE == p_cc_pdb->p_call_ctx->call_modify_req_ongoing)
    {   
         /*SPR 19491 This is to avoid sending the 183 when UPDATE is recv
           fromm peer and MMS is done for UPDATE and instead of call modify
           cfm PA sends media_connect_req , we will reject this, it is expected that
           PA sends first call_modify_cfm and then media_connect_req*/
           ret_val = icf_cc_send_pa_unexpected_msg_ind(p_cc_pdb);
           ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */
           return ret_val;
     }
ICF_CHECK_TUNNEL_MODE_END
    /*  Rel 8.1 changes Start */
    if(ICF_MEDIA_CONNECT_REQ_PRACK_VALUE_PRESENT & 
         p_media_connect->bit_mask)
    {
        /*  If INVITE received with 100rel in Require header
         *  and in first 18x API PRACK is OFF then clear the
         */
        if(ICF_PER_CALL_PRACK_FEATURE_ENABLED  != 
           (ICF_CC_CHECK_COMMON_BITMASK_2(p_cc_pdb,
            ICF_PER_CALL_PRACK_FEATURE_ENABLED)))
        {
            /* It is the case when ICF_NO_PRACK is sent by application
             * and Peer send 100rel in require header of INVITE
             * then clear the call.
             */
            if((ICF_NO_PRACK == p_media_connect->prack_value) &&
               (ICF_REL_PROV_RESP_REQUIRED_BY_PEER == 
               (ICF_CC_CHECK_COMMON_BITMASK_2(p_cc_pdb,
                ICF_REL_PROV_RESP_REQUIRED_BY_PEER))))
            {
                p_cc_pdb->p_call_ctx->release_cause =
                       ICF_CALL_CLEAR_REASON_PRACK_CALL_NOT_ALLOWED;
                ret_val = icf_cc_03_generic_err(p_cc_pdb);
                p_cc_pdb->p_call_ctx->release_cause = 
                       ICF_CALL_CLEAR_REASON_NON_PRACK_CALL_NOT_ALLOWED;
                ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb)
                return ret_val;
            }
            /* It is the case when ICF_PRACK_IN_REQUIRE is sent by application
             * and peer neither send 100rel in require nor in supported 
             * header of INVITE
             * then clear the call.
             */
            else if((ICF_REL_PROV_RESP_SUPP_BY_PEER != 
               (ICF_CC_CHECK_COMMON_BITMASK(p_cc_pdb,
                ICF_REL_PROV_RESP_SUPP_BY_PEER))) &&
               ICF_PRACK_IN_REQUIRE  == p_media_connect->prack_value)
            {
                p_cc_pdb->p_call_ctx->release_cause = 
                       ICF_CALL_CLEAR_REASON_NON_PRACK_CALL_NOT_ALLOWED;
                ret_val = icf_cc_03_generic_err(p_cc_pdb);
                ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb)
                return ret_val;
            }

    /*  If PRACK Field came in the API then check in call ctx that is is
     *  already populated or not if already populated then ignore
     *  it because then it will not be the first 18x request.
     *  else copy it in call ctx and set the common bitmask and 
     *  update the prack_reqd field in cc pdb, because all the decisions
     *  were taken for the prack on this field of cc pdb
     */
            p_cc_pdb->p_call_ctx-> prack_value =  
                                  p_media_connect->prack_value;
            ICF_CC_SET_COMMON_BITMASK_2(p_cc_pdb, 
                ICF_PER_CALL_PRACK_FEATURE_ENABLED)
            if(ICF_PRACK_IN_REQUIRE == p_media_connect->prack_value)
            {
                p_cc_pdb->prack_reqd = ICF_TRUE;
            }
            else
            {
                p_cc_pdb->prack_reqd = ICF_FALSE;
            }
        }
    }
    /* If INVITE received with 100rel in Require header
     * and in first 18x API PRACK field is not received 
     * and the system level flag for PRACK is also OFF 
     * then clear the
     */

    else if(ICF_PER_CALL_PRACK_FEATURE_ENABLED != 
           (ICF_CC_CHECK_COMMON_BITMASK_2(p_cc_pdb,
            ICF_PER_CALL_PRACK_FEATURE_ENABLED)))
    {
        /* SPR 19754 : In case system level flag is OFF and 
         * invite is received with 100rel in require then
         * establish a PRACK call irrespective of user
         * configuration
         */
        p_cc_pdb->p_call_ctx->system_level_configuration_for_prack = ICF_TRUE;
    }
    /*  Rel 8.2 changes End */

    /* check if privacy header needs to be sent */
    if (ICF_PRIVACY_IND_PRESENT & p_media_connect->bit_mask)
    {
        p_cc_pdb->p_call_ctx->send_privacy_hdr = p_media_connect->privacy_ind;
    }

    /* SPR 19590 : code removed for cloning header/tag/body list from 
     * here as this shall be taken care in icf_cc_convert_api_to_event
     * function.
     */

    /* In tunnel mode we will not process the media_mode field and will
       not do any processing based on it.Also no stream list processing will
       be done and the relevant code snippet will be completely bypassed.
    */   
ICF_CHECK_TUNNEL_MODE_START(p_cc_pdb->p_glb_pdb,ICF_FALSE)    
    /* Mode related change added by Sumit */
    if((ICF_SUCCESS == ret_val) &&
            (ICF_MEDIA_CONNECT_MEDIA_MODE_PRESENT & p_media_connect->bit_mask))
    {
      if(ICF_MEDIA_MODE_SEND_RECV != p_media_connect->media_mode &&
          ICF_MEDIA_MODE_SEND_ONLY != p_media_connect->media_mode)
       {
         p_media_connect->bit_mask &= ~(ICF_MEDIA_CONNECT_MEDIA_MODE_PRESENT);
         /*Check mode and raise alarm in case invlaid*/
         ICF_PRINT(((icf_uint8_t*)"[Alarm] Application requested mode is not SENRRECV or \
           SENDONLY in ICF_MEDIA_CONNECT_REQ. Picking up \
           default mode SENDONLY"));
       }
      else
       {
          p_cc_pdb->application_mode = p_media_connect->media_mode;
       }
    }/* if((ICF_SUCCESS == re ... */
    /* End Mode related change added by Sumit */
          
    if (ICF_STREAM_LIST_PRESENT == (p_media_connect->bit_mask & ICF_STREAM_LIST_PRESENT))
    {
        if (ICF_NULL != p_media_connect->p_stream_list)
        {
            p_cc_pdb->p_list_1 = p_media_connect->p_stream_list; 	
            if (ICF_FAILURE == icf_cc_process_app_stream_capab_req(p_cc_pdb))
            {
                if (ICF_STREAM_ID_VALIDATION_FAIL == 
                        (p_cc_pdb->common_bitmask & ICF_STREAM_ID_VALIDATION_FAIL))
                {
                    ret_val = icf_cc_03_generic_err(p_cc_pdb);
                    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */
                        return ret_val;
                }
                return ret_val;
            }
            if(ICF_OFFER_CODECS_IN_GCL == (p_cc_pdb->p_call_ctx->common_bitmask\
                         & ICF_OFFER_CODECS_IN_GCL) ||
                         (ICF_MEDIA_CONNECT_MEDIA_MODE_PRESENT &
                         p_media_connect->bit_mask))
             {
                 if (ICF_FAILURE == icf_cc_send_mms_req(p_cc_pdb))
                 {
                   ret_val = ICF_FAILURE;
                 }
                 else
                 {
                   /*
                    * Update the states in the call context
                    */
                   ICF_CC_SET_CALL_STATE(p_cc_pdb,ICF_CC_04_AWT_MMS_RESP)
                   ICF_CC_SET_NEXT_STATE(p_cc_pdb,ICF_CC_05_AWT_CONNECT)
                 }
             }    
        }
    }
ICF_CHECK_TUNNEL_MODE_END   

    /* The following code snippet checks for the field send_without_sdp in the media
       connect request, If it is set to ICF_FALSE, we will send a 183 without
       SDP and change call state to 05 or 09 (depending on PRACK support).
    */   
    
    /* Check if the application wishes to send SIP message without SDP */
    if ((ICF_SUCCESS == ret_val) &&
		(ICF_TRUE == p_media_connect->send_without_sdp))
	{
		ICF_PRINT(((icf_uint8_t*)"\n[CC]: Request to send SIP response without SDP\n"));

		if (ICF_FAILURE == icf_cc_send_ssa_media_param_wo_sdp(p_cc_pdb))
		{
			ret_val = ICF_FAILURE;
		}
		else if (ICF_TRUE == p_cc_pdb->prack_reqd)
		{
			/* wait for Prack from network */
			ICF_CC_SET_CALL_STATE(p_cc_pdb, ICF_CC_09_AWT_MEDIA_PARAM_PRACK)
			/* set flag so that 180 is not sent on receiving Prack */
			ICF_CC_SET_COMMON_BITMASK(p_cc_pdb, ICF_PRACK_FOR_MEDIA_CONNECT)
		}
		else
		{
			/* wait for connect from app */
			ICF_CC_SET_CALL_STATE(p_cc_pdb, ICF_CC_05_AWT_CONNECT)
		}

		/* no need to go down in the function and look for other parameters */	
		ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb)
		return ret_val;
	}

    /* In tunnel mode we will not process the media_mode field and will
       not do any processing based on it.Also no stream list processing will
       be done and the relevant code snippet will be completely bypassed.
    */
ICF_CHECK_TUNNEL_MODE_START(p_cc_pdb->p_glb_pdb,ICF_FALSE)
    if (ICF_MEDIA_CONNECT_DELETED_STREAM_PRESENT == 
            (p_media_connect->bit_mask & ICF_MEDIA_CONNECT_DELETED_STREAM_PRESENT))
    {
        p_cc_pdb->count = p_media_connect->count;
        p_cc_pdb->p_generic_ptr_1 = p_media_connect->deleted_media_streams;

        /* Following function will check if there is any stream 
         * deleted by user.
         */
        ret_val = icf_cc_check_for_streams_deleted_by_user(p_cc_pdb);


        /* ith level check was NOT performed inside the above function,
         * This bit is just being checked if there was any exception occured inside
         * the above function (Like stream id deleted(if there was any) but it was not
         * present inside the list of sdps received from SSA.
         */
        if (ICF_CC_ALL_STREAMS_DELETED == 
                (p_cc_pdb->common_bitmask & ICF_CC_ALL_STREAMS_DELETED))
        {
            ret_val = icf_cc_03_generic_err(p_cc_pdb);
            ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */
                return ret_val;
        }

         /* code for media mode changes starts here */
        /* check if the media mode is present and the value is not
         * send only then do the mms to with the mode given mode 
         * also ICF_MEDIA_MODE_APPLIED_FROM_APP is to check whether the mode
         * already set in Media Connect then don't change the mode here again
         * check for early media is there because we want set the mode as
         * send recv iff SDP is going in the 180/183*/
        if(ICF_MEDIA_CONNECT_MEDIA_MODE_PRESENT == (p_media_connect->bit_mask & ICF_MEDIA_CONNECT_MEDIA_MODE_PRESENT) &&
              (p_media_connect->media_mode != ICF_MEDIA_MODE_SEND_ONLY) &&
                   (ICF_MEDIA_MODE_APPLIED_FROM_APP != (p_cc_pdb->p_call_ctx->common_bitmask & ICF_MEDIA_MODE_APPLIED_FROM_APP)))
        {
            /* change the mm applied mode to send recv here 
             * No Need to check the return value as MMS is already done in the existing code*/
            icf_cc_apply_pa_media_mode(p_cc_pdb->p_call_ctx->p_stream_sdp_info,
                                                        p_media_connect->media_mode);
           /* p_cc_pdb->p_call_ctx->common_bitmask |= ICF_MEDIA_MODE_APPLIED_FROM_APP ;*/

        }       
        /* End of media mode chnage code */
      

        if (icf_cc_send_mms_req(p_cc_pdb) == ICF_FAILURE)
        {
            ret_val=ICF_FAILURE;
        }
        else
        {
            ICF_CC_SET_CALL_STATE(p_cc_pdb, ICF_CC_04_AWT_MMS_RESP)
            ICF_CC_SET_ONGOING_EVENT(p_cc_pdb,ICF_CC_PA_MEDIA_CONNECT_REQ)
            ICF_CC_SET_NEXT_STATE(p_cc_pdb, ICF_CC_05_AWT_CONNECT)
        }

    }

    /* code for media mode changes starts here */
    /* check if the media mode is present and the value is not
     * send only then do the mms to with the mode given mode 
     * also ICF_MEDIA_MODE_APPLIED_FROM_APP is to check whether the mode
     * already set in Media Connect then don't change the mode here again
     * check for early media is there because we want set the mode as
     * send recv iff SDP is going in the 180/183*/

    else if (ICF_MEDIA_CONNECT_MEDIA_MODE_PRESENT == (p_media_connect->bit_mask & ICF_MEDIA_CONNECT_MEDIA_MODE_PRESENT) &&
            (p_media_connect->media_mode != ICF_MEDIA_MODE_SEND_ONLY) &&
            (ICF_MEDIA_MODE_APPLIED_FROM_APP != (p_cc_pdb->p_call_ctx->common_bitmask & ICF_MEDIA_MODE_APPLIED_FROM_APP)))
    {
        /* change the mm applied mode to send recv here */
        if(ICF_FAILURE != icf_cc_apply_pa_media_mode(p_cc_pdb->p_call_ctx->p_stream_sdp_info,
                    p_media_connect->media_mode))
        {

            /*        p_cc_pdb->p_call_ctx->common_bitmask |= ICF_MEDIA_MODE_APPLIED_FROM_APP ;*/
            if (icf_cc_send_mms_req(p_cc_pdb) == ICF_FAILURE)
            {
                ret_val=ICF_FAILURE;
            }
            else
            {
                ICF_CC_SET_CALL_STATE(p_cc_pdb, ICF_CC_04_AWT_MMS_RESP)
                ICF_CC_SET_ONGOING_EVENT(p_cc_pdb,ICF_CC_PA_MEDIA_CONNECT_REQ)
                ICF_CC_SET_NEXT_STATE(p_cc_pdb, ICF_CC_05_AWT_CONNECT)
            }
        }
    }
   /* End of media mode change */
ICF_CHECK_TUNNEL_MODE_END
/*  when 183 will be sent with SDP Then 
 *  set media_committed to true
 */
ICF_CHECK_TUNNEL_MODE_START(p_cc_pdb->p_glb_pdb,ICF_TRUE)
    if(ICF_FALSE == p_media_connect->send_without_sdp)
    {
        if((ICF_NULL != ICF_CC_CHECK_COMMON_BITMASK(p_cc_pdb,
                             ICF_CALL_MEDIA_SETUP_TYPE)))
        {
            p_cc_pdb->p_call_ctx->media_committed = ICF_TRUE;
        }
    }
ICF_CHECK_TUNNEL_MODE_END

#ifdef ICF_QOS_SUPPORT
ICF_CHECK_QOS_SUPPORT_START(p_cc_pdb->p_glb_pdb,ICF_TRUE)

 		if ((ICF_FAILURE != ret_val) && (ICF_FALSE == p_cc_pdb->mms_sent))
		{
            /*In tunnel mode send the 183 from here with QOS lines*/
ICF_CHECK_TUNNEL_MODE_START(p_cc_pdb->p_glb_pdb,ICF_TRUE)
           ret_val = icf_cc_prep_media_param_for_qos(p_cc_pdb);
ICF_CHECK_TUNNEL_MODE_END
           if(ICF_SUCCESS == ret_val)
           {   
	          if(ICF_TRUE == p_cc_pdb->prack_reqd)
	          {
			        /*
			         * Update the states in the call context
			         */
	             ICF_CC_SET_CALL_STATE(p_cc_pdb,ICF_CC_09_AWT_MEDIA_PARAM_PRACK)
                 ICF_CC_SET_COMMON_BITMASK(p_cc_pdb,ICF_PRACK_FOR_MEDIA_CONNECT)

		      } /* if (ICF_TRUE == p_cc_pdb->p... */
		      else
		      {
			     /*
			      * Update the states in the call context
			      */
			     ICF_CC_SET_CALL_STATE(p_cc_pdb,ICF_CC_05_AWT_CONNECT)
		      }
		    }
		 }
		
ICF_CHECK_QOS_SUPPORT_END
ICF_CHECK_QOS_SUPPORT_START(p_cc_pdb->p_glb_pdb,ICF_FALSE)
 
        /* The following code snippet sends a 183 with SDP and changes
           call state to 05 or 09 (depending on PRACK support).
        */ 

		if ((ICF_FAILURE != ret_val) && (ICF_FALSE == p_cc_pdb->mms_sent))
		{
            ICF_CC_TRACE(p_cc_pdb, ICF_TRACE_MEDIA_SETUP_TYPE)
            ICF_CC_TRACE(p_cc_pdb, ICF_TRACE_PRACK_REQD)

            ret_val = icf_cc_send_ssa_media_param(p_cc_pdb);
            if ( ICF_FAILURE == ret_val)
            {
                /*Nothing to do*/
            }
            else if (ICF_TRUE == p_cc_pdb->prack_reqd)
            {
                /*
                 * Update the states in the call context
                 */
                ICF_CC_SET_CALL_STATE(p_cc_pdb,ICF_CC_09_AWT_MEDIA_PARAM_PRACK)
                ICF_CC_SET_COMMON_BITMASK(p_cc_pdb,ICF_PRACK_FOR_MEDIA_CONNECT)

            } /* if (ICF_TRUE == p_cc_pdb->p... */
            else
            {
                /*
                 * Update the states in the call context
                 */
                ICF_CC_SET_CALL_STATE(p_cc_pdb,ICF_CC_05_AWT_CONNECT)
            } /* else */
        } /* (ICF_TRUE == p_cc_pdb->p_ca... */
ICF_CHECK_QOS_SUPPORT_END
#else
		if ((ICF_FAILURE != ret_val) && (ICF_FALSE == p_cc_pdb->mms_sent))
		{
            
            ret_val= icf_cc_send_ssa_media_param(p_cc_pdb);
            ICF_CC_TRACE(p_cc_pdb, ICF_TRACE_PRACK_REQD)

            if (ICF_TRUE == p_cc_pdb->prack_reqd)
            {
                /*
                 * Update the states in the call context
                 */
                ICF_CC_SET_CALL_STATE(p_cc_pdb,ICF_CC_09_AWT_MEDIA_PARAM_PRACK)
                ICF_CC_SET_COMMON_BITMASK(p_cc_pdb,ICF_PRACK_FOR_MEDIA_CONNECT)

            } /* if (ICF_TRUE == p_cc_pdb->p... */
            else
            {
                /*
                 * Update the states in the call context
                 */
                ICF_CC_SET_CALL_STATE(p_cc_pdb,ICF_CC_05_AWT_CONNECT)
            } /* else */
        }/*End of if (ICF_FAILURE != ret_val)*/
#endif
    
    /* In tunnel mode, the following mode related bit is not to be processed. */
ICF_CHECK_TUNNEL_MODE_START(p_cc_pdb->p_glb_pdb,ICF_FALSE)
    /* this bit mask is set so that we will not send the media 
     * mode of the APPlICATION in the further APIs */
    p_cc_pdb->p_call_ctx->common_bitmask |= ICF_MEDIA_MODE_APPLIED_FROM_APP;
ICF_CHECK_TUNNEL_MODE_END

    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */
    return ret_val;
}

/*twc_update handling*/
/******************************************************************************
 *
 * FUNCTION:        icf_cc_03_pa_call_modify_req
 *
 * DESCRIPTION:     This function is invoked when the CC is awaiting 
 *                  ALERT RESP from the PA(incoming call) and
 *                  it receives a call modify request from PA.
 *
 *                  Current State: ICF_CC_03_AWT_ALERT_RESP
 *                  Current Event: ICF_CC_PA_CALL_MODIFY_REQ 
 *
 *****************************************************************************/
icf_return_t icf_cc_03_pa_call_modify_req(
        INOUT icf_cc_pdb_st *p_cc_pdb)
{
    icf_return_t                ret_val = ICF_SUCCESS;
    icf_config_data_st          *p_config_data = ICF_NULL;
    icf_call_modify_req_st      *p_call_modify_req = ICF_NULL;
    icf_stream_sdp_info_st      *p_stream_sdp_info = ICF_NULL;

    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /* function entry trace */

    p_call_modify_req = 
        (icf_call_modify_req_st*)(p_cc_pdb->p_glb_pdb->p_recv_msg->payload);

    /* Get pointer to config data from DBM */
    ret_val = icf_dbm_get_module_glb_data(
                p_cc_pdb->p_glb_pdb,
               (icf_int_module_id_t)ICF_INT_MODULE_CFG,
               (icf_void_t *)&p_config_data, p_cc_pdb->p_ecode);

    ICF_ASSERT(p_cc_pdb->p_glb_pdb, (ICF_NULL != p_config_data));

ICF_CHECK_TUNNEL_MODE_START(p_cc_pdb->p_glb_pdb,ICF_FALSE)
    /* Check if media is committed (183 is received) then send UPDATE to ssa 
     * else send failure response to Application
     */
    /* CSR 1-6188936: SPR 18835:ICF_UPDATE_PRESENT_IN_ALLOW_HDR is bitmask*/
    if ((ICF_TRUE == p_cc_pdb->p_call_ctx->media_committed) &&
        (ICF_UPDATE_PRESENT_IN_ALLOW_HDR & p_cc_pdb->p_call_ctx->allowed_value))
    {			
         /*Set common_bitmask as UPDATE_WO_SDP*/
         ICF_CC_SET_COMMON_BITMASK(p_cc_pdb, ICF_CALL_MOD_REQ_WO_SDP)

        /* SPR 19590 : code removed for cloning header/tag/body list from 
         * here as this shall be taken care in icf_cc_convert_api_to_event
         * function.
         */

         if (ICF_FAILURE == icf_cc_ssa_send_update_msg(p_cc_pdb))
         {
             ret_val = ICF_FAILURE;
         }
    }
    else 
    {
         /* reject the request*/
         p_cc_pdb->p_call_ctx->app_failure_cause = ICF_CAUSE_INVALID_CALL_MODIFY_REQUEST;
         ICF_CC_SET_COMMON_BITMASK(p_cc_pdb, ICF_CALL_MODIFY_RESP_FAIL)

         /* Send failure response for update to user */
         if(ICF_FAILURE == icf_cc_send_pa_call_modify_resp(p_cc_pdb))
         {
             ret_val = ICF_FAILURE;
         }
    }
ICF_CHECK_TUNNEL_MODE_END
ICF_CHECK_TUNNEL_MODE_START(p_cc_pdb->p_glb_pdb,ICF_TRUE)
    /* Rel 8.1 If allow header contains the UPDATE
     * then send update other wise reject request 
     */
    if(ICF_UPDATE_PRESENT_IN_ALLOW_HDR & p_cc_pdb->p_call_ctx->allowed_value)
    {
        /*  Rel 8.1
         *  If CALL_MODIFY_received without SDP
         *  Send UPDATE without SDP and move t0 14 state
         *  It is done to handle the SDP if received in 
         *  200 OK
         */
        if(ICF_FALSE== 
            p_call_modify_req->tunnelled_sdp_present)
        {

            /*Set common_bitmask as UPDATE_WO_SDP*/
            ICF_CC_SET_COMMON_BITMASK(p_cc_pdb, ICF_CALL_MOD_REQ_WO_SDP)
            if(ICF_FAILURE == icf_cc_prepare_call_modify_request(p_cc_pdb)) 
            {
                ret_val = ICF_FAILURE;
            }
            else if (ICF_FAILURE == icf_cc_ssa_send_update_msg(p_cc_pdb))
            {
                ret_val = ICF_FAILURE;
            }
            if(ICF_FAILURE == ret_val)
            {
                if(ICF_NULL != p_cc_pdb->p_call_ctx->p_to_tag)
                {
                    ICF_MEMFREE(p_cc_pdb->p_glb_pdb,
                      p_cc_pdb->p_call_ctx->p_to_tag, ICF_MEM_COMMON,
                      p_cc_pdb->p_ecode, ret_val)
                    ret_val = ICF_FAILURE;
                }
                ICF_CC_SET_COMMON_BITMASK(p_cc_pdb, ICF_CALL_MODIFY_RESP_FAIL)
                /* Send failure response for update to user */
                if(ICF_FAILURE == icf_cc_send_pa_call_modify_resp(p_cc_pdb))
                {
                    ret_val = ICF_FAILURE;
                }
            }
            /*  Fix for SPR 20078*/
            /*  if call modify request is received from application then set
             *  this boolean to track this call modify at the time of timer
             *  expiry or generic error. THis boolean will be reset once
             *  ICF_CALL_MODIFY_RESP will be sent to application.
             */
            else
            {
                p_cc_pdb->p_call_ctx->is_call_modify_ongoing = ICF_TRUE;
            }
        }
        else 
        {
            /*  If its incoming call and either media committed is false
             *  or prack is not supported then send call modify failure response 
             */
            /*SPR 19473 used prack_reqd instead of default settings*/
            if(ICF_NULL != ICF_CC_CHECK_COMMON_BITMASK(p_cc_pdb,ICF_CALL_DIR) &&
              ((ICF_FALSE == p_cc_pdb->p_call_ctx->media_committed) ||
			  (ICF_FALSE == p_cc_pdb->prack_reqd)))
            {
                /* Rel 8.1
                 * reject the request with error cause 
                 * OFFER_ANSWER_NOT_COMPLETED
                 */
                p_cc_pdb->p_call_ctx->app_failure_cause = ICF_CAUSE_OFFER_ANSWER_NOT_COMPLETED;
                ICF_CC_SET_COMMON_BITMASK(p_cc_pdb, ICF_CALL_MODIFY_RESP_FAIL)
      
                /* Send failure response for update to user */
                if(ICF_FAILURE == icf_cc_send_pa_call_modify_resp(p_cc_pdb))
                {
                      ret_val = ICF_FAILURE;
                }
            }
            /*  If its an outgoing call and either media committed is false
             *  or prack si not asked by the peer then send call modify failure response 
             */

            else if((ICF_NULL == ICF_CC_CHECK_COMMON_BITMASK(p_cc_pdb,ICF_CALL_DIR)) &&
                   ((ICF_FALSE == p_cc_pdb->p_call_ctx->media_committed) ||
                    (ICF_REL_PROV_RESP_SUPP_BY_PEER  != 
                    (ICF_REL_PROV_RESP_SUPP_BY_PEER & p_cc_pdb->p_call_ctx->common_bitmask))))
            {
                /* Rel 8.1
                 * reject the request with error cause 
                 * OFFER_ANSWER_NOT_COMPLETED
                 */
                p_cc_pdb->p_call_ctx->app_failure_cause = ICF_CAUSE_OFFER_ANSWER_NOT_COMPLETED;
                ICF_CC_SET_COMMON_BITMASK(p_cc_pdb, ICF_CALL_MODIFY_RESP_FAIL)
      
                /* Send failure response for update to user */
                if(ICF_FAILURE == icf_cc_send_pa_call_modify_resp(p_cc_pdb))
                {
                      ret_val = ICF_FAILURE;
                }
            }
            else
            {
                if(p_call_modify_req->bit_mask & 
                    ICF_CALL_MOD_REQ_TO_TAG_PRESENT)
                {
                    /*Rel 8.1
                     *Get the memory for to tag in call context & 
                     *Copy the receive to tag in call context
                     *so that when trigger is given to for UPDATE
                     *to SSA then it can be used
                     */
                    if(ICF_NULL != p_cc_pdb->p_call_ctx->p_to_tag)
                    {
                        ICF_MEMFREE(p_cc_pdb->p_glb_pdb,
                          p_cc_pdb->p_call_ctx->p_to_tag, ICF_MEM_COMMON,
                          p_cc_pdb->p_ecode, ret_val)
                    }
                    /*  Create the memory of to tag in
                     *  in call context
                     */
                    ICF_MEMGET(p_cc_pdb->p_glb_pdb,
                    (p_call_modify_req->to_tag.str_len+1),
                    ICF_MEM_COMMON,
                    p_cc_pdb->p_call_ctx->p_to_tag,
                    ICF_RET_ON_FAILURE,
                    p_cc_pdb->p_ecode,
                    ret_val)
                    /* copy the to tag in call context */
                    icf_port_strcpy(p_cc_pdb->p_call_ctx->p_to_tag,
                                    p_call_modify_req->to_tag.str);
    
                }

                /* Rel 8.1
                 * If media is also committed then 
                 * first take the backup of local sdp and then
                 * set Modify_media_type as LOACL_SDP_REQUIRED 
                 * because Local SDP Is required to send it in UPDATE
                 * and send mms to MMI
                 * and move to state 4
                 */
                if(ICF_FAILURE == icf_cc_copy_committed_sdp(p_cc_pdb))
                {
                    ret_val = ICF_FAILURE;
                }
                else
                {
                    p_stream_sdp_info = 
                         (icf_stream_sdp_info_st *)
    					 (p_cc_pdb->p_call_ctx->p_stream_sdp_info->p_payload);
                    p_stream_sdp_info->media_modify_type = ICF_NULL;
                    p_stream_sdp_info->media_modify_type |= ICF_LOCAL_SDP_REQD;
                    if(ICF_FAILURE == icf_cc_send_mms_req(p_cc_pdb))
                    {
                        ret_val = ICF_FAILURE;
                    }
                    else
                    {
                        /* Fix For SPR:20101*/
                        /* No timer started for MMS response , currently setup timer will
                         * be running so we cannot start a new timer.*/
                        ICF_CC_SET_ONGOING_EVENT(p_cc_pdb,p_cc_pdb->current_event)
                        ICF_CC_SET_CALL_STATE(p_cc_pdb,ICF_CC_04_AWT_MMS_RESP)
                        ICF_CC_SET_NEXT_STATE(p_cc_pdb, ICF_CC_03_AWT_ALERT_RESP)
                    }
                }
                if(ICF_FAILURE == ret_val)
                {
                    if(ICF_NULL != p_cc_pdb->p_call_ctx->p_to_tag)
                    {
                        ICF_MEMFREE(p_cc_pdb->p_glb_pdb,
                          p_cc_pdb->p_call_ctx->p_to_tag, ICF_MEM_COMMON,
                          p_cc_pdb->p_ecode, ret_val)
                        ret_val = ICF_FAILURE;
                    }
                     ICF_CC_SET_COMMON_BITMASK(p_cc_pdb, ICF_CALL_MODIFY_RESP_FAIL)
                    /* Send failure response for update to user */
                    if(ICF_FAILURE == icf_cc_send_pa_call_modify_resp(p_cc_pdb))
                    {
                        ret_val = ICF_FAILURE;
                    }
                }
            }
        } 
    }
    /* Rel 8.1
     * If UPDATE is not received in Allow
     * Header then send Failure call_modify_resp
     * to application
     */
    else
    {
        /* reject the request*/
        p_cc_pdb->p_call_ctx->app_failure_cause = ICF_CAUSE_INVALID_CALL_MODIFY_REQUEST;
        ICF_CC_SET_COMMON_BITMASK(p_cc_pdb, ICF_CALL_MODIFY_RESP_FAIL)
   
        /* Send failure response for update to user */
        if(ICF_FAILURE == icf_cc_send_pa_call_modify_resp(p_cc_pdb))
        {
            ret_val = ICF_FAILURE;
        }
    }
ICF_CHECK_TUNNEL_MODE_END
	
    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */
    return ret_val;
}

/******************************************************************************
 *
 * FUNCTION:        icf_cc_03_ssa_call_progress
 *
 * DESCRIPTION:     This function is invoked when the CC is awaiting
 *                  Alert from the SSA and it receives call progress indication
 *					- SIP 181/182/183 without sdp received from peer
 *                  Current State: ICF_CC_03_AWT_ALERT_RESP
 *                  Current Event: ICF_CC_SSA_CALL_PROGRESS
 *
 *****************************************************************************/
icf_return_t icf_cc_03_ssa_call_progress(
        INOUT icf_cc_pdb_st *p_cc_pdb)
{
    icf_return_t                    ret_val = ICF_SUCCESS;
    icf_nw_inc_call_progress_st     *p_nw_inc_call_progress = ICF_NULL;

    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /* function entry trace */

    p_nw_inc_call_progress = (icf_nw_inc_call_progress_st *)
                        (p_cc_pdb->p_internal_msg->p_msg_data);

    /* Send call progress indication to the application */
    ret_val = icf_cc_send_pa_progress_ind(p_cc_pdb, 
					p_nw_inc_call_progress->response_code);

    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */
    return ret_val;
}


