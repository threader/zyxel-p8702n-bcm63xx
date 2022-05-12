/*------------------------------------------------------------------------------
 *
 * File name        : icf_cc_fsm_05.c
 *
 * Purpose          : This file contains the event handlers in 
 *                    ICF_CC_05_AWT_CONNECT state.
 *
 * Revision History :
 *
 * Date         Name            Ref#            Comments
 * --------     ------------    ----            ---------------
 * 10-Dec-2004  Umang Singh     Non Service LLD Initial 
 * 21-Dec-2004  Rohit Aggarwal                  Addition of traces, stats
 * 24-Dec-2004  Rohit Aggarwal                  Compilation fixes
 * 27-Dec-2004  Rohit Aggarwal                  Coding guidelines violation fix
 * 20-Apr-2005  Sumit Gupta      SPR 7393       For a NFS call when if media is 
 *                                              not committed then called 
 *                                              function
 *                                              icf_cc_mms_rsp_pa_connect
 *  
 * 31-May-2005  Sumit Gupta     SPR 7629        Pa initiated release type flag
 *                                              would bet set even if call
 *                                              forwarding indication is sent
 *                                              back to CM when call terminate
 *                                              is received from PA
 * 10-Aug-2005	Rohit Aggarwal	ICF				Code changes specific to ICF
 * 22-Feb-2005	Amit Sharma                     Added code for auth support
 * 08-Mar-2006  Aman Aggarwal					ICF merging with ICF2.0
 * 26-Jun-2006  Amit Sharma     ICF 6.0 APIs    Structure changes for multiple
 *                                              m-lines
 * 2-Jan-2007   Priyanka Dua    ICF 6.3         Code changes for Forking and 
 						Multiple answer
 * 26-Mar-2007  Deepti Goyal    ICF 6.4         Changes for media connect req
 *                                              APi from PA.
 * 04-Apr-2007  Deepti Goyal    ICF 6.3         Changes for media connect req
 *                                              APi from PA.
 * 09-Jul-2007  Neha Choraria   ICF 7.0         Changes for Forking &
 *                                              Multiple Answer
 * 3-Apr-2008   Sumant Gupta    ICF 8.0         Rel7.1 Windows porting Merging
 * 23-Jun-2008  Anurag Khare    SPR 18684       CSR 1-5985517 Merge 
 * 03-Nov-2008  Rajiv Kumar     SPR 19188       Rel 7.1.2 SPR merging (SPR
 *                                              18963)
 * 03-Dec-2008  Tarun Gupta     SPR 19218       Merged CSR 1-6188936
 * 11-Feb-2009  Anurag Khare    ICF REl 8.1     Changes done for Call modify
 *                                              Enhancements
 * 13-Feb-2009  Tarun Gupta     Rel 8.1 LLD     SDP Tunnelling Enhancements
 * 16-Feb-2009  Anurag Khare    Rel 8.1         Call modify enhancements
 *                                              in forking scenario
 * 04-Mar-2009  Tarun Gupta     Rel 8.1         Modified icf_cc_05_ssa_call_connect
 * 06-Mar-2009  Tarun Gupta     Rel 8.1         Modified icf_cc_05_ssa_media_param
 * 09-Apr-2009  Rajiv Kumar     SPR 19428       Merged CSR 1-6851629
 * 22-Apr-2009 Abhishek Dhammawat SPR 19473     Modified
 *                                              icf_cc_05_pa_call_modify_req
 * 25-May-2009  Anuradha Gupta  SPR 19672       Merged CSR 1-6779001
 * 08-Jun-2009  Kamal Ashraf    SPR 19590       Modified icf_cc_05_pa_call_modify_req 
 * 07-Sep-2009  Anuradha Gupta  SPR 19999       Merged CSR 1-7587381
 * 10-Oct-2009  Rajiv Kumar     SPR 20101       Fix for SPR: 20101  
 * 22-Oct-2009  Anurag Khare    SPR 20078       Fix for SPR: 20078
 * 27-Oct-2009 Abhishek Dhammawat SPR 20196     Added
 *                                              icf_cc_05_media_prack_resp
 *
 * Copyright (c) 2009, Aricent.
 *---------------------------------------------------------------------------*/


#include "icf_ecode.h"
#include "icf_cc_prototypes.h"
#include "icf_cc_macro.h"


/******************************************************************************
 *
 * FUNCTION:        icf_cc_05_ssa_media_param
 * 
 * DESCRIPTION:     This function is invoked when the CC is awaiting  
 *                  CONNECT from the SSA and it receives media param
 *                  (SIP 183 ) from SSA.
 *                  Current State: ICF_CC_05_AWT_CONNECT
 *                  Current Event: ICF_CC_SSA_MEDIA_PARAM
 *
 *****************************************************************************/
