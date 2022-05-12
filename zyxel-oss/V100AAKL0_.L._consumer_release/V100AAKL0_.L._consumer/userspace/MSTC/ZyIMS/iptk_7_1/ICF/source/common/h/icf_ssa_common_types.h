
/*------------------------------------------------------------------------------
 *
 * File name        : icf_ssa_common_types.h
 *
 * Purpose          : This file contains all the common structures
 *                    or typedefs that are being used across the whole ICF.
 *
 * Revision History :
 *
 *   Date        Name             Ref#        Comments
 * --------      ------------     ----        ---------------
 * 03-Mar-2005   Aashish Suchdev              Initial
 * 08-Jun-2005   Jalaj Negi       IMS CLIENT  Added Structure for PRACK
 *                                            response from N/W
 *                                            
 * Copyright (c) 2006, Aricent.
 *----------------------------------------------------------------------------*/

#ifndef __ICF_SSA_COMMON_TYPES_H__
#define __ICF_SSA_COMMON_TYPES_H__

/*Stack and UATK common inclusion*/
/*#include "stack_headers.h"
#include "uatk_headers.h"*/

/*TK common inclusion*/
#include "icf_common_types.h"
#include "icf_common_interface_types.h"

#ifdef __cplusplus

/* Yes, C++ compiler is present.
 * Use standard C.
 */

extern "C"
{
#endif /* __cplusplus */
/* IMP : This UATK function pointer is equivalent to Sdf_ty_TimertimeOutFunc, If
         on future paramenter to Sdf_ty_TimertimeOutFunc has been modified then
         please update to this call function 
*/

     typedef icf_return_t (*icf_ssa_uatk_timeout_func)
                                (icf_uint32_t dTimerType,
	                             icf_void_t* pContextInfo, 
                                 icf_void_t *pErr);
/* --------------------------------------------------------------------
 * Type Name   :  icf_ssa_timer_data_st
 * Description :  This structure is used to store the callback related info
 * for the UATK. This will be passed to genlib on timer start.  On timer
 * expiry, genlib will invoke a function ( a timer expiry warapper in SSA)
 * which will then invoke UATK's callback function with the timer type and
 * pcontext info
 * ------------------------------------------------------------------------*/


    typedef struct{
        /*Following is the pointer to the SSA context, passed as AppData in
         * the StartTimer callback.  This is usedin case of a timer expiry
         * indication from MRM. SSA received the address of TK timer data 
         * from which it gets the SSA timer data. Then p_ssa_ctxt is used
         * to hold the address of the internal message passed from MRM.
         * When the Callback for IndicateTxnTimeout is hit, the pContext
         * there is passed the AppData( SSA CTXT). There the internal msg
         * is retreived from it and used to populate the outgoing message 
         * to CM
         */
        icf_void_t           *p_ssa_ctxt;

        /*ID of the timer returned by porting routine for start timer
         * This is stored so that in case of a StopTimer invocation from
         * UATK, we can invoke the porting routine's stop timer interface
         * with correct identifier*/
        icf_timer_id_t           timer_id;
        /*Pointer of the TK timer data block allocated at the time of 
         * StartTimer. This is required at time of StopTimer, so that 
         * we can release this instance.The interface of StopTimer passes
         * TimerHandle which is the SSA TIMER DATA block
         */
        icf_timer_data_st         *p_timer_data;

        icf_uint32_t              timer_type;
        icf_ssa_uatk_timeout_func p_uatk_timer_cbk;
        icf_void_t                *p_uatk_ctxt_info;
        icf_void_t                *p_ares_ctxt; 
        /*To keep track of Ares Qurey Id*/
        icf_uint32_t              ares_qid;
    }icf_ssa_timer_data_st;


/* These function are for accessing the decodeBase64 and encodeBase64 of SIPUATK */
void icf_cmn_ssa_decodeBase64 (icf_int8_t *p_input, icf_int8_t *p_output);

void icf_cmn_ssa_encodeBase64 (icf_int8_t* p_input,icf_int8_t* p_result);

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* __ICF_SSA_COMMON_TYPES_H__ */

