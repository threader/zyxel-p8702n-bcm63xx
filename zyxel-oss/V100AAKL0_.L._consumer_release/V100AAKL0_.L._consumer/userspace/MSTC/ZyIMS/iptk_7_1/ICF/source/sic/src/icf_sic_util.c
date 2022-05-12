/******************************************************************************
 *
 * FILE NAME:   icf_sic_util.c
 *
 * DESCRIPTION: This file contains the utility functions of SIC.
 *
 * REVISION HISTORY:
 *    Date            Name            Reference       Comments
 *  11-Aug-2005     Syed Ali Rizvi       LLD          Initial
 *
 *  31-Jan-2006     Jyoti Narula      SPR 9079 Fix    Handler for forceful
 *                                                    clearing of all SIC
 *                                                    contexts.
 * 02-Apr-2006	Puneet Sharma	ICF5.0 LLD	 changes done in the comments 
 *											 Multiple Public Users ICF5.0
 * 11-Sept-2007 Abhishek Dhammawat SPR16315 Modified icf_sic_send_subs_state_to_app
 *
 * 16-Jan-2008      Alok Tiwari      SPR-17945        Application level trace
 *                                                    added for 
 *                                                    PUBLISH/MESSAGE support
 * 23-Mar-2010  Rohit Aggarwal   SPR 20343  Event name in subscription ind
 * 09-Apr-2010   Alok Tiwari     SPR 20362       Modified Function 
 *                                               icf_sic_dealloc_context
 *
 ******************************************************************************/

#include "icf_sic_prototypes.h"
#include "icf_port_defs.h"
#include "icf_ecode.h"

#ifdef ICF_PORT_WINDOWS
#pragma warning ( disable : 4702 )
#endif

icf_uint8_t g_stop_timer;

/*******************************************************************************
 *
 * FUNCTION:        icf_sic_populate_context
 *
 * DESCRIPTION:     This function populates the data initially in the SIC ctx
 *                  when DBM allocates a new SIC ctx.
 *
 ******************************************************************************/
icf_return_t icf_sic_populate_context(
        INOUT icf_sic_pdb_st  *p_sic_pdb)
{
    icf_return_t     ret_val = ICF_SUCCESS;
    
    ICF_FUNCTION_ENTER(p_sic_pdb->p_glb_pdb)
    
        /* Allocate memory to the user address pointer */
    ICF_MEMGET(
                p_sic_pdb->p_glb_pdb,
                sizeof(icf_address_st),
                ICF_MEM_COMMON,
                p_sic_pdb->p_sic_ctx->p_user_address,
                ICF_RET_ON_FAILURE,
                p_sic_pdb->p_ecode,
                ret_val)  
               
	if(ICF_FAILURE == ret_val)
	{
		ICF_ALARM(((icf_uint8_t *)"[SIC]: Memory Failure"));
		p_sic_pdb->p_sic_ctx->error_code = ICF_ERROR_MEM_ALLOC_FAILURE;
	} 

    ICF_FUNCTION_EXIT(p_sic_pdb->p_glb_pdb)
    return ret_val;
    
}/* End function */

/*******************************************************************************
 *
 * FUNCTION:        icf_sic_send_options_req
 *
 * DESCRIPTION:     This function sneds the OPTIONS Request to SSA and 
 *
 ******************************************************************************/
icf_return_t icf_sic_send_options_req(
        INOUT icf_sic_pdb_st      *p_sic_pdb)
{
    icf_return_t     ret_val = ICF_SUCCESS;
    icf_nw_otg_options_req_st   opt_req;
    
    ICF_FUNCTION_ENTER(p_sic_pdb->p_glb_pdb)
    
           
    /* Populate the interface structure and send the data */
    opt_req.p_sic_ctx = p_sic_pdb->p_sic_ctx;
    opt_req.is_option_in_dlg = p_sic_pdb->is_option_in_dlg;
    
    p_sic_pdb->p_internal_msg->msg_hdr.msg_id = ICF_NW_OTG_OPTIONS_REQ;
    p_sic_pdb->p_internal_msg->p_msg_data = (icf_uint8_t *)&opt_req;
    
    /*ICF_SIC_TRACE(p_sic_pdb,ICF_TRACE_SIC_OPT_REQ_TO_SSA)*/
    /*ICF_SIC_TRACE(p_sic_pdb,ICF_TRACE_SIC_REQ_URI_SEND)*/

    ret_val = icf_ssa_process_mesg(p_sic_pdb->p_internal_msg);
    
	if(ICF_FAILURE == ret_val)
	{
		p_sic_pdb->p_sic_ctx->error_code = p_sic_pdb->p_internal_msg->msg_hdr.ecode;
	}
#ifdef ICF_LOAD_STAT
/*    else
    {
        p_sic_pdb->p_glb_pdb->load_data.otg_options_attempts++;
    }*/
#endif
    
    ICF_FUNCTION_EXIT(p_sic_pdb->p_glb_pdb)
    return ret_val;        
}

/*******************************************************************************
 *
 * FUNCTION:        icf_sic_send_publish_req
 *
 * DESCRIPTION:     This function sneds the PUBLISH Request to SSA and 
 *
 ******************************************************************************/
icf_return_t icf_sic_send_publish_req(
        INOUT icf_sic_pdb_st      *p_sic_pdb)
{
    icf_return_t     ret_val = ICF_SUCCESS;
    icf_nw_otg_publish_req_st   pub_req;
    
    ICF_FUNCTION_ENTER(p_sic_pdb->p_glb_pdb)
    
           
    /* Populate the interface structure and send the data */
    pub_req.p_sic_ctx = p_sic_pdb->p_sic_ctx;
    
    p_sic_pdb->p_internal_msg->msg_hdr.msg_id = ICF_NW_OTG_PUBLISH_REQ;
    p_sic_pdb->p_internal_msg->p_msg_data = (icf_uint8_t *)&pub_req;
    
    /*ICF_SIC_TRACE(p_sic_pdb,ICF_TRACE_SIC_PUB_REQ_TO_SSA)*/
    /*ICF_SIC_TRACE(p_sic_pdb,ICF_TRACE_SIC_REQ_URI_SEND)*/
    ret_val = icf_ssa_process_mesg(p_sic_pdb->p_internal_msg);

	if(ICF_FAILURE == ret_val)
	{
		p_sic_pdb->p_sic_ctx->error_code = 
			p_sic_pdb->p_internal_msg->msg_hdr.ecode;
	}
        
    ICF_FUNCTION_EXIT(p_sic_pdb->p_glb_pdb)
    return ret_val;        
}

/*******************************************************************************
 *
 * FUNCTION:        icf_sic_send_message_req
 *
 * DESCRIPTION:     This function sneds the MESSAGE Request to SSA and 
 *
 ******************************************************************************/
icf_return_t icf_sic_send_message_req(
        INOUT icf_sic_pdb_st      *p_sic_pdb)
{
    icf_return_t     ret_val = ICF_SUCCESS;
    icf_nw_otg_msg_req_st   msg_req;
    
    ICF_FUNCTION_ENTER(p_sic_pdb->p_glb_pdb)
    
           
    /* Populate the interface structure and send the data */
    msg_req.p_sic_ctx = p_sic_pdb->p_sic_ctx;
    
    p_sic_pdb->p_internal_msg->msg_hdr.msg_id = ICF_NW_OTG_MESSAGE_REQ;
    p_sic_pdb->p_internal_msg->p_msg_data = (icf_uint8_t *)&msg_req;
    
    /*ICF_SIC_TRACE(p_sic_pdb,ICF_TRACE_SIC_MSG_REQ_TO_SSA)*/
    /*ICF_SIC_TRACE(p_sic_pdb,ICF_TRACE_SIC_REQ_URI_SEND)*/

    ret_val = icf_ssa_process_mesg(p_sic_pdb->p_internal_msg);

	if(ICF_FAILURE == ret_val)
	{
		p_sic_pdb->p_sic_ctx->error_code = 
			p_sic_pdb->p_internal_msg->msg_hdr.ecode;
	}
	
    ICF_FUNCTION_EXIT(p_sic_pdb->p_glb_pdb)
    /* Always success */        
    return ret_val;        
}

/*******************************************************************************
 *
 * FUNCTION:        icf_sic_send_refer_req
 *
 * DESCRIPTION:     This function sneds the REFER Request to SSA and 
 *
 ******************************************************************************/
icf_return_t icf_sic_send_refer_req(
        INOUT icf_sic_pdb_st      *p_sic_pdb)
{
    icf_return_t     ret_val = ICF_SUCCESS;
    icf_nw_otg_refer_req_st   refer_req;
	icf_sic_refer_status_st		resp_msg;
	icf_return_t		dummy_retval= ICF_SUCCESS;	
    
    ICF_FUNCTION_ENTER(p_sic_pdb->p_glb_pdb)
    
           
    /* Populate the interface structure and send the data */
    refer_req.bitmask = ICF_NULL;
    refer_req.p_referred_to = ICF_NULL;
    refer_req.p_sic_ctx = p_sic_pdb->p_sic_ctx;
    
    p_sic_pdb->p_internal_msg->msg_hdr.msg_id = ICF_NW_OTG_REFER;
    p_sic_pdb->p_internal_msg->p_msg_data = (icf_uint8_t *)&refer_req;
    
    /*ICF_SIC_TRACE(p_sic_pdb,ICF_TRACE_SIC_REF_REQ_TO_SSA)*/
    /*ICF_SIC_TRACE(p_sic_pdb,ICF_TRACE_SIC_REQ_URI_SEND)*/

	p_sic_pdb->p_internal_msg->msg_hdr.ecode = ICF_ECODE_NO_ERROR;
    ret_val = icf_ssa_process_mesg(p_sic_pdb->p_internal_msg);

	if(ICF_FAILURE == ret_val)
	{
		p_sic_pdb->p_sic_ctx->error_code = 
			p_sic_pdb->p_internal_msg->msg_hdr.ecode;	
	}
	
    if ( ret_val == ICF_FAILURE && 
	ICF_ECODE_REFER_PENDING == p_sic_pdb->p_internal_msg->msg_hdr.ecode)
    {	
	p_sic_pdb->p_internal_msg->msg_hdr.msg_id = ICF_SIC_REFER_STATUS;
	resp_msg.resp_code = 491;
	resp_msg.p_key = p_sic_pdb->p_sic_ctx->p_app_ctx;
	resp_msg.line_id = p_sic_pdb->p_sic_ctx->line_id;
	resp_msg.call_id = p_sic_pdb->p_sic_ctx->call_id;
	resp_msg.app_id = p_sic_pdb->p_sic_ctx->app_id;
	resp_msg.header_list.count = 0;
	p_sic_pdb->p_internal_msg->p_msg_data = (icf_uint8_t*)&resp_msg;
	
	ret_val = icf_adm_map_sic_event_to_ext_event(p_sic_pdb);
	
	if ( ICF_SUCCESS == ret_val)
	{
		ret_val = icf_adm_process_mesg(p_sic_pdb->p_internal_msg);

		ICF_MEMFREE(p_sic_pdb->p_glb_pdb, 
			p_sic_pdb->p_internal_msg->p_msg_data, ICF_MEM_COMMON,
			p_sic_pdb->p_ecode, dummy_retval)

	}
	
    }
    
    ICF_FUNCTION_EXIT(p_sic_pdb->p_glb_pdb)
    return ret_val;        
}
/*******************************************************************************
 *
 * FUNCTION:        icf_sic_send_subs_req
 *
 * DESCRIPTION:     This function sneds the Subscribe Request to SSA and 
 *                  handles the failure to send if to be handled.
 *
 ******************************************************************************/
icf_return_t icf_sic_send_subs_req(
        INOUT icf_sic_pdb_st      *p_sic_pdb)
{
    icf_return_t     ret_val = ICF_SUCCESS;
    icf_nw_otg_subs_req_st   subs_req;
    
    ICF_FUNCTION_ENTER(p_sic_pdb->p_glb_pdb)
  
    subs_req.p_sic_ctx = p_sic_pdb->p_sic_ctx;
    
       
    /* Populate the interface structure and send the data */
    subs_req.p_sic_ctx = p_sic_pdb->p_sic_ctx;
    
    p_sic_pdb->p_internal_msg->msg_hdr.msg_id = ICF_NW_OTG_SUBSCRIBE_REQ;
    p_sic_pdb->p_internal_msg->p_msg_data = (icf_uint8_t *)&subs_req;
    
    /*ICF_SIC_TRACE(p_sic_pdb,ICF_TRACE_SIC_SUBS_REQ_TO_SSA)*/
    /*ICF_SIC_TRACE(p_sic_pdb,ICF_TRACE_SIC_REQ_URI_SEND)*/

    ret_val = icf_ssa_process_mesg(p_sic_pdb->p_internal_msg);

	if (ICF_SUCCESS == ret_val)
    {
        ICF_SIC_SET_STATE(p_sic_pdb,ICF_SIC_01_SUBSCRIBING)
        ICF_SIC_APP_TRACE(p_sic_pdb, ICF_TRACE_SUBSCRIPTION_REQ)
    }
    else
    {
		/* Stop Subscription Response timer */ 
        icf_sic_stop_curr_timer(p_sic_pdb);

        ICF_SIC_SET_CURR_STATUS(
            p_sic_pdb,
            ICF_SUBS_FAIL_NO_RETRY)

		p_sic_pdb->p_sic_ctx->error_code = 
			p_sic_pdb->p_internal_msg->msg_hdr.ecode;
                    
        ret_val = icf_sic_send_subs_state_to_app(p_sic_pdb);

        /* icf_sic_send_delink_to_ssa(p_sic_pdb,
                    (icf_boolean_t)ICF_TRUE); */

        icf_sic_dealloc_context(p_sic_pdb);  
    }

    ICF_FUNCTION_EXIT(p_sic_pdb->p_glb_pdb)
    return ret_val;        
}

/*******************************************************************************
 *
 * FUNCTION:        icf_sic_send_unsub_req
 *
 * DESCRIPTION:     This function sneds the Unsubscribe Request to SSA and 
 *                  handles the failure to send if to be handled.
 *
 ******************************************************************************/
icf_return_t icf_sic_send_unsub_req(
        INOUT icf_sic_pdb_st      *p_sic_pdb)
{
    icf_return_t     ret_val = ICF_SUCCESS;
    icf_nw_otg_subs_req_st   unsubs_req;
    
    ICF_FUNCTION_ENTER(p_sic_pdb->p_glb_pdb)
  
    /* Set duration to zero */ 
    p_sic_pdb->p_sic_ctx->duration = 0;
    p_sic_pdb->p_sic_ctx->app_requested_duration = 0;
    
    /* Populate the interface structure and send the data */
    unsubs_req.p_sic_ctx = p_sic_pdb->p_sic_ctx;
    
    p_sic_pdb->p_internal_msg->msg_hdr.msg_id = ICF_NW_OTG_SUBSCRIBE_REQ;
    p_sic_pdb->p_internal_msg->p_msg_data = (icf_uint8_t *)&unsubs_req;
    
    /*ICF_SIC_TRACE(p_sic_pdb,ICF_TRACE_SIC_SUBS_REQ_TO_SSA)*/
    /*ICF_SIC_TRACE(p_sic_pdb,ICF_TRACE_SIC_REQ_URI_SEND)*/
    ret_val = icf_ssa_process_mesg(p_sic_pdb->p_internal_msg);

	if(ICF_FAILURE == ret_val)
	{
		p_sic_pdb->p_sic_ctx->error_code = 
			p_sic_pdb->p_internal_msg->msg_hdr.ecode;
	}
     ICF_SIC_APP_TRACE(p_sic_pdb, ICF_TRACE_UNSUBS_REQ)

    /* if this is not a best effort unsub request then we are concerned 
	 * the ret_val from SSA */
    if (ICF_FALSE == p_sic_pdb->best_effort_unsub)
    {
        /* If SSA returns success then go to state 3 */
        if (ICF_SUCCESS == ret_val)
        {
            ICF_SIC_SET_STATE(p_sic_pdb,ICF_SIC_03_UNSUBSCRIBING)     
        }
        /* If SSA returns failure then */
        else
        {
            /*ICF_SIC_TRACE(p_sic_pdb,ICF_TRACE_SIC_SSA_FAIL)*/

            /* Stop Subscription Response timer */ 
            icf_sic_stop_curr_timer(p_sic_pdb);

            /* indicate the failure */
            ICF_SIC_SET_CURR_STATUS(
                    p_sic_pdb,
                    ICF_SUBS_UNSUBSCRIBED)
                
            ret_val = icf_sic_send_subs_state_to_app(p_sic_pdb);

            /* trigger SSA to clear the context */
            /*icf_sic_send_delink_to_ssa(p_sic_pdb,*/
                   /* (icf_boolean_t)ICF_FALSE);*/
            
            /* De-allocate SIC ctx */
            icf_sic_dealloc_context(p_sic_pdb);
        }
    }/* if (ICF_TRUE ==..*/

    ICF_FUNCTION_EXIT(p_sic_pdb->p_glb_pdb)
    return ret_val;        
}

/*******************************************************************************
 *
 * FUNCTION:        icf_sic_start_timer
 *
 * DESCRIPTION:     This function starts the timer of the type in the SIC 
 *                  ctx.
 *
 ******************************************************************************/
icf_return_t icf_sic_start_timer(
        INOUT icf_sic_pdb_st      *p_sic_pdb)
{


    icf_return_t         ret_val = ICF_SUCCESS;
    icf_timer_data_st    *p_timer_data = ICF_NULL;
    icf_timer_id_t       timer_id = ICF_TIMER_ID_INVALID;
    icf_duration_t       duration;
	icf_line_data_st	*p_line_data = ICF_NULL;   
 
    ICF_FUNCTION_ENTER(p_sic_pdb->p_glb_pdb)
        
    /* Request DBM to create timer block */
    ret_val = icf_dbm_get_mem_from_pool(
            p_sic_pdb->p_glb_pdb,
            (icf_pool_id_t)ICF_MEM_TIMER,
            (icf_void_t **)&p_timer_data,
            p_sic_pdb->p_ecode); 
    
    /* if the timer block is created then fill the timer data structure on the
     * basis of timer type 
     */
    if (ICF_SUCCESS == ret_val)
    {
        ICF_ASSERT(p_sic_pdb->p_glb_pdb, (ICF_NULL != p_timer_data))

        /* Fill the timer data strcuture */
        p_timer_data->timer_type = p_sic_pdb->p_sic_ctx->timer_type;
        p_timer_data->module_id = ICF_INT_MODULE_SIC;
        p_timer_data->p_timer_buffer = 
            (icf_void_t *)(p_sic_pdb->p_sic_ctx);
        p_timer_data->p_glb_pdb = p_sic_pdb->p_glb_pdb;
        
        /* In case of response timer the duration will be copied from the
         * config data
         */
        if (ICF_TIMER_TYPE_SUBS_RESPONSE == 
                p_sic_pdb->p_sic_ctx->timer_type)
        {
            duration = p_sic_pdb->p_config_data->subs_resp_timer;
            
            /* For trace */
            p_sic_pdb->timer_duration = duration;
            /*ICF_SIC_TRACE(p_sic_pdb,ICF_TRACE_SIC_TIMER_DATA)*/
        }
        else if ( ICF_TIMER_TYPE_AWAIT_SUBS_NOTIFY_REQ ==
					p_sic_pdb->p_sic_ctx->timer_type)
		{
			duration = ICF_SIC_AWAIT_NOTIFY_REQ;
		}
/*NOTIFY_OUT_OF_DIALOG_START*/
		else if (ICF_TIMER_TYPE_NOTIFY ==
					p_sic_pdb->p_sic_ctx->timer_type )
		{
			duration = ICF_SIC_AWAIT_NOTIFY_CFM;
		}
/*NOTIFY_OUT_OF_DIALOG_END*/
        /* In case of retry or duration timer the duration will be copied from
         * the pdb as set by the calling function
         */
	 else if (ICF_TIMER_TYPE_SUBS_DURATION ==
            p_sic_pdb->p_sic_ctx->timer_type )

        {
            if (ICF_FAILURE ==
                icf_dbm_fetch_line_cntxt_blk(
                    p_sic_pdb->p_glb_pdb,
                    p_sic_pdb->p_sic_ctx->line_id,
                    (icf_line_data_st **)(&p_line_data),
                     p_sic_pdb->p_ecode))

                {
                    icf_dbm_ret_mem_to_pool(p_sic_pdb->p_glb_pdb, ICF_MEM_TIMER,
                                            p_timer_data, p_sic_pdb->p_ecode);
                    ret_val = ICF_FAILURE;
                    ICF_FUNCTION_EXIT(p_sic_pdb->p_glb_pdb)
                    return ret_val;
                }

	        	if ((0 != p_line_data->subs_hs_duration) 
    		    	 && (2*(p_sic_pdb->timer_duration) >
    		    		 p_line_data->subs_hs_duration))

                        {
					duration = 
						2 * p_sic_pdb->timer_duration - 
						p_line_data->subs_hs_duration;
                }
                else
                {
					duration = 
						p_sic_pdb->timer_duration;
                }
        }
        else 
        {
            duration = p_sic_pdb->timer_duration;
            /*ICF_SIC_TRACE(p_sic_pdb,ICF_TRACE_SIC_TIMER_DATA)*/
        }

        /* Start the timer */
        ICF_START_TIMER(p_sic_pdb->p_glb_pdb,p_timer_data,timer_id,
                duration,p_sic_pdb->p_ecode,ret_val)
     
        /* If the timer is started then copy the p_timer_data in the SIC ctx */
        if (ICF_SUCCESS == ret_val)
        {    
            p_sic_pdb->p_sic_ctx->p_timer_data = p_timer_data;
        
            /* Save the timer id also */
            p_sic_pdb->p_sic_ctx->timer_id = timer_id;
        }
    
       /* if there is any failure to start the timer then reset the timer type 
        * in the SIC ctx and request DBM to free the timer block.
        */ 
        else
        {
            p_sic_pdb->p_sic_ctx->timer_type = ICF_NULL;     
            p_sic_pdb->p_sic_ctx->timer_id = ICF_TIMER_ID_INVALID;
            
            icf_dbm_ret_mem_to_pool(p_sic_pdb->p_glb_pdb,
                    (icf_pool_id_t)ICF_MEM_TIMER,
                    p_timer_data,p_sic_pdb->p_ecode);
        }
    }/* if (ICF_SUCCESS ==..*/

    else
    {
        /* If DBM fails to allcoate a timer block then reset the timer type 
         * in the SIC ctx to NULL 
         */
        p_sic_pdb->p_sic_ctx->timer_type = ICF_NULL;         
    }
    
    ICF_FUNCTION_EXIT(p_sic_pdb->p_glb_pdb)
    return ret_val;
}


/*******************************************************************************
 *
 * FUNCTION:        icf_sic_stop_curr_timer
 *
 * DESCRIPTION:     This function stops the current running timer if there is
 *                  any.
 *
 ******************************************************************************/
icf_return_t icf_sic_stop_curr_timer(
        INOUT icf_sic_pdb_st      *p_sic_pdb)
{
    icf_return_t     ret_val = ICF_SUCCESS;
    icf_timer_id_t   timer_id;

    ICF_FUNCTION_ENTER(p_sic_pdb->p_glb_pdb)
    
    /* If there is any running timer then 
     * 1. stop the current timer. 
     * 2. Reset the timer type and the timer data pointer in the SIC ctx. 
     * 3. Request DBM to free the timer data block.
     */
    
   timer_id = p_sic_pdb->p_sic_ctx->timer_id;
    
    if (ICF_TIMER_ID_INVALID != timer_id)
    {
        ICF_STOP_TIMER(p_sic_pdb->p_glb_pdb,timer_id,p_sic_pdb->p_ecode,
                ret_val)

        icf_dbm_ret_mem_to_pool(p_sic_pdb->p_glb_pdb,
                (icf_pool_id_t)ICF_MEM_TIMER,
                p_sic_pdb->p_sic_ctx->p_timer_data,p_sic_pdb->p_ecode);
    }        
    
    p_sic_pdb->p_sic_ctx->timer_type = ICF_NULL;
    p_sic_pdb->p_sic_ctx->timer_id = ICF_TIMER_ID_INVALID;
    p_sic_pdb->p_sic_ctx->p_timer_data = ICF_NULL;
    
    ICF_FUNCTION_EXIT(p_sic_pdb->p_glb_pdb)
    return ret_val;
}

/*******************************************************************************
 *
 * FUNCTION:        icf_sic_send_subs_state_to_app 
 *
 * DESCRIPTION:     This function sends the subscription state to the
 *                  corresponding module (APP or REG)
 *
 ******************************************************************************/
