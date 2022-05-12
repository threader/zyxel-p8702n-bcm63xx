/*------------------------------------------------------------------------------
 *
 * File name        : icf_cc_fsm_14.c
 *
 * Purpose          : This file contains the event handlers in
 *                    ICF_CC_14_AWT_CALL_MODIFY_RESP state.
 *
 * Revision History :
 *
 * Date         Name            Ref#            Comments
 * --------     ------------    ----            ---------------
 * 22-Feb-2006  Anuradha						 Initial
 * 30-Mar-2006  Anuradha						 Call Modify Additions
 * 26-Jun-2006  Amit Sharma     ICF 6.0 APIs     Structure changes for multiple
 *                                               m-lines
 * 31-Jul-2006  Anuradha        ICF 6.0          Multiple m-lines code changes
 * 17-Apr-2007  Abhishek Dutta  ICF 6.3          Added the changes for 
 *                                               TWC-UPDATE Handling
 * 20-Jun-2007  Tarun Gupta     SPR 15124        In the function icf_cc_14_ssa_call_modify_resp,
 *                                               we are freeing the SDP received in 200 Ok after
 * 10-July-2007 Abhishek Dutta  ICF 7.0          Added the changes for 
 *                                               Cancel Call Modify Req 
 *                                               Handling
 *                                               sending call_modify_ind to application
 * 10-Jul-2007  Neha Choraria   ICF 7.0          Changes for Re-Invite w/o SDP
 * 11-Jul-2007  Neha Choraria   ICF 7.0          Changes for Retry-After header
 * 10-Jul-2007	Anuradha Gupta  ICF 7.0          Partial Offer reject from application changes
 * Copyright (c) 2006, Aricent.
 *---------------------------------------------------------------------------*/

#include "icf_ecode.h"
#include "icf_cc_prototypes.h"
#include "icf_cc_macro.h"
#include "icf_internal_api_id.h"
#include "icf_dbm_prototypes.h"

ICF_HANDLE_REINVITE_START
/*******************************************************************************
 *
 * FUNCTION:        icf_cc_14_pa_call_modify_resp
 *
 * DESCRIPTION:     This function is invoked when the CC is in awaiting the response 
 *                  for the REINVITE request and it receives the RSP from PA
 *                  Current State: ICF_CC_14_AWT_CALL_MODIFY_RESP
 *                  Current Event: ICF_CC_PA_CALL_MODIFY_RSP
 *
 ******************************************************************************/
icf_return_t icf_cc_14_pa_call_modify_resp(
        INOUT icf_cc_pdb_st *p_cc_pdb)
{
	icf_return_t     ret_val = ICF_SUCCESS;
	icf_pa_cc_call_modify_cfm_st *p_call_modify_cfm = ICF_NULL;

	ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /* function entry trace */

	ret_val = icf_cc_stop_current_timer(p_cc_pdb);

	/* check the call direction , it should be an incoming call modify request here */
	if(ICF_NULL != ICF_CC_CHECK_COMMON_BITMASK(p_cc_pdb, ICF_CALL_MODIFY_DIR))
	{
		p_call_modify_cfm = (icf_pa_cc_call_modify_cfm_st *)
						p_cc_pdb->p_glb_pdb->p_recv_msg->payload;

		/* check the result for accept or reject */
		if(ICF_TRUE == p_call_modify_cfm->result)
		{
			/* Accept */

			/* copy the additional parameters from payload into call ctx */
			if (ICF_FAILURE == icf_cc_proc_pa_media_change_payload(p_cc_pdb))
			{
				ret_val = ICF_FAILURE;
			}
            else if(ICF_CALL_MODIFY_CFM_REJECTED_STREAM_PRESENT ==
                 (ICF_CALL_MODIFY_CFM_REJECTED_STREAM_PRESENT &
                      p_call_modify_cfm->bit_mask))
            {
                ICF_CC_SET_COMMON_BITMASK_2(p_cc_pdb,
                   ICF_CALL_MOD_CFM_PARTIAL_OFFER_REJECT)

                /*application has partially rejected the offer , we will
                  have to revert the changes for the rejected stream */

                ret_val = icf_cc_derive_mms_type_app_partial_reject(p_cc_pdb);
                if (ICF_FAILURE == ret_val)
                {
                   /*Case when the incoming offer is for media line delete 
                      and the request is rejected by application in rejected list*/
                    ret_val = icf_cc_14_generic_err(p_cc_pdb);
                    return ret_val ;
                } 

                if(ICF_NULL != 
                   ICF_CC_CHECK_PDB_COMMON_BITMASK(p_cc_pdb,
                       ICF_CC_ALL_STREAMS_DELETED))
                {
                   ICF_CC_RESET_COMMON_BITMASK_2(p_cc_pdb,
                      ICF_CALL_MOD_CFM_PARTIAL_OFFER_REJECT)

                    if(ICF_TRUE == p_cc_pdb->p_call_ctx->media_updated)
                    {
                      /* application has rejected all streams modification
                       * we should be sending 4xx response for it*/

                       if(ICF_FAILURE == icf_cc_send_mms_req(p_cc_pdb))
                       {
                          ret_val = ICF_FAILURE;
                       }
                       else if(ICF_TRUE == p_cc_pdb->mms_sent)
                       {
                             /*
                              * Update the states in the call context
                              */
                             ICF_CC_SET_CALL_STATE(p_cc_pdb,ICF_CC_04_AWT_MMS_RESP)
                             ICF_CC_SET_NEXT_STATE(p_cc_pdb,ICF_CC_06_CONNECTED)
                             ICF_CC_SET_ONGOING_EVENT(p_cc_pdb,
                                    ICF_CC_ONGOING_EVENT_REMOTE_MEDIA_REVERT)
                             /* Start the media timer  */
                             ret_val = icf_cc_start_media_timer(p_cc_pdb);
                        }
                     }
                     else if(ICF_FAILURE == icf_cc_send_ssa_offer_chg_err(p_cc_pdb))
                     {
                           ret_val = ICF_FAILURE;
                     }
                     else
                     {
                          ICF_CC_SET_CALL_STATE(p_cc_pdb,ICF_CC_06_CONNECTED)
                     }
                      
                }
                else if(ICF_TRUE ==  p_cc_pdb->p_call_ctx->media_updated)
                {
                    if(ICF_FAILURE == icf_cc_send_mms_req(p_cc_pdb))
                    {
                       ret_val = ICF_FAILURE;
                    }
                    else if(ICF_TRUE == p_cc_pdb->mms_sent)
                    {
                        /*
                        * Update the states in the call context
                        */
                        ICF_CC_SET_CALL_STATE(p_cc_pdb,ICF_CC_04_AWT_MMS_RESP)
                        ICF_CC_SET_NEXT_STATE(p_cc_pdb,ICF_CC_06_CONNECTED)
                        ICF_CC_SET_ONGOING_EVENT(p_cc_pdb,
                                    ICF_CC_ONGOING_EVENT_REMOTE_MEDIA_REVERT)
                        /* Start the media timer  */
                        ret_val = icf_cc_start_media_timer(p_cc_pdb);
                     }
                 }
                 else if(ICF_FAILURE == icf_cc_send_ssa_offer_chg_rsp(p_cc_pdb))
                 {
                       ret_val = ICF_FAILURE;
                 }
                 else
                 {
                      ICF_CC_SET_CALL_STATE(p_cc_pdb,ICF_CC_06_CONNECTED)
                 }
            }
			else if (ICF_FAILURE == icf_cc_send_ssa_offer_chg_rsp(p_cc_pdb)) 	
			{ 		
				ret_val = ICF_FAILURE; 	
			} 	
			else 	
			{ 		
				ICF_CC_SET_CALL_STATE(p_cc_pdb,ICF_CC_06_CONNECTED) 	
                                p_cc_pdb->p_call_ctx->media_updated = ICF_FALSE;
			} 

		}
		else
		{
			if(ICF_CAUSE_REJECT_MEDIA_CHANGE_REQ == p_call_modify_cfm->cause)
			{
				p_cc_pdb->p_call_ctx->offer_failure_reason = 
					ICF_CALL_MODIFY_REASON_REJECT_MEDIA_CHANGE;
			}
			else
			{
				p_cc_pdb->p_call_ctx->offer_failure_reason = 
					(icf_ssa_reason_t)p_call_modify_cfm->cause;
			}
			/* this response may go missing check out !!!!!! */
                     
            if(p_call_modify_cfm->bit_mask & ICF_CALL_MODIFY_RESPONSE_CODE_PRESENT)
            {
                p_cc_pdb->p_call_ctx->response_code = p_call_modify_cfm->nw_response_code;
            }
            else
            {
                p_cc_pdb->p_call_ctx->response_code = ICF_NULL;
            }
            /* Reject */

            /* revert the changes and then send response 
			 seems not required now, last committed will be used for revert*/	

			if(ICF_TRUE == p_cc_pdb->p_call_ctx->media_updated)
			{
                ret_val = icf_cc_derive_mms_type_for_revert_remote_change(p_cc_pdb);
                
                if (ICF_FAILURE == ret_val)
                {
                   /*Case when the incoming offer is for media line delete 
                      and the request is completely rejected by application*/
                    ret_val = icf_cc_14_generic_err(p_cc_pdb);
                    return ret_val ;
                } 

				if(ICF_FAILURE == icf_cc_send_mms_req(p_cc_pdb))
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
						ICF_CC_SET_CALL_STATE(p_cc_pdb,ICF_CC_04_AWT_MMS_RESP)
						ICF_CC_SET_NEXT_STATE(p_cc_pdb,ICF_CC_06_CONNECTED)
						ICF_CC_SET_ONGOING_EVENT(p_cc_pdb,
									ICF_CC_ONGOING_EVENT_REMOTE_MEDIA_REVERT)
						/* Start the media timer  */
						ret_val = icf_cc_start_media_timer(p_cc_pdb);
					}
				}/* MMS was handled succesfully */
            }			
            else if(ICF_FAILURE == icf_cc_send_ssa_offer_chg_err(p_cc_pdb))
		    {
		            ret_val = ICF_FAILURE;
		    }
		    else
		    {
		        	ICF_CC_SET_CALL_STATE(p_cc_pdb,ICF_CC_06_CONNECTED)
			}

           /******* Changes for Re-Invite w/o SDP Start (Rel7.0) *******/

                /*reset the common bitmask if it is set as
                   ICF_CALL_MOD_REQ_WO_SDP*/

                   if (ICF_NULL != ICF_CC_CHECK_COMMON_BITMASK
                             (p_cc_pdb,ICF_CALL_MOD_REQ_WO_SDP))
                   {
                          ICF_CC_RESET_COMMON_BITMASK(p_cc_pdb,
                                          ICF_CALL_MOD_REQ_WO_SDP)
                   }

          /******** Changes for Re-Invite w/o SDP End (Rel7.0) ********/


		}
              ICF_CC_RESET_COMMON_BITMASK(p_cc_pdb,ICF_CALL_MODIFY_DIR) 
	}

	ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function entry trace */

	return ret_val;
}

/*******************************************************************************
 *
 * FUNCTION:        icf_cc_14_ssa_call_modify_resp
 *
 * DESCRIPTION:     This function is invoked when the CC is in awaiting the  
 *                  response for the REINVITE request and it receives the RSP 
 *					from SSA.
 *                  Current State: ICF_CC_14_AWT_CALL_MODIFY_RESP
 *                  Current Event: ICF_CC_MEDIA_UPDATE_RESP
 *
 ******************************************************************************/
