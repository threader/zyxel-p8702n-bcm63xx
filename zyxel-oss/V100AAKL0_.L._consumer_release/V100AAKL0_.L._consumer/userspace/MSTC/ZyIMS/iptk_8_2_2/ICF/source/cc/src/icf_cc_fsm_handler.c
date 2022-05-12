/*------------------------------------------------------------------------------
 *
 * File name        : icf_cc_fsm_handler.c
 *
 * Purpose          : This file contains the FSM table for CC
 *
 * Revision History :
 *
 * Date         Name            Ref#                Comments
 * --------     ------------    ----                ---------------
 * 13-Dec-2004  Pankaj Negi     Non Service LLD     Initial
 * 28-Dec-2004  Rohit Aggarwal                      Coding for entry point func
 * 29-Dec-2004  Rohit Aggarwal                      Timer data handling
 * 14-Jan-2005  Rohit Aggarwal                      Removed insure compilation
 *                                                  warnings
 * 16-Jan-2005  Rohit Aggarwal                      Changed entry point logic
 * 17-Jan-2005  Rohit Aggarwal                      Fixing bugs found during UT
 * 18-Feb-2005  Sumit Gupta                         Updation in timer data 
 *                                                  handling
 * 06-Feb-2005  Sumit Gupta                         Handling API other than 
 *                                                  CREATE CALL from PA as first
 *                                                  API   
 * 07-Feb-2005  Sumit Gupta                         Adding code for copying SDP 
 *                                                  in case of offer change.
 * 13-Apr-2005  Sumit Gupta     SPR 7396            Updated the fsm handler to 
 *                                                  ignore message if cancel is
 *                                                  received in state  
 *                                                  ICF_CC_11_AWT_SSA_CONNECT
 *                                                  _ACK
 * 23-APR-2005   Sumit Gupta    SPR 7429            Added new entry to FSM 
 *                                                  Handler for handling SSA 
 *                                                  connect. It would be same as
 *                                                  in case of await DMS
 * 26-APR-2005   Sumit Gupta    SPR 7447            Wouldn't be setting nw 
 *                                                  cleared in case CFW return 
 *                                                  failure with cause 
 *                                                  "USER BUSY" or "NO ANSWER"
 *                                                  or "SERVICE FAILURE"
 * 26-APR-2005   Sumit Gupta    SPR 7448            Updated the rem_media_mode 
 *                                                  in call context when mode is
 *                                                  received for offer_change
 * 10-MAY-2005   Sumit Gupta                        In case of generic error as
 *                                                  ecode in return path of 
 *                                                  processing success would be 
 *                                                  propogated upwards. Status 
 *                                                  would be set to 
 *                                                  CALL_R_SRVC_TERM for CM to 
 *                                                  initiate generic call clear
 * 12-May-2005   Sumit Gupta    SPR 7440			Updated call related logs
 * 25-May-2005   Sumit Gupta    SPR 7440			Updated Stats
 * 09-JUN-2005   Jalaj Negi     IMS CLIENT			Code change for IMS CLIENT
 * 21-Oct-2005  Sumit Gupta   SPR 8128          
 * 21-Oct-2005  Sumit Gupta   SPR 8127          
 * 21-Oct-2005  Sumit Gupta   SPR 8076          
 * 08-Nov-2005  Sumit Gupta   SPR 8241     
 * 07-Mar-2006	 Anuradha Gupta	None				Added the changes for incoming
 *												Re-invite handling *
 *     
 * 08-Mar-2006  Aman Aggarwal						ICF merging with ICF2.0    
 * 30-Mar-2006  Anuradha Gupta	ICF 5.0				Call Modify Feature changes
 * 03-Jan-2007  Amit Mahajan                        INFO confirmation changes. 
 * 04-Jan-2007   Priyanka Dua    ICF 6.3            Code changes for Forking and 
 *												    Multiple answer
 * 20-Mar-2007  Rajat Chandna                       Changes for response code 
 *                                                  enhancement.
 * 26-Mar-2007  Deepti Goyal     ICF 6.4            Changes for media connect 
 *                                                  request API from peer.
 * 17-Apr-2007  Rajat Chandna                       Changes for wildcarded user
 *                                                  enhancement.
 * 04-Apr-2007  Deepti Goyal     ICF 6.3            Changes for media connect 
 *                                                  request API from peer.
 *
 * 17-Apr-2007  Abhishek Dutta   ICF 6.3            Added the changes for 
 *                                                  TWC-UPDATE Handling
 * 20-Jun-2007  Tarun Gupta      SPR 15043           Change in function:
 *                                                   icf_cc_convert_api_to_event
 *                                                   Added handling for API 
 *                                                   ICF_NW_INC_MEDIA_UPDATE
 *
 * 10-Jul-2007  Abhishek Dutta   ICF 7.0            Added the changes for
 *                                                  Cancel call modify 
 *                                                  Handling
 * 25-Jan-2008  Neha Gupta       ICF Rel8.0         Modifications for 
 *                                                  precondition feature.
 * 1-Feb-2008   Abhishek Dhammawat Rel8.0           Code walkthrough comments
 *                                                  for precondition.
 * 12-Apr-2008  Alok Tiwari       SPR-18358         Klocwork warning removal.
 * 27-May-2008  Tarun Gupta       SPR 18585         Modified icf_cc_convert_api_to_event
 * 05-Jun-2009  Tarun Gupta       SPR 18585         Re-INVITE handled in case of
 *                                                  XCONF being pending
 * 07-Jun-2008  Amit Sharma       IPTK 8.0 SRTP LLD      SRTP Enhancements
 * 21-Jun-2008  Jagmohan Chauhan  SPR 18704        Crash in IPTK  when IPTK tries to 
 *                                                 free the ssa mesage after sending 491
 *                                                  for incoming UPDATE 
 * 25-June-2008 Jagmohan Chauhan  SPR 18684        Merge CSR 1-6044134
 * 02-July-2008 Anurag Khare      SPR 18684        CSR 1-6087284 Merge
 * 27-Jun-2008  Tarun Gupta       SPR 18731        Modified icf_cc_convert_api_to_event
 * 02-July-2008 Anurag Khare      SPR 18684        CSR 1-6037047 Merge
 * 22-Jul-2008 Abhishek Dhammawat CSR1-6155032 Merge Modified
 *                                                 icf_cc_convert_api_to_event
 * 07-Aug-2008  Tarun Gupta       SPR 18658        Modified icf_cc_convert_api_to_event
 * 04-Nov-2008  Tarun Gupta       SPR 19189        Merged Rel 7.1.2 SPR 18880
 * 04-Nov-2008  Anuradha Gupta    SPR 19188        Rel 7.1.2 SPR Merging
 * 26-Nov-2008  Tarun Gupta       SPR 19218        Merged CSR 1-6200296
 * 07-Dec-2008 Abhishek Dhammawat SPR 19223        Removing klocworks warning 
 * 12-Dec-2008  Tarun Gupta       SPR 19218        Merged SPR 18097
 * 12-Dec-2008  Rajiv Kumar       SPR 19218        Merged CSR 1-6240518
 * 09-Feb-2009  Tarun Gupta       Rel 8.1 LLD      SDP Tunnelling Enhancements
 * 11-Feb-2009  Anurag Khare      ICF Rel 8.1      changes for call modify
 *                                                 Enhancements
 * 04-Mar-2009 Saurabh Gupta	   SPR 19349       Klocworks fix
 * 15-Mar-2009  Anuradha Gupta    Rel 8.1 LLD      Timer Enhancements
 * 30-Mar-2009  Tarun Gupta       Rel 8.2          REFER/NOTIFY Enhancements
 * 31-Mar-2009  Anurag Khare      Rel 8.2          Changes done for PRACK Enh
 * 1-Apr-2009   Abhishek Dhammawat Rel 8.2         REFER/NOTIFY Enhancements
 * 06-Apr-2009  Kamal Ashraf       Rel 8.2         Klocworks fix
 * 10-Apr-2009  Tarun Gupta        SPR 19435       Modified icf_cc_xx_pa_generic_msg_req 
 * 24-Apr-2009  Anuradha Gupta     SPR 19499       Modified icf_cc_init 
 * 6-May-2009   Abhishek Dhammawat SPR 19574       Modified icf_cc_init
 * 18-May-2009  Anuradha Gupta    SPR 19672     CSR_1-6347417 merging
 * 21-May-2009  Anuradha Gupta    SPR 19672     CSR_1-6347417 merging
 * 25-May-2009  Anuradha Gupta    SPR 19672     CSR_1-6779001 merging
 * 26-May-2009  Anuradha Gupta    SPR 19672     CSR_1-6727350 merging
 * 08-Jun-2009  Rajiv Kumar       SPR 19733     Fix For SPR:19733
 * 10-Jun-2009  Abhishek Dhammawat SPR 19590    hdr list overwrite Enhancements
 * 18-Jun-2009  Anuradha Gupta     SPR 19751    Modified icf_cc_convert_api_to_event
 * 02-Aug-2009  Rajiv Kumar        SPR 19886    Merged CSR 1-7480178 
 * 08-Oct-2009  Abhishek Dhammawat SPR 20063    Merged CSR 1-7507275 
 * 27-Oct-2009  Abhishek Dhammawat SPR 20196    Modified icf_cc_init
 * 9-Jul-2010   Udit Gattani       SPR 20517   Modified icf_cc_convert_api_to_event
 *                                             when 180 with SDP received
 *  
 * Copyright (c) 2009, Aricent.
 *----------------------------------------------------------------------------*/

#include "icf_cc_macro.h"
#include "icf_internal_api_id.h"
#include "icf_dbm_prototypes.h"
#include "icf_cc_prototypes.h"
#include "icf_ecode.h"
#include "icf_sc_common_types.h"

/*******************************************************************************
 *
 * FUNCTION:        icf_cc_init
 *
 * DESCRIPTION:     This function creates memory block for storing global data.
 *
 ******************************************************************************/
