/*------------------------------------------------------------------------------
 *
 * File name        : icf_cc_fsm_01.c
 *
 * Purpose          : This file contains the event handlers in 
 *                    ICF_CC_01_AWT_CMS_RESP   state.
 *
 * Revision History :
 *
 * Date         Name            Ref#            Comments
 * --------     ------------    ----            ---------------
 * 14-Dec-2004  Umang Singh     Non Service LLD Initial 
 * 21-Dec-2004  Rohit Aggarwal                  Addition of traces, stats
 * 24-Dec-2004  Rohit Aggarwal                  Compilation fixes
 * 25-Dec-2004  Rohit Aggarwal                  Compilation fixes
 * 27-Dec-2004  Rohit Aggarwal                  Coding guidelines violation fix
 * 21-Apr-2005  Sumit Gupta                     Removed the code for sending SSA
 *                                              reject in case of cancel
 * 20-Apr-2005  Sumit Gupta      SPR 7427       Set call clear reason as
 *                                              ICF_CALL_CLEAR_REASON_REMOTE_
 *                                              REJECTED when call cancel was 
 *                                              received from remote
 * 01-Jun-2005  Jalaj Negi                      Added code for IMS CLIENT
 * 08-Mar-2006  Aman Aggarwal					ICF merging with ICF2.0
 * 26-Jun-2006  Amit Sharma     ICF 6.0 APIs    Structure changes for multiple
 *                                              m-lines
 * 20-Nov-2006  Umang Singh     SPR 13794
 * 17-Apr-2007  Abhishek Dutta  ICF 6.3         Added the changes for 
 *                                              TWC-UPDATE Handling
 * 19-Jul-2007  Amit Mahajan    ICF 6.5         Fix for SPR 15070 
 * 25-Jan-2008  Neha Gupta	ICF Rel8.0      Modifications for precondition
 *                                              in icf_cc_01_create_media_resp
 *                                              function
 * 24-Mar-2008  Tarun Gupta     IPTK Rel 8.0    Merged User Param and
 *                                              Dynamic Payload Enhancements
 * 07-Jun-2008  Amit Sharma       IPTK 8.0 SRTP LLD      SRTP Enhancements
 * 01-Dec-2008  Rajiv Kumar       SPR 19215     CSR-1-6212448 Merged
 * 12-Feb-2009  Tarun Gupta     Rel 8.1 LLD     SDP Tunnelling Enhancements  
 * 16-Feb-2009  Anurag Khare    Rel 8.1         Call modify Enhancements
 * 19-Feb-2009  Abhishek Dhammawat Rel 8.1      SDP Tunnelling QOS ftr
 * 03-Mar-2009  Tarun Gupta     Rel 8.1         Modified icf_cc_01_mm_err_ind
 * 04-Mar-2009  Tarun Gupta     Rel 8.1         Modified icf_cc_01_pa_alert,
 *                                              icf_cc_01_pa_media_connect_req
 * 30-Mar-2009  Anurag Khare    REl 8.2         changes done for Prack Enh
 * 26-May-2009  Anuradha Gupta  SPR 19672       CSR 1-6727350 Merge
 * 08-Jun-2009  Kamal Ashraf    SPR 19590       Modified 
 *                                              icf_cc_01_create_media_resp,
 *                                              icf_cc_01_pa_alert and 
 *                                              icf_cc_01_pa_media_connect_req
 * 05-Jul-2009 Anuradha Gupta     SPR 19754     Modified icf_cc_01_pa_alert,
 *                                                 icf_cc_01_pa_media_connect_req 
 * 07-Sep-2009 Anuradha Gupta     SPR 19999     Merged CSR 1-7505190
 *
 * Copyright (c) 2009, Aricent
 *---------------------------------------------------------------------------*/

#include "icf_cc_prototypes.h"
#include "icf_cc_macro.h"
#include "icf_dbm_prototypes.h"


/******************************************************************************
 *
 * FUNCTION:        icf_cc_01_create_media_resp
 *
 * DESCRIPTION:     This function is invoked when the CC is awaiting
 *                  create media session resp from MMI and it receives
 *                  create media session resp from MMI.
 *                  In the case of an outgoing call,setup request
 *                  has to be sent to the SSA and in the case of an
 *                  incoming call,an incoming call indication has to
 *                  be sent to PA  
 *                  Current State: ICF_CC_01_AWT_CMS_RESP
 *                  Current Event: ICF_CC_CMS_RESP
 *
 *****************************************************************************/
