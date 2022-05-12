/*------------------------------------------------------------------------------
 *
 * File name        : icf_cc_fsm_04.c
 *
 * Purpose          : This file contains the event handlers in
 *                    ICF_CC_04_AWT_MMS_RESP   state.
 *
 * Revision History :
 *
 * Date         Name            Ref#            Comments
 * --------     ------------    ----            ---------------
 * 10-Dec-2004  Pankaj Negi     Non Service LLD Initial
 * 14-Jan-2005  Rohit Aggarwal                  Removed insure compilation
 *                                              warnings
 * 28-Feb-2005  Sumit Gupta                     Corrected MM error indication 
 *                                              handling
 * 04-Mar-2005  Sumit Gupta                     Updation for media prack with 
 *                                              SDP
 * 20-Apr-2005  Sumit Gupta      SPR 7393       Removed setting of 
 *                                              CALL_CONNECT_DONE on getting MMS
 *                                              response for event CONNECT ACK
 * 26-Apr-2005  Sumit Gupta      SPR 7454       Send alert to ssain case there
 *                                              were no pending events and mms 
 *                                              for media prack received
 * 06-May-2005  Sumit Gupta      SPR 7499       If alert prack or media param 
 *                                              prack or offer change was 
 *                                              ongoing event and 
 *                                              cancel received from peer ack 
 *                                              would be sent for msg
 * 06-May-2005 Sumit Gupta       SPR 7411       In case of initiating call clear
 *                                              from local end and 
 *                                              signaling ongoing set by 
 *                                              other module send status as 
 *                                              call_n_service_terminate to CM 
 *                                              and set state as WAIT_PROC_SIG
 * 10-May-2005 Sumit Gupta       SPR 7529       Previously in case of Call clear
 *                                              from NW release type was set as 
 *                                              INTERNAL_INITIATED it would be 
 *                                              set to NW initiated and nw 
 *                                              cleared would be marked in call 
 *                                              clear status
 *
 * 18-May-2005 Pankaj Negi       SPR 7573       CC is sending multiple error 
 *                                              resp to SSA in case UPDATE MMS 
 *                                              failed and dms is being sent
 * 25-May-2005 Sumit Gupta       SPR 7440       Updated Stats
 * 25-May-2005 Sumit Gupta       SPR 7610       In case of failure in applying 
 *                                              SDP in offer change and media 
 *                                              being normal revert back 
 *                                              rem_media_mode
 * 26-May-2005 Sumit Gupta       SPR 7599       Remote hold/resume indication 
 *                                              in case mms response for the 
 *                                              update event would only be sent 
 *                                              if call is connected.
 *                                              Additionally call hold/resume 
 *                                              indication would be given to PA 
 *                                              if mms response was received for
 *                                              event SSA_CONNECT_ACK and there
 *                                              are no pending events. 
 * 01-Jun-2005   Sumit Gupta     SPR 7635       180 message would be sent with 
 *                                              require header as 100 Rel if 
 *                                              previous 183 message had 100 
 *                                              rel in require header no matter
 *                                              what the current configuration 
 *                                              states                         
 * 09-Jun-2005   Jalaj Negi      IMS CLIENT     Added Event Handlers for IMS
 *                                              CLIENT
 * 15-Jun-2005   Sumit Gupta     SPR 7660       Stop any executing timer when 
 *                                              connect is received from SSA 
 *                                              and pending events were not 
 *                                              183/180 with sdp
 * 10-Aug-2005	Rohit Aggarwal	ICF				Code changes specific to ICF
 * 22-Feb-2005	Amit Sharma                     Added code for auth support
 * 08-Mar-2006  Aman Aggarwal					ICF merging with ICF2.0
 * 30-Mar-2006	Anuradha Gupta	ICF 5.0			Call Modify changes
 * 26-Jun-2006  Amit Sharma     ICF 6.0 APIs    Structure changes for multiple
 *                                              m-lines
 * 25-Jul-2006  Amit Sharma     ICF 6.0         Code changes for Multiple m-lines
 * 2-Jan-2007   Priyanka Dua    ICF 6.3         Code changes for Forking and 
 *						Multiple answer
* 11-Apr-2007  Deepti Goyal    ICF 6.3         Handling of codec change in PA 
 *                                              media connect request.				
 *
 * 17-Apr-2007  Abhishek Dutta  ICF 6.3         Added the changes for 
 *                                              TWC-UPDATE Handling
 * 10-Jul-2007  Abhishek Dutta  ICF 7.0         Added the changes for 
 *                                              Cancel call modify Handling
 * 11-Jul-2007  Neha Choraria   ICF 7.0         Changes made for Forking &
 *                                              Multiple Answer         
 * 19-Jul-2007  Amit Mahajan    ICF 6.5         Fix for SPR 15070 
 * 23-Sep-2009  Anurag Khare    SPR 19881       Fix for CSR 1-7507275
 * Copyright (c) 2009, Aricent.
 *---------------------------------------------------------------------------*/


#include "icf_ecode.h"
#include "icf_cc_prototypes.h"
#include "icf_cc_macro.h"
#include "icf_dbm_prototypes.h"



/******************************************************************************
 *
 * FUNCTION:        icf_cc_04_ssa_media_param
 *
 * DESCRIPTION:     This function is invoked when the CC is awaiting Modify
 *                  Media Session Response from MMI and it receives media 
 *					param (SIP 183) from SSA.
 *                  Current State: ICF_CC_04_AWT_MMS_RESP
 *                  Current Event: ICF_CC_MEDIA_PARAM
 *
 *****************************************************************************/
icf_return_t icf_cc_04_ssa_media_param(icf_cc_pdb_st *p_cc_pdb)
{
    icf_return_t		ret_val = ICF_SUCCESS;
	icf_nw_inc_media_param_st	*p_nw_inc_media_param = ICF_NULL;
	icf_boolean_t		handle_sdp = ICF_FALSE;

    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /* function entry trace */
	
	/* Media param can arrive in 3 flavours */
	/* i. 183 with same SDP from same UAS - unexpected event
	   ii. 183 with different SDP from same UAS(Multiple answer)
	   iii. 183 with SDP from another UAS(Forking) */

	p_nw_inc_media_param = (icf_nw_inc_media_param_st *)
							(p_cc_pdb->p_internal_msg->p_msg_data);

ICF_CHECK_MULT_ANS_ENBL_START(p_cc_pdb->p_glb_pdb)
		/* 183 with different SDP from same UAS(Multiple answer) */
		handle_sdp = ICF_TRUE;            
ICF_CHECK_MULT_ANS_ENBL_END

	if (ICF_FORKED_RESP == (p_cc_pdb->p_call_ctx->common_bitmask & ICF_FORKED_RESP))
	{
ICF_CHECK_MULT_ANS_DISBL_START(p_cc_pdb->p_glb_pdb)
              
                  ICF_PRINT(((icf_uint8_t *)"\n[CC]: Multiple Answer OFF in forking - Forceful\
                             call clear "));
                  icf_cc_delete_inc_media_sdp_info_list(p_cc_pdb,&p_nw_inc_media_param->p_sdp_list);
                  p_cc_pdb->p_call_ctx->release_cause = ICF_CAUSE_NETWORK_ERROR;
                  ret_val = icf_cc_04_generic_err(p_cc_pdb);
                  return ret_val;
              
ICF_CHECK_MULT_ANS_DISBL_END

		/* 183 with SDP from another UAS(Forking) */
		handle_sdp = ICF_TRUE;            
		/* Reset forked answer indication */
		ICF_CC_RESET_COMMON_BITMASK(p_cc_pdb,ICF_FORKED_RESP)
	}

	/* This field is set to false both in case of forked/non-forked response */ 
	p_cc_pdb->p_call_ctx->no_remote_sdp = ICF_FALSE;
        
        if((ICF_NULL != ICF_CC_CHECK_PENDING_EVENT(p_cc_pdb,
                        ICF_PENDING_EVENT_SSA_CONNECT)) ||
           (ICF_NULL != ICF_CC_CHECK_PENDING_EVENT(p_cc_pdb,
                        ICF_PENDING_EVENT_FMA_SSA_CONNECT)))
        {
             if (ICF_TRUE == handle_sdp)
             {
                  ret_val = icf_cc_delete_inc_media_sdp_info_list(
                                p_cc_pdb,
                                &p_nw_inc_media_param->p_sdp_list);
             }
             /* This event is not expected in current state, therefore do nothing */
             ICF_CC_TRACE(p_cc_pdb, ICF_UNEXPECTED_EVENT)
        }
	else if (ICF_TRUE == handle_sdp)
	{
		/* Reset Alert with Media if it exists */
		ICF_CC_RESET_PENDING_EVENT(p_cc_pdb,
			ICF_PENDING_EVENT_FMA_SSA_ALERT_WITH_MEDIA)

		/* Set Media param as pending event */
		ICF_CC_SET_PENDING_EVENT(p_cc_pdb,
			ICF_PENDING_EVENT_FMA_SSA_MEDIA_PARAM)
			
	   /* This p_list_1 is being set because this should be initialised 
		* with icf_inc_media_sdp_info_st before calling this function.
		*/
		p_cc_pdb->p_list_1 = p_nw_inc_media_param->p_sdp_list;
                p_cc_pdb->forked_resp_id = p_nw_inc_media_param->forked_resp_id;
		ret_val = icf_cc_validate_ith_level_m_line_check(p_cc_pdb);
		if (ICF_FAILURE == ret_val)
		{
			if (ICF_CC_STREAM_POS_VALIDATION_FAIL == 
				(p_cc_pdb->common_bitmask & ICF_CC_STREAM_POS_VALIDATION_FAIL))
			{
				ret_val = icf_cc_04_generic_err(p_cc_pdb);
			}
		}
		else
		{
			/* MMS shall be sent once ongoing MMS is complete */
		}
	}
	else
	{
		ret_val = icf_cc_delete_inc_media_sdp_info_list(
				p_cc_pdb,
				&p_nw_inc_media_param->p_sdp_list);

		/* This event is not expected in current state, therefore do nothing */
		ICF_CC_TRACE(p_cc_pdb, ICF_UNEXPECTED_EVENT)
	}

    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */
    return ret_val;
}


/******************************************************************************
 *
 * FUNCTION:        icf_cc_04_ssa_alert
 *
 * DESCRIPTION:     This function is invoked when the CC is awaiting Modify
 *                  Media Session Response from MMI and it receives alert with
 *                  media (SIP 180 without SDP) from SSA.
 *                  Current State: ICF_CC_04_AWT_MMS_RESP
 *                  Current Event: ICF_CC_SSA_ALERT
 *
 *****************************************************************************/
icf_return_t icf_cc_04_ssa_alert(icf_cc_pdb_st *p_cc_pdb)
{
    icf_return_t     ret_val = ICF_SUCCESS;

    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /* function entry trace */

	if (ICF_FORKED_RESP == (p_cc_pdb->p_call_ctx->common_bitmask & ICF_FORKED_RESP))
	{

ICF_CHECK_MULT_ANS_DISBL_START(p_cc_pdb->p_glb_pdb)
              
                  ICF_PRINT(((icf_uint8_t *)"\n[CC]: Multiple Answer OFF in forking - Forceful\
                             call clear "));
                  p_cc_pdb->p_call_ctx->release_cause = ICF_CAUSE_NETWORK_ERROR;
                  ret_val = icf_cc_04_generic_err(p_cc_pdb);
                  return ret_val;
              
ICF_CHECK_MULT_ANS_DISBL_END

		p_cc_pdb->p_call_ctx->no_remote_sdp = ICF_TRUE;
		/* Reset forked answer indication */
		ICF_CC_RESET_COMMON_BITMASK(p_cc_pdb,ICF_FORKED_RESP)
	}

    /* We have received Alert after 183. So inband info absent = false ind
     * will be sent to PA */
    p_cc_pdb->p_call_ctx->inband_info_present = ICF_FALSE;

    /*
     * Stop the call setup timer and start the alert timer
     */
    if(ICF_FAILURE == icf_cc_start_alert_timer(p_cc_pdb, ICF_FALSE, ICF_NULL))
    {
        /*Return ret_val as failure. Call will be now forcefully cleared*/
        ret_val = ICF_FAILURE;
    }
    /*
     * Send Alert req to PA
     */
    else if(ICF_FAILURE == icf_cc_send_pa_alert_ind(p_cc_pdb))
    {
       /*Return ret_val as failure. Call will be now forcefully cleared*/
        ret_val = ICF_FAILURE;
    }

    else
    {
        /*
        * Update the next state in the call context
        * As we have received alert next state will change to await connect
        */
        ICF_CC_SET_NEXT_STATE(p_cc_pdb,ICF_CC_05_AWT_CONNECT)
    }

    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */
    return ret_val;
}


/******************************************************************************
 *
 * FUNCTION:        icf_cc_04_ssa_alrt_with_media
 *
 * DESCRIPTION:     This function is invoked when the CC is awaiting Modify
 *                  Media Session Response from MMI and it receives alert with
 *                  media (SIP 180 with SDP) from SSA.
 *                  Current State: ICF_CC_04_AWT_MMS_RESP
 *                  Current Event: ICF_CC_SSA_ALERT_WITH_MEDIA
 *
 *****************************************************************************/
icf_return_t icf_cc_04_ssa_alrt_with_media(icf_cc_pdb_st *p_cc_pdb)
{
    icf_return_t     ret_val = ICF_SUCCESS;
	icf_nw_inc_alert_media_st	*p_nw_inc_alert_media = ICF_NULL;
	icf_boolean_t		handle_sdp = ICF_FALSE;

    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /* function entry trace */

	/* Alert with media can arrive in 3 flavours */
	/* i. 180 with same SDP from same UAS - same as icf_cc_04_ssa_alert
	   ii. 180 with different SDP from same UAS(Multiple answer)
	   iii. 180 with SDP from another UAS(Forking) */

	p_nw_inc_alert_media = (icf_nw_inc_alert_media_st *)
							(p_cc_pdb->p_internal_msg->p_msg_data);

ICF_CHECK_MULT_ANS_ENBL_START(p_cc_pdb->p_glb_pdb)
		/* 180 with different SDP from same UAS(Multiple answer) */
		handle_sdp = ICF_TRUE;            
ICF_CHECK_MULT_ANS_ENBL_END

	if (ICF_FORKED_RESP == (p_cc_pdb->p_call_ctx->common_bitmask & ICF_FORKED_RESP))
	{

ICF_CHECK_MULT_ANS_DISBL_START(p_cc_pdb->p_glb_pdb)
              
                  ICF_PRINT(((icf_uint8_t *)"\n[CC]: Multiple Answer OFF in forking - Forceful\
                             call clear "));
                  icf_cc_delete_inc_media_sdp_info_list(p_cc_pdb,&p_nw_inc_alert_media->p_sdp_list);
                  p_cc_pdb->p_call_ctx->release_cause = ICF_CAUSE_NETWORK_ERROR;
                  ret_val = icf_cc_04_generic_err(p_cc_pdb);
                  return ret_val;
              
ICF_CHECK_MULT_ANS_DISBL_END

		/* 180 with SDP from another UAS(Forking) */
		handle_sdp = ICF_TRUE;            
		/* Reset forked answer indication */
		ICF_CC_RESET_COMMON_BITMASK(p_cc_pdb,ICF_FORKED_RESP)
	}

	/* This field is set to false both in case of forked/non-forked response */ 
	p_cc_pdb->p_call_ctx->no_remote_sdp = ICF_FALSE;
        
        if((ICF_NULL != ICF_CC_CHECK_PENDING_EVENT(p_cc_pdb,
                        ICF_PENDING_EVENT_SSA_CONNECT)) ||
           (ICF_NULL != ICF_CC_CHECK_PENDING_EVENT(p_cc_pdb,
                        ICF_PENDING_EVENT_FMA_SSA_CONNECT)))
        {
             if (ICF_TRUE == handle_sdp)
             {
                  /* delete the SDP list as need not be processed here */
                  icf_cc_delete_inc_media_sdp_info_list(p_cc_pdb,
                                  &p_nw_inc_alert_media->p_sdp_list);
             }
             /* This event is not expected in current state, therefore do nothing */
             ICF_CC_TRACE(p_cc_pdb, ICF_UNEXPECTED_EVENT)
        }
        else if (ICF_TRUE == handle_sdp)
	{
		/* Reset Media Param if it exists */
		ICF_CC_RESET_PENDING_EVENT(p_cc_pdb,
			ICF_PENDING_EVENT_FMA_SSA_MEDIA_PARAM)

		/* Set Alert User as pending event */
		ICF_CC_SET_PENDING_EVENT(p_cc_pdb,
			ICF_PENDING_EVENT_FMA_SSA_ALERT_WITH_MEDIA)
			
		/* This p_list_1 is being set because this should be initialised 
		 * with icf_inc_media_sdp_info_st before calling this function.
		 */
		p_cc_pdb->p_list_1 = p_nw_inc_alert_media->p_sdp_list;
                p_cc_pdb->forked_resp_id = p_nw_inc_alert_media->forked_resp_id;
		ret_val = icf_cc_validate_ith_level_m_line_check(p_cc_pdb);
		if (ICF_FAILURE == ret_val)
		{
			if (ICF_CC_STREAM_POS_VALIDATION_FAIL == 
				(p_cc_pdb->common_bitmask & ICF_CC_STREAM_POS_VALIDATION_FAIL))
			{
				ret_val = icf_cc_02_generic_err(p_cc_pdb);
			}
		}
		else
		{
			/* MMS shall be sent once ongoing MMS is complete */
		}		
	}
	else
	{
       /* delete the SDP list as need not be processed here */
       icf_cc_delete_inc_media_sdp_info_list(p_cc_pdb,
                 &p_nw_inc_alert_media->p_sdp_list);

     /* As the handling for this event is the same as for event ssa_alert */
		ret_val = icf_cc_04_ssa_alert(p_cc_pdb);
	}

    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */
    return ret_val;
}


/******************************************************************************
 *
 * FUNCTION:        icf_cc_04_ssa_call_connect
 *
 * DESCRIPTION:     This function is invoked when the CC is awaiting Modify
 *                  Media Session Response from MMI and it receives connect
 *                  with media (SIP 200 OK ) from SSA.
 *                  Current State: ICF_CC_04_AWT_MMS_RESP
 *                  Current Event: ICF_CC_SSA_CALL_CONNECT
 *
 *****************************************************************************/
