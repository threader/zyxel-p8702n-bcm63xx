/******************************************************************************
 *
 * FILE NAME:   icf_sic_entry_point.c
 *
 * DESCRIPTION: This file contains the entry point function and the utitlity
 *              function called by the entry point function to call the API
 *              handlers.
 *
 * REVISION HISTORY:
 *          Name            Date            Reference           Comments
 *  Syed Ali Rizvi    11-Aug-2005             LLD         Initial
 *
 *  Jyoti Narula      31-Jan-2006     SPR 9079 Fix      Handler for forceful
 *                                                      clearing of all SIC
 *                                                      contexts.
 *  Puneet Sharma     02-Apr-2006	  ICF5.0 LLD	 changes done in the comments 
 *											 Multiple Public Users ICF5.0
 * Abhishek Dhammawat 15-FEB-2006     SPR14531 icf_sic_validate_event modified
 * Manoneet Khosla    10-Aug-2007	SPR # 15721 Added method field in icf_sic_ctx_st icf_sic_process_mesg
 * Abhishek Dhammawat 10-Sept-2007   SPR16315  Modified icf_sic_validate_event 
 *                                            ICF_SIC_INTERNAL_SUBSCRIBE_REQUEST
 * 26-Jan-2008  Amit Mahajan       ICF Rel 8.0  Changes for reg event package support
 * 28-Mar-2008  Tarun Gupta      SPR 18311      Merged Publish-Message Enhancements
 *
 * 28-Mar-2008  Rajiv Kumar        SPR 18292    Clearing of sic context for
 *                                              reg event subs context. 
 * 02-Apr-2008  Tarun Gupta      SPR 18311      Klocworks Warnings Removal                                              
 *
 * 2-Apr-2008  Sumant Gupta        SPR 18311   Merged Publish-Message Enhancement
 * 08-Apr-2008 Tarun Gupta       SPR 18355     Merged Windows Porting Changes
 * 17-Apr-2008 Tarun Gupta       SPR 18446     Changes done for Publish Message in  *                                             IMS mode 
 * 22-Apr-2008  Alok Tiwari      SPR 18455     Klocwork warning removal
 * 28-Apr-2008  Tarun Gupta      SPR 18489     Modified icf_sic_process_options_req
 * 03-May-2008 Abhishek Dhammawat SPR18430     Modified icf_sic_map_api_to_event
 * 08-May-2008  Rajiv Kumar       SPR 18517    Changes in
 *                                             icf_sic_process_notify_cfm and
 *                                             icf_sic_process_notify_out_of_dialog
 *                                             for handling out of dialog
 *                                             NOTIFY in IMS MODE
 * 11-May-2008  Rajiv Kumar       SPR17506     Changes in function 
 *                                             icf_sic_process_refer_req:Code
 *                                             removed because it sends two
 *                                             REFER over Network.
 * 17-May-2008  Amit Sharma      SPR:18577     Assigning line_id in p_glb_pdb
 * 27-Jun-2008  Amit Sharma      SPR:18706     Handling for CLEAR_SIC_CONTEXT
 * 04-Jul-2008  Tarun Gupta      SPR 18751     Added handling for SIPS preferred identity
 * 20-Nov-2008  Rajiv Kumar      SPR 19188     Rel 7.1.2 SPR merging (SPR
 *                                             19161)  
 * 24-Nov-2008  Rajiv Kumar      SPR 19215     Fix Merged For CSR-1-6216029
 * 25-Nov-2008  Rajiv Kumar      SPR 19188     Rel 7.1.2 SPR merging (SPR
 *                                             18889)  
 * 25-Nov-2008  Rajiv Kumar      SPR 19188     Rel 7.1.2 SPR merging (SPR
 *                                             18927)  
 * 25-Nov-2008 Abhishek Dhammawat SPR19189     Merge SPR18882 cycle3
 * 27-Nov-2008 Abhishek Dhammawat SPR19218     CSR 1-6232998 merge 
 * 11-Dec-2008  Anurag Khare     SPR 19218     CSR 1-6456839 Merged
 * 09-Feb-2009 Anurag Khare      ICF Rel 8.1   changes done fro storing the
 *                                             tunnel_mode in sic_ctx 
 * 02-Mar-2009  Preksha          IPTK rel8.1   icf_sic_process_options_resp
 * 04-Mar-2009 Saurabh Gupta    SPR 19349      Klocworks fix
 * 27-Mar-2009 Anuradha Gupta    Rel 8.2       Reg Event Pckg Subs Enh
 * 23-Apr-2009 Anuradha Gupta    SPR 19495     Modified fn icf_sic_map_api_to_event
 * 28-May-2009 Tarun Gupta       Rel 8.2       Async NOTIFY Support
 * 02-Jun-2009 Preksha           Rel 8.2       Asynchronus Message enhancement
 * 11-Jun-2009 Ashutosh Mohan    SPR 19737     Changes done for GCC 4.1 warning removal
 * 29-Jul-2009 Tarun Gupta       SPR 19895     Modified icf_sic_process_mesg
 * 12-Sep-2009 Rajiv Kumar       SPR 19999     Merged CSR 1-7467002
 * 16-Sep-2009 Rajiv Kumar       SPR 19999     Merged CSR 1-7615966 
 * 29-May-2010 Alok Tiwari       SPR 20412     Merged CSR 1-8223807 
 ******************************************************************************/

#include "icf_sic_prototypes.h"
#include "icf_sic_defs.h"
#include "icf_common_types.h"

/*******************************************************************************
 *
 * FUNCTION:        icf_sic_process_mesg
 *
 * DESCRIPTION:     This is the entry point function of SIC that is invoked by
 *                  MRM, REGM and SIGSA. 
 *
 ******************************************************************************/
icf_return_t icf_sic_process_mesg(
        INOUT icf_internal_msg_st        *p_internal_msg)
{
    icf_return_t			ret_val = ICF_SUCCESS;
    icf_sic_pdb_st			sic_pdb;
    icf_sic_data_st			*p_sic_data = ICF_NULL;
    icf_uint32_t			options_event = 0;	
    icf_uint32_t			message_event = 0;	
    icf_boolean_t           auto_subscription_for_reg = ICF_FALSE;


    ICF_SET_MODULE(p_internal_msg->msg_hdr.p_glb_pdb, ICF_INT_MODULE_SIC)

#ifdef ICF_UT_TEST
    /* p_ecode declared in icf_glb_pdb_st will point to 
       ecode in icf_internal_msg_st. To change this ecode, 
       value of pointer will be changed */
    p_internal_msg->msg_hdr.p_glb_pdb->p_ecode =
        &(p_internal_msg->msg_hdr.ecode);
#endif

    ICF_FUNCTION_ENTER(p_internal_msg->msg_hdr.p_glb_pdb)

    /* Copy the internal msg in the pdb */
    sic_pdb.p_internal_msg = p_internal_msg;
    ICF_ASSERT(p_internal_msg->msg_hdr.p_glb_pdb,
            (ICF_NULL != sic_pdb.p_internal_msg))

    /* Copy the global pdb in the pdb */
    sic_pdb.p_glb_pdb = p_internal_msg->msg_hdr.p_glb_pdb;
    ICF_ASSERT(p_internal_msg->msg_hdr.p_glb_pdb,
            (ICF_NULL != sic_pdb.p_glb_pdb))
    icf_sic_init_pdb(&sic_pdb);	 

    /* Request DBM to give the config data pointer */
    ret_val = icf_dbm_get_module_glb_data(
                sic_pdb.p_glb_pdb,
                (icf_int_module_id_t)ICF_INT_MODULE_CFG,
                (icf_void_t *)&(sic_pdb.p_config_data),
                sic_pdb.p_ecode);
    
    if (ICF_FAILURE == ret_val)
    {
       ICF_FUNCTION_EXIT(sic_pdb.p_glb_pdb)
       ICF_RESET_MODULE(sic_pdb.p_glb_pdb)
       return ret_val;            
    }
    
    /* In all the msg ids except the timer expiry the msg will be present 
     * in the p_msg_data in the internal msg
     */
    if (ICF_MSG_FROM_MRM != p_internal_msg->msg_hdr.msg_id)
    {
        sic_pdb.p_msg = (icf_void_t *)(sic_pdb.p_internal_msg->p_msg_data);
        ICF_ASSERT(sic_pdb.p_glb_pdb,(ICF_NULL != sic_pdb.p_msg))
    }

	/*Handle OPTIONS, MESSAGE, REFER, PUBLISH outside of state machine*/

	/* ICF_6_1_OPTIONS_SUPPORT start */
	/* Options handling will be in OPTIONS_FSM */
	/* ICF_6_1_OPTIONS_SUPPORT end */


	if( p_internal_msg->msg_hdr.msg_id == ICF_SIC_PUBLISH_REQ)
	{
		    ret_val = icf_sic_process_publish_req(&sic_pdb);
        ICF_FUNCTION_EXIT(sic_pdb.p_glb_pdb)
        ICF_RESET_MODULE(sic_pdb.p_glb_pdb)
        return ret_val;
	}
	if( p_internal_msg->msg_hdr.msg_id == ICF_SIC_MESSAGE_REQ)
	{
        ret_val = icf_sic_process_message_req(&sic_pdb);
        ICF_FUNCTION_EXIT(sic_pdb.p_glb_pdb)
        ICF_RESET_MODULE(sic_pdb.p_glb_pdb)
        return ret_val;
	}
	if( p_internal_msg->msg_hdr.msg_id == ICF_SIC_REFER_REQ)
	{
		    ret_val = icf_sic_process_refer_req(&sic_pdb);
        /*Klocwork warning resolve*/
        ICF_KLOCWORK_FALSE_POSITIVE(sic_pdb.p_glb_pdb,sic_pdb.p_list_1)

        ICF_FUNCTION_EXIT(sic_pdb.p_glb_pdb)
        ICF_RESET_MODULE(sic_pdb.p_glb_pdb)
        return ret_val;
	}

	/* ICF_6_1_OPTIONS_SUPPORT start */
	/* Options handling will be in OPTIONS_FSM */
	/* ICF_6_1_OPTIONS_SUPPORT end */


	if ((p_internal_msg->msg_hdr.msg_id == ICF_NW_INC_PUBLISH_RESP) ||
		(p_internal_msg->msg_hdr.msg_id == ICF_SIC_PUBLISH_STATUS))
	{
		ret_val = icf_sic_process_publish_resp(&sic_pdb);
        ICF_FUNCTION_EXIT(sic_pdb.p_glb_pdb)
        ICF_RESET_MODULE(sic_pdb.p_glb_pdb)
        return ret_val;
	}
	if ((p_internal_msg->msg_hdr.msg_id == ICF_NW_INC_MESSAGE_RESP) ||
		(p_internal_msg->msg_hdr.msg_id == ICF_SIC_MESSAGE_STATUS))
	{
		ret_val = icf_sic_process_message_resp(&sic_pdb);
        ICF_FUNCTION_EXIT(sic_pdb.p_glb_pdb)
        ICF_RESET_MODULE(sic_pdb.p_glb_pdb)
        return ret_val;
	}
	if ((p_internal_msg->msg_hdr.msg_id == ICF_NW_INC_REFER_RESP) ||
		(p_internal_msg->msg_hdr.msg_id == ICF_SIC_REFER_STATUS))
	{
		ret_val = icf_sic_process_refer_resp(&sic_pdb);
        /*Klocwork warning resolve*/
        ICF_KLOCWORK_FALSE_POSITIVE(sic_pdb.p_glb_pdb,sic_pdb.p_list_1)
        ICF_FUNCTION_EXIT(sic_pdb.p_glb_pdb)
        ICF_RESET_MODULE(sic_pdb.p_glb_pdb)
        return ret_val;
	}

    if(( ICF_NW_INC_MESSAGE_IND == p_internal_msg->msg_hdr.msg_id)
            &&  (ICF_NULL != sic_pdb.p_config_data) 
            && (ICF_FALSE == sic_pdb.p_config_data->async_message_support_reqd))
    {
        /* If Asychronus MESSAGE support not enabled */
        ret_val = icf_sic_process_new_message_ind(&sic_pdb);
        ICF_FUNCTION_EXIT(sic_pdb.p_glb_pdb)
        ICF_RESET_MODULE(sic_pdb.p_glb_pdb)
        return ret_val;
 }

	if ((p_internal_msg->msg_hdr.msg_id == ICF_SIC_SUBSCRIBE_REQ 
	   || p_internal_msg->msg_hdr.msg_id == ICF_INTERNAL_SUBSCRIBE_REQ))
    {

        /* the existing piece of code is restructured so that in case of reg event package
         * from REG module the SIC context is not set right now
         */ 
        icf_dbm_find_sic_cntxt_blk(
                sic_pdb.p_glb_pdb,
                ((icf_sic_subs_req_st *)(sic_pdb.p_msg))->call_id,
                &(sic_pdb.p_sic_ctx),
                sic_pdb.p_ecode);

        if(ICF_NULL != sic_pdb.p_sic_ctx)
        {
            /* in case of event package is reg and SIC context is found which means 
             * either it can be resubscription from application 
             * or it is a trigger from REGM module to perform reg event package subs
             * internal handling
             */
           /* in case of trigger from REGM, again reset the SIC context so that the 
            * handling is done in the function icf_sic_validate_event
            */ 
           if((0 == icf_port_strcmp(((icf_sic_subs_req_st *)(sic_pdb.p_msg))->event_package.str,(icf_uint8_t *)"reg")) &&
               (ICF_SIC_AUTO_SUBSCRIPTION_FOR_REG_EVENT & sic_pdb.p_sic_ctx->bitmask))
           {
                sic_pdb.p_sic_ctx =  ICF_NULL;
           } 
        }
	}
    
/*NOTIFY_OUT_OF_DIALOG_START*/

    /* SPR 19895 : Even if the application has enabled support of
       asynchronous NOTIFY for SUBSCRIBE, we should be able to
       handle an out of dialog NOTIFY. We will invoke function
       icf_sic_process_notify_cfm only if the bit
       ICF_SIC_METHOD_TYPE_OOD_NOTIFY is set. Else, the SIC
       Subscribe FSM will be triggerred.
    */

	if(p_internal_msg->msg_hdr.msg_id == ICF_SIC_NOTIFY_CFM)
	{
        /* First extract the SIC context on the basis of the Call ID. */
        icf_dbm_find_sic_cntxt_blk(
            sic_pdb.p_glb_pdb,
            ((icf_sic_notify_cfm_st *)(sic_pdb.p_msg))->call_id,
            &(sic_pdb.p_sic_ctx),
            sic_pdb.p_ecode);

        if((ICF_NULL != sic_pdb.p_sic_ctx) &&
           (ICF_SIC_METHOD_TYPE_OOD_NOTIFY == (ICF_SIC_METHOD_TYPE_OOD_NOTIFY & \
                    sic_pdb.p_sic_ctx->bitmask)))
        {
            ret_val = icf_sic_process_notify_cfm(&sic_pdb);
            ICF_FUNCTION_EXIT(sic_pdb.p_glb_pdb)
            ICF_RESET_MODULE(sic_pdb.p_glb_pdb)
            return ret_val;
        }
	}

    /* Asynchronous NOTIFY support : If an in dialog NOTIFY is received, the bit
       ICF_NW_INC_NOTIFY_OUT_OF_DIALOG will not be set by SSA. Hence the Subscribe
       FSM will be triggerred.
    */   
	if( p_internal_msg->msg_hdr.msg_id == ICF_NW_INC_NOTIFY_IND)
	{
		if(((icf_nw_inc_notify_ind_st *)p_internal_msg->p_msg_data)->bitmask 
			& ICF_NW_INC_NOTIFY_OUT_OF_DIALOG)
		{
			ret_val = icf_sic_process_notify_out_of_dialog(&sic_pdb);
            /*Klocwork warning resolve*/
            ICF_KLOCWORK_FALSE_POSITIVE(sic_pdb.p_glb_pdb,sic_pdb.p_list_1)

			ICF_FUNCTION_EXIT(sic_pdb.p_glb_pdb)
			ICF_RESET_MODULE(sic_pdb.p_glb_pdb)
			return ret_val;
		}
	}

/*NOTIFY_OUT_OF_DIALOG_END*/

    if (ICF_CLEAR_SIC_CTX == p_internal_msg->msg_hdr.msg_id)
    {
        icf_dbm_find_sic_cntxt_blk(
            sic_pdb.p_glb_pdb,
            ((icf_sic_clear_ctx_st *)(sic_pdb.p_msg))->call_id,
            &(sic_pdb.p_sic_ctx),
            sic_pdb.p_ecode);

        /* Forceful call clear received from CFG module
         *  and Asynchronus MESSAGE request is pending*/
        if((ICF_NULL != sic_pdb.p_config_data) 
                && (ICF_NULL != sic_pdb.p_sic_ctx) 
                && (ICF_TRUE == sic_pdb.p_config_data->async_message_support_reqd) 
                && (ICF_SIC_MESSAGE_REQ_ONGOING == (sic_pdb.p_sic_ctx->bitmask & ICF_SIC_MESSAGE_REQ_ONGOING)))
        {
            /* This will be used by function icf_sic_send_message_resp */
            sic_pdb.message_result = ICF_FAILURE;
            sic_pdb.resp_code = 500;
            /*Stop the message confirm Timer .Tigger SSA to send 500 response to the MESSAGE received. Call utility function 
             *icf_sic_send_message_resp to do the same.*/
            icf_sic_stop_message_timer(&sic_pdb);                
            icf_sic_send_message_resp(&sic_pdb);
            /*Reset bit ICF_SIC_MESSAGE_REQ_ONGOING in  icf_sic_send_message_resp*/
            if (ICF_NULL != sic_pdb.p_sic_ctx )
            {
                /* De-allocate SIC ctx */
                icf_sic_dealloc_context(&sic_pdb);
            }
        }


	    /* if incoming option is received then send the 4xx for the same
         * in case of in-dialog options we will free the sic context only if
         * event ICF_CLEAR_SIC_CTX is received from CC. In case it is recv from
         * cfg in case of restart, will not clear any in-dialog
         * transaction but we will clear the outof dialog contexts
         */
        /*Fix for SPR : 19188 (Rel 7.1.2 SPR merging for SPR 18889)*/
        if((ICF_NULL != sic_pdb.p_sic_ctx) &&
                (ICF_NULL != sic_pdb.p_sic_ctx->p_call_ctx) &&
                (sic_pdb.p_sic_ctx->p_call_ctx->common_bitmask_2 & 
                 ICF_CC_CLEAR_OPTIONS_CTXT))
        {
            /*This bitmask is set when OPTIONS request is already cleared by SSA
             *only timer is required to stop and SIC context to be deallocted 
             *otherwise call SSA to clear OPTIONS request*/
            if (ICF_SIC_PENDING_CLEAR_OPTIONS_CTX & 
                    sic_pdb.p_sic_ctx->pending_event)
            {
                sic_pdb.p_sic_ctx->p_call_ctx->common_bitmask_2 &= 
                    ~ICF_CC_CLEAR_OPTIONS_CTXT;
                /* not checking the return value this is the best that we can do */
                icf_sic_stop_options_timer(&sic_pdb);                
                /* De-allocate SIC ctx */
                icf_sic_dealloc_context(&sic_pdb);
            }
            else if(sic_pdb.p_sic_ctx->p_call_ctx->common_bitmask & 
                    ICF_IN_DLG_OPTIONS_RECV)
            {   
                sic_pdb.p_sic_ctx->p_call_ctx->common_bitmask_2 &= 
                    ~ICF_CC_CLEAR_OPTIONS_CTXT;
                /* not checking the return value this is the best that we can do */
                icf_sic_stop_options_timer(&sic_pdb);                
                icf_sic_send_failure_response_to_ssa(&sic_pdb); 
            }
            else if(sic_pdb.p_sic_ctx->p_call_ctx->common_bitmask & 
                    ICF_IN_DLG_OPTIONS_SEND)
            {
                /* SPR 18927: Do not reset the bitmaks for clear options context here
                 * it will be reset in the function when sending failure options '
                 * response to application.
                 */                 
                /* not checking the return value this is the best that we can do */
                icf_sic_stop_options_timer(&sic_pdb);                
                sic_pdb.p_internal_msg->msg_hdr.msg_id = ICF_SIC_OPTION_STATUS;
                sic_pdb.p_sic_ctx->error_code = ICF_CAUSE_CALL_RELEASED;
                icf_sic_send_failure_response_to_app(&sic_pdb);                
            }
        }
        /* SPR 18889: in case of call clear, clear the SIC context for
         * in dialog options and send failure response to application
         */  
        if((ICF_NULL != sic_pdb.p_sic_ctx) &&
           (ICF_NULL != sic_pdb.p_sic_ctx->p_call_ctx) &&
           (sic_pdb.p_sic_ctx->p_call_ctx->common_bitmask_2 & 
	        ICF_CC_CLEAR_OPTIONS_CTXT)&&
           (sic_pdb.p_sic_ctx->p_call_ctx->common_bitmask & 
	        ICF_IN_DLG_OPTIONS_SEND))
        {
            sic_pdb.p_sic_ctx->p_call_ctx->common_bitmask_2 &= 
		        ~ICF_CC_CLEAR_OPTIONS_CTXT;
            /* not checking the return value this is the best that we can do */
            icf_sic_stop_options_timer(&sic_pdb);
            sic_pdb.p_internal_msg->msg_hdr.msg_id = ICF_CAUSE_CALL_RELEASED;
            sic_pdb.p_sic_ctx->error_code = ICF_CAUSE_LICENSE_EXPIRED;
		    icf_sic_send_failure_response_to_app(&sic_pdb);
        }

	    /*If the above condition is met then sic context would have been freed*/
        if(ICF_NULL != sic_pdb.p_sic_ctx)
        {
            /* If this is an indialog context and not been freed yet means that
	         * this CLEAR_SIC request was recvd from cfg hence we donot free it
	         */
            /*Fix for SPR : 19188 (Rel 7.1.2 SPR merging for SPR 19161)*/
            /* SPR 19161: In case internal requested Subscribe is ongoing for
             * xconf scenarios then send the unsubscibe request and delink the
             * ssa context and also deallocate the sic context. This case is added
             * when cfg is triggering call clear.
             */
            if((ICF_NULL == sic_pdb.p_sic_ctx->p_call_ctx) ||
                (sic_pdb.p_sic_ctx->bitmask & 
                    ICF_SIC_INTERNAL_SUBSCRIBE_REQUEST))            
            {
                icf_sic_stop_curr_timer(&sic_pdb);

		        icf_sic_stop_options_timer(&sic_pdb);

		        if((ICF_SUBSCRIBE_REQ == sic_pdb.p_sic_ctx->method)&&
                   (ICF_SIC_03_UNSUBSCRIBING != sic_pdb.p_sic_ctx->sic_state))
                {
                    
                    /*If the subscription for reg event package is true and
                     * the event package in sic_pdb is "reg" then set the
                     * value of auto_subscription_for_reg as TRUE.So in this
                     * case there is no need to send the unsubscribe request
                     * on network for reg event .The check is used to prevent
                     * the unsubscribe request for reg event package in case of
                     * network initiated deregistration .By default the
                     * subscription for reg event package is ON.*/
                    
                    if (ICF_SIC_AUTO_SUBSCRIPTION_FOR_REG_EVENT ==
                         (sic_pdb.p_sic_ctx->bitmask &
                          ICF_SIC_AUTO_SUBSCRIPTION_FOR_REG_EVENT))                      
                    {
                        auto_subscription_for_reg = ICF_TRUE;
                    }   
                    if (ICF_FALSE == auto_subscription_for_reg)
                    {    
                        sic_pdb.p_sic_ctx->app_requested_duration=ICF_NULL;
                        icf_sic_send_unsub_req(&sic_pdb);
                    }     
                }

                /* Asynchronous NOTIFY support :
                   If NOTIFY is being processed, send a 500 response and
                   deallocate SIC context.
                */   
                if(ICF_SIC_NOTIFY_PROCESSING_ONGOING  ==
                      (sic_pdb.p_sic_ctx->bitmask & ICF_SIC_NOTIFY_PROCESSING_ONGOING))
                {
                   /* This will be used by function icf_sic_send_notify_resp to
                       trigger SSA to send NOTIFY response.
                    */
                    sic_pdb.notify_resp = ICF_FAILURE;
                    sic_pdb.resp_code = 500;
                    icf_sic_send_notify_resp(&sic_pdb);
                    sic_pdb.p_sic_ctx->bitmask &= ~ICF_SIC_NOTIFY_PROCESSING_ONGOING;
                }

               /* Fix for CSR 1-6993263: Sending STATUS_INDICATION to 
                * application before clearing the contexts in IPTK. */

               /*ICF_SIC_SET_CURR_STATUS(&sic_pdb,ICF_SUBS_UNSUBSCRIBED)*/

               icf_sic_send_subs_state_to_app(&sic_pdb);

               icf_sic_send_delink_to_ssa(&sic_pdb,
                    (icf_boolean_t)ICF_FALSE);
                /* De-allocate SIC ctx */
                icf_sic_dealloc_context(&sic_pdb);
           }
        }
        return ICF_SUCCESS;
    }

    /* New Code for ASYNCHRONUS_MESSAGE enhancements
     *If asynchronus flag is set Trigger the MESSAGE FSM 
     *else Process new message indication.*/

    ret_val = icf_sic_map_to_message_event(&sic_pdb,&message_event);
    if (( ICF_SIC_MAX_MESSAGE_EVENTS != message_event)  
            && (ICF_NULL != sic_pdb.p_config_data) 
            && (ICF_TRUE == sic_pdb.p_config_data->async_message_support_reqd))
    {
        if ( ICF_FAILURE == ret_val || 
                ICF_NULL == sic_pdb.p_sic_ctx )
        {
            ret_val = ICF_FAILURE;
            ICF_FUNCTION_EXIT(sic_pdb.p_glb_pdb)
            ICF_RESET_MODULE(sic_pdb.p_glb_pdb)
            return ICF_FAILURE;
        }
        sic_pdb.event = (icf_uint8_t) message_event;
        ret_val = icf_dbm_get_module_glb_data(
                sic_pdb.p_glb_pdb,
                (icf_int_module_id_t)(ICF_INT_MODULE_SIC),
                (icf_void_t *)&p_sic_data,
                sic_pdb.p_ecode);

        if (ICF_FAILURE == ret_val)
        {
            ICF_FUNCTION_EXIT(sic_pdb.p_glb_pdb)
                ICF_RESET_MODULE(sic_pdb.p_glb_pdb)
                return ret_val;
        }

        /* If asychronus MESSAGE support is enabled and incoming message 
         * request is received set the request ongoing bit.*/
        if(ICF_NW_INC_MESSAGE_IND == p_internal_msg->msg_hdr.msg_id)
        {
            sic_pdb.p_sic_ctx->bitmask |= ICF_SIC_MESSAGE_REQ_ONGOING;
        }

        if ((ICF_SIC_MAX_MESSAGE_STATES > sic_pdb.p_sic_ctx->sic_state) 
                && (ICF_SIC_MAX_MESSAGE_EVENTS > message_event))
        {
            ret_val = p_sic_data->sic_message_fsm_table[sic_pdb.p_sic_ctx->
                sic_state][message_event](&sic_pdb);        
        }
        ICF_FUNCTION_EXIT(sic_pdb.p_glb_pdb)
            ICF_RESET_MODULE(sic_pdb.p_glb_pdb)
            return ret_val;

    }
/* Asynchronus MESSAGE Support end */

	/* ICF_OPTIONS_SUPPORT start */
	ret_val = icf_sic_map_to_options_event(&sic_pdb,&options_event);
	if ( ICF_SIC_MAX_OPTIONS_EVENTS != options_event)
	{
		if ( ICF_FAILURE == ret_val || 
			ICF_NULL == sic_pdb.p_sic_ctx )
		{
			return ICF_FAILURE;
		}
		sic_pdb.event = (icf_uint8_t) options_event;
		ret_val = icf_dbm_get_module_glb_data(
                        sic_pdb.p_glb_pdb,
                        (icf_int_module_id_t)(ICF_INT_MODULE_SIC),
                        (icf_void_t *)&p_sic_data,
                        sic_pdb.p_ecode);
            
            	if (ICF_FAILURE == ret_val)
            	{
			ICF_FUNCTION_EXIT(sic_pdb.p_glb_pdb)
			ICF_RESET_MODULE(sic_pdb.p_glb_pdb)
			return ret_val;
		}
				if ( (sic_pdb.p_sic_ctx->sic_state < ICF_SIC_MAX_OPTIONS_STATES) &&
					(options_event < ICF_SIC_MAX_OPTIONS_EVENTS))
				{
				ret_val = p_sic_data->sic_options_fsm_table[sic_pdb.p_sic_ctx->
                    sic_state][options_event](&sic_pdb);        
				}
                ICF_FUNCTION_EXIT(sic_pdb.p_glb_pdb)
                ICF_RESET_MODULE(sic_pdb.p_glb_pdb)
                return ret_val;

	}
	/* ICF_6_1_OPTIONS_SUPPORT end */

    /* SPR: 9079 fix start */
    if (ICF_FORCEFUL_CALL_CLEAR_IND == 
            p_internal_msg->msg_hdr.msg_id)
    {
        ret_val = icf_sic_process_clear_all(&sic_pdb);
        ICF_FUNCTION_EXIT(sic_pdb.p_glb_pdb)
        ICF_RESET_MODULE(sic_pdb.p_glb_pdb)
        return ret_val;
    }

    if(ICF_FORCEFUL_REINIT_IND == 
            p_internal_msg->msg_hdr.msg_id)
    {
        ret_val = icf_sic_process_reinit_all(&sic_pdb);
        ICF_FUNCTION_EXIT(sic_pdb.p_glb_pdb)
        ICF_RESET_MODULE(sic_pdb.p_glb_pdb)
        return ret_val;
    }

/* SPR:9079  fix end */
	
    /* SUBSCRIPTION related handling begins*/
    
    ret_val = icf_sic_map_api_to_event(&sic_pdb);
    
    if (ICF_SIC_EVENT_INVALID != sic_pdb.event)
    {
        if (ICF_SIC_EVENT_FORCEFUL_DELETE == sic_pdb.event)
        {
            /*ret_val = icf_sic_handle_forceful_delete(&sic_pdb);*/

        }
        
        else
        {
            ret_val = icf_dbm_get_module_glb_data(
                        sic_pdb.p_glb_pdb,
                        (icf_int_module_id_t)(ICF_INT_MODULE_SIC),
                        (icf_void_t *)&p_sic_data,
                        sic_pdb.p_ecode);
            
            if (ICF_FAILURE == ret_val)
            {
                ICF_FUNCTION_EXIT(sic_pdb.p_glb_pdb)
                ICF_RESET_MODULE(sic_pdb.p_glb_pdb)
                return ret_val;
            }
            
            ret_val = icf_sic_validate_event(&sic_pdb);
        
            if ((ICF_SUCCESS == ret_val) && 
                    (ICF_TRUE == sic_pdb.valid_event))
            {
                /* if the received event is a valid event then the FSM will be 
                 * triggered. All the event package specific validation
                 * will be done till now and SIC ctx pointer will be there in 
                 * the pdb (either existing ctx or a new ctx)
                 */
            
				if ( (sic_pdb.p_sic_ctx->sic_state < ICF_SIC_MAX_STATES) &&
					(sic_pdb.event < ICF_SIC_MAX_EVENTS))
				{
                ret_val = p_sic_data->sic_fsm_table[sic_pdb.p_sic_ctx->
                    sic_state][sic_pdb.event](&sic_pdb);        
				}
            }
            
            else if (ICF_FAILURE == ret_val)
            {
                /* Send subs state to the src module */
				if( ICF_NULL == sic_pdb.p_sic_ctx)
				{
					ICF_FUNCTION_EXIT(sic_pdb.p_glb_pdb)
					ICF_RESET_MODULE(sic_pdb.p_glb_pdb)
					return ret_val;
				}
				else
				{
					icf_sic_send_subs_state_in_failure(&sic_pdb);
				}
            }
        }/* End else */      
    }/* if (ICF_SIC_EVENT_INVALID != ..*/     
    /*Klocwork warning resolve*/
    ICF_KLOCWORK_FALSE_POSITIVE(sic_pdb.p_glb_pdb,sic_pdb.p_list_1)
   
    ICF_FUNCTION_EXIT(sic_pdb.p_glb_pdb)
    ICF_RESET_MODULE(sic_pdb.p_glb_pdb)
    return ret_val;    
}   


