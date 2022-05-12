/*------------------------------------------------------------------------------
 *
 * File name        : icf_regm_basic.c
 *
 * Purpose          : This file contains the registration handler.
 *
 * Revision History :
 *
 * Date           Name          Ref#                 Comments
 * --------    ------------     ----                 ---------------
 * 20-Dec-2004  Jalaj Negi     Non Service LLD      Initial
 * 21-Apr-2005  Jalaj Negi     SPR 7409             Initialize State of New
 *                                                  registration block
 * 02-Mar-2005  Jalaj Negi     SPR 7477             Set a bit to indicate a
 *                                                  block marked for DELETION.
 * 13-May-2005  Jalaj Negi     SPR 7552             Added check for presence
 *                                                  of Registrar also.
 * 17-May-2005  Jalaj Negi     SPR 7564             Changed forceful deregister
 *                                                  function's handling.
 * 17-Aug-2005    Jyoti Narula   ICF LLD            Coding for ICF :
 *                                                  register_request_hdlr()
 *                                                  Changes in save_
                                                    smallest_duration()
 * 02-Dec-2005    Jyoti Narula    ICF 4.0 CDD       Changes under ICF_4_0_
                                                    CRM_START and END
 *
 * 28-Dec-2005    Jyoti Narula    SPR 9069           Fix for this SPR
 *                                SPR: 9068: fix     Gave default value 
 *                                                   to reginfo version  
 *
 * 02-Feb-2006    Jyoti Narula    SPR 9047           Fix for this SPR
 *                                                   handling case of
 *                                                   new version greater
 *                                                   than old by 1.
 * 02-Apr-2006	Puneet Sharma	ICF5.0 LLD	 changes done in the comments 
 *											 Multiple Public Users ICF5.0
 * 04-Apr-2007  Rajat Chandna                    changes for wildcarded user
 *                                               enhancement.                       
 * 7-Sep-2007   Deepti Goyal    SPR 16019       Enhancement and Correction in
 *
 * 26-Jan-2008  Amit Mahajan    ICF Rel 8.0  Changes for reg event package support
 * 10-Mar-2008  Alok Tiwari     ICF Rel8.0   Modified Function
 *                                           icf_rgm_process_notify
 * 12-Mar-2008  Vipul Varshney  SPR 18122       AKA Enhancement
 * 08-Apr-2008  Tarun Gupta     SPR 18355       Merged Windows Porting Changes
 * 17-May-2008  Amit Sharma     SPR:18577     Asinging line_id in p_glb_pdb
 * 26-Jun-2008  Amit Sharma     SPR:18694     Asinging rgm_context from SIC subs_key
 * 08-Jul-2008  Anuradha Gupta  SPR 18782      Klocwork warning removal
 * 04-Nov-2008  Tarun Gupta     SPR 19189    Merged Rel 7.1.2 SPR 18880
 * 01-Dec-2008  Rajiv Kumar     SPR 19215    CSR-1-6212448 Merged
 * 08-Dec-2008  Abhishek Dhammawat SPR 19223 Klocwork warning removal
 * 04-Mar-2009 Saurabh Gupta       SPR 19349      Klocworks fix
 * 18-Mar-2009 Alok Tiwari      IPTK Rel8.1  Registration Enhancement(
 *                                           Multiple Contact Feature)
 * 27-Mar-2009  Anuradha Gupta   Rel 8.2      Reg Eveny Pckg Subs Handling
 * 27-Mar-2009 Ashutosh Mohan     Rel 8.2     Modified icf_rgm_register_req_hdlr
 * 31-Mar-2009 Alok Tiwari       Rel 8.2      Modified icf_rgm_aka_auth_resp_hdlr.
 * 12-may-2009  Anuradha Gupta   SPR 19601    Modifed icf_rgm_forceful_dereg_all_users
 * 11-Jun-2009 Ashutosh Mohan    SPR 19737    Changes done for GCC 4.1 warnings removal
 * 22-Jul-2009  Anuradha Gupta   SPR 18940    Modified icf_rgm_register_req_hdlr
 * 03-Aug-2009  Tarun Gupta      SPR 19886    Merged CSR 1-7193955
 * 17-Aug-2009  Anuradha Gupta   SPR 19927    klocworks warning removal
 * 16-Oct-2009 Abhishek Dhammawat SPR 20026  Modified icf_rgm_process_notify
 * 22-Oct-2009 Abhishek Dhammawat SPR 20026  Modified icf_rgm_process_notify
 * 25-May-2010  Preksha           SPR 20412   Merged CSR 1-8389294
 * 14-Jun-2010 Aman Aggarwal      SPR 20435   Fix for CSR 1-8597937
 *
 * Copyright (c) 2010, Aricent.
 *-------------------------------------------------------------------------*/


#include "icf_internal_api_id.h"
#include "icf_dbm_prototypes.h"
#include "icf_dbm_regm_prototypes.h"
#include "icf_regm_prototypes.h"
#include "icf_regm_macro.h"
#include "icf_common_prototypes.h"

#ifndef ICF_FEATURE_ABANDON/* For ICF features not supported in ICF */
/****************************************************************************
 * FUNCTION:            icf_rgm_init_complete
 *
 * DESCRIPTION:         This function is called by the CFG module when
 *                      INIT_COMPLETE is received. The event is thus
 *                        INIT_COMPLETE.
 *
 *
 * RETURNS:             
 *
 ***************************************************************************/

icf_return_t icf_rgm_init_complete(
        INOUT icf_crm_pdb_st *p_crm_pdb)
{

	icf_return_t                     ret_val = ICF_FAILURE;
    icf_glb_pdb_st                   *p_glb_pdb;
    icf_error_t                      *p_ecode = ICF_ERROR_NONE;
    icf_rgm_context_st               *p_rgm_context = ICF_NULL;
    icf_uint16_t                      block_id = ICF_NULL;
    icf_uint16_t                     num_of_blks_created = ICF_NULL;
/*    icf_boolean_t                    any_user_registered = ICF_FALSE;*/
    


    ICF_SET_MODULE(p_crm_pdb->p_internal_msg->msg_hdr.p_glb_pdb,
            ICF_INT_MODULE_REGM)
    
    /* function entry trace */
    ICF_FUNCTION_ENTER(p_crm_pdb->p_internal_msg->msg_hdr.p_glb_pdb) 

    ICF_RGM_TRACE(p_crm_pdb, ICF_TRACE_INIT_COMPLETE_RECVD) 

    /* Get the global pdb pointer */
    p_glb_pdb = p_crm_pdb->p_internal_msg->msg_hdr.p_glb_pdb;
    ICF_ASSERT(
            p_crm_pdb->p_internal_msg->msg_hdr.p_glb_pdb,
            (ICF_NULL != p_glb_pdb))
    
    /* Get pointer to ecode */    
    p_ecode = &(p_crm_pdb->p_internal_msg->msg_hdr.ecode);

    ret_val = icf_dbm_create_all_rgn_blks(
            p_glb_pdb,
            &num_of_blks_created,
            p_ecode);

    if (ICF_NULL != num_of_blks_created)
    {
        for (block_id = 1; block_id <= num_of_blks_created; block_id++)
        {

            ret_val = icf_dbm_get_used_rgn_blk(
                    p_glb_pdb,
                    (icf_uint16_t)block_id,
                    &p_rgm_context,
                    p_ecode);
    
            if (ICF_SUCCESS == ret_val)
            {
                ret_val = icf_rgm_register_user(
                        p_crm_pdb,
                        p_rgm_context);
                 
                /*
                if (ICF_SUCCESS == ret_val)
                {
                    any_user_registered = ICF_TRUE;
                }
                */
            }
        } /* End of for loop */

        /*
        if (ICF_TRUE == any_user_registered)
        {
            ICF_RGM_TRACE(p_crm_pdb, ICF_TRACE_INIT_COMPLETE_SUCCESSFUL)
            ret_val = ICF_SUCCESS;
        }
        else
        {
            ICF_RGM_SET_ECODE(p_crm_pdb, ICF_INIT_COMPLETE_FAILED)
            ret_val = ICF_FAILURE;
        }
        */
    }
    /*
    else
    {
        ICF_RGM_SET_ECODE(p_crm_pdb, ICF_INIT_COMPLETE_FAILED)
        ret_val = ICF_FAILURE;
    }
    */
    
    ICF_RGM_TRACE(p_crm_pdb, ICF_TRACE_INIT_COMPLETE_SUCCESSFUL)
    
    
    
    /* function exit trace */
    ICF_FUNCTION_EXIT(p_crm_pdb->p_internal_msg->msg_hdr.p_glb_pdb)
    
    ICF_RESET_MODULE(p_crm_pdb->p_internal_msg->msg_hdr.p_glb_pdb)

	ret_val = ICF_SUCCESS;
    return ret_val;


}
#endif

/****************************************************************************
 * FUNCTION:            icf_rgm_register_req_hdlr
 *
 * DESCRIPTION:         This function is called by the CFG module when
 *                      REGISTER_REQUEST is received.
 *
 *
 * RETURNS:             
 *
 ***************************************************************************/
icf_return_t icf_rgm_register_req_hdlr(
        INOUT icf_crm_pdb_st *p_crm_pdb)
{
    icf_return_t				ret_val = ICF_SUCCESS;
    icf_rgm_context_st			*p_rgm_context = ICF_NULL;
    
    icf_register_req_st			*p_recv_msg_payload = ICF_NULL;

    icf_line_data_st			*p_line_data = ICF_NULL;
    
    icf_app_id_t				temp_app_id = ICF_NULL;

/*SENDING RELATED*/
    icf_rgm_register_resp_st    register_response;
    icf_user_db_node_st			*p_user_db = ICF_NULL;
	icf_uint32_t				temp_bitmask = ICF_NULL;
    icf_uint16_t                user_db_node_num = ICF_NULL;

    icf_line_id_t               line_id = ICF_INVALID_LINE_ID;
    icf_uint8_t                 registration_refresh_multiplier = ICF_NULL;

    ICF_SET_MODULE(p_crm_pdb->p_internal_msg->msg_hdr.p_glb_pdb,
                        ICF_INT_MODULE_REGM)
    
    /* function entry trace */
    ICF_FUNCTION_ENTER(p_crm_pdb->p_internal_msg->msg_hdr.p_glb_pdb) 

    ICF_RGM_INITIALIZE_REG_RESP_STRUCT(register_response);
ICF_4_0_CRM_START
    /* Initialize the members of crm_pdb to be used by functions
     * that are called */
    p_crm_pdb->p_glb_pdb = p_crm_pdb->p_internal_msg->msg_hdr.p_glb_pdb;
    p_crm_pdb->error_cause = ICF_NULL;

    /* Get the pointer to ecode */
    p_crm_pdb->p_ecode = &(p_crm_pdb->p_internal_msg->msg_hdr.ecode);

    /* get the pointer to CFG data: NULL uptil now  */

    ret_val = icf_dbm_get_module_glb_data(
                p_crm_pdb->p_glb_pdb,
                (icf_int_module_id_t)(ICF_INT_MODULE_CFG),
                (icf_void_t **)(&(p_crm_pdb->p_config_data)),
                p_crm_pdb->p_ecode);     

    ICF_ASSERT(p_crm_pdb->p_glb_pdb,
                            (ICF_NULL != p_crm_pdb->p_config_data))

    /* get the pointer to RGM global data */
    ret_val = icf_dbm_get_module_glb_data(
                p_crm_pdb->p_glb_pdb,
                (icf_int_module_id_t)(ICF_INT_MODULE_REGM),
                (icf_void_t **)(&(p_crm_pdb->p_rgm_glb_data)),
                p_crm_pdb->p_ecode);     

    ICF_ASSERT(p_crm_pdb->p_glb_pdb,
                            (ICF_NULL != p_crm_pdb->p_rgm_glb_data))

    /********************ICF 8.0***************************/    
    /* Now set the variable is_reg_triggered. This field indicates if any 
       registration request has been triggered by application. It is set on
       the receipt of the first register request from application */

    p_crm_pdb->p_config_data->is_reg_triggered = ICF_TRUE;
        
        
/*SENDING RELATED*/
    ICF_ASSERT(p_crm_pdb->p_glb_pdb,
                    (ICF_NULL != p_crm_pdb->p_glb_pdb->p_recv_msg))
    temp_app_id = p_crm_pdb->p_glb_pdb->p_recv_msg->hdr.app_id;
        
     

    /* Get the pointer to the payload received with the API */
    p_recv_msg_payload = (icf_register_req_st *)
                                    (p_crm_pdb->p_recv_payload);

/*SENDING RELATED*/

    register_response.app_id = temp_app_id;
    register_response.line_id = p_recv_msg_payload->line_id;
    register_response.p_crm_pdb = p_crm_pdb;
    register_response.p_rgm_context = ICF_NULL;
    register_response.options_flag = ICF_NULL;
    register_response.p_request_payload = p_recv_msg_payload;
    
    /* Initializing the valid_param to be used in validating functions */
    p_crm_pdb->is_valid_param = ICF_TRUE;

    /*SPR 13978 Aman 03-01-07*/

    if(ICF_FAILURE == icf_cmn_validate_open_nw_server(p_crm_pdb->p_glb_pdb,
	    				p_recv_msg_payload->line_id))
    {
	    p_crm_pdb->is_valid_param = ICF_FALSE;
        p_crm_pdb->error_cause = ICF_CAUSE_OPEN_NW_SERVER_FAILED;
        ret_val = ICF_FAILURE;
    }
    else
    {
    	/* Invoke the validate function */
    	icf_regm_validate_request_header(p_crm_pdb);
    }

    if (ICF_FALSE == p_crm_pdb->is_valid_param)
    {
        ret_val = ICF_FAILURE;

    }/* Ends here: if (ICF_FALSE == p_crm_pdb->is_valid_param)*/

    else
    {
        icf_rgm_validate_register_api(p_crm_pdb);
        
        if (ICF_FALSE == p_crm_pdb->is_valid_param)
        {
            p_crm_pdb->error_cause = ICF_CAUSE_INVALID_LINE_ID;
            ret_val = ICF_FAILURE;
            
        }/* Ends here: if (ICF_FALSE == p_crm_pdb->is_valid_param)*/
    }
    
    if (ICF_SUCCESS == ret_val)
    {
        /* ret_val made failure if the app id invalid or network deactivated. */
        ret_val = icf_rgm_chk_app_details(p_crm_pdb,
            temp_app_id);
    }

    if(ICF_SUCCESS == ret_val)
	{
	   if (ICF_SUCCESS == icf_dbm_fetch_line_cntxt_blk(
				   p_crm_pdb->p_glb_pdb, p_recv_msg_payload->line_id,
				   &p_line_data, p_crm_pdb->p_ecode))
	   {
		   if ((ICF_CFG_SELF_ADDR_PER_LINE ==
					   p_crm_pdb->p_config_data->self_addr_per_line) &&
				   (ICF_FALSE == p_line_data->line_data_init_complete))
		   {
			   p_crm_pdb->error_cause = ICF_CAUSE_LINE_NOT_CONFIGURED;
			   p_crm_pdb->is_valid_param = ICF_FALSE;
			   ret_val = ICF_FAILURE;
		   }
	   }
	   else
	   {
		   ret_val = ICF_FAILURE;
	   }
    }

/* Start: Multiple Public Users ICF5.0*/    
    if (ICF_SUCCESS == ret_val )
	{
		if(ICF_SUCCESS == icf_rgm_check_line_data(p_crm_pdb))
		{
			ret_val = ICF_SUCCESS;
		}
		else if(ICF_CAUSE_NO_PUBLIC_URI == p_crm_pdb->error_cause)
		{
#ifdef IMS_CLIENT
ICF_CHECK_IMS_START(p_crm_pdb->p_glb_pdb,ICF_TRUE)
			if(ICF_REG_REQ_TO_PRESENT & p_recv_msg_payload->bit_mask)
			{
				if(ICF_SUCCESS == icf_dbm_add_public_uri_for_line(
								   p_crm_pdb->p_glb_pdb,
								   p_recv_msg_payload->line_id,
								   &(p_recv_msg_payload->to),
								   p_crm_pdb->p_ecode))
				{
					icf_dbm_get_public_uri_node_from_uri_for_line (
									p_crm_pdb->p_glb_pdb,
                                    p_recv_msg_payload->line_id,
                                    &(p_recv_msg_payload->to),
									ICF_USER_DB_USE_PUBLIC_URIS,
									&p_user_db,
									p_crm_pdb->p_ecode);

					p_user_db->flags |= ICF_USER_DB_PUBLIC_NODE_TEMPORARY;
					ret_val = ICF_SUCCESS;
				
				}
				else
				{
					ret_val = ICF_FAILURE;			
				}
			}
			else 
			{
				ret_val = ICF_FAILURE;
			}
ICF_CHECK_IMS_END
#endif
#ifdef NON_IMS_CLIENT
			ret_val = ICF_FAILURE;
#endif
		}
		else
		{
			ret_val = ICF_FAILURE;
		}
	}

	/* Register should be sent on the network */
    if (ICF_SUCCESS == ret_val)                
    {
        /*  Fix for CSR 1-7193955: SPR 19668 */
        /*  Fetch the registartion_refresh_multiplier if present */
        if(ICF_REG_REQ_REFRESH_MULTIPLIER_PRESENT & p_recv_msg_payload->bit_mask)
        {
            if((0 < p_recv_msg_payload->registration_refresh_multiplier) &&
               (100 > p_recv_msg_payload->registration_refresh_multiplier))
            {
                registration_refresh_multiplier = 
                           p_recv_msg_payload->registration_refresh_multiplier;
            }
        }

        line_id = p_recv_msg_payload->line_id;

		/* SPR 19927: Klocworks warning removal, in case line id exceeds 
		 * ICF_MAX_NUM_OF_LINES, no furher proceeding should be done as it
	     * will result into ABR.
	     */	 
        if(ICF_MAX_NUM_OF_LINES <= line_id)
        {
            ICF_PRINT(((icf_uint8_t *)"[RGM]: Invalid Line id received\n"))
            return ICF_FAILURE;
        }


        icf_dbm_fetch_line_cntxt_blk(
            p_crm_pdb->p_glb_pdb, p_recv_msg_payload->line_id,
            &p_line_data, p_crm_pdb->p_ecode); 

		/* if payload has "To" field then only one register should be sent */
		if(ICF_REG_REQ_TO_PRESENT & p_recv_msg_payload->bit_mask)
		{
			if (ICF_FAILURE == icf_rgm_get_ctxt_for_pub_uri(p_crm_pdb->p_glb_pdb,
							p_recv_msg_payload->line_id,
							&(p_recv_msg_payload->to),
							&p_rgm_context, 
							p_crm_pdb->p_ecode))
			{
				ret_val = ICF_FAILURE;
				p_crm_pdb->error_cause = *(p_crm_pdb->p_ecode);
                register_response.options_flag |= ICF_RGM_RESPONSE_CFM;
                register_response.status = ICF_FAILURE;
                register_response.options_flag |= 
                    ICF_RGM_SINGLE_APP;/*send to single app*/
                register_response.options_flag |= ICF_RGM_REQUEST_COPY;
                register_response.error_cause = p_crm_pdb->error_cause;
                register_response.request_type = ICF_NULL;
                
                p_crm_pdb->resp_to_be_sent = ICF_FALSE;/*for CFG to not send response*/

			}/* rgm_context could not be obtained */
			else
            {
				p_crm_pdb->p_rgm_context = p_rgm_context;
                p_crm_pdb->error_cause = ICF_NULL;

                /*  Fix for CSR 1-7193955: SPR 19668 */
                /*  update the value of registartion_refresh_multiplier in
                 *  rgm_context when registration_refresh_multiplier is received
                 *  and either of the following is true
                 *  1: Exipry os not received in ICF_REGISTER_REQ
                 *  2: if expiry is received then its value is no zero.
                 */
                if((ICF_REG_REQ_REFRESH_MULTIPLIER_PRESENT & p_recv_msg_payload->bit_mask) && 
                   (((ICF_REG_REQ_EXPIRY_PRESENT & p_recv_msg_payload->bit_mask) &&
                   (0 != p_recv_msg_payload->expiry)) || 
                   (!(ICF_REG_REQ_EXPIRY_PRESENT & p_recv_msg_payload->bit_mask))))
                {
                    p_rgm_context->registration_refresh_multiplier = 
                                            registration_refresh_multiplier;
                }

                /* rgm_context found */
                if(ICF_REG_REQ_EXPIRY_PRESENT & p_recv_msg_payload->bit_mask)
                {
                    p_rgm_context->rgm_duration = p_recv_msg_payload->expiry;
                }
                else
                {
                    p_rgm_context->rgm_duration = p_line_data->registration_duration; 
                }

                if(ICF_REG_REQ_DEREG_SPECIFIC_CONTACT_PRESENT &
                      p_recv_msg_payload->bit_mask)
                {
                    /* if user not cofigured then use dereg_specific_contact value
                     * provided by the application in register request */ 
                    if(ICF_RGM_00_IDLE == p_rgm_context->rgm_state)
                    {
                        p_rgm_context->dereg_specific_contact =
                            p_recv_msg_payload->dereg_specific_contact;
                    }
                }

                /* Populate the contact_list_required field of p_rgm_context with 
                 * the contact_list_required value received from application in
                 * API ICF_REGISTER_REQ.
                 */

                if(ICF_REG_REQ_CONTACT_LIST_REQD & p_recv_msg_payload->bit_mask)
                {
                    p_rgm_context->contact_list_required =
                        p_recv_msg_payload->contact_list_required;
                }       

                /* populate the payload values received in the register request */
                if(ICF_FAILURE == icf_rgm_update_ctx_on_register_req(p_crm_pdb))
                {
                    ret_val = ICF_FAILURE;
                }
                    /* Call the FSM handler */
                else if ( p_rgm_context->rgm_state < ICF_RGM_MAX_STATES)
                {
                    ret_val = ((icf_rgm_glb_data_st *)(p_crm_pdb->p_rgm_glb_data))->icf_rgm_state_machine
                        [p_rgm_context->rgm_state][ICF_RGM_REG_REQ_RECVD](p_crm_pdb);
                }
            }
		}

		/* if  payload does not have "To" field then all the users present on the line shall be registered */
		else
		{
            /* SPR 18940: The ongoing reg counter is maintained per line so that
             * in case registration is ongoing for User1 on Line1 and new registration
             * request is received for User2 on Line 2 then it should be allowed.
             */ 
			if(0 == ((icf_rgm_glb_data_st *)(p_crm_pdb->p_rgm_glb_data))->
                            reg_count_for_line[line_id])
			{
				temp_bitmask = p_recv_msg_payload->bit_mask;
#if 0
				if (ICF_SUCCESS == icf_dbm_fetch_line_cntxt_blk(
				p_crm_pdb->p_glb_pdb, p_recv_msg_payload->line_id,
				&p_line_data, p_crm_pdb->p_ecode))
#endif
				{
                    /* SPR 18940 : This code block is moved out of while loop as it 
                     * is required to be executed once when REGISTER_ALL_USER request
                     * is received from application and a dummy succes register_cfm
                     * to be sent and then all the registrations are to be attempted.
                     */ 
                    p_crm_pdb->p_glb_pdb->line_id = p_recv_msg_payload->line_id;
                    register_response.options_flag |= ICF_RGM_RESPONSE_CFM;
                    register_response.status = ICF_SUCCESS;
                    register_response.options_flag |= ICF_RGM_SINGLE_APP;
                    register_response.options_flag |= ICF_RGM_REQUEST_COPY;
                    /* in case of no TO address present, at this momenet there are no URIs present */
                    /*register_response.options_flag |= ICF_RGM_FILL_URIS;*/
                    register_response.error_cause = ICF_NULL;
                    register_response.p_rgm_context = ICF_NULL;
                    register_response.request_type = ICF_REQUEST_TYPE_REGISTER_ALL_USERS_ON_LINE;
                    p_crm_pdb->resp_to_be_sent = ICF_FALSE;
                    ret_val = icf_rgm_send_response(p_crm_pdb->p_glb_pdb,
                            &register_response, p_crm_pdb->p_ecode);
                    /* Reset this bitmask */

					p_user_db = p_line_data->p_user_db;
					while (ICF_NULL != p_user_db)
					{
                                             user_db_node_num++;
                                             if (icf_port_strncmp((icf_uint8_t *)p_user_db->public_user.addr_val.str, \
                                                         (icf_uint8_t * )(ICF_WILDCARD_USER), icf_port_strlen((icf_uint8_t * ) ICF_WILDCARD_USER)))
                                               { 
						   if (ICF_SUCCESS == icf_rgm_get_ctxt_for_pub_uri(p_crm_pdb->p_glb_pdb,
										p_recv_msg_payload->line_id,
										&(p_user_db->public_user),
										&p_rgm_context, 
										p_crm_pdb->p_ecode))
						     {
							  p_crm_pdb->p_rgm_context = p_rgm_context;
							  p_crm_pdb->error_cause = ICF_NULL;
                              /* Fix for CSR 1-7193955: SPR 19668*/
                              /*  update the value of registartion_refresh_multiplier in
                               *  rgm_context when registration_refresh_multiplier is received
                               *  and either of the following is true
                               *  1: Exipry os not received in ICF_REGISTER_REQ
                               *  2: if expiry is received then its value is no zero.
                               */

                              if((ICF_REG_REQ_REFRESH_MULTIPLIER_PRESENT & \
                                    p_recv_msg_payload->bit_mask) && 
                                 (((ICF_REG_REQ_EXPIRY_PRESENT & \
                                    p_recv_msg_payload->bit_mask) &&
                                 (0 != p_recv_msg_payload->expiry)) || 
                                 (!(ICF_REG_REQ_EXPIRY_PRESENT & \
                                    p_recv_msg_payload->bit_mask))))
                              {
                                  p_rgm_context->registration_refresh_multiplier = 
                                         registration_refresh_multiplier;
                              }


                              if(ICF_DUMMY_APP_ID != p_crm_pdb->p_glb_pdb->p_recv_msg->hdr.app_id)
                              {
                                  p_rgm_context->indicator |= ICF_REGISTER_FOR_ALL_USER;

                                 /* SPR 18940: The ongoing reg counter is maintained per line so that
                                  * in case registration is ongoing for User1 on Line1 and new registration
                                  * request is received for User2 on Line 2 then it should be allowed.
                                  * Therefore increment the Ongoing Reg Counter for the particular line.
                                  */ 
                                  ((icf_rgm_glb_data_st *)(p_crm_pdb->p_rgm_glb_data))->
                                         reg_count_for_line[line_id]++;
                              }

                              p_recv_msg_payload->bit_mask |= ICF_REG_REQ_TO_PRESENT;
                              icf_port_strcpy(p_recv_msg_payload->to.addr_val.str,
                                      p_user_db->public_user.addr_val.str);
                              p_recv_msg_payload->to.addr_val.str_len = 
                                  p_user_db->public_user.addr_val.str_len;
                              p_recv_msg_payload->to.addr_type = 
                                  p_user_db->public_user.addr_type;

                              if(ICF_REG_REQ_EXPIRY_PRESENT & p_recv_msg_payload->bit_mask)
                              {
                                  p_rgm_context->rgm_duration = p_recv_msg_payload->expiry;
                              }
                              else
                              {
                                  p_rgm_context->rgm_duration = p_line_data->registration_duration;
                              }     

                              if(ICF_REG_REQ_DEREG_SPECIFIC_CONTACT_PRESENT &
                                      p_recv_msg_payload->bit_mask)
                              {
                                  /* if user not cofigured then use dereg_specific_contact value
                                   * provided by the application in register request */ 
                                  if(ICF_RGM_00_IDLE == p_rgm_context->rgm_state)
                                  {
                                      p_rgm_context->dereg_specific_contact =
                                          p_recv_msg_payload->dereg_specific_contact;
                                  }
                              }

                              /* Populate the contact_list_required field of p_rgm_context with 
                               * the contact_list_required value received from application in
                               * API ICF_REGISTER_REQ.
                               */
                              if(ICF_REG_REQ_CONTACT_LIST_REQD & p_recv_msg_payload->bit_mask)
                              {
                                  p_rgm_context->contact_list_required =
                                      p_recv_msg_payload->contact_list_required;
                              }  

                              /* populate the payload values received in the register request */
                              if(ICF_FAILURE == icf_rgm_update_ctx_on_register_req(p_crm_pdb))
                              {
                                  ret_val = ICF_FAILURE;
                              }
                              /* Call the FSM handler */
                              else if ( p_rgm_context->rgm_state < ICF_RGM_MAX_STATES)
                              {
                                      ret_val = ((icf_rgm_glb_data_st *)(p_crm_pdb->p_rgm_glb_data))->icf_rgm_state_machine
                                          [p_rgm_context->rgm_state][ICF_RGM_REG_REQ_RECVD](p_crm_pdb);
                              }
						     }
                          }
                           else
                             { 
                               /* If only wildcarded use is configured on line 
                                * the error will be populated which will be 
                                * sent to user in register_chm,no error cause
                                * is filled if successfull registeration req
                                * are sent to network for other users configured
                                * on line */
                                if((1 == user_db_node_num) && (ICF_NULL == p_user_db->p_next))
                                  {
                                    p_crm_pdb->error_cause = ICF_CAUSE_WILDCARDED_URI;
                                    ret_val = ICF_FAILURE;
                                  }
                             }
						p_user_db = p_user_db->p_next;
					}
				}
			}
			else
			{
				p_crm_pdb->error_cause = ICF_CAUSE_ONGOING_REGISTRATION;
			}
		}
	}
ICF_4_0_CRM_END
	if(ICF_NULL == p_rgm_context)
	{
        register_response.options_flag = ICF_NULL;
		register_response.options_flag |= ICF_RGM_RESPONSE_CFM;
		register_response.status = ICF_FAILURE;
		register_response.options_flag |= 
						ICF_RGM_SINGLE_APP;/*send to single app*/
		register_response.options_flag |= ICF_RGM_REQUEST_COPY;
		register_response.error_cause = p_crm_pdb->error_cause;
		register_response.request_type = ICF_NULL;
		register_response.p_rgm_context = p_rgm_context;
		p_crm_pdb->resp_to_be_sent = ICF_FALSE;/*for CFG to not send response*/
		ret_val = icf_rgm_send_response(p_crm_pdb->p_glb_pdb,
						&register_response, p_crm_pdb->p_ecode);
	}

/* End: Multiple Public Users ICF5.0*/
    /* function exit trace */
    ICF_FUNCTION_ENTER(p_crm_pdb->p_glb_pdb) 

    ICF_RESET_MODULE(p_crm_pdb->p_internal_msg->msg_hdr.p_glb_pdb)
    return ret_val;
}/* Function icf_rgm_register_req_hdlr() ends here. */


