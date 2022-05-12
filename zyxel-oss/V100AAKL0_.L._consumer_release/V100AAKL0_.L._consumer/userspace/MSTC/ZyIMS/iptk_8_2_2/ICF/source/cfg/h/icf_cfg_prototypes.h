/*------------------------------------------------------------------------------
 *
 * File name        : icf_cfg_prototypes.h
 *
 * Purpose          : This file contains prototypes of functions used in CFG.
 *
 * Revision History :
 *
 * Date         Name            	Ref#        Comments
 * --------     ------------    	----        ---------------
 * 21-Jan-2005  Shefali Aggarwal	ICF LLD	Beginning of coding phase
 * 02-Feb-2006  Amit Sharma                     CFG merged with ICF 2.0
 *
 * 18-Dec-2006	Deepti Goyal		Rel 6.2		NAT Binding Change Function
 *												Prototype
 * 15-Dec-2008  Rajiv Kumar         ICF 8.0     Registration Procedure
 *                                              Enhancements 
 * 17-Jan-2006  Shruti Thakral                  Enhancements for Rel 8.0
 * 8-Apr-2008   Sumant Gupta        ICF 8.0     Rel7.1 Windows porting Merging
 * 26-Jun-2008  Tarun Gupta         SPR 18616   Added function prototype
 * 25-May-2009  Kamal Ashraf        SPR 19672   SPR 19405 Merged added function 
 *                                              prototype 
 * 27-Jul-2009  Tarun Gupta         SPR 19886   CSR_1-7009954 merged
 * 02-Aug-2009  Rajiv Kumar         SPR 19886   Merged CSR-1-7423906  
 * Copyright (c) 2009, Aricent.
 *----------------------------------------------------------------------------*/


#ifndef __ICF_CFG_PROTOTYPES_H__
#define __ICF_CFG_PROTOTYPES_H__


#include "icf_cfg_internal_types.h"
#include "icf_nm_types.h"

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


/*------------------------------------------------------------------------------
 * Prototypes of functions in icf_cfg_init.c
 *----------------------------------------------------------------------------*/
/* The prototypes of these functions are in common/h file. */

/*------------------------------------------------------------------------------
 * Prototypes of functions in icf_cfg_entry_pt.c
 *----------------------------------------------------------------------------*/

/*******************************************************************************
 *
 * FUNCTION:        icf_cfg_process_mesg
 *
 * DESCRIPTION:     This is the entry point function for Configuration and
 *                  Registration handling modules.
 *
 * RETURNS:         ICF_SUCCESS in case of success and ICF_FAILURE in
 *                  case of failure.
 *
 ******************************************************************************/
icf_return_t icf_cfg_process_mesg(
        INOUT   icf_internal_msg_st      *p_internal_msg);

/*******************************************************************************
 *
 * FUNCTION:        icf_cfg_msg_handler
 *
 * DESCRIPTION:     This function checks the API received in a message from the
 *                  external Configuration Module and invokes appropriate
 *                  handler.
 *
 * RETURNS:         ICF_SUCCESS in case of success and ICF_FAILURE in
 *                  case of failure.
 *
 ******************************************************************************/
icf_return_t icf_cfg_msg_handler(
        INOUT   icf_crm_pdb_st           *p_crm_pdb);


/*------------------------------------------------------------------------------
 * Prototypes of functions in icf_cfg_api_handlers.c
 *----------------------------------------------------------------------------*/

/*******************************************************************************
 *
 * FUNCTION:        icf_cfg_set_scale_prms_hdlr
 *
 * DESCRIPTION:     This function is called when the API
 *                  ICF_CFG_CC_SET_SCALE_PARAMS_REQ is received.
 *                  It handles the scalable parameters like max_calls,
 *                  max_lines etc supported by the phone.IPPhone toolkit
 *                  scales its internal data structures based on these values.
 *
 * RETURNS:         ICF_SUCESS in case of success and ICF_FAILURE in case
 *                  invalid parameters are received, or memget fails, or DBM
 *                  fails to do initialization, or module's global data cannot
 *                  be fetched.
 *
 ******************************************************************************/
icf_return_t icf_cfg_set_scale_prms_hdlr(
        INOUT   icf_crm_pdb_st           *p_crm_pdb);

/*******************************************************************************
 *
 * FUNCTION:        icf_cfg_set_trnspt_prms_hdlr
 *
 * DESCRIPTION:     This function is called when the API
 *                  ICF_CFG_CC_SET_TRANSPORT_PARAMS_REQ is received.
 *                  This function handles transport related parameters like
 *                  proxy address, self IP address, self port etc.
 *
 * RETURNS:         ICF_SUCCESS in case of success and ICF_FAILURE in
 *                  case module's global data cannot be fetched.
 *
 ******************************************************************************/
icf_return_t icf_cfg_set_trnspt_prms_hdlr(
        INOUT   icf_crm_pdb_st           *p_crm_pdb);

/*******************************************************************************
 *
 * FUNCTION:        icf_cfg_set_call_prms_hdlr
 *
 * DESCRIPTION:     This function is called when the API
 *                  ICF_CFG_CC_SET_CALL_PARAMS_REQ is received.
 *                  This function handles call related parameters like timer
 *                  values and certain default call settings.
 *
 * RETURNS:         ICF_SUCCESS in case of success and ICF_FAILURE in
 *                  case module's global data cannot be fetched.
 *
 ******************************************************************************/
icf_return_t icf_cfg_set_call_prms_hdlr(
        INOUT   icf_crm_pdb_st           *p_crm_pdb);

/*******************************************************************************
 *
 * FUNCTION NAME:   icf_cfg_set_reg_prms_hdlr
 *
 * DESCRIPTION:     This function is called when the API
 *                  ICF_CFG_CC_SET_REGISTRATION_PARAMS_REQ is received.
 *
 * RETURNS:         ICF_SUCESS in case of success and ICF_FAILURE in case
 *                  of failure.
 *
 ******************************************************************************/
icf_return_t icf_cfg_set_reg_prms_hdlr(
        INOUT   icf_crm_pdb_st           *p_crm_pdb);

/*******************************************************************************
 *
 * FUNCTION NAME:   icf_cfg_set_self_id_prms_hdlr
 *
 * DESCRIPTION:     This function is called when the API
 *                  ICF_CFG_CC_SET_SELF_ID_PARAMS_REQ is received.
 *
 * RETURNS:         ICF_SUCCESS in case of success and ICF_FAILURE in
 *                  case module's global data cannot be fetched.
 *
 ******************************************************************************/
icf_return_t icf_cfg_set_self_id_prms_hdlr(
        INOUT   icf_crm_pdb_st           *p_crm_pdb);

/*******************************************************************************
 *
 * FUNCTION NAME:   icf_cfg_modify_reg_prms_in_db
 *
 * DESCRIPTION:     This function is called when the API
 *                  ICF_CFG_CC_SET_REGISTRATION_PARAMS_REQ is received
 *                  after init complete.It calls function to validate the
 *                  data received, sets the data in the line data and
 *                  triggers REGM to do the corrsponding action.
 *
 ******************************************************************************/
icf_return_t icf_cfg_modify_reg_prms_in_db(
        INOUT   icf_crm_pdb_st           *p_crm_pdb);

#ifdef ICF_SERVICE_ENABLE
/*******************************************************************************
 *
 * FUNCTION:        icf_cfg_set_srvc_prms_hdlr
 *
 * DESCRIPTION:     This function is called when the API
 *                  ICF_CFG_CC_SET_SERVICE_PARAMS_REQ is received.
 *                  This function handles service related parameters like
 *                  enable/disable service, activate/deactivate service and
 *                  service related timers.
 *
 * RETURNS:         ICF_SUCCESS in case of success and ICF_FAILURE in
 *                  case module's global data cannot be fetched.
 *
 ******************************************************************************/
icf_return_t icf_cfg_set_srvc_prms_hdlr(
        INOUT   icf_crm_pdb_st           *p_crm_pdb);

#endif 
/*******************************************************************************
 *
 * FUNCTION:        icf_cfg_init_complete_hdlr
 *
 * DESCRIPTION:     This function is called when the API
 *                  ICF_CFG_CC_INIT_COMPLETE_IND is received.
 *                  This function checks that all the mandatory parameters have
 *                  been received. If yes, it opens network sockets, sends
 *                  registration requests and makes toolkit ready to handle
 *                  calls.
 *
 * RETURNS:         ICF_SUCCESS in case of success and ICF_FAILURE in
 *                  case module's global data cannot be fetched, or if REGM
 *                  returns failure.
 *
 ******************************************************************************/
icf_return_t icf_cfg_init_complete_hdlr(
        INOUT   icf_crm_pdb_st           *p_crm_pdb);

/*******************************************************************************
 *
 * FUNCTION:        icf_cfg_restart_req_hdlr
 *
 * DESCRIPTION:     This function will send an indication to CM to clear all
 *                  calls and deallocate all call related buffers.
 *
 * RETURNS:         ICF_SUCCESS in case of success and ICF_FAILURE in
 *                  case of failure.
 *
 ******************************************************************************/
icf_return_t icf_cfg_restart_req_hdlr(
        INOUT   icf_crm_pdb_st           *p_crm_pdb);


/*******************************************************************************
 *
 * FUNCTION:        icf_cfg_delete_config_hdlr
 *
 * DESCRIPTION:     This function will invoke a function of RGM to de-register 
 *                  all the users.
 *                  
 * RETURNS:         ICF_SUCCESS in case of success and ICF_FAILURE in
 *                  case of failure.
 *
 ******************************************************************************/
icf_return_t icf_cfg_delete_config_hdlr(
        INOUT   icf_crm_pdb_st           *p_crm_pdb);

/*******************************************************************************
 *
 * FUNCTION:       icf_cfg_restart_reinit_rsp 
 *
 * DESCRIPTION:     This function will send an response to application after 
 *                  processing restart request sent by application
 *
 * RETURNS:         ICF_SUCCESS in case of success and ICF_FAILURE in
 *                  case of failure.
 *
 ******************************************************************************/

icf_return_t icf_cfg_restart_reinit_rsp(
        INOUT   icf_crm_pdb_st *p_crm_pdb,
        IN      icf_void_st*    p_routing_info,
        IN      icf_app_id_t    app_id,
        IN      icf_api_id_t    api_id);
/*******************************************************************************
 *
 * FUNCTION:       icf_cfg_restart_reinit_ind 
 *
 * DESCRIPTION:     This function will send an indication to other application after 
 *                  processing restart request sent by one application
 *
 * RETURNS:         ICF_SUCCESS in case of success and ICF_FAILURE in
 *                  case of failure.
 *
 ******************************************************************************/
icf_return_t icf_cfg_restart_reinit_ind(
        INOUT   icf_crm_pdb_st *p_crm_pdb,
         IN     icf_void_st*    p_routing_info,
        IN      icf_app_id_t    app_id,
        IN      icf_api_id_t    api_id);



/*******************************************************************************
 *
 * FUNCTION NAME:   icf_cfg_config_prms_resp
 *
 * DESCRIPTION:     This function sends response to the external Configuration
 *                  Module for a request message.
 *
 * RETURNS:         ICF_SUCESS in case of success and ICF_FAILURE in case
 *                  of failure.
 *
 ******************************************************************************/
icf_return_t icf_cfg_config_prms_resp(
        INOUT   icf_crm_pdb_st           *p_crm_pdb);


/*------------------------------------------------------------------------------
 * Prototypes of functions in icf_cfg_util.c
 *----------------------------------------------------------------------------*/

