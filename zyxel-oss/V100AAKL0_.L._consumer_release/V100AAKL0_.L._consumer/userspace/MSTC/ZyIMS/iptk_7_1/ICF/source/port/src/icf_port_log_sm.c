/**********************************************************************
 *
 *     FILENAME           :  icf_port_log_sm.c
 *
 *     DESCRIPTION        :  This file contains error/trace printing 
 *                           routines for CC
 *
 *     Revision History   :
 *         DATE        NAME                REFERENCE            REASON
 *         ----------------------------------------------------------
 *         07/01/05    Aashish Suchdev                        Initial
 *         16/03/05    Jyoti Narula                      Updated for 
 *                                                       printing Traces/errors 
 *
 *     Copyright 2006, Aricent
 *
 **********************************************************************/
#ifdef ICF_PORT_SOLARIS
#include <stdio.h>
#endif

#include "icf_common_types.h"
#include "icf_port_prototypes.h"
#include "icf_port_defs.h"

#ifdef ICF_ERROR_ENABLE
#ifndef ICF_SM_LOG_ENABLE
/***********************************************************************
 *
 *  FUNCTION NAME  :  icf_port_log_sm_error
 *
 *  DESCRIPTION    :  This function is used to 
 *                    print the error data from SM  
 *
 *  RETURNS        :  Nothing.
 *
 **********************************************************************/

icf_void_t icf_port_log_sm_error(
        IN icf_error_id_t    error_id,
        IN icf_error_data_st *p_error_data)
{
    if (ICF_NULL != p_error_data)
    {
        /* p_error_data:NON NULL processing begins.*/

        switch (error_id)
        {
            case ICF_ERROR_INVALID_API_ID:
                ICF_PRINT(((icf_uint8_t *)"\n>>msg_id = %d",p_error_data->int_val[0]));
                ICF_PRINT(((icf_uint8_t *)">>api_id = %d", p_error_data->int_val[1]));
                break;

            case ICF_ERROR_INVALID_PARAM:
                ICF_PRINT(((icf_uint8_t *)"\n>>Invalid Parameters for api_id = %d", 
                            p_error_data->int_val[0]));
                break;

            case ICF_ERROR_CALL_LOGGING_FAILURE:
                ICF_PRINT(((icf_uint8_t *)"\n>>Calls Logging Disabled. "));
                break;


            default:
                ICF_PRINT(((icf_uint8_t *)"\n"));
                break;

        }/* Switch: error_id ends here.*/
    }/* p_error_data NON NULL: Processing ends.*/

    else
    {
        /* NULL p_error_data. */
        ICF_PRINT(((icf_uint8_t *)"\n>> No value in error_data. "));

    }

}/* Function icf_port_sm_error() ends here. */
#endif
#endif /*ICF_ERROR_ENABLE*/


#ifdef ICF_TRACE_ENABLE
#ifndef ICF_SM_LOG_ENABLE
/*************************************************************************
 *
 *  FUNCTION NAME  :   icf_port_log_sm_trace
 *
 *  DESCRIPTION    :  This function is used to print the trace data from SM.  
 *
 *  RETURNS        :  Nothing.   
 *
 *************************************************************************/

