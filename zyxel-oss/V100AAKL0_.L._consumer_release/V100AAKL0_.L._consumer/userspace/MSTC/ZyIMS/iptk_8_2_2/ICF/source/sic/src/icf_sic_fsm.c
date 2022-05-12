/******************************************************************************
 *
 * FILE NAME:   icf_sic_fsm.c
 *
 * DESCRIPTION: This file contains the FSM handler functions of SIC.
 *
 * REVISION HISTORY:
 *  Date            Name         Reference       Comments
 * 11-Aug-2005  Syed Ali Rizvi    LLD             Initial
 * 06-Apr-2006	Puneet Sharma	  ICF5.0 LLD	  changes done in the comments
 *                                                Multiple Public Users ICF5.0
 * 26-Jan-2008  Amit Mahajan      ICF Rel 8.0     Changes for reg event package support
 * 08-Apr-2008  Tarun Gupta       SPR 18355       Merged Windows Porting Changes
 * 20-Nov-2008  Rajiv Kumar       SPR 19188       Rel 7.1.2 SPR merging (SPR
 *                                                18931)  
 * 12-Dec-2008  Rajiv Kumar       SPR 19218       Merged CSR 1-6240518\
 * 09-Feb-2009  Anurag Khare      ICF Rel 8.1     changes done for OPTIONS Enhancements
 * 17-Feb-2009  Anurag Khare      REl 8.1         KlocWorks Warning Removal
 * 18-Feb-2009  Anurag Khare      Rel 8.1         Options Enhancemenst
 * 27-Mar-2009  Anuradha Gupta    Rel 8.2         Reg Event Pckg Enhancemenst
 * 14-Apr-2009  Anuradha Gupta    SPR 19437       Modified fn icf_sic_02_notify_active
 * 27-Apr-2009  Tarun Gupta       SPR 19517       Modified icf_sic_00_inc_options_req
 * 28-May-2009  Tarun Gupta       Rel 8.2         Async NOTIFY Support
 * 02-Jun-2009  Preksha           Rel 8.2         Asynchronus Message enh
 * 31-Aug-2009  Rajiv Kumar       SPR 19999       Merged CSR 1-7502322
 * 16-Sep-2009  Rajiv Kumar       SPR 19999       Merged CSR 1-7615966 
 * 
 ******************************************************************************/

#include "icf_sic_prototypes.h"
#include "icf_port_defs.h"
#include "icf_regm_defs.h"

#ifdef ICF_PORT_WINDOWS
#pragma warning ( disable : 4702 )
#endif


/*******************************************************************************
 *
 * FUNCTION:        icf_sic_00_subs_req
 *
 * DESCRIPTION:     This function is called when - 
 *                  State - ICF_SIC_00_IDLE
 *                  Event - ICF_SIC_EVENT_SUBS_REQ
 *                  This function is called after a new ctx is created.
 *                  It calls the functions to start the response timer and 
 *                  to send Subscribe request to SSA. It also handles the 
 *                  failure to start the response timer.
 *                  
 ******************************************************************************/
icf_return_t icf_sic_00_subs_req(
        INOUT icf_sic_pdb_st  *p_sic_pdb)
{
    icf_return_t         ret_val = ICF_SUCCESS;
    icf_return_t         dummy_retval = ICF_SUCCESS;

 
    ICF_FUNCTION_ENTER(p_sic_pdb->p_glb_pdb)

    /* The context is already created and filled by DBM before this function
     * is called
     */

	 if ( 0 == p_sic_pdb->p_sic_ctx->duration)
	 {
	 	p_sic_pdb->p_sic_ctx->bitmask |= ICF_SIC_POLL_SUBSCRIBE_REQ;
	 }

    if (!(ICF_SIC_EXPIRES_HDR_NOT_TO_BE_SEND  &  p_sic_pdb->p_sic_ctx->bitmask) &&
        !(ICF_SIC_RESUBSCRIPTION_NOT_REQUIRED & p_sic_pdb->p_sic_ctx->bitmask))
    {
        /* Start Subscription Response timer */
        ICF_SIC_SET_TIMER_TYPE(p_sic_pdb,ICF_TIMER_TYPE_SUBS_RESPONSE)
        dummy_retval = icf_sic_start_timer(p_sic_pdb);
    }
    /* If the Subscription Response timer is started then trigger SSA to send 
     * Subscribe Request.
     */
    if (ICF_SUCCESS == dummy_retval)
    {
        /* success or failure from SSA will be handled in this function
         * so ret_val is being ignored */
        /*SPR 13978 Aman 03-01-07*/
		if(ICF_FAILURE == icf_cmn_validate_open_nw_server(p_sic_pdb->p_glb_pdb,
				    p_sic_pdb->line_id))
		{
			p_sic_pdb->p_sic_ctx->error_code =
				ICF_CAUSE_OPEN_NW_SERVER_FAILED;
		    ret_val = ICF_FAILURE;
		}
		else 
		{
			icf_sic_send_subs_req(p_sic_pdb);
#ifdef ICF_LOAD_STAT
                    p_sic_pdb->p_glb_pdb->load_data.subscribe_req++;
#endif
		}
	}
    /* If SIC fails to start the timer go to state 5(blocked) */
    if((ICF_FAILURE == dummy_retval) || (ICF_FAILURE == ret_val))
    {
        ICF_ALARM(((icf_uint8_t *)"[SIC]: Timer Failure"));    
        /* Indicate PA with the failure of subscribe */
        ICF_SIC_SET_CURR_STATUS(
                p_sic_pdb,
                ICF_SUBS_FAIL_NO_RETRY)

		/* ret_val is not being checked because this is returning to FSM
		 * trigger and it will be ignored */
        icf_sic_send_subs_state_to_app(p_sic_pdb);
        
        icf_sic_dealloc_context(p_sic_pdb);    
    }
    ICF_FUNCTION_EXIT(p_sic_pdb->p_glb_pdb)

    return ret_val;
}/* End function */

/*******************************************************************************
 *
 * FUNCTION:        icf_sic_01_subs_req
 *
 * DESCRIPTION:     This function is called when - 
 *                  State - ICF_SIC_01_SUBSCRIBING
 *                  Event - ICF_SIC_EVENT_SUBS_REQ
 *                  This function will reset the event buffer.
 *                  
 ******************************************************************************/
icf_return_t icf_sic_01_subs_req(
        INOUT icf_sic_pdb_st  *p_sic_pdb)
{
    icf_return_t     ret_val = ICF_SUCCESS;
    
    ICF_FUNCTION_ENTER(p_sic_pdb->p_glb_pdb)
        
    /* Check if the auto subscription for reg event package is set*/
    if (ICF_SIC_AUTO_SUBSCRIPTION_FOR_REG_EVENT ==
            (p_sic_pdb->p_sic_ctx->bitmask &
             ICF_SIC_AUTO_SUBSCRIPTION_FOR_REG_EVENT))
    {
        /* Control will be here if reg event package support was
         * enabled and user has got De-Registered for whom this Subscription
         * was sent. Here IPTK will clear this sic ctxt,and stop the
         * current timer.*/
        icf_sic_stop_curr_timer(p_sic_pdb);
  
        icf_sic_send_delink_to_ssa(p_sic_pdb,
                                       (icf_boolean_t)ICF_FALSE);
        icf_sic_dealloc_context(p_sic_pdb);
    }
    else
    {    
        /* Reset the buffer */
        ICF_SIC_RESET_BUFFER(p_sic_pdb)
    }     
    ICF_FUNCTION_EXIT(p_sic_pdb->p_glb_pdb)
    /* Always returns success */        
    return ret_val;
}/* End function */


/*******************************************************************************
 *
 * FUNCTION:        icf_sic_01_unsub_req
 *
 * DESCRIPTION:     This function is called when - 
 *                  State - ICF_SIC_01_SUBSCRIBING
 *                  Event - ICF_SIC_EVENT_UNSUB_REQ
 *                  This function will buffer the unsub req till the response
 *                  of the Subscribe req send is received.
 *                  
 ******************************************************************************/
icf_return_t icf_sic_01_unsub_req(
        INOUT icf_sic_pdb_st      *p_sic_pdb)
{
    icf_return_t     ret_val = ICF_SUCCESS;
    
    ICF_FUNCTION_ENTER(p_sic_pdb->p_glb_pdb)
    
    /* Set the buffer */
    ICF_SIC_SET_BUFFER(p_sic_pdb)

    ICF_FUNCTION_EXIT(p_sic_pdb->p_glb_pdb)

    /* Always returns success */
    return ret_val;    
}/* End function */

/*******************************************************************************
 *
 * FUNCTION:        icf_sic_01_success_resp
 *
 * DESCRIPTION:     This function is called when - 
 *                  State - ICF_SIC_01_SUBSCRIBING
 *                  Event - ICF_SIC_EVENT_SUCCESS_RESP
 *                  This function checks if unsubscribe request is buffered.
 *                  The handling is done on the basis of the current status of
 *                  the buffer.
 *                  
 ******************************************************************************/
icf_return_t icf_sic_01_success_resp(
        INOUT icf_sic_pdb_st      *p_sic_pdb)
{
    icf_return_t         ret_val = ICF_SUCCESS;
    icf_return_t         dummy_retval = ICF_SUCCESS;
    icf_nw_inc_subs_resp_st  *p_subs_resp = ICF_NULL;
    
    ICF_FUNCTION_ENTER(p_sic_pdb->p_glb_pdb)
    
    /* Stop Subscription Response timer */
    icf_sic_stop_curr_timer(p_sic_pdb);
    
    p_subs_resp = (icf_nw_inc_subs_resp_st *)(p_sic_pdb->p_msg);
    
	/* For POLL subscribe Request we need to wait for notify request.
	 * So we will start the timer of 128 Seconds
	 */
	if ( p_sic_pdb->p_sic_ctx->bitmask & ICF_SIC_POLL_SUBSCRIBE_REQ)
	{
		ICF_SIC_RESET_BUFFER(p_sic_pdb);
		icf_cmn_free_header_list(p_sic_pdb->p_glb_pdb,
			&(p_sic_pdb->p_sic_ctx->header_list),
			p_sic_pdb->p_ecode);
		ICF_SIC_SET_TIMER_TYPE(p_sic_pdb,ICF_TIMER_TYPE_AWAIT_SUBS_NOTIFY_REQ)
		dummy_retval = icf_sic_start_timer(p_sic_pdb);
        if(ICF_FAILURE == dummy_retval)
        {
            ICF_ALARM(((icf_uint8_t *)"[SIC]: Timer Failure"));
        }
	}
    /* If the buffer is NULL then */
    else if (ICF_FALSE == p_sic_pdb->p_sic_ctx->event_buffer)
    {
        /*ICF_SIC_TRACE(p_sic_pdb,ICF_TRACE_SIC_EVENT_BUFFER)*/

		/* Fix - In case NOTIFY is received before 200 OK of SUBS, then the 
		 * subs_duration timer would have already been started in 
		 * icf_sic_01_notify_active() handler */
		if (ICF_TIMER_TYPE_SUBS_DURATION != p_sic_pdb->p_sic_ctx->timer_type)
		{
			/* Stop any currently running timer */
			icf_sic_stop_curr_timer(p_sic_pdb);
			
			/* If duration is received in the response */
                     if (!(ICF_SIC_RESUBSCRIPTION_NOT_REQUIRED & p_sic_pdb->p_sic_ctx->bitmask))
                     {
			if (ICF_NW_INC_SUBS_DURATION_PRESENT & p_subs_resp->bitmask)
			{
            /* Duration of the timer will be the duration received in the 
			* response
				*/
				/* Fix - timer should be started for half the duration */
				/* Fix for timer units - 
				* timer_duration == is in milliseconds
				* p_subs_resp->duration == is in seconds
				* p_sic_ctx->duration == is in seconds */
/* Start: Multiple Public Users ICF5.0 */
				if(icf_port_strcmp( p_sic_pdb->p_sic_ctx->event_package.str, (icf_uint8_t *)"reg" ) == 0 )
				{
					if((p_subs_resp->duration * 1000)> ICF_REG_EVENT_BASE_DURATION)
					{
						p_sic_pdb->timer_duration = ((p_subs_resp->duration * 1000)-ICF_REG_EVENT_MARGIN_TIME);
					}
					else
					{
						p_sic_pdb->timer_duration = (p_subs_resp->duration * 1000) / 2;						
					}
				}
				else
				{
					p_sic_pdb->timer_duration = (p_subs_resp->duration * 1000) / 2;
				}
/* End: Multiple Public Users ICF5.0 */
				p_sic_pdb->p_sic_ctx->duration = (p_subs_resp->duration);
			}
			/* Duration is not present in the response */
			else
			{
               /* Duration will be the duration send i.e. the duration in the ctx
				*/ 
			   /* Fix - timer should be started for half the duration */
			   /* Fix for timer units - 
				* timer_duration == is in milliseconds
				* p_sic_ctx->duration == is in seconds */
				p_sic_pdb->timer_duration = 
					(p_sic_pdb->p_sic_ctx->duration * 1000)/ 2;
			}
			
			ICF_SIC_SET_TIMER_TYPE(p_sic_pdb,ICF_TIMER_TYPE_SUBS_DURATION)
				
			/* Start the Subscription Duration timer. If SIC fails to start 
			* the timer then it will retry when first notify will be received 
			* thus the failure of the starting of the timer is ignored here
			*/
			dummy_retval = icf_sic_start_timer(p_sic_pdb);

            if(ICF_FAILURE == dummy_retval)
            {
                ICF_ALARM(((icf_uint8_t *)"[SIC]: Timer Failure"));
            }
            }
        } /* else if(p_sic_pdb->p_sic_ctx->timer_type) */

        /* Indicate the Subscription State.
         * Go to state 2.
         */

        ICF_SIC_SET_STATE(p_sic_pdb,ICF_SIC_02_SUBSCRIBED)
            
		ICF_SIC_SET_CURR_STATUS(
                p_sic_pdb,
                ICF_SUBS_ACCEPTED)

		ret_val = icf_sic_send_subs_state_to_app(p_sic_pdb);
    }/* if (ICF_FALSE ==..*/
    /* If the buffer is not NULL then */
    else
    {
        /*ICF_SIC_TRACE(p_sic_pdb,ICF_TRACE_SIC_EVENT_BUFFER)*/


        ICF_SIC_RESET_BUFFER(p_sic_pdb)
            
        /* Start Subscription Response timer */
        ICF_SIC_SET_TIMER_TYPE(p_sic_pdb,ICF_TIMER_TYPE_SUBS_RESPONSE)
            
        dummy_retval = icf_sic_start_timer(p_sic_pdb);           
        
        /* If the response timer is started then trigger SSA to send 
         * Unsubscribe Request */
        if (ICF_SUCCESS == dummy_retval)
        {
            /* free the header list and body list for internally generated unsubscribe request*/
			if( p_sic_pdb->p_sic_ctx->header_list.count)
			{
				icf_cmn_free_header_list(p_sic_pdb->p_glb_pdb,
					&(p_sic_pdb->p_sic_ctx->header_list),
					p_sic_pdb->p_ecode);
			}
			if( p_sic_pdb->p_sic_ctx->body_list.count)
			{
				icf_cmn_free_msg_body_list(p_sic_pdb->p_glb_pdb,
					&(p_sic_pdb->p_sic_ctx->body_list),
					p_sic_pdb->p_ecode);
			}

			/* ret_val is being ignored since this will return to FSM only */ 
            icf_sic_send_unsub_req(p_sic_pdb);   
        }
        
        /* If SIC fails to start the timer then indicate with the failure, 
         * trigger SSA to send unsubscribe request (for best effort), 
         * trigger SSA to clear the ctx, clear SIC ctx
         */
        else
        {
            ICF_ALARM(((icf_uint8_t *)"[SIC]: Timer Failure"));
            /*ICF_SIC_TRACE(p_sic_pdb,ICF_TRACE_SIC_TIMER_FAIL)*/

			p_sic_pdb->best_effort_unsub = ICF_TRUE;
			/* best effort so ret_val is ignored */
            icf_sic_send_unsub_req(p_sic_pdb);
            
            ICF_SIC_SET_CURR_STATUS(
                    p_sic_pdb,
                    ICF_SUBS_UNSUBSCRIBED)

            /* ret_val is being ignored since this will return to FSM only */ 
            icf_sic_send_subs_state_to_app(p_sic_pdb);

            icf_sic_send_delink_to_ssa(p_sic_pdb,
                    (icf_boolean_t)ICF_FALSE);            
            
            icf_sic_dealloc_context(p_sic_pdb);    
        }/* if (ICF_FAILURE ==.. */
    }/* End else */
    
    ICF_FUNCTION_EXIT(p_sic_pdb->p_glb_pdb)
    /* Always returns success */        
    return ret_val;
}/* End function */


/*******************************************************************************
 *
 * FUNCTION:        icf_sic_01_failure_resp_no_retry
 *
 * DESCRIPTION:     This function is called when - 
 *                  State - ICF_SIC_01_SUBSCRIBING
 *                  Event - ICF_SIC_EVENT_FAILURE_RESP_NO_RETRY
 *                  The handling is done on the basis of the event buffer.
 *                  
 ******************************************************************************/
icf_return_t icf_sic_01_failure_resp_no_retry(
        INOUT icf_sic_pdb_st      *p_sic_pdb)
{
    icf_return_t     ret_val = ICF_SUCCESS;
    
    ICF_FUNCTION_ENTER(p_sic_pdb->p_glb_pdb)
    
    /* Stop Subscription Response timer */
    icf_sic_stop_curr_timer(p_sic_pdb);

    /* No retry will be done now so SSA ctx is cleared */
    icf_sic_send_delink_to_ssa(p_sic_pdb,
            (icf_boolean_t)ICF_FALSE); 
    
    /* If the buffer is NULL then indicate subscription failure and 
     * go to state 6 (awt delete)
     */
    if (ICF_FALSE == p_sic_pdb->p_sic_ctx->event_buffer)
    {
        /*ICF_SIC_TRACE(p_sic_pdb,ICF_TRACE_SIC_EVENT_BUFFER)*/


		/* For Poll Subscribe Request ICF will deallocate SIC context
		 * and ICF will send UNSUBSCRIBED to the application
		 */
		if ( !(p_sic_pdb->p_sic_ctx->bitmask & ICF_SIC_POLL_SUBSCRIBE_REQ))
		{
			ICF_SIC_SET_CURR_STATUS(
					p_sic_pdb,
					ICF_SUBS_FAIL_NO_RETRY)

			/* ret_val is being ignored since this will return to FSM only */
			icf_sic_send_subs_state_to_app(p_sic_pdb);

			icf_sic_dealloc_context(p_sic_pdb);
			    
        }
		else
		{
			ICF_SIC_SET_CURR_STATUS(
				p_sic_pdb,
				ICF_SUBS_UNSUBSCRIBED)

			ret_val = icf_sic_send_subs_state_to_app(p_sic_pdb);
			icf_sic_dealloc_context(p_sic_pdb);
		}
	}   
    /* If the buffer is not NULL then Indicate Unsubscribe success response 
     * and Clear SIC context
     */
    else
    {
        /*ICF_SIC_TRACE(p_sic_pdb,ICF_TRACE_SIC_EVENT_BUFFER)*/


        ICF_SIC_RESET_BUFFER(p_sic_pdb)
            
        ICF_SIC_SET_CURR_STATUS(
                p_sic_pdb,
                ICF_SUBS_UNSUBSCRIBED)

		/* ret_val is being ignored since this will return to FSM only */ 
        icf_sic_send_subs_state_to_app(p_sic_pdb);
        
        icf_sic_dealloc_context(p_sic_pdb);    
    }
    
    ICF_FUNCTION_EXIT(p_sic_pdb->p_glb_pdb)
    /* Always returns success */        
    return ret_val;
}/* End function */


/*******************************************************************************
 *
 * FUNCTION:        icf_sic_01_failure_resp_with_retry
 *
 * DESCRIPTION:     This function is called when - 
 *                  State - ICF_SIC_01_SUBSCRIBING
 *                  Event - ICF_SIC_EVENT_FAILURE_RESP_WITH_RETRY
 *                  The handling is done on the basis of the cuurent state of
 *                  the buffer.
 *                  
 ******************************************************************************/
icf_return_t icf_sic_01_failure_resp_with_retry(
        INOUT icf_sic_pdb_st      *p_sic_pdb)
{
    icf_return_t     ret_val = ICF_SUCCESS;
    icf_return_t     dummy_retval = ICF_SUCCESS; 
    icf_nw_inc_subs_resp_st  *p_subs_resp;
    
    ICF_FUNCTION_ENTER(p_sic_pdb->p_glb_pdb)
    
    /* Stop Subscription Response timer */
    icf_sic_stop_curr_timer(p_sic_pdb);

    p_subs_resp = (icf_nw_inc_subs_resp_st *)(p_sic_pdb->p_msg);

    /* If the buffer is NULL then Start Subscription Retry timer */  
    if (ICF_FALSE == p_sic_pdb->p_sic_ctx->event_buffer)
    {
        /*ICF_SIC_TRACE(p_sic_pdb,ICF_TRACE_SIC_EVENT_BUFFER)*/


        /* If duration is received in the response */
        if (ICF_NW_INC_SUBS_DURATION_PRESENT & p_subs_resp->bitmask)
        {
            /* Fix for SPR: 19999 (Merged CSR 1-7615966 : SPR 20030)*/
            /* timer will be of the duration received */
            p_sic_pdb->timer_duration = (p_subs_resp->duration)*1000;
        }
        
        /* If the duration is not received then */
        else
        {
            /* timer duration will be the configured duration */            
            p_sic_pdb->timer_duration = p_sic_pdb->p_config_data->
                subs_retry_timer;
        }
        ICF_SIC_SET_TIMER_TYPE(p_sic_pdb,ICF_TIMER_TYPE_SUBS_RETRY)
        
        dummy_retval = icf_sic_start_timer(p_sic_pdb);
  
        /* If the timer is started then indicate Subscribe failure Response 
         * and go to state 4
         */
        
        if (ICF_SUCCESS == dummy_retval)
        {
            /*ICF_SIC_TRACE(p_sic_pdb,ICF_TRACE_SIC_TIMER_SUCCESS)*/


            ICF_SIC_SET_STATE(p_sic_pdb,ICF_SIC_04_SUBSCRIBE_RETRY)

            ICF_SIC_SET_CURR_STATUS(
                    p_sic_pdb,
                    ICF_SUBS_FAIL_RETRY)

			/* ret_val is being ignored since this will return to FSM only */ 
            icf_sic_send_subs_state_to_app(p_sic_pdb);
        }
        
        /* If SIC fails to start the timer then indicate the failure of 
         * Subscribe and go to state 5
         */
        else
        {
            ICF_ALARM(((icf_uint8_t *)"[SIC]: Timer Failure"));
            ICF_SIC_SET_CURR_STATUS(
                    p_sic_pdb,
                    ICF_SUBS_FAIL_NO_RETRY)

			/* ret_val is being ignored since this will return to FSM only */ 
            icf_sic_send_subs_state_to_app(p_sic_pdb);

			icf_sic_send_delink_to_ssa(p_sic_pdb,
						(icf_boolean_t)ICF_FALSE);            
            
			icf_sic_dealloc_context(p_sic_pdb);    
        }
    }/* if (ICF_FALSE ==.. */
    /* If the buffer is not NULL then */
    else
    {
        /*ICF_SIC_TRACE(p_sic_pdb,ICF_TRACE_SIC_EVENT_BUFFER)*/

        /* a)Indicate Unsubscription Success Response.
         * b) Trigger SSA to clear its context.
         * c) Clear SIC context.
         * d) Go to state 0.
         */
        
        ICF_SIC_RESET_BUFFER(p_sic_pdb)
            
        ICF_SIC_SET_CURR_STATUS(
                    p_sic_pdb,
                    ICF_SUBS_UNSUBSCRIBED)

		/* ret_val is being ignored since this will return to FSM only */ 
        icf_sic_send_subs_state_to_app(p_sic_pdb);
        
        icf_sic_send_delink_to_ssa(p_sic_pdb,
                (icf_boolean_t)ICF_FALSE); 
        
        icf_sic_dealloc_context(p_sic_pdb);    
    }
    
    ICF_FUNCTION_EXIT(p_sic_pdb->p_glb_pdb)
    return ret_val;
}/* End function */