ICF_CRM_END




#ifndef ICF_FEATURE_ABANDON/* For ICF features not supported in ICF */
/****************************************************************************
 * FUNCTION:            icf_rgm_modify_user
 *
 * DESCRIPTION:         This function is called by the CFG module when
 *                      MODIFY_USER is received and user address is to be
 *                      ADDED or DELETED on a line or on all lines.
 *
 *
 * RETURNS:             
 *
 ***************************************************************************/
icf_return_t icf_rgm_modify_user(
        INOUT icf_crm_pdb_st *p_crm_pdb)
{

    icf_glb_pdb_st *p_glb_pdb; 
    icf_error_t *p_ecode = ICF_ERROR_NONE;
    icf_config_data_st *p_dbm_config_data = ICF_NULL;
    icf_return_t ret_val;

    ICF_SET_MODULE(p_crm_pdb->p_internal_msg->msg_hdr.p_glb_pdb,
            ICF_INT_MODULE_REGM)
    
    /* function entry trace */
    ICF_FUNCTION_ENTER(p_crm_pdb->p_internal_msg->msg_hdr.p_glb_pdb) 

    ICF_RGM_TRACE(p_crm_pdb,ICF_TRACE_MODIFY_USER_ADDRESS_RECVD)

    p_glb_pdb = p_crm_pdb->p_internal_msg->msg_hdr.p_glb_pdb;
    ICF_ASSERT(
            p_crm_pdb->p_internal_msg->msg_hdr.p_glb_pdb,
            (ICF_NULL != p_glb_pdb))
    
    p_ecode = &(p_crm_pdb->p_internal_msg->msg_hdr.ecode);
    
    if (ICF_FALSE == p_crm_pdb->rgm_pdb.all_lines)
    {
        ret_val = icf_rgm_modify_user_not_all_lines(p_crm_pdb);
    }
    
    else 
    {
        /* Request the DBM for getting pointer to config data 
            structure */
        ret_val = icf_dbm_get_module_glb_data(
                p_glb_pdb,
                (icf_int_module_id_t)(ICF_INT_MODULE_CFG),
                (icf_void_t **)(&p_dbm_config_data),
                p_ecode);     

        ICF_ASSERT(
                p_crm_pdb->p_internal_msg->msg_hdr.p_glb_pdb,
                (ICF_NULL != p_dbm_config_data))
    
        /* Call to DBM for getting pointer to config data structure succeeded*/
        if (ICF_SUCCESS == ret_val)
        {
            ret_val = icf_rgm_modify_user_all_lines(p_crm_pdb,
                    p_dbm_config_data->max_lines);
        }
    }

    /* function exit trace */
    ICF_FUNCTION_EXIT(p_crm_pdb->p_internal_msg->msg_hdr.p_glb_pdb)
   
    ICF_RESET_MODULE(p_crm_pdb->p_internal_msg->msg_hdr.p_glb_pdb)
    
    return ret_val;

}


/****************************************************************************
 * FUNCTION:            icf_rgm_modify_user_not_all_lines
 *
 * DESCRIPTION:         This function is called by the function
 *                      icf_rgm_modify_user when user modification is not
 *                      needed on all the lines.
 *
 *
 * RETURNS:             
 *
 ***************************************************************************/
icf_return_t icf_rgm_modify_user_not_all_lines(
        INOUT icf_crm_pdb_st *p_crm_pdb)
{

    /* loop counter */
    icf_uint8_t loop_index;
    icf_uint8_t api_index;
    icf_boolean_t failure_flag = ICF_FALSE;
    icf_glb_pdb_st *p_glb_pdb; 
/*   icf_error_t *p_ecode = ICF_ERROR_NONE;*/
    icf_address_st *p_user_address;
    icf_line_id_t line_id;
    icf_cfg_cc_set_self_id_params_req_st *p_recv_msg_payload;
    icf_return_t ret_val = ICF_SUCCESS;

    icf_rgm_modify_user_not_all_lines_list_st modify_user_context;

    /* function entry trace */
    ICF_FUNCTION_ENTER(p_crm_pdb->p_internal_msg->msg_hdr.p_glb_pdb) 
        
    p_glb_pdb = p_crm_pdb->p_internal_msg->msg_hdr.p_glb_pdb;
    
/*    p_ecode = &(p_crm_pdb->p_internal_msg->msg_hdr.ecode);*/
    
    p_recv_msg_payload = (icf_cfg_cc_set_self_id_params_req_st *)
                (p_glb_pdb->p_recv_msg->payload);
    
    /* Initialize number of elements */
    modify_user_context.num_of_elements = ICF_NULL;
    
    for (loop_index = 0; loop_index < p_crm_pdb->rgm_pdb.num_of_elements; 
            loop_index++)
    {
        /* Copy the index into local context */
        modify_user_context.local_context[loop_index].index_in_api = 
            p_crm_pdb->rgm_pdb.api_indexes[loop_index];

        /* This variable is used for enhancing readability of code */
        api_index = modify_user_context.local_context[loop_index].
            index_in_api;
        
        /* Get pointer to user address */
        p_user_address = &(p_recv_msg_payload->self_addr_list.
                self_addr[api_index].addr);
        
        /* Get line ID */
        line_id = p_recv_msg_payload->self_addr_list.
                self_addr[api_index].line_id;
        
        /* Set the pointer to registration context to NULL */
        modify_user_context.local_context[loop_index].p_rgm_context = 
            ICF_NULL;
        
        /* Set Status field to default */
        modify_user_context.local_context[loop_index].status = 0;
        
        if (ICF_CONFIG_ACTION_ADD == p_recv_msg_payload->self_addr_list.
                    self_addr[api_index].action)
        {
            ret_val = icf_rgm_add_user(
                    p_crm_pdb,
                    line_id,
                    p_user_address,
                    &(modify_user_context.local_context[loop_index].status),
                    &(modify_user_context.local_context[loop_index].
                        p_rgm_context));
            
            if (ICF_FAILURE == ret_val)
            {
                /* set failure flag */
                failure_flag = ICF_TRUE;
                break;
            }
            
            else
            {
                modify_user_context.num_of_elements++;
            }

        }

        else if (ICF_CONFIG_ACTION_DELETE == p_recv_msg_payload->
                self_addr_list.self_addr[api_index].action)
        {
            ret_val = icf_rgm_delete_user(
                    p_crm_pdb,
                    line_id,
                    p_user_address,
                    &(modify_user_context.local_context[loop_index].status),
                    &(modify_user_context.local_context[loop_index].
                        p_rgm_context));
            
            if (ICF_FAILURE == ret_val)
            {
                /* set failure flag */
                failure_flag = ICF_TRUE;
                break;
            }

            else
            {
                modify_user_context.num_of_elements++;
            }
            
        }
    }/* End of for loop */

    if (ICF_TRUE == failure_flag)
    {
        ICF_RGM_SET_ECODE(p_crm_pdb,ICF_MODIFY_USER_FAILED)

        /* Rollback */
        icf_rgm_modify_user_roll_not_all_lines(
                p_crm_pdb,
                &modify_user_context);
        
        ret_val = ICF_FAILURE;
        
    }

    else 
    {
        /* process the blocks */
        for (loop_index = 0; loop_index < modify_user_context.num_of_elements; 
                loop_index ++)
        {
            ICF_RGM_TRACE(p_crm_pdb,ICF_TRACE_PROCESS_MODIFY_USER_BLK)
            ret_val = icf_rgm_process_modify_user_block(
                    p_crm_pdb,
                    &(modify_user_context.local_context[loop_index]));
            
            if (ret_val == ICF_FAILURE)
            {
                /* RAISE ALARM */
                ICF_RGM_SET_ECODE(p_crm_pdb,
                        ICF_PROCESS_MODIFY_USER_BLK_FAILED)

            }
        }
        ret_val = ICF_SUCCESS;
    }
    ICF_FUNCTION_EXIT(p_glb_pdb)
    return ret_val;

}

/****************************************************************************
 * FUNCTION:            icf_rgm_modify_user_all_lines
 *
 * DESCRIPTION:         This function is called by the function
 *                      icf_rgm_modify_user when user modification is
 *                      needed on all the lines.
 *
 *
 * RETURNS:             
 *
 ***************************************************************************/
icf_return_t icf_rgm_modify_user_all_lines(
        INOUT icf_crm_pdb_st *p_crm_pdb,
        IN    icf_line_id_t max_lines)
{

    /* loop counter */
    icf_line_id_t loop_index = 0;
   /* icf_uint8_t api_index;*/
    icf_boolean_t failure_flag = ICF_FALSE;
    icf_glb_pdb_st *p_glb_pdb = ICF_NULL; 
    icf_error_t *p_ecode = ICF_ERROR_NONE;
    icf_address_st *p_user_address = ICF_NULL;
    icf_line_id_t line_id;
    icf_line_data_st *p_dbm_line_data = ICF_NULL;
    icf_cfg_cc_set_self_id_params_req_st *p_recv_msg_payload = ICF_NULL;
    icf_return_t ret_val = ICF_SUCCESS;

    icf_rgm_modify_user_all_lines_list_st modify_user_context;

    /* function entry trace */
    ICF_FUNCTION_ENTER(p_crm_pdb->p_internal_msg->msg_hdr.p_glb_pdb) 
        
    p_glb_pdb = p_crm_pdb->p_internal_msg->msg_hdr.p_glb_pdb;
    
    p_ecode = &(p_crm_pdb->p_internal_msg->msg_hdr.ecode);
    
    p_recv_msg_payload = (icf_cfg_cc_set_self_id_params_req_st *)
                (p_glb_pdb->p_recv_msg->payload);

    p_user_address = &(p_recv_msg_payload->self_addr_list.self_addr[0].addr);
    
    /* Initialize number of elements */
    modify_user_context.num_of_elements = 0;

    if (ICF_CONFIG_ACTION_ADD == p_recv_msg_payload->self_addr_list.
            self_addr[0].action)
    {
        /* Loop for all lines */
        for (line_id = 0; line_id < max_lines; line_id++)
        {
            if(ICF_FAILURE == icf_rgm_line_reg_configured(
                        p_glb_pdb,
                        line_id,
                        (icf_line_data_st **)(&p_dbm_line_data),
                        p_ecode))
            {
                ret_val = icf_dbm_fetch_line_cntxt_blk(
                        p_glb_pdb,
                        line_id,
                        (icf_line_data_st **)(&p_dbm_line_data),
                        p_ecode);
                
                continue;
            }
            else
            {
                /* Copy the line_id into local context */
                modify_user_context.local_context[loop_index].index_in_api = 
                    line_id;

                /* Set the pointer to registration context to NULL */
                modify_user_context.local_context[loop_index].p_rgm_context = 
                    ICF_NULL;
        
                /* Set Status field to default */
                modify_user_context.local_context[loop_index].status = 0;
                

                ret_val = icf_rgm_add_user(
                    p_crm_pdb,
                    line_id,
                    p_user_address,
                    &(modify_user_context.local_context[loop_index].status),
                    &(modify_user_context.local_context[loop_index].
                        p_rgm_context));
            
                if (ICF_FAILURE == ret_val)
                {
                    /* set failure flag */
                    failure_flag = ICF_TRUE;
                    break;
                }
            
                else
                {
                    modify_user_context.num_of_elements++;
                    loop_index++;
                }

            }
        }/* end of for loop */
    }
    else if (ICF_CONFIG_ACTION_DELETE == p_recv_msg_payload->self_addr_list.
            self_addr[0].action)
    {
        /* Loop for all lines */
        for (line_id = 0; line_id < max_lines; line_id++)
        {
            if((ICF_FAILURE == icf_rgm_line_reg_configured(
                        p_glb_pdb,
                        line_id,
                        (icf_line_data_st **)(&p_dbm_line_data),
                        p_ecode)) || 
                (ICF_FAILURE == icf_dbm_check_if_user_exits_in_line(
                            p_glb_pdb,
                            p_user_address,
                            p_dbm_line_data,
                            (icf_boolean_t) ICF_FALSE,
                            p_ecode)))
            {
                    continue;
            }
            else
            {
                /* Copy the line_id into local context */
                modify_user_context.local_context[loop_index].index_in_api = 
                    line_id;

                /* Set the pointer to registration context to NULL */
                modify_user_context.local_context[loop_index].p_rgm_context = 
                    ICF_NULL;
        
                /* Set Status field to default */
                modify_user_context.local_context[loop_index].status = 0;
                

                ret_val = icf_rgm_delete_user(
                    p_crm_pdb,
                    line_id,
                    p_user_address,
                    &(modify_user_context.local_context[loop_index].status),
                    &(modify_user_context.local_context[loop_index].
                        p_rgm_context));
            
                if (ICF_FAILURE == ret_val)
                {
                    /* set failure flag */
                    failure_flag = ICF_TRUE;
                    break;
                }
            
                else
                {
                    modify_user_context.num_of_elements++;
                    loop_index++;
                }

            }
        }/* end of for loop */
    }
    
    if (ICF_TRUE == failure_flag)
    {
        ICF_RGM_SET_ECODE(p_crm_pdb,ICF_MODIFY_USER_FAILED)

        /* Rollback */
        icf_rgm_modify_user_roll_all_lines(
                p_crm_pdb,
                &modify_user_context);
        
        ret_val = ICF_FAILURE;
        
    }

    else 
    {
        /* process the blocks */
        for (loop_index = 0; loop_index < modify_user_context.num_of_elements; 
                loop_index ++)
        {
            ICF_RGM_TRACE(p_crm_pdb,ICF_TRACE_PROCESS_MODIFY_USER_BLK)
            ret_val = icf_rgm_process_modify_user_block(
                    p_crm_pdb,
                    &(modify_user_context.local_context[loop_index]));
            
            if (ret_val == ICF_FAILURE)
            {
                /* RAISE ALARM */
                ICF_RGM_SET_ECODE(p_crm_pdb,
                        ICF_PROCESS_MODIFY_USER_BLK_FAILED)

            }
        }
        /* We have to return API failure in case not even a single line
         * configured.
         */
        if (ICF_NULL == modify_user_context.num_of_elements)
        {
            ret_val = ICF_FAILURE;
        }
        else
        {
            ret_val = ICF_SUCCESS;
        }
    }
    
   ICF_FUNCTION_EXIT(p_glb_pdb) 
    return ret_val;

}

