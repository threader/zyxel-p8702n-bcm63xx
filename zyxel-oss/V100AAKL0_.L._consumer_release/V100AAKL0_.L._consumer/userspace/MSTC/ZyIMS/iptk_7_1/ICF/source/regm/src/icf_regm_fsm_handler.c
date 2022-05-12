/*------------------------------------------------------------------------------
 *
 * File name        : icf_regm_fsm_handler.c
 *
 * Purpose          : This file contains the handler functions for registration
 *                    FSM.
 *
 * Revision History :
 *
 * Date           Name          Ref#                 Comments
 * --------    ------------     ----                 ---------------
 * 24-Dec-2004  Jalaj Negi     Non Service LLD      Initial
 * 30-Mar-2005  Jalaj Negi                          Resetting of Auth Indicator
 *                                                  after successful 
 *                                                  registration response.
 * 26-Apr-2005  Jalaj Negi     SPR 7441             Added Stats.
 * 26-Apr-2005  Jalaj Negi     SPR 7361             Added code for starting
 *                                                  expires timer.
 * 05-Dec-2005	Jyoti Narula	ICF 4.0				Changes for ICF 4.0
 *
 * 03-Jan-2006	Jyoti Narula    SPR 9139			In 02_reg_req_recd(): 
 *													giving value to 
 *													rgm_context 
 *
 * 06-Jan-2006	Jyoti Narula	SPR 9048			In 
 *													icf_rgm_03_
 * 													notify_terminated()
 *													SIC is no more
 *													triggered to send 
 *													the SUBSCRIBE with 
 *													0 in Expires.
 *
 * 27-Jan-2006	Jyoti Narula   SPR 9849				In
 *													the rgm_user_address 
 *													now stores the value
 *													as used in  To for 
 *													REGISTER message.
 * 02-Apr-2006	Puneet Sharma	ICF5.0 LLD	 changes done in the comments 
 *											 Multiple Public Users ICF5.0
 * 28-Nov-2006  Deepti Goyal    ICF 6.2             RPORT Feature support.
 *
 * 12-Dec-2006  Deepti Goyal    SPR 14030           Store the initial regis
 *                                                  tration duration and also
 *                                                  get the rgm context from
 *                                                  crm pdb.
 *
 * 13-Dec-2006  Deepti Goyal    SPR 14040           Binding refresh timer expiry
 * 28-Dec-2006  Deepti Goyal    SPR 14173           Update rport config data when
 *                                                  de-register request received 
 *                                                  in registered state.     
 * 08-JUN-2007  Alok Tiwari    Rel early 7.0        Code added for stat collection
 * 7-Sep-2007   Deepti Goyal   SPR 16019            Reg-Dereg Enhancement.
 * 28-Nov-2007  Deepti Goyal   SPR 17622            Register-Deregister handling
 *                                                  in authorizing state.
 * 06-Dec-2007  Amit Sharma                         Merged in PQT 7.0 from 6_4 Branch
 * 23-Dec-2007  Abhishek Dhammawat  SPR 17855       Modified icf_rgm_03_reg_req_recvd
 * 24-Dec-2007  Jagmohan Chauhan    SPR 17869       Modified for 01_04_timeout
 * 25-Jul-2008  Puneet Sharma  CSR 1- 6168806
 * 21-Dec-2009  Pavneet Bajaj   SPR 20244           Modified icf_rgm_05_timeout
 *  
 * Copyright (c) 2006, Aricent.
 *----------------------------------------------------------------------------*/

#include "icf_dbm_prototypes.h"
#include "icf_regm_prototypes.h"
#include "icf_regm_macro.h"
#include "icf_common_prototypes.h"
#include "icf_internal_api_id.h"
#include "icf_cfg_prototypes.h"
#include "icf_dbm_regm_prototypes.h"

/****************************************************************************
 * FUNCTION:            icf_rgm_ignore
 *
 * DESCRIPTION:         This function is invoked in case when an unexpected 
 *                      event occurs in a state.
 *
 *
 * RETURNS:             
 *
 ***************************************************************************/
icf_return_t icf_rgm_ignore(
        INOUT icf_crm_pdb_st *p_crm_pdb)
{
    icf_return_t ret_val;

    if(ICF_NULL == p_crm_pdb)
    {
       return ICF_FAILURE;
    }

	p_crm_pdb = p_crm_pdb; 
    
    ICF_FUNCTION_ENTER(p_crm_pdb->p_internal_msg->msg_hdr.p_glb_pdb)

    ICF_RGM_TRACE(p_crm_pdb, ICF_TRACE_INVALID_STATE_EVENT)
        
    ret_val = ICF_SUCCESS;
    
    ICF_FUNCTION_EXIT(p_crm_pdb->p_internal_msg->msg_hdr.p_glb_pdb)

    return ret_val;
}


/****************************************************************************
 * FUNCTION:            icf_rgm_01_04_success_resp
 *
 * DESCRIPTION:         This function is invoked when a success response is
 *                      received in REGISTERING or AUTHORIZING state.
 *
 *
 * RETURNS:             
 *
 ***************************************************************************/
icf_return_t icf_rgm_01_04_success_resp(
        INOUT icf_crm_pdb_st *p_crm_pdb)

{
    icf_rgm_nw_resp_st *p_nw_resp;
    icf_rgm_context_st *p_rgm_context;
    icf_return_t ret_val;
    /* Variables used for Code Area reduction */
    icf_glb_pdb_st *p_glb_pdb;
    

ICF_CRM_START
	/*SENDING RELATED*/
	icf_rgm_register_resp_st	register_response;
#ifdef IMS_CLIENT
	icf_reg_event_subs_ctx_st	*p_reg_event_subs_ctx = ICF_NULL;
	icf_user_db_node_st *p_node = ICF_NULL;
#endif

#ifdef ICF_IPSEC_ENABLED
    icf_duration_t			sa_duration = ICF_NULL;
#endif

ICF_CRM_END

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
    ICF_ASSERT(
            p_crm_pdb->p_internal_msg->msg_hdr.p_glb_pdb,
            (ICF_NULL != p_rgm_context))
	p_crm_pdb->p_rgm_context = p_rgm_context;
    
    
    /* Get the pointer to global PDB */
    p_glb_pdb = p_crm_pdb->p_internal_msg->msg_hdr.p_glb_pdb;
    ICF_ASSERT(
            p_crm_pdb->p_internal_msg->msg_hdr.p_glb_pdb,
            (ICF_NULL != p_glb_pdb))
	p_crm_pdb->p_glb_pdb = p_glb_pdb;

    /* Get the pointer to ecode */
    p_crm_pdb->p_ecode = &(p_crm_pdb->p_internal_msg->msg_hdr.ecode);
    ICF_ASSERT(
            p_crm_pdb->p_internal_msg->msg_hdr.p_glb_pdb,
            (ICF_NULL != p_crm_pdb->p_ecode))
    

    /* Event is REGISTRATION_SUCCESS_RESPONSE */
    ICF_STAT(p_glb_pdb, ICF_STAT_NUM_OF_REG_SUCC_RESP)

#ifdef ICF_STAT_ENABLED
    if (ICF_RGM_04_AUTHORIZING == p_rgm_context->rgm_state)
    {
        ICF_STAT(p_glb_pdb, ICF_STAT_NUM_OF_REG_AUTH_SUCC)
    }    
#endif

    ICF_STATS(p_crm_pdb->p_glb_pdb->p_stats_info->stat_register.stats_active_reg)
    ICF_STATS(p_crm_pdb->p_glb_pdb->p_stats_info->stat_register.stats_user_registered)

	/* Sending related generic members */
	/* Not setting the options_flag with SINGLE_APP
	* value will make the send function to 
	* Loop on the application list to send the register CFM/ IND
	* to the applications that are kept in this rgm_context */
    /* SPR 16019: Store app_id and line id on which register response to bs sent
     */
	register_response.app_id = p_rgm_context->app_id;	
	register_response.line_id = p_rgm_context->line_id;
	register_response.p_crm_pdb = p_crm_pdb;
	
	register_response.app_id = ICF_NULL;
	
	
	register_response.options_flag = ICF_NULL;
	register_response.options_flag |= ICF_RGM_FILL_URIS;/*fill the p-asso member*/
	register_response.options_flag |= ICF_RGM_CTXT_COPY;/*use rgm_ctxt for To/From*/
	register_response.p_rgm_context = p_rgm_context;
	register_response.p_request_payload = ICF_NULL;
       /*Fill the registration request type based on the state of registration
        *  
        */
       if(ICF_RGM_01_REGISTERING == p_rgm_context->rgm_state)
       {
           register_response.request_type = ICF_REQUEST_TYPE_REGISTERING;
       }
       else if(ICF_RGM_04_AUTHORIZING == p_rgm_context->rgm_state)
       {
          register_response.request_type = ICF_REQUEST_TYPE_AUTHORIZING;
       }

	/* To update the line_data for all the line_ids maintained in this
	 * rgm_context */
	 ret_val = ICF_SUCCESS;
 	 if (ICF_FAILURE == icf_rgm_update_line_info_on_succ(
					p_glb_pdb, p_rgm_context,p_crm_pdb->p_ecode))
	{
		/* Fetching of line data failed */
		
		register_response.status = ICF_FAILURE;
		register_response.error_cause = ICF_CAUSE_INSUFFICIENT_RESOURCES;

		p_crm_pdb->resp_to_be_sent = ICF_FALSE;/*for CFG to not send response*/
		ret_val = icf_rgm_send_response(p_glb_pdb,
								&register_response, p_crm_pdb->p_ecode);
		/* Unable to update line data: REG REQUEST not successful */
		
		ICF_RGM_SET_ECODE(p_crm_pdb, ICF_UNABLE_TO_SEND_REG_REQ)

		/* Clear registration context */
		ret_val = icf_rgm_clear_reg_context(
            p_crm_pdb,
            p_rgm_context); 
		ret_val = ICF_SUCCESS;
		return ret_val;
	}

ICF_CRM_END

ICF_4_0_CRM_START
	/* counts to be made NULL */

	p_rgm_context->auth_retry_count = ICF_NULL;
#ifdef IMS_CLIENT
ICF_CHECK_IMS_START(p_glb_pdb,ICF_TRUE)
	p_rgm_context->retry_count = ICF_NULL;
ICF_CHECK_IMS_END
#endif
#ifndef ICF_IPSEC_ENABLED
	/* this should not be done here if IPSec is enabled because we have a check
	 * on the rgm_state below in this function
	 */
	p_rgm_context->rgm_state = ICF_RGM_03_REGISTERED;
	/* In case a different value is received in 200 OK, use this value to 
     * start the refresh timer but the Re-REGISTER request should still carry 
     * the same value that was sent by the app in register_req */
    /*p_rgm_context->rgm_duration = p_nw_resp->expires_duration;*/
#endif

#ifdef IMS_CLIENT
ICF_CHECK_IMS_START(p_glb_pdb,ICF_TRUE)
    if(ICF_FAILURE == icf_dbm_get_public_uri_node_from_uri_for_line (p_glb_pdb,
        p_rgm_context->rgm_list_of_lines[0],
        &(p_rgm_context->rgm_user_address),
        ICF_USER_DB_USE_PUBLIC_URIS,
        &p_node,
        p_crm_pdb->p_ecode))
    {
        ret_val = ICF_FAILURE;
    }
    else
    {
        icf_address_list_node_st *p_curr_node;
        icf_user_db_node_st *p_temp_node=ICF_NULL;
        for(p_curr_node = p_node->p_assoc_uri_list;
        ICF_NULL != p_curr_node; 
        p_curr_node = p_curr_node->p_next)
        {
            if(ICF_FAILURE == icf_dbm_get_public_uri_node_from_uri_for_line (p_glb_pdb,
                p_rgm_context->rgm_list_of_lines[0],
                &(p_curr_node->user_addr),
                ICF_USER_DB_USE_PUBLIC_URIS,
                &p_temp_node,
                p_crm_pdb->p_ecode))
            {
                ;
            }
            else if((ICF_NULL != p_temp_node)&&(p_temp_node != p_node)) 
            /* Address exists as another node in the user db */
            {
                icf_rgm_clear_buffered_requests_for_user(p_crm_pdb,
                                                     &(p_curr_node->user_addr),
                                                      p_crm_pdb->p_ecode);
            }
        }
    }

	p_reg_event_subs_ctx = ((icf_rgm_glb_data_st *)p_crm_pdb->p_rgm_glb_data)->p_reg_event_subs_ctx;
	if(ICF_NULL == p_reg_event_subs_ctx)
	{
		if (ICF_FAILURE == icf_rgm_send_subs_on_succ(
			p_crm_pdb,p_crm_pdb->p_ecode))
		{
			/* Unable to send SUBSCRIBE */
		}
	}
ICF_CHECK_IMS_END
#endif
	if ((ICF_FAILURE == ret_val)||
		(ICF_FAILURE == icf_rgm_send_app_resp_on_succ(
					p_crm_pdb,&register_response)))
	{	
		ret_val = ICF_FAILURE;	
				
	}/* CFMs sent to app*/
	if (!(ICF_RGM_ONCE_REGISTERED & p_rgm_context->indicator))
    {
        p_rgm_context->indicator |=
                    ICF_RGM_ONCE_REGISTERED;
    }
#ifdef ICF_IPSEC_ENABLED
    
    /* This is the scenario for Re-reg without Auth */
    
    if (ICF_TRUE == p_crm_pdb->p_config_data->ipsec_enabled)
    {
        /* Free the security client parameters in REGM context */
        ICF_MEMFREE(
            p_crm_pdb->p_glb_pdb,
            ((icf_rgm_glb_data_st *)(p_crm_pdb->p_rgm_glb_data))->p_security_client_params,
            ICF_MEM_COMMON,
            p_crm_pdb->p_ecode,
            ret_val)
            
#ifdef ICF_SA_LIFETIME_MOD_SUPPORT
        
        /* If the underlying platform supports SA lifetime modification, extend 
         * the duration of the existing SA. If a lesser duration is received in 
         * register response then use that duration for SA, update the duration 
         * in REGM context and the duration in SA context will be updated on 
         * receiving success response. */
        
        /* For 200 OK in Registering state, modify request will always be sent. 
         * But for 200 OK in Authorizing state, modify request will be sent only
         * if network responds with a lower Expires value */
        
        if (ICF_RGM_01_REGISTERING  == p_crm_pdb->p_rgm_context->rgm_state)
        {
            sa_duration = 
                ((icf_rgm_glb_data_st *)(p_crm_pdb->p_rgm_glb_data))->p_new_sa_context->sa_lifetime;
            
            if ((ICF_NULL != p_nw_resp->expires_duration) && 
                (p_nw_resp->expires_duration < 
                    p_crm_pdb->p_rgm_context->rgm_duration))
            {
                sa_duration = p_nw_resp->expires_duration + 
                                ICF_RGM_IPSEC_DELTA_DURATION;
            }
            
            /* send SA modify request to NM */
            ((icf_rgm_glb_data_st *)(p_crm_pdb->p_rgm_glb_data))->sa_identifier = ICF_RGM_IPSEC_SA_NEW;
            if (ICF_FAILURE == icf_rgm_ipsec_nm_sa_modify_req(
                                    p_crm_pdb,
                                    sa_duration))
            {
                ret_val = ICF_FAILURE;
            }
            /* start timer for awaiting SA modify response */
            else if (ICF_FAILURE == icf_rgm_start_timer(
                                    p_crm_pdb->p_glb_pdb,
                                    ICF_TIMER_TYPE_RGM_SA_MODIFY,
                                    p_crm_pdb->p_rgm_context,
                                    p_crm_pdb->p_ecode))
            {
                ret_val = ICF_FAILURE;
            }
            /* save the current timestamp */
            else if (ICF_FAILURE == icf_rgm_get_timestamp(
                                   p_crm_pdb, 
                                   &(((icf_rgm_glb_data_st *)(p_crm_pdb->p_rgm_glb_data))->prev_timestamp)))
            {
                ret_val = ICF_FAILURE;
            }
        } /* End if(rgm_state) */

        else if ((ICF_RGM_04_AUTHORIZING  == 
                    p_crm_pdb->p_rgm_context->rgm_state) &&
            (ICF_NULL != p_nw_resp->expires_duration) && 
            (p_nw_resp->expires_duration < 
                    p_crm_pdb->p_rgm_context->rgm_duration))
        {
            sa_duration = p_nw_resp->expires_duration + 
                            ICF_RGM_IPSEC_DELTA_DURATION;
            
            /* send SA modify request to NM */
            ((icf_rgm_glb_data_st *)(p_crm_pdb->p_rgm_glb_data))->sa_identifier = ICF_RGM_IPSEC_SA_NEW;
            if (ICF_FAILURE == icf_rgm_ipsec_nm_sa_modify_req(
                                    p_crm_pdb,
                                    sa_duration))
            {
                ret_val = ICF_FAILURE;
            }
            /* start timer for awaiting SA modify response */
            else if (ICF_FAILURE == icf_rgm_start_timer(
                                    p_crm_pdb->p_glb_pdb,
                                    ICF_TIMER_TYPE_RGM_SA_MODIFY,
                                    p_crm_pdb->p_rgm_context,
                                    p_crm_pdb->p_ecode))
            {
                ret_val = ICF_FAILURE;
            }
            /* save the current timestamp */
            else if (ICF_FAILURE == icf_rgm_get_timestamp(
                                   p_crm_pdb,
                                   &(((icf_rgm_glb_data_st *)(p_crm_pdb->p_rgm_glb_data))->prev_timestamp)))
            {
                ret_val = ICF_FAILURE;
            }
        } /* End else(rgm_state) */
		else if ((ICF_RGM_04_AUTHORIZING  == 
                    p_crm_pdb->p_rgm_context->rgm_state) &&
				(p_nw_resp->expires_duration == 
                    p_crm_pdb->p_rgm_context->rgm_duration))
		{
			if (ICF_NULL != ((icf_rgm_glb_data_st *)(p_crm_pdb->p_rgm_glb_data))->p_old_sa_context)
			{
				if (ICF_NULL == 
					((icf_rgm_glb_data_st *)(p_crm_pdb->p_rgm_glb_data))->p_old_sa_context->num_active_trans)
				{
					/* send SA delete request to NM */
					((icf_rgm_glb_data_st *)(p_crm_pdb->p_rgm_glb_data))->sa_identifier = 
						ICF_RGM_IPSEC_SA_OLD;
					
					if (ICF_FAILURE == icf_rgm_ipsec_sa_clean_up(
											p_crm_pdb,
											ICF_RGM_03_REGISTERED))
					{
						ret_val = ICF_FAILURE;
					}
					/* save the current timestamp */
					else if (ICF_FAILURE == icf_rgm_get_timestamp(
								p_crm_pdb, 
								&(((icf_rgm_glb_data_st *)(p_crm_pdb->p_rgm_glb_data))->prev_timestamp)))
					{
						ret_val = ICF_FAILURE;
					}
				}
			}
			else
			{
				ret_val = icf_rgm_start_timer(
                            p_crm_pdb->p_glb_pdb,
                            ICF_TIMER_TYPE_RGM_REGISTRATION,
                            p_crm_pdb->p_rgm_context,
                            p_crm_pdb->p_ecode);
			}
		}
        
#else /* ICF_SA_LIFETIME_MOD_SUPPORT */
        
        /* If the platform does not support SA lifetime modification and 200 OK
		 * is received in Authorizing state, any old SA will now be deleted if 
		 * there are no pending transactions on it */
        
        if ((ICF_RGM_04_AUTHORIZING ==p_crm_pdb->p_rgm_context->rgm_state) &&
            (ICF_NULL != p_crm_pdb->p_rgm_context->p_old_sa_context))
        {
            if (ICF_NULL == 
                p_crm_pdb->p_rgm_context->p_old_sa_context->num_active_trans)
            {
                /* send SA delete request to NM */
                p_crm_pdb->p_rgm_context->sa_identifier = 
                    ICF_RGM_IPSEC_SA_OLD;
                
                if (ICF_FAILURE == icf_rgm_ipsec_sa_clean_up(
                                    p_crm_pdb,
                                    ICF_RGM_03_REGISTERED))
                {
                    ret_val = ICF_FAILURE;
                }
                /* save the current timestamp */
                else if (ICF_FAILURE == icf_rgm_get_timestamp(
                                   p_crm_pdb,
                                   &(p_crm_pdb->p_rgm_context->prev_timestamp)))
                {
                    ret_val = ICF_FAILURE;
                }
            } /* End if(num_active_trans) */
        } /* End if() */
		else
		{
			ret_val = icf_rgm_start_expires_timer(
			        p_glb_pdb,
				    p_nw_resp->expires_duration,
	                p_rgm_context,
					p_crm_pdb->p_ecode);
		}
        
#endif /* ICF_SA_LIFETIME_MOD_SUPPORT */
  } /* End if (ipsec_enabled) */
  else
  {
#endif /* ICF_IPSEC_ENABLED */
            
	ret_val = icf_rgm_start_expires_timer(
                p_glb_pdb,
                p_nw_resp->expires_duration,
                p_rgm_context,
                p_crm_pdb->p_ecode);
      
#ifdef ICF_IPSEC_ENABLED
  } /* End else(ipsec_enabled) */

	/* now we can change the state to Registered */
	p_rgm_context->rgm_state = ICF_RGM_03_REGISTERED;

	/* In case a different value is received in 200 OK, use this value to 
     * start the refresh timer but the Re-REGISTER request should still carry 
     * the same value that was sent by the app in register_req */
    /*p_rgm_context->rgm_duration = p_nw_resp->expires_duration;*/

#endif /* ICF_IPSEC_ENABLED */
	
ICF_4_0_CRM_END
#ifdef ICF_ABANDON
	 /* Set State */
    p_rgm_context->rgm_state = ICF_RGM_03_REGISTERED;
	
    /* SPR 7361 Fix Starts */
    if (0 == p_nw_resp->expires_duration)
    {

        /* Start REGISTRATION_TIMER */
        ret_val = icf_rgm_start_timer(
                p_glb_pdb,
                (icf_timer_type_t)(ICF_TIMER_TYPE_RGM_REGISTRATION),
                p_rgm_context,
                p_crm_pdb->p_ecode);
    }
    else
    {
        ret_val = icf_rgm_start_expires_timer(
                p_glb_pdb,
                p_nw_resp->expires_duration,
                p_rgm_context,
                p_crm_pdb->p_ecode);
    }
    /* SPR 7361 Fix Ends */
#endif

	if (ICF_FAILURE != ret_val)
	{
    
#ifdef NON_IMS_CLIENT    
ICF_CHECK_IMS_START(p_glb_pdb,ICF_FALSE)
		/* Check whether PA was Indicated */
		if (p_rgm_context->indicator & ICF_RGM_PA_INDICATED)
		{
			/* PA was indicated about registration failure */
			/* Reset the PA_INDICATED bit */
			p_rgm_context->indicator &= ICF_RGM_RESET_PA_IND;
		}
		/* Reset Authentication indicator after successful response */
	/*	p_rgm_context->indicator &= ICF_RGM_AUTH_RESET;*/
ICF_CHECK_IMS_END
#endif

	}/* ret_val != FAILURE ends here */

    /* incase probation timer expired while rgm_context
         * was waiting for response, a refresh REGISTER should be sent */
    if (ICF_FAILURE == icf_rgm_refresh_register(p_crm_pdb))
    {
        /* Error: could not send a Refresh Register request */        
        ret_val = ICF_FAILURE;
    }
    /* function exit trace */
    ICF_FUNCTION_EXIT(p_crm_pdb->p_internal_msg->msg_hdr.p_glb_pdb)
    
    return ret_val;
    
}/* Function icf_rgm_01_04_success_resp() ends here */