/*******************************************************************************
 *
 * FUNCTION:        icf_cfg_error_cause_to_str
 *
 * DESCRIPTION:     This function returns a string corresponding to an 
 *                  error cause and is used only for traces.
 *
 ******************************************************************************/
#ifdef ICF_TRACE_ENABLE
icf_void_t icf_cfg_error_cause_to_str(
        IN icf_error_t       error,
        OUT icf_uint8_t      *p_error_str);
#endif
    
/*******************************************************************************
 *
 * FUNCTION:        icf_cfg_trace
 *
 * DESCRIPTION:     This function checks the trace id and calls the macro for
 *                  trace.
 *
 * RETURNS:         ICF_SUCCESS in case of success and ICF_FAILURE in
 *                  case of failure.
 *
 ******************************************************************************/
#ifdef ICF_TRACE_ENABLE
icf_void_t icf_cfg_trace(
        INOUT   icf_crm_pdb_st           *p_crm_pdb,
        IN      icf_trace_id_t           trace_id);

/*******************************************************************************
 *
 * FUNCTION:        icf_cfg_app_trace
 *
 * DESCRIPTION:     This function checks the trace id and calls the macro for
 *                  trace.
 *
 * RETURNS:         ICF_SUCCESS in case of success and ICF_FAILURE in
 *                  case of failure.
 *
 ******************************************************************************/
icf_void_t icf_cfg_app_trace(
        INOUT   icf_crm_pdb_st           *p_crm_pdb,
        IN      icf_trace_id_t           trace_id);

#endif

/*******************************************************************************
 *
 * FUNCTION:        icf_cfg_error
 *
 * DESCRIPTION:     This function checks the error id and calls the macro for
 *                  error reporting.
 *
 * RETURNS:         ICF_SUCCESS in case of success and ICF_FAILURE in
 *                  case of failure.
 *
 ******************************************************************************/
#ifdef ICF_ERROR_ENABLE
icf_void_t icf_cfg_error(
        INOUT   icf_crm_pdb_st           *p_crm_pdb,
        IN      icf_error_t              ecode);
#endif

/*******************************************************************************
 *
 * FUNCTION:        icf_cfg_init_line_blk
 * 
 * DESCRIPTION:     This function sets default values in a newly created line 
 *                  block.
 *                  
 * RETURNS:        ICF_SUCCESS in case of success and ICF_FAILURE in
 *                  case of failure. 
 * 
 *****************************************************************************/
icf_return_t icf_cfg_init_line_blk(
        INOUT   icf_crm_pdb_st           *p_crm_pdb,
        IN      icf_uint8_t              line_status,
        IN      icf_line_id_t            line_id);

/*******************************************************************************
 *
 * FUNCTION:        icf_cfg_validate_scale_prms
 *
 * DESCRIPTION:     This function validates the params recieved in the set
 *                  scale params API.
 *
 * RETURNS:         ICF_SUCCESS always
 *
 *****************************************************************************/
icf_return_t icf_cfg_validate_scale_prms(
        INOUT   icf_crm_pdb_st           *p_crm_pdb);

/*******************************************************************************
 *
 * FUNCTION:        icf_cfg_validate_trnspt_prms
 *
 * DESCRIPTION:     This function will validate the tranport parmeters.
 *
 * RETURNS:         ICF_SUCCESS always
 *
 ******************************************************************************/
icf_return_t icf_cfg_validate_trnspt_prms(
        INOUT   icf_crm_pdb_st           *p_crm_pdb);

/*******************************************************************************
 *
 * FUNCTION:        icf_cfg_update_self_trnspt_in_db
 * 
 * DESCRIPTION:     This function will update the self_transport data received 
 *                  in Set Transport params message into global database.
 *                  
 * RETURNS:         ICF_SUCCESS.
 *
 ******************************************************************************/

icf_return_t icf_cfg_update_self_trnspt_in_db(
        INOUT icf_crm_pdb_st                         *p_crm_pdb,
        IN    icf_transport_addr_st                  *p_transport_addr);

/*******************************************************************************
 *
 * FUNCTION:        icf_cfg_set_trnspt_prms_in_db
 *
 * DESCRIPTION:     This function will add the data received in Set Transport
 *                  params message into global database.
 *
 * RETURNS:         ICF_SUCCESS in case of sucess and ICF_FAILURE
 *                  in case settting of proxy data returns failure.
 *
 ******************************************************************************/
icf_return_t icf_cfg_set_trnspt_prms_in_db(
        INOUT   icf_crm_pdb_st           *p_crm_pdb);

/*******************************************************************************
 *
 * FUNCTION NAME:   icf_cfg_add_proxy_prms_in_db
 * 
 * DESCRIPTION:     This function adds the proxy address received in 
 *                  the API ICF_CFG_CC_SET_TRANSPORT_PARAMS_REQ 
 *
 * RETURNS:         ICF_SUCCESS always
 *
 *****************************************************************************/
icf_return_t icf_cfg_add_proxy_prms_in_db(
        INOUT   icf_crm_pdb_st   *p_crm_pdb,        
        IN      icf_line_id_t    line_id);

/*******************************************************************************
 *
 * FUNCTION NAME:   icf_cfg_add_sec_proxy_prms_in_db
 * 
 * DESCRIPTION:     This function adds the secopndary proxy address received in 
 *                  the API ICF_CFG_CC_SET_TRANSPORT_PARAMS_REQ 
 *
 * RETURNS:         ICF_SUCCESS always
 *
 *****************************************************************************/
icf_return_t icf_cfg_add_sec_proxy_prms_in_db(
        INOUT   icf_crm_pdb_st   *p_crm_pdb,
        IN      icf_line_id_t     line_id);

/*******************************************************************************
 *
 * FUNCTION NAME:   icf_cfg_del_proxy_prms_in_db
 * 
 * DESCRIPTION:     This function deletes the proxy address received in 
 *                  the API ICF_CFG_CC_SET_TRANSPORT_PARAMS_REQ 
 *
 * RETURNS:         ICF_SUCCESS always
 *
 ******************************************************************************/
icf_return_t icf_cfg_del_proxy_prms_in_db(
        INOUT   icf_crm_pdb_st   *p_crm_pdb,
        IN      icf_line_id_t    line_id);

/*******************************************************************************
 *
 * FUNCTION NAME:   icf_cfg_del_sec_proxy_prms_in_db
 * 
 * DESCRIPTION:     This function deletes secondary proxy address received in 
 *                  the API ICF_CFG_CC_SET_TRANSPORT_PARAMS_REQ 
 *
 * RETURNS:         ICF_SUCCESS always
 *
 ******************************************************************************/
icf_return_t icf_cfg_del_sec_proxy_prms_in_db(
        INOUT   icf_crm_pdb_st   *p_crm_pdb,
        IN      icf_line_id_t    line_id);

/*******************************************************************************
 *
 * FUNCTION:        icf_cfg_set_trnspt_proxy_addr
 *
 * DESCRIPTION:     This function will add the proxy address received in Set
 *                  Transport params message into global database.
 *
 * RETURNS:         ICF_SUCCESS in case of sucess and ICF_FAILURE
 *                  in case of failure.
 *
 ******************************************************************************/
icf_return_t icf_cfg_set_trnspt_proxy_addr(
        INOUT   icf_crm_pdb_st           *p_crm_pdb);

/*******************************************************************************
 *
 * FUNCTION:        icf_cfg_set_trnspt_sec_proxy_addr
 *
 * DESCRIPTION:     This function will add the secondary proxy address 
 *                  rcvd in Set Transport params message into global database.
 *
 * RETURNS:         ICF_SUCCESS in case of sucess and ICF_FAILURE
 *                  in case of failure.
 *
 ******************************************************************************/
icf_return_t icf_cfg_set_trnspt_sec_proxy_addr(
        INOUT   icf_crm_pdb_st           *p_crm_pdb);

/*******************************************************************************
 *
 * FUNCTION:        icf_cfg_validate_sec_proxy_addr
 * 
 * DESCRIPTION:     This function will validate the secondary proxy address 
 *                  and the action to be performed on it.
 *                  
 * RETURNS:         ICF_SUCCESS.
 *
 ******************************************************************************/
icf_return_t icf_cfg_validate_sec_proxy_addr(
        INOUT   icf_crm_pdb_st          *p_crm_pdb);

/*******************************************************************************
 *
 * FUNCTION:        icf_cfg_validate_call_prms
 *
 * DESCRIPTION:     This function validates the params recieved in the set
 *                  call params API.
 *
 * RETURNS:         ICF_SUCCESS always
 *
 *****************************************************************************/
icf_return_t icf_cfg_validate_call_prms(
        INOUT   icf_crm_pdb_st           *p_crm_pdb);

/*******************************************************************************
 *
 * FUNCTION:        icf_cfg_set_call_prms_in_db
 *
 * DESCRIPTION:     This function will add the data received in Set Call params
 *                  message into global database.
 *
 * RETURNS:         ICF_SUCCESS always
 *
 ******************************************************************************/
icf_return_t icf_cfg_set_call_prms_in_db(
        INOUT   icf_crm_pdb_st           *p_crm_pdb);

/*******************************************************************************
 *
 * FUNCTION:        icf_cfg_validate_self_id_prms
 *
 * DESCRIPTION:     This function will validate the self id parameters in the 
 *                  API.
 *
 ******************************************************************************/
icf_return_t icf_cfg_validate_self_id_prms(
        INOUT   icf_crm_pdb_st           *p_crm_pdb);

/*******************************************************************************
 *
 * FUNCTION:        icf_cfg_compare_user_addr
 *
 * DESCRIPTION:     This function compares the two user addresses whose 
 *                  pointers are passed to it. This function will return
 *                  success in case the addresses are same and failure if 
 *                  the addresses are different.
 *                  
 ******************************************************************************/
icf_return_t icf_cfg_compare_user_addr(
        INOUT icf_crm_pdb_st     *p_crm_pdb,
        IN    icf_address_st     *p_addr_a,
        IN    icf_address_st     *p_addr_b);

/*******************************************************************************
 *
 * FUNCTION:        icf_cfg_validate_del_self_id_prms
 *
 * DESCRIPTION:     This function will validate the delete action in the self 
 *                  id prms API. It is called by the function to validate the
 *                  prms of this API when ever add action in found in the
 *                  prms received.
 *                  
 ******************************************************************************/
icf_return_t icf_cfg_validate_del_self_id_prms(
        INOUT icf_crm_pdb_st     *p_crm_pdb,
        IN    icf_uint8_t        api_index);

/*******************************************************************************
 *
 * FUNCTION:        icf_cfg_validate_add_self_id_prms
 *
 * DESCRIPTION:     This function will validate the add action in the self id 
 *                  prms API. It is called by the function to validate the
 *                  prms of this API when ever add action in found in the
 *                  prms received.
 *                  
 ******************************************************************************/
icf_return_t icf_cfg_validate_add_self_id_prms(
        INOUT   icf_crm_pdb_st   *p_crm_pdb,        
        IN      icf_line_id_t    line_id);

/*******************************************************************************
 *
 * FUNCTION:        icf_cfg_del_user_in_db
 *
 * DESCRIPTION:     This function will delete the user from the specified
 *                  line id.
 *
 ******************************************************************************/
icf_return_t icf_cfg_del_user_in_db(
        INOUT icf_crm_pdb_st     *p_crm_pdb,        
        IN    icf_line_id_t      line_id);

/*******************************************************************************
 *
 * FUNCTION:        icf_cfg_add_user_in_db
 *
 * DESCRIPTION:     This function will add the user on the specified
 *                  line id.
 *
 ******************************************************************************/
