/*******************************************************************************
*
* FILE NAME    : icf_twc_types.h
*
* DESCRIPTION  : Contains the datatypes used in TWC service
*
*
* Revision History :
*    DATE            NAME       REFERENCE       REASON
* --------    ------------      ---------      ---------------
*   6-Jan-2004  Amit Sharma     None           Created Original
*  27-Feb-2006  Amit Sharma                  Merged code for ICF
*
* Copyright 2006,Aricent.
*
*******************************************************************************/

#ifndef __ICF_TWC_TYPES_H__
#define __ICF_TWC_TYPES_H__
#include "icf_map_extif_feature_flags.h"

#ifdef ICF_SERVICE_ENABLE
#include "icf_twc_defs.h"
#include "icf_common_types.h"
#include "icf_macro.h"
#include "icf_sc_common_types.h"

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

#if 0
/*------------------------------------------------------------------------------
*
* Name : icf_twc_ctx_st
*
* Description  : This structure defines the service context of TWC service
*
*-----------------------------------------------------------------------------*/

    typedef struct icf_twc_ctx_st
    {
        icf_uint8_t      next_state;

        icf_uint8_t      state;

        icf_uint8_t      event;

        /*Call context of the already existing call on
         * which TWC is invoked*/
        icf_call_ctx_st  *p_call_ctx_b;

        /* Call context of the second call with party C*/
        icf_call_ctx_st  *p_call_ctx_c;

        icf_timer_data_st  *p_twc_timer_data;

        icf_timer_id_t     twc_timer_id;
    } icf_twc_ctx_st;
#endif

/*------------------------------------------------------------------------------
*
* Name : icf_twc_pdb_st
*
* Description  : This structure defines the process data block of TWC service
*
*-----------------------------------------------------------------------------*/

    typedef struct
    {
        /*Pointer to the TWC service context*/
        icf_twc_ctx_st         *p_twc_ctx;

        /*Pointer ti the service context*/
        struct icf_service_ctx_st  *p_service_ctx;

        /*Pointer to the internal message passed to TWC entry point function*/
        icf_internal_msg_st    *p_internal_msg;

        /*Pointer to the error code structure*/
        /*icf_ecode_t            *p_ecode;*/
        icf_boolean_t            conf_internal_req;
#ifdef ICF_TRACE_ENABLE
        icf_boolean_t     result;
        icf_error_t       error_cause;
        icf_call_id_t     merge_call_id;
#endif

    } icf_twc_pdb_st;


/******************************************************************************
 *  FSM function pointers definition
 *****************************************************************************/
typedef icf_return_t  (*icf_twc_fsm_func_ptr_t) (
      INOUT icf_twc_pdb_st      *p_pdb );


/*------------------------------------------------------------------------------
*
* Name : icf_twc_data_st
*
* Description  : This structure defines the global data block of TWC service
*
*-----------------------------------------------------------------------------*/

typedef struct

{
    icf_twc_fsm_func_ptr_t
        twc_fsm_table[ICF_TWC_MAX_STATE][ICF_TWC_MAX_EVENT];
} icf_twc_data_st;


#ifdef __cplusplus
}
#endif /*  __cpluscplus */
#endif /* ICF_SERVICE_ENABLE */
#endif /*  __ICF_TWC_TYPES_H__ */
