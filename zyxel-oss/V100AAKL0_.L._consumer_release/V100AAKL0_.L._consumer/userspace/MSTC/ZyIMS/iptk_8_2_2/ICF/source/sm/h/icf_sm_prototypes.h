/*------------------------------------------------------------------------------
 *
 * File name        : icf_sm_prototypes.h
 *
 * Purpose          : This file contains prototypes of functions used in SM.
 *
 * Revision History :
 *
 * Date         Name                    Ref#        Comments
 * --------     ------------            ----        ---------------
 * 09-Mar-2005  Jyoti Narula        SM LLD      Beginning of coding phase
 * 14-Mar-2006  Amit Sharma                     CFG merged with ICF 2.0
 * 06-Oct-2006  Aman Arora          ICF Rel 6.1 New function prototypes added
 *                                              for licensing
 *
 * Copyright (c) 2006, Aricent.
 *----------------------------------------------------------------------------*/


#ifndef __ICF_SM_PROTOTYPES_H__
#define __ICF_SM_PROTOTYPES_H__


#include "icf_sm_int_types.h"
#include "icf_port_prototypes.h"


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
 * Prototypes of functions in icf_sm_init.c
 *----------------------------------------------------------------------------*/

/*************************************************************************
 * FUNCTION:        icf_sm_init
 *
 * DESCRIPTION:     This function allocates static memory to the gloal data 
 *                  for the SM module and it initializes the members
 *                  with default values and the function pointer list 
 *                  with API handlers.
 *
 * RETURNS:         ICF_SUCCESS in case of success and ICF_FAILURE in
 *                  case of failure.Failure will happen when Static memory 
 *                  for the sm_global data could not be allocated.

 ***************************************************************************/
icf_return_t icf_sm_init(
        INOUT icf_glb_pdb_st *p_glb_pdb,
        OUT   icf_void_t     **p_data,
        OUT   icf_error_t    *p_ecode);
 
/*******************************************************************************
 *
 * FUNCTION:        icf_sm_deinit
 *
 * DESCRIPTION:     This function is invoked at system termination and is used 
 *                  to free the memory allocated at init time.
 *
 ******************************************************************************/
icf_void_t icf_sm_deinit(
        INOUT icf_glb_pdb_st *p_glb_pdb,
        INOUT icf_void_t     *p_data,
        OUT   icf_error_t    *p_ecode);

#ifdef ICF_STAT_ENABLED
/**************************************************************
 * FUNCTION:    icf_sm_init_call_history
 *
 * DESCRIPTION: This is a utility function invoked by the 
 *              CFG module to pass the num_calls_to_log 
 *              value to the SM.It also allocates the static 
 *              memory to the call history arrays in the SM
 *              Global data. It also makes the value of the 
 *              num_dialed/num_missed/num_recd as 0.
 *
 *
 * RETURNS:     ICF_SUCCESS if the value is correctly
 *              recorded in the SM global data.
 *              ICF_FAILURE 
 *                  -if the SM global data could 
 *                  not be retrieved successfully.
 *                  -if the memory could not be allocated 
 *                  for any of the 3 call history arrays.
 *************************************************************/
icf_return_t icf_sm_init_call_history(
        INOUT   icf_glb_pdb_st  *p_glb_pdb,
        IN      icf_uint8_t     num_calls_to_log);

  
/*******************************************************************************
 *
 * FUNCTION:        icf_sm_deinit_call_history
 *
 * DESCRIPTION:     This function is invoked to free
 *                  the static memory allocated to the call history arrays.
 *
 ******************************************************************************/
icf_void_t icf_sm_deinit_call_history(
        INOUT   icf_glb_pdb_st           *p_glb_pdb,
        OUT     icf_error_t              *p_ecode);
#endif

/*------------------------------------------------------------------------------
 * Prototypes of functions in icf_sm_entry_pt.c
 *----------------------------------------------------------------------------*/