icf_return_t icf_cc_init(
        INOUT   icf_glb_pdb_st        *p_glb_pdb,
        OUT     icf_void_t            **p_p_cc_data,
        OUT     icf_error_t           *p_ecode)
{
    icf_return_t                     ret_val = ICF_SUCCESS;
    icf_cc_glb_data_st               *p_cc_data = ICF_NULL;
    icf_uint8_t                      state, event;

    ICF_FUNCTION_ENTER(p_glb_pdb) /* function entry trace */

    /* Allocate memory for fsm function pointer table */
    ICF_STATIC_MEMGET(p_glb_pdb, sizeof(icf_cc_glb_data_st), p_cc_data,
            ICF_RET_ON_FAILURE, p_ecode, ret_val)

    /* Assignment of function pointers of fsm */

    /*
     * Init the function pointer table with ignore msg function. Then, each
     * of the non-empty handlers can be individually assigned
     */
    for (state=0; state<ICF_CC_MAX_STATES; state++)
    {
        for (event=0; event<ICF_CC_MAX_EVENTS; event++)
        {
            p_cc_data->cc_fsm_table[state][event] = icf_cc_ignore_msg;
        }
    }

    /* State ICF_CC_00_IDLE */
    p_cc_data->cc_fsm_table[ICF_CC_00_IDLE]
        [ICF_CC_PA_SETUP_REQ] = icf_cc_00_pa_setup_req;
    p_cc_data->cc_fsm_table[ICF_CC_00_IDLE]
        [ICF_CC_SSA_SETUP_REQ] = icf_cc_00_ssa_setup_req;
    p_cc_data->cc_fsm_table[ICF_CC_00_IDLE]
        [ICF_CC_GENERIC_ERROR] = icf_cc_00_generic_err;
    p_cc_data->cc_fsm_table[ICF_CC_00_IDLE]
        [ICF_CC_FORCEFUL_CLEAR] = icf_cc_00_clear_call;

    /* State ICF_CC_01_AWT_CMS_RESP */
    p_cc_data->cc_fsm_table[ICF_CC_01_AWT_CMS_RESP]
        [ICF_CC_CMS_RESP] = icf_cc_01_create_media_resp;
    p_cc_data->cc_fsm_table[ICF_CC_01_AWT_CMS_RESP]
        [ICF_CC_PA_CALL_TERMINATE_REQ] = icf_cc_01_pa_call_term_req;    
    p_cc_data->cc_fsm_table[ICF_CC_01_AWT_CMS_RESP]
        [ICF_CC_MM_ERR_IND] = icf_cc_01_mm_err_ind;
    p_cc_data->cc_fsm_table[ICF_CC_01_AWT_CMS_RESP]
        [ICF_CC_TIMER_EXPIRY] = icf_cc_01_timer_expiry;
    p_cc_data->cc_fsm_table[ICF_CC_01_AWT_CMS_RESP]
        [ICF_CC_SSA_CALL_CANCEL_REQ] = icf_cc_01_ssa_call_cancel_req;
    p_cc_data->cc_fsm_table[ICF_CC_01_AWT_CMS_RESP]
        [ICF_CC_GENERIC_ERROR] = icf_cc_01_generic_err;
    p_cc_data->cc_fsm_table[ICF_CC_01_AWT_CMS_RESP]
        [ICF_CC_FORCEFUL_CLEAR] = icf_cc_01_forceful_call_clear;
    p_cc_data->cc_fsm_table[ICF_CC_01_AWT_CMS_RESP]
        [ICF_CC_DMS_RESP] = icf_cc_01_dms_resp;
    
    /* New events handled in tunnel mode in State ICF_CC_01_AWT_CMS_RESP */
    p_cc_data->cc_fsm_table[ICF_CC_01_AWT_CMS_RESP]
        [ICF_CC_PA_ALERT] = icf_cc_01_pa_alert;
    p_cc_data->cc_fsm_table[ICF_CC_01_AWT_CMS_RESP]
        [ICF_CC_PA_MEDIA_CONNECT_REQ] = icf_cc_01_pa_media_connect_req;

   /*twc_update handling*/
    p_cc_data->cc_fsm_table[ICF_CC_01_AWT_CMS_RESP]
        [ICF_CC_PA_CALL_MODIFY_REQ] = icf_cc_01_pa_call_modify_req;

   /* Added for REFER/NOTIFY Enhancements */
    p_cc_data->cc_fsm_table[ICF_CC_01_AWT_CMS_RESP]
        [ICF_CC_PA_GENERIC_MSG_CFM] = icf_cc_xx_pa_generic_msg_cfm;
    p_cc_data->cc_fsm_table[ICF_CC_01_AWT_CMS_RESP]
        [ICF_CC_PA_GENERIC_MSG_REQ] = icf_cc_xx_pa_generic_msg_req;

    /* State ICF_CC_02_AWT_SSA_SETUP_RESP */
    p_cc_data->cc_fsm_table[ICF_CC_02_AWT_SSA_SETUP_RESP]
        [ICF_CC_SSA_SETUP_RESP] = icf_cc_02_ssa_setup_resp;
    p_cc_data->cc_fsm_table[ICF_CC_02_AWT_SSA_SETUP_RESP]
        [ICF_CC_SSA_MEDIA_PARAM] = icf_cc_02_ssa_media_param;
    p_cc_data->cc_fsm_table[ICF_CC_02_AWT_SSA_SETUP_RESP]
        [ICF_CC_SSA_ALERT] = icf_cc_02_ssa_alert;
    p_cc_data->cc_fsm_table[ICF_CC_02_AWT_SSA_SETUP_RESP]
        [ICF_CC_SSA_ALERT_WITH_MEDIA] = icf_cc_02_ssa_alrt_with_media;
    p_cc_data->cc_fsm_table[ICF_CC_02_AWT_SSA_SETUP_RESP]
        [ICF_CC_SSA_CALL_CONNECT] = icf_cc_02_ssa_call_connect;
    p_cc_data->cc_fsm_table[ICF_CC_02_AWT_SSA_SETUP_RESP]
        [ICF_CC_SSA_REMOTE_REJECT] = icf_cc_02_ssa_remote_reject;
    p_cc_data->cc_fsm_table[ICF_CC_02_AWT_SSA_SETUP_RESP]
        [ICF_CC_SSA_REDIRECT] = icf_cc_02_ssa_redirect;
    p_cc_data->cc_fsm_table[ICF_CC_02_AWT_SSA_SETUP_RESP]
        [ICF_CC_PA_CALL_TERMINATE_REQ] = icf_cc_02_pa_call_term_req;
    p_cc_data->cc_fsm_table[ICF_CC_02_AWT_SSA_SETUP_RESP]
        [ICF_CC_MM_ERR_IND] = icf_cc_02_mm_error_ind;
    p_cc_data->cc_fsm_table[ICF_CC_02_AWT_SSA_SETUP_RESP]
        [ICF_CC_TIMER_EXPIRY] = icf_cc_02_timer_expiry;
    p_cc_data->cc_fsm_table[ICF_CC_02_AWT_SSA_SETUP_RESP]
        [ICF_CC_GENERIC_ERROR] = icf_cc_02_generic_err;
    p_cc_data->cc_fsm_table[ICF_CC_02_AWT_SSA_SETUP_RESP]
        [ICF_CC_FORCEFUL_CLEAR] = icf_cc_00_clear_call;
    /*twc_update handling*/
    p_cc_data->cc_fsm_table[ICF_CC_02_AWT_SSA_SETUP_RESP]
        [ICF_CC_PA_CALL_MODIFY_REQ] = icf_cc_01_pa_call_modify_req;
    p_cc_data->cc_fsm_table[ICF_CC_02_AWT_SSA_SETUP_RESP]
        [ICF_CC_SSA_CALL_PROGRESS] = icf_cc_02_ssa_call_progress;
    p_cc_data->cc_fsm_table[ICF_CC_02_AWT_SSA_SETUP_RESP]
        [ICF_CC_SRTP_FALLBACK_REQ] = icf_cc_xx_ssa_srtp_fallback;
    
    /* Added for REFER/NOTIFY Enhancements */
    p_cc_data->cc_fsm_table[ICF_CC_02_AWT_SSA_SETUP_RESP]
        [ICF_CC_PA_GENERIC_MSG_CFM] = icf_cc_xx_pa_generic_msg_cfm;
    p_cc_data->cc_fsm_table[ICF_CC_02_AWT_SSA_SETUP_RESP]
        [ICF_CC_PA_GENERIC_MSG_REQ] = icf_cc_xx_pa_generic_msg_req;

    /* State ICF_CC_03_AWT_ALERT_RESP */
    p_cc_data->cc_fsm_table[ICF_CC_03_AWT_ALERT_RESP]
        [ICF_CC_SSA_MEDIA_PARAM] = icf_cc_03_ssa_media_param;
    p_cc_data->cc_fsm_table[ICF_CC_03_AWT_ALERT_RESP]
        [ICF_CC_SSA_ALERT] = icf_cc_03_ssa_alert;
    p_cc_data->cc_fsm_table[ICF_CC_03_AWT_ALERT_RESP]
        [ICF_CC_SSA_ALERT_WITH_MEDIA] = icf_cc_03_ssa_alrt_with_media;
    p_cc_data->cc_fsm_table[ICF_CC_03_AWT_ALERT_RESP]
        [ICF_CC_SSA_CALL_CONNECT] = icf_cc_03_ssa_call_connect;
    p_cc_data->cc_fsm_table[ICF_CC_03_AWT_ALERT_RESP]
        [ICF_CC_SSA_REMOTE_REJECT] = icf_cc_03_ssa_remote_reject;
    p_cc_data->cc_fsm_table[ICF_CC_03_AWT_ALERT_RESP]
        [ICF_CC_SSA_REDIRECT] = icf_cc_03_ssa_redirect;
    p_cc_data->cc_fsm_table[ICF_CC_03_AWT_ALERT_RESP]
        [ICF_CC_PA_CALL_TERMINATE_REQ] = icf_cc_03_pa_call_term_req;
    p_cc_data->cc_fsm_table[ICF_CC_03_AWT_ALERT_RESP]
        [ICF_CC_SSA_OFFER_CHANGE] = icf_cc_03_ssa_offer_change;
    p_cc_data->cc_fsm_table[ICF_CC_03_AWT_ALERT_RESP]
        [ICF_CC_MM_ERR_IND] = icf_cc_03_mm_error_ind;
    p_cc_data->cc_fsm_table[ICF_CC_03_AWT_ALERT_RESP]
        [ICF_CC_TIMER_EXPIRY] = icf_cc_03_timer_expiry;
    p_cc_data->cc_fsm_table[ICF_CC_03_AWT_ALERT_RESP]
        [ICF_CC_PA_ALERT] =  icf_cc_03_pa_alert;
    p_cc_data->cc_fsm_table[ICF_CC_03_AWT_ALERT_RESP]
        [ICF_CC_PA_CALL_CONNECT] = icf_cc_03_pa_call_connect;
    p_cc_data->cc_fsm_table[ICF_CC_03_AWT_ALERT_RESP]
        [ICF_CC_SSA_CALL_CANCEL_REQ] = icf_cc_03_ssa_call_cancel_req;
    p_cc_data->cc_fsm_table[ICF_CC_03_AWT_ALERT_RESP]
        [ICF_CC_GENERIC_ERROR] = icf_cc_03_generic_err;
    p_cc_data->cc_fsm_table[ICF_CC_03_AWT_ALERT_RESP]
        [ICF_CC_FORCEFUL_CLEAR] = icf_cc_03_forceful_call_clear;
#ifdef ICF_QOS_SUPPORT
    p_cc_data->cc_fsm_table[ICF_CC_03_AWT_ALERT_RESP]
        [ICF_CC_MEDIA_PARAM_PRACK_RESP] = icf_cc_03_media_prack_resp;
#endif
    p_cc_data->cc_fsm_table[ICF_CC_03_AWT_ALERT_RESP]
        [ICF_CC_MEDIA_UPDATE_RESP] = icf_cc_03_media_update_resp;
    p_cc_data->cc_fsm_table[ICF_CC_03_AWT_ALERT_RESP]
        [ICF_CC_PA_MEDIA_CONNECT_REQ] =  icf_cc_03_pa_media_connect_req;
    p_cc_data->cc_fsm_table[ICF_CC_03_AWT_ALERT_RESP]
        [ICF_CC_PA_MEDIA_CONNECT_REQ] =  icf_cc_03_pa_media_connect_req;

    /*twc_update handling*/
    p_cc_data->cc_fsm_table[ICF_CC_03_AWT_ALERT_RESP]
        [ICF_CC_PA_CALL_MODIFY_REQ] = icf_cc_03_pa_call_modify_req;

    p_cc_data->cc_fsm_table[ICF_CC_03_AWT_ALERT_RESP]
        [ICF_CC_SSA_CALL_PROGRESS] = icf_cc_03_ssa_call_progress;

    p_cc_data->cc_fsm_table[ICF_CC_03_AWT_ALERT_RESP]
        [ICF_CC_SRTP_FALLBACK_REQ] = icf_cc_xx_ssa_srtp_fallback;
    
    /* Added for REFER/NOTIFY Enhancements */
    p_cc_data->cc_fsm_table[ICF_CC_03_AWT_ALERT_RESP]
        [ICF_CC_PA_GENERIC_MSG_CFM] = icf_cc_xx_pa_generic_msg_cfm;
    p_cc_data->cc_fsm_table[ICF_CC_03_AWT_ALERT_RESP]
        [ICF_CC_PA_GENERIC_MSG_REQ] = icf_cc_xx_pa_generic_msg_req;
    
    p_cc_data->cc_fsm_table[ICF_CC_03_AWT_ALERT_RESP]
        [ICF_CC_PA_CALL_MODIFY_RSP] = icf_cc_14_pa_call_modify_resp;

    /* State ICF_CC_04_AWT_MMS_RESP */
    p_cc_data->cc_fsm_table[ICF_CC_04_AWT_MMS_RESP]
        [ICF_CC_SSA_MEDIA_PARAM] = icf_cc_04_ssa_media_param;
    p_cc_data->cc_fsm_table[ICF_CC_04_AWT_MMS_RESP]
        [ICF_CC_SSA_ALERT] = icf_cc_04_ssa_alert;
    p_cc_data->cc_fsm_table[ICF_CC_04_AWT_MMS_RESP]
        [ICF_CC_SSA_ALERT_WITH_MEDIA] = icf_cc_04_ssa_alrt_with_media;
    p_cc_data->cc_fsm_table[ICF_CC_04_AWT_MMS_RESP]
        [ICF_CC_SSA_CALL_CONNECT] = icf_cc_04_ssa_call_connect;
    p_cc_data->cc_fsm_table[ICF_CC_04_AWT_MMS_RESP]
        [ICF_CC_MMS_RESP] = icf_cc_04_mms_resp;
    p_cc_data->cc_fsm_table[ICF_CC_04_AWT_MMS_RESP]
        [ICF_CC_SSA_CALL_TERMINATE_REQ] = icf_cc_04_ssa_call_term_req;
    p_cc_data->cc_fsm_table[ICF_CC_04_AWT_MMS_RESP]
        [ICF_CC_SSA_REMOTE_REJECT] = icf_cc_04_ssa_remote_reject;
    p_cc_data->cc_fsm_table[ICF_CC_04_AWT_MMS_RESP]
        [ICF_CC_SSA_REDIRECT] = icf_cc_04_ssa_redirect;
    p_cc_data->cc_fsm_table[ICF_CC_04_AWT_MMS_RESP]
        [ICF_CC_PA_CALL_TERMINATE_REQ] = icf_cc_04_pa_call_term_req;
    p_cc_data->cc_fsm_table[ICF_CC_04_AWT_MMS_RESP]
        [ICF_CC_SSA_OFFER_CHANGE] = icf_cc_04_ssa_offer_change;
    p_cc_data->cc_fsm_table[ICF_CC_04_AWT_MMS_RESP]
        [ICF_CC_MM_ERR_IND] = icf_cc_04_mm_error_ind;
    p_cc_data->cc_fsm_table[ICF_CC_04_AWT_MMS_RESP]
        [ICF_CC_TIMER_EXPIRY] = icf_cc_04_timer_expiry;
#ifdef  ICF_PA_ALERT_INBAND_HANDLING
    p_cc_data->cc_fsm_table[ICF_CC_04_AWT_MMS_RESP]
        [ICF_CC_PA_ALERT] = icf_cc_04_pa_alert;
#endif
    p_cc_data->cc_fsm_table[ICF_CC_04_AWT_MMS_RESP]
        [ICF_CC_PA_CALL_CONNECT] = icf_cc_04_pa_call_connect;

    p_cc_data->cc_fsm_table[ICF_CC_04_AWT_MMS_RESP]
        [ICF_CC_MEDIA_PARAM_PRACK] = icf_cc_04_media_prack;
    p_cc_data->cc_fsm_table[ICF_CC_04_AWT_MMS_RESP]
        [ICF_CC_ALERT_PRACK] = icf_cc_04_alert_prack;
    p_cc_data->cc_fsm_table[ICF_CC_04_AWT_MMS_RESP]
        [ICF_CC_SSA_CONNECT_ACK] = icf_cc_04_ssa_connect_ack;
    p_cc_data->cc_fsm_table[ICF_CC_04_AWT_MMS_RESP]
        [ICF_CC_SSA_CALL_CANCEL_REQ] = icf_cc_04_ssa_call_cancel_req;
    p_cc_data->cc_fsm_table[ICF_CC_04_AWT_MMS_RESP]
        [ICF_CC_GENERIC_ERROR] = icf_cc_04_generic_err;
    p_cc_data->cc_fsm_table[ICF_CC_04_AWT_MMS_RESP]
        [ICF_CC_FORCEFUL_CLEAR] = icf_cc_04_forceful_call_clear;
    p_cc_data->cc_fsm_table[ICF_CC_04_AWT_MMS_RESP]
        [ICF_CC_PA_INFO_REQ] = icf_cc_04_pa_info_req;
    p_cc_data->cc_fsm_table[ICF_CC_04_AWT_MMS_RESP]
        [ICF_CC_SSA_INFO_RESP] = icf_cc_06_ssa_info_resp;
    /*Fix merged for CSR_1-5417099*/
    p_cc_data->cc_fsm_table[ICF_CC_04_AWT_MMS_RESP]
        [ICF_CC_PA_ALERT] = icf_cc_04_pa_alert_req;
    p_cc_data->cc_fsm_table[ICF_CC_04_AWT_MMS_RESP]
        [ICF_CC_PA_MEDIA_CONNECT_REQ] = icf_cc_04_pa_media_connect_req;
    

#ifdef ICF_QOS_SUPPORT
    p_cc_data->cc_fsm_table[ICF_CC_04_AWT_MMS_RESP]
        [ICF_CC_MEDIA_PARAM_PRACK_RESP] = icf_cc_04_media_prack_resp;
#endif
ICF_HANDLE_REINVITE_START
	p_cc_data->cc_fsm_table[ICF_CC_04_AWT_MMS_RESP]
        [ICF_CC_PA_CALL_MODIFY_REQ] = icf_cc_04_pa_call_modify_req;
ICF_HANDLE_REINVITE_END
#ifdef ICF_FAX_SUPPORT
    p_cc_data->cc_fsm_table[ICF_CC_04_AWT_MMS_RESP]
		[ICF_CC_PA_FAX_REQ] = icf_cc_14_pa_fax_req;
    p_cc_data->cc_fsm_table[ICF_CC_04_AWT_MMS_RESP]
		[ICF_CC_SSA_FAX_REQ] = icf_cc_14_ssa_fax_req;
    p_cc_data->cc_fsm_table[ICF_CC_04_AWT_MMS_RESP]
		[ICF_CC_PA_STOP_FAX_REQ] = icf_cc_04_pa_stop_fax_req;
    p_cc_data->cc_fsm_table[ICF_CC_04_AWT_MMS_RESP]
		[ICF_CC_SSA_STOP_FAX_REQ] = icf_cc_04_ssa_stop_fax_req;
    p_cc_data->cc_fsm_table[ICF_CC_04_AWT_MMS_RESP]
		[ICF_CC_SSA_CALL_CANCEL_RESP] = icf_cc_14_ssa_stop_fax_resp;
    p_cc_data->cc_fsm_table[ICF_CC_04_AWT_MMS_RESP]
		[ICF_CC_SSA_FAX_RESP] = icf_cc_04_ssa_fax_resp;
#endif
  
   /*twc_update handling*/
    p_cc_data->cc_fsm_table[ICF_CC_04_AWT_MMS_RESP]
        [ICF_CC_MEDIA_UPDATE_RESP] = icf_cc_14_ssa_call_modify_resp;

    /*call_mod_cancel*/
    p_cc_data->cc_fsm_table[ICF_CC_04_AWT_MMS_RESP]
        [ICF_CC_PA_CALL_MODIFY_CANCEL_REQ] = icf_cc_04_pa_call_modify_cancel_req;

    p_cc_data->cc_fsm_table[ICF_CC_04_AWT_MMS_RESP]
        [ICF_CC_SSA_CALL_MODIFY_CANCEL_RESP] = icf_cc_14_ssa_call_modify_cancel_resp;

    p_cc_data->cc_fsm_table[ICF_CC_04_AWT_MMS_RESP]
        [ICF_CC_SSA_CALL_PROGRESS] = icf_cc_03_ssa_call_progress;

    p_cc_data->cc_fsm_table[ICF_CC_04_AWT_MMS_RESP]
        [ICF_CC_SRTP_FALLBACK_REQ] = icf_cc_xx_ssa_srtp_fallback;

    /* Added for REFER/NOTIFY Enhancements */
    p_cc_data->cc_fsm_table[ICF_CC_04_AWT_MMS_RESP]
        [ICF_CC_PA_GENERIC_MSG_CFM] = icf_cc_xx_pa_generic_msg_cfm;
    p_cc_data->cc_fsm_table[ICF_CC_04_AWT_MMS_RESP]
        [ICF_CC_SSA_GENERIC_MSG_RESP] = icf_cc_xx_ssa_generic_msg_resp;
    p_cc_data->cc_fsm_table[ICF_CC_04_AWT_MMS_RESP]
        [ICF_CC_SSA_GENERIC_MSG_IND] = icf_cc_xx_ssa_generic_msg_ind;
    p_cc_data->cc_fsm_table[ICF_CC_04_AWT_MMS_RESP]
        [ICF_CC_PA_GENERIC_MSG_REQ] = icf_cc_xx_pa_generic_msg_req;


    /* State ICF_CC_05_AWT_CONNECT */
    p_cc_data->cc_fsm_table[ICF_CC_05_AWT_CONNECT]
        [ICF_CC_SSA_MEDIA_PARAM] = icf_cc_05_ssa_media_param;
    p_cc_data->cc_fsm_table[ICF_CC_05_AWT_CONNECT]
        [ICF_CC_SSA_ALERT] = icf_cc_05_ssa_alert;
    p_cc_data->cc_fsm_table[ICF_CC_05_AWT_CONNECT]
        [ICF_CC_SSA_ALERT_WITH_MEDIA] = icf_cc_05_ssa_alert_with_media;
    p_cc_data->cc_fsm_table[ICF_CC_05_AWT_CONNECT]
        [ICF_CC_SSA_CALL_CONNECT] = icf_cc_05_ssa_call_connect;
    p_cc_data->cc_fsm_table[ICF_CC_05_AWT_CONNECT]
        [ICF_CC_SSA_REMOTE_REJECT] = icf_cc_05_ssa_remote_reject;
    p_cc_data->cc_fsm_table[ICF_CC_05_AWT_CONNECT]
        [ICF_CC_SSA_REDIRECT] = icf_cc_05_ssa_redirect;
    p_cc_data->cc_fsm_table[ICF_CC_05_AWT_CONNECT]
        [ICF_CC_PA_CALL_TERMINATE_REQ] = icf_cc_05_pa_call_term_req;
    p_cc_data->cc_fsm_table[ICF_CC_05_AWT_CONNECT]
        [ICF_CC_SSA_OFFER_CHANGE] = icf_cc_05_ssa_offer_change;
    p_cc_data->cc_fsm_table[ICF_CC_05_AWT_CONNECT]
        [ICF_CC_MM_ERR_IND] = icf_cc_05_mm_error_ind;
    p_cc_data->cc_fsm_table[ICF_CC_05_AWT_CONNECT]
        [ICF_CC_TIMER_EXPIRY] = icf_cc_05_timer_expiry;
    p_cc_data->cc_fsm_table[ICF_CC_05_AWT_CONNECT]
        [ICF_CC_PA_CALL_CONNECT] = icf_cc_05_pa_call_connect;
    p_cc_data->cc_fsm_table[ICF_CC_05_AWT_CONNECT]
        [ICF_CC_SSA_CALL_CANCEL_REQ] = icf_cc_05_ssa_call_cancel_req;
    p_cc_data->cc_fsm_table[ICF_CC_05_AWT_CONNECT]
        [ICF_CC_GENERIC_ERROR] = icf_cc_05_generic_err;
    p_cc_data->cc_fsm_table[ICF_CC_05_AWT_CONNECT]
        [ICF_CC_FORCEFUL_CLEAR] = icf_cc_05_forceful_call_clear;
    p_cc_data->cc_fsm_table[ICF_CC_05_AWT_CONNECT]
        [ICF_CC_PA_MEDIA_CONNECT_REQ] =  icf_cc_03_pa_media_connect_req;
    p_cc_data->cc_fsm_table[ICF_CC_05_AWT_CONNECT]
        [ICF_CC_PA_ALERT] =  icf_cc_03_pa_alert;
    p_cc_data->cc_fsm_table[ICF_CC_05_AWT_CONNECT]
        [ICF_CC_PA_MEDIA_CONNECT_REQ] =  icf_cc_03_pa_media_connect_req;
    p_cc_data->cc_fsm_table[ICF_CC_05_AWT_CONNECT]
        [ICF_CC_PA_ALERT] =  icf_cc_03_pa_alert;

   /*twc_update handling*/
    p_cc_data->cc_fsm_table[ICF_CC_05_AWT_CONNECT]
        [ICF_CC_PA_CALL_MODIFY_REQ] = icf_cc_05_pa_call_modify_req;
    p_cc_data->cc_fsm_table[ICF_CC_05_AWT_CONNECT]
        [ICF_CC_MEDIA_UPDATE_RESP] = icf_cc_14_ssa_call_modify_resp;
    p_cc_data->cc_fsm_table[ICF_CC_05_AWT_CONNECT]
        [ICF_CC_SSA_CALL_PROGRESS] = icf_cc_03_ssa_call_progress;
    p_cc_data->cc_fsm_table[ICF_CC_05_AWT_CONNECT]
        [ICF_CC_PA_CALL_MODIFY_RSP] = icf_cc_14_pa_call_modify_resp;
     /*  Fix for CSR 1-7507275 : SPR 19881 */
    /*SPR 20196 We should have separate function for supporting
      CSR 1-7507275 in state 05 and should not use state 03
      function as it results triggering of QOS UPDATE twice which
      is undesirable therefore replicated 03 function as 05
      removing the QOS fucntionality*/
    p_cc_data->cc_fsm_table[ICF_CC_05_AWT_CONNECT]
        [ICF_CC_MEDIA_PARAM_PRACK_RESP] = icf_cc_05_media_prack_resp;
    p_cc_data->cc_fsm_table[ICF_CC_05_AWT_CONNECT]
        [ICF_CC_SRTP_FALLBACK_REQ] = icf_cc_xx_ssa_srtp_fallback;

    /* Added for REFER/NOTIFY Enhancements */
    p_cc_data->cc_fsm_table[ICF_CC_05_AWT_CONNECT]
        [ICF_CC_PA_GENERIC_MSG_CFM] = icf_cc_xx_pa_generic_msg_cfm;
    p_cc_data->cc_fsm_table[ICF_CC_05_AWT_CONNECT]
        [ICF_CC_PA_GENERIC_MSG_REQ] = icf_cc_xx_pa_generic_msg_req;


    /* State ICF_CC_06_CONNECTED */
    p_cc_data->cc_fsm_table[ICF_CC_06_CONNECTED]
        [ICF_CC_SSA_CALL_TERMINATE_REQ] = icf_cc_06_ssa_call_term_req;
    p_cc_data->cc_fsm_table[ICF_CC_06_CONNECTED]
        [ICF_CC_PA_CALL_TERMINATE_REQ] = icf_cc_06_pa_call_term_req;
    p_cc_data->cc_fsm_table[ICF_CC_06_CONNECTED]
        [ICF_CC_SSA_OFFER_CHANGE] = icf_cc_06_ssa_offer_change;
    p_cc_data->cc_fsm_table[ICF_CC_06_CONNECTED]
        [ICF_CC_MM_ERR_IND] = icf_cc_06_mm_error_ind;
    p_cc_data->cc_fsm_table[ICF_CC_06_CONNECTED]
        [ICF_CC_GENERIC_ERROR] = icf_cc_06_generic_err;
    p_cc_data->cc_fsm_table[ICF_CC_06_CONNECTED]
        [ICF_CC_FORCEFUL_CLEAR] = icf_cc_06_forceful_call_clear;
    p_cc_data->cc_fsm_table[ICF_CC_06_CONNECTED]
        [ICF_CC_PA_INFO_REQ] = icf_cc_06_pa_info_req;
    p_cc_data->cc_fsm_table[ICF_CC_06_CONNECTED]
        [ICF_CC_SSA_INFO_RESP] = icf_cc_06_ssa_info_resp;
#ifdef ICF_SESSION_TIMER
	p_cc_data->cc_fsm_table[ICF_CC_06_CONNECTED]
        [ICF_CC_SSA_REMOTE_REJECT] = icf_cc_06_ssa_network_reject;
    p_cc_data->cc_fsm_table[ICF_CC_06_CONNECTED]
        [ICF_CC_SESSION_REFRESH_REQ] = icf_cc_06_ssa_session_refresh_req;
	p_cc_data->cc_fsm_table[ICF_CC_06_CONNECTED]
		[ICF_CC_SESSION_REFRESH_RESP] = icf_cc_06_ssa_session_refresh_resp;
#endif
ICF_HANDLE_REINVITE_START
	p_cc_data->cc_fsm_table[ICF_CC_06_CONNECTED]
        [ICF_CC_PA_CALL_MODIFY_REQ] = icf_cc_06_pa_call_modify_req;
	
ICF_HANDLE_REINVITE_END

#ifdef ICF_FAX_SUPPORT
	p_cc_data->cc_fsm_table[ICF_CC_06_CONNECTED]
        [ICF_CC_PA_FAX_REQ] = icf_cc_06_pa_fax_req;
    p_cc_data->cc_fsm_table[ICF_CC_06_CONNECTED]
        [ICF_CC_SSA_FAX_REQ] = icf_cc_06_ssa_fax_req;
	p_cc_data->cc_fsm_table[ICF_CC_06_CONNECTED]
        [ICF_CC_PA_STOP_FAX_REQ] = icf_cc_06_pa_stop_fax_req;
    p_cc_data->cc_fsm_table[ICF_CC_06_CONNECTED]
        [ICF_CC_SSA_STOP_FAX_REQ] = icf_cc_06_ssa_stop_fax_req;
#endif
    /*twc_update handling*/
    p_cc_data->cc_fsm_table[ICF_CC_06_CONNECTED]
        [ICF_CC_MEDIA_UPDATE_RESP] = icf_cc_14_ssa_call_modify_resp;

    /*call_mod_cancel*/
    p_cc_data->cc_fsm_table[ICF_CC_06_CONNECTED]
        [ICF_CC_PA_CALL_MODIFY_CANCEL_REQ] = icf_cc_06_pa_call_modify_cancel_req;
    p_cc_data->cc_fsm_table[ICF_CC_06_CONNECTED]
        [ICF_CC_SSA_CALL_PROGRESS] = icf_cc_03_ssa_call_progress;
    p_cc_data->cc_fsm_table[ICF_CC_06_CONNECTED]
        [ICF_CC_SRTP_FALLBACK_REQ] = icf_cc_06_ssa_network_reject;

    /* Added for REFER/NOTIFY Enhancements */
    p_cc_data->cc_fsm_table[ICF_CC_06_CONNECTED]
        [ICF_CC_PA_GENERIC_MSG_CFM] = icf_cc_xx_pa_generic_msg_cfm;
    p_cc_data->cc_fsm_table[ICF_CC_06_CONNECTED]
        [ICF_CC_SSA_GENERIC_MSG_RESP] = icf_cc_xx_ssa_generic_msg_resp;
    p_cc_data->cc_fsm_table[ICF_CC_06_CONNECTED]
        [ICF_CC_SSA_GENERIC_MSG_IND] = icf_cc_xx_ssa_generic_msg_ind;
    p_cc_data->cc_fsm_table[ICF_CC_06_CONNECTED]
        [ICF_CC_PA_GENERIC_MSG_REQ] = icf_cc_xx_pa_generic_msg_req;

    /* State ICF_CC_07_AWT_DMS_RESP */
    p_cc_data->cc_fsm_table[ICF_CC_07_AWT_DMS_RESP]
        [ICF_CC_SSA_SETUP_RESP] = icf_cc_07_ssa_setup_resp;
    p_cc_data->cc_fsm_table[ICF_CC_07_AWT_DMS_RESP]
        [ICF_CC_DMS_RESP] = icf_cc_07_dms_resp;
    p_cc_data->cc_fsm_table[ICF_CC_07_AWT_DMS_RESP]
        [ICF_CC_SSA_CALL_TERMINATE_REQ] = icf_cc_07_ssa_call_term_req;
    p_cc_data->cc_fsm_table[ICF_CC_07_AWT_DMS_RESP]
        [ICF_CC_SSA_REMOTE_REJECT] = icf_cc_07_ssa_remote_reject;
    p_cc_data->cc_fsm_table[ICF_CC_07_AWT_DMS_RESP]
        [ICF_CC_PA_CALL_TERMINATE_REQ] = icf_cc_07_pa_call_term_req;
    p_cc_data->cc_fsm_table[ICF_CC_07_AWT_DMS_RESP]
        [ICF_CC_SSA_CALL_TERMINATE_RESP] = icf_cc_07_ssa_call_term_resp;
    p_cc_data->cc_fsm_table[ICF_CC_07_AWT_DMS_RESP]
        [ICF_CC_SSA_OFFER_CHANGE] = icf_cc_07_ssa_offer_change;
    p_cc_data->cc_fsm_table[ICF_CC_07_AWT_DMS_RESP]
        [ICF_CC_TIMER_EXPIRY] = icf_cc_07_timer_expiry;
    p_cc_data->cc_fsm_table[ICF_CC_07_AWT_DMS_RESP]
        [ICF_CC_GENERIC_ERROR] = icf_cc_07_generic_err;
    p_cc_data->cc_fsm_table[ICF_CC_07_AWT_DMS_RESP]
        [ICF_CC_FORCEFUL_CLEAR] = icf_cc_00_clear_call;
    p_cc_data->cc_fsm_table[ICF_CC_07_AWT_DMS_RESP]
        [ICF_CC_SSA_CONNECT_ACK] = icf_cc_07_ssa_connect_ack;
    p_cc_data->cc_fsm_table[ICF_CC_07_AWT_DMS_RESP]
        [ICF_CC_SSA_CALL_CONNECT] = icf_cc_07_ssa_call_connect;
    p_cc_data->cc_fsm_table[ICF_CC_07_AWT_DMS_RESP]
        [ICF_CC_SSA_REDIRECT] = icf_cc_07_ssa_redirect;
ICF_HANDLE_REINVITE_START
	p_cc_data->cc_fsm_table[ICF_CC_07_AWT_DMS_RESP]
        [ICF_CC_SSA_REINVITE_ACK] = icf_cc_07_ssa_reinvite_ack;
    p_cc_data->cc_fsm_table[ICF_CC_07_AWT_DMS_RESP]
        [ICF_CC_MEDIA_UPDATE_RESP] = icf_cc_07_ssa_reinvite_ack;
ICF_HANDLE_REINVITE_END
    p_cc_data->cc_fsm_table[ICF_CC_07_AWT_DMS_RESP]
        [ICF_CC_SRTP_FALLBACK_REQ] = icf_cc_07_ssa_remote_reject;

    /* CSR 1-6044134: SPR 18684: Handle stop fax from ssa in 07 state */
#ifdef ICF_FAX_SUPPORT
    p_cc_data->cc_fsm_table[ICF_CC_07_AWT_DMS_RESP]
        [ICF_CC_SSA_STOP_FAX_REQ] = icf_cc_07_ssa_offer_change;
#endif

    /* Added for REFER/NOTIFY Enhancements */
    p_cc_data->cc_fsm_table[ICF_CC_07_AWT_DMS_RESP]
        [ICF_CC_PA_GENERIC_MSG_CFM] = icf_cc_xx_pa_generic_msg_cfm;
    p_cc_data->cc_fsm_table[ICF_CC_07_AWT_DMS_RESP]
        [ICF_CC_SSA_GENERIC_MSG_RESP] = icf_cc_xx_ssa_generic_msg_resp;
    p_cc_data->cc_fsm_table[ICF_CC_07_AWT_DMS_RESP]
        [ICF_CC_SSA_GENERIC_MSG_IND] = icf_cc_xx_ssa_generic_msg_ind;
    p_cc_data->cc_fsm_table[ICF_CC_07_AWT_DMS_RESP]
        [ICF_CC_PA_GENERIC_MSG_REQ] = icf_cc_xx_pa_generic_msg_req;
    /* Added new function for handling the MM Error indication in
     * the 07 state
     */ 
    p_cc_data->cc_fsm_table[ICF_CC_07_AWT_DMS_RESP]
        [ICF_CC_MM_ERR_IND] = icf_cc_07_mm_err_ind;

    /* State ICF_CC_08_AWT_CALL_TERMINATE_RESP */
    p_cc_data->cc_fsm_table[ICF_CC_08_AWT_CALL_TERMINATE_RESP]
        [ICF_CC_SSA_SETUP_RESP] = icf_cc_08_ssa_setup_resp;
    p_cc_data->cc_fsm_table[ICF_CC_08_AWT_CALL_TERMINATE_RESP]
        [ICF_CC_SSA_CALL_TERMINATE_REQ] = icf_cc_08_ssa_call_term_req;
    p_cc_data->cc_fsm_table[ICF_CC_08_AWT_CALL_TERMINATE_RESP]
        [ICF_CC_SSA_REMOTE_REJECT] = icf_cc_08_ssa_remote_reject;
    p_cc_data->cc_fsm_table[ICF_CC_08_AWT_CALL_TERMINATE_RESP]
        [ICF_CC_PA_CALL_TERMINATE_REQ] = icf_cc_08_pa_call_term_req;
    p_cc_data->cc_fsm_table[ICF_CC_08_AWT_CALL_TERMINATE_RESP]
        [ICF_CC_PA_CALL_TERMINATE_RESP] = icf_cc_08_pa_call_term_resp;
    p_cc_data->cc_fsm_table[ICF_CC_08_AWT_CALL_TERMINATE_RESP]
        [ICF_CC_SSA_CALL_TERMINATE_RESP] = icf_cc_08_ssa_call_term_resp;
    p_cc_data->cc_fsm_table[ICF_CC_08_AWT_CALL_TERMINATE_RESP]
        [ICF_CC_SSA_OFFER_CHANGE] = icf_cc_08_ssa_offer_change;
    p_cc_data->cc_fsm_table[ICF_CC_08_AWT_CALL_TERMINATE_RESP]
        [ICF_CC_TIMER_EXPIRY] = icf_cc_08_timer_expiry;
    p_cc_data->cc_fsm_table[ICF_CC_08_AWT_CALL_TERMINATE_RESP]
        [ICF_CC_GENERIC_ERROR] = icf_cc_08_generic_err;
    p_cc_data->cc_fsm_table[ICF_CC_08_AWT_CALL_TERMINATE_RESP]
        [ICF_CC_FORCEFUL_CLEAR] = icf_cc_00_clear_call;
    p_cc_data->cc_fsm_table[ICF_CC_08_AWT_CALL_TERMINATE_RESP]
        [ICF_CC_SSA_CALL_CONNECT] = icf_cc_07_ssa_call_connect;
    p_cc_data->cc_fsm_table[ICF_CC_08_AWT_CALL_TERMINATE_RESP]
        [ICF_CC_SSA_CONNECT_ACK] = icf_cc_07_ssa_connect_ack;
    p_cc_data->cc_fsm_table[ICF_CC_08_AWT_CALL_TERMINATE_RESP]
        [ICF_CC_SSA_REDIRECT] = icf_cc_07_ssa_redirect;
    p_cc_data->cc_fsm_table[ICF_CC_08_AWT_CALL_TERMINATE_RESP]
        [ICF_CC_SSA_CALL_CONNECT] = icf_cc_07_ssa_call_connect;
    p_cc_data->cc_fsm_table[ICF_CC_08_AWT_CALL_TERMINATE_RESP]
        [ICF_CC_SRTP_FALLBACK_REQ] = icf_cc_08_ssa_remote_reject;

    /* Added for REFER/NOTIFY Enhancements */
    p_cc_data->cc_fsm_table[ICF_CC_08_AWT_CALL_TERMINATE_RESP]
        [ICF_CC_PA_GENERIC_MSG_CFM] = icf_cc_xx_pa_generic_msg_cfm;
    p_cc_data->cc_fsm_table[ICF_CC_08_AWT_CALL_TERMINATE_RESP]
        [ICF_CC_SSA_GENERIC_MSG_RESP] = icf_cc_xx_ssa_generic_msg_resp;
    p_cc_data->cc_fsm_table[ICF_CC_08_AWT_CALL_TERMINATE_RESP]
        [ICF_CC_SSA_GENERIC_MSG_IND] = icf_cc_xx_ssa_generic_msg_ind;
    p_cc_data->cc_fsm_table[ICF_CC_08_AWT_CALL_TERMINATE_RESP]
        [ICF_CC_PA_GENERIC_MSG_REQ] = icf_cc_xx_pa_generic_msg_req;


    /* State ICF_CC_09_AWT_MEDIA_PARAM_PRACK */
    p_cc_data->cc_fsm_table[ICF_CC_09_AWT_MEDIA_PARAM_PRACK]
        [ICF_CC_PA_CALL_TERMINATE_REQ] = icf_cc_09_pa_call_term_req;
    p_cc_data->cc_fsm_table[ICF_CC_09_AWT_MEDIA_PARAM_PRACK]
        [ICF_CC_SSA_OFFER_CHANGE] = icf_cc_09_ssa_offer_change;
    p_cc_data->cc_fsm_table[ICF_CC_09_AWT_MEDIA_PARAM_PRACK]
        [ICF_CC_MM_ERR_IND] = icf_cc_09_mm_err_ind;
    p_cc_data->cc_fsm_table[ICF_CC_09_AWT_MEDIA_PARAM_PRACK]
        [ICF_CC_TIMER_EXPIRY] = icf_cc_09_timer_expiry;
    p_cc_data->cc_fsm_table[ICF_CC_09_AWT_MEDIA_PARAM_PRACK]
        [ICF_CC_PA_CALL_CONNECT] = icf_cc_09_pa_call_connect;
    p_cc_data->cc_fsm_table[ICF_CC_09_AWT_MEDIA_PARAM_PRACK]
        [ICF_CC_MEDIA_PARAM_PRACK] = icf_cc_09_media_param_prack;
    p_cc_data->cc_fsm_table[ICF_CC_09_AWT_MEDIA_PARAM_PRACK]
        [ICF_CC_SSA_CALL_CANCEL_REQ] = icf_cc_09_ssa_call_cancel_req;
    p_cc_data->cc_fsm_table[ICF_CC_09_AWT_MEDIA_PARAM_PRACK]
        [ICF_CC_GENERIC_ERROR] = icf_cc_09_generic_err;
    p_cc_data->cc_fsm_table[ICF_CC_09_AWT_MEDIA_PARAM_PRACK]
        [ICF_CC_FORCEFUL_CLEAR] = icf_cc_09_forceful_call_clear;
    p_cc_data->cc_fsm_table[ICF_CC_09_AWT_MEDIA_PARAM_PRACK]
        [ICF_CC_PA_MEDIA_CONNECT_REQ] =  icf_cc_09_pa_media_connect_req;
    p_cc_data->cc_fsm_table[ICF_CC_09_AWT_MEDIA_PARAM_PRACK]
        [ICF_CC_PA_ALERT] =  icf_cc_09_pa_alert_req;
    p_cc_data->cc_fsm_table[ICF_CC_09_AWT_MEDIA_PARAM_PRACK]
        [ICF_CC_PA_MEDIA_CONNECT_REQ] =  icf_cc_09_pa_media_connect_req;
    p_cc_data->cc_fsm_table[ICF_CC_09_AWT_MEDIA_PARAM_PRACK]
        [ICF_CC_PA_ALERT] =  icf_cc_09_pa_alert_req;

    /*  Rel 8.1 changes
     *  handling of pa_call_mod_req
     *  in awt prack state
     */
    p_cc_data->cc_fsm_table[ICF_CC_09_AWT_MEDIA_PARAM_PRACK]
        [ICF_CC_PA_CALL_MODIFY_REQ] = icf_cc_10_pa_call_modify_req;

    p_cc_data->cc_fsm_table[ICF_CC_09_AWT_MEDIA_PARAM_PRACK]
        [ICF_CC_MEDIA_UPDATE_RESP]  = icf_cc_14_ssa_call_modify_resp;

    p_cc_data->cc_fsm_table[ICF_CC_09_AWT_MEDIA_PARAM_PRACK]
        [ICF_CC_PA_CALL_MODIFY_RSP] = icf_cc_14_pa_call_modify_resp;

    /* Added for REFER/NOTIFY Enhancements */
    p_cc_data->cc_fsm_table[ICF_CC_09_AWT_MEDIA_PARAM_PRACK]
        [ICF_CC_PA_GENERIC_MSG_CFM] = icf_cc_xx_pa_generic_msg_cfm;
    p_cc_data->cc_fsm_table[ICF_CC_09_AWT_MEDIA_PARAM_PRACK]
        [ICF_CC_PA_GENERIC_MSG_REQ] = icf_cc_xx_pa_generic_msg_req;

    /* State ICF_CC_10_AWT_ALERT_PRACK */
    p_cc_data->cc_fsm_table[ICF_CC_10_AWT_ALERT_PRACK]
        [ICF_CC_PA_CALL_TERMINATE_REQ] = icf_cc_10_pa_call_term_req;
    p_cc_data->cc_fsm_table[ICF_CC_10_AWT_ALERT_PRACK]
        [ICF_CC_SSA_OFFER_CHANGE] = icf_cc_10_ssa_offer_change;
    p_cc_data->cc_fsm_table[ICF_CC_10_AWT_ALERT_PRACK]
        [ICF_CC_MM_ERR_IND] = icf_cc_10_mm_err_ind;
    p_cc_data->cc_fsm_table[ICF_CC_10_AWT_ALERT_PRACK]
        [ICF_CC_TIMER_EXPIRY] = icf_cc_10_timer_expiry;
    p_cc_data->cc_fsm_table[ICF_CC_10_AWT_ALERT_PRACK]
        [ICF_CC_PA_CALL_CONNECT] = icf_cc_10_pa_call_connect;
    p_cc_data->cc_fsm_table[ICF_CC_10_AWT_ALERT_PRACK]
        [ICF_CC_ALERT_PRACK] = icf_cc_10_alert_prack;
    p_cc_data->cc_fsm_table[ICF_CC_10_AWT_ALERT_PRACK]
        [ICF_CC_SSA_CALL_CANCEL_REQ] = icf_cc_10_ssa_call_cancel_req;
    p_cc_data->cc_fsm_table[ICF_CC_10_AWT_ALERT_PRACK]
        [ICF_CC_GENERIC_ERROR] = icf_cc_10_generic_err;
    p_cc_data->cc_fsm_table[ICF_CC_10_AWT_ALERT_PRACK]
        [ICF_CC_FORCEFUL_CLEAR] = icf_cc_09_forceful_call_clear;
    p_cc_data->cc_fsm_table[ICF_CC_10_AWT_ALERT_PRACK]
        [ICF_CC_PA_MEDIA_CONNECT_REQ] =  icf_cc_09_pa_media_connect_req;
    p_cc_data->cc_fsm_table[ICF_CC_10_AWT_ALERT_PRACK]
        [ICF_CC_PA_ALERT] =  icf_cc_09_pa_alert_req; 

    /*  REl 8.1 changes
     *  handling of call_mod_req when awt prack state
     */
    p_cc_data->cc_fsm_table[ICF_CC_10_AWT_ALERT_PRACK]
        [ICF_CC_PA_CALL_MODIFY_REQ] = icf_cc_10_pa_call_modify_req;

    p_cc_data->cc_fsm_table[ICF_CC_10_AWT_ALERT_PRACK]
        [ICF_CC_MEDIA_UPDATE_RESP]  = icf_cc_14_ssa_call_modify_resp;

    p_cc_data->cc_fsm_table[ICF_CC_10_AWT_ALERT_PRACK]
        [ICF_CC_PA_CALL_MODIFY_RSP] = icf_cc_14_pa_call_modify_resp;

    /* Added for REFER/NOTIFY Enhancements */
    p_cc_data->cc_fsm_table[ICF_CC_10_AWT_ALERT_PRACK]
        [ICF_CC_PA_GENERIC_MSG_CFM] = icf_cc_xx_pa_generic_msg_cfm;
    p_cc_data->cc_fsm_table[ICF_CC_10_AWT_ALERT_PRACK]
        [ICF_CC_PA_GENERIC_MSG_REQ] = icf_cc_xx_pa_generic_msg_req;


    /* State ICF_CC_11_AWT_SSA_CONNECT_ACK */
    p_cc_data->cc_fsm_table[ICF_CC_11_AWT_SSA_CONNECT_ACK]
        [ICF_CC_PA_CALL_TERMINATE_REQ] = icf_cc_11_pa_call_term_req;
    p_cc_data->cc_fsm_table[ICF_CC_11_AWT_SSA_CONNECT_ACK]
        [ICF_CC_SSA_OFFER_CHANGE] = icf_cc_11_ssa_offer_change;
    p_cc_data->cc_fsm_table[ICF_CC_11_AWT_SSA_CONNECT_ACK]
        [ICF_CC_MM_ERR_IND] = icf_cc_11_mm_err_ind;
    p_cc_data->cc_fsm_table[ICF_CC_11_AWT_SSA_CONNECT_ACK]
        [ICF_CC_TIMER_EXPIRY] = icf_cc_11_timer_expiry;
    p_cc_data->cc_fsm_table[ICF_CC_11_AWT_SSA_CONNECT_ACK]
        [ICF_CC_SSA_CONNECT_ACK] = icf_cc_11_ssa_connect_ack;
    p_cc_data->cc_fsm_table[ICF_CC_11_AWT_SSA_CONNECT_ACK]
        [ICF_CC_GENERIC_ERROR] = icf_cc_11_generic_err;
    p_cc_data->cc_fsm_table[ICF_CC_11_AWT_SSA_CONNECT_ACK]
        [ICF_CC_FORCEFUL_CLEAR] = icf_cc_00_clear_call;
    p_cc_data->cc_fsm_table[ICF_CC_11_AWT_SSA_CONNECT_ACK]
		[ICF_CC_SSA_CALL_TERMINATE_REQ] = icf_cc_11_ssa_call_term_req;

    /* Added for REFER/NOTIFY Enhancements */
    p_cc_data->cc_fsm_table[ICF_CC_11_AWT_SSA_CONNECT_ACK]
        [ICF_CC_PA_GENERIC_MSG_CFM] = icf_cc_xx_pa_generic_msg_cfm;
    p_cc_data->cc_fsm_table[ICF_CC_11_AWT_SSA_CONNECT_ACK]
        [ICF_CC_PA_GENERIC_MSG_REQ] = icf_cc_xx_pa_generic_msg_req;

    /* State ICF_CC_12_WAIT_START_PROC_SIG */
    p_cc_data->cc_fsm_table[ICF_CC_12_WAIT_START_PROC_SIG]
        [ICF_CC_START_CALL_PROC_SIG] = icf_cc_12_start_call_proc_sig;
    p_cc_data->cc_fsm_table[ICF_CC_12_WAIT_START_PROC_SIG]
        [ICF_CC_SSA_CALL_TERMINATE_REQ] = icf_cc_12_ssa_call_term_req;
    p_cc_data->cc_fsm_table[ICF_CC_12_WAIT_START_PROC_SIG]
        [ICF_CC_PA_CALL_TERMINATE_REQ] = icf_cc_12_pa_call_term_req;
    p_cc_data->cc_fsm_table[ICF_CC_12_WAIT_START_PROC_SIG]
        [ICF_CC_PA_CALL_TERMINATE_RESP] = icf_cc_12_pa_call_term_resp;
    p_cc_data->cc_fsm_table[ICF_CC_12_WAIT_START_PROC_SIG]
        [ICF_CC_SSA_OFFER_CHANGE] = icf_cc_12_ssa_offer_change;
    p_cc_data->cc_fsm_table[ICF_CC_12_WAIT_START_PROC_SIG]
        [ICF_CC_TIMER_EXPIRY] = icf_cc_12_timer_expiry;
    p_cc_data->cc_fsm_table[ICF_CC_12_WAIT_START_PROC_SIG]
        [ICF_CC_GENERIC_ERROR] = icf_cc_12_generic_err;
    p_cc_data->cc_fsm_table[ICF_CC_12_WAIT_START_PROC_SIG]
        [ICF_CC_FORCEFUL_CLEAR] = icf_cc_06_forceful_call_clear;
ICF_HANDLE_REINVITE_START
    p_cc_data->cc_fsm_table[ICF_CC_12_WAIT_START_PROC_SIG]
        [ICF_CC_SSA_CONNECT_ACK] = icf_cc_12_ssa_connect_ack;
	p_cc_data->cc_fsm_table[ICF_CC_12_WAIT_START_PROC_SIG]
        [ICF_CC_PA_CALL_MODIFY_REQ] = icf_cc_12_pa_call_modify_req;
	p_cc_data->cc_fsm_table[ICF_CC_12_WAIT_START_PROC_SIG]
        [ICF_CC_SSA_REINVITE_ACK] = icf_cc_12_start_call_proc_sig;
    p_cc_data->cc_fsm_table[ICF_CC_12_WAIT_START_PROC_SIG]
        [ICF_CC_MEDIA_UPDATE_RESP] = icf_cc_12_start_call_proc_sig;
ICF_HANDLE_REINVITE_END
#ifdef ICF_FAX_SUPPORT
    p_cc_data->cc_fsm_table[ICF_CC_12_WAIT_START_PROC_SIG]
		[ICF_CC_PA_FAX_REQ] = icf_cc_14_pa_fax_req;
    p_cc_data->cc_fsm_table[ICF_CC_12_WAIT_START_PROC_SIG]
		[ICF_CC_SSA_FAX_REQ] = icf_cc_14_ssa_fax_req;
    p_cc_data->cc_fsm_table[ICF_CC_12_WAIT_START_PROC_SIG]
        [ICF_CC_SSA_STOP_FAX_RESP] = icf_cc_12_start_call_proc_sig;	
#endif
    p_cc_data->cc_fsm_table[ICF_CC_12_WAIT_START_PROC_SIG]
     [ICF_CC_SSA_CONNECT_ACK] = icf_cc_12_ssa_connect_ack;

    /* Added for REFER/NOTIFY Enhancements */
    p_cc_data->cc_fsm_table[ICF_CC_12_WAIT_START_PROC_SIG]
        [ICF_CC_PA_GENERIC_MSG_CFM] = icf_cc_xx_pa_generic_msg_cfm;
    p_cc_data->cc_fsm_table[ICF_CC_12_WAIT_START_PROC_SIG]
        [ICF_CC_SSA_GENERIC_MSG_RESP] = icf_cc_xx_ssa_generic_msg_resp;
    p_cc_data->cc_fsm_table[ICF_CC_12_WAIT_START_PROC_SIG]
        [ICF_CC_SSA_GENERIC_MSG_IND] = icf_cc_xx_ssa_generic_msg_ind;
    p_cc_data->cc_fsm_table[ICF_CC_12_WAIT_START_PROC_SIG]
        [ICF_CC_PA_GENERIC_MSG_REQ] = icf_cc_xx_pa_generic_msg_req;

    /* State ICF_CC_13_WAIT_START_PROC_MEDIA */
    p_cc_data->cc_fsm_table[ICF_CC_13_WAIT_START_PROC_MEDIA]
        [ICF_CC_SSA_ALERT] = icf_cc_13_ssa_alert;
    p_cc_data->cc_fsm_table[ICF_CC_13_WAIT_START_PROC_MEDIA]
        [ICF_CC_SSA_ALERT_WITH_MEDIA] = icf_cc_13_ssa_alrt_with_media;
    p_cc_data->cc_fsm_table[ICF_CC_13_WAIT_START_PROC_MEDIA]
        [ICF_CC_SSA_CALL_CONNECT] = icf_cc_13_ssa_call_connect;
    p_cc_data->cc_fsm_table[ICF_CC_13_WAIT_START_PROC_MEDIA]
        [ICF_CC_START_CALL_PROC_MEDIA] = icf_cc_13_start_proc_media;
    p_cc_data->cc_fsm_table[ICF_CC_13_WAIT_START_PROC_MEDIA]
        [ICF_CC_SSA_CALL_TERMINATE_REQ] = icf_cc_13_ssa_call_term_req;
    p_cc_data->cc_fsm_table[ICF_CC_13_WAIT_START_PROC_MEDIA]
        [ICF_CC_SSA_REMOTE_REJECT] = icf_cc_13_ssa_remote_reject;
    p_cc_data->cc_fsm_table[ICF_CC_13_WAIT_START_PROC_MEDIA]
        [ICF_CC_SSA_REDIRECT] = icf_cc_13_ssa_redirect;
    p_cc_data->cc_fsm_table[ICF_CC_13_WAIT_START_PROC_MEDIA]
        [ICF_CC_PA_CALL_TERMINATE_REQ] = icf_cc_13_pa_call_term_req;
    p_cc_data->cc_fsm_table[ICF_CC_13_WAIT_START_PROC_MEDIA]
        [ICF_CC_SSA_OFFER_CHANGE] = icf_cc_13_ssa_offer_change;
    p_cc_data->cc_fsm_table[ICF_CC_13_WAIT_START_PROC_MEDIA]
        [ICF_CC_MM_ERR_IND] = icf_cc_13_mm_error_ind;
    p_cc_data->cc_fsm_table[ICF_CC_13_WAIT_START_PROC_MEDIA]
        [ICF_CC_TIMER_EXPIRY] = icf_cc_13_timer_expiry;
    p_cc_data->cc_fsm_table[ICF_CC_13_WAIT_START_PROC_MEDIA]
        [ICF_CC_PA_CALL_CONNECT] = icf_cc_04_pa_call_connect;
    p_cc_data->cc_fsm_table[ICF_CC_13_WAIT_START_PROC_MEDIA]
        [ICF_CC_ALERT_PRACK] = icf_cc_13_alert_prack;
    p_cc_data->cc_fsm_table[ICF_CC_13_WAIT_START_PROC_MEDIA]
        [ICF_CC_SSA_CONNECT_ACK] = icf_cc_13_ssa_connect_ack;
    p_cc_data->cc_fsm_table[ICF_CC_13_WAIT_START_PROC_MEDIA]
        [ICF_CC_SSA_CALL_CANCEL_REQ] = icf_cc_13_ssa_call_cancel_req;
    p_cc_data->cc_fsm_table[ICF_CC_13_WAIT_START_PROC_MEDIA]
        [ICF_CC_GENERIC_ERROR] = icf_cc_13_generic_err;
    p_cc_data->cc_fsm_table[ICF_CC_13_WAIT_START_PROC_MEDIA]
        [ICF_CC_FORCEFUL_CLEAR] = icf_cc_04_forceful_call_clear;
ICF_HANDLE_REINVITE_START
	p_cc_data->cc_fsm_table[ICF_CC_13_WAIT_START_PROC_MEDIA]
        [ICF_CC_PA_CALL_MODIFY_REQ] = icf_cc_13_pa_call_modify_req;
ICF_HANDLE_REINVITE_END
#ifdef ICF_FAX_SUPPORT
    p_cc_data->cc_fsm_table[ICF_CC_13_WAIT_START_PROC_MEDIA]
		[ICF_CC_PA_FAX_REQ] = icf_cc_14_pa_fax_req;
    p_cc_data->cc_fsm_table[ICF_CC_13_WAIT_START_PROC_MEDIA]
		[ICF_CC_SSA_FAX_REQ] = icf_cc_14_ssa_fax_req;
#endif
    p_cc_data->cc_fsm_table[ICF_CC_13_WAIT_START_PROC_MEDIA]
        [ICF_CC_SSA_CALL_PROGRESS] = icf_cc_03_ssa_call_progress;
    p_cc_data->cc_fsm_table[ICF_CC_13_WAIT_START_PROC_MEDIA]
        [ICF_CC_SRTP_FALLBACK_REQ] = icf_cc_13_ssa_remote_reject;

    /* Added for REFER/NOTIFY Enhancements */
    p_cc_data->cc_fsm_table[ICF_CC_13_WAIT_START_PROC_MEDIA]
        [ICF_CC_PA_GENERIC_MSG_CFM] = icf_cc_xx_pa_generic_msg_cfm;
    p_cc_data->cc_fsm_table[ICF_CC_13_WAIT_START_PROC_MEDIA]
        [ICF_CC_SSA_GENERIC_MSG_RESP] = icf_cc_xx_ssa_generic_msg_resp;
    p_cc_data->cc_fsm_table[ICF_CC_13_WAIT_START_PROC_MEDIA]
        [ICF_CC_SSA_GENERIC_MSG_IND] = icf_cc_xx_ssa_generic_msg_ind;
    p_cc_data->cc_fsm_table[ICF_CC_13_WAIT_START_PROC_MEDIA]
        [ICF_CC_PA_GENERIC_MSG_REQ] = icf_cc_xx_pa_generic_msg_req;

ICF_HANDLE_REINVITE_START

	/* State ICF_CC_14_AWT_CALL_MODIFY_RESP */
	p_cc_data->cc_fsm_table[ICF_CC_14_AWT_CALL_MODIFY_RESP]
		[ICF_CC_PA_CALL_MODIFY_RSP] = icf_cc_14_pa_call_modify_resp;
	p_cc_data->cc_fsm_table[ICF_CC_14_AWT_CALL_MODIFY_RESP]
		[ICF_CC_MEDIA_UPDATE_RESP] = icf_cc_14_ssa_call_modify_resp;
	p_cc_data->cc_fsm_table[ICF_CC_14_AWT_CALL_MODIFY_RESP]
		[ICF_CC_GENERIC_ERROR] = icf_cc_14_generic_err;
	p_cc_data->cc_fsm_table[ICF_CC_14_AWT_CALL_MODIFY_RESP]
		[ICF_CC_FORCEFUL_CLEAR] = icf_cc_14_forceful_clear;
	p_cc_data->cc_fsm_table[ICF_CC_14_AWT_CALL_MODIFY_RESP]
		[ICF_CC_TIMER_EXPIRY] = icf_cc_14_timer_expiry;
	p_cc_data->cc_fsm_table[ICF_CC_14_AWT_CALL_MODIFY_RESP]
		[ICF_CC_PA_CALL_TERMINATE_REQ] = icf_cc_14_pa_call_term_req;
	p_cc_data->cc_fsm_table[ICF_CC_14_AWT_CALL_MODIFY_RESP]
		[ICF_CC_SSA_CALL_TERMINATE_REQ] = icf_cc_14_ssa_call_term_req;
	p_cc_data->cc_fsm_table[ICF_CC_14_AWT_CALL_MODIFY_RESP]
		[ICF_CC_SSA_OFFER_CHANGE] = icf_cc_14_ssa_offer_change;
	p_cc_data->cc_fsm_table[ICF_CC_14_AWT_CALL_MODIFY_RESP]
		[ICF_CC_SSA_CALL_CANCEL_REQ] = icf_cc_14_ssa_call_cancel_req;
	p_cc_data->cc_fsm_table[ICF_CC_14_AWT_CALL_MODIFY_RESP]
		[ICF_CC_MM_ERR_IND] = icf_cc_14_mm_error_ind;

#ifdef ICF_FAX_SUPPORT
    p_cc_data->cc_fsm_table[ICF_CC_14_AWT_CALL_MODIFY_RESP]
		[ICF_CC_SSA_FAX_RESP] = icf_cc_14_ssa_fax_resp;
    p_cc_data->cc_fsm_table[ICF_CC_14_AWT_CALL_MODIFY_RESP]
		[ICF_CC_PA_FAX_REQ] = icf_cc_14_pa_fax_req;
    p_cc_data->cc_fsm_table[ICF_CC_14_AWT_CALL_MODIFY_RESP]
		[ICF_CC_SSA_FAX_REQ] = icf_cc_14_ssa_fax_req;
    p_cc_data->cc_fsm_table[ICF_CC_14_AWT_CALL_MODIFY_RESP]
		[ICF_CC_PA_STOP_FAX_REQ] = icf_cc_14_pa_stop_fax_req;
    /*  CSR 1-5737059 Merge */
    /* SPR 18198: Mapping of stop fax response from ssa when method
     * is UPDATE.
     */
    p_cc_data->cc_fsm_table[ICF_CC_14_AWT_CALL_MODIFY_RESP]
        [ICF_CC_SSA_STOP_FAX_RESP] = icf_cc_14_ssa_stop_fax_resp;
#endif

    /*call_mod_cancel*/
    p_cc_data->cc_fsm_table[ICF_CC_14_AWT_CALL_MODIFY_RESP]
		[ICF_CC_PA_CALL_MODIFY_CANCEL_REQ] = icf_cc_14_pa_call_modify_cancel_req;

    p_cc_data->cc_fsm_table[ICF_CC_14_AWT_CALL_MODIFY_RESP]
		[ICF_CC_SSA_CALL_MODIFY_CANCEL_RESP] = icf_cc_14_ssa_call_modify_cancel_resp;

	/* fix for spr 13900 */

	p_cc_data->cc_fsm_table[ICF_CC_14_AWT_CALL_MODIFY_RESP]
        [ICF_CC_PA_CALL_MODIFY_REQ] = icf_cc_14_pa_call_modify_req;

    /* Added for REFER/NOTIFY Enhancements */
    p_cc_data->cc_fsm_table[ICF_CC_14_AWT_CALL_MODIFY_RESP]
        [ICF_CC_PA_GENERIC_MSG_CFM] = icf_cc_xx_pa_generic_msg_cfm;
    p_cc_data->cc_fsm_table[ICF_CC_14_AWT_CALL_MODIFY_RESP]
        [ICF_CC_SSA_GENERIC_MSG_RESP] = icf_cc_xx_ssa_generic_msg_resp;
    p_cc_data->cc_fsm_table[ICF_CC_14_AWT_CALL_MODIFY_RESP]
        [ICF_CC_SSA_GENERIC_MSG_IND] = icf_cc_xx_ssa_generic_msg_ind;
    p_cc_data->cc_fsm_table[ICF_CC_14_AWT_CALL_MODIFY_RESP]
        [ICF_CC_PA_GENERIC_MSG_REQ] = icf_cc_xx_pa_generic_msg_req;

	/* State ICF_CC_15_AWT_CALL_MODIFY_ALERT_RESP */
	p_cc_data->cc_fsm_table[ICF_CC_15_AWT_CALL_MODIFY_ALERT_RESP]
		[ICF_CC_PA_ALERT] = icf_cc_15_pa_alert;
	p_cc_data->cc_fsm_table[ICF_CC_15_AWT_CALL_MODIFY_ALERT_RESP]
		[ICF_CC_SSA_ALERT] = icf_cc_15_ssa_alert;
	p_cc_data->cc_fsm_table[ICF_CC_15_AWT_CALL_MODIFY_ALERT_RESP]
		[ICF_CC_SSA_ALERT_WITH_MEDIA] = icf_cc_15_ssa_alert_with_media;
	p_cc_data->cc_fsm_table[ICF_CC_15_AWT_CALL_MODIFY_ALERT_RESP]
		[ICF_CC_PA_CALL_MODIFY_RSP] = icf_cc_14_pa_call_modify_resp;
	p_cc_data->cc_fsm_table[ICF_CC_15_AWT_CALL_MODIFY_ALERT_RESP]
		[ICF_CC_MEDIA_UPDATE_RESP] = icf_cc_14_ssa_call_modify_resp;
	p_cc_data->cc_fsm_table[ICF_CC_15_AWT_CALL_MODIFY_ALERT_RESP]
		[ICF_CC_SSA_OFFER_CHANGE] = icf_cc_14_ssa_offer_change;
	p_cc_data->cc_fsm_table[ICF_CC_15_AWT_CALL_MODIFY_ALERT_RESP]
		[ICF_CC_GENERIC_ERROR] = icf_cc_14_generic_err;
	p_cc_data->cc_fsm_table[ICF_CC_15_AWT_CALL_MODIFY_ALERT_RESP]
		[ICF_CC_FORCEFUL_CLEAR] = icf_cc_14_forceful_clear;
	p_cc_data->cc_fsm_table[ICF_CC_15_AWT_CALL_MODIFY_ALERT_RESP]
		[ICF_CC_TIMER_EXPIRY] = icf_cc_14_timer_expiry;
	p_cc_data->cc_fsm_table[ICF_CC_15_AWT_CALL_MODIFY_ALERT_RESP]
		[ICF_CC_PA_CALL_TERMINATE_REQ] = icf_cc_14_pa_call_term_req;
	p_cc_data->cc_fsm_table[ICF_CC_15_AWT_CALL_MODIFY_ALERT_RESP]
		[ICF_CC_SSA_CALL_TERMINATE_REQ] = icf_cc_14_ssa_call_term_req;
    p_cc_data->cc_fsm_table[ICF_CC_15_AWT_CALL_MODIFY_ALERT_RESP]
        [ICF_CC_PA_CALL_MODIFY_REQ] = icf_cc_14_pa_call_modify_req;
    p_cc_data->cc_fsm_table[ICF_CC_15_AWT_CALL_MODIFY_ALERT_RESP]
	    [ICF_CC_MM_ERR_IND] = icf_cc_14_mm_error_ind; 
    p_cc_data->cc_fsm_table[ICF_CC_15_AWT_CALL_MODIFY_ALERT_RESP]
        [ICF_CC_SSA_STOP_FAX_REQ] = icf_cc_14_ssa_stop_fax_req;

#ifdef ICF_FAX_SUPPORT
    p_cc_data->cc_fsm_table[ICF_CC_15_AWT_CALL_MODIFY_ALERT_RESP]
		[ICF_CC_SSA_FAX_RESP] = icf_cc_14_ssa_fax_resp;
    p_cc_data->cc_fsm_table[ICF_CC_15_AWT_CALL_MODIFY_ALERT_RESP]
		[ICF_CC_PA_FAX_REQ] = icf_cc_14_pa_fax_req;
    p_cc_data->cc_fsm_table[ICF_CC_15_AWT_CALL_MODIFY_ALERT_RESP]
		[ICF_CC_SSA_FAX_REQ] = icf_cc_14_ssa_fax_req;
    p_cc_data->cc_fsm_table[ICF_CC_15_AWT_CALL_MODIFY_ALERT_RESP]
	    [ICF_CC_PA_STOP_FAX_REQ] = icf_cc_14_pa_stop_fax_req;
    p_cc_data->cc_fsm_table[ICF_CC_15_AWT_CALL_MODIFY_ALERT_RESP]
		[ICF_CC_SSA_CALL_CANCEL_RESP] = icf_cc_14_ssa_stop_fax_resp;
	p_cc_data->cc_fsm_table[ICF_CC_15_AWT_CALL_MODIFY_ALERT_RESP]
        [ICF_CC_SSA_STOP_FAX_RESP] = icf_cc_14_ssa_stop_fax_resp;
#endif

    /*call_mod_cancel*/
    p_cc_data->cc_fsm_table[ICF_CC_15_AWT_CALL_MODIFY_ALERT_RESP]
		[ICF_CC_PA_CALL_MODIFY_CANCEL_REQ] = icf_cc_14_pa_call_modify_cancel_req;

    p_cc_data->cc_fsm_table[ICF_CC_15_AWT_CALL_MODIFY_ALERT_RESP]
		[ICF_CC_SSA_CALL_MODIFY_CANCEL_RESP] = icf_cc_14_ssa_call_modify_cancel_resp;

    /*cancel_reinv*/
    p_cc_data->cc_fsm_table[ICF_CC_15_AWT_CALL_MODIFY_ALERT_RESP]
		[ICF_CC_SSA_CALL_CANCEL_REQ] = icf_cc_14_ssa_call_cancel_req;

    p_cc_data->cc_fsm_table[ICF_CC_15_AWT_CALL_MODIFY_ALERT_RESP]
        [ICF_CC_SSA_CALL_PROGRESS] = icf_cc_03_ssa_call_progress;

    p_cc_data->cc_fsm_table[ICF_CC_15_AWT_CALL_MODIFY_ALERT_RESP]
        [ICF_CC_MEDIA_PARAM_PRACK] = icf_cc_04_media_prack;

    p_cc_data->cc_fsm_table[ICF_CC_15_AWT_CALL_MODIFY_ALERT_RESP]
        [ICF_CC_ALERT_PRACK] = icf_cc_04_alert_prack;

    /* Added for REFER/NOTIFY Enhancements */
    p_cc_data->cc_fsm_table[ICF_CC_15_AWT_CALL_MODIFY_ALERT_RESP]
        [ICF_CC_PA_GENERIC_MSG_CFM] = icf_cc_xx_pa_generic_msg_cfm;
    p_cc_data->cc_fsm_table[ICF_CC_15_AWT_CALL_MODIFY_ALERT_RESP]
        [ICF_CC_SSA_GENERIC_MSG_RESP] = icf_cc_xx_ssa_generic_msg_resp;
    p_cc_data->cc_fsm_table[ICF_CC_15_AWT_CALL_MODIFY_ALERT_RESP]
        [ICF_CC_SSA_GENERIC_MSG_IND] = icf_cc_xx_ssa_generic_msg_ind;
    p_cc_data->cc_fsm_table[ICF_CC_15_AWT_CALL_MODIFY_ALERT_RESP]
        [ICF_CC_PA_GENERIC_MSG_REQ] = icf_cc_xx_pa_generic_msg_req;

ICF_HANDLE_REINVITE_END

#ifdef ICF_QOS_SUPPORT
	/* State ICF_CC_16_AWT_UPDATE */
	p_cc_data->cc_fsm_table[ICF_CC_16_AWT_UPDATE]
		[ICF_CC_SSA_OFFER_CHANGE] = icf_cc_16_ssa_offer_change;
	p_cc_data->cc_fsm_table[ICF_CC_16_AWT_UPDATE]
		[ICF_CC_MM_ERR_IND] = icf_cc_16_mm_err_ind;
	p_cc_data->cc_fsm_table[ICF_CC_16_AWT_UPDATE]
		[ICF_CC_TIMER_EXPIRY] = icf_cc_16_timer_expiry;
	p_cc_data->cc_fsm_table[ICF_CC_16_AWT_UPDATE]
		[ICF_CC_SSA_CALL_CANCEL_REQ] = icf_cc_16_ssa_call_cancel_req;
	p_cc_data->cc_fsm_table[ICF_CC_16_AWT_UPDATE]
		[ICF_CC_GENERIC_ERROR] = icf_cc_16_generic_err;
	p_cc_data->cc_fsm_table[ICF_CC_16_AWT_UPDATE]
		[ICF_CC_FORCEFUL_CLEAR] = icf_cc_16_forceful_call_clear;
    
    /* Added for REFER/NOTIFY Enhancements */
    p_cc_data->cc_fsm_table[ICF_CC_16_AWT_UPDATE]
        [ICF_CC_PA_GENERIC_MSG_REQ] = icf_cc_xx_pa_generic_msg_req;
    p_cc_data->cc_fsm_table[ICF_CC_16_AWT_UPDATE]
        [ICF_CC_PA_GENERIC_MSG_CFM] = icf_cc_xx_pa_generic_msg_cfm;
    p_cc_data->cc_fsm_table[ICF_CC_16_AWT_UPDATE]
        [ICF_CC_PA_ALERT] = icf_cc_16_pa_alert;
    p_cc_data->cc_fsm_table[ICF_CC_16_AWT_UPDATE]
        [ICF_CC_PA_CALL_CONNECT] = icf_cc_16_pa_call_connect;
    p_cc_data->cc_fsm_table[ICF_CC_16_AWT_UPDATE]
        [ICF_CC_PA_CALL_MODIFY_REQ] = icf_cc_10_pa_call_modify_req;
    p_cc_data->cc_fsm_table[ICF_CC_16_AWT_UPDATE]
        [ICF_CC_MEDIA_UPDATE_RESP]  = icf_cc_14_ssa_call_modify_resp;
    p_cc_data->cc_fsm_table[ICF_CC_16_AWT_UPDATE]
        [ICF_CC_PA_MEDIA_CONNECT_REQ] =  icf_cc_16_pa_media_connect_req;
    p_cc_data->cc_fsm_table[ICF_CC_16_AWT_UPDATE]
        [ICF_CC_PA_CALL_TERMINATE_REQ] = icf_cc_05_pa_call_term_req;

#endif
     /* State ICF_CC_20_AWT_EARLY_IND_RESP */
    p_cc_data->cc_fsm_table[ICF_CC_20_AWT_EARLY_IND_CFM]
        [ICF_CC_PA_EARLY_CALL_CFM] = icf_cc_20_early_ind_cfm;
    p_cc_data->cc_fsm_table[ICF_CC_20_AWT_EARLY_IND_CFM]
        [ICF_CC_SSA_CALL_CANCEL_REQ] = icf_cc_20_ssa_call_cancel_req;
    p_cc_data->cc_fsm_table[ICF_CC_20_AWT_EARLY_IND_CFM]
        [ICF_CC_GENERIC_ERROR] = icf_cc_20_generic_err;
    p_cc_data->cc_fsm_table[ICF_CC_20_AWT_EARLY_IND_CFM]
        [ICF_CC_FORCEFUL_CLEAR] = icf_cc_20_forceful_call_clear;
    p_cc_data->cc_fsm_table[ICF_CC_20_AWT_EARLY_IND_CFM]
        [ICF_CC_TIMER_EXPIRY] = icf_cc_20_timer_expiry;
    p_cc_data->cc_fsm_table[ICF_CC_20_AWT_EARLY_IND_CFM]
        [ICF_CC_PA_CALL_TERMINATE_REQ] = icf_cc_20_pa_call_term_req;
    p_cc_data->cc_fsm_table[ICF_CC_20_AWT_EARLY_IND_CFM]
        [ICF_CC_SSA_SETUP_REQ] = icf_cc_20_early_ind_cfm;

    /* Assign fsm table pointer to global data pointer maintained by DBM */
    *p_p_cc_data = (icf_void_t *)p_cc_data;
    
    ICF_ASSERT(p_glb_pdb, (ICF_NULL != p_p_cc_data))

    ICF_FUNCTION_EXIT(p_glb_pdb) /* function exit trace */
    return ret_val;
} /* end of icf_cc_init() */

