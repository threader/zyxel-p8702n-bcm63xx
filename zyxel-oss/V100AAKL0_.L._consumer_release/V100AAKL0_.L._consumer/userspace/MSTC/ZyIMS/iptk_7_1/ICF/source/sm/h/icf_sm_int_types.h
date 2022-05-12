/*------------------------------------------------------------------------------
 *
 * File name        : icf_sm_int_types.h
 *
 * Purpose          : This file contains the data structures used by SM.
 *
 * Revision History :
 *
 * Date         Name            	Ref#        Comments
 * --------     ------------    	----        ---------------
 * 09-Mar-2005  Jyoti Narula    	SM LLD	    Initial
 * 19-Oct-2006  Aman Arora         ICF REl 6.1.1  porting file to Symbian
 *
 * Copyright (c) 2006, Aricent.
 *----------------------------------------------------------------------------*/


#ifndef __ICF_SM_INT_TYPES_H__
#define __ICF_SM_INT_TYPES_H__

#include "icf_types.h"
#include "icf_common_types.h"
#include "icf_common_interface_types.h"
#include "icf_sm_defs.h"

#if (defined(ICF_PORT_SOLARIS) || defined(ICF_PORT_LINUX) || defined (ICF_PORT_SYMBIAN))
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#endif

#if (defined(ICF_PORT_WINDOWS))
#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>
#include <time.h>
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





/*------------------------------------------------------------------------------
 *
 * TYPE NAME   :  icf_sm_pdb_st
 * 
 * DESCRIPTION :  Structure for SM module's pdb. This is filled in by the SM 
 *                and used by SM to process the API's received.
 *
 *----------------------------------------------------------------------------*/
typedef struct
{
    /* The process data structure for SM that will be used 
     * across the processing of the APIs to be handled by SM Handler. */

    icf_internal_msg_st *p_internal_msg;
    icf_glb_pdb_st *p_glb_pdb;
    
    /* To store the event mapped for the API received. */
    icf_uint8_t curr_event;

    /* Pointer to error_code */
    icf_error_t *p_ecode;

    /* Boolean Flag to indicate whether the parameters received in
     * call are valid or not. */
    icf_boolean_t param_validity;

    /* Pointer to the payload of sent/received API */
    icf_void_t *p_api_payload;

    /* Pointer to sm_global_data */
    struct icf_sm_glb_data_st *p_sm_data;


}icf_sm_pdb_st;



/*------------------------------------------------------------------------------
 *
 * TYPEDEF NAME :  icf_sm_api_hdlr_ptr_t
 * 
 * DESCRIPTION  :  Defines the function pointer to be used for API-Handlers.
 *
 *----------------------------------------------------------------------------*/

typedef icf_return_t (*icf_sm_api_hdlr_ptr_t) (
                INOUT icf_sm_pdb_st *p_sm_pdb);



/*------------------------------------------------------------------------------
 *
 * TYPE NAME   :  icf_sm_log_call_details_st
 * 
 * DESCRIPTION :  This structure stores the logged details of the calls that
 *                are either dialed/missed/received.
 *
 *----------------------------------------------------------------------------*/

typedef struct
{
    icf_address_st       remote_party_addr;
    icf_time_n_date_st   time_stamp;
    icf_call_id_t        call_id;
    
}icf_sm_log_call_details_st;


/*------------------------------------------------------------------------------
 *
 * TYPE NAME   :  icf_sm_glb_data_st
 *
 * DESCRIPTION :  The global data structure for SM that will be stored
 *                in module_data[] of p_glb_data of glb_pdb.
 *                For CALL LOGGING arrays are used to store the details 
 *                of calls.The implementation is of a circular list, 
 *                hence the moving index being used here.
 *                The count for each category stores the current 
 *                number of call history. 
 *                
 *----------------------------------------------------------------------------*/

typedef struct 
{

#ifdef ICF_TRACE_ENABLE
    /* Trace details as given by the SM to the toolkit. */
    icf_boolean_t trace_status;
    icf_trace_type_t trace_type;
    icf_trace_level_t trace_level;
    icf_trace_criteria_t trace_criteria;
    icf_trace_rule_t  trace_rule;
#endif

#ifdef ICF_ERROR_ENABLE

    /* Error details as given by the SM to the toolkit. */
    icf_boolean_t error_status;
    icf_error_type_t error_type;
    icf_error_level_t error_level;
#endif

    /* Function Pointer Array needed to map received SM events
     * to API-handlers. */
    icf_sm_api_hdlr_ptr_t
             api_hdlr_list[ICF_SM_MAX_EVENT_COUNT];

    /* To store the num_calls_to_log value 
     * when it has value 0, it indicates that
     * the call logging is disabled.*/
    icf_uint8_t   num_calls_to_log;
        
    /* Members needed for dialed calls history. */
    icf_sm_log_call_details_st   *p_dialed_calls;
    icf_uint8_t                  dialed_calls_index;
    icf_uint8_t                  num_dialed_calls;

    /* Members needed for received calls history. */
    icf_sm_log_call_details_st   *p_recd_calls;
    icf_uint8_t                  recd_calls_index;
    icf_uint8_t                  num_recd_calls;

    /* Members needed for missed calls history. */
    icf_sm_log_call_details_st   *p_missed_calls;
    icf_uint8_t                  missed_calls_index;
    icf_uint8_t                  num_missed_calls;
    
    
}icf_sm_glb_data_st;


#ifdef ICF_STAT_ENABLED
/*-------------------------------------------------------------------
 *
 * TYPE NAME   :  icf_sm_stats_index_st
 *
 * DESCRIPTION :  This structure is used to get the value of
 *                the group_stats_id for current and next group.
 *                These values are used to set/get
 *                the stats values for all the members of
 *                current stat_group.
 * 
 * ----------------------------------------------------------------*/

typedef struct
{
    icf_stats_id_t curr_stats_group;
    icf_stats_id_t next_stats_group;
    
}icf_sm_stats_index_st;
#endif


#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* __ICF_SM_TYPES_H__ */  