/****************************************************************************
 * FUNCTION:        icf_sm_process_mesg
 *
 * DESCRIPTION:     This is the entry point function for SM Handler.
 *                  It puts values into required fields into 
 *                  icf_sm_pdb_st's variable.
 *                  It checks the API ID received and then calls
 *                  the appropriate handler to process the API. 
 *
 ** RETURNS:         ICF_SUCCESS always:
 *                      -When Invalid_api_id recd: ECODE set.
 *                      -DBM returns failure: no action.
 *                      -The API Handler returns Failure: no action
 *
 *************************************************************************/
icf_return_t icf_sm_process_mesg( 
        INOUT   icf_internal_msg_st  *p_internal_msg);

    
/*******************************************************************************
 *
 * FUNCTION:        icf_sm_api_to_event
 *
 * DESCRIPTION:     It performs the mapping of received API Ids 
 *                  to SM events.These events are used to call 
 *                  the appropriate API handlers.
 *                  It checks the API ID received and updates 
 *                  the appropriate statistic value using ICF_STAT.
 *
 * RETURNS:         ICF_SUCCESS in case of success and ICF_FAILURE in
 *                  case of failure. Failure occurs when an invalid
 *                  API ID is received.
************************************************************************/
icf_return_t icf_sm_api_to_event(
       INOUT    icf_sm_pdb_st *p_sm_pdb);




/*------------------------------------------------------------------------------
 * Prototypes of functions in icf_sm_api_handlers.c
 *----------------------------------------------------------------------------*/

/**********************************************************************
 * FUNCTION:        icf_sm_set_trace_hdlr
 *
 * DESCRIPTION:     This function is called when ever the API 
 *                  ICF_SM_CC_SET_TRACE_LEVEL_REQ is received.
 *                  It sets the value of trace_level and trace_type
 *                  in the global data structure as given in the
 *                  payload of incoming API.This function sets the
 *                  values in the global data structure for 
 *                  trace_status,trace_type, trace_level
 *                  in the module data of SM in the glb_pdb.
 *                  
 * RETURNS:         ICF_SUCCESS in case of success and 
 *                  ICF_FAILURE in case of failure.
 *                  Failure occurs:
 *                      -When invalid trace_details are received in the 
 *                      API.
 *
 *********************************************************************/

icf_return_t icf_sm_set_trace_hdlr(
        INOUT   icf_sm_pdb_st        *p_sm_pdb);

/**********************************************************************
 * FUNCTION:        icf_sm_set_trace_req_hdlr
 *
 * DESCRIPTION:     This function is called when ever the API
 *                  ICF_SM_CC_SET_TRACE_REQ is received.
 *                  It sets the value of trace_criteria and trace_type
 *                  in the global data structure as given in the
 *                  payload of incoming API.This function sets the
 *                  values in the global data structure for
 *                  trace_status,trace_type, trace_criteria
 *                  in the module data of SM in the glb_pdb.
 *
 * RETURNS:         ICF_SUCCESS in case of success and
 *                  ICF_FAILURE in case of failure.
 *                  Failure occurs:
 *                      -When invalid trace_details are received in the
 *                      API.
 *
 *********************************************************************/
 icf_return_t icf_sm_set_trace_req_hdlr(
         INOUT   icf_sm_pdb_st        *p_sm_pdb);


/***********************************************************************
 * FUNCTION:        icf_sm_set_error_hdlr
 *
 * DESCRIPTION:     This function is called when ever the API 
 *                  ICF_SM_CC_SET_ERROR_LEVEL_REQ is received.
 *                  It sets the value of error_status, error_level and 
 *                  error_type in the global data structure as given 
 *                  in the payload of incoming API.
 *
 * RETURNS:         ICF_SUCCESS in case of success and 
 *                  ICF_FAILURE in case of failure. Failure
 *                  occurs when
 *                  -Invalid error_details are entered.
 *
 *********************************************************************/
icf_return_t icf_sm_set_error_hdlr(
       INOUT    icf_sm_pdb_st *p_sm_pdb);



