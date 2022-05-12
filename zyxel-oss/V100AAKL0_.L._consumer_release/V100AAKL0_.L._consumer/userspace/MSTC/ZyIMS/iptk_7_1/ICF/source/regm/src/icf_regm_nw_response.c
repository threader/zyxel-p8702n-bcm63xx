/*------------------------------------------------------------------------------
 *
 * File name        : icf_regm_nw_response.c
 *
 * Purpose          : This file contains the network response handler functions.
 *
 * Revision History :
 *
 * Date           Name          Ref#                 Comments
 * --------    ------------     ----                 ---------------
 * 23-Dec-2004  Jalaj Negi     Non Service LLD      Initial
 *
 * 05-Dec-2005	Jyoti Narula	ICF 4.0 CDD			Changes under ICF_4_0_CRM_START and END
 *
 * 28-Nov-2006  Deepti Goyal    ICF rel 6.2         NAT rport feature support
 *
 * 12-Dec-2006  Deepti Goyal    SPR 14030           Set the nat binding change
 *                                                  flag to false when rport
 *                                                  is complete.
 *
 * 14-Dec-2006  Deepti Goyal    SPR 14063           Check rport status as
 *                                                  equality and not bitmask.
 * 17-Dec-2009  Pavneet Bajaj   SPR 20244           Guard Timer Implementation
 *
 * Copyright (c) 2006, Aricent.
 *----------------------------------------------------------------------------*/

#include "icf_dbm_prototypes.h"
#include "icf_regm_prototypes.h"
#include "icf_regm_macro.h"

/****************************************************************************
 * FUNCTION:            icf_rgm_process_nw_response
 *
 * DESCRIPTION:         This function is called configuration module when
 *                      it receives response from network for a registration
 *                      request.CFG module passes response structure to this
 *                      function. After that this function carries out the
 *                      necessary processing.
 *
 * RETURNS:             
 *
 ***************************************************************************/

