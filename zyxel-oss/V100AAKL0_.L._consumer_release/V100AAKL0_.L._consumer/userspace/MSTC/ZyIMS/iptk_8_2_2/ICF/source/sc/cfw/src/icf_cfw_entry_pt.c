/*******************************************************************************
 *
 * FILE NAME :      icf_cfw_entry_pt.c
 *
 * DESCRIPTION:     This file contains the code of the entry point function 
 *                  of the Call Forwarding logic.
 *                                    
 * REVISION HISTORY: 
 * 
 * Date         Name                Ref#        Comments
 * --------     ------------        ----        ---------------
 * 11-Mar-2005  Shefali Aggarwal	        	Initial
 *   
 * Copyright (c) 2006, Aricent.
 ******************************************************************************/
#include "icf_map_extif_feature_flags.h"

#ifdef ICF_SERVICE_ENABLE
/*#include "icf_api_id.h"*/
#include "icf_internal_api_id.h"
#include "icf_cfw_prototypes.h"
#include "icf_cfw_macro.h"
#include "icf_cfw_types.h"
#include "icf_cfw_defs.h"
#include "icf_common_prototypes.h"

/******************************************************************************
 *
 * FUNCTION:        icf_cfw_process_mesg
 *
 * DESCRIPTION:     This is the entry point function for Call Forwarding 
 *                  Service Logic Module. 
 *                  
 ******************************************************************************/
icf_return_t icf_cfw_process_mesg(
        INOUT icf_internal_msg_st     *p_internal_msg)
{
    icf_return_t         ret_val = ICF_SUCCESS;
    icf_cfw_pdb_st       cfw_pdb ;
    icf_uint32_t         status;
    
    ICF_SET_MODULE(p_internal_msg->msg_hdr.p_glb_pdb, ICF_INT_MODULE_CFW)

#ifdef ICF_UT_TEST
    /* p_ecode declared in icf_glb_pdb_st will point to 
       ecode in icf_internal_msg_st. To change this ecode, 
       value of pointer will be changed */
    p_internal_msg->msg_hdr.p_glb_pdb->p_ecode = 
        &(p_internal_msg->msg_hdr.ecode);
#endif

    ICF_FUNCTION_ENTER(p_internal_msg->msg_hdr.p_glb_pdb)

      /* Initialize the pdb */
    ret_val = icf_cfw_init_pdb(&cfw_pdb,p_internal_msg);

    /* Check if msg id is not ICF_NEW_SERVICE_TRIGGER then return failure 
     */
    if(ICF_NEW_SERVICE_TRIGGER != p_internal_msg->msg_hdr.msg_id)
    {
        /* Trace to invalid msg id, ignore the msg */
        ICF_CFW_TRACE(&cfw_pdb, ICF_CFW_TRACE_INVALID_MSG_ID)
    }
    
    else 
    {
        ICF_ASSERT(cfw_pdb.p_glb_pdb, 
                (ICF_NULL != p_internal_msg->p_msg_data))

        status = ((icf_service_trigger_st *)
                (p_internal_msg->p_msg_data))->status;
        
        if((ICF_STATUS_CALL_FWD_UNCOND != status) &&
            (ICF_STATUS_CALL_FWD_NO_ANS != status) &&
            (ICF_STATUS_CALL_FWD_BUSY_LINE != status))
        {
            /* Trace to call fwd type invalid, ignore the msg */
            ICF_CFW_TRACE(&cfw_pdb, 
                    ICF_CFW_TRACE_INVALID_CFW_TYPE)
        }
    
        else
        {
            /* Check the flavour of the call forwarding from the status and set
            * the status in the pdb 
            */
            cfw_pdb.cfw_type = status;
            ICF_CFW_TRACE(&cfw_pdb,ICF_CFW_TRACE_CFW_TYPE)

            ret_val = icf_cfw_forward_call(&cfw_pdb);
        }       
    }        
    
    ICF_FUNCTION_EXIT(cfw_pdb.p_glb_pdb)
    ICF_RESET_MODULE(cfw_pdb.p_glb_pdb)
    return ret_val;
}   
            
/******************************************************************************
 *
 * FUNCTION:        icf_cfw_init_pdb
 *
 * DESCRIPTION:     This function initializes the pdb of CFW.
 *                  
 ******************************************************************************/
icf_return_t icf_cfw_init_pdb(
        INOUT icf_cfw_pdb_st          *p_cfw_pdb,
        INOUT    icf_internal_msg_st     *p_internal_msg)
{
    icf_return_t         ret_val = ICF_SUCCESS;
    icf_line_id_t        line_id = 0;
    
    ICF_FUNCTION_ENTER(p_internal_msg->msg_hdr.p_glb_pdb)

    /* Copy the received internal message pointer locally in pdb */
    p_cfw_pdb->p_internal_msg = p_internal_msg;

    /* Copy the global pdb pointer locally in pdb */
    p_cfw_pdb->p_glb_pdb = p_internal_msg->msg_hdr.p_glb_pdb;

    /* Copy the call ctx pointer in the pdb */
    p_cfw_pdb->p_call_ctx = p_cfw_pdb->p_glb_pdb->p_call_ctx;
    ICF_ASSERT(p_cfw_pdb->p_glb_pdb, (ICF_NULL != p_cfw_pdb->p_call_ctx))
    
    /* Check if diversion info is not null in the call ctx received.
     * If it is not null then copy it in the local pdb. If it is null 
     * then set the pointer in the pdb also as null.
     */
    if(ICF_NULL != p_cfw_pdb->p_glb_pdb->p_call_ctx->p_diversion_header_list)
    {
        ICF_CFW_TRACE(p_cfw_pdb,ICF_CFW_TRACE_DIV_INFO_PRESENT)
            
        p_cfw_pdb->p_diversion_info = p_cfw_pdb->p_glb_pdb->p_call_ctx->
            p_diversion_header_list;
    }    
    
    else
    {
        ICF_CFW_TRACE(p_cfw_pdb,ICF_CFW_TRACE_DIV_INFO_NOT_PRESENT)
            
        p_cfw_pdb->p_diversion_info = ICF_NULL;        
    }        

    /* Initialize the flag to indicate that request is valid */
    p_cfw_pdb->is_valid_req = ICF_TRUE;
    
    /* Memset addr_for_cfw = 0 */
    icf_port_memset(
            (icf_void_t*)&(p_cfw_pdb->addr_for_cfw),
            (icf_uint8_t)0,
            (icf_uint32_t)ICF_PORT_SIZEOF(icf_address_st));
        
    /* Initialize the pointer used for traces of data send to SSA */
    p_cfw_pdb->p_temp_div_data = ICF_NULL;

    /* Initialize the config data and the line data pointers in the pdb to null
     */
    p_cfw_pdb->p_config_data = ICF_NULL;
    for (line_id = 0; line_id < ICF_MAX_NUM_OF_LINES; line_id++)
    {
        p_cfw_pdb->p_line_data[line_id] = ICF_NULL;
    }

    /* Adding intialization for rest of members */
    p_cfw_pdb->line_id = 0;
    p_cfw_pdb->cfw_type = 0;
	    
    ICF_FUNCTION_EXIT(p_cfw_pdb->p_glb_pdb)
    return ret_val;
}
#endif
