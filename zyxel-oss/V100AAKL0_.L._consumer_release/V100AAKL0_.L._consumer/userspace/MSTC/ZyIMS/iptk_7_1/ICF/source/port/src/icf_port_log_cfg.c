/*------------------------------------------------------------------------------
 *
 * File name        : icf_port_log_cfg.c
 *
 * Purpose          : This file contains error/trace printing routines of CFG.
 *
 * Revision History :
 *
 * Date         Name            Ref#            Comments
 * --------     ------------    ----            ---------------
 * 07-Jan-2005  Aashish Suchdev                 Initial
 * 17-Feb-2005  Rohit Aggarwal                  Added function definitions
 * 29 APr-2005  Shradha Budhiraja 7453          Adding ECODE
 * 02-Feb-2006  Amit Sharma                     CFG merged with ICF 2.0
 *
 * Copyright (c) 2006, Aricent.
 *----------------------------------------------------------------------------*/

#ifdef ICF_PORT_SOLARIS
#include <stdio.h>
#endif

#include "icf_common_types.h"
#include "icf_port_prototypes.h"
#include "icf_port_defs.h"
#ifdef ICF_PORT_WINDOWS
#pragma warning ( disable : 4702 )
#endif


#ifdef ICF_ERROR_ENABLE
#ifndef ICF_SM_LOG_ENABLE
/*******************************************************************************
 *
 * FUNCTION:        icf_port_log_cfg_error
 *
 * DESCRIPTION:     This function is used to print the error data from CFG.
 *
 * RETURNS:         Nothing
 *
 *
 ******************************************************************************/
icf_void_t icf_port_log_cfg_error(
        IN  icf_error_id_t       error_id,
        IN  icf_error_data_st    *p_error_data)
{
    switch (error_id)
    {
        /* To remove unused parameter warnings */
         p_error_data = p_error_data; 
        case ICF_ERROR_INIT_CMPLT:
            ICF_PRINT(((icf_uint8_t *)"\n>> All Mandatory Parameters have not been recd"));
            break;
        
        case ICF_ERROR_ES_INIT_CMPLT:
            ICF_PRINT(((icf_uint8_t *)"\n>> Failure in ES init Complete"));
	    break;        

        case ICF_ERROR_NW_INTERFACE:
            ICF_PRINT(((icf_uint8_t *)"\n>> Failure in Network Interface"));
            break; 

        case ICF_ERROR_RGM_INIT_CMPLT:
            ICF_PRINT(((icf_uint8_t *)"\n>> Failure in RGM init Complete"));
            break; 

        case ICF_ERROR_DBM_PORT_DATA:
            ICF_PRINT(((icf_uint8_t *)"\n>> Failure in fetching DBM port data"));
            break;

        default:
            ICF_PRINT(((icf_uint8_t *)"\n"));
            break;
    }
}
#endif
#endif /* End of ICF_ERROR_ENABLE */


#ifdef ICF_TRACE_ENABLE
#ifndef ICF_SM_LOG_ENABLE
/*******************************************************************************
 *
 * FUNCTION:        icf_port_log_cfg_trace
 *
 * DESCRIPTION:     This function is used to print the trace data from CFG.
 *
 * RETURNS:         Nothing
 *
 *
 ******************************************************************************/
