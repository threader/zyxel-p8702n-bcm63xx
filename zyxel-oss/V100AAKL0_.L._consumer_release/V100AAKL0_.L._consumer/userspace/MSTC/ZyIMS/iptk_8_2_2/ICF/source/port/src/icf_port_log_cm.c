/*------------------------------------------------------------------------------
 *
 * File name        : icf_port_log_cm.c
 *
 * Purpose          : This file contains error/trace printing routines of CM.
 *
 * Revision History :
 *
 * Date         Name            Ref#            Comments
 * --------     ------------    ----            ---------------
 * 07-Jan-2005  Aashish Suchdev                 Initial
 * 31-Jan-2005  Rohit Aggarwal                  Added function definitions
 * 24-Mar-2008  Shruti Thakral  SPR 18283       Klocworks warning removal
 * 7-Apr-2008   Sumant Gupta    ICF 8.0         Rel7.1 Windows porting Merging
 * Copyright (c) 2008, Aricent.
 *----------------------------------------------------------------------------*/

#ifdef ICF_PORT_SOLARIS
#include <stdio.h>
#endif

#include "icf_common_types.h"
#include "icf_port_prototypes.h"
#include "icf_port_defs.h"


#ifdef ICF_ERROR_ENABLE
#ifndef ICF_SM_LOG_ENABLE
/*******************************************************************************
 *
 * FUNCTION:        icf_port_log_cm_error
 *
 * DESCRIPTION:     This function is used to print the error data from CM.
 *
 * RETURNS:         Nothing
 *
 *
 ******************************************************************************/
icf_void_t icf_port_log_cm_error(
        IN  icf_error_id_t       error_id,
        IN  icf_error_data_st    *p_error_data)
{

    if(ICF_NULL == p_error_data)
    {
       ICF_PRINT(((icf_uint8_t*)"p_error_data is null"));

    }
    else
    {
        switch (error_id)
        {
            case ICF_MSG_PROC_FIELD_TYPE_MISMATCH:
                ICF_PRINT(((icf_uint8_t*)"\n>> field_type = %d", p_error_data->int_val[0]));
                break;

            case ICF_MSG_PROC_OPR_MISMATCH:
                ICF_PRINT(((icf_uint8_t*)"\n>> operation = %d", p_error_data->int_val[0]));
                break;

            case ICF_INVALID_API_ID_RECD:
                ICF_PRINT(((icf_uint8_t*)"\n>> msg_id = %d", p_error_data->int_val[0]));
                ICF_PRINT(((icf_uint8_t*)"\t>> api_id = %d", p_error_data->int_val[1]));
                break;

            default:
                ICF_PRINT(((icf_uint8_t*)"\n"));
                break;
        }
    }
}
#endif
#endif /* End of ICF_ERROR_ENABLE */


#ifdef ICF_TRACE_ENABLE
#ifndef ICF_SM_LOG_ENABLE
/*******************************************************************************
 *
 * FUNCTION:        icf_port_log_cm_trace
 *
 * DESCRIPTION:     This function is used to print the trace data from CM.
 *
 * RETURNS:         Nothing
 *
 *
 ******************************************************************************/
icf_void_t icf_port_log_cm_trace(
        IN  icf_trace_id_t       trace_id,
        IN  icf_trace_data_st    *p_trace_data)
{
    switch (trace_id)
    {
        case ICF_MSG_PROC_STATUS:
            ICF_PRINT(((icf_uint8_t*)"\n>> status = %d", p_trace_data->int_val[0]));
            break;

        case ICF_MSG_PROC_RECV_INT_MSG:
            ICF_PRINT(((icf_uint8_t*)"\n>> call_id = %d", p_trace_data->int_val[0]));
            ICF_PRINT(((icf_uint8_t*)"\t>> msg_id = %d", p_trace_data->int_val[1]));
            ICF_PRINT(((icf_uint8_t*)"\t>> status = %d", p_trace_data->int_val[2]));
            if (ICF_FOUR_INT_PRESENT == p_trace_data->num_of_integer_values)
            {
                ICF_PRINT(((icf_uint8_t*)"\t>> api_id = %d", p_trace_data->int_val[3]));
            }
            break;

        case ICF_MSG_PROC_ACTION_RESULT:
            ICF_PRINT(((icf_uint8_t*)"\n>> call_id = %d", p_trace_data->int_val[0]));
            ICF_PRINT(((icf_uint8_t*)"\t>> msg_id = %d", p_trace_data->int_val[1]));
            ICF_PRINT(((icf_uint8_t*)"\t>> status = %d", p_trace_data->int_val[2]));
            ICF_PRINT(((icf_uint8_t*)"\t>> module = %d", p_trace_data->int_val[3]));
            break;

        case ICF_MSG_PROC_FIELD_DATA:
            ICF_PRINT(((icf_uint8_t*)"\n>> field_type = %d", p_trace_data->int_val[0]));
            ICF_PRINT(((icf_uint8_t*)"\t>> field_value = %d", p_trace_data->int_val[1]));
            break;

        case ICF_MSG_PROC_FIELD_TYPE:
            ICF_PRINT(((icf_uint8_t*)"\n>> field_type = %d", p_trace_data->int_val[0]));
            break;

        case ICF_MSG_PROC_PRESENCE_FIELD:
            ICF_PRINT(((icf_uint8_t*)"\n>> presence_fld = %d", p_trace_data->int_val[0]));
            break;

        case ICF_MSG_PROC_VALUE_ACTION:
            ICF_PRINT(((icf_uint8_t*)"\n>> presence_fld = %d", p_trace_data->int_val[0]));
            ICF_PRINT(((icf_uint8_t*)"\t>> value = %d", p_trace_data->int_val[1]));
            ICF_PRINT(((icf_uint8_t*)"\t>> operation = %d", p_trace_data->int_val[2]));
            break;

        case ICF_MSG_PROC_ACTION:
            ICF_PRINT(((icf_uint8_t*)"\n>> presence_fld = %d", p_trace_data->int_val[0]));
            ICF_PRINT(((icf_uint8_t*)"\t>> event = %d", p_trace_data->int_val[1]));
            ICF_PRINT(((icf_uint8_t*)"\t>> dest = %d", p_trace_data->int_val[2]));
            break;

        default:
            ICF_PRINT(((icf_uint8_t*)"\n"));
            break;
    }
}
#endif
#endif/* End of ICF_TRACE_ENABLE */