icf_return_t icf_cc_05_ssa_media_param(
        INOUT icf_cc_pdb_st *p_cc_pdb)
{
    icf_return_t     ret_val = ICF_SUCCESS;
    icf_nw_inc_media_param_st	*p_nw_inc_media_param = ICF_NULL;
	icf_boolean_t	 handle_sdp = ICF_FALSE;

    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /* function entry trace */
    p_nw_inc_media_param = (icf_nw_inc_media_param_st *)
				(p_cc_pdb->p_internal_msg->p_msg_data);

ICF_CHECK_MULT_ANS_ENBL_START(p_cc_pdb->p_glb_pdb)
	/* 183 with different SDP from same UAS(Multiple answer) */
	handle_sdp = ICF_TRUE;            
ICF_CHECK_MULT_ANS_ENBL_END

         /********* Changes for Forking & Multiple Answer Start (Rel7.0) ***********/

         if (ICF_FORKED_RESP == (p_cc_pdb->p_call_ctx->common_bitmask & ICF_FORKED_RESP))
         {
ICF_CHECK_MULT_ANS_DISBL_START(p_cc_pdb->p_glb_pdb)
              
                  ICF_PRINT(((icf_uint8_t*)"\n[CC]: Multiple Answer OFF in forking - Forceful\
                             call clear "));
                  icf_cc_delete_inc_media_sdp_info_list(p_cc_pdb,&p_nw_inc_media_param->p_sdp_list);
                  p_cc_pdb->p_call_ctx->release_cause = ICF_CAUSE_NETWORK_ERROR;
                  ret_val = icf_cc_05_generic_err(p_cc_pdb);
                  return ret_val;
              
ICF_CHECK_MULT_ANS_DISBL_END

                  /* 183 with SDP from another UAS(Forking) */
		  handle_sdp = ICF_TRUE;            
	 	  /* Reset forked answer indication */
		  ICF_CC_RESET_COMMON_BITMASK(p_cc_pdb,ICF_FORKED_RESP)
         }

         /* If it is the first resp then its forked_resp_id should be
            assigned 0 */

         if((ICF_NULL == p_nw_inc_media_param->forked_resp_id) &&
               (ICF_NULL == p_cc_pdb->p_call_ctx->forked_resp_counter))
         p_nw_inc_media_param->forked_resp_id = ICF_NULL;


         /*********** Changes for Forking & Multiple Answer End (Rel7.0) ************/

         /* This field is set to false both in case of forked/non-forked response */ 
	 p_cc_pdb->p_call_ctx->no_remote_sdp = ICF_FALSE;

    /*
     * Check if media has been committed. 
     */
    ICF_CC_TRACE(p_cc_pdb, ICF_TRACE_MEDIA_COMMITTED)
    if((ICF_FALSE == p_cc_pdb->p_call_ctx->media_committed) || (ICF_TRUE == handle_sdp))
    {


			
				
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
				ret_val = icf_cc_05_generic_err(p_cc_pdb);
			}
		}
		else
		{
                       /**** Changes for Forking & Multiple Answer Start (Rel7.0) ****/

                        p_cc_pdb->forked_resp_id =  ICF_NULL;

                        /***** Changes for Forking & Multiple Answer End (Rel7.0) *****/
ICF_CHECK_TUNNEL_MODE_START(p_cc_pdb->p_glb_pdb,ICF_FALSE)
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
ICF_CHECK_TUNNEL_MODE_START(p_cc_pdb->p_glb_pdb,ICF_FALSE)
				ret_val = icf_cc_02_generic_err(p_cc_pdb);
ICF_CHECK_TUNNEL_MODE_END
                /* Since we have received a provisional response from 
                   the peer, we need to send CANCEL to the peer. Thus 
                   calling icf_cc_05_generic_err.
                */   
ICF_CHECK_TUNNEL_MODE_START(p_cc_pdb->p_glb_pdb,ICF_TRUE)
				ret_val = icf_cc_05_generic_err(p_cc_pdb);
ICF_CHECK_TUNNEL_MODE_END
			}
			else if (ICF_FAILURE != ret_val)
			{
			   /*
                            * Send Modify Media Session to the MMI 
                            */

                            /* In tunnel mode, no GCL is done, so not
                               operating on the following bitmask.
                             */
ICF_CHECK_TUNNEL_MODE_START(p_cc_pdb->p_glb_pdb,ICF_FALSE)                
                            /* IPTK will send the local codecs on the GCL iff 
                            p_prefered media profile is not null that means
                            ICF_NO_LOCAL_CODEC_IN_GCL is not set */

	                    if(ICF_TRUE == p_cc_pdb->p_call_ctx->media_committed &&
                               ICF_FALSE == (p_cc_pdb->p_call_ctx->common_bitmask & ICF_NO_LOCAL_CODEC_IN_GCL))
                            {
               		        /* TBD - post 6.3 */
               		        /* Uncommented corresponding to spr 14476 fix.*/
               		        p_cc_pdb->p_call_ctx->common_bitmask 
                                       |= ICF_OFFER_CODECS_IN_GCL;
               	            }

               	            if(p_cc_pdb->p_call_ctx->common_bitmask & ICF_NO_LOCAL_CODEC_IN_GCL)
           		    {
       	    		        p_cc_pdb->p_call_ctx->common_bitmask 
                                       &= ~ICF_NO_LOCAL_CODEC_IN_GCL;
           		    }
ICF_CHECK_TUNNEL_MODE_END

                            if(ICF_FAILURE == icf_cc_send_mms_req(p_cc_pdb))
                            {
                                ret_val = ICF_FAILURE;
                            }
                            /*  CSR 1-5985517 Merge */
                            /*  Change the state when mms is sent other wise 
                                let state unchanged */    
                            else if(ICF_FALSE != p_cc_pdb->mms_sent)
                            {
                                ICF_CC_SET_CALL_STATE(p_cc_pdb,ICF_CC_04_AWT_MMS_RESP)
                                ICF_CC_SET_NEXT_STATE(p_cc_pdb,ICF_CC_05_AWT_CONNECT)
                            }
                        }
		}
	}
	else
    {
		icf_cc_delete_inc_media_sdp_info_list(
			p_cc_pdb,&(p_nw_inc_media_param->p_sdp_list));
    }
     
   
    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */
    return ret_val;
}



/******************************************************************************
 *
 * FUNCTION:        icf_cc_05_ssa_alert
 *
 * DESCRIPTION:     This function is invoked when the CC is awaiting Connect
 *					from MMI and it receives alert without
 *                  media (SIP 180 without SDP) from SSA.
 *                  Current State: ICF_CC_05_AWT_CONNECT
 *                  Current Event: ICF_CC_SSA_ALERT
 *
 *****************************************************************************/
icf_return_t icf_cc_05_ssa_alert(icf_cc_pdb_st *p_cc_pdb)
{
          icf_return_t    ret_val = ICF_SUCCESS;
          icf_boolean_t	expected_evnt = ICF_FALSE;

          ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /* function entry trace */

ICF_CHECK_MULT_ANS_ENBL_START(p_cc_pdb->p_glb_pdb)
		/* 180 with different SDP from same UAS(Multiple answer) */
		expected_evnt = ICF_TRUE;            
ICF_CHECK_MULT_ANS_ENBL_END

         /********* Changes for Forking & Multiple Answer Start (Rel7.0) ***********/

         if (ICF_FORKED_RESP == (p_cc_pdb->p_call_ctx->common_bitmask & ICF_FORKED_RESP))
         {
ICF_CHECK_MULT_ANS_DISBL_START(p_cc_pdb->p_glb_pdb)
               
                  ICF_PRINT(((icf_uint8_t*)"\n[CC]: Multiple Answer OFF in forking - Forceful\
                            call clear "));
                  p_cc_pdb->p_call_ctx->release_cause = ICF_CAUSE_NETWORK_ERROR;
                  ret_val = icf_cc_05_generic_err(p_cc_pdb);
                  return ret_val;
              
ICF_CHECK_MULT_ANS_DISBL_END

                  /* 180 with SDP from another UAS(Forking) */
		  expected_evnt = ICF_TRUE;   

		  p_cc_pdb->p_call_ctx->no_remote_sdp = ICF_TRUE;
		
		  /* Reset forked answer indication */
		  ICF_CC_RESET_COMMON_BITMASK(p_cc_pdb,ICF_FORKED_RESP)

          }
 
         /*********** Changes for Forking & Multiple Answer End (Rel7.0) ************/

	if(ICF_TRUE == expected_evnt)
	{
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
	}
	else
	{
		/* This event is not expected in current state, therefore do nothing */
		ICF_CC_TRACE(p_cc_pdb, ICF_UNEXPECTED_EVENT)	
	}

        ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */
        return ret_val;
}



/******************************************************************************
 *
 * FUNCTION:        icf_cc_05_ssa_alert_with_media
 * 
 * DESCRIPTION:     This function is invoked when the CC is awaiting  
 *                  CONNECT from the SSA and it receives alert with media
 *                  (SIP 180 with SDP ) from SSA.
 *                  Current State: ICF_CC_05_AWT_CONNECT
 *                  Current Event: ICF_CC_ALERT_WITH_MEDIA
 *
 *****************************************************************************/