icf_void_t icf_port_log_cfg_trace(
        IN  icf_trace_id_t       trace_id,
        IN  icf_trace_data_st    *p_trace_data)
{
    switch (trace_id)
    {
        case ICF_TRACE_INVALID_MSG_ID_RECD:
            ICF_PRINT(((icf_uint8_t *)"\n>> msg_id = %d", p_trace_data->int_val[0]));
            break;

        case ICF_TRACE_INVALID_API_ID_RECD:
            ICF_PRINT(((icf_uint8_t *)"\n>> msg_id = %d", p_trace_data->int_val[0]));
            ICF_PRINT(((icf_uint8_t *)"\n>> api_id = %d", p_trace_data->int_val[1]));
            break;

        case ICF_MSG_PROC_RECV_INT_MSG:
            ICF_PRINT(((icf_uint8_t *)"\n>> msg_id = %d", p_trace_data->int_val[0]));
            if (ICF_TWO_INT_PRESENT == p_trace_data->num_of_integer_values)
            {
                ICF_PRINT(((icf_uint8_t *)"\n>> api_id = %d", p_trace_data->int_val[1]));
            }
            break;

        case ICF_TRACE_INIT_COMPLETE_STATUS:
            ICF_PRINT(((icf_uint8_t *)"\n>> config_data_init_complete = %d", 
                        p_trace_data->int_val[0]));
            ICF_PRINT(((icf_uint8_t *)"\n>> global_data_init_complete = %d", 
                        p_trace_data->int_val[1]));
            break;
        
        case ICF_TRACE_INIT_COMPLETE_RECD:
            ICF_PRINT(((icf_uint8_t *)"\n>> config_data_init_complete = %d", 
                        p_trace_data->int_val[0]));
            ICF_PRINT(((icf_uint8_t *)"\n>> global_data_init_complete = %d", 
                        p_trace_data->int_val[1]));
            break;

        case ICF_TRACE_PARAM_VALIDITY:
            ICF_PRINT(((icf_uint8_t *)"\n>> validity = %d", p_trace_data->int_val[0]));
            break;
        
        case ICF_CFG_TRACE_SCALABLE_LINE_PRMS_REQ_RECD:
            ICF_PRINT(((icf_uint8_t *)"\n>>   Max_lines = %d",p_trace_data->int_val[0]));
            ICF_PRINT(((icf_uint8_t *)">>   max_users_per_line = %d", 
                        p_trace_data->int_val[1]));
            ICF_PRINT(((icf_uint8_t *)">>Number of calls to log = %d", 
                        p_trace_data->int_val[2]));
            break;
        
        case ICF_CFG_TRACE_SCALABLE_CALL_PRMS_REQ_RECD:
            ICF_PRINT(((icf_uint8_t *)"\n>>   max_audio_calls = %d", 
                        p_trace_data->int_val[0]));
            ICF_PRINT(((icf_uint8_t *)">>   max_video_calls = %d", 
                        p_trace_data->int_val[1]));
            ICF_PRINT(((icf_uint8_t *)">>   max_service_calls = %d", 
                        p_trace_data->int_val[2]));
            ICF_PRINT(((icf_uint8_t *)">>   max_registration_calls = %d", 
                        p_trace_data->int_val[3]));
            break;

        case ICF_CFG_TRACE_TRNSPT_PRMS_REQ_RECD:
            ICF_PRINT(((icf_uint8_t *)"\n>>   presence_flag = %d", 
                        p_trace_data->int_val[0]));
            ICF_PRINT(((icf_uint8_t *)">>   Self Transport Mode = %d", 
                        p_trace_data->int_val[1]));
            ICF_PRINT(((icf_uint8_t *)">>   Self Address's port_num = %d", 
                        p_trace_data->int_val[2]));
            break;

        case ICF_CFG_TRACE_PROXY_ADDR_REQ_RECD:
            ICF_PRINT(((icf_uint8_t *)">> Line Id = %d", 
                        p_trace_data->int_val[0]));
            ICF_PRINT(((icf_uint8_t *)">> Action = %d", 
                        p_trace_data->int_val[1]));
            break;
        
        case ICF_CFG_TRACE_PROXY_ADDR_LINE_RECD:
            ICF_PRINT(((icf_uint8_t *)"\n>> Proxy Address type = %d", 
                        p_trace_data->int_val[0]));
            ICF_PRINT(((icf_uint8_t *)"\n>> Mode = %d", 
                        p_trace_data->int_val[1]));
			ICF_PRINT(((icf_uint8_t *)"\n>> Port = %d", 
                        p_trace_data->int_val[1]));

            break;

        case ICF_CFG_TRACE_PROXY_ADDR_RECD:
            if(ICF_FOUR_INT_PRESENT == 
                    p_trace_data->num_of_integer_values)
            {
                ICF_PRINT(((icf_uint8_t *)"\n>>   Proxy address = %d", 
                            p_trace_data->int_val[0]));
                ICF_PRINT(((icf_uint8_t *)".%d", 
                            p_trace_data->int_val[1]));
                ICF_PRINT(((icf_uint8_t *)".%d", 
                            p_trace_data->int_val[2]));
                ICF_PRINT(((icf_uint8_t *)" .%d", 
                            p_trace_data->int_val[3]));
            }
            else
            {
                ICF_PRINT(((icf_uint8_t *)"\n>>   Proxy Address = %s", 
                            p_trace_data->string_val[0].str));
            }
            break;
        
        case ICF_CFG_TRACE_SELF_TRNSPT_ADDR_RECD:
            if(ICF_FOUR_INT_PRESENT == 
                    p_trace_data->num_of_integer_values)
            {
                ICF_PRINT(((icf_uint8_t *)"\n>>   Self_transport address = %d", 
                            p_trace_data->int_val[0]));
                ICF_PRINT(((icf_uint8_t *)".%d", 
                            p_trace_data->int_val[1]));
                ICF_PRINT(((icf_uint8_t *)".%d", 
                            p_trace_data->int_val[2]));
                ICF_PRINT(((icf_uint8_t *)" .%d", 
                            p_trace_data->int_val[3]));
            }
            else
            {
                ICF_PRINT(((icf_uint8_t *)"\n>>   Self Transport Address = %s", 
                        p_trace_data->string_val[0].str));
            }
            break;
        
        case ICF_CFG_TRACE_CALL_PARAMS_RECD:
            ICF_PRINT(((icf_uint8_t *)"\n>>   presence_flag = %d", 
                        p_trace_data->int_val[0]));
            break;
        
        case ICF_CFG_TRACE_CALL_DEFAULT_SETTING_N_QOS_RECD:
            ICF_PRINT(((icf_uint8_t *)"\n>>   default_settings = %d", 
                        p_trace_data->int_val[0]));
            ICF_PRINT(((icf_uint8_t *)">>   QOS Mark = %d", 
                        p_trace_data->int_val[1]));
            break;

        case ICF_CFG_TRACE_CALL_TIMERS_RECD:
            ICF_PRINT(((icf_uint8_t *)"\n>>   setup_timer = %d", 
                        p_trace_data->int_val[0]));
            ICF_PRINT(((icf_uint8_t *)">>   alerting_timer = %d", 
                        p_trace_data->int_val[1]));
            ICF_PRINT(((icf_uint8_t *)">>   release_timer = %d", 
                        p_trace_data->int_val[2]));
            ICF_PRINT(((icf_uint8_t *)">>   modify_media_timer = %d", 
                        p_trace_data->int_val[3]));
#ifdef ICF_SESSION_TIMER
			ICF_PRINT(((icf_uint8_t *)">>   release_timer = %d", 
                        p_trace_data->int_val[2]));
            ICF_PRINT(((icf_uint8_t *)">>   modify_media_timer = %d", 
                        p_trace_data->int_val[3]));
#endif
            break;
        
        case ICF_CFG_TRACE_REG_TIMERS_RECD:
            ICF_PRINT(((icf_uint8_t *)"\n>>   registration_resp_timer = %d", 
                        p_trace_data->int_val[0]));
            ICF_PRINT(((icf_uint8_t *)">>   registration_retry_timer = %d", 
                        p_trace_data->int_val[1]));
            break;
        
        case ICF_CFG_TRACE_SELF_ID_PRMS_RECD:
            ICF_PRINT(((icf_uint8_t *)"\n>>   presence_flag = %d", 
                        p_trace_data->int_val[0]));
            ICF_PRINT(((icf_uint8_t *)">>   line_id   = %d", 
                        p_trace_data->int_val[1]));
            ICF_PRINT(((icf_uint8_t *)">>   action = %d", 
                        p_trace_data->int_val[2]));
            ICF_PRINT(((icf_uint8_t *)">>   priority = %d", 
                        p_trace_data->int_val[3]));
            break;
        
        case ICF_CFG_TRACE_SELF_ID_ADDR_PRMS_RECD:
            ICF_PRINT(((icf_uint8_t *)"\n>>   Self address's all_lines = %d", 
                        p_trace_data->int_val[0]));
            ICF_PRINT(((icf_uint8_t *)">>   Self_address's num_of_elements = %d", 
                        p_trace_data->int_val[1]));
            break;
        
        case ICF_CFG_TRACE_SELF_ID_NAME_PRMS_RECD:
            ICF_PRINT(((icf_uint8_t *)"\n>>   Self name's all_lines = %d", 
                        p_trace_data->int_val[0]));
            ICF_PRINT(((icf_uint8_t *)">>   Self name's num_of_elements = %d", 
                        p_trace_data->int_val[1]));
            break;
        
        case ICF_CFG_TRACE_SELF_ID_DEF_PRMS_RECD:
            ICF_PRINT(((icf_uint8_t *)"\n>>   Self def_settings' all_lines = %d", 
                        p_trace_data->int_val[0]));
            ICF_PRINT(((icf_uint8_t *)">>   Self def_settings' num_of_elements = %d", 
                        p_trace_data->int_val[1]));
            break;
        
        case ICF_CFG_TRACE_SELF_ADDR_RECD:
            ICF_PRINT(((icf_uint8_t *)"\n>> Line ID = %d", 
                        p_trace_data->int_val[0]));
            ICF_PRINT(((icf_uint8_t *)">> Action = %d", 
                        p_trace_data->int_val[1]));
            ICF_PRINT(((icf_uint8_t *)">> Address Type = %d", 
                        p_trace_data->int_val[2]));
            ICF_PRINT(((icf_uint8_t *)">>   Self Address = %s", 
                        p_trace_data->string_val[0].str));
            break;
        
        case ICF_CFG_TRACE_SELF_NAME_RECD:
            ICF_PRINT(((icf_uint8_t *)"\n>> Line ID = %d", 
                        p_trace_data->int_val[0]));
            ICF_PRINT(((icf_uint8_t *)">> Action = %d", 
                        p_trace_data->int_val[1]));
            ICF_PRINT(((icf_uint8_t *)">>   Self Name = %s", 
                        p_trace_data->string_val[0].str));
            break;
        
        case ICF_CFG_TRACE_SELF_DEF_RECD:
            ICF_PRINT(((icf_uint8_t *)"\n>> Line ID = %d", 
                        p_trace_data->int_val[0]));
            ICF_PRINT(((icf_uint8_t *)">> Self Default Settings = %d", 
                        p_trace_data->int_val[1]));
            break;
        
        case ICF_CFG_TRACE_REG_PRMS_REQ_RECD:
            ICF_PRINT(((icf_uint8_t *)"\n>>   Registration params' all_lines = %d", 
                        p_trace_data->int_val[0]));
            ICF_PRINT(((icf_uint8_t *)">>   Registration param' num_of_elements = %d", 
                        p_trace_data->int_val[1]));
            break;
        
        case ICF_CFG_TRACE_REG_ADDR_PRMS_RECD:
            ICF_PRINT(((icf_uint8_t *)"\n>> Line ID = %d", 
                        p_trace_data->int_val[0]));
            ICF_PRINT(((icf_uint8_t *)">> Action = %d", 
                        p_trace_data->int_val[1]));
            ICF_PRINT(((icf_uint8_t *)">> presence_flag = %d", 
                        p_trace_data->int_val[2]));
            ICF_PRINT(((icf_uint8_t *)">> Registrar_addr's port_num = %d", 
                        p_trace_data->int_val[3]));
            break;
        
        case ICF_CFG_TRACE_REG_ADDR_RECD:
            if(ICF_FOUR_INT_PRESENT == 
                    p_trace_data->num_of_integer_values)
            {
                ICF_PRINT(((icf_uint8_t *)"\n>>   Registrar address = %d", 
                            p_trace_data->int_val[0]));
                ICF_PRINT(((icf_uint8_t *)".%d", 
                            p_trace_data->int_val[1]));
                ICF_PRINT(((icf_uint8_t *)".%d", 
                            p_trace_data->int_val[2]));
                ICF_PRINT(((icf_uint8_t *)".%d", 
                            p_trace_data->int_val[3]));
            }
            else if(ICF_ONE_STRING_PRESENT == 
                   p_trace_data->number_of_string_values)
            {
                ICF_PRINT(((icf_uint8_t *)"\n>> Registrar Address = %s", 
                            p_trace_data->string_val[0].str));
            }
            break;

        case ICF_CFG_TRACE_REG_PRMS_RECD:
            ICF_PRINT(((icf_uint8_t *)"\n>> Port Num = %d", 
                        p_trace_data->int_val[0]));
            ICF_PRINT(((icf_uint8_t *)"\n>> Registration Mode = %d", 
                        p_trace_data->int_val[1]));
            ICF_PRINT(((icf_uint8_t *)"\n>> Registration Duration = %d", 
                        p_trace_data->int_val[2]));            
            break;
        
        case ICF_CFG_TRACE_SRVC_PRMS_REQ_RECD:
            ICF_PRINT(((icf_uint8_t *)"\n>> presence_flag = %d", 
                        p_trace_data->int_val[0]));
            ICF_PRINT(((icf_uint8_t *)">> Call_hold_option = %d", 
                        p_trace_data->int_val[1]));
            ICF_PRINT(((icf_uint8_t *)">> max_num_of_call_forwarding_hops =  %d", 
                        p_trace_data->int_val[2]));
            ICF_PRINT(((icf_uint8_t *)">> max_num_of_calls_in_call_waiting = %d", 
                        p_trace_data->int_val[3]));
            break;
        
        case ICF_CFG_TRACE_SRVC_TIMERS_RECD:
            ICF_PRINT(((icf_uint8_t *)"\n>> call_hold_resume_timer = %d", 
                        p_trace_data->int_val[0]));
            ICF_PRINT(((icf_uint8_t *)">> reinvite_race_timer = %d", 
                        p_trace_data->int_val[1]));
            ICF_PRINT(((icf_uint8_t *)">> twc_timer =  %d", 
                        p_trace_data->int_val[2]));
            ICF_PRINT(((icf_uint8_t *)">> cw_alert_timer = %d", 
                        p_trace_data->int_val[3]));
            break;
        
        case ICF_CFG_TRACE_CALL_FWD_TIMERS_RECD:
            ICF_PRINT(((icf_uint8_t *)"\n>> ctu_timer = %d", 
                        p_trace_data->int_val[0]));
            ICF_PRINT(((icf_uint8_t *)">> cta_timer = %d", 
                        p_trace_data->int_val[1]));
            ICF_PRINT(((icf_uint8_t *)">> cnd_alert_timer =  %d", 
                        p_trace_data->int_val[2]));
            ICF_PRINT(((icf_uint8_t *)">> cw_n_cnd_alert_timer = %d", 
                        p_trace_data->int_val[3]));
            break;
        
        case ICF_CFG_TRACE_SRVC_FLAGS_PRMS_RECD:
            ICF_PRINT(((icf_uint8_t *)"\n>> Service Subs flag's all_lines = %d", 
                        p_trace_data->int_val[0]));
            ICF_PRINT(((icf_uint8_t *)">> Service Subs flag's num_of_elements = %d", 
                        p_trace_data->int_val[1]));
            ICF_PRINT(((icf_uint8_t *)"\n>> Service Act flag's all_lines =  %d", 
                        p_trace_data->int_val[2]));
            ICF_PRINT(((icf_uint8_t *)">> Service Act flag's num_of_elements = %d", 
                        p_trace_data->int_val[3]));
            break;
        
        case ICF_CFG_TRACE_SRVC_FLAGS_RECD:
            ICF_PRINT(((icf_uint8_t *)"\n>> Service Subs flag's line id = %d", 
                        p_trace_data->int_val[0]));
            ICF_PRINT(((icf_uint8_t *)">> Service Subs flag's status = %d", 
                        p_trace_data->int_val[1]));
            ICF_PRINT(((icf_uint8_t *)"\n>> Service Act flag's line id =  %d", 
                        p_trace_data->int_val[2]));
            ICF_PRINT(((icf_uint8_t *)">> Service Act flag's status = %d", 
                        p_trace_data->int_val[3]));
            break;
        
        case ICF_CFG_TRACE_CFU_ADDR_PRMS_RECD:
            ICF_PRINT(((icf_uint8_t *)"\n>>   CFU Address' all_lines = %d", 
                        p_trace_data->int_val[0]));
            ICF_PRINT(((icf_uint8_t *)">>   CFU Address' num_of_elements = %d", 
                        p_trace_data->int_val[1]));
            break;
        
        case ICF_CFG_TRACE_CFU_ADDR_RECD:
            ICF_PRINT(((icf_uint8_t *)"\n>> Line ID = %d", 
                        p_trace_data->int_val[0]));
            ICF_PRINT(((icf_uint8_t *)">> Address Type = %d", 
                        p_trace_data->int_val[1]));
            ICF_PRINT(((icf_uint8_t *)">>   CFU Address = %s", 
                        p_trace_data->string_val[0].str));
            break;
        
        case ICF_CFG_TRACE_CFB_ADDR_PRMS_RECD:
            ICF_PRINT(((icf_uint8_t *)"\n>>   CFB Address' all_lines = %d", 
                        p_trace_data->int_val[0]));
            ICF_PRINT(((icf_uint8_t *)">>   CFB Address' num_of_elements = %d", 
                        p_trace_data->int_val[1]));
            break;
        
        case ICF_CFG_TRACE_CFB_ADDR_RECD:
            ICF_PRINT(((icf_uint8_t *)"\n>> Line ID = %d", 
                        p_trace_data->int_val[0]));
            ICF_PRINT(((icf_uint8_t *)">> Address Type = %d", 
                        p_trace_data->int_val[1]));
            ICF_PRINT(((icf_uint8_t *)">>   CFB Address = %s", 
                        p_trace_data->string_val[0].str));
            break;
        
        case ICF_CFG_TRACE_CFD_ADDR_PRMS_RECD:
            ICF_PRINT(((icf_uint8_t *)"\n>>   CFD Address' all_lines = %d", 
                        p_trace_data->int_val[0]));
            ICF_PRINT(((icf_uint8_t *)">>   CFD Address' num_of_elements = %d", 
                        p_trace_data->int_val[1]));
            break;
        
        case ICF_CFG_TRACE_CFD_ADDR_RECD:
            ICF_PRINT(((icf_uint8_t *)"\n>> Line ID = %d", 
                        p_trace_data->int_val[0]));
            ICF_PRINT(((icf_uint8_t *)">> Address Type = %d", 
                        p_trace_data->int_val[1]));
            ICF_PRINT(((icf_uint8_t *)">>   CFD Address = %s", 
                        p_trace_data->string_val[0].str));
            break;
        
        case ICF_CFG_TRACE_RESP_SENT:
            ICF_PRINT(((icf_uint8_t *)"\n>> API ID = %d", 
                        p_trace_data->int_val[0]));
            ICF_PRINT(((icf_uint8_t *)"\n>> Result = %d", 
                        p_trace_data->int_val[1]));
            ICF_PRINT(((icf_uint8_t *)"\n>> Error Code = %d", 
                        p_trace_data->int_val[2]));
            ICF_PRINT(((icf_uint8_t *)"\n>> Error Cause = %s", 
                                p_trace_data->string_val[0].str));

            break;

       case ICF_CFG_TRACE_SCALABLE_LINE_PRMS_SET:
            ICF_PRINT(((icf_uint8_t *)"\n>> Max_lines = %d",p_trace_data->int_val[0]));
            ICF_PRINT(((icf_uint8_t *)">> max_users_per_line = %d", 
                        p_trace_data->int_val[1]));
            ICF_PRINT(((icf_uint8_t *)">> num_of_calls_to_log = %d", 
                        p_trace_data->int_val[2]));
            break;
        
       case ICF_CFG_TRACE_SCALABLE_CALL_PRMS_SET:
            ICF_PRINT(((icf_uint8_t *)"\n>> max_audio_calls = %d", 
                        p_trace_data->int_val[0]));
            ICF_PRINT(((icf_uint8_t *)">> max_video_calls = %d", 
                        p_trace_data->int_val[1]));
            ICF_PRINT(((icf_uint8_t *)">> max_service_calls = %d", 
                        p_trace_data->int_val[2]));
            ICF_PRINT(((icf_uint8_t *)">> max_registration_calls = %d", 
                        p_trace_data->int_val[3]));
            break;        
 
       case ICF_CFG_TRACE_PROXY_ADDR_LINE_SET:
            ICF_PRINT(((icf_uint8_t *)"\n>> Line ID = %d", 
                        p_trace_data->int_val[0]));
            ICF_PRINT(((icf_uint8_t *)">> Proxy's transport Mode = %d", 
                        p_trace_data->int_val[1]));
            ICF_PRINT(((icf_uint8_t *)">> Proxy Address's port_num = %d", 
                        p_trace_data->int_val[2]));
            break;

        case ICF_CFG_TRACE_PROXY_ADDRESS_SET:
            if(ICF_FOUR_INT_PRESENT == 
                    p_trace_data->num_of_integer_values)
            {
                ICF_PRINT(((icf_uint8_t *)"\n>> Proxy address = %d", 
                            p_trace_data->int_val[0]));
                ICF_PRINT(((icf_uint8_t *)".%d", 
                            p_trace_data->int_val[1]));
                ICF_PRINT(((icf_uint8_t *)".%d", 
                            p_trace_data->int_val[2]));
                ICF_PRINT(((icf_uint8_t *)".%d", 
                            p_trace_data->int_val[3]));
            }
            else if(ICF_ONE_STRING_PRESENT == 
                            p_trace_data->number_of_string_values)
            {
                ICF_PRINT(((icf_uint8_t *)"\n>> Proxy Address = %s", 
                            p_trace_data->string_val[0].str));
            }

            break;
        
        case ICF_CFG_TRACE_SELF_TRNSPT_ADDR_SET:
            ICF_PRINT(((icf_uint8_t *)"\n>> Self_transport address = %d", 
                        p_trace_data->int_val[0]));
            ICF_PRINT(((icf_uint8_t *)".%d", 
                        p_trace_data->int_val[1]));
            ICF_PRINT(((icf_uint8_t *)".%d", 
                        p_trace_data->int_val[2]));
            ICF_PRINT(((icf_uint8_t *)".%d", 
                        p_trace_data->int_val[3]));
            break;

       case ICF_CFG_TRACE_SELF_TRNSPT_MODE_SET:
            ICF_PRINT(((icf_uint8_t *)"\n>> Port num = %d",p_trace_data->int_val[0]));
            ICF_PRINT(((icf_uint8_t *)">> Self transport mode = %d", 
                        p_trace_data->int_val[1]));
            break;
        
        case ICF_CFG_TRACE_CALL_DEFAULT_SETTING_N_QOS_SET:
            ICF_PRINT(((icf_uint8_t *)"\n>> Default_settings = %d", 
                        p_trace_data->int_val[0]));
            ICF_PRINT(((icf_uint8_t *)">> QOS Mark = %d", 
                        p_trace_data->int_val[1]));
            break;

        case ICF_CFG_TRACE_CALL_TIMERS_SET:
            ICF_PRINT(((icf_uint8_t *)"\n>> setup_timer = %d", 
                        p_trace_data->int_val[0]));
            ICF_PRINT(((icf_uint8_t *)">> alerting_timer = %d", 
                        p_trace_data->int_val[1]));
            ICF_PRINT(((icf_uint8_t *)">> release_timer = %d", 
                        p_trace_data->int_val[2]));
            ICF_PRINT(((icf_uint8_t *)">> modify_media_timer = %d", 
                        p_trace_data->int_val[3]));
            break;
        
        case ICF_CFG_TRACE_REG_TIMERS_SET:
            ICF_PRINT(((icf_uint8_t *)"\n>> registration_resp_timer = %d", 
                        p_trace_data->int_val[0]));
            ICF_PRINT(((icf_uint8_t *)">> registration_retry_timer = %d", 
                        p_trace_data->int_val[1]));
            break; 
        
        case ICF_CFG_TRACE_MAX_NUM_OF_USERS_REACHED:
            ICF_PRINT(((icf_uint8_t *)"\n>> Line ID = %d", 
                        p_trace_data->int_val[0]));
            ICF_PRINT(((icf_uint8_t *)">> Present number of user address = %d", 
                        p_trace_data->int_val[1]));
            break; 
 
        case ICF_CFG_TRACE_SELF_ADDR_SET:
            ICF_PRINT(((icf_uint8_t *)"\n>> Line ID = %d", 
                        p_trace_data->int_val[0]));
            ICF_PRINT(((icf_uint8_t *)">> User ID = %d", 
                        p_trace_data->int_val[1]));
            ICF_PRINT(((icf_uint8_t *)">> Address Type = %d", 
                        p_trace_data->int_val[2]));
            ICF_PRINT(((icf_uint8_t *)">>   Self Address = %s", 
                        p_trace_data->string_val[0].str));
            break;
        
        case ICF_CFG_TRACE_SELF_NAME_SET:
            ICF_PRINT(((icf_uint8_t *)"\n>> Line ID = %d", 
                        p_trace_data->int_val[0]));
            ICF_PRINT(((icf_uint8_t *)">>   Self Name = %s", 
                        p_trace_data->string_val[0].str));
            break;
        
        case ICF_CFG_TRACE_SELF_DEFAULT_SETTINGS_SET:
            ICF_PRINT(((icf_uint8_t *)"\n>> Line ID = %d", 
                        p_trace_data->int_val[0]));
            ICF_PRINT(((icf_uint8_t *)">> Self Default Settings = %d", 
                        p_trace_data->int_val[1]));
            break;
        
        case ICF_CFG_TRACE_LINE_BLK_DEALLOCATED:
            ICF_PRINT(((icf_uint8_t *)"\n>> Line ID = %d", 
                        p_trace_data->int_val[0]));
            break;
        
        case ICF_CFG_TRACE_REQ_ALREADY_RECD:
            ICF_PRINT(((icf_uint8_t *)"\n>> API ID = %d", 
                        p_trace_data->int_val[0]));
            break;
  
        case ICF_CFG_TRACE_REG_ADDR_SET:
            if(ICF_FOUR_INT_PRESENT == 
                    p_trace_data->num_of_integer_values)
            {
                ICF_PRINT(((icf_uint8_t *)"\n>> Registrar address = %d", 
                            p_trace_data->int_val[0]));
                ICF_PRINT(((icf_uint8_t *)".%d", 
                            p_trace_data->int_val[1]));
                ICF_PRINT(((icf_uint8_t *)".%d", 
                            p_trace_data->int_val[2]));
                ICF_PRINT(((icf_uint8_t *)".%d", 
                            p_trace_data->int_val[3]));
            }
            else if(ICF_ONE_STRING_PRESENT == 
                            p_trace_data->number_of_string_values)
            {
                ICF_PRINT(((icf_uint8_t *)"\n>> Registrar Address = %s", 
                            p_trace_data->string_val[0].str));
            }
            break;
        
        case ICF_CFG_TRACE_REG_PRMS_SET:
            ICF_PRINT(((icf_uint8_t *)"\n>> Line ID = %d", 
                        p_trace_data->int_val[0]));
            ICF_PRINT(((icf_uint8_t *)">> Registrar port Num = %d", 
                        p_trace_data->int_val[1]));
            ICF_PRINT(((icf_uint8_t *)">> Registration Duration = %d", 
                        p_trace_data->int_val[2]));            
            break;
 
        case ICF_CFG_TRACE_SRVC_PRMS_SET:
            ICF_PRINT(((icf_uint8_t *)"\n>> Call_hold_option = %d", 
                        p_trace_data->int_val[0]));
            ICF_PRINT(((icf_uint8_t *)">> max_num_of_call_forwarding_hops =  %d", 
                        p_trace_data->int_val[1]));
            ICF_PRINT(((icf_uint8_t *)">> max_num_of_calls_in_call_waiting = %d", 
                        p_trace_data->int_val[2]));
            break;
        
        case ICF_CFG_TRACE_SRVC_TIMERS_SET:
            ICF_PRINT(((icf_uint8_t *)"\n>> call_hold_resume_timer = %d", 
                        p_trace_data->int_val[0]));
            ICF_PRINT(((icf_uint8_t *)">> reinvite_race_timer = %d", 
                        p_trace_data->int_val[1]));
            ICF_PRINT(((icf_uint8_t *)">> twc_timer =  %d", 
                        p_trace_data->int_val[2]));
            ICF_PRINT(((icf_uint8_t *)">> cw_alert_timer = %d", 
                        p_trace_data->int_val[3]));
            break;
        
        case ICF_CFG_TRACE_CALL_FWD_TIMERS_SET:
            ICF_PRINT(((icf_uint8_t *)"\n>> ctu_timer = %d", 
                        p_trace_data->int_val[0]));
            ICF_PRINT(((icf_uint8_t *)">> cta_timer = %d", 
                        p_trace_data->int_val[1]));
            ICF_PRINT(((icf_uint8_t *)">> cnd_alert_timer =  %d", 
                        p_trace_data->int_val[2]));
            ICF_PRINT(((icf_uint8_t *)">> cw_n_cnd_alert_timer = %d", 
                        p_trace_data->int_val[3]));
            break;
             
        case ICF_CFG_TRACE_SRVC_FLAGS_SET:
            ICF_PRINT(((icf_uint8_t *)"\n>> Line id = %d", 
                        p_trace_data->int_val[0]));
            ICF_PRINT(((icf_uint8_t *)"\n>> Service Subs flag's status = %d", 
                        p_trace_data->int_val[1]));
            ICF_PRINT(((icf_uint8_t *)"\n>> Service Act flag's status = %d", 
                        p_trace_data->int_val[2]));
            break;
        
        case ICF_CFG_TRACE_CFU_ADDR_SET:
            ICF_PRINT(((icf_uint8_t *)"\n>> Line ID = %d", 
                        p_trace_data->int_val[0]));
            ICF_PRINT(((icf_uint8_t *)">> Address Type = %d", 
                        p_trace_data->int_val[1]));
            ICF_PRINT(((icf_uint8_t *)">> CFU Address = %s", 
                        p_trace_data->string_val[0].str));
            break;
        
        case ICF_CFG_TRACE_CFB_ADDR_SET:
            ICF_PRINT(((icf_uint8_t *)"\n>> Line ID = %d", 
                        p_trace_data->int_val[0]));
            ICF_PRINT(((icf_uint8_t *)">> Address Type = %d", 
                        p_trace_data->int_val[1]));
            ICF_PRINT(((icf_uint8_t *)">> CFB Address = %s", 
                        p_trace_data->string_val[0].str));
            break;
        
        case ICF_CFG_TRACE_CFD_ADDR_SET:
            ICF_PRINT(((icf_uint8_t *)"\n>> Line ID = %d", 
                        p_trace_data->int_val[0]));
            ICF_PRINT(((icf_uint8_t *)">> Address Type = %d", 
                        p_trace_data->int_val[1]));
            ICF_PRINT(((icf_uint8_t *)">> CFD Address = %s", 
                        p_trace_data->string_val[0].str));
            break;


            
        default:
#ifdef ICF_DETAILED_DEBUG_TRACE

            ICF_PRINT(((icf_uint8_t *)"\n"));
#endif
            break;
    }
}
#endif
#endif/* End of ICF_TRACE_ENABLE */
