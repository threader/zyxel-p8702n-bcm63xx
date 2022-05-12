/*------------------------------------------------------------------------------
 *
 * File name        : icf_cc_fsm_11.c
 *
 * Purpose          : This file contains the event handlers in
 *                    ICF_CC_11_AWT_SSA_CONNECT_ACK state.
 *
 * Revision History :
 *
 * Date         Name            Ref#                Comments
 * --------     ------------    ----                ---------------
 * 13-Dec-2004  Rohit Aggarwal  Non Service LLD     Beginning of coding
 * 21-Dec-2004  Rohit Aggarwal                      Added ecode for call fail, 
 *                                                  and stats
 * 23-Dec-2004  Rohit Aggarwal                      Addition of ecodes, traces
 * 24-Dec-2004  Rohit Aggarwal                      Compilation fixes
 * 13-Apr-2005  Sumit Gupta     SPR 7386            Removed the function for 
 *                                                  handling cancel 
 *                                                  ICF_CC_11_AWT_SSA_CONNECT
 *                                                  _ACK
 * 20-Apr-2005  Sumit Gupta      SPR 7393           CALL_CONNECT_DONE flag is 
 *                                                  set as soon as connect ack
 *                                                  is received
 * 20-Apr-2005  Sumit Gupta      SPR 7427           Set call clear reason as 
 *                                                  ICF_CALL_CLEAR_MEDIA_MISM
 *                                                  ATCH when ack was received 
 *                                                  for incomming NFS call
 *                                                  without SDP and media was 
 *                                                  not committed
 * 19-May-2005 Sumit Gupta       SPR 7589           We would set our state to 
 *                                                  Awiat MMS even if send MMS 
 *                                                  returns failure when ACK is 
 *                                                   received with SDP
 * 25-May-2005 Sumit Gupta       SPR 7440            Updated Stats
 * 26-May-2005 Sumit Gupta       SPR 7559            Remote hold indication 
 *                                                   would be sent to PA in case
 *                                                   ACK is received for a FS 
 *                                                   call or media was committed
 *                                                   and media state was 
 *                                                   remote_held
 * 10-Aug-2005	Rohit Aggarwal	ICF					Code changes specific to ICF
 * 08-Mar-2006  Aman Aggarwal						ICF merging with ICF2.0        
 * 26-Jun-2006  Amit Sharma     ICF 6.0 APIs        Structure changes for multiple
 *                                                  m-lines
 * 25-Jul-2006  Umang Singh	    ICF 6.0         Multiple m-lines
 * 05-Apr-2007  Abhishek Dhammawat SPR14867         Modified icf_cc_11_ssa_connect_ack
 *                                                  for ongoing flag reseting
 *
 * 04-Jul-2008  Anuradha Gupta   SPR 18761          Modified icf_cc_11_timer_expiry
 *                                                  for signalling ongoing flag reset
 * 09-Dec-2008  Ashutosh Mohan   SPR 19218          Merged CSR-1-6239428 
 * 12-Feb-2009  Tarun Gupta      Rel 8.1 LLD        SDP Tunnelling Enhancements
 * 25-May-2009  Anuradha Gupta   SPR 19672          Merged CSR-1-6779001
 * 26-May-2009  Anuradha Gupta   SPR 19672          Merged CSR-1-6727350
 *
 * Copyright (c) 2009, Aricent.
 *----------------------------------------------------------------------------*/


#include "icf_ecode.h"
#include "icf_cc_prototypes.h"
#include "icf_cc_macro.h"


/*******************************************************************************
 *
 * FUNCTION:        icf_cc_11_pa_call_term_req
 *
 * DESCRIPTION:     This function is invoked when the CC is awaiting ack for 
 *                  Connect sent to SSA for an incoming call and it receives 
 *                  Call Terminate Request from PA.
 *                  Current State: ICF_CC_11_AWT_SSA_CONNECT_ACK
 *                  Current Event: ICF_CC_PA_CALL_TERMINATE_REQ
 *
 ******************************************************************************/
