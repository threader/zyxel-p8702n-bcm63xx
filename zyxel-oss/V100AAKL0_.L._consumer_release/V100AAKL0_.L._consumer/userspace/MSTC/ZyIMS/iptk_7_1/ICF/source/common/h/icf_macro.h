/*******************************************************************************
 *
 * 	FILENAME	: 	icf_macro.h
 *
 * 	DESCRIPTION	:	This file contains the macros used globally
 * 					across all modules of ICF.
 * 					
 * 	Revision History	:
 * 	DATE		NAME				REFERENCE		REASON
 * 	----------- ------------------  --------------  ------------
 *  03-Nov-2004 R. Ezhirpavai       ICF LLD      Initial version.
 *  25-Dec-2004 Pankaj, Rohit                       Insure compilation
 *  29-Dec-2004 Aashish Suchdev                     Added the macro for stats
 *  06-Jan-2005 Aashish Suchdev                     Updated the call to 
 *                                                  start/stop timers
 *  14-Jan-2005 Rohit Aggarwal                      Insure compilation typecast
 *  24-Jan-2005 Pankaj Negi                         Added macro to get 32 bit integer
 *  29-May-2005 Amit Sharma         SPR:7623        p_ecode is assigned 
 *  30-Aug-2005 Rohit Aggarwal                      Function entry/exit traces 
 *	12-Dec-2005 Ashutos Rath		ICF 4.0			New macros for setting and resetting bitmasks.
 *  14-Dec-2005 Rohit Aggarwal                      Added macro for alarms
 *  2-Jan-2007  Priyanka Dua        ICF 6.3         Added macro for multiple answer
 *  9-Jul-2007  Neha Choraria       ICF 7.0         Added macro for multiple answer
 *                                                  disabled.
 *
 * 	Copyright 2006, Aricent.
 ******************************************************************************/

#ifndef __ICF_MACRO_H__
#define __ICF_MACRO_H__

#include "icf_port_defs.h"
/******************** STANDARD INCLUDE FILES *****************************/

/******************** USER INCLUDE FILES *********************************/
#include "icf_common_prototypes.h"
#include "icf_port_prototypes.h"
#include "icf_common_types.h"
#include "icf_common_defs.h"
#ifdef ICF_STAT_ENABLE
#include "icf_stats_types.h"
#endif

/* Determine if a C++ compiler is being used.
 * If so, ensure that standard
 * C is used to process the header information.
 *   */

#ifdef __cplusplus

/* Yes, C++ compiler is present.
 * Use standard C.
 * */