icf_return_t icf_cc_05_ssa_alert_with_media(
        INOUT icf_cc_pdb_st *p_cc_pdb)
{
    icf_return_t     ret_val = ICF_SUCCESS;
    icf_nw_inc_alert_media_st	*p_nw_inc_alert_media = ICF_NULL;
    icf_boolean_t	expected_evnt = ICF_FALSE;

    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /* function entry trace */
    p_nw_inc_alert_media = (icf_nw_inc_alert_media_st *)
				(p_cc_pdb->p_internal_msg->p_msg_data);

    /********* Changes for Forking & Multiple Answer Start (Rel7.0) ***********/

    if (ICF_FORKED_RESP == (p_cc_pdb->p_call_ctx->common_bitmask & ICF_FORKED_RESP))
    {
ICF_CHECK_MULT_ANS_DISBL_START(p_cc_pdb->p_glb_pdb)
          
                  ICF_PRINT(((icf_uint8_t*)"\n[CC]: Multiple Answer OFF in forking - Forceful\
                             call clear "));
                  icf_cc_delete_inc_media_sdp_info_list(p_cc_pdb,&p_nw_inc_alert_media->p_sdp_list);
                  p_cc_pdb->p_call_ctx->release_cause = ICF_CAUSE_NETWORK_ERROR;
                  ret_val = icf_cc_05_generic_err(p_cc_pdb);
                  return ret_val;
          
ICF_CHECK_MULT_ANS_DISBL_END

                 /* 180 with SDP from another UAS(Forking) */
		 expected_evnt = ICF_TRUE;            
		 /* Reset forked answer indication */
		 ICF_CC_RESET_COMMON_BITMASK(p_cc_pdb,ICF_FORKED_RESP)

     }

     /* If it is the first resp then its forked_resp_id should be
        assigned 0 */

     if((ICF_NULL == p_nw_inc_alert_media->forked_resp_id) &&
               (ICF_NULL == p_cc_pdb->p_call_ctx->forked_resp_counter))
       p_nw_inc_alert_media->forked_resp_id = ICF_NULL;
 

    /*********** Changes for Forking & Multiple Answer End (Rel7.0) ***********/

ICF_CHECK_MULT_ANS_ENBL_START(p_cc_pdb->p_glb_pdb)
		/* 180 with different SDP from same UAS(Multiple answer) */
		expected_evnt = ICF_TRUE;            
ICF_CHECK_MULT_ANS_ENBL_END

	/* This field is set to false both in case of forked/non-forked response */ 
	p_cc_pdb->p_call_ctx->no_remote_sdp = ICF_FALSE;

	if (ICF_TRUE == expected_evnt)
	{
       /*
	    * Stop the call setup timer and start the alert timer
		*/
		if (ICF_FAILURE == icf_cc_start_alert_timer(p_cc_pdb, ICF_FALSE, ICF_NULL))
		{
			ret_val = ICF_FAILURE;
		} /* if (ICF_FAILURE == icf_cc... */
		
	   /*
		* We need to do mms as second/forked answer is to be considered. 
		*/

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
				ret_val = icf_cc_02_generic_err(p_cc_pdb);
			}
		}
		else
		{
                         /**** Changes for Forking & Multiple Answer Start (Rel7.0) ****/

                         p_cc_pdb->forked_resp_id =  ICF_NULL;

                         /***** Changes for Forking & Multiple Answer End (Rel7.0) *****/
ICF_CHECK_TUNNEL_MODE_START(p_cc_pdb->p_glb_pdb,ICF_FALSE)
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
				ret_val = icf_cc_02_generic_err(p_cc_pdb);
			}
			else if (ICF_FAILURE != ret_val)
			{
			   /*
				* Send Modify Media Session to the MMI 
				*/
               /* In tunnel mode, no GCL is done, so not operating on the following bitmask */
ICF_CHECK_TUNNEL_MODE_START(p_cc_pdb->p_glb_pdb,ICF_FALSE)

                /* IPTK will send the local codecs on the GCL iff 
   			       p_prefered media profile is not null that means
				   ICF_NO_LOCAL_CODEC_IN_GCL is not set */

	             if(ICF_TRUE == p_cc_pdb->p_call_ctx->media_committed &&
        	      		ICF_FALSE == (p_cc_pdb->p_call_ctx->common_bitmask & ICF_NO_LOCAL_CODEC_IN_GCL))
                {
                	 /* TBD - post 6.3 */
                	/* Uncommented corresponding to spr 14476 fix.*/
                	p_cc_pdb->p_call_ctx->common_bitmask 
                                       |= ICF_OFFER_CODECS_IN_GCL;
                }

           		if(p_cc_pdb->p_call_ctx->common_bitmask & ICF_NO_LOCAL_CODEC_IN_GCL)
           		{
       	    		p_cc_pdb->p_call_ctx->common_bitmask 
                            &= ~ICF_NO_LOCAL_CODEC_IN_GCL;
                }
ICF_CHECK_TUNNEL_MODE_END                

				if(ICF_FAILURE == icf_cc_send_mms_req(p_cc_pdb))
				{
					ret_val = ICF_FAILURE;
				}
                /* SPR 19999 - Fix for CSR 1-7587381 SPR -19900.
				 *IPTK shall set this state only when it sends MMS request. 
				 */	
				else if (ICF_TRUE == p_cc_pdb->mms_sent)	
				{	
					ICF_CC_SET_CALL_STATE(p_cc_pdb,ICF_CC_04_AWT_MMS_RESP)
					ICF_CC_SET_NEXT_STATE(p_cc_pdb,ICF_CC_05_AWT_CONNECT)
				}
				else 
				{
					if(ICF_FAILURE == icf_cc_send_pa_alert_ind(p_cc_pdb))
					{
						/*Return ret_val as failure. Call will be now forcefully cleared*/
						ret_val = ICF_FAILURE;
					}
				}
			}
		}
	}
	else
	{
		/* This event is not expected in current state, therefore do nothing */
		ICF_CC_TRACE(p_cc_pdb, ICF_UNEXPECTED_EVENT)	
	}
	
    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */
    return ret_val;
}


/******************************************************************************
 *
 * FUNCTION:        icf_cc_05_ssa_call_connect
 * 
 * DESCRIPTION:     This function is invoked when the CC is awaiting  
 *                  CONNECT from the SSA and it receives connect 
 *                  (SIP 200 OK ) from SSA.
 *                  Current State: ICF_CC_05_AWT_CONNECT
 *                  Current Event: ICF_CC_SSA_CALL_CONNECT
 *
 *****************************************************************************/