/*******************************************************************
 * FUNCTION:        icf_sm_set_stats_hdlr
 *
 * DESCRIPTION:     This function is called by the function
 *                  icf_process_mesg whenever the
 *                  API ICF_SET_STAT_REQ is received.
 *                  This function resets the statistic values to zero,
 *                  if the reset flag in the received message 
 *                  is set to TRUE.
 *
 * RETURNS:         ICF_SUCCESS in case of success
 *                  even when reset flag is given as FALSE. 
 *                  ICF_FAILURE in case of failure.
 *                  Failure occurs when:
 *                  -The received stats_id is INVALID.
 *                  -Reset Flag has been given value other than 0/1.
 *                                    
 *************************************************************/
icf_return_t icf_sm_set_stats_hdlr(
       INOUT    icf_sm_pdb_st *p_sm_pdb);


/**********************************************************************
 * FUNCTION:        icf_sm_get_stats_hdlr
 * 
 * DESCRIPTION:     This function is called by the function 
 *                  icf_process_mesg whenever the
 *                  API ICF_GET_STAT_REQ is 
 *                  received. This function reads the value of 
 *                  stat id that is received with the API and 
 *                  fills the response API structure with 
 *                  the appropriate values of the 
 *                  stats/members of the stat group.
 *
 * RETUENS:         ICF_SUCCESS in case of success and ICF_FAILURE
 *                  otherwise.Failure occurs when:
 *                  -Memory allocation for p_sm_msg fails.
 *                  -SEND Macro returned failure. 
 *********************************************************************/

icf_return_t icf_sm_get_stats_hdlr(
       INOUT    icf_sm_pdb_st *p_sm_pdb);

/*********************************************************************
 * FUNCTION:     icf_sm_chk_module_status_hdlr
 * 
 * DESCRIPTION : This function is called by the function 
 *               icf_process_mesg whenever the API 
 *               ICF_SM_CC_CHECK_MODULE_STATUS_REQ is 
 *               received. The response API is sent to indicate that
 *               the module is alive. It has no parameters.
 *
 * RETURNS:      ICF_SUCCESS in case of success and ICF_FAILURE
 *               otherwise.Failure occurs when:
 *                  -The memory fails to be allocated 
 *                   for icf_msg_st, that has to be sent to SM.
 *                  -SEND Macro returned failure.
 *                   
 *************************************************************/
icf_return_t icf_sm_chk_module_status_hdlr(
       INOUT    icf_sm_pdb_st *p_sm_pdb);


/**********************************************************************
 * FUNCTION:     icf_sm_call_log_hdlr
 *
 * DESCRIPTION:  This function is called by the function 
 *               icf_process_mesg
 *               whenever the API ICF_SM_CC_GET_CALL_LOG_REQ 
 *               is received.
 *               The response API is sent after being filled 
 *               with the values.
 *
 * RETURNS:      ICF_SUCCESS in case of success and ICF_FAILURE
 *               otherwise.Failure occurs when:
 *                  -The memory fails to be allocated or freed
 *                   for icf_msg_st, that has to be sent to SM.
 *                  
 ***********************************************************************/
icf_return_t icf_sm_call_log_hdlr(
       INOUT    icf_sm_pdb_st *p_sm_pdb);


/*----------------------------------------------------------------
 * FUNCTION NAME:  icf_sm_timer_expiry_hdlr
 * 
 * DESCRIPTION:    This function performs the checks 
 *                 related to the 24 hour interval
 *                 of the license management scenario.
 *                
 * RETURNS :        SUCCESS: Incase all checks are successful
 * Also: what happens when the license expiry reached?
 *                  FAILURE: In the following scenarios:
 *                  -->Counter file not read.
 *                  -->DBM fails to give SM global data
 *                  -->Failed to get system time.
 *                  -->Failed to get the modify time of the license file.
 *                  -->Failed to write the updated license file.
 *                                   
 *
 * CALLED ON:       Expiration of 24 hour timer run by SM.
 *-----------------------------------------------------------------------*/
