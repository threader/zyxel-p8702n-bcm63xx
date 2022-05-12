/*------------------------------------------------------------------------------
 *
 * File name        : icf_cm_prototypes.h
 *
 * Purpose          : This file contains the prototypes of functions used in CM.
 *
 * Revision History :
 *
 * Date         Name            Ref#            Comments
 * --------     ------------    ----            ---------------
 * 21-Jan-2005  Rohit Aggarwal  ICF LLD      Beginning of coding phase
 * 31-Jan-2005  Rohit Aggarwal                  Added a utility function
 * 25-Feb-2005  Rohit Aggarwal                  Added a hash define
 *                                              ICF_CM_SCM_GENERIC_CALL_CLR
 *                                              to remove handlng Generic Call
 *                                              Clear event on SCM's return path
 * 17-May-2005  Rohit Aggarwal                  Fixed SPR 7567
 * 06-Feb-2009  Rohit Aggarwal  SPR 19316       Changes for INVITE with Replaces
 *
 * 
 * Copyright (c) 2006, Aricent.
 *----------------------------------------------------------------------------*/


#ifndef __ICF_CM_PROTOTYPES_H__
#define __ICF_CM_PROTOTYPES_H__


#include "icf_cm_types.h"


/*
 * Determine if a C++ compiler is being used. If so, ensure that standard C is
 * used to process the header information.
 */