icf_return_t icf_cc_04_ssa_call_connect(icf_cc_pdb_st *p_cc_pdb)
{
    icf_return_t            ret_val = ICF_SUCCESS;
    icf_nw_inc_connect_st	*p_nw_inc_connect = ICF_NULL;
    icf_boolean_t			handle_sdp = ICF_FALSE;
    icf_boolean_t    mms_required = ICF_FALSE;
	p_nw_inc_connect = (icf_nw_inc_connect_st *)
							(p_cc_pdb->p_internal_msg->p_msg_data);

    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /* function entry trace */

         if (ICF_FORKED_RESP == (p_cc_pdb->p_call_ctx->common_bitmask & ICF_FORKED_RESP))
         {
ICF_CHECK_MULT_ANS_DISBL_START(p_cc_pdb->p_glb_pdb)
           
                  ICF_PRINT(((icf_uint8_t *)"\n[CC]: Multiple Answer Off in forking - Forceful\
                             call clear "));
                  icf_cc_delete_inc_media_sdp_info_list(p_cc_pdb,&p_nw_inc_connect->p_sdp_list);
                  p_cc_pdb->p_call_ctx->release_cause = ICF_CAUSE_NETWORK_ERROR;
                  ret_val = icf_cc_04_generic_err(p_cc_pdb);
                  /* Set network clear bit */
                  ICF_CC_SET_CALL_CLR_STATUS(p_cc_pdb,
                      ICF_CALL_CLEAR_STATUS_NW_CLEARED)

                  return ret_val;
          
ICF_CHECK_MULT_ANS_DISBL_END
         }

	
	if (ICF_NW_INC_CONNECT_WITH_SDP == 
		(p_nw_inc_connect->bitmask & ICF_NW_INC_CONNECT_WITH_SDP))
	{
		p_cc_pdb->sdp_present = ICF_TRUE;
ICF_CHECK_MULT_ANS_ENBL_START(p_cc_pdb->p_glb_pdb)
		handle_sdp = ICF_TRUE;
ICF_CHECK_MULT_ANS_ENBL_END

		if (ICF_FORKED_RESP == (p_cc_pdb->p_call_ctx->common_bitmask & ICF_FORKED_RESP))
		{
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

		if (ICF_FORKED_RESP == (p_cc_pdb->p_call_ctx->common_bitmask & ICF_FORKED_RESP))
                /* If 200 OK received (w/o SDP) is a forked resp */
		{
                        
                         p_cc_pdb->common_bitmask |= ICF_CC_CALL_CLEAR;
                         ICF_PRINT(((icf_uint8_t *)"\n[CC]: 200 OK w/o SDP is a forked resp\
                                    - Clearing Call "));
                         /* Reset forked answer indication */
                         ICF_CC_RESET_COMMON_BITMASK(p_cc_pdb,ICF_FORKED_RESP)
		}
                else /* If 200 OK received (w/o SDP) is not a forked resp */
                {
                        p_cc_pdb->forked_resp_id = p_nw_inc_connect->forked_resp_id;

                        /* checks if mms is required based on cmp between remote_sdp
                           & forked_sdp_info */

                        mms_required = icf_cc_check_mms_req_for_forked_resp(p_cc_pdb);
                        p_cc_pdb->p_call_ctx->no_remote_sdp = ICF_FALSE;
                        handle_sdp = ICF_FALSE;
                        /* Reset forked answer indication */
                        ICF_CC_RESET_COMMON_BITMASK(p_cc_pdb,ICF_FORKED_RESP)
                }

                /****** Changes for Forking & Multiple Answer End (Rel7.0) ******/

ICF_CHECK_MULT_ANS_ENBL_END
ICF_CHECK_MULT_ANS_DISBL_START(p_cc_pdb->p_glb_pdb)
            /*    p_cc_pdb->p_call_ctx->no_remote_sdp = ICF_TRUE; */
                p_cc_pdb->sdp_present = ICF_FALSE;
ICF_CHECK_MULT_ANS_DISBL_END

}

    /* Send connect ack to SSA */
    if(ICF_FAILURE == icf_cc_send_ssa_connect_ack(p_cc_pdb))
    {
         /* Set return value as failure */
         ret_val = ICF_FAILURE;
    }
    /*
    else
    {
       ICF_CC_SET_COMMON_BITMASK(p_cc_pdb,
                    ICF_CALL_CONNECT_DONE)
    }
    */
	if((ICF_TRUE == handle_sdp) || (ICF_TRUE == p_cc_pdb->p_call_ctx->no_remote_sdp) ||
                (ICF_CC_CALL_CLEAR == (p_cc_pdb->common_bitmask & ICF_CC_CALL_CLEAR)))
	{
		/* Reset Alert with Media and Media param if they exist */
		ICF_CC_RESET_PENDING_EVENT(p_cc_pdb,
			ICF_PENDING_EVENT_FMA_SSA_MEDIA_PARAM)
		ICF_CC_RESET_PENDING_EVENT(p_cc_pdb,
			ICF_PENDING_EVENT_FMA_SSA_ALERT_WITH_MEDIA)

		/* Set Connect as pending event */
		ICF_CC_SET_PENDING_EVENT(p_cc_pdb,
			ICF_PENDING_EVENT_FMA_SSA_CONNECT)
			
	   /* This p_list_1 is being set because this should be initialised 
		* with icf_inc_media_sdp_info_st before calling this function.
		*/
		p_cc_pdb->p_list_1 = p_nw_inc_connect->p_sdp_list;
                p_cc_pdb->forked_resp_id = p_nw_inc_connect->forked_resp_id;

        if ((!(ICF_NW_INC_CONNECT_WITH_SDP & p_nw_inc_connect->bitmask)) ||
                 (ICF_CC_CALL_CLEAR == (p_cc_pdb->common_bitmask & ICF_CC_CALL_CLEAR)))
        {
            ret_val = icf_cc_clear_call_no_remote_sdp(p_cc_pdb);
            return ret_val;
        }
        else if (ICF_FAILURE == icf_cc_validate_ith_level_m_line_check(p_cc_pdb))
		{
			if (ICF_CC_STREAM_POS_VALIDATION_FAIL == 
				(p_cc_pdb->common_bitmask & ICF_CC_STREAM_POS_VALIDATION_FAIL))
			{
				ret_val = icf_cc_02_generic_err(p_cc_pdb);
			}
		}
		else
		{
			/* MMS shall be sent once ongoing MMS is complete */
		}
	}
        else if((ICF_TRUE == mms_required) && (ICF_FAILURE != ret_val))
        {
              /* MMS shall be sent once ongoing MMS is complete */
        }
	else
	{
	   /* CHeck if ongoing action is ALERT_WITH_MEDIA or SSA_MEDIA_PARAM then
		* set connect as pending event else process connect request */
		if(ICF_CC_SSA_ALERT_WITH_MEDIA == 
            p_cc_pdb->p_call_ctx->ongoing_event||
			ICF_CC_SSA_MEDIA_PARAM == p_cc_pdb->p_call_ctx->ongoing_event)
		{
			/* Buffer the 200 ok */
			ICF_CC_SET_PENDING_EVENT(p_cc_pdb,ICF_PENDING_EVENT_SSA_CONNECT)
                        ICF_CC_SET_NEXT_STATE(p_cc_pdb,ICF_CC_06_CONNECTED)         
		}
		else
		{
			/* Start media timer for the request sent*/
			ret_val = icf_cc_start_media_timer(p_cc_pdb);
			
			/* Send COnnect ind to PA */
			if(ICF_FAILURE == icf_cc_send_pa_connect_ind(p_cc_pdb))
			{
				/* Set return value as failure */
				ret_val = ICF_FAILURE;
			}
                        if (ICF_FAILURE != ret_val && 
                            ICF_TRUE == p_cc_pdb->p_call_ctx->update_reqd) 
                        {
                           /*Send UPDATE request*/
                           if (ICF_FAILURE == icf_cc_ssa_send_update_msg(p_cc_pdb))
                           {
                              ret_val = ICF_FAILURE;
                           }
                           else
                           {
                              ICF_CC_SET_NEXT_STATE(p_cc_pdb,
					     ICF_CC_14_AWT_CALL_MODIFY_RESP)
                           }
                        }
                        else if (ICF_FAILURE != ret_val)
                        {
                           ICF_CC_SET_NEXT_STATE(p_cc_pdb,ICF_CC_06_CONNECTED)
                        }  
	        }
		p_cc_pdb->p_list_1 = p_nw_inc_connect->p_sdp_list;
		icf_cc_delete_inc_media_sdp_info_list(p_cc_pdb,&p_cc_pdb->p_list_1);
		/* Set return value as failure */
 		/*ICF_CC_SET_NEXT_STATE(p_cc_pdb,ICF_CC_06_CONNECTED)*/
    }
    /* Fix for SPR 17159 */
    ICF_CC_SET_COMMON_BITMASK(p_cc_pdb,
                    ICF_CALL_CONNECT_DONE)
    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */

    return ret_val;
}


/******************************************************************************
 *
 * FUNCTION:        icf_cc_04_mms_resp
 *
 * DESCRIPTION:     This function is invoked when the CC is awaiting Modify
 *                  Media Session Response from MMI and it receives Response
 *                  for the same from MMI
 *                  Current State: ICF_CC_04_AWT_MMS_RESP
 *                  Current Event: ICF_CC_MMS_RESP
 *
 *****************************************************************************/
icf_return_t icf_cc_04_mms_resp(icf_cc_pdb_st *p_cc_pdb)
{
    icf_return_t     ret_val = ICF_SUCCESS;
    icf_uint8_t      ongoing_event = 0;

    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /* function entry trace */
    
    /*
     * If ongoing action was alert with media sent alert to PA
     */
    if(ICF_CC_06_CONNECTED == ICF_CC_GET_NEXT_STATE(p_cc_pdb))    
    {
        /* If next state is ICF_CC_06_CONNECTED then we need to stop any of 
         * the currently executing timer */ 
         icf_cc_stop_current_timer(p_cc_pdb);
    }

    ongoing_event =  ICF_CC_GET_ONGOING_EVENT(p_cc_pdb);

	/*Function will analyse the MMS response and return offer_rejected
	 *if offer is rejected by MM i.e add is failed or all modified streams
	 *are rejected*/
	icf_cc_analyse_mms_resp(p_cc_pdb);

	if(ICF_CC_SSA_ALERT_WITH_MEDIA == ongoing_event)
	{
        ret_val = icf_cc_mms_resp_ssa_alert_with_media(p_cc_pdb);
	}
	else if(ICF_CC_SSA_MEDIA_PARAM == ongoing_event)
	{
		ret_val = icf_cc_mms_resp_ssa_media_param(p_cc_pdb);
	}
	else if(ICF_CC_SSA_OFFER_CHANGE == ongoing_event)
	{
/*cancel_reinv*/
		ret_val = icf_cc_mms_resp_ssa_offer_change(p_cc_pdb);
	}
	else if (ICF_CC_MEDIA_UPDATE_RESP == ongoing_event)
	{
		ret_val = icf_cc_mms_resp_ssa_offer_change_resp(p_cc_pdb);
	}
#ifdef ICF_FAX_SUPPORT
        else if (ICF_CC_PA_STOP_FAX_REQ == ongoing_event)
    	{
        	ret_val = icf_cc_mms_resp_pa_stop_fax_req(p_cc_pdb);
    	} 		
        else if (ICF_CC_SSA_STOP_FAX_REQ == ongoing_event)
    	{
        	ret_val = icf_cc_mms_resp_ssa_stop_fax_req(p_cc_pdb);
    	} 		
        else if (ICF_CC_SSA_STOP_FAX_RESP == ongoing_event)
    	{
        	ret_val = icf_cc_mms_resp_ssa_stop_fax_resp(p_cc_pdb);
    	} 		
	else if (ICF_CC_PA_FAX_REQ == ongoing_event)
	{
		ret_val = icf_cc_mms_resp_pa_start_fax_req(p_cc_pdb);
	}
    else if (ICF_CC_SSA_FAX_REQ == ongoing_event)
	{
		ret_val = icf_cc_mms_resp_ssa_start_fax_req(p_cc_pdb);
	}
    else if (ICF_CC_SSA_FAX_RESP == ongoing_event)
	{
		ret_val = icf_cc_mms_resp_ssa_start_fax_resp(p_cc_pdb);
	}
#endif
	else if(ICF_CC_ALERT_PRACK == ongoing_event)
	{
		ret_val = icf_cc_mms_resp_alert_prack(p_cc_pdb);
	}
	else if(ICF_CC_MEDIA_PARAM_PRACK == ongoing_event)
	{
		ret_val = icf_cc_mms_resp_media_param_prack(p_cc_pdb);
	}
	else if(ICF_CC_SSA_CONNECT_ACK == ICF_CC_GET_ONGOING_EVENT(p_cc_pdb))
	{
		ret_val = icf_cc_mms_resp_ssa_connect_ack(p_cc_pdb);
	}
	else if(ICF_CC_SSA_CALL_CONNECT == ongoing_event)
	{
		ret_val = icf_cc_mms_resp_ssa_call_connect(p_cc_pdb);
	}
	else if(ICF_CC_PA_CALL_CONNECT == ongoing_event)
	{
		ret_val = icf_cc_mms_resp_pa_call_connect(p_cc_pdb);
	}
	else if(ICF_CC_PA_CALL_MODIFY_REQ == ongoing_event)
	{
/*call_mod_cancel*/
        if (ICF_NULL != ICF_CC_CHECK_PENDING_EVENT(p_cc_pdb,
                          ICF_PENDING_EVENT_PA_CALL_MOD_CANCEL_REQ))
        {
            if(ICF_NULL != ICF_CC_CHECK_COMMON_BITMASK_2(p_cc_pdb,ICF_CALL_MOD_CANCEL_REQ))
            {

               /*Case when call mod cancel comes before Re-Inv sent*/
               ICF_CC_SET_ONGOING_EVENT(p_cc_pdb,ICF_CC_SSA_CALL_MODIFY_CANCEL_RESP)
               /*Revert media to last comitted SDP*/
               if(ICF_FAILURE == icf_cc_revert_mms_for_call_mod_cancel_req(p_cc_pdb))
               {
                   ret_val = ICF_FAILURE;
               }
               ret_val = ICF_SUCCESS;
               return ret_val;
            }
        }
        else
        {
		    ret_val = icf_cc_mms_resp_pa_call_modify_req(p_cc_pdb);
        }
	}
/*call_mod_cancel*/
    else if (ICF_CC_PA_ALERT == ongoing_event)
    {
        ret_val = icf_cc_mms_resp_pa_alert(p_cc_pdb);
    }
    else if (ICF_CC_PA_MEDIA_CONNECT_REQ == ongoing_event)
    {
        ret_val = icf_cc_mms_resp_pa_media_connect(p_cc_pdb);
    }
    else if (ICF_CC_SSA_REINVITE_ACK == ongoing_event)
    {
        ret_val = icf_cc_mms_resp_media_change_ack(p_cc_pdb);
    }    
	else if ((ICF_CC_ONGOING_EVENT_LOCAL_MEDIA_REVERT == ongoing_event) || 
        (ICF_CC_ONGOING_EVENT_REMOTE_MEDIA_REVERT == ongoing_event))
	{
		    /* this is call modify specifc handling for reverting the media change */
		    ret_val = icf_cc_mms_resp_media_revert(p_cc_pdb);
    }
/*cancel_reinv*/
    else if(ICF_CC_SSA_CALL_MODIFY_CANCEL_REQ == ongoing_event)
    {
        /*Case when mms req is complete for cancel reinvite.Now send
          indication to PA*/
        if(ICF_NULL != ICF_CC_CHECK_PENDING_EVENT(p_cc_pdb,
                          ICF_PENDING_EVENT_OFFER_CHANGE_REQ))
        {
           /*Case when cancel reinvite is ongoing and new reinvite is rcvd*/
           ret_val = icf_cc_otg_clear_pend_evt_mms_rsp(p_cc_pdb);
        }
        else
        {
            if (ICF_FAILURE == 
                icf_cc_mms_resp_ssa_call_modify_cancel_req(p_cc_pdb))
            {
                ret_val = ICF_FAILURE;
            }
        }
    }
/*call_mod_cancel*/
    else if (ICF_CC_PA_CALL_MODIFY_CANCEL_REQ == ongoing_event)
    {
        if (ICF_FAILURE == 
            icf_cc_pa_call_mod_cancel_req_ongoing_event_handling(p_cc_pdb))
        {
           ret_val = ICF_FAILURE;
        }
    }
    else if (ICF_CC_SSA_CALL_MODIFY_CANCEL_RESP == ongoing_event)
    {
        if(ICF_FAILURE == 
           icf_cc_ssa_call_mod_cancel_resp_ongoing_event_handling(p_cc_pdb))
        {
           ret_val = ICF_FAILURE;
        }
    }
/*call_mod_cancel*/


#ifdef ICF_SESSION_TIMER
    if (ICF_TRUE == p_cc_pdb->p_glb_pdb->p_call_ctx->session_timer_enable)
    {    
       if(ICF_CC_SESSION_REFRESH_RESP == ongoing_event)
       {
         /* If the mms was done for applying media in session_refresh resp
          *  then send call_modify_ind to PA */
        ret_val = icf_cc_mms_resp_ssa_session_refresh_resp(p_cc_pdb);
     }
    }
#endif
    /* If mms is sent remain in same state else change call state */ 
    if(ICF_SUCCESS == ret_val && ICF_FALSE == p_cc_pdb->mms_sent)
    {
        /* Set call state,
        * reset next state, ongoing event */
        ICF_CC_SET_CALL_STATE(p_cc_pdb,ICF_CC_GET_NEXT_STATE(p_cc_pdb))
        ICF_CC_RESET_NEXT_STATE(p_cc_pdb)
        ICF_CC_RESET_ONGOING_EVENT(p_cc_pdb)
            
        /*Set return status to CM and ongoing event */
        ICF_CC_SET_STATUS(p_cc_pdb, ICF_STATUS_MEDIA_ONGOING_ENDED)
            
        /* Reset media ongoing flag */
        ICF_CC_RESET_MEDIA_ONGOING(p_cc_pdb)
    }

    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */

    return ret_val;
}




/******************************************************************************
 *
 * FUNCTION:        icf_cc_04_ssa_call_term_req
 *
 * DESCRIPTION:     This function is invoked when the CC is awaiting  Modify
 *                  Media Session Response from MMI and it receives Call
 *                  Terminate Request from SSA (SIP BYE received from peer).
 *                  Current State: ICF_CC_04_AWT_MMS_RESP
 *                  Current Event: ICF_CC_SSA_CALL_TERMINATE_REQ
 *
 *****************************************************************************/
icf_return_t icf_cc_04_ssa_call_term_req(icf_cc_pdb_st *p_cc_pdb)
{
    icf_return_t     ret_val = ICF_SUCCESS;

    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /* function entry trace */

    /*This event is expected only if call is connected*/

    if (ICF_NULL != ICF_CC_CHECK_COMMON_BITMASK(p_cc_pdb,
                    ICF_CALL_CONNECT_DONE))
    {
        /*
         * In this case if ongoing action is for UPDATE/REINVTE send
         * failuire response for it and clear the call
         * prack cannot be pending event
         */

       ICF_CC_SET_RELEASE_TYPE(p_cc_pdb,ICF_REL_TYPE_NW_INITIATED)

       ICF_CC_TRACE(p_cc_pdb, ICF_TRACE_REL_TYPE_NW_INIT)

       if(ICF_FAILURE == icf_cc_clear_pending_events(p_cc_pdb))
       {
            /*Return ret_val as failure. Call will be now forcefully cleared*/
            ret_val = ICF_FAILURE;
       }

        else if(ICF_FAILURE == icf_cc_start_release_timer(p_cc_pdb))
        {
            /*Return ret_val as failure. Call will be now forcefully cleared*/
            ret_val = ICF_FAILURE;
        }



        else if(ICF_FAILURE == icf_cc_send_ssa_call_term_rsp(p_cc_pdb))
        {
            /*Return ret_val as failure. Call will be now forcefully cleared*/
            ret_val = ICF_FAILURE;
        }
        else if(ICF_FAILURE == icf_cc_send_dms_req(p_cc_pdb))
        {
            /*Return ret_val as failure. Call will be now forcefully cleared*/
            ret_val = ICF_FAILURE;
        }
        else
        {
            /* Set network clear bit */
            ICF_CC_SET_CALL_CLR_STATUS(p_cc_pdb,
                    ICF_CALL_CLEAR_STATUS_NW_CLEARED)
            ICF_CC_TRACE(p_cc_pdb, ICF_TRACE_CALL_STATUS_NW_CLR)

            /* Set comment as Await_DMS_RESP */
            ICF_CC_SET_CALL_STATE(p_cc_pdb,ICF_CC_07_AWT_DMS_RESP)
        }

    }
    else
    {
       /* RAISE ERROR TRACE  */
    }

    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */
    return ret_val;
}


/******************************************************************************
 *
 * FUNCTION:        icf_cc_04_ssa_remote_reject
 *
 * DESCRIPTION:     This function is invoked when the CC is awaiting Modify
 *                  Media Session Response from MMI and it receives Remote Call
 *                  Reject from SSA (SIP 3xx/4xx/5xx received from peer).
 *                  Current State: ICF_CC_04_AWT_MMS_RESP
 *                  Current Event: ICF_CC_SSA_REMOTE_REJECT
 *
 *****************************************************************************/
icf_return_t icf_cc_04_ssa_remote_reject(icf_cc_pdb_st *p_cc_pdb)
{
    icf_return_t     ret_val = ICF_SUCCESS;

    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /* function entry trace */

    /*
     * In this case if ongoing action is for UPDATE/REINVTE send
     * failuire response for it and clear the call
     */
     if(ICF_FAILURE == icf_cc_clear_pending_events(p_cc_pdb))
     {
         /*Return ret_val as failure. Call will be now forcefully cleared*/
         ret_val = ICF_FAILURE;
     }

    /*
     *  If call is not answered/connected, call fsm function
     * icf_cc_03_ssa_remote_reject
     * else raise a error
     * */
    if (ICF_NULL == ICF_CC_CHECK_COMMON_BITMASK(p_cc_pdb,
                    ICF_CALL_CONNECT_DONE))
    {
        ret_val = icf_cc_03_ssa_remote_reject(p_cc_pdb);
    }
    else
    {
       /* RAISE ERROR TRACE  */
    }

    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */
    return ret_val;
}


/******************************************************************************
 *
 * FUNCTION:        icf_cc_04_ssa_redirect
 *
 * DESCRIPTION:     This function is invoked when the CC is awaiting Modify
 *                  Media Session Response from MMI and it receives reroute call
 *                  Req from SSA (SIP 302 received from peer).
 *                  Current State: ICF_CC_04_AWT_MMS_RESP
 *                  Current Event: ICF_CC_SSA_REDIRECT
 *
 *****************************************************************************/
icf_return_t icf_cc_04_ssa_redirect(icf_cc_pdb_st *p_cc_pdb)
{
    icf_return_t     ret_val = ICF_SUCCESS;

    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /* function entry trace */

    /* Send ACk for REDIRECT from SSA and DMS to MMI*/
    if(ICF_FAILURE == icf_cc_send_ssa_redirect_ack(p_cc_pdb))
    {
        /* Return error to top level function, call will be forcefully cleared*/
        ret_val = ICF_FAILURE;
    }
    else if(ICF_FAILURE == icf_cc_send_dms_req(p_cc_pdb))
    {
        /* Return error to top level function, call will be forcefully cleared*/
        ret_val = ICF_FAILURE;
    }
    else if(ICF_SUCCESS == icf_cc_validate_redirect_req(p_cc_pdb))
    {
        /* If redirect message is valid, mark nextstate as await_cms
         * and set ongoing event as REDIRECT
         */

        ICF_CC_SET_NEXT_STATE(p_cc_pdb,ICF_CC_01_AWT_CMS_RESP)

        ICF_CC_SET_ONGOING_EVENT(p_cc_pdb,ICF_CC_SSA_REDIRECT)

    }
    else
    {
        /*
         *  Set the release cause and start the release timer
         */
        
        p_cc_pdb->p_call_ctx->release_cause = ICF_CALL_CLEAR_REASON_NW_ERROR;

        ICF_CC_SET_RELEASE_TYPE(p_cc_pdb, ICF_REL_TYPE_INTERNAL_INITIATED)
        ICF_CC_TRACE(p_cc_pdb, ICF_TRACE_REL_TYPE_INT_ERR)
        
        if (ICF_FAILURE ==icf_cc_start_release_timer(p_cc_pdb))
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
    }

    /*Goto state AWAIT DMS RESP */
    ICF_CC_SET_CALL_STATE(p_cc_pdb,ICF_CC_07_AWT_DMS_RESP)

    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */
    return ret_val;
}




/******************************************************************************
 *
 * FUNCTION:        icf_cc_04_pa_call_term_req
 *
 * DESCRIPTION:     This function is invoked when the CC is awaiting Modify
 *                  Media Session Response from MMI and it receives Call
 *                  Terminate Request from PA.
 *                  Current State: ICF_CC_04_AWT_MMS_RESP
 *                  Current Event: ICF_CC_PA_CALL_TERMINATE_REQ
 *
 *****************************************************************************/
icf_return_t icf_cc_04_pa_call_term_req(icf_cc_pdb_st *p_cc_pdb)
{

    icf_return_t     ret_val = ICF_SUCCESS;
    
    icf_uint8_t  next_state;

    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /* function entry trace */
    /*
     * Set release type in call context as PA_INITIAITED
     */
    ICF_CC_SET_RELEASE_TYPE(p_cc_pdb,ICF_REL_TYPE_PA_INITIATED)

    /*
     * In this case if UPDATE/REINVTE is pending action or ongoing event send
     * failuire response for it and clear the call
     */

    if(ICF_FAILURE == icf_cc_clear_pending_events(p_cc_pdb))
    {
        /* Return error to top level function, call will be forcefully cleared*/
        ret_val = ICF_FAILURE;
     }

    ret_val = icf_cc_chk_user_busy_n_srvc(p_cc_pdb);
    
    /*If status was not returned as ICF_STATUS_CALL_FWD_BUSY_LINE continue 
     *with normal call clearing*/
    if(ICF_SUCCESS == ret_val && 
                ICF_STATUS_CALL_FWD_BUSY_LINE != 
                p_cc_pdb->p_internal_msg->msg_hdr.status)
    {

        /*
         * Start the release timer
         */
        if(ICF_FAILURE == icf_cc_start_release_timer(p_cc_pdb))
        {
            /* Return error to top level function, call will be forcefully 
             * cleared*/
            ret_val = ICF_FAILURE;
        }
        /* Send BYE to network if call has been connected */
        else if (ICF_NULL != ICF_CC_CHECK_COMMON_BITMASK(p_cc_pdb,
                    ICF_CALL_CONNECT_DONE))
        {
            /* If signaling ongoing is false we can send bye to remote else
             * wait for start proc signalling */
            if((ICF_INT_MODULE_INVALID == p_cc_pdb->p_call_ctx->signalling_ongoing)||
               (ICF_INT_MODULE_CC == p_cc_pdb->p_call_ctx->signalling_ongoing))
            {
                /* Send call clear request to the SSA */
                if(ICF_FAILURE ==icf_cc_send_ssa_call_term_req(p_cc_pdb))
                {
                    /* Return error to top level function,
                     * call will be forcefully cleared*/
                    ret_val = ICF_FAILURE;
                }
                else
                {
                    /* Update the next state in the call context */
                    ICF_CC_SET_NEXT_STATE(p_cc_pdb,
                            ICF_CC_08_AWT_CALL_TERMINATE_RESP)
                }
            }
            else /* We cannot send bye for now. wait for start proc signaling */
            {
                /*Return service terminate status to CM*/
                ICF_CC_SET_STATUS(p_cc_pdb,
                        ICF_STATUS_CALL_R_SRV_TERMINATE)
                    /*
                     * Mark next state as ICF_CC_12_WAIT_START_PROC_SIG
                     */
                    ICF_CC_SET_NEXT_STATE(p_cc_pdb,
                            ICF_CC_12_WAIT_START_PROC_SIG)

				ICF_CC_SET_PENDING_EVENT(p_cc_pdb,
                    ICF_PENDING_EVENT_PA_CALL_TERM_REQ)
            }
        }

        /* Call has not been answered, if outgoing call send cancle req to SSA */
        else if (ICF_NULL == ICF_CC_CHECK_COMMON_BITMASK(p_cc_pdb,
                    ICF_CALL_DIR))
        {
            /* This is a outgoing call */
            if(ICF_FAILURE == icf_cc_send_ssa_cancel_req(p_cc_pdb))
            {
                /* Return error to top level function,
                 * call will be forcefully cleared*/
                ret_val = ICF_FAILURE;
            }
        }

        else if (ICF_NULL != ICF_CC_CHECK_COMMON_BITMASK(p_cc_pdb,
                    ICF_CALL_DIR))
        {
            /*  call_type == INCOMING_CALL */

             next_state = ICF_CC_GET_NEXT_STATE(p_cc_pdb);
          
            /* validation for the response code(if present) */
             icf_cc_validate_response_code_in_term_req(p_cc_pdb);

            /* For a incoming call we need to send reject request */

            /*
             * If next state was 9 or 10( we are waiting for prack from remote),
             * set call terminate as pending event.
             */
            if(ICF_CC_09_AWT_MEDIA_PARAM_PRACK == next_state||
                    ((ICF_CC_10_AWT_ALERT_PRACK == next_state) && 
                     ICF_CC_MEDIA_PARAM_PRACK != 
                     ICF_CC_GET_ONGOING_EVENT(p_cc_pdb)))
            {
                /* Set call terminate as pending event */
                ICF_CC_SET_PENDING_EVENT(p_cc_pdb,
                        ICF_PENDING_EVENT_PA_CALL_TERM_REQ)
                    /*
                     * Set next state to call state as call terminate will be 
                     * processed on getting PRACK from remote
                     */

                    ICF_CC_SET_CALL_STATE(p_cc_pdb,next_state)
            }
            else
            {
                /* As prack is not awaited, send reject to remote party */
                if(ICF_FAILURE == icf_cc_send_ssa_reject(p_cc_pdb))
                {
                    /* Return error to top level function,
                     * call will be forcefully cleared*/
                    ret_val = ICF_FAILURE;
                }
            }

        }
        
        /* if call_terminate is not pending event send DMS to MMI */
        if (ICF_NULL == ICF_CC_CHECK_PENDING_EVENT(p_cc_pdb,
                    ICF_PENDING_EVENT_PA_CALL_TERM_REQ) && 
                ICF_SUCCESS == ret_val)
        {
            /*
             * Send a DMS request to MMI
             */
            if(ICF_FAILURE == icf_cc_send_dms_req(p_cc_pdb))
            {
                /* Return error to top level function,
                 * call will be forcefully cleared*/
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
    }
    else 
    {
        next_state = ICF_CC_GET_NEXT_STATE(p_cc_pdb);

        /* validation for the response code(if present) */
        icf_cc_validate_response_code_in_term_req(p_cc_pdb);

        /*
         * If next state was 9 or 10( we are waiting for prack from remote),
         * set call terminate as pending event.
         */
        if(ICF_CC_09_AWT_MEDIA_PARAM_PRACK == next_state||
                ICF_CC_10_AWT_ALERT_PRACK == next_state)
        {
            /* Set call terminate as pending event */
            ICF_CC_SET_PENDING_EVENT(p_cc_pdb,
                    ICF_PENDING_EVENT_PA_CALL_TERM_REQ)
                /*
                 * Set next state to call state as call terminate will be 
                 * processed on getting PRACK from remote
                 */

                ICF_CC_SET_CALL_STATE(p_cc_pdb,next_state)
        }
        /*Reset the status CALL_FORWARD as we would give call forward 
         * indication to CM only after PRACK is received and 200 OK is sent
         * for the same*/
        p_cc_pdb->p_internal_msg->msg_hdr.status = ICF_STATUS_CONTINUE;

    }

    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */
    return ret_val;

}

/******************************************************************************
 *
 * FUNCTION:        icf_cc_04_ssa_offer_change
 *
 * DESCRIPTION:     This function is invoked when the CC is awaiting Modify
 *                  Media Session Response from MMI and it receives a session
 *                  param change from SSA (SIP UPDATE/RE-INVITE).
 *                  Current State: ICF_CC_04_AWT_MMS_RESP
 *                  Current Event: ICF_CC_SSA_OFFER_CHANGE
 *
 *****************************************************************************/
icf_return_t icf_cc_04_ssa_offer_change(icf_cc_pdb_st *p_cc_pdb)
{
    icf_return_t     ret_val = ICF_SUCCESS;
    icf_uint8_t      ongoing_event;
    icf_nw_inc_media_change_req_st *p_offer_change_req = ICF_NULL;

    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /* function entry trace */


    /* If ongoing action ia update/Reinvite/PRACK or we have not applied
     * remote media yet and we get another update ,we will not handle this
     * and release the call
     */
    ICF_ASSERT(p_cc_pdb->p_glb_pdb, p_cc_pdb->p_internal_msg->p_msg_data)

    p_offer_change_req = (icf_nw_inc_media_change_req_st*)
            p_cc_pdb->p_internal_msg->p_msg_data;

    ongoing_event = ICF_CC_GET_ONGOING_EVENT(p_cc_pdb);
    
    /* If we are applying sdp for update or alert prack or media prack
     * then need to send failure to remote */
    if((ICF_CC_SSA_OFFER_CHANGE == ongoing_event) ||
	    (ICF_CC_PA_CALL_MODIFY_REQ == ongoing_event) || 
	    (ICF_CC_ALERT_PRACK == ongoing_event) ||
	    (ICF_CC_MEDIA_PARAM_PRACK == ongoing_event)) 
	    
    {
        /*Would direct SSA to send 491 on NW*/
        p_cc_pdb->p_call_ctx->offer_failure_reason = ICF_REQUEST_PENDING;
        icf_cc_delete_inc_media_sdp_info_list(p_cc_pdb,
                            &p_offer_change_req->p_sdp_list);
        
        /* Send failure response for update to remote */
        if(ICF_FAILURE == icf_cc_send_ssa_offer_chg_err(p_cc_pdb))
        {
            ret_val = ICF_FAILURE;
        }
    }
    else if ((ICF_NULL != ICF_CC_CHECK_PENDING_EVENT(p_cc_pdb,
                          ICF_PENDING_EVENT_OFFER_CHANGE_REQ))||
			(ICF_NULL != ICF_CC_CHECK_PENDING_EVENT(p_cc_pdb,
                          ICF_PENDING_EVENT_PA_CALL_MODIFY_REQ))||
            (ICF_NULL != ICF_CC_CHECK_PENDING_EVENT(p_cc_pdb,
                        ICF_PENDING_EVENT_ALERT_PROV_RESP_WITH_SDP))||
            (ICF_NULL != ICF_CC_CHECK_PENDING_EVENT(p_cc_pdb,
                        ICF_PENDING_EVENT_MEDIA_PROV_RESP_WITH_SDP)))
    {
        /*Would direct SSA to send 491 on NW*/
        p_cc_pdb->p_call_ctx->offer_failure_reason = ICF_REQUEST_PENDING;
        
        /* Send failure response for update to remote */
        if(ICF_FAILURE == icf_cc_send_ssa_offer_chg_err(p_cc_pdb))
        {
            /* This is error at interface level, call will be cleared
             * forcefully*/
            ret_val = ICF_FAILURE;
        }
        icf_cc_delete_inc_media_sdp_info_list(p_cc_pdb,
                            &p_offer_change_req->p_sdp_list);
    }    
    else if(ICF_CC_SSA_CALL_MODIFY_CANCEL_REQ == ongoing_event) 
    {
       /*Case when cancel reinvite ongoing and new reinvite is received*/
       /* This function is checking if there is any mismatch in the
        * oreder of streams.
        */
       p_cc_pdb->p_list_1 = p_offer_change_req->p_sdp_list;
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
           ICF_CC_SET_PENDING_EVENT(p_cc_pdb,
                ICF_PENDING_EVENT_OFFER_CHANGE_REQ)
        }
    }
    else /* We can process update request */
    {
        if(p_offer_change_req->bitmask & ICF_NW_INC_MEDIA_CHG_SDP_PRESENT)
        {
            ICF_CC_RESET_COMMON_BITMASK(p_cc_pdb,ICF_CALL_MODIFY_DIR)
            /* Set the call modify direction flag */
            p_cc_pdb->p_call_ctx->common_bitmask |= ICF_CALL_MODIFY_DIR;
            
            /* Set UPDATE as pending event */
            ICF_CC_SET_PENDING_EVENT(p_cc_pdb,
                ICF_PENDING_EVENT_OFFER_CHANGE_REQ)

            /*Update remote_received_sdp in stream_sdp_info_list
             *and prepare MMS request once the MMS resoponse received*/
            p_cc_pdb->p_list_1 = p_offer_change_req->p_sdp_list;
            /* Now if added or rejected streams are present 
            * fail the offer.We are entertaining FAX in only the 
            * connected state
            */
            if ((ICF_NW_INC_MEDIA_CHG_REJECTED_STREAM  ==
                (ICF_NW_INC_MEDIA_CHG_REJECTED_STREAM &
                p_offer_change_req->bitmask)) || 
                (ICF_NW_INC_MEDIA_ADD_STREAM  ==
                (ICF_NW_INC_MEDIA_ADD_STREAM &
                p_offer_change_req->bitmask)))
            {
                icf_cc_delete_inc_media_sdp_info_list(p_cc_pdb,
                                      &p_offer_change_req->p_sdp_list);

                ret_val = icf_cc_send_ssa_offer_chg_err(p_cc_pdb);
                ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */
                return ret_val;
            }
            /* This function shall validate the i'th level
            * matching and copy the received SDP into the 
            * call context
            */
            if (ICF_FAILURE == 
                icf_cc_validate_ith_level_m_line_check(p_cc_pdb))
            {
                /*Would direct SSA to send 491 on NW*/
                p_cc_pdb->p_call_ctx->offer_failure_reason = ICF_REQUEST_PENDING;
                
                icf_cc_delete_inc_media_sdp_info_list(p_cc_pdb,
                            &p_offer_change_req->p_sdp_list);
                ret_val = icf_cc_send_ssa_offer_chg_err(p_cc_pdb);

                /* Check if ith level check has failed */
                if (ICF_CC_STREAM_POS_VALIDATION_FAIL == (p_cc_pdb->common_bitmask & 
                    ICF_CC_STREAM_POS_VALIDATION_FAIL))
                {
                    /* Invoke the generic call clear handler corresponding to that state */
                    ret_val = icf_cc_04_generic_err(p_cc_pdb); 
                }
                else
                {
                    /* This is error at interface level, call will be cleared
                    * forcefully*/
                    ret_val = ICF_FAILURE;
                }
                ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */
                return ret_val;
            }     
        }
        else 
        {
            /*200 OK will be sent after MMS response is received*/

        }
    }

    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */
    return ret_val;
}


/******************************************************************************
 *
 * FUNCTION:        icf_cc_04_mm_error_ind
 *
 * DESCRIPTION:     This function is invoked when the CC is awaiting Modify
 *                  Media Session Response from MMI and Error indication is
 *                  received from MM to clear the call.
 *                  Current State: ICF_CC_04_AWT_MMS_RESP
 *                  Current Event: ICF_CC_MM_ERR_IND
 *
 *****************************************************************************/
icf_return_t icf_cc_04_mm_error_ind(icf_cc_pdb_st *p_cc_pdb)
{
    icf_return_t       ret_val = ICF_SUCCESS;
    icf_mm_err_ind_st  *p_mm_err_ind;
    icf_mmi_reason_t   media_error;
    icf_uint8_t        ongoing_event,next_state;
    
    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /* function entry trace */

    /*MM ERROR IND will only received to clear the call*/

    ICF_ASSERT(p_cc_pdb->p_glb_pdb, p_cc_pdb->p_internal_msg->p_msg_data)
    p_mm_err_ind = (icf_mm_err_ind_st*)
                      (p_cc_pdb->p_internal_msg->p_msg_data);
   

    media_error = p_mm_err_ind->reason;
    ongoing_event = ICF_CC_GET_ONGOING_EVENT(p_cc_pdb);
    
    ret_val = icf_cc_clear_pending_events(p_cc_pdb);

    if(ICF_SUCCESS == ret_val)
    {
        /* Set release type as MM initiated */
        ICF_CC_SET_RELEASE_TYPE(p_cc_pdb, ICF_REL_TYPE_MM_INITIATED)
        
        ICF_CC_TRACE(p_cc_pdb, ICF_TRACE_REL_TYPE_MM_INIT)
        
        /* Set release cause in call context */
        ICF_CC_SET_RELEASE_CAUSE(p_cc_pdb, 
        ICF_CALL_CLEAR_MEDIA_MISMATCH)
        
        if(ICF_FAILURE == icf_cc_start_release_timer(p_cc_pdb))
        {
            /* We have received failure at interface level so call
            * will be cleared forcefully */            
            ret_val = ICF_FAILURE;
        }        
        /* Ceck If call has been connected send bye to remote */
        else if (ICF_NULL != ICF_CC_CHECK_COMMON_BITMASK(p_cc_pdb,
            ICF_CALL_CONNECT_DONE))
        {
            /* If signaling ongoing is false we can send bye to remote else
            * wait for start proc signalling */
            if(ICF_INT_MODULE_INVALID == p_cc_pdb->p_call_ctx->signalling_ongoing)
              /*(ICF_INT_MODULE_CC == p_cc_pdb->p_call_ctx->signalling_ongoing))*/
            {
                /* Send call clear request to the SSA */
                if(ICF_FAILURE ==icf_cc_send_ssa_call_term_req(p_cc_pdb))
                {
                    /* Return error to top level function, 
                     * call will be forcefully cleared*/
                    ret_val = ICF_FAILURE;
                }
                else
                {
                    /* Update the next state in the call context*/
                    ICF_CC_SET_NEXT_STATE(p_cc_pdb,
                        ICF_CC_08_AWT_CALL_TERMINATE_RESP)                        
                }
            }
            else /* We cannot send bye for now. wait for start proc signaling */
            {
                /*Return service terminate status to CM*/
                ICF_CC_SET_STATUS(p_cc_pdb, ICF_STATUS_CALL_R_SRV_TERMINATE)
                    
                /* Mark next state as ICF_CC_12_WAIT_START_PROC_SIG */
                ICF_CC_SET_NEXT_STATE(p_cc_pdb, ICF_CC_12_WAIT_START_PROC_SIG)
            }
        }        
        /* Call has not been answered, if outgoing call send cancle req to SSA */
        else if (ICF_NULL == ICF_CC_CHECK_COMMON_BITMASK(p_cc_pdb,
            ICF_CALL_DIR))
        {
            if(ICF_FAILURE == icf_cc_send_ssa_cancel_req(p_cc_pdb))
            {
            /* Return error to top level function,
                * call will be forcefully cleared*/
                ret_val = ICF_FAILURE;
            }
        }        
        else /* This is a  INCOMING_CALL */
        {
            next_state = ICF_CC_GET_NEXT_STATE(p_cc_pdb);
            /* For a incoming call we need to send reject request  
             * If next state was 9 or 10( we are waiting for prack from remote),
             *  set call terminate as pending event.*/
            if(ICF_CC_10_AWT_ALERT_PRACK == next_state ||
                ICF_CC_09_AWT_MEDIA_PARAM_PRACK == next_state)
            {
                /* Set call terminate as pending event */
                ICF_CC_SET_PENDING_EVENT(p_cc_pdb, ICF_PENDING_EVENT_MM_ERR_IND)
                    
                /* Set next state to call state as call terminate will be 
                 * processed on getting PRACK from remote*/                    
                ICF_CC_SET_CALL_STATE(p_cc_pdb, ICF_CC_GET_NEXT_STATE(p_cc_pdb))
            }/* As prack is not awaited, send reject to remote party */            
            else if(ICF_FAILURE == icf_cc_send_ssa_reject(p_cc_pdb))
            {
                /* Return error to top level function,
                 * call will be forcefully cleared*/
                ret_val = ICF_FAILURE;
            }            
        }
        
        /* if MM ERROR INDICATION is not pending event send DMS to MMI */
        if (ICF_NULL == ICF_CC_CHECK_PENDING_EVENT(p_cc_pdb, 
            ICF_PENDING_EVENT_MM_ERR_IND) && ICF_SUCCESS == ret_val) 
        {
            /* If mmi has send mm_err_ind with reason as ICF_ECODE_DMS_IND_RECVD */
            if(ICF_ECODE_DMS_IND_RECVD != media_error)
            {                
                if(ICF_FAILURE == icf_cc_send_dms_req(p_cc_pdb))
                {
                    /* Return error to top level function,
                    * call will be forcefully cleared */
                    ret_val = ICF_FAILURE;
                }
                else
                {
                    /* Update the states in the call context */
                    ICF_CC_SET_CALL_STATE(p_cc_pdb,ICF_CC_07_AWT_DMS_RESP)                        
                }
            }
            /* MM Error Ind was due to DMS Indication from MM*/
            else
            {
                /*
                *  We need not send Dms in this case.
                *  We will send CAll terminate to PA if pa initated bit is not send
                *  else send call terminateresp to PA
                *  and set mm cleared status
                */
                
                /* Set MM cleared bit in call_clear_status */
                ICF_CC_SET_CALL_CLR_STATUS(p_cc_pdb, ICF_CALL_CLEAR_STATUS_MM_CLEARED)
                    
                ICF_CC_TRACE(p_cc_pdb, ICF_TRACE_CALL_STATUS_MM_CLR)
                    
                if (ICF_CC_12_WAIT_START_PROC_SIG ==
                    ICF_CC_GET_NEXT_STATE(p_cc_pdb))
                {
                    /* Set MM cleared bit in call_clear_status */                    
                    /* BYE has not yet been sent since signaling ongoing was true when
                    * termination was initiated, so go to wait for signal proc now*/
                    ICF_CC_SET_CALL_STATE(p_cc_pdb,
                        ICF_CC_12_WAIT_START_PROC_SIG)                        
                }
                /* PA intitiated release type not set */
                else if (ICF_NULL == ICF_CC_CHECK_RELEASE_TYPE(p_cc_pdb,
                    ICF_REL_TYPE_PA_INITIATED))
                {
                    /* Not PA initiated, send Call Terminate Request to PA */
                    if (ICF_FAILURE == icf_cc_send_pa_call_term_ind(p_cc_pdb))
                    {
                        ret_val = ICF_SUCCESS;
                    }
                    else
                    {
                        /* Go to state Awaiting Call Terminate Response */
                        ICF_CC_SET_CALL_STATE(p_cc_pdb,
                            ICF_CC_08_AWT_CALL_TERMINATE_RESP)
                    }
                }
            }/* Handling mm err ind due to DMS IND */
            
        }/* Clearing call and call terminate is not pending */        
         /* PA initiated release type set */
        else if(ICF_SUCCESS == ret_val)
        {
            /* Send Call Terminate Response to PA */
            ret_val = icf_cc_send_pa_call_term_resp(p_cc_pdb);
            if (ICF_FAILURE == ret_val)
            {
                ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */
                    return ret_val;
            }
            
            /* Set PA cleared bit */
            ICF_CC_SET_CALL_CLR_STATUS(p_cc_pdb, ICF_CALL_CLEAR_STATUS_PA_CLEARED)
            
            ICF_CC_TRACE(p_cc_pdb, ICF_TRACE_CALL_STATUS_PA_CLR)                
        }/*Handeled PA initated release */
   }
	
   ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */
   return ret_val;
}



/******************************************************************************
 *
 * FUNCTION:        icf_cc_04_timer_expiry
 *
 * DESCRIPTION:     This function is invoked when the CC is awaiting Modify
 *                  Media Session Response from MMI and setup/alert/modify
 *                  Timer expires.
 *                  Current State: ICF_CC_04_AWT_MMS_RESP
 *                  Current Event: ICF_CC_TIMER_EXPIRY
 *
 *****************************************************************************/
icf_return_t icf_cc_04_timer_expiry(icf_cc_pdb_st *p_cc_pdb)
{
    icf_return_t     ret_val = ICF_SUCCESS;

    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /* function entry trace */

	/* check if timer type is INFO , do not release the call */
    if(ICF_TIMER_TYPE_INFO == p_cc_pdb->timer_type)
    {
         p_cc_pdb->p_call_ctx->app_failure_cause = ICF_CAUSE_TIMER_EXPIRY;
		 ret_val = icf_cc_send_pa_info_resp(p_cc_pdb);
        
		 p_cc_pdb->p_call_ctx->pending_info_resp  = ICF_FALSE; 

        /* Reset the failure cause to ICF_NULL*/
        p_cc_pdb->p_call_ctx->app_failure_cause = ICF_NULL;

		ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */
		return ret_val;
    }

    /* Set release_type as timer expiry */
    ICF_CC_SET_RELEASE_TYPE(p_cc_pdb, ICF_REL_TYPE_TIMER_EXPIRY)
    ICF_CC_TRACE(p_cc_pdb, ICF_TRACE_REL_TYPE_TIMER_INIT)

    
    /* if timer type is setup or alerting set the stats */
    if(ICF_TIMER_TYPE_ALERTING == p_cc_pdb->timer_type||
            ICF_TIMER_TYPE_SETUP == p_cc_pdb->timer_type)
    {
         ICF_CC_SET_RELEASE_CAUSE(p_cc_pdb, 
                 ICF_CALL_CLEAR_REASON_RESOURCES_NOT_AVAILABLE)
    }
    else
    {
        /*
         * Send error response to SSA (SIP 500). For this, first fill
         * appropriate p_cc_pdb->p_call_ctx->release_cause that would be used by
         * SSA to decide the exact message (500 in this case) to be sent to
         * peer.
         */
        ICF_CC_SET_RELEASE_CAUSE(p_cc_pdb, ICF_CALL_CLEAR_REASON_NOANSWER)

    }


    if(ICF_FAILURE == icf_cc_start_release_timer(p_cc_pdb))
    {
        /*return failure to top level function */
        ret_val = ICF_FAILURE;
    }

    /* If update or prack is pending, send ack for the same */
    else if(ICF_FAILURE == icf_cc_clear_pending_events(p_cc_pdb))
    {
        /*return failure to top level function */
        ret_val = ICF_FAILURE;
    }

    /*
     * Send BYE to network if call is connected and signaling ongoing is FALSE
     */
    if (ICF_NULL != ICF_CC_CHECK_COMMON_BITMASK(p_cc_pdb,
                    ICF_CALL_CONNECT_DONE))
    {
        /*Call is connected. CHheck signalling status */
        /*Fixed By Aman- SPR 14019- 27.12.2006*/
         if(ICF_INT_MODULE_INVALID == p_cc_pdb->p_call_ctx->signalling_ongoing)
         {
                /*
                 * Send call clear request to the SSA
                 */
                if(ICF_FAILURE == icf_cc_send_ssa_call_term_req(p_cc_pdb))
                {
                    ret_val = ICF_FAILURE;
                }
                else
                {

                /*
                 * Succsfully send bye to remote
                 * Update the next state in the call context
                 */
                    ICF_CC_SET_NEXT_STATE(p_cc_pdb,
                            ICF_CC_08_AWT_CALL_TERMINATE_RESP)
                }
         }
         else
         {
                /*Return service terminate status to CM*/
                ICF_CC_SET_STATUS(p_cc_pdb,
                        ICF_STATUS_CALL_R_SRV_TERMINATE)
                /*
                 * Mark next state as ICF_CC_12_WAIT_START_PROC_SIG
                 */
                ICF_CC_SET_NEXT_STATE(p_cc_pdb,ICF_CC_12_WAIT_START_PROC_SIG)
        }
    }
    /*
     * Call is not connected. so we cant send bye.
     * Check if cancel or reject need to be sent
     */
    else if (ICF_NULL == ICF_CC_CHECK_COMMON_BITMASK(p_cc_pdb,
                        ICF_CALL_DIR))
    {
       /* This is a outgoing call so send cancel req to remote*/
       ret_val = icf_cc_send_ssa_cancel_req(p_cc_pdb);
    }
    else if (ICF_NULL != ICF_CC_CHECK_COMMON_BITMASK(p_cc_pdb,
                        ICF_CALL_DIR))
    {
        /*
         *  This is a incoming call, so send call reject to remote
         *  */
        ret_val = icf_cc_send_ssa_reject(p_cc_pdb);

    }


    /*
     * Send a DMS request to MMI with mode as non forceful
     */
    if(ICF_SUCCESS == ret_val)
    {
        p_cc_pdb->forceful_delete = ICF_FALSE;
        if(ICF_FAILURE == icf_cc_send_dms_req(p_cc_pdb))
        {
            ret_val = ICF_FAILURE;
        }
        else
        {
            ICF_CC_SET_CALL_STATE(p_cc_pdb,ICF_CC_07_AWT_DMS_RESP)
        }
    }


    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */
    return ret_val;
}

/******************************************************************************
 *
 * FUNCTION:        icf_cc_04_pa_call_connect
 *
 * DESCRIPTION:     This function is invoked when the CC is awaiting Modify
 *                  Media Session Response from MMI and Connect is recv from PA
 *                  Current State: ICF_CC_04_AWT_MMS_RESP
 *                  Current Event: ICF_CC_PA_CALL_CONNECT
 *
 *****************************************************************************/
icf_return_t icf_cc_04_pa_call_connect(icf_cc_pdb_st *p_cc_pdb)
{
    icf_return_t     ret_val = ICF_SUCCESS;
	icf_pa_cc_connect_req_st			*p_connect = ICF_NULL;

    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /* function entry trace */

	p_connect = (icf_pa_cc_connect_req_st*)
						p_cc_pdb->p_glb_pdb->p_recv_msg->payload;

	/* copy the additional parameters from payload into call ctx */
    
    /*REL6.0 Design: Update function to copy rejected stream list in the payload
     * to call_context's p_rejected_list*/
    if (ICF_FAILURE == icf_cc_proc_pa_connect_payload(p_cc_pdb))
    {
        ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb)
        return ret_val;
    }

     /* set call state as await connect ack */
    ICF_CC_SET_NEXT_STATE(p_cc_pdb,ICF_CC_11_AWT_SSA_CONNECT_ACK)

     ret_val = icf_cc_reset_media_pending_events(p_cc_pdb);        
    ICF_CC_SET_PENDING_EVENT(p_cc_pdb,
                ICF_PENDING_EVENT_PA_CALL_CONNECT)

    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */

    return ret_val;
}

/******************************************************************************
 *
 * FUNCTION:        icf_cc_04_alert_prack
 *
 * DESCRIPTION:     This function is invoked when the CC is awaiting Modify
 *                  Media Session Response from MMI and PRACK for 180 is
 *                  received
 *                  Current State: ICF_CC_04_AWT_MMS_RESP
 *                  Current Event: ICF_CC_SSA_ALERT_PRACK
 *
 *****************************************************************************/
icf_return_t icf_cc_04_alert_prack(icf_cc_pdb_st *p_cc_pdb)
{
    icf_return_t     ret_val = ICF_SUCCESS;
    icf_nw_inc_prack_st *p_alert_prack = ICF_NULL;
    icf_boolean_t    sdp_in_prack = ICF_FALSE;

    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /* function entry trace */

    ICF_ASSERT(p_cc_pdb->p_glb_pdb, p_cc_pdb->p_internal_msg->p_msg_data)

    p_alert_prack = (icf_nw_inc_prack_st *) 
                         p_cc_pdb->p_internal_msg->p_msg_data;

    if(p_alert_prack->bitmask & ICF_NW_INC_PRACK_SDP)
    {        
        sdp_in_prack = ICF_TRUE;
        icf_cc_delete_inc_media_sdp_info_list(p_cc_pdb,
                              &p_alert_prack->p_sdp_list);
    }

    /* If PA call terminate was pending event we need to clear the call*/
    if(ICF_NULL != ICF_CC_CHECK_PENDING_EVENT(p_cc_pdb,
                    ICF_PENDING_EVENT_PA_CALL_TERM_REQ))
    {
        ICF_CC_SET_RELEASE_TYPE(p_cc_pdb, 
                ICF_REL_TYPE_PA_INITIATED)

        ICF_CC_TRACE(p_cc_pdb, ICF_REL_TYPE_PA_INITIATED)

        p_cc_pdb->p_call_ctx->offer_failure_reason = ICF_SA_SERVER_INTERNAL_ERROR;

        /* send ack for call terminate and initate call clear */
        if(ICF_FAILURE == 
                icf_cc_send_ssa_alert_prack_err(p_cc_pdb))

        {
            /* set return value as failure. call will be cleared forcefully*/
            ret_val = ICF_FAILURE;
        }
        else if(ICF_FAILURE == icf_cc_start_release_timer(p_cc_pdb))
        {
            /* set return value as failure. call will be cleared forcefully*/
            ret_val = ICF_FAILURE;
        }
        else if(ICF_FAILURE == icf_cc_send_ssa_reject(p_cc_pdb))
        {
            /* set return value as failure. call will be cleared forcefully*/
            ret_val = ICF_FAILURE;
        }
        else if(ICF_FAILURE == icf_cc_send_dms_req(p_cc_pdb))
        {
            /* set return value as failure. call will be cleared forcefully*/
            ret_val = ICF_FAILURE;
        }
        else
        {
            /* Set call state to await DMS */
            ICF_CC_SET_CALL_STATE(p_cc_pdb,ICF_CC_07_AWT_DMS_RESP)
        }


    } /* if Call  cleared due to pending events */    
	/* Check if PRACK with SDP is received, then send failure as
     * as offer_change has been received from peer and MMS response
     *for it is pending*/
    else if (ICF_TRUE == sdp_in_prack)
    {
        /* The only scenario wherein we could get alert in state 4*/
        p_cc_pdb->p_call_ctx->offer_failure_reason = ICF_REQUEST_PENDING;
        ret_val = icf_cc_send_ssa_alert_prack_err(p_cc_pdb);
    }
    /* prack can be handled succefully*/
    else
    {        
        ret_val = icf_cc_send_ssa_alert_prov_resp_ack(p_cc_pdb);
        
        /* set call state as await connect ack */
        ICF_CC_SET_NEXT_STATE(p_cc_pdb,
            ICF_CC_05_AWT_CONNECT)
    }
    
    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */
    return ret_val;
}

/******************************************************************************
 *
 * FUNCTION:        icf_cc_04_ssa_connect_ack
 *
 * DESCRIPTION:     This function is invoked when the CC is awaiting Modify
 *                  Media Session Response from MMI and Setup/Alert Timer
 *                  expires.
 *                  Current State: ICF_CC_04_AWT_MMS_RESP
 *                  Current Event: ICF_CC_SSA_CONNECT_ACK
 *
 *****************************************************************************/
icf_return_t icf_cc_04_ssa_connect_ack(icf_cc_pdb_st *p_cc_pdb)
{
    icf_return_t                ret_val = ICF_SUCCESS;
    icf_nw_inc_connect_ack_st   *p_connect_ack = ICF_NULL;

    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /* function entry trace */

    icf_cc_stop_current_timer(p_cc_pdb);

    p_connect_ack = (icf_nw_inc_connect_ack_st*)
        p_cc_pdb->p_internal_msg->p_msg_data;

    /* Call type is fast start, no need to process sdp, set signalling 
    * ongoing flag  */
    if(p_connect_ack->bitmask & ICF_NW_INC_CONNECT_ACK_SDP)
    {
        /* SDP is present in connect ack */
        icf_cc_delete_inc_media_sdp_info_list(p_cc_pdb,
             &p_connect_ack->p_sdp_list);
        
    }

    
    /* Return status as signalling done to CM */
    ICF_CC_SET_STATUS(p_cc_pdb, ICF_STATUS_SIGNAL_ONGOING_ENDED)
        
    /* Reset signalling ongoing flag */
    ICF_CC_RESET_SIG_ONGOING(p_cc_pdb)
        
    /* Set call connect done */
    ICF_CC_SET_COMMON_BITMASK(p_cc_pdb,ICF_CALL_CONNECT_DONE)        
        
    if(ICF_CC_11_AWT_SSA_CONNECT_ACK == ICF_CC_GET_NEXT_STATE(p_cc_pdb))
    {
        ICF_CC_SET_NEXT_STATE(p_cc_pdb,ICF_CC_06_CONNECTED)
    }    

    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */
    return ret_val;
}

/******************************************************************************
 *
 * FUNCTION:        icf_cc_04_ssa_call_cancel_req
 *
 * DESCRIPTION:     This function is invoked when the CC is awaiting Modify
 *                  Media Session Response from MMI and remote sends Cancel
 *                  Current State: ICF_CC_04_AWT_MMS_RESP
 *                  Current Event: ICF_CC_SSA_CALL_CANCEL_REQ
 *
 *****************************************************************************/
icf_return_t icf_cc_04_ssa_call_cancel_req(icf_cc_pdb_st *p_cc_pdb)
{

    icf_return_t     ret_val = ICF_SUCCESS;
    icf_uint8_t      ongoing_event = ICF_NULL;

    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /* function entry trace */

    /*Get ongoing event*/
    ongoing_event =  ICF_CC_GET_ONGOING_EVENT(p_cc_pdb);
    p_cc_pdb->p_call_ctx->offer_failure_reason = ICF_CALL_CLEAR_MEDIA_MISMATCH;

    if(ICF_CC_ALERT_PRACK == ongoing_event)
    {
        /* Send Ack for provisional response recd */
        ret_val = icf_cc_send_ssa_alert_prack_err(p_cc_pdb);
    }
    else if(ICF_CC_MEDIA_PARAM_PRACK == ongoing_event)
    {
        /* Send Ack for provisional response recd */
        ret_val = icf_cc_send_ssa_media_prack_err(p_cc_pdb);
    }
    else if(ICF_CC_SSA_OFFER_CHANGE == ongoing_event)
    {
ICF_HANDLE_REINVITE_START
/*cancel_reinvite*/

    if (ICF_NULL != ICF_CC_CHECK_COMMON_BITMASK(p_cc_pdb,
                    ICF_CALL_CONNECT_DONE))
    {
        ICF_CC_SET_PENDING_EVENT(p_cc_pdb,
               ICF_PENDING_EVENT_SSA_CALL_MOD_CANCEL_REQ)
    }
    else   
    {
       ret_val = icf_cc_send_ssa_offer_chg_err(p_cc_pdb);
       if(ICF_SUCCESS == ret_val)
       {
           ret_val = icf_cc_01_ssa_call_cancel_req(p_cc_pdb);
           ICF_CC_RESET_CALL_CLR_STATUS(p_cc_pdb,
            ICF_CALL_CLEAR_STATUS_PA_CLEARED)

       }
    }

        ret_val = icf_cc_send_ssa_cancel_resp(p_cc_pdb);

        return ret_val;

ICF_HANDLE_REINVITE_END
    }
    else if(ICF_CC_PA_CALL_MODIFY_RSP == ongoing_event ||
       ICF_CC_ONGOING_EVENT_REMOTE_MEDIA_REVERT == ongoing_event)
    {
       ICF_CC_SET_PENDING_EVENT(p_cc_pdb,
             ICF_PENDING_EVENT_SSA_CALL_MOD_CANCEL_REQ)

       ret_val = icf_cc_send_ssa_cancel_resp(p_cc_pdb);
       return ret_val;
    }

    if(ICF_SUCCESS == ret_val)
    {
        ret_val = icf_cc_01_ssa_call_cancel_req(p_cc_pdb);
		/*
		* Hadling of cancel request in state 1 set PA cleared bit for incomming call. 
		* Since we are call the same handler in state 4 so we should reset this bit 
		* as the PA interface would also need to be cleared 
		* So Reset PA cleared bit 
		*/
		ICF_CC_RESET_CALL_CLR_STATUS(p_cc_pdb,
            ICF_CALL_CLEAR_STATUS_PA_CLEARED)
    }
    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */
    return ret_val;

}


/******************************************************************************
 *
 * FUNCTION:        icf_cc_04_generic_err
 *
 * DESCRIPTION:     This function is invoked when the CC is awaiting
 *                  create media session resp from MMI and it receives
 *                  an error indication from MMI..
 *                  Current State: ICF_CC_04_AWT_MMS_RESP
 *                  Current Event: ICF_CC_GENERIC_ERROR
 *
 *****************************************************************************/

icf_return_t icf_cc_04_generic_err (
        INOUT icf_cc_pdb_st *p_cc_pdb)
{
    icf_return_t     ret_val = ICF_SUCCESS;

    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /* function entry trace */
   
    /* Set release type as internal error */
    ICF_CC_SET_RELEASE_TYPE(p_cc_pdb, ICF_REL_TYPE_INTERNAL_INITIATED)
    ICF_CC_TRACE(p_cc_pdb, ICF_TRACE_REL_TYPE_INT_ERR) 

    /*Start the release timer */
    if(ICF_FAILURE == icf_cc_start_release_timer(p_cc_pdb))
    {
        /*return failure to top level function */
        ret_val = ICF_FAILURE;
    }

    /* If update or prack is pending, send ack for the same */
    else if(ICF_FAILURE == icf_cc_clear_pending_events(p_cc_pdb))
    {
        /*return failure to top level function */
        ret_val = ICF_FAILURE;
    }

    /*
     * Send BYE to network if call is connected and signaling ongoing is FALSE
     */
    if (ICF_NULL != ICF_CC_CHECK_COMMON_BITMASK(p_cc_pdb,
                    ICF_CALL_CONNECT_DONE))
    {
        /*Call is connected. CHheck signalling status */
         if(ICF_INT_MODULE_INVALID == p_cc_pdb->p_call_ctx->signalling_ongoing)
         {
                /*
                 * Send call clear request to the SSA
                 */
                if(ICF_FAILURE == icf_cc_send_ssa_call_term_req(p_cc_pdb))
                {
                    ret_val = ICF_FAILURE;
                }
                else
                {
                /*
                 * Succsfully send bye to remote
                 * Update the next state in the call context
                 */
                    ICF_CC_SET_NEXT_STATE(p_cc_pdb,
                            ICF_CC_08_AWT_CALL_TERMINATE_RESP)
                }
         }
         else
         {
                /*Return service terminate status to CM*/
                ICF_CC_SET_STATUS(p_cc_pdb,
                        ICF_STATUS_CALL_R_SRV_TERMINATE)
                /*
                 * Mark next state as ICF_CC_12_WAIT_START_PROC_SIG
                 */
                ICF_CC_SET_NEXT_STATE(p_cc_pdb,ICF_CC_12_WAIT_START_PROC_SIG)
        }
    }
    /*
     * Call is not connected. so we cant send bye.
     * Check if cancel or reject need to be sent
     */
    else if (ICF_NULL == ICF_CC_CHECK_COMMON_BITMASK(p_cc_pdb,
                        ICF_CALL_DIR))
    {
       /* This is a outgoing call so send cancel req to remote*/
       ret_val = icf_cc_send_ssa_cancel_req(p_cc_pdb);
    }
    else if (ICF_NULL != ICF_CC_CHECK_COMMON_BITMASK(p_cc_pdb,
                        ICF_CALL_DIR))
    {
        if(ICF_NULL ==  p_cc_pdb->p_call_ctx->release_cause)
        {
        ICF_CC_SET_RELEASE_CAUSE(p_cc_pdb, 
                ICF_SA_SERVER_INTERNAL_ERROR)
        }
        
        /*p_cc_pdb->p_call_ctx->release_cause = 
                ICF_CALL_CLEAR_REASON_REQUEST_TERMINATED;*/

        /*
         *  This is a incoming call, so send call reject to remote
         *  */
        ret_val = icf_cc_send_ssa_reject(p_cc_pdb);
        /* SPR 17426:  Set network clear bit as the ACK for 487 will
         * not be received at CC. SSA will clear the contexts after receiving ACK
         * for 487.
         */
        ICF_CC_SET_CALL_CLR_STATUS(p_cc_pdb, 
                ICF_CALL_CLEAR_STATUS_NW_CLEARED)
        ICF_CC_TRACE(p_cc_pdb, ICF_TRACE_CALL_STATUS_NW_CLR)
    }
    /*
     * Send a DMS request to MMI with mode as non forceful
     */
    if(ICF_SUCCESS == ret_val)
    {
        p_cc_pdb->forceful_delete = ICF_FALSE;
        if(ICF_FAILURE == icf_cc_send_dms_req(p_cc_pdb))
        {
            ret_val = ICF_FAILURE;
        }
        else
        {
            /* SPR 17426: Set call state to 07 instead of next call state.
             */
            ICF_CC_SET_CALL_STATE(p_cc_pdb,ICF_CC_07_AWT_DMS_RESP)
            ICF_CC_SET_NEXT_STATE(p_cc_pdb,ICF_CC_07_AWT_DMS_RESP)
        }
    }


    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */
    return ret_val;
}

#ifdef ICF_QOS_SUPPORT
/******************************************************************************
 *
 * FUNCTION:        icf_cc_04_media_prack_resp
 *
 * DESCRIPTION:     This function is invoked when the CC is awaiting Modify
 *                  Media Session Response from MMI and remote sends 
 *                  response for PRACK of 183.
 *                  Current State: ICF_CC_04_AWT_MMS_RESP
 *                  Current Event: ICF_CC_MEDIA_PARAM_PRACK_RESP
 *
 *****************************************************************************/
icf_return_t icf_cc_04_media_prack_resp(icf_cc_pdb_st *p_cc_pdb)
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
ICF_CHECK_QOS_SUPPORT_START(p_cc_pdb->p_glb_pdb, ICF_TRUE)

		/* check if mms_resp has already been received */
		if(p_cc_pdb->p_call_ctx->media_committed == ICF_TRUE)
		{
			/* Send the UPDATE request to peer */
			if(ICF_SUCCESS == 
                        icf_cc_match_curr_des_status_on_local_nw(p_cc_pdb))
            {
				/* We will send UPDATE request to Peer */
                ret_val = icf_cc_ssa_send_update_msg(p_cc_pdb);
            }
		}
ICF_CHECK_QOS_SUPPORT_END
    }
    else
    {
        /*  Fix for CSR 1-7507275 : SPR 19881 */
        /* Set call clear reason in call context */
        p_cc_pdb->p_call_ctx->release_cause = (icf_uint8_t)p_prack_resp->error_cause;

        /* Clear the call */
        ret_val = icf_cc_04_generic_err(p_cc_pdb);

    }
    return ret_val;
}
#endif


