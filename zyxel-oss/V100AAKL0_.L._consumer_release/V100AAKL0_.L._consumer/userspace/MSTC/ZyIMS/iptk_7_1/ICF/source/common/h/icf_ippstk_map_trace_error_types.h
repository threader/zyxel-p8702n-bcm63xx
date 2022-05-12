/******************************************************************************
*									
* FILE NAME    :icf_icf_map_trace_error_types.h				
*									
* DESCRIPTION  : System management interface data structures other than message 
*                definiton.
*									
* Revision History :							
*    DATE            NAME             REFERENCE       REASON
* --------      ------------          ----------   ---------------
*  09-Aug-2005  Rohit Aggarwal						Initial
*  10-Mar-2006  Amit Sharma                         ICF merging with ICF2.0
*   									
* Copyright 2006,Aricent.   				
*									
******************************************************************************/

#ifndef __ICF_ICF_MAP_TRACE_ERROR_TYPES_H__
#define __ICF_ICF_MAP_TRACE_ERROR_TYPES_H__

#include "icf_trace_error_types.h"
#ifdef ICF_STAT_ENABLED
#include "icf_stats_defs.h"
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

    
#ifdef ICF_TRACE_ENABLE

/*-----------------------------------------------------------------------------
 * 
 * typedef Name: icf_trace_level_t
 *
 * Description: This is used to define the level for the trace set using the 
 *               API ICF_SM_CC_SET_TRACE_LEVEL.
 *  
 *---------------------------------------------------------------------------*/

typedef icf_trace_level_t icf_trace_level_t;


/*-----------------------------------------------------------------------------
 *
 *   values for trace_level field in icf_sm_cc_set_trace_level_req_st
 *
 *---------------------------------------------------------------------------*/

#define ICF_TRACE_LEVEL_UNDEFINED     ICF_TRACE_LEVEL_UNDEFINED
#define ICF_TRACE_LEVEL_BRIEF         ICF_TRACE_LEVEL_BRIEF
#define ICF_TRACE_LEVEL_DETAILED      ICF_TRACE_LEVEL_DETAILED


/*-----------------------------------------------------------------------------
 * 
 * typedef Name: icf_trace_type_t
 *
 * Description: This is used to define the type of trace to be reported which 
 *              is set using the API ICF_SM_CC_SET_TRACE_LEVEL
 *  
 *---------------------------------------------------------------------------*/

typedef icf_trace_type_t icf_trace_type_t; 


/*-----------------------------------------------------------------------------
 * 
 * typedef Name: icf_trace_id_t
 *
 * Description: This is used to define the identifier for the trace reported 
 *              in the API ICF_CC_SM_REPORT_TRACE
 * 
 *---------------------------------------------------------------------------*/

typedef icf_trace_id_t icf_trace_id_t;


/*-----------------------------------------------------------------------------
 * 
 * Structure Name: icf_trace_data_st
 *
 * Description: This data type is used to define a structure, which contains 
 *              trace data reported in the API ICF_CC_SM_REPORT_TRACE. 
 *---------------------------------------------------------------------------*/

typedef icf_trace_data_st icf_trace_data_st;



/*-----------------------------------------------------------------------------
 * 
 * Structure Name: icf_sm_cc_set_trace_level_req_st
 *
 * Description: This is payload of API ICF_SM_CC_SET_TRACE_LEVEL_REQ.
 *              This API is sent by system administration module to enable or  
 *              disable the reporting of traces of a specific level or type,
 *              during system runtime.
 *
 *---------------------------------------------------------------------------*/

typedef icf_set_trace_level_req_st icf_sm_cc_set_trace_level_req_st;


/*-----------------------------------------------------------------------------
 * 
 * Structure Name: icf_cc_sm_report_trace_ind_st
 *
 * Description: This is payload of API ICF_CC_SM_REPORT_TRACE_IND. 
 *              This API is sent by call control to report a trace of given
 *              level and type. The API can report upto 4 integer and 2 string  
 *              values.
 *
 *---------------------------------------------------------------------------*/

typedef icf_report_trace_ind_st icf_cc_sm_report_trace_ind_st;


/*-----------------------------------------------------------------------------
 *
 *   values for icf_trace_type_t : type of traces to be reported.
 *
 *---------------------------------------------------------------------------*/