/****************************************************************************
 * FUNCTION:            icf_rgm_05_any_nw_resp
 *
 * DESCRIPTION:         This function is invoked when any type of response is
 *                      received in DEREGISTERING state.
 *
 *
 * RETURNS:             
 *
 ***************************************************************************/
icf_return_t icf_rgm_05_any_nw_resp(
        INOUT icf_crm_pdb_st *p_crm_pdb)
{
    icf_rgm_nw_resp_st *p_nw_resp;
    icf_rgm_context_st *p_rgm_context;
    icf_return_t ret_val = ICF_SUCCESS;

    /* Variables used for Code Area reduction */
    icf_glb_pdb_st *p_glb_pdb;
    icf_rgm_register_resp_st    register_response;

    

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


    /* Event is DEREGISTRATION RESPONSE */

    if (ICF_FAILURE == icf_rgm_refresh_register(p_crm_pdb))
    {

    }/* probation timer expired while waiting for response: ends */
    else
    {
#ifdef ICF_IPSEC_ENABLED
        
        /* Clear all contexts */
        
        if (ICF_TRUE == p_crm_pdb->p_config_data->ipsec_enabled)
        {
            /* Delete any existing SAs */
            ((icf_rgm_glb_data_st *)(p_crm_pdb->p_rgm_glb_data))->sa_identifier = ICF_RGM_IPSEC_SA_ALL;
            
            ret_val = icf_rgm_ipsec_sa_clean_up(
                p_crm_pdb, ICF_RGM_07_IPSEC_AWT_SA_DEL_RESP);
        } /* End if (ipsec_enabled) */
#endif /* ICF_IPSEC_ENABLED */
        /* SPR 16019: Send de-register indication to application
         */
        if(ICF_RGM_INTERNAL_GENERATED_REG == (p_rgm_context->indicator & \
                    ICF_RGM_INTERNAL_GENERATED_REG))
        {
           
            register_response.options_flag = ICF_NULL;
            register_response.app_id = p_rgm_context->app_id;
            register_response.line_id = p_rgm_context->line_id;
            register_response.options_flag |= ICF_RGM_RESPONSE_IND;
            register_response.options_flag |= ICF_RGM_CTXT_COPY;            
            register_response.status = ICF_SUCCESS;
            register_response.error_cause = ICF_NULL;
            register_response.request_type = ICF_REQUEST_TYPE_DEREGISTER;
            register_response.p_rgm_context = p_rgm_context;
            register_response.p_request_payload = ICF_NULL;
            register_response.p_crm_pdb = p_crm_pdb;
            icf_rgm_send_response(p_crm_pdb->p_glb_pdb,
                    &register_response, p_crm_pdb->p_ecode);
            p_rgm_context->indicator &=~(ICF_RGM_INTERNAL_GENERATED_REG);
        }
        else
        {
            /* Send register response to application*/
            register_response.p_crm_pdb = p_crm_pdb;
            register_response.p_rgm_context = p_rgm_context;
            register_response.app_id = p_rgm_context->app_id;
            register_response.line_id = p_rgm_context->line_id;
            register_response.options_flag = ICF_NULL;

            /*CSR 1- 6168806  In case this was dereg for all user
              donot send CFM but IND*/
            if(ICF_REGISTER_FOR_ALL_USER & p_rgm_context->indicator)
            {
                register_response.options_flag |= ICF_RGM_RESPONSE_IND;
            }
            else
            {
                register_response.options_flag |= ICF_RGM_RESPONSE_CFM;
            }
            /*copy from paylaod*/
            /*CSR 1- 6168806  Since the paylod is sent as NULL, the request 
              should not be copied*/
            /*register_response.options_flag |= ICF_RGM_REQUEST_COPY;*/
            register_response.p_request_payload = ICF_NULL;
            /*for CFG to not send response*/
            register_response.status = ICF_SUCCESS;
            register_response.error_cause = ICF_NULL;
            register_response.request_type = ICF_REQUEST_TYPE_DEREGISTER;
            p_crm_pdb->resp_to_be_sent = ICF_FALSE;
            ret_val = icf_rgm_send_response(p_crm_pdb->p_glb_pdb,
                            &register_response, p_crm_pdb->p_ecode);

        }            
            /* Clear registration context */
            ret_val = icf_rgm_clear_reg_context(
                p_crm_pdb,
                p_rgm_context);            
            
            /* Ask Aashish */
            ret_val = ICF_SUCCESS;
    }
#ifdef ICF_STAT_ENABLE
    ICF_STATS(p_crm_pdb->p_glb_pdb->p_stats_info->stat_register.stats_dereg_done)
    (p_crm_pdb->p_glb_pdb->p_stats_info->stat_register.stats_active_reg)--;
#endif    

    /* function exit trace */
    ICF_FUNCTION_EXIT(p_crm_pdb->p_internal_msg->msg_hdr.p_glb_pdb)
        
    return ret_val;
}/* function icf_rgm_05_any_nw_resp() ends here */


/****************************************************************************
 * FUNCTION:            icf_rgm_01_04_failure_resp
 *
 * DESCRIPTION:         This function is invoked when a failure response with 
 *                      reattempt received in REGISTERING or AUTHORIZING state.
 *
 *
 * RETURNS:             
 *
 ***************************************************************************/
icf_return_t icf_rgm_01_04_failure_resp(
        INOUT icf_crm_pdb_st *p_crm_pdb)

{
    icf_rgm_nw_resp_st *p_nw_resp;
    icf_rgm_context_st *p_rgm_context;
    icf_return_t ret_val = ICF_SUCCESS;

	/* For sending CFM/IND */
	icf_rgm_register_resp_st		register_response;

    /* Variables used for Code Area reduction */
    icf_glb_pdb_st *p_glb_pdb;
    

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
    ICF_ASSERT(
            p_crm_pdb->p_internal_msg->msg_hdr.p_glb_pdb,
            (ICF_NULL != p_rgm_context))
	p_crm_pdb->p_rgm_context = p_rgm_context;
    
    
    /* Get the pointer to global PDB */
    p_glb_pdb = p_crm_pdb->p_internal_msg->msg_hdr.p_glb_pdb;
    ICF_ASSERT(
            p_crm_pdb->p_internal_msg->msg_hdr.p_glb_pdb,
            (ICF_NULL != p_glb_pdb))
	p_crm_pdb->p_glb_pdb = p_glb_pdb;

    /* Get the pointer to ecode */
    p_crm_pdb->p_ecode = &(p_crm_pdb->p_internal_msg->msg_hdr.ecode);
    ICF_ASSERT(
            p_crm_pdb->p_internal_msg->msg_hdr.p_glb_pdb,
            (ICF_NULL != p_crm_pdb->p_ecode))


    /* Send failure responses/indications to applications */
    register_response.p_crm_pdb = p_crm_pdb;
    register_response.p_rgm_context = p_rgm_context;
    
	   /* Not setting the options_flag
       * with SINGLE_APP value makes the send function to 
       * Loop on the application list to send the register CFM/ IND
    * to the applications that are kept in this rgm_context */
    
      /* SPR 16019: App id and line id from rgm_context. 
    */
    register_response.app_id = p_rgm_context->app_id;
    register_response.line_id = p_rgm_context->line_id;
    
    register_response.options_flag = ICF_NULL;
    register_response.p_request_payload = ICF_NULL;
    
    
    register_response.status = ICF_FAILURE;

    /*SPR 13776 Aman 05-01-07*/
    switch(p_nw_resp->ecode)
    {
	    case ICF_REGM_SSA_AUTH_FAILURE : 
		    register_response.error_cause = ICF_CAUSE_AUTH_FAILED;
		    break;
	    case ICF_RGM_SSA_FAIL_REATTEMPT :
		    register_response.error_cause = ICF_CAUSE_NETWORK_ERROR;
		    break;
	    default : register_response.error_cause = ICF_CAUSE_NETWORK_DEREGISTERED;
    }
    /* Fill the registration request type based on the state of registration
     *  
     */
    if(ICF_RGM_01_REGISTERING == p_rgm_context->rgm_state)
    {
        register_response.request_type = ICF_REQUEST_TYPE_REGISTERING;
    }
    else if(ICF_RGM_04_AUTHORIZING == p_rgm_context->rgm_state)
    {
        register_response.request_type = ICF_REQUEST_TYPE_AUTHORIZING;
    }
    
    p_crm_pdb->resp_to_be_sent = ICF_FALSE;/*for CFG to not send response*/
    ret_val = icf_rgm_send_response(p_glb_pdb,
					&register_response, p_crm_pdb->p_ecode);

    if (!p_rgm_context->auto_reg_flag)
    {
        /* AUTO_REG is not sent through this rgm_context */	
        
        
        /* Clear the context */
        ret_val = icf_rgm_clear_reg_context(
            p_crm_pdb,
            p_rgm_context);         
        ret_val = ICF_SUCCESS;
    }
    else
    {
    /* AUTO_REG was selected on this rgm_ctx, 
        * hence reattempt timer needs to be started */                 
        
        
        /* Set the state */
        p_rgm_context->rgm_state = ICF_RGM_02_UNREGISTERED;
        /* Set the value in indicator bit */
        p_rgm_context->indicator |= ICF_RGM_UNREG_ON_REATTEMPT;
        
        /* Start REATTEMPT_TIMER */
        ret_val = icf_rgm_start_timer(
            p_glb_pdb,
            (icf_timer_type_t)(ICF_TIMER_TYPE_RGM_REATTEMPT),
            p_rgm_context,
            p_crm_pdb->p_ecode);
        
        if (ICF_FAILURE == ret_val)
        {
            /* Clear the context */
            ret_val = icf_rgm_clear_reg_context(
                p_crm_pdb,
                p_rgm_context);         
            
            
            ret_val =  ICF_SUCCESS;
            
            
        }                        
        
    }/* auto_reg selected hence start reattempt timer */
    ICF_STATS(p_crm_pdb->p_glb_pdb->p_stats_info->stat_register.stats_failed_reg)	

    /* function exit trace */
    ICF_FUNCTION_EXIT(p_crm_pdb->p_internal_msg->msg_hdr.p_glb_pdb)
        
    return ret_val;
}/* Function icf_rgm_01_04_failure_resp() ends here */


/****************************************************************************
 * FUNCTION:            icf_rgm_01_04_fail_no_resp
 *
 * DESCRIPTION:         This function is invoked when a failure response with no
 *                      reattempt received in REGISTERING or AUTHORIZING state.
 *
 *
 * RETURNS:             
 *
 ***************************************************************************/
icf_return_t icf_rgm_01_04_fail_no_resp(
        INOUT icf_crm_pdb_st *p_crm_pdb)
{
    icf_rgm_nw_resp_st *p_nw_resp;
    icf_rgm_context_st *p_rgm_context;
    icf_glb_pdb_st *p_glb_pdb;
    
    icf_return_t ret_val = ICF_SUCCESS;

   /* For sending CFM/IND */
   icf_rgm_register_resp_st		register_response;
   icf_boolean_t   clear_ctx = ICF_FALSE;

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
        
   
    /* Check whether PA was not indicated */
    if (!(p_rgm_context->indicator & ICF_RGM_PA_INDICATED))
    {
        /* PA was not indicated about registration failure */

        /* Send User Registered indication to PA */
		/* this api is not supported in icf */
		/*
        icf_rgm_send_pa_display_ind(
                (icf_display_id_t)
                ICF_DISPLAY_USER_NOT_REGISTERED,
                p_crm_pdb,
                p_rgm_context);*/

        /* Set the PA_INDICATED bit */
        p_rgm_context->indicator |= ICF_RGM_PA_INDICATED;
    }

	/* Send failure responses/indications to applications */

	register_response.p_crm_pdb = p_crm_pdb;
	register_response.p_rgm_context = p_rgm_context;
	
   /* Not setting the options_flag with
    * SINGLE_APP makes the send function to 
	* Loop on the application list to send the register CFM/ IND
	* to the applications that are kept in this rgm_context */

    /* SPR 16019: Line id and app id as stored in rgm_context
     */
	register_response.app_id = p_rgm_context->app_id;
    register_response.line_id = p_rgm_context->line_id;

	register_response.options_flag = ICF_NULL;
	register_response.p_request_payload = ICF_NULL;
 

	register_response.status = ICF_FAILURE;
	if (ICF_REGM_SSA_BASIC_AUTH_SCHEME_NOT_SUPPORTED == p_crm_pdb->error_cause)
	{
		register_response.error_cause = ICF_CAUSE_BASIC_AUTH_SCHEME_NOT_SUPPORTED;
	}
	else if(ICF_REGM_SSA_AUTH_FAILURE ==  p_crm_pdb->error_cause)
	{
		register_response.error_cause = ICF_CAUSE_AUTH_FAILED;
	} 
	else
	{
		register_response.error_cause = ICF_CAUSE_NETWORK_DEREGISTERED;
	}
        /* Fill the registration request type based on the state of registration
         *  
         */
        if(ICF_RGM_01_REGISTERING == p_rgm_context->rgm_state)
        {
            register_response.request_type = ICF_REQUEST_TYPE_REGISTERING;
        }
        else if(ICF_RGM_04_AUTHORIZING == p_rgm_context->rgm_state)
        {
            register_response.request_type = ICF_REQUEST_TYPE_AUTHORIZING;
        }

	/* SPR 8064: To send Warning header to the application */
	if (ICF_RGM_SSA_WARNING_FILLED & p_rgm_context->indicator)
	{
		/* reset the bit for next such scenario */
		p_rgm_context->indicator &= ~ICF_RGM_SSA_WARNING_FILLED;

		/* To indicate the send_response() that warning header
		 * can be passed if the application has specified it */
		register_response.options_flag |= ICF_RGM_FILL_WARNING;
	}

	p_crm_pdb->resp_to_be_sent = ICF_FALSE;/*for CFG to not send response*/
	ret_val = icf_rgm_send_response(p_glb_pdb,
					&register_response, p_crm_pdb->p_ecode);
if (ICF_TRUE == p_rgm_context->auto_reg_flag)
    {
    /* Reattempt timer should be started. 
        * because this rgm_context had auto_reg ON. */
        /* AUTO_REG was selected on this rgm_ctx, 
        * hence reattempt timer needs to be started */                 
        
        
        /* Set the state */
        p_rgm_context->rgm_state = ICF_RGM_02_UNREGISTERED;
        /* Set the value in indicator bit */
        p_rgm_context->indicator |= ICF_RGM_UNREG_ON_REATTEMPT;
        
        /* Start REATTEMPT_TIMER */
        ret_val = icf_rgm_start_timer(
            p_glb_pdb,
            (icf_timer_type_t)(ICF_TIMER_TYPE_RGM_REATTEMPT),
            p_rgm_context,
            p_crm_pdb->p_ecode);
        
        if (ICF_FAILURE == ret_val)
        {
            clear_ctx = ICF_TRUE;
            
            
        }                 
        
    }/* auto_reg set for this ctx: hence reattempt timer started */
    
    if ((ICF_TRUE == clear_ctx) ||
        (ICF_FALSE == p_rgm_context->auto_reg_flag))
    {
    /* Auto reg not set for this rgm_context 
        * hence no more retries shall be done 
        * or the starting of timer failed: hence clearing to be done */
        
#ifdef ICF_IPSEC_ENABLED
        
        /* Clear all contexts */
        
        if (ICF_TRUE == p_crm_pdb->p_config_data->ipsec_enabled)
        {
            /* Delete any existing SAs */
            if ((ICF_NULL != ((icf_rgm_glb_data_st *)(p_crm_pdb->p_rgm_glb_data))->p_old_sa_context) ||
                (ICF_NULL != ((icf_rgm_glb_data_st *)(p_crm_pdb->p_rgm_glb_data))->p_new_sa_context))
            {
                ((icf_rgm_glb_data_st *)(p_crm_pdb->p_rgm_glb_data))->sa_identifier = ICF_RGM_IPSEC_SA_ALL;
                
                ret_val = icf_rgm_ipsec_sa_clean_up(
                    p_crm_pdb, ICF_RGM_07_IPSEC_AWT_SA_DEL_RESP);
            }
        } /* End if (ipsec_enabled) */
        
#endif /* ICF_IPSEC_ENABLED */
        
        /* Clear the context */
        icf_rgm_clear_reg_context(
            p_crm_pdb,
            p_rgm_context);
    }
    ICF_STATS(p_crm_pdb->p_glb_pdb->p_stats_info->stat_register.stats_failed_reg) 

     
    /* function exit trace */
    ICF_FUNCTION_EXIT(p_crm_pdb->p_internal_msg->msg_hdr.p_glb_pdb)
    
    return ret_val;

}/* Function icf_rgm_01_04_fail_no_resp() ends here */


/****************************************************************************
 * FUNCTION:            icf_rgm_01_auth_req_resp
 *
 * DESCRIPTION:         This function is invoked when Authentication required
 *                      response is received in REGISTERING state.
 *
 *
 * RETURNS:             
 *
 ***************************************************************************/