/****************************************************************************
 * FUNCTION:            icf_rgm_add_user
 *
 * DESCRIPTION:         This function is called by the functions of RGM module
 *                      to create an entry of a user on a particular line in
 *                      the registration context.
 *
 *
 * RETURNS:             
 *
 ***************************************************************************/
icf_return_t icf_rgm_add_user(
        INOUT icf_crm_pdb_st *p_crm_pdb,
        IN    icf_line_id_t line_id,
        IN    icf_address_st *p_user_address,
        OUT   icf_uint8_t *p_status,
        OUT   icf_rgm_context_st **p_p_rgm_context)
{
    icf_glb_pdb_st *p_glb_pdb;
    icf_error_t *p_ecode = ICF_ERROR_NONE;
    icf_line_data_st *p_dbm_line_data = ICF_NULL;
    icf_return_t ret_val;
    
    /* function entry trace */
    ICF_FUNCTION_ENTER(p_crm_pdb->p_internal_msg->msg_hdr.p_glb_pdb) 
        
    p_glb_pdb = p_crm_pdb->p_internal_msg->msg_hdr.p_glb_pdb;
    
    p_ecode = &(p_crm_pdb->p_internal_msg->msg_hdr.ecode);

    
    /* Call DBM ADD USER function */
    ret_val = icf_dbm_regm_add_user(
            p_glb_pdb,
            line_id,
            p_user_address,
            p_status,
            p_p_rgm_context,
            p_ecode);

    if (ret_val == ICF_SUCCESS)
    {
        if (*p_status == ICF_RGM_DBM_NO_FREE_BLOCK)
        {
            /* Set Error cause */
            p_crm_pdb->error_cause = 
                ICF_CAUSE_MAX_REGISTRATION_CALLS_EXCEEDED;
            
            /* Rollback */
            /* send failure */
            ret_val = ICF_FAILURE;
        }

        else if (*p_status == ICF_RGM_DBM_PAIR_PRESENT_ALONE)
        {
            /* Rollback */
            /* send failure */
            ret_val = ICF_FAILURE;
        }

        /* SPR 7409 Fix Starts */
        else if (*p_status == ICF_RGM_DBM_NEW_BLOCK)
        {
            /* Initialize State */
            (*p_p_rgm_context)->rgm_state = ICF_RGM_00_IDLE;
            
            /* return success */
            ret_val = ICF_SUCCESS;            
        }

        else if ((*p_p_rgm_context)->rgm_state == ICF_RGM_05_DEREGISTERING)
        {
            /* Set Error cause */
            p_crm_pdb->error_cause = 
                ICF_CAUSE_ACTION_NOT_ALLWD_DURING_REG_ONG;

            /* Rollback */
            /* return failure */
            ret_val = ICF_FAILURE;
        }
        else if (*p_status == ICF_RGM_DBM_DURATION_CHANGE || 
                *p_status == ICF_RGM_DBM_NO_ACTION)
        {
            if ((*p_p_rgm_context)->indicator & ICF_RGM_DELETE_INDICATION)
            {
                /* Reset the flag marked for Deletion */
                (*p_p_rgm_context)->indicator &= 
                    (~(ICF_RGM_DELETE_INDICATION));

                /* Delete the 2 nd line ID from the context. In this 
                 * particular scenario only 2 lines will be in context of
                 * which 2 nd line is to be deleted from context which was
                 * added due to this ADD request.
                 */ 
                
                /* Make number of lines count to one. This will logically
                 * Delete the 2nd line ID.
                 */
                (*p_p_rgm_context)->num_of_lines = 1;

                if (*p_status == ICF_RGM_DBM_DURATION_CHANGE)
                {
                    /* Get pointer to line block from DBM */
                    ret_val = icf_dbm_fetch_line_cntxt_blk(
                            p_glb_pdb,
                            (*p_p_rgm_context)->rgm_list_of_lines[0],       
                            &p_dbm_line_data,
                            p_ecode);

                    /* Store the old Duration in the Context */
                    (*p_p_rgm_context)->rgm_duration = 
                        p_dbm_line_data->registration_duration;
                    
                }
                
                /* Set error cause */
                p_crm_pdb->error_cause =
                    ICF_CAUSE_INVALID_ACTION;
                
                /* Rollback */
                /* Return Failure */
                ret_val = ICF_FAILURE;
            }
            else
            {
                /* Add the pointer to registration context to REGM Pdb */
                /* return success */
                ret_val = ICF_SUCCESS;
            }
        }

        /* SPR 7409 Fix Ends */
    }
    ICF_FUNCTION_EXIT(p_glb_pdb)
    return ret_val;
}


/****************************************************************************
 * FUNCTION:            icf_rgm_delete_user
 *
 * DESCRIPTION:         This function is called by the functions of RGM module
 *                      to delete the entry of a user on a particular line
 *                      from the registration context. 
 *
 *
 * RETURNS:             
 *
 ***************************************************************************/
icf_return_t icf_rgm_delete_user(
        INOUT icf_crm_pdb_st *p_crm_pdb,
        IN    icf_line_id_t line_id,
        IN    icf_address_st *p_user_address,
        OUT   icf_uint8_t *p_status,
        OUT   icf_rgm_context_st **p_p_rgm_context)
{
    icf_glb_pdb_st *p_glb_pdb;
    icf_error_t *p_ecode = ICF_ERROR_NONE;
    icf_return_t ret_val;
    
    /* function entry trace */
    ICF_FUNCTION_ENTER(p_crm_pdb->p_internal_msg->msg_hdr.p_glb_pdb) 
        
    p_glb_pdb = p_crm_pdb->p_internal_msg->msg_hdr.p_glb_pdb;
    
    p_ecode = &(p_crm_pdb->p_internal_msg->msg_hdr.ecode);

    /* Call DBM DELETE USER function */
    ret_val = icf_dbm_regm_delete_user(
            p_glb_pdb,
            line_id,
            p_user_address,
            p_status,
            p_p_rgm_context,
            p_ecode);

    if (ICF_SUCCESS == ret_val)
    {
        if (*p_status == ICF_RGM_DBM_USER_NOT_FOUND)
        {
            /* Rollback */
            /* send failure */
            ret_val = ICF_FAILURE;

        }

        else if (*p_status == ICF_RGM_DBM_NO_ACTION || *p_status == 
                ICF_RGM_DBM_DURATION_CHANGE)
        {    
            /* Add the pointer to registration context to REGM Pdb */
        
            /* NOTE -> In case of duration change in delete mark status as 
             * ICF_RGM_DBM_NO_ACTION as we dont want to trigger our
             * state machine if duration changes due to delete action.
             * Ref - Pavai and Aashish
             */
        
            /* return success */
            ret_val = ICF_SUCCESS;
        }
        
        else if (*p_status == ICF_RGM_DBM_DELETE_BLOCK)
        {
               /* SPR 16019: Deletion Allowed*/

            (*p_p_rgm_context)->indicator |= ICF_RGM_DELETE_INDICATION;

                /* Add the pointer to registration context to REGM Pdb */
                /* return success */
                ret_val = ICF_SUCCESS;
            }
        }
    ICF_FUNCTION_EXIT(p_glb_pdb)
    return ret_val;
}

/****************************************************************************
 * FUNCTION:            icf_rgm_modify_user_roll_not_all_lines
 *
 * DESCRIPTION:         This function is called by the functions of RGM module
 *                      to carry out rollback processing in case set self 
 *                      identifier API in which all_lines field is set to 
 *                      FALSE.
 *
 *
 * RETURNS:             
 *
 ***************************************************************************/
icf_return_t icf_rgm_modify_user_roll_not_all_lines(
        INOUT icf_crm_pdb_st *p_crm_pdb,
        INOUT icf_rgm_modify_user_not_all_lines_list_st 
        *p_modify_user_context)
{
    icf_int16_t loop_index; /* This should be signed int */
    icf_uint8_t api_index;
    icf_glb_pdb_st *p_glb_pdb; 
    icf_error_t *p_ecode = ICF_ERROR_NONE;
    icf_address_st *p_user_address;
    icf_line_id_t line_id;
    icf_cfg_cc_set_self_id_params_req_st *p_recv_msg_payload;
    icf_return_t ret_val;
    
    ICF_FUNCTION_ENTER(p_crm_pdb->p_internal_msg->msg_hdr.p_glb_pdb)
    
    ICF_RGM_TRACE(p_crm_pdb, ICF_TRACE_MODIFY_USER_ROLLBACK)
        
    p_glb_pdb = p_crm_pdb->p_internal_msg->msg_hdr.p_glb_pdb;
    
    p_ecode = &(p_crm_pdb->p_internal_msg->msg_hdr.ecode);
    
    p_recv_msg_payload = (icf_cfg_cc_set_self_id_params_req_st *)
                (p_glb_pdb->p_recv_msg->payload);

    for (loop_index = p_modify_user_context->num_of_elements - 1;
            loop_index >= 0; loop_index--)
    {
        api_index = p_modify_user_context->local_context[loop_index].
            index_in_api;

        /* Get pointer to user address */
        p_user_address = &(p_recv_msg_payload->self_addr_list.
                self_addr[api_index].addr);
        
        /* Get line ID */
        line_id = p_recv_msg_payload->self_addr_list.
                self_addr[api_index].line_id;
        
        if (ICF_CONFIG_ACTION_ADD == p_recv_msg_payload->self_addr_list.
                self_addr[api_index].action)
        {
            if (p_modify_user_context->local_context[loop_index].status == 
                    ICF_RGM_DBM_NEW_BLOCK)
            {
                /* Deallocate the block */
                ret_val = icf_rgm_clear_reg_context(
                        p_crm_pdb,
                        p_modify_user_context->local_context[loop_index].
                        p_rgm_context);
                
            }

            else if((p_modify_user_context->local_context[loop_index].status ==
                  ICF_RGM_DBM_NO_ACTION) || 
                    (p_modify_user_context->local_context[loop_index]
                      .status == ICF_RGM_DBM_DURATION_CHANGE))
            {
                ret_val = icf_dbm_regm_delete_user(
                        p_glb_pdb,
                        line_id,
                        p_user_address,
                        &(p_modify_user_context->local_context[loop_index].
                            status),
                        &(p_modify_user_context->local_context[loop_index].
                            p_rgm_context),
                        p_ecode);
                
            }
            
        }

        else if(ICF_CONFIG_ACTION_DELETE == p_recv_msg_payload->
                self_addr_list.self_addr[api_index].action)
        {
            if (p_modify_user_context->local_context[loop_index].status == 
                    ICF_RGM_DBM_DELETE_BLOCK)
            {
                /* Do nothing */
            }

            else
            {
                ret_val = icf_dbm_regm_add_user(
                        p_glb_pdb,
                        line_id,
                        p_user_address,
                        &(p_modify_user_context->local_context[loop_index].
                            status),
                        &(p_modify_user_context->local_context[loop_index].
                            p_rgm_context),
                        p_ecode);
                
            }
            
        }
    }/* End of for loop */
    
   ret_val = ICF_SUCCESS;
   
   ICF_FUNCTION_EXIT(p_glb_pdb)
   return ret_val; 
}

/****************************************************************************
 * FUNCTION:            icf_rgm_modify_user_roll_all_lines
 *
 * DESCRIPTION:         This function is called by the functions of RGM module
 *                      to carry out rollback processing in case set self 
 *                      identifier API in which all_lines field is set to 
 *                      TRUE.
 *
 *
 * RETURNS:             
 *
 ***************************************************************************/
icf_return_t icf_rgm_modify_user_roll_all_lines(
        INOUT icf_crm_pdb_st *p_crm_pdb,
        INOUT icf_rgm_modify_user_all_lines_list_st 
        *p_modify_user_context)
{
    icf_int16_t loop_index; /* This should be signed int */
    /*icf_uint8_t api_index;*/
    icf_glb_pdb_st *p_glb_pdb; 
    icf_error_t *p_ecode = ICF_ERROR_NONE;
    icf_address_st *p_user_address;
    icf_line_id_t line_id;
    icf_cfg_cc_set_self_id_params_req_st *p_recv_msg_payload;
    icf_return_t ret_val;
    
    ICF_FUNCTION_ENTER(p_crm_pdb->p_internal_msg->msg_hdr.p_glb_pdb)
    
    ICF_RGM_TRACE(p_crm_pdb, ICF_TRACE_MODIFY_USER_ROLLBACK)    
    
    p_glb_pdb = p_crm_pdb->p_internal_msg->msg_hdr.p_glb_pdb;
    
    p_ecode = &(p_crm_pdb->p_internal_msg->msg_hdr.ecode);
    
    p_recv_msg_payload = (icf_cfg_cc_set_self_id_params_req_st *)
                (p_glb_pdb->p_recv_msg->payload);

    p_user_address = &(p_recv_msg_payload->self_addr_list.self_addr[0].addr);

    if (ICF_CONFIG_ACTION_ADD == p_recv_msg_payload->self_addr_list.
            self_addr[0].action)
    {
        for (loop_index = p_modify_user_context->num_of_elements - 1;
                loop_index >= 0; loop_index--)
        {
            /* Get line_id */
            line_id = p_modify_user_context->local_context[loop_index].
                index_in_api;
            
            if (p_modify_user_context->local_context[loop_index].status == 
                    ICF_RGM_DBM_NEW_BLOCK)
            {
                /* Deallocate the block */
                ret_val = icf_rgm_clear_reg_context(
                        p_crm_pdb,
                        p_modify_user_context->local_context[loop_index].
                        p_rgm_context);
                
            }

            else if((p_modify_user_context->local_context[loop_index].status ==
                  ICF_RGM_DBM_NO_ACTION) || 
                    (p_modify_user_context->local_context[loop_index]
                     .status == ICF_RGM_DBM_DURATION_CHANGE))

            {
                ret_val = icf_dbm_regm_delete_user(
                        p_glb_pdb,
                        line_id,
                        p_user_address,
                        &(p_modify_user_context->local_context[loop_index].
                            status),
                        &(p_modify_user_context->local_context[loop_index].
                            p_rgm_context),
                        p_ecode);
                
            }
            
        }
    }
    else if (ICF_CONFIG_ACTION_DELETE == p_recv_msg_payload->self_addr_list.
            self_addr[0].action)
    {
        for (loop_index = p_modify_user_context->num_of_elements - 1;
                loop_index >= 0; loop_index--)
        {
            /* Get line_id */
            line_id = p_modify_user_context->local_context[loop_index].
                index_in_api;

            if (p_modify_user_context->local_context[loop_index].status == 
                    ICF_RGM_DBM_DELETE_BLOCK)
            {
                /* Do nothing */
            }

            else
            {
                ret_val = icf_dbm_regm_add_user(
                        p_glb_pdb,
                        line_id,
                        p_user_address,
                        &(p_modify_user_context->local_context[loop_index].
                            status),
                        &(p_modify_user_context->local_context[loop_index].
                            p_rgm_context),
                        p_ecode);
                
            }
            
        }
    }
    ret_val = ICF_SUCCESS;
    ICF_FUNCTION_EXIT(p_glb_pdb)
    return ret_val;
}    

/****************************************************************************
 * FUNCTION:            icf_rgm_process_modify_user_block
 *
 * DESCRIPTION:         This function is called by the functions of RGM module
 *                      after all blocks have been prepared to process the
 *                      blocks according to their states.
 *
 *
 * RETURNS:             
 *
 ***************************************************************************/
icf_return_t icf_rgm_process_modify_user_block(
        INOUT icf_crm_pdb_st *p_crm_pdb,
        INOUT icf_rgm_modify_user_st *p_block)
{
    icf_glb_pdb_st *p_glb_pdb; 
    icf_error_t *p_ecode = ICF_ERROR_NONE;
    icf_void_t *p_buff = ICF_NULL;
    icf_rgm_glb_data_st *p_rgm_glb_data;
    icf_return_t ret_val;

    ICF_FUNCTION_ENTER(p_crm_pdb->p_internal_msg->msg_hdr.p_glb_pdb)
    
    p_glb_pdb = p_crm_pdb->p_internal_msg->msg_hdr.p_glb_pdb;
    
    p_ecode = &(p_crm_pdb->p_internal_msg->msg_hdr.ecode);
    
    
    if (p_block->status == ICF_RGM_DBM_NEW_BLOCK)
    {
        /* Send registration requests */
        ret_val = icf_rgm_register_user(
                p_crm_pdb,
                p_block->p_rgm_context);
       
    }

    else if (p_block->status == ICF_RGM_DBM_NO_ACTION)
    {
        /* Do nothing */
        ret_val = ICF_SUCCESS;
    }
    
    else 
    {
        /* Load FSM */
        
        /* Call DBM to get the pointer to the RGM global data in 
         * p_buff
         */
        ret_val = icf_dbm_get_module_glb_data(
                p_glb_pdb,
                (icf_int_module_id_t)(ICF_INT_MODULE_REGM),
                (icf_void_t **)(&p_buff),
                p_ecode);     

        ICF_ASSERT(
                p_crm_pdb->p_internal_msg->msg_hdr.p_glb_pdb,
                (ICF_NULL != p_buff))
        
        if (ICF_SUCCESS == ret_val)
        {
            /* Typecast into global data structure */
            p_rgm_glb_data = (icf_rgm_glb_data_st *)(p_buff);

            p_crm_pdb->p_internal_msg->p_msg_data = (icf_uint8_t *)
                (p_block->p_rgm_context);
            
            if (p_block->status == ICF_RGM_DBM_DURATION_CHANGE)
            {
                ICF_RGM_TRACE(p_crm_pdb, 
                        ICF_TRACE_REGISTRATION_FSM_TRIGGERED)
                /* Trigger FSM */
                ret_val = p_rgm_glb_data->icf_rgm_state_machine
                    [p_block->p_rgm_context->rgm_state]
                    [ICF_RGM_DURATION_CHANGE](p_crm_pdb);
            }

            else if (p_block->status == ICF_RGM_DBM_DELETE_BLOCK)
            {
                ICF_RGM_TRACE(p_crm_pdb,
                        ICF_TRACE_REGISTRATION_FSM_TRIGGERED)
                /* Trigger FSM */
                ret_val = p_rgm_glb_data->icf_rgm_state_machine
                    [p_block->p_rgm_context->rgm_state]
                    [ICF_RGM_DELETE_USER](p_crm_pdb);
            }
        }
    }
    ICF_FUNCTION_EXIT(p_glb_pdb)
    return ret_val;
}


/****************************************************************************
 * FUNCTION:            icf_rgm_modify_registrar_addr
 *
 * DESCRIPTION:         This function is called by the CFG module when it 
 *                      gets API for change of Registrar address.
 *
 *
 *                        
 * RETURNS:             
 *
 ***************************************************************************/
icf_return_t icf_rgm_modify_registrar_addr(
        INOUT icf_crm_pdb_st *p_crm_pdb)
{
    icf_glb_pdb_st *p_glb_pdb; 
    icf_error_t *p_ecode = ICF_ERROR_NONE;
    icf_config_data_st *p_dbm_config_data = ICF_NULL;
    icf_return_t ret_val;

    ICF_SET_MODULE(p_crm_pdb->p_internal_msg->msg_hdr.p_glb_pdb,
            ICF_INT_MODULE_REGM)
    
    ICF_FUNCTION_ENTER(p_crm_pdb->p_internal_msg->msg_hdr.p_glb_pdb)

    ICF_RGM_TRACE(p_crm_pdb,ICF_TRACE_MODIFY_REG_ADDRESS_RECVD)
        
    p_glb_pdb = p_crm_pdb->p_internal_msg->msg_hdr.p_glb_pdb;
    
    p_ecode = &(p_crm_pdb->p_internal_msg->msg_hdr.ecode);
    
    if (ICF_FALSE == p_crm_pdb->rgm_pdb.all_lines)
    {
        ret_val = icf_rgm_modify_reg_not_all_lines(p_crm_pdb);
    }
    
    else 
    {
        /* Request the DBM for getting pointer to config data structure */
        ret_val = icf_dbm_get_module_glb_data(
                p_glb_pdb,
                (icf_int_module_id_t)(ICF_INT_MODULE_CFG),
                (icf_void_t **)(&p_dbm_config_data),
                p_ecode);     

        ICF_ASSERT(
                p_crm_pdb->p_internal_msg->msg_hdr.p_glb_pdb,
                (ICF_NULL != p_dbm_config_data))
    
        /* Call to DBM for getting pointer to config data structure succeeded*/
        if (ICF_SUCCESS == ret_val)
        {
            ret_val = icf_rgm_modify_reg_all_lines(p_crm_pdb,
                    p_dbm_config_data->max_lines);
        }
    }
    ICF_FUNCTION_EXIT(p_glb_pdb)

    ICF_RESET_MODULE(p_crm_pdb->p_internal_msg->msg_hdr.p_glb_pdb)
    
    return ret_val;
    
}

/****************************************************************************
 * FUNCTION:            icf_rgm_modify_reg_not_all_lines
 *
 * DESCRIPTION:         This function is called by the function
 *                      icf_rgm_modify_registrar_addr when registrar
 *                      modification is not needed on all the lines.
 *
 *
 * RETURNS:             
 *
 ***************************************************************************/
