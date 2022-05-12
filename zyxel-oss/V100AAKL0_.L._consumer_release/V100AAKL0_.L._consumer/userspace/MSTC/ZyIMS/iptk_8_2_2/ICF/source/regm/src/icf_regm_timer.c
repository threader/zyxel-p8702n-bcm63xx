/*------------------------------------------------------------------------------
 *
 * File name        : icf_regm_timer.c
 *
 * Purpose          : This file contains the timer expiry handler functions.
 *
 * Revision History :
 *
 * Date           Name          Ref#                 Comments
 * --------    ------------     ----                 ---------------
 * 22-Dec-2004  Jalaj Negi     Non Service LLD      Initial
 *
 * 14-Dec-2006  Deepti Goyal   SPR  14063           Set the timer buffer
 *                                                  to NULL after freeing
 *                                                  timer data.            
 * 08-JUN-2007  Alok Tiwari    Rel early 7.0        Code added for Stat collection
 * 
 * 10-Mar-2008  Divij Agarwal  SPR 18122            Support for Async AKA 
 *                                                  negotiation
 * 08-Apr-2008  Tarun Gupta    SPR 18355            Merged Windows Porting Changes    
 * 04-Nov-2008  Tarun Gupta    SPR 19189            Merged Rel 7.1.2 SPR 18880
 * 20-Nov-2008  Rajiv Kumar    SPR 19188            Rel 7.1.2 SPR merging (SPR
 *                                                  19151)  
 * 01-Dec-2008  Rajiv Kumar    SPR 19215            CSR-1-6212448 Merged
 * 04-Mar-2009 Saurabh Gupta   SPR 19349      		Klocworks fix
 * 12-May-2009  Anuradha Gupta SPR 19601            Modified icf_rgm_reg_timer_exp
 *
 * Copyright (c) 2009, Aricent.
 *----------------------------------------------------------------------------*/

#include "icf_dbm_prototypes.h"
#include "icf_regm_prototypes.h"
#include "icf_regm_macro.h"



/****************************************************************************
 * FUNCTION:            icf_rgm_timer_exp
 *
 * DESCRIPTION:         This function is called by the function of configuration
 *		                part when TIMER_EXPIRY event is received from MRM.It 
 *                      checks the type of timer expired and then invokes the
 *                      respective handler.
 *
 *
 * RETURNS:             
 *
 ***************************************************************************/