icf_return_t icf_cc_05_ssa_call_connect(
        INOUT icf_cc_pdb_st *p_cc_pdb)
{
    icf_return_t     ret_val = ICF_SUCCESS;
    icf_nw_inc_connect_st *p_connect = ICF_NULL;
    icf_boolean_t	 handle_sdp = ICF_FALSE;
    icf_boolean_t    mms_required = ICF_FALSE;


ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /* function entry trace */

    p_connect = 
            (icf_nw_inc_connect_st*)(p_cc_pdb->p_internal_msg->p_msg_data);

    /********* Changes for Forking & Multiple Answer Start (Rel7.0) ***********/

    if (ICF_FORKED_RESP == (p_cc_pdb->p_call_ctx->common_bitmask & ICF_FORKED_RESP))
    {
ICF_CHECK_MULT_ANS_DISBL_START(p_cc_pdb->p_glb_pdb)
           
                  ICF_PRINT(((icf_uint8_t*)"\n[CC]: Multiple Answer Off in forking - Forceful\
                             call clear "));
                  icf_cc_delete_inc_media_sdp_info_list(p_cc_pdb,&p_connect->p_sdp_list);
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
                  
                  ret_val = icf_cc_05_generic_err(p_cc_pdb);
                  /* Set network clear bit */
                  ICF_CC_SET_CALL_CLR_STATUS(p_cc_pdb,
                    ICF_CALL_CLEAR_STATUS_NW_CLEARED)

                  return ret_val;
          
ICF_CHECK_MULT_ANS_DISBL_END
     }

     /* If it is the first resp then its forked_resp_id should be
        assigned 0 */

     if((ICF_NULL == p_connect->forked_resp_id) &&
               (ICF_NULL == p_cc_pdb->p_call_ctx->forked_resp_counter))
       p_connect->forked_resp_id = ICF_NULL;

    /*********** Changes for Forking & Multiple Answer End (Rel7.0) ************/

        if (ICF_NW_INC_CONNECT_WITH_SDP == 
		(p_connect->bitmask & ICF_NW_INC_CONNECT_WITH_SDP))
	{
		p_cc_pdb->sdp_present = ICF_TRUE;
ICF_CHECK_MULT_ANS_ENBL_START(p_cc_pdb->p_glb_pdb)
		/* 183 with different SDP from same UAS(Multiple answer) */
		handle_sdp = ICF_TRUE;            
ICF_CHECK_MULT_ANS_ENBL_END

		if (ICF_FORKED_RESP == (p_cc_pdb->p_call_ctx->common_bitmask & ICF_FORKED_RESP))
		{
			/* 183 with SDP from another UAS(Forking) */
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
                         ICF_PRINT(((icf_uint8_t*)"\n[CC]: 200 OK w/o SDP not a forked resp\
                                    - Clearing Call "));
                         /* Reset forked answer indication */
                         ICF_CC_RESET_COMMON_BITMASK(p_cc_pdb,ICF_FORKED_RESP)
		}
                else /* If 200 OK received (w/o SDP) is not a forked resp */
                {
                        p_cc_pdb->forked_resp_id = p_connect->forked_resp_id;

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
                p_cc_pdb->sdp_present = ICF_FALSE;
ICF_CHECK_MULT_ANS_DISBL_END
	}

ICF_SESSION_CONTROL_START	
	/*
     * Mark ongoing event as ICF_CC_SSA_CALL_CONNECT
	 */
	ICF_CC_SET_ONGOING_EVENT(p_cc_pdb,ICF_CC_SSA_CALL_CONNECT)
    /*
     * Check if media has been committed
     */
    ICF_CC_TRACE(p_cc_pdb, ICF_TRACE_MEDIA_COMMITTED)
    
    /*Send Connect ack to SSA */    
    if (ICF_FAILURE == icf_cc_send_ssa_connect_ack(p_cc_pdb))
    {
       if (ICF_NW_INC_CONNECT_WITH_SDP ==
        (p_connect->bitmask & ICF_NW_INC_CONNECT_WITH_SDP))
       {
          icf_cc_delete_inc_media_sdp_info_list(
               p_cc_pdb,&p_connect->p_sdp_list); 
        }
         ret_val = ICF_FAILURE;
    }
    else if((ICF_FALSE == p_cc_pdb->p_call_ctx->media_committed) || (ICF_TRUE == handle_sdp)
		|| (ICF_TRUE == p_cc_pdb->p_call_ctx->no_remote_sdp || 
                (ICF_CC_CALL_CLEAR == (p_cc_pdb->common_bitmask & ICF_CC_CALL_CLEAR))))
    {  
        p_cc_pdb->p_list_1 = p_connect->p_sdp_list;
        p_cc_pdb->forked_resp_id = p_connect->forked_resp_id;
        if (!(ICF_NW_INC_CONNECT_WITH_SDP & p_connect->bitmask)
            || (ICF_CC_CALL_CLEAR == (p_cc_pdb->common_bitmask & ICF_CC_CALL_CLEAR)))
        {
            ret_val = icf_cc_clear_call_no_remote_sdp(p_cc_pdb);
            return ret_val;
        }
        else if (ICF_FAILURE == icf_cc_validate_ith_level_m_line_check(p_cc_pdb))
        {
		if (ICF_CC_STREAM_POS_VALIDATION_FAIL == 
		     (p_cc_pdb->common_bitmask & ICF_CC_STREAM_POS_VALIDATION_FAIL))
		{
		   ret_val = icf_cc_05_generic_err(p_cc_pdb);
                   return ret_val;
		}

            
        }
        else 
        {
            /* In tunnel mode, dont issue this trace to the application */
ICF_CHECK_TUNNEL_MODE_START(p_cc_pdb->p_glb_pdb,ICF_FALSE)  
#ifdef ICF_TRACE_ENABLE    
                  if(ICF_NULL != p_connect->p_sdp_list)
		  {
                         ICF_CC_APP_TRACE(p_cc_pdb,ICF_TRACE_REMOTE_SDP_ANSWER)
		  } 
#endif

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
				ret_val = icf_cc_05_generic_err(p_cc_pdb);
                return ret_val;
			}
            else if (ICF_FAILURE != ret_val)
			{ 

              /*
               * Send Modify Media Session to the MMI
               */
               
              /* In tunnel mode, no GCL is done, so not operating
                 on the following bitmask.
              */
ICF_CHECK_TUNNEL_MODE_START(p_cc_pdb->p_glb_pdb,ICF_FALSE)
              /* IPTK will send the local codecs on the GCL iff 
				  p_prefered media profile is not null that means
				  ICF_NO_LOCAL_CODEC_IN_GCL is not set */

            	if(ICF_TRUE == p_cc_pdb->p_call_ctx->media_committed &&
   	             		ICF_FALSE == (p_cc_pdb->p_call_ctx->common_bitmask & ICF_NO_LOCAL_CODEC_IN_GCL))
       	     	{
                		 /* TBD - post 6.3 */
                   		/* Uncommented corresponding to spr 14476 fix.*/
                   		p_cc_pdb->p_call_ctx->common_bitmask 
                                       |= ICF_OFFER_CODECS_IN_GCL;
                 }

                 if(p_cc_pdb->p_call_ctx->common_bitmask & ICF_NO_LOCAL_CODEC_IN_GCL)
           		{
           	    		p_cc_pdb->p_call_ctx->common_bitmask 
                                        &= ~ICF_NO_LOCAL_CODEC_IN_GCL;
                }
ICF_CHECK_TUNNEL_MODE_END                 

               if (ICF_FAILURE == icf_cc_send_mms_req(p_cc_pdb))
               {
                  /*
                   * Mark ongoing event as ICF_CC_SSA_CALL_CONNECT
                   */
                  ICF_CC_SET_ONGOING_EVENT(p_cc_pdb,ICF_CC_SSA_CALL_CONNECT)

                  ret_val = ICF_FAILURE;
               } /* if (ICF_FAILURE == icf_c... */
              else if(ICF_TRUE == p_cc_pdb->mms_sent)
              {		
				 /* Start the media timer  */
				  ret_val = icf_cc_start_media_timer(p_cc_pdb);

                 /*
                  * Mark ongoing event as ICF_CC_SSA_CALL_CONNECT
                  */
                  ICF_CC_SET_ONGOING_EVENT(p_cc_pdb,ICF_CC_SSA_CALL_CONNECT)
                 /*
                  * Update the states in the call context
                  */
                  ICF_CC_SET_CALL_STATE(p_cc_pdb,ICF_CC_04_AWT_MMS_RESP)
                  ICF_CC_SET_NEXT_STATE(p_cc_pdb,ICF_CC_06_CONNECTED)
              } /* else */
#if 0
              else
               {
                   /*
                    * Stop the alert timer
                    */
                   icf_cc_stop_current_timer(p_cc_pdb);

                   /*
                    * Send CONNECT to PA
                    */
                   if (ICF_FAILURE == icf_cc_send_pa_connect_ind(p_cc_pdb))
                   {
                       ret_val = ICF_FAILURE;
                   } /* else if (ICF_FAILURE == ipp... */
                   else
                   {
                       /*
                        * Update the states in the call context
                        */
                       if (ICF_TRUE == p_cc_pdb->p_call_ctx->update_reqd) 
                       {
                           /*Send UPDATE request*/
                           if (ICF_FAILURE == icf_cc_ssa_send_update_msg(p_cc_pdb))
                           {
                               ret_val = ICF_FAILURE;
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
                   }
               }
#endif

           } /* end of if */
        }
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
		ret_val = icf_cc_05_generic_err(p_cc_pdb);
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
         if (ICF_TRUE == p_cc_pdb->sdp_present)
         {
            icf_cc_delete_inc_media_sdp_info_list(
        	            p_cc_pdb,&(p_connect->p_sdp_list));
         }
    }
 
    /* Check that if mms sent is FALSE, then it means media has already been
     * committed.So,send connect ind to the application 
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
           /*
            * Stop the alert timer
            */
           icf_cc_stop_current_timer(p_cc_pdb);
         }
ICF_CHECK_TUNNEL_MODE_END
         /* In tunnel mode, just stop the timer without the check for update_reqd */
ICF_CHECK_TUNNEL_MODE_START(p_cc_pdb->p_glb_pdb,ICF_TRUE)  
         icf_cc_stop_current_timer(p_cc_pdb);
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
              /*
               * Update the states in the call context
               */
              /* In tunnel mode, we do not send an auto UPDATE, thus the check for
                 update_reqd field in not required.
              */   
ICF_CHECK_TUNNEL_MODE_START(p_cc_pdb->p_glb_pdb,ICF_FALSE)            
	     if (ICF_TRUE == p_cc_pdb->p_call_ctx->update_reqd) 
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
         /* In tunnel mode just move to state 06 */
ICF_CHECK_TUNNEL_MODE_START(p_cc_pdb->p_glb_pdb,ICF_TRUE)
         ICF_CC_SET_CALL_STATE(p_cc_pdb,ICF_CC_06_CONNECTED)
ICF_CHECK_TUNNEL_MODE_END

	 } /* else Media was committed */ 
      }

  /******* Changes for Forking & Multiple Answer Start (Rel7.0) *******/

  ICF_CHECK_MULT_ANS_ENBL_START(p_cc_pdb->p_glb_pdb)
          
  /* p_forked_resp_list is deleted after connect ind is sent
     to PA */
  ICF_PRINT(((icf_uint8_t*)"\n[CC]: Clearing the forked_resp_list at 05 connect state "));
  ret_val = icf_cc_free_forked_resp_list(p_cc_pdb,p_cc_pdb->p_ecode);

  ICF_CHECK_MULT_ANS_ENBL_END
  /******** Changes for Forking & Multiple Answer End (Rel7.0) ********/

  }
  else
  {
       /*
       if (ICF_NW_INC_CONNECT_WITH_SDP ==
        (p_connect->bitmask & ICF_NW_INC_CONNECT_WITH_SDP))
       {
          icf_cc_delete_inc_media_sdp_info_list(
               p_cc_pdb,&p_connect->p_sdp_list); 
        }
       */
  }
  
	/* Fix for CSR 1-6851629: SPR 19297 */
	/* Set the call connect bitmask after 200 OK is processed */
	ICF_CC_SET_COMMON_BITMASK(p_cc_pdb, ICF_CALL_CONNECT_DONE)

    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */
    return ret_val;
}