icf_return_t icf_cc_11_pa_call_term_req(
        INOUT icf_cc_pdb_st *p_cc_pdb)
{
    icf_return_t                     ret_val = ICF_SUCCESS;
   
    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /* function entry trace */

    /* Set release type as PA initiated */
    ICF_CC_SET_RELEASE_TYPE(p_cc_pdb, ICF_REL_TYPE_PA_INITIATED)
    ICF_CC_TRACE(p_cc_pdb, ICF_TRACE_REL_TYPE_PA_INIT)

    /* Initiate the call clear procedure */
    ret_val = icf_cc_11_call_term_handler(p_cc_pdb);

    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */
    return ret_val;
}


/******************************************************************************
 *
 * FUNCTION:        icf_cc_11_ssa_call_term_req
 *
 * DESCRIPTION:     This function is invoked when the CC is waitiug for ACK
 *					of 200 Ok 
 *                  and it receives Call Terminate Request from N/W.
 *                  Current State: ICF_CC_11_AWT_SSA_CONNECT_ACK
 *                  Current Event: ICF_CC_SSA_CALL_TERMINATE_REQ
 *
 *****************************************************************************/
icf_return_t icf_cc_11_ssa_call_term_req(
        INOUT icf_cc_pdb_st *p_cc_pdb)
{

    icf_return_t     ret_val = ICF_SUCCESS;

    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /* function entry trace */

    /*
     * As the handling for this event is the same in the state
     * ICF_CC_06_CONNECTED,call their
     * state handler
     */
    ret_val = icf_cc_06_ssa_call_term_req(p_cc_pdb);
    
    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */
    return ret_val;
}


/*******************************************************************************
 *
 * FUNCTION:        icf_cc_11_ssa_offer_change
 *
 * DESCRIPTION:     This function is invoked when the CC is awaiting ack for 
 *                  Connect sent to SSA for an incoming call and it receives 
 *                  session param change from SSA (SIP UPDATE/RE-INVITE recd).
 *                  Current State: ICF_CC_11_AWT_SSA_CONNECT_ACK
 *                  Current Event: ICF_CC_SSA_OFFER_CHANGE
 *
 ******************************************************************************/
icf_return_t icf_cc_11_ssa_offer_change(
        INOUT icf_cc_pdb_st *p_cc_pdb)
{
    icf_return_t                     ret_val = ICF_SUCCESS;
    icf_nw_inc_media_change_req_st  *p_nw_inc_media_change_req = ICF_NULL;

    p_nw_inc_media_change_req  = (icf_nw_inc_media_change_req_st *)
                                                                p_cc_pdb->p_internal_msg->p_msg_data;
    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /* function entry trace */

    /*
     * Once Media modify request for PA Connect is sent we ouldn't accept any
     * update request till the call is connected*/

    p_cc_pdb->p_call_ctx->offer_failure_reason = ICF_REQUEST_PENDING;
    icf_cc_delete_inc_media_sdp_info_list(p_cc_pdb,&p_nw_inc_media_change_req->p_sdp_list);
    /* Send failure response for update to remote */
    if(ICF_FAILURE == icf_cc_send_ssa_offer_chg_err(p_cc_pdb))
    {
        ret_val = ICF_FAILURE;
    }

    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */
    return ret_val;
}



/*******************************************************************************
 *
 * FUNCTION:        icf_cc_11_mm_err_ind
 *
 * DESCRIPTION:     This function is invoked when the CC is awaiting ack for 
 *                  Connect sent to SSA for an incoming call and it receives 
 *                  an error indication from MMI.
 *                  Current State: ICF_CC_11_AWT_SSA_CONNECT_ACK
 *                  Current Event: ICF_CC_MM_ERR_IND
 *
 ******************************************************************************/
