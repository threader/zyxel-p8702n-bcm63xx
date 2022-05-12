/*------------------------------------------------------------------------------
 *
 * File name        : icf_cc_fsm_06.c
 *
 * Purpose          : This file contains the event handlers in
 *                    ICF_CC_06_CONNECTED state.
 *
 * Revision History :
 *
 * Date         Name            Ref#            Comments
 * --------     ------------    ----            ---------------
 * 10-Dec-2004  Umang Singh     Non Service LLD Initial
 * 21-Dec-2004  Rohit Aggarwal                  Addition of traces, stats
 * 24-Dec-2004  Rohit Aggarwal                  Compilation fixes
 * 20-Apr-2005  Sumit Gupta      SPR 7397       CC would send call terminate in 
 *                                              case of generic error or MM 
 *                                              error
 * 06-May-2005 Sumit Gupta       SPR 7411       In case of initiating call clear
 *                                              from local end and 
 *                                              signaling ongoing set by 
 *                                              other module send status as 
 *                                              call_n_service_terminate to CM 
 *                                              and set state as WAIT_PROC_SIG
 * 06-May-2005 Sumit Gupta      SPR 7489        Added call clear reason for NW 
 *                                              call clear
 * 10-Jun-2005 Sumit Gupta      SPR 7649        Set ongoing event MUTE/UNMUTE 
 *                                              when the same are received 
 *                                              before sending MMS request
 * 07-Mar-2006	Anuradha Gupta	None		Added the changes for incoming
 *					        Re-invite handling
 * 08-Mar-2006  Aman Aggarwal			ICF merging with ICF2.0
 * 30-Mar-2006  Anuradha Gupta	ICF 5.0		Call Modify Feature changes
 * 07-Apr-2006	Anuradha Gupta	ICF 5.0		Call Modify changes
 * 17-Apr-2007  Abhishek Dutta  ICF 6.3         Added the changes for 
 *                                              TWC-UPDATE Handling
 * 26-Jun-2006  Amit Sharma     ICF 6.0 APIs    Structure changes for multiple
 *                                              m-lines
 * 31-Jul-2006  Anuradha Gupta  ICF 6.0 LLD     multiple m-lines coding changes
 * 10-Jul-2007  Neha Choraria   ICF 7.0         Changes for Re-Invite w/o SDP
 * 10-Jul-2007  Abhishek Dutta  ICF 7.0         Cancel call modify changes
 * 30-Aug-2007  Amit Sharma     SPR:15974
 * 3-Apr-2008   Sumant Gupta    ICF 8.0         Rel7.1 Windows porting Merging
 * 27-May-2008  Tarun Gupta     SPR 18585       Modified icf_cc_06_ssa_fax_req and
 *                                              icf_cc_06_ssa_offer_change
 * 09-Jun-2008  Tarun Gupta     SPR 18585       Modified icf_cc_06_ssa_stop_fax_req
 * 19-Nov-2008  Rajiv Kumar     SPR 19188       Rel 7.1.2 SPR merging (SPR
 *                                              18917) 
 * 11-Feb-2009 Anurag Khare     ICF Rel 8.1     changes done for Call_modify
 *                                              enhancements
 * 09-Mar-2009  Tarun Gupta     Rel 8.1         Modified icf_cc_06_pa_call_modify_req
 * 18-Mar-2009  Anurag Khare    SPR 19395       Changes doe for SDP Tunnelling
 * 22-Apr-2009  Abhishek Dhammawat SPR 19473    Modified icf_cc_06_pa_call_modify_req
 * 29-Apr-2009  Anuradha Gupta  SPR 19526       Modified icf_cc_06_ssa_session_refresh_resp
 * 18-May-2009  Anuradha Gupta    SPR 19672     CSR_1-6347417 merging
 * 19-May-2009  Kamal Ashraf    SPR 19672       CSR_1_6903010 Merged 
 * 25-May-2009  Anuradha Gupta    SPR 19672     CSR_1-6779001 merging
 * 08-Jun-2009  Kamal Ashraf    SPR 19590       Modified icf_cc_06_ssa_offer_change 
 * 06-Oct-2009  Abhishek Dhammawat SPR 20063    Modified icf_cc_06_ssa_session_refresh_resp
 * 08-Oct-2009  Tarun Gupta     SPR 20077       Modified icf_cc_06_ssa_offer_change
 *
 * Copyright (c) 2009, Aricent.
 *---------------------------------------------------------------------------*/


#include "icf_cc_prototypes.h"
#include "icf_cc_macro.h"
#include "icf_dbm_prototypes.h"
#include "icf_internal_api_id.h"

/******************************************************************************
 *
 * FUNCTION:        icf_cc_06_ssa_call_term_req
 *
 * DESCRIPTION:     This function is invoked when the CC is in CONNECTED state
 *                  and it receives Call Terminate Request from PA.
 *                  Current State: ICF_CC_06_CONNECTED
 *                  Current Event: ICF_CC_SSA_CALL_TERMINATE_REQ
 *
 *****************************************************************************/
icf_return_t icf_cc_06_ssa_call_term_req(
        INOUT icf_cc_pdb_st *p_cc_pdb)
{
    icf_return_t     ret_val = ICF_SUCCESS;

    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /* function entry trace */

        /*
         * Start the release timer
         */
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
                else if(ICF_FAILURE == icf_cc_nw_clear_handler(p_cc_pdb))
                {
                    ret_val = ICF_FAILURE;
                } /* else if(ICF_FAILURE == ipps... */
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
                } /* else */
        }

    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */
        return ret_val;
}

/******************************************************************************
 *
 * FUNCTION:        icf_cc_06_pa_call_term_req
 *
 * DESCRIPTION:     This function is invoked when the CC is in CONNECTED state
 *                  and it receives Call Terminate Request from PA.
 *                  Current State: ICF_CC_06_CONNECTED
 *                  Current Event: ICF_CC_PA_CALL_TERMINATE_REQ
 *
 *****************************************************************************/
icf_return_t icf_cc_06_pa_call_term_req(
        INOUT icf_cc_pdb_st *p_cc_pdb)
{
    icf_return_t     ret_val = ICF_SUCCESS;

    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /* function entry trace */

        /*
         * Start the release timer
         */
        if(ICF_FAILURE == icf_cc_start_release_timer(p_cc_pdb))
        {
            ret_val = ICF_FAILURE;
        } /* if(ICF_FAILURE == icf_st... */
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

    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */
        return ret_val;
}

ICF_HANDLE_REINVITE_START
/*******************************************************************************
 *
 * FUNCTION:        icf_cc_06_ssa_offer_change
 *
 * DESCRIPTION:     This function is invoked when the CC is in CONNECTED state
 *                  and it receives a session param change from SSA
 *                  (SIP UPDATE/RE-INVITE received from peer).
 *                  Current State: ICF_CC_06_CONNECTED
 *                  Current Event: ICF_CC_SSA_OFFER_CHANGE
 *
 ******************************************************************************/