/******************************************************************************
 *
 * FUNCTION:        icf_cc_05_ssa_remote_reject
 * 
 * DESCRIPTION:     This function is invoked when the CC is awaiting 
 *                  CONNECT from the PA and it receives Remote Call 
 *                  Reject from SSA (SIP 3xx/4xx/5xx received from peer).
 *                  Current State: ICF_CC_05_AWT_CONNECT
 *                  Current Event: ICF_CC_SSA_REMOTE_REJECT
 *
 *****************************************************************************/
icf_return_t icf_cc_05_ssa_remote_reject(
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

    
    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */
    return ret_val;
}


/******************************************************************************
 *
 * FUNCTION:        icf_cc_05_ssa_redirect
 * 
 * DESCRIPTION:     This function is invoked when the CC is awaiting 
 *                  CONNECT from the PA and it receives reroute call
 *                  Req from SSA (SIP 302 received from peer).
 *                  Current State: ICF_CC_05_AWT_CONNECT
 *                  Current Event: ICF_CC_SSA_REDIRECT
 *
 *****************************************************************************/
icf_return_t icf_cc_05_ssa_redirect(
        INOUT icf_cc_pdb_st *p_cc_pdb)
{
    icf_return_t     ret_val = ICF_SUCCESS;

    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /* function entry trace */
    
    /*
     * As the handling for this event is the same in the state
     * ICF_CC_03_AWT_SSA_ALERT_RESP,call their
     * state handler
     */
    ret_val = icf_cc_03_ssa_redirect(p_cc_pdb);
    
    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */
    return ret_val;

}


/******************************************************************************
 *
 * FUNCTION:        icf_cc_05_pa_call_term_req
 * 
 * DESCRIPTION:     This function is invoked when the CC is awaiting 
 *                  CONNECT from the PA(incoming call) or CONNECT
 *                  from the SSA(SIP 200 OK) and it receives Call 
 *                  Terminate Request from PA.
 *                  Current State: ICF_CC_05_AWT_CONNECT
 *                  Current Event: ICF_CC_PA_CALL_TERMINATE_REQ
 *
 *****************************************************************************/
icf_return_t icf_cc_05_pa_call_term_req(
        INOUT icf_cc_pdb_st *p_cc_pdb)
{
    icf_return_t     ret_val = ICF_SUCCESS;

    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /* function entry trace */
  

   /*
    * Set release type in call context as PA_INITIAITED
    */
    ICF_CC_TRACE(p_cc_pdb, ICF_TRACE_REL_TYPE_PA_INIT)
    ICF_CC_SET_RELEASE_TYPE(p_cc_pdb,ICF_REL_TYPE_PA_INITIATED)

    ret_val = icf_cc_chk_user_busy_n_srvc(p_cc_pdb);
    
    /*If status was not returned as ICF_STATUS_CALL_FWD_BUSY_LINE continue 
     *with normal call clearing*/
    if(ICF_SUCCESS == ret_val && 
                ICF_STATUS_CALL_FWD_BUSY_LINE != 
                p_cc_pdb->p_internal_msg->msg_hdr.status)
    {
 
        /*
         * Stop the alert timer and start the release timer
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
            if (ICF_NULL != ICF_CC_CHECK_COMMON_BITMASK(p_cc_pdb,
                        ICF_CALL_DIR))
            {
                /* validate the response code(if present)*/
                icf_cc_validate_response_code_in_term_req(p_cc_pdb);

                ICF_CC_SET_RELEASE_CAUSE(p_cc_pdb, 
                        ICF_CALL_CLEAR_REASON_NOANSWER)    
            }
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
            } /* else */
        } /* else */
    }/*End if(ICF_SUCCESS == ret_val ... */

    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */
    return ret_val;
}


/*******************************************************************************
 *
 * FUNCTION:        icf_cc_05_ssa_offer_change
 *
 * DESCRIPTION:     This function is invoked when the CC is awaiting 
 *                  CONNECT from PA and it receives a session param
 *                  change from SSA
 *                  (SIP UPDATE/RE-INVITE received from peer).
 *                  Current State: ICF_CC_05_AWT_CONNECT
 *                  Current Event: ICF_CC_SSA_OFFER_CHANGE
 *
 ******************************************************************************/
