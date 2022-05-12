/*------------------------------------------------------------------------------
 *
 * File name        : icf_cfw_prototypes.h
 *
 * Purpose          : This file contains prototypes of functions used in CFW SLP
 *
 * Revision History :
 *
 * Date         Name                    Ref#        Comments
 * --------     ------------            ----        ---------------
 * 17-MAR-2005  Shefali Aggarwal        ICF LLD  Beginning of coding phase
 *
 *
 * Copyright (c) 2006, Aricent.
 *----------------------------------------------------------------------------*/


#ifndef __ICF_CFW_PROTOTYPES_H__
#define __ICF_CFW_PROTOTYPES_H__
#include "icf_map_extif_feature_flags.h"

#ifdef ICF_SERVICE_ENABLE
#include "icf_common_types.h"
#include "icf_cfw_types.h"


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
    
    
/******************************************************************************
 *
 * FUNCTION:        icf_cfw_forward_call
 *
 * DESCRIPTION:     This function is called by the entry point function to 
 *                  process the message received to forward the call after the
 *                  validation of msg_id and status.
 *                         
 ******************************************************************************/
icf_return_t icf_cfw_forward_call(
        INOUT icf_cfw_pdb_st     *p_cfw_pdb);

/******************************************************************************
 *
 * FUNCTION:        icf_cfw_validate_req
 *
 * DESCRIPTION:     This function is called to validate the request of CFW.
 *                         
 ******************************************************************************/
icf_return_t icf_cfw_validate_req(
       INOUT icf_cfw_pdb_st      *p_cfw_pdb);

/******************************************************************************
 *
 * FUNCTION:        icf_cfw_trigger_ssa
 *
 * DESCRIPTION:     This function is called to trigger SSA to send 302 on the
 *                  network.
 *                         
 ******************************************************************************/
icf_return_t icf_cfw_trigger_ssa(
        INOUT icf_cfw_pdb_st     *p_cfw_pdb);

/******************************************************************************
 *
 * FUNCTION:        icf_cfw_compare_addr
 *
 * DESCRIPTION:     This function compares the addresses passed to it and
 *                  returns success if they match else return failure.
 *                         
 ******************************************************************************/
icf_return_t icf_cfw_compare_addr(
        INOUT icf_cfw_pdb_st     *p_cfw_pdb,
        IN    icf_address_st     *p_addr_a,
        IN    icf_address_st     *p_addr_b);

#ifdef ICF_TRACE_ENABLE
/******************************************************************************
 *
 * FUNCTION:        icf_cfw_trace
 *
 * DESCRIPTION:     This function is  use to fill values to raise trace.
 *                         
 ******************************************************************************/
icf_void_t icf_cfw_trace(
        INOUT icf_cfw_pdb_st     *p_cfw_pdb,
        IN    icf_trace_id_t     trace_id);


/******************************************************************************
 *
 * FUNCTION:        icf_cfw_app_trace
 *
 * DESCRIPTION:     This function is  use to fill values to raise trace.
 *
 ******************************************************************************/
icf_return_t icf_cfw_app_trace(
        INOUT icf_cfw_pdb_st     *p_cfw_pdb,
        IN    icf_trace_id_t     trace_id);

#endif

#ifdef ICF_ERROR_ENABLE
/******************************************************************************
 *
 * FUNCTION:        icf_cfw_ecode
 *
 * DESCRIPTION:     This function is  use to fill values for ecode.
 *
 ******************************************************************************/
icf_void_t icf_cfw_ecode(
        INOUT icf_cfw_pdb_st     *p_cfw_pdb,
        IN    icf_error_id_t     ecode_id);
#endif

/******************************************************************************
 *
 * FUNCTION:        icf_cfw_init_pdb
 *
 * DESCRIPTION:     This function initializes the pdb of CFW.
 *
 ******************************************************************************/
icf_return_t icf_cfw_init_pdb(
        INOUT icf_cfw_pdb_st          *p_cfw_pdb,
        INOUT icf_internal_msg_st     *p_internal_msg);

/*******************************************************************************
 *
 * FUNCTION:        icf_cfw_send_call_forward_ind
 *
 * DESCRIPTION:     This function send call forward indication to application
 *
 ******************************************************************************/
icf_return_t   icf_cfw_send_call_forward_ind(
            INOUT   icf_cfw_pdb_st      *p_cfw_pdb);

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif
#endif /* End of __ICF_CFW_PROTOTYPES_H__ */


