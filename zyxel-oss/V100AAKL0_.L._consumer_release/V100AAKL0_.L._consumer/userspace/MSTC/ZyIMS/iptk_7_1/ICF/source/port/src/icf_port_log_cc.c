/*************************************************************************
 *
 *     FILENAME           :  icf_port_log_cc.c
 *
 *     DESCRIPTION        :  This file contains error/trace printing 
 *                           routines for CC
 *
 *     Revision History   :
 *         DATE        NAME                REFERENCE            REASON
 *         ----------------------------------------------------------
 *         07/01/05    Aashish Suchdev                        Initial
 *         16/02/05    Jyoti Narula                 Updated for printing traces
 *         07/03/06	   Anuradha Gupta				Updated the printing traces
 *         30/03/06	   Anuradha Gupta				Updated the printing traces
 *
 *     Copyright 2006, Aricent.
 *
 *************************************************************************/
#ifdef ICF_PORT_SOLARIS
#include <stdio.h>
#endif

#include "icf_common_types.h"
#include "icf_port_prototypes.h"
#include "icf_port_defs.h"
#include "icf_common_interface_types.h"

#ifdef ICF_TRACE_ENABLE
#include "icf_cc_defs.h"
#endif

#ifdef ICF_ERROR_ENABLE
#ifndef ICF_SM_LOG_ENABLE
/*************************************************************************
*
*  FUNCTION NAME  :   icf_port_log_cc_error
*
*  DESCRIPTION    :  This function is used to print the error data from CC  
*
*  RETURNS        :     
*
 *************************************************************************/

icf_void_t icf_port_log_cc_error(
        IN icf_error_id_t    error_id,
        IN icf_error_data_st *p_error_data)
{
#ifdef ICF_DETAILED_DEBUG_TRACE
    icf_uint8_t  count = 0, max_count = 0;    
#endif

    switch (error_id){
        default:
            {
                if(ICF_NULL != p_error_data)
                {
#ifdef ICF_DETAILED_DEBUG_TRACE

                    max_count = p_error_data->num_of_integer_values;
                    ICF_PRINT(((icf_uint8_t *)"\tDATA:integer(s):"));
                    for ( count = 0; count < max_count; count ++)
                    {
                        ICF_PRINT(((icf_uint8_t *)" %lu\n",p_error_data->int_val[count]));
                    }

                    max_count = p_error_data->number_of_string_values;
                    ICF_PRINT(((icf_uint8_t *)"\nDATA:string(s):"));
                    for ( count = 0; count < max_count; count ++)
                    {
                        ICF_PRINT(((icf_uint8_t *)"\t %s\n",
                                    p_error_data->string_val[count].str));
                    }
                    ICF_PRINT(((icf_uint8_t *)"\n"));
#endif
                }
            }
            break;
    }
}
#endif
#endif

#ifdef ICF_TRACE_ENABLE
/*******************************************************************************
 *
 * FUNCTION:        icf_port_log_cc_state_to_str
 *
 * DESCRIPTION:     This function returns a string corresponding to a state and
 *                  is used only for traces.
 *
 ******************************************************************************/
icf_void_t icf_port_log_cc_state_to_str(
        IN icf_cc_fsm_state_t state_val,
        OUT icf_uint8_t *p_state_str)
{
    switch(state_val)
    {
        case ICF_CC_00_IDLE:
            icf_port_strcpy(p_state_str,
                    (icf_uint8_t *)"ICF_CC_00_IDLE");
            break;

        case ICF_CC_01_AWT_CMS_RESP:
            icf_port_strcpy(p_state_str,
                    (icf_uint8_t *)"ICF_CC_01_AWT_CMS_RESP");
            break;

        case ICF_CC_02_AWT_SSA_SETUP_RESP:
            icf_port_strcpy(p_state_str,
                    (icf_uint8_t *)"ICF_CC_02_AWT_SSA_SETUP_RESP");
            break;

        case ICF_CC_03_AWT_ALERT_RESP:
            icf_port_strcpy(p_state_str,
                    (icf_uint8_t *)"ICF_CC_03_AWT_ALERT_RESP");
            break;

        case ICF_CC_04_AWT_MMS_RESP:
            icf_port_strcpy(p_state_str,
                    (icf_uint8_t *)"ICF_CC_04_AWT_MMS_RESP");
            break;

        case ICF_CC_05_AWT_CONNECT:
            icf_port_strcpy(p_state_str,
                    (icf_uint8_t *)"ICF_CC_05_AWT_CONNECT");
            break;

        case ICF_CC_06_CONNECTED:
            icf_port_strcpy(p_state_str,
                    (icf_uint8_t *)"ICF_CC_06_CONNECTED");
            break;

        case ICF_CC_07_AWT_DMS_RESP:
            icf_port_strcpy(p_state_str,
                    (icf_uint8_t *)"ICF_CC_07_AWT_DMS_RESP");
            break;

        case ICF_CC_08_AWT_CALL_TERMINATE_RESP:
            icf_port_strcpy(p_state_str,
                    (icf_uint8_t *)"ICF_CC_08_AWT_CALL_TERMINATE_RESP");
            break;

        case ICF_CC_09_AWT_MEDIA_PARAM_PRACK:
            icf_port_strcpy(p_state_str,
                    (icf_uint8_t *)"ICF_CC_09_AWT_MEDIA_PARAM_PRACK");
            break;

        case ICF_CC_10_AWT_ALERT_PRACK:
            icf_port_strcpy(p_state_str,
                    (icf_uint8_t *)"ICF_CC_10_AWT_ALERT_PRACK");
            break;

        case ICF_CC_11_AWT_SSA_CONNECT_ACK:
            icf_port_strcpy(p_state_str,
                    (icf_uint8_t *)"ICF_CC_11_AWT_SSA_CONNECT_ACK");
            break;

        case ICF_CC_12_WAIT_START_PROC_SIG:
            icf_port_strcpy(p_state_str,
                    (icf_uint8_t *)"ICF_CC_12_WAIT_START_PROC_SIG");
            break;

        case ICF_CC_13_WAIT_START_PROC_MEDIA:
            icf_port_strcpy(p_state_str,
                    (icf_uint8_t *)"ICF_CC_13_WAIT_START_PROC_MEDIA");
            break;

ICF_HANDLE_REINVITE_START
		case ICF_CC_14_AWT_CALL_MODIFY_RESP:
            icf_port_strcpy(p_state_str,
                    (icf_uint8_t *)"ICF_CC_14_AWT_CALL_MODIFY_RESP");
            break;

		case ICF_CC_15_AWT_CALL_MODIFY_ALERT_RESP:
            icf_port_strcpy(p_state_str,
                    (icf_uint8_t *)"ICF_CC_15_AWT_CALL_MODIFY_ALERT_RESP");
            break;

		case ICF_CC_16_AWT_UPDATE:
            icf_port_strcpy(p_state_str,
                    (icf_uint8_t *)"ICF_CC_16_AWT_UPDATE");
            break;

ICF_HANDLE_REINVITE_END

		case ICF_CC_19_AWT_SSA_CALL_MODIFY_ACK:
			icf_port_strcpy(p_state_str,
                    (icf_uint8_t *)"ICF_CC_19_AWT_SSA_CALL_MODIFY_ACK");
			break;	

        case ICF_CC_18_CALL_STATE_INVALID:
            icf_port_strcpy(p_state_str,
                    (icf_uint8_t *)"ICF_CC_18_CALL_STATE_INVALID");
            break;

        case ICF_CC_20_AWT_EARLY_IND_CFM:
             icf_port_strcpy(p_state_str,
                     (icf_uint8_t *)"ICF_CC_20_AWT_EARLY_IND_CFM");
              break;

        default:
            icf_port_strcpy(p_state_str,
                    (icf_uint8_t *)"ICF_CC_18_CALL_STATE_INVALID");
            break;

    } /* end of switch case for state_val */
} /* end of icf_port_log_cc_state_to_str() */
#endif /* end of #ifdef ICF_TRACE_ENABLE */


