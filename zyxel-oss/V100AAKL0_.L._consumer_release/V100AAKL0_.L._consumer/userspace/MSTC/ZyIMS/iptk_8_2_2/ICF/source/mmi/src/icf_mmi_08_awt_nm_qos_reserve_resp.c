/******************************************************************************
 * 
 * FILE NAME:   icf_mmi_08_awt_nm_qos_reserve_resp.c
 * 
 * DESCRIPTION: This file contains the functions of STATE
 *              ICF_MMI_08_AWT_NM_QOS_RESERVE_RESP of MMI.  
 *              
 * REVISION HISTORY:    
 *   Date            Name            Reference       Comments
 * 14-Feb-2008  Abhishek Dhammawat     Rel8.0     Moved the functions from
 *                                                icf_mmi_api_handlers.c here
 *                                                icf_mmi_08_dms_request
 *                                                icf_mmi_08_nm_qos_reserve_resp
 * Copyright (c) 2008, Aricent.  
 ******************************************************************************/

#include "icf_internal_api_id.h"
#include "icf_mmi_defs.h"
#include "icf_mmi_types.h"
#include "icf_mmi_macro.h"
#include "icf_mmi_prototypes.h"
#include "icf_dbm_prototypes.h"
#include "icf_ecode.h"

#ifdef ICF_QOS_SUPPORT

/******************************************************************************
 *
 * FUNCTION:        icf_mmi_08_nm_qos_reserve_resp
 * 
 * DESCRIPTION:     This function is invoked when MMI is awaiting Secondary
 *                  PDP Context Activation Response from NM and it receives the
 *                  same. 
 *                  Current State: ICF_MMI_08_AWT_NM_QOS_RESERVE_RESP
 *                  Current Event: ICF_MMI_NM_SEC_ACT_RESP
 *                                    
 * RETURNS:         ICF_SUCCESS in case of success and ICF_FAILURE in 
 *                  case of any failure.                  
 *
 *****************************************************************************/

icf_return_t icf_mmi_08_nm_qos_reserve_resp(
        INOUT icf_mmi_pdb_st         *p_mmi_pdb)
{
    icf_return_t			ret_val = ICF_SUCCESS;
    icf_msg_resp_st		*p_msg_data_recd = ICF_NULL;
    
    ICF_FUNCTION_ENTER(p_mmi_pdb->p_glb_pdb)
        
    p_msg_data_recd = (icf_msg_resp_st *)
        (p_mmi_pdb->p_internal_msg->p_msg_data);
    
    /* Request DBM for the ctx with the call id in the recvd msg */
    
    ICF_MMI_SET_STATE(p_mmi_pdb,ICF_MMI_00_IDLE)
    
    /* If failure response is recvd then function to process failure response
     * is called
     */
    if(ICF_FAILURE == p_msg_data_recd->result)
    {
        ICF_MMI_TRACE(p_mmi_pdb,ICF_TRACE_FAILURE_RESP)
            
        ret_val = icf_mmi_qos_reserve_failure_resp(p_mmi_pdb);
        if(ICF_FAILURE == ret_val)
        {
            /* Failure to process gcl failure response */
            ICF_MMI_TRACE(p_mmi_pdb,ICF_TRACE_FAILURE_TO_PROCESS_MSG)
        }
    }
        
    /* If the response is success then function to process sucess response is 
     * called
     */
    else
    {
        ICF_MMI_TRACE(p_mmi_pdb,ICF_TRACE_SUCCESS_RESP)
            
        ret_val = icf_mmi_qos_reserve_success_resp(p_mmi_pdb);
        if(ICF_FAILURE == ret_val)
        {
            /* Failure to process gcl success response */
            ICF_MMI_TRACE(p_mmi_pdb,ICF_TRACE_FAILURE_TO_PROCESS_MSG)
        }
    }

    ICF_FUNCTION_EXIT(p_mmi_pdb->p_glb_pdb)
    return ret_val;       
}/* End function */
#endif