icf_return_t icf_rgm_process_nw_response(
        INOUT icf_crm_pdb_st *p_crm_pdb)
{
	
    icf_void_t *p_buff = ICF_NULL;
    icf_rgm_glb_data_st *p_rgm_glb_data = ICF_NULL;
    icf_rgm_nw_resp_st *p_nw_resp = ICF_NULL;
    icf_rgm_context_st *p_rgm_context = ICF_NULL;
    icf_return_t ret_val;
#ifdef ICF_NAT_RPORT_SUPPORT
    icf_rport_config_data_st   *p_rport_config_data = ICF_NULL;
    icf_line_data_st           *p_line_data = ICF_NULL;
#endif

ICF_4_0_CRM_START
	icf_uint32_t				  loop_app= ICF_NULL;
	icf_header_list_st*        p_hdr_list = ICF_NULL;
ICF_4_0_CRM_END


    /* Variables used for Code Area reduction */
    icf_glb_pdb_st *p_glb_pdb;
   
    ICF_SET_MODULE(p_crm_pdb->p_internal_msg->msg_hdr.p_glb_pdb,
            ICF_INT_MODULE_REGM)

    /* function entry trace */
    ICF_FUNCTION_ENTER(p_crm_pdb->p_internal_msg->msg_hdr.p_glb_pdb)
        
    /* Typecast the data in internal message to network response structure */ 
    p_nw_resp = (icf_rgm_nw_resp_st *)(p_crm_pdb->
            p_internal_msg->p_msg_data);
    ICF_ASSERT(
            p_crm_pdb->p_internal_msg->msg_hdr.p_glb_pdb,
            (ICF_NULL != p_nw_resp))

    /* Get the pointer to registration context */
    p_rgm_context = p_nw_resp->p_rgm_context;
    p_crm_pdb->p_rgm_context = p_rgm_context;

    ICF_ASSERT(
            p_crm_pdb->p_internal_msg->msg_hdr.p_glb_pdb,
            (ICF_NULL != p_rgm_context))
        
    /* Get the pointer to global PDB */
    p_glb_pdb = p_crm_pdb->p_internal_msg->msg_hdr.p_glb_pdb;
    ICF_ASSERT(
            p_crm_pdb->p_internal_msg->msg_hdr.p_glb_pdb,
            (ICF_NULL != p_glb_pdb))

    /* Get the pointer to ecode */
    p_crm_pdb->p_ecode = &(p_crm_pdb->p_internal_msg->msg_hdr.ecode);
    ICF_ASSERT(
            p_crm_pdb->p_internal_msg->msg_hdr.p_glb_pdb,
            (ICF_NULL != p_crm_pdb->p_ecode))
    
    /* Call DBM to get the pointer to the RGM global data in p_buff */
    ret_val = icf_dbm_get_module_glb_data(
            p_glb_pdb,
            (icf_int_module_id_t)(ICF_INT_MODULE_REGM),
            &p_buff,
            p_crm_pdb->p_ecode);     

    ICF_ASSERT(
            p_crm_pdb->p_internal_msg->msg_hdr.p_glb_pdb,
            (ICF_NULL != p_buff))
    
#ifdef IMS_CLIENT
ICF_CHECK_IMS_START(p_glb_pdb,ICF_TRUE)
    
    /* Unset the reg ongoing flag */
    ((icf_rgm_glb_data_st *)p_buff)->reg_ongoing = ICF_FALSE;
ICF_CHECK_IMS_END
#endif	
	/* get the pointer to CFG data: NULL uptil now  */

	ret_val = icf_dbm_get_module_glb_data(
                p_glb_pdb,
                (icf_int_module_id_t)(ICF_INT_MODULE_CFG),
                (icf_void_t **)(&(p_crm_pdb->p_config_data)),
                p_crm_pdb->p_ecode);     

     ICF_ASSERT(p_crm_pdb->p_internal_msg->msg_hdr.p_glb_pdb,
							(ICF_NULL != p_crm_pdb->p_config_data))

    
    /* Typecast into global data structure */
    p_rgm_glb_data = (icf_rgm_glb_data_st *)(p_buff);

	 p_crm_pdb->p_rgm_glb_data = p_rgm_glb_data;
#ifdef ICF_NAT_RPORT_SUPPORT

 if (ICF_CFG_SELF_ADDR_PER_LINE ==
            p_crm_pdb->p_config_data->self_addr_per_line)
    {
        if(ICF_FAILURE == icf_dbm_fetch_line_cntxt_blk(
                    p_crm_pdb->p_glb_pdb, p_rgm_context->line_id,
                    &p_line_data, p_crm_pdb->p_ecode))
        {
            ret_val = ICF_FAILURE;
        }
        else
        {
            p_rport_config_data = p_line_data->p_rport_config_data;
        }
    
    }
    else
    {
        p_rport_config_data = &(p_crm_pdb->p_config_data->rport_config_data);
    }
    if (ICF_NULL == p_rport_config_data)
    {
       return ICF_FAILURE;
    }
#endif

    /*CSR 8052488 SPR 20244 This is for the guard timer that was 
    started before sending out any reg/dereg request on n/w*/
    icf_rgm_stop_timer(p_crm_pdb);

    if (p_nw_resp->status == ICF_RGM_SSA_NW_SUCCESS)
    {
        
#ifdef ICF_TRACE_ENABLE
        if (ICF_RGM_05_DEREGISTERING == p_rgm_context->rgm_state)
        {
            ICF_RGM_TRACE(p_crm_pdb,
                    ICF_TRACE_DEREG_SUCCESS_RESP)
        }
        else
        {
            ICF_RGM_TRACE(p_crm_pdb,
                    ICF_TRACE_REG_SUCCESS_RESP)            
        }
#endif        
        
        ICF_RGM_TRACE(p_crm_pdb,
                ICF_TRACE_REGISTRATION_FSM_TRIGGERED)
        
#ifdef ICF_NAT_RPORT_SUPPORT
            /* If the rport status is set to ICF_INITIAL_RPORT_INITIAL and 
             * nat binding has changed then invoke the regsitration state
             * to handle the NAT BINDING CHANGE event.
             */
            if((ICF_TRUE == p_rport_config_data->is_rport_reqd)\
                    && ((ICF_TRUE == p_rport_config_data->nat_binding_chng)\
                        ||(ICF_INITIAL_RPORT_PENDING == \
                            p_rgm_context->rport_status)))
            {
                if(p_rport_config_data->reg_id == p_rgm_context->reg_id)
                {
                    ret_val = p_rgm_glb_data->icf_rgm_state_machine\
                              [p_rgm_context->rgm_state]\
                              [ICF_RGM_NAT_BINDING_CHNG_IND](p_crm_pdb);
                }
                else
                {
                    ret_val = p_rgm_glb_data->icf_rgm_state_machine\
                              [p_rgm_context->rgm_state]\
                              [ICF_RGM_SUCCESS_RESPONSE](p_crm_pdb);

                }
            }
            else
            {
                if((ICF_TRUE == p_rport_config_data->is_rport_reqd)\
                    && (ICF_INITIAL_RPORT_ONGOING == p_rgm_context->\
                        rport_status))
                {
                    if(p_rport_config_data->reg_id == p_rgm_context->reg_id)
                    {

                        p_rgm_context->rport_status = \
                                               ICF_INITIAL_RPORT_COMPLETED;

                        p_rport_config_data->is_rport_completed = ICF_TRUE;
                        p_rport_config_data->nat_binding_chng = ICF_FALSE;
                    }
                }

                ret_val = p_rgm_glb_data->icf_rgm_state_machine[p_rgm_context->
                    rgm_state][ICF_RGM_SUCCESS_RESPONSE](p_crm_pdb);

            }
#else
        /* Trigger FSM */
        ret_val = p_rgm_glb_data->icf_rgm_state_machine[p_rgm_context->
            rgm_state][ICF_RGM_SUCCESS_RESPONSE](p_crm_pdb);
#endif
    }
    else if (p_nw_resp->status == ICF_RGM_SSA_NW_FAILURE)
    {
        switch (p_nw_resp->ecode)
        {
            case ICF_RGM_SSA_FAIL_REATTEMPT:
                
#ifdef ICF_TRACE_ENABLE
                if (ICF_RGM_05_DEREGISTERING == p_rgm_context->rgm_state)
                {
                    ICF_RGM_TRACE(p_crm_pdb,
                            ICF_TRACE_DEREG_FAIL_RESP)
                }
                else
                {
                    ICF_RGM_TRACE(p_crm_pdb,
                            ICF_TRACE_REG_FAIL_RESP)            
                }
#endif                 
                ICF_RGM_TRACE(p_crm_pdb,
                        ICF_TRACE_REGISTRATION_FSM_TRIGGERED)
                /* Trigger FSM */
                ret_val = p_rgm_glb_data->icf_rgm_state_machine
                    [p_rgm_context->rgm_state][ICF_RGM_FAILURE_REATTEMPT]
                    (p_crm_pdb);
                break;
                
            case ICF_RGM_SSA_FAIL_NO_REATTEMPT:
                
#ifdef ICF_TRACE_ENABLE
                if (ICF_RGM_05_DEREGISTERING == p_rgm_context->rgm_state)
                {
                    ICF_RGM_TRACE(p_crm_pdb,
                            ICF_TRACE_DEREG_FAIL_NO_REATT_RESP)
                }
                else
                {
                    ICF_RGM_TRACE(p_crm_pdb,
                            ICF_TRACE_REG_FAIL_NO_REATT_RESP)            
                }
#endif

                ICF_RGM_TRACE(p_crm_pdb,
                        ICF_TRACE_REGISTRATION_FSM_TRIGGERED)
                /* Trigger FSM */
                ret_val = p_rgm_glb_data->icf_rgm_state_machine
                    [p_rgm_context->rgm_state][ICF_RGM_FAILURE_NO_REATTEMPT]
                    (p_crm_pdb);
                break;

            case ICF_REGM_SSA_BASIC_AUTH_SCHEME_NOT_SUPPORTED:

                ICF_RGM_TRACE(p_crm_pdb,
                        ICF_TRACE_REGISTRATION_FSM_TRIGGERED)
				
				p_crm_pdb->error_cause = ICF_REGM_SSA_BASIC_AUTH_SCHEME_NOT_SUPPORTED;
                /* Trigger FSM */
                ret_val = p_rgm_glb_data->icf_rgm_state_machine
                    [p_rgm_context->rgm_state][ICF_RGM_FAILURE_NO_REATTEMPT]
                    (p_crm_pdb);
                break;

            case ICF_REGM_SSA_AUTH_FAILURE:

                ICF_RGM_TRACE(p_crm_pdb,
                        ICF_TRACE_REGISTRATION_FSM_TRIGGERED)
				
				p_crm_pdb->error_cause = ICF_REGM_SSA_AUTH_FAILURE;
                /* Trigger FSM */
                ret_val = p_rgm_glb_data->icf_rgm_state_machine
                    [p_rgm_context->rgm_state][ICF_RGM_FAILURE_NO_REATTEMPT]
                    (p_crm_pdb);
                break;
                
            case ICF_RGM_SSA_AUTHENTICATION_REQ:

                ICF_RGM_TRACE(p_crm_pdb,
                        ICF_TRACE_AUTHENTICATION_REQ)                
                
                ICF_RGM_TRACE(p_crm_pdb,
                        ICF_TRACE_REGISTRATION_FSM_TRIGGERED)
                /* Trigger FSM */
                ret_val = p_rgm_glb_data->icf_rgm_state_machine
                    [p_rgm_context->rgm_state][ICF_RGM_AUTHENTICATION_REQ]
                    (p_crm_pdb);
                break;
                
            case ICF_RGM_SSA_DURATION_SHORT:

                ICF_RGM_TRACE(p_crm_pdb,
                        ICF_TRACE_DURATION_SHORT)
                
                ICF_RGM_TRACE(p_crm_pdb,
                        ICF_TRACE_REGISTRATION_FSM_TRIGGERED)

				if (ICF_RGM_SSA_DURATION_FILLED & 
							p_rgm_context->indicator)
				{
					/* Copy the Min-Expires value into the rgm_context */
					p_rgm_context->rgm_duration = 
									p_nw_resp->expires_duration;
				}

                /* Trigger FSM */
                ret_val = p_rgm_glb_data->icf_rgm_state_machine
                    [p_rgm_context->rgm_state][ICF_RGM_DURATION_SHORT]
                    (p_crm_pdb);
                break;

ICF_4_0_CRM_START
			case ICF_RGM_SSA_FAIL_TIMEOUT:
                
#ifdef ICF_TRACE_ENABLE
                if (ICF_RGM_05_DEREGISTERING == 
					p_rgm_context->rgm_state)
                {
                    ICF_RGM_TRACE(p_crm_pdb,
						ICF_TRACE_DEREG_FAIL_RESP)
                }
                else
                {
                    ICF_RGM_TRACE(p_crm_pdb,
						ICF_TRACE_REG_FAIL_RESP)            
                }
#endif                 
                ICF_RGM_TRACE(p_crm_pdb,
					ICF_TRACE_REGISTRATION_FSM_TRIGGERED)
					/* Trigger FSM */
					ret_val = 
					p_rgm_glb_data->icf_rgm_state_machine
					[p_rgm_context->rgm_state]
					[ICF_RGM_FAILURE_TIMEOUT](p_crm_pdb);
                break;
              
           case ICF_REGM_SSA_DNS_FAILURE:

                ICF_RGM_TRACE(p_crm_pdb,
					ICF_TRACE_REGISTRATION_FSM_TRIGGERED)
               
                p_crm_pdb->error_cause = ICF_ERROR_DNS_QUERY_FAILURE;

					/* Trigger FSM */
					ret_val = 
					p_rgm_glb_data->icf_rgm_state_machine
					[p_rgm_context->rgm_state]
					[ICF_RGM_FAILURE_TIMEOUT](p_crm_pdb);
                break;

ICF_4_0_CRM_END

            default:
                /* Raise Alarm */
                ICF_RGM_SET_ECODE(p_crm_pdb,
                        ICF_INVALID_NETWORK_RESPONSE)
                /*ret_val = ICF_FAILURE;*/

#ifdef IMS_CLIENT
ICF_CRM_START
ICF_4_0_CRM_START
ICF_CHECK_IMS_START(p_glb_pdb,ICF_TRUE)
#ifndef ICF_FEATURE_ABANDON/* For ICF 3.0 function not used in ICF 4.0*/
				if (ICF_FAILURE == icf_rgm_default_nw_resp_hdlr(
									p_crm_pdb))
				{
					ret_val = ICF_SUCCESS;
					ICF_PRINT(("\n[ICF][REGM]: Unable to send CFM with failure"));

				}	
#endif /* ICF_FEATURE_ABANDON: For ICF 3.0 function not used in ICF 4.0*/

ICF_CHECK_IMS_END
ICF_4_0_CRM_END
ICF_CRM_END
#endif 				
                break;
        }
                
    }
    else
    {
        ret_val = ICF_FAILURE;
    }

	p_crm_pdb->resp_to_be_sent = ICF_FALSE;/*for CFG to not send response*/

ICF_4_0_CRM_START
	/* Freeing the incoming header list from the rgm_context for all applications */

	for (loop_app = 0; loop_app < p_crm_pdb->p_config_data->max_applications;
							loop_app++)
	{
		if (ICF_TRUE == p_rgm_context->p_app_data_list[loop_app].is_valid)
		{
			p_hdr_list = &(p_rgm_context->p_app_data_list[loop_app].inc_header_list);
			if( p_hdr_list->count)
			{
				/*free the list*/
				icf_cmn_free_header_list(p_crm_pdb->p_glb_pdb,
					p_hdr_list,
					p_crm_pdb->p_ecode);
			}
		}
	}/* for loop on app_data_list ends */

ICF_4_0_CRM_END

#ifdef ICF_NAT_RPORT_SUPPORT
    if(ICF_NULL != p_rport_config_data)
    {
        p_rport_config_data = ICF_NULL;
    }
#endif
#ifdef IMS_CLIENT
ICF_CHECK_IMS_START(p_glb_pdb,ICF_TRUE)
    ret_val = icf_process_buffer(p_crm_pdb);
ICF_CHECK_IMS_END
#endif 

    /* function exit trace */
    ICF_FUNCTION_EXIT(p_crm_pdb->p_internal_msg->msg_hdr.p_glb_pdb)

    ICF_RESET_MODULE(p_crm_pdb->p_internal_msg->msg_hdr.p_glb_pdb)
    
    return ret_val;
}