icf_return_t icf_cfg_add_user_in_db(
        INOUT icf_crm_pdb_st     *p_crm_pdb,        
        IN    icf_line_id_t      line_id);

/*******************************************************************************
 *
 * FUNCTION:        icf_cfg_set_self_id_prms_in_db
 *
 * DESCRIPTION:     This function will save the self id parameters in the 
 *                  global databse.
 *
 ******************************************************************************/
icf_return_t icf_cfg_set_self_id_prms_in_db(
        INOUT   icf_crm_pdb_st           *p_crm_pdb);

/*******************************************************************************
 *
 * FUNCTION:        icf_cfg_validate_add_regm_prms
 *
 * DESCRIPTION:     This function validates the ADD action of the data 
 *                  received in the Set reg params API. 
 *                  
 * RETURNS:         ICF_SUCCESS always
 *                  
******************************************************************************/
icf_return_t icf_cfg_validate_add_regm_prms(
        INOUT   icf_crm_pdb_st   *p_crm_pdb,
        /*IN      icf_uint8_t      api_index,*/
        IN      icf_line_id_t    line_id/*,
        IN      icf_boolean_t    repeat_req*/);
    
/*******************************************************************************
 *
 * FUNCTION:        icf_cfg_validate_add_sec_regm_prms
 *
 * DESCRIPTION:     This function validates the ADD action for secondary 
 *                  registrar in the Set reg params API. 
 *                  
 * RETURNS:         ICF_SUCCESS always
 *                  
******************************************************************************/
icf_return_t icf_cfg_validate_add_sec_regm_prms(
        INOUT   icf_crm_pdb_st   *p_crm_pdb,
        IN      icf_line_id_t    line_id);
    
/*******************************************************************************
 *
 * FUNCTION:        icf_cfg_validate_regm_prms
 *
 * DESCRIPTION:     This function validates the data received in the Set reg 
 *                  params API. 
 *                  
 * RETURNS:         ICF_SUCCESS always
 *                  
******************************************************************************/
icf_return_t icf_cfg_validate_regm_prms(
        INOUT   icf_crm_pdb_st           *p_crm_pdb);


/*******************************************************************************
 *
 * FUNCTION NAME:   icf_cfg_validate_del_regm_prms
 * 
 * DESCRIPTION:     This function validates the registrar address to be deleted received in 
 *                  the API ICF_CFG_CC_SET_REGISTRATION_PARAMS_REQ 
 *
 * RETURNS:         ICF_SUCCESS always
 *
 ******************************************************************************/
icf_return_t icf_cfg_validate_del_regm_prms(
        INOUT   icf_crm_pdb_st   *p_crm_pdb,
        IN      icf_line_id_t    line_id);

/*******************************************************************************
 *
 * FUNCTION NAME:   icf_cfg_validate_del_sec_regm_prms
 * 
 * DESCRIPTION:     This function validates secondary registrar address to be 
 *                  deleted received in the API 
 *                  ICF_CFG_CC_SET_REGISTRATION_PARAMS_REQ 
 *
 * RETURNS:         ICF_SUCCESS always
 *
 ******************************************************************************/
icf_return_t icf_cfg_validate_del_sec_regm_prms(
        INOUT   icf_crm_pdb_st   *p_crm_pdb,
        IN      icf_line_id_t    line_id);

/*******************************************************************************
 *
 * FUNCTION NAME:   icf_cfg_del_regm_prms_in_db
 * 
 * DESCRIPTION:     This function deletes the registrar address received in 
 *                  the API ICF_CFG_CC_SET_REGISTRATION_PARAMS_REQ 
 *
 * RETURNS:         ICF_SUCCESS always
 *
 ******************************************************************************/
icf_return_t icf_cfg_del_regm_prms_in_db(
        INOUT   icf_crm_pdb_st   *p_crm_pdb,
        IN      icf_line_id_t    line_id);

/*******************************************************************************
 *
 * FUNCTION NAME:   icf_cfg_del_sec_regm_prms_in_db
 * 
 * DESCRIPTION:     This function deletes secondary registrar address received 
 *                  in the API ICF_CFG_CC_SET_REGISTRATION_PARAMS_REQ 
 *
 * RETURNS:         ICF_SUCCESS always
 *
 ******************************************************************************/
icf_return_t icf_cfg_del_sec_regm_prms_in_db(
        INOUT   icf_crm_pdb_st   *p_crm_pdb,
        IN      icf_line_id_t    line_id);

/*******************************************************************************
 *
 * FUNCTION NAME:   icf_cfg_update_regm_prms_in_db
 * 
 * DESCRIPTION:     This function modifies the registration data received in
 *                  the API ICF_CFG_CC_SET_REGISTRATION_PARAMS_REQ 
 *
 * RETURNS:         ICF_SUCCESS always
 *
 ******************************************************************************/
icf_return_t icf_cfg_update_regm_prms_in_db(
        INOUT   icf_crm_pdb_st   *p_crm_pdb,
        /*IN      icf_uint8_t      api_index,*/
        IN      icf_line_id_t    line_id);
 
 
/*******************************************************************************
 *
 * FUNCTION NAME:   icf_cfg_add_regm_prms_in_db
 * 
 * DESCRIPTION:     This function adds the registrar address received in 
 *                  the API ICF_CFG_CC_SET_REGISTRATION_PARAMS_REQ 
 *
 * RETURNS:         ICF_SUCCESS always
 *
 ******************************************************************************/
icf_return_t icf_cfg_add_regm_prms_in_db(
        INOUT   icf_crm_pdb_st   *p_crm_pdb,
        /*IN      icf_uint8_t      api_index,*/
        IN      icf_line_id_t    line_id);

/*******************************************************************************
 *
 * FUNCTION NAME:   icf_cfg_add_sec_regm_prms_in_db
 * 
 * DESCRIPTION:     This function adds secondary registrar address received in 
 *                  the API ICF_CFG_CC_SET_REGISTRATION_PARAMS_REQ 
 *
 * RETURNS:         ICF_SUCCESS always
 *
 ******************************************************************************/
icf_return_t icf_cfg_add_sec_regm_prms_in_db(
        INOUT   icf_crm_pdb_st   *p_crm_pdb,
        IN      icf_line_id_t    line_id);

/*******************************************************************************
 *
 * FUNCTION NAME:   icf_cfg_set_reg_prms_in_db
 * 
 * DESCRIPTION:     This function is called when the API 
 *                  ICF_CFG_CC_SET_REGISTRATION_PARAMS_REQ is received 
 *                  before init complete.It calls function to validate the 
 *                  data received and sets the data in the line data.
 *
 * RETURNS:         ICF_SUCCESS in case of sucess and ICF_FAILURE
 *                  in case of failure.
 *
 ******************************************************************************/
icf_return_t icf_cfg_set_reg_prms_in_db(
        INOUT   icf_crm_pdb_st           *p_crm_pdb);

/*******************************************************************************
 *
 * FUNCTION NAME:   icf_cfg_set_regm_sec_registrar_addr
 * 
 * DESCRIPTION:     This function will add secondary registrar address received
 *                  in ICF_CFG_CC_SET_REGISTRATION_PARAMS_REQ.
 *                  
 * RETURNS:         ICF_SUCCESS always
 *
 ******************************************************************************/
icf_return_t icf_cfg_set_regm_sec_registrar_addr(
        INOUT   icf_crm_pdb_st           *p_crm_pdb);

/*******************************************************************************
 *
 * FUNCTION NAME:   icf_cfg_modify_reg_prms_in_db
 * 
 * DESCRIPTION:     This function is called when the API 
 *                  ICF_CFG_CC_SET_REGISTRATION_PARAMS_REQ is received 
 *                  after init complete.It calls function to validate the 
 *                  data received, sets the data in the line data and
 *                  triggers REGM to do the corrsponding action.
 *
 ******************************************************************************/
icf_return_t icf_cfg_modify_reg_prms_in_db(
        INOUT   icf_crm_pdb_st           *p_crm_pdb);



#ifdef ICF_SERVICE_ENABLE
/*******************************************************************************
 *
 * FUNCTION:        icf_cfg_validate_srvc_prms
 *
 * DESCRIPTION:     This function validates the params recieved in the set
 *                  service params API.
 *
 * RETURNS:         ICF_SUCCESS always
 *
 *****************************************************************************/
icf_return_t icf_cfg_validate_srvc_prms(
        INOUT   icf_crm_pdb_st           *p_crm_pdb);

/*******************************************************************************
 *
 * FUNCTION:        icf_cfg_set_srvc_prms_in_db
 *
 * DESCRIPTION:     This function will add the data received in Set Service
 *                  params message into global database. First, it adds any
 *                  general config data received and then it adds any line
 *                  specific data received.
 *
 * RETURNS:         ICF_SUCCESS always
 *
 ******************************************************************************/
icf_return_t icf_cfg_set_srvc_prms_in_db(
        INOUT   icf_crm_pdb_st           *p_crm_pdb);


/*******************************************************************************
 *
 * FUNCTION:        icf_cfg_set_srvc_config_prm
 *
 * DESCRIPTION:     This function will save the service param related to
 *                  config data into global database.
 *
 * RETURNS:         ICF_SUCCESS always
 *
 ******************************************************************************/
icf_return_t icf_cfg_set_srvc_config_prm(
        INOUT   icf_crm_pdb_st           *p_crm_pdb);

/*******************************************************************************
 *
 * FUNCTION:        icf_cfg_ret_srvc_id
 *
 * DESCRIPTION:     This function will return the bit value of the service
 *
 * RETURNS:         icf_uint16_t
 *
 ******************************************************************************/
icf_uint16_t icf_cfg_ret_srvc_id(
        INOUT   icf_crm_pdb_st           *p_crm_pdb,
        IN      icf_uint16_t         	index,
        IN      icf_boolean_t            subscription_request);

/*******************************************************************************
 *
 * FUNCTION:        icf_cfg_validate_act_flag
 *
 * DESCRIPTION:     This function will check if the services that have been 
 *                  requested to be activated have valid data on the lines.
 *                  
 * RETURNS:         ICF_SUCCESS if validation is successful
 *                  ICF_FAILURE if validation fails.
 *
 ******************************************************************************/
icf_return_t icf_cfg_validate_act_flag(
         INOUT  icf_crm_pdb_st           *p_crm_pdb, 
         IN     icf_line_id_t             line_id);


/*******************************************************************************
 *
 * FUNCTION:        icf_cfg_check_subs_flag
 *
 * DESCRIPTION:     This function will check if service that has to be activated
 *                  has been subscribed or not.
 *                  
 * RETURNS:         ICF_SUCCESS always
 *
 ******************************************************************************/
icf_return_t icf_cfg_check_subs_flag(
         INOUT  icf_crm_pdb_st           *p_crm_pdb, 
         IN     icf_line_id_t            line_id, 
         IN     icf_uint16_t             service);

/*******************************************************************************
 *
 * FUNCTION:        icf_cfg_set_act_flag
 *
 * DESCRIPTION:     This function will set the activation flag after checking
 *                  subscription status and address for various services
 *                  
 * RETURNS:         ICF_SUCCESS if validation is successful
 *                  ICF_FAILURE if validation fails.
 *
 ******************************************************************************/
icf_return_t icf_cfg_set_act_flag(
         INOUT  icf_crm_pdb_st           *p_crm_pdb, 
         IN     icf_line_id_t            line_id);