/*******************************************************************************
 *
 * FUNCTION:        icf_sic_01_notify_active 
 *
 * DESCRIPTION:     This function is called when - 
 *                  State - ICF_SIC_01_SUBSCRIBING
 *                  Event - ICF_SIC_EVENT_NOTIFY_ACTIVE
 *                  Notify (active) is handled before the response of 
 *                  subscribe and is treated as the success response.
 *                  
 ******************************************************************************/
icf_return_t icf_sic_01_notify_active(
        INOUT icf_sic_pdb_st      *p_sic_pdb)
{
    icf_return_t         ret_val = ICF_SUCCESS;
    icf_return_t         dummy_retval = ICF_SUCCESS;
    icf_nw_inc_notify_ind_st *p_notify_ind = ICF_NULL;
    
    ICF_FUNCTION_ENTER(p_sic_pdb->p_glb_pdb)
        
    p_notify_ind = (icf_nw_inc_notify_ind_st *)(p_sic_pdb->p_msg);          
    
    /* Stop Subscription Response timer
     * Trigger SSA to send Notify Response. 
     * Indicate the Notify data to the corresponding module
     */
   
    /* 
       Send notify response only if application has not enabled support
       of asynchronous NOTIFY or p_sic_pdb->p_sic_ctx->event_buffer is set.
       If the event buffer is set, no ICF_NOTIFY_IND is being given to the 
       application, thus we need to send the NOTIFY response here only.
    */   
    if(((ICF_NULL != p_sic_pdb->p_config_data) &&
        (ICF_FALSE == p_sic_pdb->p_config_data->async_notify_support_reqd)) ||
       (ICF_TRUE == p_sic_pdb->p_sic_ctx->event_buffer))
    {
        /*
           For this NOTIFY, no ICF_NOTIFY_IND will be given to
           application and it will be responded with a 200.
        */   
        p_sic_pdb->resp_code = 200;
        /* Send notify msg to the corresponding module */
	    p_sic_pdb->notify_resp = ICF_SUCCESS;
	    ret_val = icf_sic_send_notify_resp(p_sic_pdb);

        /* 
           Set bit in SIC context to indicate that NOTIFY has been
           responded to. This bit will be checked in case of a polled
           SUBSCRIBE request when application has enabled support
           of asynchronous NOTIFY.If the bit is not set, NOTIFY will
           be responded to.
        */
        p_sic_pdb->p_sic_ctx->bitmask |= ICF_SIC_NOTIFY_RESP_SENT_TO_PEER;
    }
       
	/* For poll subscribe request we will not receive NOTIFY with subs 
	 * State as ACTIVE or Pending So we will ignore the active notify
	 */
    if(p_sic_pdb->p_sic_ctx->bitmask & ICF_SIC_POLL_SUBSCRIBE_REQ)
	{
        if(ICF_SIC_NOTIFY_RESP_SENT_TO_PEER !=
                (ICF_SIC_NOTIFY_RESP_SENT_TO_PEER &
                    p_sic_pdb->p_sic_ctx->bitmask))
        {
            /* NOTIFY has not been responded to, send response now. */
            p_sic_pdb->notify_resp = ICF_SUCCESS;
            /*
               For this NOTIFY, no ICF_NOTIFY_IND will be given to
               application and it will be responded with a 200.
            */   
            p_sic_pdb->resp_code = 200;
	        ret_val = icf_sic_send_notify_resp(p_sic_pdb);
            /* Reset bit set above */
            p_sic_pdb->p_sic_ctx->bitmask &= ~ICF_SIC_NOTIFY_RESP_SENT_TO_PEER;
        }
		icf_sic_unexpected_msg(p_sic_pdb);
        ICF_FUNCTION_EXIT(p_sic_pdb->p_glb_pdb)
		return ret_val;
	}

    icf_sic_stop_curr_timer(p_sic_pdb);
    
    /* If the buffer is NULL then start Subscription duration timer of the 
     * duration if received in NOTIFY other wise of the duration present in 
     * the context
     */
    if (ICF_FALSE == p_sic_pdb->p_sic_ctx->event_buffer)
    {
        /*ICF_SIC_TRACE(p_sic_pdb,ICF_TRACE_SIC_EVENT_BUFFER)*/

		
	    /* Indicate subscription success */
		ICF_SIC_SET_CURR_STATUS(
					p_sic_pdb,
					ICF_SUBS_SUBSCRIBED)

		p_sic_pdb->send_htb_list = ICF_FALSE; 
		ret_val = icf_sic_send_subs_state_to_app(p_sic_pdb);

		p_sic_pdb->send_htb_list = ICF_TRUE; 
		ret_val = icf_sic_send_notify_msg_to_app(p_sic_pdb);

		/* If duration is received in NOTIFY */
		if (ICF_NW_INC_SUBS_DURATION_PRESENT & p_notify_ind->bitmask)
		{
			/* Fix - timer should be started for half the duration */
			/* Fix for timer units - 
			 * timer_duration == is in milliseconds
			 * p_notify_ind->duration == is in milliseconds
			 * p_sic_ctx->duration == is in seconds */
			p_sic_pdb->timer_duration = p_notify_ind->duration / 2;
			/* Line below is commented in ICF 4.2 Support merging */
			/*p_sic_pdb->p_sic_ctx->duration = p_notify_ind->duration / 1000;*/
		}        
		/* if duration is not present */
		else
		{
			/* Fix - timer should be started for half the duration */
			/* Fix for timer units - 
			 * timer_duration == is in milliseconds
			 * p_sic_ctx->duration == is in seconds */
			p_sic_pdb->timer_duration = (p_sic_pdb->p_sic_ctx->duration * 1000) / 2;
		}

		ICF_SIC_SET_TIMER_TYPE(p_sic_pdb,ICF_TIMER_TYPE_SUBS_DURATION)    
		/* failure to start the duration timer is ignored as if this fails then
		 * this will be started on receiving next notify
		 */ 
		dummy_retval = icf_sic_start_timer(p_sic_pdb);

		if(ICF_FAILURE == dummy_retval)
        {
            ICF_ALARM(((icf_uint8_t *)"[SIC]: Timer Failure"));
        } 
    }
    /* If the buffer is not NULL then */
    else
    {
        /* Start Subscription Response timer */
        ICF_SIC_SET_TIMER_TYPE(p_sic_pdb,ICF_TIMER_TYPE_SUBS_RESPONSE)    
        dummy_retval = icf_sic_start_timer(p_sic_pdb);        
        
        /* If the Response timer is started then 
         * trigger SSA to send Unsubscribe Request
         */
        if (ICF_SUCCESS == dummy_retval)
        {
            /* 
               Moved resetting of buffer from top level to here. This has been
               done to enable deallocation of SIC context when application
               has enabled support of asynchronous NOTIFY and icf_sic_start_timer
               function fails.
            */   

            ICF_SIC_RESET_BUFFER(p_sic_pdb)
			/* ret_val is being ignored since this will return to FSM only */ 
            icf_sic_send_unsub_req(p_sic_pdb);
        }
        
        else
        {
            ICF_ALARM(((icf_uint8_t *)"[SIC]: Timer Failure"));

            /* If SIC fails to start the Subscription Response timer then 
             * indicate the failure, 
             * send unsubscribe request (for best effort), 
             * trigger SSA to clear the ctx, 
             * clear SIC ctx and go to state 0
             */
                
            ICF_SIC_SET_CURR_STATUS(
                    p_sic_pdb,
                    ICF_SUBS_UNSUBSCRIBED)

			/* ret_val is being ignored since this will return to FSM only */             
			icf_sic_send_subs_state_to_app(p_sic_pdb);

			p_sic_pdb->best_effort_unsub = ICF_TRUE;

            /* best effort so ret_val is ignored */
			icf_sic_send_unsub_req(p_sic_pdb);
           
            /* 
               If asynchronous NOTIFY support is enabled or 
               p_sic_pdb->p_sic_ctx->event_buffer is not set, do not 
               deallocate SIC context here. Instead set a bit in 
               SIC context to indicate that SIC context needs to be 
               deallocated in the 05 state.If the event_buffer is set,
               no ICF_NOTIFY_IND is given to the application,hence
               deallocate SIC context here only.
            */   
            if(((ICF_NULL != p_sic_pdb->p_config_data) &&
                (ICF_FALSE == p_sic_pdb->p_config_data->async_notify_support_reqd)) ||
               (ICF_TRUE == p_sic_pdb->p_sic_ctx->event_buffer))
            {
                /* 
                  If application has enabled support of asynchronous NOTIFY and 
                  event_buffer is set in the SIC context, we need to deallocate
                  SIC context in this function only. The above check does the same.
                  It checks for event_buffer being set. Thus the resetting of the
                  event_buffer has been moved from the top of this block to here.
                */ 
                ICF_SIC_RESET_BUFFER(p_sic_pdb)

                icf_sic_send_delink_to_ssa(p_sic_pdb,
                    (icf_boolean_t)ICF_FALSE);
            
                icf_sic_dealloc_context(p_sic_pdb);    
            }
            else
            {
                p_sic_pdb->p_sic_ctx->bitmask |= ICF_SIC_CONTEXT_TO_BE_DEALLOCATED;
            }

        }/* if (ICF_FAILURE ==.. */
    }/* End else */

    /* If the SIC context has been deallocated, return from the function */
    ICF_CHECK_IF_VAR_IS_NULL(p_sic_pdb->p_glb_pdb,
                             p_sic_pdb->p_sic_ctx,
                             dummy_retval)
    if(ICF_FAILURE == dummy_retval)
    {
        /* no need to over write the ret_val value */
        ICF_FUNCTION_EXIT(p_sic_pdb->p_glb_pdb)
        return ret_val;
    }

    /* 
       If the bit ICF_SIC_NOTIFY_IND_SENT_TO_APP is set in SIC context, it means that application
       has enabled support of asynchronous NOTIFY and ICF_NOTIFY_IND has been given
       to the application. We will start a timer, save current FSM state and move to state 05.
    */
    if(ICF_SIC_NOTIFY_IND_SENT_TO_APP == 
            (ICF_SIC_NOTIFY_IND_SENT_TO_APP & p_sic_pdb->p_sic_ctx->bitmask))
    {
        icf_sic_move_to_state_await_notify_cfm(p_sic_pdb);
        
        /* reset the bit to indicate that ICF_NOTIFY_IND has been given to application */
        p_sic_pdb->p_sic_ctx->bitmask &= ~ICF_SIC_NOTIFY_IND_SENT_TO_APP;
    }

    ICF_FUNCTION_EXIT(p_sic_pdb->p_glb_pdb)
    return ret_val;
}/* End function */


/*******************************************************************************
 *
 * FUNCTION:        icf_sic_01_resp_timer_exp 
 *
 * DESCRIPTION:     This function is called when - 
 *                  State - ICF_SIC_01_SUBSCRIBING
 *                  Event - ICF_SIC_EVENT_RESP_TIMER_EXP
 *                  The handling is done on the basis of event buffer.
 *                  
 ******************************************************************************/
icf_return_t icf_sic_01_resp_timer_exp(
        INOUT icf_sic_pdb_st      *p_sic_pdb)
{
    icf_return_t     ret_val = ICF_SUCCESS;
    icf_return_t     dummy_retval = ICF_SUCCESS;
    
    ICF_FUNCTION_ENTER(p_sic_pdb->p_glb_pdb)
   
    ICF_SIC_APP_TRACE(p_sic_pdb,ICF_TRACE_SUBS_TIMER_EXPIRE)

 
    /* If the buffer is NULL then */
    if (ICF_FALSE == p_sic_pdb->p_sic_ctx->event_buffer)
    {
        /*ICF_SIC_TRACE(p_sic_pdb,ICF_TRACE_SIC_EVENT_BUFFER)*/

        if(!(p_sic_pdb->p_sic_ctx->bitmask & ICF_SIC_SUBSCRIBE_ALREADY_RETRIED))
         { 
            /* Start Subscription retry timer */
        
            p_sic_pdb->timer_duration = 
                 p_sic_pdb->p_config_data->subs_retry_timer;

            ICF_SIC_SET_TIMER_TYPE(p_sic_pdb,ICF_TIMER_TYPE_SUBS_RETRY)    
        
            dummy_retval = icf_sic_start_timer(p_sic_pdb); 
        
            /* If the retry timer is started then goto 4 */
            if (ICF_SUCCESS == dummy_retval)
             {
               /*ICF_SIC_TRACE(p_sic_pdb,ICF_TRACE_SIC_TIMER_SUCCESS)*/
               
               p_sic_pdb->p_sic_ctx->bitmask |= ICF_SIC_SUBSCRIBE_ALREADY_RETRIED;

               /* Indicate the subscription status */
               ICF_SIC_SET_CURR_STATUS(
                    p_sic_pdb,
                    ICF_SUBS_FAIL_RETRY)

               /* ret_val is being ignored since this will return to FSM only */
			icf_sic_send_subs_state_to_app(p_sic_pdb);
            
               ICF_SIC_SET_STATE(p_sic_pdb,ICF_SIC_04_SUBSCRIBE_RETRY)
             }
        
            /* If SIC fails to start the timer then go to state 5 */
           else
             {
                ICF_ALARM(((icf_uint8_t *)"[SIC]: Timer Failure"));
                ICF_SIC_SET_CURR_STATUS(
                    p_sic_pdb,
                    ICF_SUBS_FAIL_NO_RETRY)

               /* ret_val is being ignored since this will return to FSM only */ 
		icf_sic_send_subs_state_to_app(p_sic_pdb);

		icf_sic_send_delink_to_ssa(p_sic_pdb,
						(icf_boolean_t)ICF_FALSE);            
            
		icf_sic_dealloc_context(p_sic_pdb);   
             } 

         }
         else
            {
              /*We will not going to retry SUBSCRIBE again after one retry
               * when we received 200 OK of initial SUBSCRIBE without contact
               * header */
               ICF_SIC_SET_CURR_STATUS(
                    p_sic_pdb,
                    ICF_SUBS_FAIL_NO_RETRY)
               
               /* ret_val is being ignored since this will return to FSM only */
                icf_sic_send_subs_state_to_app(p_sic_pdb);

                icf_sic_send_delink_to_ssa(p_sic_pdb,
                                                (icf_boolean_t)ICF_FALSE);

                icf_sic_dealloc_context(p_sic_pdb);    
             }

    }/* if (ICF_FALSE ==..*/
    
    /* If the buffer is not NULL then */
    else
    {
        /*ICF_SIC_TRACE(p_sic_pdb,ICF_TRACE_SIC_EVENT_BUFFER)*/

        
        ICF_SIC_SET_CURR_STATUS(
                    p_sic_pdb,
                    ICF_SUBS_UNSUBSCRIBED)
        
        /* ret_val is being ignored since this will return to FSM only */ 
		icf_sic_send_subs_state_to_app(p_sic_pdb);
        
        icf_sic_send_delink_to_ssa(p_sic_pdb,
                (icf_boolean_t)ICF_FALSE);
        
        icf_sic_dealloc_context(p_sic_pdb);    
    }
    
    ICF_FUNCTION_EXIT(p_sic_pdb->p_glb_pdb)

    /* Always returns success */
    return ret_val;
}/* End function */

/*******************************************************************************
 *
 * FUNCTION:        icf_sic_01_03_notify_timer_exp 
 *
 * DESCRIPTION:     This function is called when - 
 *                  State - ICF_SIC_01_SUBSCRIBING
 *                  Event - ICF_SIC_EVENT_AWAIT_SUBS_NOTIFY_TIMER_EXP
 *                  .
 *                  
 ******************************************************************************/
icf_return_t icf_sic_01_03_notify_timer_exp(
        INOUT icf_sic_pdb_st      *p_sic_pdb)
{
    icf_return_t     ret_val = ICF_SUCCESS;
    
    ICF_FUNCTION_ENTER(p_sic_pdb->p_glb_pdb)

    /* Indicate for Unsubscribe Success Response
     * Trigger SSA to clear the context
     * Clear SIC context.
     * 
     */
    ICF_SIC_SET_CURR_STATUS(
            p_sic_pdb,
            ICF_SUBS_UNSUBSCRIBED)

	/* ret_val is being ignored since this will return to FSM only */ 
	icf_sic_send_subs_state_to_app(p_sic_pdb);

    icf_sic_send_delink_to_ssa(p_sic_pdb,
                (icf_boolean_t)ICF_FALSE);
        
    icf_sic_dealloc_context(p_sic_pdb);    

    ICF_FUNCTION_EXIT(p_sic_pdb->p_glb_pdb)
	return ret_val;
}


/*******************************************************************************
 *
 * FUNCTION:        icf_sic_02_subs_req 
 *
 * DESCRIPTION:     This function is called when - 
 *                  State - ICF_SIC_02_SUBSCRIBED
 *                  Event - ICF_SIC_EVENT_SUBS_REQ
 *                  It triggers SSA to send the REFRESH Subscribe request.
 *                  
 ******************************************************************************/
icf_return_t icf_sic_02_subs_req(
        INOUT icf_sic_pdb_st      *p_sic_pdb)
{
	icf_return_t     ret_val = ICF_SUCCESS;
    icf_return_t     dummy_retval = ICF_SUCCESS;
    
    ICF_FUNCTION_ENTER(p_sic_pdb->p_glb_pdb)
    
    /* Stop Subscription Duration timer and start Subscription Response timer
     */
    
    icf_sic_stop_curr_timer(p_sic_pdb);
    /*Checking whether the config data in sic pdb is NULL or not .If it is
     * null fetch the config data in sic pdb . */

    if (ICF_NULL == p_sic_pdb->p_config_data)
    {
        /* Request DBM to give the config data pointer */
        ret_val = icf_dbm_get_module_glb_data(
                   p_sic_pdb->p_glb_pdb,
                   (icf_int_module_id_t)ICF_INT_MODULE_CFG,
                   (icf_void_t *)&(p_sic_pdb->p_config_data),
                   p_sic_pdb->p_ecode);
    }    


    /* to identify the trigger is from Reg module, check for existence of bitmask
     * instead of system level configuration check for reg event pcks subs suppport
     */ 
    if(ICF_SIC_AUTO_SUBSCRIPTION_FOR_REG_EVENT ==
            (ICF_SIC_AUTO_SUBSCRIPTION_FOR_REG_EVENT & p_sic_pdb->p_sic_ctx->bitmask))    
    {
        /* Control will be here if reg event package support was
         * enabled and user has got De-Registered for whom this Subscription
         * was sent. Here IPTK will not clear this sic ctxt, it will just start
         * await Noitfy timer. This sic ctxt will be cleared on receiving Notify
         * or timer expiry for this timer that we are going to start.
         * Also, state will not be changed.
         */
        ICF_SIC_SET_TIMER_TYPE(p_sic_pdb,
                               ICF_TIMER_TYPE_AWAIT_SUBS_NOTIFY_REQ)
        if (ICF_FAILURE == icf_sic_start_timer(p_sic_pdb))
        {
            /* Failure in starting await notify timer, so clear sic ctx here.*/
            icf_sic_send_delink_to_ssa(p_sic_pdb,
                                       (icf_boolean_t)ICF_FALSE);
            icf_sic_dealloc_context(p_sic_pdb);    
        }
    }
    else
    {
       ICF_SIC_SET_TIMER_TYPE(p_sic_pdb,ICF_TIMER_TYPE_SUBS_RESPONSE)

       ICF_SIC_APP_TRACE(p_sic_pdb, ICF_TRACE_RESUBS)
    
       dummy_retval = icf_sic_start_timer(p_sic_pdb);
   
       /* If the Response timer is started then trigger SSA to send 
        * Unsusbcribe Request
        */
       if (ICF_SUCCESS == dummy_retval)
       {
           p_sic_pdb->trace_alrdy_sent = ICF_TRUE;
           /* Failure to send will be handled in this function */
           /* ret_val is being ignored since this will return to FSM only */ 
           icf_sic_send_subs_req(p_sic_pdb);
       }
       /* If SIC fails to start the timer then indicate the source module with 
        * the failure, */    
       else
       {
        ICF_ALARM(((icf_uint8_t *)"[SIC]: Timer Failure")); 
           /* Indicate PA with the failure of subscribe */
           ICF_SIC_SET_CURR_STATUS(
                 p_sic_pdb,
                 ICF_SUBS_FAIL_NO_RETRY)

	       /* ret_val is being ignored since this will return to FSM only */ 
           icf_sic_send_subs_state_to_app(p_sic_pdb);

		   icf_sic_send_delink_to_ssa(p_sic_pdb,
					(icf_boolean_t)ICF_FALSE);            
        
	       icf_sic_dealloc_context(p_sic_pdb);    
       }
    }

    ICF_FUNCTION_EXIT(p_sic_pdb->p_glb_pdb)
    /* always returns success */        
    return ret_val;


}/* Function icf_sic_02_subs_req() ends here */


/*******************************************************************************
 *
 * FUNCTION:        icf_sic_02_unsub_req 
 *
 * DESCRIPTION:     This function is called when - 
 *                  State - ICF_SIC_02_SUBSCRIBED
 *                  Event - ICF_SIC_EVENT_UNSUB_REQ
 *                  It sends Unsubscribe request to SSA.
 *                  
 ******************************************************************************/
icf_return_t icf_sic_02_unsub_req(
        INOUT icf_sic_pdb_st      *p_sic_pdb)
{
    icf_return_t     ret_val = ICF_SUCCESS;
    icf_return_t     dummy_retval = ICF_SUCCESS;
    
    ICF_FUNCTION_ENTER(p_sic_pdb->p_glb_pdb)
    
    /* Stop Subscription Duration timer and start Subscription Response timer
     */
    
    icf_sic_stop_curr_timer(p_sic_pdb);
    
    ICF_SIC_SET_TIMER_TYPE(p_sic_pdb,ICF_TIMER_TYPE_SUBS_RESPONSE)    
    dummy_retval = icf_sic_start_timer(p_sic_pdb);
   
    /* If the Response timer is started then trigger SSA to send 
     * Unsusbcribe Request
     */
    if (ICF_SUCCESS == dummy_retval)
    {
        /* Failure to send will be handled in this function */
		/* ret_val is being ignored since this will return to FSM only */ 
        icf_sic_send_unsub_req(p_sic_pdb);
    }
    
    /* If SIC fails to start the timer then indicate the source module with 
     * the failure, trigger ssa to send unsubscribe request, trigger SSA to 
     * clear the ctx and clear SIC ctx.
     */
    
    else
    {
		ICF_ALARM(((icf_uint8_t *)"[SIC]: Timer Failure")); 

        ICF_SIC_SET_CURR_STATUS(
                p_sic_pdb,
                ICF_SUBS_UNSUBSCRIBED)

        ret_val = icf_sic_send_subs_state_to_app(p_sic_pdb);

        /* This unsub request is send for best effort and thus failure from
         * SSA is ignored  */
		p_sic_pdb->best_effort_unsub = ICF_TRUE;

		/* best effort so ret_val is ignored */
        icf_sic_send_unsub_req(p_sic_pdb);
        
        icf_sic_send_delink_to_ssa(p_sic_pdb,
                (icf_boolean_t)ICF_FALSE);
        
        icf_sic_dealloc_context(p_sic_pdb);    
    }
    
    ICF_FUNCTION_EXIT(p_sic_pdb->p_glb_pdb)
    /* always returns success */        
    return ret_val;
}/* End function */