icf_return_t icf_cc_14_ssa_call_modify_resp(
        INOUT icf_cc_pdb_st *p_cc_pdb)
{
	icf_return_t     ret_val = ICF_SUCCESS;
	icf_nw_inc_media_change_resp_st *p_call_modify_resp = ICF_NULL;
        icf_uint8_t      ongoing_event = 0;
        

	ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /* function entry trace */

	ICF_CC_RESET_SIG_ONGOING(p_cc_pdb)

    /*Fetch ongoing event*/
    ongoing_event =  ICF_CC_GET_ONGOING_EVENT(p_cc_pdb);

    /* Return status as signalling done to CM */
    ICF_CC_SET_STATUS(p_cc_pdb, ICF_STATUS_SIGNAL_ONGOING_ENDED)

	p_call_modify_resp = (icf_nw_inc_media_change_resp_st *)
				p_cc_pdb->p_internal_msg->p_msg_data;

    if((ICF_TRUE == p_call_modify_resp->status) ||
       ((ICF_FALSE == p_call_modify_resp->status) &&
            (ICF_CALL_CLEAR_REASON_REQUEST_PENDING != 
                           p_call_modify_resp->reason_type))) 
    {
       ret_val = icf_cc_stop_current_timer(p_cc_pdb);
    } 
   
    if(ICF_FAILURE != ret_val)
    {

/*twc_update handling*/

 	/*If common_bitmask is UPDATE_WO_SDP then send response to application
         *set next_state to call state*/
  
/*twc_update handling*/

	/* check the call direction , it should be an 
	    outgoing call modify request here */
	if(ICF_NULL == ICF_CC_CHECK_COMMON_BITMASK(p_cc_pdb, ICF_CALL_MODIFY_DIR))
	{

                /*If common_bitmask is UPDATE_WO_SDP then send response to 
                 * application set next_state to call state*/
                if (ICF_NULL != ICF_CC_CHECK_COMMON_BITMASK(p_cc_pdb,ICF_CALL_MOD_REQ_WO_SDP))
                {
                    /*Send response without SDP*/
                    /*Send call modify_resp to application*/
                   if (ICF_FALSE == p_call_modify_resp->status)
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
                   
                   return ret_val;
                }
/*call_mod_cancel*/
                else if((!(ICF_CALL_MODIFY_PROV_RESP_RECVD &
                       p_cc_pdb->p_call_ctx->prov_resp_status)) &&
                       (ICF_NULL != ICF_CC_CHECK_PENDING_EVENT(p_cc_pdb,
                        ICF_PENDING_EVENT_PA_CALL_MOD_CANCEL_REQ)))
                {
                   /*This is the case when 200 OK from n/w is received and 100
                   trying is not reveived for reinv.It means that CANCEL is not 
                   sent to the n/w nor media revert took place.Hence send failure 
                   resp for cancel call modify request followed by success for call
                   modify request*/
 
                   /*Send failure response for call modify cancel request*/
                   ICF_CC_SET_COMMON_BITMASK(p_cc_pdb, ICF_CALL_MOD_CANCEL_RESP_FAIL)

                   p_cc_pdb->p_call_ctx->app_failure_cause =
                              ICF_CAUSE_CALL_MODIFY_TRANSACTION_COMPLETE;

                   /*Send call modify_cancel fail resp to application*/
                   if(ICF_FAILURE == icf_cc_send_pa_call_modify_cancel_resp(p_cc_pdb))
                   {
                      ret_val = ICF_FAILURE;
                      /* free the payload of 200 Ok of reinvite */
                      if(ICF_NW_INC_MC_SDP_PRESENT ==
                        (ICF_NW_INC_MC_SDP_PRESENT & p_call_modify_resp->bitmask))
                      {
                          if(ICF_NULL != p_call_modify_resp->p_sdp_list)
                          {
                             icf_cc_delete_inc_media_sdp_info_list(
                             p_cc_pdb,&p_call_modify_resp->p_sdp_list);
                          }
                      }
                      return ret_val;
                   }
                   /*Reset pending event as call modify cancel request is cancelled now*/
                   ICF_CC_RESET_PENDING_EVENT(p_cc_pdb,
                                               ICF_PENDING_EVENT_PA_CALL_MOD_CANCEL_REQ) 

                }
                else if(ICF_CC_PA_CALL_MODIFY_CANCEL_REQ == ongoing_event)
                {
                  /*This is the case when either :
                    1. media revert for cancel call modify request 
                       is going on(CANCEL is not sent)and 200 OK of Re-Invite is rcvd
                    2. media revert for cancel call modify request
                       is going on(CANCEL is not sent)and 4xx resp of Re-Invite is rcvd
                  */

                  /*Set call state as awt mms resp and wait for the resp from MMI*/
                  ICF_CC_SET_CALL_STATE(p_cc_pdb,ICF_CC_04_AWT_MMS_RESP) 

                  /* free the payload of 200 Ok of reinvite */  
                  if(ICF_NW_INC_MC_SDP_PRESENT ==
                     (ICF_NW_INC_MC_SDP_PRESENT & p_call_modify_resp->bitmask))
                  {
                      if(ICF_NULL != p_call_modify_resp->p_sdp_list)
                      {
                         ret_val = icf_cc_delete_inc_media_sdp_info_list(
                         p_cc_pdb,&p_call_modify_resp->p_sdp_list);
                      }
                  }    
                  return ret_val;
                }
                else if(ICF_NULL == 
                        ICF_CC_CHECK_COMMON_BITMASK_2(p_cc_pdb,ICF_CALL_MOD_CANCEL_REQ) &&
                       (ICF_NULL != 
                        ICF_CC_CHECK_COMMON_BITMASK_2(p_cc_pdb, ICF_CALL_MOD_CANCEL_REINV_REQ))) 
                {
                   /*This is the case when Re-Invite is sent second time after receiving 
                    *200 OK of Re-Invite before 200 OK of CANCEL.
                    *After mms again re-invite is sent to n/w */

                  /*Send Re-Invite again*/
                  if (ICF_FAILURE == icf_cc_ssa_send_update_msg(p_cc_pdb))
                  {
                      ret_val = ICF_FAILURE;
                  }
                  else
                  {
                      ret_val = ICF_SUCCESS;
                  }
                 
                  /* free the payload of 200 Ok of reinvite */  
                  if(ICF_NW_INC_MC_SDP_PRESENT ==
                     (ICF_NW_INC_MC_SDP_PRESENT & p_call_modify_resp->bitmask))
                  {
                      if(ICF_NULL != p_call_modify_resp->p_sdp_list)
                      {
                         icf_cc_delete_inc_media_sdp_info_list(
                         p_cc_pdb,&p_call_modify_resp->p_sdp_list);
                      }
                  }    
                  return ret_val;
               }
               else if(ICF_CALL_MOD_REINV_CANCELLED ==
                          (ICF_CALL_MOD_REINV_CANCELLED & p_call_modify_resp->bitmask))
               {

                   /*This is the case when 4xx resp for Re-Invite is rcvd and 
                     CANCEL was already sent to n/w and waiting for the 200 OK
                     response from n/w*/
                   
                   ICF_CC_RESET_COMMON_BITMASK_2(
                                 p_cc_pdb,ICF_CALL_MOD_CANCEL_REQ)

                   /*Send call modify_cancel_resp to application*/
                   if(ICF_FAILURE == icf_cc_send_pa_call_modify_cancel_resp(p_cc_pdb))
                   {
                      ret_val = ICF_FAILURE;
                   }

                   if(ICF_FAILURE != ret_val)
                   {

                       /*Send failure response for call modify request*/
                       ICF_CC_SET_COMMON_BITMASK(p_cc_pdb, ICF_CALL_MODIFY_RESP_FAIL)

                       p_cc_pdb->p_call_ctx->app_failure_cause =
                                                 ICF_CAUSE_REQUEST_CANCELLED_BY_USER;
                       if(ICF_FAILURE == icf_cc_send_pa_call_modify_resp(p_cc_pdb))
                       {
                          ret_val = ICF_FAILURE;
                       }
                       else
                       {
                          ICF_CC_SET_CALL_STATE(p_cc_pdb,ICF_CC_06_CONNECTED)
                          ret_val = ICF_SUCCESS;
                       }
                       /* free the payload of 200 Ok of reinvite */
                       if(ICF_NW_INC_MC_SDP_PRESENT ==
                         (ICF_NW_INC_MC_SDP_PRESENT & p_call_modify_resp->bitmask))
                       {
                          if(ICF_NULL != p_call_modify_resp->p_sdp_list)
                          {
                             icf_cc_delete_inc_media_sdp_info_list(
                             p_cc_pdb,&p_call_modify_resp->p_sdp_list);
                          }
                       }
                       return ret_val;
                    }
               }
               else if(ICF_CALL_MOD_REINV_2_CANCELLED ==
                          (ICF_CALL_MOD_REINV_2_CANCELLED & p_call_modify_resp->bitmask))
               {
                   /*Send call modify_cancel_resp to application*/
                   ICF_CC_SET_COMMON_BITMASK(p_cc_pdb, ICF_CALL_MOD_CANCEL_RESP_FAIL)

                   p_cc_pdb->p_call_ctx->app_failure_cause =
                                               ICF_CAUSE_REJECT_MEDIA_CHANGE_REQ;
                   if(ICF_FAILURE == icf_cc_send_pa_call_modify_cancel_resp(p_cc_pdb))
                   {
                      ret_val = ICF_FAILURE;
                   }

                   /*Send call modify_resp to application*/
                   ICF_CC_SET_COMMON_BITMASK(p_cc_pdb, ICF_CALL_MODIFY_RESP_FAIL)

                   p_cc_pdb->p_call_ctx->app_failure_cause =
                                               ICF_CAUSE_REJECT_MEDIA_CHANGE_REQ;
                   if(ICF_FAILURE == icf_cc_send_pa_call_modify_resp(p_cc_pdb))
                   {
                      ret_val = ICF_FAILURE;
                   }

                   if(ICF_FAILURE == icf_cc_clear_call_at_call_modify(p_cc_pdb))
                   {
                      ret_val = ICF_FAILURE;
                   }
                  /*
	               * Set release type in call context as INTERNAL_INITIAITED
	               */
                  ICF_CC_TRACE(p_cc_pdb, ICF_TRACE_REL_TYPE_INT_ERR)
                  ICF_CC_RESET_RELEASE_TYPE(p_cc_pdb,ICF_REL_TYPE_PA_INITIATED)
                  ICF_CC_SET_RELEASE_TYPE(p_cc_pdb,ICF_REL_TYPE_INTERNAL_INITIATED)

                  return ret_val;
                   
               }

/* commeneted for CSR 1-4846648: We shall process remote sdp in answer in all
   cases previously we ignored the remote answer sdp if there was not change
   in applications offer when UPDATE was sent */
#if 0   
		if(ICF_TRUE == p_call_modify_resp->status &&  
                ICF_FALSE == p_cc_pdb->p_call_ctx->media_updated)
               {
        	/* Since the media was not updated which means there 
         	* was not change in applications offer and hence we 
         	* dont expect peer to send anything new in answer.
         	* Hence we would not copy the media received in 
         	* answer and would not be doing any MMS for the 
         	* same 
         	*/

         	if(ICF_TRUE == p_cc_pdb->p_call_ctx->update_reqd)
         	{
           		p_cc_pdb->common_bitmask |= ICF_CC_CALL_MODIFY_CFM_NT_REQD;

           		ret_val = icf_cc_send_pa_call_modify_ind(p_cc_pdb);
           		p_cc_pdb->p_call_ctx->update_reqd = ICF_FALSE;

         	}
			else 
         	{
           		ret_val = icf_cc_send_pa_call_modify_resp(p_cc_pdb);
         	}

         	if(ICF_SUCCESS == ret_val)
			{
				ICF_CC_SET_CALL_STATE(p_cc_pdb,ICF_CC_06_CONNECTED)        
          	}

            /* We are not copying the media received in 200 OK.So we would free
            the SDP received.
            */
            if(ICF_NW_INC_MC_SDP_PRESENT ==
                   (ICF_NW_INC_MC_SDP_PRESENT & p_call_modify_resp->bitmask))
              {
                  if(ICF_NULL != p_call_modify_resp->p_sdp_list)
                  {
                     ret_val = icf_cc_delete_inc_media_sdp_info_list(
                         p_cc_pdb,&p_call_modify_resp->p_sdp_list);
                  }
              }    
    	    }
#endif            
		if(ICF_TRUE == p_call_modify_resp->status)
		{
			/* Accept */
			/*
			1.	New MMS is required for the modifed streams.
			2.  Revert MMS required for rejected streams
			3.	send the answer to application after the MMS.
            */
            p_cc_pdb->p_list_1 = p_call_modify_resp->p_sdp_list;

            if(ICF_FAILURE == 
                icf_cc_make_mms_type_for_nth_rem_succ_ans(p_cc_pdb))
            {
                if(ICF_TRUE == p_cc_pdb->p_call_ctx->update_reqd)
                {
                    p_cc_pdb->p_call_ctx->update_reqd = ICF_FALSE;
                    ret_val = icf_cc_14_generic_err(p_cc_pdb);
                    return ret_val;
                }
                else
                {
                    p_cc_pdb->p_call_ctx->app_failure_cause = ICF_CAUSE_NETWORK_ERROR;			
			        ICF_CC_SET_COMMON_BITMASK(p_cc_pdb, 
                        ICF_CALL_MODIFY_RESP_FAIL)
                    icf_cc_send_pa_call_modify_resp(p_cc_pdb);
                    ret_val = ICF_FAILURE;
                }

                

                if (ICF_CC_STREAM_POS_VALIDATION_FAIL == 
				    (p_cc_pdb->common_bitmask & 
                        ICF_CC_STREAM_POS_VALIDATION_FAIL))
		        {
			        ret_val = icf_cc_14_generic_err(p_cc_pdb);
		        }
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
					ICF_CC_SET_CALL_STATE(p_cc_pdb,ICF_CC_04_AWT_MMS_RESP)
					ICF_CC_SET_NEXT_STATE(p_cc_pdb,ICF_CC_06_CONNECTED)
					ICF_CC_SET_ONGOING_EVENT(p_cc_pdb,p_cc_pdb->current_event)
					/* Start the media timer  */
					ret_val = icf_cc_start_media_timer(p_cc_pdb);
				}
                else
                {
                    if(ICF_TRUE == p_cc_pdb->p_call_ctx->update_reqd)
                    {
                        p_cc_pdb->common_bitmask |= ICF_CC_CALL_MODIFY_CFM_NT_REQD;

                        if(ICF_FAILURE == icf_cc_send_pa_call_modify_ind(p_cc_pdb))
                        {
                            ret_val = ICF_FAILURE;
                        }
                       
                        p_cc_pdb->p_call_ctx->update_reqd = ICF_FALSE;
                        
                    }
                    /*call_mod_cancel*/
                    else if(ICF_NULL != ICF_CC_CHECK_COMMON_BITMASK_2(p_cc_pdb,ICF_CALL_MOD_CANCEL_REQ) &&
                           (ICF_NULL != 
                             ICF_CC_CHECK_COMMON_BITMASK_2(p_cc_pdb, ICF_CALL_MOD_CANCEL_REINV_REQ)))
                    {
                        /*This is the case when 200 OK of 2nd Re-Invite is received and no mms      
                         *is sent due to identical media type hence send the response to PA*/
                        /*Send succ resp for call modify cancel req*/
                        /*Send failure response for call modify cancel to PA*/
                        if (ICF_FAILURE == icf_cc_send_pa_call_modify_cancel_resp(p_cc_pdb))
                        {
                           ret_val = ICF_FAILURE;
                        }
                        /* Send failure response for call modify req to PA */
                        p_cc_pdb->p_call_ctx->app_failure_cause = ICF_CAUSE_REQUEST_CANCELLED_BY_USER;
                        ICF_CC_SET_COMMON_BITMASK(p_cc_pdb, ICF_CALL_MODIFY_RESP_FAIL)
                        if (ICF_FAILURE == icf_cc_send_pa_call_modify_resp(p_cc_pdb))
                        {
                           ret_val = ICF_FAILURE;
                        }

                     }
                     else 
                     {
                        if (ICF_FAILURE == 
                            icf_cc_send_pa_call_modify_resp(p_cc_pdb))
                        {
                           ret_val = ICF_FAILURE;
                        }
                     }

                     ICF_CC_SET_CALL_STATE(p_cc_pdb,ICF_CC_06_CONNECTED)
                }
				
			}/* MMS was handled succesfully */

		}
		else
		{
			/* Trace that remote has rejected our offer completely i.e.4xx.*/
			ICF_CC_TRACE(p_cc_pdb,ICF_TRACE_OFFER_CHANGE_FAILURE)

			/*ANURADHA : fix for SI Case 4.1.5*/
			/*p_cc_pdb->app_failure_cause = 
                          (unsigned char)p_call_modify_resp->reason_type;*/
			if(ICF_CALL_CLEAR_MEDIA_MISMATCH == p_call_modify_resp->reason_type)
			{
				p_cc_pdb->p_call_ctx->app_failure_cause = ICF_CAUSE_REJECT_MEDIA_CHANGE_REQ;
			}

                        /********** Changes for Retry-After Start (Rel7.0) **********/

                        /* This code checks for the reason type. 
                         * If it is ICF_CALL_CLEAR_REASON_REQUEST_PENDING then CC should 
                         * start the retry after timer which is used in case of 
                         * 491 response received from peer.*/

            else if(ICF_CALL_CLEAR_REASON_REQUEST_PENDING == 
                                     p_call_modify_resp->reason_type)
             {
                    ICF_CC_SET_COMMON_BITMASK_2(p_cc_pdb, 
                                   ICF_CC_491_RESP_PROCESS_ONGOING)
                    ret_val = icf_cc_start_retry_after_timer(p_cc_pdb,
                                 ICF_NULL);
                    
                   ICF_CC_RESET_SIG_ONGOING(p_cc_pdb)
	              /* Return status as signalling done to CM */
                   ICF_CC_SET_STATUS(p_cc_pdb, 
                             ICF_STATUS_SIGNAL_ONGOING_ENDED)
                   return ret_val;
             }

                         /*********** Changes for Retry-After End (Rel7.0) ***********/

			 else
			 {
				p_cc_pdb->p_call_ctx->app_failure_cause = ICF_CAUSE_NETWORK_ERROR;
			 }

			 ICF_CC_SET_COMMON_BITMASK(p_cc_pdb, ICF_CALL_MODIFY_RESP_FAIL)
			

            

			 if(ICF_CALL_CLEAR_REASON_USER_NOT_FOUND == 
                         p_call_modify_resp->reason_type)
			 { 
               icf_cc_send_pa_call_modify_resp(p_cc_pdb);

				/* terminate the call for these error reasons */
				p_cc_pdb->p_call_ctx->release_cause = 
                                (icf_uint8_t)p_call_modify_resp->reason_type;
				ret_val = icf_cc_clear_call_at_call_modify(p_cc_pdb);
			 }
            else if(ICF_TRUE == p_cc_pdb->p_call_ctx->update_reqd)
            {  
               icf_cc_send_pa_call_modify_resp(p_cc_pdb);
               ret_val = icf_cc_clear_call_at_call_modify(p_cc_pdb);
               
                p_cc_pdb->p_call_ctx->update_reqd = ICF_FALSE;                
            }
			else
			{ 
				/* Reject */
				if( ICF_FALSE == p_cc_pdb->p_call_ctx->media_updated)
				{
					/* no MMS was done when the request was received , so no need to revert the
					   changes. Send the error response */

					if(ICF_FAILURE == icf_cc_send_pa_call_modify_resp(p_cc_pdb))
					{
						ret_val = ICF_FAILURE;
					}
					else
					{
						ICF_CC_SET_CALL_STATE(p_cc_pdb,ICF_CC_06_CONNECTED)        
					}
				}
				else
				{
					/* revert the changes and then send confirmation to PA */
                    /*
                    if(ICF_FAILURE == 
                          icf_cc_process_remote_call_modify_err_resp(p_cc_pdb))
                    */
                    icf_cc_derive_mms_remote_rejected(p_cc_pdb);
                   
			        if(ICF_CC_REMOTE_REJ_STREAM_DELETE == 
				        ( ICF_CC_REMOTE_REJ_STREAM_DELETE & p_cc_pdb->common_bitmask))
			        {
				        icf_cc_send_pa_call_modify_resp(p_cc_pdb);
				        ICF_CC_SET_RELEASE_TYPE(p_cc_pdb,ICF_REL_TYPE_INTERNAL_INITIATED)
				        ret_val = icf_cc_clear_call_at_call_modify(p_cc_pdb);
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
							ICF_CC_SET_CALL_STATE(p_cc_pdb,ICF_CC_04_AWT_MMS_RESP)
							ICF_CC_SET_NEXT_STATE(p_cc_pdb,ICF_CC_06_CONNECTED)
							ICF_CC_SET_ONGOING_EVENT(p_cc_pdb,
									ICF_CC_ONGOING_EVENT_LOCAL_MEDIA_REVERT)
							
							/* Start the media timer  */
							ret_val = icf_cc_start_media_timer(p_cc_pdb);
						}
						
					}/* MMS was handled succesfully */
				}
			}
		}
	}
	
	/* reset the prov resp for call modify flag*/
	p_cc_pdb->p_call_ctx->prov_resp_status &= (~(ICF_CALL_MODIFY_PROV_RESP_RECVD));

	ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function entry trace */

    }
    else 
    { 
       /* free the payload of 200 Ok of reinvite */  
       if(ICF_NW_INC_MC_SDP_PRESENT ==
         (ICF_NW_INC_MC_SDP_PRESENT & p_call_modify_resp->bitmask))
       {
          if(ICF_NULL != p_call_modify_resp->p_sdp_list)
          {
             icf_cc_delete_inc_media_sdp_info_list(
             p_cc_pdb,&p_call_modify_resp->p_sdp_list);
             
          }
       }    
    }
	return ret_val;
}