/*******************************************************************************
 *
 * FUNCTION:        icf_cfg_set_srvc_line_prm
 *
 * DESCRIPTION:     This function will save the service param related to
 *                  line data into global database.
 *
 * RETURNS:         ICF_SUCCESS always
 *
 ******************************************************************************/
icf_return_t icf_cfg_set_srvc_line_prm(
         INOUT  icf_crm_pdb_st           *p_crm_pdb);

#endif

/*******************************************************************************
 *
 * FUNCTION:        icf_cfg_check_init_complete
 *
 * DESCRIPTION:     This function checks whether all the mandatory parameters
 *                  have been received once or not.
 *
 * RETURNS:         ICF_SUCCESS in case of sucess and ICF_FAILURE
 *                  in case module's global data cannot be fetched.
 *
 ******************************************************************************/
icf_return_t icf_cfg_check_init_complete(
        INOUT   icf_crm_pdb_st           *p_crm_pdb);

/*******************************************************************************
 *
 * FUNCTION:        icf_cfg_open_nw_interface
 *
 * DESCRIPTION:     This function triggers SSA to open network sockets.
 *                  
 * RETURNS:         ICF_SUCCESS in case of sucess and ICF_FAILURE
 *                  in case SSA fails to open network interface.
 *
 ******************************************************************************/
icf_return_t icf_cfg_open_nw_interface(
        INOUT   icf_crm_pdb_st           *p_crm_pdb,
        IN      icf_transport_mode_et      self_mode);

/*******************************************************************************
 *
 * FUNCTION:        icf_cfg_close_nw_interface
 *
 * DESCRIPTION:     This function triggers SSA to close network sockets.
 *                  
 * RETURNS:         ICF_SUCCESS in case of sucess and ICF_FAILURE
 *                  in case SSA fails to close network interface.
 *
 ******************************************************************************/
icf_return_t icf_cfg_close_nw_interface(
        INOUT   icf_crm_pdb_st           *p_crm_pdb);
/*******************************************************************************
 *
 * FUNCTION:        icf_cfg_cm_call_clear_ind
 *
 * DESCRIPTION:     This function sends forceful call clear to CM for all active
 *                  calls.
 *                  
 * RETURNS:         ICF_SUCCESS in case of sucess and ICF_FAILURE
 *                  in case of failure.
 *
 ******************************************************************************/
icf_return_t icf_cfg_cm_call_clear_ind(
        INOUT   icf_crm_pdb_st           *p_crm_pdb);
    
/*******************************************************************************
 *
 * FUNCTION:        icf_cfg_check_line_in_db
 *
 * DESCRIPTION:     It checks whether a line has been allocated. 
 *                  If not, it allocates it.
 *         
 *****************************************************************************/
icf_return_t icf_cfg_check_line_in_db(
        icf_crm_pdb_st             *p_crm_pdb,
        icf_line_id_t              line_id,
        icf_uint8_t                *p_line_status,
        icf_boolean_t              get_line);

/*******************************************************************************
 *
 * FUNCTION:        icf_cfg_check_line_configured
 *
 * DESCRIPTION:     It checks whether any one line has been configured.
 *                  If yes then takes the pointers of the lines configured 
 *                  till now else allocates memeory to all the lines and 
 *                  takes the pointers to all the lines in pdb.
 *         
 *****************************************************************************/
icf_return_t icf_cfg_check_line_configured(
        INOUT   icf_crm_pdb_st   *p_crm_pdb,
        IN      icf_boolean_t    configure_lines);
/*******************************************************************************
 *
 * FUNCTION:        icf_deregister_all
 *
 * DESCRIPTION:     It sends deregister request to REGM for all lines on which 
 *                  user and registrar have been configured.
 *         
 *****************************************************************************/
icf_return_t         icf_deregister_all(
        INOUT   icf_crm_pdb_st   *p_crm_pdb);

ICF_INIT_START
/*******************************************************************************
 *
 * FUNCTION:        icf_cfg_internal_init
 *
 * DESCRIPTION:     This function is called when the internal API 
 *                  ICF_CFG_INTERNAL_INIT is received. 
 *                  It handles the scalable parameters like max_calls,
 *                  max_lines etc supported by the phone.IPPhone toolkit
 *                  scales its internal data structures based on these values.
 *                   
 * RETURNS:         ICF_SUCESS in case of success and ICF_FAILURE in case
 *                  invalid parameters are received, or memget fails, or DBM 
 *                  fails to do initialization, or module's global data cannot 
 *                  be fetched.
 *
 ******************************************************************************/
icf_return_t icf_cfg_internal_init(
        INOUT   icf_crm_pdb_st           *p_crm_pdb);
ICF_INIT_END

/****************************************************************************
 * FUNCTION:        icf_cfg_nm_get_proxy_addr
 *
 * DESCRIPTION:     This function requests proxy addr from NM 
 *
 * RETURNS:         ICF_TRUE / ICF_FALSE           
 *
 ***************************************************************************/
icf_return_t icf_cfg_nm_get_proxy_addr(
		INOUT   icf_crm_pdb_st *p_crm_pdb,
		INOUT   icf_internal_msg_st *p_internal_msg);

#ifndef ICF_NW_ACTIVATE
/******************************************************************************
 *
 * FUNCTION:        icf_cfg_get_nw_info
 *
 * DESCRIPTION:     
 *                   
 * RETURNS:         
 *
 *****************************************************************************/
icf_return_t icf_cfg_get_nw_info(
        INOUT   icf_crm_pdb_st           *p_crm_pdb);
#endif
#ifdef ICF_NW_ACTIVATE
ICF_CRM_START
/****************************************************************************
 * FUNCTION:        icf_cfg_process_deactivate
 *
 * DESCRIPTION:     This function is called by configuration module when
 *                  it receives network deactivation request from REGM.                   It carries out the necessary processing to deactivate 
 *
 * RETURNS:             
 *
 ***************************************************************************/
icf_return_t icf_cfg_process_deactivate_on_dereg(
        INOUT   icf_crm_pdb_st* p_crm_pdb);
#endif/*#ifdef ICF_NW_ACTIVATE*/


#ifdef ICF_NW_ACTIVATE
/****************************************************************************
 * FUNCTION:        icf_cfg_process_nw_event_ind
 *
 * DESCRIPTION:     This function is called by configuration module when
 *                  it receives a network status change indication from 
 *                  network due to some event. CFG module passes the event 
 *                  structure to this function. After that this function 
 *                  carries out the necessary processing.
 *
 * RETURNS:             
 *
 ***************************************************************************/
icf_return_t icf_cfg_process_nw_event_ind(
        INOUT   icf_crm_pdb_st* p_crm_pdb);

/****************************************************************************
 * FUNCTION:        icf_cfg_get_app_network
 *
 * DESCRIPTION:     This function gets the network associated with a given 
 *                  application
 *
 * RETURNS:             
 *
 ***************************************************************************/
icf_return_t icf_cfg_get_app_network(
        INOUT   icf_glb_pdb_st *p_glb_pdb,
        IN      icf_app_id_t app_id,
        OUT     icf_string_st *p_apn,
        OUT     icf_uint32_t *p_ctxt_id,
	    OUT     icf_error_t *p_ecode);

/****************************************************************************
 * FUNCTION:        icf_cfg_get_network_num_apps
 *
 * DESCRIPTION:     This function determines the number of valid entries in 
 *                  application context block associated with a network	
 *
 * RETURNS:             
 *
 ***************************************************************************/
icf_return_t icf_cfg_get_network_num_apps(
        INOUT   icf_crm_pdb_st *p_crm_pdb,
        IN      icf_string_st apn,
        OUT     icf_uint32_t *num_apps);

/****************************************************************************
 * FUNCTION:        icf_cfg_get_num_networks
 *
 * DESCRIPTION:     This function determines the number of valid entries in 
 *                  network context block
 *
 * RETURNS:             
 *
 ***************************************************************************/
icf_return_t icf_cfg_get_num_networks(
        INOUT   icf_crm_pdb_st *p_crm_pdb,
        OUT     icf_uint32_t *num_networks);

/****************************************************************************
 * FUNCTION:        icf_cfg_get_network_vacant_app_index
 *
 * DESCRIPTION:     This function determines vacant application context index
 *                  associated with a network	
 *
 * RETURNS:             
 *
 ***************************************************************************/
icf_return_t icf_cfg_get_network_vacant_app_index(
        INOUT   icf_crm_pdb_st *p_crm_pdb,
        IN      icf_string_st apn,
        OUT     icf_uint8_t *app_index);

/****************************************************************************
 * FUNCTION:        icf_cfg_get_vacant_network_index
 *
 * DESCRIPTION:     This function determines a vacant network context index
 *
 * RETURNS:             
 *
 ***************************************************************************/
icf_return_t icf_cfg_get_vacant_network_index(
        INOUT   icf_crm_pdb_st *p_crm_pdb,
        OUT     icf_uint8_t *network_index);

/****************************************************************************
 * FUNCTION:        icf_cfg_nm_network_activate
 *
 * DESCRIPTION:     This function requests network activation from NM 
 *
 * RETURNS:         ICF_TRUE if network gets activated
 *                  ICF_FALSE if network activation fails           
 *
 ***************************************************************************/
icf_return_t icf_cfg_nm_network_activate(
        INOUT   icf_crm_pdb_st *p_crm_pdb,
		INOUT   icf_internal_msg_st *p_internal_msg,
		IN		icf_cfg_net_act_retry_timer_st	*p_net_act_timer_buffer);

#endif

/****************************************************************************
 * FUNCTION:        icf_cfg_nm_get_nw_info
 *
 * DESCRIPTION:     This function requests network activation from NM 
 *
 * RETURNS:         ICF_TRUE if able to fetch network info
 *                  ICF_FALSE if unable to fetch network info        
 *
 ***************************************************************************/
icf_return_t icf_cfg_nm_get_nw_info(
        INOUT   icf_crm_pdb_st *p_crm_pdb,
		INOUT   icf_internal_msg_st *p_internal_msg,
		IN		icf_cfg_net_act_retry_timer_st	*p_net_act_timer_buffer);

/****************************************************************************
 * FUNCTION:        icf_cfg_nm_get_acs_info
 *
 * DESCRIPTION:     This function requests access information from NM 
 *
 * RETURNS:         ICF_TRUE if able to fetch network info
 *                  ICF_FALSE if unable to fetch network info        
 *
 ***************************************************************************/
icf_return_t icf_cfg_nm_get_acs_info(
        INOUT   icf_crm_pdb_st *p_crm_pdb,
		INOUT   icf_internal_msg_st *p_internal_msg);

#ifdef ICF_NW_ACTIVATE
/*******************************************************************************
 *
 * FUNCTION:        icf_cfg_network_activate_hdlr
 *
 * DESCRIPTION:     This function is called when the API
 *                  ICF_NETWORK_ACTIVATE_REQ is received.
 *                  This function triggers network activation with NM and 
 *                  handles network access context data.
 *
 * RETURNS:         ICF_SUCCESS in case of success and ICF_FAILURE in
 *                  case module's global data cannot be fetched.
 *
 ******************************************************************************/
icf_return_t icf_cfg_network_activate_hdlr(
        INOUT   icf_crm_pdb_st *p_crm_pdb);

/*******************************************************************************
 *
 * FUNCTION:        icf_cfg_validate_network_activate
 *
 * DESCRIPTION:     This function validates the params received in the network
 *                  activate API.
 *
 * RETURNS:         ICF_SUCCESS always
 *
 *****************************************************************************/
icf_return_t icf_cfg_validate_network_activate (
        INOUT   icf_crm_pdb_st *p_crm_pdb);