icf_return_t icf_rgm_timer_exp(
        INOUT icf_crm_pdb_st *p_crm_pdb)
{
	icf_timer_data_st *p_buff = ICF_NULL;
    icf_timer_data_st **p_p_buff = ICF_NULL;
    icf_glb_pdb_st *p_glb_pdb = ICF_NULL;
    icf_result_t ret_val = ICF_SUCCESS;
	icf_error_t                      *p_ecode = ICF_NULL;
    icf_rgm_context_st               *p_rgm_context = ICF_NULL;
    icf_rgm_glb_data_st              *p_rgm_glb_data = ICF_NULL;
    icf_return_t return_val = ICF_SUCCESS;
    
    /* SPR 18880 : Added Null checks */
    if((ICF_NULL == p_crm_pdb->p_internal_msg) ||
       (ICF_NULL == p_crm_pdb->p_internal_msg->msg_hdr.p_glb_pdb))
    {
        ICF_PRINT(((icf_uint8_t *)"[RGM] : p_internal_msg or p_glb_pdb is NULL.\n"))
        return ICF_FAILURE;
    }

    ICF_SET_MODULE(p_crm_pdb->p_internal_msg->msg_hdr.p_glb_pdb,
            ICF_INT_MODULE_REGM)
        
    /* Get the pointer to global PDB */
    p_glb_pdb = p_crm_pdb->p_internal_msg->msg_hdr.p_glb_pdb;
    ICF_ASSERT(
            p_crm_pdb->p_internal_msg->msg_hdr.p_glb_pdb,
            (ICF_NULL != p_glb_pdb))

    /* function entry trace */
    ICF_FUNCTION_ENTER(p_glb_pdb)

    /* Store the ecode pointer locally */
    p_ecode = &(p_crm_pdb->p_internal_msg->msg_hdr.ecode);
    ICF_ASSERT(p_glb_pdb, (ICF_NULL != p_ecode))
    
    /* SPR 18880 */
    ICF_CHECK_IF_ANY_OF_THREE_VAR_IS_NULL(p_glb_pdb,
                              p_glb_pdb->p_recv_msg,
                              p_glb_pdb->p_recv_msg->payload,
                              p_ecode,
                              return_val)
    if(ICF_FAILURE == return_val)
    {
        ret_val = ICF_FAILURE;
        ICF_FUNCTION_EXIT(p_glb_pdb)
        return ICF_FAILURE;
    }

    p_p_buff = (icf_timer_data_st **)(p_glb_pdb->p_recv_msg->payload);

    p_buff = (icf_timer_data_st *)(*p_p_buff);

    ICF_CHECK_IF_VAR_IS_NULL(p_glb_pdb,
                             p_buff,
                             return_val)
    if(ICF_FAILURE == return_val)
    {
        ret_val = ICF_FAILURE;
        ICF_FUNCTION_EXIT(p_glb_pdb)
        return ICF_FAILURE;
    }

    ICF_ASSERT(
            p_crm_pdb->p_internal_msg->msg_hdr.p_glb_pdb,
            (ICF_NULL != p_buff))

	/* get the pointer to CFG data: NULL uptil now  */

	ret_val = icf_dbm_get_module_glb_data(
                p_glb_pdb,
                (icf_int_module_id_t)(ICF_INT_MODULE_CFG),
                (icf_void_t **)(&(p_crm_pdb->p_config_data)),
                p_ecode);   

    ICF_CHECK_IF_VAR_IS_NULL(p_glb_pdb,
                             p_crm_pdb->p_config_data,
                             return_val)
    if(ICF_FAILURE == return_val)
    {
        ret_val = ICF_FAILURE;
        ICF_FUNCTION_EXIT(p_glb_pdb)
        return ICF_FAILURE;
    }

     ICF_ASSERT(p_crm_pdb->p_internal_msg->msg_hdr.p_glb_pdb,
							(ICF_NULL != p_crm_pdb->p_config_data))

    
    /* Now we need to invoke the appropriate fsm handler. For this, first 
     * extract the rgm_context from timer buffer to find current state.
     * Also get the pointer to REGM global data having the function table */

    /* Get the pointer to registration context from received timer data */
  	p_rgm_context = (icf_rgm_context_st *)(p_buff->p_timer_buffer);

    ICF_CHECK_IF_VAR_IS_NULL(p_glb_pdb,
                             p_rgm_context,
                             return_val)
    if(ICF_FAILURE == return_val)
    {
        ret_val = ICF_FAILURE;
        ICF_FUNCTION_EXIT(p_glb_pdb)
        return ICF_FAILURE;
    }

    p_crm_pdb->p_rgm_context = p_rgm_context;
    
    /* Now call this function to do validation of the timer block 
       pointers. We have extracted p_rgm_context from the timer expiry
       that is being received from MRM. This p_rgm_context also contains
       a pointer to the timer block that is running currently.These two
       pointers should point to the same timer block.We should handle the
       timer expiry received from MRM only if both the pointers are equal
       i.e. they point to the same timer block.If the pointers are
       different, we will free the timer block received from MRM. We will
       not free the timer block in p_rgm_context as its timer expiry will
       happen sometime in future. On receiving the same from MRM,the
       pointers would match and thus the timer expiry would be handled.
     */  
    /*Fix for SPR : 19188 (Rel 7.1.2 SPR merging for SPR 19151)*/
    /* SPR 19151: In case timer type is NAT Binding refresh, then need not
     * call the common validate function. 
     * Instead validate it with rport config data
     */
#ifdef ICF_NAT_RPORT_SUPPORT
    if(ICF_BINDING_REFRESH_TIMER_TYPE != p_buff->timer_type)
    {
#endif    
    if(ICF_FAILURE == icf_rgm_is_valid_timer_blk(p_crm_pdb,p_buff))
    {
        ICF_PRINT(((icf_uint8_t *)"[RGM]: Invalid timer expiry received.\n"))

        /* Pointers did not match.Free the timer block received from MRM */
        ret_val = icf_dbm_free_timer_block(p_crm_pdb->p_glb_pdb,
                      (icf_void_t *)(p_buff),p_ecode); 
        if(ICF_SUCCESS == ret_val)
        {
            p_buff = ICF_NULL;
        }
        /* Timer block freeing failure. Raise a alarm and proceed. */
        else
        {
            ICF_PRINT(((icf_uint8_t *)"[RGM]: Failure in freeing timer block.\n"))
        }
        ret_val = ICF_FAILURE;
        ICF_FUNCTION_EXIT(p_crm_pdb->p_glb_pdb)
        return ICF_FAILURE;
    }
    /* Valid timer expiry received. Proceed to handle it */
#ifdef ICF_NAT_RPORT_SUPPORT
    }
    else if(ICF_FAILURE == icf_rgm_is_valid_nat_binding_timer_blk(p_crm_pdb,p_buff))
    {
        ICF_PRINT(((icf_uint8_t *)"[RGM]: Invalid timer expiry received.\n"))

        /* Pointers did not match.Free the timer block received from MRM */
        ret_val = icf_dbm_free_timer_block(p_crm_pdb->p_glb_pdb,
                          (icf_void_t *)(p_buff),p_ecode); 
        if(ICF_SUCCESS == ret_val)
        {
            p_buff = ICF_NULL;
        }
        /* Timer block freeing failure. Raise a alarm and proceed. */
        else
        {
            ICF_PRINT(((icf_uint8_t *)"[RGM]: Failure in freeing timer block.\n"))
        }
        ret_val = ICF_FAILURE;
        ICF_FUNCTION_EXIT(p_crm_pdb->p_glb_pdb)
        return ICF_FAILURE;
    }
#endif
    if (ICF_TIMER_TYPE_PROBATION ==
                p_buff->timer_type)
    {
        /* Probation timer is stared as a separate timer. */
        ret_val = icf_rgm_prbn_timer_exp(p_crm_pdb);
    }
    else
    {
        
        /* Call DBM to get the pointer to RGM global data in p_glb_buff */
        ret_val = icf_dbm_get_module_glb_data(
            p_glb_pdb,
            (icf_int_module_id_t)(ICF_INT_MODULE_REGM),
            (icf_void_t *)&p_rgm_glb_data,
            p_ecode);  

        ICF_CHECK_IF_VAR_IS_NULL(p_glb_pdb,
                                 p_rgm_glb_data,
                                 return_val)
        if(ICF_FAILURE == return_val)
        {
            ret_val = ICF_FAILURE;
            ICF_FUNCTION_EXIT(p_glb_pdb)
            return ICF_FAILURE;
        }
        
        ICF_ASSERT(p_glb_pdb, (ICF_NULL != p_rgm_glb_data))
            
	if ( p_rgm_context->rgm_state < ICF_RGM_MAX_STATES)
	{
		ret_val = p_rgm_glb_data->icf_rgm_state_machine
            [p_rgm_context->rgm_state][ICF_RGM_TIMER_EXPIRY](p_crm_pdb);
	}
    }
    
    /* function exit trace */
    ICF_FUNCTION_EXIT(p_glb_pdb)
    
    ICF_RESET_MODULE(p_glb_pdb)

    return ret_val;

}/* Function icf_rgm_timer_exp() ends here */