icf_return_t icf_sm_timer_expiry_hdlr(
        INOUT     icf_sm_pdb_st *p_sm_pdb);


/*------------------------------------------------------------------------------
 * Prototypes of functions in icf_sm_util.c
 *----------------------------------------------------------------------------*/


/*************************************************************************
 *     FUNCTION NAME: icf_sm_get_module_glb_data
 *
 *     DESCRIPTION : This function returns the address of data block for
 *                   the SM Module.Called by sm_check_trace/error to
 *                   avoid recursive calling when global data was
 *                   asked from DBM directly.
 *
 *     RETURNS     : Success if data is retreived successfully else failure     
 *
 *************************************************************************/
icf_return_t
icf_sm_get_module_glb_data(
        INOUT icf_glb_pdb_st        *p_glb_pdb,
        IN    icf_int_module_id_t   module_id,
        OUT   icf_void_t            **p_p_data);


#ifdef ICF_TRACE_ENABLE
/**********************************************************************
 * FUNCTION:    icf_sm_check_trace
 *
 * DESCRIPTION: This function is called by the function icf_cmn_trace
 *              to check whether the trace level and the type requested
 *              is enabled and is to be reported or not. 
 *
 * RETURNS:     ICF_SUCCESS to indicate that the received trace_id 
 *              can be reported.
 *              ICF_FAILURE to indicate:
 *                  -Received trace_id is INAVLID
 *                  -DBM failure to obtain SM Global data.
 *                  -Received trace_id cannot be reported.
 *                  
 *              
 ******************************************************/
icf_return_t icf_sm_check_trace(
       INOUT    icf_glb_pdb_st *p_glb_pdb,
       IN       icf_trace_id_t trace_id);



/**********************************************************************
 * FUNCTION:    icf_sm_report_trace
 *
 * DESCRIPTION: This function is called by the function icf_cmn_trace
 *              if the trace level and the type requested is enabled 
 *              and is to be reported. It sends the trace data to
 *              the SM.
 *
 * RETURNS:     ICF_SUCCESS in case of success and ICF_FAILURE
 *              otherwise.Failure occurs when:
 *                  -The memory fails to be allocated 
 *                   for icf_msg_st, that has to be sent to SM.
 *                  -SEND macro returned failure.
 *              
 ********************************************************************/
icf_return_t icf_sm_report_trace(
       INOUT    icf_glb_pdb_st *p_glb_pdb,
       IN       icf_trace_id_t trace_id,
       IN       icf_trace_data_st *p_trace_data);


/**********************************************************************
 * FUNCTION:    icf_sm_report_app_trace
 *
 * DESCRIPTION: This function is called by the function icf_cmn_trace
 *              if the trace criteria matches and the type requested is enabled
 *              and is to be reported. It sends the trace data to
 *              the SM.
 *
 * RETURNS:     ICF_SUCCESS in case of success and ICF_FAILURE
 *              otherwise.Failure occurs when:
 *                  -The memory fails to be allocated
 *                   for icf_msg_st, that has to be sent to SM.
 *                  -SEND macro returned failure.
 *
 ********************************************************************/
 icf_return_t icf_sm_report_app_trace(
              INOUT    icf_glb_pdb_st *p_glb_pdb,
              IN       icf_trace_id_t trace_id,
              IN       icf_trace_data_st *p_trace_data);
#endif
#ifdef ICF_ERROR_ENABLE
/**********************************************************************
 * FUNCTION:    icf_sm_check_error
 *
 * DESCRIPTION: This function is called by the function 
 *              icf_cmn_raise_error_n_trace
 *              to check whether the error level and the type requested
 *              is enabled and is to be reported or not. 
 *
 * RETURNS:     ICF_SUCCESS to indicate that the received error_id 
 *              can be reported.
 *              ICF_FAILURE to indicate:
 *                  -received error_id is out of range.
 *                  -Failure to obtain SM Global data.
 *                  -Received error_id cannot be reported.
 *              
 *********************************************************************/
