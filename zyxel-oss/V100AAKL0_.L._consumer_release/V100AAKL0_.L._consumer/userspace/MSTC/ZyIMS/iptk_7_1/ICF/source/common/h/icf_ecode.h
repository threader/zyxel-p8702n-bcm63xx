/*************************************************************************
 *
 * FILENAME         : icf_ecodes.h
 *
 * DESCRIPTION      : This file contains the internal error codes that 
 *                    shall reflect the error occured. 
 *                     
 * Revision History :
 * 
 * DATE         NAME                REFERENCE       REASON
 * -------------------------------------------------------------------
 * Nov 2004     R. Ezhirpavai      ICF LLD       Initial draft
 * 22-Dec-2004  Rohit Aggarwal                      Addition of ecode
 * 23-Aug-2005	Ashutos Rath  	  ICF		        Added errpr code
 *                                                  for SIGSA SSA
 *
 * Copyright (c) 2006, Aricent.
 *
 *************************************************************************/
#ifndef __ICF_ECODE_H
#define __ICF_ECODE_H

#include "icf_common_defs.h"

/* Determine if a C++ compiler is being used.
 * If so, ensure that standard
 * C is used to process the header information.
 * */

#ifdef __cplusplus

/* Yes, C++ compiler is present.
 * Use standard C.
 * */

extern "C"
{
#endif /* __cplusplus */


/******************** STANDARD INCLUDE FILES *****************************/

/******************** USER INCLUDE FILES *********************************/


/* -------------------------------------------------------------------------
 * Hash defined values of the internal error codes used within icf
 * ------------------------------------------------------------------------*/

/* This set of ecodes are shared with the error ids */

#define ICF_ECODE_NO_ERROR             ICF_ERROR_NONE
    
#define ICF_ECODE_MEM_ALLOC_FAILURE    ICF_ERROR_MEM_ALLOC_FAILURE
#define ICF_ECODE_MEM_FREE_FAILURE     ICF_ERROR_MEM_FREE_FAILURE
#define ICF_ECODE_TMR_START_FAILURE    ICF_ERROR_TMR_START_FAILURE
#define ICF_ECODE_TMR_STOP_FAILURE     ICF_ERROR_TMR_STOP_FAILURE
#define ICF_ECODE_INVALID_API_ID       ICF_ERROR_INVALID_API_ID


/* Note: when a new error id is added in file icf_error_id.h, 
   add corresponding ecode here */

#define ICF_ECODE_CALL_FAIL_NO_SDP     ICF_ERROR_CALL_FAIL_NO_SDP

/* This set of ecodes are shared with trace ids */

#define ICF_ECODE_ASSERT_FAILED ICF_TRACE_ASSERT_FAILED

/* Note: when a new trace id is added in file icf_trace_id.h, 
   add corresponding ecode here */


/* These are ecodes for which there is no corresponding error ids */

#define ICF_ECODE_INTERNAL_START         ICF_MAX_NUM_OF_ERROR_IDS + 1

ICF_SESSION_CONTROL_START

/* This ecode will be returned by DBM if no preferred id exists for a line */
#define ICF_ECODE_NO_PREF_ID_EXISTS      ICF_ERROR_NO_PREF_ID_EXISTS

/* This ecode will be set by MM or SSA when a logical error has heppened while
   processing a request from CC or SLP. SSA and MM will also set call clear
   reason in CC call context and return failure */
#define ICF_ECODE_GENERIC_ERROR          ICF_ERROR_GENERIC

/*Error code returned by MMI to CC when no MMI context is found at the time of 
  DMS request from CC*/
#define ICF_ECODE_NO_MMI_CONTEXT_FOUND   ICF_ERROR_NO_MMI_CONTEXT_FOUND
       
ICF_SESSION_CONTROL_END

#ifdef ICF_SIGCOMP_SUPPORT
#define ICF_ECODE_INCOMPLETE_MSG_RECEIVED ICF_ERROR_INCOMPLETE_MSG_RECEIVED
#endif

/* This error Code will be set by SSA when there is a pending in-dialog 
 * refer transaction. Fix for REFER crash
 */
#define ICF_ECODE_REFER_PENDING			 ICF_ERROR_REFER_PENDING

/* This error Code will be set by SSA when there is a mismatch in 
 * either the transport mode or the address plan while originating
 * an outgoing call
 */
#define ICF_ECODE_MISMATCH_IN_ADDR_PLAN		 ICF_ERROR_MISMATCH_IN_ADDR_PLAN

#define ICF_ECODE_NAT_MESSAGE_RECEIVED       ICF_ECODE_INTERNAL_START + 1

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* End of ifndef __ICF_ECODE_H */
