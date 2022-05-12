/*******************************************************************************
*
* FILE NAME    : icf_chr_types.h
*
* DESCRIPTION  : Contains the datatypes used in CHR service
*
*
* Revision History :
*    DATE            NAME       REFERENCE       REASON
* --------    ------------      ---------      ---------------
*   6-Dec-2004  Amit Sharma     None           Created Origional
*   5-Feb-2004  Amit Sharma     None           To add ongoing action 
*                                              ICF_CHR_OA_MMS_FOR_REMOTE_RESP
*   Rel 2.0
*   29-Sep-2005 Shefali Aggarwal               Added imm_internal_req_resp in
*                                              the pdb
*   7-Sep-2006  Amit Sharma     SPR:12840
*  24-Aug-2007  Amit Sharma                    IPTK merge from PQT6.5
*
* Copyright 2006,Aricent.
*
*******************************************************************************/

#ifndef __ICF_CHR_TYPES_H__
#define __ICF_CHR_TYPES_H__
#include "icf_map_extif_feature_flags.h"

#ifdef ICF_SERVICE_ENABLE
#include "icf_chr_defs.h"
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
* Name : icf_chr_pdb_st
*
* Description  : This structure defines the process data block of CHR service 
*
*-----------------------------------------------------------------------------*/

    typedef struct 
    {
        /*Pointer to the CHR service context*/
        icf_chr_ctx_st         *p_chr_ctx;

        /*Pointer to the Call Context*/
        icf_call_ctx_st        *p_call_ctx;

        /*Pointer ti the service context*/
        struct icf_service_ctx_st  *p_service_ctx;

        /*Pointer to the internal message passed to CHR entry point function*/
        icf_internal_msg_st    *p_internal_msg;

        /*Flag to indicate that success response for the internal call hold
         * and call resume is to be send back without calling the FSM of
         * CHR
         */
        icf_boolean_t        imm_internal_req_resp;

        icf_boolean_t        mm_err_recvd;
        icf_boolean_t        sdp_present;
        /*Pointer to the error code structure*/
        /*icf_ecode_t            *p_ecode;*/
        icf_boolean_t        mms_for_mode;
#ifdef ICF_TRACE_ENABLE
        icf_boolean_t     result;
        icf_error_t       error_cause;
#endif
    } icf_chr_pdb_st;


/******************************************************************************
 * FSM function pointers definition
 *****************************************************************************/
typedef icf_return_t  (*icf_chr_fsm_func_ptr_t) (
      INOUT icf_chr_pdb_st      *p_pdb );


/*------------------------------------------------------------------------------
*
* Name : icf_chr_data_st
*
* Description  : This structure defines the global data block of CHR service
*
*-----------------------------------------------------------------------------*/

typedef struct
{
    icf_chr_fsm_func_ptr_t 
        chr_fsm_table[ICF_CHR_MAX_STATE][ICF_CHR_MAX_EVENT];
} icf_chr_data_st; 

#ifdef __cplusplus
}
#endif /*  __cpluscplus */
#endif
#endif /*  __ICF_CHR_TYPES_H__ */