/*******************************************************************************
 *
 * FUNCTION:        icf_cc_deinit
 *
 * DESCRIPTION:     This function would free the modules global data
 *
 ******************************************************************************/
icf_void_t 
icf_cc_deinit(
        INOUT   icf_glb_pdb_st        *p_glb_pdb,
        OUT     icf_void_t            *p_cc_data,
        OUT     icf_error_t           *p_ecode)
{
    icf_return_t ret_val = ICF_SUCCESS;
    ICF_FUNCTION_ENTER(p_glb_pdb) /* function entry trace */

    /* Dellocate memory for the modules global data*/
    ICF_STATIC_MEMFREE(p_glb_pdb,p_cc_data,p_ecode, ret_val)

    ICF_FUNCTION_EXIT(p_glb_pdb) /* function exit trace */

} /* end of icf_cc_init() */


/*******************************************************************************
 *
 * FUNCTION:        icf_cc_process_mesg
 *
 * DESCRIPTION:     This is the entry point function of CC that is invoked by
 *                  Call Manager. The function checks the API Id in the message
 *                  received and invokes appropriate handler based on current
 *                  call state and the event (mapped from API Id).
 *
 ******************************************************************************/
icf_return_t icf_cc_process_mesg(
        INOUT   icf_internal_msg_st   *p_internal_msg)
{
    icf_return_t                     ret_val = ICF_SUCCESS;
    icf_cc_glb_data_st               *p_cc_data = ICF_NULL;
    icf_glb_pdb_st                   *p_glb_pdb = ICF_NULL;
    icf_cc_pdb_st                    cc_pdb;
    icf_uint32_t                     status = ICF_NULL;
    icf_timer_data_st                *p_timer_data = ICF_NULL;

    ICF_SET_MODULE(p_internal_msg->msg_hdr.p_glb_pdb, ICF_INT_MODULE_CC)

#ifdef ICF_UT_TEST
    /* p_ecode declared in icf_glb_pdb_st will point to 
       ecode in icf_internal_msg_st. To change this ecode, 
       value of pointer will be changed */
    p_internal_msg->msg_hdr.p_glb_pdb->p_ecode = 
	&(p_internal_msg->msg_hdr.ecode);
#endif

    ICF_FUNCTION_ENTER(p_internal_msg->msg_hdr.p_glb_pdb)

    /* copy global pdb pointer locally */
    p_glb_pdb = p_internal_msg->msg_hdr.p_glb_pdb;
    ICF_ASSERT(p_glb_pdb, (ICF_NULL != p_glb_pdb))

    cc_pdb.p_glb_pdb = p_glb_pdb;

    /* copy ecode pointer locally */
    cc_pdb.p_ecode = &(p_internal_msg->msg_hdr.ecode);
    ICF_ASSERT(p_glb_pdb, (ICF_NULL != cc_pdb.p_ecode))

    /* copy internal message pointer to CC pdb */
    cc_pdb.p_internal_msg = p_internal_msg;

    /* copy current call context pointer to CC pdb */
    cc_pdb.p_call_ctx = p_glb_pdb->p_call_ctx;
 
    /* Fix for CSR 1-6779001: SPR 19258 */
    /* invoke function that initializes other members of CC pdb */
    if (ICF_FAILURE == icf_cc_init_pdb(&cc_pdb))
	{
		ICF_PRINT(("\n[CC]: Failed to init pdb at entry\n"));
		ret_val = ICF_FAILURE;
		ICF_FUNCTION_EXIT(p_glb_pdb)
		return ret_val;
	}
    /* Get pointer to config data from DBM */
    else if (ICF_FAILURE == icf_dbm_get_module_glb_data(
                				p_glb_pdb,
                				(icf_int_module_id_t)ICF_INT_MODULE_CFG,
                				(icf_void_t *)&(cc_pdb.p_config_data),
								cc_pdb.p_ecode))
	{
		ICF_PRINT(("\n[CC]: Failed to get config data at entry\n"));
		ret_val = ICF_FAILURE;
		ICF_FUNCTION_EXIT(p_glb_pdb)
		return ret_val;
	} 
    /*
     * In case this is a timer expiry event, extract the call context pointer
     * from timer data buffer
     */
    if (ICF_TIMER_EXPIRY_IND == p_internal_msg->msg_hdr.msg_id)
    {
        p_timer_data = *((icf_timer_data_st **)
                    (p_glb_pdb->p_recv_msg->payload));

        p_glb_pdb->p_call_ctx = (icf_call_ctx_st *)
            ((*((icf_timer_data_st **)
                    (p_glb_pdb->p_recv_msg->payload)))->p_timer_buffer);


        ICF_ASSERT(p_glb_pdb, (ICF_NULL != p_glb_pdb->p_call_ctx))

        cc_pdb.p_call_ctx = p_glb_pdb->p_call_ctx;

        /*
        Now call this function to do validation of the timer block 
        pointers. We have extracted p_call_ctx from the timer expiry
        that is being received from MRM. This p_call_ctx also contains
        a pointer to the timer block that is running currently.These two
        pointers should point to the same timer block.We should handle the
        timer expiry received from MRM only if both the pointers are equal
        i.e. they point to the same timer block.If the pointers are
        different, we will free the timer block received from MRM. We will
        not free the timer block in p_call_ctx as its timer expiry will
        happen sometime in future. On receiving the same from MRM,the
        pointers would match and thus the timer expiry would be handled.
        */  
        if(ICF_FAILURE == icf_cc_is_valid_timer_blk(&cc_pdb,p_timer_data))
        {
            ICF_PRINT(((icf_uint8_t *)"[CC]:Invalid timer expiry received.\n"))

            /* Pointers did not match.Free the timer block received from MRM */
            if(ICF_NULL != p_timer_data)
            {
                ret_val = icf_dbm_free_timer_block(p_glb_pdb,
                          (icf_void_t *)(p_timer_data),cc_pdb.p_ecode); 
                if(ICF_SUCCESS == ret_val)
                {
                    p_timer_data = ICF_NULL;
                }
                /* Timer block freeing failure. Raise a alarm and proceed. */
                else
                {
                    ICF_PRINT(((icf_uint8_t *)"[CC]: Failure in freeing timer block.\n"))
                }
            }
            ret_val = ICF_FAILURE;
            ICF_FUNCTION_EXIT(p_glb_pdb)
            return ICF_FAILURE;
        }

        /* Valid timer expiry received. Proceed to handle it */

#ifdef ICF_TRACE_ENABLE
        /* Added for sending traces according to timer type */
        icf_cc_trace_timer_type(&cc_pdb,
                                p_timer_data->timer_type); 
#endif
        /*
         * Check if the timer which has expirew was ALERTING timer
         */
        if(ICF_TIMER_TYPE_INFO == p_timer_data->timer_type)
        {
            cc_pdb.p_glb_pdb = p_glb_pdb;
            cc_pdb.p_call_ctx = p_glb_pdb->p_call_ctx;
            /*set error cause in cc_pdb*/
            cc_pdb.p_call_ctx->app_failure_cause = ICF_CAUSE_TIMER_EXPIRY;
            ret_val = icf_cc_send_pa_info_resp(&cc_pdb);

            /* Reset the failure cause to ICF_NULL*/
            cc_pdb.p_call_ctx->app_failure_cause = ICF_NULL;

            return ICF_SUCCESS;
        }
        else if(ICF_TIMER_TYPE_INFO_CFM == p_timer_data->timer_type)
        {
           cc_pdb.p_glb_pdb = p_glb_pdb;
           cc_pdb.p_call_ctx = p_glb_pdb->p_call_ctx; 

           cc_pdb.p_call_ctx->pending_info_cfm = ICF_FALSE;

           /* Timer expired is Info confirmation timer. so, send 4xx response on 
            * the n/w for INFO received.
            */
           cc_pdb.p_call_ctx->info_failure_cause = ICF_CAUSE_TIMER_EXPIRY;
           ret_val = icf_cc_send_ssa_info_cfm_err(&cc_pdb);
           return ret_val;
           
        }
        /*CSR-1-6087284 Merge*/ 
        /*Since the data for retry_after timer is stored at a separate place
          in call context and retry after timer expiry would be taken care
          by FSM ignore the same here .*/
        else if(ICF_TIMER_TYPE_RETRY_AFTER == p_timer_data->timer_type)
        {
            /* 
             * Do Nothing. The handling for retry after timer expiry
             * would be taken care by FSM
            */
        }
#ifdef ICF_LOAD_FORCEFUL_CALL_CLEAR
        else if(ICF_TIMER_TYPE_LOAD_CALL_DURATION == p_timer_data->timer_type)
        {
           cc_pdb.p_glb_pdb = p_glb_pdb;
           cc_pdb.p_call_ctx = p_glb_pdb->p_call_ctx; 

           icf_dbm_ret_mem_to_pool(p_glb_pdb,
                (icf_pool_id_t)ICF_MEM_TIMER,
                 cc_pdb.p_call_ctx->p_load_timer_data, cc_pdb.p_ecode);
            cc_pdb.p_call_ctx->p_load_timer_data = ICF_NULL;
            cc_pdb.p_call_ctx->load_call_dur_timer_id = ICF_NULL;

           cc_pdb.p_glb_pdb->load_data.load_force_cleared_calls++;
           return ICF_FAILURE;
           
        } 
#endif 
        /*
         * Check if the timer which has expirew was ALERTING timer
         */

        /* Fix for SPR: 19886 (Merged CSR 1-7480178)*/
		/* Fix for CSR 1-7480178: SPR 19827 */
		/* DBM fn to check service status on this line should be invoked only
		 * for inc call and not for outgoing call. Services like CFU, CFNA etc
		 * are applicable only for incoming call.
		 */
        else if ((ICF_TIMER_TYPE_ALERTING == ((icf_timer_data_st *)
                    (p_glb_pdb->p_call_ctx->p_timer_data))->timer_type) &&
			(ICF_NULL != (cc_pdb.p_call_ctx->common_bitmask & ICF_CALL_DIR)))        
        {
            /*
             * If niether check line service returns failure  nor status is 
             * returned as ICF_STATUS_CALL_R_SRV_TERMINATE. Return the 
             * status to CM in the status flag in internal message.
             */ 
            if(ICF_FAILURE != icf_dbm_chk_line_service(
                        p_glb_pdb,
                        (icf_uint8_t)ICF_CAUSE_NO_ANSWER,
                        p_glb_pdb->p_call_ctx->line_id,
                        &status,
                        cc_pdb.p_ecode)) 
            {
                if(ICF_STATUS_CALL_R_SRV_TERMINATE != status)
                {
                    cc_pdb.p_internal_msg->msg_hdr.status = status;
                    ICF_FUNCTION_EXIT(p_glb_pdb) /* function exit trace */
                    ICF_RESET_MODULE(p_glb_pdb)
                    return ICF_SUCCESS;
                }/* End if(ICF_STATUS_CALL_R_SRV_TERMINATE != ... */
            }/* End if(ICF_FAILURE != ipps... */
        }/* End else if(ICF_TIMER_TYPE_ALERTING == ... */
    }/* End if (ICF_TIMER_EXPIRY_IND == p...*/


    /* copy global pdb pointer to CC pdb */
    cc_pdb.p_glb_pdb = p_glb_pdb;
    ICF_ASSERT(p_glb_pdb, (ICF_NULL != cc_pdb.p_glb_pdb))

     /*
      *Check to see if any API from PA is other than ICF_USER_CREATE_CALL has
      *come with call context NULL in global PDB
      */
    if(
		( (ICF_NULL == p_glb_pdb->p_call_ctx) && 
          (ICF_USER_CREATE_CALL != p_internal_msg->msg_hdr.msg_id) 
        )
		||
		( (ICF_NULL == p_glb_pdb->p_call_ctx) && 
          (ICF_FORCEFUL_CALL_CLEAR_IND == p_internal_msg->msg_hdr.msg_id)
        )
  	  )
    {
        ICF_ASSERT(p_glb_pdb,p_glb_pdb->p_recv_msg);
#if 0
  	if (ICF_TERMINATE_CALL_REQ == 
            p_internal_msg->msg_hdr.p_glb_pdb->p_recv_msg->hdr.api_id)
        {
           /* Send Call Terminate Response to PA */
           ret_val = icf_cc_send_pa_call_term_resp_invalid_callid(&cc_pdb);
        }
       	else if (ICF_START_FAX_REQ == 
            p_internal_msg->msg_hdr.p_glb_pdb->p_recv_msg->hdr.api_id)
        {
           /* Send Start Fax failure Response to PA */
           ret_val = icf_cc_send_pa_fax_resp_invalid_callid(&cc_pdb);
        }
#endif
        if (ICF_USER_CALL_TERMINATE ==
            p_internal_msg->msg_hdr.msg_id)
        {
           /* Send Call Terminate Response to PA */
           ret_val = icf_cc_send_pa_call_term_resp_invalid_callid(&cc_pdb);
        }
        else if (ICF_USER_START_FAX_REQ ==
            p_internal_msg->msg_hdr.msg_id)
        {
           /* Send Start Fax failure Response to PA */
           ret_val = icf_cc_send_pa_fax_resp_invalid_callid(&cc_pdb);
        }

        ICF_FUNCTION_EXIT(p_glb_pdb) /* function exit trace */
        ICF_RESET_MODULE(p_glb_pdb)

        return ICF_SUCCESS;
        
    }
    

    /* Map the incoming api to corresponding event */
    cc_pdb.current_event = ICF_CC_INVALID_EVENT;
    if (ICF_SUCCESS == ret_val &&
            ICF_FAILURE == icf_cc_convert_api_to_event(&cc_pdb))
    {
        ret_val = ICF_FAILURE;
    }

    /*
     * In case this is a timer expiry event, extract the call context pointer
     * from timer data buffer
     */
    else if (ICF_USER_CREATE_CALL == p_internal_msg->msg_hdr.msg_id)
    {

        ICF_ASSERT(p_glb_pdb,p_glb_pdb->p_recv_msg)      

        if(ICF_FAILURE == icf_dbm_alloc_call_cntxt_blk(
                    p_glb_pdb,
                    &(p_glb_pdb->p_recv_msg->hdr.call_id),
                    &p_glb_pdb->p_call_ctx,
                    cc_pdb.p_ecode))
        {
            /* Fill call clear reason in pdb
             * This is best effort. No use to set forceful
             * call clear in case of failure */

            icf_cc_send_pa_wo_ctx_call_term(
                    &cc_pdb,
                    cc_pdb.p_glb_pdb->p_recv_msg->hdr.call_id,
                    (icf_call_clear_reason_t)
                    ICF_CALL_CLEAR_REASON_MAX_AUDIO_CALLS_EXCEEDED);

            /* Set status as call terminated */

            ICF_FUNCTION_EXIT(p_glb_pdb) /* function exit trace */
            ICF_RESET_MODULE(p_glb_pdb)
            /*Klocwork warning removal*/
            ICF_KLOCWORK_FALSE_POSITIVE(p_glb_pdb,cc_pdb.p_list_1) 

            return ICF_SUCCESS;

        }


        ICF_ASSERT(p_glb_pdb, (ICF_NULL != p_glb_pdb->p_call_ctx))
        /* copy call context pointer to CC pdb */
        cc_pdb.p_call_ctx = p_glb_pdb->p_call_ctx;
        cc_pdb.p_call_ctx->call_id = p_glb_pdb->p_recv_msg->hdr.call_id ; 
    }

    /*Klocwork warning removal*/
    ICF_KLOCWORK_FALSE_POSITIVE(p_glb_pdb,cc_pdb.p_list_1) 

       /* ICF_ASSERT(p_glb_pdb, (ICF_NULL != cc_pdb.p_call_ctx))*/

    /* Get CC global data pointer from DBM to obtain function pointer table */
    if((ICF_SUCCESS == ret_val) &&
            (ICF_CC_INVALID_EVENT != cc_pdb.current_event))
    {
        ret_val = icf_dbm_get_module_glb_data(p_glb_pdb,
                (icf_int_module_id_t)ICF_INT_MODULE_CC,
                (icf_void_t *)&p_cc_data, cc_pdb.p_ecode);

           if ((ICF_SUCCESS == ret_val) &&
            (ICF_CC_FORCEFUL_CLEAR == cc_pdb.current_event))
        {
            if (ICF_FALSE == cc_pdb.p_call_ctx->force_clr_rcvd)
            {
              cc_pdb.p_call_ctx->force_clr_rcvd = ICF_TRUE;
              ICF_CC_TRACE(&cc_pdb, ICF_TRACE_FSM_TRIGGERED);
			  if (( cc_pdb.p_call_ctx->call_state < ICF_CC_MAX_STATES ) &&
			  	  (cc_pdb.current_event < ICF_CC_MAX_EVENTS)) 
			  {
              ret_val = p_cc_data->cc_fsm_table[cc_pdb.p_call_ctx->call_state]
                [cc_pdb.current_event](&cc_pdb);
			  }
            }
            else
            {
              icf_cc_send_ssa_force_clear(&cc_pdb);
            }
            ICF_FUNCTION_EXIT(p_glb_pdb) /* function exit trace */
            ICF_RESET_MODULE(p_glb_pdb)
            return ICF_SUCCESS;
        }

        /* Invoke appropriate fsm function based on current state and event */
        if(ICF_SUCCESS == ret_val)
        {
            /* At this point we are going to trigger the CC state machine
             * so we will be processing any buffered reinvite. the data will
             * be taken from internal message. but we need to free the memory
             * so the pointer is saved for future reference as once CC state 
             * m/c is triggered the internal msg will be overwritten by a new ptr
             */
            icf_nw_inc_media_change_req_st *p_buffered_ptr = ICF_NULL;

            if(cc_pdb.p_call_ctx->common_bitmask_2 & 
                ICF_CC_BUFFERED_INC_MEDIA_UPDATE)
            {
                p_buffered_ptr = (icf_nw_inc_media_change_req_st*) cc_pdb.p_internal_msg->p_msg_data;
            }  

            ICF_CC_TRACE(&cc_pdb, ICF_TRACE_FSM_TRIGGERED);
			  if (( cc_pdb.p_call_ctx->call_state < ICF_CC_MAX_STATES ) &&
			  	  (cc_pdb.current_event < ICF_CC_MAX_EVENTS)) 
			  {
            ret_val = p_cc_data->cc_fsm_table[cc_pdb.p_call_ctx->call_state]
            [cc_pdb.current_event](&cc_pdb);
			  }

            if((cc_pdb.p_call_ctx->common_bitmask_2 & 
                 ICF_CC_BUFFERED_INC_MEDIA_UPDATE)&&
               (ICF_NULL != p_buffered_ptr))
            {
                /* Free the p_added_sdp_list cloned by module CHR
                 */
                if(ICF_NULL != p_buffered_ptr->p_added_sdp_list)
                {
                    icf_cmn_free_media_sdp_info_list(
                        p_glb_pdb, &(p_buffered_ptr->p_added_sdp_list), 
                        cc_pdb.p_ecode);
                }
                ICF_MEMFREE(p_glb_pdb, p_buffered_ptr, 
                        ICF_MEM_COMMON, 
			cc_pdb.p_ecode, ret_val)

                cc_pdb.p_call_ctx->common_bitmask_2 &=
                 ~ICF_CC_BUFFERED_INC_MEDIA_UPDATE;
            }
        }
        
        if (ICF_FAILURE == ret_val &&
           (ICF_ECODE_GENERIC_ERROR == *cc_pdb.p_ecode || 
		    ICF_ECODE_NO_PREF_ID_EXISTS == *cc_pdb.p_ecode))
        {
            /* If ecode is generic call clear, set status as clear and 
             * ret val as success and CM will initate call clear*/
            ICF_CC_SET_STATUS((&cc_pdb),ICF_STATUS_CALL_R_SRV_TERMINATE)
            /*we don't require any handling in new service status
               because now call is getting clear CC_CALL_RELEASE in status
               will take care */
             cc_pdb.p_internal_msg->msg_hdr.status_2 &=
                 ~ICF_STATUS_INTERNAL_REFER_W_REP_REQ;   
             cc_pdb.p_internal_msg->msg_hdr.status_2 &=
                 ~ICF_STATUS_INTERNAL_REFER_REQ;   
             cc_pdb.p_internal_msg->msg_hdr.status_2 &=
                 ~ICF_STATUS_EXTERNAL_CONFERENCE_CONNECTED;   
             cc_pdb.p_internal_msg->msg_hdr.status_2 &=
                 ~ICF_STATUS_INTERNAL_REFER_SUCCESS_RESP;   
             cc_pdb.p_internal_msg->msg_hdr.status_2 &=
                 ~ICF_STATUS_INTERNAL_REFER_FAILURE_RESP;   
             cc_pdb.p_internal_msg->msg_hdr.status_2 &=
                 ~ICF_STATUS_CALL_ENDED_DUE_TO_RELEASE;   
             cc_pdb.p_internal_msg->msg_hdr.status_2 &=
                 ~ICF_STATUS_INCOMING_EXTERNAL_CONF_CALL;   

            ret_val = ICF_SUCCESS;
        }
        /*If no MMI context was available for the call when sending DMS request
         *handle it as DMS response is received for the DMS request
         */
        if(ICF_TRUE == cc_pdb.dms_fail_no_mmi_ctx)
        {
            ret_val = icf_cc_07_dms_resp(&cc_pdb);
            /*klocwork warning removal*/
            p_glb_pdb->p_call_ctx->p_pref_media_profile =
                           cc_pdb.p_call_ctx->p_pref_media_profile;
            p_glb_pdb->p_call_ctx->p_stream_sdp_info =
                           cc_pdb.p_call_ctx->p_stream_sdp_info;
            ICF_KLOCWORK_FALSE_POSITIVE(p_glb_pdb,cc_pdb.p_stream_sdp_info);
        }
    }
    else if((ICF_SUCCESS == ret_val) &&
            (ICF_CC_INVALID_EVENT == cc_pdb.current_event))
    {
        /* this is an unexpected event in this state */
        ret_val = icf_cc_ignore_msg(&cc_pdb);
    }

    ICF_FUNCTION_EXIT(p_glb_pdb) /* function exit trace */
    ICF_RESET_MODULE(p_glb_pdb)
    return ret_val;
} /* end of icf_cc_process_mesg() */