icf_return_t icf_cc_06_ssa_offer_change(
        INOUT icf_cc_pdb_st *p_cc_pdb)
{
    icf_return_t     ret_val = ICF_SUCCESS;
    icf_nw_inc_media_change_req_st *p_offer_chg_req = ICF_NULL;

    p_offer_chg_req =
        (icf_nw_inc_media_change_req_st *)p_cc_pdb->p_internal_msg->p_msg_data;

    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /* function entry trace */
ICF_CHECK_TUNNEL_MODE_START(p_cc_pdb->p_glb_pdb, ICF_FALSE)
        if(ICF_INT_MODULE_INVALID != ICF_CC_GET_SIG_ONGOING( p_cc_pdb))
        {
            if(ICF_NW_INC_MEDIA_CHG_SDP_PRESENT ==
                    (ICF_NW_INC_MEDIA_CHG_SDP_PRESENT & p_offer_chg_req->bitmask))
            {
                icf_cc_delete_inc_media_sdp_info_list(
                        p_cc_pdb,&(p_offer_chg_req->p_sdp_list));
            }
            /*Fix for SPR : 19188 (Rel 7.1.2 SPR merging for SPR 18917)*/
            /* SPR 18917: In case the incoming reinvite has video line wiht port
             * 0 when audio call is established or vice versa then release the
             * memory here before sending error response.
             */ 
            if(ICF_NW_INC_MEDIA_NW_REJECT_STREAM & p_offer_chg_req->bitmask)
            {
                icf_cmn_delete_list(p_cc_pdb->p_glb_pdb,
                        &(p_offer_chg_req->p_nw_reject_stream_list),
                        ICF_MEM_COMMON);
            }
            /* call modify request can not be processed. Send 491 */
            p_cc_pdb->p_call_ctx->offer_failure_reason = ICF_REQUEST_PENDING;
            ret_val = icf_cc_send_ssa_offer_chg_err(p_cc_pdb);
        }
        /* take the backup of all the local SDPs */
        else if(ICF_FAILURE == icf_cc_copy_committed_sdp(p_cc_pdb))
        {
            icf_cc_delete_inc_media_sdp_info_list(
                        p_cc_pdb,&(p_offer_chg_req->p_sdp_list));
            ret_val = ICF_FAILURE;	
        }
        else if(ICF_FAILURE == icf_cc_process_remote_call_modify_offer(p_cc_pdb))
        {
            ret_val = ICF_FAILURE;
        }
        else if(p_cc_pdb->p_call_ctx->offer_failure_reason)
        {
            /*there is some error in processing of remote call modify offer
              so reject the offer and send error response to SSA */
            ret_val = icf_cc_send_ssa_offer_chg_err(p_cc_pdb);
            ICF_CC_RESET_COMMON_BITMASK(p_cc_pdb,ICF_CALL_MOD_REQ_WO_SDP)

            if (ICF_CC_STREAM_POS_VALIDATION_FAIL == 
                    (p_cc_pdb->common_bitmask & ICF_CC_STREAM_POS_VALIDATION_FAIL))
            {
                ret_val = icf_cc_06_generic_err(p_cc_pdb);
            }
        }
        else if(ICF_INT_MODULE_INVALID != ICF_CC_GET_MEDIA_ONGOING(p_cc_pdb))
        {
            /*
             * MMS cannot be sent to nw as request is pending from MM or 
             * signalling is ongoing
             * Mark offer change as pending event
             */
            ICF_CC_SET_PENDING_EVENT(p_cc_pdb,ICF_PENDING_EVENT_OFFER_CHANGE_REQ)

                /* Set call state */
                ICF_CC_SET_CALL_STATE(p_cc_pdb,ICF_CC_13_WAIT_START_PROC_MEDIA)

                ICF_CC_SET_NEXT_STATE(p_cc_pdb,ICF_CC_15_AWT_CALL_MODIFY_ALERT_RESP)
                /* Set the call modify direction flag */
              p_cc_pdb->p_call_ctx->common_bitmask |= ICF_CALL_MODIFY_DIR;	
        }
        else
        {
            ICF_CC_RESET_COMMON_BITMASK(p_cc_pdb,ICF_CALL_MODIFY_DIR)

                /* Set the call modify direction flag */
                p_cc_pdb->p_call_ctx->common_bitmask |= ICF_CALL_MODIFY_DIR;

            /* Set ICF_INT_MODULE_CC in signalling ongoing flag */
            ICF_CC_SET_SIG_ONGOING(p_cc_pdb)

                ICF_CC_SET_ONGOING_EVENT(p_cc_pdb,p_cc_pdb->current_event)

                /******* Changes for Re-Invite w/o SDP Start (Rel7.0) *******/

                /* This part is commented to handling the reinvite without sdp as
                 * a new offer.*/

#if 0
                if(ICF_NW_INC_MEDIA_CHG_SDP_PRESENT != 
                        (ICF_NW_INC_MEDIA_CHG_SDP_PRESENT & p_offer_chg_req->bitmask))
                {
                    p_cc_pdb->pa_call_mod_wo_media = ICF_TRUE;
                    /*Send call_modify_ind to application*/
                    ret_val = icf_cc_send_pa_call_modify_ind(p_cc_pdb);

                    ICF_CC_SET_CALL_STATE(p_cc_pdb,ICF_CC_14_AWT_CALL_MODIFY_RESP)

#ifdef ICF_PATCH_FOR_OFFER_IN_200 
                        icf_cc_handle_non_fs_reinvite(p_cc_pdb);
#endif
                }

#endif
                /******* Changes for Re-Invite w/o SDP End (Rel7.0) *******/

            /*Process the call modify request. Set the appropriate media change 
              types and values in the remote received sdp corresponsding to each 
              stream */

                if(ICF_FAILURE == icf_cc_send_mms_req(p_cc_pdb))
                {
                    ret_val = ICF_FAILURE;
                }
                else if(ICF_CC_ALL_STREAMS_DELETED == 
                        (ICF_CC_ALL_STREAMS_DELETED & p_cc_pdb->common_bitmask))
                {
                    p_cc_pdb->p_call_ctx->offer_failure_reason = ICF_SA_SERVER_INTERNAL_ERROR;

                    /*there is some error in processing of remote call modify offer
                      so reject the offer and send error response to SSA */
                    icf_cc_send_ssa_offer_chg_err(p_cc_pdb);

                    ret_val = icf_cc_06_generic_err(p_cc_pdb);

                }
                else
                {
                    if(ICF_TRUE == p_cc_pdb->mms_sent)
                    {
                        /*
                         * Update the states in the call context
                         */
                        ICF_CC_SET_CALL_STATE(p_cc_pdb,ICF_CC_04_AWT_MMS_RESP)


                            ICF_CC_SET_NEXT_STATE(p_cc_pdb,
                                    ICF_CC_15_AWT_CALL_MODIFY_ALERT_RESP)

                            /* Start the media timer  */
                            ret_val = icf_cc_start_media_timer(p_cc_pdb);
                    }
                    else
                    {
                        /* reset the media updated flag */
                        p_cc_pdb->p_call_ctx->media_updated = ICF_FALSE;
                        p_cc_pdb->is_call_modify_timer = ICF_TRUE;

                        if(ICF_FAILURE == icf_cc_start_setup_timer(p_cc_pdb))
                        {
                            ret_val = ICF_FAILURE;
                        }
                        else if(ICF_FAILURE == icf_cc_send_pa_call_modify_ind(p_cc_pdb))
                        {
                            ret_val = ICF_FAILURE;
                        }
                        else
                        {
                            ICF_CC_SET_CALL_STATE(p_cc_pdb,
                                    ICF_CC_15_AWT_CALL_MODIFY_ALERT_RESP)
                                ICF_CC_SET_NEXT_STATE(p_cc_pdb,ICF_CC_14_AWT_CALL_MODIFY_RESP)            
                        }
                        /* SPR 19590 : code removed for freeing header list from 
                         * here as this shall be taken care when deallocating the
                         * call context.
                         */
                    }
                }/* MMS was handled succesfully */
        }/* MMS can be sent to NW */
ICF_CHECK_TUNNEL_MODE_END
ICF_CHECK_TUNNEL_MODE_START(p_cc_pdb->p_glb_pdb, ICF_TRUE)

        /* SPR 20077 : In case of tunnel mode, the flag signalling 
           ongoing in the call context is a decision point to 
           accept/reject a generic request from the application.
           If it is set, the request is rejected. 
           As a fix of this SPR,setting the same to reject the
           generic request from application in the following scenario:

           <---- Call is connected ---->
           <---- ReINVITE
           <---- 100

           ----> Generic request from application
        */   
        if(ICF_INT_MODULE_INVALID == ICF_CC_GET_SIG_ONGOING( p_cc_pdb))
        {
            ICF_CC_SET_SIG_ONGOING(p_cc_pdb)
        }
        /* End SPR 20077 fix */

        /* Rel 8.1
         * If UPDATE is received without SDP
         * then send the incoming_call_modify
         * ind to application, start the setup
         * timer and move to state 14
         */
       ICF_CC_RESET_COMMON_BITMASK(p_cc_pdb,ICF_CALL_MODIFY_DIR) 
        if((ICF_INC_MEDIA_CHANGE_METHOD_REINVITE != 
              (p_offer_chg_req->bitmask & ICF_INC_MEDIA_CHANGE_METHOD_REINVITE)
            && (p_offer_chg_req->p_sdp_list == ICF_NULL)))

        {
            p_cc_pdb->p_call_ctx->update_recv = ICF_TRUE;
            ICF_CC_SET_COMMON_BITMASK(p_cc_pdb,
                              ICF_CALL_MOD_REQ_WO_SDP)
            if(ICF_FAILURE == icf_cc_start_setup_timer(p_cc_pdb))
            {
                ret_val = ICF_FAILURE;
            }
            else if(ICF_FAILURE == icf_cc_send_pa_call_modify_ind(p_cc_pdb))
            {
                ret_val = ICF_FAILURE;
            }
            else
            {
                ICF_CC_RESET_COMMON_BITMASK(p_cc_pdb,ICF_CALL_MODIFY_DIR)

                /* Set the call modify direction flag */
                p_cc_pdb->p_call_ctx->common_bitmask |= ICF_CALL_MODIFY_DIR;

                ICF_CC_SET_CALL_STATE(p_cc_pdb,
                      ICF_CC_14_AWT_CALL_MODIFY_RESP)
            }
        }
        else
        {
            if((ICF_INC_MEDIA_CHANGE_METHOD_REINVITE == 
              (p_offer_chg_req->bitmask & ICF_INC_MEDIA_CHANGE_METHOD_REINVITE))
            && (p_offer_chg_req->p_sdp_list == ICF_NULL))
            {
                 /*SPR 19395 the below change  is done to resolve the
                  problem whenever update is received without SDP 
                  then IPTK is sending 200 OK with SDP to peer 
                  This was happening because the following boolean 
                  governs the decision to send 200 OK with/without SDP
                  Thus if UPDATE received without SDP then the boolean 
                  update_recv must be set and once 200 OK is  
                  sent then it must get reset */
                p_cc_pdb->p_call_ctx->update_recv = ICF_FALSE;
            }
            /*  take the backup of local sdp */
            if(ICF_FAILURE == icf_cc_copy_committed_sdp(p_cc_pdb))
            {
                icf_cc_delete_inc_media_sdp_info_list(
                        p_cc_pdb,&(p_offer_chg_req->p_sdp_list));
                ret_val = ICF_FAILURE;
            }
            /*  If tunnel mode is ON then
             *  this function basically set the MMS type
             */
            else if(ICF_FAILURE == icf_cc_process_remote_call_modify_offer(p_cc_pdb))
            {
                icf_cc_delete_inc_media_sdp_info_list(
                        p_cc_pdb,&(p_offer_chg_req->p_sdp_list));
                ret_val = ICF_FAILURE;
            }
            else if(ICF_NULL != p_cc_pdb->p_call_ctx->offer_failure_reason)
            {
                /*there is some error in processing of remote call modify offer
                  so reject the offer and send error response to SSA */
                ret_val = icf_cc_send_ssa_offer_chg_err(p_cc_pdb);
                ICF_CC_RESET_COMMON_BITMASK(p_cc_pdb,ICF_CALL_MOD_REQ_WO_SDP)
                icf_cc_delete_inc_media_sdp_info_list(
                        p_cc_pdb,&(p_offer_chg_req->p_sdp_list));
                
            }
            else
            {
                if(p_offer_chg_req->p_sdp_list == ICF_NULL)
                {
                    /* Rel 8.1
                     * Set offer required in cc_pdb;
                     */
                    p_cc_pdb->offer_required = ICF_TRUE;
                    ICF_CC_SET_COMMON_BITMASK(p_cc_pdb,ICF_CALL_MOD_REQ_WO_SDP)
                }
                else
                {
                    if(ICF_INC_MEDIA_CHANGE_METHOD_REINVITE != 
                       (p_offer_chg_req->bitmask & ICF_INC_MEDIA_CHANGE_METHOD_REINVITE))
                    {
                        p_cc_pdb->p_call_ctx->update_recv = ICF_TRUE;
                    }
                }
                if(ICF_FAILURE == icf_cc_send_pa_call_modify_ind(p_cc_pdb))
                {
                    ret_val = ICF_FAILURE;
                }
                else if(ICF_FAILURE == icf_cc_send_mms_req(p_cc_pdb))
                {
                    ret_val = ICF_FAILURE;
                }
                else if(ICF_FAILURE == icf_cc_start_media_timer(p_cc_pdb))
                {
                    ret_val = ICF_FAILURE;
                }
                else
                {
                    ICF_CC_RESET_COMMON_BITMASK(p_cc_pdb,ICF_CALL_MODIFY_DIR)

                    /* Set the call modify direction flag */
                    p_cc_pdb->p_call_ctx->common_bitmask |= ICF_CALL_MODIFY_DIR;

                    ICF_CC_SET_CALL_STATE(p_cc_pdb,ICF_CC_04_AWT_MMS_RESP)
                    ICF_CC_SET_NEXT_STATE(p_cc_pdb,
                               ICF_CC_15_AWT_CALL_MODIFY_ALERT_RESP)

                }
            }
        }
ICF_CHECK_TUNNEL_MODE_END
    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */
        return ret_val;
}