/****************************************************************************
 * FUNCTION:            icf_rgm_retry_timer_exp
 *
 * DESCRIPTION:         This function is invoked when guard timer expires.
 *
 *
 * RETURNS:             
 *
 ***************************************************************************/

icf_return_t icf_rgm_retry_timer_exp(
        INOUT icf_crm_pdb_st *p_crm_pdb)
{
	/* Event is RETRY_TIMER_EXPIRY */
	icf_timer_data_st *p_buff_retry_timer;
    icf_timer_data_st **p_p_buff_retry_timer;
	icf_rgm_context_st *p_rgm_context;
    icf_result_t ret_val = ICF_SUCCESS;
    icf_rgm_register_resp_st    register_response;
    
    /* Variables used for Code Area reduction */
    icf_glb_pdb_st *p_glb_pdb;
    icf_error_t  *p_ecode;
    icf_timer_type_t            timer_type = ICF_NULL;
#ifdef ICF_NAT_RPORT_SUPPORT
    icf_line_data_st    *p_line_data = ICF_NULL;
    icf_rport_config_data_st *p_rport_config_data = ICF_NULL;
#endif
   
 
    /* function entry trace */
    ICF_FUNCTION_ENTER(p_crm_pdb->p_internal_msg->msg_hdr.p_glb_pdb) 
        
/*    ICF_RGM_TRACE(p_crm_pdb, ICF_TRACE_RETRY_TIMER_EXPIRY)*/
        
    /* Get the pointer to global PDB */
    /*p_glb_pdb = p_crm_pdb->p_internal_msg->msg_hdr.p_glb_pdb;*/
      ICF_RGM_INITIALIZE_REG_RESP_STRUCT(register_response);

      p_glb_pdb = p_crm_pdb->p_glb_pdb;

    /* Get the pointer to ecode */
    p_ecode = &(p_crm_pdb->p_internal_msg->msg_hdr.ecode);

    /* Typecast the API payload to timer data structure */
    p_p_buff_retry_timer = (icf_timer_data_st **)(p_glb_pdb->p_recv_msg->
            payload);

    p_buff_retry_timer = (icf_timer_data_st *)(*p_p_buff_retry_timer);

    /* Fetch the timer type before freeing the timer buffer */
    timer_type = p_buff_retry_timer->timer_type;

	
    /* Get the pointer to registration context from received timer data */
	p_rgm_context = (icf_rgm_context_st *)(p_buff_retry_timer->
            p_timer_buffer);
    ICF_ASSERT(
            p_crm_pdb->p_internal_msg->msg_hdr.p_glb_pdb,
            (ICF_NULL != p_rgm_context))    

    /* This is a call to DBM to free the retry timer buffer */
    ret_val = icf_dbm_free_timer_block(
            p_glb_pdb,
            (icf_void_t *)(p_buff_retry_timer),
            p_ecode);    
    
    /* Set Timer ID to INVALID */
    p_rgm_context->timer_id = ICF_TIMER_ID_INVALID;
    /* Set pointer to timer as NULL */
    p_rgm_context->p_timer_buff = ICF_NULL;
/*  Fix for spr 18493 */
/*  Fetch the rport_config_data */
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

    if(ICF_BINDING_REFRESH_TIMER_TYPE == timer_type)
    {
       /*  Make the (NAT Timer)p_binding_refresh_timer Null because this timer 
           has been expired */
        p_rport_config_data->p_binding_refresh_timer= ICF_NULL;
        /*  Because in the current state if NAT timer expires we
            are already sending the new register so there is no
            need to send refresh register */ 
        return ICF_SUCCESS;
    }
#endif

    /* Only For Trace Purposes */
    p_crm_pdb->p_rgm_context = p_rgm_context;
    ICF_RGM_TRACE(p_crm_pdb, ICF_TRACE_RETRY_TIMER_EXPIRY)

    ICF_STATS(p_crm_pdb->p_glb_pdb->p_stats_info->stat_register.
                          stats_reg_guard_timer_expire)	
    
    switch (p_rgm_context->rgm_state)
    {
        case ICF_RGM_01_REGISTERING:
        case ICF_RGM_04_AUTHORIZING:

            /* Check whether PA was not indicated */
            if (!(p_rgm_context->indicator & ICF_RGM_PA_INDICATED))
            {
                /* PA was not indicated about registration failure */

                /* Send User Registered indication to PA */
				/* this api is not supported in icf */
				/*
                icf_rgm_send_pa_display_ind(
                        (icf_display_id_t)ICF_DISPLAY_USER_NOT_REGISTERED,
                        p_crm_pdb,
                        p_rgm_context);*/

                /* Set the PA_INDICATED bit */
                p_rgm_context->indicator |= ICF_RGM_PA_INDICATED;
            }
            
            /* Failure is assumed */
            p_rgm_context->rgm_state = ICF_RGM_02_UNREGISTERED;
 
            /*Adding code to send failure to APP and deallocate the context*/
            register_response.options_flag = ICF_NULL;
            register_response.options_flag |= ICF_RGM_RESPONSE_CFM;
                                                                                                 
            register_response.p_crm_pdb = p_crm_pdb;
            register_response.p_rgm_context = p_rgm_context;
                                                                                                 
            register_response.status = ICF_FAILURE;
            /*Send error_cause as TIMER EXPIRY as guard timer expired*/
            register_response.error_cause = ICF_CAUSE_TIMER_EXPIRY;
            register_response.p_request_payload = ICF_NULL;
                                                                                                 
            register_response.request_type = ICF_REQUEST_TYPE_REGISTERING;
                                                                                                 
            p_crm_pdb->resp_to_be_sent = ICF_FALSE;
                                                                                                 
            ret_val = icf_rgm_send_response(p_crm_pdb->p_glb_pdb,
                            &register_response, p_crm_pdb->p_ecode);

            /* Clear registration context */
            ret_val = icf_rgm_clear_reg_context(
                    p_crm_pdb,
                    p_rgm_context);
                
            ret_val = ICF_SUCCESS;
            break;
    
        case ICF_RGM_05_DEREGISTERING:
            /* Failure is assumed */
            if(ICF_RGM_APP_INITIATED_DEREG & p_rgm_context->indicator)
            {
                p_rgm_context->indicator &= ~ICF_RGM_APP_INITIATED_DEREG;
                icf_rgm_form_and_send_dereg_resp(p_crm_pdb,p_rgm_context);
            }
            /* Clear registration context */
            ret_val = icf_rgm_clear_reg_context(
                    p_crm_pdb,
                    p_rgm_context);

            ret_val = ICF_SUCCESS;
            break;
        default:
            /* Event is not expected in rest of the cases */
            ret_val = ICF_FAILURE;
            break;	
    }
        
    /* function exit trace */
    ICF_FUNCTION_EXIT(p_crm_pdb->p_internal_msg->msg_hdr.p_glb_pdb)
        
    return ret_val;
}