#ifdef ICF_TRACE_ENABLE
/*******************************************************************************
 *
 * FUNCTION:        icf_port_log_cc_event_to_str
 *
 * DESCRIPTION:     This function returns a string corresponding to an event
 *                  and is used only for traces.
 *
 ******************************************************************************/
icf_void_t icf_port_log_cc_event_to_str(
        IN icf_uint8_t event_val,
        OUT icf_uint8_t *p_event_str)
{
    switch(event_val)
    {
        case ICF_CC_PA_SETUP_REQ:
            icf_port_strcpy(p_event_str,
                    (icf_uint8_t *)"ICF_CC_PA_SETUP_REQ");
            break;

        case ICF_CC_SSA_SETUP_RESP:
            icf_port_strcpy(p_event_str,
                    (icf_uint8_t *)"ICF_CC_SSA_SETUP_RESP");
            break;

        case ICF_CC_SSA_MEDIA_PARAM:
            icf_port_strcpy(p_event_str,
                    (icf_uint8_t *)"ICF_CC_SSA_MEDIA_PARAM");
            break;

        case ICF_CC_SSA_ALERT:
            icf_port_strcpy(p_event_str,
                    (icf_uint8_t *)"ICF_CC_SSA_ALERT");
            break;

        case ICF_CC_SSA_ALERT_WITH_MEDIA:
            icf_port_strcpy(p_event_str,
                    (icf_uint8_t *)"ICF_CC_SSA_ALERT_WITH_MEDIA");
            break;

        case ICF_CC_SSA_CALL_CONNECT:
            icf_port_strcpy(p_event_str,
                    (icf_uint8_t *)"ICF_CC_SSA_CALL_CONNECT");
            break;

        case ICF_CC_CMS_RESP:
            icf_port_strcpy(p_event_str,
                    (icf_uint8_t *)"ICF_CC_CMS_RESP");
            break;

        case ICF_CC_START_CALL_PROC_SIG:
            icf_port_strcpy(p_event_str,
                    (icf_uint8_t *)"ICF_CC_START_CALL_PROC_SIG");
            break;

        case ICF_CC_START_CALL_PROC_MEDIA:
            icf_port_strcpy(p_event_str,
                    (icf_uint8_t *)"ICF_CC_START_CALL_PROC_MEDIA");
            break;

        case ICF_CC_MMS_RESP:
            icf_port_strcpy(p_event_str,
                    (icf_uint8_t *)"ICF_CC_MMS_RESP");
            break;

        case ICF_CC_DMS_RESP:
            icf_port_strcpy(p_event_str,
                    (icf_uint8_t *)"ICF_CC_DMS_RESP");
            break;

        case ICF_CC_SSA_CALL_TERMINATE_REQ:
            icf_port_strcpy(p_event_str,
                    (icf_uint8_t *)"ICF_CC_SSA_CALL_TERMINATE_REQ");
            break;

        case ICF_CC_SSA_REMOTE_REJECT:
            icf_port_strcpy(p_event_str,
                    (icf_uint8_t *)"ICF_CC_SSA_REMOTE_REJECT");
            break;

        case ICF_CC_SSA_REDIRECT:
            icf_port_strcpy(p_event_str,
                    (icf_uint8_t *)"ICF_CC_SSA_REDIRECT");
            break;

        case ICF_CC_PA_CALL_TERMINATE_REQ:
            icf_port_strcpy(p_event_str,
                    (icf_uint8_t *)"ICF_CC_PA_CALL_TERMINATE_REQ");
            break;

        case ICF_CC_SSA_CALL_CANCEL_RESP:
            icf_port_strcpy(p_event_str,
                    (icf_uint8_t *)"ICF_CC_SSA_CALL_CANCEL_RESP");
            break;

        case ICF_CC_PA_CALL_TERMINATE_RESP:
            icf_port_strcpy(p_event_str,
                    (icf_uint8_t *)"ICF_CC_PA_CALL_TERMINATE_RESP");
            break;

        case ICF_CC_SSA_CALL_TERMINATE_RESP:
            icf_port_strcpy(p_event_str,
                    (icf_uint8_t *)"ICF_CC_SSA_CALL_TERMINATE_RESP");
            break;

        case ICF_CC_SSA_OFFER_CHANGE:
            icf_port_strcpy(p_event_str,
                    (icf_uint8_t *)"ICF_CC_SSA_OFFER_CHANGE");
            break;

        case ICF_CC_CALL_MUTE_REQ:
            icf_port_strcpy(p_event_str,
                    (icf_uint8_t *)"ICF_CC_CALL_MUTE_REQ");
            break;

        case ICF_CC_CALL_UNMUTE_REQ:
            icf_port_strcpy(p_event_str,
                    (icf_uint8_t *)"ICF_CC_CALL_UNMUTE_REQ");
            break;

        case ICF_CC_MM_ERR_IND:
            icf_port_strcpy(p_event_str,
                    (icf_uint8_t *)"ICF_CC_MM_ERR_IND");
            break;

        case ICF_CC_TIMER_EXPIRY:
            icf_port_strcpy(p_event_str,
                    (icf_uint8_t *)"ICF_CC_TIMER_EXPIRY");
            break;

        case ICF_CC_SSA_SETUP_REQ:
            icf_port_strcpy(p_event_str,
                    (icf_uint8_t *)"ICF_CC_SSA_SETUP_REQ");
            break;

        case ICF_CC_PA_ALERT:
            icf_port_strcpy(p_event_str,
                    (icf_uint8_t *)"ICF_CC_PA_ALERT");
            break;

        case ICF_CC_PA_CALL_CONNECT:
            icf_port_strcpy(p_event_str,
                    (icf_uint8_t *)"ICF_CC_PA_CALL_CONNECT");
            break;

        case ICF_CC_MEDIA_PARAM_PRACK:
            icf_port_strcpy(p_event_str,
                    (icf_uint8_t *)"ICF_CC_MEDIA_PARAM_PRACK");
            break;

        case ICF_CC_ALERT_PRACK:
            icf_port_strcpy(p_event_str,
                    (icf_uint8_t *)"ICF_CC_ALERT_PRACK");
            break;

        case ICF_CC_SSA_CONNECT_ACK:
            icf_port_strcpy(p_event_str,
                    (icf_uint8_t *)"ICF_CC_SSA_CONNECT_ACK");
            break;

        case ICF_CC_SSA_CALL_CANCEL_REQ:
            icf_port_strcpy(p_event_str,
                    (icf_uint8_t *)"ICF_CC_SSA_CALL_CANCEL_REQ");
            break;

        case ICF_CC_GENERIC_ERROR:
            icf_port_strcpy(p_event_str,
                    (icf_uint8_t *)"ICF_CC_GENERIC_ERROR");
            break;

        case ICF_CC_FORCEFUL_CLEAR:
            icf_port_strcpy(p_event_str,
                    (icf_uint8_t *)"ICF_CC_FORCEFUL_CLEAR");
            break;

ICF_HANDLE_REINVITE_START
		case ICF_CC_PA_CALL_MODIFY_REQ:
			icf_port_strcpy(p_event_str,
                    (icf_uint8_t *)"ICF_CC_PA_CALL_MODIFY_REQ");
			break;

		case ICF_CC_SSA_REINVITE_ACK:
            icf_port_strcpy(p_event_str,
                    (icf_uint8_t *)"ICF_CC_SSA_REINVITE_ACK");
            break;

		case ICF_CC_PA_CALL_MODIFY_RSP:
			icf_port_strcpy(p_event_str,
                    (icf_uint8_t *)"ICF_CC_PA_CALL_MODIFY_RSP");
			break;

		case ICF_CC_MEDIA_UPDATE_RESP:
			icf_port_strcpy(p_event_str,
                    (icf_uint8_t *)"ICF_CC_MEDIA_UPDATE_RESP");
			break;

ICF_HANDLE_REINVITE_END

        case ICF_CC_INVALID_EVENT:
            icf_port_strcpy(p_event_str,
                    (icf_uint8_t *)"ICF_CC_INVALID_EVENT");
            break;

        case ICF_CC_PA_EARLY_CALL_CFM:
            icf_port_strcpy(p_event_str,
                    (icf_uint8_t *)"ICF_CC_PA_EARLY_CALL_CFM");
            break;
        case ICF_CC_PA_MEDIA_CONNECT_REQ:
            icf_port_strcpy(p_event_str,
                    (icf_uint8_t *)"ICF_CC_PA_MEDIA_CONNECT_REQ");
            break;

       case ICF_CC_SSA_CALL_PROGRESS:
            icf_port_strcpy(p_event_str,
                    (icf_uint8_t *)"ICF_CC_SSA_CALL_PROGRESS");
            break;

        default:
            icf_port_strcpy(p_event_str,
                    (icf_uint8_t *)"ICF_CC_INVALID_EVENT");
            break;

    } /* end of switch case for event_val */
} /* end of icf_port_log_cc_event_to_str() */
#endif /* end of #ifdef ICF_TRACE_ENABLE */