icf_return_t icf_sic_send_subs_state_to_app(
        INOUT icf_sic_pdb_st      *p_sic_pdb)
{
    icf_return_t     ret_val = ICF_SUCCESS, dummy_retval = ICF_SUCCESS;
    icf_sic_subs_state_st subs_state_msg;
        
    ICF_FUNCTION_ENTER(p_sic_pdb->p_glb_pdb)

    p_sic_pdb->p_internal_msg->msg_hdr.msg_id = 
        ICF_SIC_SUBSCRIPTION_STATUS;

    /* Copy the subscription state in the msg from the SIC pdb as set by 
     * the macro ICF_SIC_SET_CURR_STATUS 
     */
    subs_state_msg.subs_state = p_sic_pdb->curr_subs_status;
    
    /* Copy the event package from the SIC ctx */
    /*subs_state_msg.event_package = p_sic_pdb->event_package;*/
    icf_port_strcpy(subs_state_msg.event_package.str,
        p_sic_pdb->event_package.str);

    subs_state_msg.p_subs_key = p_sic_pdb->p_sic_ctx->p_app_ctx;
    subs_state_msg.line_id = p_sic_pdb->p_sic_ctx->line_id;
    subs_state_msg.call_id = p_sic_pdb->p_sic_ctx->call_id;
    subs_state_msg.app_id = p_sic_pdb->p_sic_ctx->app_id;
    subs_state_msg.resp_code = p_sic_pdb->resp_code;
	/* now that the duration is in seconds, dont divide by 1000 */
    /*subs_state_msg.duration = p_sic_pdb->p_sic_ctx->duration/1000;*/
	subs_state_msg.duration = p_sic_pdb->p_sic_ctx->duration;

    /*if duration is 0 then it?™s unsubscribe case and this can happen only
     * when the release xconf case therefore SL of xconf is no more present.
     * So we need to send unsubscribe request from SIC*/
    if(0 == p_sic_pdb->p_sic_ctx->duration)
    {
       p_sic_pdb->p_sic_ctx->bitmask &= 
           ~ICF_SIC_INTERNAL_SUBSCRIBE_REQUEST;

    }

    ICF_SIC_APP_TRACE(p_sic_pdb,ICF_TRACE_SUBS_RESP)

    if( p_sic_pdb->send_htb_list == ICF_TRUE )
    {
        if (ICF_NULL != p_sic_pdb->p_sic_ctx->header_list.count)
        {
            ret_val = icf_cmn_clone_header_list(
                        p_sic_pdb->p_glb_pdb,
                        &(p_sic_pdb->p_sic_ctx->header_list),
                        &(subs_state_msg.header_list),
                        p_sic_pdb->p_ecode);
            icf_cmn_free_header_list(p_sic_pdb->p_glb_pdb,
                                &(p_sic_pdb->p_sic_ctx->header_list),
                                p_sic_pdb->p_ecode);
        }
        else
            subs_state_msg.header_list.count=0;
    }
    else
    {
        subs_state_msg.header_list.count=0;
    }
    
    p_sic_pdb->p_internal_msg->p_msg_data = 
            (icf_uint8_t *)&subs_state_msg;
    
    
    /* If event package is reg */
    if(
        icf_port_strcmp( p_sic_pdb->p_sic_ctx->event_package.str, (icf_uint8_t *)"reg" ) == 0 )
    {
        /* Send the msg to REGM */
        /*ICF_SIC_TRACE(p_sic_pdb,ICF_TRACE_SIC_SUBS_STATE)*/

        ret_val = icf_cfg_process_mesg(p_sic_pdb->p_internal_msg);
        
        /* Failure from CFG will be ignored */
    }/*if (ICF_EVENT_PKG_REG ..*/

    /*  Changes For XCONF */
    /*  Sending the REsponse to the XCONF through CM */
    else if(p_sic_pdb->p_sic_ctx->bitmask & 
            ICF_SIC_INTERNAL_SUBSCRIBE_REQUEST)
    {
       /* This function prepares the mesg send to PA */
	   ret_val = icf_adm_map_sic_event_to_ext_event(p_sic_pdb);
	   if(ICF_SUCCESS == ret_val)
	   {
	      p_sic_pdb->p_internal_msg->msg_hdr.msg_id =
           ICF_INTERNAL_SUBSCRIBE_RESP;

          /* Call is required in XCONF case otherwise it fails in SCM*/
          p_sic_pdb->p_internal_msg->msg_hdr.p_glb_pdb->p_call_ctx =
               p_sic_pdb->p_sic_ctx->p_call_ctx;
	      ret_val = icf_cm_process_mesg(p_sic_pdb->p_internal_msg);
	      if(ICF_SUCCESS == ret_val)
          {
             if ( ICF_NULL != p_sic_pdb->p_internal_msg->p_msg_data)
              {
                 icf_subscription_status_ind_st* p_subs_ind = 
                 (icf_subscription_status_ind_st*)
                 (((icf_msg_st*)(p_sic_pdb->p_internal_msg->p_msg_data))->payload);

                 if ( ICF_NULL != p_subs_ind && 
                        ICF_NULL != p_subs_ind->header_list.count)
                  {
                      icf_cmn_free_header_list(p_sic_pdb->p_glb_pdb,
                              &(p_subs_ind->header_list),
                              p_sic_pdb->p_ecode);
                  }
              }
              ICF_MEMFREE(p_sic_pdb->p_glb_pdb, 
                    p_sic_pdb->p_internal_msg->p_msg_data, ICF_MEM_COMMON,
                        p_sic_pdb->p_ecode, dummy_retval)
           }
         }

       }
       /*   Change for XCONF End */
    else
    {

        /* Send the msg to APP */
        /*ICF_SIC_TRACE(p_sic_pdb,ICF_TRACE_SIC_SUBS_STATE)*/
        ret_val = icf_adm_map_sic_event_to_ext_event(p_sic_pdb);

        if( ICF_SUCCESS == ret_val)
        {
            ret_val = icf_adm_process_mesg(p_sic_pdb->p_internal_msg);
            if( ICF_SUCCESS == ret_val)
            {
                if ( ICF_NULL != p_sic_pdb->p_internal_msg->p_msg_data)
                {
                    icf_subscription_status_ind_st* p_subs_ind = (icf_subscription_status_ind_st*)
                        (((icf_msg_st*)(p_sic_pdb->p_internal_msg->p_msg_data))->payload);

                    if ( ICF_NULL != p_subs_ind && 
                        ICF_NULL != p_subs_ind->header_list.count)
                    {
                        icf_cmn_free_header_list(p_sic_pdb->p_glb_pdb,
                            &(p_subs_ind->header_list),
                            p_sic_pdb->p_ecode);
                    }
                }
                ICF_MEMFREE(p_sic_pdb->p_glb_pdb, 
                    p_sic_pdb->p_internal_msg->p_msg_data, ICF_MEM_COMMON,
                        p_sic_pdb->p_ecode, dummy_retval)
            }
        }
        /* Failure from ADM will be ignored as this is the failure to 
         * indicate PA only 
         */
    }     
    

    ICF_FUNCTION_EXIT(p_sic_pdb->p_glb_pdb)
    return ret_val;        
}/* End function */


/*******************************************************************************
 *
 * FUNCTION:        icf_sic_send_delink_to_ssa  
 *
 * DESCRIPTION:     This function sends the delink API to SSA.
 *
 ******************************************************************************/
icf_return_t icf_sic_send_delink_to_ssa(
        INOUT icf_sic_pdb_st      *p_sic_pdb,
        IN icf_boolean_t           forceful)
{
    icf_return_t             ret_val = ICF_SUCCESS;
    icf_delink_app_ctx_st    delink_app;
    /* To remove warning */
    if(ICF_TRUE==forceful)
    {}
    
    ICF_FUNCTION_ENTER(p_sic_pdb->p_glb_pdb)
    
	if(ICF_NULL != p_sic_pdb->p_sic_ctx->p_ssa_ctx)
	{
		p_sic_pdb->p_internal_msg->msg_hdr.msg_id = ICF_DELINK_APP_CTX;
    
    /* Copy the SIC ctx pointer in the msg */
    delink_app.p_sic_ctx = (icf_sic_ctx_st *)(p_sic_pdb->p_sic_ctx);
    delink_app.forceful_delete = forceful;
    delink_app.module_id = ICF_INT_MODULE_SIC;
    p_sic_pdb->p_internal_msg->p_msg_data = (icf_uint8_t *)(&delink_app);
		/* Copy the SIC ctx pointer in the msg */
		delink_app.p_sic_ctx = (icf_sic_ctx_st *)(p_sic_pdb->p_sic_ctx);
		/*delink_app.forceful_delete = forceful;*/
		/*delink_app.module_id = ICF_INT_MODULE_SIC;*/
		p_sic_pdb->p_internal_msg->p_msg_data = (icf_uint8_t *)(&delink_app);
		/* Call SSA */
		/*ICF_SIC_TRACE(p_sic_pdb,ICF_TRACE_SIC_DELINK_APP)*/
    
		ret_val = icf_ssa_process_mesg(p_sic_pdb->p_internal_msg);
    
		/* Set SSA ctx pointer in SIC ctx to NULL */
		p_sic_pdb->p_sic_ctx->p_ssa_ctx = ICF_NULL;
	}

    ICF_FUNCTION_EXIT(p_sic_pdb->p_glb_pdb)
    return ret_val;        
}

/*******************************************************************************
 *
 * FUNCTION:        icf_sic_send_notify_resp   
 *
 * DESCRIPTION:     This function sends the notify resp to SSA.
 *
 ******************************************************************************/
icf_return_t icf_sic_send_notify_resp(
        INOUT icf_sic_pdb_st      *p_sic_pdb)
{
    icf_return_t     ret_val = ICF_SUCCESS;
    icf_nw_otg_notify_resp_st notify_resp;
    
    ICF_FUNCTION_ENTER(p_sic_pdb->p_glb_pdb)
    
    p_sic_pdb->p_internal_msg->msg_hdr.msg_id = ICF_NW_OTG_NOTIFY_RESP;
    
    /* Set the result and the SIC ctx in the msg from the pdb */
    notify_resp.result = p_sic_pdb->notify_resp;
    notify_resp.p_sic_ctx = p_sic_pdb->p_sic_ctx;
	notify_resp.response_code = 200;
    
    /* Call SSA */
    p_sic_pdb->p_internal_msg->p_msg_data = (icf_uint8_t *)&notify_resp;
    
    /*ICF_SIC_TRACE(p_sic_pdb,ICF_TRACE_SIC_NOTIFY_RESP)*/
    ret_val = icf_ssa_process_mesg(p_sic_pdb->p_internal_msg);
    /* Failure from SSA is ignored */
 
    ICF_FUNCTION_EXIT(p_sic_pdb->p_glb_pdb)
    return ret_val;
}

/*******************************************************************************
 *
 * FUNCTION:        icf_sic_send_notify_msg_to_app    
 *
 * DESCRIPTION:     This function sends the notify msg to the application.
 *
 ******************************************************************************/
icf_return_t icf_sic_send_notify_msg_to_app(
        INOUT icf_sic_pdb_st      *p_sic_pdb)
{
    icf_return_t     ret_val = ICF_SUCCESS;
    icf_sic_notify_msg_st  notify_msg;
    icf_nw_inc_notify_ind_st *p_notify_recd = ICF_NULL;
    icf_return_t        dummy_retval = ICF_SUCCESS;
    ICF_FUNCTION_ENTER(p_sic_pdb->p_glb_pdb)


    /*if no body is received in NOTIFY, dont send anything to the application*/
    /* Subscription state related information would anyways be passes through */
    /* the Subscription State Indication API */
/*    if(ICF_NULL == p_sic_pdb->p_sic_ctx->body_list.count)*/
/*        return ret_val;*/

    p_notify_recd = (icf_nw_inc_notify_ind_st *)(p_sic_pdb->p_msg);

    p_sic_pdb->p_internal_msg->msg_hdr.msg_id = ICF_SIC_NOTIFY_IND;
    
	/* now that the duration is in seconds, dont divide by 1000 */
    /*notify_msg.duration= p_sic_pdb->p_sic_ctx->duration/1000;*/
    if(p_sic_pdb->p_sic_ctx->duration)
    {
	    notify_msg.duration= p_sic_pdb->p_sic_ctx->duration;
    }
    notify_msg.line_id = p_sic_pdb->p_sic_ctx->line_id;
    notify_msg.call_id = p_sic_pdb->p_sic_ctx->call_id;
    notify_msg.app_id = p_sic_pdb->p_sic_ctx->app_id;
	notify_msg.p_subs_key = ICF_NULL;
/*    notify_msg.event_pkg = p_sic_pdb->p_sic_ctx->event_package;*/
    icf_port_strcpy(notify_msg.event_pkg.str,
        p_sic_pdb->p_sic_ctx->event_package.str);

/*NOTIFY_OUT_OF_DIALOG_START*/
	if(ICF_NW_INC_NOTIFY_SENDER_PRESENT == 
			(p_notify_recd->bitmask & ICF_NW_INC_NOTIFY_SENDER_PRESENT))
	{
		notify_msg.p_sender_address = &(p_sic_pdb->p_sic_ctx->dest_addr);

	}
	else
	{
		notify_msg.p_sender_address = ICF_NULL;
	}

	if(ICF_NW_INC_NOTIFY_USER_ADDR_PRESENT ==
			(p_notify_recd->bitmask & ICF_NW_INC_NOTIFY_USER_ADDR_PRESENT))
	{
		notify_msg.p_user_address = p_sic_pdb->p_sic_ctx->p_user_address;
	}
	else
	{
		notify_msg.p_user_address = ICF_NULL;
	}

	notify_msg.subs_state = p_notify_recd->subs_state;
	if(!p_notify_recd->subs_state)
		notify_msg.reason_code = p_notify_recd->reason_code;
	else
		notify_msg.reason_code = 0;


	if(ICF_NW_INC_SUBS_DURATION_PRESENT == 
			(p_notify_recd->bitmask & ICF_NW_INC_SUBS_DURATION_PRESENT))
	{
		notify_msg.duration = p_notify_recd->duration;
	}
	else
	{
		notify_msg.duration = 0;
	}
    
    /*if duration is 0 then it?™s unsubscribe case and this can happen only
     * when the release xconf case therefore SL of xconf is no more present.
     * So we need to send unsubscribe request from SIC*/
    if(0 == notify_msg.duration)
    {
	   p_sic_pdb->p_sic_ctx->bitmask &= 
                  ~ICF_SIC_INTERNAL_SUBSCRIBE_REQUEST;
    }

	if(ICF_NW_INC_NOTIFY_OUT_OF_DIALOG == 
			(p_notify_recd->bitmask & ICF_NW_INC_NOTIFY_OUT_OF_DIALOG))
	{
		notify_msg.out_of_dlg_mesg = ICF_TRUE;
	}
	else
	{
		notify_msg.out_of_dlg_mesg = ICF_FALSE;
	}

/*NOTIFY_OUT_OF_DIALOG_END*/

    if ((ICF_SUCCESS == ret_val) && 
            (ICF_NULL != p_sic_pdb->p_sic_ctx->header_list.count))
    {
        if (ICF_SUCCESS == icf_cmn_clone_header_list(
                                p_sic_pdb->p_glb_pdb,
                                &(p_sic_pdb->p_sic_ctx->header_list),
                                &(notify_msg.header_list),
                                p_sic_pdb->p_ecode))
        {
            /* free the list in sic ctx */
            ret_val = icf_cmn_free_header_list(p_sic_pdb->p_glb_pdb,
                            &(p_sic_pdb->p_sic_ctx->header_list),
                            p_sic_pdb->p_ecode);
        }
        else
        {
            ret_val = ICF_FAILURE;
        }
    }
    else
    {
        notify_msg.header_list.count=0;
    }

    /* clone the body list */
    if ((ICF_SUCCESS == ret_val) && 
        (ICF_NULL != p_sic_pdb->p_sic_ctx->body_list.count))
    {
        if (ICF_SUCCESS == icf_cmn_clone_body_list(
                                p_sic_pdb->p_glb_pdb,
                                &(p_sic_pdb->p_sic_ctx->body_list),
                                &(notify_msg.body_list),
                                p_sic_pdb->p_ecode))
        {
            /* free the list in sic ctx */
            ret_val = icf_cmn_free_msg_body_list(p_sic_pdb->p_glb_pdb,
                            &(p_sic_pdb->p_sic_ctx->body_list),
                            p_sic_pdb->p_ecode);
        }
        else
        {
            ret_val = ICF_FAILURE;
        }
    }
    else
    {
        notify_msg.body_list.count=0;
    }

    if (ICF_SUCCESS == ret_val) 

    {
        notify_msg.p_subs_key = (icf_void_t *)
                (p_sic_pdb->p_sic_ctx->p_app_ctx);
    
        p_sic_pdb->p_internal_msg->p_msg_data = 
                    (icf_uint8_t *)&notify_msg;
        
        if(
            icf_port_strcmp( p_notify_recd->p_sic_ctx->event_package.str, (icf_uint8_t *)"reg") ==0 )
        {
        
            /*ICF_SIC_TRACE(p_sic_pdb,ICF_TRACE_SIC_NOTIFY_MSG)*/
            ret_val = icf_cfg_process_mesg(p_sic_pdb->p_internal_msg);
            /* Failure from CFG (REGM) will be ignored */
            if (ICF_NULL != notify_msg.body_list.count)
            {
                /* free the list in motify_msg */
                ret_val = icf_cmn_free_msg_body_list(p_sic_pdb->p_glb_pdb,
                    &(notify_msg.body_list),
                    p_sic_pdb->p_ecode);
            }
            if (ICF_NULL != notify_msg.header_list.count)
            {
                /* free the list in motify_msg */
                ret_val = icf_cmn_free_header_list(p_sic_pdb->p_glb_pdb,
                    &(notify_msg.header_list),
                    p_sic_pdb->p_ecode);
            }
            
        } 
	    /*  Change for XCONF */
        /* Code for sending the message to the XCONF via CM for sending Notify Indication */
	    else if(p_sic_pdb->p_sic_ctx->bitmask & 
                  ICF_SIC_INTERNAL_SUBSCRIBE_REQUEST)
	     {
            ret_val = icf_adm_map_sic_event_to_ext_event(p_sic_pdb);
            if(ICF_SUCCESS == ret_val)
            {    
	           p_sic_pdb->p_internal_msg->msg_hdr.msg_id = 
                   ICF_INTERNAL_NOTIFY_IND;
               p_sic_pdb->p_internal_msg->msg_hdr.p_glb_pdb->p_call_ctx =
                   p_sic_pdb->p_sic_ctx->p_call_ctx;

	           ret_val = icf_cm_process_mesg(p_sic_pdb->p_internal_msg);

               if (ICF_NULL != p_sic_pdb->p_internal_msg->p_msg_data)
               {
                  icf_notify_ind_st* p_notify_ind = (icf_notify_ind_st*)
                  (((icf_msg_st*)(p_sic_pdb->p_internal_msg->p_msg_data)) \
                   ->payload);
                  if ( ICF_NULL != p_notify_ind && 
                        ICF_NULL != p_notify_ind->header_list.count)
                  {
                    icf_cmn_free_header_list(p_sic_pdb->p_glb_pdb,
                            &(p_notify_ind->header_list),
                            p_sic_pdb->p_ecode);
                  }
                  if( ICF_NULL != p_notify_ind && 
                        ICF_NULL != p_notify_ind->body_list.count)
                  {
                        icf_cmn_free_msg_body_list(p_sic_pdb->p_glb_pdb,
                            &(p_notify_ind->body_list),
                            p_sic_pdb->p_ecode);
                  }

               }
               ICF_MEMFREE(p_sic_pdb->p_glb_pdb, 
                    p_sic_pdb->p_internal_msg->p_msg_data, ICF_MEM_COMMON,
                    p_sic_pdb->p_ecode, dummy_retval)
              }      
	      }
	      /* Change for XCONF End */
         else
         {
            /*ICF_SIC_TRACE(p_sic_pdb,ICF_TRACE_SIC_NOTIFY_MSG)*/
            ret_val = icf_adm_map_sic_event_to_ext_event(p_sic_pdb);
            if( ICF_SUCCESS == ret_val)
            {

                ret_val = icf_adm_process_mesg(p_sic_pdb->p_internal_msg);

                if ( ICF_NULL != p_sic_pdb->p_internal_msg->p_msg_data)
                {
                    icf_notify_ind_st* p_notify_ind = (icf_notify_ind_st*)
                        (((icf_msg_st*)(p_sic_pdb->p_internal_msg->p_msg_data))->payload);
                    if ( ICF_NULL != p_notify_ind && 
                        ICF_NULL != p_notify_ind->header_list.count)
                    {
                        icf_cmn_free_header_list(p_sic_pdb->p_glb_pdb,
                            &(p_notify_ind->header_list),
                            p_sic_pdb->p_ecode);
                    }
                    if( ICF_NULL != p_notify_ind && 
                        ICF_NULL != p_notify_ind->body_list.count)
                    {
                        icf_cmn_free_msg_body_list(p_sic_pdb->p_glb_pdb,
                            &(p_notify_ind->body_list),
                            p_sic_pdb->p_ecode);
                    }

                }
                ICF_MEMFREE(p_sic_pdb->p_glb_pdb, 
                    p_sic_pdb->p_internal_msg->p_msg_data, ICF_MEM_COMMON,
                    p_sic_pdb->p_ecode, dummy_retval)
            }
       
            /* Failure from APP will be ignored */
        }
     
    }


   
    ICF_FUNCTION_EXIT(p_sic_pdb->p_glb_pdb) 
    return ret_val;        
}

/*******************************************************************************
 *
 * FUNCTION:        icf_sic_send_message_resp   
 *
 * DESCRIPTION:     This function sends the Message resp to SSA.
 *
 ******************************************************************************/
icf_return_t icf_sic_send_message_resp(
        INOUT icf_sic_pdb_st      *p_sic_pdb)
{
    icf_return_t     ret_val = ICF_SUCCESS;
    icf_nw_otg_message_resp_st message_resp;
    
    ICF_FUNCTION_ENTER(p_sic_pdb->p_glb_pdb)
    
    p_sic_pdb->p_internal_msg->msg_hdr.msg_id = ICF_NW_OTG_MESSAGE_RESP;
    
    /* Set the result and the SIC ctx in the msg from the pdb */
    message_resp.result = ICF_SUCCESS;
    message_resp.p_sic_ctx = p_sic_pdb->p_sic_ctx;
    
    /* Call SSA */
    p_sic_pdb->p_internal_msg->p_msg_data = (icf_uint8_t *)&message_resp;
   
    /*ICF_SIC_TRACE(p_sic_pdb,ICF_TRACE_SIC_NOTIFY_RESP)*/
    ret_val = icf_ssa_process_mesg(p_sic_pdb->p_internal_msg);
    /* Failure from SSA is ignored */
 
    ICF_FUNCTION_EXIT(p_sic_pdb->p_glb_pdb)
    return ret_val;
}




#ifdef ICF_TRACE_ENABLE
/*******************************************************************************
 *
 * FUNCTION:        icf_sic_app_trace
 *
 * DESCRIPTION:     This function is used by SIC for trace handling.
 *
 ******************************************************************************/