/*******************************************************************************
 *
 * FUNCTION:        icf_cc_06_pa_call_modify_req
 *
 * DESCRIPTION:     This function is invoked when the CC is in CONNECTED state
 *                  and it receives a call modify request from PA
 *                  Current State: ICF_CC_06_CONNECTED
 *                  Current Event: ICF_CC_PA_CALL_MODIFY_REQ
 *
 ******************************************************************************/
icf_return_t icf_cc_06_pa_call_modify_req(
        INOUT icf_cc_pdb_st *p_cc_pdb)
{
    icf_return_t     ret_val = ICF_SUCCESS;
    icf_config_data_st  *p_config_data = ICF_NULL;
    icf_call_modify_req_st *p_call_modify_req = ICF_NULL;
    icf_stream_sdp_info_st *p_stream_sdp_info = ICF_NULL;

    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /* function entry trace */

    /* Get pointer to config data from DBM */
    ret_val = icf_dbm_get_module_glb_data(
                p_cc_pdb->p_glb_pdb,
                (icf_int_module_id_t)ICF_INT_MODULE_CFG,
                (icf_void_t *)&p_config_data, p_cc_pdb->p_ecode);

    ICF_ASSERT(p_cc_pdb->p_glb_pdb, (ICF_NULL != p_config_data));

    if((ICF_INT_MODULE_INVALID != ICF_CC_GET_SIG_ONGOING( p_cc_pdb)) ||
            (ICF_INT_MODULE_INVALID != ICF_CC_GET_MEDIA_ONGOING(p_cc_pdb)))
    {
        /* reject the request*/
        p_cc_pdb->p_call_ctx->app_failure_cause = ICF_CAUSE_CALL_MODIFY_REQ_PENDING;
        ICF_CC_SET_COMMON_BITMASK(p_cc_pdb, ICF_CALL_MODIFY_RESP_FAIL)

        /* Send failure response for update to user */
        if(ICF_FAILURE == icf_cc_send_pa_call_modify_resp(p_cc_pdb))
        {
            ret_val = ICF_FAILURE;
        }
        return ret_val;
    }
    else
    {
        ICF_CC_SET_COMMON_BITMASK(p_cc_pdb,ICF_CALL_MODIFY_DIR)
        ICF_CC_RESET_COMMON_BITMASK(p_cc_pdb,ICF_CALL_MODIFY_DIR)
        ICF_CC_SET_ONGOING_EVENT(p_cc_pdb,p_cc_pdb->current_event)



        /* copy the additional parameters from payload into call ctx */
        if (ICF_FAILURE == icf_cc_proc_pa_media_change_payload(p_cc_pdb))
        {
            ret_val = ICF_FAILURE;	
            return ret_val;
        }
    }
 
    /*twc_update handling*/
     p_call_modify_req = (icf_call_modify_req_st *)
                         p_cc_pdb->p_glb_pdb->p_recv_msg->payload;

ICF_CHECK_TUNNEL_MODE_START(p_cc_pdb->p_glb_pdb, ICF_FALSE)
    /* If either media stream is present or application has deleted media stream 
     * then invoke media apis else there is no need to invoke an media api*/
    if(ICF_CALL_MOD_REQ_MEDIA_MOD_LIST_PRESENT & p_call_modify_req->bit_mask || 
       ICF_CALL_MOD_REQ_DELETED_STREAM_PRESENT & p_call_modify_req->bit_mask)
    {
        /* take the backup of all the local SDPs */
        if(ICF_FAILURE == icf_cc_copy_committed_sdp(p_cc_pdb))
        {
            ret_val = ICF_FAILURE;	
        }
        /*Process the call modify request. Set the appropriate media change 
          types and values in the local sdp corresponsding to each added stream 
         */
        else if(ICF_FAILURE == icf_cc_process_local_call_modify_offer(p_cc_pdb))
        {
            ret_val = ICF_FAILURE;	
        }
        else if(p_cc_pdb->p_call_ctx->app_failure_cause)
        {
            /*there is some error in processing of local call modify offer
              so reject the offer and send response to PA */
            ICF_CC_SET_COMMON_BITMASK(p_cc_pdb, ICF_CALL_MODIFY_RESP_FAIL)
            ret_val = icf_cc_send_pa_call_modify_resp(p_cc_pdb);
            return ret_val;
        }
        /*
         * Send Modify Media Session to the MMI
         * and start the modify media timer.
         */	
        else if(ICF_FAILURE == icf_cc_send_mms_req(p_cc_pdb))
        {
            ret_val = ICF_FAILURE;
        }
        else if(ICF_CC_ALL_STREAMS_DELETED == 
               (ICF_CC_ALL_STREAMS_DELETED & p_cc_pdb->common_bitmask))
        {
            p_cc_pdb->p_call_ctx->app_failure_cause = ICF_ERROR_GENERIC;

            /*there is some error in processing of local call modify offer
              so reject the offer and send response to PA */
            ICF_CC_SET_COMMON_BITMASK(p_cc_pdb, ICF_CALL_MODIFY_RESP_FAIL)

            icf_cc_send_pa_call_modify_resp(p_cc_pdb);

            ret_val = icf_cc_06_generic_err(p_cc_pdb);
        }
        else if(ICF_TRUE == p_cc_pdb->mms_sent)
        {
            /*
             * Update the states in the call context
             */
            ICF_CC_SET_CALL_STATE(p_cc_pdb,ICF_CC_04_AWT_MMS_RESP)

            if(ICF_CFG_CALL_MODIFY_METHOD_INVITE ==
                          p_config_data->call_modify_req_method)
            {
                ICF_CC_SET_NEXT_STATE(p_cc_pdb,
                               ICF_CC_15_AWT_CALL_MODIFY_ALERT_RESP)
            }
            else
            {
                ICF_CC_SET_NEXT_STATE(p_cc_pdb,
                               ICF_CC_14_AWT_CALL_MODIFY_RESP) 
            }

            /* Start the media timer  */
            ret_val = icf_cc_start_media_timer(p_cc_pdb);
        }
    } 
    /*twc_update handling*/
    /* MMS sent would be false in 2 cases 
     * 1) if the media parmeters received from application 
     *    have same media attributes as applied towards the platform
     * 2) No media attributes or deleted stream received from application
     * In both cases sent REINVITE/UPDATE on network immediately
     */
    if(ICF_FALSE == p_cc_pdb->mms_sent && 
       ICF_FAILURE != ret_val)	
    {
        /* reset the media updated flag */
        p_cc_pdb->p_call_ctx->media_updated = ICF_FALSE;

        if(ICF_FAILURE == icf_cc_start_setup_timer(p_cc_pdb))
        {
            ret_val = ICF_FAILURE;
        }
        else if(ICF_FAILURE == icf_cc_ssa_send_update_msg(p_cc_pdb))
        {
            ret_val = ICF_FAILURE;
        }
        /* Fix for CSR 1-6779001: SPR 19258 */
        /* Move to state 15 for ReINVITE and state 14 for UPDATE */
        else if ((ICF_CFG_CALL_MODIFY_METHOD_INVITE ==
                    p_config_data->call_modify_req_method) ||
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
    }/* MMS can be sent to NW */
ICF_CHECK_TUNNEL_MODE_END

ICF_CHECK_TUNNEL_MODE_START(p_cc_pdb->p_glb_pdb, ICF_TRUE)
    if(ICF_FALSE == 
	p_call_modify_req->tunnelled_sdp_present)
    {
          /*SPR 19473 if UPDATE has been received in ALLOW hdr from peer
          then send UPDATE wo SDP or reject the API in case ALLOW header
          is not at all received. If call modify method is set as RE-INV 
           then also UPDATE wo SDP shall be send if UPDATE is recv in ALLOW
           hdr this is done in icf_cc_ssa_send_update_msg*/
        if(ICF_CFG_CALL_MODIFY_METHOD_INVITE ==
            p_config_data->call_modify_req_method)
        {
            /* Rel 8.1
             * reject the request because IPTK can not 
             * send ReInvite without SDP
			 */
            p_cc_pdb->p_call_ctx->app_failure_cause = 
                ICF_CAUSE_INVALID_CALL_MODIFY_REQUEST;
            ICF_CC_SET_COMMON_BITMASK(p_cc_pdb, ICF_CALL_MODIFY_RESP_FAIL)
       
            /* Send failure response for update to user */
            if(ICF_FAILURE == icf_cc_send_pa_call_modify_resp(p_cc_pdb))
            {
                ret_val = ICF_FAILURE;
            }
        }
        else if(ICF_UPDATE_PRESENT_IN_ALLOW_HDR & 
                p_cc_pdb->p_call_ctx->allowed_value)
        {
            /* Set common_bitmask as UPDATE_WO_SDP to send UPDATE 
               without SDP to the peer. SSA uses this bit
               to determine whether to send UPDATE with/without
               SDP.
            */
            ICF_CC_SET_COMMON_BITMASK(p_cc_pdb, ICF_CALL_MOD_REQ_WO_SDP)

            /*  Trigger SSA to send UPDATE */
            if (ICF_FAILURE == icf_cc_ssa_send_update_msg(p_cc_pdb))
            {
                ret_val = ICF_FAILURE;
            }
            else
            {
                /*  Change the stae as the timer exipry will not be
                 *  handled in 06 state */
                if(ICF_CFG_CALL_MODIFY_METHOD_INVITE ==
                        p_config_data->call_modify_req_method)
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
        }
        else
        {
            /* reject the request*/
            p_cc_pdb->p_call_ctx->app_failure_cause = 
                ICF_CAUSE_INVALID_CALL_MODIFY_REQUEST;
            ICF_CC_SET_COMMON_BITMASK(p_cc_pdb, ICF_CALL_MODIFY_RESP_FAIL)
            /* Send failure response for update to user */
            if(ICF_FAILURE == icf_cc_send_pa_call_modify_resp(p_cc_pdb))
            {
               ret_val = ICF_FAILURE;
            }
        }
    }
    /* REl 8.1
     * If call_modify_req received with SDP from application
     */
    else
    {
        /*  Rel 8.1
         * Take the backup of local sdp
         * set the media modify type to 
         * ICF_LOCAL_SDP_REQD because SDP is 
         * required bu CC to send in 
         * ReInvite/UPATE
         * and do the MMS to MMI
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
                ret_val = icf_cc_start_media_timer(p_cc_pdb);
                ICF_CC_SET_ONGOING_EVENT(p_cc_pdb,p_cc_pdb->current_event)
                ICF_CC_SET_CALL_STATE(p_cc_pdb,ICF_CC_04_AWT_MMS_RESP)

                /* Rel 8.1
                 * If reInvite would be sent on to network
                 * then set next state as 15
                 * else set next state as 14
                 */
                if(ICF_CFG_CALL_MODIFY_METHOD_INVITE ==
                        p_config_data->call_modify_req_method)
                {
                    ICF_CC_SET_NEXT_STATE(p_cc_pdb,
                               ICF_CC_15_AWT_CALL_MODIFY_ALERT_RESP)
                }
                else
                {
                    ICF_CC_SET_NEXT_STATE(p_cc_pdb,
                               ICF_CC_14_AWT_CALL_MODIFY_RESP) 
                }
            }
        }
    }
ICF_CHECK_TUNNEL_MODE_END

    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */
    return ret_val;
}
ICF_HANDLE_REINVITE_END