icf_return_t icf_cc_01_create_media_resp(
        INOUT icf_cc_pdb_st *p_cc_pdb)
{
    icf_return_t     ret_val = ICF_SUCCESS;
 
    
    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /* function entry trace */

    /* Reset media ongoing flag */
    ICF_CC_RESET_MEDIA_ONGOING(p_cc_pdb)

    /* Check the call type from the call context */
    ICF_CC_TRACE(p_cc_pdb, ICF_TRACE_CALL_DIR)
           
    if (ICF_SUCCESS != ret_val)
    {

    }
    else if (ICF_NULL != ICF_CC_CHECK_COMMON_BITMASK(p_cc_pdb,
                        ICF_CALL_DIR))
    {
		ret_val=icf_cc_update_stream_sdps_aftr_cms_resp(p_cc_pdb);
		if (ICF_CC_ALL_STREAMS_DELETED == 
            (p_cc_pdb->common_bitmask & ICF_CC_ALL_STREAMS_DELETED))
		{
            /* SPR 19999 - Fix for CSR 1-7505190 
			*  the problem occurs when ICF_GCL_RESP Fails of replace
			*  initiated call, In this case CM was triggering CC
			*  icf_cc_00_pa_setup_req baecause replace init call id is not null
			*  Now the code change is done as if it is a replace init call and 
			*  MM failure occurs then CM shall not trigger CC icf_cc_00_pa_setup_req
			*/
			p_cc_pdb->p_call_ctx->replace_initiated_call_id = ICF_INVALID_CALL_ID;
			ret_val = icf_cc_01_mm_err_ind(p_cc_pdb);
		}
		else
		{
			if (ICF_FAILURE != ret_val)
			{
                /* In tunnel mode, following function call is not applicable
                   as we do not understand media modes.
                */
ICF_CHECK_TUNNEL_MODE_START(p_cc_pdb->p_glb_pdb,ICF_FALSE)                
                icf_cc_set_local_sdp_media_mode_from_rem_mode(p_cc_pdb);
ICF_CHECK_TUNNEL_MODE_END
				/*
				 * For a fast start call set media commited to TRUE
				 */
				if((ICF_NULL != ICF_CC_CHECK_COMMON_BITMASK(p_cc_pdb,
								   ICF_CALL_MEDIA_SETUP_TYPE)))
				{
  /*  Rel 8.1
   *  now this field will be set 
   *  when 18x or 2xx will be sent
   */
ICF_CHECK_TUNNEL_MODE_START(p_cc_pdb->p_glb_pdb,ICF_FALSE)
					/* Set media commited to true */
					p_cc_pdb->p_call_ctx->media_committed = ICF_TRUE;

					/* Return status as media commited  to CM */
					ICF_CC_SET_STATUS(p_cc_pdb, ICF_STATUS_MEDIA_COMMITTED)

				   /* Set trace for media commited*/
					ICF_CC_TRACE(p_cc_pdb, ICF_TRACE_MEDIA_COMMITTED) 
ICF_CHECK_TUNNEL_MODE_END
				}
                else
                {
                   /* Fix for SPR 15070, p_offered_sdp will be formed in                  
                    * case of fast start call.*/
                    ret_val = 
                      icf_cc_make_duplicate_of_offered_sdp_per_stream(p_cc_pdb);

                }


#ifdef ICF_QOS_SUPPORT
ICF_CHECK_QOS_SUPPORT_START(p_cc_pdb->p_glb_pdb,ICF_TRUE)
                 /*Invoke the 183 automatically sending only in case*/    
                 /*of non tunnel mode */
ICF_CHECK_TUNNEL_MODE_START(p_cc_pdb->p_glb_pdb,ICF_FALSE)

				/* Initilaize the local SDP with default Qos values */
				icf_cc_fill_local_sdp_with_default_qos_values(p_cc_pdb);

				/* Set current_status values in local send/recv of local_sdp to
				 * ICF_RESOURCE_RESERVED */
				icf_cc_fill_local_sdp_with_curr_values_inc_call(p_cc_pdb);
                
				/* Now we will be updating the Qos parameters in local SDP
				 * structure by comparing it with Qos parameters in remote
				 * received SDP. For invite without SDP we do not have 
                                 * remote SDP.
				 */
                                if(ICF_NULL != ICF_CC_CHECK_COMMON_BITMASK(p_cc_pdb,
                            				ICF_CALL_MEDIA_SETUP_TYPE))
                                 {
				      icf_cc_compare_and_update_qos_params(p_cc_pdb);
                                 } 
            
				/* Send media param to the SSA. This function will send
				 * 183 with SDP to peer
				 */
            
				if (ICF_FAILURE == icf_cc_send_ssa_media_param(p_cc_pdb))
				{
					ret_val = ICF_FAILURE;
				} /* if (ICF_FAILURE == icf_c... */
				else
				{
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
                    
						/* Reset confirmation status so that it will not be sent
						* in 200 OK for UPDATE 
						*/
						p_sdp->p_local_sdp->remote_send.confirmation_status = 
							ICF_RESOURCE_RESERVATION_CONF_NOT_REQ;
						p_sdp->p_local_sdp->remote_recv.confirmation_status = 
							ICF_RESOURCE_RESERVATION_CONF_NOT_REQ;
                    
						p_list = (icf_internal_list_st*)p_list->p_next;
					}
                
					ICF_CC_TRACE(p_cc_pdb, ICF_TRACE_PRACK_REQD)

					if (ICF_TRUE == p_cc_pdb->prack_reqd)
					{
						/*
						 * Update the states in the call context
						 */
						ICF_CC_SET_CALL_STATE(p_cc_pdb,
								ICF_CC_09_AWT_MEDIA_PARAM_PRACK)
					} /* if (ICF_TRUE == p_cc_pdb->p... */
				}
ICF_CHECK_TUNNEL_MODE_END
ICF_CHECK_TUNNEL_MODE_START(p_cc_pdb->p_glb_pdb,ICF_TRUE)
                /*In tunnel mode we shall not be triggering the 183
                 automatically but shall do so when we receive the
                 first signalling API 180/183/200. Therefore just set
                 the state to await alert.This was done so that PA
                 does have same behaviour of triggerring 183 once we
                 receive signalling API.*/
						ICF_CC_SET_CALL_STATE(p_cc_pdb,
								ICF_CC_03_AWT_ALERT_RESP)
ICF_CHECK_TUNNEL_MODE_END
ICF_CHECK_QOS_SUPPORT_END
#endif
			}
#ifdef ICF_QOS_SUPPORT
ICF_CHECK_QOS_SUPPORT_START(p_cc_pdb->p_glb_pdb,ICF_FALSE)
			if (ICF_INVALID_CALL_ID == p_cc_pdb->p_call_ctx->replace_initiated_call_id)
            {
                /* For tunnel mode incoming call indication has already been sent to 
                   application in the 20th state.
                   Also do not excecute the call waiting code leg as call waiting
                   service is not supported.
                */
ICF_CHECK_TUNNEL_MODE_START(p_cc_pdb->p_glb_pdb,ICF_FALSE)

     			/*
	    		 * Send an incoming call indication to PA
		    	 */
			    if (ICF_FAILURE  == icf_cc_send_pa_incoming_call_ind(
				  		p_cc_pdb))

				{

					ret_val = ICF_FAILURE;
				}
				else
				{
					/* If call waiting indication was successfully given to PA then 
					 * update stat ICF_STAT_SUCC_CALL_WAITING*/
					if(ICF_TRUE == p_cc_pdb->p_call_ctx->call_waiting_call)
					{
						ICF_STAT(p_cc_pdb->p_glb_pdb,ICF_STAT_SUCC_CALL_WAITING)
					}

					/*
					 * Update the states in the call context
					 */
					ICF_CC_SET_CALL_STATE(
							p_cc_pdb,ICF_CC_03_AWT_ALERT_RESP)
                }
ICF_CHECK_TUNNEL_MODE_END
ICF_CHECK_TUNNEL_MODE_START(p_cc_pdb->p_glb_pdb,ICF_TRUE)
  
				/* In tunnel mode, we will just change the state to 03 */
				ICF_CC_SET_CALL_STATE(
							p_cc_pdb,ICF_CC_03_AWT_ALERT_RESP)

ICF_CHECK_TUNNEL_MODE_END

				if(ICF_TRUE == p_cc_pdb->p_call_ctx->media_committed &&
						ICF_NULL != ICF_CC_CHECK_PENDING_EVENT(p_cc_pdb,
							ICF_PENDING_EVENT_PA_CALL_CONNECT))
				{
                    /* SPR 19590 : Extract the txn id from pending event
                       list corresponding to input bit so that we work on
                       correct header list using the extracted txn-id */
                       icf_cc_extract_txnid_from_pending_event_list(p_cc_pdb,
                                             ICF_PENDING_EVENT_PA_CALL_CONNECT);
                    /* SPR 19590 : End */
					/*
					 * Send Modify Media Session to the MMI
					 */
					ret_val = icf_cc_snd_mms_pa_connect(p_cc_pdb);
					if(ICF_FALSE == p_cc_pdb->mms_sent &&
							ICF_SUCCESS == ret_val)
					{
						icf_cc_mms_rsp_pa_connect(p_cc_pdb);
					}
				}
			}/*End of if (ICF_FAILURE != ret_val)*/

            /* In tunnel mode, REFER with replaces is not supported */
ICF_CHECK_TUNNEL_MODE_START(p_cc_pdb->p_glb_pdb,ICF_FALSE)

            /* Fix for CSR 1-6727350: SPR 19316 */
			/* Check whether APP Connect is required for Replace-initiated call.
			 * If it is required, send incoming_call_ind to APP and proceed as normal
			 * incoming call, else directly send Connect to SSA.
			 */
            if ((ICF_TRUE == p_cc_pdb->p_config_data->app_connect_reqd_for_replaces) &&
					 (ICF_INVALID_CALL_ID != p_cc_pdb->p_call_ctx->replace_initiated_call_id))
			{
	            icf_call_ctx_st 	*p_old_context = ICF_NULL;
            	icf_call_ctx_st 	*p_replace_initiated_call_ctx = p_cc_pdb->p_call_ctx;
                          
				if(ICF_SUCCESS == icf_dbm_find_call_cntxt_blk(
                              p_cc_pdb->p_glb_pdb,
                              p_cc_pdb->p_call_ctx->replace_initiated_call_id,
                              &p_old_context,
                              p_cc_pdb->p_ecode))
                {
                    p_cc_pdb->p_call_ctx = p_old_context;
                    p_cc_pdb->p_glb_pdb->p_call_ctx = p_old_context;
                    /* No need to send any indication to MM/SSA/NW
                     * it is already done in forward path.
					 */
                    ICF_CC_SET_CALL_CLR_STATUS(p_cc_pdb,
                              ICF_CALL_CLEAR_STATUS_NW_CLEARED)
                    ICF_CC_SET_CALL_CLR_STATUS(p_cc_pdb,
                              ICF_CALL_CLEAR_STATUS_PA_CLEARED)
                    ICF_CC_SET_CALL_CLR_STATUS(p_cc_pdb,
                              ICF_CALL_CLEAR_STATUS_MM_CLEARED)

                    /* Delete the old context */
                    icf_cc_00_clear_call(p_cc_pdb);
                }

                p_cc_pdb->p_call_ctx = p_replace_initiated_call_ctx;
                p_cc_pdb->p_glb_pdb->p_call_ctx = p_replace_initiated_call_ctx;
				p_cc_pdb->p_call_ctx->replace_initiated_call_id = ICF_INVALID_CALL_ID;

				if (ICF_FAILURE == icf_cc_send_pa_incoming_call_ind(p_cc_pdb))
				{
					ret_val = ICF_FAILURE;
				}	
				else
				{
					ICF_CC_SET_CALL_STATE(p_cc_pdb,ICF_CC_03_AWT_ALERT_RESP)
				}
			}
            else if ((ICF_FALSE == p_cc_pdb->p_config_data->app_connect_reqd_for_replaces) &&
					 (ICF_INVALID_CALL_ID != p_cc_pdb->p_call_ctx->replace_initiated_call_id))
			{
				/* directly send final response to peer */
				p_cc_pdb->media_to_be_sent = ICF_TRUE;
				ret_val = icf_cc_send_ssa_connect(p_cc_pdb);
				if (ICF_SUCCESS == ret_val)
				{
					/* set call state as await connect ack */
					ICF_CC_SET_CALL_STATE(p_cc_pdb, ICF_CC_11_AWT_SSA_CONNECT_ACK)
					/* now we can send the party_replaced_ind to CC and 
					* then reset the replace_initiated_call_id in this new call_ctx */
					ret_val = icf_cc_send_pa_party_replaced_ind(p_cc_pdb);
					if(ICF_SUCCESS == ret_val)
					{
						  icf_call_ctx_st *p_old_context = ICF_NULL;
						  icf_call_ctx_st *p_replace_initiated_call_ctx = p_cc_pdb->p_call_ctx;
						  /* Fetch Call Context of the replace_initiated_call*/
						  /* update line_id,mode from old to new */
						  if(ICF_SUCCESS == icf_dbm_find_call_cntxt_blk(
							  p_cc_pdb->p_glb_pdb,
							  p_cc_pdb->p_call_ctx->replace_initiated_call_id,
							  &p_old_context,
							  p_cc_pdb->p_ecode))
						  {
							  p_cc_pdb->p_call_ctx = p_old_context;
							  p_cc_pdb->p_glb_pdb->p_call_ctx = p_old_context;
							  /* No need to send any indication to MM/SSA/NW
							   * it is already done in forward path */
							ICF_CC_SET_CALL_CLR_STATUS(p_cc_pdb,
							  ICF_CALL_CLEAR_STATUS_NW_CLEARED)
							ICF_CC_SET_CALL_CLR_STATUS(p_cc_pdb,
							  ICF_CALL_CLEAR_STATUS_PA_CLEARED)
							ICF_CC_SET_CALL_CLR_STATUS(p_cc_pdb,
							  ICF_CALL_CLEAR_STATUS_MM_CLEARED)

							  /* Delete the old context */
							  icf_cc_00_clear_call(p_cc_pdb);

						  }

						   p_cc_pdb->p_call_ctx = p_replace_initiated_call_ctx;
						   p_cc_pdb->p_glb_pdb->p_call_ctx = p_replace_initiated_call_ctx;
                      
					  }
					  p_cc_pdb->p_call_ctx->replace_initiated_call_id = ICF_INVALID_CALL_ID;
				}

			}
ICF_CHECK_TUNNEL_MODE_END

ICF_CHECK_QOS_SUPPORT_END
#else
			if (ICF_INVALID_CALL_ID == p_cc_pdb->p_call_ctx->replace_initiated_call_id)
			{
                /* For tunnel mode incoming call indication has already been sent to 
                   application in the 20th state.
                   Also do not excecute the call waiting code leg as call waiting
                   service is not supported.
                */
ICF_CHECK_TUNNEL_MODE_START(p_cc_pdb->p_glb_pdb,ICF_FALSE)

     			/*
	    		 * Send an incoming call indication to PA
				 */
				if (ICF_FAILURE  == icf_cc_send_pa_incoming_call_ind(
				  		p_cc_pdb))

				{

					ret_val = ICF_FAILURE;
				}
				else
				{

					/* If call waiting indication was successfully given to PA then 
					 * update stat ICF_STAT_SUCC_CALL_WAITING*/
					if(ICF_TRUE == p_cc_pdb->p_call_ctx->call_waiting_call)
					{
						ICF_STAT(p_cc_pdb->p_glb_pdb,ICF_STAT_SUCC_CALL_WAITING)
					}
                    
					/*
					 * Update the states in the call context
					 */
					ICF_CC_SET_CALL_STATE(
							p_cc_pdb,ICF_CC_03_AWT_ALERT_RESP)
                }
ICF_CHECK_TUNNEL_MODE_END                        

ICF_CHECK_TUNNEL_MODE_START(p_cc_pdb->p_glb_pdb,ICF_TRUE)
               
                /* In tunnel mode, we will just change the state to 03 */
				ICF_CC_SET_CALL_STATE(
							p_cc_pdb,ICF_CC_03_AWT_ALERT_RESP)
ICF_CHECK_TUNNEL_MODE_END

				if(ICF_TRUE == p_cc_pdb->p_call_ctx->media_committed &&
						ICF_NULL != ICF_CC_CHECK_PENDING_EVENT(p_cc_pdb,
							ICF_PENDING_EVENT_PA_CALL_CONNECT))
				{
                    /* SPR 19590 : Extract the txn id from pending event 
                       list corresponding to input bit so that we work on
                       correct header list using the extracted txn-id */
                    icf_cc_extract_txnid_from_pending_event_list(p_cc_pdb, 
                                         ICF_PENDING_EVENT_PA_CALL_CONNECT); 
                    /* SPR 19590 : End */
					/*
					 * Send Modify Media Session to the MMI
					 */
					ret_val = icf_cc_snd_mms_pa_connect(p_cc_pdb);
					if(ICF_FALSE == p_cc_pdb->mms_sent &&
							ICF_SUCCESS == ret_val)
					{
						icf_cc_mms_rsp_pa_connect(p_cc_pdb);
					}
				}
			}/*End of if (ICF_FAILURE != ret_val)*/
            /* In tunnel mode, REFER with replaces is not supported */
ICF_CHECK_TUNNEL_MODE_START(p_cc_pdb->p_glb_pdb,ICF_FALSE)

            /* Fix for CSR 1-6727350: SPR 19316 */
            /* Check whether APP Connect is required for Replace-initiated call.
             * If it is required, send incoming_call_ind to APP and proceed as normal
             * incoming call, else directly send Connect to SSA. 
             */
            if(ICF_NULL == p_cc_pdb->p_config_data)
            {
                /* do nothing return failure
                 */
                ret_val = ICF_FAILURE;
            }
            else if ((ICF_TRUE == p_cc_pdb->p_config_data->app_connect_reqd_for_replaces) &&
                     (ICF_INVALID_CALL_ID != p_cc_pdb->p_call_ctx->replace_initiated_call_id))
            {
                icf_call_ctx_st     *p_old_context = ICF_NULL;
                icf_call_ctx_st     *p_replace_initiated_call_ctx = p_cc_pdb->p_call_ctx;
        
                if(ICF_SUCCESS == icf_dbm_find_call_cntxt_blk(
                              p_cc_pdb->p_glb_pdb,
                              p_cc_pdb->p_call_ctx->replace_initiated_call_id,
                              &p_old_context,
                              p_cc_pdb->p_ecode))
                {
                    p_cc_pdb->p_call_ctx = p_old_context;
                    p_cc_pdb->p_glb_pdb->p_call_ctx = p_old_context;
                    /* No need to send any indication to MM/SSA/NW
                     * it is already done in forward path.
                     */
                    ICF_CC_SET_CALL_CLR_STATUS(p_cc_pdb,
                              ICF_CALL_CLEAR_STATUS_NW_CLEARED)
                    ICF_CC_SET_CALL_CLR_STATUS(p_cc_pdb,
                              ICF_CALL_CLEAR_STATUS_PA_CLEARED)
                    ICF_CC_SET_CALL_CLR_STATUS(p_cc_pdb,
                              ICF_CALL_CLEAR_STATUS_MM_CLEARED)

                    /* Delete the old context */
                    icf_cc_00_clear_call(p_cc_pdb);
                }

                p_cc_pdb->p_call_ctx = p_replace_initiated_call_ctx;
                p_cc_pdb->p_glb_pdb->p_call_ctx = p_replace_initiated_call_ctx;
                p_cc_pdb->p_call_ctx->replace_initiated_call_id = ICF_INVALID_CALL_ID;

                if (ICF_FAILURE == icf_cc_send_pa_incoming_call_ind(p_cc_pdb))
                {
                    ret_val = ICF_FAILURE;
                }
                else
                {
                    ICF_CC_SET_CALL_STATE(p_cc_pdb,ICF_CC_03_AWT_ALERT_RESP)
                }
            }
            else if ((ICF_FALSE == p_cc_pdb->p_config_data->app_connect_reqd_for_replaces) &&
                     (ICF_INVALID_CALL_ID != p_cc_pdb->p_call_ctx->replace_initiated_call_id))
			{
				/* directly send final response to peer */
				p_cc_pdb->media_to_be_sent = ICF_TRUE;
				ret_val = icf_cc_send_ssa_connect(p_cc_pdb);
				if (ICF_SUCCESS == ret_val)
				{
					/* set call state as await connect ack */
					ICF_CC_SET_CALL_STATE(p_cc_pdb, ICF_CC_11_AWT_SSA_CONNECT_ACK)
					/* now we can send the party_replaced_ind to CC and 
					* then reset the replace_initiated_call_id in this new call_ctx */
					ret_val = icf_cc_send_pa_party_replaced_ind(p_cc_pdb);
					if(ICF_SUCCESS == ret_val)
					{
						  icf_call_ctx_st *p_old_context = ICF_NULL;
						  icf_call_ctx_st *p_replace_initiated_call_ctx = p_cc_pdb->p_call_ctx;
						  /* Fetch Call Context of the replace_initiated_call*/
						  /* update line_id,mode from old to new */
						  if(ICF_SUCCESS == icf_dbm_find_call_cntxt_blk(
							  p_cc_pdb->p_glb_pdb,
							  p_cc_pdb->p_call_ctx->replace_initiated_call_id,
							  &p_old_context,
							  p_cc_pdb->p_ecode))
						  {
							  p_cc_pdb->p_call_ctx = p_old_context;
							  p_cc_pdb->p_glb_pdb->p_call_ctx = p_old_context;
							  /* No need to send any indication to MM/SSA/NW
							   * it is already done in forward path */
							ICF_CC_SET_CALL_CLR_STATUS(p_cc_pdb,
							  ICF_CALL_CLEAR_STATUS_NW_CLEARED)
							ICF_CC_SET_CALL_CLR_STATUS(p_cc_pdb,
							  ICF_CALL_CLEAR_STATUS_PA_CLEARED)
							ICF_CC_SET_CALL_CLR_STATUS(p_cc_pdb,
							  ICF_CALL_CLEAR_STATUS_MM_CLEARED)

							  /* Delete the old context */
							  icf_cc_00_clear_call(p_cc_pdb);

						  }

						   p_cc_pdb->p_call_ctx = p_replace_initiated_call_ctx;
						   p_cc_pdb->p_glb_pdb->p_call_ctx = p_replace_initiated_call_ctx;
              
					  }
					  p_cc_pdb->p_call_ctx->replace_initiated_call_id = ICF_INVALID_CALL_ID;
				}

			}
ICF_CHECK_TUNNEL_MODE_END
#endif
		}/*End of if (ICF_CC_STREAM_POS_VALIDATION_FA */
	}/* end of if for incomimg call processing */
    else /* outgoing call processing */
    { 
        /* In tunnel mode, following function call is not applicable
           as we do not understand media modes.
        */
ICF_CHECK_TUNNEL_MODE_START(p_cc_pdb->p_glb_pdb,ICF_FALSE)
		/* CSR 1-6212448: Set the media mode in local sdp */
		icf_cc_set_local_sdp_media_mode(p_cc_pdb, 
							ICF_MEDIA_MODE_SEND_RECV);
ICF_CHECK_TUNNEL_MODE_END

		ret_val=icf_cc_update_stream_sdps_aftr_cms_resp(p_cc_pdb);
		if (ICF_CC_ALL_STREAMS_DELETED == 
			(p_cc_pdb->common_bitmask & ICF_CC_ALL_STREAMS_DELETED))
		{
			ret_val = icf_cc_01_mm_err_ind(p_cc_pdb);
		}
		else 
		{
			if (ICF_FAILURE != ret_val)
			{
		
#ifdef ICF_QOS_SUPPORT
ICF_CHECK_QOS_SUPPORT_START(p_cc_pdb->p_glb_pdb,ICF_TRUE)

                             ret_val = icf_cc_fill_local_sdp_with_default_qos_values(
                                       p_cc_pdb);

ICF_CHECK_QOS_SUPPORT_END
#endif
              
                       /* In tunnel mode, we do not understand SRTP streams and modes. */
ICF_CHECK_TUNNEL_MODE_START(p_cc_pdb->p_glb_pdb,ICF_FALSE)
                       /*Check if any SRTP stream present in local SDP, if not then
                        *this function will reset srtp_fallback_reqd flag in call context*/
                       icf_cc_check_for_srtp_streams_in_local_sdp(p_cc_pdb);
ICF_CHECK_TUNNEL_MODE_END

		              /*
		               * Send call setup to SSA with event name as
		               * ICF_CC_SSA_CREATE_CALL_REQ which will send the
		               * INVITE on the network.
		               */
		               if (ICF_FAILURE == icf_cc_send_ssa_setup_req(p_cc_pdb))
		               {
		                   ret_val = ICF_FAILURE;
		               }
		               else
		               {
		                   /*
			            * Update the states in the call context
			           */
			           ICF_CC_SET_CALL_STATE(p_cc_pdb,ICF_CC_02_AWT_SSA_SETUP_RESP)
                                   /* Newly added function, which will make deep copy of   
                                   * p_local_sdp into the p_offer_sdp on per stream basis.*/
                                   ret_val = 
                                   icf_cc_make_duplicate_of_offered_sdp_per_stream(p_cc_pdb);
		              }
			}
		}/*End of if (ICF_CC_STREAM_POS_VALIDATION_FAI...*/
    } /* end of else */
    
    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */
    return ret_val;
}