icf_return_t icf_cc_11_mm_err_ind(
        INOUT icf_cc_pdb_st *p_cc_pdb)
{
    icf_return_t                     ret_val = ICF_SUCCESS;
	icf_mm_err_ind_st                *p_mm_err_ind = ICF_NULL;
    icf_mmi_reason_t                 media_error;
    
    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /* function entry trace */

	p_mm_err_ind = (icf_mm_err_ind_st*)
                      (p_cc_pdb->p_internal_msg->p_msg_data);
 
    media_error = p_mm_err_ind->reason;

    /* Set release type as MM initiated */
    ICF_CC_SET_RELEASE_TYPE(p_cc_pdb, ICF_REL_TYPE_MM_INITIATED)
    ICF_CC_TRACE(p_cc_pdb, ICF_TRACE_REL_TYPE_MM_INIT)

    ret_val = icf_cc_11_call_term_handler(p_cc_pdb);

	/*
	* If mmi has send mm_err_ind with reason as ICF_ECODE_DMS_IND_RECVD
	*/
    if(ICF_ECODE_DMS_IND_RECVD == media_error)
    {
        /* Mark MM interface as cleared so as we don’t send DMS when we 
         * receive prack from network*/
        ICF_CC_SET_CALL_CLR_STATUS(p_cc_pdb, 
                ICF_CALL_CLEAR_STATUS_MM_CLEARED)
    }

    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */
    return ret_val;
}


/*******************************************************************************
 *
 * FUNCTION:        icf_cc_11_timer_expiry
 *
 * DESCRIPTION:     This function is invoked when the CC is awaiting ack for 
 *                  Connect sent to SSA for an incoming call and timer expires.
 *                  Current State: ICF_CC_11_AWT_SSA_CONNECT_ACK
 *                  Current Event: ICF_CC_TIMER_EXPIRY
 *
 ******************************************************************************/
icf_return_t icf_cc_11_timer_expiry(
        INOUT icf_cc_pdb_st *p_cc_pdb)
{
    icf_return_t                     ret_val = ICF_SUCCESS;
    
    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /* function entry trace */

    /*Check if release timer has expired*/    
    if(ICF_TIMER_TYPE_RELEASE == ((icf_timer_data_st *)
                   (p_cc_pdb->p_glb_pdb->p_call_ctx->p_timer_data))->timer_type)
    {
        /* Send forceful call clear to SSA */
        ret_val = icf_cc_send_ssa_force_clear(p_cc_pdb);
        ret_val = icf_cc_07_timer_expiry(p_cc_pdb);
    }
    else
    {

        /* when no ACK is received from remote then on timer expiry
         * BYE should be sent so reset the signalling ongoing so that
         * BYE can be attempted
         */
        ICF_CC_RESET_SIG_ONGOING(p_cc_pdb)

        /* Set release type as timer expiry */
        ICF_CC_SET_RELEASE_TYPE(p_cc_pdb, ICF_REL_TYPE_TIMER_EXPIRY)
        ICF_CC_TRACE(p_cc_pdb, ICF_TRACE_REL_TYPE_TIMER_INIT)
        if (ICF_FAILURE == icf_cc_11_call_term_handler(p_cc_pdb))
		{
			ret_val = ICF_FAILURE;
		}
        /* no need to send forceful clear to SSA as graceful 
         * termination is being attempted here
         */
        /*
		else
		{
			ret_val = icf_cc_send_ssa_force_clear(p_cc_pdb);
		}
        */
    }

    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */
    return ret_val;
}

/*******************************************************************************
 *
 * FUNCTION:        icf_cc_11_ssa_connect_ack
 *
 * DESCRIPTION:     This function is invoked when the CC is awaiting ack for 
 *                  Connect sent to SSA for an incoming call and it receives 
 *                  the same (SIP ACK recd for 200 OK sent for incoming INVITE).
 *                  In this case, if media committed is true then the state is 
 *                  changed to Connected. If media committed is false, MMS 
 *                  request is sent to MMI if ACK contains SDP, else Call is 
 *                  terminated if ACK does not contain SDP.
 *                  Current State: ICF_CC_11_AWT_SSA_CONNECT_ACK
 *                  Current Event: ICF_CC_SSA_CONNECT_ACK
 *
 ******************************************************************************/