/****************************************************************************
 * FUNCTION:            icf_rgm_reg_timer_exp
 *
 * DESCRIPTION:         This function is invoked on expiry of registration timer
 *
 *
 * RETURNS:             
 *
 ***************************************************************************/
icf_return_t icf_rgm_reg_timer_exp(
        INOUT icf_crm_pdb_st *p_crm_pdb)
{
	/* Event is REGISTRATION_TIMER_EXPIRY */ 
	icf_timer_data_st *p_buff_reg_timer = ICF_NULL;
    icf_timer_data_st **p_p_buff_reg_timer;
	icf_rgm_context_st *p_rgm_context = ICF_NULL;
    icf_result_t ret_val = ICF_SUCCESS;
    icf_glb_pdb_st *p_glb_pdb = ICF_NULL;
    icf_error_t  *p_ecode = ICF_NULL;
    icf_timer_type_t		timer_type = ICF_NULL;
#ifdef ICF_NAT_RPORT_SUPPORT
    icf_line_data_st	*p_line_data = ICF_NULL;
    icf_rport_config_data_st *p_rport_config_data = ICF_NULL;
#endif
    
    /* function entry trace */
    ICF_FUNCTION_ENTER(p_crm_pdb->p_internal_msg->msg_hdr.p_glb_pdb) 
    
/*    ICF_RGM_TRACE(p_crm_pdb, ICF_TRACE_REG_TIMER_EXPIRY)*/
    ICF_RGM_APP_TRACE(p_crm_pdb, ICF_TRACE_REG_TIMER_EXPIRE)
        
    /* Get the pointer to global PDB */
    p_glb_pdb = p_crm_pdb->p_internal_msg->msg_hdr.p_glb_pdb;

    /* Get the pointer to ecode */
    p_ecode = &(p_crm_pdb->p_internal_msg->msg_hdr.ecode);

    /* Typecast the API payload to timer data structure */
    p_p_buff_reg_timer = (icf_timer_data_st **)
        (p_glb_pdb->p_recv_msg->payload);

    p_buff_reg_timer = (icf_timer_data_st *)(*p_p_buff_reg_timer);
	
    /* Get the pointer to registration context from received timer data */
	p_rgm_context = (icf_rgm_context_st *)(p_buff_reg_timer->
            p_timer_buffer);
    ICF_ASSERT(
            p_crm_pdb->p_internal_msg->msg_hdr.p_glb_pdb,
            (ICF_NULL != p_rgm_context))
        
            
    /* Fetch the timer type before freeing the timer buffer */
    timer_type = p_buff_reg_timer->timer_type;
    

	/* debug traces */
    ICF_PRINT(((icf_uint8_t *)"\n[REGM]: Recd expiry of timer-type = %d of timer-id = %ld\n"\
		, timer_type, p_rgm_context->timer_id));

    /* This is a call to DBM to free the registration timer buffer */
    ret_val = icf_dbm_free_timer_block(
            p_glb_pdb,
            (icf_void_t *)(p_buff_reg_timer),
            p_ecode);
	/* Fix for FMW */
    /* icf_port_memset((icf_void_t*)p_buff_reg_timer,ICF_NULL,(icf_uint32_t)\
            ICF_PORT_SIZEOF(icf_timer_data_st)); */
    p_buff_reg_timer = ICF_NULL;

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

    if(ICF_BINDING_REFRESH_TIMER_TYPE == timer_type)
    {
        p_rport_config_data->p_binding_refresh_timer= ICF_NULL;
        /*  If the timer that get expired is Nat Timer then
            the Expires timer was not getting freed. 
            (Fix for spr 18493)
         */
         icf_rgm_stop_timer(p_crm_pdb);
    }
#endif
    /* Set Timer ID to INVALID */
    p_rgm_context->timer_id = ICF_TIMER_ID_INVALID;
    /* Set pointer to timer as NULL */
    p_rgm_context->p_timer_buff = ICF_NULL;
        
    /* Only For Trace Purposes */
    p_crm_pdb->p_rgm_context = p_rgm_context;
	/* CSR 1-6212448: SPR 18873:If Valid line-id is received then 
	 * Update the line id as received in payload of register request.*/
	if(ICF_INVALID_LINE_ID != p_rgm_context->line_id)
	{
		p_crm_pdb->line_id = p_rgm_context->line_id;
	}    
    ICF_RGM_TRACE(p_crm_pdb, ICF_TRACE_REG_TIMER_EXPIRY)
    
    switch (p_rgm_context->rgm_state)
    {
        case ICF_RGM_03_REGISTERED:

#ifdef ICF_IPSEC_ENABLED
            
        /* This can be expiry of registration timer, or SA delete timer, or SA 
         * modify timer */
        
        if (ICF_TRUE == p_crm_pdb->p_config_data->ipsec_enabled)
        {
            if (ICF_TIMER_TYPE_RGM_SA_DELETE == timer_type)
            {
                ret_val = icf_rgm_03_sa_delete_timer_exp(p_crm_pdb);
            }
#ifdef ICF_SA_LIFETIME_MOD_SUPPORT
            else if (ICF_TIMER_TYPE_RGM_SA_MODIFY == timer_type)
            {
                ret_val = icf_rgm_03_sa_modify_timer_exp(p_crm_pdb);
            }
#endif /* ICF_SA_LIFETIME_MOD_SUPPORT */
            else if (ICF_TIMER_TYPE_RGM_REGISTRATION == timer_type)
            {
                /* This is internally generated registration */
                p_rgm_context->indicator |= ICF_RGM_INTERNAL_GENERATED_REG;
                
                /* current implementation */
                ret_val = 
                    icf_rgm_register_user(p_crm_pdb, p_rgm_context);
            }
        } /* End if(ipsec_enabled) */
        else
        {
#endif /* ICF_IPSEC_ENABLED */
           /* Added for trace functionality */
           p_crm_pdb->p_rgm_context = p_rgm_context;
           ICF_RGM_APP_TRACE(p_crm_pdb, ICF_TRACE_REREG_REQ)

#ifdef ICF_NAT_RPORT_SUPPORT
           /* SPR 19601 : In case the timer expiry is NAT binding timer
            * expiry then set the indicator of internal generated
            * registration request in the reg context
            */     
           if(ICF_BINDING_REFRESH_TIMER_TYPE == timer_type)
           {
               p_rgm_context->indicator |= ICF_RGM_INTERNAL_GENERATED_REG;
           } 
#endif   

            /*Registration is to be retried */
            ret_val = icf_rgm_register_user(
                    p_crm_pdb, 
                    p_rgm_context);
        if(ret_val  != ICF_FAILURE)
        {
             ICF_STATS(p_crm_pdb->p_glb_pdb->p_stats_info->stat_register.stats_rereg_done)       
        }

#ifdef ICF_LOAD_STAT
        if(ICF_SUCCESS == ret_val)
        {
            p_crm_pdb->p_glb_pdb->load_data.register_refresh++;
        }
#endif
#ifdef ICF_IPSEC_ENABLED
        } /* End else(ipsec_enabled) */
#endif /* ICF_IPSEC_ENABLED */

            break;

        default:
            /* Event is not expected in rest of the cases */
            ret_val = ICF_FAILURE;
            break;	
    }
    /* function exit trace */
    ICF_FUNCTION_EXIT(p_crm_pdb->p_internal_msg->msg_hdr.p_glb_pdb)
        
    return ret_val;
}