/*******************************************************************************
 *
 * FUNCTION:        icf_cc_convert_api_to_event
 *
 * DESCRIPTION:     This function maps internal APIs to CC-specific events.
 *
 ******************************************************************************/
icf_return_t  icf_cc_convert_api_to_event(
        INOUT  icf_cc_pdb_st *p_cc_pdb)
{
    icf_return_t                     ret_val = ICF_SUCCESS;
    icf_uint8_t                      *p_event = ICF_NULL;
	icf_fax_start_type_t 			copy_fax_type = 0;
    /*icf_app_conf_st					 *p_app_conf = ICF_NULL;*/
    icf_config_data_st  *p_config_data = ICF_NULL;
    icf_list_st         *p_temp_list_node = ICF_NULL;

    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /* function entry trace */

        /* copy current event pointer locally */
        p_event = &(p_cc_pdb->current_event);
    ICF_ASSERT(p_cc_pdb->p_glb_pdb, (ICF_NULL != p_event))

    /* Get pointer to config data from DBM */
    ret_val = icf_dbm_get_module_glb_data(
                p_cc_pdb->p_glb_pdb,
                (icf_int_module_id_t)ICF_INT_MODULE_CFG,
                (icf_void_t *)&p_config_data, p_cc_pdb->p_ecode);

    if(ICF_SUCCESS == ret_val)
    {
        switch(p_cc_pdb->p_internal_msg->msg_hdr.msg_id)
        {
            /* Info response received from SSA*/
            case ICF_NW_INC_INFO_RESP:
                *p_event = ICF_CC_SSA_INFO_RESP;
                break;

                /*HANDLE_INFO_IND_START*/
            case ICF_NW_INC_INFO_IND:
                *p_event = ICF_CC_SSA_INFO_REQ;
                if (! p_cc_pdb->p_call_ctx->pending_info_cfm)
                {
                    /* There is NOT any info confirmation pending for previous 
                     * INFO requests.Even though if there is info_cfm pending then
                     * we will not do anything here bcoz we have already send response
                     * code 400 for the subsequent INFO received from SSA itself.
                     */
                    ret_val = icf_cc_start_info_cfm_timer(p_cc_pdb);
                    if (ICF_FAILURE != ret_val)
                    {
                        if (ICF_FAILURE == (ret_val = icf_cc_send_pa_info_ind(p_cc_pdb)))
                        {
                            /* Failure in giving info_ind to PA, so stop info_cfm_timer.*/
                            icf_cc_stop_info_cfm_timer(p_cc_pdb);
                        }
                        else
                        {
                            p_cc_pdb->p_call_ctx->pending_info_cfm = ICF_TRUE;                            
                        }  
                    } 
                }
                break;

            case ICF_USER_INFO_CFM:
                *p_event = ICF_CC_PA_INFO_CFM;
                if (p_cc_pdb->p_call_ctx->pending_info_cfm)
                {
                    icf_cc_stop_info_cfm_timer(p_cc_pdb);
                    p_cc_pdb->p_call_ctx->pending_info_cfm = ICF_FALSE;
                    ret_val = icf_cc_send_ssa_info_cfm(p_cc_pdb);
                }
                break; 

                /*HANDLE_INFO_IND_END*/

                /*Ack for reinvite is received*/
            case ICF_NW_INC_MEDIA_CHANGE_ACK:
                ICF_HANDLE_REINVITE_START
                    *p_event = ICF_CC_SSA_REINVITE_ACK;

                ret_val = icf_cc_handle_reinvite_ack(p_cc_pdb);

                ICF_HANDLE_REINVITE_END

                    break;

                /* Create call API from PA map to setup req */
            case ICF_USER_CREATE_CALL:
                *p_event = ICF_CC_PA_SETUP_REQ;
                break;

            case ICF_INTERNAL_INITIATE_CALL:
                *p_event = ICF_CC_PA_SETUP_REQ;
		if(!(ICF_CC_EXTERNAL_CONFERENCE_CALL & p_cc_pdb->p_call_ctx->common_bitmask_2))
   		{
                    p_cc_pdb->refer_init_call = ICF_TRUE;
 		}
                break;

                /* INC_CALL_SETUP_ACK map to SSA_SETUP_RESP */
            case ICF_NW_INC_CALL_SETUP_ACK:
                ICF_HANDLE_REINVITE_START
                    if((ICF_CC_15_AWT_CALL_MODIFY_ALERT_RESP == 
                                ICF_CC_GET_CALL_STATE(p_cc_pdb)) ||
                            (ICF_CC_15_AWT_CALL_MODIFY_ALERT_RESP ==
                             ICF_CC_GET_NEXT_STATE(p_cc_pdb)))
                    {
                        /*call_mod_cancel*/
                        if(ICF_NULL != ICF_CC_CHECK_PENDING_EVENT(p_cc_pdb,
                                    ICF_PENDING_EVENT_PA_CALL_MOD_CANCEL_REQ))
                        {
                            ICF_CC_RESET_PENDING_EVENT(p_cc_pdb,
                                    ICF_PENDING_EVENT_PA_CALL_MOD_CANCEL_REQ)

                            p_cc_pdb->p_call_ctx->prov_resp_status |= 
                                              ICF_CALL_MODIFY_PROV_RESP_RECVD;

                            if(ICF_FAILURE == icf_cc_14_pa_call_modify_cancel_req(p_cc_pdb))
                            {
                               ret_val = ICF_FAILURE;
                               break;
                            }

                        }
                        else if(ICF_NULL != ICF_CC_CHECK_PENDING_EVENT(p_cc_pdb,
                                    ICF_PENDING_EVENT_PA_STOP_FAX_REQ))
                        {
                            /* sending cancel to NW */
                            icf_cc_send_ssa_cancel_req(p_cc_pdb);
                            p_cc_pdb->p_call_ctx->stop_fax_resp_recvd = ICF_FALSE;
                            ICF_CC_RESET_PENDING_EVENT(p_cc_pdb,
                                    ICF_PENDING_EVENT_PA_STOP_FAX_REQ)
                        }
                        p_cc_pdb->p_call_ctx->prov_resp_status |= 
                            ICF_CALL_MODIFY_PROV_RESP_RECVD;
                        *p_event = ICF_CC_INVALID_EVENT;
                    }
                    else
                    {
                        ICF_HANDLE_REINVITE_END
                            *p_event = ICF_CC_SSA_SETUP_RESP;
                    }
                break;

                /* 183 from remote map to ssa media param */
            case ICF_NW_INC_MEDIA_PARAM:

                *p_event = ICF_CC_SSA_MEDIA_PARAM;

                ICF_HANDLE_REINVITE_START
                    if(ICF_CC_15_AWT_CALL_MODIFY_ALERT_RESP == ICF_CC_GET_CALL_STATE(p_cc_pdb))
                    {
                        p_cc_pdb->p_call_ctx->prov_resp_status |= 
                            ICF_CALL_MODIFY_PROV_RESP_RECVD;

                        icf_cc_free_ssa_msg(p_cc_pdb);
                        *p_event = ICF_CC_INVALID_EVENT;
                    }
                    /* Fix For SPR:19733*/
                    /* The following code is added to handle the 183
                     * with/without SDP received for Re-Invite other 
                     * than (Call modify request and Fax request) ie for 
                     * handling session refresh Re-Invite.IPTK does not process
                     * the 180 response with/without SDP for session refresh 
                     * Re-Invite.*/                
                    else if (ICF_NULL != ICF_CC_CHECK_COMMON_BITMASK(p_cc_pdb,
                            ICF_CALL_CONNECT_DONE))
                    {
                        *p_event = ICF_CC_INVALID_EVENT;
                    }                
                    else
                    {
                        ICF_HANDLE_REINVITE_END
                            if (ICF_NULL == (p_cc_pdb->p_call_ctx->common_bitmask & ICF_FORKED_RESP))
                            {
ICF_CHECK_MULT_ANS_ENBL_START(p_cc_pdb->p_glb_pdb)

                                    ret_val = icf_cc_chk_prack_req_media_param(p_cc_pdb);
                                    p_cc_pdb->sdp_present = ICF_TRUE;
                                    /* 
                                     * Set media param received status to true in prov_resp_status 
                                     */
                                    p_cc_pdb->p_call_ctx->prov_resp_status |=
                                        ICF_MEDIA_PARAM_RECEIVED;
                                    *p_event = ICF_CC_SSA_MEDIA_PARAM;

ICF_CHECK_MULT_ANS_ENBL_END

ICF_CHECK_MULT_ANS_DISBL_START(p_cc_pdb->p_glb_pdb)

                                if(ICF_MEDIA_PARAM_RECEIVED & 
                                        p_cc_pdb->p_call_ctx->prov_resp_status) 
                                {
                                    icf_cc_free_ssa_msg(p_cc_pdb);
                                    *p_event = ICF_CC_INVALID_EVENT;
                                }

                                else
                                {
                                    ret_val = icf_cc_chk_prack_req_media_param(p_cc_pdb);
                                    p_cc_pdb->sdp_present = ICF_TRUE;
                                    /* 
                                     * Set media param received status to true in prov_resp_status 
                                     */
                                    p_cc_pdb->p_call_ctx->prov_resp_status |= 
                                        ICF_MEDIA_PARAM_RECEIVED;
                                    *p_event = ICF_CC_SSA_MEDIA_PARAM;
                                }

ICF_CHECK_MULT_ANS_DISBL_END
                            }
                            else 
                            {
                                ret_val = icf_cc_chk_prack_req_media_param(p_cc_pdb);
                                p_cc_pdb->sdp_present = ICF_TRUE;
                                /* 
                                 * Set media param received status to true in prov_resp_status 
                                 */
                                p_cc_pdb->p_call_ctx->prov_resp_status |= 
                                    ICF_MEDIA_PARAM_RECEIVED;
                                *p_event = ICF_CC_SSA_MEDIA_PARAM;
                            }
                    }
                break;

                /* 180 without sdp map to ICF_CC_SSA_ALERT */
            case ICF_NW_INC_CALL_ALERT:
                *p_event = ICF_CC_SSA_ALERT;

                ICF_HANDLE_REINVITE_START
                    if(ICF_CC_15_AWT_CALL_MODIFY_ALERT_RESP == ICF_CC_GET_CALL_STATE(p_cc_pdb))
                    {
                        p_cc_pdb->p_call_ctx->prov_resp_status |= 
                            ICF_CALL_MODIFY_PROV_RESP_RECVD;
                        *p_event = ICF_CC_SSA_ALERT;
                    }
                    /* Fix For SPR:19733*/
                    /* The following code is added to handle the 180 without SDP
                     * received for Re-Invite other than (Call modify request
                     * and Fax request) ie for handling session refresh
                     * Re-Invite.IPTK does not process the 180 response
                     * without SDP for session refresh Re-Invite.*/                
                    else if (ICF_NULL != ICF_CC_CHECK_COMMON_BITMASK(p_cc_pdb,
                            ICF_CALL_CONNECT_DONE))
                    {
                        *p_event = ICF_CC_INVALID_EVENT;
                    }                  
                    else
                    {
                        ICF_HANDLE_REINVITE_END

                            if (ICF_NULL == (p_cc_pdb->p_call_ctx->common_bitmask & ICF_FORKED_RESP))
                            {

                                if(ICF_ALERT_RECEIVED & 
                                        p_cc_pdb->p_call_ctx->prov_resp_status)
                                {
                                    /*CSR_1-6155032 : In Case 2nd 180 is 
                                      received, IPTK shall check whether PRACK is 
                                      required or not. If yes, then IPTK shall send 
                                      PRACK for this 180 */
                                    ret_val = icf_cc_check_prack_req_alert(p_cc_pdb);
                                    /* SPR 19218 : Merged SPR 18097 */
                                    *p_event = ICF_CC_INVALID_EVENT;
                                } 
                                else
                                {
                                    ret_val = icf_cc_check_prack_req_alert(p_cc_pdb);
                                    /* Set alert received status to true in prov_resp_status */
                                    p_cc_pdb->p_call_ctx->prov_resp_status |= ICF_ALERT_RECEIVED;
                                    *p_event = ICF_CC_SSA_ALERT;
                                }
                            }
                            else
                            {
                                ret_val = icf_cc_check_prack_req_alert(p_cc_pdb);
                                /* Set alert received status to true in prov_resp_status */
                                p_cc_pdb->p_call_ctx->prov_resp_status |= ICF_ALERT_RECEIVED;
                                *p_event = ICF_CC_SSA_ALERT;
                            }
                    }
                break;

                /* 180 with  sdp map to  SSA_ALERT_WITH_MEDIA */
            case ICF_NW_INC_ALERT_WITH_MEDIA:

                *p_event = ICF_CC_SSA_ALERT_WITH_MEDIA;

                ICF_HANDLE_REINVITE_START
                    if(ICF_CC_15_AWT_CALL_MODIFY_ALERT_RESP == ICF_CC_GET_CALL_STATE(p_cc_pdb))
                    {
                        p_cc_pdb->p_call_ctx->prov_resp_status |= 
                            ICF_CALL_MODIFY_PROV_RESP_RECVD;
                        *p_event = ICF_CC_SSA_ALERT_WITH_MEDIA;
                    }
                    /* Fix For SPR:19733*/
                    /* The following code is added to handle the 180 with SDP
                     * received for Re-Invite other than (Call modify request
                     * and Fax request) ie for handling session refresh
                     * Re-Invite.IPTK does not process the 180 response with
                     * SDP for session refresh Re-Invite.*/
                    else if (ICF_NULL != ICF_CC_CHECK_COMMON_BITMASK(p_cc_pdb,
                            ICF_CALL_CONNECT_DONE))
                    {
                        *p_event = ICF_CC_INVALID_EVENT;
                    }
                    else
                    {
                        ICF_HANDLE_REINVITE_END
                            if (ICF_NULL == (p_cc_pdb->p_call_ctx->common_bitmask & ICF_FORKED_RESP))
                            {
ICF_CHECK_MULT_ANS_ENBL_START(p_cc_pdb->p_glb_pdb)
                        ret_val = icf_cc_check_prack_req_alert(p_cc_pdb);                                    
                        p_cc_pdb->sdp_present = ICF_TRUE;
                        /* Set alert received status to true in prov_resp_status */
                        p_cc_pdb->p_call_ctx->prov_resp_status |= ICF_ALERT_RECEIVED;                                    
                      /* CSR 1-8780514 SPR 20517 */
                      /* Parameter is_sdp_received is set 
                         when 180 with SDP is received */ 
                      p_cc_pdb->p_call_ctx->is_sdp_received = ICF_TRUE;
       
                        *p_event = ICF_CC_SSA_ALERT_WITH_MEDIA;
ICF_CHECK_MULT_ANS_ENBL_END
ICF_CHECK_MULT_ANS_DISBL_START(p_cc_pdb->p_glb_pdb)
                                if(ICF_ALERT_RECEIVED & 
                                        p_cc_pdb->p_call_ctx->prov_resp_status)
                                {
                                    icf_cc_free_ssa_msg(p_cc_pdb);
                                    /* SPR 19218 : Merged SPR 18097 */
                                    icf_cc_check_prack_req_alert(p_cc_pdb);
                                    *p_event = ICF_CC_INVALID_EVENT;
                                }
                                else
                                {
                                    ret_val = icf_cc_check_prack_req_alert(p_cc_pdb);
                                    p_cc_pdb->sdp_present = ICF_TRUE;
                                    /* Set alert received status to true in prov_resp_status */
                                    p_cc_pdb->p_call_ctx->prov_resp_status |= ICF_ALERT_RECEIVED;
                                    /* CSR 1-8780514 SPR 20517 */
                                    p_cc_pdb->p_call_ctx->is_sdp_received = ICF_TRUE;

                                    *p_event = ICF_CC_SSA_ALERT_WITH_MEDIA;
                                }
ICF_CHECK_MULT_ANS_DISBL_END
                            }
                            else
                            {
                                ret_val = icf_cc_check_prack_req_alert(p_cc_pdb);
                                p_cc_pdb->sdp_present = ICF_TRUE;
                                /* Set alert received status to true in prov_resp_status */
                                p_cc_pdb->p_call_ctx->prov_resp_status |= ICF_ALERT_RECEIVED;
                                /* CSR 1-8780514 SPR 20517 */
                                p_cc_pdb->p_call_ctx->is_sdp_received = ICF_TRUE;

                                *p_event = ICF_CC_SSA_ALERT_WITH_MEDIA;
                            }
                    }
                break;

                /* 200 ok for invite map to SSA_CALL_CONNECT*/
            case ICF_NW_INC_CONNECT:
                /*Check that whether for the failure response of 200ok
                 *In this case,send ACK on the network and then clear the call. 
                 */
                /*SPR- 13995 Aman 04-01-07*/
                if(ICF_SUCCESS == icf_cc_validate_ssa_connect_failure(p_cc_pdb)) 
                {
                    icf_cc_handle_ssa_connect_failure(p_cc_pdb);
                    *p_event = ICF_CC_INVALID_EVENT;
                }
                else
                {
                    *p_event = ICF_CC_SSA_CALL_CONNECT;
                }
                break;

                /* Create media resp from MMI map to CMS_RESP */
            case ICF_CREATE_MEDIA_RESP:
                *p_event = ICF_CC_CMS_RESP;
                /*SPR 19590 change start*/
                /*Copy the txn id from the mmi ctx into the call ctx
                  as the API/SIP message which has undergone CMS request
                  is now getting processed after CMS response is recv.
                  Therefore to work on current message/API hdr/body/tag list
                  the txn id in the call context is updated.*/
                if(ICF_NULL != p_cc_pdb->p_call_ctx)
                {    
                    p_cc_pdb->p_call_ctx->txn_id = 
                      p_cc_pdb->p_call_ctx->mmi_context.txn_id;
                    p_cc_pdb->p_call_ctx->mmi_context.txn_id = 0;
                }    
                break;

                /* This API is used by CM to indicate start Signaling*/
            case ICF_START_PROC_SIGNALING:
                *p_event = ICF_CC_START_CALL_PROC_SIG;
                break;

                /* This API is used by CM to indicate start media*/
            case ICF_START_PROC_MEDIA:
                *p_event = ICF_CC_START_CALL_PROC_MEDIA;
                break;

                /* MMI send this api as mms response */
            case ICF_MODIFY_MEDIA_RESP:
                *p_event = ICF_CC_MMS_RESP;
                /*SPR 19590 change start*/
                /*Copy the txn id from the mmi ctx into the call ctx
                  as the API/SIP message which has undergone MMS request
                  is now getting processed after MMS response is recv.
                  Therefore to work on current message/API hdr/body/tag list
                  the txn id in the call context is updated.*/
                if(ICF_NULL != p_cc_pdb->p_call_ctx)
                {    
                   p_cc_pdb->p_call_ctx->txn_id = 
                      p_cc_pdb->p_call_ctx->mmi_context.txn_id;
                   p_cc_pdb->p_call_ctx->mmi_context.txn_id = 0;
                }   
                /*SPR 19590 change end*/
                break;

                /* MMI send this api as dms response */
            case ICF_DELETE_MEDIA_RESP:
                *p_event = ICF_CC_DMS_RESP;
                /*SPR 19590 change start*/
                /*Copy the txn id from the mmi ctx into the call ctx
                  as the API/SIP message which has undergone DMS request
                  is now getting processed after DMS response is recv.
                  Therefore to work on current message/API hdr/body/tag list
                  the txn id in the call context is updated.*/
                if(ICF_NULL != p_cc_pdb->p_call_ctx)
                {    
                    p_cc_pdb->p_call_ctx->txn_id = 
                      p_cc_pdb->p_call_ctx->mmi_context.txn_id;
                    p_cc_pdb->p_call_ctx->mmi_context.txn_id = 0;
                }  
                /*SPR 19590 change end*/
                break;

                /* SSA send this api on reciving bye from nw*/
            case ICF_NW_INC_CALL_TERMINATE:
                *p_event = ICF_CC_SSA_CALL_TERMINATE_REQ;
#ifdef ICF_TRACE_ENABLE 
                 p_cc_pdb->call_clear_dir = 2;
                /* Added for sending traces */
                 ICF_CC_APP_TRACE(p_cc_pdb,
                      ICF_TRACE_CALL_TERMINATION_INITIATED)
#endif
                break;

                /* SSA send this api on reciving 4xx/5xx resp from remote */
            case ICF_NW_INC_CALL_REJECT:
                /* Fix for SPR 18731: Use the release cause filled by SSA only if
                it has not been filled by CC already.
                */
                if(ICF_CALL_CLEAR_REASON_INVALID == p_cc_pdb->p_call_ctx->release_cause)
                { 
                    p_cc_pdb->p_call_ctx->release_cause = 
                        (icf_uint8_t)((icf_nw_inc_reject_st *)(p_cc_pdb->p_internal_msg->p_msg_data))
                        ->reason_code;
                }
                *p_event = ICF_CC_SSA_REMOTE_REJECT;
                break;

                /* SSA send this api on reciving 302 from remote party */
            case ICF_NW_INC_CALL_REDIRECT:
                *p_event = ICF_CC_SSA_REDIRECT;
                break;

                /* PA send this indication when user clear call */
            case ICF_USER_CALL_TERMINATE:
                /*If it is an incomming call then enter the call which is not 
                 * connected then add this to received call list. If the call is 
                 * connected we would have added the to received call list when 
                 * connect was received
                 */
                if (ICF_NULL != ICF_CC_CHECK_COMMON_BITMASK(p_cc_pdb,
                            ICF_CALL_DIR) && 
                        (ICF_NULL == ICF_CC_CHECK_COMMON_BITMASK(p_cc_pdb,
                                                                 ICF_CALL_CONNECT_DONE)))
                {
                    /*Remove this call from missed call list and add it to received 
                     *call list as terminate is received for the call */
#ifdef ICF_STAT_ENABLED
                    icf_sm_set_recd_call(p_cc_pdb->p_glb_pdb);
#endif
                }
                if(ICF_INVALID_CALL_ID != p_cc_pdb->p_call_ctx->replace_initiated_call_id)
                {
                    icf_call_ctx_st *p_replace_initiated_call_ctxt = ICF_NULL;
                    icf_call_ctx_st *p_old_call_ctxt = ICF_NULL;
                    icf_error_t       ecode = 0;

                    /* Fetch Call Context of the replace_initiated_call*/
                    /* update line_id,mode from old to new */
                    if(ICF_SUCCESS == icf_dbm_find_call_cntxt_blk(
                                p_cc_pdb->p_glb_pdb,
                                p_cc_pdb->p_call_ctx->replace_initiated_call_id,
                                &p_replace_initiated_call_ctxt,
                                &ecode))
                    {
                        /* Set the same line_id and media_mode_to_be_applied for the new call */
                        p_replace_initiated_call_ctxt->line_id = p_cc_pdb->p_call_ctx->line_id;

                        /* Fix for CSR 1-6727350: SPR 19316 */
						/* If APP Connect is required for new call with Replaces
						 * then first send PARTY_REPLACED_IND on old call.
						 */
						if (ICF_TRUE == 
						 		p_cc_pdb->p_config_data->app_connect_reqd_for_replaces)
						{
							p_old_call_ctxt = p_cc_pdb->p_call_ctx;
							p_cc_pdb->p_call_ctx = p_replace_initiated_call_ctxt;
							p_cc_pdb->p_glb_pdb->p_call_ctx = p_replace_initiated_call_ctxt;		
						  	if (ICF_SUCCESS == icf_cc_send_pa_party_replaced_ind(p_cc_pdb))
							{
								p_cc_pdb->p_call_ctx = p_old_call_ctxt;
								p_cc_pdb->p_glb_pdb->p_call_ctx = p_old_call_ctxt;
							}
						}
                    }
                }

                /* set the call clear reason in call ctx */
                icf_cc_set_pa_init_call_rel_cause(p_cc_pdb);
                    
                *p_event = ICF_CC_PA_CALL_TERMINATE_REQ;
#ifdef ICF_TRACE_ENABLE 
                p_cc_pdb->call_clear_dir = 1;
                /* Added for sending traces */
                ICF_CC_APP_TRACE(p_cc_pdb,
                                ICF_TRACE_CALL_TERMINATION_INITIATED)
#endif
                break;

            case ICF_NW_INC_CALL_ABORT_RESP:
                *p_event = ICF_CC_SSA_CALL_CANCEL_RESP;
                break;

            case ICF_USER_CALL_TERMINATE_RESP:
                *p_event = ICF_CC_PA_CALL_TERMINATE_RESP;
                break;

            case ICF_USER_INFO:
                *p_event = ICF_CC_PA_INFO_REQ;
                break; 

                /* SSA send this api to indicate that remote has send ack for BYE */
            case ICF_NW_INC_CALL_TERMINATE_RESP:
            case ICF_NW_INC_CALL_REJECT_ACK:
                *p_event = ICF_CC_SSA_CALL_TERMINATE_RESP;
                break;

                /* SSA send this api on reciving UPDATE/REINVITE */
            case ICF_NW_INC_MEDIA_UPDATE:
                {
                    icf_nw_inc_media_change_req_st  *p_inc_med_change = ICF_NULL;
                    icf_boolean_t      resp_sent = ICF_FALSE;

                    p_inc_med_change = (icf_nw_inc_media_change_req_st *)
                        p_cc_pdb->p_internal_msg->p_msg_data;
                  
                    /*If UPDATE wo SDP received, send call_modify_ind_wo_cfm_reqd to application.
                     *This is only for UPDATE.
                     *In case of REINVITE,CC will handle this.*/

                   /* SPR 19188: SPR 18979: If UPDATE is receved with media line with port 0 then 
                    * it should not be treated as without SDP , therefore the check
                    * has been added for nw_reject_list so that when none is present then
                    * this should be terated as UPDATE without SDP
                    */ 
                   if ((ICF_NULL == p_inc_med_change->p_sdp_list) && (ICF_NULL ==
                           p_inc_med_change->p_added_sdp_list) &&
                           (ICF_NULL ==  p_inc_med_change->p_nw_reject_stream_list) &&   
                       !(ICF_NW_INC_MEDIA_CHG_REJECTED_STREAM & p_inc_med_change->bitmask) && 
                       !(ICF_INC_MEDIA_CHANGE_METHOD_REINVITE &
                           p_inc_med_change->bitmask)) 
                    {    
                       /* Rel8.0 We do need to send
                        * call_modify_ind_wo_cfm_reqd only if it's non
                        * precondition call or precondition call but it's
                        * in connected state*/
                       if((ICF_NULL == ICF_CC_CHECK_COMMON_BITMASK_2(p_cc_pdb,
                                                ICF_PRECONDITION_REQD)) ||
                          (ICF_NULL != ICF_CC_CHECK_COMMON_BITMASK(p_cc_pdb, 
                                    ICF_CALL_CONNECT_DONE)))
                        {
ICF_CHECK_TUNNEL_MODE_START(p_cc_pdb->p_glb_pdb, ICF_FALSE)
                           p_cc_pdb->common_bitmask |= ICF_CC_CALL_MODIFY_CFM_NT_REQD;
                           p_cc_pdb->pa_call_mod_wo_media = ICF_TRUE;
ICF_CHECK_TUNNEL_MODE_END
                           /* Fix for SPR 18658:
                              Setting the common bitmask ICF_CALL_MOD_REQ_WO_SDP
                              before sending call modify indication to application.
                           */
                           ICF_CC_SET_COMMON_BITMASK(p_cc_pdb,
                              ICF_CALL_MOD_REQ_WO_SDP)
                           /*  Rel 8.1 changes
                            *  It is done because it is done from FSM
                            *  as of now 200 OK is not send to peer in tunnel
                            *  that is why this indication is stopped from here.
                            */
ICF_CHECK_TUNNEL_MODE_START(p_cc_pdb->p_glb_pdb, ICF_FALSE)
                           ret_val = icf_cc_send_pa_call_modify_ind(p_cc_pdb);
                           /* resetting the bitmask ICF_CALL_MOD_REQ_WO_SDP */
                           ICF_CC_RESET_COMMON_BITMASK(p_cc_pdb,
                              ICF_CALL_MOD_REQ_WO_SDP)
ICF_CHECK_TUNNEL_MODE_END
                        }
ICF_CHECK_TUNNEL_MODE_START(p_cc_pdb->p_glb_pdb, ICF_FALSE)
                        return ret_val;
ICF_CHECK_TUNNEL_MODE_END
                    }
     
                    if (ICF_INC_MEDIA_CHANGE_METHOD_REINVITE & p_inc_med_change->bitmask)
                    {
                        ret_val = icf_cc_send_ssa_setup_resp(p_cc_pdb);
                    }
ICF_CHECK_TUNNEL_MODE_START(p_cc_pdb->p_glb_pdb, ICF_FALSE)
                    if (ICF_FAILURE == ret_val)
                    {
                        /*Do nothing*/
                    }
                    else if(ICF_NULL != p_cc_pdb->p_call_ctx->fax_type)
                    {
                        /*CSR_1-6347417 : SPR-19129 fixed.*/
                        /* A Re-INVITE for fax request with T38 codec is received
                         * from remote with same fax method (add/replace) as
                         * Re-invite for start fax has been sent on network with T38
                         * codec when retry after timer is running after
                         * receiving 491 from remote.
                         */

                        if((ICF_NULL != ICF_CC_CHECK_COMMON_BITMASK_2(p_cc_pdb,
                                        ICF_CC_491_RESP_PROCESS_ONGOING)) &&
                                (ICF_METHOD_STOP_FAX_REQ != 
                                 (ICF_METHOD_STOP_FAX_REQ &
                                  p_cc_pdb->p_call_ctx->call_modify_method_bitmask)))
                        {
                            icf_nw_inc_media_change_req_st  *p_update_req = 
                                (icf_nw_inc_media_change_req_st *)
                                p_cc_pdb->p_internal_msg->p_msg_data;

                            p_cc_pdb->p_list_1 = p_update_req->p_sdp_list;

                            /* Re-INVITE/UPDATE received with any chnage in
                             * m-line from sent Re-INVITE will be rejected 
                             * with 500 response code.  
                             */

                            if((ICF_NW_INC_MEDIA_NW_REJECT_STREAM == 
                                        (ICF_NW_INC_MEDIA_NW_REJECT_STREAM &
                                         p_update_req->bitmask)) ||
                                    (ICF_NW_INC_MEDIA_ADD_STREAM ==
                                     (ICF_NW_INC_MEDIA_ADD_STREAM &
                                      p_update_req->bitmask)) ||
                                    (ICF_NW_INC_MEDIA_CHG_REJECTED_STREAM ==
                                     (ICF_NW_INC_MEDIA_CHG_REJECTED_STREAM &
                                      p_update_req->bitmask)) ||
                                    ((ICF_NW_INC_MEDIA_CHG_SDP_PRESENT ==
                                      (ICF_NW_INC_MEDIA_CHG_SDP_PRESENT &
                                       p_update_req->bitmask)) &&   
                                     (ICF_FAILURE ==
                                      icf_cc_validate_inc_offer_change_on_fax_retry(p_cc_pdb))))
                            {
                                p_cc_pdb->p_internal_msg->msg_hdr.msg_id =
                                    ICF_NW_INC_MEDIA_UPDATE;
                                p_cc_pdb->p_call_ctx->offer_failure_reason =
                                    ICF_SA_SERVER_INTERNAL_ERROR;
                                icf_cc_free_ssa_msg(p_cc_pdb);
                                resp_sent = ICF_TRUE;
                                ret_val = icf_cc_send_ssa_offer_chg_err(p_cc_pdb);
                            }

                            /*Handling when reinvite without SDP is received*/
                            else if((ICF_NW_INC_MEDIA_NW_REJECT_STREAM !=
                                        (ICF_NW_INC_MEDIA_NW_REJECT_STREAM &
                                         p_update_req->bitmask)) &&
                                    (ICF_NW_INC_MEDIA_ADD_STREAM !=
                                     (ICF_NW_INC_MEDIA_ADD_STREAM &
                                      p_update_req->bitmask)) &&
                                    (ICF_NW_INC_MEDIA_CHG_REJECTED_STREAM !=
                                     (ICF_NW_INC_MEDIA_CHG_REJECTED_STREAM &
                                      p_update_req->bitmask)) &&
                                    (ICF_NW_INC_MEDIA_CHG_SDP_PRESENT !=
                                     (ICF_NW_INC_MEDIA_CHG_SDP_PRESENT & 
                                      p_update_req->bitmask)))
                            {
                                p_cc_pdb->p_internal_msg->msg_hdr.msg_id =
                                    ICF_NW_INC_MEDIA_UPDATE;
                                p_cc_pdb->p_call_ctx->offer_failure_reason =
                                    ICF_SA_SERVER_INTERNAL_ERROR;
                                icf_cc_free_ssa_msg(p_cc_pdb);
                                resp_sent = ICF_TRUE;
                                ret_val = icf_cc_send_ssa_offer_chg_err(p_cc_pdb);
                            }       
                            else
                            {
                                if(ICF_CALL_TYPE_T38 == (ICF_CALL_TYPE_T38 &
                                            p_cc_pdb->p_call_ctx->call_type))
                                {
                                    *p_event = ICF_CC_SSA_FAX_REQ;
                                }
                                else
                                {
                                    *p_event = ICF_CC_SSA_OFFER_CHANGE;
                                }
                            }
                        }                        
                        /*We will not reject RE-INVITE for every case of service active
                          because XCONF requires incase isfocus comes in RE-INVITE.
                          So restriction will be in fax condition and removed
                          the service_ctx check*/ 
                        else if (ICF_FALSE != icf_cc_check_is_fax_stop_req(p_cc_pdb))
                        {
                            *p_event = ICF_CC_SSA_STOP_FAX_REQ;
                        }
                        else
                        {
                            /* CSR 1-5390593 Merge*/
                            if(ICF_TRUE == icf_cc_check_ssa_offer_change(p_cc_pdb)) 
                            {
                                resp_sent = ICF_TRUE;
                                ret_val = icf_cc_send_ssa_offer_chg_rsp(p_cc_pdb);
                            }
                            else
                            { 
                                p_cc_pdb->p_call_ctx->offer_failure_reason =
                                    ICF_FAX_ERR_INVALID_REQUEST;
                                resp_sent = ICF_TRUE;
                                ret_val = icf_cc_send_ssa_offer_chg_err(p_cc_pdb);
                            }
                        }
                    }

                    else if(ICF_SUCCESS == icf_cc_check_fax_req(p_cc_pdb))
                    {
                        if((ICF_NULL != p_cc_pdb->p_call_ctx->p_service_ctx)||
                                (ICF_MEDIA_STATE_LOCAL_HELD == (ICF_MEDIA_STATE_LOCAL_HELD &
                                                                p_cc_pdb->p_call_ctx->call_status)) ||
                                (ICF_CALL_STATUS_CONFERENCE == (ICF_CALL_STATUS_CONFERENCE & 
                                                                p_cc_pdb->p_call_ctx->call_status)))
                        {
                            p_cc_pdb->p_internal_msg->msg_hdr.msg_id = ICF_NW_INC_MEDIA_UPDATE;
                            p_cc_pdb->p_call_ctx->offer_failure_reason = 
                                ICF_FAX_ERR_INVALID_REQUEST;
                            icf_cc_free_ssa_msg(p_cc_pdb);
                            resp_sent = ICF_TRUE;
                            ret_val = icf_cc_send_ssa_offer_chg_err(p_cc_pdb);
                        }
                        else if(ICF_NULL == 
                                ICF_CC_CHECK_COMMON_BITMASK(p_cc_pdb, 
                                    ICF_CALL_CONNECT_DONE))
                        {
                            p_cc_pdb->p_internal_msg->msg_hdr.msg_id = ICF_NW_INC_MEDIA_UPDATE;
                            p_cc_pdb->p_call_ctx->offer_failure_reason =
                                ICF_FAX_ERR_INVALID_REQUEST;
                            icf_cc_free_ssa_msg(p_cc_pdb);
                            resp_sent = ICF_TRUE;
                            ret_val = icf_cc_send_ssa_offer_chg_err(p_cc_pdb);

                        }
                        else
                        {
                            *p_event = ICF_CC_SSA_FAX_REQ;
                        }

                    }
                    /* CSR 1-5872015 fix : when a service is active and reinvite is received
                        with no change then send 200 else reject the reinvite with 491 imstead of 488 */
					else if(ICF_NULL != p_cc_pdb->p_call_ctx->p_service_ctx)
                    {
                        /* SPR 18604: when reINVITE is received by CC and 
                         * ongoing service is XCONF then handle the reINVITE */
                        if(ICF_CC_EXTERNAL_CONFERENCE_CALL & p_cc_pdb->p_call_ctx->common_bitmask_2)
                        {
                            *p_event = ICF_CC_SSA_OFFER_CHANGE;
                        }
                        /* CSR 1-6037047 Merge  
                         * when conference service is ongoing and call is not on
                         * local hold hten handle the reinvite , else reject the request with 491 
                         * response 
                         */
                        else if(ICF_SERVICE_TWC == 
                           ((icf_service_ctx_st *)p_cc_pdb->p_call_ctx->p_service_ctx)
                                  ->active_service_list.active_service_status)
                        {
                            if(ICF_CALL_STATUS_LOCAL_HELD == (ICF_CALL_STATUS_LOCAL_HELD &
                                                      p_cc_pdb->p_call_ctx->call_status))
                           {
                                p_cc_pdb->p_call_ctx->offer_failure_reason =
                                    ICF_REQUEST_PENDING;
                                icf_cc_free_ssa_msg(p_cc_pdb);
                                resp_sent = ICF_TRUE; 
                                ret_val = icf_cc_send_ssa_offer_chg_err(p_cc_pdb);
                            }
                            else
                            {
                                *p_event = ICF_CC_SSA_OFFER_CHANGE;
                            }
                        }
                        else
                        {
                            if(ICF_TRUE == icf_cc_check_ssa_offer_change(p_cc_pdb))                            {
                                resp_sent = ICF_TRUE;
                                ret_val = icf_cc_send_ssa_offer_chg_rsp(p_cc_pdb); 	                           }
                            else
                            {
                                p_cc_pdb->p_call_ctx->offer_failure_reason =
                                    ICF_REQUEST_PENDING;
                                resp_sent = ICF_TRUE; 
                                ret_val = icf_cc_send_ssa_offer_chg_err(p_cc_pdb);
                            }
                        }
                    }
                    else
                    {
                        *p_event = ICF_CC_SSA_OFFER_CHANGE;
                    }
ICF_CHECK_TUNNEL_MODE_END
ICF_CHECK_TUNNEL_MODE_START(p_cc_pdb->p_glb_pdb, ICF_TRUE)
                    if (ICF_FAILURE == ret_val)
                    {
                        /*Do nothing*/
                    }
                    else
                    {
                        *p_event = ICF_CC_SSA_OFFER_CHANGE; 
                    }
ICF_CHECK_TUNNEL_MODE_END
                    /* CSR 1-5872015 fix : when reinvite got rejected and 
                     * application sent call resume request then the call
                     * context header list was populated for received messages
                     * from network, so IPTK was wrongly inserting the header
                     * list in the SIP message 
                     */
                    if(ICF_SUCCESS == ret_val && ICF_TRUE == resp_sent)
                    {
                        /* free the header list populated by SSA so that it does
                         * not reflect in subsequent requests 
                         */
                        /*Fetch the current node and delete the same in call
                         * ctx list*/
                        if(ICF_FAILURE == icf_cmn_fetch_curr_hdr_list_node_from_call_ctx(
                                   p_cc_pdb->p_glb_pdb,
                                   &p_temp_list_node))
                        {
                            ret_val = ICF_FAILURE;
                        }
                        else if(ICF_FAILURE == icf_cmn_del_header_list_node_in_call_ctx(
                                   p_cc_pdb->p_glb_pdb,
                                   p_temp_list_node))
                        {
                            ret_val = ICF_FAILURE;
                        }
                    }
                }
                break;

                ICF_HANDLE_REINVITE_START
            case ICF_USER_CALL_MODIFY_REQ:
                {
                    /* Added for sending traces */
                    ICF_CC_APP_TRACE(p_cc_pdb,ICF_TRACE_CALL_MODIFY_REQ)
ICF_CHECK_TUNNEL_MODE_START(p_cc_pdb->p_glb_pdb, ICF_FALSE)
#ifdef NON_IMS_CLIENT
                    ICF_CHECK_IMS_START(p_cc_pdb->p_glb_pdb,ICF_FALSE)

#ifdef ICF_FAX_SUPPORT

                    /* If fax call is ongoing then send the failure 
                       response to application */
                    if(ICF_NO_FAX_CALL != p_cc_pdb->p_call_ctx->fax_type)
                    {
                        p_cc_pdb->p_call_ctx->app_failure_cause = 
                                ICF_CAUSE_INVALID_CALL_MODIFY_REQUEST;
                        ICF_CC_SET_COMMON_BITMASK(p_cc_pdb, 
                                    ICF_CALL_MODIFY_RESP_FAIL)

                        /* Send failure response to application */
                        ret_val = icf_cc_send_pa_call_modify_resp(p_cc_pdb);
                        return ret_val;
                    }
#endif /* ICF_FAX_SUPPORT */
                    ICF_CHECK_IMS_END
#endif /* NON_IMS_CLIENT */
                    /* If the call is not connected and the call modify request is received with SDP
                     * then send failure response to the application
                     */ 
                    if(ICF_NULL == ICF_CC_CHECK_COMMON_BITMASK(
                                     p_cc_pdb, ICF_CALL_CONNECT_DONE))
                    {
                        /*twc_update handling*/
 
                        icf_call_modify_req_st *p_call_modify_req = ICF_NULL;
 
                         p_call_modify_req = 
                          (icf_call_modify_req_st*)(p_cc_pdb->p_glb_pdb->p_recv_msg->payload);
 
                        if(ICF_CALL_MOD_REQ_MEDIA_MOD_LIST_PRESENT & p_call_modify_req->bit_mask ||
                          ICF_CALL_MOD_REQ_DELETED_STREAM_PRESENT & p_call_modify_req->bit_mask) 
                        {
                            p_cc_pdb->p_call_ctx->app_failure_cause = ICF_CAUSE_INVALID_CALL_MODIFY_REQUEST;
                            ICF_CC_SET_COMMON_BITMASK(p_cc_pdb, ICF_CALL_MODIFY_RESP_FAIL)
                            ret_val = icf_cc_send_pa_call_modify_resp(p_cc_pdb);
                        }
                        /* If common_bitmask is UPDATE_WO_SDP then already UPDATE wo SDP is sent
                         * and no response is received hence send failure response*/  
                        else if(ICF_NULL != ICF_CC_CHECK_COMMON_BITMASK(p_cc_pdb,ICF_CALL_MOD_REQ_WO_SDP))
                        {
                            p_cc_pdb->p_call_ctx->app_failure_cause = ICF_CAUSE_CALL_MODIFY_REQ_PENDING;
                            ICF_CC_SET_COMMON_BITMASK(p_cc_pdb, ICF_CALL_MODIFY_RESP_FAIL)
                            ret_val = icf_cc_send_pa_call_modify_resp(p_cc_pdb);
                        }
                        else 
                        {
                            *p_event = ICF_CC_PA_CALL_MODIFY_REQ;
                            /* Fix for CSR 1-6580995: SPR 19321 */
		         	        ICF_CC_SET_COMMON_BITMASK_2(p_cc_pdb, ICF_APP_CALL_MODIFY_REQ_ONGOING)
                        }
                    }
                    else if(ICF_NULL != p_cc_pdb->p_call_ctx->p_service_ctx)
                    {
                         p_cc_pdb->p_call_ctx->app_failure_cause = 
                                     ICF_CAUSE_SERVICE_ONGOING;
                         ICF_CC_SET_COMMON_BITMASK(p_cc_pdb, 
                                              ICF_CALL_MODIFY_RESP_FAIL)
                         ret_val = icf_cc_send_pa_call_modify_resp(p_cc_pdb);
                    }
                    else if(ICF_NULL != ICF_CC_CHECK_COMMON_BITMASK_2(p_cc_pdb,ICF_INCOMING_CALL_MOD_CANCEL_REQ))
                    {
                        p_cc_pdb->p_call_ctx->app_failure_cause = ICF_CAUSE_MEDIA_MODIFY_ONGOING;
                        ICF_CC_SET_COMMON_BITMASK(p_cc_pdb, ICF_CALL_MODIFY_RESP_FAIL)
                        ret_val = icf_cc_send_pa_call_modify_resp(p_cc_pdb);
                    }
                    else
                    {
                        p_cc_pdb->p_call_ctx->call_modify_method_bitmask 
                                 |= ICF_METHOD_CALL_MODIFY_REQ;
                        *p_event = ICF_CC_PA_CALL_MODIFY_REQ;
                        /* Fix for CSR 1-6580995: SPR 19321 */
		         	      ICF_CC_SET_COMMON_BITMASK_2(p_cc_pdb, ICF_APP_CALL_MODIFY_REQ_ONGOING)
                    }
ICF_CHECK_TUNNEL_MODE_END
ICF_CHECK_TUNNEL_MODE_START(p_cc_pdb->p_glb_pdb,ICF_TRUE) 
                    /*  In tunnel mode if one update without SDP is sent then 
                     *  reject another one */
                    if(ICF_NULL == ICF_CC_CHECK_COMMON_BITMASK(
                                     p_cc_pdb, ICF_CALL_CONNECT_DONE))
                    {
                        if(ICF_NULL != ICF_CC_CHECK_COMMON_BITMASK(p_cc_pdb,ICF_CALL_MOD_REQ_WO_SDP))
                        {
                            p_cc_pdb->p_call_ctx->app_failure_cause = ICF_CAUSE_CALL_MODIFY_REQ_PENDING;
                            ICF_CC_SET_COMMON_BITMASK(p_cc_pdb, ICF_CALL_MODIFY_RESP_FAIL)
                            ret_val = icf_cc_send_pa_call_modify_resp(p_cc_pdb);
                        }
                        else 
                        {
                            *p_event = ICF_CC_PA_CALL_MODIFY_REQ;

                            /*SPR 19751 : In case of Tunnelled call, set the bitmask so that
                             * call modify response is given to application
                             */ 
                            ICF_CC_SET_COMMON_BITMASK_2(p_cc_pdb, ICF_APP_CALL_MODIFY_REQ_ONGOING)
                        }
                    }
                    else
                    {
                        p_cc_pdb->p_call_ctx->call_modify_method_bitmask 
                            |= ICF_METHOD_CALL_MODIFY_REQ;
                        *p_event = ICF_CC_PA_CALL_MODIFY_REQ;

                        /*SPR 19751 : In case of Tunnelled call, set the bitmask so that
                         * call modify response is given to application
                         */ 
                        ICF_CC_SET_COMMON_BITMASK_2(p_cc_pdb, ICF_APP_CALL_MODIFY_REQ_ONGOING)
                    }

ICF_CHECK_TUNNEL_MODE_END
                }
          break;

            case ICF_USER_CALL_MODIFY_CFM:
                if((ICF_CC_SSA_CALL_MODIFY_CANCEL_REQ != 
                                 ICF_CC_GET_ONGOING_EVENT(p_cc_pdb)) || 
                   (ICF_CC_06_CONNECTED != ICF_CC_GET_CALL_STATE(p_cc_pdb)))
                {
                  *p_event = ICF_CC_PA_CALL_MODIFY_RSP;
                  /* Added for sending traces */
                  ICF_CC_APP_TRACE(p_cc_pdb,ICF_TRACE_CALL_MODIFY_CFM)
                }
               break;
          

                ICF_HANDLE_REINVITE_END

                    /* This API is used by MMI to indicate error in processing/error ind
                     * from MM*/
            case ICF_MEDIA_ERROR_IND:
                    *p_event = ICF_CC_MM_ERR_IND;

                    /* Set the stat for media_error indication */
                    ICF_STAT(p_cc_pdb->p_glb_pdb,
                            ICF_STAT_MEDIA_ERROR)
                        break;

            case ICF_TIMER_EXPIRY_IND:
                    if(ICF_TIMER_ID_INVALID == p_cc_pdb->p_call_ctx->retry_after_timer_id)
                    {
                      icf_cc_reset_timer_id(p_cc_pdb);
                    }
                    *p_event = ICF_CC_TIMER_EXPIRY;
                    break;

            case ICF_NW_INC_CALL_SETUP:
                    /*Set this call as missed call. This would be marked as received 
                     *call if call term indication or call connect indication is 
                     *received from PA for this call*/
#ifdef ICF_STAT_ENABLED
                    icf_sm_set_missed_call(p_cc_pdb->p_glb_pdb);
#endif
                    *p_event = ICF_CC_SSA_SETUP_REQ;
                    break;

            case ICF_CALL_WAIT_CONTINUE:
                    p_cc_pdb->p_call_ctx->call_waiting_call = ICF_TRUE;
                    *p_event = ICF_CC_SSA_SETUP_REQ;
                    break;

            case ICF_USER_CALL_ALERT:
                    /* 
                     * If this was an outgoing call then we can't get alert from PA. 
                     * But in case of some misbehaviour we would be ignoring the message
                     */
                    ICF_HANDLE_REINVITE_START
                        /*
                         * If there is an incoming call modify request then PA may send an 
                         * alert inspite of it being an outgoing call. So we cannt make it an
                         * invalid event
                         */
                        ICF_HANDLE_REINVITE_END
                        if (ICF_NULL == ICF_CC_CHECK_COMMON_BITMASK(p_cc_pdb,
                                    ICF_CALL_DIR))
                        {
                            ICF_HANDLE_REINVITE_START
                                if(ICF_CC_15_AWT_CALL_MODIFY_ALERT_RESP == ICF_CC_GET_CALL_STATE(p_cc_pdb))
                                {
                                    *p_event = ICF_CC_PA_ALERT;
                                    /* Added for sending traces */
#ifdef ICF_TRACE_ENABLE
                                    p_cc_pdb->early_media=
                                        ((icf_local_user_alerted_req_st *)
                                        p_cc_pdb->p_glb_pdb->p_recv_msg->
                                            payload)->early_media_reqd;
                                    ICF_CC_APP_TRACE(p_cc_pdb,ICF_TRACE_ALERT_FROM_APP)
#endif                                    
                                }
                                else
                                {
                                    *p_event = ICF_CC_INVALID_EVENT;
                                }
                        }
                    ICF_HANDLE_REINVITE_END
                        else
                        {
                            *p_event = ICF_CC_PA_ALERT;
#ifdef ICF_TRACE_ENABLE
                            /* Added for sending traces */
                            p_cc_pdb->early_media=
                                 ((icf_local_user_alerted_req_st *)
                                 p_cc_pdb->p_glb_pdb->p_recv_msg->
                                 payload)->early_media_reqd;
                            ICF_CC_APP_TRACE(p_cc_pdb,ICF_TRACE_ALERT_FROM_APP)
                            p_cc_pdb->p_call_ctx->p_trace_criteria_data->call_type |=
                                 ICF_TRACE_ALERTED_CALL;
                            p_cc_pdb->p_call_ctx->p_trace_criteria_data->bitmask |=
                                 ICF_TRACE_CALL_TYPE_PRESENT;
#endif
                        }
                    break;

            case ICF_USER_CALL_CONNECT:
                    /* 
                     * If this was an outgoing call then we can't get alert from PA. 
                     * But in case of some misbehaviour we would be ignoring the message
                     */ 
                    if (ICF_NULL == ICF_CC_CHECK_COMMON_BITMASK(p_cc_pdb,
                                ICF_CALL_DIR))
                    {
                        *p_event = ICF_CC_INVALID_EVENT;
                    }

                    else
                    {
#ifdef ICF_STAT_ENABLED
                        /*Remove this call from missed call list and add it to received 
                         * call list as connect is received for the call*/
                        icf_sm_set_recd_call(p_cc_pdb->p_glb_pdb);
#endif
                        *p_event = ICF_CC_PA_CALL_CONNECT;
                    }

                    break;

                    /* This api is used by SSA to indicate prack for 183 */
            case ICF_NW_INC_MEDIA_PARAM_PRACK:
                    *p_event = ICF_CC_MEDIA_PARAM_PRACK;
                    break;

                    /* This api is used by SSA to indicate prack for 180(alert) */
            case ICF_NW_INC_ALERT_PRACK:
                    *p_event = ICF_CC_ALERT_PRACK;
                    break;

                    /*  This api is used by SSA to indicate ack for 200 ok of invite */
            case ICF_NW_INC_CONNECT_ACK:
                    *p_event = ICF_CC_SSA_CONNECT_ACK;
                    break;

                    /* This API is used to indicate that CANCEL has been received from NW*/
            case ICF_NW_INC_CALL_ABORT:
            /* REl 8.1
             * By pass the handling of Fax 
             * when tunnel mode is ON.
             */
ICF_CHECK_TUNNEL_MODE_START(p_cc_pdb->p_glb_pdb, ICF_FALSE)
                    if(ICF_CC_SSA_FAX_REQ == ICF_CC_GET_ONGOING_EVENT(p_cc_pdb))
                    {
                        *p_event = ICF_CC_SSA_STOP_FAX_REQ;
                    }
ICF_CHECK_TUNNEL_MODE_END
                    if(ICF_CC_SSA_FAX_REQ != ICF_CC_GET_ONGOING_EVENT(p_cc_pdb))
                    {
                        if(ICF_NULL != 
                            ICF_CC_CHECK_COMMON_BITMASK(p_cc_pdb, 
                                   ICF_CALL_CONNECT_DONE))
                         {
/*cancel_reinvite*/
                                ICF_CC_SET_COMMON_BITMASK_2(p_cc_pdb, 
                                               ICF_INCOMING_CALL_MOD_CANCEL_REQ)
                         }
                        *p_event = ICF_CC_SSA_CALL_CANCEL_REQ;
                    }
                    break;


                    /* This API indicates that call need to be cleared due to some failure
                     * or config request   */
#ifdef ICF_SERVICE_ENABLE
            case ICF_SC_SERVICE_ERROR_IND:
                    /*if service error comes and cause is set to forward with user busy 
                     * or forward unconditional or forward no-answer or call forwarded, 
                     * set the network call cleared flag and should not send reject. */
                    if(ICF_CALL_CLEAR_REASON_CALL_FORWARDED == 
                            p_cc_pdb->p_call_ctx->release_cause)
                    {
                        ICF_CC_SET_CALL_CLR_STATUS(p_cc_pdb, 
                                ICF_CALL_CLEAR_STATUS_NW_CLEARED)
                    }            
                    *p_event = ICF_CC_GENERIC_ERROR;
                    break;
#endif

                    /* This API indicates that call need to be cleared due to some failure
                     * or config request   */
            case ICF_GENERIC_CALL_CLEAR:
            case ICF_SC_SRV_ERR_IND_W_PAYLOAD:

                    *p_event = ICF_CC_GENERIC_ERROR;

                    /* The following code change is done to send a failure response to the
                       application for the request API ICF_GENERIC_MSG_REQ. SSA will send 
                       ICF_GENERIC_CALL_CLEAR to CC in case of a transaction timeout. In such
                       a scenario, the request API has to be responded with a failure
                       ICF_GENERIC_MSG_RESP.
                    */
                    if(ICF_CC_CHECK_COMMON_BITMASK_2(p_cc_pdb, ICF_GENERIC_MSG_REQ_ONGOING))
    	            {
                        /* This failure cause will be used by the function 
                           icf_cc_send_pa_generic_msg_resp to populate the
                           error_cause field of API ICF_GENERIC_MSG_RESP.
                        */
                        p_cc_pdb->p_call_ctx->app_failure_cause =
                            ICF_CAUSE_GENERIC_MSG_REQ_INTERNAL_FAILURE;
                       
                        /* The following bit is used by the function
                           icf_cc_send_pa_generic_msg_resp to ascertain 
                           whether to send a success or a failure response to
                           the application.
                        */   
            	        ICF_CC_SET_PDB_COMMON_BITMASK(p_cc_pdb, ICF_GENERIC_MSG_RESP_FAILURE)
                        icf_cc_send_pa_generic_msg_resp(p_cc_pdb);

                        /* Also reset signalling ongoing flag and ICF_GENERIC_MSG_REQ_ONGOING 
                           bit set in the function icf_cc_xx_pa_generic_msg_req.
                        */
                        ICF_CC_RESET_SIG_ONGOING(p_cc_pdb);
                        /* This bit needs to be reset so that if another generic call
                           clear is received from SSA, failure ICF_GENERIC_MSG_RESP is not sent
                           to the application again.
                        */   
                        ICF_CC_RESET_COMMON_BITMASK_2(p_cc_pdb,ICF_GENERIC_MSG_REQ_ONGOING)
                    }
                    break;

                    /* MRM send this API to get forcefully call cleared     */
            case ICF_FORCEFUL_CALL_CLEAR_IND:
                    *p_event = ICF_CC_FORCEFUL_CLEAR;
                    break;
                    /* This Event is generated on reception of response for PRACK */
            case ICF_NW_INC_MEDIA_PARAM_PRACK_RESP:
            /*  If it is tunnel mode then send indication to APPLICATION */
ICF_CHECK_TUNNEL_MODE_START(p_cc_pdb->p_glb_pdb,ICF_TRUE)                
                    ret_val = icf_cc_handle_prack_resp_in_tunnel_mode(p_cc_pdb);
ICF_CHECK_TUNNEL_MODE_END

#ifdef ICF_QOS_SUPPORT
ICF_CHECK_QOS_SUPPORT_START(p_cc_pdb->p_glb_pdb,ICF_TRUE)
                    *p_event = ICF_CC_MEDIA_PARAM_PRACK_RESP;
ICF_CHECK_QOS_SUPPORT_END
#endif
                    break;
                    /* This Event is generated on reception of response for UPDATE
                     * while Qos preconditions processing
                     */
            case ICF_NW_INC_MEDIA_UPDATE_RESP:
/* Rel 8.1
 * Bypass the fax handling.
 * if tunnel mode is ON
 */
ICF_CHECK_TUNNEL_MODE_START(p_cc_pdb->p_glb_pdb, ICF_FALSE)
                    if(ICF_NULL != ICF_CC_CHECK_COMMON_BITMASK(p_cc_pdb,
                                ICF_FAX_CALL_ESTABLISHED))
                    {
                        *p_event = ICF_CC_SSA_STOP_FAX_RESP;
                    }
                    else if(ICF_SUCCESS == icf_cc_check_fax_resp(p_cc_pdb))
                    {
                        *p_event = ICF_CC_SSA_FAX_RESP;
                    }
ICF_CHECK_TUNNEL_MODE_END
                    if((ICF_CC_SSA_STOP_FAX_RESP != *p_event)
                        && (ICF_CC_SSA_FAX_RESP != *p_event))
                    {
                        if(ICF_CC_12_WAIT_START_PROC_SIG == ICF_CC_GET_CALL_STATE(p_cc_pdb))
                        {
                                icf_cc_free_ssa_msg(p_cc_pdb);
                        }
                        *p_event = ICF_CC_MEDIA_UPDATE_RESP;
                    }

                    break;
            case ICF_NW_INC_CANCEL_COMPLETE_RESP:
/*call_mod_cancel*/
                    if(ICF_NULL != 
                    ICF_CC_CHECK_COMMON_BITMASK_2(p_cc_pdb,ICF_CALL_MOD_CANCEL_REQ) &&
                    (ICF_NULL == 
                    ICF_CC_CHECK_COMMON_BITMASK_2(p_cc_pdb,ICF_CALL_MOD_CANCEL_REINV_REQ)))
                    {
                        *p_event = ICF_CC_SSA_CALL_MODIFY_CANCEL_RESP;
                    }
                    break;

#ifdef ICF_SESSION_TIMER
            case ICF_NW_OTG_SESSION_REFRESH_IND:
                    *p_event = ICF_CC_SESSION_REFRESH_REQ;
                    break;
            case ICF_NW_OTG_SESSION_REFRESH_ACK:
                    *p_event = ICF_CC_SESSION_REFRESH_RESP;
                    break;
#endif

            case ICF_USER_START_FAX_REQ:

                    if(ICF_NULL == 
                            ICF_CC_CHECK_COMMON_BITMASK(p_cc_pdb, ICF_CALL_CONNECT_DONE))
                    {
                        p_cc_pdb->p_call_ctx->app_failure_cause = ICF_CAUSE_CALL_SETUP_IN_PROGRESS;

                        ICF_CC_SET_COMMON_BITMASK(p_cc_pdb, ICF_FAX_RESP_FAIL)

                            /* Send failure response to PA */
                            if (ICF_FAILURE == icf_cc_send_pa_fax_resp(p_cc_pdb))
                            {
                                ret_val = ICF_FAILURE;
                            }
                    }
                    else if((ICF_NULL != p_cc_pdb->p_call_ctx->p_service_ctx)||
                            (ICF_MEDIA_STATE_LOCAL_HELD == (ICF_MEDIA_STATE_LOCAL_HELD &
                                                            p_cc_pdb->p_call_ctx->media_state)) ||
                            (ICF_CALL_STATUS_CONFERENCE == (ICF_CALL_STATUS_CONFERENCE & 
                                                            p_cc_pdb->p_call_ctx->call_status)))
                    {
                        p_cc_pdb->p_call_ctx->app_failure_cause = 
                            ICF_CAUSE_SERVICE_ONGOING;
                        ICF_CC_SET_COMMON_BITMASK(p_cc_pdb, 
                                ICF_FAX_RESP_FAIL)
                            ret_val = icf_cc_send_pa_fax_resp(p_cc_pdb);
                    }
                    else if((ICF_MEDIA_STATE_LOCAL_HELD == (ICF_MEDIA_STATE_LOCAL_HELD &
                                    p_cc_pdb->p_call_ctx->media_state)) ||
                            (ICF_CALL_STATUS_CONFERENCE == (ICF_CALL_STATUS_CONFERENCE & 
                                                            p_cc_pdb->p_call_ctx->call_status)))
                    {
                        p_cc_pdb->p_call_ctx->app_failure_cause = 
                            ICF_CAUSE_SERVICE_ONGOING;
                        ICF_CC_SET_COMMON_BITMASK(p_cc_pdb, 
                                ICF_FAX_RESP_FAIL)
                            ret_val = icf_cc_send_pa_fax_resp(p_cc_pdb);
                    }
                    else
                    {
                        icf_start_fax_req_st *p_fax_req = ICF_NULL;

                        /* SPR 19218 : Adding protective NULL checks for the merge of 
                           CSR 1-6200296 */
                        if((ICF_NULL != p_cc_pdb->p_glb_pdb->p_recv_msg) &&
                           (ICF_NULL != p_cc_pdb->p_glb_pdb->p_recv_msg->payload))
                        {
                            p_fax_req = (icf_start_fax_req_st *)
                                p_cc_pdb->p_glb_pdb->p_recv_msg->payload;
                        }
                        else
                        {
                            ICF_PRINT(((icf_uint8_t *)"[CC]:payload in recv msg is null\n"))
                            return ICF_FAILURE;
                        }    

			            p_cc_pdb->p_call_ctx->call_modify_method_bitmask 
            				|= ICF_METHOD_START_FAX_REQ;

                        /*CSR 1-6200296: setting bit from API to call-ctx*/
                        if(p_fax_req->bitmask & ICF_START_FAX_REQ_CLEAR_CALL_ON_415)
                        {
                            p_cc_pdb->p_call_ctx->common_bitmask_2 |= ICF_CLEAR_CALL_ON_FAX_FAILURE_415;
                        }
                        else
                        {
                            p_cc_pdb->p_call_ctx->common_bitmask_2 &= ~ICF_CLEAR_CALL_ON_FAX_FAILURE_415;
                        }
                        *p_event = ICF_CC_PA_FAX_REQ;
                    }
                    break;

            case ICF_USER_EARLY_CALL_CFM:
                    *p_event = ICF_CC_PA_EARLY_CALL_CFM;
#ifdef ICF_TRACE_ENABLE
                    /* Added for sending traces */
                    p_cc_pdb->result= ((icf_early_incoming_call_cfm_st *)
                            p_cc_pdb->p_glb_pdb->p_recv_msg->payload)->result; 
					/* CSR 1-6240518: get the line id from the payload received */
					if(ICF_SUCCESS == p_cc_pdb->result)
					{
						p_cc_pdb->p_call_ctx->line_id = ((icf_early_incoming_call_cfm_st *)
                            p_cc_pdb->p_glb_pdb->p_recv_msg->payload)->line_id;
					}                    
                    ICF_CC_APP_TRACE(p_cc_pdb,ICF_TRACE_EARLY_INC_CALL_CFM)
#endif
                    break;
            case ICF_USER_CALL_MEDIA_CONNECT:
                    *p_event = ICF_CC_PA_MEDIA_CONNECT_REQ;
                    /* Added for sending traces */
                    ICF_CC_APP_TRACE(p_cc_pdb,ICF_TRACE_MEDIA_CONNECT_FROM_APP);
                    break;

            case ICF_USER_STOP_FAX_REQ:
                    /* Added for sending traces */
                    ICF_CC_APP_TRACE(p_cc_pdb,ICF_TRACE_STOP_FAX_FROM_APP)
                    if(ICF_NULL == 
                            ICF_CC_CHECK_COMMON_BITMASK(p_cc_pdb, ICF_CALL_CONNECT_DONE))
                    {
                        p_cc_pdb->p_call_ctx->app_failure_cause = ICF_CAUSE_CALL_SETUP_IN_PROGRESS;

                        ICF_CC_SET_COMMON_BITMASK(p_cc_pdb, ICF_FAX_RESP_FAIL)

                            /* Send failure response to PA */
                            if (ICF_FAILURE == icf_cc_send_pa_stop_fax_resp(p_cc_pdb))
                            {
                                ret_val = ICF_FAILURE;
                            }
                    }
                    else
		    {
			if((ICF_METHOD_STOP_FAX_REQ ==
					(ICF_METHOD_STOP_FAX_REQ &
                        		p_cc_pdb->p_call_ctx->call_modify_method_bitmask)))
			{
				p_cc_pdb->p_call_ctx->app_failure_cause = ICF_CAUSE_STOP_FAX_REQ_ONGOING;       
		        	ICF_CC_SET_COMMON_BITMASK(p_cc_pdb, ICF_FAX_RESP_FAIL)
				copy_fax_type = p_cc_pdb->p_call_ctx->fax_type;
        			icf_cc_send_pa_stop_fax_resp(p_cc_pdb);
				ICF_CC_SET_COMMON_BITMASK(p_cc_pdb, ICF_FAX_CALL_ESTABLISHED)
				p_cc_pdb->p_call_ctx->fax_type = copy_fax_type;
			}
			else if((ICF_METHOD_CALL_MODIFY_REQ ==
					(ICF_METHOD_CALL_MODIFY_REQ &
                        		p_cc_pdb->p_call_ctx->call_modify_method_bitmask)))
			{
				p_cc_pdb->p_call_ctx->app_failure_cause = 
					ICF_CAUSE_CALL_MODIFY_REQ_PENDING;       
		        	ICF_CC_SET_COMMON_BITMASK(p_cc_pdb, ICF_FAX_RESP_FAIL)
        			icf_cc_send_pa_stop_fax_resp(p_cc_pdb);
			}
			else
			{
			    	p_cc_pdb->p_call_ctx->call_modify_method_bitmask 
						|= ICF_METHOD_STOP_FAX_REQ;
                        	*p_event = ICF_CC_PA_STOP_FAX_REQ;
			}
		    }
                    break;
/*call_mod_cancel*/ 
            case ICF_USER_CALL_MOD_CANCEL_REQ:
                    if(ICF_CFG_CALL_MODIFY_METHOD_INVITE !=
                                p_config_data->call_modify_req_method)
                    {
                        /*Send failure response as call modify method reinvite is not supported*/
                        p_cc_pdb->p_call_ctx->app_failure_cause = ICF_CAUSE_CALL_MODIFY_METHOD_NOT_INVITE;

                        ICF_CC_SET_COMMON_BITMASK(p_cc_pdb, ICF_CALL_MOD_CANCEL_RESP_FAIL)

                        /* Send failure response to PA */
                        if (ICF_FAILURE == icf_cc_send_pa_call_modify_cancel_resp(p_cc_pdb))
                        {
                            ret_val = ICF_FAILURE;
                        }
                        return ret_val;
                    }
                    else if(ICF_NULL ==
                            ICF_CC_CHECK_COMMON_BITMASK(p_cc_pdb, ICF_CALL_CONNECT_DONE))
                    {
                        p_cc_pdb->p_call_ctx->app_failure_cause = ICF_CAUSE_CALL_SETUP_IN_PROGRESS;

                        ICF_CC_SET_COMMON_BITMASK(p_cc_pdb, ICF_CALL_MOD_CANCEL_RESP_FAIL)

                        /* Send failure response to PA */
                        if (ICF_FAILURE == icf_cc_send_pa_call_modify_cancel_resp(p_cc_pdb))
                        {
                            ret_val = ICF_FAILURE;
                        }
                    }
                    else if ((p_cc_pdb->p_call_ctx->call_modify_method_bitmask
                             & ICF_METHOD_START_FAX_REQ ) ||
                            (p_cc_pdb->p_call_ctx->fax_type != ICF_NULL))
                    {
                        /*Send failure response as invalid request as call modify cancel can't be done 
                          over fax call*/
                        p_cc_pdb->p_call_ctx->app_failure_cause = ICF_CAUSE_INVALID_REQUEST;

                        ICF_CC_SET_COMMON_BITMASK(p_cc_pdb, ICF_CALL_MOD_CANCEL_RESP_FAIL)

                        /* Send failure response to PA */
                        if (ICF_FAILURE == icf_cc_send_pa_call_modify_cancel_resp(p_cc_pdb))
                        {
                            ret_val = ICF_FAILURE;
                        }
                    }
                    else
                    {
                        p_cc_pdb->p_call_ctx->common_bitmask_2
                                                          |= ICF_CALL_MOD_CANCEL_REQ;
                        *p_event = ICF_CC_PA_CALL_MODIFY_CANCEL_REQ;
                    }
                    break;

            case ICF_NW_INC_CALL_PROGRESS:
			*p_event = ICF_CC_SSA_CALL_PROGRESS;
			break;

            case ICF_SRTP_FALLBACK_REQ: 
                        *p_event = ICF_CC_SRTP_FALLBACK_REQ;
                        break;

#ifdef ICF_SESSION_TIMER                        
            case ICF_USER_SESSION_TIMER_TOGGLE_REQ: 
                  ret_val = icf_cc_proc_pa_session_timer_toggle_payload(p_cc_pdb);
                  break;
#endif           
            /* Added for in call REFER/NOTIFY enhancements */
            case ICF_USER_GENERIC_MSG_REQ: 
                 *p_event = ICF_CC_PA_GENERIC_MSG_REQ;               
                 break;

            case ICF_NW_INC_GENERIC_MSG_IND: 
                 *p_event = ICF_CC_SSA_GENERIC_MSG_IND;               
                 break;

            case ICF_USER_GENERIC_MSG_CFM: 
                 *p_event = ICF_CC_PA_GENERIC_MSG_CFM;               
                 break;

            case ICF_NW_INC_GENERIC_MSG_RESP: 
                 *p_event = ICF_CC_SSA_GENERIC_MSG_RESP;               
                 break;
         
            /* End REFER/NOTIFY Enhancements */     
            default:
                    *p_event = ICF_CC_INVALID_EVENT;
                    break;
        } /* switch(p_pdb->p_internal_msg->... */
        /*SPR 19590 change start*/
        /*If the event is not invalid then invoke the utility function
          for cloning the header/body/tag list from the API payload into
          the call ctx lists of list*/
        if(ICF_CC_INVALID_EVENT != *p_event) 
        {
            ret_val = icf_cc_make_hdr_body_tag_list_from_api(p_cc_pdb,
                                                   p_event);
        }    
        /*SPR 19590 change end*/
    }
    else
    {
       icf_cc_ignore_msg(p_cc_pdb);
    }

    ICF_PRINT(("\n[CC]: Current-Event:: %d\n",*p_event));
    return ret_val;
} /* end of icf_cc_convert_api_to_event() */