/*******************************************************************************
 *
 * FUNCTION:        icf_sic_02_success_resp
 *
 * DESCRIPTION:     This function is called when - 
 *                  State - ICF_SIC_02_SUBSCRIBED
 *                  Event - ICF_SIC_EVENT_SUCCESS_RESP 
 *                  
 ******************************************************************************/
icf_return_t icf_sic_02_success_resp(
        INOUT icf_sic_pdb_st      *p_sic_pdb)
{
    icf_return_t     ret_val = ICF_SUCCESS;
    icf_return_t     dummy_retval = ICF_SUCCESS;
    icf_nw_inc_subs_resp_st  *p_subs_resp = ICF_NULL;
    
    ICF_FUNCTION_ENTER(p_sic_pdb->p_glb_pdb)

    p_subs_resp = (icf_nw_inc_subs_resp_st *)(p_sic_pdb->p_msg);        
        
    /* If duration is received, then restart the subscription duration 
     * timer with new value
     */
    if ((ICF_NW_INC_SUBS_DURATION_PRESENT & p_subs_resp->bitmask) && 
            (p_sic_pdb->p_sic_ctx->duration != p_subs_resp->duration))
    {
        icf_sic_stop_curr_timer(p_sic_pdb);
        
		/* Fix for timer units - 
		 * timer_duration == is in milliseconds
		 * p_sic_ctx->duration == is in seconds */
        p_sic_pdb->timer_duration = (p_subs_resp->duration * 1000) / 2;
                
        ICF_SIC_SET_TIMER_TYPE(p_sic_pdb,ICF_TIMER_TYPE_SUBS_DURATION)    
        dummy_retval = icf_sic_start_timer(p_sic_pdb);
        
        if(ICF_FAILURE== dummy_retval)
        {
            ICF_ALARM(((icf_uint8_t *)"[SIC]: Timer Failure"));
        }
        
        /* Failure to start the timer is ignored here as if it fails then 
         * next try to start the timer will be done when notify will be 
         * received
         */
    }
    ICF_FUNCTION_EXIT(p_sic_pdb->p_glb_pdb)
        
    /* Always returns success */       
    return ret_val;
}/* End fucntion */


/*******************************************************************************
 *
 * FUNCTION:        icf_sic_02_notify_active
 *
 * DESCRIPTION:     This function is called when - 
 *                  State - ICF_SIC_02_SUBSCRIBED
 *                  Event - ICF_SIC_EVENT_NOTIFY_ACTIVE 
 *                  
 ******************************************************************************/
icf_return_t icf_sic_02_notify_active(
        INOUT icf_sic_pdb_st      *p_sic_pdb)
{
    icf_return_t     ret_val = ICF_SUCCESS;
    icf_return_t     dummy_retval = ICF_SUCCESS;
    icf_nw_inc_notify_ind_st *p_notify_ind = ICF_NULL;
    
    ICF_FUNCTION_ENTER(p_sic_pdb->p_glb_pdb)
        
    p_notify_ind = (icf_nw_inc_notify_ind_st *)(p_sic_pdb->p_msg);        

    if (ICF_TRUE == p_sic_pdb->await_notify_for_reg_event_pkg)
    {
        /* Stop the await notify timer that would have 
         * been started when this public uri was  
         * deregistered */
        icf_sic_stop_curr_timer(p_sic_pdb);

        p_sic_pdb->notify_resp = ICF_FAILURE;
        p_sic_pdb->resp_code = 500;
        ret_val = icf_sic_send_notify_resp(p_sic_pdb);

        icf_sic_send_delink_to_ssa(p_sic_pdb,(icf_boolean_t)ICF_FALSE);
        icf_sic_dealloc_context(p_sic_pdb);    
    }
    else
    {

	  /* Fix - no need to restart the timer on getting NOTIFY */
	  /* Un-commented line may be a bug if notify_ind does not contain duration */
	  /*p_sic_pdb->p_sic_ctx->duration = p_notify_ind->duration;*/

      /* 
         Send notify response only if application has not enabled support
         of asynchronous NOTIFY.
       */   
    if((ICF_NULL != p_sic_pdb->p_config_data) &&
       (ICF_FALSE == p_sic_pdb->p_config_data->async_notify_support_reqd))
    {
	    /* Send notify resp to SSA */
	    p_sic_pdb->notify_resp = ICF_SUCCESS;
  
        /* ret_val will be ignored here */
	    icf_sic_send_notify_resp(p_sic_pdb); 
    }    

	  /* Getting the 200 OK response to SUBSCRIBE we had passed the status as 
	   * ICF_SUBS_ACCEPTED. So now we have to pass it as ICF_SUBS_SUBSCRIBED
	   */
	  ICF_SIC_SET_CURR_STATUS(
                p_sic_pdb,
                ICF_SUBS_SUBSCRIBED)

      p_sic_pdb->send_htb_list = ICF_TRUE;

	  if(ICF_FAILURE != icf_sic_send_notify_msg_to_app(p_sic_pdb))
	  {
	     p_sic_pdb->send_htb_list = ICF_FALSE;

         /* Scenario: Notify for reg event package subscription is received
          * with State as Active but State of Contact in Message Body as
          * terminated. From the function icf_sic_send_notify_msg_to_app,
          * REGM module is triggered which deallocates the SIC context.
          * Now on success return of this function the execution should 
          * not proceed further as the SIC context block has been deallocated
          * therefore on check of call ID as INVALID in the SIC Context
          * control should return, no need to send any TRACE or Start any 
          * timer as the subscription has terminated at this point
          */ 
         if(ICF_INVALID_CALL_ID == p_sic_pdb->p_sic_ctx->call_id)
         {
              /* return from the function as the SIC context block has been
               * deallocated and no need to do further processing
               */
               ICF_FUNCTION_EXIT(p_sic_pdb->p_glb_pdb)
               return ICF_SUCCESS;
         }

	     /* ret_val is ignored here as this will end in FSM only */
        
         /* If notify received for reg event package for subscribe request 
          * which is triggered by IPTK on its own then there is no need to
          * send the subscription state to application */

         if (!(ICF_SIC_AUTO_SUBSCRIPTION_FOR_REG_EVENT & 
                   p_sic_pdb->p_sic_ctx->bitmask))
         {    
	 	     icf_sic_send_subs_state_to_app(p_sic_pdb);
         }    
	  }

      /* If duration is received in NOTIFY then the duration timer will be 
       * restarted of the duration received.
       */
    
      /* It is possible that the timer is not running already if there was a 
       * failure to start the timer earlier. In this case, start it now.
       */
    
	  /* Fix - no need to restart the timer on getting NOTIFY. Start it only if it 
	   * was not already running */
     
      /* Fix For SPR: 19999*/
      /* Fix for CSR 1-7502322: SPR 19844 */
      /* Start the refresh timer only if App has not set the flag
       * that indicates IPTK should not refresh subscriptions.
       */
      if (!(ICF_SIC_RESUBSCRIPTION_NOT_REQUIRED & p_sic_pdb->p_sic_ctx->bitmask))
      {        
          if (ICF_NW_INC_SUBS_DURATION_PRESENT & p_notify_ind->bitmask)
          {
              /* Fix - timer should be started for half the duration */
              /* Fix for timer units - 
               * timer_duration == is in milliseconds
               * p_notify_ind->duration == is in milliseconds
               * p_sic_ctx->duration == is in seconds */
              p_sic_pdb->timer_duration = p_notify_ind->duration / 2;
              p_sic_pdb->p_sic_ctx->duration = p_notify_ind->duration / 1000;

              /* If the timer is already running then stop it */
              icf_sic_stop_curr_timer(p_sic_pdb);

              ICF_SIC_SET_TIMER_TYPE(p_sic_pdb,ICF_TIMER_TYPE_SUBS_DURATION)    
                  dummy_retval = icf_sic_start_timer(p_sic_pdb);
              if(ICF_FAILURE == dummy_retval)
              {
                  ICF_ALARM(((icf_uint8_t *)"[SIC]: Timer Failure"));
              }
          }
          /* If duration is not received in the msg then the duration timer will
           * be started only if it is not running already. The duration of the 
           * timer will be taken from the SIC ctx.
           */
          else
          {
              if(ICF_TIMER_TYPE_SUBS_DURATION != 
                      p_sic_pdb->p_sic_ctx->timer_type)
              {
                  /* Fix - timer should be started for half the duration */
                  /* Fix for timer units - 
                   * timer_duration == is in milliseconds
                   * p_sic_ctx->duration == is in seconds */
                  p_sic_pdb->timer_duration = (p_sic_pdb->p_sic_ctx->duration * 1000) / 2;

                  /* Stop any currently running timer */
                  icf_sic_stop_curr_timer(p_sic_pdb);

                  ICF_SIC_SET_TIMER_TYPE(p_sic_pdb,
                          ICF_TIMER_TYPE_SUBS_DURATION)    

                  dummy_retval = icf_sic_start_timer(p_sic_pdb);

                  if(ICF_FAILURE == dummy_retval)
                  {
                      ICF_ALARM(((icf_uint8_t *)"[SIC]: Timer Failure"));
                  }
              }
          }
      } /* if (ICF_SIC_RESUBSCRIPTION_NOT_REQUIRED) */
      /* State will remain the same */

     /* If the SIC context has been deallocated, return from the function */
     ICF_CHECK_IF_VAR_IS_NULL(p_sic_pdb->p_glb_pdb,
                              p_sic_pdb->p_sic_ctx,
                              dummy_retval)
     if(ICF_FAILURE == dummy_retval)
     {
         /* no need to over write the ret_val value */
         ICF_FUNCTION_EXIT(p_sic_pdb->p_glb_pdb)
         return ret_val;
     }

     /* 
       If the bit ICF_SIC_NOTIFY_IND_SENT_TO_APP is set in SIC context, it means that application
       has enabled support of asynchronous NOTIFY and ICF_NOTIFY_IND has been given
       to the application. We will start a timer, save current FSM state and move to state 05.
     */
     if(ICF_SIC_NOTIFY_IND_SENT_TO_APP == 
             (ICF_SIC_NOTIFY_IND_SENT_TO_APP & p_sic_pdb->p_sic_ctx->bitmask))
     {
         icf_sic_move_to_state_await_notify_cfm(p_sic_pdb);
        
         /* reset the bit to indicate that ICF_NOTIFY_IND has been given to application */
         p_sic_pdb->p_sic_ctx->bitmask &= ~ICF_SIC_NOTIFY_IND_SENT_TO_APP;
     }
   }

   ICF_FUNCTION_EXIT(p_sic_pdb->p_glb_pdb)
   return ret_val;
}/* End function */

/*******************************************************************************
 *
 * FUNCTION:        icf_sic_02_notify_term_no_retry
 *
 * DESCRIPTION:     This function is called when - 
 *                  State - ICF_SIC_02_SUBSCRIBED
 *                  Event - ICF_SIC_EVENT_NOTIFY_TERM_NO_RETRY 
 *                  
 ******************************************************************************/
icf_return_t icf_sic_02_notify_term_no_retry(
        INOUT icf_sic_pdb_st      *p_sic_pdb)
{
    icf_return_t     ret_val = ICF_SUCCESS;
	icf_return_t     dummy_retval = ICF_SUCCESS;
    
    ICF_FUNCTION_ENTER(p_sic_pdb->p_glb_pdb)
    
    if (ICF_TRUE == p_sic_pdb->await_notify_for_reg_event_pkg)        
    {
       /* Stop the await notify timer that would have 
        * been started when this public uri was  
        * deregistered */
       icf_sic_stop_curr_timer(p_sic_pdb);

       p_sic_pdb->notify_resp = ICF_SUCCESS;
       p_sic_pdb->resp_code = 200;
       ret_val = icf_sic_send_notify_resp(p_sic_pdb);

       icf_sic_send_delink_to_ssa(p_sic_pdb,(icf_boolean_t)ICF_FALSE);
       icf_sic_dealloc_context(p_sic_pdb);    
    }
    else
    {   
      /* Indicate the corresponding module with the NOTIFY data.
       * Trigger SSA to send Notify Response
       */
      /* Send notify msg to the corresponding module */

      /* 
         Send notify response only if application has not enabled support
         of asynchronous NOTIFY.
      */   
      if((ICF_NULL != p_sic_pdb->p_config_data) &&
         (ICF_FALSE == p_sic_pdb->p_config_data->async_notify_support_reqd))
      {
    	  p_sic_pdb->notify_resp = ICF_SUCCESS;

    	  /* ret_val from SSA is ignored */
          icf_sic_send_notify_resp(p_sic_pdb);
      }

	  dummy_retval = icf_sic_send_notify_msg_to_app(p_sic_pdb);

      /* Stop Subscription Duration timer */
      icf_sic_stop_curr_timer(p_sic_pdb);
    
      ICF_SIC_SET_CURR_STATUS(
            p_sic_pdb,
            ICF_SUBS_FAIL_NO_RETRY)

      if(ICF_SUCCESS == dummy_retval)
	  {
	     icf_sic_send_subs_state_to_app(p_sic_pdb);
	  }
   
      /* 
         If asynchronous NOTIFY support is enabled, do not 
         deallocate SIC context here. Instead set a bit in 
         SIC context to indicate that SIC context needs to be 
         deallocated in the 05 state.
      */
      if((ICF_NULL != p_sic_pdb->p_config_data) &&
         (ICF_FALSE == p_sic_pdb->p_config_data->async_notify_support_reqd))
      {
          /* Trigger SSA to clear ctx and Go to state 6 */
          icf_sic_send_delink_to_ssa(p_sic_pdb,
                (icf_boolean_t)ICF_FALSE);
    
	      icf_sic_dealloc_context(p_sic_pdb);
      }
      else
      {
          p_sic_pdb->p_sic_ctx->bitmask |= ICF_SIC_CONTEXT_TO_BE_DEALLOCATED;
      }

      /* If the SIC context has been deallocated, return from the function */
      ICF_CHECK_IF_VAR_IS_NULL(p_sic_pdb->p_glb_pdb,
                               p_sic_pdb->p_sic_ctx,
                               dummy_retval)
      if(ICF_FAILURE == dummy_retval)
      {
          /* no need to over write the ret_val value */
          ICF_FUNCTION_EXIT(p_sic_pdb->p_glb_pdb)
          return ret_val;
      }

      /* 
        If the bit ICF_SIC_NOTIFY_IND_SENT_TO_APP is set in SIC context, it means that application
        has enabled support of asynchronous NOTIFY and ICF_NOTIFY_IND has been given
        to the application. We will start a timer, save current FSM state and move to state 05.
      */
      if(ICF_SIC_NOTIFY_IND_SENT_TO_APP == 
              (ICF_SIC_NOTIFY_IND_SENT_TO_APP & p_sic_pdb->p_sic_ctx->bitmask))
      {
          icf_sic_move_to_state_await_notify_cfm(p_sic_pdb);
        
          /* reset the bit to indicate that ICF_NOTIFY_IND has been given to application */
          p_sic_pdb->p_sic_ctx->bitmask &= ~ICF_SIC_NOTIFY_IND_SENT_TO_APP;
      }
    } 

    ICF_FUNCTION_EXIT(p_sic_pdb->p_glb_pdb)
    return ret_val;
}/* End function */

/*******************************************************************************
 *
 * FUNCTION:        icf_sic_02_notify_term_retry_imm
 *
 * DESCRIPTION:     This function is called when - 
 *                  State - ICF_SIC_02_SUBSCRIBED
 *                  Event - ICF_SIC_EVENT_NOTIFY_TERM_RETRY_IMM 
 *                  
 ******************************************************************************/
icf_return_t icf_sic_02_notify_term_retry_imm(
        INOUT icf_sic_pdb_st      *p_sic_pdb)
{
    icf_return_t     ret_val = ICF_SUCCESS;
    icf_return_t     dummy_retval = ICF_SUCCESS;
    
    ICF_FUNCTION_ENTER(p_sic_pdb->p_glb_pdb)
   
    if(ICF_TRUE == p_sic_pdb->await_notify_for_reg_event_pkg)    
    {
        /* Stop the await notify timer that would have 
         * been started when this public uri was  
         * deregistered */
        icf_sic_stop_curr_timer(p_sic_pdb);

        p_sic_pdb->notify_resp = ICF_FAILURE;
        p_sic_pdb->resp_code = 500;
        ret_val = icf_sic_send_notify_resp(p_sic_pdb);

        icf_sic_send_delink_to_ssa(p_sic_pdb,(icf_boolean_t)ICF_FALSE);
        icf_sic_dealloc_context(p_sic_pdb);    
    }
    else
    {
        /* Indicate the corresponding module with the NOTIFY data.
         * Trigger SSA to send Notify Response.    
         */
  
        /*
           Send notify response only if application has not enabled support
           of asynchronous NOTIFY.
        */   
        if((ICF_NULL != p_sic_pdb->p_config_data) &&
           (ICF_FALSE == p_sic_pdb->p_config_data->async_notify_support_reqd))
        {
       	    /* Send notify msg to the corresponding module */
   	        p_sic_pdb->notify_resp = ICF_SUCCESS;

            /* ret_val from SSA is ignored */
   	        icf_sic_send_notify_resp(p_sic_pdb);
        }

	    icf_sic_send_notify_msg_to_app(p_sic_pdb);
       
        /* 
           The following handling will be done in this handler function only
           if the application has not enabled support of asynchronous NOTIFY.
           In case the application has enabled support of asynchronous
           NOTIFY, the handling of this function will be done in new state 05
           after receving NOTIFY confirmation from the application and sending
           the NOTIFY response. We will save the current event in SIC context's
           prev_event and the current state in SIC context's prev_state.
           On the basis of these two variables, the common utility function 
           icf_sic_process_02_notify_term_retry_imm will be called in state 05.
        */
        if((ICF_NULL != p_sic_pdb->p_config_data) &&
           (ICF_FALSE == p_sic_pdb->p_config_data->async_notify_support_reqd))
        {
            ret_val = icf_sic_process_02_notify_term_retry_imm(p_sic_pdb);
        }

        /* If the SIC context has been deallocated, return from the function */
        ICF_CHECK_IF_VAR_IS_NULL(p_sic_pdb->p_glb_pdb,
                                 p_sic_pdb->p_sic_ctx,
                                 dummy_retval)
        if(ICF_FAILURE == dummy_retval)
        {
            /* no need to over write the ret_val value */
            ICF_FUNCTION_EXIT(p_sic_pdb->p_glb_pdb)
            return ret_val;
        }

        /* 
          If the bit ICF_SIC_NOTIFY_IND_SENT_TO_APP is set in SIC context, it means that application
          has enabled support of asynchronous NOTIFY and ICF_NOTIFY_IND has been given
          to the application. We will start a timer, save current FSM state and move to state 05.
        */
        if(ICF_SIC_NOTIFY_IND_SENT_TO_APP == 
                (ICF_SIC_NOTIFY_IND_SENT_TO_APP & p_sic_pdb->p_sic_ctx->bitmask))
        {
            /* Store current FSM event in SIC context's prev_event */
            ICF_SIC_SET_PREV_EVENT(p_sic_pdb,ICF_SIC_EVENT_NOTIFY_TERM_RETRY_IMM)

            icf_sic_move_to_state_await_notify_cfm(p_sic_pdb);
        
            /* reset the bit to indicate that ICF_NOTIFY_IND has been given to application */
            p_sic_pdb->p_sic_ctx->bitmask &= ~ICF_SIC_NOTIFY_IND_SENT_TO_APP;
        }
    }
    ICF_FUNCTION_EXIT(p_sic_pdb->p_glb_pdb)
    return ret_val;
}/* End function */ 


/*******************************************************************************
 *
 * FUNCTION:        icf_sic_02_notify_term_retry_after
 * 
 * DESCRIPTION:     This function is called when - 
 *                  State - ICF_SIC_02_SUBSCRIBED
 *                  Event - ICF_SIC_EVENT_NOTIFY_TERM_RETRY_AFTER 
 *                  
 ******************************************************************************/
icf_return_t icf_sic_02_notify_term_retry_after(
        INOUT icf_sic_pdb_st      *p_sic_pdb)
{
    icf_return_t             ret_val = ICF_SUCCESS;
    icf_return_t             dummy_retval = ICF_SUCCESS;
    icf_nw_inc_notify_ind_st *p_notify_ind = ICF_NULL;
        
    ICF_FUNCTION_ENTER(p_sic_pdb->p_glb_pdb)

    if(ICF_TRUE == p_sic_pdb->await_notify_for_reg_event_pkg)    
    {
        /* Stop the await notify timer that would have 
         * been started when this public uri was  
         * deregistered */
        icf_sic_stop_curr_timer(p_sic_pdb);

        p_sic_pdb->notify_resp = ICF_FAILURE;
        p_sic_pdb->resp_code = 500;
        ret_val = icf_sic_send_notify_resp(p_sic_pdb);

        icf_sic_send_delink_to_ssa(p_sic_pdb,(icf_boolean_t)ICF_FALSE);
        icf_sic_dealloc_context(p_sic_pdb);    
    }
    else
    {   
        p_notify_ind = (icf_nw_inc_notify_ind_st *)(p_sic_pdb->p_msg);

        ICF_CHECK_IF_VAR_IS_NULL(p_sic_pdb->p_glb_pdb,
                                 p_notify_ind,
                                 dummy_retval)
        if(ICF_FAILURE == dummy_retval)
        {
            ret_val = ICF_FAILURE;
            ICF_FUNCTION_EXIT(p_sic_pdb->p_glb_pdb)
            return ret_val;
        }

        /* 
           We will store the duration received in the NOTIFY in the SIC context.
           This will be used in 05 state to start a timer of the same duration.
           The timer duration will be stored in SIC PDB in 05 state.
        */   
        
        /* If the retry after duration is present in the notify then start 
         * the Subscription Retry timer of that duration else start the timer 
         * of the configured duration
         */
        if(ICF_NW_INC_SUBS_DURATION_PRESENT & p_notify_ind->bitmask)
        {
	        /* Fix for timer units -
             * timer_duration == is in milliseconds
             * p_notify_ind->duration == is in seconds
             * p_sic_ctx->duration == is in seconds */
            p_sic_pdb->p_sic_ctx->retry_after_duration = p_notify_ind->duration * 1000;
        }
        else
        {
            p_sic_pdb->p_sic_ctx->retry_after_duration = 
                p_sic_pdb->p_config_data->subs_retry_timer;
        }

        /* Indicate the corresponding module with the NOTIFY data.
         * Trigger SSA to send Notify Response.    
         */

        /* 
          Send notify response only if application has not enabled support
          of asynchronous NOTIFY.
        */   
        if((ICF_NULL != p_sic_pdb->p_config_data) &&
           (ICF_FALSE == p_sic_pdb->p_config_data->async_notify_support_reqd))
        {
	        /* Send notify msg to the corresponding module */
    	    p_sic_pdb->notify_resp = ICF_SUCCESS;

    	    icf_sic_send_notify_resp(p_sic_pdb);
        }

        icf_sic_send_notify_msg_to_app(p_sic_pdb);

        /* 
          The following handling will be done in this handler function only
          if the application has not enabled support of asynchronous NOTIFY.
          In case the application has enabled support of asynchronous
          NOTIFY, the handling of this function will be done in new state 05
          after receving NOTIFY confirmation from the application and sending
          the NOTIFY response. We will save the current event in SIC context's
          prev_event and the current state in SIC context's prev_state.
          On the basis of these two variables, the common utility function 
          icf_sic_process_02_notify_term_retry_after will be called in state 05.
        */
        if((ICF_NULL != p_sic_pdb->p_config_data) &&
           (ICF_FALSE == p_sic_pdb->p_config_data->async_notify_support_reqd))
        {
            ret_val = icf_sic_process_02_notify_term_retry_after(p_sic_pdb);
        }

        /* If the SIC context has been deallocated, return from the function */
        ICF_CHECK_IF_VAR_IS_NULL(p_sic_pdb->p_glb_pdb,
                                 p_sic_pdb->p_sic_ctx,
                                 dummy_retval)
        if(ICF_FAILURE == dummy_retval)
        {
            /* no need to over write the ret_val value */
            ICF_FUNCTION_EXIT(p_sic_pdb->p_glb_pdb)
            return ret_val;
        }

        /* 
          If the bit ICF_SIC_NOTIFY_IND_SENT_TO_APP is set in SIC context,
          it means that application has enabled support of asynchronous NOTIFY
          and ICF_NOTIFY_IND has been given to the application. We will start
          a timer, save current FSM state and move to state 05.
        */
        if(ICF_SIC_NOTIFY_IND_SENT_TO_APP == 
                (ICF_SIC_NOTIFY_IND_SENT_TO_APP & p_sic_pdb->p_sic_ctx->bitmask))
        {
            /* Store current FSM event in SIC context's prev_event */
            ICF_SIC_SET_PREV_EVENT(p_sic_pdb,ICF_SIC_EVENT_NOTIFY_TERM_RETRY_AFTER)

            icf_sic_move_to_state_await_notify_cfm(p_sic_pdb);
        
            /* reset the bit to indicate that ICF_NOTIFY_IND has been given to application */
            p_sic_pdb->p_sic_ctx->bitmask &= ~ICF_SIC_NOTIFY_IND_SENT_TO_APP;
        }
    }
    ICF_FUNCTION_EXIT(p_sic_pdb->p_glb_pdb)
    return ret_val;
}/* End function */