/*******************************************************************************
 *
 * FUNCTION:        icf_sic_map_api_to_event
 *
 * DESCRIPTION:     This function maps the received API to the event.
 *
 ******************************************************************************/
icf_return_t icf_sic_map_api_to_event(
        INOUT icf_sic_pdb_st      *p_sic_pdb)
{
    icf_return_t             ret_val = ICF_SUCCESS;
    icf_nw_inc_subs_resp_st  *p_subs_resp = ICF_NULL;
    icf_nw_inc_notify_ind_st *p_notify_ind = ICF_NULL;
    icf_timer_data_st        *p_timer_data = ICF_NULL;
    icf_sic_subs_req_st      *p_subs_req = ICF_NULL;
    icf_sic_unsubs_req_st    *p_unsubs_req = ICF_NULL;
    icf_boolean_t            free_timer = ICF_FALSE;
    icf_sic_notify_cfm_st    *p_notify_cfm = ICF_NULL;
        
    ICF_FUNCTION_ENTER(p_sic_pdb->p_glb_pdb)
       
    switch (p_sic_pdb->p_internal_msg->msg_hdr.msg_id)
    {
        case ICF_SIC_SUBSCRIBE_REQ:
        case ICF_INTERNAL_SUBSCRIBE_REQ:   
            {
                p_subs_req = (icf_sic_subs_req_st *)(p_sic_pdb->p_msg);
				
                
				icf_port_strcpy(p_sic_pdb->event_package.str,
				p_subs_req->event_package.str);
				p_sic_pdb->event_package.str_len = (icf_uint16_t)icf_port_strlen(p_sic_pdb->event_package.str);

                           
                icf_port_memcpy( 
					(icf_void_t*)&(p_sic_pdb->dest_addr),
					(icf_void_t*) &( ( (icf_sic_subs_req_st *)(p_sic_pdb->p_msg))->destination_info),
					(icf_uint32_t) ICF_PORT_SIZEOF(icf_address_st) ); 
				
                
                ICF_ASSERT(p_sic_pdb->p_glb_pdb,
                    (ICF_NULL != p_sic_pdb->p_dest_addr))
            
            
                /*ICF_SIC_SET_EVENT(p_sic_pdb,ICF_SIC_EVENT_SUBS_REQ)*/

			   if( ( (icf_sic_subs_req_st *)(p_sic_pdb->p_msg))->duration )
				{
					ICF_SIC_SET_EVENT(p_sic_pdb,ICF_SIC_EVENT_SUBS_REQ)
				}
				else
				{
					if ( ICF_NULL == p_sic_pdb->p_sic_ctx)
					{
						ICF_SIC_SET_EVENT(p_sic_pdb,ICF_SIC_EVENT_SUBS_REQ)
					}
					else
					{
						ICF_SIC_SET_EVENT(p_sic_pdb,ICF_SIC_EVENT_UNSUB_REQ)
                    }
				}
            }
            break;
    
        case ICF_SIC_UNSUBSCRIBE_REQ:
            {
                p_unsubs_req = (icf_sic_unsubs_req_st *)(p_sic_pdb->p_msg);

				icf_port_memcpy(((icf_void_t *)(&(p_sic_pdb->event_package ))),
				((icf_void_t *)(&(p_sic_pdb->p_sic_ctx->event_package))),
						ICF_PORT_SIZEOF(icf_string_st));

                if (ICF_FALSE == p_unsubs_req->forceful_delete)
                {
                    ICF_SIC_SET_EVENT(p_sic_pdb,
                        ICF_SIC_EVENT_UNSUB_REQ)

                 
					/* Fetch the SIC context block from the 
					 * received call-id and populate the destination address
					 * from it*/
                    /*p_sic_pdb->p_dest_addr = (icf_address_st *)*/

                    /*    (p_unsubs_req->p_dest_addr);     */

                    /*ICF_ASSERT(p_sic_pdb->p_glb_pdb,*/

                    /*    (ICF_NULL != p_sic_pdb->p_vms_server_addr)) */

                        

                }/* if (ICF_FALSE == .. */
                else
                {
                    ICF_SIC_SET_EVENT(p_sic_pdb,
                            ICF_SIC_EVENT_FORCEFUL_DELETE)
                }
            }
            break;
            
        case ICF_NW_INC_SUBSCRIBE_RESP:
            {
                p_subs_resp = (icf_nw_inc_subs_resp_st *)
                    (p_sic_pdb->p_msg);
            
                p_sic_pdb->p_sic_ctx = p_subs_resp->p_sic_ctx;

                /* 
                   Asynchronous NOTIFY Support:
                   Copy the request received from the peer into p_pending_req field
                   of SIC context.While control is in ICF_SIC_05_AWAIT_NOTIFY_CFM
                   state,this request will be marked as pending and will be
                   processed when NOTIFY confirm is received from the application.
                   Make a copy of the request only if the current state in SIC 
                   context is 05.
                */ 
                if((ICF_NULL != p_sic_pdb->p_config_data) &&
                   (ICF_TRUE == p_sic_pdb->p_config_data->async_notify_support_reqd) &&
                   (ICF_SIC_05_AWAIT_NOTIFY_CFM == p_sic_pdb->p_sic_ctx->sic_state))     
                {
                    /* Free previously acquired memory for p_pending_req */
                    if(ICF_NULL != p_sic_pdb->p_sic_ctx->p_pending_req)
                    {
                        ICF_MEMFREE(p_sic_pdb->p_glb_pdb,
                                    p_sic_pdb->p_sic_ctx->p_pending_req,
                                    ICF_MEM_COMMON,
                                    p_sic_pdb->p_ecode,
                                    ret_val)
                    }

                    /* Allocate memory for p_pending_req */
                    ICF_MEMGET(p_sic_pdb->p_glb_pdb,
                               ICF_PORT_SIZEOF(icf_nw_inc_subs_resp_st),
                               ICF_MEM_COMMON,
                               p_sic_pdb->p_sic_ctx->p_pending_req,
                               ICF_RET_ON_FAILURE,
                               p_sic_pdb->p_ecode,
                               ret_val)

                    /* Make a copy of the request received */    
				    icf_port_memcpy(p_sic_pdb->p_sic_ctx->p_pending_req,
				                    p_sic_pdb->p_msg,
        						    ICF_PORT_SIZEOF(icf_nw_inc_subs_resp_st));
                }

                ICF_ASSERT(p_sic_pdb->p_glb_pdb,
                        (ICF_NULL != p_sic_pdb->p_sic_ctx))

				icf_port_memcpy(((icf_void_t *)(&(p_sic_pdb->event_package ))),
				((icf_void_t *)(&(p_sic_pdb->p_sic_ctx->event_package))),
						ICF_PORT_SIZEOF(icf_string_st));


                
/*                ICF_SIC_TRACE(p_sic_pdb,ICF_TRACE_SIC_EVENT_PKG_RECD)*/


				p_sic_pdb->resp_code=p_subs_resp->resp_code;

                /* If response is success */
                if (ICF_SUCCESS == p_subs_resp->response)
                {
                    ICF_SIC_SET_EVENT(p_sic_pdb,
                            ICF_SIC_EVENT_SUCCESS_RESP)
                }                   
               
                /* If failure with no retry */
                else if (ICF_NW_INC_SUBS_RESP_NO_RETRY == 
                        p_subs_resp->response)
                {
                    ICF_SIC_SET_EVENT(p_sic_pdb,
                            ICF_SIC_EVENT_FAILURE_RESP_NO_RETRY)
                }
                
                /* If failure with retry */
                else if (ICF_NW_INC_SUBS_RESP_WITH_RETRY == 
                        p_subs_resp->response)
                {
				   /* Handling network success response with expires=0 
					* for Unsubscribe request */
					if (0 == p_subs_resp->duration) 
					{ 
                        if (ICF_SIC_03_UNSUBSCRIBING == p_sic_pdb->p_sic_ctx->sic_state || 
							(ICF_SIC_01_SUBSCRIBING == p_sic_pdb->p_sic_ctx->sic_state && 
							(p_sic_pdb->p_sic_ctx->bitmask & ICF_SIC_POLL_SUBSCRIBE_REQ)))
                        { 
							ICF_SIC_SET_EVENT(p_sic_pdb, 
								ICF_SIC_EVENT_SUCCESS_RESP) 
                        }
                        /* SPR 19495 : In case the Subscription request is being
                         * sent internally for reg event package subscription then it
                         * can not be retried therefore the event should be FAILURE with
                         * no RETRY else the SIC context block shall not be deallocted
                         */  
                        else if(ICF_SIC_AUTO_SUBSCRIPTION_FOR_REG_EVENT ==
                             (p_sic_pdb->p_sic_ctx->bitmask &
                                    ICF_SIC_AUTO_SUBSCRIPTION_FOR_REG_EVENT))
                        {
                            ICF_SIC_SET_EVENT(p_sic_pdb,
                                 ICF_SIC_EVENT_FAILURE_RESP_NO_RETRY)
                        }
                        else 
                        { 
							ICF_SIC_SET_EVENT(p_sic_pdb, 
								ICF_SIC_EVENT_FAILURE_RESP_WITH_RETRY) 
                        } 
					} 
                    /* SPR 19495 : In case the Subscription request is being
                     * sent internally for reg event package subscription then it
                     * can not be retried therefore the event should be FAILURE with
                     * no RETRY else the SIC context block shall not be deallocted
                     */  
                    else if(ICF_SIC_AUTO_SUBSCRIPTION_FOR_REG_EVENT ==
                            (p_sic_pdb->p_sic_ctx->bitmask &
                             ICF_SIC_AUTO_SUBSCRIPTION_FOR_REG_EVENT))
                    {
                        ICF_SIC_SET_EVENT(p_sic_pdb,
                                ICF_SIC_EVENT_FAILURE_RESP_NO_RETRY)
                    }
					else 
					{
						ICF_SIC_SET_EVENT(p_sic_pdb, 
							ICF_SIC_EVENT_FAILURE_RESP_WITH_RETRY)
					}
                }
            }/* case ICF_NW_INC_SUBSCRIBE_RESP.. */
            break;
    
        case ICF_NW_INC_NOTIFY_IND:
            {
                p_notify_ind = (icf_nw_inc_notify_ind_st *)
                    (p_sic_pdb->p_msg);
                
                p_sic_pdb->p_sic_ctx = p_notify_ind->p_sic_ctx;
                ICF_ASSERT(p_sic_pdb->p_glb_pdb,
                        (ICF_NULL != p_sic_pdb->p_sic_ctx))

               icf_port_memcpy(((icf_void_t *)(&(p_sic_pdb->event_package ))),
				((icf_void_t *)(&(p_sic_pdb->p_sic_ctx->event_package))),
						ICF_PORT_SIZEOF(icf_string_st));

		if( 
                   icf_port_strcmp(
					p_sic_pdb->event_package.str,(icf_uint8_t *)"refer") ==0 )
		{
		   /*This is a notification for REFER*/
		   /* Pass it to the application without invoking the SM*/
		   /* Send notify resp to SSA */
		   icf_sic_send_notify_resp(p_sic_pdb);
		   icf_sic_send_notify_msg_to_app(p_sic_pdb);
                   icf_sic_dealloc_context(p_sic_pdb);
		   return ret_val;
		}
        /* to identify the trigger is from Reg module, check for existence of bitmask
             * instead of system level configuration check for reg event pcks subs suppport
             */ 
        else if(ICF_SIC_AUTO_SUBSCRIPTION_FOR_REG_EVENT ==
                   (ICF_SIC_AUTO_SUBSCRIPTION_FOR_REG_EVENT & p_sic_pdb->p_sic_ctx->bitmask))    
		{
                    /* If Notify received is having event package as reg and await notify
                     * timer is not started then it will be forwarded to cfg which in turn
                     * will send it to regm module.
                     * if event package was reg but await notify timer was started then
                     * IPTK will handle it in sic state 02 ,with the boolean 
                     * await_notify_for_reg_event_pkg in sic_pdb set to true
                     * ,indicating that we are waiting for the notify message.
                     */
		     if (((p_sic_pdb->p_sic_ctx->timer_id != ICF_TIMER_ID_INVALID) && \
			 (ICF_TIMER_TYPE_AWAIT_SUBS_NOTIFY_REQ == p_sic_pdb->p_sic_ctx->timer_type)))
		     {	      
		         p_sic_pdb->await_notify_for_reg_event_pkg = ICF_TRUE;  
                
		     }   
		}
                
/*                ICF_SIC_TRACE(p_sic_pdb,ICF_TRACE_SIC_EVENT_PKG_RECD)*/


                /* Subs state is active */
                if (ICF_SUBS_STATE_ACTIVE == p_notify_ind->subs_state)
                {
                    ICF_SIC_SET_EVENT(p_sic_pdb,
                            ICF_SIC_EVENT_NOTIFY_ACTIVE)
                }
                
                /* State is terminated */
                else if (ICF_SUBS_STATE_TERMINATED == 
                        p_notify_ind->subs_state)
                {
                    /* reason code is no retry */
                    if (ICF_NW_INC_NOTIFY_NO_RETRY == 
                            p_notify_ind->reason_code)
                    {
                       ICF_SIC_SET_EVENT(p_sic_pdb, 
                           ICF_SIC_EVENT_NOTIFY_TERM_NO_RETRY)
                    }
                    
                    /* reason code is rety imm */
                    else if (ICF_NW_INC_NOTIFY_RETRY_IMM == 
                            p_notify_ind->reason_code)
                    {
                        ICF_SIC_SET_EVENT(p_sic_pdb,
                            ICF_SIC_EVENT_NOTIFY_TERM_RETRY_IMM)
                    }
                    
                    /* reason code is retry after */
                    else if (ICF_NW_INC_NOTIFY_RETRY_AFTER ==
                            p_notify_ind->reason_code)
                    {
                        ICF_SIC_SET_EVENT(p_sic_pdb,
                            ICF_SIC_EVENT_NOTIFY_TERM_RETRY_AFTER)
                    }
                }/* else if (ICF_SUBS_STATE_TERMINATED ==.. */
            }/* case ICF_NW_INC_NOTIFY_IND.. */ 
            break;
 
        case ICF_MSG_FROM_MRM:
            {
                p_timer_data = *((icf_timer_data_st **)
                    (p_sic_pdb->p_glb_pdb->p_recv_msg->payload));
                ICF_ASSERT(p_sic_pdb->p_glb_pdb,
                        (ICF_NULL != p_timer_data))
                
                p_sic_pdb->p_sic_ctx = (icf_sic_ctx_st *)
                    (p_timer_data->p_timer_buffer);
                ICF_ASSERT(p_sic_pdb->p_glb_pdb,
                        (ICF_NULL != p_sic_pdb->p_sic_ctx))

               icf_port_memcpy(((icf_void_t *)(&(p_sic_pdb->event_package ))),
				((icf_void_t *)(&(p_sic_pdb->p_sic_ctx->event_package))),
						ICF_PORT_SIZEOF(icf_string_st));

                
/*                ICF_SIC_TRACE(p_sic_pdb,ICF_TRACE_SIC_EVENT_PKG_RECD)*/

                
                /* Timer type is response timer */
                if ((ICF_TIMER_TYPE_SUBS_RESPONSE == p_timer_data->
                            timer_type) &&
                        (ICF_TIMER_TYPE_SUBS_RESPONSE == 
                         p_sic_pdb->p_sic_ctx->timer_type))
                {
                    ICF_SIC_SET_EVENT(p_sic_pdb,
                            ICF_SIC_EVENT_RESP_TIMER_EXP)
                    free_timer = ICF_TRUE;                        
                }
                /* Timer type is poll notify request */
				else if ( ICF_TIMER_TYPE_AWAIT_SUBS_NOTIFY_REQ ==
							p_timer_data->timer_type)
				{
					ICF_SIC_SET_EVENT(p_sic_pdb,
						ICF_SIC_EVENT_AWAIT_SUBS_NOTIFY_TIMER_EXP)
					free_timer  = ICF_TRUE;
				}
                /* Timer type is duration timer */
                else if ((ICF_TIMER_TYPE_SUBS_DURATION == 
                            p_timer_data->timer_type) && 
                        (ICF_TIMER_TYPE_SUBS_DURATION == 
                            p_sic_pdb->p_sic_ctx->timer_type))
                {
                    ICF_SIC_SET_EVENT(p_sic_pdb,
                            ICF_SIC_EVENT_DURATION_TIMER_EXP)
                    free_timer = ICF_TRUE;                        
                }
                
                /* Timer type is retry timer */
                else if ((ICF_TIMER_TYPE_SUBS_RETRY == 
                            p_timer_data->timer_type) && 
                        (ICF_TIMER_TYPE_SUBS_RETRY == 
                            p_sic_pdb->p_sic_ctx->timer_type))
                {
                    ICF_SIC_SET_EVENT(p_sic_pdb,
                            ICF_SIC_EVENT_RETRY_TIMER_EXP)
                    free_timer = ICF_TRUE;                        
                }
                
/*NOTIFY_OUT_OF_DIALOG_START*/

				
				/* Timer type is notify timer */
                else if ((ICF_TIMER_TYPE_NOTIFY == 
                            p_timer_data->timer_type) && 
                        (ICF_TIMER_TYPE_NOTIFY == 
                            p_sic_pdb->p_sic_ctx->timer_type))
                {
                    /* send the 200 response for the notify */
					/* Send notify msg to the corresponding module */
					p_sic_pdb->notify_resp=ICF_SUCCESS;
					icf_sic_send_notify_resp(p_sic_pdb);
                    
                    /*SPR 18430 For out of dialog Notify when PA does not send
                      notify_cfm sic context was deallocated and ssa ctx pointer 
                      inside sic ctx was set to null but sic context inside ssa ctx
                      was not set to null immediately. In between another out of dialog
                      notify is received by IPTK and system allocates same memory block
                      again for new sic context. Now timer expiry for first SSA context
                      comes which still has the sic context pointer, however the memory
                      now belongs to new sic context therefore inside FreeAppData it sets
                      the new SSA ctx to null and later crash occurs when accessing the
                      pointer for new SSA ctx. Therefore to just initilaize the sic ctx 
                      pointer inside SSA ctx after sending notify response we used below
                      flag which is set true for only pointer to be set and
                      not forceful timer expiry to be done in delink_to_ssa*/ 
                    p_sic_pdb->p_sic_ctx->detach_sic_ctx = ICF_TRUE;
		            icf_sic_send_delink_to_ssa(p_sic_pdb,
                           (icf_boolean_t)ICF_FALSE); 
					
                    icf_sic_dealloc_context(p_sic_pdb);
       
					/* Rest processing is done if the success response of notify is send */
					if (ICF_FAILURE == p_sic_pdb->notify_resp)
					{
						/*ICF_SIC_TRACE(p_sic_pdb,ICF_TRACE_SIC_NOTIFY_FAIL)*/

						ret_val = ICF_FAILURE;
					}
                    free_timer = ICF_TRUE;                        
                }
/*NOTIFY_OUT_OF_DIALOG_END*/

                /* Handling done for new timer type ICF_TIMER_TYPE_NOTIFY_CFM */
                else if(ICF_TIMER_TYPE_NOTIFY_CFM == p_timer_data->timer_type)
	            {
                    ICF_SIC_SET_EVENT(p_sic_pdb,
                              		  ICF_SIC_EVENT_NOTIFY_CFM_TIMER_EXP)
                    /* No need to free the timer block in the end on the basis of
                       this boolean. The timer data in the SIC context for
                       NOTIFY CFM will be freed here only.
                    */
                    free_timer = ICF_FALSE;                        

        		    /* Free timer data */
	                if(ICF_NULL != p_sic_pdb->p_sic_ctx->p_notify_cfm_timer_data)
                    {
               	        ret_val = icf_dbm_ret_mem_to_pool(
                                      p_sic_pdb->p_glb_pdb,
                                      ICF_MEM_TIMER,
                              	      p_sic_pdb->p_sic_ctx->p_notify_cfm_timer_data,
                                      p_sic_pdb->p_ecode);
               	        /* Timer block has been successfully returned to the pool */
                        if(ICF_SUCCESS == ret_val)
              	        {
                	 	    p_sic_pdb->p_sic_ctx->p_notify_cfm_timer_data = ICF_NULL;
                            p_sic_pdb->p_sic_ctx->notify_cfm_timer_id = 
                                ICF_TIMER_ID_INVALID;
                    	}
                        /* Timer block freeing failure. Raise a alarm and proceed. */
               	        else
                    	{
                            ICF_PRINT(((icf_uint8_t *)"[SIC]: Failure in freeing timer block.\n"))
               	        }
                    }
                }
                
                if (ICF_TRUE == free_timer)
                {
                    icf_dbm_ret_mem_to_pool(p_sic_pdb->p_glb_pdb,
                            (icf_pool_id_t )ICF_MEM_TIMER,
                            p_timer_data,
                            p_sic_pdb->p_ecode);
                }
            }/* case ICF_TIMER_EXPIRY_IND.. */           
            break;
  
        /* Asynchronous NOTIFY support : Handling of event ICF_SIC_NOTIFY_CFM */
        case ICF_SIC_NOTIFY_CFM:
             
             p_notify_cfm = (icf_sic_notify_cfm_st*)(p_sic_pdb->p_msg);
				
             /* Find the SIC context, if successfully found, set event as
               ICF_SIC_EVENT_NOTIFY_CFM,else, set event as INVALID.
             */
             ret_val = icf_dbm_find_sic_cntxt_blk(
                           p_sic_pdb->p_glb_pdb,
                           p_notify_cfm->call_id,
                           &(p_sic_pdb->p_sic_ctx),
                           p_sic_pdb->p_ecode);
			
             if(ICF_SUCCESS == ret_val)
             {
                 ICF_SIC_SET_EVENT(p_sic_pdb,
                                   ICF_SIC_EVENT_NOTIFY_CFM);
             }
             else
             {
                 ICF_SIC_SET_EVENT(p_sic_pdb,
                                   ICF_SIC_EVENT_INVALID)
             }
 
             break;
            
        default:
            break;
    }/* End switch */        
    
    ICF_FUNCTION_EXIT(p_sic_pdb->p_glb_pdb)
    return ret_val;
}





