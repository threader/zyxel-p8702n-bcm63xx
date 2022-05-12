/*------------------------------------------------------------------------------
 *
 * File name        : icf_regm_prototypes.h
 *
 * Purpose          : This file contains the datatypes used by RGM.
 *
 * Revision History :
 *
 * Date         Name            Ref#                Comments
 * --------     ------------    ----                ---------------
 * 20-Dec-2004  Jalaj Negi      Non Service LLD     Initial
 * 26-Apr-2005  Jalaj Negi      SPR 7361            Added prototype of function
 *                                                  for starting expires timer. 
 * 05-Dec-2005	Jyoti Narula	ICF 4.0 CDD			Changes under ICF_4_0_CRM_START and END
 * 07-Dec-2005  Rohit Aggarwal  ICF Rel 4.0 CDD     Coding for IPSec support
 *
 * 29-Nov-2006  Deepti Goyal    ICF Rel 6.2         NAT "rport" feature support
 * 28-Dec-2006  Deepti Goyal    SPR 14173           Added prototype of function 
 *                                                  to update the binding timer info.
 *                                                  in rport_config data.
 * 24-Jan-2008  Tarun Gupta     IPTK Rel 8.0 LLD    Changes done for AKA related handling.                                                  
 * 26-Jan-2008  Amit Mahajan    ICF Rel 8.0         Changes for reg event package support
 *
 * 07-Mar-2008  Divij Agarwal   SPR 18122           Support for async AKA negotiation.
 * 04-Nov-2008  Tarun Gupta     SPR 19189           Merged Rel 7.1.2 SPR 18880
 * 20-Nov-2008  Rajiv Kumar     SPR 19188           Rel 7.1.2 SPR merging (SPR
 *                                                  19151) 
 * 01-Apr-2009  Anuradha Gupta   Rel 8.2            New function prototype added    
 * 01-Apr-2009  Alok Tiwari      Rel 8.2            New function prototype added:
 *                                         icf_rgm_process_security_association
 *
 * Copyright (c) 2009, Aricent.
 *----------------------------------------------------------------------------*/

#ifndef __ICF_REGM_PROTOTYPES_H__
#define __ICF_REGM_PROTOTYPES_H__

#include "icf_regm_types.h"
#include "icf_cfg_prototypes.h"

#if defined ICF_NW_ACTIVATE || defined IMS_CLIENT
ICF_CRM_START

#include "icf_nm_prototypes.h"

ICF_CRM_END
#endif



/* Determine if a C++ compiler is being used.
 * If so, ensure that standard
 * C is used to process the API information.
 */

#ifdef __cplusplus

/* Yes, C++ compiler is present.
 * Use standard C.
 */