#ifdef ICF_TRACE_ENABLE
/*******************************************************************************
 *
 * FUNCTION:        icf_port_log_cc_media_to_str
 *
 * DESCRIPTION:     This function returns a string corresponding to media mode
 *                  and is used only for traces.
 *
 ******************************************************************************/
icf_void_t icf_port_log_cc_media_to_str(
        IN icf_media_mode_t mode_val,
        OUT icf_uint8_t *p_mode_str)
{
    switch(mode_val)
    {
        case ICF_MEDIA_MODE_INVALID:
            icf_port_strcpy(p_mode_str,
                    (icf_uint8_t *)"ICF_MEDIA_MODE_INVALID");
            break;

        case ICF_MEDIA_MODE_INACTIVE:
            icf_port_strcpy(p_mode_str,
                    (icf_uint8_t *)"ICF_MEDIA_MODE_INACTIVE");
            break;

        case ICF_MEDIA_MODE_SEND_ONLY:
            icf_port_strcpy(p_mode_str,
                    (icf_uint8_t *)"ICF_MEDIA_MODE_SEND_ONLY");
            break;

        case ICF_MEDIA_MODE_RECV_ONLY:
            icf_port_strcpy(p_mode_str,
                    (icf_uint8_t *)"ICF_MEDIA_MODE_RECV_ONLY");
            break;

        case ICF_MEDIA_MODE_SEND_RECV:
            icf_port_strcpy(p_mode_str,
                    (icf_uint8_t *)"ICF_MEDIA_MODE_SEND_RECV");
            break;

        default:
            icf_port_strcpy(p_mode_str,
                    (icf_uint8_t *)"ICF_MEDIA_MODE_INVALID");
            break;

    } /* end of switch case for mode_val */
} /* end of icf_port_log_cc_media_to_str() */
#endif /* end of #ifdef ICF_TRACE_ENABLE */