/*******************************************************************************
 *
 * FUNCTION NAME:   icf_cfg_network_activate_resp
 *
 * DESCRIPTION:     This function sends response to external Application 
 *                  Module for a request message.
 *
 * RETURNS:         ICF_SUCESS in case of success and ICF_FAILURE in 
 *                  case of failure.
 *
 *****************************************************************************/
icf_return_t icf_cfg_network_activate_resp(
        INOUT   icf_crm_pdb_st *p_crm_pdb,
        IN      icf_app_id_list_st app_id_list);
#endif /*#ifdef ICF_NW_ACTIVATE*/

/*******************************************************************************
 *
 * FUNCTION:        icf_cfg_application_add_hdlr
 *
 * DESCRIPTION:     This function is called when the API ICF_APP_ADD_REQ 
 *                  is received.
 *                  This function adds application context data. 
 *                  
 * RETURNS:         ICF_SUCCESS in case of success and ICF_FAILURE in
 *                  case module's global data cannot be fetched.
 *
 ******************************************************************************/
icf_return_t icf_cfg_application_add_hdlr(
        INOUT   icf_crm_pdb_st *p_crm_pdb);

/****************************************************************************
 * FUNCTION:        icf_cfg_get_num_apps
 *
 * DESCRIPTION:     This function determines the number of valid entries in 
 *                  application context block	
 *
 * RETURNS:             
 *
 ***************************************************************************/
icf_return_t icf_cfg_get_num_apps(
        INOUT   icf_crm_pdb_st *p_crm_pdb,
        OUT     icf_uint32_t *num_apps);

/****************************************************************************
 * FUNCTION:        icf_cfg_get_vacant_app_index
 *
 * DESCRIPTION:     This function determines vacant application context index
 *
 * RETURNS:             
 *
 ***************************************************************************/
icf_return_t icf_cfg_get_vacant_app_index(
        INOUT   icf_crm_pdb_st *p_crm_pdb,
        OUT     icf_uint8_t *app_index);

/****************************************************************************
 * FUNCTION:        icf_cfg_app_already_present
 *
 * DESCRIPTION:     This function determines if application context is alraedy 
 *                  present
 *
 ***************************************************************************/
/* Fix for SPR: 19886 (Merged CSR-1-7423906)*/
/* Fix for CSR 1-7423906: SPR 19772 */
icf_return_t icf_cfg_app_already_present(
        INOUT   icf_crm_pdb_st *p_crm_pdb,
        IN      icf_string_st app_name,
#ifdef ICF_PORT_DEBIAN
        INOUT     icf_uint8_t *app_index);
#else
        INOUT     icf_uint16_t *app_index);
#endif

/*******************************************************************************
 *
 * FUNCTION NAME:   icf_cfg_application_add_resp
 *
 * DESCRIPTION:     This function sends response to external Application 
 *                  Module for a request message.
 *
 * RETURNS:         ICF_SUCESS in case of success and ICF_FAILURE in 
 *                  case of failure.
 *
 *****************************************************************************/
icf_return_t icf_cfg_application_add_resp(
        INOUT   icf_crm_pdb_st *p_crm_pdb,
        INOUT   icf_void_st     *p_routing_info,
        IN      icf_app_id_t app_id);

/*******************************************************************************
 *
 * FUNCTION:        icf_cfg_application_remove_hdlr
 *
 * DESCRIPTION:     This function is called when the API ICF_APP_REMOVE_REQ 
 *                  is received.
 *                  This function adds application context data. 
 *                  
 * RETURNS:         ICF_SUCCESS in case of success and ICF_FAILURE in
 *                  case module's global data cannot be fetched.
 *
 ******************************************************************************/
icf_return_t icf_cfg_application_remove_hdlr(
        INOUT   icf_crm_pdb_st *p_crm_pdb);

/*******************************************************************************
 *
 * FUNCTION NAME:   icf_cfg_application_remove_resp
 *
 * DESCRIPTION:     This function sends response to external Application 
 *                  Module for a request message.
 *
 * RETURNS:         ICF_SUCESS in case of success and ICF_FAILURE in 
 *                  case of failure.
 *
 *****************************************************************************/
icf_return_t icf_cfg_application_remove_resp(
        INOUT   icf_crm_pdb_st *p_crm_pdb,
        IN      icf_app_id_t app_id);

#ifdef ICF_NW_ACTIVATE
/*******************************************************************************
 *
 * FUNCTION:        icf_cfg_network_deactivate_hdlr
 *
 * DESCRIPTION:     This function is called when the API ICF_NETWORK_DEACTIVATE_REQ 
 *                  is received.
 *                  This function adds application context data. 
 *                  
 * RETURNS:         ICF_SUCCESS in case of success and ICF_FAILURE in
 *                  case module's global data cannot be fetched.
 *
 ******************************************************************************/
icf_return_t icf_cfg_network_deactivate_hdlr(
        INOUT   icf_crm_pdb_st *p_crm_pdb);

/*******************************************************************************
 *
 * FUNCTION NAME:   icf_cfg_network_deactivate_resp
 *
 * DESCRIPTION:     This function sends response to external Application 
 *                  Module for a request message.
 *
 * RETURNS:         ICF_SUCESS in case of success and ICF_FAILURE in 
 *                  case of failure.
 *
 *****************************************************************************/
icf_return_t icf_cfg_network_deactivate_resp(
        INOUT   icf_crm_pdb_st *p_crm_pdb,
        IN      icf_app_id_t app_id);
#endif/*#ifdef ICF_NW_ACTIVATE*/

/****************************************************************************
 * FUNCTION:        icf_cfg_fetch_app_index
 *
 * DESCRIPTION:     This function fetches app_index for a given app_id
 *                  in Application Context
 *
 * RETURNS:             
 *
 ***************************************************************************/
icf_return_t icf_cfg_fetch_app_index(
        INOUT icf_crm_pdb_st *p_crm_pdb,
        IN    icf_app_id_t app_id,
        OUT	  icf_uint8_t *app_index);

#ifdef ICF_NW_ACTIVATE
/****************************************************************************
 * FUNCTION:        icf_cfg_fetch_network_app_index
 *
 * DESCRIPTION:     This function determines network and application index 
 *                  for a given apn and app_id in Network Activate Context	
 *
 * RETURNS:             
 *
 ***************************************************************************/
icf_return_t icf_cfg_fetch_network_app_index(
        INOUT   icf_crm_pdb_st *p_crm_pdb,
        IN      icf_string_st apn,
        IN      icf_app_id_t app_id,
        OUT     icf_uint8_t *ntwrk_index,
        OUT     icf_uint8_t *app_index);

/****************************************************************************
 * FUNCTION:        icf_cfg_nm_network_deactivate
 *
 * DESCRIPTION:     This function requests network deactivation from NM 
 *
 * RETURNS:         ICF_TRUE if network gets deactivated
 *                  ICF_FALSE if network de-activation fails           
 *
 ***************************************************************************/
icf_return_t icf_cfg_nm_network_deactivate(
        INOUT   icf_crm_pdb_st *p_crm_pdb,
		INOUT   icf_internal_msg_st *p_internal_msg,
        IN      icf_uint32_t ctxt_id);

/*******************************************************************************
 *
 * FUNCTION:        icf_cfg_validate_network_deactivate
 * 
 * DESCRIPTION:     This function validates the params received in network 
 *                  deactivate API.
 *                  
 * RETURNS:         ICF_SUCCESS always
 * 
 *****************************************************************************/
icf_return_t icf_cfg_validate_network_deactivate(
        INOUT   icf_crm_pdb_st           *p_crm_pdb);

/*******************************************************************************
 *
 * FUNCTION:        icf_cfg_process_nw_event_ind
 *
 * DESCRIPTION:     This function is called when network status gets changed 
 *                  due to some failure, uninitiated by user request
 *
 * RETURNS:         ICF_SUCCESS in case of success and ICF_FAILURE in
 *                  case module's global data cannot be fetched.
 *
 ******************************************************************************/
icf_return_t icf_cfg_process_nw_event_ind(
        INOUT   icf_crm_pdb_st *p_crm_pdb);

/*******************************************************************************
 *
 * FUNCTION:        icf_cfg_process_nw_auto_deactivate
 *
 * DESCRIPTION:     This function is called when network get deactivated 
 *                  uninitiated by user request
 *
 * RETURNS:         ICF_SUCCESS in case of success and ICF_FAILURE in
 *                  case module's global data cannot be fetched.
 *
 ******************************************************************************/
icf_return_t icf_cfg_process_nw_auto_deactivate(
        INOUT   icf_crm_pdb_st *p_crm_pdb);

/*******************************************************************************
 *
 * FUNCTION NAME:   icf_cfg_network_event_indication
 *
 * DESCRIPTION:     This function sends response to external Application 
 *                  Module for a request message.
 *
 * RETURNS:         ICF_SUCESS in case of success and ICF_FAILURE in 
 *                  case of failure.
 *
 *****************************************************************************/
icf_return_t icf_cfg_network_event_indication(
        INOUT   icf_crm_pdb_st *p_crm_pdb,
        IN      icf_app_id_list_st app_id_list,
ICF_NW_RECOVERY_START
        IN      icf_uint32_t          event_code);
ICF_NW_RECOVERY_END

/*******************************************************************************
 *
 * FUNCTION:        icf_cfg_process_deactivate_on_dereg
 *
 * DESCRIPTION:     This function is called when REGM triggers network 
 *                  deactivation on deregister.
 *
 * RETURNS:         ICF_SUCCESS in case of success and ICF_FAILURE in
 *                  case module's global data cannot be fetched.
 *
 ******************************************************************************/
icf_return_t icf_cfg_process_deactivate_on_dereg(
        INOUT   icf_crm_pdb_st *p_crm_pdb);
#endif/*#ifdef ICF_NW_ACTIVATE*/

/*******************************************************************************
 *
 * FUNCTION:        icf_cfg_validate_request_header
 * 
 * DESCRIPTION:     This function validates the header for various request APIs 
 *                  
 * RETURNS:         ICF_SUCCESS always
 * 
 *****************************************************************************/
icf_return_t icf_cfg_validate_request_header(
        INOUT   icf_crm_pdb_st           *p_crm_pdb);

/*******************************************************************************
 *
 * FUNCTION:        icf_cfg_validate_app_add
 * 
 * DESCRIPTION:     This function validates the params received in application 
 *                  add API.
 *                  
 * RETURNS:         ICF_SUCCESS always
 * 
 *****************************************************************************/
icf_return_t icf_cfg_validate_app_add(
        INOUT   icf_crm_pdb_st           *p_crm_pdb);
ICF_CRM_END
/*******************************************************************************
 * FUNCTION:        icf_cfg_parse_filter_expression
 *
 * DESCRIPTION:     This function parses the filter expression strings given in 
 *                  the application filter structure by an application in APP_ADD_REQ. 
 *                  The expression string is stored in the filter expression 
 *                  structure if the parsing is successful.
 *					The function returns ICF_SUCCESS if the expression string is 
 *                  successfully parsed and stored in the filter expression structure 
 *                  passed.
 *
 ******************************************************************************/

 ICF_NW_RECOVERY_START
#ifdef ICF_NW_ACTIVATE
/*******************************************************************************
 *
 * FUNCTION:        icf_cfg_process_nw_recovered
 *
 * DESCRIPTION:     This function is called when network get reactivated
 *                  uninitiated by user request
 *
 * RETURNS:         ICF_SUCCESS in case of success and ICF_FAILURE in
 *                  case module's global data cannot be fetched.
 *
 ******************************************************************************/
