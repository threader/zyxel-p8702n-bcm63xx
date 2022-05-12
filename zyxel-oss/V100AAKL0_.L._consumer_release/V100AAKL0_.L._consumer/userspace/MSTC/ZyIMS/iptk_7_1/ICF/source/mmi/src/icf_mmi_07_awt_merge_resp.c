/******************************************************************************
 * 
 * FILE NAME:   icf_mmi_07_awt_merge_resp.c
 * 
 * DESCRIPTION: This file contains functions to handle events in the AWT MERGE
 * 				response state in MMI.
 *              
 * REVISION HISTORY:    
 *   Date            Name          Reference       Comments
 * 25-july-2006     Puneet       ICF Rel 6.0      Initial Version- support for 
 *                                                multiple m-lines
 *
 * 10-Dec-2007      Jagmohan Chauhan ICF Rel 7.0   SPR fix:17665
 * 												 * 						
 * 08-Jan-2009      Vinay Jindal     19250       Updated icf_mmi_07_merge_resp
 * 												  						
 * Copyright (c) 2009, Aricent.  
 *
 ******************************************************************************/

/* include files */
#include "icf_internal_api_id.h"
#include "icf_mmi_defs.h"
#include "icf_mmi_types.h"
#include "icf_mmi_macro.h"
#include "icf_mmi_prototypes.h"
#include "icf_dbm_prototypes.h"
#include "icf_ecode.h"


/******************************************************************************
 *
 * FUNCTION:        icf_mmi_07_dms_request
 * 
 * DESCRIPTION:     This function is invoked when the MMI is in 
 *                  ICF_MMI_07_AWT_MERGE_RESP and it receives a DMS 
 *                  req from CC.
 *                                    
 * RETURNS:         ICF_SUCCESS in case of success and ICF_FAILURE in 
 *                  case of any failure.                  
 *
 *****************************************************************************/

icf_return_t icf_mmi_07_dms_request(
        INOUT icf_mmi_pdb_st             *p_mmi_pdb)
{
    icf_return_t                         ret_val = ICF_SUCCESS;
    icf_mmi_dms_request_st               *p_msg_data_recd = ICF_NULL;
    icf_call_ctx_st                      *p_dbm_call_ctx = ICF_NULL;
    
    ICF_FUNCTION_ENTER(p_mmi_pdb->p_glb_pdb)
        
    p_msg_data_recd = (icf_mmi_dms_request_st *)(p_mmi_pdb->
            p_internal_msg->p_msg_data);
    
    /* Check if it is a forceful delete */
    if(ICF_TRUE == p_msg_data_recd->forceful_delete)
    {
        ICF_MMI_TRACE(p_mmi_pdb,ICF_TRACE_FORCEFUL_DMS)
            
        /* Get Call ctx pointer from DBM to update the mmi ctx to reset the 
         * merge call id field and the state in the mmi ctx corresponding to 
         * both the call ids
         */
            
        ret_val = icf_dbm_find_call_cntxt_blk(
                p_mmi_pdb->p_glb_pdb,
                p_mmi_pdb->p_mmi_ctx->merge_call_id,
                &(p_dbm_call_ctx),
                p_mmi_pdb->p_ecode);
        
        if(ICF_SUCCESS == ret_val)
        {
            p_dbm_call_ctx->mmi_context.merge_call_id  = ICF_INVALID_CALL_ID;
            p_dbm_call_ctx->mmi_context.mmi_state = ICF_MMI_00_IDLE;
            p_mmi_pdb->p_mmi_ctx->merge_call_id = ICF_INVALID_CALL_ID;
        }
        else
        {
            /* failure by dbm */
            ICF_MMI_TRACE(p_mmi_pdb,ICF_TRACE_FAILURE_BY_DBM)
            return ret_val;                
        }
                    
        /* Send DMS req to MM and Rel Res Req to RM simultaneously */
		icf_mmi_send_forceful_dms_req(p_mmi_pdb);
    }
    
    /* if it is not a forceful delete */
    else
    {
        ICF_MMI_TRACE(p_mmi_pdb,ICF_TRACE_DMS_NOT_FORCEFUL)
        /* Ignore the req */
    }
    
    ICF_FUNCTION_EXIT(p_mmi_pdb->p_glb_pdb)
    return ret_val;        
}/* End function */

/******************************************************************************
 *
 * FUNCTION:        icf_mmi_07_merge_resp
 * 
 * DESCRIPTION:     This function is invoked when the MMI is in 
 *                  ICF_MMI_07_AWT_MERGE_RESP and it receives the 
 *                  same from MM.
 *                                    
 * RETURNS:         ICF_SUCCESS in case of success and ICF_FAILURE in 
 *                  case of any failure.                  
 *
 *****************************************************************************/