#ifdef ICF_TRACE_ENABLE
/*******************************************************************************
 *
 * FUNCTION:        icf_port_log_cc_module_to_str
 *
 * DESCRIPTION:     This function returns a string corresponding to a module
 *                  and is used only for traces.
 *
 ******************************************************************************/
icf_void_t icf_port_log_cc_module_to_str(
        IN icf_uint8_t module_id,
        OUT icf_uint8_t *p_module_str)
{
    switch(module_id)
    {
        case ICF_INT_MODULE_INVALID:
            icf_port_strcpy(p_module_str,
                    (icf_uint8_t *)"ICF_INT_MODULE_INVALID");
            break;

        case ICF_INT_MODULE_MRM:
            icf_port_strcpy(p_module_str,
                    (icf_uint8_t *)"ICF_INT_MODULE_MRM");
            break;

        case ICF_INT_MODULE_CM:
            icf_port_strcpy(p_module_str,
                    (icf_uint8_t *)"ICF_INT_MODULE_CM");
            break;

        case ICF_INT_MODULE_CC:
            icf_port_strcpy(p_module_str,
                    (icf_uint8_t *)"ICF_INT_MODULE_CC");
            break;

        case ICF_INT_MODULE_DBM:
            icf_port_strcpy(p_module_str,
                    (icf_uint8_t *)"ICF_INT_MODULE_DBM");
            break;

        case ICF_INT_MODULE_SM:
            icf_port_strcpy(p_module_str,
                    (icf_uint8_t *)"ICF_INT_MODULE_SM");
            break;

        case ICF_INT_MODULE_REGM:
            icf_port_strcpy(p_module_str,
                    (icf_uint8_t *)"ICF_INT_MODULE_REGM");
            break;

        case ICF_INT_MODULE_CFG:
            icf_port_strcpy(p_module_str,
                    (icf_uint8_t *)"ICF_INT_MODULE_CFG");
            break;

        case ICF_INT_MODULE_SSA:
            icf_port_strcpy(p_module_str,
                    (icf_uint8_t *)"ICF_INT_MODULE_SSA");
            break;

        case ICF_INT_MODULE_SIP_UA:
            icf_port_strcpy(p_module_str,
                    (icf_uint8_t *)"ICF_INT_MODULE_SIP_UA");
            break;

        case ICF_INT_MODULE_SIP_STACK:
            icf_port_strcpy(p_module_str,
                    (icf_uint8_t *)"ICF_INT_MODULE_SIP_STACK");
            break;

        case ICF_INT_MODULE_MMI:
            icf_port_strcpy(p_module_str,
                    (icf_uint8_t *)"ICF_INT_MODULE_MMI");
            break;

        case ICF_INT_MODULE_SCM:
            icf_port_strcpy(p_module_str,
                    (icf_uint8_t *)"ICF_INT_MODULE_SCM");
            break;

        case ICF_INT_MODULE_ADM:
            icf_port_strcpy(p_module_str,
                    (icf_uint8_t *)"ICF_INT_MODULE_ADM");
            break;

        case ICF_INT_MODULE_CHR:
            icf_port_strcpy(p_module_str,
                    (icf_uint8_t *)"ICF_INT_MODULE_CHR");
            break;

        case ICF_INT_MODULE_RNW:
            icf_port_strcpy(p_module_str,
                    (icf_uint8_t *)"ICF_INT_MODULE_RNW");
            break;

        case ICF_INT_MODULE_TWC:
            icf_port_strcpy(p_module_str,
                    (icf_uint8_t *)"ICF_INT_MODULE_TWC");
            break;

        case ICF_INT_MODULE_CTA:
            icf_port_strcpy(p_module_str,
                    (icf_uint8_t *)"ICF_INT_MODULE_CTA");
            break;

        case ICF_INT_MODULE_CTU:
            icf_port_strcpy(p_module_str,
                    (icf_uint8_t *)"ICF_INT_MODULE_CTU");
            break;


        case ICF_INT_MODULE_CFW:
            icf_port_strcpy(p_module_str,
                    (icf_uint8_t *)"ICF_INT_MODULE_CFW");
            break;

        default:
            icf_port_strcpy(p_module_str,
                    (icf_uint8_t *)"ICF_INT_MODULE_INVALID");
            break;
    } /* end of switch case for module_id */
} /* end of icf_port_log_cc_module_to_str() */
#endif /* end of #ifdef ICF_TRACE_ENABLE */

#ifdef ICF_TRACE_ENABLE
/*******************************************************************************
 *
 * FUNCTION:        icf_port_log_cc_timer_to_str
 *
 * DESCRIPTION:     This function returns a string corresponding to a timer
 *                  type and is used only for traces.
 *
 ******************************************************************************/