extern "C"
{
#endif /* __cplusplus */

/******************** MACROS *********************************************/ 

#if (defined(ICF_DEBUG) && (defined(ICF_PORT_LINUX) || defined(ICF_PORT_VXWORKS)))
#define ICF_DEBUG_PRINT(str) printf str
#else
#define ICF_DEBUG_PRINT(str)
#endif


    
/* ---------------------------------------------------------------------
 * Macro to raise error/trace whenever a ecode is set.   
 * ---------------------------------------------------------------------*/
#ifndef ICF_ERROR_ENABLE
#define ICF_SET_ECODE(p_glb_pdb, p_error_data, ecode, p_ecode)      \
{                                                                      \
} /* End of set ecode macro */
#else
#define ICF_SET_ECODE(p_glb_pdb, p_error_data, ecode, p_ecode) \
{ \
	*p_ecode = (icf_error_t)(ecode); \
}
#endif
#ifdef ICF_ERROR_ENABLE
#define ICF_SET_ERROR(p_glb_pdb, p_error_data, ecode, p_ecode)      \
{                                                                      \
   icf_cmn_raise_error_n_trace(p_glb_pdb, p_ecode, (icf_error_t)ecode, \
           (icf_error_data_st *)p_error_data);  \
} /* End of set ecode macro */
#else
#define ICF_SET_ECODE(p_glb_pdb, p_error_data, ecode, p_ecode) \
{ \
	*p_ecode = (icf_error_t)(ecode); \
}
#endif

/* ---------------------------------------------------------------------  
 * Macro to log the trace as and when required.   
 * ---------------------------------------------------------------------*/

#ifdef ICF_TRACE_ENABLE
#define ICF_TRACE(p_glb_pdb, trace_id, p_trace_data_trace)               \
{                                                                     \
    icf_cmn_trace(p_glb_pdb, (icf_trace_id_t)trace_id, \
            p_trace_data_trace);    \
} /* End of trace logging macro */
#else
#define ICF_TRACE(p_glb_pdb, trace_id, p_trace_data_trace)
#endif /* End of if trace enabled */


#ifdef ICF_TRACE_ENABLE
#define ICF_APP_TRACE(p_glb_pdb, trace_id, p_trace_data_trace,p_criteria_data)               \
{                                                                     \
    icf_cmn_app_trace(p_glb_pdb, (icf_trace_id_t)trace_id, \
            p_trace_data_trace,p_criteria_data);  \
} /* End of trace logging macro */
#else
#define ICF_APP_TRACE(p_glb_pdb, trace_id, p_trace_data_trace)
#endif

#ifdef ICF_TRACE_ENABLE
/*------------------------------------------------------------------------------
 *
 * Macro Name       : ICF_CMN_APP_TRACE
 * Description      : New Macro for trace handling
 *
 *----------------------------------------------------------------------------*/
#define ICF_CMN_APP_TRACE(p_glb_pdb, trace_id,p_msg_to_be_traced)   \
{\
    icf_cmn_capture_app_trace(p_glb_pdb, (icf_trace_id_t)(trace_id), p_msg_to_be_traced);\
}
#else
#define ICF_CMN_APP_TRACE(p_glb_pdb, trace_id,p_msg_to_be_traced)
#endif

/* ---------------------------------------------------------------------
 *
 * Macro to raise the trace for a function entry.
 *
 * ---------------------------------------------------------------------*/


#ifdef ICF_TRACE_ENABLE
/*#if defined  (ICF_PORT_WINDOWS) || defined (ICF_PORT_SYMBIAN)*/
#if defined  ICF_PORT_WINDOWS && !defined (ICF_PORT_WINCE )
#define ICF_FUNCTION_ENTER(p_glb_pdb)                                 \
{                                                                        \
	icf_trace_data_st   trace_data_func_ent;                       \
       trace_data_func_ent.num_of_integer_values = ICF_ONE_INT_PRESENT;\
       trace_data_func_ent.number_of_string_values =                       \
                                            ICF_TWO_STRING_PRESENT;   \
       icf_port_strcpy(trace_data_func_ent.string_val[0].str,\
               (icf_uint8_t *)"MODULE FUNCTION ENTERED"); \
       trace_data_func_ent.string_val[0].str_len = (icf_uint16_t)                       \
              icf_port_strlen(trace_data_func_ent.string_val[0].str); \
	   icf_port_strcpy(trace_data_func_ent.string_val[1].str,\
               (icf_void_t *)__FILE__); \
       trace_data_func_ent.string_val[1].str_len =(icf_uint16_t)                        \
              icf_port_strlen(trace_data_func_ent.string_val[1].str); \
	   trace_data_func_ent.int_val[0] = __LINE__ ; \
       ICF_TRACE( p_glb_pdb, ICF_TRACE_FUNCTION_ENTRY, \
               &trace_data_func_ent)\
} /* Macro to raise the trace for function entry */
#elif defined( ICF_PORT_SYMBIAN )  ||  defined( ICF_PORT_WINCE )
#define ICF_FUNCTION_ENTER(p_glb_pdb)
#else
#define ICF_FUNCTION_ENTER(p_glb_pdb)                                 \
{                                                                        \
	icf_trace_data_st   trace_data_func_ent;                       \
       trace_data_func_ent.num_of_integer_values = ICF_NO_INT_PRESENT;\
       trace_data_func_ent.number_of_string_values =                     \
                                            ICF_ONE_STRING_PRESENT;   \
      icf_port_strcpy(trace_data_func_ent.string_val[0].str,\
               (icf_void_t *)__func__); \
       trace_data_func_ent.string_val[0].str_len =                       \
              icf_port_strlen(trace_data_func_ent.string_val[0].str); \
       ICF_TRACE( p_glb_pdb, ICF_TRACE_FUNCTION_ENTRY, \
               &trace_data_func_ent)\
} /* Macro to raise the trace for function entry */
#endif /* #ifdef ICF_PORT_WINDOWS */
#else /* #ifdef ICF_TRACE_ENABLE */
#define ICF_FUNCTION_ENTER(p_glb_pdb)
#endif /* #ifdef ICF_TRACE_ENABLE */

/* ---------------------------------------------------------------------
 *
 * Macro to raise the trace for a function entry.
 *
 * ---------------------------------------------------------------------*/

#ifdef ICF_TRACE_ENABLE
/*#if defined  (ICF_PORT_WINDOWS) || defined (ICF_PORT_SYMBIAN)*/
#if defined  ICF_PORT_WINDOWS  && !defined (ICF_PORT_WINCE)
#define ICF_FUNCTION_EXIT(p_glb_pdb)                                 \
{                                                                        \
       icf_trace_data_st   trace_data_func_ext;                        \
       trace_data_func_ext.num_of_integer_values = ICF_TWO_INT_PRESENT; \
       trace_data_func_ext.number_of_string_values =                      \
                                              ICF_TWO_STRING_PRESENT;  \
       trace_data_func_ext.int_val[0] = ret_val;                          \
	   icf_port_strcpy(trace_data_func_ext.string_val[0].str,          \
               (icf_uint8_t*)"MODULE FUNCTION EXITED");                                \
       trace_data_func_ext.string_val[0].str_len = (icf_uint16_t)                        \
               icf_port_strlen(trace_data_func_ext.string_val[0].str); \
	   icf_port_strcpy(trace_data_func_ext.string_val[1].str,\
               (icf_void_t *)__FILE__); \
       trace_data_func_ext.string_val[1].str_len = (icf_uint16_t)                      \
              icf_port_strlen(trace_data_func_ext.string_val[1].str); \
	   trace_data_func_ext.int_val[1] = __LINE__; \
	   ICF_TRACE( p_glb_pdb, ICF_TRACE_FUNCTION_EXIT, \
               &trace_data_func_ext)\
} /* Macro to raise the trace for function entry */
#elif defined ( ICF_PORT_SYMBIAN ) || defined ( ICF_PORT_WINCE )
#define ICF_FUNCTION_EXIT(p_glb_pdb)
#else
#define ICF_FUNCTION_EXIT(p_glb_pdb)                                   \
{                                                                         \
       icf_trace_data_st   trace_data_func_ext;                        \
       trace_data_func_ext.num_of_integer_values = ICF_ONE_INT_PRESENT; \
       trace_data_func_ext.number_of_string_values =                      \
                                              ICF_ONE_STRING_PRESENT;  \
       trace_data_func_ext.int_val[0] = ret_val;                          \
       icf_port_strcpy(trace_data_func_ext.string_val[0].str,          \
               (icf_void_t *)__func__);                                \
       trace_data_func_ext.string_val[0].str_len =                        \
               icf_port_strlen(trace_data_func_ext.string_val[0].str); \
       ICF_TRACE(p_glb_pdb, ICF_TRACE_FUNCTION_EXIT,                \
               &trace_data_func_ext)                                      \
} /* Macro to raise the trace for function exit */
#endif /* #ifdef ICF_PORT_WINDOWS */
#else /* #ifdef ICF_TRACE_ENABLE */
#define ICF_FUNCTION_EXIT(p_glb_pdb)
#endif /* #ifdef ICF_TRACE_ENABLE */

/* ---------------------------------------------------------------------
 * Macro to allocate the memory. If the memget fails, then this raises the
 * error 
 * ---------------------------------------------------------------------*/

#define ICF_MEMGET(p_glb_pdb, size, pool_id,p_buff, errhndlr, p_ecode, ret_val) \
{ \
   icf_uint8_t err = errhndlr; \
   p_buff = icf_cmn_memget(p_glb_pdb, (icf_uint32_t)size, \
               (icf_pool_id_t)pool_id, p_ecode); \
   if (ICF_NULL == p_buff) \
   { \
       ret_val = ICF_FAILURE; \
       if (ICF_RET_ON_FAILURE == err) \
       { \
           ICF_FUNCTION_EXIT(p_glb_pdb)                               \
           return (ret_val); \
       } \
   } \
   else \
   { \
       ret_val = ICF_SUCCESS; \
   } \
} /* End of ICF_MEMGET macro */

/* ---------------------------------------------------------------------
 *  Macro to free the memory. This also makes the buffer pointer as null. 
 * ---------------------------------------------------------------------*/

#define ICF_MEMFREE(p_glb_pdb, p_buff, pool_id, p_ecode, ret_val)                \
{ \
    if (ICF_NULL != p_buff) \
    { \
       /* Call icf memory free function */ \
       ret_val = icf_cmn_memfree(p_glb_pdb, p_buff, \
                   (icf_pool_id_t)pool_id, p_ecode); \
       p_buff = ICF_NULL; \
    } \
} /* End of ICF_MEMFREE macro */

/* ---------------------------------------------------------------------  
 * Macro for assertion. Note that this always returns failure in case of
 * failure, as this is a failure that should not occur in field and has
 * to be corrected because of internal handling. the return is put just so
 * that the subsequent statement may not result into coredumps.
 * ---------------------------------------------------------------------*/

#ifdef ICF_ASSERT_ENABLE
#define ICF_ASSERT(p_glb_pdb, condition)                             \
{                                                                       \
   if((condition)) {}                                                   \
   else                                                                 \
   {                                                                    \
     icf_trace_data_st     trace_data_assert;                        \
     trace_data_assert.int_val[0] = __LINE__;                           \
     icf_port_strcpy(trace_data_assert.string_val[0].str,            \
             (icf_void_t *)__FILE__);  \
     trace_data_assert.string_val[0].str_len =                          \
               icf_port_strlen(trace_data_assert.string_val[0].str); \
     trace_data_assert.num_of_integer_values = ICF_ONE_INT_PRESENT;  \
     trace_data_assert.number_of_string_values = ICF_ONE_STRING_PRESENT;  \
     ICF_TRACE(p_glb_pdb, (ICF_TRACE_ASSERT_FAILED), \
            &trace_data_assert) \
     ret_val = ICF_FAILURE; \
     ICF_FUNCTION_EXIT(p_glb_pdb)                                    \
     return ICF_FAILURE;                                             \
   }                                                                    \
} /* End of Assert condition checking macro */
#else
#define ICF_ASSERT(p_glb_pdb, condition)
#endif

/* ---------------------------------------------------------------------  
 * This macro is used to get memory from the system rather than from 
 * memory manager. This is used for those memory allocations that would 
 * be allocated during initialization and would not get freed during the
 * course of execution.   
 * ---------------------------------------------------------------------*/

#define ICF_STATIC_MEMGET(p_glb_pdb,size,p_buff,errhndlr,p_ecode,ret_val)\
{ \
	icf_uint16_t err = errhndlr; \
   p_buff = icf_cmn_static_memget(p_glb_pdb,(icf_uint32_t)size, p_ecode); \
   if (ICF_NULL == p_buff) \
   { \
       ret_val = ICF_FAILURE; \
       if (ICF_RET_ON_FAILURE == err) \
       { \
           ICF_FUNCTION_EXIT(p_glb_pdb)                               \
           return (ret_val); \
       } \
   } \
   else \
   { \
       ret_val = ICF_SUCCESS; \
   } \
} /* End of static memget */

/* ---------------------------------------------------------------------
 *  This Macro frees the static buffer back to the system buffer. It should
 *  be taken care that this should be called for freeing only those buffers
 *  that were allocated using ICF_STATIC_MEMGET. 
 *  This also makes the buffer pointer as null after freeing the buffer. 
 * ---------------------------------------------------------------------*/

#define ICF_STATIC_MEMFREE(p_glb_pdb, p_buff, p_ecode, ret_val)          \
{ \
    if (ICF_NULL != p_buff) \
    { \
       /* Call icf memory free function */ \
       ret_val = icf_cmn_static_memfree(p_glb_pdb,p_buff, p_ecode); \
       p_buff = ICF_NULL; \
    } \
} /* End of ICF_STATIC_MEMFREE macro */

/* ---------------------------------------------------------------------  
 * Macro to start the timer. Note that the duration is in millisecond.
 * ---------------------------------------------------------------------*/

#define ICF_START_TIMER(p_glb_pdb,p_buff,timer_id,duration,p_ecode,ret_val)\
{                                                                          \
    ret_val = icf_cmn_start_timer(p_glb_pdb, p_buff, duration, &timer_id, p_ecode);\
} /* Macro for staring timer in milliseconds. */

/* ---------------------------------------------------------------------  
 * Macro to stop the timer.
 * ---------------------------------------------------------------------*/

#define ICF_STOP_TIMER(p_glb_pdb, timer_id, p_ecode, ret_val)         \
{                                                                        \
    ret_val = icf_cmn_stop_timer(p_glb_pdb, timer_id, p_ecode);       \
} /* Macro for stopping the timer. */


/* ---------------------------------------------------------------------  
 * Macro to set the module id at the entry of the module. Note that it is
 * assumed that we are not having multi-threaded architecture.
 * ---------------------------------------------------------------------*/
#ifdef ICF_TRACE_ENABLE

#ifdef ICF_PORT_SYMBIAN
#define ICF_SET_MODULE(p_glb_pdb, module_id)
#else
#define ICF_SET_MODULE(p_glb_pdb, module_id)                          \
{\
    if(p_glb_pdb->curr_module_index < ICF_MAX_MODULE_TVSL)       \
    {                                                                 \
        p_glb_pdb->curr_module_index++;                                   \
        p_glb_pdb->module_hist[p_glb_pdb->curr_module_index] = module_id; \
     }\
} /* Macro for setting the current active module id in global pdb. */
#endif
#else
#define ICF_SET_MODULE(p_glb_pdb, module_id)
#endif

/* ---------------------------------------------------------------------
 * Macro to reset the module id at the exit of the module. Note that it is
 * assumed that we are not having multi-threaded architecture.
 * ---------------------------------------------------------------------*/

#ifdef ICF_TRACE_ENABLE
#ifdef ICF_PORT_SYMBIAN
#define ICF_RESET_MODULE(p_glb_pdb)
#else
#define ICF_RESET_MODULE(p_glb_pdb)                                   \
{   \
    if(p_glb_pdb->curr_module_index <= ICF_MAX_MODULE_TVSL)         \
    {                                                                 \
       p_glb_pdb->module_hist[p_glb_pdb->curr_module_index] =              \
                            ICF_INT_MODULE_INVALID;                     \
       p_glb_pdb->curr_module_index--;                                     \
    }\
} /* Macro for setting the current active module id in global pdb. */
#endif
#else
#define ICF_RESET_MODULE(p_glb_pdb)
#endif

/* ---------------------------------------------------------------------  
 * Macro for Statistic Update.
 * ---------------------------------------------------------------------*/

#ifdef ICF_STAT_ENABLED

/* This macro increment statid by 1 */
#define ICF_STAT(p_glb_pdb, stat_id)                             \
{                                                                         \
    if(ICF_NULL != p_glb_pdb && ICF_NULL != p_glb_pdb->p_glb_data) \
        if(ICF_NULL != p_glb_pdb->p_glb_data->p_stats_data) \
        {\
             ((icf_stat_glb_st*)(p_glb_pdb->p_glb_data->p_stats_data))->   \
                   stat_counter_list[stat_id]++;                             \
              ICF_DEBUG_PRINT(("\nICF STAT ID: %d, value %d", stat_id ,\
                  ((icf_stat_glb_st*)(p_glb_pdb->p_glb_data->p_stats_data))\
                  ->stat_counter_list[stat_id]));\
        }\
}

/* This macro increment statid by counter */
#define ICF_STAT_COUNT(p_glb_pdb, stat_id, count)  icf_stat_count(p_glb_pdb, stat_id, count)

#else

#define ICF_STAT(p_glb_pdb, stat_id)

#define ICF_STAT_COUNT(p_glb_pdb, stat_id, count)  

#endif


#ifdef ICF_STAT_ENABLE

/* This macro increments stat value  by 1 */
#define ICF_STATS(counter)                             \
{                                                                         \
    counter ++;                                                            \
}

#else

#define ICF_STATS(counter)

#endif

#define ICF_SEND(p_glb_pdb, p_msg, ret_val, p_ecode)	\
{  \
          ret_val = icf_cmn_send(p_glb_pdb, p_msg, p_ecode);	\
}

#define ICF_MAKE_32BIT_INT(bt3, bt2, bt1, bt0) (((((int)bt3&0xFF)<<24) \
        |((int)bt2&0xFF)<<16) |(((int)bt1&0xFF)<<8) | (bt0 & 0xFF))

