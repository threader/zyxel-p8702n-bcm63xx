/*-----------------------------------------------------------------------------
 *
 * File name        : icf_cc_fsm_02.c
 *
 * Purpose          : This file contains the event handlers in 
 *                    ICF_CC_02_AWT_SSA_SETUP_RESP state.
 *
 * Revision History :
 *
 * Date         Name            Ref#            Comments
 * --------     ------------    ----            ---------------
 * 10-Dec-2004  Umang Singh     Non Service LLD Initial 
 * 21-Dec-2004  Rohit Aggarwal                  Addition of traces, stats
 * 23-Dec-2004  Rohit Aggarwal                  Addition of traces, stats
 * 24-Dec-2004  Rohit Aggarwal                  Compilation fixes
 * 10-Aug-2005	Rohit Aggarwal	ICF				Code changes specific to ICF
 * 22-Feb-2005	Amit Sharma                     Added code for auth support
 * 08-Mar-2006  Aman Aggarwal					ICF merging with ICF2.0
 * 26-Jun-2006  Amit Sharma     ICF 6.0 APIs    Structure changes for multiple
 *                                              m-lines
 * 2-Jan-2007   Priyanka Dua    ICF 6.3         Code changes for Forking and 
 *												Multiple answer
 * 9-Jul-2007   Neha Choraria   ICF 7.0         Changes for Forking &
 *                                              Multiple Answer
 
 * 3-Apr-2008   Sumant Gupta    ICF 8.0         Rel7.1 Windows porting Merging
 * 07-Jun-2008  Amit Sharma     IPTK 8.0 SRTP LLD     SRTP Enhancements
 * 21-Jul-2008  Abhishek Dhammawat CSR1-6155032 merge modified 
 *                                              icf_cc_02_ssa_media_param
 * 17-Nov-2008  Rajiv Kumar     SPR 19188       Rel 7.1.2 SPR merging (SPR
 *                                              19023) 
 * 12-Feb-2009  Tarun Gupta     Rel 8.1 LLD     SDP Tunnelling Enhancements
 * 04-Mar-2009  Tarun Gupta     Rel 8.1         Modified icf_cc_02_ssa_media_param
 * 09-Apr-2009  Rajiv Kumar     SPR 19428       Merged CSR 1-6851629
 * Copyright (c) 2009, Aricent.
 *---------------------------------------------------------------------------*/

#include "icf_ecode.h"
#include "icf_cc_prototypes.h"
#include "icf_cc_macro.h"


/******************************************************************************
 *
 * FUNCTION:        icf_cc_02_ssa_setup_resp
 * 
 * DESCRIPTION:     This function is invoked when the CC is awaiting  
 *                  setup resp from the SSA and it receives 
 *                  setup resp from SSA.
 *                  Current State: ICF_CC_02_AWT_SSA_SETUP_RESP
 *                  Current Event: ICF_CC_SSA_SETUP_RESP
 *
 *****************************************************************************/
icf_return_t icf_cc_02_ssa_setup_resp(
        INOUT icf_cc_pdb_st *p_cc_pdb)
{
    icf_return_t     ret_val = ICF_SUCCESS;

    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /* function entry trace */

    /*
     * Update the states in the call context
     */
    ICF_CC_SET_CALL_STATE(p_cc_pdb, ICF_CC_03_AWT_ALERT_RESP)

    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */
    return ret_val;
}


/******************************************************************************
 *
 * FUNCTION:        icf_cc_02_ssa_media_param
 * 
 * DESCRIPTION:     This function is invoked when the CC is awaiting  
 *                  setup resp from the SSA and it receives media param
 *                  (SIP 183 ) from SSA.
 *                  Current State: ICF_CC_02_AWT_SSA_SETUP_RESP
 *                  Current Event: ICF_CC_SSA_MEDIA_PARAM
 *
 *****************************************************************************/