extern "C"
{
#endif /* __cplusplus */


/* -------------------------------------------------------------------------- */
/* **** RGM Init Time Functions **** */
/* -------------------------------------------------------------------------- */

/****************************************************************************
 * FUNCTION:            icf_rgm_initialize_fsm
 *
 * DESCRIPTION:         This function is called by icf_rgm_init to 
 *                      initialize the Registration FSM.
 *
 *
 * RETURNS:             
 *
 ***************************************************************************/
icf_return_t icf_rgm_initialize_fsm(
        OUT icf_rgm_glb_data_st *p_rgm_pdb);



/* -------------------------------------------------------------------------- */
/* **** FSM handlers **** */
/* -------------------------------------------------------------------------- */

/****************************************************************************
 * FUNCTION:            icf_rgm_ignore
 *
 * DESCRIPTION:         This function is invoked in case when an unexpected 
 *                      event occurs in a state.
 *
 *
 * RETURNS:             
 *
 ***************************************************************************/
icf_return_t icf_rgm_ignore(
        INOUT icf_crm_pdb_st *p_crm_pdb);


/****************************************************************************
 * FUNCTION:            icf_rgm_01_04_success_resp
 *
 * DESCRIPTION:         This function is invoked when a success response is
 *                      received in REGISTERING or AUTHORIZING state.
 *
 *
 * RETURNS:             
 *
 ***************************************************************************/
icf_return_t icf_rgm_01_04_success_resp(
        INOUT icf_crm_pdb_st *p_crm_pdb);


/****************************************************************************
 * FUNCTION:            icf_rgm_05_any_nw_resp
 *
 * DESCRIPTION:         This function is invoked when any type of response is
 *                      received in DEREGISTERING state.
 *
 *
 * RETURNS:             
 *
 ***************************************************************************/
icf_return_t icf_rgm_05_any_nw_resp(
        INOUT icf_crm_pdb_st *p_crm_pdb);


/****************************************************************************
 * FUNCTION:            icf_rgm_01_04_failure_resp
 *
 * DESCRIPTION:         This function is invoked when a failure response with 
 *                      reattempt received in REGISTERING or AUTHORIZING state.
 *
 *
 * RETURNS:             
 *
 ***************************************************************************/
icf_return_t icf_rgm_01_04_failure_resp(
        INOUT icf_crm_pdb_st *p_crm_pdb);


/****************************************************************************
 * FUNCTION:            icf_rgm_01_04_fail_no_resp
 *
 * DESCRIPTION:         This function is invoked when a failure response with no
 *                      reattempt received in REGISTERING or AUTHORIZING state.
 *
 *
 * RETURNS:             
 *
 ***************************************************************************/
icf_return_t icf_rgm_01_04_fail_no_resp(
        INOUT icf_crm_pdb_st *p_crm_pdb);



/****************************************************************************
 * FUNCTION:            icf_rgm_01_auth_req_resp
 *
 * DESCRIPTION:         This function is invoked when Authentication required
 *                      response is received in REGISTERING state.
 *
 *
 * RETURNS:             
 *
 ***************************************************************************/
icf_return_t icf_rgm_01_auth_req_resp(
        INOUT icf_crm_pdb_st *p_crm_pdb);


/****************************************************************************
 * FUNCTION:            icf_rgm_01_04_dur_short_resp
 *
 * DESCRIPTION:         This function is invoked when Registration Duration
 *                      Brief response is received in REGISTERING or 
 *                      Authorizing state.
 *
 *
 * RETURNS:             
 *
 ***************************************************************************/
icf_return_t icf_rgm_01_04_dur_short_resp(
        INOUT icf_crm_pdb_st *p_crm_pdb);

/****************************************************************************
 * FUNCTION:            icf_rgm_04_auth_req_resp
 *
 * DESCRIPTION:         This function is invoked when Authentication required
 *                      response is received in AUTHORIZING state.
 *
 *
 * RETURNS:             
 *
 ***************************************************************************/
icf_return_t icf_rgm_04_auth_req_resp(
        INOUT icf_crm_pdb_st *p_crm_pdb);

/****************************************************************************
 * FUNCTION:            icf_rgm_02_duration_change
 *
 * DESCRIPTION:         This function is invoked when Registration Duration
 *                      changes in UNREGISTERED state.
 *
 *
 * RETURNS:             
 *
 ***************************************************************************/
icf_return_t icf_rgm_02_duration_change(
        INOUT icf_crm_pdb_st *p_crm_pdb);


/****************************************************************************
 * FUNCTION:            icf_rgm_03_duration_change
 *
 * DESCRIPTION:         This function is invoked when Registration Duration
 *                      changes in REGISTERED state.
 *
 *
 * RETURNS:             
 *
 ***************************************************************************/
icf_return_t icf_rgm_03_duration_change(
        INOUT icf_crm_pdb_st *p_crm_pdb);


/****************************************************************************
 * FUNCTION:            icf_rgm_01_04_delete_user
 *
 * DESCRIPTION:         This function is invoked when Delete User is received
 *                      in REGISTERING or AUTHORIZING state.
 *
 *
 * RETURNS:             
 *
 ***************************************************************************/
icf_return_t icf_rgm_01_04_delete_user(
        INOUT icf_crm_pdb_st *p_crm_pdb);


/****************************************************************************
 * FUNCTION:            icf_rgm_02_delete_user
 *
 * DESCRIPTION:         This function is invoked when Delete User is received
 *                      in UNREGISTERED state.
 *
 *
 * RETURNS:             
 *
 ***************************************************************************/
icf_return_t icf_rgm_02_delete_user(
        INOUT icf_crm_pdb_st *p_crm_pdb);


/****************************************************************************
 * FUNCTION:            icf_rgm_03_delete_user
 *
 * DESCRIPTION:         This function is invoked when Delete User is received
 *                      in REGISTERED state.
 *
 *
 * RETURNS:             
 *
 ***************************************************************************/
icf_return_t icf_rgm_03_delete_user(
        INOUT icf_crm_pdb_st *p_crm_pdb);


/****************************************************************************
 * FUNCTION:            icf_rgm_03_auth_info_change
 *
 * DESCRIPTION:         This function is invoked when Authentication 
 *                      information changes in REGISTERED state.
 *
 *
 * RETURNS:             
 *
 ***************************************************************************/
icf_return_t icf_rgm_03_auth_info_change(
        INOUT icf_crm_pdb_st *p_crm_pdb);

/****************************************************************************
 * FUNCTION:	icf_rgm_06_user_auth_resp
 * Description: This function is invoked when Authentication response is 
 *              received from application. 
 *      Current State: ICF_RGM_06_AWT_AUTH_RESP 
 *      Event Received: ICF_RGM_AUTH_RESP 
 *
 ***************************************************************************/
icf_return_t icf_rgm_06_user_auth_resp(
        INOUT icf_crm_pdb_st *p_crm_pdb); 

/****************************************************************************
 * FUNCTION:	icf_rgm_06_guard_timer_exp
 * Description: This function is invoked when Guard timer at IPTK has expired.
 *      Current State: ICF_RGM_06_AWT_AUTH_RESP 
 *      Event Received: ICF_RGM_TIMER_EXPIRY 
 *
 ***************************************************************************/
icf_return_t icf_rgm_06_guard_timer_exp(
        INOUT icf_crm_pdb_st *p_crm_pdb);


/****************************************************************************
 * FUNCTION:	icf_rgm_06_notify_terminated
 * Description: This function is invoked when ICF_RGM_NOTIFY_TERMINATED event
 *              is received in the await authentication response state.
 *      Current State: ICF_RGM_06_AWT_AUTH_RESP 
 *      Event Received: ICF_RGM_NOTIFY_TERMINATED 
 *
 ***************************************************************************/
icf_return_t icf_rgm_06_notify_terminated(
        INOUT icf_crm_pdb_st *p_crm_pdb); 


/****************************************************************************
 * FUNCTION:	icf_rgm_06_reg_req_recvd
 * Description: This function is invoked when REGISTER request API is received
 *              from application by IPTK in Await Auth Response state.
 *      Current State: ICF_RGM_06_AWT_AUTH_RESP 
 *      Event Received: ICF_RGM_REG_REQ_RECVD 
 *
 ***************************************************************************/
icf_return_t icf_rgm_06_reg_req_recvd(
        INOUT icf_crm_pdb_st *p_crm_pdb);

ICF_4_0_CRM_START

/****************************************************************************
 * FUNCTION:            icf_rgm_00_reg_req_recvd
 *
 * DESCRIPTION:         This function is invoked REGISTER_REQUEST
 *                      API is received in IDLE state.
 *
 *
 * RETURNS:             
 *
 ***************************************************************************/
icf_return_t icf_rgm_00_reg_req_recvd(
        INOUT icf_crm_pdb_st *p_crm_pdb);

/****************************************************************************
 * FUNCTION:            icf_rgm_01_reg_req_recvd
 *
 * DESCRIPTION:         This function is invoked REGISTER_REQUEST
 *                      API is received in REGISTERING state.
 *
 *
 * RETURNS:             
 *
 ***************************************************************************/
icf_return_t icf_rgm_01_reg_req_recvd(
        INOUT icf_crm_pdb_st *p_crm_pdb);

/****************************************************************************
 * FUNCTION:            icf_rgm_02_reg_req_recvd
 *
 * DESCRIPTION:         This function is invoked REGISTER_REQUEST
 *                      API is received in UNREGISTERED state.
 *
 *
 * RETURNS:             
 *
 ***************************************************************************/
icf_return_t icf_rgm_02_reg_req_recvd(
        INOUT icf_crm_pdb_st *p_crm_pdb);

/****************************************************************************
 * FUNCTION:            icf_rgm_03_reg_req_recvd
 *
 * DESCRIPTION:         This function is invoked REGISTER_REQUEST
 *                      API is received in REGISTERED state.
 *
 *
 * RETURNS:             
 *
 ***************************************************************************/
icf_return_t icf_rgm_03_reg_req_recvd(
        INOUT icf_crm_pdb_st *p_crm_pdb);

/****************************************************************************
 * FUNCTION:            icf_rgm_04_reg_req_recvd
 *
 * DESCRIPTION:         This function is invoked REGISTER_REQUEST
 *                      API is received in AUTHORIZING state.
 *
 *
 * RETURNS:             
 *
 ***************************************************************************/
icf_return_t icf_rgm_04_reg_req_recvd(
        INOUT icf_crm_pdb_st *p_crm_pdb);

/****************************************************************************
 * FUNCTION:            icf_rgm_05_reg_req_recvd
 *
 * DESCRIPTION:         This function is invoked REGISTER_REQUEST
 *                      API is received in DEREGISTETRING state.
 *
 *
 * RETURNS:             
 *
 ***************************************************************************/
icf_return_t icf_rgm_05_reg_req_recvd(
        INOUT icf_crm_pdb_st *p_crm_pdb);


/****************************************************************************
 * FUNCTION:            icf_rgm_01_04_timeout
 *
 * DESCRIPTION:         This function is invoked when ICF_RGM_FAILURE_TIMEOUT
 *                      event from SSA  is received in 
 *						REGISTETRING or AUTHORIZING state.
 *
 * RETURNS:             
 *
 ***************************************************************************/
icf_return_t icf_rgm_01_04_timeout(
        INOUT icf_crm_pdb_st *p_crm_pdb);

/****************************************************************************
 * FUNCTION:            icf_rgm_05_timeout
 *
 * DESCRIPTION:         This function is invoked when ICF_RGM_FAILURE_TIMEOUT
 *                      event from SSA  is received in 
 *						DEREGISTETRING state.
 *
 * RETURNS:             
 *
 ***************************************************************************/
icf_return_t icf_rgm_05_timeout(
        INOUT icf_crm_pdb_st *p_crm_pdb);


/****************************************************************************
 * FUNCTION:            icf_rgm_01_03_04_05_nw_deactivated
 *
 * DESCRIPTION:         This function is invoked when ICF_RGM_NW_DEACTIVATED
 *                      event from SSA  is received in 
 *						DEREGISTERING/Authorizing/ Registering  state.
 *
 * RETURNS:             
 *
 ***************************************************************************/
icf_return_t icf_rgm_01_03_04_05_nw_deactivated(
        INOUT icf_crm_pdb_st *p_crm_pdb);

/****************************************************************************
 * FUNCTION:            icf_rgm_02_nw_activated
 *
 * DESCRIPTION:         This function is invoked when ICF_RGM_NW_ACTIVATED
 *                      event from SSA  is received in 
 *						UnRegistered  state.
 *
 * RETURNS:             
 *
 ***************************************************************************/
icf_return_t icf_rgm_02_nw_activated(
        INOUT icf_crm_pdb_st *p_crm_pdb);


/****************************************************************************
 * FUNCTION:            icf_rgm_03_notify_shortened
 *
 * DESCRIPTION:         This function is invoked when ICF_RGM_NOTIFY_SHORTENED
 *                      event is received in Registered  state.
 *
 * RETURNS:             
 *
 ***************************************************************************/
icf_return_t icf_rgm_03_notify_shortened(
        INOUT icf_crm_pdb_st *p_crm_pdb);

/****************************************************************************
 * FUNCTION:            icf_rgm_03_notify_terminated
 *
 * DESCRIPTION:         This function is invoked when ICF_RGM_NOTIFY_TERMINATED
 *                      event is received in Registered  state.
 *
 * RETURNS:             
 *
 ***************************************************************************/
icf_return_t icf_rgm_03_notify_terminated(
        INOUT icf_crm_pdb_st *p_crm_pdb);

/****************************************************************************
 * FUNCTION:            icf_rgm_nfy_con_probation
 *
 * DESCRIPTION:         This function is invoked when valid
 *                      contact with state=terminated and
 *                      event=probation is received in REGISTERED state.
 *
 * RETURNS:             
 *
 ***************************************************************************/
icf_return_t icf_rgm_nfy_con_probation(
        INOUT icf_crm_pdb_st *p_crm_pdb);


/****************************************************************************
 * FUNCTION:            icf_rgm_nfy_con_expired
 *
 * DESCRIPTION:         This function is invoked when valid
 *                      contact with state=terminated and
 *                      event=expired is received in REGISTERED state.
 *
 * RETURNS:             
 *
 ***************************************************************************/
icf_return_t icf_rgm_nfy_con_expired(
        INOUT icf_crm_pdb_st *p_crm_pdb);

/****************************************************************************
 * FUNCTION:            icf_rgm_nfy_con_deactivated
 *
 * DESCRIPTION:         This function is invoked when valid
 *                      contact with state=terminated and
 *                      event=deactivated is received in REGISTERED state.
 *
 * RETURNS:             
 *
 ***************************************************************************/
icf_return_t icf_rgm_nfy_con_deactivated(
        INOUT icf_crm_pdb_st *p_crm_pdb);

/****************************************************************************
 * FUNCTION:            icf_rgm_nfy_con_rejected
 *
 * DESCRIPTION:         This function is invoked when valid
 *                      contact with state=terminated and
 *                      event=rejected is received in REGISTERED state.
 *
 * RETURNS:             
 *
 ***************************************************************************/
icf_return_t icf_rgm_nfy_con_rejected(
        INOUT icf_crm_pdb_st *p_crm_pdb);

/****************************************************************************
 * FUNCTION:            icf_rgm_nfy_con_unregistered
 *
 * DESCRIPTION:         This function is invoked when valid
 *                      contact with state=terminated and
 *                      event=unregistered is received in REGISTERED state.
 *
 * RETURNS:             
 *
 ***************************************************************************/
icf_return_t icf_rgm_nfy_con_unregistered(
        INOUT icf_crm_pdb_st *p_crm_pdb);


ICF_4_0_CRM_END

/* -------------------------------------------------------------------------- */
/* **** Handlers  for Basic CFG function calls**** */
/* -------------------------------------------------------------------------- */

/****************************************************************************
 * FUNCTION:            icf_rgm_init_complete
 *
 * DESCRIPTION:         This function is called by the CFG module when
 *                      INIT_COMPLETE is received. The event is thus
 *		                INIT_COMPLETE.
 *
 *
 * RETURNS:             
 *
 ***************************************************************************/

icf_return_t icf_rgm_init_complete(
        INOUT icf_crm_pdb_st *p_crm_pdb);


/****************************************************************************
 * FUNCTION:            icf_rgm_register_req_hdlr
 *
 * DESCRIPTION:         This function is called by the CFG module when
 *                      REGISTER_REQUEST is received.
 *
 *
 * RETURNS:             
 *
 ***************************************************************************/
icf_return_t icf_rgm_register_req_hdlr(
        INOUT icf_crm_pdb_st *p_crm_pdb);

ICF_CRM_END

ICF_4_0_CRM_START

/****************************************************************************
 * FUNCTION:            icf_rgm_nw_indicated_dereg
 *
 * DESCRIPTION:         This function is called by the functions of CFG
 *                      module in order to deregister applications of the list.
 *						These applications belong to same network which has
 *						deactivated. This does not clear the rgm-context
 *						because when the network comes up, ICF should re-register
 *						the applications without a trigger from applications.
 *
 * RETURNS:             FAILURE: if all applications are not found in single
 *								 rgm_context
 *						SUCCESS:
 *
 ***************************************************************************/
icf_return_t icf_rgm_nw_indicated_dereg(
        INOUT	icf_crm_pdb_st		*p_crm_pdb,
		INOUT	icf_app_id_list_st	*p_app_list);

/****************************************************************************
 * FUNCTION:            icf_rgm_recover_registration
 *
 * DESCRIPTION:         This function is called by the functions of CFG
 *                      module in order to reregister applications of the list.
 *						These applications belong to same network which was
 *						activated. The rgm_context will be unregistered with
 *						NW-DEACTIVATED set in indicator.
 *						When the network comes up, ICF should re-register
 *						the applications without a trigger from applications.
 *
 * RETURNS:             FAILURE: if all applications are not found in single
 *								 rgm_context
 *						SUCCESS:
 *
 ***************************************************************************/
icf_return_t icf_rgm_recover_registration(
        INOUT	icf_crm_pdb_st		*p_crm_pdb,
		INOUT	icf_app_id_list_st	*p_app_list);




/****************************************************************************
 * FUNCTION:            icf_rgm_modify_user
 *
 * DESCRIPTION:         This function is called by the CFG module when
 *                      MODIFY_USER is received and user address is to be
 *                      ADDED or DELETED on a line or on all lines.
 *
 *
 * RETURNS:             
 *
 ***************************************************************************/
icf_return_t icf_rgm_modify_user(
        INOUT icf_crm_pdb_st *p_crm_pdb);

/****************************************************************************
 * FUNCTION:            icf_rgm_modify_user_not_all_lines
 *
 * DESCRIPTION:         This function is called by the function
 *                      icf_rgm_modify_user when user modification is not
 *                      needed on all the lines.
 *
 *
 * RETURNS:             
 *
 ***************************************************************************/
icf_return_t icf_rgm_modify_user_not_all_lines(
        INOUT icf_crm_pdb_st *p_crm_pdb);

/****************************************************************************
 * FUNCTION:            icf_rgm_modify_user_all_lines
 *
 * DESCRIPTION:         This function is called by the function
 *                      icf_rgm_modify_user when user modification is
 *                      needed on all the lines.
 *
 *
 * RETURNS:             
 *
 ***************************************************************************/
icf_return_t icf_rgm_modify_user_all_lines(
        INOUT icf_crm_pdb_st *p_crm_pdb,
        IN    icf_line_id_t max_lines);

/****************************************************************************
 * FUNCTION:            icf_rgm_add_user
 *
 * DESCRIPTION:         This function is called by the functions of RGM module
 *                      to create an entry of a user on a particular line in
 *                      the registration context.
 *
 *
 * RETURNS:             
 *
 ***************************************************************************/
icf_return_t icf_rgm_add_user(
        INOUT icf_crm_pdb_st *p_crm_pdb,
        IN    icf_line_id_t line_id,
        IN    icf_address_st *p_user_address,
        OUT   icf_uint8_t *p_status,
        OUT   icf_rgm_context_st **p_p_rgm_context);

/****************************************************************************
 * FUNCTION:            icf_rgm_delete_user
 *
 * DESCRIPTION:         This function is called by the functions of RGM module
 *                      to delete the entry of a user on a particular line
 *                      from the registration context. 
 *
 *
 * RETURNS:             
 *
 ***************************************************************************/
icf_return_t icf_rgm_delete_user(
        INOUT icf_crm_pdb_st *p_crm_pdb,
        IN    icf_line_id_t line_id,
        IN    icf_address_st *p_user_address,
        OUT   icf_uint8_t *p_status,
        OUT   icf_rgm_context_st **p_p_rgm_context);

/****************************************************************************
 * FUNCTION:            icf_rgm_modify_user_roll_not_all_lines
 *
 * DESCRIPTION:         This function is called by the functions of RGM module
 *                      to carry out rollback processing in case set self 
 *                      identifier API in which all_lines field is set to 
 *                      FALSE.
 *
 *
 * RETURNS:             
 *
 ***************************************************************************/
icf_return_t icf_rgm_modify_user_roll_not_all_lines(
        INOUT icf_crm_pdb_st *p_crm_pdb,
        INOUT icf_rgm_modify_user_not_all_lines_list_st 
        *p_modify_user_context);

/****************************************************************************
 * FUNCTION:            icf_rgm_modify_user_roll_all_lines
 *
 * DESCRIPTION:         This function is called by the functions of RGM module
 *                      to carry out rollback processing in case set self 
 *                      identifier API in which all_lines field is set to 
 *                      TRUE.
 *
 *
 * RETURNS:             
 *
 ***************************************************************************/
icf_return_t icf_rgm_modify_user_roll_all_lines(
        INOUT icf_crm_pdb_st *p_crm_pdb,
        INOUT icf_rgm_modify_user_all_lines_list_st 
        *p_modify_user_context);

/****************************************************************************
 * FUNCTION:            icf_rgm_process_modify_user_block
 *
 * DESCRIPTION:         This function is called by the functions of RGM module
 *                      after all blocks have been prepared to process the
 *                      blocks according to their states.
 *
 *
 * RETURNS:             
 *
 ***************************************************************************/
icf_return_t icf_rgm_process_modify_user_block(
        INOUT icf_crm_pdb_st *p_crm_pdb,
        INOUT icf_rgm_modify_user_st *p_block);

/****************************************************************************
 * FUNCTION:            icf_rgm_modify_registrar_addr
 *
 * DESCRIPTION:         This function is called by the CFG module when it 
 *                      gets API for change of Registrar address.
 *
 *
 *                        
 * RETURNS:             
 *
 ***************************************************************************/
icf_return_t icf_rgm_modify_registrar_addr(
        INOUT icf_crm_pdb_st *p_crm_pdb);

/****************************************************************************
 * FUNCTION:            icf_rgm_modify_reg_not_all_lines
 *
 * DESCRIPTION:         This function is called by the function
 *                      icf_rgm_modify_registrar_addr when registrar
 *                      modification is not needed on all the lines.
 *
 *
 * RETURNS:             
 *
 ***************************************************************************/
icf_return_t icf_rgm_modify_reg_not_all_lines(
        INOUT icf_crm_pdb_st *p_crm_pdb);

/****************************************************************************
 * FUNCTION:            icf_rgm_modify_reg_all_lines
 *
 * DESCRIPTION:         This function is called by the function
 *                      icf_rgm_modify_registrar_addr when registrar
 *                      modification is needed on all the lines.
 *
 *
 * RETURNS:             
 *
 ***************************************************************************/
icf_return_t icf_rgm_modify_reg_all_lines(
        INOUT icf_crm_pdb_st *p_crm_pdb,
        IN    icf_line_id_t max_lines);

/****************************************************************************
 * FUNCTION:            icf_rgm_add_all_users
 *
 * DESCRIPTION:         This function is called by the functions of RGM
 *                      module in order to add all users configured on a
 *                      particular line to registration blocks.
 *
 * RETURNS:             
 *
 ***************************************************************************/
icf_return_t icf_rgm_add_all_users(
        INOUT icf_crm_pdb_st *p_crm_pdb,
        IN    icf_line_data_st *p_dbm_line_data,
        INOUT icf_rgm_set_reg_params_block_st *p_block);


/****************************************************************************
 * FUNCTION:            icf_rgm_delete_all_users
 *
 * DESCRIPTION:         This function is called by the functions of RGM
 *                      module in order to delete all users configured on a
 *                      particular line to registration blocks.
 *
 * RETURNS:             
 *
 ***************************************************************************/
icf_return_t icf_rgm_delete_all_users(
        INOUT icf_crm_pdb_st *p_crm_pdb,
        IN    icf_line_data_st *p_dbm_line_data,
        INOUT icf_rgm_set_reg_params_block_st *p_block);

/****************************************************************************
 * FUNCTION:            icf_rgm_modify_reg_roll_not_all_lines
 *
 * DESCRIPTION:         This function is called by the functions of RGM module
 *                      to carry out rollback processing in case set  
 *                      registration params API in which all_lines field is set
 *                      to FALSE.
 *
 *
 * RETURNS:             
 *
 ***************************************************************************/
icf_return_t icf_rgm_modify_reg_roll_not_all_lines(
        INOUT icf_crm_pdb_st *p_crm_pdb,
        INOUT icf_rgm_set_reg_params_not_all_lines_list_st 
        *p_modify_registrar_context);

/****************************************************************************
 * FUNCTION:            icf_rgm_modify_reg_roll_all_lines
 *
 * DESCRIPTION:         This function is called by the functions of RGM module
 *                      to carry out rollback processing in case set  
 *                      registration params API in which all_lines field is set
 *                      to TRUE.
 *
 *
 * RETURNS:             
 *
 ***************************************************************************/
icf_return_t icf_rgm_modify_reg_roll_all_lines(
        INOUT icf_crm_pdb_st *p_crm_pdb,
        INOUT icf_rgm_set_reg_params_all_lines_list_st 
        *p_modify_registrar_context);


/****************************************************************************
 * FUNCTION:            icf_rgm_process_modify_registrar_node
 *
 * DESCRIPTION:         This function is called by the functions of RGM module
 *                      after all blocks have been prepared to process a
 *                      node in a block according to its state.
 *
 *
 * RETURNS:             
 *
 ***************************************************************************/
icf_return_t icf_rgm_process_modify_registrar_node(
        INOUT icf_crm_pdb_st *p_crm_pdb,
        INOUT icf_rgm_set_reg_params_node_st *p_node);

/****************************************************************************
 * FUNCTION:            icf_rgm_modify_duration
 *
 * DESCRIPTION:         This function is called by the CFG module when it 
 *                      gets API for modification of Registrar Duration.
 *
 *
 *                        
 * RETURNS:             
 *
 ***************************************************************************/
icf_return_t icf_rgm_modify_duration(
        INOUT icf_crm_pdb_st *p_crm_pdb);

/****************************************************************************
 * FUNCTION:            icf_rgm_modify_dur_not_all_lines
 *
 * DESCRIPTION:         This function is called by the function
 *                      icf_rgm_modify_duration when registration duration
 *                      modification is not needed on all the lines.
 *
 *
 * RETURNS:             
 *
 ***************************************************************************/
icf_return_t icf_rgm_modify_dur_not_all_lines(
        INOUT icf_crm_pdb_st *p_crm_pdb);

/****************************************************************************
 * FUNCTION:            icf_rgm_modify_dur_all_lines
 *
 * DESCRIPTION:         This function is called by the function
 *                      icf_rgm_modify_duration when registration duration
 *                      modification is needed on all the lines.
 *
 *
 * RETURNS:             
 *
 ***************************************************************************/
icf_return_t icf_rgm_modify_dur_all_lines(
        INOUT icf_crm_pdb_st *p_crm_pdb,
        IN    icf_line_id_t max_lines);

/****************************************************************************
 * FUNCTION:            icf_rgm_dur_change_all_users
 *
 * DESCRIPTION:         This function is called by the functions of RGM
 *                      module in order to change duration of all users
 *                      configured on a particular line in registration blocks.
 *
 * RETURNS:             
 *
 ***************************************************************************/
icf_return_t icf_rgm_dur_change_all_users(
        INOUT icf_crm_pdb_st *p_crm_pdb,
        IN    icf_duration_t new_duration,
        INOUT icf_line_data_st *p_dbm_line_data,
        INOUT icf_rgm_set_reg_params_block_st *p_block);

/****************************************************************************
 * FUNCTION:            icf_rgm_dur_change_user
 *
 * DESCRIPTION:         This function is called by the functions of RGM
 *                      module in order to change duration of a user
 *                      configured on a particular line in registration blocks.
 *
 * RETURNS:             
 *
 ***************************************************************************/
icf_return_t icf_rgm_dur_change_user(
        INOUT icf_crm_pdb_st *p_crm_pdb,
        IN    icf_duration_t new_duration,
        IN    icf_address_st *p_user_address,
        INOUT icf_line_data_st *p_dbm_line_data,
        OUT   icf_uint8_t *p_status,
        OUT   icf_rgm_context_st **p_p_rgm_context);

/****************************************************************************
 * FUNCTION:            icf_rgm_modify_dur_roll_not_all_lines
 *
 * DESCRIPTION:         This function is called by the functions of RGM module
 *                      to carry out rollback processing in case set  
 *                      registration params API contains duration change.
 *
 *
 * RETURNS:             
 *
 ***************************************************************************/
icf_return_t icf_rgm_modify_dur_roll_not_all_lines(
        INOUT icf_crm_pdb_st *p_crm_pdb,
        INOUT icf_rgm_set_reg_params_not_all_lines_list_st 
        *p_modify_duration_context);

/****************************************************************************
 * FUNCTION:            icf_rgm_modify_dur_roll_all_lines
 *
 * DESCRIPTION:         This function is called by the functions of RGM module
 *                      to carry out rollback processing in case set  
 *                      registration params API contains duration change.
 *
 *
 * RETURNS:             
 *
 ***************************************************************************/
icf_return_t icf_rgm_modify_dur_roll_all_lines(
        INOUT icf_crm_pdb_st *p_crm_pdb,
        INOUT icf_rgm_set_reg_params_all_lines_list_st 
        *p_modify_duration_context);

/****************************************************************************
 * FUNCTION:            icf_rgm_save_smallest_dur
 *
 * DESCRIPTION:         This function is called by the functions of RGM module
 *                      in order to save the smallest duration in the 
 *                      registration block.
 *
 *
 * RETURNS:             
 *
 ***************************************************************************/
icf_return_t icf_rgm_save_smallest_dur(
        INOUT icf_glb_pdb_st *p_glb_pdb,
        INOUT icf_rgm_context_st *p_rgm_context,
        OUT   icf_error_t *p_ecode);


/****************************************************************************
 * FUNCTION:            icf_rgm_line_reg_configured
 *
 * DESCRIPTION:         This function is called by the functions of RGM module
 *                      to fetch the pointer of line block on which
 *                      registrar is configured.
 *
 *
 * RETURNS:             
 *
 ***************************************************************************/
icf_return_t icf_rgm_line_reg_configured(
        INOUT icf_glb_pdb_st *p_glb_pdb,
        IN    icf_line_id_t line_id,
        OUT   icf_line_data_st **p_p_dbm_line_data,
        OUT   icf_error_t *p_ecode);

/****************************************************************************
 * FUNCTION:            icf_rgm_line_self_configured
 *
 * DESCRIPTION:         This function is called by the functions of RGM module
 *                      to fetch the pointer of line block on which 
 *                      any user is configured.
 *
 *
 * RETURNS:             
 *
 ***************************************************************************/
icf_return_t icf_rgm_line_self_configured(
        INOUT icf_glb_pdb_st *p_glb_pdb,
        IN    icf_line_id_t line_id,
        OUT   icf_line_data_st **p_p_dbm_line_data,
        OUT   icf_error_t *p_ecode);

/****************************************************************************
 * FUNCTION:            icf_rgm_line_self_reg_config
 *
 * DESCRIPTION:         This function is called by the functions of RGM module
 *                      to fetch the pointer of line block on which Self Address
 *                      and registrar are configured.
 *
 *
 * RETURNS:             
 *
 ***************************************************************************/
icf_return_t icf_rgm_line_self_reg_config(
        INOUT icf_glb_pdb_st *p_glb_pdb,
        IN    icf_line_id_t line_id,
        OUT   icf_line_data_st **p_p_dbm_line_data,
        OUT   icf_error_t *p_ecode);


/****************************************************************************
 * FUNCTION:            icf_rgm_forceful_dereg_all_users
 *
 * DESCRIPTION:         This function is called by the functions of CFG
 *                      module in order to forcefully deregister all users.
 *
 * RETURNS:             
 *
 ***************************************************************************/
icf_return_t icf_rgm_forceful_dereg_all_users(
        INOUT icf_crm_pdb_st *p_crm_pdb);

/****************************************************************************
 * FUNCTION:            icf_rgm_auth_info_present
 *
 * DESCRIPTION:         This function is called by icf_rgm_01_auth_req_resp
 *                      function of FSM to find whether all authentication
 *                      information for this block is present on some line
 *                      and to place that line at a proper place in the 
 *                      registration block. Line having all Authentication 
 *                      information has to be placed at the end of 
 *                      the registration block because SSA will pick the last
 *                      line in case Authentication is required. 
 *
 *
 * RETURNS:             
 *
 ***************************************************************************/
icf_return_t icf_rgm_auth_info_present(
        INOUT icf_crm_pdb_st *p_crm_pdb);

/****************************************************************************
 * FUNCTION:            icf_rgm_process_nw_response
 *
 * DESCRIPTION:         This function is called configuration module when
 *                      it receives response from network for a registration
 *                      request.CFG module passes response structure to this
 *                      function. After that this function carries out the
 *                      necessary processing.
 *
 * RETURNS:             
 *
 ***************************************************************************/
icf_return_t icf_rgm_process_nw_response(
        INOUT icf_crm_pdb_st* p_crm_pdb);



/* -------------------------------------------------------------------------- */
/* **** Handlers  for Timer Expiry**** */
/* -------------------------------------------------------------------------- */

/****************************************************************************
 * FUNCTION:            icf_rgm_timer_exp
 *
 * DESCRIPTION:         This function is called by the function of configuration
 *		                part when TIMER_EXPIRY event is received from MRM.It 
 *                      checks the type of timer expired and then invokes the
 *                      respective handler.
 *
 *
 * RETURNS:             
 *
 ***************************************************************************/
icf_return_t icf_rgm_timer_exp(
        INOUT icf_crm_pdb_st *p_crm_pdb);


/****************************************************************************
 * FUNCTION:            icf_rgm_retry_timer_exp
 *
 * DESCRIPTION:         This function is invoked when retry timer expires.
 *
 *
 * RETURNS:             
 *
 ***************************************************************************/
icf_return_t icf_rgm_retry_timer_exp(
        INOUT icf_crm_pdb_st *p_crm_pdb);


/****************************************************************************
 * FUNCTION:            icf_rgm_reg_timer_exp
 *
 * DESCRIPTION:         This function is invoked on expiry of registration timer
 *
 *
 * RETURNS:             
 *
 ***************************************************************************/
icf_return_t icf_rgm_reg_timer_exp(
        INOUT icf_crm_pdb_st *p_crm_pdb);


/****************************************************************************
 * FUNCTION:            icf_rgm_reattempt_timer_exp
 *
 * DESCRIPTION:         This function is invoked on expiry of registration
 *                      reattempt timer.
 *
 *
 * RETURNS:             
 *
 ***************************************************************************/
icf_return_t icf_rgm_reattempt_timer_exp(
        INOUT icf_crm_pdb_st *p_crm_pdb);



/* -------------------------------------------------------------------------- */
/* **** Functions for sending Registration and Deregistration requests**** */
/* -------------------------------------------------------------------------- */

/****************************************************************************
 * FUNCTION:            icf_rgm_register_user
 *
 * DESCRIPTION:         This function is called by the functions of RGM in order                        to send registration requests.
 *
 * RETURNS:             
 *
 ***************************************************************************/
icf_return_t icf_rgm_register_user(
        INOUT icf_crm_pdb_st *p_crm_pdb,
        INOUT icf_rgm_context_st *p_rgm_context);

/****************************************************************************
 * FUNCTION:            icf_rgm_deregister_user
 *
 * DESCRIPTION:         This function is called by the functions of RGM in order                        to send de-registration requests.
 *
 * RETURNS:             
 *
 ***************************************************************************/
icf_return_t icf_rgm_deregister_user(
        INOUT icf_crm_pdb_st *p_crm_pdb,
        INOUT icf_rgm_context_st *p_rgm_context);


/****************************************************************************
 * FUNCTION:            icf_rgm_prepare_timer_struct
 *
 * DESCRIPTION:         This function is called by the functions of RGM for 
 *                      preparing timer structures.
 *
 * RETURNS:             
 *
 ***************************************************************************/
icf_return_t icf_rgm_prepare_timer_struct(
        INOUT icf_glb_pdb_st *p_glb_pdb,
        IN icf_timer_type_t timer_type,
        IN icf_void_t *p_timer_buffer,
        INOUT icf_timer_data_st *p_buff);

/****************************************************************************
 * FUNCTION:            icf_rgm_start_timer
 *
 * DESCRIPTION:         This function is called by the functions of RGM for 
 *                      creating timer structure and starting appropriate timer.
 *
 * RETURNS:             
 *
 ***************************************************************************/
icf_return_t icf_rgm_start_timer(
        INOUT icf_glb_pdb_st *p_glb_pdb,
        IN    icf_timer_type_t timer_type,
        INOUT icf_rgm_context_st *p_rgm_context,
        INOUT icf_error_t *p_ecode);

/****************************************************************************
 * FUNCTION:            icf_rgm_start_expires_timer
 *
 * DESCRIPTION:         This function is called by the functions of RGM for 
 *                      creating timer structure and starting timer of the 
 *                      duration mentioned in Expires field of 200 OK to
 *                      registration request.
 *
 * RETURNS:             
 *
 ***************************************************************************/
icf_return_t icf_rgm_start_expires_timer(
        INOUT icf_glb_pdb_st *p_glb_pdb,
        INOUT icf_duration_t duration,
        INOUT icf_rgm_context_st *p_rgm_context,
        INOUT icf_error_t *p_ecode);

/****************************************************************************
 * FUNCTION:            icf_rgm_clear_reg_context
 *
 * DESCRIPTION:         This function is called by the functions of RGM in order                        to clear registration context.
 * RETURNS:             
 *
 ***************************************************************************/

icf_return_t icf_rgm_clear_reg_context(
        INOUT icf_crm_pdb_st *p_crm_pdb,
        INOUT icf_rgm_context_st *p_rgm_context);

/*******************************************************************************
 *
 * FUNCTION:        icf_rgm_send_pa_display_ind
 *
 * DESCRIPTION:     This function is used to send a display indication
 *                  to the PA on failure in registration process.
 *
 ******************************************************************************/
/*
icf_return_t icf_rgm_send_pa_display_ind(
        IN    icf_display_id_t display_id,
        INOUT icf_crm_pdb_st *p_crm_pdb,
        INOUT icf_rgm_context_st *p_rgm_context);
*/

/****************************************************************************
 * FUNCTION:            icf_rgm_find_n_remove_app
 *
 * DESCRIPTION:         This function is used to find and 
 *						remove the Application's
 *						context from the rgm_context. 
 *
 *
 * RETURNS:             
 ***************************************************************************/
icf_return_t		icf_rgm_find_n_remove_app(
				INOUT	icf_crm_pdb_st		*p_crm_pdb,
				IN		icf_app_id_t			app_id,
				INOUT	icf_rgm_context_st	*p_rgm_context,
				INOUT	icf_config_data_st	*p_config_data,
				OUT		icf_error_t			*p_ecode);

/****************************************************************************
 * FUNCTION:            icf_rgm_remove_app_cntxt
 *
 * DESCRIPTION:         This function is used to remove the Application's
 *						context from the rgm_context. To be used by CFG
 *
 *
 * RETURNS:             
 ***************************************************************************/
icf_return_t	icf_rgm_remove_app_cntxt(
				INOUT	icf_crm_pdb_st	*p_crm_pdb,
				IN		icf_app_id_t		app_id,
				OUT		icf_error_t		*p_ecode);

/****************************************************************************
 * FUNCTION:            icf_rgm_validate_register_api
 *
 * DESCRIPTION:         This function is used to validate the payload
 *                      of REGISTER_REQ API.
 *
 *
 * RETURNS:             
 ***************************************************************************/
icf_void_t icf_rgm_validate_register_api(
          INOUT icf_crm_pdb_st	*p_crm_pdb);


/****************************************************************************
 * FUNCTION:            icf_rgm_parse_string_to_address
 *
 * DESCRIPTION:         This function is used to parse the string
 *                      to address_st type.
 *
 *
 * RETURNS:             
 ***************************************************************************/
icf_void_t   icf_rgm_parse_string_to_address(
                IN      icf_uint8_t          *p_address_string,
                INOUT   icf_address_st          *p_address);
#ifndef ICF_FEATURE_ABANDON
/******************************************************************************
 *
 * FUNCTION:        icf_rgm_get_ctxt_for_uri
 * 
 * DESCRIPTION:     This function returns pointer to regn context for  
 *                  a user (uri). In case no regn context exists then
 *                  pointer to empty ctxt is returned.
 *                   
 * RETURNS:         ICF_SUCCESS: In case regm ctxt is found.       
 *					ICF_FAILURE: In case no regm_ctxt is found.
 ******************************************************************************/
icf_return_t
icf_rgm_get_ctxt_for_uri(
        INOUT icf_glb_pdb_st         *p_glb_pdb,
		IN    icf_address_st         *p_rgm_uri,
        OUT   icf_rgm_context_st     **p_p_rgn_cntxt_blk,
        OUT   icf_error_t            *p_ecode);

#endif
/******************************************************************************
 *
 * FUNCTION:        icf_rgm_update_rgm_cntxt
 * 
 * DESCRIPTION:     This function updates the regn context with the
 *                  payload of register_request API/
 *                  
 *                   
 * RETURNS:         ICF_SUCCESS: In case regm ctxt is updated.       
 *					ICF_FAILURE: In case no regm_ctxt is not updated
 ******************************************************************************/
icf_return_t     icf_rgm_update_rgm_cntxt(
                INOUT icf_crm_pdb_st         *p_crm_pdb,                
                IN    icf_address_st         *p_rgm_uri);
                

/******************************************************************************
 *
 * FUNCTION:        icf_rgm_put_nonce_n_resp
 * 
 * DESCRIPTION:     This function updates the line data with auth Info
 *					received from the network
 *                  
 * 
 * RETURNS:         Nothing
 ******************************************************************************/
icf_void_t	icf_rgm_put_nonce_n_resp(
	INOUT	icf_glb_pdb_st		*p_glb_pdb,
	INOUT	icf_rgm_context_st	*p_rgm_context,
	INOUT	icf_rgm_nw_resp_st	*p_rgm_nw_resp,
	INOUT	icf_uint8_t			*p_response,
	IN		icf_boolean_t		res_flag,/* If this is false, then AUTS to be set */
	INOUT	icf_error_t		*p_ecode);


/****************************************************************************
 * FUNCTION:            icf_rgm_handle_dereg
 *
 * DESCRIPTION:         This function is used to handle the DEREG
 *						request made by the application
 *
 *
 * RETURNS:             
 ***************************************************************************/
icf_return_t		icf_rgm_handle_dereg(
				INOUT	icf_glb_pdb_st		*p_glb_pdb, 
				INOUT	icf_rgm_context_st	*p_rgm_context, 
				INOUT	icf_crm_pdb_st		*p_crm_pdb, 
				OUT		icf_error_t			*p_ecode);

/******************************************************************************
 *
 * FUNCTION:        icf_rgm_fetch_app_info
 * 
 * DESCRIPTION:     This function finds whether the application id
 *					is present in the rgm_context or not
 *                  
 * 
 * RETURNS:         TRUE/FALSE and pointer to app_context
 ******************************************************************************/
icf_boolean_t	icf_rgm_fetch_app_info(
			INOUT	icf_crm_pdb_st			*p_crm_pdb,
			INOUT	icf_rgm_context_st		*p_rgm_context,
			IN		icf_app_id_t				app_id,
			INOUT	icf_rgm_app_ctxt_st		**p_p_app_info);

/******************************************************************************
 *
 * FUNCTION:        icf_rgm_make_cfm_hdr
 * 
 * DESCRIPTION:     This function makes the header for REGISTER_CFM
 *                  
 * 
 * RETURNS:         
 ******************************************************************************/
icf_void_t	icf_rgm_make_cfm_hdr(
					INOUT	icf_msg_st	*p_msg_cfm,
					IN		icf_app_id_t	app_id);

/******************************************************************************
 *
 * FUNCTION:        icf_rgm_make_ind_hdr
 * 
 * DESCRIPTION:     This function makes the header for REGISTER_STATUS_IND
 *                  
 * 
 * RETURNS:         
 ******************************************************************************/
icf_void_t	icf_rgm_make_ind_hdr(
					INOUT	icf_msg_st	*p_msg_ind,
					IN		icf_app_id_t	app_id);

/*****************************************************************
 * FUNCTION:        icf_rgm_send_to_app
 * 
 * DESCRIPTION:     This function sends response to single application
 * 
 * RETURNS:         SUCCESS/ FAILURE
*******************************************************************/
icf_return_t	icf_rgm_send_to_app(
		INOUT	icf_glb_pdb_st			*p_glb_pdb,
		INOUT	icf_rgm_register_resp_st	*p_register_resp,
		OUT		icf_error_t				*p_ecode);

/******************************************************************************
 *
 * FUNCTION:        icf_rgm_send_response
 * 
 * DESCRIPTION:     This function sends the REGISTER response 
 *                  
 * 
 * RETURNS:         SUCCESS/ FAILURE
 ******************************************************************************/
icf_return_t		icf_rgm_send_response(
			INOUT	icf_glb_pdb_st			*p_glb_pdb,
			INOUT	icf_rgm_register_resp_st	*p_register_resp,
			INOUT	icf_error_t		*p_ecode);

/******************************************************************************
 *
 * FUNCTION:        icf_regm_copy_uris
 * 
 * DESCRIPTION:     This function copies the uri from rgm_context
 *					to uri_list_st
 *                  
 *                   
 * RETURNS:         icf_return_t
 ******************************************************************************/
icf_return_t icf_regm_copy_uris(
            icf_glb_pdb_st        *p_glb_pdb,
            icf_pub_uri_list_node_st **p_p_uri_list,
            icf_rgm_context_st    *p_rgm_context,
            icf_line_id_t         line_id,
            icf_error_t           *p_ecode);
/******************************************************************************
 *
 * FUNCTION:        icf_rgm_copy_app_details
 * 
 * DESCRIPTION:     This function copies the application related details
 *					from the register_request payload to rgm_app_cxt_st memory
 *                  
 * 
 * RETURNS:         SUCCESS/FAILURE
 ******************************************************************************/
icf_return_t	icf_rgm_copy_app_details(
				INOUT	icf_glb_pdb_st		*p_glb_pdb,
				INOUT	icf_rgm_app_ctxt_st	*p_app_data,
				INOUT	icf_register_req_st		*p_register_req_payload,
				IN		icf_app_id_t			app_id,
				OUT		icf_error_t			*p_ecode);


/******************************************************************************
 *
 * FUNCTION:        icf_rgm_get_empty_app
 * 
 * DESCRIPTION:     This function returns the pointer to an empty app_data
 *					entry of the rgm_context
 * 
 * RETURNS:         SUCCESS/FAILURE
 ******************************************************************************/
icf_return_t	icf_rgm_get_empty_app(
				INOUT	icf_glb_pdb_st		*p_glb_pdb,
				INOUT	icf_crm_pdb_st		*p_crm_pdb,
				INOUT	icf_rgm_context_st	*p_rgm_context,
				INOUT	icf_rgm_app_ctxt_st	**p_p_app_data,
				OUT		icf_error_t			*p_ecode);

/******************************************************************************
 *
 * FUNCTION:        icf_rgm_update_line_data
 * 
 * DESCRIPTION:     This function updates the line data on receipt of 200 OK
 * 
 * RETURNS:         SUCCESS/FAILURE
 ******************************************************************************/
icf_return_t	icf_rgm_update_line_data(
			INOUT	icf_glb_pdb_st		*p_glb_pdb, 
			INOUT	icf_rgm_context_st	*p_rgm_context,
			INOUT	icf_line_data_st		*p_line_data, 
			INOUT	icf_line_data_st		*p_line_data_with_auth,
			OUT		icf_error_t			*p_ecode);

#ifdef IMS_CLIENT
ICF_CRM_START
/******************************************************************************
 *
 * FUNCTION:        icf_rgm_fetch_nm_info
 * 
 * DESCRIPTION:     This function fetches the information from NM
 * 
 * RETURNS:         SUCCESS/FAILURE
 ******************************************************************************/
icf_return_t	icf_rgm_fetch_nm_info(
			INOUT	icf_glb_pdb_st			*p_glb_pdb, 
			INOUT	icf_rgm_nm_info_st		*p_rgm_nm_info,	
			OUT		icf_error_t				*p_ecode);
#endif
/******************************************************************************
 *
 * FUNCTION:        icf_rgm_process_challenge
 * 
 * DESCRIPTION:     This function processes the challenge by interacting with
 *					NM and sending failure indication to applications if needed
 *					Incase of success, the line data is updated with authorization
 *					related info
 * 
 * RETURNS:         SUCCESS: when AUTH related data is correctly updated
 *					FAILURE: when the AUTH related info is not updated on linedata
 ******************************************************************************/
icf_return_t	icf_rgm_process_challenge(
			INOUT	icf_glb_pdb_st			*p_glb_pdb, 
			INOUT	icf_rgm_nw_resp_st		*p_nw_resp,
			INOUT	icf_crm_pdb_st			*p_crm_pdb,	
			OUT		icf_error_t				*p_ecode);



/*******************************************************************************
 *
 * FUNCTION:        icf_regm_validate_request_header
 * 
 * DESCRIPTION:     This function validates the header for various request APIs 
 *                  
 * RETURNS:         ICF_SUCCESS always
 * 
 *****************************************************************************/
icf_return_t icf_regm_validate_request_header(
        INOUT   icf_crm_pdb_st           *p_crm_pdb);

/*******************************************************************************
 *
 * FUNCTION:        icf_regm_append_hdr_to_list
 * 
 * DESCRIPTION:     This function appends a icf_header_st  
 *					to icf_header_list_st;
 *                  
 * RETURNS:         ICF_SUCCESS always
 * 
 *****************************************************************************/
icf_return_t icf_regm_append_hdr_to_list(
			INOUT	icf_glb_pdb_st			*p_glb_pdb,
			INOUT	icf_header_st			*p_header,
			INOUT	icf_header_list_st		*p_hdr_list,
			INOUT	icf_error_t				*p_ecode);

/****************************************************************************
 * FUNCTION:            icf_rgm_default_nw_resp_hdlr
 *
 * DESCRIPTION:         This function is invoked when DEFAULT(UNHANDLED)
 *						network response is received from SSA
 *
 *
 * RETURNS:             
 *
 ***************************************************************************/
icf_return_t icf_rgm_default_nw_resp_hdlr(
        INOUT icf_crm_pdb_st *p_crm_pdb);


ICF_CRM_END


 

#ifdef ICF_TRACE_ENABLE
/*******************************************************************************
 *
 * FUNCTION:        icf_rgm_trace
 *
 * DESCRIPTION:     This function is used by RGM for trace handling.
 *
 ******************************************************************************/

icf_void_t icf_rgm_trace(
        INOUT icf_crm_pdb_st *p_crm_pdb, 
        IN    icf_trace_id_t trace_id);
/*******************************************************************************
 *
 * FUNCTION:        icf_rgm_app_trace
 *
 * DESCRIPTION:     This function is used by RGM for trace handling.
 *
 ******************************************************************************/

 icf_return_t icf_rgm_app_trace(
         INOUT icf_crm_pdb_st *p_crm_pdb,
         IN    icf_trace_id_t trace_id);
#endif

#ifdef ICF_ERROR_ENABLE
/******************************************************************************
 *
 * FUNCTION:        icf_rgm_error
 * 
 * DESCRIPTION:     This function is invoked by the macro ICF_RGM_SET_ECODE. 
 *                  It fills in the error data on the basis of ecode id and 
 *                  calls the macro ICF_SET_ECODE to report the error.
 *                   
 * RETURNS:         Nothing.       
 *
 ******************************************************************************/

icf_void_t icf_rgm_error(
        INOUT icf_crm_pdb_st            *p_crm_pdb,
        INOUT    icf_error_t               ecode);
#endif


/****************************************************************************
 * FUNCTION:            icf_rgm_05_auth_req_resp
 *
 * DESCRIPTION:         This function is invoked when Authentication required
 *                      response is received in DEREGISTERING state.
 *
 *
 * RETURNS:             
 *
 ***************************************************************************/
icf_return_t icf_rgm_05_auth_req_resp(
        INOUT icf_crm_pdb_st *p_crm_pdb);



ICF_CRM_START

/****************************************************************************
 * FUNCTION:            icf_rgm_process_subscribe_state
 *
 * DESCRIPTION:         This function handles subscription state for reg event.
 *
 *
 * RETURNS:             
 *
 ***************************************************************************/
icf_return_t icf_rgm_process_subscribe_state( 
						INOUT icf_crm_pdb_st *p_crm_pdb); 

/****************************************************************************
 * FUNCTION:            icf_rgm_process_notify
 *
 * DESCRIPTION:         This function handles notify for reg event.
 *
 *
 * RETURNS:             
 *
 ***************************************************************************/
icf_return_t icf_rgm_process_notify(
						INOUT icf_crm_pdb_st *p_crm_pdb); 

ICF_CRM_END

ICF_4_0_CRM_START


/****************************************************************************
 * FUNCTION:            icf_rgm_check_line_data
 *
 * DESCRIPTION:         This function checks the line data for
 *						line id received in the payload for REGISTER_REQ API.
 *						Failure with error_cause is returned in crm_pdb. 
 *
 *
 * RETURNS:             
 *
 ***************************************************************************/
 icf_return_t icf_rgm_check_line_data(
		INOUT icf_crm_pdb_st *p_crm_pdb);

/**************************************************************************
 *
 * FUNCTION:        icf_rgm_get_ctxt_for_pub_uri
 * 
 * DESCRIPTION:     This function returns pointer to regn context for  
 *                  a public user id. In case no regn context exists then
 *                  pointer to empty ctxt is returned.
 *                   
 * RETURNS:         ICF_SUCCESS: In case regm ctxt is found.       
 *                  ICF_FAILURE: In case no regm_ctxt is found.
 **************************************************************************/
icf_return_t    icf_rgm_get_ctxt_for_pub_uri(
            INOUT icf_glb_pdb_st        *p_glb_pdb,
			IN	  icf_line_id_t			line_id,
            INOUT icf_address_st        *p_pub_uri,
            OUT   icf_rgm_context_st    **p_p_rgn_cntxt_blk,
            OUT   icf_error_t			*p_ecode);

/******************************************************************************
 *
 * FUNCTION:        icf_rgm_update_auth_info
 * 
 * DESCRIPTION:     This function updates auth_info for rgm_context kept in crm_pdb.
 *					If the rgm_context is fresh then the auth_data is 
 *					initialized. Incase it is an old rgm_context, then sent_auth_data is 
 *					updated for the new set of recd_auth_data. 
 *					Also the auth_info_present() function is invoked to make the
 *					last line of the array in rgm_context.
 *					The AUTH_SET bit is set for according to the scenario: AKA/non AKA
 *                   
 * RETURNS:         ICF_SUCCESS: .       
 *					ICF_FAILURE: 
 ******************************************************************************/
icf_return_t	icf_rgm_update_auth_info(
			INOUT icf_crm_pdb_st			*p_crm_pdb);

/******************************************************************************
 *
 * FUNCTION:        icf_rgm_process_auth_data()
 * 
 * DESCRIPTION:     This function processes the authorization data by
 *					invoking appropriate functions for diff scenarios.
 *                   
 * RETURNS:         ICF_SUCCESS: 
 *					ICF_FAILURE: 
 *									
 *
 ******************************************************************************/
icf_return_t	icf_rgm_process_auth_data(
			INOUT icf_crm_pdb_st			*p_crm_pdb);


/******************************************************************************
 *
 * FUNCTION:        icf_rgm_non_aka_auth_process
 * 
 * DESCRIPTION:     This function processes the non AKA authorization related
 *					information present in auth_data_recd/sent. 
 *                   
 * RETURNS:         ICF_SUCCESS/ICF_FAILURE
 *									
 *
 ******************************************************************************/
icf_return_t	icf_rgm_non_aka_auth_process(
			INOUT icf_crm_pdb_st			*p_crm_pdb);

/**********************************************************************
 *
 * FUNCTION:      icf_rgm_aka_auth_process

 * 
 * DESCRIPTION:   This function processes AKA authorization related
 *                information present in auth_data_recd/sent. No processing
 *                is done if the sent_nonce == recd_nonce,It initiates the 
 *                process by calling icf_cmn_process_aka_challenge which
 *                returns an appropriate error code.On the basis of the error 
 *                code,an error cause is filled in crm pdb.
 *                                   
 * RETURNS:       ICF_SUCCESS/ICF_FAILURE
 *                                    
 *
 ****************************************************************************/
icf_return_t	icf_rgm_aka_auth_process(
			INOUT icf_crm_pdb_st			*p_crm_pdb);

/******************************************************************************
 *
 * FUNCTION:        icf_rgm_exists_uri
 * 
 * DESCRIPTION:     This function checks whether the address_st parameter passed
 *					is present in rgm_context or not.
 *                   
 * RETURNS:         ICF_SUCCESS: .
 *					ICF_FAILURE: 
 ******************************************************************************/
icf_return_t	icf_rgm_exists_uri(
			INOUT icf_glb_pdb_st			*p_glb_pdb,
			INOUT icf_rgm_context_st		*p_rgm_context,
			INOUT icf_address_st			*p_uri,
			OUT	  icf_boolean_t			*p_status,
			INOUT icf_error_t			*p_ecode);

/******************************************************************************
 *
 * FUNCTION:        icf_rgm_send_subs_on_succ
 * 
 * DESCRIPTION:     This function triggers SIC to send SUBSCRIBE on 200 OK
 
 *                   
 * RETURNS:         ICF_SUCCESS: .
 *					ICF_FAILURE: 
 ******************************************************************************/
icf_return_t	icf_rgm_send_subs_on_succ(
			INOUT icf_crm_pdb_st			*p_crm_pdb,
			INOUT icf_error_t			*p_ecode);

/******************************************************************************
 *
 * FUNCTION:        icf_rgm_update_line_info_on_succ
 * 
 * DESCRIPTION:     This function updates the lines' data on 200
 
 *                   
 * RETURNS:         ICF_SUCCESS: .
 *					ICF_FAILURE: 
 ******************************************************************************/
icf_return_t	icf_rgm_update_line_info_on_succ(
			INOUT icf_glb_pdb_st			*p_glb_pdb,
			INOUT icf_rgm_context_st		*p_rgm_context,
			INOUT icf_error_t			*p_ecode);

/******************************************************************************
 *
 * FUNCTION:        icf_rgm_send_app_resp_on_succ
 * 
 * DESCRIPTION:     This function send CFM to app on 200
 
 *                   
 * RETURNS:         ICF_SUCCESS: .
 *					ICF_FAILURE: 
 ******************************************************************************/
icf_return_t icf_rgm_send_app_resp_on_succ(
					INOUT	icf_crm_pdb_st				*p_crm_pdb,
					INOUT	icf_rgm_register_resp_st		*p_register_response);

/******************************************************************************
 *
 * FUNCTION:        icf_rgm_subs_on_dereg
 * 
 * DESCRIPTION:     This function triggers SIC to send SUBSCRIBE on DEREG
 
 *                   
 * RETURNS:         ICF_SUCCESS: .
 *					ICF_FAILURE: 
 ******************************************************************************/
icf_return_t	icf_rgm_subs_on_dereg(
			INOUT icf_glb_pdb_st			*p_glb_pdb,
			INOUT icf_rgm_context_st		*p_rgm_context,
			INOUT icf_error_t			*p_ecode);

#ifndef ICF_FEATURE_ABANDON
/****************************************************************************
 * FUNCTION:            icf_rgm_all_app_present
 *
 * DESCRIPTION:         This function is finds whether all the applications 
 *						present in the list exists in the rgm_conetxt or not.
 *						Status is passed as a parameter
 *
 * RETURNS:             FAILURE: 
 *						SUCCESS:
 *
 ***************************************************************************/
icf_return_t icf_rgm_all_app_present(
        INOUT	icf_crm_pdb_st		*p_crm_pdb,
		INOUT	icf_app_id_list_st	*p_app_list,
		INOUT	icf_boolean_t		*p_status);
#endif
/****************************************************************************
 * FUNCTION:            icf_rgm_chk_app_details
 *
 * DESCRIPTION:         This function is checks whether the application
 *						is added to an activated network and also it is 
 *						a valid app_id.
 *
 * RETURNS:             FAILURE: 
 *						SUCCESS:
 *
 ***************************************************************************/
icf_return_t		icf_rgm_chk_app_details(
						INOUT	icf_crm_pdb_st		*p_crm_pdb,
						INOUT	icf_app_id_t			app_id);



/*******************************************************************************
 * FUNCTION:        icf_rgm_parse_notify_body
 *
 * DESCRIPTION:     This function parse message body obtained
 *                  in notify message for reg event package
 *
 ******************************************************************************/
icf_return_t icf_rgm_parse_notify_body(
			INOUT	icf_glb_pdb_st		*p_glb_pdb,
			INOUT	icf_rgm_reginfo_st	*p_reg_info,
			INOUT   icf_uint8_t			*p_buffer,
/* SPR 9044: fix_start*/
			INOUT	icf_uint32_t			buf_length,
/* SPR 9044: fix_end */
			OUT		icf_error_t			*p_ecode);

/*******************************************************************************
 *
 * FUNCTION:        icf_rgm_free_contact_list(
 *
 * DESCRIPTION:     This is a utility function that frees the contact_list
 *					maintained in the regn(NOTIFY related registration
 *					subelement) data structure. It is
 *					upto the calling function to free the input ptr if needed.
 *
 ******************************************************************************/
icf_return_t icf_rgm_free_contact_list(
		INOUT	icf_glb_pdb_st			*p_glb_pdb,
        INOUT	icf_rgm_contact_list_st	*p_contact_list,
		OUT		icf_error_t				*p_ecode);


/*******************************************************************************
 *
 * FUNCTION:        icf_rgm_free_regn_list
 *
 * DESCRIPTION:     This is a utility function that frees the regn_list
 *					maintained in the reginfo  data structure. It is
 *					upto the calling function to free the input ptr if needed.
 *
 ******************************************************************************/
icf_return_t icf_rgm_free_regn_list(
		INOUT	icf_glb_pdb_st			*p_glb_pdb,
        INOUT	icf_rgm_regn_list_st		*p_regn_list,
		OUT		icf_error_t				*p_ecode);


/*******************************************************************************
 *
 * FUNCTION:        icf_rgm_validate_rgm_info_version_state
 *
 * DESCRIPTION:     This is a utility function that validates the version
 *					number recd in NOTIFy with the one stored in rgm_context.
 *	
 * RETURNS:			Success: received version is > stored version
 *							Nothing stored, 0 received.
 *					Faiure: if 0 not received when no old reg_info stored.
 *							received version <= stored version 
 ******************************************************************************/
icf_return_t icf_rgm_validate_rgm_info_version_state(
		INOUT	icf_crm_pdb_st			*p_crm_pdb,
        INOUT	icf_rgm_reginfo_st		*p_reg_info);

/*******************************************************************************
 *
 * FUNCTION:        icf_rgm_handle_notify_regn_blk
 *
 * DESCRIPTION:     This is a utility function that handles the regn_block
 *					obtained from the NOTIFY: reg_info for 
 *					the rgm_context present in crm_pdb. 
 *					
 *	
 * RETURNS:			Success: 
 *							 
 *
 *					Faiure: 						
 ******************************************************************************/
icf_return_t  icf_rgm_handle_notify_regn_blk(
			INOUT	icf_crm_pdb_st				*p_crm_pdb, 
			INOUT	icf_rgm_regn_block_st		*p_regn_blk);

/*******************************************************************************
 *
 * FUNCTION:        icf_rgm_nfy_regn_init
 *
 * DESCRIPTION:     This is a utility function that handles the regn_block
 *					obtained from the NOTIFY: when the state is INIT
 *					
 *	
 * RETURNS:			Success: 
 *							 
 *
 *					Faiure: 						
 ******************************************************************************/
icf_return_t  icf_rgm_nfy_regn_init(
			INOUT	icf_crm_pdb_st				*p_crm_pdb, 
			INOUT	icf_rgm_regn_block_st		*p_regn_blk);       

/*******************************************************************************
 *
 * FUNCTION:        icf_rgm_nfy_regn_active
 *
 * DESCRIPTION:     This is a utility function that handles the regn_block
 *					obtained from the NOTIFY: when the state is ACTIVE
 *					
 *	
 * RETURNS:			Success: 
 *							 
 *
 *					Faiure: 						
 ******************************************************************************/
icf_return_t  icf_rgm_nfy_regn_active(
			INOUT	icf_crm_pdb_st				*p_crm_pdb, 
			INOUT	icf_rgm_regn_block_st		*p_regn_blk);       

/*******************************************************************************
 *
 * FUNCTION:        icf_rgm_nfy_regn_terminated
 *
 * DESCRIPTION:     This is a utility function that handles the regn_block
 *					obtained from the NOTIFY: when the state is TERMINATED
 *					
 *	
 * RETURNS:			Success: 
 *							 
 *
 *					Faiure: 						
 ******************************************************************************/
icf_return_t  icf_rgm_nfy_regn_terminated(
			INOUT	icf_crm_pdb_st				*p_crm_pdb, 
			INOUT	icf_rgm_regn_block_st		*p_regn_blk);     

/*******************************************************************************
 *
 * FUNCTION:        icf_rgm_nfy_contact_active
 *
 * DESCRIPTION:     This is a utility function that handles the contact_block
 *					obtained from the NOTIFY: when the contact_state is ACTIVE
 *					
 *	
 * RETURNS:			Success: 							 
 *
 *					Faiure: 						
 ******************************************************************************/
icf_return_t  icf_rgm_nfy_contact_active(
			INOUT	icf_crm_pdb_st				*p_crm_pdb, 
			INOUT	icf_rgm_contact_blk_st		*p_contact_blk);       

/*******************************************************************************
 *
 * FUNCTION:        icf_rgm_nfy_contact_terminated
 *
 * DESCRIPTION:     This is a utility function that handles the contact_blk
 *					obtained from the NOTIFY: when the contact_state is TERMINATED
 *					
 *	
 * RETURNS:			Success: 
 *							 
 *
 *					Faiure: 						
 ******************************************************************************/
icf_return_t  icf_rgm_nfy_contact_terminated(
			INOUT	icf_crm_pdb_st				*p_crm_pdb, 
			INOUT	icf_rgm_contact_blk_st		*p_contact_blk,
            INOUT   icf_address_st              *p_aor);     


/******************************************************************************
 *      UTILITY FUNCTIONS FOR FILLING REGINFO
 ******************************************************************************/

/***************************************************************
 * FUNCTION:  icf_rgm_init_address
 *
 * DESCRIPTION: This function Gets Memory required for icf_address_st
 *              and Initialise it properly
 **************************************************************/ 
icf_return_t icf_rgm_init_address(
                                   INOUT  icf_glb_pdb_st             *p_glb_pdb,
                                   OUT   icf_address_st **p_p_Aor,
                                   INOUT icf_error_t     *p_ecode);

/******************************************************************************
 * FUNCTION:  icf_rgm_init_regnblock
 *
 * DESCRIPTION: This function Gets Memory required for icf_rgm_regn_block_st
 *              and Initialise it properly
 *****************************************************************************/ 
icf_return_t icf_rgm_init_regnblock(
                               INOUT  icf_glb_pdb_st             *p_glb_pdb,
                               OUT   icf_rgm_regn_block_st    **p_p_regn_block,
                               INOUT icf_error_t              *p_ecode);


/******************************************************************************
 * FUNCTION:  icf_rgm_init_contact_blk
 *
 * DESCRIPTION: This function Gets Memory required for icf_rgm_contact_blk_st
 *              and Initialise it properly
 *****************************************************************************/ 
icf_return_t icf_rgm_init_contact_blk(
                              INOUT  icf_glb_pdb_st       *p_glb_pdb,
                              OUT icf_rgm_contact_blk_st **p_p_contact_block,
                              INOUT icf_error_t     *p_ecode);

/******************************************************************************
 * FUNCTION:  icf_rgm_init_glb_parse_reginfo_block
 *
 * DESCRIPTION: This function Gets Memory required for 
 *              icf_rgm_glb_parse_reginfo_st
 *              and Initialise it properly
 *****************************************************************************/ 
icf_return_t icf_rgm_init_glb_parse_reginfo_block(
                     INOUT  icf_glb_pdb_st                *p_glb_pdb,
					 IN  icf_rgm_reginfo_st            *p_reginfo,
                     OUT icf_rgm_glb_parse_reginfo_st  **p_p_glb_parse_reginfo,
                     INOUT icf_error_t        *p_ecode);
/******************************************************************************
 * FUNCTION:  icf_rgm_free_regn_block
 *
 * DESCRIPTION: This function frees the Registration Block
 *              
 *****************************************************************************/ 
icf_return_t icf_rgm_free_regn_block(
                             INOUT  icf_glb_pdb_st           *p_glb_pdb,
                             INOUT icf_rgm_regn_block_st  *p_regn_block,
                             INOUT icf_error_t            *p_ecode);

/******************************************************************************
 * FUNCTION:  icf_rgm_free_contact_block
 *
 * DESCRIPTION: This function frees the Contact Block
 *              
 *****************************************************************************/ 
icf_return_t icf_rgm_free_contact_block(
                               INOUT  icf_glb_pdb_st            *p_glb_pdb,
                               INOUT icf_rgm_contact_blk_st  *p_contact_block,
                               INOUT icf_error_t             *p_ecode);

/******************************************************************************
 * FUNCTION:  icf_rgm_free_reginfo
 *
 * DESCRIPTION: This function frees the  Registration Info block 
 *
 *****************************************************************************/ 
icf_return_t icf_rgm_free_reginfo(
                               INOUT  icf_glb_pdb_st       *p_glb_pdb,
                               INOUT icf_rgm_reginfo_st  *p_reginfo,
                               INOUT icf_error_t       *p_ecode);


/******************************************************************************
 * FUNCTION:  icf_rgm_free_glb_parse_reginfo
 *
 * DESCRIPTION: This function frees the Global Parse Registration Info block 
 *
 *****************************************************************************/ 
icf_return_t icf_rgm_free_glb_parse_reginfo(
                INOUT  icf_glb_pdb_st                  *p_glb_pdb,
                INOUT icf_rgm_glb_parse_reginfo_st  *p_glb_parse_reginfo,
                INOUT icf_error_t       *p_ecode);



/******************************************************************************
 * FUNCTION:  icf_rgm_fill_reginfo_attribute
 *
 * DESCRIPTION: This function Fills RegInfo Attributes 
 *              
 *****************************************************************************/ 
icf_return_t icf_rgm_fill_reginfo_attribute(
                                IN  icf_glb_pdb_st       *p_glb_pdb,
								INOUT icf_rgm_reginfo_st *p_rgm_reg_info,
								IN    icf_uint8_t *p_attribute,
								IN    icf_uint8_t *p_Value,
								INOUT icf_error_t *p_ecode);
/******************************************************************************
 * FUNCTION:  icf_rgm_fill_regn_blk_attribute
 *
 * DESCRIPTION: This function Fills Registration Block Attributes to
 *              icf_rgm_regn_block_st
 *****************************************************************************/
icf_return_t icf_rgm_fill_regn_blk_attribute(
                            INOUT  icf_glb_pdb_st       *p_glb_pdb,
							INOUT   icf_rgm_regn_block_st    *p_regn_block,
							IN    	icf_uint8_t *p_attribute,
							IN    	icf_uint8_t *p_Value,
							INOUT 	icf_error_t *p_ecode);


/******************************************************************************
 * FUNCTION:  icf_rgm_fill_contact_blk_attribute
 *
 * DESCRIPTION: This function Fills Contact Block Attributes to 
 *              icf_rgm_contact_blk_st
 *              
 *****************************************************************************/
icf_return_t icf_rgm_fill_contact_blk_attribute(
                            IN  icf_glb_pdb_st       *p_glb_pdb,
							INOUT   icf_rgm_contact_blk_st *p_contact_block,
							IN      icf_uint8_t *p_attribute,
							IN      icf_uint8_t *p_Value,
							INOUT   icf_error_t *p_ecode);


/******************************************************************************
 * FUNCTION:  icf_regm_append_regnblk_to_regnblk_list
 *
 * DESCRIPTION: This function Fills Regisration Block 
 *              icf_rgm_contact_blk_st to Registration List
 *              
 *****************************************************************************/
icf_return_t    icf_regm_append_regnblk_to_regnblk_list(
                      INOUT  icf_rgm_regn_block_st		 	*p_regn_block,
                      INOUT  icf_rgm_regn_list_st           *p_regn_list,
                      INOUT  icf_error_t                 *p_ecode);


/******************************************************************************
 * FUNCTION:  icf_regm_append_contactblk_to_contact_list
 *
 * DESCRIPTION: This function appends the Contact Block  
 *              icf_rgm_contact_blk_st to Contact List 
 *              icf_rgm_contact_list_st
 *              
 *****************************************************************************/
icf_return_t    icf_regm_append_contactblk_to_contact_list(
                      INOUT    icf_rgm_contact_blk_st		*p_contact_blk,
                      INOUT    icf_rgm_contact_list_st      *p_contact_list,
                      INOUT    icf_error_t                  *p_ecode);



ICF_4_0_CRM_END




#ifdef ICF_IPSEC_ENABLED
/* -------------------------------------------------------------------------- */
/* **** RGM IPSec Functions **** */
/* -------------------------------------------------------------------------- */

/*******************************************************************************
 *
 * FUNCTION:        icf_rgm_ipsec_sa_clean_up
 *
 * DESCRIPTION:     This is a utility function that sends deletion request to NM
 *                  for all existing SAs, starts response timer and changes 
 *                  state to the one passed as argument. The calling function 
 *                  needs to set the sa_identifier and next_state correctly 
 *                  before invoking this function.
 *
 * RETURNS:         ICF_SUCCESS on success, ICF_FAILURE on failure
 *
 ******************************************************************************/
icf_return_t icf_rgm_ipsec_sa_clean_up(
        INOUT   icf_crm_pdb_st		*p_crm_pdb,
        IN	    icf_uint8_t		    next_state);


/*******************************************************************************
 *
 * FUNCTION:        icf_rgm_ipsec_alloc_sa_context
 *
 * DESCRIPTION:     This is a utility function that will allocate memory for a 
 *                  new SA context structure, initialize its members with 
 *                  default values and return the pointer to calling function.
 *
 * RETURNS:         ICF_SUCCESS on success, ICF_FAILURE on failure
 *
 ******************************************************************************/
icf_return_t icf_rgm_ipsec_alloc_sa_context(
	    INOUT   icf_crm_pdb_st			*p_crm_pdb,
        OUT 	icf_ipsec_sa_context_st  **p_p_sa_context);


/*******************************************************************************
 *
 * FUNCTION:        icf_rgm_ipsec_dealloc_sa_context
 *
 * DESCRIPTION:     This is a utility function that will de-allocate SA context
 *                  identified by the SA identifier in REGM context.
 *
 * RETURNS:         ICF_SUCCESS on success, ICF_FAILURE on failure
 *
 ******************************************************************************/
icf_return_t icf_rgm_ipsec_dealloc_sa_context(
	    INOUT   icf_crm_pdb_st		*p_crm_pdb);


/*******************************************************************************
 *
 * FUNCTION:        icf_rgm_ipsec_get_sec_params
 *
 * DESCRIPTION:     This is a utility function that will allocate memory for a 
 *                  new security parameter structure, initialize its members 
 *                  with default values and return pointer to calling function.
 *
 * RETURNS:         ICF_SUCCESS on success, ICF_FAILURE on failure
 *
 ******************************************************************************/
icf_return_t icf_rgm_ipsec_get_sec_params(
	    INOUT   icf_crm_pdb_st				*p_crm_pdb,
        OUT 	icf_ipsec_security_params_st	**p_p_sec_params);


/*******************************************************************************
 *
 * FUNCTION:        icf_rgm_ipsec_get_unused_port
 *
 * DESCRIPTION:     This is a utility function that will return, by value, a 
 *                  port from the configured range that is not in use by any 
 *                  existing SA. This port will then be used as the local 
 *                  protected client port for new SA creation. Whenever this 
 *                  function is invoked, pointer to old SA context in regm_cntxt
 *                  will always be null but the pointer to new SA context may 
 *                  (initial reg) or may not (re-reg) be null.
 *
 * RETURNS:         ICF_SUCCESS on success, ICF_FAILURE on failure
 *
 ******************************************************************************/
icf_return_t icf_rgm_ipsec_get_unused_port(
	    INOUT   icf_crm_pdb_st		*p_crm_pdb,
        OUT 	icf_uint16_t		    *p_new_port);


/*******************************************************************************
 *
 * FUNCTION:        icf_rgm_ipsec_get_unused_spi
 *
 * DESCRIPTION:     This is a utility function that will generate a random spi 
 *                  value for a new security association. It will return a value
 *                  that is not equal to used_spi and also not in use in new SA 
 *                  context. Whenever this function is invoked, pointer to old 
 *                  SA context in regm_cntxt will always be null but the pointer
 *                  to new SA ctx may (initial reg) or may not (re-reg) be null.
 *
 * RETURNS:         ICF_SUCCESS on success, ICF_FAILURE on failure
 *
 ******************************************************************************/
icf_return_t icf_rgm_ipsec_get_unused_spi(
	    INOUT   icf_crm_pdb_st		*p_crm_pdb,
        IN 	    icf_uint32_t		    used_spi,
        OUT 	icf_uint32_t		    *p_new_spi);


/*******************************************************************************
 *
 * FUNCTION:        icf_rgm_ipsec_add_sec_agree_tag
 *
 * DESCRIPTION:     This is a utility function that will add tag lists for 
 *                  "sec-agree" in Require and Proxy-Require headers in the 
 *                  application context in REGM context. This function needs to 
 *                  be called only on receiving register request from 
 *                  application, and will be available to SSA for sending all 
 *                  subsequent Register requests.
 *
 * RETURNS:         ICF_SUCCESS on success, ICF_FAILURE on failure
 *
 ******************************************************************************/
icf_return_t icf_rgm_ipsec_add_sec_agree_tag(
        INOUT   icf_crm_pdb_st		*p_crm_pdb);


/*******************************************************************************
 *
 * FUNCTION:        icf_rgm_ipsec_nm_sa_fetch_algo
 *
 * DESCRIPTION:     This is a utility function for fetching the list of locally 
 *                  supported security algorithms from NM and saving them in 
 *                  REGM global data.
 *
 * RETURNS:         ICF_SUCCESS on success, ICF_FAILURE on failure
 *
 ******************************************************************************/
icf_return_t icf_rgm_ipsec_nm_sa_fetch_algo(
        INOUT   icf_crm_pdb_st		    *p_crm_pdb,
        OUT     icf_ipsec_self_algo_st   **p_p_rgm_algo_list);


/*******************************************************************************
 *
 * FUNCTION:        icf_rgm_ipsec_nm_sa_create_req
 *
 * DESCRIPTION:     This is a utility function that will make and send request 
 *                  to NM for the creation of all new unidirectional SAs for a 
 *                  secure connection between one IMPI and P-CSCF. It will use 
 *                  the parameters stored in new SA context structure within the
 *                  REGM context for filling the request structure.
 *
 *                  This function will take care of sending as many requests to 
 *                  NM as the number of unidirectional SAs that should be 
 *                  created. Hence, the state machine handler needs to invoke 
 *                  this function only once for SA creation.
 *
 * RETURNS:         ICF_SUCCESS on success, ICF_FAILURE on failure
 *
 ******************************************************************************/
icf_return_t icf_rgm_ipsec_nm_sa_create_req(
        INOUT   icf_crm_pdb_st		*p_crm_pdb);


#ifdef ICF_SA_LIFETIME_MOD_SUPPORT
/*******************************************************************************
 *
 * FUNCTION:        icf_rgm_ipsec_nm_sa_modify_req
 *
 * DESCRIPTION:     This is a utility function that will make and send request 
 *                  to NM for the modification of all unidirectional SAs for a 
 *                  secure connection between one IMPI and P-CSCF. It will use 
 *                  the parameters stored in new SA context structure within the
 *                  REGM context and the new duration passed as argument for 
 *                  filling the request structure.
 *
 *                  This function will take care of sending as many requests to 
 *                  NM as the number of unidirectional SAs that should be 
 *                  modified. Hence, the state machine handler needs to invoke 
 *                  this function only once for SA modification.
 *
 *                  It will be invoked only if the underlying platform supports 
 *                  SA lifetime modification and hence shall be under 
 *                  ICF_SA_LIFETIME_MOD_SUPPORT compilation flag.
 *
 * RETURNS:         ICF_SUCCESS on success, ICF_FAILURE on failure
 *
 ******************************************************************************/
icf_return_t icf_rgm_ipsec_nm_sa_modify_req(
	    INOUT   icf_crm_pdb_st		*p_crm_pdb,
        OUT	    icf_duration_t		new_lifetime);
#endif /* ICF_SA_LIFETIME_MOD_SUPPORT */


/*******************************************************************************
 *
 * FUNCTION:        icf_rgm_ipsec_nm_sa_delete_req
 *
 * DESCRIPTION:     This is a utility function that will make and send request 
 *                  to NM for the deletion of all unidirectional SAs for a 
 *                  secure connection between one IMPI and P-CSCF. It will use 
 *                  the sa_identifier parameter stored in REGM context to find 
 *                  the list of SAs that need to be filled in request structure.
 *
 *                  This function will take care of sending as many requests to 
 *                  NM as the number of unidirectional SAs that should be 
 *                  deleted. Hence, the state machine handler needs to invoke 
 *                  this function only once for SA deletion.
 *
 * RETURNS:         ICF_SUCCESS on success, ICF_FAILURE on failure
 *
 ******************************************************************************/
icf_return_t icf_rgm_ipsec_nm_sa_delete_req(
        INOUT   icf_crm_pdb_st		*p_crm_pdb);


/*******************************************************************************
 *
 * FUNCTION:        icf_rgm_ipsec_match_resp_spi
 *
 * DESCRIPTION:     This is a utility function that resets the bit in 
 *                  req_bitmask corresponding to the spi value for which 
 *                  response from NM has been received.
 *
 * RETURNS:         Success if the response SPI matches with any of the request
 *                  SPI values, else Failure
 *
 ******************************************************************************/
icf_return_t icf_rgm_ipsec_match_resp_spi(
        INOUT   icf_crm_pdb_st		*p_crm_pdb,
        IN      icf_uint32_t         resp_spi);


/*******************************************************************************
 *
 * FUNCTION:        icf_rgm_ipsec_process_nm_msg
 *
 * DESCRIPTION:     This is a function that is invoked by CFG on receiving a 
 *                  message from NM. It will trigger the state machine handler.
 *
 * RETURNS:         Nothing
 *
 ******************************************************************************/
icf_return_t icf_rgm_ipsec_process_nm_msg(
        INOUT   icf_crm_pdb_st		*p_crm_pdb);


/*******************************************************************************
 *
 * FUNCTION:        icf_rgm_01_sa_delete_resp
 *
 * DESCRIPTION:     This is a state machine handler that will be invoked when 
 *                  REGM receives SA deletion response in registering state.
 *
 * RETURNS:         ICF_SUCCESS on success, ICF_FAILURE on failure
 *
 ******************************************************************************/
icf_return_t icf_rgm_01_sa_delete_resp(
        INOUT   icf_crm_pdb_st		*p_crm_pdb);


/*******************************************************************************
 *
 * FUNCTION:        icf_rgm_01_sa_delete_timer_exp
 *
 * DESCRIPTION:     This is a state machine handler that will be invoked when 
 *                  REGM receives SA deletion guard timer expiry in registering 
 *                  state. This will just raise an alarm and return.
 *
 * RETURNS:         ICF_SUCCESS on success, ICF_FAILURE on failure
 *
 ******************************************************************************/
icf_return_t icf_rgm_01_sa_delete_timer_exp(
        INOUT   icf_crm_pdb_st		*p_crm_pdb);


#ifdef ICF_SA_LIFETIME_MOD_SUPPORT
/*******************************************************************************
 *
 * FUNCTION:        icf_rgm_03_sa_modify_resp
 *
 * DESCRIPTION:     This is a state machine handler that will be invoked when 
 *                  REGM receives SA modification response in registered state.
 *
 *                  SA modification will be done only if the underlying platform
 *                  supports SA lifetime change, and hence this function will be
 *                  inside the compilation flag ICF_SA_LIFETIME_MOD_SUPPORT.
 *
 * RETURNS:         ICF_SUCCESS on success, ICF_FAILURE on failure
 *
 ******************************************************************************/
icf_return_t icf_rgm_03_sa_modify_resp(
        INOUT   icf_crm_pdb_st		*p_crm_pdb);
#endif /* ICF_SA_LIFETIME_MOD_SUPPORT */


#ifdef ICF_SA_LIFETIME_MOD_SUPPORT
/*******************************************************************************
 *
 * FUNCTION:        icf_rgm_03_sa_modify_timer_exp
 *
 * DESCRIPTION:     This is a state machine handler that will be invoked when 
 *                  REGM receives SA modification guard timer expiry in 
 *                  registered state.
 *
 *                  SA modification will be done only if the underlying platform
 *                  supports SA lifetime change, and hence this function will be
 *                  inside the compilation flag ICF_SA_LIFETIME_MOD_SUPPORT.
 *
 * RETURNS:         ICF_SUCCESS on success, ICF_FAILURE on failure
 *
 ******************************************************************************/
icf_return_t icf_rgm_03_sa_modify_timer_exp(
        INOUT   icf_crm_pdb_st		*p_crm_pdb);
#endif /* ICF_SA_LIFETIME_MOD_SUPPORT */


/*******************************************************************************
 *
 * FUNCTION:        icf_rgm_03_sa_delete_resp
 *
 * DESCRIPTION:     This is a state machine handler that will be invoked when 
 *                  REGM receives SA deletion response in registered state.
 *
 * RETURNS:         ICF_SUCCESS on success, ICF_FAILURE on failure
 *
 ******************************************************************************/
icf_return_t icf_rgm_03_sa_delete_resp(
        INOUT   icf_crm_pdb_st		*p_crm_pdb);


/*******************************************************************************
 *
 * FUNCTION:        icf_rgm_03_sa_delete_timer_exp
 *
 * DESCRIPTION:     This is a state machine handler that will be invoked when 
 *                  REGM receives SA deletion guard timer expiry in registered 
 *                  state.
 *
 * RETURNS:         ICF_SUCCESS on success, ICF_FAILURE on failure
 *
 ******************************************************************************/
icf_return_t icf_rgm_03_sa_delete_timer_exp(
        INOUT   icf_crm_pdb_st		*p_crm_pdb);


/*******************************************************************************
 *
 * FUNCTION:        icf_rgm_03_sa_expiry_ind
 *
 * DESCRIPTION:     This is a state machine handler that will be invoked when 
 *                  REGM receives an SA expiry indication from NM. It will 
 *                  forcefully clear all the ongoing dialogs, send indication to
 *                  the application, stop any timer that is running and clear 
 *                  the REGM context.
 *
 * RETURNS:         ICF_SUCCESS on success, ICF_FAILURE on failure
 *
 ******************************************************************************/
icf_return_t icf_rgm_03_sa_expiry_ind(
        INOUT   icf_crm_pdb_st		*p_crm_pdb);


/*******************************************************************************
 *
 * FUNCTION:        icf_rgm_ipsec_01_sa_create_resp
 *
 * DESCRIPTION:     This is a state machine handler that will be invoked when 
 *                  REGM receives SA creation response in state awaiting SA 
 *                  creation response.
 *
 * RETURNS:         ICF_SUCCESS on success, ICF_FAILURE on failure
 *
 ******************************************************************************/
icf_return_t icf_rgm_ipsec_01_sa_create_resp(
        INOUT   icf_crm_pdb_st		*p_crm_pdb);


/*******************************************************************************
 *
 * FUNCTION:        icf_rgm_ipsec_01_sa_create_timer_exp
 *
 * DESCRIPTION:     This is a state machine handler that will be invoked when 
 *                  REGM receives SA creation guard timer expiry in state 
 *                  awaiting SA create response.
 *
 * RETURNS:         ICF_SUCCESS on success, ICF_FAILURE on failure
 *
 ******************************************************************************/
icf_return_t icf_rgm_ipsec_01_sa_create_timer_exp(
        INOUT   icf_crm_pdb_st		*p_crm_pdb);


/*******************************************************************************
 *
 * FUNCTION:        icf_rgm_ipsec_07_sa_delete_resp
 *
 * DESCRIPTION:     This is a state machine handler that will be invoked when 
 *                  REGM receives SA deletion response in state awaiting SA 
 *                  delete response.
 *
 * RETURNS:         ICF_SUCCESS on success, ICF_FAILURE on failure
 *
 ******************************************************************************/
icf_return_t icf_rgm_ipsec_07_sa_delete_resp(
        INOUT   icf_crm_pdb_st		*p_crm_pdb);


/*******************************************************************************
 *
 * FUNCTION:        icf_rgm_ipsec_07_sa_delete_timer_exp
 *
 * DESCRIPTION:     This is a state machine handler that will be invoked when 
 *                  REGM receives SA deletion guard timer expiry in state 
 *                  awaiting SA delete response. This will just raise an alarm 
 *                  and return.
 *
 * RETURNS:         ICF_SUCCESS on success, ICF_FAILURE on failure
 *
 ******************************************************************************/
icf_return_t icf_rgm_ipsec_07_sa_delete_timer_exp(
        INOUT   icf_crm_pdb_st		*p_crm_pdb);


/*******************************************************************************
 *
 * FUNCTION:        icf_rgm_ipsec_delete_old_sa
 *
 * DESCRIPTION:     This new REGM function shall be invoked by CFG on getting 
 *                  ICF_DELETE_OLD_SA internal message from SSA. It will be 
 *                  handled outside the state machine and shall send request to 
 *                  NM to delete the old SA. REGM will not wait for the response
 *                  of this request and will not start any guard timer.
 *
 * RETURNS:         ICF_SUCCESS on success, ICF_FAILURE on failure
 *
 ******************************************************************************/
icf_return_t icf_rgm_ipsec_delete_old_sa(
        INOUT   icf_crm_pdb_st		*p_crm_pdb);


/* ------------------ End: New functions for IP Security Support ------------ */
#endif /* ICF_IPSEC_ENABLED */


/* -------------------------------------------------------------------------- */
/* ** More RGM Utility functions added in ICF Rel 4.0 during IPSec design  ** */
/* -------------------------------------------------------------------------- */

/*******************************************************************************
 *
 * FUNCTION:        icf_rgm_stop_timer
 *
 * DESCRIPTION:     This is a utility function that will stop the currently 
 *                  running timer and free the timer block. It does not need the
 *                  timer-id for the timer to be stopped, since currently REGM 
 *                  runs only one timer at a time.
 *
 * RETURNS:         ICF_SUCCESS on success, ICF_FAILURE on failure
 *
 ******************************************************************************/
icf_return_t icf_rgm_stop_timer(
        INOUT   icf_crm_pdb_st		*p_crm_pdb);

#ifdef ICF_SA_LIFETIME_MOD_SUPPORT
/*******************************************************************************
 *
 * FUNCTION:        icf_rgm_get_timestamp
 *
 * DESCRIPTION:     This is a utility function that will return the current 
 *                  system time in milliseconds.
 *
 * RETURNS:         ICF_SUCCESS on success, ICF_FAILURE on failure
 *
 ******************************************************************************/
icf_return_t icf_rgm_get_timestamp(
        INOUT   icf_crm_pdb_st		*p_crm_pdb,
        OUT	    icf_time_t		    *p_time);
#endif
/******************************************************************************
 *
 * FUNCTION:        icf_rgm_send_clear_subs_on_dereg
 * 
 * DESCRIPTION:     This function triggers SIC to clear context on De-reg.
 
 *                   
 * RETURNS:         ICF_SUCCESS: .
 *					ICF_FAILURE: 
 ******************************************************************************/
icf_return_t	icf_rgm_send_clear_subs_on_dereg(
			INOUT icf_crm_pdb_st		*p_crm_pdb,
			INOUT icf_error_t			*p_ecode);

/****************************************************************************
 * FUNCTION:            icf_rgm_prbn_timer_exp
 *
 * DESCRIPTION:         This function is invoked on expiry of probation
 *                      timer which was started in
 *
 *
 * RETURNS:             
 *
 ***************************************************************************/
icf_return_t icf_rgm_prbn_timer_exp(
        INOUT icf_crm_pdb_st *p_crm_pdb);


/****************************************************************************
 * FUNCTION:            icf_rgm_ctx_handled_after_nfy
 *
 * DESCRIPTION:         This function is invoked after all
 *                      contacts of the NOTIFY are handled.
 *
 * RETURNS:             
 *
 ***************************************************************************/
icf_return_t icf_rgm_ctx_handled_after_nfy(
        INOUT icf_crm_pdb_st *p_crm_pdb);

/****************************************************************************
 * FUNCTION:            icf_rgm_refresh_register
 *
 * DESCRIPTION:         This function is invoked after 200
 *                      is received in AUTHORIZING or REGISTERING
 *                      or DEREGISTERING state. 
 *                       Incase probation timer expired while rgm_context
 *                      was waiting for response, 
 *                      a refresh REGISTER should be sent 
 *
 * RETURNS:             
 *
 ***************************************************************************/
icf_return_t icf_rgm_refresh_register(
        INOUT icf_crm_pdb_st *p_crm_pdb);

/****************************************************************************
 * FUNCTION:            icf_rgm_verify_ctx_status
 *
 * DESCRIPTION:         This function is invoked after NOTIFY msg
 *                      body has been processed.
 *                      It verifies the state of rgm_ctx to see
 *                      if all contacts have been terminated with
 *                      no further REGISTER needed to be sent on the
 *                      network, in which case rgm_ctx will be cleared
 *
 * RETURNS:             
 *
 ***************************************************************************/
icf_return_t icf_rgm_verify_ctx_status(
        INOUT icf_crm_pdb_st *p_crm_pdb);

/*******************************************************************************
 *
 * FUNCTION:        icf_rgm_check_for_buffer_condition
 *
 * DESCRIPTION:     This is a utility function that checks for buffering condition
 *                  
 *                  
 *
 * RETURNS:         ICF_TRUE if the current registration procedure should be 
 *                  buffered. ICF_FALSE otherwise
 *
 ******************************************************************************/

icf_boolean_t icf_rgm_check_for_buffer_condition(icf_crm_pdb_st *p_crm_pdb);

/*******************************************************************************
 *
 * FUNCTION:        icf_process_buffer
 *
 * DESCRIPTION:     This is a utility function that processes the buffered list
 *                  and invokes the next buffered registration block
 *                  
 *
 * RETURNS:         icf_return_t 
 *
 ******************************************************************************/
icf_return_t icf_process_buffer(icf_crm_pdb_st *p_crm_pdb);

/*******************************************************************************
 *
 * FUNCTION:        icf_rgm_get_next_buffer_id
 *
 * DESCRIPTION:     This is a utility function that returns the next available
 *                  buffer id.
 *                  
 *
 * RETURNS:         icf_uint8_t
 *
 ******************************************************************************/
icf_uint8_t icf_rgm_get_next_buffer_id(icf_crm_pdb_st *p_crm_pdb);


#ifdef ICF_SIGCOMP_SUPPORT
/****************************************************************************
 * FUNCTION:           icf_rgm_send_create_comp_req_to_sigsa
 *
 * DESCRIPTION:        This routine Sends CREATE_COMP_REQUEST TO
 *                     SIGSA for Creating the compartment for sending
 *                     Compressed Message
 *****************************************************************************/
icf_return_t icf_rgm_send_create_comp_req_to_sigsa(
        INOUT  icf_crm_pdb_st   *p_crm_pdb);

/****************************************************************************
 * FUNCTION:           icf_rgm_send_delete_comp_ind_to_sigsa
 *
 * DESCRIPTION:        This routine Sends ICF_DELETE_COMP_IND TO
 *                     SIGSA for deleting the compartment 
 *****************************************************************************/
icf_return_t icf_rgm_send_delete_comp_ind_to_sigsa(
        INOUT  icf_crm_pdb_st   *p_crm_pdb);


/* External Utility Function dependant upon SIGSA */
/*extern 
icf_return_t icf_sigsa_decr_ref_count_to_sigsa_ctxt(
                                INOUT icf_void_t *p_sigsa_ctxt);
icf_return_t icf_sigsa_incr_ref_count_to_sigsa_ctxt(
                                INOUT icf_void_t *p_sigsa_ctxt);
*/
#endif
/*******************************************************************************
 *
 * FUNCTION:        icf_rgm_make_rgm_context
 *
 * DESCRIPTION:     This is a utility function that makes a new rrm_context for a 
 *					user
 *                  
 *
 * RETURNS:         icf_return_t 
 *
 ******************************************************************************/
icf_return_t icf_rgm_make_rgm_context(
						INOUT	icf_glb_pdb_st		*p_glb_pdb,
						OUT		icf_rgm_context_st	**p_p_rgn_cntxt_blk,
						OUT		icf_error_t			*p_ecode);

/*******************************************************************************
 *
 * FUNCTION:        icf_cfg_rgm_deregister_user_on_deletion
 *
 * DESCRIPTION:     This is a utility function that is trigerred from cfg if a user/registrar
 *					is deleted from a line and this is the last user on this line
 *                  
 *
 * RETURNS:         icf_return_t 
 *
 ******************************************************************************/
icf_return_t icf_cfg_rgm_deregister_user_on_deletion(
						INOUT	icf_crm_pdb_st		*p_crm_pdb,
						OUT		icf_rgm_context_st	*p_rgm_context);


icf_return_t icf_rgm_clear_buffered_requests_for_user(icf_crm_pdb_st  *p_crm_pdb,
                                                      icf_address_st  *p_user_addr,
                                                      icf_error_t     *p_ecode);

#ifdef ICF_NAT_RPORT_SUPPORT
/****************************************************************************
 * FUNCTION:            icf_rgm_01_03_nat_binding_chng
 *
 * DESCRIPTION:         This function is invoked when NAT binding has changed in
 *                      Registering/Registered State
 *
 *
 * RETURNS:             
 *
 ***************************************************************************/
icf_return_t icf_rgm_01_03_nat_binding_chng(
        INOUT icf_crm_pdb_st *p_crm_pdb);


/****************************************************************************
 * FUNCTION:            icf_rgm_05_nat_binding_chng
 *
 * DESCRIPTION:         This function is invoked when NAT binding has changed in
 *                      Deregistering State. This is invoked when response to 
 *                      De-register request sent on NAT binding change has been 
 *                      received. Also update the contact address in the global
 *                      config data.
 *
 *
 * RETURNS:             ICF_SUCCESS/ICF_FAILURE
 *
 ***************************************************************************/
icf_return_t icf_rgm_05_nat_binding_chng( 
        INOUT icf_crm_pdb_st *p_crm_pdb);

/****************************************************************************
 * FUNCTION:            icf_rgm_04_nat_binding_chng
 *
 * DESCRIPTION:         This function is invoked when NAT binding has changed in
 *                      Authorizing State. This is invoked when response to 
 *                      register request sent on NAT binding change has been 
 *                      received.
 *
 *
 * RETURNS:             ICF_SUCCESS/ICF_FAILURE
 *
 ***************************************************************************/
icf_return_t icf_rgm_04_nat_binding_chng( 
        INOUT icf_crm_pdb_st *p_crm_pdb);

/*******************************************************************************
 *
 * FUNCTION:        icf_rgm_update_nat_binding_refresh_blk
 *
 * DESCRIPTION:     This is a utility function that is trigerred to update the 
 *                  binding refresh timer data in the global config data.
 *                  this will get the next active registration context block and
 *                  the new binding refresh timer will be started on this newly
 *                  obtained active registration block.
 *
 * RETURNS:         icf_return_t 
 *
 ******************************************************************************/

icf_return_t  icf_rgm_update_nat_binding_refresh_blk(
                INOUT    icf_glb_pdb_st        *p_glb_pdb, 
                INOUT    icf_crm_pdb_st        *p_crm_pdb, 
                OUT      icf_error_t           *p_ecode);

icf_void_t icf_rgm_set_rport_status( 
                INOUT    icf_crm_pdb_st        *p_crm_pdb) ;

icf_return_t icf_rgm_04_nat_binding_chng(
        INOUT icf_crm_pdb_st *p_crm_pdb);

#endif

/***************************************************************************
 *
 * FUNCTION:        icf_rgm_fetch_ctxt_wrt_uri_wrt_line
 * 
 * DESCRIPTION:     This function returns pointer to regn context for  
 *                  a user (uri) on a given line. In case no regn context
 *                  exists then ICF_FAILURE is returned.
 *                   
 * RETURNS:         ICF_SUCCESS: In case regm ctxt is found.       
 *                  ICF_FAILURE: In case no regm_ctxt is found.
 **************************************************************************/

icf_return_t icf_rgm_fetch_ctxt_wrt_uri_wrt_line(
        INOUT icf_glb_pdb_st         *p_glb_pdb,
        IN    icf_address_st         *p_rgm_uri,
        OUT   icf_rgm_context_st     **p_p_rgn_cntxt_blk,
        IN    icf_line_id_t          line_id, 
        OUT   icf_error_t            *p_ecode);



/***************************************************************************
 *
 * FUNCTION:        icf_rgm_aka_auth_resp_hdlr
 * 
 * DESCRIPTION:     This function finds the reg context for which the 
 *                  authentication response has been received and calls 
 *                  the fsm handler.
 *                   
 * RETURNS:         ICF_SUCCESS or ICF_FAILURE
 **************************************************************************/
icf_return_t icf_rgm_aka_auth_resp_hdlr(
		INOUT	 icf_crm_pdb_st      	    *p_crm_pdb);


/***************************************************************************
 *
 * FUNCTION:        icf_rgm_print_parsed_notify_body
 * 
 * DESCRIPTION:     This function prints the parsed notify body .
 *                   
 * RETURNS:         
 **************************************************************************/

icf_void_t icf_rgm_print_parsed_notify_body(
                                    IN icf_rgm_reginfo_st	*p_reg_info);

/****************************************************************************
 * FUNCTION:            icf_rgm_form_and_send_dereg_resp
 * DESCRIPTION:         this function forms and send success dereg resp to 
 *                      APP
 * RETURNS:             nothing since it is a utility function and we are
 *                      clearing the context after this
 ***************************************************************************/

icf_return_t icf_rgm_form_and_send_dereg_resp
(
        INOUT icf_crm_pdb_st *p_crm_pdb,
        INOUT icf_rgm_context_st *p_rgm_context
);

/****************************************************************************
 * FUNCTION:            icf_rgm_is_valid_timer_blk
 * DESCRIPTION:         This function validates the timer expiry event received. 
 * RETURNS:             ICF_SUCCESS if timer expiry is valid, ICF_FAILURE otherwise.
 ***************************************************************************/
icf_return_t icf_rgm_is_valid_timer_blk(
        IN icf_crm_pdb_st *p_crm_pdb,
        IN icf_void_t     *p_buff);

/*Fix for SPR : 19188 (Rel 7.1.2 SPR merging for SPR 19151)*/
#ifdef ICF_NAT_RPORT_SUPPORT
/**************************************************************************************
 *   FUNCTION:            icf_rgm_is_valid_nat_binding_timer_blk 
 *   DESCRIPTION:         This function validates the timer expiry event  received. 
 *   RETURNS:             ICF_SUCCESS if timer expiry is valid, ICF_FAILURE  otherwise.
 *   
 **************************************************************************************/
icf_return_t icf_rgm_is_valid_nat_binding_timer_blk(
        IN icf_crm_pdb_st *p_crm_pdb,
        IN icf_void_t     *p_buff);
#endif

/*************************************************************************
 * FUNCTION:            icf_rgm_process_security_association
 * DESCRIPTION:         This function performs the handling required for
 *                      security association.
 *
 * RETURN:              This function always returns ICF_SUCCESS.
 ***************************************************************************/

icf_return_t icf_rgm_process_security_association(
                    INOUT icf_crm_pdb_st     *p_crm_pdb,
                    IN    icf_boolean_t       security_association_ind);


/****************************************************************************
 * FUNCTION:            icf_rgm_update_ctx_on_register_req
 * DESCRIPTION:         This function updates register context on receiving a
 *                      register request from application. 
 * RETURNS:             ICF_SUCCESS if all operations are successful, 
 *                      ICF_FAILURE otherwise.
 ***************************************************************************/
icf_return_t icf_rgm_update_ctx_on_register_req(
        INOUT icf_crm_pdb_st *p_crm_pdb);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __ICF_REGM_PROTOTYPES_H__ */
