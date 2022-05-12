/******************************************************************************
*
* FILE NAME    : icf_ctu_types.h
*
* DESCRIPTION  : Contains the datatypes used in CTU service
*
*
* Revision History :
*    DATE            NAME       REFERENCE       REASON
* --------    ------------      ---------      ---------------
* 25-Jan-2005  Amit Sharma       None           Created Origional
* 27-Feb-2006  Amit Sharma                     ICF Merging with icf rel2.0
*
* Copyright 2006, Aricent .
*
*******************************************************************************/

#ifndef __ICF_CTU_TYPES_H__
#define __ICF_CTU_TYPES_H__
#include "icf_map_extif_feature_flags.h"

#ifdef ICF_SERVICE_ENABLE
#include "icf_ctu_defs.h"
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
* Name : icf_ctu_pdb_st
*
* Description  : This structure defines the process data block of CTU service 
*
*-----------------------------------------------------------------------------*/

    typedef struct 
    {
        /*Pointer to the CTU service context*/
        icf_ctu_ctx_st             *p_ctu_ctx;

        /*Pointer to the Call Context*/
        icf_call_ctx_st            *p_call_ctx;

        /*Pointer ti the service context*/
        struct icf_service_ctx_st  *p_service_ctx;

        /*Pointer to the internal message passed to CTU entry point function*/
        icf_internal_msg_st        *p_internal_msg;
#ifdef ICF_TRACE_ENABLE
        icf_boolean_t     result;
        icf_error_t       error_cause;
#endif
    } icf_ctu_pdb_st;


/******************************************************************************
 * FSM function pointers definition
 *****************************************************************************/
typedef icf_return_t  (*icf_ctu_fsm_func_ptr_t) (
      INOUT icf_ctu_pdb_st      *p_pdb );


/*------------------------------------------------------------------------------
*
* Name : icf_ctu_data_st
*
* Description  : This structure defines the global data block of CTU service
*
*-----------------------------------------------------------------------------*/

typedef struct
{
    icf_ctu_fsm_func_ptr_t 
        ctu_fsm_table[ICF_CTU_MAX_STATE][ICF_CTU_MAX_EVENT];
} icf_ctu_data_st; 


#ifdef __cplusplus
}
#endif /*  __cpluscplus */
#endif
#endif /*  __ICF_CTU_TYPES_H__ */