/******************************************************************************
 *
 * FUNCTION:        icf_cc_04_forceful_call_clear
 *
 * DESCRIPTION:     This function is invoked when Forceful call clear is 
 *                  received in state ICF_CC_04_AWT_MMS_RESP 
 *                  Current State: ICF_CC_04_AWT_MMS_RESP
 *                  Current Event: ICF_CC_FORCEFUL_CLEAR
 *
 *****************************************************************************/
icf_return_t icf_cc_04_forceful_call_clear(
        INOUT icf_cc_pdb_st *p_cc_pdb)
{
    icf_return_t ret_val = ICF_SUCCESS;

    ICF_CC_SET_RELEASE_CAUSE(p_cc_pdb,ICF_REL_CAUSE_SERV_INT_ERR)

    if (ICF_NULL != ICF_CC_CHECK_COMMON_BITMASK(p_cc_pdb,
                    ICF_CALL_CONNECT_DONE))
    {
        /*
         * Send call clear request to the SSA
         */
        icf_cc_send_ssa_call_term_req(p_cc_pdb);
    }
    else
    {
        /*
         * Send call clear request to the SSA
         */
        icf_cc_send_clear_req_to_ssa(p_cc_pdb);
    } /* if (ICF_FAILURE == icf_c... */
 
    /*Handling would be same as that of forceful call clear in atate IDLE*/
    icf_cc_00_clear_call (p_cc_pdb);

    return ret_val;
    
}