/*******************************************************************************
 *
 * FUNCTION:        icf_sic_02_duration_timer_exp
 * 
 * DESCRIPTION:     This function is called when - 
 *                  State - ICF_SIC_02_SUBSCRIBED
 *                  Event - ICF_SIC_EVENT_DURATION_TIMER_EXP 
 *                  
 ******************************************************************************/
icf_return_t icf_sic_02_duration_timer_exp(
        INOUT icf_sic_pdb_st      *p_sic_pdb)
{
    icf_return_t     ret_val = ICF_SUCCESS;
    icf_return_t     dummy_retval = ICF_SUCCESS;

    /* Start Subscription Response timer */
    ICF_SIC_SET_TIMER_TYPE(p_sic_pdb,ICF_TIMER_TYPE_SUBS_RESPONSE)    
    dummy_retval = icf_sic_start_timer(p_sic_pdb);
 
    /* If the response timer is started then trigger SSA to send 
     * Subscription Request again
     */
    if (ICF_SUCCESS == dummy_retval)
    {
/* CSR 1-6240518: Send resubs trace to application */
#ifdef ICF_TRACE_ENABLE
        ICF_SIC_APP_TRACE(p_sic_pdb, ICF_TRACE_RESUBS)
        p_sic_pdb->trace_alrdy_sent = ICF_TRUE;
#endif        
        icf_sic_send_subs_req(p_sic_pdb);
#ifdef ICF_LOAD_STAT
        p_sic_pdb->p_glb_pdb->load_data.subscribe_refresh++;
#endif
    }
    /* If SIC fails to start the timer then indicate the failure and 
     * go to state 5
     */
    else
    {
        ICF_ALARM(((icf_uint8_t *)"[SIC]: Timer Failure"));

        ICF_SIC_SET_CURR_STATUS(
                p_sic_pdb,
                ICF_SUBS_FAIL_NO_RETRY)

        icf_sic_send_subs_state_to_app(p_sic_pdb);

		icf_sic_send_delink_to_ssa(p_sic_pdb,
					(icf_boolean_t)ICF_FALSE);            
        
		icf_sic_dealloc_context(p_sic_pdb);    

    }
    
    ICF_FUNCTION_EXIT(p_sic_pdb->p_glb_pdb)
    return ret_val;        
}/* End function */

/******************************************************************************
 * FUNCTION:        icf_sic_02_await_notify_timer_exp 
 * DESCRIPTION:     This function is invoked when await notify timer expires
 *		    in state 02, it will clear the sic ctxt.
 *                  State - ICF_SIC_02_SUBSCRIBED
 *                  Event - ICF_SIC_EVENT_AWAIT_SUBS_NOTIFY_TIMER_EXP  
 *                         
 ******************************************************************************/
icf_return_t icf_sic_02_await_notify_timer_exp(
                         INOUT icf_sic_pdb_st  *p_sic_pdb)
{
    icf_return_t     ret_val = ICF_SUCCESS;
    
    ICF_FUNCTION_ENTER(p_sic_pdb->p_glb_pdb)    
        
    icf_sic_send_delink_to_ssa(p_sic_pdb,
                              (icf_boolean_t)ICF_FALSE);
    icf_sic_dealloc_context(p_sic_pdb);    
    
    ICF_FUNCTION_EXIT(p_sic_pdb->p_glb_pdb)
    return ret_val; 
}  

/*******************************************************************************
 *
 * FUNCTION:        icf_sic_03_subs_req
 * 
 * DESCRIPTION:     This function is called when - 
 *                  State - ICF_SIC_03_UNSUBSCRIBING
 *                  Event - ICF_SIC_EVENT_SUBS_REQ 
 *                  
 ******************************************************************************/
icf_return_t icf_sic_03_subs_req(
        INOUT icf_sic_pdb_st      *p_sic_pdb)
{
    icf_return_t     ret_val = ICF_SUCCESS;
    
    ICF_FUNCTION_ENTER(p_sic_pdb->p_glb_pdb)
 
    /* Set the buffer.Remain in the same state 3.    
     */
    ICF_SIC_SET_BUFFER(p_sic_pdb)
    
    ICF_FUNCTION_EXIT(p_sic_pdb->p_glb_pdb)
    /* Always returns success */
    return ret_val;
}


/*******************************************************************************
 *
 * FUNCTION:        icf_sic_03_unsub_req
 * 
 * DESCRIPTION:     This function is called when - 
 *                  State - ICF_SIC_03_UNSUBSCRIBING
 *                  Event - ICF_SIC_EVENT_UNSUB_REQ 
 *                  
 ******************************************************************************/
icf_return_t icf_sic_03_unsub_req(
        INOUT icf_sic_pdb_st      *p_sic_pdb)
{
    icf_return_t     ret_val = ICF_SUCCESS;
    
    ICF_FUNCTION_ENTER(p_sic_pdb->p_glb_pdb)
 
    /* Set the buffer to NULL
     */
    ICF_SIC_RESET_BUFFER(p_sic_pdb)

    ICF_FUNCTION_EXIT(p_sic_pdb->p_glb_pdb)
    /* Always returns success */
    return ret_val;
}

/*******************************************************************************
 *
 * FUNCTION:        icf_sic_03_success_resp
 * 
 * DESCRIPTION:     This function is called when - 
 *                  State - ICF_SIC_03_UNSUBSCRIBING
 *                  Event - ICF_SIC_EVENT_SUCCESS_RESP 
 *                  
 ******************************************************************************/
icf_return_t icf_sic_03_success_resp(
        INOUT icf_sic_pdb_st      *p_sic_pdb)
{
    icf_return_t     ret_val = ICF_SUCCESS;
    icf_return_t     dummy_retval = ICF_SUCCESS;
    
    ICF_FUNCTION_ENTER(p_sic_pdb->p_glb_pdb)

    /* Stop Subscription Response timer */
    icf_sic_stop_curr_timer(p_sic_pdb);
    
    /* If the buffer is NULL then */
    if (ICF_FALSE == p_sic_pdb->p_sic_ctx->event_buffer)
    {
        ICF_SIC_SET_CURR_STATUS(
                    p_sic_pdb,
                    ICF_SUBS_UNSUBSCRIBED)

        icf_sic_send_subs_state_to_app(p_sic_pdb);

		ICF_SIC_RESET_BUFFER(p_sic_pdb);

		icf_cmn_free_header_list(p_sic_pdb->p_glb_pdb,
			&(p_sic_pdb->p_sic_ctx->header_list),
			p_sic_pdb->p_ecode);

		/* Wait fo for Terminating Notify request */
		ICF_SIC_SET_TIMER_TYPE(p_sic_pdb,ICF_TIMER_TYPE_AWAIT_SUBS_NOTIFY_REQ)

		dummy_retval = icf_sic_start_timer(p_sic_pdb);

        if(ICF_FAILURE == dummy_retval)
        {
            ICF_ALARM(((icf_uint8_t *)"[SIC]: Timer Failure"));
        }    
    }
    /* If the buffer is not NULL then */
    else
    {
        ICF_SIC_RESET_BUFFER(p_sic_pdb)
            
        /* Start Subscription Response timer */
        ICF_SIC_SET_TIMER_TYPE(p_sic_pdb,ICF_TIMER_TYPE_SUBS_RESPONSE)    

        dummy_retval = icf_sic_start_timer(p_sic_pdb);
        
        /* If the response timer is started then trigger SSA to send the 
         * Subscription Request
         */
        if (ICF_SUCCESS == dummy_retval)
        {
            icf_sic_send_subs_req(p_sic_pdb);
        }
        /* If SIC fails to start the timer then indicate the failure, 
         * go to state 5(blocked)
         */
        else
        {
            ICF_ALARM(((icf_uint8_t *)"[SIC]: Timer Failure"));

            ICF_SIC_SET_CURR_STATUS(
                    p_sic_pdb,
                    ICF_SUBS_FAIL_NO_RETRY)

            icf_sic_send_subs_state_to_app(p_sic_pdb);

			icf_sic_send_delink_to_ssa(p_sic_pdb,
						(icf_boolean_t)ICF_FALSE);            
            
			icf_sic_dealloc_context(p_sic_pdb);    

        }
    }/* End else */
    
    ICF_FUNCTION_EXIT(p_sic_pdb->p_glb_pdb)
    return ret_val;
}/* End function */

/*******************************************************************************
 *
 * FUNCTION:        icf_sic_03_failure_resp_no_retry 
 * 
 * DESCRIPTION:     This function is called when - 
 *                  State - ICF_SIC_03_UNSUBSCRIBING
 *                  Event - ICF_SIC_EVENT_FAILURE_RESP_NO_RETRY 
 *                  
 ******************************************************************************/
icf_return_t icf_sic_03_failure_resp_no_retry(
        INOUT icf_sic_pdb_st      *p_sic_pdb)
{
    icf_return_t     ret_val = ICF_SUCCESS;
    
    /* Stop Subscription Response timer */
    icf_sic_stop_curr_timer(p_sic_pdb);
    
    /* If the buffer is NULL then */
    if (ICF_FALSE == p_sic_pdb->p_sic_ctx->event_buffer)
    {
        /* Indicate Unsubscribe Failure Response */
        ICF_SIC_SET_CURR_STATUS(
                    p_sic_pdb,
                    ICF_SUBS_UNSUBSCRIBED)

        icf_sic_send_subs_state_to_app(p_sic_pdb);
 
        /* Trigger SSA to clear its context
         * Clear SIC context 
         */
        icf_sic_send_delink_to_ssa(p_sic_pdb,
                (icf_boolean_t)ICF_FALSE);

        icf_sic_dealloc_context(p_sic_pdb);    
    }
    /* If the buffer is not NULL then */
    else
    {
        ICF_SIC_RESET_BUFFER(p_sic_pdb)
            
        ICF_SIC_SET_CURR_STATUS(
                    p_sic_pdb,
                    ICF_SUBS_FAIL_NO_RETRY)

        icf_sic_send_subs_state_to_app(p_sic_pdb);

        icf_sic_send_delink_to_ssa(p_sic_pdb,
                (icf_boolean_t)ICF_FALSE);

		icf_sic_dealloc_context(p_sic_pdb);
    }           
    ICF_FUNCTION_EXIT(p_sic_pdb->p_glb_pdb)
    return ret_val;
}/* End function */


/*******************************************************************************
 *
 * FUNCTION:        icf_sic_03_failure_resp_with_retry 
 * 
 * DESCRIPTION:     This function is called when - 
 *                  State - ICF_SIC_03_UNSUBSCRIBING
 *                  Event - ICF_SIC_EVENT_FAILURE_RESP_WITH_RETRY 
 *                  
 ******************************************************************************/
icf_return_t icf_sic_03_failure_resp_with_retry(
        INOUT icf_sic_pdb_st      *p_sic_pdb)
{
    icf_return_t     ret_val = ICF_SUCCESS;
    icf_return_t     dummy_retval = ICF_SUCCESS;
    
    /* Stop Subscription Response timer */
    icf_sic_stop_curr_timer(p_sic_pdb);
    
    /* If the buffer is NULL then */
    if(ICF_FALSE == p_sic_pdb->p_sic_ctx->event_buffer)
    {
        /* indicate the failure, trigger SSA to clear the context, 
         * clear SIC context
         */
        ICF_SIC_SET_CURR_STATUS(
                    p_sic_pdb,
                    ICF_SUBS_UNSUBSCRIBED)

        icf_sic_send_subs_state_to_app(p_sic_pdb);

        icf_sic_send_delink_to_ssa(p_sic_pdb,
                (icf_boolean_t)ICF_FALSE);

        icf_sic_dealloc_context(p_sic_pdb);    
    }
    /* If the buffer is not NULL then */
    else
    {
        ICF_SIC_RESET_BUFFER(p_sic_pdb)
        
        /* Start Subscription Response timer */
        ICF_SIC_SET_TIMER_TYPE(p_sic_pdb,ICF_TIMER_TYPE_SUBS_RESPONSE)

        dummy_retval = icf_sic_start_timer(p_sic_pdb);

        /* If the timer is started then trigger SSA to send subscribe request
         */
        if (ICF_SUCCESS == dummy_retval)
        {
            icf_sic_send_subs_req(p_sic_pdb);
        }
        /* If SIC fails to start the timer then indicate the failure and 
         * go to blocked state 5    
         */
        else
        {
            ICF_ALARM(((icf_uint8_t *)"[SIC]: Timer Failure"));

            ICF_SIC_SET_CURR_STATUS(
                    p_sic_pdb,
                    ICF_SUBS_FAIL_NO_RETRY)

            icf_sic_send_subs_state_to_app(p_sic_pdb);

			icf_sic_send_delink_to_ssa(p_sic_pdb,
						(icf_boolean_t)ICF_FALSE);            
            
			icf_sic_dealloc_context(p_sic_pdb);    
        }
    }/* End else */
    
    ICF_FUNCTION_EXIT(p_sic_pdb->p_glb_pdb)
    return ret_val;
}/* End function */

/* SPR 18931: New function added for rejecting the NOTIFY when un-subscription
 * is ongoing.
 */  
/*******************************************************************************
 *
 * FUNCTION:        icf_sic_03_notify_active
 * 
 * DESCRIPTION:     This function is called when - 
 *                  State - ICF_SIC_03_UNSUBSCRIBING 
 *                  Event - ICF_SIC_EVENT_NOTIFY_ACTIVE
 *                  
 ******************************************************************************/
icf_return_t icf_sic_03_notify_active(
        INOUT icf_sic_pdb_st      *p_sic_pdb)
{
    icf_return_t     ret_val = ICF_SUCCESS;
    
    ICF_FUNCTION_ENTER(p_sic_pdb->p_glb_pdb)
    
    /* While unsubscription is ongoing, if NOTIFY with
     * state active is received then it should be rejected 
     * as no action can be taken on the message body of NOTIFY
     */ 

	p_sic_pdb->notify_resp = ICF_FAILURE;
    /*
      For this NOTIFY, no ICF_NOTIFY_IND will be given to
      application and it will be responded with a 500.
    */   
    p_sic_pdb->resp_code = 500;

    ret_val = icf_sic_send_notify_resp(p_sic_pdb);
	
    ICF_FUNCTION_EXIT(p_sic_pdb->p_glb_pdb)
    /* Always returns success */
    return ret_val;
}

/*******************************************************************************
 *
 * FUNCTION:        icf_sic_03_notify_term_no_retry 
 * 
 * DESCRIPTION:     This function is called when - 
 *                  State - ICF_SIC_03_UNSUBSCRIBING
 *                  Event - ICF_SIC_EVENT_NOTIFY_TERM_NO_RETRY 
 *                  
 ******************************************************************************/
icf_return_t icf_sic_03_notify_term_no_retry(
        INOUT icf_sic_pdb_st      *p_sic_pdb)
{
    icf_return_t     ret_val = ICF_SUCCESS,
                     dummy_retval = ICF_SUCCESS;
    
    ICF_FUNCTION_ENTER(p_sic_pdb->p_glb_pdb)
    
    /* Indicate the corresponding module with the NOTIFY data.
     * Trigger SSA to send notify response. 
     */
    
    /* 
       Send notify response only if application has not enabled support
       of asynchronous NOTIFY.
    */   
    if((ICF_NULL != p_sic_pdb->p_config_data) &&
       (ICF_FALSE == p_sic_pdb->p_config_data->async_notify_support_reqd))
    {
    	/* Send notify msg to the corresponding module */
	    p_sic_pdb->notify_resp = ICF_SUCCESS;

    	/* ret_val is ignored here */
        icf_sic_send_notify_resp(p_sic_pdb);
    }

	icf_sic_send_notify_msg_to_app(p_sic_pdb);

    /* Stop Subscription Response timer */
    icf_sic_stop_curr_timer(p_sic_pdb);
   
    /* If the buffer is NULL then */
    if (ICF_FALSE == p_sic_pdb->p_sic_ctx->event_buffer)
    {
        /* Indicate the success response of Unsubscribe */
        ICF_SIC_SET_CURR_STATUS(
                    p_sic_pdb,
                    ICF_SUBS_UNSUBSCRIBED)

        icf_sic_send_subs_state_to_app(p_sic_pdb);

        /* 
           If asynchronous NOTIFY support is enabled, do not 
           deallocate SIC context here. Instead set a bit in 
           SIC context to indicate that SIC context needs to be 
           deallocated in the 05 state.
        */
        if((ICF_NULL != p_sic_pdb->p_config_data) &&
           (ICF_FALSE == p_sic_pdb->p_config_data->async_notify_support_reqd))
        {
            /* Trigger SSA to clear its context.
             * Clear SIC context and go to state 0.
             */
            icf_sic_send_delink_to_ssa(p_sic_pdb,
                    (icf_boolean_t)ICF_FALSE);

            icf_sic_dealloc_context(p_sic_pdb);    
        }
        else
        {
            p_sic_pdb->p_sic_ctx->bitmask |= ICF_SIC_CONTEXT_TO_BE_DEALLOCATED;
        }
    }
    /* If the buffer is not NULL then */
    else
    {
        /* Send Subscription failure.
         * Trigger SSA to clear ctx and Go to state 6.
         */

        ICF_SIC_SET_CURR_STATUS(
                    p_sic_pdb,
                    ICF_SUBS_FAIL_NO_RETRY)
        
        icf_sic_send_subs_state_to_app(p_sic_pdb);

        /* 
           If asynchronous NOTIFY support is enabled, do not 
           deallocate SIC context here. Instead set a bit in 
           SIC context to indicate that SIC context needs to be 
           deallocated in the 05 state.
        */
        if((ICF_NULL != p_sic_pdb->p_config_data) &&
           (ICF_FALSE == p_sic_pdb->p_config_data->async_notify_support_reqd))
        {
            icf_sic_send_delink_to_ssa(p_sic_pdb,
                    (icf_boolean_t)ICF_FALSE);

            icf_sic_dealloc_context(p_sic_pdb);
        }
        else
        {
            p_sic_pdb->p_sic_ctx->bitmask |= ICF_SIC_CONTEXT_TO_BE_DEALLOCATED;
        }
    }

    /* If the SIC context has been deallocated, return from the function */
    ICF_CHECK_IF_VAR_IS_NULL(p_sic_pdb->p_glb_pdb,
                             p_sic_pdb->p_sic_ctx,
                             dummy_retval)
    if(ICF_FAILURE == dummy_retval)
    {
        /* no need to over write the ret_val value */
        ICF_FUNCTION_EXIT(p_sic_pdb->p_glb_pdb)
        return ret_val;
    }

    /* 
       If the bit ICF_SIC_NOTIFY_IND_SENT_TO_APP is set in SIC context, it means that application
       has enabled support of asynchronous NOTIFY and ICF_NOTIFY_IND has been given
       to the application. We will start a timer, save current FSM state and move to state 05.
    */
    if(ICF_SIC_NOTIFY_IND_SENT_TO_APP == 
            (ICF_SIC_NOTIFY_IND_SENT_TO_APP & p_sic_pdb->p_sic_ctx->bitmask))
    {
        icf_sic_move_to_state_await_notify_cfm(p_sic_pdb);
        
        /* reset the bit to indicate that ICF_NOTIFY_IND has been given to application */
        p_sic_pdb->p_sic_ctx->bitmask &= ~ICF_SIC_NOTIFY_IND_SENT_TO_APP;
    }

    ICF_FUNCTION_EXIT(p_sic_pdb->p_glb_pdb)
    return ret_val;
}/* End fucntion */


/*******************************************************************************
 *
 * FUNCTION:        icf_sic_03_notify_term_retry_imm
 * 
 * DESCRIPTION:     This function is called when - 
 *                  State - ICF_SIC_03_UNSUBSCRIBING
 *                  Event - ICF_SIC_EVENT_NOTIFY_TERM_RETRY_IMM
 *                  
 ******************************************************************************/