/*******************************************************************************
 *
 * FUNCTION:        icf_sic_validate_event
 *
 * DESCRIPTION:     This function does the validations on the message-summary
 *                  event package.  
 *                  These validations are independent of the state machine.
 *                  It also creates a new SIC context if the context does not
 *                  exists.
 *
 ******************************************************************************/
icf_return_t icf_sic_validate_event(
        INOUT icf_sic_pdb_st      *p_sic_pdb)

{
    icf_return_t     ret_val = ICF_SUCCESS;
    icf_return_t     dummy_retval = ICF_SUCCESS;
    icf_line_id_t    line_id = 0;
    icf_sic_subs_req_st   *p_subs_req = ICF_NULL;

    ICF_FUNCTION_ENTER(p_sic_pdb->p_glb_pdb)
    p_subs_req = (icf_sic_subs_req_st *)(p_sic_pdb->p_msg);
 
    for(line_id = 0; (line_id < p_sic_pdb->p_config_data->max_lines) && (line_id < ICF_MAX_NUM_OF_LINES ) ; line_id++)
    {
        /* Fetch the pointer of the line from DBM */
        dummy_retval = icf_dbm_fetch_line_cntxt_blk(
                p_sic_pdb->p_glb_pdb,
                line_id,
                &(p_sic_pdb->p_line_data[line_id]),
                p_sic_pdb->p_ecode);
    }

    /*dummy_retval = icf_dbm_find_sic_cntxt_blk(
                        p_sic_pdb->p_glb_pdb,
                        ((icf_sic_subs_req_st *)(p_sic_pdb->p_msg))->call_id,
                        &(p_sic_pdb->p_sic_ctx),
                        p_sic_pdb->p_ecode);*/

    /* If Context cooresponding to the call id  exists i.e. 
     * DBM returns success 
     */

    /* If no context exists for the call id then a new context will
     * be allocated
     */
    /*if (ICF_FAILURE == dummy_retval)*/

	if( ICF_NULL ==	p_sic_pdb->p_sic_ctx )
    {
        /* Request DBM to make a new SIC ctx in case of subscribe request */
        if (ICF_SIC_EVENT_SUBS_REQ == p_sic_pdb->event)
        {

            if (0 == icf_port_strcmp(((icf_sic_subs_req_st
                                    *)(p_sic_pdb->p_msg))->event_package.str,(icf_uint8_t *)"reg"))
            {
                /* if event package is reg then identify it is application triigered
                 * or reg triggered subscription.
                 */
                 if((ICF_DUMMY_APP_ID != p_subs_req->app_id) && 
                    (ICF_NULL == p_subs_req->p_key))
                 {
                     /* subs key will be NULL in the case of either application triggered 
                      * Subscription request for reg event package or user initiated deregistartion
                      */  
                     /* app_id will be ICF_DUMMY_APP_ID in case of user initiated deregistration
                      * therefore this code leg will be hit when it is application triggered
                      * subscription request
                      */
                     ret_val = icf_dbm_alloc_sic_cntxt_blk(
                                   p_sic_pdb->p_glb_pdb,
                                   &(((icf_sic_subs_req_st *)(p_sic_pdb->p_msg))->call_id),
                                   &(p_sic_pdb->p_sic_ctx),
                                   p_sic_pdb->p_ecode);
                 }
                 else
                 {
                     /* if event package is reg then IPTK need not to allocate sic
                      * context block here because it is alredy allocated in the 
                      * regm module.*/
                     ret_val = icf_dbm_find_sic_cntxt_blk(
                             p_sic_pdb->p_glb_pdb,
                             ((icf_sic_subs_req_st*)(p_sic_pdb->p_msg))->call_id,
                             &(p_sic_pdb->p_sic_ctx),
                             p_sic_pdb->p_ecode);

                     /* Following if statement will be true when
                      * regm module has triggered sic on 
                      * receiving de-register from app, in this case no further
                      * proceedings need to be done in this function.*/   
                     if (ICF_NULL == ((icf_sic_subs_req_st*)(p_sic_pdb->p_msg))->p_key)
                     {
                         p_sic_pdb->line_id = ((icf_sic_subs_req_st *)(p_sic_pdb->p_msg))->line_id;
                         return ret_val;
                     }
                 }
            }
            else
            {
               /* if event package is not reg then allocate sic context block
                * as per earlier handling.*/ 
               ret_val = icf_dbm_alloc_sic_cntxt_blk(
                    p_sic_pdb->p_glb_pdb,
                    &(((icf_sic_subs_req_st *)(p_sic_pdb->p_msg))->call_id),
                    &(p_sic_pdb->p_sic_ctx),
                    p_sic_pdb->p_ecode);
            }
       
            
			if (ICF_SUCCESS == ret_val)
			{
                p_sic_pdb->p_sic_ctx->method = ICF_SUBSCRIBE_REQ;
                /* If it is XCONF case then set bitmask and extract call ctx */
		        if(p_sic_pdb->p_internal_msg->msg_hdr.msg_id == 
                        ICF_INTERNAL_SUBSCRIBE_REQ)
		         {
		            p_sic_pdb->p_sic_ctx->bitmask |= 
                        ICF_SIC_INTERNAL_SUBSCRIBE_REQUEST;
                    p_sic_pdb->p_sic_ctx->p_call_ctx =
                     p_sic_pdb->p_internal_msg->msg_hdr.p_glb_pdb->p_call_ctx;

		         }
				ret_val = icf_sic_populate_context(p_sic_pdb);
            
				/* context is deallocated as it is possible that memory is 
				 * allocated to some pointers in the SIC ctx and failure
				 * took place after that
				 */
				if (ICF_FAILURE == ret_val)
				{
					p_sic_pdb->p_internal_msg->msg_hdr.msg_id = ICF_SIC_SUBSCRIPTION_STATUS;
				        icf_sic_send_failure_response_to_app(p_sic_pdb);
                    return ret_val;
				}

				icf_port_memcpy( 
					(icf_void_t*)&(p_sic_pdb->p_sic_ctx->dest_addr),
					(icf_void_t*) &( ( (icf_sic_subs_req_st *)(p_sic_pdb->p_msg))->destination_info),
					(icf_uint32_t) ICF_PORT_SIZEOF(icf_address_st) );
					
				
				icf_port_memcpy((icf_void_t*)(&(p_sic_pdb->p_sic_ctx->event_package)),
					(icf_void_t*)
						(&((icf_sic_subs_req_st *)(p_sic_pdb->p_msg))->event_package),
						(icf_uint32_t)ICF_PORT_SIZEOF(icf_string_st));

				p_sic_pdb->line_id = 
					((icf_sic_subs_req_st *)(p_sic_pdb->p_msg))->line_id;
				p_sic_pdb->p_sic_ctx->line_id = 
					((icf_sic_subs_req_st *)(p_sic_pdb->p_msg))->line_id;

                                /*Assigning line_id to global pdb*/
                                p_sic_pdb->p_glb_pdb->line_id = p_sic_pdb->p_sic_ctx->line_id;

				p_sic_pdb->p_sic_ctx->call_id = 
					((icf_sic_subs_req_st *)(p_sic_pdb->p_msg))->call_id;

				p_sic_pdb->p_sic_ctx->app_id = 
					((icf_sic_subs_req_st *)(p_sic_pdb->p_msg))->app_id;

				p_sic_pdb->p_sic_ctx->p_app_ctx = 
					((icf_sic_subs_req_st *)(p_sic_pdb->p_msg))->p_key;

				p_sic_pdb->p_sic_ctx->duration = 
					((icf_sic_subs_req_st *)(p_sic_pdb->p_msg))->duration;

				p_sic_pdb->p_sic_ctx->app_requested_duration = 
					((icf_sic_subs_req_st *)(p_sic_pdb->p_msg))->duration;

        if (ICF_SUB_REQ_EXPIRES_HDR_NOT_TO_BE_SEND  & 
            ((icf_sic_subs_req_st *)(p_sic_pdb->p_msg))->bitmask)
        {
            p_sic_pdb->p_sic_ctx->bitmask |= ICF_SIC_EXPIRES_HDR_NOT_TO_BE_SEND;
        }
        if (ICF_SUB_REQ_RESUBSCRIPTION_NOT_REQUIRED   & 
            ((icf_sic_subs_req_st *)(p_sic_pdb->p_msg))->bitmask)
        {
            p_sic_pdb->p_sic_ctx->bitmask |= ICF_SIC_RESUBSCRIPTION_NOT_REQUIRED;
        }
        
        /* Fix for SPR: 19999 (Merged CSR 1-7467002 : SPR 19863)*/
        /* Populate the subscription head start duration in sic context
         * if it has been received in API ICF_SUBSCRIBE_REQ.
         */
        /* The duration of subs_hs_duration is taken in seconds from
         * application and stored internally in milliseconds ; this is
         * done as this field is also present in set_line_params, where also
         * the value is taken in seconds from application.*/
        if (ICF_SUB_REQ_SUBS_HEAD_START_DURATION_PRESENT == 
                (ICF_SUB_REQ_SUBS_HEAD_START_DURATION_PRESENT & p_subs_req->bitmask))
        {
            p_sic_pdb->p_sic_ctx->bitmask |=
                ICF_SIC_SUBS_HEAD_START_DURATION_RECVD;
            p_sic_pdb->p_sic_ctx->subs_hs_duration = 
                (p_subs_req->subs_head_start_duration) * 1000;
        }
        
        /* Fix for SPR: 19999 (Merged CSR 1-7615966 : SPR 20030)*/
        /* set the bitmask ICF_SIC_RETRY_NOT_REQUIRED if the bitmask
         * ICF_SUB_REQ_RETRY_NOT_REQUIRED is set by application in API
         * ICF_SUBSCRIBE_REQ.
         */
        if (ICF_SUB_REQ_RETRY_NOT_REQUIRED & 
            ((icf_sic_subs_req_st *)(p_sic_pdb->p_msg))->bitmask)
        {
            p_sic_pdb->p_sic_ctx->bitmask |= ICF_SIC_SUBS_RETRY_NOT_REQUIRED;
        }
        /* set the bitmask ICF_SIC_RETRY_NOT_REQUIRED_FOR_3XX if the bitmask
         * ICF_SUB_REQ_RETRY_NOT_REQUIRED_FOR_3XX is set by application in API
         * ICF_SUBSCRIBE_REQ.
         */
        if (ICF_SUB_REQ_RETRY_NOT_REQUIRED_FOR_3XX & 
            ((icf_sic_subs_req_st *)(p_sic_pdb->p_msg))->bitmask)
        {
            p_sic_pdb->p_sic_ctx->bitmask |= 
                                  ICF_SIC_SUBS_RETRY_NOT_REQUIRED_FOR_3XX;
        }

        /*
           We will operate upon the p_preferred_identity field only if the
           bitmask is set.
         */
        if(ICF_PREF_IDENTITY_PRESENT ==
                (ICF_PREF_IDENTITY_PRESENT & p_subs_req->bitmask))
        {
            /*
               Bit mask is set.Proceed to determine the address type
               of the prefered identity
             */

            if(ICF_FAILURE == icf_cmn_parse_transport_addr(
                        p_sic_pdb->p_glb_pdb,
                        &(p_subs_req->pref_identity),
                        p_sic_pdb->p_ecode))
            {
                ret_val = ICF_FAILURE;
            }
            else
                /* address type has been successfully determined */
            {
                if((ICF_ADDRESS_PLAN_SIP_URI == p_subs_req->pref_identity.addr_type) ||
                        (ICF_ADDRESS_PLAN_TEL_URI == p_subs_req->pref_identity.addr_type) ||
                        (ICF_ADDRESS_PLAN_E164 == p_subs_req->pref_identity.addr_type) ||
                        (ICF_ADDRESS_PLAN_SIPS_URI == p_subs_req->pref_identity.addr_type))  	
                {
                    p_sic_pdb->p_sic_ctx->p_preferred_identity = 
                        icf_cmn_memget(
							p_sic_pdb->p_glb_pdb,
							sizeof(icf_address_st),
							ICF_MEM_COMMON,
							p_sic_pdb->p_ecode);
											
					if (  p_sic_pdb->p_sic_ctx->p_preferred_identity )
					{
					icf_port_memcpy(
						p_sic_pdb->p_sic_ctx->p_preferred_identity,
                        &(p_subs_req->pref_identity),
                        sizeof(icf_address_st) );
					}
            /*  Merging start of CSR 1-5433369  */

		    /*If the application has configured the P-preferred-Identity, 
             * then IPTK should send that in FROM header.
             * else check for self address on the line data 
             */ 
            if ( p_sic_pdb->p_sic_ctx->p_preferred_identity && (ICF_SUCCESS == ret_val))
            {    
		        icf_port_memcpy(
	                (icf_void_t *)(p_sic_pdb->p_sic_ctx->
        		    p_user_address),
	                (icf_void_t *)(p_sic_pdb->
	   	        p_sic_ctx->p_preferred_identity),
    		    (icf_uint32_t)
		        ICF_PORT_SIZEOF(icf_address_st));
            }   
		/*  Merging End    */

				}/* end of SIP_URI/TEL/E164 handling*/
				else
				{
                    /* Address type is UNDEFINED.*/
                    ret_val = ICF_FAILURE;
                }

            }/* address type has been successfully determined */    

        }/* end of Bit mask is set..... */   
       /*  Merging start of CSR 1-5433369  */ 
    else
        /*  Merging End    */
	{
 
        if(ICF_NULL == p_sic_pdb->p_sic_ctx->p_user_address)
        {
 		    p_sic_pdb->p_sic_ctx->p_user_address = 
			icf_cmn_memget(
				p_sic_pdb->p_glb_pdb,
				sizeof(icf_address_st),
				ICF_MEM_COMMON,
				p_sic_pdb->p_ecode);
	    }

        /* to identify the trigger is from Reg module, check for existence of bitmask
         * instead of system level configuration check for reg event pcks subs suppport
         */ 
        if(ICF_SIC_AUTO_SUBSCRIPTION_FOR_REG_EVENT ==
                   (ICF_SIC_AUTO_SUBSCRIPTION_FOR_REG_EVENT & p_sic_pdb->p_sic_ctx->bitmask))    
        {
            /* For reg event package, destination info is being copied into user address
             * because it is the default public uri.*/
			 if ( p_sic_pdb->p_sic_ctx->p_user_address )
			 {
            icf_port_memcpy(p_sic_pdb->p_sic_ctx->p_user_address,
                            &(((icf_sic_subs_req_st *)(p_sic_pdb->p_msg))->destination_info),
                            sizeof(icf_address_st));
			}	
        }
        else
        {        
            /*Merging start of CSR 1-5433369  */
            ret_val = icf_dbm_get_preferred_identity(
                    p_sic_pdb->p_glb_pdb,
                    p_sic_pdb->p_sic_ctx->line_id,
                    p_sic_pdb->p_sic_ctx->p_user_address,
                    p_sic_pdb->p_ecode);
           /*  Merging End    */
        }  
    }    
        if(ICF_SUCCESS == ret_val)
        {

/* End: Multiple Public Users ICF5.0*/
            /* clone the header list into sic ctx, if received from app */
            if (ICF_NULL != ((icf_sic_subs_req_st *)(p_sic_pdb->p_msg))->header_list.count)
            {
                ret_val = icf_cmn_clone_header_list(
                        p_sic_pdb->p_glb_pdb,
                        &( ((icf_sic_subs_req_st *)(p_sic_pdb->p_msg))->header_list),
                        &(p_sic_pdb->p_sic_ctx->header_list),
                        p_sic_pdb->p_ecode);
                ret_val = icf_cmn_clone_header_list(
                        p_sic_pdb->p_glb_pdb,
                        &( ((icf_sic_subs_req_st *)(p_sic_pdb->p_msg))->header_list),
                        &(p_sic_pdb->p_sic_ctx->persistent_header_list),
                        p_sic_pdb->p_ecode);
            }
            else
            {
                p_sic_pdb->p_sic_ctx->header_list.count = 0;
                p_sic_pdb->p_sic_ctx->persistent_header_list.count = 0;
            }


            /* clone the tag list into sic ctx, if received from app */
            if (ICF_NULL != ((icf_sic_subs_req_st *)(p_sic_pdb->p_msg))->tag_list.count)
            {
                ret_val = icf_cmn_clone_tag_list(
                        p_sic_pdb->p_glb_pdb,
                        &( ((icf_sic_subs_req_st *)(p_sic_pdb->p_msg))->tag_list),
                        &(p_sic_pdb->p_sic_ctx->tag_list),
                        p_sic_pdb->p_ecode);
            }
            else
                p_sic_pdb->p_sic_ctx->tag_list.count = 0;

            /* clone the body list into sic ctx, if received from app */
            if (ICF_NULL != ((icf_sic_subs_req_st *)(p_sic_pdb->p_msg))->body_list.count)
            {
                ret_val = icf_cmn_clone_body_list(
                        p_sic_pdb->p_glb_pdb,
                        &( ((icf_sic_subs_req_st *)(p_sic_pdb->p_msg))->body_list),
                        &(p_sic_pdb->p_sic_ctx->body_list),
                        p_sic_pdb->p_ecode);
            }
            else
                p_sic_pdb->p_sic_ctx->body_list.count = 0;
        }
        else
        {
            p_sic_pdb->p_internal_msg->msg_hdr.msg_id = ICF_SIC_SUBSCRIPTION_STATUS;
            p_sic_pdb->p_sic_ctx->error_code = ICF_CAUSE_PUBLIC_URI_BARRED;
            /* removed ret_val for icf_sic_send_failure_response_to_app
               as it overrides the previous FAILURE value - SPR14531 */
            icf_sic_send_failure_response_to_app(p_sic_pdb);
        }
            }
        }
        else
        {
            p_sic_pdb->valid_event = ICF_FALSE;            
        }

#ifdef ICF_LOAD_STAT
        if(ICF_SUCCESS == ret_val)
        {
       /*     p_sic_pdb->p_glb_pdb->load_data.subscribe_req++;*/
        }
#endif

    }/* if (NULL sic_ctx..*/

	else
	{/* Non null sic_ctx */
		ret_val = icf_sic_refresh_sic_ctx(p_sic_pdb);
#ifdef ICF_LOAD_STAT
        if(ICF_SUCCESS == ret_val)
        {
            p_sic_pdb->p_glb_pdb->load_data.subscribe_refresh++;
        }
#endif

	}

    /* Asynchronous NOTIFY Support:
       Copy the request received from the peer into p_pending_req field
       of SIC context.While control is in ICF_SIC_05_AWAIT_NOTIFY_CFM
       state,this request will be marked as pending and will be
       processed when NOTIFY confirm is received from the application.
    */ 
    if(((ICF_SIC_EVENT_SUBS_REQ == p_sic_pdb->event) ||
       (ICF_SIC_EVENT_UNSUB_REQ == p_sic_pdb->event)) &&
       (ICF_NULL != p_sic_pdb->p_config_data) &&
       (ICF_TRUE == p_sic_pdb->p_config_data->async_notify_support_reqd))
    {
        /* 
           Make a copy of the request only if the current state in SIC 
           context is 05.
        */   
        if((ICF_NULL != p_sic_pdb->p_sic_ctx) &&
           (ICF_SIC_05_AWAIT_NOTIFY_CFM == p_sic_pdb->p_sic_ctx->sic_state))
        {
            /* Free previously acquired memory for p_pending_req */
            if(ICF_NULL != p_sic_pdb->p_sic_ctx->p_pending_req)
            {
                ICF_MEMFREE(p_sic_pdb->p_glb_pdb,
                            p_sic_pdb->p_sic_ctx->p_pending_req,
                            ICF_MEM_COMMON,
                            p_sic_pdb->p_ecode,
                            ret_val)
            }

            /* Allocate memory for p_pending_req */
            ICF_MEMGET(p_sic_pdb->p_glb_pdb,
                       ICF_PORT_SIZEOF(icf_sic_subs_req_st),
                       ICF_MEM_COMMON,
                       p_sic_pdb->p_sic_ctx->p_pending_req,
                       ICF_RET_ON_FAILURE,
                       p_sic_pdb->p_ecode,
                       ret_val)

            /* Make a copy of the request received */    
	    	icf_port_memcpy(p_sic_pdb->p_sic_ctx->p_pending_req,
		                    p_sic_pdb->p_msg,
        	    		    ICF_PORT_SIZEOF(icf_sic_subs_req_st));
        }
    } 

    ICF_FUNCTION_EXIT(p_sic_pdb->p_glb_pdb)
    /* Returns failure if the function icf_sic_populate_context returns
     * failure or there is a failure by DBM to allocate SIC ctx block 
     */        
    return ret_val;
}