/******************************************************************************
 *
 * FUNCTION:        icf_cc_04_pa_info_req 
 *
 * DESCRIPTION:     This function is invoked when the CC is awaiting MMS 
 *                  response from MMI and it receives info request from PA
 *                  Current State: ICF_CC_04_AWT_MMS_RESP
 *                  Current Event: ICF_CC_PA_INFO_REQ
 * 
 *****************************************************************************/
icf_return_t icf_cc_04_pa_info_req(
        INOUT icf_cc_pdb_st *p_cc_pdb)
{
    icf_return_t ret_val = ICF_SUCCESS;

    if(ICF_FAILURE == icf_cc_validate_info_req(p_cc_pdb))
    {
        p_cc_pdb->p_call_ctx->app_failure_cause = ICF_CAUSE_INVALID_ACTION;
        ret_val = icf_cc_send_pa_info_resp(p_cc_pdb);
        
        /* Reset the failure cause to ICF_NULL*/
        p_cc_pdb->p_call_ctx->app_failure_cause = ICF_NULL;
    }
    else if(ICF_TRUE == p_cc_pdb->p_call_ctx->pending_info_resp)
    {
        p_cc_pdb->p_call_ctx->app_failure_cause = ICF_CAUSE_INFO_ALREADY_PENDING;
        ret_val = icf_cc_send_pa_info_resp(p_cc_pdb);       
       
        /* Reset the failure cause to ICF_NULL*/
        p_cc_pdb->p_call_ctx->app_failure_cause = ICF_NULL;
 
    }
    /* If this is an outgoing call or call is connected*/
    else if(ICF_NULL == ICF_CC_CHECK_COMMON_BITMASK(p_cc_pdb,
                 ICF_CALL_DIR) ||
            ICF_NULL != ICF_CC_CHECK_COMMON_BITMASK(p_cc_pdb,
                    ICF_CALL_CONNECT_DONE))
    {
        if(ICF_FAILURE == icf_cc_start_info_timer(p_cc_pdb))
        {
            p_cc_pdb->p_call_ctx->app_failure_cause = ICF_ERROR_TMR_START_FAILURE;
            ret_val = ICF_FAILURE;
        }
        else if(ICF_FAILURE == icf_cc_send_ssa_info_req(p_cc_pdb))
        {
            p_cc_pdb->p_call_ctx->app_failure_cause = ICF_CAUSE_NETWORK_ERROR;
            ret_val = ICF_FAILURE;
        }

        /* If there was a failure in sending info request on network then  
         * send info failure response to PA*/
        if(ICF_FAILURE == ret_val)
        {
            /* 
             * Reset ret_val to SUCCESS as we would indicate info failure 
             * upwards
             */
            ret_val = ICF_SUCCESS;  
            ret_val = icf_cc_send_pa_info_resp(p_cc_pdb);
        }
        /* 
         * If info was successfully sent on network the set the pending info 
         * flag
         */
        else
        {
            p_cc_pdb->p_call_ctx->pending_info_resp  = ICF_TRUE;
        }
    }/* End else if(ICF_NULL == ICF_CC ... */
    /* If this was not an outgoing call or was not connected */
    else
    {
        p_cc_pdb->p_call_ctx->app_failure_cause = ICF_CAUSE_INVALID_ACTION;
        ret_val = icf_cc_send_pa_info_resp(p_cc_pdb);       
    }

    return ret_val;
}

/*******************************************************************************
 *
 * FUNCTION:        icf_cc_04_pa_call_modify_req
 *
 * DESCRIPTION:     This function is invoked when the CC is awaiting
 *                  Modify Media Session Response and offer change
 *                  is recvd from the PA
 *                  (SIP UPDATE/RE-INVITE received from peer).
 *                  Current State: ICF_CC_04_AWT_MMS_RESP
 *                  Current Event: ICF_CC_PA_CALL_MODIFY_REQ
 *
 ******************************************************************************/
icf_return_t icf_cc_04_pa_call_modify_req(INOUT icf_cc_pdb_st *p_cc_pdb)
{
    icf_return_t     ret_val = ICF_SUCCESS;    
    icf_config_data_st  *p_config_data = ICF_NULL;
    icf_call_modify_req_st  *p_call_modify_req = ICF_NULL;

    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /* function entry trace */
   
    p_call_modify_req = 
      (icf_call_modify_req_st*)(p_cc_pdb->p_glb_pdb->p_recv_msg->payload);

    /* Get pointer to config data from DBM */
    ret_val = icf_dbm_get_module_glb_data(
                p_cc_pdb->p_glb_pdb,
                (icf_int_module_id_t)ICF_INT_MODULE_CFG,
                (icf_void_t *)&p_config_data, p_cc_pdb->p_ecode);

    ICF_ASSERT(p_cc_pdb->p_glb_pdb, (ICF_NULL != p_config_data));

    /*If call is not connected and SDP in pa_call_mod_req present
     *then send failure to appllication*/

    /*twc_update handling*/
    if((ICF_CALL_MOD_REQ_MEDIA_MOD_LIST_PRESENT & p_call_modify_req->bit_mask ||
       ICF_CALL_MOD_REQ_DELETED_STREAM_PRESENT & p_call_modify_req->bit_mask))
    {
          /*Send PA Call Modify Req failure if it received in MMS response
           *state, as call connect done then MMS must only be due to
           *SSA Offer Change or PA Call Modify Req.
           */
           if(ICF_NULL != p_cc_pdb->p_call_ctx->fax_type)
           {
               p_cc_pdb->p_call_ctx->app_failure_cause = ICF_CAUSE_FAX_REQ_PENDING;
           }
           else
           {
               p_cc_pdb->p_call_ctx->app_failure_cause = ICF_CAUSE_CALL_MODIFY_REQ_PENDING;
           }

           ICF_CC_SET_COMMON_BITMASK(p_cc_pdb, ICF_CALL_MODIFY_RESP_FAIL)

          /* Send failure response */
         if(ICF_FAILURE == icf_cc_send_pa_call_modify_resp(p_cc_pdb))
        {
           /* This is error at interface level, call will be cleared
            * forcefully*/
            ret_val = ICF_FAILURE;
        }

	ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */
        return ICF_SUCCESS;
    }
    /*Send failure response to PA if remote does not supports UPDATE*/
    else if(ICF_UPDATE_PRESENT_IN_ALLOW_HDR != p_cc_pdb->p_call_ctx->allowed_value)
    {
         /* reject the request*/
         p_cc_pdb->p_call_ctx->app_failure_cause = ICF_CAUSE_INVALID_CALL_MODIFY_REQUEST;
         ICF_CC_SET_COMMON_BITMASK(p_cc_pdb, ICF_CALL_MODIFY_RESP_FAIL)

         /* Send failure response for update to user */
         if(ICF_FAILURE == icf_cc_send_pa_call_modify_resp(p_cc_pdb))
         {
             ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */
             return ICF_SUCCESS;
         }
    
    }
    else
    {
         /*This is the case when 183/180 with SDP is received but media is not committed  
          *and call modify wo SDP comes from PA.In this case send UPDATE wo SDP and remain
          *in the same state for MMS response*/

         /*Set common_bitmask as UPDATE_WO_SDP*/
         ICF_CC_SET_COMMON_BITMASK(p_cc_pdb, ICF_CALL_MOD_REQ_WO_SDP)
         /* clone the header list into call ctx, if received from app */
         if(ICF_FAILURE == icf_cmn_clone_header_list(
                                          p_cc_pdb->p_glb_pdb,
                                          &(p_call_modify_req->header_list),
                                          &(p_cc_pdb->p_call_ctx->header_list),
                                          p_cc_pdb->p_ecode))
         {
              ret_val = ICF_FAILURE;
         }
         /* clone the tag list into call ctx, if received from app */
         if ((ICF_SUCCESS == ret_val) &&
                 (ICF_TAG_LIST_PRESENT & p_call_modify_req->bit_mask))
         {
              ret_val = icf_cmn_clone_tag_list(
                                         p_cc_pdb->p_glb_pdb,
                                         &(p_call_modify_req->tag_list),
                                         &(p_cc_pdb->p_call_ctx->tag_list),
                                         p_cc_pdb->p_ecode);
         }

         /* clone the body list into call ctx, if received from app */
         if ((ICF_SUCCESS == ret_val) && 
                (ICF_BODY_LIST_PRESENT & p_call_modify_req->bit_mask))
         {
              ret_val = icf_cmn_clone_body_list(
                                         p_cc_pdb->p_glb_pdb,
                                         &(p_call_modify_req->body_list),
                                         &(p_cc_pdb->p_call_ctx->body_list),
                                         p_cc_pdb->p_ecode);
         }

         if (ICF_FAILURE == icf_cc_ssa_send_update_msg(p_cc_pdb))
         {
	    ret_val = ICF_FAILURE;
         }

    }

    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */
    return ret_val;
}

#ifdef ICF_START_FAX
/*******************************************************************************
 *
 * FUNCTION:        icf_cc_04_pa_start_fax_req
 *
 * DESCRIPTION:     This function is invoked when the CC is awaiting
 *                  Modify Media Session Response and start FAX request
 *                  received from application
 *                  Current State: ICF_CC_04_AWT_MMS_RESP
 *                  Current Event: ICF_CC_PA_START_FAX_REQ
 *
 ******************************************************************************/
icf_return_t icf_cc_04_pa_start_fax_req(INOUT icf_cc_pdb_st *p_cc_pdb)
{
	icf_return_t     ret_val = ICF_SUCCESS;    

    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /* function entry trace */    

    /* Send failure response for FAX request*/
    if(ICF_FAILURE == icf_cc_send_fax_failure_resp(p_cc_pdb))
    {
        /* This is error at interface level, call will be cleared
        * forcefully*/
        ret_val = ICF_FAILURE;
    }

	ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */
    return ret_val;
}
#endif

/*******************************************************************************
 *
 * FUNCTION:        icf_cc_04_ssa_stop_fax_req
 *
 * DESCRIPTION:     This function is invoked when the CC is awaiting
 *                  Modify Media Session Response and start FAX request
 *                  received from application
 *                  Current State: ICF_CC_04_AWT_MMS_RESP
 *                  Current Event: ICF_CC_SSA_STOP_FAX_REQ
 *
 ******************************************************************************/
icf_return_t icf_cc_04_ssa_stop_fax_req(INOUT icf_cc_pdb_st *p_cc_pdb)
{
	icf_return_t     ret_val = ICF_SUCCESS;    

    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /* function entry trace */    
    
    if (ICF_CC_PA_STOP_FAX_REQ == ICF_CC_GET_ONGOING_EVENT(p_cc_pdb))
    {
          /* if ongoing event is stop fax req from PA and remote also sends
           * stop fax req the 491 will be issued.
           */

          ICF_PRINT(((icf_uint8_t *)"Stop Fax Req from PA already going on"));
          p_cc_pdb->p_call_ctx->offer_failure_reason = ICF_REQUEST_PENDING;
          ret_val = icf_cc_send_ssa_offer_chg_err(p_cc_pdb);
    }

    else if(ICF_CC_SSA_FAX_REQ == ICF_CC_GET_ONGOING_EVENT(p_cc_pdb))
	{
		ICF_CC_SET_PENDING_EVENT(p_cc_pdb,ICF_PENDING_EVENT_SSA_STOP_FAX_REQ)
		if(ICF_FAILURE == icf_cc_send_ssa_cancel_resp(p_cc_pdb))
    		{
        		ret_val = ICF_FAILURE;
    		}
		ICF_CC_RESET_SIG_ONGOING(p_cc_pdb)
	}
	else if(ICF_CC_SSA_FAX_RESP == ICF_CC_GET_ONGOING_EVENT(p_cc_pdb))
	{
		ICF_CC_SET_PENDING_EVENT(p_cc_pdb,ICF_PENDING_EVENT_SSA_STOP_FAX_REQ)
	}
	else
	{
		/* do the connected fax call handling here */
	}

	ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */
    return ret_val;
}



/**************************************************************************
          State-04: Utility function for Ongoing_event handling
**************************************************************************/

/******************************************************************************
 *
 * FUNCTION:        icf_cc_mms_resp_ssa_alert_with_media
 *
 * DESCRIPTION:     This function is invoked when MMS response is received
 *                  and ongoing event is alert with media
 *
 *****************************************************************************/
icf_return_t icf_cc_mms_resp_ssa_alert_with_media(
	icf_cc_pdb_st      *p_cc_pdb)
{
	icf_return_t   ret_val = ICF_SUCCESS;
    
    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb)

    /*MMS response received for 180(SDP) means it is the first answer
     *from remote, if any stream is rejected then initiate call clear*/
	if ((ICF_CC_MMS_RESP_OFFER_REJECTED == p_cc_pdb->mms_resp_type) || 
        (ICF_CC_MMS_RESP_MOD_REJECTED == p_cc_pdb->mms_resp_type))
    {
		ret_val = icf_cc_initiate_call_clear(p_cc_pdb);
        ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) 
        return ret_val;       
    }

    if (ICF_SUCCESS == icf_cc_check_if_update_to_be_sent(p_cc_pdb))
    {
        p_cc_pdb->p_call_ctx->update_reqd = ICF_TRUE;
    }

    if (ICF_FAILURE == icf_cc_update_local_sdp(p_cc_pdb))
    {
        ret_val = ICF_FAILURE;
    }
    else
    {
        icf_cc_update_applied_sdp_mode(p_cc_pdb);

        /*If MMS response is success for all streams
         *send alert ind to application*/
		        /* Set media commited to true */
        p_cc_pdb->p_call_ctx->media_committed = ICF_TRUE;
  
        /* Return status as media commited  to CM */
        ICF_CC_SET_STATUS(p_cc_pdb, ICF_STATUS_MEDIA_COMMITTED)
 
        /* Set inband info flag as true as Alert was with sdp */
        p_cc_pdb->p_call_ctx->inband_info_present = ICF_TRUE;

        /*Fill all remote and local accepted and rejected streams*/
        if(ICF_FAILURE == icf_cc_send_pa_alert_ind(p_cc_pdb))
        {
            ret_val = ICF_FAILURE;
        }

		/* If pending list is not empty handle it */
		/* Possible pending events - 
		ICF_PENDING_EVENT_SSA_MEDIA_PARAM - in case of forking/multiple_answer
		ICF_PENDING_EVENT_SSA_ALERT_WITH_MEDIA - in case of forking/multiple answer
		ICF_PENDING_EVENT_SSA_CONNECT - in case of forking/multiple answer 
                */
		if(ICF_SUCCESS == ret_val &&
			ICF_NULL != ICF_CHECK_PENDING_LIST_IS_EMPTY(p_cc_pdb))
		{
			ret_val = icf_cc_mms_rsp_ssa_alert_with_media(p_cc_pdb);
		}
    }

    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb)

	return ret_val;
}


/******************************************************************************
 *
 * FUNCTION:        icf_cc_mms_resp_ssa_media_param
 *
 * DESCRIPTION:     This function is invoked when MMS response is received
 *                  and ongoing event is SSA media param
 *
 *****************************************************************************/
icf_return_t icf_cc_mms_resp_ssa_media_param(
	icf_cc_pdb_st      *p_cc_pdb)
{
	icf_return_t   ret_val = ICF_SUCCESS;

    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb)
    
    /*MMS response received for 180(SDP) means it is the first answer
    *from remote, if any stream is rejected then initiate call clear*/
    if ((ICF_CC_MMS_RESP_OFFER_REJECTED == p_cc_pdb->mms_resp_type) || 
        (ICF_CC_MMS_RESP_MOD_REJECTED == p_cc_pdb->mms_resp_type))
    {
        ret_val = icf_cc_initiate_call_clear(p_cc_pdb);
    }
    else
    {
       if (ICF_SUCCESS == icf_cc_check_if_update_to_be_sent(p_cc_pdb)) 
       {
            p_cc_pdb->p_call_ctx->update_reqd = ICF_TRUE;
       }

        icf_cc_update_applied_sdp_mode(p_cc_pdb);

        /* Set media commited to true */
        p_cc_pdb->p_call_ctx->media_committed = ICF_TRUE;
        
        /* Return status as media commited  to CM */
        ICF_CC_SET_STATUS(p_cc_pdb, ICF_STATUS_MEDIA_COMMITTED)

        ret_val = icf_cc_update_local_sdp(p_cc_pdb);
            
#ifdef ICF_QOS_SUPPORT       
ICF_CHECK_QOS_SUPPORT_START(p_cc_pdb->p_glb_pdb,ICF_TRUE)

            /*Fetch the internal sdp stream from list in call context
             * iterate thru the list and fill qos values in local sdp
             * of each stream*/
            icf_stream_sdp_info_st      *p_sdp = ICF_NULL;            
            icf_internal_list_st        *p_list = ICF_NULL;

            p_list = p_cc_pdb->p_call_ctx->p_stream_sdp_info;

            while (ICF_NULL != p_list)
            {       
                p_sdp = 
                    (icf_stream_sdp_info_st*)(p_list->p_payload);

				if(ICF_NULL == p_sdp->p_local_sdp)
				{
					p_list = p_list->p_next;
					continue;
				}
				/* we should update the local send/recv current status to res_reserved,
				 * since successful mms_resp implies successful sec_pdp_activation */
				p_sdp->p_local_sdp->local_send.current_status=
				p_sdp->p_local_sdp->local_recv.current_status=ICF_RESOURCE_RESERVED;

				/* go to next node in list */
                p_list = (icf_internal_list_st*)p_list->p_next;
            }


                
                /* Now we will be updating the Qos parameters in local SDP
                * structure by comparing it with Qos parameters in remote
                * received SDP
                */
                icf_cc_compare_and_update_qos_params(p_cc_pdb);
                

                /* Check Whether 200 OK for PRACK is received */
                if (ICF_FALSE == p_cc_pdb->p_call_ctx->
                        received_successful_prack_resp)
                {
                    ret_val = ICF_SUCCESS;
                }
                else if(ICF_SUCCESS == 
                        icf_cc_match_curr_des_status_on_local_nw(p_cc_pdb))
                {
                    /* We have to Send UPDATE request to Peer */
                    ret_val = icf_cc_ssa_send_update_msg(p_cc_pdb);
                }
				
ICF_CHECK_QOS_SUPPORT_END
#endif
        
        if (ICF_FAILURE == ret_val)
        {
            ret_val = ICF_FAILURE;
        }
        else 
        {
#ifdef ICF_QOS_SUPPORT
ICF_CHECK_QOS_SUPPORT_START(p_cc_pdb->p_glb_pdb,ICF_FALSE)
            if(ICF_FAILURE == icf_cc_send_pa_media_connect(p_cc_pdb))
            {
                ret_val = ICF_FAILURE;
            }
ICF_CHECK_QOS_SUPPORT_END
#else
            if(ICF_FAILURE == icf_cc_send_pa_media_connect(p_cc_pdb))
            {
                ret_val = ICF_FAILURE;
            }
#endif
			
			/* If pending list is not empty handle it */
			/* Possible pending events - 
			ICF_PENDING_EVENT_FMA_SSA_MEDIA_PARAM - in case of forking/multiple_answer
			ICF_PENDING_EVENT_FMA_SSA_ALERT_WITH_MEDIA - in case of forking/multiple answer
			ICF_PENDING_EVENT_FMA_SSA_CONNECT - in case of forking/multiple answer
                        */
 
			if(ICF_SUCCESS == ret_val &&
				ICF_NULL != ICF_CHECK_PENDING_LIST_IS_EMPTY(p_cc_pdb))
			{
				ret_val = icf_cc_mms_rsp_ssa_media_param(p_cc_pdb);
			}
        }
    }
    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb)
    return ret_val;
}