icf_return_t icf_sic_03_notify_term_retry_imm(
        INOUT icf_sic_pdb_st      *p_sic_pdb)
{
    icf_return_t     ret_val = ICF_SUCCESS;
    icf_return_t     dummy_retval = ICF_SUCCESS;
  
    /* 
       Send notify response only if application has not enabled support
       of asynchronous NOTIFY.
    */   
    if((ICF_NULL != p_sic_pdb->p_config_data) &&
       (ICF_FALSE == p_sic_pdb->p_config_data->async_notify_support_reqd))
    {
	    /* Send notify msg to the corresponding module */
    	p_sic_pdb->notify_resp = ICF_SUCCESS;

        icf_sic_send_notify_resp(p_sic_pdb);
    }

	icf_sic_send_notify_msg_to_app(p_sic_pdb);
   
    /* 
      The following handling will be done in this handler function only
      if the application has not enabled support of asynchronous NOTIFY.
      In case the application has enabled support of asynchronous
      NOTIFY, the handling of this function will be done in new state 05
      after receving NOTIFY confirmation from the application and sending
      the NOTIFY response. We will save the current event in SIC context's
      prev_event and the current state in SIC context's prev_state.
      On the basis of these two variables, the common utility function 
      icf_sic_process_03_notify_term_retry_imm will be called in state 05.
    */
    if((ICF_NULL != p_sic_pdb->p_config_data) &&
       (ICF_FALSE == p_sic_pdb->p_config_data->async_notify_support_reqd))
    {
        ret_val = icf_sic_process_03_notify_term_retry_imm(p_sic_pdb);
    }
   
    /* If the SIC context has been deallocated, return from the function */
    ICF_CHECK_IF_VAR_IS_NULL(p_sic_pdb->p_glb_pdb,
                             p_sic_pdb->p_sic_ctx,
                             dummy_retval)
    if(ICF_FAILURE == dummy_retval)
    {
        /* no need to over write the ret_val value */
        ICF_FUNCTION_EXIT(p_sic_pdb->p_glb_pdb)
        return ret_val;
    }

    /* 
       If the bit ICF_SIC_NOTIFY_IND_SENT_TO_APP is set in SIC context, it means that application
       has enabled support of asynchronous NOTIFY and ICF_NOTIFY_IND has been given
       to the application. We will start a timer, save current FSM state and move to state 05.
    */
    if(ICF_SIC_NOTIFY_IND_SENT_TO_APP == 
            (ICF_SIC_NOTIFY_IND_SENT_TO_APP & p_sic_pdb->p_sic_ctx->bitmask))
    {
        /* Store current FSM event in SIC context's prev_event */
        ICF_SIC_SET_PREV_EVENT(p_sic_pdb,ICF_SIC_EVENT_NOTIFY_TERM_RETRY_IMM)

        icf_sic_move_to_state_await_notify_cfm(p_sic_pdb);
        
        /* reset the bit to indicate that ICF_NOTIFY_IND has been given to application */
        p_sic_pdb->p_sic_ctx->bitmask &= ~ICF_SIC_NOTIFY_IND_SENT_TO_APP;
    }
    
    ICF_FUNCTION_EXIT(p_sic_pdb->p_glb_pdb)
    return ret_val;
}/* End function */


/*******************************************************************************
 *
 * FUNCTION:        icf_sic_03_notify_term_retry_after
 * 
 * DESCRIPTION:     This function is called when - 
 *                  State - ICF_SIC_03_UNSUBSCRIBING
 *                  Event - ICF_SIC_EVENT_NOTIFY_TERM_RETRY_AFTER
 *                  
 ******************************************************************************/
icf_return_t icf_sic_03_notify_term_retry_after(
        INOUT icf_sic_pdb_st      *p_sic_pdb)
{
    icf_return_t             ret_val = ICF_SUCCESS;
    icf_return_t             dummy_retval = ICF_SUCCESS;
    icf_nw_inc_notify_ind_st *p_notify_ind = ICF_NULL;
        
    ICF_FUNCTION_ENTER(p_sic_pdb->p_glb_pdb)

    p_notify_ind = (icf_nw_inc_notify_ind_st *)(p_sic_pdb->p_msg);

    ICF_CHECK_IF_VAR_IS_NULL(p_sic_pdb->p_glb_pdb,
                             p_notify_ind,
                             dummy_retval)
    if(ICF_FAILURE == dummy_retval)
    {
        ret_val = ICF_FAILURE;
        ICF_FUNCTION_EXIT(p_sic_pdb->p_glb_pdb)
        return ret_val;
    }

    /* 
       If the event_buffer is set, we will store the duration received
       in the NOTIFY in the SIC context. This will be used in 05 state
       to start a timer of the same duration.The timer duration will be 
       stored in SIC PDB in 05 state.
    */   
    if(ICF_TRUE == p_sic_pdb->p_sic_ctx->event_buffer)
    {
        /* If the retry after duration is present in the notify then start 
         * the Subscription Retry timer of that duration else start the timer 
         * of the configured duration
         */
        if(ICF_NW_INC_SUBS_DURATION_PRESENT & p_notify_ind->bitmask)
        {
	        /* Fix for timer units -
             * timer_duration == is in milliseconds
             * p_notify_ind->duration == is in seconds
             * p_sic_ctx->duration == is in seconds */
            p_sic_pdb->p_sic_ctx->retry_after_duration = p_notify_ind->duration * 1000;
        }
        else
        {
            p_sic_pdb->p_sic_ctx->retry_after_duration = 
                p_sic_pdb->p_config_data->subs_retry_timer;
        }
    }

    /* 
       Send notify response only if application has not enabled support
       of asynchronous NOTIFY.
    */   
    if((ICF_NULL != p_sic_pdb->p_config_data) &&
       (ICF_FALSE == p_sic_pdb->p_config_data->async_notify_support_reqd))
    {
    	/* Send notify msg to the corresponding module */
	    p_sic_pdb->notify_resp = ICF_SUCCESS;

        icf_sic_send_notify_resp(p_sic_pdb);
    }

	icf_sic_send_notify_msg_to_app(p_sic_pdb);
    
    /* 
      The following handling will be done in this handler function only
      if the application has not enabled support of asynchronous NOTIFY.
      In case the application has enabled support of asynchronous
      NOTIFY, the handling of this function will be done in new state 05
      after receving NOTIFY confirmation from the application and sending
      the NOTIFY response. We will save the current event in SIC context's
      prev_event and the current state in SIC context's prev_state.
      On the basis of these two variables, the common utility function 
      icf_sic_process_03_notify_term_retry_after will be called in state 05.
    */
    if((ICF_NULL != p_sic_pdb->p_config_data) &&
       (ICF_FALSE == p_sic_pdb->p_config_data->async_notify_support_reqd))
    {
        ret_val = icf_sic_process_03_notify_term_retry_after(p_sic_pdb);
    }
   
    /* If the SIC context has been deallocated, return from the function */
    ICF_CHECK_IF_VAR_IS_NULL(p_sic_pdb->p_glb_pdb,
                             p_sic_pdb->p_sic_ctx,
                             dummy_retval)
    if(ICF_FAILURE == dummy_retval)
    {
        /* no need to over write the ret_val value */
        ICF_FUNCTION_EXIT(p_sic_pdb->p_glb_pdb)
        return ret_val;
    }

    /* 
       If the bit ICF_SIC_NOTIFY_IND_SENT_TO_APP is set in SIC context, it means that application
       has enabled support of asynchronous NOTIFY and ICF_NOTIFY_IND has been given
       to the application. We will start a timer, save current FSM state and move to state 05.
       Also since this FSM function changes the current state to ICF_SIC_04_SUBSCRIBE_RETRY,
       we will save this in SIC context's prev_state and would use the same to restore
       current state in state 05 handler functions.
    */
    if(ICF_SIC_NOTIFY_IND_SENT_TO_APP == 
            (ICF_SIC_NOTIFY_IND_SENT_TO_APP & p_sic_pdb->p_sic_ctx->bitmask))
    {
        /* Store current FSM event in SIC context's prev_event */
        ICF_SIC_SET_PREV_EVENT(p_sic_pdb,ICF_SIC_EVENT_NOTIFY_TERM_RETRY_AFTER)

        icf_sic_move_to_state_await_notify_cfm(p_sic_pdb);
        
        /* reset the bit to indicate that ICF_NOTIFY_IND has been given to application */
        p_sic_pdb->p_sic_ctx->bitmask &= ~ICF_SIC_NOTIFY_IND_SENT_TO_APP;
    }

    ICF_FUNCTION_EXIT(p_sic_pdb->p_glb_pdb)
    return ret_val;
}/* End function */


/*******************************************************************************
 *
 * FUNCTION:        icf_sic_03_resp_timer_exp
 * 
 * DESCRIPTION:     This function is called when - 
 *                  State - ICF_SIC_03_UNSUBSCRIBING
 *                  Event - ICF_SIC_EVENT_RESP_TIMER_EXP
 *                  
 ******************************************************************************/
icf_return_t icf_sic_03_resp_timer_exp(
        INOUT icf_sic_pdb_st      *p_sic_pdb)
{
    icf_return_t     ret_val = ICF_SUCCESS;
    icf_return_t     dummy_retval = ICF_SUCCESS;

    /* If the buffer is NULL then */
    if (ICF_FALSE == p_sic_pdb->p_sic_ctx->event_buffer)
    {
        /* indicate the failure, send unsubscribe request, 
         * trigger SSA to clear the ctx, clear SIC ctx 
         */
        
        ICF_SIC_SET_CURR_STATUS(
                    p_sic_pdb,
                    ICF_SUBS_UNSUBSCRIBED)

        icf_sic_send_subs_state_to_app(p_sic_pdb);

        /* This unsubscribe is send only for best effort, failure from 
         * SSA will be ignored 
         */ 
		p_sic_pdb->best_effort_unsub = ICF_TRUE;

		/* best effort so ret_val is ignored */
        icf_sic_send_unsub_req(p_sic_pdb);
        
        icf_sic_send_delink_to_ssa(p_sic_pdb,
                (icf_boolean_t)ICF_FALSE);
        /* request DBM to clear SIC ctx */
         icf_sic_dealloc_context(p_sic_pdb);
    }
    /* If the buffer is not NULL then */
    else
    {
        ICF_SIC_RESET_BUFFER(p_sic_pdb)
            
        /* Start Subscription Response timer */            
        ICF_SIC_SET_TIMER_TYPE(p_sic_pdb,ICF_TIMER_TYPE_SUBS_RESPONSE)    

        dummy_retval = icf_sic_start_timer(p_sic_pdb);

        /* If the response timer is started then trigger SSA to send 
         * Subscribe Request
         */
        if (ICF_SUCCESS == dummy_retval)
        {
			icf_sic_send_subs_req(p_sic_pdb);
        }
        else
        {
            ICF_ALARM(((icf_uint8_t *)"[SIC]: Timer Failure"));

            ICF_SIC_SET_CURR_STATUS(
                    p_sic_pdb,
                    ICF_SUBS_FAIL_NO_RETRY)

            icf_sic_send_subs_state_to_app(p_sic_pdb);

			icf_sic_send_delink_to_ssa(p_sic_pdb,
						(icf_boolean_t)ICF_FALSE);            
            
			icf_sic_dealloc_context(p_sic_pdb);    
        }
    }/* End else */

    ICF_FUNCTION_EXIT(p_sic_pdb->p_glb_pdb)
    return ret_val;
}/* End function */

/*******************************************************************************
 *
 * FUNCTION:        icf_sic_01_notify_term_retry_imm
 *
 * DESCRIPTION:     This function is called when 
 *					NOTIFY was received Before 200 Ok
 *					of an UNSUBSCRIBE Request due to some
 *					Network Delays
 *                  State - ICF_SIC_03_UNSUBSCRIBING
 *                  Event - ICF_SIC_EVENT_NOTIFY_TERM_RETRY_IMM 
 *                  
 ******************************************************************************/
icf_return_t icf_sic_01_notify_term_retry_imm(
        INOUT icf_sic_pdb_st      *p_sic_pdb)
{
    icf_return_t     ret_val = ICF_SUCCESS,
                     dummy_retval = ICF_SUCCESS;
    icf_nw_inc_notify_ind_st *p_notify_ind = ICF_NULL;
    
    ICF_FUNCTION_ENTER(p_sic_pdb->p_glb_pdb)
      
	/* We will receive this Notify for Poll Subscribe Request
	 * for Normal Subcribe Request This is an unexpected Event
	 */
	 icf_sic_stop_curr_timer(p_sic_pdb);

	if(p_sic_pdb->p_sic_ctx->bitmask & ICF_SIC_POLL_SUBSCRIBE_REQ)
	{
		p_notify_ind = (icf_nw_inc_notify_ind_st *)(p_sic_pdb->p_msg);        

        /* 
           Send notify response only if application has not enabled support
           of asynchronous NOTIFY.
        */   
        if((ICF_NULL != p_sic_pdb->p_config_data) &&
           (ICF_FALSE == p_sic_pdb->p_config_data->async_notify_support_reqd))
        {
		    /* Send notify resp to SSA */
		    p_sic_pdb->notify_resp = ICF_SUCCESS;

    	    icf_sic_send_notify_resp(p_sic_pdb);
        }

		icf_sic_send_notify_msg_to_app(p_sic_pdb);
		
		/*Sending subscription state to Application*/
		ICF_SIC_SET_CURR_STATUS(
					p_sic_pdb,
					ICF_SUBS_UNSUBSCRIBED)

		p_sic_pdb->send_htb_list = ICF_FALSE;

		ret_val = icf_sic_send_subs_state_to_app(p_sic_pdb);
	
        /* 
           If asynchronous NOTIFY support is enabled, do not 
           deallocate SIC context here. Instead set a bit in 
           SIC context to indicate that SIC context needs to be 
           deallocated in the 05 state.
        */
        if((ICF_NULL != p_sic_pdb->p_config_data) &&
           (ICF_FALSE == p_sic_pdb->p_config_data->async_notify_support_reqd))
        {
		    /* Trigger SSA to clear the context and Clear SIC context*/
    		icf_sic_send_delink_to_ssa(p_sic_pdb,(icf_boolean_t)ICF_FALSE);

	    	icf_sic_dealloc_context(p_sic_pdb);    
        }
        else
        {
            p_sic_pdb->p_sic_ctx->bitmask |= ICF_SIC_CONTEXT_TO_BE_DEALLOCATED;
        }
	}
	else
	{
		icf_sic_unexpected_msg(p_sic_pdb);
	}

    /* If the SIC context has been deallocated, return from the function */
    ICF_CHECK_IF_VAR_IS_NULL(p_sic_pdb->p_glb_pdb,
                             p_sic_pdb->p_sic_ctx,
                             dummy_retval)
    if(ICF_FAILURE == dummy_retval)
    {
        /* no need to over write the ret_val value */
        ICF_FUNCTION_EXIT(p_sic_pdb->p_glb_pdb)
        return ret_val;
    }

    /* 
       If the bit ICF_SIC_NOTIFY_IND_SENT_TO_APP is set in SIC context, it means that application
       has enabled support of asynchronous NOTIFY and ICF_NOTIFY_IND has been given
       to the application. We will start a timer, save current FSM state and move to state 05.
    */
    if(ICF_SIC_NOTIFY_IND_SENT_TO_APP == 
            (ICF_SIC_NOTIFY_IND_SENT_TO_APP & p_sic_pdb->p_sic_ctx->bitmask))
    {
        icf_sic_move_to_state_await_notify_cfm(p_sic_pdb);
        
        /* reset the bit to indicate that ICF_NOTIFY_IND has been given to application */
        p_sic_pdb->p_sic_ctx->bitmask &= ~ICF_SIC_NOTIFY_IND_SENT_TO_APP;
    }

	ICF_FUNCTION_EXIT(p_sic_pdb->p_glb_pdb)
    return ret_val;
}

/*******************************************************************************
 *
 * FUNCTION:        icf_sic_04_unsub_req
 * 
 * DESCRIPTION:     This function is called when - 
 *                  State - ICF_SIC_04_SUBSCRIBE_RETRY
 *                  Event - ICF_SIC_EVENT_UNSUB_REQ
 *                  
 ******************************************************************************/
icf_return_t icf_sic_04_unsub_req(
        INOUT icf_sic_pdb_st      *p_sic_pdb)
{
    icf_return_t     ret_val = ICF_SUCCESS;
    
    ICF_FUNCTION_ENTER(p_sic_pdb->p_glb_pdb)
    
    /* Stop Subscription Retry timer */
    icf_sic_stop_curr_timer(p_sic_pdb);
    
    /* Indicate Unsubscribe success */
    ICF_SIC_SET_CURR_STATUS(
            p_sic_pdb,
            ICF_SUBS_UNSUBSCRIBED)

    icf_sic_send_subs_state_to_app(p_sic_pdb);
    
    /* Trigger SSA to clear the context and Clear SIC context
     */
    icf_sic_send_delink_to_ssa(p_sic_pdb,(icf_boolean_t)ICF_FALSE);

    icf_sic_dealloc_context(p_sic_pdb);    

    ICF_FUNCTION_EXIT(p_sic_pdb->p_glb_pdb)
    /* Always returns success */
    return ret_val;
}


/*******************************************************************************
 *
 * FUNCTION:        icf_sic_04_notify_active
 * 
 * DESCRIPTION:     This function is called when - 
 *                  State - ICF_SIC_04_SUBSCRIBE_RETRY
 *                  Event - ICF_SIC_EVENT_NOTIFY_ACTIVE
 *                  
 ******************************************************************************/
icf_return_t icf_sic_04_notify_active(
        INOUT icf_sic_pdb_st      *p_sic_pdb)
{
    icf_return_t     ret_val = ICF_SUCCESS;
    
    ICF_FUNCTION_ENTER(p_sic_pdb->p_glb_pdb)
    
    /* Send the NOTIFY data to the corresponding module.
     * Trigger SSA to send response.
     * Remain in the same state.    
     */

    /* 
       Send notify response only if application has not enabled support
       of asynchronous NOTIFY.
    */   
    if((ICF_NULL != p_sic_pdb->p_config_data) &&
       (ICF_FALSE == p_sic_pdb->p_config_data->async_notify_support_reqd))
    {
    	/* Send notify msg to the corresponding module */
	    p_sic_pdb->notify_resp = ICF_SUCCESS;

        icf_sic_send_notify_resp(p_sic_pdb);
    }
	
    icf_sic_send_notify_msg_to_app(p_sic_pdb);

    /* 
       If the bit ICF_SIC_NOTIFY_IND_SENT_TO_APP is set in SIC context, it means that application
       has enabled support of asynchronous NOTIFY and ICF_NOTIFY_IND has been given
       to the application. We will start a timer, save current FSM state and move to state 05.
    */
    if(ICF_SIC_NOTIFY_IND_SENT_TO_APP == 
            (ICF_SIC_NOTIFY_IND_SENT_TO_APP & p_sic_pdb->p_sic_ctx->bitmask))
    {
        icf_sic_move_to_state_await_notify_cfm(p_sic_pdb);
        
        /* reset the bit to indicate that ICF_NOTIFY_IND has been given to application */
        p_sic_pdb->p_sic_ctx->bitmask &= ~ICF_SIC_NOTIFY_IND_SENT_TO_APP;
    }
 
    ICF_FUNCTION_EXIT(p_sic_pdb->p_glb_pdb)
    /* Always returns success */
    return ret_val;
}


/*******************************************************************************
 *
 * FUNCTION:        icf_sic_04_notify_term_no_retry
 * 
 * DESCRIPTION:     This function is called when - 
 *                  State - ICF_SIC_04_SUBSCRIBE_RETRY
 *                  Event - ICF_SIC_EVENT_NOTIFY_TERM_NO_RETRY
 *                  
 ******************************************************************************/
icf_return_t icf_sic_04_notify_term_no_retry(
        INOUT icf_sic_pdb_st      *p_sic_pdb)
{
    icf_return_t     ret_val = ICF_SUCCESS,
                     dummy_retval = ICF_SUCCESS;
    
    ICF_FUNCTION_EXIT(p_sic_pdb->p_glb_pdb)
    
    /* 
       Send notify response only if application has not enabled support
       of asynchronous NOTIFY.
    */   
    if((ICF_NULL != p_sic_pdb->p_config_data) &&
       (ICF_FALSE == p_sic_pdb->p_config_data->async_notify_support_reqd))
    {
    	/* Send notify msg to the corresponding module */
	    p_sic_pdb->notify_resp = ICF_SUCCESS;

        icf_sic_send_notify_resp(p_sic_pdb);
    }

	icf_sic_send_notify_msg_to_app(p_sic_pdb);
    
    /* Stop Retry timer */
    icf_sic_stop_curr_timer(p_sic_pdb);
    
    /* Send the subscription status */
    ICF_SIC_SET_CURR_STATUS(
            p_sic_pdb,
            ICF_SUBS_FAIL_NO_RETRY)

    icf_sic_send_subs_state_to_app(p_sic_pdb);
    
    /* 
       If asynchronous NOTIFY support is enabled, do not 
       deallocate SIC context here. Instead set a bit in 
       SIC context to indicate that SIC context needs to be 
       deallocated in the 05 state.
    */
    if((ICF_NULL != p_sic_pdb->p_config_data) &&
       (ICF_FALSE == p_sic_pdb->p_config_data->async_notify_support_reqd))
    {
        /* clear SSA Ctx and go to state 6 */
        icf_sic_send_delink_to_ssa(p_sic_pdb,(icf_boolean_t)ICF_FALSE);
    
    	icf_sic_dealloc_context(p_sic_pdb);
    }
    else
    {
        p_sic_pdb->p_sic_ctx->bitmask |= ICF_SIC_CONTEXT_TO_BE_DEALLOCATED;
    }

    /* If the SIC context has been deallocated, return from the function */
    ICF_CHECK_IF_VAR_IS_NULL(p_sic_pdb->p_glb_pdb,
                             p_sic_pdb->p_sic_ctx,
                             dummy_retval)
    if(ICF_FAILURE == dummy_retval)
    {
        /* no need to over write the ret_val value */
        ICF_FUNCTION_EXIT(p_sic_pdb->p_glb_pdb)
        return ret_val;
    }

    /* 
       If the bit ICF_SIC_NOTIFY_IND_SENT_TO_APP is set in SIC context, it means that application
       has enabled support of asynchronous NOTIFY and ICF_NOTIFY_IND has been given
       to the application. We will start a timer, save current FSM state and move to state 05.
    */
    if(ICF_SIC_NOTIFY_IND_SENT_TO_APP == 
            (ICF_SIC_NOTIFY_IND_SENT_TO_APP & p_sic_pdb->p_sic_ctx->bitmask))
    {
        icf_sic_move_to_state_await_notify_cfm(p_sic_pdb);
        
        /* reset the bit to indicate that ICF_NOTIFY_IND has been given to application */
        p_sic_pdb->p_sic_ctx->bitmask &= ~ICF_SIC_NOTIFY_IND_SENT_TO_APP;
    }

    ICF_FUNCTION_EXIT(p_sic_pdb->p_glb_pdb)
    return ret_val;
}

/*******************************************************************************
 *
 * FUNCTION:        icf_sic_04_notify_term_retry_imm
 * 
 * DESCRIPTION:     This function is called when - 
 *                  State - ICF_SIC_04_SUBSCRIBE_RETRY
 *                  Event - ICF_SIC_EVENT_NOTIFY_TERM_RETRY_IMM
 *                  
 ******************************************************************************/