icf_return_t icf_cc_02_ssa_media_param(
        INOUT icf_cc_pdb_st *p_cc_pdb)
{
        icf_return_t     ret_val = ICF_SUCCESS;
	icf_nw_inc_media_param_st	*p_nw_inc_media_param = ICF_NULL;

        ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /* function entry trace */

        /*
         * Update the states in the call context
         */

	p_nw_inc_media_param = (icf_nw_inc_media_param_st *)
							(p_cc_pdb->p_internal_msg->p_msg_data);


        /********* Changes for Forking & Multiple Answer Start (Rel7.0) ***********/

        /* If it is the first 180/183 then its forked_resp_id should be
           assigned 0 */

        if((ICF_NULL == p_nw_inc_media_param->forked_resp_id) &&
                   (ICF_NULL == p_cc_pdb->p_call_ctx->forked_resp_counter))
                p_nw_inc_media_param->forked_resp_id = ICF_NULL;

        /*********** Changes for Forking & Multiple Answer End (Rel7.0) ************/

	if (ICF_FORKED_RESP == (p_cc_pdb->p_call_ctx->common_bitmask & ICF_FORKED_RESP))
	{

ICF_CHECK_MULT_ANS_DISBL_START(p_cc_pdb->p_glb_pdb)
 
           ICF_PRINT(((icf_uint8_t*)"\n[CC]: Multiple Answer OFF in Forking - Clearing Call "));
           icf_cc_delete_inc_media_sdp_info_list(p_cc_pdb,&p_nw_inc_media_param->p_sdp_list);
           p_cc_pdb->p_call_ctx->release_cause = ICF_CAUSE_NETWORK_ERROR;
           ret_val = icf_cc_02_generic_err(p_cc_pdb);
           return ret_val;

ICF_CHECK_MULT_ANS_DISBL_END

		/* 183 with SDP from another UAS(Forking) */
		/* Reset forked answer indication */
		ICF_CC_RESET_COMMON_BITMASK(p_cc_pdb,ICF_FORKED_RESP)
	}
    
	/* This field is set to false both in case of forked/non-forked response */ 
	p_cc_pdb->p_call_ctx->no_remote_sdp = ICF_FALSE;

	/* This p_list_1 is being set because this should be initialised 
	 * with icf_inc_media_sdp_info_st before calling this function.
	 */
	p_cc_pdb->p_list_1 = p_nw_inc_media_param->p_sdp_list;

        /********* Changes for Forking & Multiple Answer Start (Rel7.0) ***********/

        p_cc_pdb->forked_resp_id =  p_nw_inc_media_param->forked_resp_id;

        /*********** Changes for Forking & Multiple Answer End (Rel7.0) ***********/

	ret_val = icf_cc_validate_ith_level_m_line_check(p_cc_pdb);
	if (ICF_FAILURE == ret_val)
	{
		if (ICF_CC_STREAM_POS_VALIDATION_FAIL == 
			(p_cc_pdb->common_bitmask & ICF_CC_STREAM_POS_VALIDATION_FAIL))
		{
            /*Fix merge for CSR_1_5447125*/
		    /* Now that provisional response has been received, call 
			 * clear request can be sent to SSA also to send CANCEL.
			 */
			ret_val = icf_cc_03_generic_err(p_cc_pdb);	            
		}
	}
	else
	{
        /* In tunnel mode, dont issue this trace to the application */
#ifdef ICF_TRACE_ENABLE        
ICF_CHECK_TUNNEL_MODE_START(p_cc_pdb->p_glb_pdb,ICF_FALSE)        
                if(ICF_NULL != p_nw_inc_media_param->p_sdp_list)
                   ICF_CC_APP_TRACE(p_cc_pdb,ICF_TRACE_REMOTE_SDP_ANSWER)
ICF_CHECK_TUNNEL_MODE_END                       
#endif

                /**** Changes for Forking & Multiple Answer Start (Rel7.0) ****/

                p_cc_pdb->forked_resp_id =  ICF_NULL;
                ICF_PRINT(((icf_uint8_t*)"\n[CC]: FORKED RESP ID: %d\n",p_cc_pdb->forked_resp_id));

                /***** Changes for Forking & Multiple Answer End (Rel7.0) *****/
        /* In tunnel mode, dont issue this trace to the application */
ICF_CHECK_TUNNEL_MODE_START(p_cc_pdb->p_glb_pdb,ICF_FALSE)
#ifdef ICF_TRACE_ENABLE    
         if(ICF_NULL != p_nw_inc_media_param->p_sdp_list)
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
           then also this 183 with SDP is an answer only.
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
            /*Fix merge for CSR_1_5447125*/
		    /* Now that provisional response has been received, call 
			 * clear request can be sent to SSA also to send CANCEL.
			 */
			ret_val = icf_cc_03_generic_err(p_cc_pdb);            
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
                           ICF_FALSE == (p_cc_pdb->p_call_ctx->common_bitmask & ICF_NO_LOCAL_CODEC_IN_GCL))
			{
				/* TBD - post 6.3 */
			   p_cc_pdb->p_call_ctx->common_bitmask |= ICF_OFFER_CODECS_IN_GCL;
			}

                        if(p_cc_pdb->p_call_ctx->common_bitmask & ICF_NO_LOCAL_CODEC_IN_GCL)
                        {
                           p_cc_pdb->p_call_ctx->common_bitmask &= ~ICF_NO_LOCAL_CODEC_IN_GCL;
                        }
ICF_CHECK_TUNNEL_MODE_END

			if(ICF_FAILURE == icf_cc_send_mms_req(p_cc_pdb))
			{
				ret_val = ICF_FAILURE;
			}
            /*CSR 1-6155032 Set the state as _04_ only when CC has sent MMS*/
            else if(ICF_TRUE == p_cc_pdb->mms_sent)
            {
                ICF_CC_SET_CALL_STATE(p_cc_pdb,ICF_CC_04_AWT_MMS_RESP)
                ICF_CC_SET_NEXT_STATE(p_cc_pdb,ICF_CC_03_AWT_ALERT_RESP)
            }
		}
	}
    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */
    return ret_val;
}