/*******************************************************************************
 *
 * FUNCTION:        icf_cc_14_ssa_offer_change
 *
 * DESCRIPTION:     This function is invoked when the CC is in awaiting the response 
 *                  for the REINVITE request and it receives another reinvite from SSA
 *                  Current State: ICF_CC_14_AWT_CALL_MODIFY_RESP
 *                  Current Event: ICF_CC_SSA_OFFER_CHANGE
 *
 ******************************************************************************/
icf_return_t icf_cc_14_ssa_offer_change(
        INOUT icf_cc_pdb_st *p_cc_pdb)
{
	icf_return_t     ret_val = ICF_SUCCESS;
    icf_nw_inc_media_change_req_st *p_offer_chg_req = ICF_NULL;

    icf_uint8_t *p_msg_data =ICF_NULL; 
    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /* function entry trace */


    p_msg_data = p_cc_pdb->p_internal_msg->p_msg_data; 
             
    p_offer_chg_req =
                (icf_nw_inc_media_change_req_st *)p_cc_pdb->p_internal_msg->
					p_msg_data;

    if (ICF_NULL == p_offer_chg_req)
    {
       return ICF_FAILURE;
    }

        /************ Changes for Retry-After Start (Rel7.0) ***********/

        /* A code segment is added to handle the 
         * reinvite from remote side while the 491 has been received of 
         * outgoing reinvite and a retry after timer is running 
         * to again send the reinvite */

        if(ICF_NULL != (p_cc_pdb->p_call_ctx->common_bitmask_2 & 
                                   ICF_CC_491_RESP_PROCESS_ONGOING))
        {
              ICF_CC_SET_COMMON_BITMASK(p_cc_pdb,
                                         ICF_CALL_MODIFY_RESP_FAIL)
               p_cc_pdb->p_call_ctx->app_failure_cause = 
                                         ICF_CAUSE_CALL_MODIFY_REQ_PENDING;

              if(ICF_FAILURE == icf_cc_stop_current_timer (p_cc_pdb))
              {
                 if(ICF_NW_INC_MEDIA_CHG_SDP_PRESENT ==
                     (ICF_NW_INC_MEDIA_CHG_SDP_PRESENT & 
                                    p_offer_chg_req->bitmask))
                  {
                      icf_cc_delete_inc_media_sdp_info_list(
                               p_cc_pdb,&(p_offer_chg_req->p_sdp_list));
                  }

                  ret_val = ICF_FAILURE;
              }
              else if(ICF_FAILURE == icf_cc_send_pa_call_modify_resp (p_cc_pdb))
              {
                 if(ICF_NW_INC_MEDIA_CHG_SDP_PRESENT ==
                    (ICF_NW_INC_MEDIA_CHG_SDP_PRESENT & 
                             p_offer_chg_req->bitmask))
                 {
                      icf_cc_delete_inc_media_sdp_info_list(
                                p_cc_pdb,&(p_offer_chg_req->p_sdp_list));
                 }

                  ret_val = ICF_FAILURE;
              }
              else
              { 
                   p_cc_pdb->p_internal_msg->p_msg_data = p_msg_data;

                   ret_val = icf_cc_06_ssa_offer_change(p_cc_pdb);
              }
              ICF_CC_RESET_COMMON_BITMASK_2(p_cc_pdb,
                      ICF_CC_491_RESP_PROCESS_ONGOING)
         }
         else
         {

         /************ Changes for Retry-After End (Rel7.0) ***********/
            if(ICF_NW_INC_MEDIA_CHG_SDP_PRESENT ==
                    (ICF_NW_INC_MEDIA_CHG_SDP_PRESENT & p_offer_chg_req->bitmask))
             {
                   icf_cc_delete_inc_media_sdp_info_list(
                               p_cc_pdb,&(p_offer_chg_req->p_sdp_list));
             }

	         /*Would direct SSA to send 491 on NW*/
                 p_cc_pdb->p_call_ctx->offer_failure_reason = ICF_REQUEST_PENDING;
    
                 /* Send failure response for update to remote */
                 p_cc_pdb->p_call_ctx->offer_failure_reason = 
                 ICF_CALL_MODIFY_REASON_REJECT_MEDIA_CHANGE;

                 ret_val = icf_cc_send_ssa_offer_chg_err(p_cc_pdb);          
    	         ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function entry trace */
         }

         return ret_val;
}


/*******************************************************************************
 *
 * FUNCTION:        icf_cc_14_pa_call_term_req
 *
 * DESCRIPTION:     This function is invoked when the CC is in awaiting the response 
 *                  for the REINVITE request and it receives the CALL TERM REQ from PA
 *                  Current State: ICF_CC_14_AWT_CALL_MODIFY_RESP
 *                  Current Event: ICF_CC_PA_CALL_TERMINATE_REQ
 *
 ******************************************************************************/
