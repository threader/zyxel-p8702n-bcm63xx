/*******************************************************************************
 *
 * FILENAME         : icf_log_display.h
 *
 * DESCRIPTION      : This file defines the prototypes that are used for trace 
 *			display functions
 *
 * DATE         NAME                REFERENCE       REASON
 * -----------  -------------       ------------    --------------
 * 12-12-2007  vipul varshney      		      Initial version.
 *
 * Copyright (c) 2006, Aricent.
 ******************************************************************************/

#ifndef __IPTKAPP_CORE_LOG_H__
#define __IPTKAPP_CORE_LOG_H__

#include <time.h>
#include "icf_trace_error_types.h"
#include "iptkapp_cfg_types.h"

/*******************************************************************************
 *
 * FUNCTION NAME : iptkapp_trace_init
 *
 * DESCRIPTION   : This function initilize the trace settings
 *
 * RETURNS       : iptkapp_return_t
 *
 ******************************************************************************/

iptkapp_return_t iptkapp_trace_init(iptkapp_error_t *p_err);


/*******************************************************************************
 *
 * FUNCTION NAME : iptkapp_trace_enable_req
 *
 * DESCRIPTION   : The function is to enable traces
 *
 * RETURNS       : iptkapp_return_t
 *
 ******************************************************************************/

iptkapp_return_t iptkapp_trace_enable_req(iptkapp_error_t *p_err);


/*******************************************************************************
 *
 * FUNCTION NAME : iptkapp_core_trace_handler
 *
 * DESCRIPTION   : The function extracts payload data from message coming from iptk
 *
 * RETURNS       : iptkapp_return_t
 *
 ******************************************************************************/

iptkapp_return_t iptkapp_core_trace_handler(icf_msg_st *p_msg);


/*******************************************************************************
 *
 * FUNCTION NAME : iptkapp_trace_send_data_to_screen
 *
 * DESCRIPTION   : The function send the payload data of message received to a
 *                 log file and also to standard output
 *
 * RETURNS       : iptkapp_return_t
 *
 ******************************************************************************/
icf_uint8_t iptkapp_trace_send_data_to_screen (icf_report_trace_ind_st *);



/*******************************************************************************
 *
 * FUNCTION NAME : iptkapp_trace_send_data_to_file
 *
 * DESCRIPTION   : The function send the payload data of message received to a
 *                 log file and also to standard output
 *
 * RETURNS       : iptkapp_return_t
 *
 ******************************************************************************/

icf_uint8_t iptkapp_trace_send_data_to_file (icf_report_trace_ind_st *);

/*******************************************************************************
 *
 * FUNCTION NAME : iptkapp_trace_close_file
 *
 * DESCRIPTION   : The function closes the file pointer
 *
 * RETURNS       : iptkapp_return_t
 *
 ******************************************************************************/

icf_void_t iptkapp_trace_close_file (icf_void_t);

/*******************************************************************************
 *
 * FUNCTION NAME : iptkapp_trace_open_file
 *
 * DESCRIPTION   : The function opens the log file
 *
 * RETURNS       : iptkapp_return_t
 *
 ******************************************************************************/

icf_return_t iptkapp_trace_open_file (icf_void_t);

#endif