icf_void_t icf_port_log_cc_timer_to_str(
        IN icf_timer_type_t timer_type,
        OUT icf_uint8_t *p_timer_str)
{
    switch(timer_type)
    {
        case ICF_TIMER_TYPE_SETUP:
            icf_port_strcpy(p_timer_str,
                    (icf_uint8_t *)"ICF_TIMER_TYPE_SETUP");
            break;

        case ICF_TIMER_TYPE_ALERTING:
            icf_port_strcpy(p_timer_str,
                    (icf_uint8_t *)"ICF_TIMER_TYPE_ALERTING");
            break;

        case ICF_TIMER_TYPE_MEDIA:
            icf_port_strcpy(p_timer_str,
                    (icf_uint8_t *)"ICF_TIMER_TYPE_MEDIA");
            break;

        case ICF_TIMER_TYPE_RELEASE:
            icf_port_strcpy(p_timer_str,
                    (icf_uint8_t *)"ICF_TIMER_TYPE_RELEASE");
            break;
        default:
            icf_port_strcpy(p_timer_str,
                    (icf_uint8_t *)"Invalid timer type");
            break;

    } /* end of switch-case for timer_type */
} /* end of icf_port_log_cc_timer_to_str() */
#endif /* end of #ifdef ICF_TRACE_ENABLE */




#ifdef ICF_TRACE_ENABLE
/*************************************************************************
*
*  FUNCTION NAME  :   icf_port_log_cc_trace
*
*  DESCRIPTION    :  This function is used to print the trace data from CC  
*
*  RETURNS        :     
*
 *************************************************************************/