icf_return_t icf_cc_14_pa_call_term_req(
        INOUT icf_cc_pdb_st *p_cc_pdb)
{
	icf_return_t     ret_val = ICF_SUCCESS;
	icf_config_data_st  *p_config_data = ICF_NULL;

	ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /* function entry trace */

    
   /*
	* Set release type in call context as PA_INITIAITED
	*/
	ICF_CC_TRACE(p_cc_pdb, ICF_TRACE_REL_TYPE_PA_INIT)
	ICF_CC_SET_RELEASE_TYPE(p_cc_pdb,ICF_REL_TYPE_PA_INITIATED)

	/* check the call direction  */
	if(ICF_NULL != ICF_CC_CHECK_COMMON_BITMASK(p_cc_pdb, ICF_CALL_MODIFY_DIR))
	{
		/* if an incoming call modify request handle here */
	    if(ICF_CC_SSA_CALL_CANCEL_REQ != p_cc_pdb->current_event)			
        {
		  p_cc_pdb->p_call_ctx->offer_failure_reason = ICF_CAUSE_INTERNAL_FAILURE;
		  icf_cc_send_ssa_offer_chg_err(p_cc_pdb);
        }
        ICF_CC_RESET_SIG_ONGOING(p_cc_pdb)

		if(ICF_FAILURE == icf_cc_start_release_timer(p_cc_pdb))
		{
			ret_val = ICF_FAILURE;
		} /* if(ICF_FAILURE == icf_st... */
		else
		{
			/*
			* Send a DMS request to MMI
			*/
			p_cc_pdb->forceful_delete = ICF_FALSE;

			if(ICF_FAILURE == icf_cc_send_dms_req(p_cc_pdb))
			{
				ret_val = ICF_FAILURE;
			} /* if(ICF_FAILURE == icf_cc... */
			else
            {
                 /*
                  * Update the states in the call context
                  */
                 ICF_CC_SET_CALL_STATE(p_cc_pdb,ICF_CC_07_AWT_DMS_RESP)
                 /*
                  * Send BYE to network if signaling ongoing is FALSE
                  */
                 ICF_CC_TRACE(p_cc_pdb, ICF_TRACE_SIG_ONGOING)
                 if((ICF_INT_MODULE_INVALID == 
                         ICF_CC_GET_SIG_ONGOING(p_cc_pdb)) ||
                         (ICF_INT_MODULE_CC ==
                         ICF_CC_GET_SIG_ONGOING(p_cc_pdb)))
                 {
                     /*
                      * Send call clear request to the SSA
                      */
                     if(ICF_FAILURE == icf_cc_send_ssa_call_term_req(p_cc_pdb))
                     {
                         ret_val = ICF_FAILURE;
                     } /* if(ICF_FAILURE == icf_cc... */
                     else
                     {
                         /*
                          * Update the next state in the call context
                          */
                         ICF_CC_SET_NEXT_STATE(p_cc_pdb,
                                 ICF_CC_08_AWT_CALL_TERMINATE_RESP)
                     } /* else */
                 } /* if(ICF_NULL == ICF_CC_GET_SIG_ON... */
                 else
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
                    /*
                     * TO DO indicate to CM
                     */
                 } /* else */
             } /* else */
		} /* else */
	}
	else
	{

		/* if an outgoing call modify request handle here */

		/* Get pointer to config data from DBM */
		ret_val = icf_dbm_get_module_glb_data(
				p_cc_pdb->p_glb_pdb,
				(icf_int_module_id_t)ICF_INT_MODULE_CFG,
				(icf_void_t *)&p_config_data, p_cc_pdb->p_ecode);

		if(ICF_SUCCESS == ret_val)
        {
            if((ICF_CFG_CALL_MODIFY_METHOD_INVITE ==
			        p_config_data->call_modify_req_method) &&
			    (ICF_CALL_MODIFY_PROV_RESP_RECVD & 
                    p_cc_pdb->p_call_ctx->prov_resp_status))
		    {
                if(ICF_NULL !=
                   ICF_CC_CHECK_COMMON_BITMASK_2(p_cc_pdb,ICF_CALL_MOD_CANCEL_REQ))
                {
                   /*Dont send cancel again as it is already sent and reset the 
                     bitmask*/
                   /*Case when CANCEL is sent and no response is received*/
                   /*Send call modify_cancel_resp to application*/
                   ICF_CC_SET_COMMON_BITMASK(p_cc_pdb, ICF_CALL_MOD_CANCEL_RESP_FAIL)

                   if (ICF_CC_PA_CALL_TERMINATE_REQ == p_cc_pdb->current_event)
                   {
                       p_cc_pdb->p_call_ctx->app_failure_cause =
                                      ICF_CAUSE_USER_INITIATED_CALL_TERM_REQ;
                   }
                   else
                   {
                       p_cc_pdb->p_call_ctx->app_failure_cause =
                                               ICF_CAUSE_NETWORK_ERROR;
                   }

                   if(ICF_FAILURE == icf_cc_send_pa_call_modify_cancel_resp(p_cc_pdb))
                   {
                      ret_val = ICF_FAILURE;
                   }

                   /*Send call modify_resp to application*/
                   ICF_CC_SET_COMMON_BITMASK(p_cc_pdb, ICF_CALL_MODIFY_RESP_FAIL)

                   if (ICF_CC_PA_CALL_TERMINATE_REQ == p_cc_pdb->current_event)
                   {
                       p_cc_pdb->p_call_ctx->app_failure_cause =
                                      ICF_CAUSE_USER_INITIATED_CALL_TERM_REQ;
                   }
                   else
                   {
                       p_cc_pdb->p_call_ctx->app_failure_cause =
                                               ICF_CAUSE_NETWORK_ERROR;
                   }

                   if(ICF_FAILURE == icf_cc_send_pa_call_modify_resp(p_cc_pdb))
                   {
                      ret_val = ICF_FAILURE;
                   }

                }
                else if(ICF_NULL !=
                   ICF_CC_CHECK_COMMON_BITMASK_2(p_cc_pdb,ICF_CALL_MOD_CANCEL_REINV_REQ))
                {
                   /*Case when 2nd ReInvite was sent and no response is received from 
                     network and PA sends term call request*/

                   /*Send call modify_cancel_resp to application*/
                   ICF_CC_SET_COMMON_BITMASK(p_cc_pdb, ICF_CALL_MOD_CANCEL_RESP_FAIL)

                   p_cc_pdb->p_call_ctx->app_failure_cause =
                                               ICF_CAUSE_USER_INITIATED_CALL_TERM_REQ;
                   if(ICF_FAILURE == icf_cc_send_pa_call_modify_cancel_resp(p_cc_pdb))
                   {
                      ret_val = ICF_FAILURE;
                   }

                   /*Send call modify_resp to application*/
                   ICF_CC_SET_COMMON_BITMASK(p_cc_pdb, ICF_CALL_MODIFY_RESP_FAIL)

                   p_cc_pdb->p_call_ctx->app_failure_cause =
                                               ICF_CAUSE_USER_INITIATED_CALL_TERM_REQ;
                   if(ICF_FAILURE == icf_cc_send_pa_call_modify_resp(p_cc_pdb))
                   {
                      ret_val = ICF_FAILURE;
                   }
			       icf_cc_send_ssa_cancel_req(p_cc_pdb);
                }
                else
                {
			       icf_cc_send_ssa_cancel_req(p_cc_pdb);
                }
                ICF_CC_RESET_SIG_ONGOING(p_cc_pdb)
		    }

            if(ICF_METHOD_STOP_FAX_REQ ==
                       (ICF_METHOD_STOP_FAX_REQ &
                       p_cc_pdb->p_call_ctx->call_modify_method_bitmask))
           	{
            	p_cc_pdb->p_call_ctx->app_failure_cause = ICF_CAUSE_USER_INITIATED_CALL_TERM_REQ;       
                ICF_CC_SET_COMMON_BITMASK(p_cc_pdb, ICF_FAX_RESP_FAIL)
                icf_cc_send_pa_stop_fax_resp(p_cc_pdb);
            }
            else if (ICF_METHOD_START_FAX_REQ== (ICF_METHOD_START_FAX_REQ &
						p_cc_pdb->p_call_ctx->call_modify_method_bitmask))
            {
             	p_cc_pdb->p_call_ctx->app_failure_cause = ICF_CAUSE_USER_INITIATED_CALL_TERM_REQ;       
                ICF_CC_SET_COMMON_BITMASK(p_cc_pdb, ICF_FAX_RESP_FAIL)    
                icf_cc_send_pa_fax_resp(p_cc_pdb); 
            }  
            
          
		    /* if an outgoing call modify request handle here */
		    ret_val = icf_cc_clear_call_at_call_modify(p_cc_pdb);
	
        }
		
	}

	ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function entry trace */

	return ret_val;
}

/*******************************************************************************
 *
 * FUNCTION:        icf_cc_14_generic_err
 *
 * DESCRIPTION:     This function is invoked when the CC is in awaiting the response 
 *                  for the REINVITE request and it receives  error
 *                  Current State: ICF_CC_14_AWT_CALL_MODIFY_RESP
 *                  Current Event: ICF_CC_GENERIC_ERROR
 *
 ******************************************************************************/
icf_return_t icf_cc_14_generic_err(
        INOUT icf_cc_pdb_st *p_cc_pdb)
{
	icf_return_t     ret_val = ICF_SUCCESS;

	ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /* function entry trace */
	/*Fix for SPR 14165*/
	if(ICF_INT_MODULE_INVALID != 
		ICF_CC_GET_SIG_ONGOING(p_cc_pdb))
	{
		ICF_CC_RESET_SIG_ONGOING(p_cc_pdb)

		if(ICF_METHOD_START_FAX_REQ ==
			(ICF_METHOD_START_FAX_REQ & 
				p_cc_pdb->p_call_ctx->call_modify_method_bitmask))
		{
			p_cc_pdb->p_call_ctx->app_failure_cause = ICF_CAUSE_NETWORK_ERROR;			
			ICF_CC_SET_COMMON_BITMASK(p_cc_pdb, ICF_FAX_RESP_FAIL)
			icf_cc_send_pa_fax_resp(p_cc_pdb);
		}
		
		if(ICF_METHOD_STOP_FAX_REQ ==
			(ICF_METHOD_STOP_FAX_REQ & 
				p_cc_pdb->p_call_ctx->call_modify_method_bitmask))
		{
			p_cc_pdb->p_call_ctx->app_failure_cause = ICF_CAUSE_NETWORK_ERROR;			
			ICF_CC_SET_COMMON_BITMASK(p_cc_pdb, ICF_FAX_RESP_FAIL)
			icf_cc_send_pa_stop_fax_resp(p_cc_pdb);
		}
       	else
       	{
       	   /*SPR 13900*/
       	   ICF_CC_RESET_SIG_ONGOING(p_cc_pdb)

           if(ICF_NULL !=
                 ICF_CC_CHECK_COMMON_BITMASK_2(p_cc_pdb,ICF_CALL_MOD_CANCEL_REQ))
           {
              /*Do nothing*/
           }
           else
           {
              /*Send call modify response only in case of network error*/              
              if(p_cc_pdb->p_call_ctx->offer_failure_reason !=
                                   ICF_CALL_MODIFY_REASON_REJECT_MEDIA_CHANGE) 
              {
       	         p_cc_pdb->p_call_ctx->app_failure_cause = 
		         ICF_CAUSE_NETWORK_ERROR;
       	         ICF_CC_SET_COMMON_BITMASK(p_cc_pdb, ICF_CALL_MODIFY_RESP_FAIL)
                 icf_cc_send_pa_call_modify_resp(p_cc_pdb);
              }
           }
        }
	}

    ret_val = icf_cc_14_pa_call_term_req(p_cc_pdb);

    /*
	 * Set release type in call context as INTERNAL_INITIAITED
	 */
    ICF_CC_TRACE(p_cc_pdb, ICF_TRACE_REL_TYPE_INT_ERR)
    ICF_CC_RESET_RELEASE_TYPE(p_cc_pdb,ICF_REL_TYPE_PA_INITIATED)
    ICF_CC_SET_RELEASE_TYPE(p_cc_pdb,ICF_REL_TYPE_INTERNAL_INITIATED)

	ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function entry trace */

	return ret_val;
}

/******************************************************************************
 *
 * FUNCTION:        icf_cc_14_timer_expiry
 *
 * DESCRIPTION:     This function is invoked when the CC is awaiting
 *                  reinvite resp and the it receives timer expire event.
 *                  Current State: ICF_CC_14_AWT_CALL_MODIFY_RESP
 *                  Current Event: ICF_CC_TIMER_EXPIRY
 *
 *****************************************************************************/