icf_void_t icf_port_log_sm_trace(
        IN icf_trace_id_t    trace_id,
        IN icf_trace_data_st *p_trace_data)
{
    if (ICF_NULL != p_trace_data)
    {
        /* NON NULL p_trace_data: Processing begins.*/

        switch (trace_id)
        {
            case ICF_SM_TRACE_TRACE_LVL_REQ_RECD:

                ICF_PRINT(((icf_uint8_t *)"\n>>Set_Trace: status = %d",
                            p_trace_data->int_val[0]));
                ICF_PRINT(((icf_uint8_t *)">>trace_level = %d",p_trace_data->int_val[1]));
                ICF_PRINT(((icf_uint8_t *)">>trace_type = %d",p_trace_data->int_val[2]));
                break;

            case ICF_SM_TRACE_ERROR_LVL_REQ_RECD:

                ICF_PRINT(((icf_uint8_t *)"\n>>Set_Error: status = %d",
                            p_trace_data->int_val[0]));
                /* SPR fix 17651 Only print status */
                /*ICF_PRINT(((icf_uint8_t *)">>error_level = %d",p_trace_data->int_val[1]));
                ICF_PRINT((">>error_type = %d",p_trace_data->int_val[2])); */
                break;

            case ICF_SM_TRACE_SET_STATS_REQ_RECD:
                ICF_PRINT(((icf_uint8_t *)"\n>>Set_Stats: stats_id = %d ",
                            p_trace_data->int_val[0]));
                ICF_PRINT(((icf_uint8_t *)">>status = %d",p_trace_data->int_val[1]));
                ICF_PRINT(((icf_uint8_t *)">>reset = %d",p_trace_data->int_val[2]));
                break;

            case ICF_SM_TRACE_GET_STATS_REQ_RECD:
                ICF_PRINT(((icf_uint8_t *)"\n>>Get_Stats_Req: stats_id = %d",
                            p_trace_data->int_val[0]));
                ICF_PRINT(((icf_uint8_t *)">>Number of stats reported = %d",
                            p_trace_data->int_val[1]));
                break;


            case ICF_SM_TRACE_GET_STATS_RESP_SENT:
                ICF_PRINT(((icf_uint8_t *)"\n>>Get_Stats_Resp: stats_id = %d",
                            p_trace_data->int_val[0]));
                ICF_PRINT(((icf_uint8_t *)">>reset = %d",p_trace_data->int_val[1]));
                break;

            case ICF_SM_TRACE_CHK_MODULE_REQ_RECD:
                ICF_PRINT(((icf_uint8_t *)"\n>>Check_module_status_req_recd"));
                break;


            case ICF_SM_TRACE_CHK_MODULE_RESP_SENT:
                ICF_PRINT(((icf_uint8_t *)"\n>>check_module_status_resp_sent"));
                break;

            case ICF_SM_TRACE_CALL_LOG_REQ_RECD:
                ICF_PRINT(((icf_uint8_t *)"\n>>call_log_req_recd"));
                break;


            case ICF_SM_TRACE_CALL_LOG_RESP_SENT:
                ICF_PRINT(((icf_uint8_t *)"\n>>Call Log response Sent"));
                break;

            case ICF_SM_TRACE_VALIDATE_PARAM:
                ICF_PRINT(((icf_uint8_t *)"\n>>SM_Validate_param: param_validity: %d",
                            p_trace_data->int_val[0]));
                break;

            case ICF_SM_TRACE_NUM_CALLS_RECORD:
                ICF_PRINT(((icf_uint8_t *)"\n>>SM_num_calls_record: value of \
                                   num_calls_to_log stored in SM global data \
                                   : %d", p_trace_data->int_val[0]));
                break;
                


            case ICF_SM_TRACE_DIALED_CALL_LOG_REQ:
                if (ICF_NO_INT_PRESENT != p_trace_data->
                                    num_of_integer_values)
                {
                ICF_PRINT(((icf_uint8_t *)"\n>>Set_dialed_call_req: \
                            Top element of dialed call history: call_id = %d",
                            p_trace_data->int_val[0]));
                ICF_PRINT(((icf_uint8_t *)">>date = %d",p_trace_data->int_val[1]));
                ICF_PRINT(((icf_uint8_t *)">>hour = %d",p_trace_data->int_val[2]));
                ICF_PRINT(((icf_uint8_t *)">>minutes = %d",p_trace_data->int_val[3]));
                ICF_PRINT(((icf_uint8_t *)">>remote_party_addr = %s",
                            p_trace_data->string_val[0].str));
                }
                else
                {
                    ICF_PRINT(((icf_uint8_t *)"\n>>Dialed Call List Empty."));
                }
                break;


            case ICF_SM_TRACE_DIALED_CALL_LOG_RESP:
                ICF_PRINT(((icf_uint8_t *)"\n>>Set_dialed_call_resp: \
                            Top element of dialed call history: call_id = %d",
                            p_trace_data->int_val[0]));
                ICF_PRINT(((icf_uint8_t *)">>date = %d",p_trace_data->int_val[1]));
                ICF_PRINT(((icf_uint8_t *)">>hour = %d",p_trace_data->int_val[2]));
                ICF_PRINT(((icf_uint8_t *)">>minutes = %d",p_trace_data->int_val[3]));
                ICF_PRINT(((icf_uint8_t *)">>remote_party_addr = %s",
                            p_trace_data->string_val[0].str));

                break;


            case ICF_SM_TRACE_MISSED_CALL_LOG_REQ:
                if (ICF_NO_INT_PRESENT != p_trace_data->
                                   num_of_integer_values)
                {
                ICF_PRINT(((icf_uint8_t *)"\n>>Set_missed_call_req: \
                            Top element of missed call history : call_id = %d",
                            p_trace_data->int_val[0]));
                ICF_PRINT(((icf_uint8_t *)">>date = %d",p_trace_data->int_val[1]));
                ICF_PRINT(((icf_uint8_t *)">>hour = %d",p_trace_data->int_val[2]));
                ICF_PRINT(((icf_uint8_t *)">>minutes = %d",p_trace_data->int_val[3]));
                ICF_PRINT(((icf_uint8_t *)">>remote_party_addr = %s",
                            p_trace_data->string_val[0].str));
                 }
                else
                {
                    ICF_PRINT(((icf_uint8_t *)"\n>>Missed Call List Empty."));
                }

                break;


            case ICF_SM_TRACE_MISSED_CALL_LOG_RESP:
                ICF_PRINT(((icf_uint8_t *)"\n>>Set_missed_call_resp: \
                            Top element of missed call history: call_id = %d",
                            p_trace_data->int_val[0]));
                ICF_PRINT(((icf_uint8_t *)">>date = %d",p_trace_data->int_val[1]));
                ICF_PRINT(((icf_uint8_t *)">>hour = %d",p_trace_data->int_val[2]));
                ICF_PRINT(((icf_uint8_t *)">>minutes = %d",p_trace_data->int_val[3]));
                ICF_PRINT(((icf_uint8_t *)">>remote_party_addr = %s",
                            p_trace_data->string_val[0].str));
                break;


            case ICF_SM_TRACE_RECD_CALL_LOG_REQ:
                 if (ICF_NO_INT_PRESENT != p_trace_data->
                                   num_of_integer_values)
                {
                ICF_PRINT(((icf_uint8_t *)"\n>>Set_recd_call_req: \
                            Top element of recd_call_history: call_id = %d",
                            p_trace_data->int_val[0]));
                ICF_PRINT(((icf_uint8_t *)">>date = %d",p_trace_data->int_val[1]));
                ICF_PRINT(((icf_uint8_t *)">>hour = %d",p_trace_data->int_val[2]));
                ICF_PRINT(((icf_uint8_t *)">>minutes = %d",p_trace_data->int_val[3]));
                ICF_PRINT(((icf_uint8_t *)">>remote_party_addr = %s",
                            p_trace_data->string_val[0].str));
                }
                else
                {
                    ICF_PRINT(((icf_uint8_t *)"\n>>Received Call List Empty."));
                }

                break;


            case ICF_SM_TRACE_RECD_CALL_LOG_RESP:
                ICF_PRINT(((icf_uint8_t *)"\n>>Set_recd_call_resp: \
                            Top element of recd_call_history: call_id = %d",
                            p_trace_data->int_val[0]));
                ICF_PRINT(((icf_uint8_t *)">>date = %d",p_trace_data->int_val[1]));
                ICF_PRINT(((icf_uint8_t *)">>hour = %d",p_trace_data->int_val[2]));
                ICF_PRINT(((icf_uint8_t *)">>minutes = %d",p_trace_data->int_val[3]));
                ICF_PRINT(((icf_uint8_t *)">>remote_party_addr = %s",
                            p_trace_data->string_val[0].str));
                break;


            default:
                ICF_PRINT(((icf_uint8_t *)"\n"));
                break;
        }/* Switch(trace_id) ends here. */

    } /* NON NULL p_trace_data: Processing Ends.*/

    else
    {
        /* NULL p_error_data. */
        ICF_PRINT(((icf_uint8_t *)"\n>> No values in trace_data. "));
    }


}/* Function icf_sm_trace() ends here. */
#endif
#endif/*ICF_TRACE_ENABLE*/