icf_return_t icf_cc_11_ssa_connect_ack(
        INOUT icf_cc_pdb_st *p_cc_pdb)
{
    icf_return_t                     ret_val = ICF_SUCCESS;
    icf_nw_inc_connect_ack_st        *p_connect_ack =ICF_NULL;
        

    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /* function entry trace */
           
    p_connect_ack = (icf_nw_inc_connect_ack_st *) 
                         p_cc_pdb->p_internal_msg->p_msg_data;
    
    /* Start change for SPR14867 */
    ICF_CC_SET_STATUS(p_cc_pdb,
                   ICF_STATUS_SIGNAL_ONGOING_ENDED)
    /* Reset signalling ongoing flag */
    ICF_CC_RESET_SIG_ONGOING(p_cc_pdb)
    /* End change for SPR14867 */

    if(p_connect_ack->bitmask & ICF_NW_INC_CONNECT_ACK_SDP)
    {     
        if(ICF_FALSE == p_cc_pdb->p_call_ctx->media_committed)
        {

	    ICF_CC_TRACE(p_cc_pdb, ICF_TRACE_SDP_IN_PEER_MSG)
	    p_cc_pdb->sdp_present = ICF_TRUE;
	    p_cc_pdb->p_list_1 = p_connect_ack->p_sdp_list;
	    /* This function shall validate the i'th level
	     * matching and copy the received SDP into the 
	     * call context
	     */
	    if (ICF_FAILURE == 
		    icf_cc_validate_ith_level_m_line_check(p_cc_pdb))
	    {
	        /* Check if ith level check has failed */
	        if (ICF_CC_STREAM_POS_VALIDATION_FAIL == 
                        (p_cc_pdb->common_bitmask & 
                            ICF_CC_STREAM_POS_VALIDATION_FAIL))
	        {
		        /* Invoke the generic call clear handler corresponding 
                to that state */
		        ret_val = icf_cc_11_generic_err(p_cc_pdb);
			ICF_CC_SET_RELEASE_CAUSE(p_cc_pdb,ICF_CALL_CLEAR_REASON_INVALID_SDP_NW)

                ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */
                return ret_val; 
	        }
	        else
	        {
		        ret_val = ICF_FAILURE;
	        }
	    }
	    if (ICF_FAILURE == ret_val)
	    {
	        ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */
	        return ret_val;
	    }

        }
        /* This is the case where the ACK comes with SDP.In such a scenario
         * we would simply ignore it.
         */
        else
        {
            icf_cc_delete_inc_media_sdp_info_list(p_cc_pdb,
                            &p_connect_ack->p_sdp_list);
        }
   }
   else if(p_connect_ack->bitmask & ICF_NW_INC_CONNECT_ACK_ERROR)
    {
          ICF_CC_SET_RELEASE_CAUSE(p_cc_pdb,ICF_CALL_CLEAR_REASON_INVALID_SDP_NW)
          ret_val = icf_cc_11_generic_err(p_cc_pdb);   
          ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */
          return ret_val;
    }

    ICF_CC_TRACE(p_cc_pdb, ICF_TRACE_MEDIA_SETUP_TYPE)
    /* Set call connect done */
    ICF_CC_SET_COMMON_BITMASK(p_cc_pdb,ICF_CALL_CONNECT_DONE)  

    if(ICF_CC_08_AWT_CALL_TERMINATE_RESP == 
	    ICF_CC_GET_NEXT_STATE(p_cc_pdb))
    {
	    /* Send Call Terminate Request to SSA */
	    ret_val = icf_cc_send_ssa_call_term_req(p_cc_pdb);

	    if (ICF_SUCCESS == ret_val)
	    {
	        /* Change call_state to Awaiting DMS response */
	        ICF_CC_SET_CALL_STATE(p_cc_pdb, 
		        ICF_CC_08_AWT_CALL_TERMINATE_RESP)
	    }

	    /* Set the stat for unsuccessfull call */
	    ICF_STAT(p_cc_pdb->p_glb_pdb,
		    ICF_STAT_UNSUCC_CALLS)

    }/*End if(ICF_CC_08_AWT_CALL_TERMINATE_RESP == ...*/

    else
    {
	    /* Check that the call is NFS and media committed is false */
	    ICF_CC_TRACE(p_cc_pdb, ICF_TRACE_CALL_DIR)
	    ICF_CC_TRACE(p_cc_pdb, ICF_TRACE_MEDIA_SETUP_TYPE)
	    ICF_CC_TRACE(p_cc_pdb, ICF_TRACE_MEDIA_COMMITTED)

	    if ((ICF_NULL == ICF_CC_CHECK_COMMON_BITMASK(p_cc_pdb, 
			    ICF_CALL_MEDIA_SETUP_TYPE)) 
		    && (ICF_FALSE == p_cc_pdb->p_call_ctx->media_committed))
	    {
		    /* TO DO - sdp_present */
		    /* Fill the value of p_cc_pdb->sdp_present */
		    /* Check whether the Ack contains SDP or not */
		    ICF_CC_TRACE(p_cc_pdb, ICF_TRACE_SDP_IN_PEER_MSG)

		    if(ICF_TRUE == p_cc_pdb->sdp_present)
		    {
			    /* ACK contains SDP */
			    /* Stop Alert timer and start modify media timer */
			    ret_val = icf_cc_start_media_timer(p_cc_pdb);
		    }
		    else
		    {
			    /* ACK does not contain SDP */

			    /* ICF_CC_TRACE(p_cc_pdb, ICF_TRACE_CALL_FAIL_NO_SDP) */
			    ICF_CC_SET_ECODE(p_cc_pdb, ICF_ERROR_CALL_FAIL_NO_SDP)

			    /*
			     * Fill the release cause as ICF_CALL_CLEAR_MEDIA_MISMATCH
			     */
			    ICF_CC_SET_RELEASE_CAUSE(p_cc_pdb,
				    ICF_CALL_CLEAR_MEDIA_MISMATCH)


			    /* Stop Alert timer and start Release timer */
			    ret_val = icf_cc_start_release_timer(p_cc_pdb);

			    /* Set the stat for unsuccessfull call */
			    ICF_STAT(p_cc_pdb->p_glb_pdb,
				    ICF_STAT_UNSUCC_CALLS)

		    }



		    if ((ICF_TRUE == p_cc_pdb->sdp_present) 
			    && (ICF_SUCCESS == ret_val))
		    {
		        /* ACK contained SDP, media timer successfully started */
		        /* 
		         * Now invoke the function which shall compute the 
		         * MMS type while comparing the applied and the network 
                 * received SDP's
		         * If media_committed = true ,this is second answer
		         * else it is first answer
		         */

                /* In tunnel mode, the following code snippet is not required
                   as we not understand media modes.
                */   
ICF_CHECK_TUNNEL_MODE_START(p_cc_pdb->p_glb_pdb,ICF_FALSE)                
                icf_internal_list_st	*p_stream_sdp_info_node=p_cc_pdb->
									p_call_ctx->p_stream_sdp_info;
                icf_stream_sdp_info_st	*p_stream_sdp_info_data = ICF_NULL;

                while(p_stream_sdp_info_node != ICF_NULL)
	            {
		            p_stream_sdp_info_data = (icf_stream_sdp_info_st *)
									            (p_stream_sdp_info_node->p_payload);
		            if (ICF_MEDIA_STREAM_ACTIVE ==
			            (p_stream_sdp_info_data->stream_status &
			            ICF_MEDIA_STREAM_ACTIVE)
		               )
		            {
                        p_stream_sdp_info_data->application_mode =
                            ICF_MEDIA_MODE_SEND_RECV;
                    }

                    p_stream_sdp_info_node = (icf_internal_list_st *)
										(p_stream_sdp_info_node->p_next);
                }
ICF_CHECK_TUNNEL_MODE_END

                /* This is an answer from remote.In tunnel mode,
                   we will set the media_modify_type field which will
                   determine whether the tunnelled MMS request
                   and response will contain the tunnelled SDP or not.
                */
ICF_CHECK_TUNNEL_MODE_START(p_cc_pdb->p_glb_pdb,ICF_FALSE)
		        if (ICF_FAILURE == icf_cc_make_mms_type_for_rem_succ_ans(p_cc_pdb))
		        {
			        ret_val = ICF_FAILURE;
    		    }
ICF_CHECK_TUNNEL_MODE_END
ICF_CHECK_TUNNEL_MODE_START(p_cc_pdb->p_glb_pdb,ICF_TRUE)
                if(ICF_FAILURE == icf_cc_make_mms_type_for_answer_in_tunnel_mode(p_cc_pdb))
                {
                    ret_val = ICF_FAILURE;	
                }
ICF_CHECK_TUNNEL_MODE_END

		        if (ICF_FAILURE != ret_val)
		        { 
			        if(ICF_INT_MODULE_INVALID != 
				        ICF_CC_GET_MEDIA_ONGOING( p_cc_pdb))
			        {
			            /*
			             * MMS cannot be sent to nw as request is pending 
			             * from MM Mark offer change as pending event
			             */
			            ICF_CC_SET_PENDING_EVENT(p_cc_pdb,
				            ICF_PENDING_EVENT_SSA_CALL_CONNECT_ACK)

				        /* Set call state */
				        ICF_CC_SET_CALL_STATE(p_cc_pdb,
					        ICF_CC_13_WAIT_START_PROC_MEDIA)
				        ICF_CC_SET_NEXT_STATE(p_cc_pdb,
					        ICF_CC_11_AWT_SSA_CONNECT_ACK)
			        }
			        else
			        {
			            /* Change call state to Awaiting MMS response */
			            ICF_CC_SET_CALL_STATE(p_cc_pdb, 
				            ICF_CC_04_AWT_MMS_RESP)

				        /* Send MMS Request to MMI */
				        ret_val = icf_cc_send_mms_req(p_cc_pdb);
                        if ((ICF_SUCCESS == ret_val) &&
                             (ICF_CC_ALL_STREAMS_DELETED == 
                                (ICF_CC_ALL_STREAMS_DELETED & p_cc_pdb->common_bitmask)))
                        {
                            ret_val = icf_cc_11_generic_err(p_cc_pdb);
                            ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */
                            return ret_val;
	                    }
			        }
		        }
		    }/*end if(ICF_INT_MODULE_INVALID != ...*/
		    else if ((ICF_FALSE == p_cc_pdb->sdp_present) 
			    && (ICF_SUCCESS == ret_val))
		    {
		        /* ACK did not contain SDP, Release timer started successfully 
		        */


		        /* Send Call Terminate Request to SSA since we never got peer 
		         * SDP */
		        ret_val = icf_cc_send_ssa_call_term_req(p_cc_pdb);
		    }

		    if ((ICF_TRUE == p_cc_pdb->sdp_present) 
			    && (ICF_SUCCESS == ret_val))
		    {

		        /* Check if MMS was sent */
		        if(ICF_TRUE == p_cc_pdb->mms_sent)
		        {
			        /* ACK contained SDP, MMS req successfully sent to MMI */

			        /* Change call state to Awaiting MMS response */
			        ICF_CC_SET_CALL_STATE(p_cc_pdb, 
				        ICF_CC_04_AWT_MMS_RESP)

			        /* Set next state as connected */
			        ICF_CC_SET_NEXT_STATE(p_cc_pdb, ICF_CC_06_CONNECTED)

			        /* Mark current event as ongoing event */
			        ICF_CC_SET_ONGOING_EVENT(p_cc_pdb, 
				        p_cc_pdb->current_event)

			        /* Stop Alert timer and start modify media timer */
			        ret_val  = icf_cc_start_media_timer(p_cc_pdb);
		        }
		        else
		        {
                    /* Invoke the generic call clear handler corresponding 
                    to that state */
		            ret_val = icf_cc_11_generic_err(p_cc_pdb);
                    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */
                    return ret_val; 
		        }
		    }
		    else if ((ICF_FALSE == p_cc_pdb->sdp_present) 
			    && (ICF_SUCCESS == ret_val))
		    {
		        /* ACK did not contain SDP, Terminate successfully sent to SSA 
		        */

		        /* Send DMS Request to MMI */
		        p_cc_pdb->forceful_delete = ICF_FALSE;
		        ret_val = icf_cc_send_dms_req(p_cc_pdb);
		    }
		    /*If SDP was present and there was some failure returned by any 
		     *called function the update the stat for unsuccessfull call*/
		    else if(ICF_TRUE == p_cc_pdb->sdp_present)
		    {
		        ICF_STAT(p_cc_pdb->p_glb_pdb,
			        ICF_STAT_UNSUCC_CALLS)                
		    }
		    if ((ICF_FALSE == p_cc_pdb->sdp_present) 
			    && (ICF_SUCCESS == ret_val))
		    {
		        /* ACK did not contain SDP, DMS req successfully sent to MMI */

		        /* Change call_state to Awaiting DMS response */
		        ICF_CC_SET_CALL_STATE(p_cc_pdb, ICF_CC_07_AWT_DMS_RESP)
		    }

	    } /* End of if for NFS and media_committed */
	    else
	    {
            /* Change call state to connected */
			ICF_CC_SET_CALL_STATE(p_cc_pdb, ICF_CC_06_CONNECTED)
			ICF_STAT(p_cc_pdb->p_glb_pdb,
				ICF_STAT_SUCC_INC_CALLS_EST)

			/*Update the stats for successfull calls*/
			ICF_STAT(p_cc_pdb->p_glb_pdb,
				ICF_STAT_SUCC_CALLS_EST)

			/*Stop current timer */ 
			icf_cc_stop_current_timer(p_cc_pdb); 

ICF_CHECK_TUNNEL_MODE_START(p_cc_pdb->p_glb_pdb,ICF_FALSE)
            /* REFER with replaces is not supported in tunnel mode */
            /* Fix for CSR 1-6727350: SPR 19316 */
			/* This indication needs to be sent from this point only for the old
			 * behavior if APP connect is not required for INV with Replaces.
			 */
            if ((ICF_INVALID_CALL_ID != 
					p_cc_pdb->p_call_ctx->replace_initiated_call_id) &&
				(ICF_FALSE == 
					p_cc_pdb->p_config_data->app_connect_reqd_for_replaces))
            {
                /* now we can send the party_replaced_ind to CC and 
                * then reset the replace_initiated_call_id in this new call_ctx */
                ret_val = icf_cc_send_pa_party_replaced_ind(p_cc_pdb);
                p_cc_pdb->p_call_ctx->replace_initiated_call_id = ICF_INVALID_CALL_ID;
            }
ICF_CHECK_TUNNEL_MODE_END            

            /* 
             * Now check whether update is reqd 
             * If required ,then send UPDATE 
             */
             if (ICF_FAILURE == icf_cc_send_pa_connect_resp(p_cc_pdb))
             {
                ret_val = ICF_FAILURE;
             }
             /* In tunnel mode, we will not send an auto UPDATE */
ICF_CHECK_TUNNEL_MODE_START(p_cc_pdb->p_glb_pdb,ICF_FALSE)
             if ((ICF_FAILURE != ret_val) &&
                 (ICF_TRUE == p_cc_pdb->p_call_ctx->update_reqd))
             {
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
             }
ICF_CHECK_TUNNEL_MODE_END
#if 0
#ifdef ICF_SERVICE_ENABLE
		/* If media state is remote held, send remote hold ind to pa*/
		if(ICF_NULL != ICF_CC_CHECK_MEDIA_STATE(p_cc_pdb,
			    ICF_MEDIA_STATE_REMOTE_HELD))
		{
		    ret_val = icf_cc_send_pa_remote_hold_ind(p_cc_pdb);
		}
#endif
#endif
	    } /* End of else for NFS and media_committed */
    }
   
    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */
	return ret_val;
}