icf_return_t icf_cfg_process_nw_recovered(
        INOUT   icf_crm_pdb_st *p_crm_pdb);
#endif

/*******************************************************************************
 *
 * FUNCTION:        icf_cfg_cm_call_clear_ind_wrt_app
 *
 * DESCRIPTION:     This function sends forceful call clear to CM for all active
 *                  calls associated with particular application.
 *                  
 * RETURNS:         ICF_SUCCESS in case of sucess and ICF_FAILURE
 *                  in case of failure.
 *
 ******************************************************************************/
icf_return_t icf_cfg_cm_call_clear_ind_wrt_app(
        INOUT   icf_crm_pdb_st           *p_crm_pdb,
        IN      icf_app_id_t				app_id);

ICF_NW_RECOVERY_END
ICF_CONF_STATUS_API_START
/*******************************************************************************
 *
 * FUNCTION:        icf_cfg_clone_apn_list
 * 
 * DESCRIPTION:     This function clones the APN list
 *                  
 * RETURNS:         ICF_SUCCESS always
 * 
 *****************************************************************************/
icf_return_t icf_cfg_clone_apn_list(
        INOUT	icf_glb_pdb_st		    *p_glb_pdb,
        IN 		icf_uint32_t		         nofapns,
        IN		icf_string_st	        *p_src_apn_list,
        OUT		icf_string_st	       **p_dst_apn_list,
        OUT		icf_error_t	    		*p_ecode);    
/*******************************************************************************
 *
 * FUNCTION:        icf_cfg_get_system_settings_req_hdlr
 *
 * DESCRIPTION:     This function is called when Application triggers req 
 *                  to fetch system settings.
 *
 * RETURNS:         ICF_SUCCESS in case of success and ICF_FAILURE in
 *                  case module's global data cannot be fetched.
 *
 ******************************************************************************/

icf_return_t icf_cfg_get_system_settings_req_hdlr(
        INOUT   icf_crm_pdb_st *p_crm_pdb);
/*******************************************************************************
 *
 * FUNCTION:        icf_cfg_get_line_settings_req_hdlr
 *
 * DESCRIPTION:     This function is called when Application triggers req 
 *                  to fetch line settings.
 *
 * RETURNS:         ICF_SUCCESS in case of success and ICF_FAILURE in
 *                  case module's global data cannot be fetched.
 *
 ******************************************************************************/
icf_return_t icf_cfg_get_line_settings_req_hdlr(
        INOUT   icf_crm_pdb_st *p_crm_pdb);

/*******************************************************************************
 *
 * FUNCTION:        icf_cfg_send_system_settings_resp
 *
 * DESCRIPTION:     This function is called when ICF wants to send 
 *                  resp to application for get-system-settings API 
 *
 * RETURNS:         ICF_SUCCESS in case of success and ICF_FAILURE in
 *                  case module's global data cannot be fetched.
 *
 ******************************************************************************/
icf_return_t icf_cfg_send_system_settings_resp(
        INOUT   icf_crm_pdb_st                  *p_crm_pdb);

/*******************************************************************************
 *
 * FUNCTION:        icf_cfg_send_line_settings_resp
 *
 * DESCRIPTION:     This function is called when ICF wants to send 
 *                  resp to application for get-line-settings API 
 *
 * RETURNS:         ICF_SUCCESS in case of success and ICF_FAILURE in
 *                  case module's global data cannot be fetched.
 *
 ******************************************************************************/
icf_return_t icf_cfg_send_line_settings_resp(
        INOUT   icf_crm_pdb_st                  *p_crm_pdb);

ICF_CONF_STATUS_API_END

icf_return_t 
icf_cfg_parse_filter_expression(INOUT icf_crm_pdb_st *p_crm_pdb,
								INOUT	icf_uint8_t		   *p_expr_str,
								OUT	icf_filter_expr_st  *p_expr,
								OUT	icf_error_t		   *p_ecode);

/*******************************************************************************
 * FUNCTION:        icf_cfg_parse_filter_list
 *
 * DESCRIPTION:     This function parses the filter list given by an application 
 *                  in APP_ADD_REQ. It converts the application filter_list to
 *                  internal filter list strucutre format.
 *                  
 *					The function returns ICF_SUCCESS if the filter list is 
 *                  successfully parsed and stored in the internal filter list 
 *                  structure passed.
 *
 ******************************************************************************/

icf_return_t icf_cfg_parse_filter_list( INOUT icf_crm_pdb_st      *p_crm_pdb,
										IN		icf_filter_list_st		   *p_app_filter_list,
										INOUT	icf_int_filter_list_st  **p_p_int_filter_list,
										OUT		icf_error_t			   *p_ecode);

/*******************************************************************************
 * FUNCTION:        icf_cfg_free_filter_expr
 *
 * DESCRIPTION:     Function to free Filter Expression strucure 
 *
 ******************************************************************************/

icf_return_t icf_cfg_free_filter_expr(INOUT icf_crm_pdb_st     *p_crm_pdb,
									        INOUT icf_filter_expr_st	*p_filter_expr);

/*******************************************************************************
 * FUNCTION:        icf_cfg_free_internal_filter_list
 *
 * DESCRIPTION:     Function to free Internal Filter List strucure 
 *
 ******************************************************************************/

icf_return_t icf_cfg_free_internal_filter_list(INOUT icf_crm_pdb_st         *p_crm_pdb,
													 INOUT icf_int_filter_list_st *p_filter_list);


#ifdef ICF_NW_ACTIVATE
/*******************************************************************************
 *
 * FUNCTION:        icf_cfg_nw_not_responding
 *
 * DESCRIPTION:     This function will clear all active calls and invoke a 
 *                  function of RGM to de-register all the users.
 *                  
 * RETURNS:         ICF_SUCCESS in case of success and ICF_FAILURE in
 *                  case of failure.
 *
 ******************************************************************************/
icf_return_t icf_cfg_nw_not_responding(
        INOUT   icf_crm_pdb_st           *p_crm_pdb);
#endif/*#ifdef ICF_NW_ACTIVATE*/

/*******************************************************************************
 *
 * FUNCTION:        icf_cfg_remove_app_ctxt
 *
 * DESCRIPTION:     This function is called when the same application sends 
 *                  app_add again and the app_add fails.
 *                  This function adds application context data. 
 *                  
 * RETURNS:         ICF_SUCCESS 
 *
 ******************************************************************************/
icf_return_t icf_cfg_remove_app_ctxt(
        INOUT   icf_crm_pdb_st *p_crm_pdb,
        IN      icf_app_id_t app_id);

/*******************************************************************************
 *
 * FUNCTION:        icf_cfg_delete_line
 *
 * DESCRIPTION:     This function will delete the line if after the user has 
 *                  been deleted, registrar and proxy also don't exist on the 
 *                  line specified.
 *
 ******************************************************************************/
icf_return_t icf_cfg_delete_line(
        INOUT   icf_crm_pdb_st           *p_crm_pdb,
        IN      icf_line_id_t             line_id);

/*******************************************************************************
 *
 * FUNCTION:        icf_cfg_send_reg
 *
 * DESCRIPTION:     This function is called when the application has activated
 *                  the network. This function will send REGISTER incase the 
 *					App ID is not GENERIC app ID and the AUTO_REG is ON.
 *                  
 * RETURNS:         ICF_SUCCESS 
 *
 ******************************************************************************/
icf_return_t			icf_cfg_send_reg(
				INOUT		icf_crm_pdb_st			*p_crm_pdb);
 
/*******************************************************************************
 *
 * FUNCTION:        icf_cfg_free_app_profile
 *
 * DESCRIPTION:     This function frees an app profile structure 
 * RETURNS:         ICF_SUCCESS 
 *
 ******************************************************************************/
icf_return_t icf_cfg_free_app_profile(icf_crm_pdb_st *p_crm_pdb,
                                            icf_app_profile_st *p_app_profile);

/*******************************************************************************
 *
 * FUNCTION:        icf_cfg_move_string_list
 *
 * DESCRIPTION:     This function moves the contents of p_src to p_dest
 * RETURNS:         ICF_SUCCESS 
 *
 ******************************************************************************/
icf_return_t icf_cfg_move_string_list(icf_string_list_st *p_dest,icf_string_list_st *p_src);

/*******************************************************************************
 *
 * FUNCTION:        icf_cfg_cm_call_clear_ind_wrt_user
 *
 * DESCRIPTION:     This function sends forceful call clear to CM for all active
 *                  calls associated with particular user.
 *                  
 * RETURNS:         icf_SUCCESS in case of sucess and icf_FAILURE
 *                  in case of failure.
 *
 ******************************************************************************/
icf_return_t icf_cfg_cm_call_clear_ind_wrt_user(
        INOUT   icf_crm_pdb_st           *p_crm_pdb,
        INOUT		icf_address_st			*p_rgm_uri,
		IN		icf_line_id_t			line_id,
		IN		icf_boolean_t			line_specific,
		OUT		icf_error_t				*p_ecode);

/*******************************************************************************
 *
 * FUNCTION:        icf_cfg_cm_call_clear_ind_wrt_app_user
 *
 * DESCRIPTION:     This function sends forceful call clear to CM for all active
 *                  calls associated with set of particular user and particular
 *					application.
 *                  
 * RETURNS:         icf_SUCCESS in case of sucess and icf_FAILURE
 *                  in case of failure.
 *
 ******************************************************************************/
icf_return_t icf_cfg_cm_call_clear_ind_wrt_app_user(
        INOUT   icf_crm_pdb_st           *p_crm_pdb,
        INOUT		icf_address_st			*p_rgm_uri,
		INOUT		icf_app_id_t				app_id,
		OUT		icf_error_t				*p_ecode);

/*******************************************************************************
 *
 * FUNCTION:        icf_cfg_update_rgm_context_remove_entry
 *
 * DESCRIPTION:     This function updates the rgm context whenever a user/registrar
  *					registration for a line has to be removed.
 *					
 * RETURNS:         ICF_SUCCESS 
 *
 ******************************************************************************/
icf_return_t icf_cfg_update_rgm_context_remove_entry(
						INOUT	icf_crm_pdb_st		*p_crm_pdb,
						INOUT	icf_rgm_context_st	*p_rgm_context,
						IN		icf_line_id_t		line_id);
/************************************************************************
 *
 * FUNCTION NAME: icf_cfg_delete_public_uri_list 
 *
 * DESCRIPTION : This function deletes the public uri list passed
 *              
 * RETURNS: icf_return_t
 *
 ************************************************************************/

icf_return_t 
icf_cfg_delete_public_uri_list (
                                icf_glb_pdb_st	*p_glb_pdb,
                                icf_pub_uri_list_node_st **p_p_pub_uri_list,
                                icf_error_t     *p_ecode
                                );
#ifdef ICF_NW_ACTIVATE
/********************************************************************
 *
 * FUNCTION:        icf_cfg_gprs_suspend_hdlr
 *
 * DESCRIPTION:     This function is called when GPRS suspend
 *                  is detected by the platform
 *
 * RETURNS:         ICF_SUCCESS/ ICF_FAILURE                   
 *
 *********************************************************************/
icf_return_t icf_cfg_gprs_suspend_hdlr(
        INOUT   icf_crm_pdb_st *p_crm_pdb);

/******************************************************************
 *
 * FUNCTION:        icf_cfg_gprs_resume_hdlr
 *
 * DESCRIPTION:     This function is called when GPRS ACTIVE
 *                  is detected by the platform
 *
 * RETURNS:         ICF_SUCCESS/ ICF_FAILURE                   
 *
 ****************************************************************/