icf_return_t icf_cc_05_ssa_offer_change(
        INOUT icf_cc_pdb_st *p_cc_pdb)
{
    icf_return_t     ret_val = ICF_SUCCESS;

    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /* function entry trace */
    
    /*
     * As the handling for this event is the same in the 
     * the ICF_CC_03_AWT_ALERT_RESP state,call 
     * their handler
     */
    ret_val = icf_cc_03_ssa_offer_change(p_cc_pdb);
    
    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */
    return ret_val;
}

#if 0
/******************************************************************************
 *
 * FUNCTION:        icf_cc_05_call_mute_req
 * 
 * DESCRIPTION:     This function is invoked when the CC is awaiting 
 *                  CONNECT from the PA and it receives Call 
 *                  Mute Request from PA.
 *                  Current State: ICF_CC_05_AWT_CONNECT
 *                  Current Event: ICF_CC_CALL_MUTE_REQ
 *
 *****************************************************************************/
icf_return_t icf_cc_05_call_mute_req(
        INOUT icf_cc_pdb_st *p_cc_pdb)
{
    icf_return_t     ret_val = ICF_SUCCESS;

    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /* function entry trace */
     /* 
     * If this is an incomminc call mute is not expected before the call is 
     * connected
     */ 
    if((ICF_NULL != ICF_CC_CHECK_COMMON_BITMASK(p_cc_pdb,
                        ICF_CALL_DIR)))
    {
        ICF_CC_TRACE(p_cc_pdb,ICF_UNEXPECTED_EVENT)
        ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */
        return ret_val;

    }

    /*
     * If media is committed and media_ongoing is false
     * send MMS to MMI else mark as pending event and call type is outgoing
     */
    ICF_CC_TRACE(p_cc_pdb, ICF_TRACE_MEDIA_COMMITTED)
    if(ICF_TRUE == p_cc_pdb->p_call_ctx->media_committed 
            && (ICF_NULL == ICF_CC_CHECK_COMMON_BITMASK(p_cc_pdb,
                        ICF_CALL_DIR)))
    {
        /* Check if media ongoing is true */
        ICF_CC_TRACE(p_cc_pdb, ICF_TRACE_MEDIA_ONGOING)
        if(ICF_INT_MODULE_INVALID == ICF_CC_GET_MEDIA_ONGOING(p_cc_pdb))
        {
            /*
             *  Set the media_state bitmask in the call context
             */
            ICF_CC_TRACE(p_cc_pdb, ICF_TRACE_CALL_MUTE_SET)
            ICF_CC_SET_MEDIA_STATE(p_cc_pdb,ICF_MEDIA_STATE_CALL_MUTE)
             /*
              * Send Modify Media Session to the MMI
              */
            if (ICF_FAILURE == icf_cc_send_mms_req(p_cc_pdb))
            {
                ret_val = ICF_FAILURE;
            }
            else
            {
                if(ICF_TRUE == p_cc_pdb->mms_sent)
                {
                    /* If mms was sent then only we neet to change call state
                     * else no need to change call state */

                    /*
                     * Update the states in the call context
                     */
                    ICF_CC_SET_NEXT_STATE(p_cc_pdb,
                           ICF_CC_GET_CALL_STATE(p_cc_pdb))
                    ICF_CC_SET_CALL_STATE(p_cc_pdb,ICF_CC_04_AWT_MMS_RESP)
                    
                    ICF_CC_SET_ONGOING_EVENT(p_cc_pdb,
                        ICF_CC_CALL_MUTE_REQ)
                } /* MMS was sent to MMI succesfully */

            }/*  MMS was handled succesfully */

        } /* if(ICF_NULL == ICF_CC_GET_MEDIA_... */

        else
        {
            /*
             * Mark call mute as pending event
             */
            ICF_CC_TRACE(p_cc_pdb, ICF_TRACE_EVENT_CALL_MUTE)
            ICF_CC_SET_PENDING_EVENT(p_cc_pdb,ICF_PENDING_EVENT_CALL_MUTE)

            ICF_CC_SET_NEXT_STATE(p_cc_pdb,
                  ICF_CC_GET_CALL_STATE(p_cc_pdb))
            ICF_CC_SET_CALL_STATE(p_cc_pdb,
                  ICF_CC_13_WAIT_START_PROC_MEDIA)
        } /* else */
    } /* if(ICF_TRUE == p_cc_pdb->p_... */
    else
    {
        /*
         *  Set the media_state bitmask in the call context
         */
        ICF_CC_TRACE(p_cc_pdb, ICF_TRACE_CALL_MUTE_SET)
        ICF_CC_SET_MEDIA_STATE(p_cc_pdb,ICF_MEDIA_STATE_CALL_MUTE)
    } /* else */
    
    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */
    return ret_val;
}
#endif

/******************************************************************************
 *
 * FUNCTION:        icf_cc_05_call_unmute_req
 * 
 * DESCRIPTION:     This function is invoked when the CC is awaiting 
 *                  CONNECT from the PA and it receives Call 
 *                  Unmute Request from PA.
 *                  Current State: ICF_CC_05_AWT_CONNECT
 *                  Current Event: ICF_CC_CALL_UNMUTE_REQ
 *
 *****************************************************************************/
#if 0
icf_return_t icf_cc_05_call_unmute_req(
        INOUT icf_cc_pdb_st *p_cc_pdb)
{
    icf_return_t     ret_val = ICF_SUCCESS;

    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /* function entry trace */

    /*
     * If media is committed and media_ongoing is false
     * send MMS to MMI else mark as pending event and call is not an incomming 
     * call
     */
    ICF_CC_TRACE(p_cc_pdb, ICF_TRACE_MEDIA_COMMITTED)
    if(ICF_TRUE == p_cc_pdb->p_call_ctx->media_committed &&
           ICF_NULL == ICF_CC_CHECK_COMMON_BITMASK(p_cc_pdb,
                        ICF_CALL_DIR))
    {
        /* Reset call mute from media state */
        ICF_CC_TRACE(p_cc_pdb, ICF_TRACE_MEDIA_ONGOING)
        if(ICF_INT_MODULE_INVALID == ICF_CC_GET_MEDIA_ONGOING(p_cc_pdb))
        {
            /*
             *  reset the media_state bitmask in the call context
             */
            
            ICF_CC_RESET_MEDIA_STATE(p_cc_pdb,ICF_MEDIA_STATE_CALL_MUTE)
            ICF_CC_TRACE(p_cc_pdb, ICF_TRACE_CALL_MUTE_RESET)
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
                 * remain in this state */
                if(ICF_TRUE == p_cc_pdb->mms_sent)
                {
                    /* If mms was sent then only we neet to change call state
                     * else no need to change call state */

                   /*
                    * Update the states in the call context
                    */
                    ICF_CC_SET_NEXT_STATE(p_cc_pdb,
                         ICF_CC_GET_CALL_STATE(p_cc_pdb))
                    ICF_CC_SET_CALL_STATE(p_cc_pdb,ICF_CC_04_AWT_MMS_RESP)
                }
            } /* else */
        } /* end of if */
        else
        {
            /*
             * Mark call unmute as pending event
             */
            ICF_CC_TRACE(p_cc_pdb, ICF_TRACE_EVENT_CALL_UNMUTE)
            ICF_CC_SET_PENDING_EVENT(p_cc_pdb,
                    ICF_PENDING_EVENT_CALL_UNMUTE)
            /* Update call state */ 
            ICF_CC_SET_NEXT_STATE(p_cc_pdb,
                   ICF_CC_GET_CALL_STATE(p_cc_pdb))
            ICF_CC_SET_CALL_STATE(p_cc_pdb,
                   ICF_CC_13_WAIT_START_PROC_MEDIA)
        } /* else */
    } /* if(ICF_TRUE == p_cc_pdb->p_... */
    else
    {
        /*
         *  Set the media_state bitmask in the call context
         */
        ICF_CC_TRACE(p_cc_pdb, ICF_TRACE_CALL_MUTE_RESET)
        ICF_CC_RESET_MEDIA_STATE(p_cc_pdb,ICF_MEDIA_STATE_CALL_MUTE)
    }
    
    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */
    return ret_val;
}
#endif