/******************************************************************************
 *
 * FUNCTION:        icf_cc_01_pa_call_term_req
 *
 * DESCRIPTION:     This function is invoked when the CC is awaiting
 *                  create media session resp from MMI and it receives
 *                  Call Terminate Request from PA.
 *                  Current State: ICF_CC_01_AWT_CMS_RESP
 *                  Current Event: ICF_CC_PA_CALL_TERMINATE_REQ
 *
 *****************************************************************************/

icf_return_t icf_cc_01_pa_call_term_req (
        INOUT icf_cc_pdb_st *p_cc_pdb)
{
    icf_return_t     ret_val = ICF_SUCCESS;

    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /* function entry trace */

    /* Check the call type from the call context */
    ICF_CC_TRACE(p_cc_pdb, ICF_TRACE_CALL_DIR)

    if (ICF_NULL != ICF_CC_CHECK_COMMON_BITMASK(p_cc_pdb,
                ICF_CALL_DIR))
    {
     /*
     * Stop the call setup timer and start the release timer
     */
    if (ICF_FAILURE == icf_cc_start_release_timer(p_cc_pdb))
      {
        ret_val = ICF_FAILURE;
      }

         /*
         * Set release type in call context as PA INITIATED
         */
        ICF_CC_TRACE(p_cc_pdb, ICF_TRACE_REL_TYPE_PA_INIT)
        ICF_CC_SET_RELEASE_TYPE(p_cc_pdb, ICF_REL_TYPE_PA_INITIATED)

        /* Set the release cause in call context */
        ICF_CC_SET_RELEASE_CAUSE(p_cc_pdb,
                     ICF_CALL_CLEAR_REASON_USER_NOT_FOUND)
        /*
         * Send a DMS request to MM
         */
        if (ICF_FAILURE == icf_cc_send_dms_req(p_cc_pdb))
        {
            ret_val = ICF_FAILURE;
        }
         icf_cc_validate_response_code_in_term_req(p_cc_pdb);

         ret_val = icf_cc_send_clear_req_to_ssa(p_cc_pdb);
     }
    /*
     * Stop the call setup timer and start the release timer
     */
    else if (ICF_FAILURE == icf_cc_start_release_timer(p_cc_pdb))
    {
        ret_val = ICF_FAILURE;
    }
    else
    {
        /*
         * Set release type in call context as PA INITIATED
         */
        ICF_CC_TRACE(p_cc_pdb, ICF_TRACE_REL_TYPE_PA_INIT)
        ICF_CC_SET_RELEASE_TYPE(p_cc_pdb, ICF_REL_TYPE_PA_INITIATED) 
        /*
         * Send a DMS request to MM
         */
        if (ICF_FAILURE == icf_cc_send_dms_req(p_cc_pdb))
        {
            ret_val = ICF_FAILURE;
        }
        else
        {
            ret_val = icf_cc_nw_clear_handler(p_cc_pdb);
        }
    } /* else */

    if (ICF_SUCCESS == ret_val)
    {
        /*
         * Update the states in the call context
         */
        ICF_CC_SET_CALL_STATE(p_cc_pdb, ICF_CC_07_AWT_DMS_RESP)
    }
    
    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */
    return ret_val;
}