/******************************************************************************
 *
 * FUNCTION:        icf_cc_02_ssa_alert
 * 
 * DESCRIPTION:     This function is invoked when the CC is awaiting  
 *                  setup resp from the SSA and it receives alert without 
 *                  media (SIP 180 without SDP) from SSA.
 *                  Current State: ICF_CC_02_AWT_SSA_SETUP_RESP
 *                  Current Event: ICF_CC_SSA_ALERT
 *
 *****************************************************************************/
icf_return_t icf_cc_02_ssa_alert(
        INOUT icf_cc_pdb_st *p_cc_pdb)
{
    icf_return_t     ret_val = ICF_SUCCESS;

    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /* function entry trace */

    /*
     * Update the states in the call context
     */
        ICF_CC_SET_CALL_STATE(p_cc_pdb,ICF_CC_05_AWT_CONNECT)
    /*
     * Stop the call setup timer and start the alert timer
     */
    if(ICF_FAILURE == icf_cc_start_alert_timer(p_cc_pdb, ICF_FALSE, ICF_NULL))
    {
        ret_val = ICF_FAILURE;
    }
    /*
     * Send ALERT indication to PA
     */
    else if(ICF_FAILURE == icf_cc_send_pa_alert_ind(p_cc_pdb))
    {
        ret_val = ICF_FAILURE;
    }

    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */
    return ret_val;
}


/******************************************************************************
 *
 * FUNCTION:        icf_cc_02_ssa_alrt_with_media
 * 
 * DESCRIPTION:     This function is invoked when the CC is awaiting  
 *                  setup resp from the SSA and it receives alert with 
 *                  media (SIP 180 with SDP) from SSA.
 *                  Current State: ICF_CC_02_AWT_SSA_SETUP_RESP
 *                  Current Event: ICF_CC_SSA_ALERT_WITH_MEDIA
 *
 *****************************************************************************/
