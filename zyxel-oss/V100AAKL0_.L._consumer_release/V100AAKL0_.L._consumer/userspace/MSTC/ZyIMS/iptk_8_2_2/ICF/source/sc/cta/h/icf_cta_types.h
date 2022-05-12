/******************************************************************************
*
* FILE NAME    : icf_cta_types.h
*
* DESCRIPTION  : Contains the datatypes used in CTA service
*
*
* Revision History :
*    DATE            NAME       REFERENCE       REASON
* --------    ------------      ---------      ---------------
* 18-Feb-2005  Amit Sharma       None           Created Origional
* 27-Feb-2006  Amit Sharma                     ICF Merging with icf rel2.0
*
* Copyright 2006, Aricent 
*
*******************************************************************************/

#ifndef __ICF_CTA_TYPES_H__
#define __ICF_CTA_TYPES_H__
#include "icf_map_extif_feature_flags.h"

#ifdef ICF_SERVICE_ENABLE
#include "icf_cta_defs.h"
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
* Name : icf_cta_pdb_st
*
* Description  : This structare defines the process data block of CTA service 
*
*-----------------------------------------------------------------------------*/

    typedef struct 
    {
        /*Pointer to the CTA service context*/
        icf_cta_ctx_st             *p_cta_ctx;

        /*Pointer to the Call Context*/
        icf_call_ctx_st            *p_call_ctx;

        /*Pointer ti the service context*/
        struct icf_service_ctx_st  *p_service_ctx;

        /*Pointer to the internal message passed to CTA entry point function*/
        icf_internal_msg_st        *p_internal_msg;
#ifdef ICF_TRACE_ENABLE
        icf_boolean_t     result;
        icf_error_t       error_cause;
        icf_address_st           *p_transferred_party_addr;

         icf_name_st              *p_transferred_party_name;
 
#endif
    } icf_cta_pdb_st;


/******************************************************************************
 * FSM function pointers definition
 *****************************************************************************/
typedef icf_return_t  (*icf_cta_fsm_func_ptr_t) (
      INOUT icf_cta_pdb_st      *p_pdb );


/*------------------------------------------------------------------------------
*
* Name : icf_cta_data_st
*
* Description  : This structare defines the global data block of CTA service
*
*-----------------------------------------------------------------------------*/

typedef struct
{
    icf_cta_fsm_func_ptr_t 
        cta_fsm_table[ICF_CTA_MAX_STATE][ICF_CTA_MAX_EVENT];
} icf_cta_data_st; 


#ifdef __cplusplus
}
#endif /*  __cpluscplus */
#endif
#endif /*  __ICF_CTA_TYPES_H__ */



