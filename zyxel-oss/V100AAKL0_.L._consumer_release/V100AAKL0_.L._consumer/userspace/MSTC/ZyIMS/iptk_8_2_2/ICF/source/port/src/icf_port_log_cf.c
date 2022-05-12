/*************************************************************************
 *
 *     FILENAME           :  icf_port_log_cf.c
 *
 *     DESCRIPTION        :  This file contains error/trace printing 
 *                           routines for CC
 *
 *     Revision History   :
 *         DATE        NAME                REFERENCE            REASON
 *         ----------------------------------------------------------
 *         07/01/05    Aashish Suchdev                        Initial
 *         07/04/08    Sumant Gupta      ICF 8.0      Rel7.1 Windows porting Merging
 *        04-Mar-2009 Saurabh Gupta         SPR 19349      Klocworks fix
 *     Copyright 2006, Aricent.
 *
 *************************************************************************/
#ifdef ICF_PORT_SOLARIS
#include <stdio.h>
#endif

#include "icf_common_types.h"
#include "icf_port_prototypes.h"
#include "icf_port_defs.h"

#ifdef ICF_ERROR_ENABLE
#ifndef ICF_SM_LOG_ENABLE
/*************************************************************************
*
*  FUNCTION NAME  :   icf_port_log_cf_error
*
*  DESCRIPTION    :  This function is used to print the error data from CC  
*
*  RETURNS        :     
*
 *************************************************************************/

icf_void_t icf_port_log_cf_error(
        IN icf_error_id_t    error_id,
        IN icf_error_data_st *p_error_data)
{
    icf_uint8_t  count = 0, max_count = 0;    

    switch (error_id){
        default:
            {
                max_count = p_error_data->num_of_integer_values;
                ICF_PRINT(((icf_uint8_t*)"\tDATA:integer(s):"));
                for ( count = 0; (count < max_count) && (count < ICF_MAX_INT_ERROR_DATA); count ++)
                {
                    ICF_PRINT(((icf_uint8_t*)" %lu",p_error_data->int_val[count]));
                }

                max_count = p_error_data->number_of_string_values;
                ICF_PRINT(((icf_uint8_t*)"\nDATA:string(s):"));
                for ( count = 0; (count < max_count) && (count < ICF_MAX_STR_ERROR_DATA); count ++)
                {
                    ICF_PRINT(((icf_uint8_t*)"\t %s",p_error_data->string_val[count].str));
                }
                ICF_PRINT(((icf_uint8_t*)"\n"));
            }
            break;
    }
}
#endif
#endif /*ICF_ERROR_ENABLE*/

#ifdef ICF_TRACE_ENABLE
#ifndef ICF_SM_LOG_ENABLE
/*************************************************************************
*
*  FUNCTION NAME  :   icf_port_log_cf_trace
*
*  DESCRIPTION    :  This function is used to print the trace data from CC  
*
*  RETURNS        :     
*
 *************************************************************************/

icf_void_t icf_port_log_cf_trace(
        IN icf_trace_id_t    trace_id,
        IN icf_trace_data_st *p_trace_data)
{
    icf_uint8_t  count = 0, max_count = 0;    

    switch (trace_id)
    {
        case ICF_CFW_TRACE_FAIL_TO_GET_LINE_DATA:
            ICF_PRINT(((icf_uint8_t*)" Line id: %d\n",p_trace_data->int_val[0]));
            break;
            
        case ICF_CFW_TRACE_SELF_CFW_ADDR_MATCH:
            ICF_PRINT(((icf_uint8_t*)" User on line id: %d\n",p_trace_data->int_val[0]));
            break;

        case ICF_CFW_TRACE_DATA_SEND:
            if(ICF_ONE_INT_PRESENT == p_trace_data->num_of_integer_values)
            {
                ICF_PRINT(((icf_uint8_t*)"Bitmask: %d\n",p_trace_data->int_val[0]));
            }
            else
            {
               ICF_PRINT(((icf_uint8_t*)" No of diversion data: %d\n",
                       p_trace_data->int_val[0]));
               ICF_PRINT(((icf_uint8_t*)" No of hops: %d\n",p_trace_data->int_val[1]));
               ICF_PRINT(((icf_uint8_t*)" Bitmask: %d\n",p_trace_data->int_val[2]));
               ICF_PRINT(((icf_uint8_t*)" Address Type: %d\n",p_trace_data->int_val[3]));
               ICF_PRINT(((icf_uint8_t*)" Address : %s\n",p_trace_data->string_val[0]));
            }
            break;
            
        case ICF_CFW_TRACE_REL_TYPE_N_CAUSE:
            ICF_PRINT(((icf_uint8_t*)" Release type: %d\n",p_trace_data->int_val[0]));
            ICF_PRINT(((icf_uint8_t*)" Release Cause: %d\n",p_trace_data->int_val[1]));
            break;

        default:
                {
                    max_count = p_trace_data->num_of_integer_values;
                    ICF_PRINT(((icf_uint8_t*)"\tDATA:integer(s):"));
                    for ( count = 0; (count < max_count) && (count < ICF_MAX_INT_TRACE_DATA) ; count ++)
                    {
                        ICF_PRINT(((icf_uint8_t*)" %lu",p_trace_data->int_val[count]));
                    }

                    max_count = p_trace_data->number_of_string_values;
                    ICF_PRINT(((icf_uint8_t*)"\nDATA:string(s):"));
                    for ( count = 0; (count < max_count) && (count < ICF_MAX_STR_TRACE_DATA) ; count ++)
                    {
                        ICF_PRINT(((icf_uint8_t*)"\t %s",p_trace_data->string_val[count].str));
                    }
                    ICF_PRINT(((icf_uint8_t*)"\n"));
                }
            break;
    }
}
#endif
#endif/*ICF_TRACE_ENABLE*/