icf_return_t icf_cc_02_ssa_alrt_with_media(
        INOUT icf_cc_pdb_st *p_cc_pdb)
{
    icf_return_t     ret_val = ICF_SUCCESS;

	icf_nw_inc_alert_media_st	*p_nw_inc_alert_media = ICF_NULL;

    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /* function entry trace */


	p_nw_inc_alert_media = (icf_nw_inc_alert_media_st *)
							(p_cc_pdb->p_internal_msg->p_msg_data);

    ICF_CC_SET_ONGOING_EVENT(p_cc_pdb, p_cc_pdb->current_event)
   
    /* As alert has been received change call state, otherwise in failure
       case we assume we are still waiting for remote response */ 
    ICF_CC_SET_CALL_STATE(p_cc_pdb,ICF_CC_05_AWT_CONNECT)

    /********* Changes for Forking & Multiple Answer Start (Rel7.0) ***********/

    /* If it is the first 180/183 then its forked_resp_id should be
        assigned 0 */

    if((ICF_NULL == p_nw_inc_alert_media->forked_resp_id) &&
              (ICF_NULL == p_cc_pdb->p_call_ctx->forked_resp_counter))
    p_nw_inc_alert_media->forked_resp_id = ICF_NULL;

    /*********** Changes for Forking & Multiple Answer End (Rel7.0) ************/

    
    /*
     * Stop the call setup timer and start the alert timer
     */
    if(ICF_FAILURE == icf_cc_start_alert_timer(p_cc_pdb, ICF_FALSE, ICF_NULL))
    {
        icf_cc_delete_inc_media_sdp_info_list(p_cc_pdb,&p_nw_inc_alert_media->p_sdp_list); 
        ret_val = ICF_FAILURE;
    }
    if(ICF_SUCCESS == ret_val)
    {
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
            /*Fix merge for CSR_1_5447125*/
			/* Now that provisional response has been received, call 
			 * clear request can be sent to SSA also to send CANCEL.
			 */
			ret_val = icf_cc_03_generic_err(p_cc_pdb);            
		}
	}
	else
	{
                /**** Changes for Forking & Multiple Answer Start (Rel7.0) ****/

                p_cc_pdb->forked_resp_id =  ICF_NULL;

                /***** Changes for Forking & Multiple Answer End (Rel7.0) *****/
ICF_CHECK_TUNNEL_MODE_START(p_cc_pdb->p_glb_pdb,ICF_FALSE) 	
		/* This Function is deriving appropriate mms type.*/
        ret_val = icf_cc_make_mms_type_for_rem_succ_ans (p_cc_pdb);
ICF_CHECK_TUNNEL_MODE_END
ICF_CHECK_TUNNEL_MODE_START(p_cc_pdb->p_glb_pdb,ICF_TRUE)     

        /* In tunnel mode, we will set the media_modify_type field 
           which will determine whether the tunnelled MMS request
           and response will contain the tunnelled SDP or not.
        */
        ret_val = icf_cc_make_mms_type_for_answer_in_tunnel_mode(p_cc_pdb);
ICF_CHECK_TUNNEL_MODE_END
        
		if (ICF_CC_ALL_STREAMS_DELETED == 
				(p_cc_pdb->common_bitmask & ICF_CC_ALL_STREAMS_DELETED))
		{
            /*Fix merge for CSR_1_5447125*/
			/* Now that provisional response has been received, call 
			 * clear request can be sent to SSA also to send CANCEL.
			 */
			ret_val = icf_cc_03_generic_err(p_cc_pdb);            
		}
		else if (ICF_FAILURE != ret_val)
		{
			/*
			 * Send Modify Media Session to the MMI
			 */
			if (ICF_FAILURE == icf_cc_send_mms_req(p_cc_pdb))
			{
				ret_val = ICF_FAILURE;
			}
            /*CSR 1-6155032 Set the state as _04_ only when CC has sent MMS*/
			else if(ICF_TRUE == p_cc_pdb->mms_sent)
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
    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */
    return ret_val;
}


/******************************************************************************
 *
 * FUNCTION:        icf_cc_02_ssa_call_connect
 * 
 * DESCRIPTION:     This function is invoked when the CC is awaiting  
 *                  setup resp from the SSA and it receives connect 
 *                  (SIP 200 OK ) from SSA.
 *                  Current State: ICF_CC_02_AWT_SSA_SETUP_RESP
 *                  Current Event: ICF_CC_SSA_CALL_CONNECT
 *
 *****************************************************************************/
icf_return_t icf_cc_02_ssa_call_connect(
        INOUT icf_cc_pdb_st *p_cc_pdb)
{
    icf_return_t     ret_val = ICF_SUCCESS;
    icf_nw_inc_connect_st *p_nw_inc_connect = ICF_NULL;

    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /* function entry trace */

	p_nw_inc_connect = (icf_nw_inc_connect_st *)
						(p_cc_pdb->p_internal_msg->p_msg_data);

        /********* Changes for Forking & Multiple Answer Start (Rel7.0) ***********/

        /* If it is the first resp then its forked_resp_id should be
           assigned 0 */

        if((ICF_NULL == p_nw_inc_connect->forked_resp_id) &&
               (ICF_NULL == p_cc_pdb->p_call_ctx->forked_resp_counter))
        {
                p_nw_inc_connect->forked_resp_id = ICF_NULL;
        }

        /*********** Changes for Forking & Multiple Answer End (Rel7.0) ************/


	if (ICF_NW_INC_CONNECT_WITH_SDP == 
		(p_nw_inc_connect->bitmask & ICF_NW_INC_CONNECT_WITH_SDP))
	{
		p_cc_pdb->sdp_present = ICF_TRUE;
	}
	else
	{
                p_cc_pdb->sdp_present = ICF_FALSE;
	}
       
    /*
     * Send Call Connect ACK to peer
     */
    if (ICF_FAILURE == icf_cc_send_ssa_connect_ack(p_cc_pdb))
    {
        /*Fix for SPR : 19188 (Rel 7.1.2 SPR merging for SPR 19023)*/
        /*  Fix for spr 19023 */
        if (ICF_NW_INC_CONNECT_WITH_SDP ==
        (p_nw_inc_connect->bitmask & ICF_NW_INC_CONNECT_WITH_SDP))
        {
            icf_cc_delete_inc_media_sdp_info_list(p_cc_pdb,
                     &p_nw_inc_connect->p_sdp_list);
        }        
        ret_val = ICF_FAILURE;
    }
    else
    {
        /* As connect has been received change call state, otherwise in failure
        case we assume we are still waiting for remote response */ 
        ICF_CC_SET_CALL_STATE(p_cc_pdb,ICF_CC_06_CONNECTED)
       
        /*Fix for CSR_1-5447125*/    
        /*As ACK is send on network and we move to connected state
         * set the ICF_CALL_CONNECT_DONE*/
        ICF_CC_SET_COMMON_BITMASK(p_cc_pdb,
                            ICF_CALL_CONNECT_DONE)

        /*
         * If SDP is not recvd in CALL CONNECT fail the call.
         * (1)Send call termination request to peer
         * (2)Send DMS request to MMI
         */
        /* Fill the value of p_cc_pdb->sdp_present */
        /* Check whether the 200 OK contains SDP or not */
        ICF_CC_TRACE(p_cc_pdb, ICF_TRACE_SDP_IN_PEER_MSG)
        if((ICF_FALSE == p_cc_pdb->sdp_present) || 
           /*(ICF_FALSE == p_cc_pdb->p_call_ctx->media_committed) ||*/
           (ICF_CC_CALL_CLEAR == (p_cc_pdb->common_bitmask & ICF_CC_CALL_CLEAR)))
        {
            ret_val = icf_cc_clear_call_no_remote_sdp(p_cc_pdb);
        } /* if(ICF_FALSE == p_cc_pdb->s... */

        		else
		{
			/*
			 * If SDP is present,
		
			* This p_list_1 is being set because this should be initialised 
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
                    /*Fix merge for CSR_1_5447125*/
					/* Now that final response has been received, call 
			 		 * clear request can be sent to SSA also to send BYE.
			 		 */
					ret_val = icf_cc_05_generic_err(p_cc_pdb);                    
				}
			}
			else
			{
ICF_CHECK_TUNNEL_MODE_START(p_cc_pdb->p_glb_pdb,ICF_FALSE)                 
				/* This Function is deriving appropriate mms type.*/
				ret_val = icf_cc_make_mms_type_for_rem_succ_ans (p_cc_pdb);
ICF_CHECK_TUNNEL_MODE_END

ICF_CHECK_TUNNEL_MODE_START(p_cc_pdb->p_glb_pdb,ICF_TRUE)     
    
                /* In tunnel mode, we will set the media_modify_type field 
                   which will determine whether the tunnelled MMS request
                   and response will contain the tunnelled SDP or not.
                */
                ret_val = icf_cc_make_mms_type_for_answer_in_tunnel_mode(p_cc_pdb);
ICF_CHECK_TUNNEL_MODE_END
    
				if (ICF_CC_ALL_STREAMS_DELETED == 
					 (p_cc_pdb->common_bitmask & ICF_CC_ALL_STREAMS_DELETED))
				{
                    /*Fix merge for CSR_1_5447125*/
					/* Now that final response has been received, call 
			 		 * clear request can be sent to SSA also to send BYE.
			 		 */
					ret_val = icf_cc_05_generic_err(p_cc_pdb);                    
				}
				else if (ICF_FAILURE != ret_val)
				{
					/* Send Modify Media Session to the MMI.*/
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
						ICF_CC_SET_NEXT_STATE(p_cc_pdb,ICF_CC_06_CONNECTED)
					}
				}
			}
		}
    }

	/* Fix for CSR 1-6851629: SPR 19297 */
    /* Set the call connect bitmask after 200 OK is processed */
    ICF_CC_SET_COMMON_BITMASK(p_cc_pdb, ICF_CALL_CONNECT_DONE)
		
    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */
    return ret_val;
}