icf_return_t icf_sic_app_trace(
        INOUT icf_sic_pdb_st *p_sic_pdb,
        IN icf_trace_id_t trace_id)
{
    icf_trace_data_st                trace_data = {{0,0},0,{0},0,{{0,"\0"}},{0,"\0"},0} ;
    icf_trace_criteria_data_st       *p_criteria_data = ICF_NULL;
    icf_return_t                     ret_val = ICF_SUCCESS;
    icf_error_t                      ecode = ICF_ERROR_NONE;
    icf_internal_msg_st              *p_internal_msg=ICF_NULL;
    icf_sic_message_status_st        *p_sic_msg_resp = ICF_NULL;
    icf_sic_publish_status_st        *p_sic_pub_resp = ICF_NULL;
 
    ICF_FUNCTION_ENTER(p_sic_pdb->p_glb_pdb)
    p_internal_msg = p_sic_pdb->p_internal_msg;
    if(ICF_NULL == p_sic_pdb->p_sic_ctx->p_trace_criteria_data)
     {
            ICF_MEMGET(p_sic_pdb->p_glb_pdb,
              ICF_PORT_SIZEOF(icf_trace_criteria_data_st),
              ICF_MEM_COMMON,
              p_criteria_data,
              ICF_RET_ON_FAILURE, &ecode, ret_val)

          p_sic_pdb->p_sic_ctx->p_trace_criteria_data = p_criteria_data;
     }
     else
     {
          p_criteria_data = p_sic_pdb->p_sic_ctx->p_trace_criteria_data;
     }

    if(ICF_NULL != p_sic_pdb->p_glb_pdb->p_recv_msg)
     {
        p_sic_pdb->p_glb_pdb->p_glb_data->app_trace_info.call_id =
                p_sic_pdb->p_glb_pdb->p_recv_msg->hdr.call_id;
     }
    else
     {
        p_sic_pdb->p_glb_pdb->p_glb_data->app_trace_info.call_id =
                p_sic_pdb->p_sic_ctx->call_id;
     }

    switch(trace_id)
    {

       case ICF_TRACE_OPTIONS_TIMER_EXPIRE:
             p_sic_pdb->p_glb_pdb->p_glb_data->app_trace_info.app_id =
                p_sic_pdb->p_sic_ctx->app_id;

             trace_data.num_of_integer_values = ICF_NO_INT_PRESENT;

             trace_data.number_of_string_values = ICF_NO_STRING_PRESENT;

             break;

      case ICF_TRACE_SUBSCRIPTION_REQ:
      case ICF_TRACE_RESUBS:
      case ICF_TRACE_OTG_PUBLISH:
          
             p_sic_pdb->p_glb_pdb->p_glb_data->app_trace_info.app_id =
                p_sic_pdb->p_sic_ctx->app_id;
 

             trace_data.num_of_integer_values = ICF_TWO_INT_PRESENT;
 
             trace_data.int_val[0] =
                     (icf_uint32_t)(p_sic_pdb->p_sic_ctx)->line_id;

             trace_data.int_val[1] =
                     (icf_uint32_t)(p_sic_pdb->p_sic_ctx)->duration;
             
 
             /*trace_data.number_of_string_values = ICF_THREE_STRING_PRESENT;*/
              
             if(p_sic_pdb->p_sic_ctx->p_preferred_identity)
             {     
                 trace_data.number_of_string_values = ICF_THREE_STRING_PRESENT;

                 icf_port_strcpy(trace_data.string_val[0].str, 
                         p_sic_pdb->p_sic_ctx->p_preferred_identity->addr_val.str);
                 trace_data.string_val[0].str_len =
                     (icf_uint16_t)icf_port_strlen(trace_data.string_val[0].str);

                 icf_port_strcpy(trace_data.string_val[1].str,
                                        p_sic_pdb->p_sic_ctx->dest_addr.addr_val.str);
                 trace_data.string_val[1].str_len =
                        (icf_uint16_t)icf_port_strlen(trace_data.string_val[1].str);

                 icf_port_strcpy(trace_data.string_val[2].str,
                                        p_sic_pdb->event_package.str);
                 trace_data.string_val[2].str_len =
                        (icf_uint16_t)icf_port_strlen(trace_data.string_val[2].str);
 
            }
            else
            {
                 trace_data.number_of_string_values = ICF_TWO_STRING_PRESENT;

                /* icf_port_strcpy(trace_data.string_val[0].str,
                         p_sic_pdb->p_sic_ctx->p_preferred_identity->addr_val.str);
                 trace_data.string_val[0].str_len =
                     icf_port_strlen(trace_data.string_val[0].str);*/

                 icf_port_strcpy(trace_data.string_val[0].str,
                                        p_sic_pdb->p_sic_ctx->dest_addr.addr_val.str);
                 trace_data.string_val[0].str_len =
                        (icf_uint16_t)icf_port_strlen(trace_data.string_val[0].str);

                 icf_port_strcpy(trace_data.string_val[1].str,
                                        p_sic_pdb->event_package.str);
                 trace_data.string_val[1].str_len =
                        (icf_uint16_t)icf_port_strlen(trace_data.string_val[1].str);

            }                    

             p_criteria_data->line_id =
                            p_sic_pdb->p_sic_ctx->line_id;
             p_criteria_data->bitmask |= ICF_TRACE_LINE_ID_PRESENT;

             p_criteria_data->p_calling_party_address =
                 p_sic_pdb->p_sic_ctx->p_user_address;
             p_criteria_data->bitmask |= ICF_TRACE_CALLING_PARTY_ADDR_PRESENT;



             break;



	 case ICF_TRACE_UNSUBS_REQ:

             p_sic_pdb->p_glb_pdb->p_glb_data->app_trace_info.app_id =
                p_sic_pdb->p_sic_ctx->app_id;


             trace_data.num_of_integer_values = ICF_ONE_INT_PRESENT;
 
              trace_data.int_val[0] =
                     (icf_uint32_t)p_sic_pdb->p_sic_ctx->line_id;

            /* trace_data.number_of_string_values = ICF_TWO_STRING_PRESENT;*/

            if(p_sic_pdb->p_sic_ctx->p_preferred_identity)
            {
                trace_data.number_of_string_values = ICF_TWO_STRING_PRESENT;

                icf_port_strcpy(trace_data.string_val[0].str, 
                        p_sic_pdb->p_sic_ctx->p_preferred_identity->addr_val.str);
                trace_data.string_val[0].str_len =
                    (icf_uint16_t)icf_port_strlen(trace_data.string_val[0].str);
            
                icf_port_strcpy(trace_data.string_val[1].str,
                                        p_sic_pdb->p_sic_ctx->dest_addr.addr_val.str);
                trace_data.string_val[1].str_len =
                        (icf_uint16_t)icf_port_strlen(trace_data.string_val[1].str);
            }
            else
            {
                trace_data.number_of_string_values = ICF_ONE_STRING_PRESENT;

               /* icf_port_strcpy(trace_data.string_val[0].str,
                        p_sic_pdb->p_sic_ctx->p_preferred_identity->addr_val.str);
                trace_data.string_val[0].str_len =
                    icf_port_strlen(trace_data.string_val[0].str);*/

                icf_port_strcpy(trace_data.string_val[0].str,
                                        p_sic_pdb->p_sic_ctx->dest_addr.addr_val.str);
                trace_data.string_val[0].str_len =
                        (icf_uint16_t)icf_port_strlen(trace_data.string_val[0].str);
          
            } 

             break;

        case ICF_TRACE_SUBS_RESP:

               p_sic_pdb->p_glb_pdb->p_glb_data->app_trace_info.app_id
                    = p_sic_pdb->p_sic_ctx->app_id;

             trace_data.num_of_integer_values = ICF_THREE_INT_PRESENT;
 
              trace_data.int_val[0] =
                     (icf_uint32_t)p_sic_pdb->p_sic_ctx->line_id;

                trace_data.int_val[1] =
                     (icf_uint32_t)p_sic_pdb->p_sic_ctx->error_code;

               trace_data.int_val[2] =
                     (icf_uint32_t)(p_sic_pdb->p_sic_ctx->duration);

            /* trace_data.number_of_string_values = ICF_TWO_STRING_PRESENT;*/

             if(p_sic_pdb->p_sic_ctx->p_preferred_identity)
             { 
                 
                 trace_data.number_of_string_values = ICF_TWO_STRING_PRESENT;

                 icf_port_strcpy(trace_data.string_val[0].str, 
                         p_sic_pdb->p_sic_ctx->p_preferred_identity->addr_val.str);
                 trace_data.string_val[0].str_len =
                     (icf_uint16_t)icf_port_strlen(trace_data.string_val[0].str);
            
                 icf_port_strcpy(trace_data.string_val[1].str,
                                        p_sic_pdb->p_sic_ctx->dest_addr.addr_val.str);
                 trace_data.string_val[1].str_len =
                        (icf_uint16_t)icf_port_strlen(trace_data.string_val[1].str);
             }
             else
             {
                 trace_data.number_of_string_values = ICF_ONE_STRING_PRESENT;

                 /*icf_port_strcpy(trace_data.string_val[0].str,
                         p_sic_pdb->p_sic_ctx->p_preferred_identity->addr_val.str);
                 trace_data.string_val[0].str_len =
                     icf_port_strlen(trace_data.string_val[0].str);*/
            
                 icf_port_strcpy(trace_data.string_val[0].str,
                                        p_sic_pdb->p_sic_ctx->dest_addr.addr_val.str);
                 trace_data.string_val[0].str_len =
                        (icf_uint16_t)icf_port_strlen(trace_data.string_val[0].str);

             }  

             break;


       case ICF_TRACE_OTDLG_NOTIFY_IND:
                  
             p_sic_pdb->p_glb_pdb->p_glb_data->app_trace_info.app_id =
                p_sic_pdb->p_sic_ctx->app_id;


             trace_data.num_of_integer_values = ICF_TWO_INT_PRESENT;

             trace_data.int_val[0] =
                     (icf_uint32_t)(p_sic_pdb->p_sic_ctx->line_id);

             trace_data.int_val[1] =
                     (icf_uint32_t)(p_sic_pdb->p_sic_ctx->duration);

             /* trace_data.number_of_string_values = ICF_THREE_STRING_PRESENT;*/

             if(ICF_NULL != p_sic_pdb->p_sic_ctx->p_user_address)
             {

                  trace_data.number_of_string_values = ICF_THREE_STRING_PRESENT;
                   
                     /* Subs state is active */
                   if (ICF_SUBS_STATE_ACTIVE == p_sic_pdb->subs_state)
                   {
                     icf_port_strcpy(trace_data.string_val[0].str,
                                        (icf_uint8_t *) "Active");
                   }

                   /* State is terminated */
                   else if (ICF_SUBS_STATE_TERMINATED ==
                        p_sic_pdb->subs_state)
                   {
                      icf_port_strcpy(trace_data.string_val[0].str,
                                        (icf_uint8_t *) "terminated");

		   }
                   trace_data.string_val[0].str_len =
                     (icf_uint16_t)icf_port_strlen(trace_data.string_val[0].str);

                  icf_port_strcpy(trace_data.string_val[1].str,
                     p_sic_pdb->p_sic_ctx->dest_addr.addr_val.str);
                  trace_data.string_val[1].str_len =
                        (icf_uint16_t)icf_port_strlen(trace_data.string_val[1].str);
                 
                    icf_port_strcpy(trace_data.string_val[2].str,
                       p_sic_pdb->p_sic_ctx->p_user_address->addr_val.str);
                    trace_data.string_val[2].str_len =
                        (icf_uint16_t)icf_port_strlen(trace_data.string_val[2].str);
             }
             else
             { 
                   trace_data.number_of_string_values = ICF_TWO_STRING_PRESENT;
                   
                     /* Subs state is active */
                   if (ICF_SUBS_STATE_ACTIVE == p_sic_pdb->subs_state)
                   {
                     icf_port_strcpy(trace_data.string_val[0].str,
                                        (icf_uint8_t *)"Active");
                   }

                   /* State is terminated */
                   else if (ICF_SUBS_STATE_TERMINATED ==
                        p_sic_pdb->subs_state)
                   {
                      icf_port_strcpy(trace_data.string_val[0].str,
                                        (icf_uint8_t *)"terminated");

                   }
                   trace_data.string_val[0].str_len =
                     (icf_uint16_t)icf_port_strlen(trace_data.string_val[0].str);

                  icf_port_strcpy(trace_data.string_val[1].str,
                     p_sic_pdb->p_sic_ctx->dest_addr.addr_val.str);
                  trace_data.string_val[1].str_len =
                        (icf_uint16_t)icf_port_strlen(trace_data.string_val[1].str);
 
              }                 

                  p_criteria_data->line_id =
                            p_sic_pdb->p_sic_ctx->line_id;
                  p_criteria_data->bitmask |= ICF_TRACE_LINE_ID_PRESENT;

                  p_criteria_data->p_calling_party_address =
                        p_sic_pdb->p_sic_ctx->p_user_address;
                  p_criteria_data->bitmask |= ICF_TRACE_CALLING_PARTY_ADDR_PRESENT;

              break;


            case ICF_TRACE_NOTIFY_CFM:

            p_sic_pdb->p_glb_pdb->p_glb_data->app_trace_info.app_id =
                p_sic_pdb->p_sic_ctx->app_id;


             trace_data.num_of_integer_values = ICF_TWO_INT_PRESENT;

              trace_data.int_val[0] =
                     (p_sic_pdb)->notify_resp;

                trace_data.int_val[1] =
                     (p_sic_pdb)->error_cause;

             trace_data.number_of_string_values = ICF_NO_STRING_PRESENT;

             break;

	case ICF_TRACE_SUBS_TIMER_EXPIRE:
                  
             p_sic_pdb->p_glb_pdb->p_glb_data->app_trace_info.app_id =
                p_sic_pdb->p_sic_ctx->app_id;

             trace_data.num_of_integer_values = ICF_NO_INT_PRESENT;

             trace_data.number_of_string_values = ICF_NO_STRING_PRESENT;

             break;

     case ICF_TRACE_OTG_MESSAGE:

             p_sic_pdb->p_glb_pdb->p_glb_data->app_trace_info.app_id =
                p_sic_pdb->p_sic_ctx->app_id;
 

             trace_data.num_of_integer_values = ICF_ONE_INT_PRESENT;
             trace_data.int_val[0] =
                     (icf_uint32_t)(p_sic_pdb->p_sic_ctx->line_id);

 
             if(p_sic_pdb->p_sic_ctx->p_preferred_identity && p_sic_pdb->p_sic_ctx->p_user_address)
             {     
                 trace_data.number_of_string_values = ICF_THREE_STRING_PRESENT;

                 icf_port_strcpy(trace_data.string_val[0].str, 
                         p_sic_pdb->p_sic_ctx->p_preferred_identity->addr_val.str);
                 trace_data.string_val[0].str_len =
                     (icf_uint16_t)icf_port_strlen(trace_data.string_val[0].str);

                 icf_port_strcpy(trace_data.string_val[1].str, 
                         p_sic_pdb->p_sic_ctx->p_user_address->addr_val.str);
                 trace_data.string_val[1].str_len =
                     (icf_uint16_t)icf_port_strlen(trace_data.string_val[1].str);

                 icf_port_strcpy(trace_data.string_val[2].str,
                                        p_sic_pdb->p_sic_ctx->dest_addr.addr_val.str);
                 trace_data.string_val[2].str_len =
                        (icf_uint16_t)icf_port_strlen(trace_data.string_val[2].str);

            }
            else if (p_sic_pdb->p_sic_ctx->p_preferred_identity)
            {
                 
                trace_data.number_of_string_values = ICF_TWO_STRING_PRESENT;

                 icf_port_strcpy(trace_data.string_val[0].str, 
                         p_sic_pdb->p_sic_ctx->p_preferred_identity->addr_val.str);
                 trace_data.string_val[0].str_len =
                     (icf_uint16_t)icf_port_strlen(trace_data.string_val[0].str);

                 icf_port_strcpy(trace_data.string_val[1].str,
                                        p_sic_pdb->p_sic_ctx->dest_addr.addr_val.str);
                 trace_data.string_val[1].str_len =
                        (icf_uint16_t)icf_port_strlen(trace_data.string_val[1].str);
            }
            
            else if (p_sic_pdb->p_sic_ctx->p_user_address)
            {
                trace_data.number_of_string_values = ICF_TWO_STRING_PRESENT;

                 icf_port_strcpy(trace_data.string_val[0].str, 
                         p_sic_pdb->p_sic_ctx->p_user_address->addr_val.str);
                 trace_data.string_val[0].str_len =
                     (icf_uint16_t)icf_port_strlen(trace_data.string_val[0].str);

                 icf_port_strcpy(trace_data.string_val[1].str,
                                        p_sic_pdb->p_sic_ctx->dest_addr.addr_val.str);
                 trace_data.string_val[1].str_len =
                        (icf_uint16_t)icf_port_strlen(trace_data.string_val[1].str);
            }
            else
            {

                trace_data.number_of_string_values = ICF_ONE_STRING_PRESENT;

                 icf_port_strcpy(trace_data.string_val[0].str,
                                        p_sic_pdb->p_sic_ctx->dest_addr.addr_val.str);
                 trace_data.string_val[0].str_len =
                        (icf_uint16_t)icf_port_strlen(trace_data.string_val[0].str);

            }                    

             p_criteria_data->line_id =
                            p_sic_pdb->p_sic_ctx->line_id;
             p_criteria_data->bitmask |= ICF_TRACE_LINE_ID_PRESENT;

             break;

      case ICF_TRACE_OTG_PUBLISH_RESP:

             p_sic_pub_resp = (icf_sic_publish_status_st*)p_internal_msg->p_msg_data;
             p_sic_pdb->p_glb_pdb->p_glb_data->app_trace_info.app_id =
                p_sic_pdb->p_sic_ctx->app_id;
             
             trace_data.num_of_integer_values = ICF_FOUR_INT_PRESENT;
              
             trace_data.int_val[0] =
                     (icf_uint32_t)(p_sic_pdb->p_sic_ctx)->line_id;

             trace_data.int_val[1] =
                            (icf_uint32_t)p_sic_pub_resp->duration;
                
             trace_data.int_val[2] =
                            (icf_uint32_t)p_sic_pub_resp->resp_code;
            
             if( 200 == trace_data.int_val[2]) 
              {
                  if(0 != icf_port_strlen(p_sic_pdb->p_sic_ctx->entity_tag.str))
                   {
                      trace_data.int_val[3] = 1;
                   }
                  else
                   {
                      trace_data.int_val[3] = 0;
                   }
              }
             else if( trace_data.int_val[2] > 0)
              {
                   trace_data.int_val[3] = 1;
              }
             else
              {
                   trace_data.int_val[3] = 0;
              }

             if(icf_port_strlen(p_sic_pdb->p_sic_ctx->entity_tag.str))
              {     
                 trace_data.number_of_string_values = ICF_ONE_STRING_PRESENT;

                 icf_port_strcpy(trace_data.string_val[0].str, 
                         p_sic_pdb->p_sic_ctx->entity_tag.str);
                 trace_data.string_val[0].str_len =
                     (icf_uint16_t)icf_port_strlen(trace_data.string_val[0].str);
             }
            else
                trace_data.number_of_string_values = ICF_NO_STRING_PRESENT;   

             p_criteria_data->line_id =
                            p_sic_pdb->p_sic_ctx->line_id;
             p_criteria_data->bitmask |= ICF_TRACE_LINE_ID_PRESENT;
             
             break;


      case ICF_TRACE_OTG_MESSAGE_RESP:
             p_sic_msg_resp = (icf_sic_message_status_st*)p_internal_msg->p_msg_data;
             p_sic_pdb->p_glb_pdb->p_glb_data->app_trace_info.app_id =
                p_sic_pdb->p_sic_ctx->app_id;

             trace_data.num_of_integer_values = ICF_THREE_INT_PRESENT;
 
             trace_data.int_val[0] =
                     (icf_uint32_t)(p_sic_pdb->p_sic_ctx)->line_id;
                
             trace_data.int_val[1] =
                            (icf_uint32_t)p_sic_msg_resp->resp_code;

             if(trace_data.int_val[1] >0)
              {
                  trace_data.int_val[2] = 1;
              }
             else
              {
                  trace_data.int_val[2] = 0;
              }
                 
             trace_data.number_of_string_values = ICF_NO_STRING_PRESENT;

             p_criteria_data->line_id =
                            p_sic_pdb->p_sic_ctx->line_id;
             p_criteria_data->bitmask |= ICF_TRACE_LINE_ID_PRESENT;
             
             break;

     case ICF_TRACE_INC_MESSAGE:
            p_sic_pdb->p_glb_pdb->p_glb_data->app_trace_info.app_id =
                p_sic_pdb->p_sic_ctx->app_id;
            
            trace_data.num_of_integer_values = ICF_ONE_INT_PRESENT;
 
            trace_data.int_val[0] =
                     (icf_uint32_t)(p_sic_pdb->p_sic_ctx)->line_id;

            trace_data.number_of_string_values = ICF_TWO_STRING_PRESENT;   

            icf_port_strcpy(trace_data.string_val[0].str,
                    p_sic_pdb->p_sic_ctx->dest_addr.addr_val.str);
            trace_data.string_val[0].str_len =
                (icf_uint16_t)icf_port_strlen(trace_data.string_val[0].str);

            icf_port_strcpy(trace_data.string_val[1].str,
                    p_sic_pdb->p_sic_ctx->p_user_address->addr_val.str);
            trace_data.string_val[1].str_len =
                (icf_uint16_t)icf_port_strlen(trace_data.string_val[1].str);
             
             p_criteria_data->line_id =
                            p_sic_pdb->p_sic_ctx->line_id;
             p_criteria_data->bitmask |= ICF_TRACE_LINE_ID_PRESENT;
             
             break;

      default:
            trace_data.num_of_integer_values = ICF_NO_INT_PRESENT;

            trace_data.number_of_string_values = ICF_NO_STRING_PRESENT;

            return ICF_SUCCESS;

    } /* end of switch-case for trace_id */

     /* Fix for CSR-1-5249829 */
      ret_val = icf_cmn_check_app_trace(p_sic_pdb->p_glb_pdb,
                                        trace_id,
					p_criteria_data);
      if( ICF_FAILURE == ret_val)
      {
          return ret_val;
      }

    /* Finally, invoke the common macro for trace handling */
    ICF_APP_TRACE(p_sic_pdb->p_glb_pdb, trace_id, &trace_data,
                  p_criteria_data)

    return ret_val;


} /* end of icf_sic_app_trace() */
#endif            

/*******************************************************************************
 *  
 * FUNCTION:        icf_sic_dealloc_context
 *
 * DESCRIPTION:     This function de-allocates the pointers in the SIC ctx
 *                  and then calls DBM to de-allocate the SIC ctx.
 * 
 *****************************************************************************/
icf_void_t icf_sic_dealloc_context(
        INOUT icf_sic_pdb_st  *p_sic_pdb)
{
    icf_return_t     ret_val = ICF_SUCCESS;
    icf_sic_ctx_st   *p_sic_ctx = ICF_NULL;
    
    ICF_FUNCTION_ENTER(p_sic_pdb->p_glb_pdb)

    p_sic_ctx = p_sic_pdb->p_sic_ctx;
    ICF_ASSERT(p_sic_pdb->p_glb_pdb, 
            (ICF_NULL != p_sic_ctx))

    
    
    
    /* Free the memory for the pointer to the user address */
    if (ICF_NULL != p_sic_pdb->p_sic_ctx->p_user_address)
    {
        ICF_MEMFREE(
                p_sic_pdb->p_glb_pdb,
                p_sic_pdb->p_sic_ctx->p_user_address,
                ICF_MEM_COMMON,
                p_sic_pdb->p_ecode,
                ret_val)
    }

    /* Free the memory for the pointer to the preferred identity */
    if (ICF_NULL != p_sic_pdb->p_sic_ctx->p_preferred_identity)
    {
        ICF_MEMFREE(
                p_sic_pdb->p_glb_pdb,
                p_sic_pdb->p_sic_ctx->p_preferred_identity,
                ICF_MEM_COMMON,
                p_sic_pdb->p_ecode,
                ret_val)
    }   

#ifdef ICF_TRACE_ENABLE
     /* Free the memory for the pointer to the p_trace_criteria_data*/
    if (ICF_NULL != p_sic_pdb->p_sic_ctx->p_trace_criteria_data)
    {
        ICF_MEMFREE(
                p_sic_pdb->p_glb_pdb,
                p_sic_pdb->p_sic_ctx->p_trace_criteria_data,
                ICF_MEM_COMMON,
                p_sic_pdb->p_ecode,
                ret_val)
    }   
#endif

 
    /* if the vms server address is present then */
   /* if (ICF_NULL != p_sic_pdb->p_sic_ctx->p_dest_addr)
    {
        ICF_SIC_TRACE(p_sic_pdb,ICF_TRACE_SIC_EVENT_PKG_RECD)

        ICF_MEMFREE(
            p_sic_pdb->p_glb_pdb,
            p_sic_pdb->p_sic_ctx->p_dest_addr,
            ICF_MEM_COMMON,
            p_sic_pdb->p_ecode,
            ret_val)        
    }*/
   	if ( ICF_NULL != p_sic_ctx->p_sdp_list)
	{
		icf_sic_free_sdp_list(p_sic_pdb);
        /* change for SPR14488 */
        p_sic_ctx->p_sdp_list = ICF_NULL;
	} 
    p_sic_ctx->event_buffer = ICF_FALSE;
    p_sic_ctx->line_id = ICF_INVALID_LINE_ID;
    p_sic_ctx->sic_state = ICF_SIC_00_IDLE;  
    p_sic_ctx->p_timer_data = ICF_NULL;
    p_sic_ctx->timer_type = ICF_NULL;
    p_sic_ctx->duration = ICF_NULL;
	p_sic_ctx->app_requested_duration = ICF_NULL;
    p_sic_ctx->event_package.str[0] = ICF_NULL;
    p_sic_ctx->timer_id = ICF_TIMER_ID_INVALID;
    p_sic_ctx->p_ssa_ctx = ICF_NULL;    
    
        
    p_sic_ctx->dest_addr.addr_type = 0;
    p_sic_ctx->dest_addr.addr_val.str_len = 0;
    p_sic_ctx->dest_addr.addr_val.str[0] = 0;
    p_sic_ctx->app_id = ICF_INVALID_APP_ID;
        p_sic_ctx->p_preferred_identity = ICF_NULL;
    p_sic_ctx->p_app_ctx = ICF_NULL;
    p_sic_ctx->p_call_ctx = ICF_NULL;

    if (ICF_NULL != p_sic_ctx->header_list.count)
    {
        icf_cmn_free_header_list(
        p_sic_pdb->p_glb_pdb,
        &(p_sic_ctx->header_list),
        p_sic_pdb->p_ecode);

    }
    p_sic_ctx->header_list.count = ICF_NULL;
    p_sic_ctx->header_list.hdr = ICF_NULL;

    if (ICF_NULL != p_sic_ctx->persistent_header_list.count)
    {
        icf_cmn_free_header_list(
        p_sic_pdb->p_glb_pdb,
        &(p_sic_ctx->persistent_header_list),
        p_sic_pdb->p_ecode);

    }
	p_sic_ctx->persistent_header_list.count = 0;
	p_sic_ctx->persistent_header_list.hdr = ICF_NULL;

    if (ICF_NULL != p_sic_ctx->tag_list.count)
    {
        icf_cmn_free_tag_list(
        p_sic_pdb->p_glb_pdb,
        &(p_sic_ctx->tag_list),
        p_sic_pdb->p_ecode);

    }
    p_sic_ctx->tag_list.count = ICF_NULL;
    p_sic_ctx->tag_list.tag = ICF_NULL;

    if (ICF_NULL != p_sic_ctx->body_list.count)
    {
        icf_cmn_free_msg_body_list(
        p_sic_pdb->p_glb_pdb,
        &(p_sic_ctx->body_list),
        p_sic_pdb->p_ecode);

    }
    p_sic_ctx->body_list.count = ICF_NULL;
    p_sic_ctx->body_list.body = ICF_NULL;
    
    p_sic_ctx->entity_tag.str[0] = 0;
    p_sic_ctx->entity_tag.str_len=0;

    icf_sic_free_mm_capabilities(p_sic_pdb,
		&(p_sic_pdb->p_sic_ctx->p_mm_media_capab));

    if ((ICF_NULL != p_sic_pdb->p_sic_ctx->p_options_timer) || 
        (ICF_TIMER_ID_INVALID != p_sic_pdb->p_sic_ctx->options_timer_id))
    {
         printf("break");
    }
   
    /*Fix for CSR 1-8223807 : SPR-20362*/
    /* Free the memory for the pointer to the p_remove_hdrs */
    if (ICF_NULL != p_sic_pdb->p_sic_ctx->p_remove_hdrs)
    {
        ICF_MEMFREE(
                p_sic_pdb->p_glb_pdb,
                p_sic_pdb->p_sic_ctx->p_remove_hdrs,
                ICF_MEM_COMMON,
                p_sic_pdb->p_ecode,
                ret_val)
    }
    
    /* Call DBM to deallocate the SIC ctx */
    ret_val = icf_dbm_dealloc_sic_cntxt_blk(p_sic_pdb->p_glb_pdb,
            p_sic_pdb->p_sic_ctx->call_id,
            p_sic_pdb->p_ecode);
    
    if (ICF_SUCCESS == ret_val)
    {
        p_sic_pdb->p_sic_ctx = ICF_NULL;
    }  
    ICF_FUNCTION_EXIT(p_sic_pdb->p_glb_pdb)
}



/*******************************************************************************
 *
 * FUNCTION:        icf_sic_send_subs_state_in_failure 
 *
 * DESCRIPTION:     This function sends the subscription state to the
 *                  corresponding module (MWI or REG) in case of failure.
 *                  Invalid line id will go in this case due to failure.
 *
 ******************************************************************************/