icf_return_t icf_cfg_gprs_resume_hdlr(
        INOUT   icf_crm_pdb_st *p_crm_pdb);
#endif/*#ifdef ICF_NW_ACTIVATE*/

/*******************************************************************************
 *
 * FUNCTION:			icf_cfg_timer_expiry
 *
 * DESCRIPTION:         This function is called when TIMER_EXPIRY event is  
 *                      received from MRM.It checks the type of timer expired 
 *                      and invokes the respective handler.
 *
 ******************************************************************************/
icf_return_t icf_cfg_timer_expiry(
		        INOUT icf_crm_pdb_st *p_crm_pdb);

#ifdef ICF_NW_ACTIVATE
/*******************************************************************************
 *
 * FUNCTION:			icf_cfg_net_act_retry_timer_expiry
 *
 * DESCRIPTION:         This function is called when TIMER_EXPIRY event is  
 *                      received for network activate retry timer
 *
 ******************************************************************************/
icf_return_t icf_cfg_net_act_retry_timer_expiry(
				INOUT icf_crm_pdb_st *p_crm_pdb);

/*******************************************************************************
 *
 * FUNCTION:        icf_cfg_stop_net_act_retry_timer
 *
 * DESCRIPTION:     This function stops Retry timer.
 *
 ******************************************************************************/
icf_return_t icf_cfg_stop_net_act_retry_timer(
				INOUT	icf_crm_pdb_st  *p_crm_pdb,
				IN	    icf_uint8_t		nw_index);

/*******************************************************************************
 *
 * FUNCTION:        icf_cfg_start_net_act_retry_timer
 *
 * DESCRIPTION:     This function starts Retry timer.
 *
 ******************************************************************************/
icf_return_t icf_cfg_start_net_act_retry_timer(
				INOUT	icf_crm_pdb_st					*p_crm_pdb,
				IN		icf_cfg_net_act_retry_timer_st	*p_timer_buff);

/*******************************************************************************
 *
 * FUNCTION:        icf_cfg_request_network_activation_from_nm
 *
 * DESCRIPTION:     This function requests network activation from NM
 *
 ******************************************************************************/
icf_return_t icf_cfg_request_network_activation_from_nm(
				INOUT	icf_crm_pdb_st	*p_crm_pdb,
				IN		icf_uint8_t		nw_idx,
				IN		icf_cfg_net_act_retry_timer_st	*p_net_act_timer_buffer);

/*******************************************************************************
 *
 * FUNCTION:        icf_cfg_populate_network_activate_context
 *
 * DESCRIPTION:     This function populates network activate context
 *
 ******************************************************************************/
icf_return_t icf_cfg_populate_network_activate_context(
				INOUT	icf_crm_pdb_st	*p_crm_pdb,
				IN		icf_internal_msg_st	*p_internal_msg,
				IN		icf_uint8_t		nw_idx);

/*******************************************************************************
 *
 * FUNCTION:        icf_cfg_set_transport_data_for_activated_nw
 *
 * DESCRIPTION:     This function populates transport data for activated network
 *
 ******************************************************************************/
void icf_cfg_set_transport_data_for_activated_nw(
				INOUT	icf_crm_pdb_st	*p_crm_pdb,
				IN		icf_internal_msg_st	*p_internal_msg);

/*******************************************************************************
 *
 * FUNCTION:        icf_cfg_set_line_data_for_activated_nw
 *
 * DESCRIPTION:     This function populates line data for activated network
 *
 ******************************************************************************/
icf_return_t icf_cfg_set_line_data_for_activated_nw(
				INOUT	icf_crm_pdb_st	*p_crm_pdb,
				IN		icf_internal_msg_st	*p_internal_msg,
				IN		icf_uint8_t		nw_idx);

/*******************************************************************************
 *
 * FUNCTION:        icf_cfg_open_network_connection
 *
 * DESCRIPTION:     This function opens network connection
 *
 ******************************************************************************/
icf_return_t icf_cfg_open_network_connection(
				INOUT	icf_crm_pdb_st	*p_crm_pdb);

/*******************************************************************************
 *
 * FUNCTION:        icf_cfg_set_user_address
 *
 * DESCRIPTION:     This function sets user address
 *
 ******************************************************************************/
icf_return_t icf_cfg_set_user_address(
				INOUT	icf_crm_pdb_st	*p_crm_pdb,
				IN		icf_nm_acs_info_st	*p_acs_info);

/*******************************************************************************
 *
 * FUNCTION:        icf_cfg_set_private_username
 *
 * DESCRIPTION:     This function sets private username
 *
 ******************************************************************************/
icf_return_t icf_cfg_set_private_username(
				INOUT	icf_crm_pdb_st	*p_crm_pdb,
				IN		icf_nm_acs_info_st	*p_acs_info);

/*******************************************************************************
 *
 * FUNCTION:        icf_cfg_set_auth_passwd
 *
 * DESCRIPTION:     This function sets auth password
 *
 ******************************************************************************/
icf_return_t icf_cfg_set_auth_passwd(
				INOUT	icf_crm_pdb_st	*p_crm_pdb);

/*******************************************************************************
 *
 * FUNCTION:        icf_cfg_set_network_activation_failure
 *
 * DESCRIPTION:     This function sets network activation failure 
 *
 ******************************************************************************/
icf_return_t icf_cfg_set_network_activation_failure(
				INOUT	icf_crm_pdb_st	*p_crm_pdb,
				IN		icf_uint8_t		nw_idx);

/*******************************************************************************
 *
 * FUNCTION:        icf_cfg_set_registrar_data
 *
 * DESCRIPTION:     This function populates registrar data
 *
 ******************************************************************************/
icf_return_t icf_cfg_set_registrar_data(
				INOUT	icf_crm_pdb_st		*p_crm_pdb,
				IN		icf_nm_acs_info_st	*p_acs_info);


/*******************************************************************************
 *
 * FUNCTION:        icf_cfg_set_network_activation_success
 *
 * DESCRIPTION:     This function sets network activation success
 *
 ******************************************************************************/
icf_return_t icf_cfg_set_network_activation_success(
				INOUT	icf_crm_pdb_st	*p_crm_pdb);

/*******************************************************************************
 *
 * FUNCTION:        icf_cfg_create_network_context
 *
 * DESCRIPTION:     This function creates network activate context
 *
 ******************************************************************************/
icf_return_t icf_cfg_create_network_context(
				INOUT	icf_crm_pdb_st  *p_crm_pdb,
				IN	    icf_cfg_net_act_retry_timer_st	*p_net_act_timer_buffer,
				OUT		icf_uint8_t		*network_index);

/*******************************************************************************
 *
 * FUNCTION:        icf_cfg_start_retry_timer
 *
 * DESCRIPTION:     This function populates network activate context
 *
 ******************************************************************************/
icf_return_t icf_cfg_start_retry_timer(
				INOUT	icf_crm_pdb_st	*p_crm_pdb,
				IN		icf_uint8_t		nw_idx);

/************************************************************************
 *
 * FUNCTION NAME: icf_cfg_send_to_mm 
 *
 * DESCRIPTION : This function sends ip to MM after network activation
 *              
 * RETURNS: icf_return_t
 *
 ************************************************************************/
icf_return_t icf_cfg_send_to_mm(
				INOUT	icf_crm_pdb_st	*p_crm_pdb,
				icf_nw_status_st			*p_nw_stat);


#endif/*#ifdef ICF_NW_ACTIVATE*/

/***************************************************************************
 *
 * FUNCTION:        icf_cfg_get_asso_uris 
 * 
 * DESCRIPTION:     This function returns all associated URIs          
 *                  
 *                  
 *                   
 * RETURNS:         ICF_SUCCESS: In case associated URI is found.       
 *					ICF_FAILURE: In case no associated URI is found
 ******************************************************************************/
icf_return_t    
icf_cfg_get_asso_uris (
    INOUT    icf_glb_pdb_st    *p_glb_pdb,
    INOUT    icf_pub_uri_list_node_st  **p_p_uris,
    INOUT    icf_error_t        *p_ecode);

/*************************************************************************
 *
 * FUNCTION:        icf_cfg_update_mm_self_ip
 * 
 * DESCRIPTION:              
 *                  
 *                  
 *                   
 * RETURNS:         ICF_SUCCESS:        
 *                    ICF_FAILURE: 
 *************************************************************************/
icf_return_t icf_cfg_update_mm_self_ip(
        INOUT icf_crm_pdb_st                         *p_crm_pdb);

/*******************************************************************************
 *
 * FUNCTION:        icf_cfg_validate_mode
 * 
 * DESCRIPTION:     This function will check for the self mode.If it is changed 
 *                  from BOTH/UDP->TCP or BOTH/TCP->UDP, it will update the  
 *                  mode in proxy/vms/registrar correspondingly.
 *                  
 * RETURNS:         ICF_SUCCESS.
 *
 ******************************************************************************/

icf_return_t icf_cfg_validate_mode(
        INOUT   icf_crm_pdb_st          *p_crm_pdb);

/*******************************************************************************
 *  
 * FUNCTION:        icf_cfg_validate_srvc_config_prmms
 *  
 * DESCRIPTION:     This function will validate the non-line specific data
 *                  of the set service params API.
 *                  
 * RETURNS:         ICF_SUCCESS always
 *
 ******************************************************************************/
icf_return_t icf_cfg_validate_srvc_config_prms(
        INOUT   icf_crm_pdb_st           *p_crm_pdb);


/******************************************************************************
 *
 * FUNCTION:        icf_cfg_get_nw_info
 *
 * DESCRIPTION:     
 *                   
 * RETURNS:         
 *
 *****************************************************************************/
icf_return_t icf_cfg_get_nw_info(
        INOUT   icf_crm_pdb_st           *p_crm_pdb);




#ifdef ICF_NAT_RPORT_SUPPORT
/*************************************************************************
 *
 * FUNCTION:        icf_cfg_handle_nat_binding_chng
 * 
 * DESCRIPTION:     This function will handle the NAT binding change event
 *                  Clear all ongoing calls and unsubscribe all users.
 *                  
 * RETURNS:         ICF_SUCCESS:        
 *                  ICF_FAILURE: 
 *************************************************************************/

icf_return_t    icf_cfg_handle_nat_binding_chng
(
 INOUT  icf_crm_pdb_st          *p_crm_pdb
);
#endif

/*******************************************************************************
 *
 * FUNCTION:        icf_cfg_validate_self_addr_per_line
 * 
 * DESCRIPTION:     This function will validate the self tranport parmeters
 *                  received per line.
 *                  
 * RETURNS:         ICF_SUCCESS always
 *
 ******************************************************************************/
icf_return_t icf_cfg_validate_self_addr_per_line(
      icf_crm_pdb_st    *p_crm_pdb);

/*******************************************************************************
 *
 * FUNCTION:        icf_cfg_set_self_addr_in_db_per_line
 * 
 * DESCRIPTION:     This function will update the self tranport parmeters
 *                  received per line.
 *                  
 * RETURNS:         ICF_SUCCESS always
 *
 ******************************************************************************/
icf_return_t icf_cfg_set_self_addr_in_db_per_line(
      icf_crm_pdb_st    *p_crm_pdb);

/*******************************************************************************
 *
 * FUNCTION:        icf_cfg_update_self_trnspt_in_line_db
 * 
 * DESCRIPTION:     This function will update the self_transport data received 
 *                  in Set Transport params message into line database.
 *                  
 * RETURNS:         ICF_SUCCESS.
 *
 ******************************************************************************/