/******************************************************************************
 *
 * FUNCTION:        icf_cc_05_mm_error_ind
 *
 * DESCRIPTION:     This function is invoked when the CC is awaiting 
 *                  CONNECT from the PA and MM error indication
 *                  is received from MMI
 *                  Current State: ICF_CC_05_AWT_CONNECT
 *                  Current Event: ICF_CC_MM_ERR_IND
 *
 *****************************************************************************/
icf_return_t icf_cc_05_mm_error_ind(
        INOUT icf_cc_pdb_st *p_cc_pdb)
{
    icf_return_t     ret_val = ICF_SUCCESS;

    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /* function entry trace */
    
    /*
     * As the handling for this event is the same in the 
     * the ICF_CC_03_AWT_ALERT_RESP state,call 
     * their handler
     */
    ret_val = icf_cc_03_mm_error_ind(p_cc_pdb);

    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */
    return ret_val;
}


/******************************************************************************
 *
 * FUNCTION:        icf_cc_05_timer_expiry
 *
 * DESCRIPTION:     This function is invoked when the CC is awaiting 
 *                  CONNECT from the PA and Setup Timer expires.
 *                  Current State: ICF_CC_05_AWT_CONNECT
 *                  Current Event: ICF_CC_TIMER_EXPIRY
 *
 *****************************************************************************/
icf_return_t icf_cc_05_timer_expiry(
        INOUT icf_cc_pdb_st *p_cc_pdb)
{
    icf_return_t     ret_val = ICF_SUCCESS;

    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /* function entry trace */

    /*
     * As the handling for this event is the same in the 
     * the ICF_CC_03_AWT_ALERT_RESP state,call 
     * their handler
     */
    ret_val = icf_cc_03_timer_expiry(p_cc_pdb);

    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */
    return ret_val;
}


/******************************************************************************
 *
 * FUNCTION:        icf_cc_05_pa_call_connect
 *
 * DESCRIPTION:     This function is invoked when the CC is awaiting 
 *                  CONNECT from the PA(incoming call) and
 *                  call CONNECT is received from the PA.
 *                  Current State: ICF_CC_05_AWT_CONNECT
 *                  Current Event: ICF_CC_PA_CALL_CONNECT
 *
 *****************************************************************************/
icf_return_t icf_cc_05_pa_call_connect(
        INOUT icf_cc_pdb_st *p_cc_pdb)
{
    icf_return_t     ret_val = ICF_SUCCESS;
ICF_SESSION_CONTROL_START
	icf_pa_cc_connect_req_st			*p_connect = ICF_NULL;
ICF_SESSION_CONTROL_END

    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /* function entry trace */

ICF_SESSION_CONTROL_START

	p_connect = (icf_pa_cc_connect_req_st*)
						p_cc_pdb->p_glb_pdb->p_recv_msg->payload;
    /*  Fix for SPR 20078 */
    /*  If pa connect is received and cal;l modify is ongoing then reject the
     *  request.
     */
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
    ICF_CC_TRACE(p_cc_pdb, ICF_TRACE_MEDIA_COMMITTED)

    /* In tunnel mode, we will not process stream list and the relevant 
       code will be bypassed. Also on receiving connect request from the
       application, we will not do a tunnelled MMS request and thus
       the check for media_committed in the cal context also becomes 
       redundant. In tunnel mode, on receiving connect, we will just 
       send 200 ok to SSA with the local SDP as populated through
       the tunnelled CMS response.
    */   
       
ICF_CHECK_TUNNEL_MODE_START(p_cc_pdb->p_glb_pdb,ICF_FALSE)
    /*
     * Check if it is a an incomming NFS call and media is not committed
     *Send connect to ssa and set next state as ICF_CC_11_AWT_SSA_CONNECT_ACK
     */
    if(ICF_TRUE != p_cc_pdb->p_call_ctx->media_committed &&
            (ICF_NULL == ICF_CC_CHECK_COMMON_BITMASK(p_cc_pdb,
                           ICF_CALL_MEDIA_SETUP_TYPE)))
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
                              ICF_CC_SET_RELEASE_CAUSE(p_cc_pdb,ICF_REL_CAUSE_SERV_INT_ERR)    
		
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
    /*
     * If media is commited and it was a fast start call
     */
    else if(ICF_TRUE == p_cc_pdb->p_call_ctx->media_committed)
    {
        if (ICF_CONNECT_DELETED_STREAM_PRESENT == 
		(p_connect->bit_mask & ICF_CONNECT_DELETED_STREAM_PRESENT))
        {
           p_cc_pdb->count = p_connect->count;
           p_cc_pdb->p_generic_ptr_1 = p_connect->deleted_media_streams;
           ret_val = icf_cc_check_for_streams_deleted_by_user(p_cc_pdb);
           if (ICF_CC_ALL_STREAMS_DELETED == (p_cc_pdb->common_bitmask 
                                           & ICF_CC_ALL_STREAMS_DELETED))
           {
             /* p_cc_pdb->p_call_ctx->release_cause = ICF_CALL_CLEAR_REASON_REQUEST_TERMINATED;*/	
	
              ICF_CC_SET_RELEASE_CAUSE(p_cc_pdb,ICF_REL_CAUSE_SERV_INT_ERR)	
              ret_val = icf_cc_05_generic_err(p_cc_pdb);
              ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb);
              return ret_val;
           }
        }
        ICF_CC_TRACE(p_cc_pdb, ICF_TRACE_MEDIA_ONGOING)
        if(ICF_INT_MODULE_INVALID == ICF_CC_GET_MEDIA_ONGOING(p_cc_pdb))
        {
            ret_val = icf_cc_snd_mms_pa_connect(p_cc_pdb);

        }
        else
        {
            /*
             * Mark PA connect as pending event
             */
            ICF_CC_TRACE(p_cc_pdb, ICF_TRACE_EVENT_PA_CONNECT)

            ICF_CC_SET_PENDING_EVENT(p_cc_pdb,
                    ICF_PENDING_EVENT_PA_CALL_CONNECT)

            ICF_CC_SET_NEXT_STATE(p_cc_pdb,
                    ICF_CC_11_AWT_SSA_CONNECT_ACK)
                
            ICF_CC_SET_CALL_STATE(p_cc_pdb,
                    ICF_CC_13_WAIT_START_PROC_MEDIA)
        }
    }
    else
    {
        ICF_CC_SET_PENDING_EVENT(p_cc_pdb,
                    ICF_PENDING_EVENT_PA_CALL_CONNECT)
        ICF_CC_SET_CALL_STATE(p_cc_pdb,
                    ICF_CC_11_AWT_SSA_CONNECT_ACK)
    }