/******************************************************************************
 *
 * FUNCTION:        icf_cc_06_mm_error_ind
 *
 * DESCRIPTION:     This function is invoked when the CC is in CONNECTED state
 *                  and MM error indication is received from MMI
 *                  Current State: ICF_CC_06_CONNECTED
 *                  Current Event: ICF_CC_MM_ERR_IND
 *
 *****************************************************************************/
icf_return_t icf_cc_06_mm_error_ind(INOUT icf_cc_pdb_st *p_cc_pdb)
{
    icf_return_t     ret_val = ICF_SUCCESS;
    icf_mm_err_ind_st *p_mm_err_ind = ICF_NULL;
    icf_mmi_reason_t  media_error;

    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /* function entry trace */

        p_mm_err_ind = (icf_mm_err_ind_st*)
        (p_cc_pdb->p_internal_msg->p_msg_data);


    media_error = p_mm_err_ind->reason;
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

                    /* Set MM cleared bit in call_clear_status */
                    ICF_CC_SET_CALL_CLR_STATUS(p_cc_pdb, 
                            ICF_CALL_CLEAR_STATUS_MM_CLEARED)

            }

        if(ICF_SUCCESS == ret_val)
        {
            /*
             * Send BYE to network if signaling ongoing is FALSE
             */
            if(ICF_INT_MODULE_INVALID == 
                    ICF_CC_GET_SIG_ONGOING(p_cc_pdb))
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




/******************************************************************************
 *
 * FUNCTION:        icf_cc_06_generic_err
 *
 * DESCRIPTION:     This function is invoked when the CC is awaiting
 *                  create media session resp from MMI and it receives
 *                  an error indication from MMI..
 *                  Current State: tion from MMI..
 *                  Current State: ICF_CC_06_CONNECTED
 *                  Current Event: ICF_CC_GENERIC_ERROR
 *
 *****************************************************************************/

icf_return_t icf_cc_06_generic_err (
        INOUT icf_cc_pdb_st *p_cc_pdb)
{
    icf_return_t     ret_val = ICF_SUCCESS;

    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /* function entry trace */

        /* Set release type as internal error */
        ICF_CC_SET_RELEASE_TYPE(p_cc_pdb, ICF_REL_TYPE_INTERNAL_INITIATED)
        ICF_CC_TRACE(p_cc_pdb, ICF_TRACE_REL_TYPE_INT_ERR)

        /*
         * Start the release timer
         */
        if(ICF_FAILURE == icf_cc_start_release_timer(p_cc_pdb))
        {
            ret_val = ICF_FAILURE;
        } /* if(ICF_FAILURE == icf_st... */
    /* Send a DMS request to MMI */
        else  if(ICF_FAILURE == icf_cc_send_dms_req(p_cc_pdb))
        {
            ret_val = ICF_FAILURE;
        } /* if(ICF_FAILURE == icf_cc... */
        /* CSR_1_6903010 merged SPR 19672 start */
        /* CSR 1-6903010: Add the case when the signalling is with CC */
        else if((ICF_INT_MODULE_INVALID == ICF_CC_GET_SIG_ONGOING(p_cc_pdb))||
                (ICF_INT_MODULE_CC == ICF_CC_GET_SIG_ONGOING(p_cc_pdb)))
        /* CSR_1_6903010 merged SPR 19672 end */
        { 
            /*
             * Update the states in the call context
             */
            ICF_CC_SET_CALL_STATE(p_cc_pdb,ICF_CC_07_AWT_DMS_RESP)

                ICF_CC_TRACE(p_cc_pdb, ICF_TRACE_SIG_ONGOING)
                /*
                 * Send call terminate request to the SSA
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
                    ICF_CC_SET_SIG_ONGOING(p_cc_pdb)

                        ICF_CC_SET_NEXT_STATE(p_cc_pdb,
                                ICF_CC_08_AWT_CALL_TERMINATE_RESP)
                }
        } /* if(ICF_NULL == ICF_CC_GET_SIG_ON... */
        else
        {
            /*Return service terminate status to CM*/
            ICF_CC_SET_STATUS(p_cc_pdb,
                    ICF_STATUS_CALL_R_SRV_TERMINATE)
                /*
                 * Update the states in the call context
                 */
                ICF_CC_SET_CALL_STATE(p_cc_pdb,ICF_CC_07_AWT_DMS_RESP)
                /*
                 * Mark next state as ICF_CC_12_WAIT_START_PROC_SIG
                 */
                ICF_CC_SET_NEXT_STATE(p_cc_pdb,
                        ICF_CC_12_WAIT_START_PROC_SIG)
        }       
             /*we don't require any handling in new service status
               because now call is getting clear CC_CALL_RELEASE in status
               will take care */
             p_cc_pdb->p_internal_msg->msg_hdr.status_2 &=
                 ~ICF_STATUS_INTERNAL_REFER_W_REP_REQ;   
             p_cc_pdb->p_internal_msg->msg_hdr.status_2 &=
                 ~ICF_STATUS_INTERNAL_REFER_REQ;   
             p_cc_pdb->p_internal_msg->msg_hdr.status_2 &=
                 ~ICF_STATUS_EXTERNAL_CONFERENCE_CONNECTED;   
             p_cc_pdb->p_internal_msg->msg_hdr.status_2 &=
                 ~ICF_STATUS_INTERNAL_REFER_SUCCESS_RESP;   
             p_cc_pdb->p_internal_msg->msg_hdr.status_2 &=
                 ~ICF_STATUS_INTERNAL_REFER_FAILURE_RESP;   
             p_cc_pdb->p_internal_msg->msg_hdr.status_2 &=
                 ~ICF_STATUS_CALL_ENDED_DUE_TO_RELEASE;   
             p_cc_pdb->p_internal_msg->msg_hdr.status_2 &=
                 ~ICF_STATUS_INCOMING_EXTERNAL_CONF_CALL;   


    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */
        return ret_val;
}

#ifdef ICF_SESSION_TIMER
/******************************************************************************
 *
 * FUNCTION:        icf_cc_06_ssa_session_refresh_req
 *
 * DESCRIPTION:     This function is invoked when 
 *					session timer had expired. This will set the signalling
 *					ongoing flag to CC Module.
 *                  
 *                  ..
 *                  Current State: ICF_CC_06_CONNECTED
 *                  Current Event: ICF_CC_SESSION_REFRESH_REQ
 *
 *****************************************************************************/

icf_return_t icf_cc_06_ssa_session_refresh_req(
        INOUT icf_cc_pdb_st *p_cc_pdb)
{

    icf_return_t ret_val = ICF_SUCCESS;
    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /* function entry trace */
        ICF_CC_SET_SIG_ONGOING(p_cc_pdb);
    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */


        return ret_val ;
}
/******************************************************************************
 *
 * FUNCTION:        icf_cc_06_ssa_session_refresh_resp
 *
 * DESCRIPTION:     This function is invoked when 
 *					session refresh request is accepted (200 Ok)
 *                  
 *                  ..
 *                  Current State: ICF_CC_06_CONNECTED
 *                  Current Event: ICF_CC_SESSION_REFRESH_RESP
 *
 *****************************************************************************/

icf_return_t icf_cc_06_ssa_session_refresh_resp(
        INOUT icf_cc_pdb_st *p_cc_pdb)
{
    icf_return_t ret_val = ICF_SUCCESS;
    icf_nw_inc_media_change_resp_st *p_call_modify_resp = ICF_NULL;

    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /* function entry trace */
    
    ICF_CC_RESET_SIG_ONGOING(p_cc_pdb);
   
    /* Return status as signalling done to CM */
    ICF_CC_SET_STATUS(p_cc_pdb, ICF_STATUS_SIGNAL_ONGOING_ENDED);

    ret_val = icf_cc_stop_current_timer(p_cc_pdb);
    
    p_call_modify_resp = (icf_nw_inc_media_change_resp_st *)
                          p_cc_pdb->p_internal_msg->p_msg_data;

    if(ICF_NULL == p_call_modify_resp)
    {
        return ret_val;
    }

    if(ICF_NULL != p_call_modify_resp)
    {
       if(ICF_TRUE == p_call_modify_resp->status)
       {
           /* SPR 19526: In case Tunnel Mode is TRUE, there should not have been
            * a session refresh request from IPTK, but in case application misbehaves
            * and IPTK sends a session refresh request then no attempt should be made
            * to apply the media of the 200 OK response, the SDP should be ignored.
            */ 
ICF_CHECK_TUNNEL_MODE_START(p_cc_pdb->p_glb_pdb, ICF_TRUE)
         if(ICF_NULL != (p_call_modify_resp->bitmask & ICF_NW_INC_MC_SDP_PRESENT)) 
         {
             icf_cc_delete_inc_media_sdp_info_list(
                        p_cc_pdb,&(p_call_modify_resp->p_sdp_list)); 
         }
ICF_CHECK_TUNNEL_MODE_END
ICF_CHECK_TUNNEL_MODE_START(p_cc_pdb->p_glb_pdb, ICF_FALSE)           
         if(ICF_NULL == (p_call_modify_resp->bitmask & ICF_NW_INC_MC_SDP_PRESENT)) 
         {
           ret_val =  ICF_FAILURE;
         }
         else
         {
            ICF_CC_SET_ONGOING_EVENT(p_cc_pdb,p_cc_pdb->current_event)
            p_cc_pdb->p_list_1 = p_call_modify_resp->p_sdp_list;
            /*CSR 1-7785609. IPTK shall reset the media modify type for all streams. */
			icf_cc_reset_media_modify_type_for_all_streams(p_cc_pdb);
            if(ICF_FAILURE ==
              icf_cc_make_mms_type_for_nth_rem_succ_ans(p_cc_pdb))
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
                /* Update the states in the call context */
                ICF_CC_SET_CALL_STATE(p_cc_pdb,ICF_CC_04_AWT_MMS_RESP)
                ICF_CC_SET_NEXT_STATE(p_cc_pdb,ICF_CC_06_CONNECTED)
                /* Start the media timer  */
                ret_val = icf_cc_start_media_timer(p_cc_pdb);
               }
               else
               {
                  ICF_CC_SET_CALL_STATE(p_cc_pdb,ICF_CC_06_CONNECTED)
               }
            }/* MMS was handled succesfully */
          }
ICF_CHECK_TUNNEL_MODE_END
      }
      else
      {
         ret_val = icf_cc_06_generic_err(p_cc_pdb);
      }
    }
    else
    {
       ICF_CC_SET_CALL_STATE(p_cc_pdb,ICF_CC_06_CONNECTED)
    } 
    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */
    return ret_val ;
}