/*******************************************************************************
 *
 * FUNCTION:        icf_sic_process_options_req
 *
 * DESCRIPTION:     This function processes the options request.  
 *                  
 *
 ******************************************************************************/
icf_return_t icf_sic_process_options_req(
        INOUT icf_sic_pdb_st      *p_sic_pdb)

{
    icf_return_t              ret_val = ICF_SUCCESS;
    icf_sic_options_req_st   *p_options_req = ICF_NULL;
    icf_boolean_t             in_dialog_options = ICF_FALSE;
	

    ICF_FUNCTION_ENTER(p_sic_pdb->p_glb_pdb)

    p_options_req = (icf_sic_options_req_st *)(p_sic_pdb->p_msg);
  
        
    if (ICF_SUCCESS == ret_val)
    {
        
        ret_val = icf_sic_populate_context(p_sic_pdb);
        
        /* context is deallocated as it is possible that memory is 
         * allocated to some pointers in the SIC ctx and failure
         * took place after that
         */
        if (ICF_FAILURE == ret_val)
        {
            p_sic_pdb->p_internal_msg->msg_hdr.msg_id = ICF_SIC_OPTION_STATUS;
            icf_sic_send_failure_response_to_app(p_sic_pdb);

            return ret_val;
        }

        icf_port_memcpy( 
             (icf_void_t*)&(p_sic_pdb->p_sic_ctx->dest_addr),
             (icf_void_t*) &(((icf_sic_options_req_st *)(p_sic_pdb->p_msg))->destination_info),
             (icf_uint32_t) ICF_PORT_SIZEOF(icf_address_st) );


        p_sic_pdb->line_id = p_options_req->line_id;
        p_sic_pdb->p_sic_ctx->line_id = p_options_req->line_id;
        p_sic_pdb->p_sic_ctx->call_id = p_options_req->call_id;
        p_sic_pdb->p_sic_ctx->app_id = p_options_req->app_id;

       /*Assigning line_id to global pdb*/
       p_sic_pdb->p_glb_pdb->line_id = p_sic_pdb->p_sic_ctx->line_id;
            /*
               We will operate upon the p_preferred_identity field only if the
               bitmask is set.
             */
            if(ICF_PREF_IDENTITY_PRESENT ==
                    (ICF_PREF_IDENTITY_PRESENT & p_options_req->bit_mask))
            {
                /*
                 Bit mask is set.Proceed to determine the address type
                 of the prefered identity
                 */
               if(ICF_FAILURE == icf_cmn_parse_transport_addr(
                      p_sic_pdb->p_glb_pdb,
                      &(p_options_req->pref_identity),
                      p_sic_pdb->p_ecode))
                {
                    ret_val = ICF_FAILURE;
                }
                else
                /* address type has been successfully determined */
                {
                    if((ICF_ADDRESS_PLAN_SIP_URI == p_options_req->pref_identity.
                                addr_type) || (ICF_ADDRESS_PLAN_TEL_URI == 
                                    p_options_req->pref_identity.addr_type) ||
                   (ICF_ADDRESS_PLAN_E164 == p_options_req->pref_identity.
                    addr_type) ||
                    (ICF_ADDRESS_PLAN_SIPS_URI == p_options_req->pref_identity.addr_type))
                    {
                        p_sic_pdb->p_sic_ctx->p_preferred_identity = 
                            icf_cmn_memget(
                                    p_sic_pdb->p_glb_pdb,
                                    sizeof(icf_address_st),
                                    ICF_MEM_COMMON,
                                    p_sic_pdb->p_ecode);
                        if ( p_sic_pdb->p_sic_ctx->p_preferred_identity ) 
						{
						icf_port_memcpy(
                                p_sic_pdb->p_sic_ctx->p_preferred_identity,
                                &(p_options_req->pref_identity),
                               sizeof(icf_address_st) );
						}
                    }/* end of SIP_URI/TEL/E164 handling*/
                    else
                    {
                        /* Address type is UNDEFINED.*/
                        ret_val = ICF_FAILURE;
                    }
                }/* address type has been successfully determined */    

            }/* end of Bit mask is set..... */
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
                in_dialog_options = ICF_TRUE;
            }
            if(ICF_SUCCESS == ret_val)
            {
                ret_val = icf_dbm_get_preferred_identity(
                        p_sic_pdb->p_glb_pdb,
                        p_sic_pdb->p_sic_ctx->line_id,
                        p_sic_pdb->p_sic_ctx->p_user_address,
                        p_sic_pdb->p_ecode);

                /* End: Multiple Public Users ICF5.0*/
                /* clone the header list into sic ctx, if received from app */
                if (ICF_NULL != p_options_req->header_list.count)
                {
                    ret_val = icf_cmn_clone_header_list(
                            p_sic_pdb->p_glb_pdb,
                            &(p_options_req->header_list),
                            &(p_sic_pdb->p_sic_ctx->header_list),
							p_sic_pdb->p_ecode);
                }

                /* clone the tag list into sic ctx, if received from app */
                if (ICF_NULL != p_options_req->tag_list.count)
                {
                    ret_val = icf_cmn_clone_tag_list(
                            p_sic_pdb->p_glb_pdb,
                            &(p_options_req->tag_list),
                            &(p_sic_pdb->p_sic_ctx->tag_list),
                            p_sic_pdb->p_ecode);
                }

                /* clone the body list into sic ctx, if received from app */
                if (ICF_NULL != p_options_req->body_list.count)
                {
                    ret_val = icf_cmn_clone_body_list(
                                    p_sic_pdb->p_glb_pdb,
                                    &(p_options_req->body_list),
                                    &(p_sic_pdb->p_sic_ctx->body_list),
                                    p_sic_pdb->p_ecode);
                }
                /* assign the boolean here */
                p_sic_pdb->is_option_in_dlg = in_dialog_options;
               
                /* Fix for SPR 20412: CSR 1-8223807 merged*/
                /* Process the remove_hdr_list in sic context if the 
                 * count is not 0 i.e. if any header name is present in 
                 * remove_hdr_list.
                 */
                if((ICF_FALSE == in_dialog_options) &&
                   (ICF_NULL != p_options_req->remove_hdr_list.count))
                {
                    /* This is the size of required memory to store the
                     * headers as string buffer concatenated by <sp><comma>.
                     * Since max size of each haeder can be ICF_MAX_STR_LEN
                     * and each header will be separated by <sp><comma>, for
                     * each header in string buffer (ICF_MAX_STR_LEN + 2) bytes
                     * required.
                     */
                    icf_uint16_t required_buff_size =
                      (p_options_req->remove_hdr_list.count)*(ICF_MAX_STR_LEN + 2);

                    p_sic_pdb->p_sic_ctx->p_remove_hdrs = 
                            icf_cmn_memget(
                                    p_sic_pdb->p_glb_pdb,
                                    required_buff_size,
                                    ICF_MEM_COMMON,
                                    p_sic_pdb->p_ecode);
                    
                    /* Convert the remove_hdr_list into string buffer and save
                     * it into p_sic_ctx->p_remove_hdrs.
                     */
                    icf_cmn_convert_hdr_name_list_into_str_buffer(
                              &(p_sic_pdb->p_sic_ctx->p_remove_hdrs),
                              &(p_options_req->remove_hdr_list));
                }
                /*  REl 8.1
                 *  if tunnel mode is present in ICF_OPTIONS_REQ
                 *  API Then save it into sic_ctx 
                 */ 
                if(ICF_SIC_OPTIONS_TUNNEL_MODE_OPERATION_PRESENT == 
                   (ICF_SIC_OPTIONS_TUNNEL_MODE_OPERATION_PRESENT & 
                    p_options_req->bit_mask))
                {
                    p_sic_pdb->p_sic_ctx->tunnel_mode = 
                              p_options_req->tunnel_mode;
                }
                   

                if (ICF_FAILURE == ret_val)
                {
                    /* do nothing */
                }
                else if (ICF_FAILURE == icf_sic_send_options_req(p_sic_pdb))
                {
                    p_sic_pdb->p_internal_msg->msg_hdr.msg_id = ICF_SIC_OPTION_STATUS;
                    icf_sic_send_failure_response_to_app(p_sic_pdb);
                    ret_val = ICF_FAILURE;
                }
                else
                {
                    if(in_dialog_options == ICF_TRUE)
                    {
                        /* mark in CC context that we have send 
                           the In-dialog option successfully */
                        p_sic_pdb->p_sic_ctx->p_call_ctx->common_bitmask |= 
                                            ICF_IN_DLG_OPTIONS_SEND;
                    }
                }

            }
            else
            {
                p_sic_pdb->p_internal_msg->msg_hdr.msg_id = ICF_SIC_OPTION_STATUS;

                if ( ICF_TRUE == in_dialog_options)
                {
                    p_sic_pdb->p_sic_ctx->error_code = 
                         ICF_CAUSE_IN_DIALOG_OTG_OPTIONS_RECEIVED;
                }
                else
                {
                    p_sic_pdb->p_sic_ctx->error_code = ICF_CAUSE_INVALID_ADDRESS_RECEIVED;
                }	

                icf_sic_send_failure_response_to_app(p_sic_pdb);
            }
    }
    ICF_FUNCTION_EXIT(p_sic_pdb->p_glb_pdb)
    return ret_val;
}
/*******************************************************************************
 *
 * FUNCTION:        icf_sic_process_publish_req
 *
 * DESCRIPTION:     This function processes the PUBLISH request.  
 *                  
 *
 ******************************************************************************/
