/*------------------------------------------------------------------------------
 *
 * File name        : icf_mrm_prototypes.h
 *
 * Purpose          : This file consists functions' prototypes in MRM module 
 * 
 * Revision History :
 *
 *   Date           Name           Ref#        Comments
 * --------      ------------      ----      ---------------
 *  21 DEC 2004   Sumit Gupta                File created and initial version
 *
 *
 *
 * Copyright (c) 2006, Aricent.
 *----------------------------------------------------------------------------*/


#ifndef __ICF_MRM_PROTOTYPES_H__
#define __ICF_MRM_PROTOTYPES_H__

/* Determine if a C++ compiler is being used.
 * If so, ensure that standard
 * C is used to process the API information.
 */

#include "icf_common_types.h"
#include "icf_common_defs.h"


#ifdef __cplusplus

/* Yes, C++ compiler is present.
 * Use standard C.
 */


extern "C"
{
#endif /* __cplusplus */


#if defined ICF_PORT_WINCE
/************************************************************************
 *
 * DESCRIPTION:  This function would create allocate memory for global PDB and 
 *               initialize global data non-scalable data structures
 *
 ************************************************************************/
icf_return_t 
icf_mrm_init(
		IN	icf_uint8_t sys_resource_limit,
		IN	icf_uint8_t logs_indication,
        OUT icf_void_t **p_glb_pdb);
#else
/************************************************************************
 *
 * DESCRIPTION:  This function would create allocate memory for global PDB and 
 *               initialize global data non-scalable data structures
 *
 ************************************************************************/
icf_return_t 
icf_mrm_init(
		IN	icf_uint8_t sys_resource_limit,
        OUT icf_void_t **p_glb_pdb);
#endif


/************************************************************************
 *
 * DESCRIPTION:  This function would initialize the entire ICF
 *                
 *
 ************************************************************************/

icf_return_t 
icf_init(
        INOUT	icf_init_param_st	*p_init_param,
		OUT icf_void_t			**p_p_icf_glb_data);

/****************************************************************************
** Function: icf_auto_activate
**
****************************************************************************
** DESCRIPTION: This function scales the database as per the initial parameters 
**            
** 
** 
**
***************************************************************************/
icf_return_t icf_auto_activate (
					INOUT	icf_glb_pdb_st     *p_glb_pdb, 
					IN		icf_init_param_st  *p_init_param,
					INOUT	icf_error_t        *p_ecode);

/****************************************************************************
** Function: icf_auto_register
**
****************************************************************************
** DESCRIPTION: This function scales the database as per the initial parameters 
**            
** 
** 
**
***************************************************************************/
icf_return_t icf_auto_register (
					INOUT	icf_glb_pdb_st     *p_glb_pdb, 
					IN		icf_init_param_st  *p_init_param,
					INOUT	icf_error_t        *p_ecode);

 /************************************************************************
 *
 * DESCRIPTION:  The function would call the porting layer's recv function 
 *               for reading data from socetets , pipes etc
 *
 ************************************************************************/
icf_return_t
icf_mrm_recv_msg(
        INOUT icf_glb_pdb_st *p_glb_pdb,
        OUT   icf_error_t *p_ecode);

/************************************************************************
 *
 * DESCRIPTION:  This function would free memory allocated for global PDB
 *
 ************************************************************************/
icf_return_t 
icf_mrm_static_deinit(
        INOUT icf_void_t *p_glb_pdb);


/************************************************************************
 *
 * DESCRIPTION:  Entry point function for MRM module
 *
 ************************************************************************/
#if defined(ICF_PORT_SYMBIAN)
icf_return_t
icf_process_message (
        INOUT icf_void_t *p_glb_pdb,
		INOUT icf_msg_list_st *p_msg_lst);
#else
icf_return_t
icf_process_message (
        INOUT icf_void_t *p_glb_pdb);
#endif

/****************************************************************************
*
* DESCRIPTION :   This function reports the genlib timer expiry to a module.
*
*****************************************************************************/
icf_void_t
icf_mrm_timer_cb_fn(
        INOUT icf_void_t *p_buf);


/****************************************************************************
*
* DESCRIPTION :   This function reports the genlib timer expiry to a module.
*
*****************************************************************************/
icf_return_t
icf_mrm_handle_failure(
        INOUT icf_glb_pdb_st *p_glb_pdb,
        IN icf_module_id_t failure_ret_module);

/****************************************************************************
*
* DESCRIPTION :   This is utility function to route messages from ICF.
*
*****************************************************************************/
icf_return_t 
icf_mrm_icf_process_msg(
        INOUT icf_internal_msg_st *p_internal_msg);

/****************************************************************************
 *
 * DESCRIPTION : This is utility function to send appropriate message to modules
 *               in case the message was not successfully processed.
 * 
 *****************************************************************************/
icf_void_t icf_mrm_handle_process_fail(
        icf_internal_msg_st *p_internal_msg,
        icf_uint8_t failure_ret_module);


/************************************************************************
 *
 * DESCRIPTION:  This function would initialize global PDB
 *
 ************************************************************************/
icf_void_t
icf_mrm_init_glb_pdb(
        INOUT icf_glb_pdb_st *p_glb_pdb);


icf_void_t 
icf_mrm_map_and_process_external_api_for_sic(
        icf_internal_msg_st *p_internal_msg);

icf_return_t 
icf_mrm_validate_hdr_and_get_dest_module(
                                            icf_glb_pdb_st    *p_glb_pdb,
                                            icf_api_header_st *p_hdr, 
                                            icf_int_module_id_t *p_dest_int_module,
                                            icf_error_t  *p_ecode   );
#ifdef ICF_TRACE_ENABLE
/*******************************************************************************
 *
 * FUNCTION:        icf_mrm_app_trace
 *
 * DESCRIPTION:     This function is used by MRM for trace handling.
 *
 ******************************************************************************/
 icf_return_t icf_mrm_app_trace(
              INOUT  icf_glb_pdb_st *p_glb_pdb,
              INOUT icf_trace_id_t trace_id);
#endif

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* __ICF_MRM_PROTOTYPES_H__ */