/******************************************************************************
 *
 * FUNCTION:        icf_cc_06_ssa_network_reject
 *
 * DESCRIPTION:     This function is invoked when 
 *					session refresh request is rejected
 *                  
 *                  ..
 *                  Current State: ICF_CC_06_CONNECTED
 *                  Current Event: ICF_CC_SSA_REMOTE_REJECT
 *
 *****************************************************************************/

icf_return_t icf_cc_06_ssa_network_reject(
        INOUT icf_cc_pdb_st *p_cc_pdb)
{
    icf_nw_inc_reject_st *p_internal_msg_param = ICF_NULL;
    icf_return_t ret_val = ICF_SUCCESS;
    p_internal_msg_param = (icf_nw_inc_reject_st *)
        p_cc_pdb->p_internal_msg->p_msg_data;

    if (ICF_FAILURE == icf_cc_start_release_timer(p_cc_pdb)) 
    {
        ret_val = ICF_FAILURE;
    }

    if(p_internal_msg_param->reason_code != 
            ICF_CALL_CLEAR_REMOTE_UNREACHABLE)
    {
        if (ICF_FAILURE == icf_cc_send_ssa_reject_ack(p_cc_pdb))
        {
            ret_val = ICF_FAILURE;
        }
        else
        {
            ICF_CC_TRACE(p_cc_pdb, ICF_TRACE_REL_TYPE_NW_INIT)
                ICF_CC_SET_RELEASE_TYPE(p_cc_pdb,ICF_REL_TYPE_NW_INITIATED) 
        }	
    }

    if(ICF_FAILURE == icf_cc_nw_clear_handler(p_cc_pdb))
    {
        ret_val = ICF_FAILURE;
    }
    /*
     * Send a DMS request to MMI
     */
    else if(ICF_FAILURE == icf_cc_send_dms_req(p_cc_pdb)) {
        ret_val = ICF_FAILURE;
    }
    else
    {
        /*
         * Update the states in the call context
         */
        ICF_CC_SET_CALL_STATE(p_cc_pdb,ICF_CC_07_AWT_DMS_RESP)
    }
    return ret_val;
} 

