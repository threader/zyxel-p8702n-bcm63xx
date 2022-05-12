/*------------------------------------------------------------------------------
 *
 * File name        : icf_cfw_defs.h
 *
 * Purpose          : This file contains certain hash defines used by CFW.
 *
 * Revision History :
 *
 * Date         Name                Ref#        Comments
 * --------     ------------        ----        ---------------
 * 17-Mar-2005  Shefali Aggarwal    ICF LLD	Beginning of coding phase
 *
 *
 * Copyright (c) 2006, Aricent.
 *----------------------------------------------------------------------------*/


#ifndef __ICF_CFW_DEFS_H__
#define __ICF_CFW_DEFS_H__
#include "icf_map_extif_feature_flags.h"

#ifdef ICF_SERVICE_ENABLE
#include "icf_common_defs.h"

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

#define ICF_ECODE_INVALID_MSG_ID 1     
#define ICF_ECODE_INVALID_CALL_FWD_TYPE 2   
   
/*********TRACES ***************/
/*#define ICF_CFW_TRACE_VALIDITY 1
#define ICF_CFW_TRACE_MSG_SEND 2
#define ICF_CFW_TRACE_MSG_SEND_FAILURE 3
#define ICF_CFW_TRACE_FAIL_TO_GET_CFG_DATA 4
#define ICF_CFW_TRACE_LINE_ID 5
#define ICF_CFW_TRACE_FAIL_TO_GET_LINE_DATA 6
#define ICF_CFW_TRACE_CFW_TYPE 7
#define ICF_CFW_TRACE_CALLING_CFW_ADDR_MATCH 8
#define ICF_CFW_TRACE_ZERO_HOPS 10
#define ICF_CFW_TRACE_CFW_DIV_ADDR_MATCH 11
#define ICF_CFW_TRACE_SELF_CFW_ADDR_MATCH 12
#define ICF_CFW_TRACE_DIV_INFO_PRESENT 13
#define ICF_CFW_TRACE_DIV_INFO_NOT_PRESENT 14
#define ICF_CFW_TRACE_DATA_SEND 15
#define ICF_CFW_TRACE_REL_TYPE_N_CAUSE 16
#define ICF_CFW_TRACE_STATUS 17 */
    
#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif
#endif /* End of __ICF_CFW_DEFS_H__ */
    
    