/*#ifndef ICF_FEATURE_ABANDON For ICF 3.0 function not used in ICF 4.0*/
/****************************************************************************
 * FUNCTION:            icf_rgm_reattempt_timer_exp
 *
 * DESCRIPTION:         This function is invoked on expiry of registration
 *                      reattempt timer.
 *
 *
 * RETURNS:             
 *
 ***************************************************************************/

icf_return_t icf_rgm_reattempt_timer_exp(
        INOUT icf_crm_pdb_st *p_crm_pdb)
{
	/* Event is REGISTRATION_REATTEMPT_TIMER_EXPIRY */
	icf_timer_data_st *p_buff_reattempt_timer;
    icf_timer_data_st **p_p_buff_reattempt_timer;
	icf_rgm_context_st *p_rgm_context;
    icf_result_t ret_val = ICF_SUCCESS;
    icf_glb_pdb_st *p_glb_pdb;
    icf_error_t  *p_ecode;

    
    /* function entry trace */
    ICF_FUNCTION_ENTER(p_crm_pdb->p_internal_msg->msg_hdr.p_glb_pdb) 
      
/*    ICF_RGM_TRACE(p_crm_pdb, ICF_TRACE_REG_REATT_TIMER_EXPIRY)*/
        
    /* Get the pointer to global PDB */
    p_glb_pdb = p_crm_pdb->p_internal_msg->msg_hdr.p_glb_pdb;

    /* Get the pointer to ecode */
    p_ecode = &(p_crm_pdb->p_internal_msg->msg_hdr.ecode);

    /* Typecast the API payload to timer data structure */
    p_p_buff_reattempt_timer = (icf_timer_data_st **)
        (p_glb_pdb->p_recv_msg->payload);

    p_buff_reattempt_timer = (icf_timer_data_st *)
        (*p_p_buff_reattempt_timer);
	
    /* Get the pointer to registration context from received timer data */
	p_rgm_context = (icf_rgm_context_st *)(p_buff_reattempt_timer->
            p_timer_buffer);
    ICF_ASSERT(
            p_crm_pdb->p_internal_msg->msg_hdr.p_glb_pdb,
            (ICF_NULL != p_rgm_context))
  p_crm_pdb->p_rgm_context = p_rgm_context;

    if ((ICF_RGM_02_UNREGISTERED != p_rgm_context->rgm_state) || 
        (ICF_TRUE != p_rgm_context->auto_reg_flag) ||
        (ICF_NULL == (ICF_RGM_UNREG_ON_REATTEMPT & 
                        p_rgm_context->indicator)))
    {
    /* This function should not have been hit 
        if the state was not UNREGISTERED or auto_reg was not TRUE */
        /* This is a call to DBM to free the registration reattempt timer buffer */
        ret_val = icf_dbm_free_timer_block(
            p_glb_pdb,
            (icf_void_t *)(p_buff_reattempt_timer),
            p_ecode);    
        
        
        /* Set Timer ID to INVALID */
        p_rgm_context->timer_id = ICF_TIMER_ID_INVALID;
        /* Set pointer to timer as NULL */
        p_rgm_context->p_timer_buff = ICF_NULL;

        return ICF_FAILURE;
    }
    
    /* This is a call to DBM to free the registration reattempt timer buffer */
    ret_val = icf_dbm_free_timer_block(
            p_glb_pdb,
            (icf_void_t *)(p_buff_reattempt_timer),
            p_ecode);    
    
    
    /* Set Timer ID to INVALID */
    p_rgm_context->timer_id = ICF_TIMER_ID_INVALID;
    /* Set pointer to timer as NULL */
    p_rgm_context->p_timer_buff = ICF_NULL;


    /* Only For Trace Purposes */    
    ICF_RGM_TRACE(p_crm_pdb, ICF_TRACE_REG_REATT_TIMER_EXPIRY)
        
    
    /* Init the retry_count to 0 and auth_retry_count to 0 */
    p_rgm_context->auth_retry_count = ICF_NULL;
#ifdef IMS_CLIENT
ICF_CHECK_IMS_START(p_crm_pdb->p_glb_pdb,ICF_TRUE)
    p_rgm_context->retry_count = ICF_NULL;
ICF_CHECK_IMS_END
#endif
    /*Registration is to be retried */
    ret_val = icf_rgm_register_user(
        p_crm_pdb, 
        p_rgm_context);
   

	

    /* function exit trace */
    ICF_FUNCTION_EXIT(p_crm_pdb->p_internal_msg->msg_hdr.p_glb_pdb)
        
    return ret_val;
}
/*#endif  ICF_FEATURE_ABANDON*/
/*		 For ICF 3.0 feature not used in ICF 4.0*/