#endif



/******************************************************************************
 *
 * FUNCTION:        icf_cc_06_forceful_call_clear
 *
 * DESCRIPTION:     This function is invoked when Forceful call clear is 
 *                  received in state ICF_CC_06_CONNECTED 
 *                  Current State: ICF_CC_06_CONNECTED
 *                  Current Event: ICF_CC_FORCEFUL_CLEAR
 *
 *****************************************************************************/
icf_return_t icf_cc_06_forceful_call_clear(
        INOUT icf_cc_pdb_st *p_cc_pdb)
{
    icf_return_t ret_val = ICF_SUCCESS;

    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb)

    ICF_CC_SET_RELEASE_CAUSE(p_cc_pdb,ICF_REL_CAUSE_SERV_INT_ERR)

    /*
     * Send call clear request to the SSA
     */
     icf_cc_send_ssa_call_term_req(p_cc_pdb);

    /*Handling would be same as that of forceful call clear in atate IDLE*/
    icf_cc_00_clear_call (p_cc_pdb);

    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb)

    return ret_val;
}

/******************************************************************************
 *
 * FUNCTION:        icf_cc_06_pa_info_req 
 *
 * DESCRIPTION:     This function is invoked when the CC is waiting for 
 *                  START_PROC_MEDIA response from MMI and it receives info 
 *                  request from PA
 *                  Current State: ICF_CC_06_CONNECTED 
 *                  Current Event: ICF_CC_PA_INFO_REQ
 * 
 *****************************************************************************/
icf_return_t icf_cc_06_pa_info_req(
        INOUT icf_cc_pdb_st *p_cc_pdb)
{
    icf_return_t ret_val = ICF_SUCCESS;

    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb)

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
        else 
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

        }

    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb)

        return ret_val;
}


/******************************************************************************
 *
 * FUNCTION:        icf_cc_06_ssa_info_resp
 *
 * DESCRIPTION:     This function is invoked when the CC is connected state and 
 *                  it gets info response from network
 *                  Current State: ICF_CC_06_CONNECTED 
 *                  Current Event: ICF_CC_SSA_INFO_RESP
 * 
 *****************************************************************************/
icf_return_t icf_cc_06_ssa_info_resp(
        INOUT icf_cc_pdb_st *p_cc_pdb)
{
    icf_return_t ret_val = ICF_SUCCESS;
    icf_nw_inc_info_resp_st *p_info_resp;

    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb)

        p_info_resp = (icf_nw_inc_info_resp_st *)
        (p_cc_pdb->p_internal_msg->p_msg_data);

    if(ICF_TRUE == p_cc_pdb->p_call_ctx->pending_info_resp)
    {
        /* Stop the info timer*/
        icf_cc_stop_info_timer(p_cc_pdb);

        if(ICF_FAILURE == p_info_resp->status)
        {
           /* SPR 18064 Merge */
           if(ICF_NULL == p_info_resp->error_cause)
           {
               p_cc_pdb->p_call_ctx->app_failure_cause = ICF_CAUSE_INFO_REJECTED_BY_REMOTE;
           }
           else
           {
               p_cc_pdb->p_call_ctx->app_failure_cause =
			p_info_resp->error_cause;
           }
        }

        p_cc_pdb->p_call_ctx->pending_info_resp  = ICF_FALSE; 
        ret_val = icf_cc_send_pa_info_resp(p_cc_pdb);
    }

    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb)

        return ret_val;
}/*End icf_cc_06_ssa_info_resp*/


#ifdef ICF_FAX_SUPPORT

/*******************************************************************************
 *
 * FUNCTION:        icf_cc_06_pa_fax_req
 *
 * DESCRIPTION:     This function is invoked when the CC is in CONNECTED state
 *                  and it receives a fax request from PA
 *                  Current State: ICF_CC_06_CONNECTED
 *                  Current Event: ICF_CC_PA_FAX_REQ
 *
 ******************************************************************************/