icf_return_t icf_rgm_modify_reg_not_all_lines(
        INOUT icf_crm_pdb_st *p_crm_pdb)
{
    /* loop counter */
    icf_uint8_t loop_index;
    icf_uint8_t api_index;
    icf_uint8_t node_index;
    icf_boolean_t failure_flag = ICF_FALSE;
    icf_glb_pdb_st *p_glb_pdb; 
    icf_error_t *p_ecode = ICF_ERROR_NONE;
/*    icf_address_st *p_user_address;*/
    icf_line_id_t line_id;
    icf_line_data_st *p_dbm_line_data = ICF_NULL;
    icf_cfg_cc_set_registration_params_req_st *p_recv_msg_payload;
    icf_rgm_set_reg_params_node_st *p_node;
    icf_return_t ret_val = ICF_SUCCESS;
    icf_rgm_set_reg_params_not_all_lines_list_st modify_registrar_context;

    /* function entry trace */
    ICF_FUNCTION_ENTER(p_crm_pdb->p_internal_msg->msg_hdr.p_glb_pdb) 
        
    p_glb_pdb = p_crm_pdb->p_internal_msg->msg_hdr.p_glb_pdb;
    
    p_ecode = &(p_crm_pdb->p_internal_msg->msg_hdr.ecode);
    
    p_recv_msg_payload = (icf_cfg_cc_set_registration_params_req_st *)
                (p_glb_pdb->p_recv_msg->payload);
    
    /* Initialize number of elements */
    modify_registrar_context.num_of_elements = 0;

    for (loop_index = 0; loop_index < p_crm_pdb->rgm_pdb.num_of_elements; 
            loop_index++)
    {
        /* Copy the index into local context */
        modify_registrar_context.local_context[loop_index].index_in_api = 
            p_crm_pdb->rgm_pdb.api_indexes[loop_index];

        /* This variable is used for enhancing readability of code */
        api_index = modify_registrar_context.local_context[loop_index].
            index_in_api;
        
        /* Get line ID */
        line_id = p_recv_msg_payload->registration_param_list
            [api_index].line_id;

        /* Set the pointer to Node list to NULL */
        modify_registrar_context.local_context[loop_index].p_node = 
            ICF_NULL;
        
        /* Set Number of nodes field to Zero */
        modify_registrar_context.local_context[loop_index].num_of_nodes = 0;

        if (ICF_CONFIG_ACTION_ADD == p_recv_msg_payload->
                registration_param_list[api_index].action)
        {
            if(ICF_FAILURE == icf_rgm_line_self_configured(
                        p_glb_pdb,
                        line_id,
                        (icf_line_data_st **)(&p_dbm_line_data),
                        p_ecode))
            {
                failure_flag = ICF_TRUE;
                break;
            }

            /* Increment the number of blocks */
            modify_registrar_context.num_of_elements++;
            
            ret_val = icf_rgm_add_all_users(
                    p_crm_pdb,
                    p_dbm_line_data,
                    &(modify_registrar_context.local_context[loop_index]));
            
            if (ICF_FAILURE == ret_val)
            {
                /* set failure flag */
                failure_flag = ICF_TRUE;
                break;
            }
        }
        
        else if (ICF_CONFIG_ACTION_DELETE == p_recv_msg_payload->
                registration_param_list[api_index].action)
        {
            if(ICF_FAILURE == icf_rgm_line_self_configured(
                        p_glb_pdb,
                        line_id,
                        (icf_line_data_st **)(&p_dbm_line_data),
                        p_ecode))
            {
                failure_flag = ICF_TRUE;
                break;
            }
                        
            /* Increment the number of blocks */
            modify_registrar_context.num_of_elements++;
            
            ret_val = icf_rgm_delete_all_users(
                    p_crm_pdb,
                    p_dbm_line_data,
                    &(modify_registrar_context.local_context[loop_index]));
            
            if (ICF_FAILURE == ret_val)
            {
                /* set failure flag */
                failure_flag = ICF_TRUE;
                break;
            }
        }
    }/* End of for loop */

    if (ICF_TRUE == failure_flag)
    {
        ICF_RGM_SET_ECODE(p_crm_pdb,ICF_MODIFY_REGISTRAR_FAILED)

        /* Rollback */
        icf_rgm_modify_reg_roll_not_all_lines(
                p_crm_pdb,
                &modify_registrar_context);
        
        ret_val = ICF_FAILURE;
    }

    else
    {
        /* Process the context */
        /* This is the loop for traversing the context*/
        for (loop_index = 0; loop_index < modify_registrar_context.
                num_of_elements; loop_index ++)
        {
            p_node = modify_registrar_context.local_context[loop_index].p_node;

            /* This is the loop for traversing all nodes in the block */
            for (node_index = 0; node_index < modify_registrar_context.
                    local_context[loop_index].num_of_nodes; node_index++)
            {
                ICF_RGM_TRACE(p_crm_pdb, 
                        ICF_TRACE_PROCESS_MODIFY_REG_BLK)
                ret_val = icf_rgm_process_modify_registrar_node(
                        p_crm_pdb,
                        &(p_node[node_index]));
                
                if (ICF_FAILURE == ret_val)
                {
                    /* RAISE ALARM */
                    ICF_RGM_SET_ECODE(p_crm_pdb,
                            ICF_PROCESS_MODIFY_REG_BLK_FAILED)
                    
                }
            } /* End of for loop for traversing all nodes in a block */

            /* Free the memory allocated to the node */
            ICF_MEMFREE(p_glb_pdb, (icf_uint8_t *)p_node
                    , ICF_MEM_COMMON,
                    p_ecode, ret_val)
            
            /* Mark Node as Invalid */
            modify_registrar_context.local_context[loop_index].num_of_nodes
                = ICF_RGM_NODE_INVALID; 

        } /* End of for loop for traversing all blocks */
        ret_val = ICF_SUCCESS;
    }
    ICF_FUNCTION_EXIT(p_glb_pdb)
    return ret_val;
}

/****************************************************************************
 * FUNCTION:            icf_rgm_modify_reg_all_lines
 *
 * DESCRIPTION:         This function is called by the function
 *                      icf_rgm_modify_registrar_addr when registrar
 *                      modification is needed on all the lines.
 *
 *
 * RETURNS:             
 *
 ***************************************************************************/
icf_return_t icf_rgm_modify_reg_all_lines(
        INOUT icf_crm_pdb_st *p_crm_pdb,
        IN    icf_line_id_t max_lines)
{
    /* loop counter */
    icf_line_id_t loop_index = 0;
    /*icf_uint8_t api_index;*/
    icf_uint8_t node_index;
    icf_boolean_t failure_flag = ICF_FALSE;
    icf_glb_pdb_st *p_glb_pdb; 
    icf_error_t *p_ecode = ICF_ERROR_NONE;
/*    icf_address_st *p_user_address;*/
    icf_line_id_t line_id;
    icf_line_data_st *p_dbm_line_data = ICF_NULL;
    icf_cfg_cc_set_registration_params_req_st *p_recv_msg_payload;
    icf_rgm_set_reg_params_node_st *p_node;
    icf_return_t ret_val = ICF_SUCCESS;
    icf_rgm_set_reg_params_all_lines_list_st modify_registrar_context;

    /* function entry trace */
    ICF_FUNCTION_ENTER(p_crm_pdb->p_internal_msg->msg_hdr.p_glb_pdb) 
        
    p_glb_pdb = p_crm_pdb->p_internal_msg->msg_hdr.p_glb_pdb;
    
    p_ecode = &(p_crm_pdb->p_internal_msg->msg_hdr.ecode);
    
    p_recv_msg_payload = (icf_cfg_cc_set_registration_params_req_st *)
                (p_glb_pdb->p_recv_msg->payload);
    
    /* Initialize number of elements */
    modify_registrar_context.num_of_elements = 0;
        
    if (ICF_CONFIG_ACTION_ADD == p_recv_msg_payload->registration_param_list
            [0].action)
    {
        for (line_id = 0; line_id < max_lines; line_id++)
        {
            if(ICF_FAILURE == icf_rgm_line_self_configured(
                        p_glb_pdb,
                        line_id,
                        (icf_line_data_st **)(&p_dbm_line_data),
                        p_ecode))
            {
                continue;
            }
            
            else
            {
                /* Copy the line_id into local context */
                modify_registrar_context.local_context[loop_index].
                    index_in_api = line_id;
                
                /* Set the pointer to Node list to NULL */
                modify_registrar_context.local_context[loop_index].p_node = 
                    ICF_NULL;
        
                /* Set Number of nodes field to Zero */
                modify_registrar_context.local_context[loop_index].
                    num_of_nodes = 0;

                /* Increment the number of blocks */
                modify_registrar_context.num_of_elements++;
    
                ret_val = icf_rgm_add_all_users(
                        p_crm_pdb,
                        p_dbm_line_data,
                        &(modify_registrar_context.local_context[loop_index]));
            
                if (ICF_FAILURE == ret_val)
                {
                    /* set failure flag */
                    failure_flag = ICF_TRUE;
                    break;
                }
                else
                {
                    loop_index++;
                }
                
            }
        }/* End of for loop */
        
    }

    else if (ICF_CONFIG_ACTION_DELETE == p_recv_msg_payload->
            registration_param_list[0].action)
    {
        for (line_id = 0; line_id < max_lines; line_id++)
        {
            /* SPR 7552 Fix Starts */
            if (ICF_FAILURE == icf_rgm_line_self_reg_config(
                        p_glb_pdb,
                        line_id,
                        (icf_line_data_st **)(&p_dbm_line_data),
                        p_ecode))
            {
                continue;
            }
            /* SPR 7552 Fix Ends */

            else
            {
                /* Copy the line_id into local context */
                modify_registrar_context.local_context[loop_index].
                    index_in_api = line_id;
                
                /* Set the pointer to Node list to NULL */
                modify_registrar_context.local_context[loop_index].p_node = 
                    ICF_NULL;
        
                /* Set Number of nodes field to Zero */
                modify_registrar_context.local_context[loop_index].
                    num_of_nodes = 0;

                /* Increment the number of blocks */
                modify_registrar_context.num_of_elements++;
    
                ret_val = icf_rgm_delete_all_users(
                        p_crm_pdb,
                        p_dbm_line_data,
                        &(modify_registrar_context.local_context[loop_index]));
            
                if (ICF_FAILURE == ret_val)
                {
                    /* set failure flag */
                    failure_flag = ICF_TRUE;
                    break;
                }
                else
                {
                    loop_index++;
                }
                
            }
        }/* End of for loop */
    }

    if (ICF_TRUE == failure_flag)
    {
        ICF_RGM_SET_ECODE(p_crm_pdb,ICF_MODIFY_REGISTRAR_FAILED)

        /* Rollback */
        ret_val = icf_rgm_modify_reg_roll_all_lines(
                p_crm_pdb,
                &modify_registrar_context);
        
        ret_val = ICF_FAILURE;
    }

    else
    {
        /* Process the context */
        /* This is the loop for traversing the context*/
        for (loop_index = 0; loop_index < modify_registrar_context.
                num_of_elements; loop_index ++)
        {
            p_node = modify_registrar_context.local_context[loop_index].p_node;

            /* This is the loop for traversing all nodes in the block */
            for (node_index = 0; node_index < modify_registrar_context.
                    local_context[loop_index].num_of_nodes; node_index++)
            {
                ICF_RGM_TRACE(p_crm_pdb,ICF_TRACE_PROCESS_MODIFY_REG_BLK)
                ret_val = icf_rgm_process_modify_registrar_node(
                        p_crm_pdb,
                        &(p_node[node_index]));
                
                if (ICF_FAILURE == ret_val)
                {
                    /* RAISE ALARM */
                    ICF_RGM_SET_ECODE(p_crm_pdb,
                            ICF_PROCESS_MODIFY_REG_BLK_FAILED)

                }
            } /* End of for loop for traversing all nodes in a block */

            /* Free the memory allocated to the node */
            ICF_MEMFREE(p_glb_pdb, (icf_uint8_t *)p_node
                    , ICF_MEM_COMMON,
                    p_ecode, ret_val)
            
            /* Mark Node as Invalid */
            modify_registrar_context.local_context[loop_index].num_of_nodes
                = ICF_RGM_NODE_INVALID;             

        } /* End of for loop for traversing all blocks */
        if (ICF_NULL != modify_registrar_context.num_of_elements)
        {
            ret_val = ICF_SUCCESS;
        }
        else
        {
            ret_val = ICF_FAILURE;
        }
    }
    ICF_FUNCTION_EXIT(p_glb_pdb)
   return ret_val; 
}

/****************************************************************************
 * FUNCTION:            icf_rgm_add_all_users
 *
 * DESCRIPTION:         This function is called by the functions of RGM
 *                      module in order to add all users configured on a
 *                      particular line to registration blocks.
 *
 * RETURNS:             
 *
 ***************************************************************************/
icf_return_t icf_rgm_add_all_users(
        INOUT icf_crm_pdb_st *p_crm_pdb,
        IN    icf_line_data_st *p_dbm_line_data,
        INOUT icf_rgm_set_reg_params_block_st *p_block)
{
    icf_uint8_t user_addr_list_index;
    icf_return_t ret_val = ICF_SUCCESS;
    icf_uint16_t node_list_size;
    icf_uint8_t  *p_list = ICF_NULL;

    icf_glb_pdb_st *p_glb_pdb;
    icf_error_t *p_ecode = ICF_ERROR_NONE;
    
    /* function entry trace */
    ICF_FUNCTION_ENTER(p_crm_pdb->p_internal_msg->msg_hdr.p_glb_pdb) 
        
    p_glb_pdb = p_crm_pdb->p_internal_msg->msg_hdr.p_glb_pdb;
    
    p_ecode = &(p_crm_pdb->p_internal_msg->msg_hdr.ecode);    
   
    
    p_block->num_of_nodes = ICF_RGM_NODE_INVALID;

    node_list_size = (ICF_PORT_SIZEOF(icf_rgm_set_reg_params_node_st))
        * (p_dbm_line_data->num_of_user_addr); 

    /* Allocate memory for the Nodes */
    ICF_MEMGET(p_glb_pdb, node_list_size, ICF_MEM_COMMON, p_list, 
            ICF_RET_ON_FAILURE, p_ecode, ret_val)

    if (ICF_SUCCESS == ret_val)
    {
        p_block->p_node = (icf_rgm_set_reg_params_node_st *)p_list;

        /* Set number of nodes to Zero */
        p_block->num_of_nodes = ICF_NULL;

        /* This is loop for traversing all users on a line */
        for (user_addr_list_index = 0; 
                user_addr_list_index < p_dbm_line_data->num_of_user_addr; 
                user_addr_list_index++)
        {
            /* Copy the user address from line data to node */
            p_block->p_node[user_addr_list_index].p_user_address =
                &(p_dbm_line_data->p_user_addr_list[user_addr_list_index]);
            
            /* Set the pointer to registration context to NULL */
            p_block->p_node[user_addr_list_index].p_rgm_context = 
                ICF_NULL;

            /* Set Status field to default */
            p_block->p_node[user_addr_list_index].status = ICF_NULL;

            ret_val = icf_rgm_add_user(
                    p_crm_pdb,
                    p_dbm_line_data->line_id,
                    &(p_dbm_line_data->p_user_addr_list[user_addr_list_index]),
                    &(p_block->p_node[user_addr_list_index].status),
                    &(p_block->p_node[user_addr_list_index].p_rgm_context));

            if (ICF_FAILURE == ret_val)
            {
                break;
            }

            else 
            {
                p_block->num_of_nodes++;
            }

        }

    }
    ICF_FUNCTION_EXIT(p_glb_pdb)
    return ret_val;
}
        
/****************************************************************************
 * FUNCTION:            icf_rgm_delete_all_users
 *
 * DESCRIPTION:         This function is called by the functions of RGM
 *                      module in order to delete all users configured on a
 *                      particular line to registration blocks.
 *
 * RETURNS:             
 *
 ***************************************************************************/
icf_return_t icf_rgm_delete_all_users(
        INOUT icf_crm_pdb_st *p_crm_pdb,
        IN    icf_line_data_st *p_dbm_line_data,
        INOUT icf_rgm_set_reg_params_block_st *p_block)
{
    icf_uint8_t user_addr_list_index;
    icf_return_t ret_val = ICF_SUCCESS;
    icf_uint16_t node_list_size;
    icf_uint8_t  *p_list = ICF_NULL;

    icf_glb_pdb_st *p_glb_pdb;
    icf_error_t *p_ecode = ICF_ERROR_NONE;
    
    /* function entry trace */
    ICF_FUNCTION_ENTER(p_crm_pdb->p_internal_msg->msg_hdr.p_glb_pdb) 
        
    p_glb_pdb = p_crm_pdb->p_internal_msg->msg_hdr.p_glb_pdb;
    
    p_ecode = &(p_crm_pdb->p_internal_msg->msg_hdr.ecode);
    
    
    p_block->num_of_nodes = ICF_RGM_NODE_INVALID;

    node_list_size = (ICF_PORT_SIZEOF(icf_rgm_set_reg_params_node_st))
        * (p_dbm_line_data->num_of_user_addr);

    /* Allocate memory for the Nodes */
    ICF_MEMGET(p_glb_pdb, node_list_size, ICF_MEM_COMMON, p_list, 
            ICF_RET_ON_FAILURE, p_ecode, ret_val)

    if(ICF_SUCCESS == ret_val)
    {
        p_block->p_node = (icf_rgm_set_reg_params_node_st *)p_list;

        /* Set number of nodes to Zero */
        p_block->num_of_nodes = ICF_NULL;

        /* This is loop for traversing all users on a line */
        for (user_addr_list_index = 0; 
                user_addr_list_index < p_dbm_line_data->num_of_user_addr; 
                user_addr_list_index++)
        {
            /* Copy the user address from line data to node */
            p_block->p_node[user_addr_list_index].p_user_address =
                &(p_dbm_line_data->p_user_addr_list[user_addr_list_index]);

            /* Set the pointer to registration context to NULL */
            p_block->p_node[user_addr_list_index].p_rgm_context = 
                ICF_NULL;

            /* Set Status field to default */
            p_block->p_node[user_addr_list_index].status = ICF_NULL;

            ret_val = icf_rgm_delete_user(
                    p_crm_pdb,
                    p_dbm_line_data->line_id,
                    &(p_dbm_line_data->p_user_addr_list[user_addr_list_index]),
                    &(p_block->p_node[user_addr_list_index].status),
                    &(p_block->p_node[user_addr_list_index].p_rgm_context));

            if (ICF_FAILURE == ret_val)
            {
                break;
            }

            else 
            {
                p_block->num_of_nodes++;
            }

        }

    }
    ICF_FUNCTION_EXIT(p_glb_pdb)
    return ret_val;
}

/****************************************************************************
 * FUNCTION:            icf_rgm_modify_reg_roll_not_all_lines
 *
 * DESCRIPTION:         This function is called by the functions of RGM module
 *                      to carry out rollback processing in case set  
 *                      registration params API in which all_lines field is set
 *                      to FALSE.
 *
 *
 * RETURNS:             
 *
 ***************************************************************************/
icf_return_t icf_rgm_modify_reg_roll_not_all_lines(
        INOUT icf_crm_pdb_st *p_crm_pdb,
        INOUT icf_rgm_set_reg_params_not_all_lines_list_st 
        *p_modify_registrar_context)
{
    icf_int16_t loop_index; /* This should be signed int */
    icf_uint8_t api_index;
    icf_int16_t node_index; /* This should be signed int */
    icf_glb_pdb_st *p_glb_pdb; 
    icf_error_t *p_ecode = ICF_ERROR_NONE;
/*    icf_address_st *p_user_address;*/
    icf_line_id_t line_id;
    icf_cfg_cc_set_registration_params_req_st *p_recv_msg_payload;
    icf_rgm_set_reg_params_node_st *p_node;
    icf_return_t ret_val;

    ICF_FUNCTION_ENTER(p_crm_pdb->p_internal_msg->msg_hdr.p_glb_pdb)
    
    ICF_RGM_TRACE(p_crm_pdb, ICF_TRACE_MODIFY_REG_ROLLBACK)
        
    p_glb_pdb = p_crm_pdb->p_internal_msg->msg_hdr.p_glb_pdb;
    
    p_ecode = &(p_crm_pdb->p_internal_msg->msg_hdr.ecode);
    
    p_recv_msg_payload = (icf_cfg_cc_set_registration_params_req_st *)
                (p_glb_pdb->p_recv_msg->payload);

    for (loop_index = p_modify_registrar_context->num_of_elements - 1;
            loop_index >= 0; loop_index--)
    {
        api_index = p_modify_registrar_context->local_context[loop_index].
            index_in_api;

        /* Get pointer to user address */
/*        p_user_address = &(p_recv_msg_payload->registrar_addr_list.
                transport_addr[api_index].addr);*/
        
        /* Get line ID */
        line_id = p_recv_msg_payload->registration_param_list
            [api_index].line_id;
        
        if (ICF_RGM_NODE_INVALID == p_modify_registrar_context->
                local_context[loop_index].num_of_nodes)
        {
            /* This node need not be processed */
            continue;
        }
        
        p_node = p_modify_registrar_context->local_context[loop_index].p_node;
        
        if (ICF_CONFIG_ACTION_ADD == p_recv_msg_payload->
                registration_param_list[api_index].action)
        {
            /* This is loop for traversing all nodes of a block in reverse
             * order
             */
            for (node_index = p_modify_registrar_context->local_context
                    [loop_index].num_of_nodes - 1; node_index >= 0; 
                    node_index--)
            {
                if (p_node[node_index].status == ICF_RGM_DBM_NEW_BLOCK)
                {
                    /* Deallocate the block */
                    ret_val = icf_rgm_clear_reg_context(
                            p_crm_pdb,
                            p_node[node_index].p_rgm_context);
                
                }

                else if((p_node[node_index].status == ICF_RGM_DBM_NO_ACTION)
                        || (p_node[node_index].status == 
                            ICF_RGM_DBM_DURATION_CHANGE))
                {
                    ret_val = icf_dbm_regm_delete_user(
                            p_glb_pdb,
                            line_id,
                            p_node[node_index].p_user_address,
                            &(p_node[node_index].status),
                            &(p_node[node_index].p_rgm_context),
                            p_ecode);
                }
                
            }/* End of for loop for traversing nodes of a block*/
        }
        
        else if (ICF_CONFIG_ACTION_DELETE == p_recv_msg_payload->
                registration_param_list[api_index].action)
        {
            for (node_index = p_modify_registrar_context->local_context
                    [loop_index].num_of_nodes - 1; node_index >= 0; 
                    node_index--)
            {
                if (p_node[node_index].status == ICF_RGM_DBM_DELETE_BLOCK)
                {
                    /* Do nothing */
                }

                else
                {
                    ret_val = icf_dbm_regm_add_user(
                            p_glb_pdb,
                            line_id,
                            p_node[node_index].p_user_address,
                            &(p_node[node_index].status),
                            &(p_node[node_index].p_rgm_context),
                            p_ecode);
                }
                
            }/* End of for loop for traversing nodes of a block*/
        }

        /* Free the memory allocated to the node */
        ICF_MEMFREE(p_glb_pdb, (icf_uint8_t *)p_node, ICF_MEM_COMMON,
                p_ecode, ret_val)
        /* Mark Node as Invalid */
        p_modify_registrar_context->local_context[loop_index].num_of_nodes = 
            ICF_RGM_NODE_INVALID;     

    }/* End of for loop for traversing all blocks of a context */
    ret_val = ICF_SUCCESS;
    ICF_FUNCTION_EXIT(p_glb_pdb)
    return ret_val;
}