icf_return_t icf_cc_14_timer_expiry(
        INOUT icf_cc_pdb_st *p_cc_pdb)
{
	icf_return_t     ret_val = ICF_SUCCESS;
	icf_config_data_st  *p_config_data = ICF_NULL;

	ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /* function entry trace */

        /********* Changes for Retry-After Start (Rel7.0) *********/ 

        /* This code checks for the timer type. If it returns true 
        * then CC will call  ssa to send the Re-INVITE request again.* /
        if(ICF_NULL != p_cc_pdb->p_call_ctx->p_retry_after_timer_data)
        { */
           if(ICF_TIMER_TYPE_RETRY_AFTER == (*((icf_timer_data_st **)
                    (p_cc_pdb->p_glb_pdb->p_recv_msg->payload)))->timer_type)
           {
              ret_val = icf_cc_ssa_send_update_msg(p_cc_pdb);
              ICF_CC_RESET_COMMON_BITMASK_2(p_cc_pdb,
                      ICF_CC_491_RESP_PROCESS_ONGOING)
              ICF_CC_SET_SIG_ONGOING(p_cc_pdb)
              
              return ret_val;
           }
        /* }*/
        /********** Changes for Retry-After End (Rel7.0) **********/

        /* Alert Timer expiry took place and no answer of UPDATE was
         * received(decode failed of 200 OK)*/
        if(ICF_TRUE == p_cc_pdb->p_call_ctx->update_reqd)
            {  
               p_cc_pdb->p_call_ctx->app_failure_cause = ICF_CAUSE_NETWORK_ERROR;
               ICF_CC_SET_COMMON_BITMASK(p_cc_pdb, ICF_CALL_MODIFY_RESP_FAIL)
               if(ICF_FAILURE == icf_cc_send_pa_call_modify_resp(p_cc_pdb))
                {
                    ret_val = ICF_FAILURE;
                }
               else if(ICF_FAILURE == icf_cc_clear_call_at_call_modify(p_cc_pdb))
                {
                    ret_val = ICF_FAILURE;
                } 
               p_cc_pdb->p_call_ctx->update_reqd = ICF_FALSE;                
            }

		/* check the call direction  */
        else if(ICF_NULL != ICF_CC_CHECK_COMMON_BITMASK(p_cc_pdb, ICF_CALL_MODIFY_DIR))
	{
		/* if an incoming call modify request handle here */

		p_cc_pdb->p_call_ctx->offer_failure_reason = ICF_CALL_MODIFY_REASON_USER_BUSY;
        /* revert the changes and then send response */
            
        icf_cc_derive_mms_type_for_revert_remote_change(p_cc_pdb);

		if(ICF_FALSE != p_cc_pdb->p_call_ctx->media_updated)
		{
			if(ICF_FAILURE == icf_cc_send_mms_req(p_cc_pdb))
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
					ICF_CC_SET_CALL_STATE(p_cc_pdb,ICF_CC_04_AWT_MMS_RESP)
					ICF_CC_SET_NEXT_STATE(p_cc_pdb,ICF_CC_06_CONNECTED)
                    ICF_CC_SET_ONGOING_EVENT(p_cc_pdb,
							ICF_CC_ONGOING_EVENT_REMOTE_MEDIA_REVERT)

					/* Start the media timer  */
					ret_val = icf_cc_start_media_timer(p_cc_pdb);
				}
			}/* MMS was handled succesfully */
		}
        else if(ICF_FAILURE == icf_cc_send_ssa_offer_chg_err(p_cc_pdb))
		{
			ret_val = ICF_FAILURE;
		}
		else
		{
			ICF_CC_SET_CALL_STATE(p_cc_pdb,ICF_CC_06_CONNECTED)
		}
	}
	else
	{
		/* if an outgoing call modify request handle here */

		/* Get pointer to config data from DBM */
		ret_val = icf_dbm_get_module_glb_data(
				p_cc_pdb->p_glb_pdb,
				(icf_int_module_id_t)ICF_INT_MODULE_CFG,
				(icf_void_t *)&p_config_data, p_cc_pdb->p_ecode);

		if(ICF_SUCCESS == ret_val)
        {
            if((ICF_CFG_CALL_MODIFY_METHOD_INVITE ==
			        p_config_data->call_modify_req_method) &&
			    (ICF_CALL_MODIFY_PROV_RESP_RECVD & 
                    p_cc_pdb->p_call_ctx->prov_resp_status))
	    {
                ICF_CC_RESET_SIG_ONGOING(p_cc_pdb)

                if(ICF_NULL !=
                   ICF_CC_CHECK_COMMON_BITMASK_2(p_cc_pdb,ICF_CALL_MOD_CANCEL_REQ))
                {
                   /*Dont send cancel request again*/ 

                   /*Send call modify_cancel_resp to application*/
                   ICF_CC_SET_COMMON_BITMASK(p_cc_pdb, ICF_CALL_MOD_CANCEL_RESP_FAIL)

                   p_cc_pdb->p_call_ctx->app_failure_cause =
                                                ICF_CAUSE_NETWORK_ERROR;
                   if(ICF_FAILURE == icf_cc_send_pa_call_modify_cancel_resp(p_cc_pdb))
                   {
                       ret_val = ICF_FAILURE;
                   }

                   /*Send call modify_cancel_resp to application*/
                   ICF_CC_SET_COMMON_BITMASK(p_cc_pdb, ICF_CALL_MODIFY_RESP_FAIL)

                   p_cc_pdb->p_call_ctx->app_failure_cause =
                                                ICF_CAUSE_NETWORK_ERROR;
                   if(ICF_FAILURE == icf_cc_send_pa_call_modify_resp(p_cc_pdb))
                   {
                       ret_val = ICF_FAILURE;
                   }
                }
                else 
                {
                     if(ICF_TIMER_ID_INVALID == 
                                     p_cc_pdb->p_call_ctx->retry_after_timer_id)
                     {
			             icf_cc_send_ssa_cancel_req(p_cc_pdb);
                     }
                     
                     ICF_CC_SET_COMMON_BITMASK(p_cc_pdb,ICF_CALL_MODIFY_RESP_FAIL)
                     p_cc_pdb->p_call_ctx->app_failure_cause =
                                                ICF_CAUSE_NETWORK_ERROR;
                     icf_cc_send_pa_call_modify_resp(p_cc_pdb);
                }
	    }
            else
           {
                     ICF_CC_SET_COMMON_BITMASK(p_cc_pdb,ICF_CALL_MODIFY_RESP_FAIL)
                     p_cc_pdb->p_call_ctx->app_failure_cause =
                                                ICF_CAUSE_NETWORK_ERROR;
                     icf_cc_send_pa_call_modify_resp(p_cc_pdb);
           }

            /* if an outgoing call modify request handle here */
	    p_cc_pdb->p_call_ctx->release_cause = ICF_CALL_CLEAR_REMOTE_UNREACHABLE;
	    ret_val = icf_cc_clear_call_at_call_modify(p_cc_pdb);
        
        }
		
	}

	ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function entry trace */

	return ret_val;
}



/*******************************************************************************
 *
 * FUNCTION:        icf_cc_14_ssa_call_term_req
 *
 * DESCRIPTION:     This function is invoked when the CC is in awaiting the response 
 *                  for the REINVITE request and it receives the CALL TERM REQ from SSA
 *                  Current State: ICF_CC_14_AWT_CALL_MODIFY_RESP
 *                  Current Event: ICF_CC_SSA_CALL_TERMINATE_REQ
 *
 ******************************************************************************/
icf_return_t icf_cc_14_ssa_call_term_req(
        INOUT icf_cc_pdb_st *p_cc_pdb)
{
	icf_return_t     ret_val = ICF_SUCCESS;
	ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /* function entry trace */

	ICF_CC_RESET_SIG_ONGOING(p_cc_pdb)
#if 0
	/* check the call direction  */
	if(ICF_NULL != ICF_CC_CHECK_COMMON_BITMASK(p_cc_pdb, ICF_CALL_MODIFY_DIR))
	{

		/* if an incoming call modify request handle here */

		/* do nothing */
		p_cc_pdb->p_call_ctx->offer_failure_reason = ICF_CALL_MODIFY_REASON_REQUEST_TERMINATED;

		if(ICF_FAILURE == icf_cc_send_ssa_offer_chg_err(p_cc_pdb))
		{
			ret_val = ICF_FAILURE;
		}
	}
	else
	{
		/* Get pointer to config data from DBM */
		ret_val = icf_dbm_get_module_glb_data(
				p_cc_pdb->p_glb_pdb,
				(icf_int_module_id_t)ICF_INT_MODULE_CFG,
				(icf_void_t *)&p_config_data, p_cc_pdb->p_ecode);

		ICF_ASSERT(p_cc_pdb->p_glb_pdb, (ICF_NULL != p_config_data));

		/* if an outgoing call modify request handle here */

		if(ICF_SUCCESS == ret_val && 
			ICF_CFG_CALL_MODIFY_METHOD_INVITE == 
			p_config_data->call_modify_req_method &&
			(ICF_CALL_MODIFY_PROV_RESP_RECVD & p_cc_pdb->p_call_ctx->prov_resp_status))
		{
			if(ICF_FAILURE == icf_cc_send_ssa_cancel_req(p_cc_pdb))
			{
				ret_val = ICF_FAILURE;
			}
		}

		p_cc_pdb->p_call_ctx->app_failure_cause = ICF_CALL_CLEAR_REASON_REMOTE_RELEASED;
		ICF_CC_SET_COMMON_BITMASK(p_cc_pdb, ICF_CALL_CLEAR_REASON_REMOTE_RELEASED)

        ICF_CC_SET_COMMON_BITMASK(p_cc_pdb, ICF_CALL_MODIFY_RESP_FAIL)

		if (ICF_FAILURE == icf_cc_send_pa_call_modify_resp(p_cc_pdb))
		{
			ret_val = ICF_FAILURE;
		}

	}
#endif

    if(ICF_NULL == ICF_CC_CHECK_COMMON_BITMASK(p_cc_pdb, ICF_CALL_MODIFY_DIR))
	{
	    if(ICF_SUCCESS == icf_cc_check_fax_resp(p_cc_pdb))
            {
      		ICF_CC_RESET_SIG_ONGOING(p_cc_pdb)
   	        ICF_CC_SET_COMMON_BITMASK(p_cc_pdb, ICF_FAX_RESP_FAIL)
        	 	icf_cc_send_pa_fax_resp(p_cc_pdb);
    	    }
            else
    	    {
      		p_cc_pdb->p_call_ctx->app_failure_cause = 
        						ICF_CALL_CLEAR_REASON_REMOTE_RELEASED;
      		ICF_CC_SET_COMMON_BITMASK(p_cc_pdb, 
          	ICF_CALL_CLEAR_REASON_REMOTE_RELEASED)
            ICF_CC_SET_COMMON_BITMASK(p_cc_pdb, ICF_CALL_MODIFY_RESP_FAIL)

       		ret_val = icf_cc_send_pa_call_modify_resp(p_cc_pdb);
    	     }
  	}

	if(ICF_SUCCESS == ret_val)
	{
	    if(ICF_FAILURE == icf_cc_start_release_timer(p_cc_pdb))
	    {
		    ret_val = ICF_FAILURE;
	    }
	    else
	    {
		    /*
		     * Set release type in call context as NW_INITIAITED
		     */
		    ICF_CC_TRACE(p_cc_pdb, ICF_TRACE_REL_TYPE_NW_INIT)
		    ICF_CC_SET_RELEASE_TYPE(p_cc_pdb,ICF_REL_TYPE_NW_INITIATED)
		    ICF_CC_SET_RELEASE_CAUSE(p_cc_pdb,
				    ICF_CALL_CLEAR_REASON_REMOTE_RELEASED)
		    /*
		     * Send Call Termination Resp to SSA and call NW clear handler
		     */
		    if(ICF_FAILURE == icf_cc_send_ssa_call_term_rsp(
					    p_cc_pdb))
		    {
			    ret_val = ICF_FAILURE;
		    } /* if(ICF_FAILURE == icf_cc... */
		    /*
		     * Send a DMS request to MMI
		     */
		    else if(ICF_FAILURE == icf_cc_send_dms_req(p_cc_pdb))
		    {
			    ret_val = ICF_FAILURE;
		    } /* else if(ICF_FAILURE == ipps... */
		    else
		    {
			    /*
			     * Update the states in the call context
			     */
			    ICF_CC_SET_CALL_STATE(p_cc_pdb,ICF_CC_07_AWT_DMS_RESP)

			    /* Set ICF_INT_MODULE_CC in signalling ongoing flag */
			    ICF_CC_SET_SIG_ONGOING(p_cc_pdb)

                ret_val = icf_cc_nw_clear_handler(p_cc_pdb);
		    } /* else */
	    }
	}

	ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function entry trace */

	return ret_val;
}

/*cancel_reinv*/
/*******************************************************************************
 *
 * FUNCTION:        icf_cc_14_ssa_call_cancel_req
 *
 * DESCRIPTION:     This function is invoked when the CC is in awaiting the response 
 *                  for the REINVITE request and it receives SIP CANCEL from SSA
 *                  Current State: ICF_CC_14_AWT_CALL_MODIFY_RESP
 *                  Current Event: ICF_CC_SSA_CALL_CANCEL_REQ
 *
 ******************************************************************************/
icf_return_t icf_cc_14_ssa_call_cancel_req(
        INOUT icf_cc_pdb_st *p_cc_pdb)
{
	icf_return_t     ret_val = ICF_SUCCESS;

	ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /* function entry trace */

    ICF_CC_RESET_SIG_ONGOING(p_cc_pdb)

	/* check the call direction , it should be an incoming call modify request here */
	if(ICF_NULL != ICF_CC_CHECK_COMMON_BITMASK(p_cc_pdb, ICF_CALL_MODIFY_DIR))
	{
            icf_cc_stop_current_timer(p_cc_pdb);
               
		    if(ICF_FAILURE == icf_cc_send_ssa_cancel_resp(p_cc_pdb))
		    {
			ret_val = ICF_FAILURE;
		    }
		    else if(ICF_FAILURE == icf_cc_send_pa_call_modify_cancel_ind(p_cc_pdb))
		    {
			ret_val = ICF_FAILURE;
		    }
		    else if( ICF_FALSE == p_cc_pdb->p_call_ctx->media_updated)
		    {
			/*
			 * Update the states in the call context
			 */
			ICF_CC_SET_CALL_STATE(p_cc_pdb,ICF_CC_06_CONNECTED)
		    }
		    else
		    {
               /* revert the changes and then send cancel indication to PA */
               if(ICF_FAILURE == icf_cc_derive_mms_type_for_revert_remote_change(p_cc_pdb))
               {
                  ret_val = icf_cc_14_generic_err(p_cc_pdb);
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
				    ICF_CC_SET_CALL_STATE(p_cc_pdb,ICF_CC_04_AWT_MMS_RESP)
				    ICF_CC_SET_NEXT_STATE(p_cc_pdb,ICF_CC_06_CONNECTED)
				    ICF_CC_SET_ONGOING_EVENT(p_cc_pdb,
				               		ICF_CC_SSA_CALL_MODIFY_CANCEL_REQ)
				
				    /* Start the media timer  */
				    ret_val = icf_cc_start_media_timer(p_cc_pdb);
			     }
			  }/* MMS was handled succesfully */
	        } /* end of else */
	}

	/* reset the prov resp for call modify flag */
	p_cc_pdb->p_call_ctx->prov_resp_status &= (~(ICF_CALL_MODIFY_PROV_RESP_RECVD));

	ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function entry trace */

	return ret_val;
}