icf_return_t icf_sm_check_error(
       INOUT    icf_glb_pdb_st *p_glb_pdb,
       IN       icf_error_id_t error_id);


/**********************************************************************
 * FUNCTION:    icf_sm_report_error
 * 
 * DESCRIPTION: This function is called by the function 
 *              icf_cmn_rasie_error_n_trace if the error level
 *              and the type requested is enabled and is to be reported.
 *              It sends the error_data to the SM.
 *
 * RETURNS:     ICF_SUCCESS in case of success and 
 *              ICF_FAILURE otherwise.Failure occurs when:
 *                  -The memory fails to be allocated 
 *                   for icf_msg_st, that has to be sent to SM.
 *                  -SEND Macro returned Failure.
 *              
 ***********************************************************************/
icf_return_t icf_sm_report_error(
        INOUT   icf_glb_pdb_st *p_glb_pdb,
        IN      icf_error_id_t error_id,
        IN      icf_error_data_st *p_error_data,
        INOUT   icf_error_t *p_ecode);
#endif

#ifdef ICF_STAT_ENABLED
/*********************************************************************
 * FUNCTION:    icf_sm_get_stats_index
 *
 * DESCRIPTION: This function is called by the function 
 *              icf_get_stats_hdlr() and icf_set_stats_hdlr()
 *              to retrieve the  of current stats group and the
 *              next stats group.
 *
 * RETURNS:     ICF_SUCCESS in case of success and
 *              ICF_FAILURE when the recd_stats_id is not a group id.
 *              
 *************************************************************/
icf_return_t icf_sm_get_stats_index(
        IN      icf_stats_id_t recd_stats_id,
        INOUT   icf_sm_stats_index_st *p_sm_stats_index);
#endif




/**************************************************************
 * FUNCTION:    icf_sm_log_call_details
 *
 * DESCRIPTION: This is a utility function invoked by the
 *                  -icf_sm_set_dialed_call()
 *                  -icf_sm_set_missed_call()
 *                  -icf_sm_set_recd_call()
 *              to log the call details into the SM Global
 *              data.
 *
 * RETURNS:     Nothing. 
 *                   
*************************************************************/
icf_void_t icf_sm_log_call_details(
        INOUT   icf_sm_log_call_details_st *p_call_details,
        INOUT   icf_glb_pdb_st  *p_glb_pdb,
        INOUT   icf_time_n_date_st *p_time_details,
        IN      icf_uint8_t  call_index);

/**************************************************************
 * FUNCTION:    icf_sm_set_dialed_call
 *
 * DESCRIPTION: This is a utility function invoked by the CC
 *              whenever the CREATE_CALL is received from the PA.
 *
 * RETURNS:     ICF_SUCCESS to indicate that the dialed call
 *              has been  logged properly. ICF_FAILURE is 
 *              returned when:
 *                  -DBM returns failure.
 *                   
*************************************************************/
icf_return_t icf_sm_set_dialed_call(
        INOUT   icf_glb_pdb_st *p_glb_pdb);


/**************************************************************
 * FUNCTION:    icf_sm_set_missed_call
 *
 * DESCRIPTION: This is a utility function invoked by the CC
 *              whenever the USER_ALERTED is received from the PA. 
 *              Once the CONNECT_REMOTE/TERMINATE_CALL is received 
 *              the call details are moved from missed_calls to recd_calls.
 *              
 * RETURNS:     ICF_SUCCESS to indicate that the missed call
 *              has been  logged properly. ICF_FAILURE is 
 *              returned when:
 *                  -DBM returns failure.
 *                   
*************************************************************/
icf_return_t icf_sm_set_missed_call(
        INOUT   icf_glb_pdb_st *p_glb_pdb);
/**************************************************************
 * FUNCTION:    icf_sm_copy_call_details
 *
 * DESCRIPTION: This is a utility function invoked by the
 *                  -icf_sm_remv_missed_call()
 *              to copy the call details from one member onto 
 *              another.
 *
 * RETURNS:     Nothing. 
 *                   
*************************************************************/
icf_void_t icf_sm_copy_call_details(
        INOUT   icf_sm_log_call_details_st *p_dest_call_details,
        INOUT   icf_sm_log_call_details_st *p_src_call_details);


