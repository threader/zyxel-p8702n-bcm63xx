/*------------------------------------------------------------------------------
 *
 * File name        : icf_cm_defs.h
 *
 * Purpose          : This file contains the hash defines used by CM.
 *
 * Revision History :
 *
 * Date         Name            Ref#            Comments
 * --------     ------------    ----            ---------------
 * 21-Jan-2005  Rohit Aggarwal  ICF LLD      Beginning of coding phase
 *
 * 
 * Copyright (c) 2006, Aricent.
 *----------------------------------------------------------------------------*/


#ifndef __ICF_CM_DEFS_H__
#define __ICF_CM_DEFS_H__


/*
 * Determine if a C++ compiler is being used. If so, ensure that standard C is
 * used to process the header information.
 */
#ifdef __cplusplus
/* Yes, C++ compiler is present. Use standard C */
extern "C"
{
#endif /* __cplusplus */

    
/*------------------------------------------------------------------------------
 * Hash define values for check status of type icf_cm_check_status_t
 *----------------------------------------------------------------------------*/
#define ICF_CM_CHECK_STATUS_CONT     0x00
#define ICF_CM_CHECK_STATUS_END      0x01


/*------------------------------------------------------------------------------
 * Hash define values for check type icf_cm_check_type_t
 *----------------------------------------------------------------------------*/
/* Check for forward message received by CM entry point function */
#define ICF_CM_CHECK_TYPE_FORWARD        0
/* Check for backward message received from SC */
#define ICF_CM_CHECK_TYPE_BACKWARD_SC    1
/* Check for backward message received from CC */
#define ICF_CM_CHECK_TYPE_BACKWARD_CC    2
/* Maximum number of unique directions of message flow */
#define ICF_CM_CHECK_TYPE_MAX            3


/*------------------------------------------------------------------------------
 * Hash define value for no destination of type icf_int_module_id_t
 * This will be used when received message need not be forwarded to CC or ADM.
 *----------------------------------------------------------------------------*/
#define ICF_CM_DESTINATION_NONE          ICF_INT_MAX_MODULE + 1


/*------------------------------------------------------------------------------
 * Hash define values for counts, buffer sizes, ranges
 *----------------------------------------------------------------------------*/
#define ICF_CM_MAX_BUFFER_SIZE           100

    
    
#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* End of ifndef __ICF_CM_DEFS_H__ */