ICF_CHECK_TUNNEL_MODE_END

    /* Tunnel mode handling of connect request from application.*/
ICF_CHECK_TUNNEL_MODE_START(p_cc_pdb->p_glb_pdb,ICF_TRUE)
    /* This function will send 200 to the peer.*/
    if((ICF_NULL != ICF_CC_CHECK_COMMON_BITMASK(p_cc_pdb,
                                        ICF_CALL_MEDIA_SETUP_TYPE)))
    {
        p_cc_pdb->p_call_ctx->media_committed = ICF_TRUE;
    }
    ret_val = icf_cc_snd_mms_pa_connect(p_cc_pdb);
    if(ICF_SUCCESS == ret_val)
    {
        ICF_CC_SET_CALL_STATE(p_cc_pdb,ICF_CC_11_AWT_SSA_CONNECT_ACK)
    }
ICF_CHECK_TUNNEL_MODE_END

    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */
    return ret_val;
}


/******************************************************************************
 *
 * FUNCTION:        icf_cc_05_ssa_call_cancel_req
 *
 * DESCRIPTION:     This function is invoked when the CC is awaiting
 *                  CONNECT from the PA(incoming call) and
 *                  it receives a Call Cancel Request from SSA
 *                  (SIP CANCEL received from peer).
 *                  Current State: ICF_CC_05_AWT_CONNECT
 *                  Current Event: ICF_CC_SSA_CALL_CANCEL_REQ
 *
 *****************************************************************************/

icf_return_t icf_cc_05_ssa_call_cancel_req(
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
	 * Since we are call the same handler in state 5 so we should reset this bit 
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
 * FUNCTION:        icf_cc_05_generic_err
 *
 * DESCRIPTION:     This function is invoked when the CC is awaiting
 *                  create media session resp from MMI and it receives
 *                  an error indication from MMI..
 *                  Current State: ICF_CC_05_AWT_CONNECT
 *                  Current Event: ICF_CC_GENERIC_ERROR
 *
 *****************************************************************************/

icf_return_t icf_cc_05_generic_err (
        INOUT icf_cc_pdb_st *p_cc_pdb)
{
    icf_return_t     ret_val = ICF_SUCCESS;

    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /* function entry trace */
    
    ret_val = icf_cc_03_generic_err(p_cc_pdb); 
    
    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */
    return ret_val;
}

/******************************************************************************
 *
 * FUNCTION:        icf_cc_05_forceful_call_clear
 *
 * DESCRIPTION:     This function is invoked when Forceful call clear is 
 *                  received in state ICF_CC_05_AWT_CONNECT 
 *                  Current State: ICF_CC_05_AWT_CONNECT
 *                  Current Event: ICF_CC_FORCEFUL_CLEAR
 *
 *****************************************************************************/
icf_return_t icf_cc_05_forceful_call_clear(
        INOUT icf_cc_pdb_st *p_cc_pdb)
{
    icf_return_t ret_val = ICF_SUCCESS;
    
    ICF_CC_SET_RELEASE_CAUSE(p_cc_pdb,ICF_REL_CAUSE_SERV_INT_ERR)    
        
    /*Handling would be same as that of forceful call clear in atate IDLE*/
    icf_cc_03_forceful_call_clear(p_cc_pdb);

    return ret_val;

}

/*twc_update handling*/
/******************************************************************************
 *
 * FUNCTION:        icf_cc_05_pa_call_modify_req
 *
 * DESCRIPTION:     This function is invoked when the CC is awaiting
 *                  200 OK and it receives a call modify request from PA.
 *
 *                  Current State: ICF_CC_05_AWT_CONNECT
 *                  Current Event: ICF_CC_PA_CALL_MODIFY_REQ
 *
 *****************************************************************************/
icf_return_t icf_cc_05_pa_call_modify_req(
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
ICF_CHECK_TUNNEL_MODE_START(p_cc_pdb->p_glb_pdb, ICF_FALSE)
    /* CSr 1-6188936: SPR 18835: ICF_UPDATE_PRESENT_IN_ALLOW_HDR is now a bitmask*/
	if(ICF_UPDATE_PRESENT_IN_ALLOW_HDR & p_cc_pdb->p_call_ctx->allowed_value)
    {
   
        ICF_CC_SET_COMMON_BITMASK(p_cc_pdb, ICF_CALL_MOD_REQ_WO_SDP);

        /* SPR 19590 : code removed for cloning header/tag/body list from 
         * here as this shall be taken care in icf_cc_convert_api_to_event
         * function.
         */

        if ((ICF_SUCCESS == ret_val) && (ICF_SUCCESS ==
                          icf_cc_ssa_send_update_msg(p_cc_pdb)))
        {
            ret_val = ICF_SUCCESS;
        } 
        else
        {
            ret_val = ICF_FAILURE;
        }
    }
    /*Send failure response to PA if remote does not supports UPDATE*/
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
ICF_CHECK_TUNNEL_MODE_START(p_cc_pdb->p_glb_pdb, ICF_TRUE)
    /* Rel 8.1 If allow header contains the UPDATE
	 * then send update other wise reject request 
     */
    if(ICF_UPDATE_PRESENT_IN_ALLOW_HDR & p_cc_pdb->p_call_ctx->allowed_value)
    {
         /*  Rel 8.1
         *  If CALL_MODIFY_received without SDP
         *  Send UPDATE without SDP and remain in same state
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
            /*  is ret_val is failure free the memory taken for
             *  to tag.
             */
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
             /*SPR 19473 use prack_reqd instead of default settings */
             if(ICF_NULL != ICF_CC_CHECK_COMMON_BITMASK(p_cc_pdb,
                    ICF_CALL_DIR) && ((ICF_FALSE == p_cc_pdb->p_call_ctx->media_committed) ||
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

            else if((ICF_NULL == ICF_CC_CHECK_COMMON_BITMASK(p_cc_pdb,
                        ICF_CALL_DIR)) && 
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
                        /* Fix for SPR:20101*/
                        /* No timer started for MMS response , currently setup timer will
                         * be running so we cannot start a new timer.Setup
                         * timer will guard the MMS request.*/
                        ICF_CC_SET_ONGOING_EVENT(p_cc_pdb,p_cc_pdb->current_event)
                        ICF_CC_SET_CALL_STATE(p_cc_pdb,ICF_CC_04_AWT_MMS_RESP)
                        ICF_CC_SET_NEXT_STATE(p_cc_pdb,ICF_CC_05_AWT_CONNECT)
                    }
                    /*  Free the memory of to tag if failure
                     */
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
                }/* success of icf_cc_copy_committed_sdp*/
            }/*  It is the case when UPDATE with SDP can be sent */
        }/*  case of CALL MODIFY REQ with SDP*/
    }/*  Success case of UPDATE is allowed */
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
 * FUNCTION:        icf_cc_05_media_prack_resp
 *
 * DESCRIPTION:     This function is invoked when the CC is awaiting 200 OK
 *                  Response and remote sends response for PRACK
 *                  of 180.
 *                  Current State: ICF_CC_03_AWT_CONNECT
 *                  Current Event: ICF_CC_MEDIA_PARAM_PRACK_RESP
 *
 *****************************************************************************/
icf_return_t icf_cc_05_media_prack_resp(icf_cc_pdb_st *p_cc_pdb)
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