/****************************************************************************
 * FUNCTION:            icf_rgm_modify_reg_roll_all_lines
 *
 * DESCRIPTION:         This function is called by the functions of RGM module
 *                      to carry out rollback processing in case set  
 *                      registration params API in which all_lines field is set
 *                      to TRUE.
 *
 *
 * RETURNS:             
 *
 ***************************************************************************/
icf_return_t icf_rgm_modify_reg_roll_all_lines(
        INOUT icf_crm_pdb_st *p_crm_pdb,
        INOUT icf_rgm_set_reg_params_all_lines_list_st 
        *p_modify_registrar_context)
{
    icf_int16_t loop_index; /* This should be signed int */
/*   icf_uint8_t api_index;*/
    icf_int16_t node_index; /* This should be signed int */
    icf_glb_pdb_st *p_glb_pdb; 
    icf_error_t *p_ecode = ICF_ERROR_NONE;
/*    icf_address_st *p_user_address;*/
    icf_line_id_t line_id;
    icf_cfg_cc_set_registration_params_req_st *p_recv_msg_payload;
    icf_rgm_set_reg_params_node_st *p_node;
    icf_return_t ret_val;
    
    ICF_FUNCTION_ENTER(p_crm_pdb->p_internal_msg->msg_hdr.p_glb_pdb)
    
    ICF_RGM_TRACE(p_crm_pdb, ICF_TRACE_MODIFY_REG_ROLLBACK)
        
    p_glb_pdb = p_crm_pdb->p_internal_msg->msg_hdr.p_glb_pdb;
    
    p_ecode = &(p_crm_pdb->p_internal_msg->msg_hdr.ecode);
    
    p_recv_msg_payload = (icf_cfg_cc_set_registration_params_req_st *)
                (p_glb_pdb->p_recv_msg->payload);
    
    if (ICF_CONFIG_ACTION_ADD == p_recv_msg_payload->
            registration_param_list[0].action)
    {
        for (loop_index = p_modify_registrar_context->num_of_elements - 1;
                loop_index >= 0; loop_index--)
        {
            if (ICF_RGM_NODE_INVALID == p_modify_registrar_context->
                    local_context[loop_index].num_of_nodes)
            {
                /* This node need not be processed */
                continue;
            }

            /* Get line ID */
            line_id = p_modify_registrar_context->local_context[loop_index].
                index_in_api;
        
            p_node = p_modify_registrar_context->local_context[loop_index].
                p_node;

            /* This is loop for traversing all nodes of a block in reverse
             * order
             */
            for (node_index = p_modify_registrar_context->local_context
                    [loop_index].num_of_nodes - 1; node_index >= 0; 
                    node_index--)
            {
                if (p_node[node_index].status == ICF_RGM_DBM_NEW_BLOCK)
                {
                    /* Deallocate the block */
                    ret_val = icf_rgm_clear_reg_context(
                            p_crm_pdb,
                            p_node[node_index].p_rgm_context);
                
                }

                else if((p_node[node_index].status == ICF_RGM_DBM_NO_ACTION)
                        || (p_node[node_index].status == 
                            ICF_RGM_DBM_DURATION_CHANGE))
                {
                    ret_val = icf_dbm_regm_delete_user(
                            p_glb_pdb,
                            line_id,
                            p_node[node_index].p_user_address,
                            &(p_node[node_index].status),
                            &(p_node[node_index].p_rgm_context),
                            p_ecode);
                }
                
            }/* End of for loop for traversing nodes of a block*/

        /* Free the memory allocated to the node */
        ICF_MEMFREE(p_glb_pdb, (icf_uint8_t *)p_node, ICF_MEM_COMMON,
                p_ecode, ret_val)
        
            /* Mark Node as Invalid */
        p_modify_registrar_context->local_context[loop_index].num_of_nodes = 
            ICF_RGM_NODE_INVALID;     
            
        }/* End of for loop for traversing all blocks in reverse order */
    }

    else if (ICF_CONFIG_ACTION_DELETE == p_recv_msg_payload->
            registration_param_list[0].action)
    {
        for (loop_index = p_modify_registrar_context->num_of_elements - 1;
                loop_index >= 0; loop_index--)
        {
            if (ICF_RGM_NODE_INVALID == p_modify_registrar_context->
                    local_context[loop_index].num_of_nodes)
            {
                /* This node need not be processed */
                continue;
            }

            /* Get line ID */
            line_id = p_modify_registrar_context->local_context[loop_index].
                index_in_api;
        
            p_node = p_modify_registrar_context->local_context[loop_index].
                p_node;

            /* This is loop for traversing all nodes of a block in reverse
             * order
             */
            for (node_index = p_modify_registrar_context->local_context
                    [loop_index].num_of_nodes - 1; node_index >= 0; 
                    node_index--)
            {
                if (p_node[node_index].status == ICF_RGM_DBM_DELETE_BLOCK)
                {
                    /* Do nothing */
                }

                else
                {
                    ret_val = icf_dbm_regm_add_user(
                            p_glb_pdb,
                            line_id,
                            p_node[node_index].p_user_address,
                            &(p_node[node_index].status),
                            &(p_node[node_index].p_rgm_context),
                            p_ecode);
                }
                
            }
            
        /* Free the memory allocated to the node */
        ICF_MEMFREE(p_glb_pdb, (icf_uint8_t *)p_node, ICF_MEM_COMMON,
                p_ecode, ret_val)
        
        /* Mark Node as Invalid */
        p_modify_registrar_context->local_context[loop_index].num_of_nodes = 
            ICF_RGM_NODE_INVALID;     
            
        }/* End of for loop for traversing all blocks in reverse order */
        
    }
    ret_val = ICF_SUCCESS;
    ICF_FUNCTION_EXIT(p_glb_pdb)
    return ret_val;
}
    
/****************************************************************************
 * FUNCTION:            icf_rgm_process_modify_registrar_node
 *
 * DESCRIPTION:         This function is called by the functions of RGM module
 *                      after all blocks have been prepared to process a
 *                      node in a block according to its state.
 *
 *
 * RETURNS:             
 *
 ***************************************************************************/
icf_return_t icf_rgm_process_modify_registrar_node(
        INOUT icf_crm_pdb_st *p_crm_pdb,
        INOUT icf_rgm_set_reg_params_node_st *p_node)
{
    icf_glb_pdb_st *p_glb_pdb; 
    icf_error_t *p_ecode = ICF_ERROR_NONE;
    icf_void_t *p_buff = ICF_NULL;
    icf_rgm_glb_data_st *p_rgm_glb_data;
    icf_return_t ret_val;

    ICF_FUNCTION_ENTER(p_crm_pdb->p_internal_msg->msg_hdr.p_glb_pdb)
 
    p_glb_pdb = p_crm_pdb->p_internal_msg->msg_hdr.p_glb_pdb;
    
    p_ecode = &(p_crm_pdb->p_internal_msg->msg_hdr.ecode);
    
    
    if (p_node->status == ICF_RGM_DBM_NEW_BLOCK)
    {
        /* Send registration requests */
        ret_val = icf_rgm_register_user(
                p_crm_pdb,
                p_node->p_rgm_context);
       
    }

    else if (p_node->status == ICF_RGM_DBM_NO_ACTION)
    {
        /* Do nothing */
        ret_val = ICF_SUCCESS;
    }
    
    else 
    {
        /* Load FSM */
        
        /* Call DBM to get the pointer to the RGM global data in 
         * p_buff
         */
        ret_val = icf_dbm_get_module_glb_data(
                p_glb_pdb,
                (icf_int_module_id_t)(ICF_INT_MODULE_REGM),
                (icf_void_t **)(&p_buff),
                p_ecode);     

        ICF_ASSERT(
                p_crm_pdb->p_internal_msg->msg_hdr.p_glb_pdb,
                (ICF_NULL != p_buff))
        
        if (ICF_SUCCESS == ret_val)
        {
            /* Typecast into global data structure */
            p_rgm_glb_data = (icf_rgm_glb_data_st *)(p_buff);

            p_crm_pdb->p_internal_msg->p_msg_data = (icf_uint8_t *)
                (p_node->p_rgm_context);
            
            if (p_node->status == ICF_RGM_DBM_DURATION_CHANGE)
            {
                ICF_RGM_TRACE(p_crm_pdb,
                        ICF_TRACE_REGISTRATION_FSM_TRIGGERED)
                /* Trigger FSM */
                ret_val = p_rgm_glb_data->icf_rgm_state_machine
                    [p_node->p_rgm_context->rgm_state]
                    [ICF_RGM_DURATION_CHANGE](p_crm_pdb);
            }

            else if (p_node->status == ICF_RGM_DBM_DELETE_BLOCK)
            {
                ICF_RGM_TRACE(p_crm_pdb,
                        ICF_TRACE_REGISTRATION_FSM_TRIGGERED)
                /* Trigger FSM */
                ret_val = p_rgm_glb_data->icf_rgm_state_machine
                    [p_node->p_rgm_context->rgm_state]
                    [ICF_RGM_DELETE_USER](p_crm_pdb);
            }
        }
    }
    ICF_FUNCTION_EXIT(p_glb_pdb)
    return ret_val;
}

/****************************************************************************
 * FUNCTION:            icf_rgm_modify_duration
 *
 * DESCRIPTION:         This function is called by the CFG module when it 
 *                      gets API for modification of Registrar Duration.
 *
 *
 *                        
 * RETURNS:             
 *
 ***************************************************************************/
icf_return_t icf_rgm_modify_duration(
        INOUT icf_crm_pdb_st *p_crm_pdb)
{
    icf_glb_pdb_st *p_glb_pdb; 
    icf_error_t *p_ecode = ICF_ERROR_NONE;
    icf_config_data_st *p_dbm_config_data = ICF_NULL;
    icf_return_t ret_val;

    ICF_SET_MODULE(p_crm_pdb->p_internal_msg->msg_hdr.p_glb_pdb,
            ICF_INT_MODULE_REGM)
    
    ICF_FUNCTION_ENTER(p_crm_pdb->p_internal_msg->msg_hdr.p_glb_pdb)

    ICF_RGM_TRACE(p_crm_pdb,ICF_TRACE_MODIFY_DURATION_RECVD)
        
    p_glb_pdb = p_crm_pdb->p_internal_msg->msg_hdr.p_glb_pdb;
    
    p_ecode = &(p_crm_pdb->p_internal_msg->msg_hdr.ecode);
    
    if (ICF_FALSE == p_crm_pdb->rgm_pdb.all_lines)
    {
        ret_val = icf_rgm_modify_dur_not_all_lines(p_crm_pdb);
    }
    
    else 
    {
        /* Request the DBM for getting pointer to config data structure */
        ret_val = icf_dbm_get_module_glb_data(
                p_glb_pdb,
                (icf_int_module_id_t)(ICF_INT_MODULE_CFG),
                (icf_void_t **)(&p_dbm_config_data),
                p_ecode);     

        ICF_ASSERT(
                p_crm_pdb->p_internal_msg->msg_hdr.p_glb_pdb,
                (ICF_NULL != p_dbm_config_data))
    
        /* Call to DBM for getting pointer to config data structure succeeded*/
        if (ICF_SUCCESS == ret_val)
        {
            ret_val = icf_rgm_modify_dur_all_lines(p_crm_pdb,
                    p_dbm_config_data->max_lines);
        }
    }
    ICF_FUNCTION_EXIT(p_glb_pdb)

    ICF_RESET_MODULE(p_crm_pdb->p_internal_msg->msg_hdr.p_glb_pdb)
    
    return ret_val;
    
}

/****************************************************************************
 * FUNCTION:            icf_rgm_modify_dur_not_all_lines
 *
 * DESCRIPTION:         This function is called by the function
 *                      icf_rgm_modify_duration when registration duration
 *                      modification is not needed on all the lines.
 *
 *
 * RETURNS:             
 *
 ***************************************************************************/
icf_return_t icf_rgm_modify_dur_not_all_lines(
        INOUT icf_crm_pdb_st *p_crm_pdb)
{
    /* loop counter */
    icf_uint8_t loop_index;
    icf_uint8_t api_index;
    icf_uint8_t node_index;
    icf_boolean_t failure_flag = ICF_FALSE;
    icf_glb_pdb_st *p_glb_pdb; 
    icf_error_t *p_ecode = ICF_ERROR_NONE;
/*    icf_address_st *p_user_address;*/
    icf_line_id_t line_id;
    icf_line_data_st *p_dbm_line_data = ICF_NULL;
    icf_cfg_cc_set_registration_params_req_st *p_recv_msg_payload;
    icf_rgm_set_reg_params_node_st *p_node;
    icf_return_t ret_val = ICF_SUCCESS;
    icf_rgm_set_reg_params_not_all_lines_list_st modify_duration_context;

    /* function entry trace */
    ICF_FUNCTION_ENTER(p_crm_pdb->p_internal_msg->msg_hdr.p_glb_pdb) 
        
    p_glb_pdb = p_crm_pdb->p_internal_msg->msg_hdr.p_glb_pdb;
    
    p_ecode = &(p_crm_pdb->p_internal_msg->msg_hdr.ecode);
    
    p_recv_msg_payload = (icf_cfg_cc_set_registration_params_req_st *)
                (p_glb_pdb->p_recv_msg->payload);
    
    /* Initialize number of elements */
    modify_duration_context.num_of_elements = 0;

    for (loop_index = 0; loop_index < p_crm_pdb->rgm_pdb.num_of_elements; 
            loop_index++)
    {
        /* Copy the index into local context */
        modify_duration_context.local_context[loop_index].index_in_api = 
            p_crm_pdb->rgm_pdb.api_indexes[loop_index];

        /* This variable is used for enhancing readability of code */
        api_index = modify_duration_context.local_context[loop_index].
            index_in_api;
        
        /* Get line ID */
        line_id = p_recv_msg_payload->registration_param_list
            [api_index].line_id;

        /* Set the pointer to Node list to NULL */
        modify_duration_context.local_context[loop_index].p_node = 
            ICF_NULL;
        
        /* Set Number of nodes field to Zero */
        modify_duration_context.local_context[loop_index].num_of_nodes = 0;

        if (ICF_CONFIG_ACTION_MODIFY == p_recv_msg_payload->
                registration_param_list[api_index].action)
        {
            if(ICF_FAILURE == icf_rgm_line_self_reg_config(
                        p_glb_pdb,
                        line_id,
                        (icf_line_data_st **)(&p_dbm_line_data),
                        p_ecode))
            {
                failure_flag = ICF_TRUE;
                break;
            }
            
            /* Increment the number of blocks */
            modify_duration_context.num_of_elements++;
            
            ret_val = icf_rgm_dur_change_all_users(
                    p_crm_pdb,
                    p_recv_msg_payload->registration_param_list[api_index].
                    registration_duration,
                    p_dbm_line_data,
                    &(modify_duration_context.local_context[loop_index]));
            
            if (ICF_FAILURE == ret_val)
            {
                /* set failure flag */
                failure_flag = ICF_TRUE;
                break;
            }
        }
        
        else
        {
            /* Set Failure flag */
            failure_flag = ICF_TRUE;
            break;
        }
    } /* End of for loop */
    
    if (ICF_TRUE == failure_flag)
    {
        ICF_RGM_SET_ECODE(p_crm_pdb,ICF_MODIFY_DURATION_FAILED)

        /* Rollback */
        icf_rgm_modify_dur_roll_not_all_lines(
                p_crm_pdb,
                &modify_duration_context);
        
        ret_val = ICF_FAILURE;
    }

    else
    {
        /* Process the context */
        /* This is the loop for traversing the context*/
        for (loop_index = 0; loop_index < modify_duration_context.
                num_of_elements; loop_index ++)
        {
            p_node = modify_duration_context.local_context[loop_index].p_node;

            /* This is the loop for traversing all nodes in the block */
            for (node_index = 0; node_index < modify_duration_context.
                    local_context[loop_index].num_of_nodes; node_index++)
            {
                ICF_RGM_TRACE(p_crm_pdb,ICF_TRACE_PROCESS_MODIFY_DUR_BLK)
                ret_val = icf_rgm_process_modify_registrar_node(
                        p_crm_pdb,
                        &(p_node[node_index]));
                
                if (ICF_FAILURE == ret_val)
                {
                    /* RAISE ALARM */
                    ICF_RGM_SET_ECODE(p_crm_pdb,
                            ICF_PROCESS_MODIFY_DUR_BLK_FAILED)
                }
            } /* End of for loop for traversing all nodes in a block */
            
            /* Free the memory allocated to the node */
            ICF_MEMFREE(p_glb_pdb, (icf_uint8_t *)p_node
                    , ICF_MEM_COMMON,
                    p_ecode, ret_val)
            
            /* Mark Node as Invalid */
            modify_duration_context.local_context[loop_index].num_of_nodes
                = ICF_RGM_NODE_INVALID; 
        } /* End of for loop for traversing all blocks */
        ret_val = ICF_SUCCESS;
    }
    ICF_FUNCTION_EXIT(p_glb_pdb)
    return ret_val;
}

/****************************************************************************
 * FUNCTION:            icf_rgm_modify_dur_all_lines
 *
 * DESCRIPTION:         This function is called by the function
 *                      icf_rgm_modify_duration when registration duration
 *                      modification is needed on all the lines.
 *
 *
 * RETURNS:             
 *
 ***************************************************************************/
icf_return_t icf_rgm_modify_dur_all_lines(
        INOUT icf_crm_pdb_st *p_crm_pdb,
        IN    icf_line_id_t max_lines)
{
    /* loop counter */
    icf_line_id_t loop_index = 0;
    /*icf_uint8_t api_index;*/
    icf_uint8_t node_index;
    icf_boolean_t failure_flag = ICF_FALSE;
    icf_glb_pdb_st *p_glb_pdb; 
    icf_error_t *p_ecode = ICF_ERROR_NONE;
/*    icf_address_st *p_user_address;*/
    icf_line_id_t line_id;
    icf_line_data_st *p_dbm_line_data = ICF_NULL;
    icf_cfg_cc_set_registration_params_req_st *p_recv_msg_payload;
    icf_rgm_set_reg_params_node_st *p_node;
    icf_return_t ret_val = ICF_SUCCESS;
    icf_rgm_set_reg_params_all_lines_list_st modify_duration_context;

    /* function entry trace */
    ICF_FUNCTION_ENTER(p_crm_pdb->p_internal_msg->msg_hdr.p_glb_pdb) 
        
    p_glb_pdb = p_crm_pdb->p_internal_msg->msg_hdr.p_glb_pdb;
    
    p_ecode = &(p_crm_pdb->p_internal_msg->msg_hdr.ecode);
    
    p_recv_msg_payload = (icf_cfg_cc_set_registration_params_req_st *)
                (p_glb_pdb->p_recv_msg->payload);
    
    /* Initialize number of elements */
    modify_duration_context.num_of_elements = 0;
        
    for (line_id = 0; line_id < max_lines; line_id++)
    {
        if(ICF_FAILURE == icf_rgm_line_self_configured(
                    p_glb_pdb,
                    line_id,
                    (icf_line_data_st **)(&p_dbm_line_data),
                    p_ecode))
        {
            continue;
        }

        else
        {
            /* Copy the line_id into local context */
            modify_duration_context.local_context[loop_index].
                index_in_api = line_id;

            /* Set the pointer to Node list to NULL */
            modify_duration_context.local_context[loop_index].p_node = 
                ICF_NULL;

            /* Set Number of nodes field to Zero */
            modify_duration_context.local_context[loop_index].
                num_of_nodes = ICF_NULL;

            /* Increment the number of blocks */
            modify_duration_context.num_of_elements++;

            ret_val = icf_rgm_dur_change_all_users(
                    p_crm_pdb,
                    p_recv_msg_payload->registration_param_list[0].
                    registration_duration,
                    p_dbm_line_data,
                    &(modify_duration_context.local_context[loop_index]));

            if (ICF_FAILURE == ret_val)
            {
                /* set failure flag */
                failure_flag = ICF_TRUE;
                break;
            }
            else
            {
                loop_index++;
            }

        }
    }/* End of for loop */
    if (ICF_TRUE == failure_flag)
    {
        ICF_RGM_SET_ECODE(p_crm_pdb,ICF_MODIFY_DURATION_FAILED)

        /* Rollback */
        icf_rgm_modify_dur_roll_all_lines(
                p_crm_pdb,
                &modify_duration_context); 
        ret_val = ICF_FAILURE;
    }

    else
    {
        /* Process the context */
        /* This is the loop for traversing the context*/
        for (loop_index = 0; loop_index < modify_duration_context.
                num_of_elements; loop_index ++)
        {
            p_node = modify_duration_context.local_context[loop_index].p_node;

            /* This is the loop for traversing all nodes in the block */
            for (node_index = 0; node_index < modify_duration_context.
                    local_context[loop_index].num_of_nodes; node_index++)
            {
                ICF_RGM_TRACE(p_crm_pdb,ICF_TRACE_PROCESS_MODIFY_DUR_BLK)
                ret_val = icf_rgm_process_modify_registrar_node(
                        p_crm_pdb,
                        &(p_node[node_index]));
                
                if (ICF_FAILURE == ret_val)
                {
                    /* RAISE ALARM */
                    ICF_RGM_SET_ECODE(p_crm_pdb,
                            ICF_PROCESS_MODIFY_DUR_BLK_FAILED)
                }
            } /* End of for loop for traversing all nodes in a block */

            /* Free the memory allocated to the node */
            ICF_MEMFREE(p_glb_pdb, (icf_uint8_t *)p_node
                    , ICF_MEM_COMMON,
                    p_ecode, ret_val)
            
            /* Mark Node as Invalid */
            modify_duration_context.local_context[loop_index].num_of_nodes
                = ICF_RGM_NODE_INVALID; 
            
        } /* End of for loop for traversing all blocks */
        if (ICF_NULL != modify_duration_context.num_of_elements)
        {
            ret_val = ICF_SUCCESS;
        }
        else
        {
            ret_val = ICF_FAILURE;
        }
    }
    ICF_FUNCTION_EXIT(p_glb_pdb)
    return ret_val;
}
        