/******************************************************************************
 *
 * FUNCTION:        icf_cc_mms_resp_ssa_offer_change
 *
 * DESCRIPTION:     This function is invoked when MMS response is received
 *                  and ongoing event is SSA Offer Change
 *****************************************************************************/
icf_return_t icf_cc_mms_resp_ssa_offer_change(
	icf_cc_pdb_st      *p_cc_pdb)
{
    icf_return_t        ret_val = ICF_SUCCESS;

    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb)
    
    if(ICF_NULL == ICF_CC_CHECK_PENDING_EVENT(p_cc_pdb,
                          ICF_PENDING_EVENT_SSA_CALL_MOD_CANCEL_REQ))
    {
        /* Start : SPR14875 */ 
        if (ICF_NULL != ICF_CC_CHECK_COMMON_BITMASK(p_cc_pdb,
                                      ICF_CALL_CONNECT_DONE))
        {   
            icf_cc_stop_current_timer(p_cc_pdb);
        }
        /*If ADD new stream(T.38) is rejected then send failure response
         *for incoming offer change request*/
        if (ICF_CC_MMS_RESP_OFFER_REJECTED == p_cc_pdb->mms_resp_type)
        {
            p_cc_pdb->p_call_ctx->offer_failure_reason = 
                ICF_CALL_MODIFY_REASON_REJECT_MEDIA_CHANGE;

            /*reset the common bitmask if it is set as ICF_CALL_MOD_REQ_WO_SDP*/
            if(ICF_NULL != ICF_CC_CHECK_COMMON_BITMASK
                           (p_cc_pdb,ICF_CALL_MOD_REQ_WO_SDP))
             {
                  ICF_CC_RESET_COMMON_BITMASK(p_cc_pdb,
                                          ICF_CALL_MOD_REQ_WO_SDP)
             }

            ret_val = icf_cc_send_ssa_offer_chg_err(p_cc_pdb);
            icf_cc_stop_current_timer(p_cc_pdb);
        if (ICF_NULL != ICF_CC_CHECK_COMMON_BITMASK(p_cc_pdb,
                    ICF_CALL_CONNECT_DONE))
        {
            ICF_CC_SET_NEXT_STATE(p_cc_pdb,ICF_CC_06_CONNECTED) 
        }
        /* If pending list is not empty handle it */
        if(ICF_SUCCESS == ret_val &&
                ICF_NULL != ICF_CHECK_PENDING_LIST_IS_EMPTY(p_cc_pdb))
        {
            ret_val = icf_cc_mms_rsp_update_req(p_cc_pdb);
        }
            ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb)
            return ret_val;
        }    
        /*If modify is rejected means media has been reverted by MMI
         *Send failure response for all these streams*/
        else if (ICF_CC_MMS_RESP_MOD_REJECTED == p_cc_pdb->mms_resp_type)
        {		
            icf_cc_set_reject_stream_status(p_cc_pdb, 
               ICF_MEDIA_STREAM_REMOTE_OFFER_MID_CALL_REJECT);
        }

        /* Set media updated flag as true */
        p_cc_pdb->p_call_ctx->media_updated = ICF_TRUE;

        icf_cc_update_applied_sdp_mode(p_cc_pdb);

        if(ICF_NULL == ICF_CC_CHECK_COMMON_BITMASK(p_cc_pdb,
                               ICF_CALL_MOD_REQ_WO_SDP))
        {
             /*Update local SDP Codec from Applied SDP*/
             if (ICF_FAILURE == icf_cc_update_local_sdp(p_cc_pdb))
             {
                ret_val = ICF_FAILURE;
             }
        }

        if (ICF_SUCCESS == ret_val && ICF_NULL != ICF_CC_CHECK_COMMON_BITMASK(p_cc_pdb,
                               ICF_CALL_CONNECT_DONE))
        {
            icf_cc_stop_current_timer(p_cc_pdb);
            p_cc_pdb->is_call_modify_timer = ICF_TRUE; 
            if(ICF_FAILURE == icf_cc_start_setup_timer(p_cc_pdb))
            {
                ret_val = ICF_FAILURE;
            }
            /*fill all accepted streams and rejected streams in call modify request*/
            else if(ICF_FAILURE == icf_cc_send_pa_call_modify_ind(p_cc_pdb))
            {
                ret_val = ICF_FAILURE;
            }
            else
            {
                ICF_CC_SET_NEXT_STATE(p_cc_pdb,ICF_CC_15_AWT_CALL_MODIFY_ALERT_RESP)
            }
        }
        else
        {
            /* SPR 12674  CC should not go in connected state 
             ICF_CC_SET_NEXT_STATE(p_cc_pdb,ICF_CC_06_CONNECTED) */
            /* Send response for offer change to remote party */
            if(ICF_FAILURE ==  icf_cc_send_ssa_offer_chg_rsp(p_cc_pdb))
            {
                ret_val = ICF_FAILURE;
            }
                /* In case we are not sending any indication to app, free 
                 * the header list in call_ctx that were filled 
                 * by SSA on receiving the offer change from network */
                if (ICF_NULL != p_cc_pdb->p_call_ctx->header_list.count)
                {
                    ret_val = icf_cmn_free_header_list(
                                        p_cc_pdb->p_glb_pdb,
                                        &(p_cc_pdb->p_call_ctx->header_list),
                                        p_cc_pdb->p_ecode);
                }
        }

        /* If pending list is not empty handle it */
        if(ICF_SUCCESS == ret_val &&
            ICF_NULL != ICF_CHECK_PENDING_LIST_IS_EMPTY(p_cc_pdb))
        {
            ret_val = icf_cc_mms_rsp_update_req(p_cc_pdb);
        }
    }
    else if(ICF_NULL != ICF_CC_CHECK_PENDING_EVENT(p_cc_pdb,
                        ICF_PENDING_EVENT_SSA_CALL_MOD_CANCEL_REQ))
    {
        /*Case when mms is going on for remote offer and CANCEL
          of reinvite is recvd.Here only revert the media to
          last committed sdp and don't send cancel ind to PA
          as incoming call modify ind is also not sent to PA
          at this moment*/

           /* revert the changes*/
           if(ICF_FAILURE == icf_cc_derive_mms_type_for_revert_remote_change(p_cc_pdb))
           {
              ICF_CC_RESET_PENDING_EVENT(p_cc_pdb,
                        ICF_PENDING_EVENT_SSA_CALL_MOD_CANCEL_REQ)
              ICF_CC_RESET_ONGOING_EVENT(p_cc_pdb)
              ret_val=icf_cc_04_generic_err(p_cc_pdb);
           }
           else if(ICF_FAILURE == icf_cc_send_mms_req(p_cc_pdb))
           {
              ret_val = ICF_FAILURE;
           }
           else
           {
             if(ICF_TRUE == p_cc_pdb->mms_sent)
             {
                /*
                 * Update the states in the call context
                 */
                ICF_CC_SET_NEXT_STATE(p_cc_pdb,ICF_CC_04_AWT_MMS_RESP)
                ICF_CC_SET_ONGOING_EVENT(p_cc_pdb,
                              ICF_CC_SSA_CALL_MODIFY_CANCEL_REQ)

                /* Start the media timer  */
                ret_val = icf_cc_start_media_timer(p_cc_pdb);
              }
           }/* MMS was handled succesfully */
    }

    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb)
    return ret_val;
}


/******************************************************************************
 *
 * FUNCTION:        icf_cc_mms_resp_ssa_offer_change_resp
 *
 * DESCRIPTION:     This function is invoked when MMS response is received
 *                  and ongoing event is SSA Offer Change response
 *
 *****************************************************************************/
icf_return_t icf_cc_mms_resp_ssa_offer_change_resp(
	icf_cc_pdb_st      *p_cc_pdb)
{
	icf_return_t   ret_val = ICF_SUCCESS;

    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb)

    icf_cc_stop_current_timer(p_cc_pdb);

	/*if MMS response is failure for any stream in this case
	 *means media can't be revert back and hence clear the call*/
    if ((ICF_CC_MMS_RESP_OFFER_REJECTED == p_cc_pdb->mms_resp_type) ||
        (ICF_CC_MMS_RESP_MOD_REJECTED == p_cc_pdb->mms_resp_type))
    {
		ret_val = icf_cc_initiate_call_clear(p_cc_pdb);        
    }
    else
    {
        icf_cc_update_applied_sdp_mode(p_cc_pdb);

        if(ICF_TRUE == p_cc_pdb->p_call_ctx->update_reqd)
        {
            p_cc_pdb->common_bitmask |= ICF_CC_CALL_MODIFY_CFM_NT_REQD;

            if(ICF_FAILURE == icf_cc_send_pa_call_modify_ind(p_cc_pdb))
            {
                ret_val = ICF_FAILURE;
            }
           
            p_cc_pdb->p_call_ctx->update_reqd = ICF_FALSE;
            
        }
        /*If MMS response successful received then
         *Send call modify cfm to application and
         *go to state ICF_CC_06_CONNECTED*/

/*call_mod_cancel*/
        else if (ICF_NULL != 
        ICF_CC_CHECK_COMMON_BITMASK_2(p_cc_pdb, ICF_CALL_MOD_CANCEL_REINV_REQ &&
        ICF_NULL != 
        ICF_CC_CHECK_COMMON_BITMASK_2(p_cc_pdb, ICF_CALL_MOD_CANCEL_REQ)))
        {
            /*This is the case when 200 OK of 
             *2nd Re-Invite is received and after mms again 
             *to change in media due to peer send the response to PA*/
            /*Send succ resp for call modify cancel req*/
            /* Send failure response for call modify cancel to PA */
            if (ICF_FAILURE == icf_cc_send_pa_call_modify_cancel_resp(p_cc_pdb))
            {
                ret_val = ICF_FAILURE;
            }

            if(ICF_FAILURE != ret_val)
            {
                /* Send failure response for call modify req to PA */
                p_cc_pdb->p_call_ctx->app_failure_cause = 
                                                 ICF_CAUSE_REQUEST_CANCELLED_BY_USER;
                ICF_CC_SET_COMMON_BITMASK(p_cc_pdb, ICF_CALL_MODIFY_RESP_FAIL)
                if (ICF_FAILURE == icf_cc_send_pa_call_modify_resp(p_cc_pdb))
                {
                   ret_val = ICF_FAILURE;
                }
                ICF_CC_SET_NEXT_STATE(p_cc_pdb,ICF_CC_06_CONNECTED)
            }
        }
        else
        {
           if (ICF_FAILURE == icf_cc_send_pa_call_modify_resp(p_cc_pdb))
           {
              ret_val = ICF_FAILURE;
           }        
        }
    }
    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb)

	return ret_val;
}

#ifdef ICF_FAX_SUPPORT
/******************************************************************************
 *
 * FUNCTION:        icf_cc_mms_resp_pa_start_fax_req
 *
 * DESCRIPTION:     This function is invoked when MMS response is received
 *                  and ongoing event is Start FAX request
 *
 *****************************************************************************/
icf_return_t icf_cc_mms_resp_pa_start_fax_req(
	icf_cc_pdb_st      *p_cc_pdb)
{
	icf_return_t            ret_val = ICF_SUCCESS;
    icf_fax_start_type_t    fax_type;
    icf_boolean_t           send_update = ICF_TRUE;

    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb)

    /*icf_fax_start_type_t  fax_type; is stored in call context
     *When MMS response for FAX is received, check if any failure happens
     */

    fax_type = p_cc_pdb->p_call_ctx->fax_type;
    
   icf_cc_stop_current_timer(p_cc_pdb);

    /*If ADD new stream is failed then check */
    if ((ICF_CC_MMS_RESP_ADD_REJECTED == p_cc_pdb->mms_resp_type) || 
        (ICF_CC_MMS_RESP_OFFER_REJECTED == p_cc_pdb->mms_resp_type))
    {
        if (ICF_FAX_PT_CALL == fax_type)
        {
            /*Send failure FAX response to application*/
            ICF_CC_SET_COMMON_BITMASK(p_cc_pdb, ICF_FAX_RESP_FAIL)
            p_cc_pdb->p_call_ctx->app_failure_cause = ICF_CAUSE_MEDIA_FAILURE;
            p_cc_pdb->p_call_ctx->fax_type = ICF_NULL;
            p_cc_pdb->p_call_ctx->fax_method = ICF_FAX_METHOD_REPLACE_STREAM;
            ret_val = icf_cc_send_pa_fax_resp(p_cc_pdb);

	   if(ICF_NULL != ICF_CC_CHECK_PENDING_EVENT(p_cc_pdb,
                  ICF_PENDING_EVENT_PA_STOP_FAX_REQ))
	   {
		ret_val = icf_cc_send_pa_stop_fax_resp(p_cc_pdb);
		ICF_CC_RESET_PENDING_EVENT(p_cc_pdb,
			ICF_PENDING_EVENT_PA_STOP_FAX_REQ)
            }
            send_update = ICF_FALSE;
            ICF_CC_SET_NEXT_STATE(p_cc_pdb, ICF_CC_06_CONNECTED)
        }
        else if (ICF_T38_FAX_CALL == fax_type)
        {            
            /*Send MMS request to revert to old audio SDP*/
            ret_val = 
                icf_cc_send_mms_to_revert_audio_mode(p_cc_pdb);

            if (ICF_TRUE == p_cc_pdb->mms_sent)
            {
                 p_cc_pdb->p_call_ctx->app_failure_cause = ICF_CAUSE_MEDIA_FAILURE;
                 ICF_CC_SET_NEXT_STATE(p_cc_pdb, ICF_CC_04_AWT_MMS_RESP)
            }
	    else
	    {
		ICF_CC_SET_COMMON_BITMASK(p_cc_pdb, ICF_FAX_RESP_FAIL)
                p_cc_pdb->p_call_ctx->app_failure_cause = ICF_CAUSE_MEDIA_FAILURE;
                p_cc_pdb->p_call_ctx->fax_type = ICF_NULL;
                p_cc_pdb->p_call_ctx->fax_method = ICF_FAX_METHOD_REPLACE_STREAM;
                ret_val = icf_cc_send_pa_fax_resp(p_cc_pdb);
		if(ICF_NULL != ICF_CC_CHECK_PENDING_EVENT(p_cc_pdb,
                  ICF_PENDING_EVENT_PA_STOP_FAX_REQ))
	       {
		   ret_val = icf_cc_send_pa_stop_fax_resp(p_cc_pdb);
		   ICF_CC_RESET_PENDING_EVENT(p_cc_pdb,
			ICF_PENDING_EVENT_PA_STOP_FAX_REQ)
                }

                ICF_CC_SET_NEXT_STATE(p_cc_pdb, ICF_CC_06_CONNECTED)
	    }
            send_update = ICF_FALSE;
            
        }
        else
        {
		if(ICF_NULL != ICF_CC_CHECK_PENDING_EVENT(p_cc_pdb,
			ICF_PENDING_EVENT_PA_STOP_FAX_REQ))
        	{
		/*Send MMS request to revert to old audio SDP*/
	            ret_val = 
        	        icf_cc_send_mms_to_revert_audio_mode(p_cc_pdb);

            	if (ICF_TRUE == p_cc_pdb->mms_sent)
            	{
                	ICF_CC_SET_NEXT_STATE(p_cc_pdb, ICF_CC_04_AWT_MMS_RESP)
            	}
	    	else
	    	{
		ICF_CC_SET_COMMON_BITMASK(p_cc_pdb, ICF_FAX_RESP_FAIL)
                p_cc_pdb->p_call_ctx->app_failure_cause = ICF_CAUSE_MEDIA_FAILURE;
                p_cc_pdb->p_call_ctx->fax_type = ICF_NULL;
                ret_val = icf_cc_send_pa_fax_resp(p_cc_pdb);
		if(ICF_NULL != ICF_CC_CHECK_PENDING_EVENT(p_cc_pdb,
                  ICF_PENDING_EVENT_PA_STOP_FAX_REQ))
	       {
		   ret_val = icf_cc_send_pa_stop_fax_resp(p_cc_pdb);
		   ICF_CC_RESET_PENDING_EVENT(p_cc_pdb,
			ICF_PENDING_EVENT_PA_STOP_FAX_REQ)
                }

                ICF_CC_SET_NEXT_STATE(p_cc_pdb, ICF_CC_06_CONNECTED)
	    	}

		}
		else
		{
            		/*Send MMS request to switch audio stream to G711
             		*Search active audio stream and check if it is not already
             		*audio codec then send mms_req for codec change*/
            		ret_val = icf_cc_send_mms_req_for_fax_failover(p_cc_pdb);

            		/*It was a failover to FAX_PT call so, send MMS for fax 
             		*failover and set fax_type to FAX_PT_CALL*/
            		p_cc_pdb->p_call_ctx->fax_type = ICF_FAX_PT_CALL;

            		if (ICF_TRUE == p_cc_pdb->mms_sent)
            		{
                		ICF_CC_SET_NEXT_STATE(p_cc_pdb, ICF_CC_04_AWT_MMS_RESP)
                		send_update = ICF_FALSE;
            		}
		}
	    
        }
    }
    /*Send REINVITE to network if MMS response is successful*/
    if ((ICF_SUCCESS == ret_val) && (ICF_TRUE == send_update))
    {

	if(ICF_NULL != ICF_CC_CHECK_PENDING_EVENT(p_cc_pdb,
			ICF_PENDING_EVENT_PA_STOP_FAX_REQ))
       	{
		ret_val = icf_cc_make_revert_mms_fax_cancel(p_cc_pdb);
		p_cc_pdb->p_call_ctx->fax_type = ICF_NULL;
	}
	else
	{
        ICF_CC_SET_PDB_COMMON_BITMASK(p_cc_pdb,ICF_CC_LOCAL_FAX_REQ)
        
        if((ICF_T38_FAX_CALL == p_cc_pdb->p_call_ctx->fax_type ||
		ICF_T38_FAX_CALL_FAILURE_SWITCH_PT_CALL == 
		p_cc_pdb->p_call_ctx->fax_type) &&
		ICF_FAX_METHOD_REPLACE_STREAM ==
			p_cc_pdb->p_call_ctx->fax_method)
        {
	    icf_cc_update_nw_allow_for_stream(
			    p_cc_pdb,
			    ICF_STREAM_TYPE_AUDIO,
                            ICF_FALSE);
        }

        if(ICF_FAILURE == icf_cc_stop_current_timer(p_cc_pdb))
        {
            ret_val = ICF_FAILURE;
        }
        else if(ICF_FAILURE == icf_cc_start_setup_timer(p_cc_pdb))
        {
            ret_val = ICF_FAILURE;
        }
        else if(ICF_FAILURE == icf_cc_ssa_send_update_msg(p_cc_pdb))
        {
            ret_val = ICF_FAILURE;
        }
        else
        {
            ICF_CC_SET_NEXT_STATE(p_cc_pdb,
                ICF_CC_15_AWT_CALL_MODIFY_ALERT_RESP)

        }
	}
    }

    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb)
	return ret_val;
}


/******************************************************************************
 *
 * FUNCTION:        icf_cc_mms_resp_ssa_start_fax_req
 *
 * DESCRIPTION:     This function is invoked when MMS response is received
 *                  and ongoing event is Start FAX request from remote
 *
 *****************************************************************************/
icf_return_t icf_cc_mms_resp_ssa_start_fax_req(
	icf_cc_pdb_st      *p_cc_pdb)
{
	icf_return_t            ret_val = ICF_SUCCESS;

        icf_internal_list_st    *p_stream_sdp_info_node = ICF_NULL;
        icf_stream_sdp_info_st  *p_stream_sdp_info_data = ICF_NULL;
    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb)

    icf_cc_stop_current_timer(p_cc_pdb);

	if(ICF_NULL != ICF_CC_CHECK_PENDING_EVENT(p_cc_pdb,
                   ICF_PENDING_EVENT_SSA_STOP_FAX_REQ))
     {
       /* logic for reverting fax related mms  */
	ret_val = icf_cc_process_remote_fax_revert(p_cc_pdb);
     }
     else
	{	

    /*If Offer is rejected by MM, means modify is rejected and add new
    *stream is not attempted Send failure response for reinvite and 
    *goto state connected*/
    if (ICF_CC_MMS_RESP_OFFER_REJECTED == p_cc_pdb->mms_resp_type)
    {
        p_cc_pdb->p_call_ctx->offer_failure_reason = 
             ICF_CALL_MODIFY_REASON_REJECT_MEDIA_CHANGE;
        ret_val = icf_cc_send_ssa_offer_chg_err(p_cc_pdb);

        ICF_CC_SET_NEXT_STATE(p_cc_pdb, ICF_CC_06_CONNECTED)
    }
    /*If modify audio stream is successful and ADD new stream is failed 
     *then send failure response for reinvite and revert audio mode */
    else if (ICF_CC_MMS_RESP_ADD_REJECTED == p_cc_pdb->mms_resp_type)
    {
        /* SPR 17399: send 488
         */
        if((ICF_CALL_CLEAR_REASON_RESOURCES_NOT_AVAILABLE_FOR_T38_FAX ==
                    ICF_CC_GET_RELEASE_CAUSE(p_cc_pdb)))
        {
            p_cc_pdb->p_call_ctx->offer_failure_reason = 
                ICF_CALL_MODIFY_REASON_REJECT_MEDIA_CHANGE;
        }
        else
        {
           p_cc_pdb->p_call_ctx->offer_failure_reason = ICF_CALL_CLEAR_MEDIA_MISMATCH;
        }
        ret_val = icf_cc_send_ssa_offer_chg_err(p_cc_pdb);

        if (ICF_SUCCESS == ret_val)
        {
            /*Create MMS request to revert mode*/
            ret_val = 
                icf_cc_send_mms_to_revert_audio_mode(p_cc_pdb);             
            if(ICF_CC_ALL_STREAMS_DELETED == (ICF_CC_ALL_STREAMS_DELETED &
                     p_cc_pdb->common_bitmask))
            {
                icf_cc_initiate_call_clear(p_cc_pdb);
        	ICF_CC_SET_NEXT_STATE(p_cc_pdb, ICF_CC_06_CONNECTED)
            }
            else if (ICF_FALSE == p_cc_pdb->mms_sent)
	    {
              
        	ICF_CC_SET_NEXT_STATE(p_cc_pdb, ICF_CC_06_CONNECTED)
	    }
	    else
	    {
		 ICF_CC_SET_NEXT_STATE(p_cc_pdb, ICF_CC_04_AWT_MMS_RESP)
	    }
            
        }
    }
    else
    {
        icf_cc_update_applied_sdp_mode(p_cc_pdb);
        
        /* Fetch audio sdp  if exist 
           Set CLINE status as ICF_MEDIA_STREAM_CLINE_ZERO */
        p_stream_sdp_info_node = p_cc_pdb->p_call_ctx->p_stream_sdp_info;
        while(ICF_NULL != p_stream_sdp_info_node)
        {
           p_stream_sdp_info_data = (icf_stream_sdp_info_st*)
					(p_stream_sdp_info_node->p_payload);
           if (ICF_STREAM_TYPE_AUDIO == p_stream_sdp_info_data->stream_type)
           {
             p_stream_sdp_info_data->stream_status |= ICF_MEDIA_STREAM_CLINE_ZERO;
             break;
           }
           p_stream_sdp_info_node = p_stream_sdp_info_node->p_next;
        }	

        /*Send Start FAX indication to application and 
         *goto ICF_CC_15_AWT_CALL_MODIFY_ALERT_RESP*/

        if(ICF_SUCCESS != icf_cc_send_ssa_offer_chg_rsp(p_cc_pdb))
        {
            ret_val = ICF_FAILURE;
        }
        else if(ICF_SUCCESS != icf_cc_send_pa_start_fax_ind(p_cc_pdb))
        {
            ret_val = ICF_FAILURE;
        }
        else
        {
            ICF_CC_SET_NEXT_STATE(p_cc_pdb, ICF_CC_06_CONNECTED)
        }
    }
    }
    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb)
    return ret_val;
}
#endif


/******************************************************************************
 *
 * FUNCTION:        icf_cc_mms_resp_alert_prack
 *
 * DESCRIPTION:     This function is invoked when MMS response is received
 *                  and ongoing event is alert prack
 *                  1. If FS call and MMS response contains all stream 
 *                      rejection, send failure PRACK response
 *                  2. IF NFS call, complete offer rejected or some stream 
 *                      deleted, as it is the first remote answer, send PRACK
 *                      response and clear the call
 *                  3. Otherwise, send success PRACK response
 *
 *****************************************************************************/