icf_return_t icf_cfg_update_self_trnspt_in_line_db(
        INOUT icf_crm_pdb_st                         *p_crm_pdb);

icf_return_t icf_cfg_init_complete_hdlr_for_line(
        INOUT   icf_crm_pdb_st           *p_crm_pdb);

icf_return_t icf_cfg_check_init_complete_for_line(
        INOUT   icf_crm_pdb_st           *p_crm_pdb);

/*******************************************************************************
 *
 * FUNCTION:        icf_cfg_cm_call_clear_ind_wrt_line
 *
 * DESCRIPTION:     This function sends forceful call clear to CM for all active
 *                  calls on a particular line.
 *
 * RETURNS:         ICF_SUCCESS in case of sucess and ICF_FAILURE
 *                  in case of failure.
 *
 ******************************************************************************/
icf_return_t icf_cfg_cm_call_clear_ind_wrt_line(
        INOUT   icf_crm_pdb_st           *p_crm_pdb);

/*******************************************************************************
 *
 * FUNCTION:        icf_cfg_dereg_for_line
 *
 * DESCRIPTION:     This function sends DeRegister request for particular line
 *
 * RETURNS:         ICF_SUCCESS in case of sucess and ICF_FAILURE
 *                  in case of failure.
 *
 ******************************************************************************/
icf_return_t icf_cfg_dereg_for_line(
              icf_crm_pdb_st    *p_crm_pdb);


/*******************************************************************************
 * FUNCTION:        icf_cfg_validate_supported_method_list
 * DESCRIPTION:     This function validates whether the all the methods specified
 *                  in app_add_req are supported by ICF
 * RETURNS:         ICF_SUCCESS in case of sucess and ICF_FAILURE
 *                  in case of failure.
 ******************************************************************************/
icf_return_t icf_cfg_validate_supported_method_list
(
        INOUT   icf_crm_pdb_st           *p_crm_pdb,
        IN      icf_string_list_st       *p_str_list
);

/*************************************************************************
 *
 * FUNCTION:        icf_cfg_set_line_params_hdlr
 * 
 * DESCRIPTION:     This function will handle the ICF_SET_LINE_PARAM_REQ
 *                  API send from application..
 *                  
 * RETURNS:         ICF_SUCCESS:        
 *                  ICF_FAILURE: 
 *************************************************************************/
icf_return_t icf_cfg_set_line_params_hdlr 
(
	INOUT	icf_crm_pdb_st	*p_crm_pdb
);

/*************************************************************************
 *
 * FUNCTION:        icf_cfg_validate_line_prms
 * 
 * DESCRIPTION:     This function will validate the ICF_SET_LINE_PARAM_REQ
 *                  API send from application..
 *                  
 * RETURNS:         ICF_SUCCESS:        
 *                  ICF_FAILURE: 
 *************************************************************************/
icf_return_t icf_cfg_validate_line_prms 
(
	INOUT	icf_crm_pdb_st	*p_crm_pdb
);


/*******************************************************************************
 *
 * FUNCTION:        icf_cfg_cm_reinit_ind
 *
 * DESCRIPTION:     This function sends forceful call clear to CM for all active
 *                  calls.
 *
 * RETURNS:         ICF_SUCCESS in case of sucess and ICF_FAILURE
 *                  in case of failure.
 *
 ******************************************************************************/
icf_return_t icf_cfg_cm_reinit_ind(
        INOUT   icf_crm_pdb_st           *p_crm_pdb);



/*******************************************************************************
 *
 * FUNCTION:        icf_cfg_ssa_initiate_cleanup
 *
 * DESCRIPTION:     This function triggers SSA to initiate clean up of all its
 *                  context including timers.
 *
 *****************************************************************************/
icf_return_t icf_cfg_ssa_initiate_cleanup(
        INOUT   icf_crm_pdb_st   *p_crm_pdb);

/*************************************************************************
 *
 * FUNCTION:        icf_cfg_trim_string
 *
 * DESCRIPTION:     This function will remove the single and double quotes
 *                  from input string.
 *
 * RETURNS:         length of modified string
 *************************************************************************/
icf_uint16_t icf_cfg_trim_string(icf_uint8_t   *p_str);

/*************************************************************************
 *
 * FUNCTION:        icf_cfg_trim_string_for_filter_list
 *
 * DESCRIPTION:     This function will remove the single and double quotes
 *                  from input string.
 *
 * RETURNS:         length of modified string
 *************************************************************************/
icf_uint16_t icf_cfg_trim_string_for_filter_list(icf_uint8_t   *p_str);

/*************************************************************************
 *
 * FUNCTION:        icf_cfg_get_register_status_req_hdlr
 *
 * DESCRIPTION:     This function is used  to handle the API 
 *                  ICF_CFG_GET_REGISTER_STATUS_REQ 
 *
 * RETURNS:         This returns ICF_SUCCESS if success else it returns
 *                  ICF_FAILURE.
 *************************************************************************/

icf_return_t icf_cfg_get_register_status_req_hdlr(
                      INOUT  icf_crm_pdb_st  *p_crm_pdb);

#ifdef ICF_NAT_RPORT_SUPPORT

/*******************************************************************************
 *
 * FUNCTION:        icf_cfg_rport_config_data_init
 *
 * DESCRIPTION:     This function initialises the RPORT config data
 *
 * RETURNS:         ICF_SUCCESS in case of sucess and ICF_FAILURE
 *                  in case of failure.
 *
 ******************************************************************************/
icf_return_t icf_cfg_rport_config_data_init(
        INOUT   icf_glb_pdb_st                     *p_glb_pdb,
        INOUT   icf_rport_config_data_st           **p_p_rport_config_data);
#endif




/*************************************************************************
 *
 * FUNCTION:        icf_cfg_get_register_status_req_hdlr
 *
 * DESCRIPTION:     This function is used  to handle the API 
 *                  ICF_CFG_GET_REGISTER_STATUS_REQ 
 *
 * RETURNS:         This returns ICF_SUCCESS if success else it returns
 *                  ICF_FAILURE.
 *************************************************************************/

icf_return_t icf_cfg_get_register_status_req_hdlr(
                      INOUT  icf_crm_pdb_st  *p_crm_pdb);

/*******************************************************************************
 *
 * FUNCTION:        icf_cfg_cm_forceful_call_clear
 *
 * DESCRIPTION:     This function sends forceful call clear to CM for all active
 *                  calls.
 *
 * RETURNS:         ICF_SUCCESS in case of sucess and ICF_FAILURE
 *                  in case of failure.
 *
 ******************************************************************************/

icf_return_t icf_cfg_cm_forceful_call_clear(
                      INOUT icf_crm_pdb_st *p_crm_pdb);


/************************************************************************
 *
 * FUNCTION NAME: icf_cfg_process_auth_key_for_user
 *
 *  DESCRIPTION : This function adds/deletes the auth data
 *                in the user database of the given of given user.
 *
 * RETURNS: icf_return_t
 *
************************************************************************/
icf_return_t icf_cfg_process_auth_key_for_user
(
    icf_glb_pdb_st              *p_glb_pdb,
        icf_cfg_cc_set_self_id_params_req_st *p_recv_msg_payload
);


/************************************************************************
 *
 * FUNCTION NAME: icf_cfg_process_auth_passwd_for_user
 *
 *  DESCRIPTION : This function adds/deletes the auth passwd
 *                in the user database of the given of given user.
 *
 * RETURNS: icf_return_t
 *
************************************************************************/
icf_return_t icf_cfg_process_auth_passwd_for_user
(
    icf_glb_pdb_st                       *p_glb_pdb,
    icf_cfg_cc_set_self_id_params_req_st *p_recv_msg_payload
    );

/************************************************************************
 *
 * FUNCTION NAME: icf_cfg_handle_dhcp_query
 *
 *  DESCRIPTION : This function internally invokes port functions to do 
 * 		  DHCP query. This function do default DHCP query for SIP 
 *  		  server discovery. It checks if DNS server address is 
 *                not configured by application then do DHCP query for DNS
 *		  server discovery also.
 *
 * RETURNS: icf_return_t
 *
************************************************************************/

icf_return_t icf_cfg_handle_dhcp_query
(
	INOUT icf_crm_pdb_st *p_crm_pdb
);

/************************************************************************
 *
 * FUNCTION NAME: icf_cfg_cm_call_clear_ind_wrt_user_on_all_lines 
 *
 * DESCRIPTION :  This function clears the calls from all line with
 *                 respect to a particular URI 
 *                
 * RETURNS:       icf_return_t
 *
 ************************************************************************/
icf_return_t icf_cfg_cm_call_clear_ind_wrt_user_on_all_lines (
    INOUT    icf_crm_pdb_st    *p_crm_pdb,
    INOUT	 icf_address_st	   *p_rgm_uri,
	OUT		 icf_error_t	   *p_ecode);

/*  THis feature is out of scope now in 8.0 */
#if 0
/*************************************************************************
 *
 * FUNCTION:        icf_cfg_rediscover_proxy_server
 *
 * DESCRIPTION:     This function will be invoked when CFG receives
 *                  ICF_REDISCOVER_PROXY_SERVER from SSA. This function will
 *                  search for the proxy address received from SSA which is
 *                  not responding in icf_config_data_st first. If it is
 *                  found then clear all calls, dereg users if registered
 *                  through proxy and unsubscribe users if request was through
 *                  that proxy address.
 *
 * RETURNS:         ICF_SUCCESS or ICF_FAILURE
 *************************************************************************/

icf_return_t    icf_cfg_rediscover_proxy_server (
        icf_crm_pdb_st           *p_crm_pdb
        );
#endif

/*******************************************************************************
 *
 * FUNCTION NAME:   icf_cfg_validate_modify_regm_prms
 * 
 * DESCRIPTION:     This function validates the registrar address to be modified 
 *                  received in the API ICF_CFG_CC_SET_REGISTRATION_PARAMS_REQ 
 *
 * RETURNS:         ICF_SUCCESS always
 *
 ******************************************************************************/
icf_return_t icf_cfg_validate_modify_regm_prms(
        INOUT   icf_crm_pdb_st   *p_crm_pdb,
        IN      icf_line_id_t    line_id,
        IN      icf_boolean_t    validate_primary);

/* SPR 19405 Merged SPR 19672 Start */
/*******************************************************************************
 * FUNCTION:        icf_cfg_check_all_lines_dealloc 
 * DESCRIPTION:     This function checks whether all the configurable lines are
 *                  deallocated.
 * RETURNS:         ICF_SUCCESS in case of sucess and ICF_FAILURE
 *                  in case of failure.
 ******************************************************************************/
icf_return_t icf_cfg_check_all_lines_dealloc(
        INOUT   icf_crm_pdb_st           *p_crm_pdb
);

/* SPR 19405 Merged SPR 19672 End */

/* SPR 19886 : Merged Fix for CSR_1-7009954: SPR-19599*/
/************************************************************************
 *
 * FUNCTION NAME: icf_cfg_process_realm_for_user
 *
 *  DESCRIPTION : This function adds/deletes the realm
 *                in the user database of the given of given user.
 *              
 * RETURNS: icf_return_t
 *
************************************************************************/
icf_return_t icf_cfg_process_realm_for_user 
(
    icf_glb_pdb_st		*p_glb_pdb,
    icf_cfg_cc_set_self_id_params_req_st *p_recv_msg_payload
);

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* End of __ICF_CFG_PROTOTYPES_H__ */