icf_return_t icf_sic_send_subs_state_in_failure(
        INOUT icf_sic_pdb_st      *p_sic_pdb)
{
    icf_return_t     ret_val = ICF_SUCCESS;
    icf_sic_subs_state_st subs_state_msg;
    icf_return_t        dummy_retval = ICF_SUCCESS;

    ICF_FUNCTION_ENTER(p_sic_pdb->p_glb_pdb)

    p_sic_pdb->p_internal_msg->msg_hdr.msg_id = 
            ICF_SIC_SUBSCRIPTION_STATUS;

    icf_port_memset(((icf_void_t *)(&subs_state_msg)), 0,
                ((icf_uint32_t)ICF_PORT_SIZEOF(icf_sic_subs_state_st )));

    if (ICF_SIC_EVENT_SUBS_REQ == p_sic_pdb->event)            
    {
        subs_state_msg.subs_state = ICF_SUBS_FAIL_NO_RETRY;
    }
    else 
    {
        subs_state_msg.subs_state = ICF_SUBS_UNSUBSCRIBED;        
    }

    /* Copy the event package from the SIC ctx */
    
    icf_port_memcpy(((icf_void_t *)(&(subs_state_msg.event_package))),
                ((icf_void_t *)(&(p_sic_pdb->event_package))), ICF_PORT_SIZEOF(icf_string_st));

    subs_state_msg.line_id = p_sic_pdb->p_sic_ctx->line_id;
    subs_state_msg.p_subs_key = p_sic_pdb->p_sic_ctx->p_app_ctx;
        
    p_sic_pdb->p_internal_msg->p_msg_data = 
            (icf_uint8_t *)&subs_state_msg;
    
    /* If event package is reg */
    if(
        icf_port_strcmp( p_sic_pdb->p_sic_ctx->event_package.str, (icf_uint8_t *)"reg") ==0 )
    {
       ret_val = icf_cfg_process_mesg(p_sic_pdb->p_internal_msg);
    }
    else
    {
        ret_val = icf_adm_map_sic_event_to_ext_event(p_sic_pdb);
        if( ICF_SUCCESS == ret_val)
        {
	    /*   Changes For REL 7.0 XCONF */
	
	    if(p_sic_pdb->p_sic_ctx->bitmask & ICF_SIC_INTERNAL_SUBSCRIBE_REQUEST)
	    {

		p_sic_pdb->p_internal_msg->msg_hdr.msg_id = ICF_INTERNAL_SUBSCRIBE_RESP;
		ret_val = icf_cm_process_mesg(p_sic_pdb->p_internal_msg);
	    }
	    /*  Changes For XCONF End */
	    else
	    {		
                ret_val = icf_adm_process_mesg(p_sic_pdb->p_internal_msg);
	    }
            
            if ( ICF_NULL != p_sic_pdb->p_internal_msg->p_msg_data)
            {
                icf_subscription_status_ind_st* p_subs_ind = (icf_subscription_status_ind_st*)
                    (((icf_msg_st*)(p_sic_pdb->p_internal_msg->p_msg_data))->payload);
                if ( ICF_NULL != p_subs_ind && 
                    ICF_NULL != p_subs_ind->header_list.count)
                {
                    icf_cmn_free_header_list(p_sic_pdb->p_glb_pdb,
                        &(p_subs_ind->header_list),
                        p_sic_pdb->p_ecode);
                }
            }
            ICF_MEMFREE(p_sic_pdb->p_glb_pdb, 
                p_sic_pdb->p_internal_msg->p_msg_data, ICF_MEM_COMMON,
                p_sic_pdb->p_ecode, dummy_retval)
        }
    }
    
    ICF_FUNCTION_EXIT(p_sic_pdb->p_glb_pdb)
    return ret_val;
}/* End function */

/*******************************************************************************
 *
 * FUNCTION:        icf_sic_unexpected_msg 
 *
 * DESCRIPTION:     This function is called when unexpected msg is received.
 *
 ******************************************************************************/
icf_return_t icf_sic_unexpected_msg(
        INOUT icf_sic_pdb_st    *p_sic_pdb)
{
    icf_return_t     ret_val = ICF_SUCCESS;
    
    ICF_FUNCTION_ENTER(p_sic_pdb->p_glb_pdb)
   
	ICF_SIC_RESET_BUFFER(p_sic_pdb);
    if(ICF_NULL != p_sic_pdb->p_sic_ctx->header_list.count)	
    {
         icf_cmn_free_header_list(p_sic_pdb->p_glb_pdb,
		&(p_sic_pdb->p_sic_ctx->header_list),
		p_sic_pdb->p_ecode);
    }
    /* Trace that unexpected msg is received */
    /*ICF_SIC_TRACE(p_sic_pdb,ICF_TRACE_SIC_UNEXPECTED_MSG)*/

    ICF_FUNCTION_EXIT(p_sic_pdb->p_glb_pdb)       
    return ret_val;
}

/*******************************************************************************
 *
 * FUNCTION:        icf_adm_map_sic_event_to_ext_event
 *
 * DESCRIPTION:     This function is use to map SIC API to external API.
 *
 ******************************************************************************/
icf_return_t icf_adm_map_sic_event_to_ext_event(
        icf_sic_pdb_st *p_sic_pdb)
{
    icf_msg_st            *p_app_msg=ICF_NULL;
    icf_uint32_t            msg_len=0;
    icf_internal_msg_st *p_internal_msg=ICF_NULL;
    icf_return_t            ret_val=ICF_SUCCESS;

    icf_sic_subs_state_st        *p_sic_subs_resp = ICF_NULL;
    icf_subscription_status_ind_st    *p_app_subs_resp = ICF_NULL;

    icf_sic_publish_status_st    *p_sic_pub_resp = ICF_NULL;
    icf_publish_resp_st                *p_app_pub_resp = ICF_NULL;

    icf_sic_message_status_st    *p_sic_msg_resp = ICF_NULL;
    icf_message_resp_st                *p_app_msg_resp = ICF_NULL;

    icf_sic_msg_ind_st            *p_sic_msg_ind = ICF_NULL;
    icf_message_ind_st                *p_app_msg_ind = ICF_NULL;

    icf_sic_refer_status_st        *p_sic_refer_resp = ICF_NULL;
    icf_refer_resp_st                *p_app_refer_resp = ICF_NULL;

    icf_sic_notify_msg_st        *p_sic_notify_ind = ICF_NULL;
    icf_notify_ind_st                *p_app_notify_ind = ICF_NULL;
#ifdef ICF_NW_ACTIVATE
    icf_boolean_t                   gprs_status = ICF_TRUE;
#endif

    icf_sic_options_status_st	 *p_sic_options_resp = ICF_NULL;
    icf_options_resp_st				*p_app_options_resp = ICF_NULL;

    p_internal_msg = p_sic_pdb->p_internal_msg;

    switch( p_internal_msg->msg_hdr.msg_id )
    {
        case ICF_SIC_SUBSCRIPTION_STATUS :
            {
                p_sic_subs_resp = (icf_sic_subs_state_st*)p_internal_msg->p_msg_data;

                msg_len = sizeof(icf_api_header_st) +
                    sizeof(icf_subscription_status_ind_st);

                /*Allocate memory for API header strucure */
                p_app_msg = (icf_msg_st*)icf_cmn_memget(
                            p_sic_pdb->p_glb_pdb,
                            msg_len, 
                            ICF_MEM_COMMON,
                            p_sic_pdb->p_ecode);
                if( ICF_NULL == p_app_msg)
                {
                    ret_val = ICF_FAILURE;
                }
                else
                {
                   /* Fill the message header for ICF_SUBSCRIBE_RESP */

                    p_app_msg->hdr.api_id = ICF_SUBSCRIPTION_STATUS_IND;
                    p_app_msg->hdr.app_id = p_sic_pdb->p_sic_ctx->app_id;
                    p_app_msg->hdr.api_length = (icf_uint16_t)msg_len;
            
                    p_app_msg->hdr.call_id = p_sic_subs_resp->call_id;

                    /* Fill the payload for ICF_SUBSCRIBE_RESP */
                    p_app_subs_resp = (icf_subscription_status_ind_st *)
                        (p_app_msg->payload);

                    /* Initialize the presence flag */
                    p_app_subs_resp->bit_mask = ICF_NULL;
                    p_app_subs_resp->duration = p_sic_subs_resp->duration;

                    if(p_sic_pdb->p_sic_ctx->error_code)
                    {
                        p_app_subs_resp->bit_mask |= ICF_SUBS_ST_IND_ERROR_CODE_PRESENT;
                        p_app_subs_resp->error_code = p_sic_pdb->p_sic_ctx->error_code;
                        p_sic_pdb->p_sic_ctx->error_code = ICF_NULL;
                    }			

                    if( p_sic_subs_resp->resp_code )
                    {
                        p_app_subs_resp->bit_mask |= ICF_SUBS_ST_IND_RESP_CODE_PRESENT;
                        p_app_subs_resp->response_code = p_sic_subs_resp->resp_code;
                    }

#ifdef ICF_NW_ACTIVATE
                    else if ((ICF_FAILURE != icf_cmn_chk_gprs_status(
							p_sic_pdb->p_glb_pdb, &gprs_status, 
							p_sic_pdb->p_ecode)) &&
							(ICF_FALSE == gprs_status))
		    {
			/* because response code was NULL, SSA
			 * indicated Timeout; thus GPRS status checked */
			p_app_subs_resp->error_code =
		    	    ICF_CAUSE_GPRS_SUSPENDED;
			p_app_subs_resp->bit_mask |=
			    ICF_SUBS_ST_IND_ERROR_CODE_PRESENT;
		    }
#endif

    /*              if( 200 == p_app_subs_resp->response_code )*/
    /*              p_app_subs_resp->result = ICF_SUCCESS;*/
    /*              else*/
    /*              p_app_subs_resp->result = ICF_FAILURE;*/

                    p_app_subs_resp->status = p_sic_subs_resp->subs_state;

                    if( p_sic_subs_resp->header_list.count )
                    {
                        icf_cmn_clone_header_list(
                            p_sic_pdb->p_glb_pdb,
                            &(p_sic_subs_resp->header_list),
                            &(p_app_subs_resp->header_list),
                            p_sic_pdb->p_ecode );
                        /*free the header list source header list*/ 
                        icf_cmn_free_header_list(p_sic_pdb->p_glb_pdb,
                            &(p_sic_subs_resp->header_list),
                            p_sic_pdb->p_ecode);
                        p_app_subs_resp->bit_mask |= ICF_HDR_LIST_PRESENT;
                    }
                    else
                        p_app_subs_resp->header_list.count=0;

					/* Fix for CSR 1-8420460: SPR 20343 */
					/* Event package name for which subscribe request sent */
					icf_port_strcpy(
						p_app_subs_resp->event_pkg_name.str,
						p_sic_subs_resp->event_package.str);
					p_app_subs_resp->event_pkg_name.str_len = 
						icf_port_strlen(p_app_subs_resp->event_pkg_name.str);
                }
                 
                break;
            }
        case ICF_SIC_MESSAGE_STATUS :
            {
                p_sic_msg_resp = (icf_sic_message_status_st*)p_internal_msg->p_msg_data;

                msg_len = sizeof(icf_api_header_st) +
                    sizeof(icf_message_resp_st);

                /*Allocate memory for API header strucure */
                p_app_msg = (icf_msg_st*)icf_cmn_memget(
                            p_sic_pdb->p_glb_pdb,
                            msg_len, 
                            ICF_MEM_COMMON,
                            p_sic_pdb->p_ecode);
                if( ICF_NULL == p_app_msg)
                {
                    return ICF_FAILURE;
                }
                else
                {
                   /* Fill the message header for ICF_MESSAGE_RESP */

                    p_app_msg->hdr.api_id = ICF_MESSAGE_RESP;
                    p_app_msg->hdr.app_id = p_sic_pdb->p_sic_ctx->app_id;
                    p_app_msg->hdr.api_length = (icf_uint16_t)msg_len;
            
                    p_app_msg->hdr.call_id = p_sic_msg_resp->call_id;

                    /* Fill the payload for  ICF_MESSAGE_RESP*/
                    p_app_msg_resp = (icf_message_resp_st *)
                        (p_app_msg->payload);

                    /* Initialize the presence flag */
                    p_app_msg_resp->bit_mask = ICF_NULL;

                    p_app_msg_resp->response_code = p_sic_msg_resp->resp_code;

                    if (0 != p_sic_pdb->p_sic_ctx->retry_after_duration)
                    {
                        p_app_msg_resp->retry_after_duration =
                             p_sic_pdb->p_sic_ctx->retry_after_duration;
                        p_sic_pdb->p_sic_ctx->retry_after_duration = 0;
                    }
#ifdef ICF_NW_ACTIAVTE
                    if ((ICF_NULL == p_app_msg_resp->response_code) &&
			(ICF_FAILURE != icf_cmn_chk_gprs_status(
			p_sic_pdb->p_glb_pdb, &gprs_status, 
			p_sic_pdb->p_ecode)) &&
			(ICF_FALSE == gprs_status))
        	    {
			/* SSA indicates the timeout as response
		         * code = NULL */
			p_app_msg_resp->result = ICF_FAILURE;
			p_app_msg_resp->error_code = 
                            ICF_CAUSE_GPRS_SUSPENDED;
                        p_app_msg_resp->bit_mask |= 
                            ICF_MSG_RESP_ERROR_CODE_PRESENT;

		    }
                    else
#endif
                    {
			if( p_app_msg_resp->response_code )
			{
		            p_app_msg_resp->bit_mask |= ICF_MSG_RESP_RESP_CODE_PRESENT;
                        
			    if( 299 >= p_app_msg_resp->response_code )
			    {
			        p_app_msg_resp->result = ICF_SUCCESS;
			    }
			    else
			    {
				p_app_msg_resp->result = ICF_FAILURE;
			    }
			}
			else
			{
			    p_app_msg_resp->result = ICF_FAILURE;
			    /* currently, this will happen only in one scenario: When no 
			     * response is recd from network and MESSAGE req times out */
			    
                            if(p_sic_pdb->p_sic_ctx->error_code)
                            {
                                p_app_msg_resp->bit_mask |= ICF_MSG_RESP_ERROR_CODE_PRESENT;
                                p_app_msg_resp->error_code = p_sic_pdb->p_sic_ctx->error_code;
                                p_sic_pdb->p_sic_ctx->error_code = ICF_NULL;
                            }		
			}
                    }

                    if( p_sic_msg_resp->header_list.count )
                    {
                        icf_cmn_clone_header_list(
                            p_sic_pdb->p_glb_pdb,
                            &(p_sic_msg_resp->header_list),
                            &(p_app_msg_resp->header_list),
                            p_sic_pdb->p_ecode );
                        icf_cmn_free_header_list(p_sic_pdb->p_glb_pdb,
                            &(p_sic_msg_resp->header_list),
                            p_sic_pdb->p_ecode);

                        p_app_msg_resp->bit_mask |= ICF_HDR_LIST_PRESENT;
                    }
                    else
                        p_app_msg_resp->header_list.count=0;
                }
                
                break;
            }
	case ICF_SIC_OPTION_STATUS :
            {
                p_sic_options_resp = 
                    (icf_sic_options_status_st*)p_internal_msg->p_msg_data;
                /* reset the bit-mask for the in-dialog sent option */
                if(ICF_NULL!= p_sic_pdb->p_sic_ctx->p_call_ctx &&
                p_sic_pdb->p_sic_ctx->p_call_ctx->common_bitmask & 
                    ICF_IN_DLG_OPTIONS_SEND)
                {
                    p_sic_pdb->p_sic_ctx->p_call_ctx->common_bitmask &= 
                        ~ICF_IN_DLG_OPTIONS_SEND;

                    if(ICF_CAUSE_REMOTE_UNREACHABLE == 
                           p_sic_pdb->p_sic_ctx->error_code)
                    {
                        p_internal_msg->msg_hdr.msg_id = 
                            ICF_GENERIC_CALL_CLEAR;
                        p_internal_msg->p_msg_data = ICF_NULL;
                        p_internal_msg->msg_hdr.payload_length = 0;
                        p_internal_msg->msg_hdr.p_glb_pdb = 
                            p_sic_pdb->p_glb_pdb;
                        icf_cc_process_mesg(p_internal_msg);
                    }
                }

			msg_len = sizeof(icf_api_header_st) +
				sizeof(icf_options_resp_st);

			/*Allocate memory for API header strucure */
			p_app_msg = (icf_msg_st*)icf_cmn_memget(
						p_sic_pdb->p_glb_pdb,
						msg_len, 
						ICF_MEM_COMMON,
						p_sic_pdb->p_ecode);
			if( ICF_NULL == p_app_msg)
			{
				return ICF_FAILURE;
			}
			else
			{
				/* Fill the message header for ICF_OPTIONS_RESP */

				p_app_msg->hdr.api_id = ICF_OPTIONS_RESP;
				p_app_msg->hdr.app_id = p_sic_pdb->p_sic_ctx->app_id;
				p_app_msg->hdr.api_length = (icf_uint16_t)msg_len;

				p_app_msg->hdr.call_id = p_sic_options_resp->call_id;

				/* Fill the payload for  ICF_OPTIONS_RESP*/
				p_app_options_resp = (icf_options_resp_st *)
					(p_app_msg->payload);

				/* Initialize the presence flag */
				p_app_options_resp->bit_mask = ICF_NULL;

				p_app_options_resp->response_code_for_options = 
					p_sic_options_resp->resp_code;

				p_app_options_resp->p_remote_stream_list = ICF_NULL;

#ifdef ICF_NW_ACTIVATE
                    if ((ICF_NULL == p_app_options_resp->response_code_for_options) &&
			(ICF_FAILURE != icf_cmn_chk_gprs_status(
			p_sic_pdb->p_glb_pdb, &gprs_status, 
			p_sic_pdb->p_ecode)) &&
			(ICF_FALSE == gprs_status))
		    {
			/* SSA indicates the timeout as response
			 * code = NULL */
			p_app_options_resp->result = ICF_FAILURE;
			p_app_options_resp->error_code_for_options = 
                            ICF_CAUSE_GPRS_SUSPENDED;
                        p_app_options_resp->bit_mask |= 
                            ICF_OPTIONS_ERROR_CODE_PRESENT;

		    }
                    else
#endif
                    {
                        if( p_app_options_resp->response_code_for_options )
                        {
                            p_app_options_resp->bit_mask |= 
                                ICF_OPTIONS_RESP_CODE_PRESENT;
                        
                            if( 299 >= p_app_options_resp->\
                                          response_code_for_options )
                            {
                                p_app_options_resp->result = ICF_SUCCESS;
                            }
                            else
                            {
                                p_app_options_resp->result = ICF_FAILURE;
                                /* Start SPR14524 */
                                if(p_sic_pdb->p_sic_ctx->error_code)
                                {
                                    p_app_options_resp->bit_mask |= 
                                        ICF_OPTIONS_ERROR_CODE_PRESENT;
                                    p_app_options_resp->\
                                        error_code_for_options =
                                    (icf_uint16_t)p_sic_pdb->p_sic_ctx->error_code;
                                    /* reset the error code */
                                    p_sic_pdb->p_sic_ctx->error_code = 
                                        ICF_NULL;
                                }
                                /* End SPR14524 */
                            }
                        }
                        else
                        {
                            p_app_options_resp->result = ICF_FAILURE;
                            /* currently, this will happen only in one 
                              scenario: When no response is recd from 
                              network and OPTIONS req times out */
							
                            if(p_sic_pdb->p_sic_ctx->error_code)
                            {
                                p_app_options_resp->bit_mask |= 
                                    ICF_OPTIONS_ERROR_CODE_PRESENT;
                                p_app_options_resp->error_code_for_options = 
                                    (icf_uint16_t)p_sic_pdb->p_sic_ctx->error_code;

                                /* reset the error code */
                                p_sic_pdb->p_sic_ctx->error_code = ICF_NULL;
                            }
                        }
                    }

                    /* clone header list if present*/
                    if( p_sic_options_resp->header_list.count )
                    {
                        icf_cmn_clone_header_list(
                            p_sic_pdb->p_glb_pdb,
                            &(p_sic_options_resp->header_list),
                            &(p_app_options_resp->header_list),
                            p_sic_pdb->p_ecode );
                        icf_cmn_free_header_list(p_sic_pdb->p_glb_pdb,
                            &(p_sic_options_resp->header_list),
                            p_sic_pdb->p_ecode);

					p_app_options_resp->bit_mask |= ICF_HDR_LIST_PRESENT;
				}
				else
	            {
					p_app_options_resp->header_list.count=0;
					p_app_options_resp->header_list.hdr = ICF_NULL;
				}

				/* clone body list if present*/
				if( p_sic_options_resp->body_list.count )
				{
					icf_cmn_clone_body_list(
						p_sic_pdb->p_glb_pdb,
						&(p_sic_options_resp->body_list),
						&(p_app_options_resp->body_list),
						p_sic_pdb->p_ecode );
					icf_cmn_free_msg_body_list(p_sic_pdb->p_glb_pdb,
						&(p_sic_options_resp->body_list),
						p_sic_pdb->p_ecode);
					p_app_options_resp->bit_mask |= ICF_BODY_LIST_PRESENT;
				}
				else
				{
					p_app_options_resp->body_list.count=0;
					p_app_options_resp->body_list.body = ICF_NULL;
				}

                    /* clone stream list if present*/
                    ret_val = icf_sic_create_app_media_data(p_sic_pdb);
                   /* Added p_app_options_resp->result check SPR14524 */ 
                    if(p_app_options_resp->result == ICF_SUCCESS)
                     {
                       if(ICF_SUCCESS == ret_val)
                        {
		                  /*assign the values in the message data from*/
                          /*p_sic_pdb */
                          p_app_options_resp->p_remote_stream_list = p_sic_pdb->p_list_1;
		
						p_app_options_resp->bit_mask |= ICF_OPTIONS_REMOTE_STREAM_LIST_PRESENT;
					}
				}

				/*Fill header in internal message structure*/
				p_sic_pdb->p_internal_msg->msg_hdr.msg_id = ICF_OPTIONS_RESP;

				p_sic_pdb->p_internal_msg->msg_hdr.payload_length = (icf_uint8_t)msg_len;

				/*Assign p_pa_msg to p_pa_msg_data of icf_internal_msg_st*/
				p_sic_pdb->p_internal_msg->p_msg_data = (icf_uint8_t*)p_app_msg;

			}
			break;
		}
        case ICF_SIC_NEW_MESSAGE_IND :
            {
                p_sic_msg_ind = (icf_sic_msg_ind_st*)p_internal_msg->p_msg_data;

                msg_len = sizeof(icf_api_header_st) +
                    sizeof(icf_message_ind_st);

                /*Allocate memory for API header strucure */
                p_app_msg = (icf_msg_st*)icf_cmn_memget(
                            p_sic_pdb->p_glb_pdb,
                            msg_len, 
                            ICF_MEM_COMMON,
                            p_sic_pdb->p_ecode);
                if( ICF_NULL == p_app_msg)
                {
                    ret_val = ICF_FAILURE;
                }
                else
                {
                   /* Fill the message header for ICF_MESSAGE_IND */

                    p_app_msg->hdr.api_id = ICF_MESSAGE_IND;
                    p_app_msg->hdr.app_id = p_sic_pdb->p_sic_ctx->app_id;
                    p_app_msg->hdr.api_length = (icf_uint16_t)msg_len;
            
                    p_app_msg->hdr.call_id = p_sic_msg_ind->call_id;

                    /* Fill the payload for  ICF_MESSAGE_RESP*/
                    p_app_msg_ind = (icf_message_ind_st *)
                        (p_app_msg->payload);

                    /* Initialize the presence flag */
                    p_app_msg_ind->bit_mask = ICF_NULL;

                    if(p_sic_pdb->p_sic_ctx->dest_addr.addr_type != ICF_ADDRESS_PLAN_UNDEFINED)
                    {
                        p_app_msg_ind->bit_mask |= ICF_MSG_IND_SENDER_PRESENT;
                        icf_port_memcpy(
                                (icf_void_t *)&(p_app_msg_ind->sender),
                                (icf_void_t *)&(p_sic_pdb->p_sic_ctx->dest_addr),
                                (icf_uint32_t)ICF_PORT_SIZEOF(icf_address_st));
                    }

                    if( p_sic_msg_ind->header_list.count )
                    {
                        icf_cmn_clone_header_list(
                            p_sic_pdb->p_glb_pdb,
                            &(p_sic_msg_ind->header_list),
                            &(p_app_msg_ind->header_list),
                            p_sic_pdb->p_ecode );
                        icf_cmn_free_header_list(p_sic_pdb->p_glb_pdb,
                            &(p_sic_msg_ind->header_list),
                            p_sic_pdb->p_ecode);

                        p_app_msg_ind->bit_mask |= ICF_HDR_LIST_PRESENT;
                    }
                    else
                        p_app_msg_ind->header_list.count=0;

                    if( p_sic_msg_ind->body_list.count )
                    {
                        icf_cmn_clone_body_list(
                            p_sic_pdb->p_glb_pdb,
                            &(p_sic_msg_ind->body_list),
                            &(p_app_msg_ind->body_list),
                            p_sic_pdb->p_ecode );
                        icf_cmn_free_msg_body_list(p_sic_pdb->p_glb_pdb,
                            &(p_sic_msg_ind->body_list),
                            p_sic_pdb->p_ecode);
                        p_app_msg_ind->bit_mask |= ICF_BODY_LIST_PRESENT;
                    }
                    else
                        p_app_msg_ind->body_list.count=0;
                }
                break;
            }
        case ICF_SIC_REFER_STATUS :
            {
                p_sic_refer_resp = (icf_sic_refer_status_st*)p_internal_msg->p_msg_data;

                msg_len = sizeof(icf_api_header_st) +
                    sizeof(icf_refer_resp_st);

                /*Allocate memory for API header strucure */
                p_app_msg = (icf_msg_st*)icf_cmn_memget(
                            p_sic_pdb->p_glb_pdb,
                            msg_len, 
                            ICF_MEM_COMMON,
                            p_sic_pdb->p_ecode);
                if( ICF_NULL == p_app_msg)
                {
                    ret_val = ICF_FAILURE;
                }
                else
                {
                   /* Fill the message header for ICF_MESSAGE_RESP */

                    p_app_msg->hdr.api_id = ICF_REFER_RESP;
                    p_app_msg->hdr.app_id = p_sic_pdb->p_sic_ctx->app_id;
                    p_app_msg->hdr.api_length = (icf_uint16_t)msg_len;
            
                    p_app_msg->hdr.call_id = p_sic_refer_resp->call_id;

                    /* Fill the payload for  ICF_MESSAGE_RESP*/
                    p_app_refer_resp = (icf_refer_resp_st *)
                        (p_app_msg->payload);

                    /* Initialize the presence flag */
                    p_app_refer_resp->bit_mask = ICF_NULL;
                    p_app_refer_resp->response_code = p_sic_refer_resp->resp_code;
                    
                    if (0 != p_sic_pdb->p_sic_ctx->retry_after_duration)
                    {
                         p_app_refer_resp->retry_after_duration =
                             p_sic_pdb->p_sic_ctx->retry_after_duration;
                         p_sic_pdb->p_sic_ctx->retry_after_duration = 0;
                    }

#ifdef ICF_NW_ACTIAVTE
                    if ((ICF_NULL == p_app_refer_resp->response_code) &&
			(ICF_FAILURE != icf_cmn_chk_gprs_status(
			p_sic_pdb->p_glb_pdb, &gprs_status, 
			p_sic_pdb->p_ecode)) &&
			(ICF_FALSE == gprs_status))
		    {
		        /* SSA indicates the timeout as response
			 * code = NULL */
			p_app_refer_resp->result = ICF_FAILURE;
			p_app_refer_resp->error_code = 
                            ICF_CAUSE_GPRS_SUSPENDED;
			p_app_refer_resp->bit_mask |= 
                            ICF_REF_RESP_ERROR_CODE_PRESENT;
                    }
                    else
#endif
                    {
                        if( p_app_refer_resp->response_code )
                        {
                            p_app_refer_resp->bit_mask |= ICF_REF_RESP_RESP_CODE_PRESENT;

                            if( 299 >= p_app_refer_resp->response_code )
                            {
                                p_app_refer_resp->result = ICF_SUCCESS;
                            }
                            else
                            {
                                p_app_refer_resp->result = ICF_FAILURE;
                            }
                        }
                        else
                        {
                            p_app_refer_resp->result = ICF_FAILURE;
                            /* currently, this will happen only in one scenario: When no 
                             * response is recd from network and REFER req times out */
                            if(p_sic_pdb->p_sic_ctx->error_code)
                            {	
                                p_app_refer_resp->bit_mask |= ICF_REF_RESP_ERROR_CODE_PRESENT;
                                p_app_refer_resp->error_code = p_sic_pdb->p_sic_ctx->error_code;
                                /* reset the error code */
                                p_sic_pdb->p_sic_ctx->error_code = ICF_NULL;
                            }
                        }
                    }
   
                    if( p_sic_refer_resp->header_list.count )
                    {
                        icf_cmn_clone_header_list(
                            p_sic_pdb->p_glb_pdb,
                            &(p_sic_refer_resp->header_list),
                            &(p_app_refer_resp->header_list),
                            p_sic_pdb->p_ecode );
                        icf_cmn_free_header_list(p_sic_pdb->p_glb_pdb,
                            &(p_sic_refer_resp->header_list),
                            p_sic_pdb->p_ecode);

                        p_app_refer_resp->bit_mask |= ICF_HDR_LIST_PRESENT;
                    }
                    else
                        p_app_refer_resp->header_list.count=0;
                }
            break;
            }
        case ICF_SIC_PUBLISH_STATUS :
            {
                p_sic_pub_resp = (icf_sic_publish_status_st*)p_internal_msg->p_msg_data;


                msg_len = sizeof(icf_api_header_st) +
                    sizeof(icf_publish_resp_st);

                /*Allocate memory for API header strucure */
                p_app_msg = (icf_msg_st*)icf_cmn_memget(
                            p_sic_pdb->p_glb_pdb,
                            msg_len, 
                            ICF_MEM_COMMON,
                            p_sic_pdb->p_ecode);
                if( ICF_NULL == p_app_msg)
                {
                    ret_val = ICF_FAILURE;
                }
                else
                {
                   /* Fill the message header for ICF_PUBLISH_RESP */

                    p_app_msg->hdr.api_id = ICF_PUBLISH_RESP;
                    p_app_msg->hdr.app_id = p_sic_pdb->p_sic_ctx->app_id;
                    p_app_msg->hdr.api_length = (icf_uint16_t)msg_len;
            
                    p_app_msg->hdr.call_id = p_sic_pub_resp->call_id;

                    /* Fill the payload for ICF_PUBLISH_RESP */
                    p_app_pub_resp = (icf_publish_resp_st *)
                        (p_app_msg->payload);

                    /* Initialize the presence flag */
                    p_app_pub_resp->bit_mask = ICF_NULL;

                    p_app_pub_resp->duration = p_sic_pub_resp->duration;
                    p_app_pub_resp->response_code = p_sic_pub_resp->resp_code;

                    if (0 != p_sic_pdb->p_sic_ctx->retry_after_duration)
                    {
                         p_app_pub_resp->retry_after_duration =
                             p_sic_pdb->p_sic_ctx->retry_after_duration;
                         p_sic_pdb->p_sic_ctx->retry_after_duration = 0;
                    }

#ifdef ICF_NW_ACTIVATE
                    if ((ICF_NULL == p_app_pub_resp->response_code) &&
						(ICF_FAILURE != icf_cmn_chk_gprs_status(
						p_sic_pdb->p_glb_pdb, &gprs_status, 
						p_sic_pdb->p_ecode)) &&
						(ICF_FALSE == gprs_status))
                    {
			/* SSA indicates the timeout as response
			* code = NULL */
			p_app_pub_resp->result = ICF_FAILURE;
			p_app_pub_resp->error_code = 
		            ICF_CAUSE_GPRS_SUSPENDED;
			p_app_pub_resp->bit_mask |= 
			    ICF_PUB_RESP_ERROR_CODE_PRESENT;
		    }
		    else
#endif
                    {
			if(p_app_pub_resp->response_code)
			{
		  	    p_app_pub_resp->bit_mask |= ICF_PUB_RESP_RESP_CODE_PRESENT;
					
			    if( 299 >= p_app_pub_resp->response_code )
			    {
				p_app_pub_resp->result = ICF_SUCCESS;

                                if(p_sic_pdb->p_sic_ctx->entity_tag.str_len)
                                {
                                    icf_port_strcpy( p_app_pub_resp->entity_tag.str, 
                                    p_sic_pdb->p_sic_ctx->entity_tag.str);
                                    p_app_pub_resp->entity_tag.str_len = (icf_uint16_t)icf_port_strlen(
                                    p_app_pub_resp->entity_tag.str);
                                }
                                else
                                {
                                    /* SIP-ETag header is mandatory in Publish resp */
			            p_app_pub_resp->result = ICF_FAILURE;
			      	    p_app_pub_resp->bit_mask |= ICF_PUB_RESP_ERROR_CODE_PRESENT;
			            p_app_pub_resp->error_code = ICF_CAUSE_MANDATORY_HDR_NOT_RCVD;
                                }
			    }
			    else
			    {
				p_app_pub_resp->result = ICF_FAILURE;
			    }
			}
			else
			{
			    p_app_pub_resp->result = ICF_FAILURE;
			    /* currently, this will happen only in one scenario: When no 
			     * response is recd from network and PUBLISH req times out */
			    if(p_sic_pdb->p_sic_ctx->error_code)
                            {
           	                p_app_pub_resp->bit_mask |= ICF_PUB_RESP_ERROR_CODE_PRESENT;
    				p_app_pub_resp->error_code = p_sic_pdb->p_sic_ctx->error_code;
    				/* reset the error code */
    				p_sic_pdb->p_sic_ctx->error_code = ICF_NULL;
                            }
			}
                    }

                    if( p_sic_pub_resp->header_list.count )
                    {
                        icf_cmn_clone_header_list(
                            p_sic_pdb->p_glb_pdb,
                            &(p_sic_pub_resp->header_list),
                            &(p_app_pub_resp->header_list),
                            p_sic_pdb->p_ecode );
                        icf_cmn_free_header_list(p_sic_pdb->p_glb_pdb,
                            &(p_sic_pub_resp->header_list),
                            p_sic_pdb->p_ecode);
                        p_app_pub_resp->bit_mask |= ICF_HDR_LIST_PRESENT;
                    }
                    else
                        p_app_pub_resp->header_list.count=0;
                }
                break;
            }
        case ICF_SIC_NOTIFY_IND :
            {
                p_sic_notify_ind = (icf_sic_notify_msg_st*)p_internal_msg->p_msg_data;

                msg_len = sizeof(icf_api_header_st) +
                    sizeof(icf_notify_ind_st);

                /*Allocate memory for API header strucure */
                p_app_msg = (icf_msg_st*)icf_cmn_memget(
                            p_sic_pdb->p_glb_pdb,
                            msg_len, 
                            ICF_MEM_COMMON,
                            p_sic_pdb->p_ecode);
                if( ICF_NULL == p_app_msg)
                {
                    ret_val = ICF_FAILURE;
                }
                else
                {
                   /* Fill the message header for ICF_MESSAGE_IND */

                    p_app_msg->hdr.api_id = ICF_NOTIFY_IND;
                    p_app_msg->hdr.app_id = p_sic_pdb->p_sic_ctx->app_id;
                    p_app_msg->hdr.api_length = (icf_uint16_t)msg_len;
            
                    p_app_msg->hdr.call_id = p_sic_notify_ind->call_id;

                    /* Fill the payload for  ICF_MESSAGE_RESP*/
                    p_app_notify_ind = (icf_notify_ind_st *)
                        (p_app_msg->payload);

                    /* Initialize the presence flag */
                    p_app_notify_ind->bit_mask = ICF_NULL;

                    p_app_notify_ind->line_id = p_sic_pdb->p_sic_ctx->line_id;

    /*                p_app_notify_ind->duration = p_sic_notify_ind->duration;*/

                    p_app_notify_ind->event_pkg_name.str_len = 
                        (icf_uint16_t)icf_port_strlen( p_sic_notify_ind->event_pkg.str );
                    icf_port_strcpy(p_app_notify_ind->event_pkg_name.str,
                        p_sic_notify_ind->event_pkg.str);

                    /*switch (p_sic_notify_ind->event_pkg)
                    {
                        case ICF_EVENT_PKG_MESSAGE_SUMMARY:
                            icf_port_strcpy(p_app_notify_ind->event_pkg_name.str,"message-summary");
                            p_app_notify_ind->event_pkg_name.str_len = icf_port_strlen("message-summary");

                            break;

                        case ICF_EVENT_PKG_REG:
                            icf_port_strcpy(p_app_notify_ind->event_pkg_name.str,"reg");
                            p_app_notify_ind->event_pkg_name.str_len = icf_port_strlen("reg");
           
                            break;

                        case ICF_EVENT_PKG_CONFERENCE:
                            icf_port_strcpy(p_app_notify_ind->event_pkg_name.str,"conference");
                            p_app_notify_ind->event_pkg_name.str_len = icf_port_strlen("conference");
           
                            break;

                        case ICF_EVENT_PKG_REFER:
                            icf_port_strcpy(p_app_notify_ind->event_pkg_name.str,"refer");
                            p_app_notify_ind->event_pkg_name.str_len = icf_port_strlen("refer");
           
                            break;

                    }*/

/*NOTIFY_OUT_OF_DIALOG_START*/
					p_app_notify_ind->subs_state = p_sic_notify_ind->subs_state;
					if(!p_sic_notify_ind->subs_state)
						p_app_notify_ind->reason_code = p_sic_notify_ind->reason_code;
					else
						p_app_notify_ind->reason_code = 0;


					if(p_sic_notify_ind->duration)
					{
						p_app_notify_ind->duration = p_sic_notify_ind->duration;
						p_app_notify_ind->bit_mask |= ICF_SUBS_DURATION_PRESENT;
					}
					else
					{
						p_app_notify_ind->duration = 0;
					}

					if(p_sic_notify_ind->p_sender_address)
					{
						icf_port_memcpy(&(p_app_notify_ind->sender),
							p_sic_notify_ind->p_sender_address,sizeof(icf_address_st));
						p_app_notify_ind->bit_mask |= ICF_NOTIFY_IND_SENDER_PRESENT;
					}

					if(p_sic_notify_ind->p_user_address)
					{
						icf_port_memcpy(&(p_app_notify_ind->user_address),
							p_sic_notify_ind->p_user_address,sizeof(icf_address_st));
						p_app_notify_ind->bit_mask |= ICF_NOTIFY_IND_USER_ADDR_PRESENT;
					}

					if(ICF_TRUE == p_sic_notify_ind->out_of_dlg_mesg)
					{
						p_app_notify_ind->bit_mask |= ICF_NOTIFY_IND_OUT_OF_DIALOG;
					}
                    
                                        if(p_sic_pdb->p_sic_ctx->bitmask &
                                                     ICF_SIC_METHOD_TYPE_STRAY_NOTIFY)
                                        {
                                            p_app_notify_ind->bit_mask |= ICF_NOTIFY_IND_STRAY;
                                            p_sic_pdb->p_sic_ctx->bitmask &= 
                                                    ~ICF_SIC_METHOD_TYPE_STRAY_NOTIFY;
                                        }

                    if( p_sic_notify_ind->header_list.count )
                    {
                        icf_cmn_clone_header_list(
                            p_sic_pdb->p_glb_pdb,
                            &(p_sic_notify_ind->header_list),
                            &(p_app_notify_ind->header_list),
                            p_sic_pdb->p_ecode );
                        icf_cmn_free_header_list(p_sic_pdb->p_glb_pdb,
                            &(p_sic_notify_ind->header_list),
                            p_sic_pdb->p_ecode);

                        p_app_notify_ind->bit_mask |= ICF_HDR_LIST_PRESENT;
                    }
                    else
                        p_app_notify_ind->header_list.count=0;

                    if( p_sic_notify_ind->body_list.count )
                    {
                        icf_cmn_clone_body_list(
                            p_sic_pdb->p_glb_pdb,
                            &(p_sic_notify_ind->body_list),
                            &(p_app_notify_ind->body_list),
                            p_sic_pdb->p_ecode );
                        icf_cmn_free_msg_body_list(p_sic_pdb->p_glb_pdb,
                            &(p_sic_notify_ind->body_list),
                            p_sic_pdb->p_ecode);
                        p_app_notify_ind->bit_mask |= ICF_BODY_LIST_PRESENT;
                    }
                    else
                        p_app_notify_ind->body_list.count=0;
                }

                break;
          }
        default:
                ret_val = ICF_FAILURE;
                break;
        
    }
    if( ICF_SUCCESS == ret_val)
    {
        p_sic_pdb->p_internal_msg->msg_hdr.msg_id = p_app_msg->hdr.api_id;
        p_sic_pdb->p_internal_msg->p_msg_data = (icf_uint8_t*)p_app_msg;
    }
    return ret_val;

}

