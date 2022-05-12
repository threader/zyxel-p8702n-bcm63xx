/******************************************************************************
 *
 * FILE NAME    : icf_scm_trace.c
 *
 * DESCRIPTION  : Contains the SCM trace functions.
 *
 *
 * Revision History :
 *    DATE            NAME       REFERENCE       REASON
 * --------    ------------      ---------      ---------------
 * 28-Jan-2005    Navneet Gupta   None            Initial
 *
 * Copyright 2006,Aricent.
 ******************************************************************************/
#include "icf_map_extif_feature_flags.h"

#ifdef ICF_SERVICE_ENABLE
#include "icf_scm_types.h"
#include "icf_scm_prototypes.h"
#include "icf_dbm_prototypes.h"

#ifdef ICF_TRACE_ENABLE
/*******************************************************************************
 *
 * FUNCTION NAME : icf_scm_trace
 * 
 * DESCRIPTION   : This is the trace function for SCM
 *
 * RETURNS       : icf_void
 *
 ******************************************************************************/
icf_void_t   icf_scm_trace(
        INOUT icf_scm_pdb_st *p_pdb,
        IN    icf_trace_id_t trace_id)
 {
    icf_trace_data_st  trace_data = {{0,0},0,{0},0,{{0,"\0"}},{0,"\0"},0};

    switch (trace_id)
    {
        /* Switch to those trace ids only, for which an integer value */
        /* is to be shown in traces */
        case ICF_TRACE_API_RECVD:
            trace_data.num_of_integer_values = ICF_ONE_INT_PRESENT; 
            trace_data.int_val[0] = p_pdb->p_msg->msg_hdr.msg_id;
            trace_data.number_of_string_values = ICF_NO_STRING_PRESENT;
            break;
        case ICF_TRACE_SEND_FAILURE_TO_SL:
        case ICF_TRACE_GET_HANDLER_SL:
        case ICF_TRACE_SL_CTX_CREATE:
        case ICF_TRACE_CALL_RELEASE_FORWARDED:
        case ICF_TRACE_CALL_ID_EXTRACTED:
        case ICF_TRACE_MSG_ROUTING_PROGRESS:
            trace_data.num_of_integer_values = ICF_ONE_INT_PRESENT;
            trace_data.int_val[0] = p_pdb->trace_int;
            trace_data.number_of_string_values = ICF_NO_STRING_PRESENT; 
            break;
        default: 
            trace_data.num_of_integer_values = ICF_NO_INT_PRESENT;
            trace_data.number_of_string_values = ICF_NO_STRING_PRESENT;
            break; 
    }

    /* Not checking the return value */                                   
    ICF_TRACE(p_pdb->p_msg->msg_hdr.p_glb_pdb,trace_id,&trace_data)
}
#endif

#ifdef ICF_ERROR_ENABLE

/*******************************************************************************
 *
 * FUNCTION NAME : icf_scm_error
 * 
 * DESCRIPTION   : This is the error function for SCM
 *
 * RETURNS       : icf_void
 *
 ******************************************************************************/
icf_void_t   icf_scm_error(
        INOUT icf_scm_pdb_st *p_pdb,
        IN    icf_error_t ecode)
 {
    icf_error_data_st                error_data;

    switch (ecode)
    {
        case ICF_ERROR_SERVICE_NOT_SUBSCRIBED:
        case ICF_ERROR_SERVICE_NOT_ACTIVE:
        case ICF_ERROR_INVALID_SERVICE_INTERACTION:
            error_data.num_of_integer_values = ICF_ONE_INT_PRESENT;
            error_data.int_val[0] = p_pdb->trace_int;
            error_data.number_of_string_values = ICF_NO_STRING_PRESENT;
            break;
        default: 
            error_data.num_of_integer_values = ICF_NO_INT_PRESENT;
            error_data.number_of_string_values = ICF_NO_STRING_PRESENT;
            break; 

    }
    /* Currently Ecode is not set by ICF_SET_ECODE, 
     * so adding that functionality here */
    p_pdb->p_msg->msg_hdr.ecode = ecode;
   
    ICF_SET_ECODE(p_pdb->p_msg->msg_hdr.p_glb_pdb,
            &error_data,
            ecode,
            &(p_pdb->p_msg->msg_hdr.ecode))
}
#endif
#endif