icf_return_t icf_rgm_01_auth_req_resp(
        INOUT icf_crm_pdb_st *p_crm_pdb)
{
    icf_rgm_nw_resp_st *p_nw_resp;
    icf_rgm_context_st *p_rgm_context;
    icf_return_t ret_val = ICF_SUCCESS;
	icf_rgm_register_resp_st		register_response;

    /* Variables used for Code Area reduction */
    icf_glb_pdb_st *p_glb_pdb;
    
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
    ICF_ASSERT(
            p_crm_pdb->p_internal_msg->msg_hdr.p_glb_pdb,
            (ICF_NULL != p_rgm_context))
	p_crm_pdb->p_rgm_context = p_rgm_context;
    
    
    /* Get the pointer to global PDB */
    p_glb_pdb = p_crm_pdb->p_internal_msg->msg_hdr.p_glb_pdb;
    ICF_ASSERT(
            p_crm_pdb->p_internal_msg->msg_hdr.p_glb_pdb,
            (ICF_NULL != p_glb_pdb))
	p_crm_pdb->p_glb_pdb = p_glb_pdb;


    /* Get the pointer to ecode */
    p_crm_pdb->p_ecode = &(p_crm_pdb->p_internal_msg->msg_hdr.ecode);
    ICF_ASSERT(
            p_crm_pdb->p_internal_msg->msg_hdr.p_glb_pdb,
            (ICF_NULL != p_crm_pdb->p_ecode))

    /* Registration is challenged */
    ICF_STAT(p_glb_pdb, ICF_STAT_NUM_OF_REG_REJ_FOR_AUTH)

ICF_4_0_CRM_START
	/* For Sending failure responses/indications to applications */

	register_response.p_crm_pdb = p_crm_pdb;
	register_response.p_rgm_context = p_rgm_context;
	
   /* Not giving SINGLE_APP value to options_flag
    * makes the send function to 
	* Loop on the application list to send the register CFM/ IND
	* to the applications that are kept in this rgm_context */
    /* SPR 16019: Line id and App id as stored in rgm context
     */
	register_response.app_id = p_rgm_context->app_id;
	register_response.line_id = p_rgm_context->line_id;
	register_response.options_flag = ICF_NULL;
	register_response.options_flag |= ICF_RGM_CTXT_COPY;
	register_response.p_request_payload = ICF_NULL;
	register_response.request_type = ICF_REQUEST_TYPE_AUTHORIZING;
	
	
	/* making error_cause as NULL to be used during auth-processing */
	p_crm_pdb->error_cause = ICF_NULL;

	if (ICF_FAILURE == icf_rgm_update_auth_info(p_crm_pdb))
	{
#ifdef ICF_IPSEC_ENABLED

        /* This is the scenario for Initial-reg or Re-reg with Auth with RES or 
         * SQN failure. In this case, new SA will not be created and we will 
         * send request to proxy with new security parameters */

        if (ICF_TRUE == p_crm_pdb->p_config_data->ipsec_enabled)
        {
             ICF_MEMFREE(
                p_crm_pdb->p_glb_pdb,
                ((icf_rgm_glb_data_st *)(p_crm_pdb->p_rgm_glb_data))->p_security_client_params,
                ICF_MEM_COMMON,
                p_crm_pdb->p_ecode,
                ret_val)
            
            /* In case of Re-reg with Auth, an SA already exists. Let the new SA
             * context point to this already existing old SA */
            if (ICF_NULL != ((icf_rgm_glb_data_st *)(p_crm_pdb->p_rgm_glb_data))->p_new_sa_context)
            {
                 ICF_MEMFREE(
                    p_crm_pdb->p_glb_pdb,
                    ((icf_rgm_glb_data_st *)(p_crm_pdb->p_rgm_glb_data))->p_new_sa_context,
                    ICF_MEM_COMMON,
                    p_crm_pdb->p_ecode,
                    ret_val)
            }
            ((icf_rgm_glb_data_st *)(p_crm_pdb->p_rgm_glb_data))->p_new_sa_context = 
                ((icf_rgm_glb_data_st *)(p_crm_pdb->p_rgm_glb_data))->p_old_sa_context;
            ((icf_rgm_glb_data_st *)(p_crm_pdb->p_rgm_glb_data))->p_old_sa_context = ICF_NULL;
            
            /* New local security client parameters will be fetched at the time 
             * of sending register request to SSA */
	        ret_val = icf_rgm_register_user(p_crm_pdb, p_rgm_context);
        }
#else
#ifdef IMS_CLIENT
ICF_CHECK_IMS_START(p_glb_pdb,ICF_TRUE)
    if (ICF_NULL != p_crm_pdb->error_cause)
		{
ICF_CHECK_IMS_END
#endif
				/* Send failure responses/indications to applications */

			register_response.status = ICF_FAILURE;
			register_response.error_cause = p_crm_pdb->error_cause;
			
			p_crm_pdb->resp_to_be_sent = ICF_FALSE;/*for CFG to not send response*/
			ret_val = icf_rgm_send_response(p_glb_pdb,
							&register_response, p_crm_pdb->p_ecode);
		  /* Clear the context */
           ret_val = icf_rgm_clear_reg_context(
                    p_crm_pdb,
                    p_rgm_context);         
        
        ret_val = ICF_SUCCESS;
#ifdef IMS_CLIENT
ICF_CHECK_IMS_START(p_glb_pdb,ICF_TRUE)
		}/* Applications can be indicated : ends*/
		else
		{/* System error */
        ICF_RGM_SET_ECODE(p_crm_pdb,
                ICF_AUTHENTICATION_INFO_ABSENT)        
		/* Clear the context */
           ret_val = icf_rgm_clear_reg_context(
                    p_crm_pdb,
                    p_rgm_context);         

		ret_val = ICF_FAILURE;
        
		}
ICF_CHECK_IMS_END
#endif

#endif /* ICF_IPSEC_ENABLED */
    }/* update_auth_info() Failure: ends */
	else
	{   
#ifdef ICF_IPSEC_ENABLED
        
        /* This is the scenario for Initial-reg or Re-reg with Auth when network
         * authentication succeeds. In this case, a new SA will be created and 
         * the old SA will be deleted */
        
        if (ICF_TRUE == p_crm_pdb->p_config_data->ipsec_enabled)
        {
            /* Copy the security parameters in new SA context */
            ICF_MEMGET(
                p_crm_pdb->p_glb_pdb,
                ICF_PORT_SIZEOF(icf_ipsec_security_params_st),
                ICF_MEM_COMMON,
                (icf_void_t *)((icf_rgm_glb_data_st *)(p_crm_pdb->p_rgm_glb_data))->p_new_sa_context->p_ue_security_params,
                ICF_RET_ON_FAILURE,
                p_crm_pdb->p_ecode,
                ret_val)
        
            icf_port_memcpy(
              ((icf_rgm_glb_data_st *)(p_crm_pdb->p_rgm_glb_data))->p_new_sa_context->p_ue_security_params,
              ((icf_rgm_glb_data_st *)(p_crm_pdb->p_rgm_glb_data))->p_security_client_params,
              ICF_PORT_SIZEOF(icf_ipsec_security_params_st));
            
            /* Set the lifetime for new SAs */
#ifdef ICF_SA_LIFETIME_MOD_SUPPORT
            /* If SA lifetime modification is supported, set the SA duration to 
             * registration duration plus 30 seconds */
            ((icf_rgm_glb_data_st *)(p_crm_pdb->p_rgm_glb_data))->p_new_sa_context->sa_lifetime = 
                p_crm_pdb->p_rgm_context->rgm_duration + 
                ICF_RGM_IPSEC_DELTA_DURATION;
#else
            /* If SA lifetime modification is not supported, set the SA duration
             * to infinite (2**32 - 1) */
            ((icf_rgm_glb_data_st *)(p_crm_pdb->p_rgm_glb_data))->p_new_sa_context->sa_lifetime = 
                ICF_RGM_IPSEC_DEF_SA_LIFETIME;
#endif /* ICF_SA_LIFETIME_MOD_SUPPORT */
            
            /* send SA create request to NM */
            ((icf_rgm_glb_data_st *)(p_crm_pdb->p_rgm_glb_data))->sa_identifier = ICF_RGM_IPSEC_SA_NEW;
            if (ICF_FAILURE == icf_rgm_ipsec_nm_sa_create_req(p_crm_pdb))
            {
                ret_val = ICF_FAILURE;
            }
            /* start timer for awaiting SA create response */
            else if (ICF_FAILURE == icf_rgm_start_timer(
                                    p_crm_pdb->p_glb_pdb,
                                    ICF_TIMER_TYPE_RGM_SA_CREATE,
                                    p_crm_pdb->p_rgm_context,

                                    p_crm_pdb->p_ecode))
            {
                ret_val = ICF_FAILURE;
            }
            else
            {
                /* Change state to await SA creation response */
                p_crm_pdb->p_rgm_context->rgm_state = 
                    ICF_RGM_01_IPSEC_AWT_SA_CREATE_RESP; 
            }
        } /* End if(ipsec_enabled) */
        else
        {
#endif /* ICF_IPSEC_ENABLED */

	   /* Registration is to be retried */
        ret_val = icf_rgm_register_user(
                p_crm_pdb, 
                p_rgm_context);

#ifdef ICF_IPSEC_ENABLED
        } /* End else(ipsec_enabled) */
#endif /* ICF_IPSEC_ENABLED */

	}
    /* function exit trace */
    ICF_FUNCTION_EXIT(p_crm_pdb->p_internal_msg->msg_hdr.p_glb_pdb)
        
    return ret_val;
}/* Function icf_rgm_01_auth_req_resp() ends here  */
ICF_4_0_CRM_END



/****************************************************************************
 * FUNCTION:            icf_rgm_01_04_dur_short_resp
 *
 * DESCRIPTION:         This function is invoked when Registration Duration
 *                      Brief response is received in REGISTERING or 
 *                      Authorizing state.
 *
 *
 * RETURNS:             
 *
 ***************************************************************************/
icf_return_t icf_rgm_01_04_dur_short_resp(
        INOUT icf_crm_pdb_st *p_crm_pdb)
{
    icf_rgm_nw_resp_st *p_nw_resp;
    icf_rgm_context_st *p_rgm_context;
    icf_return_t ret_val = ICF_SUCCESS;

    /* Variables used for Code Area reduction */
    icf_glb_pdb_st *p_glb_pdb;
        
    icf_duration_t   default_duration = ICF_RGM_DEFAULT_DURATION_NON_IMS;
    
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
#ifdef IMS_CLIENT
   /*Extract the default duration of the registration based on the IMS and NON-IMS*/
ICF_CHECK_IMS_START(p_glb_pdb,ICF_TRUE)
        default_duration = ICF_RGM_DEFAULT_DURATION_IMS_CLIENT;
ICF_CHECK_IMS_END
#endif


	if (ICF_RGM_SSA_DURATION_FILLED & p_rgm_context->indicator)
	{
		/* SSA has set the duration field with Min-Expires value */
		/* No need to change the value of rgm_duration*/

		/* Reset the indicator for this bit */
		p_rgm_context->indicator &= ~(ICF_RGM_SSA_DURATION_FILLED);

		/* The value sent by the Registrar in 423 is in seconds
		 * whereas the duration is maintained as milliseconds in ICF/ICF
		 * Hence we will multiply by 1000 to maintain it in rgm_duration */

		p_rgm_context->rgm_duration *= 1000;

	}
	else
	{
		/* Set Default Duration */
		p_rgm_context->rgm_duration = default_duration;
	}
           
    /* Registration is to be retried */
    ret_val = icf_rgm_register_user(
            p_crm_pdb, 
            p_rgm_context);
    
    /* function exit trace */
    ICF_FUNCTION_EXIT(p_crm_pdb->p_internal_msg->msg_hdr.p_glb_pdb)
        
    return ret_val;
}

/****************************************************************************
 * FUNCTION:            icf_rgm_04_auth_req_resp
 *
 * DESCRIPTION:         This function is invoked when Authentication required
 *                      response is received in AUTHORIZING state.
 *
 *
 * RETURNS:             
 *
 ***************************************************************************/
icf_return_t icf_rgm_04_auth_req_resp(
        INOUT icf_crm_pdb_st *p_crm_pdb)

{
    icf_rgm_nw_resp_st *p_nw_resp;
    icf_rgm_context_st *p_rgm_context;
    icf_return_t ret_val = ICF_SUCCESS;

	/* For sending CFM/IND to applications */ 
	icf_rgm_register_resp_st		register_response;

    /* Variables used for Code Area reduction */
    icf_glb_pdb_st *p_glb_pdb;
    

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
    ICF_ASSERT(
            p_crm_pdb->p_internal_msg->msg_hdr.p_glb_pdb,
            (ICF_NULL != p_rgm_context))
	p_crm_pdb->p_rgm_context = p_rgm_context;
    
    
    /* Get the pointer to global PDB */
    p_glb_pdb = p_crm_pdb->p_internal_msg->msg_hdr.p_glb_pdb;
    ICF_ASSERT(
            p_crm_pdb->p_internal_msg->msg_hdr.p_glb_pdb,
            (ICF_NULL != p_glb_pdb))
	p_crm_pdb->p_glb_pdb = p_glb_pdb;


    /* Get the pointer to ecode */
    p_crm_pdb->p_ecode = &(p_crm_pdb->p_internal_msg->msg_hdr.ecode);
    ICF_ASSERT(
            p_crm_pdb->p_internal_msg->msg_hdr.p_glb_pdb,
            (ICF_NULL != p_crm_pdb->p_ecode))

	/* Sending related generic parameters */
	
	register_response.p_crm_pdb = p_crm_pdb;
	register_response.p_rgm_context = p_rgm_context;
	
	/* Not setting the value to options_flag as
	 * SINGLE_APP make the send function to 
	 * Loop on the application list to send the register CFM/ IND
	 * to the applications that are kept in this rgm_context */
	
    /* SPR 16019:Line Id and App id as stored in rgm context
     */
	register_response.app_id = p_rgm_context->app_id;
	register_response.line_id = p_rgm_context->line_id;
	register_response.options_flag = ICF_NULL;
	register_response.p_request_payload = ICF_NULL;
	register_response.request_type = ICF_REQUEST_TYPE_AUTHORIZING;
	
	/* increment auth_retry_count */
	p_rgm_context->auth_retry_count++;

	if (p_rgm_context->auth_retry_count < 2)
	{/* less than 2 401s to handle: begins*/
	
		/* making error_cause as NULL to be used during auth-processing */
		p_crm_pdb->error_cause = ICF_NULL;
		
		if (ICF_FAILURE == icf_rgm_update_auth_info(p_crm_pdb))
		{
#ifdef IMS_CLIENT
ICF_CHECK_IMS_START(p_glb_pdb,ICF_TRUE)

			if (ICF_NULL != p_crm_pdb->error_cause)
			{
ICF_CHECK_IMS_END
#endif
				/* Send failure responses/indications to applications */
				
				register_response.status = ICF_FAILURE;
				register_response.error_cause = p_crm_pdb->error_cause;
				
				p_crm_pdb->resp_to_be_sent = ICF_FALSE;/*For CFG to not send response*/
				ret_val = icf_rgm_send_response(p_glb_pdb,
					&register_response, p_crm_pdb->p_ecode);
				/* Clear the context */
				ret_val = icf_rgm_clear_reg_context(
                    p_crm_pdb,
                    p_rgm_context);         
				
				ret_val = ICF_SUCCESS;
#ifdef IMS_CLIENT
ICF_CHECK_IMS_START(p_glb_pdb,ICF_TRUE)				
			}/* Applications can be indicated : ends*/
			else
			{/* System error */
				ICF_RGM_SET_ECODE(p_crm_pdb,
					ICF_AUTHENTICATION_INFO_ABSENT)        
					/* Clear the context */
					ret_val = icf_rgm_clear_reg_context(
                    p_crm_pdb,
                    p_rgm_context);         
				
				ret_val = ICF_FAILURE;
				
			}
ICF_CHECK_IMS_END
#endif			
		}/* update_auth_info() Failure: ends */
		else
		{
			
			/* Registration is to be retried */
			ret_val = icf_rgm_register_user(
                p_crm_pdb, 
                p_rgm_context);
		}


	}/* less than 2 401s to handle: ends */
	else
    {/* 2 invalid challenges have been handled: begins */
        
        register_response.status = ICF_FAILURE;
        register_response.error_cause = ICF_CAUSE_AUTH_FAILED;
        
        p_crm_pdb->resp_to_be_sent = ICF_FALSE;/*for CFG to not send response*/
        ret_val = icf_rgm_send_response(p_glb_pdb,
            &register_response, p_crm_pdb->p_ecode);			
        
        
        /* Clear the context */
        ret_val = icf_rgm_clear_reg_context(
            p_crm_pdb,
            p_rgm_context);   
        
        ret_val =  ICF_SUCCESS;
    }/* 2 invalid challenges have been handled: ends  */

   
    /* function exit trace */
    ICF_FUNCTION_EXIT(p_crm_pdb->p_glb_pdb)
        
    return ret_val;

}/* Function icf_rgm_04_auth_req_resp() ends here */


/****************************************************************************
 * FUNCTION:            icf_rgm_05_auth_req_resp
 *
 * DESCRIPTION:         This function is invoked when Authentication required
 *                      response is received in DEREGISTERING state.
 *
 *
 * RETURNS:             
 *
 ***************************************************************************/
icf_return_t icf_rgm_05_auth_req_resp(
        INOUT icf_crm_pdb_st *p_crm_pdb)
{
    icf_rgm_nw_resp_st *p_nw_resp;
    icf_rgm_context_st *p_rgm_context;
    icf_return_t ret_val = ICF_SUCCESS;
	
    /* Variables used for Code Area reduction */
    icf_glb_pdb_st *p_glb_pdb;
    
	
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
    ICF_ASSERT(
		p_crm_pdb->p_internal_msg->msg_hdr.p_glb_pdb,
		(ICF_NULL != p_rgm_context))
		p_crm_pdb->p_rgm_context = p_rgm_context;
    
    
    /* Get the pointer to global PDB */
    p_glb_pdb = p_crm_pdb->p_internal_msg->msg_hdr.p_glb_pdb;
    ICF_ASSERT(
		p_crm_pdb->p_internal_msg->msg_hdr.p_glb_pdb,
		(ICF_NULL != p_glb_pdb))
		p_crm_pdb->p_glb_pdb = p_glb_pdb;
	
	
    /* Get the pointer to ecode */
    p_crm_pdb->p_ecode = &(p_crm_pdb->p_internal_msg->msg_hdr.ecode);
    ICF_ASSERT(
		p_crm_pdb->p_internal_msg->msg_hdr.p_glb_pdb,
		(ICF_NULL != p_crm_pdb->p_ecode))
		

	/* Registration is challenged */
	ICF_STAT(p_glb_pdb, ICF_STAT_NUM_OF_REG_REJ_FOR_AUTH)

	
ICF_4_0_CRM_START

	
	/* increment auth_retry_count */
	p_rgm_context->auth_retry_count++;
	
	if (p_rgm_context->auth_retry_count < 2)
	{/* less than 2 401s to handle: begins*/
		
		/* making error_cause as NULL to be used during auth-processing */
		p_crm_pdb->error_cause = ICF_NULL;
		if (ICF_FAILURE == icf_rgm_update_auth_info(p_crm_pdb))
		{
			ICF_RGM_SET_ECODE(p_crm_pdb,
					ICF_AUTHENTICATION_INFO_ABSENT)        
			
			/* Clear the context */
			ret_val = icf_rgm_clear_reg_context(
				p_crm_pdb,
				p_rgm_context);         
			
			ret_val = ICF_FAILURE;
				
		}/* update_auth_info() Failure: ends */
		else
		{
			
			/* Registration is to be retried */
			ret_val = icf_rgm_deregister_user(
				p_crm_pdb, 
				p_rgm_context);
		}
	}/* less than 2 401s to handle: ends */

	else
	{/* 2 invalid challenges have been handled: begins */
		
			
		/* Clear the context */
		ret_val = icf_rgm_clear_reg_context(
			p_crm_pdb,
			p_rgm_context);   
		
		ret_val =  ICF_SUCCESS;
	}/* 2 invalid challenges have been handled: ends  */
	

	/* function exit trace */
	ICF_FUNCTION_EXIT(p_crm_pdb->p_glb_pdb)
		
	return ret_val;
		
}/* function icf_rgm_05_auth_req_resp() ends here */

ICF_4_0_CRM_END


ICF_4_0_CRM_START

/****************************************************************************
 * FUNCTION:            icf_rgm_00_reg_req_recvd
 *
 * DESCRIPTION:         This function is invoked REGISTER_REQUEST
 *                      API is received in IDLE state.
 *
 *
 * RETURNS:             SUCCESS
 *						FAILURE: Any system error
 *
 ***************************************************************************/