#define ICF_TRACE_TYPE_UNDEFINED                 ICF_TRACE_TYPE_UNDEFINED
#define ICF_TRACE_TYPE_SYSTEM                    ICF_TRACE_TYPE_SYSTEM
#define ICF_TRACE_TYPE_INTERFACE                 ICF_TRACE_TYPE_INTERFACE
#define ICF_TRACE_TYPE_SIP                       ICF_TRACE_TYPE_SIP
#define ICF_TRACE_TYPE_CALL_CONTROL              ICF_TRACE_TYPE_CALL_CONTROL
#define ICF_TRACE_TYPE_MEDIA                     ICF_TRACE_TYPE_MEDIA
#define ICF_TRACE_TYPE_CONFIG                    ICF_TRACE_TYPE_CONFIG
#define ICF_TRACE_TYPE_REGISTRATION              ICF_TRACE_TYPE_REGISTRATION
#define ICF_TRACE_TYPE_SM                        ICF_TRACE_TYPE_SM
#define ICF_TRACE_TYPE_CALL_HOLD_RESUME          ICF_TRACE_TYPE_CALL_HOLD_RESUME
#define ICF_TRACE_TYPE_THREE_WAY_CALL            ICF_TRACE_TYPE_THREE_WAY_CALL
#define ICF_TRACE_TYPE_CALL_TRANSFER_UNATTENDED  ICF_TRACE_TYPE_CALL_TRANSFER_UNATTENDED
#define ICF_TRACE_TYPE_CALL_TRANSFER_ATTENDED    ICF_TRACE_TYPE_CALL_TRANSFER_ATTENDED
#define ICF_TRACE_TYPE_REFER                     ICF_TRACE_TYPE_REFER
#define ICF_TRACE_TYPE_CALL_FORWARDING           ICF_TRACE_TYPE_CALL_FORWARDING
#define ICF_TRACE_TYPE_CALL_WAITING              ICF_TRACE_TYPE_CALL_WAITING
#define ICF_TRACE_TYPE_GENERAL                   ICF_TRACE_TYPE_GENERAL
#define ICF_TRACE_TYPE_SSA                       ICF_TRACE_TYPE_SSA
#define ICF_TRACE_TYPE_GENERAL                   ICF_TRACE_TYPE_GENERAL
#define ICF_TRACE_TYPE_SIP_STACK                 ICF_TRACE_TYPE_SIP_STACK
#define ICF_TRACE_TYPE_SIP_STACK_DEBUG           ICF_TRACE_TYPE_SIP_STACK_DEBUG

#endif /* #ifdef ICF_TRACE_ENABLE */


#ifdef ICF_ERROR_ENABLE

/*-----------------------------------------------------------------------------
 * 
 * typedef Name: icf_error_level_t
 *
 * Description: This is used to define the level for the  error set using the 
 *              API ICF_SM_CC_SET_ERROR_REPORT_LEVEL
 *  
 *---------------------------------------------------------------------------*/

typedef icf_error_level_t icf_error_level_t;


/*-----------------------------------------------------------------------------
 *
 *   values for icf_error_level_t : level of the errors to be reported.
 *
 *---------------------------------------------------------------------------*/

#define ICF_ERROR_LEVEL_UNDEFINED     ICF_ERROR_LEVEL_UNDEFINED
#define ICF_ERROR_LEVEL_CRITICAL      ICF_ERROR_LEVEL_CRITICAL
#define ICF_ERROR_LEVEL_MAJOR         ICF_ERROR_LEVEL_MAJOR
#define ICF_ERROR_LEVEL_INFO          ICF_ERROR_LEVEL_INFO


/*-----------------------------------------------------------------------------
 * 
 * typedef Name: icf_error_type_t
 *
 * Description: This is used to define the type of error to be reported which is
 *              set using the API ICF_SM_CC_SET_ERROR_REPORT_LEVEL
 *  
 *---------------------------------------------------------------------------*/

typedef icf_error_type_t icf_error_type_t; 


/*-----------------------------------------------------------------------------
 * 
 * typedef Name: icf_error_id_t
 *
 * Description: This is used to define the identifier for the error reported in
 *              the API ICF_CC_SM_REPORT_ERROR.
 *  
 *---------------------------------------------------------------------------*/

typedef icf_error_id_t icf_error_id_t; 

/*-----------------------------------------------------------------------------
 * 
 * Structure Name: icf_error_data_st
 *
 * Description: This data type is used to define a structure, which contains 
 *              error data reported in the API ICF_CC_SM_REPORT_ERROR
 * 
 *---------------------------------------------------------------------------*/