icf_void_t icf_port_log_cc_trace(
        IN icf_trace_id_t    trace_id,
        IN icf_trace_data_st *p_trace_data)
{
#ifdef ICF_DETAILED_DEBUG_TRACE
    icf_uint8_t  count = 0, max_count = 0;   
#endif

    icf_string_st temp; 

    switch (trace_id){

        case ICF_TRACE_FSM_TRIGGERED:
            ICF_PRINT(((icf_uint8_t *)">> call_id = %d\n", p_trace_data->int_val[0]));
/*          ICF_PRINT(((icf_uint8_t *)">> call_state = %d\n", p_trace_data->int_val[1]));
            ICF_PRINT(((icf_uint8_t *)">> current_event = %d\n", p_trace_data->int_val[2]));*/

            icf_port_log_cc_state_to_str((icf_uint8_t)p_trace_data->int_val[1],temp.str);
            ICF_PRINT(((icf_uint8_t *)">> call_state = %s\n", temp.str));
            icf_port_log_cc_event_to_str((icf_uint8_t)p_trace_data->int_val[2],temp.str);
                    
            ICF_PRINT(((icf_uint8_t *)">> current_event = %s\n", temp.str));

            break;

        case ICF_TRACE_CALL_DIR:
            ICF_PRINT(((icf_uint8_t *)">> call_id = %d\n", p_trace_data->int_val[0]));
            ICF_PRINT(((icf_uint8_t *)">> call_direction = %s\n", p_trace_data->string_val[0].str));
            break;

        case ICF_TRACE_MEDIA_SETUP_TYPE:
            ICF_PRINT(((icf_uint8_t *)">> Call_id = %d\n", p_trace_data->int_val[0]));
            ICF_PRINT(((icf_uint8_t *)">> Call_Media_Setup_Type = %s\n", p_trace_data->string_val[0].str));
            break;

        case ICF_TRACE_MEDIA_COMMITTED:
            ICF_PRINT(((icf_uint8_t *)">> Call_id = %d\n", p_trace_data->int_val[0]));
            ICF_PRINT(((icf_uint8_t *)">> Media_Committed = %s\n", p_trace_data->string_val[0].str));
            break;

        case ICF_TRACE_SDP_IN_PEER_MSG:
            ICF_PRINT(((icf_uint8_t *)">> Call_id = %d\n", p_trace_data->int_val[0]));
            ICF_PRINT(((icf_uint8_t *)">> SDP_Present = %s\n", p_trace_data->string_val[0].str));
            break;

        case ICF_TRACE_PREVIOUS_STATE:
        case ICF_TRACE_SET_PREVIOUS_STATE:
            
            ICF_PRINT(((icf_uint8_t *)">> Call_id = %d\n", p_trace_data->int_val[0]));
         /*ICF_PRINT((">> Previous State = %d\n", p_trace_data->int_val[1]));*/
           icf_port_log_cc_state_to_str((icf_uint8_t)p_trace_data->int_val[1],temp.str);
            ICF_PRINT(((icf_uint8_t *)">> Previous State = %s\n", temp.str));

            break;

        case ICF_TRACE_CURRENT_STATE:
        case ICF_TRACE_SET_CURRENT_STATE:
            ICF_PRINT(((icf_uint8_t *)">> Call_id = %d\n", p_trace_data->int_val[0]));
/*           ICF_PRINT((">> Call State = %d\n", p_trace_data->int_val[1]));*/
            icf_port_log_cc_state_to_str((icf_uint8_t)p_trace_data->int_val[1],temp.str);
            ICF_PRINT(((icf_uint8_t *)">> Current Call State = %s\n", temp.str));
                
            break;

        case ICF_TRACE_NEXT_STATE:
        case ICF_TRACE_SET_NEXT_STATE:
            ICF_PRINT(((icf_uint8_t *)">> Call_id = %d\n", p_trace_data->int_val[0]));
/*          ICF_PRINT((">> Next State = %d\n", p_trace_data->int_val[1]));*/
            icf_port_log_cc_state_to_str((icf_uint8_t)p_trace_data->int_val[1],temp.str);
            ICF_PRINT(((icf_uint8_t *)">> Next Call State = %s\n", temp.str));

            break;

        case ICF_TRACE_MEDIA_MODE:
        case ICF_TRACE_SET_MEDIA_MODE:
            ICF_PRINT(((icf_uint8_t *)">> Call_id = %d\n", p_trace_data->int_val[0]));
            ICF_PRINT(((icf_uint8_t *)">> Media Mode = %s\n", p_trace_data->string_val[0].str));
            break;

        case ICF_TRACE_SET_MODE_TO_BE_APP:
            ICF_PRINT(((icf_uint8_t *)">> Call_id = %d\n", p_trace_data->int_val[0]));
            ICF_PRINT(((icf_uint8_t *)">> Media Mode To Be Applied = %s\n", p_trace_data->string_val[0].str));

            break;

        case ICF_TRACE_SET_RELEASE_CAUSE:
            ICF_PRINT(((icf_uint8_t *)">> Call_id = %d\n", p_trace_data->int_val[0]));
            ICF_PRINT(((icf_uint8_t *)">> Release Cause = %d\n", p_trace_data->int_val[1]));

            break;

        case ICF_TRACE_MEDIA_ONGOING:
        case ICF_TRACE_SET_MEDIA_ONGOING:
            ICF_PRINT(((icf_uint8_t *)">> Call_id = %d\n", p_trace_data->int_val[0]));
            icf_port_log_cc_module_to_str((icf_uint8_t)p_trace_data->int_val[1],temp.str);

            ICF_PRINT(((icf_uint8_t *)">> Media Ongoing = %s\n", temp.str));

            break;

        case ICF_TRACE_SIG_ONGOING:
        case ICF_TRACE_SET_SIG_ONGOING:
           ICF_PRINT(((icf_uint8_t *)">> Call_id = %d\n", p_trace_data->int_val[0]));
            icf_port_log_cc_module_to_str((icf_uint8_t)p_trace_data->int_val[1],temp.str);

            ICF_PRINT(((icf_uint8_t *)">> Signalling Ongoing = %s\n", temp.str));

            break;


        case ICF_TRACE_ONGOING_EVENT:
        case ICF_TRACE_SET_ONGOING_EVENT:
            ICF_PRINT(((icf_uint8_t *)">> Call_id = %d\n", p_trace_data->int_val[0]));
            icf_port_log_cc_event_to_str((icf_uint8_t)p_trace_data->int_val[1],temp.str);

            ICF_PRINT(((icf_uint8_t *)">> Event Ongoing = %s\n", temp.str));


            break;

        case ICF_TRACE_INBAND_INFO:
            ICF_PRINT(((icf_uint8_t *)">> Call_id = %d\n", p_trace_data->int_val[0]));
            ICF_PRINT(((icf_uint8_t *)">> Inband Info Present = %s\n", p_trace_data->string_val[0].str));

            break;

        case ICF_TRACE_PRACK_REQD:
            ICF_PRINT(((icf_uint8_t *)">> Call_id = %d\n", p_trace_data->int_val[0]));
            ICF_PRINT(((icf_uint8_t *)">> Prack Required = %s\n", p_trace_data->string_val[0].str));

            break;

        case ICF_TRACE_MEDIA_TO_BE_SENT:
             ICF_PRINT(((icf_uint8_t *)">> Call_id = %d\n", p_trace_data->int_val[0]));
            ICF_PRINT(((icf_uint8_t *)">> Media To be Sent = %s\n", p_trace_data->string_val[0].str));
            
            break;

        case ICF_TRACE_MEDIA_MODIFY_SENT:
             ICF_PRINT(((icf_uint8_t *)">> Call_id = %d\n", p_trace_data->int_val[0]));

             icf_port_log_cc_media_to_str(
                     (icf_media_mode_t)p_trace_data->int_val[0],
                     temp.str);
             ICF_PRINT(((icf_uint8_t *)">> Remote media mode = %s\n",temp.str));
             
             icf_port_log_cc_mmtyp_to_str(
                     (icf_media_mode_t)p_trace_data->int_val[1],
                     temp.str);

             ICF_PRINT(((icf_uint8_t *)">> MMS Type = %s\n", temp.str));

             icf_port_log_cc_media_to_str(
                     (icf_media_mode_t)p_trace_data->int_val[3],
                     temp.str);

             ICF_PRINT(((icf_uint8_t *)">> Media Mode = %s\n", temp.str));

             icf_port_log_cc_media_to_str(
                     (icf_media_mode_t)p_trace_data->int_val[3],
                     temp.str);

             ICF_PRINT(((icf_uint8_t *)">> Mode to be applied = %s\n", temp.str));

            break;


        case ICF_TRACE_EVENT_PA_CONNECT:
            ICF_PRINT(((icf_uint8_t *)">> Call_id = %d\n", p_trace_data->int_val[0]));
            ICF_PRINT(((icf_uint8_t *)">> Event PA Connect Pending = %s\n", p_trace_data->string_val[0].str));

            break;

        case ICF_TRACE_EVENT_PA_TERM_REQ:
            ICF_PRINT(((icf_uint8_t *)">> Call_id = %d\n", p_trace_data->int_val[0]));
            ICF_PRINT(((icf_uint8_t *)">> Event PA CALL_TERM Request Pending = %s\n", p_trace_data->string_val[0].str));

            break;

        case ICF_TRACE_EVENT_MM_ERR_IND:
            ICF_PRINT(((icf_uint8_t *)">> Call_id = %d\n", p_trace_data->int_val[0]));
            ICF_PRINT(((icf_uint8_t *)">> Event MM_ERR Indication Pending = %s\n", p_trace_data->string_val[0].str));

            break;

        case ICF_TRACE_EVENT_OFFER_CHANGE:
            ICF_PRINT(((icf_uint8_t *)">> Call_id = %d\n", p_trace_data->int_val[0]));
            ICF_PRINT(((icf_uint8_t *)">> Event Offer Change Pending = %s\n", p_trace_data->string_val[0].str));

            break;

        case ICF_TRACE_EVENT_INT_ERR:
            ICF_PRINT(((icf_uint8_t *)">> Call_id = %d\n", p_trace_data->int_val[0]));
            ICF_PRINT(((icf_uint8_t *)">> Event INT_ERR Pending = %s\n", p_trace_data->string_val[0].str));

            break;

        case ICF_TRACE_EVENT_CALL_MUTE:
            ICF_PRINT(((icf_uint8_t *)">> Call_id = %d\n", p_trace_data->int_val[0]));
            ICF_PRINT(((icf_uint8_t *)">> Event Call Mute Pending = %s\n", p_trace_data->string_val[0].str));

            break;

        case ICF_TRACE_EVENT_CALL_UNMUTE:
            ICF_PRINT(((icf_uint8_t *)">> Call_id = %d\n", p_trace_data->int_val[0]));
            ICF_PRINT(((icf_uint8_t *)">> Event Call Unmute Pending = %s\n", p_trace_data->string_val[0].str));
            break;

        case ICF_TRACE_TIMER_EXPIRED:
            ICF_PRINT(((icf_uint8_t *)">> Call_id = %d\n", p_trace_data->int_val[0]));
            icf_port_log_cc_timer_to_str((icf_uint8_t)p_trace_data->int_val[1],temp.str);
            ICF_PRINT(((icf_uint8_t *)">> Timer that expired = %s\n", temp.str));
            break;

        case ICF_TRACE_START_TIMER:
        case ICF_TRACE_STOP_TIMER:
            ICF_PRINT(((icf_uint8_t *)">> Call_id = %d\n", p_trace_data->int_val[0]));
            icf_port_log_cc_timer_to_str((icf_uint8_t)p_trace_data->int_val[1],temp.str);
            ICF_PRINT(((icf_uint8_t *)">> Timer that started/stopped = %s\n", temp.str));

            break;

        case ICF_TRACE_CALL_STATUS_PA_CLR:
        case ICF_TRACE_CALL_STATUS_MM_CLR:
        case ICF_TRACE_CALL_STATUS_NW_CLR:
        case ICF_TRACE_CALL_STATUS_ALL_CLR:
        case ICF_TRACE_REL_TYPE_PA_INIT:
        case ICF_TRACE_REL_TYPE_MM_INIT:
        case ICF_TRACE_REL_TYPE_NW_INIT:
        case ICF_TRACE_REL_TYPE_TIMER_INIT:
        case ICF_TRACE_REL_TYPE_INT_ERR:
        case ICF_TRACE_CALL_MUTE_SET:
        case ICF_TRACE_CALL_MUTE_RESET:
        case ICF_TRACE_CALL_FAIL_NO_SDP:
        case ICF_PA_MEDIA_CONNECT:
        case ICF_PA_CALL_HOLD_IND:
        case ICF_PA_CALL_RESUME_IND:
        case ICF_PA_CONNECT_IND:
        case ICF_PA_TERMINATE_CALL_RESP:

            ICF_PRINT(((icf_uint8_t *)">> Call_id = %d\n", p_trace_data->int_val[0]));
            break;

        case ICF_TRACE_SEND_MMI_REQ:
            ICF_PRINT(((icf_uint8_t *)">> Call_id = %d\n", p_trace_data->int_val[0]));
            ICF_PRINT(((icf_uint8_t *)">> Event SEND MMI REQ = %s\n", p_trace_data->string_val[0].str));
            
            break;

        case ICF_TRACE_DMS_REQ_PAYLOAD:
            ICF_PRINT(((icf_uint8_t *)">> Call_id = %d\n", p_trace_data->int_val[0]));
            ICF_PRINT(((icf_uint8_t *)">> Trace for DMS REQ PAYLOAD = %s\n", p_trace_data->string_val[0].str));

            break;

        case ICF_TRACE_MMS_REQ_PAYLOAD:
            ICF_PRINT(((icf_uint8_t *)">> Call_id = %d\n", p_trace_data->int_val[0]));
            ICF_PRINT(((icf_uint8_t *)">> MMS REQ PAYLOAD = %d\n", p_trace_data->int_val[1]));

            break;

        case ICF_INTERFACE_FAILURE:


            ICF_PRINT(((icf_uint8_t *)">> Call_id = %d\n", p_trace_data->int_val[0]));
            ICF_PRINT(((icf_uint8_t *)">> Line_id = %d\n", p_trace_data->int_val[1]));
            ICF_PRINT(((icf_uint8_t *)">> Message_id = %d\n", p_trace_data->int_val[2]));
            
            break;


        case ICF_PA_INCOMING_CALL_IND:
            
             ICF_PRINT(((icf_uint8_t *)">> Call_id = %d\n", p_trace_data->int_val[0]));
             ICF_PRINT(((icf_uint8_t *)">> ---someFieldHere--- = %d\n", p_trace_data->int_val[1]));

             ICF_PRINT(((icf_uint8_t *)">> Call_Waiting_Call = %d\n", p_trace_data->int_val[2]));
 
             ICF_PRINT(((icf_uint8_t *)">> Remote_Asserted_Address = %d\n", p_trace_data->int_val[3]));
 
            
            ICF_PRINT(((icf_uint8_t *)">> Remote Party Address = %s\n", p_trace_data->string_val[0].str));
			
			if(2 == p_trace_data->number_of_string_values)
            ICF_PRINT(((icf_uint8_t *)">> Remote Party Name  = %s\n", p_trace_data->string_val[1].str));
 
            break;


        case ICF_PA_ALERT_USER_IND:
             ICF_PRINT(((icf_uint8_t *)">> Call_id = %d\n", p_trace_data->int_val[0]));
             ICF_PRINT(((icf_uint8_t *)">> Inbacnd Info Present = %d\n", p_trace_data->int_val[1]));

            break;

        case ICF_PA_CALL_TERM_IND:
             ICF_PRINT(((icf_uint8_t *)">> Call_id = %d\n", p_trace_data->int_val[0]));
             ICF_PRINT(((icf_uint8_t *)">> Release Cause = %d\n", p_trace_data->int_val[1]));
   
            break;

        case ICF_PA_CALL_REDIRECTION_IND:
            ICF_PRINT(((icf_uint8_t *)">> Call_id = %d\n", p_trace_data->int_val[0]));
            ICF_PRINT(((icf_uint8_t *)">> Redirection Address = %s\n", p_trace_data->string_val[0].str));

            ICF_PRINT(((icf_uint8_t *)">> Redirection Name = %s\n", p_trace_data->string_val[1].str));

            break;

        case ICF_UNEXPECTED_EVENT:
            ICF_PRINT(((icf_uint8_t *)">> Call_id = %d\n", p_trace_data->int_val[0]));
            icf_port_log_cc_event_to_str((icf_uint8_t)p_trace_data->int_val[1],temp.str);
            ICF_PRINT(((icf_uint8_t *)">>  Current Event(Unexpected) = %s\n", temp.str));

            icf_port_log_cc_state_to_str((icf_uint8_t)p_trace_data->int_val[2],temp.str);
            ICF_PRINT(((icf_uint8_t *)">> Call State = %s\n", temp.str));

            break;


        case ICF_TRANSPORT_ADDRESS:
            ICF_PRINT(((icf_uint8_t *)">> Call_id = %d\n", p_trace_data->int_val[0]));
            ICF_PRINT(((icf_uint8_t *)">> Remote Address type= %d\n", p_trace_data->int_val[1]));
            ICF_PRINT(((icf_uint8_t *)">> Local Address Type = %d\n", p_trace_data->int_val[2]));
            ICF_PRINT(((icf_uint8_t *)">> Remote Address String = %s\n", p_trace_data->string_val[0].str));

            ICF_PRINT(((icf_uint8_t *)">> Local Address String = %s\n", p_trace_data->string_val[1].str));
            break;

            

        case ICF_TRACE_SRVC_TRIG_STAT_RET:
            ICF_PRINT(((icf_uint8_t *)">> Call_id = %d\n", p_trace_data->int_val[0]));
            ICF_PRINT(((icf_uint8_t *)">> Status returned to CM = %d\n", 
                        p_trace_data->int_val[1]));
            break;

    default:
            {
#ifdef ICF_DETAILED_DEBUG_TRACE
                max_count = p_trace_data->num_of_integer_values;

                ICF_PRINT(((icf_uint8_t *)"\tDATA:integer(s):"));
                for ( count = 0; count < max_count; count ++)
                {
                    ICF_PRINT(((icf_uint8_t *)" %lu\n",p_trace_data->int_val[count]));
                }

                max_count = p_trace_data->number_of_string_values;
                ICF_PRINT(((icf_uint8_t *)"\nDATA:string(s):"));
                for ( count = 0; count < max_count; count ++)
                {
                    ICF_PRINT(((icf_uint8_t *)"\t %s\n",p_trace_data->string_val[count].str));
                }
                ICF_PRINT(((icf_uint8_t *)"\n"));
#endif
            }
            break;

    

    } /* end of switch-case for trace_id */

    /* Finally, invoke the common macro for trace handling */
/*    ICF_TRACE(p_cc_pdb->p_glb_pdb, trace_id, &trace_data)*/
    
} /* end of icf_port_log_cc_trace() */
#endif /* end of #ifdef ICF_TRACE_ENABLE */