/******************************************************************************
 *
 * FUNCTION:        icf_cc_02_ssa_remote_reject
 * 
 * DESCRIPTION:     This function is invoked when the CC is awaiting 
 *                  setup resp from the SSA and it receives Remote Call 
 *                  Reject from SSA (SIP 3xx/4xx/5xx received from peer).
 *                  Current State: ICF_CC_02_AWT_SSA_SETUP_RESP
 *                  Current Event: ICF_CC_SSA_REMOTE_REJECT
 *
 *****************************************************************************/
icf_return_t icf_cc_02_ssa_remote_reject(
        INOUT icf_cc_pdb_st *p_cc_pdb)
{
    icf_return_t     ret_val = ICF_SUCCESS;
    icf_return_t     dummy_retval = ICF_SUCCESS;

    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /* function entry trace */

    if(ICF_NULL != ICF_CC_CHECK_COMMON_BITMASK(p_cc_pdb,
                ICF_CALL_AUTHENTICATION_REQ))
    {
            /* Call should be contiuned. set the bitmask for same */
            ICF_CC_SET_COMMON_BITMASK(p_cc_pdb,ICF_CONTINUE_CALL)
            if(ICF_NULL != p_cc_pdb->p_call_ctx->p_response_str)
            {
               ICF_MEMFREE(p_cc_pdb->p_glb_pdb,
                  p_cc_pdb->p_call_ctx->p_response_str, ICF_MEM_COMMON,
                  p_cc_pdb->p_ecode, dummy_retval)

             }
    }

    if (ICF_FAILURE == icf_cc_send_ssa_reject_ack(p_cc_pdb))
    {
        ret_val = ICF_FAILURE;
    }
    /* if PROXY AUTH or USER AUTH is required send a new invite
     * with auth param else clear the call */
    else if(ICF_NULL == ICF_CC_CHECK_COMMON_BITMASK(p_cc_pdb,
                ICF_CALL_AUTHENTICATION_REQ))
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
        /*
         * Send call setup to SSA  which will send the INVITE on the
         * network. 
         */
        if (ICF_FAILURE == icf_cc_send_ssa_setup_req(p_cc_pdb))
        {
            ret_val = ICF_FAILURE;
        } /* if (ICF_FAILURE == icf_c... */
        else
        {
             ICF_CC_RESET_RELEASE_CAUSE(p_cc_pdb)
            /*
             * Update the states in the call context
             */
            ICF_CC_SET_CALL_STATE(p_cc_pdb, ICF_CC_02_AWT_SSA_SETUP_RESP)

        } /* else */
    } /* else */


    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */
    return ret_val;
}