/*******************************************************************************
 *
 * FUNCTION:        icf_sic_refresh_sic_ctx 
 *
 * DESCRIPTION:     This function is called to refresh the SIC ctx incase it 
 *                    has received a SUBSCRIBE request for an already existing a SIC ctx.
 *
 ******************************************************************************/
icf_return_t icf_sic_refresh_sic_ctx(
        INOUT icf_sic_pdb_st    *p_sic_pdb)
{
    icf_return_t                    ret_val = ICF_SUCCESS;
    icf_sic_subs_req_st            *p_subs_req = ICF_NULL;
    icf_sic_ctx_st                *p_sic_ctx = ICF_NULL;

    p_subs_req = ((icf_sic_subs_req_st *)(p_sic_pdb->p_msg));
    p_sic_ctx = p_sic_pdb->p_sic_ctx;

    if (ICF_SIC_EVENT_SUBS_REQ != p_sic_pdb->event)
        return ICF_SUCCESS;

    if (!(icf_port_strcmp(p_sic_ctx->event_package.str, p_subs_req->event_package.str)))
    {
        /* request->event_pkg same as sic_ctx->event_pkg: starts  */
        
        
        if (ICF_ADDRESS_PLAN_UNDEFINED != p_subs_req->destination_info.addr_type)
        {
            icf_port_memset((icf_void_t*)&(p_sic_pdb->p_sic_ctx->dest_addr), 0,
                (icf_uint32_t) ICF_PORT_SIZEOF(icf_address_st));
            icf_port_memcpy( 
                (icf_void_t*)&(p_sic_ctx->dest_addr),
                (icf_void_t*) &(p_subs_req->destination_info),
                (icf_uint32_t) ICF_PORT_SIZEOF(icf_address_st));
            
        }/* the received request has a destination address */        
        
        
        p_sic_pdb->line_id = p_subs_req->line_id;
        p_sic_ctx->line_id = p_subs_req->line_id;        
        p_sic_ctx->call_id = p_subs_req->call_id;        
        p_sic_ctx->app_id = p_subs_req->app_id;        
        p_sic_ctx->p_app_ctx = p_subs_req->p_key;        
        p_sic_ctx->duration = p_subs_req->duration;
        
        if(p_subs_req->pref_identity.addr_type != ICF_ADDRESS_PLAN_UNDEFINED )
        {
            if (p_sic_ctx->p_preferred_identity)
            {
                icf_cmn_memfree(p_sic_pdb->p_glb_pdb,
                    p_sic_ctx->p_preferred_identity,
                    ICF_MEM_COMMON,
                    p_sic_pdb->p_ecode);
            }
            
            p_sic_ctx->p_preferred_identity = 
                icf_cmn_memget(
                p_sic_pdb->p_glb_pdb,
                sizeof(icf_address_st),
                ICF_MEM_COMMON,
                p_sic_pdb->p_ecode);
            
            icf_port_memcpy(
                p_sic_ctx->p_preferred_identity,
                &(p_subs_req->pref_identity),
                sizeof(icf_address_st));
        }
        else
        {/* nothing to be done: as sic_ctx will already have the 
            * preferred identity */
        }        
              icf_port_memcpy(
                    (icf_void_t *)(p_sic_ctx->p_user_address),
                    (icf_void_t *)&(p_sic_ctx->p_preferred_identity),
                    (icf_uint32_t)
                    ICF_PORT_SIZEOF(icf_address_st));
        
        /* clone the header list into sic ctx, if received from app */
        if (ICF_NULL != p_subs_req->header_list.count)
        {
            
            ret_val = icf_cmn_clone_header_list(
                p_sic_pdb->p_glb_pdb,
                &(p_subs_req->header_list),
                &(p_sic_ctx->header_list),
                p_sic_pdb->p_ecode);
        }    
                
        /* clone the tag list into sic ctx, if received from app */
        if (ICF_NULL != p_subs_req->tag_list.count)
        {
            
            ret_val = icf_cmn_clone_tag_list(
                p_sic_pdb->p_glb_pdb,
                &(p_subs_req->tag_list),
                &(p_sic_ctx->tag_list),
                p_sic_pdb->p_ecode);
        }
        
        /* clone the body list into sic ctx, if received from app */        
        if (ICF_NULL != p_subs_req->body_list.count)
        {
            
            ret_val = icf_cmn_clone_body_list(
                p_sic_pdb->p_glb_pdb,
                &(p_subs_req->body_list),
                &(p_sic_ctx->body_list),
                p_sic_pdb->p_ecode);
        }
    }/* request->event_pkg same as sic_ctx->event_pkg: ends  */
    else
    {
        /* the request has now been sent with a different event package */
        ret_val = ICF_FAILURE;
        
    }

    ICF_FUNCTION_EXIT(p_sic_pdb->p_glb_pdb)       
    return ret_val;
                    

}/* Function icf_sic_refresh_sic_ctx() ends */


/*******************************************************************************
 *
 * FUNCTION:        icf_sic_create_app_media_data
 *
 * DESCRIPTION:     This Function will be invoked to create list of stream capab
 *					corresponding to applied sdps from the 
 *					p_sic_pdb->p_sic_ctx->p_sdp_list
 *
 *					This function will do following things:
 *					1)Traverse the p_sic_pdb->p_sic_ctx->p_sdp_list list
 *					2)Check from the stream_status bitmask whether this particular
 *					  stream is Acitive or not.
 *					  if stream is INACTIVE then
 *						increase the count field in p_cc_pdb by one
 *						AND fill deleted_media_streams[count].stream_id,
 *						deleted_media_streams[count].stream_type for this stream
 *						kept in p_cc_pdb.
 *					  else
 *						create data of type stream capab from the applied sdp of
 * 						this stream and add the pointer to newly created stream
 *						capab data into the p_cc_pdb->p_accepted_streams list.
******************************************************************************/
icf_return_t
icf_sic_create_app_media_data(
	    icf_sic_pdb_st	*p_sic_pdb)
{
	icf_return_t				ret_val = ICF_SUCCESS;

	icf_list_st		*p_stream_sdp_info_node =
								p_sic_pdb->p_sic_ctx->p_sdp_list;

	icf_stream_sdp_info_st		*p_stream_sdp_info = ICF_NULL;

	/*p_cc_pdb->count = 0;*/

	ICF_FUNCTION_ENTER(p_sic_pdb->p_glb_pdb)

	p_sic_pdb->p_list_1 = ICF_NULL;


	/* Following while loop traverses each stream node by node and count
	 * for rejected streams and make list of stream capabs from the applied
	 * sdps of Active streams.
	 */
	while (ICF_NULL != p_stream_sdp_info_node)
	{
		p_stream_sdp_info = (icf_stream_sdp_info_st *)
							(p_stream_sdp_info_node->p_data);


			/* Before calling this function we need to set p_cc_pdb->p_stream
			 * _sdp_info with this particular streams p_stream_sdp_info data.
			 * And also after returning from this function extract newly created
			 * stream capab pointer from p_cc_pdb->p_generic_ptr_1.
			 * p_cc_pdb->count must be initialised to 0 before calling
			 * this function.
			 */
			p_sic_pdb->p_stream_sdp_info = p_stream_sdp_info;

			if(ICF_FAILURE == icf_sic_fill_stream_capab_from_local_sdp(p_sic_pdb))
			{
				ret_val = ICF_FAILURE;
				break;
			}
            if(ICF_FAILURE == icf_cmn_add_node_to_list(
										p_sic_pdb->p_glb_pdb,
										&(p_sic_pdb->p_list_1),
										p_sic_pdb->p_generic_ptr_1))
            {
                ret_val = ICF_FAILURE;
                break;
            }

		p_stream_sdp_info_node = p_stream_sdp_info_node->p_next;
	}

	ICF_FUNCTION_EXIT(p_sic_pdb->p_glb_pdb)

	return ret_val;
}

/*******************************************************************************
 *
 * FUNCTION:        icf_sic_delete_app_media_data
 *
 * DESCRIPTION:     This Function will be invoked to delete the app media data
******************************************************************************/
icf_return_t
icf_sic_delete_app_media_data(
	    icf_sic_pdb_st	*p_sic_pdb)
{
	icf_return_t				ret_val = ICF_SUCCESS;

	ICF_FUNCTION_ENTER(p_sic_pdb->p_glb_pdb)

	icf_sic_free_stream_capab_list(p_sic_pdb);
	
	p_sic_pdb->p_list_1 = ICF_NULL;

	ICF_FUNCTION_EXIT(p_sic_pdb->p_glb_pdb)

	return ret_val;
}


/*******************************************************************************
 *
 * FUNCTION:        icf_sic_fill_stream_capab_from_local_sdp
 *
 * DESCRIPTION:     This Function will be invoked to create new stream capab
 *					structure from applied sdp of p_sic_pdb->p_stream_sdp_info
 *					->p_local_sdp and return pointer to newly created structure
 *					in p_cc_pdb->p_generic_ptr_1.
******************************************************************************/
icf_return_t icf_sic_fill_stream_capab_from_local_sdp
(
	icf_sic_pdb_st	*p_sic_pdb
)
{
	icf_return_t				ret_val = ICF_SUCCESS;

	icf_stream_capab_st			*p_stream_capab = ICF_NULL;

	icf_stream_sdp_info_st		*p_stream_sdp_info =
								p_sic_pdb->p_stream_sdp_info;

	icf_sdp_stream_st			*p_sdp_stream =
								&(p_sic_pdb->p_stream_sdp_info
								->p_local_sdp->sdp_stream);

	ICF_FUNCTION_ENTER(p_sic_pdb->p_glb_pdb)

	/*Assigning memory of type icf_internal_sdp_stream.*/
	ICF_MEMGET(p_sic_pdb->p_glb_pdb,
        ICF_PORT_SIZEOF(icf_stream_capab_st),
        ICF_MEM_COMMON,
        p_stream_capab,
        ICF_RET_ON_FAILURE,
        p_sic_pdb->p_ecode,
        ret_val)

    icf_port_memset(p_stream_capab, 0, ICF_PORT_SIZEOF(icf_stream_capab_st));

	p_stream_capab->stream_id = p_stream_sdp_info->stream_id;
	p_stream_capab->stream_type = p_stream_sdp_info->stream_type;
	
	if (ICF_NULL != p_sdp_stream->p_codec_info_list)
	{
		/* Codec attrib list is present in applied sdp.*/
		ret_val = icf_cmn_clone_codec_attrib_list(p_sic_pdb->p_glb_pdb,
						&(p_stream_capab->p_codec_info_list),
						p_sdp_stream->p_codec_info_list);

		p_stream_capab->bit_mask |= ICF_STREAM_CAPAB_CODEC_INFO_PRESENT;
	}
	if (ICF_FAILURE != ret_val)
	{
		if(ICF_NULL != p_sdp_stream->p_encoded_attributes)
		{
			ret_val = icf_cmn_copy_string_list(
								p_sic_pdb->p_glb_pdb,
								&(p_stream_capab->p_encoded_attributes),
								p_sdp_stream->p_encoded_attributes,
								p_sic_pdb->p_ecode);

			p_stream_capab->bit_mask |= ICF_STREAM_CAPAB_ENC_ATTRIB_PRESENT;
		}
	}
	if (ICF_FAILURE != ret_val)
	{
		p_stream_capab->media_stream_param = p_sdp_stream->media_stream_param;
	}

	if (ICF_SUCCESS == ret_val)
	{
		/* if successfull in every task then set p_stream_capab.*/
		p_sic_pdb->p_generic_ptr_1 = p_stream_capab;
	}
	else
	{
		/* In case of failure free the memory assigned to p_stream_capab.*/
		ICF_MEMFREE(p_sic_pdb->p_glb_pdb,
                p_stream_capab,
				ICF_MEM_COMMON,
                p_sic_pdb->p_ecode,
				ret_val)
	}

	ICF_FUNCTION_EXIT(p_sic_pdb->p_glb_pdb)
	return ret_val;
}

