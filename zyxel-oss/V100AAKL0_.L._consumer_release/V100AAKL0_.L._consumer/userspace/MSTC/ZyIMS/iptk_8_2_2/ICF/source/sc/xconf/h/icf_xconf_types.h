/*****************************************************************************
*
* FILE NAME    : icf_xconf_types.h
*
* DESCRIPTION  : Contains the datatypes used in XCONF service
*
* Revision History :
*    DATE            NAME       REFERENCE       REASON
* --------    ------------      ---------      ---------------
* 4-Jul-2007  Abhishek Dhammawat CDD Rel7.0    Created Origional
*
* Copyright 2007,Aricent.
*
*******************************************************************************/

#ifndef __ICF_XCONF_TYPES_H__
#define __ICF_XCONF_TYPES_H__
#include "icf_map_extif_feature_flags.h"

#ifdef ICF_SERVICE_ENABLE
#include "icf_xconf_defs.h"
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
* Name : icf_xconf_pdb_st
*
* Description  : This structure defines the process data block of XCONF service 
*
*-----------------------------------------------------------------------------*/

typedef struct 
{
      /*Pointer to the XCONF service context*/
      icf_xconf_ctx_st         *p_xconf_ctx;

      /*Pointer to the Call Context*/
      icf_call_ctx_st        *p_call_ctx;

      /*Pointer to the service context*/
      struct icf_service_ctx_st  *p_service_ctx;

      /*Pointer to the internal message passed to XCONF entry point function*/
      icf_internal_msg_st    *p_internal_msg;

      icf_error_t            *p_ecode;

      icf_glb_pdb_st         *p_glb_pdb;

      icf_boolean_t     result;
      icf_error_t       error_cause;
  
} icf_xconf_pdb_st;

    

/******************************************************************************
 * FSM function pointers definition
 *****************************************************************************/
typedef icf_return_t  (*icf_xconf_fsm_func_ptr_t) (
      INOUT icf_xconf_pdb_st      *p_pdb );


/*------------------------------------------------------------------------------
*
* Name : icf_xconf_data_st
*
* Description  : This structure defines the global data block of XCONF service
*
*-----------------------------------------------------------------------------*/

typedef struct
{
    icf_xconf_fsm_func_ptr_t 
        xconf_fsm_table[ICF_XCONF_MAX_STATE][ICF_XCONF_MAX_EVENT];
} icf_xconf_data_st; 


#ifdef __cplusplus
}
#endif /*  __cpluscplus */
#endif
#endif /*  __ICF_XCONF_TYPES_H__ */


