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
 * 13-Jul-2009  Anurag Khare    CSR 1-7505190   Changes done in function
 *                                              icf_cc_01_create_media_resp
 * Copyright (c) 2009, Flextronics Software Systems Ltd.
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
		   /* Fix for CSR 1-7505190 
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

                               icf_cc_set_local_sdp_media_mode_from_rem_mode(p_cc_pdb);
				/*
				 * For a fast start call set media commited to TRUE
				 */
				if((ICF_NULL != ICF_CC_CHECK_COMMON_BITMASK(p_cc_pdb,
								   ICF_CALL_MEDIA_SETUP_TYPE)))
				{
					/* Set media commited to true */
					p_cc_pdb->p_call_ctx->media_committed = ICF_TRUE;

					/* Return status as media commited  to CM */
					ICF_CC_SET_STATUS(p_cc_pdb, ICF_STATUS_MEDIA_COMMITTED)

				   /* Set trace for media commited*/
					ICF_CC_TRACE(p_cc_pdb, ICF_TRACE_MEDIA_COMMITTED) 
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

				/* Initilaize the local SDP with default Qos values */
				icf_cc_fill_local_sdp_with_default_qos_values(p_cc_pdb);

				/* Set current_status values in local send/recv of local_sdp to
				 * ICF_RESOURCE_RESERVED */
				icf_cc_fill_local_sdp_with_curr_values_inc_call(p_cc_pdb);
                
				/* Now we will be updating the Qos parameters in local SDP
				 * structure by comparing it with Qos parameters in remote
				 * received SDP
				 */
				icf_cc_compare_and_update_qos_params(p_cc_pdb);
                                
            
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
					else
					{
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
					}
				}
ICF_CHECK_QOS_SUPPORT_END
#endif
			}
#ifdef ICF_QOS_SUPPORT
ICF_CHECK_QOS_SUPPORT_START(p_cc_pdb->p_glb_pdb,ICF_FALSE)
			if (ICF_INVALID_CALL_ID == p_cc_pdb->p_call_ctx->replace_initiated_call_id)
            {
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

				if(ICF_TRUE == p_cc_pdb->p_call_ctx->media_committed &&
						ICF_NULL != ICF_CC_CHECK_PENDING_EVENT(p_cc_pdb,
							ICF_PENDING_EVENT_PA_CALL_CONNECT))
				{
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
			/* Fix for CSR 1-6727350: SPR 19316 */
			/* Check whether APP Connect is required for Replace-initiated call.
			 * If it is required, send incoming_call_ind to APP and proceed as normal
			 * incoming call, else directly send Connect to SSA.
			 */
            else if ((ICF_TRUE == p_cc_pdb->p_config_data->app_connect_reqd_for_replaces) &&
					 (ICF_INVALID_CALL_ID != p_cc_pdb->p_call_ctx->replace_initiated_call_id))
			{
	            icf_call_ctx_st 	*p_old_context = ICF_NULL;
                icf_error_t       	ecode = 0;
            	icf_call_ctx_st 	*p_replace_initiated_call_ctx = p_cc_pdb->p_call_ctx;
                          
				if(ICF_SUCCESS == icf_dbm_find_call_cntxt_blk(
                              p_cc_pdb->p_glb_pdb,
                              p_cc_pdb->p_call_ctx->replace_initiated_call_id,
                              &p_old_context,
                              &ecode))
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
						  icf_error_t       ecode = 0;
						  icf_call_ctx_st *p_replace_initiated_call_ctx = p_cc_pdb->p_call_ctx;
						  /* Fetch Call Context of the replace_initiated_call*/
						  /* update line_id,mode from old to new */
						  if(ICF_SUCCESS == icf_dbm_find_call_cntxt_blk(
							  p_cc_pdb->p_glb_pdb,
							  p_cc_pdb->p_call_ctx->replace_initiated_call_id,
							  &p_old_context,
							  &ecode))
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
ICF_CHECK_QOS_SUPPORT_END
#else
			if (ICF_INVALID_CALL_ID == p_cc_pdb->p_call_ctx->replace_initiated_call_id)
			{
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

				if(ICF_TRUE == p_cc_pdb->p_call_ctx->media_committed &&
						ICF_NULL != ICF_CC_CHECK_PENDING_EVENT(p_cc_pdb,
							ICF_PENDING_EVENT_PA_CALL_CONNECT))
				{
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
            /* Fix for CSR 1-6727350: SPR 19316 */
            /* Check whether APP Connect is required for Replace-initiated call.
             * If it is required, send incoming_call_ind to APP and proceed as normal
             * incoming call, else directly send Connect to SSA. 
             */
            else if ((ICF_TRUE == p_cc_pdb->p_config_data->app_connect_reqd_for_replaces) &&
                     (ICF_INVALID_CALL_ID != p_cc_pdb->p_call_ctx->replace_initiated_call_id))
            {
                icf_call_ctx_st     *p_old_context = ICF_NULL;
                icf_error_t         ecode = 0;
                icf_call_ctx_st     *p_replace_initiated_call_ctx = p_cc_pdb->p_call_ctx;
        
                if(ICF_SUCCESS == icf_dbm_find_call_cntxt_blk(
                              p_cc_pdb->p_glb_pdb,
                              p_cc_pdb->p_call_ctx->replace_initiated_call_id,
                              &p_old_context,
                              &ecode))
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
						  icf_error_t       ecode = 0;
						  icf_call_ctx_st *p_replace_initiated_call_ctx = p_cc_pdb->p_call_ctx;
						  /* Fetch Call Context of the replace_initiated_call*/
						  /* update line_id,mode from old to new */
						  if(ICF_SUCCESS == icf_dbm_find_call_cntxt_blk(
							  p_cc_pdb->p_glb_pdb,
							  p_cc_pdb->p_call_ctx->replace_initiated_call_id,
							  &p_old_context,
							  &ecode))
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
#endif
		}/*End of if (ICF_CC_STREAM_POS_VALIDATION_FA */
	}/* end of if for incomimg call processing */
    else
    { 
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
				icf_cc_set_local_sdp_media_mode(p_cc_pdb, 
					ICF_MEDIA_MODE_SEND_RECV);
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
                          ICF_CC_SET_CALL_CLR_STATUS(p_cc_pdb,
                              ICF_CALL_CLEAR_STATUS_PA_CLEARED)
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
                      icf_error_t       ecode = 0;
                      icf_call_ctx_st *p_replace_initiated_call_ctx = p_cc_pdb->p_call_ctx;
                      /* Fetch Call Context of the replace_initiated_call*/
                      /* update line_id,mode from old to new */
                      if(ICF_SUCCESS == icf_dbm_find_call_cntxt_blk(
                          p_cc_pdb->p_glb_pdb,
                          p_cc_pdb->p_call_ctx->replace_initiated_call_id,
                          &p_old_context,
                          &ecode))
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
            ICF_CC_SET_CALL_CLR_STATUS(p_cc_pdb,
                    ICF_CALL_CLEAR_STATUS_PA_CLEARED)

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
        ICF_CC_SET_CALL_CLR_STATUS(p_cc_pdb,
                ICF_CALL_CLEAR_STATUS_PA_CLEARED)
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
 