#ifdef ICF_STAT_ENABLED
/**************************************************************
 * FUNCTION:    icf_sm_remv_missed_call
 * 
 * DESCRIPTION: This utility function is invoked by 
 *              icf_sm_set_recd_call() when it has
 *              recorded the call details in the sm_recd_calls[]
 *              list and now that call has to be removed from
 *              the sm_missed_calls[] list. 
 *              
 * RETURNS:     SUCCESS to indicate that the call was removed from
 *              missed_calls[]. FAILURE indicating that the call
 *              was not in the missed_calls[] list and hence not
 *              removed.
 * 
 *************************************************************/
icf_return_t icf_sm_remv_missed_call(
       INOUT    icf_glb_pdb_st *p_glb_pdb,
       IN       icf_call_id_t remv_call_id,
       INOUT    icf_sm_glb_data_st *p_sm_data);



/**************************************************************
 * FUNCTION:    icf_sm_set_recd_call
 *
 * DESCRIPTION: This is a utility function invoked by the CC
 *              whenever CONNECT_REMOTE/TERMINATE_CALL is 
 *              received from the PA, to log the call details in
 *              the received_call history. 
 *
 * RETURNS:     ICF_SUCCESS to indicate that the received call
 *              has been  logged properly. ICF_FAILURE is 
 *              returned when:
 *                  -DBM returns failure.
 *                   
*************************************************************/
icf_return_t icf_sm_set_recd_call(
        INOUT   icf_glb_pdb_st *p_glb_pdb);



/**************************************************************
 * FUNCTION:    icf_sm_fill_def_call_details
 *
 * DESCRIPTION: This is a utility function invoked by the
 *              icf_sm_call_log_hdlr() to fill the NULL
 *              call history into the response API for
 *              dialed/missed and received calls, when DBM
 *              failed to give the config data.
 *              It is also called by fill_call_details() to
 *              fill the elements which are extra than the 
 *              num_calls_to_log value.
 *
 * RETURNS:     Nothing.
 *                   
 *************************************************************/
icf_void_t icf_sm_fill_def_call_details(
        INOUT   icf_call_info_st *p_call_info,
        IN      icf_uint8_t  recd_call_index);


/**************************************************************
 * FUNCTION:    icf_sm_fill_call_details
 *
 * DESCRIPTION: This is a utility function invoked by the
 *              icf_sm_call_log_hdlr() to fill the 
 *              call history into the response API for
 *              dialed/missed and received calls.
 *
 * RETURNS:     Nothing. 
 *                   
*************************************************************/
icf_void_t icf_sm_fill_call_details(
        INOUT   icf_call_info_st *p_call_info,
        INOUT   icf_sm_log_call_details_st *p_logged_calls,
        IN      icf_uint8_t  logged_num_of_calls,
        IN      icf_uint8_t  recd_call_index,
        IN      icf_uint8_t  num_calls_to_log);
#endif /* #ifdef ICF_STAT_ENABLED */


#ifdef ICF_TRACE_ENABLE
/**********************************************************************
 *
 * FUNCTION:        icf_sm_trace
 *
 * DESCRIPTION:     This function checks the trace id and 
 *                  calls the macro for trace.
 *                     
 * RETURNS:         ICF_SUCCESS in case of success
 *                  and ICF_FAILURE in case of failure.
 *
 ******************************************************************/
icf_void_t icf_sm_trace(
        INOUT   icf_sm_pdb_st           *p_sm_pdb,
        IN      icf_trace_id_t           trace_id);


/**********************************************************************
 *
 * FUNCTION:        icf_sm_non_api_trace
 *
 * DESCRIPTION:     This function checks the trace id and 
 *                  calls the macro for trace.It receives the 
 *                  trace_id that are relevant to non_api
 *                  functions of SM.
 *                     
 * RETURNS:         ICF_SUCCESS in case of success
 *                  and ICF_FAILURE in case of failure.
 *
 ******************************************************************/
