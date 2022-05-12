/*****************************************************************************
*
* FILE NAME    : icf_rnw_types.h
*
* DESCRIPTION  : Contains the datatypes used in RNW service
*
* Revision History :
*    DATE            NAME       REFERENCE       REASON
* --------    ------------      ---------      ---------------
*   3-Mar-2005  Amit Sharma     None           Created Origional
* 27-Feb-2006  Amit Sharma                     ICF Merging with icf rel2.0
*
* Copyright 2006,Aricent.
*
*******************************************************************************/

#ifndef __ICF_RNW_TYPES_H__
#define __ICF_RNW_TYPES_H__
#include "icf_map_extif_feature_flags.h"

#ifdef ICF_SERVICE_ENABLE
#include "icf_rnw_defs.h"
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

/*------------------------------------------------------------------------------
*
* Name : icf_rnw_pdb_st
*
* Description  : This structure defines the process data block of RNW service 
*
*-----------------------------------------------------------------------------*/

    typedef struct 
    {
        /*Pointer to the RNW service context*/
        icf_rnw_ctx_st         *p_rnw_ctx;

        /*Pointer to the Call Context*/
        icf_call_ctx_st        *p_call_ctx;

        /*Pointer ti the service context*/
        struct icf_service_ctx_st  *p_service_ctx;

        /*Pointer to the internal message passed to RNW entry point function*/
        icf_internal_msg_st    *p_internal_msg;
        icf_uint32_t          resp_code;
#ifdef ICF_TRACE_ENABLE
	icf_boolean_t         replace_header_present;
        icf_call_id_t         new_call_id;
        icf_boolean_t     result;
        icf_error_t       error_cause;
#endif
    } icf_rnw_pdb_st;

    

/******************************************************************************
 * FSM function pointers definition
 *****************************************************************************/
typedef icf_return_t  (*icf_rnw_fsm_func_ptr_t) (
      INOUT icf_rnw_pdb_st      *p_pdb );


/*------------------------------------------------------------------------------
*
* Name : icf_rnw_data_st
*
* Description  : This structure defines the global data block of RNW service
*
*-----------------------------------------------------------------------------*/

typedef struct
{
    icf_rnw_fsm_func_ptr_t 
        rnw_fsm_table[ICF_RNW_MAX_STATE][ICF_RNW_MAX_EVENT];
} icf_rnw_data_st; 


#ifdef __cplusplus
}
#endif /*  __cpluscplus */
#endif
#endif /*  __ICF_RNW_TYPES_H__ */