icf_return_t icf_sic_process_publish_req(
        INOUT icf_sic_pdb_st      *p_sic_pdb)

{
    icf_return_t				ret_val = ICF_SUCCESS;
    icf_sic_publish_req_st   *p_publish_req = ICF_NULL;
    icf_line_data_st *p_line_blk = ICF_NULL;


    ICF_FUNCTION_ENTER(p_sic_pdb->p_glb_pdb)

    p_publish_req = (icf_sic_publish_req_st *)(p_sic_pdb->p_msg);

    ret_val = icf_dbm_alloc_sic_cntxt_blk(
            p_sic_pdb->p_glb_pdb,
            &(p_publish_req->call_id),
            &(p_sic_pdb->p_sic_ctx),
            p_sic_pdb->p_ecode);


    if (ICF_SUCCESS == ret_val)
    {

        ret_val = icf_sic_populate_context(p_sic_pdb);

        /* context is deallocated as it is possible that memory is 
         * allocated to some pointers in the SIC ctx and failure
         * took place after that
         */
        if (ICF_FAILURE == ret_val)
        {
            p_sic_pdb->p_internal_msg->msg_hdr.msg_id = ICF_SIC_PUBLISH_STATUS;
            p_sic_pdb->line_id = p_publish_req->line_id;
            p_sic_pdb->p_sic_ctx->line_id = p_publish_req->line_id;
            p_sic_pdb->p_sic_ctx->app_id = p_publish_req->app_id;
            /*Assigning line_id to global pdb*/
            p_sic_pdb->p_glb_pdb->line_id = p_sic_pdb->p_sic_ctx->line_id;
            ret_val = icf_sic_send_failure_response_to_app(p_sic_pdb);
            return ret_val;
        }
        else if(ICF_FAILURE ==
                icf_cmn_validate_open_nw_server(p_sic_pdb->p_glb_pdb,p_publish_req->line_id))
        {
            p_sic_pdb->line_id = p_publish_req->line_id;
            p_sic_pdb->p_sic_ctx->line_id = p_publish_req->line_id;
            p_sic_pdb->p_sic_ctx->app_id = p_publish_req->app_id;
            p_sic_pdb->p_sic_ctx->error_code = ICF_CAUSE_OPEN_NW_SERVER_FAILED;
            p_sic_pdb->p_internal_msg->msg_hdr.msg_id = ICF_SIC_PUBLISH_STATUS;
            /*Assigning line_id to global pdb*/
            p_sic_pdb->p_glb_pdb->line_id = p_sic_pdb->p_sic_ctx->line_id;
            ret_val = icf_sic_send_failure_response_to_app(p_sic_pdb);
            return ret_val;
        }

        else if (ICF_USER_ADDRESS_PRESENT ==
                (ICF_USER_ADDRESS_PRESENT & p_publish_req->bit_mask))
        {
            if (ICF_SUCCESS == icf_dbm_fetch_line_cntxt_blk(p_sic_pdb->p_glb_pdb,
                        p_publish_req->line_id,
                        &p_line_blk,
                        p_sic_pdb->p_ecode))
            {
                if (ICF_FAILURE == icf_dbm_check_if_user_exits_in_line(
                            p_sic_pdb->p_glb_pdb,
                            &p_publish_req->user_address,
                            p_line_blk,
                            (icf_boolean_t) ICF_TRUE,
                            p_sic_pdb->p_ecode))
                {

                    ret_val = ICF_FAILURE;
                    p_sic_pdb->line_id = p_publish_req->line_id;
                    p_sic_pdb->p_sic_ctx->line_id = p_publish_req->line_id;
                    p_sic_pdb->p_sic_ctx->app_id = p_publish_req->app_id;
                    p_sic_pdb->p_sic_ctx->error_code = ICF_USER_NOT_ADDED;
                    p_sic_pdb->p_internal_msg->msg_hdr.msg_id = ICF_SIC_PUBLISH_STATUS;
                    /*Assigning line_id to global pdb*/
                    p_sic_pdb->p_glb_pdb->line_id = p_sic_pdb->p_sic_ctx->line_id;
                    ret_val = icf_sic_send_failure_response_to_app(p_sic_pdb);
                    return ret_val;
                }
            }
        }

        icf_port_memcpy( 
                (icf_void_t*)&(p_sic_pdb->p_sic_ctx->dest_addr),
                (icf_void_t*) &( ( (icf_sic_publish_req_st *)(p_sic_pdb->p_msg))->destination_info),
                (icf_uint32_t) ICF_PORT_SIZEOF(icf_address_st) );



        p_sic_pdb->p_sic_ctx->duration = p_publish_req->duration;
        /* Added for SPR14543 Event Header is Mandatory.*/
        /*If not received then return failure           */
        if(ICF_NULL == p_publish_req->event_package.str_len)

        {
            p_sic_pdb->p_internal_msg->msg_hdr.msg_id = ICF_SIC_PUBLISH_STATUS;

            p_sic_pdb->line_id = p_publish_req->line_id;
            p_sic_pdb->p_sic_ctx->line_id = p_publish_req->line_id;
            p_sic_pdb->p_sic_ctx->app_id = p_publish_req->app_id;
            p_sic_pdb->p_sic_ctx->error_code = ICF_CAUSE_MANDATORY_HDR_NOT_RCVD;
            /*Assigning line_id to global pdb*/
            p_sic_pdb->p_glb_pdb->line_id = p_sic_pdb->p_sic_ctx->line_id;
            ret_val = icf_sic_send_failure_response_to_app(p_sic_pdb);
        }
        else
        {

           p_sic_pdb->p_sic_ctx->event_package = p_publish_req->event_package;
#ifdef ICF_TRACE_ENABLE
           p_sic_pdb->event_package = p_publish_req->event_package; 
#endif
            p_sic_pdb->line_id = p_publish_req->line_id;
            p_sic_pdb->p_sic_ctx->line_id = p_publish_req->line_id;
            p_sic_pdb->p_sic_ctx->app_id = p_publish_req->app_id;

            /*Assigning line_id to global pdb*/
            p_sic_pdb->p_glb_pdb->line_id = p_sic_pdb->p_sic_ctx->line_id;

            /*
               We will operate upon the p_preferred_identity field only if the
               bitmask is set.
             */
            if(ICF_PREF_IDENTITY_PRESENT ==
                    (ICF_PREF_IDENTITY_PRESENT & p_publish_req->bit_mask))
            {
                /*
                   Bit mask is set.Proceed to determine the address type
                   of the prefered identity
                 */

                if(ICF_FAILURE == icf_cmn_parse_transport_addr(
                            p_sic_pdb->p_glb_pdb,
                            &(p_publish_req->pref_identity),
                            p_sic_pdb->p_ecode))
                {
                    ret_val = ICF_FAILURE;
                }
                else
                    /* address type has been successfully determined */
                {
                    if((ICF_ADDRESS_PLAN_SIP_URI == p_publish_req->pref_identity.
                                addr_type) || (ICF_ADDRESS_PLAN_TEL_URI == 
                                    p_publish_req->pref_identity.addr_type) ||
                            (ICF_ADDRESS_PLAN_E164 == 
                             p_publish_req->pref_identity.addr_type) ||
                             (ICF_ADDRESS_PLAN_SIPS_URI == p_publish_req->pref_identity.addr_type))
                   {
                        p_sic_pdb->p_sic_ctx->p_preferred_identity = 
                            icf_cmn_memget(
                                    p_sic_pdb->p_glb_pdb,
                                    sizeof(icf_address_st),
                                    ICF_MEM_COMMON,
                                    p_sic_pdb->p_ecode);
						if ( p_sic_pdb->p_sic_ctx->p_preferred_identity )
						{
                        icf_port_memcpy(
                                p_sic_pdb->p_sic_ctx->p_preferred_identity,
                                &(p_publish_req->pref_identity),
                               sizeof(icf_address_st) );
						}
                   }/* end of SIP_URI/TEL/E164 handling*/
                   else
                   {
                       /* Address type is UNDEFINED.*/
                       ret_val = ICF_FAILURE;
                   }

               }/* address type has been successfully determined */    

            }/* end of Bit mask is set..... */    
            if(ICF_USER_ADDRESS_PRESENT ==
                    (ICF_USER_ADDRESS_PRESENT & p_publish_req->bit_mask))
            {
                /*
                   Bit mask is set.Proceed to determine the address type
                   of the user address
                 */

                if(ICF_FAILURE == icf_cmn_parse_transport_addr(
                            p_sic_pdb->p_glb_pdb,
                            &(p_publish_req->user_address),
                            p_sic_pdb->p_ecode))
                {
                    ret_val = ICF_FAILURE;
                }
                else
                    /* address type has been successfully determined */
                {
                    if((ICF_ADDRESS_PLAN_SIP_URI == p_publish_req->user_address.
                                addr_type) || (ICF_ADDRESS_PLAN_TEL_URI == 
                                    p_publish_req->user_address.addr_type) ||
                            (ICF_ADDRESS_PLAN_E164 == 
                             p_publish_req->user_address.addr_type))
                    {
                      if( ICF_NULL == p_sic_pdb->p_sic_ctx->p_user_address)
                       {
                         p_sic_pdb->p_sic_ctx->p_user_address = 
                             icf_cmn_memget(
                                     p_sic_pdb->p_glb_pdb,
                                     sizeof(icf_address_st),
                                     ICF_MEM_COMMON,
                                     p_sic_pdb->p_ecode);
                        }
						if ( p_sic_pdb->p_sic_ctx->p_user_address )
						{
                        icf_port_memcpy(
                                p_sic_pdb->p_sic_ctx->p_user_address,
                                &(p_publish_req->user_address),
                                sizeof(icf_address_st) );
						}
                    }/* end of SIP_URI/TEL/E164 handling*/
                    else
                    {
                        /* Address type is UNDEFINED.*/
                        ret_val = ICF_FAILURE;
                    }

                }/* address type has been successfully determined */    

            }/* end of Bit mask is set..... */
            else
            {
                ret_val = icf_dbm_get_preferred_identity(
                        p_sic_pdb->p_glb_pdb,
                        p_sic_pdb->p_sic_ctx->line_id,
                        p_sic_pdb->p_sic_ctx->p_user_address,
                        p_sic_pdb->p_ecode);
            }

            if(ICF_SUCCESS == ret_val)
            {
                /* End: Multiple Public Users ICF5.0*/
                /* clone the header list into sic ctx, if received from app */
                if (ICF_NULL != p_publish_req->header_list.count)
                {
                    ret_val = icf_cmn_clone_header_list(
                            p_sic_pdb->p_glb_pdb,
                            &(p_publish_req->header_list),
                            &(p_sic_pdb->p_sic_ctx->header_list),
                            p_sic_pdb->p_ecode);
                }

                /* clone the tag list into sic ctx, if received from app */
                if (ICF_NULL != p_publish_req->tag_list.count)
                {
                    ret_val = icf_cmn_clone_tag_list(
                            p_sic_pdb->p_glb_pdb,
                            &(p_publish_req->tag_list),
                            &(p_sic_pdb->p_sic_ctx->tag_list),
                            p_sic_pdb->p_ecode);
                }

                /* clone the body list into sic ctx, if received from app */
                if (ICF_NULL != p_publish_req->body_list.count)
                {
                    ret_val = icf_cmn_clone_body_list(
                            p_sic_pdb->p_glb_pdb,
                            &(p_publish_req->body_list),
                            &(p_sic_pdb->p_sic_ctx->body_list),
                            p_sic_pdb->p_ecode);
                }
				 /* Application trace for PUBLISH request received from application*/
                 ICF_SIC_APP_TRACE(p_sic_pdb,ICF_TRACE_OTG_PUBLISH)

                if (ICF_FAILURE == icf_sic_send_publish_req(p_sic_pdb))
                {
                    p_sic_pdb->p_internal_msg->msg_hdr.msg_id = ICF_SIC_PUBLISH_STATUS;                              
                    /* free the list in sic ctx */
                    if (ICF_NULL != p_sic_pdb->p_sic_ctx->header_list.count)
                    {    
                        ret_val = icf_cmn_free_header_list(p_sic_pdb->p_glb_pdb,
                                &(p_sic_pdb->p_sic_ctx->header_list),
                                p_sic_pdb->p_ecode);
                    }

                    if (ICF_NULL != p_sic_pdb->p_sic_ctx->tag_list.count)
                    {    
                        ret_val = icf_cmn_free_tag_list(p_sic_pdb->p_glb_pdb,
                                &(p_sic_pdb->p_sic_ctx->tag_list),
                                p_sic_pdb->p_ecode);
                    }

                    if (ICF_NULL != p_sic_pdb->p_sic_ctx->body_list.count)
                    {    
                        ret_val = icf_cmn_free_msg_body_list(p_sic_pdb->p_glb_pdb,
                                &(p_sic_pdb->p_sic_ctx->body_list),
                                p_sic_pdb->p_ecode);
                    }
                    ret_val = icf_sic_send_failure_response_to_app(p_sic_pdb);

                }

            }
            else
            {
                p_sic_pdb->p_internal_msg->msg_hdr.msg_id = ICF_SIC_PUBLISH_STATUS;

                p_sic_pdb->p_sic_ctx->error_code = ICF_USER_NOT_ADDED;
                ret_val = icf_sic_send_failure_response_to_app(p_sic_pdb);
            }

	     }
    }  
   

	ICF_FUNCTION_EXIT(p_sic_pdb->p_glb_pdb)
    return ret_val;
}


/*******************************************************************************
 *
 * FUNCTION:        icf_sic_process_message_req
 *
 * DESCRIPTION:     This function processes the options request.  
 *                  
 *
 ******************************************************************************/
icf_return_t icf_sic_process_message_req(
        INOUT icf_sic_pdb_st      *p_sic_pdb)