/******************************************************************************
 *
 * FUNCTION:        icf_cc_14_forceful_clear
 *
 * DESCRIPTION:     This function is invoked when the CC is in awaiting the response 
 *                  for the REINVITE request CM want to clear call due to system error
 *                  Current State: ICF_CC_14_AWT_CALL_MODIFY_RESP
 *                  Current Event: ICF_CC_FORCEFUL_CLEAR
 *
 *****************************************************************************/
icf_return_t icf_cc_14_forceful_clear(
        INOUT icf_cc_pdb_st *p_cc_pdb)
{
	icf_return_t     ret_val = ICF_SUCCESS;

	ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /* function entry trace */

	ICF_CC_RESET_SIG_ONGOING(p_cc_pdb)


	ICF_CC_SET_RELEASE_CAUSE(p_cc_pdb,ICF_REL_CAUSE_SERV_INT_ERR)

    icf_cc_stop_current_timer(p_cc_pdb);

    /*
     * Send call clear request to the SSA
     */
    icf_cc_send_ssa_call_term_req(p_cc_pdb);

	icf_cc_00_clear_call (p_cc_pdb);

	ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function entry trace */

	return ret_val;
}


/******************************************************************************
 *
 * FUNCTION:        icf_cc_14_mm_error_ind
 *
 * DESCRIPTION:     This function is invoked when the CC is in awaiting the response 
 *                  for the REINVITE request MM err indication is received.
 *                  Current State: ICF_CC_14_AWT_CALL_MODIFY_RESP
 *                  Current Event: ICF_CC_MM_ERR_IND
 *
 *****************************************************************************/
icf_return_t icf_cc_14_mm_error_ind(
        INOUT icf_cc_pdb_st *p_cc_pdb)
{
	icf_return_t     ret_val = ICF_SUCCESS;
	icf_config_data_st  *p_config_data = ICF_NULL;
    icf_mm_err_ind_st *p_mm_err_ind = ICF_NULL;
    icf_mmi_reason_t  media_error;

    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /* function entry trace */

	

    p_mm_err_ind = (icf_mm_err_ind_st*)
        (p_cc_pdb->p_internal_msg->p_msg_data);


    media_error = p_mm_err_ind->reason;

	/* Get pointer to config data from DBM */
	ret_val = icf_dbm_get_module_glb_data(
			p_cc_pdb->p_glb_pdb,
			(icf_int_module_id_t)ICF_INT_MODULE_CFG,
			(icf_void_t *)&p_config_data, p_cc_pdb->p_ecode);

	ICF_ASSERT(p_cc_pdb->p_glb_pdb, (ICF_NULL != p_config_data));

	if(ICF_SUCCESS == ret_val && (ICF_CFG_CALL_MODIFY_METHOD_INVITE ==
			p_config_data->call_modify_req_method) &&
			(ICF_CALL_MODIFY_PROV_RESP_RECVD & p_cc_pdb->p_call_ctx->prov_resp_status))
	{
        ICF_CC_RESET_SIG_ONGOING(p_cc_pdb)
		if(ICF_FAILURE == icf_cc_send_ssa_cancel_req(p_cc_pdb))
		{
			ret_val = ICF_FAILURE;
		}
	}

    /*
     * Start the release timer
     */
    if(ICF_FAILURE == icf_cc_start_release_timer(p_cc_pdb))
    {
        ret_val = ICF_FAILURE;
    } /* if(failure == icf_st... */
    else
    {
        /*
         * Set release type in call context as MM_INITIAITED
         */

        ICF_CC_SET_RELEASE_TYPE(p_cc_pdb,ICF_REL_TYPE_MM_INITIATED)
    
        /*
         * Set release cause in call context as MM_ERROR_IND if not set by MMI
         */
         ICF_CC_SET_RELEASE_CAUSE(p_cc_pdb,
                    ICF_CALL_CLEAR_MEDIA_MISMATCH)
        /*
         *If mmi has send mm_err_ind with reason as ICF_ECODE_DMS_IND_RECVD
         */
         if(ICF_ECODE_DMS_IND_RECVD != media_error)
         {
             /*
              * Send a DMS request to MMI
              */
             if(ICF_FAILURE == icf_cc_send_dms_req(p_cc_pdb))
             {
                 ret_val = ICF_FAILURE;
             } /* if(failure == icf_cc... */
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
             /* Send Call Terminate Request to PA */
            ret_val = icf_cc_send_pa_call_term_ind(p_cc_pdb);

            ICF_CC_SET_CALL_CLR_STATUS(p_cc_pdb,
                ICF_CALL_CLEAR_STATUS_PA_CLEARED)
            /* Set MM cleared bit in call_clear_status */
            ICF_CC_SET_CALL_CLR_STATUS(p_cc_pdb, 
                ICF_CALL_CLEAR_STATUS_MM_CLEARED)

         }

        if(ICF_SUCCESS == ret_val)
        {
            /*
             * Send BYE to network if signaling ongoing is FALSE
             */
            if((ICF_INT_MODULE_INVALID == 
                    ICF_CC_GET_SIG_ONGOING(p_cc_pdb)) ||
                    (ICF_INT_MODULE_CC ==
                    ICF_CC_GET_SIG_ONGOING(p_cc_pdb)))
            {
                ICF_CC_SET_SIG_ONGOING(p_cc_pdb)
                /*
                 * Send call clear request to the SSA
                 */
                 if(ICF_FAILURE == 
                         icf_cc_send_ssa_call_term_req(p_cc_pdb))
                 {
                     ret_val = ICF_FAILURE;
                 } /* if(failure == icf_cc... */
                 else
                 {
                     /*
                      * If mmi has send mm_err_ind with reason as   
                      * ICF_ECODE_DMS_IND_RECVD
                      */
                     if(ICF_ECODE_DMS_IND_RECVD != media_error)
                     {

                         /*
                          * Update the next state in the call context
                          */
                         ICF_CC_SET_NEXT_STATE(p_cc_pdb,
                                 ICF_CC_08_AWT_CALL_TERMINATE_RESP)
                     }
                     else
                     {
                         ICF_CC_SET_CALL_STATE(p_cc_pdb,
                                 ICF_CC_08_AWT_CALL_TERMINATE_RESP)
                     }
                 }
            } /* if(ICF_NULL == ICF_CC_GET_SIG_ON... */
            else
            {
                /*Return service terminate status to CM*/
                ICF_CC_SET_STATUS(p_cc_pdb,
                        ICF_STATUS_CALL_R_SRV_TERMINATE)
                /*
                 * If mmi has send mm_err_ind with reason as   
                 * ICF_ECODE_DMS_IND_RECVD
                 */
                 if(ICF_ECODE_DMS_IND_RECVD != media_error)
                 {
                     /*
                      * Mark next state as 
                      * ICF_CC_12_WAIT_START_PROC_SIG
                      */
                     ICF_CC_SET_NEXT_STATE(p_cc_pdb,
                             ICF_CC_12_WAIT_START_PROC_SIG)
                 }
                 else
                 {
                     ICF_CC_SET_CALL_STATE(p_cc_pdb,
                             ICF_CC_12_WAIT_START_PROC_SIG)
                 }
            }
        } /* else */
    } /* else */

    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */
    return ret_val;

}


#ifdef ICF_FAX_SUPPORT
/******************************************************************************
 *
 * FUNCTION:        icf_cc_14_ssa_fax_resp
 *
 * DESCRIPTION:     This function is invoked when the CC is in awaiting the  
 *                  response for the FAX request and it receives the response.
 *                  Current State: ICF_CC_14_AWT_CALL_MODIFY_RESP
 *                  Current Event: ICF_CC_SSA_FAX_RESP
 *
 *****************************************************************************/
icf_return_t icf_cc_14_ssa_fax_resp(
        INOUT icf_cc_pdb_st *p_cc_pdb)
{
	icf_return_t     ret_val = ICF_SUCCESS;
	icf_nw_inc_media_change_resp_st *p_call_modify_resp = ICF_NULL;

	ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /* function entry trace */

	ICF_CC_RESET_SIG_ONGOING(p_cc_pdb)
    /* Return status as signalling done to CM */
    ICF_CC_SET_STATUS(p_cc_pdb, ICF_STATUS_SIGNAL_ONGOING_ENDED)

	ret_val = icf_cc_stop_current_timer(p_cc_pdb);

	/* check the call direction , it should be an 
	    outgoing call modify request here */
	if(ICF_NULL == ICF_CC_CHECK_COMMON_BITMASK(p_cc_pdb, ICF_CALL_MODIFY_DIR))
	{
		p_call_modify_resp = (icf_nw_inc_media_change_resp_st *)
				p_cc_pdb->p_internal_msg->p_msg_data;

		if(ICF_TRUE == p_call_modify_resp->status)
		{
			/* Accept */
			
			/*
			1.	New MMS is required for the modifed streams.
			2.  Revert MMS required for rejected streams
			3.	send the answer to application after the MMS.
			*/

           /*START: fix for BDFam_32183*/
            if ( ICF_FAX_PT_CALL != 
                   p_cc_pdb->p_call_ctx->fax_type)
            {
                icf_cc_reset_media_modify_type_for_all_streams(p_cc_pdb);
            }
            /*END: fix for BDFam_32183*/
 
			if(ICF_FAILURE == icf_cc_process_remote_fax_resp(p_cc_pdb))
			{
                ret_val = ICF_FAILURE;

				p_cc_pdb->p_call_ctx->app_failure_cause = ICF_CAUSE_NETWORK_ERROR;			
			    ICF_CC_SET_COMMON_BITMASK(p_cc_pdb, ICF_FAX_RESP_FAIL)
                icf_cc_send_pa_fax_resp(p_cc_pdb);

                if (ICF_CC_STREAM_POS_VALIDATION_FAIL == 
				    (p_cc_pdb->common_bitmask & 
                        ICF_CC_STREAM_POS_VALIDATION_FAIL))
		        {
			        ret_val = icf_cc_14_generic_err(p_cc_pdb);
		        }
			
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
					ICF_CC_SET_COMMON_BITMASK(p_cc_pdb, ICF_FAX_CALL_ESTABLISHED)
					ICF_CC_SET_CALL_STATE(p_cc_pdb,ICF_CC_04_AWT_MMS_RESP)
					ICF_CC_SET_NEXT_STATE(p_cc_pdb,ICF_CC_06_CONNECTED)
					ICF_CC_SET_ONGOING_EVENT(p_cc_pdb,
										p_cc_pdb->current_event)
					/* Start the media timer  */
					ret_val = icf_cc_start_media_timer(p_cc_pdb);
				}	
			}/* MMS was handled succesfully */
		
		}
		else
		{
            p_cc_pdb->p_call_ctx->app_failure_cause = ICF_CAUSE_REJECT_MEDIA_CHANGE_REQ;
            ICF_CC_SET_COMMON_BITMASK(p_cc_pdb, ICF_FAX_RESP_FAIL)

			if(ICF_CALL_CLEAR_REASON_USER_NOT_FOUND == 
							p_call_modify_resp->reason_type )
			{
                p_cc_pdb->p_call_ctx->app_failure_cause = ICF_CAUSE_NETWORK_ERROR;			
                icf_cc_send_pa_fax_resp(p_cc_pdb);

				/* terminate the call for these error reasons */
				p_cc_pdb->p_call_ctx->release_cause = 
							(icf_uint8_t)p_call_modify_resp->reason_type;
				ret_val = icf_cc_clear_call_at_call_modify(p_cc_pdb);
			}
		       /* SPR 17399:
                        * Check if fax type is ICF_T38_FAX_CALL_FAILURE_SWITCH_PT_CALL and 
                        * error cause is 488 or 606 then initiate fax passthrough 
                        * else revert media change and return start_fax error response.
                        */
      	                else
			{
	 
                /*If the Failure response is not 488/606,
                 *then IPTK should not revert to PT call,if its so.
                 */  
			    if(ICF_CALL_CLEAR_MEDIA_MISMATCH !=
                                    p_call_modify_resp->reason_type)
                {
                     if(ICF_T38_FAX_CALL_FAILURE_SWITCH_PT_CALL == 
                                             p_cc_pdb->p_call_ctx->fax_type)
                     {
                         p_cc_pdb->p_call_ctx->fax_type = 
                                                    ICF_T38_FAX_CALL;
                     }
                }	
               /* Reject */
				icf_cc_process_remote_fax_err_resp(p_cc_pdb);
                
				/* revert the changes and then send response to PA */
				if(ICF_FAILURE == icf_cc_send_mms_req(p_cc_pdb))
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
						ICF_CC_SET_CALL_STATE(p_cc_pdb,ICF_CC_04_AWT_MMS_RESP)
						ICF_CC_SET_NEXT_STATE(p_cc_pdb,ICF_CC_06_CONNECTED)
						ICF_CC_SET_ONGOING_EVENT(p_cc_pdb,
                            p_cc_pdb->current_event)
						
						/* Start the media timer  */
						ret_val = icf_cc_start_media_timer(p_cc_pdb);
					}
					
				}/* MMS was handled succesfully */
				
			}
		}
	}
	


	ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */
    return ret_val;

}