icf_return_t icf_cc_mms_resp_alert_prack(
	icf_cc_pdb_st      *p_cc_pdb)
{
	icf_return_t   ret_val = ICF_SUCCESS;
    
    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb)

    if(ICF_TRUE == p_cc_pdb->p_call_ctx->media_committed)
    {
        /*MMS response received for 180 PRACK means it is the first answer
        *from remote, if any stream is rejected then initiate call clear*/
        if (ICF_CC_MMS_RESP_OFFER_REJECTED == p_cc_pdb->mms_resp_type)
        {
            p_cc_pdb->p_call_ctx->offer_failure_reason = ICF_SA_NETWORK_ERROR;
            ret_val = icf_cc_send_ssa_alert_prack_err(p_cc_pdb);
            ICF_CC_SET_NEXT_STATE(p_cc_pdb,ICF_CC_05_AWT_CONNECT);
            ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb)
            return ret_val;
        }
        else
        {
           /* fs call - if prack was after media commited set media 
            * updated as true*/
            p_cc_pdb->p_call_ctx->media_updated = ICF_TRUE;
            /*Mark the stream as MID CALL REJECTED, to send port=0
             *for the rejected streams*/
            icf_cc_set_reject_stream_status(p_cc_pdb, 
                ICF_MEDIA_STREAM_REMOTE_OFFER_MID_CALL_REJECT);
        }
    }
    else
    {
       /*MMS response received for 180 PRACK means it is the first answer
        *from remote, if any stream is rejected then initiate call clear*/
        if ((ICF_CC_MMS_RESP_OFFER_REJECTED == p_cc_pdb->mms_resp_type) || 
            (ICF_CC_MMS_RESP_MOD_REJECTED == p_cc_pdb->mms_resp_type))
        {
            /*This is the first answer from remote and MMS failure received
             *send PRACK response and clear call*/
            p_cc_pdb->p_call_ctx->offer_failure_reason = ICF_SA_NETWORK_ERROR;
            ret_val = icf_cc_send_ssa_alert_prack_err(p_cc_pdb);
            
            if (ICF_SUCCESS == ret_val)
            {
                ret_val = icf_cc_initiate_call_clear(p_cc_pdb);
            }
            ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb)
            return ret_val;
        }
        else
        {
           /* non fs call - prack was resp for sdp in alert, set media 
            * commited as true */
            p_cc_pdb->p_call_ctx->media_committed = ICF_TRUE;
            
            /* Return status as media commited  to CM */
            ICF_CC_SET_STATUS(p_cc_pdb, ICF_STATUS_MEDIA_COMMITTED) 
        }
    }

    if (ICF_SUCCESS == ret_val)
    {
       /* For a fast start call send alert prack resp to remote 
        * Also if it was a new offer send alert prack to remote*/
        if(ICF_NULL != ICF_CC_CHECK_COMMON_BITMASK(p_cc_pdb,
            ICF_CALL_MEDIA_SETUP_TYPE)
            || ICF_TRUE == p_cc_pdb->p_call_ctx->media_updated)
        {
            /* Set the flag so that media prov resp ack will have sdp */
            p_cc_pdb->media_to_be_sent = ICF_TRUE;        
        }
        
        icf_cc_update_applied_sdp_mode(p_cc_pdb);

        if(ICF_FAILURE == icf_cc_send_ssa_alert_prov_resp_ack(
            p_cc_pdb))
        {
            ret_val = ICF_FAILURE;
        }        
        
        if(ICF_NULL != ICF_CHECK_PENDING_LIST_IS_EMPTY(p_cc_pdb))
        {
            /* If pending list is not empty send mms again to pa */
            /* Mode for MMS will be collated for call mute/unmute
            * and update*/
            ret_val = icf_cc_mms_rsp_alert_prack_req(p_cc_pdb);
        }
    }
    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb)
    return ret_val;
}

/******************************************************************************
 *
 * FUNCTION:        icf_cc_mms_resp_media_param_prack
 *
 * DESCRIPTION:     This function is invoked when MMS response is received
 *                  and ongoing event is alert prack
 *                  1. If FS call and MMS response contains all stream 
 *                      rejection, send failure PRACK response
 *                  2. IF NFS call, complete offer rejected or some stream 
 *                      deleted, as it is the first remote answer, send PRACK
 *                      response and clear the call
 *                  3. Otherwise, send success PRACK response
 *
 *****************************************************************************/
icf_return_t icf_cc_mms_resp_media_param_prack(
	icf_cc_pdb_st      *p_cc_pdb)
{
    icf_return_t			ret_val = ICF_SUCCESS;
#ifdef ICF_QOS_SUPPORT
	icf_internal_list_st	*p_list = ICF_NULL;
	icf_stream_sdp_info_st	*p_sdp = ICF_NULL;
#endif

    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb)

    if(ICF_TRUE == p_cc_pdb->p_call_ctx->media_committed)
    {
        /*MMS response received for 180 PRACK means it is the first answer
        *from remote, if any stream is rejected then initiate call clear*/
        if (ICF_CC_MMS_RESP_OFFER_REJECTED == p_cc_pdb->mms_resp_type)
        {
            p_cc_pdb->p_call_ctx->offer_failure_reason = ICF_SA_NETWORK_ERROR;
            ret_val = icf_cc_send_ssa_media_prack_err(p_cc_pdb);
            if (ICF_FAILURE != ret_val)
            {
               p_cc_pdb->prack_reqd = ICF_TRUE;
               ret_val = icf_cc_send_ssa_alert(p_cc_pdb);
               if (ICF_FAILURE != ret_val)
               {
                  ICF_CC_SET_NEXT_STATE(p_cc_pdb,ICF_CC_10_AWT_ALERT_PRACK)
               }
            }
            ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb)
            return ret_val;
        }
        else
        {
            /* fs call - if prack was after media commited set media 
            * updated as true*/
            p_cc_pdb->p_call_ctx->media_updated = ICF_TRUE;
            /*Mark the stream as MID CALL REJECTED, to send port=0
             *for the rejected streams*/
            icf_cc_set_reject_stream_status(p_cc_pdb, 
                ICF_MEDIA_STREAM_REMOTE_OFFER_MID_CALL_REJECT);
        }
    }
    else
    {
        /*MMS response received for 180 PRACK means it is the first answer
        *from remote, if any stream is rejected then initiate call clear*/
        if ((ICF_CC_MMS_RESP_OFFER_REJECTED == p_cc_pdb->mms_resp_type) || 
            (ICF_CC_MMS_RESP_MOD_REJECTED == p_cc_pdb->mms_resp_type))
        {
            /*This is the first answer from remote and MMS failure received
             *send PRACK response and clear call*/
            p_cc_pdb->p_call_ctx->offer_failure_reason = ICF_SA_NETWORK_ERROR;
            ret_val = icf_cc_send_ssa_alert_prack_err(p_cc_pdb);

            if (ICF_SUCCESS == ret_val)
            {
                ret_val = icf_cc_initiate_call_clear(p_cc_pdb);
            }
            ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb)
            return ret_val;
        }
        else
        {
           /* non fs call - prack was resp for sdp in alert, set media 
            * commited as true */
            p_cc_pdb->p_call_ctx->media_committed = ICF_TRUE;
            
            /* Return status as media commited  to CM */
            ICF_CC_SET_STATUS(p_cc_pdb, ICF_STATUS_MEDIA_COMMITTED) 
        }
    }
    
    if (ICF_SUCCESS == ret_val)
    {
        icf_cc_update_applied_sdp_mode(p_cc_pdb);

        /* For a fast start call send media param prack to remote */
        if(ICF_NULL != ICF_CC_CHECK_COMMON_BITMASK(p_cc_pdb,
            ICF_CALL_MEDIA_SETUP_TYPE))
        {
            /* Set the flag so that media prov resp will have sdp */
            p_cc_pdb->media_to_be_sent = ICF_TRUE;
        }
        if(ICF_FAILURE == icf_cc_send_ssa_media_prov_resp_ack(
            p_cc_pdb))
        {
            ret_val = ICF_FAILURE;
        }        
        
        if(ICF_NULL != ICF_CHECK_PENDING_LIST_IS_EMPTY(p_cc_pdb) && 
            ICF_SUCCESS == ret_val)
        {
            /* If pending list is not empty send mms again to pa */
            /* Mode for MMS will be collated for call mute/unmute
            * and update*/
            ret_val = icf_cc_mms_rsp_media_prack_req(p_cc_pdb);
        }
        else
        {
/* To be tested*/
#ifdef ICF_QOS_SUPPORT
ICF_CHECK_QOS_SUPPORT_START(p_cc_pdb->p_glb_pdb,ICF_TRUE)
	
			p_list = p_cc_pdb->p_call_ctx->p_stream_sdp_info;

			while (ICF_NULL != p_list)
			{
				p_sdp = (icf_stream_sdp_info_st*)(p_list->p_payload);

				if(ICF_NULL == p_sdp->p_local_sdp)
				{
					p_list = p_list->p_next;
					continue;
				}

				/* we set the cc state to AWT_UPDATE only if remote has indicated 
				 * that the desired status at its end is MANDATORY */
				if((p_sdp->p_local_sdp->remote_send.desired_status == 
					ICF_RESOURCE_RESERVATION_MANDATORY) || 
				(p_sdp->p_local_sdp->remote_recv.desired_status ==
					ICF_RESOURCE_RESERVATION_MANDATORY))
				{

					/* Set Call State to AWT_UPDATE */
					ICF_CC_SET_CALL_STATE(p_cc_pdb, ICF_CC_16_AWT_UPDATE)

					ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */
			        return ret_val;
				}
				/* go to the next node in list */
				p_list = (icf_internal_list_st*)p_list->p_next;
			}

			/* Since remote did not indicate mandatory desired_status, 
			 * we do not wait for UPDATE from remote. send incoming_call_ind 
			 * to PA and set state to AWT_ALERT_RESP to await user_alerted 
			 * response from PA */
			/* Send an incoming call indication to PA */
			if (ICF_FAILURE  == 
						icf_cc_send_pa_incoming_call_ind(p_cc_pdb))
			{
				ret_val = ICF_FAILURE;
			}
			else
			{
				/*
				 * Update the states in the call context
				 */
				ICF_CC_SET_CALL_STATE(
						 p_cc_pdb,ICF_CC_03_AWT_ALERT_RESP)
			}

ICF_CHECK_QOS_SUPPORT_END
ICF_CHECK_QOS_SUPPORT_START(p_cc_pdb->p_glb_pdb,ICF_FALSE)
            /*
            * Set prack required flag to true. This would ensure even if the 
            * configuration has changed the prack required flag to FALSE we 
            * would still continue sending the message with 100 rel in require 
            * header. This would ensure both provisional messages would either 
            * be sent with 100 rel or without 100 rel
            */
            
            if(ICF_PRACK_FOR_MEDIA_CONNECT != (ICF_CC_CHECK_COMMON_BITMASK(\
                            p_cc_pdb,ICF_PRACK_FOR_MEDIA_CONNECT)))
            {
                p_cc_pdb->prack_reqd = ICF_TRUE;
            
            /*Send alert to SSA*/
            ret_val = icf_cc_send_ssa_alert(p_cc_pdb);
            }
            else
            {
                ICF_CC_RESET_COMMON_BITMASK(p_cc_pdb,ICF_PRACK_FOR_MEDIA_CONNECT)
                ICF_CC_SET_NEXT_STATE(p_cc_pdb,ICF_CC_05_AWT_CONNECT)

            }
ICF_CHECK_QOS_SUPPORT_END
#else
            /*
            * Set prack required flag to true. This would ensure even if the 
            * configuration has changed the prack required flag to FALSE we 
            * would still continue sending the message with 100 rel in require 
            * header. This would ensure both provisional messages would either 
            * be sent with 100 rel or without 100 rel
            */
            if(ICF_PRACK_FOR_MEDIA_CONNECT != (ICF_CC_CHECK_COMMON_BITMASK(\
                            p_cc_pdb,ICF_PRACK_FOR_MEDIA_CONNECT)))
            {
               if(ICF_PRACK_FOR_MEDIA_CONNECT != (ICF_CC_CHECK_COMMON_BITMASK(\
                    p_cc_pdb,ICF_PRACK_FOR_MEDIA_CONNECT)))
               {
                    p_cc_pdb->prack_reqd = ICF_TRUE;
            
                /*Send alert to SSA*/
                ret_val = icf_cc_send_ssa_alert(p_cc_pdb);
               }
               else
               {
                  ICF_CC_RESET_COMMON_BITMASK(p_cc_pdb,ICF_PRACK_FOR_MEDIA_CONNECT)
                  ICF_CC_SET_NEXT_STATE(p_cc_pdb,ICF_CC_05_AWT_CONNECT)
               }
            }
            else
            {
                ICF_CC_RESET_COMMON_BITMASK(p_cc_pdb,ICF_PRACK_FOR_MEDIA_CONNECT)
                ICF_CC_SET_NEXT_STATE(p_cc_pdb,ICF_CC_05_AWT_CONNECT)
            }
#endif
        }
    }
    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb)
    return ret_val;
}


/******************************************************************************
 *
 * FUNCTION:        icf_cc_mms_resp_ssa_connect_ack
 *
 * DESCRIPTION:     This function is invoked when MMS response is received
 *                  and ongoing event is SSA connect ACK. In case of NFS 
 *                  call remote answered SDP in ACK.
 *
 *****************************************************************************/
icf_return_t icf_cc_mms_resp_ssa_connect_ack(
	icf_cc_pdb_st      *p_cc_pdb)
{
    icf_return_t   ret_val = ICF_SUCCESS;
    icf_internal_list_st             *p_stream_sdp_info_node = ICF_NULL;
    icf_stream_sdp_info_st           *p_stream_sdp_info_data = ICF_NULL;

    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb)
    /*It is MMS for first answer from remote and MMS response is rejected
     *or some stream is rejected by MM, then initiate call clear*/
    if ((ICF_CC_MMS_RESP_OFFER_REJECTED == p_cc_pdb->mms_resp_type) || 
        (ICF_CC_MMS_RESP_MOD_REJECTED == p_cc_pdb->mms_resp_type))
    {
        /*This is the first answer from remote and MMS failure received
        *clear call*/
        ret_val = icf_cc_initiate_call_clear(p_cc_pdb);
        ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb)
        return ret_val;
    }

    icf_cc_update_applied_sdp_mode(p_cc_pdb);

    /* non fs call - sdp was given in connect ack, set media 
    * commited as true */
    p_cc_pdb->p_call_ctx->media_committed = ICF_TRUE;
    
    /* Return status as media commited  to CM */
    ICF_CC_SET_STATUS(p_cc_pdb, ICF_STATUS_MEDIA_COMMITTED) 
        
    /* Return status as signalling done to CM */
    ICF_CC_SET_STATUS(p_cc_pdb, ICF_STATUS_SIGNAL_ONGOING_ENDED) 
        
    /* Reset signalling ongoing flag */
    ICF_CC_RESET_SIG_ONGOING(p_cc_pdb)    
        
    /*Update the stat for successfull incomming call establishment*/
    ICF_STAT(p_cc_pdb->p_glb_pdb, ICF_STAT_SUCC_INC_CALLS_EST)
        
    /*Update the stats for successfull calls*/
    ICF_STAT(p_cc_pdb->p_glb_pdb, ICF_STAT_SUCC_CALLS_EST)

    /*Update local SDP, if any stream is rejected in the
    *PA connect request And mark if UPDATE to be send
    *after ACK is received*/
    if (ICF_SUCCESS == icf_cc_check_if_update_to_be_sent(p_cc_pdb))
    {
        p_cc_pdb->p_call_ctx->update_reqd = ICF_TRUE;
    }

     /* In case of incoming non fast start call, we will free p_offered_sdp here.*/
    p_stream_sdp_info_node = p_cc_pdb->p_call_ctx->p_stream_sdp_info;
    while(ICF_NULL != p_stream_sdp_info_node)
    {
        p_stream_sdp_info_data = p_stream_sdp_info_node->p_payload;
        if(ICF_NULL != p_stream_sdp_info_data->p_offered_sdp)
        {
           icf_cmn_delete_internal_sdp_stream(p_cc_pdb->p_glb_pdb,
		&(p_stream_sdp_info_data->p_offered_sdp),
		p_cc_pdb->p_ecode);

        }
        p_stream_sdp_info_node = p_stream_sdp_info_node->p_next;
    }


    /*Update local SDP*/
    ret_val = icf_cc_update_local_sdp(p_cc_pdb);

    if (ICF_SUCCESS == ret_val)
    {
        if ((ICF_INVALID_CALL_ID == p_cc_pdb->p_call_ctx->replace_initiated_call_id) &&
            (ICF_FAILURE == icf_cc_send_pa_media_connect(p_cc_pdb)))
        {
            ret_val = ICF_FAILURE;
        }
        else if ((ICF_INVALID_CALL_ID == p_cc_pdb->p_call_ctx->replace_initiated_call_id) && 
				 (ICF_FAILURE == icf_cc_send_pa_connect_resp(p_cc_pdb)))
        {
            ret_val = ICF_FAILURE;
        }
        else if(ICF_NULL != ICF_CHECK_PENDING_LIST_IS_EMPTY(p_cc_pdb))
        {
            /* If pending list is not empty send mms again to pa */
            /* Mode for MMS will be collated for call mute/unmute
            * and update*/
            ret_val = icf_cc_mms_rsp_ssa_call_con_ack(p_cc_pdb);
        }
    }

    /*If media is modified with the offer sent to remote then
     *send UPDATE request to remote, with the latest media
     *applied at local end*/
    if ((ICF_TRUE == p_cc_pdb->p_call_ctx->update_reqd) && 
        (ICF_SUCCESS == ret_val))
    {

        /*Send UPDATE request*/
        if (ICF_FAILURE == icf_cc_ssa_send_update_msg(p_cc_pdb))
        {
            ret_val = ICF_FAILURE;
        }
        else
        {
            ICF_CC_SET_NEXT_STATE(p_cc_pdb,
                ICF_CC_14_AWT_CALL_MODIFY_RESP)
        }
    }
    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb)
    return ret_val;
}


/******************************************************************************
 *
 * FUNCTION:        icf_cc_mms_resp_ssa_call_connect
 *
 * DESCRIPTION:     This function is invoked when MMS response is received
 *                  and ongoing event is SSA connect ACK. In case of NFS 
 *                  call remote answered SDP in ACK.
 *
 *****************************************************************************/
icf_return_t icf_cc_mms_resp_ssa_call_connect(
	icf_cc_pdb_st      *p_cc_pdb)
{
    icf_return_t   ret_val = ICF_SUCCESS;

    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb)
    /*It is MMS for first answer from remote and MMS response is rejected
     *or some stream is rejected by MM, then initiate call clear*/
    if ((ICF_CC_MMS_RESP_OFFER_REJECTED == p_cc_pdb->mms_resp_type) || 
        (ICF_CC_MMS_RESP_MOD_REJECTED == p_cc_pdb->mms_resp_type))
    {
        /*This is the first answer from remote and MMS failure received
        *clear call*/
        ret_val = icf_cc_initiate_call_clear(p_cc_pdb);
        ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb)
        return ret_val;
    }
    if (ICF_NULL == ICF_CC_CHECK_COMMON_BITMASK(p_cc_pdb,ICF_CALL_MOD_REQ_WO_SDP))
    {
      ICF_CC_RESET_SIG_ONGOING(p_cc_pdb)    
    }
  
    /* non fs call - sdp was given in connect ack, set media 
    * commited as true */
    p_cc_pdb->p_call_ctx->media_committed = ICF_TRUE;

    icf_cc_update_applied_sdp_mode(p_cc_pdb);

    
    /* Return status as media commited  to CM */
    ICF_CC_SET_STATUS(p_cc_pdb, ICF_STATUS_MEDIA_COMMITTED) 

    /*Update local SDP, if any stream is rejected in the
    *PA connect request And mark if UPDATE to be send
    *after ACK is received*/

    if (ICF_SUCCESS == icf_cc_check_if_update_to_be_sent(p_cc_pdb)) 
    {
        p_cc_pdb->p_call_ctx->update_reqd = ICF_TRUE;
    }

    /*Update local SDP*/
    ret_val = icf_cc_update_local_sdp(p_cc_pdb);

    if (ICF_SUCCESS == ret_val)
    {
    /* In case call connect was pending event it means sdp is applied
        * for first time */ 
       
      if(ICF_FALSE == p_cc_pdb->p_call_ctx->update_reqd)
       { 
         if (ICF_FAILURE == icf_cc_stop_current_timer(p_cc_pdb))
          {
            ret_val = ICF_FAILURE;
          }
       }
        /*
        * Send CONNECT to PA
        */
        if (ICF_FAILURE == icf_cc_send_pa_connect_ind(p_cc_pdb))
        {
            ret_val = ICF_FAILURE;
        } /* else if (ICF_FAILURE == ipp... */
        /* Send ACK to SSA */
        else
        {
            /* Update the states in the call context */
            ICF_CC_SET_NEXT_STATE(p_cc_pdb,ICF_CC_06_CONNECTED)
        } /* else */    
    }
    
    if(ICF_NULL != ICF_CHECK_PENDING_LIST_IS_EMPTY(p_cc_pdb) &&
        ICF_SUCCESS == ret_val)
    {
        /* If pending list is not empty, means SSA offer change 
         * is pending_event, so send MMS for SSA Offer Change*/
        ret_val = icf_cc_mms_rsp_ssa_call_con_ack(p_cc_pdb);
    }

    /*If media is modified with the offer sent to remote then
     *send UPDATE request to remote, with the latest media
     *applied at local end*/
    if ((ICF_TRUE == p_cc_pdb->p_call_ctx->update_reqd) && 
        (ICF_SUCCESS == ret_val))
    {

        /*Send UPDATE request*/
        if (ICF_FAILURE == icf_cc_ssa_send_update_msg(p_cc_pdb))
        {
            ICF_CC_SET_NEXT_STATE(p_cc_pdb,
                ICF_CC_06_CONNECTED)
                /*ret_val = ICF_FAILURE;*/
        }
        else
        {
            ICF_CC_SET_NEXT_STATE(p_cc_pdb,
                ICF_CC_14_AWT_CALL_MODIFY_RESP)
        }
    }
    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb)
    return ret_val;
}


/******************************************************************************
 *
 * FUNCTION:        icf_cc_mms_resp_pa_call_connect
 *
 * DESCRIPTION:     This function is invoked when MMS response is received
 *                  and ongoing event is Call connect from application
 *
 *****************************************************************************/
icf_return_t icf_cc_mms_resp_pa_call_connect(
	icf_cc_pdb_st      *p_cc_pdb)
{
    icf_return_t   ret_val = ICF_SUCCESS;

    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb)

    /*It is MMS for first answer from remote and MMS response is rejected
     *or some stream is rejected by MM, then initiate call clear*/
    if ((ICF_CC_MMS_RESP_OFFER_REJECTED == p_cc_pdb->mms_resp_type) || 
        (ICF_CC_MMS_RESP_MOD_REJECTED == p_cc_pdb->mms_resp_type))
    {
        /*This is the first answer from remote and MMS failure received
        *clear call*/
        ret_val = icf_cc_initiate_call_clear(p_cc_pdb);
        ICF_CC_SET_NEXT_STATE(p_cc_pdb,ICF_CC_04_AWT_MMS_RESP)
        ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb)
        return ret_val;
    }

    /*Update local SDP, if any stream is rejected in the
    *PA connect request And mark if UPDATE to be send
    *after ACK is received*/
    if(ICF_TRUE == p_cc_pdb->p_call_ctx->media_committed)
    {
        if (ICF_SUCCESS == icf_cc_check_if_update_to_be_sent(p_cc_pdb))
        {
            p_cc_pdb->p_call_ctx->update_reqd = ICF_TRUE;
        }
    }
    
    ret_val = icf_cc_mms_rsp_pa_connect(p_cc_pdb);    

    if (ICF_SUCCESS == ret_val && 
            ICF_TRUE == p_cc_pdb->p_call_ctx->media_committed)
    {
        /*Update local SDP*/
        ret_val = icf_cc_update_local_sdp(p_cc_pdb);
    }
    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb)
    return ret_val;
}


/******************************************************************************
 *
 * FUNCTION:        icf_cc_mms_resp_pa_call_modify_req
 *
 * DESCRIPTION:     This function is invoked when MMS response is received
 *                  and ongoing event is call modify from application.
 *
 *****************************************************************************/
icf_return_t icf_cc_mms_resp_pa_call_modify_req(
	icf_cc_pdb_st      *p_cc_pdb)
{
    icf_return_t            ret_val = ICF_SUCCESS;
    icf_config_data_st      *p_config_data = ICF_NULL;

    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb)

    if(ICF_FAILURE == icf_cc_stop_current_timer(p_cc_pdb))
    {
        ret_val = ICF_FAILURE;
        return ret_val;
    }
	  
    /*It is MMS for call modify request from application
     *if MMS is failed, send failure call_modify_resp to app*/
    if (ICF_CC_MMS_RESP_OFFER_REJECTED == p_cc_pdb->mms_resp_type)
    {
        /*This is the first answer from remote and MMS failure received
        *clear call*/
        p_cc_pdb->p_call_ctx->app_failure_cause = ICF_CAUSE_MEDIA_FAILURE;
        
        ICF_CC_SET_COMMON_BITMASK(p_cc_pdb, ICF_CALL_MODIFY_RESP_FAIL)
        ret_val = icf_cc_send_pa_call_modify_resp(p_cc_pdb);
	ICF_CC_SET_NEXT_STATE(p_cc_pdb,ICF_CC_06_CONNECTED)
        ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb)
        return ret_val;
    }
    else if (ICF_CC_MMS_RESP_MOD_REJECTED == p_cc_pdb->mms_resp_type)
    {
        ret_val = icf_cc_back_local_sdp(p_cc_pdb);
    }
    
    if (ICF_SUCCESS == ret_val)
    {
        p_cc_pdb->p_call_ctx->media_updated = ICF_TRUE;
        
        /* Get pointer to config data from DBM */
        ret_val = icf_dbm_get_module_glb_data(
            p_cc_pdb->p_glb_pdb,
            (icf_int_module_id_t)ICF_INT_MODULE_CFG,
            (icf_void_t *)&p_config_data, p_cc_pdb->p_ecode);
        
        ICF_ASSERT(p_cc_pdb->p_glb_pdb, (ICF_NULL != p_config_data));
    }
    
    /* send the call modify request to SSA */
    if(ICF_SUCCESS == ret_val)
    {
        if(ICF_FAILURE == icf_cc_start_setup_timer(p_cc_pdb))
        {
            ret_val = ICF_FAILURE;
        }
        else if(ICF_FAILURE == icf_cc_ssa_send_update_msg(p_cc_pdb))
        {
            ret_val = ICF_FAILURE;
        }
        else
        {
            if(ICF_CFG_CALL_MODIFY_METHOD_INVITE ==
                p_config_data->call_modify_req_method)
            {
                ICF_CC_SET_NEXT_STATE(p_cc_pdb,ICF_CC_15_AWT_CALL_MODIFY_ALERT_RESP)
            }
            else
            {
                ICF_CC_SET_NEXT_STATE(p_cc_pdb,ICF_CC_14_AWT_CALL_MODIFY_RESP)
            }
        }
    }
    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb)
    return ret_val;
}


/******************************************************************************
 *
 * FUNCTION:        icf_cc_mms_resp_pa_alert
 *
 * DESCRIPTION:     This function is invoked when MMS response is received
 *                  and ongoing event Alert from application
 *
 *****************************************************************************/