{
    icf_return_t			ret_val = ICF_SUCCESS;
	icf_sic_message_req_st  *p_message_req = ICF_NULL;
    icf_line_data_st        *p_line_blk = ICF_NULL;
    
    ICF_FUNCTION_ENTER(p_sic_pdb->p_glb_pdb)


	p_message_req = (icf_sic_message_req_st *)(p_sic_pdb->p_msg);
  
	ret_val = icf_dbm_alloc_sic_cntxt_blk(
                p_sic_pdb->p_glb_pdb,
                &(p_message_req->call_id),
                &(p_sic_pdb->p_sic_ctx),
                p_sic_pdb->p_ecode);
    
        
    if (ICF_SUCCESS == ret_val)
    {
        
        ret_val = icf_sic_populate_context(p_sic_pdb);
        
        /* context is deallocated as it is possible that memory is 
         * allocated to some pointers in the SIC ctx and failure
         * took place after that
         */
        if (ICF_FAILURE == ret_val)
        {
			p_sic_pdb->p_internal_msg->msg_hdr.msg_id = ICF_SIC_MESSAGE_STATUS;
			ret_val = icf_sic_send_failure_response_to_app(p_sic_pdb);
            return ret_val;
        }
		else if(ICF_FAILURE == icf_cmn_validate_open_nw_server(p_sic_pdb->p_glb_pdb,p_message_req->line_id))
		{
			p_sic_pdb->line_id = p_message_req->line_id;
			p_sic_pdb->p_sic_ctx->line_id = p_message_req->line_id;
			p_sic_pdb->p_sic_ctx->call_id = p_message_req->call_id;
			p_sic_pdb->p_sic_ctx->app_id = p_message_req->app_id;
			p_sic_pdb->p_sic_ctx->error_code = ICF_CAUSE_OPEN_NW_SERVER_FAILED;
			p_sic_pdb->p_internal_msg->msg_hdr.msg_id = ICF_SIC_MESSAGE_STATUS;
                        /*Assigning line_id to global pdb*/
                        p_sic_pdb->p_glb_pdb->line_id = p_sic_pdb->p_sic_ctx->line_id;
			ret_val = icf_sic_send_failure_response_to_app(p_sic_pdb);
			return ret_val;
		}

        else if (ICF_USER_ADDRESS_PRESENT ==
                (ICF_USER_ADDRESS_PRESENT & p_message_req->bit_mask))
        {
            if (ICF_SUCCESS == icf_dbm_fetch_line_cntxt_blk(p_sic_pdb->p_glb_pdb,
                        p_message_req->line_id,
                        &p_line_blk,
                        p_sic_pdb->p_ecode))
            {
                if (ICF_FAILURE == icf_dbm_check_if_user_exits_in_line(
                            p_sic_pdb->p_glb_pdb,
                            &p_message_req->user_address,
                            p_line_blk,
                            (icf_boolean_t) ICF_TRUE,
                            p_sic_pdb->p_ecode))
                {

                    ret_val = ICF_FAILURE;
                    p_sic_pdb->line_id = p_message_req->line_id;
                    p_sic_pdb->p_sic_ctx->line_id = p_message_req->line_id;
                    p_sic_pdb->p_sic_ctx->app_id = p_message_req->app_id;
                    p_sic_pdb->p_sic_ctx->error_code = ICF_USER_NOT_ADDED;
                    p_sic_pdb->p_internal_msg->msg_hdr.msg_id = ICF_SIC_MESSAGE_STATUS;
                    /*Assigning line_id to global pdb*/
                    p_sic_pdb->p_glb_pdb->line_id = p_sic_pdb->p_sic_ctx->line_id;
                    ret_val = icf_sic_send_failure_response_to_app(p_sic_pdb);
                    return ret_val;
                }
            }
        }

		icf_port_memcpy( 
					(icf_void_t*)&(p_sic_pdb->p_sic_ctx->dest_addr),
					(icf_void_t*) &( ( (icf_sic_message_req_st *)(p_sic_pdb->p_msg))->destination_info),
					(icf_uint32_t) ICF_PORT_SIZEOF(icf_address_st) );

		p_sic_pdb->line_id = p_message_req->line_id;
		p_sic_pdb->p_sic_ctx->line_id = p_message_req->line_id;
		p_sic_pdb->p_sic_ctx->call_id = p_message_req->call_id;
		p_sic_pdb->p_sic_ctx->app_id = p_message_req->app_id;
                /*Assigning line_id to global pdb*/
                p_sic_pdb->p_glb_pdb->line_id = p_sic_pdb->p_sic_ctx->line_id;

        /*
           We will operate upon the p_preferred_identity field only if the
           bitmask is set.
         */
        if(ICF_PREF_IDENTITY_PRESENT ==
                (ICF_PREF_IDENTITY_PRESENT & p_message_req->bit_mask))
        {
            /*
               Bit mask is set.Proceed to determine the address type
               of the prefered identity
             */

            if(ICF_FAILURE == icf_cmn_parse_transport_addr(
                        p_sic_pdb->p_glb_pdb,
                        &(p_message_req->pref_identity),
                        p_sic_pdb->p_ecode))
            {
                ret_val = ICF_FAILURE;
            }
            else
                /* address type has been successfully determined */
            {
                if((ICF_ADDRESS_PLAN_SIP_URI == p_message_req->pref_identity.
                            addr_type) || (ICF_ADDRESS_PLAN_TEL_URI == 
                                p_message_req->pref_identity.addr_type) ||
                        (ICF_ADDRESS_PLAN_E164 == 
                         p_message_req->pref_identity.addr_type) ||
                         (ICF_ADDRESS_PLAN_SIPS_URI == p_message_req->pref_identity.addr_type)) 
                {
                    p_sic_pdb->p_sic_ctx->p_preferred_identity = 
                        icf_cmn_memget(
                                p_sic_pdb->p_glb_pdb,
                                sizeof(icf_address_st),
                                ICF_MEM_COMMON,
                                p_sic_pdb->p_ecode);
					if (p_sic_pdb->p_sic_ctx->p_preferred_identity)
					{
                    icf_port_memcpy(
                            p_sic_pdb->p_sic_ctx->p_preferred_identity,
                            &(p_message_req->pref_identity),
                            sizeof(icf_address_st) );
					}
                }/* end of SIP_URI/TEL/E164 handling*/
                else
                {
                    /* Address type is UNDEFINED.*/
                    ret_val = ICF_FAILURE;
                }

            }/* address type has been successfully determined */    

        }/* end of Bit mask is set..... */
            
        if(ICF_USER_ADDRESS_PRESENT ==
        (ICF_USER_ADDRESS_PRESENT & p_message_req->bit_mask))
        {
            /*
               Bit mask is set.Proceed to determine the address type
               of the prefered identity
             */

            if(ICF_FAILURE == icf_cmn_parse_transport_addr(
                        p_sic_pdb->p_glb_pdb,
                        &(p_message_req->user_address),
                        p_sic_pdb->p_ecode))
            {
                ret_val = ICF_FAILURE;
            }
            else
                /* address type has been successfully determined */
            {
                if((ICF_ADDRESS_PLAN_SIP_URI == p_message_req->user_address.
                            addr_type) || (ICF_ADDRESS_PLAN_TEL_URI == 
                                p_message_req->user_address.addr_type) ||
                        (ICF_ADDRESS_PLAN_E164 == 
                         p_message_req->user_address.addr_type))
                {
                  if(ICF_NULL ==  p_sic_pdb->p_sic_ctx->p_user_address)
                   {
                     p_sic_pdb->p_sic_ctx->p_user_address = 
                         icf_cmn_memget(
                                 p_sic_pdb->p_glb_pdb,
                                 sizeof(icf_address_st),
                                 ICF_MEM_COMMON,
                                 p_sic_pdb->p_ecode);
                    }
					if ( p_sic_pdb->p_sic_ctx->p_user_address )
					{
                    icf_port_memcpy(
                            p_sic_pdb->p_sic_ctx->p_user_address,
                            &(p_message_req->user_address),
                            sizeof(icf_address_st) );
					}
                }/* end of SIP_URI/TEL/E164 handling*/
                else
                {
                    /* Address type is UNDEFINED.*/
                    ret_val = ICF_FAILURE;
                }

            }/* address type has been successfully determined */    

        }/* end of Bit mask is set..... */
        else
        {
            ret_val = icf_dbm_get_preferred_identity(
                    p_sic_pdb->p_glb_pdb,
                    p_sic_pdb->p_sic_ctx->line_id,
                    p_sic_pdb->p_sic_ctx->p_user_address,
                    p_sic_pdb->p_ecode);
        }

		if(ICF_SUCCESS == ret_val)
		{
            /* End: Multiple Public Users ICF5.0*/
			/* clone the header list into sic ctx, if received from app */
			if (ICF_NULL != p_message_req->header_list.count)
			{
				ret_val = icf_cmn_clone_header_list(
							p_sic_pdb->p_glb_pdb,
							&(p_message_req->header_list),
							&(p_sic_pdb->p_sic_ctx->header_list),
							p_sic_pdb->p_ecode);
			}

			/* clone the tag list into sic ctx, if received from app */
			if (ICF_NULL != p_message_req->tag_list.count)
			{
				ret_val = icf_cmn_clone_tag_list(
							p_sic_pdb->p_glb_pdb,
							&(p_message_req->tag_list),
							&(p_sic_pdb->p_sic_ctx->tag_list),
							p_sic_pdb->p_ecode);
			}

			/* clone the body list into sic ctx, if received from app */
			if (ICF_NULL != p_message_req->body_list.count)
			{
				ret_val = icf_cmn_clone_body_list(
							p_sic_pdb->p_glb_pdb,
							&(p_message_req->body_list),
							&(p_sic_pdb->p_sic_ctx->body_list),
							p_sic_pdb->p_ecode);
			}
			/* Application trace for MESSAGE request received from application*/
            ICF_SIC_APP_TRACE(p_sic_pdb,ICF_TRACE_OTG_MESSAGE)


            if (ICF_FAILURE == ret_val)
            {
                /* do nothing */
            }
			
            else if (ICF_FAILURE == icf_sic_send_message_req(p_sic_pdb))
            {
			     p_sic_pdb->p_internal_msg->msg_hdr.msg_id = ICF_SIC_MESSAGE_STATUS;
                            /* free the list in sic ctx */
                            if (ICF_NULL != p_sic_pdb->p_sic_ctx->header_list.count)
                             {   
                                 ret_val = icf_cmn_free_header_list(p_sic_pdb->p_glb_pdb,
                                                        &(p_sic_pdb->p_sic_ctx->header_list),
                                                        p_sic_pdb->p_ecode);
                             }

                            if (ICF_NULL != p_sic_pdb->p_sic_ctx->tag_list.count)
                             {   
                                 ret_val = icf_cmn_free_tag_list(p_sic_pdb->p_glb_pdb,
                                                        &(p_sic_pdb->p_sic_ctx->tag_list),
                                                        p_sic_pdb->p_ecode);
                             }

                            if (ICF_NULL != p_sic_pdb->p_sic_ctx->body_list.count)
                             {   
                                 ret_val = icf_cmn_free_msg_body_list(p_sic_pdb->p_glb_pdb,
                                                        &(p_sic_pdb->p_sic_ctx->body_list),
                                                        p_sic_pdb->p_ecode);
                             }
                            ret_val = icf_sic_send_failure_response_to_app(p_sic_pdb);

            }
		}
		else
		{
			p_sic_pdb->p_internal_msg->msg_hdr.msg_id = 
				ICF_SIC_MESSAGE_STATUS;

			p_sic_pdb->p_sic_ctx->error_code = ICF_USER_NOT_ADDED;
			ret_val = icf_sic_send_failure_response_to_app(p_sic_pdb);
		}
	}

    ICF_FUNCTION_EXIT(p_sic_pdb->p_glb_pdb)
    return ret_val;
}


/*******************************************************************************
 *
 * FUNCTION:        icf_sic_process_refer_req
 *
 * DESCRIPTION:     This function processes the REFER request.  
 *                  
 *
 ******************************************************************************/
icf_return_t icf_sic_process_refer_req(
        INOUT icf_sic_pdb_st      *p_sic_pdb)


{
    icf_return_t				ret_val = ICF_SUCCESS;
	icf_sic_refer_req_st   *p_refer_req = ICF_NULL;
	icf_user_db_node_st		*p_user_db = ICF_NULL;
	
    
    ICF_FUNCTION_ENTER(p_sic_pdb->p_glb_pdb)


	p_refer_req = (icf_sic_refer_req_st *)(p_sic_pdb->p_msg);
  
	/*Check if there is already an existing SIC context block for*/

	/*this call id. This will happen in case application wants to*/

	/*send multiple in-call REFERs*/


	icf_dbm_find_sic_cntxt_blk(
            p_sic_pdb->p_glb_pdb,
            p_refer_req->call_id,
            &(p_sic_pdb->p_sic_ctx),
            p_sic_pdb->p_ecode);


	if( p_sic_pdb->p_sic_ctx )
	{
		/*Free the existing SIC context block*/

		icf_sic_dealloc_context(p_sic_pdb);	
	}
    

	/* Allocate a fresh SIC context block*/

		ret_val = icf_dbm_alloc_sic_cntxt_blk(
                p_sic_pdb->p_glb_pdb,
                &(p_refer_req->call_id),
                &(p_sic_pdb->p_sic_ctx),
                p_sic_pdb->p_ecode);
        
    if (ICF_SUCCESS == ret_val)
    {
        
        ret_val = icf_sic_populate_context(p_sic_pdb);
        
        /* context is deallocated as it is possible that memory is 
         * allocated to some pointers in the SIC ctx and failure
         * took place after that
         */
        if (ICF_FAILURE == ret_val)
        {
			p_sic_pdb->p_internal_msg->msg_hdr.msg_id = ICF_SIC_REFER_STATUS;
			ret_val = icf_sic_send_failure_response_to_app(p_sic_pdb);
            return ret_val;
        }
        else if(ICF_FAILURE ==
                icf_cmn_validate_open_nw_server(p_sic_pdb->p_glb_pdb,p_refer_req->line_id))
        {
   			p_sic_pdb->line_id = p_refer_req->line_id;
		    p_sic_pdb->p_sic_ctx->line_id = p_refer_req->line_id;
		    p_sic_pdb->p_sic_ctx->app_id = p_refer_req->app_id;
		    p_sic_pdb->p_sic_ctx->call_id = p_refer_req->call_id;
            p_sic_pdb->p_sic_ctx->error_code = ICF_CAUSE_OPEN_NW_SERVER_FAILED;
            p_sic_pdb->p_internal_msg->msg_hdr.msg_id = ICF_SIC_REFER_STATUS;
            /*Assigning line_id to global pdb*/
            p_sic_pdb->p_glb_pdb->line_id = p_sic_pdb->p_sic_ctx->line_id;
            ret_val = icf_sic_send_failure_response_to_app(p_sic_pdb);
            return ret_val;
        }

		icf_port_memcpy( 
					(icf_void_t*)&(p_sic_pdb->p_sic_ctx->dest_addr),
					(icf_void_t*) &( ( (icf_sic_refer_req_st *)(p_sic_pdb->p_msg))->destination_info),
					(icf_uint32_t) ICF_PORT_SIZEOF(icf_address_st) );

		p_sic_pdb->p_sic_ctx->subs_reqd = p_refer_req->subs_reqd;

		p_sic_pdb->line_id = p_refer_req->line_id;
		p_sic_pdb->p_sic_ctx->line_id = p_refer_req->line_id;
		p_sic_pdb->p_sic_ctx->app_id = p_refer_req->app_id;
		p_sic_pdb->p_sic_ctx->call_id = p_refer_req->call_id;
	/*	p_sic_pdb->p_sic_ctx->event_package = ICF_EVENT_PKG_REFER;*/
                /*Assigning line_id to global pdb*/
                p_sic_pdb->p_glb_pdb->line_id = p_sic_pdb->p_sic_ctx->line_id;

		icf_port_strcpy( p_sic_pdb->p_sic_ctx->event_package.str, (icf_uint8_t *)"refer");
		
	
		if( p_refer_req->pref_identity.addr_type != ICF_ADDRESS_PLAN_UNDEFINED )
		{
			p_sic_pdb->p_sic_ctx->p_preferred_identity = 
				icf_cmn_memget(
					p_sic_pdb->p_glb_pdb,
					sizeof(icf_address_st),
					ICF_MEM_COMMON,
					p_sic_pdb->p_ecode);
			if ( p_sic_pdb->p_sic_ctx->p_preferred_identity )
			{
			icf_port_memcpy(
					p_sic_pdb->p_sic_ctx->p_preferred_identity,
					&(p_refer_req->pref_identity),
					sizeof(icf_address_st) );
			}
/* Start: Multiple Public Users ICF5.0*/
/*If the pref_identity is not the registered one in IMS MODE set
  the value of ret_val as failure*/
ICF_CHECK_IMS_START(p_sic_pdb->p_glb_pdb,ICF_TRUE)
			ret_val = icf_dbm_get_public_uri_node_from_uri_for_line(
						p_sic_pdb->p_glb_pdb,
						p_refer_req->line_id,
						&(p_refer_req->pref_identity),
						ICF_USER_DB_USE_ASSOCIATED_URIS,
						&p_user_db,
						p_sic_pdb->p_ecode);

			if ((ICF_FAILURE == ret_val) || 
				((ICF_SUCCESS == ret_val) && 
				(ICF_NULL == p_user_db->p_contact_uri_list)))
			{
				ret_val = ICF_FAILURE;		
			}
ICF_CHECK_IMS_END
#ifdef NON_IMS_CLIENT
ICF_CHECK_IMS_START(p_sic_pdb->p_glb_pdb,ICF_FALSE)
			ret_val = icf_dbm_get_public_uri_node_from_uri_for_line(
						p_sic_pdb->p_glb_pdb,
						p_refer_req->line_id,
						&(p_refer_req->pref_identity),
						ICF_USER_DB_USE_PUBLIC_URIS,
						&p_user_db,
						p_sic_pdb->p_ecode);
ICF_CHECK_IMS_END
#endif
		}
		else
		{
			p_sic_pdb->p_sic_ctx->p_preferred_identity = 
				icf_cmn_memget(
					p_sic_pdb->p_glb_pdb,
					sizeof(icf_address_st),
					ICF_MEM_COMMON,
					p_sic_pdb->p_ecode);

			if (p_sic_pdb->p_sic_ctx->p_preferred_identity) 
			{
				ret_val = icf_dbm_get_preferred_identity(
					p_sic_pdb->p_glb_pdb,
                    p_sic_pdb->p_sic_ctx->line_id,
                    p_sic_pdb->p_sic_ctx->p_preferred_identity,
					p_sic_pdb->p_ecode);
			}

		}

		if(ICF_NULL != p_sic_pdb->p_sic_ctx->p_preferred_identity)
		{
			icf_port_memcpy(
					(icf_void_t *)(p_sic_pdb->p_sic_ctx->
						p_user_address),
					(icf_void_t *)(p_sic_pdb->
						p_sic_ctx->p_preferred_identity),
					(icf_uint32_t)
						ICF_PORT_SIZEOF(icf_address_st));
		}
/* End: Multiple Public Users ICF5.0*/

		if(ICF_SUCCESS == ret_val)
		{
		/* SPR13957 */	
			if((p_refer_req->refer_to.addr_type != ICF_ADDRESS_PLAN_UNDEFINED ) 
                    && (ICF_NULL != p_refer_req->refer_to.addr_val.str_len))
			{
				icf_port_memcpy(
						&(p_sic_pdb->p_sic_ctx->refer_to),
						&(p_refer_req->refer_to),
						sizeof(icf_address_st) );
  
        	    /* clone the header list into sic ctx, if received from app */
    			if (ICF_NULL != p_refer_req->header_list.count)
	    		{
					ret_val = icf_cmn_clone_header_list(
								p_sic_pdb->p_glb_pdb,
								&(p_refer_req->header_list),
								&(p_sic_pdb->p_sic_ctx->header_list),
								p_sic_pdb->p_ecode);
				}

				/* clone the tag list into sic ctx, if received from app */
				if (ICF_NULL != p_refer_req->tag_list.count)
				{
					ret_val = icf_cmn_clone_tag_list(
								p_sic_pdb->p_glb_pdb,
								&(p_refer_req->tag_list),
								&(p_sic_pdb->p_sic_ctx->tag_list),
								p_sic_pdb->p_ecode);
				}
		
				/* clone the body list into sic ctx, if received from app */
				if (ICF_NULL != p_refer_req->body_list.count)
				{
					ret_val = icf_cmn_clone_body_list(
								p_sic_pdb->p_glb_pdb,
								&(p_refer_req->body_list),
								&(p_sic_pdb->p_sic_ctx->body_list),
								p_sic_pdb->p_ecode);
				}
	
				/* Check if the request is within dialog */
				if(ICF_FAILURE == icf_dbm_find_call_cntxt_blk(
								p_sic_pdb->p_glb_pdb,
								p_refer_req->call_id,
								&(p_sic_pdb->p_sic_ctx->p_call_ctx),
								p_sic_pdb->p_ecode))
				{
					p_sic_pdb->p_sic_ctx->p_call_ctx = ICF_NULL;
				}
				else
				{
					p_sic_pdb->p_sic_ctx->bitmask |= ICF_SIC_REQUEST_WITHIN_DIALOG;
				}
                if( ICF_FAILURE != ret_val )
                {
                 ret_val = icf_sic_send_refer_req(p_sic_pdb);
                 if(ICF_FAILURE == ret_val)
                 {
                  p_sic_pdb->p_internal_msg->msg_hdr.msg_id = ICF_SIC_REFER_STATUS;
                   /* free the list in sic ctx */
                     if (ICF_NULL != p_sic_pdb->p_sic_ctx->header_list.count)
                     {
                         ret_val = icf_cmn_free_header_list(p_sic_pdb->p_glb_pdb,
                                                    &(p_sic_pdb->p_sic_ctx->header_list),
                                                    p_sic_pdb->p_ecode);
                     }

                     if (ICF_NULL != p_sic_pdb->p_sic_ctx->tag_list.count)
                     {
                         ret_val = icf_cmn_free_tag_list(p_sic_pdb->p_glb_pdb,
                                                    &(p_sic_pdb->p_sic_ctx->tag_list),
                                                    p_sic_pdb->p_ecode);
                     }

                     if (ICF_NULL != p_sic_pdb->p_sic_ctx->body_list.count)
                     {
                         ret_val = icf_cmn_free_msg_body_list(p_sic_pdb->p_glb_pdb,
                                                    &(p_sic_pdb->p_sic_ctx->body_list),
                                                    p_sic_pdb->p_ecode);
                     }

                  ret_val = icf_sic_send_failure_response_to_app(p_sic_pdb);
                 }
                } 
			}
            else
			{
				p_sic_pdb->p_internal_msg->msg_hdr.msg_id = ICF_SIC_REFER_STATUS;
                p_sic_pdb->p_sic_ctx->error_code = ICF_CAUSE_REFER_TO_ABSENT;
                ret_val = icf_sic_send_failure_response_to_app(p_sic_pdb);
            }

		}
		else
		{
			p_sic_pdb->p_internal_msg->msg_hdr.msg_id = ICF_SIC_REFER_STATUS;
			p_sic_pdb->p_sic_ctx->error_code = ICF_CAUSE_PUBLIC_URI_BARRED;
			ret_val = icf_sic_send_failure_response_to_app(p_sic_pdb);
        }
	}

	ICF_FUNCTION_EXIT(p_sic_pdb->p_glb_pdb)
    return ret_val;
}

/*******************************************************************************
 *
 * FUNCTION:        icf_sic_process_options_resp
 *
 * DESCRIPTION:     This function processes the options response.  
 *                  
 *
 ******************************************************************************/
icf_return_t icf_sic_process_options_resp(
        INOUT icf_sic_pdb_st      *p_sic_pdb)