ICF_4_0_CRM_START


#define ICF_SET_BIT(common_bitmask, bit) common_bitmask |= bit; 
#define ICF_IS_BIT_SET(common_bitmask,bit) \
	((common_bitmask & bit)?ICF_TRUE:ICF_FALSE)
#define ICF_RESET_BIT(common_bitmask,bit) common_bitmask &= ~bit; 

#define icf_maxConfigFileLen		1000


ICF_4_0_CRM_END


/* ---------------------------------------------------------------------  
 * Macro for raising alarms.
 * ---------------------------------------------------------------------*/
#define ICF_ALARM(str)   ICF_PRINT(((icf_uint8_t *)"\n[ICF_ALARM]"));ICF_PRINT(str)

/* Run time check for IMS_CLIENT,Put ICF_CHECK_IMS_START(p_glb_pdb,ICF_TRUE)
 * where #ifdef IMS_CLIENT is there. for #ifndef IMS_CLIENT 
 * put ICF_CHECK_IMS_START(p_glb_pdb,ICF_FALSE)
 */
#define ICF_CHECK_IMS_START(p_glb_pdb,boolean_flag) \
    if(boolean_flag == \
        ((icf_config_data_st*)(p_glb_pdb->p_glb_data->p_module_data[ICF_INT_MODULE_CFG]))->ims_oprn_flag)\
    {\

/* Run time check for IMS_CLIENT,Put ICF_CHECK_IMS_END
 * before #endif and #else for the corresonding 
 * #ifdef IMS_CLIENT
 */
#define ICF_CHECK_IMS_END \
    }\