icf_return_t icf_cc_mms_resp_pa_alert(
	icf_cc_pdb_st      *p_cc_pdb)
{
    icf_return_t   ret_val = ICF_SUCCESS;

    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /* function entry trace */
            
#ifdef ICF_QOS_SUPPORT
ICF_CHECK_QOS_SUPPORT_START(p_cc_pdb->p_glb_pdb,ICF_FALSE)
    if (ICF_SUCCESS == ret_val)
    {
	    /* check the call type and stream list to take further action */
        if ((ICF_NULL == ICF_CC_CHECK_COMMON_BITMASK(p_cc_pdb,
            ICF_CALL_MEDIA_SETUP_TYPE))||
            (/* OR If media_setup_type is non-null and inband_info false*/
                (ICF_NULL != ICF_CC_CHECK_COMMON_BITMASK(p_cc_pdb,
                ICF_CALL_MEDIA_SETUP_TYPE)) && 
                (ICF_FALSE == p_cc_pdb->p_call_ctx->inband_info_present)
            ))
        {
            /* Send media param to the SSA */
            if (ICF_FAILURE == icf_cc_send_ssa_media_param(p_cc_pdb))
            {
                ret_val = ICF_FAILURE;
            } /* if (ICF_FAILURE == icf_c... */
            else
            {
                ICF_CC_TRACE(p_cc_pdb, ICF_TRACE_PRACK_REQD)
                    
                if (ICF_TRUE == p_cc_pdb->prack_reqd)
                {
                    /* Update the states in the call context */
                    ICF_CC_SET_NEXT_STATE(p_cc_pdb,ICF_CC_09_AWT_MEDIA_PARAM_PRACK)
                } /* if (ICF_TRUE == p_cc_pdb->p... */
                else
                {
                    if (ICF_FAILURE == icf_cc_send_ssa_alert(p_cc_pdb))
                    {
                        ret_val = ICF_FAILURE;
                    } /* if (ICF_FAILURE == icf_c... */
                    else
                    {
                        /* Update the states in the call context */
                        ICF_CC_SET_NEXT_STATE(p_cc_pdb,ICF_CC_05_AWT_CONNECT)
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
                
            if (ICF_FAILURE == icf_cc_send_ssa_alert_media(p_cc_pdb))
            {
                ret_val = ICF_FAILURE;
            } /* if (ICF_FAILURE == icf_c... */
            else if (ICF_TRUE == p_cc_pdb->prack_reqd)
            {
                /* Update the states in the call context */
                ICF_CC_SET_NEXT_STATE(p_cc_pdb,ICF_CC_10_AWT_ALERT_PRACK)
            } /* if (ICF_TRUE == p_cc_pdb->p... */
            else
            {
                /* Update the states in the call context */
                ICF_CC_SET_NEXT_STATE(p_cc_pdb,ICF_CC_05_AWT_CONNECT)
            } /* else */
        } /* (ICF_TRUE == p_cc_pdb->p_ca... */
        if(ICF_NULL != ICF_CC_CHECK_PENDING_EVENT(p_cc_pdb,
                    ICF_PENDING_EVENT_PA_CALL_CONNECT)) 
         {
             /* Reset media ongoing flag */
              ICF_CC_RESET_MEDIA_ONGOING(p_cc_pdb)
              ret_val = icf_cc_snd_mms_pa_connect(p_cc_pdb);
              ICF_CC_SET_NEXT_STATE(p_cc_pdb,
              ICF_CC_11_AWT_SSA_CONNECT_ACK)
        }

	} /* end else */
ICF_CHECK_QOS_SUPPORT_END
#else
    if (ICF_SUCCESS == ret_val)
    {
	    /* check the call type and stream list to take further action */
        if ((ICF_NULL == ICF_CC_CHECK_COMMON_BITMASK(p_cc_pdb,
            ICF_CALL_MEDIA_SETUP_TYPE))||
            (/* OR If media_setup_type is non-null and inband_info false*/
                (ICF_NULL != ICF_CC_CHECK_COMMON_BITMASK(p_cc_pdb,
                ICF_CALL_MEDIA_SETUP_TYPE)) && 
                (ICF_FALSE == p_cc_pdb->p_call_ctx->inband_info_present)
            ))
        {
            /* Send media param to the SSA */
            if (ICF_FAILURE == icf_cc_send_ssa_media_param(p_cc_pdb))
            {
                ret_val = ICF_FAILURE;
            } /* if (ICF_FAILURE == icf_c... */
            else
            {
                ICF_CC_TRACE(p_cc_pdb, ICF_TRACE_PRACK_REQD)
                    
                if (ICF_TRUE == p_cc_pdb->prack_reqd)
                {
                    /* Update the states in the call context */
                    ICF_CC_SET_NEXT_STATE(p_cc_pdb,ICF_CC_09_AWT_MEDIA_PARAM_PRACK)
                } /* if (ICF_TRUE == p_cc_pdb->p... */
                else
                {
                    if (ICF_FAILURE == icf_cc_send_ssa_alert(p_cc_pdb))
                    {
                        ret_val = ICF_FAILURE;
                    } /* if (ICF_FAILURE == icf_c... */
                    else
                    {
                        /* Update the states in the call context */
                        ICF_CC_SET_NEXT_STATE(p_cc_pdb,ICF_CC_05_AWT_CONNECT)
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
                
            if (ICF_FAILURE == icf_cc_send_ssa_alert_media(p_cc_pdb))
            {
                ret_val = ICF_FAILURE;
            } /* if (ICF_FAILURE == icf_c... */
            else if (ICF_TRUE == p_cc_pdb->prack_reqd)
            {
                /* Update the states in the call context */
                ICF_CC_SET_NEXT_STATE(p_cc_pdb,ICF_CC_10_AWT_ALERT_PRACK)
            } /* if (ICF_TRUE == p_cc_pdb->p... */
            else
            {
                /* Update the states in the call context */
                ICF_CC_SET_NEXT_STATE(p_cc_pdb,ICF_CC_05_AWT_CONNECT)
            } /* else */
        } /* (ICF_TRUE == p_cc_pdb->p_ca... */
        if(ICF_NULL != ICF_CC_CHECK_PENDING_EVENT(p_cc_pdb,
                    ICF_PENDING_EVENT_PA_CALL_CONNECT)) 
         {
             /* Reset media ongoing flag */
              ICF_CC_RESET_MEDIA_ONGOING(p_cc_pdb)
              ret_val = icf_cc_snd_mms_pa_connect(p_cc_pdb);
              ICF_CC_SET_NEXT_STATE(p_cc_pdb,
              ICF_CC_11_AWT_SSA_CONNECT_ACK)
        }
	} /* end else */
#endif


    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */

    return ret_val;
}



/******************************************************************************
 *
 * FUNCTION:        icf_cc_mms_resp_media_revert
 *
 * DESCRIPTION:     This function is invoked when MMS response is received
 *                  and ongoing event media_revert local/remote
 *
 *****************************************************************************/
icf_return_t icf_cc_mms_resp_media_revert(
	icf_cc_pdb_st      *p_cc_pdb)
{
    icf_return_t   ret_val = ICF_SUCCESS;
    icf_uint8_t      ongoing_event = 0;

    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb)

    ongoing_event =  ICF_CC_GET_ONGOING_EVENT(p_cc_pdb);

   icf_cc_stop_current_timer(p_cc_pdb);

    if(ICF_CC_ONGOING_EVENT_REMOTE_MEDIA_REVERT == ongoing_event)
    {
        if(ICF_NULL != ICF_CC_CHECK_COMMON_BITMASK_2(p_cc_pdb,
             ICF_CALL_MOD_CFM_PARTIAL_OFFER_REJECT))
        {
           /* partial offer is rejected */
           if(ICF_NULL != ICF_CC_CHECK_PENDING_EVENT(p_cc_pdb,
                    ICF_PENDING_EVENT_SSA_CALL_MOD_CANCEL_REQ))
           {
              ICF_CC_RESET_PENDING_EVENT(p_cc_pdb,
                 ICF_PENDING_EVENT_SSA_CALL_MOD_CANCEL_REQ)
              if(ICF_FAILURE == icf_cc_derive_mms_type_for_revert_remote_change(p_cc_pdb))
              {
                  ret_val = ICF_FAILURE;
              }
              else if(ICF_FAILURE == icf_cc_send_mms_req(p_cc_pdb))
              {
                 ret_val = ICF_FAILURE;
              }
              else
              {
                if(ICF_TRUE == p_cc_pdb->mms_sent)
                {
                /*
                 * Update the states in the call context
                 */
                ICF_CC_SET_NEXT_STATE(p_cc_pdb,ICF_CC_04_AWT_MMS_RESP)
                ICF_CC_SET_ONGOING_EVENT(p_cc_pdb,
                              ICF_CC_SSA_CALL_MODIFY_CANCEL_REQ)

                /* Start the media timer  */
                ret_val = icf_cc_start_media_timer(p_cc_pdb);
                }
              }/* MMS was handled succesfully */
           }
           else
           {
              ret_val = icf_cc_send_ssa_offer_chg_rsp(p_cc_pdb);
              ICF_CC_RESET_COMMON_BITMASK_2(p_cc_pdb,
                  ICF_CALL_MOD_CFM_PARTIAL_OFFER_REJECT)
           }
        }
        else if(ICF_NULL != ICF_CC_CHECK_PENDING_EVENT(p_cc_pdb,
                    ICF_PENDING_EVENT_SSA_CALL_MOD_CANCEL_REQ))
        {
           ICF_CC_RESET_PENDING_EVENT(p_cc_pdb,
            ICF_PENDING_EVENT_SSA_CALL_MOD_CANCEL_REQ)
        }
        else
        {
          /* complete offer is rejected */
           ret_val = icf_cc_send_ssa_offer_chg_err(p_cc_pdb);
        }
    }
    else if(ICF_CC_ONGOING_EVENT_LOCAL_MEDIA_REVERT == ongoing_event)
    {
        if(ICF_NULL != p_cc_pdb->p_call_ctx->fax_type)
        {
            if(ICF_NULL == ICF_CC_CHECK_COMMON_BITMASK(
                            p_cc_pdb, ICF_CALL_MODIFY_DIR))
            {
                if((ICF_T38_FAX_CALL == p_cc_pdb->p_call_ctx->fax_type ||
	               	ICF_T38_FAX_CALL_FAILURE_SWITCH_PT_CALL == 
               		p_cc_pdb->p_call_ctx->fax_type) &&
	             	ICF_FAX_METHOD_REPLACE_STREAM ==
			p_cc_pdb->p_call_ctx->fax_method)
		{
			icf_cc_update_nw_allow_for_stream(
					p_cc_pdb,
					ICF_STREAM_TYPE_AUDIO,
					ICF_TRUE);
		}

		/* 
		 * Revert the fax method to the default method
		 * which is ICF_FAX_METHOD_REPLACE_STREAM
		 */
		p_cc_pdb->p_call_ctx->fax_method = 
			ICF_FAX_METHOD_REPLACE_STREAM;

                 /* send error response for local fax request */
                /*Send failure FAX response to application*/
                ICF_CC_SET_COMMON_BITMASK(p_cc_pdb, ICF_FAX_RESP_FAIL)
            
                ret_val = icf_cc_send_pa_fax_resp(p_cc_pdb);
		if(ICF_NULL != ICF_CC_CHECK_PENDING_EVENT(
			p_cc_pdb,ICF_PENDING_EVENT_PA_STOP_FAX_REQ))
		{
		    ret_val = icf_cc_send_pa_stop_fax_resp(p_cc_pdb);
		    ICF_CC_RESET_PENDING_EVENT(p_cc_pdb,
			ICF_PENDING_EVENT_PA_STOP_FAX_REQ)
		}
            }
            p_cc_pdb->p_call_ctx->fax_type = ICF_NULL;
        }
        else
        {
            if(ICF_NULL !=
               ICF_CC_CHECK_COMMON_BITMASK_2(p_cc_pdb,
                                       ICF_INCOMING_CALL_MOD_CANCEL_REQ))
            {
               
               /*Don't send any resp to PA,reset the bitmask*/
               ICF_CC_RESET_COMMON_BITMASK_2(p_cc_pdb, 
                                       ICF_INCOMING_CALL_MOD_CANCEL_REQ);
            }
            else
            {
               /* send error response for call modify request */
               ICF_CC_SET_COMMON_BITMASK(p_cc_pdb, ICF_CALL_MODIFY_RESP_FAIL)
               ret_val = icf_cc_send_pa_call_modify_resp(p_cc_pdb);
            }
        }
    }


    if ((ICF_CC_MMS_RESP_OFFER_REJECTED == p_cc_pdb->mms_resp_type) || 
        (ICF_CC_MMS_RESP_MOD_REJECTED == p_cc_pdb->mms_resp_type))
    {
        /*This is the first answer from remote and MMS failure received
        *clear call*/
        ret_val = icf_cc_initiate_call_clear(p_cc_pdb);
        ICF_CC_SET_NEXT_STATE(p_cc_pdb,ICF_CC_04_AWT_MMS_RESP)
    }
    else
    {
        ICF_CC_SET_NEXT_STATE(p_cc_pdb,ICF_CC_06_CONNECTED)
    }

    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb)
    return ret_val;
}


/******************************************************************************
 *
 * FUNCTION:        icf_cc_mms_resp_media_change_ack
 *
 * DESCRIPTION:     This function is invoked when MMS response is received
 *                  and ongoing event media_change_ack
 *
 *****************************************************************************/
icf_return_t icf_cc_mms_resp_media_change_ack(
	icf_cc_pdb_st      *p_cc_pdb)
{
    icf_return_t   ret_val = ICF_SUCCESS;

    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb)
 
    icf_cc_stop_current_timer(p_cc_pdb);

    if ((ICF_CC_MMS_RESP_OFFER_REJECTED == p_cc_pdb->mms_resp_type) || 
        (ICF_CC_MMS_RESP_MOD_REJECTED == p_cc_pdb->mms_resp_type))
    {
        /*This is the first answer from remote and MMS failure received
        *clear call*/
        ret_val = icf_cc_initiate_call_clear(p_cc_pdb);
    }
    else
    {
        icf_cc_update_applied_sdp_mode(p_cc_pdb);

        p_cc_pdb->common_bitmask |= ICF_CC_CALL_MODIFY_CFM_NT_REQD;

        if(ICF_FAILURE == icf_cc_send_pa_call_modify_ind(p_cc_pdb))
        {
            ret_val = ICF_FAILURE;
        }
        else if(ICF_NULL != ICF_CC_CHECK_PENDING_EVENT(p_cc_pdb,
                ICF_PENDING_EVENT_OFFER_CHANGE_WO_SDP_REQ))
        {
           ICF_CC_RESET_PENDING_EVENT(p_cc_pdb,
                   ICF_PENDING_EVENT_OFFER_CHANGE_WO_SDP_REQ)
                 
           p_cc_pdb->pa_call_mod_wo_media = ICF_TRUE;

           /*Send call_modify_ind to application*/
           ret_val = icf_cc_send_pa_call_modify_ind(p_cc_pdb);

           ICF_CC_SET_CALL_STATE(p_cc_pdb,ICF_CC_14_AWT_CALL_MODIFY_RESP)

/*#ifdef ICF_PATCH_FOR_OFFER_IN_200 
           icf_cc_handle_non_fs_reinvite(p_cc_pdb);
#endif*/
        }
        /* Handle added for OFFER Change as pending event, */
        else if(ICF_NULL != ICF_CC_CHECK_PENDING_EVENT(p_cc_pdb,
              ICF_PENDING_EVENT_OFFER_CHANGE_REQ))
        {
          ICF_CC_RESET_MODIFY_MEDIA_TYPE(p_cc_pdb)
            if (ICF_FAILURE == icf_cc_make_mms_type_for_remote_offer_change(p_cc_pdb))
            {
               ret_val = ICF_FAILURE;
            }
            else
            {
               /*If pending_event is offer_change, then send MMS request
                *for offer change and remain in the same state and set
                *next_state as 11_connect_ack*/
               ret_val = icf_cc_send_mms_req(p_cc_pdb);
               if (ICF_SUCCESS == ret_val)
               {
                  ICF_CC_SET_NEXT_STATE(p_cc_pdb,
                     ICF_CC_15_AWT_CALL_MODIFY_ALERT_RESP)
                  ICF_CC_SET_ONGOING_EVENT(p_cc_pdb,
                     ICF_CC_SSA_OFFER_CHANGE)
                  ICF_CC_RESET_PENDING_EVENT(p_cc_pdb,
                      ICF_PENDING_EVENT_OFFER_CHANGE_REQ)
                  p_cc_pdb->p_call_ctx->update_reqd = ICF_FALSE;
                  /*REL6.0: if any other pending_event raise alarm*/
                }   
                     
             }
        }   
        else
        {
            ICF_CC_SET_NEXT_STATE(p_cc_pdb,ICF_CC_06_CONNECTED)
        }
    }
    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb)
    return ret_val;
}


/******************************************************************************
 *
 * FUNCTION:        icf_cc_mms_resp_ssa_stop_fax_resp
 *
 * DESCRIPTION:     This function is invoked when MMS response is received
 *                  and ongoing event SSA FAX RESP
 *
 *****************************************************************************/
icf_return_t icf_cc_mms_resp_ssa_stop_fax_resp(
	icf_cc_pdb_st      *p_cc_pdb)
{
    icf_return_t   ret_val = ICF_SUCCESS;

    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb)

        icf_cc_stop_current_timer(p_cc_pdb);

    if ((ICF_CC_MMS_RESP_OFFER_REJECTED == p_cc_pdb->mms_resp_type) || 
            (ICF_CC_MMS_RESP_MOD_REJECTED == p_cc_pdb->mms_resp_type))
    {
        /*This is the first answer from remote and MMS failure received
         *clear call*/
        ret_val = icf_cc_initiate_call_clear(p_cc_pdb);
    }
    else
    {
        if(ICF_FAILURE == icf_cc_send_pa_stop_fax_resp(p_cc_pdb))
        { 
            ret_val = ICF_FAILURE;
        }
    }
    /* A call modify will not run */
    ICF_CC_SET_NEXT_STATE(p_cc_pdb,ICF_CC_06_CONNECTED)	;
    p_cc_pdb->p_call_ctx->fax_type = ICF_NO_FAX_CALL;
    ICF_CC_RESET_SIG_ONGOING(p_cc_pdb);
    ICF_CC_SET_STATUS(p_cc_pdb, ICF_STATUS_SIGNAL_ONGOING_ENDED);

    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb)
        return ret_val;
}


/******************************************************************************
 *
 * FUNCTION:        icf_cc_mms_resp_ssa_start_fax_resp
 *
 * DESCRIPTION:     This function is invoked when MMS response is received
 *                  and ongoing event SSA FAX RESP
 *
 *****************************************************************************/
icf_return_t icf_cc_mms_resp_ssa_start_fax_resp(
	icf_cc_pdb_st      *p_cc_pdb)
{
    icf_return_t   ret_val = ICF_SUCCESS;

    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb)

    icf_cc_stop_current_timer(p_cc_pdb);
	
    if ((ICF_CC_MMS_RESP_OFFER_REJECTED == p_cc_pdb->mms_resp_type) || 
        (ICF_CC_MMS_RESP_MOD_REJECTED == p_cc_pdb->mms_resp_type))
    {
        /*This is the first answer from remote and MMS failure received
        *clear call*/
        ret_val = icf_cc_initiate_call_clear(p_cc_pdb);
    }
	else if((ICF_NULL != ICF_CC_CHECK_PENDING_EVENT(p_cc_pdb,
                    ICF_PENDING_EVENT_PA_STOP_FAX_REQ))
        || (ICF_NULL != ICF_CC_CHECK_PENDING_EVENT(p_cc_pdb,
                    ICF_PENDING_EVENT_SSA_STOP_FAX_REQ)))
    {
		ICF_CC_SET_COMMON_BITMASK(p_cc_pdb, ICF_FAX_RESP_FAIL)
		p_cc_pdb->p_call_ctx->app_failure_cause = ICF_CAUSE_FAX_REQUEST_STOPPED;
		if(ICF_FAILURE == icf_cc_send_pa_fax_resp(p_cc_pdb))
		{ 
	            ret_val = ICF_FAILURE;
		}
		else if(ICF_FAILURE == icf_cc_make_revert_mms_fax_cancel(p_cc_pdb))
        {
            ret_val = ICF_FAILURE;
        }
        else
        {
			ICF_CC_SET_NEXT_STATE(p_cc_pdb,ICF_CC_04_AWT_MMS_RESP)
            if(ICF_NULL!= ICF_CC_CHECK_PENDING_EVENT(p_cc_pdb,
                                ICF_PENDING_EVENT_PA_STOP_FAX_REQ))
            {
                ICF_CC_SET_ONGOING_EVENT(p_cc_pdb,
                                    ICF_CC_PA_STOP_FAX_REQ)
                ICF_CC_RESET_PENDING_EVENT(p_cc_pdb,
                                    ICF_PENDING_EVENT_PA_STOP_FAX_REQ)
            }
            else if(ICF_NULL!= ICF_CC_CHECK_PENDING_EVENT(p_cc_pdb,
                                ICF_PENDING_EVENT_SSA_STOP_FAX_REQ))   
            {
                ICF_CC_SET_ONGOING_EVENT(p_cc_pdb,
                                       ICF_CC_SSA_STOP_FAX_REQ)
                   ICF_CC_RESET_PENDING_EVENT(p_cc_pdb,
                                       ICF_PENDING_EVENT_SSA_STOP_FAX_REQ)
            }
       }

    }
    else
    {
        if(ICF_NULL != ICF_CC_CHECK_COMMON_BITMASK(p_cc_pdb,
                    ICF_FAX_RESP_FAIL))
        {
            if(ICF_T38_FAX_CALL_FAILURE_SWITCH_PT_CALL ==
                p_cc_pdb->p_call_ctx->fax_type)
            {
                p_cc_pdb->p_call_ctx->fax_type = ICF_FAX_PT_CALL;
                if(ICF_FAILURE == icf_cc_ssa_send_update_msg(p_cc_pdb))
                {
                    ret_val = ICF_FAILURE;
                }
                else
                {
                    ICF_CC_SET_NEXT_STATE(p_cc_pdb,ICF_CC_14_AWT_CALL_MODIFY_RESP)
                }
            }
            else
            {
                if(ICF_FAILURE == icf_cc_send_pa_fax_resp(p_cc_pdb))
                {
                    ret_val = ICF_FAILURE;
                }
                else
                {
                    ICF_CC_SET_NEXT_STATE(p_cc_pdb,ICF_CC_06_CONNECTED)
                }
            }
	   
	   ICF_CC_RESET_COMMON_BITMASK(p_cc_pdb,ICF_FAX_RESP_FAIL)
        }
        else
        {
            /* code added for successfull t38 addition and 
             * fax type in start fax was ICF_T38_FAX_CALL_FAILURE_SWITCH_PT_CALL*/ 
            if(ICF_T38_FAX_CALL_FAILURE_SWITCH_PT_CALL ==
                p_cc_pdb->p_call_ctx->fax_type)
            {
                p_cc_pdb->p_call_ctx->fax_type = ICF_T38_FAX_CALL;
            }
            if(ICF_FAILURE == icf_cc_send_pa_fax_resp(p_cc_pdb))
            {
                ret_val = ICF_FAILURE;
            }
            else
            {
                ICF_CC_SET_NEXT_STATE(p_cc_pdb,ICF_CC_06_CONNECTED)
            }
        }
    }
	
    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb)
    return ret_val;
}

/******************************************************************************
 *
 * FUNCTION:        icf_cc_mms_resp_pa_stop_fax_req
 *
 * DESCRIPTION:     This function is used for handling the PA responses for 
 *                  stop fax after mms is done for reverting fax media. Also
 *                  if cancel is not send to network  it sends the same.
 *
 *****************************************************************************/
icf_return_t icf_cc_mms_resp_pa_stop_fax_req(
      INOUT icf_cc_pdb_st *p_cc_pdb)
{
     icf_return_t        ret_val = ICF_SUCCESS;
     icf_config_data_st  *p_config_data = ICF_NULL;
 
     ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /* function entry trace */
	ret_val = icf_cc_stop_current_timer(p_cc_pdb);  
	 /* if an outgoing call modify request handle here */
     /* Get pointer to config data from DBM */
	 ret_val = icf_dbm_get_module_glb_data(
                                     p_cc_pdb->p_glb_pdb,
                                     (icf_int_module_id_t)ICF_INT_MODULE_CFG,
                                     (icf_void_t *)&p_config_data, p_cc_pdb->p_ecode);

     /* check the call direction  */
	if ((ICF_CC_MMS_RESP_OFFER_REJECTED == p_cc_pdb->mms_resp_type) 
		||(ICF_CC_MMS_RESP_MOD_REJECTED == p_cc_pdb->mms_resp_type))
    {
	ICF_CC_SET_COMMON_BITMASK(p_cc_pdb, ICF_FAX_RESP_FAIL)
        p_cc_pdb->p_call_ctx->app_failure_cause = ICF_CAUSE_FAX_REQUEST_STOPPED;
              if(ICF_FAILURE == icf_cc_send_pa_fax_resp(p_cc_pdb))
               {  
                 ret_val = ICF_FAILURE;
               }
  
	ICF_CC_SET_COMMON_BITMASK(p_cc_pdb, ICF_FAX_RESP_FAIL)
        p_cc_pdb->p_call_ctx->app_failure_cause = ICF_CAUSE_MEDIA_FAILURE;
              icf_cc_send_pa_stop_fax_resp(p_cc_pdb);

        /*This is the first answer from remote and MMS failure received
        *clear call*/
        ret_val = icf_cc_initiate_call_clear(p_cc_pdb);
    }
     else 
      {
		   	if(ICF_NULL != ICF_CC_CHECK_COMMON_BITMASK(p_cc_pdb,
            		ICF_FAX_CALL_ESTABLISHED))
			{
				ret_val = icf_dbm_get_module_glb_data(
                                     p_cc_pdb->p_glb_pdb,
                                     (icf_int_module_id_t)ICF_INT_MODULE_CFG,
                                     (icf_void_t *)&p_config_data, p_cc_pdb->p_ecode);

				if(ICF_SUCCESS == ret_val)
    			{
        			if(ICF_FAILURE == icf_cc_start_setup_timer(p_cc_pdb))
        			{
            			ret_val = ICF_FAILURE;
        			}
        			else if(ICF_FAILURE == icf_cc_ssa_send_update_msg(p_cc_pdb))
        			{
            			ret_val = ICF_FAILURE;
        			}
        			else
        		{
            			if(ICF_CFG_CALL_MODIFY_METHOD_INVITE ==
                					p_config_data->call_modify_req_method)
            			{
                			ICF_CC_SET_NEXT_STATE(p_cc_pdb,ICF_CC_15_AWT_CALL_MODIFY_ALERT_RESP)
            			}
            			else
            			{
                			ICF_CC_SET_NEXT_STATE(p_cc_pdb,ICF_CC_14_AWT_CALL_MODIFY_RESP)
            			}
				ICF_CC_SET_SIG_ONGOING(p_cc_pdb)
        			}
    			}
			}
			else  if ((ICF_TRUE == p_cc_pdb->p_call_ctx->stop_fax_resp_recvd) ||
                (p_cc_pdb->p_call_ctx->fax_type == ICF_NULL))
           	{
              /* this is added to send the responses to PA for the stop fax, start fax */
              /* case when stop fax req is send immediately after start fax , after mms resp */
              /* we reach here and need to send the resp to PA */
              /* Flag check ->Here we have already recvd the 200OK for the cancel send earlier*/
              /* Now only the responses to the PA are send */
              ICF_CC_SET_NEXT_STATE(p_cc_pdb,ICF_CC_06_CONNECTED)
              ICF_CC_SET_COMMON_BITMASK(p_cc_pdb, ICF_FAX_RESP_FAIL)
              p_cc_pdb->p_call_ctx->app_failure_cause = ICF_CAUSE_FAX_REQUEST_STOPPED;
              if(ICF_FAILURE == icf_cc_send_pa_fax_resp(p_cc_pdb))
               {
                 ret_val = ICF_FAILURE;
               }
              icf_cc_send_pa_stop_fax_resp(p_cc_pdb);
           	}
			else
           	{
            	if((ICF_CFG_CALL_MODIFY_METHOD_INVITE ==
                     			p_config_data->call_modify_req_method) &&
                      			(ICF_CALL_MODIFY_PROV_RESP_RECVD &
                      			p_cc_pdb->p_call_ctx->prov_resp_status))
                {
                     /* call this function for sending CANCEL on network */
                     icf_cc_send_ssa_cancel_req(p_cc_pdb);
                     ICF_CC_SET_SIG_ONGOING(p_cc_pdb)
                }
            }
       }
       ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */
       return ret_val;
} 