/****************************************************************************
 * FUNCTION:            icf_rgm_prbn_timer_exp
 *
 * DESCRIPTION:         This function is invoked on expiry of probation
 *                      timer which was started when some contact with
 *                      with event=probation was received in NOTIFY
 *
 * RETURNS:             
 *
 ***************************************************************************/
icf_return_t icf_rgm_prbn_timer_exp(
        INOUT icf_crm_pdb_st *p_crm_pdb)
{
    icf_return_t                ret_val = ICF_SUCCESS;
    icf_rgm_context_st          *p_rgm_context = ICF_NULL;
    	/* Event is PRBN_TIMER_EXPIRY */
	icf_timer_data_st *p_buff_prbn_timer;
    icf_timer_data_st **p_p_buff_prbn_timer;
	
       
    /* Variables used for Code Area reduction */
    icf_glb_pdb_st *p_glb_pdb;
    icf_error_t  *p_ecode;
    

    /* function entry trace */
    ICF_FUNCTION_ENTER(p_crm_pdb->p_glb_pdb) 
     

     /* Get the pointer to global PDB */
    p_glb_pdb = p_crm_pdb->p_internal_msg->msg_hdr.p_glb_pdb;

    /* Get the pointer to ecode */
    p_ecode = &(p_crm_pdb->p_internal_msg->msg_hdr.ecode);

    /* Typecast the API payload to timer data structure */
    p_p_buff_prbn_timer = (icf_timer_data_st **)(p_glb_pdb->p_recv_msg->
            payload);

    p_buff_prbn_timer = (icf_timer_data_st *)(*p_p_buff_prbn_timer);
	
    /* Get the pointer to registration context from received timer data */
	p_rgm_context = (icf_rgm_context_st *)(p_buff_prbn_timer->
            p_timer_buffer);
    ICF_ASSERT(
            p_crm_pdb->p_internal_msg->msg_hdr.p_glb_pdb,
            (ICF_NULL != p_rgm_context))    

    /* This is a call to DBM to free the retry timer buffer */
    ret_val = icf_dbm_free_timer_block(
            p_glb_pdb,
            (icf_void_t *)(p_buff_prbn_timer),
            p_ecode);    
    
    /* Set Timer ID to INVALID */
    p_rgm_context->prbn_timer_id = ICF_TIMER_ID_INVALID;
    /* Set pointer to timer as NULL */
    p_rgm_context->p_prbn_timer_buff = ICF_NULL;

    p_crm_pdb->p_rgm_context = p_rgm_context;
    
    /* reset the indicator bit for this value */
    if (ICF_RGM_NFY_ON_PROBATION &
            p_rgm_context->indicator)
    {
        p_rgm_context->indicator &=
                    ~ICF_RGM_NFY_ON_PROBATION;
    }  


    /* Switch on the auto_reg_flag if auto_register is ON */
    if ((p_crm_pdb->p_config_data->auto_register) &&
                (ICF_AUTO_REG_PROBATION ==
                    p_rgm_context->event_auto_reg))

    {
        p_rgm_context->auto_reg_flag =
                        ICF_TRUE;
    }

    /* If any of the ING states then, set indicator with
     * a value to send REGISTER on receipt of final success response */
    if (ICF_RGM_04_AUTHORIZING == p_rgm_context->rgm_state ||
            ICF_RGM_01_REGISTERING == p_rgm_context->rgm_state ||
            ICF_RGM_05_DEREGISTERING == p_rgm_context->rgm_state)
    {
        p_rgm_context->indicator |= 
                            ICF_RGM_REFRESH_REGISTER_ON_PRBN;            
    }
    else if ((ICF_RGM_02_UNREGISTERED == p_rgm_context->rgm_state) &&
                (ICF_IS_BIT_SET(p_rgm_context->indicator,
                    ICF_RGM_NW_DEACTIVATED)))
    {/* Incase timer is expired but the network is deactivated */
        ret_val = ICF_SUCCESS;

    }
    else
    {/* Incase of REGISTERED or UNREGISTERED state with On_Probation */

        p_rgm_context->indicator |=
                    ICF_RGM_REFRESH_REGISTER_ON_PRBN;
        ret_val = icf_rgm_refresh_register(
                                        p_crm_pdb);

        /*if (ICF_FAILURE == icf_rgm_save_smallest_dur(
                p_crm_pdb->p_glb_pdb,
                p_rgm_context,p_crm_pdb->p_ecode))
        {
            ret_val = ICF_FAILURE;
        }
        else if (ICF_NULL !=
                        p_rgm_context->rgm_duration)
        {
             send register 
            ret_val = icf_rgm_register_user( p_crm_pdb,p_rgm_context);
        }       */
        
    }
    
    ICF_FUNCTION_EXIT(p_crm_pdb->p_glb_pdb)
    return ret_val;    
    

}/* Function icf_rgm_prbn_timer_exp() ends here */