/*******************************************************************************
 *
 * FUNCTION:        icf_cc_11_generic_err
 *
 * DESCRIPTION:     This function is invoked when the CC is awaiting ack for 
 *                  Connect sent to SSA for an incoming call and it receives a 
 *                  generic error indication.
 *                  Current State: ICF_CC_11_AWT_SSA_CONNECT_ACK
 *                  Current Event: ICF_CC_GENERIC_ERROR
 *
 ******************************************************************************/
icf_return_t icf_cc_11_generic_err(
        INOUT icf_cc_pdb_st *p_cc_pdb)
{
    icf_return_t                     ret_val = ICF_SUCCESS;

    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /* function entry trace */

    ICF_CC_SET_RELEASE_TYPE(p_cc_pdb, ICF_REL_TYPE_INTERNAL_INITIATED)
    ICF_CC_TRACE(p_cc_pdb, ICF_TRACE_REL_TYPE_INT_ERR)
    
    /*Fix for SPR 18884 ----->CSR-1-6239428*/
    /* Reset the signalling ongoing in call context as here we are awaiting ACK
     * for connect send to SSA for an incoming call and we receives a generic
     * error;This results in sending dms request towards application and BYE
     * request towards network.Also set the call clear reason as
     * ICF_CALL_CLEAR_REASON_NW_TIMEOUT */
    ICF_CC_RESET_SIG_ONGOING(p_cc_pdb)
    ICF_CC_SET_RELEASE_CAUSE(p_cc_pdb, 
            ICF_CALL_CLEAR_REASON_NW_TIMEOUT)   
    
    /* Initiate the call clear procedure */
    ret_val = icf_cc_11_call_term_handler(p_cc_pdb);

    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */
    return ret_val;
}