typedef icf_error_data_st icf_error_data_st;


/*-----------------------------------------------------------------------------
 *
 *   values for icf_error_type_t: Type of errors to be reported.
 *   These can be system, interface, SIP, media, call control etc.
 *
 *---------------------------------------------------------------------------*/

#define ICF_ERROR_TYPE_UNDEFINED                 ICF_ERROR_TYPE_UNDEFINED
#define ICF_ERROR_TYPE_SYSTEM                    ICF_ERROR_TYPE_SYSTEM
#define ICF_ERROR_TYPE_INTERFACE                 ICF_ERROR_TYPE_INTERFACE
#define ICF_ERROR_TYPE_SIP                       ICF_ERROR_TYPE_SIP
#define ICF_ERROR_TYPE_CALL_CONTROL              ICF_ERROR_TYPE_CALL_CONTROL
#define ICF_ERROR_TYPE_MEDIA                     ICF_ERROR_TYPE_MEDIA
#define ICF_ERROR_TYPE_CONFIG                    ICF_ERROR_TYPE_CONFIG
#define ICF_ERROR_TYPE_REGISTRATION              ICF_ERROR_TYPE_REGISTRATION
#define ICF_ERROR_TYPE_SM                        ICF_ERROR_TYPE_SM
#define ICF_ERROR_TYPE_CALL_HOLD_RESUME          ICF_ERROR_TYPE_CALL_HOLD_RESUME
#define ICF_ERROR_TYPE_THREE_WAY_CALL            ICF_ERROR_TYPE_THREE_WAY_CALL
#define ICF_ERROR_TYPE_CALL_TRANSFER_UNATTENDED  ICF_ERROR_TYPE_CALL_TRANSFER_UNATTENDED
#define ICF_ERROR_TYPE_CALL_TRANSFER_ATTENDED    ICF_ERROR_TYPE_CALL_TRANSFER_ATTENDED
#define ICF_ERROR_TYPE_REFER                     ICF_ERROR_TYPE_REFER
#define ICF_ERROR_TYPE_CALL_FORWARDING           ICF_ERROR_TYPE_CALL_FORWARDING
#define ICF_ERROR_TYPE_CALL_WAITING              ICF_ERROR_TYPE_CALL_WAITING
#define ICF_ERROR_TYPE_GENERAL                   ICF_ERROR_TYPE_GENERAL

#endif /* #ifdef ICF_ERROR_ENABLE */


#if defined ICF_TRACE_ENABLE || ICF_ERROR_ENABLE

/*-----------------------------------------------------------------------------
 *
 *   values to be used in field number_of_integer_values of error_data and 
 *   trace_data.
 *
 *---------------------------------------------------------------------------*/

#define ICF_NO_INT_PRESENT           ICF_NO_INT_PRESENT
#define ICF_ONE_INT_PRESENT          ICF_ONE_INT_PRESENT
#define ICF_TWO_INT_PRESENT          ICF_TWO_INT_PRESENT
#define ICF_THREE_INT_PRESENT        ICF_THREE_INT_PRESENT
#define ICF_FOUR_INT_PRESENT	     ICF_FOUR_INT_PRESENT


/*-----------------------------------------------------------------------------
 *
 *   values to be used in field number_of_string_values of error_data and 
 *   trace_data.
 *
 *---------------------------------------------------------------------------*/

#define ICF_NO_STRING_PRESENT        ICF_NO_STRING_PRESENT
#define ICF_ONE_STRING_PRESENT       ICF_ONE_STRING_PRESENT
#define ICF_TWO_STRING_PRESENT       ICF_TWO_STRING_PRESENT

#endif


#ifdef ICF_STAT_ENABLED
/*-----------------------------------------------------------------------------
 *
 * typedef Name: icf_stats_id_t
 *
 * Description: This is used to define the identifier for the stats reported in
 *              the API ICF_CC_SM_GET_STATS_RESP.
 *
 *---------------------------------------------------------------------------*/

typedef icf_stats_id_t icf_stats_id_t;

#endif /* #ifdef ICF_STAT_ENABLED */

#ifdef ICF_ERROR_ENABLE
typedef icf_set_error_report_level_req_st icf_sm_cc_set_error_report_level_req_st;
#endif

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* __ICF_ICF_MAP_TRACE_ERROR_TYPES_H__ */