/******************************************************************************
 *
 * FUNCTION:        icf_cc_01_mm_err_ind
 *
 * DESCRIPTION:     This function is invoked when the CC is awaiting
 *                  create media session resp from MMI and it receives
 *                  an error indication from MMI..
 *                  Current State: ICF_CC_01_AWT_CMS_RESP
 *                  Current Event: ICF_CC_MM_ERR_IND
 *
 *****************************************************************************/

icf_return_t icf_cc_01_mm_err_ind (
        INOUT icf_cc_pdb_st *p_cc_pdb)
{
    icf_return_t     ret_val = ICF_SUCCESS;

    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /* function entry trace */

    /* If no codec could be allocated for call the check for call busy service 
     * for being active. If it is active than forward the call else clear the 
     * call */
    if(ICF_CALL_CLEAR_REASON_RESOURCES_NOT_AVAILABLE == 
		    ICF_CC_GET_RELEASE_CAUSE(p_cc_pdb))
    {
	    /* Check if user has actived CALL_FWD_BUSY_LINE service. If he has 
	     * activated the service then dont release the call*/
	    ret_val = icf_cc_chk_user_busy_n_srvc(p_cc_pdb);
    }
    if(ICF_NULL == ICF_CC_GET_RELEASE_CAUSE(p_cc_pdb))
    {
	    ICF_CC_SET_RELEASE_CAUSE(p_cc_pdb,ICF_CALL_CLEAR_MEDIA_MISMATCH)
    }

#ifdef ICF_QOS_SUPPORT
ICF_CHECK_QOS_SUPPORT_START(p_cc_pdb->p_glb_pdb,ICF_TRUE)
/*Fill desired values as failure to send 580	 */
	 if(ICF_CALL_CLEAR_REASON_PRECONDITIONS_FAILURE == 
		 ICF_CC_GET_RELEASE_CAUSE(p_cc_pdb))
	 {
		
		icf_stream_sdp_info_st      *p_sdp = ICF_NULL;                
		icf_internal_list_st        *p_list = ICF_NULL;
    
		p_list = p_cc_pdb->p_call_ctx->p_stream_sdp_info;
    
		while (ICF_NULL != p_list)
		{       
			p_sdp = (icf_stream_sdp_info_st*)(p_list->p_payload);

			if(ICF_NULL == p_sdp->p_local_sdp)
			{
				p_list = p_list->p_next;
				continue;
			}

			/* Desired Status set as failure for all directions
			   to send 580 on network */
						   
			p_sdp->p_local_sdp->local_send.desired_status = 
									 ICF_RESOURCE_RESERVATION_FAILURE;
			p_sdp->p_local_sdp->local_recv.desired_status = 
									 ICF_RESOURCE_RESERVATION_FAILURE;
			p_sdp->p_local_sdp->remote_send.desired_status = 
									 ICF_RESOURCE_RESERVATION_FAILURE;
			p_sdp->p_local_sdp->remote_recv.desired_status = 
									 ICF_RESOURCE_RESERVATION_FAILURE;

			p_list = (icf_internal_list_st*)p_list->p_next;
		}

	 }
ICF_CHECK_QOS_SUPPORT_END
#endif

	 /*If status was not returned as ICF_STATUS_CALL_FWD_BUSY_LINE continue 
     *with normal call clearing*/
	 if(ICF_SUCCESS == ret_val && 
		 ICF_STATUS_CALL_FWD_BUSY_LINE != 
		 p_cc_pdb->p_internal_msg->msg_hdr.status)
	 {
		 	 
		/*
		 * Set release type in call context as MM INITIATED
		 */
		 ICF_CC_TRACE(p_cc_pdb, ICF_TRACE_REL_TYPE_MM_INIT)
		 ICF_CC_SET_RELEASE_TYPE(p_cc_pdb,ICF_REL_TYPE_MM_INITIATED) 
		 
		 /*
		  * Stop the call setup timer and start the release timer
		  */
		  if (ICF_FAILURE == icf_cc_start_release_timer(p_cc_pdb))
		  {
			  ret_val = ICF_FAILURE;
		  }
		  /* Send a DMS request to MMI */
		  else
		  {
			  if (ICF_CALL_CLEAR_REASON_RESOURCES_NOT_AVAILABLE != 
				  p_cc_pdb->p_call_ctx->release_cause)
			  {
			  /* In case GCL fails, MMI will set the release_cause as 
			  * ICF_CALL_CLEAR_MEDIA_MISMATCH. In that case, CC need not send 
				  * the DMS request to MMI */
			
				  ret_val = icf_cc_send_dms_req(p_cc_pdb);
                  if(ICF_CC_08_AWT_CALL_TERMINATE_RESP != p_cc_pdb->p_call_ctx->call_state)
                  {
                       ICF_CC_SET_CALL_STATE(p_cc_pdb,ICF_CC_07_AWT_DMS_RESP)
                  }

			  }
			  else
			  {
				  /* Mark MM cleared bit as no need to send any indication to MM */
				  ICF_CC_SET_CALL_CLR_STATUS(p_cc_pdb, 
					  ICF_CALL_CLEAR_STATUS_MM_CLEARED)
					  ICF_CC_TRACE(p_cc_pdb, ICF_TRACE_CALL_STATUS_MM_CLR)
					  ICF_CC_SET_CALL_STATE(p_cc_pdb,ICF_CC_08_AWT_CALL_TERMINATE_RESP)
			  }
		  }
			 
		  if (ICF_SUCCESS == ret_val)
		  {
		  /* 
		  * Send Release Request to SSA in case of incoming call
		  * else mark NW_cleared as INVITE hasn't been sent 
		  * on the network
			  */
			  ICF_CC_TRACE(p_cc_pdb, ICF_TRACE_CALL_DIR)
				  if (ICF_NULL != ICF_CC_CHECK_COMMON_BITMASK(p_cc_pdb,
					  ICF_CALL_DIR))
				  {
					  /* Mark PA cleared bit as no need to send any indication to PA */
                      if(ICF_INVALID_CALL_ID == p_cc_pdb->p_call_ctx->replace_initiated_call_id)
                      {
                          /* We will not set the following bit in case of tunnel mode.In tunnel mode, 
                             the application has already been given incoming call ind, thus we
                             have to send term call ind to the application through the function
                             icf_cc_07_clear_interfaces.When MMI sends DMS response, the function
                             icf_cc_07_clear_interfaces sends term call ind to the application only
                             if ICF_CALL_CLEAR_STATUS_PA_CLEARED bit is not set.
                          */   
ICF_CHECK_TUNNEL_MODE_START(p_cc_pdb->p_glb_pdb,ICF_FALSE)                          
                          ICF_CC_SET_CALL_CLR_STATUS(p_cc_pdb,
                              ICF_CALL_CLEAR_STATUS_PA_CLEARED)
ICF_CHECK_TUNNEL_MODE_END
                      }
                      else
                      {
                          ICF_CC_RESET_CALL_CLR_STATUS(p_cc_pdb,
                              ICF_CALL_CLEAR_STATUS_NW_CLEARED)
                     
                      }
					  ret_val = icf_cc_send_clear_req_to_ssa(p_cc_pdb);
                    if(ICF_INVALID_CALL_ID != p_cc_pdb->p_call_ctx->replace_initiated_call_id)
                    {
                      icf_call_ctx_st *p_old_context = ICF_NULL;
                      icf_call_ctx_st *p_replace_initiated_call_ctx = p_cc_pdb->p_call_ctx;
                      /* Fetch Call Context of the replace_initiated_call*/
                      /* update line_id,mode from old to new */
                      if(ICF_SUCCESS == icf_dbm_find_call_cntxt_blk(
                          p_cc_pdb->p_glb_pdb,
                          p_cc_pdb->p_call_ctx->replace_initiated_call_id,
                          &p_old_context,
                          p_cc_pdb->p_ecode))
                      {
                          p_cc_pdb->p_call_ctx = p_old_context;
                          p_cc_pdb->p_glb_pdb->p_call_ctx = p_old_context;

                          if(ICF_SUCCESS == icf_cc_send_pa_call_term_ind(p_cc_pdb))
                          {
                              ICF_CC_SET_CALL_STATE(p_cc_pdb,ICF_CC_08_AWT_CALL_TERMINATE_RESP)
                              ICF_CC_RESET_CALL_CLR_STATUS(p_cc_pdb,
                                    ICF_CALL_CLEAR_STATUS_NW_CLEARED)
                              ICF_CC_RESET_CALL_CLR_STATUS(p_cc_pdb,
                                    ICF_CALL_CLEAR_STATUS_MM_CLEARED)
                          }
                      }

                       p_cc_pdb->p_call_ctx = p_replace_initiated_call_ctx;
                       p_cc_pdb->p_glb_pdb->p_call_ctx = p_replace_initiated_call_ctx;
                      
                    }
				  }
				  else 
				  {
					  if (ICF_CC_07_AWT_DMS_RESP != p_cc_pdb->p_call_ctx->call_state)
					  {
						  if(ICF_SUCCESS == icf_cc_send_pa_call_term_ind(p_cc_pdb))
						  {
							  ICF_CC_SET_CALL_STATE(p_cc_pdb,ICF_CC_08_AWT_CALL_TERMINATE_RESP)
								  ret_val = icf_cc_nw_clear_handler(p_cc_pdb);  
						  }
						  else
						  {
							  ret_val = ICF_FAILURE;
						  }
					  }
					  else
					  {
						  ret_val = icf_cc_nw_clear_handler(p_cc_pdb);  
					  }
				  }
		  } /* else */
			 
		/* Update the states in the call context */
		if ((ICF_SUCCESS == ret_val) && 
			(ICF_NULL == ICF_CC_CHECK_CALL_CLR_STATUS(
							p_cc_pdb, ICF_CALL_CLEAR_STATUS_MM_CLEARED)))
		{
			ICF_CC_SET_CALL_STATE(p_cc_pdb,ICF_CC_07_AWT_DMS_RESP)
		}
	 }

    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */
    return ret_val;
}        