#ifdef ICF_TRACE_ENABLE
/*******************************************************************************
 *
 * FUNCTION:        icf_port_log_cc_mmtyp_to_str
 *
 * DESCRIPTION:     This function returns a string corresponding to media modify
 *                  type and is used only for traces.
 *
 ******************************************************************************/
icf_void_t icf_port_log_cc_mmtyp_to_str(
        IN icf_uint8_t mms_type,
        OUT icf_uint8_t *p_mms_type_str)
{
    switch(mms_type)
    {
/* These fileds have been chenged in 6.0
        case ICF_MODIFY_FOR_LOCAL_CODEC_CHANGE:
            icf_port_strcpy(p_mms_type_str,
                    (icf_uint8_t *)"ICF_MODIFY_FOR_LOCAL_CODEC_CHANGE");
            break;

        case ICF_MODIFY_FOR_REMOTE_CODEC_CHANGE:
            icf_port_strcpy(p_mms_type_str,
                    (icf_uint8_t *)"ICF_MODIFY_FOR_REMOTE_CODEC_CHANGE");
            break;

        case ICF_MODIFY_FOR_REMOTE_MEDIA_CHANGE:
            icf_port_strcpy(p_mms_type_str,
                    (icf_uint8_t *)"ICF_MODIFY_FOR_REMOTE_MEDIA_CHANGE");
            break;
        
        case ICF_MODIFY_FOR_LOCAL_MEDIA_CHANGE:
            icf_port_strcpy(p_mms_type_str,
                    (icf_uint8_t *)"ICF_MODIFY_FOR_LOCAL_MEDIA_CHANGE");
            break;

        case ICF_MODIFY_FOR_MODE_CHANGE:
            icf_port_strcpy(p_mms_type_str,
                    (icf_uint8_t *)"ICF_MODIFY_FOR_MODE_CHANGE");
            break;

        case ICF_MODIFY_FOR_REMOTE_MEDIA_APPLICATION:
            icf_port_strcpy(p_mms_type_str,
                (icf_uint8_t *)"ICF_MODIFY_FOR_REMOTE_MEDIA_APPLICATION");
            break;
*/
        default:
            icf_port_strcpy(p_mms_type_str,
                    (icf_uint8_t *)"ICF_NULL");
            break;

    } /* end of switch case for mms_type */
} /* end of icf_port_log_cc_media_to_str() */
#endif /* end of #ifdef ICF_TRACE_ENABLE */