/*******************************************************************************
 *
 * FUNCTION:        icf_cc_ignore_msg
 *
 * DESCRIPTION:     This function is invoked when an event needs to be ignored.
 *
 ******************************************************************************/
icf_return_t  icf_cc_ignore_msg(
        INOUT icf_cc_pdb_st *p_cc_pdb)
{
    icf_return_t ret_val = ICF_SUCCESS;

    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /* function entry trace */

    switch(p_cc_pdb->current_event)
          {
            case ICF_CC_PA_CALL_MODIFY_REQ :
                              ret_val = icf_cc_pa_handle_unexpected_call_modify_req(p_cc_pdb);
                              break;
            case ICF_CC_PA_FAX_REQ :
                              ret_val = icf_cc_pa_handle_unexpected_fax_req(p_cc_pdb);
                              break;
            case ICF_CC_PA_INFO_REQ:
                              ret_val = icf_cc_pa_handle_unexpected_info_req(p_cc_pdb);
                              break;
            case ICF_CC_PA_STOP_FAX_REQ:
                              ret_val = icf_cc_pa_handle_unexpected_stop_fax_req(p_cc_pdb);
                              break;

            case ICF_CC_PA_ALERT:
            case ICF_CC_PA_CALL_CONNECT:
            case ICF_CC_PA_EARLY_CALL_CFM:
            case ICF_CC_PA_MEDIA_CONNECT_REQ:
            case ICF_CC_PA_CALL_TERMINATE_REQ:
            case ICF_CC_PA_CALL_MODIFY_RSP:
                              ret_val = icf_cc_send_pa_unexpected_msg_ind(p_cc_pdb);
                              break;
          }
    
     /* This event is not expected in current state, therefore do nothing */
	ICF_CC_TRACE(p_cc_pdb, ICF_UNEXPECTED_EVENT)

    icf_cc_free_ssa_msg(p_cc_pdb);

    
    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */
    return ret_val;
}