/******************************************************************************
 *
 * FUNCTION:        icf_cc_01_timer_expiry
 *
 * DESCRIPTION:     This function is invoked when the CC is awaiting
 *                  create media session resp from MMI and 
 *                  the setup timer expires.
 *                  Current State: ICF_CC_01_AWT_CMS_RESP
 *                  Current Event: ICF_CC_TIMER_EXPIRY
 *
 *****************************************************************************/

icf_return_t icf_cc_01_timer_expiry (
        INOUT icf_cc_pdb_st *p_cc_pdb)
{
    icf_return_t     ret_val = ICF_SUCCESS;

    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /* function entry trace */
    /*
     * Set release type in call context as TIMER EXPIRY
     */
    ICF_CC_TRACE(p_cc_pdb, ICF_TRACE_REL_TYPE_TIMER_INIT)
    ICF_CC_SET_RELEASE_TYPE(p_cc_pdb,ICF_REL_TYPE_TIMER_EXPIRY)

    ICF_CC_TRACE(p_cc_pdb, ICF_TRACE_CALL_DIR)
    
    /*
     * Send a forceful DMS request to MMI
     */
    p_cc_pdb->forceful_delete = ICF_FALSE;
    /*
     * Start the release timer
     */
    if (ICF_FAILURE == icf_cc_start_release_timer(p_cc_pdb))
    {
        ret_val = ICF_FAILURE;
    }
    else  if (ICF_FAILURE == icf_cc_send_dms_req(p_cc_pdb))
    {
        ret_val = ICF_FAILURE;
    }
    /* 
     * Send Release to SSA if incoming call
     * else call NW clear handler
     */
    else if (ICF_NULL != ICF_CC_CHECK_COMMON_BITMASK(p_cc_pdb,
                ICF_CALL_DIR))
    {
        ICF_CC_SET_RELEASE_CAUSE(p_cc_pdb,ICF_CALL_CLEAR_REASON_NOANSWER)               
        /* Mark PA cleared bit as no need to send any indication to PA */
        /* We will not set the following bit in case of tunnel mode.In tunnel mode, 
           the application has already been given incoming call ind, thus we
           have to send term call ind to the application.
        */   
ICF_CHECK_TUNNEL_MODE_START(p_cc_pdb->p_glb_pdb,ICF_FALSE)                          
            ICF_CC_SET_CALL_CLR_STATUS(p_cc_pdb,
                    ICF_CALL_CLEAR_STATUS_PA_CLEARED)
ICF_CHECK_TUNNEL_MODE_END

            ret_val = icf_cc_send_clear_req_to_ssa(p_cc_pdb);
    }
    else
    {
        ret_val = icf_cc_nw_clear_handler(p_cc_pdb); 
    }
    /*
     * Update the states in the call context
     */
    if (ICF_SUCCESS == ret_val)
    {
        ICF_CC_SET_CALL_STATE(p_cc_pdb,ICF_CC_07_AWT_DMS_RESP)
    }

    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */
    return ret_val;
}