icf_return_t icf_sic_04_notify_term_retry_imm(
        INOUT icf_sic_pdb_st      *p_sic_pdb)
{
    icf_return_t     ret_val = ICF_SUCCESS;
    icf_return_t     dummy_retval = ICF_SUCCESS;
    icf_error_t      ecode = ICF_ERROR_NONE;
    icf_return_t     return_val = ICF_SUCCESS;
    
    ICF_FUNCTION_ENTER(p_sic_pdb->p_glb_pdb)
    
    /* 
       Send notify response only if application has not enabled support
       of asynchronous NOTIFY.
    */   
    if((ICF_NULL != p_sic_pdb->p_config_data) &&
       (ICF_FALSE == p_sic_pdb->p_config_data->async_notify_support_reqd))
    { 
	    /* Send notify msg to the corresponding module */
	    p_sic_pdb->notify_resp = ICF_SUCCESS;

        icf_sic_send_notify_resp(p_sic_pdb);
    }

	icf_sic_send_notify_msg_to_app(p_sic_pdb);

    /* Stop Retry timer */
    icf_sic_stop_curr_timer(p_sic_pdb);

    /* Start Subscription Response timer */
    ICF_SIC_SET_TIMER_TYPE(p_sic_pdb,ICF_TIMER_TYPE_SUBS_RESPONSE)

    dummy_retval = icf_sic_start_timer(p_sic_pdb);
    
    /* If the timer is started then trigger SSA to send SUBSCRIBE request */
    if (ICF_SUCCESS == dummy_retval)
    {
        /*cloning the header list from persistent header list */
        if(0 < p_sic_pdb->p_sic_ctx->persistent_header_list.count)
        {
            /*before cloning we are freeing the header list(if present)
            in the sic context*/
            if(0 < p_sic_pdb->p_sic_ctx->header_list.count)
            { 
                icf_cmn_free_header_list(
                            p_sic_pdb->p_glb_pdb,
                            &(p_sic_pdb->p_sic_ctx->header_list),
                            &ecode);
            }
            icf_cmn_clone_header_list(
                            p_sic_pdb->p_glb_pdb,
                            &(p_sic_pdb->p_sic_ctx->persistent_header_list),
                            &(p_sic_pdb->p_sic_ctx->header_list),
                            p_sic_pdb->p_ecode);
        }
        icf_sic_send_subs_req(p_sic_pdb);
    }
    /* If SIC fails to start the timer then  indicate the failure and 
     * go to state 5
     */
    else
    {
        ICF_ALARM(((icf_uint8_t *)"[SIC]: Timer Failure"));

        ICF_SIC_SET_CURR_STATUS(
                p_sic_pdb,
                ICF_SUBS_FAIL_NO_RETRY)

        icf_sic_send_subs_state_to_app(p_sic_pdb);
	
        /* 
           If asynchronous NOTIFY support is enabled, do not 
           deallocate SIC context here. Instead set a bit in 
           SIC context to indicate that SIC context needs to be 
           deallocated in the 05 state.
        */
        if((ICF_NULL != p_sic_pdb->p_config_data) &&
           (ICF_FALSE == p_sic_pdb->p_config_data->async_notify_support_reqd))
        {
		    icf_sic_send_delink_to_ssa(p_sic_pdb,
    					(icf_boolean_t)ICF_FALSE);            
        
	    	icf_sic_dealloc_context(p_sic_pdb);    
        }
        else
        {
            p_sic_pdb->p_sic_ctx->bitmask |= ICF_SIC_CONTEXT_TO_BE_DEALLOCATED;
        }
    }

    /* If the SIC context has been deallocated, return from the function */
    ICF_CHECK_IF_VAR_IS_NULL(p_sic_pdb->p_glb_pdb,
                             p_sic_pdb->p_sic_ctx,
                             return_val)
    if(ICF_FAILURE == return_val)
    {
        /* no need to over write the ret_val value */
        ICF_FUNCTION_EXIT(p_sic_pdb->p_glb_pdb)
        return ret_val;
    }

    /* 
       If the bit ICF_SIC_NOTIFY_IND_SENT_TO_APP is set in SIC context, it means that application
       has enabled support of asynchronous NOTIFY and ICF_NOTIFY_IND has been given
       to the application. We will start a timer, save current FSM state and move to state 05.
    */
    if(ICF_SIC_NOTIFY_IND_SENT_TO_APP == 
            (ICF_SIC_NOTIFY_IND_SENT_TO_APP & p_sic_pdb->p_sic_ctx->bitmask))
    {
        icf_sic_move_to_state_await_notify_cfm(p_sic_pdb);
        
        /* reset the bit to indicate that ICF_NOTIFY_IND has been given to application */
        p_sic_pdb->p_sic_ctx->bitmask &= ~ICF_SIC_NOTIFY_IND_SENT_TO_APP;
    }

    ICF_FUNCTION_EXIT(p_sic_pdb->p_glb_pdb)
    /* Always returns success */
    return ret_val;
}/* End function */


/*******************************************************************************
 *
 * FUNCTION:        icf_sic_04_notify_term_retry_after
 * 
 * DESCRIPTION:     This function is called when - 
 *                  State - ICF_SIC_04_SUBSCRIBE_RETRY
 *                  Event - ICF_SIC_EVENT_NOTIFY_TERM_RETRY_AFTER
 *                  
 ******************************************************************************/
icf_return_t icf_sic_04_notify_term_retry_after(
        INOUT icf_sic_pdb_st      *p_sic_pdb)
{
    icf_return_t     ret_val = ICF_SUCCESS;
    icf_return_t     dummy_retval = ICF_SUCCESS;
    icf_nw_inc_notify_ind_st *p_notify_ind = ICF_NULL;
    icf_return_t     return_val = ICF_SUCCESS;
    
    ICF_FUNCTION_ENTER(p_sic_pdb->p_glb_pdb)

    /* 
       Send notify response only if application has not enabled support
       of asynchronous NOTIFY.
    */   
    if((ICF_NULL != p_sic_pdb->p_config_data) &&
       (ICF_FALSE == p_sic_pdb->p_config_data->async_notify_support_reqd))
    {
        /* Send notify msg to the corresponding module */
    	p_sic_pdb->notify_resp = ICF_SUCCESS;

        icf_sic_send_notify_resp(p_sic_pdb);
    }

	icf_sic_send_notify_msg_to_app(p_sic_pdb);

    p_notify_ind = (icf_nw_inc_notify_ind_st *)(p_sic_pdb->p_msg);
    
    /* Restart the retry timer with if the duration is received */
    if (ICF_NW_INC_SUBS_DURATION_PRESENT & p_notify_ind->bitmask)
    {
        /* Stop Retry timer */
        icf_sic_stop_curr_timer(p_sic_pdb);
        
		/* Fix for timer units - 
		 * timer_duration == is in milliseconds
		 * p_notify_ind->duration == is in seconds
		 * p_sic_ctx->duration == is in seconds */
        p_sic_pdb->timer_duration = p_notify_ind->duration * 1000;
    }
        
    /* if the timer is stopped */
    if (ICF_TIMER_TYPE_SUBS_RETRY != p_sic_pdb->p_sic_ctx->timer_id)
    {
        ICF_SIC_SET_TIMER_TYPE(p_sic_pdb,ICF_TIMER_TYPE_SUBS_RETRY)    

        dummy_retval = icf_sic_start_timer(p_sic_pdb);
        
        /* If SIC fails to restart the timer then go to blocked state 5 */
        if (ICF_FAILURE == dummy_retval)
        {
            ICF_ALARM(((icf_uint8_t *)"[SIC]: Timer Failure"));

            ICF_SIC_SET_CURR_STATUS(
                    p_sic_pdb,
                    ICF_SUBS_FAIL_NO_RETRY)

            icf_sic_send_subs_state_to_app(p_sic_pdb);

            /* 
               If asynchronous NOTIFY support is enabled, do not 
               deallocate SIC context here. Instead set a bit in 
               SIC context to indicate that SIC context needs to be 
               deallocated in the 05 state.
            */
            if((ICF_NULL != p_sic_pdb->p_config_data) &&
               (ICF_FALSE == p_sic_pdb->p_config_data->async_notify_support_reqd))
            {
    			icf_sic_send_delink_to_ssa(p_sic_pdb,
	    					(icf_boolean_t)ICF_FALSE);            
            
		    	icf_sic_dealloc_context(p_sic_pdb);    
            }
            else
            {
                p_sic_pdb->p_sic_ctx->bitmask |= ICF_SIC_CONTEXT_TO_BE_DEALLOCATED;
            }
        }
        /* If the timer is started then remain in the same state */
    }/* if (ICF_TIMER_TYPE_SUBS_RETRY..*/
  
    /* If the SIC context has been deallocated, return from the function */
    ICF_CHECK_IF_VAR_IS_NULL(p_sic_pdb->p_glb_pdb,
                             p_sic_pdb->p_sic_ctx,
                             return_val)
    if(ICF_FAILURE == return_val)
    {
        /* no need to over write the ret_val value */
        ICF_FUNCTION_EXIT(p_sic_pdb->p_glb_pdb)
        return ret_val;
    }

    /* 
       If the bit ICF_SIC_NOTIFY_IND_SENT_TO_APP is set in SIC context, it means that application
       has enabled support of asynchronous NOTIFY and ICF_NOTIFY_IND has been given
       to the application. We will start a timer, save current FSM state and move to state 05.
    */
    if(ICF_SIC_NOTIFY_IND_SENT_TO_APP == 
            (ICF_SIC_NOTIFY_IND_SENT_TO_APP & p_sic_pdb->p_sic_ctx->bitmask))
    {
        icf_sic_move_to_state_await_notify_cfm(p_sic_pdb);
        
        /* reset the bit to indicate that ICF_NOTIFY_IND has been given to application */
        p_sic_pdb->p_sic_ctx->bitmask &= ~ICF_SIC_NOTIFY_IND_SENT_TO_APP;
    }

    ICF_FUNCTION_EXIT(p_sic_pdb->p_glb_pdb)
    return ret_val;
}

/*******************************************************************************
 *
 * FUNCTION:        icf_sic_04_retry_timer_exp
 * 
 * DESCRIPTION:     This function is called when - 
 *                  State - ICF_SIC_04_SUBSCRIBE_RETRY
 *                  Event - ICF_SIC_EVENT_RETRY_TIMER_EXP
 *                  
 ******************************************************************************/
icf_return_t icf_sic_04_retry_timer_exp(
        INOUT icf_sic_pdb_st      *p_sic_pdb)
{
    icf_return_t     ret_val = ICF_SUCCESS;
    icf_return_t     dummy_retval = ICF_SUCCESS;
    icf_error_t      ecode = ICF_ERROR_NONE;
    
    ICF_FUNCTION_ENTER(p_sic_pdb->p_glb_pdb)
    
    /* Start Subscription Response timer */
    ICF_SIC_SET_TIMER_TYPE(p_sic_pdb,ICF_TIMER_TYPE_SUBS_RESPONSE)

    dummy_retval = icf_sic_start_timer(p_sic_pdb);

    /* If the response timer is started then trigger SSA to send 
     * Subscribe Request
     */
    if (ICF_SUCCESS == dummy_retval)
    {
         /*cloning the header list from persistent header list */
        if(0 < p_sic_pdb->p_sic_ctx->persistent_header_list.count)
        {
            /*before cloning we are freeing the header list(if present)
            in the sic context*/
            if(0 <  p_sic_pdb->p_sic_ctx->header_list.count)
            {
                icf_cmn_free_header_list(
                        p_sic_pdb->p_glb_pdb,
                        &(p_sic_pdb->p_sic_ctx->header_list),
                        &ecode);
            }
            icf_cmn_clone_header_list(
                        p_sic_pdb->p_glb_pdb,
                        &(p_sic_pdb->p_sic_ctx->persistent_header_list),
                        &(p_sic_pdb->p_sic_ctx->header_list),
                        p_sic_pdb->p_ecode);
        }

        icf_sic_send_subs_req(p_sic_pdb);
    }
    /* If SIC fails to start the timer then indicate the failure, 
     * go to 5 state 
     */
    else
    {
        ICF_ALARM(((icf_uint8_t *)"[SIC]: Timer Failure"));

        ICF_SIC_SET_CURR_STATUS(
                p_sic_pdb,
                ICF_SUBS_FAIL_NO_RETRY)

        icf_sic_send_subs_state_to_app(p_sic_pdb);

		icf_sic_send_delink_to_ssa(p_sic_pdb,
					(icf_boolean_t)ICF_FALSE);            
        
		icf_sic_dealloc_context(p_sic_pdb);    
    }
    
    ICF_FUNCTION_EXIT(p_sic_pdb->p_glb_pdb)
    return ret_val;
}/* End function */


/*******************************************************************************
 *
 * FUNCTION:        icf_sic_handle_forceful_delete
 *  
 * DESCRIPTION:     This function is called when - 
 *                  Event - ICF_SIC_EVENT_UNSUB_REQ
 *                  
 ******************************************************************************/
icf_return_t icf_sic_handle_forceful_delete(
        INOUT icf_sic_pdb_st      *p_sic_pdb)
{
    icf_return_t     ret_val = ICF_SUCCESS;
   
   if(ICF_NULL == p_sic_pdb)
   {
       ICF_PRINT(((icf_uint8_t *)"p_sic_pdb is null"));
       return ICF_SUCCESS;

   }

	p_sic_pdb = p_sic_pdb;

    ICF_FUNCTION_ENTER(p_sic_pdb->p_glb_pdb)

	/*This function is not called from anywhere, for future use */

    ICF_FUNCTION_EXIT(p_sic_pdb->p_glb_pdb)
    /* Always success */       
    return ret_val;
}/* End function */

/* ICF_6_1_OPTIONS_SUPPORT_START */

/*******************************************************************************
 *
 * FUNCTION:            icf_sic_00_inc_options_req
 *
 * DESCRIPTION:         This Function will be invoked when an SIP OPTIONS
 *                      Request is received from network.
 *
 * CURRENT_STATE:       ICF_SIC_00_OPTIONS_IDLE
 *
 * CURRENT_EVENT:       ICF_SIC_EVENT_INC_OPTIONS_REQ
 *     
 * NEXT_STATES:
 *                      1) ICF_SIC_01_AWT_MEDIA_CAPAB_RSP
 *                      2) ICF_SIC_02_AWT_OPTIONS_APP_CFM
 *                      3) ICF_SIC_04_AWT_EARLY_SESSION_CFM
 *
 ******************************************************************************/
icf_return_t icf_sic_00_inc_options_req(
                icf_sic_pdb_st  *p_sic_pdb)
{
    icf_return_t ret_val = ICF_SUCCESS;
    icf_nw_inc_opt_ind_st *p_inc_opt_ind = ICF_NULL;
    icf_app_conf_st *p_app_conf = ICF_NULL;
    icf_list_st	 *p_app_media_capab	= ICF_NULL;

    ICF_FUNCTION_ENTER(p_sic_pdb->p_glb_pdb)

    p_inc_opt_ind = (icf_nw_inc_opt_ind_st *)
                       (p_sic_pdb->p_msg);	
    p_sic_pdb->p_sic_ctx->options_timer_id = 
    	ICF_TIMER_ID_INVALID;
    p_sic_pdb->p_sic_ctx->p_options_timer =
    	ICF_NULL;
    p_sic_pdb->p_sic_ctx->p_mm_media_capab = 
    	ICF_NULL;
    p_sic_pdb->p_sic_ctx->pending_event = 0;

   	/* Code for in-dialog option requet */
    if(ICF_FAILURE == icf_dbm_find_call_cntxt_blk(
	    				p_sic_pdb->p_glb_pdb,
	    				p_sic_pdb->p_sic_ctx->call_id,
	    				&(p_sic_pdb->p_sic_ctx->p_call_ctx),
	    				p_sic_pdb->p_ecode))
    {
        p_sic_pdb->p_sic_ctx->p_call_ctx = ICF_NULL;
    }
    else
    {
        /* set the in-dialog option bit in the call context */
        p_sic_pdb->p_sic_ctx->p_call_ctx->common_bitmask |= ICF_IN_DLG_OPTIONS_RECV;
    }
		
    ret_val = icf_sic_start_options_timer(p_sic_pdb);
    if ( ICF_FAILURE == ret_val)
    {
        icf_sic_send_failure_response_to_ssa(p_sic_pdb);
        ICF_FUNCTION_EXIT(p_sic_pdb->p_glb_pdb)
        return ret_val;
    }
    /*  Rel 8.1
     *  if out of dialog options received then send
     *  ICF_EARLY_SESSION_IND to application 
     */
    if(ICF_TRUE == p_sic_pdb->p_config_data->early_incoming_session_ind_reqd)
    {
        /* Rel 8.1
         * Out of dialog OPTIONS then send ICF_EARLY_SESSION_IND
         * and move to state 04
         */
        if (ICF_NULL == p_sic_pdb->p_sic_ctx->p_call_ctx)
        {
            /* SPR 19517 fix: Resolved compile time warnings on Vxworks */
            ret_val = icf_sic_send_early_session_ind_to_app(p_sic_pdb);
            /* End of SPR 19517 fix */ 
            if(ICF_FAILURE == ret_val)
            {
                icf_sic_stop_options_timer(p_sic_pdb);
                icf_sic_send_failure_response_to_ssa(p_sic_pdb);
                ICF_FUNCTION_EXIT(p_sic_pdb->p_glb_pdb)
                return ret_val;
            }
            ICF_SIC_SET_STATE(p_sic_pdb,ICF_SIC_04_AWT_EARLY_SESSION_CFM)
            p_sic_pdb->p_sic_ctx->is_sdp_reqd = p_inc_opt_ind->is_sdp_reqd;
        }
        /* Rel 8.1
         * If OPTIONS received Indialog
         * case 1: If tunnel mode is TRUE  
         *         then send options_ind 
         *         if sdp required then send media_cap_req
         *         and move to state 01
         *         else move to state 02
         * case 2: If tunnel mode false 
         *         if sdp reqd is true then send 
         *         media_cap_req and move to state 01
         *         else
         *         send options_ind and move to state 02
         */
        else
        {
ICF_CHECK_TUNNEL_MODE_START(p_sic_pdb->p_glb_pdb,ICF_TRUE)
                /* SPR 19517 fix: Resolved compile time warnings on Vxworks */
                ret_val = icf_sic_send_options_ind_to_app(p_sic_pdb);
                /* End of SPR 19517 fix */ 
                if(ICF_FAILURE == ret_val)
                {
                    icf_sic_stop_options_timer(p_sic_pdb);
                    icf_sic_send_failure_response_to_ssa(p_sic_pdb);
                    ICF_FUNCTION_EXIT(p_sic_pdb->p_glb_pdb)
                    return ret_val;
                }
ICF_CHECK_TUNNEL_MODE_END
            if ( ICF_TRUE == p_inc_opt_ind->is_sdp_reqd)
            {
ICF_CHECK_TUNNEL_MODE_START(p_sic_pdb->p_glb_pdb,ICF_FALSE)
                /* Rel 8.1
                 * get the media profile from app_add
                 * to send in ICF_MEDIA_CAPABILITIES_REQ
                 */
                if(ICF_SUCCESS ==
                icf_cfg_fetch_app_conf(p_sic_pdb->p_glb_pdb,p_sic_pdb->p_sic_ctx->app_id,
                &p_app_conf,
                p_sic_pdb->p_ecode))
                {
                    if((ICF_NULL != p_app_conf) &&
                               (ICF_TRUE == p_app_conf->is_valid))
                    {
                        if ( ICF_NULL !=
                             p_app_conf->p_app_media_profile)
                        {
                            icf_sic_map_icf_internal_list_icf_list(
                               p_sic_pdb,
                               p_app_conf->p_app_media_profile,
                               &p_app_media_capab);
                        }
                        else
                            p_app_media_capab = ICF_NULL;
                    }
                }
ICF_CHECK_TUNNEL_MODE_END
                ret_val = 
                icf_sic_send_media_capab_req_to_mmi(p_sic_pdb,p_app_media_capab);
                /*  KlocWorks Warning Resolution */
                if(ICF_NULL != p_app_media_capab)
                {
                    icf_cmn_free_icf_stream_capab_list(
                           p_sic_pdb->p_glb_pdb,
                           &p_app_media_capab);
                }
                if(ICF_FAILURE == ret_val)
                {
                    icf_sic_stop_options_timer(p_sic_pdb);
                    icf_sic_send_failure_response_to_ssa(p_sic_pdb);
                    ICF_FUNCTION_EXIT(p_sic_pdb->p_glb_pdb)
                    return ret_val;
                }
                ICF_SIC_SET_STATE(p_sic_pdb,ICF_SIC_01_AWT_MEDIA_CAPAB_RSP)
           }
           else
           {
ICF_CHECK_TUNNEL_MODE_START(p_sic_pdb->p_glb_pdb,ICF_FALSE)
               /* SPR 19517 fix: Resolved compile time warnings on Vxworks */ 
               ret_val = icf_sic_send_options_ind_to_app(p_sic_pdb);
               /* End of SPR 19517 fix */ 
               if(ICF_FAILURE == ret_val)
               {
                   icf_sic_stop_options_timer(p_sic_pdb);
                   icf_sic_send_failure_response_to_ssa(p_sic_pdb);
                   ICF_FUNCTION_EXIT(p_sic_pdb->p_glb_pdb)
                   return ret_val;
               }
ICF_CHECK_TUNNEL_MODE_END
               ICF_SIC_SET_STATE(p_sic_pdb,ICF_SIC_02_AWT_OPTIONS_APP_CFM)
           }
       }
    } 
    /*  Rel 8.1
     *  if early_incoming_session_reqd is false
     */  
    else
    {
        /*  Rel 8.1
         *  SCENARIO :
         *  case 1: when early_incoming_session_reqd is FALSE
         *          and it is out of dialog options then 
         *  case 2: when early_incoming_session_reqd is FALSE
         *          and it is indialog options and 
         *          tunnel mode is false in call ctxt
         *  then in both the cases do the normal handling.
         *  Here the macr is not used to check the tunnel mode 
         *  because then in a single condition the data can 
         *  not be put as it is done here.
         */
        if(ICF_NULL == p_sic_pdb->p_sic_ctx->p_call_ctx
           || ((ICF_NULL != p_sic_pdb->p_sic_ctx->p_call_ctx)
           && (ICF_FALSE == p_sic_pdb->p_sic_ctx->p_call_ctx->tunnel_mode)))
        {
            /* if is_sdp_reqd is true then SIC will send
             * media capabilities request to MMI 
             * otherwise SIC will send options_ind
             * to Appliction.
             */
            if ( ICF_TRUE == p_inc_opt_ind->is_sdp_reqd)
            {
                /* Rel 8.1 
                 * Fetch app_media_profile from app_add
                 * to send it in ICF_MEDIA_CAPABILITIES_REQ
                 */
                if(ICF_SUCCESS == 
                    icf_cfg_fetch_app_conf(p_sic_pdb->p_glb_pdb,p_sic_pdb->p_sic_ctx->app_id, 
                    &p_app_conf,
                    p_sic_pdb->p_ecode))
                {
                    if((ICF_NULL != p_app_conf) &&
                               (ICF_TRUE == p_app_conf->is_valid))
                    {
                        if ( ICF_NULL !=
                             p_app_conf->p_app_media_profile)
                        {
                          icf_sic_map_icf_internal_list_icf_list(
                               p_sic_pdb,
                               p_app_conf->p_app_media_profile,
                               &p_app_media_capab);
                        }
                        else
                           	p_app_media_capab = ICF_NULL;
                         ret_val = icf_sic_send_media_capab_req_to_mmi(p_sic_pdb,p_app_media_capab);
                         icf_cmn_free_icf_stream_capab_list(
                           p_sic_pdb->p_glb_pdb,
                           &p_app_media_capab); 
                         if ( ICF_FAILURE == ret_val)
                         {
                             icf_sic_stop_options_timer(p_sic_pdb);
                             icf_sic_send_failure_response_to_ssa(p_sic_pdb);
                             ICF_FUNCTION_EXIT(p_sic_pdb->p_glb_pdb)
                             return ret_val;
                         }
                         ICF_SIC_SET_STATE(p_sic_pdb,ICF_SIC_01_AWT_MEDIA_CAPAB_RSP)
                    }
                    else
                    {
                        if ( ICF_FAILURE == ret_val)
                        {
                            icf_sic_stop_options_timer(p_sic_pdb);
                            icf_sic_send_failure_response_to_ssa(p_sic_pdb);
                            ICF_FUNCTION_EXIT(p_sic_pdb->p_glb_pdb)
                            return ret_val;
                        }
                    }
                }
                else
                {
                    icf_sic_stop_options_timer(p_sic_pdb);
                    icf_sic_send_failure_response_to_ssa(p_sic_pdb);
                    ICF_FUNCTION_EXIT(p_sic_pdb->p_glb_pdb)
                    return ret_val;
                }
            }
            else /*  when SDP Is not required */
            {
                ret_val = icf_sic_send_options_ind_to_app(p_sic_pdb);
                if ( ICF_FAILURE == ret_val)
                {
                    icf_sic_stop_options_timer(p_sic_pdb);
                    icf_sic_send_failure_response_to_ssa(p_sic_pdb);
                    ICF_FUNCTION_EXIT(p_sic_pdb->p_glb_pdb)
                    return ret_val;
                }
                ICF_SIC_SET_STATE(p_sic_pdb,ICF_SIC_02_AWT_OPTIONS_APP_CFM)
            }
        }
        else
ICF_CHECK_TUNNEL_MODE_START(p_sic_pdb->p_glb_pdb, ICF_TRUE)
        {
            /* Rel 8.1
             * If Tunnel mode is ON
             * then OPTIONS_IND need to be sent
             * followed by ICF_MEDIA_CAPABILITIES_REQ
             */
            ret_val = icf_sic_send_options_ind_to_app(p_sic_pdb);
            if ( ICF_FAILURE == ret_val)
            {
                icf_sic_stop_options_timer(p_sic_pdb);
                icf_sic_send_failure_response_to_ssa(p_sic_pdb);
                ICF_FUNCTION_EXIT(p_sic_pdb->p_glb_pdb)
                return ret_val;
            }

            if(ICF_TRUE == p_inc_opt_ind->is_sdp_reqd)
            {
                ret_val =
                icf_sic_send_media_capab_req_to_mmi(p_sic_pdb,p_app_media_capab);
                if(ICF_FAILURE == ret_val)
                {
                    icf_sic_stop_options_timer(p_sic_pdb);
                    icf_sic_send_failure_response_to_ssa(p_sic_pdb);
                    ICF_FUNCTION_EXIT(p_sic_pdb->p_glb_pdb)
                    return ret_val;
                }
                ICF_SIC_SET_STATE(p_sic_pdb,ICF_SIC_01_AWT_MEDIA_CAPAB_RSP)
            }
            else
            {
                ICF_SIC_SET_STATE(p_sic_pdb,ICF_SIC_02_AWT_OPTIONS_APP_CFM)
            }
        }
ICF_CHECK_TUNNEL_MODE_END
    }
    
    ICF_FUNCTION_EXIT(p_sic_pdb->p_glb_pdb)
    
    return ret_val;

} /* End of icf_sic_00_inc_options_req */