/*******************************************************************************
 *
 * FUNCTION:        icf_cc_free_ssa_msg
 *
 * DESCRIPTION:     This function is invoked to free ssa message if it is ignored.
 *
 ******************************************************************************/
icf_return_t  icf_cc_free_ssa_msg(
        INOUT icf_cc_pdb_st *p_cc_pdb)
{
    icf_return_t ret_val = ICF_SUCCESS;

    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /* function entry trace */

    switch(p_cc_pdb->p_internal_msg->msg_hdr.msg_id)
    {
        case ICF_NW_INC_ALERT_WITH_MEDIA:
            {
                icf_nw_inc_alert_media_st *p_msg = 
                    (icf_nw_inc_alert_media_st *)
                            p_cc_pdb->p_internal_msg->p_msg_data;

                if(ICF_NULL != p_msg->p_sdp_list)
                {
                    ret_val = icf_cc_delete_inc_media_sdp_info_list(
				        p_cc_pdb,&p_msg->p_sdp_list);
                }
                break;
            }

        case ICF_NW_INC_CONNECT:
            {
                icf_nw_inc_connect_st *p_msg = 
                    (icf_nw_inc_connect_st *)
                            p_cc_pdb->p_internal_msg->p_msg_data;

                if(ICF_NW_INC_CONNECT_WITH_SDP ==
                    (ICF_NW_INC_CONNECT_WITH_SDP & p_msg->bitmask))
                {
                    if(ICF_NULL != p_msg->p_sdp_list)
                    {
                        ret_val = icf_cc_delete_inc_media_sdp_info_list(
				            p_cc_pdb,&p_msg->p_sdp_list);
                    }
                }
                break;
            }

        case ICF_NW_INC_MEDIA_UPDATE:
            {
                icf_nw_inc_media_change_req_st *p_msg = 
                    (icf_nw_inc_media_change_req_st *)
                            p_cc_pdb->p_internal_msg->p_msg_data;

                if(ICF_NW_INC_MEDIA_CHG_SDP_PRESENT ==
                    (ICF_NW_INC_MEDIA_CHG_SDP_PRESENT & p_msg->bitmask))
                {
                    if(ICF_NULL != p_msg->p_sdp_list)
                    {
                        ret_val = icf_cc_delete_inc_media_sdp_info_list(
				            p_cc_pdb,&p_msg->p_sdp_list);
                    }
                }
                if(ICF_NW_INC_MEDIA_ADD_STREAM ==
                    (ICF_NW_INC_MEDIA_ADD_STREAM & p_msg->bitmask))
                {
                    if(ICF_NULL != p_msg->p_sdp_list)
                    {
                        ret_val = icf_cc_delete_inc_media_sdp_info_list(
				            p_cc_pdb,&p_msg->p_added_sdp_list);
                    }
                }
                break;
            }

        case ICF_NW_INC_MEDIA_PARAM_PRACK:
        case ICF_NW_INC_ALERT_PRACK:
            {
                icf_nw_inc_prack_st *p_msg = 
                    (icf_nw_inc_prack_st *)
                            p_cc_pdb->p_internal_msg->p_msg_data;

                if(ICF_NW_INC_PRACK_SDP ==
                    (ICF_NW_INC_PRACK_SDP & p_msg->bitmask))
                {
                    if(ICF_NULL != p_msg->p_sdp_list)
                    {
                        ret_val = icf_cc_delete_inc_media_sdp_info_list(
				            p_cc_pdb,&p_msg->p_sdp_list);
                    }
                }
                break;
            }
        
        case ICF_NW_INC_MEDIA_PARAM:
            {
                icf_nw_inc_media_param_st *p_msg = 
                    (icf_nw_inc_media_param_st *)
                            p_cc_pdb->p_internal_msg->p_msg_data;
               
                if(ICF_NULL != p_msg->p_sdp_list)
                {
                    ret_val = icf_cc_delete_inc_media_sdp_info_list(
				        p_cc_pdb,&p_msg->p_sdp_list);
                }
                
                break;
            }

        case ICF_NW_INC_CONNECT_ACK:
            {
                icf_nw_inc_connect_ack_st *p_msg = 
                    (icf_nw_inc_connect_ack_st *)
                            p_cc_pdb->p_internal_msg->p_msg_data;

                if(ICF_NW_INC_CONNECT_ACK_SDP ==
                    (ICF_NW_INC_CONNECT_ACK_SDP & p_msg->bitmask))
                {
                    if(ICF_NULL != p_msg->p_sdp_list)
                    {
                        ret_val = icf_cc_delete_inc_media_sdp_info_list(
				            p_cc_pdb,&p_msg->p_sdp_list);
                    }
                }
                break;
            }

        case ICF_NW_INC_MEDIA_CHANGE_ACK:
            {
                icf_nw_inc_media_change_ack_st *p_msg = 
                    (icf_nw_inc_media_change_ack_st *)
                            p_cc_pdb->p_internal_msg->p_msg_data;

                if(ICF_NW_INC_MC_ACK_SDP_PRESENT ==
                    (ICF_NW_INC_MC_ACK_SDP_PRESENT & p_msg->bitmask))
                {
                    if(ICF_NULL != p_msg->p_sdp_list)
                    {
                        ret_val = icf_cc_delete_inc_media_sdp_info_list(
				            p_cc_pdb,&p_msg->p_sdp_list);
                    }
                }
                break;
            }
        
        case ICF_NW_INC_MEDIA_UPDATE_RESP:
            {
                icf_nw_inc_media_change_resp_st *p_msg = 
                    (icf_nw_inc_media_change_resp_st *)
                            p_cc_pdb->p_internal_msg->p_msg_data;

                if(ICF_NW_INC_MC_SDP_PRESENT ==
                    (ICF_NW_INC_MC_SDP_PRESENT & p_msg->bitmask))
                {
                    if(ICF_NULL != p_msg->p_sdp_list)
                    {
                        ret_val = icf_cc_delete_inc_media_sdp_info_list(
				            p_cc_pdb,&p_msg->p_sdp_list);
                    }
                }
                break;
            }

        default:
            break;
    }

    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */
    return ret_val;
}