{
    icf_return_t          ret_val = ICF_SUCCESS;
/*  icf_return_t          dummy_retval = ICF_SUCCESS;*/

/*  icf_line_id_t       line_id = 0;*/

    icf_nw_inc_opt_resp_st      *p_opt_resp=ICF_NULL;
    icf_sic_options_status_st	resp_msg;
    icf_return_t                dummy_retval = ICF_SUCCESS;
	
    
    ICF_FUNCTION_ENTER(p_sic_pdb->p_glb_pdb)


    p_opt_resp = (icf_nw_inc_opt_resp_st *)
                    (p_sic_pdb->p_msg);

    p_sic_pdb->p_sic_ctx = p_opt_resp->p_sic_ctx;
    ICF_ASSERT(p_sic_pdb->p_glb_pdb,
            (ICF_NULL != p_sic_pdb->p_sic_ctx))
        
   

    
    resp_msg.resp_code = p_opt_resp->response;
    
    /*  SPR 18064 Merge */
    if(ICF_SIC_AUTH_PARAM_NOT_RCVD &                
                  p_sic_pdb->p_sic_ctx->bitmask)      
    {            
        p_sic_pdb->p_sic_ctx->error_code =  
                   ICF_CAUSE_AUTH_PARAM_NOT_RCVD;     
    }   
    else if ( 0 == resp_msg.resp_code)
    {
        p_sic_pdb->p_sic_ctx->error_code = 
                   ICF_CAUSE_REMOTE_UNREACHABLE;
    }
    else if (401 == resp_msg.resp_code ||
         407 == resp_msg.resp_code)
    {
        if(ICF_NULL == p_sic_pdb->p_sic_ctx->error_code)
        {
           p_sic_pdb->p_sic_ctx->error_code =
			   ICF_CAUSE_INVALID_AUTH_PARAM;
        }
    }
    else if ( 300 <= resp_msg.resp_code )
    {
        p_sic_pdb->p_sic_ctx->error_code = 
			ICF_CAUSE_NETWORK_ERROR;
    }
#ifdef ICF_LOAD_STAT
    else
    {
        p_sic_pdb->p_glb_pdb->load_data.inc_options_success++;
    }
#endif
   
   if ( p_sic_pdb->p_sic_ctx->error_code == 
               ICF_CAUSE_REMOTE_UNREACHABLE ||
        p_sic_pdb->p_sic_ctx->error_code == 
               ICF_CAUSE_INVALID_AUTH_PARAM ||
        p_sic_pdb->p_sic_ctx->error_code == 
               ICF_CAUSE_INVALID_AUTH_RESP_STATUS)
    {
        icf_sic_send_delink_to_ssa(p_sic_pdb,
                    (icf_boolean_t)ICF_FALSE); \
    }
 
    p_sic_pdb->p_internal_msg->msg_hdr.msg_id =  
        ICF_SIC_OPTION_STATUS;

    resp_msg.p_key = p_sic_pdb->p_sic_ctx->p_app_ctx;
    resp_msg.line_id = p_sic_pdb->p_sic_ctx->line_id;
    resp_msg.call_id = p_sic_pdb->p_sic_ctx->call_id;
    resp_msg.app_id = p_sic_pdb->p_sic_ctx->app_id;
	
    resp_msg.header_list.hdr = ICF_NULL;
    resp_msg.header_list.count = 0;
    resp_msg.tag_list.tag = ICF_NULL;
    resp_msg.tag_list.count = 0;
    resp_msg.body_list.body = ICF_NULL;
    resp_msg.body_list.count = 0;


    if ((ICF_SUCCESS == ret_val) && 
        (ICF_NULL != p_sic_pdb->p_sic_ctx->header_list.count))
    {
        if (ICF_SUCCESS == icf_cmn_clone_header_list(
                           p_sic_pdb->p_glb_pdb,
                           &(p_sic_pdb->p_sic_ctx->header_list),
                           &(resp_msg.header_list),
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

    /* clone the body list */
    if ((ICF_SUCCESS == ret_val) && 
        (ICF_NULL != p_sic_pdb->p_sic_ctx->body_list.count))
    {
        if (ICF_SUCCESS == icf_cmn_clone_body_list(
                           p_sic_pdb->p_glb_pdb,
                           &(p_sic_pdb->p_sic_ctx->body_list),
                           &(resp_msg.body_list),
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

	
    p_sic_pdb->p_internal_msg->p_msg_data = 
            (icf_uint8_t *)&resp_msg;
    
    ret_val = icf_adm_map_sic_event_to_ext_event(p_sic_pdb);
    if( ICF_SUCCESS == ret_val)
    {
        ret_val = icf_adm_process_mesg(p_sic_pdb->p_internal_msg);
        
        if ( ICF_NULL != p_sic_pdb->p_internal_msg->p_msg_data)
        {
ICF_CHECK_TUNNEL_MODE_START(p_sic_pdb->p_glb_pdb, ICF_FALSE)

            icf_options_resp_st* p_options_resp = (icf_options_resp_st*)
               (((icf_msg_st*)(p_sic_pdb->p_internal_msg->p_msg_data))->payload);

            if ( ICF_NULL != p_options_resp && 
                 ICF_NULL != p_options_resp->header_list.count)
            {
                icf_cmn_free_header_list(p_sic_pdb->p_glb_pdb,
                &(p_options_resp->header_list),
                p_sic_pdb->p_ecode);
            }
            if ( ICF_NULL != p_options_resp &&
                 ICF_NULL != p_options_resp->body_list.count)
            {
                icf_cmn_free_msg_body_list(p_sic_pdb->p_glb_pdb,
                               &(p_options_resp->body_list),
                               p_sic_pdb->p_ecode);
            }
            /* Free the acquired memory for the list node */
            icf_sic_free_stream_capab_list(p_sic_pdb);	
ICF_CHECK_TUNNEL_MODE_END

ICF_CHECK_TUNNEL_MODE_START(p_sic_pdb->p_glb_pdb, ICF_TRUE)
            icf_tunnelled_options_resp_st* p_tunnel_options_resp = 
                         (icf_tunnelled_options_resp_st*)
                         (((icf_msg_st*)(p_sic_pdb->p_internal_msg->p_msg_data))->payload);
            /*  Free the header list */
            if ( ICF_NULL != p_tunnel_options_resp &&
                 ICF_NULL != p_tunnel_options_resp->header_list.count)
            {
                icf_cmn_free_header_list(p_sic_pdb->p_glb_pdb,
                      &(p_tunnel_options_resp->header_list),
                      p_sic_pdb->p_ecode);
            }
            /*  Free the body list */
            if ( ICF_NULL != p_tunnel_options_resp &&
                 ICF_NULL != p_tunnel_options_resp->body_list.count)
            {
                icf_cmn_free_msg_body_list(p_sic_pdb->p_glb_pdb,
                              &(p_tunnel_options_resp->body_list),
                              p_sic_pdb->p_ecode);
            }
ICF_CHECK_TUNNEL_MODE_END
        }
        ICF_MEMFREE(p_sic_pdb->p_glb_pdb, 
            p_sic_pdb->p_internal_msg->p_msg_data, ICF_MEM_COMMON,
            p_sic_pdb->p_ecode, dummy_retval)
        
    }
        
    /* Deallocate the SIC context block */

    if( p_sic_pdb->p_sic_ctx->p_preferred_identity)
    {
        ICF_MEMFREE(
               p_sic_pdb->p_glb_pdb,
               p_sic_pdb->p_sic_ctx->p_preferred_identity,
               ICF_MEM_COMMON,
               p_sic_pdb->p_ecode,
               ret_val)
    }
	
    icf_sic_dealloc_context(p_sic_pdb);
    ICF_FUNCTION_EXIT(p_sic_pdb->p_glb_pdb)
    
    return ret_val;
}


/*******************************************************************************
 *
 * FUNCTION:        icf_sic_process_publish_resp
 *
 * DESCRIPTION:     This function processes the publish response.  
 *                  
 *
 ******************************************************************************/
icf_return_t icf_sic_process_publish_resp(
        INOUT icf_sic_pdb_st      *p_sic_pdb)

{
    icf_return_t					ret_val = ICF_SUCCESS;
/*    icf_return_t					dummy_retval = ICF_SUCCESS;*/

/*    icf_line_id_t				line_id = 0;*/

	icf_nw_inc_publish_resp_st	*p_pub_resp=ICF_NULL;
	icf_sic_publish_status_st	resp_msg;
	icf_return_t				dummy_retval = ICF_SUCCESS;
    
    ICF_FUNCTION_ENTER(p_sic_pdb->p_glb_pdb)


	p_pub_resp = (icf_nw_inc_publish_resp_st *)
                    (p_sic_pdb->p_msg);
                
    p_sic_pdb->p_sic_ctx = p_pub_resp->p_sic_ctx;
    ICF_ASSERT(p_sic_pdb->p_glb_pdb,
            (ICF_NULL != p_sic_pdb->p_sic_ctx))
        

    /*  Fix Merged fpr CSR 1-6456839*/
    /*  SCENARIO : If Application sends PUBLISH req
     *             PUBLISH retransmits and remote does not respond
     *             then there was memmory in use of call object
     */
    resp_msg.resp_code = p_pub_resp->response;
	if ( 0 == resp_msg.resp_code)
    {
        p_sic_pdb->p_sic_ctx->error_code =
                    ICF_CAUSE_REMOTE_UNREACHABLE;
    }
    if ( p_sic_pdb->p_sic_ctx->error_code ==
                    ICF_CAUSE_REMOTE_UNREACHABLE)
    {
        icf_sic_send_delink_to_ssa(p_sic_pdb,
            (icf_boolean_t)ICF_FALSE); \
    }
    
	p_sic_pdb->p_internal_msg->msg_hdr.msg_id = 
        ICF_SIC_PUBLISH_STATUS;

    
    resp_msg.p_key = p_sic_pdb->p_sic_ctx->p_app_ctx;
	resp_msg.line_id = p_sic_pdb->p_sic_ctx->line_id;
	resp_msg.call_id = p_sic_pdb->p_sic_ctx->call_id;
	resp_msg.app_id = p_sic_pdb->p_sic_ctx->app_id;
	resp_msg.duration = p_pub_resp->duration;

	if (ICF_NULL != p_sic_pdb->p_sic_ctx->header_list.count)
	{
		ret_val = icf_cmn_clone_header_list(
					p_sic_pdb->p_glb_pdb,
					&(p_sic_pdb->p_sic_ctx->header_list),
					&(resp_msg.header_list),
					p_sic_pdb->p_ecode);
	}
	else
		resp_msg.header_list.count = 0;

	
    p_sic_pdb->p_internal_msg->p_msg_data = 
            (icf_uint8_t *)&resp_msg;
    
    /* Application trace for PUBLISH response sent to application*/
   ICF_SIC_APP_TRACE(p_sic_pdb, ICF_TRACE_OTG_PUBLISH_RESP)
    
	ret_val = icf_adm_map_sic_event_to_ext_event(p_sic_pdb);
	if( ICF_SUCCESS == ret_val)
	{
		ret_val = icf_adm_process_mesg(p_sic_pdb->p_internal_msg);
        
		if ( ICF_NULL != p_sic_pdb->p_internal_msg->p_msg_data)
		{
			icf_publish_resp_st* p_pub_ind = (icf_publish_resp_st*)
				(((icf_msg_st*)(p_sic_pdb->p_internal_msg->p_msg_data))->payload);
			if ( ICF_NULL != p_pub_ind && 
				ICF_NULL != p_pub_ind->header_list.count)
			{
				icf_cmn_free_header_list(p_sic_pdb->p_glb_pdb,
					&(p_pub_ind->header_list),
					p_sic_pdb->p_ecode);
			}
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

/*******************************************************************************
 *
 * FUNCTION:        icf_sic_process_message_resp
 *
 * DESCRIPTION:     This function processes the message response.  
 *                  
 *
 ******************************************************************************/
icf_return_t icf_sic_process_message_resp(
        INOUT icf_sic_pdb_st      *p_sic_pdb)

{
    icf_return_t					ret_val = ICF_SUCCESS;
/*    icf_return_t					dummy_retval = ICF_SUCCESS;*/

/*    icf_line_id_t				line_id = 0;*/

	icf_nw_inc_msg_resp_st		*p_msg_resp=ICF_NULL;
	icf_sic_message_status_st	resp_msg;
	icf_return_t			dummy_retval = ICF_SUCCESS;
    
    ICF_FUNCTION_ENTER(p_sic_pdb->p_glb_pdb)


	p_msg_resp = (icf_nw_inc_msg_resp_st *)
                    (p_sic_pdb->p_msg);
                
    p_sic_pdb->p_sic_ctx = p_msg_resp->p_sic_ctx;
    ICF_ASSERT(p_sic_pdb->p_glb_pdb,
            (ICF_NULL != p_sic_pdb->p_sic_ctx))
        
/*    ICF_SIC_TRACE(p_sic_pdb,ICF_TRACE_SIC_EVENT_PKG_RECD)*/


    

   
    /*  Fix Merged for CSR 1-6456839 */
    /*  SCENARIO : If Application sends MESSAGE req
     *             MESSAGE retransmits and remote does not respond
     *             then there was memmory in use of call object
     */
    resp_msg.resp_code = p_msg_resp->response;

     if ( 0 == resp_msg.resp_code)
     {
         p_sic_pdb->p_sic_ctx->error_code =
                        ICF_CAUSE_REMOTE_UNREACHABLE;
     }
    if ( p_sic_pdb->p_sic_ctx->error_code ==
                    ICF_CAUSE_REMOTE_UNREACHABLE)
    {
        icf_sic_send_delink_to_ssa(p_sic_pdb,
            (icf_boolean_t)ICF_FALSE); \
    }
 
    p_sic_pdb->p_internal_msg->msg_hdr.msg_id = 
        ICF_SIC_MESSAGE_STATUS;

    resp_msg.p_key = p_sic_pdb->p_sic_ctx->p_app_ctx;
	resp_msg.line_id = p_sic_pdb->p_sic_ctx->line_id;
	resp_msg.call_id = p_sic_pdb->p_sic_ctx->call_id;
	resp_msg.app_id = p_sic_pdb->p_sic_ctx->app_id;

	if (ICF_NULL != p_sic_pdb->p_sic_ctx->header_list.count)
	{
		ret_val = icf_cmn_clone_header_list(
					p_sic_pdb->p_glb_pdb,
					&(p_sic_pdb->p_sic_ctx->header_list),
					&(resp_msg.header_list),
					p_sic_pdb->p_ecode);
	}
	else
		resp_msg.header_list.count = 0;

	
    p_sic_pdb->p_internal_msg->p_msg_data = 
            (icf_uint8_t *)&resp_msg;

    /* Application trace for MESSAGE response sent to application*/
    ICF_SIC_APP_TRACE(p_sic_pdb,ICF_TRACE_OTG_MESSAGE_RESP)   
    
    ret_val = icf_adm_map_sic_event_to_ext_event(p_sic_pdb);
	if( ICF_SUCCESS == ret_val)
	{
		ret_val = icf_adm_process_mesg(p_sic_pdb->p_internal_msg);
        
		if ( ICF_NULL != p_sic_pdb->p_internal_msg->p_msg_data)
		{
			icf_message_resp_st* p_message_resp = (icf_message_resp_st*)
				(((icf_msg_st*)(p_sic_pdb->p_internal_msg->p_msg_data))->payload);
			if ( ICF_NULL != p_message_resp && 
				ICF_NULL != p_message_resp->header_list.count)
			{
				icf_cmn_free_header_list(p_sic_pdb->p_glb_pdb,
					&(p_message_resp->header_list),
					p_sic_pdb->p_ecode);
			}
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

/*******************************************************************************
 *
 * FUNCTION:        icf_sic_process_new_message_ind
 *
 * DESCRIPTION:     This function processes the message ind from SSA.  
 *                  
 *
 ******************************************************************************/
icf_return_t icf_sic_process_new_message_ind(
        INOUT icf_sic_pdb_st      *p_sic_pdb)

{
    icf_return_t             ret_val = ICF_SUCCESS;
    icf_return_t             return_val = ICF_SUCCESS;
    icf_nw_inc_msg_ind_st    *p_msg_ind=ICF_NULL;
    icf_sic_msg_ind_st       msg_ind = {0,0,0,ICF_NULL,{0,0},{0,0},{0,{0,{0}},0}};
    icf_return_t             dummy_retval = ICF_SUCCESS;

    ICF_FUNCTION_ENTER(p_sic_pdb->p_glb_pdb)


    p_msg_ind = (icf_nw_inc_msg_ind_st *) (p_sic_pdb->p_msg);

    /* If p_msg_ind is NULL, return without further processing */
    ICF_CHECK_IF_VAR_IS_NULL(p_sic_pdb->p_glb_pdb, p_msg_ind, return_val)
    if(ICF_FAILURE == return_val)
    {
        ret_val = ICF_FAILURE;
        ICF_FUNCTION_EXIT(p_sic_pdb->p_glb_pdb)
        return ret_val;
    }

    p_sic_pdb->p_sic_ctx = p_msg_ind->p_sic_ctx;
    ICF_ASSERT(p_sic_pdb->p_glb_pdb,
            (ICF_NULL != p_sic_pdb->p_sic_ctx))

    /*Send the message response to SSA ,If Asychronus MESSAGE support not enabled */
    if((ICF_NULL != p_sic_pdb->p_config_data) &&
        (ICF_FALSE == p_sic_pdb->p_config_data->async_message_support_reqd))
    {
        icf_sic_send_message_resp(p_sic_pdb);
    }


    p_sic_pdb->p_internal_msg->msg_hdr.msg_id = 
        ICF_SIC_NEW_MESSAGE_IND;


    msg_ind.call_id = p_msg_ind->p_sic_ctx->call_id;

    msg_ind.p_key = p_sic_pdb->p_sic_ctx->p_app_ctx;
    msg_ind.line_id = p_sic_pdb->p_sic_ctx->line_id;
    msg_ind.app_id = p_sic_pdb->p_sic_ctx->app_id;

    if (ICF_NULL != p_sic_pdb->p_sic_ctx->header_list.count)
    {
        ret_val = icf_cmn_clone_header_list(
                p_sic_pdb->p_glb_pdb,
                &(p_sic_pdb->p_sic_ctx->header_list),
                &(msg_ind.header_list),
                p_sic_pdb->p_ecode);
    }
    else
        msg_ind.header_list.count = 0;

    if (ICF_NULL != p_sic_pdb->p_sic_ctx->body_list.count)
    {
        ret_val = icf_cmn_clone_body_list(
                p_sic_pdb->p_glb_pdb,
                &(p_sic_pdb->p_sic_ctx->body_list),
                &(msg_ind.body_list),
                p_sic_pdb->p_ecode);
    }
    else
        msg_ind.body_list.count = 0;


    p_sic_pdb->p_internal_msg->p_msg_data = 
        (icf_uint8_t *)&msg_ind;

    /* send trace to application for message indication */
    ICF_SIC_APP_TRACE(p_sic_pdb, ICF_TRACE_INC_MESSAGE)

        ret_val = icf_adm_map_sic_event_to_ext_event(p_sic_pdb);
    if( ICF_SUCCESS == ret_val)
    {
        ret_val = icf_adm_process_mesg(p_sic_pdb->p_internal_msg);

        if ( ICF_NULL != p_sic_pdb->p_internal_msg->p_msg_data)
        {
            icf_message_ind_st* p_message_ind = (icf_message_ind_st*)
                (((icf_msg_st*)(p_sic_pdb->p_internal_msg->p_msg_data))->payload);
            if ( ICF_NULL != p_message_ind && 
                    ICF_NULL != p_message_ind->header_list.count)
            {
                icf_cmn_free_header_list(p_sic_pdb->p_glb_pdb,
                        &(p_message_ind->header_list),
                        p_sic_pdb->p_ecode);
            }
            if( ICF_NULL != p_message_ind && 
                    ICF_NULL != p_message_ind->body_list.count)
            {
                icf_cmn_free_msg_body_list(p_sic_pdb->p_glb_pdb,
                        &(p_message_ind->body_list),
                        p_sic_pdb->p_ecode);
            }
        }
        ICF_MEMFREE(p_sic_pdb->p_glb_pdb, 
                p_sic_pdb->p_internal_msg->p_msg_data, ICF_MEM_COMMON,
                p_sic_pdb->p_ecode, dummy_retval)

    }
    if((ICF_NULL != p_sic_pdb->p_config_data) &&
        (ICF_FALSE == p_sic_pdb->p_config_data->async_message_support_reqd))
    {
        /* If Asychronus MESSAGE support not enabled 
         * Deallocate the SIC context block */
        icf_sic_dealloc_context(p_sic_pdb);
    }
    ICF_FUNCTION_EXIT(p_sic_pdb->p_glb_pdb)
        return ret_val;
}

/*******************************************************************************
 *
 * FUNCTION:        icf_sic_process_refer_resp
 *
 * DESCRIPTION:     This function processes the refer response.  
 *                  
 *
 ******************************************************************************/
icf_return_t icf_sic_process_refer_resp(
        INOUT icf_sic_pdb_st      *p_sic_pdb)

{
    icf_return_t					ret_val = ICF_SUCCESS;
 /*   icf_return_t					dummy_retval = ICF_SUCCESS;*/

 /*   icf_line_id_t				line_id = 0;*/

	icf_nw_inc_refer_resp_st		*p_refer_resp=ICF_NULL;
	icf_sic_refer_status_st		resp_msg;
	icf_return_t					dummy_retval = ICF_SUCCESS;
    
    ICF_FUNCTION_ENTER(p_sic_pdb->p_glb_pdb)


	p_refer_resp = (icf_nw_inc_refer_resp_st *)
                    (p_sic_pdb->p_msg);
                
    p_sic_pdb->p_sic_ctx = p_refer_resp->p_sic_ctx;
    ICF_ASSERT(p_sic_pdb->p_glb_pdb,
            (ICF_NULL != p_sic_pdb->p_sic_ctx))
        
/*    ICF_SIC_TRACE(p_sic_pdb,ICF_TRACE_SIC_EVENT_PKG_RECD)*/


    
	p_sic_pdb->p_internal_msg->msg_hdr.msg_id = 
        ICF_SIC_REFER_STATUS;

    
    resp_msg.resp_code = p_refer_resp->response;
    resp_msg.p_key = p_sic_pdb->p_sic_ctx->p_app_ctx;
	resp_msg.line_id = p_sic_pdb->p_sic_ctx->line_id;
	resp_msg.call_id = p_sic_pdb->p_sic_ctx->call_id;
	resp_msg.app_id = p_sic_pdb->p_sic_ctx->app_id;

	if (ICF_NULL != p_sic_pdb->p_sic_ctx->header_list.count)
	{
		ret_val = icf_cmn_clone_header_list(
					p_sic_pdb->p_glb_pdb,
					&(p_sic_pdb->p_sic_ctx->header_list),
					&(resp_msg.header_list),
					p_sic_pdb->p_ecode);

		icf_cmn_free_header_list(p_sic_pdb->p_glb_pdb,
							&(p_sic_pdb->p_sic_ctx->header_list),
							p_sic_pdb->p_ecode);
	}
	else
		resp_msg.header_list.count = 0;

	
    p_sic_pdb->p_internal_msg->p_msg_data = 
            (icf_uint8_t *)&resp_msg;
    
    ret_val = icf_adm_map_sic_event_to_ext_event(p_sic_pdb);
	if( ICF_SUCCESS == ret_val)
	{
		ret_val = icf_adm_process_mesg(p_sic_pdb->p_internal_msg);
        
		if ( ICF_NULL != p_sic_pdb->p_internal_msg->p_msg_data)
		{
			icf_refer_resp_st* p_refer_resp = (icf_refer_resp_st*)
				(((icf_msg_st*)(p_sic_pdb->p_internal_msg->p_msg_data))->payload);
			if ( ICF_NULL != p_refer_resp && 
				ICF_NULL != p_refer_resp->header_list.count)
			{
				icf_cmn_free_header_list(p_sic_pdb->p_glb_pdb,
					&(p_refer_resp->header_list),
					p_sic_pdb->p_ecode);
			}
		}
		ICF_MEMFREE(p_sic_pdb->p_glb_pdb, 
			p_sic_pdb->p_internal_msg->p_msg_data, ICF_MEM_COMMON,
			p_sic_pdb->p_ecode, dummy_retval)
	}
        
    /* if the request is outside dialog, deallocate the SIC context block */

	if( !(p_sic_pdb->p_sic_ctx->bitmask & ICF_SIC_REQUEST_WITHIN_DIALOG)  && 
        !(p_sic_pdb->p_sic_ctx->subs_reqd))
	{
		icf_sic_dealloc_context(p_sic_pdb);
	}

       

	ICF_FUNCTION_EXIT(p_sic_pdb->p_glb_pdb)
    return ret_val;
}

/* SPR: 9079 fix start */
/****************************************************************
 *
 * FUNCTION:        icf_sic_process_clear_all
 *
 * DESCRIPTION:     This function handles the FORCEFUL CALL clear
 *                  message ID
 *
 **************************************************************/
icf_return_t icf_sic_process_clear_all(
        INOUT icf_sic_pdb_st      *p_sic_pdb)

{
    icf_return_t          ret_val = ICF_SUCCESS;
    icf_dbm_data_st       *p_dbm_data = ICF_NULL;
    icf_config_data_st    *p_config_data = ICF_NULL;
    
    icf_uint16_t          max_num_of_calls;
    icf_uint16_t          temp_index;
     
    
    ICF_FUNCTION_ENTER(p_sic_pdb->p_glb_pdb)

    ret_val = icf_dbm_get_module_glb_data(
        p_sic_pdb->p_glb_pdb,
        (icf_int_module_id_t const)ICF_INT_MODULE_DBM,
        (icf_void_t *)&p_dbm_data,
        p_sic_pdb->p_ecode);

    ICF_ASSERT(p_glb_pdb, (ICF_FAILURE != ret_val))

    ret_val = icf_dbm_get_module_glb_data(     
                p_sic_pdb->p_glb_pdb,
                (icf_int_module_id_t const)ICF_INT_MODULE_CFG,
                (icf_void_t *)&p_config_data,
                p_sic_pdb->p_ecode);

    ICF_ASSERT(p_glb_pdb, (ICF_FAILURE != ret_val))

    /*Reinitialize retval to default value*/
    ret_val = ICF_SUCCESS;

    max_num_of_calls = ICF_CMN_SIC_MAX_CALL_CONTEXTS;

    /* loop to delink all SIC contexts from SSA */
    temp_index = 0;
    while(temp_index < max_num_of_calls) 
    {
       p_sic_pdb->p_sic_ctx = 
            ((icf_sic_ctx_st *)p_dbm_data->p_sic_cntxt_blk) +
                    temp_index;

       if (ICF_INVALID_CALL_ID != p_sic_pdb->p_sic_ctx->call_id)
       {
           if ( (p_sic_pdb->p_sic_ctx->bitmask & 
                       ICF_SIC_REQUEST_WITHIN_DIALOG) )
           {
               if(ICF_SIC_01_AWT_MEDIA_CAPAB_RSP == 
                       p_sic_pdb->p_sic_ctx->sic_state ||
                       ICF_SIC_02_AWT_OPTIONS_APP_CFM ==
                       p_sic_pdb->p_sic_ctx->sic_state )
               {
                   /*Send failure response toward network */
                   icf_sic_send_failure_response_to_ssa(p_sic_pdb); 
               }
           }
               if(ICF_NULL != p_sic_pdb->p_sic_ctx)
               {
                   /* Stop Subscription Response timer */ 
                   icf_sic_stop_curr_timer(p_sic_pdb);
                   
                   if((ICF_SUBSCRIBE_REQ == p_sic_pdb->p_sic_ctx->method)&&
                      (ICF_SIC_03_UNSUBSCRIBING != 
                       p_sic_pdb->p_sic_ctx->sic_state))
                   {
                        p_sic_pdb->p_sic_ctx->app_requested_duration=ICF_NULL;
						/* CSR 1-6216029 : SPR 18848
						 *  Set best_effort_unsub to TRUE as there is no need to 
						 * wait for response for Unsubscribe request
						 */
						p_sic_pdb->best_effort_unsub = ICF_TRUE;                        
                        icf_sic_send_unsub_req(p_sic_pdb);
                   }

                   if (ICF_FAILURE == icf_sic_send_delink_to_ssa(
                               p_sic_pdb,ICF_TRUE)) 
                   {
                       ret_val = ICF_FAILURE;
                       break;
                   }
                   else 
                   {         
                       /* De-allocate SIC ctx */
                       icf_sic_dealloc_context(p_sic_pdb);       
                   }       
               }
       }
      temp_index += 1;


    }/* while ends here */    


    ICF_FUNCTION_EXIT(p_sic_pdb->p_glb_pdb)
    return ret_val;

}/* Function icf_sic_process_clear_all() ends here */

/* SPR: 9079 fix end */


/*NOTIFY_OUT_OF_DIALOG_START*/

/*******************************************************************************
 *
 * FUNCTION:        icf_sic_process_notify_cfm
 *
 * DESCRIPTION:     This function processes the notify confirmation.  
 *                  
 *
 ******************************************************************************/
icf_return_t icf_sic_process_notify_cfm(
        INOUT icf_sic_pdb_st      *p_sic_pdb)

{
    icf_return_t		      ret_val = ICF_SUCCESS;
	icf_return_t              dummy_retval = ICF_SUCCESS;
	icf_nw_otg_notify_resp_st notify_resp = {0,0,ICF_NULL};
	icf_sic_notify_cfm_st     *p_notify_cfm = ICF_NULL;

    ICF_FUNCTION_ENTER(p_sic_pdb->p_glb_pdb)

	p_notify_cfm = (icf_sic_notify_cfm_st *)(p_sic_pdb->p_msg);
  
	ret_val = icf_dbm_find_sic_cntxt_blk(
                p_sic_pdb->p_glb_pdb,
                p_notify_cfm->call_id,
                &(p_sic_pdb->p_sic_ctx),
                p_sic_pdb->p_ecode);
	
	p_sic_pdb->notify_resp = p_notify_cfm->result;
    p_sic_pdb->error_cause = p_notify_cfm->error_code;


	if(ICF_NULL == p_sic_pdb->p_sic_ctx)
    {
        ret_val = ICF_FAILURE;
        ICF_FUNCTION_EXIT(p_sic_pdb->p_glb_pdb)
		return ret_val;
    }

    ICF_SIC_APP_TRACE(p_sic_pdb,ICF_TRACE_NOTIFY_CFM)

    /* If this NOTIFY was received within SUBSCRIBE dialog, no need to
     * do anything further here, simply return success  
     */
    if (ICF_NULL ==
            (ICF_SIC_METHOD_TYPE_OOD_NOTIFY & p_sic_pdb->p_sic_ctx->bitmask))
    {
        ICF_FUNCTION_EXIT(p_sic_pdb->p_glb_pdb)
        return ret_val;
    }

	dummy_retval = icf_sic_stop_curr_timer(p_sic_pdb);

	notify_resp.p_sic_ctx = p_sic_pdb->p_sic_ctx;
	notify_resp.result = p_notify_cfm->result;

	if(notify_resp.result)
	{
		notify_resp.response_code = 200;
	}
	else if(p_notify_cfm->response_code)
	{
		notify_resp.response_code = p_notify_cfm->response_code;
	}
	else if(p_notify_cfm->error_code)
	{
		if(ICF_CAUSE_INTERNAL_FAILURE == p_notify_cfm->error_code)
		{
			notify_resp.response_code = 500;
		}
		else if(ICF_CAUSE_MIME_BODY_DECODE_FAILURE == p_notify_cfm->error_code)
		{
			notify_resp.response_code = 400;
		}
		else
		{
			notify_resp.response_code = 500;
		}
	}
	else
	{
		notify_resp.response_code = 500;
	}

    /* Populate header, tag and body list in SIC context */
    if(ICF_NULL != p_notify_cfm->header_list.count)
    {
        ret_val = icf_cmn_clone_header_list(
                      p_sic_pdb->p_glb_pdb,
                      &(p_notify_cfm->header_list),
                      &(p_sic_pdb->p_sic_ctx->header_list),
                      p_sic_pdb->p_ecode);
    }    
                
    if(ICF_NULL != p_notify_cfm->tag_list.count)
    {
        ret_val = icf_cmn_clone_tag_list(
                      p_sic_pdb->p_glb_pdb,
                      &(p_notify_cfm->tag_list),
                      &(p_sic_pdb->p_sic_ctx->tag_list),
                      p_sic_pdb->p_ecode);
    }
        
    if(ICF_NULL != p_notify_cfm->body_list.count)
    {
        ret_val = icf_cmn_clone_body_list(
                      p_sic_pdb->p_glb_pdb,
                      &(p_notify_cfm->body_list),
                      &(p_sic_pdb->p_sic_ctx->body_list),
                      p_sic_pdb->p_ecode);
    }

	p_sic_pdb->p_internal_msg->p_msg_data = 
					(icf_uint8_t *)&notify_resp;
	p_sic_pdb->p_internal_msg->msg_hdr.msg_id = ICF_NW_OTG_NOTIFY_RESP;

    /* Trigger SSA */ 
    if(ICF_FAILURE == icf_ssa_process_mesg(p_sic_pdb->p_internal_msg))
	{
		ret_val = ICF_FAILURE;
	}
	else
	{
        /*CSR 1-6232998: When we send an CFM for OOD NOTIFY
          the context is being deallocated, so the SSA ctxt should also be detached*/
        p_sic_pdb->p_sic_ctx->detach_sic_ctx = ICF_TRUE;
        icf_sic_send_delink_to_ssa(p_sic_pdb,
                    (icf_boolean_t)ICF_FALSE);
		icf_sic_dealloc_context(p_sic_pdb);
	}

	ICF_FUNCTION_EXIT(p_sic_pdb->p_glb_pdb)
    return ret_val;

}/* Function icf_sic_process_notify_cfm() ends here */

/*******************************************************************************
 *
 * FUNCTION:        icf_sic_process_notify_out_of_dialog
 *
 * DESCRIPTION:     This function is called when a stray NOTIFY req is receved
 *					remote.
 *                  
 ******************************************************************************/
icf_return_t icf_sic_process_notify_out_of_dialog(
        INOUT icf_sic_pdb_st  *p_sic_pdb)
{
    icf_return_t         ret_val = ICF_SUCCESS;

    icf_return_t         dummy_retval = ICF_SUCCESS;
    icf_nw_inc_notify_ind_st *p_notify_ind = ICF_NULL;
    
    ICF_FUNCTION_ENTER(p_sic_pdb->p_glb_pdb)
        
    p_notify_ind = (icf_nw_inc_notify_ind_st *)(p_sic_pdb->p_msg);   
	p_sic_pdb->p_sic_ctx = p_notify_ind->p_sic_ctx;
    	
    /* Start Subscription Response timer */
    ICF_SIC_SET_TIMER_TYPE(p_sic_pdb,ICF_TIMER_TYPE_NOTIFY)
    dummy_retval = icf_sic_start_timer(p_sic_pdb);

      p_sic_pdb->p_sic_ctx->duration = p_notify_ind->duration;
      p_sic_pdb->subs_state = p_notify_ind->subs_state;
      ICF_SIC_APP_TRACE(p_sic_pdb,ICF_TRACE_OTDLG_NOTIFY_IND)

    /* If the buffer is NULL then start Subscription duration timer of the 
     * duration if received in NOTIFY other wise of the duration present in 
     * the context
     */
    if (ICF_FALSE == p_sic_pdb->p_sic_ctx->event_buffer)
    {
		p_sic_pdb->send_htb_list = ICF_TRUE; 
		icf_sic_send_notify_msg_to_app(p_sic_pdb);
    }
    
    /* If the buffer is not NULL then */
    else
    {
        ICF_SIC_RESET_BUFFER(p_sic_pdb)
                 
        icf_sic_send_delink_to_ssa(p_sic_pdb,
                (icf_boolean_t)ICF_FALSE);
        
        icf_sic_dealloc_context(p_sic_pdb);    
    }/* End else */
    
    ICF_FUNCTION_EXIT(p_sic_pdb->p_glb_pdb)

    /* Always returns success */    
    return ret_val;
}/* End icf_sic_process_notify_out_of_dialog() function */

/****************************************************************
 *
 * FUNCTION:        icf_sic_process_reinit_all
 *
 * DESCRIPTION:     This function handles the FORCEFUL reinitialize clear
 *                  message ID
 *
 **************************************************************/
icf_return_t icf_sic_process_reinit_all(
        INOUT icf_sic_pdb_st      *p_sic_pdb)

{
    icf_return_t          ret_val = ICF_SUCCESS;
    icf_dbm_data_st       *p_dbm_data = ICF_NULL;
    icf_config_data_st    *p_config_data = ICF_NULL;
    
    icf_uint16_t          max_num_of_calls = 0;
    icf_uint16_t          temp_index = 0;
    icf_boolean_t         auto_subscription_for_reg = ICF_FALSE;     
    
    ICF_FUNCTION_ENTER(p_sic_pdb->p_glb_pdb)

    ret_val = icf_dbm_get_module_glb_data(
        p_sic_pdb->p_glb_pdb,
        (icf_int_module_id_t const)ICF_INT_MODULE_DBM,
        (icf_void_t *)&p_dbm_data,
        p_sic_pdb->p_ecode);

    ICF_ASSERT(p_glb_pdb, (ICF_FAILURE != ret_val))

    ret_val = icf_dbm_get_module_glb_data(     
                p_sic_pdb->p_glb_pdb,
                (icf_int_module_id_t const)ICF_INT_MODULE_CFG,
                (icf_void_t *)&p_config_data,
                p_sic_pdb->p_ecode);

    ICF_ASSERT(p_glb_pdb, (ICF_FAILURE != ret_val))

    /*Reinitialize retval to default value*/
    ret_val = ICF_SUCCESS;

    max_num_of_calls = ICF_CMN_SIC_MAX_CALL_CONTEXTS;

    /* loop to delink all SIC contexts from SSA */
    temp_index = 0;
    while(temp_index < max_num_of_calls) 
    {
       p_sic_pdb->p_sic_ctx = 
            ((icf_sic_ctx_st *)p_dbm_data->p_sic_cntxt_blk) +
                    temp_index;

       if (ICF_INVALID_CALL_ID != p_sic_pdb->p_sic_ctx->call_id)
       {
           if (ICF_SIC_02_SUBSCRIBED == p_sic_pdb->p_sic_ctx->sic_state)
           {
               /*If the subscription for reg event package is true and
                * the event package in sic_pdb is "reg" then set the
                * value of auto_subscription_for_reg as TRUE.So in this
                * case there is no need to send the unsubscribe request
                * on network for reg event .*/

                /* to identify the trigger is from Reg module, check for existence of bitmask
                 * instead of system level configuration check for reg event pcks subs suppport
                 */ 
                if(ICF_SIC_AUTO_SUBSCRIPTION_FOR_REG_EVENT ==
                   (ICF_SIC_AUTO_SUBSCRIPTION_FOR_REG_EVENT & p_sic_pdb->p_sic_ctx->bitmask))    
                {
                    auto_subscription_for_reg = ICF_TRUE;
                }   
                if (ICF_FALSE == auto_subscription_for_reg)
                {   
                    /* Send Unsubscribe for the subscribed context */
                    icf_sic_send_unsub_req(p_sic_pdb);

                    /*Reset the value of auto_subscription_for_reg*/
                    auto_subscription_for_reg = ICF_FALSE;
                }     
           }
           else if(ICF_SIC_01_SUBSCRIBING ==  p_sic_pdb->p_sic_ctx->sic_state)
           {
               icf_sic_01_failure_resp_no_retry(p_sic_pdb);
           }
           else if(ICF_SIC_01_AWT_MEDIA_CAPAB_RSP == 
                   p_sic_pdb->p_sic_ctx->sic_state ||
                   ICF_SIC_02_AWT_OPTIONS_APP_CFM ==
                   p_sic_pdb->p_sic_ctx->sic_state )
           {
               /*Send failure response toward network */
               icf_sic_send_failure_response_to_ssa(p_sic_pdb); 
           }

           if(ICF_NULL != p_sic_pdb->p_sic_ctx)
           {
               /* Stop Subscription Response timer */ 
               icf_sic_stop_curr_timer(p_sic_pdb);

               if (ICF_FAILURE == icf_sic_send_delink_to_ssa(
                           p_sic_pdb,ICF_TRUE)) 
               {
                   ret_val = ICF_FAILURE;
                   break;
               }
               else 
               {         
                   /* De-allocate SIC ctx */
                   icf_sic_dealloc_context(p_sic_pdb);       
               }       
           }
       }
       
      temp_index += 1;


    }/* while ends here */    


    ICF_FUNCTION_EXIT(p_sic_pdb->p_glb_pdb)
    return ret_val;

}
