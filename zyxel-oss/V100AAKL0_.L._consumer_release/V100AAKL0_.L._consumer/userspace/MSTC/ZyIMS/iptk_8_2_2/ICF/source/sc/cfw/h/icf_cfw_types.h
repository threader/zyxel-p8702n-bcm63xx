/*------------------------------------------------------------------------------
 *
 * File name        : icf_cfw_types.h
 *
 * Purpose          : This file contains the data structures used by CFW.
 *
 * Revision History :
 *
 * Date         Name            	Ref#        Comments
 * --------     ------------    	----        ---------------
 * 11-Mar-2005  Shefali Aggarwal	            Initial
 *
 *
 * Copyright (c) 2006, Aricent .
 *----------------------------------------------------------------------------*/


#ifndef __ICF_CFW_TYPES_H__
#define __ICF_CFW_TYPES_H__
#include "icf_map_extif_feature_flags.h"

#ifdef ICF_SERVICE_ENABLE
/*#include "icf_types.h"*/

#include "icf_common_types.h"
#include "icf_common_interface_types.h"

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
 * Type Name   :  icf_cfw_pdb_st
 * Description :  Structure for CFW pdb
 *
 *----------------------------------------------------------------------------*/    
 typedef struct
 {
    /* This is the pointer to the internal msg received in the process mesg 
     * from SCM
     */
    icf_internal_msg_st      *p_internal_msg;
    
    /* Pointer to global pdb */
    icf_glb_pdb_st          *p_glb_pdb;
    
    /* Flag to indicate whether call forwarding request is valid or not */
    icf_boolean_t            is_valid_req;
    
    /* Pointer to call ctx */
    icf_call_ctx_st          *p_call_ctx;
    
    /* Pointer to the diversion info present in the current call context */
    icf_diversion_info_st    *p_diversion_info;
    
    /* Datatype to save the type of call fwd requested */
    icf_uint32_t              cfw_type;
    
    /* Address to which the call is to be forwarded */
    icf_address_st           addr_for_cfw;
    
    /* For traces */
    icf_line_id_t            line_id;
    
    /* For traces */
    icf_diversion_data_st    *p_temp_div_data;

    /* Pointer to config data */
    icf_config_data_st       *p_config_data;

    /* Pointer to line data */
    icf_line_data_st         *p_line_data[ICF_MAX_NUM_OF_LINES];
 }icf_cfw_pdb_st;  
    
    
#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif
#endif /* __ICF_CFW_TYPES_H__ */