icf_return_t icf_cc_06_pa_fax_req(
        INOUT icf_cc_pdb_st *p_cc_pdb)
{
    icf_return_t     ret_val = ICF_SUCCESS;

    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /* function entry trace */


        if((ICF_INT_MODULE_INVALID != ICF_CC_GET_SIG_ONGOING( p_cc_pdb)) ||
                (ICF_INT_MODULE_INVALID != ICF_CC_GET_MEDIA_ONGOING(p_cc_pdb)))
        {
            /* reject the request*/
            p_cc_pdb->p_call_ctx->app_failure_cause = ICF_CAUSE_CALL_MODIFY_REQ_PENDING;
            ICF_CC_SET_COMMON_BITMASK(p_cc_pdb, ICF_FAX_RESP_FAIL)

                /* Send failure response for update to user */
                if(ICF_FAILURE == icf_cc_send_pa_fax_resp(p_cc_pdb))
                {
                    ret_val = ICF_FAILURE;
                }

        }
        else
        {
            /*ICF_CC_SET_COMMON_BITMASK(p_cc_pdb,ICF_CALL_MODIFY_DIR)*/
            ICF_CC_RESET_COMMON_BITMASK(p_cc_pdb,ICF_CALL_MODIFY_DIR)

                ICF_CC_SET_ONGOING_EVENT(p_cc_pdb,p_cc_pdb->current_event)


                /*
                   Validate and Process the fax request. Set the appropriate media  
                   change types and values in the local sdp corresponsding to each 
                   added stream 
                 */
                if(ICF_FAILURE == icf_cc_process_local_fax_req(p_cc_pdb))
                {
                    ret_val = ICF_FAILURE;	
                }
                else if(p_cc_pdb->p_call_ctx->app_failure_cause)
                {
                    /*there is some error in processing of local fax request
                      so reject the request and send response to PA */
                    ICF_CC_SET_COMMON_BITMASK(p_cc_pdb, ICF_FAX_RESP_FAIL)
                        ret_val = icf_cc_send_pa_fax_resp(p_cc_pdb);
                }
            /*
             * Send Modify Media Session to the MMI
             * and start the modify media timer.
             */	
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
                            ICF_CC_SET_NEXT_STATE(p_cc_pdb,ICF_CC_14_AWT_CALL_MODIFY_RESP) 

                            /* Start the media timer  */
                            ret_val = icf_cc_start_media_timer(p_cc_pdb);

                        if(ICF_T38_FAX_CALL == p_cc_pdb->p_call_ctx->fax_type ||
                                ICF_T38_FAX_CALL_FAILURE_SWITCH_PT_CALL == 
                                p_cc_pdb->p_call_ctx->fax_type)
                        {
                            p_cc_pdb->p_call_ctx->call_type |= ICF_CALL_TYPE_T38;
                        }
                    }
                    else
                    {
                        ICF_CC_RESET_COMMON_BITMASK(p_cc_pdb, ICF_FAX_RESP_FAIL)
                            ret_val = icf_cc_send_pa_fax_resp(p_cc_pdb);

                        /* reset the media updated flag * /
                           p_cc_pdb->p_call_ctx->media_updated = ICF_FALSE;

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
                           ICF_CC_SET_CALL_STATE(p_cc_pdb,
                           ICF_CC_14_AWT_CALL_MODIFY_RESP) 
                           }
                         */
                    }
                }/* MMS was handled succesfully */
        }/* MMS can be sent to NW */



    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */
        return ret_val;
}


/*******************************************************************************
 *
 * FUNCTION:        icf_cc_06_ssa_fax_req
 *
 * DESCRIPTION:     This function is invoked when the CC is in CONNECTED state
 *                  and it receives a fax request from SSA
 *                  Current State: ICF_CC_06_CONNECTED
 *                  Current Event: ICF_CC_SSA_FAX_REQ
 *
 ******************************************************************************/
icf_return_t icf_cc_06_ssa_fax_req(
        INOUT icf_cc_pdb_st *p_cc_pdb)
{
    icf_return_t     ret_val = ICF_SUCCESS;

    icf_nw_inc_media_change_req_st *p_start_fax_req =
        (icf_nw_inc_media_change_req_st *)p_cc_pdb->p_internal_msg->p_msg_data;

    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /* function entry trace */

    if(ICF_INT_MODULE_INVALID != ICF_CC_GET_SIG_ONGOING( p_cc_pdb))
    {
        if(ICF_NW_INC_MEDIA_CHG_SDP_PRESENT ==
                (ICF_NW_INC_MEDIA_CHG_SDP_PRESENT & p_start_fax_req->bitmask))
        {
            icf_cc_delete_inc_media_sdp_info_list(
                    p_cc_pdb,&(p_start_fax_req->p_sdp_list));
        }

        /* call modify request can not be processed. Send 491 */
        p_cc_pdb->p_call_ctx->offer_failure_reason = ICF_REQUEST_PENDING;
        ret_val = icf_cc_send_ssa_offer_chg_err(p_cc_pdb);
    }
    /*CSR_1-6347417 : SPR-19129 fixed.*/
    /* A Re-INVITE for fax request with T38 codec is received from
     * remote with same fax method (add/replace) as Re-invite for start fax
     * has been sent on network with T38 codec when retry after timer is
     * running after receiving 491 from remote.
     */  
    else if((ICF_NULL != ICF_CC_CHECK_COMMON_BITMASK_2(p_cc_pdb,
                    ICF_CC_491_RESP_PROCESS_ONGOING)) &&
            (ICF_NULL != p_cc_pdb->p_call_ctx->fax_type))
    {
        ret_val = icf_cc_process_remote_fax_req_when_retry(p_cc_pdb);
    }
    else if(ICF_FAILURE == icf_cc_process_remote_fax_req(p_cc_pdb))
    {
        ret_val = ICF_FAILURE;
    }

	if(ICF_SUCCESS == ret_val)
	{
        if(p_cc_pdb->p_call_ctx->offer_failure_reason)
        {
            /*there is some error in processing of remote call modify offer
              so reject the offer and send error response to SSA */
            ret_val = icf_cc_send_ssa_offer_chg_err(p_cc_pdb);

            if (ICF_CC_STREAM_POS_VALIDATION_FAIL == 
                    (p_cc_pdb->common_bitmask & ICF_CC_STREAM_POS_VALIDATION_FAIL))
            {
                ret_val = icf_cc_06_generic_err(p_cc_pdb);
            }
        }
        else if(ICF_INT_MODULE_INVALID != ICF_CC_GET_MEDIA_ONGOING(p_cc_pdb))
        {
            /*
             * MMS cannot be sent to nw as request is pending from MM or 
             * signalling is ongoing
             * Mark offer change as pending event
             */
            ICF_CC_SET_PENDING_EVENT(p_cc_pdb,ICF_PENDING_EVENT_SSA_FAX_REQ)

                /* Set call state */
                ICF_CC_SET_CALL_STATE(p_cc_pdb,ICF_CC_13_WAIT_START_PROC_MEDIA)

                ICF_CC_SET_NEXT_STATE(p_cc_pdb,ICF_CC_06_CONNECTED)
                /* Set the call modify direction flag */
                p_cc_pdb->p_call_ctx->common_bitmask |= ICF_CALL_MODIFY_DIR;
        }
        else
        {
            ICF_CC_RESET_COMMON_BITMASK(p_cc_pdb,ICF_CALL_MODIFY_DIR)

                /* Set the call modify direction flag */
                p_cc_pdb->p_call_ctx->common_bitmask |= ICF_CALL_MODIFY_DIR;

            /* Set ICF_INT_MODULE_CC in signalling ongoing flag */
            ICF_CC_SET_SIG_ONGOING(p_cc_pdb)

                ICF_CC_SET_ONGOING_EVENT(p_cc_pdb,p_cc_pdb->current_event)

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
                      /*  CSR MERGE START : 1-5433345
                      * If the remote has requested to add a T38 stream and has not put
                      * the audio stream on hold, we would be rejecting the t38 stream.
                      * In this case the event would get set to ICF_CC_SSA_OFFER_CHANGE
                      * The call type should remain as audio as t38 would be rejected
                      */
                      if(ICF_CC_SSA_OFFER_CHANGE != p_cc_pdb->current_event)
                      {

                            p_cc_pdb->p_call_ctx->call_type |= ICF_CALL_TYPE_T38;
                        /* CSR MERGE END : 1-5433345 */
                         }

                        /* Start the media timer  */
                        ret_val = icf_cc_start_media_timer(p_cc_pdb);
                    }
                    else
                    {
                        icf_cc_send_ssa_offer_chg_rsp(p_cc_pdb);
                    }

                }/* MMS was handled succesfully */
        }/* MMS can be sent to NW */

    }

    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */
        return ret_val;
}

/*******************************************************************************
 *
 * FUNCTION:        icf_cc_06_pa_stop_fax_req
 *
 * DESCRIPTION:     This function is invoked when the CC is in CONNECTED state
 *                  and it receives a stop fax request from PA
 *                  Current State: ICF_CC_06_CONNECTED
 *                  Current Event: ICF_CC_PA_STOP_FAX_REQ
 *
 ******************************************************************************/
icf_return_t icf_cc_06_pa_stop_fax_req(
		INOUT icf_cc_pdb_st *p_cc_pdb)
{
	icf_return_t     ret_val = ICF_SUCCESS;

	ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /* function entry trace */

		if(ICF_NULL != p_cc_pdb->p_call_ctx->fax_type)
		{
			if(ICF_FAX_PT_CALL == p_cc_pdb->p_call_ctx->fax_type)
			{
				if(ICF_FAILURE == icf_cc_send_pa_stop_fax_resp(p_cc_pdb))
				{
					ret_val = ICF_FAILURE;
				}
				else
				{
					p_cc_pdb->p_call_ctx->fax_type = ICF_NULL;	
				}
			}
			else if(ICF_T38_FAX_CALL == p_cc_pdb->p_call_ctx->fax_type)
			{
				/* Audio line is not present or Network Disallow 
				   send the failure response to Applcation */
				if(ICF_FAILURE == icf_cc_check_audio_stream_status(p_cc_pdb))
				{
					p_cc_pdb->p_call_ctx->app_failure_cause = ICF_CAUSE_NO_ACTIVE_AUDIO_STREAM_PRESENT;
					ICF_CC_SET_COMMON_BITMASK(p_cc_pdb, ICF_FAX_RESP_FAIL)
						if(ICF_FAILURE == icf_cc_send_pa_stop_fax_resp(p_cc_pdb))
						{
							ret_val = ICF_FAILURE;
						}
				}
				/* Revert back to previous audio line */
				else
				{
                    /*  Fix for CSR 1-5947245 */
                    ICF_CC_RESET_COMMON_BITMASK(p_cc_pdb,ICF_CALL_MODIFY_DIR)
					ret_val = icf_cc_make_revert_mms_fax_cancel(p_cc_pdb);
					ICF_CC_SET_NEXT_STATE(p_cc_pdb,ICF_CC_15_AWT_CALL_MODIFY_ALERT_RESP)
				}
			}	
		}
		else /*If there is no fax call*/
		{
			p_cc_pdb->p_call_ctx->app_failure_cause = ICF_CAUSE_NO_FAX_CALL_ONGOING;
			ICF_CC_SET_COMMON_BITMASK(p_cc_pdb, ICF_FAX_RESP_FAIL)
				if(ICF_FAILURE == icf_cc_send_pa_stop_fax_resp(p_cc_pdb))
				{
					ret_val = ICF_FAILURE;
				}
		}
#ifdef ICF_LOAD_STAT
    if(ICF_SUCCESS == ret_val)
    {
        p_cc_pdb->p_glb_pdb->load_data.otg_stop_fax_req++;
    }
#endif
	return ret_val;
}

#endif

/******************************************************************************
 *
 * FUNCTION:        icf_cc_06_timer_expiry
 *
 * DESCRIPTION:     This function is invoked when the CC is connected state
 *					and INFO timer expires.
 *                  Timer expires.
 *                  Current State: ICF_CC_06_CONNECTED
 *                  Current Event: ICF_CC_TIMER_EXPIRY
 *
 *****************************************************************************/
icf_return_t icf_cc_06_timer_expiry(icf_cc_pdb_st *p_cc_pdb)
{
	icf_return_t     ret_val = ICF_SUCCESS;

	ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /* function entry trace */


		/* check if timer type is INFO */
		if(ICF_TIMER_TYPE_INFO == p_cc_pdb->timer_type)
		{
			p_cc_pdb->p_call_ctx->app_failure_cause = ICF_CAUSE_TIMER_EXPIRY;
			ret_val = icf_cc_send_pa_info_resp(p_cc_pdb);

			p_cc_pdb->p_call_ctx->pending_info_resp  = ICF_FALSE; 

			/* Reset the failure cause to ICF_NULL*/
			p_cc_pdb->p_call_ctx->app_failure_cause = ICF_NULL;
		}


	ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */
		return ret_val;
}

/******************************************************************************
 *
 * FUNCTION:        icf_cc_06_ssa_stop_fax_req
 *
 * DESCRIPTION:  *  This function handles the event when stop fax request
 * comes from network. It sends MMS to MMI
 *
 *****************************************************************************/
icf_return_t icf_cc_06_ssa_stop_fax_req(icf_cc_pdb_st *p_cc_pdb)
{

	icf_return_t     ret_val = ICF_SUCCESS;
	icf_internal_list_st    *p_sdp_list = ICF_NULL;
	icf_stream_sdp_info_st *p_audio_stream_info = ICF_NULL;
	icf_internal_list_st	*p_list =
		p_cc_pdb->p_call_ctx->p_stream_sdp_info;


	ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb); /* function entry trace */
        ret_val  = icf_cc_process_ssa_stop_fax_req(p_cc_pdb);
        if ((ICF_FAILURE == ret_val) ||
        (ICF_NULL != p_cc_pdb->p_call_ctx->offer_failure_reason)) 
        {
            ICF_PRINT(((icf_uint8_t*)"FAILURE in mms type remote offer change. Sending 487"));
            ret_val = icf_cc_send_ssa_offer_chg_err(p_cc_pdb);
            p_cc_pdb->p_call_ctx->offer_failure_reason = ICF_NULL;
        } 
	else if (ICF_FAILURE == icf_cc_send_mms_req(p_cc_pdb))
	{
		ICF_PRINT(((icf_uint8_t*)"Not able to make mms request. Sending 487"));
		ret_val = icf_cc_send_ssa_offer_chg_err(p_cc_pdb);
	}
	else
	{
		ret_val = icf_cc_start_media_timer(p_cc_pdb);
        
        /*  Fix for CSR 1-5947245 */
        ICF_CC_RESET_COMMON_BITMASK(p_cc_pdb,ICF_CALL_MODIFY_DIR)

        /* Set the call modify direction flag */
        p_cc_pdb->p_call_ctx->common_bitmask |= ICF_CALL_MODIFY_DIR;

		if(ICF_TRUE == p_cc_pdb->mms_sent)
		{
			/*
			 * Update the states in the call context
			 */
			ICF_PRINT(((icf_uint8_t*)"MMS has been sent successfully"));
			ICF_CC_SET_CALL_STATE(p_cc_pdb,ICF_CC_04_AWT_MMS_RESP);

			ICF_CC_SET_NEXT_STATE(p_cc_pdb,ICF_CC_06_CONNECTED);
			ICF_CC_SET_ONGOING_EVENT(p_cc_pdb,p_cc_pdb->current_event);
			p_sdp_list = icf_cc_get_node_from_stream_list(p_list, ICF_STREAM_TYPE_AUDIO);
			if (ICF_NULL != p_sdp_list)
			{
				ICF_PRINT(((icf_uint8_t*)"Resetting CLINE zero"));
				p_audio_stream_info = (icf_stream_sdp_info_st *)p_sdp_list->p_payload;

				p_audio_stream_info->stream_status &= ~ICF_MEDIA_STREAM_CLINE_ZERO;
			}

		}
	}

	ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb); /* function exit trace */
	return ret_val;
}

/*call_mod_cancel*/
/*******************************************************************************
 *
 * FUNCTION:        icf_cc_06_pa_call_modify_cancel_req
 *
 * DESCRIPTION:     This function is invoked when the CC is in CONNECTED state
 *                  and it receives call modify cancel request from PA
 *                  Current State: ICF_CC_06_CONNECTED
 *                  Current Event: ICF_CC_PA_CALL_MODIFY_CANCEL_REQ
 *
 ******************************************************************************/
icf_return_t icf_cc_06_pa_call_modify_cancel_req(
        INOUT icf_cc_pdb_st *p_cc_pdb)
{
    icf_return_t     ret_val = ICF_SUCCESS;

    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /* function entry trace */

    /*reject the request as 200 OK of Re-Invite is already received*/
    p_cc_pdb->p_call_ctx->app_failure_cause = 
                                           ICF_CAUSE_INVALID_REQUEST;
    ICF_CC_SET_COMMON_BITMASK(p_cc_pdb, ICF_CALL_MOD_CANCEL_RESP_FAIL)

    /* Send failure response to PA */
    if (ICF_FAILURE == icf_cc_send_pa_call_modify_cancel_resp(p_cc_pdb))
    {
        ret_val = ICF_FAILURE;
    }

    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */

    return ret_val;
}