/******************************************************************************
 *
 * FUNCTION:        icf_cc_02_ssa_redirect
 * 
 * DESCRIPTION:     This function is invoked when the CC is awaiting 
 *                  setup resp from the SSA and it receives reroute call
 *                  Req from SSA (SIP 302 received from peer).
 *                  Current State: ICF_CC_02_AWT_SSA_SETUP_RESP
 *                  Current Event: ICF_CC_SSA_REDIRECT
 *
 *****************************************************************************/
icf_return_t icf_cc_02_ssa_redirect(
        INOUT icf_cc_pdb_st *p_cc_pdb)
{
    icf_return_t     ret_val = ICF_SUCCESS;

    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /* function entry trace */

    /*
     *  Send ACK for redirect request
    */
    if (ICF_FAILURE == icf_cc_send_ssa_redirect_ack(p_cc_pdb))
    {
        ret_val = ICF_FAILURE;
    } /* if (ICF_FAILURE == icf_c... */

    else if(ICF_FAILURE == icf_cc_validate_redirect_req(p_cc_pdb))
    {
        /* Validation failed clear the call */
        
        /* Send DMS Request to MMI */
        p_cc_pdb->forceful_delete = ICF_FALSE;

        p_cc_pdb->p_call_ctx->release_cause = ICF_CALL_CLEAR_REASON_NW_ERROR;

        ICF_CC_SET_RELEASE_TYPE(p_cc_pdb, ICF_REL_TYPE_INTERNAL_INITIATED)
        ICF_CC_TRACE(p_cc_pdb, ICF_TRACE_REL_TYPE_INT_ERR)

        /*
         * Stop the call setup timer and start the release timer
         */
        if (ICF_FAILURE == icf_cc_start_release_timer(p_cc_pdb))
        {
            ret_val = ICF_FAILURE;
        } /* else if (ICF_FAILURE == ipp...  */
        /*
         * mark network clear
         */
        else if (ICF_FAILURE == icf_cc_nw_clear_handler(p_cc_pdb))
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
        else 
        {
            /*
             * Update the states in the call context
             */
            ICF_CC_SET_CALL_STATE(p_cc_pdb,ICF_CC_07_AWT_DMS_RESP)
        }
    }/* end of if(ICF_FAILURE == icf_cc_validate_redirect_req... */
    else
    {
        /*
         * Send call setup to SSA  which will send the INVITE on the
         * network. 
         */
        if (ICF_FAILURE == icf_cc_send_ssa_setup_req(p_cc_pdb))
        {
            ret_val = ICF_FAILURE;
        } /* if (ICF_FAILURE == icf_c... */
        else
        {
            /*
             * Update the states in the call context
             */
            ICF_CC_SET_CALL_STATE(p_cc_pdb,
                    ICF_CC_02_AWT_SSA_SETUP_RESP)

        } /* else */
    } /* else */
    
    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */
    return ret_val;
}


/******************************************************************************
 *
 * FUNCTION:        icf_cc_02_pa_call_term_req
 * 
 * DESCRIPTION:     This function is invoked when the CC is awaiting 
 *                  setup resp from the SSA and it receives Call 
 *                  Terminate Request from PA.
 *                  Current State: ICF_CC_02_AWT_SSA_SETUP_RESP
 *                  Current Event: ICF_CC_PA_CALL_TERMINATE_REQ
 *
 *****************************************************************************/
icf_return_t icf_cc_02_pa_call_term_req(
        INOUT icf_cc_pdb_st *p_cc_pdb)
{
    icf_return_t     ret_val = ICF_SUCCESS;

    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /* function entry trace */

    /*
     * Stop the call setup timer and start the release timer
     */
    if (ICF_FAILURE == icf_cc_start_release_timer(p_cc_pdb))
    {
        ret_val = ICF_FAILURE;
    } /* if (ICF_FAILURE = icf_st... */
    else
    {
        /*
         * Set release type in call context as PA_INITIAITED
         */
        ICF_CC_TRACE(p_cc_pdb, ICF_TRACE_REL_TYPE_PA_INIT)
        ICF_CC_SET_RELEASE_TYPE(p_cc_pdb,ICF_REL_TYPE_PA_INITIATED) 
        /*
         * Send a DMS request to MMI
         */
        if (ICF_FAILURE == icf_cc_send_dms_req(p_cc_pdb))
        {
            ret_val = ICF_FAILURE;
        } /* if (ICF_FAILURE == icf_c... */
        else
        {
            /*
             * Update the states in the call context
             */
            ICF_CC_SET_CALL_STATE(p_cc_pdb,ICF_CC_07_AWT_DMS_RESP)
        } /* else */
    } /* else */

    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */
    return ret_val;
}