/*******************************************************************************
 *
 * FUNCTION:        icf_cc_11_call_term_handler
 *
 * DESCRIPTION:     This is a utility function that does common processing for
 *                  call termination in state ICF_CC_11_AWT_SSA_CONNECT_ACK.
 *                  The function
 *                  a. Starts Release timer
 *                  b. Sends Call Terminate Request/Call Reject (due to Server
 *                     Internal Error) to SSA in the current state (whatever
 *                     stack allows)
 *                  c. Sends DMS Request to MMI
 *                  d. Changes call state to Awaiting DMS Response
 *
 ******************************************************************************/
icf_return_t icf_cc_11_call_term_handler(
        INOUT icf_cc_pdb_st *p_cc_pdb)
{
    icf_return_t                     ret_val = ICF_SUCCESS;

    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /* function entry trace */

    /* Stop Alert timer and start Release timer */
    if(ICF_FAILURE == icf_cc_start_release_timer(p_cc_pdb))
    {
        ret_val = ICF_FAILURE;
    }
    /* Check if MM has been cleared */
    else if(ICF_NULL == ICF_CC_CHECK_CALL_CLR_STATUS(p_cc_pdb,
                                        ICF_CALL_CLEAR_STATUS_MM_CLEARED))
    {
        if (ICF_SUCCESS == ret_val)
        {
            /* Send non-forceful DMS Request to MMI */
            p_cc_pdb->forceful_delete = ICF_FALSE;
            ret_val = icf_cc_send_dms_req(p_cc_pdb);
        }

        if (ICF_SUCCESS == ret_val)
        {
            /* Change call_state to Awaiting DMS response */
            ICF_CC_SET_CALL_STATE(p_cc_pdb, ICF_CC_07_AWT_DMS_RESP)
        }

        /* start spr14506 */
        ICF_CC_TRACE(p_cc_pdb, ICF_TRACE_SIG_ONGOING) 
        if(ICF_INT_MODULE_INVALID ==
                 ICF_CC_GET_SIG_ONGOING(p_cc_pdb))
         {
          /*
           * Send call clear request to the SSA
           */
           if(ICF_FAILURE == icf_cc_send_ssa_call_term_req(p_cc_pdb))
           {
             ret_val = ICF_FAILURE;
           }/* if(ICF_FAILURE == icf_cc... */
           else
           {
             /*
              * Update the next state in the call context
              */
             ICF_CC_SET_NEXT_STATE(p_cc_pdb,
                   ICF_CC_08_AWT_CALL_TERMINATE_RESP)
           }/* else */
        }/* if(ICF_NULL == ICF_CC_GET_SIG_ON... */   
        else
        {
           ICF_CC_SET_NEXT_STATE(p_cc_pdb, ICF_CC_12_WAIT_START_PROC_SIG)
        }
                  
    }
    else
    {
        /*
         * If MM is cleared then set next state as 
         * ICF_CC_08_AWT_CALL_TERMINATE_RESP. This would happen in case MM 
         * error indication was received with ecode ICF_ECODE_DMS_IND_RECVD
         */
        ICF_CC_SET_NEXT_STATE(p_cc_pdb,ICF_CC_08_AWT_CALL_TERMINATE_RESP)
    }
    
    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */
    return ret_val;
}