icf_return_t icf_mmi_07_merge_resp(
        INOUT icf_mmi_pdb_st             *p_mmi_pdb)
{
    icf_return_t                         ret_val = ICF_SUCCESS;
    icf_msg_resp_st *p_recv_msg_payload = ICF_NULL;
    icf_internal_msg_st                  *p_cm_internal_msg = ICF_NULL;
    icf_mm_err_ind_st                    err_ind;
    icf_call_ctx_st                      *p_dbm_call_ctx = ICF_NULL;
    icf_config_data_st                   *p_config_data = ICF_NULL;
        
    ICF_FUNCTION_ENTER(p_mmi_pdb->p_glb_pdb)
        
    /* Update call states and merge call id field in both ctx */
    ICF_MMI_SET_STATE(p_mmi_pdb,ICF_MMI_00_IDLE)

	/* Get the config data */
    ret_val = icf_dbm_get_module_glb_data(
            p_mmi_pdb->p_glb_pdb,
            (icf_int_module_id_t)ICF_INT_MODULE_CFG,
            (icf_void_t *)&p_config_data, p_mmi_pdb->p_ecode);
        
    ICF_ASSERT(p_mmi_pdb->p_glb_pdb, (ICF_NULL != p_config_data));
        
    /* Get pointer of call ctx corresponding to second call id from DBM */
    ret_val = icf_dbm_find_call_cntxt_blk(
            p_mmi_pdb->p_glb_pdb,
            p_mmi_pdb->p_mmi_ctx->merge_call_id,
            &(p_dbm_call_ctx),
            p_mmi_pdb->p_ecode);
    
    if(ICF_SUCCESS == ret_val)
    {
        p_dbm_call_ctx->mmi_context.mmi_state = ICF_MMI_00_IDLE;
    }
    else
    {
        /* failure by dbm */
        ICF_MMI_TRACE(p_mmi_pdb,ICF_TRACE_FAILURE_BY_DBM)
        return ret_val;            
    }
    
    /* Check call state of call ctx. If CC is in AWT DMS state then present 
     * session needs to be cleared
     */
    
    if(ICF_CC_07_AWT_DMS_RESP == p_mmi_pdb->p_call_ctx-> call_state)
    {
        ICF_MMI_TRACE(p_mmi_pdb,ICF_TRACE_CC_AWT_DMS)

		icf_mmi_send_awaited_dms_req(p_mmi_pdb);
        /* 
         * The other call in conference may also be in releasing phase and if cc
         * would have sent DMS for the other call it would have been ignored so
         * check if the state in call contect is AWT_DMS_RESP for other call in
         * conference then send dms for the other call as well
         */
         if(ICF_CC_07_AWT_DMS_RESP == p_dbm_call_ctx->call_state) 
         {
             /* Set the call context in mmi pdb to the call context of the other
              * call in conference */
             p_mmi_pdb->p_call_ctx = p_dbm_call_ctx;

			 p_mmi_pdb->p_mmi_ctx->ongoing_event = ICF_MMI_DMS_ONGOING;
             
             /* Send DMS req to MM */
             ret_val = icf_mmi_send_dms_req(p_mmi_pdb);            
             if(ICF_SUCCESS == ret_val)
             {
                 /* Change state */
                 p_dbm_call_ctx->mmi_context.mmi_state = 
						 ICF_MMI_05_AWT_DMS_RESP;
             }
         }
    }/* end if */
    
    /* Rest processing will be done if CC is not in AWT DMS state */
    else
    {
        p_recv_msg_payload = (icf_msg_resp_st *)
            (p_mmi_pdb->p_glb_pdb->p_recv_msg->payload);
        
        p_cm_internal_msg = p_mmi_pdb->p_internal_msg;
        
		/* If this parameter is set, then dont set p_recv_msg to NULL, so that 
         * payload becomes available in twc module
         */

        if(ICF_TRUE != p_config_data->conf_fail_call_clear_not_reqd)
		{
			p_cm_internal_msg->msg_hdr.p_glb_pdb->p_recv_msg = ICF_NULL;
			p_cm_internal_msg->msg_hdr.payload_length = 0;
		}
        
        /* check the response recvd */
        if(ICF_SUCCESS == p_recv_msg_payload->result)
        {
            /* Send success response to SC */
            p_cm_internal_msg->msg_hdr.msg_id = ICF_CREATE_CONF_RESP;
        }
        else
        {
            /* Send failure response to SC */
            p_cm_internal_msg->msg_hdr.msg_id = ICF_MEDIA_ERROR_IND;
            err_ind.reason = ICF_ECODE_MERGE_RESP_FAILURE;
            p_cm_internal_msg->p_msg_data = (icf_void_t *)&(err_ind);
        }
        
        ret_val = icf_cm_process_mesg(p_cm_internal_msg);
        if(ICF_FAILURE == ret_val)
        {
            /* failure by cm */
            ICF_MMI_TRACE(p_mmi_pdb,ICF_TRACE_FAILURE_BY_CM)
        }
#ifdef ICF_TRACE_ENABLE
        /* Fix for SPR 17665 so that merge media resp traces get printed */
        p_mmi_pdb->result =  p_recv_msg_payload->result; 
    
        ICF_MMI_APP_TRACE(p_mmi_pdb,ICF_TRACE_STREAM_MERGE)
#endif
  
    }/* End else */
 
    ICF_FUNCTION_EXIT(p_mmi_pdb->p_glb_pdb)
    return ret_val;    
}/* End function */  