#ifdef __cplusplus
/* Yes, C++ compiler is present. Use standard C */
extern "C"
{
#endif /* __cplusplus */


/*------------------------------------------------------------------------------
 * Prototypes of functions in icf_cm_init.c
 *----------------------------------------------------------------------------*/
/* The prototypes of these functions are in common/h file. */
    
          
/*------------------------------------------------------------------------------
 * Prototypes of functions in icf_cm_entry_pt.c
 *----------------------------------------------------------------------------*/

/*******************************************************************************
 *
 * FUNCTION:        icf_cm_process_mesg
 *
 * DESCRIPTION:     This is the entry point function of CM to process an
 *                  incoming message. The message can be received either from
 *                  MRM or RGM or SSA or MMI.
 *                  This function first checks the rules in forward direction
 *                  and then after getting control back, it can send messages
 *                  to either CC or ADM based on the return status or the event
 *                  received.
 *                  The Call Manager does not allocate a new internal message
 *                  or assign a new payload. It just re-routes the message to
 *                  the relevant destination.
 *
 ******************************************************************************/
icf_return_t icf_cm_process_mesg(
        INOUT   icf_internal_msg_st  *p_internal_msg);


/*------------------------------------------------------------------------------
 * Prototypes of functions in icf_cm_db.c
 *----------------------------------------------------------------------------*/
    
#ifdef ICF_SERVICE_ENABLE
/*******************************************************************************
 *
 * FUNCTION:        icf_cm_db_set_forward_data
 *
 * DESCRIPTION:     This function is used to set the rules for routing the
 *                  messages in forward direction into the module's global data.
 *                  It checks on events or APIs to see where to route the
 *                  message.
 *                  For doing a routing, CM goes through the tree structure in
 *                  its global data to find where the appropriate check matches
 *                  with the received message. Once it finds that, it does the
 *                  action associated with the same. In case it does not get
 *                  appropriate check for routing the message in forward
 *                  direction, it routes the message to Call Control as that is
 *                  the default destination for all messages.
 *
 ******************************************************************************/
icf_return_t icf_cm_db_set_forward_data(
        INOUT   icf_cm_pdb_st        *p_cm_pdb);

/*******************************************************************************
 *
 * FUNCTION:        icf_cm_db_set_sc_backward_data
 *
 * DESCRIPTION:     This function sets the rules for re-routing the received
 *                  message or forming a new message after getting back the
 *                  control from Service Controller and routing back to
 *                  Call Control or Service Controller.
 *                  In case the CM does not get the appropriate leaf node that
 *                  matches the received status or the event in backward
 *                  direction, it implies that there is no action required for
 *                  this status and it returns back to the calling function.
 *
 ******************************************************************************/
icf_return_t icf_cm_db_set_sc_backward_data(
        INOUT   icf_cm_pdb_st        *p_cm_pdb);
#endif /* #ifdef ICF_SERVICE_ENABLE */

/*******************************************************************************
 *
 * FUNCTION:        icf_cm_db_set_cc_backward_data
 *
 * DESCRIPTION:     This function sets the rules for re-routing the received
 *                  message or forming a new message after getting back the
 *                  control from Call Control and routing back to Call Control
 *                  or Service Controller.
 *                  In case the CM does not get the appropriate leaf node that
 *                  matches the received status or the event in backward
 *                  direction, it implies that there is no action required for
 *                  this status and it returns back to the calling function.
 *
 ******************************************************************************/
icf_return_t icf_cm_db_set_cc_backward_data(
        INOUT   icf_cm_pdb_st        *p_cm_pdb);


/*------------------------------------------------------------------------------
 * Prototypes of functions in icf_cm_process.c
 *----------------------------------------------------------------------------*/

#ifdef ICF_SERVICE_ENABLE
/*******************************************************************************
 *
 * FUNCTION:        icf_cm_process_forward_msg
 *
 * DESCRIPTION:     This function is called on forward direction so that it can
 *                  check for actions in its forward path. The action function
 *                  is same, but only the root level tree node structure is
 *                  different, hence same function is called indicating from
 *                  which tree it has to be searched.
 *
 ******************************************************************************/
icf_return_t icf_cm_process_forward_msg(
        INOUT   icf_cm_pdb_st        *p_cm_pdb);

/*******************************************************************************
 *
 * FUNCTION:        icf_cm_process_sc_backward_msg
 *
 * DESCRIPTION:     This function is called on return path of call from Service
 *                  Controller so that it can check for actions in its return
 *                  path. The action function is same, but only the root level
 *                  tree node structure is different, hence same function is
 *                  called indicating from which tree it has to be searched.
 *
 ******************************************************************************/
icf_return_t icf_cm_process_sc_backward_msg(
        INOUT   icf_cm_pdb_st        *p_cm_pdb);
#endif /* #ifdef ICF_SERVICE_ENABLE */

/*******************************************************************************
 *
 * FUNCTION:        icf_cm_process_cc_backward_msg
 *
 * DESCRIPTION:     This function is called on return path of call from Call
 *                  Control so that it can check for actions in its return path.
 *                  The action function is same, but only the root level tree
 *                  node structure is different, hence same function is called
 *                  indicating from which tree it has to be searched.
 *
 ******************************************************************************/
icf_return_t icf_cm_process_cc_backward_msg(
        INOUT   icf_cm_pdb_st        *p_cm_pdb);

/*******************************************************************************
 *
 * FUNCTION:        icf_cm_proc_check_action
 *
 * DESCRIPTION:     This function gets the pointer of the root of the tree
 *                  where the data driven logic for routing is kept. Once the
 *                  root level node is identified, it calls a recursive
 *                  function to find the action.
 *
 ******************************************************************************/
icf_return_t icf_cm_proc_check_action(
        INOUT   icf_cm_pdb_st        *p_cm_pdb,
        IN      icf_cm_check_type_t  check_type);

/*******************************************************************************
 *
 * FUNCTION:        icf_cm_proc_recc_new_check
 *
 * DESCRIPTION:     This function the recursive function that checks from the
 *                  root level structure to find the appropriate branch of tree
 *                  which satisfies the condition.
 *
 ******************************************************************************/
icf_return_t icf_cm_proc_recc_new_check(
        INOUT   icf_cm_pdb_st        *p_cm_pdb,
        IN      icf_cm_new_check_st  *p_new_check);

/*******************************************************************************
 *
 * FUNCTION:        icf_cm_proc_a_new_check
 *
 * DESCRIPTION:     This function checks against the field of the tree node to
 *                  find which action has to be processed. If the complete
 *                  check fails then it returns so that the next field can be
 *                  checked from the current level.
 *
 ******************************************************************************/
icf_return_t icf_cm_proc_a_new_check(
        INOUT   icf_cm_pdb_st        *p_cm_pdb,
        IN      icf_cm_new_check_st  *p_new_check);

/*******************************************************************************
 *
 * FUNCTION:        icf_cm_proc_do_action
 *
 * DESCRIPTION:     This function acts on the action part. This would be called
 *                  if the field values matches. The action could be of
 *                  following types:
 *                  (a) Identify to which destination this message has to be
 *                  routed
 *                  (b) Identify a new function that would do complex job of
 *                  setting some variables or could further do some more checks
 *                  (c) Call another branch level checking with new values
 *                  If the action indicates that new level of checking is
 *                  required, the recursive function is called again so that
 *                  now checking can start from that level of tree.
 *
 ******************************************************************************/
icf_return_t icf_cm_proc_do_action(
        INOUT   icf_cm_pdb_st            *p_cm_pdb,
        IN      icf_cm_value_action_st   *p_value_action);


/*------------------------------------------------------------------------------
 * Prototypes of functions in icf_cm_util.c
 *----------------------------------------------------------------------------*/

#ifdef ICF_TRACE_ENABLE
/*******************************************************************************
 *
 * FUNCTION:        icf_cm_trace
 *
 * DESCRIPTION:     This function is used by CM for trace handling.
 *
 ******************************************************************************/
icf_void_t icf_cm_trace(
        INOUT   icf_cm_pdb_st        *p_cm_pdb,
        IN      icf_trace_id_t       trace_id);
#endif

#ifdef ICF_ERROR_ENABLE
/*******************************************************************************
 *
 * FUNCTION:        icf_cm_error
 *
 * DESCRIPTION:     This function is used by CM for error handling.
 *
 ******************************************************************************/
icf_void_t icf_cm_error(
        INOUT   icf_cm_pdb_st        *p_cm_pdb,
        IN      icf_error_t          ecode);
#endif

#ifdef ICF_STAT_ENABLED
/*******************************************************************************
 *
 * FUNCTION:        icf_cm_util_update_stats
 *
 * DESCRIPTION:     This function checks the received api and updates its stats.
 *
 ******************************************************************************/
icf_return_t icf_cm_util_update_stats(
        INOUT   icf_cm_pdb_st            *p_cm_pdb);
#endif

/*******************************************************************************
 *
 * FUNCTION:        icf_cm_util_check_field_agnst_value
 *
 * DESCRIPTION:     This function checks whether the condition value matches
 *                  with the value that has come in the field.
 *
 ******************************************************************************/
icf_return_t icf_cm_util_check_field_agnst_value(
        INOUT   icf_cm_pdb_st            *p_cm_pdb,
        IN      icf_uint32_t             field_value,
        OUT     icf_cm_value_action_st   *p_value_action);        

/*******************************************************************************
 *
 * FUNCTION:        icf_cm_util_convert_api_to_event
 *
 * DESCRIPTION:     This function maps internal APIs to module-specific events.
 *
 ******************************************************************************/
icf_return_t icf_cm_util_convert_api_to_event(
        INOUT   icf_cm_pdb_st            *p_cm_pdb);

/*******************************************************************************
 *
 * FUNCTION:        icf_cm_util_fill_value_action
 *
 * DESCRIPTION:     This function accepts a value_action pointer and fills the 
 *                  action for that particular value in its action pointer. It 
 *                  is used for those actions that do not have further checks.
 *
 ******************************************************************************/
icf_return_t icf_cm_util_fill_value_action(
        INOUT   icf_cm_pdb_st            *p_cm_pdb,
        IN      icf_uint32_t             value,
        IN      icf_uint8_t              opr,
        IN      icf_uint32_t              event,
        IN      icf_int_module_id_t      dest,
        INOUT   icf_cm_value_action_st   **p_p_value_action,
        OUT     icf_error_t              *p_ecode);

#ifdef ICF_SERVICE_ENABLE
/*******************************************************************************
 *
 * FUNCTION:        icf_cm_util_fill_fp_va_forceful_clr
 *
 * DESCRIPTION:     This function defines the value action on forward path for 
 *                  Forceful Call Clear to be sent to SCM.
 *                  This function sets the action only if service context is 
 *                  not null.
 *
 ******************************************************************************/
icf_return_t icf_cm_util_fill_fp_va_forceful_clr(
        INOUT   icf_cm_pdb_st            *p_cm_pdb,
        INOUT   icf_cm_value_action_st   **p_p_value_action_root,
        OUT     icf_error_t              *p_ecode);

/*******************************************************************************
 *
 * FUNCTION:        icf_cm_fp_srv_api_fail
 *
 * DESCRIPTION:     This function forms the failure response to be sent to ADM 
 *                  in case req_api is recd from PA with invalid callid.
 *
 ******************************************************************************/
icf_return_t icf_cm_fp_srv_api_fail(
        INOUT   icf_cm_pdb_st            *p_cm_pdb);

/*******************************************************************************
 *
 * FUNCTION:        icf_cm_util_fill_fp_va_mrm_srv_api
 *
 * DESCRIPTION:     This function checks the received PA service req_api and 
 *                  forwards internal event to ADM if call_ctx is not null, 
 *                  else it sends a failure response to ADM.
 *
 ******************************************************************************/
icf_return_t icf_cm_util_fill_fp_va_mrm_srv_api(
        INOUT   icf_cm_pdb_st            *p_cm_pdb,
        IN      icf_uint32_t             req_api,
        IN      icf_uint32_t              event,
        INOUT   icf_cm_value_action_st   **p_p_value_action_root,
        OUT     icf_uint32_t             resp_api,
        OUT     icf_error_t              *p_ecode);

/*******************************************************************************
 *
 * FUNCTION:        icf_cm_util_fill_fp_va_msg_frm_mrm
 *
 * DESCRIPTION:     This function defines the value action on forward path for 
 *                  messages received from MRM.
 *
 ******************************************************************************/
icf_return_t icf_cm_util_fill_fp_va_msg_frm_mrm(
        INOUT   icf_cm_pdb_st            *p_cm_pdb,
        INOUT   icf_cm_value_action_st   **p_p_value_action_root,
        OUT     icf_error_t              *p_ecode);

/*#ifdef IMS_CLIENT*/
/*******************************************************************************
 *
 * FUNCTION:        icf_cm_util_fill_fp_va_media_update_resp
 *
 * DESCRIPTION:     This function defines the value action on forward path for 
 *                  success response for Reinvite received from SSA.
 *
 ******************************************************************************/
icf_return_t icf_cm_util_fill_fp_va_media_update_resp(
        INOUT   icf_cm_pdb_st            *p_cm_pdb,
        INOUT   icf_cm_value_action_st   **p_p_value_action_root,
        OUT     icf_error_t              *p_ecode);
/*#endif*/

/*******************************************************************************
 *
 * FUNCTION:        icf_cm_util_fill_fp_va_reinvite
 *
 * DESCRIPTION:     This function defines the value action on forward path for 
 *                  Reinvite received from SSA.
 *
 ******************************************************************************/
icf_return_t icf_cm_util_fill_fp_va_reinvite(
        INOUT   icf_cm_pdb_st            *p_cm_pdb,
        INOUT   icf_cm_value_action_st   **p_p_value_action_root,
        OUT     icf_error_t              *p_ecode);
    
/*******************************************************************************
 *
 * FUNCTION:        icf_cm_util_fill_fp_va_mms_resp
 *
 * DESCRIPTION:     This function defines the value action on forward path for 
 *                  MMS response received from MMI.
 *
 ******************************************************************************/
icf_return_t icf_cm_util_fill_fp_va_mms_resp(
        INOUT   icf_cm_pdb_st            *p_cm_pdb,
        INOUT   icf_cm_value_action_st   **p_p_value_action_root,
        OUT     icf_error_t              *p_ecode);

/*******************************************************************************
 *
 * FUNCTION:        icf_cm_util_fill_fp_va_media_err
 *
 * DESCRIPTION:     This function defines the value action on forward path for 
 *                  Media Error indication received from MMI.
 *
 ******************************************************************************/
icf_return_t icf_cm_util_fill_fp_va_media_err(
        INOUT   icf_cm_pdb_st            *p_cm_pdb,
        INOUT   icf_cm_value_action_st   **p_p_value_action_root,
        OUT     icf_error_t              *p_ecode);

/*******************************************************************************
 *
 * FUNCTION:        icf_cm_util_fill_fp_va_setup
 *
 * DESCRIPTION:     This function defines the value action on forward path for 
 *                  setup request received from SSA.
 *                  CM shall check if the replace call id is present in setup 
 *                  req. If present, this function shall invoke an action 
 *                  routine that sends call terminate to CC for old call id.
 *
 ******************************************************************************/
icf_return_t icf_cm_util_fill_fp_va_setup(
        INOUT   icf_cm_pdb_st            *p_cm_pdb,
        INOUT   icf_cm_value_action_st   **p_p_value_action_root,
        OUT     icf_error_t              *p_ecode);

/*******************************************************************************
 *
 * FUNCTION:        icf_cm_fp_setup_with_replace
 *
 * DESCRIPTION:     This function defines the value action on forward path for 
 *                  setup request received from SSA.
 *                  CM shall check if the replace call id is present in setup 
 *                  req. If present, this function does the following:
 *                  (1) Find the call ctx corresponding to the replace call id 
 *                      (called as old call ctx and the current one in glb_pdb 
 *                      is named as new call ctx).
 *                  (2) Update old call ctx's replace call id with new call id. 
 *                  (3) It shall also check if the call status of the old call 
 *                      ctx has the local call held status as true, then it 
 *                      shall update the local call held status in new call ctx.
 *                      This will only be done if IPTK handles the new call on
 *                      its own and does not wait for the APP trigger for call
 *                      connect.
 *                  (4) It shall now send PA initiated call terminate to CC 
 *                      after filling the old call ctx in glb_pdb.
 *
 ******************************************************************************/
icf_return_t icf_cm_fp_setup_with_replace(
        INOUT   icf_cm_pdb_st            *p_cm_pdb);

/*******************************************************************************
 *
 * FUNCTION:        icf_cm_util_fill_fp_va_cancel
 *
 * DESCRIPTION:     This function defines the value action on forward path for 
 *                  Cancel request received from SSA.
 *                  CM shall check if the replace call id is present in Cancel 
 *                  request. If present, this function shall invoke an action 
 *                  routine that does further processing of received message.
 *
 ******************************************************************************/
icf_return_t icf_cm_util_fill_fp_va_cancel(
        INOUT   icf_cm_pdb_st            *p_cm_pdb,
        INOUT   icf_cm_value_action_st   **p_p_value_action_root,
        OUT     icf_error_t              *p_ecode);

/*******************************************************************************
 *
 * FUNCTION:        icf_cm_fp_cancel_for_replace
 *
 * DESCRIPTION:     This function defines the value action on forward path for 
 *                  Cancel request received from SSA.
 *                  CM shall check if the replace call id is present in Cancel 
 *                  req. If present, this function does the following:
 *                  (1) Find the call ctx corresponding to the replace call id 
 *                      (old call ctx; current one in glb_pdb is new call ctx).
 *                  (2) Remove old call ctx's replace call id. 
 *
 ******************************************************************************/
icf_return_t icf_cm_fp_cancel_for_replace(
        INOUT   icf_cm_pdb_st            *p_cm_pdb);

/*******************************************************************************
 *
 * FUNCTION:        icf_cm_util_fill_sc_bp_va_media_ong_end
 *
 * DESCRIPTION:     This function defines the value action on SC backward path 
 *                  for Media Ongoing ended by SC.
 *
 ******************************************************************************/
icf_return_t icf_cm_util_fill_sc_bp_va_media_ong_end(
        INOUT   icf_cm_pdb_st            *p_cm_pdb,
        INOUT   icf_cm_value_action_st   **p_p_value_action_root,
        OUT     icf_error_t              *p_ecode);

/*******************************************************************************
 *
 * FUNCTION:        icf_cm_util_fill_sc_bp_va_sig_ong_end
 *
 * DESCRIPTION:     This function defines the value action on SC backward path 
 *                  for Signaling Ongoing ended by SC.
 *
 ******************************************************************************/
icf_return_t icf_cm_util_fill_sc_bp_va_sig_ong_end(
        INOUT   icf_cm_pdb_st            *p_cm_pdb,
        INOUT   icf_cm_value_action_st   **p_p_value_action_root,
        OUT     icf_error_t              *p_ecode);

/*******************************************************************************
 *
 * FUNCTION:        icf_cm_util_fill_sc_bp_va_service_ended_normal
 *
 * DESCRIPTION:     This function defines the value action on SC backward path 
 *                  for Service ended normal status by SC.
 *
 ******************************************************************************/
icf_return_t icf_cm_util_fill_sc_bp_va_service_ended_normal(
        INOUT   icf_cm_pdb_st            *p_cm_pdb,
        INOUT   icf_cm_value_action_st   **p_p_value_action_root,
        OUT     icf_error_t              *p_ecode);

/*******************************************************************************
 *
 * FUNCTION:        icf_cm_util_fill_sc_bp_va_service_err_ind_wo_pld
 *
 * DESCRIPTION:     This function defines the value action on SC backward path 
 *                  for Service ended error without payload.
 *                  This function sends Forceful Call Clear to CC if the event 
 *                  was Forceful Call Clear to SCM, else it sends Service Err 
 *                  Indication to CC.
 *
 ******************************************************************************/
icf_return_t icf_cm_util_fill_sc_bp_va_service_err_ind_wo_pld(
        INOUT   icf_cm_pdb_st            *p_cm_pdb,
        INOUT   icf_cm_value_action_st   **p_p_value_action_root,
        OUT     icf_error_t              *p_ecode);

/*******************************************************************************
 *
 * FUNCTION:        icf_cm_util_fill_sc_bp_va_service_err_ind_with_pld
 *
 * DESCRIPTION:     This function defines the value action on SCM backward path 
 *                  for Service error indication with payload.
 *                  This function sets Service Error event in action for CC and 
 *                  also invokes an action function that prepares the payload 
 *                  for the message to be sent to CC. In case this is the 
 *                  return of Forceful Call Clear from SCM, then Forceful Call 
 *                  Clear is sent to CC without any payload but after saving 
 *                  the second call ctx.
 *
 ******************************************************************************/
icf_return_t icf_cm_util_fill_sc_bp_va_service_err_ind_with_pld(
        INOUT   icf_cm_pdb_st            *p_cm_pdb,
        INOUT   icf_cm_value_action_st   **p_p_value_action_root,
        OUT     icf_error_t              *p_ecode);

/*******************************************************************************
 *
 * FUNCTION:        icf_cm_bp_sc_service_term_payload
 *
 * DESCRIPTION:     This function fills the payload on SCM backward path 
 *                  for Service error indication to be sent to CC (for TWC).
 *
 ******************************************************************************/
icf_return_t icf_cm_bp_sc_service_term_payload(
        INOUT   icf_cm_pdb_st            *p_cm_pdb);

#ifdef ICF_CM_SCM_GENERIC_CALL_CLR
/*******************************************************************************
 *
 * FUNCTION:        icf_cm_util_fill_sc_bp_va_generic_clr
 *
 * DESCRIPTION:     This function defines the value action on backward path for 
 *                  Generic Call Clear event for SC.
 *                  It checks the pdb bit and sends Forceful Call Clear event 
 *                  to CC if the bit is set. Set bit indicates that this 
 *                  Generic Call Clear that was sent to SCM was triggered by CC.
 *
 ******************************************************************************/
icf_return_t icf_cm_util_fill_sc_bp_va_generic_clr(
        INOUT   icf_cm_pdb_st            *p_cm_pdb,
        INOUT   icf_cm_value_action_st   **p_p_value_action_root,
        OUT     icf_error_t              *p_ecode);
#endif

/*******************************************************************************
 *
 * FUNCTION:        icf_cm_util_fill_sc_bp_va_call_rel
 *
 * DESCRIPTION:     This function defines the value action on backward path for 
 *                  CC Call Released event for SC.
 *                  It checks the pdb bit and sends Generic Call Clear event 
 *                  to CC if the bit is set. Set bit indicates that this 
 *                  Generic Call Clear that was sent to SCM was triggered by 
 *                  ecode Generic Error.
 *
 ******************************************************************************/
icf_return_t icf_cm_util_fill_sc_bp_va_call_rel(
        INOUT   icf_cm_pdb_st            *p_cm_pdb,
        INOUT   icf_cm_value_action_st   **p_p_value_action_root,
        OUT     icf_error_t              *p_ecode);

/*******************************************************************************
 *
 * FUNCTION:        icf_cm_util_fill_sc_bp_va_ecode_generic_err
 *
 * DESCRIPTION:     This function defines the value action on SC backward path 
 *                  for Ecode Generic Error.
 *                  This function sends CC Call Released to SCM and also sets a 
 *                  pdb bit indicating that Generic Call Clear needs to be sent 
 *                  to CC in the return path from SCM. If service ctx ptr is 
 *                  null, it sends Generic Call Clear to CC directly.
 *
 ******************************************************************************/
icf_return_t icf_cm_util_fill_sc_bp_va_ecode_generic_err(
        INOUT   icf_cm_pdb_st            *p_cm_pdb,
        INOUT   icf_cm_value_action_st   **p_p_value_action_root,
        OUT     icf_error_t              *p_ecode);

/*******************************************************************************
 *
 * FUNCTION:        icf_cm_util_fill_cc_bp_va_media_ong_end
 *
 * DESCRIPTION:     This function defines the value action on CC backward path 
 *                  for Media Ongoing ended by CC.
 *
 ******************************************************************************/
icf_return_t icf_cm_util_fill_cc_bp_va_media_ong_end(
        INOUT   icf_cm_pdb_st            *p_cm_pdb,
        INOUT   icf_cm_value_action_st   **p_p_value_action_root,
        OUT     icf_error_t              *p_ecode);

/*******************************************************************************
 *
 * FUNCTION:        icf_cm_util_fill_cc_bp_va_sig_ong_end
 *
 * DESCRIPTION:     This function defines the value action on CC backward path 
 *                  for Signaling Ongoing ended by CC.
 *
 ******************************************************************************/
icf_return_t icf_cm_util_fill_cc_bp_va_sig_ong_end(
        INOUT   icf_cm_pdb_st            *p_cm_pdb,
        INOUT   icf_cm_value_action_st   **p_p_value_action_root,
        OUT     icf_error_t              *p_ecode);

/*******************************************************************************
 *
 * FUNCTION:        icf_cm_util_fill_cc_bp_va_med_comtd
 *
 * DESCRIPTION:     This function defines the value action on CC backward path 
 *                  for Media Committed status by CC.
 *
 ******************************************************************************/
icf_return_t icf_cm_util_fill_cc_bp_va_med_comtd(
        INOUT   icf_cm_pdb_st            *p_cm_pdb,
        INOUT   icf_cm_value_action_st   **p_p_value_action_root,
        OUT     icf_error_t              *p_ecode);
#endif /* #ifdef ICF_SERVICE_ENABLE */

/*******************************************************************************
 *
 * FUNCTION:        icf_cm_util_fill_cc_bp_va_call_term
 *
 * DESCRIPTION:     This function defines the value action on backward path for 
 *                  Call Termination status received from CC.
 *                  This function sends Generic Call Clear to SCM and also sets 
 *                  a bit in CM pdb through an action function that would be 
 *                  needed in the backward path of Generic Call Clear from SCM.
 *
 ******************************************************************************/
icf_return_t icf_cm_util_fill_cc_bp_va_call_term(
        INOUT   icf_cm_pdb_st            *p_cm_pdb,
        INOUT   icf_cm_value_action_st   **p_p_value_action_root,
        OUT     icf_error_t              *p_ecode);

#ifdef ICF_SERVICE_ENABLE
#ifdef ICF_CM_SCM_GENERIC_CALL_CLR
/*******************************************************************************
 *
 * FUNCTION:        icf_cm_fp_cc_call_term
 *
 * DESCRIPTION:     This function defines the value action on backward path for 
 *                  Call Termination status from CC.
 *                  This function sets a bit in CM pdb that is checked in the 
 *                  return path of Generic Call Clear event from SC to determine
 *                  whether Forceful Call Clear needs to be sent to CC.
 *
 ******************************************************************************/
icf_return_t icf_cm_fp_cc_call_term(
        INOUT   icf_cm_pdb_st            *p_cm_pdb);
#endif

/*******************************************************************************
 *
 * FUNCTION:        icf_cm_util_fill_cc_bp_va_call_fwd
 *
 * DESCRIPTION:     This function defines the value action on CC backward path 
 *                  for Call Forward scenarios by CC.
 *
 ******************************************************************************/
icf_return_t icf_cm_util_fill_cc_bp_va_call_fwd(
        INOUT   icf_cm_pdb_st            *p_cm_pdb,
        IN      icf_uint32_t             status_value,
        INOUT   icf_cm_value_action_st   **p_p_value_action_root,
        OUT     icf_error_t              *p_ecode);

/*******************************************************************************
 *
 * FUNCTION:        icf_cm_fp_call_fwd_payload
 *
 * DESCRIPTION:     This function fills the payload on CC backward path 
 *                  for Call Forward scenarios to be sent to SCM.
 *
 ******************************************************************************/
icf_return_t icf_cm_fp_call_fwd_payload(
        INOUT   icf_cm_pdb_st            *p_cm_pdb);

/*******************************************************************************
 *
 * FUNCTION:        icf_cm_util_fill_cc_bp_va_invite_resp
 *
 * DESCRIPTION:     This function defines the value action on CC backward path 
 *                  for 200 OK of Reinvite received from SSA.
 *
 ******************************************************************************/
icf_return_t icf_cm_util_fill_cc_bp_va_invite_resp(
        INOUT   icf_cm_pdb_st            *p_cm_pdb,
        INOUT   icf_cm_value_action_st   **p_p_value_action_root,
        OUT     icf_error_t              *p_ecode);

/*******************************************************************************
 *
 * FUNCTION:        icf_cm_util_fill_cc_bp_va_dms_resp
 *
 * DESCRIPTION:     This function defines the value action on backward path for 
 *                  DMS response received from MMI by CC.
 *                  CM shall check if the replace call id is present in call
 *                  context of DMS response. If present, this function shall 
 *                  invoke an action routine that does further processing of 
 *                  received message and sends Initiate Call to CC for new ctx.
 *
 ******************************************************************************/
icf_return_t icf_cm_util_fill_cc_bp_va_dms_resp(
        INOUT   icf_cm_pdb_st            *p_cm_pdb,
        INOUT   icf_cm_value_action_st   **p_p_value_action_root,
        OUT     icf_error_t              *p_ecode);

/*******************************************************************************
 *
 * FUNCTION:        icf_cm_cc_bp_dms_resp_for_replace
 *
 * DESCRIPTION:     This function defines the value action on backward path for 
 *                  DMS response received from MMI by CC.
 *                  CM shall check if the replace call id is present in call 
 *                  context of the DMS response received by CC. If present, 
 *                  this function does the following:
 *                  (1) Find the call ctx corresponding to the replace call id 
 *                      (new call ctx; current one in glb_pdb is old call ctx).
 *                  (2) Replace the call ctx in glb_pdb with new call ctx so 
 *                      that Initiate Call is sent to CC with new call ctx
 *                  (3) Remove old and new call ctx's replace call id.
 *                  (4) Set a pdb flag indicating that an action needs to be 
 *                      taken on the backward path of Initiate Call being sent.
 *
 ******************************************************************************/
icf_return_t icf_cm_cc_bp_dms_resp_for_replace(
        INOUT   icf_cm_pdb_st            *p_cm_pdb);

/*******************************************************************************
 *
 * FUNCTION:        icf_cm_util_fill_cc_bp_va_initiate_call
 *
 * DESCRIPTION:     This function defines the value action on CC backward path 
 *                  for Initiate Call event.
 *
 ******************************************************************************/
icf_return_t icf_cm_util_fill_cc_bp_va_initiate_call(
        INOUT   icf_cm_pdb_st            *p_cm_pdb,
        INOUT   icf_cm_value_action_st   **p_p_value_action_root,
        OUT     icf_error_t              *p_ecode);

/*******************************************************************************
 *
 * FUNCTION:        icf_cm_util_fill_cc_bp_va_service_err_ind
 *
 * DESCRIPTION:     This function defines the value action on CC backward path 
 *                  for Service error indication with payload.
 *                  This function sets Service Error event in action for CC and 
 *                  also invokes an action function that sets call ctx pointer 
 *                  for the message to be sent to CC.
 *
 ******************************************************************************/
icf_return_t icf_cm_util_fill_cc_bp_va_service_err_ind(
        INOUT   icf_cm_pdb_st            *p_cm_pdb,
        INOUT   icf_cm_value_action_st   **p_p_value_action_root,
        OUT     icf_error_t              *p_ecode);

/*******************************************************************************
 *
 * FUNCTION:        icf_cm_bp_cc_service_term_payload
 *
 * DESCRIPTION:     This function gets the call id from the payload of Service 
 *                  Error indication on CC backward path and sets the 
 *                  corresponding call ctx pointer in global pdb to be sent to 
 *                  CC for Service Error Ind of second call (for TWC).
 *
 ******************************************************************************/
icf_return_t icf_cm_bp_cc_service_term_payload(
        INOUT   icf_cm_pdb_st            *p_cm_pdb);

/*******************************************************************************
 *
 * FUNCTION:        icf_cm_util_fill_cc_bp_va_forceful_clear
 *
 * DESCRIPTION:     This function defines the value action on CC backward path 
 *                  for Forceful Call Clear.
 *                  This function sets Forceful Call Clear in action for CC if 
 *                  the call ctx ptr in service_call_buff is not null. It 
 *                  also invokes an action function that sets call ctx pointer 
 *                  for the message to be sent to CC for second call.
 *
 ******************************************************************************/
icf_return_t icf_cm_util_fill_cc_bp_va_forceful_clear(
        INOUT   icf_cm_pdb_st            *p_cm_pdb,
        INOUT   icf_cm_value_action_st   **p_p_value_action_root,
        OUT     icf_error_t              *p_ecode);

/*******************************************************************************
 *
 * FUNCTION:        icf_cm_util_fill_cc_bp_va_inc_call_term
 *
 * DESCRIPTION:     This function defines the value action on CC backward path 
 *                  for Incoming Call Terminate Request.
 *                  This function sends Generic Call Clear to SCM if service 
 *                  context exists. This will be used on return path of CC 
 *                  after CC has sent dms_req but before CC sends 
 *                  call_term_ind/resp to app so that response to application 
 *                  for service is sent before response for call.
 *
 ******************************************************************************/
icf_return_t icf_cm_util_fill_cc_bp_va_inc_call_term(
        INOUT   icf_cm_pdb_st            *p_cm_pdb,
        INOUT   icf_cm_value_action_st   **p_p_value_action_root,
        OUT     icf_error_t              *p_ecode);

#endif /* #ifdef ICF_SERVICE_ENABLE */

/*******************************************************************************
 *
 * FUNCTION:        icf_cm_util_fill_cc_bp_va_ecode_generic_err
 *
 * DESCRIPTION:     This function defines the value action on CC backward path 
 *                  for Ecode Generic Error.
 *                  This function sends CC Call Released to SCM and also sets a 
 *                  pdb bit indicating that Generic Call Clear needs to be sent 
 *                  to CC in the return path from SCM. If service ctx ptr is 
 *                  null, it sends Generic Call Clear to CC directly.
 *
 ******************************************************************************/
icf_return_t icf_cm_util_fill_cc_bp_va_ecode_generic_err(
        INOUT   icf_cm_pdb_st            *p_cm_pdb,
        INOUT   icf_cm_value_action_st   **p_p_value_action_root,
        OUT     icf_error_t              *p_ecode);

/*******************************************************************************
 *
 * FUNCTION:        icf_cm_bp_ecode_generic_err
 *
 * DESCRIPTION:     This function defines the value action on backward path for 
 *                  Ecode Generic Error from CC.
 *                  This function sets a bit in CM pdb that is checked in the 
 *                  return path of CC Call Released event from SC to determine
 *                  whether Generic Call Clear needs to be sent to CC.
 *
 ******************************************************************************/
icf_return_t icf_cm_bp_ecode_generic_err(
        INOUT   icf_cm_pdb_st            *p_cm_pdb);

/*******************************************************************************
 *
 * FUNCTION:        icf_cm_util_delete_nodes
 *
 * DESCRIPTION:     This function recursively deletes all the new checks. It 
 *                  goes inside each new_check to see if the check is a field 
 *                  check. In that case, it further traverses the list of 
 *                  value_action(s) and deletes all action(s). In case an 
 *                  action is further new_check, this same function is called 
 *                  recursively.
 *
 ******************************************************************************/
icf_return_t  icf_cm_util_delete_nodes(
        INOUT   icf_cm_pdb_st        *p_cm_pdb,
        OUT     icf_cm_new_check_st  *p_new_check,
        OUT     icf_error_t          *p_ecode);

/*******************************************************************************
 *
 * FUNCTION:        icf_cm_util_fill_sc_bp_va_cc_handle_reinvite
 *
 * DESCRIPTION:     This function defines the value action on SCM backward path
 *                  for CC handle reinvite.
 *                  This function invokes a handler function that copies the
 *                  internal message payload sent from SSA to CHR and sends the
 *                  same payload on return path to CC.
 *
 ******************************************************************************/
icf_return_t icf_cm_util_fill_sc_bp_va_cc_handle_reinvite(
        INOUT   icf_cm_pdb_st            *p_cm_pdb,
        INOUT   icf_cm_value_action_st   **p_p_value_action_root,
        OUT     icf_error_t              *p_ecode);


/******************************************************************************
 *
 * FUNCTION:        icf_cm_util_fill_cc_bp_va_twc
 * DESCRIPTION:    This function allocates a node to the routing table of CM 
 *               to route the message on returning path of CC to SCM and 
 *                 finally to TWC 
 ***********************************************************************************/
icf_return_t icf_cm_util_fill_cc_bp_va_twc(
        INOUT   icf_cm_pdb_st            *p_cm_pdb,
        IN      icf_uint32_t             status_value,
        INOUT   icf_cm_value_action_st   **p_p_value_action_root,
        OUT     icf_error_t              *p_ecode);

/*******************************************************************************
 *
 * FUNCTION:        icf_cm_util_fill_cc_bp_va_xconf_ext_connect
 *
 * DESCRIPTION:     This function defines the value action on CC backward path 
 *                  for xconf external conference connect scenario by CC.
 *
 ******************************************************************************/
icf_return_t icf_cm_util_fill_cc_bp_va_xconf_ext_connect(
        INOUT   icf_cm_pdb_st            *p_cm_pdb,
        IN      icf_uint32_t             status_value,
        INOUT   icf_cm_value_action_st   **p_p_value_action_root,
        OUT     icf_error_t              *p_ecode);

/*******************************************************************************
 *
 * FUNCTION:        icf_cm_util_fill_cc_bp_va_xconf_inc_call
 *
 * DESCRIPTION:     This function defines the value action on CC backward path 
 *                  for xconf incoming conference call scenario by CC.
 *
 ******************************************************************************/
icf_return_t icf_cm_util_fill_cc_bp_va_xconf_inc_call(
        INOUT   icf_cm_pdb_st            *p_cm_pdb,
        IN      icf_uint32_t             status_value,
        INOUT   icf_cm_value_action_st   **p_p_value_action_root,
        OUT     icf_error_t              *p_ecode);

/*******************************************************************************
 *
 * FUNCTION:        icf_cm_util_fill_fp_va_mrm_srv_init_conf_api
 *
 * DESCRIPTION:     This function checks the received PA service req_api and
 *                  forwards internal event to ADM if call_ctx is  null,
 *                  else it sends a failure response to ADM.
 * RETURN: ICF_FAILURE or ICF_SUCCESS
 ******************************************************************************/
icf_return_t icf_cm_util_fill_fp_va_mrm_srv_init_conf_api(
        INOUT   icf_cm_pdb_st            *p_cm_pdb,
        IN      icf_uint32_t             req_api,
        IN      icf_uint32_t              event,
        INOUT   icf_cm_value_action_st   **p_p_value_action_root,
        OUT     icf_uint32_t             resp_api,
        OUT     icf_error_t              *p_ecode);

icf_return_t icf_cm_bp_sc_cc_handle_reinvite(
        INOUT   icf_cm_pdb_st            *p_cm_pdb);


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* End of ifndef __ICF_CM_PROTOTYPES_H__ */