icf_void_t icf_sm_non_api_trace(
        INOUT   icf_glb_pdb_st           *p_glb_pdb,
        INOUT   icf_sm_glb_data_st       *p_sm_data,
        IN      icf_trace_id_t           trace_id);
#endif


#ifdef ICF_ERROR_ENABLE
/*******************************************************************************
 *
 * FUNCTION:        icf_sm_error
 *
 * DESCRIPTION:     This function checks the error id and calls the macro for
 *                  error reporting.
 *
 * RETURNS:         Nothing.
 *
 ******************************************************************************/
icf_void_t icf_sm_error(
        INOUT   icf_sm_pdb_st           *p_sm_pdb,
        IN      icf_error_id_t              ecode);
#endif


/*------------------------------------------------------------------------
 *          Fucntions to check parameter validity.
 *---------------------------------------------------------------------*/
#ifdef ICF_TRACE_ENABLE
/**********************************************************************
 *
 * FUNCTION:        icf_sm_validate_trace_details
 *
 * DESCRIPTION:     This function checks the trace details 
 *                  received in the API: SET_TRACE_LEVEL_REQ.
 *                     
 * RETURNS:         ICF_SUCCESS in case of trace details are valid,
 *                  and ICF_FAILURE in case invalid values have been given.
 *
 ******************************************************************/
icf_void_t icf_sm_validate_trace_details(
        INOUT   icf_sm_pdb_st                *p_sm_pdb,
        INOUT   icf_sm_cc_set_trace_level_req_st  *p_trace_details);
#endif

#ifdef ICF_ERROR_ENABLE
/**********************************************************************
 *
 * FUNCTION:        icf_sm_validate_error_details
 *
 * DESCRIPTION:     This function checks the error details 
 *                  received in the API: SET_ERROR_LEVEL_REQ.
 *                     
 * RETURNS:         ICF_SUCCESS in case of error details are valid,
 *                  and ICF_FAILURE in case invalid values have been given.
 *
 ******************************************************************/
icf_void_t icf_sm_validate_error_details(
        INOUT   icf_sm_pdb_st                       *p_sm_pdb,
        INOUT   icf_sm_cc_set_error_report_level_req_st  *p_error_details);
#endif

#ifdef ICF_STAT_ENABLE
icf_return_t icf_sm_stats_hdlr(
          INOUT    icf_sm_pdb_st  *p_sm_pdb);
#endif          

#ifdef ICF_STAT_ENABLED
/**********************************************************************
 *
 * FUNCTION:        icf_sm_validate_set_stats
 *
 * DESCRIPTION:     This function checks the stats details 
 *                  received in the API: SET_STATS_REQ.
 *                     
 * RETURNS:         ICF_SUCCESS in case of parameter values are valid,
 *                  and ICF_FAILURE in case invalid values have been given.
 *
 ******************************************************************/
icf_void_t icf_sm_validate_set_stats(
        INOUT   icf_sm_pdb_st          *p_sm_pdb,
        INOUT   icf_set_stats_req_st  *p_stats_details);

/**********************************************************************
 *
 * FUNCTION:        icf_sm_validate_get_stats
 *
 * DESCRIPTION:     This function checks the stats details 
 *                  received in the API: GET_STATS_REQ. 
 *                  Only the reset flag is checked here, because the
 *                  INVALID stats_id is to be reported in the response 
 *                  to this API.
 *                     
 * RETURNS:         ICF_SUCCESS in case of parameter values are valid,
 *                  and ICF_FAILURE in case invalid values have been given.
 *
 ******************************************************************/
icf_void_t icf_sm_validate_get_stats(
        INOUT   icf_sm_pdb_st          *p_sm_pdb,
        INOUT   icf_get_icf_stats_req_st  *p_stats_details);
#endif


/****************************************************************
 *      For License Management
 ******************************************************************/
#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* __ICF_SM_TYPES_H__ */  