/******************************************************************************
 *
 * FUNCTION:        icf_cc_01_ssa_call_cancel_req
 *
 * DESCRIPTION:     This function is invoked when the CC is awaiting
 *                  create media session resp from MMI and 
 *                  it receives a Call Cancel Request from SSA
 *                  (SIP CANCEL received from peer).
 *                  Current State: ICF_CC_01_AWT_CMS_RESP
 *                  Current Event: ICF_CC_SSA_CALL_CANCEL_REQ
 *
 *****************************************************************************/

icf_return_t icf_cc_01_ssa_call_cancel_req(
        INOUT icf_cc_pdb_st *p_cc_pdb)
{
    icf_return_t     ret_val = ICF_SUCCESS;

    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /* function entry trace */
        
    /*
     * Set release type in call context as network initiated
     */
    ICF_CC_TRACE(p_cc_pdb, ICF_TRACE_REL_TYPE_NW_INIT)
    ICF_CC_SET_RELEASE_TYPE(p_cc_pdb,ICF_REL_TYPE_NW_INITIATED) 

    /* Mark PA cleared bit as no need to send any indication to PA */
    ICF_CC_SET_CALL_CLR_STATUS(p_cc_pdb,
            ICF_CALL_CLEAR_STATUS_PA_CLEARED)

    /*
     * Fill the release cause as ICF_CALL_CLEAR_REASON_REMOTE_REJECTED
     */
     ICF_CC_SET_RELEASE_CAUSE(p_cc_pdb,
             ICF_CALL_CLEAR_REASON_REMOTE_REJECTED)

    /*
     * Stop the call setup timer and start the release timer
     */
    if (ICF_FAILURE == icf_cc_start_release_timer(p_cc_pdb))
    {
        ret_val = ICF_FAILURE;
    }
    /* Send a DMS request to MMI */
    else if (ICF_FAILURE == icf_cc_send_dms_req(p_cc_pdb))
    {
        ret_val = ICF_FAILURE;
    }
    /* Send call cancellation response to SSA */
    else if(ICF_FAILURE == icf_cc_send_ssa_cancel_resp(p_cc_pdb))
    {
        ret_val = ICF_FAILURE;
    }
    else
    {
        /*
         * Update the states in the call context
         */
        ICF_CC_SET_CALL_STATE(p_cc_pdb,ICF_CC_07_AWT_DMS_RESP)
        ICF_CC_SET_CALL_CLR_STATUS(p_cc_pdb,
            ICF_CALL_CLEAR_STATUS_NW_CLEARED)
    }

    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */
    return ret_val;
}




/******************************************************************************
 *
 * FUNCTION:        icf_cc_01_generic_err
 *
 * DESCRIPTION:     This function is invoked when the CC is awaiting
 *                  create media session resp from MMI and it receives
 *                  an error indication from MMI..
 *                  Current State: ICF_CC_01_AWT_CMS_RESP
 *                  Current Event: ICF_CC_GENERIC_ERROR
 *
 *****************************************************************************/

icf_return_t icf_cc_01_generic_err (
        INOUT icf_cc_pdb_st *p_cc_pdb)
{
    icf_return_t     ret_val = ICF_SUCCESS;

    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /* function entry trace */
    
    /*
     * Set release type in call context as MM INITIATED
     */

    /* Set release type as internal error */
    ICF_CC_SET_RELEASE_TYPE(p_cc_pdb, ICF_REL_TYPE_INTERNAL_INITIATED)
    ICF_CC_TRACE(p_cc_pdb, ICF_TRACE_REL_TYPE_INT_ERR)

    /*
     * Stop the call setup timer and start the release timer
     */
    if (ICF_FAILURE == icf_cc_start_release_timer(p_cc_pdb))
    {
        ret_val = ICF_FAILURE;
    }
    /* Send a DMS request to MMI */
    else if (ICF_FAILURE == icf_cc_send_dms_req(p_cc_pdb))
    {
        ret_val = ICF_FAILURE;
    }
    else
    {
        /* 
         * Send Release Request to SSA in case of incoming call
         * else mark NW_cleared as INVITE hasn't been sent 
         * on the network
         */
        ICF_CC_TRACE(p_cc_pdb, ICF_TRACE_CALL_DIR)
        if (ICF_NULL != ICF_CC_CHECK_COMMON_BITMASK(p_cc_pdb,
                     ICF_CALL_DIR))
        {
            /* Mark PA cleared bit as no need to send any indication to PA */
            ICF_CC_SET_CALL_CLR_STATUS(p_cc_pdb,
                    ICF_CALL_CLEAR_STATUS_PA_CLEARED)
            ret_val = icf_cc_send_clear_req_to_ssa(p_cc_pdb);
        }
        else
        {
            ret_val = icf_cc_nw_clear_handler(p_cc_pdb);
        }
    } /* else */
    /*
     * Update the states in the call context
     */
    if (ICF_SUCCESS == ret_val)
    {
        ICF_CC_SET_CALL_STATE(p_cc_pdb,ICF_CC_07_AWT_DMS_RESP)
    }

    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */
    return ret_val;
}

/******************************************************************************
 *
 * FUNCTION:        icf_cc_01_forceful_call_clear
 *
 * DESCRIPTION:     This function is invoked when Forceful call clear is 
 *                  received in state ICF_CC_01_AWT_CMS_RESP 
 *                  Current State: ICF_CC_01_AWT_CMS_RESP
 *                  Current Event: ICF_CC_FORCEFUL_CLEAR
 *
 *****************************************************************************/
icf_return_t icf_cc_01_forceful_call_clear(
        INOUT icf_cc_pdb_st *p_cc_pdb)
{
    icf_return_t ret_val = ICF_SUCCESS;
    
    /* 
     * Send Release Request to SSA in case of incoming call
     * else mark NW_cleared as INVITE hasn't been sent 
     * on the network
     */
    ICF_CC_TRACE(p_cc_pdb, ICF_TRACE_CALL_DIR)
    if (ICF_NULL != ICF_CC_CHECK_COMMON_BITMASK(p_cc_pdb,
                ICF_CALL_DIR))
    {
        ICF_CC_SET_RELEASE_CAUSE(p_cc_pdb,ICF_REL_CAUSE_SERV_INT_ERR)
        /*
         *  Mark PA cleared bit as no need to send any indication to PA 
         */
        /* We will not set the following bit in case of tunnel mode.In tunnel mode, 
           the application has already been given incoming call ind, thus we
           have to send term call ind to the application.
        */   
ICF_CHECK_TUNNEL_MODE_START(p_cc_pdb->p_glb_pdb,ICF_FALSE)                          
        ICF_CC_SET_CALL_CLR_STATUS(p_cc_pdb,
                ICF_CALL_CLEAR_STATUS_PA_CLEARED)
ICF_CHECK_TUNNEL_MODE_END
        ret_val = icf_cc_send_clear_req_to_ssa(p_cc_pdb);
    }

    /*Handling would be same as that of forceful call clear in atate IDLE*/
    icf_cc_00_clear_call (p_cc_pdb);

    return ret_val;

}


/******************************************************************************
 *
 * FUNCTION:        icf_cc_01_dms_resp
 *
 * DESCRIPTION:     This function is invoked when the CC has send DMS request 
 *                  and MM sends REL_RES or CLOSE_CHANNEL_IND and no DMS resp 
 *                  is awaited from Network
 *                  Current State: ICF_CC_01_AWT_CMS_RESP
 *                  Current Event: ICF_CC_DMS_RESP
 *
 *****************************************************************************/

icf_return_t icf_cc_01_dms_resp (
        INOUT icf_cc_pdb_st *p_cc_pdb)
{
    icf_return_t     ret_val = ICF_SUCCESS;

    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /* function entry trace */

    /* Mark MM cleared bit as no need to send any indication to MM 
     * and NW cleared bit as INVITe has not been sent*/
    ICF_CC_SET_CALL_CLR_STATUS(p_cc_pdb, 
					  ICF_CALL_CLEAR_STATUS_MM_CLEARED)
  
   if (ICF_NULL == ICF_CC_CHECK_COMMON_BITMASK(p_cc_pdb,
                ICF_CALL_DIR))
    { 
       /* since it is an outgoing call */
       ICF_CC_SET_CALL_STATE(p_cc_pdb,ICF_CC_08_AWT_CALL_TERMINATE_RESP)
	}

    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */
    return ret_val;
}       