/******************************************************************************
 *
 * FUNCTION:        icf_cc_02_mm_error_ind
 *
 * DESCRIPTION:     This function is invoked when the CC is awaiting 
 *                  setup resp from the SSA and MM error indication 
 *                  is received from MMI
 *                  Current State: ICF_CC_02_AWT_SSA_SETUP_RESP
 *                  Current Event: ICF_CC_MM_ERR_IND
 *
 *****************************************************************************/
icf_return_t icf_cc_02_mm_error_ind(
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
    if (ICF_FAILURE ==icf_cc_start_release_timer(p_cc_pdb))
    {
        ret_val = ICF_FAILURE;
    }
    else
    {
        /*
         * Set release type in call context as MM INITIATED
         */
        ICF_CC_TRACE(p_cc_pdb, ICF_TRACE_REL_TYPE_MM_INIT)
        ICF_CC_SET_RELEASE_TYPE(p_cc_pdb,ICF_REL_TYPE_MM_INITIATED) 
         /*
          * Set release cause in call context as ICF_REL_CAUSE_SERV_INT_ERR
          * so that SIP 500 is sent to peer 
          */
        ICF_CC_SET_RELEASE_CAUSE(p_cc_pdb,ICF_CALL_CLEAR_MEDIA_MISMATCH)
		if(ICF_ECODE_DMS_IND_RECVD != media_error)
        {
			
		/*
		* Send a DMS request to MMI
			*/
            if (ICF_FAILURE == icf_cc_send_dms_req(p_cc_pdb))
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
        }
        else
        {
			/* Set MM cleared bit in call_clear_status */
            ICF_CC_SET_CALL_CLR_STATUS(p_cc_pdb,
                ICF_CALL_CLEAR_STATUS_MM_CLEARED)
				
				/*At this point we could not send cancel on network till we get   
				*setup response. Send forceful clear to ssa*/
				ret_val = icf_cc_send_ssa_force_clear(p_cc_pdb);
				/*
				* State state as idle and send status as SRVC terminate so as CM 
				* would clear any service ongoing and send back generic error
			*/
            ICF_CC_SET_CALL_STATE(p_cc_pdb,ICF_CC_00_IDLE)
                
				ICF_CC_SET_STATUS(p_cc_pdb,ICF_STATUS_CALL_R_SRV_TERMINATE)
				ICF_CC_SET_CALL_CLR_STATUS(p_cc_pdb,
				ICF_CALL_CLEAR_STATUS_NW_CLEARED)
        } /* else */
    } /* else */

    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */
    return ret_val;
}


/******************************************************************************
 *
 * FUNCTION:        icf_cc_02_timer_expiry
 *
 * DESCRIPTION:     This function is invoked when the CC is awaiting 
 *                  setup resp from the SSA and Setup Timer expires.
 *                  Current State: ICF_CC_02_AWT_SSA_SETUP_RESP
 *                  Current Event: ICF_CC_TIMER_EXPIRY
 *
 *****************************************************************************/
icf_return_t icf_cc_02_timer_expiry(
        INOUT icf_cc_pdb_st *p_cc_pdb)
{
    icf_return_t     ret_val = ICF_SUCCESS;

    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /* function entry trace */

        /*
         * Set release type in call context as timer expiry
         */
    ICF_CC_TRACE(p_cc_pdb, ICF_TRACE_REL_TYPE_TIMER_INIT)
    ICF_CC_SET_RELEASE_TYPE(p_cc_pdb,ICF_REL_TYPE_TIMER_EXPIRY)
    /*
     * Start the release timer
     */
    if (ICF_FAILURE == icf_cc_start_release_timer(p_cc_pdb))
    {
        ret_val = ICF_FAILURE;
    } /* if (ICF_FAILURE == icf_c... */
    else if (ICF_FAILURE == icf_cc_send_dms_req(p_cc_pdb))
        /* Send a DMS request to MMI*/
    {
        ret_val = ICF_FAILURE;
    }
    else
    {
        /*
         * Update the states in the call context
         */
        ICF_CC_SET_CALL_STATE(p_cc_pdb,ICF_CC_07_AWT_DMS_RESP)
    } /* else */
    
    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */
    return ret_val;
}



/******************************************************************************
 *
 * FUNCTION:        icf_cc_02_generic_err
 *
 * DESCRIPTION:     This function is invoked when the CC is awaiting
 *                  create media session resp from MMI and it receives
 *                  an error indication from MMI..
 *                  Current State: ICF_CC_02_AWT_SSA_SETUP_RESP
 *                  Current Event: ICF_CC_GENERIC_ERROR
 *
 *****************************************************************************/