icf_return_t icf_rgm_00_reg_req_recvd(
        INOUT icf_crm_pdb_st *p_crm_pdb)
{
	
	icf_line_id_t			response_line_id = ICF_NULL;
	icf_app_id_t				app_id = ICF_NULL;
	icf_register_req_st			*p_recv_payload = ICF_NULL;
	icf_rgm_register_resp_st	register_response;
	icf_return_t				ret_val = ICF_SUCCESS;
        icf_return_t				dummy_ret_val = ICF_SUCCESS;
	
	icf_uint8_t				*p_temp = ICF_NULL;
	 
	icf_address_st       rgm_uri;/* The URI that will be used to send REGISTER */
    
    icf_duration_t   default_duration = ICF_RGM_DEFAULT_DURATION_NON_IMS;
#ifdef ICF_NAT_RPORT_SUPPORT
    icf_rport_config_data_st            *p_rport_config_data = ICF_NULL;
    icf_line_data_st                    *p_line_data = ICF_NULL;
#endif

#ifdef ICF_IPSEC_ENABLED
    icf_rgm_glb_data_st			*p_rgm_glb_data = ICF_NULL;
#endif

	ICF_ASSERT(p_crm_pdb->p_glb_pdb,
				(p_crm_pdb->p_glb_pdb->p_recv_msg != ICF_NULL))

	ICF_ASSERT(p_crm_pdb->p_glb_pdb,
			(p_crm_pdb->p_recv_payload != ICF_NULL))

    ICF_RGM_APP_TRACE(p_crm_pdb,ICF_TRACE_REGISTARTION_REQ)
/* START: Multiple Public User ICF5.0*/
#ifdef IMS_CLIENT
ICF_CHECK_IMS_START(p_crm_pdb->p_glb_pdb,ICF_TRUE)
	if(p_crm_pdb->p_rgm_context->buffer_id != 0)
	{
		ret_val = icf_rgm_01_reg_req_recvd(p_crm_pdb);
		return ret_val;
	}
ICF_CHECK_IMS_END
#endif
/* END: Multiple Public User ICF5.0*/

	p_recv_payload = p_crm_pdb->p_recv_payload;

	/* retrieving the app_id from recd_msg */
	app_id = p_crm_pdb->p_glb_pdb->p_recv_msg->hdr.app_id;

	response_line_id = ((icf_register_req_st *)
						(p_crm_pdb->p_recv_payload))->line_id;

	p_crm_pdb->error_cause = ICF_NULL;

	/* Basic values for Sending */
	register_response.p_crm_pdb = p_crm_pdb;
	register_response.p_rgm_context = p_crm_pdb->p_rgm_context;
	
	/* Giving value to app_id will make the send function to 
	* send the CFM to only this application */
	
	register_response.app_id = app_id;
	register_response.line_id = response_line_id;
	register_response.options_flag = ICF_NULL;
	register_response.options_flag |= ICF_RGM_SINGLE_APP;/*send to single app*/
	register_response.options_flag |= ICF_RGM_REQUEST_COPY;/*copy from request*/
	
	register_response.p_request_payload = p_recv_payload;
#ifdef IMS_CLIENT
   /*Extract the default duration of the registration based on the IMS and NON-IMS*/
ICF_CHECK_IMS_START(p_crm_pdb->p_glb_pdb,ICF_TRUE)
        default_duration = ICF_RGM_DEFAULT_DURATION_IMS_CLIENT;
ICF_CHECK_IMS_END
#endif
	if ((ICF_REG_REQ_EXPIRY_PRESENT & p_recv_payload->bit_mask) &&
				(ICF_NULL == p_recv_payload->expiry))
	{
		/* Application asked for DEREG: send CFM with FAILURE: 
		 * as it is not even REGed now */		
                /* CSR 1-5350927 Fixed */
		register_response.request_type =  ICF_REQUEST_TYPE_DEREGISTER;
		register_response.status = ICF_FAILURE;
		register_response.error_cause = ICF_CAUSE_INVALID_REG_REQUEST;
		register_response.options_flag |= ICF_RGM_RESPONSE_CFM;	
        ICF_RGM_APP_TRACE(p_crm_pdb, ICF_TRACE_DEREG_REQ)

		p_crm_pdb->resp_to_be_sent = ICF_FALSE;/*for CFG to not send response*/
		ret_val = icf_rgm_send_response(p_crm_pdb->p_glb_pdb,
							&register_response, p_crm_pdb->p_ecode);			

               /* SPR 17636: CSR 1-5390722: Deallocate the registration context */
               if (ICF_NULL != p_crm_pdb->p_rgm_context)
               {
                   dummy_ret_val = icf_rgm_clear_reg_context(
                        p_crm_pdb, p_crm_pdb->p_rgm_context); 
               }
	}
	else
	{
		/* REGISTER : begin */
		 rgm_uri.addr_val.str_len = ICF_NULL;
		 if (ICF_REG_REQ_TO_PRESENT & p_recv_payload->bit_mask)
		 {
			 /* if the URI doesn't have a user name, return failure */
			 if(ICF_ADDRESS_PLAN_SIP_URI == p_recv_payload->to.addr_type)
			 {
				 /* copy the URI from the request payload */
				 				 p_temp = icf_port_strchr(p_recv_payload->to.addr_val.str, '@');
				 if (p_temp == ICF_NULL)
				 {
					 p_crm_pdb->error_cause = ICF_CAUSE_INVALID_REG_REQUEST;
					 ret_val = ICF_FAILURE;
				 }
				 else
				 {
					 /* copy the URI from the request payload */
					 rgm_uri.addr_type = p_recv_payload->to.addr_type;
					 icf_port_strcpy(rgm_uri.addr_val.str, 
						 p_recv_payload->to.addr_val.str);
					 rgm_uri.addr_val.str_len = 
						 p_recv_payload->to.addr_val.str_len;
				 }
			 }
			 else
			 {
				 rgm_uri.addr_type = p_recv_payload->to.addr_type;
				 icf_port_strcpy(rgm_uri.addr_val.str, 
					 p_recv_payload->to.addr_val.str);
				 rgm_uri.addr_val.str_len = 
				  p_recv_payload->to.addr_val.str_len;
			 }
		 }
		 else
		 {
			 ret_val = ICF_FAILURE;
			 
			 
		 }
		 if ((ICF_FAILURE == ret_val) ||
			 (ICF_FAILURE == icf_rgm_update_rgm_cntxt(
								p_crm_pdb, &rgm_uri)))
		 {
			 /* If error_cause is NULL, it means that 
			  * updating rgm_context failed due to following system errors:
			  * Fetching line data failed.
			  * Copying app data: cloning the lists failed */
			 if (ICF_NULL == p_crm_pdb->error_cause)
			 {
				 ret_val = ICF_FAILURE;
			 }
			 else
			 {
				register_response.request_type = ICF_REQUEST_TYPE_REGISTERING;
				register_response.status = ICF_FAILURE;
				register_response.error_cause = p_crm_pdb->error_cause;
				register_response.options_flag |= ICF_RGM_RESPONSE_CFM;		

				p_crm_pdb->resp_to_be_sent = ICF_FALSE;/*for CFG to not send response*/
				ret_val = icf_rgm_send_response(p_crm_pdb->p_glb_pdb,
								&register_response, p_crm_pdb->p_ecode);			
				p_crm_pdb->error_cause = ICF_NULL;/* So that response is not sent again */

                                /* CSR 1-5390722: Deallocate the registration context */
                                if (ICF_NULL != p_crm_pdb->p_rgm_context)
                                {
                                    dummy_ret_val = icf_rgm_clear_reg_context(
                                            p_crm_pdb, p_crm_pdb->p_rgm_context);
                                }
			 }

		 }
		 else 
         {
	         icf_rgm_update_auth_info(p_crm_pdb);
		 
#ifdef ICF_IPSEC_ENABLED
             
             if (ICF_TRUE == p_crm_pdb->p_config_data->ipsec_enabled)
             {
                 /* Fetch the list of locally supported security algorithms from
                  * NM and save it in REGM global data if not already present */
                 if (ICF_FAILURE == icf_dbm_get_module_glb_data(
                             p_crm_pdb->p_glb_pdb,
                             (icf_int_module_id_t)(ICF_INT_MODULE_REGM),
                             (icf_void_t **)(&p_rgm_glb_data),
                             p_crm_pdb->p_ecode))
                 {
                     ret_val = ICF_FAILURE;
                 }
                 else if (ICF_NULL == p_rgm_glb_data->p_ue_algo_list)
                 {
                     ret_val = icf_rgm_ipsec_nm_sa_fetch_algo(
                             p_crm_pdb,
                             &(p_rgm_glb_data->p_ue_algo_list));	
                 }
                 
                 /* Invoke function to insert additional tags in the tag list */
                 if (ICF_SUCCESS == ret_val)
                 {
                     ret_val = icf_rgm_ipsec_add_sec_agree_tag(p_crm_pdb);
                 }
                 
                 if (ICF_FAILURE == ret_val)
                 {
				    register_response.request_type = ICF_REQUEST_TYPE_REGISTERING;
				    register_response.status = ICF_FAILURE;
				    register_response.error_cause = ICF_CAUSE_IPSEC_FAILURE;
				    register_response.options_flag |= ICF_RGM_RESPONSE_CFM;

				    p_crm_pdb->resp_to_be_sent = ICF_FALSE;
				    ret_val = icf_rgm_send_response(p_crm_pdb->p_glb_pdb,
								    &register_response, p_crm_pdb->p_ecode);
				    p_crm_pdb->error_cause = ICF_NULL;
                    ICF_FUNCTION_EXIT(p_crm_pdb->p_glb_pdb)
                    return ret_val;
                 }
             } /* End if(ipsec_enabled) */
             
#endif /* ICF_IPSEC_ENABLED */

			 if ((ICF_DUMMY_APP_ID == app_id) &&
				 p_crm_pdb->p_config_data->auto_register)
			 {
                 p_crm_pdb->p_rgm_context->auto_reg_flag =
                                ICF_TRUE;
				 p_crm_pdb->p_rgm_context->rgm_duration = 
					 default_duration;
				 /* invoke register_user */
				 ret_val = icf_rgm_register_user( p_crm_pdb,p_crm_pdb->p_rgm_context);
				 p_crm_pdb->resp_to_be_sent = ICF_FALSE;/*for CFG to not send response to app*/
				 
			 }/* auto register case: ends */
			 else if ((ICF_DUMMY_APP_ID == app_id) &&
				 !(p_crm_pdb->p_config_data->auto_register))
			 {/* DUMMY APP ID with auto register OFF*/
				 ret_val = ICF_FAILURE;
			 }
			 else
			 {
				 if (ICF_FAILURE == icf_rgm_save_smallest_dur(
				 p_crm_pdb->p_glb_pdb,p_crm_pdb->p_rgm_context,p_crm_pdb->p_ecode))
				{
					 ret_val = ICF_FAILURE;
				}
				else if (ICF_NULL != 
                    p_crm_pdb->p_rgm_context->rgm_duration)
				{
				 /* invoke register_user */
                 /* SPR 16019: Save app id and line id in rgm context
                  */
                    p_crm_pdb->p_rgm_context->app_id = app_id ;
                    p_crm_pdb->p_rgm_context->line_id = response_line_id;
                    ret_val = icf_rgm_register_user(
                            p_crm_pdb,p_crm_pdb->p_rgm_context);
#ifdef ICF_LOAD_STAT
                 if(ICF_SUCCESS == ret_val)
                 {
                     p_crm_pdb->p_glb_pdb->load_data.register_req++;
                 }
#endif                 
				 p_crm_pdb->resp_to_be_sent = 
                     ICF_FALSE;/*for CFG to not send response to app*/
                } 
				
			 }/* non dummy app case: ends  */			 

		 }/* duration updated and SSA triggered: ends  */

		 
		 if ((ICF_FAILURE == ret_val) &&
			 (ICF_NULL != p_crm_pdb->error_cause))
		 {
			 
		 /*  send CFM with FAILURE: */		
			 register_response.request_type = ICF_REQUEST_TYPE_REGISTERING;
			 register_response.status = ICF_FAILURE;
			 register_response.error_cause = p_crm_pdb->error_cause;
			 register_response.options_flag |= ICF_RGM_RESPONSE_CFM;		
			 
			 p_crm_pdb->resp_to_be_sent = ICF_FALSE;/*for CFG to not send response*/
			 ret_val = icf_rgm_send_response(p_crm_pdb->p_glb_pdb,
				 &register_response, p_crm_pdb->p_ecode);			
                         /* SPR 17636: CSR 1-5390722: Deallocate the registration context */
                         if (ICF_NULL != p_crm_pdb->p_rgm_context)
                         {
                             dummy_ret_val = icf_rgm_clear_reg_context(
                                     p_crm_pdb, p_crm_pdb->p_rgm_context);
                         }
		 }
		 
	}/* REGISTER : ends */
#ifdef ICF_NAT_RPORT_SUPPORT

    if (ICF_CFG_SELF_ADDR_PER_LINE ==
            p_crm_pdb->p_config_data->self_addr_per_line)
    {
        if(ICF_FAILURE == icf_dbm_fetch_line_cntxt_blk(
                    p_crm_pdb->p_glb_pdb, response_line_id,
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

    icf_rgm_set_rport_status(p_crm_pdb);

    if(ICF_NULL == p_rport_config_data)
    {
       ret_val = ICF_FAILURE; 
    }  
    /* Store the initial registration id in rgm_context
    */
    else if(ICF_FALSE == p_rport_config_data->is_initial_reg)
    {
        p_crm_pdb->p_rgm_context->reg_id = ICF_INITIAL_REG_ID;
        if(ICF_NULL == p_rport_config_data->reg_id)
        {
            p_rport_config_data->reg_id = ICF_INITIAL_REG_ID;
            p_rport_config_data->init_reg_duration =\
                                           p_crm_pdb->p_rgm_context->rgm_duration;
            p_rport_config_data->line_id = response_line_id;
            icf_port_memcpy((icf_void_t*)&(p_rport_config_data->\
                 user_address),(icf_void_t*)&(p_crm_pdb->p_rgm_context->\
                     rgm_user_address),(icf_uint32_t)ICF_PORT_SIZEOF\
                (icf_address_st));
            p_rport_config_data->is_initial_reg = ICF_TRUE;
       }
   }
   else if(ICF_NULL == p_crm_pdb->p_rgm_context->reg_id)
   {
       p_crm_pdb->p_rgm_context->reg_id = ICF_INITIAL_REG_ID;
   }


#endif

    /* function exit trace */
    ICF_FUNCTION_EXIT(p_crm_pdb->p_glb_pdb)        
    return ret_val;	



}/* Function icf_rgm_00_reg_req_recvd() ends here */

/****************************************************************************
 * FUNCTION:            icf_rgm_01_reg_req_recvd
 *
 * DESCRIPTION:         This function is invoked REGISTER_REQUEST
 *                      API is received in REGISTERING state.
 *
 *
 * RETURNS:             
 *
 ***************************************************************************/
icf_return_t icf_rgm_01_reg_req_recvd(
        INOUT icf_crm_pdb_st *p_crm_pdb)
{
	
    icf_return_t			ret_val = ICF_SUCCESS;
    icf_register_req_st		*p_recv_msg_payload = ICF_NULL;
    
	icf_app_id_t			temp_app_id = ICF_NULL;
	
	
	icf_rgm_register_resp_st	register_response;
    icf_boolean_t               send_reg_resp = ICF_FALSE;	
    /* function entry trace */
    ICF_FUNCTION_ENTER(p_crm_pdb->p_glb_pdb) 

	ICF_ASSERT(p_crm_pdb->p_glb_pdb,
				(p_crm_pdb->p_glb_pdb->p_recv_msg != ICF_NULL))

	ICF_ASSERT(p_crm_pdb->p_glb_pdb,
				(p_crm_pdb->p_recv_payload != ICF_NULL))
		
		
	temp_app_id = p_crm_pdb->p_glb_pdb->p_recv_msg->hdr.app_id;
	
	
    /* Get the pointer to the payload received with the API */
    p_recv_msg_payload = (icf_register_req_st *)
				(p_crm_pdb->p_recv_payload);
      p_crm_pdb->line_id = p_recv_msg_payload->line_id;
    /* SPR 16019 Handling to validate the register request message
     */
    /* Validate the register request API
     */
    icf_regm_validate_request_header(p_crm_pdb);
    if(ICF_FALSE == p_crm_pdb->is_valid_param)
    {
        ret_val = ICF_FAILURE;
    }
    else 
    {
       icf_rgm_validate_register_api(p_crm_pdb);
       if (ICF_FALSE == p_crm_pdb->is_valid_param)
       {
            p_crm_pdb->error_cause = ICF_CAUSE_INVALID_LINE_ID;
            ret_val = ICF_FAILURE;
        }
    }
    if(ICF_SUCCESS == ret_val)
    {
        /* Check if the expiry value is 0 then send de-register request */
        if ((ICF_REG_REQ_EXPIRY_PRESENT & p_recv_msg_payload->bit_mask) &&
				(ICF_NULL == p_recv_msg_payload->expiry))
        {
#ifdef ICF_NAT_RPORT_SUPPORT
            /* Get the next used registration block on which binding refresh timer 
             * will be started.
             */
            ret_val = icf_rgm_update_nat_binding_refresh_blk(p_crm_pdb->p_glb_pdb,\
                    p_crm_pdb,p_crm_pdb->p_ecode);
#endif	
            ret_val = icf_rgm_handle_dereg(p_crm_pdb->p_glb_pdb, 
                    p_crm_pdb->p_rgm_context, p_crm_pdb, p_crm_pdb->p_ecode);	
        }
        else
        {
            register_response.status = ICF_SUCCESS;
            register_response.error_cause = ICF_NULL;
            register_response.request_type = ICF_REQUEST_TYPE_REGISTERING;
            send_reg_resp = ICF_TRUE;
        }
    }
    else
    {
      register_response.status = ICF_FAILURE;
      register_response.error_cause = p_crm_pdb->error_cause;
      register_response.request_type = ICF_REQUEST_TYPE_REGISTERING;
      send_reg_resp = ICF_TRUE;
    }/* Return success response to application
         */
    if(ICF_TRUE == send_reg_resp)
    {	
        register_response.app_id = temp_app_id;
        register_response.line_id = p_recv_msg_payload->line_id;
        register_response.p_crm_pdb = p_crm_pdb;
        register_response.p_rgm_context = p_crm_pdb->p_rgm_context;
        register_response.options_flag = ICF_NULL;
        register_response.p_request_payload = p_recv_msg_payload;

        register_response.options_flag |= ICF_RGM_RESPONSE_CFM;
        register_response.options_flag |= ICF_RGM_FILL_URIS;/*fill the p-asso member*/
        register_response.options_flag |= ICF_RGM_CTXT_COPY;/*use rgm_context*/
        register_response.options_flag |= ICF_RGM_SINGLE_APP;/*send to single app*/
        p_crm_pdb->resp_to_be_sent = ICF_FALSE;/*for CFG to not send response*/
        ret_val = icf_rgm_send_response(p_crm_pdb->p_glb_pdb,
                &register_response, p_crm_pdb->p_ecode);
    }	
	/* function exit trace */
    ICF_FUNCTION_EXIT(p_crm_pdb->p_glb_pdb)        
    return ret_val;	
	
}/* Function icf_rgm_01_reg_req_recvd() ends here */


 /****************************************************************************
 * FUNCTION:            icf_rgm_02_reg_req_recvd
 *
 * DESCRIPTION:         This function is invoked REGISTER_REQUEST
 *                      API is received in UNREGISTERED state.
 *
 *
 * RETURNS:             
 *
 ***************************************************************************/
icf_return_t icf_rgm_02_reg_req_recvd(
        INOUT icf_crm_pdb_st *p_crm_pdb)
{
	icf_line_id_t			response_line_id = ICF_NULL;
	icf_app_id_t				app_id = ICF_NULL;
	icf_register_req_st			*p_recv_payload = ICF_NULL;
	icf_rgm_register_resp_st	register_response;
	icf_return_t				ret_val = ICF_SUCCESS;
	icf_line_id_t			request_line_id = ICF_NULL;
	icf_boolean_t			app_status = ICF_FALSE;
	icf_rgm_app_ctxt_st		*p_app_info = ICF_NULL;
	
	ICF_FUNCTION_ENTER(p_crm_pdb->p_glb_pdb)
		
		
		ICF_ASSERT(p_crm_pdb->p_glb_pdb,
		(p_crm_pdb->p_glb_pdb->p_recv_msg != ICF_NULL))
		
		ICF_ASSERT(p_crm_pdb->p_glb_pdb,
		(p_crm_pdb->p_recv_payload != ICF_NULL))
		
		p_recv_payload = p_crm_pdb->p_recv_payload;
	
	/* retrieving the app_id from recd_msg */
	app_id = p_crm_pdb->p_glb_pdb->p_recv_msg->hdr.app_id;
	
	request_line_id = ((icf_register_req_st *)
		(p_crm_pdb->p_recv_payload))->line_id;
	
	/* Sending related parameters initialized */
	/* to send the REGISTER_CFM with request_type = ICF_REQUEST_TYPE_DEREGISTER*/
	register_response.p_crm_pdb = p_crm_pdb;
	register_response.p_rgm_context = p_crm_pdb->p_rgm_context;
	
	/* Giving value to app_id will make the send function to 
	* send the CFM to only this application */
	
	register_response.app_id = app_id;
	register_response.line_id = request_line_id;
	register_response.options_flag = ICF_NULL;
	register_response.options_flag |= ICF_RGM_SINGLE_APP;/*send to single app*/
	register_response.options_flag |= ICF_RGM_REQUEST_COPY;/*copy from request payload*/
	register_response.options_flag |= ICF_RGM_RESPONSE_CFM;
	register_response.p_request_payload = p_recv_payload;
	
	
	if ((ICF_REG_REQ_EXPIRY_PRESENT & p_recv_payload->bit_mask) &&
		(ICF_NULL == p_recv_payload->expiry))
	{
		/* Application asked for DEREG */

		/* obtain the Line Id of this application
		* which was stored when the application registered itself */
		app_status = icf_rgm_fetch_app_info(p_crm_pdb,
							p_crm_pdb->p_rgm_context, app_id, &p_app_info);
		
		/* Remove its data from the rgm_context 
		* this function will always return SUCCESS, because this
		* rgm_context definitely contains this app_id */
		if (ICF_TRUE == 	app_status)
		{
			response_line_id = p_app_info->line_id;
			if (request_line_id == response_line_id)
			{
				ret_val = icf_rgm_find_n_remove_app(
					p_crm_pdb, app_id, p_crm_pdb->p_rgm_context,
					p_crm_pdb->p_config_data,p_crm_pdb->p_ecode);
				register_response.request_type = ICF_REQUEST_TYPE_DEREGISTER;
				register_response.status = ICF_SUCCESS;
				register_response.error_cause = ICF_NULL;
			}
			else
			{
				p_crm_pdb->error_cause = ICF_CAUSE_LINE_ID_MISMATCH;
                                register_response.request_type = ICF_REQUEST_TYPE_DEREGISTER;
				register_response.status = ICF_FAILURE;
				register_response.error_cause = p_crm_pdb->error_cause;
				ret_val = ICF_SUCCESS;
			}
		}/* app found in rgm_context: ends */		
                else
                {
                    /* SPR 17636 : CSR 1-5390722 :UMR's removed
                     */
                    register_response.status = ICF_FAILURE;
                    register_response.error_cause = p_crm_pdb->error_cause;
                    register_response.request_type = ICF_REQUEST_TYPE_DEREGISTER;
                    ret_val = ICF_SUCCESS;
                }
	}/* app asked for Dereg: ends  */
	
	else
	{/* Application sent a  non-dereg REGISTER request*/
		/* Send CFM with FAILURE:As network is deactivated */
                register_response.request_type = ICF_REQUEST_TYPE_DEREGISTER;
		register_response.status = ICF_FAILURE;
		register_response.error_cause = ICF_CAUSE_NW_DEACTIVATED;		
	}	
	
	p_crm_pdb->resp_to_be_sent = ICF_FALSE;/*for CFG to not send response*/
	ret_val = icf_rgm_send_response(p_crm_pdb->p_glb_pdb,
					&register_response, p_crm_pdb->p_ecode);			

	/* function exit trace */
    ICF_FUNCTION_EXIT(p_crm_pdb->p_glb_pdb)        
    return ret_val;	
	
}/* Function icf_rgm_02_reg_req_recvd() ends here */

/****************************************************************************
 * FUNCTION:            icf_rgm_03_reg_req_recvd
 *
 * DESCRIPTION:         This function is invoked REGISTER_REQUEST
 *                      API is received in REGISTERED state.
 *
 *
 * RETURNS:             
 *
 ***************************************************************************/
icf_return_t icf_rgm_03_reg_req_recvd(
        INOUT icf_crm_pdb_st *p_crm_pdb)
{
	icf_line_id_t			response_line_id = ICF_NULL;
	icf_app_id_t				app_id = ICF_NULL;
	icf_register_req_st			*p_recv_payload = ICF_NULL;
	icf_rgm_register_resp_st	register_response;
	icf_return_t				ret_val = ICF_SUCCESS;
	icf_address_st       rgm_uri;/* The URI that will be used to send REGISTER */
	icf_boolean_t		uri_present = ICF_TRUE;
	icf_boolean_t		dummy_ret_val = ICF_SUCCESS;
    
	ICF_ASSERT(p_crm_pdb->p_glb_pdb,
				(p_crm_pdb->p_glb_pdb->p_recv_msg != ICF_NULL))

	ICF_ASSERT(p_crm_pdb->p_glb_pdb,
			(p_crm_pdb->p_recv_payload != ICF_NULL))

	

	p_recv_payload = p_crm_pdb->p_recv_payload;

	/* retrieving the app_id from recd_msg */
	app_id = p_crm_pdb->p_glb_pdb->p_recv_msg->hdr.app_id;

	/* This is for the case: auto_activate OFF and auto_register ON 
	 * and the application activates the network. So 
     * REGISTER is to be sent only
	 * for the first time the network is activated. 
     * Not for each time the network is
	 * activated by the application */
	if (ICF_DUMMY_APP_ID == app_id)
    {
       
		return ICF_SUCCESS;
    }

	response_line_id = ((icf_register_req_st *)
						(p_crm_pdb->p_recv_payload))->line_id;

	p_crm_pdb->error_cause = ICF_NULL;
        p_crm_pdb->line_id = p_recv_payload->line_id;

	rgm_uri.addr_val.str_len = ICF_NULL;

	/* Basic values for Sending */
	register_response.p_crm_pdb = p_crm_pdb;
	register_response.p_rgm_context = p_crm_pdb->p_rgm_context;
	
	/* Giving value to app_id will make the send function to 
	* send the CFM to only this application */
	
	register_response.app_id = app_id;
	register_response.line_id = response_line_id;
	register_response.request_type = ICF_NULL;/*klocwork warning removal */
	register_response.options_flag = ICF_NULL;
	register_response.options_flag |= ICF_RGM_SINGLE_APP;/*send to single app*/
		
	register_response.p_request_payload = p_recv_payload;

    /* SPR 16019: Update app id and line id in rgm_context if state Registered
     */
    if(ICF_RGM_03_REGISTERED == p_crm_pdb->p_rgm_context->rgm_state)
    {
        p_crm_pdb->p_rgm_context->app_id = app_id;
        p_crm_pdb->p_rgm_context->line_id = response_line_id;
    }
	if ((ICF_REG_REQ_EXPIRY_PRESENT & p_recv_payload->bit_mask) &&
				(ICF_NULL == p_recv_payload->expiry))
	{
#ifdef ICF_NAT_RPORT_SUPPORT
        /* Get the next used registration block on which binding refresh timer 
         * will be started.
         */
        ret_val = icf_rgm_update_nat_binding_refresh_blk(p_crm_pdb->p_glb_pdb,\
            p_crm_pdb,p_crm_pdb->p_ecode);
#endif	
		ret_val = icf_rgm_handle_dereg(p_crm_pdb->p_glb_pdb, 
				p_crm_pdb->p_rgm_context, p_crm_pdb, p_crm_pdb->p_ecode);
                register_response.request_type = ICF_REQUEST_TYPE_DEREGISTERING;

	}
	else
	{/* non dereg scenario: begins */
		
		if(ICF_REG_REQ_TO_PRESENT & p_recv_payload->bit_mask)
		{/* To value present in payload : begins */
#ifdef IMS_CLIENT
ICF_CHECK_IMS_START(p_crm_pdb->p_glb_pdb,ICF_TRUE)

            dummy_ret_val = icf_rgm_exists_uri(p_crm_pdb->p_glb_pdb,
				p_crm_pdb->p_rgm_context,&(p_recv_payload->to),&uri_present,p_crm_pdb->p_ecode);
			dummy_ret_val = ICF_SUCCESS;
			
			if (ICF_FALSE == uri_present)
			{				
                /*prepare CFM with FAILURE to app: for ecode: Barred URI:
				Give the Set of P-Assoc URI to register*/
				register_response.options_flag |= ICF_RGM_RESPONSE_CFM;
				register_response.options_flag |= ICF_RGM_FILL_URIS;/*fill the p-asso member*/
				register_response.options_flag |= ICF_RGM_CTXT_COPY;/*use rgm_context*/
				register_response.options_flag |= ICF_RGM_SINGLE_APP;/*send to single app*/
				register_response.p_rgm_context = p_crm_pdb->p_rgm_context;
				
				register_response.status = ICF_FAILURE;
				register_response.error_cause = ICF_CAUSE_PUBLIC_URI_BARRED;
				register_response.request_type = ICF_REQUEST_TYPE_REGISTERING;
				p_crm_pdb->resp_to_be_sent = ICF_FALSE;/*for CFG to not send response*/
				ret_val = icf_rgm_send_response(p_crm_pdb->p_glb_pdb,
					&register_response, p_crm_pdb->p_ecode);			

				dummy_ret_val = ICF_FAILURE;
				
			}/* Uri in api payload not present in rgm_context: ends  */
ICF_CHECK_IMS_END
#endif
			if (ICF_TRUE == uri_present)
            {
				/* copy the URI from the request payload */
				rgm_uri.addr_type = p_recv_payload->to.addr_type;
				icf_port_strcpy(rgm_uri.addr_val.str, 
					p_recv_payload->to.addr_val.str);
				rgm_uri.addr_val.str_len = 
					p_recv_payload->to.addr_val.str_len;

			}
			
		}/* To value present in payload : ends */
		
		if (ICF_FAILURE != dummy_ret_val)
		{/* URI found in rgm_context or URI not present in TO field */
		
			if (ICF_FAILURE == icf_rgm_update_rgm_cntxt(
				p_crm_pdb, &rgm_uri))
			{
				/* If error_cause is NULL, it means that 
				 * updating rgm_context failed due to following system errors:
				 * Fetching line data failed.
				 * Copying app data: cloning the lists failed */

				if (ICF_NULL == p_crm_pdb->error_cause)
				{
					ret_val = ICF_FAILURE;
				}
				else
				{
					register_response.request_type = ICF_REQUEST_TYPE_REGISTERING;
					register_response.status = ICF_FAILURE;
					register_response.error_cause = p_crm_pdb->error_cause;
					register_response.options_flag |= ICF_RGM_RESPONSE_CFM;		
					register_response.options_flag |= ICF_RGM_FILL_URIS;/*fill the p-asso member*/
					register_response.options_flag |= ICF_RGM_CTXT_COPY;/*use rgm_context*/
					register_response.options_flag |= ICF_RGM_SINGLE_APP;/*send to single app*/
					register_response.p_rgm_context = p_crm_pdb->p_rgm_context;
					
					p_crm_pdb->resp_to_be_sent = ICF_FALSE;/*for CFG to not send response*/
					ret_val = icf_rgm_send_response(p_crm_pdb->p_glb_pdb,
						&register_response, p_crm_pdb->p_ecode);			
					p_crm_pdb->error_cause = ICF_NULL;/* So that response is not sent again */
					
				}
				
			}
			else 
			{
                icf_rgm_update_auth_info(p_crm_pdb);
				if (ICF_FAILURE == icf_rgm_save_smallest_dur(
					p_crm_pdb->p_glb_pdb,p_crm_pdb->p_rgm_context,
                    p_crm_pdb->p_ecode))
				{
					ret_val = ICF_FAILURE;
				}
				else if (ICF_NULL != 
                        p_crm_pdb->p_rgm_context->rgm_duration)
				{
					/* invoke register_user */
					ret_val = icf_rgm_register_user( p_crm_pdb,p_crm_pdb->p_rgm_context);
					p_crm_pdb->resp_to_be_sent = ICF_FALSE;/*for CFG to not send response to app*/
				}
                else if((ICF_NULL == 
                        p_crm_pdb->p_rgm_context->rgm_duration) &&
                        (ICF_IS_BIT_SET(p_crm_pdb->p_rgm_context->indicator,
                        ICF_RGM_NFY_ON_PROBATION)))
                {
                    /* rgm_ctx is on probation: set it to UNREGISTERED state*/
                    p_crm_pdb->p_rgm_context->rgm_state =
                            ICF_RGM_02_UNREGISTERED;
                    /* Stop the timer */
                 icf_rgm_stop_timer(p_crm_pdb);

                }

				
				
			}/* duration updated and SSA triggered: ends  */
			
			
			if ((ICF_FAILURE == ret_val) &&
				(ICF_NULL != p_crm_pdb->error_cause))
			{
				
			/* send CFM with FAILURE: 
				* as it is not even REGed now */
                         if(ICF_NULL == register_response.request_type)
                         {
                                register_response.request_type = ICF_REQUEST_TYPE_DEREGISTER;
                         }       		
				register_response.status = ICF_FAILURE;
				register_response.error_cause = p_crm_pdb->error_cause;
				register_response.options_flag |= ICF_RGM_RESPONSE_CFM;		
				
				p_crm_pdb->resp_to_be_sent = ICF_FALSE;/*for CFG to not send response*/
				ret_val = icf_rgm_send_response(p_crm_pdb->p_glb_pdb,
					&register_response, p_crm_pdb->p_ecode);			
			}
			
		}/* URI found in rgm_context or URI not present in TO field: ends here  */
		
	}/* non dereg scenario ends*/

	/* function exit trace */
    ICF_FUNCTION_EXIT(p_crm_pdb->p_glb_pdb)        
    return ret_val;	


}/* Function icf_rgm_03_reg_req_recvd() ends here */



/****************************************************************************
 * FUNCTION:            icf_rgm_04_reg_req_recvd
 *
 * DESCRIPTION:         This function is invoked REGISTER_REQUEST
 *                      API is received in AUTHORIZING state.
 *
 *
 * RETURNS:             
 *
 ***************************************************************************/
icf_return_t icf_rgm_04_reg_req_recvd(
											INOUT icf_crm_pdb_st *p_crm_pdb)
{
	
	
    icf_return_t			ret_val = ICF_SUCCESS;
    icf_register_req_st		*p_recv_msg_payload = ICF_NULL;
    
	icf_app_id_t			temp_app_id = ICF_NULL;
	
	icf_rgm_register_resp_st	register_response;
    icf_boolean_t               send_reg_resp=ICF_FALSE;
	
	
    /* function entry trace */
    ICF_FUNCTION_ENTER(p_crm_pdb->p_glb_pdb) 
		
		
	ICF_ASSERT(p_crm_pdb->p_glb_pdb, 
							(ICF_NULL != p_crm_pdb->p_glb_pdb->p_recv_msg))

	temp_app_id = p_crm_pdb->p_glb_pdb->p_recv_msg->hdr.app_id;
	
	
    /* Get the pointer to the payload received with the API */
    p_recv_msg_payload = (icf_register_req_st *)
						(p_crm_pdb->p_recv_payload);

    /* SPR 17622: Fix for CSR 1-5385118: Handling to validate the register
     * request message. If validation is successful then if it is a deregister
     * request then abort ongoing register transaction and send deregister
     * else send successful response to application.
     */
    icf_regm_validate_request_header(p_crm_pdb);
    if(ICF_FALSE == p_crm_pdb->is_valid_param)
    {
        ret_val = ICF_FAILURE;
    }
    else 
    {
       icf_rgm_validate_register_api(p_crm_pdb);
       if (ICF_FALSE == p_crm_pdb->is_valid_param)
       {
            p_crm_pdb->error_cause = ICF_CAUSE_INVALID_LINE_ID;
            ret_val = ICF_FAILURE;
        }
    }
    if(ICF_SUCCESS == ret_val)
    {
        /* Check if the expiry value is 0 then send de-register request */
        if ((ICF_REG_REQ_EXPIRY_PRESENT & p_recv_msg_payload->bit_mask) &&
				(ICF_NULL == p_recv_msg_payload->expiry))
        {
#ifdef ICF_NAT_RPORT_SUPPORT
            /* Get the next used registration block on which binding refresh timer 
             * will be started.
             */
            ret_val = icf_rgm_update_nat_binding_refresh_blk(p_crm_pdb->p_glb_pdb,\
                    p_crm_pdb,p_crm_pdb->p_ecode);
#endif	
            ret_val = icf_rgm_handle_dereg(p_crm_pdb->p_glb_pdb, 
                    p_crm_pdb->p_rgm_context, p_crm_pdb, p_crm_pdb->p_ecode);
        }
        else
        {
            register_response.status = ICF_SUCCESS;
            register_response.error_cause = ICF_NULL;
            register_response.request_type = ICF_REQUEST_TYPE_REGISTERING;
            send_reg_resp = ICF_TRUE;
        }
    }
    else
    {
      register_response.status = ICF_FAILURE;
      register_response.error_cause = p_crm_pdb->error_cause;
      register_response.request_type = ICF_REQUEST_TYPE_REGISTERING;
      send_reg_resp = ICF_TRUE;
    }/* Return success response to application */

    if(ICF_TRUE == send_reg_resp)
    {	
   	    register_response.app_id = temp_app_id;
        register_response.line_id = p_recv_msg_payload->line_id;
        register_response.p_crm_pdb = p_crm_pdb;
        register_response.p_rgm_context = p_crm_pdb->p_rgm_context;
        register_response.options_flag = ICF_NULL;
        register_response.p_request_payload = p_recv_msg_payload;
	
        /* REGISTER_CFM with FAILURE and error_cause as ICF_CAUSE_ONGOING_REGISTRATION
        * and request_type as ICF_REQUEST_TYPE_AUTHORIZING */
        register_response.options_flag |= ICF_RGM_RESPONSE_CFM;
        register_response.options_flag |= ICF_RGM_FILL_URIS;/*fill the p-asso member*/
        register_response.options_flag |= ICF_RGM_CTXT_COPY;/*use rgm_context*/
        register_response.options_flag |= ICF_RGM_SINGLE_APP;/*send to single app*/
	
        p_crm_pdb->resp_to_be_sent = ICF_FALSE;/*for CFG to not send response*/
        ret_val = icf_rgm_send_response(p_crm_pdb->p_glb_pdb,
                &register_response, p_crm_pdb->p_ecode);
    } /* Fix for SPR 17622 Ends */

	ICF_FUNCTION_EXIT(p_crm_pdb->p_glb_pdb)
	return ret_val;	
	
}/* Function icf_rgm_04_reg_req_recvd() ends here */


 /****************************************************************************
 * FUNCTION:            icf_rgm_05_reg_req_recvd
 *
 * DESCRIPTION:         This function is invoked REGISTER_REQUEST
 *                      API is received in DEREGISTETRING state.
 *
 *
 * RETURNS:             
 *
***************************************************************************/
icf_return_t icf_rgm_05_reg_req_recvd(
											INOUT icf_crm_pdb_st *p_crm_pdb)
{
	
    icf_return_t			ret_val = ICF_SUCCESS;
    
	icf_rgm_register_resp_st	register_response;
	
	icf_line_id_t			response_line_id = ICF_NULL;
	icf_app_id_t				app_id = ICF_NULL;
	icf_register_req_st			*p_recv_payload = ICF_NULL;
    icf_boolean_t               send_reg_resp = ICF_FALSE;
    icf_rgm_context_st          *p_rgm_context;
    /* Variables used for Code Area reduction */
		
	

	/* function entry trace */
    ICF_FUNCTION_ENTER(p_crm_pdb->p_internal_msg->msg_hdr.p_glb_pdb) 

	ICF_ASSERT(p_crm_pdb->p_glb_pdb,
				(p_crm_pdb->p_glb_pdb->p_recv_msg != ICF_NULL))

	ICF_ASSERT(p_crm_pdb->p_glb_pdb,
			(p_crm_pdb->p_recv_payload != ICF_NULL))

	p_recv_payload = p_crm_pdb->p_recv_payload;
    /* Get the pointer to registration context */
    p_rgm_context = p_crm_pdb->p_rgm_context;
    ICF_ASSERT(
            p_crm_pdb->p_internal_msg->msg_hdr.p_glb_pdb,
            (ICF_NULL != p_rgm_context))
	/* retrieving the app_id from recd_msg */
	app_id = p_crm_pdb->p_glb_pdb->p_recv_msg->hdr.app_id;

	response_line_id = ((icf_register_req_st *)
						(p_crm_pdb->p_recv_payload))->line_id;
	
    /* SPR 16019: Validate the register API received and take appropriate action.
     */
	 /* Validate the register request API
     */
    icf_regm_validate_request_header(p_crm_pdb);
    if(ICF_FALSE == p_crm_pdb->is_valid_param)
    {
        ret_val = ICF_FAILURE;
    }
    else 
    {
       icf_rgm_validate_register_api(p_crm_pdb);
       if (ICF_FALSE == p_crm_pdb->is_valid_param)
       {
            p_crm_pdb->error_cause = ICF_CAUSE_INVALID_LINE_ID;
            ret_val = ICF_FAILURE;
        }
    }
    if(ICF_SUCCESS == ret_val)
    {
        /* Check if the expiry value is non- 0 then send register request */
        if ((ICF_REG_REQ_EXPIRY_PRESENT & p_recv_payload->bit_mask) &&
				(ICF_NULL != p_recv_payload->expiry))
        {
            p_rgm_context->indicator |= ICF_RGM_CLEAR_ONGOING_REGISTRATION;
            ret_val = icf_rgm_03_reg_req_recvd(p_crm_pdb);
            if(ICF_SUCCESS == ret_val)
            {
                register_response.p_crm_pdb = p_crm_pdb;
                register_response.app_id = p_rgm_context->app_id;
                register_response.line_id = p_rgm_context->line_id;
                register_response.options_flag = ICF_NULL;
                register_response.options_flag |= ICF_RGM_FILL_URIS;
                /*fill the p-asso member*/
                register_response.options_flag |= ICF_RGM_CTXT_COPY;
                /*use rgm_ctxt for To/From*/
                register_response.p_rgm_context = p_rgm_context;
                register_response.p_request_payload = ICF_NULL;
                register_response.request_type = ICF_REQUEST_TYPE_DEREGISTERING;
                if(ICF_FAILURE == icf_rgm_send_app_resp_on_succ(p_crm_pdb,\
                            &register_response))
                {
                    ret_val = ICF_FAILURE;
                }
                send_reg_resp = ICF_FALSE;
            }
            /* Update app id and line id in rgm context
             */
            p_rgm_context->app_id = app_id;
            p_rgm_context->line_id = response_line_id;
            /* SPR 16109: Reset the app_indicator for app
             * to send register cfm
             */
            if(ICF_NULL != app_id)
            {
                p_rgm_context->p_app_data_list[app_id-1].app_indicator &=
                    ~(ICF_RGM_APP_CFM_SENT);
            }
            else
            {
                p_rgm_context->p_app_data_list[app_id].app_indicator &=
                    ~(ICF_RGM_APP_CFM_SENT);

            }

        }
        else
        {
            register_response.status = ICF_SUCCESS;
            register_response.error_cause = ICF_NULL;
            send_reg_resp = ICF_TRUE;
        }
    }
    else
    {
      register_response.status = ICF_FAILURE;
      register_response.error_cause = p_crm_pdb->error_cause;
      send_reg_resp = ICF_TRUE;
    }/* Return success response to application
         */
    if(ICF_TRUE == send_reg_resp)
    {
        register_response.request_type = ICF_REQUEST_TYPE_DEREGISTERING;
	
	register_response.app_id = app_id;
	register_response.line_id = p_recv_payload->line_id;
	register_response.p_crm_pdb = p_crm_pdb;
	register_response.p_rgm_context = p_crm_pdb->p_rgm_context;
	register_response.options_flag = ICF_NULL;
	register_response.p_request_payload = p_recv_payload;
	
	
	register_response.options_flag |= ICF_RGM_RESPONSE_CFM;
	register_response.options_flag |= ICF_RGM_FILL_URIS;/*fill the p-asso member*/
	register_response.options_flag |= ICF_RGM_CTXT_COPY;/*use rgm_context*/
	register_response.options_flag |= ICF_RGM_SINGLE_APP;/*send to single app*/
	
	p_crm_pdb->resp_to_be_sent = ICF_FALSE;/*for CFG to not send response*/
	ret_val = icf_rgm_send_response(p_crm_pdb->p_glb_pdb,
		&register_response, p_crm_pdb->p_ecode);
    }

	ICF_FUNCTION_EXIT(p_crm_pdb->p_glb_pdb)
	return ret_val;	
	
}/* Function icf_rgm_05_reg_req_recvd() ends here */

/****************************************************************************
 * FUNCTION:            icf_rgm_01_04_timeout
 *
 * DESCRIPTION:         This function is invoked when ICF_RGM_FAILURE_TIMEOUT
 *                      event from SSA  is received in 
 *						REGISTETRING or AUTHORIZING state.
 *
 * RETURNS:             
 *
 ***************************************************************************/
icf_return_t icf_rgm_01_04_timeout(
        INOUT icf_crm_pdb_st *p_crm_pdb)
{

	icf_rgm_nw_resp_st *p_nw_resp = ICF_NULL;
    icf_rgm_context_st *p_rgm_context = ICF_NULL;
    icf_return_t ret_val = ICF_SUCCESS;

#ifdef ICF_NAT_RPORT_SUPPORT
    icf_rport_config_data_st            *p_rport_config_data = ICF_NULL;
    icf_line_data_st                    *p_line_data = ICF_NULL;
#endif


#ifdef IMS_CLIENT
	icf_return_t dummy_ret_val = ICF_SUCCESS;
	icf_uint8_t			*p_temp_msg_data = ICF_NULL;
	/* For storing the msg_data locally. */
	icf_uint32_t			temp_msg_id = ICF_NULL;
	icf_uint32_t			temp_pay_len = ICF_NULL;
#endif
	icf_rgm_register_resp_st	register_response;
	
    /* Variables used for Code Area reduction */
    icf_glb_pdb_st *p_glb_pdb = ICF_NULL;
	
	

    icf_boolean_t        is_ims_max_retry = ICF_TRUE;
    icf_boolean_t        clear_ctx = ICF_FALSE;
	/* function entry trace */
    ICF_FUNCTION_ENTER(p_crm_pdb->p_internal_msg->msg_hdr.p_glb_pdb) 
        
	/* Typecast the data in internal message to network response structure */ 
	p_nw_resp = (icf_rgm_nw_resp_st *)(p_crm_pdb->
				p_internal_msg->p_msg_data);
    ICF_ASSERT(
		p_crm_pdb->p_internal_msg->msg_hdr.p_glb_pdb,
		(ICF_NULL != p_nw_resp))
		
		
	/* Get the pointer to registration context */
     /*SPR Fixed 17869 .If message is from mrm fill the rgm_context form
     p_crm_pdb ,else fill it from p_nw_resp */
     if(ICF_MSG_FROM_MRM == p_crm_pdb->p_internal_msg->msg_hdr.msg_id)
     {
         p_rgm_context = p_crm_pdb->p_rgm_context;
     }
     else
     {
         p_rgm_context = p_nw_resp->p_rgm_context;
     }
   
    ICF_ASSERT(
		p_crm_pdb->p_internal_msg->msg_hdr.p_glb_pdb,
		(ICF_NULL != p_rgm_context))
		p_crm_pdb->p_rgm_context = p_rgm_context;
    
    
    /* Get the pointer to global PDB */
    p_glb_pdb = p_crm_pdb->p_internal_msg->msg_hdr.p_glb_pdb;
    ICF_ASSERT(
		p_crm_pdb->p_internal_msg->msg_hdr.p_glb_pdb,
		(ICF_NULL != p_glb_pdb))
	p_crm_pdb->p_glb_pdb = p_glb_pdb;
	
	
    /* Get the pointer to ecode */
    p_crm_pdb->p_ecode = &(p_crm_pdb->p_internal_msg->msg_hdr.ecode);
    ICF_ASSERT(
		p_crm_pdb->p_internal_msg->msg_hdr.p_glb_pdb,
		(ICF_NULL != p_crm_pdb->p_ecode))

	/* Sending related generic members */
	/* Not setting the options_flag with SINGLE_APP
	* value will make the send function to 
	* Loop on the application list to send the register CFM/ IND
	* to the applications that are kept in this rgm_context */
	register_response.p_crm_pdb = p_crm_pdb;
    /* SPR 16019: App id and Line id from rgm_context
     */
	register_response.app_id = p_rgm_context->app_id;
    register_response.line_id = p_rgm_context->line_id;
	register_response.options_flag = ICF_NULL;
	register_response.options_flag |= ICF_RGM_FILL_URIS;/*fill the p-asso member*/
	register_response.options_flag |= ICF_RGM_CTXT_COPY;/*use rgm_ctxt for To/From*/
	register_response.p_rgm_context = p_rgm_context;
	register_response.p_request_payload = ICF_NULL;
        /* Fill the registration request type based on the state of registration
         *  
         */
        if(ICF_RGM_01_REGISTERING == p_rgm_context->rgm_state)
       {
          register_response.request_type = ICF_REQUEST_TYPE_REGISTERING;
       }
        else if(ICF_RGM_04_AUTHORIZING == p_rgm_context->rgm_state)
       {
         register_response.request_type = ICF_REQUEST_TYPE_AUTHORIZING;
       }

#ifdef IMS_CLIENT
ICF_CHECK_IMS_START(p_glb_pdb,ICF_TRUE)
    p_rgm_context->retry_count++;
	if (ICF_RGM_MAX_RETRY <= p_rgm_context->retry_count)
	{
        is_ims_max_retry = ICF_TRUE;
    }
    else
    {
        is_ims_max_retry = ICF_FALSE;
    }
ICF_CHECK_IMS_END
/*#else*/
	/* There is no funda of Retrying Registration 5 times so we will simpy send
	 * Registration failure to Application 
	 */
#endif	
    if(ICF_TRUE == is_ims_max_retry)
    {
        /*Send CFM/IND to applications for FAILURE*/
        /* Clear the REG context */
        register_response.status = ICF_FAILURE;
        if(ICF_ERROR_DNS_QUERY_FAILURE == p_crm_pdb->error_cause)
        {
           register_response.error_cause = ICF_ERROR_DNS_QUERY_FAILURE;
           p_crm_pdb->error_cause = ICF_ERROR_UNDEFINED;
        }
        else
        {
           register_response.error_cause = ICF_CAUSE_REGISTERATION_RETRY_OVER;
        }
        register_response.request_type = ICF_NULL;
    
#ifdef ICF_NAT_RPORT_SUPPORT
        if( (ICF_RGM_05_DEREGISTERING == p_rgm_context->rgm_state) &&
            (ICF_INITIAL_RPORT_COMPLETED == p_rgm_context->rport_status))
        {
            /* do nothing */
        }
        else
        {
           ret_val = icf_rgm_send_response(p_glb_pdb,
              &register_response, p_crm_pdb->p_ecode);

           ICF_RGM_SET_ECODE(p_crm_pdb, ICF_UNABLE_TO_SEND_REG_REQ)
        }
#else
        ret_val = icf_rgm_send_response(p_glb_pdb,
            &register_response, p_crm_pdb->p_ecode);

        ICF_RGM_SET_ECODE(p_crm_pdb, ICF_UNABLE_TO_SEND_REG_REQ)
#endif


 if (ICF_TRUE == p_rgm_context->auto_reg_flag)
        {
            /* Reattempt timer should be started. 
             * because this rgm_context had auto_reg ON. */
            /* AUTO_REG was selected on this rgm_ctx, 
            * hence reattempt timer needs to be started */                 
            
            
            /* Set the state */
            p_rgm_context->rgm_state = ICF_RGM_02_UNREGISTERED;
            /* Set the value in indicator bit */
            p_rgm_context->indicator |= ICF_RGM_UNREG_ON_REATTEMPT;

            /* Start REATTEMPT_TIMER */
            ret_val = icf_rgm_start_timer(
                p_glb_pdb,
                (icf_timer_type_t)(ICF_TIMER_TYPE_RGM_REATTEMPT),
                p_rgm_context,
                p_crm_pdb->p_ecode);

            if (ICF_FAILURE == ret_val)
            {
                clear_ctx = ICF_TRUE;
                
                
            }                 
            
        }/* auto_reg set for this ctx: hence reattempt timer started */

        if ((ICF_TRUE == clear_ctx) ||
            (ICF_FALSE == p_rgm_context->auto_reg_flag))
        {
            /* Auto reg not set for this rgm_context 
             * hence no more retries shall be done */

#ifdef ICF_IPSEC_ENABLED
            
            /* This can be expiry of Registration transaction timer or SA deletion 
            * guard timer */
            if (ICF_TRUE == p_crm_pdb->p_config_data->ipsec_enabled)
            {
                /* Registration transaction timeout */
                
                /* This event occurs on Registration transaction timeout. Timer F 
                * expiry would mean we need to discard all ongoing transactions and 
                * dialogs silently and re-attempt registration, as if it was the 
                * initial registration so it will go unprotected */
                
                /* Delete any existing SA before sending fresh register request */
                ((icf_rgm_glb_data_st *)(p_crm_pdb->p_rgm_glb_data))->sa_identifier = ICF_RGM_IPSEC_SA_ALL;
                
                if (ICF_FAILURE == icf_rgm_ipsec_sa_clean_up(
                    p_crm_pdb,
                    p_crm_pdb->p_rgm_context->rgm_state))
                {
                    ret_val = ICF_FAILURE;
                    return ICF_FAILURE;
                }
            }
#endif /* ICF_IPSEC_ENABLED */
           
#ifdef ICF_NAT_RPORT_SUPPORT
     
 if (ICF_CFG_SELF_ADDR_PER_LINE ==
            p_crm_pdb->p_config_data->self_addr_per_line)
    {
        if(ICF_FAILURE == icf_dbm_fetch_line_cntxt_blk(
                    p_crm_pdb->p_glb_pdb, 
                    p_crm_pdb->p_rgm_context->line_id,
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

	if((ICF_NULL != p_rport_config_data) && 
           (ICF_TRUE == p_rport_config_data->is_rport_reqd))
	{
		/*Send call clear indication to CM */
		ret_val = icf_cfg_cm_call_clear_ind(p_crm_pdb);
	}
#endif 
            /* Clear registration context */
            ret_val = icf_rgm_clear_reg_context(
                p_crm_pdb,
                p_rgm_context); 
            ret_val = ICF_SUCCESS;
            

        }/* 5 times retried and auto_reg not set for this context
         * else the starting of timer failed: hence clearing needed */

    }/* ims_max_retry = TRUE: */

#ifdef IMS_CLIENT
ICF_CHECK_IMS_START(p_glb_pdb,ICF_TRUE)
    /*If the max retry is less than 5*/
    if (ICF_FALSE == is_ims_max_retry)
	{
		/* Indicate SSA to clear the REG Object*/
		/* Copy the current msg_data locally */
		if (ICF_NULL != p_crm_pdb->p_internal_msg->p_msg_data)
		{
			p_temp_msg_data = p_crm_pdb->p_internal_msg->p_msg_data;
			temp_msg_id = p_crm_pdb->p_internal_msg->msg_hdr.msg_id;
			temp_pay_len = p_crm_pdb->p_internal_msg->msg_hdr.payload_length;
		}
		/* Fill CRM PDB Structure */
		p_crm_pdb->p_internal_msg->p_msg_data = 
					(icf_uint8_t *)(p_rgm_context);
		p_crm_pdb->p_internal_msg->msg_hdr.msg_id = 
					ICF_CLEAR_REG_OBJECT;
		p_crm_pdb->p_internal_msg->msg_hdr.payload_length = 
					(icf_uint8_t)(ICF_PORT_SIZEOF
							(icf_rgm_context_st));
		
		/* Call SSA to clear Registration Call Object */
		ret_val = icf_ssa_process_mesg(
					(icf_internal_msg_st *)
					(p_crm_pdb->p_internal_msg));
		
		/* Copy the old msg_data back */
		if (ICF_NULL != p_temp_msg_data)
		{
			p_crm_pdb->p_internal_msg->p_msg_data = p_temp_msg_data;
			p_crm_pdb->p_internal_msg->msg_hdr.msg_id = temp_msg_id;
			p_crm_pdb->p_internal_msg->
					msg_hdr.payload_length = temp_pay_len;
		}
		
		if (ICF_RGM_ONCE_REGISTERED & p_rgm_context->indicator)    
		{
			/* Forcefully clear dialogs: Indicate to CC and SIC */
			
			/* To clear all the calls: */
            /* sic_clear_all()*/

			icf_cfg_cm_call_clear_ind_wrt_user(p_crm_pdb, 
				&(p_rgm_context->rgm_user_address),(icf_line_id_t)p_crm_pdb->line_id,
				(icf_boolean_t)ICF_TRUE, p_crm_pdb->p_ecode);

		}/* reason was re-registering: so on timeout clear the dialogs */
		
		 /* Clear the contents of auth_data_sent and auth_data_recd 
		  * members of rgm_context */
		if (ICF_NULL != p_rgm_context->auth_data_recd.p_nonce)
		{
			ICF_MEMFREE(p_crm_pdb->p_glb_pdb,
						p_rgm_context->auth_data_recd.p_nonce, 
					ICF_MEM_COMMON, p_crm_pdb->p_ecode, dummy_ret_val)
		}	
		if (ICF_NULL != p_rgm_context->auth_data_sent.p_nonce)
		{
			ICF_MEMFREE(p_crm_pdb->p_glb_pdb,
						p_rgm_context->auth_data_sent.p_nonce, 
					ICF_MEM_COMMON, p_crm_pdb->p_ecode, dummy_ret_val)
		}	
		icf_port_memset(
				(icf_void_t*)&(p_rgm_context->auth_data_recd),
				(icf_uint8_t)ICF_NULL,
				(icf_uint32_t) ICF_PORT_SIZEOF(icf_rgm_auth_data_st));
		icf_port_memset(
				(icf_void_t*)&(p_rgm_context->auth_data_sent),
				(icf_uint8_t)ICF_NULL,
				(icf_uint32_t) ICF_PORT_SIZEOF(icf_rgm_auth_data_st));
		
		/* making error_cause as NULL to be used during auth-processing */
		p_crm_pdb->error_cause = ICF_NULL;
		
		if (ICF_FAILURE == icf_rgm_update_auth_info(p_crm_pdb))
		{
#ifdef IMS_CLIENT
ICF_CHECK_IMS_START(p_glb_pdb,ICF_TRUE)
			if (ICF_NULL != p_crm_pdb->error_cause)
			{
#endif
ICF_CHECK_IMS_END
				/* Send failure responses/indications to applications */
				
				register_response.status = ICF_FAILURE;
				register_response.error_cause = p_crm_pdb->error_cause;
				
				p_crm_pdb->resp_to_be_sent = ICF_FALSE;/*for CFG to not send response*/
				ret_val = icf_rgm_send_response(p_glb_pdb,
					&register_response, p_crm_pdb->p_ecode);
				/* Clear the context */
				ret_val = icf_rgm_clear_reg_context(
                    p_crm_pdb,
                    p_rgm_context);         
				
				ret_val = ICF_SUCCESS;
#ifdef IMS_CLIENT
ICF_CHECK_IMS_START(p_glb_pdb,ICF_TRUE)						
			}/* Applications can be indicated : ends*/
			else
			{/* System error */
				ICF_RGM_SET_ECODE(p_crm_pdb,
					ICF_AUTHENTICATION_INFO_ABSENT)        
					/* Clear the context */
					ret_val = icf_rgm_clear_reg_context(
                    p_crm_pdb,
                    p_rgm_context);         
				
				ret_val = ICF_FAILURE;
				
			}
#endif
ICF_CHECK_IMS_END
			
		}/* update_auth_info() Failure: ends */
		else
		{

#ifdef ICF_IPSEC_ENABLED
        
        /* This can be expiry of Registration transaction timer or SA deletion 
		 * guard timer */
        if (ICF_TRUE == p_crm_pdb->p_config_data->ipsec_enabled)
        {
            /* Registration transaction timeout */
            
            /* This event occurs on Registration transaction timeout. Timer F 
			 * expiry would mean we need to discard all ongoing transactions and 
			 * dialogs silently and re-attempt registration, as if it was the 
			 * initial registration so it will go unprotected */
            
            /* Delete any existing SA before sending fresh register request */
			if ((ICF_NULL != ((icf_rgm_glb_data_st *)(p_crm_pdb->p_rgm_glb_data))->p_new_sa_context) &&
				(ICF_NULL != ((icf_rgm_glb_data_st *)(p_crm_pdb->p_rgm_glb_data))->p_old_sa_context))
			{
				((icf_rgm_glb_data_st *)(p_crm_pdb->p_rgm_glb_data))->sa_identifier = 
					ICF_RGM_IPSEC_SA_ALL;
			}
			else if (ICF_NULL != ((icf_rgm_glb_data_st *)(p_crm_pdb->p_rgm_glb_data))->p_new_sa_context)
			{
				((icf_rgm_glb_data_st *)(p_crm_pdb->p_rgm_glb_data))->sa_identifier = 
					ICF_RGM_IPSEC_SA_NEW;
			}
			else if (ICF_NULL != ((icf_rgm_glb_data_st *)(p_crm_pdb->p_rgm_glb_data))->p_old_sa_context)
			{
				((icf_rgm_glb_data_st *)(p_crm_pdb->p_rgm_glb_data))->sa_identifier = 
					ICF_RGM_IPSEC_SA_OLD;
			}
			if ( !((ICF_NULL == ((icf_rgm_glb_data_st *)(p_crm_pdb->p_rgm_glb_data))->p_new_sa_context) &&
				(ICF_NULL == ((icf_rgm_glb_data_st *)(p_crm_pdb->p_rgm_glb_data))->p_old_sa_context)))
			{
				if (ICF_FAILURE == icf_rgm_ipsec_nm_sa_delete_req(p_crm_pdb))
				{
					ret_val = ICF_FAILURE;
				}
				/* Now delete all IPSec contexts from rgm context */
				else if (ICF_FAILURE == icf_rgm_ipsec_dealloc_sa_context(
												p_crm_pdb))
				{
					ret_val = ICF_FAILURE;
				}
			}
			if( ICF_NULL != ((icf_rgm_glb_data_st *)(p_crm_pdb->p_rgm_glb_data))->p_security_client_params)
			{

				ICF_MEMFREE(
					p_crm_pdb->p_glb_pdb,
					((icf_rgm_glb_data_st *)(p_crm_pdb->p_rgm_glb_data))->p_security_client_params,
					ICF_MEM_COMMON,
					p_crm_pdb->p_ecode,
					ret_val);
			}
		}
#endif /* ICF_IPSEC_ENABLED */

			/* Registration is to be retried */
			if (ICF_SUCCESS == ret_val)
			{
				ret_val = icf_rgm_register_user(
							p_crm_pdb, 
							p_rgm_context);
			}
		}	

	}/* less than 5 times retried: ends here */
ICF_CHECK_IMS_END
#endif
	ICF_FUNCTION_EXIT(p_crm_pdb->p_glb_pdb)
	return ret_val;	
}/* Function icf_rgm_01_04_timeout() ends here */

/****************************************************************************
 * FUNCTION:            icf_rgm_05_timeout
 *
 * DESCRIPTION:         This function is invoked when ICF_RGM_FAILURE_TIMEOUT
 *                      event from SSA  is received in 
 *						DEREGISTETRING state.The reg_context is cleared.
 *
 * RETURNS:             
 *
 ***************************************************************************/
icf_return_t icf_rgm_05_timeout(
        INOUT icf_crm_pdb_st *p_crm_pdb)
{
	
	icf_rgm_nw_resp_st *p_nw_resp = ICF_NULL;
    icf_rgm_context_st *p_rgm_context = ICF_NULL;
    icf_return_t ret_val = ICF_SUCCESS;
	
    /* Variables used for Code Area reduction */
    icf_glb_pdb_st *p_glb_pdb = ICF_NULL;
    icf_rgm_register_resp_st	register_response;	
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
    ICF_ASSERT(
		p_crm_pdb->p_internal_msg->msg_hdr.p_glb_pdb,
		(ICF_NULL != p_rgm_context))
		p_crm_pdb->p_rgm_context = p_rgm_context;
        
    /* Get the pointer to global PDB */
    p_glb_pdb = p_crm_pdb->p_internal_msg->msg_hdr.p_glb_pdb;
    ICF_ASSERT(
		p_crm_pdb->p_internal_msg->msg_hdr.p_glb_pdb,
		(ICF_NULL != p_glb_pdb))
	p_crm_pdb->p_glb_pdb = p_glb_pdb;
	
	    /* Get the pointer to ecode */
    p_crm_pdb->p_ecode = &(p_crm_pdb->p_internal_msg->msg_hdr.ecode);
    ICF_ASSERT(
		p_crm_pdb->p_internal_msg->msg_hdr.p_glb_pdb,
		(ICF_NULL != p_crm_pdb->p_ecode))
        
#ifdef ICF_IPSEC_ENABLED
    
    if (ICF_TRUE == p_crm_pdb->p_config_data->ipsec_enabled)
    {
        /* Invoke utility function for clean up */
        ((icf_rgm_glb_data_st *)(p_crm_pdb->p_rgm_glb_data))->sa_identifier = ICF_RGM_IPSEC_SA_ALL;
        
        ret_val = icf_rgm_ipsec_sa_clean_up(
            p_crm_pdb, ICF_RGM_07_IPSEC_AWT_SA_DEL_RESP);
    } /* End if(ipsec_enabled) */
#endif /* ICF_IPSEC_ENABLED */
#ifdef ICF_STAT_ENABLE
         ICF_STATS(p_crm_pdb->p_glb_pdb->p_stats_info->stat_register.stats_dereg_done)
         (p_crm_pdb->p_glb_pdb->p_stats_info->stat_register.stats_active_reg)--;
#endif         
        /* SPR 16019: Send de-reg failure response to applications.
         */
        
        /* Not setting the options_flag with SINGLE_APP
         * value will make the send function to 
         * Loop on the application list to send the register CFM/ IND
         * to the applications that are kept in this rgm_context */
        register_response.p_crm_pdb = p_crm_pdb;
        register_response.app_id = p_rgm_context->app_id;
        register_response.line_id = p_rgm_context->line_id;
        register_response.options_flag = ICF_NULL;
        register_response.options_flag |= ICF_RGM_FILL_URIS;/*fill the p-asso member*/
        register_response.options_flag |= ICF_RGM_CTXT_COPY;/*use rgm_ctxt for To/From*/
        register_response.p_rgm_context = p_rgm_context;
        register_response.p_request_payload = ICF_NULL;
        register_response.status = ICF_FAILURE;
        register_response.error_cause = ICF_CAUSE_REGISTERATION_RETRY_OVER;
        register_response.request_type = ICF_REQUEST_TYPE_DEREGISTERING;
        if(ICF_RGM_INTERNAL_GENERATED_REG == (p_rgm_context->indicator & \
                    ICF_RGM_INTERNAL_GENERATED_REG))
        {
            register_response.options_flag |= ICF_RGM_RESPONSE_IND;
            p_rgm_context->indicator &=~(ICF_RGM_INTERNAL_GENERATED_REG);
        }
        else
        {
            /*CSR 8052488 SPR 20244*/
            /*Issue : IPTK was sending register confirm with failure 
            response instead of register_status_ind in case deregister 
            request received from application without to address and no 200 OK
            received from peer for deregister */ 
            /*Fix : Fix done such that if Confirmation has already been
            sent then status indication should be sent. */
            if (ICF_RGM_APP_CFM_SENT    &
                        p_rgm_context->p_app_data_list[(p_rgm_context->app_id)-1].app_indicator)
            {
                register_response.options_flag |= ICF_RGM_RESPONSE_IND;

             }
             else
            {
                    
            register_response.options_flag |= ICF_RGM_RESPONSE_CFM;
        }
        }

        p_crm_pdb->resp_to_be_sent = ICF_FALSE;/*for CFG to not send response*/
        ret_val = icf_rgm_send_response(p_glb_pdb,
            &register_response, p_crm_pdb->p_ecode);
       
        ICF_RGM_SET_ECODE(p_crm_pdb, ICF_UNABLE_TO_SEND_REG_REQ)
	    /* Clear the context */
	    ret_val = icf_rgm_clear_reg_context(
                        p_crm_pdb,
                        p_rgm_context);         
				    
	    ret_val = ICF_SUCCESS;

	ICF_FUNCTION_EXIT(p_crm_pdb->p_glb_pdb)
	return ret_val;	

}/* Function icf_rgm_05_timeout() ends here */

/****************************************************************************
 * FUNCTION:            icf_rgm_01_03_04_05_nw_deactivated
 *
 * DESCRIPTION:         This function is invoked when ICF_RGM_NW_DEACTIVATED
 *                      event from SSA  is received in 
 *						DEREGISTETRING/Authorizing/ Registering/Registered  state.
 *						regm indicates SSA to clear the REG object, 
 *						The rgm_context is not cleared because when the network
 *						comes up, the REGISTER will be sent without any
 *						trigger from the application.
 *
 * RETURNS:             
 *
 ***************************************************************************/
icf_return_t icf_rgm_01_03_04_05_nw_deactivated(
        INOUT icf_crm_pdb_st *p_crm_pdb)
{
#ifdef IMS_CLIENT
    icf_rgm_context_st *p_rgm_context = ICF_NULL;
    icf_return_t dummy_ret_val = ICF_SUCCESS;
    /* For storing the msg_data locally. */
    icf_uint8_t			*p_temp_msg_data = ICF_NULL;
    icf_uint32_t			temp_msg_id = ICF_NULL;
    icf_uint32_t			temp_pay_len = ICF_NULL;
#endif
    icf_return_t ret_val = ICF_SUCCESS;

	p_crm_pdb = p_crm_pdb; 

	/* function entry trace */
    if(ICF_NULL == p_crm_pdb)
    {
       return ICF_FAILURE;
    }

    ICF_FUNCTION_ENTER(p_crm_pdb->p_internal_msg->msg_hdr.p_glb_pdb)
		

#ifdef IMS_CLIENT	
ICF_CHECK_IMS_START(p_crm_pdb->p_glb_pdb,ICF_TRUE)		
    /* Get the pointer to registration context */
	p_rgm_context = p_crm_pdb->p_rgm_context; 
	ICF_ASSERT(
		p_crm_pdb->p_glb_pdb,
		(ICF_NULL != p_rgm_context))		
    
		
	/* Indicate SSA to clear the REG Object*/
	/* Copy the current msg_data locally */
	if (ICF_NULL != p_crm_pdb->p_internal_msg->p_msg_data)
	{
		p_temp_msg_data = p_crm_pdb->p_internal_msg->p_msg_data;
		temp_msg_id = p_crm_pdb->p_internal_msg->msg_hdr.msg_id;
		temp_pay_len = p_crm_pdb->p_internal_msg->msg_hdr.payload_length;
	}
	/* Fill CRM PDB Structure */
	p_crm_pdb->p_internal_msg->p_msg_data = 
		(icf_uint8_t *)(p_rgm_context);
	p_crm_pdb->p_internal_msg->msg_hdr.msg_id = 
		ICF_CLEAR_REG_OBJECT;
	p_crm_pdb->p_internal_msg->msg_hdr.payload_length = 
		(icf_uint8_t)(ICF_PORT_SIZEOF
		(icf_rgm_context_st));
	
	/* Call SSA to clear Registration Call Object */
	ret_val = icf_ssa_process_mesg(
		(icf_internal_msg_st *)
		(p_crm_pdb->p_internal_msg));
	
	/* Copy the old msg_data back */
	if (ICF_NULL != p_temp_msg_data)
	{
		p_crm_pdb->p_internal_msg->p_msg_data = p_temp_msg_data;
		p_crm_pdb->p_internal_msg->msg_hdr.msg_id = temp_msg_id;
		p_crm_pdb->p_internal_msg->
			msg_hdr.payload_length = temp_pay_len;
	}
	
	/* Clear the contents of auth_data_sent and auth_data_recd 
		  * members of rgm_context */
	if (ICF_NULL != p_rgm_context->auth_data_recd.p_nonce)
	{
		ICF_MEMFREE(p_crm_pdb->p_glb_pdb,
			p_rgm_context->auth_data_recd.p_nonce, 
			ICF_MEM_COMMON, p_crm_pdb->p_ecode, dummy_ret_val)
	}

	icf_port_memset(
		(icf_void_t*)&(p_rgm_context->auth_data_recd),
		(icf_uint8_t)ICF_NULL,
		(icf_uint32_t) ICF_PORT_SIZEOF(icf_rgm_auth_data_st));
	icf_port_memset(
		(icf_void_t*)&(p_rgm_context->auth_data_sent),
		(icf_uint8_t)ICF_NULL,
		(icf_uint32_t) ICF_PORT_SIZEOF(icf_rgm_auth_data_st));
	/* Making counts as NULL */
	p_rgm_context->retry_count = ICF_NULL;
	p_rgm_context->auth_retry_count = ICF_NULL;
	/* Stop the timer */
	if ( p_rgm_context->timer_id != ICF_TIMER_ID_INVALID)
	{
		ICF_STOP_TIMER(p_crm_pdb->p_glb_pdb, p_rgm_context->timer_id, p_crm_pdb->p_ecode, ret_val)
	}
    /* Call DBM to free timer structure */
    ret_val = icf_dbm_free_timer_block(
            p_crm_pdb->p_glb_pdb,
            (icf_void_t *)(p_rgm_context->p_timer_buff),
            p_crm_pdb->p_ecode);    

    /* Set Timer ID to INVALID */
    p_rgm_context->timer_id = ICF_TIMER_ID_INVALID;
    /* Set pointer to timer as NULL */
    p_rgm_context->p_timer_buff = ICF_NULL;

	/* Setting values in indicator and state */
	p_rgm_context->indicator |= ICF_RGM_NW_STATUS_DEACTIVATED;
	p_rgm_context->rgm_state = ICF_RGM_02_UNREGISTERED;
ICF_CHECK_IMS_END
#endif
	
	ICF_FUNCTION_EXIT(p_crm_pdb->p_glb_pdb)
	return ret_val;	
}/* Function icf_rgm_01_03_04_05_nw_deactivated() ends here */

/****************************************************************************
 * FUNCTION:            icf_rgm_02_nw_activated
 *
 * DESCRIPTION:         This function is invoked when ICF_RGM_NW_ACTIVATED
 *                      event from SSA  is received in 
 *						UnRegistered  state.
 *
 * RETURNS:             
 *
 ***************************************************************************/
icf_return_t icf_rgm_02_nw_activated(
        INOUT icf_crm_pdb_st *p_crm_pdb)
{
	icf_return_t		ret_val = ICF_SUCCESS;
	icf_rgm_register_resp_st	register_response;
	icf_duration_t   default_duration = ICF_RGM_DEFAULT_DURATION_NON_IMS;

	/* function entry trace */
    ICF_FUNCTION_ENTER(p_crm_pdb->p_glb_pdb) 
		
	p_crm_pdb->error_cause = ICF_NULL;

	/* generic sending related */
	register_response.app_id = ICF_NULL;
	register_response.line_id = ICF_NULL;
	register_response.p_crm_pdb = p_crm_pdb;
	register_response.p_rgm_context = p_crm_pdb->p_rgm_context;
	register_response.options_flag = ICF_NULL;
	register_response.p_request_payload = ICF_NULL;
	register_response.options_flag |= ICF_RGM_FILL_URIS;/*fill the p-asso member*/
	register_response.options_flag |= ICF_RGM_CTXT_COPY;/*use rgm_context*/

#ifdef IMS_CLIENT	
	   /*Extract the default duration of the registration based on the IMS and NON-IMS*/
ICF_CHECK_IMS_START(p_crm_pdb->p_glb_pdb,ICF_TRUE)
        default_duration = ICF_RGM_DEFAULT_DURATION_IMS_CLIENT;
ICF_CHECK_IMS_END
#endif

	if (!(ICF_IS_BIT_SET(p_crm_pdb->p_rgm_context->indicator,
                ICF_RGM_NW_STATUS_DEACTIVATED)))        
    {
        /* return SUCCESS because the NW_DEACTIVATED was not
         * set in the indicator flag. That is the state had
         * been made UNREGISTERED not because of NW_DEACTIVATED */
        ret_val = ICF_SUCCESS;

    }
    else
    {/* state was made UNREGISTERED when the NW was 
        * deactivated */

        /* Reset the NW_DEACTIVATED bit*/
        ICF_RESET_BIT(p_crm_pdb->p_rgm_context->indicator,
            ICF_RGM_NW_STATUS_DEACTIVATED)
        
        icf_rgm_update_auth_info(p_crm_pdb);
            if (p_crm_pdb->p_config_data->auto_register)
            {
                
                p_crm_pdb->p_rgm_context->rgm_duration = 
                    default_duration;
                /* invoke register_user */
                ret_val = icf_rgm_register_user( 
                    p_crm_pdb,p_crm_pdb->p_rgm_context);
                p_crm_pdb->resp_to_be_sent = 
                    ICF_FALSE;/*for CFG to not send response to app*/
                
          }/* auto register case: ends */
         else
          {
                if (ICF_FAILURE == icf_rgm_save_smallest_dur(
                    p_crm_pdb->p_glb_pdb,
                    p_crm_pdb->p_rgm_context,p_crm_pdb->p_ecode))
                {
                    ret_val = ICF_FAILURE;
                }
                else if((ICF_NULL == 
                    p_crm_pdb->p_rgm_context->rgm_duration) &&
                    (ICF_IS_BIT_SET(p_crm_pdb->p_rgm_context->indicator,
                    ICF_RGM_NFY_ON_PROBATION)))
                {
                    /* rgm_ctx is on probation: set it to UNREGISTERED state*/
                    p_crm_pdb->p_rgm_context->rgm_state =
                        ICF_RGM_02_UNREGISTERED;
                    /* Stop the timer */
                 icf_rgm_stop_timer(p_crm_pdb);
                    
                }
                else 
                {
                    if ((ICF_NULL == 
                        p_crm_pdb->p_rgm_context->rgm_duration) &&
                        (!p_crm_pdb->p_config_data->auto_register))
                    {
                        /* deregister scenario */		
                        ret_val = icf_rgm_deregister_user(
                            p_crm_pdb, 
                            p_crm_pdb->p_rgm_context);
                        p_crm_pdb->resp_to_be_sent = 
                            ICF_FALSE;/*for CFG to not send response to app*/
                    }				
                    else
                    {
                        /* register scenario */
                        if (p_crm_pdb->p_config_data->auto_register)
                        {
                            p_crm_pdb->p_rgm_context->rgm_duration =
                                    default_duration;                        
                        }
                        
                        /* invoke register_user */
                        ret_val = 
                            icf_rgm_register_user(
                            p_crm_pdb,p_crm_pdb->p_rgm_context);
                        p_crm_pdb->resp_to_be_sent = 
                            ICF_FALSE;/*for CFG to not send response to app*/
                    }
                }/* success: save_smallest_duration() ends */
                
           }/* non auto-register case : ends */			 
            
        if ((ICF_FAILURE == ret_val) &&
            (ICF_NULL != p_crm_pdb->error_cause))
        {
            
            /*  send CFM with FAILURE: */		
            register_response.request_type = ICF_NULL;;
            register_response.status = ICF_FAILURE;
            register_response.error_cause = p_crm_pdb->error_cause;
            
            p_crm_pdb->resp_to_be_sent = 
                    ICF_FALSE;/*for CFG to not send response*/
            ret_val = icf_rgm_send_response(p_crm_pdb->p_glb_pdb,
                &register_response, p_crm_pdb->p_ecode);			
        }
    }
	
	ICF_FUNCTION_EXIT(p_crm_pdb->p_glb_pdb)
	return ret_val;	
	
}/* Function icf_rgm_02_nw_activated() ends here */

/****************************************************************************
 * FUNCTION:            icf_rgm_03_notify_shortened
 *
 * DESCRIPTION:         This function is invoked when ICF_RGM_NOTIFY_SHORTENED
 *                      event is received in Registered  state.
 *
 * RETURNS:             
 *
 ***************************************************************************/
icf_return_t icf_rgm_03_notify_shortened(
        INOUT icf_crm_pdb_st *p_crm_pdb)
{
	icf_return_t				ret_val = ICF_SUCCESS;
	icf_rgm_context_st		*p_rgm_context = ICF_NULL;
	icf_rgm_register_resp_st	register_response;
	
	/* function entry trace */
    ICF_FUNCTION_ENTER(p_crm_pdb->p_glb_pdb) 

	/* local pointer to rgm_context */
	p_rgm_context = p_crm_pdb->p_rgm_context;
	ICF_ASSERT(p_crm_pdb->p_glb_pdb, 
					(ICF_NULL != p_rgm_context))

	p_crm_pdb->error_cause = ICF_NULL;	

	/* Basic values for Sending */
	register_response.p_crm_pdb = p_crm_pdb;
	register_response.app_id = ICF_NULL;
	register_response.line_id = ICF_NULL;
	register_response.options_flag = ICF_NULL;
	register_response.options_flag |= ICF_RGM_FILL_URIS;/*fill the p-asso member*/
	register_response.options_flag |= ICF_RGM_CTXT_COPY;/*use rgm_ctxt for To/From*/
	register_response.p_rgm_context = p_rgm_context;
	register_response.p_request_payload = ICF_NULL;
	

	/* Stop the Running Registration Timer*/
	
	if ( p_rgm_context->timer_id != ICF_TIMER_ID_INVALID)
	{
		ICF_STOP_TIMER(p_crm_pdb->p_glb_pdb, 
						p_rgm_context->timer_id, p_crm_pdb->p_ecode, ret_val)
	}
    
    /* Call DBM to free timer structure */
    ret_val = icf_dbm_free_timer_block(
            p_crm_pdb->p_glb_pdb,
            (icf_void_t *)(p_rgm_context->p_timer_buff),
            p_crm_pdb->p_ecode);    

    /* Set Timer ID to INVALID */
    p_rgm_context->timer_id = ICF_TIMER_ID_INVALID;
    /* Set pointer to timer as NULL */
    p_rgm_context->p_timer_buff = ICF_NULL;

	/* Start REG timer with shortened_expiry received in NOTIFY */
	
	if (ICF_FAILURE == icf_rgm_start_expires_timer(
                p_crm_pdb->p_glb_pdb,
                p_crm_pdb->shortened_expiry,
                p_rgm_context,
                p_crm_pdb->p_ecode))
	{
		/* Send REGISTER again */
		icf_rgm_update_auth_info(p_crm_pdb);
		if (ICF_FAILURE == icf_rgm_save_smallest_dur(
	         	p_crm_pdb->p_glb_pdb,p_crm_pdb->p_rgm_context,
                p_crm_pdb->p_ecode))
		{
			ret_val = ICF_FAILURE;
		}
        else if((ICF_NULL == 
                p_crm_pdb->p_rgm_context->rgm_duration) &&
                (ICF_IS_BIT_SET(p_crm_pdb->p_rgm_context->indicator,
                ICF_RGM_NFY_ON_PROBATION)))
         {
                /* rgm_ctx is on probation: set it to UNREGISTERED state*/
                p_crm_pdb->p_rgm_context->rgm_state =
                    ICF_RGM_02_UNREGISTERED;
                 
                /* Stop the timer */
                 icf_rgm_stop_timer(p_crm_pdb);
                
            }
            else if (ICF_NULL != p_crm_pdb->p_rgm_context->rgm_duration)
			{
				/* invoke register_user */
				ret_val = icf_rgm_register_user( p_crm_pdb,p_crm_pdb->p_rgm_context);
				p_crm_pdb->resp_to_be_sent = ICF_FALSE;/*for CFG to not send response to app*/
			}
			
		if ((ICF_FAILURE == ret_val) &&
			(ICF_NULL != p_crm_pdb->error_cause))
		{
			/* Register could not be sent */
			register_response.request_type = ICF_NULL;;
			register_response.status = ICF_FAILURE;
			register_response.error_cause = p_crm_pdb->error_cause;
						
			p_crm_pdb->resp_to_be_sent = ICF_FALSE;/*for CFG to not send response*/
			ret_val = icf_rgm_send_response(p_crm_pdb->p_glb_pdb,
				&register_response, p_crm_pdb->p_ecode);								
		
			ICF_RGM_SET_ECODE(p_crm_pdb, ICF_UNABLE_TO_SEND_REG_REQ)

			/* Clear registration context */
			ret_val = icf_rgm_clear_reg_context(
								p_crm_pdb,
								p_rgm_context); 
			ret_val = ICF_SUCCESS;		
		}/* Sending REGISTER failed:*/

	}/* Starting Timer failed: */
	else
	{
		/* nothing to be done:REG timer successfully started */

	}
		
	
	ICF_FUNCTION_EXIT(p_crm_pdb->p_glb_pdb)
	return ret_val;	

}/* Function icf_rgm_03_notify_shortened() ends here */

/****************************************************************************
 * FUNCTION:            icf_rgm_03_notify_terminated
 *
 * DESCRIPTION:         This function is invoked when ICF_RGM_NOTIFY_TERMINATED
 *                      event is received in Registered  state.
 *
 * RETURNS:             
 *
 ***************************************************************************/
icf_return_t icf_rgm_03_notify_terminated(
        INOUT icf_crm_pdb_st *p_crm_pdb)
{

	icf_return_t		ret_val = ICF_SUCCESS;
#ifdef IMS_CLIENT
	icf_rgm_context_st		*p_rgm_context = ICF_NULL;
	icf_rgm_register_resp_st	register_response ={0};
#endif

	p_crm_pdb = p_crm_pdb; 

	/* function entry trace */
    if(ICF_NULL == p_crm_pdb)
    {
       return ICF_FAILURE;
    }
    ICF_FUNCTION_ENTER(p_crm_pdb->p_glb_pdb) 
#ifdef IMS_CLIENT
ICF_CHECK_IMS_START(p_crm_pdb->p_glb_pdb,ICF_TRUE)
	/* local pointer to rgm_context */
	p_rgm_context = p_crm_pdb->p_rgm_context;
	ICF_ASSERT(p_crm_pdb->p_glb_pdb, 
					(ICF_NULL != p_rgm_context))

	if (ICF_APP_ID_INVALID == p_crm_pdb->recd_con_app_id)
    {
        /* state=terminated was received in the <aor> element of 
         * the NOTIFY message */
	/* Basic values for Sending */
	register_response.p_crm_pdb = p_crm_pdb;
	register_response.app_id = ICF_NULL;
	register_response.line_id = ICF_NULL;
	register_response.options_flag = ICF_NULL;
	register_response.options_flag |= ICF_RGM_CTXT_COPY;/*use rgm_ctxt for To/From*/
	register_response.options_flag |= ICF_RGM_FILL_URIS;
	register_response.p_rgm_context = p_rgm_context;
	register_response.p_request_payload = ICF_NULL;

	/* Stop the Running Registration Timer*/
	if ( p_rgm_context->timer_id != ICF_TIMER_ID_INVALID)
	{
		ICF_STOP_TIMER(p_crm_pdb->p_glb_pdb, 
						p_rgm_context->timer_id, p_crm_pdb->p_ecode, ret_val)
	}
    /* Call DBM to free timer structure */
    ret_val = icf_dbm_free_timer_block(
            p_crm_pdb->p_glb_pdb,
            (icf_void_t *)(p_rgm_context->p_timer_buff),
            p_crm_pdb->p_ecode);    
    /* Set Timer ID to INVALID */
    p_rgm_context->timer_id = ICF_TIMER_ID_INVALID;
    /* Set pointer to timer as NULL */
    p_rgm_context->p_timer_buff = ICF_NULL;

	
	/* Network initiated DEREG happened */
	register_response.request_type = ICF_NULL;;
	register_response.status = ICF_FAILURE;
	register_response.error_cause = ICF_CAUSE_NETWORK_DEREGISTERED;
	
	p_crm_pdb->resp_to_be_sent = ICF_FALSE;/*for CFG to not send response*/
	ret_val = icf_rgm_send_response(p_crm_pdb->p_glb_pdb,
		&register_response, p_crm_pdb->p_ecode);								
	
		
	/* Clear registration context */
	ret_val = icf_rgm_clear_reg_context(
								p_crm_pdb,
								p_rgm_context); 
	ret_val = ICF_SUCCESS;	
    }
    else
    {
        /* state=terminated was received in the <contact>
         * element of the NOTIFY message */
       switch (p_crm_pdb->recd_con_event)
       {
       case ICF_RGM_NFY_CON_EVT_EXPIRED:
           ret_val = icf_rgm_nfy_con_expired(p_crm_pdb);
           break;
       case ICF_RGM_NFY_CON_EVT_DEACTIVATED:
           ret_val = icf_rgm_nfy_con_deactivated(p_crm_pdb);
           break;
       case ICF_RGM_NFY_CON_EVT_PROBATION:
           ret_val = icf_rgm_nfy_con_probation(p_crm_pdb);
           break;
       case ICF_RGM_NFY_CON_EVT_UNREGISTERED:
           ret_val = icf_rgm_nfy_con_unregistered(p_crm_pdb);
           break;
       case ICF_RGM_NFY_CON_EVT_REJECTED:
           ret_val = icf_rgm_nfy_con_rejected(p_crm_pdb);
           break;	
       default:
           ret_val= ICF_FAILURE;
           break;


       }/* switch on the contact_event ends here */            
    }/* state=terminated in <contact> element ends here */
ICF_CHECK_IMS_END
#endif	
	ICF_FUNCTION_EXIT(p_crm_pdb->p_glb_pdb)

	return ret_val;	

}/* Function icf_rgm_03_notify_terminated() ends here */

#ifdef ICF_NAT_RPORT_SUPPORT

/****************************************************************************
 * FUNCTION:            icf_rgm_01_03_nat_binding_chng
 *
 * DESCRIPTION:         This function is invoked when NAT binding has changed in
 *                      Registering/Registered State
 *
 *
 * RETURNS:             
 *
 ***************************************************************************/
icf_return_t icf_rgm_01_03_nat_binding_chng(
        INOUT icf_crm_pdb_st *p_crm_pdb)
{
    icf_rgm_context_st          *p_rgm_context = ICF_NULL;
    icf_rgm_context_st          *p_temp_rgm_context = ICF_NULL;
    icf_return_t                ret_val = ICF_SUCCESS;

    /* Variables used for Code Area reduction */
    icf_glb_pdb_st              *p_glb_pdb = ICF_NULL;
    icf_error_t                 *p_ecode = ICF_NULL;
    icf_rport_config_data_st    *p_rport_config_data=ICF_NULL;
    icf_line_data_st            *p_line_data = ICF_NULL;
    icf_user_db_node_st         *p_user_db = ICF_NULL; 
    icf_uint8_t                  reg_sent_counter = 0;
 
    /* function entry trace */
    ICF_FUNCTION_ENTER(p_crm_pdb->p_internal_msg->msg_hdr.p_glb_pdb) 
        
    /* Get the pointer to registration context */
    p_rgm_context = p_crm_pdb->p_rgm_context;
   /* (icf_rgm_context_st *)(p_crm_pdb->p_internal_msg->
            p_msg_data);*/
    ICF_ASSERT(
            p_crm_pdb->p_internal_msg->msg_hdr.p_glb_pdb,
            (ICF_NULL != p_rgm_context))
    
    
    /* Get the pointer to global PDB */
    p_glb_pdb = p_crm_pdb->p_glb_pdb;
    /*p_internal_msg->msg_hdr.p_glb_pdb;*/
    ICF_ASSERT(p_glb_pdb,(ICF_NULL != p_glb_pdb))

    /* Get the pointer to ecode */
    p_ecode = p_crm_pdb->p_ecode;
    ICF_ASSERT(p_glb_pdb,(ICF_NULL != p_ecode))
        
   if(ICF_FAILURE == icf_dbm_fetch_line_cntxt_blk(
                    p_crm_pdb->p_glb_pdb, p_rgm_context->line_id,
                    &p_line_data, p_crm_pdb->p_ecode))
     {
          return ICF_FAILURE;
     }

    if (ICF_CFG_SELF_ADDR_PER_LINE ==
            p_crm_pdb->p_config_data->self_addr_per_line)
    {
            p_rport_config_data = p_line_data->p_rport_config_data;
    }
    else
    {
        p_rport_config_data = &(p_crm_pdb->p_config_data->rport_config_data);
    }
   
    if (ICF_NULL == p_rport_config_data)
    {
        return ICF_FAILURE;
    } 
 
    if(ICF_RGM_03_REGISTERED == p_rgm_context->rgm_state)
    {
        /* Stop Registration Timer */
        if ( p_rgm_context->timer_id != ICF_TIMER_ID_INVALID)
        {
            ICF_STOP_TIMER(p_glb_pdb, p_rgm_context->timer_id, p_ecode, ret_val)
        }

        /* Call DBM to free timer structure */
        ret_val = icf_dbm_free_timer_block(
                p_glb_pdb,
                (icf_void_t *)(p_rgm_context->p_timer_buff),
                p_ecode);    

        /* Set Timer ID to INVALID */
        p_rgm_context->timer_id = ICF_TIMER_ID_INVALID;
        /* Set pointer to timer as NULL */
        p_rgm_context->p_timer_buff = ICF_NULL;
    }


    if((ICF_INITIAL_RPORT_INITIAL == p_rgm_context->rport_status)||\
            (ICF_INITIAL_RPORT_COMPLETED == p_rgm_context->rport_status))
    {
                  p_user_db = p_line_data->p_user_db;
		  while (ICF_NULL != p_user_db)
		  {

				if (ICF_SUCCESS == icf_rgm_get_ctxt_for_pub_uri(p_crm_pdb->p_glb_pdb,
							p_rgm_context->line_id,
							&(p_user_db->public_user),
							&p_temp_rgm_context, 
								p_crm_pdb->p_ecode))
				{
					p_crm_pdb->p_rgm_context = p_rgm_context;
					p_crm_pdb->error_cause = ICF_NULL;

                    if(ICF_RGM_00_IDLE == p_temp_rgm_context->rgm_state ||
                        ICF_RGM_02_UNREGISTERED == p_temp_rgm_context->rgm_state)
                    {
                        if(ICF_RGM_00_IDLE == p_temp_rgm_context->rgm_state)
                        {
                             icf_dbm_dealloc_rgn_blk(
				p_crm_pdb->p_glb_pdb,
				p_temp_rgm_context,
				p_crm_pdb->p_ecode);
                        }
                        p_user_db = p_user_db->p_next;
                        continue;
                    }
                    else if(ICF_RGM_03_REGISTERED == p_temp_rgm_context->rgm_state)
                    {
                        /* Set the rport status to INITIAL Pending */
                        p_temp_rgm_context->rport_status = ICF_INITIAL_RPORT_PENDING;
                       p_rport_config_data->is_rport_completed = ICF_FALSE; 
					    /* Deregistration is to be tried */
                        if(ICF_FAILURE == icf_rgm_deregister_user(
                                p_crm_pdb, 
                                p_temp_rgm_context))
                        {
                            ret_val = ICF_FAILURE;
                            break;
                        }
                        p_temp_rgm_context->ong_dereg_counter++;
                    }
                    else if(ICF_RGM_01_REGISTERING == p_temp_rgm_context->rgm_state)
                    {
                        if(ICF_INITIAL_RPORT_INITIAL == p_temp_rgm_context->rport_status ||
                           ICF_INITIAL_RPORT_COMPLETED == p_temp_rgm_context->rport_status)
                        {
                            if(ICF_FAILURE == icf_rgm_deregister_user(
                                p_crm_pdb,
                                p_temp_rgm_context))
                            {
                                ret_val = ICF_FAILURE;
                                 break;
                            }
                            else
                            {
                                 p_temp_rgm_context->ong_dereg_counter++;
                                 reg_sent_counter++;
                            }
                        }
                          /* Set the rport status to INITIAL Pending */
                          p_temp_rgm_context->rport_status = ICF_INITIAL_RPORT_PENDING;
                          p_rport_config_data->is_rport_completed = ICF_FALSE;
                    }
                    else if(ICF_RGM_04_AUTHORIZING == p_temp_rgm_context->rgm_state) 
                    {
                        if(ICF_INITIAL_RPORT_PENDING == p_temp_rgm_context->rport_status)
                        {
                            if(ICF_FAILURE == icf_rgm_deregister_user(
                                p_crm_pdb,
                                p_temp_rgm_context))
                            {
                                ret_val = ICF_FAILURE;
                                 break;
                            }
                            else
                            {
                                 p_temp_rgm_context->ong_dereg_counter++;
                                 reg_sent_counter++;
                            }
                        }

                        p_temp_rgm_context->rport_status = ICF_INITIAL_RPORT_PENDING;
                        p_rport_config_data->is_rport_completed = ICF_FALSE;
                    }
                    else if(ICF_RGM_05_DEREGISTERING == p_temp_rgm_context->rgm_state)
                    {
                        if(ICF_INITIAL_RPORT_PENDING == p_temp_rgm_context->rport_status)
                        {
                            p_temp_rgm_context->rport_status = ICF_INITIAL_RPORT_ONGOING;
                            p_temp_rgm_context->rgm_duration = p_rport_config_data->init_reg_duration;
                            if(!reg_sent_counter)
                            {
                              if(ICF_FAILURE == icf_rgm_register_user(
                                p_crm_pdb,
                                p_temp_rgm_context))
                              {
                                ret_val = ICF_FAILURE;
                                 break;
                              }
                            }
                        }
                        else
                        {
                          /* Set the rport status to INITIAL Pending */
                          p_temp_rgm_context->rport_status = ICF_INITIAL_RPORT_PENDING;
                          p_rport_config_data->is_rport_completed = ICF_FALSE;
                        }
                    }
                    else
                    {
                        /* Set the rport status to INITIAL Pending */
                       p_temp_rgm_context->rport_status = ICF_INITIAL_RPORT_PENDING;
                       p_rport_config_data->is_rport_completed = ICF_FALSE; 

                        /* Call the FSM handler */
						ret_val = ((icf_rgm_glb_data_st *)(p_crm_pdb->p_rgm_glb_data))->icf_rgm_state_machine
								[p_temp_rgm_context->rgm_state][ICF_RGM_NAT_BINDING_CHNG_IND](p_crm_pdb);
                    }	
				}
				p_user_db = p_user_db->p_next;
			}
 
    }
    
    /* function exit trace */
    ICF_FUNCTION_EXIT(p_glb_pdb)
        
    return ret_val;
    
}

/****************************************************************************
 * FUNCTION:            icf_rgm_05_nat_binding_chng
 *
 * DESCRIPTION:         This function is invoked when NAT binding has changed in
 *                      Deregistering State. This is invoked when response to 
 *                      De-register request sent on NAT binding change has been 
 *                      received. Also update the contact address in the global
 *                      config data.
 *
 *
 * RETURNS:             ICF_SUCCESS/ICF_FAILURE
 *
 ***************************************************************************/
icf_return_t icf_rgm_05_nat_binding_chng(
        INOUT icf_crm_pdb_st *p_crm_pdb)
{
    icf_return_t                ret_val = ICF_SUCCESS;
    icf_rport_config_data_st    *p_rport_config_data=ICF_NULL;
    icf_line_data_st            *p_line_data = ICF_NULL;
    icf_rgm_context_st          *p_temp_rgm_context = ICF_NULL;
    icf_user_db_node_st         *p_user_db = ICF_NULL;
    icf_uint8_t                  reg_sent_counter = 0;

    /* function entry trace */
    ICF_FUNCTION_ENTER(p_crm_pdb->p_internal_msg->msg_hdr.p_glb_pdb) 
        
    if(ICF_FAILURE == icf_dbm_fetch_line_cntxt_blk(
                    p_crm_pdb->p_glb_pdb, p_crm_pdb->p_rgm_context->line_id,
                    &p_line_data, p_crm_pdb->p_ecode))
     {
           ret_val = ICF_FAILURE;
     }
    
    if (ICF_CFG_SELF_ADDR_PER_LINE ==
            p_crm_pdb->p_config_data->self_addr_per_line)
    {
            p_rport_config_data = p_line_data->p_rport_config_data;
    }
    else
    {
        p_rport_config_data = &(p_crm_pdb->p_config_data->rport_config_data);
    }

    if (ICF_NULL == p_rport_config_data)
    {
       return ICF_FAILURE;
    }  
    /* Update the contact address and set the rport status to ongoing.
    */
  
    if(p_crm_pdb->p_rgm_context->ong_dereg_counter)
    {
        icf_port_memcpy((icf_void_t*)&(p_rport_config_data->temp_contact_ip_addr),\
            (icf_void_t*)&(p_rport_config_data->contact_ip_addr),\
            (icf_uint32_t)ICF_PORT_SIZEOF(icf_transport_address_st));
    }
 
    icf_port_memcpy((icf_void_t*)&(p_rport_config_data->contact_ip_addr),\
            (icf_void_t*)&(p_rport_config_data->received_addr),\
            (icf_uint32_t)ICF_PORT_SIZEOF(icf_transport_address_st));


         p_user_db = p_line_data->p_user_db;
	 while (ICF_NULL != p_user_db)
	 {

				if (ICF_SUCCESS == icf_rgm_get_ctxt_for_pub_uri(p_crm_pdb->p_glb_pdb,
							p_crm_pdb->p_rgm_context->line_id,
							&(p_user_db->public_user),
							&p_temp_rgm_context, 
								p_crm_pdb->p_ecode))
				{
					p_crm_pdb->error_cause = ICF_NULL;

                    if(ICF_RGM_00_IDLE == p_temp_rgm_context->rgm_state ||
                        ICF_RGM_02_UNREGISTERED == p_temp_rgm_context->rgm_state)
                    {
                        if(ICF_RGM_00_IDLE == p_temp_rgm_context->rgm_state)
                        {
                             icf_dbm_dealloc_rgn_blk(
				p_crm_pdb->p_glb_pdb,
				p_temp_rgm_context,
				p_crm_pdb->p_ecode);
                        }

                        p_user_db = p_user_db->p_next;
                        continue;
                    }
                    else if(ICF_RGM_03_REGISTERED == p_temp_rgm_context->rgm_state) 
                    {
                        /* Set the rport status to INITIAL Pending */
                        p_temp_rgm_context->rport_status = ICF_INITIAL_RPORT_PENDING;
                       p_rport_config_data->is_rport_completed = ICF_FALSE; 
					    /* Deregistration is to be tried */
                        if(ICF_FAILURE == icf_rgm_deregister_user(
                                p_crm_pdb, 
                                p_temp_rgm_context))
                        {
                            ret_val = ICF_FAILURE;
                            break;
                        }
                        p_temp_rgm_context->ong_dereg_counter++;
                    }
                    else if(ICF_RGM_01_REGISTERING == p_temp_rgm_context->rgm_state)
                    {
                        if(ICF_INITIAL_RPORT_INITIAL == p_temp_rgm_context->rport_status ||
                           ICF_INITIAL_RPORT_COMPLETED == p_temp_rgm_context->rport_status)
                        {
                            if(ICF_FAILURE == icf_rgm_deregister_user(
                                p_crm_pdb,
                                p_temp_rgm_context))
                            {
                                ret_val = ICF_FAILURE;
                                 break;
                            }
                            p_temp_rgm_context->ong_dereg_counter++;
                        }
                        else if(ICF_INITIAL_RPORT_ONGOING != p_temp_rgm_context->rport_status)
                        {
                          /* Set the rport status to INITIAL Pending */
                          p_temp_rgm_context->rport_status = ICF_INITIAL_RPORT_PENDING;
                        }
                          p_rport_config_data->is_rport_completed = ICF_FALSE;
                    }
                    else if(ICF_RGM_04_AUTHORIZING == p_temp_rgm_context->rgm_state)
                    {
                        if(ICF_INITIAL_RPORT_PENDING == p_temp_rgm_context->rport_status)
                        {
                            if(ICF_FAILURE == icf_rgm_deregister_user(
                                p_crm_pdb,
                                p_temp_rgm_context))
                            {
                                ret_val = ICF_FAILURE;
                                 break;
                            }
                            else
                            {
                                 p_temp_rgm_context->ong_dereg_counter++;
                                 reg_sent_counter++;
                            }
                        }
                        p_temp_rgm_context->rport_status = ICF_INITIAL_RPORT_PENDING;
                        p_rport_config_data->is_rport_completed = ICF_FALSE;
                    }
                    else if(ICF_RGM_05_DEREGISTERING == p_temp_rgm_context->rgm_state)
                    {
                        if(ICF_INITIAL_RPORT_PENDING == p_temp_rgm_context->rport_status)
                        {
                            
                            if(!reg_sent_counter)
                            {
                               p_temp_rgm_context->rport_status = ICF_INITIAL_RPORT_ONGOING;
                              p_temp_rgm_context->rgm_duration = p_rport_config_data->init_reg_duration;

                              if(ICF_FAILURE == icf_rgm_register_user(
                                p_crm_pdb,
                                p_temp_rgm_context))
                              {
                                ret_val = ICF_FAILURE;
                                 break;
                              }
                              else
                              {
                                 p_temp_rgm_context->ong_dereg_counter--;
                                 reg_sent_counter++;
                               }
                            }
                        }
                        else if(!p_temp_rgm_context->ong_dereg_counter)
                        {
                            ret_val = icf_rgm_05_any_nw_resp(p_crm_pdb);
                        }
                        else 
                        {
                          /* Set the rport status to INITIAL Pending */
                          p_temp_rgm_context->rport_status = ICF_INITIAL_RPORT_PENDING;
                          p_rport_config_data->is_rport_completed = ICF_FALSE;
                        }
                    }
                    else
                    {
                        /* Set the rport status to INITIAL Pending */
                       p_temp_rgm_context->rport_status = ICF_INITIAL_RPORT_PENDING;
                       p_rport_config_data->is_rport_completed = ICF_FALSE; 

                        /* Call the FSM handler */
						ret_val = ((icf_rgm_glb_data_st *)(p_crm_pdb->p_rgm_glb_data))->icf_rgm_state_machine
								[p_temp_rgm_context->rgm_state][ICF_RGM_NAT_BINDING_CHNG_IND](p_crm_pdb);
                    }	
				}
				p_user_db = p_user_db->p_next;
			}

	 
    /* function exit trace */
    ICF_FUNCTION_EXIT(p_crm_pdb->p_internal_msg->msg_hdr.p_glb_pdb)
    return ret_val;

}


icf_return_t icf_rgm_04_nat_binding_chng(
        INOUT icf_crm_pdb_st *p_crm_pdb)
{
    icf_rgm_context_st          *p_rgm_context = ICF_NULL;
    icf_return_t                ret_val = ICF_SUCCESS;

    /* Variables used for Code Area reduction */
    icf_glb_pdb_st              *p_glb_pdb = ICF_NULL;
    icf_error_t                 *p_ecode = ICF_NULL;
    icf_rport_config_data_st    *p_rport_config_data=ICF_NULL;
    icf_line_data_st            *p_line_data = ICF_NULL;
    icf_uint8_t                  reg_sent_counter = 0;
 
    /* function entry trace */
    ICF_FUNCTION_ENTER(p_crm_pdb->p_internal_msg->msg_hdr.p_glb_pdb) 
        
    /* Get the pointer to registration context */
    p_rgm_context = p_crm_pdb->p_rgm_context;
   /* (icf_rgm_context_st *)(p_crm_pdb->p_internal_msg->
            p_msg_data);*/
    ICF_ASSERT(
            p_crm_pdb->p_internal_msg->msg_hdr.p_glb_pdb,
            (ICF_NULL != p_rgm_context))
    
    
    /* Get the pointer to global PDB */
    p_glb_pdb = p_crm_pdb->p_glb_pdb;
    /*p_internal_msg->msg_hdr.p_glb_pdb;*/
    ICF_ASSERT(p_glb_pdb,(ICF_NULL != p_glb_pdb))

    /* Get the pointer to ecode */
    p_ecode = p_crm_pdb->p_ecode;
    ICF_ASSERT(p_glb_pdb,(ICF_NULL != p_ecode))
        
    if(ICF_FAILURE == icf_dbm_fetch_line_cntxt_blk(
                    p_crm_pdb->p_glb_pdb, p_rgm_context->line_id,
                    &p_line_data, p_crm_pdb->p_ecode))
     {
          return ICF_FAILURE;
     }

    if (ICF_CFG_SELF_ADDR_PER_LINE ==
            p_crm_pdb->p_config_data->self_addr_per_line)
    {
            p_rport_config_data = p_line_data->p_rport_config_data;
    }
    else
    {
        p_rport_config_data = &(p_crm_pdb->p_config_data->rport_config_data);
    }
   
    if (ICF_NULL == p_rport_config_data)
    {
        return ICF_FAILURE;
    } 
 
    if(ICF_INITIAL_RPORT_PENDING == p_rgm_context->rport_status ||
       ICF_INITIAL_RPORT_INITIAL == p_rgm_context->rport_status)
    {
          if(ICF_FAILURE == icf_rgm_deregister_user(
                         p_crm_pdb,
                         p_rgm_context))
          {
               ret_val = ICF_FAILURE;
          }
          else
          {
               p_rgm_context->ong_dereg_counter++;
               reg_sent_counter++;
          }
          
          p_rgm_context->rport_status = ICF_INITIAL_RPORT_PENDING;
          p_rport_config_data->is_rport_completed = ICF_FALSE; 
    }
    else
    {
         ret_val = icf_rgm_01_04_success_resp(p_crm_pdb); 
    }

    /* function exit trace */
    ICF_FUNCTION_EXIT(p_glb_pdb)
        
    return ret_val;
    
}

#endif


ICF_4_0_CRM_END