/*twc_update handling*/
/*******************************************************************************
 *
 * FUNCTION:        icf_cc_01_pa_call_modify_req
 *
 * DESCRIPTION:     This function is invoked when the CC is awaiting
 *                  Create Media Session Response and call modify req 
 *                  is recvd from the PA
 *                  Current State: ICF_CC_01_AWT_CMS_RESP
 *                  Current Event: ICF_CC_PA_CALL_MODIFY_REQ
 *
 ******************************************************************************/
icf_return_t icf_cc_01_pa_call_modify_req(INOUT icf_cc_pdb_st *p_cc_pdb)
{
    icf_return_t     ret_val = ICF_SUCCESS;

    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /* function entry trace */

    /* reject the request*/
    p_cc_pdb->p_call_ctx->app_failure_cause = ICF_CAUSE_INVALID_CALL_MODIFY_REQUEST;
    ICF_CC_SET_COMMON_BITMASK(p_cc_pdb, ICF_CALL_MODIFY_RESP_FAIL)

    /* Send failure response for update to user */
    if(ICF_FAILURE == icf_cc_send_pa_call_modify_resp(p_cc_pdb))
    {
        ret_val = ICF_FAILURE;
    }
 
    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */
    return ret_val;
}

/******************************************************************************
 *
 * FUNCTION:        icf_cc_01_pa_alert
 *
 * DESCRIPTION:     This function is invoked when the CC is awaiting
 *                  CMS response from MMI and it receives alert request
 *                  from the application.
 *                  Current State: ICF_CC_01_AWT_CMS_RESP
 *                  Current Event: ICF_CC_PA_ALERT
 *
 *****************************************************************************/
icf_return_t icf_cc_01_pa_alert(
        INOUT icf_cc_pdb_st *p_cc_pdb)
{ 
    icf_return_t                    ret_val = ICF_SUCCESS;
    icf_pa_cc_user_alerted_req_st   *p_user_alerted = ICF_NULL;
    icf_config_data_st              *p_config_data = ICF_NULL;
    icf_boolean_t                   send_alert = ICF_TRUE;

    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /* function entry trace */

    ICF_CHECK_IF_ANY_OF_THREE_VAR_IS_NULL(p_cc_pdb->p_glb_pdb,
                                          p_cc_pdb->p_glb_pdb->p_recv_msg,
                                          p_cc_pdb->p_glb_pdb->p_recv_msg->payload,
                                          p_cc_pdb->p_call_ctx,
                                          ret_val)
    if(ICF_FAILURE == ret_val)
    {
        ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */
    	return ret_val;
    }

    /* Retreiving the config data */
    if (ICF_FAILURE == icf_dbm_get_module_glb_data(
                           p_cc_pdb->p_glb_pdb,
                           (icf_module_id_t)(ICF_INT_MODULE_CFG),
                           (icf_void_t *)&p_config_data,
                           p_cc_pdb->p_ecode))
    {
        ICF_PRINT(((icf_uint8_t *)"[CC] : p_config_data is NULL.\n"))
	    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb)
		return ICF_FAILURE;
    }

	p_user_alerted = ((icf_pa_cc_user_alerted_req_st *)
                     p_cc_pdb->p_glb_pdb->p_recv_msg->payload);

ICF_CHECK_TUNNEL_MODE_START(p_cc_pdb->p_glb_pdb,ICF_TRUE)  
    /*  Rel 8.2 changes Start */
    /* If prack field is present in request
     * then check if prack support required then
     * reject the request
     * else send 183
     */
    if (p_user_alerted->bit_mask &  
        ICF_LOCAL_USER_ALERTED_PRACK_VALUE_PRESENT)
    {
	    /*  It is the case when prack value present in
         *  API and it is the first 18x API from application
         */
        if(ICF_PER_CALL_PRACK_FEATURE_ENABLED  != 
           (ICF_CC_CHECK_COMMON_BITMASK_2(p_cc_pdb,
            ICF_PER_CALL_PRACK_FEATURE_ENABLED)))
        {
            /* It is the case when ICF_NO_PRACK is sent by application
             * and Peer send 100rel in require header of INVITE
             * then clear the call.
             */
            if((ICF_NO_PRACK == p_user_alerted->prack_value) &&
               (ICF_REL_PROV_RESP_REQUIRED_BY_PEER == 
               (ICF_CC_CHECK_COMMON_BITMASK_2(p_cc_pdb,
                ICF_REL_PROV_RESP_REQUIRED_BY_PEER))))
            {
                p_cc_pdb->p_call_ctx->release_cause =
                       ICF_CALL_CLEAR_REASON_PRACK_CALL_NOT_ALLOWED;
                ret_val = icf_cc_03_generic_err(p_cc_pdb);
                /*  this reason is set to give in term_call_ind */
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
               ICF_PRACK_IN_REQUIRE  == p_user_alerted->prack_value)
            {
                /*  500 shall be sent on to network */
                p_cc_pdb->p_call_ctx->release_cause = 
                       ICF_CALL_CLEAR_REASON_NON_PRACK_CALL_NOT_ALLOWED;
                ret_val = icf_cc_03_generic_err(p_cc_pdb);
                ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb)
                return ret_val;
            }
            /* If either in require or in supported of incoming INVITE if
             * 100rel is present then set this boolean so that this request 
             * can be rejected because at this point of time IPTK can not
             * send reliable 180
            */
            else if(ICF_PRACK_IN_REQUIRE == p_user_alerted->prack_value)
            {
                send_alert = ICF_FALSE;
            }
        }
        /* It is the case when this media connect_req is not the first
         * 18x API from application and still contains prack_value
         * the actual prack value will be present in prack_reqd
         */
        else if(ICF_TRUE == p_cc_pdb->prack_reqd)        
        {
            send_alert = ICF_FALSE;
        }
    }
    /*  When the prack_value is not received and 
     *  it is the first 18x then API
     */
    else if(ICF_PER_CALL_PRACK_FEATURE_ENABLED  !=
           (ICF_CC_CHECK_COMMON_BITMASK_2(p_cc_pdb,
            ICF_PER_CALL_PRACK_FEATURE_ENABLED)))
    {
         /* SPR 19754 : In case system level flag is OFF and 
         * invite is received with 100rel in require then
         * establish a PRACK call irrespective of user
         * configuration. But in the state of 01 , since CMS
         * response is awaited therefore alert can not be sent
         * to remote therefore the alert API will be rejected 
         * with unexpected message indication.
         */
        
        /* It is the case when PRACK values not received in first 18x
         * API and 
         * 100rel is not received in require header
         * and prack required is true at system level
         * then reject the API
         */
        if(ICF_TRUE == p_cc_pdb->prack_reqd)
        {
            send_alert = ICF_FALSE;
        }
    }
    /*  It is the case when prack_value is not received 
     *  in this API and it is not the first 18x API then
     *  actual prack value will be present in prack_reqd
     */
    else if(ICF_TRUE == p_cc_pdb->prack_reqd)
    {
        send_alert = ICF_FALSE;
    }

    if(ICF_FALSE == send_alert)
    {
        ret_val = icf_cc_send_pa_unexpected_msg_ind(p_cc_pdb);
    }
    /* Rel 8.2 changes End */

    /* Send 180 without SDP ignoring the SDP related fields if present in the request */
    else
    {
        /*  REl 8.2 changes Start */
        /* If PRACK Field came in the API then check in call ctx that is
         * already populated or not if already populated then ignore
         * it because then it will not be the first 18x request.
         * else copy it in call ctx and set the common bitmask and 
         * update the prack_reqd field in cc pdb, because all the 
         * decisions were taken for the prack on this field of cc 
         * pdb
         */
        if(ICF_LOCAL_USER_ALERTED_PRACK_VALUE_PRESENT & 
             p_user_alerted->bit_mask)
        { 
            if(ICF_PER_CALL_PRACK_FEATURE_ENABLED  != 
               (ICF_CC_CHECK_COMMON_BITMASK_2(p_cc_pdb,
                ICF_PER_CALL_PRACK_FEATURE_ENABLED)))
            {
                p_cc_pdb->p_call_ctx-> prack_value =  
                                            p_user_alerted -> prack_value;
                ICF_CC_SET_COMMON_BITMASK_2(p_cc_pdb,   
                  ICF_PER_CALL_PRACK_FEATURE_ENABLED)
                p_cc_pdb->prack_reqd = ICF_FALSE;
            }
        }
        else if(ICF_PER_CALL_PRACK_FEATURE_ENABLED != 
           (ICF_CC_CHECK_COMMON_BITMASK_2(p_cc_pdb,
            ICF_PER_CALL_PRACK_FEATURE_ENABLED)))
        {
            p_cc_pdb->p_call_ctx->system_level_configuration_for_prack = ICF_TRUE;
        }
        /*  Rel 8.2 changes End */

        /* We will not read the fields early_media_reqd or inband_info_status as in 
           this state, we cannot send a request with SDP.
         */   
        if(ICF_USER_ALERTED_ALERT_TIMER_PRESENT & 
              p_user_alerted->bit_mask)
        {
            /* Stop the call setup timer and start the alert timer */
            if (ICF_FAILURE == icf_cc_start_alert_timer(p_cc_pdb, ICF_TRUE, 
                               p_user_alerted->alert_timer_duration))
            {
				ret_val = ICF_FAILURE;
				ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb)
                return ret_val;
            }
        }
        else
        {
            /* Stop the call setup timer and start the alert timer */
            if (ICF_FAILURE == icf_cc_start_alert_timer(p_cc_pdb, ICF_FALSE, ICF_NULL))
            {
				ret_val = ICF_FAILURE;
				ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb)
                return ret_val;
            }
        }

		/* Check if privacy header needs to be sent */
	    if (ICF_PRIVACY_IND_PRESENT & p_user_alerted->bit_mask)
	    {
		    p_cc_pdb->p_call_ctx->send_privacy_hdr = p_user_alerted->privacy_ind;
	    }
        
        /* SPR 19590 : code removed for cloning header/tag/body list from 
         * here as this shall be taken care in icf_cc_convert_api_to_event
         * function.
         */

		/* Send 180 without SDP to the peer. Do not change the call state. */
		if(ICF_FAILURE == icf_cc_send_ssa_alert(p_cc_pdb))
		{
			 ret_val = ICF_FAILURE;
		}
	}