/*******************************************************************************
 *
 * FUNCTION:        icf_sic_free_stream_capab_list
 *
 * DESCRIPTION:     This Function will be invoked to delete the stream capab data
******************************************************************************/
icf_return_t
icf_sic_free_stream_capab_list(
	    icf_sic_pdb_st	*p_sic_pdb)
{
	icf_return_t				ret_val = ICF_SUCCESS;

	icf_list_st		*p_stream_capab_data_list =  p_sic_pdb->p_list_1;
	icf_stream_capab_st  *p_data = ICF_NULL;

	ICF_FUNCTION_ENTER(p_sic_pdb->p_glb_pdb)

	while(ICF_NULL != p_stream_capab_data_list)
	{
		p_data = (icf_stream_capab_st *)p_stream_capab_data_list->p_data;

		if(ICF_NULL != p_data->p_codec_info_list)
		{
			ret_val = icf_cmn_delete_codec_attrib_list(p_sic_pdb->p_glb_pdb,
						&(p_data->p_codec_info_list),p_sic_pdb->p_ecode);
		}

		if(ICF_NULL != p_data->p_encoded_attributes)
		{
			ret_val = icf_cmn_delete_list(p_sic_pdb->p_glb_pdb,
						(&(p_data->p_encoded_attributes)),
						ICF_MEM_COMMON);
		}

		/*ICF_MEMFREE(p_cc_pdb->p_glb_pdb,
				(icf_void_t *)(p_data),
				ICF_MEM_COMMON,
				p_cc_pdb->p_ecode,
				ret_val)

        p_temp_list = p_stream_capab_data_list;*/
		p_stream_capab_data_list = p_stream_capab_data_list->p_next;

  /*      ICF_MEMFREE(p_cc_pdb->p_glb_pdb,
				(icf_void_t *)(p_temp_list),
				ICF_MEM_COMMON,
				p_cc_pdb->p_ecode,
				ret_val)*/
	}

    icf_cmn_delete_list(p_sic_pdb->p_glb_pdb, &(p_sic_pdb->p_list_1), ICF_MEM_COMMON);

	ICF_FUNCTION_EXIT(p_sic_pdb->p_glb_pdb)

	return ret_val;
}

/* ICF_6_1_OPTIONS_SUPPORT start */

/*****************************************************************************
 ** FUNCTION:		icf_sic_map_to_options_event
 **
 * DESCRIPTION:		This function will Map external event to options fsm
 *					EVENT. if external event is not related to options
 *					fsm then this function will return 
 *					ICF_SIC_MAX_OPTIONS_EVENTS to p_options_event.
 *
 ****************************************************************************/
icf_return_t icf_sic_map_to_options_event(
	icf_sic_pdb_st *p_sic_pdb,
	icf_uint32_t *p_options_event)
{
	
	icf_return_t	ret_val = ICF_SUCCESS;
	icf_internal_msg_st *p_internal_msg = p_sic_pdb->p_internal_msg;
	icf_nw_inc_opt_resp_st		*p_opt_resp = ICF_NULL;
	icf_sic_options_req_st   *p_options_req = ICF_NULL;
	icf_nw_inc_opt_ind_st *p_nw_inc_opt_ind = ICF_NULL;
	icf_get_media_cap_resp_st *p_med_cap_resp = ICF_NULL;
	icf_sic_options_cfm_st *p_options_cfm = ICF_NULL;
	icf_sic_clear_options_ctx_st *p_clear_opt_ctx = ICF_NULL;
	icf_timer_data_st *p_timer_data = ICF_NULL;
	

	ICF_FUNCTION_ENTER(p_sic_pdb->p_glb_pdb)

	switch(p_internal_msg->msg_hdr.msg_id)
	{
		case ICF_SIC_OPTION_REQ:
		
			p_options_req = (icf_sic_options_req_st *)(p_sic_pdb->p_msg);
			
			ret_val = icf_dbm_find_sic_cntxt_blk(
				p_sic_pdb->p_glb_pdb,
				p_options_req->call_id,
				&(p_sic_pdb->p_sic_ctx),
				p_sic_pdb->p_ecode);

			if ( ICF_FAILURE == ret_val )
			{
  				ret_val = icf_dbm_alloc_sic_cntxt_blk(
					p_sic_pdb->p_glb_pdb,
					&(p_options_req->call_id),
					&(p_sic_pdb->p_sic_ctx),
					p_sic_pdb->p_ecode);
				if(ICF_FAILURE == ret_val)
				{
					return ret_val;
				}

				ICF_SIC_SET_STATE(p_sic_pdb,ICF_SIC_00_OPTIONS_IDLE)
			}

			*p_options_event = ICF_SIC_EVENT_OTG_OPTIONS_REQ;

			break;

		case ICF_NW_INC_OPTIONS_RESP:
		case ICF_SIC_OPTION_STATUS:
			
			p_opt_resp = (icf_nw_inc_opt_resp_st *)
                    (p_sic_pdb->p_msg);
			p_sic_pdb->p_sic_ctx = p_opt_resp->p_sic_ctx;

			*p_options_event = ICF_SIC_EVENT_OPTIONS_NW_RSP;
			
			break;

		case ICF_NW_INC_OPTIONS_IND:
		
			p_nw_inc_opt_ind = (icf_nw_inc_opt_ind_st *)
                    (p_sic_pdb->p_msg);
			p_sic_pdb->p_sic_ctx = p_nw_inc_opt_ind->p_sic_ctx;
			*p_options_event = ICF_SIC_EVENT_INC_OPTIONS_REQ;

			break;

		case ICF_GET_MEDIA_CAP_RESP:

			p_med_cap_resp = (icf_get_media_cap_resp_st*)
					(p_sic_pdb->p_msg);

			ret_val = icf_dbm_find_sic_cntxt_blk(
				p_sic_pdb->p_glb_pdb,
				p_med_cap_resp->context_identifier,
				&(p_sic_pdb->p_sic_ctx),
				p_sic_pdb->p_ecode);
			
			if ( ICF_SUCCESS == ret_val )
			{
				*p_options_event = ICF_SIC_EVENT_MEDIA_CAPAB_RSP;
			}
			else
			{
				*p_options_event = ICF_SIC_MAX_OPTIONS_EVENTS;
			}

			break;
		
		case ICF_SIC_OPTIONS_CFM:
			
			p_options_cfm = (icf_sic_options_cfm_st*)
					(p_sic_pdb->p_msg);
			
			ret_val = icf_dbm_find_sic_cntxt_blk(
				p_sic_pdb->p_glb_pdb,
				p_options_cfm->call_id,
				&(p_sic_pdb->p_sic_ctx),
				p_sic_pdb->p_ecode);
			
			if ( ICF_SUCCESS == ret_val )
			{
				*p_options_event = ICF_SIC_EVENT_OPTIONS_APP_CFM;
			}
			else
			{
				*p_options_event = ICF_SIC_MAX_OPTIONS_EVENTS;
			}

			break;

		case ICF_MSG_FROM_MRM:

			p_timer_data = *((icf_timer_data_st **)
                    (p_sic_pdb->p_glb_pdb->p_recv_msg->payload));
	
			if ( ICF_SIC_OPTIONS_TIMER == p_timer_data->timer_type )
			{
                                g_stop_timer = 1;
				p_sic_pdb->p_sic_ctx = (icf_sic_ctx_st *)
                    (p_timer_data->p_timer_buffer);
				*p_options_event = ICF_SIC_EVENT_TIMER_EXPIRY;
			   icf_dbm_ret_mem_to_pool(p_sic_pdb->p_glb_pdb, ICF_MEM_TIMER,
        			    p_sic_pdb->p_sic_ctx->p_options_timer,
            				p_sic_pdb->p_ecode);
                            p_sic_pdb->p_sic_ctx->p_options_timer = ICF_NULL;
                            p_sic_pdb->p_sic_ctx->options_timer_id = ICF_TIMER_ID_INVALID;

			}
			else
			{
				*p_options_event = ICF_SIC_MAX_OPTIONS_EVENTS;
			}

			break;
		case ICF_SIC_CLEAR_OPTIONS_CTX:
			p_clear_opt_ctx  = (icf_sic_clear_options_ctx_st*)
                              (p_sic_pdb->p_msg);			
			p_sic_pdb->p_sic_ctx = 
				p_clear_opt_ctx->p_sic_ctx;

			*p_options_event = ICF_SIC_EVENT_CLEAR_OPTIONS_CTX;
			break;
	
		default :

			*p_options_event = ICF_SIC_MAX_OPTIONS_EVENTS;
			break;
	}

	ICF_FUNCTION_EXIT(p_sic_pdb->p_glb_pdb)
	return ret_val;
}


/*****************************************************************************
 ** FUNCTION:     icf_sic_process_invalid_options_req
 **
 ** DESCRIPTION:  this function will be invoked when options FSM 
 **				  receives icf_options_req when fsm in 01 or 02 or 03 state.
 **				 
 **
 *****************************************************************************/
icf_return_t	icf_sic_process_invalid_options_req
(
icf_sic_pdb_st  *p_sic_pdb
)
{

	icf_return_t ret_val = ICF_SUCCESS;
		icf_sic_options_status_st	resp_msg;
	    icf_return_t	dummy_retval = ICF_SUCCESS;


        ICF_FUNCTION_ENTER(p_sic_pdb->p_glb_pdb)
 
		p_sic_pdb->p_internal_msg->msg_hdr.msg_id = 
			ICF_SIC_OPTION_STATUS;
		
		/* ICF will not process this options request */
		p_sic_pdb->p_sic_ctx->error_code = 
			ICF_CAUSE_REQUEST_ALREADY_PENDING;
		
		resp_msg.resp_code = 0;
		resp_msg.p_key = p_sic_pdb->p_sic_ctx->p_app_ctx;
		resp_msg.line_id = p_sic_pdb->p_sic_ctx->line_id;
		resp_msg.call_id = p_sic_pdb->p_sic_ctx->call_id;
		resp_msg.app_id = p_sic_pdb->p_sic_ctx->app_id;
		resp_msg.header_list.count = 0;
        resp_msg.body_list.count = 0;
	
		p_sic_pdb->p_internal_msg->p_msg_data = 
			(icf_uint8_t *)&resp_msg;
		
		ret_val = icf_adm_map_sic_event_to_ext_event(p_sic_pdb);
		
		if( ICF_SUCCESS == ret_val)
		{
			ret_val = icf_adm_process_mesg(p_sic_pdb->p_internal_msg);
			
			ICF_MEMFREE(p_sic_pdb->p_glb_pdb, 
				p_sic_pdb->p_internal_msg->p_msg_data, ICF_MEM_COMMON,
				p_sic_pdb->p_ecode, dummy_retval)
		}
		

		ICF_FUNCTION_EXIT(p_sic_pdb->p_glb_pdb)

        return ret_val;
}

/*****************************************************************************
 ** FUNCTION:     icf_sic_start_options_timer
 **
 ** DESCRIPTION:  This routine is used for starting the options timer
 **				 
 **
 *****************************************************************************/
icf_return_t icf_sic_start_options_timer(
         icf_sic_pdb_st  *p_sic_pdb)
{
    icf_return_t             ret_val = ICF_SUCCESS;
    icf_timer_id_t           timer_id = ICF_TIMER_ID_INVALID;
    icf_timer_data_st        *p_timer_data = ICF_NULL;
    icf_config_data_st   *p_dbm_config_data = ICF_NULL;


    ICF_FUNCTION_ENTER(p_sic_pdb->p_glb_pdb)

    if ( ICF_FAILURE == icf_dbm_get_mem_from_pool(
                p_sic_pdb->p_glb_pdb, ICF_MEM_TIMER,
                (icf_void_t **)&p_timer_data, 
                p_sic_pdb->p_ecode))
    {
        ICF_PRINT(((icf_uint8_t *)"\n[SIC]:FailureInAllocatingTimerBlockForOptionsTimer\n"));
        ret_val = ICF_FAILURE;
    }
    else
    {
        /* fill the timer data structure */
        p_timer_data->timer_type = ICF_SIC_OPTIONS_TIMER;
        p_timer_data->module_id = ICF_INT_MODULE_SIC;

        /*
         * In case of timer expiry, eshell will pass the p_timer_data
         * to SIC.SIC will then get the p_ssa_timer_data
         */
        p_timer_data->p_timer_buffer = 
            (icf_void_t*)p_sic_pdb->p_sic_ctx;
        p_timer_data->p_glb_pdb = p_sic_pdb->p_glb_pdb;

		if (ICF_FAILURE == icf_dbm_get_module_glb_data(
			p_sic_pdb->p_glb_pdb, (icf_module_id_t)(ICF_INT_MODULE_CFG),
			(icf_void_t**)(&p_dbm_config_data), 
			p_sic_pdb->p_ecode))
		{
			/* Unable to get CFG data */
            ICF_PRINT(((icf_uint8_t *)"\n[SIC]:Unable to get CFG data"));
            /*Free the allocated timer block here*/
            icf_dbm_ret_mem_to_pool(p_sic_pdb->p_glb_pdb, ICF_MEM_TIMER,
                    p_timer_data, p_sic_pdb->p_ecode);
			return ICF_FAILURE;
		}


        ICF_START_TIMER(p_sic_pdb->p_glb_pdb, 
                p_timer_data, timer_id,
                p_dbm_config_data->options_timer,
                p_sic_pdb->p_ecode, ret_val)

#ifdef ICF_LOAD_STAT
        printf("\ntimer_id %4d\tp_timer_data %x\t p_sic_ctx %x\n",timer_id, p_timer_data, p_sic_pdb->p_sic_ctx);
#endif

        if ( ICF_FAILURE == ret_val)
        {
            ICF_PRINT(((icf_uint8_t *)"\n[SIC]:Failure!! InStartingOptionsTimer"));
            /*Free the allocated timer block here*/
            icf_dbm_ret_mem_to_pool(p_sic_pdb->p_glb_pdb, ICF_MEM_TIMER,
                    p_timer_data, p_sic_pdb->p_ecode);
        }
        else
        {
            p_sic_pdb->p_sic_ctx->options_timer_id = timer_id;
            p_sic_pdb->p_sic_ctx->p_options_timer = p_timer_data;
        } /* else */
    }

    ICF_FUNCTION_EXIT(p_sic_pdb->p_glb_pdb)
    return ret_val;
}

/*****************************************************************************
 ** FUNCTION:     icf_sic_send_failure_response_to_ssa
 **
 ** DESCRIPTION:  This routine is used for sending failure indication to 
 **		  SSA.
 **
 *****************************************************************************/
icf_return_t icf_sic_send_failure_response_to_ssa(
	icf_sic_pdb_st  *p_sic_pdb)
{

    icf_return_t             ret_val = ICF_SUCCESS;
	icf_nw_inc_opt_cfm_st	 nw_inc_opt_cfm;
	
    ICF_FUNCTION_ENTER(p_sic_pdb->p_glb_pdb)

    icf_sic_stop_options_timer(p_sic_pdb);

	p_sic_pdb->p_internal_msg->msg_hdr.msg_id = 
				ICF_NW_INC_OPTIONS_CFM;

	p_sic_pdb->p_internal_msg->p_msg_data = 
		(icf_uint8_t *)&nw_inc_opt_cfm;
	
	nw_inc_opt_cfm.bit_mask = 0;
	nw_inc_opt_cfm.header_list.hdr = ICF_NULL;
	nw_inc_opt_cfm.header_list.count = 0;
	nw_inc_opt_cfm.tag_list.tag = ICF_NULL;
	nw_inc_opt_cfm.tag_list.count = 0;
	nw_inc_opt_cfm.body_list.body = ICF_NULL;
	nw_inc_opt_cfm.body_list.count = 0;

	nw_inc_opt_cfm.bit_mask |= ICF_OPTIONS_CFM_ERROR_PRESENT;

	nw_inc_opt_cfm.error_code = ICF_CAUSE_INTERNAL_ERROR;

	nw_inc_opt_cfm.p_sic_ctx = p_sic_pdb->p_sic_ctx;
	if( ICF_NULL == p_sic_pdb->p_sic_ctx->p_ssa_ctx )
	{
	    ret_val = ICF_SUCCESS;
	}
	else
	{
	    icf_ssa_process_mesg(p_sic_pdb->p_internal_msg);
	}
	icf_sic_dealloc_context(p_sic_pdb);
    
	ICF_FUNCTION_EXIT(p_sic_pdb->p_glb_pdb)
    return ret_val;
}

/*****************************************************************************
 ** FUNCTION:     icf_sic_send_failure_response_for_unxepected_inc_options
 **
 ** DESCRIPTION:  This routine is used for sending failure indication to
 **               SSA for a unexpected inc options requset.
 **
 *****************************************************************************/
icf_return_t icf_sic_send_failure_response_for_unxepected_inc_options(
        icf_sic_pdb_st  *p_sic_pdb)
{

    icf_return_t             ret_val = ICF_SUCCESS;
        icf_nw_inc_opt_cfm_st    nw_inc_opt_cfm;

    ICF_FUNCTION_ENTER(p_sic_pdb->p_glb_pdb)

        p_sic_pdb->p_internal_msg->msg_hdr.msg_id =
                                ICF_NW_INC_OPTIONS_CFM;

        p_sic_pdb->p_internal_msg->p_msg_data =
                (icf_uint8_t *)&nw_inc_opt_cfm;

        nw_inc_opt_cfm.bit_mask = 0;
        nw_inc_opt_cfm.header_list.hdr = ICF_NULL;
        nw_inc_opt_cfm.header_list.count = 0;
        nw_inc_opt_cfm.tag_list.tag = ICF_NULL;
        nw_inc_opt_cfm.tag_list.count = 0;
        nw_inc_opt_cfm.body_list.body = ICF_NULL;
        nw_inc_opt_cfm.body_list.count = 0;

        nw_inc_opt_cfm.bit_mask |= ICF_OPTIONS_CFM_ERROR_PRESENT;

        nw_inc_opt_cfm.error_code = ICF_CAUSE_INTERNAL_ERROR;

        nw_inc_opt_cfm.p_sic_ctx = p_sic_pdb->p_sic_ctx;

        icf_ssa_process_mesg(p_sic_pdb->p_internal_msg);


        ICF_FUNCTION_EXIT(p_sic_pdb->p_glb_pdb)
    return ret_val;
}


/*****************************************************************************
 ** FUNCTION:     icf_sic_send_options_ind_to_app
 **
 ** DESCRIPTION:  This routine is used to send incoming options
 **				  Indication to Application
 **
 *****************************************************************************/
icf_return_t icf_sic_send_options_ind_to_app(
         icf_sic_pdb_st  *p_sic_pdb)
{
    icf_return_t             ret_val = ICF_SUCCESS;
    icf_return_t	dummy_retval = ICF_SUCCESS;
    icf_inc_options_ind_st	 *p_options_ind = ICF_NULL;
    icf_msg_st            *p_app_msg=ICF_NULL;
    icf_uint32_t            msg_len=0;
    icf_list_st *p_app_list = ICF_NULL;

    ICF_FUNCTION_ENTER(p_sic_pdb->p_glb_pdb)
	
	msg_len = sizeof(icf_api_header_st) +
		sizeof(icf_inc_options_ind_st);
	/*Allocate memory for API header strucure */
	ICF_MEMGET(
		p_sic_pdb->p_glb_pdb,
		msg_len,
		ICF_MEM_COMMON,
		p_app_msg,
		ICF_RET_ON_FAILURE,
		p_sic_pdb->p_ecode,
		ret_val
		);

	if( ICF_NULL == p_app_msg)
	{
		ret_val = ICF_FAILURE;
	}
	else
	{
		/* Fill the message header for ICF_INC_OPTIONS_IND */
		
		p_app_msg->hdr.api_id = ICF_INC_OPTIONS_IND;
		p_app_msg->hdr.app_id = p_sic_pdb->p_sic_ctx->app_id;
		p_app_msg->hdr.api_length = (icf_uint16_t)msg_len;
		
		p_app_msg->hdr.call_id = p_sic_pdb->p_sic_ctx->call_id;
		
		/* Fill the payload for ICF_INC_OPTIONS_IND */
		p_options_ind = (icf_inc_options_ind_st *)
			(p_app_msg->payload);
		
		p_options_ind->header_list.hdr = ICF_NULL;
		p_options_ind->header_list.count = 0;
		p_options_ind->tag_list.tag = ICF_NULL;
		p_options_ind->tag_list.count = 0;
		p_options_ind->body_list.body = ICF_NULL;
		p_options_ind->body_list.count = 0;
		p_options_ind->p_app_sdp_list = ICF_NULL;
                p_options_ind->bit_mask =  ICF_NULL;

		if (ICF_NULL != p_sic_pdb->p_sic_ctx->header_list.count)
		{
			ret_val = icf_cmn_clone_header_list(
				p_sic_pdb->p_glb_pdb,
                                &(p_sic_pdb->p_sic_ctx->header_list),
				&(p_options_ind->header_list),
				p_sic_pdb->p_ecode);
			icf_cmn_free_header_list(p_sic_pdb->p_glb_pdb,
				&(p_sic_pdb->p_sic_ctx->header_list),
				p_sic_pdb->p_ecode);
			p_options_ind->bit_mask |= ICF_HDR_LIST_PRESENT;
		}
		if ( ICF_NULL != p_sic_pdb->p_sic_ctx->tag_list.count)
		{
			ret_val = icf_cmn_clone_tag_list(
				p_sic_pdb->p_glb_pdb,
                                &(p_sic_pdb->p_sic_ctx->tag_list),
				&(p_options_ind->tag_list),
				p_sic_pdb->p_ecode);
			icf_cmn_free_tag_list(
				p_sic_pdb->p_glb_pdb,
				&(p_sic_pdb->p_sic_ctx->tag_list),
				p_sic_pdb->p_ecode);
			
			p_options_ind->bit_mask |= ICF_TAG_LIST_PRESENT;
		}
		if ( ICF_NULL != p_sic_pdb->p_sic_ctx->body_list.count)
		{
			ret_val = icf_cmn_clone_body_list(
				p_sic_pdb->p_glb_pdb,
                                &(p_sic_pdb->p_sic_ctx->body_list),
				&(p_options_ind->body_list),
				p_sic_pdb->p_ecode);
			icf_cmn_free_msg_body_list(
				p_sic_pdb->p_glb_pdb,
				&(p_sic_pdb->p_sic_ctx->body_list),
				p_sic_pdb->p_ecode);
			p_options_ind->bit_mask |= ICF_BODY_LIST_PRESENT;
		}
		if ( ICF_NULL != p_sic_pdb->p_sic_ctx->p_mm_media_capab)
		{
			icf_sic_form_options_ind_media_capab_list(
				p_sic_pdb,
				&p_app_list);
			icf_cmn_clone_stream_capab_list(
				p_sic_pdb->p_glb_pdb,
				&(p_options_ind->p_app_sdp_list),
				p_app_list);
			icf_cmn_free_icf_stream_capab_list(
				p_sic_pdb->p_glb_pdb,
				&(p_app_list));
			p_options_ind->bit_mask |= ICF_INC_OPTIONS_IND_MEDIA_PRESENT;
		}
        p_sic_pdb->p_internal_msg->msg_hdr.msg_id = p_app_msg->hdr.api_id;
        p_sic_pdb->p_internal_msg->p_msg_data = (icf_uint8_t*)p_app_msg;
		ret_val = icf_adm_process_mesg(p_sic_pdb->p_internal_msg);
        
		if ( ICF_HDR_LIST_PRESENT == 
			(ICF_HDR_LIST_PRESENT & p_options_ind->bit_mask))
		{
			icf_cmn_free_header_list(p_sic_pdb->p_glb_pdb,
				&(p_options_ind->header_list),
				p_sic_pdb->p_ecode);
		}
		if ( ICF_TAG_LIST_PRESENT == 
			(ICF_TAG_LIST_PRESENT & p_options_ind->bit_mask))
		{
			icf_cmn_free_tag_list(
				p_sic_pdb->p_glb_pdb,
				&(p_options_ind->tag_list),
				p_sic_pdb->p_ecode);
		}
		if ( ICF_BODY_LIST_PRESENT == 
			(ICF_BODY_LIST_PRESENT & p_options_ind->bit_mask))
		{
			icf_cmn_free_msg_body_list(
				p_sic_pdb->p_glb_pdb,
				&(p_options_ind->body_list),
				p_sic_pdb->p_ecode);
		}
		if ( ICF_INC_OPTIONS_IND_MEDIA_PRESENT ==
			(ICF_INC_OPTIONS_IND_MEDIA_PRESENT & p_options_ind->bit_mask))
		{
			icf_cmn_free_icf_stream_capab_list(
				p_sic_pdb->p_glb_pdb,
				&(p_options_ind->p_app_sdp_list));
		}
		ICF_MEMFREE(p_sic_pdb->p_glb_pdb, 
			p_sic_pdb->p_internal_msg->p_msg_data, ICF_MEM_COMMON,
			p_sic_pdb->p_ecode, dummy_retval)
	}
	ICF_FUNCTION_EXIT(p_sic_pdb->p_glb_pdb)
    return ret_val;
}