/*******************************************************************************
 *
 * FUNCTION:            icf_sic_00_otg_options_req
 *
 * DESCRIPTION:         This Function will be invoked when application
 *                      sends icf_options_req_st API to ICF.
 *
 * CURRENT_STATE:       ICF_SIC_00_OPTIONS_IDLE
 *
 * CURRENT_EVENT:       ICF_SIC_EVENT_OTG_OPTIONS_REQ
 *
 * NEXT_STATES:
 *                      1) ICF_SIC_03_AWT_OPTIONS_NW_CFM
 *
 ******************************************************************************/
icf_return_t icf_sic_00_otg_options_req(
                icf_sic_pdb_st  *p_sic_pdb)
{

	icf_return_t ret_val = ICF_SUCCESS;
	
	ICF_FUNCTION_ENTER(p_sic_pdb->p_glb_pdb)

 	 p_sic_pdb->p_sic_ctx->options_timer_id = 
                ICF_TIMER_ID_INVALID;
     p_sic_pdb->p_sic_ctx->p_options_timer =
                ICF_NULL;
     p_sic_pdb->p_sic_ctx->p_mm_media_capab = 
                ICF_NULL;
     p_sic_pdb->p_sic_ctx->pending_event = 0;

     p_sic_pdb->p_sic_ctx->p_sdp_list = ICF_NULL;
 
    ret_val = icf_sic_process_options_req(p_sic_pdb);
    /* incase of send or any other failure, failure response is sent to the application in the
     * function icf_sic_process_options_req */
	if ( ICF_SUCCESS == ret_val )
	{
	    ICF_SIC_SET_STATE(p_sic_pdb,ICF_SIC_03_AWT_OPTIONS_NW_CFM)
    }

	ICF_FUNCTION_EXIT(p_sic_pdb->p_glb_pdb)

	return ret_val;
} /* End of icf_sic_00_otg_options_req */

/*******************************************************************************
 *
 * FUNCTION:            icf_sic_01_media_capabilites_resp
 *
 * DESCRIPTION:         This Function will be invoked when MMI sebds
 *                      media capabilites response.
 *
 * CURRENT_STATE:       ICF_SIC_01_AWT_MEDIA_CAPAB_RSP
 *
 * CURRENT_EVENT:       ICF_SIC_EVENT_MEDIA_CAPAB_RSP
 *
 * NEXT_STATES:
 *                      1) ICF_SIC_02_AWT_OPTIONS_APP_CFM
 *                      2) ICF_SIC_00_OPTIONS_IDLE
 *
 ******************************************************************************/
 icf_return_t icf_sic_01_media_capabilites_resp(
                icf_sic_pdb_st  *p_sic_pdb)
{
    icf_return_t             ret_val = ICF_SUCCESS;
  	icf_get_media_cap_resp_st *p_med_cap_resp = ICF_NULL;

    ICF_FUNCTION_ENTER(p_sic_pdb->p_glb_pdb)

	if ( ICF_SIC_PENDING_CLEAR_OPTIONS_CTX == 
		(p_sic_pdb->p_sic_ctx->pending_event &
			ICF_SIC_PENDING_CLEAR_OPTIONS_CTX))
	{
		icf_sic_stop_options_timer(p_sic_pdb);
		icf_sic_dealloc_context(p_sic_pdb);
		return ICF_SUCCESS;
	} 
	p_med_cap_resp = (icf_get_media_cap_resp_st*)
				(p_sic_pdb->p_msg);

	if ( ICF_FAILURE == p_med_cap_resp->result)
	{
        icf_sic_stop_options_timer(p_sic_pdb);
        icf_sic_send_failure_response_to_ssa(p_sic_pdb);
        ICF_FUNCTION_EXIT(p_sic_pdb->p_glb_pdb)
        return ret_val;
	} 		
	icf_sic_extract_mm_stream_capab(p_sic_pdb);
/* Rel 8.1
 * If it is tunnel mode then do not send ICF_OPTIONS_IND
 * as it has already been sent when ICF_MEDIA_CAPABILITIES_REQ 
 * is sent
 */
ICF_CHECK_TUNNEL_MODE_START(p_sic_pdb->p_glb_pdb, ICF_FALSE)
	ret_val = icf_sic_send_options_ind_to_app(p_sic_pdb);
	if ( ICF_FAILURE == ret_val)
	{
		icf_sic_stop_options_timer(p_sic_pdb);
		icf_sic_send_failure_response_to_ssa(p_sic_pdb);
		ICF_FUNCTION_EXIT(p_sic_pdb->p_glb_pdb)
		return ret_val;
	}
ICF_CHECK_TUNNEL_MODE_END
	
	ICF_SIC_SET_STATE(p_sic_pdb,ICF_SIC_02_AWT_OPTIONS_APP_CFM);

	ICF_FUNCTION_EXIT(p_sic_pdb->p_glb_pdb)
    return ret_val;

} /* End of icf_sic_01_media_capabilites_resp */

/*******************************************************************************
 *
 * FUNCTION:            icf_sic_01_02_timer_expiry
 *
 * DESCRIPTION:         This Function will be invoked when options timer
 *                      expiry event is received
 *
 * CURRENT_STATE:       ICF_SIC_01_AWT_MEDIA_CAPAB_RSP or
 *                      ICF_SIC_02_AWT_OPTIONS_APP_CFM
 *
 * CURRENT_EVENT:       ICF_SIC_EVENT_TIMER_EXPIRY
 *
 * NEXT_STATES:
 *                      1) ICF_SIC_00_OPTIONS_IDLE
 *
 ******************************************************************************/
icf_return_t icf_sic_01_02_timer_expiry(
                icf_sic_pdb_st  *p_sic_pdb)
{

	icf_return_t ret_val = ICF_SUCCESS;
	
	ICF_FUNCTION_ENTER(p_sic_pdb->p_glb_pdb)

        ICF_SIC_APP_TRACE(p_sic_pdb,ICF_TRACE_OPTIONS_TIMER_EXPIRE)

        if ( ICF_SIC_PENDING_CLEAR_OPTIONS_CTX ==
                (p_sic_pdb->p_sic_ctx->pending_event &
                        ICF_SIC_PENDING_CLEAR_OPTIONS_CTX))
        {
                icf_sic_dealloc_context(p_sic_pdb);
                return ICF_SUCCESS;
        }

	icf_sic_send_failure_response_to_ssa(p_sic_pdb);

	ICF_FUNCTION_EXIT(p_sic_pdb->p_glb_pdb)

	return ret_val;
} /* End of icf_sic_01_02_timer_expiry */

/*******************************************************************************
 *
 * FUNCTION:            icf_sic_01_02_ssa_clear_options_ctx
 *
 * DESCRIPTION:         This Function will be invoked when an SSA 
 *                      sends ICF_SIC_CLEAR_OPTIONS_CTX
 *
 * CURRENT_STATE:       ICF_SIC_01_AWT_MEDIA_CAPAB_RSP or
 *			ICF_SIC_02_AWT_OPTIONS_APP_CFM
 *
 * CURRENT_EVENT:       ICF_SIC_EVENT_CLEAR_OPTIONS_CTX
 *
 * NEXT_STATES:
 *                      same state
 *
 ******************************************************************************/
icf_return_t icf_sic_01_02_ssa_clear_options_ctx(
                icf_sic_pdb_st  *p_sic_pdb)
{
    icf_return_t	ret_val = ICF_SUCCESS;
    ICF_FUNCTION_ENTER(p_sic_pdb->p_glb_pdb)

	p_sic_pdb->p_sic_ctx->pending_event |= 
		ICF_SIC_PENDING_CLEAR_OPTIONS_CTX;

	ICF_FUNCTION_EXIT(p_sic_pdb->p_glb_pdb)
    return ret_val;

}


/*******************************************************************************
 *
 * FUNCTION:            icf_sic_02_options_app_cfm
 *
 * DESCRIPTION:         This Function will be invoked when application sends
 *                      icf_options_cfm_st API to ICF.
 *
 * CURRENT_STATE:       ICF_SIC_02_AWT_OPTIONS_APP_CFM
 *
 * CURRENT_EVENT:       ICF_SIC_EVENT_OPTIONS_APP_CFM
 *
 * NEXT_STATES:
 *                      1) ICF_SIC_00_OPTIONS_IDLE
 *
 ******************************************************************************/
 icf_return_t icf_sic_02_options_app_cfm(
                icf_sic_pdb_st  *p_sic_pdb)
{

    icf_return_t             ret_val = ICF_SUCCESS;
    icf_list_st		*p_sdp_list = ICF_NULL;
	icf_sic_options_cfm_st *p_options_cfm = ICF_NULL;
   
     ICF_FUNCTION_ENTER(p_sic_pdb->p_glb_pdb)

    icf_sic_stop_options_timer(p_sic_pdb);

        if ( ICF_SIC_PENDING_CLEAR_OPTIONS_CTX == 
                (p_sic_pdb->p_sic_ctx->pending_event &
                        ICF_SIC_PENDING_CLEAR_OPTIONS_CTX))
        {
                icf_sic_dealloc_context(p_sic_pdb);
                return ICF_SUCCESS;
        }

	if(ICF_NULL != p_sic_pdb->p_sic_ctx->p_call_ctx && 
            p_sic_pdb->p_sic_ctx->p_call_ctx->common_bitmask & ICF_IN_DLG_OPTIONS_RECV)
    {
        /* reset the bitmask for in-coming IN-DLG OPTIONS*/
        p_sic_pdb->p_sic_ctx->p_call_ctx->common_bitmask &= ~ICF_IN_DLG_OPTIONS_RECV;
        
    }

    p_options_cfm = (icf_sic_options_cfm_st*)
			(p_sic_pdb->p_msg);
	if ( ICF_FAILURE == p_options_cfm->result)
	{
        if(ICF_SIC_OPTIONS_FAILURE_RESP_PRESENT & 
                                            p_options_cfm->bitmask)
        {
              if(400 <= p_options_cfm->response_code)
              {
                  p_sic_pdb->resp_code = p_options_cfm->response_code;     
              }
              else
              {
                  p_sic_pdb->resp_code = ICF_NULL;
              }
        }
        else
        {
            p_sic_pdb->resp_code = ICF_NULL;
        }
		icf_sic_send_failure_response_to_ssa(p_sic_pdb);
		ICF_FUNCTION_EXIT(p_sic_pdb->p_glb_pdb)
		return ret_val;
	}

     /* clone the header list into sic ctx, if received from app */
        if (ICF_NULL != p_options_cfm->header_list.count)
        {
            ret_val = icf_cmn_clone_header_list(
                                    p_sic_pdb->p_glb_pdb,
                                    &(p_options_cfm->header_list),
                                    &(p_sic_pdb->p_sic_ctx->header_list),
                                    p_sic_pdb->p_ecode);
        }

     /* clone the tag list into sic ctx, if received from app */
        if (ICF_NULL != p_options_cfm->tag_list.count)
        {
            ret_val = icf_cmn_clone_tag_list(
                                    p_sic_pdb->p_glb_pdb,
                                    &(p_options_cfm->tag_list),
                                    &(p_sic_pdb->p_sic_ctx->tag_list),
                                    p_sic_pdb->p_ecode);
        }

     /* clone the body list into sic ctx, if received from app */
        if (ICF_NULL != p_options_cfm->body_list.count)
        {
            ret_val = icf_cmn_clone_body_list(
                                    p_sic_pdb->p_glb_pdb,
                                    &(p_options_cfm->body_list),
                                    &(p_sic_pdb->p_sic_ctx->body_list),
                                    p_sic_pdb->p_ecode);
        }

ICF_CHECK_TUNNEL_MODE_START(p_sic_pdb->p_glb_pdb,ICF_FALSE) 
    icf_sic_form_final_capabilities(p_sic_pdb,&p_sdp_list);

    icf_sic_send_options_success_resp_to_ssa(p_sic_pdb,p_sdp_list);
	
    icf_sic_free_final_capabilities(p_sic_pdb,&p_sdp_list);
ICF_CHECK_TUNNEL_MODE_END
ICF_CHECK_TUNNEL_MODE_START(p_sic_pdb->p_glb_pdb,ICF_TRUE)
    /* Rel 8.1
     * If Tunneled_media_capab_resp received with SDP 
     * then it gets copied into sic_ctx
     * now this SDP is to be sent to SSA
     * to send in 200 OK
     */
    p_sdp_list = p_sic_pdb->p_sic_ctx->p_sdp_list;
    icf_sic_send_options_success_resp_to_ssa(p_sic_pdb,p_sdp_list);
ICF_CHECK_TUNNEL_MODE_END


    ICF_FUNCTION_EXIT(p_sic_pdb->p_glb_pdb)

    return ret_val;
} /* End of icf_sic_02_options_app_cfm */

/*******************************************************************************
 *
 * FUNCTION:            icf_sic_03_options_resp
 *
 * DESCRIPTION:         This Function will be invoked when an SIP OPTIONS
 *                      Request is received from network.
 *
 * CURRENT_STATE:       ICF_SIC_03_AWT_OPTIONS_NW_CFM
 *
 * CURRENT_EVENT:       ICF_SIC_EVENT_OPTIONS_NW_RSP
 *
 * NEXT_STATES:
 *                      1) ICF_SIC_00_OPTIONS_IDLE
 *
 ******************************************************************************/
 icf_return_t icf_sic_03_options_resp(
                icf_sic_pdb_st  *p_sic_pdb)
{

	icf_return_t ret_val = ICF_SUCCESS;
	
	ICF_FUNCTION_ENTER(p_sic_pdb->p_glb_pdb)

	icf_sic_process_options_resp(p_sic_pdb);

	ICF_FUNCTION_EXIT(p_sic_pdb->p_glb_pdb)

	return ret_val;

} /* End of icf_sic_03_options_resp */

/*******************************************************************************
 *
 * FUNCTION:            icf_sic_xx_unexpected_options_event
 *
 * DESCRIPTION:         This Function will be invoked when an SIP OPTIONS
 *                      Request is received from network.
 *
 * CURRENT_STATE:       Any State
 *
 * CURRENT_EVENT:       Unexpected event
 *
 * NEXT_STATES:
 *                      NONE
 *
 ******************************************************************************/
 icf_return_t icf_sic_xx_unexpected_options_event(
                icf_sic_pdb_st  *p_sic_pdb)
{

	icf_return_t ret_val = ICF_SUCCESS;
	
	ICF_FUNCTION_ENTER(p_sic_pdb->p_glb_pdb)

	switch( p_sic_pdb->event)
	{
		case ICF_SIC_EVENT_OTG_OPTIONS_REQ:
			icf_sic_process_invalid_options_req(p_sic_pdb);
			break;	
		case ICF_SIC_EVENT_INC_OPTIONS_REQ:
			icf_sic_send_failure_response_for_unxepected_inc_options(p_sic_pdb);
			break;
	}

	ICF_FUNCTION_EXIT(p_sic_pdb->p_glb_pdb)

	return ret_val;
} /* End of icf_sic_xx_unexpected_options_event */

/* ICF_6_1_OPTIONS_SUPPORT_END */



/*******************************************************************************
 *
 * FUNCTION:            icf_sic_04_early_session_cfm
 *
 * DESCRIPTION:         This Function will be invoked when OUt of
 *                      dialog OPTIONS received from network 
 *                      ICF_EARLY_SESSION_IND sent to application
 *                      and Application responded with 
 *                      ICF_EARLY_SESSION_CFM
 *
 * CURRENT_STATE:       ICF_SIC_04_AWT_EARLY_SESSION_CFM
 *
 * CURRENT_EVENT:       ICF_SIC_EVENT_EARLY_SESSION_CFM
 *
 * NEXT_STATES:
 *                      1) ICF_SIC_01_AWT_MEDIA_CAPAB_RSP
 *                      2) ICF_SIC_02_AWT_OPTIONS_APP_CFM
 *
 ******************************************************************************/
 icf_return_t icf_sic_04_early_session_cfm(
                icf_sic_pdb_st  *p_sic_pdb)
{
    icf_return_t             ret_val = ICF_SUCCESS;
    /* The variable that will contain the data received from
     * MRM 
     */
    icf_sic_early_session_cfm_st *p_early_session_cfm = ICF_NULL;
    /*  This is the list that would be paased to the
     *  utility function to send media_capab_req
     */   
    icf_list_st  *p_app_media_capab     = ICF_NULL;   
    icf_app_conf_st *p_app_conf = ICF_NULL;

    ICF_FUNCTION_ENTER(p_sic_pdb->p_glb_pdb)

    p_early_session_cfm = (icf_sic_early_session_cfm_st*)
                  (p_sic_pdb->p_msg);
    /*  If result is failure then send failure response of
        OPTIONS */
    if(ICF_NULL != p_early_session_cfm)
    {
        ICF_SIC_APP_TRACE(p_sic_pdb, ICF_TRACE_EARLY_SESSION_CFM)
        if ( ICF_FAILURE == p_early_session_cfm->result)
        {
            /*  response code is filled by application then
             *  copy that resposne code in sic_pdb
             */
            if(ICF_SIC_EARLY_SESSION_RESPONSE_CODE_PRESENT & 
                              p_early_session_cfm->bit_mask)
            {
                p_sic_pdb->resp_code = p_early_session_cfm->response_code;     
            }
            else
            {
                p_sic_pdb->resp_code = ICF_NULL;
            }
            icf_sic_stop_options_timer(p_sic_pdb);
            icf_sic_send_failure_response_to_ssa(p_sic_pdb);
            ICF_FUNCTION_EXIT(p_sic_pdb->p_glb_pdb)
            return ret_val;
         }
         else
         {	
             /*  If tunnel mode is received then  copy it in sic_ctx */
             if (ICF_NULL != (p_early_session_cfm->bit_mask &
                          ICF_SIC_EARLY_SESSION_TUNNEL_MODE_OPERATION_PRESENT))
             {
                 p_sic_pdb->p_sic_ctx->tunnel_mode = 
                        p_early_session_cfm->tunnel_mode;
             }
             else
             {
                 p_sic_pdb->p_sic_ctx->tunnel_mode =
                                       ICF_FALSE;
             }
    
             if ( ICF_TRUE == p_sic_pdb->p_sic_ctx->is_sdp_reqd)
             {
                 /*  If tunnel mode is true the send options_ind 
                  *  first then medi_capab_req
                  */
ICF_CHECK_TUNNEL_MODE_START(p_sic_pdb->p_glb_pdb,ICF_TRUE)
                     ret_val = icf_sic_send_options_ind_to_app(p_sic_pdb);
                     if ( ICF_FAILURE == ret_val)
                     {
                         icf_sic_stop_options_timer(p_sic_pdb);
                         icf_sic_send_failure_response_to_ssa(p_sic_pdb);
                         ICF_FUNCTION_EXIT(p_sic_pdb->p_glb_pdb)
                         return ret_val;
                     }
    
ICF_CHECK_TUNNEL_MODE_END
ICF_CHECK_TUNNEL_MODE_START(p_sic_pdb->p_glb_pdb,ICF_FALSE)
/*  Rel 8.1
 *  If non tunnel mode then fill the media_capab
 *  to send the icf_media_capabilities_req
 */
                 if(ICF_SUCCESS ==
                    icf_cfg_fetch_app_conf(p_sic_pdb->p_glb_pdb,p_sic_pdb->p_sic_ctx->app_id,
                    &p_app_conf,
                    p_sic_pdb->p_ecode))
                 {
                     if((ICF_NULL != p_app_conf) &&
                               (ICF_TRUE == p_app_conf->is_valid))
                     {
                         if ( ICF_NULL !=
                              p_app_conf->p_app_media_profile)
                         {
                             icf_sic_map_icf_internal_list_icf_list(
                                p_sic_pdb,
                                p_app_conf->p_app_media_profile,
                                &p_app_media_capab);
                         }
                         else
                            p_app_media_capab = ICF_NULL;
                     }
                 }
ICF_CHECK_TUNNEL_MODE_END

                 /*  Send ICF_MEDIA_CAPABILITIES_REQ */
                 ret_val =
                 icf_sic_send_media_capab_req_to_mmi(p_sic_pdb,p_app_media_capab);
                 if(ICF_NULL != p_app_media_capab)
                 {
                     icf_cmn_free_icf_stream_capab_list(
                       p_sic_pdb->p_glb_pdb,
                       &p_app_media_capab);
                 }
                 ICF_SIC_SET_STATE(p_sic_pdb,ICF_SIC_01_AWT_MEDIA_CAPAB_RSP)
                 /*  Now this bit is not required anymore
                  *  so resetting this bit
                  */
                 p_sic_pdb->p_sic_ctx->is_sdp_reqd = ICF_FALSE;
             }
             else
             {
                 /*  If SDP is not required in 200OK then send OPTIONS IND.*/
                 ret_val = icf_sic_send_options_ind_to_app(p_sic_pdb);
                 if ( ICF_FAILURE == ret_val)
                 {
                     icf_sic_stop_options_timer(p_sic_pdb);
                     icf_sic_send_failure_response_to_ssa(p_sic_pdb);
                     ICF_FUNCTION_EXIT(p_sic_pdb->p_glb_pdb)
                     return ret_val;
                 }
                 ICF_SIC_SET_STATE(p_sic_pdb,ICF_SIC_02_AWT_OPTIONS_APP_CFM)
             }
        }
    }
    /*  If recev message is NULL */
    else
    {
        ret_val = ICF_FAILURE;
    }
    ICF_FUNCTION_EXIT(p_sic_pdb->p_glb_pdb)
    return ret_val;
}


