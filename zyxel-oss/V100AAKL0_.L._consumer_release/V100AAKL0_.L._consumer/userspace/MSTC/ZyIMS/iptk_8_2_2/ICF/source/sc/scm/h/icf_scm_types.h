/*******************************************************************************
*
* FILE NAME    : icf_scm_types.h
*
* DESCRIPTION  : Contains the datatypes used by the Service Controller Module
*
* Revision History :
*    DATE            NAME       REFERENCE       REASON
* --------    ------------      ---------      ---------------
* 11-Jan-2005  Navneet Gupta     None        Initial
* 21-Sep-2005  Shefali Aggarwal             Added p_second_call_ctx in pdb
* 27-Feb-2006  Amit Sharma                  Merged code for ICF
*
* Copyright 2006,Aricent.
*
*******************************************************************************/
#ifndef __ICF_SCM_TYPES_H__
#define __ICF_SCM_TYPES_H__
#include "icf_map_extif_feature_flags.h"

#ifdef ICF_SERVICE_ENABLE
#include "icf_common_types.h"
#include "icf_macro.h"
#include "icf_sc_common_types.h"
#include "icf_common_interface_types.h"

#ifdef __cplusplus

extern "C"
{
#endif /* __cplusplus */

typedef icf_return_t  (*icf_scm_sl_msg_handler_func_t) (
      INOUT icf_internal_msg_st  *);

/*******************************************************************************
*
* Structure     :  icf_sl_info_st
*
* Description   : This structure stores information about a particular sl
*
*******************************************************************************/
typedef struct
{
icf_scm_sl_msg_handler_func_t msg_handler;
icf_uint8_t			service_bitmask;
} icf_sl_info_st;

/*******************************************************************************
*
* Structure     :  icf_scm_data_st
*
* Description  : This structure defines the global data block of SCM
*
*******************************************************************************/

typedef struct
{
        /* SL Info */
   icf_sl_info_st scm_sl_info[ICF_SCM_MAX_SL]; 
} icf_scm_data_st;

/*******************************************************************************
*
* Structure     : icf_scm_pdb_st
*
* Description   : This structure stores the SCM module specific temporary 
*		  variables which may be useful during execution. 
*		  It is filled every time excution flow enters the module.
*
*******************************************************************************/

typedef struct
{
icf_service_ctx_st   *p_service_ctx;

/* Added in rel 2.0 */
icf_call_ctx_st      *p_second_call_ctx;
icf_glb_data_st      *p_glb_data;
icf_scm_data_st      *p_scm_data;
icf_internal_msg_st  *p_msg;
icf_api_id_t         msg_id;
icf_service_id_t     current_service_index;
icf_uint32_t         trace_int;
icf_boolean_t        call_waiting_fwd;
icf_void_t           *p_chr_buff_evt;
} icf_scm_pdb_st;



#ifdef __cplusplus
}
#endif /*  __cpluscplus */
#endif /* ICF_SERVICE_ENABLE */
#endif /*  __ICF_SCM_TYPES_H__ */