/******************************************************************************
 *
 * FUNCTION:        icf_cc_xx_pa_generic_msg_cfm
 *
 * DESCRIPTION:     This function is used to send a response for a generic
 *                  message received from the peer. 
 *****************************************************************************/
icf_return_t icf_cc_xx_pa_generic_msg_cfm(
        INOUT  icf_cc_pdb_st *p_cc_pdb)
{
    /* Rel 8.2 Klocworks fix */
    icf_nw_otg_generic_msg_cfm_st generic_msg = {{0,"\0"},0};
    /* Rel 8.2 Klocworks fix end*/
    icf_generic_msg_cfm_st        *p_generic_msg_cfm = ICF_NULL;
    icf_return_t                  ret_val = ICF_SUCCESS;
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

    p_generic_msg_cfm = (icf_generic_msg_cfm_st *)
        (p_cc_pdb->p_glb_pdb->p_recv_msg->payload);

    /* If any generic message has not been received from the network, then reject this API
       with unexpected msg ind.
    */

    if(ICF_NULL == ICF_CC_CHECK_COMMON_BITMASK_2(p_cc_pdb, ICF_GENERIC_MSG_IND_ONGOING))
    {
        ret_val = icf_cc_send_pa_unexpected_msg_ind(p_cc_pdb);
    }
    else
    {
        /* Valid request, some request has been received from the peer.
           Route this request to SSA to send a response to an incoming
           request received from the peer.
        */
        

        if(ICF_SUCCESS == ret_val)
        {
            /* Copy the fields from the API payload to the internal message and trigger SSA */

            /* Copy method name */
            icf_port_strcpy(generic_msg.method_name.str,p_generic_msg_cfm->method_name.str);
            generic_msg.method_name.str_len = icf_port_strlen(p_generic_msg_cfm->method_name.str);

            /* Copy response code */
            generic_msg.response_code = p_generic_msg_cfm->response_code;

            /* Fill internal API ID and trigger SSA */
            p_cc_pdb->p_internal_msg->msg_hdr.msg_id = ICF_NW_OTG_GENERIC_MSG_CFM ;
            p_cc_pdb->p_internal_msg->msg_hdr.payload_length = sizeof(generic_msg);
            p_cc_pdb->p_internal_msg->p_msg_data = (icf_uint8_t *)(&generic_msg);

            /* Call entry point of SSA */
            ret_val = icf_ssa_process_mesg(p_cc_pdb->p_internal_msg);
        }
        else 
        {
            /* Issue an alarm to the application. */
            ICF_ALARM(((icf_uint8_t *)"[CC]:Failure in processing ICF_GENERIC_MSG_CFM API\n"))
        }
    }
    if(ICF_SUCCESS == ret_val)
    {
        /* If the API ICF_GENERIC_MSG_CFM is successfuly processed and a response 
           has been sent successfuly to the peer, reset the following bit. Now we
           are ready to receive further generic requests from the peer.
        */
        ICF_CC_RESET_COMMON_BITMASK_2(p_cc_pdb,ICF_GENERIC_MSG_IND_ONGOING)
    }
    else
    {
        /* Issue an alarm to the application. */
        ICF_ALARM(((icf_uint8_t *)"[CC]:Failure in processing ICF_GENERIC_MSG_CFM API\n"))
    }

    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */
    return ret_val;
}

/******************************************************************************
 *
 * FUNCTION:        icf_cc_xx_ssa_generic_msg_ind
 *
 * DESCRIPTION:     This function handles a generic message request 
 *                  received from the peer. 
 *****************************************************************************/
icf_return_t icf_cc_xx_ssa_generic_msg_ind(
        INOUT  icf_cc_pdb_st *p_cc_pdb)
{
    icf_return_t                  ret_val = ICF_SUCCESS;

    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb)
    
    /* Call the handler function */
    ret_val = icf_cc_send_pa_generic_msg_ind(p_cc_pdb);

    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb)
    return ret_val;    
}

/******************************************************************************
 *
 * FUNCTION:        icf_cc_xx_ssa_generic_msg_resp
 *
 * DESCRIPTION:     This function handles a generic message response received 
 *                  from the peer. 
 *****************************************************************************/
icf_return_t icf_cc_xx_ssa_generic_msg_resp(
        INOUT  icf_cc_pdb_st *p_cc_pdb)
{
    icf_return_t                  ret_val = ICF_SUCCESS;

    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb)
    
    /* Call the handler function */
    ret_val = icf_cc_send_pa_generic_msg_resp(p_cc_pdb);

    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb)
    return ret_val;
}

/******************************************************************************
 *
 * FUNCTION:        icf_cc_xx_pa_generic_msg_req
 *
 * DESCRIPTION:     This function handles a generic message request 
 *                  received from the application. 
 *****************************************************************************/
icf_return_t icf_cc_xx_pa_generic_msg_req(
        INOUT icf_cc_pdb_st *p_cc_pdb)
{
    icf_return_t                  ret_val = ICF_SUCCESS;
    icf_nw_otg_generic_msg_req_st nw_otg_generic_msg_req = {{0, "\0"}};
    icf_pa_cc_generic_msg_req_st  *p_generic_msg_req = ICF_NULL;

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
    
    p_generic_msg_req = 
       (icf_pa_cc_generic_msg_req_st *)p_cc_pdb->p_glb_pdb->p_recv_msg->payload;
    
    /* copy method name */
    icf_port_strcpy(nw_otg_generic_msg_req.method_name.str,
                    p_generic_msg_req->method_name.str);
    nw_otg_generic_msg_req.method_name.str_len =
           icf_port_strlen(p_generic_msg_req->method_name.str);

    p_cc_pdb->p_internal_msg->msg_hdr.msg_id = ICF_NW_OTG_GENERIC_MSG_REQ;
    p_cc_pdb->p_internal_msg->msg_hdr.payload_length = 
        sizeof(nw_otg_generic_msg_req);
    p_cc_pdb->p_internal_msg->p_msg_data = (icf_uint8_t*)(&nw_otg_generic_msg_req);

    /* Populate the method name to be sent in the failure response */
    icf_port_strcpy(p_cc_pdb->p_call_ctx->generic_req_method_name.str,
                    p_generic_msg_req->method_name.str);
    p_cc_pdb->p_call_ctx->generic_req_method_name.str_len =
       icf_port_strlen(p_generic_msg_req->method_name.str);

    /* If a generic message request is already pending, reject this 
       request.
    */   
    if(ICF_CC_CHECK_COMMON_BITMASK_2(p_cc_pdb, ICF_GENERIC_MSG_REQ_ONGOING))
    {
        /* reject the request*/
        p_cc_pdb->p_call_ctx->app_failure_cause =
            ICF_CAUSE_GENERIC_MSG_REQ_ALREADY_PENDING;

        ICF_CC_SET_PDB_COMMON_BITMASK(p_cc_pdb, ICF_GENERIC_MSG_RESP_FAILURE)

        /* Send failure response for generic_msg_req to user */
        if(ICF_FAILURE == icf_cc_send_pa_generic_msg_resp(p_cc_pdb))
        {
            ret_val = ICF_FAILURE;
        }
        ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */
        return ret_val;
    }

    /* If signalling ongoing is happening or the call is not connected
       then reject the generic_msg_req API sending failure
       generic_msg_resp
     */
    if((ICF_INT_MODULE_INVALID != ICF_CC_GET_SIG_ONGOING(p_cc_pdb)) ||
       (!(ICF_CC_CHECK_COMMON_BITMASK(p_cc_pdb, ICF_CALL_CONNECT_DONE))))
    {
        /* reject the request*/
        p_cc_pdb->p_call_ctx->app_failure_cause =
            ICF_CAUSE_GENERIC_MSG_REQ_NOT_ALLOWED;

        ICF_CC_SET_PDB_COMMON_BITMASK(p_cc_pdb, ICF_GENERIC_MSG_RESP_FAILURE)

        /* Send failure response for generic_msg_req to user */
        if(ICF_FAILURE == icf_cc_send_pa_generic_msg_resp(p_cc_pdb))
        {
            ret_val = ICF_FAILURE;
        }
        ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */
        return ret_val;
    }

    /* Set signalling ongoing flag */
    ICF_CC_SET_SIG_ONGOING(p_cc_pdb);

    /* Trigger SSA to send request on network */
    ret_val = icf_ssa_process_mesg(p_cc_pdb->p_internal_msg);

    /* If the request has been successfully sent, set bit ICF_GENERIC_MSG_REQ_ONGOING */
    if(ICF_SUCCESS == ret_val)
    {
        ICF_CC_SET_COMMON_BITMASK_2(p_cc_pdb, ICF_GENERIC_MSG_REQ_ONGOING)
    }
    else
    {
        /* Reset signalling ongoing flag here. This is being done as in the function
           icf_cc_send_pa_generic_msg_resp, this is being reset only in case
           a success response is being given to the application.*/
        ICF_CC_RESET_SIG_ONGOING(p_cc_pdb);

        /* SSA has returned failure.Send failure response to application
           and clear call.
        */
        p_cc_pdb->p_call_ctx->app_failure_cause =
            ICF_CAUSE_GENERIC_MSG_REQ_INTERNAL_FAILURE;

        ICF_CC_SET_PDB_COMMON_BITMASK(p_cc_pdb, ICF_GENERIC_MSG_RESP_FAILURE)

        /* Send failure response for generic_msg_req to user */
        icf_cc_send_pa_generic_msg_resp(p_cc_pdb);

        /* Issue an alarm to the application. */
        ICF_ALARM(((icf_uint8_t *)"[CC]:Failure in processing ICF_GENERIC_MSG_REQ API\n"))
    }

    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */
    return ret_val;
}