/*******************************************************************************
 *
 * FUNCTION:        icf_cc_14_pa_fax_req
 *
 * DESCRIPTION:     This function is invoked when the CC is in awaiting the response 
 *                  for the REINVITE request and it receives FAX request from PA.
 *                  Current State: ICF_CC_14_AWT_CALL_MODIFY_RESP
 *                  Current Event: ICF_CC_PA_FAX_REQ
 *
 ******************************************************************************/
icf_return_t icf_cc_14_pa_fax_req(
        INOUT icf_cc_pdb_st *p_cc_pdb)
{
	icf_return_t     ret_val = ICF_SUCCESS;

	ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /* function entry trace */

	/*Would direct SSA to send 491 on NW*/

    if(ICF_NULL != p_cc_pdb->p_call_ctx->fax_type)
    {
        p_cc_pdb->p_call_ctx->app_failure_cause = ICF_CAUSE_FAX_REQ_PENDING;
    }
    else
    {
        p_cc_pdb->p_call_ctx->app_failure_cause = ICF_CAUSE_CALL_MODIFY_REQ_PENDING;
    }

	ICF_CC_SET_COMMON_BITMASK(p_cc_pdb, ICF_FAX_RESP_FAIL)
    
    /* Send failure response to PA */
    ret_val = icf_cc_send_pa_fax_resp(p_cc_pdb);

	ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function entry trace */

	return ret_val;
}


/*******************************************************************************
 *
 * FUNCTION:        icf_cc_14_ssa_fax_req
 *
 * DESCRIPTION:     This function is invoked when the CC is in awaiting the response 
 *                  for the REINVITE request and it receives FAX request from SSA.
 *                  Current State: ICF_CC_14_AWT_CALL_MODIFY_RESP
 *                  Current Event: ICF_CC_SSA_FAX_REQ
 *
 ******************************************************************************/
icf_return_t icf_cc_14_ssa_fax_req(
        INOUT icf_cc_pdb_st *p_cc_pdb)
{
	icf_return_t     ret_val = ICF_SUCCESS;

    icf_nw_inc_media_change_req_st *p_fax_req =
		(icf_nw_inc_media_change_req_st *)p_cc_pdb->p_internal_msg->p_msg_data;

	ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /* function entry trace */
        
    /************ Changes for Retry-After Start (Rel7.0) ***********/

    /* A code segment is added to handle the 
     * reinvite from remote side while the 491 has been received of 
     * outgoing reinvite and a retry after timer is running 
     * to again send the reinvite */

    if(ICF_NULL != (p_cc_pdb->p_call_ctx->common_bitmask_2 & 
                                ICF_CC_491_RESP_PROCESS_ONGOING))
    {
         icf_uint8_t *p_msg_data = 
                    p_cc_pdb->p_internal_msg->p_msg_data;
             
 
        ICF_CC_SET_COMMON_BITMASK(p_cc_pdb,
                      ICF_CALL_MODIFY_RESP_FAIL)
        p_cc_pdb->p_call_ctx->app_failure_cause = 
                              ICF_CAUSE_CALL_MODIFY_REQ_PENDING;

        if(ICF_FAILURE == icf_cc_stop_current_timer (p_cc_pdb))
        {
                  ret_val = ICF_FAILURE;
        }
        else if(ICF_FAILURE == icf_cc_send_pa_call_modify_resp (p_cc_pdb))
        {
                  ret_val = ICF_FAILURE;
        }
        else
        { 
                   p_cc_pdb->p_internal_msg->p_msg_data = p_msg_data;

                   ret_val = icf_cc_06_ssa_fax_req(p_cc_pdb);
        }

        if(ICF_FAILURE == ret_val)
        {
              if(ICF_NW_INC_MEDIA_CHG_SDP_PRESENT ==
                    (ICF_NW_INC_MEDIA_CHG_SDP_PRESENT &
                             p_fax_req->bitmask))
               {
                      icf_cc_delete_inc_media_sdp_info_list(
                                p_cc_pdb,&(p_fax_req->p_sdp_list));
               }
        }

         ICF_CC_RESET_COMMON_BITMASK_2(p_cc_pdb,
                      ICF_CC_491_RESP_PROCESS_ONGOING)
         return ret_val;
     }


    if(ICF_NULL != p_fax_req->p_sdp_list)
    {
        icf_cc_delete_inc_media_sdp_info_list(p_cc_pdb,&p_fax_req->p_sdp_list);
    }

	/*Would direct SSA to send 491 on NW*/
    p_cc_pdb->p_call_ctx->offer_failure_reason = ICF_REQUEST_PENDING;
    
    /* Send failure response for update to remote */
    ret_val = icf_cc_send_ssa_offer_chg_err(p_cc_pdb);
        
	ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function entry trace */

	return ret_val;
}

/*******************************************************************************
 *
 * FUNCTION:        icf_cc_14_pa_stop_fax_req
 *
 * DESCRIPTION:     This function is  used for handling the buffering of 100 trying
 *                  and reverting of media for audio once stop fax req is send by PA
 *
 ******************************************************************************/
icf_return_t icf_cc_14_pa_stop_fax_req(
         INOUT icf_cc_pdb_st *p_cc_pdb)
{
     icf_return_t     ret_val = ICF_SUCCESS;
     icf_config_data_st  *p_config_data = ICF_NULL;
     
     ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /* function entry trace */

     /* Review comment incorporation, if call modify is going on either from PA or remote
      * and pa_stop_fax_req is being received in this state, then it should
      * be rejected.*/
     if((ICF_METHOD_CALL_MODIFY_REQ == (p_cc_pdb->p_call_ctx->call_modify_method_bitmask &
					ICF_METHOD_CALL_MODIFY_REQ))
         || (ICF_NULL != ICF_CC_CHECK_COMMON_BITMASK(p_cc_pdb,
                                        ICF_CALL_MODIFY_DIR)))
     {
        p_cc_pdb->p_call_ctx->app_failure_cause = ICF_CAUSE_CALL_MODIFY_REQ_PENDING;       
        ICF_CC_SET_COMMON_BITMASK(p_cc_pdb, ICF_FAX_RESP_FAIL)
        ret_val = icf_cc_send_pa_stop_fax_resp(p_cc_pdb);
        return ret_val;
     } 
#if 0
     /* check the call direction  */
	if(ICF_NULL != ICF_CC_CHECK_COMMON_BITMASK(p_cc_pdb,  
                         ICF_CALL_MODIFY_DIR))
      	{
        /* Logic for handling incoming call , yet to be designed*/
      	}
     	else
      	{
#endif
         /* if an outgoing call modify request handle here */
         /* Get pointer to config data from DBM            */
         ret_val = icf_dbm_get_module_glb_data(
                    p_cc_pdb->p_glb_pdb,
                    (icf_int_module_id_t)ICF_INT_MODULE_CFG,
                    (icf_void_t *)&p_config_data, p_cc_pdb->p_ecode);
        
        /* check the bitmask for the 100 trying recvd */
        /* If not then make the stop fax as pending event */
        if(!(ICF_CALL_MODIFY_PROV_RESP_RECVD & 
          p_cc_pdb->p_call_ctx->prov_resp_status))
        {
          ICF_CC_SET_PENDING_EVENT(p_cc_pdb,
                       ICF_PENDING_EVENT_PA_STOP_FAX_REQ)
        }                
         ret_val = icf_cc_make_revert_mms_fax_cancel(p_cc_pdb); 
         if(ICF_TRUE == p_cc_pdb->mms_sent)
          {
            ICF_CC_SET_CALL_STATE(p_cc_pdb,ICF_CC_04_AWT_MMS_RESP)
            ICF_CC_SET_NEXT_STATE(p_cc_pdb,ICF_CC_15_AWT_CALL_MODIFY_ALERT_RESP)
            ICF_CC_SET_ONGOING_EVENT(p_cc_pdb, ICF_CC_PA_STOP_FAX_REQ)
          }
#if 0
      }
#endif
    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function entry trace */
    return ret_val;
}  

/*******************************************************************************
 *
 * FUNCTION:        icf_cc_14_ssa_stop_fax_resp
 *
 * DESCRIPTION:     This function is called when 200 OK for cancel is recvd and
 *                  stop and start fax responses needs to be sent to PA.
 *
 ******************************************************************************/

icf_return_t icf_cc_14_ssa_stop_fax_resp(
             INOUT icf_cc_pdb_st *p_cc_pdb)
{
     icf_return_t     ret_val = ICF_SUCCESS;
	 icf_nw_inc_media_change_resp_st *p_call_modify_resp = ICF_NULL;
     
     ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /* function entry trace */

     p_call_modify_resp = (icf_nw_inc_media_change_resp_st *)
        					p_cc_pdb->p_internal_msg->p_msg_data;

     if (ICF_NULL != p_call_modify_resp)
     {
       if((ICF_TRUE == p_call_modify_resp->status) ||
          ((ICF_FALSE == p_call_modify_resp->status) &&
            (ICF_CALL_CLEAR_REASON_REQUEST_PENDING !=
                           p_call_modify_resp->reason_type)))
       {
          ret_val = icf_cc_stop_current_timer(p_cc_pdb);
       }
     }
     else
     {
          ret_val = icf_cc_stop_current_timer(p_cc_pdb);
     }  

	ICF_CC_RESET_SIG_ONGOING(p_cc_pdb)
	/* Return status as signalling done to CM */
    ICF_CC_SET_STATUS(p_cc_pdb, ICF_STATUS_SIGNAL_ONGOING_ENDED)
	
	if((ICF_NULL != ICF_CC_CHECK_COMMON_BITMASK(p_cc_pdb,
					ICF_FAX_CALL_ESTABLISHED)) &&
        (ICF_NULL != p_call_modify_resp))
	{
        if(ICF_TRUE == p_call_modify_resp->status)
        {
			/* Accept */
			/*
			1.	New MMS is required for the modifed streams.
			2.  Revert MMS required for rejected streams
			3.	send the answer to application after the MMS.
            */
            p_cc_pdb->p_list_1 = p_call_modify_resp->p_sdp_list;

            if(ICF_FAILURE == 
                icf_cc_make_mms_type_for_nth_rem_succ_ans(p_cc_pdb))
            {
                    p_cc_pdb->p_call_ctx->app_failure_cause = ICF_CAUSE_NETWORK_ERROR;			
			        ICF_CC_SET_COMMON_BITMASK(p_cc_pdb, 
                        ICF_FAX_RESP_FAIL)
                    icf_cc_send_pa_stop_fax_resp(p_cc_pdb);

			        ret_val = icf_cc_14_generic_err(p_cc_pdb);
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
					ICF_CC_SET_CALL_STATE(p_cc_pdb,ICF_CC_04_AWT_MMS_RESP)
					ICF_CC_SET_NEXT_STATE(p_cc_pdb,ICF_CC_06_CONNECTED)
					ICF_CC_SET_ONGOING_EVENT(p_cc_pdb,p_cc_pdb->current_event)
					/* Start the media timer  */
					ret_val = icf_cc_start_media_timer(p_cc_pdb);
				}
                else
                {

        			icf_cc_send_pa_stop_fax_resp(p_cc_pdb);
                    ICF_CC_SET_CALL_STATE(p_cc_pdb,ICF_CC_06_CONNECTED)
                }
				
			}/* MMS was handled succesfully */

		}
		else
		{
		    if(ICF_CALL_CLEAR_REASON_REQUEST_PENDING == 
                                     p_call_modify_resp->reason_type)
            {
                    ICF_CC_SET_COMMON_BITMASK(p_cc_pdb, 
                                   ICF_CC_491_RESP_PROCESS_ONGOING)
                    ret_val = icf_cc_start_retry_after_timer(p_cc_pdb,
                                 ICF_NULL);
                    
                   ICF_CC_RESET_SIG_ONGOING(p_cc_pdb)
	              /* Return status as signalling done to CM */
                   ICF_CC_SET_STATUS(p_cc_pdb, 
                             ICF_STATUS_SIGNAL_ONGOING_ENDED)
                   return ret_val;
            }
 
           	p_cc_pdb->p_call_ctx->app_failure_cause = ICF_CAUSE_REJECT_MEDIA_CHANGE_REQ;
            ICF_CC_SET_COMMON_BITMASK(p_cc_pdb, ICF_FAX_RESP_FAIL)

            if(ICF_CALL_CLEAR_REASON_USER_NOT_FOUND ==
                            p_call_modify_resp->reason_type)
            {
            	p_cc_pdb->p_call_ctx->app_failure_cause = ICF_CAUSE_NETWORK_ERROR;
				/* terminate the call for these error reasons */
                p_cc_pdb->p_call_ctx->release_cause =
                            (icf_uint8_t)p_call_modify_resp->reason_type;
			}
			icf_cc_send_pa_stop_fax_resp(p_cc_pdb);
            ret_val = icf_cc_clear_call_at_call_modify(p_cc_pdb);
			ICF_CC_RESET_COMMON_BITMASK(p_cc_pdb,
                    ICF_FAX_CALL_ESTABLISHED)
		}

	}
	else
	{

     if(ICF_FAILURE == icf_cc_send_pa_stop_fax_resp(p_cc_pdb))
      {
       ret_val = ICF_FAILURE;
      }  

     ICF_CC_SET_COMMON_BITMASK(p_cc_pdb, ICF_FAX_RESP_FAIL)
     p_cc_pdb->p_call_ctx->app_failure_cause =  ICF_CAUSE_FAX_REQUEST_STOPPED;
     if(ICF_FAILURE == icf_cc_send_pa_fax_resp(p_cc_pdb))
      {
       ret_val = ICF_FAILURE;
	  }
	
     ICF_CC_SET_CALL_STATE(p_cc_pdb,ICF_CC_06_CONNECTED)
	}
	
     ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function entry trace */
     return ret_val;
} 