/****************************************************************************
 * FUNCTION:            icf_rgm_dur_change_all_users
 *
 * DESCRIPTION:         This function is called by the functions of RGM
 *                      module in order to change duration of all users
 *                      configured on a particular line in registration blocks.
 *
 * RETURNS:             
 *
 ***************************************************************************/
icf_return_t icf_rgm_dur_change_all_users(
        INOUT icf_crm_pdb_st *p_crm_pdb,
        IN    icf_duration_t new_duration,
        INOUT icf_line_data_st *p_dbm_line_data,
        INOUT icf_rgm_set_reg_params_block_st *p_block)

{
    icf_uint8_t user_addr_list_index;
    icf_return_t ret_val = ICF_SUCCESS;
    icf_uint16_t node_list_size;
    icf_uint8_t  *p_list = ICF_NULL;

    icf_glb_pdb_st *p_glb_pdb;
    icf_error_t *p_ecode = ICF_ERROR_NONE;
    
    /* function entry trace */
    ICF_FUNCTION_ENTER(p_crm_pdb->p_internal_msg->msg_hdr.p_glb_pdb) 
        
    p_glb_pdb = p_crm_pdb->p_internal_msg->msg_hdr.p_glb_pdb;
    
    p_ecode = &(p_crm_pdb->p_internal_msg->msg_hdr.ecode);
    
    /* Start Preparing Nodes */
    p_block->num_of_nodes = ICF_RGM_NODE_INVALID;

    node_list_size = (ICF_PORT_SIZEOF(icf_rgm_set_reg_params_node_st))
        * (p_dbm_line_data->num_of_user_addr); 

    /* Allocate memory for the Nodes */
    ICF_MEMGET(p_glb_pdb, node_list_size, ICF_MEM_COMMON, p_list, 
            ICF_RET_ON_FAILURE, p_ecode, ret_val)

    if(ICF_SUCCESS == ret_val)
    {
        p_block->p_node = (icf_rgm_set_reg_params_node_st *)p_list;

        /* Set number of nodes to Zero */
        p_block->num_of_nodes = ICF_NULL;

        /* This is loop for traversing all users on a line */
        for (user_addr_list_index = 0; 
                user_addr_list_index < p_dbm_line_data->num_of_user_addr; 
                user_addr_list_index++)
        {
            /* Copy the user address from line data to node */
            p_block->p_node[user_addr_list_index].p_user_address =
                &(p_dbm_line_data->p_user_addr_list[user_addr_list_index]);            
            /* Set the pointer to registration context to NULL */
            p_block->p_node[user_addr_list_index].p_rgm_context = 
                ICF_NULL;

            /* Set Status field to default */
            p_block->p_node[user_addr_list_index].status = ICF_NULL;

            ret_val = icf_rgm_dur_change_user(
                    p_crm_pdb,
                    new_duration,
                    &(p_dbm_line_data->p_user_addr_list[user_addr_list_index]),
                    p_dbm_line_data,
                    &(p_block->p_node[user_addr_list_index].status),
                    &(p_block->p_node[user_addr_list_index].p_rgm_context));

            if (ICF_FAILURE == ret_val)
            {
                break;
            }

            else 
            {
                p_block->num_of_nodes++;
            }

        }

    }
    ICF_FUNCTION_EXIT(p_glb_pdb)
    return ret_val;
}

/****************************************************************************
 * FUNCTION:            icf_rgm_dur_change_user
 *
 * DESCRIPTION:         This function is called by the functions of RGM
 *                      module in order to change duration of a user
 *                      configured on a particular line in registration blocks.
 *
 * RETURNS:             
 *
 ***************************************************************************/
icf_return_t icf_rgm_dur_change_user(
        INOUT icf_crm_pdb_st *p_crm_pdb,
        IN    icf_duration_t new_duration,
        IN    icf_address_st *p_user_address,
        INOUT icf_line_data_st *p_dbm_line_data,
        OUT   icf_uint8_t *p_status,
        OUT   icf_rgm_context_st **p_p_rgm_context)
{
    icf_line_id_t line_id = p_dbm_line_data->line_id;
    icf_duration_t old_line_duration;
    icf_duration_t old_reg_duration;
    icf_return_t ret_val;
  
    icf_glb_pdb_st *p_glb_pdb;
    icf_error_t *p_ecode = ICF_ERROR_NONE;
    
    /* function entry trace */
    ICF_FUNCTION_ENTER(p_crm_pdb->p_internal_msg->msg_hdr.p_glb_pdb) 
        
    p_glb_pdb = p_crm_pdb->p_internal_msg->msg_hdr.p_glb_pdb;
    
    p_ecode = &(p_crm_pdb->p_internal_msg->msg_hdr.ecode);    
    
    
    /* Fetch user id line id pair from DBM */
    ret_val = icf_dbm_fetch_rgn_blk(
            p_glb_pdb,
            p_user_address,
            line_id,
            p_p_rgm_context,
            p_ecode);

    if (ret_val == ICF_SUCCESS)
    {
        if ((*p_p_rgm_context)->rgm_state == ICF_RGM_05_DEREGISTERING)
        {
            /* Do Nothing and return Success */
            *p_status =  ICF_RGM_DBM_NO_ACTION;
            ret_val = ICF_SUCCESS;
        }
        
        else
        {
            if((*p_p_rgm_context)->rgm_duration > new_duration)
            {
                (*p_p_rgm_context)->rgm_duration = new_duration;
                *p_status = ICF_RGM_DBM_DURATION_CHANGE; 
            }
            else if ((*p_p_rgm_context)->rgm_duration == new_duration)
            {
                *p_status =  ICF_RGM_DBM_NO_ACTION;
            }
            else
            {
                if ((*p_p_rgm_context)->rgm_duration == 
                        p_dbm_line_data->registration_duration)
                {
                    /* Old Duration of registration block is stored */
                    old_reg_duration = (*p_p_rgm_context)->rgm_duration;
                    
                    /* Duration on line is changed to new duration to
                     * facilitate processing
                     */
                    old_line_duration = p_dbm_line_data->registration_duration;
                    p_dbm_line_data->registration_duration = new_duration;
                    
                    ret_val = icf_rgm_save_smallest_dur(
                            p_glb_pdb,
                            *p_p_rgm_context,
                            p_ecode);
                    
                    /* Old duration on the line is restored */
                    p_dbm_line_data->registration_duration = old_line_duration;

                    if ((*p_p_rgm_context)->rgm_duration != 
                            old_reg_duration)
                    {
                        *p_status = ICF_RGM_DBM_DURATION_CHANGE; 
                    }
                    else
                    {
                        *p_status =  ICF_RGM_DBM_NO_ACTION; 
                    }
                }  
                else
                {
                    *p_status =  ICF_RGM_DBM_NO_ACTION;
                }
            }
            /*ret_val = ICF_SUCCESS;*/
        }
    }
    ICF_FUNCTION_EXIT(p_glb_pdb)
    return ret_val;
}

/****************************************************************************
 * FUNCTION:            icf_rgm_modify_dur_roll_not_all_lines
 *
 * DESCRIPTION:         This function is called by the functions of RGM module
 *                      to carry out rollback processing in case set  
 *                      registration params API contains duration change.
 *
 *
 * RETURNS:             
 *
 ***************************************************************************/
icf_return_t icf_rgm_modify_dur_roll_not_all_lines(
        INOUT icf_crm_pdb_st *p_crm_pdb,
        INOUT icf_rgm_set_reg_params_not_all_lines_list_st 
        *p_modify_duration_context)
{
    icf_int16_t loop_index; /* This should be signed int */
    icf_int16_t node_index; /* This should be signed int */
    icf_glb_pdb_st *p_glb_pdb; 
    icf_error_t *p_ecode = ICF_ERROR_NONE;
    icf_rgm_set_reg_params_node_st *p_node;
    icf_return_t ret_val;

    ICF_FUNCTION_ENTER(p_crm_pdb->p_internal_msg->msg_hdr.p_glb_pdb)
   
    ICF_RGM_TRACE(p_crm_pdb, ICF_TRACE_MODIFY_DUR_ROLLBACK)
        
    p_glb_pdb = p_crm_pdb->p_internal_msg->msg_hdr.p_glb_pdb;
    
    p_ecode = &(p_crm_pdb->p_internal_msg->msg_hdr.ecode);
    
    for (loop_index = p_modify_duration_context->num_of_elements - 1;
            loop_index >= 0; loop_index--)
    {
        
        if (ICF_RGM_NODE_INVALID == p_modify_duration_context->
                local_context[loop_index].num_of_nodes)
        {
            /* This node need not be processed */
            continue;
        }
        
        p_node = p_modify_duration_context->local_context[loop_index].p_node;
        
        /* This is loop for traversing all nodes of a block in reverse
         * order
         */
        for (node_index = p_modify_duration_context->local_context
                [loop_index].num_of_nodes - 1; node_index >= 0; 
                node_index--)
        {
            if (p_node[node_index].status == ICF_RGM_DBM_DURATION_CHANGE)
            {
                ret_val = icf_rgm_save_smallest_dur(
                        p_glb_pdb,
                        p_node[node_index].p_rgm_context,
                        p_ecode);

            }

        }/* End of for loop for traversing nodes of a block*/

        /* Free the memory allocated to the node */
        ICF_MEMFREE(p_glb_pdb, (icf_uint8_t *)p_node, ICF_MEM_COMMON,
                p_ecode, ret_val)
        /* Mark Node as Invalid */
        p_modify_duration_context->local_context[loop_index].num_of_nodes = 
            ICF_RGM_NODE_INVALID;     
        
    }/* End of for loop for traversing all blocks */
    ret_val = ICF_SUCCESS;
    ICF_FUNCTION_EXIT(p_glb_pdb)
    return ret_val;
}

/***********************************************************************
 * FUNCTION:            icf_rgm_modify_dur_roll_all_lines
 *
 * DESCRIPTION:         This function is called by the functions of RGM module
 *                      to carry out rollback processing in case set  
 *                      registration params API contains duration change.
 *
 *
 * RETURNS:             
 *
 **********************************************************************/
icf_return_t icf_rgm_modify_dur_roll_all_lines(
        INOUT icf_crm_pdb_st *p_crm_pdb,
        INOUT icf_rgm_set_reg_params_all_lines_list_st 
        *p_modify_duration_context)
{
    icf_int16_t loop_index; /* This should be signed int */
    icf_int16_t node_index; /* This should be signed int */
    icf_glb_pdb_st *p_glb_pdb; 
    icf_error_t *p_ecode = ICF_ERROR_NONE;
    icf_rgm_set_reg_params_node_st *p_node;
    icf_return_t ret_val;
    
    ICF_FUNCTION_ENTER(p_crm_pdb->p_internal_msg->msg_hdr.p_glb_pdb)

    ICF_RGM_TRACE(p_crm_pdb, ICF_TRACE_MODIFY_DUR_ROLLBACK)
        
    p_glb_pdb = p_crm_pdb->p_internal_msg->msg_hdr.p_glb_pdb;
    
    p_ecode = &(p_crm_pdb->p_internal_msg->msg_hdr.ecode);
    
    for (loop_index = p_modify_duration_context->num_of_elements - 1;
            loop_index >= 0; loop_index--)
    {
        
        if (ICF_RGM_NODE_INVALID == p_modify_duration_context->
                local_context[loop_index].num_of_nodes)
        {
            /* This node need not be processed */
            continue;
        }
        
        p_node = p_modify_duration_context->local_context[loop_index].p_node;
        
        /* This is loop for traversing all nodes of a block in reverse
         * order
         */
        for (node_index = p_modify_duration_context->local_context
                [loop_index].num_of_nodes - 1; node_index >= 0; 
                node_index--)
        {
            if (p_node[node_index].status == ICF_RGM_DBM_DURATION_CHANGE)
            {
                /* Deallocate the block */
                ret_val = icf_rgm_save_smallest_dur(
                        p_glb_pdb,
                        p_node[node_index].p_rgm_context,
                        p_ecode);

            }

        }/* End of for loop for traversing nodes of a block*/

        /* Free the memory allocated to the node */
        ICF_MEMFREE(p_glb_pdb, (icf_uint8_t *)p_node, ICF_MEM_COMMON,
                p_ecode, ret_val)
        /* Mark Node as Invalid */
        p_modify_duration_context->local_context[loop_index].num_of_nodes = 
            ICF_RGM_NODE_INVALID;     
        
    }/* End of for loop for traversing all blocks */
    ret_val = ICF_SUCCESS;
    ICF_FUNCTION_EXIT(p_glb_pdb)
    return ret_val;
}

#endif /* endif: ICF_FEATURE_ABANDON
        * For ICF features not supported in ICF */

/*********************************************************************
 * FUNCTION:            icf_rgm_save_smallest_dur
 *
 * DESCRIPTION:         This function is called by the 
 *                      functions of RGM module
 *                      in order to save the smallest duration in the 
 *                      registration block.This will search check from the APP
 *                      data list, If  sent by any application
 *                      then it will search from the Line data
 * RETURNS:             
 *
 ******************************************************************/
icf_return_t icf_rgm_save_smallest_dur(
        INOUT icf_glb_pdb_st *p_glb_pdb,
        INOUT icf_rgm_context_st *p_rgm_context,
        OUT   icf_error_t *p_ecode)
{
    icf_return_t     ret_val = ICF_SUCCESS;

#ifndef ICF_FEATURE_ABANDON
    icf_line_id_t loop_index=0;
    icf_line_data_st *p_dbm_line_data = ICF_NULL;
    icf_duration_t   smallest_duration = 0;
    icf_boolean_t    valid_entry_present = ICF_FALSE;
    icf_boolean_t    duration_changed = ICF_FALSE;
    icf_duration_t   default_duration = ICF_RGM_DEFAULT_DURATION_NON_IMS;
#endif

    icf_config_data_st   *p_dbm_config_data = ICF_NULL;
    
    ICF_FUNCTION_ENTER(p_glb_pdb)

    p_rgm_context=p_rgm_context;

 
    /* ICF functionality put here: smallest duration
     * found using the app_data_list */
    
    /* Get config data from dbm */
    if (ICF_FAILURE == icf_dbm_get_module_glb_data(
        p_glb_pdb, (icf_module_id_t)(ICF_INT_MODULE_CFG),
        (icf_void_t**)(&p_dbm_config_data), p_ecode))
    {
       
        /* Unable to get CFG data */
        
        return ICF_FAILURE;
    }
#ifdef IMS_CLIENT    
    /*Extract the default duration of the registration based on the IMS and NON-IMS*/
ICF_CHECK_IMS_START(p_glb_pdb,ICF_TRUE)
        default_duration = ICF_RGM_DEFAULT_DURATION_IMS_CLIENT;
ICF_CHECK_IMS_END
#endif

#ifndef ICF_FEATURE_ABANDON
    for (loop_index = 0; loop_index < p_dbm_config_data->max_applications;
           loop_index++)
    {
        if (ICF_FALSE == p_rgm_context->p_app_data_list[
            loop_index].is_valid)
        {
            continue;
        }
	    if (ICF_IS_BIT_SET(p_rgm_context->p_app_data_list[
                            loop_index].app_indicator,
                            ICF_RGM_APP_DEREG_ON_PROBATION))
        {                               
                 continue;
        }
        valid_entry_present = ICF_TRUE;
        if(p_rgm_context->p_app_data_list[loop_index].expiry != 
                                        default_duration)
        {
            /* If application does not send any expiry value in the
             * Registration request then we set it as ICF_RGM_DEFAULT_DURATION
             */
            smallest_duration = p_rgm_context->p_app_data_list[
                loop_index].expiry;
            duration_changed = ICF_TRUE;
        }
		else if (p_rgm_context->p_app_data_list[loop_index].expiry == default_duration)
        {
            smallest_duration = p_rgm_context->p_app_data_list[loop_index].expiry;
        }

        if ((ICF_TRUE == duration_changed ) &&
            (smallest_duration > p_rgm_context->p_app_data_list[
              loop_index].expiry))
        {
            smallest_duration = p_rgm_context->p_app_data_list[
                loop_index].expiry;
        }
    }/* Loop ends for all the applications */
    
    if (ICF_FALSE == valid_entry_present)
    {
        smallest_duration = 0;
    }
    else
    {
        if (!p_dbm_config_data->ims_oprn_flag)
            
        {
            /* For NON IMS the smallest is picked from app data*/
        }
        else
        {
            /* the default  is being taken as the standard value for IMS option */
            smallest_duration = default_duration;
        }
        
    }/* valid entries present */

    /* functionality of ICF */

   if((0 == smallest_duration ) && (ICF_TRUE == valid_entry_present))
   {
       /* Get the lowest from the line data */
       /* Get pointer to line block */
       ret_val = icf_dbm_fetch_line_cntxt_blk(
           p_glb_pdb,
           p_rgm_context->rgm_list_of_lines[0],       
           &p_dbm_line_data,
           p_ecode);
       
       if (ICF_SUCCESS == ret_val)
       {
           smallest_duration = p_dbm_line_data->registration_duration;
           for (loop_index = 1; loop_index < p_rgm_context->num_of_lines;
           loop_index++)
           {
               ret_val = icf_dbm_fetch_line_cntxt_blk(
                   p_glb_pdb,
                   p_rgm_context->rgm_list_of_lines[loop_index],       
                   &p_dbm_line_data,
                   p_ecode);
               
               if (ICF_SUCCESS == ret_val)
               {
                   if (smallest_duration > p_dbm_line_data->registration_duration)
                   {
                       smallest_duration = p_dbm_line_data->registration_duration;
                   } 
               }
               else
               {
                   /* Return Failure */
                   break;
               }
               
           }/* End of for loop */

           if ((smallest_duration/1000) > 0)
           {
               p_rgm_context->rgm_duration = smallest_duration;
           }
       }
   }
    
    /* smallest_duration should be interm of seconds (we store as milli second),
     * Else set it to the default duration */
     
    if ((smallest_duration/1000) > 0)
    {
        p_rgm_context->rgm_duration = smallest_duration;
    }
    else
    {
		if(ICF_TRUE == valid_entry_present)
		{
	        ICF_ALARM(("[REGM] Smallest duration %d ms is not interm of seconds,"\
                      "Default Duration set as %d ms",smallest_duration,
                      default_duration));
			p_rgm_context->rgm_duration = default_duration;
		}
		else
		{
			/* It is a case of De registration so fill the duration as
			 * 0 */
			 
			p_rgm_context->rgm_duration = 0;
		}
    }
#endif
    ICF_FUNCTION_EXIT(p_glb_pdb)
    return ret_val;

}/* Function icf_rgm_save_smallest_dur() ends here */



#ifndef ICF_FEATURE_ABANDON/* For ICF features not supported in ICF */
/****************************************************************************
 * FUNCTION:            icf_rgm_line_reg_configured
 *
 * DESCRIPTION:         This function is called by the functions of RGM module
 *                      to fetch the pointer of line block on which 
 *                      registrar is configured.
 *
 *
 * RETURNS:             
 *
 ***************************************************************************/
icf_return_t icf_rgm_line_reg_configured(
        INOUT icf_glb_pdb_st *p_glb_pdb,
        IN    icf_line_id_t line_id,
        OUT   icf_line_data_st **p_p_dbm_line_data,
        OUT   icf_error_t *p_ecode)
      
{
    icf_return_t ret_val = ICF_FAILURE;

    ICF_FUNCTION_ENTER(p_glb_pdb)
    
    /* Get pointer to line block */
    ret_val = icf_dbm_fetch_line_cntxt_blk(
            p_glb_pdb,
            line_id,       
            p_p_dbm_line_data,
            p_ecode);
    
    if (ICF_SUCCESS == ret_val)
    {
        /* Check whether both the flags are set */
        if ((*p_p_dbm_line_data)->line_data_received &
           ICF_CFG_LINE_DATA_REGISTRAR_RECD)
        {
            ret_val = ICF_SUCCESS;
        }
        else
        {
            ret_val = ICF_FAILURE;
        }
    }
    ICF_FUNCTION_EXIT(p_glb_pdb)
    return ret_val;
}

/****************************************************************************
 * FUNCTION:            icf_rgm_line_self_configured
 *
 * DESCRIPTION:         This function is called by the functions of RGM module
 *                      to fetch the pointer of line block on which 
 *                      any user is configured.
 *
 *
 * RETURNS:             
 *
 ***************************************************************************/
icf_return_t icf_rgm_line_self_configured(
        INOUT icf_glb_pdb_st *p_glb_pdb,
        IN    icf_line_id_t line_id,
        OUT   icf_line_data_st **p_p_dbm_line_data,
        OUT   icf_error_t *p_ecode)
      
{
    icf_return_t ret_val = ICF_FAILURE;
    
    ICF_FUNCTION_ENTER(p_glb_pdb)
    
    /* Get pointer to line block */
    ret_val = icf_dbm_fetch_line_cntxt_blk(
            p_glb_pdb,
            line_id,       
            p_p_dbm_line_data,
            p_ecode);
    
    if (ICF_SUCCESS == ret_val)
    {
        /* Check whether both the flags are set */
        if ((*p_p_dbm_line_data)->line_data_received &
           ICF_CFG_LINE_DATA_SELF_ADDR_RECD) 
        {
            ret_val = ICF_SUCCESS;
        }
        else
        {
            ret_val = ICF_FAILURE;
        }
    }
    ICF_FUNCTION_EXIT(p_glb_pdb)
    return ret_val;
}

/****************************************************************************
 * FUNCTION:            icf_rgm_line_self_reg_config
 *
 * DESCRIPTION:         This function is called by the functions of RGM module
 *                      to fetch the pointer of line block on which Self Address
 *                      and registrar are configured.
 *
 *
 * RETURNS:             
 *
 ***************************************************************************/
icf_return_t icf_rgm_line_self_reg_config(
        INOUT icf_glb_pdb_st *p_glb_pdb,
        IN    icf_line_id_t line_id,
        OUT   icf_line_data_st **p_p_dbm_line_data,
        OUT   icf_error_t *p_ecode)
      
{
    icf_return_t ret_val = ICF_FAILURE;
    
    ICF_FUNCTION_ENTER(p_glb_pdb)
    
    /* Get pointer to line block */
    ret_val = icf_dbm_fetch_line_cntxt_blk(
            p_glb_pdb,
            line_id,       
            p_p_dbm_line_data,
            p_ecode);
    
    if (ICF_SUCCESS == ret_val)
    {
        /* Check whether both the flags are set */
        if (((*p_p_dbm_line_data)->line_data_received &
           ICF_CFG_LINE_DATA_SELF_ADDR_RECD) &&
           ((*p_p_dbm_line_data)->line_data_received &
             ICF_CFG_LINE_DATA_REGISTRAR_RECD))
        {
            ret_val = ICF_SUCCESS;
        }
        else
        {
            ret_val = ICF_FAILURE;
        }
    }
    ICF_FUNCTION_EXIT(p_glb_pdb)
    return ret_val;
}
#endif /* endif: ICF_FEATURE_ABANDON
        * For ICF features not supported in ICF */


/******************************************************************
 * FUNCTION:            icf_rgm_forceful_dereg_all_users
 *
 * DESCRIPTION:         This function is called by the 
 *                      functions of CFG
 *                      module in order to forcefully deregister all users.
 *
 * RETURNS:             
 *
 ****************************************************************/
icf_return_t icf_rgm_forceful_dereg_all_users(
        INOUT icf_crm_pdb_st *p_crm_pdb)
{
    icf_glb_pdb_st *p_glb_pdb;
    icf_error_t *p_ecode;
    icf_rgm_context_st *p_rgm_context = ICF_NULL;
    icf_return_t ret_val = ICF_SUCCESS;
    icf_uint32_t        count_reg = ICF_NULL;
    
    /* For sending CFM/IND*/
    icf_rgm_register_resp_st        register_response;
    /* SPR 16019: Boolean to store whether register response to be sent
     * or not
     */
    icf_boolean_t                   send_reg_resp = ICF_FALSE;
    icf_uint8_t                     index = ICF_NULL;
    icf_line_id_t                   line_id = ICF_NULL;
    icf_line_data_st                *p_line_data = ICF_NULL;

    ICF_SET_MODULE(p_crm_pdb->p_internal_msg->msg_hdr.p_glb_pdb,
            ICF_INT_MODULE_REGM)
    
    ICF_FUNCTION_ENTER(p_crm_pdb->p_internal_msg->msg_hdr.p_glb_pdb)

    p_glb_pdb = p_crm_pdb->p_internal_msg->msg_hdr.p_glb_pdb;
    ICF_ASSERT(
            p_crm_pdb->p_internal_msg->msg_hdr.p_glb_pdb,
            (ICF_NULL != p_glb_pdb))
    
    p_ecode = &(p_crm_pdb->p_internal_msg->msg_hdr.ecode);
    ICF_RGM_INITIALIZE_REG_RESP_STRUCT(register_response);

    count_reg = 1;
    /* DBM will return Failure if all Registration blocks are exhausted */
    while (ICF_SUCCESS == icf_dbm_get_used_rgn_blk(
                p_glb_pdb,
                (icf_uint16_t)count_reg,
                &p_rgm_context,
                p_ecode))
    {
        p_crm_pdb->p_rgm_context = p_rgm_context;
        /* Stop Timer */
        if ( p_rgm_context->timer_id != ICF_TIMER_ID_INVALID)
        {
            ICF_STOP_TIMER(p_glb_pdb, \
                    p_rgm_context->timer_id, p_ecode, ret_val)
        }

        /* SPR 18880 : Free timer  block only if timer was 
           stopped successfully */
        if((ICF_SUCCESS == ret_val) &&
           (ICF_NULL != p_rgm_context->p_timer_buff))
        {
            /* Call DBM to free timer structure */
            ret_val = icf_dbm_free_timer_block(
                    p_glb_pdb,
                    (icf_void_t *)(p_rgm_context->p_timer_buff),
                    p_ecode);   
            
            if(ICF_SUCCESS == ret_val)
            {
                /* Set Timer ID to INVALID */
                p_rgm_context->timer_id = ICF_TIMER_ID_INVALID;
                /* Set pointer to timer as NULL */
                p_rgm_context->p_timer_buff = ICF_NULL;
            }
            /* Timer block freeing failure. Raise a alarm and proceed. */
            else
            {
                ICF_PRINT(((icf_uint8_t *)"[RGM]: Failure in freeing timer block.\n"))
            }
        }

        /* SPR 19601: In case the registration block information is reset then no
         * attempt should be made to invloke dereigstration requests, else it can  
         * result into undefined behaviour
         */ 
        if (ICF_RGM_00_IDLE == p_rgm_context->rgm_state)
        {
            count_reg ++;
            continue;
        }
         
              /* SPR 16019:Changes for deregistration
       */
        if (ICF_RGM_05_DEREGISTERING != p_rgm_context->rgm_state)
        {
            /* State = REGISTERED begins */
            p_rgm_context->indicator |= ICF_RGM_INTERNAL_GENERATED_REG;
#ifdef ICF_NW_ACTIVATE
            if (ICF_TRUE != p_crm_pdb->network_deactivated)
            {
#endif
                /* Send De-Registration request */
            
                /* Setting duration to Zero */
                p_rgm_context->rgm_duration = 0;

                /* Fill CRM PDB Structure */
                p_crm_pdb->p_internal_msg->p_msg_data = (icf_uint8_t *)
                        (p_rgm_context);
                p_crm_pdb->p_internal_msg->msg_hdr.msg_id = ICF_NW_OTG_REG_REQ;
                p_crm_pdb->p_internal_msg->msg_hdr.payload_length = 
                    (icf_uint16_t)(ICF_PORT_SIZEOF(icf_rgm_context_st)); 
                /* SPR 16019: Forcefully clear ongoing registration
                 */
                if(ICF_RGM_01_REGISTERING == p_rgm_context->rgm_state ||
                        ICF_RGM_04_AUTHORIZING == p_rgm_context->rgm_state)
                {
                    p_rgm_context->indicator |= ICF_RGM_CLEAR_ONGOING_REGISTRATION;
                    send_reg_resp = ICF_TRUE;
                }
                /* SPR 16019: TCP Blocking connect required if user already
                 * registered.
                 */
                else if(ICF_RGM_03_REGISTERED == p_rgm_context->rgm_state)
                {
                    p_crm_pdb->p_config_data->is_tcp_blocking_connect = ICF_TRUE;
                }
                /* The SIP message should be sent only when network is not
                 * deactivated */
                /* Call SSA to send De-registration Request */
                ret_val = icf_ssa_process_mesg(p_crm_pdb->p_internal_msg);
                p_rgm_context->rgm_state = ICF_RGM_05_DEREGISTERING;
             #ifdef ICF_NW_ACTIVATE
            }
#endif
        }/* State = REGISTERED ends */

       if (ICF_SUCCESS == ret_val && (ICF_TRUE == p_crm_pdb->forceful_clear))
        {
            /*SPR 16019: send register response and forcefully clear reg context.
             */
            if(ICF_TRUE == send_reg_resp)
                {
                    /* Send failure responses/indications to applications */
                    register_response.p_crm_pdb = p_crm_pdb;
                    register_response.p_rgm_context = p_rgm_context;

                    /* Not setting the options_flag with
                     * SINGLE_APP value will make the send function to 
                     * Loop on the application list to send the register CFM/ IND
                     * to the applications that are kept in this rgm_context */

                    register_response.app_id = ICF_NULL;

                    register_response.options_flag = ICF_NULL;
                    register_response.options_flag |= ICF_RGM_RESPONSE_IND;

                    register_response.p_request_payload = ICF_NULL;


                    register_response.status = ICF_SUCCESS;
                    register_response.error_cause = ICF_NULL;
                    register_response.request_type = ICF_REQUEST_TYPE_DEREGISTER;

                    p_crm_pdb->resp_to_be_sent = ICF_FALSE;/*for CFG to not send response*/
                    ret_val = icf_rgm_send_response(p_glb_pdb,
                            &register_response, p_ecode);
                    send_reg_resp = ICF_FALSE;


                }
            /* Send registration indicatiom to application for deregistration*/
            register_response.p_crm_pdb = p_crm_pdb;
            register_response.p_rgm_context = p_rgm_context;

            /* Not setting the options_flag with
             * SINGLE_APP value will make the send function to 
             * Loop on the application list to send the register CFM/ IND
             * to the applications that are kept in this rgm_context */

            register_response.app_id = ICF_NULL;

            register_response.options_flag = ICF_NULL;
            register_response.options_flag |= ICF_RGM_RESPONSE_IND;

            register_response.p_request_payload = ICF_NULL;
            register_response.status = ICF_SUCCESS;
            register_response.error_cause = ICF_CAUSE_NETWORK_DEREGISTERED;
            register_response.request_type = ICF_REQUEST_TYPE_DEREGISTER;

            ret_val = icf_rgm_send_response(p_glb_pdb,
                    &register_response, p_ecode);

            if(ICF_RGM_INTERNAL_GENERATED_REG == (p_rgm_context->indicator & \
                        ICF_RGM_INTERNAL_GENERATED_REG))
               {
                   p_crm_pdb->resp_to_be_sent = ICF_TRUE;
                   p_rgm_context->indicator &= ~(ICF_RGM_INTERNAL_GENERATED_REG);
               }
        }
        else
       {
           /* Send failure response to application
            */
           register_response.request_type = ICF_REQUEST_TYPE_DEREGISTER;
           /* For Trace Purposes Only */
           p_crm_pdb->p_rgm_context = p_rgm_context;
           ICF_RGM_SET_ECODE(p_crm_pdb, ICF_UNABLE_TO_SEND_REG_REQ)
            
           register_response.options_flag = ICF_NULL;
           register_response.options_flag |= ICF_RGM_RESPONSE_IND;
           register_response.options_flag |= ICF_RGM_CTXT_COPY;            
           register_response.status = ICF_FAILURE;
           register_response.error_cause = ICF_UNABLE_TO_SEND_REG_REQ;
           register_response.p_rgm_context = p_rgm_context;
           register_response.p_request_payload = ICF_NULL;
           register_response.p_crm_pdb = p_crm_pdb;
           register_response.app_id = ICF_NULL;
           ret_val = icf_rgm_send_response(p_crm_pdb->p_glb_pdb,
                   &register_response,
                p_crm_pdb->p_ecode);
       }
       /* Fix for freeing contact uri list SPR 17017
        *
        */
       for(index=0;index < p_rgm_context->num_of_lines;index++)
       {
           line_id = p_rgm_context->rgm_list_of_lines[index];
           if ( ICF_SUCCESS == icf_dbm_fetch_line_cntxt_blk(p_glb_pdb,line_id,
                       (icf_void_t *)&(p_line_data), p_ecode))
           {
               icf_dbm_clear_public_uri_node_for_line (p_glb_pdb, line_id, 
                       &(p_rgm_context->rgm_user_address),
                       ICF_USER_DB_USE_ASSOCIATED_URIS | 
                       ICF_USER_DB_USE_CONTACT_URIS,
                          p_ecode);
           }
       }
       /* Clear Registration context */
       icf_rgm_clear_reg_context(
                p_crm_pdb,
                p_rgm_context);

        /* Fix for CSR 1-8597937 SPR 20435.
         * There is no need to increment the counter as DBM implicit
         * check for next available used registration context.
         */
      /*           count_reg ++;*/
    }/* while ends here */
    
    ret_val = ICF_SUCCESS;

    ICF_FUNCTION_EXIT(p_crm_pdb->p_internal_msg->msg_hdr.p_glb_pdb)
    
    ICF_RESET_MODULE(p_crm_pdb->p_internal_msg->msg_hdr.p_glb_pdb)
    
    return ret_val;
}/* Function icf_rgm_forceful_dereg_all_users() ends here */

#ifdef ICF_NW_ACTIVATE
ICF_4_0_CRM_START

/********************************************************************
 * FUNCTION:     icf_rgm_nw_indicated_dereg
 *
 * DESCRIPTION:  This function is called by the functions of CFG
 *               module in order to deregister applications of the list.
 *               These applications belong to same network which has
 *               deactivated. This does not clear the rgm-context
 *               because when the network comes up, ICF should re-register
 *               the applications without a trigger from applications.
 *
 *               SPR 9069: This release the app_id_list is not used, this
 *               will be helpful for multiple network scenario.
 *
 * RETURNS:      FAILURE: if all applications are not found in single
 *               rgm_context
 *               SUCCESS:
 *
 ***********************************************************************/
icf_return_t icf_rgm_nw_indicated_dereg(
        INOUT    icf_crm_pdb_st        *p_crm_pdb,
        INOUT    icf_app_id_list_st    *p_app_list)
{
    icf_uint16_t            index_cntxt = ICF_NULL;
    icf_void_t            *p_buff = ICF_NULL;
    icf_rgm_glb_data_st    *p_rgm_glb_data = ICF_NULL;
    icf_return_t            ret_val = ICF_SUCCESS;

    ICF_FUNCTION_ENTER(p_crm_pdb->p_glb_pdb)

   /* To remove warning */
    p_app_list=p_app_list; 
    index_cntxt=1;
    p_crm_pdb->p_rgm_context = ICF_NULL;


/* SPR 9069: Fix start */
    /* Call DBM to get the pointer to the RGM global data in p_buff */
    if(ICF_SUCCESS == icf_dbm_get_module_glb_data(
            p_crm_pdb->p_glb_pdb,
            (icf_int_module_id_t)(ICF_INT_MODULE_REGM),
            &p_buff,
            p_crm_pdb->p_ecode))
    {
            ICF_ASSERT(
                p_crm_pdb->p_internal_msg->msg_hdr.p_glb_pdb,
                (ICF_NULL != p_buff))
            /* Typecast into global data structure */
            p_rgm_glb_data = (icf_rgm_glb_data_st *)(p_buff);
			p_crm_pdb->p_rgm_glb_data = p_rgm_glb_data;
                
                
            while (ICF_SUCCESS == icf_dbm_get_used_rgn_blk(
                    p_crm_pdb->p_glb_pdb,
                    (icf_uint16_t)index_cntxt,&(p_crm_pdb->p_rgm_context),
                    p_crm_pdb->p_ecode))
            {
            /*if (ICF_FAILURE == icf_rgm_all_app_present(p_crm_pdb,
                    p_app_list, &status))
                    {
                    continue;
                    }*/
                if (p_crm_pdb->p_rgm_context)
                {
                    
                    ret_val = p_rgm_glb_data->icf_rgm_state_machine[p_crm_pdb->p_rgm_context->
                        rgm_state][ICF_RGM_NW_DEACTIVATED](p_crm_pdb);
                }
                p_crm_pdb->p_rgm_context = ICF_NULL;
                index_cntxt++;            
                
            }
    }/* rgm_glb_data found: ends */
    else
    {/* rgm_glb_data not found */
        ret_val = ICF_FAILURE;
    }
    
    
    ICF_FUNCTION_EXIT(p_crm_pdb->p_glb_pdb)
    return ret_val;
/* SPR 9069: Fix end */
    
    
}/* Function icf_rgm_nw_indicated_dereg() ends here*/

/********************************************************************
 * FUNCTION:     icf_rgm_recover_registration
 *
 * DESCRIPTION:  This function is called by the functions of CFG
 *               module in order to reregister applications of the list.
 *               These applications belong to same network which was
 *               activated. The rgm_context will be unregistered with
 *               NW-DEACTIVATED set in indicator.
 *               When the network comes up, ICF should re-register
 *               the applications without a trigger from applications.
 *
 *               SPR 9069: This release the app_id_list is not used, this
 *               will be helpful for multiple network scenario.
 *
 * RETURNS:      FAILURE: if all applications are not found in single
 *               rgm_context. System error in data fetching
 *               SUCCESS:
 *
 ***************************************************************/
icf_return_t icf_rgm_recover_registration(
        INOUT    icf_crm_pdb_st        *p_crm_pdb,
        INOUT    icf_app_id_list_st    *p_app_list)
{
    icf_uint16_t            index_cntxt = ICF_NULL;
    icf_void_t            *p_buff = ICF_NULL;
    icf_rgm_glb_data_st    *p_rgm_glb_data;
    icf_return_t            ret_val = ICF_SUCCESS;

    ICF_FUNCTION_ENTER(p_crm_pdb->p_glb_pdb)
    /* To remove warning */
    p_app_list=p_app_list;
    index_cntxt=1;
    p_crm_pdb->p_rgm_context = ICF_NULL;

/* SPR 9069: Fix start */
    /* Call DBM to get the pointer to the RGM global data in p_buff */
    if(ICF_SUCCESS == icf_dbm_get_module_glb_data(
            p_crm_pdb->p_glb_pdb,
            (icf_int_module_id_t)(ICF_INT_MODULE_REGM),
            &p_buff,
            p_crm_pdb->p_ecode))
    {
            ICF_ASSERT(
                p_crm_pdb->p_internal_msg->msg_hdr.p_glb_pdb,
                (ICF_NULL != p_buff))
            /* Typecast into global data structure */
            p_rgm_glb_data = (icf_rgm_glb_data_st *)(p_buff);
			p_crm_pdb->p_rgm_glb_data = p_rgm_glb_data;                
                
            while (ICF_SUCCESS == icf_dbm_get_used_rgn_blk(
                    p_crm_pdb->p_glb_pdb,
                    (icf_uint16_t)index_cntxt,&(p_crm_pdb->p_rgm_context),
                    p_crm_pdb->p_ecode))
            {
            /*if (ICF_FAILURE == icf_rgm_all_app_present(p_crm_pdb,
                    p_app_list, &status))
                    {
                    continue;
                    }*/
                if (p_crm_pdb->p_rgm_context)
                {
                    
                    ret_val = p_rgm_glb_data->icf_rgm_state_machine[p_crm_pdb->p_rgm_context->
                        rgm_state][ICF_RGM_NW_ACTIVATED](p_crm_pdb);
                }
                p_crm_pdb->p_rgm_context = ICF_NULL;
                index_cntxt++;
                
                
            }
    }/* rgm_glb_data found: ends */
    else
    {/* rgm_glb_data not found */
        ret_val = ICF_FAILURE;
    }
    
    
    ICF_FUNCTION_EXIT(p_crm_pdb->p_glb_pdb)
    return ret_val;
/* SPR 9069: Fix end */

}/* Function icf_rgm_recover_registration() ends here */

#endif

ICF_4_0_CRM_END

/************************************************************************
 * FUNCTION:            icf_rgm_process_notify
 *
 * DESCRIPTION:         This function handles notify for reg event.
 *
 *
 * RETURNS:             
 *
 ************************************************************************/
icf_return_t icf_rgm_process_notify(
                    INOUT icf_crm_pdb_st *p_crm_pdb)
{
    icf_return_t                 ret_val = ICF_SUCCESS;
    icf_sic_notify_msg_st        *p_notify = ICF_NULL; 
    icf_uint32_t                 count = 0,index_regn_blk = 0;
    icf_glb_pdb_st               *p_glb_pdb = ICF_NULL;
    icf_msg_body_st				*p_msg_body = ICF_NULL;
    icf_int32_t					old_version = ICF_NULL;
    /*Klocwork warning fix ; Variable recd_reg_info not initialised */
    icf_rgm_reginfo_st			recd_reg_info = {-1,0,{0,ICF_NULL}};
	icf_reg_event_subs_ctx_st	*p_reg_event_subs_ctx = ICF_NULL;
    /*Klocwork warning fix ; Variable p_regn_blk not initialised */
    icf_rgm_regn_block_st		*p_regn_blk = ICF_NULL;
    icf_user_db_node_st	        *p_node = ICF_NULL;
    icf_address_list_node_st    *p_assoc_uri_list = ICF_NULL;
    icf_boolean_t               match_assoc_uri = ICF_FALSE;
    icf_error_t                 ecode;
    icf_rgm_register_resp_st	register_response;


    /* function entry trace */
    ICF_FUNCTION_ENTER(p_crm_pdb->p_internal_msg->msg_hdr.p_glb_pdb) 
    
	p_glb_pdb = p_crm_pdb->p_internal_msg->msg_hdr.p_glb_pdb;

    p_crm_pdb->p_glb_pdb = p_glb_pdb;

    /* pointer to ecode */
    p_crm_pdb->p_ecode =
                &(p_crm_pdb->p_internal_msg->msg_hdr.ecode);
    ICF_RGM_INITIALIZE_REG_RESP_STRUCT(register_response);   

    /* Pointer to notify message */
    p_notify = (icf_sic_notify_msg_st *)
                    (p_crm_pdb->p_internal_msg->p_msg_data);

    if (ICF_NULL == p_notify)
    {
       ICF_FUNCTION_EXIT(p_crm_pdb->p_internal_msg->msg_hdr.p_glb_pdb) 
       return ICF_FAILURE;
    }

    /* get CFG global data */
    ret_val = icf_dbm_get_module_glb_data(
        p_crm_pdb->p_glb_pdb,
        (icf_int_module_id_t)(ICF_INT_MODULE_CFG),
        (icf_void_t **)(&(p_crm_pdb->p_config_data)),
        p_crm_pdb->p_ecode);     

    if (ICF_NULL == p_crm_pdb->p_config_data)
    {
       ICF_FUNCTION_EXIT(p_crm_pdb->p_internal_msg->msg_hdr.p_glb_pdb) 
       return ICF_FAILURE;
    }

  /* get the pointer to RGM global data */
    ret_val = icf_dbm_get_module_glb_data(
                p_crm_pdb->p_glb_pdb,
                (icf_int_module_id_t)(ICF_INT_MODULE_REGM),
                (icf_void_t **)(&(p_crm_pdb->p_rgm_glb_data)),
                p_crm_pdb->p_ecode);     

    if (ICF_NULL == p_crm_pdb->p_rgm_glb_data)    
    {
       ICF_FUNCTION_EXIT(p_crm_pdb->p_internal_msg->msg_hdr.p_glb_pdb)
       return ICF_FAILURE;
    }

/* Start: Multiple Public Users ICF5.0*/
    /* pointer to sic_context*/
    p_reg_event_subs_ctx = (icf_reg_event_subs_ctx_st *)(p_notify->p_subs_key);

    if(ICF_NULL != p_reg_event_subs_ctx)
    {
        /* klocwork warning removal and also a valid check */
        /*Assign p_rgm_context from p_subs_key received from SIC*/
        p_crm_pdb->p_rgm_context = p_reg_event_subs_ctx->p_rgm_context;
        /*SPR 20026 In case the subscription state header of NOTIFY
          does have the state as terminated then there is no need to
          parse the xml body and we should clear the REGISTRATION 
          reason being that if only the subscription is cleared then
          network initiated de-registration cannot happen because 
          subscription no more exists.*/
        if(ICF_SUBS_STATE_TERMINATED == p_notify->subs_state)
        {
            /* Basic values for Sending */
	        register_response.p_crm_pdb = p_crm_pdb;
	        register_response.app_id = ICF_NULL;
	        register_response.line_id = ICF_NULL;
	        register_response.options_flag = ICF_NULL;
	        register_response.options_flag |= ICF_RGM_CTXT_COPY;/*use rgm_ctxt for To/From*/
	        register_response.options_flag |= ICF_RGM_FILL_URIS;
	        register_response.p_rgm_context = p_crm_pdb->p_rgm_context;
	        register_response.p_request_payload = ICF_NULL;
            /*Set the value of request type as ICF_REQUEST_TYPE_DEREGISTER in
             * register response .This will be send in register status indication
             * to appilcation as request type.*/
	        register_response.request_type = ICF_REQUEST_TYPE_DEREGISTER;
	        register_response.status = ICF_FAILURE;
	        register_response.error_cause = ICF_CAUSE_NETWORK_DEREGISTERED;
	        register_response.response_code = 0;
	        p_crm_pdb->resp_to_be_sent = ICF_FALSE;/*for CFG to not send response*/

	        ret_val = icf_rgm_send_response(p_crm_pdb->p_glb_pdb,
	            	&register_response, p_crm_pdb->p_ecode);
            /*Stop the timer */
	        icf_rgm_stop_timer(p_crm_pdb);
            /*Report trace indication of type
             * ICF_TRACE_DEREG_REQ is called for sending
             * network initiated deregistration to application
             * .*/
            ICF_RGM_APP_TRACE(p_crm_pdb, ICF_TRACE_DEREG_REQ)
			 
            /*Clear the context */
		    icf_rgm_clear_reg_context(p_crm_pdb,
			             p_crm_pdb->p_rgm_context);

			 p_crm_pdb->p_rgm_context = ICF_NULL;
             p_reg_event_subs_ctx = ICF_NULL;
             ICF_FUNCTION_EXIT(p_crm_pdb->p_internal_msg->msg_hdr.p_glb_pdb)
             return ret_val;
        }    
    }

	/* Initializing the reg_info structure with 0 */
    icf_port_memset(
                (icf_void_t*)&(recd_reg_info),
                (icf_uint8_t)ICF_NULL,
                (icf_uint32_t) ICF_PORT_SIZEOF(
                        icf_rgm_reginfo_st));
/* SPR: 9068: fix starts */
    recd_reg_info.version = ICF_INVALID_REGINFO_VERSION;
/* SPR: 9068: fix ends */
    

	if(ICF_NULL != p_notify->body_list.count)
	{
    p_msg_body = p_notify->body_list.body;

	for(count = 0; count < p_notify->body_list.count; count++ )

    {
        if (ICF_NULL != p_msg_body)
        {
            
            if (!icf_port_strcmp(p_msg_body->content_type.str, 
                (icf_uint8_t *)"application/reginfo+xml"))
            {
                break; 
            }
            else
            {
                /* do nothing */
            }
            p_msg_body = p_msg_body->next; 
        }
    }/* For loop on msg_list ends */
    
    if ((ICF_NULL != p_msg_body) && (ICF_NULL != p_msg_body->length))
    {
        /* RGM will not send Failure to the calling function for
         * Parsing Failed 
         * or The XML body dont have any meaning for the current 
         * RGM Context
         * So below we are simply setting ret_val to success in 
         * the failure cases
         */
        if (ICF_FAILURE == icf_rgm_parse_notify_body(p_crm_pdb->p_glb_pdb,
                        &recd_reg_info, p_msg_body->value, 
/* SPR: 9044: fix start */
                        p_msg_body->length,
/* SPR: 9044: fix end */
                                p_crm_pdb->p_ecode))
        {
            ret_val = ICF_FAILURE;
            ICF_PRINT(((icf_uint8_t *)"\nFailure in notify body parsing \n"))
        }
        else if (ICF_FAILURE == icf_rgm_validate_rgm_info_version_state(
                            p_crm_pdb,&recd_reg_info))
        {
            ret_val = ICF_FAILURE;
        }
        else 
		{
            icf_rgm_print_parsed_notify_body(&recd_reg_info);
            
            if (ICF_NULL != recd_reg_info.registration_list.p_regn_block)
			{
				p_regn_blk = recd_reg_info.registration_list.p_regn_block;
			}

			for (index_regn_blk = 0; index_regn_blk < 
						recd_reg_info.registration_list.count;
						index_regn_blk++)
			{
                match_assoc_uri = ICF_FALSE;

				if (ICF_NULL == p_regn_blk)
				{
					break;
				}
				if((ICF_NULL == p_regn_blk->p_aor) ||
						(ICF_NULL == p_regn_blk->p_id)||
						(ICF_RGM_NFY_REG_STATE_INVALID == p_regn_blk->state))
				{
					p_regn_blk = p_regn_blk->p_next;
					continue;
				}
				else
				{
                    /* From now onwards,
                     * rgm context will be fetched directly from p_subs_key
                     * received in the Notify indication.
                     */
                    if (ICF_SUCCESS == icf_dbm_get_public_uri_node_from_uri_for_line(
                                       p_crm_pdb->p_glb_pdb,
                                       p_reg_event_subs_ctx->p_rgm_context->rgm_list_of_lines[0],
                                       p_regn_blk->p_aor,
                                       ICF_USER_DB_USE_ASSOCIATED_URIS,
                                       &p_node,
                                       &ecode))
                    {
                        /*********************ICF 8.0*******************/
                        
                        /*Check if the AOR is the one for which subscrition is
                          send ie whether it is Public AOR or not,if yes then 
                          in the reg context set the bitmask that it is a pubic 
                          AOR.For the check that it is a pubic AOR or not we 
                          check that the public user in user db node  is same
                          as AOR in regm context  */

                        if (ICF_TRUE == icf_dbm_cmp_addr_in_aor(
                                         p_crm_pdb->p_glb_pdb,p_regn_blk->p_aor,
                                         &(p_node->public_user)))
                        {
                            p_reg_event_subs_ctx->p_rgm_context->indicator |=
                                ICF_RGM_PUBLIC_AOR_RECEIVED;
                        }
                       
                        /*Checking whether the AOR in registration block
                         *,is present in Assoc uri list or not ,if not a
                         *public Aor .If not present in Assoc uri list and
                         *also not a public AOR we will ignore the whole
                         *registration block.*/
                        if (!(ICF_RGM_PUBLIC_AOR_RECEIVED & p_reg_event_subs_ctx
                            ->p_rgm_context->indicator))
                        {
                            p_assoc_uri_list = p_node->p_assoc_uri_list;
                            
                            while(ICF_NULL != p_assoc_uri_list)
                            {
                                if (ICF_TRUE == icf_dbm_cmp_addr_in_aor(
                                        p_crm_pdb->p_glb_pdb,p_regn_blk->p_aor,
                                        &(p_assoc_uri_list->user_addr)))
                                { 
                                     match_assoc_uri = ICF_TRUE;
                                     /*SPR 20026 In case of network initiated
                                      * de-registration if NOTIFY with
                                      * subscription state terminated is
                                      * received and aor in xml is of
                                      * Associated URI then set new indicator
                                      * value which shall be used for
                                      * proceeding with the De-Registration*/
                                     p_reg_event_subs_ctx->p_rgm_context->indicator_1 |=
                                            ICF_RGM_ASSOCIATED_URI_RECEIVED;
                                     break;
                                }
                                p_assoc_uri_list = p_assoc_uri_list->p_next;
                            }
                        }   
                        
                        /*If the AOR in regm block is neither public nor
                         * present in the assoc uri list , ignore the whole
                         * regm block*/
                        if ((ICF_FALSE == match_assoc_uri) && 
                            !(ICF_RGM_PUBLIC_AOR_RECEIVED & p_reg_event_subs_ctx
                            ->p_rgm_context->indicator))
                        {
                            p_regn_blk = p_regn_blk->p_next;
                            continue ;
                        }   
                        
                       p_crm_pdb->p_rgm_context = p_reg_event_subs_ctx->p_rgm_context; 
				
                       /*Handles the regn_block obtained from the NOTIFY*/
                       icf_rgm_handle_notify_regn_blk(p_crm_pdb, p_regn_blk);

                       /* Check whether rgm_ctxt_needs_to_be_cleared bitmask is
                        * or not ,if set then break out of the loop and clear 
                        * the rgm context, no further processing of contacts of
                        * AORs needs to be done.Since the contact in public
                        * AOR is our own contact and it is either expired
                        * ,deactivated,unregistered or deactivated*/

                       if (ICF_RGM_CTXT_NEEDS_TO_BE_CLEARED & p_crm_pdb->
                               p_rgm_context->indicator)
                       {
                           /*Stop the timer */
	     		           icf_rgm_stop_timer(p_crm_pdb);
                           /*Report trace indication of type
                            * ICF_TRACE_DEREG_REQ is called for sending
                            * network initiated deregistration to application
                            * .*/
                           ICF_RGM_APP_TRACE(p_crm_pdb, ICF_TRACE_DEREG_REQ)
			   
                           /*Clear the context */
			               icf_rgm_clear_reg_context(p_crm_pdb,
						             p_crm_pdb->p_rgm_context);

			               p_crm_pdb->p_rgm_context = ICF_NULL;
                           p_reg_event_subs_ctx = ICF_NULL;

                           break;
                       }/*if (ICF_RGM_CTXT_NEEDS_TO_BE_CLEARED & ...*/    

                       /*Reset the indicator bit that self contact received*/
                       ICF_RESET_BIT(p_reg_event_subs_ctx->p_rgm_context->
                                  indicator,ICF_RGM_CTXT_NEEDS_TO_BE_CLEARED);

                       /*Reset the indicator bit that it is a PUBLIC AOR*/
                       ICF_RESET_BIT(p_reg_event_subs_ctx->p_rgm_context->
                               indicator,ICF_RGM_PUBLIC_AOR_RECEIVED);
                       
                       /*SPR 20026 Reset the indicator bit that it is a Associated URI
                        * received*/
                       ICF_RESET_BIT(p_reg_event_subs_ctx->p_rgm_context->
                               indicator_1,ICF_RGM_ASSOCIATED_URI_RECEIVED);
                       
                       /*Reset the indicator bit that self contact received*/
                       ICF_RESET_BIT(p_reg_event_subs_ctx->p_rgm_context->
                               indicator,ICF_RGM_SELF_CONTACT_RECEIVED);
                       

                    }/*if (ICF_SUCCESS ==
                            icf_dbm_get_public_uri_node_from_uri..*/
                    
				}/*Else of if((ICF_NULL == p_regn_blk->p_aor) ||.. */
                
			}/*for (index_regn_blk = 0; index_re....*/
            
		}/*else block..*/ 

#if 0        
		/* if all contact are terminated then the rgm_context would be cleared
		   further if the last rgm_context is being cleared then the subs_context
		   will also be cleared. The state of contact should only be checked ifthe
		   rgm_context has been updated in this notify processing otherwise no need
		   to check*/

		if(ICF_SUCCESS == ret_val)
		{
			ret_val = icf_rgm_verify_ctx_status(p_crm_pdb);
			/* processing for all contacts terminated for all reg_context */
		}
#endif   
        /*To avoid the case in which there is a network initiated
         * deregistration and we have alredy clear the coressponding reg
         * context and p_reg_event_subs_ctx*/
        if (ICF_NULL != p_reg_event_subs_ctx)
        {    
            /* handling the case of new version > old version + 1*/
            old_version = p_reg_event_subs_ctx->reg_info_version;
        
            /* Store the new version in rgm_context*/
            p_reg_event_subs_ctx->reg_info_version = recd_reg_info.version;

            /* If all contacts have been terminated along
             * with the auto_reg contact, then clear the 
             * rgm_context */
        
            if ((p_reg_event_subs_ctx->reg_info_version >
                old_version + 1))
            {
                /*Since the version number received in notify body is more
                 * than 1+stored version number , we will create a new
                 * subscription dialog for the reg event package .*/
                 
                /*Before creating the new subscription dialog we will clear
                 * the memory allocated for sic_context and reg_event_subs_
                 * context in regm_context.For that we will set the 
                 * recd_con_app_id in crm_pdb as ICF_APP_ID_INVALID ,Now in
                 * function icf_rgm_send_clear_subs_on_dereg this is treated
                 * as network initiated deregistration and therefore
                 * function call to icf_sic_process_mesg clear the sic context and
                 * in the retun path of icf_rgm_send_clear_subs_on_dereg we
                 * also clear the memory allocated for reg_event_subs_context */
                 
                p_crm_pdb->recd_con_app_id = ICF_APP_ID_INVALID;
                
                ret_val = icf_rgm_send_clear_subs_on_dereg(
                                     p_crm_pdb,
                                     p_crm_pdb->p_ecode);
                
                /* A refresh SUSCRIBE has to go */
                /*Send new subscribe request for reg event package only when
                 * the reg_event_subs_context and sic context has successfully
                 *deallocated */
                 
                if (ICF_SUCCESS == ret_val)
                {
                    if (ICF_FAILURE ==  icf_rgm_send_subs_on_succ(
                        p_crm_pdb, p_crm_pdb->p_ecode))
                    {
                        /* Unable to send a REFRESH SUBSCRIBE request */                    
                    }
                }   
            }
        }    
        /* We would free the regn_block_list irrespective of a failure or a 
         * success in parsing the xml body.
         * For eg :
         * If noe reginfo element is found,the function
         * icf_rgm_validate_rgm_info_version_state sets 
         * failure and if the regn_block_list is not freed,
         * it would lead to memory leaks.
         */
        icf_rgm_free_regn_list(p_crm_pdb->p_glb_pdb, 
                            &(recd_reg_info.registration_list),p_crm_pdb->p_ecode);
        
    }/* Msg body found for reg_event: ends */
	}/* if ICF_NULL != body.count */

	ret_val = ICF_SUCCESS;/*always success*/

/* End: Multiple Public Users ICF5.0*/

    /* function exit trace */
    ICF_FUNCTION_EXIT(p_crm_pdb->p_internal_msg->msg_hdr.p_glb_pdb)
    return ret_val;
}/* Function icf_rgm_process_notify() ends here */


/*******************************************************************************
 *
 * FUNCTION:        icf_rgm_aka_auth_resp_hdlr
 *
 * DESCRIPTION:     This function finds the reg context for which the
 *                  authentication response has been received and calls 
 *                  the fsm handler.nn
 *                  
 * RETURNS:         ICF_SUCCESS in case of success and ICF_FAILURE in
 *                  case of failure.
 *
 *****************************************************************************/
icf_return_t icf_rgm_aka_auth_resp_hdlr (
                                INOUT	icf_crm_pdb_st *p_crm_pdb)
{

    icf_return_t				ret_val = ICF_SUCCESS;
    icf_rgm_context_st			*p_rgm_context = ICF_NULL;
    
    icf_aka_auth_resp_st	*p_recv_msg_payload = ICF_NULL;

    icf_line_data_st			*p_line_data = ICF_NULL;

    ICF_SET_MODULE(p_crm_pdb->p_internal_msg->msg_hdr.p_glb_pdb,
                        ICF_INT_MODULE_REGM)

    /*function entry trace*/ 
    ICF_FUNCTION_ENTER(p_crm_pdb->p_internal_msg->msg_hdr.p_glb_pdb)

    /*update global pdb*/
    p_crm_pdb->p_glb_pdb = p_crm_pdb->p_internal_msg->msg_hdr.p_glb_pdb;
    p_crm_pdb->error_cause = ICF_NULL;

    p_crm_pdb->p_ecode = &(p_crm_pdb->p_internal_msg->msg_hdr.ecode);

    /*fetch CFG module data*/
    ret_val = icf_dbm_get_module_glb_data(
                p_crm_pdb->p_glb_pdb,
                (icf_int_module_id_t)(ICF_INT_MODULE_CFG),
                (icf_void_t **)(&(p_crm_pdb->p_config_data)),
                p_crm_pdb->p_ecode);     

    ICF_ASSERT(p_crm_pdb->p_glb_pdb,
                            (ICF_NULL != p_crm_pdb->p_config_data))
   
    if(ICF_SUCCESS == ret_val)
	{
        /*fetch REGM module data*/
        ret_val = icf_dbm_get_module_glb_data(
                p_crm_pdb->p_glb_pdb,
                (icf_int_module_id_t)(ICF_INT_MODULE_REGM),
                (icf_void_t **)(&(p_crm_pdb->p_rgm_glb_data)),
                p_crm_pdb->p_ecode);     

        ICF_ASSERT(p_crm_pdb->p_glb_pdb,
                (ICF_NULL != p_crm_pdb->p_rgm_glb_data))
    }

    if(ICF_SUCCESS == ret_val)
    {
        /*fetch message payload*/
        p_recv_msg_payload = (icf_aka_auth_resp_st *)
                                    (p_crm_pdb->p_recv_payload);
    }

    if(ICF_SUCCESS == ret_val)
	{
        /*fetch line data*/
        ret_val = icf_dbm_fetch_line_cntxt_blk(
                p_crm_pdb->p_glb_pdb, p_recv_msg_payload->line_id,
                &p_line_data, p_crm_pdb->p_ecode);
    }

	if(ICF_SUCCESS == ret_val)
	{
	    if (ICF_FAILURE == icf_dbm_fetch_rgn_blk(p_crm_pdb->p_glb_pdb,
                    & p_recv_msg_payload->user_address,p_recv_msg_payload->line_id
                    ,&p_rgm_context,p_crm_pdb->p_ecode))
	    {
			*(p_crm_pdb->p_ecode) = ICF_NO_RGN_CTX_EXIST;
            ret_val = icf_cmn_send_pa_unexpected_msg_ind(p_crm_pdb->p_glb_pdb,
                        p_crm_pdb->p_glb_pdb->p_recv_msg,p_crm_pdb->p_ecode);
            ret_val = ICF_FAILURE;
	    } /*rgm context could not be obtained*/

	    else    /*if rgm context found*/
	    {
            if(p_rgm_context->line_id == p_recv_msg_payload->line_id)
            {
                p_crm_pdb->p_rgm_context = p_rgm_context;
                p_crm_pdb->error_cause = ICF_NULL;

                /* Set the default value of security_association_reqd as
                 * ICF_FALSE if application does not send this in API
                 * ICF_AKA_AUTH_RESP. 
                 */
                if(ICF_NULL == (p_recv_msg_payload->bit_mask &
                         ICF_AKA_AUTH_SECURITY_ASSOCIATION_REQD_PRESENT))
                {
                   p_recv_msg_payload->security_association_reqd = ICF_FALSE;
                }
                /* Process the security association based on received
                 * security_association_reqd value. 
                 */ 
                if(ICF_FAILURE ==
                        icf_rgm_process_security_association(p_crm_pdb,
                          p_recv_msg_payload->security_association_reqd))
                {
                    ret_val = ICF_FAILURE;
                }
    		    /* Call the FSM handler */
				else if ( p_rgm_context->rgm_state < ICF_RGM_MAX_STATES) 
				{
		        	ret_val = ((icf_rgm_glb_data_st *)(p_crm_pdb->p_rgm_glb_data))->
                        icf_rgm_state_machine[p_rgm_context->rgm_state] 
                        [ICF_RGM_AUTH_RESP](p_crm_pdb);
				}
            }/*End if*/
            else
            {
			    *(p_crm_pdb->p_ecode) = ICF_CAUSE_INVALID_LINE_ID;
                ret_val = icf_cmn_send_pa_unexpected_msg_ind(p_crm_pdb->p_glb_pdb,
                        p_crm_pdb->p_glb_pdb->p_recv_msg,p_crm_pdb->p_ecode);
                ret_val = ICF_FAILURE;
            }
	    }/*End else*/
    }

    /*Function Exit Traces macro and reset module macro*/

    ICF_FUNCTION_EXIT(p_crm_pdb->p_glb_pdb) 
    ICF_RESET_MODULE(p_crm_pdb->p_internal_msg->msg_hdr.p_glb_pdb)
    return ret_val;
}