/*****************************************************************************
 ** FUNCTION:     icf_sic_send_media_capab_req_to_mmi
 **
 ** DESCRIPTION:  This routine is used for sending media_capablities 
 **				  request to MMI
 **
 *****************************************************************************/
icf_return_t icf_sic_send_media_capab_req_to_mmi(
         icf_sic_pdb_st  *p_sic_pdb,
		 icf_list_st	 *p_app_media_capab)
{
    icf_return_t             ret_val = ICF_SUCCESS;
	icf_get_media_cap_req_st get_media_cap_req;

    ICF_FUNCTION_ENTER(p_sic_pdb->p_glb_pdb)
	p_sic_pdb->p_internal_msg->msg_hdr.msg_id = ICF_GET_MEDIA_CAP_REQ;
	p_sic_pdb->p_internal_msg->p_msg_data = (icf_uint8_t*)&get_media_cap_req;

	get_media_cap_req.context_identifier = p_sic_pdb->p_sic_ctx->call_id;
	get_media_cap_req.app_id = p_sic_pdb->p_sic_ctx->app_id;
	get_media_cap_req.p_app_sdp_list = ICF_NULL;
        if ( ICF_NULL != p_app_media_capab)
	{	
		ret_val = icf_cmn_clone_stream_capab_list(
				p_sic_pdb->p_glb_pdb,
				&(get_media_cap_req.p_app_sdp_list),
				p_app_media_capab);
	}
	if ( ICF_SUCCESS == ret_val)
	{
		ret_val = icf_mmi_process_mesg(p_sic_pdb->p_internal_msg);
		icf_cmn_free_icf_stream_capab_list(
			p_sic_pdb->p_glb_pdb,
			&(get_media_cap_req.p_app_sdp_list));
	}

	ICF_FUNCTION_EXIT(p_sic_pdb->p_glb_pdb)
    return ret_val;

}

/*****************************************************************************
 ** FUNCTION:     icf_sic_stop_options_timer
 **
 ** DESCRIPTION:  This routine is used for stoping the options timer
 **				 
 **
 *****************************************************************************/
icf_return_t icf_sic_stop_options_timer(
         icf_sic_pdb_st  *p_sic_pdb)
{
    icf_return_t             ret_val = ICF_SUCCESS;
    ICF_FUNCTION_ENTER(p_sic_pdb->p_glb_pdb)
    /* Stop the OPTIONS timer */
    if(ICF_TIMER_ID_INVALID != p_sic_pdb->p_sic_ctx->options_timer_id)
    {
        ICF_STOP_TIMER(p_sic_pdb->p_glb_pdb,
            p_sic_pdb->p_sic_ctx->options_timer_id,
            p_sic_pdb->p_ecode, ret_val)

        icf_dbm_ret_mem_to_pool(p_sic_pdb->p_glb_pdb, ICF_MEM_TIMER,
            p_sic_pdb->p_sic_ctx->p_options_timer,
            p_sic_pdb->p_ecode);
        p_sic_pdb->p_sic_ctx->options_timer_id = ICF_TIMER_ID_INVALID;
                p_sic_pdb->p_sic_ctx->p_options_timer = ICF_NULL;
    }


        ICF_FUNCTION_EXIT(p_sic_pdb->p_glb_pdb)
    return ret_val;

}


/*****************************************************************************
 ** FUNCTION:     icf_sic_map_icf_internal_list_icf_list
 **
 ** DESCRIPTION:  This routine is used to create icf_list from
 **				  icf_internal_list_st 
 **
 *****************************************************************************/
icf_return_t icf_sic_map_icf_internal_list_icf_list(
         icf_sic_pdb_st  *p_sic_pdb,
	 icf_internal_list_st *p_internal_list,
	 icf_list_st **p_list)
{
    icf_return_t             ret_val = ICF_SUCCESS;
	icf_list_st *p_tmp_list = ICF_NULL;
	icf_list_st *p_tmp_curr_list = ICF_NULL;
	icf_internal_list_st *p_tmp_internal_list = ICF_NULL;
	icf_stream_capab_st *p_tmp_stream_capab = ICF_NULL;

    ICF_FUNCTION_ENTER(p_sic_pdb->p_glb_pdb)

	if(ICF_NULL != *p_list)
	{
		ICF_FUNCTION_EXIT(p_sic_pdb->p_glb_pdb)
		return ICF_FAILURE;
	}

	for ( p_tmp_internal_list = p_internal_list;
		p_tmp_internal_list != ICF_NULL; 
		p_tmp_internal_list = p_tmp_internal_list->p_next)
	{	
		ICF_MEMGET(
			p_sic_pdb->p_glb_pdb,
			sizeof(icf_list_st),
			ICF_MEM_COMMON,
			p_tmp_curr_list,
			ICF_RET_ON_FAILURE,
			p_sic_pdb->p_ecode,
			ret_val
		);
		icf_cmn_clone_stream_capab(p_sic_pdb->p_glb_pdb,
			&p_tmp_stream_capab,
			(icf_stream_capab_st*)(p_tmp_internal_list->p_payload));
		p_tmp_curr_list->p_data = (icf_void_t*)p_tmp_stream_capab;	
		p_tmp_stream_capab = ICF_NULL;

		p_tmp_curr_list->p_next = ICF_NULL;

		if ( ICF_NULL == *p_list)
		{
			*p_list = p_tmp_curr_list;
		}
		else
		{
			p_tmp_list->p_next = p_tmp_curr_list;
		}
		p_tmp_list = p_tmp_curr_list;
	}

	ICF_FUNCTION_EXIT(p_sic_pdb->p_glb_pdb)
    return ret_val;
}

/******************************************************************************
 *
 * FUNCTION:        icf_sic_extract_mm_stream_capab
 *
 * DESCRIPTION:     This function is invoked for extracting
 *                   media capablities of MM and it will be stored in
 *                  p_mm_media_capab of sic_ctx
 *
 * RETURNS:         ICF_SUCCESS in case of success and ICF_FAILURE in 
 *                  case of any failure.       
 *
 ******************************************************************************/

icf_return_t icf_sic_extract_mm_stream_capab(
        INOUT icf_sic_pdb_st  *p_sic_pdb)
{
    icf_return_t             ret_val = ICF_SUCCESS;
	icf_sic_mm_stream_capab_st *p_tmp_sic_mm_stream = ICF_NULL;
	icf_inc_media_sdp_info_st	*p_src_data = ICF_NULL;
	icf_get_media_cap_resp_st *p_med_cap_resp = ICF_NULL;
	icf_list_st *p_dest_tmp_list = ICF_NULL;
	icf_list_st *p_dest_list = ICF_NULL;
	icf_list_st *p_tmp_list = ICF_NULL;
	icf_error_t	ecode;


    ICF_FUNCTION_ENTER(p_sic_pdb->p_glb_pdb)

	p_med_cap_resp = (icf_get_media_cap_resp_st*)
		(p_sic_pdb->p_msg);

	if ( ICF_FAILURE == p_med_cap_resp->result)
	{
		return ICF_FAILURE;
	}

	if (ICF_NULL != p_sic_pdb->p_sic_ctx->p_mm_media_capab)
	{
		return ICF_FAILURE;
	}

	for ( p_tmp_list = p_med_cap_resp->p_sdp_list;
		p_tmp_list != ICF_NULL;
		p_tmp_list = p_tmp_list->p_next)
	{
		p_src_data = (icf_inc_media_sdp_info_st*)
						(p_tmp_list->p_data);
		ICF_MEMGET(
			p_sic_pdb->p_glb_pdb,
			sizeof(icf_sic_mm_stream_capab_st),
			ICF_MEM_COMMON,
			p_tmp_sic_mm_stream,
			ICF_RET_ON_FAILURE,
			p_sic_pdb->p_ecode,
			ret_val
		);
		p_tmp_sic_mm_stream->stream_id = 
			p_src_data->stream_id;
		p_tmp_sic_mm_stream->stream_type = 
			p_src_data->stream_type;
		p_tmp_sic_mm_stream->trans_mode =
			p_src_data->p_int_sdp_stream->
			sdp_stream.trans_mode;


		icf_port_memcpy(&(p_tmp_sic_mm_stream->stream_address),
			&(p_src_data->p_int_sdp_stream->
				sdp_stream.stream_address),
			sizeof(icf_transport_address_st));

		p_tmp_sic_mm_stream->bit_mask = 0;
		p_tmp_sic_mm_stream->p_codec_info_list = ICF_NULL;
		p_tmp_sic_mm_stream->p_encoded_attributes = ICF_NULL;


		if ( ICF_NULL != 
			(p_src_data->p_int_sdp_stream->
				sdp_stream.bit_mask &
				ICF_SDP_STREAM_MEDIA_STREAM_PRESENT))
		{
			p_tmp_sic_mm_stream->media_stream_param =
				p_src_data->p_int_sdp_stream->
				sdp_stream.media_stream_param;
			p_tmp_sic_mm_stream->bit_mask |=
				ICF_SIC_STREAM_CAPAB_MEDIA_STREAM_PARAM_PRESENT;
		}
		if ( ICF_NULL != 
			(p_src_data->p_int_sdp_stream->sdp_stream.bit_mask &
				ICF_SDP_STREAM_RTCP_INFO_PRESENT))
		{
			icf_port_memcpy(&(p_tmp_sic_mm_stream->
				rtcp_address),
				&(p_src_data->p_int_sdp_stream->
					sdp_stream.rtcp_address),
				sizeof(icf_transport_address_st));
			p_tmp_sic_mm_stream->bit_mask |=
				ICF_SIC_STREAM_RTCP_INFO_PRESENT;
		}

		if ( ICF_NULL != 
			p_src_data->p_int_sdp_stream->sdp_stream.
				p_codec_info_list)
		{
			icf_cmn_clone_codec_attrib_list(p_sic_pdb->p_glb_pdb,
				&(p_tmp_sic_mm_stream->p_codec_info_list),
				p_src_data->p_int_sdp_stream->
				sdp_stream.p_codec_info_list);
			p_tmp_sic_mm_stream->bit_mask |=
				ICF_SIC_STREAM_CAPAB_CODEC_INFO_PRESENT;
		}
		
		if ( ICF_NULL != (p_src_data->p_int_sdp_stream->
					sdp_stream.bit_mask & 
			ICF_SDP_STREAM_ENCPARAMS_PRESENT))
		{
			icf_cmn_copy_string_list(p_sic_pdb->p_glb_pdb,
				&(p_tmp_sic_mm_stream->p_encoded_attributes),
				p_src_data->p_int_sdp_stream->
				sdp_stream.p_encoded_attributes,
				&ecode);
			p_tmp_sic_mm_stream->bit_mask |=
				ICF_SIC_STREAM_CAPAB_ENC_ATTRIB_PRESENT;
		}
		ICF_MEMGET(
			p_sic_pdb->p_glb_pdb,
			sizeof(icf_list_st),
			ICF_MEM_COMMON,
			p_dest_tmp_list,
			ICF_DONOT_RET_ON_FAILURE,
			p_sic_pdb->p_ecode,
			ret_val
		);
                if (ICF_FAILURE == ret_val || ICF_NULL == p_dest_tmp_list)
                {
                    ICF_MEMFREE(p_sic_pdb->p_glb_pdb,
               		        p_tmp_sic_mm_stream, 
               		        ICF_MEM_COMMON, 
				p_sic_pdb->p_ecode, 
				ret_val)
                    return ICF_FAILURE; 
                }
		p_dest_tmp_list->p_data = 
			(icf_void_t*)p_tmp_sic_mm_stream;
		p_dest_tmp_list->p_next = ICF_NULL;
		
		if ( ICF_NULL == 
			p_sic_pdb->p_sic_ctx->p_mm_media_capab)
		{
			p_sic_pdb->p_sic_ctx->p_mm_media_capab = 
				p_dest_tmp_list;
		}
		else
		{
			p_dest_list->p_next = p_dest_tmp_list;
		}
		p_dest_list = p_dest_tmp_list;
	}

	ICF_FUNCTION_EXIT(p_sic_pdb->p_glb_pdb)
    return ret_val;

}


/******************************************************************************
 *
 * FUNCTION:        icf_sic_form_final_capabilities
 *
 * DESCRIPTION:     This function is invoked for extracting
 *                   media capablities of MM and it will be stored in
 *                  p_mm_media_capab of sic_ctx
 *
 * RETURNS:         ICF_SUCCESS in case of success and ICF_FAILURE in 
 *                  case of any failure.       
 *
 ******************************************************************************/

icf_return_t icf_sic_form_final_capabilities(
        INOUT icf_sic_pdb_st  *p_sic_pdb,
		INOUT icf_list_st **p_sdp_list)
{
    icf_return_t             ret_val = ICF_SUCCESS;
        icf_list_st *p_tmp_list = ICF_NULL;
        icf_list_st *p_tmp_dest_list = ICF_NULL;
        icf_list_st *p_dest_list = ICF_NULL;
        icf_inc_media_sdp_info_st *p_temp_dest_stream_capab = ICF_NULL;
        icf_stream_capab_st *p_app_stream = ICF_NULL;
        icf_sic_mm_stream_capab_st *p_tmp_src_stream_capab = ICF_NULL;
        icf_sic_options_cfm_st *p_options_cfm = ICF_NULL;
        icf_boolean_t is_app_stream_present = ICF_FALSE;
        icf_error_t     ecode;

    ICF_FUNCTION_ENTER(p_sic_pdb->p_glb_pdb)

		if(ICF_NULL != *p_sdp_list)
		{
			return ICF_FAILURE;
		}

        p_options_cfm = (icf_sic_options_cfm_st*)
                (p_sic_pdb->p_msg);

        for ( p_tmp_list = p_sic_pdb->p_sic_ctx->p_mm_media_capab;
                p_tmp_list != ICF_NULL ;
                p_tmp_list = p_tmp_list->p_next)
        {
                p_tmp_src_stream_capab =
                        (icf_sic_mm_stream_capab_st*)(p_tmp_list->p_data);
                if ( (0 != p_options_cfm->count) &&
                         (ICF_TRUE ==
                         icf_sic_find_stream_id_from_deleted_list(p_sic_pdb,
                                p_options_cfm->deleted_media_streams,
                                p_options_cfm->count,
                                p_tmp_src_stream_capab->stream_id)))
                {
                        continue;
                }
                else if ( ICF_NULL != p_options_cfm->p_app_sdp_list)
                {
                            is_app_stream_present =
                                icf_sic_find_stream_id_from_app_list(
                                p_sic_pdb,
                                p_options_cfm->p_app_sdp_list,
                                p_tmp_src_stream_capab->stream_id,
                                &p_app_stream);
                }
                ICF_MEMGET(
                        p_sic_pdb->p_glb_pdb,
                        sizeof(icf_sdp_stream_st),
                        ICF_MEM_COMMON,
                        p_temp_dest_stream_capab,
                        ICF_RET_ON_FAILURE,
                        p_sic_pdb->p_ecode,
                        ret_val
                        );
                if (ICF_NULL == p_temp_dest_stream_capab)
                {
                   return ICF_FAILURE;
                }
                ICF_MEMGET(
                        p_sic_pdb->p_glb_pdb,
                        sizeof(icf_internal_sdp_stream_st),
                        ICF_MEM_COMMON,
                        p_temp_dest_stream_capab->p_int_sdp_stream,
                        ICF_DONOT_RET_ON_FAILURE,
                        p_sic_pdb->p_ecode,
                        ret_val
                        );
                 if (ICF_FAILURE == ret_val || ICF_NULL == p_temp_dest_stream_capab->p_int_sdp_stream)
                 {
                    ICF_MEMFREE(p_sic_pdb->p_glb_pdb,
               		            p_temp_dest_stream_capab, 
               		            ICF_MEM_COMMON, 
				                p_sic_pdb->p_ecode, 
				                ret_val)
                    return ICF_FAILURE;    
                 }

				p_temp_dest_stream_capab->stream_type =
					p_tmp_src_stream_capab->stream_type;
				p_temp_dest_stream_capab->stream_id =
					p_tmp_src_stream_capab->stream_id;
                icf_port_memcpy(
                        &(p_temp_dest_stream_capab->p_int_sdp_stream->sdp_stream.stream_address),
                        &(p_tmp_src_stream_capab->stream_address),
                        sizeof(icf_transport_address_st));

                p_temp_dest_stream_capab->p_int_sdp_stream->sdp_stream.trans_mode =
                        p_tmp_src_stream_capab->trans_mode;

                if ( ICF_NULL !=
                        (p_tmp_src_stream_capab->bit_mask &
                        ICF_SIC_STREAM_RTCP_INFO_PRESENT))
                {
                        icf_port_memcpy(
                                &(p_temp_dest_stream_capab->p_int_sdp_stream->sdp_stream.rtcp_address),
                                &(p_tmp_src_stream_capab->rtcp_address),
                                sizeof(icf_transport_address_st));
                        p_temp_dest_stream_capab->p_int_sdp_stream->sdp_stream.bit_mask |=
                                ICF_SDP_STREAM_RTCP_INFO_PRESENT;
                }
                if ( ICF_FALSE == is_app_stream_present)
                {
                        if ( ICF_NULL !=
                                (p_tmp_src_stream_capab->bit_mask &
                                ICF_SIC_STREAM_CAPAB_MEDIA_STREAM_PARAM_PRESENT))
                        {
                                p_temp_dest_stream_capab->p_int_sdp_stream->sdp_stream.media_stream_param =
                                        p_tmp_src_stream_capab->media_stream_param;
                                p_temp_dest_stream_capab->p_int_sdp_stream->sdp_stream.bit_mask |=
                                        ICF_SDP_STREAM_MEDIA_STREAM_PRESENT;
                        }
                        if ( ICF_NULL !=
                                (p_tmp_src_stream_capab->bit_mask &
                                ICF_SIC_STREAM_CAPAB_CODEC_INFO_PRESENT))
                        {
                                icf_cmn_clone_codec_attrib_list(
                                        p_sic_pdb->p_glb_pdb,
                                        &(p_temp_dest_stream_capab->p_int_sdp_stream->sdp_stream.p_codec_info_list),
                                        p_tmp_src_stream_capab->p_codec_info_list);
                        }
                        if ( ICF_NULL !=
                                (p_tmp_src_stream_capab->bit_mask &
                                ICF_SIC_STREAM_CAPAB_ENC_ATTRIB_PRESENT ))
                        {
                                icf_cmn_copy_string_list(
                                        p_sic_pdb->p_glb_pdb,
                                        &(p_temp_dest_stream_capab->p_int_sdp_stream->sdp_stream.p_encoded_attributes),
                                        p_tmp_src_stream_capab->p_encoded_attributes,
                                        &ecode);
                                p_temp_dest_stream_capab->p_int_sdp_stream->sdp_stream.bit_mask |=
                                        ICF_SDP_STREAM_ENCPARAMS_PRESENT;
                        }
                }
                else
                {
                        if ( ICF_NULL !=
                                (p_app_stream->bit_mask &
                                ICF_STREAM_CAPAB_MEDIA_STREAM_PARAM_PRESENT))
                        {
                                p_temp_dest_stream_capab->p_int_sdp_stream->sdp_stream.media_stream_param =
                                        p_app_stream->media_stream_param;
                                p_temp_dest_stream_capab->p_int_sdp_stream->sdp_stream.bit_mask |=
                                        ICF_SDP_STREAM_MEDIA_STREAM_PRESENT;
                        }
                        if ( ICF_NULL !=
                                (p_app_stream->bit_mask &
                                ICF_STREAM_CAPAB_CODEC_INFO_PRESENT))
                        {
                                icf_cmn_clone_codec_attrib_list(
                                        p_sic_pdb->p_glb_pdb,
                                        &(p_temp_dest_stream_capab->p_int_sdp_stream->sdp_stream.p_codec_info_list),
                                        p_app_stream->p_codec_info_list);
                        }
                        if ( ICF_NULL !=
                                (p_app_stream->bit_mask &
                                ICF_STREAM_CAPAB_ENC_ATTRIB_PRESENT ))
                        {
                                icf_cmn_copy_string_list(
                                        p_sic_pdb->p_glb_pdb,
                                        &(p_temp_dest_stream_capab->p_int_sdp_stream->sdp_stream.p_encoded_attributes),
                                        p_app_stream->p_encoded_attributes,
                                        &ecode);
                                p_temp_dest_stream_capab->p_int_sdp_stream->sdp_stream.bit_mask |=
                                        ICF_SDP_STREAM_ENCPARAMS_PRESENT;
                        }
                }
                ICF_MEMGET(
                        p_sic_pdb->p_glb_pdb,
                        sizeof(icf_list_st),
                        ICF_MEM_COMMON,
                        p_tmp_dest_list,
                        ICF_DONOT_RET_ON_FAILURE,
                        p_sic_pdb->p_ecode,
                        ret_val
                        );
                 if (ICF_FAILURE == ret_val) 
                 {
                    ICF_MEMFREE(p_sic_pdb->p_glb_pdb,
               		            p_temp_dest_stream_capab->p_int_sdp_stream, 
               		            ICF_MEM_COMMON, 
				                p_sic_pdb->p_ecode, 
				                ret_val)

                    ICF_MEMFREE(p_sic_pdb->p_glb_pdb,
               		            p_temp_dest_stream_capab, 
               		            ICF_MEM_COMMON, 
				                p_sic_pdb->p_ecode, 
				                ret_val)
 
                    return ICF_FAILURE;    
                 }
                p_tmp_dest_list->p_data = (icf_void_t*)p_temp_dest_stream_capab;
                p_tmp_dest_list->p_next = ICF_NULL;

                if ( ICF_NULL == *p_sdp_list)
                {
                        *p_sdp_list = p_tmp_dest_list;
                }
                else
                {
                        p_dest_list->p_next = p_tmp_dest_list;
                }
                p_dest_list = p_tmp_dest_list;
                p_app_stream = ICF_NULL;
                is_app_stream_present = ICF_FALSE;

        }

        ICF_FUNCTION_EXIT(p_sic_pdb->p_glb_pdb)
    return ret_val;

}

/******************************************************************************
 *
 * FUNCTION:        icf_sic_send_options_success_resp_to_ssa
 *
 * DESCRIPTION:     This function is invoked for 
 *                  sending 200 OK for OPTIONS request
 *
 * RETURNS:         ICF_SUCCESS in case of success and ICF_FAILURE in 
 *                  case of any failure.       
 *
******************************************************************************/
icf_return_t icf_sic_send_options_success_resp_to_ssa(
        INOUT icf_sic_pdb_st  *p_sic_pdb,
		INOUT icf_list_st *p_sdp_list)
{
    icf_return_t             ret_val = ICF_SUCCESS;
	icf_nw_inc_opt_cfm_st	 nw_inc_opt_cfm;
	
    ICF_FUNCTION_ENTER(p_sic_pdb->p_glb_pdb)

	p_sic_pdb->p_internal_msg->msg_hdr.msg_id = 
				ICF_NW_INC_OPTIONS_CFM;

	p_sic_pdb->p_internal_msg->p_msg_data = 
		(icf_uint8_t *)&nw_inc_opt_cfm;
	
	nw_inc_opt_cfm.bit_mask = 0;
	nw_inc_opt_cfm.header_list.hdr = ICF_NULL;
	nw_inc_opt_cfm.header_list.count = 0;
	nw_inc_opt_cfm.tag_list.tag = ICF_NULL;
	nw_inc_opt_cfm.tag_list.count = 0;
	nw_inc_opt_cfm.body_list.body = ICF_NULL;
	nw_inc_opt_cfm.body_list.count = 0;
	nw_inc_opt_cfm.p_sdp_list = ICF_NULL;

	nw_inc_opt_cfm.p_sic_ctx = p_sic_pdb->p_sic_ctx;

	if ( ICF_NULL != p_sdp_list)
	{
		nw_inc_opt_cfm.p_sdp_list = p_sdp_list;
		nw_inc_opt_cfm.bit_mask |= 
			ICF_OPTIONS_CFM_MEDIA_PRESENT;
	}

	if (ICF_NULL != p_sic_pdb->p_sic_ctx->header_list.count)
	{
		ret_val = icf_cmn_clone_header_list(
			p_sic_pdb->p_glb_pdb,
			&(p_sic_pdb->p_sic_ctx->header_list),
                        &(nw_inc_opt_cfm.header_list),
			p_sic_pdb->p_ecode);
		icf_cmn_free_header_list(p_sic_pdb->p_glb_pdb,
			&(p_sic_pdb->p_sic_ctx->header_list),
			p_sic_pdb->p_ecode);
		nw_inc_opt_cfm.bit_mask |= ICF_HDR_LIST_PRESENT;
	}
	if ( ICF_NULL != p_sic_pdb->p_sic_ctx->tag_list.count)
	{
		ret_val = icf_cmn_clone_tag_list(
			p_sic_pdb->p_glb_pdb,
			&(p_sic_pdb->p_sic_ctx->tag_list),
                        &(nw_inc_opt_cfm.tag_list),
			p_sic_pdb->p_ecode);
		icf_cmn_free_tag_list(
			p_sic_pdb->p_glb_pdb,
			&(p_sic_pdb->p_sic_ctx->tag_list),
			p_sic_pdb->p_ecode);
		
		nw_inc_opt_cfm.bit_mask |= ICF_TAG_LIST_PRESENT;
	}
	if ( ICF_NULL != p_sic_pdb->p_sic_ctx->body_list.count)
	{
		ret_val = icf_cmn_clone_body_list(
			p_sic_pdb->p_glb_pdb,
			&(p_sic_pdb->p_sic_ctx->body_list),
                        &(nw_inc_opt_cfm.body_list),
			p_sic_pdb->p_ecode);
		icf_cmn_free_msg_body_list(
			p_sic_pdb->p_glb_pdb,
			&(p_sic_pdb->p_sic_ctx->body_list),
			p_sic_pdb->p_ecode);
		nw_inc_opt_cfm.bit_mask |= ICF_BODY_LIST_PRESENT;
	}

	icf_ssa_process_mesg(p_sic_pdb->p_internal_msg);
	
	if ( ICF_HDR_LIST_PRESENT == 
		(ICF_HDR_LIST_PRESENT & nw_inc_opt_cfm.bit_mask))
	{
		icf_cmn_free_header_list(p_sic_pdb->p_glb_pdb,
			&(nw_inc_opt_cfm.header_list),
			p_sic_pdb->p_ecode);
	}
	if ( ICF_TAG_LIST_PRESENT == 
		(ICF_TAG_LIST_PRESENT & nw_inc_opt_cfm.bit_mask))
	{
		icf_cmn_free_tag_list(
			p_sic_pdb->p_glb_pdb,
			&(nw_inc_opt_cfm.tag_list),
			p_sic_pdb->p_ecode);
	}
	if ( ICF_BODY_LIST_PRESENT == 
		(ICF_BODY_LIST_PRESENT & nw_inc_opt_cfm.bit_mask))
	{
		icf_cmn_free_msg_body_list(
			p_sic_pdb->p_glb_pdb,
			&(nw_inc_opt_cfm.body_list),
			p_sic_pdb->p_ecode);
	}

	icf_sic_dealloc_context(p_sic_pdb);
    
	ICF_FUNCTION_EXIT(p_sic_pdb->p_glb_pdb)
    return ret_val;

}