/******************************************************************************
 *
 * FUNCTION:        icf_cc_mms_resp_ssa_stop_fax_req
 *
 * DESCRIPTION:     This function is used for handling the SSA responses for 
 *                  stop fax after mms is done for reverting fax media.
 *
 *****************************************************************************/
icf_return_t icf_cc_mms_resp_ssa_stop_fax_req(
      INOUT icf_cc_pdb_st *p_cc_pdb)
{
    icf_return_t        ret_val = ICF_SUCCESS;

    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb); /* function entry trace */
    icf_cc_stop_current_timer(p_cc_pdb);
    /* check the call direction  */

    if ((ICF_CC_MMS_RESP_OFFER_REJECTED == p_cc_pdb->mms_resp_type) || 
            (ICF_CC_MMS_RESP_MOD_REJECTED == p_cc_pdb->mms_resp_type))
    {
        /*This is the first answer from remote and MMS failure received
         *clear call*/
        ICF_PRINT(((icf_uint8_t *)" Failure response comes from MMI, so sending error and clearing call"))
        p_cc_pdb->p_call_ctx->offer_failure_reason =  ICF_CALL_CLEAR_MEDIA_MISMATCH;
        ret_val = icf_cc_send_ssa_offer_chg_err(p_cc_pdb);
        ret_val = icf_cc_04_generic_err(p_cc_pdb);
    }
    else if (ICF_NULL != ICF_CC_CHECK_COMMON_BITMASK(p_cc_pdb,
            		                             ICF_STOP_FAX_RESP_ALREADY_GIVEN))
    { 
        /* Control will be here only in case of galre handling for stop_fax_req,
         * This bitmask is set means IPTK has already given stop_fax_resp to
         * application, so there is no need to given stop_fax_ind.
         */
        ICF_CC_RESET_COMMON_BITMASK(p_cc_pdb,
                                    ICF_STOP_FAX_RESP_ALREADY_GIVEN)

        p_cc_pdb->p_call_ctx->media_updated = ICF_TRUE;

        icf_cc_update_applied_sdp_mode(p_cc_pdb);

        /*Update local SDP Codec from Applied SDP*/
        if (ICF_FAILURE == icf_cc_update_local_sdp(p_cc_pdb))
        {
            ret_val = ICF_FAILURE;
        }
        else if(ICF_FAILURE ==  icf_cc_send_ssa_offer_chg_rsp(p_cc_pdb))
        {
            ret_val = ICF_FAILURE;
        }
        ICF_CC_SET_NEXT_STATE(p_cc_pdb,ICF_CC_06_CONNECTED)	;
        p_cc_pdb->p_call_ctx->fax_type = ICF_NO_FAX_CALL;
        ICF_CC_RESET_SIG_ONGOING(p_cc_pdb);
        ICF_CC_SET_STATUS(p_cc_pdb, ICF_STATUS_SIGNAL_ONGOING_ENDED);

    } 
    else if(ICF_NULL == ICF_CC_CHECK_COMMON_BITMASK(p_cc_pdb,
            		ICF_FAX_CALL_ESTABLISHED))
    {
        ICF_PRINT(((icf_uint8_t *)" Its the cancel resposnse, so setting 06 as next state"))
        ICF_CC_SET_NEXT_STATE(p_cc_pdb,ICF_CC_06_CONNECTED);
        ret_val = ICF_SUCCESS;
    }

    else
    {
        ICF_PRINT(((icf_uint8_t *)" SUCCESS response comes from MMI, Sending STOP_FAX_IND to app"\
                    "and 200 OK to network"))
            p_cc_pdb->p_call_ctx->media_updated = ICF_TRUE;

        icf_cc_update_applied_sdp_mode(p_cc_pdb);

        /*Update local SDP Codec from Applied SDP*/
        if (ICF_FAILURE == icf_cc_update_local_sdp(p_cc_pdb))
        {
            ret_val = ICF_FAILURE;
        }
        else if(ICF_FAILURE ==  icf_cc_send_ssa_offer_chg_rsp(p_cc_pdb))
        {
            ret_val = ICF_FAILURE;
        }
        else if (ICF_FAILURE == icf_cc_send_pa_stop_fax_ind(p_cc_pdb))
        {
            ret_val = ICF_FAILURE;
        }
        ICF_CC_SET_NEXT_STATE(p_cc_pdb,ICF_CC_06_CONNECTED)	;
        p_cc_pdb->p_call_ctx->fax_type = ICF_NO_FAX_CALL;
        ICF_CC_RESET_SIG_ONGOING(p_cc_pdb);
        ICF_CC_SET_STATUS(p_cc_pdb, ICF_STATUS_SIGNAL_ONGOING_ENDED);


    }
    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb); /* function exit trace */
    return ret_val;
} 

/******************************************************************************
 *
 * FUNCTION:        icf_cc_04_pa_stop_fax_req
 *
 * DESCRIPTION:     This function is used for handling the scenario when 
 *                  immediately after sending the start fax req , application
 *                  sends the cancel fax request. Here we check the ongoing
 *                  event for start fax and put the stop fax as pending event.
 *
 *****************************************************************************/
icf_return_t icf_cc_04_pa_stop_fax_req(
       INOUT icf_cc_pdb_st *p_cc_pdb)
{
    icf_return_t ret_val = ICF_SUCCESS;
    icf_uint8_t      ongoing_event = 0;

    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /* function entry trace */
   
    ongoing_event =  ICF_CC_GET_ONGOING_EVENT(p_cc_pdb);

    if(ICF_CC_PA_FAX_REQ == ongoing_event)
    {
      	ICF_CC_SET_PENDING_EVENT(p_cc_pdb,
            ICF_PENDING_EVENT_PA_STOP_FAX_REQ)
    }
	else if(ICF_CC_SSA_FAX_RESP == ongoing_event)
	{
		ICF_CC_SET_PENDING_EVENT(p_cc_pdb,
            ICF_PENDING_EVENT_PA_STOP_FAX_REQ)
	}
    else if ( (ICF_CC_PA_CALL_MODIFY_REQ == ongoing_event) ||
            (ICF_CC_SSA_OFFER_CHANGE == ongoing_event) ||
            (ICF_CC_MEDIA_UPDATE_RESP == ongoing_event))
    {
        ICF_CC_SET_COMMON_BITMASK(p_cc_pdb, ICF_FAX_RESP_FAIL);
        p_cc_pdb->p_call_ctx->app_failure_cause = ICF_CAUSE_CALL_MODIFY_REQ_PENDING;
        ret_val = icf_cc_send_pa_stop_fax_resp(p_cc_pdb);
    }

    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */
    return ret_val;
} 

/******************************************************************************
 *
 * FUNCTION:        icf_cc_04_ssa_fax_resp
 *
 * DESCRIPTION:      
 *
 *****************************************************************************/
icf_return_t icf_cc_04_ssa_fax_resp(
       INOUT icf_cc_pdb_st *p_cc_pdb)
{
    icf_return_t ret_val = ICF_SUCCESS;
    icf_uint8_t      ongoing_event = 0;

    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /* function entry trace */
   
    ongoing_event =  ICF_CC_GET_ONGOING_EVENT(p_cc_pdb);

    if(ICF_CC_PA_STOP_FAX_REQ == ongoing_event)
    {
	ICF_CC_SET_COMMON_BITMASK(p_cc_pdb, ICF_FAX_CALL_ESTABLISHED)
	icf_cc_free_ssa_msg(p_cc_pdb);
    }

    p_cc_pdb->p_call_ctx->app_failure_cause = ICF_CAUSE_REJECT_MEDIA_CHANGE_REQ;
            ICF_CC_SET_COMMON_BITMASK(p_cc_pdb, ICF_FAX_RESP_FAIL)
	icf_cc_send_pa_fax_resp(p_cc_pdb);
	
    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */
    return ret_val;
} 

/******************************************************************************
 *
 * FUNCTION:        icf_cc_mms_resp_pa_media_connect
 *
 * DESCRIPTION:     This function is invoked when MMS response is received
 *                  and ongoing event Media connect from application
 *
 *****************************************************************************/
icf_return_t icf_cc_mms_resp_pa_media_connect(
	icf_cc_pdb_st      *p_cc_pdb)
{
    icf_return_t   ret_val = ICF_SUCCESS;
    icf_return_t   dummy_ret_val = ICF_SUCCESS;

    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /* function entry trace */

    /*It is MMS for call modify request from application
     *if MMS is failed, send failure call_modify_resp to app*/
        /* If this bitmask is set then generate ALARM and ignore this message.
         */
    if (ICF_CC_MMS_RESP_OFFER_REJECTED == p_cc_pdb->mms_resp_type)
    {
        /*This is the first answer from remote and MMS failure received
         *clear call*/
        p_cc_pdb->p_call_ctx->app_failure_cause = ICF_CAUSE_MEDIA_FAILURE;
        p_cc_pdb->p_call_ctx->release_cause = 
            ICF_CALL_MODIFY_REASON_REJECT_MEDIA_CHANGE;

        ret_val = icf_cc_04_generic_err(p_cc_pdb);
        dummy_ret_val = ICF_FAILURE;
        ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb)
        return ret_val;
    }
    else if (ICF_CC_MMS_RESP_MOD_REJECTED == p_cc_pdb->mms_resp_type)
    {
        ret_val = icf_cc_back_local_sdp(p_cc_pdb);
        dummy_ret_val = ICF_FAILURE;
    }
    else if(ICF_SUCCESS == dummy_ret_val)
    {

#ifdef ICF_QOS_SUPPORT
        ICF_CHECK_QOS_SUPPORT_START(p_cc_pdb->p_glb_pdb,ICF_FALSE)
	    /* check the call type and stream list to take further action */
    /* Send media param to the SSA */
    if (ICF_FAILURE == icf_cc_send_ssa_media_param(p_cc_pdb))
    {
        ret_val = ICF_FAILURE;
    } /* if (ICF_FAILURE == icf_c... */
    else
    {
        ICF_CC_TRACE(p_cc_pdb, ICF_TRACE_PRACK_REQD)
        if (ICF_TRUE == p_cc_pdb->prack_reqd)
        {
            /* Update the states in the call context */
            ICF_CC_SET_NEXT_STATE(p_cc_pdb,ICF_CC_09_AWT_MEDIA_PARAM_PRACK)
            ICF_CC_SET_COMMON_BITMASK(p_cc_pdb,ICF_PRACK_FOR_MEDIA_CONNECT)
        } /* if (ICF_TRUE == p_cc_pdb->p... */
        else
        {
            
             /* if call_connect is pending event send connect 200 OK */
            if (ICF_PENDING_EVENT_PA_CALL_CONNECT == ICF_CC_CHECK_PENDING_EVENT(\
                        p_cc_pdb,ICF_PENDING_EVENT_PA_CALL_CONNECT) &&
                ICF_SUCCESS == ret_val)
            {
                /*
                 * Send a  call connect request to SSA
                 */
                if(ICF_FAILURE == icf_cc_snd_mms_pa_connect(p_cc_pdb))
                {
                    ret_val = ICF_FAILURE;
                }
                else
                {
                    /*
                     * Update the states in the call context
                     */
                    if(ICF_TRUE == p_cc_pdb->mms_sent)
                    {

                        ICF_CC_SET_CALL_STATE(p_cc_pdb, ICF_CC_04_AWT_MMS_RESP)
                        ICF_CC_SET_ONGOING_EVENT(p_cc_pdb,ICF_CC_PA_CALL_CONNECT)
                        ICF_CC_SET_NEXT_STATE(p_cc_pdb, ICF_CC_11_AWT_SSA_CONNECT_ACK)
                    }
                }

                /* Update the states in the call context */
            } /* else */
            else
            {
                /* Update the states in the call context */
                ICF_CC_SET_NEXT_STATE(p_cc_pdb,ICF_CC_05_AWT_CONNECT)
            }

        } /* else */
    } /* else */
ICF_CHECK_QOS_SUPPORT_END
#else
    /* Send media param to the SSA */
    if (ICF_FAILURE == icf_cc_send_ssa_media_param(p_cc_pdb))
    {
        ret_val = ICF_FAILURE;
    } /* if (ICF_FAILURE == icf_c... */
    else
    {
        ICF_CC_TRACE(p_cc_pdb, ICF_TRACE_PRACK_REQD)

        if (ICF_TRUE == p_cc_pdb->prack_reqd)
        {
            /* Update the states in the call context */
            ICF_CC_SET_NEXT_STATE(p_cc_pdb,ICF_CC_09_AWT_MEDIA_PARAM_PRACK)
            ICF_CC_SET_COMMON_BITMASK(p_cc_pdb,ICF_PRACK_FOR_MEDIA_CONNECT)
        } /* if (ICF_TRUE == p_cc_pdb->p... */
        else
        {
            /* if call_connect is pending event send connect 200 OK */
            if (ICF_PENDING_EVENT_PA_CALL_CONNECT == ICF_CC_CHECK_PENDING_EVENT\
                    (p_cc_pdb,ICF_PENDING_EVENT_PA_CALL_CONNECT) && 
                    ICF_SUCCESS == ret_val)
            {
                /*
                 * Send a  call connect request to SSA
                 */
                if(ICF_FAILURE == icf_cc_snd_mms_pa_connect(p_cc_pdb))
                {
                    ret_val = ICF_FAILURE;
                }
                else
                {
                    /*
                     * Update the states in the call context
                     */
                    if(ICF_TRUE == p_cc_pdb->mms_sent)
                    {

                        ICF_CC_SET_CALL_STATE(p_cc_pdb, ICF_CC_04_AWT_MMS_RESP)
                        ICF_CC_SET_ONGOING_EVENT(p_cc_pdb,ICF_CC_PA_CALL_CONNECT)
                        ICF_CC_SET_NEXT_STATE(p_cc_pdb, ICF_CC_11_AWT_SSA_CONNECT_ACK)
                    }
                }

                /* Update the states in the call context */
            } /* else */
            else
            {
                /* Update the states in the call context */
                ICF_CC_SET_NEXT_STATE(p_cc_pdb,ICF_CC_05_AWT_CONNECT)
            }
        } /* else */
    }
#endif
    }
    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */

    return ret_val;
}
#ifdef ICF_SESSION_TIMER
/******************************************************************************
 *
 * FUNCTION:        icf_cc_mms_resp_ssa_session_refresh_resp
 *
 * DESCRIPTION:     This function is invoked when MMS response is received
 *                  and ongoing event is SSA Offer Change response
 *
 *****************************************************************************/
icf_return_t icf_cc_mms_resp_ssa_session_refresh_resp
(
    icf_cc_pdb_st      *p_cc_pdb
)
{
   icf_return_t   ret_val = ICF_SUCCESS;

   ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb)

   icf_cc_stop_current_timer(p_cc_pdb);

   /*if MMS response is failure for any stream in this case
    *means media can't be revert back and hence clear the call*/

    if ((ICF_CC_MMS_RESP_OFFER_REJECTED == p_cc_pdb->mms_resp_type) ||
        (ICF_CC_MMS_RESP_MOD_REJECTED == p_cc_pdb->mms_resp_type))
     { 
         ret_val = icf_cc_initiate_call_clear(p_cc_pdb);
     }
    else
     {
         icf_cc_update_applied_sdp_mode(p_cc_pdb);
         p_cc_pdb->common_bitmask |= ICF_CC_CALL_MODIFY_CFM_NT_REQD;
         if(ICF_FAILURE == icf_cc_send_pa_call_modify_ind(p_cc_pdb))
         {
             ret_val = ICF_FAILURE;
         }
     }
     ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb)

     return ret_val;
}
#endif


/*call_mod_cancel*/
/******************************************************************************
 *
 * FUNCTION:        icf_cc_04_pa_call_modify_cancel_req
 *
 * DESCRIPTION:     This function is used for handling the scenario when
 *                  immediately after sending the call modify req , application
 *                  sends the call modify cancel request. Here we check the ongoing
 *                  event for call modify req and put the call mod cancel as 
 *                  pending event.
 *
 *****************************************************************************/
icf_return_t icf_cc_04_pa_call_modify_cancel_req(
       INOUT icf_cc_pdb_st *p_cc_pdb)
{
    icf_return_t ret_val = ICF_SUCCESS;
    icf_uint8_t      ongoing_event = 0;

    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /* function entry trace */

    ongoing_event =  ICF_CC_GET_ONGOING_EVENT(p_cc_pdb);

    if(ICF_CC_PA_CALL_MODIFY_REQ == ongoing_event)
    {
        /*set the common_bimask to ICF_CALL_MOD_CANCEL_REQ as it will 
         *indicate after mms complete send success response for call modify cancel 
         *request followed by failure for call modify request*/ 

        ICF_CC_SET_COMMON_BITMASK_2(p_cc_pdb, ICF_CALL_MOD_CANCEL_REQ)
        ICF_CC_SET_PENDING_EVENT(p_cc_pdb,
        ICF_PENDING_EVENT_PA_CALL_MOD_CANCEL_REQ)

    }
    else if((ICF_CC_MEDIA_UPDATE_RESP == ongoing_event) ||
            (ICF_CC_ONGOING_EVENT_LOCAL_MEDIA_REVERT == ongoing_event)) 
    {
       /*Case when call modify cancel is received when 200 KK of ReInvite is already
         received and media updation is going on*/

       /*Send failure response for call modify cancel request*/
       ICF_CC_SET_COMMON_BITMASK(p_cc_pdb, ICF_CALL_MOD_CANCEL_RESP_FAIL)

       p_cc_pdb->p_call_ctx->app_failure_cause =
                          ICF_CAUSE_CALL_MODIFY_TRANSACTION_COMPLETE;

       /*Send call modify_cancel fail resp to application*/
       if(ICF_FAILURE == icf_cc_send_pa_call_modify_cancel_resp(p_cc_pdb))
       {
           ret_val = ICF_FAILURE;
       }
    }

    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */
    return ret_val;
}


/******************************************************************************
 *
 * FUNCTION:        icf_cc_mms_resp_ssa_call_modify_cancel_req
 *
 * DESCRIPTION:     This function is invoked when MMS response is received
 *                  media_revert and ongoing event is cancel reinvite from
 *                  network.This function sends call modify cancel indication
 *                  to PA and sends response(487) for reinvite.
 *
 *****************************************************************************/
icf_return_t icf_cc_mms_resp_ssa_call_modify_cancel_req(
    icf_cc_pdb_st      *p_cc_pdb)
{
    icf_return_t   ret_val = ICF_SUCCESS;
    icf_uint8_t      ongoing_event = 0;

    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb)

    ongoing_event =  ICF_CC_GET_ONGOING_EVENT(p_cc_pdb);

   icf_cc_stop_current_timer(p_cc_pdb);

    if ((ICF_CC_MMS_RESP_OFFER_REJECTED == p_cc_pdb->mms_resp_type) || 
        (ICF_CC_MMS_RESP_MOD_REJECTED == p_cc_pdb->mms_resp_type))
    {
        /*This is the first answer from remote and MMS failure received
        *clear call*/
        ret_val = icf_cc_initiate_call_clear(p_cc_pdb);
        ICF_CC_SET_NEXT_STATE(p_cc_pdb,ICF_CC_04_AWT_MMS_RESP)
    }
    else
    {
        ICF_CC_SET_NEXT_STATE(p_cc_pdb,ICF_CC_06_CONNECTED)
    }

    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb)
    return ret_val;
}


/******************************************************************************
 *
 * FUNCTION:        icf_cc_pa_call_mod_cancel_req_ongoing_event_handling
 *
 * DESCRIPTION:     This function is invoked when MMS response is received
 *                  and ongoing event is cancel call modify request from PA. 
 *
 *****************************************************************************/
icf_return_t icf_cc_pa_call_mod_cancel_req_ongoing_event_handling(
    icf_cc_pdb_st      *p_cc_pdb)
{
    icf_return_t     ret_val = ICF_SUCCESS;

    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /* function entry trace */
         
    icf_cc_stop_current_timer(p_cc_pdb);

    if ((ICF_CC_MMS_RESP_OFFER_REJECTED == p_cc_pdb->mms_resp_type) || 
        (ICF_CC_MMS_RESP_MOD_REJECTED == p_cc_pdb->mms_resp_type))
    {
        /*This is the first answer from remote and MMS failure received
        *clear call*/
        ret_val = icf_cc_initiate_call_clear(p_cc_pdb);
        ICF_CC_SET_NEXT_STATE(p_cc_pdb,ICF_CC_04_AWT_MMS_RESP)
    }
    else if(ICF_NULL != 
       ICF_CC_CHECK_COMMON_BITMASK_2(p_cc_pdb,ICF_CALL_MOD_CANCEL_REQ))
    {
        /*send cancel to NW */
       if (ICF_FAILURE == icf_cc_send_ssa_cancel_req(p_cc_pdb))
       {
           ret_val = ICF_FAILURE;
       }
       else
       {
           icf_cc_start_setup_timer(p_cc_pdb);
       }
    }
    else if(ICF_NULL !=
    ICF_CC_CHECK_COMMON_BITMASK_2(p_cc_pdb, ICF_CALL_MOD_CANCEL_REINV_REQ))
    {
       /*This is the case when 200 OK of Re-Invite is rcvd when media revert was
       going on for cancel call modify req.Since media is already reverted back
       hence sent 2nd re-invite to n/w with present media as new offer*/

       /*Send Re-Invite again*/
       if (ICF_FAILURE == icf_cc_ssa_send_update_msg(p_cc_pdb))
       {
           ret_val = ICF_FAILURE;
       }
       else
       {
           ret_val = ICF_SUCCESS;
       }
    }
    else
    {
      /*This is the case when media revert for cancel call modify request
      is going on(CANCEL is not sent)and 4xx resp of Re-Invite is rcvd*/

      /*Send call modify_cancel_resp to application*/
      if(ICF_FAILURE == icf_cc_send_pa_call_modify_cancel_resp(p_cc_pdb))
      {
          ret_val = ICF_FAILURE;
      }

      /*Call modify cancel response is followed by call modify req fail response*/
      p_cc_pdb->p_call_ctx->app_failure_cause =
                          ICF_CAUSE_REQUEST_CANCELLED_BY_USER;
      ICF_CC_SET_COMMON_BITMASK(p_cc_pdb, ICF_CALL_MODIFY_RESP_FAIL)

      if(ICF_FAILURE == icf_cc_send_pa_call_modify_resp(p_cc_pdb))
      {
         ret_val = ICF_FAILURE;
      }
      ICF_CC_SET_NEXT_STATE(p_cc_pdb, ICF_CC_06_CONNECTED)
   }
   ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb)
   return ret_val;
}

/******************************************************************************
 *
 * FUNCTION:        icf_cc_ssa_call_mod_cancel_resp_ongoing_event_handling
 *
 * DESCRIPTION:     This function is invoked when MMS response is received
 *                  and ongoing event is cancel call modify resp from SSA.
 *
 *****************************************************************************/
icf_return_t icf_cc_ssa_call_mod_cancel_resp_ongoing_event_handling(
    icf_cc_pdb_st      *p_cc_pdb)
{

   icf_return_t     ret_val = ICF_SUCCESS;

   ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /* function entry trace */

    if ((ICF_CC_MMS_RESP_OFFER_REJECTED == p_cc_pdb->mms_resp_type) || 
        (ICF_CC_MMS_RESP_MOD_REJECTED == p_cc_pdb->mms_resp_type))
    {
        /*This is the first answer from remote and MMS failure received
        *clear call*/
        ret_val = icf_cc_initiate_call_clear(p_cc_pdb);
        ICF_CC_SET_NEXT_STATE(p_cc_pdb,ICF_CC_04_AWT_MMS_RESP)
    }
    else if(ICF_NULL == 
      ICF_CC_CHECK_COMMON_BITMASK_2(p_cc_pdb,ICF_CALL_MOD_CANCEL_REQ))
   {
       /*If this bitmask is already reset it 
       means 200 OK of Re-Invite is received
       before 200 OK of CANCEL and hence revert the media only*/

       ICF_CC_SET_NEXT_STATE(p_cc_pdb,ICF_CC_06_CONNECTED)
       ICF_CC_RESET_PENDING_EVENT(p_cc_pdb,
                       ICF_PENDING_EVENT_PA_CALL_MOD_CANCEL_REQ)
       ICF_CC_RESET_ONGOING_EVENT(p_cc_pdb)

       /*Send failure response for call modify cancel request*/
       ICF_CC_SET_COMMON_BITMASK(p_cc_pdb, ICF_CALL_MOD_CANCEL_RESP_FAIL)

       p_cc_pdb->p_call_ctx->app_failure_cause =
                          ICF_CAUSE_CALL_MODIFY_TRANSACTION_COMPLETE;

       /*Send call modify_cancel fail resp to application*/
       if(ICF_FAILURE == icf_cc_send_pa_call_modify_cancel_resp(p_cc_pdb))
       {
           ret_val = ICF_FAILURE;
       }

    }
    else
    {
        /*This is the case when call modify cancel req comes during mms(Re-Inv not
        sent to N/W,hence after reverting the media send the response to PA*/
        /*This bitmask is reset in this function after sending response*/
        if(ICF_FAILURE == icf_cc_send_pa_call_modify_cancel_resp(p_cc_pdb))
        {
           ret_val = ICF_FAILURE;
        }

        if(ICF_FAILURE != ret_val)
        {
           /* Call modify cancel success response is followed by call modify
              req fail response*/
           p_cc_pdb->p_call_ctx->app_failure_cause =
                         ICF_CAUSE_REQUEST_CANCELLED_BY_USER;
           ICF_CC_SET_COMMON_BITMASK(p_cc_pdb, ICF_CALL_MODIFY_RESP_FAIL)

           if(ICF_FAILURE == icf_cc_send_pa_call_modify_resp(p_cc_pdb))
           {
              ret_val = ICF_FAILURE;
           }
           else
           {
              ICF_CC_SET_NEXT_STATE(p_cc_pdb,ICF_CC_06_CONNECTED)
              ret_val = ICF_SUCCESS;
           }
        }
    }
    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb)
    return ret_val;
}