#endif


ICF_HANDLE_REINVITE_END

/*twc_update handling*/
/******************************************************************************
 *
 * FUNCTION:        icf_cc_14_pa_call_modify_req
 *
 * DESCRIPTION:     This function is invoked when the CC is awaiting 
 *                  call modify resp and
 *                  it receives a call modify request from PA.
 *
 *                  Current State: ICF_CC_14_AWT_CALL_MODIFY_RESP
 *                  Current Event: ICF_CC_PA_CALL_MODIFY_REQ 
 *
 *****************************************************************************/
icf_return_t icf_cc_14_pa_call_modify_req(
        INOUT icf_cc_pdb_st *p_cc_pdb)
{
    icf_return_t     ret_val = ICF_SUCCESS;
    icf_config_data_st  *p_config_data = ICF_NULL;
    icf_call_modify_req_st  *p_call_modify_req = ICF_NULL;

    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /* function entry trace */

    p_call_modify_req = (icf_call_modify_req_st*)(p_cc_pdb->p_glb_pdb->p_recv_msg->payload);

    /* Get pointer to config data from DBM */
    ret_val = icf_dbm_get_module_glb_data(
              p_cc_pdb->p_glb_pdb,
             (icf_int_module_id_t)ICF_INT_MODULE_CFG,
             (icf_void_t *)&p_config_data, p_cc_pdb->p_ecode);

    ICF_ASSERT(p_cc_pdb->p_glb_pdb, (ICF_NULL != p_config_data));
    /* reject the request*/
    p_cc_pdb->p_call_ctx->app_failure_cause = ICF_CAUSE_CALL_MODIFY_REQ_PENDING;
    ICF_CC_SET_COMMON_BITMASK(p_cc_pdb, ICF_CALL_MODIFY_RESP_FAIL)

    /* Send failure response for update to user */
    if(ICF_FAILURE == icf_cc_send_pa_call_modify_resp(p_cc_pdb))
    {
       ret_val = ICF_FAILURE;
    }

    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */
    return ret_val;

}


/*call_mod_cancel*/
/*******************************************************************************
 *
 * FUNCTION:        icf_cc_14_pa_call_modify_cancel_req
 *
 * DESCRIPTION:     This function is  used for handling the buffering of 100 trying
 *                  and reverting of media to last comitted SDP when call modify
 *                  cancel is sent by PA
 *
 ******************************************************************************/
icf_return_t icf_cc_14_pa_call_modify_cancel_req(
         INOUT icf_cc_pdb_st *p_cc_pdb)
{
     icf_return_t     ret_val = ICF_SUCCESS;
     icf_config_data_st  *p_config_data = ICF_NULL;

     ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /* function entry trace */
     /* check the call direction  */
     if(ICF_NULL != ICF_CC_CHECK_COMMON_BITMASK(p_cc_pdb,
                         ICF_CALL_MODIFY_DIR))
     {
        /*Do nothing*/
     }
     else
     {
         /* if an outgoing call modify request handle here */
         /* Get pointer to config data from DBM            */
         ret_val = icf_dbm_get_module_glb_data(
                    p_cc_pdb->p_glb_pdb,
                    (icf_int_module_id_t)ICF_INT_MODULE_CFG,
                    (icf_void_t *)&p_config_data, p_cc_pdb->p_ecode);

        /* check the bitmask for the 100 trying recvd */
        /* If not then make the call mod cancel as pending event */
        if(!(ICF_CALL_MODIFY_PROV_RESP_RECVD &
          p_cc_pdb->p_call_ctx->prov_resp_status))
        {
          ICF_CC_SET_PENDING_EVENT(p_cc_pdb,
                       ICF_PENDING_EVENT_PA_CALL_MOD_CANCEL_REQ)
        }
        else
        {
            if(ICF_FAILURE == icf_cc_revert_mms_for_call_mod_cancel_req(p_cc_pdb))
            {
                ret_val = ICF_FAILURE;
            }
            else
            {

                if(ICF_TRUE == p_cc_pdb->mms_sent)
                {
                    /*Set common_bitmask*/
                    ICF_CC_SET_COMMON_BITMASK_2(p_cc_pdb, ICF_CALL_MOD_CANCEL_REQ)

                    ICF_CC_SET_CALL_STATE(p_cc_pdb,ICF_CC_04_AWT_MMS_RESP)
                    ICF_CC_SET_NEXT_STATE(p_cc_pdb,ICF_CC_15_AWT_CALL_MODIFY_ALERT_RESP)
                    ICF_CC_SET_ONGOING_EVENT(p_cc_pdb, ICF_CC_PA_CALL_MODIFY_CANCEL_REQ)
                }
                else
                {
                    /*Case when there is no media change,hence no mms is sent to mmi,
                      here directly send the cancel req over network*/
                    /*send cancel to NW */
                    if (ICF_FAILURE == icf_cc_send_ssa_cancel_req(p_cc_pdb))
                    {
                        ret_val = ICF_FAILURE;
                    }
                    ICF_CC_SET_NEXT_STATE(p_cc_pdb,ICF_CC_15_AWT_CALL_MODIFY_ALERT_RESP)
                    ICF_CC_SET_ONGOING_EVENT(p_cc_pdb, ICF_CC_PA_CALL_MODIFY_CANCEL_REQ)
                }
            }
        }
    }
    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function entry trace */
    return ret_val;
} 


/*call_mod_cancel*/
/*******************************************************************************
 *
 * FUNCTION:        icf_cc_14_ssa_call_modify_cancel_resp
 *
 * DESCRIPTION:     This function is called when 200 OK for cancel is recvd and
 *                  response of call modify cancel needs to be sent to PA.
 *
 ******************************************************************************/

icf_return_t icf_cc_14_ssa_call_modify_cancel_resp(
             INOUT icf_cc_pdb_st *p_cc_pdb)
{
    icf_return_t     ret_val = ICF_SUCCESS;
    icf_nw_inc_media_change_resp_st *p_call_modify_cancel_resp = ICF_NULL;

    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /* function entry trace */

    ICF_CC_RESET_SIG_ONGOING(p_cc_pdb)

    /* Return status as signalling done to CM */
    ICF_CC_SET_STATUS(p_cc_pdb, ICF_STATUS_SIGNAL_ONGOING_ENDED)

    ret_val = icf_cc_stop_current_timer(p_cc_pdb);

    if(ICF_FAILURE != ret_val)
    {
        /* check the call direction , it should be an
            outgoing call modify request here */
        if(ICF_NULL == ICF_CC_CHECK_COMMON_BITMASK(p_cc_pdb, ICF_CALL_MODIFY_DIR))
        {
            p_call_modify_cancel_resp = (icf_nw_inc_media_change_resp_st *)
                    p_cc_pdb->p_internal_msg->p_msg_data;

            /*If common_bitmask is ICF_CALL_MOD_CANCEL_REQ then send response to
             * application set next_state to call state*/
            if (ICF_NULL != ICF_CC_CHECK_COMMON_BITMASK_2(p_cc_pdb,ICF_CALL_MOD_CANCEL_REQ))
            {
                /*Send call modify_cancel_resp to application*/
                /*This bitmask is reset in this function after sending response*/
                if(ICF_FAILURE == icf_cc_send_pa_call_modify_cancel_resp(p_cc_pdb))
                {
                    ret_val = ICF_FAILURE;
                }
                if(ICF_FAILURE != ret_val)
                {
                    /* Call modify cancel response is followed by call modify req fail response*/
                    p_cc_pdb->p_call_ctx->app_failure_cause =
                                  ICF_CAUSE_REQUEST_CANCELLED_BY_USER;
                    ICF_CC_SET_COMMON_BITMASK(p_cc_pdb, ICF_CALL_MODIFY_RESP_FAIL)
                
                    if(ICF_FAILURE == icf_cc_send_pa_call_modify_resp(p_cc_pdb))
                    {
                        ret_val = ICF_FAILURE;
                    }
                    else
                    {
                        ICF_CC_SET_CALL_STATE(p_cc_pdb,ICF_CC_06_CONNECTED)
                        ret_val = ICF_SUCCESS;
                    }
                    return ret_val;
                 }
             }
         }
     }
     else
     {
        ret_val = ICF_FAILURE;
     }

     ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function entry trace */
     return ret_val;
} 

/*******************************************************************************
 *
 * FUNCTION:        icf_cc_14_ssa_stop_fax_req
 *
 * DESCRIPTION:     This function is  used for handling the buffering of 100 trying
 *                  and reverting of media for audio once stop fax req is send by PA
 *
 ******************************************************************************/
icf_return_t icf_cc_14_ssa_stop_fax_req(
         INOUT icf_cc_pdb_st *p_cc_pdb)
{
    icf_return_t     ret_val = ICF_SUCCESS;

    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb); /* function entry trace */

    if ((ICF_TIMER_ID_INVALID != p_cc_pdb->p_call_ctx->retry_after_timer_id) && \
        (ICF_NULL != p_cc_pdb->p_call_ctx->p_retry_after_timer_data))
    {
       /* Non-zero id means that the timer is running, so stop it */
       ICF_STOP_TIMER(p_cc_pdb->p_glb_pdb, p_cc_pdb->p_call_ctx->retry_after_timer_id,
               p_cc_pdb->p_ecode, ret_val)

       icf_dbm_ret_mem_to_pool(p_cc_pdb->p_glb_pdb,
                (icf_pool_id_t)ICF_MEM_TIMER,
                 p_cc_pdb->p_call_ctx->p_retry_after_timer_data,
                 p_cc_pdb->p_ecode);

       p_cc_pdb->p_call_ctx->p_retry_after_timer_data = ICF_NULL;
       p_cc_pdb->p_call_ctx->retry_after_timer_id = ICF_TIMER_ID_INVALID;

       ret_val  = icf_cc_process_ssa_stop_fax_req(p_cc_pdb);
       if ((ICF_FAILURE == ret_val) ||
        (ICF_NULL != p_cc_pdb->p_call_ctx->offer_failure_reason)) 
       {
           ICF_PRINT(((icf_uint8_t *)"FAILURE in mms type remote offer change. Sending 487"));
           ret_val = icf_cc_send_ssa_offer_chg_err(p_cc_pdb);
           p_cc_pdb->p_call_ctx->offer_failure_reason = ICF_NULL;
       }
       else if (ICF_FAILURE == icf_cc_send_mms_req(p_cc_pdb))
       {
          ICF_PRINT(((icf_uint8_t *)"Not able to make mms request. Sending 487"));
          ret_val = icf_cc_send_ssa_offer_chg_err(p_cc_pdb);
       }
       else
       {
          /* Give stop_fax_resp to application as succes.*/
          if (ICF_FAILURE == icf_cc_send_pa_stop_fax_resp(p_cc_pdb))
          {
             ret_val = ICF_FAILURE;
          }
          else if(ICF_TRUE == p_cc_pdb->mms_sent)
          {
             /* if mms was sent bcoz of change in parameters in audio
              * stream then set the call state to 04, Also change the Audio
              * set the common_bitmask with ICF_STOP_FAX_RESP_ALREADY_GIVEN.
              */   

             ret_val = icf_cc_start_media_timer(p_cc_pdb);
             /*
              * Update the states in the call context
              */
             ICF_PRINT(((icf_uint8_t *)"MMS has been sent successfully"));
             ICF_CC_SET_CALL_STATE(p_cc_pdb,ICF_CC_04_AWT_MMS_RESP);
             ICF_CC_SET_NEXT_STATE(p_cc_pdb,ICF_CC_06_CONNECTED);
             ICF_CC_SET_COMMON_BITMASK(p_cc_pdb, 
                                       ICF_STOP_FAX_RESP_ALREADY_GIVEN)
             ICF_CC_SET_ONGOING_EVENT(p_cc_pdb,ICF_CC_SSA_STOP_FAX_REQ)

          }
          else
          {
             if(ICF_FAILURE ==  icf_cc_send_ssa_offer_chg_rsp(p_cc_pdb))
             {
                ret_val = ICF_FAILURE;
             }
             ICF_CC_RESET_ONGOING_EVENT(p_cc_pdb)
             ICF_CC_SET_CALL_STATE(p_cc_pdb,ICF_CC_06_CONNECTED)
             ICF_CC_RESET_NEXT_STATE(p_cc_pdb)
             p_cc_pdb->p_call_ctx->fax_type = ICF_NO_FAX_CALL;
         }
      }
    }
    else
    {
       /* Retry-after Timer is not running and control is coming here
        * then IPTK will simply reject that offer.
        */
        p_cc_pdb->p_call_ctx->offer_failure_reason = ICF_CAUSE_REJECT_MEDIA_CHANGE_REQ;
        ret_val = icf_cc_send_ssa_offer_chg_err(p_cc_pdb);  
    } 

    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */
    return ret_val; 
}