ICF_CHECK_TUNNEL_MODE_END

ICF_CHECK_TUNNEL_MODE_START(p_cc_pdb->p_glb_pdb,ICF_FALSE)  

    /* In non-tunnel mode, if the application gives ICF_LOCAL_USER_ALERTED_REQ
       while we are in ICF_CC_01_AWT_CMS_RESP state, we will reject the request 
	   with ICF_UNEXPECTED_MSG_IND.
    */  
    ret_val = icf_cc_send_pa_unexpected_msg_ind(p_cc_pdb);
ICF_CHECK_TUNNEL_MODE_END

	ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */
	return ret_val;
}

/******************************************************************************
 *
 * FUNCTION:        icf_cc_01_pa_media_connect_req
 *
 * DESCRIPTION:     This function is invoked when the CC is awaiting
 *                  CMS response from MMI and it receives media connect request
 *                  from the application.
 *                  Current State: ICF_CC_01_AWT_CMS_RESP
 *                  Current Event: ICF_CC_PA_MEDIA_CONNECT
 *
 *****************************************************************************/
icf_return_t icf_cc_01_pa_media_connect_req(
        INOUT icf_cc_pdb_st *p_cc_pdb)
{
    icf_return_t                    ret_val = ICF_SUCCESS;
    icf_pa_cc_media_connect_req_st	*p_media_connect = ICF_NULL;
    icf_config_data_st              *p_config_data = ICF_NULL;
    icf_boolean_t                   send_media_connect = ICF_TRUE;

    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /* function entry trace */

    ICF_CHECK_IF_ANY_OF_THREE_VAR_IS_NULL(p_cc_pdb->p_glb_pdb,
                                          p_cc_pdb->p_glb_pdb->p_recv_msg,
                                          p_cc_pdb->p_glb_pdb->p_recv_msg->payload,
                                          p_cc_pdb->p_call_ctx,
                                          ret_val)
    if(ICF_FAILURE == ret_val)
    {
        ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */
    	return ret_val;
    }

    /* Retreiving the config data */
    if (ICF_FAILURE == icf_dbm_get_module_glb_data(
                           p_cc_pdb->p_glb_pdb,
                           (icf_module_id_t)(ICF_INT_MODULE_CFG),
                           (icf_void_t *)&p_config_data,
                           p_cc_pdb->p_ecode))
    {
        ICF_PRINT(((icf_uint8_t *)"[CC] : p_config_data is NULL.\n"))
	    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb)
		return ICF_FAILURE;
    }
  
    p_media_connect = ((icf_pa_cc_media_connect_req_st *)
                p_cc_pdb->p_glb_pdb->p_recv_msg->payload);
   
ICF_CHECK_TUNNEL_MODE_START(p_cc_pdb->p_glb_pdb,ICF_TRUE) 
    /*  Rel 8.2 Changes start */

    /* If prack field is present in request
     * then check if prack support required then
     * reject the request
     * else send 183
     */
    if (p_media_connect->bit_mask &  
        ICF_MEDIA_CONNECT_REQ_PRACK_VALUE_PRESENT)
    {
        /*  It is the case when prack value present in
         *  API and it is the first 18x API from application
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
            /* If either in require or in supported of incoming INVITE if
             * 100rel is present then set this boolean so that this request 
             * can be rejected because at this point of time IPTK can not
             * send reliable 183
            */
            else if(ICF_PRACK_IN_REQUIRE == p_media_connect->prack_value)
            {
                send_media_connect = ICF_FALSE;
            }
        }
        /* It is the case when this media connect_req is not the first
         * 18x API from application and still contains prack_value
         * the actual prack value will be present in prack_reqd
         */
        else if(ICF_TRUE == p_cc_pdb->prack_reqd)        
        {
            send_media_connect = ICF_FALSE;
        }
    }
    /*  When the prack_value is not received and 
     *  it is the first 18x then API
     */
     
    else if(ICF_PER_CALL_PRACK_FEATURE_ENABLED  !=
           (ICF_CC_CHECK_COMMON_BITMASK_2(p_cc_pdb,
            ICF_PER_CALL_PRACK_FEATURE_ENABLED)))
    {
        /* SPR 19754 : In case system level flag is OFF and 
         * invite is received with 100rel in require then
         * establish a PRACK call irrespective of user
         * configuration. But in the state of 01 , since CMS
         * response is awaited therefore alert can not be sent
         * to remote therefore the alert API will be rejected 
         * with unexpected message indication.
         */

        /* It is the case when PRACK values not received in first 18x
         * API and 
         * 100rel is not received in require header
         * and prack required is true at system level
         * then reject the API
         */
        if(ICF_TRUE == p_cc_pdb->prack_reqd)
        {
            send_media_connect = ICF_FALSE;
        }
    }
    /*  It is the case when prack_value is not received 
     *  in this API and it is not the first 18x API then
     *  actual prack value will be present in prack_reqd
     */
    else if(ICF_TRUE == p_cc_pdb->prack_reqd)
    {
        send_media_connect = ICF_FALSE;
    }

    if(ICF_FALSE == send_media_connect)
    {
        ret_val = icf_cc_send_pa_unexpected_msg_ind(p_cc_pdb);
    }
    /*  Rel 8.2 changes end */
    /* Send 183 without SDP ignoring the SDP related fields if present in the request */
    else
    {
        /*  Rel 8.2 changes start */
        /* If PRACK Field came in the API then check in call ctx that is
         * already populated or not if already populated then ignore
         * it because then it will not be the first 18x request.
         * else copy it in call ctx and set the common bitmask and 
         * update the prack_reqd field in cc pdb, because all the 
         * decisions were taken for the prack on this field of cc 
         * pdb
         */
        if(ICF_MEDIA_CONNECT_REQ_PRACK_VALUE_PRESENT & 
             p_media_connect->bit_mask)
        { 
            if(ICF_PER_CALL_PRACK_FEATURE_ENABLED  != 
               (ICF_CC_CHECK_COMMON_BITMASK_2(p_cc_pdb,
                ICF_PER_CALL_PRACK_FEATURE_ENABLED)))
            {
                p_cc_pdb->p_call_ctx->prack_value =  
                                            p_media_connect-> prack_value;
                ICF_CC_SET_COMMON_BITMASK_2(p_cc_pdb,   
                  ICF_PER_CALL_PRACK_FEATURE_ENABLED)
                p_cc_pdb->prack_reqd = ICF_FALSE;
            }
        }
        else if(ICF_PER_CALL_PRACK_FEATURE_ENABLED != 
           (ICF_CC_CHECK_COMMON_BITMASK_2(p_cc_pdb,
            ICF_PER_CALL_PRACK_FEATURE_ENABLED)))
        {
            p_cc_pdb->p_call_ctx->system_level_configuration_for_prack = ICF_TRUE;
        }
        /*  Rel 8.2 changes end */
        /* We will not read the fields send_without_sdp and media_mode as in 
           this state, we cannot send a request with SDP.
         */
    	
		/* Check if privacy header needs to be sent */
        if (ICF_PRIVACY_IND_PRESENT & p_media_connect->bit_mask)
        {
            p_cc_pdb->p_call_ctx->send_privacy_hdr = p_media_connect->privacy_ind;
        }

        /* SPR 19590 : code removed for cloning header/tag/body list from 
         * here as this shall be taken care in icf_cc_convert_api_to_event
         * function.
         */

	/* Send 183 without SDP to the peer.Do not change the call state. */
        if (ICF_FAILURE == icf_cc_send_ssa_media_param_wo_sdp(p_cc_pdb))
        {
            ret_val = ICF_FAILURE;
        }
    }
ICF_CHECK_TUNNEL_MODE_END    

ICF_CHECK_TUNNEL_MODE_START(p_cc_pdb->p_glb_pdb,ICF_FALSE)  

    /* In non-tunnel mode, if the application gives ICF_MEDIA_CONNECT_REQ
       while we are in ICF_CC_01_AWT_CMS_RESP state, we will reject the request 
	   with ICF_UNEXPECTED_MSG_IND.
    */  
    ret_val = icf_cc_send_pa_unexpected_msg_ind(p_cc_pdb);
ICF_CHECK_TUNNEL_MODE_END    
    
	ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */
	return ret_val;
}