/*Fix for SPR : 19188 (Rel 7.1.2 SPR merging for SPR 19151)*/
#ifdef ICF_NAT_RPORT_SUPPORT
/****************************************************************************
 * FUNCTION:            icf_rgm_is_valid_nat_binding_timer_blk 
 * DESCRIPTION:         This function validates the timer expiry event received. 
 * RETURNS:             ICF_SUCCESS if timer expiry is valid, ICF_FAILURE otherwise.
 ***************************************************************************/
icf_return_t icf_rgm_is_valid_nat_binding_timer_blk(
        IN icf_crm_pdb_st *p_crm_pdb,
        IN icf_void_t     *p_buff)
{
    icf_return_t ret_val = ICF_SUCCESS;
    icf_line_data_st	*p_line_data = ICF_NULL;
    icf_rport_config_data_st *p_rport_config_data = ICF_NULL;
    icf_rgm_context_st *p_rgm_context =  ICF_NULL;

    /* SPR 18880 : Added Null checks */
    if(ICF_NULL == p_crm_pdb->p_glb_pdb)
    {
        ICF_PRINT(((icf_uint8_t *)"[RGM] : p_glb_pdb is NULL.\n"))
        return ICF_FAILURE;
    }

    ICF_FUNCTION_ENTER(p_crm_pdb->p_glb_pdb) /* function entry trace */
    
    ICF_CHECK_IF_ANY_OF_THREE_VAR_IS_NULL(p_crm_pdb->p_glb_pdb,
                                        p_crm_pdb->p_config_data,
                                        p_buff,
                                        ((icf_timer_data_st *)p_buff)->p_timer_buffer,
                                        ret_val)
    if(ICF_FAILURE == ret_val)
    {
        ICF_FUNCTION_EXIT(p_crm_pdb->p_glb_pdb) /* function exit trace */
        return ICF_FAILURE;
    }

    p_rgm_context = ((icf_timer_data_st *)p_buff)->p_timer_buffer;

    /* compare the timer buffer as present in the RPORT config data */
    /* extract the rport config data */
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

    ICF_CHECK_IF_VAR_IS_NULL(p_crm_pdb->p_glb_pdb,
                             p_rport_config_data,
                             ret_val)
    if(ICF_FAILURE == ret_val)
    {
        ICF_FUNCTION_EXIT(p_crm_pdb->p_glb_pdb) /* function exit trace */
        return ICF_FAILURE;
    } 

    /* Now check for the equality of the two pointers */
    if(p_buff != p_rport_config_data->p_binding_refresh_timer)    
    {
        /* pointers are different. */
        ICF_PRINT(((icf_uint8_t *)"[RGM] : Invalid timer block received.\n"))
        ret_val = ICF_FAILURE;
    }
    /* Now check the validity of the timer ID */
    else if(((icf_timer_data_st *)p_buff)->timer_id != 
            p_rport_config_data->binding_refresh_timer_id)
    {
        /* Invalid Timer ID received */
        ICF_PRINT(((icf_uint8_t *)"[RGM] : Invalid timer ID received.\n"))
        ret_val = ICF_FAILURE;
    }
    
    ICF_FUNCTION_EXIT(p_crm_pdb->p_glb_pdb) /* function exit trace */
    return ret_val;
}
#endif