/* Added for Asynchronous NOTIFY support */
/*******************************************************************************
 * FUNCTION:        icf_sic_05_notify_cfm
 * 
 * DESCRIPTION:     This function is called when - 
 *                  State - ICF_SIC_05_AWAIT_NOTIFY_CFM
 *                  Event - ICF_SIC_EVENT_NOTIFY_CFM
 *                  
 ******************************************************************************/
icf_return_t icf_sic_05_notify_cfm(
        INOUT icf_sic_pdb_st      *p_sic_pdb)
{
    icf_return_t              ret_val = ICF_SUCCESS,
                              return_val = ICF_SUCCESS;
	icf_sic_notify_cfm_st     *p_notify_cfm = ICF_NULL;

    ICF_FUNCTION_ENTER(p_sic_pdb->p_glb_pdb)
   
    /* Extract API payload as received from application */
    p_notify_cfm = (icf_sic_notify_cfm_st *)(p_sic_pdb->p_msg);

    /* If p_notify_cfm is NULL, return without further processing */
    ICF_CHECK_IF_VAR_IS_NULL(p_sic_pdb->p_glb_pdb,
                             p_notify_cfm,
                             return_val)
    if(ICF_FAILURE == return_val)
    {
        ret_val = ICF_FAILURE;
        ICF_FUNCTION_EXIT(p_sic_pdb->p_glb_pdb)
        return ret_val;
    }

    /* Find the SIC context on the basis of the Call Id */
  	ret_val = icf_dbm_find_sic_cntxt_blk(
                  p_sic_pdb->p_glb_pdb,
                  p_notify_cfm->call_id,
                  &(p_sic_pdb->p_sic_ctx),
                  p_sic_pdb->p_ecode);

    /* If the SIC context is NULL, return without further processing */
    ICF_CHECK_IF_VAR_IS_NULL(p_sic_pdb->p_glb_pdb,
                             p_sic_pdb->p_sic_ctx,
                             return_val)
    if(ICF_FAILURE == return_val)
    {
        ret_val = ICF_FAILURE;
        ICF_FUNCTION_EXIT(p_sic_pdb->p_glb_pdb)
        return ret_val;
    }

    /* Stop notify cfm timer */
    icf_sic_stop_notify_cfm_timer(p_sic_pdb);

    /* 
       Fill notify_resp and error_cause in SIC PDB for application
       trace data.The field p_sic_pdb->notify_resp is also used by
       the function icf_sic_send_notify_resp.
    */
    p_sic_pdb->notify_resp = p_notify_cfm->result;
    p_sic_pdb->error_cause = p_notify_cfm->error_code;
 
    /* Issue notify cfm trace to application */
    ICF_SIC_APP_TRACE(p_sic_pdb,ICF_TRACE_NOTIFY_CFM)

    /* 
       Fill resp_code in SIC PDB.These will be used by function
       icf_sic_send_notify_resp to form an internal message and
       trigger SSA to send NOTIFY response.The resp_code will be
       populated according to the following logic:
    
           if result is success
               set response code as 200
           else if response code is present
               set response code as received in API
           else if error_code is present
               set response code on the basis of error code
           else
               set response code as 500
    */
    if(ICF_SUCCESS == p_notify_cfm->result)
	{
		p_sic_pdb->resp_code = 200;
	}
	else if(ICF_NULL != p_notify_cfm->response_code)
	{
		p_sic_pdb->resp_code = p_notify_cfm->response_code;
	}
	else if(ICF_NULL != p_notify_cfm->error_code)
	{
		if(ICF_CAUSE_INTERNAL_FAILURE == p_notify_cfm->error_code)
		{
			p_sic_pdb->resp_code = 500;
		}
		else if(ICF_CAUSE_MIME_BODY_DECODE_FAILURE == p_notify_cfm->error_code)
		{
			p_sic_pdb->resp_code = 400;
		}
		else
		{
			p_sic_pdb->resp_code = 500;
		}
	}
	else
	{
        /* 
           Result is ICF_FAILURE, response_code and error_code not present,
           fill default response_code.
        */   
		p_sic_pdb->resp_code = 500;
	}    

    /* Populate header, tag and body list in SIC context */
    if(ICF_NULL != p_notify_cfm->header_list.count)
    {
        /* Free any existing header list in SIC context */
        icf_cmn_free_header_list(
                    p_sic_pdb->p_glb_pdb,
                    &(p_sic_pdb->p_sic_ctx->header_list),
                    p_sic_pdb->p_ecode);
        
        icf_cmn_clone_header_list(
                    p_sic_pdb->p_glb_pdb,
                    &(p_notify_cfm->header_list),
                    &(p_sic_pdb->p_sic_ctx->header_list),
                    p_sic_pdb->p_ecode);
    }    
                
    if(ICF_NULL != p_notify_cfm->tag_list.count)
    {
        /* Free any existing tag list in SIC context */
        icf_cmn_free_tag_list(
                    p_sic_pdb->p_glb_pdb,
                    &(p_sic_pdb->p_sic_ctx->tag_list),
                    p_sic_pdb->p_ecode);
        
        icf_cmn_clone_tag_list(
                    p_sic_pdb->p_glb_pdb,
                    &(p_notify_cfm->tag_list),
                    &(p_sic_pdb->p_sic_ctx->tag_list),
                    p_sic_pdb->p_ecode);
    }
        
    if(ICF_NULL != p_notify_cfm->body_list.count)
    {
        /* Free any body header list in SIC context */
        icf_cmn_free_msg_body_list(p_sic_pdb->p_glb_pdb,
                    &(p_sic_pdb->p_sic_ctx->body_list),
                    p_sic_pdb->p_ecode);
        
        icf_cmn_clone_body_list(
                    p_sic_pdb->p_glb_pdb,
                    &(p_notify_cfm->body_list),
                    &(p_sic_pdb->p_sic_ctx->body_list),
                    p_sic_pdb->p_ecode);
    }

    /* Send the NOTIFY response */
    icf_sic_send_notify_resp(p_sic_pdb);

    /* After responding to the NOTIFY,process pending events */
    ret_val = icf_sic_post_notify_resp_processing(p_sic_pdb);

    ICF_FUNCTION_EXIT(p_sic_pdb->p_glb_pdb)
    return ret_val;
}

/*******************************************************************************
 * FUNCTION:        icf_sic_05_timer_expiry
 * 
 * DESCRIPTION:     This function is called when - 
 *                  State - ICF_SIC_05_AWAIT_NOTIFY_CFM
 *                  Event - ICF_SIC_EVENT_NOTIFY_CFM_TIMER_EXP
 *                  
 ******************************************************************************/
icf_return_t icf_sic_05_timer_expiry(
        INOUT icf_sic_pdb_st      *p_sic_pdb)
{
    icf_return_t ret_val = ICF_SUCCESS;

    ICF_FUNCTION_ENTER(p_sic_pdb->p_glb_pdb)
    
    /* Issue notify cfm timer expiry trace to application */
    ICF_SIC_APP_TRACE(p_sic_pdb,ICF_TRACE_NOTIFY_CFM_TIMER_EXPIRE)

    /* 
       Fill notify_resp and resp_code in SIC PDB.These will be used by
       function icf_sic_send_notify_resp to form an internal message and
       trigger SSA to send NOTIFY response.
    */
    p_sic_pdb->notify_resp = ICF_FAILURE;

    /* On timer expiry, the NOTIFY will be responded to with a 500 response */
    p_sic_pdb->resp_code = 500;

    icf_sic_send_notify_resp(p_sic_pdb);

    /* After responding to the NOTIFY,process pending events */
    ret_val = icf_sic_post_notify_resp_processing(p_sic_pdb);

    ICF_FUNCTION_EXIT(p_sic_pdb->p_glb_pdb)
    return ret_val;
}

/*******************************************************************************
 * FUNCTION:        icf_sic_05_subs_req
 * 
 * DESCRIPTION:     This function is called when - 
 *                  State - ICF_SIC_05_AWAIT_NOTIFY_CFM
 *                  Event - ICF_SIC_EVENT_SUBS_REQ 
 *                  
 ******************************************************************************/
icf_return_t icf_sic_05_subs_req(
        INOUT icf_sic_pdb_st      *p_sic_pdb)
{
    icf_return_t ret_val = ICF_SUCCESS;

    ICF_FUNCTION_EXIT(p_sic_pdb->p_glb_pdb)

    /* 
       Set pending event as ICF_SIC_EVENT_SUBS_REQ. This will be
       processed in 05 state on receiving notify cfm from application
       or on timer expiry. 
    */
    ICF_SIC_SET_PENDING_EVENT(p_sic_pdb,ICF_SIC_EVENT_SUBS_REQ)

    ICF_FUNCTION_EXIT(p_sic_pdb->p_glb_pdb)
    return ret_val;
}

/*******************************************************************************
 * FUNCTION:        icf_sic_05_unsub_req
 * 
 * DESCRIPTION:     This function is called when - 
 *                  State - ICF_SIC_05_AWAIT_NOTIFY_CFM
 *                  Event - ICF_SIC_EVENT_UNSUB_REQ 
 *                  
 ******************************************************************************/
icf_return_t icf_sic_05_unsub_req(
        INOUT icf_sic_pdb_st      *p_sic_pdb)
{
    icf_return_t ret_val = ICF_SUCCESS;

    ICF_FUNCTION_EXIT(p_sic_pdb->p_glb_pdb)

    /* 
       Set pending event as ICF_SIC_EVENT_UNSUB_REQ. This will be
       processed in 05 state on receiving notify cfm from application
       or on timer expiry. 
    */
    ICF_SIC_SET_PENDING_EVENT(p_sic_pdb,ICF_SIC_EVENT_UNSUB_REQ)

    ICF_FUNCTION_EXIT(p_sic_pdb->p_glb_pdb)
    return ret_val;
}

/*******************************************************************************
 * FUNCTION:        icf_sic_05_success_resp
 * 
 * DESCRIPTION:     This function is called when - 
 *                  State - ICF_SIC_05_AWAIT_NOTIFY_CFM
 *                  Event - ICF_SIC_EVENT_SUCCESS_RESP 
 *                  
 ******************************************************************************/
icf_return_t icf_sic_05_success_resp(
        INOUT icf_sic_pdb_st      *p_sic_pdb)
{
    icf_return_t ret_val = ICF_SUCCESS;

    ICF_FUNCTION_EXIT(p_sic_pdb->p_glb_pdb)

    /* 
       Set pending event as ICF_SIC_EVENT_SUCCESS_RESP. This will be
       processed in 05 state on receiving notify cfm from application
       or on timer expiry. 
    */
    ICF_SIC_SET_PENDING_EVENT(p_sic_pdb,ICF_SIC_EVENT_SUCCESS_RESP)

    ICF_FUNCTION_EXIT(p_sic_pdb->p_glb_pdb)
    return ret_val;
}

/*******************************************************************************
 * FUNCTION:        icf_sic_05_failure_resp_no_retry
 * 
 * DESCRIPTION:     This function is called when - 
 *                  State - ICF_SIC_05_AWAIT_NOTIFY_CFM
 *                  Event - ICF_SIC_EVENT_FAILURE_RESP_NO_RETRY 
 *                  
 ******************************************************************************/
icf_return_t icf_sic_05_failure_resp_no_retry(
        INOUT icf_sic_pdb_st      *p_sic_pdb)
{
    icf_return_t ret_val = ICF_SUCCESS;

    ICF_FUNCTION_EXIT(p_sic_pdb->p_glb_pdb)

    /* 
       Set pending event as ICF_SIC_EVENT_FAILURE_RESP_NO_RETRY. This will be
       processed in 05 state on receiving notify cfm from application
       or on timer expiry. 
    */
    ICF_SIC_SET_PENDING_EVENT(p_sic_pdb,ICF_SIC_EVENT_FAILURE_RESP_NO_RETRY)

    ICF_FUNCTION_EXIT(p_sic_pdb->p_glb_pdb)
    return ret_val;
}

/*******************************************************************************
 * FUNCTION:        icf_sic_05_failure_resp_with_retry
 * 
 * DESCRIPTION:     This function is called when - 
 *                  State - ICF_SIC_05_AWAIT_NOTIFY_CFM
 *                  Event - ICF_SIC_EVENT_FAILURE_RESP_WITH_RETRY 
 *                  
 ******************************************************************************/
icf_return_t icf_sic_05_failure_resp_with_retry(
        INOUT icf_sic_pdb_st      *p_sic_pdb)
{
    icf_return_t ret_val = ICF_SUCCESS;

    ICF_FUNCTION_EXIT(p_sic_pdb->p_glb_pdb)

    /* 
       Set pending event as ICF_SIC_EVENT_FAILURE_RESP_WITH_RETRY. This will be
       processed in 05 state on receiving notify cfm from application
       or on timer expiry. 
    */
    ICF_SIC_SET_PENDING_EVENT(p_sic_pdb,ICF_SIC_EVENT_FAILURE_RESP_WITH_RETRY)

    ICF_FUNCTION_EXIT(p_sic_pdb->p_glb_pdb)
    return ret_val;    
}

/*******************************************************************************
 * FUNCTION:        icf_sic_05_duration_timer_exp
 * 
 * DESCRIPTION:     This function is called when - 
 *                  State - ICF_SIC_05_AWAIT_NOTIFY_CFM
 *                  Event - ICF_SIC_EVENT_DURATION_TIMER_EXP 
 *                  
 ******************************************************************************/
icf_return_t icf_sic_05_duration_timer_exp(
        INOUT icf_sic_pdb_st      *p_sic_pdb)
{
    icf_return_t ret_val = ICF_SUCCESS;

    ICF_FUNCTION_EXIT(p_sic_pdb->p_glb_pdb)

    /* 
       Set pending event as ICF_SIC_EVENT_DURATION_TIMER_EXP. This will be
       processed in 05 state on receiving notify cfm from application
       or on timer expiry. 
    */
    ICF_SIC_SET_PENDING_EVENT(p_sic_pdb,ICF_SIC_EVENT_DURATION_TIMER_EXP)

    ICF_FUNCTION_EXIT(p_sic_pdb->p_glb_pdb)
    return ret_val;    
}        

/*******************************************************************************
 * FUNCTION:        icf_sic_05_retry_timer_exp
 * 
 * DESCRIPTION:     This function is called when - 
 *                  State - ICF_SIC_05_AWAIT_NOTIFY_CFM
 *                  Event - ICF_SIC_EVENT_RETRY_TIMER_EXP 
 *                  
 ******************************************************************************/
icf_return_t icf_sic_05_retry_timer_exp(
        INOUT icf_sic_pdb_st      *p_sic_pdb)
{
    icf_return_t ret_val = ICF_SUCCESS;

    ICF_FUNCTION_EXIT(p_sic_pdb->p_glb_pdb)

    /* 
       Set pending event as ICF_SIC_EVENT_RETRY_TIMER_EXP. This will be
       processed in 05 state on receiving notify cfm from application
       or on timer expiry. 
    */
    ICF_SIC_SET_PENDING_EVENT(p_sic_pdb,ICF_SIC_EVENT_RETRY_TIMER_EXP)

    ICF_FUNCTION_EXIT(p_sic_pdb->p_glb_pdb)
    return ret_val;    
}

/* New Code for ASYNCHRONUS_MESSAGE enhancements*/

/*******************************************************************************
 *
 * FUNCTION:            icf_sic_00_inc_message_req
 *
 * DESCRIPTION:         This Function will be invoked when an SIP MESSAGE
 *                      Request is received from network.
 *
 * CURRENT_STATE:       ICF_SIC_00_MESSAGE_IDLE
 *
 * CURRENT_EVENT:       ICF_SIC_EVENT_INC_MESSAGE_REQ
 *     
 * NEXT_STATES:
 *                      1) ICF_SIC_01_AWAIT_MESSAGE_CFM
 *
 ******************************************************************************/
icf_return_t icf_sic_00_inc_message_req(
                icf_sic_pdb_st  *p_sic_pdb)
{
    /* 
       Set Next State as ICF_SIC_01_AWAIT_MESSAGE_CFM and Start the Message confirm Timer.
     */

    icf_return_t ret_val = ICF_SUCCESS;

    ICF_FUNCTION_ENTER(p_sic_pdb->p_glb_pdb)

    icf_sic_start_message_timer(p_sic_pdb);

    ret_val = icf_sic_process_new_message_ind(p_sic_pdb);
    if ( ICF_FAILURE == ret_val)
    {
        icf_sic_stop_message_timer(p_sic_pdb);
        p_sic_pdb->message_result = ICF_FAILURE;
        p_sic_pdb->resp_code = 500;
        icf_sic_send_message_resp(p_sic_pdb);
        /* The following processing will be done to deallocate the SIC context 
         *if sic context is not NULL*/
        if (ICF_NULL != p_sic_pdb->p_sic_ctx )
        {
            /*dealloc sic ctx and delink*/
            icf_sic_dealloc_context(p_sic_pdb);    
        }
        ICF_FUNCTION_EXIT(p_sic_pdb->p_glb_pdb)
            return ret_val;
    }
    ICF_SIC_SET_STATE(p_sic_pdb,ICF_SIC_01_AWAIT_MESSAGE_CFM)


    ICF_FUNCTION_EXIT(p_sic_pdb->p_glb_pdb)

    return ret_val;

} /* End of icf_sic_00_inc_message_req */

/*******************************************************************************
 *
 * FUNCTION:            icf_sic_01_msg_cfm_timer_exp
 *
 * DESCRIPTION:         This Function will be invoked when Message timer
 *                      expiry event is received
 *
 * CURRENT_STATE:       ICF_SIC_01_AWAIT_MESSAGE_CFM
 *
 * CURRENT_EVENT:       ICF_SIC_EVENT_AWT_MESSAGE_CFM_TIMER_EXP
 *
 * NEXT_STATES:
 *                      1) ICF_SIC_00_MESSAGE_IDLE
 *
 ******************************************************************************/
icf_return_t icf_sic_01_msg_cfm_timer_exp(
                icf_sic_pdb_st  *p_sic_pdb)
{
    /* 
       Fill message_result and resp_code in SIC PDB.These will be used by
       function icf_sic_send_message_resp to form an internal message and
       trigger SSA to send MESSAGE response.
     */

    icf_return_t ret_val = ICF_SUCCESS;

    ICF_FUNCTION_ENTER(p_sic_pdb->p_glb_pdb)

    /* Issue message cfm timer expiry trace to application */
    ICF_SIC_APP_TRACE(p_sic_pdb,ICF_TRACE_MESSAGE_CFM_TIMER_EXPIRE)

    p_sic_pdb->message_result = ICF_FAILURE;

    /* On timer expiry, the MESSAGE will be responded to with a 500 response */
    p_sic_pdb->resp_code = 500;

    icf_sic_send_message_resp(p_sic_pdb);
    /* ICF_SIC_MESSAGE_REQ_ONGOING reset in icf_sic_message_resp*/
    /* The following processing will be done to deallocate the SIC context 
     *if sic context is not NULL*/
    if (ICF_NULL != p_sic_pdb->p_sic_ctx )
    {
        /*dealloc sic ctx and delink*/
        icf_sic_dealloc_context(p_sic_pdb);    
    }

    ICF_FUNCTION_EXIT(p_sic_pdb->p_glb_pdb)
    return ret_val;
} /* End of icf_sic_01_msg_cfm_timer_exp */


/*******************************************************************************
 *
 * FUNCTION:            icf_sic_01_message_cfm
 *
 * DESCRIPTION:         This Function will be invoked when application sends
 *                      icf_message_cfm_st API to ICF.
 *
 * CURRENT_STATE:       ICF_SIC_01_AWAIT_MESSAGE_CFM
 *
 * CURRENT_EVENT:       ICF_SIC_EVENT_MESSAGE_APP_CFM
 *
 * NEXT_STATES:
 *                      1) ICF_SIC_00_MESSAGE_IDLE
 *
 ******************************************************************************/
 icf_return_t icf_sic_01_message_cfm(
                icf_sic_pdb_st  *p_sic_pdb)
{
    /* 
       Send the MESSAGE response on network  and Stop the Message confirm Timer.
     */

    icf_return_t             ret_val = ICF_SUCCESS,
                              return_val = ICF_SUCCESS;
    icf_sic_message_cfm_st *p_message_cfm = ICF_NULL;

    ICF_FUNCTION_ENTER(p_sic_pdb->p_glb_pdb)

    icf_sic_stop_message_timer(p_sic_pdb);

    /* Extract API payload as received from application */
    p_message_cfm = (icf_sic_message_cfm_st*) (p_sic_pdb->p_msg);

    /* If p_message_cfm is NULL, return without further processing */
    ICF_CHECK_IF_VAR_IS_NULL(p_sic_pdb->p_glb_pdb,
                             p_message_cfm,
                             return_val)
    if(ICF_FAILURE == return_val)
    {
        ret_val = ICF_FAILURE;
        ICF_FUNCTION_EXIT(p_sic_pdb->p_glb_pdb)
        return ret_val;
    }

    /* 
       Fill message_result and resp_code in SIC PDB for application
       trace data.
     */

    if ( ICF_FAILURE == p_message_cfm->result)
    {
        if(ICF_SIC_APP_CONFIRMATION_RESP_CODE_PRESENT == (ICF_SIC_APP_CONFIRMATION_RESP_CODE_PRESENT & p_message_cfm->bitmask))
        {
            p_sic_pdb->resp_code = p_message_cfm->response_code;     
        }
        else
        {
            p_sic_pdb->resp_code = 500;
        }
    }
    else 
    {
        p_sic_pdb->resp_code = 200;
    }

    /* clone the header list into sic ctx, if received from app */
    if (ICF_NULL != p_message_cfm->header_list.count)
    {
        /* Free any existing header list in SIC context */
        icf_cmn_free_header_list(
                p_sic_pdb->p_glb_pdb,
                &(p_sic_pdb->p_sic_ctx->header_list),
                p_sic_pdb->p_ecode);

        ret_val = icf_cmn_clone_header_list(
                p_sic_pdb->p_glb_pdb,
                &(p_message_cfm->header_list),
                &(p_sic_pdb->p_sic_ctx->header_list),
                p_sic_pdb->p_ecode);
    }

    /* clone the tag list into sic ctx, if received from app */
    if (ICF_NULL != p_message_cfm->tag_list.count)
    {
        /* Free any existing tag list in SIC context */
        icf_cmn_free_tag_list(
                p_sic_pdb->p_glb_pdb,
                &(p_sic_pdb->p_sic_ctx->tag_list),
                p_sic_pdb->p_ecode);
        ret_val = icf_cmn_clone_tag_list(
                p_sic_pdb->p_glb_pdb,
                &(p_message_cfm->tag_list),
                &(p_sic_pdb->p_sic_ctx->tag_list),
                p_sic_pdb->p_ecode);
    }

    /* This will be used by function icf_sic_send_message_resp */
    p_sic_pdb->message_result = p_message_cfm->result;  /*result as received in the API*/
    ICF_SIC_APP_TRACE(p_sic_pdb,ICF_TRACE_MESSAGE_CFM)

    /*Form an internal message and trigger SSA by calling utility function icf_sic_send_message_resp*/
    icf_sic_send_message_resp(p_sic_pdb);

    /* The following processing will be done to deallocate the SIC context 
     *if sic context is not NULL*/
    if (ICF_NULL != p_sic_pdb->p_sic_ctx )
    {
        /*dealloc sic ctx and delink*/
        icf_sic_dealloc_context(p_sic_pdb);    
    }

    ICF_FUNCTION_EXIT(p_sic_pdb->p_glb_pdb)

    return ret_val;
}