/******************************************************************************
 *
 * FUNCTION:        icf_sic_form_options_ind_media_capab_list
 *
 * DESCRIPTION:     This function is invoked for creating
 *                   media capablities list from p_mm_media_capab. 
 *					
 *
 * RETURNS:         ICF_SUCCESS in case of success and ICF_FAILURE in 
 *                  case of any failure.       
 *
 ******************************************************************************/

icf_return_t icf_sic_form_options_ind_media_capab_list(
        INOUT icf_sic_pdb_st  *p_sic_pdb,
		INOUT icf_list_st **p_app_list)
{
    icf_return_t             ret_val = ICF_SUCCESS;
	icf_list_st *p_tmp_list = ICF_NULL;
	icf_list_st *p_tmp_dest_list = ICF_NULL;
	icf_list_st *p_tmp_src_list = ICF_NULL;
	icf_stream_capab_st *p_temp_dest_stream_capab = ICF_NULL;
	icf_sic_mm_stream_capab_st *p_temp_src_stream_capab = ICF_NULL;
	icf_error_t ecode;

	
    ICF_FUNCTION_ENTER(p_sic_pdb->p_glb_pdb)

	if(ICF_NULL != *p_app_list)
	{
		return ICF_FAILURE;
	}

	for ( p_tmp_list = p_sic_pdb->p_sic_ctx->p_mm_media_capab;
		p_tmp_list != ICF_NULL ; 
		p_tmp_list = p_tmp_list->p_next)
	{
		p_temp_src_stream_capab = 
			(icf_sic_mm_stream_capab_st*)(p_tmp_list->p_data);
		ICF_MEMGET(
			p_sic_pdb->p_glb_pdb,
			sizeof(icf_stream_capab_st),
			ICF_MEM_COMMON,
			p_temp_dest_stream_capab,
			ICF_RET_ON_FAILURE,
			p_sic_pdb->p_ecode,
			ret_val
		);
		
		p_temp_dest_stream_capab->stream_id = 
			p_temp_src_stream_capab->stream_id;
		
		p_temp_dest_stream_capab->stream_type = 
			p_temp_src_stream_capab->stream_type;

		p_temp_dest_stream_capab->bit_mask = 0;

 		if ( ICF_NULL != (p_temp_src_stream_capab->bit_mask & 
			ICF_SIC_STREAM_CAPAB_MEDIA_STREAM_PARAM_PRESENT))
		{
			p_temp_dest_stream_capab->media_stream_param = 
				p_temp_src_stream_capab->media_stream_param;
			p_temp_dest_stream_capab->bit_mask |=
				ICF_STREAM_CAPAB_MEDIA_STREAM_PARAM_PRESENT;

		}
		
		if ( ICF_NULL != (p_temp_src_stream_capab->bit_mask & 
			ICF_SIC_STREAM_CAPAB_CODEC_INFO_PRESENT))
		{
			icf_cmn_clone_codec_attrib_list(p_sic_pdb->p_glb_pdb,
				&(p_temp_dest_stream_capab->p_codec_info_list),
				p_temp_src_stream_capab->p_codec_info_list);
			p_temp_dest_stream_capab->bit_mask |=
				ICF_STREAM_CAPAB_CODEC_INFO_PRESENT;
			
		}
		
		if ( ICF_NULL != (p_temp_src_stream_capab->bit_mask & 
			ICF_SIC_STREAM_CAPAB_ENC_ATTRIB_PRESENT))
		{
			icf_cmn_copy_string_list(p_sic_pdb->p_glb_pdb,
				&(p_temp_dest_stream_capab->p_encoded_attributes),
				p_temp_src_stream_capab->p_encoded_attributes,
				&ecode);
			p_temp_dest_stream_capab->bit_mask |=
				ICF_STREAM_CAPAB_ENC_ATTRIB_PRESENT;

		}
		ICF_MEMGET(
			p_sic_pdb->p_glb_pdb,
			sizeof(icf_list_st),
			ICF_MEM_COMMON,
			p_tmp_dest_list,
			ICF_DONOT_RET_ON_FAILURE,
			p_sic_pdb->p_ecode,
			ret_val
		);
                if (ICF_FAILURE == ret_val || ICF_NULL == p_tmp_dest_list)
                {
                    ICF_MEMFREE(p_sic_pdb->p_glb_pdb,
               		        p_temp_dest_stream_capab, 
               		        ICF_MEM_COMMON, 
				p_sic_pdb->p_ecode, 
				ret_val)
                    return ICF_FAILURE; 
                }
		p_tmp_dest_list->p_data = 
			(icf_void_t*)p_temp_dest_stream_capab;
		p_tmp_dest_list->p_next = ICF_NULL;

		if ( ICF_NULL == *p_app_list)
		{
			*p_app_list = p_tmp_dest_list;
		}
		else
		{
			p_tmp_src_list->p_next = p_tmp_dest_list;
		}
		p_tmp_src_list = p_tmp_dest_list;

	}
		

	ICF_FUNCTION_EXIT(p_sic_pdb->p_glb_pdb)
    return ret_val;


}

/******************************************************************************
 *
 * FUNCTION:        icf_sic_find_stream_id_from_deleted_list
 *
 * DESCRIPTION:     This function is invoked for finding 
 *                  stream_id for deleted stream ids list
 *
 *
 * RETURNS:         ICF_SUCCESS in case of success and ICF_FAILURE in 
 *                  case of any failure.       
 *
 ******************************************************************************/

icf_boolean_t icf_sic_find_stream_id_from_deleted_list(
        INOUT icf_sic_pdb_st  *p_sic_pdb,
		INOUT icf_stream_list_st *p_deleted_list,
		INOUT icf_uint32_t count,
		INOUT icf_stream_id_t stream_id)
{

 	icf_uint32_t	iterator = 0;

	icf_return_t	ret_val = ICF_SUCCESS;

    if(ICF_NULL == p_sic_pdb)
    {
          ICF_PRINT(((icf_uint8_t *)"p_sic_pdb is null"));
           return ICF_FALSE;
    }


	p_sic_pdb = p_sic_pdb;	

    ICF_FUNCTION_ENTER(p_sic_pdb->p_glb_pdb)

	for ( iterator = 0; iterator < count; iterator++)
	{
		if ( stream_id == p_deleted_list[iterator].stream_id)
		{
			return ICF_TRUE;
		}
	}
	ret_val =  ICF_SUCCESS;
	ICF_FUNCTION_EXIT(p_sic_pdb->p_glb_pdb)
    return ICF_FALSE;
}

/******************************************************************************
 *
 * FUNCTION:        icf_sic_find_stream_id_from_app_list
 *
 * DESCRIPTION:     This function is invoked for finding 
 *                  stream_id from app modified list
 *
 *
 * RETURNS:         ICF_SUCCESS in case of success and ICF_FAILURE in 
 *                  case of any failure.       
 *
 ******************************************************************************/

icf_boolean_t icf_sic_find_stream_id_from_app_list(
        INOUT icf_sic_pdb_st  *p_sic_pdb,
	INOUT icf_list_st *p_app_list,
	INOUT icf_stream_id_t stream_id,
	INOUT icf_stream_capab_st **p_app_stream)
{
	icf_list_st *p_iterator_list = ICF_NULL;
	icf_stream_capab_st *p_tmp_src_stream_capab = ICF_NULL;
        icf_return_t    ret_val = ICF_SUCCESS;

        p_sic_pdb = p_sic_pdb;

        if(ICF_NULL == p_sic_pdb)
        {
           ICF_PRINT(((icf_uint8_t *) "p_sic_pdb is null in icf_sic_find_stream_id_from_app_list()"));
           return ICF_FALSE;
        }
        
	ICF_FUNCTION_ENTER(p_sic_pdb->p_glb_pdb)

	for ( p_iterator_list = p_app_list;
		p_iterator_list != ICF_NULL;
		p_iterator_list = p_iterator_list->p_next)
	{
		p_tmp_src_stream_capab =
			(icf_stream_capab_st*)(p_iterator_list->p_data);
		if ( stream_id == p_tmp_src_stream_capab->stream_id)
		{
			*p_app_stream = p_tmp_src_stream_capab;
			return ICF_TRUE;
		}
	}
	ret_val = ret_val;
	ICF_FUNCTION_EXIT(p_sic_pdb->p_glb_pdb)
    return ICF_FALSE;

}

/******************************************************************************
 *
 * FUNCTION:        icf_sic_free_final_capabilities
 *
 * DESCRIPTION:     This function frees the  extracted
 *                   media capablities of MM 
 *
 * RETURNS:         ICF_SUCCESS in case of success and ICF_FAILURE in 
 *                  case of any failure.       
 *
 ******************************************************************************/

icf_return_t icf_sic_free_final_capabilities(
        INOUT icf_sic_pdb_st  *p_sic_pdb,
		INOUT icf_list_st **p_sdp_list)
{
    icf_return_t             ret_val = ICF_SUCCESS;
        icf_list_st *p_tmp_list = ICF_NULL;
        icf_inc_media_sdp_info_st *p_temp_stream_capab = ICF_NULL;
        icf_error_t     ecode;

    ICF_FUNCTION_ENTER(p_sic_pdb->p_glb_pdb)

		if(ICF_NULL == *p_sdp_list)
		{
			return ICF_NULL;
		}

        for ( p_tmp_list = *p_sdp_list;
                p_tmp_list != ICF_NULL ;
                p_tmp_list = p_tmp_list->p_next)
        {
                p_temp_stream_capab =
                        (icf_inc_media_sdp_info_st*)(p_tmp_list->p_data);

				if(ICF_NULL == p_temp_stream_capab || ICF_NULL ==
						p_temp_stream_capab->p_int_sdp_stream)
				{
					continue;
				}

				if ( ICF_SDP_STREAM_ENCPARAMS_PRESENT == 
					(p_temp_stream_capab->p_int_sdp_stream->sdp_stream.bit_mask &
					 ICF_SDP_STREAM_ENCPARAMS_PRESENT))
				{
					icf_cmn_delete_list(p_sic_pdb->p_glb_pdb,
						(&(p_temp_stream_capab->p_int_sdp_stream->sdp_stream.p_encoded_attributes)),
						ICF_MEM_COMMON);
				}
				if ( ICF_NULL != 
					p_temp_stream_capab->p_int_sdp_stream->sdp_stream.p_codec_info_list)
				{
					icf_cmn_delete_codec_attrib_list
					(p_sic_pdb->p_glb_pdb,
					&(p_temp_stream_capab->p_int_sdp_stream->sdp_stream.p_codec_info_list),
					&ecode);
				}
				ICF_MEMFREE
				(
					p_sic_pdb->p_glb_pdb, 
               		p_temp_stream_capab->p_int_sdp_stream, 
               		ICF_MEM_COMMON, 
					&ecode, 
					ret_val
				);
        }
		icf_cmn_delete_list(p_sic_pdb->p_glb_pdb,
				p_sdp_list,ICF_MEM_COMMON);
		*p_sdp_list = ICF_NULL;


        ICF_FUNCTION_EXIT(p_sic_pdb->p_glb_pdb)
    return ret_val;
}

/******************************************************************************
 *
 * FUNCTION:        icf_sic_free_mm_capabilities
 *
 * DESCRIPTION:     This function frees the  extracted
 *                   media capablities 
 *
 * RETURNS:         ICF_SUCCESS in case of success and ICF_FAILURE in 
 *                  case of any failure.       
 *
 ******************************************************************************/

icf_return_t icf_sic_free_mm_capabilities(
        INOUT icf_sic_pdb_st  *p_sic_pdb,
		INOUT icf_list_st **p_sdp_list)
{
    icf_return_t             ret_val = ICF_SUCCESS;
        icf_list_st *p_tmp_list = ICF_NULL;
        icf_sic_mm_stream_capab_st *p_temp_stream_capab = ICF_NULL;
        icf_error_t     ecode;

    ICF_FUNCTION_ENTER(p_sic_pdb->p_glb_pdb)

        for ( p_tmp_list = *p_sdp_list;
                p_tmp_list != ICF_NULL ;
                p_tmp_list = p_tmp_list->p_next)
        {
                p_temp_stream_capab =
                        (icf_sic_mm_stream_capab_st*)(p_tmp_list->p_data);
				if ( ICF_SIC_STREAM_CAPAB_ENC_ATTRIB_PRESENT ==
						(p_temp_stream_capab->bit_mask &
							ICF_SIC_STREAM_CAPAB_ENC_ATTRIB_PRESENT))
				{
					icf_cmn_delete_list(p_sic_pdb->p_glb_pdb,
						(&(p_temp_stream_capab->p_encoded_attributes)),
						ICF_MEM_COMMON);
				}
				if ( ICF_SIC_STREAM_CAPAB_CODEC_INFO_PRESENT ==
						(p_temp_stream_capab->bit_mask &
							ICF_SIC_STREAM_CAPAB_CODEC_INFO_PRESENT))
				{
					icf_cmn_delete_codec_attrib_list
					(p_sic_pdb->p_glb_pdb,
					&(p_temp_stream_capab->p_codec_info_list),
					&ecode);
				}

        }		
		icf_cmn_delete_list(p_sic_pdb->p_glb_pdb,
				p_sdp_list,ICF_MEM_COMMON);
		*p_sdp_list = ICF_NULL;


        ICF_FUNCTION_EXIT(p_sic_pdb->p_glb_pdb)
    return ret_val;
}


/* ICF_6_1_OPTIONS_SUPPORT end */



/******************************************************************************
 * FUNCTION:        icf_sic_send_failure_response_to_app
 * DESCRIPTION:     This function sends the failure response to APP in case the
 *					request was not being sent over the network.	
 * 					this function takes input only for api_id and the error 
 * 					code to be sent to the APP so fill the api_id in internal msg
 * 					and error_code in the sic context before triggering this function
 * RETURNS:         ICF_SUCCESS in case of success and ICF_FAILURE in 
 *                  case of any failure.       
 ******************************************************************************/

icf_return_t icf_sic_send_failure_response_to_app
(
	INOUT icf_sic_pdb_st  *p_sic_pdb
)
{
	icf_return_t             		ret_val = ICF_SUCCESS;
	icf_return_t					dummy_retval = ICF_SUCCESS;
	icf_sic_subs_state_st			subs_resp_msg;
	icf_sic_options_status_st		options_resp_msg;
	icf_sic_publish_status_st		publish_resp_msg;
	icf_sic_message_status_st		message_resp_msg;
	icf_sic_refer_status_st			refer_resp_msg;

	ICF_FUNCTION_ENTER(p_sic_pdb->p_glb_pdb)

	switch(p_sic_pdb->p_internal_msg->msg_hdr.msg_id)
	{
		case ICF_SIC_SUBSCRIPTION_STATUS:
			subs_resp_msg.subs_state = ICF_SUBS_FAIL_NO_RETRY;
			subs_resp_msg.p_subs_key = p_sic_pdb->p_sic_ctx->p_app_ctx;
			subs_resp_msg.duration = p_sic_pdb->p_sic_ctx->duration;
			subs_resp_msg.resp_code = 0;
			subs_resp_msg.line_id = p_sic_pdb->p_sic_ctx->line_id;
			subs_resp_msg.call_id = p_sic_pdb->p_sic_ctx->call_id;
			subs_resp_msg.app_id = p_sic_pdb->p_sic_ctx->app_id;
			/* Fix for CSR 1-8420460: SPR 20343 */
    		/* Copy the event package from the SIC ctx. Note that currently
			 * there is no case where Subs Ind is sent to App from this leg.
			 * However, this code is being added for completeness to ensure
			 * event name is filled when this leg is invoked in future.
			 */
    		icf_port_strcpy(
				subs_resp_msg.event_package.str,
				p_sic_pdb->event_package.str);
			subs_resp_msg.event_package.str_len = 
				icf_port_strlen(subs_resp_msg.event_package.str);
	
			if (ICF_NULL != p_sic_pdb->p_sic_ctx->header_list.count)
			{
				ret_val = icf_cmn_clone_header_list(
						p_sic_pdb->p_glb_pdb,
						&(p_sic_pdb->p_sic_ctx->header_list),
						&(subs_resp_msg.header_list),
						p_sic_pdb->p_ecode);
			}
			else
			{
				subs_resp_msg.header_list.count = 0;
			}
		
			p_sic_pdb->p_internal_msg->p_msg_data = (icf_uint8_t *)&subs_resp_msg;

        break;

        case ICF_SIC_OPTION_STATUS:
			options_resp_msg.p_key = p_sic_pdb->p_sic_ctx->p_app_ctx;
			options_resp_msg.resp_code = 0;
			options_resp_msg.line_id = p_sic_pdb->p_sic_ctx->line_id;
			options_resp_msg.call_id = p_sic_pdb->p_sic_ctx->call_id;
			options_resp_msg.app_id = p_sic_pdb->p_sic_ctx->app_id;
			options_resp_msg.header_list.count = 0;
		
			/* Fix for SPR 13989 */
			options_resp_msg.header_list.hdr = ICF_NULL;
			options_resp_msg.tag_list.count = 0;
			options_resp_msg.tag_list.tag = ICF_NULL;
			options_resp_msg.body_list.count = 0;
			options_resp_msg.body_list.body = ICF_NULL;

			p_sic_pdb->p_internal_msg->p_msg_data = (icf_uint8_t *)&options_resp_msg;
		    
        break;

        case ICF_SIC_PUBLISH_STATUS:
			publish_resp_msg.p_key = p_sic_pdb->p_sic_ctx->p_app_ctx;
			publish_resp_msg.duration = p_sic_pdb->p_sic_ctx->duration;
			publish_resp_msg.resp_code = 0;
			publish_resp_msg.line_id = p_sic_pdb->p_sic_ctx->line_id;
			publish_resp_msg.call_id = p_sic_pdb->p_sic_ctx->call_id;
			publish_resp_msg.app_id = p_sic_pdb->p_sic_ctx->app_id;
	
			if (ICF_NULL != p_sic_pdb->p_sic_ctx->header_list.count)
			{
				ret_val = icf_cmn_clone_header_list(
						p_sic_pdb->p_glb_pdb,
						&(p_sic_pdb->p_sic_ctx->header_list),
						&(publish_resp_msg.header_list),
						p_sic_pdb->p_ecode);
			}
			else
			{
				publish_resp_msg.header_list.count = 0;
			}
		
			p_sic_pdb->p_internal_msg->p_msg_data = (icf_uint8_t *)&publish_resp_msg;
		
        break;

        case ICF_SIC_MESSAGE_STATUS:
			message_resp_msg.p_key = p_sic_pdb->p_sic_ctx->p_app_ctx;
			message_resp_msg.resp_code = 0;
			message_resp_msg.line_id = p_sic_pdb->p_sic_ctx->line_id;
			message_resp_msg.call_id = p_sic_pdb->p_sic_ctx->call_id;
			message_resp_msg.app_id = p_sic_pdb->p_sic_ctx->app_id;
	
			if (ICF_NULL != p_sic_pdb->p_sic_ctx->header_list.count)
			{
				ret_val = icf_cmn_clone_header_list(
						p_sic_pdb->p_glb_pdb,
						&(p_sic_pdb->p_sic_ctx->header_list),
						&(message_resp_msg.header_list),
						p_sic_pdb->p_ecode);
			}
			else
			{
				message_resp_msg.header_list.count = 0;
			}
		
			p_sic_pdb->p_internal_msg->p_msg_data = (icf_uint8_t *)&message_resp_msg;

		break;
        
		case ICF_SIC_REFER_STATUS:
			refer_resp_msg.p_key = p_sic_pdb->p_sic_ctx->p_app_ctx;
			refer_resp_msg.resp_code = 0;
			refer_resp_msg.line_id = p_sic_pdb->p_sic_ctx->line_id;
			refer_resp_msg.call_id = p_sic_pdb->p_sic_ctx->call_id;
			refer_resp_msg.app_id = p_sic_pdb->p_sic_ctx->app_id;
	
			if (ICF_NULL != p_sic_pdb->p_sic_ctx->header_list.count)
			{
				ret_val = icf_cmn_clone_header_list(
						p_sic_pdb->p_glb_pdb,
						&(p_sic_pdb->p_sic_ctx->header_list),
						&(refer_resp_msg.header_list),
						p_sic_pdb->p_ecode);
			}
			else
			{
				refer_resp_msg.header_list.count = 0;
			}
		
			p_sic_pdb->p_internal_msg->p_msg_data = (icf_uint8_t *)&refer_resp_msg;

		break;

		default:
			return ICF_FAILURE;
    }

	ret_val = icf_adm_map_sic_event_to_ext_event(p_sic_pdb);

	if( ICF_SUCCESS == ret_val)
	{
		/* Changes For REL7.0 XCONF */
		if(p_sic_pdb->p_sic_ctx->bitmask & ICF_SIC_INTERNAL_SUBSCRIBE_REQUEST)
		{
		    p_sic_pdb->p_internal_msg->msg_hdr.msg_id = ICF_INTERNAL_SUBSCRIBE_RESP;
		    ret_val = icf_cm_process_mesg(p_sic_pdb->p_internal_msg);
		}
		/*  Changes for XCONF End */
		else
		{
		    ret_val = icf_adm_process_mesg(p_sic_pdb->p_internal_msg);
		}

		ICF_MEMFREE(p_sic_pdb->p_glb_pdb, 
			p_sic_pdb->p_internal_msg->p_msg_data, ICF_MEM_COMMON,
			p_sic_pdb->p_ecode, dummy_retval)
	}

	/* Deallocate the SIC context block */
	icf_sic_dealloc_context(p_sic_pdb);

    ICF_FUNCTION_EXIT(p_sic_pdb->p_glb_pdb)
    return ret_val;
}

/******************************************************************************
 * FUNCTION:        icf_sic_free_sdp_list
 * DESCRIPTION:     This function is invoked to free the p_sic_pdb
 *					p_sic_pdb->p_sic_ctx->p_sdp_list	
 * RETURNS:         ICF_SUCCESS  
 *                         
 ******************************************************************************/

icf_return_t icf_sic_free_sdp_list
(
	icf_sic_pdb_st *p_sic_pdb
)
{
	icf_return_t	ret_val = ICF_SUCCESS;
	icf_return_t	dummy_retval = ICF_SUCCESS;
	icf_list_st		*p_stream_sdp_info_node =
								p_sic_pdb->p_sic_ctx->p_sdp_list;
	icf_stream_sdp_info_st		*p_stream_sdp_info = ICF_NULL;
	icf_list_st		*p_tmp_stream_sdp_info_node = ICF_NULL;

	ICF_FUNCTION_ENTER(p_sic_pdb->p_glb_pdb)
   
	while ( ICF_NULL != p_stream_sdp_info_node)
	{
		p_stream_sdp_info = (icf_stream_sdp_info_st *)
			(p_stream_sdp_info_node->p_data);
		if ( ICF_NULL != p_stream_sdp_info)
		{
			if ( ICF_NULL != p_stream_sdp_info->p_local_sdp &&
				 ICF_NULL != p_stream_sdp_info->p_local_sdp->sdp_stream.p_codec_info_list)
			{
				icf_cmn_delete_codec_attrib_list(p_sic_pdb->p_glb_pdb,
							&(p_stream_sdp_info->p_local_sdp->sdp_stream.p_codec_info_list),
							p_sic_pdb->p_ecode);
			}
			if ( ICF_NULL != p_stream_sdp_info->p_local_sdp &&
				 ICF_NULL != p_stream_sdp_info->p_local_sdp->sdp_stream.p_encoded_attributes)
			{
				icf_cmn_delete_list(
                    p_sic_pdb->p_glb_pdb,
                    &(p_stream_sdp_info->p_local_sdp->sdp_stream.p_encoded_attributes),
                    ICF_MEM_COMMON);
			}
			if ( ICF_NULL != p_stream_sdp_info->p_local_sdp)
			{
				ICF_MEMFREE(p_sic_pdb->p_glb_pdb, 
					p_stream_sdp_info->p_local_sdp, ICF_MEM_COMMON,
					p_sic_pdb->p_ecode, dummy_retval)
			}
			ICF_MEMFREE(p_sic_pdb->p_glb_pdb, 
					p_stream_sdp_info, ICF_MEM_COMMON,
					p_sic_pdb->p_ecode, dummy_retval)

		}
		p_tmp_stream_sdp_info_node = p_stream_sdp_info_node;
		p_stream_sdp_info_node = p_stream_sdp_info_node->p_next;
		ICF_MEMFREE(p_sic_pdb->p_glb_pdb, 
				p_tmp_stream_sdp_info_node, ICF_MEM_COMMON,
				p_sic_pdb->p_ecode, dummy_retval)			
	
	}
	 
	ICF_FUNCTION_EXIT(p_sic_pdb->p_glb_pdb)
    return ret_val;	
}