icf_return_t icf_cc_02_generic_err (
        INOUT icf_cc_pdb_st *p_cc_pdb)
{
    icf_return_t     ret_val = ICF_SUCCESS;

    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /* function entry trace */
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
    else 
    {
        /*
         * Update the states in the call context
         */
        ICF_CC_SET_CALL_STATE(p_cc_pdb,ICF_CC_07_AWT_DMS_RESP)
    }

    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */
    return ret_val;
}


/******************************************************************************
 *
 * FUNCTION:        icf_cc_02_ssa_call_progress
 *
 * DESCRIPTION:     This function is invoked when the CC is awaiting
 *                  setup resp from SSA and it receives call progress indication
 *			- SIP 181/182/183 without sdp received from peer
 *                  Current State: ICF_CC_02_AWT_SSA_SETUP_RESP
 *                  Current Event: ICF_CC_SSA_CALL_PROGRESS
 *
 *****************************************************************************/
icf_return_t icf_cc_02_ssa_call_progress(
        INOUT icf_cc_pdb_st *p_cc_pdb)
{
    icf_return_t     				ret_val = ICF_SUCCESS;
	icf_nw_inc_call_progress_st     *p_nw_inc_call_progress = ICF_NULL;

    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /* function entry trace */

	p_nw_inc_call_progress = (icf_nw_inc_call_progress_st *)
                        (p_cc_pdb->p_internal_msg->p_msg_data);

	/* Send call progress indication to the application */
	if (ICF_FAILURE == icf_cc_send_pa_progress_ind(p_cc_pdb, 
		p_nw_inc_call_progress->response_code))
	{
		ret_val = ICF_FAILURE;
	}
	else
	{
    	/*
     	 * Update the states in the call context
     	 */
    	ICF_CC_SET_CALL_STATE(p_cc_pdb, ICF_CC_03_AWT_ALERT_RESP)
	}

    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */
    return ret_val;
}


/******************************************************************************
 *
 * FUNCTION:        icf_cc_xx_ssa_srtp_fallback
 *
 * DESCRIPTION:     This function is invoked when the CC is in following states
 *                  and receives SRTP FALLBACK request from SSA
 *                  Current State: ICF_CC_02_AWT_SSA_SETUP_RESP
 *                                 ICF_CC_03_AWT_ALERT_RESP
 *                                 ICF_CC_04_AWT_MMS_RESP
 *                                 ICF_CC_05_AWT_CONNECT
 *                  Current Event: ICF_CC_SSA_SRTP_FALLBACK
 *
 *****************************************************************************/
icf_return_t icf_cc_xx_ssa_srtp_fallback(
        INOUT icf_cc_pdb_st *p_cc_pdb)
{
     icf_return_t      ret_val = ICF_SUCCESS;

    p_cc_pdb->forceful_delete = ICF_FALSE;
    /* Send ACK to remote because the  call has been 
       rejected and SRTP Fallback has been invoked */
    if (ICF_FAILURE == icf_cc_send_ssa_reject_ack(p_cc_pdb))
    {
        ret_val = ICF_FAILURE;
    }
    /*Send a DMS request to MMI */
    else if (ICF_FAILURE == icf_cc_send_dms_req(p_cc_pdb))
    {
        ret_val = ICF_FAILURE;
    } /* if (ICF_FAILURE == icf_c... */
    else
    {
        /* Invoke SRTP Fallback invocation*/
        /*Reset srtp_fallback_reqd flag. This flag has been 
          reset to avoid looping conduition in IPTK. The 
          loop can occur when SRTP Fallback has been invoked 
          and MM again sends RTP/SAVP as media profile due to 
          misbehaviour .In such a case RTP/SAVP will again 
          go as media profile in the new INVITE. Now if
          Remote again sends a failure final response i.e 
          4xx/5xx/6xx ,then SRTP Fallback shall not be 
          invoked  */
        p_cc_pdb->p_call_ctx->srtp_fallback_reqd = ICF_FALSE;

        /*Set media_transport_type as RTP_AVP*/
        p_cc_pdb->p_call_ctx->media_transport_type = 
            ICF_TRANSPORT_MODE_RTP_AVP;

        /*Update next state as ICF_CC_01_AWT_CMS_RESP*/
        ICF_CC_SET_NEXT_STATE(p_cc_pdb,ICF_CC_01_AWT_CMS_RESP)
        
        /* Update ongoing event as ICF_CC_SSA_REDIRECT */
        ICF_CC_SET_ONGOING_EVENT(p_cc_pdb,ICF_CC_SRTP_FALLBACK_REQ)

        /* Update the states in the call context */
        ICF_CC_SET_CALL_STATE(p_cc_pdb,ICF_CC_07_AWT_DMS_RESP)
    }
    return ret_val;
}