#define ICF_CHECK_QOS_SUPPORT_START(p_glb_pdb,boolean_flag) \
    if(boolean_flag == \
        ((((icf_config_data_st*)(p_glb_pdb->p_glb_data->p_module_data[ICF_INT_MODULE_CFG]))->\
		nw_res_reservation_supported)&&\
		(ICF_NULL != p_glb_pdb->p_call_ctx) &&\
		(p_glb_pdb->p_call_ctx->common_bitmask & ICF_NW_RES_RESERV_REQD)))\
    {\

#define ICF_CHECK_QOS_SUPPORT_END \
    }\

#define ICF_CHECK_MULT_ANS_ENBL_START(p_glb_pdb) \
   if(ICF_TRUE == \
((icf_config_data_st*)(p_glb_pdb->p_glb_data->p_module_data[ICF_INT_MODULE_CFG]))->\
		multiple_answer_enable)\
    {\

#define ICF_CHECK_MULT_ANS_ENBL_END \
    }\

#define ICF_CHECK_MULT_ANS_DISBL_START(p_glb_pdb) \
   if(ICF_FALSE == \
((icf_config_data_st*)(p_glb_pdb->p_glb_data->p_module_data[ICF_INT_MODULE_CFG]))->\
		multiple_answer_enable)\
    {\

#define ICF_CHECK_MULT_ANS_DISBL_END \
    }\

/* ---------------------------------------------------------------------------  
 * adding macros to get the pointer to the next node of the common lists 
 * within ICF 
 * --------------------------------------------------------------------------*/
#define ICF_CMN_GET_NXT_NODE_FOR_LIST(p_tmp_node)	\
	p_tmp_node->p_next

#define ICF_CMN_GET_DATA_NODE_FOR_LIST(p_tmp_node)	\
	p_tmp_node->p_data

#define ICF_CMN_GET_DATA_NODE_FOR_INTERNAL_LIST(p_tmp_node)	\
	p_tmp_node->p_payload

/* This macro returns true if the argument is a special digit like 
   a '*' or a '#'.*/
#define ICF_ISSPECIALDIGIT(var) \
    (('*' == var)||('#' == var))

/*Marco for wildcard user */
#define  ICF_WILDCARD_USER            "wildcarded" 

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* End of ifndef __ICF_MACRO_H__ */
