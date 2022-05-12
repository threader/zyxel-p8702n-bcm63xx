/*------------------------------------------------------------------------------
 *
 * File name        : icf_sc_common_types.h
 *
 * Purpose          : This file contains the types definitions of SCM module.
 *
 * Revision History :
 *
 * Date         Name            Ref#                Comments
 * --------     ------------    ----                ---------------
 * 15-Dec-2004  Amit Sharma                           Initial
 * 28-Jan-2005  Navneet Gupta			      Changed Active Services 
 *	                                              Structure
 * 2-Feb-2005  Amit Sharma                           To remove icf_chr_types.h
 *
 *
 * Copyright (c) 2006, Aricent.
 *----------------------------------------------------------------------------*/


#ifndef __ICF_SC_COMMON_TYPES_H__
#define __ICF_SC_COMMON_TYPES_H__

#include "icf_common_types.h"
/*
 * Determine if a C++ compiler is being used. If so, ensure that standard C is
 * used to process the header information.
 */
#ifdef __cplusplus
/* Yes, C++ compiler is present. Use standard C */
extern "C"
{
#endif /* __cplusplus */
/* Number of Service Logic Modules */
#define ICF_SCM_MAX_SL    (ICF_INT_MODULE_SL_END - ICF_INT_MODULE_SL_START)


/*------------------------------------------------------------------------------
*
* Name : icf_service_id_t
*
* Description  : This contains the id of the services
*
*-----------------------------------------------------------------------------*/
typedef icf_uint8_t  icf_service_id_t;

/*------------------------------------------------------------------------------
*
* Name : icf_service_list_st
*
* Description  : This structure contains the list of active services
*                and pointers of service logic contexts
*
*-----------------------------------------------------------------------------*/
typedef struct
{
    icf_uint8_t        active_service_status;
    icf_void_t         *p_sl_ctx[ICF_SCM_MAX_SL];
    

} icf_service_list_st;

/*------------------------------------------------------------------------------
*
* Name : icf_expected_events_st
*
* Description  : This structure contains parameters to route the message
*                to service logics
*
*-----------------------------------------------------------------------------*/
typedef struct
{
    icf_service_id_t     media_events;

    icf_service_id_t     start_proc_media;

    icf_service_id_t     signaling_events;

    icf_service_id_t     start_proc_signaling;

    icf_service_id_t     resp_frm_chr;

} icf_expected_events_st; /* typedef struct */


    
/*------------------------------------------------------------------------------
*
* Name : icf_service_ctx_st
*
* Description  : This structure defines the service context
*
*-----------------------------------------------------------------------------*/
typedef struct icf_service_ctx_st
{
    /* list of services logic context which are active */
    icf_service_list_st      active_service_list;
    
    /* this structure stores the expected events and the service ids 
     * which can want to handle them */
    icf_expected_events_st   expected_events;
    
    /* pointer to self call context */
    icf_call_ctx_st          *p_call_ctx;

} icf_service_ctx_st;


#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif
